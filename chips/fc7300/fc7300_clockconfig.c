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
#include <assert.h>
#include <debug.h>

#include <nuttx/arch.h>
#include <nuttx/power/pm.h>

#include "arm_internal.h"
#include "hardware/fc7300_scg.h"
#include "hardware/fc7300_scm.h"
#include "hardware/fc7300_csc.h"
#include "fc7300_periphclocks.h"
#include "fc7300_clockconfig.h"
#include "fc7300_start.h"

#include <arch/board/board.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#define SCG_PLL0DIV_DIV_ACK_MASK \
        (SCG_PLL0DIV_DIVL_ACK_MASK | SCG_PLL0DIV_DIVM_ACK_MASK | \
         SCG_PLL0DIV_DIVH_ACK_MASK)

#define SCG_PLL1DIV_DIV_ACK_MASK \
        (SCG_PLL1DIV_DIVL_ACK_MASK | SCG_PLL1DIV_DIVM_ACK_MASK | \
         SCG_PLL1DIV_DIVH_ACK_MASK)

#define SCG_SIRCDIV_DIV_ACK_MASK \
        (SCG_SIRCDIV_DIVL_ACK_MASK | SCG_SIRCDIV_DIVM_ACK_MASK | \
         SCG_SIRCDIV_DIVH_ACK_MASK)

#define SCG_FIRCDIV_DIV_ACK_MASK \
        (SCG_FIRCDIV_DIVL_ACK_MASK | SCG_FIRCDIV_DIVM_ACK_MASK | \
         SCG_FIRCDIV_DIVH_ACK_MASK)

#define SCG_FOSCDIV_DIV_ACK_MASK \
        (SCG_FOSCDIV_DIVL_ACK_MASK | SCG_FOSCDIV_DIVM_ACK_MASK | \
         SCG_FOSCDIV_DIVH_ACK_MASK)

#define SCG_FIRCDIV_DIV_EN_MASK \
        (SCG_FIRCDIV_DIVL_EN_MASK | SCG_FIRCDIV_DIVM_EN_MASK | \
         SCG_FIRCDIV_DIVH_EN_MASK)

#define SCG_SIRCDIV_DIV_EN_MASK \
        (SCG_SIRCDIV_DIVL_EN_MASK | SCG_SIRCDIV_DIVM_EN_MASK | \
         SCG_SIRCDIV_DIVH_EN_MASK)

#define SCG_FOSCDIV_DIV_EN_MASK \
        (SCG_FOSCDIV_DIVL_EN_MASK | SCG_FOSCDIV_DIVM_EN_MASK | \
         SCG_FOSCDIV_DIVH_EN_MASK)

/****************************************************************************
 * Private Function Declarations
 ****************************************************************************/

/****************************************************************************
 * Private Types
 ****************************************************************************/

/****************************************************************************
 * Private Data
 ****************************************************************************/

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: fc7300_scg_getfoscvalid
 *
 * Description:
 *    Get FOSC valid status, use this status to check FOSC stable status.
 *
 * Input Parameters:
 *   None
 *
 * Returned Value:
 *    bool. true as valid; false as invalid
 *
 ****************************************************************************/

static inline bool fc7300_scg_getfoscvalid(void)
{
  return ((SCG_FOSCCSR_VLD_MASK == (getreg32(FC7300_SCG_FOSCCSR) &
          SCG_FOSCCSR_VLD_MASK)) ? true : false);
}

/****************************************************************************
 * Name: fc7300_scg_diablefosc
 *
 * Description:
 *   Disable SOSC.
 *
 * Input Parameters:
 *   None.
 *
 * Returned Value:
 *   None.
 *
 ****************************************************************************/

static void fc7300_scg_diablefosc(void)
{
  uint32_t regval;

  regval = getreg32(FC7300_SCG_FOSCCSR);
  regval &= ~SCG_FOSCCSR_EN_MASK;
  putreg32(regval, FC7300_SCG_FOSCCSR);
}

/****************************************************************************
 * Name: fc7300_scg_getpll0valid
 *
 * Description:
 *   Check PLL0 valid.
 *
 * Input Parameters:
 *   None.
 *
 * Returned Value:
 *   If PLL0 is valid, return true, else return false.
 *
 ****************************************************************************/

static inline bool fc7300_scg_getpll0valid(void)
{
  return ((SCG_PLL0CSR_VLD_MASK == (getreg32(FC7300_SCG_PLL0CSR) &
          SCG_PLL0CSR_VLD_MASK)) ? true : false);
}

/****************************************************************************
 * Name: fc7300_scg_getpll1valid
 *
 * Description:
 *   Check PLL1 valid.
 *
 * Input Parameters:
 *   None.
 *
 * Returned Value:
 *   If PLL1 is valid, return true, else return false.
 *
 ****************************************************************************/

static inline bool fc7300_scg_getpll1valid(void)
{
  return ((SCG_PLL1CSR_VLD_MASK == (getreg32(FC7300_SCG_PLL1CSR) &
          SCG_PLL1CSR_VLD_MASK)) ? true : false);
}

/****************************************************************************
 * Name: fc7300_scg_disablepll0
 *
 * Description:
 *   Gets SCG Fast IRC clock frequency (FIRC).
 *
 * Input Parameters:
 *   None.
 *
 * Returned Value:
 *   None.
 *
 ****************************************************************************/

static void fc7300_scg_disablepll0(void)
{
  uint32_t regval;

  regval = getreg32(FC7300_SCG_PLL0CSR);
  regval &= ~SCG_PLL0CSR_EN_MASK;
  putreg32(regval, FC7300_SCG_PLL0CSR);
}

/****************************************************************************
 * Name: fc7300_scg_disablepll1
 *
 * Description:
 *   Gets SCG Fast IRC clock frequency (FIRC).
 *
 * Input Parameters:
 *   None.
 *
 * Returned Value:
 *   None.
 *
 ****************************************************************************/

static void fc7300_scg_disablepll1(void)
{
  uint32_t regval;

  regval = getreg32(FC7300_SCG_PLL1CSR);
  regval &= ~SCG_PLL1CSR_EN_MASK;
  putreg32(regval, FC7300_SCG_PLL1CSR);
}

/****************************************************************************
 * Name: fc7300_scg_getfircdivack
 *
 * Description:
 *   Check FIRC DIVH/M/L ACK to 1/0.
 *
 * Input Parameters:
 *   None.
 *
 * Returned Value:
 *   True as DIVM function clock has been enabled,
 *   FALSE as not enabled.
 *
 ****************************************************************************/

static inline bool fc7300_scg_getfircdivack(bool valid)
{
  if (valid)
    {
      return ((SCG_FIRCDIV_DIV_ACK_MASK == (getreg32(FC7300_SCG_FIRCDIV) &
               SCG_FIRCDIV_DIV_ACK_MASK)) ? true : false);
    }
  else
    {
      return ((0 == (getreg32(FC7300_SCG_FIRCDIV) &
               SCG_FIRCDIV_DIV_ACK_MASK)) ? true : false);
    }
}

/****************************************************************************
 * Name: fc7300_scg_getsircdivack
 *
 * Description:
 *   Check SIRC DIVH/M/L ACK to 1/0.
 *
 * Input Parameters:
 *   None.
 *
 * Returned Value:
 *   Get whether SIRC DIV ACK.
 *
 ****************************************************************************/

static inline bool fc7300_scg_getsircdivack(bool valid)
{
  if (valid)
    {
      return ((SCG_SIRCDIV_DIV_ACK_MASK == (getreg32(FC7300_SCG_SIRCDIV) &
               SCG_SIRCDIV_DIV_ACK_MASK)) ? true : false);
    }
  else
    {
      return ((0U == (getreg32(FC7300_SCG_SIRCDIV) & SCG_SIRCDIV_DIV_ACK_MASK))
               ? true : false);
    }
}

