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
#include <time.h>
#include <assert.h>
#include <debug.h>
#include <nuttx/config.h>
#include <nuttx/irq.h>
#include <nuttx/kmalloc.h>
#include <nuttx/spinlock.h>
#include <nuttx/timers/oneshot.h>

#include "fc7300_oneshot.h"

/****************************************************************************
 * Private Types
 ****************************************************************************/

/* Oneshot Lowerhalf Private Data */

struct fc7300_oneshot_lowerhalf_priv_s
{
  /* This is the part of the lower half driver that is visible to the upper-
   * half client of the driver.  This must be the first thing in this
   * structure so that pointers to struct oneshot_lowerhalf_s are cast
   * compatible to struct fc7300_oneshot_lowerhalf_priv_s and vice versa.
   */

  const struct oneshot_operations_s *ops;

  /* Private lower half data follows */

  struct fc7300_oneshot_s oneshot; /* FC7300-specific oneshot state */
};

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

static void fc7300_oneshot_handler(void *arg);

static int fc7300_max_delay(struct oneshot_lowerhalf_s *lower,
                            struct timespec *ts);
static int fc7300_start(struct oneshot_lowerhalf_s *lower,
                        const struct timespec *ts);
static int fc7300_cancel(struct oneshot_lowerhalf_s *lower,
                         struct timespec *ts);
static int fc7300_current(struct oneshot_lowerhalf_s *lower,
                          struct timespec *ts);

/****************************************************************************
 * Private Data
 ****************************************************************************/

/* Lower half operations */

static const struct oneshot_operations_s g_oneshot_ops =
{
  .max_delay = fc7300_max_delay,
  .start     = fc7300_start,
  .cancel    = fc7300_cancel,
  .current   = fc7300_current,
};

static struct fc7300_oneshot_lowerhalf_priv_s g_oneshort0_priv =
{
  .ops      = &g_oneshot_ops,
};

static struct fc7300_oneshot_lowerhalf_priv_s g_oneshort1_priv =
{
  .ops      = &g_oneshot_ops,
};

static struct fc7300_oneshot_lowerhalf_priv_s g_oneshort2_priv =
{
  .ops      = &g_oneshot_ops,
};

static struct fc7300_oneshot_lowerhalf_priv_s g_oneshort3_priv =
{
  .ops      = &g_oneshot_ops,
};

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: fc7300_oneshot_handler
 *
 * Description:
 *   Timer expiration handler
 *
 * Input Parameters:
 *   arg - Should be the same argument provided when fc7300_oneshot_start()
 *         was called.
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/

static void fc7300_oneshot_handler(void *arg)
{
  struct fc7300_oneshot_lowerhalf_priv_s *priv =
    (struct fc7300_oneshot_lowerhalf_priv_s *)arg;

  oneshot_process_callback((struct oneshot_lowerhalf_s *)priv);
}

/****************************************************************************
 * Name: fc7300_max_delay
 *
 * Description:
 *   Determine the maximum delay of the one-shot timer (in microseconds)
 *
 * Input Parameters:
 *   lower  - An instance of the lower-half oneshot state structure.  This
 *            structure must have been previously initialized via a call to
 *            oneshot_initialize();
 *   ts     - The location in which to return the maximum delay.
 *
 * Returned Value:
 *   Zero (OK) is returned on success; a negated errno value is returned
 *   on failure.
 *
 ****************************************************************************/

static int fc7300_max_delay(struct oneshot_lowerhalf_s *lower,
                            struct timespec *ts)
{
  struct fc7300_oneshot_lowerhalf_priv_s *priv =
    (struct fc7300_oneshot_lowerhalf_priv_s *)lower;
  uint64_t usecs;
  int ret;

  DEBUGASSERT(priv != NULL && ts != NULL);

  ret = fc7300_oneshot_max_delay(&priv->oneshot, &usecs);
  if (ret >= 0)
    {
      uint64_t sec = usecs / 1000000;
      usecs -= 1000000 * sec;

      ts->tv_sec  = (time_t)sec;
      ts->tv_nsec = (long)(usecs * 1000);
    }
  else
    {
      ts->tv_sec  = 0;
      ts->tv_nsec = 0;
    }

  return ret;
}

/****************************************************************************
 * Name: fc7300_start
 *
 * Description:
 *   Start the oneshot timer
 *
 * Input Parameters:
 *   lower   - An instance of the lower-half oneshot state structure.  This
 *             structure must have been previously initialized via a call to
 *             oneshot_initialize();
 *   handler - The function to call when when the oneshot timer expires.
 *   arg     - An opaque argument that will accompany the callback.
 *   ts      - Provides the duration of the one shot timer.
 *
 * Returned Value:
 *   Zero (OK) is returned on success; a negated errno value is returned
 *   on failure.
 *
 ****************************************************************************/

