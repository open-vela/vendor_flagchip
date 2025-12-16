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

#include <sys/types.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include <debug.h>
#include <nuttx/irq.h>
#include <nuttx/spinlock.h>
#include <nuttx/timers/timer.h>

#include <arch/board/board.h>

#include "fc7300_fcpit.h"
#include "fc7300_periphclocks.h"

#if (defined(CONFIG_FC7300_FCPIT0_CH0) || defined(CONFIG_FC7300_FCPIT0_CH1)||\
     defined(CONFIG_FC7300_FCPIT0_CH2) || defined(CONFIG_FC7300_FCPIT0_CH3)||\
     defined(CONFIG_FC7300_FCPIT1_CH0) || defined(CONFIG_FC7300_FCPIT1_CH1)||\
     defined(CONFIG_FC7300_FCPIT1_CH2) || defined(CONFIG_FC7300_FCPIT1_CH3))

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Private Types
 ****************************************************************************/

/* This structure provides the private representation of the "lower-half"
 * driver state structure.  This structure must be cast-compatible with the
 * timer_lowerhalf_s structure.
 */

struct fc7300_timer_callbackmap_s
{
  struct fc7300_fcpit_dev_s  *fcpit;     /* fc7300 timer driver */
  tccb_t          callback;
  void                       *arg;       /* Argument passed to upper half callback */
};

struct fc7300_lowerhalf_s
{
  const struct timer_ops_s   *ops;       /* Lower half operations */
  struct fc7300_timer_callbackmap_s *cbtable;   /* Current user interrupt callback */
  bool                       started;    /* True: Timer has been started */
  const uint8_t              resolution; /* Number of bits in the timer (16 or 32 bits) */
  uint8_t                    channel;
};

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

static int fc7300_timer_handler(int irq, void * context, void * arg);

/* "Lower half" driver methods **********************************************/

static int fc7300_start(struct timer_lowerhalf_s *lower);
static int fc7300_stop(struct timer_lowerhalf_s *lower);
static int fc7300_getstatus(struct timer_lowerhalf_s *lower,
                            struct timer_status_s *status);
static int fc7300_settimeout(struct timer_lowerhalf_s *lower,
                             uint32_t timeout);
static void fc7300_setcallback(struct timer_lowerhalf_s *lower,
                               tccb_t callback, void *arg);
static int fc7300_maxtimeout(struct timer_lowerhalf_s *lower,
                       uint32_t *maxtimeout);
/****************************************************************************
 * Private Data
 ****************************************************************************/

/* "Lower half" driver methods */

struct fc7300_timer_callbackmap_s g_fcpit0_callback[4] =
{
  {
    .arg            = NULL,
    .fcpit          = NULL,
    .callback       = NULL,
  },
  {
    .arg            = NULL,
    .fcpit          = NULL,
    .callback       = NULL,
  },
  {
    .arg            = NULL,
    .fcpit          = NULL,
    .callback       = NULL,
  },
  {
    .arg            = NULL,
    .fcpit          = NULL,
    .callback       = NULL,
  },
};

struct fc7300_timer_callbackmap_s g_fcpit1_callback[4] =
{
  {
    .arg            = NULL,
    .fcpit          = NULL,
    .callback       = NULL,
  },
  {
    .arg            = NULL,
    .fcpit          = NULL,
    .callback       = NULL,
  },
  {
    .arg            = NULL,
    .fcpit          = NULL,
    .callback       = NULL,
  },
  {
    .arg            = NULL,
    .fcpit          = NULL,
    .callback       = NULL,
  },
};

static const struct timer_ops_s g_timer_ops =
{
  .start       = fc7300_start,
  .stop        = fc7300_stop,
  .getstatus   = fc7300_getstatus,
  .settimeout  = fc7300_settimeout,
  .setcallback = fc7300_setcallback,
  .ioctl       = NULL,
  .maxtimeout  = fc7300_maxtimeout,
};

