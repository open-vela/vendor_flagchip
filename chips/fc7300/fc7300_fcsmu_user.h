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

#ifndef __VENDOR_FLAGCHIP_CHIPS_FC7300_FC7300_FCSMU_USER_H
#define __VENDOR_FLAGCHIP_CHIPS_FC7300_FC7300_FCSMU_USER_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>
#include <stdint.h>
#include <nuttx/bits.h>

#define FCSMU_MAX_ERROR_NUM 32

/****************************************************************************
 * Public Types
 ****************************************************************************/

enum fcsmu_error_type_e
{
  FCSMU_ERROR_TYPE_NONE = 0,
  FCSMU_ERROR_TYPE_FAULT = 1,
  FCSMU_ERROR_TYPE_WARNING,
  FCSMU_ERROR_TYPE_TIMEOUT,
};

/* FCSMU fault result data structure */

begin_packed_struct struct fcsmu_fault_data_s
{
  uint8_t  fcsmu_no       :6;  /* FCSMU error channel */
  uint8_t  fcsmu_err_type :2;  /* Master number */
} end_packed_struct;

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
 * Name: fc7300_fcsmu_safety_initialize
 *
 * Description:
 *   Initialize and register FCSMU safety monitor
 *
 ****************************************************************************/

int fc7300_fcsmu_safety_initialize(void);

/****************************************************************************
 * Name: fc7300_fcsmu_user_callback
 *
 * Description:
 *   User-level callback function after FCSMU interrupt.
 *
 * Input Parameters:
 *   channel    - The channel number where the error occurred.
 *   error_type - Error Type(0x1: fault, 0x2: warning, 0x3: timeout).
 *
 * Return Value:
 *   0: Clear interrupt flag by driver.
 *   1: Clear interrupt flag by user.
 *
 ****************************************************************************/

int fc7300_fcsmu_user_callback(uint32_t channel, uint32_t error_type);

#undef EXTERN
#if defined(__cplusplus)
}
#endif

#endif /* __ASSEMBLY__ */
#endif /* __VENDOR_FLAGCHIP_CHIPS_FC7300_FC7300_ERM_USER_H */
