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
#include <syslog.h>

#include <nuttx/irq.h>
#include <nuttx/arch.h>
#include <nuttx/timers/rtc.h>
#include <nuttx/timers/arch_rtc.h>
#include <nuttx/arch.h>
#include <arch/board/board.h>

#include "arm_internal.h"
#include "chip.h"

#include "fc7300_clockconfig.h"
#include "fc7300_rtc.h"
#include "hardware/fc7300_rtc.h"
#include "hardware/fc7300_csc.h"

/* This module then only compiles if there is at least one enabled timer
 * intended for use with the rtc upper half driver.
 */

#if defined(CONFIG_FC7300_RTC)

/****************************************************************************
 * Public Data
 ****************************************************************************/

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Private Types
 ****************************************************************************/

#ifdef CONFIG_RTC_ALARM
struct fc7300_cbinfo_s
{
  rtc_alarm_callback_t cb;  /* Callback when the alarm expires */
  void *priv;               /* Private argument to accompany callback */
};
#endif

/* RTC Device Private Data */

struct fc7300_rtc_priv_s
{
  const struct rtc_ops_s   *ops;     /* RTC operations */
  uint32_t                  base;    /* The base address of the rtc */

  mutex_t devlock;      /* Threads can only exclusively access the RTC */

  bool rtc_have_set_time;
#ifdef CONFIG_RTC_ALARM
  /* Alarm callback information */

  struct fc7300_cbinfo_s cbinfo;
  uint32_t  irq;                  /* Interrupt ID */
  uint32_t  irq_priority;         /* Interrupt priority */
#endif

#ifdef CONFIG_RTC_IOCTL
  bool rtc_alarm_flag;            /* Alarm flag */
#endif

#ifdef CONFIG_RTC_PERIODIC
  /* Periodic wakeup information */

  bool periodic_enable;
  struct lower_setperiodic_s periodic;
#endif
};

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

/* Register access */

static inline uint32_t rtc_getreg(struct fc7300_rtc_priv_s *priv, int offset);
static inline void rtc_putreg(struct fc7300_rtc_priv_s *priv, int offset,
                       uint32_t value);

/* RTC Driver Methods */

static int fc7300_rdtime(struct rtc_lowerhalf_s *lower,
                        struct rtc_time *rtctime);
static int fc7300_settime(struct rtc_lowerhalf_s *lower,
                         const struct rtc_time *rtctime);
static bool fc7300_havesettime(struct rtc_lowerhalf_s *lower);

#ifdef CONFIG_RTC_ALARM
static int fc7300_setalarm(struct rtc_lowerhalf_s *lower,
                          const struct lower_setalarm_s *alarminfo);
static int fc7300_setrelative(struct rtc_lowerhalf_s *lower,
                            const struct lower_setrelative_s *alarminfo);
static int fc7300_cancelalarm(struct rtc_lowerhalf_s *lower,
                             int alarmid);
static int fc7300_rdalarm(struct rtc_lowerhalf_s *lower,
                         struct lower_rdalarm_s *alarminfo);
#endif

#ifdef CONFIG_RTC_IOCTL
static int fc7300_rtc_ioctl(struct rtc_lowerhalf_s *lower, int cmd,
                    unsigned long arg);
#endif

#ifdef CONFIG_RTC_PERIODIC
static int fc7300_setperiodic(struct rtc_lowerhalf_s *lower,
                            const struct lower_setperiodic_s *alarminfo);
static int fc7300_cancelperiodic(struct rtc_lowerhalf_s *lower, int id);
#endif

static time_t fc7300_rtc_readseconds(struct rtc_lowerhalf_s *lower);

/****************************************************************************
 * Private Data
 ****************************************************************************/

/* This is the list of lower half rtc driver methods used by the upper half
 * driver.
 */

