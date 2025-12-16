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

#include <sys/types.h>

#include <stdint.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <nuttx/irq.h>
#include <nuttx/spinlock.h>
#include <nuttx/timers/capture.h>
#include "arm_internal.h"

#include <arch/board/board.h>

#include "fc7300_clockconfig.h"
#include "fc7300_periphclocks.h"
#include "fc7300_capture.h"
#include "fc7300_irq.h"

#if defined(CONFIG_FC7300_CAP)

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Private Types
 ****************************************************************************/

struct fc7300_capturemap_s
{
  uint8_t channelid;
  struct fc7300_cap_lowerhalf_s lower;
};

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

static int fc7300_cap_handler(int irq, void * context, void * arg);

/* "Lower half" driver methods */

static int cap_start(struct cap_lowerhalf_s *lower);
static int cap_stop(struct cap_lowerhalf_s *lower);
static int cap_getduty(struct cap_lowerhalf_s *lower, uint8_t *duty);
static int cap_getfreq(struct cap_lowerhalf_s *lower, uint32_t *freq);
static int cap_getedges(struct cap_lowerhalf_s *lower, uint32_t *edges);

static uint32_t fc7300_cap_pckfreq(uint8_t hwunit);
void fc7300_cap_clearfreqandduty(struct fc7300_cap_lowerhalf_s *lower);

/****************************************************************************
 * Private Data
 ****************************************************************************/

/* "Lower half" driver methods */

static const struct cap_ops_s g_cap_ops =
{
  .start       = cap_start,
  .stop        = cap_stop,
  .getduty     = cap_getduty,
  .getfreq     = cap_getfreq,
  .getedges    = cap_getedges
};

#define DECLARE_CAP_ISM_CHANNEL(X) \
{                                                     \
  .ops         = &g_cap_ops,                          \
  .channel     = X,                                   \
  .pincfg      = GPIO_ISM_CH##X##IN,                  \
  .mode        = CONFIG_FC7300_ISM_CAP_CH##X##_MODE,  \
  .clock       = 150000000,                           \
  .hwunit      = 12,                                  \
  .trgsel      = TRGSEL3_SRC_CH##X##IN,               \
  .clear       = fc7300_cap_clearfreqandduty,         \
}

#define DECLARE_CAP_FTU_CHANNEL(X) \
{                                                     \
  .ops         = &g_cap_ops,                          \
  .channel     = (X + 16),                            \
  .pincfg      = GPIO_FTU_CH##X##IN,                  \
  .mode        = CONFIG_FC7300_FTU_CAP_CH##X##_MODE,  \
  .clock       = 0,                                   \
  .hwunit      = (X  / 8),                            \
  .clear       = fc7300_cap_clearfreqandduty,         \
}

