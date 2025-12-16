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

#ifndef __VENDOR_FLAGCHIP_CHIPS_FC7300_FC7300_CLKCONFIG_H
#define __VENDOR_FLAGCHIP_CHIPS_FC7300_FC7300_CLKCONFIG_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>

#include <stdint.h>
#include <stdbool.h>
#include <arch/board/board.h>

#if defined(CONFIG_ARCH_CHIP_FC7300F8M)
#  define FTU_INSTANCE_COUNT             12
#elif defined(CONFIG_ARCH_CHIP_FC7300F4M)
#  define FTU_INSTANCE_COUNT             10
#endif

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Nomial frequencies of internal clocks */

/*   PCBA external oscillator value, these macros are user defined.
 *   FOSC/SOSC clock need to be manually configured according to
 *   PCBA XTAL value in SOSC/FOSC configuration.
 *   Current default value is the XTAL value on the EVB board.
 */

#define SOSC_CLOCK_DFT                     32000U
#define FOSC_CLOCK_DFT                     BOARD_XTAL_FREQUENCY
#define SIRC_CLOCK                         12000000U
#define SIRC32K_CLOCK                      32000U
#define FIRC_CLOCK                         96000000U
#define SCG_CLKSRC_STABILIZATION_TIMEOUT   2000U
#define CLOCK_OFF_STABILIZATION_TIMEOUT    2000U
#define CLOCK_DIV_STABILIZATION_TIMEOUT    100000U
#define PLL_CLK_MAX                        300000000U
#define PLL_CLK_MIN                        90000000U
#define SYS_CORE_CLK_MAX                   300000000U
#define SYS_BUS_CLK_MAX                    150000000U
#define SYS_SLOW_CLK_MAX                   75000000U
#define FOSC_STABILIZATION_TIMEOUT         4800000U
#define FIRC_STABILIZATION_TIMEOUT         1500U
#define SIRC_STABILIZATION_TIMEOUT         3000U
#define SOSC_STABILIZATION_TIMEOUT         150000000U
#define PLL_STABILIZATION_TIMEOUT          641000U

/* The FC7300 AON CLK source frequency */

#define SIRCDIV_128K_FREQUENCY             128000U
#define SIRC32K_FREQUENCY                  32000U
#define SIRCDIV_32K_FREQUENCY              32000U
#define SIRC32_1K_FREQUENCY                1000U

/****************************************************************************
 * Public Types
 ****************************************************************************/

extern const unsigned int g_peripheral_clocks_num_0;

/* Clock Configuration ******************************************************/

enum scg_clksrctype_e
{
  /* Main clocks */

  SCG_CORE_CLK                       = 0U,       /* Core clock */
  SCG_BUS_CLK                        = 1U,       /* Bus clock                  */
  SCG_SLOW_CLK                       = 2U,       /* Slow clock                 */

  /* Other internal clocks used by peripherals. */

  SCG_FOSC_CLK                       = 3U,       /* FOSC clock                 */
  SCG_FIRC_CLK                       = 4U,       /* FIRC clock                 */
  SCG_PLL0_CLK                       = 5U,       /* PLL0 clock                 */
  SCG_PLL1_CLK                       = 6U,       /* PLL1 clock                 */
  SCG_SIRC_CLK                       = 7U,       /* SIRC clock                 */
  SCG_SIRC32K_CLK                    = 8U,       /* SIRC32K clock              */
  SCG_SOSC_CLK                       = 9U,       /* SOSC clock                 */
  SCG_SCG_CLKOUT_CLK                 = 10U,      /* SCG CLK_OUT clock          */
  SCG_SIRCDIVH_CLK                   = 11U,      /* SIRCDIVH functional clock  */
  SCG_SIRCDIVM_CLK                   = 12U,      /* SIRCDIVM functional clock  */
  SCG_SIRCDIVL_CLK                   = 13U,      /* SIRCDIVL functional clock  */
  SCG_FIRCDIVH_CLK                   = 14U,      /* FIRCDIVH functional clock  */
  SCG_FIRCDIVM_CLK                   = 15U,      /* FIRCDIVM functional clock  */
  SCG_FIRCDIVL_CLK                   = 16U,      /* FIRCDIVL functional clock  */
  SCG_FOSCDIVH_CLK                   = 17U,      /* FOSCDIVH functional clock  */
  SCG_FOSCDIVM_CLK                   = 18U,      /* FOSCDIVM functional clock  */
  SCG_FOSCDIVL_CLK                   = 19U,      /* FOSCDIVL functional clock  */
  SCG_PLL0DIVH_CLK                   = 20U,      /* PLL0DIVH functional clock  */
  SCG_PLL0DIVM_CLK                   = 21U,      /* PLL0DIVM functional clock  */
  SCG_PLL0DIVL_CLK                   = 22U,      /* PLL0DIVL functional clock  */
  SCG_PLL1DIVH_CLK                   = 23U,      /* PLL1DIVH functional clock  */
  SCG_PLL1DIVM_CLK                   = 24U,      /* PLL1DIVM functional clock  */
  SCG_PLL1DIVL_CLK                   = 25U,      /* PLL1DIVL functional clock  */
  SCG_END_OF_CLOCKS                  = 26U       /* End of SCG clocks          */
};

