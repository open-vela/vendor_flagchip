/****************************************************************************
 * Copyright (C) 2025 Xiaomi Corporation
 *
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.  The
 * ASF licenses this file to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance with the
 * License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the
 * License for the specific language governing permissions and limitations
 * under the License.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <debug.h>
#include <errno.h>

#include <nuttx/irq.h>
#include <nuttx/arch.h>
#include <nuttx/queue.h>
#include <nuttx/spinlock.h>
#include <nuttx/mutex.h>
#include <nuttx/semaphore.h>

#include "arm_internal.h"
#include "sched/sched.h"

#include "chip.h"
#include "fc7300_fcsmu.h"
#include "fc7300_fcsmu_user.h"
#include "hardware/fc7300_fcsmu.h"
#include "hardware/fc7300_pin.h"
#include "hardware/fc7300_pinmux.h"
#include "fc7300_multicore.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#define FCSMU_CHANNEL_SOFTWARE_TRIGGER_ERROR  18U

#define FCSMU_OP_CODE_NONE                    0U      /* FCSMU operation code none */
#define FCSMU_OP_CODE_MOVE_TO_CONFIG          1U      /* FCSMU operation code move to configuration state */
#define FCSMU_OP_CODE_MOVE_TO_NORMAL          2U      /* FCSMU operation code move to normal state */
#define FCSMU_OP_CODE_CLEAR_FAULT_INFO        13U     /* FCSMU operation code clear fault channel information register */
#define FCSMU_OP_CODE_CLEAR_OPS_TO_IDLE       15U     /* FCSMU operation code clear ops to idle */

#define FCSMU_OP_STATE_IDLE                   0U      /* FCSMU operation status idle */
#define FCSMU_OP_STATE_BUSY                   1U      /* FCSMU operation status busy */
#define FCSMU_OP_STATE_FAILED                 2U      /* FCSMU operation status failed */
#define FCSMU_OP_STATE_SUCCESSFUL             3U      /* FCSMU operation status successful */

#define FCSMU_STATE_NORMAL                    0U      /* FCSMU state normal */
#define FCSMU_STATE_CONGIG                    1U      /* FCSMU state config */
#define FCSMU_STATE_WARN                      2U      /* FCSMU state warn */
#define FCSMU_STATE_FAULT                     3U      /* FCSMU state fault */

#define FCSMU_CRC_STATE_IDLE                  0U      /* FCSMU crc status idle */
#define FCSMU_CRC_STATE_BUSY                  1U      /* FCSMU crc status busy */

#define FCSMU_ERROR_TYPE_FAULT                0x1U
#define FCSMU_ERROR_TYPE_WARNING              0x2U
#define FCSMU_ERROR_TYPE_TIMEOUT              0x3U

#define FCSMU_FST_UNLOCK_KEY                  0x951413CFUL
#define FCSMU_OPC1_UNLOCK_KEY                 0xFC2020CFUL
#define FCSMU_OPC2_UNLOCK_KEY                 0x20FCCF20UL
#define FCSMU_CONFIG_TMEP_UNLOCK_KEY          0xFCU
#define FCSMU_CONFIG_TMEP_LOCK_KEY            0xFFU

/****************************************************************************
 * Private Types
 ****************************************************************************/

/* FCSMU Device Private Data */

struct fc7300_fcsmu_config_s
{
  uint32_t reg_base;                      /* FCSMU register base address */
  uint32_t irq;                           /* Interrupt ID */
  uint32_t irq_priority;                  /* Interrupt priority */
  uint32_t CFG_TO;                        /* Configuration State Timeout Interval Register */
  uint32_t CFG_TO_IEN;                    /* Configuration State Timeout IRQ Enable Register */
  uint32_t FE;                            /* Fault Enable Register */
  uint32_t FAULT_IEN;                     /* Fault State Interrupt Enable Register */
  uint32_t FCCR;                          /* Fault Clear Control Register 0 */
  uint32_t FRST;                          /* Fault Reset Control Register 0 */
  uint32_t WARNING_TO;                    /* Warning Timeout Interval Register */
  uint32_t WARNING_EN;                    /* Warning State Enable Register 0 */
  uint32_t WARNING_IEN;                   /* Warning State Interrupt Enable Register */
  uint32_t SOCTRL;                        /* Status Output Control Register */
  uint32_t SOUT_DIAG;                     /* SOUT Diagnostic Register */
  uint32_t STMR;                          /* SOUT Timer Interval Register */
  uint32_t SOUT_EN;                       /* SOUT Enable Register */
  uint32_t CRC_CTRL;                      /* CRC Control Register */
};

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

static inline uint32_t
fc7300_fcsmu_getreg(struct fc7300_fcsmu_config_s *priv, uint16_t offset);
static inline void fc7300_fcsmu_putreg(struct fc7300_fcsmu_config_s *priv,
                                     uint16_t offset, uint32_t value);
static int fc7300_fcsmu_isr(int irq, void *context, void *arg);

static int fc7300_fcsmu_normal_to_config(struct fc7300_fcsmu_config_s *priv);
static int fc7300_fcsmu_config_to_normal(struct fc7300_fcsmu_config_s *priv);

/****************************************************************************
 * Private Data
 ****************************************************************************/

static uint32_t por_fst = 0U;             /* Record fst status when system reset */