static struct fc7300_capturemap_s g_cap_capturemap[] =
{
#ifdef CONFIG_FC7300_ISM0_CAP_CHANNEL0
  {
    .channelid   = 0,
    .lower       = DECLARE_CAP_ISM_CHANNEL(0),
  },
#endif
#ifdef CONFIG_FC7300_ISM0_CAP_CHANNEL1
  {
    .channelid   = 1,
    .lower       = DECLARE_CAP_ISM_CHANNEL(1),
  },
#endif
#ifdef CONFIG_FC7300_ISM0_CAP_CHANNEL2
  {
    .channelid   = 2,
    .lower       = DECLARE_CAP_ISM_CHANNEL(2),
  },
#endif
#ifdef CONFIG_FC7300_ISM0_CAP_CHANNEL3
  {
    .channelid   = 3,
    .lower       = DECLARE_CAP_ISM_CHANNEL(3),
  },
#endif
#ifdef CONFIG_FC7300_ISM0_CAP_CHANNEL4
  {
    .channelid   = 4,
    .lower       = DECLARE_CAP_ISM_CHANNEL(4),
  },
#endif
#ifdef CONFIG_FC7300_ISM0_CAP_CHANNEL5
  {
    .channelid   = 5,
    .lower       = DECLARE_CAP_ISM_CHANNEL(5),
  },
#endif
#ifdef CONFIG_FC7300_ISM0_CAP_CHANNEL6
  {
    .channelid   = 6,
    .lower       = DECLARE_CAP_ISM_CHANNEL(6),
  },
#endif
#ifdef CONFIG_FC7300_ISM0_CAP_CHANNEL7
  {
    .channelid   = 7,
    .lower       = DECLARE_CAP_ISM_CHANNEL(7),
  },
#endif
#ifdef CONFIG_FC7300_ISM0_CAP_CHANNEL8
  {
    .channelid   = 8,
    .lower       = DECLARE_CAP_ISM_CHANNEL(8),
  },
#endif
#ifdef CONFIG_FC7300_ISM0_CAP_CHANNEL9
  {
    .channelid   = 9,
    .lower       = DECLARE_CAP_ISM_CHANNEL(9),
  },
#endif
#ifdef CONFIG_FC7300_ISM0_CAP_CHANNEL10
  {
    .channelid   = 10,
    .lower       = DECLARE_CAP_ISM_CHANNEL(10),
  },
#endif
#ifdef CONFIG_FC7300_ISM0_CAP_CHANNEL11
  {
    .channelid   = 11,
    .lower       = DECLARE_CAP_ISM_CHANNEL(11),
  },
#endif
#ifdef CONFIG_FC7300_ISM0_CAP_CHANNEL12
  {
    .channelid   = 12,
    .lower       = DECLARE_CAP_ISM_CHANNEL(12),
  },
#endif
#ifdef CONFIG_FC7300_ISM0_CAP_CHANNEL13
  {
    .channelid   = 13,
    .lower       = DECLARE_CAP_ISM_CHANNEL(13),
  },
#endif
#ifdef CONFIG_FC7300_ISM0_CAP_CHANNEL14
  {
    .channelid   = 14,
    .lower       = DECLARE_CAP_ISM_CHANNEL(14),
  },
#endif
#ifdef CONFIG_FC7300_ISM0_CAP_CHANNEL15
  {
    .channelid   = 15,
    .lower       = DECLARE_CAP_ISM_CHANNEL(15),
  },
#endif
#ifdef CONFIG_FC7300_FTU0_CAP_CHANNEL0
  {
    .channelid   = 16,
    .lower       = DECLARE_CAP_FTU_CHANNEL(0),
  },
#endif
#ifdef CONFIG_FC7300_FTU0_CAP_CHANNEL1
  {
    .channelid   = 17,
    .lower       = DECLARE_CAP_FTU_CHANNEL(1),
  },
#endif
#ifdef CONFIG_FC7300_FTU0_CAP_CHANNEL2
  {
    .channelid   = 18,
    .lower       = DECLARE_CAP_FTU_CHANNEL(2),
  },
#endif
#ifdef CONFIG_FC7300_FTU0_CAP_CHANNEL3
  {
    .channelid   = 19,
    .lower       = DECLARE_CAP_FTU_CHANNEL(3),
  },
#endif
#ifdef CONFIG_FC7300_FTU0_CAP_CHANNEL4
  {
    .channelid   = 20,
    .lower       = DECLARE_CAP_FTU_CHANNEL(4),
  },
#endif
#ifdef CONFIG_FC7300_FTU0_CAP_CHANNEL5
  {
    .channelid   = 21,
    .lower       = DECLARE_CAP_FTU_CHANNEL(5),
  },
#endif
#ifdef CONFIG_FC7300_FTU0_CAP_CHANNEL6
  {
    .channelid   = 22,
    .lower       = DECLARE_CAP_FTU_CHANNEL(6),
  },
#endif
#ifdef CONFIG_FC7300_FTU0_CAP_CHANNEL7
  {
    .channelid   = 23,
    .lower       = DECLARE_CAP_FTU_CHANNEL(7),
  },
#endif
#ifdef CONFIG_FC7300_FTU1_CAP_CHANNEL0
  {
    .channelid   = 24,
    .lower       = DECLARE_CAP_FTU_CHANNEL(8),
  },
#endif
#ifdef CONFIG_FC7300_FTU1_CAP_CHANNEL1
  {
    .channelid   = 25,
    .lower       = DECLARE_CAP_FTU_CHANNEL(9),
  },
#endif
#ifdef CONFIG_FC7300_FTU1_CAP_CHANNEL2
  {
    .channelid   = 26,
    .lower       = DECLARE_CAP_FTU_CHANNEL(10),
  },
#endif
#ifdef CONFIG_FC7300_FTU1_CAP_CHANNEL3
  {
    .channelid   = 27,
    .lower       = DECLARE_CAP_FTU_CHANNEL(11),
  },
#endif
#ifdef CONFIG_FC7300_FTU1_CAP_CHANNEL4
  {
    .channelid   = 28,
    .lower       = DECLARE_CAP_FTU_CHANNEL(12),
  },
#endif
#ifdef CONFIG_FC7300_FTU1_CAP_CHANNEL5
  {
    .channelid   = 29,
    .lower       = DECLARE_CAP_FTU_CHANNEL(13),
  },
#endif
#ifdef CONFIG_FC7300_FTU1_CAP_CHANNEL6
  {
    .channelid   = 30,
    .lower       = DECLARE_CAP_FTU_CHANNEL(14),
  },
#endif
#ifdef CONFIG_FC7300_FTU1_CAP_CHANNEL7
  {
    .channelid   = 31,
    .lower       = DECLARE_CAP_FTU_CHANNEL(15),
  },
#endif
#ifdef CONFIG_FC7300_FTU2_CAP_CHANNEL0
  {
    .channelid   = 32,
    .lower       = DECLARE_CAP_FTU_CHANNEL(16),
  },
#endif
#ifdef CONFIG_FC7300_FTU2_CAP_CHANNEL1
  {
    .channelid   = 33,
    .lower       = DECLARE_CAP_FTU_CHANNEL(17),
  },
#endif
#ifdef CONFIG_FC7300_FTU2_CAP_CHANNEL2
  {
    .channelid   = 34,
    .lower       = DECLARE_CAP_FTU_CHANNEL(18),
  },
#endif
#ifdef CONFIG_FC7300_FTU2_CAP_CHANNEL3
  {
    .channelid   = 35,
    .lower       = DECLARE_CAP_FTU_CHANNEL(19),
  },
#endif
#ifdef CONFIG_FC7300_FTU2_CAP_CHANNEL4
  {
    .channelid   = 36,
    .lower       = DECLARE_CAP_FTU_CHANNEL(20),
  },
#endif
#ifdef CONFIG_FC7300_FTU2_CAP_CHANNEL5
  {
    .channelid   = 37,
    .lower       = DECLARE_CAP_FTU_CHANNEL(21),
  },
#endif
#ifdef CONFIG_FC7300_FTU2_CAP_CHANNEL6
  {
    .channelid   = 38,
    .lower       = DECLARE_CAP_FTU_CHANNEL(22),
  },
#endif
#ifdef CONFIG_FC7300_FTU2_CAP_CHANNEL7
  {
    .channelid   = 39,
    .lower       = DECLARE_CAP_FTU_CHANNEL(23),
  },
#endif
#ifdef CONFIG_FC7300_FTU3_CAP_CHANNEL0
  {
    .channelid   = 40,
    .lower       = DECLARE_CAP_FTU_CHANNEL(24),
  },
#endif
#ifdef CONFIG_FC7300_FTU3_CAP_CHANNEL1
  {
    .channelid   = 41,
    .lower       = DECLARE_CAP_FTU_CHANNEL(25),
  },
#endif
#ifdef CONFIG_FC7300_FTU3_CAP_CHANNEL2
  {
    .channelid   = 42,
    .lower       = DECLARE_CAP_FTU_CHANNEL(26),
  },
#endif
#ifdef CONFIG_FC7300_FTU3_CAP_CHANNEL3
  {
    .channelid   = 43,
    .lower       = DECLARE_CAP_FTU_CHANNEL(27),
  },
#endif
#ifdef CONFIG_FC7300_FTU3_CAP_CHANNEL4
  {
    .channelid   = 44,
    .lower       = DECLARE_CAP_FTU_CHANNEL(28),
  },
#endif
#ifdef CONFIG_FC7300_FTU3_CAP_CHANNEL5
  {
    .channelid   = 45,
    .lower       = DECLARE_CAP_FTU_CHANNEL(29),
  },
#endif
#ifdef CONFIG_FC7300_FTU3_CAP_CHANNEL6
  {
    .channelid   = 46,
    .lower       = DECLARE_CAP_FTU_CHANNEL(30),
  },
#endif
#ifdef CONFIG_FC7300_FTU3_CAP_CHANNEL7
  {
    .channelid   = 47,
    .lower       = DECLARE_CAP_FTU_CHANNEL(31),
  },
#endif
#ifdef CONFIG_FC7300_FTU4_CAP_CHANNEL0
  {
    .channelid   = 48,
    .lower       = DECLARE_CAP_FTU_CHANNEL(32),
  },
#endif
#ifdef CONFIG_FC7300_FTU4_CAP_CHANNEL1
  {
    .channelid   = 49,
    .lower       = DECLARE_CAP_FTU_CHANNEL(33),
  },
#endif
#ifdef CONFIG_FC7300_FTU4_CAP_CHANNEL2
  {
    .channelid   = 50,
    .lower       = DECLARE_CAP_FTU_CHANNEL(34),
  },
#endif
#ifdef CONFIG_FC7300_FTU4_CAP_CHANNEL3
  {
    .channelid   = 51,
    .lower       = DECLARE_CAP_FTU_CHANNEL(35),
  },
#endif
#ifdef CONFIG_FC7300_FTU4_CAP_CHANNEL4
  {
    .channelid   = 52,
    .lower       = DECLARE_CAP_FTU_CHANNEL(36),
  },
#endif
#ifdef CONFIG_FC7300_FTU4_CAP_CHANNEL5
  {
    .channelid   = 53,
    .lower       = DECLARE_CAP_FTU_CHANNEL(37),
  },
#endif
#ifdef CONFIG_FC7300_FTU4_CAP_CHANNEL6
  {
    .channelid   = 54,
    .lower       = DECLARE_CAP_FTU_CHANNEL(38),
  },
#endif
#ifdef CONFIG_FC7300_FTU4_CAP_CHANNEL7
  {
    .channelid   = 55,
    .lower       = DECLARE_CAP_FTU_CHANNEL(39),
  },
#endif
#ifdef CONFIG_FC7300_FTU5_CAP_CHANNEL0
  {
    .channelid   = 56,
    .lower       = DECLARE_CAP_FTU_CHANNEL(40),
  },
#endif
#ifdef CONFIG_FC7300_FTU5_CAP_CHANNEL1
  {
    .channelid   = 57,
    .lower       = DECLARE_CAP_FTU_CHANNEL(41),
  },
#endif
#ifdef CONFIG_FC7300_FTU5_CAP_CHANNEL2
  {
    .channelid   = 58,
    .lower       = DECLARE_CAP_FTU_CHANNEL(42),
  },
#endif
#ifdef CONFIG_FC7300_FTU5_CAP_CHANNEL3
  {
    .channelid   = 59,
    .lower       = DECLARE_CAP_FTU_CHANNEL(43),
  },
#endif
#ifdef CONFIG_FC7300_FTU5_CAP_CHANNEL4
  {
    .channelid   = 60,
    .lower       = DECLARE_CAP_FTU_CHANNEL(44),
  },
#endif
#ifdef CONFIG_FC7300_FTU5_CAP_CHANNEL5
  {
    .channelid   = 61,
    .lower       = DECLARE_CAP_FTU_CHANNEL(45),
  },
#endif
#ifdef CONFIG_FC7300_FTU5_CAP_CHANNEL6
  {
    .channelid   = 62,
    .lower       = DECLARE_CAP_FTU_CHANNEL(46),
  },
#endif
#ifdef CONFIG_FC7300_FTU5_CAP_CHANNEL7
  {
    .channelid   = 63,
    .lower       = DECLARE_CAP_FTU_CHANNEL(47),
  },
#endif
#ifdef CONFIG_FC7300_FTU6_CAP_CHANNEL0
  {
    .channelid   = 64,
    .lower       = DECLARE_CAP_FTU_CHANNEL(48),
  },
#endif
#ifdef CONFIG_FC7300_FTU6_CAP_CHANNEL1
  {
    .channelid   = 65,
    .lower       = DECLARE_CAP_FTU_CHANNEL(49),
  },
#endif
#ifdef CONFIG_FC7300_FTU6_CAP_CHANNEL2
  {
    .channelid   = 66,
    .lower       = DECLARE_CAP_FTU_CHANNEL(50),
  },
#endif
#ifdef CONFIG_FC7300_FTU6_CAP_CHANNEL3
  {
    .channelid   = 67,
    .lower       = DECLARE_CAP_FTU_CHANNEL(51),
  },
#endif
#ifdef CONFIG_FC7300_FTU6_CAP_CHANNEL4
  {
    .channelid   = 68,
    .lower       = DECLARE_CAP_FTU_CHANNEL(52),
  },
#endif
#ifdef CONFIG_FC7300_FTU6_CAP_CHANNEL5
  {
    .channelid   = 69,
    .lower       = DECLARE_CAP_FTU_CHANNEL(53),
  },
#endif
#ifdef CONFIG_FC7300_FTU6_CAP_CHANNEL6
  {
    .channelid   = 70,
    .lower       = DECLARE_CAP_FTU_CHANNEL(54),
  },
#endif
#ifdef CONFIG_FC7300_FTU6_CAP_CHANNEL7
  {
    .channelid   = 71,
    .lower       = DECLARE_CAP_FTU_CHANNEL(55),
  },
#endif
#ifdef CONFIG_FC7300_FTU7_CAP_CHANNEL0
  {
    .channelid   = 72,
    .lower       = DECLARE_CAP_FTU_CHANNEL(56),
  },
#endif
#ifdef CONFIG_FC7300_FTU7_CAP_CHANNEL1
  {
    .channelid   = 73,
    .lower       = DECLARE_CAP_FTU_CHANNEL(57),
  },
#endif
#ifdef CONFIG_FC7300_FTU7_CAP_CHANNEL2
  {
    .channelid   = 74,
    .lower       = DECLARE_CAP_FTU_CHANNEL(58),
  },
#endif
#ifdef CONFIG_FC7300_FTU7_CAP_CHANNEL3
  {
    .channelid   = 75,
    .lower       = DECLARE_CAP_FTU_CHANNEL(59),
  },
#endif
#ifdef CONFIG_FC7300_FTU7_CAP_CHANNEL4
  {
    .channelid   = 76,
    .lower       = DECLARE_CAP_FTU_CHANNEL(60),
  },
#endif
#ifdef CONFIG_FC7300_FTU7_CAP_CHANNEL5
  {
    .channelid   = 77,
    .lower       = DECLARE_CAP_FTU_CHANNEL(61),
  },
#endif
#ifdef CONFIG_FC7300_FTU7_CAP_CHANNEL6
  {
    .channelid   = 78,
    .lower       = DECLARE_CAP_FTU_CHANNEL(62),
  },
#endif
#ifdef CONFIG_FC7300_FTU7_CAP_CHANNEL7
  {
    .channelid   = 79,
    .lower       = DECLARE_CAP_FTU_CHANNEL(63),
  },
#endif
#ifdef CONFIG_FC7300_FTU8_CAP_CHANNEL0
  {
    .channelid   = 80,
    .lower       = DECLARE_CAP_FTU_CHANNEL(64),
  },
#endif
#ifdef CONFIG_FC7300_FTU8_CAP_CHANNEL1
  {
    .channelid   = 81,
    .lower       = DECLARE_CAP_FTU_CHANNEL(65),
  },
#endif
#ifdef CONFIG_FC7300_FTU8_CAP_CHANNEL2
  {
    .channelid   = 82,
    .lower       = DECLARE_CAP_FTU_CHANNEL(66),
  },
#endif
#ifdef CONFIG_FC7300_FTU8_CAP_CHANNEL3
  {
    .channelid   = 83,
    .lower       = DECLARE_CAP_FTU_CHANNEL(67),
  },
#endif
#ifdef CONFIG_FC7300_FTU8_CAP_CHANNEL4
  {
    .channelid   = 84,
    .lower       = DECLARE_CAP_FTU_CHANNEL(68),
  },
#endif
#ifdef CONFIG_FC7300_FTU8_CAP_CHANNEL5
  {
    .channelid   = 85,
    .lower       = DECLARE_CAP_FTU_CHANNEL(69),
  },
#endif
#ifdef CONFIG_FC7300_FTU8_CAP_CHANNEL6
  {
    .channelid   = 86,
    .lower       = DECLARE_CAP_FTU_CHANNEL(70),
  },
#endif
#ifdef CONFIG_FC7300_FTU8_CAP_CHANNEL7
  {
    .channelid   = 87,
    .lower       = DECLARE_CAP_FTU_CHANNEL(71),
  },
#endif
#ifdef CONFIG_FC7300_FTU9_CAP_CHANNEL0
  {
    .channelid   = 88,
    .lower       = DECLARE_CAP_FTU_CHANNEL(72),
  },
#endif
#ifdef CONFIG_FC7300_FTU9_CAP_CHANNEL1
  {
    .channelid   = 89,
    .lower       = DECLARE_CAP_FTU_CHANNEL(73),
  },
#endif
#ifdef CONFIG_FC7300_FTU9_CAP_CHANNEL2
  {
    .channelid   = 90,
    .lower       = DECLARE_CAP_FTU_CHANNEL(74),
  },
#endif
#ifdef CONFIG_FC7300_FTU9_CAP_CHANNEL3
  {
    .channelid   = 91,
    .lower       = DECLARE_CAP_FTU_CHANNEL(75),
  },
#endif
#ifdef CONFIG_FC7300_FTU9_CAP_CHANNEL4
  {
    .channelid   = 92,
    .lower       = DECLARE_CAP_FTU_CHANNEL(76),
  },
#endif
#ifdef CONFIG_FC7300_FTU9_CAP_CHANNEL5
  {
    .channelid   = 93,
    .lower       = DECLARE_CAP_FTU_CHANNEL(77),
  },
#endif
#ifdef CONFIG_FC7300_FTU9_CAP_CHANNEL6
  {
    .channelid   = 94,
    .lower       = DECLARE_CAP_FTU_CHANNEL(78),
  },
#endif
#ifdef CONFIG_FC7300_FTU9_CAP_CHANNEL7
  {
    .channelid   = 95,
    .lower       = DECLARE_CAP_FTU_CHANNEL(79),
  },
#endif
#ifdef CONFIG_FC7300_FTU10_CAP_CHANNEL0
  {
    .channelid   = 96,
    .lower       = DECLARE_CAP_FTU_CHANNEL(80),
  },
#endif
#ifdef CONFIG_FC7300_FTU10_CAP_CHANNEL1
  {
    .channelid   = 97,
    .lower       = DECLARE_CAP_FTU_CHANNEL(81),
  },
#endif
#ifdef CONFIG_FC7300_FTU10_CAP_CHANNEL2
  {
    .channelid   = 98,
    .lower       = DECLARE_CAP_FTU_CHANNEL(82),
  },
#endif
#ifdef CONFIG_FC7300_FTU10_CAP_CHANNEL3
  {
    .channelid   = 99,
    .lower       = DECLARE_CAP_FTU_CHANNEL(83),
  },
#endif
#ifdef CONFIG_FC7300_FTU10_CAP_CHANNEL4
  {
    .channelid   = 100,
    .lower       = DECLARE_CAP_FTU_CHANNEL(84),
  },
#endif
#ifdef CONFIG_FC7300_FTU10_CAP_CHANNEL5
  {
    .channelid   = 101,
    .lower       = DECLARE_CAP_FTU_CHANNEL(85),
  },
#endif
#ifdef CONFIG_FC7300_FTU10_CAP_CHANNEL6
  {
    .channelid   = 102,
    .lower       = DECLARE_CAP_FTU_CHANNEL(86),
  },
#endif
#ifdef CONFIG_FC7300_FTU10_CAP_CHANNEL7
  {
    .channelid   = 103,
    .lower       = DECLARE_CAP_FTU_CHANNEL(87),
  },
#endif
#ifdef CONFIG_FC7300_FTU11_CAP_CHANNEL0
  {
    .channelid   = 104,
    .lower       = DECLARE_CAP_FTU_CHANNEL(88),
  },
#endif
#ifdef CONFIG_FC7300_FTU11_CAP_CHANNEL1
  {
    .channelid   = 105,
    .lower       = DECLARE_CAP_FTU_CHANNEL(89),
  },
#endif
#ifdef CONFIG_FC7300_FTU11_CAP_CHANNEL2
  {
    .channelid   = 106,
    .lower       = DECLARE_CAP_FTU_CHANNEL(90),
  },
#endif
#ifdef CONFIG_FC7300_FTU11_CAP_CHANNEL3
  {
    .channelid   = 107,
    .lower       = DECLARE_CAP_FTU_CHANNEL(91),
  },
#endif
#ifdef CONFIG_FC7300_FTU11_CAP_CHANNEL4
  {
    .channelid   = 108,
    .lower       = DECLARE_CAP_FTU_CHANNEL(92),
  },
#endif
#ifdef CONFIG_FC7300_FTU11_CAP_CHANNEL5
  {
    .channelid   = 109,
    .lower       = DECLARE_CAP_FTU_CHANNEL(93),
  },
#endif
#ifdef CONFIG_FC7300_FTU11_CAP_CHANNEL6
  {
    .channelid   = 110,
    .lower       = DECLARE_CAP_FTU_CHANNEL(94),
  },
#endif
#ifdef CONFIG_FC7300_FTU11_CAP_CHANNEL7
  {
    .channelid   = 111,
    .lower       = DECLARE_CAP_FTU_CHANNEL(95),
  },
#endif
};

