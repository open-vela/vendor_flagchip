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

#include <nuttx/analog/adc.h>
#include <arch/board/board.h>

#include "chip.h"
#include "arm_internal.h"
#include "fc7300_adc.h"
#include "fc7300f8m-evb.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#ifdef CONFIG_FC7300_ADC0

/* Identifying sampletime of each used ADC channel in a sequence: */

#define SAMPLETIME_FC7300_ADC0_CH2    ADC_SAMPLE_TIME_OPTION_1
#define SAMPLETIME_FC7300_ADC0_CH1    ADC_SAMPLE_TIME_OPTION_1
#define SAMPLETIME_FC7300_ADC0_CH14   ADC_SAMPLE_TIME_OPTION_1
#define SAMPLETIME_FC7300_ADC0_CH15   ADC_SAMPLE_TIME_OPTION_1
#define SAMPLETIME_FC7300_ADC0_CH11   ADC_SAMPLE_TIME_OPTION_1
#define SAMPLETIME_FC7300_ADC0_CH16   ADC_SAMPLE_TIME_OPTION_1
#define SAMPLETIME_FC7300_ADC0_CH18   ADC_SAMPLE_TIME_OPTION_1
#define SAMPLETIME_FC7300_ADC0_CH19   ADC_SAMPLE_TIME_OPTION_1
#define SAMPLETIME_FC7300_ADC0_CH29   ADC_SAMPLE_TIME_OPTION_1
#define SAMPLETIME_FC7300_ADC0_CH30   ADC_SAMPLE_TIME_OPTION_1
#define SAMPLETIME_FC7300_ADC0_CH31   ADC_SAMPLE_TIME_OPTION_1
#define SAMPLETIME_FC7300_ADC0_CH13   ADC_SAMPLE_TIME_OPTION_1
#define SAMPLETIME_FC7300_ADC0_CH6    ADC_SAMPLE_TIME_OPTION_1
#define SAMPLETIME_FC7300_ADC0_CH27   ADC_SAMPLE_TIME_OPTION_1
#define SAMPLETIME_FC7300_ADC0_CH9    ADC_SAMPLE_TIME_OPTION_1
#define SAMPLETIME_FC7300_ADC0_CH8    ADC_SAMPLE_TIME_OPTION_1
#define SAMPLETIME_FC7300_ADC0_CH5    ADC_SAMPLE_TIME_OPTION_1
#define SAMPLETIME_FC7300_ADC0_CH12   ADC_SAMPLE_TIME_OPTION_1
#define SAMPLETIME_FC7300_ADC0_CH7    ADC_SAMPLE_TIME_OPTION_1
#define SAMPLETIME_FC7300_ADC0_CH0    ADC_SAMPLE_TIME_OPTION_1
#define SAMPLETIME_FC7300_ADC0_CH4    ADC_SAMPLE_TIME_OPTION_1
#define SAMPLETIME_FC7300_ADC0_CH20   ADC_SAMPLE_TIME_OPTION_1
#define SAMPLETIME_FC7300_ADC0_CH21   ADC_SAMPLE_TIME_OPTION_1
#define SAMPLETIME_FC7300_ADC0_CH22   ADC_SAMPLE_TIME_OPTION_1
#define SAMPLETIME_FC7300_ADC0_CH23   ADC_SAMPLE_TIME_OPTION_1
#define SAMPLETIME_FC7300_ADC0_CH3    ADC_SAMPLE_TIME_OPTION_1

#define DECLARE_ADC0_CHANNEL(X) \
{                                                         \
  .channel        = X,                                    \
  .sampletime     = SAMPLETIME_FC7300_ADC0_CH##X,         \
  .pincfg         = GPIO_ADC0_CH##X##IN,                  \
  .trgselsrc      = TRGSEL0_SRC_FCPIT0_CH0,                \
  .trgseldes      = TRGSEL0_TARGET_ADC0_TRG0,              \
}

/* Identifying number of each ADC channel in a sequence:
 * Channel number refer to adc_enum channeltype_e.
 *
 * ADC0: {DECLARE_ADC0_CHANNEL(32),
 *        DECLARE_ADC0_CHANNEL(34),
 *        DECLARE_ADC0_CHANNEL(33),
 *        DECLARE_ADC0_CHANNEL(0),
 *        DECLARE_ADC0_CHANNEL(4)};
 */

