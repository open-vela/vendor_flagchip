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
#include "ram_vectors.h"
#include "fc7300_erm.h"
#include "fc7300_fcsmu.h"
#include "fc7300_cmu.h"
#include "fc7300_eim.h"
#include "hardware/fc7300_eim.h"
#include "fc7300_multicore.h"
#include "mpu.h"
#include "nvic.h"
#include "fc7300_mam.h"
#include "fc7300_lowputc.h"
#include "hardware/fc7300_fmc.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#define EIM_CHANNEL_MAX           86U   /* EIM channel(78) + flash(3) + MPU(1) + MAM(3) + FMC(1) */
#define EIM_TYPE_INVALID          0x0U
#define EIM_TYPE_CORE0_MEMORY     0x01U
#define EIM_TYPE_CORE1_MEMORY     0x11U
#define EIM_TYPE_CORE2_MEMORY     0x21U
#define EIM_TYPE_CORE0_ICACHE     0x02U
#define EIM_TYPE_CORE1_ICACHE     0x12U
#define EIM_TYPE_CORE2_ICACHE     0x22U
#define EIM_TYPE_CORE0_DCACHE     0x03U
#define EIM_TYPE_CORE1_DCACHE     0x13U
#define EIM_TYPE_CORE2_DCACHE     0x23U
#define EIM_TYPE_FLASH            0x04U
#define EIM_TYPE_ROM              0x05U
#define EIM_TYPE_MPU              0x06U
#define EIM_TYPE_MAM              0x07U
#define EIM_TYPE_FMC              0x08U

#define EIM_TEST_OPEN             0x01U
#define EIM_TEST_SUCCESS          0x02U
#define EIM_TEST_CLOSE            0x03U
#define EIM_TEST_TIMEOUT          0xFFUL

#define SCB_SHCSR_ADDRESS         0xE000ED24UL    /* (0xE000E000UL+0x0D00UL+0x024UL) */
#define SCB_CFSR_ADDRESS          0xE000ED28UL    /* (0xE000E000UL+0x0D00UL+0x028UL) */
#define NVIC_ISER_ADDRESS         0xE000E100UL    /* (0xE000E000UL+0x0100UL+0x0u) */
#define NVIC_ICER_ADDRESS         0xE000E180UL    /* (0xE000E000UL+0x0100UL+0x80u) */
#define NVIC_ISPR_ADDRESS         0xE000E200UL    /* (0xE000E000UL+0x0100UL+0x100UL) */
#define NVIC_ICPR_ADDRESS         0xE000E280UL    /* (0xE000E000UL+0x0100UL+0x180UL) */
#define VTOR_ADDRESS              0xE000ED08UL    /* (0xE000E000UL+0x0D00UL+0x8u) */

#define FCSMU_NVIC_OFFSET         0x4UL           /* 0x4u */
#define FCSMU_NVIC_MASK           0x00000100UL    /* NVIC Interrupt ID 40 */
#define ERM_NVIC_OFFSET           0x4UL           /* 0x4u */
#define ERM_NVIC_MASK             0x00000400UL    /* NVIC Interrupt ID 42 */
#define CMU0_NVIC_OFFSET          0x4UL           /* 0x4u */
#define CMU0_NVIC_MASK            0x00200000UL    /* NVIC Interrupt ID 53 */
#define CMU1_NVIC_OFFSET          0x4UL           /* 0x4u */
#define CMU1_NVIC_MASK            0x00400000UL    /* NVIC Interrupt ID 54 */
#define CMU2_NVIC_OFFSET          0x4UL           /* 0x4u */
#define CMU2_NVIC_MASK            0x00800000UL    /* NVIC Interrupt ID 55 */
#define CMU3_NVIC_OFFSET          0x4UL           /* 0x4u */
#define CMU3_NVIC_MASK            0x01000000UL    /* NVIC Interrupt ID 56 */
#define CMU4_NVIC_OFFSET          0x10UL          /* 0x10u */
#define CMU4_NVIC_MASK            0x02000000UL    /* NVIC Interrupt ID 153 */

#define SCM_INT_ROUTER_FCSMU      0x400722A0UL    /* FCSMU Interrupt Router 0x40072000UL+0x200+40*4 */
#define SCM_INT_ROUTER_ERM        0x400722A8UL    /* ERM Interrupt Router 0x40072000UL+0x200+42*4 */

#define SCM_MATRIX_STATUS0_ADDR   0x400720B0UL    /* SCM_MATRIX_STATUS0 0x40072000UL + 0xB0 */
#define SCM_MATRIX_STATUS1_ADDR   0x400720B4UL    /* SCM_MATRIX_STATUS1 0x40072000UL + 0xB4 */
#define SCM_MATRIX_STATUS2_ADDR   0x400720B8UL    /* SCM_MATRIX_STATUS2 0x40072000UL + 0xB8 */
#define SCM_MATRIX_STATUS3_ADDR   0x400720BCUL    /* SCM_MATRIX_STATUS3 0x40072000UL + 0xBC */
#define SCM_MATRIX_STATUS4_ADDR   0x400720C0UL    /* SCM_MATRIX_STATUS4 0x40072000UL + 0xC0 */
#define SCM_MATRIX_STATUS5_ADDR   0x400720C4UL    /* SCM_MATRIX_STATUS5 0x40072000UL + 0xC4 */

#define IDLE_STACK_MPU_BASEADDR  (uintptr_t)((uintptr_t)_ebss & (~(CONFIG_IDLETHREAD_STACKSIZE - 1))) /*Aligning Addresses by Size*/

#define FLASHDRV_ADDR                   0x04810200UL
#define FLASH_API_SIZE_8M               0x0UL
#define FLASH_CLEAR_ECC_MASK            0x011UL
#define FLASH_CLEAR_SBC_MASK            0x021UL
#define FLASH_SBC_CHECK_MASK            0x021UL
#define FLASH_STATUS_SUCCESS            0x001UL
#define FLASH_STATUS_ERROR              0x002UL
#define FLASH_STATUS_PARA_ERROR         0x003UL
#define FLASH_STATUS_ECC_ERROR          0x011UL
#define FLASH_STATUS_SBC_ERROR          0x021UL
#define FLASH_STATUS_AEE_ERROR          0x041UL
#define FLASH_STATUS_EDC_ERROR          0x081UL
#define FLASH_STATUS_SPACE_ERROR        0x101UL
#define FLASH_STATUS_TWO_ERROR          0x201UL
#define FLASH_STATUS_BLANKH_ERROR       0x401UL
#define FLASH_STATUS_BLANKL_ERROR       0x801UL
#define FLASH_STATUS_RWW_ERROR          0x1001UL
#define FLASH_STATUS_PEP_ERROR          0x2001UL
#define FLASH_STATUS_PES_ERROR          0x4001UL
#define FLASH_STATUS_HVOP               0x8001UL
#define FLASH_STATUS_PEG_FAILED         0x10001UL
#define FLASH_STATUS_WDG_ERROR          0x20001UL
#define FLASH_STATUS_NVR_TIMEOUT_ERROR  0x40001UL
#define FLASH_STATUS_RE_ERROR           0x80001UL
#define FLASH_STATUS_NVR_LOCK_ERROR     0x100001UL
#define FLASH_STATUS_ARRAY_BP           0x200001UL

/****************************************************************************
 * Private Types
 ****************************************************************************/

struct fc7300_eim_channel_s
{
  uint8_t type;
  uint32_t ctrl_reg;
  uint32_t bus_reg;
  uint32_t test_addr;
};

/* EIM Device Private Data */

struct fc7300_eim_config_s
{
  uint32_t reg_base;        /* EIM register base address */
  struct fc7300_eim_channel_s eim_channel[EIM_CHANNEL_MAX];
};

typedef struct
{
  uint32_t data1;
  uint32_t data2;
  uint32_t data3;
  uint32_t data4;
  uint32_t data5;
  uint32_t size;
  uint32_t dest;
} FLASH_API_ECC_CHECK_CFG_TYPE;

typedef struct
{
  uint32_t RESERVED1[13U];
  uint32_t (*ecc_check)(uint32_t flash_api_cfg);
  uint32_t (*ecc_clear)(uint32_t flash_api_cfg);
  uint32_t RESERVED2[6U];
  uint32_t (*ecc_injection)(FLASH_API_ECC_CHECK_CFG_TYPE * flash_api_cfg, uint32_t size);
  uint32_t (*ecc_injection_close)(void);
  uint32_t RESERVED3[5U];
  uint32_t (*ecc_injection_open)(void);
  uint32_t RESERVED4[1U];
  uint32_t (*ecc_check_p1)(uint32_t flash_api_cfg);
  uint32_t (*ecc_check_p2)(uint32_t flash_api_cfg);
  uint32_t (*ecc_clear_p1)(uint32_t flash_api_cfg);
  uint32_t (*ecc_clear_p2)(uint32_t flash_api_cfg);
}flash_rom_api_entry_t;

typedef uint32_t (*flash_ecc_check_t)(uint32_t flash_api_cfg);
typedef uint32_t (*flash_ecc_clear_t)(uint32_t flash_api_cfg);

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

static inline uint32_t
fc7300_eim_getreg(struct fc7300_eim_config_s *priv, uint16_t offset);
static inline void fc7300_eim_putreg(struct fc7300_eim_config_s *priv,
                                     uint16_t offset, uint32_t value);

static void fc7300_default_isr(void);
static void fc7300_erm_isr(void);
static void fc7300_fcsmu_isr(void);
static void fc7300_cmu0_isr(void);
static void fc7300_cmu1_isr(void);
static void fc7300_cmu2_isr(void);
static void fc7300_cmu3_isr(void);
static void fc7300_cmu4_isr(void);

static void fc7300_eim_memory(uint32_t eim_ch) __attribute__((optimize("O0")));
static void fc7300_eim_icache(uint32_t eim_ch) __attribute__((optimize("O0")));
static void fc7300_eim_dcache(uint32_t eim_ch) __attribute__((optimize("O0")));
static void fc7300_eim_rom(uint32_t eim_ch) __attribute__((optimize("O0")));
static void fc7300_eim_mpu(uint32_t eim_ch) __attribute__((optimize("O0")));
static void fc7300_eim_mam(uint32_t eim_ch) __attribute__((optimize("O0")));
static void fc7300_eim_fmc(void) __attribute__((optimize("O0")));
static void fc7300_eim_flash(uint32_t eim_ch) __attribute__((optimize("O0")));
#if defined(CONFIG_FC7300_EIM_CPU0_LOCKSTEP) || defined(CONFIG_FC7300_EIM_CPU1_LOCKSTEP)
static void fc7300_eim_lockstep(uint8_t core_index) __attribute__((optimize("O0")));
#endif /* defined(CONFIG_FC7300_EIM_CPU0_LOCKSTEP) || defined(CONFIG_FC7300_EIM_CPU1_LOCKSTEP) */

