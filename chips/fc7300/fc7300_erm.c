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
#include "fc7300_erm.h"
#include "fc7300_erm_user.h"
#include "hardware/fc7300_erm.h"
#include "fc7300_multicore.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Private Types
 ****************************************************************************/

/* ERM Device Private Data */

struct fc7300_erm_config_s
{
  uint32_t reg_base;        /* ERM register base address */
  uint32_t irq;             /* Interrupt ID */
  uint32_t irq_priority;    /* Interrupt priority */
  uint32_t CR0;
  uint32_t CR1;
  uint32_t CR2;
  uint32_t CR3;
};

#define ERM_NON_CORRECTABLE_IRQ         0x1u      /* select Non-correctable interrupt report */
#define ERM_SINGLE_CORRECTION_IRQ       0x2u      /* select single correction interrupt report */

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

static inline uint32_t
fc7300_erm_getreg(struct fc7300_erm_config_s *priv, uint16_t offset);
static inline void fc7300_erm_putreg(struct fc7300_erm_config_s *priv,
                                     uint16_t offset, uint32_t value);
static int fc7300_erm_isr(int irq, void *context, void *arg);

/****************************************************************************
 * Private Data
 ****************************************************************************/

static struct fc7300_erm_config_s g_erm_config =
{
  .reg_base     = FC7300_ERM_BASE,
  .irq          = FC7300_IRQ_ERM,
  .irq_priority = NVIC_ERM_PRIORITY_DEFAULT,
  .CR0          = 0x0u |
#if defined(CONFIG_FC7300_ERM_PFLASH0_ECC_MODE_S) || defined(CONFIG_FC7300_ERM_PFLASH0_ECC_MODE_A)
                  (ERM_SINGLE_CORRECTION_IRQ << 26u) |  /* 1U  PFlash0 ECC Error      */
#endif
#if defined(CONFIG_FC7300_ERM_PFLASH0_ECC_MODE_M) || defined(CONFIG_FC7300_ERM_PFLASH0_ECC_MODE_A)
                  (ERM_NON_CORRECTABLE_IRQ << 26u) |
#endif
#if defined(CONFIG_FC7300_ERM_PFLASH1_ECC_MODE_S) || defined(CONFIG_FC7300_ERM_PFLASH1_ECC_MODE_A)
                  (ERM_SINGLE_CORRECTION_IRQ << 22u) |  /* 2U  PFlash1 ECC Error      */
#endif
#if defined(CONFIG_FC7300_ERM_PFLASH1_ECC_MODE_M) || defined(CONFIG_FC7300_ERM_PFLASH1_ECC_MODE_A)
                  (ERM_NON_CORRECTABLE_IRQ << 22u) |
#endif
#if defined(CONFIG_FC7300_ERM_DFLASH_ECC_MODE_S) || defined(CONFIG_FC7300_ERM_DFLASH_ECC_MODE_A)
                  (ERM_SINGLE_CORRECTION_IRQ << 18u) |  /* 3U  DFlash ECC Error       */
#endif
#if defined(CONFIG_FC7300_ERM_DFLASH_ECC_MODE_M) || defined(CONFIG_FC7300_ERM_DFLASH_ECC_MODE_A)
                  (ERM_NON_CORRECTABLE_IRQ << 18u) |
#endif
#if defined(CONFIG_FC7300_ERM_DMACFG0_ECC_MODE_S) || defined(CONFIG_FC7300_ERM_DMACFG0_ECC_MODE_A)
                  (ERM_SINGLE_CORRECTION_IRQ << 14u) |  /* 4U  DMACFG0 ECC Error      */
#endif
#if defined(CONFIG_FC7300_ERM_DMACFG0_ECC_MODE_M) || defined(CONFIG_FC7300_ERM_DMACFG0_ECC_MODE_A)
                  (ERM_NON_CORRECTABLE_IRQ << 14u) |
#endif
#if defined(CONFIG_FC7300_ERM_DMACFG1_ECC_MODE_S) || defined(CONFIG_FC7300_ERM_DMACFG1_ECC_MODE_A)
                  (ERM_SINGLE_CORRECTION_IRQ << 10u) |  /* 5U  DMACFG1 ECC Error      */
#endif
#if defined(CONFIG_FC7300_ERM_DMACFG1_ECC_MODE_M) || defined(CONFIG_FC7300_ERM_DMACFG1_ECC_MODE_A)
                  (ERM_NON_CORRECTABLE_IRQ << 10u) |
#endif
#if defined(CONFIG_FC7300_ERM_ROM_ECC_MODE_S) || defined(CONFIG_FC7300_ERM_ROM_ECC_MODE_A)
                  (ERM_SINGLE_CORRECTION_IRQ << 6u) |  /* 6U  ROM ECC Error          */
#endif
#if defined(CONFIG_FC7300_ERM_ROM_ECC_MODE_M) || defined(CONFIG_FC7300_ERM_ROM_ECC_MODE_A)
                  (ERM_NON_CORRECTABLE_IRQ << 6u) |
#endif
                  0x0u,
  .CR1          = 0x0u |
#if defined(CONFIG_FC7300_ERM_SYSRAM0_ECC_MODE_S) || defined(CONFIG_FC7300_ERM_SYSRAM0_ECC_MODE_A)
                  (ERM_SINGLE_CORRECTION_IRQ << 30u) |  /* 8U  SysRAM0 ECC Error      */
#endif
#if defined(CONFIG_FC7300_ERM_SYSRAM0_ECC_MODE_M) || defined(CONFIG_FC7300_ERM_SYSRAM0_ECC_MODE_A)
                  (ERM_NON_CORRECTABLE_IRQ << 30u) |
#endif
#if defined(CONFIG_FC7300_ERM_SYSRAM1_ECC_MODE_S) || defined(CONFIG_FC7300_ERM_SYSRAM1_ECC_MODE_A)
                  (ERM_SINGLE_CORRECTION_IRQ << 26u) |  /* 9U  SysRAM1 ECC Error      */
#endif
#if defined(CONFIG_FC7300_ERM_SYSRAM1_ECC_MODE_M) || defined(CONFIG_FC7300_ERM_SYSRAM1_ECC_MODE_A)
                  (ERM_NON_CORRECTABLE_IRQ << 26u) |
#endif
#if defined(CONFIG_FC7300_ERM_SYSRAM2_ECC_MODE_S) || defined(CONFIG_FC7300_ERM_SYSRAM2_ECC_MODE_A)
                  (ERM_SINGLE_CORRECTION_IRQ << 22u) |  /* 10U SysRAM2 ECC Error      */
#endif
#if defined(CONFIG_FC7300_ERM_SYSRAM2_ECC_MODE_M) || defined(CONFIG_FC7300_ERM_SYSRAM2_ECC_MODE_A)
                  (ERM_NON_CORRECTABLE_IRQ << 22u) |
#endif
#if defined(CONFIG_FC7300_ERM_CPU0_ITCM_ECC_MODE_S) || defined(CONFIG_FC7300_ERM_CPU0_ITCM_ECC_MODE_A)
                  (ERM_SINGLE_CORRECTION_IRQ << 18u) |  /* 11U CPU0 ITCM ECC Error    */
#endif
#if defined(CONFIG_FC7300_ERM_CPU0_ITCM_ECC_MODE_M) || defined(CONFIG_FC7300_ERM_CPU0_ITCM_ECC_MODE_A)
                  (ERM_NON_CORRECTABLE_IRQ << 18u) |
#endif
#if defined(CONFIG_FC7300_ERM_CPU0_DTCM0_ECC_MODE_S) || defined(CONFIG_FC7300_ERM_CPU0_DTCM0_ECC_MODE_A)
                  (ERM_SINGLE_CORRECTION_IRQ << 14u) |  /* 12U CPU0 DTCM0 ECC Error   */
#endif
#if defined(CONFIG_FC7300_ERM_CPU0_DTCM0_ECC_MODE_M) || defined(CONFIG_FC7300_ERM_CPU0_DTCM0_ECC_MODE_A)
                  (ERM_NON_CORRECTABLE_IRQ << 14u) |
#endif
#if defined(CONFIG_FC7300_ERM_CPU0_DTCM1_ECC_MODE_S) || defined(CONFIG_FC7300_ERM_CPU0_DTCM1_ECC_MODE_A)
                  (ERM_SINGLE_CORRECTION_IRQ << 10u) |  /* 13U CPU0 DTCM1 ECC Error   */
#endif
#if defined(CONFIG_FC7300_ERM_CPU0_DTCM1_ECC_MODE_M) || defined(CONFIG_FC7300_ERM_CPU0_DTCM1_ECC_MODE_A)
                  (ERM_NON_CORRECTABLE_IRQ << 10u) |
#endif
#if defined(CONFIG_FC7300_ERM_CPU0_ICACHE_ECC_MODE_S) || defined(CONFIG_FC7300_ERM_CPU0_ICACHE_ECC_MODE_A)
                  (ERM_SINGLE_CORRECTION_IRQ << 6u) |  /* 14U CPU0 ICACHE ECC Error  */
#endif
#if defined(CONFIG_FC7300_ERM_CPU0_ICACHE_ECC_MODE_M) || defined(CONFIG_FC7300_ERM_CPU0_ICACHE_ECC_MODE_A)
                  (ERM_NON_CORRECTABLE_IRQ << 6u) |
#endif
#if defined(CONFIG_FC7300_ERM_CPU0_DCACHE_ECC_MODE_S) || defined(CONFIG_FC7300_ERM_CPU0_DCACHE_ECC_MODE_A)
                  (ERM_SINGLE_CORRECTION_IRQ << 2u) |  /* 15U CPU0 DCACHE ECC Error  */
#endif
#if defined(CONFIG_FC7300_ERM_CPU0_DCACHE_ECC_MODE_M) || defined(CONFIG_FC7300_ERM_CPU0_DCACHE_ECC_MODE_A)
                  (ERM_NON_CORRECTABLE_IRQ << 2u) |
#endif
                  0x0u,
  .CR2          = 0x0u |
#if defined(CONFIG_FC7300_ERM_CPU1_ITCM_ECC_MODE_S) || defined(CONFIG_FC7300_ERM_CPU1_ITCM_ECC_MODE_A)
                  (ERM_SINGLE_CORRECTION_IRQ << 30u) |  /* 16U CPU1 ITCMECC Error     */
#endif
#if defined(CONFIG_FC7300_ERM_CPU1_ITCM_ECC_MODE_M) || defined(CONFIG_FC7300_ERM_CPU1_ITCM_ECC_MODE_A)
                  (ERM_NON_CORRECTABLE_IRQ << 30u) |
#endif
#if defined(CONFIG_FC7300_ERM_CPU1_DTCM0_ECC_MODE_S) || defined(CONFIG_FC7300_ERM_CPU1_DTCM0_ECC_MODE_A)
                  (ERM_SINGLE_CORRECTION_IRQ << 26u) |  /* 17U CPU1 DTCM0 ECC Error   */
#endif
#if defined(CONFIG_FC7300_ERM_CPU1_DTCM0_ECC_MODE_M) || defined(CONFIG_FC7300_ERM_CPU1_DTCM0_ECC_MODE_A)
                  (ERM_NON_CORRECTABLE_IRQ << 26u) |
#endif
#if defined(CONFIG_FC7300_ERM_CPU1_DTCM1_ECC_MODE_S) || defined(CONFIG_FC7300_ERM_CPU1_DTCM1_ECC_MODE_A)
                  (ERM_SINGLE_CORRECTION_IRQ << 22u) |  /* 18U CPU1 DTCM1 ECC Error   */
#endif
#if defined(CONFIG_FC7300_ERM_CPU1_DTCM1_ECC_MODE_M) || defined(CONFIG_FC7300_ERM_CPU1_DTCM1_ECC_MODE_A)
                  (ERM_NON_CORRECTABLE_IRQ << 22u) |
#endif
#if defined(CONFIG_FC7300_ERM_CPU1_ICACHE_ECC_MODE_S) || defined(CONFIG_FC7300_ERM_CPU1_ICACHE_ECC_MODE_A)
                  (ERM_SINGLE_CORRECTION_IRQ << 18u) |  /* 19U CPU1 ICACHE ECC Error  */
#endif
#if defined(CONFIG_FC7300_ERM_CPU1_ICACHE_ECC_MODE_M) || defined(CONFIG_FC7300_ERM_CPU1_ICACHE_ECC_MODE_A)
                  (ERM_NON_CORRECTABLE_IRQ << 18u) |
#endif
#if defined(CONFIG_FC7300_ERM_CPU1_DCACHE_ECC_MODE_S) || defined(CONFIG_FC7300_ERM_CPU1_DCACHE_ECC_MODE_A)
                  (ERM_SINGLE_CORRECTION_IRQ << 14u) |  /* 20U CPU1 DCACHE ECC Error  */
#endif
#if defined(CONFIG_FC7300_ERM_CPU1_DCACHE_ECC_MODE_M) || defined(CONFIG_FC7300_ERM_CPU1_DCACHE_ECC_MODE_A)
                  (ERM_NON_CORRECTABLE_IRQ << 14u) |
#endif
#if defined(CONFIG_FC7300_ERM_CPU2_ITCM_ECC_MODE_S) || defined(CONFIG_FC7300_ERM_CPU2_ITCM_ECC_MODE_A)
                  (ERM_SINGLE_CORRECTION_IRQ << 10u) |  /* 21U CPU2 ITCMECC ECC Error */
#endif
#if defined(CONFIG_FC7300_ERM_CPU2_ITCM_ECC_MODE_M) || defined(CONFIG_FC7300_ERM_CPU2_ITCM_ECC_MODE_A)
                  (ERM_NON_CORRECTABLE_IRQ << 10u) |
#endif
#if defined(CONFIG_FC7300_ERM_CPU2_DTCM0_ECC_MODE_S) || defined(CONFIG_FC7300_ERM_CPU2_DTCM0_ECC_MODE_A)
                  (ERM_SINGLE_CORRECTION_IRQ << 6u) |  /* 22U CPU2 DTCM0 ECC Error   */
#endif
#if defined(CONFIG_FC7300_ERM_CPU2_DTCM0_ECC_MODE_M) || defined(CONFIG_FC7300_ERM_CPU2_DTCM0_ECC_MODE_A)
                  (ERM_NON_CORRECTABLE_IRQ << 6u) |
#endif
#if defined(CONFIG_FC7300_ERM_CPU2_DTCM1_ECC_MODE_S) || defined(CONFIG_FC7300_ERM_CPU2_DTCM1_ECC_MODE_A)
                  (ERM_SINGLE_CORRECTION_IRQ << 2u) |  /* 23U CPU2 DTCM1 ECC Error   */
#endif
#if defined(CONFIG_FC7300_ERM_CPU2_DTCM1_ECC_MODE_M) || defined(CONFIG_FC7300_ERM_CPU2_DTCM1_ECC_MODE_A)
                  (ERM_NON_CORRECTABLE_IRQ << 2u) |
#endif
                  0x0u,
  .CR3          = 0x0u |
#if defined(CONFIG_FC7300_ERM_CPU1_ICACHE_ECC_MODE_S) || defined(CONFIG_FC7300_ERM_CPU1_ICACHE_ECC_MODE_A)
                  (ERM_SINGLE_CORRECTION_IRQ << 30u) |  /* 24U CPU2 ICACHE ECC Error  */
#endif
#if defined(CONFIG_FC7300_ERM_CPU1_ICACHE_ECC_MODE_M) || defined(CONFIG_FC7300_ERM_CPU1_ICACHE_ECC_MODE_A)
                  (ERM_NON_CORRECTABLE_IRQ << 30u) |
#endif
#if defined(CONFIG_FC7300_ERM_CPU1_DCACHE_ECC_MODE_S) || defined(CONFIG_FC7300_ERM_CPU1_DCACHE_ECC_MODE_A)
                  (ERM_SINGLE_CORRECTION_IRQ << 26u) |  /* 25U CPU2 DCACHE ECC Error  */
#endif
#if defined(CONFIG_FC7300_ERM_CPU1_DCACHE_ECC_MODE_M) || defined(CONFIG_FC7300_ERM_CPU1_DCACHE_ECC_MODE_A)
                  (ERM_NON_CORRECTABLE_IRQ << 26u) |
#endif
                  0x0u,
};

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: fc7300_erm_getreg
 *
 * Description:
 *   Get a 32-bit register value by offset
 *
 * Input Parameters:
 *   priv   - ERM device private data
 *   offset - Register offset with respect to the base address of the ERM
 *            peripheral
 *
 * Returned Value:
 *   The 32-bit value retrieved from the register
 *
 ****************************************************************************/