static const struct fc7300_fcsmu_config_s g_fcsmu_config =
{
  .reg_base     = FC7300_FCSMU_BASE,
  .irq          = FC7300_IRQ_FCSMU,
  .irq_priority = NVIC_FCSMU_PRIORITY_DEFAULT,
  .CFG_TO       =
#if defined(CONFIG_FC7300_FCSMU_CFG_TIMEOUT_MODE0)
                  FCSMU_CFG_TO_TIME(0x0U),
#endif /* defined(CONFIG_FC7300_FCSMU_CFG_TIMEOUT_MODE0) */
#if defined(CONFIG_FC7300_FCSMU_CFG_TIMEOUT_MODE1)
                  FCSMU_CFG_TO_TIME(0x1U),
#endif /* defined(CONFIG_FC7300_FCSMU_CFG_TIMEOUT_MODE1) */
#if defined(CONFIG_FC7300_FCSMU_CFG_TIMEOUT_MODE2)
                  FCSMU_CFG_TO_TIME(0x2U),
#endif /* defined(CONFIG_FC7300_FCSMU_CFG_TIMEOUT_MODE2) */
#if defined(CONFIG_FC7300_FCSMU_CFG_TIMEOUT_MODE3)
                  FCSMU_CFG_TO_TIME(0x3U),
#endif /* defined(CONFIG_FC7300_FCSMU_CFG_TIMEOUT_MODE3) */
#if defined(CONFIG_FC7300_FCSMU_CFG_TIMEOUT_MODE4)
                  FCSMU_CFG_TO_TIME(0x4U),
#endif /* defined(CONFIG_FC7300_FCSMU_CFG_TIMEOUT_MODE4) */
#if defined(CONFIG_FC7300_FCSMU_CFG_TIMEOUT_MODE5)
                  FCSMU_CFG_TO_TIME(0x5U),
#endif /* defined(CONFIG_FC7300_FCSMU_CFG_TIMEOUT_MODE5) */
#if defined(CONFIG_FC7300_FCSMU_CFG_TIMEOUT_MODE6)
                  FCSMU_CFG_TO_TIME(0x6U),
#endif /* defined(CONFIG_FC7300_FCSMU_CFG_TIMEOUT_MODE6) */
#if defined(CONFIG_FC7300_FCSMU_CFG_TIMEOUT_MODE7)
                  FCSMU_CFG_TO_TIME(0x7U),
#endif /* defined(CONFIG_FC7300_FCSMU_CFG_TIMEOUT_MODE7) */
  .CFG_TO_IEN   = 0x0U |
#if defined(CONFIG_FC7300_FCSMU_CFG_TIMEOUT_INTERRUPT_ENABLE)
                  FCSMU_IRQ_EN_CFG_TO_IEN |
#endif
                  0x0U,
  .FE           = 0x0U |
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_TEMP_ERROR)
                  0x1U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_PMC_ERROR)
                  0x2U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_NVR_ERROR)
                  0x4U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_STCU_BIST_FAIL)
                  0x8U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_LS0_ERR)
                  0x10U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_LS1_ERR)
                  0x20U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_SYSRAM_CPU0_ERR)
                  0x40U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_SYSRAM_CPU1_ERR)
                  0x80U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_SYSRAM_CPU2_ERR)
                  0x100U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_SYSRAM_NON_CPU_ERR)
                  0x200U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_SCM_CPU0_ERR)
                  0x400U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_SCM_CPU1_ERR)
                  0x800U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_SCM_CPU2_ERR)
                  0x1000U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_SCM_NON_CPU_ERR)
                  0x2000U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_CPU0_ECC_ERR)
                  0x4000U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_CPU1_ECC_ERR)
                  0x8000U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_CPU2_ECC_ERR)
                  0x10000U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_SYSTEM_ABNORMAL_ALARM)
                  0x20000U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_SOFTWARE)
                  0x40000U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_CMU4_FAIL)
                  0x80000U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_CMU1_2_FAIL)
                  0x100000U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_FLASH_ECC)
                  0x200000U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_SCG_PLL_FOSC)
                  0x400000U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_DMA0_ERR)
                  0x800000U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_DMA1_ERR)
                  0x1000000U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_INTM0_ERR)
                  0x2000000U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_INTM1_ERR)
                  0x4000000U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_INTM2_ERR)
                  0x8000000U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_FMC_ERR)
                  0x10000000U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_SCG_CRC)
                  0x20000000U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_SCM_CRC)
                  0x40000000U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_MAM_WDOG_TIMEOUT)
                  0x80000000U |
#endif
                  0x0U,
  .FAULT_IEN    = 0x0U |
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_TEMP_ERROR_FAULT_INTERRUPT_ENABLE)
                  0x1U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_PMC_ERROR_FAULT_INTERRUPT_ENABLE)
                  0x2U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_NVR_ERROR_FAULT_INTERRUPT_ENABLE)
                  0x4U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_STCU_BIST_FAIL_FAULT_INTERRUPT_ENABLE)
                  0x8U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_LS0_ERR_FAULT_INTERRUPT_ENABLE)
                  0x10U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_LS1_ERR_FAULT_INTERRUPT_ENABLE)
                  0x20U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_SYSRAM_CPU0_ERR_FAULT_INTERRUPT_ENABLE)
                  0x40U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_SYSRAM_CPU1_ERR_FAULT_INTERRUPT_ENABLE)
                  0x80U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_SYSRAM_CPU2_ERR_FAULT_INTERRUPT_ENABLE)
                  0x100U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_SYSRAM_NON_CPU_ERR_FAULT_INTERRUPT_ENABLE)
                  0x200U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_SCM_CPU0_ERR_FAULT_INTERRUPT_ENABLE)
                  0x400U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_SCM_CPU1_ERR_FAULT_INTERRUPT_ENABLE)
                  0x800U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_SCM_CPU2_ERR_FAULT_INTERRUPT_ENABLE)
                  0x1000U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_SCM_NON_CPU_ERR_FAULT_INTERRUPT_ENABLE)
                  0x2000U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_CPU0_ECC_ERR_FAULT_INTERRUPT_ENABLE)
                  0x4000U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_CPU1_ECC_ERR_FAULT_INTERRUPT_ENABLE)
                  0x8000U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_CPU2_ECC_ERR_FAULT_INTERRUPT_ENABLE)
                  0x10000U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_SYSTEM_ABNORMAL_ALARM_FAULT_INTERRUPT_ENABLE)
                  0x20000U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_SOFTWARE_FAULT_INTERRUPT_ENABLE)
                  0x40000U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_CMU4_FAIL_FAULT_INTERRUPT_ENABLE)
                  0x80000U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_CMU1_2_FAIL_FAULT_INTERRUPT_ENABLE)
                  0x100000U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_FLASH_ECC_FAULT_INTERRUPT_ENABLE)
                  0x200000U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_SCG_PLL_FOSC_FAULT_INTERRUPT_ENABLE)
                  0x400000U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_DMA0_ERR_FAULT_INTERRUPT_ENABLE)
                  0x800000U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_DMA1_ERR_FAULT_INTERRUPT_ENABLE)
                  0x1000000U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_INTM0_ERR_FAULT_INTERRUPT_ENABLE)
                  0x2000000U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_INTM1_ERR_FAULT_INTERRUPT_ENABLE)
                  0x4000000U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_INTM2_ERR_FAULT_INTERRUPT_ENABLE)
                  0x8000000U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_FMC_ERR_FAULT_INTERRUPT_ENABLE)
                  0x10000000U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_SCG_CRC_FAULT_INTERRUPT_ENABLE)
                  0x20000000U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_SCM_CRC_FAULT_INTERRUPT_ENABLE)
                  0x40000000U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_MAM_WDOG_TIMEOUT_FAULT_INTERRUPT_ENABLE)
                  0x80000000U |
#endif
                  0x0U,
  .FCCR         = 0x0U |
#if !defined(CONFIG_FC7300_FCSMU_CHANNEL_TEMP_ERROR_AUTO_CLEAR)
                  0x1U |
#endif
#if !defined(CONFIG_FC7300_FCSMU_CHANNEL_PMC_ERROR_AUTO_CLEAR)
                  0x2U |
#endif
#if !defined(CONFIG_FC7300_FCSMU_CHANNEL_NVR_ERROR_AUTO_CLEAR)
                  0x4U |
#endif
#if !defined(CONFIG_FC7300_FCSMU_CHANNEL_STCU_BIST_FAIL_AUTO_CLEAR)
                  0x8U |
#endif
#if !defined(CONFIG_FC7300_FCSMU_CHANNEL_LS0_ERR_AUTO_CLEAR)
                  0x10U |
#endif
#if !defined(CONFIG_FC7300_FCSMU_CHANNEL_LS1_ERR_AUTO_CLEAR)
                  0x20U |
#endif
#if !defined(CONFIG_FC7300_FCSMU_CHANNEL_SYSRAM_CPU0_ERR_AUTO_CLEAR)
                  0x40U |
#endif
#if !defined(CONFIG_FC7300_FCSMU_CHANNEL_SYSRAM_CPU1_ERR_AUTO_CLEAR)
                  0x80U |
#endif
#if !defined(CONFIG_FC7300_FCSMU_CHANNEL_SYSRAM_CPU2_ERR_AUTO_CLEAR)
                  0x100U |
#endif
#if !defined(CONFIG_FC7300_FCSMU_CHANNEL_SYSRAM_NON_CPU_ERR_AUTO_CLEAR)
                  0x200U |