static struct adcchan_s g_adc0channels[] =
{
  DECLARE_ADC0_CHANNEL(2),    /* ADC_CHANNEL_2 */
  DECLARE_ADC0_CHANNEL(1),    /* ADC_CHANNEL_1 */
  DECLARE_ADC0_CHANNEL(14),   /* ADC_CHANNEL_14 */
  DECLARE_ADC0_CHANNEL(15),   /* ADC_CHANNEL_15 */
  DECLARE_ADC0_CHANNEL(11),   /* ADC_CHANNEL_11 */
  DECLARE_ADC0_CHANNEL(16),   /* ADC_CHANNEL_16 */
  DECLARE_ADC0_CHANNEL(18),   /* ADC_CHANNEL_18 */
  DECLARE_ADC0_CHANNEL(19),   /* ADC_CHANNEL_19 */
  DECLARE_ADC0_CHANNEL(29),   /* ADC_CHANNEL_29 */
  DECLARE_ADC0_CHANNEL(30),   /* ADC_CHANNEL_30 */
  DECLARE_ADC0_CHANNEL(31),   /* ADC_CHANNEL_31 */
  DECLARE_ADC0_CHANNEL(13),   /* ADC_CHANNEL_13 */
  DECLARE_ADC0_CHANNEL(6),    /* ADC_CHANNEL_6 */
  DECLARE_ADC0_CHANNEL(27),   /* ADC_CHANNEL_27 */
  DECLARE_ADC0_CHANNEL(9),    /* ADC_CHANNEL_9 */
  DECLARE_ADC0_CHANNEL(8),    /* ADC_CHANNEL_8 */
  DECLARE_ADC0_CHANNEL(5),    /* ADC_CHANNEL_5 */
  DECLARE_ADC0_CHANNEL(12),   /* ADC_CHANNEL_12 */
  DECLARE_ADC0_CHANNEL(7),    /* ADC_CHANNEL_7 */
  DECLARE_ADC0_CHANNEL(0),    /* ADC_CHANNEL_0 */
  DECLARE_ADC0_CHANNEL(4),    /* ADC_CHANNEL_4 */
  DECLARE_ADC0_CHANNEL(20),   /* ADC_CHANNEL_20 */
  DECLARE_ADC0_CHANNEL(21),   /* ADC_CHANNEL_21 */
  DECLARE_ADC0_CHANNEL(22),   /* ADC_CHANNEL_22 */
  DECLARE_ADC0_CHANNEL(23),   /* ADC_CHANNEL_23 */
  DECLARE_ADC0_CHANNEL(3),    /* ADC_CHANNEL_3 */
};

#define FC7300_ADC0_NUMCHANNELS \
              sizeof(g_adc0channels) / sizeof(g_adc0channels[0])

#endif /* CONFIG_FC7300_ADC0 */

#ifdef CONFIG_FC7300_ADC1

/* Identifying sampletime of each used ADC channel in a sequence: */

#define SAMPLETIME_FC7300_ADC1_CH14  ADC_SAMPLE_TIME_OPTION_1
#define SAMPLETIME_FC7300_ADC1_CH15  ADC_SAMPLE_TIME_OPTION_1
#define SAMPLETIME_FC7300_ADC1_CH30  ADC_SAMPLE_TIME_OPTION_1
#define SAMPLETIME_FC7300_ADC1_CH31  ADC_SAMPLE_TIME_OPTION_1
#define SAMPLETIME_FC7300_ADC1_CH2   ADC_SAMPLE_TIME_OPTION_1
#define SAMPLETIME_FC7300_ADC1_CH23  ADC_SAMPLE_TIME_OPTION_1
#define SAMPLETIME_FC7300_ADC1_CH17  ADC_SAMPLE_TIME_OPTION_1
#define SAMPLETIME_FC7300_ADC1_CH13  ADC_SAMPLE_TIME_OPTION_1
#define SAMPLETIME_FC7300_ADC1_CH12  ADC_SAMPLE_TIME_OPTION_1
#define SAMPLETIME_FC7300_ADC1_CH11  ADC_SAMPLE_TIME_OPTION_1
#define SAMPLETIME_FC7300_ADC1_CH10  ADC_SAMPLE_TIME_OPTION_1
#define SAMPLETIME_FC7300_ADC1_CH9   ADC_SAMPLE_TIME_OPTION_1
#define SAMPLETIME_FC7300_ADC1_CH8   ADC_SAMPLE_TIME_OPTION_1
#define SAMPLETIME_FC7300_ADC1_CH5   ADC_SAMPLE_TIME_OPTION_1
#define SAMPLETIME_FC7300_ADC1_CH1   ADC_SAMPLE_TIME_OPTION_1
#define SAMPLETIME_FC7300_ADC1_CH27  ADC_SAMPLE_TIME_OPTION_1
#define SAMPLETIME_FC7300_ADC1_CH4   ADC_SAMPLE_TIME_OPTION_1
#define SAMPLETIME_FC7300_ADC1_CH26  ADC_SAMPLE_TIME_OPTION_1
#define SAMPLETIME_FC7300_ADC1_CH0   ADC_SAMPLE_TIME_OPTION_1