static inline uint32_t
fc7300_erm_getreg(struct fc7300_erm_config_s *priv, uint16_t offset)
{
  return getreg32(priv->reg_base + offset);
}

/****************************************************************************
 * Name: fc7300_erm_putreg
 *
 * Description:
 *  Put a 32-bit register value by offset
 *
 * Input Parameters:
 *   priv   - ERM device private data
 *   offset - Register offset with respect to the base address of the ERM
 *            peripheral
 *   value  - The 32-bit value that should be put into the register
 *
 ****************************************************************************/

static inline void fc7300_erm_putreg(struct fc7300_erm_config_s *priv,
                                     uint16_t offset, uint32_t value)
{
  putreg32(value, priv->reg_base + offset);
}

/****************************************************************************
 * Name: fc7300_erm_isr
 *
 * Description:
 *   Common ERM interrupt service routine
 *
 ****************************************************************************/

static int fc7300_erm_isr(int irq, void *context, void *arg)
{
  struct fc7300_erm_config_s *priv = (struct fc7300_erm_config_s *)arg;

  DEBUGASSERT(priv != NULL);

  int ret;
  uint32_t reg_value;
  uint32_t e_addr;
  uint8_t loop_i;
  uint8_t loop_j;
  uint8_t e_type;
  uint8_t e_ch;

  for (loop_i = 0u; loop_i < 4u; loop_i++)
    {
      reg_value = fc7300_erm_getreg(priv, FC7300_ERM_SR0_OFFSET + (loop_i << 2));
      if (reg_value != 0u)
        {
          for (loop_j = 0u; loop_j < 8u; loop_j++)
            {
              e_type = (reg_value >> (30u - 4u * loop_j)) & 0x3u;
              if (e_type != 0u)
                {
                  e_ch = (uint8_t)((loop_i << 3u) + loop_j);
                  e_addr =
                    fc7300_erm_getreg(priv, FC7300_ERM_EAR_OFFSET(e_ch));

                  ret = fc7300_erm_user_callback(e_ch, e_type, e_addr);
                  if (ret == 0u)
                    {
                      fc7300_erm_putreg(priv,
                        FC7300_ERM_SR0_OFFSET + (loop_i << 2),
                        (reg_value & (0x3ul << (30u - 4u * loop_j))));
                    }
                }
            }
        }
    }

  return OK;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: fc7300_erm_init
 *
 * Description:
 *   Hardware level initializes the ERM Driver settings based on the
 * configuration.
 *
 ****************************************************************************/

void fc7300_erm_init(void)
{
  if (fc7300_cpu_index() == 0U)
    {
      fc7300_erm_putreg(&g_erm_config, FC7300_ERM_SR0_OFFSET, 0xccccccccul);
      fc7300_erm_putreg(&g_erm_config, FC7300_ERM_SR1_OFFSET, 0xccccccccul);
      fc7300_erm_putreg(&g_erm_config, FC7300_ERM_SR2_OFFSET, 0xccccccccul);
      fc7300_erm_putreg(&g_erm_config, FC7300_ERM_SR3_OFFSET, 0xccccccccul);

      fc7300_erm_putreg(&g_erm_config, FC7300_ERM_CR0_OFFSET, g_erm_config.CR0);
      fc7300_erm_putreg(&g_erm_config, FC7300_ERM_CR1_OFFSET, g_erm_config.CR1);
      fc7300_erm_putreg(&g_erm_config, FC7300_ERM_CR2_OFFSET, g_erm_config.CR2);
      fc7300_erm_putreg(&g_erm_config, FC7300_ERM_CR3_OFFSET, g_erm_config.CR3);
    }
}

/****************************************************************************
 * Name: fc7300_erm_irq_attach
 *
 * Description:
 *   Interrupt registration function after the system starts.
 *
 ****************************************************************************/

void fc7300_erm_irq_attach(void)
{
  if (fc7300_cpu_index() == 0U)
    {
      irq_attach(g_erm_config.irq, fc7300_erm_isr, &g_erm_config);
#  ifdef CONFIG_ARCH_IRQPRIO
      up_prioritize_irq(g_erm_config.irq, g_erm_config.irq_priority);
#  endif
      up_enable_irq(g_erm_config.irq);
    }
}

/****************************************************************************
 * Name: fc7300_erm_irq_handler
 *
 * Description:
 *   Interrupt entry function used by EIM.
 *
 ****************************************************************************/

void fc7300_erm_irq_handler(void)
{
  fc7300_erm_isr(0u, NULL, &g_erm_config);
}

/****************************************************************************
 * Name: fc7300_erm_clear_flag
 *
 * Description:
 *   Clear all error flags in the ERM (Error Reporting Module) status registers.
 *
 ****************************************************************************/

void fc7300_erm_clear_flag(void)
{
  fc7300_erm_putreg(&g_erm_config, FC7300_ERM_SR0_OFFSET, 0xccccccccul);
  fc7300_erm_putreg(&g_erm_config, FC7300_ERM_SR1_OFFSET, 0xccccccccul);
  fc7300_erm_putreg(&g_erm_config, FC7300_ERM_SR2_OFFSET, 0xccccccccul);
  fc7300_erm_putreg(&g_erm_config, FC7300_ERM_SR3_OFFSET, 0xccccccccul);
}