#endif
#if !defined(CONFIG_FC7300_FCSMU_CHANNEL_SCM_CPU0_ERR_AUTO_CLEAR)
                  0x400U |
#endif
#if !defined(CONFIG_FC7300_FCSMU_CHANNEL_SCM_CPU1_ERR_AUTO_CLEAR)
                  0x800U |
#endif
#if !defined(CONFIG_FC7300_FCSMU_CHANNEL_SCM_CPU2_ERR_AUTO_CLEAR)
                  0x1000U |
#endif
#if !defined(CONFIG_FC7300_FCSMU_CHANNEL_SCM_NON_CPU_ERR_AUTO_CLEAR)
                  0x2000U |
#endif
#if !defined(CONFIG_FC7300_FCSMU_CHANNEL_CPU0_ECC_ERR_AUTO_CLEAR)
                  0x4000U |
#endif
#if !defined(CONFIG_FC7300_FCSMU_CHANNEL_CPU1_ECC_ERR_AUTO_CLEAR)
                  0x8000U |
#endif
#if !defined(CONFIG_FC7300_FCSMU_CHANNEL_CPU2_ECC_ERR_AUTO_CLEAR)
                  0x10000U |
#endif
#if !defined(CONFIG_FC7300_FCSMU_CHANNEL_SYSTEM_ABNORMAL_ALARM_AUTO_CLEAR)
                  0x20000U |
#endif
#if !defined(CONFIG_FC7300_FCSMU_CHANNEL_SOFTWARE_AUTO_CLEAR)
                  0x40000U |
#endif
#if !defined(CONFIG_FC7300_FCSMU_CHANNEL_CMU4_FAIL_AUTO_CLEAR)
                  0x80000U |
#endif
#if !defined(CONFIG_FC7300_FCSMU_CHANNEL_CMU1_2_FAIL_AUTO_CLEAR)
                  0x100000U |
#endif
#if !defined(CONFIG_FC7300_FCSMU_CHANNEL_FLASH_ECC_AUTO_CLEAR)
                  0x200000U |
#endif
#if !defined(CONFIG_FC7300_FCSMU_CHANNEL_SCG_PLL_FOSC_AUTO_CLEAR)
                  0x400000U |
#endif
#if !defined(CONFIG_FC7300_FCSMU_CHANNEL_DMA0_ERR_AUTO_CLEAR)
                  0x800000U |
#endif
#if !defined(CONFIG_FC7300_FCSMU_CHANNEL_DMA1_ERR_AUTO_CLEAR)
                  0x1000000U |
#endif
#if !defined(CONFIG_FC7300_FCSMU_CHANNEL_INTM0_ERR_AUTO_CLEAR)
                  0x2000000U |
#endif
#if !defined(CONFIG_FC7300_FCSMU_CHANNEL_INTM1_ERR_AUTO_CLEAR)
                  0x4000000U |
#endif
#if !defined(CONFIG_FC7300_FCSMU_CHANNEL_INTM2_ERR_AUTO_CLEAR)
                  0x8000000U |
#endif
#if !defined(CONFIG_FC7300_FCSMU_CHANNEL_FMC_ERR_AUTO_CLEAR)
                  0x10000000U |
#endif
#if !defined(CONFIG_FC7300_FCSMU_CHANNEL_SCG_CRC_AUTO_CLEAR)
                  0x20000000U |
#endif
#if !defined(CONFIG_FC7300_FCSMU_CHANNEL_SCM_CRC_AUTO_CLEAR)
                  0x40000000U |
#endif
#if !defined(CONFIG_FC7300_FCSMU_CHANNEL_MAM_WDOG_TIMEOUT_AUTO_CLEAR)
                  0x80000000U |
#endif
                  0x0U,
  .FRST         = 0x0U |
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_TEMP_ERROR_TRIGGER_RESET)
                  0x1U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_PMC_ERROR_TRIGGER_RESET)
                  0x2U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_NVR_ERROR_TRIGGER_RESET)
                  0x4U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_STCU_BIST_FAIL_TRIGGER_RESET)
                  0x8U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_LS0_ERR_TRIGGER_RESET)
                  0x10U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_LS1_ERR_TRIGGER_RESET)
                  0x20U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_SYSRAM_CPU0_ERR_TRIGGER_RESET)
                  0x40U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_SYSRAM_CPU1_ERR_TRIGGER_RESET)
                  0x80U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_SYSRAM_CPU2_ERR_TRIGGER_RESET)
                  0x100U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_SYSRAM_NON_CPU_ERR_TRIGGER_RESET)
                  0x200U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_SCM_CPU0_ERR_TRIGGER_RESET)
                  0x400U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_SCM_CPU1_ERR_TRIGGER_RESET)
                  0x800U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_SCM_CPU2_ERR_TRIGGER_RESET)
                  0x1000U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_SCM_NON_CPU_ERR_TRIGGER_RESET)
                  0x2000U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_CPU0_ECC_ERR_TRIGGER_RESET)
                  0x4000U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_CPU1_ECC_ERR_TRIGGER_RESET)
                  0x8000U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_CPU2_ECC_ERR_TRIGGER_RESET)
                  0x10000U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_SYSTEM_ABNORMAL_ALARM_TRIGGER_RESET)
                  0x20000U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_SOFTWARE_TRIGGER_RESET)
                  0x40000U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_CMU4_FAIL_TRIGGER_RESET)
                  0x80000U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_CMU1_2_FAIL_TRIGGER_RESET)
                  0x100000U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_FLASH_ECC_TRIGGER_RESET)
                  0x200000U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_SCG_PLL_FOSC_TRIGGER_RESET)
                  0x400000U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_DMA0_ERR_TRIGGER_RESET)
                  0x800000U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_DMA1_ERR_TRIGGER_RESET)
                  0x1000000U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_INTM0_ERR_TRIGGER_RESET)
                  0x2000000U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_INTM1_ERR_TRIGGER_RESET)
                  0x4000000U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_INTM2_ERR_TRIGGER_RESET)
                  0x8000000U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_FMC_ERR_TRIGGER_RESET)
                  0x10000000U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_SCG_CRC_TRIGGER_RESET)
                  0x20000000U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_SCM_CRC_TRIGGER_RESET)
                  0x40000000U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_MAM_WDOG_TIMEOUT_TRIGGER_RESET)
                  0x80000000U |