#define DECLARE_ADC1_CHANNEL(X) \
{                                                          \
  .channel        = X,                                     \
  .sampletime     = SAMPLETIME_FC7300_ADC1_CH##X,          \
  .pincfg         = GPIO_ADC1_CH##X##IN,                   \
  .trgselsrc      = TRGSEL0_SRC_FCPIT0_CH0,                \
  .trgseldes      = TRGSEL0_TARGET_ADC1_TRG0,              \
}

static struct adcchan_s g_adc1channels[] =
{
  DECLARE_ADC1_CHANNEL(14),   /* ADC_CHANNEL_14 */
  DECLARE_ADC1_CHANNEL(15),   /* ADC_CHANNEL_15 */
  DECLARE_ADC1_CHANNEL(30),   /* ADC_CHANNEL_30 */
  DECLARE_ADC1_CHANNEL(31),   /* ADC_CHANNEL_31 */
  DECLARE_ADC1_CHANNEL(2),    /* ADC_CHANNEL_2 */
  DECLARE_ADC1_CHANNEL(23),   /* ADC_CHANNEL_23 */
  DECLARE_ADC1_CHANNEL(17),   /* ADC_CHANNEL_17 */
  DECLARE_ADC1_CHANNEL(13),   /* ADC_CHANNEL_13 */
  DECLARE_ADC1_CHANNEL(12),   /* ADC_CHANNEL_12 */
  DECLARE_ADC1_CHANNEL(11),   /* ADC_CHANNEL_11 */
  DECLARE_ADC1_CHANNEL(10),   /* ADC_CHANNEL_10 */
  DECLARE_ADC1_CHANNEL(9),    /* ADC_CHANNEL_9 */
  DECLARE_ADC1_CHANNEL(8),    /* ADC_CHANNEL_8 */
  DECLARE_ADC1_CHANNEL(5),    /* ADC_CHANNEL_5 */
  DECLARE_ADC1_CHANNEL(1),    /* ADC_CHANNEL_1 */
  DECLARE_ADC1_CHANNEL(27),   /* ADC_CHANNEL_27 */
  DECLARE_ADC1_CHANNEL(4),    /* ADC_CHANNEL_4 */
  DECLARE_ADC1_CHANNEL(26),   /* ADC_CHANNEL_26 */
  DECLARE_ADC1_CHANNEL(0),    /* ADC_CHANNEL_0 */
};

#define FC7300_ADC1_NUMCHANNELS \
              sizeof(g_adc1channels) / sizeof(g_adc1channels[0])

#endif /* CONFIG_FC7300_ADC1 */

#ifdef CONFIG_FC7300_ADC2

/* Identifying sampletime of each used ADC channel in a sequence: */