/****************************************************************************
 * Name: fc7300_scg_getfoscdivack
 *
 * Description:
 *   Check fosc DIVH/M/L ACK to 1/0.
 *
 * Input Parameters:
 *   None.
 *
 * Returned Value:
 *   True as DIVM function clock has been enabled,
 *   FALSE as not enabled.
 *
 ****************************************************************************/

static inline bool fc7300_scg_getfoscdivack(bool valid)
{
  if (valid)
    {
      return ((SCG_FOSCDIV_DIV_ACK_MASK == (getreg32(FC7300_SCG_FOSCDIV) &
               SCG_FOSCDIV_DIV_ACK_MASK)) ? true : false);
    }
  else
    {
      return ((0 == (getreg32(FC7300_SCG_FOSCDIV) &
               SCG_FOSCDIV_DIV_ACK_MASK)) ? true : false);
    }
}

/****************************************************************************
 * Name: fc7300_scg_getpll0divack
 *
 * Description:
 *   Check PLL0 DIVH/M/L ACK to 1/0.
 *
 * Input Parameters:
 *   None.
 *
 * Returned Values:
 *   Get whether PLL0 DIV ACK.
 *
 ****************************************************************************/

static inline bool fc7300_scg_getpll0divack(bool valid)
{
  if (valid)
    {
      return ((SCG_PLL0DIV_DIV_ACK_MASK == (getreg32(FC7300_SCG_PLL0DIV) &
               SCG_PLL0DIV_DIV_ACK_MASK)) ? true : false);
    }
  else
    {
      return ((0 == (getreg32(FC7300_SCG_PLL0DIV) &
               SCG_PLL0DIV_DIV_ACK_MASK)) ? true : false);
    }
}

/****************************************************************************
 * Name: fc7300_scg_getpll1divack
 *
 * Description:
 *   Check PLL1 DIVH/M/L ACK to 1/0.
 *
 * Input Parameters:
 *   None.
 *
 * Returned Values:
 *   Get whether PLL1 DIV ACK.
 *
 ****************************************************************************/

static inline bool fc7300_scg_getpll1divack(bool valid)
{
  if (valid)
    {
      return ((SCG_PLL1DIV_DIV_ACK_MASK == (getreg32(FC7300_SCG_PLL1DIV) &
               SCG_PLL1DIV_DIV_ACK_MASK)) ? true : false);
    }
  else
    {
      return ((0 == (getreg32(FC7300_SCG_PLL1DIV) &
               SCG_PLL1DIV_DIV_ACK_MASK)) ? true : false);
    }
}

/****************************************************************************
 * Name: fc7300_scg_waitpll0clockstable
 *
 * Description:
 *   Wait PLL0 clock stable.
 *
 * Input Parameters:
 *   stablestatus - Wait PLL0 clock valid stable or
 *   wait PLL0 clock invalid stable
 *
 * Returned Value:
 *   PLL0 status.
 *
 ****************************************************************************/

static int fc7300_scg_waitpll0clockstable(bool stablestatus)
{
  uint32_t temp;

  /* Check PLL0 valid */

  temp = PLL_STABILIZATION_TIMEOUT;
  while ((fc7300_scg_getpll0valid() != stablestatus) && (temp > 0U))
    {
      temp--;
    }

  return temp ? OK : -ETIMEDOUT;
}

/****************************************************************************
 * Name: fc7300_scg_waitpll1clockstable
 *
 * Description:
 *   Wait PLL1 clock stable.
 *
 * Input Parameters:
 *   stablestatus - Wait PLL1 clock valid stable or
 *   wait PLL1 clock invalid stable
 *
 * Returned Value:
 *   PLL1 status.
 *
 ****************************************************************************/

static int fc7300_scg_waitpll1clockstable(bool stablestatus)
{
  uint32_t temp;

  /* Check PLL1 valid */

  temp = PLL_STABILIZATION_TIMEOUT;
  while ((fc7300_scg_getpll1valid() != stablestatus) && (temp > 0U))
    {
      temp--;
    }

  return temp ? OK : -ETIMEDOUT;
}

/****************************************************************************
 * Name: fc7300_scg_getsoscvalid
 *
 * Description:
 *   Check SOSC valid.
 *
 * Input Parameters:
 *   None.
 *
 * Returned Value:
 *   If SOSC is valid, return TRUE, else return FALSE.
 *
 ****************************************************************************/

static inline bool fc7300_scg_getsoscvalid(void)
{
  return ((SCG_SOSCCSR_VLD_MASK == (getreg32(FC7300_SCG_SOSCCSR) &
          SCG_SOSCCSR_VLD_MASK)) ? true : false);
}

/****************************************************************************
 * Name: fc7300_scg_waitsoscclockstable
 *
 * Description:
 *   Wait SOSC clock stable.
 *
 * Input Parameters:
 *   stablestatus - Wait SOSC clock valid stable or
 *                   wait SOSC clock invalid stable.
 *
 * Returned Value:
 *   SOSC status.
 *
 ****************************************************************************/

static int fc7300_scg_waitsoscclockstable(bool stablestatus)
{
  uint32_t temp;

  /* Check SOSC valid */

  temp = SOSC_STABILIZATION_TIMEOUT;
  while ((fc7300_scg_getsoscvalid() != stablestatus) && (temp > 0U))
    {
      temp--;
    }

  return temp ? OK : -ETIMEDOUT;
}

/****************************************************************************
 * Name: fc7300_scg_waitfoscclockstable
 *
 * Description:
 *   Wait FOSC clock stable.
 *
 * Input Parameters:
 *   None.
 *
 * Returned Value:
 *   FOSC status.
 *
 ****************************************************************************/

static int fc7300_scg_waitfoscclockstable(void)
{
  uint32_t temp;

  temp = CLOCK_DIV_STABILIZATION_TIMEOUT;
  while (false == (fc7300_scg_getfoscdivack(true)) && (temp > 0UL))
    {
      temp--;
    }

  return temp ? OK : -ETIMEDOUT;
}

/****************************************************************************
 * Name: fc7300_scg_getsysclocksrc
 *
 * Description:
 *   Get system clock source.
 *
 * Input Parameters:
 *   None.
 *
 * Returned Value:
 *   System clock source type.
 *
 ****************************************************************************/

static inline uint8_t fc7300_scg_getsysclocksrc(void)
{
  return ((getreg32(FC7300_SCG_CSR) & SCG_CSR_SCS_MASK) \
           >> SCG_CSR_SCS_SHIFT);
}

/****************************************************************************
 * Name: fc7300_scg_getfircvalid
 *
 * Description:
 *   Check FIRC valid.
 *
 * Input Parameters:
 *   None.
 *
 * Returned Value:
 *   If FIRC is valid, return TRUE, else return FALSE.
 *
 ****************************************************************************/

static inline bool fc7300_scg_getfircvalid(void)
{
  return ((SCG_FIRCCSR_VLD_MASK == (getreg32(FC7300_SCG_FIRCCSR) &
          SCG_FIRCCSR_VLD_MASK)) ? true : false);
}

/****************************************************************************
 * Name: fc7300_foscdivset
 *
 * Description:
 *   Set FOSC div values in register bit order.
 *
 * Input Parameters:
 *   divh, divm, divl - value to set.
 *
 * Returned Value:
 *   Values in register bit order.
 *
 ****************************************************************************/

static inline uint32_t fc7300_foscdivset(enum scg_asyncclockdivtype_e divh,
                                         enum scg_asyncclockdivtype_e divm,
                                         enum scg_asyncclockdivtype_e divl)
{
  return ((((uint32_t)divh << SCG_FOSCDIV_DIVH_SHIFT) &
                              SCG_FOSCDIV_DIVH_MASK) |
          (((uint32_t)divm << SCG_FOSCDIV_DIVM_SHIFT) &
                              SCG_FOSCDIV_DIVM_MASK) |
          (((uint32_t)divl << SCG_FOSCDIV_DIVL_SHIFT) &
                              SCG_FOSCDIV_DIVL_MASK));
}

