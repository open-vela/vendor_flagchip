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

#include "fc7300_clockconfig.h"
#include "fc7300_start.h"
#include "fc7300f8m-evb.h"
#include <arch/board/board.h>
/****************************************************************************
 * Public Data
 ****************************************************************************/

/* Each FC7300F8M board must provide the following initialized structure.
 * This is needed to establish the initial board clocking.
 */

const struct clock_configuration_s g_initial_clkconfig =
{
  .scg =
  {
    .sirc         =
    {
      .lock       = false,                        /* SCG_SIRCCSR[LK] bit, Write to set the register can be written or not  */
      .cm         = false,                        /* SCG_SIRCCSR[CM] bit, SIRC Clock Monitor Enable */
      .tren       = false,                        /* SCG_SIRCCSR[TREN] bit, IRC software trim enable (auto trim) */
      .lpen       = true,                         /* SCG_SIRCCSR[LPEN] bit, Clock Stop Enable */
      .sten       = true,                         /* SCG_SIRCCSR[STEN] bit, Clock Standby Enable */
      .divh       = SCG_ASYNCCLOCKDIV_BY1,        /* SCG_SIRCDIV[DIVH] bit field definition. */
      .divm       = SCG_ASYNCCLOCKDIV_BY1,        /* SCG_SIRCDIV[DIVM] bit field definition. */
      .divl       = SCG_ASYNCCLOCKDIV_BY2,        /* SCG_SIRCDIV[DIVL] bit field definition. */
      .trimsrc    = 0,                            /* SCG_SIRCTCFG[TRIMSRC] IRC clock auto trim reference clock source select. */
    },
    .firc         =
    {
      .enable     = true,                         /* SCG_FIRCCSR[EN] bit, FIRC clock enable */
      .lock       = false,                        /* SCG_FIRCCSR[VLD] bit, Read only, Clock is enabled and output clock is valid */
      .cm         = false,                        /* SCG_FIRCCSR[CM] bit, FIRC Clock Monitor Enable */
      .tren       = false,                        /* SCG_FIRCCSR[TREN] bit, IRC software trim enable (auto trim) */
      .sten       = false,                        /* SCG_FIRCCSR[STEN] bit, Clock Standby Enable */
      .divh       = SCG_ASYNCCLOCKDIV_BY1,        /* SCG_FIRCDIV[DIVH] bit field definition. */
      .divm       = SCG_ASYNCCLOCKDIV_BY2,        /* SCG_FIRCDIV[DIVM] bit field definition. */
      .divl       = SCG_ASYNCCLOCKDIV_BY4,        /* SCG_FIRCDIV[DIVL] bit field definition. */
      .trimsrc    = 0,
    },
    .fosc         =
    {
      .lock       = false,                        /* SCG_FOSCCSR[LK] bit, Write to set the register can be written or not */
      .cm         = false,                        /* SCG_FOSCCSR[CM] bit, Clock Monitor is enable */
      .cmre       = false,                        /* SCG_FOSCCSR[CMRE] bit, Clock Monitor Reset Enable 0 Generates interrupt, 1 Generates rese */
      .sten       = false,                        /* SCG_FOSCCSR[STEN] bit, Clock Stop in Stop modes */
      .enable     = true,                         /* SCG_FOSCCSR[EN] bit, FOSC clock enable */
      .bypass     = false,                        /* SCG_FOSCCFG[BYPASS] bit, Configures FOSC for bypassing the internal oscillator. */
      .divh       = SCG_ASYNCCLOCKDIV_BY1,        /* SCG_FOSCDIV[DIVH] bit field definition. */
      .divm       = SCG_ASYNCCLOCKDIV_BY1,        /* SCG_FOSCDIV[DIVM] bit field definition. */
      .divl       = SCG_ASYNCCLOCKDIV_BY2,        /* SCG_FOSCDIV[DIVL] bit field definition. */
      .xtalfreq   = FOSC_CLOCK_DFT,               /* static parameter for FOSC clock record. */
    },
    .pll0         =
    {
      .enable     = true,                         /* SCG_PLL0CSR[EN] bit, PLL0 clock enable */
      .lock       = false,                        /* SCG_PLL0CSR[LK] bit, Write to set the register can be written or not. */
      .cm         = false,                        /* SCG_PLL0CSR[CM] bit,Clock Monitor enable. */
      .cmre       = false,                        /* SCG_PLL0CSR[CMRE] bit, 1:Clock Monitor Reset, 0: interrupt */
      .sten       = true,                         /* SCG_PLL0CSR[STEN] bit, Clock Standby Enable */
      .divh       = SCG_ASYNCCLOCKDIV_BY2,        /* SCG_PLL0DIV[DIVH] bit field definition. */
      .divm       = SCG_ASYNCCLOCKDIV_BY2,        /* SCG_PLL0DIV[DIVM] bit field definition. */
      .divl       = SCG_ASYNCCLOCKDIV_BY4,        /* SCG_PLL0DIV[DIVL] bit field definition. */
      .prediv     = 11U,                          /* SCG_PLL0CFG[PREDIV] bit field definition. */
      .pstdiv     = SCG_PLLPSTDIV_BY2,
      .mult       = 299U,                         /* SCG_PLL0CFG[MULT] bit field definition. */
      .src        = SCG_PLLSOURCE_FOSC,           /* SCG_PLL0CFG[SOURCE] bit field definition. */
    },
    .pll1         =
    {
      .enable     = true,                         /* SCG_PLL0CSR[EN] bit, PLL0 clock enable */
      .lock       = false,                        /* SCG_PLL0CSR[LK] bit, Write to set the register can be written or not. */
      .cm         = false,                        /* SCG_PLL0CSR[CM] bit,Clock Monitor enable. */
      .cmre       = false,                        /* SCG_PLL0CSR[CMRE] bit, 1:Clock Monitor Reset, 0: interrupt */
      .sten       = true,                         /* SCG_PLL0CSR[STEN] bit, Clock Standby Enable */
      .divh       = SCG_ASYNCCLOCKDIV_BY2,        /* SCG_PLL0DIV[DIVH] bit field definition. */
      .divm       = SCG_ASYNCCLOCKDIV_BY2,        /* SCG_PLL0DIV[DIVM] bit field definition. */
      .divl       = SCG_ASYNCCLOCKDIV_BY4,        /* SCG_PLL0DIV[DIVL] bit field definition. */
      .prediv     = 11U,                          /* SCG_PLL0CFG[PREDIV] bit field definition. */
      .pstdiv     = SCG_PLLPSTDIV_BY2,
      .mult       = 249U,                         /* SCG_PLL0CFG[MULT] bit field definition. */
      .src        = SCG_PLLSOURCE_FOSC,           /* SCG_PLL0CFG[SOURCE] bit field definition. */
    },
    .sosc          =
    {
      .enable = true,                             /* SCG_SOSCCSR[en] bit, sosc clock enable */
      .lock   = false,                            /* SCG_SOSCCSR[lk] bit, write to set the register can be written or not. */
      .bypass = false,                            /* SCG_SOSCCSR[bypass] bit, configures sosc for bypassing the internal oscillator. */
      .cm     = false,                            /* SCG_SOSCCSR[cm] bit,clock monitor enable. */
      .cmre   = false,                            /* SCG_SOSCCSR[cmre] bit, 1:clock monitor reset, 0: interrupt */
    },
    .clockctrl     =
    {
      .sysclkmonitor     = false,                  /* SCG_CCR[SYSCLK_CME], System Clock monitor bit. */
      .src               = SCG_CLOCK_SRC_PLL0,     /* SCG_CCR[SCS], System Clock Source. */
      .divslow           = SCG_CLOCK_DIV_BY2,      /* SCG_CCR[DIVSLOW], Slow Clock Divide Ratio. */
      .divbus            = SCG_CLOCK_DIV_BY2,      /* SCG_CCR[DIVBUS], Bus Clock Divide Ratio. */
      .divcore           = SCG_CLOCK_DIV_BY1,      /* SCG_CCR[DIVCORE], Core Clock Divide Ratio. */
      .clkoursrc         = SCG_CLOCKOUT_SRC_OFF,
    },
  },
  .pcc             =
  {
    .pclks         = g_peripheral_clockconfig0,     /* Peripheral clock configurations */
  },
};

/****************************************************************************
 * Public Functions
 ****************************************************************************/
