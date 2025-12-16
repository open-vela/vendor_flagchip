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

#ifndef __ARCH_ARM_SRC_FC7300_FC7300_CAPTURE_H
#define __ARCH_ARM_SRC_FC7300_FC7300_CAPTURE_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>
#include <sys/types.h>

#include "chip.h"
#include <arch/board/board.h>
#include "hardware/fc7300_ftu.h"
#include "hardware/fc7300_gpio.h"
#include "hardware/fc7300_pin.h"
#include "hardware/fc7300_pinmux.h"
#include "hardware/fc7300_trgsel.h"
#include "hardware/fc7300_ism.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Helpers ******************************************************************/

#if defined(CONFIG_FC7300_FTU0_CAP) && defined(CONFIG_FC7300_FTU0_PWM)
#  define CONFIG_FC7300_FTU0_ALREADY_USED   TRUE
#endif

#if defined(CONFIG_FC7300_FTU1_CAP) && defined(CONFIG_FC7300_FTU1_PWM)
#  define CONFIG_FC7300_FTU1_ALREADY_USED   TRUE
#endif

#if defined(CONFIG_FC7300_FTU2_CAP) && defined(CONFIG_FC7300_FTU2_PWM)
#  define CONFIG_FC7300_FTU2_ALREADY_USED   TRUE
#endif

#if defined(CONFIG_FC7300_FTU3_CAP) && defined(CONFIG_FC7300_FTU3_PWM)
#  define CONFIG_FC7300_FTU3_ALREADY_USED   TRUE
#endif

#if defined(CONFIG_FC7300_FTU4_CAP) && defined(CONFIG_FC7300_FTU4_PWM)
#  define CONFIG_FC7300_FTU4_ALREADY_USED   TRUE
#endif

#if defined(CONFIG_FC7300_FTU5_CAP) && defined(CONFIG_FC7300_FTU5_PWM)
#  define CONFIG_FC7300_FTU5_ALREADY_USED   TRUE
#endif

#if defined(CONFIG_FC7300_FTU6_CAP) && defined(CONFIG_FC7300_FTU6_PWM)
#  define CONFIG_FC7300_FTU6_ALREADY_USED   TRUE
#endif

#if defined(CONFIG_FC7300_FTU7_CAP) && defined(CONFIG_FC7300_FTU7_PWM)
#  define CONFIG_FC7300_FTU7_ALREADY_USED   TRUE
#endif

#if defined(CONFIG_FC7300_FTU8_CAP) && defined(CONFIG_FC7300_FTU8_PWM)
#  define CONFIG_FC7300_FTU8_ALREADY_USED   TRUE
#endif

#if defined(CONFIG_FC7300_FTU9_CAP) && defined(CONFIG_FC7300_FTU9_PWM)
#  define CONFIG_FC7300_FTU9_ALREADY_USED   TRUE
#endif

#if defined(CONFIG_FC7300_FTU10_CAP) && defined(CONFIG_FC7300_FTU10_PWM)
#  define CONFIG_FC7300_FTU10_ALREADY_USED   TRUE
#endif

#if defined(CONFIG_FC7300_FTU11_CAP) && defined(CONFIG_FC7300_FTU11_PWM)
#  define CONFIG_FC7300_FTU11_ALREADY_USED   TRUE
#endif

#if defined(CONFIG_FC7300_FTU_CAP_IRQ_OVERFLOW)
#  define FC7300_CAP_OVERFLOW_USED          TRUE
#endif

