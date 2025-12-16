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

#include <debug.h>
#include <arch/types.h>
#include <nuttx/config.h>
#include <nuttx/arch.h>
#include <nuttx/irq.h>
#include <nuttx/notifier.h>
#include <nuttx/panic_notifier.h>
#include <nuttx/timers/watchdog.h>
#include "fc7300_wdog.h"
#include "fc7300_clockconfig.h"
#include "fc7300_periphclocks.h"
#include "hardware/fc7300_wdog.h"
#include "chip.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#define WDOG_CS_DISABLE_WDOG   (WDOG_CS_UPDATE | WDOG_CS_CLK_SEL_AON_CLK | \
                                WDOG_CS_PRESCALER_ENABLE | WDOG_CS_ULK_STAT)

#define WDOG_CONFIGURATION_TIMEOUT 0xFFFFu
#define WDOG_TIMEOUT_MAX           130000

#if defined(CONFIG_WATCHDOG) && defined(CONFIG_FC7300_WATCHDOG)

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: wdog_getreg
 *
 * Description:
 *   Read the value of a Watchdog timer register.
 *
 * Input Parameters:
 *   priv - A reference to the Watchdog block status
 *   offset - The offset to the register to read
 *
 * Returned Value:
 *   The current contents of the specified register
 *
 ****************************************************************************/

static uint32_t wdog_getreg(struct fc7300_wdog_s *priv, int offset)
{
  return getreg32(priv->base + offset);
}

/****************************************************************************
 * Name: wdog_putreg
 *
 * Description:
 *   Set the value of a Watchdog timer register.
 *
 * Input Parameters:
 *   priv - A reference to the Watchdog block status
 *   offset - The offset to the register to set
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/

static void wdog_putreg(struct fc7300_wdog_s *priv, int offset,
                        uint32_t value)
{
  putreg32(value, priv->base + offset);
}

/****************************************************************************
 * Name: wdog_wait_reconfig
 *
 * Description:
 *   Wait watchdog reconfig bit as 1. Reconfig bit 1 means that it can
 *   reconfigure the watchdog cs,timeout,window register.
 *
 * Input Parameters:
 *   priv - A reference to the Watchdog block status
 *
 * Returned Value:
 *   ERROR - Reconfig bit = 0
 *   OK    - Reconfig bit = 1
 *
 ****************************************************************************/

static inline int wdog_wait_reconfig(struct fc7300_wdog_s *priv)
{
  /* Wait the RECFG_STAT to become 1. */

  if (wdog_getreg(priv, FC7300_WDOG_CS_OFFSET) & WDOG_CS_RECFG_STAT)
      return OK;
  else
      return ERROR;
}

/****************************************************************************
 * Name: wdog_unlock
 *
 * Description:
 *   Unlock watchdog for reconfiguring.
 *
 * Input Parameters:
 *   priv - A reference to the Watchdog block status
 *
 * Returned Value:
 *   ERROR - Unlock failed
 *   OK    - Unlock succeeded
 *
 ****************************************************************************/

static inline int wdog_unlock(struct fc7300_wdog_s *priv)
{
  volatile uint32_t try_cnt = WDOG_CONFIGURATION_TIMEOUT;

  /* Write of the WDOG unlock key to COUNTER register.  This must be done in
   * order to allow any modifications to the WDOG configuration.
   */

  wdog_putreg(priv, FC7300_WDOG_COUNTER_OFFSET, WDOG_COUNTER_UNLOCK);

  while ((!(wdog_getreg(priv, FC7300_WDOG_CS_OFFSET) & WDOG_CS_ULK_STAT))
          && (try_cnt != 0))
    {
      try_cnt--;
    }

  if (try_cnt == 0)
      return ERROR;
  else
      return OK;
}

/****************************************************************************
 * Name: fc7300_wdog_isr
 *
 * Description:
 *   Watchdog interrupt handler
 *
 * Input Parameters:
 *   Usual interrupt handler arguments.
 *
 * Returned Value:
 *   Always returns OK.
 *
 ****************************************************************************/