#endif
                  0x0U,
  .WARNING_TO   = CONFIG_FC7300_FCSMU_WARNING_TIMEOUT * 12,
  .WARNING_EN   = 0x0U |
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_TEMP_ERROR_WARNING_ENABLE)
                  0x1U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_PMC_ERROR_WARNING_ENABLE)
                  0x2U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_NVR_ERROR_WARNING_ENABLE)
                  0x4U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_STCU_BIST_FAIL_WARNING_ENABLE)
                  0x8U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_LS0_ERR_WARNING_ENABLE)
                  0x10U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_LS1_ERR_WARNING_ENABLE)
                  0x20U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_SYSRAM_CPU0_ERR_WARNING_ENABLE)
                  0x40U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_SYSRAM_CPU1_ERR_WARNING_ENABLE)
                  0x80U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_SYSRAM_CPU2_ERR_WARNING_ENABLE)
                  0x100U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_SYSRAM_NON_CPU_ERR_WARNING_ENABLE)
                  0x200U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_SCM_CPU0_ERR_WARNING_ENABLE)
                  0x400U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_SCM_CPU1_ERR_WARNING_ENABLE)
                  0x800U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_SCM_CPU2_ERR_WARNING_ENABLE)
                  0x1000U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_SCM_NON_CPU_ERR_WARNING_ENABLE)
                  0x2000U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_CPU0_ECC_ERR_WARNING_ENABLE)
                  0x4000U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_CPU1_ECC_ERR_WARNING_ENABLE)
                  0x8000U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_CPU2_ECC_ERR_WARNING_ENABLE)
                  0x10000U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_SYSTEM_ABNORMAL_ALARM_WARNING_ENABLE)
                  0x20000U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_SOFTWARE_WARNING_ENABLE)
                  0x40000U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_CMU4_FAIL_WARNING_ENABLE)
                  0x80000U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_CMU1_2_FAIL_WARNING_ENABLE)
                  0x100000U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_FLASH_ECC_WARNING_ENABLE)
                  0x200000U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_SCG_PLL_FOSC_WARNING_ENABLE)
                  0x400000U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_DMA0_ERR_WARNING_ENABLE)
                  0x800000U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_DMA1_ERR_WARNING_ENABLE)
                  0x1000000U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_INTM0_ERR_WARNING_ENABLE)
                  0x2000000U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_INTM1_ERR_WARNING_ENABLE)
                  0x4000000U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_INTM2_ERR_WARNING_ENABLE)
                  0x8000000U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_FMC_ERR_WARNING_ENABLE)
                  0x10000000U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_SCG_CRC_WARNING_ENABLE)
                  0x20000000U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_SCM_CRC_WARNING_ENABLE)
                  0x40000000U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_MAM_WDOG_TIMEOUT_WARNING_ENABLE)
                  0x80000000U |
#endif
                  0x0U,
  .WARNING_IEN  = 0x0U |
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_TEMP_ERROR_WARNING_INTERRUPT_ENABLE)
                  0x1U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_PMC_ERROR_WARNING_INTERRUPT_ENABLE)
                  0x2U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_NVR_ERROR_WARNING_INTERRUPT_ENABLE)
                  0x4U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_STCU_BIST_FAIL_WARNING_INTERRUPT_ENABLE)
                  0x8U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_LS0_ERR_WARNING_INTERRUPT_ENABLE)
                  0x10U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_LS1_ERR_WARNING_INTERRUPT_ENABLE)
                  0x20U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_SYSRAM_CPU0_ERR_WARNING_INTERRUPT_ENABLE)
                  0x40U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_SYSRAM_CPU1_ERR_WARNING_INTERRUPT_ENABLE)
                  0x80U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_SYSRAM_CPU2_ERR_WARNING_INTERRUPT_ENABLE)
                  0x100U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_SYSRAM_NON_CPU_ERR_WARNING_INTERRUPT_ENABLE)
                  0x200U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_SCM_CPU0_ERR_WARNING_INTERRUPT_ENABLE)
                  0x400U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_SCM_CPU1_ERR_WARNING_INTERRUPT_ENABLE)
                  0x800U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_SCM_CPU2_ERR_WARNING_INTERRUPT_ENABLE)
                  0x1000U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_SCM_NON_CPU_ERR_WARNING_INTERRUPT_ENABLE)
                  0x2000U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_CPU0_ECC_ERR_WARNING_INTERRUPT_ENABLE)
                  0x4000U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_CPU1_ECC_ERR_WARNING_INTERRUPT_ENABLE)
                  0x8000U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_CPU2_ECC_ERR_WARNING_INTERRUPT_ENABLE)
                  0x10000U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_SYSTEM_ABNORMAL_ALARM_WARNING_INTERRUPT_ENABLE)
                  0x20000U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_SOFTWARE_WARNING_INTERRUPT_ENABLE)
                  0x40000U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_CMU4_FAIL_WARNING_INTERRUPT_ENABLE)
                  0x80000U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_CMU1_2_FAIL_WARNING_INTERRUPT_ENABLE)
                  0x100000U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_FLASH_ECC_WARNING_INTERRUPT_ENABLE)
                  0x200000U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_SCG_PLL_FOSC_WARNING_INTERRUPT_ENABLE)
                  0x400000U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_DMA0_ERR_WARNING_INTERRUPT_ENABLE)
                  0x800000U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_DMA1_ERR_WARNING_INTERRUPT_ENABLE)
                  0x1000000U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_INTM0_ERR_WARNING_INTERRUPT_ENABLE)
                  0x2000000U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_INTM1_ERR_WARNING_INTERRUPT_ENABLE)
                  0x4000000U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_INTM2_ERR_WARNING_INTERRUPT_ENABLE)
                  0x8000000U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_FMC_ERR_WARNING_INTERRUPT_ENABLE)
                  0x10000000U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_SCG_CRC_WARNING_INTERRUPT_ENABLE)
                  0x20000000U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_SCM_CRC_WARNING_INTERRUPT_ENABLE)
                  0x40000000U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_MAM_WDOG_TIMEOUT_WARNING_INTERRUPT_ENABLE)
                  0x80000000U |
#endif
                  0x0U,
  .SOCTRL       = 0x0U |
#if defined(CONFIG_FC7300_FCSMU_SOUT_PINS_ENABLE)
                  FCSMU_SOCTRL_SOUT_PEN |
#if defined(CONFIG_FC7300_FCSMU_SOUT_CTRL_MODE0)
                  FCSMU_SOCTRL_SOUT_CTRL(0U) |
#endif
#if defined(CONFIG_FC7300_FCSMU_SOUT_CTRL_MODE1)
                  FCSMU_SOCTRL_SOUT_CTRL(1U) |
#endif
#if defined(CONFIG_FC7300_FCSMU_SOUT_CTRL_MODE2)
                  FCSMU_SOCTRL_SOUT_CTRL(2U) |
#endif
#if defined(CONFIG_FC7300_FCSMU_SOUT_CTRL_MODE3)
                  FCSMU_SOCTRL_SOUT_CTRL(3U) |
#endif
                  FCSMU_SOCTRL_SMRDT(1U << CONFIG_FC7300_FCSMU_SOUT_SMRDT) |
#if defined(CONFIG_FC7300_FCSMU_SOUT_FASTMODE_ENABLE)
                  FCSMU_SOCTRL_FASTEN |
#endif
#if defined(CONFIG_FC7300_FCSMU_SOUT_PINS_ENABLE)
                  FCSMU_SOCTRL_SOUT_PEN |
#endif
#if defined(CONFIG_FC7300_FCSMU_SOUT_POLARITY_SWITCH)
                  FCSMU_SOCTRL_POLSW |
#endif
#if defined(CONFIG_FC7300_FCSMU_SOUT_PROTOCOL_MODE0)
                  FCSMU_SOCTRL_SOUT_PTC(0U) |
#endif
#if defined(CONFIG_FC7300_FCSMU_SOUT_PROTOCOL_MODE1)
                  FCSMU_SOCTRL_SOUT_PTC(1U) |
#endif
#if defined(CONFIG_FC7300_FCSMU_SOUT_PROTOCOL_MODE2)
                  FCSMU_SOCTRL_SOUT_PTC(2U) |