#define FC7300_CAP_SETCLOCK(d,h,f,ch)        ((d)->ops->setclock(d,h,f,ch))
#define FC7300_CAP_SETCHANNEL(d,h,ch,p,t,m)  ((d)->ops->setchannel(d,h,ch,p,t,m))
#define FC7300_CAP_GETCAPTURE(d,h,ch)        ((d)->ops->getcapture(d,h,ch))
#define FC7300_CAP_SETISR(d,hnd,arg)         ((d)->ops->setisr(d,hnd,arg))
#define FC7300_CAP_ENABLEINT(d,h,ch,on)      ((d)->ops->enableint(d,h,ch,on))
#define FC7300_CAP_ACKFLAGS(d)               ((d)->ops->ackflags(d))
#define FC7300_CAP_GETINPUTLEVEL(d,ch)       ((d)->ops->getinputlevel(d, ch))
#define FC7300_CAP_REMAPCHANNELS(d,ch)       ((d)->ops->remapchannels(d, ch))
#define FC7300_CAP_READOVERFLOWFLG(d)        ((d)->ops->overflowflg(d))
#define FC7300_CAP_GETOVERFLOWVAL(d)         ((d)->ops->overflowval(d))
#define FC7300_CAP_GETPRESCALE(d)            ((d)->ops->getprescale(d))

#define MAX_ISM_CHANNEL_COUNT                16U

#define MAX_FTU_CHANNEL_COUNT                8U

#if defined(CONFIG_ARCH_CHIP_FC7300F8M)
#  define MAX_FTU_INSTANCE_COUNT             12U
#  define MAX_ISM_INSTANCE_COUNT             1U
#  define MAX_INSTANCE_COUNT   MAX_ISM_INSTANCE_COUNT + MAX_FTU_INSTANCE_COUNT
#endif

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

/* Ftu input capture mode */

enum ftu_inputcapmodetype_e
{
  FTU_INPUT_RISING_EDGE = 0U,
  FTU_INPUT_FALLING_EDGE,
  FTU_INPUT_BOTH_EDGE
};

/* Ftu input level */

enum ftu_inputleveltype_e
{
  FTU_INPUT_ERROR = -1,
  FTU_INPUT_LOW = 0,
  FTU_INPUT_HIGH,
};

/* brief Ftu clock source */

enum ftu_clksrctype_e
{
  FTU_NO_CLK = 0U,
  FTU_INTERNAL_CLK = 1U,
  FTU_EXTERNAL_CLK0 = 3U,
  FTU_EXTERNAL_CLK1 = 4U,
  FTU_EXTERNAL_CLK2 = 5U
};

/* Ftu filter prescaler */

enum ftu_filterprescalertype_e
{
  FTU_FLT_DIV_1 = 0U,
  FTU_FLT_DIV_2,
  FTU_FLT_DIV_3,
  FTU_FLT_DIV_4,
  FTU_FLT_DIV_5,
  FTU_FLT_DIV_6,
  FTU_FLT_DIV_7,
  FTU_FLT_DIV_8,
  FTU_FLT_DIV_9,
  FTU_FLT_DIV_10,
  FTU_FLT_DIV_11,
  FTU_FLT_DIV_12,
  FTU_FLT_DIV_13,
  FTU_FLT_DIV_14,
  FTU_FLT_DIV_15,
  FTU_FLT_DIV_16
};

enum ism_instancetype_e
{
  ISM_INSTANCE_0 = 0U
};

enum fpc_glitchdetecttype_e
{
  FPC_RISING_GLITCH_DETECTED  = 1U,     /* PFC Rising Glitch Detected. */
  FPC_FALLING_GLITCH_DETECTED = 2U      /* PFC Falling Glitch Detected. */
};

enum ism_fpc_edgedetectmodetype_e
{
  ISM_FPC_DETECT_NO_FILTER        = 0U,   /* No filter mode. Copy input to SOUT directly, which is LAM input. */
  ISM_FPC_DETECT_IMMI_FILTER      = 1U,   /* Immediate filter mode. */
  ISM_FPC_DETECT_DELAY_MODE       = 2U,   /* Delay mode. */
  ISM_FPC_DETECT_PRESCALER_MODE   = 3U    /* Prescaler mode. */
};