static const struct rtc_ops_s g_rtc_ops =
{
  .rdtime      = fc7300_rdtime,
  .settime     = fc7300_settime,
  .havesettime = fc7300_havesettime,
#ifdef CONFIG_RTC_ALARM
  .setalarm    = fc7300_setalarm,
  .setrelative = fc7300_setrelative,
  .cancelalarm = fc7300_cancelalarm,
  .rdalarm     = fc7300_rdalarm,
#endif
#ifdef CONFIG_RTC_IOCTL
  .ioctl       = fc7300_rtc_ioctl,
#endif
#ifdef CONFIG_RTC_PERIODIC
  .setperiodic    = fc7300_setperiodic,
  .cancelperiodic = fc7300_cancelperiodic,
#endif
};

static struct fc7300_rtc_priv_s g_rtc_lowerhalf =
{
  .ops               = &g_rtc_ops,
  .base              = FC7300_RTC_BASE,
  .devlock           = NXMUTEX_INITIALIZER,
  .rtc_have_set_time = false,
#ifdef CONFIG_RTC_ALARM
  .irq               = FC7300_IRQ_RTC,
  .irq_priority      = NVIC_RTC_PRIORITY_DEFAULT,
#endif
#ifdef CONFIG_RTC_IOCTL
  .rtc_alarm_flag    = false,
#endif
#ifdef CONFIG_RTC_PERIODIC
  .periodic_enable   = false,
#endif
};

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: rtc_getreg
 *
 * Description:
 *   Get a 32-bit register value by offset
 *
 * Input Parameters:
 *   priv   - rtc device private data
 *   offset - Register offset with respect to the base address of the Rtc
 *            peripheral
 *
 * Returned Value:
 *   The 32-bit value retrieved from the register
 *
 ****************************************************************************/

static inline uint32_t rtc_getreg(struct fc7300_rtc_priv_s *priv, int offset)
{
  return getreg32(priv->base + offset);
}

/****************************************************************************
 * Name: rtc_putreg
 *
 * Description:
 *   Write a value to an ADC register.
 *
 * Input Parameters:
 *   priv   - A reference to the ADC block status
 *   offset - The offset to the register to write to
 *   value  - The value to write to the register
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/

static inline void rtc_putreg(struct fc7300_rtc_priv_s *priv, int offset,
                       uint32_t value)
{
  putreg32(value, priv->base + offset);
}

static time_t fc7300_rtc_readseconds(struct rtc_lowerhalf_s *lower)
{
  time_t seconds;
  time_t pre_seconds;
  struct fc7300_rtc_priv_s *priv = (struct fc7300_rtc_priv_s *)lower;

  pre_seconds = rtc_getreg(priv, FC7300_RTC_SR_OFFSET);

  do
    {
      seconds = rtc_getreg(priv, FC7300_RTC_SR_OFFSET);
      if (seconds == pre_seconds)
          break;
      else
          pre_seconds = seconds;
    } while(1);

  return seconds;
}

/****************************************************************************
 * Name: fc7300_rdtime
 *
 * Description:
 *   Implements the rdtime() method of the RTC driver interface
 *
 * Input Parameters:
 *   lower   - A reference to RTC lower half driver state structure
 *   rcttime - The location in which to return the current RTC time.
 *
 * Returned Value:
 *   Zero (OK) is returned on success; a negated errno value is returned
 *   on any failure.
 *
 ****************************************************************************/

static int fc7300_rdtime(struct rtc_lowerhalf_s *lower,
                        struct rtc_time *rtctime)
{
  time_t seconds;

  struct fc7300_rtc_priv_s *priv = (struct fc7300_rtc_priv_s *)lower;
  DEBUGASSERT(priv != NULL && rtctime != NULL);

  /* Get seconds register value */

  seconds = fc7300_rtc_readseconds(lower);

  gmtime_r(&seconds, (struct tm *)rtctime);

  return OK;
}

/****************************************************************************
 * Name: fc7300_settime
 *
 * Description:
 *   Implements the settime() method of the RTC driver interface
 *
 * Input Parameters:
 *   lower   - A reference to RTC lower half driver state structure
 *   rcttime - The new time to set
 *
 * Returned Value:
 *   Zero (OK) is returned on success; a negated errno value is returned
 *   on any failure.
 *
 ****************************************************************************/

