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
#include <nuttx/arch.h>
#include <nuttx/irq.h>

#include <sys/types.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include <errno.h>
#include <debug.h>

#include <arch/board/board.h>

#include "chip.h"
#include "arm_internal.h"
#include "fc7300_fcpit.h"
#include "hardware/fc7300_fcpit.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* This module then only compiles if there are enabled timers that are not
 * intended for some other purpose.
 */

#if defined(CONFIG_FC7300_FCPIT0_CH0) || defined(CONFIG_FC7300_FCPIT0_CH1) ||\
    defined(CONFIG_FC7300_FCPIT0_CH2) || defined(CONFIG_FC7300_FCPIT0_CH3) ||\
    defined(CONFIG_FC7300_FCPIT1_CH0) || defined(CONFIG_FC7300_FCPIT1_CH1) ||\
    defined(CONFIG_FC7300_FCPIT1_CH2) || defined(CONFIG_FC7300_FCPIT1_CH3)

/****************************************************************************
 * Private Types
 ****************************************************************************/

struct fc7300_fcpit_priv_s
{
  const struct fc7300_fcpit_ops_s *ops;
  enum fc7300_fcpit_mode_e mode;
  uint32_t base;                      /* Channel control base address */
  uint8_t instance;
};

/****************************************************************************
 * Private Function prototypes
 ****************************************************************************/

/* Register helpers */

static inline uint32_t fc7300_getreg32(struct fc7300_fcpit_dev_s *dev,
                                      uint8_t offset);
static inline void fc7300_putreg32(struct fc7300_fcpit_dev_s *dev,
                                  uint8_t offset, uint32_t value);

/* Timer helpers */

static void fc7300_fcpit_enable(struct fc7300_fcpit_dev_s *dev);
static void fc7300_fcpit_disable(struct fc7300_fcpit_dev_s *dev);
static void fc7300_fcpit_reset(struct fc7300_fcpit_dev_s *dev);

/* Timer methods */

static int fc7300_fcpit_setmode(struct fc7300_fcpit_dev_s *dev,
                                enum fc7300_fcpit_mode_e mode);
static void fc7300_fcpit_setclock(struct fc7300_fcpit_dev_s *dev,
                                  uint32_t freq);
static uint32_t fc7300_fcpit_getsetcounter(struct fc7300_fcpit_dev_s *dev);
static uint32_t fc7300_fcpit_getcounter(struct fc7300_fcpit_dev_s *dev);
static int fc7300_fcpit_setcounter(struct fc7300_fcpit_dev_s *dev,
                                    uint32_t count);
static int fc7300_fcpit_setchannel(struct fc7300_fcpit_dev_s *dev,
                                   uint8_t channel, bool enable);
static int fc7300_fcpit_setisr(struct fc7300_fcpit_dev_s *dev,
                               xcpt_t handler, void *arg, int source);
static int fc7300_fcpit_enableint(struct fc7300_fcpit_dev_s *dev,
                                  uint8_t channel);
static int fc7300_fcpit_disableint(struct fc7300_fcpit_dev_s *dev,
                                   uint8_t channel);
static int fc7300_fcpit_ackint(struct fc7300_fcpit_dev_s *dev,
                                uint32_t channel);
static uint32_t fc7300_fcpit_pckfreq(struct fc7300_fcpit_dev_s *dev);

/****************************************************************************
 * Private Data
 ****************************************************************************/

static const struct fc7300_fcpit_ops_s fc7300_fcpit_ops =
{
  .setmode       = fc7300_fcpit_setmode,
  .setclock      = fc7300_fcpit_setclock,
  .getsetcounter = fc7300_fcpit_getsetcounter,
  .getcounter    = fc7300_fcpit_getcounter,
  .setcounter    = fc7300_fcpit_setcounter,
  .setchannel    = fc7300_fcpit_setchannel,
  .setisr        = fc7300_fcpit_setisr,
  .enableint     = fc7300_fcpit_enableint,
  .disableint    = fc7300_fcpit_disableint,
  .ackint        = fc7300_fcpit_ackint,
  .pckclk        = fc7300_fcpit_pckfreq,
};

