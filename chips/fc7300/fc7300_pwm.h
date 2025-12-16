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

#ifndef __VENDOR_FLAGCHIP_CHIPS_FC7300_FC7300_PWM_H
#define __VENDOR_FLAGCHIP_CHIPS_FC7300_FC7300_PWM_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>
#include <arch/board/board.h>

#include "chip.h"
#include "hardware/fc7300_pinmux.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Configuration ************************************************************/

/* Timer devices may be used for different purposes.  One special purpose is
 * to generate modulated outputs for such things as motor control.  If
 * CONFIG_FC7300_FTUn is defined then the CONFIG_FC7300_FTUn_PWM must also
 * be defined to indicate that timer "n" is intended to be used for pulsed
 * output signal generation.
 */

#ifndef CONFIG_FC7300_FTU0
#  undef CONFIG_FC7300_FTU0_PWM
#endif
#ifndef CONFIG_FC7300_FTU1
#  undef CONFIG_FC7300_FTU1_PWM
#endif
#ifndef CONFIG_FC7300_FTU2
#  undef CONFIG_FC7300_FTU2_PWM
#endif
#ifndef CONFIG_FC7300_FTU3
#  undef CONFIG_FC7300_FTU3_PWM
#endif
#ifndef CONFIG_FC7300_FTU4
#  undef CONFIG_FC7300_FTU4_PWM
#endif
#ifndef CONFIG_FC7300_FTU5
#  undef CONFIG_FC7300_FTU5_PWM
#endif
#ifndef CONFIG_FC7300_FTU6
#  undef CONFIG_FC7300_FTU6_PWM
#endif
#ifndef CONFIG_FC7300_FTU7
#  undef CONFIG_FC7300_FTU7_PWM
#endif
#ifndef CONFIG_FC7300_FTU8
#  undef CONFIG_FC7300_FTU8_PWM
#endif
#ifndef CONFIG_FC7300_FTU9
#  undef CONFIG_FC7300_FTU9_PWM
#endif
#ifndef CONFIG_FC7300_FTU10
#  undef CONFIG_FC7300_FTU10_PWM
#endif
#ifndef CONFIG_FC7300_FTU11
#  undef CONFIG_FC7300_FTU11_PWM
#endif
/****************************************************************************
 * Public Types
 ****************************************************************************/

/* Ftu pwm mode */

enum ftu_pwmpinmodetype_e
{
    FTU_EDGE_PWM_HIGH_TRUE_PULSE = 0U,
    FTU_EDGE_PWM_LOW_TRUE_PULSE,
    FTU_CENTER_PWM_HIGH_TRUE_PULSE,
    FTU_CENTER_PWM_LOW_TRUE_PULSE
};

/* FTU PWM deadtime type */

struct ftu_deadtimeparamtype
{
  /* auto calculate prescaler value for deadtime, in/out parameter */

  uint8_t prescaler;

  /* auto calculate deadtime value, in/out parameter */

  uint16_t deadtimeval;
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

#if defined(CONFIG_FC7300_FTU0_PWM)  || defined(CONFIG_FC7300_FTU1_PWM)  || \
    defined(CONFIG_FC7300_FTU2_PWM)  || defined(CONFIG_FC7300_FTU3_PWM)  || \
    defined(CONFIG_FC7300_FTU4_PWM)  || defined(CONFIG_FC7300_FTU5_PWM)  || \
    defined(CONFIG_FC7300_FTU6_PWM)  || defined(CONFIG_FC7300_FTU7_PWM)  || \
    defined(CONFIG_FC7300_FTU8_PWM)  || defined(CONFIG_FC7300_FTU9_PWM)  || \
    defined(CONFIG_FC7300_FTU10_PWM) || defined(CONFIG_FC7300_FTU11_PWM)
/****************************************************************************
 * Name: fc7300_pwminitialize
 *
 * Description:
 *   Initialize one timer for use with the upper_level PWM driver.
 *
 * Input Parameters:
 *   timer - A number identifying the timer use.
 *
 * Returned Value:
 *   On success, a pointer to the FC7300 lower half PWM driver is returned.
 *   NULL is returned on any failure.
 *
 ****************************************************************************/

struct pwm_lowerhalf_s *fc7300_pwminitialize(int timer);

/****************************************************************************
 * Name: fc7300_pwm_initialize
 *
 * Description:
 *   Initialize PWM and register the PWM device.
 *
 ****************************************************************************/

int fc7300_pwm_initialize(void);

#endif /* CONFIG_FC7300_FTUx_PWM */

#undef EXTERN
#if defined(__cplusplus)
}
#endif

#endif /* __ASSEMBLY__ */
#endif /* __VENDOR_FLAGCHIP_CHIPS_FC7300_FC7300_PWM_H */
