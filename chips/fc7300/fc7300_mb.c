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

#include <assert.h>
#include <debug.h>
#include <errno.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <arm_internal.h>

#include "chip.h"
#include "fc7300_mb.h"
#include "fc7300_multicore.h"
#include "hardware/fc7300_mb.h"

#ifdef CONFIG_FC7300_MB

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#ifdef CONFIG_FC7300_MB_CH0_REQ_INT
#  define FC7300_MB_CH0_REQ_INT_FLAG (CONFIG_FC7300_MB_CH0_REQ_INT << 0U)
#else
#  define FC7300_MB_CH0_REQ_INT_FLAG (0U)
#endif
#ifdef CONFIG_FC7300_MB_CH1_REQ_INT
#  define FC7300_MB_CH1_REQ_INT_FLAG (CONFIG_FC7300_MB_CH1_REQ_INT << 1U)
#else
#  define FC7300_MB_CH1_REQ_INT_FLAG (0U)
#endif
#ifdef CONFIG_FC7300_MB_CH2_REQ_INT
#  define FC7300_MB_CH2_REQ_INT_FLAG (CONFIG_FC7300_MB_CH2_REQ_INT << 2U)
#else
#  define FC7300_MB_CH2_REQ_INT_FLAG (0U)
#endif
#ifdef CONFIG_FC7300_MB_CH3_REQ_INT
#  define FC7300_MB_CH3_REQ_INT_FLAG (CONFIG_FC7300_MB_CH3_REQ_INT << 3U)
#else
#  define FC7300_MB_CH3_REQ_INT_FLAG (0U)
#endif
#ifdef CONFIG_FC7300_MB_CH4_REQ_INT
#  define FC7300_MB_CH4_REQ_INT_FLAG (CONFIG_FC7300_MB_CH4_REQ_INT << 4U)
#else
#  define FC7300_MB_CH4_REQ_INT_FLAG (0U)
#endif
#ifdef CONFIG_FC7300_MB_CH5_REQ_INT
#  define FC7300_MB_CH5_REQ_INT_FLAG (CONFIG_FC7300_MB_CH5_REQ_INT << 5U)
#else
#  define FC7300_MB_CH5_REQ_INT_FLAG (0U)
#endif
#ifdef CONFIG_FC7300_MB_CH6_REQ_INT
#  define FC7300_MB_CH6_REQ_INT_FLAG (CONFIG_FC7300_MB_CH6_REQ_INT << 6U)
#else
#  define FC7300_MB_CH6_REQ_INT_FLAG (0U)
#endif
#ifdef CONFIG_FC7300_MB_CH7_REQ_INT
#  define FC7300_MB_CH7_REQ_INT_FLAG (CONFIG_FC7300_MB_CH7_REQ_INT << 7U)
#else
#  define FC7300_MB_CH7_REQ_INT_FLAG (0U)
#endif
#ifdef CONFIG_FC7300_MB_CH8_REQ_INT
#  define FC7300_MB_CH8_REQ_INT_FLAG (CONFIG_FC7300_MB_CH8_REQ_INT << 8U)
#else
#  define FC7300_MB_CH8_REQ_INT_FLAG (0U)
#endif
#ifdef CONFIG_FC7300_MB_CH9_REQ_INT
#  define FC7300_MB_CH9_REQ_INT_FLAG (CONFIG_FC7300_MB_CH9_REQ_INT << 9U)
#else
#  define FC7300_MB_CH9_REQ_INT_FLAG (0U)
#endif
#ifdef CONFIG_FC7300_MB_CH10_REQ_INT
#  define FC7300_MB_CH10_REQ_INT_FLAG (CONFIG_FC7300_MB_CH10_REQ_INT << 10U)
#else
#  define FC7300_MB_CH10_REQ_INT_FLAG (0U)
#endif
#ifdef CONFIG_FC7300_MB_CH11_REQ_INT
#  define FC7300_MB_CH11_REQ_INT_FLAG (CONFIG_FC7300_MB_CH11_REQ_INT << 11U)
#else
#  define FC7300_MB_CH11_REQ_INT_FLAG (0U)
#endif
#ifdef CONFIG_FC7300_MB_CH12_REQ_INT
#  define FC7300_MB_CH12_REQ_INT_FLAG (CONFIG_FC7300_MB_CH12_REQ_INT << 12U)
#else
#  define FC7300_MB_CH12_REQ_INT_FLAG (0U)
#endif
#ifdef CONFIG_FC7300_MB_CH13_REQ_INT
#  define FC7300_MB_CH13_REQ_INT_FLAG (CONFIG_FC7300_MB_CH13_REQ_INT << 13U)
#else
#  define FC7300_MB_CH13_REQ_INT_FLAG (0U)
#endif
#ifdef CONFIG_FC7300_MB_CH14_REQ_INT
#  define FC7300_MB_CH14_REQ_INT_FLAG (CONFIG_FC7300_MB_CH14_REQ_INT << 14U)
#else
#  define FC7300_MB_CH14_REQ_INT_FLAG (0U)
#endif
#ifdef CONFIG_FC7300_MB_CH15_REQ_INT
#  define FC7300_MB_CH15_REQ_INT_FLAG (CONFIG_FC7300_MB_CH15_REQ_INT << 15U)
#else
#  define FC7300_MB_CH15_REQ_INT_FLAG (0U)
#endif
#ifdef CONFIG_FC7300_MB_CH0_DONE_INT
#  define FC7300_MB_CH0_DONE_INT_FLAG (CONFIG_FC7300_MB_CH0_DONE_INT << 16U)
#else
#  define FC7300_MB_CH0_DONE_INT_FLAG (0U)
#endif
#ifdef CONFIG_FC7300_MB_CH1_DONE_INT
#  define FC7300_MB_CH1_DONE_INT_FLAG (CONFIG_FC7300_MB_CH1_DONE_INT << 17U)
#else
#  define FC7300_MB_CH1_DONE_INT_FLAG (0U)
#endif
#ifdef CONFIG_FC7300_MB_CH2_DONE_INT
#  define FC7300_MB_CH2_DONE_INT_FLAG (CONFIG_FC7300_MB_CH2_DONE_INT << 18U)
#else
#  define FC7300_MB_CH2_DONE_INT_FLAG (0U)
#endif
#ifdef CONFIG_FC7300_MB_CH3_DONE_INT
#  define FC7300_MB_CH3_DONE_INT_FLAG (CONFIG_FC7300_MB_CH3_DONE_INT << 19U)
#else
#  define FC7300_MB_CH3_DONE_INT_FLAG (0U)
#endif
#ifdef CONFIG_FC7300_MB_CH4_DONE_INT
#  define FC7300_MB_CH4_DONE_INT_FLAG (CONFIG_FC7300_MB_CH4_DONE_INT << 20U)
#else
#  define FC7300_MB_CH4_DONE_INT_FLAG (0U)
#endif
#ifdef CONFIG_FC7300_MB_CH5_DONE_INT
#  define FC7300_MB_CH5_DONE_INT_FLAG (CONFIG_FC7300_MB_CH5_DONE_INT << 21U)
#else
#  define FC7300_MB_CH5_DONE_INT_FLAG (0U)
#endif
#ifdef CONFIG_FC7300_MB_CH6_DONE_INT
#  define FC7300_MB_CH6_DONE_INT_FLAG (CONFIG_FC7300_MB_CH6_DONE_INT << 22U)
#else
#  define FC7300_MB_CH6_DONE_INT_FLAG (0U)
#endif
#ifdef CONFIG_FC7300_MB_CH7_DONE_INT
#  define FC7300_MB_CH7_DONE_INT_FLAG (CONFIG_FC7300_MB_CH7_DONE_INT << 23U)
#else
#  define FC7300_MB_CH7_DONE_INT_FLAG (0U)
#endif
#ifdef CONFIG_FC7300_MB_CH8_DONE_INT
#  define FC7300_MB_CH8_DONE_INT_FLAG (CONFIG_FC7300_MB_CH8_DONE_INT << 24U)
#else
#  define FC7300_MB_CH8_DONE_INT_FLAG (0U)
#endif
#ifdef CONFIG_FC7300_MB_CH9_DONE_INT
#  define FC7300_MB_CH9_DONE_INT_FLAG (CONFIG_FC7300_MB_CH9_DONE_INT << 25U)
#else
#  define FC7300_MB_CH9_DONE_INT_FLAG (0U)
#endif
#ifdef CONFIG_FC7300_MB_CH10_DONE_INT
#  define FC7300_MB_CH10_DONE_INT_FLAG (CONFIG_FC7300_MB_CH10_DONE_INT << 26U)
#else
#  define FC7300_MB_CH10_DONE_INT_FLAG (0U)
#endif
#ifdef CONFIG_FC7300_MB_CH11_DONE_INT
#  define FC7300_MB_CH11_DONE_INT_FLAG (CONFIG_FC7300_MB_CH11_DONE_INT << 27U)
#else
#  define FC7300_MB_CH11_DONE_INT_FLAG (0U)
#endif
#ifdef CONFIG_FC7300_MB_CH12_DONE_INT
#  define FC7300_MB_CH12_DONE_INT_FLAG (CONFIG_FC7300_MB_CH12_DONE_INT << 28U)
#else
#  define FC7300_MB_CH12_DONE_INT_FLAG (0U)
#endif
#ifdef CONFIG_FC7300_MB_CH13_DONE_INT
#  define FC7300_MB_CH13_DONE_INT_FLAG (CONFIG_FC7300_MB_CH13_DONE_INT << 29U)
#else
#  define FC7300_MB_CH13_DONE_INT_FLAG (0U)
#endif
#ifdef CONFIG_FC7300_MB_CH14_DONE_INT
#  define FC7300_MB_CH14_DONE_INT_FLAG (CONFIG_FC7300_MB_CH14_DONE_INT << 30U)
#else
#  define FC7300_MB_CH14_DONE_INT_FLAG (0U)
#endif
#ifdef CONFIG_FC7300_MB_CH15_DONE_INT
#  define FC7300_MB_CH15_DONE_INT_FLAG (CONFIG_FC7300_MB_CH15_DONE_INT << 31U)
#else
#  define FC7300_MB_CH15_DONE_INT_FLAG (0U)
#endif

