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

#include <sys/types.h>
#include <assert.h>
#include <debug.h>

#include "arm_internal.h"
#include "hardware/fc7300_scg.h"
#include "hardware/fc7300_pcc.h"
#include "fc7300_clockconfig.h"
#include "fc7300_periphclocks.h"

/****************************************************************************
 * Public Data
 ****************************************************************************/

/* Peripheral PCC assignments */

struct peripheral_clock_assignments_s g_pcc_assignments[] =
{
  {PCC_CLK_FCPIT0, HAS_DIVIDER_H},
  {PCC_CLK_AONTIMER, HAS_DIVIDER_L},
  {PCC_CLK_ADC0, HAS_DIVIDER_H},
  {PCC_CLK_ADC1, HAS_DIVIDER_H},
  {PCC_CLK_TMU, HAS_DIVIDER_L},
  {PCC_CLK_SENT0, HAS_DIVIDER_L},
  {PCC_CLK_FTU0, HAS_DIVIDER_H},
  {PCC_CLK_FTU1, HAS_DIVIDER_H},
  {PCC_CLK_FTU2, HAS_DIVIDER_H},
  {PCC_CLK_FTU3, HAS_DIVIDER_H},
  {PCC_CLK_FTU4, HAS_DIVIDER_H},
  {PCC_CLK_FTU5, HAS_DIVIDER_H},
  {PCC_CLK_FCSPI0, HAS_DIVIDER_M},
  {PCC_CLK_FCSPI1, HAS_DIVIDER_M},
  {PCC_CLK_FCSPI2, HAS_DIVIDER_M},
  {PCC_CLK_FCSPI3, HAS_DIVIDER_M},
  {PCC_CLK_FCIIC0, HAS_DIVIDER_M},
  {PCC_CLK_FCUART0, HAS_DIVIDER_M},
  {PCC_CLK_FCUART1, HAS_DIVIDER_M},
  {PCC_CLK_FCUART2, HAS_DIVIDER_M},
  {PCC_CLK_FCUART3, HAS_DIVIDER_M},
  {PCC_CLK_FCUART4, HAS_DIVIDER_M},
  {PCC_CLK_FCUART5, HAS_DIVIDER_M},
  {PCC_CLK_FCUART6, HAS_DIVIDER_M},
  {PCC_CLK_FCUART7, HAS_DIVIDER_M},
#if defined(CONFIG_ARCH_CHIP_FC7300F8M) || defined(CONFIG_ARCH_CHIP_FC7300F4M)
  {PCC_CLK_ENET, HAS_DIVIDER_H},
#endif
  {PCC_CLK_FLEXCAN0, HAS_DIVIDER_H},
  {PCC_CLK_FLEXCAN1, HAS_DIVIDER_H},
  {PCC_CLK_FLEXCAN2, HAS_DIVIDER_H},
  {PCC_CLK_FLEXCAN3, HAS_DIVIDER_H},
  {PCC_CLK_FLEXCAN4, HAS_DIVIDER_H},
  {PCC_CLK_ADC2, HAS_DIVIDER_H},
#if defined(CONFIG_ARCH_CHIP_FC7300F8M) || defined(CONFIG_ARCH_CHIP_FC7300F4M)
  {PCC_CLK_ADC3, HAS_DIVIDER_H},
#endif
  {PCC_CLK_FCPIT1, HAS_DIVIDER_H},
#if defined(CONFIG_ARCH_CHIP_FC7300F8M)
  {PCC_CLK_FCUART16, HAS_DIVIDER_M},
  {PCC_CLK_FCUART17, HAS_DIVIDER_M},
  {PCC_CLK_FCSPI6, HAS_DIVIDER_M},
  {PCC_CLK_FCSPI7, HAS_DIVIDER_M},
#endif
#if defined(CONFIG_ARCH_CHIP_FC7300F8M) || defined(CONFIG_ARCH_CHIP_FC7300F4M)
  {PCC_CLK_SENT1, HAS_DIVIDER_L},
#endif
  {PCC_CLK_FTU6, HAS_DIVIDER_H},
  {PCC_CLK_FTU7, HAS_DIVIDER_H},
#if defined(CONFIG_ARCH_CHIP_FC7300F8M) || defined(CONFIG_ARCH_CHIP_FC7300F4M)
  {PCC_CLK_FTU8, HAS_DIVIDER_H},
  {PCC_CLK_FTU9, HAS_DIVIDER_H},
#endif
#if defined(CONFIG_ARCH_CHIP_FC7300F8M)
  {PCC_CLK_FTU10, HAS_DIVIDER_H},
  {PCC_CLK_FTU11, HAS_DIVIDER_H},
#endif
  {PCC_CLK_FCIIC1, HAS_DIVIDER_M},
#if defined(CONFIG_ARCH_CHIP_FC7300F8M) || defined(CONFIG_ARCH_CHIP_FC7300F4M)
  {PCC_CLK_FCUART8, HAS_DIVIDER_M},
  {PCC_CLK_FCUART9, HAS_DIVIDER_M},
#endif
#if defined(CONFIG_ARCH_CHIP_FC7300F8M)
  {PCC_CLK_FCUART10, HAS_DIVIDER_M},
  {PCC_CLK_FCUART11, HAS_DIVIDER_M},
  {PCC_CLK_FCUART12, HAS_DIVIDER_M},
  {PCC_CLK_FCUART13, HAS_DIVIDER_M},
  {PCC_CLK_FCUART14, HAS_DIVIDER_M},
  {PCC_CLK_FCUART15, HAS_DIVIDER_M},
#endif
  {PCC_CLK_FCSPI4, HAS_DIVIDER_M},
  {PCC_CLK_FCSPI5, HAS_DIVIDER_M},
  {PCC_CLK_MSC0, HAS_DIVIDER_M},
#if defined(CONFIG_ARCH_CHIP_FC7300F8M) || defined(CONFIG_ARCH_CHIP_FC7300F4M)
  {PCC_CLK_MSC1, HAS_DIVIDER_M},
  {PCC_CLK_FLEXCAN5, HAS_DIVIDER_H},
#endif
#if defined(CONFIG_ARCH_CHIP_FC7300F8M)
  {PCC_CLK_FLEXCAN6, HAS_DIVIDER_H},
  {PCC_CLK_FLEXCAN7, HAS_DIVIDER_H},
  {PCC_CLK_FLEXCAN8, HAS_DIVIDER_H},
  {PCC_CLK_FLEXCAN9, HAS_DIVIDER_H},
#endif
};

