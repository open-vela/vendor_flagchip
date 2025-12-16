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

#include <sys/types.h>
#include <stdint.h>
#include <stdbool.h>
#include <sched.h>
#include <assert.h>
#include <errno.h>
#include <debug.h>
#include <nuttx/config.h>
#include <nuttx/irq.h>
#include <nuttx/clock.h>
#include <nuttx/spinlock.h>

#include "fc7300_clockconfig.h"
#include "fc7300_periphclocks.h"
#include "fc7300_oneshot.h"

#ifdef CONFIG_FC7300_ONESHOT

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

/****************************************************************************
 * Private Data
 ****************************************************************************/

#define UINT56_MAX          UINT64_C(72057594037927935)

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: fc7300_oneshot_initialize
 *
 * Description:
 *   Initialize the oneshot timer wrapper
 *
 * Input Parameters:
 *   oneshot    - Caller allocated instance of the oneshot state structure
 *   chan       - Timer counter channel to be used.
 *   resolution - The required resolution of the timer in units of
 *                microseconds.  NOTE that the range is restricted to the
 *                range of uint16_t (excluding zero).
 *
 * Returned Value:
 *   Zero (OK) is returned on success; a negated errno value is returned
 *   on failure.
 *
 ****************************************************************************/

int fc7300_oneshot_initialize(struct fc7300_oneshot_s *oneshot, int chan,
                              uint16_t resolution)
{
  tmrinfo("chan=%d resolution=%d usec\n", chan, resolution);

  DEBUGASSERT(oneshot && resolution > 0);

  oneshot->dev = fc7300_tstmp_init(chan);
  if (oneshot->dev == NULL)
    {
      tmrerr("ERROR: Failed to allocate TSTMP%d\n", chan);
      return -EBUSY;
    }

  /* Initialize the remaining fields in the state structure. */

  oneshot->running   = false;
  oneshot->frequency = fc7300_get_busclk();

  return OK;
}

/****************************************************************************
 * Name: fc7300_oneshot_max_delay
 *
 * Description:
 *   Determine the maximum delay of the one-shot timer (in microseconds)
 *
 * Input Parameters:
 *   oneshot - Caller allocated instance of the oneshot state structure. This
 *             structure must have been previously initialized via a call to
 *             fc7300_oneshot_initialize();
 *   ts      - The location in which to return the maximum delay.
 *
 * Returned Value:
 *   Zero (OK) is returned on success; a negated errno value is returned
 *   on failure.
 *
 ****************************************************************************/

int fc7300_oneshot_max_delay(struct fc7300_oneshot_s *oneshot,
                             uint64_t *usec)
{
  double val;

  DEBUGASSERT(oneshot != NULL && usec != NULL);

  val = ((double)UINT56_MAX / oneshot->frequency);
  *usec = (uint64_t)(val * USEC_PER_SEC);

  return OK;
}

/****************************************************************************
 * Name: fc7300_oneshot_start
 *
 * Description:
 *   Start the oneshot timer
 *
 * Input Parameters:
 *   oneshot - Caller allocated instance of the oneshot state structure. This
 *             structure must have been previously initialized via a call to
 *             fc7300_oneshot_initialize();
 *   handler - The function to call when when the oneshot timer expires.
 *   arg     - An opaque argument that will accompany the callback.
 *   ts      - Provides the duration of the one shot timer.
 *
 * Returned Value:
 *   Zero (OK) is returned on success; a negated errno value is returned
 *   on failure.
 *
 ****************************************************************************/

int fc7300_oneshot_start(struct fc7300_oneshot_s *oneshot,
                         fc7300_tstmp_callback_t handler, void *arg,
                         const struct timespec *ts)
{
  uint64_t usec;
  uint64_t period;
  irqstate_t flags;

  //coverity[Y2K38_SAFETY:SUPPRESS]
  tmrinfo("handler=%p arg=%p, ts=(%lu, %lu)\n", handler, arg,
          (unsigned long)ts->tv_sec, (unsigned long)ts->tv_nsec);

  DEBUGASSERT(oneshot && handler && ts);

  /* Express the delay in microseconds */

  usec = (uint64_t)ts->tv_sec * USEC_PER_SEC +
         (uint64_t)(ts->tv_nsec / NSEC_PER_USEC);
  if (usec == 0)
    {
      /* set usec = 1 when it is zero */

      usec = 1;
    }

  /* Was the oneshot already running? */

  flags = enter_critical_section();
  if (oneshot->running)
    {
      /* Yes.. then cancel it */

      tmrinfo("Already running... cancelling\n");
      fc7300_oneshot_cancel(oneshot, NULL);
    }

  /* Get the timer counter frequency and determine the number of counts need
   * to achieve the requested delay.
   *
   *   frequency = ticks / second
   *   ticks     = seconds * frequency
   *             = (usecs * frequency) / USEC_PER_SEC;
   */

  period = usec * (oneshot->frequency / USEC_PER_SEC) - 1;

  tmrinfo("usec=%llu period=%08llx\n", usec, period);
  DEBUGASSERT(period <= UINT56_MAX);

  /* Enable the channel */

  FC7300_TSTMP_START(oneshot->dev, period, handler, arg);

  /* Set timer period */

  oneshot->period = period;

  oneshot->running = true;
  leave_critical_section(flags);

  return OK;
}