/****************************************************************************
 * Name: fc7300_pll0divset
 *
 * Description:
 *   Set PLL0 div values in register bit order.
 *
 * Input Parameters:
 *   divh, divm, divl - value to set.
 *
 * Returned Value:
 *   Values in register bit order.
 *
 ****************************************************************************/

static inline uint32_t fc7300_pll0divset(enum scg_asyncclockdivtype_e divh,
                                         enum scg_asyncclockdivtype_e divm,
                                         enum scg_asyncclockdivtype_e divl)
{
  return ((((uint32_t)divh << SCG_PLL0DIV_DIVH_SHIFT) &
                              SCG_PLL0DIV_DIVH_MASK) |
          (((uint32_t)divm << SCG_PLL0DIV_DIVM_SHIFT) &
                              SCG_PLL0DIV_DIVM_MASK) |
          (((uint32_t)divl << SCG_PLL0DIV_DIVL_SHIFT) &
                              SCG_PLL0DIV_DIVL_MASK));
}

/****************************************************************************
 * Name: fc7300_pll1divset
 *
 * Description:
 *   Set PLL1 div values in register bit order.
 *
 * Input Parameters:
 *   divh, divm, divl - value to set.
 *
 * Returned Value:
 *   Values in register bit order.
 *
 ****************************************************************************/

static inline uint32_t fc7300_pll1divset(enum scg_asyncclockdivtype_e divh,
                                         enum scg_asyncclockdivtype_e divm,
                                         enum scg_asyncclockdivtype_e divl)
{
  return ((((uint32_t)divh << SCG_PLL1DIV_DIVH_SHIFT) &
                              SCG_PLL1DIV_DIVH_MASK) |
          (((uint32_t)divm << SCG_PLL1DIV_DIVM_SHIFT) &
                              SCG_PLL1DIV_DIVM_MASK) |
          (((uint32_t)divl << SCG_PLL1DIV_DIVL_SHIFT) &
                              SCG_PLL1DIV_DIVL_MASK));
}

/****************************************************************************
 * Name: fc7300_scg_waitfircclockstable
 *
 * Description:
 *   Wait FIRC clock stable.
 *
 * Input Parameters:
 *   stablestatus - Wait FIRC clock valid stable or
 *                   wait FIRC clock invalid stable.
 *
 * Returned Value:
 *   FIRC status.
 *
 ****************************************************************************/

static int fc7300_scg_waitfircclockstable(bool stablestatus)
{
  uint32_t temp;

  /* Check FIRC valid */

  temp = FIRC_STABILIZATION_TIMEOUT;
  while ((fc7300_scg_getfircvalid() != stablestatus) && (temp > 0U))
    {
      temp--;
    }

  return temp ? OK : -ETIMEDOUT;
}

/****************************************************************************
 * Name: fc7300_scg_switchsystemclock
 *
 * Description:
 *    Switch system clock.
 *
 * Input Parameters:
 *    u8clk - clock source to be switched to.
 *
 * Returned Value:
 *    None.
 *
 ****************************************************************************/

static void fc7300_scg_switchsystemclock(uint8_t u8clk)
{
  uint32_t regval;
  regval = getreg32(FC7300_SCG_CCR);
  regval = ((regval & (~SCG_CCR_SCS_MASK)) | SCG_CCR_SCS(u8clk));
  putreg32(regval, FC7300_SCG_CCR);
}

/****************************************************************************
 * Name: fc7300_scg_getsysclkuprd
 *
 * Description:
 *   Get system clock valid status,
 *   use this status to check system clock update finished or not.
 *
 * Input Parameters:
 *   None.
 *
 * Returned Value:
 *   boolean. TRUE as updated; FALSE as not updated.
 *
 ****************************************************************************/

static inline bool fc7300_scg_getsysclkuprd(void)
{
  return (((getreg32(FC7300_SCG_CSR) & SCG_CSR_CCR_UPRD_MASK) != 0U)
          ? true : false);
}

/****************************************************************************
 * Name: fc7300_scg_switchclkctrlsrc
 *
 * Description:
 *   Switch system clock source during run time.
 *
 * Input Parameters:
 *   clock - target clock source user want to switch.
 *
 * Returned Value:
 *    Switch clock source status.
 *    This function check if the clock source is in proper value.
 *
 ****************************************************************************/

static int fc7300_scg_switchclkctrlsrc(enum scg_clocksrctype_e clock)
{
  int ret = OK;
  uint32_t temp;

  switch (clock)
    {
      case SCG_CLOCK_SRC_FOSC:
        if (false == fc7300_scg_getfoscvalid())
          {
            ret = -EINVAL;
          }
        break;
      case SCG_CLOCK_SRC_FIRC:
        if (false == fc7300_scg_getfircvalid())
          {
            ret = -EINVAL;
          }
        break;
      case SCG_CLOCK_SRC_PLL0:
        if (false == fc7300_scg_getpll0valid())
          {
            ret = -EINVAL;
          }
        break;
      case SCG_CLOCK_SRC_NONE:
        ret = -EINVAL;
        break;
      default:
        ret = -EINVAL;
        break;
    }

  if (OK == ret)
    {
      /* Switch system clock */

      fc7300_scg_switchsystemclock((uint8_t)clock);
      temp = SCG_CLKSRC_STABILIZATION_TIMEOUT;
      while ((false == fc7300_scg_getsysclkuprd()) && (temp > 0U))
        {
          temp--;
        }

      /* Time out, clock select failed */

      if (0U == temp)
        {
          ret = -ETIMEDOUT;
        }
      else if (fc7300_scg_getsysclocksrc() != (uint8_t)clock)
        {
          ret = -EINVAL;
        }
    }

  return ret;
}

/****************************************************************************
 * Name: fc7300_firc_config
 *
 * Description:
 *   Configures FIRC module based on provided configuration.
 *
 * Input Parameters:
 *   fircconfig - Describes the desired FORC configuration.
 *
 * Returned Value:
 *   Zero (OK) is returned a success;  A negated errno value is returned on
 *   any failure.
 *
 ****************************************************************************/