static const uint16_t g_pcc_assignments_len =
                sizeof(g_pcc_assignments) / sizeof(g_pcc_assignments[0]);

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: fc7300_get_pclkctrl
 *
 * Description:
 *   Given a clock name, this functions returns the address the the PCC
 *   control register for the peripheral.
 *
 * Input Parameters:
 *   clkname - The name of the peripheral clock.
 *
 * Returned Value:
 *   Address of peripheral control register.
 *
 ****************************************************************************/

static uint32_t *fc7300_get_pclkctrl(enum pcc_clksrctype_e clkname)
{
  /* Map the clock name to an index to the corresponding PCC control
   * register.
   */

  uint16_t index = (uint16_t)clkname;

  /* Return the fall address of the PCC control register */

  return (uint32_t *)((uint32_t)FC7300_PCC_BASE + (index * 4));
}

/****************************************************************************
 * Name: fc7300_set_pcc
 *
 * Description:
 *   Sets PCC control register.
 *
 * Input Parameters:
 *   pclk - Describes the PCLK configuration.
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/

static void fc7300_set_pcc(const struct peripheral_clock_config_s *pclk)
{
  uint32_t *ctrlp = fc7300_get_pclkctrl(pclk->eclockname);
  uint32_t regval;

  DEBUGASSERT(ctrlp != NULL);

  /* Configure the peripheral clock source, the fractional clock divider and
   * the clock gate.
   */

  regval = PCC_SEL(pclk->eclksrc);

  regval |= PCC_DIV(pclk->edivider);

  regval |= PCC_DWP(pclk->ectrlowner);

  if (pclk->blockctrl)
    {
      regval |= PCC_DWPLK_MASK;
    }

  if (pclk->bclkgate)
    {
      regval |= PCC_CGC(1);
    }

  *ctrlp = regval;
}

/****************************************************************************
 * Name: fc7300_reset_pcc
 *
 * Description:
 *   Resets peripherals through PCC register. Before resetting a peripheral by
 *   using the PCC, the peripheral's interrupt(s) should be disabled by
 *   deasserting the corresponding NVIC Interrupt Set-Enable Register or
 *   deasserting its interrupt enable bit(s), and the DMA function should be
 *   disabled by deasserting its DMA enable bit or disabling the DMA channels
 *   via DMAMUX.
 *
 * Input Parameters:
 *   eclockname - The name of the peripheral clock.
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/

void fc7300_reset_pcc(enum pcc_clksrctype_e eclockname)
{
  volatile uint32_t *ctrlp = fc7300_get_pclkctrl(eclockname);
  uint32_t regval;

  /* The following instructions must be exceuted step by step.
   * Make sure they are not optimized.
   */

  /* Disable CGC */

  regval = *ctrlp & ~PCC_CGC_MASK;
  *ctrlp = regval;

  /* Assert SWR bit */

  regval = *ctrlp | PCC_SWR_MASK;
  *ctrlp = regval;

  /* Deassert SWR bit */

  regval = *ctrlp & ~PCC_SWR_MASK;
  *ctrlp = regval;

  /* Enable CGC */

  regval = *ctrlp | PCC_CGC_MASK;
  *ctrlp = regval;
}