/* Array for saving signal measurement data of hwunits and channels */

static struct fc7300_cap_msmt_s g_cap_msmt[MAX_INSTANCE_COUNT];

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: cap_measurementdutycycle
 *
 * Description:
 *   measure duty cycle in isr handler.
 *
 * Input Parameters:
 *   lower      - A pointer the publicly visible representation of the
 *                "lower-half" driver state structure.
 *   ch         - Specified channel.
 *   inputlevel - Level state when entering interrupt.
 *   tick       - Counter when entering interrupt.
 * Returned Value:
 *   Whether final results calculated finish.
 *
 ****************************************************************************/

static bool cap_measurementdutycycle(struct fc7300_cap_lowerhalf_s *lower,
                                     uint8_t ch,
                                     enum ftu_inputleveltype_e inputlevel,
                                     uint32_t tick)
{
  bool completed = false;
  enum ftu_inputleveltype_e activelevel;
  uint8_t hwunit = lower->hwunit;

  if (FTU_INPUT_FALLING_EDGE == lower->mode)
    {
      activelevel = FTU_INPUT_LOW;
    }
  else if (FTU_INPUT_RISING_EDGE == lower->mode)
    {
      activelevel = FTU_INPUT_HIGH;
    }
  else
    {
      /* BOTH_EDGES */

      if (0u == g_cap_msmt[hwunit].signalmsmtseq[ch])
        {
          g_cap_msmt[hwunit].sigmsmtactivelevel[ch] = inputlevel;
        }

      activelevel = g_cap_msmt[hwunit].sigmsmtactivelevel[ch];
    }

  if (activelevel == inputlevel)
    {
      /* Active Edge */

      if (0u == g_cap_msmt[hwunit].signalmsmtseq[ch])
        {
          /* not start */

          g_cap_msmt[hwunit].signalmsmtseq[ch] = 1;
        }
      else if (2u == g_cap_msmt[hwunit].signalmsmtseq[ch])
        {
          /* second edge */

          g_cap_msmt[hwunit].sigmsmtactive[ch] = tick;
          g_cap_msmt[hwunit].signalmsmtseq[ch] = 3u;
#ifdef FC7300_CAP_OVERFLOW_USED
          g_cap_msmt[hwunit].ofexcnt[ch] = 0u;
#endif
        }
      else
        {
          /* do nothing */
        }
    }
  else
    {
      /* start Edge */

      if (1u >= g_cap_msmt[hwunit].signalmsmtseq[ch])
        {
          /* first edge */

          g_cap_msmt[hwunit].sigmsmtstart[ch] = tick;
          g_cap_msmt[hwunit].signalmsmtseq[ch] = 2u;
#ifdef FC7300_CAP_OVERFLOW_USED
          g_cap_msmt[hwunit].ofcnt[ch] = 0u;
#endif
        }
      else if (3u == g_cap_msmt[hwunit].signalmsmtseq[ch])
        {
          /* end edge */

#ifdef FC7300_CAP_OVERFLOW_USED
          g_cap_msmt[hwunit].periodofcnt[ch] =
          g_cap_msmt[hwunit].ofcnt[ch];
          g_cap_msmt[hwunit].period[ch] = cap_getsignalduration(lower->cap,
            tick, g_cap_msmt[hwunit].sigmsmtstart[ch],
            &g_cap_msmt[hwunit].periodofcnt[ch]);
          g_cap_msmt[hwunit].period[ch] +=
          g_cap_msmt[hwunit].periodofcnt[ch] * g_cap_msmt[hwunit].maxofcnt[ch];
          g_cap_msmt[hwunit].periodofcnt[ch] = 0u;
#else
          g_cap_msmt[hwunit].period[ch] = cap_getsignalduration(lower->cap,
                                   tick, g_cap_msmt[hwunit].sigmsmtstart[ch]);
#endif
#ifdef FC7300_CAP_OVERFLOW_USED
          g_cap_msmt[hwunit].activeofcnt[ch] = g_cap_msmt[hwunit].ofexcnt[ch];
          g_cap_msmt[hwunit].activewidth[ch] = cap_getsignalduration(lower->cap,
                                     tick, g_cap_msmt[hwunit].sigmsmtactive[ch],
                                     &g_cap_msmt[hwunit].activeofcnt[ch]);
          g_cap_msmt[hwunit].activewidth[ch] +=
            g_cap_msmt[hwunit].activeofcnt[ch]*g_cap_msmt[hwunit].maxofcnt[ch];
          g_cap_msmt[hwunit].activeofcnt[ch] = 0u;
#else
          g_cap_msmt[hwunit].activewidth[ch] = cap_getsignalduration(lower->cap,
                                  tick, g_cap_msmt[hwunit].sigmsmtactive[ch]);
#endif
          g_cap_msmt[hwunit].sigmsmtstart[ch] = tick;
          g_cap_msmt[hwunit].signalmsmtseq[ch] = 2u;
          if (g_cap_msmt[hwunit].period[ch] != 0U)
            {
              g_cap_msmt[hwunit].duty[ch] = g_cap_msmt[hwunit].activewidth[ch]
                                         * 100 / g_cap_msmt[hwunit].period[ch];
              g_cap_msmt[hwunit].freq[ch] = lower->clock /
                                            g_cap_msmt[hwunit].period[ch];
              completed = true;
#ifdef FC7300_CAP_OVERFLOW_USED
              g_cap_msmt[hwunit].ofcnt[ch] = 0u;
              g_cap_msmt[hwunit].ofexcnt[ch] = 0u;
#endif
            }
        }
      else
        {
          /* do nothing */
        }
    }

  return completed;
}

