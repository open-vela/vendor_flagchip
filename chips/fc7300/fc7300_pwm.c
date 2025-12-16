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
#include <stdio.h>
#include <assert.h>
#include <errno.h>
#include <debug.h>

#include <nuttx/irq.h>
#include <nuttx/arch.h>
#include <nuttx/timers/pwm.h>
#include <arch/board/board.h>
#include <nuttx/spinlock.h>

#include "arm_internal.h"
#include "chip.h"

#include "fc7300_clockconfig.h"
#include "fc7300_pwm.h"
#include "hardware/fc7300_gpio.h"
#include "hardware/fc7300_pin.h"
#include "hardware/fc7300_ftu.h"

/* This module then only compiles if there is at least one enabled timer
 * intended for use with the PWM upper half driver.
 */

#if defined(CONFIG_FC7300_FTU0_PWM) || defined(CONFIG_FC7300_FTU1_PWM) || \
    defined(CONFIG_FC7300_FTU2_PWM) || defined(CONFIG_FC7300_FTU3_PWM) || \
    defined(CONFIG_FC7300_FTU4_PWM) || defined(CONFIG_FC7300_FTU5_PWM) || \
    defined(CONFIG_FC7300_FTU6_PWM) || defined(CONFIG_FC7300_FTU7_PWM) || \
    defined(CONFIG_FC7300_FTU8_PWM) || defined(CONFIG_FC7300_FTU9_PWM) || \
    defined(CONFIG_FC7300_FTU10_PWM) || defined(CONFIG_FC7300_FTU11_PWM)

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Calculate the number of the channel of instance Timer n  *****************/

#define PWM_ARRAY_SIZE(n) (sizeof(g_pwm##n##channels) / sizeof(g_pwm##n##channels[0]))

/****************************************************************************
 * Private Types
 ****************************************************************************/

/* This structure represents the state of one PWM timer */

struct fc7300_pwmtimer_s
{
  const struct pwm_ops_s   *ops;          /* PWM operations */
  uint8_t                   tpmid;        /* Timer/PWM Module ID {0,..,11} */
  uint8_t                   chan_num;     /* Number of configured channels */
  struct fc7300_pwmchan_s   *channels;    /* Timer/PWM Module channel: {0,...,7} */
  uint32_t                  base;         /* The base address of the timer */
  uint32_t                  pclk;         /* The frequency of peripheral clock */
  enum ftu_pwmpinmodetype_e mode;         /* The ftu pwm mode */
  uint32_t                  deadtimefreq; /* pwm deadtime frequency (hz) */
};

/* PWM channel configuration */

struct fc7300_pwmchan_s
{
  uint8_t                  channel;     /* Timer output channel  */
  bool                     deadtimeen;  /* PWM channel deadtime enable */
  uint32_t                 pincfg;      /* Output pin configuration */
};
/****************************************************************************
 * Static Function Prototypes
 ****************************************************************************/

/* Register access */

static uint32_t fc7300_pwm_getreg(struct fc7300_pwmtimer_s *priv,
                         int offset);
static void fc7300_pwm_putreg(struct fc7300_pwmtimer_s *priv,
                         int offset, uint32_t value);

static void fc7300_pwm_dumpregs(struct fc7300_pwmtimer_s *priv,
                         const char *msg);

/* Timer management */

static int fc7300_pwm_timer(struct fc7300_pwmtimer_s *priv,
                     const struct pwm_info_s *info);

/* PWM driver methods */

static int fc7300_pwm_setup(struct pwm_lowerhalf_s *dev);
static int fc7300_pwm_shutdown(struct pwm_lowerhalf_s *dev);

static int fc7300_pwm_start(struct pwm_lowerhalf_s *dev,
                     const struct pwm_info_s *info);

static int fc7300_pwm_stop(struct pwm_lowerhalf_s *dev);
static int fc7300_pwm_ioctl(struct pwm_lowerhalf_s *dev, int cmd,
                     unsigned long arg);

/****************************************************************************
 * Private Data
 ****************************************************************************/

/* This is the list of lower half PWM driver methods used by the upper half
 * driver.
 */

static const struct pwm_ops_s g_pwmops =
{
  .setup    = fc7300_pwm_setup,
  .shutdown = fc7300_pwm_shutdown,
  .start    = fc7300_pwm_start,
  .stop     = fc7300_pwm_stop,
  .ioctl    = fc7300_pwm_ioctl,
};

#ifdef CONFIG_FC7300_FTU0_PWM

static struct fc7300_pwmchan_s g_pwm0channels[] =
{
  /* FTU0 has 8 channels */

#ifdef CONFIG_FC7300_FTU0_PWM_CHANNEL0
  {
    .channel = 0,
#ifdef CONFIG_FC7300_FTU0_PWM_CH0_DEADTIMEEN
    .deadtimeen = true,
#endif
    .pincfg = GPIO_FTU0_CH0OUT,
  },
#endif
#ifdef CONFIG_FC7300_FTU0_PWM_CHANNEL1
  {
    .channel = 1,
#ifdef CONFIG_FC7300_FTU0_PWM_CH1_DEADTIMEEN
    .deadtimeen = true,
#endif
    .pincfg = GPIO_FTU0_CH1OUT,
  },
#endif
#ifdef CONFIG_FC7300_FTU0_PWM_CHANNEL2
  {
    .channel = 2,
#ifdef CONFIG_FC7300_FTU0_PWM_CH2_DEADTIMEEN
    .deadtimeen = true,
#endif
    .pincfg = GPIO_FTU0_CH2OUT,
  },
#endif
#ifdef CONFIG_FC7300_FTU0_PWM_CHANNEL3
  {
    .channel = 3,
#ifdef CONFIG_FC7300_FTU0_PWM_CH3_DEADTIMEEN
    .deadtimeen = true,
#endif
    .pincfg = GPIO_FTU0_CH3OUT,
  },
#endif
#ifdef CONFIG_FC7300_FTU0_PWM_CHANNEL4
  {
    .channel = 4,
#ifdef CONFIG_FC7300_FTU0_PWM_CH4_DEADTIMEEN
    .deadtimeen = true,
#endif
    .pincfg = GPIO_FTU0_CH4OUT,
  },
#endif
#ifdef CONFIG_FC7300_FTU0_PWM_CHANNEL5
  {
    .channel = 5,
#ifdef CONFIG_FC7300_FTU0_PWM_CH5_DEADTIMEEN
    .deadtimeen = true,
#endif
    .pincfg = GPIO_FTU0_CH5OUT,
  },
#endif
#ifdef CONFIG_FC7300_FTU0_PWM_CHANNEL6
  {
    .channel = 6,
#ifdef CONFIG_FC7300_FTU0_PWM_CH6_DEADTIMEEN
    .deadtimeen = true,
#endif
    .pincfg = GPIO_FTU0_CH6OUT,
  },
#endif
#ifdef CONFIG_FC7300_FTU0_PWM_CHANNEL7
  {
    .channel = 7,
#ifdef CONFIG_FC7300_FTU0_PWM_CH7_DEADTIMEEN
    .deadtimeen = true,
#endif
    .pincfg = GPIO_FTU0_CH7OUT,
  },
#endif
};

static struct fc7300_pwmtimer_s g_pwm0dev =
{
  .ops          = &g_pwmops,
  .tpmid        = 0,
  .chan_num     = PWM_ARRAY_SIZE(0),
  .channels     = g_pwm0channels,
  .base         = FC7300_FTU0_BASE,
  .mode         = (enum ftu_pwmpinmodetype_e)CONFIG_FC7300_FTU0_PWM_MODE,
  .deadtimefreq = CONFIG_FC7300_FTU0_PWM_DEADTIMEFREQ,
};
#endif

#ifdef CONFIG_FC7300_FTU1_PWM