#ifdef CONFIG_FC7300_FCPIT0_CH0
struct fc7300_fcpit_priv_s fc7300_fcpit0_ch0_priv =
{
  .ops        = &fc7300_fcpit_ops,
  .mode       = FC7300_FCPIT_MODE_UNUSED,
  .base       = FC7300_FCPIT0_CH0_CTRL_BASE,
  .instance   = 0
};
#endif
#ifdef CONFIG_FC7300_FCPIT0_CH1
struct fc7300_fcpit_priv_s fc7300_fcpit0_ch1_priv =
{
  .ops        = &fc7300_fcpit_ops,
  .mode       = FC7300_FCPIT_MODE_UNUSED,
  .base       = FC7300_FCPIT0_CH1_CTRL_BASE,
  .instance   = 0
};
#endif

#ifdef CONFIG_FC7300_FCPIT0_CH2
struct fc7300_fcpit_priv_s fc7300_fcpit0_ch2_priv =
{
  .ops        = &fc7300_fcpit_ops,
  .mode       = FC7300_FCPIT_MODE_UNUSED,
  .base       = FC7300_FCPIT0_CH2_CTRL_BASE,
  .instance   = 0
};
#endif

#ifdef CONFIG_FC7300_FCPIT0_CH3
struct fc7300_fcpit_priv_s fc7300_fcpit0_ch3_priv =
{
  .ops        = &fc7300_fcpit_ops,
  .mode       = FC7300_FCPIT_MODE_UNUSED,
  .base       = FC7300_FCPIT0_CH3_CTRL_BASE,
  .instance   = 0
};
#endif

#ifdef CONFIG_FC7300_FCPIT1_CH0
struct fc7300_fcpit_priv_s fc7300_fcpit1_ch0_priv =
{
  .ops        = &fc7300_fcpit_ops,
  .mode       = FC7300_FCPIT_MODE_UNUSED,
  .base       = FC7300_FCPIT1_CH0_CTRL_BASE,
  .instance   = 1
};
#endif

#ifdef CONFIG_FC7300_FCPIT1_CH1
struct fc7300_fcpit_priv_s fc7300_fcpit1_ch1_priv =
{
  .ops        = &fc7300_fcpit_ops,
  .mode       = FC7300_FCPIT_MODE_UNUSED,
  .base       = FC7300_FCPIT1_CH1_CTRL_BASE,
  .instance   = 1
};
#endif

#ifdef CONFIG_FC7300_FCPIT1_CH2
struct fc7300_fcpit_priv_s fc7300_fcpit1_ch2_priv =
{
  .ops        = &fc7300_fcpit_ops,
  .mode       = FC7300_FCPIT_MODE_UNUSED,
  .base       = FC7300_FCPIT1_CH2_CTRL_BASE,
  .instance   = 1
};
#endif

#ifdef CONFIG_FC7300_FCPIT1_CH3
struct fc7300_fcpit_priv_s fc7300_fcpit1_ch3_priv =
{
  .ops        = &fc7300_fcpit_ops,
  .mode       = FC7300_FCPIT_MODE_UNUSED,
  .base       = FC7300_FCPIT1_CH3_CTRL_BASE,
  .instance   = 1
};
#endif

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: fc7300_getreg32
 *
 * Description:
 *   Get a 32-bit register value by offset.  This applies only for the STM32
 *   F4 32-bit registers (CNT, ARR, CRR1-4) in the 32-bit timers TIM2-5.
 *
 ****************************************************************************/

static inline uint32_t fc7300_getreg32(struct fc7300_fcpit_dev_s *dev,
                                      uint8_t offset)
{
  return getreg32(((struct fc7300_fcpit_priv_s *)dev)->base + offset);
}

/****************************************************************************
 * Name: fc7300_putreg32
 *
 * Description:
 *   Put a 32-bit register value by offset.  This applies only for the STM32
 *   F4 32-bit registers (CNT, ARR, CRR1-4) in the 32-bit timers TIM2-5.
 *
 ****************************************************************************/

static inline void fc7300_putreg32(struct fc7300_fcpit_dev_s *dev,
                                  uint8_t offset, uint32_t value)
{
  putreg32(value, ((struct fc7300_fcpit_priv_s *)dev)->base + offset);
}

/****************************************************************************
 * Name: fc7300_fcpit_readmoduleenable
 *
 * Description:
 *   Return module status, true if module has been enabled.
 *
 ****************************************************************************/