__ramfunc__ static int fc7300_wdog_isr(int irq, void *context, void *arg)
{
  struct fc7300_wdog_s *priv = (struct fc7300_wdog_s *)arg;
  uint32_t regval;

  /* Check if the interrupt is really pending */

  regval = wdog_getreg(priv, FC7300_WDOG_CS_OFFSET);
  if ((regval & WDOG_CS_FLAG) != 0)
    {
      /* Is there a registered handler? */

      if (priv->handler)
        {
          priv->handler(irq, context, arg);
        }
      else
        {
          PANIC_WITH_REGS("watchdog trigger", context);
        }

      /* wait reset */

      if (wdog_wait_reconfig(priv) == OK)
        {
          if (wdog_unlock(priv) == OK)
            {
              wdog_putreg(priv, regval, FC7300_WDOG_CS_OFFSET);
            }
        }

      up_mdelay(10);
    }

  return OK;
}

#if (CONFIG_FC7300_WATCHDOG_SW_WATCHDOG_INTERVAL > 0)

/****************************************************************************
 * Name: fc7300_wdt_notifier
 *
 * Description:
 *   Software watchdog timer, for handle panic message.
 *
 * Input Parameters:
 *   arg - The hardware watchdog instance private data.
 *
 * Returned Values:
 *   Zero on success.
 *
 ****************************************************************************/

static int fc7300_wdt_notifier(struct notifier_block *nb,
                               unsigned long action,
                               void *data)
{
  if (PANIC_KERNEL_FINAL == action)
    {
      up_mdelay(CONFIG_FC7300_WATCHDOG_SW_WATCHDOG_INTERVAL);
    }

  return 0;
}

/****************************************************************************
 * Name: fc7300_wdt_phandler
 *
 * Description:
 *   Software watchdog timer, for handle panic message.
 *
 * Input Parameters:
 *   arg - The hardware watchdog instance private data.
 *
 * Returned Values:
 *   None
 *
 ****************************************************************************/

static void fc7300_wdt_phandler(wdparm_t arg)
{
  struct fc7300_wdog_s *priv = (struct fc7300_wdog_s *)arg;

  if (priv->iscaptured)
    {
      priv->handler(0, NULL, NULL);
    }
  else
    {
      priv->nb.notifier_call = fc7300_wdt_notifier;
      panic_notifier_chain_register(&priv->nb);
      PANIC();
    }
}

#endif /* CONFIG_FC7300_WATCHDOG_SW_WATCHDOG_INTERVAL > 0 */

/****************************************************************************
 * Name: fc7300_wdog_start
 *
 * Description:
 *   Start the watchdog timer, resetting the time to the current timeout.
 *
 * Input Parameters:
 *   lower - A pointer the publicly visible representation of the
 *           "lower-half" driver state structure.
 *
 * Returned Values:
 *   Zero on success; a negated errno value on failure.
 *
 ****************************************************************************/

static int fc7300_wdog_start(struct watchdog_lowerhalf_s *lower)
{
  int ret;
#if (CONFIG_FC7300_WATCHDOG_SW_WATCHDOG_INTERVAL > 0)
  clock_t tick;
#endif
  uint32_t clkfreq;
  uint32_t cs_regval;
  uint32_t timeout_regval;
  irqstate_t flags;
  struct fc7300_wdog_s *priv = (struct fc7300_wdog_s *)lower;

  DEBUGASSERT(priv);

  cs_regval = wdog_getreg(priv, FC7300_WDOG_CS_OFFSET);
  cs_regval |= WDOG_CS_ENABLE | WDOG_CS_UPDATE | WDOG_CS_PRESCALER_ENABLE;
  cs_regval &= ~WDOG_CS_CLK_SEL_MASK;
  cs_regval &= ~WDOG_CS_DLY_CNT_MSB_MASK;
  cs_regval |= WDOG_CS_CLK_SEL_AON_CLK | WDOG_CS_DLY_CNT_MSB_1024;

  /* Fixed AON_CLK */

  clkfreq = fc7300_get_aonclk();

  /* Get real clock freq in advance to avoid overflow */

  clkfreq = clkfreq / 256;

  timeout_regval = priv->timeout * clkfreq / 1000;

  ret = wdog_wait_reconfig(priv);
  if (ret != OK)
    {
      wderr("ERROR: Wait reconfig failed!\n");
      return -EIO;
    }

  flags = enter_critical_section();

  ret = wdog_unlock(priv);
  if (OK == ret)
    {
      /* Unlock successed, write the new configuration */

      wdog_putreg(priv, FC7300_WDOG_CS_OFFSET, cs_regval);
      wdog_putreg(priv, FC7300_WDOG_TIMEOUT_OFFSET, timeout_regval);
    }

  leave_critical_section(flags);

  if (ret != OK)
      return -EIO;

#if (CONFIG_FC7300_WATCHDOG_SW_WATCHDOG_INTERVAL > 0)
  if (CONFIG_FC7300_WATCHDOG_SW_WATCHDOG_INTERVAL > priv->timeout)
    {
       priv->timeout = CONFIG_FC7300_WATCHDOG_SW_WATCHDOG_INTERVAL;
    }
  //coverity[OVERFLOW_BEFORE_WIDEN:SUPPRESS]
  tick = (priv->timeout - CONFIG_FC7300_WATCHDOG_SW_WATCHDOG_INTERVAL) \
         * USEC_PER_MSEC / USEC_PER_TICK;
  wd_start(&priv->wdog, tick, (wdentry_t)fc7300_wdt_phandler,
          (wdparm_t)priv);
#endif

  return OK;
}