static struct fc7300_pwmchan_s g_pwm1channels[] =
{
  /* FTU1 has 8 channels */

#ifdef CONFIG_FC7300_FTU1_PWM_CHANNEL0
  {
    .channel = 0,
#ifdef CONFIG_FC7300_FTU1_PWM_CH0_DEADTIMEEN
    .deadtimeen = true,
#endif
    .pincfg = GPIO_FTU1_CH0OUT,
  },
#endif
#ifdef CONFIG_FC7300_FTU1_PWM_CHANNEL1
  {
    .channel = 1,
#ifdef CONFIG_FC7300_FTU1_PWM_CH1_DEADTIMEEN
    .deadtimeen = true,
#endif
    .pincfg = GPIO_FTU1_CH1OUT,
  },
#endif
#ifdef CONFIG_FC7300_FTU1_PWM_CHANNEL2
  {
    .channel = 2,
#ifdef CONFIG_FC7300_FTU1_PWM_CH2_DEADTIMEEN
    .deadtimeen = true,
#endif
    .pincfg = GPIO_FTU1_CH2OUT,
  },
#endif
#ifdef CONFIG_FC7300_FTU1_PWM_CHANNEL3
  {
    .channel = 3,
#ifdef CONFIG_FC7300_FTU1_PWM_CH3_DEADTIMEEN
    .deadtimeen = true,
#endif
    .pincfg = GPIO_FTU1_CH3OUT,
  },
#endif
#ifdef CONFIG_FC7300_FTU1_PWM_CHANNEL4
  {
    .channel = 4,
#ifdef CONFIG_FC7300_FTU1_PWM_CH4_DEADTIMEEN
    .deadtimeen = true,
#endif
    .pincfg = GPIO_FTU1_CH4OUT,
  },
#endif
#ifdef CONFIG_FC7300_FTU1_PWM_CHANNEL5
  {
    .channel = 5,
#ifdef CONFIG_FC7300_FTU1_PWM_CH5_DEADTIMEEN
    .deadtimeen = true,
#endif
    .pincfg = GPIO_FTU1_CH5OUT,
  },
#endif
#ifdef CONFIG_FC7300_FTU1_PWM_CHANNEL6
  {
    .channel = 6,
#ifdef CONFIG_FC7300_FTU1_PWM_CH6_DEADTIMEEN
    .deadtimeen = true,
#endif
    .pincfg = GPIO_FTU1_CH6OUT,
  },
#endif
#ifdef CONFIG_FC7300_FTU1_PWM_CHANNEL7
  {
    .channel = 7,
#ifdef CONFIG_FC7300_FTU1_PWM_CH7_DEADTIMEEN
    .deadtimeen = true,
#endif
    .pincfg = GPIO_FTU1_CH7OUT,
  },
#endif
};

static struct fc7300_pwmtimer_s g_pwm1dev =
{
  .ops          = &g_pwmops,
  .tpmid        = 1,
  .chan_num     = PWM_ARRAY_SIZE(1),
  .channels     = g_pwm1channels,
  .base         = FC7300_FTU1_BASE,
  .mode         = (enum ftu_pwmpinmodetype_e)CONFIG_FC7300_FTU1_PWM_MODE,
  .deadtimefreq = CONFIG_FC7300_FTU1_PWM_DEADTIMEFREQ,
};
#endif

#ifdef CONFIG_FC7300_FTU2_PWM

static struct fc7300_pwmchan_s g_pwm2channels[] =
{
  /* FTU2 has 8 channels */

#ifdef CONFIG_FC7300_FTU2_PWM_CHANNEL0
  {
    .channel = 0,
#ifdef CONFIG_FC7300_FTU2_PWM_CH0_DEADTIMEEN
    .deadtimeen = true,
#endif
    .pincfg = GPIO_FTU2_CH0OUT,
  },
#endif
#ifdef CONFIG_FC7300_FTU2_PWM_CHANNEL1
  {
    .channel = 1,
#ifdef CONFIG_FC7300_FTU2_PWM_CH1_DEADTIMEEN
    .deadtimeen = true,
#endif
    .pincfg = GPIO_FTU2_CH1OUT,
  },
#endif
#ifdef CONFIG_FC7300_FTU2_PWM_CHANNEL2
  {
    .channel = 2,
#ifdef CONFIG_FC7300_FTU2_PWM_CH2_DEADTIMEEN
    .deadtimeen = true,
#endif
    .pincfg = GPIO_FTU2_CH2OUT,
  },
#endif
#ifdef CONFIG_FC7300_FTU2_PWM_CHANNEL3
  {
    .channel = 3,
#ifdef CONFIG_FC7300_FTU2_PWM_CH3_DEADTIMEEN
    .deadtimeen = true,
#endif
    .pincfg = GPIO_FTU2_CH3OUT,
  },
#endif
#ifdef CONFIG_FC7300_FTU2_PWM_CHANNEL4
  {
    .channel = 4,
#ifdef CONFIG_FC7300_FTU2_PWM_CH4_DEADTIMEEN
    .deadtimeen = true,
#endif
    .pincfg = GPIO_FTU2_CH4OUT,
  },
#endif
#ifdef CONFIG_FC7300_FTU2_PWM_CHANNEL5
  {
    .channel = 5,
#ifdef CONFIG_FC7300_FTU2_PWM_CH5_DEADTIMEEN
    .deadtimeen = true,
#endif
    .pincfg = GPIO_FTU2_CH5OUT,
  },
#endif
#ifdef CONFIG_FC7300_FTU2_PWM_CHANNEL6
  {
    .channel = 6,
#ifdef CONFIG_FC7300_FTU2_PWM_CH6_DEADTIMEEN
    .deadtimeen = true,
#endif
    .pincfg = GPIO_FTU2_CH6OUT,
  },
#endif
#ifdef CONFIG_FC7300_FTU2_PWM_CHANNEL7
  {
    .channel = 7,
#ifdef CONFIG_FC7300_FTU2_PWM_CH7_DEADTIMEEN
    .deadtimeen = true,
#endif
    .pincfg = GPIO_FTU2_CH7OUT,
  },
#endif
};

static struct fc7300_pwmtimer_s g_pwm2dev =
{
  .ops          = &g_pwmops,
  .tpmid        = 2,
  .chan_num     = PWM_ARRAY_SIZE(2),
  .channels     = g_pwm2channels,
  .base         = FC7300_FTU2_BASE,
  .mode         = (enum ftu_pwmpinmodetype_e)CONFIG_FC7300_FTU2_PWM_MODE,
  .deadtimefreq = CONFIG_FC7300_FTU2_PWM_DEADTIMEFREQ,
};
#endif

#ifdef CONFIG_FC7300_FTU3_PWM

static struct fc7300_pwmchan_s g_pwm3channels[] =
{
  /* FTU3 has 8 channels */

#ifdef CONFIG_FC7300_FTU3_PWM_CHANNEL0
  {
    .channel = 0,
#ifdef CONFIG_FC7300_FTU3_PWM_CH0_DEADTIMEEN
    .deadtimeen = true,
#endif
    .pincfg = GPIO_FTU3_CH0OUT,
  },
#endif
#ifdef CONFIG_FC7300_FTU3_PWM_CHANNEL1
  {
    .channel = 1,
#ifdef CONFIG_FC7300_FTU3_PWM_CH1_DEADTIMEEN
    .deadtimeen = true,
#endif
    .pincfg = GPIO_FTU3_CH1OUT,
  },
#endif
#ifdef CONFIG_FC7300_FTU3_PWM_CHANNEL2
  {
    .channel = 2,
#ifdef CONFIG_FC7300_FTU3_PWM_CH2_DEADTIMEEN
    .deadtimeen = true,
#endif
    .pincfg = GPIO_FTU3_CH2OUT,
  },
#endif
#ifdef CONFIG_FC7300_FTU3_PWM_CHANNEL3
  {
    .channel = 3,
#ifdef CONFIG_FC7300_FTU3_PWM_CH3_DEADTIMEEN
    .deadtimeen = true,
#endif
    .pincfg = GPIO_FTU3_CH3OUT,
  },
#endif
#ifdef CONFIG_FC7300_FTU3_PWM_CHANNEL4
  {
    .channel = 4,
#ifdef CONFIG_FC7300_FTU3_PWM_CH4_DEADTIMEEN
    .deadtimeen = true,
#endif
    .pincfg = GPIO_FTU3_CH4OUT,
  },
#endif
#ifdef CONFIG_FC7300_FTU3_PWM_CHANNEL5
  {
    .channel = 5,
#ifdef CONFIG_FC7300_FTU3_PWM_CH5_DEADTIMEEN
    .deadtimeen = true,
#endif
    .pincfg = GPIO_FTU3_CH5OUT,
  },
#endif
#ifdef CONFIG_FC7300_FTU3_PWM_CHANNEL6
  {
    .channel = 6,
#ifdef CONFIG_FC7300_FTU3_PWM_CH6_DEADTIMEEN
    .deadtimeen = true,
#endif
    .pincfg = GPIO_FTU3_CH6OUT,
  },
#endif
#ifdef CONFIG_FC7300_FTU3_PWM_CHANNEL7
  {
    .channel = 7,
#ifdef CONFIG_FC7300_FTU3_PWM_CH7_DEADTIMEEN
    .deadtimeen = true,
#endif
    .pincfg = GPIO_FTU3_CH7OUT,
  },
#endif
};

static struct fc7300_pwmtimer_s g_pwm3dev =
{
  .ops          = &g_pwmops,
  .tpmid        = 3,
  .chan_num     = PWM_ARRAY_SIZE(3),
  .channels     = g_pwm3channels,
  .base         = FC7300_FTU3_BASE,
  .mode         = (enum ftu_pwmpinmodetype_e)CONFIG_FC7300_FTU3_PWM_MODE,
  .deadtimefreq = CONFIG_FC7300_FTU3_PWM_DEADTIMEFREQ,
};
#endif

#ifdef CONFIG_FC7300_FTU4_PWM