static int fc7300_firc_config(const struct scg_firc_config_s *fircconfig)
{
  uint32_t regval;
  uint32_t temp;
  int ret = OK;
  uint16_t trimdiv = 0;

  DEBUGASSERT(fircconfig != NULL);

  /* Unlock FIRC CSR register */

  regval = getreg32(FC7300_SCG_FIRCCSR);
  regval &= ~SCG_FIRCCSR_LK_MASK;
  putreg32(regval, FC7300_SCG_FIRCCSR);

  /* Set FIRCCSR */

  if (fircconfig->enable)
    {
      /* Configure FIRCCFG register */

      putreg32(SCG_FIRCCFG_CLKEN(3U), FC7300_SCG_FIRCCFG);

      /* configure TRUP and EN together with TREN setting */

      regval = getreg32(FC7300_SCG_FIRCCSR);
      regval &= ~(SCG_FIRCCSR_STEN_MASK | SCG_FIRCCSR_TREN_MASK |
                  SCG_FIRCCSR_TRUP_MASK | SCG_FIRCCSR_CM_MASK);
      regval |= (SCG_FIRCCSR_TRUP(fircconfig->tren) |
                 SCG_FIRCCSR_TREN(fircconfig->tren) |
                 SCG_FIRCCSR_STEN(fircconfig->sten));

      putreg32(regval, FC7300_SCG_FIRCCSR);

      /* Set CSR[EN] bit to 1 */

      regval = getreg32(FC7300_SCG_FIRCCSR);
      regval |= (uint32_t)SCG_FIRCCSR_EN_MASK;
      putreg32(regval, FC7300_SCG_FIRCCSR);

      /* Wait FIRC clock stable */

      ret = fc7300_scg_waitfircclockstable(true);

      if (ret == OK)
        {
          regval = getreg32(FC7300_SCG_FIRCCSR);
          regval |= SCG_FIRCCSR_CM(fircconfig->cm);
          putreg32(regval, FC7300_SCG_FIRCCSR);

          regval = getreg32(FC7300_SCG_FIRCDIV);
          regval &= ~(SCG_FIRCDIV_DIVL_EN_MASK | SCG_FIRCDIV_DIVM_EN_MASK |
                      SCG_FIRCDIV_DIVH_EN_MASK);
          putreg32(regval, FC7300_SCG_FIRCDIV);

          temp = CLOCK_DIV_STABILIZATION_TIMEOUT;
          while ((false == fc7300_scg_getfircdivack(false)) && (temp > 0))
            {
              temp--;
            }

          if (0UL == temp)
            {
              return -ETIMEDOUT;
            }

          /* Configure FIRC DIV register */

          regval = (SCG_FIRCDIV_DIVH(fircconfig->divh) |
                    SCG_FIRCDIV_DIVM(fircconfig->divm) |
                    SCG_FIRCDIV_DIVL(fircconfig->divl));

          putreg32(regval, FC7300_SCG_FIRCDIV);

          /* Enable FOSC DIV clocks */

          regval = getreg32(FC7300_SCG_FIRCDIV);
          regval |= SCG_FIRCDIV_DIV_EN_MASK;
          putreg32(regval, FC7300_SCG_FIRCDIV);

          temp = CLOCK_DIV_STABILIZATION_TIMEOUT;
          while ((false == fc7300_scg_getfircdivack(true)) && (temp > 0))
            {
              temp--;
            }

          if (0UL == temp)
            {
              ret = -ETIMEDOUT;
            }

          if (fircconfig->tren == true)
            {
              if (fircconfig->trimsrc == SCG_IRC_TRIMSRC_FOSC)
                {
                  /* Trim clock source choose FOSC */

                  trimdiv = (uint16_t)(FOSC_CLOCK_DFT / 250000U - 1U);
                }
              else if (fircconfig->trimsrc == SCG_IRC_TRIMSRC_SOSC)
                {
                  /* Trim clock source choose SOSC */

                  trimdiv = 0U;
                }
              else
                {
                  /* This shall never be reached */

                  trimdiv = 0U;
                }

              /* FIRC configuration */

              regval = (SCG_FIRCTCFG_TRIMSRC(fircconfig->trimsrc) |
                        SCG_FIRCTCFG_TRIMDIV(trimdiv));

              putreg32(regval, FC7300_SCG_FIRCTCFG);
            }
        }
    }
  else
    {
      putreg32(0U, FC7300_SCG_FIRCCSR);

      regval = getreg32(FC7300_SCG_FIRCDIV);
      regval &= ~(SCG_FIRCDIV_DIVL_EN_MASK | SCG_FIRCDIV_DIVM_EN_MASK |
                  SCG_FIRCDIV_DIVH_EN_MASK);
      putreg32(regval, FC7300_SCG_FIRCDIV);

      temp = CLOCK_DIV_STABILIZATION_TIMEOUT;
      while ((false == fc7300_scg_getfircdivack(false)) && (temp > 0))
        {
          temp--;
        }

      if (0UL == temp)
        {
          return -ETIMEDOUT;
        }
    }

  /* Lock FIRC CSR register */

  regval = getreg32(FC7300_SCG_FIRCCSR);
  regval |= (SCG_FIRCCSR_LK(fircconfig->lock));
  putreg32(regval, FC7300_SCG_FIRCCSR);

  return ret;
}

/****************************************************************************
 * Name: fc7300_sirc_config
 *
 * Description:
 *   Configures SIRC module based on provided configuration.
 *
 * Input Parameters:
 *   sirccfg - Describes the desired SIRC configuration.
 *
 * Returned Value:
 *   Zero (OK) is returned a success;  A negated errno value is returned on
 *   any failure.
 *
 ****************************************************************************/

static int fc7300_sirc_config(const struct scg_sirc_config_s *sircconfig)
{
  uint32_t regval;
  uint32_t temp;
  uint16_t trimdiv = 0;
  int ret = OK;

  DEBUGASSERT(sircconfig != NULL);

  /* Unlock SIRC CSR register */

  regval = getreg32(FC7300_SCG_SIRCCSR);
  regval &= ~SCG_SIRCCSR_LK_MASK;
  putreg32(regval, FC7300_SCG_SIRCCSR);

  /* Configure CSR register */

  regval = getreg32(FC7300_SCG_SIRCCSR);

  regval &= ~(SCG_SIRCCSR_CM_MASK | SCG_SIRCCSR_TRUP_MASK |
              SCG_SIRCCSR_TREN_MASK | SCG_SIRCCSR_LPEN_MASK |
              SCG_SIRCCSR_STEN_MASK);

  regval |= (SCG_SIRCCSR_TRUP(sircconfig->tren) |
             SCG_SIRCCSR_TREN(sircconfig->tren) |
             SCG_SIRCCSR_LPEN(sircconfig->lpen) |
             SCG_SIRCCSR_STEN(sircconfig->sten));

  putreg32(regval, FC7300_SCG_SIRCCSR);

  /* diable sirc div[en] */

  regval = getreg32(FC7300_SCG_SIRCDIV);

  regval &= ~(SCG_SIRCDIV_DIVL_EN_MASK | SCG_SIRCDIV_DIVM_EN_MASK |
              SCG_SIRCDIV_DIVH_EN_MASK);

  putreg32(regval, FC7300_SCG_SIRCDIV);

  temp = CLOCK_DIV_STABILIZATION_TIMEOUT;
  while ((false == fc7300_scg_getsircdivack(false)) && (temp > 0U))
    {
      temp--;
    }

  if (temp == 0U)
      return -ETIMEDOUT;

  regval = getreg32(FC7300_SCG_SIRCCSR);
  regval |= SCG_SIRCCSR_CM(sircconfig->cm);
  putreg32(regval, FC7300_SCG_SIRCCSR);

  /* set sirc div */

  regval = (SCG_SIRCDIV_DIVH(sircconfig->divh) |
            SCG_SIRCDIV_DIVM(sircconfig->divm) |
            SCG_SIRCDIV_DIVL(sircconfig->divl));

  putreg32(regval, FC7300_SCG_SIRCDIV);

  /* enable sirc div */

  regval = getreg32(FC7300_SCG_SIRCDIV);
  regval |= SCG_SIRCDIV_DIV_EN_MASK;
  putreg32(regval, FC7300_SCG_SIRCDIV);

  temp = CLOCK_DIV_STABILIZATION_TIMEOUT;
  while ((false == fc7300_scg_getsircdivack(true)) && (temp > 0U))
    {
      temp--;
    }

  if (temp == 0U)
    {
      /* timeout, clear sirc div register */

      putreg32(0x0, FC7300_SCG_SIRCDIV);
      return -ETIMEDOUT;
    }

  if (sircconfig->tren == true)
    {
      if (sircconfig->trimsrc == SCG_IRC_TRIMSRC_FOSC)
        {
          /* Trim clock source choose FOSC */

          trimdiv = (uint16_t)(FOSC_CLOCK_DFT / 250000U - 1U);
        }
      else if (sircconfig->trimsrc == SCG_IRC_TRIMSRC_SOSC)
        {
          /* Trim clock source choose SOSC */

          trimdiv = 0U;
        }
      else
        {
          /* This shall never be reached */

          trimdiv = 0U;
        }

      /* SIRC configuration */

      regval = (SCG_SIRCTCFG_TRIMSRC(sircconfig->trimsrc) |
                SCG_SIRCTCFG_TRIMDIV(trimdiv));

      putreg32(regval, FC7300_SCG_SIRCTCFG);
    }

  /* Lock SIRC CSR register */

  regval = getreg32(FC7300_SCG_SIRCCSR);
  regval |= SCG_SIRCCSR_LK(sircconfig->lock);
  putreg32(regval, FC7300_SCG_SIRCCSR);

  return ret;
}

