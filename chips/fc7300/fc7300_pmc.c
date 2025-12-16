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
#include "fc7300_pmc.h"
#include "fc7300_pmc_user.h"
#include "hardware/fc7300_pmc.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Private Types
 ****************************************************************************/

/* PMC Device Private Data */

struct fc7300_pmc_config_s
{
  uint32_t reg_base;        /* PMC register base address */
  uint32_t irq;             /* Interrupt ID */
  uint32_t irq_priority;    /* Interrupt priority */
  uint32_t config;
};

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

static inline uint32_t
fc7300_pmc_getreg(struct fc7300_pmc_config_s *priv, uint16_t offset);
static inline void fc7300_pmc_putreg(struct fc7300_pmc_config_s *priv,
                                     uint16_t offset, uint32_t value);
static int fc7300_pmc_isr(int irq, void *context, void *arg);

/****************************************************************************
 * Private Data
 ****************************************************************************/

static struct fc7300_pmc_config_s g_pmc_config =
{
  .reg_base     = FC7300_PMC_BASE,
  .irq          = FC7300_IRQ_PMC,
  .irq_priority = NVIC_PMC_PRIORITY_DEFAULT,
  .config       = 0x0U |
#if defined(CONFIG_FC7300_PMC_RPM_VDD2P5_ENABLE)
                  PMC_CONFIG_RPM_VDD2P5_EN |
#endif /* defined(CONFIG_FC7300_PMC_RPM_VDD2P5_ENABLE) */
#if defined(CONFIG_FC7300_PMC_V15_AUTOSW_ENABLE)
                  PMC_CONFIG_V15_AUTOSW |
#endif /* defined(CONFIG_FC7300_PMC_V15_AUTOSW_ENABLE) */
#if defined(CONFIG_FC7300_PMC_V15_CTRL_ENABLE)
                  PMC_CONFIG_V15_CTRL_EN |
#endif /* defined(CONFIG_FC7300_PMC_V15_CTRL_ENABLE) */
#if defined(CONFIG_FC7300_PMC_5VB_MON_ENABLE)
                  PMC_CONFIG_5VB_MON_EN |
#endif /* defined(CONFIG_FC7300_PMC_5VB_MON_ENABLE) */
#if defined(CONFIG_FC7300_PMC_HVD_INTERRUPT_ENABLE)
                  PMC_CONFIG_HVD_IE |
#endif /* defined(CONFIG_FC7300_PMC_HVD_INTERRUPT_ENABLE) */
#if defined(CONFIG_FC7300_PMC_LVD_INTERRUPT_ENABLE)
                  PMC_CONFIG_LVD_IE |
#endif /* defined(CONFIG_FC7300_PMC_LVD_INTERRUPT_ENABLE) */
#if defined(CONFIG_FC7300_PMC_V15_LOCK)
                  PMC_CONFIG_V15_LOCK |
#endif /* defined(CONFIG_FC7300_PMC_V15_LOCK) */
                  0x0U,
};

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: fc7300_pmc_getreg
 *
 * Description:
 *   Get a 32-bit register value by offset
 *
 * Input Parameters:
 *   priv   - PMC device private data
 *   offset - Register offset with respect to the base address of the PMC
 *            peripheral
 *
 * Returned Value:
 *   The 32-bit value retrieved from the register
 *
 ****************************************************************************/

static inline uint32_t
fc7300_pmc_getreg(struct fc7300_pmc_config_s *priv, uint16_t offset)
{
  return getreg32(priv->reg_base + offset);
}

/****************************************************************************
 * Name: fc7300_pmc_putreg
 *
 * Description:
 *  Put a 32-bit register value by offset
 *
 * Input Parameters:
 *   priv   - PMC device private data
 *   offset - Register offset with respect to the base address of the PMC
 *            peripheral
 *   value  - The 32-bit value that should be put into the register
 *
 ****************************************************************************/

static inline void fc7300_pmc_putreg(struct fc7300_pmc_config_s *priv,
                                     uint16_t offset, uint32_t value)
{
  putreg32(value, priv->reg_base + offset);
}

/****************************************************************************
 * Name: fc7300_pmc_isr
 *
 * Description:
 *   Common PMC interrupt service routine
 *
 ****************************************************************************/

