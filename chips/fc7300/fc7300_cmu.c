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
#include "fc7300_cmu.h"
#include "fc7300_cmu_user.h"
#include "hardware/fc7300_cmu.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#define CMU_INTERRUPT_TYPE_NONE   0U      /* No CMU interrupt. */
#define CMU_INTERRUPT_TYPE_LOC    1U      /* CMU_ST[LOC] bit, Monitor clock is loss. */
#define CMU_INTERRUPT_TYPE_MIS    2U      /* CMU_ST[MIS] bit, Monitor clock is mismatched. */

/****************************************************************************
 * Private Types
 ****************************************************************************/

/* CMU Device Private Data */

struct fc7300_cmu_config_s
{
  uint32_t reg_base;        /* CMU register base address */
  uint32_t irq;             /* Interrupt ID */
  uint32_t irq_priority;    /* Interrupt priority */
  uint32_t instance_no;     /* CMU instance number */
  uint32_t ctrl;            /* CMU Control */
  uint32_t ref_window;      /* Reference Window */
  uint32_t min_threshold;   /* Minimum Threshold */
  uint32_t max_threshold;   /* Maximum Threshold */
};

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

static inline uint32_t
fc7300_cmu_getreg(struct fc7300_cmu_config_s *priv, uint16_t offset);
static inline void fc7300_cmu_putreg(struct fc7300_cmu_config_s *priv,
                                     uint16_t offset, uint32_t value);
static int fc7300_cmu_isr(int irq, void *context, void *arg);

/****************************************************************************
 * Private Data
 ****************************************************************************/

#if defined(CONFIG_FC7300_CMU0)
static struct fc7300_cmu_config_s g_cmu0_config =
{
  .reg_base      = FC7300_CMU0_BASE,
  .irq           = FC7300_IRQ_CMU0,
  .irq_priority  = NVIC_CMU0_PRIORITY_DEFAULT,
  .instance_no   = 0,
  .ctrl          = 0x0U |
#if defined(CONFIG_FC7300_CMU0_STOP_MODE_SUPPORT)
                   CMU_CTRL_STOP_EN |
#endif
#if defined(CONFIG_FC7300_CMU0_STANDBY_MODE_SUPPORT)
                   CMU_CTRL_LP_EN |
#endif
#if defined(CONFIG_FC7300_CMU0_RESTART_ENABLE)
                   CMU_CTRL_RESTART_EN |
#endif
#if defined(CONFIG_FC7300_CMU0_INTERRUPT_ENABLE)
                   CMU_CTRL_IRQ_EN |
#endif
                   CMU_CTRL_REF_DIV(CONFIG_FC7300_CMU0_REF_DIVISION_FACTOR) |
                   0x0U,
  .ref_window    = CONFIG_FC7300_CMU0_REF_WINDOW,
  .min_threshold = CONFIG_FC7300_CMU0_MIN_THRESHOLD,
  .max_threshold = CONFIG_FC7300_CMU0_MAX_THRESHOLD,
};
#endif /* defined(CONFIG_FC7300_CMU0) */

#if defined(CONFIG_FC7300_CMU1)
static struct fc7300_cmu_config_s g_cmu1_config =
{
  .reg_base      = FC7300_CMU1_BASE,
  .irq           = FC7300_IRQ_CMU1,
  .irq_priority  = NVIC_CMU1_PRIORITY_DEFAULT,
  .instance_no   = 1,
  .ctrl          = 0x0U |
#if defined(CONFIG_FC7300_CMU1_STOP_MODE_SUPPORT)
                   CMU_CTRL_STOP_EN |
#endif
#if defined(CONFIG_FC7300_CMU1_STANDBY_MODE_SUPPORT)
                   CMU_CTRL_LP_EN |
#endif
#if defined(CONFIG_FC7300_CMU1_RESTART_ENABLE)
                   CMU_CTRL_RESTART_EN |
#endif
#if defined(CONFIG_FC7300_CMU1_INTERRUPT_ENABLE)
                   CMU_CTRL_IRQ_EN |
#endif
                   CMU_CTRL_REF_DIV(CONFIG_FC7300_CMU1_REF_DIVISION_FACTOR) |
                   0x0U,
  .ref_window    = CONFIG_FC7300_CMU1_REF_WINDOW,
  .min_threshold = CONFIG_FC7300_CMU1_MIN_THRESHOLD,
  .max_threshold = CONFIG_FC7300_CMU1_MAX_THRESHOLD,
};
#endif /* defined(CONFIG_FC7300_CMU1) */