/****************************************************************************
 * Private Data
 ****************************************************************************/

static volatile uint32_t l_eim_test_result = EIM_TEST_CLOSE;
static volatile uint32_t l_eim_timeout;

const void * const fc7300_eim_vtor_table[] aligned_data(VECTAB_ALIGN) =
{
  /* Initial stack */

  [0]                                          = (void (*)(void))0,

  /* Reserved */

  [1 ... 3]                                    = fc7300_default_isr,

  /* Vectors 4 MemManage Fault */

  [4]                                          = fc7300_memmanage_isr,

  /* Vectors 5 Bus Fault */

  [5]                                          = fc7300_busfault_isr,

  /* Reserved */

  [6 ... 55]                                   = fc7300_default_isr,

  /* Vectors 56 Fault Control and Safety Manage Unit */

  [56]                                         = fc7300_fcsmu_isr,

  /* Reserved */

  [57]                                         = fc7300_default_isr,

  /* Vectors 58 ERM single/double bit error correction */

  [58]                                         = fc7300_erm_isr,

  /* Reserved */

  [59 ... 68]                                  = fc7300_default_isr,

  /* Vectors 69 CMU0 Interrupt */

  [69]                                         = fc7300_cmu0_isr,

  /* Vectors 70 CMU1 Interrupt */

  [70]                                         = fc7300_cmu1_isr,

  /* Vectors 71 CMU2 Interrupt */

  [71]                                         = fc7300_cmu2_isr,

  /* Vectors 72 CMU3 Interrupt */

  [72]                                         = fc7300_cmu3_isr,

  /* Reserved */

  [73 ... 168]                                 = fc7300_default_isr,

  /* Vectors 169 CMU4 Interrupt */

  [169]                                        = fc7300_cmu4_isr,
};

static const flash_rom_api_entry_t  *s_flash_driver_func_header =
  (flash_rom_api_entry_t *)FLASHDRV_ADDR;

static struct fc7300_eim_config_s g_eim_config =
{
  .reg_base    = FC7300_EIM_BASE,

