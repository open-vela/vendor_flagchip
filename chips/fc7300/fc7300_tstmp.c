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
#include <nuttx/spinlock.h>
#include <sys/types.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include <errno.h>
#include <debug.h>
#include <arch/board/board.h>
#include "chip.h"
#include "arm_internal.h"
#include "hardware/fc7300_tstmp.h"
#include "fc7300_tstmp.h"
#include "fc7300_irq.h"

#ifdef CONFIG_FC7300_TSTMP

/****************************************************************************
 * Private Types
 ****************************************************************************/

/* TSTMP Device hardware configuration */

struct fc7300_tstmp_config_s
{
  uint32_t reg_base;             /* TSTMP register base address */
  uint32_t irq;                  /* Interrupt ID */
  uint32_t irq_priority;         /* Interrupt priority */
};

/* TSTMP Device Private Data */

struct fc7300_tstmp_priv_s
{
  const struct fc7300_tstmp_ops_s *ops;
  const struct fc7300_tstmp_config_s *config;

  /* work State */

  uint64_t start_value;
  uint8_t ch_idx;
  uint8_t inited;

  /* user callback function */

  fc7300_tstmp_callback_t handler;
  void *arg;
};

/****************************************************************************
 * Private Function prototypes
 ****************************************************************************/

/* Register functions */

static inline uint32_t fc7300_tstmp_getreg(struct fc7300_tstmp_priv_s *priv,
                                           uint16_t offset);
static inline void fc7300_tstmp_putreg(struct fc7300_tstmp_priv_s *priv,
                                       uint16_t offset, uint32_t value);
static inline void fc7300_tstmp_modifyreg(struct fc7300_tstmp_priv_s *priv,
                                          uint16_t offset,
                                          uint32_t clearbits,
                                          uint32_t setbits);

/* TSTMP functions */

static inline uint64_t fc7300_tstmp_getval(struct fc7300_tstmp_priv_s *priv);

static inline uint32_t
fc7300_tstmp_readmoduleenable(struct fc7300_tstmp_priv_s *priv);

static void fc7300_tstmp_disable(struct fc7300_tstmp_priv_s *priv);

static int fc7300_tstmp_isr(int irq, void *context, void *arg);

static void fc7300_tstmp_start(struct fc7300_tstmp_dev_s *dev,
                               uint32_t usec,
                               fc7300_tstmp_callback_t handler,
                               void *arg);
static uint64_t fc7300_tstmp_stop(struct fc7300_tstmp_dev_s *dev);
static uint64_t fc7300_tstmp_getcounter(struct fc7300_tstmp_dev_s *dev);

/****************************************************************************
 * Private Data
 ****************************************************************************/

static uint64_t l_system_start_value = 0;
static uint8_t  l_system_init = false;

/* TSTMP interface */

static const struct fc7300_tstmp_ops_s g_tstmp_ops =
{
  .start      = fc7300_tstmp_start,
  .stop       = fc7300_tstmp_stop,
  .getcounter = fc7300_tstmp_getcounter,
};

/* TSTMP Device hardware configuration */

#if defined(CONFIG_FC7300_TSTMP1)
static const struct fc7300_tstmp_config_s g_tstmp1_config =
{
  .reg_base     = FC7300_TSTMP1_BASE,
  .irq          = FC7300_IRQ_TSTMP1,
  .irq_priority = NVIC_TSTMP1_PRIORITY_DEFAULT,
};
#elif defined(CONFIG_FC7300_TSTMP2)
static const struct fc7300_tstmp_config_s g_tstmp1_config =
{
  .reg_base     = FC7300_TSTMP2_BASE,
  .irq          = FC7300_IRQ_TSTMP2,
  .irq_priority = NVIC_TSTMP2_PRIORITY_DEFAULT,
};
#elif defined(CONFIG_FC7300_TSTMP3)
static const struct fc7300_tstmp_config_s g_tstmp1_config =
{
  .reg_base     = FC7300_TSTMP3_BASE,
  .irq          = FC7300_IRQ_TSTMP3,
  .irq_priority = NVIC_TSTMP3_PRIORITY_DEFAULT,
};
#endif

/* TSTMP Device Private Data */

#if defined(CONFIG_FC7300_TSTMP1_CH0) || \
    defined(CONFIG_FC7300_TSTMP2_CH0) || \
    defined(CONFIG_FC7300_TSTMP3_CH0)
static struct fc7300_tstmp_priv_s g_tstmp1_ch0_priv =
{
  .ops             = &g_tstmp_ops,
  .config          = &g_tstmp1_config,
  .ch_idx          = TSTMP_CHANNEL_0,
  .start_value     = 0,
  .inited          = false,
  .handler         = NULL,
  .arg             = NULL,
};
#endif

