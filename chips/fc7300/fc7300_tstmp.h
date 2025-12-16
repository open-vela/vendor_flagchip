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

#ifndef __VENDOR_FLAGCHIP_CHIPS_FC7300_FC7300_TSTMP_H
#define __VENDOR_FLAGCHIP_CHIPS_FC7300_FC7300_TSTMP_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#define FC7300_TSTMP_START(d,us,hd,arg)         ((d)->ops->start(d,us,hd,arg))
#define FC7300_TSTMP_STOP(d)                    ((d)->ops->stop(d))
#define FC7300_TSTMP_GETCOUNTER(d)              ((d)->ops->getcounter(d))

/* This describes the callback function that will be invoked when the oneshot
 * timer expires.
 *
 *   arg - The opaque argument provided when the interrupt was registered
 */

typedef void (*fc7300_tstmp_callback_t)(void *arg);

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

/* TSTMP Device Structure */

struct fc7300_tstmp_dev_s
{
  struct fc7300_tstmp_ops_s *ops;
};

/* TSTMP channel number */

enum fc7300_tstmp_channeltype
{
    TSTMP_CHANNEL_0 = 0U,
    TSTMP_CHANNEL_1,
    TSTMP_CHANNEL_2,
    TSTMP_CHANNEL_3,
};

/* The TSTMP lower half driver interface */

struct fc7300_tstmp_ops_s
{
  void     (*start)(struct fc7300_tstmp_dev_s *dev, uint32_t usec,
                    fc7300_tstmp_callback_t handler, void *arg);
  uint64_t (*stop)(struct fc7300_tstmp_dev_s *dev);
  uint64_t (*getcounter)(struct fc7300_tstmp_dev_s *dev);
};

/****************************************************************************
 * Public Functions Prototypes
 ****************************************************************************/

/****************************************************************************
 * Name: fc7300_tstmp_init
 *
 * Description:
 *   Initialize tstmp hardware, ready for operation with defaults
 *
 * Input Parameters:
 *   timer    - Timer counter channel to be used.
 *
 * Returned Value:
 *   On success, a non-NULL instance of the TSTMP lower-half driver is
 *   returned.  NULL is return on any failure.
 *
 ****************************************************************************/

struct fc7300_tstmp_dev_s *fc7300_tstmp_init(int timer);

/****************************************************************************
 * Name: fc7300_tstmp_deinit
 *
 * Description:
 *   De-initialize tstmp hardware
 *
 * Input Parameters:
 *   dev  - An pointer of struct fc7300_tstmp_dev_s which return by
 *          fc7300_tstmp_init()
 *
 * Returned Value:
 *   Zero (OK) is returned on success, a negated errno value is returned on
 *   any failure.
 *
 ****************************************************************************/

int fc7300_tstmp_deinit(struct fc7300_tstmp_dev_s * dev);

#undef EXTERN
#if defined(__cplusplus)
}
#endif

#endif /* __ASSEMBLY__ */
#endif /* __VENDOR_FLAGCHIP_CHIPS_FC7300_FC7300_TSTMP_H */