#endif
#if defined(CONFIG_FC7300_FCSMU_SOUT_PROTOCOL_MODE3)
                  FCSMU_SOCTRL_SOUT_PTC(3U) |
#endif
#if defined(CONFIG_FC7300_FCSMU_SOUT_PROTOCOL_MODE4)
                  FCSMU_SOCTRL_SOUT_PTC(4U) |
#endif
#if defined(CONFIG_FC7300_FCSMU_SOUT_PROTOCOL_MODE5)
                  FCSMU_SOCTRL_SOUT_PTC(5U) |
#endif
#if defined(CONFIG_FC7300_FCSMU_SOUT_PROTOCOL_MODE6)
                  FCSMU_SOCTRL_SOUT_PTC(6U) |
#endif
#if defined(CONFIG_FC7300_FCSMU_SOUT_PROTOCOL_MODE7)
                  FCSMU_SOCTRL_SOUT_PTC(7U) |
#endif
                  FCSMU_SOCTRL_SOUT_DIV(CONFIG_FC7300_FCSMU_SOUT_DIVIDER_RATIO) |
#endif /* defined(CONFIG_FC7300_FCSMU_SOUT_PINS_ENABLE) */
                  0x0U,
  .SOUT_DIAG    = 0x0U |
#if defined(CONFIG_FC7300_FCSMU_SOUT_OUTPUT0)
                  FCSMU_SOUT_DIAG_SOUT0 |
#endif
#if defined(CONFIG_FC7300_FCSMU_SOUT_OUTPUT1)
                  FCSMU_SOUT_DIAG_SOUT1 |
#endif
                  0x0U,
  .STMR         = 0x0U |
#if defined(CONFIG_FC7300_FCSMU_MINI_TIME)
                  CONFIG_FC7300_FCSMU_MINI_TIME |
#endif
                  0x0U,
  .SOUT_EN      = 0x0U |
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_TEMP_ERROR_SOUT_EN)
                  0x1U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_PMC_ERROR_SOUT_EN)
                  0x2U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_NVR_ERROR_SOUT_EN)
                  0x4U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_STCU_BIST_FAIL_SOUT_EN)
                  0x8U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_LS0_ERR_SOUT_EN)
                  0x10U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_LS1_ERR_SOUT_EN)
                  0x20U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_SYSRAM_CPU0_ERR_SOUT_EN)
                  0x40U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_SYSRAM_CPU1_ERR_SOUT_EN)
                  0x80U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_SYSRAM_CPU2_ERR_SOUT_EN)
                  0x100U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_SYSRAM_NON_CPU_ERR_SOUT_EN)
                  0x200U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_SCM_CPU0_ERR_SOUT_EN)
                  0x400U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_SCM_CPU1_ERR_SOUT_EN)
                  0x800U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_SCM_CPU2_ERR_SOUT_EN)
                  0x1000U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_SCM_NON_CPU_ERR_SOUT_EN)
                  0x2000U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_CPU0_ECC_ERR_SOUT_EN)
                  0x4000U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_CPU1_ECC_ERR_SOUT_EN)
                  0x8000U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_CPU2_ECC_ERR_SOUT_EN)
                  0x10000U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_SYSTEM_ABNORMAL_ALARM_SOUT_EN)
                  0x20000U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_SOFTWARE_SOUT_EN)
                  0x40000U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_CMU4_FAIL_SOUT_EN)
                  0x80000U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_CMU1_2_FAIL_SOUT_EN)
                  0x100000U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_FLASH_ECC_SOUT_EN)
                  0x200000U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_SCG_PLL_FOSC_SOUT_EN)
                  0x400000U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_DMA0_ERR_SOUT_EN)
                  0x800000U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_DMA1_ERR_SOUT_EN)
                  0x1000000U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_INTM0_ERR_SOUT_EN)
                  0x2000000U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_INTM1_ERR_SOUT_EN)
                  0x4000000U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_INTM2_ERR_SOUT_EN)
                  0x8000000U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_FMC_ERR_SOUT_EN)
                  0x10000000U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_SCG_CRC_SOUT_EN)
                  0x20000000U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_SCM_CRC_SOUT_EN)
                  0x40000000U |
#endif
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_MAM_WDOG_TIMEOUT_SOUT_EN)
                  0x80000000U |
#endif
                  0x0U,
  .CRC_CTRL     = 0x0U |
#if defined(CONFIG_FC7300_FCSMU_CRC_ERROR_OUTPUT_ENABLE)
                  FCSMU_CRC_CTRL_EOEN |
#endif
#if defined(CONFIG_FC7300_FCSMU_CRC_CHECKER_ENABLE)
                  FCSMU_CRC_CTRL_CHKEN |
#endif
#if defined(CONFIG_FC7300_FCSMU_CRC_TRIGGER_ENABLE)
                  FCSMU_CRC_CTRL_TRGEN |
#endif
                  0x0U,
};

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: fc7300_fcsmu_getreg
 *
 * Description:
 *   Get a 32-bit register value by offset
 *
 * Input Parameters:
 *   priv   - fcsmu device private data
 *   offset - Register offset with respect to the base address of the FCSMU
 *            peripheral
 *
 * Returned Value:
 *   The 32-bit value retrieved from the register
 *
 ****************************************************************************/

static inline uint32_t
fc7300_fcsmu_getreg(struct fc7300_fcsmu_config_s *priv, uint16_t offset)
{
  return *(volatile uint32_t *)(priv->reg_base + offset);
}

/****************************************************************************
 * Name: fc7300_fcsmu_putreg
 *
 * Description:
 *  Put a 32-bit register value by offset
 *
 * Input Parameters:
 *   priv   - FCSMU device private data
 *   offset - Register offset with respect to the base address of the FCSMU
 *            peripheral
 *   value  - The 32-bit value that should be put into the register
 *
 ****************************************************************************/

static inline void fc7300_fcsmu_putreg(struct fc7300_fcsmu_config_s *priv,
                                     uint16_t offset, uint32_t value)
{
  *(volatile uint32_t *)(priv->reg_base + offset) = value;
}

/****************************************************************************
 * Name: fc7300_fcsmu_isr
 *
 * Description:
 *   Common FCSMU interrupt service routine
 *
 ****************************************************************************/

