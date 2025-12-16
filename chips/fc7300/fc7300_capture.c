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
#include <math.h>

#include "chip.h"
#include "arm_internal.h"
#include "fc7300_capture.h"
#include "fc7300_irq.h"
#include "fc7300_trgsel.h"

/****************************************************************************
 * Private Types
 ****************************************************************************/

/* Configuration ************************************************************/

/* This module then only compiles if there are enabled timers that are not
 * intended for some other purpose.
 */

#if defined(CONFIG_FC7300_FTU0_CAP) || defined(CONFIG_FC7300_FTU1_CAP) || \
    defined(CONFIG_FC7300_FTU2_CAP) || defined(CONFIG_FC7300_FTU3_CAP) || \
    defined(CONFIG_FC7300_FTU4_CAP) || defined(CONFIG_FC7300_FTU5_CAP) || \
    defined(CONFIG_FC7300_FTU6_CAP) || defined(CONFIG_FC7300_FTU7_CAP) || \
    defined(CONFIG_FC7300_FTU8_CAP) || defined(CONFIG_FC7300_FTU9_CAP) || \
    defined(CONFIG_FC7300_FTU10_CAP) || defined(CONFIG_FC7300_FTU11_CAP) || \
    defined(CONFIG_FC7300_ISM0_CAP)

/****************************************************************************
 * Private Types
 ****************************************************************************/

/* Device Structure */

struct fc7300_cap_priv_s
{
  const struct   fc7300_cap_ops_s *ops;
  const uint8_t  instance_num;
  const uint32_t base;      /* ISMn or FTUn base address */
  const int irq;            /* irq vector */
};

#ifdef FC7300_CAP_OVERFLOW_USED

/* Flag whether the Ftu instance Overflow interrupt enabled by capture */

uint8_t g_capoverflowflag[MAX_FTU_INSTANCE_COUNT];

#endif

static int fc7300_cap_setclock(struct fc7300_cap_dev_s *dev, uint8_t hwunit,
                               uint32_t freq, uint8_t channel);

static int fc7300_cap_setchannel(struct fc7300_cap_dev_s *dev, uint8_t hwunit,
                                 uint8_t channel, uint32_t pincfg,
                                uint8_t trgsel, uint8_t mode);

static uint32_t fc7300_cap_getcapture(struct fc7300_cap_dev_s *dev,
                                      uint8_t hwunit, uint8_t channel);

static void fc7300_cap_setisr(struct fc7300_cap_dev_s *dev, xcpt_t handler,
                              void *arg);

static int fc7300_cap_enableint(struct fc7300_cap_dev_s *dev, uint8_t hwunit,
                                uint8_t channel, bool on);

static uint8_t fc7300_cap_ackflags(struct fc7300_cap_dev_s *dev);

static enum ftu_inputleveltype_e fc7300_cap_getinputlevel
                               (struct fc7300_cap_dev_s *dev, uint8_t channel);

static uint8_t fc7300_cap_remapchannels(struct fc7300_cap_dev_s *dev,
                                        uint8_t channel_in);

static bool fc7300_cap_readoverflowflg(struct fc7300_cap_dev_s *dev);

static uint32_t fc7300_cap_getoverflowval(struct fc7300_cap_dev_s *dev);

static uint32_t fc7300_cap_getprescale(struct fc7300_cap_dev_s *dev);

struct fc7300_cap_ops_s fc7300_cap_ops =
{
  .setclock      = &fc7300_cap_setclock,
  .setchannel    = &fc7300_cap_setchannel,
  .getcapture    = &fc7300_cap_getcapture,
  .setisr        = &fc7300_cap_setisr,
  .enableint     = &fc7300_cap_enableint,
  .ackflags      = &fc7300_cap_ackflags,
  .getinputlevel = &fc7300_cap_getinputlevel,
  .remapchannels = &fc7300_cap_remapchannels,
  .overflowflg   = &fc7300_cap_readoverflowflg,
  .overflowval   = &fc7300_cap_getoverflowval,
  .getprescale   = &fc7300_cap_getprescale
};

#ifdef CONFIG_FC7300_ISM0_CAP
const struct fc7300_cap_priv_s fc7300_cap0_priv =
{
  .ops = &fc7300_cap_ops,
  .instance_num = 0,
  .base = FC7300_ISM_BASE,
  .irq = FC7300_IRQ_ISM,
};
#endif

#ifdef CONFIG_FC7300_FTU0_CAP
const struct fc7300_cap_priv_s fc7300_cap1_priv =
{
  .ops          = &fc7300_cap_ops,
  .instance_num = 0,
  .base         = FC7300_FTU0_BASE,
  .irq          = FC7300_IRQ_FTU0,
};
#endif

#ifdef CONFIG_FC7300_FTU1_CAP
const struct fc7300_cap_priv_s fc7300_cap2_priv =
{
  .ops          = &fc7300_cap_ops,
  .instance_num = 1,
  .base         = FC7300_FTU1_BASE,
  .irq          = FC7300_IRQ_FTU1,
};
#endif

#ifdef CONFIG_FC7300_FTU2_CAP
const struct fc7300_cap_priv_s fc7300_cap3_priv =
{
  .ops          = &fc7300_cap_ops,
  .instance_num = 2,
  .base         = FC7300_FTU2_BASE,
  .irq          = FC7300_IRQ_FTU2,
};
#endif

#ifdef CONFIG_FC7300_FTU3_CAP
const struct fc7300_cap_priv_s fc7300_cap4_priv =
{
  .ops          = &fc7300_cap_ops,
  .instance_num = 3,
  .base         = FC7300_FTU3_BASE,
  .irq          = FC7300_IRQ_FTU3,
};
#endif

#ifdef CONFIG_FC7300_FTU4_CAP
const struct fc7300_cap_priv_s fc7300_cap5_priv =
{
  .ops          = &fc7300_cap_ops,
  .instance_num = 4,
  .base         = FC7300_FTU4_BASE,
  .irq          = FC7300_IRQ_FTU4,
};
#endif

#ifdef CONFIG_FC7300_FTU5_CAP
const struct fc7300_cap_priv_s fc7300_cap6_priv =
{
  .ops          = &fc7300_cap_ops,
  .instance_num = 5,
  .base         = FC7300_FTU5_BASE,
  .irq          = FC7300_IRQ_FTU5,
};
#endif

#ifdef CONFIG_FC7300_FTU6_CAP
const struct fc7300_cap_priv_s fc7300_cap7_priv =
{
  .ops          = &fc7300_cap_ops,
  .instance_num = 6,
  .base         = FC7300_FTU6_BASE,
  .irq          = FC7300_IRQ_FTU6,
};
#endif

#ifdef CONFIG_FC7300_FTU7_CAP
const struct fc7300_cap_priv_s fc7300_cap8_priv =
{
  .ops          = &fc7300_cap_ops,
  .instance_num = 7,
  .base         = FC7300_FTU7_BASE,
  .irq          = FC7300_IRQ_FTU7,
};
#endif