static inline uint32_t fc7300_fcpit_readmoduleenable(struct fc7300_fcpit_dev_s *dev)
{
  uint32_t val;

  if (((struct fc7300_fcpit_priv_s *)dev)->instance == 0)
    {
      val = getreg32(FC7300_FCPIT0_MCR);
    }
  else
    {
      val = getreg32(FC7300_FCPIT1_MCR);
    }
  return (uint32_t)(val & FCPIT_MCR_M_CEN_MASK);
}

/****************************************************************************
 * Name: fc7300_fcpit_enable
 ****************************************************************************/

static void fc7300_fcpit_enable(struct fc7300_fcpit_dev_s *dev)
{
  uint32_t val;

  if (((struct fc7300_fcpit_priv_s *)dev)->instance == 0)
    {
      val = getreg32(FC7300_FCPIT0_MCR);
      if((val & FCPIT_MCR_M_CEN_MASK) != 0x1u)
        {
          val |= FCPIT_MCR_M_CEN_MASK;
          putreg32(val, FC7300_FCPIT0_MCR);
        }
    }
  else
    {
      val = getreg32(FC7300_FCPIT1_MCR);
      if((val & FCPIT_MCR_M_CEN_MASK) != 0x1u)
        {
          val |= FCPIT_MCR_M_CEN_MASK;
          putreg32(val, FC7300_FCPIT1_MCR);
        }
    }
}

/****************************************************************************
 * Name: fc7300_fcpit_disable
 ****************************************************************************/

static void fc7300_fcpit_disable(struct fc7300_fcpit_dev_s *dev)
{
  uint32_t val = fc7300_getreg32(dev, FC7300_FCPIT_TCTRL_OFFSET);

  val = (val & ~(uint32_t)FCPIT_TCTRL_T_EN_MASK);
  fc7300_putreg32(dev, FC7300_FCPIT_TCTRL_OFFSET, val);
}

/****************************************************************************
 * Name: fc7300_fcpit_pckfreq
 *
 * Description:
 *   Get the peripheral clock frequency for the FCPIT1 peripheral
 *
 * Input Parameters:
 *   base - The base address of the FCPIT1 peripheral registers
 *
 * Returned Value:
 *   The frequency of the FCPIT1 functional input frequency
 *   (or zero on a failure)
 *
 ****************************************************************************/

static uint32_t fc7300_fcpit_pckfreq(struct fc7300_fcpit_dev_s *dev)
{
  uint32_t pccclk;
  int ret;

  if (((struct fc7300_fcpit_priv_s *)dev)->instance == 0)
  {
    /* Get the PCC source clock */

    ret = fc7300_get_pclkfreq(PCC_CLK_FCPIT0, &pccclk);
  }
  else
  {
    ret = fc7300_get_pclkfreq(PCC_CLK_FCPIT1, &pccclk);
  }

  DEBUGASSERT(ret >= 0);
  if (ret < 0)
    {
      return 0;
    }

  return pccclk;
}

/****************************************************************************
 * Name: fc7300_fcpit_reset
 *
 * Description:
 *   Reset timer into system default state, but do not affect output/input
 *   pins
 *
 ****************************************************************************/

static void fc7300_fcpit_reset(struct fc7300_fcpit_dev_s *dev)
{
  ((struct fc7300_fcpit_priv_s *)dev)->mode = FC7300_FCPIT_DIABLED;
}

/****************************************************************************
 * Name: fc7300_fcpit_setmode
 ****************************************************************************/

static int fc7300_fcpit_setmode(struct fc7300_fcpit_dev_s *dev,
                                enum fc7300_fcpit_mode_e mode)
{
  uint16_t val = fc7300_getreg32(dev, FC7300_FCPIT_TCTRL_OFFSET);

  DEBUGASSERT(dev != NULL);

  switch (mode)
    {
      case FC7300_FCPIT_MODE_UNUSED:
        val = 0;
        break;

      case FC7300_FCPIT_32PERIODIC_COUNTER:

      case FC7300_FCPIT_DUAL_16PERIODIC_COUNTER:

      case FC7300_FCPIT_ACCUMULATOR:

      case FC7300_FCPIT_INPUT_CAPTURE:
        val = ((val & ~(uint32_t)FCPIT_TCTRL_MODE_MASK) |
                FCPIT_TCTRL_MODE(mode));
        break;

      default:
        return -EINVAL;
    }
  fc7300_putreg32(dev, FC7300_FCPIT_TCTRL_OFFSET, val);
  return OK;
}