/****************************************************************************
 * Name: fc7300_sosc_config
 *
 * Description:
 *   Configure to the SOSC clock source.
 *
 * Input Parameters:
 *   soscconfig - Describes the desired SOSC configuration.
 *
 * Returned Value:
 *   Zero (OK) is returned a success;  A negated errno value is returned on
 *   any failure.
 *
 ****************************************************************************/

static int fc7300_sosc_config(const struct scg_sosc_config_s *soscconfig)
{
  uint32_t regval;
  int ret = OK;

  DEBUGASSERT(soscconfig != NULL);

  /* Unlock SOSC CSR register */

  regval = getreg32(FC7300_SCG_SOSCCSR);
  regval &= ~SCG_SOSCCSR_LK_MASK;
  putreg32(regval, FC7300_SCG_SOSCCSR);

  /* Set SOSC */

  if (soscconfig->enable == true)
    {
      /* Set recommend value to SOSCCFG register */

      regval = (SCG_SOSCCFG_EOCV(64U) | SCG_SOSCCFG_GM_SEL(3U) |
                SCG_SOSCCFG_CURPRG_SF(3U) | SCG_SOSCCFG_CURPRG_COMP(3U));

      putreg32(regval, FC7300_SCG_SOSCCFG);

      /* Configure SOSC CSR register */

      regval = getreg32(FC7300_SCG_SOSCCSR);
      regval &= ~(SCG_SOSCCSR_BYPASS_MASK | SCG_SOSCCSR_CM_MASK | \
                  SCG_SOSCCSR_CMRE_MASK);
      regval |= SCG_SOSCCSR_BYPASS(soscconfig->bypass);

      putreg32(regval, FC7300_SCG_SOSCCSR);

      /* Enable SOSC */

      regval = getreg32(FC7300_SCG_SOSCCSR);
      regval |= SCG_SOSCCSR_EN(1U);

      putreg32(regval, FC7300_SCG_SOSCCSR);

      ret = fc7300_scg_waitsoscclockstable(true);
      if (-ETIMEDOUT == ret)
        {
          /* Disable SOSC */

          regval = getreg32(FC7300_SCG_SOSCCSR);
          regval &= ~SCG_SOSCCSR_EN_MASK;
          putreg32(regval, FC7300_SCG_SOSCCSR);
        }
      else
        {
          /* Enable SOSC clock monitor */

          regval = getreg32(FC7300_SCG_SOSCCSR);
          regval |= SCG_SOSCCSR_CM(soscconfig->cm);
          putreg32(regval, FC7300_SCG_SOSCCSR);

          /* Reset SOSC clock monitor */

          regval = getreg32(FC7300_SCG_SOSCCSR);
          regval |= SCG_SOSCCSR_CMRE(soscconfig->cmre);
          putreg32(regval, FC7300_SCG_SOSCCSR);

          /* Lock SOSC CSR register */

          regval = getreg32(FC7300_SCG_SOSCCSR);
          regval |= SCG_SOSCCSR_LK(soscconfig->lock);
          putreg32(regval, FC7300_SCG_SOSCCSR);
        }
    }
  else
    {
      regval = getreg32(FC7300_SCG_SOSCCSR);
      regval &= ~SCG_SOSCCSR_EN_MASK;
      putreg32(regval, FC7300_SCG_SOSCCSR);
    }

  return ret;
}

/****************************************************************************
 * Name: fc7300_fosc_config
 *
 * Description:
 *   CConfigures FOSC module based on provided configuration.
 *
 * Input Parameters:
 *   foscconfig - Describes the desired FOSC configuration.
 *
 * Returned Value:
 *   Zero (OK) is returned a success;  A negated errno value is returned on
 *   any failure.
 *
 ****************************************************************************/

static int fc7300_fosc_config(const struct scg_fosc_config_s *foscconfig)
{
  uint32_t regval;
  uint32_t temp = 0U;
  uint8_t msbval;
  bool comp_en = false;
  int ret = OK;

  DEBUGASSERT(foscconfig != NULL);

  if (foscconfig->enable)
    {
      comp_en = foscconfig->bypass ? false : true;

      /* Unlock FOSC CSR register */

      regval = getreg32(FC7300_SCG_FOSCCSR);
      regval &= ~SCG_FOSCCSR_LK_MASK;
      putreg32(regval, FC7300_SCG_FOSCCSR);

      /* COMP_EN must be 1 when using an external crystal and
       * configure GM to the max value (GM_SEL: 15U)
       */

      regval = SCG_FOSCCFG_BYPASS(foscconfig->bypass) |
               SCG_FOSCCFG_COMP_EN(comp_en) | SCG_FOSCCFG_EOCV(50U) |
               SCG_FOSCCFG_GM_SEL(15U) | SCG_FOSCCFG_ALC_D(1U) |
               SCG_FOSCCFG_HYST_D(0U);

      putreg32(regval, FC7300_SCG_FOSCCFG);

      /* Configure stop enable and enable FOSC */

      regval = getreg32(FC7300_SCG_FOSCCSR);
      regval &= ~(SCG_FOSCCSR_STEN_MASK | SCG_FOSCCSR_CM_MASK | \
                  SCG_FOSCCSR_CMRE_MASK);
      regval |= SCG_FOSCCSR_STEN(foscconfig->sten);
      putreg32(regval, FC7300_SCG_FOSCCSR);

      /* Enable FOSC */

      regval = getreg32(FC7300_SCG_FOSCCSR);
      regval |= SCG_FOSCCSR_EN_MASK;
      putreg32(regval, FC7300_SCG_FOSCCSR);

      /* Wait FOSC valid */

      temp = FOSC_STABILIZATION_TIMEOUT;
      while ((fc7300_scg_getfoscvalid() == false) && (temp > 0U))
        {
          temp--;
        }

      if (temp != 0U)
        {
          regval = getreg32(FC7300_SCG_FOSCCSR);

          /* Configure CM CMRE and lock */

          regval |= SCG_FOSCCSR_CM(foscconfig->cm);
          putreg32(regval, FC7300_SCG_FOSCCSR);

          regval &= ~SCG_FOSCCSR_CMRE_MASK;
          regval |= SCG_FOSCCSR_CMRE(foscconfig->cmre) |
                    SCG_FOSCCSR_LK(foscconfig->lock);

          putreg32(regval, FC7300_SCG_FOSCCSR);

          /* Disable FOSC DIV */

          regval = getreg32(FC7300_SCG_FOSCDIV);

          regval &= ~(SCG_FOSCDIV_DIVL_EN_MASK | SCG_FOSCDIV_DIVM_EN_MASK |
                      SCG_FOSCDIV_DIVH_EN_MASK);
          putreg32(regval, FC7300_SCG_FOSCDIV);

          temp = CLOCK_DIV_STABILIZATION_TIMEOUT;
          while ((false == fc7300_scg_getfoscdivack(false)) && (temp > 0))
            {
              temp--;
            }

          if (0UL == temp)
            {
              return -ETIMEDOUT;
            }

          /* Config FOSC DIV */

          regval = (SCG_FOSCDIV_DIVH(foscconfig->divh) |
                    SCG_FOSCDIV_DIVM(foscconfig->divm) |
                    SCG_FOSCDIV_DIVL(foscconfig->divl));
          putreg32(regval, FC7300_SCG_FOSCDIV);

          /* Enable FOSC Div clocks */

          regval = getreg32(FC7300_SCG_FOSCDIV);
          regval |= SCG_FOSCDIV_DIV_EN_MASK;
          putreg32(regval, FC7300_SCG_FOSCDIV);

          ret = fc7300_scg_waitfoscclockstable();

          /* This is the protection measure during low power wake up,
           * if SCG register not valid after the setting time,
           * the chip will reset and set clock error flag in RGM register
           */

          msbval = ((FOSC_CLOCK_DFT / 8000000U) >= 5U) ? 5U : \
                     ((6U - (FOSC_CLOCK_DFT / 8000000U)) * 5U);
          regval = SCG_WKPWDG_MSB(msbval) | SCG_WKPWDG_EN_MASK;
          putreg32(regval, FC7300_SCG_WKPWDG);
        }
      else
        {
          fc7300_scg_diablefosc();
          ret = -ETIMEDOUT;
        }
    }
  else
    {
      fc7300_scg_diablefosc();
    }

  return ret;
}