static int fc7300_settime(struct rtc_lowerhalf_s *lower,
                         const struct rtc_time *rtctime)
{
  uint32_t seconds;
  uint32_t regval;

  struct fc7300_rtc_priv_s *priv = (struct fc7300_rtc_priv_s *)lower;
  DEBUGASSERT(priv != NULL && rtctime != NULL);

  /* Get seconds register value */

  seconds = timegm((struct tm *)rtctime);

  /* disable the rtc clock */

  regval = rtc_getreg(priv, FC7300_RTC_STR_OFFSET);
  regval &= ~RTC_STR_TCE_MASK;
  rtc_putreg(priv, FC7300_RTC_STR_OFFSET, regval);

  /* Set Count Value */

  rtc_putreg(priv, FC7300_RTC_PR_OFFSET, 0U);
  rtc_putreg(priv, FC7300_RTC_SR_OFFSET, seconds);

  /* enable the rtc clock */

  regval = rtc_getreg(priv, FC7300_RTC_STR_OFFSET);
  regval |= RTC_STR_TCE_MASK;
  rtc_putreg(priv, FC7300_RTC_STR_OFFSET, regval);

  priv->rtc_have_set_time = true;

  return OK;
}

/****************************************************************************
 * Name: fc7300_havesettime
 *
 * Description:
 *   Implements the havesettime() method of the RTC driver interface
 *
 * Input Parameters:
 *   lower   - A reference to RTC lower half driver state structure
 *
 * Returned Value:
 *   Returns true if RTC date-time have been previously set.
 *
 ****************************************************************************/

static bool fc7300_havesettime(struct rtc_lowerhalf_s *lower)
{
  struct fc7300_rtc_priv_s *priv = (struct fc7300_rtc_priv_s *)lower;

  return priv->rtc_have_set_time;
}

#ifdef CONFIG_RTC_ALARM

/****************************************************************************
 * Name: fc7300_setalarm
 *
 * Description:
 *   Set a new alarm.  This function implements the setalarm() method of the
 *   RTC driver interface
 *
 * Input Parameters:
 *   lower - A reference to RTC lower half driver state structure
 *   alarminfo - Provided information needed to set the alarm
 *
 * Returned Value:
 *   Zero (OK) is returned on success; a negated errno value is returned
 *   on any failure.
 *
 ****************************************************************************/

static int fc7300_setalarm(struct rtc_lowerhalf_s *lower,
                          const struct lower_setalarm_s *alarminfo)
{
  struct fc7300_rtc_priv_s *priv;
  struct fc7300_cbinfo_s *cbinfo;
  uint32_t alarmvalue;

  uint32_t regval;
  int ret;

  DEBUGASSERT(lower != NULL && alarminfo != NULL);
  priv = (struct fc7300_rtc_priv_s *)lower;

  ret = nxmutex_lock(&priv->devlock);
  if (ret < 0)
    {
      return ret;
    }

  /* Convert the RTC time to a timespec */

  alarmvalue = mktime((struct tm *)&alarminfo->time) - 1U;

  /* Remember the callback information */

  cbinfo            = &priv->cbinfo;
  cbinfo->cb        = alarminfo->cb;
  cbinfo->priv      = alarminfo->priv;

  /* Update alarm value and set TAR register */

  rtc_putreg(priv, FC7300_RTC_AR_OFFSET, alarmvalue);

  /* enable timer alarm interrupt */

  regval = rtc_getreg(priv, FC7300_RTC_IER_TR_OFFSET);
  regval |= RTC_IER_TAIE_MASK;
  rtc_putreg(priv, FC7300_RTC_IER_TR_OFFSET, regval);

  nxmutex_unlock(&priv->devlock);

  return OK;
}

/****************************************************************************
 * Name: fc7300_setrelative
 *
 * Description:
 *   Set a new alarm relative to the current time.  This function implements
 *   the setrelative() method of the RTC driver interface
 *
 * Input Parameters:
 *   lower - A reference to RTC lower half driver state structure
 *   alarminfo - Provided information needed to set the alarm
 *
 * Returned Value:
 *   Zero (OK) is returned on success; a negated errno value is returned
 *   on any failure.
 *
 ****************************************************************************/