  .eim_channel =
  {
    /* 0. EIM_MAM0_S0 */
    {
#if defined(CONFIG_FC7300_EIM_MAM0_S0)
      EIM_TYPE_ROM,
#else
      EIM_TYPE_INVALID,
#endif
      EIM_CTRL_REG_BUS_SEL(0U) | EIM_CTRL_REG_DATA0EIE,
      EIM_BUS_REG_DATA0(3U),
      0x04810200U,
    },

    /* 1. EIM_MAM0_S1 */
    {
#if defined(CONFIG_FC7300_EIM_MAM0_S1)
      EIM_TYPE_ROM,
#else
      EIM_TYPE_INVALID,
#endif
      EIM_CTRL_REG_BUS_SEL(0U) | EIM_CTRL_REG_DATA0EIE,
      EIM_BUS_REG_DATA0(3U),
      0x01000400UL,
    },

    /* 2. EIM_MAM0_S2 */
    {
#if defined(CONFIG_FC7300_EIM_MAM0_S2)
      EIM_TYPE_ROM,
#else
      EIM_TYPE_INVALID,
#endif
      EIM_CTRL_REG_BUS_SEL(0U) | EIM_CTRL_REG_DATA0EIE,
      EIM_BUS_REG_DATA0(3U),
      0x01400400UL,
    },

    /* 3. EIM_MAM0_S3 */
    {
#if defined(CONFIG_FC7300_EIM_MAM0_S3)
      EIM_TYPE_ROM,
#else
      EIM_TYPE_INVALID,
#endif
      EIM_CTRL_REG_BUS_SEL(0U) | EIM_CTRL_REG_DATA0EIE,
      EIM_BUS_REG_DATA0(3U),
      0x04000400UL,
    },

    /* 4. EIM_MAM1_S0 */
    {
#if defined(CONFIG_FC7300_EIM_MAM1_S0)
      EIM_TYPE_CORE0_MEMORY,
#else
      EIM_TYPE_INVALID,
#endif
      EIM_CTRL_REG_BUS_SEL(0U) | EIM_CTRL_REG_DATA0EIE,
      EIM_BUS_REG_DATA0(3U),
      0x21000020U,
    },

    /* 5. EIM_MAM1_S1 */
    {
#if defined(CONFIG_FC7300_EIM_MAM1_S1)
      EIM_TYPE_CORE0_MEMORY,
#else
      EIM_TYPE_INVALID,
#endif
      EIM_CTRL_REG_BUS_SEL(0U) | EIM_CTRL_REG_DATA0EIE,
      EIM_BUS_REG_DATA0(3U),
      0x21020020U,
    },

    /* 6. EIM_MAM1_S2 */
    {
#if defined(CONFIG_FC7300_EIM_MAM1_S2)
      EIM_TYPE_CORE0_MEMORY,
#else
      EIM_TYPE_INVALID,
#endif
      EIM_CTRL_REG_BUS_SEL(0U) | EIM_CTRL_REG_DATA0EIE,
      EIM_BUS_REG_DATA0(3U),
      0x21040020U,
    },

    /* 7. EIM_MAM1_S3 */
    {
#if defined(CONFIG_FC7300_EIM_MAM1_S3)
      EIM_TYPE_CORE0_MEMORY,
#else
      EIM_TYPE_INVALID,
#endif
      EIM_CTRL_REG_BUS_SEL(0U) | EIM_CTRL_REG_DATA0EIE,
      EIM_BUS_REG_DATA0(3U),
      0x22000030U,
    },

    /* 8. EIM_MAM2_S0 Target: AFCB0 */
    {
#if defined(CONFIG_FC7300_EIM_MAM2_S0)
      EIM_TYPE_ROM,
#else
      EIM_TYPE_INVALID,
#endif
      EIM_CTRL_REG_BUS_SEL(0U) | EIM_CTRL_REG_DATA0EIE,
      EIM_BUS_REG_DATA0(3U),
      0x40023000U,
    },

    /* 9. EIM_MAM2_S1 Target: AFCB1 */
    {
#if defined(CONFIG_FC7300_EIM_MAM2_S1)
      EIM_TYPE_ROM,
#else
      EIM_TYPE_INVALID,
#endif
      EIM_CTRL_REG_BUS_SEL(0U) | EIM_CTRL_REG_DATA0EIE,
      EIM_BUS_REG_DATA0(3U),
      0x40437000U,
    },

    /* 10. EIM_MAM2_S2 Target: GPIO */
    {
#if defined(CONFIG_FC7300_EIM_MAM2_S2)
      EIM_TYPE_ROM,
#else
      EIM_TYPE_INVALID,
#endif
      EIM_CTRL_REG_BUS_SEL(0U) | EIM_CTRL_REG_DATA0EIE,
      EIM_BUS_REG_DATA0(3U),
      0x41000000U,
    },

    /* 11. EIM_CPU0_AHBM */
    {
#if defined(CONFIG_FC7300_EIM_CPU0_AHBM)
      EIM_TYPE_CORE0_MEMORY,
#else
      EIM_TYPE_INVALID,
#endif
      EIM_CTRL_REG_BUS_SEL(0U) | EIM_CTRL_REG_DATA0EIE,
      EIM_BUS_REG_DATA0(3U),
      0x21000000u,
    },

    /* 12. EIM_CPU0_AHBP */
    {
#if defined(CONFIG_FC7300_EIM_CPU0_AHBP)
      EIM_TYPE_CORE0_MEMORY,
#else
      EIM_TYPE_INVALID,
#endif
      EIM_CTRL_REG_BUS_SEL(0U) | EIM_CTRL_REG_DATA0EIE,
      EIM_BUS_REG_DATA0(3U),
      0x41000000u,
    },

    /* 13. EIM_CPU1_AHBM */
    {
#if defined(CONFIG_FC7300_EIM_CPU1_AHBM)
      EIM_TYPE_CORE1_MEMORY,
#else
      EIM_TYPE_INVALID,
#endif
      EIM_CTRL_REG_BUS_SEL(0U) | EIM_CTRL_REG_DATA0EIE,
      EIM_BUS_REG_DATA0(3U),
      0x21000000u,
    },

    /* 14. EIM_CPU1_AHBP */
    {
#if defined(CONFIG_FC7300_EIM_CPU1_AHBP)
      EIM_TYPE_CORE1_MEMORY,
#else
      EIM_TYPE_INVALID,
#endif
      EIM_CTRL_REG_BUS_SEL(0U) | EIM_CTRL_REG_DATA0EIE,
      EIM_BUS_REG_DATA0(3U),
      0x41000000u,
    },

    /* 15. EIM_CPU2_AHBM */
    {
#if defined(CONFIG_FC7300_EIM_CPU2_AHBM)
      EIM_TYPE_CORE2_MEMORY,
#else
      EIM_TYPE_INVALID,
#endif
      EIM_CTRL_REG_BUS_SEL(0U),
      0x0U,
      0x0U,
    },

    /* 16. EIM_CPU2_AHBP */
    {
#if defined(CONFIG_FC7300_EIM_CPU2_AHBP)
      EIM_TYPE_CORE2_MEMORY,
#else
      EIM_TYPE_INVALID,
#endif
      EIM_CTRL_REG_BUS_SEL(0U),
      0x0U,
      0x0U,
    },

    /* 17. EIM_ENET */
    {
#if defined(CONFIG_FC7300_EIM_ENET)
      EIM_TYPE_ROM,
#else
      EIM_TYPE_INVALID,
#endif
      EIM_CTRL_REG_BUS_SEL(0U),
      0x0U,
      0x0U,
    },

    /* 18. EIM_HSM */
    {
#if defined(CONFIG_FC7300_EIM_HSM)
      EIM_TYPE_INVALID,
#else
      EIM_TYPE_INVALID,
#endif
      EIM_CTRL_REG_BUS_SEL(0U),
      0x0U,
      0x0U,
    },

    /* 19. EIM_DMA0 */
    {
#if defined(CONFIG_FC7300_EIM_DMA0)
      EIM_TYPE_INVALID,
#else
      EIM_TYPE_INVALID,
#endif
      EIM_CTRL_REG_BUS_SEL(0U),
      0x0U,
      0x0U,
    },

    /* 20. EIM_DMA1 */
    {
#if defined(CONFIG_FC7300_EIM_DMA1)
      EIM_TYPE_INVALID,
#else
      EIM_TYPE_INVALID,
#endif
      EIM_CTRL_REG_BUS_SEL(0U),
      0x0U,
      0x0U,
    },

    /* 21. EIM_CPU0_AHBS */
    {
#if defined(CONFIG_FC7300_EIM_CPU0_AHBS)
      EIM_TYPE_CORE0_MEMORY,
#else
      EIM_TYPE_INVALID,
#endif
      EIM_CTRL_REG_BUS_SEL(0U),
      0x0U,
      0x0U,
    },

    /* 22. EIM_CPU1_AHBS */
    {
#if defined(CONFIG_FC7300_EIM_CPU1_AHBS)
      EIM_TYPE_CORE1_MEMORY,
#else
      EIM_TYPE_INVALID,
#endif
      EIM_CTRL_REG_BUS_SEL(0U),
      0x0U,
      0x0U,
    },

    /* 23. EIM_CPU2_AHBS */
    {
#if defined(CONFIG_FC7300_EIM_CPU2_AHBS)
      EIM_TYPE_CORE2_MEMORY,
#else
      EIM_TYPE_INVALID,
#endif
      EIM_CTRL_REG_BUS_SEL(0U),
      0x0U,
      0x0U,
    },

    /* 24. EIM_CPU0_ITCM */
    {
#if defined(CONFIG_FC7300_EIM_CPU0_ITCM)
      EIM_TYPE_CORE0_MEMORY,
#else
      EIM_TYPE_INVALID,
#endif
      EIM_CTRL_REG_BUS_SEL(0U) | EIM_CTRL_REG_DATA0EIE,
      EIM_BUS_REG_DATA0(10U),
      0x00000020U,
    },

    /* 25. EIM_CPU0_DTCM0 */
    {
#if defined(CONFIG_FC7300_EIM_CPU0_DTCM0)
      EIM_TYPE_CORE0_MEMORY,
#else
      EIM_TYPE_INVALID,
#endif
      EIM_CTRL_REG_BUS_SEL(0U) | EIM_CTRL_REG_DATA0EIE,
      EIM_BUS_REG_DATA0(3U),
      0x20000030U,
    },

    /* 26. EIM_CPU0_DTCM1 */
    {
#if defined(CONFIG_FC7300_EIM_CPU0_DTCM1)
      EIM_TYPE_CORE0_MEMORY,
#else
      EIM_TYPE_INVALID,
#endif
      EIM_CTRL_REG_BUS_SEL(0U) | EIM_CTRL_REG_DATA0EIE,
      EIM_BUS_REG_DATA0(3U),
      0x20000034U,
    },

    /* 27. EIM_CPU1_ITCM */
    {
#if defined(CONFIG_FC7300_EIM_CPU1_ITCM)
      EIM_TYPE_CORE1_MEMORY,
#else
      EIM_TYPE_INVALID,
#endif
      EIM_CTRL_REG_BUS_SEL(0U) | EIM_CTRL_REG_DATA0EIE,
      EIM_BUS_REG_DATA0(10U),
      0x00000020U,
    },

    /* 28. EIM_CPU1_DTCM0 */
    {
#if defined(CONFIG_FC7300_EIM_CPU1_DTCM0)
      EIM_TYPE_CORE1_MEMORY,
#else
      EIM_TYPE_INVALID,
#endif
      EIM_CTRL_REG_BUS_SEL(0U) | EIM_CTRL_REG_DATA0EIE,
      EIM_BUS_REG_DATA0(3U),
      0x20000030U,
    },

    /* 29. EIM_CPU1_DTCM1 */
    {
#if defined(CONFIG_FC7300_EIM_CPU1_DTCM1)
      EIM_TYPE_CORE1_MEMORY,
#else
      EIM_TYPE_INVALID,
#endif
      EIM_CTRL_REG_BUS_SEL(0U) | EIM_CTRL_REG_DATA0EIE,
      EIM_BUS_REG_DATA0(3U),
      0x20000034U,
    },

    /* 30. EIM_CPU2_ITCM */
    {
#if defined(CONFIG_FC7300_EIM_CPU2_ITCM)
      EIM_TYPE_CORE2_MEMORY,
#else
      EIM_TYPE_INVALID,
#endif
      EIM_CTRL_REG_BUS_SEL(0U),
      0x0U,
      0x0U,
    },

    /* 31. EIM_CPU2_DTCM0 */
    {
#if defined(CONFIG_FC7300_EIM_CPU2_DTCM0)
      EIM_TYPE_CORE2_MEMORY,
#else
      EIM_TYPE_INVALID,
#endif
      EIM_CTRL_REG_BUS_SEL(0U),
      0x0U,
      0x0U,
    },

    /* 32. EIM_CPU2_DTCM1 */
    {
#if defined(CONFIG_FC7300_EIM_CPU2_DTCM1)
      EIM_TYPE_CORE2_MEMORY,
#else
      EIM_TYPE_INVALID,
#endif
      EIM_CTRL_REG_BUS_SEL(0U),
      0x0U,
      0x0U,
    },

    /* 33. EIM_SRAM0 */
    {
#if defined(CONFIG_FC7300_EIM_SRAM0)
      EIM_TYPE_CORE0_MEMORY,
#else
      EIM_TYPE_INVALID,
#endif
      EIM_CTRL_REG_BUS_SEL(0U) | EIM_CTRL_REG_DATA0EIE,
      EIM_BUS_REG_DATA0(3U),
      0x21000020U,
    },

    /* 34. EIM_SRAM1 */
    {
#if defined(CONFIG_FC7300_EIM_SRAM1)
      EIM_TYPE_CORE0_MEMORY,
#else
      EIM_TYPE_INVALID,
#endif
      EIM_CTRL_REG_BUS_SEL(0U) | EIM_CTRL_REG_DATA0EIE,
      EIM_BUS_REG_DATA0(2U),
      0x21020040U,
    },

    /* 35. EIM_SRAM2 */
    {
#if defined(CONFIG_FC7300_EIM_SRAM2)
      EIM_TYPE_CORE0_MEMORY,
#else
      EIM_TYPE_INVALID,
#endif
      EIM_CTRL_REG_BUS_SEL(0U) | EIM_CTRL_REG_DATA0EIE,
      EIM_BUS_REG_DATA0(4U),
      0x21040080U,
    },

    /* 36. EIM_CPU0DCACHE_DATA0_01, this channel contain 36~39 */
    {
#if defined(CONFIG_FC7300_EIM_CPU0DCACHE_DATA)
      EIM_TYPE_CORE0_DCACHE,
#else
      EIM_TYPE_INVALID,
#endif
      EIM_CTRL_REG_BUS_SEL(0U) | EIM_CTRL_REG_DATA1EIE | EIM_CTRL_REG_DATA0EIE,
      EIM_BUS_REG_DATA1(55U) | EIM_BUS_REG_DATA0(15U),
      0x0U,
    },

    /* 37. EIM_CPU0DCACHE_DATA0_23 */
    {
      EIM_TYPE_INVALID,
      EIM_CTRL_REG_BUS_SEL(0U),
      0x0U,
      0x0U,
    },

    /* 38. EIM_CPU0DCACHE_DATA1_01 */
    {
      EIM_TYPE_INVALID,
      EIM_CTRL_REG_BUS_SEL(0U),
      0x0U,
      0x0U,
    },

    /* 39. EIM_CPU0DCACHE_DATA1_23 */
    {
      EIM_TYPE_INVALID,
      EIM_CTRL_REG_BUS_SEL(0U),
      0x0U,
      0x0U,
    },

    /* 40. EIM_CPU0DCACHE_TAG_01, this channel contain 40~41 */
    {
#if defined(CONFIG_FC7300_EIM_CPU0DCACHE_TAG)
      EIM_TYPE_CORE0_DCACHE,
#else
      EIM_TYPE_INVALID,
#endif
      EIM_CTRL_REG_BUS_SEL(0U) | EIM_CTRL_REG_DATA1EIE | EIM_CTRL_REG_DATA0EIE,
      EIM_BUS_REG_DATA1(48U) | EIM_BUS_REG_DATA0(15U),
      0x0U,
    },

    /* 41. EIM_CPU0DCACHE_TAG_23 */
    {
      EIM_TYPE_INVALID,
      EIM_CTRL_REG_BUS_SEL(0U),
      0x0U,
      0x0U,
    },

    /* 42. EIM_CPU0ICACHE_DATA0, this channel contain 42~43 */
    {
#if defined(CONFIG_FC7300_EIM_CPU0ICACHE_DATA)
      EIM_TYPE_CORE0_ICACHE,
#else
      EIM_TYPE_INVALID,
#endif
      EIM_CTRL_REG_BUS_SEL(0U) | EIM_CTRL_REG_DATA0EIE,
      EIM_BUS_REG_DATA0(15U),
      0x0U,
    },

    /* 43. EIM_CPU0ICACHE_DATA1 */
    {
      EIM_TYPE_INVALID,
      EIM_CTRL_REG_BUS_SEL(0U),
      0x0U,
      0x0U,
    },

    /* 44. EIM_CPU0ICACHE_TAG */
    {
#if defined(CONFIG_FC7300_EIM_CPU0ICACHE_TAG)
      EIM_TYPE_CORE0_ICACHE,
#else
      EIM_TYPE_INVALID,
#endif
      EIM_CTRL_REG_BUS_SEL(0U) | EIM_CTRL_REG_DATA1EIE | EIM_CTRL_REG_DATA0EIE,
      EIM_BUS_REG_DATA1(45U) | EIM_BUS_REG_DATA0(15U),
      0x0U,
    },

    /* 45. EIM_CPU1DCACHE_DATA0_01, this channel contain 45~48 */
    {
#if defined(CONFIG_FC7300_EIM_CPU1DCACHE_DATA)
      EIM_TYPE_CORE1_DCACHE,
#else
      EIM_TYPE_INVALID,
#endif
      EIM_CTRL_REG_BUS_SEL(0U) | EIM_CTRL_REG_DATA1EIE | EIM_CTRL_REG_DATA0EIE,
      EIM_BUS_REG_DATA1(55U) | EIM_BUS_REG_DATA0(15U),
      0x0U,
    },

    /* 46. EIM_CPU1DCACHE_DATA0_23 */
    {
      EIM_TYPE_INVALID,
      EIM_CTRL_REG_BUS_SEL(0U),
      0x0U,
      0x0U,
    },

    /* 47. EIM_CPU1DCACHE_DATA1_01 */
    {
      EIM_TYPE_INVALID,
      EIM_CTRL_REG_BUS_SEL(0U),
      0x0U,
      0x0U,
    },

    /* 48. EIM_CPU1DCACHE_DATA1_23 */
    {
      EIM_TYPE_INVALID,
      EIM_CTRL_REG_BUS_SEL(0U),
      0x0U,
      0x0U,
    },

    /* 49. EIM_CPU1DCACHE_TAG_01, this channel contain 49~50 */
    {
#if defined(CONFIG_FC7300_EIM_CPU1DCACHE_TAG)
      EIM_TYPE_CORE1_DCACHE,
#else
      EIM_TYPE_INVALID,
#endif
      EIM_CTRL_REG_BUS_SEL(0U) | EIM_CTRL_REG_DATA1EIE | EIM_CTRL_REG_DATA0EIE,
      EIM_BUS_REG_DATA1(48U) | EIM_BUS_REG_DATA0(15U),
      0x0U,
    },

    /* 50. EIM_CPU1DCACHE_TAG_23 */
    {
      EIM_TYPE_INVALID,
      EIM_CTRL_REG_BUS_SEL(0U),
      0x0U,
      0x0U,
    },

    /* 51. EIM_CPU1ICACHE_DATA0, this channel contain 51~52 */
    {
#if defined(CONFIG_FC7300_EIM_CPU1ICACHE_DATA)
      EIM_TYPE_CORE1_ICACHE,
#else
      EIM_TYPE_INVALID,
#endif
      EIM_CTRL_REG_BUS_SEL(0U) | EIM_CTRL_REG_DATA0EIE,
      EIM_BUS_REG_DATA0(15U),
      0x0U,
    },

    /* 52. EIM_CPU1ICACHE_DATA1 */
    {
      EIM_TYPE_INVALID,
      EIM_CTRL_REG_BUS_SEL(0U),
      0x0U,
      0x0U,
    },

    /* 53. EIM_CPU1ICACHE_TAG */
    {
#if defined(CONFIG_FC7300_EIM_CPU1ICACHE_TAG)
      EIM_TYPE_CORE1_ICACHE,
#else
      EIM_TYPE_INVALID,
#endif
      EIM_CTRL_REG_BUS_SEL(0U) | EIM_CTRL_REG_DATA1EIE | EIM_CTRL_REG_DATA0EIE,
      EIM_BUS_REG_DATA1(45U) | EIM_BUS_REG_DATA0(15U),
      0x0U,
    },

    /* 54. EIM_CPU2DCACHE_DATA0_01, this channel contain 54~57 */
    {
#if defined(CONFIG_FC7300_EIM_CPU2DCACHE_DATA)
      EIM_TYPE_CORE2_DCACHE,
#else
      EIM_TYPE_INVALID,
#endif
      EIM_CTRL_REG_BUS_SEL(0U),
      0x0U,
      0x0U,
    },

    /* 55. EIM_CPU2DCACHE_DATA0_23 */
    {
      EIM_TYPE_INVALID,
      EIM_CTRL_REG_BUS_SEL(0U),
      0x0U,
      0x0U,
    },

    /* 56. EIM_CPU2DCACHE_DATA1_01 */
    {
      EIM_TYPE_INVALID,
      EIM_CTRL_REG_BUS_SEL(0U),
      0x0U,
      0x0U,
    },

    /* 57. EIM_CPU2DCACHE_DATA1_23 */
    {
      EIM_TYPE_INVALID,
      EIM_CTRL_REG_BUS_SEL(0U),
      0x0U,
      0x0U,
    },

    /* 58. EIM_CPU2DCACHE_TAG_01, this channel contain 58~59 */
    {
#if defined(CONFIG_FC7300_EIM_CPU2DCACHE_TAG)
      EIM_TYPE_CORE2_DCACHE,
#else
      EIM_TYPE_INVALID,
#endif
      EIM_CTRL_REG_BUS_SEL(0U),
      0x0U,
      0x0U,
    },

    /* 59. EIM_CPU2DCACHE_TAG_23 */
    {
      EIM_TYPE_INVALID,
      EIM_CTRL_REG_BUS_SEL(0U),
      0x0U,
      0x0U,
    },

    /* 60. EIM_CPU2ICACHE_DATA0, this channel contain 60~61 */
    {
#if defined(CONFIG_FC7300_EIM_CPU2ICACHE_DATA)
      EIM_TYPE_CORE2_ICACHE,
#else
      EIM_TYPE_INVALID,
#endif
      EIM_CTRL_REG_BUS_SEL(0U),
      0x0U,
      0x0U,
    },

    /* 61. EIM_CPU2ICACHE_DATA1 */
    {
      EIM_TYPE_INVALID,
      EIM_CTRL_REG_BUS_SEL(0U),
      0x0U,
      0x0U,
    },

    /* 62. EIM_CPU2ICACHE_TAG */
    {
#if defined(CONFIG_FC7300_EIM_CPU2ICACHE_TAG)
      EIM_TYPE_CORE2_ICACHE,
#else
      EIM_TYPE_INVALID,
#endif
      EIM_CTRL_REG_BUS_SEL(0U),
      0x0U,
      0x0U,
    },

    /* 63. EIM_MAM1_S3_SLAVEDOWNSIZE */
    {
#if defined(CONFIG_FC7300_EIM_MAM1_S3_SLAVEDOWNSIZE)
      EIM_TYPE_ROM,
#else
      EIM_TYPE_INVALID,
#endif
      EIM_CTRL_REG_BUS_SEL(0U) | EIM_CTRL_REG_DATA0EIE,
      EIM_BUS_REG_DATA0(3U),
      0x2201F000u,
    },

    /* 64. EIM_MAM1_S3_MASTERDOWNSIZE */
    {
#if defined(CONFIG_FC7300_EIM_MAM1_S3_MASTERDOWNSIZE)
      EIM_TYPE_ROM,
#else
      EIM_TYPE_INVALID,
#endif
      EIM_CTRL_REG_BUS_SEL(0U) | EIM_CTRL_REG_DATA0EIE,
      EIM_BUS_REG_DATA0(3U),
      0x2201F000u,
    },

    /* 65. EIM_DMA0_CFG */
    {
#if defined(CONFIG_FC7300_EIM_DMA0_CFG)
      EIM_TYPE_CORE0_MEMORY,
#else
      EIM_TYPE_INVALID,
#endif
      EIM_CTRL_REG_BUS_SEL(0U) | EIM_CTRL_REG_DATA0EIE,
      EIM_BUS_REG_DATA0(8U),
      0x40009000U,
    },

    /* 66. EIM_DMA1_CFG */
    {
#if defined(CONFIG_FC7300_EIM_DMA1_CFG)
      EIM_TYPE_CORE1_MEMORY,
#else
      EIM_TYPE_INVALID,
#endif
      EIM_CTRL_REG_BUS_SEL(0U) | EIM_CTRL_REG_DATA0EIE,
      EIM_BUS_REG_DATA0(18U),
      0x40409000U,
    },

    /* 67. EIM_ROM_ECC */
    {
#if defined(CONFIG_FC7300_EIM_ROM_ECC)
      EIM_TYPE_ROM,
#else
      EIM_TYPE_INVALID,
#endif
      EIM_CTRL_REG_BUS_SEL(0U) | EIM_CTRL_REG_DATA0EIE,
      EIM_BUS_REG_DATA0(22U),
      0x04810200U,
    },

    /* 68. EIM_MAM1_S3_MONITOR */
    {
#if defined(CONFIG_FC7300_EIM_MAM1_S3_MONITOR)
      EIM_TYPE_CORE0_MEMORY,
#else
      EIM_TYPE_INVALID,
#endif
      EIM_CTRL_REG_BUS_SEL(0U) | EIM_CTRL_REG_DATA0EIE,
      EIM_BUS_REG_DATA0(2U),
      0x22000000u,
    },

    /* 69. EIM_CPU0AHBM0_AHBM1_MONITOR */
    {
#if defined(CONFIG_FC7300_EIM_CPU0AHBM0_AHBM1_MONITOR)
      EIM_TYPE_CORE0_MEMORY,
#else
      EIM_TYPE_INVALID,
#endif
      EIM_CTRL_REG_BUS_SEL(0U) | EIM_CTRL_REG_DATA0EIE,
      EIM_BUS_REG_DATA0(2U),
      0x21000000u,
    },

    /* 70. EIM_CPU0AHBP_AHBS_MONITOR */
    {
#if defined(CONFIG_FC7300_EIM_CPU0AHBP_AHBS_MONITOR)
      EIM_TYPE_CORE0_MEMORY,
#else
      EIM_TYPE_INVALID,
#endif
      EIM_CTRL_REG_BUS_SEL(0U) | EIM_CTRL_REG_DATA0EIE,
      EIM_BUS_REG_DATA0(2U),
      0x41000000u,
    },

    /* 71. EIM_CPU1AHBM0_AHBM1_MONITOR */
    {
#if defined(CONFIG_FC7300_EIM_CPU1AHBM0_AHBM1_MONITOR)
      EIM_TYPE_CORE1_MEMORY,
#else
      EIM_TYPE_INVALID,
#endif
      EIM_CTRL_REG_BUS_SEL(0U) | EIM_CTRL_REG_DATA0EIE,
      EIM_BUS_REG_DATA0(2U),
      0x21000000u,
    },

    /* 72. EIM_CPU1AHBP_AHBS_MONITOR */
    {
#if defined(CONFIG_FC7300_EIM_CPU1AHBP_AHBS_MONITOR)
      EIM_TYPE_CORE1_MEMORY,
#else
      EIM_TYPE_INVALID,
#endif
      EIM_CTRL_REG_BUS_SEL(0U) | EIM_CTRL_REG_DATA0EIE,
      EIM_BUS_REG_DATA0(2U),
      0x41000000u,
    },

    /* 73. EIM_CPU2AHBM0_AHBM1_MONITOR */
    {
#if defined(CONFIG_FC7300_EIM_CPU2AHBM0_AHBM1_MONITOR)
      EIM_TYPE_INVALID,
#else
      EIM_TYPE_INVALID,
#endif
      EIM_CTRL_REG_BUS_SEL(0U),
      0x0U,
      0x0U,
    },

    /* 74. EIM_CPU2AHBP_AHBS_MONITOR */
    {
#if defined(CONFIG_FC7300_EIM_CPU2AHBP_AHBS_MONITOR)
      EIM_TYPE_INVALID,
#else
      EIM_TYPE_INVALID,
#endif
      EIM_CTRL_REG_BUS_SEL(0U),
      0x0U,
      0x0U,
    },

    /* 75. EIM_MAM2_S2_SLAVEDOWNSIZE */
    {
#if defined(CONFIG_FC7300_EIM_MAM2_S2_SLAVEDOWNSIZE)
      EIM_TYPE_ROM,
#else
      EIM_TYPE_INVALID,
#endif
      EIM_CTRL_REG_BUS_SEL(0U) | EIM_CTRL_REG_DATA0EIE,
      EIM_BUS_REG_DATA0(2U),
      0x41000000u,
    },

    /* 76. EIM_MAM2_S2_MASTERDOWNSIZE */
    {
#if defined(CONFIG_FC7300_EIM_MAM2_S2_MASTERDOWNSIZE)
      EIM_TYPE_ROM,
#else
      EIM_TYPE_INVALID,
#endif
      EIM_CTRL_REG_BUS_SEL(0U) | EIM_CTRL_REG_DATA0EIE,
      EIM_BUS_REG_DATA0(2U),
      0x41000000u,
    },

    /* 77. EIM_RAM_DECODER_MONITOR */
    {
#if defined(CONFIG_FC7300_EIM_RAM_DECODER_MONITOR)
      EIM_TYPE_CORE0_MEMORY,
#else
      EIM_TYPE_INVALID,
#endif
      EIM_CTRL_REG_BUS_SEL(0U) | EIM_CTRL_REG_ADDREIE,
      EIM_BUS_REG_ADDR(4U),
      0x2101F000u,
    },

    /* 78. PFLASH0
     * Addr - 0x01000400UL 
     * ECC  - 0x03004351UL
     *        0x0300C4AAUL
     *        0x03003F2AUL
     *        0x0300392AUL
     *        0x03003A2AUL
     */
    {
#if defined(CONFIG_FC7300_EIM_PFLASH0)
      EIM_TYPE_FLASH,
#else
      EIM_TYPE_INVALID,
#endif
      0xC4AA3F2AUL,
      0x392A3A2AUL,
      0x01004351UL,
    },

    /* 79. PFLASH1
     * Addr - 0x01400400UL 
     * ECC  - 0x030041D1UL
     *        0x0300C62AUL
     *        0x03003DAAUL
     *        0x03003BAAUL
     *        0x030038AAUL
     */
    {
#if defined(CONFIG_FC7300_EIM_PFLASH1)
      EIM_TYPE_FLASH,
#else
      EIM_TYPE_INVALID,
#endif
      0xC62A3DAAUL,
      0x3BAA38AAUL,
      0x014041D1UL,
    },

    /* 80. DFLASH
     * Addr - 0x04000400UL 
     * ECC  - 0x03005B51UL
     *        0x0300DCAAUL
     *        0x0300272AUL
     *        0x0300212AUL
     *        0x0300222AUL
     */
    {
#if defined(CONFIG_FC7300_EIM_DFLASH)
      EIM_TYPE_FLASH,
#else
      EIM_TYPE_INVALID,
#endif
      0xDCAA272AUL,
      0x212A222AUL,
      0x04005B51UL,
    },

    /* 81. MPU */
    {
#if defined(CONFIG_FC7300_EIM_MPU)
      EIM_TYPE_MPU,
      0x0U,
      0x0U,
      CONFIG_FC7300_EIM_MPU_ADDRESS,
#else
      EIM_TYPE_INVALID,
      0x0U,
      0x0U,
      0x0U,
#endif
    },

    /* 82. MAM0 */
    {
#if defined(CONFIG_FC7300_EIM_MAM0)
      EIM_TYPE_MAM,
#else
      EIM_TYPE_INVALID,
#endif
      0U,
      96U,
      0x0403A004UL,
    },

    /* 83. MAM1 */
    {
#if defined(CONFIG_FC7300_EIM_MAM1)
      EIM_TYPE_MAM,
#else
      EIM_TYPE_INVALID,
#endif
      1U,
      4U,
      0x21010008UL,
    },

    /* 84. MAM2 */
    {
#if defined(CONFIG_FC7300_EIM_MAM2)
      EIM_TYPE_MAM,
#else
      EIM_TYPE_INVALID,
#endif
      2U,
      35U,
      0x40023010UL,
    },

    /* 85. FMC buffer */
    {
#if defined(CONFIG_FC7300_EIM_FMC)
      EIM_TYPE_FMC,
#else
      EIM_TYPE_INVALID,
#endif
      0U,
      0U,
      0U,
    },
  },
};
static volatile uint32_t eim_value;

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: fc7300_eim_getreg
 *
 * Description:
 *   Get a 32-bit register value by offset
 *
 * Input Parameters:
 *   priv   - EIM device private data
 *   offset - Register offset with respect to the base address of the EiM
 *            peripheral
 *
 * Returned Value:
 *   The 32-bit value retrieved from the register
 *
 ****************************************************************************/

