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

#include <assert.h>
#include "fc7300_userspace.h"
/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: fc7300_userspace
 *
 * Description:
 *   For the case of the separate user-/kernel-space build, perform whatever
 *   platform specific initialization of the user memory is required.
 *   Normally this just means initializing the user space .data and .bss
 *   segments.
 *
 * Assumptions:
 *   The D-Cache has not yet been enabled.
 *
 ****************************************************************************/

void fc7300_userspace(void)
{
  uint8_t   *src;
  uint8_t   *dest;
  uint8_t   *end;
  struct fc7300_userspace *puserspace = (struct fc7300_userspace *)USERSPACE;

  /* Clear all of user-space .bss */

  DEBUGASSERT(puserspace->common.us_bssstart != 0 &&
              puserspace->common.us_bssend != 0 &&
              puserspace->common.us_bssstart <=
                        puserspace->common.us_bssend);

  dest = (uint8_t *)puserspace->common.us_bssstart;
  end  = (uint8_t *)puserspace->common.us_bssend;

  while (dest != end)
    {
      *dest++ = 0;
    }

  /* Initialize all of user-space .data */

  DEBUGASSERT(puserspace->common.us_datasource != 0 &&
              puserspace->common.us_datastart != 0 &&
              puserspace->common.us_dataend != 0 &&
              puserspace->common.us_datastart <=
                        puserspace->common.us_dataend);

  src  = (uint8_t *)puserspace->common.us_datasource;
  dest = (uint8_t *)puserspace->common.us_datastart;
  end  = (uint8_t *)puserspace->common.us_dataend;

  while (dest != end)
    {
      *dest++ = *src++;
    }

  /* Initialize all of user-space .calibra */

  DEBUGASSERT(puserspace->priv.us_calibsource != 0 &&
              puserspace->priv.us_calibstart != 0 &&
              puserspace->priv.us_calibend != 0 &&
              puserspace->priv.us_calibstart <=
                          puserspace->priv.us_calibend);

  src  = (uint8_t *)puserspace->priv.us_calibsource;
  dest = (uint8_t *)puserspace->priv.us_calibstart;
  end  = (uint8_t *)puserspace->priv.us_calibend;

  while (dest != end)
    {
      *dest++ = *src++;
    }
}