static struct fc7300_pwmchan_s g_pwm4channels[] =
{
  /* FTU4 has 8 channels */

#ifdef CONFIG_FC7300_FTU4_PWM_CHANNEL0
  {
    .channel = 0,
#ifdef CONFIG_FC7300_FTU4_PWM_CH0_DEADTIMEEN
    .deadtimeen = true,
#endif
    .pincfg = GPIO_FTU4_CH0OUT,
  },
#endif
#ifdef CONFIG_FC7300_FTU4_PWM_CHANNEL1
  {
    .channel = 1,
#ifdef CONFIG_FC7300_FTU4_PWM_CH1_DEADTIMEEN
    .deadtimeen = true,
#endif
    .pincfg = GPIO_FTU4_CH1OUT,
  },
#endif
#ifdef CONFIG_FC7300_FTU4_PWM_CHANNEL2
  {
    .channel = 2,
#ifdef CONFIG_FC7300_FTU4_PWM_CH2_DEADTIMEEN
    .deadtimeen = true,
#endif
    .pincfg = GPIO_FTU4_CH2OUT,
  },
#endif
#ifdef CONFIG_FC7300_FTU4_PWM_CHANNEL3
  {
    .channel = 3,
#ifdef CONFIG_FC7300_FTU4_PWM_CH3_DEADTIMEEN
    .deadtimeen = true,
#endif
    .pincfg = GPIO_FTU4_CH3OUT,
  },
#endif
#ifdef CONFIG_FC7300_FTU4_PWM_CHANNEL4
  {
    .channel = 4,
#ifdef CONFIG_FC7300_FTU4_PWM_CH4_DEADTIMEEN
    .deadtimeen = true,
#endif
    .pincfg = GPIO_FTU4_CH4OUT,
  },
#endif
#ifdef CONFIG_FC7300_FTU4_PWM_CHANNEL5
  {
    .channel = 5,
#ifdef CONFIG_FC7300_FTU4_PWM_CH5_DEADTIMEEN
    .deadtimeen = true,
#endif
    .pincfg = GPIO_FTU4_CH5OUT,
  },
#endif
#ifdef CONFIG_FC7300_FTU4_PWM_CHANNEL6
  {
    .channel = 6,
#ifdef CONFIG_FC7300_FTU4_PWM_CH6_DEADTIMEEN
    .deadtimeen = true,
#endif
    .pincfg = GPIO_FTU4_CH6OUT,
  },
#endif
#ifdef CONFIG_FC7300_FTU4_PWM_CHANNEL7
  {
    .channel = 7,
#ifdef CONFIG_FC7300_FTU4_PWM_CH7_DEADTIMEEN
    .deadtimeen = true,
#endif
    .pincfg = GPIO_FTU4_CH7OUT,
  },
#endif
};

static struct fc7300_pwmtimer_s g_pwm4dev =
{
  .ops          = &g_pwmops,
  .tpmid        = 4,
  .chan_num     = PWM_ARRAY_SIZE(4),
  .channels     = g_pwm4channels,
  .base         = FC7300_FTU4_BASE,
  .mode         = (enum ftu_pwmpinmodetype_e)CONFIG_FC7300_FTU4_PWM_MODE,
  .deadtimefreq = CONFIG_FC7300_FTU4_PWM_DEADTIMEFREQ,
};
#endif

#ifdef CONFIG_FC7300_FTU5_PWM

static struct fc7300_pwmchan_s g_pwm5channels[] =
{
  /* FTU5 has 8 channels */

#ifdef CONFIG_FC7300_FTU5_PWM_CHANNEL0
  {
    .channel = 0,
#ifdef CONFIG_FC7300_FTU5_PWM_CH0_DEADTIMEEN
    .deadtimeen = true,
#endif
    .pincfg = GPIO_FTU5_CH0OUT,
  },
#endif
#ifdef CONFIG_FC7300_FTU5_PWM_CHANNEL1
  {
    .channel = 1,
#ifdef CONFIG_FC7300_FTU5_PWM_CH1_DEADTIMEEN
    .deadtimeen = true,
#endif
    .pincfg = GPIO_FTU5_CH1OUT,
  },
#endif
#ifdef CONFIG_FC7300_FTU5_PWM_CHANNEL2
  {
    .channel = 2,
#ifdef CONFIG_FC7300_FTU5_PWM_CH2_DEADTIMEEN
    .deadtimeen = true,
#endif
    .pincfg = GPIO_FTU5_CH2OUT,
  },
#endif
#ifdef CONFIG_FC7300_FTU5_PWM_CHANNEL3
  {
    .channel = 3,
#ifdef CONFIG_FC7300_FTU5_PWM_CH3_DEADTIMEEN
    .deadtimeen = true,
#endif
    .pincfg = GPIO_FTU5_CH3OUT,
  },
#endif
#ifdef CONFIG_FC7300_FTU5_PWM_CHANNEL4
  {
    .channel = 4,
#ifdef CONFIG_FC7300_FTU5_PWM_CH4_DEADTIMEEN
    .deadtimeen = true,
#endif
    .pincfg = GPIO_FTU5_CH4OUT,
  },
#endif
#ifdef CONFIG_FC7300_FTU5_PWM_CHANNEL5
  {
    .channel = 5,
#ifdef CONFIG_FC7300_FTU5_PWM_CH5_DEADTIMEEN
    .deadtimeen = true,
#endif
    .pincfg = GPIO_FTU5_CH5OUT,
  },
#endif
#ifdef CONFIG_FC7300_FTU5_PWM_CHANNEL6
  {
    .channel = 6,
#ifdef CONFIG_FC7300_FTU5_PWM_CH6_DEADTIMEEN
    .deadtimeen = true,
#endif
    .pincfg = GPIO_FTU5_CH6OUT,
  },
#endif
#ifdef CONFIG_FC7300_FTU5_PWM_CHANNEL7
  {
    .channel = 7,
#ifdef CONFIG_FC7300_FTU5_PWM_CH7_DEADTIMEEN
    .deadtimeen = true,
#endif
    .pincfg = GPIO_FTU5_CH7OUT,
  },
#endif
};

static struct fc7300_pwmtimer_s g_pwm5dev =
{
  .ops          = &g_pwmops,
  .tpmid        = 5,
  .chan_num     = PWM_ARRAY_SIZE(5),
  .channels     = g_pwm5channels,
  .base         = FC7300_FTU5_BASE,
  .mode         = (enum ftu_pwmpinmodetype_e)CONFIG_FC7300_FTU5_PWM_MODE,
  .deadtimefreq = CONFIG_FC7300_FTU5_PWM_DEADTIMEFREQ,
};
#endif

#ifdef CONFIG_FC7300_FTU6_PWM

static struct fc7300_pwmchan_s g_pwm6channels[] =
{
  /* FTU6 has 8 channels */

#ifdef CONFIG_FC7300_FTU6_PWM_CHANNEL0
  {
    .channel = 0,
#ifdef CONFIG_FC7300_FTU6_PWM_CH0_DEADTIMEEN
    .deadtimeen = true,
#endif
    .pincfg = GPIO_FTU6_CH0OUT,
  },
#endif
#ifdef CONFIG_FC7300_FTU6_PWM_CHANNEL1
  {
    .channel = 1,
#ifdef CONFIG_FC7300_FTU6_PWM_CH1_DEADTIMEEN
    .deadtimeen = true,
#endif
    .pincfg = GPIO_FTU6_CH1OUT,
  },
#endif
#ifdef CONFIG_FC7300_FTU6_PWM_CHANNEL2
  {
    .channel = 2,
#ifdef CONFIG_FC7300_FTU6_PWM_CH2_DEADTIMEEN
    .deadtimeen = true,
#endif
    .pincfg = GPIO_FTU6_CH2OUT,
  },
#endif
#ifdef CONFIG_FC7300_FTU6_PWM_CHANNEL3
  {
    .channel = 3,
#ifdef CONFIG_FC7300_FTU6_PWM_CH3_DEADTIMEEN
    .deadtimeen = true,
#endif
    .pincfg = GPIO_FTU6_CH3OUT,
  },
#endif
#ifdef CONFIG_FC7300_FTU6_PWM_CHANNEL4
  {
    .channel = 4,
#ifdef CONFIG_FC7300_FTU6_PWM_CH4_DEADTIMEEN
    .deadtimeen = true,
#endif
    .pincfg = GPIO_FTU6_CH4OUT,
  },
#endif
#ifdef CONFIG_FC7300_FTU6_PWM_CHANNEL5
  {
    .channel = 5,
#ifdef CONFIG_FC7300_FTU6_PWM_CH5_DEADTIMEEN
    .deadtimeen = true,
#endif
    .pincfg = GPIO_FTU6_CH5OUT,
  },
#endif
#ifdef CONFIG_FC7300_FTU6_PWM_CHANNEL6
  {
    .channel = 6,
#ifdef CONFIG_FC7300_FTU6_PWM_CH6_DEADTIMEEN
    .deadtimeen = true,
#endif
    .pincfg = GPIO_FTU6_CH6OUT,
  },
#endif
#ifdef CONFIG_FC7300_FTU6_PWM_CHANNEL7
  {
    .channel = 7,
#ifdef CONFIG_FC7300_FTU6_PWM_CH7_DEADTIMEEN
    .deadtimeen = true,
#endif
    .pincfg = GPIO_FTU6_CH7OUT,
  },
#endif
};