#define FC7300_MB_INT_ENABLE_FLAG                                \
(FC7300_MB_CH0_REQ_INT_FLAG    | FC7300_MB_CH1_REQ_INT_FLAG    | \
 FC7300_MB_CH2_REQ_INT_FLAG    | FC7300_MB_CH3_REQ_INT_FLAG    | \
 FC7300_MB_CH4_REQ_INT_FLAG    | FC7300_MB_CH5_REQ_INT_FLAG    | \
 FC7300_MB_CH6_REQ_INT_FLAG    | FC7300_MB_CH7_REQ_INT_FLAG    | \
 FC7300_MB_CH8_REQ_INT_FLAG    | FC7300_MB_CH9_REQ_INT_FLAG    | \
 FC7300_MB_CH10_REQ_INT_FLAG   | FC7300_MB_CH11_REQ_INT_FLAG   | \
 FC7300_MB_CH12_REQ_INT_FLAG   | FC7300_MB_CH13_REQ_INT_FLAG   | \
 FC7300_MB_CH14_REQ_INT_FLAG   | FC7300_MB_CH15_REQ_INT_FLAG   | \
 FC7300_MB_CH0_DONE_INT_FLAG   | FC7300_MB_CH1_DONE_INT_FLAG   | \
 FC7300_MB_CH2_DONE_INT_FLAG   | FC7300_MB_CH3_DONE_INT_FLAG   | \
 FC7300_MB_CH4_DONE_INT_FLAG   | FC7300_MB_CH5_DONE_INT_FLAG   | \
 FC7300_MB_CH6_DONE_INT_FLAG   | FC7300_MB_CH7_DONE_INT_FLAG   | \
 FC7300_MB_CH8_DONE_INT_FLAG   | FC7300_MB_CH9_DONE_INT_FLAG   | \
 FC7300_MB_CH10_DONE_INT_FLAG  | FC7300_MB_CH11_DONE_INT_FLAG  | \
 FC7300_MB_CH12_DONE_INT_FLAG  | FC7300_MB_CH13_DONE_INT_FLAG  | \
 FC7300_MB_CH14_DONE_INT_FLAG  | FC7300_MB_CH15_DONE_INT_FLAG)