/* PLL clock type */

enum scg_pllclktype_e
{
  SCG_PLL0 = 0U,                                 /* PLL0 clock */
  SCG_PLL1 = 1U                                  /* PLL1 clock */
};

/* indicate the clock status for each clock source in clock tree list. */

enum scg_statustype_e
{
  SCG_STATUS_SUCCESS        = 0U,   /* function called success */
  SCG_STATUS_SEQUENCE_ERROR = 1U,   /* function called report sequence error */
  SCG_STATUS_TIMEOUT        = 2U,   /* function called report timeout error */
  SCG_STATUS_IRC_ERROR      = 3U,   /* function called report internal clock error */
  SCG_STATUS_PARAM_ERROR    = 4U    /* function called report internal clock error */
};

/* indicate the CRC check result */

enum scg_crccheckrestype_e
{
  SCG_CRC_CHECK_SUCCESS = 0U,        /* CRC check success */
  SCG_CRC_CHECK_FAILED  = 1U         /* CRC check failed */
};

/* SCG xxxIRC TRIMSRC type. */

enum scg_irctrimsrctype_e
{
  SCG_IRC_TRIMSRC_RESERVE0   = 0U,               /* Clock Trim source reserve0 */
  SCG_IRC_TRIMSRC_RESERVE1   = 1U,               /* Clock Trim source reserve1 */
  SCG_IRC_TRIMSRC_FOSC       = 2U,               /* Clock Trim source FOSC */
  SCG_IRC_TRIMSRC_SOSC       = 3U,               /* Clock Trim source SOSC */
};

/* SCG_CCR [SCS]  to Select system clock source */

enum scg_clocksrctype_e
{
  SCG_CLOCK_SRC_FOSC     = 1U,       /* System OSC. */
  SCG_CLOCK_SRC_FIRC     = 3U,       /* Fast IRC.   */
  SCG_CLOCK_SRC_PLL0     = 6U,       /* System PLL. */
  SCG_CLOCK_SRC_NONE     = 255U      /* MAX value.  */
};

/* SCG_CCR [DIVCORE]/ SCG_CCR [DIVBUS]/ SCG_CCR [DIVSLOW] */

enum scg_clockdivtype_e
{
  SCG_CLOCK_DIV_BY1   = 0U,     /* Divided by 1.    */
  SCG_CLOCK_DIV_BY2   = 1U,     /* Divided by 2.    */
  SCG_CLOCK_DIV_BY3   = 2U,     /* Divided by 3.    */
  SCG_CLOCK_DIV_BY4   = 3U      /* Divided by 4.    */
};

/* SCG_ CLKOUTCFG [CLKOUTSEL]  to Select system clock source. */

enum scg_clockoutsrctype_e
{
  SCG_CLOCKOUT_SRC_OFF      = 0U,   /* SCG OFF.     */
  SCG_CLOCKOUT_SRC_FOSC     = 1U,   /* Fast OSC.    */
  SCG_CLOCKOUT_SRC_SIRC     = 2U,   /* Slow IRC.    */
  SCG_CLOCKOUT_SRC_FIRC     = 3U,   /* Fast IRC.    */
  SCG_CLOCKOUT_SRC_SOSC     = 4U,   /* Slow OSC.    */
  SCG_CLOCKOUT_SRC_PLL1     = 5U,   /* System PLL1. */
  SCG_CLOCKOUT_SRC_PLL0     = 6U,   /* System PLL0. */
  SCG_CLOCKOUT_SRC_SIRC32K  = 7U,   /* SIRC32K_CLK. */
};

/* NVM clock source enumeration */

enum scg_nvmclksrctype_e
{
  SCG_NVMCLK_SRC_SIRC = 30u,   /* NVM source choose SIRC.   */
  SCG_NVMCLK_SRC_FIRC = 31u    /* NVM source choose FIRC.   */
};