static struct fc7300_pwmtimer_s g_pwm6dev =
{
  .ops          = &g_pwmops,
  .tpmid        = 6,
  .chan_num     = PWM_ARRAY_SIZE(6),
  .channels     = g_pwm6channels,
  .base         = FC7300_FTU6_BASE,
  .mode         = (enum ftu_pwmpinmodetype_e)CONFIG_FC7300_FTU6_PWM_MODE,
  .deadtimefreq = CONFIG_FC7300_FTU6_PWM_DEADTIMEFREQ,
};
#endif

#ifdef CONFIG_FC7300_FTU7_PWM

static struct fc7300_pwmchan_s g_pwm7channels[] =
{
  /* FTU7 has 8 channels */

#ifdef CONFIG_FC7300_FTU7_PWM_CHANNEL0
  {
    .channel = 0,
#ifdef CONFIG_FC7300_FTU7_PWM_CH0_DEADTIMEEN
    .deadtimeen = true,
#endif
    .pincfg = GPIO_FTU7_CH0OUT,
  },
#endif
#ifdef CONFIG_FC7300_FTU7_PWM_CHANNEL1
  {
    .channel = 1,
#ifdef CONFIG_FC7300_FTU7_PWM_CH1_DEADTIMEEN
    .deadtimeen = true,
#endif
    .pincfg = GPIO_FTU7_CH1OUT,
  },
#endif
#ifdef CONFIG_FC7300_FTU7_PWM_CHANNEL2
  {
    .channel = 2,
#ifdef CONFIG_FC7300_FTU7_PWM_CH2_DEADTIMEEN
    .deadtimeen = true,
#endif
    .pincfg = GPIO_FTU7_CH2OUT,
  },
#endif
#ifdef CONFIG_FC7300_FTU7_PWM_CHANNEL3
  {
    .channel = 3,
#ifdef CONFIG_FC7300_FTU7_PWM_CH3_DEADTIMEEN
    .deadtimeen = true,
#endif
    .pincfg = GPIO_FTU7_CH3OUT,
  },
#endif
#ifdef CONFIG_FC7300_FTU7_PWM_CHANNEL4
  {
    .channel = 4,
#ifdef CONFIG_FC7300_FTU7_PWM_CH4_DEADTIMEEN
    .deadtimeen = true,
#endif
    .pincfg = GPIO_FTU7_CH4OUT,
  },
#endif
#ifdef CONFIG_FC7300_FTU7_PWM_CHANNEL5
  {
    .channel = 5,
#ifdef CONFIG_FC7300_FTU7_PWM_CH5_DEADTIMEEN
    .deadtimeen = true,
#endif
    .pincfg = GPIO_FTU7_CH5OUT,
  },
#endif
#ifdef CONFIG_FC7300_FTU7_PWM_CHANNEL6
  {
    .channel = 6,
#ifdef CONFIG_FC7300_FTU7_PWM_CH6_DEADTIMEEN
    .deadtimeen = true,
#endif
    .pincfg = GPIO_FTU7_CH6OUT,
  },
#endif
#ifdef CONFIG_FC7300_FTU7_PWM_CHANNEL7
  {
    .channel = 7,
#ifdef CONFIG_FC7300_FTU7_PWM_CH7_DEADTIMEEN
    .deadtimeen = true,
#endif
    .pincfg = GPIO_FTU7_CH7OUT,
  },
#endif
};

static struct fc7300_pwmtimer_s g_pwm7dev =
{
  .ops          = &g_pwmops,
  .tpmid        = 7,
  .chan_num     = PWM_ARRAY_SIZE(7),
  .channels     = g_pwm7channels,
  .base         = FC7300_FTU7_BASE,
  .mode         = (enum ftu_pwmpinmodetype_e)CONFIG_FC7300_FTU7_PWM_MODE,
  .deadtimefreq = CONFIG_FC7300_FTU7_PWM_DEADTIMEFREQ,
};
#endif

#ifdef CONFIG_FC7300_FTU8_PWM

static struct fc7300_pwmchan_s g_pwm8channels[] =
{
  /* FTU8 has 8 channels */

#ifdef CONFIG_FC7300_FTU8_PWM_CHANNEL0
  {
    .channel = 0,
#ifdef CONFIG_FC7300_FTU8_PWM_CH0_DEADTIMEEN
    .deadtimeen = true,
#endif
    .pincfg = GPIO_FTU8_CH0OUT,
  },
#endif
#ifdef CONFIG_FC7300_FTU8_PWM_CHANNEL1
  {
    .channel = 1,
#ifdef CONFIG_FC7300_FTU8_PWM_CH1_DEADTIMEEN
    .deadtimeen = true,
#endif
    .pincfg = GPIO_FTU8_CH1OUT,
  },
#endif
#ifdef CONFIG_FC7300_FTU8_PWM_CHANNEL2
  {
    .channel = 2,
#ifdef CONFIG_FC7300_FTU8_PWM_CH2_DEADTIMEEN
    .deadtimeen = true,
#endif
    .pincfg = GPIO_FTU8_CH2OUT,
  },
#endif
#ifdef CONFIG_FC7300_FTU8_PWM_CHANNEL3
  {
    .channel = 3,
#ifdef CONFIG_FC7300_FTU8_PWM_CH3_DEADTIMEEN
    .deadtimeen = true,
#endif
    .pincfg = GPIO_FTU8_CH3OUT,
  },
#endif
#ifdef CONFIG_FC7300_FTU8_PWM_CHANNEL4
  {
    .channel = 4,
#ifdef CONFIG_FC7300_FTU8_PWM_CH4_DEADTIMEEN
    .deadtimeen = true,
#endif
    .pincfg = GPIO_FTU8_CH4OUT,
  },
#endif
#ifdef CONFIG_FC7300_FTU8_PWM_CHANNEL5
  {
    .channel = 5,
#ifdef CONFIG_FC7300_FTU8_PWM_CH5_DEADTIMEEN
    .deadtimeen = true,
#endif
    .pincfg = GPIO_FTU8_CH5OUT,
  },
#endif
#ifdef CONFIG_FC7300_FTU8_PWM_CHANNEL6
  {
    .channel = 6,
#ifdef CONFIG_FC7300_FTU8_PWM_CH6_DEADTIMEEN
    .deadtimeen = true,
#endif
    .pincfg = GPIO_FTU8_CH6OUT,
  },
#endif
#ifdef CONFIG_FC7300_FTU8_PWM_CHANNEL7
  {
    .channel = 7,
#ifdef CONFIG_FC7300_FTU8_PWM_CH7_DEADTIMEEN
    .deadtimeen = true,
#endif
    .pincfg = GPIO_FTU8_CH7OUT,
  },
#endif
};

static struct fc7300_pwmtimer_s g_pwm8dev =
{
  .ops          = &g_pwmops,
  .tpmid        = 8,
  .chan_num     = PWM_ARRAY_SIZE(8),
  .channels     = g_pwm8channels,
  .base         = FC7300_FTU8_BASE,
  .mode         = (enum ftu_pwmpinmodetype_e)CONFIG_FC7300_FTU8_PWM_MODE,
  .deadtimefreq = CONFIG_FC7300_FTU8_PWM_DEADTIMEFREQ,
};
#endif

#ifdef CONFIG_FC7300_FTU9_PWM

static struct fc7300_pwmchan_s g_pwm9channels[] =
{
  /* FTU9 has 8 channels */

#ifdef CONFIG_FC7300_FTU9_PWM_CHANNEL0
  {
    .channel = 0,
#ifdef CONFIG_FC7300_FTU9_PWM_CH0_DEADTIMEEN
    .deadtimeen = true,
#endif
    .pincfg = GPIO_FTU9_CH0OUT,
  },
#endif
#ifdef CONFIG_FC7300_FTU9_PWM_CHANNEL1
  {
    .channel = 1,
#ifdef CONFIG_FC7300_FTU9_PWM_CH1_DEADTIMEEN
    .deadtimeen = true,
#endif
    .pincfg = GPIO_FTU9_CH1OUT,
  },
#endif
#ifdef CONFIG_FC7300_FTU9_PWM_CHANNEL2
  {
    .channel = 2,
#ifdef CONFIG_FC7300_FTU9_PWM_CH2_DEADTIMEEN
    .deadtimeen = true,
#endif
    .pincfg = GPIO_FTU9_CH2OUT,
  },
#endif
#ifdef CONFIG_FC7300_FTU9_PWM_CHANNEL3
  {
    .channel = 3,
#ifdef CONFIG_FC7300_FTU9_PWM_CH3_DEADTIMEEN
    .deadtimeen = true,
#endif
    .pincfg = GPIO_FTU9_CH3OUT,
  },
#endif
#ifdef CONFIG_FC7300_FTU9_PWM_CHANNEL4
  {
    .channel = 4,
#ifdef CONFIG_FC7300_FTU9_PWM_CH4_DEADTIMEEN
    .deadtimeen = true,
#endif
    .pincfg = GPIO_FTU9_CH4OUT,
  },
#endif
#ifdef CONFIG_FC7300_FTU9_PWM_CHANNEL5
  {
    .channel = 5,
#ifdef CONFIG_FC7300_FTU9_PWM_CH5_DEADTIMEEN
    .deadtimeen = true,
#endif
    .pincfg = GPIO_FTU9_CH5OUT,
  },
#endif
#ifdef CONFIG_FC7300_FTU9_PWM_CHANNEL6
  {
    .channel = 6,
#ifdef CONFIG_FC7300_FTU9_PWM_CH6_DEADTIMEEN
    .deadtimeen = true,
#endif
    .pincfg = GPIO_FTU9_CH6OUT,
  },
#endif
#ifdef CONFIG_FC7300_FTU9_PWM_CHANNEL7
  {
    .channel = 7,
#ifdef CONFIG_FC7300_FTU9_PWM_CH7_DEADTIMEEN
    .deadtimeen = true,
#endif
    .pincfg = GPIO_FTU9_CH7OUT,
  },
#endif
};