#ifdef CONFIG_FC7300_FTU8_CAP
const struct fc7300_cap_priv_s fc7300_cap9_priv =
{
  .ops          = &fc7300_cap_ops,
  .instance_num = 8,
  .base         = FC7300_FTU8_BASE,
  .irq          = FC7300_IRQ_FTU8,
};
#endif

#ifdef CONFIG_FC7300_FTU9_CAP
const struct fc7300_cap_priv_s fc7300_cap10_priv =
{
  .ops          = &fc7300_cap_ops,
  .instance_num = 9,
  .base         = FC7300_FTU9_BASE,
  .irq          = FC7300_IRQ_FTU9,
};
#endif

#ifdef CONFIG_FC7300_FTU10_CAP
const struct fc7300_cap_priv_s fc7300_cap11_priv =
{
  .ops          = &fc7300_cap_ops,
  .instance_num = 10,
  .base         = FC7300_FTU10_BASE,
  .irq          = FC7300_IRQ_FTU10,
};
#endif

#ifdef CONFIG_FC7300_FTU11_CAP
const struct fc7300_cap_priv_s fc7300_cap12_priv =
{
  .ops          = &fc7300_cap_ops,
  .instance_num = 11,
  .base         = FC7300_FTU11_BASE,
  .irq          = FC7300_IRQ_FTU11,
};
#endif

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/* Get a 32-bit register value by offset. */

static inline uint32_t cap_getreg32(const struct fc7300_cap_priv_s *priv,
                                    uint32_t offset)
{
  return getreg32(priv->base + offset);
}

/* Put a 32-bit register value by offset. */

static inline void cap_putreg32(const struct fc7300_cap_priv_s *priv,
                                uint32_t offset, uint32_t value)
{
  putreg32(value, priv->base + offset);
}

/* Get channel interrupt flag. */

static inline bool cap_getchannelintflag(const struct fc7300_cap_priv_s *priv,
                                         uint8_t channel)
{
  uint32_t regval;
  uint32_t mask = FTU_CSC_CHIE_MASK | FTU_CSC_CHF_MASK;

  switch (channel)
    {
        case 0:  /* CAP channel isr configuration: Channel 0 */
          {
            regval = cap_getreg32(priv, FC7300_FTU_C0SC_OFFSET);
          }
        break;

        case 1:  /* CAP channel isr configuration: Channel 1 */
          {
            regval = cap_getreg32(priv, FC7300_FTU_C1SC_OFFSET);
          }
        break;

        case 2:  /* CAP channel isr configuration: Channel 2 */
          {
            regval = cap_getreg32(priv, FC7300_FTU_C2SC_OFFSET);
          }
        break;

        case 3:  /* CAP channel isr configuration: Channel 3 */
          {
            regval = cap_getreg32(priv, FC7300_FTU_C3SC_OFFSET);
          }
        break;

        case 4:  /* CAP channel isr configuration: Channel 4 */
          {
            regval = cap_getreg32(priv, FC7300_FTU_C4SC_OFFSET);
          }
        break;

        case 5:  /* CAP channel isr configuration: Channel 5 */
          {
            regval = cap_getreg32(priv, FC7300_FTU_C5SC_OFFSET);
          }
        break;

        case 6:  /* CAP channel isr configuration: Channel 6 */
          {
            regval = cap_getreg32(priv, FC7300_FTU_C6SC_OFFSET);
          }
        break;

        case 7:  /* CAP channel isr configuration: Channel 7 */
          {
            regval = cap_getreg32(priv, FC7300_FTU_C7SC_OFFSET);
          }
        break;

        default:
            return -EINVAL;
    }

  return (regval & mask) == mask ? true : false;
}

/* Clear channel interrupt flag. */

static inline void cap_clrchannelintflag(const struct fc7300_cap_priv_s *priv,
                                         uint8_t channel)
{
  uint32_t regval;

  switch (channel)
    {
      case 0:  /* CAP channel isr configuration: Channel 0 */
        {
          regval = cap_getreg32(priv, FC7300_FTU_C0SC_OFFSET);
          regval &= ~FTU_CSC_CHF_MASK;
          cap_putreg32(priv, FC7300_FTU_C0SC_OFFSET, regval);
        }
      break;

      case 1:  /* CAP channel isr configuration: Channel 1 */
        {
          regval = cap_getreg32(priv, FC7300_FTU_C1SC_OFFSET);
          regval &= ~FTU_CSC_CHF_MASK;
          cap_putreg32(priv, FC7300_FTU_C1SC_OFFSET, regval);
        }
      break;

      case 2:  /* CAP channel isr configuration: Channel 2 */
        {
          regval = cap_getreg32(priv, FC7300_FTU_C2SC_OFFSET);
          regval &= ~FTU_CSC_CHF_MASK;
          cap_putreg32(priv, FC7300_FTU_C2SC_OFFSET, regval);
        }
      break;

      case 3:  /* CAP channel isr configuration: Channel 3 */
        {
          regval = cap_getreg32(priv, FC7300_FTU_C3SC_OFFSET);
          regval &= ~FTU_CSC_CHF_MASK;
          cap_putreg32(priv, FC7300_FTU_C3SC_OFFSET, regval);
        }
      break;

      case 4:  /* CAP channel isr configuration: Channel 4 */
        {
          regval = cap_getreg32(priv, FC7300_FTU_C4SC_OFFSET);
          regval &= ~FTU_CSC_CHF_MASK;
          cap_putreg32(priv, FC7300_FTU_C4SC_OFFSET, regval);
        }
      break;

      case 5:  /* CAP channel isr configuration: Channel 5 */
        {
          regval = cap_getreg32(priv, FC7300_FTU_C5SC_OFFSET);
          regval &= ~FTU_CSC_CHF_MASK;
          cap_putreg32(priv, FC7300_FTU_C5SC_OFFSET, regval);
        }
      break;

      case 6:  /* CAP channel isr configuration: Channel 6 */
        {
          regval = cap_getreg32(priv, FC7300_FTU_C6SC_OFFSET);
          regval &= ~FTU_CSC_CHF_MASK;
          cap_putreg32(priv, FC7300_FTU_C6SC_OFFSET, regval);
        }
      break;

      case 7:  /* CAP channel isr configuration: Channel 7 */
        {
          regval = cap_getreg32(priv, FC7300_FTU_C7SC_OFFSET);
          regval &= ~FTU_CSC_CHF_MASK;
          cap_putreg32(priv, FC7300_FTU_C7SC_OFFSET, regval);
        }
      break;

      default:
        return;
    }
}

/* Get private timer structure. */