/****************************************************************************
 * Name: fc7300_pll0_config
 *
 * Description:
 *    Configures PLL0 module based on provided configuration.
 *
 * Input Parameters:
 *   pll0config - Describes the desired PLL0 configuration.
 *
 * Returned Value:
 *   Zero (OK) is returned a success;  A negated errno value is returned on
 *   any failure.
 *
 ****************************************************************************/

static int fc7300_pll0_config(const struct scg_pll_config_s *pll0config)
{
  uint32_t regval;
  uint32_t temp;
  int ret = OK;

  DEBUGASSERT(pll0config != NULL);

  /* Unlock PLL0 CSR register */

  regval = getreg32(FC7300_SCG_PLL0CSR);
  regval &= ~SCG_PLL0CSR_LK_MASK;
  putreg32(regval, FC7300_SCG_PLL0CSR);

  /* Configure PLL0 */

  if (pll0config->enable)
    {
      if (fc7300_scg_getpll0valid())
        {
          /* Disable PLL0 first */

          fc7300_scg_disablepll0();
          ret = fc7300_scg_waitpll0clockstable(false);
        }

      if (OK == ret)
        {
          /* Configure PLL CFG register */

          regval =  (SCG_PLL0CFG_PREDIV(pll0config->prediv) |
                     SCG_PLL0CFG_MULT(pll0config->mult) |
                     SCG_PLL0CFG_PSTDIV(pll0config->pstdiv) |
                     SCG_PLL0CFG_SOURCE(pll0config->src));

          putreg32(regval, FC7300_SCG_PLL0CFG);

          /* Set CSR[EN] bit to 1 */

          regval = getreg32(FC7300_SCG_PLL0CSR);
          regval |= SCG_PLL0CSR_EN(pll0config->enable);

          putreg32(regval, FC7300_SCG_PLL0CSR);

          /* Wait till PLL0 valid */

          ret = fc7300_scg_waitpll0clockstable(true);

          if (ret == OK)
            {
              /* Disable PLL0 DIV */

              regval = getreg32(FC7300_SCG_PLL0DIV);
              regval &= ~(SCG_PLL0DIV_DIVL_EN_MASK | SCG_PLL0DIV_DIVM_EN_MASK |
                          SCG_PLL0DIV_DIVH_EN_MASK);
              putreg32(regval, FC7300_SCG_PLL0DIV);

              temp = CLOCK_DIV_STABILIZATION_TIMEOUT;
              while ((false == fc7300_scg_getpll0divack(false)) && (temp > 0))
                {
                  temp--;
                }

              if (0UL == temp)
                {
                  return -ETIMEDOUT;
                }

              /* Configure PLL0DIV */

              regval = fc7300_pll0divset(pll0config->divh,
                                         pll0config->divm,
                                         pll0config->divl);

              putreg32(regval, FC7300_SCG_PLL0DIV);

              /* Enable PLL0 div */

              regval = getreg32(FC7300_SCG_PLL0DIV);

              regval |= (SCG_PLL0DIV_DIVL_EN_MASK |
                         SCG_PLL0DIV_DIVM_EN_MASK |
                         SCG_PLL0DIV_DIVH_EN_MASK);

              putreg32(regval, FC7300_SCG_PLL0DIV);

              /* Configure PLL CSR register */

              regval = getreg32(FC7300_SCG_PLL0CSR);
              regval |= (SCG_PLL0CSR_ERR_MASK |
                         SCG_PLL0CSR_CM(pll0config->cm) |
                         SCG_PLL0CSR_CMRE(pll0config->cmre) |
                         SCG_PLL0CSR_STEN(pll0config->sten));

              putreg32(regval, FC7300_SCG_PLL0CSR);

              /* Lock PLL0 CSR register */

              regval = getreg32(FC7300_SCG_PLL0CSR);
              regval |= SCG_PLL0CSR_LK(pll0config->lock);
              putreg32(regval, FC7300_SCG_PLL0CSR);

              temp = CLOCK_DIV_STABILIZATION_TIMEOUT;
              while ((false == fc7300_scg_getpll0divack(true))
                     && (temp > 0U))
                {
                  temp--;
                }

              if (0U == temp)
                {
                  ret = -ETIMEDOUT;
                }
            }
          else
            {
              fc7300_scg_disablepll0();
            }
        }
    }
  else
    {
      fc7300_scg_disablepll0();
    }

  return ret;
}

/****************************************************************************
 * Name: fc7300_pll1_config
 *
 * Description:
 *    Configures PLL1 module based on provided configuration.
 *
 * Input Parameters:
 *   pll0config - Describes the desired PLL1 configuration.
 *
 * Returned Value:
 *   Zero (OK) is returned a success;  A negated errno value is returned on
 *   any failure.
 *
 ****************************************************************************/

static int fc7300_pll1_config(const struct scg_pll_config_s *pll1config)
{
  uint32_t regval;
  uint32_t temp;
  int ret = OK;

  DEBUGASSERT(pll1config != NULL);

  /* Unlock PLL1 CSR register */

  regval = getreg32(FC7300_SCG_PLL1CSR);
  regval &= ~SCG_PLL1CSR_LK_MASK;
  putreg32(regval, FC7300_SCG_PLL1CSR);

  /* Configure PLL1 */

  if (pll1config->enable)
    {
      if (fc7300_scg_getpll1valid())
        {
          /* Disable PLL1 first */

          fc7300_scg_disablepll1();
          ret = fc7300_scg_waitpll1clockstable(false);
        }

      if (OK == ret)
        {
          /* Configure PLL CFG register */

          regval =  (SCG_PLL1CFG_PREDIV(pll1config->prediv) |
                     SCG_PLL1CFG_MULT(pll1config->mult) |
                     SCG_PLL1CFG_PSTDIV(pll1config->pstdiv) |
                     SCG_PLL1CFG_SOURCE(pll1config->src));

          putreg32(regval, FC7300_SCG_PLL1CFG);

          /* Set CSR[EN] bit to 1 */

          regval = getreg32(FC7300_SCG_PLL1CSR);
          regval |= SCG_PLL1CSR_EN(pll1config->enable);

          putreg32(regval, FC7300_SCG_PLL1CSR);

          /* Wait till PLL1 valid */

          ret = fc7300_scg_waitpll1clockstable(true);

          if (ret == OK)
            {
              /* Disable PLL0 DIV */

              regval = getreg32(FC7300_SCG_PLL1DIV);
              regval &= ~(SCG_PLL1DIV_DIVL_EN_MASK | SCG_PLL1DIV_DIVM_EN_MASK |
                          SCG_PLL1DIV_DIVH_EN_MASK);
              putreg32(regval, FC7300_SCG_PLL1DIV);

              temp = CLOCK_DIV_STABILIZATION_TIMEOUT;
              while ((false == fc7300_scg_getpll1divack(false)) && (temp > 0))
                {
                  temp--;
                }

              if (0UL == temp)
                {
                  return -ETIMEDOUT;
                }

              /* Configure PLLDIV */

              regval = fc7300_pll1divset(pll1config->divh,
                                        pll1config->divm,
                                        pll1config->divl);
              putreg32(regval, FC7300_SCG_PLL1DIV);

              /* Enable PLL1 div */

              regval = getreg32(FC7300_SCG_PLL1DIV);

              regval |= (SCG_PLL1DIV_DIVL_EN_MASK |
                         SCG_PLL1DIV_DIVM_EN_MASK |
                         SCG_PLL1DIV_DIVH_EN_MASK);

              putreg32(regval, FC7300_SCG_PLL1DIV);

              /* Configure PLL CSR register */

              regval = getreg32(FC7300_SCG_PLL1CSR);
              regval |= (SCG_PLL1CSR_ERR_MASK |
                         SCG_PLL1CSR_CM(pll1config->cm) |
                         SCG_PLL1CSR_CMRE(pll1config->cmre) |
                         SCG_PLL1CSR_STEN(pll1config->sten));

              putreg32(regval, FC7300_SCG_PLL1CSR);

              /* Lock PLL0 CSR register */

              regval = getreg32(FC7300_SCG_PLL1CSR);
              regval |= SCG_PLL1CSR_LK(pll1config->lock);
              putreg32(regval, FC7300_SCG_PLL1CSR);

              temp = CLOCK_DIV_STABILIZATION_TIMEOUT;
              while ((false == fc7300_scg_getpll1divack(true))
                     && (temp > 0U))
                {
                  temp--;
                }

              if (0U == temp)
                {
                  ret = -ETIMEDOUT;
                }
            }
          else
            {
              fc7300_scg_disablepll1();
            }
        }
    }
  else
    {
      fc7300_scg_disablepll1();
    }

  return ret;
}

