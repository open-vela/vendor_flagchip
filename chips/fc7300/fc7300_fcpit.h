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

#ifndef __VENDOR_FLAGCHIP_CHIPS_FC7300_FC7300_FCPIT_H
#define __VENDOR_FLAGCHIP_CHIPS_FC7300_FC7300_FCPIT_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>

#include "chip.h"
#include "hardware/fc7300_fcpit.h"

#include <nuttx/irq.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#define FC7300_FCPIT0_CH0_RES   32
#define FC7300_FCPIT0_CH1_RES   32
#define FC7300_FCPIT0_CH2_RES   32
#define FC7300_FCPIT0_CH3_RES   32
#define FC7300_FCPIT1_CH0_RES   32
#define FC7300_FCPIT1_CH1_RES   32
#define FC7300_FCPIT1_CH2_RES   32
#define FC7300_FCPIT1_CH3_RES   32

/* Helpers ******************************************************************/

#define FC7300_FCPIT_SETMODE(d,mode)          ((d)->ops->setmode(d,mode))
#define FC7300_FCPIT_SETCLOCK(d,freq)         ((d)->ops->setclock(d,freq))
#define FC7300_FCPIT_GETSETCOUNTER(d)         ((d)->ops->getsetcounter(d))
#define FC7300_FCPIT_GETCOUNTER(d)            ((d)->ops->getcounter(d))
#define FC7300_FCPIT_SETCOUNTER(d,c)          ((d)->ops->setcounter(d,c))
#define FC7300_FCPIT_SETCHANNEL(d,ch,en)      ((d)->ops->setchannel(d,ch,en))
#define FC7300_FCPIT_SETISR(d,hnd,arg,s)      ((d)->ops->setisr(d,hnd,arg,s))
#define FC7300_FCPIT_ENABLEINT(d,c)           ((d)->ops->enableint(d,c))
#define FC7300_FCPIT_DISABLEINT(d,c)          ((d)->ops->disableint(d,c))
#define FC7300_FCPIT_ACKINT(d,c)              ((d)->ops->ackint(d,c))
#define FC7300_FCPIT_PCKFREQ(d)               ((d)->ops->pckclk(d))

/****************************************************************************
 * Public Types
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

/* TIM Device Structure */

struct fc7300_fcpit_dev_s
{
  struct fc7300_fcpit_ops_s *ops;
};

/* TIM Modes of Operation */

enum fc7300_fcpit_mode_e
{
  FC7300_FCPIT_MODE_UNUSED       = -1,

  /* One of the following */

  FC7300_FCPIT_32PERIODIC_COUNTER = 0,
  FC7300_FCPIT_DUAL_16PERIODIC_COUNTER,
  FC7300_FCPIT_ACCUMULATOR,
  FC7300_FCPIT_INPUT_CAPTURE,
  FC7300_FCPIT_DIABLED,
};

/* Fcpit channel number */

enum fc7300_fcpit_channeltype_e
{
  FCPIT_CHANNEL_0 = 0U,
  FCPIT_CHANNEL_1,
  FCPIT_CHANNEL_2,
  FCPIT_CHANNEL_3
};

struct fc7300_fcpit_ops_s
{
  /* Basic Timers */
  int  (*setmode)(struct fc7300_fcpit_dev_s *dev, enum fc7300_fcpit_mode_e mode);
  void  (*setclock)(struct fc7300_fcpit_dev_s *dev, uint32_t freq);
  uint32_t (*getsetcounter)(struct fc7300_fcpit_dev_s *dev);
  uint32_t (*getcounter)(struct fc7300_fcpit_dev_s *dev);
  int (*setcounter)(struct fc7300_fcpit_dev_s *dev, uint32_t count);

  /* General and Advanced Timers Adds */
  int  (*setchannel)(struct fc7300_fcpit_dev_s *dev,
                     uint8_t channel, bool enable);

  /* Timer interrupts */
  int  (*setisr)(struct fc7300_fcpit_dev_s *dev,
                 xcpt_t handler, void *arg, int source);
  int (*enableint)(struct fc7300_fcpit_dev_s *dev,
                    uint8_t channel);
  int (*disableint)(struct fc7300_fcpit_dev_s *dev,
                     uint8_t channel);
  int (*ackint)(struct fc7300_fcpit_dev_s *dev,
                 uint32_t channel);
  uint32_t (*pckclk)(struct fc7300_fcpit_dev_s *dev);
};

/****************************************************************************
 * Public Functions Prototypes
 ****************************************************************************/

/* Power-up timer and get its structure */

struct fc7300_fcpit_dev_s *fc7300_fcpit_init(int timer);

/* Power-down timer, mark it as unused */

int fc7300_fcpit_deinit(struct fc7300_fcpit_dev_s * dev);

#ifdef CONFIG_TIMER

/****************************************************************************
 * Name: fc7300_timer_initialize
 *
 * Description:
 *   Bind the configuration timer to a timer lower half instance and
 *   register the timer drivers at 'devpath'
 *
 * Input Parameters:
 *   devpath - The full path to the timer device.
 *              This should be of the form /dev/timer0
 *   timer - the timer number.
 *
 * Returned Value:
 *   Zero (OK) is returned on success; A negated errno value is returned
 *   to indicate the nature of any failure.
 *
 ****************************************************************************/

int fc7300_timer_initialize(const char *devpath, int timer);

/****************************************************************************
 * Name: fc7300_timer_driver_setup
 *
 * Description:
 *   Configure the timer driver.
 *
 * Input Parameters:
 *   devpath - The full path to the timer device.  This should be of the
 *             form /dev/timer0
 *   timer   - The timer's number.
 *
 * Returned Value:
 *   Zero (OK) is returned on success; A negated errno value is returned
 *   to indicate the nature of any failure.
 *
 ****************************************************************************/

int fc7300_timer_driver_setup(const char *devpath, int timer);
#endif

#ifdef CONFIG_SCHED_TICKLESS
/****************************************************************************
 * Name: fc7300_tickless_handler
 *
 * Description:
 *   Generic interrupt handler for this timer.  It checks the source of the
 *   interrupt and fires the appropriate handler.
 *
 * Input Parameters:
 *   None
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/

int fc7300_tickless_handler(int irq, void *context, void *arg);
#endif

#undef EXTERN
#if defined(__cplusplus)
}
#endif

#endif /* __ASSEMBLY__ */
#endif /* __VENDOR_FLAGCHIP_CHIPS_FC7300_FC7300_FCPIT_H */