static inline uint32_t
fc7300_eim_getreg(struct fc7300_eim_config_s *priv, uint16_t offset)
{
  return getreg32(priv->reg_base + offset);
}

/****************************************************************************
 * Name: fc7300_eim_putreg
 *
 * Description:
 *  Put a 32-bit register value by offset
 *
 * Input Parameters:
 *   priv   - EIM device private data
 *   offset - Register offset with respect to the base address of the EIM
 *            peripheral
 *   value  - The 32-bit value that should be put into the register
 *
 ****************************************************************************/

static inline void fc7300_eim_putreg(struct fc7300_eim_config_s *priv,
                                     uint16_t offset, uint32_t value)
{
  putreg32(value, priv->reg_base + offset);
}

/****************************************************************************
 * Name: fc7300_default_isr
 *
 * Description:
 *   Default interrupt service routine for FC7300.
 *
 ****************************************************************************/

static void fc7300_default_isr(void)
{
  uint32_t i;

  /* Clear unexpected interrupts */

  for (i = 0; i < 8u; ++i)
    {
      *(volatile uint32_t *)(NVIC_ICPR_ADDRESS + i * 4) = *(volatile uint32_t *)(NVIC_ISPR_ADDRESS + i * 4);
    }
}

/****************************************************************************
 * Name: fc7300_erm_isr
 *
 * Description:
 *   ERM interrupt service routine for FC7300.
 *
 ****************************************************************************/