enum ism_fpc_edgedelaymodetype_e
{
  ISM_FPC_DELAY_FIXED0            = 0U,   /* Fixed delay mode. */
  ISM_FPC_DELAY_FIXED1            = 1U,   /* Fixed delay mode. */
  ISM_FPC_DELAY_SMART_DELAY0      = 2U,   /* Smart delay mode. The counter is decremented when a glitch happens. */
  ISM_FPC_DELAY_SMART_DELAY1      = 3U    /* Smart delay mode. The counter is reset when a glitch happens. */
};

enum ism_lam_inverteventwindowtype_e
{
  ISM_LAM_EVT_WIN_NON_INVERT      = 0U,   /* Event window non-inverted. */
  ISM_LAM_EVT_WIN_INVERT          = 1U    /* Event window inverted. */
};

enum ism_lam_eventwindowedgetype_e
{
  ISM_LAM_NTR_CLEAR_NTR_GATE      = 0U,   /* Neither edge used to clear the event window counter. Neither edge used to gate event generation. */
  ISM_LAM_POS_CLEAR_NTR_GATE      = 1U,   /* Positive edge used to clear the event window counter. Neither edge used to gate event generation. */
  ISM_LAM_NEG_CLEAR_NTR_GATE      = 2U,   /* Negative edge used to clear the event window counter. Neither edge used to gate event generation. */
  ISM_LAM_ETR_CLEAR_NTR_GATE      = 3U,   /* Either edge used to clear the event window counter. Neither edge used to gate event generation. */
  ISM_LAM_NTR_CLEAR_POS_GATE      = 4U,   /* Neither edge used to clear the event window counter. Positive edge used to gate event generation. */
  ISM_LAM_POS_CLEAR_POS_GATE      = 5U,   /* Positive edge used to clear the event window counter. Positive edge used to gate event generation. */
  ISM_LAM_NEG_CLEAR_POS_GATE      = 6U,   /* Negative edge used to clear the event window counter. Positive edge used to gate event generation. */
  ISM_LAM_ETR_CLEAR_POS_GATE      = 7U,   /* Either edge used to clear the event window counter. Positive edge used to gate event generation. */
  ISM_LAM_NTR_CLEAR_NEG_GATE      = 8U,   /* Neither edge used to clear the event window counter. Negative edge used to gate event generation. */
  ISM_LAM_POS_CLEAR_NEG_GATE      = 9U,   /* Positive edge used to clear the event window counter. Negative edge used to gate event generation. */
  ISM_LAM_NEG_CLEAR_NEG_GATE      = 10U,  /* Negative edge used to clear the event window counter. Negative edge used to gate event generation. */
  ISM_LAM_ETR_CLEAR_NEG_GATE      = 11U,  /* Either edge used to clear the event window counter. Negative edge used to gate event generation. */
  ISM_LAM_NTR_CLEAR_ETR_GATE      = 12U,  /* Neither edge used to clear the event window counter. Either edge used to gate event generation. */
  ISM_LAM_POS_CLEAR_ETR_GATE      = 13U,  /* Positive edge used to clear the event window counter. Either edge used to gate event generation. */
  ISM_LAM_NEG_CLEAR_ETR_GATE      = 14U,  /* Negative edge used to clear the event window counter. Either edge used to gate event generation. */
  ISM_LAM_ETR_CLEAR_ETR_GATE      = 15U   /* Either edge used to clear the event window counter. Either edge used to gate event generation. */
};

enum ism_lam_eventwindowselecttype_e
{
  ISM_LAM_EVT_WIN_SEL_REF         = 0U,   /* Event window generation is determined from the reference signal. */
  ISM_LAM_EVT_WIN_SEL_MON         = 1U    /* Event window generation is determined from the monitor signal. */
};

enum ism_lam_runmodeselecttype_e
{
  ISM_LAM_RUN_FREE                = 0U,   /* Event window generation is free-running. */
  ISM_LAM_RUN_GATED               = 1U    /* Event window generation is gated with the monitor or reference signal. */
};

enum ism_lam_monitorsourcetype_e
{
  ISM_LAM_SRC_FPC_MON             = 0U,   /* Monitor signal is sourced directly from FPC monitor channel. */
  ISM_LAM_SRC_EXORD_FPC_REF       = 1U    /* Monitor signal is EXOR'd with FPC reference channel. */
};

