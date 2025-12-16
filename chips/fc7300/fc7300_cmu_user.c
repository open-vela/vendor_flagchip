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

#include <nuttx/safety/safety.h>
#include <nuttx/bits.h>

#include "fc7300_cmu.h"
#include "fc7300_cmu_user.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Private Types
 ****************************************************************************/

/* CMU lower half private data */

struct safety_cmu_priv_s
{
  struct safety_lowerhalf_s lower;         /* Must be first member */
  safety_handler_t          handler;       /* Registered callback handler */
  uint8_t                   channel_mask;  /* CMU channel number */
  FAR void                  *arg;          /* Callback argument */
  struct cmu_fault_data_s   fault[CMU_MAX_ERROR_NUM];
};

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

static int cmu_setup(FAR struct safety_lowerhalf_s *lower);
static int cmu_inject(FAR struct safety_lowerhalf_s *lower, FAR void *arg);
static int cmu_selftest(FAR struct safety_lowerhalf_s *lower);
static int cmu_set_callback(FAR struct safety_lowerhalf_s *lower,
                            safety_handler_t handler, FAR void *arg);

/****************************************************************************
 * Private Data
 ****************************************************************************/

static const struct safety_ops_s g_cmu_ops =
{
  .setup        = cmu_setup,
  .inject       = cmu_inject,
  .selftest     = cmu_selftest,
  .set_callback = cmu_set_callback,
};
static struct safety_cmu_priv_s g_cmu_lower;

/****************************************************************************
 * Private Functions
 ****************************************************************************/

static int cmu_setup(FAR struct safety_lowerhalf_s *lower)
{
  return OK;
}

static int cmu_inject(FAR struct safety_lowerhalf_s *lower, FAR void *arg)
{
  return OK;
}

static int cmu_selftest(FAR struct safety_lowerhalf_s *lower)
{
  FAR struct safety_cmu_priv_s *priv =  &g_cmu_lower;
  uint8_t channel_mask = 0;
  uint8_t unmatched_bits = 0;

#if defined(CONFIG_FC7300_EIM_CMU0) && \
    defined(CONFIG_FC7300_CMU0_INTERRUPT_ENABLE)
  channel_mask |= (1 << 0);
#endif

#if defined(CONFIG_FC7300_EIM_CMU1) && \
    defined(CONFIG_FC7300_CMU1_INTERRUPT_ENABLE)
  channel_mask |= (1 << 1);
#endif

#if defined(CONFIG_FC7300_EIM_CMU2) && \
    defined(CONFIG_FC7300_CMU2_INTERRUPT_ENABLE)
  channel_mask |= (1 << 2);
#endif

#if defined(CONFIG_FC7300_EIM_CMU3) && \
    defined(CONFIG_FC7300_CMU3_INTERRUPT_ENABLE)
  channel_mask |= (1 << 3);
#endif

#if defined(CONFIG_FC7300_EIM_CMU4) && \
    defined(CONFIG_FC7300_CMU4_INTERRUPT_ENABLE)
  channel_mask |= (1 << 4);
#endif

  unmatched_bits = channel_mask & ~(priv->channel_mask);

  if (unmatched_bits != 0)
    {
      priv->handler(priv->arg, priv->fault, 0,
                    CMU_MAX_ERROR_NUM *
                    sizeof(struct cmu_fault_data_s));
      return ERROR;
    }

  return 0;
}

static int cmu_set_callback(FAR struct safety_lowerhalf_s *lower,
                            safety_handler_t handler, FAR void *arg)
{
  FAR struct safety_cmu_priv_s *priv = (FAR struct safety_cmu_priv_s *)lower;

  /* Store callback handler */

  priv->handler = handler;
  return OK;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: fc7300_cmu_user_callback
 *
 * Description:
 *   User-level callback function after CMU interrupt.
 *
 * Input Parameters:
 *   channel    - The channel number where the error occurred.
 *   error_type - Error Type
 *               (0x1: Monitor clock is loss
 *                0x2: Monitor clock is mismatched)
 * Return Value:
 *   0: Clear interrupt flag by driver.
 *   1: Clear interrupt flag by user.
 *
 ****************************************************************************/

int fc7300_cmu_user_callback(
  uint8_t channel, uint8_t error_type)
{
  FAR struct safety_cmu_priv_s *priv = &g_cmu_lower;

  /* Package fault data */

  priv->fault[channel].cmu_idx        = channel;
  priv->fault[channel].cmu_fault_type = error_type;

  /* Report fault through safety core */

  if (priv->handler == NULL)
    {
      set_bit(channel, &priv->channel_mask);
    }
  else
    {
      return priv->handler(priv->arg, &priv->fault[channel],
                           channel * sizeof(struct cmu_fault_data_s),
                           sizeof(struct cmu_fault_data_s));
    }

  return 0;  /* Clear interrupt by driver */
}

int fc7300_cmu_safety_initialize(void)
{
  g_cmu_lower.lower.ops  = &g_cmu_ops;

  /* Register CMU safety monitor */

  return safety_register(&g_cmu_lower.lower, SAFETY_MODULE_CLOCK,
                         CMU_MAX_ERROR_NUM * sizeof(struct cmu_fault_data_s));
}