/****************************************************************************
 * Private Functions Decleration
 ****************************************************************************/

static int fc7300_mb_acquire_sema(uint8_t chidx);

static void fc7300_mb_release_sema(uint8_t chidx);

static int fc7300_mb_send_data(uint8_t chidx, enum fc7300_mb_core_e dst_core,
                               uint32_t data0, uint32_t data1);

static void fc7300_mb_get_data(uint8_t chidx, uint32_t *pdata0,
                               uint32_t *pdata1);

static int fc7300_mb_register_req_callback(uint8_t chidx,
                                           fc7300_mb_callback_t callback,
                                           void *arg);

static int fc7300_mb_register_done_callback(uint8_t chidx,
                                            fc7300_mb_callback_t callback,
                                            void *arg);

/****************************************************************************
 * Private Types
 ****************************************************************************/

struct fc7300_mb_dev_priv_s
{
  struct fc7300_mb_dev_s dev;
  uint32_t int_enable;
  struct fc7300_mb_chan_priv_s *pchannels[FC7300_MB_COM_CHN_CNT];
};

struct fc7300_mb_chan_priv_s
{
  fc7300_mb_callback_t req_callback;
  void                *req_context;
  fc7300_mb_callback_t done_callback;
  void                *done_context;
  uint8_t              chidx;
};