#define SAMPLETIME_FC7300_ADC2_CH13  ADC_SAMPLE_TIME_OPTION_1
#define SAMPLETIME_FC7300_ADC2_CH11  ADC_SAMPLE_TIME_OPTION_1
#define SAMPLETIME_FC7300_ADC2_CH29  ADC_SAMPLE_TIME_OPTION_1
#define SAMPLETIME_FC7300_ADC2_CH28  ADC_SAMPLE_TIME_OPTION_1
#define SAMPLETIME_FC7300_ADC2_CH27  ADC_SAMPLE_TIME_OPTION_1
#define SAMPLETIME_FC7300_ADC2_CH26  ADC_SAMPLE_TIME_OPTION_1
#define SAMPLETIME_FC7300_ADC2_CH25  ADC_SAMPLE_TIME_OPTION_1
#define SAMPLETIME_FC7300_ADC2_CH20  ADC_SAMPLE_TIME_OPTION_1
#define SAMPLETIME_FC7300_ADC2_CH3   ADC_SAMPLE_TIME_OPTION_1
#define SAMPLETIME_FC7300_ADC2_CH6   ADC_SAMPLE_TIME_OPTION_1
#define SAMPLETIME_FC7300_ADC2_CH0   ADC_SAMPLE_TIME_OPTION_1
#define SAMPLETIME_FC7300_ADC2_CH2   ADC_SAMPLE_TIME_OPTION_1
#define SAMPLETIME_FC7300_ADC2_CH7   ADC_SAMPLE_TIME_OPTION_1
#define SAMPLETIME_FC7300_ADC2_CH10  ADC_SAMPLE_TIME_OPTION_1
#define SAMPLETIME_FC7300_ADC2_CH12  ADC_SAMPLE_TIME_OPTION_1
#define SAMPLETIME_FC7300_ADC2_CH9   ADC_SAMPLE_TIME_OPTION_1
#define SAMPLETIME_FC7300_ADC2_CH8   ADC_SAMPLE_TIME_OPTION_1
#define SAMPLETIME_FC7300_ADC2_CH16  ADC_SAMPLE_TIME_OPTION_1
#define SAMPLETIME_FC7300_ADC2_CH17  ADC_SAMPLE_TIME_OPTION_1
#define SAMPLETIME_FC7300_ADC2_CH30  ADC_SAMPLE_TIME_OPTION_1
#define SAMPLETIME_FC7300_ADC2_CH15  ADC_SAMPLE_TIME_OPTION_1
#define SAMPLETIME_FC7300_ADC2_CH18  ADC_SAMPLE_TIME_OPTION_1
#define SAMPLETIME_FC7300_ADC2_CH4   ADC_SAMPLE_TIME_OPTION_1
#define SAMPLETIME_FC7300_ADC2_CH5   ADC_SAMPLE_TIME_OPTION_1
#define SAMPLETIME_FC7300_ADC2_CH21  ADC_SAMPLE_TIME_OPTION_1
#define SAMPLETIME_FC7300_ADC2_CH22  ADC_SAMPLE_TIME_OPTION_1
#define SAMPLETIME_FC7300_ADC2_CH1   ADC_SAMPLE_TIME_OPTION_1
#define SAMPLETIME_FC7300_ADC2_CH24  ADC_SAMPLE_TIME_OPTION_1

#define DECLARE_ADC2_CHANNEL(X) \
{                                                          \
  .channel        = X,                                     \
  .sampletime     = SAMPLETIME_FC7300_ADC2_CH##X,          \
  .pincfg         = GPIO_ADC2_CH##X##IN,                   \
  .trgselsrc      = TRGSEL0_SRC_FCPIT0_CH0,                \
  .trgseldes      = TRGSEL0_TARGET_ADC2_TRG0,              \
}

