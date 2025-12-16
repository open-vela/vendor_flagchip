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
#include <assert.h>
#include <nuttx/hwspinlock/hwspinlock.h>

#include "chip.h"
#include "fc7300_mb.h"
#include "fc7300_hwspinlock.h"

#ifdef CONFIG_FC7300_HWSPINLOCK

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Private Types
 ****************************************************************************/

struct fc7300_hwspinlock_dev_s
{
  struct hwspinlock_dev_s dev;
  struct fc7300_mb_dev_s *pmb;
  uint8_t mbidx[CONFIG_FC7300_HWSPINLOCK_CHN_NUM];
};

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

static bool fc7300_hwspinlock_trylock(struct hwspinlock_dev_s *dev,
                                      int id, int priority);

static void fc7300_hwspinlock_unlock(struct hwspinlock_dev_s *dev, int id);

/****************************************************************************
 * Private Data
 ****************************************************************************/

static const struct hwspinlock_ops_s g_fc7300_hwspinlock_ops =
{
  .trylock = fc7300_hwspinlock_trylock,
  .unlock  = fc7300_hwspinlock_unlock,
};

static struct fc7300_hwspinlock_dev_s g_fc7300_hwspinlock_priv =
{
  .dev =
    {
      .ops = &g_fc7300_hwspinlock_ops,
    },
  .pmb = NULL,
  .mbidx =
    {
#if CONFIG_FC7300_HWSPINLOCK_CHN_NUM > 0
      CONFIG_FC7300_HWSPINLOCK_CHN0_ID,
#endif
#if CONFIG_FC7300_HWSPINLOCK_CHN_NUM > 1
      CONFIG_FC7300_HWSPINLOCK_CHN1_ID,
#endif
#if CONFIG_FC7300_HWSPINLOCK_CHN_NUM > 2
      CONFIG_FC7300_HWSPINLOCK_CHN2_ID,
#endif
#if CONFIG_FC7300_HWSPINLOCK_CHN_NUM > 3
      CONFIG_FC7300_HWSPINLOCK_CHN3_ID,
#endif
#if CONFIG_FC7300_HWSPINLOCK_CHN_NUM > 4
      CONFIG_FC7300_HWSPINLOCK_CHN4_ID,
#endif
#if CONFIG_FC7300_HWSPINLOCK_CHN_NUM > 5
      CONFIG_FC7300_HWSPINLOCK_CHN5_ID,
#endif
#if CONFIG_FC7300_HWSPINLOCK_CHN_NUM > 6
      CONFIG_FC7300_HWSPINLOCK_CHN6_ID,
#endif
#if CONFIG_FC7300_HWSPINLOCK_CHN_NUM > 7
      CONFIG_FC7300_HWSPINLOCK_CHN7_ID,
#endif
#if CONFIG_FC7300_HWSPINLOCK_CHN_NUM > 8
      CONFIG_FC7300_HWSPINLOCK_CHN8_ID,
#endif
#if CONFIG_FC7300_HWSPINLOCK_CHN_NUM > 9
      CONFIG_FC7300_HWSPINLOCK_CHN9_ID,
#endif
#if CONFIG_FC7300_HWSPINLOCK_CHN_NUM > 10
      CONFIG_FC7300_HWSPINLOCK_CHN10_ID,
#endif
#if CONFIG_FC7300_HWSPINLOCK_CHN_NUM > 11
      CONFIG_FC7300_HWSPINLOCK_CHN11_ID,
#endif
#if CONFIG_FC7300_HWSPINLOCK_CHN_NUM > 12
      CONFIG_FC7300_HWSPINLOCK_CHN12_ID,
#endif
#if CONFIG_FC7300_HWSPINLOCK_CHN_NUM > 13
      CONFIG_FC7300_HWSPINLOCK_CHN13_ID,
#endif
#if CONFIG_FC7300_HWSPINLOCK_CHN_NUM > 14
      CONFIG_FC7300_HWSPINLOCK_CHN14_ID,
#endif
#if CONFIG_FC7300_HWSPINLOCK_CHN_NUM > 15
      CONFIG_FC7300_HWSPINLOCK_CHN15_ID,
#endif
    }
};

/****************************************************************************
 * Private Functions
 ****************************************************************************/

static bool fc7300_hwspinlock_trylock(struct hwspinlock_dev_s *dev,
                                      int id, int priority)
{
  struct fc7300_hwspinlock_dev_s *priv =
    (struct fc7300_hwspinlock_dev_s *)dev;
  bool ret;

  DEBUGASSERT((priv != NULL) && (priv->pmb != NULL));
  DEBUGASSERT((id >= 0) && (id < CONFIG_FC7300_HWSPINLOCK_CHN_NUM));

  if (FC7300_MB_ACQUIRE_SEMA(priv->pmb, priv->mbidx[id]) == OK)
    {
      ret = true;
    }
  else
    {
      ret = false;
    }

  return ret;
}

static void fc7300_hwspinlock_unlock(struct hwspinlock_dev_s *dev, int id)
{
  struct fc7300_hwspinlock_dev_s *priv =
    (struct fc7300_hwspinlock_dev_s *)dev;

  DEBUGASSERT((priv != NULL) && (priv->pmb != NULL));
  DEBUGASSERT((id >= 0) && (id < CONFIG_FC7300_HWSPINLOCK_CHN_NUM));

  FC7300_MB_RELEASE_SEMA(priv->pmb, priv->mbidx[id]);
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

struct hwspinlock_dev_s *fc7300_hwspinlock_initialize(void)
{
  g_fc7300_hwspinlock_priv.pmb = fc7300_mb_getdev();

  return &g_fc7300_hwspinlock_priv.dev;
}

spinlock_t up_testset(FAR volatile spinlock_t *lock)
{
  spinlock_t ret;

  if (!hwspin_trylock(&g_fc7300_hwspinlock_priv.dev, 0, 0))
    {
      return SP_LOCKED;
    }

  ret = *lock;

  if (ret == SP_UNLOCKED)
    {
      *lock = SP_LOCKED;
      SP_DMB();
    }

  hwspin_unlock(&g_fc7300_hwspinlock_priv.dev, 0);
  return ret;
}

#endif
