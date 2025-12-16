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

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <debug.h>

#include <nuttx/safety/safety.h>
#include "fc7300_erm.h"
#include "fc7300_erm_user.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Private Types
 ****************************************************************************/

struct safety_erm_priv_s
{
  struct safety_lowerhalf_s lower;
  safety_handler_t          handler;       /* Registered callback handler */
  FAR void                  *arg;          /* Callback argument */
  uint8_t                   inject_count;  /* Inject count   */
  struct erm_fault_data_s   fault[ERM_MAX_ERROR_NUM];
};

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

static int erm_setup(FAR struct safety_lowerhalf_s *lower);
static int erm_selftest(FAR struct safety_lowerhalf_s *lower);
static int erm_set_callback(FAR struct safety_lowerhalf_s *lower,
                            safety_handler_t handler, FAR void *arg);

/****************************************************************************
 * Private Data
 ****************************************************************************/

static const struct safety_ops_s g_erm_ops =
{
  .setup        = erm_setup,
  .selftest     = erm_selftest,
  .set_callback = erm_set_callback,
};

static struct safety_erm_priv_s g_erm_lower;

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: erm_setup
 *
 * Description:
 *   Initialize the ERM safety module.
 *
 ****************************************************************************/

static int erm_setup(FAR struct safety_lowerhalf_s *lower)
{
  return OK;
}

/****************************************************************************
 * Name: erm_selftest
 *
 * Description:
 *   Check the ERM fault flag through error injection test
 *
 * Input Parameters:
 *   lower - Lower half driver instance
 *
 * Returned Value:
 *   Zero (OK) on success; a negated errno value on failure
 *
 ****************************************************************************/

static int erm_selftest(FAR struct safety_lowerhalf_s *lower)
{
  FAR struct safety_erm_priv_s *priv = &g_erm_lower;
  int ch_count  = 0;

#if defined(CONFIG_FC7300_EIM_ROM_ECC) && \
    (defined(CONFIG_FC7300_ERM_ROM_ECC_MODE_S) || \
    defined(CONFIG_FC7300_ERM_ROM_ECC_MODE_M) || \
    defined(CONFIG_FC7300_ERM_ROM_ECC_MODE_A))
  ++ch_count;
#endif

#if defined(CONFIG_FC7300_EIM_SRAM0) && \
    (defined(CONFIG_FC7300_ERM_SRAM0_ECC_MODE_S) || \
    defined(CONFIG_FC7300_ERM_SRAM0_ECC_MODE_M) || \
    defined(CONFIG_FC7300_ERM_SRAM0_ECC_MODE_A))
  ++ch_count;
#endif

#if defined(CONFIG_FC7300_EIM_SRAM1) && \
    (defined(CONFIG_FC7300_ERM_SRAM1_ECC_MODE_S) || \
    defined(CONFIG_FC7300_ERM_SRAM1_ECC_MODE_M) || \
    defined(CONFIG_FC7300_ERM_SRAM1_ECC_MODE_A))
  ++ch_count;
#endif

#if defined(CONFIG_FC7300_EIM_SRAM2) && \
    (defined(CONFIG_FC7300_ERM_SRAM2_ECC_MODE_S) || \
    defined(CONFIG_FC7300_ERM_SRAM2_ECC_MODE_M) || \
    defined(CONFIG_FC7300_ERM_SRAM2_ECC_MODE_A))
  ++ch_count;
#endif

#if defined(CONFIG_FC7300_EIM_CPU0_ITCM) && \
    (defined(CONFIG_FC7300_ERM_CPU0_ITCM_ECC_MODE_S) || \
    defined(CONFIG_FC7300_ERM_CPU0_ITCM_ECC_MODE_A))
  ++ch_count;
#endif

#if defined(CONFIG_FC7300_EIM_CPU0_DTCM0) && \
    (defined(CONFIG_FC7300_ERM_CPU0_DTCM0_ECC_MODE_S) || \
    defined(CONFIG_FC7300_ERM_CPU0_DTCM0_ECC_MODE_M) || \
    defined(CONFIG_FC7300_ERM_CPU0_DTCM0_ECC_MODE_A))
  ++ch_count;
#endif

#if defined(CONFIG_FC7300_EIM_CPU0_DTCM1) && \
    (defined(CONFIG_FC7300_ERM_CPU0_DTCM1_ECC_MODE_S) || \
    defined(CONFIG_FC7300_ERM_CPU0_DTCM1_ECC_MODE_M) || \
    defined(CONFIG_FC7300_ERM_CPU0_DTCM1_ECC_MODE_A))
  ++ch_count;
#endif

#if defined(CONFIG_FC7300_EIM_PFLASH0) && \
    (defined(CONFIG_FC7300_ERM_PFlash0_ECC_MODE_S) || \
    defined(CONFIG_FC7300_ERM_PFlash0_ECC_MODE_M) || \
    defined(CONFIG_FC7300_ERM_PFLASH0_ECC_MODE_A))
  ++ch_count;
#endif

#if defined(CONFIG_FC7300_EIM_PFLASH1) && \
    (defined(CONFIG_FC7300_ERM_PFlash1_ECC_MODE_S) || \
    defined(CONFIG_FC7300_ERM_PFlash1_ECC_MODE_M) || \
    defined(CONFIG_FC7300_ERM_PFLASH1_ECC_MODE_A))
  ++ch_count;
#endif

#if defined(CONFIG_FC7300_EIM_DFLASH) && \
    (defined(CONFIG_FC7300_ERM_DFlash_ECC_MODE_S) || \
    defined(CONFIG_FC7300_ERM_DFlash_ECC_MODE_M) || \
    defined(CONFIG_FC7300_ERM_DFLASH_ECC_MODE_A))
  ++ch_count;
#endif

#if (defined(CONFIG_FC7300_ERM_CPU0_ICACHE_ECC_MODE_S)|| \
    defined(CONFIG_FC7300_ERM_CPU0_ICACHE_ECC_MODE_M)|| \
    defined(CONFIG_FC7300_ERM_CPU0_ICACHE_ECC_MODE_A))
  #if defined(CONFIG_FC7300_EIM_CPU0ICACHE_DATA)
    ++ch_count;
  #endif
  #if defined(CONFIG_FC7300_EIM_CPU0ICACHE_TAG)
    ++ch_count;
  #endif