enum ism_lam_invertmonitortype_e
{
  ISM_LAM_FPC_MON_NON_INVERT      = 0U,   /* Do not invert the monitor signal from FPC. */
  ISM_LAM_FPC_MON_INVERT          = 1U    /* Invert the monitor signal from FPC. */
};

enum ism_lam_invertreferencetype_e
{
  ISM_LAM_FPC_REF_NON_INVERT      = 0U,   /* Do not invert the reference signal from FPC. */
  ISM_LAM_FPC_REF_INVERT          = 1U    /* Invert the reference signal from FPC. */
};

/* Capture Device Structure */

struct fc7300_cap_dev_s
{
  struct fc7300_cap_ops_s *ops;
};

/* Cap channel configuration */

struct fc7300_capchan_s
{
  uint8_t                         channel;     /* Timer input channel  */
  uint32_t                        pincfg;      /* Input pin configuration */
  enum ftu_inputcapmodetype_e     mode;        /* The ftu capture mode */
  uint32_t                        freq;        /* Result pwm frequence */
  uint8_t                         duty;        /* Result pwm duty */
};

/* Capture Operations */

struct fc7300_cap_ops_s
{
  int  (*setclock)(struct fc7300_cap_dev_s *dev, uint8_t hwunit,
                   uint32_t freq, uint8_t channel);
  int  (*setchannel)(struct fc7300_cap_dev_s *dev, uint8_t hwunit,
                     uint8_t channel, uint32_t pincfg, uint8_t trgsel,
                     uint8_t mode);
  uint32_t (*getcapture)(struct fc7300_cap_dev_s *dev, uint8_t hwunit,
                         uint8_t channel);
  void  (*setisr)(struct fc7300_cap_dev_s *dev, xcpt_t handler, void *arg);
  int (*enableint)(struct fc7300_cap_dev_s *dev, uint8_t hwunit,
                   uint8_t channel, bool on);
  uint8_t (*ackflags)(struct fc7300_cap_dev_s *dev);
  enum ftu_inputleveltype_e (*getinputlevel)(struct fc7300_cap_dev_s *dev,
                                             uint8_t channel);
  uint8_t (*remapchannels)(struct fc7300_cap_dev_s *dev, uint8_t channel_in);
  bool (*overflowflg)(struct fc7300_cap_dev_s *dev);
  uint32_t (*overflowval)(struct fc7300_cap_dev_s *dev);
  uint32_t (*getprescale)(struct fc7300_cap_dev_s *dev);
};

typedef CODE void (*cap_callback_t)(void);

struct fc7300_callbackmap_s
{
  uint8_t         channelid;
  cap_callback_t  callback;
};

/* Capture Measurements */

struct fc7300_cap_msmt_s
{
  /* Array for saving the signal measurement start time */

  uint32_t                     sigmsmtstart[MAX_FTU_CHANNEL_COUNT];

  /* Array for saving the signal measurement active time */

  uint32_t                     sigmsmtactive[MAX_FTU_CHANNEL_COUNT];

  /* Array for saving the period */

  uint32_t                     period[MAX_FTU_CHANNEL_COUNT];

  /* Array for saving the pulse width of active time */

  uint32_t                     activewidth[MAX_FTU_CHANNEL_COUNT];

  /* Array for saving the freq */

  uint32_t                     freq[MAX_FTU_CHANNEL_COUNT];

  /* Array for saving signal measurement sequence of channel */

  uint8_t                      signalmsmtseq[MAX_FTU_CHANNEL_COUNT];

  /* Array for saving the duty */

  uint8_t                      duty[MAX_FTU_CHANNEL_COUNT];

  /* Array for saving the signal measurement active level */

  enum ftu_inputleveltype_e    sigmsmtactivelevel[MAX_FTU_CHANNEL_COUNT];

  /* Array for saving the edge counter */

