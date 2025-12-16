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

#ifndef __VENDOR_FLAGCHIP_CHIPS_FC7300_FC7300_MB_H
#define __VENDOR_FLAGCHIP_CHIPS_FC7300_FC7300_MB_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>
#include <nuttx/arch.h>
#include <nuttx/irq.h>
#include <stdint.h>
#include <sys/types.h>

#include "chip.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Name: FC7300_MB_ACQUIRE_SEMA
 *
 * Description:
 *   Get the semaphore with the mailbox channel
 *
 * Input Parameters:
 *   dev   - The mailbox device handle
 *   chidx - The mailbox channel index
 *
 * Returned Value:
 *   Zero (OK) is returned on success; a negated errno value is returned on
 *   any failure.
 *
 ****************************************************************************/

#define FC7300_MB_ACQUIRE_SEMA(dev, chidx) (dev)->ops->acquire_sema(chidx)

/****************************************************************************
 * Name: FC7300_MB_RELEASE_SEMA
 *
 * Description:
 *   Release semaphore of the mailbox channel
 *
 * Input Parameters:
 *   dev   - The mailbox device handle
 *   chidx - The mailbox channel index
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/

#define FC7300_MB_RELEASE_SEMA(dev, chidx) (dev)->ops->release_sema(chidx)

/****************************************************************************
 * Name: FC7300_MB_SEND_DATA
 *
 * Description:
 *   Send data to other core with the mailbox channel
 *
 * Input Parameters:
 *   dev      - The mailbox device handle
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

#define FC7300_MB_SEND_DATA(dev, chidx, dst_core, data0, data1) \
    (dev)->ops->send_data(chidx, dst_core, data0, data1)

/****************************************************************************
 * Name: FC7300_MB_GET_DATA
 *
 * Description:
 *   Get data form the mailbox channel
 *
 * Input Parameters:
 *   dev    - The mailbox device handle
 *   chidx  - The mailbox channel index
 *   pdata0 - the buffer for the first data
 *   pdata1 - the buffer for the second data
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/

#define FC7300_MB_GET_DATA(dev, chidx, pdata0, pdata1) \
    (dev)->ops->get_data(chidx, pdata0, pdata1)

/****************************************************************************
 * Name: FC7300_MB_REG_REQ_CBK
 *
 * Description:
 *   Register REQ interrupt callback
 *
 * Input Parameters:
 *   dev      - The mailbox device handle
 *   chidx    - The mailbox channel index
 *   callback - the callback function
 *   arg      - the context argument
 *
 * Returned Value:
 *   Zero (OK) is returned on success; a negated errno value is returned on
 *   any failure.
 *
 ****************************************************************************/

#define FC7300_MB_REG_REQ_CBK(dev, chidx, callback, arg) \
    (dev)->ops->register_req_callback(chidx, callback, arg)

/****************************************************************************
 * Name: FC7300_MB_REG_DONE_CBK
 *
 * Description:
 *   Register DONE interrupt callback
 *
 * Input Parameters:
 *   dev      - The mailbox device handle
 *   chidx    - The mailbox channel index
 *   callback - the callback function
 *   arg      - the context argument
 *
 * Returned Value:
 *   Zero (OK) is returned on success; a negated errno value is returned on
 *   any failure.
 *
 ****************************************************************************/

#define FC7300_MB_REG_DONE_CBK(dev, chidx, callback, arg) \
    (dev)->ops->register_done_callback(chidx, callback, arg)

/****************************************************************************
 * Public Types
 ****************************************************************************/

typedef void (*fc7300_mb_callback_t)(uint8_t chidx, void *context);

enum fc7300_mb_core_e
{
  fc7300_mb_core0 = 0,
  fc7300_mb_core1 = 1,
  fc7300_mb_core2 = 2,
  fc7300_mb_hsm   = 3
};

struct fc7300_mb_ops_s
{
  int (*acquire_sema)(uint8_t chidx);
  void (*release_sema)(uint8_t chidx);
  int (*send_data)(uint8_t chidx,
                   enum fc7300_mb_core_e dst_core,
                   uint32_t data0, uint32_t data1);
  void (*get_data)(uint8_t chidx,
                   uint32_t *pdata0, uint32_t *pdata1);
  int (*register_req_callback)(uint8_t chidx,
                               fc7300_mb_callback_t callback,
                               void *arg);
  int (*register_done_callback)(uint8_t chidx,
                                fc7300_mb_callback_t callback,
                                void *arg);
};


struct fc7300_mb_dev_s
{
  const struct fc7300_mb_ops_s *ops;
};

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

void fc7300_mb_initialize(void);

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

struct fc7300_mb_dev_s *fc7300_mb_getdev(void);

#endif /* __VENDOR_FLAGCHIP_CHIPS_FC7300_FC7300_MB_H */