/****************************************************************************
 * Name: fc7300_fcpit_setclock
 ****************************************************************************/

static void fc7300_fcpit_setclock(struct fc7300_fcpit_dev_s *dev, uint32_t freq)
{

  DEBUGASSERT(dev != NULL);

  if (freq == 0)
    {
      fc7300_fcpit_disable(dev);
    }
  else
    {
      fc7300_fcpit_enable(dev);
    }
}

/****************************************************************************
 * Name: fc7300_fcpit_getsetcounter
 ****************************************************************************/

static uint32_t fc7300_fcpit_getsetcounter(struct fc7300_fcpit_dev_s *dev)
{
  DEBUGASSERT(dev != NULL);

  return fc7300_getreg32(dev, FC7300_FCPIT_TVAL_OFFSET);
}

/****************************************************************************
 * Name: fc7300_fcpit_getcounter
 ****************************************************************************/

static uint32_t fc7300_fcpit_getcounter(struct fc7300_fcpit_dev_s *dev)
{
  uint32_t setcnt;
  DEBUGASSERT(dev != NULL);

  if(fc7300_fcpit_readmoduleenable(dev) == 0x1u)
    {
      /* Add one back to get the actual timeout value */

      setcnt = fc7300_fcpit_getsetcounter(dev) + 1;
      return (setcnt - fc7300_getreg32(dev, FC7300_FCPIT_CVAL_OFFSET));
    }
  else
    {
      return 0;
    }
}

/****************************************************************************
 * Name: fc7300_fcpit_setcounter
 ****************************************************************************/

static int fc7300_fcpit_setcounter(struct fc7300_fcpit_dev_s *dev,
                                   uint32_t count)
{
  DEBUGASSERT(dev != NULL);

  /* The timer interrupt is asserted during reloading the timeout value
   * aka the next clock cycle.
   * Hence, the actual counter should be one less then expected cycle clocks
   * 3    2    1    0    reload    reload - 1
   *                     ^
   *                  Interrupt
   */

  fc7300_putreg32(dev, FC7300_FCPIT_TVAL_OFFSET, count - 1);

  return OK;
}

/****************************************************************************
 * Name: fc7300_fcpit_setrunonlpm
 ****************************************************************************/

static void fc7300_fcpit_setrunonlpm(struct fc7300_fcpit_dev_s *dev,
                                     bool enable)
{
  uint32_t val;

  if (((struct fc7300_fcpit_priv_s *)dev)->instance == 0)
    {
      val = getreg32(FC7300_FCPIT0_MCR);
      if(enable)
        {
          val |= FCPIT_MCR_LPM_EN_MASK;
        }
      else
        {
          val &= ~(uint32_t)FCPIT_MCR_LPM_EN_MASK;
        }
      putreg32(val, FC7300_FCPIT0_MCR);
    }
  else
    {
      val = getreg32(FC7300_FCPIT1_MCR);
      if(enable)
        {
          val |= FCPIT_MCR_LPM_EN_MASK;
        }
      else
        {
          val &= ~(uint32_t)FCPIT_MCR_LPM_EN_MASK;
        }
      putreg32(val, FC7300_FCPIT1_MCR);
    }
}

/****************************************************************************
 * Name: fc7300_fcpit_setchannel
 ****************************************************************************/

static int fc7300_fcpit_setchannel(struct fc7300_fcpit_dev_s *dev,
                                   uint8_t channel, bool enable)
{
  uint16_t value = fc7300_getreg32(dev, FC7300_FCPIT_TCTRL_OFFSET);

  DEBUGASSERT(dev != NULL);

  if (channel > 4)
    {
      return -EINVAL;
    }

  if(enable)
    {
      value |= FCPIT_TCTRL_T_EN_MASK;
      fc7300_fcpit_setrunonlpm(dev, true);
    }
  else
    {
      value &= ~(uint32_t)FCPIT_TCTRL_T_EN_MASK;
    }

  fc7300_putreg32(dev, FC7300_FCPIT_TCTRL_OFFSET, value);

  return OK;
}