static struct adcchan_s g_adc2channels[] =
{
  DECLARE_ADC2_CHANNEL(13),   /* ADC_CHANNEL_13 */
  DECLARE_ADC2_CHANNEL(11),   /* ADC_CHANNEL_11 */
  DECLARE_ADC2_CHANNEL(29),   /* ADC_CHANNEL_29 */
  DECLARE_ADC2_CHANNEL(28),   /* ADC_CHANNEL_28 */
  DECLARE_ADC2_CHANNEL(27),   /* ADC_CHANNEL_27 */
  DECLARE_ADC2_CHANNEL(26),   /* ADC_CHANNEL_26 */
  DECLARE_ADC2_CHANNEL(25),   /* ADC_CHANNEL_25 */
  DECLARE_ADC2_CHANNEL(20),   /* ADC_CHANNEL_20 */
  DECLARE_ADC2_CHANNEL(3),    /* ADC_CHANNEL_3 */
  DECLARE_ADC2_CHANNEL(6),    /* ADC_CHANNEL_6 */
  DECLARE_ADC2_CHANNEL(0),    /* ADC_CHANNEL_0 */
  DECLARE_ADC2_CHANNEL(2),    /* ADC_CHANNEL_2 */
  DECLARE_ADC2_CHANNEL(7),    /* ADC_CHANNEL_7 */
  DECLARE_ADC2_CHANNEL(10),   /* ADC_CHANNEL_10 */
  DECLARE_ADC2_CHANNEL(12),   /* ADC_CHANNEL_12 */
  DECLARE_ADC2_CHANNEL(9),    /* ADC_CHANNEL_9 */
  DECLARE_ADC2_CHANNEL(8),    /* ADC_CHANNEL_8 */
  DECLARE_ADC2_CHANNEL(16),   /* ADC_CHANNEL_16 */
  DECLARE_ADC2_CHANNEL(17),   /* ADC_CHANNEL_17 */
  DECLARE_ADC2_CHANNEL(30),   /* ADC_CHANNEL_30 */
  DECLARE_ADC2_CHANNEL(15),   /* ADC_CHANNEL_15 */
  DECLARE_ADC2_CHANNEL(18),   /* ADC_CHANNEL_18 */
  DECLARE_ADC2_CHANNEL(4),    /* ADC_CHANNEL_4 */
  DECLARE_ADC2_CHANNEL(5),    /* ADC_CHANNEL_5 */
  DECLARE_ADC2_CHANNEL(21),   /* ADC_CHANNEL_21 */
  DECLARE_ADC2_CHANNEL(22),   /* ADC_CHANNEL_22 */
  DECLARE_ADC2_CHANNEL(1),    /* ADC_CHANNEL_1 */
  DECLARE_ADC2_CHANNEL(24),   /* ADC_CHANNEL_24 */
};

#define FC7300_ADC2_NUMCHANNELS \
              sizeof(g_adc2channels) / sizeof(g_adc2channels[0])

#endif /* CONFIG_FC7300_ADC2 */

#ifdef CONFIG_FC7300_ADC3

/* Identifying sampletime of each used ADC channel in a sequence: */

#define SAMPLETIME_FC7300_ADC3_CH18  ADC_SAMPLE_TIME_OPTION_0
#define SAMPLETIME_FC7300_ADC3_CH17  ADC_SAMPLE_TIME_OPTION_0
#define SAMPLETIME_FC7300_ADC3_CH31  ADC_SAMPLE_TIME_OPTION_0
#define SAMPLETIME_FC7300_ADC3_CH30  ADC_SAMPLE_TIME_OPTION_0
#define SAMPLETIME_FC7300_ADC3_CH29  ADC_SAMPLE_TIME_OPTION_0
#define SAMPLETIME_FC7300_ADC3_CH19  ADC_SAMPLE_TIME_OPTION_0
#define SAMPLETIME_FC7300_ADC3_CH21  ADC_SAMPLE_TIME_OPTION_0
#define SAMPLETIME_FC7300_ADC3_CH22  ADC_SAMPLE_TIME_OPTION_0
#define SAMPLETIME_FC7300_ADC3_CH0   ADC_SAMPLE_TIME_OPTION_0
#define SAMPLETIME_FC7300_ADC3_CH20  ADC_SAMPLE_TIME_OPTION_0
#define SAMPLETIME_FC7300_ADC3_CH1   ADC_SAMPLE_TIME_OPTION_0
#define SAMPLETIME_FC7300_ADC3_CH4   ADC_SAMPLE_TIME_OPTION_0
#define SAMPLETIME_FC7300_ADC3_CH2   ADC_SAMPLE_TIME_OPTION_0
#define SAMPLETIME_FC7300_ADC3_CH10  ADC_SAMPLE_TIME_OPTION_0
#define SAMPLETIME_FC7300_ADC3_CH23  ADC_SAMPLE_TIME_OPTION_0
#define SAMPLETIME_FC7300_ADC3_CH24  ADC_SAMPLE_TIME_OPTION_0
#define SAMPLETIME_FC7300_ADC3_CH3   ADC_SAMPLE_TIME_OPTION_0
#define SAMPLETIME_FC7300_ADC3_CH14  ADC_SAMPLE_TIME_OPTION_0
#define SAMPLETIME_FC7300_ADC3_CH13  ADC_SAMPLE_TIME_OPTION_0
#define SAMPLETIME_FC7300_ADC3_CH15  ADC_SAMPLE_TIME_OPTION_0
#define SAMPLETIME_FC7300_ADC3_CH25  ADC_SAMPLE_TIME_OPTION_0
#define SAMPLETIME_FC7300_ADC3_CH8   ADC_SAMPLE_TIME_OPTION_0
#define SAMPLETIME_FC7300_ADC3_CH9   ADC_SAMPLE_TIME_OPTION_0
#define SAMPLETIME_FC7300_ADC3_CH16  ADC_SAMPLE_TIME_OPTION_0
#define SAMPLETIME_FC7300_ADC3_CH11  ADC_SAMPLE_TIME_OPTION_0
#define SAMPLETIME_FC7300_ADC3_CH12  ADC_SAMPLE_TIME_OPTION_0
#define SAMPLETIME_FC7300_ADC3_CH28  ADC_SAMPLE_TIME_OPTION_0
#define SAMPLETIME_FC7300_ADC3_CH27  ADC_SAMPLE_TIME_OPTION_0
#define SAMPLETIME_FC7300_ADC3_CH26  ADC_SAMPLE_TIME_OPTION_0