static void fc7300_erm_isr(void)
{
  fc7300_eim_putreg(&g_eim_config, FC7300_EIM_CR_OFFSET, 0x0U);
  __asm __volatile__("dsb":::"memory");

  l_eim_test_result = EIM_TEST_SUCCESS;
#if defined(CONFIG_FC7300_ERM)
  fc7300_erm_irq_handler();
#endif
}

/****************************************************************************
 * Name: fc7300_fcsmu_isr
 *
 * Description:
 *   FCSMU interrupt service routine for FC7300.
 *
 ****************************************************************************/

static void fc7300_fcsmu_isr(void)
{
  fc7300_eim_putreg(&g_eim_config, FC7300_EIM_CR_OFFSET, 0x0U);
  __asm __volatile__("dsb":::"memory");

#if defined(CONFIG_FC7300_EIM_CPU0_LOCKSTEP)
  fc7300_eim_putreg(&g_eim_config,
                    FC7300_EIM_CPU0_LOCKSTEP_OFFSET,
                    EIM_CPU_LOCKSTEP_LOCKSTEP_MON0_CLR);
  __asm __volatile__("dsb":::"memory");
#endif /* defined(CONFIG_FC7300_EIM_CPU0_LOCKSTEP) */

#if defined(CONFIG_FC7300_EIM_CPU1_LOCKSTEP)
  fc7300_eim_putreg(&g_eim_config,
                    FC7300_EIM_CPU1_LOCKSTEP_OFFSET,
                    EIM_CPU_LOCKSTEP_LOCKSTEP_MON0_CLR);
  __asm __volatile__("dsb":::"memory");
#endif /* defined(CONFIG_FC7300_EIM_CPU1_LOCKSTEP) */

  l_eim_test_result = EIM_TEST_SUCCESS;

#if defined(CONFIG_FC7300_EIM_CMU1)
  fc7300_cmu_recovery(1U);
#endif /* defined(CONFIG_FC7300_EIM_CMU1) */

#if defined(CONFIG_FC7300_EIM_CMU2)
  fc7300_cmu_recovery(2U);
#endif /* defined(CONFIG_FC7300_EIM_CMU2) */

#if defined(CONFIG_FC7300_EIM_CMU4)
  fc7300_cmu_recovery(4U);
#endif /* defined(CONFIG_FC7300_EIM_CMU4) */

#if defined(CONFIG_FC7300_FCSMU)
  fc7300_fcsmu_irq_handler();
#endif
}

/****************************************************************************
 * Name: fc7300_cmu0_isr
 *
 * Description:
 *   CMU0 interrupt service routine for FC7300.
 *
 ****************************************************************************/

static void fc7300_cmu0_isr(void)
{
#if defined(CONFIG_FC7300_CMU0)
  fc7300_cmu_irq_handler(0U);
#endif
}

/****************************************************************************
 * Name: fc7300_cmu1_isr
 *
 * Description:
 *   CMU0 interrupt service routine for FC7300.
 *
 ****************************************************************************/

static void fc7300_cmu1_isr(void)
{
#if defined(CONFIG_FC7300_CMU1)
  fc7300_cmu_irq_handler(1U);
#endif
}

/****************************************************************************
 * Name: fc7300_cmu2_isr
 *
 * Description:
 *   CMU0 interrupt service routine for FC7300.
 *
 ****************************************************************************/

static void fc7300_cmu2_isr(void)
{
#if defined(CONFIG_FC7300_CMU2)
  fc7300_cmu_irq_handler(2U);
#endif
}

/****************************************************************************
 * Name: fc7300_cmu3_isr
 *
 * Description:
 *   CMU0 interrupt service routine for FC7300.
 *
 ****************************************************************************/

static void fc7300_cmu3_isr(void)
{
#if defined(CONFIG_FC7300_CMU3)
  fc7300_cmu_irq_handler(3U);
#endif
}

/****************************************************************************
 * Name: fc7300_cmu4_isr
 *
 * Description:
 *   CMU0 interrupt service routine for FC7300.
 *
 ****************************************************************************/

static void fc7300_cmu4_isr(void)
{
#if defined(CONFIG_FC7300_CMU4)
  fc7300_cmu_irq_handler(4U);
#endif
}

/****************************************************************************
 * Name: fc7300_eim_putreg
 *
 * Description:
 *   Configures the EIM channel for memory operations.
 *
 * Input Parameters:
 *   eim_ch - The EIM channel number to configure.
 *
 ****************************************************************************/

static void fc7300_eim_memory(uint32_t eim_ch)
{
  l_eim_test_result = EIM_TEST_OPEN;
  l_eim_timeout = EIM_TEST_TIMEOUT;

  fc7300_eim_putreg(&g_eim_config, FC7300_EIM_BUS_REG_OFFSET(0U),
    g_eim_config.eim_channel[eim_ch].bus_reg);
  fc7300_eim_putreg(&g_eim_config, FC7300_EIM_CTRL_REG_OFFSET(eim_ch),
    g_eim_config.eim_channel[eim_ch].ctrl_reg);

  /* re-init memory ecc */

  eim_value = *(uint32_t *)g_eim_config.eim_channel[eim_ch].test_addr;
  *(uint32_t *)g_eim_config.eim_channel[eim_ch].test_addr = eim_value;

  fc7300_eim_putreg(&g_eim_config, FC7300_EIM_CR_OFFSET, 0x01U);
  __asm __volatile__("dsb":::"memory");

  eim_value = *(uint32_t *)g_eim_config.eim_channel[eim_ch].test_addr;
  __asm __volatile__("dsb":::"memory");

  while ((l_eim_test_result != EIM_TEST_SUCCESS) &&\
         (l_eim_timeout != 0))
    {
      l_eim_timeout--;
    }

  /* Disable global ECC injection */

  fc7300_eim_putreg(&g_eim_config, FC7300_EIM_CR_OFFSET, 0x0U);
  __asm __volatile__("dsb":::"memory");

  fc7300_eim_putreg(&g_eim_config, FC7300_EIM_BUS_REG_OFFSET(0U), 0U);
  fc7300_eim_putreg(&g_eim_config, FC7300_EIM_CTRL_REG_OFFSET(eim_ch), 0U);
}