#if defined(CONFIG_FC7300_CMU2)
static struct fc7300_cmu_config_s g_cmu2_config =
{
  .reg_base      = FC7300_CMU2_BASE,
  .irq           = FC7300_IRQ_CMU2,
  .irq_priority  = NVIC_CMU2_PRIORITY_DEFAULT,
  .instance_no   = 2,
  .ctrl          = 0x0U |
#if defined(CONFIG_FC7300_CMU2_STOP_MODE_SUPPORT)
                   CMU_CTRL_STOP_EN |
#endif
#if defined(CONFIG_FC7300_CMU2_STANDBY_MODE_SUPPORT)
                   CMU_CTRL_LP_EN |
#endif
#if defined(CONFIG_FC7300_CMU2_RESTART_ENABLE)
                   CMU_CTRL_RESTART_EN |
#endif
#if defined(CONFIG_FC7300_CMU2_INTERRUPT_ENABLE)
                   CMU_CTRL_IRQ_EN |
#endif
                   CMU_CTRL_REF_DIV(CONFIG_FC7300_CMU2_REF_DIVISION_FACTOR) |
                   0x0U,
  .ref_window    = CONFIG_FC7300_CMU2_REF_WINDOW,
  .min_threshold = CONFIG_FC7300_CMU2_MIN_THRESHOLD,
  .max_threshold = CONFIG_FC7300_CMU2_MAX_THRESHOLD,
};
#endif /* defined(CONFIG_FC7300_CMU2) */

#if defined(CONFIG_FC7300_CMU3)
static struct fc7300_cmu_config_s g_cmu3_config =
{
  .reg_base      = FC7300_CMU3_BASE,
  .irq           = FC7300_IRQ_CMU3,
  .irq_priority  = NVIC_CMU3_PRIORITY_DEFAULT,
  .instance_no   = 3,
  .ctrl          = 0x0U |
#if defined(CONFIG_FC7300_CMU3_STOP_MODE_SUPPORT)
                   CMU_CTRL_STOP_EN |
#endif
#if defined(CONFIG_FC7300_CMU3_STANDBY_MODE_SUPPORT)
                   CMU_CTRL_LP_EN |
#endif
#if defined(CONFIG_FC7300_CMU3_RESTART_ENABLE)
                   CMU_CTRL_RESTART_EN |
#endif
#if defined(CONFIG_FC7300_CMU3_INTERRUPT_ENABLE)
                   CMU_CTRL_IRQ_EN |
#endif
                   CMU_CTRL_REF_DIV(CONFIG_FC7300_CMU3_REF_DIVISION_FACTOR) |
                   0x0U,
  .ref_window    = CONFIG_FC7300_CMU3_REF_WINDOW,
  .min_threshold = CONFIG_FC7300_CMU3_MIN_THRESHOLD,
  .max_threshold = CONFIG_FC7300_CMU3_MAX_THRESHOLD,
};
#endif /* defined(CONFIG_FC7300_CMU3) */