  uint32_t edgecnt[MAX_FTU_CHANNEL_COUNT];

  /* Array for saving the last edge, 0 means falling edge */

  uint8_t edgedirection[MAX_FTU_CHANNEL_COUNT];

  /* Array for saving the counter when edge occured */

  uint32_t timestamp[MAX_FTU_CHANNEL_COUNT];

#ifdef FC7300_CAP_OVERFLOW_USED

  /* Array for saving maximum count of channel */

  uint32_t maxofcnt[MAX_FTU_CHANNEL_COUNT];

  /* Array for saving overflow count of channels */

  uint32_t ofcnt[MAX_FTU_CHANNEL_COUNT];

  /* Array for saving overflow extend(only for DUTY_CYCLE) count of channel */

  uint32_t ofexcnt[MAX_FTU_CHANNEL_COUNT];

  /* Array for saving active overflow count of channels */

  uint32_t activeofcnt[MAX_FTU_CHANNEL_COUNT];

  /* Array for saving period overflow count of channels */

  uint32_t periodofcnt[MAX_FTU_CHANNEL_COUNT];

#endif
};

/* This structure provides the private representation of the "lower-half"
 * driver state structure.  This structure must be cast-compatible with the
 * cap_lowerhalf_s structure.
 */

struct fc7300_cap_lowerhalf_s
{
  const struct cap_ops_s          *ops;    /* Lower half operations */
  struct fc7300_cap_dev_s         *cap;    /* fc7300 capture driver */
  bool                            started; /* True: Timer has been started */
  uint8_t                         channel; /* Timer input channel  */
  uint32_t                        pincfg;  /* Input pin configuration */
  uint8_t                         mode;    /* The ftu capture mode */
  uint32_t                        freq;    /* Result pwm frequence */
  uint8_t                         duty;    /* Result pwm duty */
  uint32_t                        clock;   /* Timer clock frequence */
  uint8_t                         hwunit;  /* instance number, ism0~12,ftu0~11 */
  uint8_t                         trgsel;  /* trgsel source */
  struct fc7300_callbackmap_s     *cbtable; /* usercallback */
  void (*clear)(struct fc7300_cap_lowerhalf_s *lower); /* clear freq and duty */
};

/****************************************************************************
 * Public Data
 ****************************************************************************/

#ifdef FC7300_CAP_OVERFLOW_USED

/* Flag whether the Ftu instance Overflow interrupt enabled by capture */

extern uint8_t g_capoverflowflag[MAX_FTU_INSTANCE_COUNT];

#endif

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

#if defined(CONFIG_FC7300_FTU0_CAP)
void fc7300_init_ftu0_callback(uint8_t ch, void *cbfunc);
#endif

#if defined(CONFIG_FC7300_FTU1_CAP)
void fc7300_init_ftu1_callback(uint8_t ch, void *cbfunc);
#endif

#if defined(CONFIG_FC7300_FTU2_CAP)
void fc7300_init_ftu2_callback(uint8_t ch, void *cbfunc);
#endif

#if defined(CONFIG_FC7300_FTU3_CAP)
void fc7300_init_ftu3_callback(uint8_t ch, void *cbfunc);
#endif

#if defined(CONFIG_FC7300_FTU4_CAP)
void fc7300_init_ftu4_callback(uint8_t ch, void *cbfunc);
#endif

#if defined(CONFIG_FC7300_FTU5_CAP)
void fc7300_init_ftu5_callback(uint8_t ch, void *cbfunc);
#endif

#if defined(CONFIG_FC7300_FTU6_CAP)
void fc7300_init_ftu6_callback(uint8_t ch, void *cbfunc);
#endif

#if defined(CONFIG_FC7300_FTU7_CAP)
void fc7300_init_ftu7_callback(uint8_t ch, void *cbfunc);
#endif

#if defined(CONFIG_FC7300_FTU8_CAP)
void fc7300_init_ftu8_callback(uint8_t ch, void *cbfunc);
#endif