/****************************************************************************
 * Name: fc7300_wdog_stop
 *
 * Description:
 *   Stop the watchdog timer
 *
 * Input Parameters:
 *   lower - A pointer the publicly visible representation of the
 *           "lower-half" driver state structure.
 *
 * Returned Values:
 *   Zero on success; a negated errno value on failure.
 *
 ****************************************************************************/

static int fc7300_wdog_stop(struct watchdog_lowerhalf_s *lower)
{
  int ret;
  uint32_t regval;
  irqstate_t flags;
  struct fc7300_wdog_s *priv = (struct fc7300_wdog_s *)lower;

  DEBUGASSERT(priv);

  regval = (wdog_getreg(priv, FC7300_WDOG_CS_OFFSET) | WDOG_CS_UPDATE | \
            WDOG_CS_PRESCALER_ENABLE) & (~WDOG_CS_ENABLE);

  ret = wdog_wait_reconfig(priv);
  if (ret != OK)
    {
      wderr("ERROR: Wait reconfig failed!\n");
      return -EIO;
    }

  flags = enter_critical_section();

  ret = wdog_unlock(priv);
  if (OK == ret)
    {
      /* Unlock successed, write the new configuration */

      wdog_putreg(priv, FC7300_WDOG_CS_OFFSET, regval);
    }

  leave_critical_section(flags);

  if (ret != OK)
      return -EIO;

#if (CONFIG_FC7300_WATCHDOG_SW_WATCHDOG_INTERVAL > 0)
  /* Stop software watchdog */

  wd_cancel(&priv->wdog);
#endif

  return OK;
}

/****************************************************************************
 * Name: fc7300_wdog_keepalive
 *
 * Description:
 *   Reset the watchdog timer to the current timeout value, prevent any
 *   imminent watchdog timeouts.  This is sometimes referred as "pinging"
 *   the watchdog timer or "petting the dog".
 *
 * Input Parameters:
 *   lower - A pointer the publicly visible representation of the
 *           "lower-half" driver state structure.
 *
 * Returned Values:
 *   Zero on success; a negated errno value on failure.
 *
 ****************************************************************************/

