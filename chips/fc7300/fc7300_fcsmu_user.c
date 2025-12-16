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

#include "fc7300_fcsmu.h"
#include "fc7300_fcsmu_user.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Private Types
 ****************************************************************************/

/* FCSMU lower half private data */

struct safety_fcsmu_priv_s
{
  struct safety_lowerhalf_s lower;          /* Must be first member */
  safety_handler_t          handler;        /* Registered callback handler */
  uint32_t                  inject_count;   /* Inject count */
  uint32_t                  inject_ch_mask; /* Inject channel mask */
  FAR void                  *arg;           /* Callback argument */
  struct fcsmu_fault_data_s fault[FCSMU_MAX_ERROR_NUM];
};

static int fcsmu_setup(FAR struct safety_lowerhalf_s *lower);
static int fcsmu_selftest(FAR struct safety_lowerhalf_s *lower);
static int fcsmu_set_callback(FAR struct safety_lowerhalf_s *lower,
                              safety_handler_t handler, FAR void *arg);

/****************************************************************************
 * Private Data
 ****************************************************************************/

static const struct safety_ops_s g_fcsmu_ops =
{
  .setup        = fcsmu_setup,
  .selftest     = fcsmu_selftest,
  .set_callback = fcsmu_set_callback,
};

static struct safety_fcsmu_priv_s g_fcsmu_lower;

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: fcsmu_setup
 *
 * Description:
 *   FCSMU setup function
 *
 ****************************************************************************/

static int fcsmu_setup(FAR struct safety_lowerhalf_s *lower)
{
  return OK;
}

/****************************************************************************
 * Name: fcsmu_selftest
 *
 * Description:
 *   FCSMU self-test function
 *
 ****************************************************************************/