/* CMU4 clock source enumeration */

enum scg_cmu4clksrctype_e
{
  SCG_CMU4CLK_SRC_SIRC = 28u,   /* NVM source choose SIRC.   */
  SCG_CMU4CLK_SRC_FOSC = 29u    /* NVM source choose FORC.   */
};

/* [DIVL]/[DIVM]/[DIVH] bit field definition
 * for SCG_SIRC/ SCG_FIRC/ SCG_FOSC/ SCG_PLL0 registers
 */

enum scg_asyncclockdivtype_e
{
  SCG_ASYNCCLOCK_DISABLE  = 0U,         /* Clock output is disabled.  */
  SCG_ASYNCCLOCKDIV_BY1    = 1U,        /* Divided by 1.              */
  SCG_ASYNCCLOCKDIV_BY2    = 2U,        /* Divided by 2.              */
  SCG_ASYNCCLOCKDIV_BY4    = 3U,        /* Divided by 4.              */
  SCG_ASYNCCLOCKDIV_BY8    = 4U,        /* Divided by 8.              */
  SCG_ASYNCCLOCKDIV_BY16   = 5U,        /* Divided by 16.             */
  SCG_ASYNCCLOCKDIV_BY32   = 6U,        /* Divided by 32.             */
  SCG_ASYNCCLOCKDIV_BY64   = 7U         /* Divided by 64.             */
};

/* SCG_ PLLCFG[PREDIV]  for PLL clock calculation.
 * The pre-div value range is 0 ~ 31.
 */

typedef uint8_t scg_pllpredivtype;

/* SCG_ PLLCFG[PSTDIV]  for PLL clock calculation */

enum scg_pllpstdivtype_e
{
  SCG_PLLPSTDIV_BY2 = 1U,
  SCG_PLLPSTDIV_BY4 = 2U,
  SCG_PLLPSTDIV_BY8 = 3U
};

/* SCG_ PLLCFG[SOURCE] to Select PLL clock sourec */

enum scg_pllsourcetype_e
{
  SCG_PLLSOURCE_FOSC = 0U,
  SCG_PLLSOURCE_FIRC = 1U,
};

/* SCG_ PLLCFG [MULT]  for PLL clock calculation.
 * The mult value range is 96 ~ 512.
 */

typedef uint16_t scg_pllmultiplytype_e;

struct scg_sirc_config_s
{
  bool lock;                              /* SCG_SIRCCSR[LK] bit, Write to set the register can be written or not  */
  bool cm;                                /* SCG_SIRCCSR[CM] bit, SIRC Clock Monitor Enable */
  bool tren;                              /* SCG_SIRCCSR[TREN] bit, IRC software trim enable (auto trim) */
  bool lpen;                              /* SCG_SIRCCSR[LPEN] bit, Clock Stop Enable */
  bool sten;                              /* SCG_SIRCCSR[STEN] bit, Clock Standby Enable */
  enum scg_asyncclockdivtype_e divh;      /* SCG_SIRCDIV[DIVH] bit field definition. */
  enum scg_asyncclockdivtype_e divm;      /* SCG_SIRCDIV[DIVM] bit field definition. */
  enum scg_asyncclockdivtype_e divl;      /* SCG_SIRCDIV[DIVL] bit field definition. */
  uint8_t trimsrc;                        /* SCG_SIRCTCFG[TRIMSRC] IRC clock auto trim reference clock source select. */
};

/* SCG FIRC clock configuration */

struct scg_firc_config_s
{
  bool enable;                            /* SCG_FIRCCSR[EN] bit, FIRC clock enable */
  bool lock;                              /* SCG_FIRCCSR[VLD] bit, Read only, Clock is enabled and output clock is valid */
  bool cm;                                /* SCG_FIRCCSR[CM] bit, FIRC Clock Monitor Enable */
  bool tren;                              /* SCG_FIRCCSR[TREN] bit, IRC software trim enable (auto trim) */
  bool sten;                              /* SCG_FIRCCSR[STEN] bit, Clock Standby Enable */
  enum scg_asyncclockdivtype_e divh;      /* SCG_FIRCDIV[DIVH] bit field definition. */
  enum scg_asyncclockdivtype_e divm;      /* SCG_FIRCDIV[DIVM] bit field definition. */
  enum scg_asyncclockdivtype_e divl;      /* SCG_FIRCDIV[DIVL] bit field definition. */
  uint8_t trimsrc;                        /* SCG_FIRCTCFG[TRIMSRC] IRC clock auto trim reference clock source select. */
};