static int fc7300_setrelative(struct rtc_lowerhalf_s *lower,
                             const struct lower_setrelative_s *alarminfo)
{
  struct fc7300_rtc_priv_s *priv;
  struct fc7300_cbinfo_s *cbinfo;
  uint32_t alarmvalue;
  uint32_t regval;
  time_t seconds;
  int ret;

  DEBUGASSERT(lower != NULL && alarminfo != NULL);
  priv = (struct fc7300_rtc_priv_s *)lower;

  ret = nxmutex_lock(&priv->devlock);

  if (ret < 0)
    {
      return ret;
    }

  /* Convert the RTC time to a timespec */

  alarmvalue = (uint32_t)(alarminfo->reltime);

  /* Remember the callback information */

  cbinfo            = &priv->cbinfo;
  cbinfo->cb        = alarminfo->cb;
  cbinfo->priv      = alarminfo->priv;

  /* Update alarm value and set TAR register */

  seconds = fc7300_rtc_readseconds(lower);
  alarmvalue += seconds - 1;
  rtc_putreg(priv, FC7300_RTC_AR_OFFSET, alarmvalue);

  /* enable timer alarm interrupt */

  regval = rtc_getreg(priv, FC7300_RTC_IER_TR_OFFSET);
  regval |= RTC_IER_TAIE_MASK;
  rtc_putreg(priv, FC7300_RTC_IER_TR_OFFSET, regval);

  nxmutex_unlock(&priv->devlock);

  return OK;
}

/****************************************************************************
 * Name: fc7300_cancelalarm
 *
 * Description:
 *   Cancel the current alarm.  This function implements the cancelalarm()
 *   method of the RTC driver interface
 *
 * Input Parameters:
 *   lower - A reference to RTC lower half driver state structure
 *   alarminfo - Provided information needed to set the alarm
 *
 * Returned Value:
 *   Zero (OK) is returned on success; a negated errno value is returned
 *   on any failure.
 *
 ****************************************************************************/

static int fc7300_cancelalarm(struct rtc_lowerhalf_s *lower, int alarmid)
{
  struct fc7300_rtc_priv_s *priv;
  struct fc7300_cbinfo_s *cbinfo;
  uint32_t regval;
  DEBUGASSERT(lower != NULL);
  priv = (struct fc7300_rtc_priv_s *)lower;

  int ret = nxmutex_lock(&priv->devlock);
  if (ret < 0)
    {
      return ret;
    }

  /* Nullify callback information to reduce window for race conditions */

  cbinfo       = &priv->cbinfo;
  cbinfo->cb   = NULL;
  cbinfo->priv = NULL;

  /* Disable rtc alarm interrupt */

  regval = rtc_getreg(priv, FC7300_RTC_IER_TR_OFFSET);
  regval &= ~RTC_IER_TAIE_MASK;
  rtc_putreg(priv, FC7300_RTC_IER_TR_OFFSET, regval);

  nxmutex_unlock(&priv->devlock);
  return OK;
}

/****************************************************************************
 * Name: fc7300_rdalarm
 *
 * Description:
 *   Query the RTC alarm.
 *
 * Input Parameters:
 *   lower - A reference to RTC lower half driver state structure
 *   alarminfo - Provided information needed to query the alarm
 *
 * Returned Value:
 *   Zero (OK) is returned on success; a negated errno value is returned
 *   on any failure.
 *
 ****************************************************************************/

static int fc7300_rdalarm(struct rtc_lowerhalf_s *lower,
                         struct lower_rdalarm_s *alarminfo)
{
  struct fc7300_rtc_priv_s *priv;
  time_t alarm_value;
  DEBUGASSERT(lower != NULL && alarminfo != NULL && alarminfo->time != NULL);
  priv = (struct fc7300_rtc_priv_s *)lower;

  /* Get alarm value */

  alarm_value = rtc_getreg(priv, FC7300_RTC_AR_OFFSET) + 1U;

  gmtime_r((const time_t *)&alarm_value, (struct tm *)(alarminfo->time));