static struct fc7300_pwmtimer_s g_pwm9dev =
{
  .ops          = &g_pwmops,
  .tpmid        = 9,
  .chan_num     = PWM_ARRAY_SIZE(9),
  .channels     = g_pwm9channels,
  .base         = FC7300_FTU9_BASE,
  .mode         = (enum ftu_pwmpinmodetype_e)CONFIG_FC7300_FTU9_PWM_MODE,
  .deadtimefreq = CONFIG_FC7300_FTU9_PWM_DEADTIMEFREQ,
};
#endif

#ifdef CONFIG_FC7300_FTU10_PWM

static struct fc7300_pwmchan_s g_pwm10channels[] =
{
  /* FTU10 has 8 channels */

#ifdef CONFIG_FC7300_FTU10_PWM_CHANNEL0
  {
    .channel = 0,
#ifdef CONFIG_FC7300_FTU10_PWM_CH0_DEADTIMEEN
    .deadtimeen = true,
#endif
    .pincfg = GPIO_FTU10_CH0OUT,
  },
#endif
#ifdef CONFIG_FC7300_FTU10_PWM_CHANNEL1
  {
    .channel = 1,
#ifdef CONFIG_FC7300_FTU10_PWM_CH1_DEADTIMEEN
    .deadtimeen = true,
#endif
    .pincfg = GPIO_FTU10_CH1OUT,
  },
#endif
#ifdef CONFIG_FC7300_FTU10_PWM_CHANNEL2
  {
    .channel = 2,
#ifdef CONFIG_FC7300_FTU10_PWM_CH2_DEADTIMEEN
    .deadtimeen = true,
#endif
    .pincfg = GPIO_FTU10_CH2OUT,
  },
#endif
#ifdef CONFIG_FC7300_FTU10_PWM_CHANNEL3
  {
    .channel = 3,
#ifdef CONFIG_FC7300_FTU10_PWM_CH3_DEADTIMEEN
    .deadtimeen = true,
#endif
    .pincfg = GPIO_FTU10_CH3OUT,
  },
#endif
#ifdef CONFIG_FC7300_FTU10_PWM_CHANNEL4
  {
    .channel = 4,
#ifdef CONFIG_FC7300_FTU10_PWM_CH4_DEADTIMEEN
    .deadtimeen = true,
#endif
    .pincfg = GPIO_FTU10_CH4OUT,
  },
#endif
#ifdef CONFIG_FC7300_FTU10_PWM_CHANNEL5
  {
    .channel = 5,
#ifdef CONFIG_FC7300_FTU10_PWM_CH5_DEADTIMEEN
    .deadtimeen = true,
#endif
    .pincfg = GPIO_FTU10_CH5OUT,
  },
#endif
#ifdef CONFIG_FC7300_FTU10_PWM_CHANNEL6
  {
    .channel = 6,
#ifdef CONFIG_FC7300_FTU10_PWM_CH6_DEADTIMEEN
    .deadtimeen = true,
#endif
    .pincfg = GPIO_FTU10_CH6OUT,
  },
#endif
#ifdef CONFIG_FC7300_FTU10_PWM_CHANNEL7
  {
    .channel = 7,
#ifdef CONFIG_FC7300_FTU10_PWM_CH7_DEADTIMEEN
    .deadtimeen = true,
#endif
    .pincfg = GPIO_FTU10_CH7OUT,
  },
#endif
};

static struct fc7300_pwmtimer_s g_pwm10dev =
{
  .ops          = &g_pwmops,
  .tpmid        = 10,
  .chan_num     = PWM_ARRAY_SIZE(10),
  .channels     = g_pwm10channels,
  .base         = FC7300_FTU10_BASE,
  .mode         = (enum ftu_pwmpinmodetype_e)CONFIG_FC7300_FTU10_PWM_MODE,
  .deadtimefreq = CONFIG_FC7300_FTU10_PWM_DEADTIMEFREQ,
};
#endif

#ifdef CONFIG_FC7300_FTU11_PWM

static struct fc7300_pwmchan_s g_pwm11channels[] =
{
  /* FTU11 has 8 channels */

#ifdef CONFIG_FC7300_FTU11_PWM_CHANNEL0
  {
    .channel = 0,
#ifdef CONFIG_FC7300_FTU11_PWM_CH0_DEADTIMEEN
    .deadtimeen = true,
#endif
    .pincfg = GPIO_FTU11_CH0OUT,
  },
#endif
#ifdef CONFIG_FC7300_FTU11_PWM_CHANNEL1
  {
    .channel = 1,
#ifdef CONFIG_FC7300_FTU11_PWM_CH1_DEADTIMEEN
    .deadtimeen = true,
#endif
    .pincfg = GPIO_FTU11_CH1OUT,
  },
#endif
#ifdef CONFIG_FC7300_FTU11_PWM_CHANNEL2
  {
    .channel = 2,
#ifdef CONFIG_FC7300_FTU11_PWM_CH2_DEADTIMEEN
    .deadtimeen = true,
#endif
    .pincfg = GPIO_FTU11_CH2OUT,
  },
#endif
#ifdef CONFIG_FC7300_FTU11_PWM_CHANNEL3
  {
    .channel = 3,
#ifdef CONFIG_FC7300_FTU11_PWM_CH3_DEADTIMEEN
    .deadtimeen = true,
#endif
    .pincfg = GPIO_FTU11_CH3OUT,
  },
#endif
#ifdef CONFIG_FC7300_FTU11_PWM_CHANNEL4
  {
    .channel = 4,
#ifdef CONFIG_FC7300_FTU11_PWM_CH4_DEADTIMEEN
    .deadtimeen = true,
#endif
    .pincfg = GPIO_FTU11_CH4OUT,
  },
#endif
#ifdef CONFIG_FC7300_FTU11_PWM_CHANNEL5
  {
    .channel = 5,
#ifdef CONFIG_FC7300_FTU11_PWM_CH5_DEADTIMEEN
    .deadtimeen = true,
#endif
    .pincfg = GPIO_FTU11_CH5OUT,
  },
#endif
#ifdef CONFIG_FC7300_FTU11_PWM_CHANNEL6
  {
    .channel = 6,
#ifdef CONFIG_FC7300_FTU11_PWM_CH6_DEADTIMEEN
    .deadtimeen = true,
#endif
    .pincfg = GPIO_FTU11_CH6OUT,
  },
#endif
#ifdef CONFIG_FC7300_FTU11_PWM_CHANNEL7
  {
    .channel = 7,
#ifdef CONFIG_FC7300_FTU11_PWM_CH7_DEADTIMEEN
    .deadtimeen = true,
#endif
    .pincfg = GPIO_FTU11_CH7OUT,
  },
#endif
};

static struct fc7300_pwmtimer_s g_pwm11dev =
{
  .ops          = &g_pwmops,
  .tpmid        = 11,
  .chan_num     = PWM_ARRAY_SIZE(11),
  .channels     = g_pwm11channels,
  .base         = FC7300_FTU11_BASE,
  .mode         = (enum ftu_pwmpinmodetype_e)CONFIG_FC7300_FTU11_PWM_MODE,
  .deadtimefreq = CONFIG_FC7300_FTU11_PWM_DEADTIMEFREQ,
};
#endif

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: fc7300_pwm_getreg
 *
 * Description:
 *   Read the value of an PWM timer register.
 *
 * Input Parameters:
 *   priv - A reference to the PWM block status
 *   offset - The offset to the register to read
 *
 * Returned Value:
 *   The current contents of the specified register
 *
 ****************************************************************************/

static uint32_t fc7300_pwm_getreg(struct fc7300_pwmtimer_s *priv, int offset)
{
  return getreg32(priv->base + offset);
}