/****************************************************************************
 * Name: fc7300_clkctrl_config
 *
 * Description:
 *   Set system related clock source-- FOSC, FIRC, PLL0, PLL1.
 *
 * Input Parameters:
 *   sysclkconfig: pointer to the clockCtrlType structure data instance,
 *   which defined for system clock selection.
 * Returned Value:
 *   System clock status.
 *   This function check the clock source status before set it system clock,
 *   if the chosen clock source is invalid, it would return false.
 *
 ****************************************************************************/

static int fc7300_clkctrl_config(
            const struct scg_clockctrl_config_s *sysclkconfig)
{
  int ret = OK;
  uint32_t regval;
  uint32_t temp;

  switch (sysclkconfig->src)
    {
      case SCG_CLOCK_SRC_FOSC:
        if (false == fc7300_scg_getfoscvalid())
          {
            ret = -EBADR;
          }
        break;
      case SCG_CLOCK_SRC_FIRC:
        if (false == fc7300_scg_getfircvalid())
          {
            ret = -EBADR;
          }
        break;
    case SCG_CLOCK_SRC_PLL0:
        if (false == fc7300_scg_getpll0valid())
          {
            ret = -EBADR;
          }
        break;
    case SCG_CLOCK_SRC_NONE:
        ret = -EBADR;
        break;
    default:
        ret = -EBADR;
        break;
    }

  if (OK == ret)
    {
      /* Configure system control register */

      regval = (SCG_CCR_SYSCLK_CME(sysclkconfig->sysclkmonitor) |
                SCG_CCR_SCS(sysclkconfig->src) |
                SCG_CCR_DIVCORE(sysclkconfig->divcore) |
                SCG_CCR_DIVBUS(sysclkconfig->divbus)  |
                SCG_CCR_DIVSLOW(sysclkconfig->divslow));

      putreg32(regval, FC7300_SCG_CCR);

      temp = SCG_CLKSRC_STABILIZATION_TIMEOUT;
      while ((false == fc7300_scg_getsysclkuprd()) && (temp > 0U))
        {
          temp--;
        }

      /* Time out, clock select failed */

      if (0UL == temp)
        {
          ret = -ETIMEDOUT;
        }
      else if (fc7300_scg_getsysclocksrc() != (uint8_t)sysclkconfig->src)
        {
          ret = -EBADR;
        }
      else
        {
          /* do nothing */
        }
    }

  return ret;
}

/****************************************************************************
 * Name: fc7300_clkout_config
 *
 * Description:
 *   Configures all modules from SCG (SIRC, FIRC, FOSC and PLL0)
 *
 * Input Parameters:
 *   None
 *
 * Returned Value:
 *   Zero (OK) is returned a success;  A negated errno value is returned on
 *   any failure.
 *
 ****************************************************************************/

void fc7300_clkout_config(const struct scg_clockctrl_config_s *sysclkconfig)
{
  uint32_t regval;
  enum scg_clockoutsrctype_e clockoutsrc;
  clockoutsrc = sysclkconfig->clkoursrc;

  regval = getreg32(FC7300_SCG_CLKOUTCFG);
  regval |= SCG_CLKOUTCFG_CLKOUTSEL(clockoutsrc);
  putreg32(regval, FC7300_SCG_CLKOUTCFG);
}

/****************************************************************************
 * Name: fc7300_scg_config
 *
 * Description:
 *   Configure SCG clocking.
 *
 * Input Parameters:
 *   scgcfg - Describes the new SCG clock configuration
 *
 * Returned Value:
 *   Zero (OK) is returned a success;  A negated errno value is returned on
 *   any failure.
 *
 ****************************************************************************/