#if defined(CONFIG_FC7300_TSTMP1_CH1) || \
    defined(CONFIG_FC7300_TSTMP2_CH1) || \
    defined(CONFIG_FC7300_TSTMP3_CH1)
static struct fc7300_tstmp_priv_s g_tstmp1_ch1_priv =
{
  .ops             = &g_tstmp_ops,
  .config          = &g_tstmp1_config,
  .ch_idx          = TSTMP_CHANNEL_1,
  .start_value     = 0,
  .inited          = false,
  .handler         = NULL,
  .arg             = NULL,
};
#endif

#if defined(CONFIG_FC7300_TSTMP1_CH2) || \
    defined(CONFIG_FC7300_TSTMP2_CH2) || \
    defined(CONFIG_FC7300_TSTMP3_CH2)
static struct fc7300_tstmp_priv_s g_tstmp1_ch2_priv =
{
  .ops             = &g_tstmp_ops,
  .config          = &g_tstmp1_config,
  .ch_idx          = TSTMP_CHANNEL_2,
  .start_value     = 0,
  .inited          = false,
  .handler         = NULL,
  .arg             = NULL,
};
#endif

#if defined(CONFIG_FC7300_TSTMP1_CH3) || \
    defined(CONFIG_FC7300_TSTMP2_CH3) || \
    defined(CONFIG_FC7300_TSTMP3_CH3)
static struct fc7300_tstmp_priv_s g_tstmp1_ch3_priv =
{
  .ops             = &g_tstmp_ops,
  .config          = &g_tstmp1_config,
  .ch_idx          = TSTMP_CHANNEL_3,
  .start_value     = 0,
  .inited          = false,
  .handler         = NULL,
  .arg             = NULL,
};
#endif

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: fc7300_tstmp_getreg
 *
 * Description:
 *   Get a 32-bit register value by offset
 *
 * Input Parameters:
 *   priv   - TSTMP device private data
 *   offset - Register offset with respect to the base address of the
 *            TSTMP peripheral
 *
 * Returned Value:
 *   The 32-bit value retrieved from the register
 *
 ****************************************************************************/

static inline uint32_t fc7300_tstmp_getreg(struct fc7300_tstmp_priv_s *priv,
                                           uint16_t offset)
{
  return getreg32(priv->config->reg_base + offset);
}

/****************************************************************************
 * Name: fc7300_tstmp_putreg
 *
 * Description:
 *  Put a 32-bit register value by offset
 *
 * Input Parameters:
 *   priv   - TSTMP device private data
 *   offset - Register offset with respect to the base address of the
 *            TSTMP peripheral
 *   value  - The 32-bit value that should be put into the register
 *
 ****************************************************************************/

static inline void fc7300_tstmp_putreg(struct fc7300_tstmp_priv_s *priv,
                                       uint16_t offset, uint32_t value)
{
  putreg32(value, priv->config->reg_base + offset);
}

/****************************************************************************
 * Name: fc7300_tstmp_modifyreg
 *
 * Description:
 *   Modify a 32-bit register value by offset
 *
 * Input Parameters:
 *   priv      - TSTMP device private data
 *   offset    - Register offset with respect to the base address of the
 *               TSTMP peripheral
 *   clearbits - Bitmask with the bits that should be cleared (put to 0)
 *   setbits   - Bitmask with the bits that should be set (put to 1)
 *
 ****************************************************************************/

static inline void fc7300_tstmp_modifyreg(struct fc7300_tstmp_priv_s *priv,
                                          uint16_t offset,
                                          uint32_t clearbits,
                                          uint32_t setbits)
{
  modifyreg32(priv->config->reg_base + offset, clearbits, setbits);
}

/****************************************************************************
 * Name: fc7300_tstmp_getval
 *
 * Description:
 *   Return tstmp counter value
 *
 * Input Parameters:
 *   priv   - Device-specific state data
 *
 * Returned Value:
 *   tstmp counter value
 *
 ****************************************************************************/

static inline uint64_t fc7300_tstmp_getval(struct fc7300_tstmp_priv_s *priv)
{
  uint32_t vl1, vl2;
  uint64_t vh;
  irqstate_t irqflags;

  irqflags = enter_critical_section();

  vl1 = fc7300_tstmp_getreg(priv, FC7300_TSTMP_VALL_OFFSET);
  vh  = fc7300_tstmp_getreg(priv, FC7300_TSTMP_VALH_OFFSET);
  vl2 = fc7300_tstmp_getreg(priv, FC7300_TSTMP_VALL_OFFSET);

  if (vl2 < vl1)
    {
      vh = fc7300_tstmp_getreg(priv, FC7300_TSTMP_VALH_OFFSET);
    }

  leave_critical_section(irqflags);

  return (vh << 32) | vl2;
}