static inline const struct fc7300_cap_priv_s *fc7300_cap_get_priv(int timer)
{
  switch (timer)
  {
#ifdef CONFIG_FC7300_ISM0_CAP
      case 0:
        return &fc7300_cap0_priv;
#endif
#ifdef CONFIG_FC7300_FTU0_CAP
      case 1:
        return &fc7300_cap1_priv;
#endif
#ifdef CONFIG_FC7300_FTU1_CAP
      case 2:
        return &fc7300_cap2_priv;
#endif
#ifdef CONFIG_FC7300_FTU2_CAP
      case 3:
        return &fc7300_cap3_priv;
#endif
#ifdef CONFIG_FC7300_FTU3_CAP
      case 4:
        return &fc7300_cap4_priv;
#endif
#ifdef CONFIG_FC7300_FTU4_CAP
      case 5:
        return &fc7300_cap5_priv;
#endif
#ifdef CONFIG_FC7300_FTU5_CAP
      case 6:
        return &fc7300_cap6_priv;
#endif
#ifdef CONFIG_FC7300_FTU6_CAP
      case 7:
        return &fc7300_cap7_priv;
#endif
#ifdef CONFIG_FC7300_FTU7_CAP
      case 8:
        return &fc7300_cap8_priv;
#endif
#ifdef CONFIG_FC7300_FTU8_CAP
      case 9:
        return &fc7300_cap9_priv;
#endif
#ifdef CONFIG_FC7300_FTU9_CAP
      case 10:
        return &fc7300_cap10_priv;
#endif
#ifdef CONFIG_FC7300_FTU10_CAP
      case 11:
        return &fc7300_cap11_priv;
#endif
#ifdef CONFIG_FC7300_FTU11_CAP
      case 12:
        return &fc7300_cap12_priv;
#endif
      default:
        break;
  }

  return NULL;
}

/****************************************************************************
 * Basic Functions
 ****************************************************************************/

/****************************************************************************
 * Name: fc7300_cap_isr_handle
 *
 * Description:
 *   Disable CAPn interrupt.
 *
 * Input Parameters:
 *
 * Returned Value: void
 *
 ****************************************************************************/

#ifdef CONFIG_FC7300_CAP_IRQ_ATTACH_THREAD
__ramfunc__ static int fc7300_cap_isr_handle(int irq, void *regs, void *arg)
{
  up_disable_irq(irq);
  return IRQ_WAKE_THREAD;
}
#endif

/****************************************************************************
 * Name: fc7300_cap_setisr
 *
 * Description:
 *   Allocate a CPU Interrupt, connect the peripheral source to this
 *   Interrupt, register the callback and enable CPU the Interruption.
 *   In case a NULL handler is provided, deallocate the interrupt and
 *   unregister the previously provided handler.
 *
 * Parameters:
 *   dev           - Pointer to the driver state structure.
 *   handler       - Callback to be invoked on timer interrupt.
 *   arg           - Argument to be passed to the handler callback.
 *
 * Returned Values:
 *   void.
 *
 ****************************************************************************/

static void fc7300_cap_setisr(struct fc7300_cap_dev_s *dev, xcpt_t handler,
                              void *arg)
{
  const struct fc7300_cap_priv_s *priv = (const struct fc7300_cap_priv_s *)dev;
  int irq;

  DEBUGASSERT(dev != NULL);

  irq = priv->irq;

  /* Disable interrupt when callback is removed */

  if (!handler)
    {
      up_disable_irq(irq);
      irq_detach(irq);
#ifdef CONFIG_FC7300_CAP_IRQ_ATTACH_THREAD
      irq_detach_wqueue(irq);
#endif
    }

#ifdef CONFIG_ARCH_IRQPRIO
  switch (priv->base)
  {
#ifdef CONFIG_FC7300_ISM0_CAP
      case FC7300_ISM_BASE:
        up_prioritize_irq(irq, NVIC_SYSH_PRIORITY_DEFAULT);
        break;
#endif
#ifdef CONFIG_FC7300_FTU0_CAP
      case FC7300_FTU0_BASE:
        up_prioritize_irq(irq, NVIC_FTU0_PRIORITY_DEFAULT);
        break;
#endif
#ifdef CONFIG_FC7300_FTU1_CAP
      case FC7300_FTU1_BASE:
        up_prioritize_irq(irq, NVIC_FTU1_PRIORITY_DEFAULT);
        break;
#endif
#ifdef CONFIG_FC7300_FTU2_CAP
      case FC7300_FTU2_BASE:
        up_prioritize_irq(irq, NVIC_FTU2_PRIORITY_DEFAULT);
        break;
#endif
#ifdef CONFIG_FC7300_FTU3_CAP
      case FC7300_FTU3_BASE:
        up_prioritize_irq(irq, NVIC_FTU3_PRIORITY_DEFAULT);
        break;
#endif
#ifdef CONFIG_FC7300_FTU4_CAP
      case FC7300_FTU4_BASE:
        up_prioritize_irq(irq, NVIC_FTU4_PRIORITY_DEFAULT);
        break;
#endif
#ifdef CONFIG_FC7300_FTU5_CAP
      case FC7300_FTU5_BASE:
        up_prioritize_irq(irq, NVIC_FTU5_PRIORITY_DEFAULT);
        break;
#endif
#ifdef CONFIG_FC7300_FTU6_CAP
      case FC7300_FTU6_BASE:
        up_prioritize_irq(irq, NVIC_FTU6_PRIORITY_DEFAULT);
        break;
#endif
#ifdef CONFIG_FC7300_FTU7_CAP
      case FC7300_FTU7_BASE:
        up_prioritize_irq(irq, NVIC_FTU7_PRIORITY_DEFAULT);
        break;
#endif
#ifdef CONFIG_FC7300_FTU8_CAP
      case FC7300_FTU8_BASE:
        up_prioritize_irq(irq, NVIC_FTU8_PRIORITY_DEFAULT);
        break;
#endif
#ifdef CONFIG_FC7300_FTU9_CAP
      case FC7300_FTU9_BASE:
        up_prioritize_irq(irq, NVIC_FTU9_PRIORITY_DEFAULT);
        break;
#endif
#ifdef CONFIG_FC7300_FTU10_CAP
      case FC7300_FTU10_BASE:
        up_prioritize_irq(irq, NVIC_FTU10_PRIORITY_DEFAULT);
        break;
#endif
#ifdef CONFIG_FC7300_FTU11_CAP
      case FC7300_FTU11_BASE:
        up_prioritize_irq(irq, NVIC_FTU11_PRIORITY_DEFAULT);
        break;
#endif
  default:
    break;
  }
#endif

  /* Otherwise set callback and enable interrupt */

#ifdef CONFIG_FC7300_CAP_IRQ_ATTACH_THREAD
    irq_attach_wqueue(irq, fc7300_cap_isr_handle, handler, arg,
                      isr_cfg[CAP_ISR_THREAD]);
#else
    irq_attach(irq, handler, arg);
#endif
  up_enable_irq(irq);
}

/****************************************************************************
 * Name: fc7300_cap_enableint
 *
 * Description:
 *   This method is to enable interrupt of specified channel.
 *
 * Input Parameters:
 *   dev - A pointer to device.
 *   channel - The channel number to be operated.
 *   on - Wether to enable interrupt.
 *
 * Returned Value:
 *   OK if set successfully.
 *
 ****************************************************************************/