/****************************************************************************
 * Public Functions
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
                         const struct peripheral_clock_config_s *pclks)
{
  unsigned int i;

  DEBUGASSERT(pclks != NULL);

  for (i = 0; i < count; i++, pclks++)
    {
      /* Set peripheral clock control */

      fc7300_set_pcc(pclks);
    }
}

/****************************************************************************
 * Name: fc7300_get_pccmuxtype
 *
 * Description:
 *   Get pcc clock mux type.
 *
 * Input Parameters:
 *   clockname - PCC index.
 *
 * Returned Value:
 *   PCC clock mux type is returned on success. Zero is returned on any
 *   failure.
 *
 ****************************************************************************/

static uint8_t fc7300_get_pccmuxtype(enum pcc_clksrctype_e clockname)
{
  uint16_t index;

  for (index = 0; index < g_pcc_assignments_len; index++)
    {
      if (g_pcc_assignments[index].eclockname == clockname)
        {
          return g_pcc_assignments[index].clock_mux_type;
        }
    }

  return 0;
}

/****************************************************************************
 * Name: fc7300_get_pccsrcfreq
 *
 * Description:
 *   Get pcc clock's source clock frequency.
 *
 * Input Parameters:
 *   clksrc - The selected clock source for peripheral clock.
 *   clockname - PCC index.
 *
 * Returned Value:
 *  The frequency of the selected clock source. Zero is returned on any
 *  failure.
 *
 ****************************************************************************/

static uint32_t fc7300_get_pccsrcfreq(enum pcc_clkgatesrctype_e clksrc,
                                      enum pcc_clksrctype_e clockname)
{
  uint8_t pccmuxtype;
  uint32_t freq;

  pccmuxtype = fc7300_get_pccmuxtype(clockname);

  switch (clksrc)
    {
      case PCC_CLKGATE_SRC_FOSCDIV:
        freq = fc7300_get_foscfreq(pccmuxtype);
        break;

      case PCC_CLKGATE_SRC_SIRCDIV:
        freq = fc7300_get_sircfreq(pccmuxtype);
        break;

      case PCC_CLKGATE_SRC_FIRCDIV:
        freq = fc7300_get_fircfreq(pccmuxtype);
        break;

      case PCC_CLKGATE_SRC_PLL0DIV:
        freq = fc7300_get_pll0freq(pccmuxtype);
        break;

      case PCC_CLKGATE_SRC_PLL1DIV:
        freq = fc7300_get_pll1freq(pccmuxtype);
        break;

      default:
        freq = 0;
        break;
    }

  return freq;
}

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

int fc7300_get_pclkfreq(enum pcc_clksrctype_e clockname, uint32_t *frequency)
{
  uint32_t *ctrlp = fc7300_get_pclkctrl(clockname);
  uint32_t regval;
  uint32_t freq = 0;
  enum pcc_clkgatesrctype_e clksrc;
  enum pcc_clkdivtype_e pccdivider = PCC_CLK_DIV_BY1;
  bool clkgate;
  int ret = OK;

  DEBUGASSERT(ctrlp != NULL);

  regval = *ctrlp;

  clkgate = (regval & PCC_CGC_MASK) >> PCC_CGC_SHIFT;

  if (clkgate)
    {
      clksrc = (enum pcc_clkgatesrctype_e)((regval & PCC_SEL_MASK) >> PCC_SEL_SHIFT);

      pccdivider = (enum pcc_clkdivtype_e)((regval & PCC_DIV_MASK) >> PCC_DIV_SHIFT);

      switch (clksrc)
        {
          case PCC_CLKGATE_SRC_FOSCDIV:
          case PCC_CLKGATE_SRC_SIRCDIV:
          case PCC_CLKGATE_SRC_FIRCDIV:
          case PCC_CLKGATE_SRC_PLL0DIV:
          case PCC_CLKGATE_SRC_PLL1DIV:
            freq = fc7300_get_pccsrcfreq(clksrc, clockname);
            break;

          default:
            freq = 0;
            break;
        }
    }

  if (freq == 0)
    {
      ret = -ENODEV;
    }

  if (frequency != NULL)
    {
      *frequency = freq / (pccdivider + 1);
    }

  return ret;
}