/****************************************************************************
 * Name: fc7300_tstmp_readmoduleenable
 *
 * Description:
 *   Return channel-specific state
 *
 * Input Parameters:
 *   priv   - Device-specific state data
 *
 * Returned Value:
 *   channel-specific state
 *
 ****************************************************************************/

static inline uint32_t
fc7300_tstmp_readmoduleenable(struct fc7300_tstmp_priv_s *priv)
{
  return (uint32_t)(fc7300_tstmp_getreg(priv,
                    FC7300_TSTMP_MOD_INTEN_OFFSET) &
                    TSTMP_MOD_INTEN_MOD_ENABLE(priv->ch_idx));
}

/****************************************************************************
 * Name: fc7300_tstmp_disable
 *
 * Description:
 *   Close channel-specific
 *
 * Input Parameters:
 *   priv   - Device-specific state data
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/

static void fc7300_tstmp_disable(struct fc7300_tstmp_priv_s *priv)
{
  if (fc7300_tstmp_readmoduleenable(priv))
    {
      fc7300_tstmp_modifyreg(priv,
                             FC7300_TSTMP_MOD_INTEN_OFFSET,
                             TSTMP_MOD_INTEN_MOD_ENABLE(priv->ch_idx), 0);
    }
}

/****************************************************************************
 * Name: fc7300_tstmp_start
 *
 * Description:
 *   Start channel-specific timer
 *
 * Input Parameters:
 *   dev     - Device-specific state data
 *   usec    - The timer count in us.
 *   handler - The function to call when when the oneshot timer expires.
 *   arg     - An opaque argument that will accompany the callback.
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/

static void fc7300_tstmp_start(struct fc7300_tstmp_dev_s *dev,
                               uint32_t usec,
                               fc7300_tstmp_callback_t handler, void *arg)
{
  struct fc7300_tstmp_priv_s *priv = (struct fc7300_tstmp_priv_s *)dev;

  if (fc7300_tstmp_readmoduleenable(priv) == 0)
    {
      /* Init count value */

      fc7300_tstmp_putreg(priv,
                          FC7300_TSTMP_MOD_SETVAL_OFFSET(priv->ch_idx),
                          usec);

      /* Clear interrupt flag */

      fc7300_tstmp_putreg(priv, FC7300_TSTMP_MOD_STATUS_OFFSET,
                          TSTMP_MOD_STATUS_MOD_MATCH(priv->ch_idx));

      priv->handler = handler;
      priv->arg = arg;

      /* Enable interrupt */

      fc7300_tstmp_modifyreg(priv, FC7300_TSTMP_MOD_INTEN_OFFSET, 0,
                             TSTMP_MOD_INTEN_MOD_INTEN(priv->ch_idx));

      /* Save the start value */

      priv->start_value = fc7300_tstmp_getval(priv);

      /* Enable timer */

      fc7300_tstmp_modifyreg(priv, FC7300_TSTMP_MOD_INTEN_OFFSET, 0,
                             TSTMP_MOD_INTEN_MOD_ENABLE(priv->ch_idx));
    }
}

/****************************************************************************
 * Name: fc7300_tstmp_stop
 *
 * Description:
 *   Close channel-specific timer
 *
 * Input Parameters:
 *   dev     - Device-specific state data
 *
 * Returned Value:
 *   The timer count elapsed time with us.
 *
 ****************************************************************************/

static uint64_t fc7300_tstmp_stop(struct fc7300_tstmp_dev_s *dev)
{
  struct fc7300_tstmp_priv_s *priv = (struct fc7300_tstmp_priv_s *)dev;
  uint64_t val = 0;
  uint64_t cv;

  if (fc7300_tstmp_readmoduleenable(priv))
    {
      /* Get current time value */

      cv = fc7300_tstmp_getval(priv);

      /* disable timer and interrupt */

      fc7300_tstmp_modifyreg(priv, FC7300_TSTMP_MOD_INTEN_OFFSET,
                             TSTMP_MOD_INTEN_MOD_ENABLE(priv->ch_idx) |
                             TSTMP_MOD_INTEN_MOD_INTEN(priv->ch_idx), 0);

      /* Clear interrupt flag */

      fc7300_tstmp_putreg(priv, FC7300_TSTMP_MOD_STATUS_OFFSET,
                          TSTMP_MOD_STATUS_MOD_MATCH(priv->ch_idx));

      val =  cv - priv->start_value;
    }

  return val;
}