static int fc7300_cap_enableint(struct fc7300_cap_dev_s *dev, uint8_t hwunit,
                                uint8_t channel, bool on)
{
  const struct fc7300_cap_priv_s *priv = (const struct fc7300_cap_priv_s *)dev;
  uint32_t regval;

  DEBUGASSERT(dev != NULL);

  /* enable or disable channel interrupt */

  if (hwunit == 12)
    {
      regval = cap_getreg32(priv, FC7300_ISM_CTRL_OFFSET);
      regval = (regval & ~FC7300_ISM_CTRL_IEN_MASK) |
               FC7300_ISM_CTRL_IEN(false);
      cap_putreg32(priv, FC7300_ISM_CTRL_OFFSET, regval);
    }
  else if (hwunit < 12)
    {
      /* stop ftu timer */

      regval = cap_getreg32(priv, FC7300_FTU_SC_OFFSET);
      regval &= ~(FTU_SC_CLKS_MASK);
      cap_putreg32(priv, FC7300_FTU_SC_OFFSET, regval);

      /* enable or disable channel interrupt */

      switch (channel)
        {
          case 0:  /* CAP channel isr configuration: Channel 0 */
            {
              regval = cap_getreg32(priv, FC7300_FTU_C0SC_OFFSET);
              regval = on ? (regval | FTU_CSC_CHIE_MASK) :
                            (regval & ~(FTU_CSC_CHIE_MASK | FTU_CSC_CHF_MASK |
                                      FTU_CSC_ELSA_MASK | FTU_CSC_ELSB_MASK));
              cap_putreg32(priv, FC7300_FTU_C0SC_OFFSET, regval);
            }
            break;

          case 1:  /* CAP channel isr configuration: Channel 1 */
            {
              regval = cap_getreg32(priv, FC7300_FTU_C1SC_OFFSET);
              regval = on ? (regval | FTU_CSC_CHIE_MASK) :
                            (regval & ~(FTU_CSC_CHIE_MASK | FTU_CSC_CHF_MASK |
                                      FTU_CSC_ELSA_MASK | FTU_CSC_ELSB_MASK));
              cap_putreg32(priv, FC7300_FTU_C1SC_OFFSET, regval);
            }
            break;

          case 2:  /* CAP channel isr configuration: Channel 2 */
            {
              regval = cap_getreg32(priv, FC7300_FTU_C2SC_OFFSET);
              regval = on ? (regval | FTU_CSC_CHIE_MASK) :
                            (regval & ~(FTU_CSC_CHIE_MASK | FTU_CSC_CHF_MASK |
                                      FTU_CSC_ELSA_MASK | FTU_CSC_ELSB_MASK));
              cap_putreg32(priv, FC7300_FTU_C2SC_OFFSET, regval);
            }
            break;

          case 3:  /* CAP channel isr configuration: Channel 3 */
            {
              regval = cap_getreg32(priv, FC7300_FTU_C3SC_OFFSET);
              regval = on ? (regval | FTU_CSC_CHIE_MASK) :
                            (regval & ~(FTU_CSC_CHIE_MASK | FTU_CSC_CHF_MASK |
                                      FTU_CSC_ELSA_MASK | FTU_CSC_ELSB_MASK));
              cap_putreg32(priv, FC7300_FTU_C3SC_OFFSET, regval);
            }
            break;

          case 4:  /* CAP channel isr configuration: Channel 4 */
            {
              regval = cap_getreg32(priv, FC7300_FTU_C4SC_OFFSET);
              regval = on ? (regval | FTU_CSC_CHIE_MASK) :
                            (regval & ~(FTU_CSC_CHIE_MASK | FTU_CSC_CHF_MASK |
                                      FTU_CSC_ELSA_MASK | FTU_CSC_ELSB_MASK));
              cap_putreg32(priv, FC7300_FTU_C4SC_OFFSET, regval);
            }
            break;

          case 5:  /* CAP channel isr configuration: Channel 5 */
            {
              regval = cap_getreg32(priv, FC7300_FTU_C5SC_OFFSET);
              regval = on ? (regval | FTU_CSC_CHIE_MASK) :
                            (regval & ~(FTU_CSC_CHIE_MASK | FTU_CSC_CHF_MASK |
                                      FTU_CSC_ELSA_MASK | FTU_CSC_ELSB_MASK));
              cap_putreg32(priv, FC7300_FTU_C5SC_OFFSET, regval);
            }
            break;

          case 6:  /* CAP channel isr configuration: Channel 6 */
            {
              regval = cap_getreg32(priv, FC7300_FTU_C6SC_OFFSET);
              regval = on ? (regval | FTU_CSC_CHIE_MASK) :
                            (regval & ~(FTU_CSC_CHIE_MASK | FTU_CSC_CHF_MASK |
                                      FTU_CSC_ELSA_MASK | FTU_CSC_ELSB_MASK));
              cap_putreg32(priv, FC7300_FTU_C6SC_OFFSET, regval);
            }
            break;

          case 7:  /* CAP channel isr configuration: Channel 7 */
            {
              regval = cap_getreg32(priv, FC7300_FTU_C7SC_OFFSET);
              regval = on ? (regval | FTU_CSC_CHIE_MASK) :
                            (regval & ~(FTU_CSC_CHIE_MASK | FTU_CSC_CHF_MASK |
                                      FTU_CSC_ELSA_MASK | FTU_CSC_ELSB_MASK));
              cap_putreg32(priv, FC7300_FTU_C7SC_OFFSET, regval);
            }
            break;

          default:
              return -EINVAL;
        }

      /* IRQ on module overflow */
#ifdef FC7300_CAP_OVERFLOW_USED
      if (on)
        {
          regval = cap_getreg32(priv, FC7300_FTU_SC_OFFSET);
          regval |= FTU_SC_TOIE_MASK;
          cap_putreg32(priv, FC7300_FTU_SC_OFFSET, regval);
          g_capoverflowflag[priv->instance_num] |= (1u << channel);
        }
      else
        {
          regval = cap_getreg32(priv, FC7300_FTU_SC_OFFSET);
          regval &= ~FTU_SC_TOIE_MASK;
          cap_putreg32(priv, FC7300_FTU_SC_OFFSET, regval);
          g_capoverflowflag[priv->instance_num] &= ~(1u << channel);
        }
#endif

    /* set ftu clk source */

    regval = cap_getreg32(priv, FC7300_FTU_SC_OFFSET);
    regval = (regval & ~(uint32_t)FTU_SC_CLKS_MASK) |
             FTU_SC_CLKS(FTU_EXTERNAL_CLK0);
    cap_putreg32(priv, FC7300_FTU_SC_OFFSET, regval);
  }
  else
  {
    return -EINVAL;
  }

  return OK;
}

/****************************************************************************
 * Name: fc7300_cap_readoverflowflg
 *
 * Description:
 *   This method is to get ISM module overflow flag.
 *
 * Input Parameters:
 *   dev - A pointer to device.
 *
 * Returned Value:
 *   Wether overflow flag is set.
 *
 ****************************************************************************/