/****************************************************************************
 * Private Data
 ****************************************************************************/

#ifdef CONFIG_FC7300_MB_CH0
static struct fc7300_mb_chan_priv_s s_fc7300_mb_ch0 =
{
  .req_callback = NULL,
  .req_context = NULL,
  .done_callback = NULL,
  .done_context = NULL,
  .chidx = 0
};
#endif

#ifdef CONFIG_FC7300_MB_CH1
static struct fc7300_mb_chan_priv_s s_fc7300_mb_ch1 =
{
  .req_callback = NULL,
  .req_context = NULL,
  .done_callback = NULL,
  .done_context = NULL,
  .chidx = 1
};
#endif

#ifdef CONFIG_FC7300_MB_CH2
static struct fc7300_mb_chan_priv_s s_fc7300_mb_ch2 =
{
  .req_callback = NULL,
  .req_context = NULL,
  .done_callback = NULL,
  .done_context = NULL,
  .chidx = 2
};
#endif

#ifdef CONFIG_FC7300_MB_CH3
static struct fc7300_mb_chan_priv_s s_fc7300_mb_ch3 =
{
  .req_callback = NULL,
  .req_context = NULL,
  .done_callback = NULL,
  .done_context = NULL,
  .chidx = 3
};
#endif

#ifdef CONFIG_FC7300_MB_CH4
static struct fc7300_mb_chan_priv_s s_fc7300_mb_ch4 =
{
  .req_callback = NULL,
  .req_context = NULL,
  .done_callback = NULL,
  .done_context = NULL,
  .chidx = 4
};
#endif

#ifdef CONFIG_FC7300_MB_CH5
static struct fc7300_mb_chan_priv_s s_fc7300_mb_ch5 =
{
  .req_callback = NULL,
  .req_context = NULL,
  .done_callback = NULL,
  .done_context = NULL,
  .chidx = 5
};
#endif

#ifdef CONFIG_FC7300_MB_CH6
static struct fc7300_mb_chan_priv_s s_fc7300_mb_ch6 =
{
  .req_callback = NULL,
  .req_context = NULL,
  .done_callback = NULL,
  .done_context = NULL,
  .chidx = 6
};
#endif

#ifdef CONFIG_FC7300_MB_CH7
static struct fc7300_mb_chan_priv_s s_fc7300_mb_ch7 =
{
  .req_callback = NULL,
  .req_context = NULL,
  .done_callback = NULL,
  .done_context = NULL,
  .chidx = 7
};
#endif

#ifdef CONFIG_FC7300_MB_CH8
static struct fc7300_mb_chan_priv_s s_fc7300_mb_ch8 =
{
  .req_callback = NULL,
  .req_context = NULL,
  .done_callback = NULL,
  .done_context = NULL,
  .chidx = 8
};
#endif

