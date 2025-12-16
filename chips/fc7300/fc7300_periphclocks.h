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

#ifndef __VENDOR_FLAGCHIP_CHIPS_FC7300_FC7300_PERIPHCLOCKS_H
#define __VENDOR_FLAGCHIP_CHIPS_FC7300_FC7300_PERIPHCLOCKS_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>

#include <stdint.h>
#include <assert.h>
#include <debug.h>

#include <nuttx/arch.h>

#include "arm_internal.h"
#include "fc7300_config.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Peripheral pcc DIV features. */

#define HAS_NO_DIVIDER                 (0)      /* Divider implemented in PCC source */
#define HAS_DIVIDER_H                  (1 << 0) /* DIVH implemented in PCC source */
#define HAS_DIVIDER_M                  (1 << 1) /* DIVM implemented in PCC source */
#define HAS_DIVIDER_L                  (1 << 2) /* DIVL implemented in PCC source */

enum pcc_clksrctype_e
{
  PCC_CLK_DMA0      = 8,
  PCC_CLK_DMAMUX0   = 10,
  PCC_CLK_FCSMU     = 15,
  PCC_CLK_ROMC      = 19,
  PCC_CLK_SEC       = 20,
  PCC_CLK_ERM       = 24,
  PCC_CLK_EIM       = 25,
  PCC_CLK_INTM0     = 26,
  PCC_CLK_ISM       = 27,
  PCC_CLK_FC        = 32,
  PCC_CLK_FCA       = 33,
  PCC_CLK_WDOG0     = 34,
  PCC_CLK_SCG       = 35,
  PCC_CLK_PCC       = 36,
  PCC_CLK_TRGSEL0   = 38,
  PCC_CLK_TRGSEL1   = 39,
  PCC_CLK_TRGSEL2   = 40,
  PCC_CLK_TRGSEL3   = 41,
  PCC_CLK_CRC0      = 42,
  PCC_CLK_CORDIC    = 43,
  PCC_CLK_TSTMP0    = 44,
  PCC_CLK_TSTMP1    = 45,
  PCC_CLK_FCPIT0    = 46,
  PCC_CLK_AONTIMER  = 47,
  PCC_CLK_RTC       = 48,
  PCC_CLK_CMU0      = 49,
  PCC_CLK_CMU1      = 50,
  PCC_CLK_CMU2      = 51,
  PCC_CLK_CMU3      = 52,
  PCC_CLK_CMU4      = 53,
  PCC_CLK_PTIMER0   = 55,
  PCC_CLK_PTIMER1   = 56,
  PCC_CLK_ADC0      = 59,
  PCC_CLK_ADC1      = 60,
  PCC_CLK_WKU       = 63,
  PCC_CLK_CMP0      = 64,
  PCC_CLK_CMP1      = 65,
#if defined(CONFIG_ARCH_CHIP_FC7300F8M) || defined(CONFIG_ARCH_CHIP_FC7300F4M)
  PCC_CLK_CMP2      = 66,
#endif
  PCC_CLK_TMU       = 67,
  PCC_CLK_PMC       = 68,
  PCC_CLK_SMC       = 69,
  PCC_CLK_RGM       = 70,
  PCC_CLK_PORTA     = 71,
  PCC_CLK_PORTB     = 72,
  PCC_CLK_PORTC     = 73,
  PCC_CLK_PORTD     = 74,
  PCC_CLK_PORTE     = 75,
  PCC_CLK_PORTF     = 76,
  PCC_CLK_PORTG     = 77,
  PCC_CLK_PORTH     = 78,
  PCC_CLK_PORTI     = 79,
  PCC_CLK_SENT0     = 84,
  PCC_CLK_MB        = 88,
  PCC_CLK_FTU0      = 92,
  PCC_CLK_FTU1      = 93,
  PCC_CLK_FTU2      = 94,
  PCC_CLK_FTU3      = 95,
  PCC_CLK_FTU4      = 96,
  PCC_CLK_FTU5      = 97,
  PCC_CLK_FCSPI0    = 98,
  PCC_CLK_FCSPI1    = 99,
  PCC_CLK_FCSPI2    = 100,
  PCC_CLK_FCSPI3    = 101,
  PCC_CLK_FCIIC0    = 102,
  PCC_CLK_FCUART0   = 104,
  PCC_CLK_FCUART1   = 105,
  PCC_CLK_FCUART2   = 106,
  PCC_CLK_FCUART3   = 107,
  PCC_CLK_FCUART4   = 108,
  PCC_CLK_FCUART5   = 109,
  PCC_CLK_FCUART6   = 110,
  PCC_CLK_FCUART7   = 111,
  PCC_CLK_LU0       = 112,
  PCC_CLK_LU1       = 113,
  PCC_CLK_SCM       = 114,
#if defined(CONFIG_ARCH_CHIP_FC7300F8M) || defined(CONFIG_ARCH_CHIP_FC7300F4M)
  PCC_CLK_ENET      = 116,
#endif
  PCC_CLK_FREQM     = 120,
  PCC_CLK_CSC0      = 121,
#if defined(CONFIG_ARCH_CHIP_FC7300F8M) || defined(CONFIG_ARCH_CHIP_FC7300F4M)
  PCC_CLK_CSC1      = 122,
#endif
#if defined(CONFIG_ARCH_CHIP_FC7300F8M)
  PCC_CLK_CSC2      = 123,
#endif
  PCC_CLK_STCU      = 127,
  PCC_CLK_FLEXCAN0  = 128,
  PCC_CLK_FLEXCAN1  = 132,
  PCC_CLK_FLEXCAN2  = 136,
  PCC_CLK_FLEXCAN3  = 140,
  PCC_CLK_FLEXCAN4  = 144,
#if defined(CONFIG_ARCH_CHIP_FC7300F8M) || defined(CONFIG_ARCH_CHIP_FC7300F4M)
  PCC_CLK_DMA1      = 168,
  PCC_CLK_DMAMUX1   = 170,
#endif
  PCC_CLK_MAM0      = 173,
  PCC_CLK_MAM1      = 174,
  PCC_CLK_MAM2      = 175,
#if defined(CONFIG_ARCH_CHIP_FC7300F8M) || defined(CONFIG_ARCH_CHIP_FC7300F4M)
  PCC_CLK_INTM1     = 181,
#endif
#if defined(CONFIG_ARCH_CHIP_FC7300F8M)
  PCC_CLK_INTM2     = 182,
#endif
  PCC_CLK_PTIMER2   = 194,
  PCC_CLK_PTIMER3   = 195,
  PCC_CLK_ADC2      = 198,
#if defined(CONFIG_ARCH_CHIP_FC7300F8M) || defined(CONFIG_ARCH_CHIP_FC7300F4M)
  PCC_CLK_ADC3      = 199,
  PCC_CLK_SDDF      = 203,
  PCC_CLK_CRC1      = 210,
#endif
  PCC_CLK_WDOG1     = 211,
  PCC_CLK_WDOG2     = 212,
  PCC_CLK_FCPIT1    = 215,
#if defined(CONFIG_ARCH_CHIP_FC7300F8M) || defined(CONFIG_ARCH_CHIP_FC7300F4M)
  PCC_CLK_TSTMP2    = 216,
#endif
#if defined(CONFIG_ARCH_CHIP_FC7300F8M)
  PCC_CLK_TSTMP3    = 217,
  PCC_CLK_OSPI      = 221,
  PCC_CLK_FCUART16  = 230,
  PCC_CLK_FCUART17  = 231,
  PCC_CLK_FCSPI6    = 232,
  PCC_CLK_FCSPI7    = 233,
#endif
#if defined(CONFIG_ARCH_CHIP_FC7300F8M) || defined(CONFIG_ARCH_CHIP_FC7300F4M)
  PCC_CLK_SENT1     = 244,
#endif
  PCC_CLK_FTU6      = 255,
  PCC_CLK_FTU7      = 256,
#if defined(CONFIG_ARCH_CHIP_FC7300F8M) || defined(CONFIG_ARCH_CHIP_FC7300F4M)
  PCC_CLK_FTU8      = 257,
  PCC_CLK_FTU9      = 258,
#endif
#if defined(CONFIG_ARCH_CHIP_FC7300F8M)
  PCC_CLK_FTU10     = 259,
  PCC_CLK_FTU11     = 260,
#endif
  PCC_CLK_FCIIC1    = 263,
#if defined(CONFIG_ARCH_CHIP_FC7300F8M) || defined(CONFIG_ARCH_CHIP_FC7300F4M)
  PCC_CLK_FCUART8   = 264,
  PCC_CLK_FCUART9   = 265,
#endif
#if defined(CONFIG_ARCH_CHIP_FC7300F8M)
  PCC_CLK_FCUART10  = 266,
  PCC_CLK_FCUART11  = 267,
  PCC_CLK_FCUART12  = 268,
  PCC_CLK_FCUART13  = 269,
  PCC_CLK_FCUART14  = 270,
  PCC_CLK_FCUART15  = 271,
#endif
  PCC_CLK_FCSPI4    = 272,
  PCC_CLK_FCSPI5    = 273,
  PCC_CLK_MSC0      = 276,
#if defined(CONFIG_ARCH_CHIP_FC7300F8M) || defined(CONFIG_ARCH_CHIP_FC7300F4M)
  PCC_CLK_MSC1      = 277,
#endif
#if defined(CONFIG_ARCH_CHIP_FC7300F8M) || defined(CONFIG_ARCH_CHIP_FC7300F4M)
  PCC_CLK_FLEXCAN5  = 288,
#endif
#if defined(CONFIG_ARCH_CHIP_FC7300F8M)
  PCC_CLK_FLEXCAN6  = 292,
  PCC_CLK_FLEXCAN7  = 296,
  PCC_CLK_FLEXCAN8  = 300,
  PCC_CLK_FLEXCAN9  = 304
#endif
};

