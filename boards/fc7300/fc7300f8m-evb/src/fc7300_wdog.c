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

#include <errno.h>
#include <debug.h>
#include <nuttx/timers/watchdog.h>
#include "fc7300_wdog.h"
#include "fc7300f8m-evb.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Private Types
 ****************************************************************************/

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: fc7300_wdog_setup
 *
 * Description:
 *   Initialize wdog drivers for use with /dev/watchdogX
 *
 ****************************************************************************/

int fc7300_wdog_setup(void)
{
  void * handle = NULL;
  struct watchdog_lowerhalf_s *wdog = NULL;

#ifdef CONFIG_FC7300_WATCHDOG0
  wdog = fc7300_wdoginitialize(0);
  if (!wdog)
    {
      wderr("ERROR: Failed to get the FC7300 Watchdog lower half\n");
      return -ENODEV;
    }

  handle = watchdog_register("/dev/watchdog0", wdog);
  if (handle == NULL)
    {
      wderr("ERROR: watchdog_register failed\n");
      return -ENODEV;
    }
#endif

#ifdef CONFIG_FC7300_WATCHDOG1
  wdog = fc7300_wdoginitialize(1);
  if (!wdog)
    {
      wderr("ERROR: Failed to get the FC7300 Watchdog lower half\n");
      return -ENODEV;
    }

  handle = watchdog_register("/dev/watchdog1", wdog);
  if (handle == NULL)
    {
      wderr("ERROR: watchdog_register failed\n");
      return -ENODEV;
    }
#endif

#ifdef CONFIG_FC7300_WATCHDOG2
  wdog = fc7300_wdoginitialize(2);
  if (!wdog)
    {
      wderr("ERROR: Failed to get the FC7300 Watchdog lower half\n");
      return -ENODEV;
    }

  handle = watchdog_register("/dev/watchdog2", wdog);
  if (handle == NULL)
    {
      wderr("ERROR: watchdog_register failed\n");
      return -ENODEV;
    }
#endif

  return 0;
}