static int fc7300_wdog_keepalive(struct watchdog_lowerhalf_s *lower)
{
#if (CONFIG_FC7300_WATCHDOG_SW_WATCHDOG_INTERVAL > 0)
  clock_t tick;
#endif
  irqstate_t flags;
  struct fc7300_wdog_s *priv = (struct fc7300_wdog_s *)lower;

  DEBUGASSERT(priv);

  /* Disable global interrupt */

  flags = enter_critical_section();

  /* Refresh sequence */

  wdog_putreg(priv, FC7300_WDOG_COUNTER_OFFSET, WDOG_COUNTER_REFRESH);

  /* Enable global interrupt */

  leave_critical_section(flags);

#if (CONFIG_FC7300_WATCHDOG_SW_WATCHDOG_INTERVAL > 0)
  if (CONFIG_FC7300_WATCHDOG_SW_WATCHDOG_INTERVAL > priv->timeout)
    {
       priv->timeout = CONFIG_FC7300_WATCHDOG_SW_WATCHDOG_INTERVAL;
    }
  //coverity[OVERFLOW_BEFORE_WIDEN:SUPPRESS]
  tick = (priv->timeout - CONFIG_FC7300_WATCHDOG_SW_WATCHDOG_INTERVAL) \
         * USEC_PER_MSEC / USEC_PER_TICK;
  wd_cancel(&priv->wdog);
  wd_start(&priv->wdog, tick, (wdentry_t)fc7300_wdt_phandler,
          (wdparm_t)priv);
#endif

  return OK;
}

/****************************************************************************
 * Name: fc7300_wdog_getstatus
 *
 * Description:
 *   Get the current watchdog timer status
 *
 * Input Parameters:
 *   lower  - A pointer the publicly visible representation of
 *            the "lower-half" driver state structure.
 *   status - The location to return the watchdog status information.
 *
 * Returned Values:
 *   Zero on success; a negated errno value on failure.
 *
 ****************************************************************************/

static int fc7300_wdog_getstatus(struct watchdog_lowerhalf_s *lower,
                           struct watchdog_status_s *status)
{
  uint32_t cs_regval;
  uint64_t counter_regval;
  uint64_t timeout_regval;
  uint64_t clkfreq;
  uint32_t enable;
  struct fc7300_wdog_s *priv = (struct fc7300_wdog_s *)lower;

  DEBUGASSERT(priv);

  cs_regval = wdog_getreg(priv, FC7300_WDOG_CS_OFFSET);
  enable = cs_regval & WDOG_CS_ENABLE_MASK;

  /* Fixed AON clk */

  clkfreq = fc7300_get_aonclk();

  if (enable)
    {
      status->flags |= WDFLAGS_ACTIVE;
    }
  else
    {
      status->flags &= ~WDFLAGS_ACTIVE;
    }

  /* Just get timeout from private structure, don't calculate from register.
   * Calculating from register may loss accuracy.
   */

  status->timeout = priv->timeout;

  counter_regval = (uint64_t)wdog_getreg(priv, FC7300_WDOG_COUNTER_OFFSET);
  timeout_regval = (uint64_t)wdog_getreg(priv, FC7300_WDOG_TIMEOUT_OFFSET);

  /* Note :
   *  1.Using unit64_t to avoid overload and accuracy lossing.
   *  2.Calculating timeleft at last to avoid accuracy lossing.
   */

  status->timeleft = (uint32_t)((uint64_t)1000 * 256 \
                     * (timeout_regval - counter_regval) / clkfreq);

  return OK;
}

/****************************************************************************
 * Name: fc7300_wdog_settimeout
 *
 * Description:
 *   Set a new timeout value (and reset the watchdog timer)
 *
 * Input Parameters:
 *   lower   - A pointer the publicly visible representation of
 *             the "lower-half" driver state structure.
 *   timeout - The new timeout value in milliseconds.
 *
 * Returned Values:
 *   Zero on success; a negated errno value on failure.
 *
 ****************************************************************************/

static int fc7300_wdog_settimeout(struct watchdog_lowerhalf_s *lower,
                           uint32_t timeout)
{
  struct fc7300_wdog_s *priv = (struct fc7300_wdog_s *)lower;

  DEBUGASSERT(priv);

  if (timeout == 0 || timeout > WDOG_TIMEOUT_MAX)
    {
      wderr("ERROR: Watchdong timeout is invalid!\n");
      return -EINVAL;
    }

  if ((wdog_getreg(priv, FC7300_WDOG_CS_OFFSET) & WDOG_CS_ENABLE_MASK))
    {
      wderr("ERROR: Watchdong is running!\n");
      return -EIO;
    }

  priv->timeout = timeout;

  return OK;
}