#endif

#if (defined(CONFIG_FC7300_ERM_CPU0_DCACHE_ECC_MODE_S)|| \
    defined(CONFIG_FC7300_ERM_CPU0_DCACHE_ECC_MODE_M)|| \
    defined(CONFIG_FC7300_ERM_CPU0_DCACHE_ECC_MODE_A))
  #if defined(CONFIG_FC7300_EIM_CPU0DCACHE_DATA)
    ++ch_count;
  #endif
  #if defined(CONFIG_FC7300_EIM_CPU0DCACHE_TAG)
    ++ch_count;
  #endif
#endif

  if(ch_count != priv->inject_count)
    {
      priv->handler(priv->arg, priv->fault, 0,
                    ERM_MAX_ERROR_NUM *
                    sizeof(struct erm_fault_data_s));
      return ERROR;
    }

  return OK;
}

/****************************************************************************
 * Name: erm_set_callback
 *
 * Description:
 *   Set the callback handler for the ERM safety module
 *
 * Input Parameters:
 *   lower   - Lower half driver instance
 *   handler - The callback handler to be set
 *
 * Returned Value:
 *   Zero (OK) on success; a negated errno value on failure
 *
 ****************************************************************************/

static int erm_set_callback(FAR struct safety_lowerhalf_s *lower,
                            safety_handler_t handler,FAR void *arg)
{
  FAR struct safety_erm_priv_s *priv = (FAR struct safety_erm_priv_s *)lower;
  priv->handler = handler;
  priv->arg     = arg;
  return OK;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: fc7300_erm_user_callback
 *
 * Description:
 *   User-level callback function after ERM interrupt.
 *
 * Input Parameters:
 *   channel    - The channel number where the error occurred.
 *   error_type - Error Type(0x1: Non-correctable, 0x2: single correction)
 *   error_addr - The address where the error occurred
 *
 * Return Value:
 *   0: Clear interrupt flag by driver.
 *   1: Clear interrupt flag by user.
 *
 ****************************************************************************/

int fc7300_erm_user_callback(uint8_t channel, uint8_t error_type,
                             uint32_t error_addr)
{
  FAR struct safety_erm_priv_s *priv = &g_erm_lower;

  /* Initialize fault data */

  if (channel >= ERM_MAX_ERROR_NUM)
    {
      serr("Invalid channel number: %d\n", channel);
      return -1;
    }

  priv->fault[channel].channel    = channel;
  priv->fault[channel].error_type = error_type;
  priv->fault[channel].error_addr = error_addr;

  if (priv->handler == NULL)
    {
      if (error_type == ERM_ERROR_TYPE_SINGLE||error_type == ERM_ERROR_TYPE_DOUBLE)
        {
          priv->inject_count++;
        }
    }
  else
    {
      return priv->handler(priv->arg, &priv->fault[channel],
                           channel * sizeof(struct erm_fault_data_s),
                           sizeof(struct erm_fault_data_s));
    }

  return 0;
}

/****************************************************************************
 * Name: fc7300_erm_safety_initialize
 *
 * Description:
 *   Initialize the ERM safety module.
 *
 * Return Value:
 *   OK: Success
 *   ERROR: Failure
 *
 ****************************************************************************/

int fc7300_erm_safety_initialize(void)
{
  int ret;
  g_erm_lower.lower.ops = &g_erm_ops;

  ret = safety_register(&g_erm_lower.lower,
                        SAFETY_MODULE_ERROR_REPORT,
                        ERM_MAX_ERROR_NUM * sizeof(struct erm_fault_data_s));
  if (ret < 0)
    {
      saerr("register register monitor failed!\n");
    }

  return ret;
}