static bool fc7300_cap_readoverflowflg(struct fc7300_cap_dev_s *dev)
{
  const struct fc7300_cap_priv_s *priv = (const struct fc7300_cap_priv_s *)dev;
  uint32_t regval;

  regval = cap_getreg32(priv, FC7300_FTU_SC_OFFSET);

  return (0u != (regval & FTU_SC_TOF_MASK));
}

/****************************************************************************
 * Name: fc7300_cap_getoverflowval
 *
 * Description:
 *   This method is to get FTU module overflow flag.
 *
 * Input Parameters:
 *   dev - A pointer to device.
 *
 * Returned Value:
 *   Wether overflow flag is set.
 *
 ****************************************************************************/

static uint32_t fc7300_cap_getoverflowval(struct fc7300_cap_dev_s *dev)
{
  const struct fc7300_cap_priv_s *priv = (const struct fc7300_cap_priv_s *)dev;

  return cap_getreg32(priv, FC7300_FTU_MOD_OFFSET);
}

/****************************************************************************
 * Name: fc7300_cap_getprescale
 *
 * Description:
 *   This method is to get FTU module clk prescale.
 *
 * Input Parameters:
 *   dev - A pointer to device.
 *
 * Returned Value:
 *   Clk prescale value.
 *
 ****************************************************************************/

static uint32_t fc7300_cap_getprescale(struct fc7300_cap_dev_s *dev)
{
  const struct fc7300_cap_priv_s *priv = (const struct fc7300_cap_priv_s *)dev;
  uint32_t regval;

  regval = cap_getreg32(priv, FC7300_FTU_SC_OFFSET);
  regval &= FTU_SC_PS_MASK;

  return (uint32_t)(1 << regval);
}

/****************************************************************************
 * Name: fc7300_cap_ackflags
 *
 * Description:
 *   This method is to get and clear interrupt flag.
 *
 * Input Parameters:
 *   dev - A pointer to device.
 *
 * Returned Value:
 *   Characterize which channel caused the interruption.
 *
 ****************************************************************************/

static uint8_t fc7300_cap_ackflags(struct fc7300_cap_dev_s *dev)
{
  const struct fc7300_cap_priv_s *priv = (const struct fc7300_cap_priv_s *)dev;
  uint8_t channelintflag = 0;
  uint8_t i;
  uint32_t regval;

  for (i = 0U; i < MAX_FTU_CHANNEL_COUNT; i++)
    {
      if (true == cap_getchannelintflag(priv, i))
        {
          cap_clrchannelintflag(priv, i);
          channelintflag |= (uint8_t)(1u << i);
        }
    }

  regval = cap_getreg32(priv, FC7300_FTU_SC_OFFSET);

  if ((0u != (regval & FTU_SC_TOIE_MASK)) &&
      (fc7300_cap_readoverflowflg(dev) == true))
    {
      regval = cap_getreg32(priv, FC7300_FTU_SC_OFFSET);
      regval &= ~FTU_SC_TOF_MASK;
      cap_putreg32(priv, FC7300_FTU_SC_OFFSET, regval);
    }

  return channelintflag;
}

/****************************************************************************
 * Name: fc7300_cap_remapchannels
 *
 * Description:
 *   This method is convert the FTUxCH to 0~96.
 *
 * Input Parameters:
 *   dev - A pointer to device.
 *   channel_in - The channel number to be converted.
 *
 * Returned Value:
 *   Converted channel number.
 *
 ****************************************************************************/

static uint8_t fc7300_cap_remapchannels(struct fc7300_cap_dev_s *dev,
                                        uint8_t channel_in)
{
  const struct fc7300_cap_priv_s *priv = (const struct fc7300_cap_priv_s *)dev;
  uint8_t channel_num = 0;

  switch (priv->base)
    {
      case FC7300_FTU0_BASE:
        channel_num = channel_in;
        break;

      case FC7300_FTU1_BASE:
        channel_num = channel_in + 8;
        break;

      case FC7300_FTU2_BASE:
        channel_num = channel_in + 16;
        break;

      case FC7300_FTU3_BASE:
        channel_num = channel_in + 24;
        break;

      case FC7300_FTU4_BASE:
        channel_num = channel_in + 32;
        break;

      case FC7300_FTU5_BASE:
        channel_num = channel_in + 40;
        break;

      case FC7300_FTU6_BASE:
        channel_num = channel_in + 48;
        break;

      case FC7300_FTU7_BASE:
        channel_num = channel_in + 56;
        break;

      case FC7300_FTU8_BASE:
        channel_num = channel_in + 64;
        break;

      case FC7300_FTU9_BASE:
        channel_num = channel_in + 72;
        break;

      case FC7300_FTU10_BASE:
        channel_num = channel_in + 80;
        break;

      case FC7300_FTU11_BASE:
        channel_num = channel_in + 88;
        break;

      default:
        break;
    }

  return channel_num;
}

/****************************************************************************
 * General Functions
 ****************************************************************************/

/****************************************************************************
 * Name: fc7300_cap_ftu_timer
 *
 * Description:
 *   This method is called in setclock to actually execution of MOD.
 *
 * Input Parameters:
 *   priv - A pointer to private structure.
 *   freq - Actually ignored, all input freq will be regarded as 0xFFFF.
 *          Value "0" is regarded as close clock.
 *
 * Returned Value:
 *   Always returned OK.
 *
 ****************************************************************************/

static int fc7300_cap_ftu_timer(const struct fc7300_cap_priv_s *priv,
                                uint32_t freq)
{
  uint32_t modulo;
  uint32_t regval;
  int ret = OK;

  if (freq)
    {
      modulo = 65535;
    }
  else
    {
      modulo = 0;
    }

  /* Disable FTU and reset CNT before writing MOD and PS */

  regval = cap_getreg32(priv, FC7300_FTU_SC_OFFSET);
  regval &= ~(FTU_SC_CLKS_MASK);
  cap_putreg32(priv, FC7300_FTU_SC_OFFSET, regval);

  cap_putreg32(priv, FC7300_FTU_CNT_OFFSET, 0);

  /* Set the modulo value */

  cap_putreg32(priv, FC7300_FTU_MOD_OFFSET, (uint16_t)modulo);

  /* Set filter */

  regval = cap_getreg32(priv, FC7300_FTU_SC_OFFSET);
  regval = (regval & ~FTU_SC_FLTPS_MASK) | FTU_SC_FLTPS(FTU_FLT_DIV_1) |
            FTU_SC_PS(7);
  cap_putreg32(priv, FC7300_FTU_SC_OFFSET, regval);

  /* Set ftu update MOD, CV OUTMASK register when FTU counter reaches MOD */

  regval = cap_getreg32(priv, FC7300_FTU_SYNC_OFFSET);
  regval |= FTU_SYNC_CNTMAX_MASK;
  cap_putreg32(priv, FC7300_FTU_SYNC_OFFSET, regval);

  /* Update ftu MOD, CV OUTMASK register via software or hardware sync */

  regval = cap_getreg32(priv, FC7300_FTU_MODE_OFFSET);
  regval |= FTU_MODE_FTUEN_MASK;
  cap_putreg32(priv, FC7300_FTU_MODE_OFFSET, regval);

  return ret;
}