/****************************************************************************
 * Name: fc7300_cap_handler
 *
 * Description:
 *   timer interrupt handler
 *
 * Input Parameters:
 *
 * Returned Value:
 *
 ****************************************************************************/

__ramfunc__ static int fc7300_cap_handler(int irq, void * context, void * arg)
{
  struct fc7300_cap_lowerhalf_s *lower = (struct fc7300_cap_lowerhalf_s *) arg;
  uint8_t channelintflag = 0;
  int cnt = 0;
  uint8_t i = 0;
  uint32_t hwunit = lower->hwunit;
  enum ftu_inputleveltype_e inputlevel;

#ifdef FC7300_CAP_OVERFLOW_USED
  bool overflowflg = FC7300_CAP_READOVERFLOWFLG(lower->cap);
#endif
  /* Identify interrupt source channels */

  channelintflag = FC7300_CAP_ACKFLAGS(lower->cap);

  for (i = 0U; i < MAX_FTU_CHANNEL_COUNT; i++)
    {

#ifdef FC7300_CAP_OVERFLOW_USED
      if ((true == overflowflg) &&
          (0u != (g_capoverflowflag[hwunit] & (1u << i))))
        {
          g_cap_msmt[hwunit].ofcnt[i]++;
          g_cap_msmt[hwunit].ofexcnt[i]++;
        }
#endif

      if (channelintflag & (uint8_t)(1u << i))
        {
          /* Multiple channels may using one arg */

          cnt = FC7300_CAP_GETCAPTURE(lower->cap, hwunit, i);
          inputlevel = FC7300_CAP_GETINPUTLEVEL(lower->cap, i);
          (void)cap_measurementdutycycle(lower, i, inputlevel, cnt);
          g_cap_msmt[hwunit].edgecnt[i]++;
          g_cap_msmt[hwunit].edgedirection[i] = inputlevel;
          g_cap_msmt[hwunit].timestamp[i] = cnt;

          if (lower->cbtable[i].callback != NULL)
            {
              /* Call user function if necessary */

              lower->cbtable[i].callback();
            }
        }
    }

#ifdef CONFIG_FC7300_CAP_IRQ_ATTACH_THREAD
  fc7300_clearpendingisr(irq);
  up_enable_irq(irq);
#endif
  return OK;
}

