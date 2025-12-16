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
#include "fc7300_tmu.h"
#include "fc7300_tmu_user.h"
#include "hardware/fc7300_tmu.h"
#include "fc7300_multicore.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Private Types
 ****************************************************************************/

/* TMU Device Private Data */

struct fc7300_tmu_config_s
{
  uint32_t reg_base;        /* TMU register base address */
  uint32_t irq;             /* Interrupt ID */
  uint32_t irq_priority;    /* Interrupt priority */
  uint32_t CTRL;
};

#define TMU_CTRL_REG_LOCK         0x0U          /* TF_CTRL and TV_CTRL are locked */
#define TMU_CTRL_REG_UNLOCK       0xA5A50001U   /* TF_CTRL and TV_CTRL are unlocked */

#define TMU_TF_MODE_NONE          0x00U         /* No flag has beed set */
#define TMU_TF_MODE_125F          0x01U         /* 125 flag has beed set */
#define TMU_TF_MODE_150F          0x02U         /* 125 and 150 flag has beed set */

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

static inline uint32_t
fc7300_tmu_getreg(struct fc7300_tmu_config_s *priv, uint16_t offset);
static inline void fc7300_tmu_putreg(struct fc7300_tmu_config_s *priv,
                                     uint16_t offset, uint32_t value);
static int fc7300_tmu_isr(int irq, void *context, void *arg);

/****************************************************************************
 * Private Data
 ****************************************************************************/

static struct fc7300_tmu_config_s g_tmu_config =
{
  .reg_base     = FC7300_TMU_BASE,
  .irq          = FC7300_IRQ_TMU,
  .irq_priority = NVIC_TMU_PRIORITY_DEFAULT,
  .CTRL         = 0x0U |
                  TMU_TF_CTRL_TF_START_CNT(7U) |
#if defined(CONFIG_FC7300_TMU_TF_CTRL_125F_INTERRUPT_ENABLE)
                  TMU_TF_CTRL_TF_125F_IE |
#endif
#if defined(CONFIG_FC7300_TMU_TF_CTRL_150F_INTERRUPT_ENABLE)
                  TMU_TF_CTRL_TF_150F_IE |
#endif
#if !defined(CONFIG_FC7300_TMU_TF_CTRL_HYSOFF_ENABLE)
                  TMU_TF_CTRL_TF_HYSOFF |
#endif
#if !defined(CONFIG_FC7300_TMU_TF_CTRL_FILT_ENABLE)
                  TMU_TF_CTRL_TF_FILT_BYP |
#endif
                  0x0U,
};

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: fc7300_tmu_getreg
 *
 * Description:
 *   Get a 32-bit register value by offset
 *
 * Input Parameters:
 *   priv   - TMU device private data
 *   offset - Register offset with respect to the base address of the TMU
 *            peripheral
 *
 * Returned Value:
 *   The 32-bit value retrieved from the register
 *
 ****************************************************************************/

static inline uint32_t
fc7300_tmu_getreg(struct fc7300_tmu_config_s *priv, uint16_t offset)
{
  return getreg32(priv->reg_base + offset);
}

/****************************************************************************
 * Name: fc7300_tmu_putreg
 *
 * Description:
 *  Put a 32-bit register value by offset
 *
 * Input Parameters:
 *   priv   - TMU device private data
 *   offset - Register offset with respect to the base address of the TMU
 *            peripheral
 *   value  - The 32-bit value that should be put into the register
 *
 ****************************************************************************/

static inline void fc7300_tmu_putreg(struct fc7300_tmu_config_s *priv,
                                     uint16_t offset, uint32_t value)
{
  putreg32(value, priv->reg_base + offset);
}

/****************************************************************************
 * Name: fc7300_tmu_isr
 *
 * Description:
 *   Common TMU interrupt service routine
 *
 ****************************************************************************/

static int fc7300_tmu_isr(int irq, void *context, void *arg)
{
  struct fc7300_tmu_config_s *priv = (struct fc7300_tmu_config_s *)arg;

  DEBUGASSERT(priv != NULL);

  int ret;
  uint32_t reg_value;
  uint8_t e_type;

  reg_value = fc7300_tmu_getreg(priv, FC7300_TMU_TF_STATUS_OFFSET);

  if (reg_value & TMU_TF_STATUS_TF_150F)
    {
      e_type = TMU_TF_MODE_150F;
    }
  else if (reg_value & TMU_TF_STATUS_TF_125F)
    {
      e_type = TMU_TF_MODE_125F;
    }
  else
    {
      e_type = TMU_TF_MODE_NONE;
    }

  ret = fc7300_tmu_user_callback(e_type);
  if (ret == 0U)
    {
      fc7300_tmu_putreg(&g_tmu_config, FC7300_TMU_TF_STATUS_OFFSET,
                        TMU_TF_STATUS_TF_125F | TMU_TF_STATUS_TF_150F);
    }

  return OK;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: fc7300_tmu_init
 *
 * Description:
 *   Hardware level initializes the TMU Driver settings based on the
 * configuration.
 *
 ****************************************************************************/

int fc7300_tmu_init(void)
{
  int ret = OK;
  uint32_t timeout = 65535U;

  fc7300_tmu_putreg(&g_tmu_config, FC7300_TMU_TF_CTRL_OFFSET,
                    g_tmu_config.CTRL);
  fc7300_tmu_putreg(&g_tmu_config, FC7300_TMU_TF_CTRL_OFFSET,
                    g_tmu_config.CTRL | TMU_TF_CTRL_TF_EN);
  fc7300_tmu_putreg(&g_tmu_config, FC7300_TMU_TV_CTRL_OFFSET,
                    TMU_TV_CTRL_TV_EN);

  while (timeout)
    {
      timeout--;

      if (fc7300_tmu_getreg(&g_tmu_config, FC7300_TMU_TF_STATUS_OFFSET) &
          TMU_TF_STATUS_TF_RDYF)
        {
          fc7300_tmu_putreg(&g_tmu_config, FC7300_TMU_TF_STATUS_OFFSET,
                            TMU_TF_STATUS_TF_RDYF);
          break;
        }
    }

  if (timeout == 0)
    {
      ret = ERROR;
    }

  return ret;
}

/****************************************************************************
 * Name: fc7300_tmu_irq_attach
 *
 * Description:
 *   Interrupt registration function after the system starts.
 *
 ****************************************************************************/

void fc7300_tmu_irq_attach(void)
{
  irq_attach(g_tmu_config.irq, fc7300_tmu_isr, &g_tmu_config);
#  ifdef CONFIG_ARCH_IRQPRIO
  up_prioritize_irq(g_tmu_config.irq, g_tmu_config.irq_priority);
#  endif
  up_enable_irq(g_tmu_config.irq);
}