#define DECLARE_ADC3_CHANNEL(X) \
{                                                          \
  .channel        = X,                                     \
  .sampletime     = SAMPLETIME_FC7300_ADC3_CH##X,          \
  .pincfg         = GPIO_ADC3_CH##X##IN,                   \
  .trgselsrc      = TRGSEL0_SRC_FCPIT0_CH0,                \
  .trgseldes      = TRGSEL0_TARGET_ADC3_TRG0,              \
}

static struct adcchan_s g_adc3channels[] =
{
  DECLARE_ADC3_CHANNEL(18),   /* ADC_CHANNEL_18 */
  DECLARE_ADC3_CHANNEL(17),   /* ADC_CHANNEL_17 */
  DECLARE_ADC3_CHANNEL(31),   /* ADC_CHANNEL_31 */
  DECLARE_ADC3_CHANNEL(30),   /* ADC_CHANNEL_30 */
  DECLARE_ADC3_CHANNEL(29),   /* ADC_CHANNEL_29 */
  DECLARE_ADC3_CHANNEL(19),   /* ADC_CHANNEL_19 */
  DECLARE_ADC3_CHANNEL(21),   /* ADC_CHANNEL_21 */
  DECLARE_ADC3_CHANNEL(22),   /* ADC_CHANNEL_22 */
  DECLARE_ADC3_CHANNEL(0),    /* ADC_CHANNEL_0 */
  DECLARE_ADC3_CHANNEL(20),   /* ADC_CHANNEL_20 */
  DECLARE_ADC3_CHANNEL(1),    /* ADC_CHANNEL_1 */
  DECLARE_ADC3_CHANNEL(4),    /* ADC_CHANNEL_4 */
  DECLARE_ADC3_CHANNEL(2),    /* ADC_CHANNEL_2 */
  DECLARE_ADC3_CHANNEL(10),   /* ADC_CHANNEL_10 */
  DECLARE_ADC3_CHANNEL(23),   /* ADC_CHANNEL_23 */
  DECLARE_ADC3_CHANNEL(24),   /* ADC_CHANNEL_24 */
  DECLARE_ADC3_CHANNEL(3),    /* ADC_CHANNEL_3 */
  DECLARE_ADC3_CHANNEL(14),   /* ADC_CHANNEL_14 */
  DECLARE_ADC3_CHANNEL(13),   /* ADC_CHANNEL_13 */
  DECLARE_ADC3_CHANNEL(15),   /* ADC_CHANNEL_15 */
  DECLARE_ADC3_CHANNEL(25),   /* ADC_CHANNEL_25 */
  DECLARE_ADC3_CHANNEL(8),    /* ADC_CHANNEL_8 */
  DECLARE_ADC3_CHANNEL(9),    /* ADC_CHANNEL_9 */
  DECLARE_ADC3_CHANNEL(16),   /* ADC_CHANNEL_16 */
  DECLARE_ADC3_CHANNEL(11),   /* ADC_CHANNEL_11 */
  DECLARE_ADC3_CHANNEL(12),   /* ADC_CHANNEL_12 */
  DECLARE_ADC3_CHANNEL(28),   /* ADC_CHANNEL_28 */
  DECLARE_ADC3_CHANNEL(27),   /* ADC_CHANNEL_27 */
  DECLARE_ADC3_CHANNEL(26),   /* ADC_CHANNEL_26 */
};

