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
#include "fc7300_periphclocks.h"
#include "fc7300f8m-evb.h"

/****************************************************************************
 * Public Data
 ****************************************************************************/

/* Each FC7300 board must provide the following initialized structure.
 * This is needed to establish the initial peripheral clocking.
 */

const struct peripheral_clock_config_s g_peripheral_clockconfig0[] =
{
  {
    .eclockname = PCC_CLK_ADC0,
#ifdef CONFIG_FC7300_ADC0
    .bclkgate = true,
#else
    .bclkgate = false,
#endif
    .eclksrc = PCC_CLKGATE_SRC_FOSCDIV,          /* Peripheral clock source. */
    .edivider = PCC_CLK_DIV_BY1,                 /* Peripheral clock divider value. */
    .ectrlowner = PCC_CTRL_BY_CPU0,
    .blockctrl = false,
  },
  {
    .eclockname = PCC_CLK_ADC1,
#ifdef CONFIG_FC7300_ADC1
    .bclkgate = true,
#else
    .bclkgate = false,
#endif
    .eclksrc = PCC_CLKGATE_SRC_FOSCDIV,          /* Peripheral clock source. */
    .edivider = PCC_CLK_DIV_BY1,                 /* Peripheral clock divider value. */
    .ectrlowner = PCC_CTRL_BY_CPU0,
    .blockctrl = false,
  },
  {
    .eclockname = PCC_CLK_ADC2,
#ifdef CONFIG_FC7300_ADC2
    .bclkgate = true,
#else
    .bclkgate = false,
#endif
    .eclksrc = PCC_CLKGATE_SRC_FOSCDIV,          /* Peripheral clock source. */
    .edivider = PCC_CLK_DIV_BY1,                 /* Peripheral clock divider value. */
    .ectrlowner = PCC_CTRL_BY_CPU0,
    .blockctrl = false,
  },
  {
    .eclockname = PCC_CLK_ADC3,
#ifdef CONFIG_FC7300_ADC3
    .bclkgate = true,
#else
    .bclkgate = false,
#endif
    .eclksrc = PCC_CLKGATE_SRC_FOSCDIV,          /* Peripheral clock source. */
    .edivider = PCC_CLK_DIV_BY1,                 /* Peripheral clock divider value. */
    .ectrlowner = PCC_CTRL_BY_CPU0,
    .blockctrl = false,
  },
  {
    .eclockname = PCC_CLK_DMA0,
#ifdef CONFIG_FC7300_DMA0
    .bclkgate = true,
#else
    .bclkgate = false,
#endif
    .eclksrc = PCC_CLKGATE_SRC_OFF,              /* Peripheral clock source. */
    .edivider = PCC_CLK_DIV_BY1,                 /* Peripheral clock divider value. */
    .ectrlowner = PCC_CTRL_BY_CPU0,
    .blockctrl = false,
  },
  {
    .eclockname = PCC_CLK_DMAMUX0,
#ifdef CONFIG_FC7300_DMA0
    .bclkgate = true,
#else
    .bclkgate = false,
#endif
    .eclksrc = PCC_CLKGATE_SRC_OFF,              /* Peripheral clock source. */
    .edivider = PCC_CLK_DIV_BY1,                 /* Peripheral clock divider value. */
    .ectrlowner = PCC_CTRL_BY_CPU0,
    .blockctrl = false,
  },
  {
    .eclockname = PCC_CLK_DMA1,
#ifdef CONFIG_FC7300_DMA1
    .bclkgate = true,
#else
    .bclkgate = false,
#endif
    .eclksrc = PCC_CLKGATE_SRC_OFF,              /* Peripheral clock source. */
    .edivider = PCC_CLK_DIV_BY1,                 /* Peripheral clock divider value. */
    .ectrlowner = PCC_CTRL_BY_CPU0,
    .blockctrl = false,
  },
  {
    .eclockname = PCC_CLK_DMAMUX1,
#ifdef CONFIG_FC7300_DMA1
    .bclkgate = true,
#else
    .bclkgate = false,
#endif
    .eclksrc = PCC_CLKGATE_SRC_OFF,              /* Peripheral clock source. */
    .edivider = PCC_CLK_DIV_BY1,                 /* Peripheral clock divider value. */
    .ectrlowner = PCC_CTRL_BY_CPU0,
    .blockctrl = false,
  },
  {
    .eclockname = PCC_CLK_FLEXCAN0,
#ifdef CONFIG_FC7300_FLEXCAN0
    .bclkgate = true,
#else
    .bclkgate = false,
#endif
    .eclksrc = PCC_CLKGATE_SRC_PLL0DIV,          /* Peripheral clock source. */
    .edivider = PCC_CLK_DIV_BY1,                 /* Peripheral clock divider value. */
    .ectrlowner = PCC_CTRL_BY_CPU0,
    .blockctrl = false,
  },
  {
    .eclockname = PCC_CLK_FLEXCAN1,
#ifdef CONFIG_FC7300_FLEXCAN1
    .bclkgate = true,
#else
    .bclkgate = false,
#endif
    .eclksrc = PCC_CLKGATE_SRC_PLL0DIV,          /* Peripheral clock source. */
    .edivider = PCC_CLK_DIV_BY1,                 /* Peripheral clock divider value. */
    .ectrlowner = PCC_CTRL_BY_CPU0,
    .blockctrl = false,
  },
  {
    .eclockname = PCC_CLK_FLEXCAN2,
#ifdef CONFIG_FC7300_FLEXCAN2
    .bclkgate = true,
#else
    .bclkgate = false,
#endif
    .eclksrc = PCC_CLKGATE_SRC_PLL0DIV,          /* Peripheral clock source. */
    .edivider = PCC_CLK_DIV_BY1,                 /* Peripheral clock divider value. */
    .ectrlowner = PCC_CTRL_BY_CPU0,
    .blockctrl = false,
  },
  {
    .eclockname = PCC_CLK_FLEXCAN3,
#ifdef CONFIG_FC7300_FLEXCAN3
    .bclkgate = true,
#else
    .bclkgate = false,
#endif
    .eclksrc = PCC_CLKGATE_SRC_PLL0DIV,          /* Peripheral clock source. */
    .edivider = PCC_CLK_DIV_BY1,                 /* Peripheral clock divider value. */
    .ectrlowner = PCC_CTRL_BY_CPU0,
    .blockctrl = false,
  },
  {
    .eclockname = PCC_CLK_FLEXCAN4,
#ifdef CONFIG_FC7300_FLEXCAN4
    .bclkgate = true,
#else
    .bclkgate = false,
#endif
    .eclksrc = PCC_CLKGATE_SRC_PLL0DIV,          /* Peripheral clock source. */
    .edivider = PCC_CLK_DIV_BY1,                 /* Peripheral clock divider value. */
    .ectrlowner = PCC_CTRL_BY_CPU0,
    .blockctrl = false,
  },
  {
    .eclockname = PCC_CLK_FLEXCAN5,
#ifdef CONFIG_FC7300_FLEXCAN5
    .bclkgate = true,
#else
    .bclkgate = false,
#endif
    .eclksrc = PCC_CLKGATE_SRC_PLL0DIV,          /* Peripheral clock source. */
    .edivider = PCC_CLK_DIV_BY1,                 /* Peripheral clock divider value. */
    .ectrlowner = PCC_CTRL_BY_CPU0,
    .blockctrl = false,
  },
  {
    .eclockname = PCC_CLK_FLEXCAN6,
#ifdef CONFIG_FC7300_FLEXCAN6
    .bclkgate = true,
#else
    .bclkgate = false,
#endif
    .eclksrc = PCC_CLKGATE_SRC_PLL0DIV,          /* Peripheral clock source. */
    .edivider = PCC_CLK_DIV_BY1,                 /* Peripheral clock divider value. */
    .ectrlowner = PCC_CTRL_BY_CPU0,
    .blockctrl = false,
  },
  {
    .eclockname = PCC_CLK_FLEXCAN7,
#ifdef CONFIG_FC7300_FLEXCAN7
    .bclkgate = true,
#else
    .bclkgate = false,
#endif
    .eclksrc = PCC_CLKGATE_SRC_PLL0DIV,          /* Peripheral clock source. */
    .edivider = PCC_CLK_DIV_BY1,                 /* Peripheral clock divider value. */
    .ectrlowner = PCC_CTRL_BY_CPU0,
    .blockctrl = false,
  },
  {
    .eclockname = PCC_CLK_FLEXCAN8,
#ifdef CONFIG_FC7300_FLEXCAN8
    .bclkgate = true,
#else
    .bclkgate = false,
#endif
    .eclksrc = PCC_CLKGATE_SRC_PLL0DIV,          /* Peripheral clock source. */
    .edivider = PCC_CLK_DIV_BY1,                 /* Peripheral clock divider value. */
    .ectrlowner = PCC_CTRL_BY_CPU0,
    .blockctrl = false,
  },
  {
    .eclockname = PCC_CLK_FLEXCAN9,
#ifdef CONFIG_FC7300_FLEXCAN9
    .bclkgate = true,
#else
    .bclkgate = false,
#endif
    .eclksrc = PCC_CLKGATE_SRC_PLL0DIV,          /* Peripheral clock source. */
    .edivider = PCC_CLK_DIV_BY1,                 /* Peripheral clock divider value. */
    .ectrlowner = PCC_CTRL_BY_CPU0,
    .blockctrl = false,
  },
  {
    .eclockname = PCC_CLK_FTU0,
#ifdef CONFIG_FC7300_FTU0
    .bclkgate = true,
#else
    .bclkgate = false,
#endif
    .eclksrc = PCC_CLKGATE_SRC_FOSCDIV,          /* Peripheral clock source. */
    .edivider = PCC_CLK_DIV_BY1,                 /* Peripheral clock divider value. */
    .ectrlowner = PCC_CTRL_BY_CPU0,
    .blockctrl = false,
  },
  {
    .eclockname = PCC_CLK_FTU1,
#ifdef CONFIG_FC7300_FTU1
    .bclkgate = true,
#else
    .bclkgate = false,
#endif
    .eclksrc = PCC_CLKGATE_SRC_FOSCDIV,          /* Peripheral clock source. */
    .edivider = PCC_CLK_DIV_BY1,                 /* Peripheral clock divider value. */
    .ectrlowner = PCC_CTRL_BY_CPU0,
    .blockctrl = false,
  },
  {
    .eclockname = PCC_CLK_FTU2,
#ifdef CONFIG_FC7300_FTU2
    .bclkgate = true,
#else
    .bclkgate = false,
#endif
    .eclksrc = PCC_CLKGATE_SRC_FOSCDIV,          /* Peripheral clock source. */
    .edivider = PCC_CLK_DIV_BY1,                 /* Peripheral clock divider value. */
    .ectrlowner = PCC_CTRL_BY_CPU0,
    .blockctrl = false,
  },
  {
    .eclockname = PCC_CLK_FTU3,
#ifdef CONFIG_FC7300_FTU3
    .bclkgate = true,
#else
    .bclkgate = false,
#endif
    .eclksrc = PCC_CLKGATE_SRC_FOSCDIV,          /* Peripheral clock source. */
    .edivider = PCC_CLK_DIV_BY1,                 /* Peripheral clock divider value. */
    .ectrlowner = PCC_CTRL_BY_CPU0,
    .blockctrl = false,
  },
  {
    .eclockname = PCC_CLK_FTU4,
#ifdef CONFIG_FC7300_FTU4
    .bclkgate = true,
#else
    .bclkgate = false,
#endif
    .eclksrc = PCC_CLKGATE_SRC_FOSCDIV,          /* Peripheral clock source. */
    .edivider = PCC_CLK_DIV_BY1,                 /* Peripheral clock divider value. */
    .ectrlowner = PCC_CTRL_BY_CPU0,
    .blockctrl = false,
  },
  {
    .eclockname = PCC_CLK_FTU5,
#ifdef CONFIG_FC7300_FTU5
    .bclkgate = true,
#else
    .bclkgate = false,
#endif
    .eclksrc = PCC_CLKGATE_SRC_FOSCDIV,          /* Peripheral clock source. */
    .edivider = PCC_CLK_DIV_BY1,                 /* Peripheral clock divider value. */
    .ectrlowner = PCC_CTRL_BY_CPU0,
    .blockctrl = false,
  },
  {
    .eclockname = PCC_CLK_FTU6,
#ifdef CONFIG_FC7300_FTU6
    .bclkgate = true,
#else
    .bclkgate = false,
#endif
    .eclksrc = PCC_CLKGATE_SRC_FOSCDIV,          /* Peripheral clock source. */
    .edivider = PCC_CLK_DIV_BY1,                 /* Peripheral clock divider value. */
    .ectrlowner = PCC_CTRL_BY_CPU0,
    .blockctrl = false,
  },
  {
    .eclockname = PCC_CLK_FTU7,
#ifdef CONFIG_FC7300_FTU7
    .bclkgate = true,
#else
    .bclkgate = false,
#endif
    .eclksrc = PCC_CLKGATE_SRC_FOSCDIV,          /* Peripheral clock source. */
    .edivider = PCC_CLK_DIV_BY1,                 /* Peripheral clock divider value. */
    .ectrlowner = PCC_CTRL_BY_CPU0,
    .blockctrl = false,
  },
  {
    .eclockname = PCC_CLK_FTU8,
#ifdef CONFIG_FC7300_FTU8
    .bclkgate = true,
#else
    .bclkgate = false,
#endif
    .eclksrc = PCC_CLKGATE_SRC_FOSCDIV,          /* Peripheral clock source. */
    .edivider = PCC_CLK_DIV_BY1,                 /* Peripheral clock divider value. */
    .ectrlowner = PCC_CTRL_BY_CPU0,
    .blockctrl = false,
  },
  {
    .eclockname = PCC_CLK_FTU9,
#ifdef CONFIG_FC7300_FTU9
    .bclkgate = true,
#else
    .bclkgate = false,
#endif
    .eclksrc = PCC_CLKGATE_SRC_FOSCDIV,          /* Peripheral clock source. */
    .edivider = PCC_CLK_DIV_BY1,                 /* Peripheral clock divider value. */
    .ectrlowner = PCC_CTRL_BY_CPU0,
    .blockctrl = false,
  },
  {
    .eclockname = PCC_CLK_FTU10,
#ifdef CONFIG_FC7300_FTU10
    .bclkgate = true,
#else
    .bclkgate = false,
#endif
    .eclksrc = PCC_CLKGATE_SRC_FOSCDIV,          /* Peripheral clock source. */
    .edivider = PCC_CLK_DIV_BY1,                 /* Peripheral clock divider value. */
    .ectrlowner = PCC_CTRL_BY_CPU0,
    .blockctrl = false,
  },
  {
    .eclockname = PCC_CLK_FTU11,
#ifdef CONFIG_FC7300_FTU11
    .bclkgate = true,
#else
    .bclkgate = false,
#endif
    .eclksrc = PCC_CLKGATE_SRC_FOSCDIV,          /* Peripheral clock source. */
    .edivider = PCC_CLK_DIV_BY1,                 /* Peripheral clock divider value. */
    .ectrlowner = PCC_CTRL_BY_CPU0,
    .blockctrl = false,
  },
  {
    .eclockname = PCC_CLK_FCIIC0,
#ifdef CONFIG_FC7300_FCIIC0
    .bclkgate = true,
#else
    .bclkgate = false,
#endif
    .eclksrc = PCC_CLKGATE_SRC_FOSCDIV,          /* Peripheral clock source. */
    .edivider = PCC_CLK_DIV_BY1,                 /* Peripheral clock divider value. */
    .ectrlowner = PCC_CTRL_BY_CPU0,
    .blockctrl = false,
  },
  {
    .eclockname = PCC_CLK_FCIIC1,
#ifdef CONFIG_FC7300_FCIIC1
    .bclkgate = true,
#else
    .bclkgate = false,
#endif
    .eclksrc = PCC_CLKGATE_SRC_FOSCDIV,          /* Peripheral clock source. */
    .edivider = PCC_CLK_DIV_BY1,                 /* Peripheral clock divider value. */
    .ectrlowner = PCC_CTRL_BY_CPU0,
    .blockctrl = false,
  },
  {
    .eclockname = PCC_CLK_FCSPI0,
#ifdef CONFIG_FC7300_FCSPI0
    .bclkgate = true,
#else
    .bclkgate = false,
#endif
    .eclksrc = PCC_CLKGATE_SRC_FOSCDIV,          /* Peripheral clock source. */
    .edivider = PCC_CLK_DIV_BY1,                 /* Peripheral clock divider value. */
    .ectrlowner = PCC_CTRL_BY_CPU0,
    .blockctrl = false,
  },
  {
    .eclockname = PCC_CLK_FCSPI1,
#ifdef CONFIG_FC7300_FCSPI1
    .bclkgate = true,
#else
    .bclkgate = false,
#endif
    .eclksrc = PCC_CLKGATE_SRC_FOSCDIV,          /* Peripheral clock source. */
    .edivider = PCC_CLK_DIV_BY1,                 /* Peripheral clock divider value. */
    .ectrlowner = PCC_CTRL_BY_CPU0,
    .blockctrl = false,
  },
  {
    .eclockname = PCC_CLK_FCSPI2,
#ifdef CONFIG_FC7300_FCSPI2
    .bclkgate = true,
#else
    .bclkgate = false,
#endif
    .eclksrc = PCC_CLKGATE_SRC_FOSCDIV,          /* Peripheral clock source. */
    .edivider = PCC_CLK_DIV_BY1,                 /* Peripheral clock divider value. */
    .ectrlowner = PCC_CTRL_BY_CPU0,
    .blockctrl = false,
  },
  {
    .eclockname = PCC_CLK_FCSPI3,
#ifdef CONFIG_FC7300_FCSPI3
    .bclkgate = true,
#else
    .bclkgate = false,
#endif
    .eclksrc = PCC_CLKGATE_SRC_FOSCDIV,          /* Peripheral clock source. */
    .edivider = PCC_CLK_DIV_BY1,                 /* Peripheral clock divider value. */
    .ectrlowner = PCC_CTRL_BY_CPU0,
    .blockctrl = false,
  },
  {
    .eclockname = PCC_CLK_FCSPI4,
#ifdef CONFIG_FC7300_FCSPI4
    .bclkgate = true,
#else
    .bclkgate = false,
#endif
    .eclksrc = PCC_CLKGATE_SRC_FOSCDIV,          /* Peripheral clock source. */
    .edivider = PCC_CLK_DIV_BY1,                 /* Peripheral clock divider value. */
    .ectrlowner = PCC_CTRL_BY_CPU0,
    .blockctrl = false,
  },
  {
    .eclockname = PCC_CLK_FCSPI5,
#ifdef CONFIG_FC7300_FCSPI5
    .bclkgate = true,
#else
    .bclkgate = false,
#endif
    .eclksrc = PCC_CLKGATE_SRC_FOSCDIV,          /* Peripheral clock source. */
    .edivider = PCC_CLK_DIV_BY1,                 /* Peripheral clock divider value. */
    .ectrlowner = PCC_CTRL_BY_CPU0,
    .blockctrl = false,
  },
  {
    .eclockname = PCC_CLK_FCSPI6,
#ifdef CONFIG_FC7300_FCSPI6
    .bclkgate = true,
#else
    .bclkgate = false,
#endif
    .eclksrc = PCC_CLKGATE_SRC_FOSCDIV,          /* Peripheral clock source. */
    .edivider = PCC_CLK_DIV_BY1,                 /* Peripheral clock divider value. */
    .ectrlowner = PCC_CTRL_BY_CPU0,
    .blockctrl = false,
  },
  {
    .eclockname = PCC_CLK_FCSPI7,
#ifdef CONFIG_FC7300_FCSPI7
    .bclkgate = true,
#else
    .bclkgate = false,
#endif
    .eclksrc = PCC_CLKGATE_SRC_FOSCDIV,          /* Peripheral clock source. */
    .edivider = PCC_CLK_DIV_BY1,                 /* Peripheral clock divider value. */
    .ectrlowner = PCC_CTRL_BY_CPU0,
    .blockctrl = false,
  },
  {
    .eclockname = PCC_CLK_FCUART0,
#if (defined(CONFIG_FC7300_FCUART0) || defined(CONFIG_FC7300_LIN0))
    .bclkgate = true,
#else
    .bclkgate = false,
#endif
    .eclksrc = PCC_CLKGATE_SRC_FOSCDIV,          /* Peripheral clock source. */
    .edivider = PCC_CLK_DIV_BY1,                 /* Peripheral clock divider value. */
    .ectrlowner = PCC_CTRL_BY_CPU0,
    .blockctrl = false,
  },
  {
    .eclockname = PCC_CLK_FCUART1,
#if (defined(CONFIG_FC7300_FCUART1) || defined(CONFIG_FC7300_LIN1))
    .bclkgate = true,
#else
    .bclkgate = false,
#endif
    .eclksrc = PCC_CLKGATE_SRC_FOSCDIV,          /* Peripheral clock source. */
    .edivider = PCC_CLK_DIV_BY1,                 /* Peripheral clock divider value. */
    .ectrlowner = PCC_CTRL_BY_CPU0,
    .blockctrl = false,
  },
  {
    .eclockname = PCC_CLK_FCUART2,
#if (defined(CONFIG_FC7300_FCUART2) || defined(CONFIG_FC7300_LIN2))
    .bclkgate = true,
#else
    .bclkgate = false,
#endif
    .eclksrc = PCC_CLKGATE_SRC_FOSCDIV,          /* Peripheral clock source. */
    .edivider = PCC_CLK_DIV_BY1,                 /* Peripheral clock divider value. */
    .ectrlowner = PCC_CTRL_BY_CPU0,
    .blockctrl = false,
  },
  {
    .eclockname = PCC_CLK_FCUART3,
#if (defined(CONFIG_FC7300_FCUART3) || defined(CONFIG_FC7300_LIN3))
    .bclkgate = true,
#else
    .bclkgate = false,
#endif
    .eclksrc = PCC_CLKGATE_SRC_FOSCDIV,          /* Peripheral clock source. */
    .edivider = PCC_CLK_DIV_BY1,                 /* Peripheral clock divider value. */
    .ectrlowner = PCC_CTRL_BY_CPU0,
    .blockctrl = false,
  },
  {
    .eclockname = PCC_CLK_FCUART4,
#if (defined(CONFIG_FC7300_FCUART4) || defined(CONFIG_FC7300_LIN4))
    .bclkgate = true,
#else
    .bclkgate = false,
#endif
    .eclksrc = PCC_CLKGATE_SRC_FOSCDIV,          /* Peripheral clock source. */
    .edivider = PCC_CLK_DIV_BY1,                 /* Peripheral clock divider value. */
    .ectrlowner = PCC_CTRL_BY_CPU0,
    .blockctrl = false,
  },
  {
    .eclockname = PCC_CLK_FCUART5,
#if (defined(CONFIG_FC7300_FCUART5) || defined(CONFIG_FC7300_LIN5))
    .bclkgate = true,
#else
    .bclkgate = false,
#endif
    .eclksrc = PCC_CLKGATE_SRC_FOSCDIV,          /* Peripheral clock source. */
    .edivider = PCC_CLK_DIV_BY1,                 /* Peripheral clock divider value. */
    .ectrlowner = PCC_CTRL_BY_CPU0,
    .blockctrl = false,
  },
  {
    .eclockname = PCC_CLK_FCUART6,
#if (defined(CONFIG_FC7300_FCUART6) || defined(CONFIG_FC7300_LIN6))
    .bclkgate = true,
#else
    .bclkgate = false,
#endif
    .eclksrc = PCC_CLKGATE_SRC_FOSCDIV,          /* Peripheral clock source. */
    .edivider = PCC_CLK_DIV_BY1,                 /* Peripheral clock divider value. */
    .ectrlowner = PCC_CTRL_BY_CPU0,
    .blockctrl = false,
  },
  {
    .eclockname = PCC_CLK_FCUART7,
#if (defined(CONFIG_FC7300_FCUART7) || defined(CONFIG_FC7300_LIN7))
    .bclkgate = true,
#else
    .bclkgate = false,
#endif
    .eclksrc = PCC_CLKGATE_SRC_FOSCDIV,          /* Peripheral clock source. */
    .edivider = PCC_CLK_DIV_BY1,                 /* Peripheral clock divider value. */
    .ectrlowner = PCC_CTRL_BY_CPU0,
    .blockctrl = false,
  },
  {
    .eclockname = PCC_CLK_FCUART8,
#if (defined(CONFIG_FC7300_FCUART8) || defined(CONFIG_FC7300_LIN8))
    .bclkgate = true,
#else
    .bclkgate = false,
#endif
    .eclksrc = PCC_CLKGATE_SRC_FOSCDIV,          /* Peripheral clock source. */
    .edivider = PCC_CLK_DIV_BY1,                 /* Peripheral clock divider value. */
    .ectrlowner = PCC_CTRL_BY_CPU0,
    .blockctrl = false,
  },
  {
    .eclockname = PCC_CLK_FCUART9,
#if (defined(CONFIG_FC7300_FCUART9) || defined(CONFIG_FC7300_LIN9))
    .bclkgate = true,
#else
    .bclkgate = false,
#endif
    .eclksrc = PCC_CLKGATE_SRC_FOSCDIV,          /* Peripheral clock source. */
    .edivider = PCC_CLK_DIV_BY1,                 /* Peripheral clock divider value. */
    .ectrlowner = PCC_CTRL_BY_CPU0,
    .blockctrl = false,
  },
  {
    .eclockname = PCC_CLK_FCUART10,
#if (defined(CONFIG_FC7300_FCUART10) || defined(CONFIG_FC7300_LIN10))
    .bclkgate = true,
#else
    .bclkgate = false,
#endif
    .eclksrc = PCC_CLKGATE_SRC_FOSCDIV,          /* Peripheral clock source. */
    .edivider = PCC_CLK_DIV_BY1,                 /* Peripheral clock divider value. */
    .ectrlowner = PCC_CTRL_BY_CPU0,
    .blockctrl = false,
  },
  {
    .eclockname = PCC_CLK_FCUART11,
#if (defined(CONFIG_FC7300_FCUART11) || defined(CONFIG_FC7300_LIN11))
    .bclkgate = true,
#else
    .bclkgate = false,
#endif
    .eclksrc = PCC_CLKGATE_SRC_FOSCDIV,          /* Peripheral clock source. */
    .edivider = PCC_CLK_DIV_BY1,                 /* Peripheral clock divider value. */
    .ectrlowner = PCC_CTRL_BY_CPU0,
    .blockctrl = false,
  },
  {
    .eclockname = PCC_CLK_FCUART12,
#if (defined(CONFIG_FC7300_FCUART12) || defined(CONFIG_FC7300_LIN12))
    .bclkgate = true,
#else
    .bclkgate = false,
#endif
    .eclksrc = PCC_CLKGATE_SRC_FOSCDIV,          /* Peripheral clock source. */
    .edivider = PCC_CLK_DIV_BY1,                 /* Peripheral clock divider value. */
    .ectrlowner = PCC_CTRL_BY_CPU0,
    .blockctrl = false,
  },
  {
    .eclockname = PCC_CLK_FCUART13,
#if (defined(CONFIG_FC7300_FCUART13) || defined(CONFIG_FC7300_LIN13))
    .bclkgate = true,
#else
    .bclkgate = false,
#endif
    .eclksrc = PCC_CLKGATE_SRC_FOSCDIV,          /* Peripheral clock source. */
    .edivider = PCC_CLK_DIV_BY1,                 /* Peripheral clock divider value. */
    .ectrlowner = PCC_CTRL_BY_CPU0,
    .blockctrl = false,
  },
  {
    .eclockname = PCC_CLK_FCUART14,
#if (defined(CONFIG_FC7300_FCUART14) || defined(CONFIG_FC7300_LIN14))
    .bclkgate = true,
#else
    .bclkgate = false,
#endif
    .eclksrc = PCC_CLKGATE_SRC_FOSCDIV,          /* Peripheral clock source. */
    .edivider = PCC_CLK_DIV_BY1,                 /* Peripheral clock divider value. */
    .ectrlowner = PCC_CTRL_BY_CPU0,
    .blockctrl = false,
  },
  {
    .eclockname = PCC_CLK_FCUART15,
#if (defined(CONFIG_FC7300_FCUART15) || defined(CONFIG_FC7300_LIN15))
    .bclkgate = true,
#else
    .bclkgate = false,
#endif
    .eclksrc = PCC_CLKGATE_SRC_FOSCDIV,          /* Peripheral clock source. */
    .edivider = PCC_CLK_DIV_BY1,                 /* Peripheral clock divider value. */
    .ectrlowner = PCC_CTRL_BY_CPU0,
    .blockctrl = false,
  },
  {
    .eclockname = PCC_CLK_FCUART16,
#if (defined(CONFIG_FC7300_FCUART16) || defined(CONFIG_FC7300_LIN16))
    .bclkgate = true,
#else
    .bclkgate = false,
#endif
    .eclksrc = PCC_CLKGATE_SRC_FOSCDIV,          /* Peripheral clock source. */
    .edivider = PCC_CLK_DIV_BY1,                 /* Peripheral clock divider value. */
    .ectrlowner = PCC_CTRL_BY_CPU0,
    .blockctrl = false,
  },
  {
    .eclockname = PCC_CLK_FCUART17,
#if (defined(CONFIG_FC7300_FCUART17) || defined(CONFIG_FC7300_LIN17))
    .bclkgate = true,
#else
    .bclkgate = false,
#endif
    .eclksrc = PCC_CLKGATE_SRC_FOSCDIV,          /* Peripheral clock source. */
    .edivider = PCC_CLK_DIV_BY1,                 /* Peripheral clock divider value. */
    .ectrlowner = PCC_CTRL_BY_CPU0,
    .blockctrl = false,
  },
  {
    .eclockname = PCC_CLK_ISM,
#ifdef CONFIG_FC7300_ISM0
    .bclkgate = true,
#else
    .bclkgate = false,
#endif
    .eclksrc = PCC_CLKGATE_SRC_FOSCDIV,          /* Peripheral clock source. */
    .edivider = PCC_CLK_DIV_BY1,                 /* Peripheral clock divider value. */
    .ectrlowner = PCC_CTRL_BY_CPU0,
    .blockctrl = false,
  },
  {
    .eclockname = PCC_CLK_TRGSEL0,
#ifdef CONFIG_FC7300_ADC3
    .bclkgate = true,
#else
    .bclkgate = false,
#endif
    .eclksrc = PCC_CLKGATE_SRC_FOSCDIV,          /* Peripheral clock source. */
    .edivider = PCC_CLK_DIV_BY1,                 /* Peripheral clock divider value. */
    .ectrlowner = PCC_CTRL_BY_CPU0,
    .blockctrl = false,
  },
  {
    .eclockname = PCC_CLK_TRGSEL3,
#ifdef CONFIG_FC7300_ISM0
    .bclkgate = true,
#else
    .bclkgate = false,
#endif
    .eclksrc = PCC_CLKGATE_SRC_FOSCDIV,          /* Peripheral clock source. */
    .edivider = PCC_CLK_DIV_BY1,                 /* Peripheral clock divider value. */
    .ectrlowner = PCC_CTRL_BY_CPU0,
    .blockctrl = false,
  },
  {
    .eclockname = PCC_CLK_TSTMP1,
    .bclkgate = true,
    .eclksrc = PCC_CLKGATE_SRC_FOSCDIV,          /* Peripheral clock source. */
    .edivider = PCC_CLK_DIV_BY1,                 /* Peripheral clock divider value. */
    .ectrlowner = PCC_CTRL_BY_CPU0,
    .blockctrl = false,
  },
  {
    .eclockname = PCC_CLK_TSTMP2,
    .bclkgate = true,
    .eclksrc = PCC_CLKGATE_SRC_FOSCDIV,          /* Peripheral clock source. */
    .edivider = PCC_CLK_DIV_BY1,                 /* Peripheral clock divider value. */
    .ectrlowner = PCC_CTRL_BY_CPU0,
    .blockctrl = false,
  },
  {
    .eclockname = PCC_CLK_TSTMP3,
    .bclkgate = true,
    .eclksrc = PCC_CLKGATE_SRC_FOSCDIV,          /* Peripheral clock source. */
    .edivider = PCC_CLK_DIV_BY1,                 /* Peripheral clock divider value. */
    .ectrlowner = PCC_CTRL_BY_CPU0,
    .blockctrl = false,
  },
  {
    .eclockname = PCC_CLK_FCPIT0,
#ifdef CONFIG_FC7300_FCPIT0
    .bclkgate = true,
#else
    .bclkgate = false,
#endif
    .eclksrc = PCC_CLKGATE_SRC_FOSCDIV,          /* Peripheral clock source. */
    .edivider = PCC_CLK_DIV_BY1,                 /* Peripheral clock divider value. */
    .ectrlowner = PCC_CTRL_BY_CPU0,
    .blockctrl = false,
  },
  {
    .eclockname = PCC_CLK_FCPIT1,
#ifdef CONFIG_FC7300_FCPIT1
    .bclkgate = true,
#else
    .bclkgate = false,
#endif
    .eclksrc = PCC_CLKGATE_SRC_FOSCDIV,          /* Peripheral clock source. */
    .edivider = PCC_CLK_DIV_BY1,                 /* Peripheral clock divider value. */
    .ectrlowner = PCC_CTRL_BY_CPU0,
    .blockctrl = false,
  },
  {
    .eclockname = PCC_CLK_CRC0,
    .bclkgate = true,
    .eclksrc = PCC_CLKGATE_SRC_FOSCDIV,          /* Peripheral clock source. */
    .edivider = PCC_CLK_DIV_BY1,                 /* Peripheral clock divider value. */
    .ectrlowner = PCC_CTRL_BY_CPU0,
    .blockctrl = false,
  },
  {
    .eclockname = PCC_CLK_ERM,
#ifdef CONFIG_FC7300_ERM
    .bclkgate = true,
#else
    .bclkgate = false,
#endif
    .eclksrc = PCC_CLKGATE_UNINVOLVED,           /* Peripheral clock source. */
    .edivider = PCC_CLK_UNINVOLVED,              /* Peripheral clock divider value. */
    .ectrlowner = PCC_CTRL_BY_CPU0,
    .blockctrl = false,
  },
  {
    .eclockname = PCC_CLK_EIM,
#ifdef CONFIG_FC7300_EIM
    .bclkgate = true,
#else
    .bclkgate = false,
#endif
    .eclksrc = PCC_CLKGATE_UNINVOLVED,           /* Peripheral clock source. */
    .edivider = PCC_CLK_UNINVOLVED,              /* Peripheral clock divider value. */
    .ectrlowner = PCC_CTRL_BY_CPU0,
    .blockctrl = false,
  },
  {
    .eclockname = PCC_CLK_CMU0,
#ifdef CONFIG_FC7300_CMU0
    .bclkgate = true,
#else
    .bclkgate = false,
#endif
    .eclksrc = PCC_CLKGATE_UNINVOLVED,           /* Peripheral clock source. */
    .edivider = PCC_CLK_UNINVOLVED,              /* Peripheral clock divider value. */
    .ectrlowner = PCC_CTRL_BY_CPU0,
    .blockctrl = false,
  },
  {
    .eclockname = PCC_CLK_CMU1,
#ifdef CONFIG_FC7300_CMU1
    .bclkgate = true,
#else
    .bclkgate = false,
#endif
    .eclksrc = PCC_CLKGATE_UNINVOLVED,           /* Peripheral clock source. */
    .edivider = PCC_CLK_UNINVOLVED,              /* Peripheral clock divider value. */
    .ectrlowner = PCC_CTRL_BY_CPU0,
    .blockctrl = false,
  },
  {
    .eclockname = PCC_CLK_CMU2,
#ifdef CONFIG_FC7300_CMU2
    .bclkgate = true,
#else
    .bclkgate = false,
#endif
    .eclksrc = PCC_CLKGATE_UNINVOLVED,           /* Peripheral clock source. */
    .edivider = PCC_CLK_UNINVOLVED,              /* Peripheral clock divider value. */
    .ectrlowner = PCC_CTRL_BY_CPU0,
    .blockctrl = false,
  },
  {
    .eclockname = PCC_CLK_CMU3,
#ifdef CONFIG_FC7300_CMU3
    .bclkgate = true,
#else
    .bclkgate = false,
#endif
    .eclksrc = PCC_CLKGATE_UNINVOLVED,           /* Peripheral clock source. */
    .edivider = PCC_CLK_UNINVOLVED,              /* Peripheral clock divider value. */
    .ectrlowner = PCC_CTRL_BY_CPU0,
    .blockctrl = false,
  },
  {
    .eclockname = PCC_CLK_CMU4,
#ifdef CONFIG_FC7300_CMU4
    .bclkgate = true,
#else
    .bclkgate = false,
#endif
    .eclksrc = PCC_CLKGATE_UNINVOLVED,           /* Peripheral clock source. */
    .edivider = PCC_CLK_UNINVOLVED,              /* Peripheral clock divider value. */
    .ectrlowner = PCC_CTRL_BY_CPU0,
    .blockctrl = false,
  },
  {
    .eclockname = PCC_CLK_STCU,
#ifdef CONFIG_FC7300_STCU
    .bclkgate = true,
#else
    .bclkgate = false,
#endif
    .eclksrc = PCC_CLKGATE_SRC_OFF,           /* Peripheral clock source. */
    .edivider = PCC_CLK_UNINVOLVED,           /* Peripheral clock divider value. */
    .ectrlowner = PCC_CTRL_BY_CPU0,
    .blockctrl = false,
  },
  {
    .eclockname = PCC_CLK_TMU,
#ifdef CONFIG_FC7300_TMU
    .bclkgate = true,
#else
    .bclkgate = false,
#endif
    .eclksrc = PCC_CLKGATE_SRC_PLL0DIV,          /* Peripheral clock source. */
    .edivider = PCC_CLK_DIV_BY1,                 /* Peripheral clock divider value. */
    .ectrlowner = PCC_CTRL_BY_CPU0,
    .blockctrl = false,
  },
  {
    .eclockname = PCC_CLK_ENET,
#ifdef CONFIG_FC7300_ENET0
    .bclkgate = true,
#else
    .bclkgate = false,
#endif
    .eclksrc = PCC_CLKGATE_SRC_PLL0DIV,          /* Peripheral clock source. */
    .edivider = PCC_CLK_DIV_BY1,                 /* Peripheral clock divider value. */
    .ectrlowner = PCC_CTRL_BY_CPU0,
    .blockctrl = false,
  }
};

unsigned int const g_peripheral_clocks_num_0 =
    sizeof(g_peripheral_clockconfig0) /
    sizeof(g_peripheral_clockconfig0[0]);

/****************************************************************************
 * Public Functions
 ****************************************************************************/