/****************************************************************************
 * Name: fc7300_pwm_putreg
 *
 * Description:
 *   Set the value of an PWM timer register.
 *
 * Input Parameters:
 *   priv - A reference to the PWM block status
 *   offset - The offset to the register to set
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/

static void fc7300_pwm_putreg(struct fc7300_pwmtimer_s *priv, int offset,
                       uint32_t value)
{
  putreg32(value, priv->base + offset);
}

/****************************************************************************
 * Name: fc7300_pwm_dumpregs
 *
 * Description:
 *   Dump all timer registers.
 *
 * Input Parameters:
 *   priv - A reference to the PWM block status
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/

static void fc7300_pwm_dumpregs(struct fc7300_pwmtimer_s *priv,
                        const char *msg)
{
  pwminfo("%s:\n", msg);
  pwminfo("  FTU%d_SC:     %0lx   FTU%d_CNT:  %0lx     FTU%d_MOD:  %0lx\n",
          priv->tpmid, fc7300_pwm_getreg(priv, FC7300_FTU_SC_OFFSET),
          priv->tpmid, fc7300_pwm_getreg(priv, FC7300_FTU_CNT_OFFSET),
          priv->tpmid, fc7300_pwm_getreg(priv, FC7300_FTU_MOD_OFFSET));
  pwminfo("  FTU%d_STATUS: %0lx   FTU%d_CONF: %0lx\n",
          priv->tpmid, fc7300_pwm_getreg(priv, FC7300_FTU_STATUS_OFFSET),
          priv->tpmid, fc7300_pwm_getreg(priv, FC7300_FTU_CONF_OFFSET));
  pwminfo("   FTU%d_C0SC:  %0lx   FTU%d_C0V:  %0lx\n",
          priv->tpmid, fc7300_pwm_getreg(priv, FC7300_FTU_C0SC_OFFSET),
          priv->tpmid, fc7300_pwm_getreg(priv, FC7300_FTU_C0V_OFFSET));
  pwminfo("   FTU%d_C1SC:  %0lx   FTU%d_C1V:  %0lx\n",
          priv->tpmid, fc7300_pwm_getreg(priv, FC7300_FTU_C1SC_OFFSET),
          priv->tpmid, fc7300_pwm_getreg(priv, FC7300_FTU_C1V_OFFSET));
  pwminfo("   FTU%d_C2SC:  %0lx   FTU%d_C2V:  %0lx\n",
          priv->tpmid, fc7300_pwm_getreg(priv, FC7300_FTU_C2SC_OFFSET),
          priv->tpmid, fc7300_pwm_getreg(priv, FC7300_FTU_C2V_OFFSET));
  pwminfo("   FTU%d_C3SC:  %0lx   FTU%d_C3V:  %0lx\n",
          priv->tpmid, fc7300_pwm_getreg(priv, FC7300_FTU_C3SC_OFFSET),
          priv->tpmid, fc7300_pwm_getreg(priv, FC7300_FTU_C3V_OFFSET));
  pwminfo("   FTU%d_C4SC:  %0lx   FTU%d_C4V:  %0lx\n",
          priv->tpmid, fc7300_pwm_getreg(priv, FC7300_FTU_C4SC_OFFSET),
          priv->tpmid, fc7300_pwm_getreg(priv, FC7300_FTU_C4V_OFFSET));
  pwminfo("   FTU%d_C5SC:  %0lx   FTU%d_C5V:  %0lx\n",
          priv->tpmid, fc7300_pwm_getreg(priv, FC7300_FTU_C5SC_OFFSET),
          priv->tpmid, fc7300_pwm_getreg(priv, FC7300_FTU_C5V_OFFSET));
  pwminfo("   FTU%d_C6SC:  %0lx   FTU%d_C6V:  %0lx\n",
          priv->tpmid, fc7300_pwm_getreg(priv, FC7300_FTU_C6SC_OFFSET),
          priv->tpmid, fc7300_pwm_getreg(priv, FC7300_FTU_C6V_OFFSET));
  pwminfo("   FTU%d_C7SC:  %0lx   FTU%d_C7V:  %0lx\n",
          priv->tpmid, fc7300_pwm_getreg(priv, FC7300_FTU_C7SC_OFFSET),
          priv->tpmid, fc7300_pwm_getreg(priv, FC7300_FTU_C7V_OFFSET));
}

/****************************************************************************
 * Name: fc7300_pwm_deadtime_calculate
 *
 * Description:
 *   Calculate deadtime relevant parameters.
 *
 * Input Parameters:
 *   priv - A reference to the PWM block status
 *   deadtime - deadtime relevant parameters
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/

static void fc7300_pwm_deadtime_calculate(struct fc7300_pwmtimer_s *priv,
                                   struct ftu_deadtimeparamtype *deadtime)
{
  uint8_t prescale;
  uint8_t value = 1;

  deadtime->prescaler = 0;
  deadtime->deadtimeval = 0;

  for (prescale = 0U; prescale < 5; prescale += 2, value++)
    {
      if (priv->pclk / (1 << prescale) / priv->deadtimefreq < 1024)
        {
          deadtime->deadtimeval = ((priv->pclk) / (1 << prescale) / \
           (priv->deadtimefreq));
          deadtime->prescaler = prescale ? 0 : value;
          break;
        }
    }
}

/****************************************************************************
 * Name: fc7300_pwm_deadtime_set
 *
 * Description:
 *   set deadtime relevant registers.
 *
 * Input Parameters:
 *   priv - A reference to the PWM block status
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/

static void fc7300_pwm_deadtime_set(struct fc7300_pwmtimer_s *priv)
{
  struct ftu_deadtimeparamtype deadtimevalue;
  uint32_t regval;

  fc7300_pwm_deadtime_calculate(priv, &deadtimevalue);

  /* set pwm deadtime configuration */

  if (0U != priv->deadtimefreq)
    {
      regval = FTU_DEADTIME_DTPS(deadtimevalue.prescaler) |
               FTU_DEADTIME_DTVAL(deadtimevalue.deadtimeval & 0x3fu);
      if (deadtimevalue.deadtimeval > 63u)
        {
          regval |= FTU_DEADTIME_DTVALEX((deadtimevalue.deadtimeval & 0x3c0u)
                                         >> 6u);
        }

      fc7300_pwm_putreg(priv, FC7300_FTU_DEADTIME_OFFSET, regval);
    }
}

/****************************************************************************
 * Name: fc7300_pwm_timer
 *
 * Description:
 *   (Re-)initialize the timer resources and start the pulsed output
 *
 * Input Parameters:
 *   priv - A reference to the lower half PWM driver state structure
 *   info - A reference to the characteristics of the pulsed output
 *
 * Returned Value:
 *   Zero on success; a negated errno value on failure
 *
 ****************************************************************************/