#ifdef CONFIG_FC7300_FCPIT0_CH0
static struct fc7300_lowerhalf_s g_fcpit0_ch0_lowerhalf =
{
  .ops         = &g_timer_ops,
  .resolution  = FC7300_FCPIT0_CH0_RES,
  .channel     = 0,
  .cbtable     = g_fcpit0_callback,
};
#endif

#ifdef CONFIG_FC7300_FCPIT0_CH1
static struct fc7300_lowerhalf_s g_fcpit0_ch1_lowerhalf =
{
  .ops         = &g_timer_ops,
  .resolution  = FC7300_FCPIT0_CH1_RES,
  .channel     = 1,
  .cbtable     = g_fcpit0_callback,
};
#endif

#ifdef CONFIG_FC7300_FCPIT0_CH2
static struct fc7300_lowerhalf_s g_fcpit0_ch2_lowerhalf =
{
  .ops         = &g_timer_ops,
  .resolution  = FC7300_FCPIT0_CH2_RES,
  .channel     = 2,
  .cbtable     = g_fcpit0_callback,
};
#endif

#ifdef CONFIG_FC7300_FCPIT0_CH3
static struct fc7300_lowerhalf_s g_fcpit0_ch3_lowerhalf =
{
  .ops         = &g_timer_ops,
  .resolution  = FC7300_FCPIT0_CH3_RES,
  .channel     = 3,
  .cbtable     = g_fcpit0_callback,
};
#endif

#ifdef CONFIG_FC7300_FCPIT1_CH0
static struct fc7300_lowerhalf_s g_fcpit1_ch0_lowerhalf =
{
  .ops         = &g_timer_ops,
  .resolution  = FC7300_FCPIT1_CH0_RES,
  .channel     = 0,
  .cbtable     = g_fcpit1_callback,
};
#endif

#ifdef CONFIG_FC7300_FCPIT1_CH1
static struct fc7300_lowerhalf_s g_fcpit1_ch1_lowerhalf =
{
  .ops         = &g_timer_ops,
  .resolution  = FC7300_FCPIT1_CH1_RES,
  .channel     = 1,
  .cbtable     = g_fcpit1_callback,
};
#endif

#ifdef CONFIG_FC7300_FCPIT1_CH2
static struct fc7300_lowerhalf_s g_fcpit1_ch2_lowerhalf =
{
  .ops         = &g_timer_ops,
  .resolution  = FC7300_FCPIT1_CH2_RES,
  .channel     = 2,
  .cbtable     = g_fcpit1_callback,
};
#endif

#ifdef CONFIG_FC7300_FCPIT1_CH3
static struct fc7300_lowerhalf_s g_fcpit1_ch3_lowerhalf =
{
  .ops         = &g_timer_ops,
  .resolution  = FC7300_FCPIT1_CH3_RES,
  .channel     = 3,
  .cbtable     = g_fcpit1_callback,
};
#endif

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: fc7300_timer_handler
 *
 * Description:
 *   timer interrupt handler
 *
 * Input Parameters:
 *
 * Returned Value:
 *
 ****************************************************************************/

__ramfunc__ static int fc7300_timer_handler(int irq, void * context, void * arg)
{
  struct fc7300_lowerhalf_s *lower = (struct fc7300_lowerhalf_s *) arg;
  uint32_t next_interval_us = 0;
  uint8_t i;

  for(i = 0; i < 4; i++)
    {
      if (lower->cbtable[i].fcpit != NULL)
        {
          if(FC7300_FCPIT_ACKINT(lower->cbtable[i].fcpit, i) == OK)
            {
              if (lower->cbtable[i].callback(&next_interval_us,
                  (void *)lower->cbtable[i].arg))
                {
                  if (next_interval_us > 0)
                    {
                      FC7300_FCPIT_SETCOUNTER(lower->cbtable[i].fcpit,
                                              next_interval_us);
                    }
                }
              else
                {
                  fc7300_stop((struct timer_lowerhalf_s *)lower);
                }
            }
        }
    }

  return OK;
}