  return OK;
}
#endif

#ifdef CONFIG_RTC_IOCTL

/****************************************************************************
 * Name: fc7300_rtc_ioctl
 *
 * Description:
 *   This function is called by the RTC driver to perform an IOCTL
 *   operation on the RTC device.
 *
 * Input Parameters:
 *   lower - A reference to RTC lower half driver state structure
 *   cmd - The IOCTL command code
 *   arg - The IOCTL argument
 *
 * Returned Value:
 *   Zero (OK) is returned on success; a negated errno value is returned
 *   on any failure.
 *
 ****************************************************************************/
static int fc7300_rtc_ioctl(struct rtc_lowerhalf_s *lower, int cmd,
                    unsigned long arg)
{
  struct fc7300_rtc_priv_s *priv;
  DEBUGASSERT(lower != NULL);
  priv = (struct fc7300_rtc_priv_s *)lower;

  switch (cmd)
    {
    /* RTC_RD_ALARM_FLAG returns the flag of alarm.
     *
     * Argument: A writeable reference to a boolean value.
     */

    case RTC_RD_ALARM_FLAG:
      {
        FAR bool *have_trigger = (FAR bool *)((uintptr_t)arg);

        *have_trigger = priv->rtc_alarm_flag;
      }
      return OK;

    default:
      return -ENOSYS;
    }
}
#endif

#ifdef CONFIG_RTC_PERIODIC

/****************************************************************************
 * Name: fc7300_setperiodic
 *
 * Description:
 *   Set a new periodic wakeup relative to the current time, with a given
 *   period. This function implements the setperiodic() method of the RTC
 *   driver interface
 *
 * Input Parameters:
 *   lower - A reference to RTC lower half driver state structure
 *   alarminfo - Provided information needed to set the wakeup activity
 *
 * Returned Value:
 *   Zero (OK) is returned on success; a negated errno value is returned
 *   on any failure.
 *
 ****************************************************************************/

static int fc7300_setperiodic(struct rtc_lowerhalf_s *lower,
                             const struct lower_setperiodic_s *alarminfo)
{
  struct fc7300_rtc_priv_s *priv;
  irqstate_t flags;
  uint32_t alarmvalue;
  uint32_t regval;

  DEBUGASSERT(lower != NULL && alarminfo != NULL);
  priv = (struct fc7300_rtc_priv_s *)lower;

  flags = enter_critical_section();
  priv->periodic = *alarminfo;
  priv->periodic_enable = true;
  // memcpy(&priv->periodic, alarminfo, sizeof(struct lower_setperiodic_s));
  leave_critical_section(flags);

  alarmvalue = alarminfo->period.tv_sec + *(volatile uint32_t *)FC7300_RTC_SR - 1;

  /* Update alarm value and set TAR register */

  rtc_putreg(priv, FC7300_RTC_AR_OFFSET, alarmvalue);

  /* enable timer alarm interrupt */

  regval = rtc_getreg(priv, FC7300_RTC_IER_TR_OFFSET);
  regval |= RTC_IER_TAIE_MASK;
  rtc_putreg(priv, FC7300_RTC_IER_TR_OFFSET, regval);

  return OK;
}

/****************************************************************************
 * Name: fc7300_cancelperiodic
 *
 * Description:
 *   Cancel the current periodic wakeup activity.  This function implements
 *   the cancelperiodic() method of the RTC driver interface
 *
 * Input Parameters:
 *   lower - A reference to RTC lower half driver state structure
 *
 * Returned Value:
 *   Zero (OK) is returned on success; a negated errno value is returned
 *   on any failure.
 *
 ****************************************************************************/

static int fc7300_cancelperiodic(struct rtc_lowerhalf_s *lower, int id)
{
  struct fc7300_rtc_priv_s *priv;
  irqstate_t flags;
  uint32_t regval;

  DEBUGASSERT(lower != NULL);
  priv = (struct fc7300_rtc_priv_s *)lower;

  DEBUGASSERT(id == 0);

  flags = enter_critical_section();
  priv->periodic_enable = false;
  leave_critical_section(flags);

  regval = rtc_getreg(priv, FC7300_RTC_IER_TR_OFFSET);
  regval &= ~RTC_IER_TAIE_MASK;
  rtc_putreg(priv, FC7300_RTC_IER_TR_OFFSET, regval);

  return OK;
}

