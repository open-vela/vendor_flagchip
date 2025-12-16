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

#ifndef __VENDOR_FLAGCHIP_BOARDS_FC7300_FC7300F1M_ESMALL_SRC_FC7300F1M_ESMALL_H
#define __VENDOR_FLAGCHIP_BOARDS_FC7300_FC7300F1M_ESMALL_SRC_FC7300F1M_ESMALL_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>
#include <nuttx/compiler.h>

#include <stdint.h>

#include "hardware/fc7300_pinmux.h"
#include "fc7300_periphclocks.h"
#include "hardware/fc7300_pin.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Configuration ************************************************************/

/* FC7300F8M-EVB GPIOs ******************************************************/

/* LEDs.    The FC7300F8M-EVB has three LEDs:
 *
 *   LED0    PTA26
 *   LED1    PTD31
 *   LED2    PTA14
 *
 * An output of '1' illuminates the LED.
 */

#define GPIO_LED_0  (PIN_PTA26 | GPIO_LOWDRIVE | GPIO_OUTPUT_ZERO)
#define GPIO_LED_1  (PIN_PTD31 | GPIO_LOWDRIVE | GPIO_OUTPUT_ZERO)
#define GPIO_LED_2  (PIN_PTA14 | GPIO_LOWDRIVE | GPIO_OUTPUT_ZERO)
#define GPIO0       (PIN_PTH11 | GPIO_INPUT)    /* WP */
#define GPIO1       (PIN_PTF17 | GPIO_OUTPUT | GPIO_OUTPUT_ZERO)  /* HOLD */
#define GPIO_KEY1   (PIN_PTD20  | GPIO_INPUT | PIN_INT_FALLING)

#define NUM_OF_GPIO 2

/* NMI button used as interrupt source */

#define GPIO_KEY_NMI (PIN_PTA11 | GPIO_INPUT | PIN_INT_FALLING)

/* PWM instance definations */

#ifdef CONFIG_FC7300_PWM
#  define FC7300F8M_ESMALL_PWM0 0
#  define FC7300F8M_ESMALL_PWM1 1
#  define FC7300F8M_ESMALL_PWM2 2
#  define FC7300F8M_ESMALL_PWM3 3
#  define FC7300F8M_ESMALL_PWM4 4
#  define FC7300F8M_ESMALL_PWM5 5
#  define FC7300F8M_ESMALL_PWM6 6
#  define FC7300F8M_ESMALL_PWM7 7
#  define FC7300F8M_ESMALL_PWM8 8
#  define FC7300F8M_ESMALL_PWM9 9
#  define FC7300F8M_ESMALL_PWM10 10
#  define FC7300F8M_ESMALL_PWM11 11
#endif

/* WATCHDOG instance definations */

#ifdef CONFIG_WATCHDOG
#  define FC7300F1M_EVAL_WATCHDOG0 0
#  define FC7300F1M_EVAL_WATCHDOG1 1
#endif

/* CAP instance definations */

#ifdef CONFIG_CAPTURE
#  define FC7300F8M_EVAL_CAP_ISM(X)        X
#  define FC7300F8M_EVAL_CAP_FTU0(X)      (X + 16)
#  define FC7300F8M_EVAL_CAP_FTU1(X)      (8 + X + 16)
#  define FC7300F8M_EVAL_CAP_FTU2(X)      (16 + X + 16)
#  define FC7300F8M_EVAL_CAP_FTU3(X)      (24 + X + 16)
#  define FC7300F8M_EVAL_CAP_FTU4(X)      (32 + X + 16)
#  define FC7300F8M_EVAL_CAP_FTU5(X)      (40 + X + 16)
#  define FC7300F8M_EVAL_CAP_FTU6(X)      (48 + X + 16)
#  define FC7300F8M_EVAL_CAP_FTU7(X)      (56 + X + 16)
#  define FC7300F8M_EVAL_CAP_FTU8(X)      (64 + X + 16)
#  define FC7300F8M_EVAL_CAP_FTU9(X)      (72 + X + 16)
#  define FC7300F8M_EVAL_CAP_FTU10(X)     (80 + X + 16)
#  define FC7300F8M_EVAL_CAP_FTU11(X)     (88 + X + 16)
#endif

/****************************************************************************
 * Public Data
 ****************************************************************************/

#ifndef __ASSEMBLY__

/* User peripheral configuration structure 0 */

extern const struct peripheral_clock_config_s g_peripheral_clockconfig0[];

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

/****************************************************************************
 * Name: fc7300_bringup
 *
 * Description:
 *   Perform architecture-specific initialization
 *
 *   CONFIG_BOARD_LATE_INITIALIZE=y :
 *     Called from board_late_initialize().
 *
 *   CONFIG_BOARD_LATE_INITIALIZE=y && CONFIG_BOARDCTL=y :
 *     Called from the NSH library
 *
 ****************************************************************************/

int fc7300_bringup(void);

#endif /* __ASSEMBLY__ */
#endif /* __VENDOR_FLAGCHIP_BOARDS_FC7300_FC7300F8M_EVB_SRC_FC7300F8M_EVB_H */