/****************************************************************************
 * Name: fc7300_start
 *
 * Description:
 *   Start the timer, resetting the time to the current timeout,
 *
 * Input Parameters:
 *   lower - A pointer the publicly visible representation of the
 *           "lower-half" driver state structure.
 *
 * Returned Value:
 *   Zero on success; a negated errno value on failure.
 *
 ****************************************************************************/

static int fc7300_start(struct timer_lowerhalf_s *lower)
{
  struct fc7300_lowerhalf_s *priv = (struct fc7300_lowerhalf_s *)lower;

  if (!priv->started)
    {
      FC7300_FCPIT_SETCLOCK(priv->cbtable[priv->channel].fcpit,
                     FC7300_FCPIT_PCKFREQ(priv->cbtable[priv->channel].fcpit));
      FC7300_FCPIT_SETMODE(priv->cbtable[priv->channel].fcpit,
                           FC7300_FCPIT_32PERIODIC_COUNTER);

      if (priv->cbtable[priv->channel].callback != NULL)
        {
          FC7300_FCPIT_SETISR(priv->cbtable[priv->channel].fcpit,
                              fc7300_timer_handler, priv, 0);
          FC7300_FCPIT_ENABLEINT(priv->cbtable[priv->channel].fcpit,
                                 priv->channel);
        }
      priv->started = true;
      return OK;
    }

  /* Return EBUSY to indicate that the timer was already running */

  return -EBUSY;
}

/****************************************************************************
 * Name: fc7300_stop
 *
 * Description:
 *   Stop the timer
 *
 * Input Parameters:
 *   lower - A pointer the publicly visible representation of the
 *           "lower-half" driver state structure.
 *
 * Returned Value:
 *   Zero on success; a negated errno value on failure.
 *
 ****************************************************************************/

static int fc7300_stop(struct timer_lowerhalf_s *lower)
{
  struct fc7300_lowerhalf_s *priv = (struct fc7300_lowerhalf_s *)lower;

  if (priv->started)
    {
      FC7300_FCPIT_SETMODE(priv->cbtable[priv->channel].fcpit,
                                         FC7300_FCPIT_DIABLED);
      FC7300_FCPIT_DISABLEINT(priv->cbtable[priv->channel].fcpit,
                              priv->channel);
      FC7300_FCPIT_SETISR(priv->cbtable[priv->channel].fcpit, NULL, NULL, 0);
      priv->started = false;
      return OK;
    }

  /* Return ENODEV to indicate that the timer was not running */

  return -ENODEV;
}

/****************************************************************************
 * Name: fc7300_getstatus
 *
 * Description:
 *   get timer status
 *
 * Input Parameters:
 *   lower  - A pointer the publicly visible representation of the "lower-
 *            half" driver state structure.
 *   status - The location to return the status information.
 *
 * Returned Value:
 *   Zero on success; a negated errno value on failure.
 *
 ****************************************************************************/
static int fc7300_getstatus(struct timer_lowerhalf_s *lower,
                            struct timer_status_s *status)
{
  struct fc7300_lowerhalf_s *priv = (struct fc7300_lowerhalf_s *)lower;
  uint32_t timeout;
  uint32_t settick;
  uint32_t counter;

  DEBUGASSERT(status != NULL);

  /* Return the status bit */

  status->flags = 0;
  if (priv->started)
    {
      status->flags |= TCFLAGS_ACTIVE;
    }

  if (priv->cbtable[priv->channel].callback)
    {
      status->flags |= TCFLAGS_HANDLER;
    }

  /* Get timeout */

  settick = FC7300_FCPIT_GETSETCOUNTER(priv->cbtable[priv->channel].fcpit);

  timeout = settick / (FC7300_FCPIT_PCKFREQ(priv->cbtable[priv->channel].fcpit)/
            USEC_PER_SEC);