/* pcc_clkgatesrctype_e
 *
 * defined the clock source for function clock,
 * match with PCC_XXX[SEL] bit filed if the peripheral has.
 */

enum pcc_clkgatesrctype_e
{
  PCC_CLKGATE_SRC_OFF        = 0U,
  PCC_CLKGATE_SRC_FOSCDIV    = 1U,
  PCC_CLKGATE_SRC_SIRCDIV    = 2U,
  PCC_CLKGATE_SRC_FIRCDIV    = 3U,
  PCC_CLKGATE_SRC_RESERVE0   = 4U,
  PCC_CLKGATE_SRC_PLL1DIV    = 5U,
  PCC_CLKGATE_SRC_PLL0DIV    = 6U,
  PCC_CLKGATE_SRC_RESERVE1   = 7U,
  PCC_CLKGATE_UNINVOLVED     = 8U
};

/*  pcc_clkdivtype_e
 *
 *  define the clock divider,match with PCC_XXX[DIV] bit filed if
 *  the peripheral has it.
 */

enum pcc_clkdivtype_e
{
  PCC_CLK_DIV_BY1     = 0U,           /* Divide by 1 (no clock divide) */
  PCC_CLK_DIV_BY2     = 1U,           /* Divide by 2 */
  PCC_CLK_DIV_BY3     = 2U,           /* Divide by 3 */
  PCC_CLK_DIV_BY4     = 3U,           /* Divide by 4 */
  PCC_CLK_DIV_BY5     = 4U,           /* Divide by 5 */
  PCC_CLK_DIV_BY6     = 5U,           /* Divide by 6 */
  PCC_CLK_DIV_BY7     = 6U,           /* Divide by 7 */
  PCC_CLK_DIV_BY8     = 7U,           /* Divide by 8 */
  PCC_CLK_UNINVOLVED  = 8U
};