/****************************************************************************
 * Name: fc7300_eim_icache
 *
 * Description:
 *   Configures the EIM channel for icache operations.
 *
 * Input Parameters:
 *   eim_ch - The EIM channel number to configure.
 *
 ****************************************************************************/

static void fc7300_eim_icache(uint32_t eim_ch)
{
  uint32_t i;

  fc7300_eim_putreg(&g_eim_config, FC7300_EIM_BUS_REG_OFFSET(0U),
    g_eim_config.eim_channel[eim_ch].bus_reg);
  fc7300_eim_putreg(&g_eim_config, FC7300_EIM_CTRL_REG_OFFSET(eim_ch),
    g_eim_config.eim_channel[eim_ch].ctrl_reg);
  
  /* When Eim ICahce Data, enable data0/data1 */
  if ((eim_ch == 42U) || (eim_ch == 51U) || (eim_ch == 60U))
    {
      fc7300_eim_putreg(&g_eim_config, FC7300_EIM_CTRL_REG_OFFSET(eim_ch+1),
        g_eim_config.eim_channel[eim_ch].ctrl_reg);
    }

  up_enable_icache();

  fc7300_eim_putreg(&g_eim_config, FC7300_EIM_CR_OFFSET, 0x01U);
  __asm __volatile__("dsb":::"memory");
  l_eim_test_result = EIM_TEST_OPEN;
  for (i = 0u; i < 100; ++i)
    {
      /*According to the simulation results of FlagChip Micro, to trigger the fault interrupt of ICache, 
      more instructions need to be added to the While loop. 
      Because when there are fewer instructions, 
      the CPU will only fetch the value from the prefetched buffer and will not go to the ICache to fetch the value, 
      and it will not be able to trigger the fault interrupt.*/
      eim_value = i * 15 / 11 + 3;
      eim_value = i * 14 / 12 + 4;
      eim_value = i * 13 / 13 + 5;
      eim_value = i * 12 / 14 + 6;
      if (l_eim_test_result == EIM_TEST_SUCCESS)
      {
        break;
      }
    }

  fc7300_eim_putreg(&g_eim_config, FC7300_EIM_CR_OFFSET, 0x0U);
  __asm __volatile__("dsb":::"memory");

  up_disable_icache();

  fc7300_eim_putreg(&g_eim_config, FC7300_EIM_BUS_REG_OFFSET(0U), 0U);
  fc7300_eim_putreg(&g_eim_config, FC7300_EIM_CTRL_REG_OFFSET(eim_ch), 0U);

  /* When Eim ICahce Data, disable data0/data1 */
  if ((eim_ch == 42U) || (eim_ch == 51U) || (eim_ch == 60U))
  {
    fc7300_eim_putreg(&g_eim_config, FC7300_EIM_CTRL_REG_OFFSET(eim_ch+1), 0U);
  }
}

/****************************************************************************
 * Name: fc7300_eim_dcache
 *
 * Description:
 *   Configures the EIM channel for dcache operations.
 *
 * Input Parameters:
 *   eim_ch - The EIM channel number to configure.
 *
 ****************************************************************************/

static void fc7300_eim_dcache(uint32_t eim_ch)
{
  uint32_t i, j;

  up_enable_dcache();

  /* When Eim DCahce Data, enable data0_01\data0_23\data1_01\data1_23 */

  if ((eim_ch == 36U) || (eim_ch == 45U) || (eim_ch == 54U))
  {
      for (i = 0; i < 100; ++i)
        {
          eim_value = *(uint32_t *)(0x21000000 + i * 4);
        }

      fc7300_eim_putreg(&g_eim_config, FC7300_EIM_BUS_REG_OFFSET(0U),
        g_eim_config.eim_channel[eim_ch].bus_reg);
     
        for (j = 0; j < 8; ++j)
        {
          fc7300_eim_putreg(&g_eim_config, FC7300_EIM_CTRL_REG_OFFSET(eim_ch + ((j & 0x6U) >> 1U)),
            (j & 0x1U) + 1U);
          fc7300_eim_putreg(&g_eim_config, FC7300_EIM_CR_OFFSET, 0x01U);
          __asm __volatile__("dsb":::"memory");

          l_eim_test_result = EIM_TEST_OPEN;
          for (i = 0; i < 100; ++i)
            {
              eim_value = *(uint32_t *)(0x21000000 + i * 4);
              if (l_eim_test_result == EIM_TEST_SUCCESS)
              {
                break;
              }
            }

          fc7300_eim_putreg(&g_eim_config, FC7300_EIM_CR_OFFSET, 0x0U);
          __asm __volatile__("dsb":::"memory");

          fc7300_eim_putreg(&g_eim_config, FC7300_EIM_CTRL_REG_OFFSET(eim_ch + ((j & 0x6U) >> 1U)),
            0U);
        }
  }

  /* When Eim DCahce Tag, enable data0_tag\data1_tag */

  else if ((eim_ch == 40U) || (eim_ch == 49U) || (eim_ch == 58U))
  {
      for (j = 0; j < 4; ++j)
      {
       for (i = 0; i < 100; ++i)
       {
         eim_value = *(uint32_t *)(0x21000000 + 0x800 * j + i * 4);
       }

        fc7300_eim_putreg(&g_eim_config, FC7300_EIM_BUS_REG_OFFSET(0U),
          g_eim_config.eim_channel[eim_ch].bus_reg);
        fc7300_eim_putreg(&g_eim_config, FC7300_EIM_CTRL_REG_OFFSET(eim_ch + ((j & 0x2U) >> 1U)),
          (j & 0x1U) + 1U);
        fc7300_eim_putreg(&g_eim_config, FC7300_EIM_CR_OFFSET, 0x01U);
        __asm __volatile__("dsb":::"memory");

        l_eim_test_result = EIM_TEST_OPEN;
        for (i = 0; i < 100; ++i)
          {
            eim_value = *(uint32_t *)(0x21000000 + 0x800 * j + i * 4);
            if (l_eim_test_result == EIM_TEST_SUCCESS)
            {
              break;
            }
          }

        fc7300_eim_putreg(&g_eim_config, FC7300_EIM_CR_OFFSET, 0x0U);
        __asm __volatile__("dsb":::"memory");

        fc7300_eim_putreg(&g_eim_config, FC7300_EIM_CTRL_REG_OFFSET(eim_ch + ((j & 0x2U) >> 1U)),
          0U);
      }
  }
  else
  {

  }

  up_disable_dcache();

  fc7300_eim_putreg(&g_eim_config, FC7300_EIM_BUS_REG_OFFSET(0U), 0U);
  fc7300_eim_putreg(&g_eim_config, FC7300_EIM_CTRL_REG_OFFSET(eim_ch), 0U);

  /* When Eim DCahce Data, disable data0_01\data0_23\data1_01\data1_23 */

  if ((eim_ch == 36U) || (eim_ch == 45U) || (eim_ch == 54U))
  {
    fc7300_eim_putreg(&g_eim_config, FC7300_EIM_CTRL_REG_OFFSET(eim_ch+1), 0U);
    fc7300_eim_putreg(&g_eim_config, FC7300_EIM_CTRL_REG_OFFSET(eim_ch+2), 0U);
    fc7300_eim_putreg(&g_eim_config, FC7300_EIM_CTRL_REG_OFFSET(eim_ch+3), 0U);
  }

  /* When Eim DCahce Tag, disable data0_tag\data1_tag */

  else if ((eim_ch == 40U) || (eim_ch == 49U) || (eim_ch == 58U))
  {
    fc7300_eim_putreg(&g_eim_config, FC7300_EIM_CTRL_REG_OFFSET(eim_ch+1), 0U);
  }
  else
  {

  }
}

/****************************************************************************
 * Name: fc7300_eim_rom
 *
 * Description:
 *   Configures the EIM channel for rom operations.
 *
 * Input Parameters:
 *   eim_ch - The EIM channel number to configure.
 *
 ****************************************************************************/

static void fc7300_eim_rom(uint32_t eim_ch)
{
  l_eim_test_result = EIM_TEST_OPEN;
  l_eim_timeout = EIM_TEST_TIMEOUT;

  fc7300_eim_putreg(&g_eim_config, FC7300_EIM_BUS_REG_OFFSET(0U),
    g_eim_config.eim_channel[eim_ch].bus_reg);
  fc7300_eim_putreg(&g_eim_config, FC7300_EIM_CTRL_REG_OFFSET(eim_ch),
    g_eim_config.eim_channel[eim_ch].ctrl_reg);

  fc7300_eim_putreg(&g_eim_config, FC7300_EIM_CR_OFFSET, 0x01U);
  __asm __volatile__("dsb":::"memory");

  eim_value = *(uint32_t *)g_eim_config.eim_channel[eim_ch].test_addr;
  __asm __volatile__("dsb":::"memory");

  while ((l_eim_test_result != EIM_TEST_SUCCESS) &&\
         (l_eim_timeout != 0))
    {
      l_eim_timeout--;
    }

  /* Disable global ECC injection */

  fc7300_eim_putreg(&g_eim_config, FC7300_EIM_CR_OFFSET, 0x0U);
  __asm __volatile__("dsb":::"memory");

  fc7300_eim_putreg(&g_eim_config, FC7300_EIM_BUS_REG_OFFSET(0U), 0U);
  fc7300_eim_putreg(&g_eim_config, FC7300_EIM_CTRL_REG_OFFSET(eim_ch), 0U);
}

/****************************************************************************
 * Name: fc7300_eim_mpu
 *
 * Description:
 *   Injects MPU fault.
 *
 * Input Parameters:
 *   eim_ch - The EIM channel number to configure.
 *
 ****************************************************************************/

static void fc7300_eim_mpu(uint32_t eim_ch)
{
#if defined(CONFIG_FC7300_EIM_MPU)
  uint32_t region;

  mpu_reset();
  region = mpu_configure_region(g_eim_config.eim_channel[eim_ch].test_addr,
                                0x100UL,            /* 256Bytes Region for test */
                                MPU_RASR_TEX_SO |   /* Strongly Ordered */
                                MPU_RASR_AP_NONO |  /* P:None U:None */
                                MPU_RASR_XN |       /* Instruction access disable */
                                MPU_RASR_ENABLE);   /* Region enable */
  mpu_control(true, true, true);
  *(volatile uint32_t *)SCB_SHCSR_ADDRESS |= (1UL << 16U);  /* Enable MemManage fault */

  eim_value = *(uint32_t *)(g_eim_config.eim_channel[eim_ch].test_addr + 4UL);
  __asm __volatile__("dsb":::"memory");

  *(volatile uint32_t *)SCB_SHCSR_ADDRESS &= ~(1UL << 16U); /* Disable MemManage fault */
  mpu_control(false, false, false);
  mpu_freeregion(region);
  mpu_reset();
#endif
}