  status->timeout = timeout;

  /* Get the time remaining until the timer expires (in microseconds) */

  counter    = FC7300_FCPIT_GETCOUNTER(priv->cbtable[priv->channel].fcpit);
  status->timeleft = (settick - counter) /
                     (FC7300_FCPIT_PCKFREQ(priv->cbtable[priv->channel].fcpit)/
                      USEC_PER_SEC);
  tmrinfo("timeout=%" PRIu32 " counter=%" PRIu32 "\n", timeout, counter);
  tmrinfo("timeleft=%" PRIu32 "\n", status->timeleft);
  return OK;
}

/****************************************************************************
 * Name: fc7300_settimeout
 *
 * Description:
 *   Set a new timeout value (and reset the timer)
 *
 * Input Parameters:
 *   lower   - A pointer the publicly visible representation of the
 *             "lower-half" driver state structure.
 *   timeout - The new timeout value in microseconds.
 *
 * Returned Value:
 *   Zero on success; a negated errno value on failure.
 *
 ****************************************************************************/

static int fc7300_settimeout(struct timer_lowerhalf_s *lower,
                            uint32_t timeout)
{
  struct fc7300_lowerhalf_s *priv = (struct fc7300_lowerhalf_s *)lower;
  uint32_t maxtimeout;
  uint32_t timeouttick;

  if (priv->started)
    {
      return -EPERM;
    }

  FC7300_FCPIT_SETCLOCK(priv->cbtable[priv->channel].fcpit,
                    FC7300_FCPIT_PCKFREQ(priv->cbtable[priv->channel].fcpit));
  timeouttick = timeout *
    (FC7300_FCPIT_PCKFREQ(priv->cbtable[priv->channel].fcpit) / USEC_PER_SEC);
  maxtimeout = (uint32_t)0xFFFFFFFFul;
  if (timeouttick > maxtimeout)
    {
      FC7300_FCPIT_SETCOUNTER(priv->cbtable[priv->channel].fcpit, maxtimeout);
    }
  else
    {
      FC7300_FCPIT_SETCOUNTER(priv->cbtable[priv->channel].fcpit, timeouttick);
    }

  return OK;
}

/****************************************************************************
 * Name: fc7300_setcallback
 *
 * Description:
 *   Call this user provided timeout callback.
 *
 * Input Parameters:
 *   lower      - A pointer the publicly visible representation of the
 *                "lower-half" driver state structure.
 *   callback - The new timer expiration function pointer.  If this
 *                function pointer is NULL, then the reset-on-expiration
 *                behavior is restored,
 *  arg          - Argument that will be provided in the callback
 *
 * Returned Value:
 *   The previous timer expiration function pointer or NULL is there was
 *   no previous function pointer.
 *
 ****************************************************************************/

static void fc7300_setcallback(struct timer_lowerhalf_s *lower,
                              tccb_t callback, void *arg)
{
  struct fc7300_lowerhalf_s *priv = (struct fc7300_lowerhalf_s *)lower;

  irqstate_t flags = enter_critical_section();

  /* Save the new callback */

  priv->cbtable[priv->channel].callback = callback;
  priv->cbtable[priv->channel].arg      = arg;

  if (callback != NULL && priv->started)
    {
      FC7300_FCPIT_SETISR(priv->cbtable[priv->channel].fcpit,
                          fc7300_timer_handler, priv, 0);
      FC7300_FCPIT_ENABLEINT(priv->cbtable[priv->channel].fcpit,
                             priv->channel);
    }
  else
    {
      FC7300_FCPIT_DISABLEINT(priv->cbtable[priv->channel].fcpit,
                              priv->channel);
      FC7300_FCPIT_SETISR(priv->cbtable[priv->channel].fcpit, NULL, NULL, 0);
    }

  leave_critical_section(flags);
}