#ifdef CONFIG_FC7300_MB_CH9
static struct fc7300_mb_chan_priv_s s_fc7300_mb_ch9 =
{
  .req_callback = NULL,
  .req_context = NULL,
  .done_callback = NULL,
  .done_context = NULL,
  .chidx = 9
};
#endif

#ifdef CONFIG_FC7300_MB_CH10
static struct fc7300_mb_chan_priv_s s_fc7300_mb_ch10 =
{
  .req_callback = NULL,
  .req_context = NULL,
  .done_callback = NULL,
  .done_context = NULL,
  .chidx = 10
};
#endif

#ifdef CONFIG_FC7300_MB_CH11
static struct fc7300_mb_chan_priv_s s_fc7300_mb_ch11 =
{
  .req_callback = NULL,
  .req_context = NULL,
  .done_callback = NULL,
  .done_context = NULL,
  .chidx = 11
};
#endif

#ifdef CONFIG_FC7300_MB_CH12
static struct fc7300_mb_chan_priv_s s_fc7300_mb_ch12 =
{
  .req_callback = NULL,
  .req_context = NULL,
  .done_callback = NULL,
  .done_context = NULL,
  .chidx = 12
};
#endif

#ifdef CONFIG_FC7300_MB_CH13
static struct fc7300_mb_chan_priv_s s_fc7300_mb_ch13 =
{
  .req_callback = NULL,
  .req_context = NULL,
  .done_callback = NULL,
  .done_context = NULL,
  .chidx = 13
};
#endif

#ifdef CONFIG_FC7300_MB_CH14
static struct fc7300_mb_chan_priv_s s_fc7300_mb_ch14 =
{
  .req_callback = NULL,
  .req_context = NULL,
  .done_callback = NULL,
  .done_context = NULL,
  .chidx = 14
};
#endif

#ifdef CONFIG_FC7300_MB_CH15
static struct fc7300_mb_chan_priv_s s_fc7300_mb_ch15 =
{
  .req_callback = NULL,
  .req_context = NULL,
  .done_callback = NULL,
  .done_context = NULL,
  .chidx = 15
};
#endif

static const struct fc7300_mb_ops_s s_fc7300_mb_ops =
{
  .acquire_sema = fc7300_mb_acquire_sema,
  .release_sema = fc7300_mb_release_sema,
  .send_data = fc7300_mb_send_data,
  .get_data = fc7300_mb_get_data,
  .register_req_callback = fc7300_mb_register_req_callback,
  .register_done_callback = fc7300_mb_register_done_callback
};

static struct fc7300_mb_dev_priv_s s_fc7300_mb_priv =
{
  .dev =
  {
    .ops = &s_fc7300_mb_ops
  },
  .int_enable = FC7300_MB_INT_ENABLE_FLAG,
  .pchannels =
  {
#ifdef CONFIG_FC7300_MB_CH0
    &s_fc7300_mb_ch0,
#else
    NULL,
#endif
#ifdef CONFIG_FC7300_MB_CH1
    &s_fc7300_mb_ch1,
#else
    NULL,
#endif
#ifdef CONFIG_FC7300_MB_CH2
    &s_fc7300_mb_ch2,
#else
    NULL,
#endif
#ifdef CONFIG_FC7300_MB_CH3
    &s_fc7300_mb_ch3,
#else
    NULL,
#endif
#ifdef CONFIG_FC7300_MB_CH4
    &s_fc7300_mb_ch4,
#else
    NULL,
#endif
#ifdef CONFIG_FC7300_MB_CH5
    &s_fc7300_mb_ch5,
#else
    NULL,
#endif
#ifdef CONFIG_FC7300_MB_CH6
    &s_fc7300_mb_ch6,
#else
    NULL,
#endif
#ifdef CONFIG_FC7300_MB_CH7
    &s_fc7300_mb_ch7,
#else
    NULL,
#endif
#ifdef CONFIG_FC7300_MB_CH8
    &s_fc7300_mb_ch8,
#else
    NULL,
#endif
#ifdef CONFIG_FC7300_MB_CH9
    &s_fc7300_mb_ch9,
#else
    NULL,
#endif
#ifdef CONFIG_FC7300_MB_CH10
    &s_fc7300_mb_ch10,
#else
    NULL,
#endif
#ifdef CONFIG_FC7300_MB_CH11
    &s_fc7300_mb_ch11,
#else
    NULL,
#endif
#ifdef CONFIG_FC7300_MB_CH12
    &s_fc7300_mb_ch12,
#else
    NULL,
#endif
#ifdef CONFIG_FC7300_MB_CH13
    &s_fc7300_mb_ch13,
#else
    NULL,
#endif
#ifdef CONFIG_FC7300_MB_CH14
    &s_fc7300_mb_ch14,
#else
    NULL,
#endif
#ifdef CONFIG_FC7300_MB_CH15
    &s_fc7300_mb_ch15
#else
    NULL
#endif
  }
};