/****************************************************************************
 * Name: fc7300_cap_ism_timer
 *
 * Description:
 *   This method is called in setclock to actually execution of MOD.
 *
 * Input Parameters:
 *   priv - A pointer to private structure.
 *   freq - Actually ignored, all input freq will be regarded as 0xFFFF.
 *          Value "0" is regarded as close clock.
 *   channel - Specified which channel to be configured.
 *
 * Returned Value:
 *   Always returned OK.
 *
 ****************************************************************************/

static int fc7300_cap_ism_timer(const struct fc7300_cap_priv_s *priv,
                                uint32_t freq, uint8_t channel)
{
  uint32_t regval;
  uint32_t tempvalue = 0;
  int ret = OK;

  if (freq == 0)
    {
      /* No need for ISM module */

      return OK;
    }

  tempvalue |= FC7300_ISM_FPC_CONFIG_FED(ISM_FPC_DELAY_FIXED0);
  tempvalue |= FC7300_ISM_FPC_CONFIG_FEG(ISM_FPC_DETECT_NO_FILTER);
  tempvalue |= FC7300_ISM_FPC_CONFIG_RED(ISM_FPC_DELAY_FIXED0);
  tempvalue |= FC7300_ISM_FPC_CONFIG_REG(ISM_FPC_DETECT_NO_FILTER);
  tempvalue |= FC7300_ISM_FPC_CONFIG_CMP(0);

  regval = cap_getreg32(priv, FC7300_ISM_FPC_CTRLn_OFFSET(channel));
  regval = (regval & ~FC7300_ISM_FPC_CTRL_IEN_MASK) |
            FC7300_ISM_FPC_CTRL_IEN(false);
  cap_putreg32(priv, FC7300_ISM_FPC_CTRLn_OFFSET(channel), regval);

  cap_putreg32(priv, FC7300_ISM_FPC_CONFIGn_OFFSET(channel), tempvalue);

  regval = cap_getreg32(priv, FC7300_ISM_FPC_CTRLn_OFFSET(channel));
  regval = (regval & ~FC7300_ISM_FPC_CTRL_EN_MASK) |
            FC7300_ISM_FPC_CTRL_EN(true);
  cap_putreg32(priv, FC7300_ISM_FPC_CTRLn_OFFSET(channel), regval);

  regval = cap_getreg32(priv, FC7300_ISM_CTRL_OFFSET);
  regval = (regval & ~FC7300_ISM_CTRL_EN_MASK) | FC7300_ISM_CTRL_EN(true);
  cap_putreg32(priv, FC7300_ISM_CTRL_OFFSET, regval);

  regval = cap_getreg32(priv, FC7300_ISM_E_CTRL_OFFSET);
  regval = (regval & ~FC7300_ISM_E_CTRL_EE_MASK) | FC7300_ISM_E_CTRL_EE(true);
  cap_putreg32(priv, FC7300_ISM_E_CTRL_OFFSET, regval);

  return ret;
}

/****************************************************************************
 * Name: fc7300_cap_setclock
 *
 * Description:
 *   This method is called in start or stop when the driver is opened.
 *
 * Input Parameters:
 *   dev - A pointer to device.
 *   hwunit - Specified hardware unit.
 *   freq - Actually ignored, all input freq will be regarded as 0xFFFF.
 *          Value "0" is regarded as close clock.
 *   channel - Specified which channel to be configured.
 *
 * Returned Value:
 *   OK if settings valid.
 *
 ****************************************************************************/

static int fc7300_cap_setclock(struct fc7300_cap_dev_s *dev, uint8_t hwunit,
                               uint32_t freq, uint8_t channel)
{
  const struct fc7300_cap_priv_s *priv = (const struct fc7300_cap_priv_s *)dev;
  int ret = OK;

  DEBUGASSERT(dev != NULL);
  if (hwunit == 12)
    {
      ret = fc7300_cap_ism_timer(priv, freq, channel);
    }
  else
    {
      ret = fc7300_cap_ftu_timer(priv, freq);
    }

  return ret;
}

/****************************************************************************
 * Name: fc7300_cap_setismchannel
 *
 * Description:
 *   This method is called in start when the driver is opened.
 *   The method should configure and initialize the device so that it is ready
 *   for use.
 *
 * Input Parameters:
 *   dev - A pointer to device.
 *   channel - Specified channel to be used as capture function.
 *   pincfg - Input pin attribute.
 *
 * Returned Value:
 *   OK if settings valid.
 *
 ****************************************************************************/

static int fc7300_cap_setismchannel(struct fc7300_cap_dev_s *dev,
                                    uint8_t channel, uint32_t pincfg,
                                    uint8_t trgsel, uint8_t mode)
{
  const struct fc7300_cap_priv_s *priv = (const struct fc7300_cap_priv_s *)dev;
  uint32_t regval;

  DEBUGASSERT(dev != NULL);
  DEBUGASSERT(channel < 16);

  /* Set the value of LAM_COUNTER to max */

  regval = cap_getreg32(priv, FC7300_ISM_LAM_COUNTERn_OFFSET(channel));
  regval = 15;
  cap_putreg32(priv, FC7300_ISM_LAM_COUNTERn_OFFSET(channel), regval);

  /* Set the value of LAM_CONFIG */

  regval = 0;
  if (mode == 0)
    {
      regval |= FC7300_ISM_LAM_CONFIG_RCS(0) |
                FC7300_ISM_LAM_CONFIG_MCS(channel) |
                FC7300_ISM_LAM_CONFIG_IVW(ISM_LAM_EVT_WIN_NON_INVERT) |
                FC7300_ISM_LAM_CONFIG_EDS(ISM_LAM_POS_CLEAR_POS_GATE) |
                FC7300_ISM_LAM_CONFIG_EWS(ISM_LAM_EVT_WIN_SEL_MON) |
                FC7300_ISM_LAM_CONFIG_RMS(ISM_LAM_RUN_FREE) |
                FC7300_ISM_LAM_CONFIG_MOS(ISM_LAM_SRC_FPC_MON) |
                FC7300_ISM_LAM_CONFIG_IVM(ISM_LAM_FPC_MON_NON_INVERT) |
                FC7300_ISM_LAM_CONFIG_IVR(ISM_LAM_FPC_REF_NON_INVERT);
    }
  else
    {
      regval |= FC7300_ISM_LAM_CONFIG_RCS(0) |
                FC7300_ISM_LAM_CONFIG_MCS(channel) |
                FC7300_ISM_LAM_CONFIG_IVW(ISM_LAM_EVT_WIN_NON_INVERT) |
                FC7300_ISM_LAM_CONFIG_EDS(ISM_LAM_POS_CLEAR_NEG_GATE) |
                FC7300_ISM_LAM_CONFIG_EWS(ISM_LAM_EVT_WIN_SEL_MON) |
                FC7300_ISM_LAM_CONFIG_RMS(ISM_LAM_RUN_FREE) |
                FC7300_ISM_LAM_CONFIG_MOS(ISM_LAM_SRC_FPC_MON) |
                FC7300_ISM_LAM_CONFIG_IVM(ISM_LAM_FPC_MON_NON_INVERT) |
                FC7300_ISM_LAM_CONFIG_IVR(ISM_LAM_FPC_REF_NON_INVERT);
    }

  cap_putreg32(priv, FC7300_ISM_LAM_CONFIGn_OFFSET(channel), regval);

  /* Set the EN of LAM_CTRL */

  regval = cap_getreg32(priv, FC7300_ISM_LAM_CTRLn_OFFSET(channel));
  regval = (regval & ~FC7300_ISM_LAM_CTRL_EN_MASK) |
            FC7300_ISM_LAM_CTRL_EN(true);
  cap_putreg32(priv, FC7300_ISM_LAM_CTRLn_OFFSET(channel), regval);

  /* Configure the capture input pin */

  fc7300_pinconfig(pincfg);
  fc7300_trgsel3_settriggersource(channel, trgsel);

  return OK;
}