/****************************************************************************
 * Name: fc7300_fcpit_setisr
 ****************************************************************************/

static int fc7300_fcpit_setisr(struct fc7300_fcpit_dev_s *dev, xcpt_t handler,
                               void * arg, int source)
{
  int vectorno;

  DEBUGASSERT(dev != NULL);
  DEBUGASSERT(source == 0);

  if (((struct fc7300_fcpit_priv_s *)dev)->instance == 0)
    {
      vectorno = FC7300_IRQ_FCPIT0;
    }
  else
    {
      vectorno = FC7300_IRQ_FCPIT1;
    }

  /* Disable interrupt when callback is removed */

  if (!handler)
    {
      /* Here can not detach due to other channel may already used */

      return OK;
    }

  /* Otherwise set callback and enable interrupt */

  irq_attach(vectorno, handler, arg);
  up_enable_irq(vectorno);

  return OK;
}

/****************************************************************************
 * Name: fc7300_fcpit_enableint
 ****************************************************************************/

static int fc7300_fcpit_enableint(struct fc7300_fcpit_dev_s *dev,
                                  uint8_t channel)
{
  uint32_t val;

  DEBUGASSERT(dev != NULL);

  if(fc7300_fcpit_readmoduleenable(dev) != 0x1u)
    {
      return -EINVAL;
    }

  /* Disable channel */

  fc7300_fcpit_setchannel(dev, channel, (bool)false);

  if (((struct fc7300_fcpit_priv_s *)dev)->instance == 0)
    {
      /* Clear channel interrupt flag */

      val = getreg32(FC7300_FCPIT0_MSR);
      val |= (uint32_t)1U << (uint32_t)channel;
      putreg32(val, FC7300_FCPIT0_MSR);

      /* Enable channel interrupt */

      val = getreg32(FC7300_FCPIT0_MIER);
      val |= (uint32_t)1U << (uint32_t)channel;
      putreg32(val, FC7300_FCPIT0_MIER);
    }
  else
    {
      /* Clear channel interrupt flag */

      val = getreg32(FC7300_FCPIT1_MSR);
      val |= (uint32_t)1U << (uint32_t)channel;
      putreg32(val, FC7300_FCPIT1_MSR);

      /* Enable channel interrupt */

      val = getreg32(FC7300_FCPIT1_MIER);
      val |= (uint32_t)1U << (uint32_t)channel;
      putreg32(val, FC7300_FCPIT1_MIER);
    }

  /* Enable channel */

  fc7300_fcpit_setchannel(dev, channel, (bool)true);

  return OK;
}

/****************************************************************************
 * Name: fc7300_fcpit_disableint
 ****************************************************************************/

static int fc7300_fcpit_disableint(struct fc7300_fcpit_dev_s *dev,
                                   uint8_t channel)
{
  uint32_t val;

  DEBUGASSERT(dev != NULL);

  if(fc7300_fcpit_readmoduleenable(dev) != 0x1u)
    {
      return -EINVAL;
    }

  /* Disable channel */

  fc7300_fcpit_setchannel(dev, channel, (bool)false);

  if (((struct fc7300_fcpit_priv_s *)dev)->instance == 0)
    {
      /* Clear channel interrupt flag */

      val = getreg32(FC7300_FCPIT0_MSR);
      val |= (uint32_t)1U << (uint32_t)channel;
      putreg32(val, FC7300_FCPIT0_MSR);

      /* Disable channel interrupt */

      val = getreg32(FC7300_FCPIT0_MIER);
      val &= ~((uint32_t)1U << (uint32_t)channel);
      putreg32(val, FC7300_FCPIT0_MIER);
    }
  else
    {
      /* Clear channel interrupt flag */

      val = getreg32(FC7300_FCPIT1_MSR);
      val |= (uint32_t)1U << (uint32_t)channel;
      putreg32(val, FC7300_FCPIT1_MSR);

      /* Disable channel interrupt */

      val = getreg32(FC7300_FCPIT1_MIER);
      val &= ~((uint32_t)1U << (uint32_t)channel);
      putreg32(val, FC7300_FCPIT1_MIER);
    }

  /* Enable channel */

  fc7300_fcpit_setchannel(dev, channel, (bool)true);

  return OK;
}

/****************************************************************************
 * Name: fc7300_fcpit_ackint
 ****************************************************************************/

