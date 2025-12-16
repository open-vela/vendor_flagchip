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

#include <errno.h>
#include <debug.h>

#include <nuttx/timers/pwm.h>
#include <arch/board/board.h>

#include "chip.h"
#include "arm_internal.h"
#include "fc7300_pwm.h"
#include "fc7300f8m-evb.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: fc7300_pwm_initialize
 *
 * Description:
 *   Initialize PWM and register the PWM device.
 *
 ****************************************************************************/

int fc7300_pwm_initialize(void)
{
  static bool initialized = false;
  struct pwm_lowerhalf_s *pwm;
  int ret = OK;

  /* Have we already initialized? */

  if (!initialized)
    {
#ifdef CONFIG_FC7300_FTU0_PWM

      /* Call fc7300_pwminitialize() to get instance 0 of the PWM interface */

      pwm = fc7300_pwminitialize(FC7300F8M_ESMALL_PWM0);
      if (!pwm)
        {
          aerr("ERROR: Failed to get the FC7300 PWM lower half\n");
          return -ENODEV;
        }

      /* Register the PWM driver at "/dev/pwm0" */

      ret = pwm_register("/dev/pwm0", pwm);
      if (ret < 0)
        {
          aerr("ERROR: pwm_register failed: %d\n", ret);
          return ret;
        }
#endif

#ifdef CONFIG_FC7300_FTU1_PWM

      /* Call fc7300_pwminitialize() to get instance 1 of the PWM interface */

      pwm = fc7300_pwminitialize(FC7300F8M_ESMALL_PWM1);
      if (!pwm)
        {
          aerr("ERROR: Failed to get the FC7300 PWM lower half\n");
          return -ENODEV;
        }

      /* Register the PWM driver at "/dev/pwm1" */

      ret = pwm_register("/dev/pwm1", pwm);
      if (ret < 0)
        {
          aerr("ERROR: pwm_register failed: %d\n", ret);
          return ret;
        }
#endif

#ifdef CONFIG_FC7300_FTU3_PWM

      /* Call fc7300_pwminitialize() to get instance 3 of the PWM interface */

      pwm = fc7300_pwminitialize(FC7300F8M_ESMALL_PWM3);
      if (!pwm)
        {
          aerr("ERROR: Failed to get the FC7300 PWM lower half\n");
          return -ENODEV;
        }

      /* Register the PWM driver at "/dev/pwm3" */

      ret = pwm_register("/dev/pwm3", pwm);
      if (ret < 0)
        {
          aerr("ERROR: pwm_register failed: %d\n", ret);
          return ret;
        }
#endif

#ifdef CONFIG_FC7300_FTU4_PWM

      /* Call fc7300_pwminitialize() to get instance 4 of the PWM interface */

      pwm = fc7300_pwminitialize(FC7300F8M_ESMALL_PWM4);
      if (!pwm)
        {
          aerr("ERROR: Failed to get the FC7300 PWM lower half\n");
          return -ENODEV;
        }

      /* Register the PWM driver at "/dev/pwm4" */

      ret = pwm_register("/dev/pwm4", pwm);
      if (ret < 0)
        {
          aerr("ERROR: pwm_register failed: %d\n", ret);
          return ret;
        }
#endif

#ifdef CONFIG_FC7300_FTU6_PWM

      /* Call fc7300_pwminitialize() to get instance 6 of the PWM interface */

      pwm = fc7300_pwminitialize(FC7300F8M_ESMALL_PWM6);
      if (!pwm)
        {
          aerr("ERROR: Failed to get the FC7300 PWM lower half\n");
          return -ENODEV;
        }

      /* Register the PWM driver at "/dev/pwm6" */

      ret = pwm_register("/dev/pwm6", pwm);
      if (ret < 0)
        {
          aerr("ERROR: pwm_register failed: %d\n", ret);
          return ret;
        }
#endif

#ifdef CONFIG_FC7300_FTU9_PWM

      /* Call fc7300_pwminitialize() to get instance 9 of the PWM interface */

      pwm = fc7300_pwminitialize(FC7300F8M_ESMALL_PWM9);
      if (!pwm)
        {
          aerr("ERROR: Failed to get the FC7300 PWM lower half\n");
          return -ENODEV;
        }

      /* Register the PWM driver at "/dev/pwm9" */

      ret = pwm_register("/dev/pwm9", pwm);
      if (ret < 0)
        {
          aerr("ERROR: pwm_register failed: %d\n", ret);
          return ret;
        }
#endif

      /* Now we are initialized */

      initialized = true;
    }

  return ret;
}