#if defined(CONFIG_FC7300_CMU4)
static struct fc7300_cmu_config_s g_cmu4_config =
{
  .reg_base      = FC7300_CMU4_BASE,
  .irq           = FC7300_IRQ_CMU4,
  .irq_priority  = NVIC_CMU4_PRIORITY_DEFAULT,
  .instance_no   = 4,
  .ctrl          = 0x0U |
#if defined(CONFIG_FC7300_CMU4_STOP_MODE_SUPPORT)
                   CMU_CTRL_STOP_EN |
#endif
#if defined(CONFIG_FC7300_CMU4_STANDBY_MODE_SUPPORT)
                   CMU_CTRL_LP_EN |
#endif
#if defined(CONFIG_FC7300_CMU4_RESTART_ENABLE)
                   CMU_CTRL_RESTART_EN |
#endif
#if defined(CONFIG_FC7300_CMU4_INTERRUPT_ENABLE)
                   CMU_CTRL_IRQ_EN |
#endif
                   CMU_CTRL_REF_DIV(CONFIG_FC7300_CMU4_REF_DIVISION_FACTOR) |
                   0x0U,
  .ref_window    = CONFIG_FC7300_CMU4_REF_WINDOW,
  .min_threshold = CONFIG_FC7300_CMU4_MIN_THRESHOLD,
  .max_threshold = CONFIG_FC7300_CMU4_MAX_THRESHOLD,
};
#endif /* defined(CONFIG_FC7300_CMU4) */

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: fc7300_cmu_getreg
 *
 * Description:
 *   Get a 32-bit register value by offset
 *
 * Input Parameters:
 *   priv   - CMU device private data
 *   offset - Register offset with respect to the base address of the CMU
 *            peripheral
 *
 * Returned Value:
 *   The 32-bit value retrieved from the register
 *
 ****************************************************************************/

static inline uint32_t
fc7300_cmu_getreg(struct fc7300_cmu_config_s *priv, uint16_t offset)
{
  return getreg32(priv->reg_base + offset);
}

/****************************************************************************
 * Name: fc7300_cmu_putreg
 *
 * Description:
 *  Put a 32-bit register value by offset
 *
 * Input Parameters:
 *   priv   - CMU device private data
 *   offset - Register offset with respect to the base address of the CMU
 *            peripheral
 *   value  - The 32-bit value that should be put into the register
 *
 ****************************************************************************/

static inline void fc7300_cmu_putreg(struct fc7300_cmu_config_s *priv,
                                     uint16_t offset, uint32_t value)
{
  putreg32(value, priv->reg_base + offset);
}

/****************************************************************************
 * Name: fc7300_cmu_isr
 *
 * Description:
 *   Common CMU interrupt service routine
 *
 ****************************************************************************/