#if defined(CONFIG_FC7300_FTU9_CAP)
void fc7300_init_ftu9_callback(uint8_t ch, void *cbfunc);
#endif

#if defined(CONFIG_FC7300_FTU10_CAP)
void fc7300_init_ftu10_callback(uint8_t ch, void *cbfunc);
#endif

#if defined(CONFIG_FC7300_FTU11_CAP)
void fc7300_init_ftu11_callback(uint8_t ch, void *cbfunc);
#endif

#if defined(CONFIG_FC7300_FTU0_CAP) || defined(CONFIG_FC7300_FTU1_CAP) || \
    defined(CONFIG_FC7300_FTU2_CAP) || defined(CONFIG_FC7300_FTU3_CAP) || \
    defined(CONFIG_FC7300_FTU4_CAP) || defined(CONFIG_FC7300_FTU5_CAP) || \
    defined(CONFIG_FC7300_FTU6_CAP) || defined(CONFIG_FC7300_FTU7_CAP) || \
    defined(CONFIG_FC7300_FTU8_CAP) || defined(CONFIG_FC7300_FTU9_CAP) || \
    defined(CONFIG_FC7300_FTU10_CAP) || defined(CONFIG_FC7300_FTU11_CAP) || \
    defined(CONFIG_FC7300_ISM0_CAP)

/****************************************************************************
 * Name: fc7300_cap_initpriv
 *
 * Description:
 *   Power-up timer and get its structure.
 *
 * Input Parameters:
 *   channel - A number identifying the timer channel use.
 *     The number of valid timer channels varies with the FC7300 MCU and MCU
 *     family but is somewhere in the range of {0,..,7}.
 *
 * Returned Value:
 *   On success, a pointer to the FC7300 capture private driver returned.
 *   NULL is returned on any failure.
 *
 ****************************************************************************/

struct fc7300_cap_dev_s *fc7300_cap_initpriv(uint8_t channel);

/****************************************************************************
 * Name: fc7300_cap_initialize
 *
 * Description:
 *   Initialize one timer for use with the upper_level capture driver.
 *
 * Input Parameters:
 *   timer - A number identifying the timer use.  The number of valid timer
 *     IDs varies with the FC7300 MCU and MCU family but is somewhere in
 *     the range of {0,..,7}.
 *
 * Returned Value:
 *   On success, a pointer to the FC7300 lower half capture driver returned.
 *   NULL is returned on any failure.
 *
 ****************************************************************************/

struct cap_lowerhalf_s *fc7300_cap_initialize(uint8_t channel,
                                        struct fc7300_callbackmap_s *cbtable);

/****************************************************************************
 * Name: cap_getsignalduration
 *
 * Description:
 *   Calculate duration between current and start.
 *
 * Input Parameters:
 *   dev - A pointer to device.
 *   current - current ticks.
 *   start - beginning ticks.
 *
 * Returned Value:
 *   Always success, duration returned.
 *
 ****************************************************************************/

#ifdef FC7300_CAP_OVERFLOW_USED
uint32_t cap_getsignalduration(struct fc7300_cap_dev_s *dev, uint32_t current,
                               uint32_t start, uint32_t *ofcnt);
#else
uint32_t cap_getsignalduration(struct fc7300_cap_dev_s *dev, uint32_t current,
                               uint32_t start);
#endif
/****************************************************************************
 * Name: fc7300_capture_setup
 *
 * Description:
 *   Initialize and register the pwm capture driver.
 *
 * Input parameters:
 *   void
 *
 * Returned Value:
 *   Zero (OK) on success; a negated errno value on failure.
 *
 ****************************************************************************/

int fc7300_capture_setup(void);

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

void fc7300_cap_clearfreqandduty(struct fc7300_cap_lowerhalf_s *lower);

#endif /* CONFIG_FC7300_FTUx_CAP */

#undef EXTERN
#if defined(__cplusplus)
}
#endif

#endif /* __ASSEMBLY__ */
#endif /* __ARCH_ARM_SRC_FC7300_FC7300_CAPTURE_H */