static int fc7300_fcsmu_isr(int irq, void *context, void *arg)
{
  struct fc7300_fcsmu_config_s *priv = (struct fc7300_fcsmu_config_s *)arg;
  uint32_t irq_stat;
  uint32_t irq_channel_mask;
  uint32_t nt_irq_channel;
  uint32_t ot_irq_channel;
  int ret;

  DEBUGASSERT(priv != NULL);

  irq_stat = fc7300_fcsmu_getreg(priv, FC7300_FCSMU_IRQ_STAT_OFFSET);
  irq_channel_mask = 0U;
  if ((irq_stat & FCSMU_IRQ_STAT_FAULT_IRQ) != 0)
    {
      nt_irq_channel = fc7300_fcsmu_getreg(priv, FC7300_FCSMU_NTF_OFFSET) &
                       FCSMU_NTF_INDEX_MASK;
      ot_irq_channel = fc7300_fcsmu_getreg(priv, FC7300_FCSMU_WTF_OFFSET) &
                       FCSMU_WTF_INDEX_MASK;

      if ((nt_irq_channel == FCSMU_NTF_INDEX_MASK) ||
          (ot_irq_channel == FCSMU_WTF_INDEX_MASK))
        {
          irq_channel_mask = fc7300_fcsmu_getreg(priv,
                                                 FC7300_FCSMU_FST0_OFFSET);
          irq_channel_mask &= fc7300_fcsmu_getreg(priv, FC7300_FCSMU_FE0_OFFSET);
        }
      else
        {
          if (nt_irq_channel > 0U && nt_irq_channel <= 32U)
            {
              irq_channel_mask |=
                (uint32_t)1U << (nt_irq_channel - (uint32_t)1U);
            }

          if (ot_irq_channel > 0U && ot_irq_channel <= 32U)
            {
              irq_channel_mask |=
                (uint32_t)1U << (ot_irq_channel - (uint32_t)1U);
            }
        }

      ret = fc7300_fcsmu_user_callback(irq_channel_mask,
                                       FCSMU_ERROR_TYPE_FAULT);
      if (ret == 0U)
        {
          fc7300_fcsmu_clear_fault(irq_channel_mask);
        }
    }
  else if ((irq_stat & FCSMU_IRQ_STAT_WARNING_IRQ) != 0)
    {
      nt_irq_channel =
        fc7300_fcsmu_getreg(priv, FC7300_FCSMU_NTW_OFFSET) &
        FCSMU_NTW_INDEX_MASK;
      ot_irq_channel =
        fc7300_fcsmu_getreg(priv, FC7300_FCSMU_FTW_OFFSET) &
        FCSMU_FTW_INDEX_MASK;

      if ((nt_irq_channel == FCSMU_NTW_INDEX_MASK) ||
          (ot_irq_channel == FCSMU_FTW_INDEX_MASK))
        {
          irq_channel_mask =
            fc7300_fcsmu_getreg(priv, FC7300_FCSMU_FST0_OFFSET);
            irq_channel_mask &= fc7300_fcsmu_getreg(priv, FC7300_FCSMU_FE0_OFFSET);
        }
      else
        {
          if (nt_irq_channel > 0U && nt_irq_channel <= 32U)
            {
              irq_channel_mask |=
                (uint32_t)1U << (nt_irq_channel - (uint32_t)1U);
            }

          if (ot_irq_channel > 0U && ot_irq_channel <= 32U)
            {
              irq_channel_mask |=
                (uint32_t)1U << (ot_irq_channel - (uint32_t)1U);
            }
        }

      ret = fc7300_fcsmu_user_callback(irq_channel_mask,
                                       FCSMU_ERROR_TYPE_WARNING);
      if (ret == 0U)
        {
          fc7300_fcsmu_clear_fault(irq_channel_mask);
        }
    }
  else if ((irq_stat & FCSMU_IRQ_STAT_CFG_TO_IRQ) != 0)
    {
      ret = fc7300_fcsmu_user_callback(0U, FCSMU_ERROR_TYPE_TIMEOUT);
      if (ret == 0U)
        {
          fc7300_fcsmu_putreg(priv,
                              FC7300_FCSMU_IRQ_STAT_OFFSET,
                              FCSMU_IRQ_STAT_CFG_TO_IRQ);
        }
    }
  else
    {
      /* Do nothing. */
    }

  return OK;
}

/****************************************************************************
 * Name: fc7300_fcsmu_normal_to_config
 *
 * Description:
 *  Set fcsmu to config state.
 *
 * Input Parameters:
 *   priv   - FCSMU device private data
 *
 ****************************************************************************/

static int fc7300_fcsmu_normal_to_config(struct fc7300_fcsmu_config_s *priv)
{
  int status = ERROR;
  uint32_t retry_times = 65535U;
  uint32_t timeout;
  uint32_t temp;

  while (retry_times != 0U)
    {
      timeout = 65535U;
      fc7300_fcsmu_putreg(priv,
                          FC7300_FCSMU_TEMP_UNLK_OFFSET,
                          FCSMU_CONFIG_TMEP_UNLOCK_KEY);
      fc7300_fcsmu_putreg(priv,
                          FC7300_FCSMU_OPRK_OFFSET,
                          FCSMU_OPC1_UNLOCK_KEY);
      fc7300_fcsmu_putreg(priv,
                          FC7300_FCSMU_CTRL_OFFSET,
                          FCSMU_OP_CODE_MOVE_TO_CONFIG);

      while (timeout != 0U)
        {
          temp = (fc7300_fcsmu_getreg(priv, FC7300_FCSMU_CTRL_OFFSET) &
                 FCSMU_CTRL_OPS_MASK) >> FCSMU_CTRL_OPS_SHIFT;
          if (temp == FCSMU_OP_STATE_BUSY)
            {
              timeout--;
            }
          else
            {
              break;
            }
        }

      if (timeout != 0U)
        {
          temp = (fc7300_fcsmu_getreg(priv, FC7300_FCSMU_STATUS_OFFSET) &
                 FCSMU_STATUS_STAT_MASK) >> FCSMU_STATUS_STAT_SHIFT;
          if (temp == FCSMU_STATE_CONGIG)
            {
              break;
            }
        }

      retry_times--;
    }

  if (retry_times != 0U)
    {
      status = OK;
    }

  return status;
}

/****************************************************************************
 * Name: fc7300_fcsmu_config_to_normal
 *
 * Description:
 *  Set fcsmu to normal state.
 *
 * Input Parameters:
 *   priv   - FCSMU device private data
 *
 ****************************************************************************/