static int fc7300_fcpit_ackint(struct fc7300_fcpit_dev_s *dev,
                                uint32_t channel)
{
  uint32_t val;
  int ret = ERROR;

  DEBUGASSERT(dev != NULL);

  if (((struct fc7300_fcpit_priv_s *)dev)->instance == 0)
    {
      /* Clear channel interrupt flag */

      val = getreg32(FC7300_FCPIT0_MSR);
      if ((val & (1U << channel)) != 0)
      {
        val = 1U << channel;
        putreg32(val, FC7300_FCPIT0_MSR);
        val = getreg32(FC7300_FCPIT0_MIER);
        if ((val & (1U << channel)) != 0)
        {
          ret = OK;
        }
      }
    }
  else
    {
      /* Clear channel interrupt flag */

      val = getreg32(FC7300_FCPIT1_MSR);
      if ((val & (1U << channel)) != 0)
      {
        val = 1U << channel;
        putreg32(val, FC7300_FCPIT1_MSR);
        val = getreg32(FC7300_FCPIT1_MIER);
        if ((val & (1U << channel)) != 0)
        {
          ret = OK;
        }
      }
    }
    return ret;
}

/****************************************************************************
 * Pubic Functions
 ****************************************************************************/

/****************************************************************************
 * Name: fc7300_fcpit_init
 ****************************************************************************/

struct fc7300_fcpit_dev_s *fc7300_fcpit_init(int timer)
{
  struct fc7300_fcpit_dev_s *dev = NULL;

  /* Get structure and enable power */

  switch (timer)
    {
#ifdef CONFIG_FC7300_FCPIT0_CH0
      case 0:
        dev = (struct fc7300_fcpit_dev_s *)&fc7300_fcpit0_ch0_priv;
        break;
#endif
#ifdef CONFIG_FC7300_FCPIT0_CH1
      case 1:
        dev = (struct fc7300_fcpit_dev_s *)&fc7300_fcpit0_ch1_priv;
        break;
#endif
#ifdef CONFIG_FC7300_FCPIT0_CH2
      case 2:
        dev = (struct fc7300_fcpit_dev_s *)&fc7300_fcpit0_ch2_priv;
        break;
#endif
#ifdef CONFIG_FC7300_FCPIT0_CH3
      case 3:
        dev = (struct fc7300_fcpit_dev_s *)&fc7300_fcpit0_ch3_priv;
        break;
#endif
#ifdef CONFIG_FC7300_FCPIT1_CH0
      case 4:
        dev = (struct fc7300_fcpit_dev_s *)&fc7300_fcpit1_ch0_priv;
        break;
#endif
#ifdef CONFIG_FC7300_FCPIT1_CH1
      case 5:
        dev = (struct fc7300_fcpit_dev_s *)&fc7300_fcpit1_ch1_priv;
        break;
#endif
#ifdef CONFIG_FC7300_FCPIT1_CH2
      case 6:
        dev = (struct fc7300_fcpit_dev_s *)&fc7300_fcpit1_ch2_priv;
        break;
#endif
#ifdef CONFIG_FC7300_FCPIT1_CH3
      case 7:
        dev = (struct fc7300_fcpit_dev_s *)&fc7300_fcpit1_ch3_priv;
        break;
#endif
      default:
        return NULL;
    }

  /* Is device already allocated */

  if (((struct fc7300_fcpit_priv_s *)dev)->mode != FC7300_FCPIT_MODE_UNUSED)
    {
      return NULL;
    }

  fc7300_fcpit_reset(dev);

  return dev;
}

/****************************************************************************
 * Name: fc7300_fcpit_deinit
 *
 * TODO: Detach interrupts, and close down all FCPIT Channels
 *
 ****************************************************************************/

int fc7300_fcpit_deinit(struct fc7300_fcpit_dev_s * dev)
{
  DEBUGASSERT(dev != NULL);

  /* Disable power */

  fc7300_fcpit_disable(dev);

  /* Mark it as free */

  ((struct fc7300_fcpit_priv_s *)dev)->mode = FC7300_FCPIT_MODE_UNUSED;

  return OK;
}

#endif /* defined(CONFIG_FC7300_FCPIT0_CH0 || ... || CONFIG_FC7300_FCPIT1_CH3) */