static const uint8_t s_fc7300_mb_ids[FC7300_MB_INT_CHN_CNT] = { 0, 2, 4, 8 };

/****************************************************************************
 * public Data
 ****************************************************************************/

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: arm_lsb
 *
 * Description:
 *   Calculate position of lsb that's equal to 1
 *
 * Input Parameters:
 *   value - The value to perform the operation on
 *
 * Returned Value:
 *   location of lsb which is equal to 1, returns 32 when value is 0
 *
 ****************************************************************************/

static inline uint32_t arm_lsb(uint32_t value)
{
  uint32_t          ret;
  volatile uint32_t rvalue = value;
  __asm__ __volatile__("rbit %1,%0" : "=r"(rvalue) : "r"(rvalue));
  __asm__ __volatile__("clz %0, %1" : "=r"(ret) : "r"(rvalue));
  return ret;
}

/****************************************************************************
 * Name: fc7300_mb_getcoreidx
 *
 * Description:
 *   Get the core ID from mailbox ID
 *
 * Input Parameters:
 *   mb_id - The mailbox ID
 *
 * Returned Value:
 *   location of lsb which is equal to 1, returns 32 when value is 0
 *
 ****************************************************************************/

static inline uint8_t fc7300_mb_getcoreidx(uint8_t mb_id)
{
  uint8_t coreidx;

  for (coreidx = 0; coreidx < FC7300_MB_INT_CHN_CNT; coreidx++)
    {
      if (s_fc7300_mb_ids[coreidx] == mb_id)
        {
          break;
        }
    }

  return coreidx;
}

/****************************************************************************
 * Name: fc7300_mb_acquire_sema
 *
 * Description:
 *   Get the semaphore with the mailbox channel
 *
 * Input Parameters:
 *   chidx - The mailbox channel index
 *
 * Returned Value:
 *   Zero (OK) is returned on success; a negated errno value is returned on
 *   any failure.
 *
 ****************************************************************************/

static int fc7300_mb_acquire_sema(uint8_t chidx)
{
  uint32_t regval;
  int ret;

  regval = getreg32(FC7300_MB_CC_SEMA(chidx));

  ret    = ((regval & FC7300_MB_CC_SEMA_LOCK) != 0) ? OK : -EBUSY;

  return ret;
}

/****************************************************************************
 * Name: fc7300_mb_release_sema
 *
 * Description:
 *   Release semaphore of the mailbox channel
 *
 * Input Parameters:
 *   chidx - The mailbox channel index
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/

static void fc7300_mb_release_sema(uint8_t chidx)
{
  putreg32(FC7300_MB_CC_DONE_CLEAR_LOCK_KEY,
           FC7300_MB_CC_DONE(chidx));
}

/****************************************************************************
 * Name: fc7300_mb_send_data
 *
 * Description:
 *   Send data to other core with the mailbox channel
 *
 * Input Parameters:
 *   chidx    - The mailbox channel index
 *   dst_core - The core id to receive the data
 *   data0    - first data to send
 *   data1    - second data to send
 *
 * Returned Value:
 *   Zero (OK) is returned on success; a negated errno value is returned on
 *   any failure.
 *
 ****************************************************************************/