/****************************************************************************
 * Name: cap_start
 *
 * Description:
 *   Start the timer, resetting the time to the current timeout,
 *
 * Input Parameters:
 *   lower - A pointer the publicly visible representation of the
 *           "lower-half" driver state structure.
 *
 * Returned Value:
 *   Zero on success; a negated errno value on failure.
 *
 ****************************************************************************/

static int cap_start(struct cap_lowerhalf_s *lower)
{
  struct fc7300_cap_lowerhalf_s *priv = (struct fc7300_cap_lowerhalf_s *)lower;
  int ret = OK;
  uint8_t ch;
  uint32_t hwunit = priv->hwunit;
  uint32_t cappccclk;

  if (hwunit == 12)
    {
      ch = priv->channel;
    }
  else
    {
      ch = (priv->channel - 16) - ((priv->channel - 16) / 8) * 8;
    }

  if (priv->started)
    {
      return ret;
    }
  else
    {
      irqstate_t flags = enter_critical_section();
#if defined(CONFIG_FC7300_FTU11_ALREADY_USED)
      if (hwunit == 11)
        {
          /* workaround for FTU11 both using pwm and capture */
        }
      else
#endif
        {
          ret = FC7300_CAP_SETCLOCK(priv->cap, hwunit, priv->clock, ch);
        }

#if defined(FC7300_CAP_OVERFLOW_USED) && defined(CONFIG_FC7300_FTU_CAP)
      if (hwunit != 12)
        {
          g_cap_msmt[hwunit].ofcnt[ch] = 0;
          g_cap_msmt[hwunit].ofexcnt[ch] = 0;
          g_cap_msmt[hwunit].maxofcnt[ch] =
          FC7300_CAP_GETOVERFLOWVAL(priv->cap);
        }
#endif

      if (ret == OK)
        {
          if (hwunit != 12)
            {
              cappccclk = fc7300_cap_pckfreq(hwunit);
              priv->clock = cappccclk / FC7300_CAP_GETPRESCALE(priv->cap);
            }

          ret = FC7300_CAP_SETCHANNEL(priv->cap, hwunit, ch, priv->pincfg,
                                      priv->trgsel, priv->mode);
          FC7300_CAP_SETISR(priv->cap, fc7300_cap_handler, priv);
          (void)FC7300_CAP_ENABLEINT(priv->cap, hwunit, ch, true);

          priv->started = true;
        }

      if ((priv->mode == 1) && (hwunit == 12))
        {
          /* open related ism channel */

          ret = FC7300_CAP_SETCLOCK(priv->cap, hwunit, priv->clock, (ch-1));
          if (ret == OK)
            {
              ret = FC7300_CAP_SETCHANNEL(priv->cap, hwunit, (ch-1), priv->pincfg,
                                          priv->trgsel, 0);
              (void)FC7300_CAP_ENABLEINT(priv->cap, hwunit, (ch-1), true);
            }
        }

      leave_critical_section(flags);
    }

  return ret;
}