static int fc7300_fcsmu_config_to_normal(struct fc7300_fcsmu_config_s *priv)
{
  int status = ERROR;
  uint32_t retry_times = 65535U;
  uint32_t timeout;
  uint32_t temp;

  while (retry_times != 0U)
    {
      timeout = 65535U;
      fc7300_fcsmu_putreg(priv,
                          FC7300_FCSMU_OPRK_OFFSET,
                          FCSMU_OPC2_UNLOCK_KEY);
      fc7300_fcsmu_putreg(priv,
                          FC7300_FCSMU_CTRL_OFFSET,
                          FCSMU_OP_CODE_MOVE_TO_NORMAL);

      while (timeout != 0U)
        {
          temp = (fc7300_fcsmu_getreg(priv, FC7300_FCSMU_CTRL_OFFSET) &
                 FCSMU_CTRL_OPS_MASK) >> FCSMU_CTRL_OPS_SHIFT;
          if (temp == FCSMU_OP_STATE_BUSY)
            {
              timeout--;
            }
          else
            {
              break;
            }
        }

      if (timeout != 0U)
        {
          temp = (fc7300_fcsmu_getreg(priv, FC7300_FCSMU_STATUS_OFFSET) &
                  FCSMU_STATUS_STAT_MASK) >> FCSMU_STATUS_STAT_SHIFT;
          if (temp == FCSMU_STATE_NORMAL)
            {
              break;
            }
        }

      retry_times--;
    }

  if (retry_times != 0U)
    {
      status = OK;
    }

  fc7300_fcsmu_putreg(priv,
                      FC7300_FCSMU_TEMP_UNLK_OFFSET,
                      FCSMU_CONFIG_TMEP_LOCK_KEY);

  return status;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: fc7300_fcsmu_init
 *
 * Description:
 *   Hardware level initializes the FCSMU Driver settings based on the
 * configuration.
 *
 ****************************************************************************/

void fc7300_fcsmu_init(void)
{
  uint32_t temp;
  uint32_t timeout = 65535U;

  if (fc7300_cpu_index() == 0U)
    {
      por_fst = fc7300_fcsmu_getreg((struct fc7300_fcsmu_config_s *)&g_fcsmu_config,
                                    FC7300_FCSMU_FST0_OFFSET);
      fc7300_fcsmu_clear_fault(0xFFFFFFFFUL);
      fc7300_fcsmu_putreg((struct fc7300_fcsmu_config_s *)&g_fcsmu_config,
                          FC7300_FCSMU_CFG_TO_OFFSET,
                          g_fcsmu_config.CFG_TO);
      fc7300_fcsmu_putreg((struct fc7300_fcsmu_config_s *)&g_fcsmu_config,
                          FC7300_FCSMU_IRQ_EN_OFFSET,
                          g_fcsmu_config.CFG_TO_IEN);
      fc7300_fcsmu_putreg((struct fc7300_fcsmu_config_s *)&g_fcsmu_config,
                          FC7300_FCSMU_IRQ_STAT_OFFSET,
                          FCSMU_IRQ_STAT_CFG_TO_IRQ);

      if (fc7300_fcsmu_normal_to_config((struct fc7300_fcsmu_config_s *)&g_fcsmu_config) == OK)
        {
          fc7300_fcsmu_putreg((struct fc7300_fcsmu_config_s *)&g_fcsmu_config,
                              FC7300_FCSMU_WARNING_TO_OFFSET,
                              g_fcsmu_config.WARNING_TO);
          fc7300_fcsmu_putreg((struct fc7300_fcsmu_config_s *)&g_fcsmu_config,
                              FC7300_FCSMU_WARNING_EN0_OFFSET,
                              g_fcsmu_config.WARNING_EN);
          fc7300_fcsmu_putreg((struct fc7300_fcsmu_config_s *)&g_fcsmu_config,
                              FC7300_FCSMU_WARNING_IEN0_OFFSET,
                              g_fcsmu_config.WARNING_IEN);
          fc7300_fcsmu_putreg((struct fc7300_fcsmu_config_s *)&g_fcsmu_config,
                              FC7300_FCSMU_FAULT_IEN0_OFFSET,
                              g_fcsmu_config.FAULT_IEN);
          fc7300_fcsmu_putreg((struct fc7300_fcsmu_config_s *)&g_fcsmu_config,
                              FC7300_FCSMU_FRST0_OFFSET,
                              g_fcsmu_config.FRST);
          fc7300_fcsmu_putreg((struct fc7300_fcsmu_config_s *)&g_fcsmu_config,
                              FC7300_FCSMU_FCCR0_OFFSET,
                              g_fcsmu_config.FCCR);
          fc7300_fcsmu_putreg((struct fc7300_fcsmu_config_s *)&g_fcsmu_config,
                              FC7300_FCSMU_FE0_OFFSET,
                              g_fcsmu_config.FE);

          if (g_fcsmu_config.SOCTRL & FCSMU_SOCTRL_SOUT_PEN)
            {
              fc7300_pinconfig(PIN_FCSMU_PIN0);
              fc7300_pinconfig(PIN_FCSMU_PIN1);
            }

          fc7300_fcsmu_putreg((struct fc7300_fcsmu_config_s *)&g_fcsmu_config,
                              FC7300_FCSMU_SOCTRL_OFFSET,
                              g_fcsmu_config.SOCTRL);
          fc7300_fcsmu_putreg((struct fc7300_fcsmu_config_s *)&g_fcsmu_config,
                              FC7300_FCSMU_STMR_OFFSET,
                              g_fcsmu_config.STMR);
          fc7300_fcsmu_putreg((struct fc7300_fcsmu_config_s *)&g_fcsmu_config,
                              FC7300_FCSMU_SOUT_EN0_OFFSET,
                              g_fcsmu_config.SOUT_EN);
          fc7300_fcsmu_config_to_normal((struct fc7300_fcsmu_config_s *)&g_fcsmu_config);

          fc7300_fcsmu_putreg((struct fc7300_fcsmu_config_s *)&g_fcsmu_config,
                              FC7300_FCSMU_SOUT_DIAG_OFFSET,
                              g_fcsmu_config.SOUT_DIAG);

          fc7300_fcsmu_putreg((struct fc7300_fcsmu_config_s *)&g_fcsmu_config,
                              FC7300_FCSMU_CRC_CTRL_OFFSET,
                              FCSMU_CRC_CTRL_EF);
          fc7300_fcsmu_putreg((struct fc7300_fcsmu_config_s *)&g_fcsmu_config,
                              FC7300_FCSMU_CRC_CTRL_OFFSET,
                              FCSMU_CRC_CTRL_GEN);
          while (timeout != 0U)
            {
              temp = fc7300_fcsmu_crc_is_busy();
              timeout--;
              if (temp == FCSMU_CRC_STATE_IDLE)
                {
                  break;
                }
            }

          if (timeout != 0U)
            {
#if defined(CONFIG_FC7300_FCSMU_CRC_ERROR_INTERRUPT)
              *(volatile uint32_t *)0x40079060U |= 0x1000U;
#endif
#if defined(CONFIG_FC7300_FCSMU_CRC_ERROR_RESET)
              *(volatile uint32_t *)0x40079060U |= 0x2000U;
#endif
              fc7300_fcsmu_putreg((struct fc7300_fcsmu_config_s *)&g_fcsmu_config,
                                  FC7300_FCSMU_CRC_CTRL_OFFSET,
                                  g_fcsmu_config.CRC_CTRL);
            }
        }
    }
}

/****************************************************************************
 * Name: fc7300_fcsmu_deinit
 *
 * Description:
 *   Hardware level ide-nitializes the FCSMU Driver.
 *
 ****************************************************************************/

void fc7300_fcsmu_deinit(void)
{
  if (fc7300_cpu_index() == 0U)
    {
      up_disable_irq(g_fcsmu_config.irq);
      fc7300_fcsmu_putreg((struct fc7300_fcsmu_config_s *)&g_fcsmu_config,
                          FC7300_FCSMU_CFG_TO_OFFSET, 0x5UL);
      fc7300_fcsmu_putreg((struct fc7300_fcsmu_config_s *)&g_fcsmu_config,
                          FC7300_FCSMU_IRQ_EN_OFFSET, 0x0UL);

      if (fc7300_fcsmu_normal_to_config((struct fc7300_fcsmu_config_s *)&g_fcsmu_config) == OK)
        {
          fc7300_fcsmu_putreg((struct fc7300_fcsmu_config_s *)&g_fcsmu_config,
                              FC7300_FCSMU_FE0_OFFSET, 0x0UL);
          fc7300_fcsmu_putreg((struct fc7300_fcsmu_config_s *)&g_fcsmu_config,
                              FC7300_FCSMU_WARNING_TO_OFFSET, 0x3A980UL);
          fc7300_fcsmu_putreg((struct fc7300_fcsmu_config_s *)&g_fcsmu_config,
                              FC7300_FCSMU_WARNING_EN0_OFFSET, 0x0UL);
          fc7300_fcsmu_putreg((struct fc7300_fcsmu_config_s *)&g_fcsmu_config,
                              FC7300_FCSMU_WARNING_IEN0_OFFSET, 0x0UL);
          fc7300_fcsmu_putreg((struct fc7300_fcsmu_config_s *)&g_fcsmu_config,
                              FC7300_FCSMU_FAULT_IEN0_OFFSET, 0x0UL);
          fc7300_fcsmu_putreg((struct fc7300_fcsmu_config_s *)&g_fcsmu_config,
                              FC7300_FCSMU_FRST0_OFFSET, 0x0UL);
          fc7300_fcsmu_putreg((struct fc7300_fcsmu_config_s *)&g_fcsmu_config,
                              FC7300_FCSMU_FCCR0_OFFSET, 0xFFFFFFFFUL);

          fc7300_fcsmu_putreg((struct fc7300_fcsmu_config_s *)&g_fcsmu_config,
                              FC7300_FCSMU_SOCTRL_OFFSET, 0x0UL);
          fc7300_fcsmu_putreg((struct fc7300_fcsmu_config_s *)&g_fcsmu_config,
                              FC7300_FCSMU_STMR_OFFSET, 0x0UL);
          fc7300_fcsmu_putreg((struct fc7300_fcsmu_config_s *)&g_fcsmu_config,
                              FC7300_FCSMU_SOUT_EN0_OFFSET, 0x0UL);
          fc7300_fcsmu_config_to_normal((struct fc7300_fcsmu_config_s *)&g_fcsmu_config);

          fc7300_fcsmu_putreg((struct fc7300_fcsmu_config_s *)&g_fcsmu_config,
                              FC7300_FCSMU_SOUT_DIAG_OFFSET, 0x0UL);

          fc7300_fcsmu_putreg((struct fc7300_fcsmu_config_s *)&g_fcsmu_config,
                              FC7300_FCSMU_CRC_CTRL_OFFSET, 0x0UL);
        }

        fc7300_fcsmu_clear_fault(0xFFFFFFFFUL);
    }
}

/****************************************************************************
 * Name: fc7300_fcsmu_get_error_status
 *
 * Description:
 *   Get fcsmu error after system reset.
 *
 ****************************************************************************/

uint32_t fc7300_fcsmu_get_error_status(void)
{
  return por_fst;
}

/****************************************************************************
 * Name: fc7300_fcsmu_software_trigger_error
 *
 * Description:
 *   Software trigger error for fcsmu channel 18.
 *
 ****************************************************************************/

void fc7300_fcsmu_software_trigger_error(void)
{
  fc7300_fcsmu_putreg((struct fc7300_fcsmu_config_s *)&g_fcsmu_config,
                      FC7300_FCSMU_INJECT_OFFSET,
                      FCSMU_CHANNEL_SOFTWARE_TRIGGER_ERROR);
}

/****************************************************************************
 * Name: fc7300_fcsmu_irq_attach
 *
 * Description:
 *   Interrupt registration function after the system starts.
 *
 ****************************************************************************/

void fc7300_fcsmu_irq_attach(void)
{
  if (fc7300_cpu_index() == 0U)
    {
      irq_attach(g_fcsmu_config.irq, fc7300_fcsmu_isr, (struct fc7300_fcsmu_config_s *)&g_fcsmu_config);
#  ifdef CONFIG_ARCH_IRQPRIO
      up_prioritize_irq(g_fcsmu_config.irq, g_fcsmu_config.irq_priority);
#  endif
      up_enable_irq(g_fcsmu_config.irq);
    }
}

/****************************************************************************
 * Name: fc7300_fcsmu_irq_handler
 *
 * Description:
 *   Interrupt entry function used by EIM.
 *
 ****************************************************************************/

void fc7300_fcsmu_irq_handler(void)
{
  fc7300_fcsmu_isr(0u, NULL, (struct fc7300_fcsmu_config_s *)&g_fcsmu_config);
}

/****************************************************************************
 * Name: fc7300_fcsmu_crc_software_trigger
 *
 * Description:
 *   Software triggered CRC check.
 *
 ****************************************************************************/

void fc7300_fcsmu_crc_software_trigger(void)
{
  fc7300_fcsmu_putreg((struct fc7300_fcsmu_config_s *)&g_fcsmu_config,
                      FC7300_FCSMU_CRC_CTRL_OFFSET,
                      g_fcsmu_config.CRC_CTRL | FCSMU_CRC_CTRL_GEN);
}

/****************************************************************************
 * Name: fc7300_fcsmu_crc_is_busy
 *
 * Description:
 *   Get crc check whether it is completed.
 *
 ****************************************************************************/

uint32_t fc7300_fcsmu_crc_is_busy(void)
{
  return ((fc7300_fcsmu_getreg((struct fc7300_fcsmu_config_s *)&g_fcsmu_config,
                               FC7300_FCSMU_CRC_CTRL_OFFSET) &
          0x10U) >> 4U);
}

/****************************************************************************
 * Name: fc7300_fcsmu_crc_result
 *
 * Description:
 *   Get crc result.
 *
 ****************************************************************************/

uint32_t fc7300_fcsmu_crc_result(void)
{
  return fc7300_fcsmu_getreg((struct fc7300_fcsmu_config_s *)&g_fcsmu_config, FC7300_FCSMU_CRC_RES_OFFSET);
}

/****************************************************************************
 * Name: fc7300_fcsmu_clear_fault
 *
 * Description:
 *  Clear channel fault.
 *
 * Input Parameters:
 *   fault_channel - FCSMU channel mask
 *
 ****************************************************************************/

int fc7300_fcsmu_clear_fault(uint32_t fault_channel)
{
  uint32_t timeout = 65535U;
  int ret = OK;

  fc7300_fcsmu_putreg((struct fc7300_fcsmu_config_s *)&g_fcsmu_config,
                  FC7300_FCSMU_FST_UNLK_OFFSET,
                  FCSMU_FST_UNLOCK_KEY);
  fc7300_fcsmu_putreg((struct fc7300_fcsmu_config_s *)&g_fcsmu_config,
                  FC7300_FCSMU_FST0_OFFSET,
                  fault_channel);

  while (((fc7300_fcsmu_getreg((struct fc7300_fcsmu_config_s *)&g_fcsmu_config, FC7300_FCSMU_CTRL_OFFSET) &
          FCSMU_CTRL_OPS_MASK) >> FCSMU_CTRL_OPS_SHIFT) !=
        FCSMU_OP_STATE_SUCCESSFUL)
    {
      timeout--;
      if (timeout == 0U)
        {
          ret = ERROR;
          break;
        }
    }

  /* Clear NTF/WTF/NTW/FTW register when no FST flag is set. */

  if (ret == OK)
    {
      if (fc7300_fcsmu_getreg((struct fc7300_fcsmu_config_s *)&g_fcsmu_config, FC7300_FCSMU_FST0_OFFSET) == 0U)
        {
          fc7300_fcsmu_putreg((struct fc7300_fcsmu_config_s *)&g_fcsmu_config,
                              FC7300_FCSMU_CTRL_OFFSET,
                              FCSMU_OP_CODE_CLEAR_FAULT_INFO);

          timeout = 65535U;
          while (((fc7300_fcsmu_getreg((struct fc7300_fcsmu_config_s *)&g_fcsmu_config, FC7300_FCSMU_CTRL_OFFSET) &
                  FCSMU_CTRL_OPS_MASK) >> FCSMU_CTRL_OPS_SHIFT) !=
                FCSMU_OP_STATE_SUCCESSFUL)
            {
              timeout--;
              if (timeout == 0U)
                {
                  ret = ERROR;
                  break;
                }
            }
        }
    }

  return ret;
}