/*  pcc_ctrlownertype_e
 *
 *  Indicate which CPU can control PCC
 */

enum pcc_ctrlownertype_e
{
  PCC_CTRL_BY_ALL = 0U,    /* All CPUs are allowed to write this peripheral */
  PCC_CTRL_BY_CPU0 = 1U,   /* Only CPU0 is allowed to control this peripheral */
  PCC_CTRL_BY_CPU1 = 2U,   /* Only CPU1 is allowed to control this peripheral */
  PCC_CTRL_BY_CPU2 = 3U,   /* Only CPU2 is allowed to control this peripheral */
  PCC_CTRL_BY_NONE = 7U,   /* None CPU is allowed to control this peripheral */
};

/****************************************************************************
 * Public Types
 ****************************************************************************/

typedef uint8_t peripheral_clock_source_t;  /* See CLK_SRC_* definitions */

struct peripheral_clock_config_s
{
  enum pcc_clksrctype_e eclockname;      /* Peripheral clock source. */
  bool bclkgate;                         /* Peripheral clock gate. */
  enum pcc_clkgatesrctype_e eclksrc;     /* Peripheral clock source. */
  enum pcc_clkdivtype_e edivider;        /* Peripheral clock divider value. */
  enum pcc_ctrlownertype_e ectrlowner;   /* Peripheral control by which CPI */
  bool blockctrl;                        /* Peripheral control setting locked or not */
};

struct peripheral_clock_assignments_s
{
  enum pcc_clksrctype_e eclockname;
  uint8_t clock_mux_type;
};

#ifndef __ASSEMBLY__

/****************************************************************************
 * Public Data
 ****************************************************************************/

#undef EXTERN
#if defined(__cplusplus)
#define EXTERN extern "C"
extern "C"
{
#else
#define EXTERN extern
#endif

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

/****************************************************************************
 * Name: fc7300_periphclocks
 *
 * Description:
 *   This function configures peripheral clocks in the PCC block.
 *
 * Input Parameters:
 *   count - Number of peripheral clocks to be configured
 *   pclks - Pointer to an array of peripheral clock configurations
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/

void fc7300_periphclocks(unsigned int count,
                         const struct peripheral_clock_config_s *pclks);

/****************************************************************************
 * Name: fc7300_get_pclkfreq
 *
 * Description:
 *   This function returns the clock frequency of the specified peripheral
 *   functional clock.
 *
 * Input Parameters:
 *   clkname   - Identifies the peripheral clock of interest
 *   frequency - The location where the peripheral clock frequency will be
 *              returned
 *
 * Returned Value:
 *   Zero (OK) is returned on success; a negated errno value is returned on
 *   any failure.  -ENODEV is returned if the clock is not enabled or is not
 *   being clocked.
 *
 ****************************************************************************/

int fc7300_get_pclkfreq(enum pcc_clksrctype_e clockname,
                        uint32_t *frequency);

void fc7300_reset_pcc(enum pcc_clksrctype_e eclockname);

#undef EXTERN
#if defined(__cplusplus)
}
#endif

#endif /* __ASSEMBLY__ */
#endif /* __VENDOR_FLAGCHIP_CHIPS_FC7300_FC7300_PERIPHCLOCKS_H */