/* SCG FOSC Clock Configuration */

struct scg_fosc_config_s
{
  bool lock;                              /* SCG_FOSCCSR[lk] bit, write to set the register can be written or not */
  bool cm;                                /* SCG_FOSCCSR[cm] bit, clock monitor is enable */
  bool cmre;                              /* SCG_FOSCCSR[cmre] bit, clock monitor reset enable 0 generates interrupt, 1 generates rese */
  bool sten;                              /* SCG_FOSCCSR[sten] bit, clock stop in stop modes */
  bool enable;                            /* SCG_FOSCCSR[en] bit, fosc clock enable */
  bool bypass;                            /* SCG_FOSCCSR[bypass] bit, configures fosc for bypassing the internal oscillator. */
  enum scg_asyncclockdivtype_e divh;      /* SCG_FOSCDIV[divh] bit field definition. */
  enum scg_asyncclockdivtype_e divm;      /* SCG_FOSCDIV[divm] bit field definition. */
  enum scg_asyncclockdivtype_e divl;      /* SCG_FOSCDIV[divl] bit field definition. */
  uint32_t xtalfreq;                      /* Static parameter for fosc clock record. */
};

/* SCG ClockControl Configuration */

struct scg_clockctrl_config_s
{
  bool sysclkmonitor;                     /* SCG_CCR[sysclk_cme], system clock monitor bit. */
  enum scg_clocksrctype_e src;            /* SCG_CCR[scs], system clock source. */
  enum scg_clockdivtype_e divslow;        /* SCG_CCR[divslow], slow clock divide ratio. */
  enum scg_clockdivtype_e divbus;         /* SCG_CCR[divbus], bus clock divide ratio. */
  enum scg_clockdivtype_e divcore;        /* SCG_CCR[divcore], core clock divide ratio. */
  enum scg_clockoutsrctype_e clkoursrc;
};

/* PLL definition, include register SCG_PLLCSR/ SCG_PLLDIV/SCG_PLLCFG. */

struct scg_pll_config_s
{
  bool enable;                            /* SCG_PLLCSR[en] bit, pll0 clock enable */
  bool lock;                              /* SCG_PLLCSR[lk] bit, write to set the register can be written or not. */
  bool cm;                                /* SCG_PLLCSR[cm] bit,clock monitor enable. */
  bool cmre;                              /* SCG_PLLCSR[cmre] bit, 1:clock monitor reset, 0: interrupt */
  bool sten;                              /* SCG_PLLCSR[sten] bit, clock standby enable */
  enum scg_asyncclockdivtype_e divh;      /* SCG_PLLDIV[divh] bit field definition. */
  enum scg_asyncclockdivtype_e divm;      /* SCG_PLLDIV[divm] bit field definition. */
  enum scg_asyncclockdivtype_e divl;      /* SCG_PLLDIV[divl] bit field definition. */
  uint16_t prediv;                        /* SCG_PLLCFG[prediv] bit field definition. */
  enum scg_pllpstdivtype_e pstdiv;        /* SCG_PLLCFG[pstdiv] bit field definition. */
  uint16_t mult;                          /* SCG_PLLCFG[mult] bit field definition. */
  enum scg_pllsourcetype_e src;           /* SCG_PLLCFG[source] bit field definition. */
};

/* SOSC definition, include register SCG_SOSCCFG. */

struct scg_sosc_config_s
{
  bool enable;                            /* SCG_SOSCCSR[en] bit, sosc clock enable */
  bool lock;                              /* SCG_SOSCCSR[lk] bit, write to set the register can be written or not. */
  bool bypass;                            /* SCG_SOSCCSR[bypass] bit, configures sosc for bypassing the internal oscillator. */
  bool cm;                                /* SCG_SOSCCSR[cm] bit,clock monitor enable. */
  bool cmre;                              /* SCG_SOSCCSR[cmre] bit, 1:clock monitor reset, 0: interrupt */
};

/* Overall SCG Configuration */

struct scg_config_s
{
  struct scg_sirc_config_s sirc;             /* Slow internal reference clock configuration */
  struct scg_firc_config_s firc;             /* Fast internal reference clock configuration */
  struct scg_fosc_config_s fosc;             /* Fast oscillator configuration */
  struct scg_pll_config_s  pll0;             /* PLL0 configuration */
  struct scg_pll_config_s  pll1;             /* PLL1 configuration */
  struct scg_sosc_config_s sosc;             /* Slow oscillator configuration */
  struct scg_clockctrl_config_s clockctrl;   /* SCG clockout configuration */
};

/* SIM trace clock configuration */