/****************************************************************************
 * Name: fc7300_wdog_capture
 *
 * Description:
 *   Don't reset on watchdog timer timeout; instead, call this user provider
 *   timeout handler.  NOTE:  Providing handler==NULL will restore the reset
 *   behavior.
 *
 * Input Parameters:
 *   lower      - A pointer the publicly visible representation of the
 *                "lower-half" driver state structure.
 *   newhandler - The new watchdog expiration function pointer.  If this
 *                function pointer is NULL, then the reset-on-expiration
 *                behavior is restored,
 *
 * Returned Value:
 *   The previous watchdog expiration function pointer or NULL is there was
 *   no previous function pointer, i.e., if the previous behavior was
 *   reset-on-expiration (NULL is also returned if an error occurs).
 *
 ****************************************************************************/

static xcpt_t fc7300_wdog_capture(struct watchdog_lowerhalf_s *lower,
                                  xcpt_t handler)
{
  struct fc7300_wdog_s *priv = (struct fc7300_wdog_s *)lower;
  irqstate_t flags;
  xcpt_t oldhandler;
  uint32_t regval;
  int ret;

  DEBUGASSERT(priv);

  ret = wdog_wait_reconfig(priv);
  if (ret != OK)
    {
      wderr("ERROR: Wait reconfig failed!\n");
      return priv->handler;
    }

  /* Get the old handler */

  flags = enter_critical_section();
  oldhandler = priv->handler;

  /* Are we attaching or detaching the handler? */

  regval = wdog_getreg(priv, FC7300_WDOG_CS_OFFSET);
  if (handler)
    {
      /* Attaching... Enable interrupt */

      regval |= WDOG_CS_INT;
      ret = wdog_unlock(priv);
      if (OK == ret)
        {
          wdog_putreg(priv, FC7300_WDOG_CS_OFFSET, regval);

          priv->handler    = handler;
          priv->iscaptured = true;
        }
    }
  else
    {
      /* Detaching... Disable interrupt */

      regval &= ~WDOG_CS_INT;
      ret = wdog_unlock(priv);
      if (OK == ret)
        {
          wdog_putreg(priv, FC7300_WDOG_CS_OFFSET, regval);

          priv->handler    = handler;
          priv->iscaptured = false;
        }
    }

  leave_critical_section(flags);
  return oldhandler;
}

static struct watchdog_ops_s fc7300_wdog_ops =
{
  .start      = fc7300_wdog_start,
  .stop       = fc7300_wdog_stop,
  .keepalive  = fc7300_wdog_keepalive,
  .getstatus  = fc7300_wdog_getstatus,
  .settimeout = fc7300_wdog_settimeout,
  .capture    = fc7300_wdog_capture,
  .ioctl      = NULL,
};

#if defined(CONFIG_FC7300_WATCHDOG0)
static struct fc7300_wdog_s fc7300_wdog0 =
{
  .ops  = &fc7300_wdog_ops,
  .base = FC7300_WDOG0_BASE,
  .irq  = FC7300_IRQ_WDOG0,
  .isr_priority = NVIC_WATCHDOG0_PRIORITY_DEFAULT,
  .handler = NULL,
  .iscaptured = false
};
#endif

#if defined(CONFIG_FC7300_WATCHDOG1)
static struct fc7300_wdog_s fc7300_wdog1 =
{
  .ops  = &fc7300_wdog_ops,
  .base = FC7300_WDOG1_BASE,
  .irq  = FC7300_IRQ_WDOG1,
  .isr_priority = NVIC_WATCHDOG1_PRIORITY_DEFAULT,
  .handler = NULL,
  .iscaptured = false
};
#endif

#if defined(CONFIG_FC7300_WATCHDOG2)
static struct fc7300_wdog_s fc7300_wdog2 =
{
  .ops  = &fc7300_wdog_ops,
  .base = FC7300_WDOG2_BASE,
  .irq  = FC7300_IRQ_WDOG2,
  .isr_priority = NVIC_WATCHDOG2_PRIORITY_DEFAULT,
  .handler = NULL,
  .iscaptured = false
};
#endif

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: fc7300_wdoginitialize
 *
 * Description:
 *   Initialize one watchdog instance for use with the upper_level watchdog
 *   driver.
 *
 * Input Parameters:
 *   instance - A number identifying the watchdog use.
 *
 * Returned Value:
 *   On success, a pointer to the FC7300 lower half Watchdog driver is
 *   returned. NULL is returned on any failure.
 *
 ****************************************************************************/