/****************************************************************************
 * Name: fc7300_maxtimeout
 *
 * Description:
 *   Get the maximum supported timeout value
 *
 * Input Parameters:
 *   lower   - A pointer the publicly visible representation of the
 *             "lower-half" driver state structure.
 *   maxtimeout - Support max timeout value in microseconds.
 *
 * Returned Value:
 *   Zero on success; a negated errno value on failure.
 *
 ****************************************************************************/

static int fc7300_maxtimeout(struct timer_lowerhalf_s *lower,
                             uint32_t *maxtimeout)
{
  struct fc7300_lowerhalf_s *priv = (struct fc7300_lowerhalf_s *)lower;

  *maxtimeout = UINT32_MAX /
                (FC7300_FCPIT_PCKFREQ(priv->cbtable[priv->channel].fcpit) /
                USEC_PER_SEC);
  return OK;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: fc7300_timer_initialize
 *
 * Description:
 *   Bind the configuration timer to a timer lower half instance and
 *   register the timer drivers at 'devpath'
 *
 * Input Parameters:
 *   devpath - The full path to the timer device.  This should be of the
 *     form /dev/timer0
 *   timer - the timer's number.
 *
 * Returned Value:
 *   Zero (OK) is returned on success; A negated errno value is returned
 *   to indicate the nature of any failure.
 *
 ****************************************************************************/

int fc7300_timer_initialize(const char *devpath, int timer)
{
  struct fc7300_lowerhalf_s *lower;
  uint8_t timertmp;

  switch (timer)
    {
#ifdef CONFIG_FC7300_FCPIT0_CH0
      case 0:
        lower = &g_fcpit0_ch0_lowerhalf;
        break;
#endif
#ifdef CONFIG_FC7300_FCPIT0_CH1
      case 1:
        lower = &g_fcpit0_ch1_lowerhalf;
        break;
#endif
#ifdef CONFIG_FC7300_FCPIT0_CH2
      case 2:
        lower = &g_fcpit0_ch2_lowerhalf;
        break;
#endif
#ifdef CONFIG_FC7300_FCPIT0_CH3
      case 3:
        lower = &g_fcpit0_ch3_lowerhalf;
        break;
#endif
#ifdef CONFIG_FC7300_FCPIT1_CH0
      case 4:
        lower = &g_fcpit1_ch0_lowerhalf;
        break;
#endif
#ifdef CONFIG_FC7300_FCPIT1_CH1
      case 5:
        lower = &g_fcpit1_ch1_lowerhalf;
        break;
#endif
#ifdef CONFIG_FC7300_FCPIT1_CH2
      case 6:
        lower = &g_fcpit1_ch2_lowerhalf;
        break;
#endif
#ifdef CONFIG_FC7300_FCPIT1_CH3
      case 7:
        lower = &g_fcpit1_ch3_lowerhalf;
        break;
#endif
      default:
        return -ENODEV;
    }

  /* Initialize the elements of lower half state structure */

  timertmp                          = timer % 4;
  lower->started                    = false;
  lower->cbtable[timertmp].callback = NULL;
  lower->cbtable[timertmp].fcpit    = fc7300_fcpit_init(timer);

  if (lower->cbtable[timertmp].fcpit == NULL)
    {
      return -EINVAL;
    }

  /* Register the timer driver as /dev/timerX.  The returned value from
   * timer_register is a handle that could be used with timer_unregister().
   * REVISIT: The returned handle is discard here.
   */

  void *drvr = timer_register(devpath,
                              (struct timer_lowerhalf_s *)lower);
  if (drvr == NULL)
    {
      /* The actual cause of the failure may have been a failure to allocate
       * perhaps a failure to register the timer driver (such as if the
       * 'depath' were not unique).  We know here but we return EEXIST to
       * indicate the failure (implying the non-unique devpath).
       */

      return -EEXIST;
    }

  return OK;
}

#endif /* CONFIG_TIMER */