#endif

#ifdef CONFIG_RTC_ALARM

/****************************************************************************
 * Name: fc7300_rtc_isr_process
 *
 * Description:
 *  Common Interrupt Service Routine
 *
 ****************************************************************************/

static int fc7300_rtc_isr_process(struct fc7300_rtc_priv_s *priv)
{
  uint32_t regval;
  int32_t status = (rtc_getreg(priv, FC7300_RTC_STR_OFFSET)
                   & RTC_STR_TAF_MASK);
  uint32_t alarm_irq_en = (rtc_getreg(priv, FC7300_RTC_IER_TR_OFFSET)
                                    & RTC_IER_TAIE_MASK);

  if (status & alarm_irq_en)
    {
      /* Disable rtc alarm interrupt */

      regval = rtc_getreg(priv, FC7300_RTC_IER_TR_OFFSET);
      regval &= ~RTC_IER_TAIE_MASK;
      rtc_putreg(priv, FC7300_RTC_IER_TR_OFFSET, regval);

#ifdef CONFIG_RTC_PERIODIC
      if (priv->periodic_enable)
      {
        uint32_t alarmvalue;

        alarmvalue = priv->periodic.period.tv_sec + *(volatile uint32_t *)FC7300_RTC_SR - 1U;

        rtc_putreg(priv, FC7300_RTC_AR_OFFSET, alarmvalue);

        regval = rtc_getreg(priv, FC7300_RTC_IER_TR_OFFSET);
        regval |= RTC_IER_TAIE_MASK;
        rtc_putreg(priv, FC7300_RTC_IER_TR_OFFSET, regval);

        if (priv->periodic.cb)
          {
            priv->periodic.cb(priv->periodic.priv, priv->periodic.id);
          }
      }
#endif

      rtc_alarm_callback_t cb = (rtc_alarm_callback_t)priv->cbinfo.cb;
      void *p_arg             = (void *)(priv->cbinfo.priv);
      priv->cbinfo.cb         = NULL;
      priv->cbinfo.priv       = NULL;

      if (cb != NULL)
        {
          cb(p_arg, 0);
        }
    }

  return OK;
}

#ifdef CONFIG_FC7300_RTC_IRQ_ATTACH_THREAD

/****************************************************************************
 * Name: fc7300_rtc_isr_handle
 *
 * Description:
 *   Disable rtc interrupt.
 *
 * Input Parameters:
 *
 * Returned Value: void
 *
 ****************************************************************************/

static int fc7300_rtc_isr_handle(int irq, void *regs, void *arg)
{
  up_disable_irq(irq);
  return IRQ_WAKE_THREAD;
}
#endif

/****************************************************************************
 * Name: fc7300_rtc_isr
 *
 * Description:
 *   Common RTC interrupt service routine
 *
 ****************************************************************************/

static int fc7300_rtc_isr(int irq, void *context, void *arg)
{
  struct fc7300_rtc_priv_s *priv = (struct fc7300_rtc_priv_s *)arg;

  DEBUGASSERT(priv != NULL);

  fc7300_rtc_isr_process(priv);

#ifdef CONFIG_FC7300_RTC_IRQ_ATTACH_THREAD
  fc7300_clearpendingisr(irq);
  up_enable_irq(irq);
#endif

  return OK;
}
#endif

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: up_rtc_initialize
 *
 * Description:
 *   Initialize the hardware RTC per the selected configuration.  This
 *   function is called once during the OS initialization sequence
 *
 * Input Parameters:
 *   None
 *
 * Returned Value:
 *   Zero (OK) on success; a negated errno on failure
 *
 ****************************************************************************/