/****************************************************************************
 * Name: fc7300_cap_setftuchannel
 *
 * Description:
 *   This method is called in start when the driver is opened.
 *   The method should configure and initialize the device so that it is
 *   ready for use.
 *
 * Input Parameters:
 *   dev - A pointer to device.
 *   channel - Specified channel to be used as capture function.
 *   pincfg - Input pin attribute.
 *
 * Returned Value:
 *   OK if settings valid.
 *
 ****************************************************************************/

static int fc7300_cap_setftuchannel(struct fc7300_cap_dev_s *dev,
                                    uint8_t channel, uint32_t pincfg)
{
  const struct fc7300_cap_priv_s *priv = (const struct fc7300_cap_priv_s *)dev;
  uint32_t csc = 0;
  uint32_t regval;

  DEBUGASSERT(dev != NULL);

  /* Disable FTU and reset CNT before writing MOD and PS */

  regval = cap_getreg32(priv, FC7300_FTU_SC_OFFSET);
  regval &= ~(FTU_SC_CLKS_MASK);
  cap_putreg32(priv, FC7300_FTU_SC_OFFSET, regval);

  /* clear update register by software or hardware sync */

  regval = cap_getreg32(priv, FC7300_FTU_MODE_OFFSET);
  regval &=  ~FTU_MODE_FTUEN_MASK;
  cap_putreg32(priv, FC7300_FTU_MODE_OFFSET, regval);

  /* Configure the capture input pin */

  fc7300_pinconfig(pincfg);

  /* Select the capture mode for specific */

  csc = FTU_CSC_ELSA_MASK | FTU_CSC_ELSB_MASK;

  switch (channel)
    {
        case 0:  /* CAP Mode configuration: Channel 0 */
          cap_putreg32(priv, FC7300_FTU_C0SC_OFFSET, csc);
          break;

        case 1:  /* CAP Mode configuration: Channel 1 */
          cap_putreg32(priv, FC7300_FTU_C1SC_OFFSET, csc);
          break;

        case 2:  /* CAP Mode configuration: Channel 2 */
          cap_putreg32(priv, FC7300_FTU_C2SC_OFFSET, csc);
          break;

        case 3:  /* CAP Mode configuration: Channel 3 */
          cap_putreg32(priv, FC7300_FTU_C3SC_OFFSET, csc);
          break;

        case 4:  /* CAP Mode configuration: Channel 4 */
          cap_putreg32(priv, FC7300_FTU_C4SC_OFFSET, csc);
          break;

        case 5:  /* CAP Mode configuration: Channel 5 */
          cap_putreg32(priv, FC7300_FTU_C5SC_OFFSET, csc);
          break;

        case 6:  /* CAP Mode configuration: Channel 6 */
          cap_putreg32(priv, FC7300_FTU_C6SC_OFFSET, csc);
          break;

        case 7:  /* CAP Mode configuration: Channel 7 */
          cap_putreg32(priv, FC7300_FTU_C7SC_OFFSET, csc);
          break;

        default:
          return -EINVAL;
    }

  /* Set ftu update MOD, CV OUTMASK register when FTU counter reaches MOD */

  regval = cap_getreg32(priv, FC7300_FTU_SYNC_OFFSET);
  regval |= FTU_SYNC_CNTMAX_MASK;
  cap_putreg32(priv, FC7300_FTU_SYNC_OFFSET, regval);

  /* Update ftu MOD, CV OUTMASK register via software or hardware sync */

  regval = cap_getreg32(priv, FC7300_FTU_MODE_OFFSET);
  regval |= FTU_MODE_FTUEN_MASK;
  cap_putreg32(priv, FC7300_FTU_MODE_OFFSET, regval);

  /* set ftu clk source */

  regval = cap_getreg32(priv, FC7300_FTU_SC_OFFSET);
  regval = (regval & ~(uint32_t)FTU_SC_CLKS_MASK) | FTU_SC_CLKS(FTU_EXTERNAL_CLK0);
  cap_putreg32(priv, FC7300_FTU_SC_OFFSET, regval);

  return OK;
}

/****************************************************************************
 * Name: fc7300_cap_setchannel
 *
 * Description:
 *   This method is called in start when the driver is opened.
 *   The method should configure and initialize the device so that it is ready
 *   for use.
 *
 * Input Parameters:
 *   dev - A pointer to device.
 *   hwunit - Specified hardware unit.
 *   channel - Specified channel to be used as capture function.
 *   pincfg - Input pin attribute.
 *
 * Returned Value:
 *   OK if settings valid.
 *
 ****************************************************************************/

static int fc7300_cap_setchannel(struct fc7300_cap_dev_s *dev, uint8_t hwunit,
                                 uint8_t channel, uint32_t pincfg,
                                 uint8_t trgsel, uint8_t mode)
{
  int ret = OK;

  DEBUGASSERT(dev != NULL);
  if (hwunit == 12)
    {
      ret = fc7300_cap_setismchannel(dev, channel, pincfg, trgsel, mode);
    }
  else
    {
      ret = fc7300_cap_setftuchannel(dev, channel, pincfg);
    }

  return ret;
}

/****************************************************************************
 * Name: fc7300_cap_getcapture
 *
 * Description:
 *   Get captured counter of the specific cahnnel.
 *
 * Input Parameters:
 *   dev - A pointer to device.
 *   channel - Specified channel to be read.
 *
 * Returned Value:
 *   Current counter value.
 *
 ****************************************************************************/

static uint32_t fc7300_cap_getismcapture(struct fc7300_cap_dev_s *dev,
                                      uint8_t channel)
{
  const struct fc7300_cap_priv_s *priv = (const struct fc7300_cap_priv_s *)dev;
  uint32_t regval;
  uint32_t cnt;

  DEBUGASSERT(dev != NULL);

  regval = cap_getreg32(priv, FC7300_ISM_LAM_STATUSn_OFFSET(channel));
  cnt = (regval & FC7300_ISM_LAM_STATUS_COUNT_MASK) >>
         FC7300_ISM_LAM_STATUS_COUNT_SHIFT;

  /* Clear the OVFL of LAM_STATUS */

  cap_putreg32(priv, FC7300_ISM_LAM_STATUSn_OFFSET(channel),
               FC7300_ISM_LAM_STATUS_OVFL_MASK);

  /* Clear the COUNT of LAM_STATUS */

  cap_putreg32(priv, FC7300_ISM_LAM_STATUSn_OFFSET(channel),
               FC7300_ISM_LAM_STATUS_COUNT_MASK);

  return cnt;
}