/****************************************************************************
 * Name: fc7300_oneshot_cancel
 *
 * Description:
 *   Cancel the oneshot timer and return the time remaining on the timer.
 *
 * Input Parameters:
 *   oneshot - Caller allocated instance of the oneshot state structure.
 *             This structure must have been previously initialized via a
 *             call to fc7300_oneshot_initialize();
 *   ts      - The location in which to return the time remaining on the
 *             oneshot timer.  A time of zero is returned if the timer is
 *             not running.  ts may be zero in which case the time remaining
 *             is not returned.
 *
 * Returned Value:
 *   Zero (OK) is returned on success.  A call to up_timer_cancel() when
 *   the timer is not active should also return success; a negated errno
 *   value is returned on any failure.
 *
 ****************************************************************************/

int fc7300_oneshot_cancel(struct fc7300_oneshot_s *oneshot,
                          struct timespec *ts)
{
  irqstate_t flags;
  uint64_t usec;
  uint64_t sec;
  uint64_t nsec;
  uint64_t count;
  uint64_t period;

  /* Was the timer running? */

  flags = enter_critical_section();
  if (!oneshot->running)
    {
      /* No.. Just return zero timer remaining and successful cancellation.
       * This function may execute at a high rate with no timer running
       * (as when pre-emption is enabled and disabled).
       */

      if (ts != NULL)
        {
          ts->tv_sec  = 0;
          ts->tv_nsec = 0;
        }

      leave_critical_section(flags);
      return OK;
    }

  /* Yes.. Get the timer counter and period registers and stop the counter.
   * If the counter expires while we are doing this, the counter clock will
   * be stopped, but the clock will not be disabled.
   *
   * The expected behavior is that the counter register will freezes at
   * a value equal to the RC register when the timer expires.  The counter
   * should have values between 0 and RC in all other cased.
   *
   * REVISIT:  This does not appear to be the case.
   */

  tmrinfo("Cancelling...\n");

  /* Disable the interrupt and stop the timer. */

  count  = FC7300_TSTMP_STOP(oneshot->dev);
  period = oneshot->period;

  oneshot->running = false;
  leave_critical_section(flags);

  /* Did the caller provide us with a location to return the time
   * remaining?
   */

  if (ts)
    {
      /* Yes.. then calculate and return the time remaining on the
       * oneshot timer.
       */

      tmrinfo("period=%lu count=%lu\n",
              (unsigned long)period, (unsigned long)count);

      /* REVISIT: I am not certain why the timer counter value sometimes
       * exceeds RC.  Might be a bug, or perhaps the counter does not stop
       * in all cases.
       */

      if (count >= period)
        {
          /* No time remaining (?) */

          ts->tv_sec  = 0;
          ts->tv_nsec = 0;
        }
      else
        {
          /* The total time remaining is the difference.  Convert that
           * to units of microseconds.
           *
           *   frequency = ticks / second
           *   seconds   = ticks * frequency
           *   usecs     = (ticks * USEC_PER_SEC) / frequency;
           */

          usec        = ((uint64_t)(period - count)) /
                        (oneshot->frequency / USEC_PER_SEC);

          /* Return the time remaining in the correct form */

          sec         = usec / USEC_PER_SEC;
          nsec        = ((usec) - (sec * USEC_PER_SEC)) * NSEC_PER_USEC;

          ts->tv_sec  = (time_t)sec;
          ts->tv_nsec = (unsigned long)nsec;
        }
      //coverity[Y2K38_SAFETY:SUPPRESS]
      tmrinfo("remaining (%lu, %lu)\n",
              (unsigned long)ts->tv_sec, (unsigned long)ts->tv_nsec);
    }

  return OK;
}

/****************************************************************************
 * Name: fc7300_oneshot_current
 *
 * Description:
 *   Get the current time.
 *
 * Input Parameters:
 *   oneshot - Caller allocated instance of the oneshot state structure.
 *             This structure must have been previously initialized via a
 *             call to fc7300_oneshot_initialize();
 *   usec    - The maximum delay in us.
 *
 * Returned Value:
 *   Zero (OK) is returned on success; a negated errno value is returned
 *   on failure.
 *
 ****************************************************************************/

int fc7300_oneshot_current(struct fc7300_oneshot_s *oneshot,
                           struct timespec *ts)
{
  uint64_t usec;
  uint64_t sec;
  uint64_t nsec;
  uint64_t count;

  count       = FC7300_TSTMP_GETCOUNTER(oneshot->dev);
  usec        = count / (oneshot->frequency / USEC_PER_SEC);

  sec         = usec / USEC_PER_SEC;
  nsec        = ((usec) - (sec * USEC_PER_SEC)) * NSEC_PER_USEC;

  ts->tv_sec  = (time_t)sec;
  ts->tv_nsec = (unsigned long)nsec;

  return OK;
}

#endif /* CONFIG_FC7300_ONESHOT */