int up_rtc_initialize(void)
{
  uint32_t regval;
  bool sync = true;
  struct rtc_lowerhalf_s *rtc = (struct rtc_lowerhalf_s *)&g_rtc_lowerhalf;
  syslog(LOG_INFO, "Up RTc Initialize\n");

  /* Change Clock to SOSC */

  regval = *(volatile uint32_t *)FC7300_CSC0_AONCLKSR;
  regval &= ~CSC0_AONCLKSR_RTCCLKSEL_MASK;
  regval |= CSC0_AONCLKSR_RTCCLKSEL(2);
  *(volatile uint32_t *)FC7300_CSC0_AONCLKSR = regval;

  time_t seconds = fc7300_rtc_readseconds(rtc);

  if(seconds == 0)
    {
      /* Disable the time countr  */

        regval = rtc_getreg(&g_rtc_lowerhalf, FC7300_RTC_STR_OFFSET);
        regval &= ~RTC_STR_TCE_MASK;
        rtc_putreg(&g_rtc_lowerhalf, FC7300_RTC_STR_OFFSET, regval);

        /* Reset all RTC registers except for the CR[SWRST] */

        regval = rtc_getreg(&g_rtc_lowerhalf, FC7300_RTC_CR_OFFSET);
        regval |= RTC_CR_SWRST_MASK;
        rtc_putreg(&g_rtc_lowerhalf, FC7300_RTC_CR_OFFSET, regval);

        /* Clear CR[SWRST] */

        regval = rtc_getreg(&g_rtc_lowerhalf, FC7300_RTC_CR_OFFSET);
        regval &= ~RTC_CR_SWRST_MASK;
        rtc_putreg(&g_rtc_lowerhalf, FC7300_RTC_CR_OFFSET, regval);

        /* Disable all interrupts */

        regval = rtc_getreg(&g_rtc_lowerhalf, FC7300_RTC_IER_TR_OFFSET);
        regval &= ~(RTC_IER_TSIC_MASK | RTC_IER_TSIE_MASK \
                  | RTC_IER_TAIE_MASK | RTC_IER_TOIE_MASK);
        rtc_putreg(&g_rtc_lowerhalf, FC7300_RTC_IER_TR_OFFSET, regval);

        /* Set compensation */

        regval = rtc_getreg(&g_rtc_lowerhalf, FC7300_RTC_COMPR_OFFSET);
        regval &= ~RTC_COMPR_TCR_MASK;
        regval |= RTC_COMPR_TCR(0U);
        rtc_putreg(&g_rtc_lowerhalf, FC7300_RTC_COMPR_OFFSET, regval);

        /* Enable the time countr */

        regval = rtc_getreg(&g_rtc_lowerhalf, FC7300_RTC_STR_OFFSET);
        regval |= RTC_STR_TCE_MASK;
        rtc_putreg(&g_rtc_lowerhalf, FC7300_RTC_STR_OFFSET, regval);
    }

  /* read the alarm flag */

#ifdef CONFIG_RTC_IOCTL
  regval = rtc_getreg(&g_rtc_lowerhalf, FC7300_RTC_STR_OFFSET);
  g_rtc_lowerhalf.rtc_alarm_flag = regval & RTC_STR_TAF_MASK;
#endif

#ifdef CONFIG_RTC_ALARM

  /* Attach ISRs */

#ifdef CONFIG_FC7300_RTC_IRQ_ATTACH_THREAD
  irq_attach_wqueue(g_rtc_lowerhalf.irq,
                    fc7300_rtc_isr_handle, fc7300_rtc_isr,
                    &g_rtc_lowerhalf, g_isr_cfg[RTC_ISR_THREAD]);
#else
  irq_attach(g_rtc_lowerhalf.irq, fc7300_rtc_isr, &g_rtc_lowerhalf);
#endif

#ifdef CONFIG_ARCH_IRQPRIO
  up_prioritize_irq(g_rtc_lowerhalf.irq, g_rtc_lowerhalf.irq_priority);
#endif

  up_enable_irq(g_rtc_lowerhalf.irq);
#endif

  up_rtc_set_lowerhalf(rtc, sync);

  return rtc_initialize(0, rtc);
}

#endif /* CONFIG_FC7300_RTC */
