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

#ifndef __VENDOR_FLAGCHIP_CHIPS_FC7300_FC7300_WDOG_H
#define __VENDOR_FLAGCHIP_CHIPS_FC7300_FC7300_WDOG_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>
#include <nuttx/notifier.h>
#include <nuttx/panic_notifier.h>

#include "arm_internal.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Public Types
 ****************************************************************************/

/****************************************************************************
 * Inline Functions
 ****************************************************************************/

#ifndef __ASSEMBLY__

/****************************************************************************
 * Public Data
 ****************************************************************************/

#undef EXTERN
#if defined(__cplusplus)
#define EXTERN extern "C"
extern "C"
{
#else
#define EXTERN extern
#endif

/* This structure provides the private representation of the "lower-half"
 * driver state structure.  This structure must be cast-compatible with the
 * well-known watchdog_lowerhalf_s structure.
 */

struct fc7300_wdog_s
{
  const struct watchdog_ops_s  *ops;     /* Watchdog operations */
  uint32_t     base;                     /* The base address of the timer */
  uint32_t     irq;                      /* Watchdog irq */
  uint32_t     isr_priority;             /* Watchdog interrupt priority */
  xcpt_t       handler;                  /* Callback function instead of timeout reset */
  uint32_t     timeout;                  /* Current timeout value in millisecond */
  struct       wdog_s wdog;              /* Software watchdog timer */
  bool         iscaptured;               /* Whether the watchdog is captured */
  struct       notifier_block nb;        /* Notifier block for software watchdog timer */
};

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

/****************************************************************************
 * Name: fc7300_wdoginitialize
 *
 * Description:
 *   Initialize one watchdog instance for use with the upper_level watchdog
 *   driver.
 *
 * Input Parameters:
 *   instance - A number identifying the watchdog use.
 *
 * Returned Value:
 *   On success, a pointer to the FC7300 lower half Watchdog driver is
 *   returned. NULL is returned on any failure.
 *
 ****************************************************************************/

struct watchdog_lowerhalf_s *fc7300_wdoginitialize(int instance);

/****************************************************************************
 * Name: FC7300_Open_Internal_Watchdog
 *
 * Description:
 *   Start the watchdog timer, resetting the time to the current timeout.
 *
 * Note:
 *   This api is suggested to call only in boot stage.
 *
 ****************************************************************************/

int FC7300_Open_Internal_Watchdog(uint8_t instance, uint32_t timeout);

/****************************************************************************
 * Name: FC7300_Close_Internal_Watchdog
 *
 * Description:
 *   Close the watchdog timer
 *
 * Input Parameters:
 *   instance - Hardware unit index
 *
 * Returned Values:
 *   Void
 *
 * Note:
 *   This api is suggested to call only in boot stage.
 *
 ****************************************************************************/

void FC7300_Close_Internal_Watchdog(uint8_t instance);

#undef EXTERN
#if defined(__cplusplus)
}
#endif

#endif /* __ASSEMBLY__ */
#endif /* __VENDOR_FLAGCHIP_CHIPS_FC7300_FC7300_WDOG_H */