/****************************************************************************
 * Name: fc7300_tstmp_getcounter
 *
 * Description:
 *   Get timer count elapsed time
 *
 * Input Parameters:
 *   dev     - Device-specific state data
 *
 * Returned Value:
 *   The timer count elapsed time with us.
 *
 ****************************************************************************/

static uint64_t fc7300_tstmp_getcounter(struct fc7300_tstmp_dev_s *dev)
{
  struct fc7300_tstmp_priv_s *priv = (struct fc7300_tstmp_priv_s *)dev;
  uint64_t cv;

  cv = fc7300_tstmp_getval(priv);

  return (cv - l_system_start_value);
}

#ifdef CONFIG_FC7300_TSTMP_IRQ_ATTACH_THREAD
/****************************************************************************
 * Name: fc7300_tstmp_isr_handle
 *
 * Description:
 *   Disable tstmpn interrupt.
 *
 * Input Parameters:
 *
 * Returned Value: void
 *
 ****************************************************************************/

static int fc7300_tstmp_isr_handle(int irq, void *regs, void *arg)
{
  up_disable_irq(irq);
  return IRQ_WAKE_THREAD;
}
#endif

/****************************************************************************
 * Name: fc7300_tstmp_isr
 *
 * Description:
 *   Common I2C interrupt service routine
 *
 * Input Parameters:
 *   irq     - Number of the IRQ that generated the interrupt
 *   context - Interrupt register state save info (architecture-specific)
 *   arg     - Specific callback parameters.
 *
 * Returned Value:
 *   Zero (OK) is returned on success; a negated errno value is returned
 *   on failure.
 *
 ****************************************************************************/