/****************************************************************************
 * Name: fc7300_cap_getftucapture
 *
 * Description:
 *   Get captured counter of the ftu specific cahnnel.
 *
 * Input Parameters:
 *   dev - A pointer to device.
 *   channel - Specified channel to be read.
 *
 * Returned Value:
 *   Current counter value.
 *
 ****************************************************************************/

static uint32_t fc7300_cap_getftucapture(struct fc7300_cap_dev_s *dev,
                                      uint8_t channel)
{
  const struct fc7300_cap_priv_s *priv = (const struct fc7300_cap_priv_s *)dev;
  uint32_t offset;

  DEBUGASSERT(dev != NULL);

  switch (channel)
    {
      case 0:
        offset = FC7300_FTU_C0V_OFFSET;
        break;

      case 1:
        offset = FC7300_FTU_C1V_OFFSET;
        break;

      case 2:
        offset = FC7300_FTU_C2V_OFFSET;
        break;

      case 3:
        offset = FC7300_FTU_C3V_OFFSET;
        break;

      case 4:
        offset = FC7300_FTU_C4V_OFFSET;
        break;

      case 5:
        offset = FC7300_FTU_C5V_OFFSET;
        break;

      case 6:
        offset = FC7300_FTU_C6V_OFFSET;
        break;

      case 7:
        offset = FC7300_FTU_C7V_OFFSET;
        break;

      default:
        return ERROR;
    }

  return cap_getreg32(priv, offset);
}

/****************************************************************************
 * Name: fc7300_cap_getcapture
 *
 * Description:
 *   Get captured counter of the ftu/ism specific cahnnel.
 *
 * Input Parameters:
 *   dev - A pointer to device.
 *   hwunit - Specified hardware unit.
 *   channel - Specified channel to be read.
 *
 * Returned Value:
 *   Current counter value.
 *
 ****************************************************************************/

static uint32_t fc7300_cap_getcapture(struct fc7300_cap_dev_s *dev,
                                      uint8_t hwunit, uint8_t channel)
{
  uint32_t cnt;

  DEBUGASSERT(dev != NULL);

  if (hwunit != 12)
    {
      cnt = fc7300_cap_getftucapture(dev, channel);
    }
  else
    {
      cnt = fc7300_cap_getismcapture(dev, channel);
    }

  return cnt;
}

/****************************************************************************
 * Name: fc7300_cap_getinputlevel
 *
 * Description:
 *   Get captured input level of the specific cahnnel.
 *
 * Input Parameters:
 *   dev - A pointer to device.
 *   channel - Specified channel to be read.
 *
 * Returned Value:
 *   Input high level or low level.
 *
 ****************************************************************************/

static enum ftu_inputleveltype_e fc7300_cap_getinputlevel
                               (struct fc7300_cap_dev_s *dev, uint8_t channel)
{
  const struct fc7300_cap_priv_s *priv = (const struct fc7300_cap_priv_s *)dev;
  uint32_t offset;
  uint32_t inputlevel;

  DEBUGASSERT(dev != NULL);

  switch (channel)
    {
      case 0:
        offset = FC7300_FTU_C0SC_OFFSET;
        break;

      case 1:
        offset = FC7300_FTU_C1SC_OFFSET;
        break;

      case 2:
        offset = FC7300_FTU_C2SC_OFFSET;
        break;

      case 3:
        offset = FC7300_FTU_C3SC_OFFSET;
        break;

      case 4:
        offset = FC7300_FTU_C4SC_OFFSET;
        break;

      case 5:
        offset = FC7300_FTU_C5SC_OFFSET;
        break;

      case 6:
        offset = FC7300_FTU_C6SC_OFFSET;
        break;

      case 7:
        offset = FC7300_FTU_C7SC_OFFSET;
        break;

      default:
        return FTU_INPUT_ERROR;
    }

  inputlevel = (cap_getreg32(priv, offset) & FTU_CSC_CHIS_MASK);
  return (0u == inputlevel) ? FTU_INPUT_LOW : FTU_INPUT_HIGH;
}

/****************************************************************************
 * Device Structures, Instantiation
 ****************************************************************************/

/****************************************************************************
 * Public Function - Initialization
 ****************************************************************************/

/****************************************************************************
 * Name: fc7300_cap_initpriv
 *
 * Description:
 *   Power-up timer and get its structure.
 *
 * Input Parameters:
 *   channel - A number identifying the timer channel use.
 *     The number of valid timer channels varies with the FC7300 MCU and MCU
 *     family but is somewhere in the range of {0,..,16}.
 *
 * Returned Value:
 *   On success, a pointer to the FC7300 capture private driver returned.
 *   NULL is returned on any failure.
 *
 ****************************************************************************/

struct fc7300_cap_dev_s *fc7300_cap_initpriv(uint8_t channel)
{
  uint8_t timer;
  if (channel < 16)
    {
      timer = 0;
    }
  else
    {
      timer = (channel - 16) / 8 + 1;
    }

  const struct fc7300_cap_priv_s *priv = fc7300_cap_get_priv(timer);

  return (struct fc7300_cap_dev_s *)priv;
}

/****************************************************************************
 * Name: cap_getsignalduration
 *
 * Description:
 *   Calculate duration between current and start.
 *
 * Input Parameters:
 *   dev - A pointer to device.
 *   current - current ticks.
 *   start - beginning ticks.
 *
 * Returned Value:
 *   Always success, duration returned.
 *
 ****************************************************************************/

#ifdef FC7300_CAP_OVERFLOW_USED
uint32_t cap_getsignalduration(struct fc7300_cap_dev_s *dev, uint32_t current,
                               uint32_t start, uint32_t *ofcnt)
#else
uint32_t cap_getsignalduration(struct fc7300_cap_dev_s *dev, uint32_t current,
                               uint32_t start)
#endif
{
  const struct fc7300_cap_priv_s *priv = (const struct fc7300_cap_priv_s *)dev;
  uint32_t duration;
  uint32_t maxvalue;

  maxvalue = cap_getreg32(priv, FC7300_FTU_MOD_OFFSET);

  if (current > start)
    {
      duration = current - start;
    }
  else
    {
      /* overflow */

      duration = maxvalue - start + current + 1u;
#ifdef FC7300_CAP_OVERFLOW_USED
      if (0u != *ofcnt)
        {
          *ofcnt -= 1u;
        }
      else
        {
          /* Miss overflow */
        }
#endif
    }

  return duration;
}
#endif /* CONFIG_FC7300_FTUX_CAP */