/****************************************************************************
 * Name: cap_stop
 *
 * Description:
 *   Stop all the capture channels in specific timer module.
 *
 * Input Parameters:
 *   lower - A pointer the publicly visible representation of the
 *           "lower-half" driver state structure.
 *
 * Returned Value:
 *   Zero on success; a negated errno value on failure.
 *
 ****************************************************************************/

static int cap_stop(struct cap_lowerhalf_s *lower)
{
  struct fc7300_cap_lowerhalf_s *priv = (struct fc7300_cap_lowerhalf_s *)lower;
  int ret = OK;
  uint8_t ch;
  uint32_t hwunit = priv->hwunit;

  if (hwunit == 12)
    {
      ch = priv->channel;
    }
  else
    {
      ch = (priv->channel - 16) - ((priv->channel - 16) / 8) * 8;
    }

  if (priv->started)
    {
      irqstate_t flags = enter_critical_section();
      (void)FC7300_CAP_ENABLEINT(priv->cap, hwunit, ch, false);
      FC7300_CAP_SETISR(priv->cap, NULL, NULL);

      /* If PWM also used this FTU module, do not stop the timer */

      ret = FC7300_CAP_SETCLOCK(priv->cap, hwunit, 0, ch);
      priv->started = false;
      g_cap_msmt[hwunit].period[ch] = 0;

      if ((priv->mode == 1) && (hwunit == 12))
        {
          /* close related ism channel */

          (void)FC7300_CAP_ENABLEINT(priv->cap, hwunit, (ch-1), false);
          ret = FC7300_CAP_SETCLOCK(priv->cap, hwunit, 0, (ch-1));
        }
      leave_critical_section(flags);
      return ret;
    }

  /* Return ENODEV to indicate that the timer was not running */

  return -ENODEV;
}