struct watchdog_lowerhalf_s *fc7300_wdoginitialize(int instance)
{
  struct fc7300_wdog_s *wdog = NULL;

  switch (instance)
  {
    case 0:
#if defined(CONFIG_FC7300_WATCHDOG0)
      wdog = &fc7300_wdog0;
      break;
#else
      return NULL;
#endif
    case 1:
#if defined(CONFIG_FC7300_WATCHDOG1)
      wdog = &fc7300_wdog1;
      break;
#else
      return NULL;
#endif
    case 2:
#if defined(CONFIG_FC7300_WATCHDOG2)
      wdog = &fc7300_wdog2;
      break;
#else
      return NULL;
#endif
    default:
      return NULL;
  }

  /* Attach Interrupt handler */

  irq_attach(wdog->irq, fc7300_wdog_isr, wdog);
#ifdef CONFIG_ARCH_IRQPRIO
  up_prioritize_irq(wdog->irq, wdog->isr_priority);
#endif
  up_enable_irq(wdog->irq);

  return (struct watchdog_lowerhalf_s  *)wdog;
}

#endif /* CONFIG_WATCHDOG && CONFIG_FC7300_WATCHDOG */

/****************************************************************************
 * Name: FC7300_Open_Internal_Watchdog
 *
 * Description:
 *   Start the watchdog timer, resetting the time to the current timeout.
 *
 * Input Parameters:
 *   instance - Hardware unit index
 *   timeout  - Timeout value in milliseconds
 *
 * Returned Values:
 *   Zero on success; a negated errno value on failure.
 *
 ****************************************************************************/

int FC7300_Open_Internal_Watchdog(uint8_t instance, uint32_t timeout)
{
  uint32_t try_cnt = 128u;
  uint32_t wdg_base;
  uint32_t clkfreq;
  uint32_t cs_regval;
  uint32_t timeout_regval;

  if (timeout == 0 || timeout > WDOG_TIMEOUT_MAX)
      return ERROR;

  switch (instance)
    {
    case 0:
      wdg_base = FC7300_WDOG0_BASE;
      break;

#if defined(FC7300_WDOG1_BASE)
    case 1:
      wdg_base = FC7300_WDOG1_BASE;
      break;
#endif

#if defined(FC7300_WDOG2_BASE)
    case 2:
      wdg_base = FC7300_WDOG2_BASE;
      break;
#endif

    default:
      return ERROR;
    }

  cs_regval = WDOG_CS_ENABLE | WDOG_CS_CLK_SEL_AON_CLK |
              WDOG_CS_PRESCALER_ENABLE | WDOG_CS_UPDATE |
              WDOG_CS_DLY_CNT_MSB_1024;

  clkfreq = fc7300_get_aonclk();
  clkfreq = clkfreq / 256;
  timeout_regval = timeout * clkfreq / 1000;

  /* If it is not the first time to configure wdog, unlock status will only
     persist for 128 bus clocks. */

  while (try_cnt != 0)
    {
      if ((getreg32(wdg_base + FC7300_WDOG_CS_OFFSET) & WDOG_CS_ULK_STAT) == 0)
        {
          break;
        }
      try_cnt--;
    }

  /* If ULK_STAT turns into 0 in 128 try counts, it means this is not the
     first time to configure the wdog. */

  if (try_cnt != 0)
    {
      /* When ULK_STAT = 0, the wdog can only be unlocked when RECFG_STAT
         becomes 1. */

      try_cnt = WDOG_CONFIGURATION_TIMEOUT;

      while (((getreg32(wdg_base + FC7300_WDOG_CS_OFFSET) &
               WDOG_CS_RECFG_STAT) == 0) &&
             (try_cnt != 0))
        {
          try_cnt--;
        }

      /* Unlock the wdog.
         Note: The unlock status only persist for 128 bus clocks, you shall
         not use single-step or break points in the following few lines.  */

      putreg32(WDOG_COUNTER_UNLOCK, wdg_base + FC7300_WDOG_COUNTER_OFFSET);

      /* Wait until the unlock take effect. */

      try_cnt = WDOG_CONFIGURATION_TIMEOUT;

      while (((getreg32(wdg_base + FC7300_WDOG_CS_OFFSET) &
               WDOG_CS_ULK_STAT) == 0) &&
             (try_cnt != 0))
        {
          try_cnt--;
        }
    }

  /* Configure control and status register */

  putreg32(cs_regval, wdg_base + FC7300_WDOG_CS_OFFSET);

  /* Configure timeout value register */

  putreg32(timeout_regval, wdg_base + FC7300_WDOG_TIMEOUT_OFFSET);

  /* Wait the RECFG_STAT to become 1. */

  try_cnt = WDOG_CONFIGURATION_TIMEOUT;

  while (((getreg32(wdg_base + FC7300_WDOG_CS_OFFSET) &
           WDOG_CS_RECFG_STAT) == 0) &&
         (try_cnt != 0))
    {
      try_cnt--;
    }

  return OK;
}