static int fcsmu_selftest(FAR struct safety_lowerhalf_s *lower)
{
  int ch_count  = 0;
  int old_count = 0;
  FAR struct safety_fcsmu_priv_s *priv = &g_fcsmu_lower;

#if defined(CONFIG_FC7300_EIM_CPU0_LOCKSTEP) && \
    defined(CONFIG_FC7300_FCSMU_CHANNEL_LS0_ERR)
  ++ch_count;
  if (!test_bit(4, &priv->inject_ch_mask))
    {
      return ERROR;
    }
  old_count = ch_count;
#endif

#if defined(CONFIG_FC7300_EIM_CPU1_LOCKSTEP) && \
    defined(CONFIG_FC7300_FCSMU_CHANNEL_LS1_ERR)
  ++ch_count;
  if (!test_bit(5, &priv->inject_ch_mask))
    {
      return ERROR;
    }
  old_count = ch_count;
#endif

#if defined(CONFIG_FC7300_FCSMU_CHANNEL_SYSRAM_CPU0_ERR)
  #if defined(CONFIG_FC7300_EIM_SRAM0)
  ++ch_count;
  #endif

  #if defined(CONFIG_FC7300_EIM_SRAM1)
  ++ch_count;
  #endif

  #if defined(CONFIG_FC7300_EIM_SRAM2)
  ++ch_count;
  #endif

  if (ch_count > old_count && (!test_bit(6, &priv->inject_ch_mask)))
    {
      return ERROR;
    }
  old_count = ch_count;
#endif

#if defined(CONFIG_FC7300_EIM_RAM_DECODER_MONITOR) && \
    defined(CONFIG_FC7300_FCSMU_CHANNEL_SYSRAM_NON_CPU_ERR)
  ++ch_count;
  if (!test_bit(9, &priv->inject_ch_mask))
    {
      return ERROR;
    }
  old_count = ch_count;
#endif

#if defined(CONFIG_FC7300_FCSMU_CHANNEL_SCM_CPU0_ERR)
  #if defined(CONFIG_FC7300_EIM_MAM0_S0)
  ++ch_count;
  #endif

  #if defined(CONFIG_FC7300_EIM_MAM0_S1)
  ++ch_count;
  #endif

  #if defined(CONFIG_FC7300_EIM_MAM0_S2)
  ++ch_count;
  #endif

  #if defined(CONFIG_FC7300_EIM_MAM0_S3)
  ++ch_count;
  #endif

  #if defined(CONFIG_FC7300_EIM_MAM1_S0)
  ++ch_count;
  #endif

  #if defined(CONFIG_FC7300_EIM_MAM1_S1)
  ++ch_count;
  #endif

  #if defined(CONFIG_FC7300_EIM_MAM1_S2)
  ++ch_count;
  #endif

  #if defined(CONFIG_FC7300_EIM_MAM1_S3)
  ++ch_count;
  #endif

  #if defined(CONFIG_FC7300_EIM_MAM2_S0)
  ++ch_count;
  #endif

  #if defined(CONFIG_FC7300_EIM_MAM2_S1)
  ++ch_count;
  #endif

  #if defined(CONFIG_FC7300_EIM_MAM2_S2)
  ++ch_count;
  #endif

  #if defined(CONFIG_FC7300_EIM_CPU0_AHBM)
  ++ch_count;
  #endif

  #if defined(CONFIG_FC7300_EIM_CPU0_AHBP)
  ++ch_count;
  #endif

  #if defined(CONFIG_FC7300_EIM_MAM1_S3_MONITOR)
  ++ch_count;
  #endif

  #if defined(CONFIG_FC7300_EIM_CPU0AHBM0_AHBM1_MONITOR)
  ++ch_count;
  #endif

  #if defined(CONFIG_FC7300_EIM_CPU0AHBP_AHBS_MONITOR)
  ++ch_count;
  #endif

  if (ch_count > old_count && (!test_bit(10, &priv->inject_ch_mask)))
    {
      return ERROR;
    }
  old_count = ch_count;
#endif

#if defined(CONFIG_FC7300_FCSMU_CHANNEL_SCM_CPU1_ERR)
  #if defined(CONFIG_FC7300_EIM_CPU1_AHBM)
  ++ch_count;
  #endif

  #if defined(CONFIG_FC7300_EIM_CPU1_AHBP)
  ++ch_count;
  #endif

  #if defined(CONFIG_FC7300_EIM_CPU1AHBM0_AHBM1_MONITOR)
  ++ch_count;
  #endif

  #if defined(CONFIG_FC7300_EIM_CPU1AHBP_AHBS_MONITOR)
  ++ch_count;
  #endif

  if (ch_count > old_count && (!test_bit(11, &priv->inject_ch_mask)))
    {
      return ERROR;
    }
  old_count = ch_count;
#endif

#if defined(CONFIG_FC7300_FCSMU_CHANNEL_SCM_NON_CPU_ERR)
  #if defined(CONFIG_FC7300_EIM_MAM1_S3_SLAVEDOWNSIZE)
  ++ch_count;
  #endif

  #if defined(CONFIG_FC7300_EIM_MAM1_S3_MASTERDOWNSIZE)
  ++ch_count;
  #endif

  #if defined(CONFIG_FC7300_EIM_MAM2_S2_SLAVEDOWNSIZE)
  ++ch_count;
  #endif

  #if defined(CONFIG_FC7300_EIM_MAM2_S2_MASTERDOWNSIZE)
  ++ch_count;
  #endif

  if (ch_count > old_count && (!test_bit(13, &priv->inject_ch_mask)))
    {
      return ERROR;
    }
  old_count = ch_count;
#endif

#if defined(CONFIG_FC7300_FCSMU_CHANNEL_CPU0_ECC_ERR)
  #if defined(CONFIG_FC7300_EIM_CPU0_ITCM)
  ++ch_count;
  #endif

  #if defined(CONFIG_FC7300_EIM_CPU0_DTCM0)
  ++ch_count;
  #endif

  #if defined(CONFIG_FC7300_EIM_CPU0_DTCM1)
  ++ch_count;
  #endif

  #if defined(CONFIG_FC7300_EIM_CPU0DCACHE_DATA)
  ch_count += 8;
  #endif

  #if defined(CONFIG_FC7300_EIM_CPU0DCACHE_TAG)
  ch_count += 4;
  #endif

  #if defined(CONFIG_FC7300_EIM_CPU0ICACHE_DATA)
  ++ch_count;
  #endif

  #if defined(CONFIG_FC7300_EIM_CPU0ICACHE_TAG)
  ++ch_count;
  #endif

  if (ch_count > old_count && (!test_bit(14, &priv->inject_ch_mask)))
    {
      return ERROR;
    }
  old_count = ch_count;
#endif

#if defined(CONFIG_FC7300_FCSMU_CHANNEL_CPU1_ECC_ERR)
  #if defined(CONFIG_FC7300_EIM_CPU1_ITCM)
  ++ch_count;
  #endif

  #if defined(CONFIG_FC7300_EIM_CPU1_DTCM0)
  ++ch_count;
  #endif

  #if defined(CONFIG_FC7300_EIM_CPU1_DTCM1)
  ++ch_count;
  #endif

  #if defined(CONFIG_FC7300_EIM_CPU1DCACHE_DATA)
  ch_count += 8;
  #endif

  #if defined(CONFIG_FC7300_EIM_CPU1DCACHE_TAG)
  ch_count += 4;
  #endif

  #if defined(CONFIG_FC7300_EIM_CPU1ICACHE_DATA)
  ++ch_count;
  #endif

  #if defined(CONFIG_FC7300_EIM_CPU1ICACHE_TAG)
  ++ch_count;
  #endif

  if (ch_count > old_count && (!test_bit(15, &priv->inject_ch_mask)))
    {
      return ERROR;
    }
  old_count = ch_count;
#endif

#if defined(CONFIG_FC7300_FCSMU_CHANNEL_CMU1_2_FAIL)
  #if defined(CONFIG_FC7300_EIM_CMU1)
  ++ch_count;
  #endif

  #if defined(CONFIG_FC7300_EIM_CMU2)
  ++ch_count;
  #endif
  if (ch_count > old_count && (!test_bit(20, &priv->inject_ch_mask)))
    {
      return ERROR;
    }
  old_count = ch_count;
#endif

#if defined(CONFIG_FC7300_EIM_CMU4) && \
    defined(CONFIG_FC7300_FCSMU_CHANNEL_CMU4_FAIL)
  ++ch_count;
  if (ch_count > old_count && !test_bit(19, &priv->inject_ch_mask))
    {
      return ERROR;
    }
  old_count = ch_count;

#endif

#if defined(CONFIG_FC7300_FCSMU_CHANNEL_FLASH_ECC)
  #if defined(CONFIG_FC7300_EIM_PFLASH0)
  ++ch_count;
  #endif

  #if defined(CONFIG_FC7300_EIM_PFLASH1)
  ++ch_count;
  #endif

  #if defined(CONFIG_FC7300_EIM_DFLASH)
  ++ch_count;
  #endif
if (ch_count > old_count && !test_bit(21, &priv->inject_ch_mask))
    {
      return ERROR;
    }
#endif

#if defined(CONFIG_FC7300_EIM_FMC) && \
    defined(CONFIG_FC7300_FCSMU_CHANNEL_FMC_ERR)
  ++ch_count;
  if ( !test_bit(28, &priv->inject_ch_mask))
    {
      return ERROR;
    }
#endif


  if (ch_count != priv->inject_count)
    {
      priv->handler(priv->arg,priv->fault, 0,
                    FCSMU_MAX_ERROR_NUM *
                    sizeof(struct fcsmu_fault_data_s));
      return ERROR;
    }

  return OK;
}