#define FC7300_ADC3_NUMCHANNELS \
              sizeof(g_adc3channels) / sizeof(g_adc3channels[0])

#endif /* CONFIG_FC7300_ADC3 */

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: fc7300_adc_setup
 *
 * Description:
 *   Initialize ADC and register the ADC device.
 *
 ****************************************************************************/

int fc7300_adc_setup(void)
{
  static bool initialized = false;
  struct adc_dev_s *adc;
  int ret = OK;

  /* Have we already initialized? */

  if (!initialized)
    {
#ifdef CONFIG_FC7300_ADC0

      /* Call fc7300_adc_initialize() to get instance 0 of adc interface */

      adc = fc7300_adc_initialize(ADC_INSTANCE_0, g_adc0channels,
                                  FC7300_ADC0_NUMCHANNELS);
      if (!adc)
        {
          aerr("ERROR: Failed to get ADC0 interface\n");
          return -ENODEV;
        }

      /* Register the ADC driver at "/dev/adc0" */

      ret = adc_register("/dev/adc0", adc);
      if (ret < 0)
        {
          aerr("ERROR: adc_register failed: %d\n", ret);
          return ret;
        }
#endif

#ifdef CONFIG_FC7300_ADC1

      /* Call fc7300_adc_initialize() to get instance 1 of adc interface */

      adc = fc7300_adc_initialize(ADC_INSTANCE_1, g_adc1channels,
                                  FC7300_ADC1_NUMCHANNELS);
      if (!adc)
        {
          aerr("ERROR: Failed to get ADC1 interface\n");
          return -ENODEV;
        }

      /* Register the ADC driver at "/dev/adc1" */

      ret = adc_register("/dev/adc1", adc);
      if (ret < 0)
        {
          aerr("ERROR: adc_register failed: %d\n", ret);
          return ret;
        }
#endif

#ifdef CONFIG_FC7300_ADC2

      /* Call fc7300_adc_initialize() to get instance 2 of adc interface */

      adc = fc7300_adc_initialize(ADC_INSTANCE_2, g_adc2channels,
                                  FC7300_ADC2_NUMCHANNELS);
      if (!adc)
        {
          aerr("ERROR: Failed to get ADC2 interface\n");
          return -ENODEV;
        }

      /* Register the ADC driver at "/dev/adc2" */

      ret = adc_register("/dev/adc2", adc);
      if (ret < 0)
        {
          aerr("ERROR: adc_register failed: %d\n", ret);
          return ret;
        }
#endif

#ifdef CONFIG_FC7300_ADC3

      /* Call fc7300_adc_initialize() to get instance 3 of adc interface */

      adc = fc7300_adc_initialize(ADC_INSTANCE_3, g_adc3channels,
                                  FC7300_ADC3_NUMCHANNELS);
      if (!adc)
        {
          aerr("ERROR: Failed to get ADC3 interface\n");
          return -ENODEV;
        }

      /* Register the ADC driver at "/dev/adc3" */

      ret = adc_register("/dev/adc3", adc);
      if (ret < 0)
        {
          aerr("ERROR: adc_register failed: %d\n", ret);
          return ret;
        }
#endif

#if defined(CONFIG_FC7300_ADC0) || defined(CONFIG_FC7300_ADC1) || \
    defined(CONFIG_FC7300_ADC2) || defined(CONFIG_FC7300_ADC3)
      /* Now we are initialized */

      initialized = true;
#else
      return -ENOSYS;
#endif
    }

  return ret;
}