static int fc7300_tstmp_isr(int irq, void *context, void *arg)
{
  struct fc7300_tstmp_priv_s *priv = (struct fc7300_tstmp_priv_s *)arg;
  fc7300_tstmp_callback_t callback;
  void *cbarg;
  uint32_t int_mask;

  /* Check which channel has occur interrupt */

  int_mask = fc7300_tstmp_getreg(priv, FC7300_TSTMP_MOD_STATUS_OFFSET);

  if ((int_mask & 0x01) != 0)
    {
#if defined(CONFIG_FC7300_TSTMP1_CH0) || \
    defined(CONFIG_FC7300_TSTMP2_CH0) || \
    defined(CONFIG_FC7300_TSTMP3_CH0)
      priv = &g_tstmp1_ch0_priv;
#else
      priv = NULL;
#endif
    }
  else if ((int_mask & 0x02) != 0)
    {
#if defined(CONFIG_FC7300_TSTMP1_CH1) || \
    defined(CONFIG_FC7300_TSTMP2_CH1) || \
    defined(CONFIG_FC7300_TSTMP3_CH1)
      priv = &g_tstmp1_ch1_priv;
#else
      priv = NULL;
#endif
    }
  else if ((int_mask & 0x04) != 0)
    {
#if defined(CONFIG_FC7300_TSTMP1_CH2) || \
    defined(CONFIG_FC7300_TSTMP2_CH2) || \
    defined(CONFIG_FC7300_TSTMP3_CH2)
      priv = &g_tstmp1_ch2_priv;
#else
      priv = NULL;
#endif
    }
  else if ((int_mask & 0x08) != 0)
    {
#if defined(CONFIG_FC7300_TSTMP1_CH3) || \
    defined(CONFIG_FC7300_TSTMP2_CH3) || \
    defined(CONFIG_FC7300_TSTMP3_CH3)
      priv = &g_tstmp1_ch3_priv;
#else
      priv = NULL;
#endif
    }
  else
    {
      priv = NULL;
    }

  if (priv == NULL)
    {
#ifdef CONFIG_FC7300_TSTMP_IRQ_ATTACH_THREAD
      fc7300_clearpendingisr(irq);
      up_enable_irq(irq);
#endif
      return ERROR;
    }

  /* Clear interrupt and close tstmp channel */

  fc7300_tstmp_modifyreg(priv, FC7300_TSTMP_MOD_INTEN_OFFSET,
                         TSTMP_MOD_INTEN_MOD_ENABLE(priv->ch_idx) |
                         TSTMP_MOD_INTEN_MOD_INTEN(priv->ch_idx), 0);
  fc7300_tstmp_putreg(priv, FC7300_TSTMP_MOD_STATUS_OFFSET,
                      TSTMP_MOD_STATUS_MOD_MATCH(priv->ch_idx));

  if (priv->handler != NULL)
    {
      callback      = priv->handler;
      cbarg         = priv->arg;
      priv->handler = NULL;
      priv->arg     = NULL;

      callback(cbarg);
    }

#ifdef CONFIG_FC7300_TSTMP_IRQ_ATTACH_THREAD
  fc7300_clearpendingisr(irq);
  up_enable_irq(irq);
#endif
  return OK;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: fc7300_tstmp_init
 *
 * Description:
 *   Initialize tstmp hardware, ready for operation with defaults
 *
 * Input Parameters:
 *   timer    - Timer counter channel to be used.
 *
 * Returned Value:
 *   On success, a non-NULL instance of the TSTMP lower-half driver is
 *   returned.  NULL is return on any failure.
 *
 ****************************************************************************/

struct fc7300_tstmp_dev_s *fc7300_tstmp_init(int timer)
{
  struct fc7300_tstmp_priv_s *priv = NULL;

  /* Get structure and enable power */

  switch (timer)
    {
#if defined(CONFIG_FC7300_TSTMP1_CH0) || \
    defined(CONFIG_FC7300_TSTMP2_CH0) || \
    defined(CONFIG_FC7300_TSTMP3_CH0)
      case 0:
        priv = &g_tstmp1_ch0_priv;
        break;
#endif
#if defined(CONFIG_FC7300_TSTMP1_CH1) || \
    defined(CONFIG_FC7300_TSTMP2_CH1) || \
    defined(CONFIG_FC7300_TSTMP3_CH1)
      case 1:
        priv = &g_tstmp1_ch1_priv;
        break;
#endif
#if defined(CONFIG_FC7300_TSTMP1_CH2) || \
    defined(CONFIG_FC7300_TSTMP2_CH2) || \
    defined(CONFIG_FC7300_TSTMP3_CH2)
      case 2:
        priv = &g_tstmp1_ch2_priv;
        break;
#endif
#if defined(CONFIG_FC7300_TSTMP1_CH3) || \
    defined(CONFIG_FC7300_TSTMP2_CH3) || \
    defined(CONFIG_FC7300_TSTMP3_CH3)
      case 3:
        priv = &g_tstmp1_ch3_priv;
        break;
#endif
      default:
        return NULL;
    }

  /* Is device already allocated */

  if (priv->inited != false)
    {
      return NULL;
    }

  fc7300_tstmp_disable(priv);
  priv->inited = true;

  if (l_system_init == 0)
    {
      l_system_start_value = fc7300_tstmp_getval(priv);

      /* Attach ISRs */

#ifdef CONFIG_FC7300_TSTMP_IRQ_ATTACH_THREAD
      irq_attach_wqueue(priv->config->irq, fc7300_tstmp_isr_handle,
                        fc7300_tstmp_isr, priv,
                        isr_cfg[TSTMP_ISR_THREAD]);
#else
      irq_attach(priv->config->irq, fc7300_tstmp_isr, priv);
#endif
#ifdef CONFIG_ARCH_IRQPRIO
      up_prioritize_irq(priv->config->irq, priv->config->irq_priority);
#endif
      up_enable_irq(priv->config->irq);
    }

  /* Record which timers are initialized */

  l_system_init |= (0x1 << timer);

  return (struct fc7300_tstmp_dev_s *)priv;
}

/****************************************************************************
 * Name: fc7300_tstmp_deinit
 *
 * Description:
 *   De-initialize tstmp hardware
 *
 * Input Parameters:
 *   dev  - An pointer of struct fc7300_tstmp_dev_s which return by
 *          fc7300_tstmp_init()
 *
 * Returned Value:
 *   Zero (OK) is returned on success, a negated errno value is returned on
 *   any failure.
 *
 ****************************************************************************/

int fc7300_tstmp_deinit(struct fc7300_tstmp_dev_s * dev)
{
  struct fc7300_tstmp_priv_s *priv = (struct fc7300_tstmp_priv_s *)dev;

  /* Disable power */

  fc7300_tstmp_disable(priv);

  /* Mark it as free */

  priv->inited = false;

  /* Release which timers are initialized */

  l_system_init &= ~(0x1 << priv->ch_idx);

  /* Disable and detach interrupts when all timer are released */

  if (l_system_init == 0)
    {
      up_disable_irq(priv->config->irq);
      irq_detach(priv->config->irq);
    }

  return OK;
}

#endif /* CONFIG_FC7300_TSTMP */