static int fc7300_pmc_isr(int irq, void *context, void *arg)
{
  struct fc7300_pmc_config_s *priv = (struct fc7300_pmc_config_s *)arg;

  DEBUGASSERT(priv != NULL);

  uint32_t lvscr = fc7300_pmc_getreg(priv, FC7300_PMC_LVSCR_OFFSET);
  int ret;

  /* high voltage detection of 1.1V interrupt */

  if (lvscr & PMC_LVSCR_HVD1P1V_FLAG)
    {
      if (lvscr & PMC_LVSCR_HVD1P1V_STATUS)
        {
          ret = fc7300_pmc_user_callback(PMC_HVD_1P1V_HAPPEN);
        }
      else
        {
          ret = fc7300_pmc_user_callback(PMC_HVD_1P1V_RESUME);
        }

      if (ret == 0U)
        {
          fc7300_pmc_putreg(priv, FC7300_PMC_LVSCR_OFFSET,
                            PMC_LVSCR_HVD1P1V_FLAG);
        }
    }

  /* high voltage detection of 1.5V interrupt */

  else if (lvscr & PMC_LVSCR_HVD1P5V_FLAG)
    {
      if (lvscr & PMC_LVSCR_HVD1P5V_STATUS)
        {
          ret = fc7300_pmc_user_callback(PMC_HVD_1P5V_HAPPEN);
        }
      else
        {
          ret = fc7300_pmc_user_callback(PMC_HVD_1P5V_RESUME);
        }

      if (ret == 0U)
        {
          fc7300_pmc_putreg(priv, FC7300_PMC_LVSCR_OFFSET,
                            PMC_LVSCR_HVD1P5V_FLAG);
        }
    }

  /* high voltage detection of 2.5V interrupt */

  else if (lvscr & PMC_LVSCR_HVD2P5V_FLAG)
    {
      if (lvscr & PMC_LVSCR_HVD2P5V_STATUS)
        {
          ret = fc7300_pmc_user_callback(PMC_HVD_2P5V_HAPPEN);
        }
      else
        {
          ret = fc7300_pmc_user_callback(PMC_HVD_2P5V_RESUME);
        }

      if (ret == 0U)
        {
          fc7300_pmc_putreg(priv, FC7300_PMC_LVSCR_OFFSET,
                            PMC_LVSCR_HVD2P5V_FLAG);
        }
    }

  /* high voltage detection of 5V interrupt */

  else if (lvscr & PMC_LVSCR_HVD5V_FLAG)
    {
      if (lvscr & PMC_LVSCR_HVD5V_STATUS)
        {
          ret = fc7300_pmc_user_callback(PMC_HVD_5V_HAPPEN);
        }
      else
        {
          ret = fc7300_pmc_user_callback(PMC_HVD_5V_RESUME);
        }

      if (ret == 0U)
        {
          fc7300_pmc_putreg(priv, FC7300_PMC_LVSCR_OFFSET,
                            PMC_LVSCR_HVD5V_FLAG);
        }
    }

  /* high voltage detection of 5VB interrupt */

  else if (lvscr & PMC_LVSCR_HVD5VB_FLAG)
    {
      if (lvscr & PMC_LVSCR_HVD5VB_STATUS)
        {
          ret = fc7300_pmc_user_callback(PMC_HVD_5VB_HAPPEN);
        }
      else
        {
          ret = fc7300_pmc_user_callback(PMC_HVD_5VB_RESUME);
        }

      if (ret == 0U)
        {
          fc7300_pmc_putreg(priv, FC7300_PMC_LVSCR_OFFSET,
                            PMC_LVSCR_HVD5VB_FLAG);
        }
    }

  /* low voltage detection of 1.5V interrupt */

  else if (lvscr & PMC_LVSCR_LVD1P5V_FLAG)
    {
      if (lvscr & PMC_LVSCR_LVD1P5V_STATUS)
        {
          ret = fc7300_pmc_user_callback(PMC_LVD_1P5V_HAPPEN);
        }
      else
        {
          ret = fc7300_pmc_user_callback(PMC_LVD_1P5V_RESUME);
        }

      if (ret == 0U)
        {
          fc7300_pmc_putreg(priv, FC7300_PMC_LVSCR_OFFSET,
                            PMC_LVSCR_LVD1P5V_FLAG);
        }
    }

  /* low voltage detection of 5V interrupt */

  else if (lvscr & PMC_LVSCR_LVD5V_FLAG)
    {
      if (lvscr & PMC_LVSCR_LVD5V_STATUS)
        {
          ret = fc7300_pmc_user_callback(PMC_LVD_5V_HAPPEN);
        }
      else
        {
          ret = fc7300_pmc_user_callback(PMC_LVD_5V_RESUME);
        }

      if (ret == 0U)
        {
          fc7300_pmc_putreg(priv, FC7300_PMC_LVSCR_OFFSET,
                            PMC_LVSCR_LVD5V_FLAG);
        }
    }
  else
    {
      /* Do nothing. */
    }

  return OK;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: fc7300_pmc_init
 *
 * Description:
 *   Hardware level initializes the PMC Driver settings based on the
 * configuration.
 *
 ****************************************************************************/

void fc7300_pmc_init(void)
{
  /* Clear all voltage flag */

  fc7300_pmc_putreg(&g_pmc_config, FC7300_PMC_LVSCR_OFFSET,
                    PMC_LVSCR_HVD5V_FLAG | PMC_LVSCR_HVD5VB_FLAG |
                    PMC_LVSCR_HVD2P5V_FLAG | PMC_LVSCR_HVD1P1V_FLAG |
                    PMC_LVSCR_LVD5V_FLAG | PMC_LVSCR_LVD1P5V_FLAG |
                    PMC_LVSCR_HVD1P5V_FLAG | PMC_LVSCR_LVR5V_FPM_FLAG |
                    PMC_LVSCR_LVR5V_RPM_FLAG | PMC_LVSCR_LVR5VB_FPM_FLAG |
                    PMC_LVSCR_LVR5VB_RPM_FLAG | PMC_LVSCR_LVR2P5V_FPM_FLAG |
                    PMC_LVSCR_LVR2P5V_RPM_FLAG | PMC_LVSCR_LVR1P1V_FPM_FLAG |
                    PMC_LVSCR_LVR1P1V_RPM_FLAG | PMC_LVSCR_POR_FLAG);

  fc7300_pmc_putreg(&g_pmc_config,
                    FC7300_PMC_CONFIG_OFFSET,
                    g_pmc_config.config);
}

/****************************************************************************
 * Name: fc7300_pmc_irq_attach
 *
 * Description:
 *   Interrupt registration function after the system starts.
 *
 ****************************************************************************/

void fc7300_pmc_irq_attach(void)
{
  irq_attach(g_pmc_config.irq, fc7300_pmc_isr, &g_pmc_config);
#ifdef CONFIG_ARCH_IRQPRIO
  up_prioritize_irq(g_pmc_config.irq, g_pmc_config.irq_priority);
#endif
  up_enable_irq(g_pmc_config.irq);
}
