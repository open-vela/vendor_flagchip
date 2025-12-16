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

#ifndef __VENDOR_FLAGCHIP_CHIPS_FC7300_FC7300_ONESHOT_H
#define __VENDOR_FLAGCHIP_CHIPS_FC7300_FC7300_ONESHOT_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>
#include "fc7300_tstmp.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Public Types
 ****************************************************************************/

/* The oneshot client must allocate an instance of this structure and called
 * fc7300_oneshot_initialize() before using the oneshot facilities.  The
 * client should not access the contents of this structure directly since
 * the contents are subject to change.
 */

struct fc7300_oneshot_s
{
  bool running;                    /* True: the timer is running */
  struct fc7300_tstmp_dev_s *dev;  /* Pointer returned by fc7300_tstmp_init() */
  uint32_t frequency;              /* Timer counter frequency */
  uint64_t period;                 /* Expect timer counter when start */
};

/****************************************************************************
 * Public Data
 ****************************************************************************/

#ifndef __ASSEMBLY__

#undef EXTERN
#if defined(__cplusplus)
#define EXTERN extern "C"
extern "C"
{
#else
#define EXTERN extern
#endif

/****************************************************************************
 * Public Function Prototypes
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
                              uint16_t resolution);

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
                             uint64_t *usec);

/****************************************************************************
 * Name: fc7300_oneshot_start
 *
 * Description:
 *   Start the oneshot timer
 *
 * Input Parameters:
 *   oneshot - Caller allocated instance of the oneshot state structure.
 *             This structure must have been previously initialized via a
 *             call to fc7300_oneshot_initialize();
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
                         const struct timespec *ts);

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
 *             not running.
 *
 * Returned Value:
 *   Zero (OK) is returned on success.  A call to up_timer_cancel() when
 *   the timer is not active should also return success; a negated errno
 *   value is returned on any failure.
 *
 ****************************************************************************/

int fc7300_oneshot_cancel(struct fc7300_oneshot_s *oneshot,
                          struct timespec *ts);

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
 *   usec    The maximum delay in us.
 *
 * Returned Value:
 *   Zero (OK) is returned on success; a negated errno value is returned
 *   on failure.
 *
 ****************************************************************************/

int fc7300_oneshot_current(struct fc7300_oneshot_s *oneshot,
                           struct timespec *ts);

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
 *   Zero (OK) is returned on success; a negated errno value is returned
 *   on failure.
 *
 ****************************************************************************/

struct oneshot_lowerhalf_s *oneshot_initialize(int chan,
                                               uint16_t resolution);

#undef EXTERN
#ifdef __cplusplus
}
#endif

#endif /* __ASSEMBLY__ */
#endif /* __VENDOR_FLAGCHIP_CHIPS_FC7300_FC7300_ONESHOT_H */