static int fc7300_mb_send_data(uint8_t chidx, enum fc7300_mb_core_e dst_core,
                               uint32_t data0, uint32_t data1)
{
  uint32_t regval;
  uint32_t coreid;
  int ret;

  regval = getreg32(FC7300_MB_CC_SEMA(chidx));

  if (regval & FC7300_MB_CC_SEMA_LOCK)
    {
      coreid = fc7300_cpu_index();

      /* If done interrupt is disabled, clear the done flag first. */

      regval = (1U << (chidx + FC7300_MB_INT_INTEN_DONE_INT_EN_SHIFT)) &
               ~(s_fc7300_mb_priv.int_enable);

      if (regval != 0U)
        {
          putreg32(regval, FC7300_MB_INT_FLG(coreid));
        }

      /* Configure the dst core to send done event and master core to receive
       * done event. */

      regval = FC7300_MB_CC_DONE_MASK_DONE_MASK(coreid) |
               FC7300_MB_CC_DONE_MASK_DONE_MASTER_ID(
                 s_fc7300_mb_ids[dst_core]);
      putreg32(regval, FC7300_MB_CC_DONE_MASK(chidx));

      /* Auto unlock after receiving done event. */

      putreg32(FC7300_MB_CC_SEMA_UNLK_AUTO_CLEAR_EN(coreid),
               FC7300_MB_CC_SEMA_UNLK(chidx));

      /* Write data */

      putreg32(data0, FC7300_MB_CC_DATA0(chidx));
      putreg32(data1, FC7300_MB_CC_DATA1(chidx));

      /* Trigger request */

      putreg32(FC7300_MB_CC_REQUEST_REQ(dst_core), FC7300_MB_CC_REQUEST(chidx));

      ret = OK;
    }
  else
    {
      ret = -EBUSY;
    }

  return ret;
}

/****************************************************************************
 * Name: fc7300_mb_get_data
 *
 * Description:
 *   Get data form the mailbox channel
 *
 * Input Parameters:
 *   chidx  - The mailbox channel index
 *   pdata0 - the buffer for the first data
 *   pdata1 - the buffer for the second data
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/

static void fc7300_mb_get_data(uint8_t chidx,
                               uint32_t *pdata0, uint32_t *pdata1)
{
  *pdata0 = getreg32(FC7300_MB_CC_DATA0(chidx));
  *pdata1 = getreg32(FC7300_MB_CC_DATA1(chidx));
}

/****************************************************************************
 * Name: fc7300_mb_register_req_callback
 *
 * Description:
 *   Register REQ interrupt callback
 *
 * Input Parameters:
 *   chidx    - The mailbox channel index
 *   callback - the callback function
 *   arg      - the context argument
 *
 * Returned Value:
 *   Zero (OK) is returned on success; a negated errno value is returned on
 *   any failure.
 *
 ****************************************************************************/

static int fc7300_mb_register_req_callback(uint8_t chidx,
                                           fc7300_mb_callback_t callback,
                                           void *arg)
{
  int ret;

  if (s_fc7300_mb_priv.pchannels[chidx] != NULL)
    {
      s_fc7300_mb_priv.pchannels[chidx]->req_callback = callback;
      s_fc7300_mb_priv.pchannels[chidx]->req_context = arg;
      ret = OK;
    }
  else
    {
      ret = -ENXIO;
    }

  return ret;
}

/****************************************************************************
 * Name: fc7300_mb_register_done_callback
 *
 * Description:
 *   Register DONE interrupt callback
 *
 * Input Parameters:
 *   chidx    - The mailbox channel index
 *   callback - the callback function
 *   arg      - the context argument
 *
 * Returned Value:
 *   Zero (OK) is returned on success; a negated errno value is returned on
 *   any failure.
 *
 ****************************************************************************/

static int fc7300_mb_register_done_callback(uint8_t chidx,
                                            fc7300_mb_callback_t callback,
                                            void *arg)
{
  int ret;

  if (s_fc7300_mb_priv.pchannels[chidx] != NULL)
    {
      s_fc7300_mb_priv.pchannels[chidx]->done_callback = callback;
      s_fc7300_mb_priv.pchannels[chidx]->done_context = arg;
      ret = OK;
    }
  else
    {
      ret = -ENXIO;
    }

  return ret;
}