static int fc7300_pwm_timer(struct fc7300_pwmtimer_s *priv,
                     const struct pwm_info_s *info)
{
  /* Calculated values */

  uint32_t prescaler;
  uint32_t tpmclk;
  uint32_t modulo;
  uint32_t regval;
  uint32_t cv;
  uint32_t csc = 0;
  uint32_t duty = 0;
  #ifdef CONFIG_PWM_MULTICHAN
  uint8_t  j;
  #endif
  uint8_t  i;
  uint8_t  channel;

  static const uint8_t presc_values[8] =
  {
    1, 2, 4, 8, 16, 32, 64, 128
  };

  DEBUGASSERT(priv != NULL && info != NULL);
  DEBUGASSERT(priv->chan_num > 0);

  /* Calculate MOD register value.
   * Calculate  multiples of the maximum of MOD(65535).
   * And round up to an integer.
   */

  prescaler = (priv->pclk / info->frequency + 65534) / 65535;
  for (i = 0; i < 8; i++)
    {
      if (prescaler <= presc_values[i])
        {
           break;
        }
    }

  if (i == 8)
    {
      i = 7;
    }

  prescaler = i;

  tpmclk = priv->pclk / info->frequency;

  if (priv->mode > FTU_EDGE_PWM_LOW_TRUE_PULSE)
    {
      tpmclk >>= 1U;
    }

  modulo = tpmclk / presc_values[prescaler];

  if (modulo < 1)
    {
      modulo = 1;
    }
  else if (modulo > 65535)
    {
      modulo = 65535;
    }

  /* Set the modulo value */

  fc7300_pwm_putreg(priv, FC7300_FTU_MOD_OFFSET, (uint16_t)modulo);
  fc7300_pwm_deadtime_set(priv);

  /* Select the pwm mode for specific */

  switch (priv->mode)
    {
      case FTU_EDGE_PWM_HIGH_TRUE_PULSE:
        {
          csc = FTU_CSC_MSB_MASK | FTU_CSC_ELSB_MASK;
        }
        break;

      case FTU_EDGE_PWM_LOW_TRUE_PULSE:
        {
          csc = FTU_CSC_MSB_MASK | FTU_CSC_ELSA_MASK;
        }
        break;

      case FTU_CENTER_PWM_HIGH_TRUE_PULSE:
        {
          regval = fc7300_pwm_getreg(priv, FC7300_FTU_SC_OFFSET);
          regval |= FTU_SC_CPWMS_MASK;
          fc7300_pwm_putreg(priv, FC7300_FTU_SC_OFFSET, regval);
          csc = FTU_CSC_ELSB_MASK;
        }
        break;

      case FTU_CENTER_PWM_LOW_TRUE_PULSE:
        {
          regval = fc7300_pwm_getreg(priv, FC7300_FTU_SC_OFFSET);
          regval |= FTU_SC_CPWMS_MASK;
          fc7300_pwm_putreg(priv, FC7300_FTU_SC_OFFSET, regval);
          csc = FTU_CSC_ELSA_MASK;
        }
        break;

      default:
        pwmerr("ERROR: No such mode: %d\n", priv->mode);
        return -EINVAL;
    }

  for (i = 0; i < priv->chan_num; i += 1)
    {
#ifndef CONFIG_PWM_MULTICHAN
  DEBUGASSERT(info->frequency > 0 && info->duty >= 0 &&
              info->duty < uitoub16(100));
#endif

      /* Set the duty cycle for channel specific */

      /* Duty cycle: duty cycle = cv / modulo (fractional value) */

#ifdef CONFIG_PWM_MULTICHAN

      /* Break the loop if all following channels are not configured */

      if (info->channels[i].channel == -1)
        {
          break;
        }

      duty    = info->channels[i].duty;
      channel = info->channels[i].channel;

      /* A value of zero means to skip this channel */

      if (channel == 0)
        {
          continue;
        }

      /* Find the channel */

      for (j = 0; j < priv->chan_num; j++)
        {
          if (priv->channels[j].channel == (channel - 1))
            {
              break;
            }
        }

      /* Check range */

      if (j >= priv->chan_num)
        {
          pwmerr("ERROR: No such channel: %u\n", channel-1);
          return -EINVAL;
        }

      channel = channel -1;

#else
      channel = priv->channels[0].channel;
      duty = info->duty;
#endif
      /* Solve for loss of accuracy in edge mode */

      if ((priv->mode ==  FTU_EDGE_PWM_HIGH_TRUE_PULSE) ||
           (priv->mode ==  FTU_EDGE_PWM_LOW_TRUE_PULSE))
        {
          if (duty == 0)
            {
              cv = 0;
            }
          else
            {
              cv = b16toi(duty * modulo) + 2;
            }  
        }
      else
        {
          if (duty == 0)
            {
              cv = 0;
            }
          else
            {
              cv = b16toi(duty * modulo) + 1;
            }
        }

      switch (channel)
        {
          case 0:  /* PWM Mode configuration: Channel 0 */
            {
              fc7300_pwm_putreg(priv, FC7300_FTU_C0SC_OFFSET, csc);
              fc7300_pwm_putreg(priv, FC7300_FTU_C0V_OFFSET, cv);
            }
            break;

          case 1:  /* PWM Mode configuration: Channel 1 */
            {
              fc7300_pwm_putreg(priv, FC7300_FTU_C1SC_OFFSET, csc);
              fc7300_pwm_putreg(priv, FC7300_FTU_C1V_OFFSET, cv);
            }
            break;

          case 2:  /* PWM Mode configuration: Channel 2 */
            {
              fc7300_pwm_putreg(priv, FC7300_FTU_C2SC_OFFSET, csc);
              fc7300_pwm_putreg(priv, FC7300_FTU_C2V_OFFSET, cv);
            }
            break;

          case 3:  /* PWM Mode configuration: Channel 3 */
            {
              fc7300_pwm_putreg(priv, FC7300_FTU_C3SC_OFFSET, csc);
              fc7300_pwm_putreg(priv, FC7300_FTU_C3V_OFFSET, cv);
            }
            break;

          case 4:  /* PWM Mode configuration: Channel 4 */
            {
              fc7300_pwm_putreg(priv, FC7300_FTU_C4SC_OFFSET, csc);
              fc7300_pwm_putreg(priv, FC7300_FTU_C4V_OFFSET, cv);
            }
            break;

          case 5:  /* PWM Mode configuration: Channel 5 */
            {
              fc7300_pwm_putreg(priv, FC7300_FTU_C5SC_OFFSET, csc);
              fc7300_pwm_putreg(priv, FC7300_FTU_C5V_OFFSET, cv);
            }
          break;

          case 6:  /* PWM Mode configuration: Channel 6 */
            {
              fc7300_pwm_putreg(priv, FC7300_FTU_C6SC_OFFSET, csc);
              fc7300_pwm_putreg(priv, FC7300_FTU_C6V_OFFSET, cv);
            }
            break;

          case 7:  /* PWM Mode configuration: Channel 7 */
            {
              fc7300_pwm_putreg(priv, FC7300_FTU_C7SC_OFFSET, csc);
              fc7300_pwm_putreg(priv, FC7300_FTU_C7V_OFFSET, cv);
            }
            break;

          default:
              pwmerr("ERROR: No such channel: %d\n", channel);
              return -EINVAL;
        }

      if (priv->channels[i].deadtimeen)
        {
          if (0 == channel % 2)
            {
              /* Set deadtime enable */

              regval = fc7300_pwm_getreg(priv, FC7300_FTU_CHCTRL_OFFSET);
              regval |= FTU_CHCTRL_DEADTIME_CHANNEL(channel);
              fc7300_pwm_putreg(priv, FC7300_FTU_CHCTRL_OFFSET, regval);
            }
          else
            {
              /* Set complement mode enable */

              regval = fc7300_pwm_getreg(priv, FC7300_FTU_CHCTRL_OFFSET);
              regval |= FTU_CHCTRL_COMPLEMENT_CHANNEL(channel);
              fc7300_pwm_putreg(priv, FC7300_FTU_CHCTRL_OFFSET, regval);
            }
        }

      /* enable channel sync function */

      regval = fc7300_pwm_getreg(priv, FC7300_FTU_CHCTRL_OFFSET);
      regval |= FTU_CHCTRL_SYNCEN_CHANNEL(channel);
      fc7300_pwm_putreg(priv, FC7300_FTU_CHCTRL_OFFSET, regval);

      /* Set prescaler and enable channel output */

      regval = fc7300_pwm_getreg(priv, FC7300_FTU_SC_OFFSET);
      regval &= ~(FTU_SC_PS_MASK);
      regval |= prescaler;
      regval |= FTU_SC_CHNOUTEN(1 << channel);
      fc7300_pwm_putreg(priv, FC7300_FTU_SC_OFFSET, regval);
    }

  /* Enable the loading of the MOD, CNTIN, and CV registers with the values of their buffers */

  regval = fc7300_pwm_getreg(priv, FC7300_FTU_PWMLOAD_OFFSET);
  regval |= FTU_PWMLOAD_LDOK_MASK;
  fc7300_pwm_putreg(priv, FC7300_FTU_PWMLOAD_OFFSET, regval);

  fc7300_pwm_dumpregs(priv, "After starting");
  return OK;
}

/****************************************************************************
 * Name: fc7300_pwm_setup
 *
 * Description:
 *   This method is called when the driver is opened.  The lower half driver
 *   should configure and initialize the device so that it is ready for use.
 *   It should not, however, output pulses until the start method is called.
 *
 * Input Parameters:
 *   dev - A reference to the lower half PWM driver state structure
 *
 * Returned Value:
 *   Zero on success; a negated errno value on failure
 *
 * Assumptions:
 *   AHB1 or 2 clocking for the GPIOs and timer has already been configured
 *   by the RCC logic at power up.
 *
 ****************************************************************************/

static int fc7300_pwm_setup(struct pwm_lowerhalf_s *dev)
{
  struct fc7300_pwmtimer_s *priv = (struct fc7300_pwmtimer_s *)dev;
  uint32_t pincfg;
  uint8_t i;

  /* Note: The appropriate clock should for the right FTU device should
   * already be enabled in the board-specific fc7300_periphclocks.c file.
   */

  DEBUGASSERT(priv->chan_num > 0);
  fc7300_pwm_dumpregs(priv, "Initially");

  for (i = 0; i < priv->chan_num; i += 1)
    {
      pincfg = priv->channels[i].pincfg;

      /* Configure the PWM output pin, but do not start the timer yet */

      fc7300_pinconfig(pincfg);
    }

  return OK;
}

/****************************************************************************
 * Name: fc7300_pwm_shutdown
 *
 * Description:
 *   This method is called when the driver is closed.  The lower half driver
 *   stop pulsed output, free any resources, disable the timer hardware, and
 *   put the system into the lowest possible power usage state
 *
 * Input Parameters:
 *   dev - A reference to the lower half PWM driver state structure
 *
 * Returned Value:
 *   Zero on success; a negated errno value on failure
 *
 ****************************************************************************/

static int fc7300_pwm_shutdown(struct pwm_lowerhalf_s *dev)
{
  struct fc7300_pwmtimer_s *priv = (struct fc7300_pwmtimer_s *)dev;
  uint32_t pincfg;
  uint8_t i;

  /* Make sure that the output has been stopped */

  DEBUGASSERT(priv->chan_num > 0);
  fc7300_pwm_stop(dev);

  for (i = 0; i < priv->chan_num; i += 1)
    {
      /* Then put the GPIO pin back to the default state */

      pincfg = (priv->channels[i].pincfg & ~(_PIN_MODE_MASK));
      pincfg |= GPIO_INPUT;
      fc7300_pinconfig(pincfg);
    }

  return OK;
}

