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

#include <stdint.h>
#include <time.h>
#include <assert.h>
#include <debug.h>

#include <nuttx/arch.h>
#include <nuttx/timers/arch_timer.h>
#include <nuttx/timers/oneshot.h>
#include <nuttx/timers/arch_alarm.h>
#include <arch/board/board.h>
#include "systick.h"
#include "nvic.h"
#include "arm_internal.h"
#include "clock/clock.h"
#include "fc7300_clockconfig.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* The SysTick clock input (Fsystick) is determined by the CLKSOURCE file of
 * the SysTick CSR register:  The CLKSOURCE field in SysTick Control and
 * Status register selects either the core clock (when CLKSOURCE = 1) or a
 * divide-by-16 of the core clock (when CLKSOURCE = 0).
 *
 * The desired timer interrupt frequency is provided by the definition
 * CLK_TCK (see include/time.h).  CLK_TCK defines the desired number of
 * system clock ticks per second.  That value is a user configurable setting
 * that defaults to 100 (100 ticks per second = 10 MS interval).
 *
 *    reload = (Fsystick / CLK_TICK) - 1
 */

#define SYSTICK_RELOAD(coreclk) (((coreclk) / CLK_TCK) - 1)

/****************************************************************************
 * Private Functions
 ****************************************************************************/

#if !(defined(CONFIG_ALARM_ARCH) && defined(CONFIG_ONESHOT)) && \
    !(defined(CONFIG_ARMV7M_SYSTICK) && defined(CONFIG_TIMER_ARCH))
/****************************************************************************
 * Function:  fc7300_timerisr
 *
 * Description:
 *   The timer ISR will perform a variety of services for various portions
 *   of the systems.
 *
 ****************************************************************************/

__ramfunc__ static int fc7300_timerisr(int irq, uint32_t *regs, void *arg)
{
  /* Process timer interrupt */

  nxsched_process_timer();
  return 0;
}
#endif

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Function:  up_timer_initialize
 *
 * Description:
 *   This function is called during start-up to initialize
 *   the timer interrupt.
 *
 ****************************************************************************/

void up_timer_initialize(void)
{
#if defined(CONFIG_ALARM_ARCH) && defined(CONFIG_ONESHOT)
#if defined(CONFIG_FC7300_TSTMP1_CH0_USED_FOR_TICK) || \
    defined(CONFIG_FC7300_TSTMP2_CH0_USED_FOR_TICK) || \
    defined(CONFIG_FC7300_TSTMP3_CH0_USED_FOR_TICK)
  up_alarm_set_lowerhalf(oneshot_initialize(0, 1));
#endif
#if defined(CONFIG_ARMV7M_SYSTICK)
  systick_initialize(true, FC7300_SYSCLK_FREQUENCY, 0);
#endif
#elif defined(CONFIG_ARMV7M_SYSTICK) && defined(CONFIG_TIMER_ARCH)
  up_timer_set_lowerhalf(systick_initialize(true, FC7300_SYSCLK_FREQUENCY,
                                            -1));
#else
  uint32_t coreclk;
  uint32_t reload;

  /* Make sure that the SYSTICK clock source is set to use the SysTick
   * function clock (CLKSOURCE==1).
   */

  putreg32(NVIC_SYSTICK_CTRL_CLKSOURCE, NVIC_SYSTICK_CTRL);

  /* Get the reload value */

  coreclk = fc7300_get_coreclk();
  reload  = SYSTICK_RELOAD(coreclk);

  /* The size of the reload field is 24 bits. */

  DEBUGASSERT(reload <= 0x00ffffff);

  /* Configure SysTick to interrupt at the requested rate */

  putreg32(reload, NVIC_SYSTICK_RELOAD);

  /* Attach the timer interrupt vector */

  irq_attach(FC7300_IRQ_SYSTICK, (xcpt_t)fc7300_timerisr, NULL);

  /* Enable SysTick interrupts */

  putreg32((NVIC_SYSTICK_CTRL_CLKSOURCE | NVIC_SYSTICK_CTRL_TICKINT |
            NVIC_SYSTICK_CTRL_ENABLE), NVIC_SYSTICK_CTRL);

  /* And enable the timer interrupt */

  up_enable_irq(FC7300_IRQ_SYSTICK);
#endif
}