/****************************************************************************
 * Name: cap_getduty
 *
 * Description:
 *   get result duty
 *
 * Input Parameters:
 *   lower - A pointer the publicly visible representation of the
 *             "lower-half" driver state structure.
 *   duty  - A pointer to get the duty value.
 *             The value is DutyCycle * 100.
 *
 * Returned Value:
 *   Zero on success; a negated errno value on failure.
 *
 ****************************************************************************/

static int cap_getduty(struct cap_lowerhalf_s *lower, uint8_t *duty)
{
  struct fc7300_cap_lowerhalf_s *priv = (struct fc7300_cap_lowerhalf_s *)lower;
  uint8_t ch;
  uint32_t hwunit = priv->hwunit;
  uint32_t freq = 0;

  if (hwunit == 12)
    {
      ch = priv->channel;
      if (priv->mode == 1)
        {
          if (g_cap_msmt[hwunit].freq[ch] == 0)
            {
              g_cap_msmt[hwunit].duty[ch] = 0;
            }
          else
            {
              freq = priv->clock / g_cap_msmt[hwunit].freq[ch];
              g_cap_msmt[hwunit].duty[ch] = FC7300_CAP_GETCAPTURE(priv->cap, hwunit, ch) * 100 / freq;
            }
        }
      else
        {
          g_cap_msmt[hwunit].duty[ch] = 0;
        }
    }
  else
    {
      ch = (priv->channel - 16) - ((priv->channel - 16) / 8) * 8;
    }

  irqstate_t flags = enter_critical_section();

  if (ch >= MAX_FTU_CHANNEL_COUNT)
    {
      leave_critical_section(flags);
      return ERROR;
    }

  *duty = g_cap_msmt[hwunit].duty[ch];

  leave_critical_section(flags);

  if (*duty <= 100)
    {
      return OK;
    }
  else
    {
      return ERROR;
    }
}

/****************************************************************************
 * Name: cap_getfreq
 *
 * Description:
 *   get result freq
 *
 * Input Parameters:
 *   lower - A pointer the publicly visible representation of the
 *             "lower-half" driver state structure.
 *   freq  - A pointer to get the freq value.
 *             Frequence in Hz.
 *
 * Returned Value:
 *   Zero on success; a negated errno value on failure.
 *
 ****************************************************************************/

static int cap_getfreq(struct cap_lowerhalf_s *lower, uint32_t *freq)
{
  struct fc7300_cap_lowerhalf_s *priv = (struct fc7300_cap_lowerhalf_s *)lower;
  uint8_t ch;
  uint32_t hwunit = priv->hwunit;

  if (hwunit == 12)
    {
      ch = priv->channel;
      if (priv->mode == 1)
        {
          g_cap_msmt[hwunit].freq[ch] = priv->clock / FC7300_CAP_GETCAPTURE(priv->cap, hwunit, (ch-1));
        }
      else
        {
          g_cap_msmt[hwunit].freq[ch] = (priv->clock / FC7300_CAP_GETCAPTURE(priv->cap, hwunit, ch));
        }
    }
  else
    {
      ch = (priv->channel - 16) - ((priv->channel - 16) / 8) * 8;
    }

  irqstate_t flags = enter_critical_section();

  if (ch >= MAX_FTU_CHANNEL_COUNT)
    {
      leave_critical_section(flags);
      return ERROR;
    }
  *freq = g_cap_msmt[hwunit].freq[ch];
  leave_critical_section(flags);
  return OK;
}