static int fc7300_start(struct oneshot_lowerhalf_s *lower,
                        const struct timespec *ts)
{
  struct fc7300_oneshot_lowerhalf_priv_s *priv =
    (struct fc7300_oneshot_lowerhalf_priv_s *)lower;
  irqstate_t flags;
  int ret;

  DEBUGASSERT(priv != NULL && ts != NULL);

  /* Save the callback information and start the timer */

  flags = enter_critical_section();
  ret   = fc7300_oneshot_start(&priv->oneshot,
                               fc7300_oneshot_handler, priv, ts);
  leave_critical_section(flags);

  if (ret < 0)
    {
      tmrerr("ERROR: fc7300_oneshot_start failed: %d\n", flags);
    }

  return ret;
}

/****************************************************************************
 * Name: fc7300_cancel
 *
 * Description:
 *   Cancel the oneshot timer and return the time remaining on the timer.
 *
 * Input Parameters:
 *   lower  - Caller allocated instance of the oneshot state structure.  This
 *            structure must have been previously initialized via a call to
 *            oneshot_initialize();
 *   ts     - The location in which to return the time remaining on the
 *            oneshot timer.  A time of zero is returned if the timer is
 *            not running.
 *
 * Returned Value:
 *   Zero (OK) is returned on success.  A call to up_timer_cancel() when
 *   the timer is not active should also return success; a negated errno
 *   value is returned on any failure.
 *
 ****************************************************************************/

static int fc7300_cancel(struct oneshot_lowerhalf_s *lower,
                         struct timespec *ts)
{
  struct fc7300_oneshot_lowerhalf_priv_s *priv =
    (struct fc7300_oneshot_lowerhalf_priv_s *)lower;
  irqstate_t flags;
  int ret;

  DEBUGASSERT(priv != NULL);

  /* Cancel the timer */

  flags = enter_critical_section();
  ret   = fc7300_oneshot_cancel(&priv->oneshot, ts);
  leave_critical_section(flags);

  if (ret < 0)
    {
      tmrerr("ERROR: fc7300_oneshot_cancel failed: %d\n", flags);
    }

  return ret;
}

/****************************************************************************
 * Name: fc7300_current
 *
 * Description:
 *  Get the current time.
 *
 * Input Parameters:
 *   lower  - Caller allocated instance of the oneshot state structure.  This
 *            structure must have been previously initialized via a call to
 *            oneshot_initialize();
 *   ts     - The location in which to return the current time. A time of
 *            zero is returned for the initialization moment.
 *
 * Returned Value:
 *   Zero (OK) is returned on success, a negated errno value is returned on
 *   any failure.
 *
 ****************************************************************************/

static int fc7300_current(struct oneshot_lowerhalf_s *lower,
                          struct timespec *ts)
{
  struct fc7300_oneshot_lowerhalf_priv_s *priv =
    (struct fc7300_oneshot_lowerhalf_priv_s *)lower;

  DEBUGASSERT(priv != NULL);
  DEBUGASSERT(ts != NULL);

  return fc7300_oneshot_current(&priv->oneshot, ts);
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: oneshot_initialize
 *
 * Description:
 *   Initialize the oneshot timer and return a oneshot lower half driver
 *   instance.
 *
 * Input Parameters:
 *   chan       - Timer counter channel to be used.
 *   resolution - The required resolution of the timer in units of
 *                microseconds.  NOTE that the range is restricted to the
 *                range of uint16_t (excluding zero).
 *
 * Returned Value:
 *   On success, a non-NULL instance of the oneshot lower-half driver is
 *   returned.  NULL is return on any failure.
 *
 ****************************************************************************/

struct oneshot_lowerhalf_s *oneshot_initialize(int chan,
                                               uint16_t resolution)
{
  struct fc7300_oneshot_lowerhalf_priv_s *priv = NULL;
  int ret;

  switch (chan)
    {
      case 0:
        priv = &g_oneshort0_priv;
        break;
      case 1:
        priv = &g_oneshort1_priv;
        break;
      case 2:
        priv = &g_oneshort2_priv;
        break;
      case 3:
        priv = &g_oneshort3_priv;
        break;
      default:
        return NULL;
    }

  /* Initialize the contained FC7300 oneshot timer */

  ret = fc7300_oneshot_initialize(&priv->oneshot, chan, resolution);
  if (ret < 0)
    {
      tmrerr("ERROR: fc7300_oneshot_initialize failed: %d\n", ret);
      return NULL;
    }

  return (struct oneshot_lowerhalf_s *)priv;
}
