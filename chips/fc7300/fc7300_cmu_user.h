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

#ifndef __VENDOR_FLAGCHIP_CHIPS_FC7300_FC7300_CMU_USER_H
#define __VENDOR_FLAGCHIP_CHIPS_FC7300_FC7300_CMU_USER_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>
#include <stdint.h>

#define CMU_MAX_ERROR_NUM 5

/****************************************************************************
 * Public Data
 ****************************************************************************/

struct cmu_fault_data_s
{
  uint8_t cmu_idx;         /* CMU instance number */
  uint8_t cmu_fault_type;  /* Fault type */
};

#ifndef __ASSEMBLY__

#undef EXTERN
#if defined(__cplusplus)
#define EXTERN extern "C"
extern "C"
{
#else
#define EXTERN extern
#endif

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

/****************************************************************************
 * Name: fc7300_cmu_safety_initialize
 *
 * Description:
 *   Initialize CMU safety module.
 *
 ****************************************************************************/

int fc7300_cmu_safety_initialize(void);

/****************************************************************************
 * Name: fc7300_cmu_user_callback
 *
 * Description:
 *   User-level callback function after CMU interrupt.
 *
 * Input Parameters:
 *   channel    - The channel number where the error occurred.
 *   error_type - Error Type
 *               (0x1: Monitor clock is loss
 *                0x2: Monitor clock is mismatched)
 *
 * Return Value:
 *   0: Clear interrupt flag by driver.
 *   1: Clear interrupt flag by user.
 *
 ****************************************************************************/

int fc7300_cmu_user_callback(
    uint8_t channel, uint8_t error_type);

#undef EXTERN
#if defined(__cplusplus)
}
#endif

#endif /* __ASSEMBLY__ */
#endif /* __VENDOR_FLAGCHIP_CHIPS_FC7300_FC7300_CMU_USER_H */