/****************************************************************************
 * Name: fc7300_pwm_start
 *
 * Description:
 *   (Re-)initialize the timer resources and start the pulsed output
 *
 * Input Parameters:
 *   dev - A reference to the lower half PWM driver state structure
 *   info - A reference to the characteristics of the pulsed output
 *
 * Returned Value:
 *   Zero on success; a negated errno value on failure
 *
 ****************************************************************************/

static int fc7300_pwm_start(struct pwm_lowerhalf_s *dev,
                     const struct pwm_info_s *info)
{
  struct fc7300_pwmtimer_s *priv = (struct fc7300_pwmtimer_s *)dev;
  int ret;

  ret = fc7300_pwm_timer(priv, info);

  if (ret != OK)
    {
      return ret;
    }

  return ret;
}

/****************************************************************************
 * Name: fc7300_pwm_stop
 *
 * Description:
 *   Stop the pulsed output and reset the timer resources
 *
 * Input Parameters:
 *   dev - A reference to the lower half PWM driver state structure
 *
 * Returned Value:
 *   Zero on success; a negated errno value on failure
 *
 * Assumptions:
 *   This function is called to stop the pulsed output at anytime.  This
 *   method is also called from the timer interrupt handler when a repetition
 *   count expires... automatically stopping the timer.
 *
 ****************************************************************************/

static int fc7300_pwm_stop(struct pwm_lowerhalf_s *dev)
{
  struct fc7300_pwmtimer_s *priv = (struct fc7300_pwmtimer_s *)dev;
  irqstate_t flags;
  uint8_t channel;
  uint8_t i;
  uint32_t regval;

  /* Disable interrupts momentary to stop any ongoing timer processing and
   * to prevent any concurrent access to the reset register.
   */

  DEBUGASSERT(priv->chan_num > 0);
  flags = enter_critical_section();

  /* Determine which timer channel to clear */

  for (i = 0; i < priv->chan_num; i += 1)
    {
      channel = priv->channels[i].channel;
      switch (channel)
        {
          case 0:
            fc7300_pwm_putreg(priv, FC7300_FTU_C0V_OFFSET, 0);
            break;

          case 1:
            fc7300_pwm_putreg(priv, FC7300_FTU_C1V_OFFSET, 0);
            break;

          case 2:
            fc7300_pwm_putreg(priv, FC7300_FTU_C2V_OFFSET, 0);
            break;

          case 3:
            fc7300_pwm_putreg(priv, FC7300_FTU_C3V_OFFSET, 0);
            break;

          case 4:
            fc7300_pwm_putreg(priv, FC7300_FTU_C4V_OFFSET, 0);
            break;

          case 5:
            fc7300_pwm_putreg(priv, FC7300_FTU_C5V_OFFSET, 0);
            break;

          case 6:
            fc7300_pwm_putreg(priv, FC7300_FTU_C6V_OFFSET, 0);
            break;

          case 7:
            fc7300_pwm_putreg(priv, FC7300_FTU_C7V_OFFSET, 0);
            break;

          default:
            pwmerr("ERROR: No such channel: %d\n", channel);
            leave_critical_section(flags);
            return -EINVAL;
        }
    }

  /* Enable the loading of the MOD, CNTIN, and CV registers with the values of their buffers */

  regval = fc7300_pwm_getreg(priv, FC7300_FTU_PWMLOAD_OFFSET);
  regval |= FTU_PWMLOAD_LDOK_MASK;
  fc7300_pwm_putreg(priv, FC7300_FTU_PWMLOAD_OFFSET, regval);

  leave_critical_section(flags);

  fc7300_pwm_dumpregs(priv, "After stop");
  return OK;
}

/****************************************************************************
 * Name: fc7300_pwm_ioctl
 *
 * Description:
 *   Lower-half logic may support platform-specific ioctl commands
 *
 * Input Parameters:
 *   dev - A reference to the lower half PWM driver state structure
 *   cmd - The ioctl command
 *   arg - The argument accompanying the ioctl command
 *
 * Returned Value:
 *   Zero on success; a negated errno value on failure
 *
 ****************************************************************************/

static int fc7300_pwm_ioctl(struct pwm_lowerhalf_s *dev, int cmd,
                     unsigned long arg)
{
#ifdef CONFIG_DEBUG_PWM_INFO
  struct fc7300_pwmtimer_s *priv = (struct fc7300_pwmtimer_s *)dev;

  /* There are no platform-specific ioctl commands */

  pwminfo("FTM%d\n", priv->tpmid);
#endif

  return -ENOTTY;
}

/****************************************************************************
 * Name: fc7300pwm_common_int
 *
 * Description:
 *   initialization of common Lower-half logic of specific hardware instance
 *
 * Input Parameters:
 *   timer - A number identifying the timer use.
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/
void fc7300pwm_common_int(uint32_t base_address)
{
  uint32_t regval;

  /* Disable FTU and reset CNT before writing MOD and PS */

  putreg32(FTU_SC_CLKS(0), base_address + FC7300_FTU_SC_OFFSET);
  putreg32(0, base_address + FC7300_FTU_CNTIN_OFFSET);
  putreg32(0, base_address + FC7300_FTU_CNT_OFFSET);

  /* Minimum loading point enable*/

  putreg32(FTU_SYNC_CNTMIN(1), base_address + FC7300_FTU_SYNC_OFFSET);

  /* Set mode to oxffff*/

  putreg32((uint16_t)0xffff,base_address + FC7300_FTU_MOD_OFFSET);

  /* Set FTU module clock source */

  regval = getreg32(base_address + FC7300_FTU_SC_OFFSET);
  regval |= FTU_SC_CLKS(FTU_SC_CLKS_FTUINPUT);
  putreg32(regval, base_address + FC7300_FTU_SC_OFFSET);

  /* Set FTUEN = 1 */

  regval = getreg32(base_address + FC7300_FTU_MODE_OFFSET);
  regval |= FTU_MODE_FTUEN_MASK;
  putreg32(regval,base_address + FC7300_FTU_MODE_OFFSET);
}
/****************************************************************************
 * Public Functions
 ****************************************************************************/

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

struct pwm_lowerhalf_s *fc7300_pwminitialize(int timer)
{
  struct fc7300_pwmtimer_s *lower;
  int ret;
  uint32_t sysclk;
  sysclk = fc7300_get_busclk();
  ret = (sysclk == 0) ? -ENODEV : OK;
  switch (timer)
    {
#ifdef CONFIG_FC7300_FTU0_PWM
      case 0:
        lower = &g_pwm0dev;
        fc7300pwm_common_int(g_pwm0dev.base);
        break;
#endif
#ifdef CONFIG_FC7300_FTU1_PWM
      case 1:
        lower = &g_pwm1dev;
        fc7300pwm_common_int(g_pwm1dev.base);
        break;
#endif
#ifdef CONFIG_FC7300_FTU2_PWM
      case 2:
        lower = &g_pwm2dev;
        fc7300pwm_common_int(g_pwm2dev.base);
        break;
#endif
#ifdef CONFIG_FC7300_FTU3_PWM
      case 3:
        lower = &g_pwm3dev;
        fc7300pwm_common_int(g_pwm3dev.base);
        break;
#endif
#ifdef CONFIG_FC7300_FTU4_PWM
      case 4:
        lower = &g_pwm4dev;
        fc7300pwm_common_int(g_pwm4dev.base);
        break;
#endif
#ifdef CONFIG_FC7300_FTU5_PWM
      case 5:
        lower = &g_pwm5dev;
        fc7300pwm_common_int(g_pwm5dev.base);
        break;
#endif
#ifdef CONFIG_FC7300_FTU6_PWM
      case 6:
        lower = &g_pwm6dev;
        fc7300pwm_common_int(g_pwm6dev.base);
        break;
#endif
#ifdef CONFIG_FC7300_FTU7_PWM
      case 7:
        lower = &g_pwm7dev;
        fc7300pwm_common_int(g_pwm7dev.base);
        break;
#endif
#ifdef CONFIG_FC7300_FTU8_PWM
      case 8:
        lower = &g_pwm8dev;
        fc7300pwm_common_int(g_pwm8dev.base);
        break;
#endif
#ifdef CONFIG_FC7300_FTU9_PWM
      case 9:
        lower = &g_pwm9dev;
        fc7300pwm_common_int(g_pwm9dev.base);
        break;
#endif
#ifdef CONFIG_FC7300_FTU10_PWM
      case 10:
        lower = &g_pwm10dev;
        fc7300pwm_common_int(g_pwm10dev.base);
        break;
#endif
#ifdef CONFIG_FC7300_FTU11_PWM
      case 11:
        lower = &g_pwm11dev;
        fc7300pwm_common_int(g_pwm11dev.base);
        break;
#endif
      default:
        pwmerr("ERROR: No such timer configured\n");
        return NULL;
    }

    if (ret != OK)
      {
        pwmerr("ERROR: FTU%d peripheral clock not available\n", timer);
        return NULL;
      }

  lower->pclk = sysclk;
  return (struct pwm_lowerhalf_s *)lower;
}

#endif /* CONFIG_FC7300_FTUn_PWM, n = 0,...,7 */