/****************************************************************************
 * Name: FC7300_Close_Internal_Watchdog
 *
 * Description:
 *   Close the watchdog timer
 *
 * Input Parameters:
 *   instance - Hardware unit index
 *
 * Returned Values:
 *   Void
 *
 ****************************************************************************/

void FC7300_Close_Internal_Watchdog(uint8_t instance)
{
  uint32_t try_cnt = 128u;
  uint32_t wdg_base;

  switch (instance)
    {
    case 0:
      wdg_base = FC7300_WDOG0_BASE;
      break;

#if defined(FC7300_WDOG1_BASE)
    case 1:
      wdg_base = FC7300_WDOG1_BASE;
      break;
#endif

#if defined(FC7300_WDOG2_BASE)
    case 2:
      wdg_base = FC7300_WDOG2_BASE;
      break;
#endif

    default:
      return;
    }

  /* If it is not the first time to configure wdog, unlock status will only
     persist for 128 bus clocks. */

  while (try_cnt != 0)
    {
      if ((getreg32(wdg_base + FC7300_WDOG_CS_OFFSET) & WDOG_CS_ULK_STAT) == 0)
        {
          break;
        }
      try_cnt--;
    }

  /* If ULK_STAT turns into 0 in 128 try counts, it means this is not the
     first time to configure the wdog. */

  if (try_cnt != 0)
    {
      /* When ULK_STAT = 0, the wdog can only be unlocked when RECFG_STAT
         becomes 1. */

      try_cnt = WDOG_CONFIGURATION_TIMEOUT;

      while (((getreg32(wdg_base + FC7300_WDOG_CS_OFFSET) &
               WDOG_CS_RECFG_STAT) == 0) &&
             (try_cnt != 0))
        {
          try_cnt--;
        }

      /* Unlock the wdog.
         Note: The unlock status only persist for 128 bus clocks, you shall
         not use single-step or break points in the following few lines.  */

      putreg32(WDOG_COUNTER_UNLOCK, wdg_base + FC7300_WDOG_COUNTER_OFFSET);

      /* Wait until the unlock take effect. */

      try_cnt = WDOG_CONFIGURATION_TIMEOUT;

      while (((getreg32(wdg_base + FC7300_WDOG_CS_OFFSET) &
               WDOG_CS_ULK_STAT) == 0) &&
             (try_cnt != 0))
        {
          try_cnt--;
        }
    }

  /* Disable Watchdog */

  putreg32(WDOG_CS_DISABLE_WDOG, wdg_base + FC7300_WDOG_CS_OFFSET);

  /* Configure timeout to the maximum. */

  putreg32(0xFFFFu, wdg_base + FC7300_WDOG_TIMEOUT_OFFSET);

  /* Wait the RECFG_STAT to become 1. */

  try_cnt = WDOG_CONFIGURATION_TIMEOUT;

  while (((getreg32(wdg_base + FC7300_WDOG_CS_OFFSET) &
           WDOG_CS_RECFG_STAT) == 0) &&
         (try_cnt != 0))
    {
      try_cnt--;
    }
}