enum clock_trace_src_e
{
  CLOCK_TRACE_SRC_CORE_CLK = 0x0       /* Core clock */
};

/* PCC clock configuration */

struct pcc_config_s
{
  const struct peripheral_clock_config_s *pclks; /* The peripheral clock configuration array */
};

/* Overall clock configuration */

struct clock_configuration_s
{
  struct scg_config_s scg;                   /* SCG Clock configuration */
  struct pcc_config_s pcc;                   /* PCC Clock configuration */
};

/****************************************************************************
 * Inline Functions
 ****************************************************************************/

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
 * Name: fc7300_clockconfig
 *
 * Description:
 *   Called to initialize the fc7300.  This does whatever setup is needed
 *   to put the MCU in a usable state.  This includes the initialization of
 *   clocking using the settings in board.h.  This function also performs
 *   other low-level chip as necessary.
 *
 * Input Parameters:
 *   clkcfg - Describes the new clock configuration
 *
 * Returned Value:
 *   Zero (OK) is returned a success;  A negated errno value is returned on
 *   any failure.
 *
 ****************************************************************************/

int fc7300_clockconfig(const struct clock_configuration_s *clkcfg);

/****************************************************************************
 * Name: fc7300_get_coreclk
 *
 * Description:
 *   Return the current value of the CORE clock frequency.
 *
 * Input Parameters:
 *   None
 *
 * Returned Values:
 *   The current value of the CORE clock frequency.  Zero is returned on any
 *   failure.
 *
 ****************************************************************************/

uint32_t fc7300_get_coreclk(void);

/****************************************************************************
 * Name: fc7300_get_busclk
 *
 * Description:
 *   Return the current value of the BUS clock frequency.
 *
 * Input Parameters:
 *   None
 *
 * Returned Values:
 *   The current value of the BUS clock frequency.  Zero is returned on any
 *   failure.
 *
 ****************************************************************************/

uint32_t fc7300_get_busclk(void);

/****************************************************************************
 * Name: fc7300_get_foscfreq
 *
 * Description:
 *   Gets fosc clock frequency with a specific divider.
 *
 * Input Parameters:
 *   div - The clock source divider.
 *
 * Returned Value:
 *   The frequency of the requested asynchronous clock source.
 *
 ****************************************************************************/

uint32_t fc7300_get_foscfreq(uint8_t div);

/****************************************************************************
 * Name: fc7300_get_sircfreq
 *
 * Description:
 *   Gets sirc clock frequency with a specific divider.
 *
 * Input Parameters:
 *   div - The clock source divider.
 *
 * Returned Value:
 *   The frequency of the requested asynchronous clock source.
 *
 ****************************************************************************/

uint32_t fc7300_get_sircfreq(uint8_t div);

/****************************************************************************
 * Name: fc7300_get_fircfreq
 *
 * Description:
 *   Gets firc clock frequency with a specific divider.
 *
 * Input Parameters:
 *   div - The clock source divider.
 *
 * Returned Value:
 *   The frequency of the requested asynchronous clock source.
 *
 ****************************************************************************/

uint32_t fc7300_get_fircfreq(uint8_t div);

/****************************************************************************
 * Name: fc7300_get_pll0freq
 *
 * Description:
 *   Gets pll0 clock frequency with a specific divider.
 *
 * Input Parameters:
 *   div - The clock source divider.
 *
 * Returned Value:
 *   The frequency of the requested asynchronous clock source.
 *
 ****************************************************************************/

uint32_t fc7300_get_pll0freq(uint8_t div);

/****************************************************************************
 * Name: fc7300_get_pll1freq
 *
 * Description:
 *   Gets pll1 clock frequency with a specific divider.
 *
 * Input Parameters:
 *   div - The clock source divider.
 *
 * Returned Value:
 *   The frequency of the requested PLL1 clock source.
 *
 ****************************************************************************/

uint32_t fc7300_get_pll1freq(uint8_t div);

/****************************************************************************
 * Name: fc7300_get_aonclk
 *
 * Description:
 *   Return the current value of the aon clock frequency.
 *
 * Input Parameters:
 *   None
 *
 * Returned Values:
 *   The current value of the aon clock frequency.  Zero is returned on any
 *   failure.
 *
 ****************************************************************************/

uint32_t fc7300_get_aonclk(void);

#undef EXTERN
#if defined(__cplusplus)
}
#endif

#endif /* __ASSEMBLY__ */
#endif /* __VENDOR_FLAGCHIP_CHIPS_FC7300_FC7300_CLKCONFIG_H */