static int fcsmu_set_callback(FAR struct safety_lowerhalf_s *lower,
                          safety_handler_t handler, FAR void *arg)
{
  FAR struct safety_fcsmu_priv_s *priv =
      (FAR struct safety_fcsmu_priv_s *)lower;

  priv->handler = handler;
  priv->arg     = arg;
  return OK;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: fc7300_fcsmu_user_callback
 *
 * Description:
 *   User-level callback function after FCSMU interrupt.
 *
 * Input Parameters:
 *   channel_mask - The channel mask where the error occurred.
 *   error_type   - Error Type(0x1: fault, 0x2: warning, 0x3: timeout).
 *
 * Return Value:
 *   0: Clear interrupt flag by driver.
 *   1: Clear interrupt flag by user.
 *
 ****************************************************************************/

int fc7300_fcsmu_user_callback(uint32_t channel_mask, uint32_t error_type)
{
  FAR struct safety_fcsmu_priv_s *priv = &g_fcsmu_lower;
  int idx;

  for (idx = 0; idx < FCSMU_MAX_ERROR_NUM; ++idx)
    {
      if(channel_mask & (1 << idx))
        {
          priv->fault[idx].fcsmu_no       = idx;
          priv->fault[idx].fcsmu_err_type = error_type;
          if (priv->handler != NULL)
            {
              priv->handler(priv->arg, &priv->fault[idx],
                            idx * sizeof(struct fcsmu_fault_data_s),
                            sizeof(struct fcsmu_fault_data_s));
            }
          else
            {
              ++priv->inject_count;
              priv->inject_ch_mask |= 1 << idx;
            }
        }
    }
  return 0;
}

/****************************************************************************
 * Name: fc7300_fcsmu_safety_initialize
 *
 * Description:
 *   Initialize and register FCSMU safety monitor
 *
 * Return Value:
 *   Zero (OK) on success; a negated errno value on failure.
 *
 ****************************************************************************/

int fc7300_fcsmu_safety_initialize(void)
{
  g_fcsmu_lower.lower.ops = &g_fcsmu_ops;

  int ret = safety_register(&g_fcsmu_lower.lower, SAFETY_MODULE_SMU,
    FCSMU_MAX_ERROR_NUM *sizeof(struct fcsmu_fault_data_s));

  if (ret < 0)
    {
      saerr("Failed to register FCSMU safety monitor\n");
    }

  return ret;
}