/****************************************************************************
 * Name: fc7300_eim_mam
 *
 * Description:
 *   Injects MAM fault.
 *
 * Input Parameters:
 *   eim_ch - The EIM channel number to configure.
 *
 ****************************************************************************/

static void fc7300_eim_mam(uint32_t eim_ch)
{
#if defined(CONFIG_FC7300_MAM)
  fc7300_mam_injection_open(g_eim_config.eim_channel[eim_ch].ctrl_reg,
    g_eim_config.eim_channel[eim_ch].bus_reg >> 3U,
    g_eim_config.eim_channel[eim_ch].bus_reg & 0x7U);
  *(volatile uint32_t *)SCB_SHCSR_ADDRESS |= (1UL << 17U);  /* Enable Bus fault */

  eim_value = *(uint32_t *)(g_eim_config.eim_channel[eim_ch].test_addr);
  __asm __volatile__("dsb":::"memory");

  *(volatile uint32_t *)SCB_SHCSR_ADDRESS &= ~(1UL << 17U); /* Disable Bus fault */
  fc7300_mam_injection_close(g_eim_config.eim_channel[eim_ch].ctrl_reg,
    g_eim_config.eim_channel[eim_ch].bus_reg >> 3U,
    g_eim_config.eim_channel[eim_ch].bus_reg & 0x7U);
#endif
}

/****************************************************************************
 * Name: fc7300_eim_fmc
 *
 * Description:
 *   Injects FMC buffer fault.
 *
 ****************************************************************************/

static void fc7300_eim_fmc(void)
{
#if defined(CONFIG_FC7300_EIM_FMC)
  putreg32(0x00050005UL, FC7300_FEIPC);
  putreg32(0x77770000UL, FC7300_FEEC);
  __asm __volatile__("dsb":::"memory");

  putreg32(0UL, FC7300_FEIPC);
  putreg32(0UL, FC7300_FEEC);
#endif
}

/****************************************************************************
 * Name: fc7300_eim_flash
 *
 * Description:
 *   Injects ECC into flash memory at a specified address.
 *
 * Input Parameters:
 *   eim_ch - The EIM channel number to configure.
 *
 ****************************************************************************/

static void fc7300_eim_flash(uint32_t eim_ch)
{
  FLASH_API_ECC_CHECK_CFG_TYPE cfg;
  uint32_t u32Addr = (g_eim_config.eim_channel[eim_ch].test_addr & 0xFFFF0000UL) | 0x400UL;
  uint32_t u32ExpectedEcc[5];
  uint32_t u32LoopI;
  uint32_t u32Temp;
  flash_ecc_check_t flash_ecc_check = (s_flash_driver_func_header)->ecc_check;
  flash_ecc_clear_t flash_ecc_clear = (s_flash_driver_func_header)->ecc_clear;

  /* Prepare Expected Ecc Array, Prepare function pointer and Close FMC buffer */

  u32ExpectedEcc[0] = 0x03000000UL | (g_eim_config.eim_channel[eim_ch].test_addr & 0xFFFFUL);
  u32ExpectedEcc[1] = 0x03000000UL | ((g_eim_config.eim_channel[eim_ch].ctrl_reg >> 16U) & 0xFFFFUL);
  u32ExpectedEcc[2] = 0x03000000UL | (g_eim_config.eim_channel[eim_ch].ctrl_reg & 0xFFFFUL);
  u32ExpectedEcc[3] = 0x03000000UL | ((g_eim_config.eim_channel[eim_ch].bus_reg >> 16U) & 0xFFFFUL);
  u32ExpectedEcc[4] = 0x03000000UL | (g_eim_config.eim_channel[eim_ch].bus_reg & 0xFFFFUL);
  if (eim_ch == 78U)
    {
      flash_ecc_check = (s_flash_driver_func_header)->ecc_check;
      flash_ecc_clear = (s_flash_driver_func_header)->ecc_clear;

      *(volatile uint32_t *)FC7300_FAPC0 = 0;
    }
  else if (eim_ch == 79U)
    {
      flash_ecc_check = (s_flash_driver_func_header)->ecc_check_p1;
      flash_ecc_clear = (s_flash_driver_func_header)->ecc_clear_p1;

      *(volatile uint32_t *)FC7300_FAPC1 = 0;
    }
  else if (eim_ch == 80U)
    {
      flash_ecc_check = (s_flash_driver_func_header)->ecc_check_p2;
      flash_ecc_clear = (s_flash_driver_func_header)->ecc_clear_p2;

      *(volatile uint32_t *)FC7300_FAPC2 = 0;
    }
  else
    {

    }

  *(volatile uint32_t *)FC7300_FEEC = 0x80;

  cfg.data1 = 0x535EB1A7UL; /* injection in bit0 */
  cfg.data2 = 0x4075E981UL;
  cfg.data3 = 0x82196404UL;
  cfg.data4 = 0xDB8FFAB7UL;
  cfg.size  = FLASH_API_SIZE_8M;
  cfg.dest  = u32Addr;

  /* Close NVIC */

  *(volatile uint32_t *)(NVIC_ICER_ADDRESS + ERM_NVIC_OFFSET) = ERM_NVIC_MASK;
  *(volatile uint32_t *)(NVIC_ICER_ADDRESS + FCSMU_NVIC_OFFSET) = FCSMU_NVIC_MASK;

  s_flash_driver_func_header->ecc_injection_open();

  /* Search which space can generate single bit error */

  for (u32LoopI = 0U; u32LoopI < 5U; ++u32LoopI)
    {
      cfg.data5 = u32ExpectedEcc[u32LoopI]; /* Expected ECC value */
      u32Temp = s_flash_driver_func_header->ecc_injection(&cfg, FLASH_API_SIZE_8M);
      if (u32Temp == FLASH_STATUS_SUCCESS)
        {
          eim_value = *(uint32_t *)u32Addr;
          if (FLASH_STATUS_SBC_ERROR == (FLASH_STATUS_SBC_ERROR & flash_ecc_check((uint32_t)FLASH_SBC_CHECK_MASK)))
            {
              fc7300_erm_clear_flag();
              fc7300_fcsmu_clear_fault(0xFFFFFFFFUL);
              flash_ecc_clear((uint32_t)FLASH_CLEAR_SBC_MASK);
              u32Temp = 0x55AAUL;
              break;
            }

          fc7300_erm_clear_flag();
          fc7300_fcsmu_clear_fault(0xFFFFFFFFUL);
          flash_ecc_clear((uint32_t)FLASH_CLEAR_ECC_MASK);
        }
    }

  /* Clear Pending and close nvic */

  *(volatile uint32_t *)(NVIC_ICPR_ADDRESS + ERM_NVIC_OFFSET) = ERM_NVIC_MASK;
  *(volatile uint32_t *)(NVIC_ICPR_ADDRESS + FCSMU_NVIC_OFFSET) = FCSMU_NVIC_MASK;
  *(volatile uint32_t *)(NVIC_ISER_ADDRESS + ERM_NVIC_OFFSET) = ERM_NVIC_MASK;
  *(volatile uint32_t *)(NVIC_ISER_ADDRESS + FCSMU_NVIC_OFFSET) = FCSMU_NVIC_MASK;

  /* If the space can trigger single bit error, run flash test, otherwise flash injection failed. */

  if (u32Temp == 0x55AAUL)
    {
      l_eim_test_result = EIM_TEST_OPEN;
      l_eim_timeout = EIM_TEST_TIMEOUT;

      u32Temp = s_flash_driver_func_header->ecc_injection(&cfg, FLASH_API_SIZE_8M);
      if (u32Temp == FLASH_STATUS_SUCCESS)
        {
          eim_value = *(uint32_t *)u32Addr;
          __asm __volatile__("dsb":::"memory");

          while ((l_eim_test_result != EIM_TEST_SUCCESS) &&\
                 (l_eim_timeout != 0))
            {
              l_eim_timeout--;
            }
        }
    }

  s_flash_driver_func_header->ecc_injection_close();
  flash_ecc_clear((uint32_t)FLASH_CLEAR_SBC_MASK);

  /* Enable FMC buffer */

  if (eim_ch == 78U)
    {
      *(volatile uint32_t *)FC7300_FAPC0 = 0x3;
    }
  else if (eim_ch == 79U)
    {
      *(volatile uint32_t *)FC7300_FAPC1 = 0x3;
    }
  else if (eim_ch == 80U)
    {
      *(volatile uint32_t *)FC7300_FAPC2 = 0x3;
    }
  else
    {

    }

  *(volatile uint32_t *)FC7300_FEEC = 0;
}

#if defined(CONFIG_FC7300_EIM_CPU0_LOCKSTEP) || defined(CONFIG_FC7300_EIM_CPU1_LOCKSTEP)

/****************************************************************************
 * Name: fc7300_eim_lockstep
 *
 * Description:
 *   Injects ECC into lockstep.
 *
 * Input Parameters:
 *   core_index   - The CPU index.
 *
 ****************************************************************************/

static void fc7300_eim_lockstep(uint8_t core_index)
{
  l_eim_test_result = EIM_TEST_OPEN;
  l_eim_timeout = EIM_TEST_TIMEOUT;

  fc7300_eim_putreg(&g_eim_config,
                    FC7300_EIM_CPU0_LOCKSTEP_OFFSET + (core_index << 2U),
                    EIM_CPU_LOCKSTEP_LOCKSTEP_MON0_SET);
  __asm __volatile__("dsb":::"memory");

  while ((l_eim_test_result != EIM_TEST_SUCCESS) &&\
         (l_eim_timeout != 0))
    {
      l_eim_timeout--;
    }

  /* Clear ECC injection */

  fc7300_eim_putreg(&g_eim_config,
                    FC7300_EIM_CPU0_LOCKSTEP_OFFSET + (core_index << 2U),
                    EIM_CPU_LOCKSTEP_LOCKSTEP_MON0_CLR);
  __asm __volatile__("dsb":::"memory");

  fc7300_eim_putreg(&g_eim_config,
                    FC7300_EIM_CPU0_LOCKSTEP_OFFSET + (core_index << 2U),
                    0UL);
  __asm __volatile__("dsb":::"memory");
}

#endif /* defined(CONFIG_FC7300_EIM_CPU0_LOCKSTEP) || defined(CONFIG_FC7300_EIM_CPU1_LOCKSTEP) */

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: fc7300_eim_run
 *
 * Description:
 *   Performing injection self-test using the EIM module.
 *
 ****************************************************************************/