static int fc7300_scg_config(const struct scg_config_s *scgcfg)
{
  bool bfircset = false;
  int ret = OK;

  struct scg_pll_config_s tpll0cfg =
  {
    .enable = false,
    .lock       = false,
    .cm         = false,
    .cmre       = false,
    .sten       = true,
    .divh       = SCG_ASYNCCLOCKDIV_BY2,
    .divm       = SCG_ASYNCCLOCKDIV_BY2,
    .divl       = SCG_ASYNCCLOCKDIV_BY4,
    .prediv     = 11U,
    .pstdiv     = SCG_PLLPSTDIV_BY2,
    .mult       = 299U,
    .src        = SCG_PLLSOURCE_FOSC,
  };

  struct scg_firc_config_s tfirccfg =
  {
    .enable = true,
    .cm = false,
    .sten = false,
    .tren = false,
    .trimsrc = 0U,
    .divh = SCG_ASYNCCLOCKDIV_BY1,
    .divm = SCG_ASYNCCLOCKDIV_BY1,
    .divl = SCG_ASYNCCLOCKDIV_BY2
  };

  /* Switch to credible clock */

  if ((uint8_t)SCG_CLOCK_SRC_FIRC != fc7300_scg_getsysclocksrc())
    {
      if (scgcfg->firc.enable)
        {
          ret = fc7300_firc_config(&(scgcfg->firc));
          bfircset = true;
        }
      else
        {
          ret = fc7300_firc_config(&tfirccfg);
        }

      if (OK == ret)
        {
          ret = fc7300_scg_switchclkctrlsrc(SCG_CLOCK_SRC_FIRC);
        }
    }

  /* Disable PLL0 first */

  if (ret != OK)
    {
      return ret;
    }

  ret = fc7300_pll0_config(&tpll0cfg);
  if (ret != OK)
    {
      return ret;
    }

  ret = fc7300_sirc_config(&(scgcfg->sirc));
  if (ret != OK)
    {
      return ret;
    }

  ret = fc7300_sosc_config(&(scgcfg->sosc));
  if (ret != OK)
    {
      return ret;
    }

  ret = fc7300_fosc_config(&(scgcfg->fosc));
  if (ret != OK)
    {
      return ret;
    }

  /* Configure PLL0 */

  ret = fc7300_pll0_config(&(scgcfg->pll0));
  if (ret != OK)
    {
      return ret;
    }

  /* Configure PLL1 */

  ret = fc7300_pll1_config(&(scgcfg->pll1));
  if (ret != OK)
    {
      return ret;
    }

  /* Configure system clock */

  ret = fc7300_clkctrl_config(&(scgcfg->clockctrl));

  if (ret != OK)
    {
      return ret;
    }

  /* Configure clock out */

  fc7300_clkout_config(&(scgcfg->clockctrl));

  /* Configure FIRC */

  if (!bfircset)
    {
      ret = fc7300_firc_config(&(scgcfg->firc));
    }

  return ret;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: fc7300_clockconfig
 *
 * Description:
 *   Called to initialize the fc7300 clock.  This does whatever setup is
 *   needed to put the MCU in a usable state.  This includes the
 *   initialization of clocking using the settings in board.h.
 *   This function also performs other low-level chip as necessary.
 *
 * Input Parameters:
 *   clkcfg - Describes the new clock configuration
 *
 * Returned Value:
 *   Zero (OK) is returned a success;  A negated errno value is returned on
 *   any failure.
 *
 ****************************************************************************/

int fc7300_clockconfig(const struct clock_configuration_s *clkcfg)
{
  int ret;

  DEBUGASSERT(clkcfg != NULL);

  /* Set SCG configuration */

  ret = fc7300_scg_config(&clkcfg->scg);

  if (ret >= 0)
    {
      /* Set PCC configuration */

      fc7300_periphclocks(g_peripheral_clocks_num_0, clkcfg->pcc.pclks);
    }

  return ret;
}

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

uint32_t fc7300_get_coreclk(void)
{
  uint32_t coreclk;
  uint32_t regval;
  uint32_t divider;
  uint32_t prediv;
  uint32_t postdiv;
  uint32_t mult;

  /* Get the core clock divider */

  regval  = getreg32(FC7300_SCG_CSR);
  divider = ((regval & SCG_CSR_DIVCORE_MASK) >> SCG_CSR_DIVCORE_SHIFT) + 1;

  /* Coreclock = EXTAL * mult / (2 * prediv) */

  regval  = getreg32(FC7300_SCG_PLL0CFG);
  prediv  = ((regval & SCG_PLL0CFG_PREDIV_MASK) >>
             SCG_PLL0CFG_PREDIV_SHIFT) + 1u;
  postdiv = (1 << ((regval & SCG_PLL0CFG_PSTDIV_MASK) >>
             SCG_PLL0CFG_PSTDIV_SHIFT));
  mult    = ((regval & SCG_PLL0CFG_MULT_MASK) >>
             SCG_PLL0CFG_MULT_SHIFT) + 1u;

  coreclk = ((FOSC_CLOCK_DFT / prediv) * mult) / postdiv;

  return coreclk / divider;
}

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

uint32_t fc7300_get_busclk(void)
{
  uint32_t coreclk;
  uint32_t regval;
  uint32_t divider;

  /* Get the bus clock divider */

  regval  = getreg32(FC7300_SCG_CSR);
  divider = ((regval & SCG_CSR_DIVBUS_MASK) >> SCG_CSR_DIVBUS_SHIFT) + 1;

  /* Get the core clock frequency */

  coreclk = fc7300_get_coreclk();
  return coreclk / divider;
}

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
 *   The frequency of the requested FOSC clock source.
 *
 ****************************************************************************/

uint32_t fc7300_get_foscfreq(uint8_t div)
{
  uint8_t fosc_div;

  switch (div)
    {
      case HAS_DIVIDER_H:
           fosc_div = g_initial_clkconfig.scg.fosc.divh;
           break;
      case HAS_DIVIDER_M:
           fosc_div = g_initial_clkconfig.scg.fosc.divm;
           break;
      case HAS_DIVIDER_L:
           fosc_div = g_initial_clkconfig.scg.fosc.divl;
           break;
      default:
           return 0;
    }

  return (FOSC_CLOCK_DFT >> (fosc_div - 1));
}

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
 *   The frequency of the requested SIRC clock source.
 *
 ****************************************************************************/

uint32_t fc7300_get_sircfreq(uint8_t div)
{
  uint8_t sirc_div;

  switch (div)
    {
      case HAS_DIVIDER_H:
           sirc_div =  g_initial_clkconfig.scg.sirc.divh;
           break;
      case HAS_DIVIDER_M:
           sirc_div =  g_initial_clkconfig.scg.sirc.divm;
           break;
      case HAS_DIVIDER_L:
           sirc_div =  g_initial_clkconfig.scg.sirc.divl;
           break;
      default:
           return 0;
    }

  return (FOSC_CLOCK_DFT >> (sirc_div - 1));
}

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
 *   The frequency of the requested FIRC clock source.
 *
 ****************************************************************************/

uint32_t fc7300_get_fircfreq(uint8_t div)
{
  uint8_t firc_div;

  switch (div)
    {
      case HAS_DIVIDER_H:
           firc_div = g_initial_clkconfig.scg.firc.divh;
           break;
      case HAS_DIVIDER_M:
           firc_div = g_initial_clkconfig.scg.firc.divm;
           break;
      case HAS_DIVIDER_L:
           firc_div = g_initial_clkconfig.scg.firc.divl;
           break;
      default:
         return 0;
    }

  return (FIRC_CLOCK >> (firc_div - 1));
}

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
 *   The frequency of the requested PLL0 clock source.
 *
 ****************************************************************************/

uint32_t fc7300_get_pll0freq(uint8_t div)
{
  uint32_t predivclk;
  uint32_t regval;
  uint32_t pll0_div;
  uint32_t prediv;
  uint32_t postdiv;
  uint32_t mult;

  regval = getreg32(FC7300_SCG_PLL0CFG);
  prediv = ((regval & SCG_PLL0CFG_PREDIV_MASK) >>
            SCG_PLL0CFG_PREDIV_SHIFT) + 1;
  postdiv = (1 << ((regval & SCG_PLL0CFG_PSTDIV_MASK) >>
             SCG_PLL0CFG_PSTDIV_SHIFT));
  mult   = ((regval & SCG_PLL0CFG_MULT_MASK) >>
            SCG_PLL0CFG_MULT_SHIFT) + 1;

  predivclk = ((FOSC_CLOCK_DFT / prediv) * mult) / postdiv;

  switch (div)
    {
      case HAS_DIVIDER_H:
           pll0_div = g_initial_clkconfig.scg.pll0.divh;
           break;
      case HAS_DIVIDER_M:
           pll0_div = g_initial_clkconfig.scg.pll0.divm;
           break;
      case HAS_DIVIDER_L:
           pll0_div = g_initial_clkconfig.scg.pll0.divl;
           break;
      default:
           return 0;
    }

  return (predivclk >> (pll0_div - 1));
}

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

uint32_t fc7300_get_pll1freq(uint8_t div)
{
  uint32_t predivclk;
  uint32_t regval;
  uint32_t pll1_div;
  uint32_t prediv;
  uint32_t postdiv;
  uint32_t mult;

  regval = getreg32(FC7300_SCG_PLL1CFG);
  prediv = ((regval & SCG_PLL1CFG_PREDIV_MASK) >>
            SCG_PLL1CFG_PREDIV_SHIFT) + 1;
  postdiv = (1 << ((regval & SCG_PLL1CFG_PSTDIV_MASK) >>
             SCG_PLL1CFG_PSTDIV_SHIFT));
  mult   = ((regval & SCG_PLL1CFG_MULT_MASK) >>
            SCG_PLL1CFG_MULT_SHIFT) + 1;

  predivclk = ((FOSC_CLOCK_DFT  / prediv) * mult) / postdiv;

  switch (div)
    {
      case HAS_DIVIDER_H:
           pll1_div = g_initial_clkconfig.scg.pll1.divh;
           break;
      case HAS_DIVIDER_M:
           pll1_div = g_initial_clkconfig.scg.pll1.divm;
           break;
      case HAS_DIVIDER_L:
           pll1_div = g_initial_clkconfig.scg.pll1.divl;
           break;
      default:
           return 0;
    }

  return (predivclk >> (pll1_div - 1));
}

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

uint32_t fc7300_get_aonclk(void)
{
  uint32_t regval;
  uint32_t clkfreq;

  regval = getreg32(FC7300_CSC0_AONCLKSR);
  regval &= CSC0_AONCLKSR_AONCLKSEL_MASK;
  regval >>= CSC0_AONCLKSR_AONCLKSEL_SHIFT;

  switch (regval)
  {
    case 0:
      clkfreq = SIRCDIV_128K_FREQUENCY;
      break;
    case 1:
      clkfreq = SIRC32K_FREQUENCY;
      break;
    case 2:
      clkfreq = SIRCDIV_32K_FREQUENCY;
      break;
    case 3:
      clkfreq = SIRC32_1K_FREQUENCY;
      break;
    //coverity[DEADCODE:SUPPRESS]
    default:
      clkfreq = 0;
      break;
  }

  return clkfreq;
}