/****************************************************************************
 * Name: cap_getedges
 *
 * Description:
 *   get result edge counters
 *
 * Input Parameters:
 *   lower - A pointer the publicly visible representation of the
 *             "lower-half" driver state structure.
 *   edges  - A pointer to get the edge counters.
 *
 * Returned Value:
 *   Zero on success; a negated errno value on failure.
 *
 ****************************************************************************/

static int cap_getedges(struct cap_lowerhalf_s *lower, uint32_t *edges)
{
  struct fc7300_cap_lowerhalf_s *priv = (struct fc7300_cap_lowerhalf_s *)lower;
  uint8_t ch;
  uint32_t hwunit = priv->hwunit;

  irqstate_t flags = enter_critical_section();

  if (hwunit == 12)
    {
      ch = priv->channel;
      g_cap_msmt[hwunit].edgecnt[ch] = 0;
    }
  else
    {
      ch = (priv->channel - 16) - ((priv->channel - 16) / 8) * 8;
    }

  if (ch >= MAX_FTU_CHANNEL_COUNT)
    {
      leave_critical_section(flags);
      return ERROR;
    }

  *edges = g_cap_msmt[hwunit].edgecnt[ch];

  leave_critical_section(flags);

  return OK;
}

/****************************************************************************
 * Name: fc7300_cap_pckfreq
 *
 * Description:
 *   Get the peripheral clock frequency for the FTUX peripheral
 *
 * Input Parameters:
 *   base - The base address of the FTUX peripheral registers
 *
 * Returned Value:
 *   The frequency of the FTUX functional input frequency
 *   (or zero on a failure)
 *
 ****************************************************************************/

static uint32_t fc7300_cap_pckfreq(uint8_t hwunit)
{
  enum pcc_clksrctype_e clkname;
  uint32_t pccclk;
  int ret;

  /* Get the PCC source clock */

  switch (hwunit)
    {
      case 0:
        clkname = PCC_CLK_FTU0;
        break;
      case 1:
        clkname = PCC_CLK_FTU1;
        break;
      case 2:
        clkname = PCC_CLK_FTU2;
        break;
      case 3:
        clkname = PCC_CLK_FTU3;
        break;
      case 4:
        clkname = PCC_CLK_FTU4;
        break;
      case 5:
        clkname = PCC_CLK_FTU5;
        break;
      case 6:
        clkname = PCC_CLK_FTU6;
        break;
      case 7:
        clkname = PCC_CLK_FTU7;
        break;
      case 8:
        clkname = PCC_CLK_FTU8;
        break;
      case 9:
        clkname = PCC_CLK_FTU9;
        break;
      case 10:
        clkname = PCC_CLK_FTU10;
        break;
      case 11:
        clkname = PCC_CLK_FTU11;
        break;
      default:
        DEBUGPANIC();
        return -EINVAL;
    }

  ret = fc7300_get_pclkfreq(clkname, &pccclk);
  DEBUGASSERT(ret >= 0);
  if (ret < 0)
    {
      return 0;
    }

  return pccclk;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: fc7300_cap_initialize
 *
 * Description:
 *   Initialize one channel for use with the upper_level capture driver.
 *
 * Input Parameters:
 *   channel - A number identifying the channel use. The number of valid
 *     channel IDs varies with the FC7300 MCU and MCU family but is somewhere
 *     in the range of {0,..,63}.
 *
 * Returned Value:
 *   On success, a pointer to the FC7300 lower half capture driver returned.
 *   NULL is returned on any failure.
 *
 ****************************************************************************/

struct cap_lowerhalf_s *fc7300_cap_initialize(uint8_t channel,
                                        struct fc7300_callbackmap_s *cbtable)
{
  struct fc7300_cap_lowerhalf_s *lower = NULL;
  uint8_t i;
  uint8_t chnum = sizeof(g_cap_capturemap) / sizeof(g_cap_capturemap[0]);
  uint32_t cappccclk;
  struct fc7300_cap_lowerhalf_s *priv;

  for (i = 0; i < chnum; i++)
    {
      if (channel == g_cap_capturemap[i].channelid)
        {
            lower = &g_cap_capturemap[i].lower;
            lower->cbtable = cbtable;
            priv = (struct fc7300_cap_lowerhalf_s *)lower;
            break;
        }
    }

  if (i < chnum)
    {
      /* Initialize the elements of lower half state structure */

      lower->started  = false;
      lower->cap      = fc7300_cap_initpriv(channel);

      if (lower->cap == NULL)
        {
          lower = NULL;
        }
    }
  if (lower && lower->hwunit != 12)
    {
      cappccclk = fc7300_cap_pckfreq(lower->hwunit);
      lower->clock = cappccclk / FC7300_CAP_GETPRESCALE(priv->cap);
    }

  return (struct cap_lowerhalf_s *)lower;
}

/****************************************************************************
 * Name: fc7300_cap_clearfreqandduty
 *
 * Description:
 *   Clear the frequency and duty of history.
 *
 * Input Parameters:
 *   lower - A pointer the publicly visible representation of the
 *             "lower-half" driver state structure.
 *
 * Returned Value:
 *   void
 *
 ****************************************************************************/

void fc7300_cap_clearfreqandduty(struct fc7300_cap_lowerhalf_s *lower)
{
  uint8_t ch;
  uint32_t hwunit = lower->hwunit;

  if (hwunit != 12)
    {
      ch = (lower->channel - 16) - ((lower->channel - 16) / 8) * 8;
      g_cap_msmt[hwunit].freq[ch] = 0;
      g_cap_msmt[hwunit].duty[ch] = 0;
    }
}

#endif /* CONFIG_FC7300_CAP */