void fc7300_eim_run(void)
{
  uint32_t vtor_table_bak;
  uint32_t i;
  int32_t cpu_idx = fc7300_cpu_index();
  volatile uint32_t iser[8];
  #if defined(CONFIG_FC7300_EIM_CPU0DCACHE_DATA) || \
  defined(CONFIG_FC7300_EIM_CPU0DCACHE_TAG) || \
  defined(CONFIG_FC7300_EIM_CPU0ICACHE_DATA) || \
  defined(CONFIG_FC7300_EIM_CPU0ICACHE_TAG) || \
  defined(CONFIG_FC7300_EIM_CPU1DCACHE_DATA) || \
  defined(CONFIG_FC7300_EIM_CPU1DCACHE_TAG) || \
  defined(CONFIG_FC7300_EIM_CPU1ICACHE_DATA) || \
  defined(CONFIG_FC7300_EIM_CPU1ICACHE_TAG)
  mpu_configure_region(IDLE_STACK_MPU_BASEADDR,
                       CONFIG_IDLETHREAD_STACKSIZE,
                       MPU_RASR_TEX_NOR  | /* Normal             */
                                           /* Non-Cacheable      */
                                           /* Non-Bufferable     */
                                           /* Non-Shareable      */
                       MPU_RASR_AP_RWRW);  /* P:RW   U:RW
                                            * Instruction access */

  mpu_configure_region(((uintptr_t)(IDLE_STACK_MPU_BASEADDR + (uintptr_t)CONFIG_IDLETHREAD_STACKSIZE)),
                       CONFIG_IDLETHREAD_STACKSIZE,
                       MPU_RASR_TEX_NOR  | /* Normal             */
                                           /* Non-Cacheable      */
                                           /* Non-Bufferable     */
                                           /* Non-Shareable      */
                       MPU_RASR_AP_RWRW);  /* P:RW   U:RW
                                            * Instruction access */
  mpu_control(true, true, true);
  #endif

  for (i = 0; i < 8; ++i)
    {
      iser[i] = *(volatile uint32_t *)(NVIC_ISER_ADDRESS + i * 4);
      *(volatile uint32_t *)(NVIC_ICER_ADDRESS + i * 4) = 0xFFFFFFFFUL;
    }

  /* Take over the interrupt vector table and enable interrupts */

  vtor_table_bak = *(volatile uint32_t *)VTOR_ADDRESS;
  *(volatile uint32_t *)VTOR_ADDRESS = (uint32_t)fc7300_eim_vtor_table;
  *(volatile uint32_t *)(NVIC_ISER_ADDRESS + ERM_NVIC_OFFSET) = ERM_NVIC_MASK;
  *(volatile uint32_t *)(NVIC_ISER_ADDRESS + FCSMU_NVIC_OFFSET) = FCSMU_NVIC_MASK;
  *(volatile uint32_t *)SCM_INT_ROUTER_FCSMU = (0x1UL << (24 + cpu_idx));
  *(volatile uint32_t *)SCM_INT_ROUTER_ERM = (0x1UL << (24 + cpu_idx));
  __asm__ __volatile__("cpsie  i");

  for (i = 0U; i < EIM_CHANNEL_MAX; i++)
    {
      if (g_eim_config.eim_channel[i].type == EIM_TYPE_INVALID)
        {
          /* Do nothing. */
        }
      else if (g_eim_config.eim_channel[i].type == EIM_TYPE_CORE0_MEMORY &&
               fc7300_cpu_index() == 0U)
        {
          fc7300_eim_memory(i);
        }
      else if (g_eim_config.eim_channel[i].type == EIM_TYPE_CORE0_ICACHE &&
               fc7300_cpu_index() == 0U)
        {
          fc7300_eim_icache(i);
        }
      else if (g_eim_config.eim_channel[i].type == EIM_TYPE_CORE0_DCACHE &&
               fc7300_cpu_index() == 0U)
        {
          fc7300_eim_dcache(i);
        }
      else if (g_eim_config.eim_channel[i].type == EIM_TYPE_ROM)
        {
          fc7300_eim_rom(i);
        }
      else if (g_eim_config.eim_channel[i].type == EIM_TYPE_FLASH)
        {
          fc7300_eim_flash(i);
        }
      else if (g_eim_config.eim_channel[i].type == EIM_TYPE_CORE1_MEMORY &&
               fc7300_cpu_index() == 1U)
        {
          fc7300_eim_memory(i);
        }
      else if (g_eim_config.eim_channel[i].type == EIM_TYPE_CORE1_ICACHE &&
               fc7300_cpu_index() == 1U)
        {
          fc7300_eim_icache(i);
        }
      else if (g_eim_config.eim_channel[i].type == EIM_TYPE_CORE1_DCACHE &&
               fc7300_cpu_index() == 1U)
        {
          fc7300_eim_dcache(i);
        }
      else if (g_eim_config.eim_channel[i].type == EIM_TYPE_MPU)
        {
          fc7300_eim_mpu(i);
        }
      else if (g_eim_config.eim_channel[i].type == EIM_TYPE_MAM)
        {
          fc7300_eim_mam(i);
        }
      else if (g_eim_config.eim_channel[i].type == EIM_TYPE_FMC)
        {
          fc7300_eim_fmc();
        }
      else
        {
          /* Do nothing. */
        }
    }

#if defined(CONFIG_FC7300_EIM_CPU0_LOCKSTEP)
  fc7300_eim_lockstep(0U);
#endif /* defined(CONFIG_FC7300_EIM_CPU0_LOCKSTEP) */

#if defined(CONFIG_FC7300_EIM_CPU1_LOCKSTEP)
  fc7300_eim_lockstep(1U);
#endif /* defined(CONFIG_FC7300_EIM_CPU1_LOCKSTEP) */

#if defined(CONFIG_FC7300_EIM_CMU0)
  *(volatile uint32_t *)(NVIC_ISER_ADDRESS + CMU0_NVIC_OFFSET) = CMU0_NVIC_MASK;
  fc7300_cmu_injection(0U);
  *(volatile uint32_t *)(NVIC_ICER_ADDRESS + CMU0_NVIC_OFFSET) = CMU0_NVIC_MASK;
#endif /* defined(CONFIG_FC7300_EIM_CMU0) */

#if defined(CONFIG_FC7300_EIM_CMU1)
  *(volatile uint32_t *)(NVIC_ISER_ADDRESS + CMU1_NVIC_OFFSET) = CMU1_NVIC_MASK;
  fc7300_cmu_injection(1U);
  *(volatile uint32_t *)(NVIC_ICER_ADDRESS + CMU1_NVIC_OFFSET) = CMU1_NVIC_MASK;
#endif /* defined(CONFIG_FC7300_EIM_CMU1) */

#if defined(CONFIG_FC7300_EIM_CMU2)
  *(volatile uint32_t *)(NVIC_ISER_ADDRESS + CMU2_NVIC_OFFSET) = CMU2_NVIC_MASK;
  fc7300_cmu_injection(2U);
  *(volatile uint32_t *)(NVIC_ICER_ADDRESS + CMU2_NVIC_OFFSET) = CMU2_NVIC_MASK;
#endif /* defined(CONFIG_FC7300_EIM_CMU2) */

#if defined(CONFIG_FC7300_EIM_CMU3)
  *(volatile uint32_t *)(NVIC_ISER_ADDRESS + CMU3_NVIC_OFFSET) = CMU3_NVIC_MASK;
  fc7300_cmu_injection(3U);
  *(volatile uint32_t *)(NVIC_ICER_ADDRESS + CMU3_NVIC_OFFSET) = CMU3_NVIC_MASK;
#endif /* defined(CONFIG_FC7300_EIM_CMU3) */

#if defined(CONFIG_FC7300_EIM_CMU4)
  *(volatile uint32_t *)(NVIC_ISER_ADDRESS + CMU4_NVIC_OFFSET) = CMU4_NVIC_MASK;
  fc7300_cmu_injection(4U);
  *(volatile uint32_t *)(NVIC_ICER_ADDRESS + CMU4_NVIC_OFFSET) = CMU4_NVIC_MASK;
#endif /* defined(CONFIG_FC7300_EIM_CMU4) */

  /* Restore the interrupt vector table and disable interrupts */

  __asm__ __volatile__("cpsid  i");
  *(volatile uint32_t *)SCM_INT_ROUTER_FCSMU = (0x7UL << 24);
  *(volatile uint32_t *)SCM_INT_ROUTER_ERM = (0x7UL << 24);
  *(volatile uint32_t *)VTOR_ADDRESS = vtor_table_bak;
  *(volatile uint32_t *)(NVIC_ICER_ADDRESS + ERM_NVIC_OFFSET) = ERM_NVIC_MASK;
  *(volatile uint32_t *)(NVIC_ICER_ADDRESS + FCSMU_NVIC_OFFSET) = FCSMU_NVIC_MASK;
  for (i = 0; i < 8; ++i)
    {
      *(volatile uint32_t *)(NVIC_ISER_ADDRESS + i * 4) = iser[i];
    }
#if defined(CONFIG_FC7300_EIM_CPU0DCACHE_DATA) || \
defined(CONFIG_FC7300_EIM_CPU0DCACHE_TAG) || \
defined(CONFIG_FC7300_EIM_CPU0ICACHE_DATA) || \
defined(CONFIG_FC7300_EIM_CPU0ICACHE_TAG) || \
defined(CONFIG_FC7300_EIM_CPU1DCACHE_DATA) || \
defined(CONFIG_FC7300_EIM_CPU1DCACHE_TAG) || \
defined(CONFIG_FC7300_EIM_CPU1ICACHE_DATA) || \
defined(CONFIG_FC7300_EIM_CPU1ICACHE_TAG)
  mpu_freeregion(0);
  mpu_freeregion(1);
  mpu_control(false, false, false);
#endif

  /* Clear SCM Matrix */

  *(volatile uint32_t *)SCM_MATRIX_STATUS0_ADDR = 0xFFFFFFFFUL;
  *(volatile uint32_t *)SCM_MATRIX_STATUS1_ADDR = 0xFFFFFFFFUL;
  *(volatile uint32_t *)SCM_MATRIX_STATUS2_ADDR = 0xFFFFFFFFUL;
  *(volatile uint32_t *)SCM_MATRIX_STATUS3_ADDR = 0xFFFFFFFFUL;
  *(volatile uint32_t *)SCM_MATRIX_STATUS4_ADDR = 0xFFFFFFFFUL;
  *(volatile uint32_t *)SCM_MATRIX_STATUS5_ADDR = 0xFFFFFFFFUL;
}

/****************************************************************************
 * Name: fc7300_memmanage_process
 *
 * Description:
 *   memmanage fault handling c function.
 *
 ****************************************************************************/

void fc7300_memmanage_process(Exception_Type *pFrame)
{
  *(volatile uint32_t *)SCB_CFSR_ADDRESS |= (NVIC_CFAULTS_DACCVIOL | NVIC_CFAULTS_MMARVALID);
  pFrame->pc += 2;
}

/****************************************************************************
 * Name: fc7300_busfault_process
 *
 * Description:
 *   bus fault handling c function.
 *
 ****************************************************************************/

void fc7300_busfault_process(Exception_Type *pFrame)
{
  *(volatile uint32_t *)SCB_CFSR_ADDRESS |= (NVIC_CFAULTS_BFARVALID | NVIC_CFAULTS_PRECISERR);
  pFrame->pc += 2;
}

