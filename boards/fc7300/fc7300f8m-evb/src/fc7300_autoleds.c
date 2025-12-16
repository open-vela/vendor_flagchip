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

/* The FC7300F8M-EVB has three LEDs:
 *
 *   GPIO_LED_0    PIN_PTA26
 *   GPIO_LED_1    PIN_PTD31
 *   GPIO_LED_2    PIN_PTA14
 * An output of '1' illuminates the LED.
 *
 */

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>

#include <stdint.h>
#include <stdbool.h>

#include <nuttx/board.h>

#include "fc7300f8m-evb.h"

#ifdef CONFIG_ARCH_LEDS

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Summary of all possible settings */

#define LED_NOCHANGE     0 /* LED_IRQSENABLED, LED_INIRQ, LED_SIGNAL, LED_ASSERTION */
#define LED_OFF_OFF_OFF  1 /* LED_STARTED */
#define LED_OFF_OFF_ON   2 /* LED_HEAPALLOCATE */
#define LED_OFF_ON_OFF   3 /* LED_STACKCREATED */
#define LED_ON_OFF_OFF   4 /* LED_PANIC */

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: board_autoled_initialize
 ****************************************************************************/

void board_autoled_initialize(void)
{
  /* Configure LED GPIOs for output */

  fc7300_pinconfig(GPIO_LED_0);
  fc7300_pinconfig(GPIO_LED_1);
  fc7300_pinconfig(GPIO_LED_2);
}

/****************************************************************************
 * Name: board_autoled_on
 ****************************************************************************/

void board_autoled_on(int led)
{
  if (led != LED_NOCHANGE)
    {
      bool led0 = false;
      bool led1 = false;
      bool led2 = false;

      switch (led)
        {
          default:
          case LED_OFF_OFF_OFF:
            break;

          case LED_OFF_OFF_ON:
            led2 = true;
            break;

          case LED_OFF_ON_OFF:
            led1 = true;
            break;

          case LED_ON_OFF_OFF:
            led0 = true;
            break;
        }

      /* Invert output, an output of '1' illuminates the LED */

      fc7300_gpiowrite(GPIO_LED_0, led0);
      fc7300_gpiowrite(GPIO_LED_1, led1);
      fc7300_gpiowrite(GPIO_LED_2, led2);
    }
}

/****************************************************************************
 * Name: board_autoled_off
 ****************************************************************************/

void board_autoled_off(int led)
{
  if (led == LED_ON_OFF_OFF)
    {
      /* Invert outputs, an output of '1' illuminates the LED */

      fc7300_gpiowrite(GPIO_LED_0, false);
      fc7300_gpiowrite(GPIO_LED_1, false);
      fc7300_gpiowrite(GPIO_LED_2, false);
    }
}

#endif /* CONFIG_ARCH_LEDS */