/****************************************************************************
 * Name: fc7300_mb_interrupt
 *
 * Description:
 *   The interrupt handler of Mailbox
 *
 * Input Parameters:
 *   irq     - The mailbox channel index
 *   context - the context argument
 *   arg     - the additional argument
 *
 * Returned Value:
 *   Zero (OK) is returned on success; a negated errno value is returned on
 *   any failure.
 *
 ****************************************************************************/

static int fc7300_mb_interrupt(int irq, void *context, void *arg)
{
  struct fc7300_mb_dev_priv_s *priv = (struct fc7300_mb_dev_priv_s *)arg;
  uint32_t coreid = fc7300_cpu_index();
  struct fc7300_mb_chan_priv_s *pchannel;
  uint32_t regval;
  uint32_t flag;
  uint32_t masterid;
  uint8_t chidx;

  regval = getreg32(FC7300_MB_INT_FLG(coreid)) & priv->int_enable;
  putreg32(regval, FC7300_MB_INT_FLG(coreid));

  flag   = (regval & FC7300_MB_INT_FLG_STAT_REQ_FLAG_STAT_MASK) >>
           FC7300_MB_INT_FLG_STAT_REQ_FLAG_STAT_SHIFT;

  while ((chidx = arm_lsb(flag)) != 32)
   {
      pchannel = priv->pchannels[chidx];

      if ((pchannel != NULL) &&
          (pchannel->req_callback != NULL))
        {
          pchannel->req_callback(pchannel->chidx, pchannel->req_context);
        }

      masterid = getreg32(FC7300_MB_CC_STAT(chidx));
      masterid = (masterid & FC7300_MB_CC_STAT_CURRENT_LOCK_MASTER_ID_MASK) >>
                 FC7300_MB_CC_STAT_CURRENT_LOCK_MASTER_ID_SHIFT;
      masterid = fc7300_mb_getcoreidx(masterid);
      putreg32(FC7300_MB_CC_DONE_DONE(masterid), FC7300_MB_CC_DONE(chidx));

      flag    &= ~(1 << chidx);
   }

  flag = (regval & FC7300_MB_INT_FLG_STAT_DONE_FLAG_STAT_MASK) >>
         FC7300_MB_INT_FLG_STAT_DONE_FLAG_STAT_SHIFT;

  while ((chidx = arm_lsb(flag)) != 32)
   {
      pchannel = priv->pchannels[chidx];

      if ((pchannel != NULL) &&
          (pchannel->done_callback != NULL))
        {
          pchannel->done_callback(pchannel->chidx, pchannel->done_context);
        }

      flag    &= ~(1 << chidx);
   }

  return OK;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: fc7300_mb_initialize
 *
 * Description:
 *   Initialize the Mailbox peripheral
 *
 * Input Parameters:
 *   None
 *
 * Returned Value:
 *   None
 *
 * Note:
 *   Every core need the Mailbox shall call this function to enable the
 *   dedicated interrupt.
 *
 ****************************************************************************/

void fc7300_mb_initialize(void)
{
  uint8_t coreidx;

  for (coreidx = 0; coreidx < FC7300_MB_INT_CHN_CNT; coreidx++)
    {
      putreg32(0, FC7300_MB_INT_CTRL(coreidx));
      putreg32(0xFFFFFFFF, FC7300_MB_INT_FLG_MASK(coreidx));
    }

  coreidx = fc7300_cpu_index();
  putreg32(s_fc7300_mb_priv.int_enable, FC7300_MB_INT_INTEN(coreidx));

  irq_attach(FC7300_IRQ_MB, fc7300_mb_interrupt, &s_fc7300_mb_priv);
#ifdef CONFIG_ARCH_IRQPRIO
  up_prioritize_irq(FC7300_IRQ_MB, NVIC_SYSH_PRIORITY_DEFAULT);
#endif
  up_enable_irq(FC7300_IRQ_MB);
}

/****************************************************************************
 * Name: fc7300_mb_getdev
 *
 * Description:
 *   Get the mailbox device handle
 *
 * Input Parameters:
 *   None
 *
 * Returned Value:
 *   The maibox device handle
 *
 ****************************************************************************/

struct fc7300_mb_dev_s *fc7300_mb_getdev(void)
{
  return &s_fc7300_mb_priv.dev;
}

#endif