static int fc7300_cmu_isr(int irq, void *context, void *arg)
{
  struct fc7300_cmu_config_s *priv = (struct fc7300_cmu_config_s *)arg;

  DEBUGASSERT(priv != NULL);

  int ret;
  uint32_t reg_value;

  reg_value = fc7300_cmu_getreg(priv, FC7300_CMU_ST_OFFSET);

  if (reg_value & CMU_ST_MIS)
    {
      ret = fc7300_cmu_user_callback(priv->instance_no,
                                     CMU_INTERRUPT_TYPE_MIS);
      if (ret == 0U)
        {
          fc7300_cmu_putreg(priv, FC7300_CMU_ST_OFFSET, CMU_ST_MIS);
        }
    }

  if (reg_value & CMU_ST_LOC)
    {
      ret = fc7300_cmu_user_callback(priv->instance_no,
                                     CMU_INTERRUPT_TYPE_LOC);
      if (ret == 0U)
        {
          fc7300_cmu_putreg(priv, FC7300_CMU_ST_OFFSET, CMU_ST_LOC);
        }
    }

  return OK;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: fc7300_cmu_init
 *
 * Description:
 *   Hardware level initializes the CMU Driver settings based on the
 * configuration.
 *
 ****************************************************************************/

void fc7300_cmu_init(void)
{
#if defined(CONFIG_FC7300_CMU0)
  fc7300_cmu_putreg(&g_cmu0_config,
                    FC7300_CMU_CTRL_OFFSET,
                    g_cmu0_config.ctrl);
  fc7300_cmu_putreg(&g_cmu0_config,
                    FC7300_CMU_MIN_OFFSET,
                    g_cmu0_config.min_threshold);
  fc7300_cmu_putreg(&g_cmu0_config,
                    FC7300_CMU_MAX_OFFSET,
                    g_cmu0_config.max_threshold);
  fc7300_cmu_putreg(&g_cmu0_config,
                    FC7300_CMU_REF_WINDOW_OFFSET,
                    g_cmu0_config.ref_window);
  fc7300_cmu_putreg(&g_cmu0_config,
                    FC7300_CMU_CTRL_OFFSET,
                    g_cmu0_config.ctrl | CMU_CTRL_ENABLE);
#endif /* defined(CONFIG_FC7300_CMU0) */

#if defined(CONFIG_FC7300_CMU1)
  fc7300_cmu_putreg(&g_cmu1_config,
                    FC7300_CMU_CTRL_OFFSET,
                    g_cmu1_config.ctrl);
  fc7300_cmu_putreg(&g_cmu1_config,
                    FC7300_CMU_MIN_OFFSET,
                    g_cmu1_config.min_threshold);
  fc7300_cmu_putreg(&g_cmu1_config,
                    FC7300_CMU_MAX_OFFSET,
                    g_cmu1_config.max_threshold);
  fc7300_cmu_putreg(&g_cmu1_config,
                    FC7300_CMU_REF_WINDOW_OFFSET,
                    g_cmu1_config.ref_window);
  fc7300_cmu_putreg(&g_cmu1_config,
                    FC7300_CMU_CTRL_OFFSET,
                    g_cmu1_config.ctrl | CMU_CTRL_ENABLE);
#endif /* defined(CONFIG_FC7300_CMU1) */

#if defined(CONFIG_FC7300_CMU2)
  fc7300_cmu_putreg(&g_cmu2_config,
                    FC7300_CMU_CTRL_OFFSET,
                    g_cmu2_config.ctrl);
  fc7300_cmu_putreg(&g_cmu2_config,
                    FC7300_CMU_MIN_OFFSET,
                    g_cmu2_config.min_threshold);
  fc7300_cmu_putreg(&g_cmu2_config,
                    FC7300_CMU_MAX_OFFSET,
                    g_cmu2_config.max_threshold);
  fc7300_cmu_putreg(&g_cmu2_config,
                    FC7300_CMU_REF_WINDOW_OFFSET,
                    g_cmu2_config.ref_window);
  fc7300_cmu_putreg(&g_cmu2_config,
                    FC7300_CMU_CTRL_OFFSET,
                    g_cmu2_config.ctrl | CMU_CTRL_ENABLE);
#endif /* defined(CONFIG_FC7300_CMU2) */

#if defined(CONFIG_FC7300_CMU3)
  fc7300_cmu_putreg(&g_cmu3_config,
                    FC7300_CMU_CTRL_OFFSET,
                    g_cmu3_config.ctrl);
  fc7300_cmu_putreg(&g_cmu3_config,
                    FC7300_CMU_MIN_OFFSET,
                    g_cmu3_config.min_threshold);
  fc7300_cmu_putreg(&g_cmu3_config,
                    FC7300_CMU_MAX_OFFSET,
                    g_cmu3_config.max_threshold);
  fc7300_cmu_putreg(&g_cmu3_config,
                    FC7300_CMU_REF_WINDOW_OFFSET,
                    g_cmu3_config.ref_window);
  fc7300_cmu_putreg(&g_cmu3_config,
                    FC7300_CMU_CTRL_OFFSET,
                    g_cmu3_config.ctrl | CMU_CTRL_ENABLE);
#endif /* defined(CONFIG_FC7300_CMU3) */

#if defined(CONFIG_FC7300_CMU4)
  fc7300_cmu_putreg(&g_cmu4_config,
                    FC7300_CMU_CTRL_OFFSET,
                    g_cmu4_config.ctrl);
  fc7300_cmu_putreg(&g_cmu4_config,
                    FC7300_CMU_MIN_OFFSET,
                    g_cmu4_config.min_threshold);
  fc7300_cmu_putreg(&g_cmu4_config,
                    FC7300_CMU_MAX_OFFSET,
                    g_cmu4_config.max_threshold);
  fc7300_cmu_putreg(&g_cmu4_config,
                    FC7300_CMU_REF_WINDOW_OFFSET,
                    g_cmu4_config.ref_window);
  fc7300_cmu_putreg(&g_cmu4_config,
                    FC7300_CMU_CTRL_OFFSET,
                    g_cmu4_config.ctrl | CMU_CTRL_ENABLE);
#endif /* defined(CONFIG_FC7300_CMU4) */
}

/****************************************************************************
 * Name: fc7300_cmu_irq_attach
 *
 * Description:
 *   Interrupt registration function after the system starts.
 *
 ****************************************************************************/

void fc7300_cmu_irq_attach(void)
{
#if defined(CONFIG_FC7300_CMU0)
  irq_attach(g_cmu0_config.irq, fc7300_cmu_isr, &g_cmu0_config);
#  ifdef CONFIG_ARCH_IRQPRIO
  up_prioritize_irq(g_cmu0_config.irq, g_cmu0_config.irq_priority);
#  endif
  up_enable_irq(g_cmu0_config.irq);
#endif /* defined(CONFIG_FC7300_CMU0) */

#if defined(CONFIG_FC7300_CMU1)
  irq_attach(g_cmu1_config.irq, fc7300_cmu_isr, &g_cmu1_config);
#  ifdef CONFIG_ARCH_IRQPRIO
  up_prioritize_irq(g_cmu1_config.irq, g_cmu1_config.irq_priority);
#  endif
  up_enable_irq(g_cmu1_config.irq);
#endif /* defined(CONFIG_FC7300_CMU1) */

#if defined(CONFIG_FC7300_CMU2)
  irq_attach(g_cmu2_config.irq, fc7300_cmu_isr, &g_cmu2_config);
#  ifdef CONFIG_ARCH_IRQPRIO
  up_prioritize_irq(g_cmu2_config.irq, g_cmu2_config.irq_priority);
#  endif
  up_enable_irq(g_cmu2_config.irq);
#endif /* defined(CONFIG_FC7300_CMU2) */

#if defined(CONFIG_FC7300_CMU3)
  irq_attach(g_cmu3_config.irq, fc7300_cmu_isr, &g_cmu3_config);
#  ifdef CONFIG_ARCH_IRQPRIO
  up_prioritize_irq(g_cmu3_config.irq, g_cmu3_config.irq_priority);
#  endif
  up_enable_irq(g_cmu3_config.irq);
#endif /* defined(CONFIG_FC7300_CMU3) */

#if defined(CONFIG_FC7300_CMU4)
  irq_attach(g_cmu4_config.irq, fc7300_cmu_isr, &g_cmu4_config);
#  ifdef CONFIG_ARCH_IRQPRIO
  up_prioritize_irq(g_cmu4_config.irq, g_cmu4_config.irq_priority);
#  endif
  up_enable_irq(g_cmu4_config.irq);
#endif /* defined(CONFIG_FC7300_CMU4) */
}

/****************************************************************************
 * Name: fc7300_cmu_injection
 *
 * Description:
 *   CMU error injection function used by EIM.
 *
 ****************************************************************************/

void fc7300_cmu_injection(uint8_t instance)
{
  struct fc7300_cmu_config_s *priv = NULL;

#if defined(CONFIG_FC7300_EIM_CMU0)
  if (instance == 0U)
    {
      priv = &g_cmu0_config;
    }
#endif /* defined(CONFIG_FC7300_EIM_CMU0) */

#if defined(CONFIG_FC7300_EIM_CMU1)
  if (instance == 1U)
    {
      priv = &g_cmu1_config;
    }
#endif /* defined(CONFIG_FC7300_EIM_CMU1) */

#if defined(CONFIG_FC7300_EIM_CMU2)
  if (instance == 2U)
    {
      priv = &g_cmu2_config;
    }
#endif /* defined(CONFIG_FC7300_EIM_CMU2) */

#if defined(CONFIG_FC7300_EIM_CMU3)
  if (instance == 3U)
    {
      priv = &g_cmu3_config;
    }
#endif /* defined(CONFIG_FC7300_EIM_CMU3) */

#if defined(CONFIG_FC7300_EIM_CMU4)
  if (instance == 4U)
    {
      priv = &g_cmu4_config;
    }
#endif /* defined(CONFIG_FC7300_EIM_CMU4) */

  if (priv != NULL)
    {
      //coverity[DEADCODE:SUPPRESS]
      uint32_t timeout = 0xFFFFFU;
      fc7300_cmu_putreg(priv,
                        FC7300_CMU_CTRL_OFFSET,
                        priv->ctrl);
      fc7300_cmu_putreg(priv,
                        FC7300_CMU_MAX_OFFSET,
                        priv->min_threshold + 1U);
      fc7300_cmu_putreg(priv,
                        FC7300_CMU_CTRL_OFFSET,
                        priv->ctrl | CMU_CTRL_ENABLE);
      while (timeout != 0U)
        {
          timeout--;
          if (fc7300_cmu_getreg(priv, FC7300_CMU_MAX_OFFSET) ==
              priv->max_threshold)
            {
              break;
            }
        }

      if (timeout == 0U)
        {
          fc7300_cmu_recovery(instance);
        }

      fc7300_cmu_putreg(priv,
                        FC7300_CMU_CTRL_OFFSET,
                        priv->ctrl | CMU_CTRL_ENABLE);
    }
}

/****************************************************************************
 * Name: fc7300_cmu_recovery
 *
 * Description:
 *   CMU error recovery function used by EIM.
 *
 ****************************************************************************/

void fc7300_cmu_recovery(uint8_t instance)
{
  struct fc7300_cmu_config_s *priv = NULL;

#if defined(CONFIG_FC7300_EIM_CMU0)
  if (instance == 0U)
    {
      priv = &g_cmu0_config;
    }
#endif /* defined(CONFIG_FC7300_EIM_CMU0) */

#if defined(CONFIG_FC7300_EIM_CMU1)
  if (instance == 1U)
    {
      priv = &g_cmu1_config;
    }
#endif /* defined(CONFIG_FC7300_EIM_CMU1) */

#if defined(CONFIG_FC7300_EIM_CMU2)
  if (instance == 2U)
    {
      priv = &g_cmu2_config;
    }
#endif /* defined(CONFIG_FC7300_EIM_CMU2) */

#if defined(CONFIG_FC7300_EIM_CMU3)
  if (instance == 3U)
    {
      priv = &g_cmu3_config;
    }
#endif /* defined(CONFIG_FC7300_EIM_CMU3) */

#if defined(CONFIG_FC7300_EIM_CMU4)
  if (instance == 4U)
    {
      priv = &g_cmu4_config;
    }
#endif /* defined(CONFIG_FC7300_EIM_CMU4) */

  if (priv != NULL)
    {
      //coverity[DEADCODE:SUPPRESS]
      if (fc7300_cmu_getreg(priv, FC7300_CMU_MAX_OFFSET) !=
              priv->max_threshold)
        {
          fc7300_cmu_putreg(priv,
                            FC7300_CMU_CTRL_OFFSET,
                            priv->ctrl);
          fc7300_cmu_putreg(priv,
                            FC7300_CMU_MAX_OFFSET,
                            priv->max_threshold);
        }
    }
}

/****************************************************************************
 * Name: fc7300_cmu_irq_handler
 *
 * Description:
 *   Interrupt entry function used by EIM.
 *
 ****************************************************************************/

void fc7300_cmu_irq_handler(uint8_t instance)
{
  struct fc7300_cmu_config_s *priv = NULL;

#if defined(CONFIG_FC7300_EIM_CMU0)
  if (instance == 0U)
    {
      priv = &g_cmu0_config;
    }
#endif /* defined(CONFIG_FC7300_EIM_CMU0) */

#if defined(CONFIG_FC7300_EIM_CMU1)
  if (instance == 1U)
    {
      priv = &g_cmu1_config;
    }
#endif /* defined(CONFIG_FC7300_EIM_CMU1) */

#if defined(CONFIG_FC7300_EIM_CMU2)
  if (instance == 2U)
    {
      priv = &g_cmu2_config;
    }
#endif /* defined(CONFIG_FC7300_EIM_CMU2) */

#if defined(CONFIG_FC7300_EIM_CMU3)
  if (instance == 3U)
    {
      priv = &g_cmu3_config;
    }
#endif /* defined(CONFIG_FC7300_EIM_CMU3) */

#if defined(CONFIG_FC7300_EIM_CMU4)
  if (instance == 4U)
    {
      priv = &g_cmu4_config;
    }
#endif /* defined(CONFIG_FC7300_EIM_CMU4) */

  if (priv != NULL)
    {
      //coverity[DEADCODE:SUPPRESS]
      fc7300_cmu_isr(0u, NULL, priv);
      fc7300_cmu_recovery(instance);
    }
}
