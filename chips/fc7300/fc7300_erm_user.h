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

#ifndef __VENDOR_FLAGCHIP_CHIPS_FC7300_FC7300_ERM_USER_H
#define __VENDOR_FLAGCHIP_CHIPS_FC7300_FC7300_ERM_USER_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>
#include <stdint.h>

#define ERM_MAX_ERROR_NUM 26

/****************************************************************************
 * Public Types
 ****************************************************************************/

/* Enum for reporting the results of actions performed by the monitor */

enum erm_error_type_e
{
  ERM_ERROR_NONE = 0,         /* No action taken */
  ERM_ERROR_TYPE_SINGLE = 1,  /* Single error */
  ERM_ERROR_TYPE_DOUBLE,      /* Multiple errors */
};


begin_packed_struct struct erm_fault_data_s
{
  uint32_t error_addr;    /* Error address */
  uint8_t  channel    :6; /* Channel number */
  uint8_t  error_type :2; /* Error type  erm_error_type_e*/
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
 * Name: fc7300_erm_user_callback
 *
 * Description:
 *   User-level callback function after ERM interrupt.
 *
 * Input Parameters:
 *   channel    - The channel number where the error occurred.
 *   error_type - Error Type(0x1: Non-correctable, 0x2: single correction)
 *   error_addr - The address where the error occurred
 *
 * Return Value:
 *   0: Clear interrupt flag by driver.
 *   1: Clear interrupt flag by user.
 *
 ****************************************************************************/

int fc7300_erm_user_callback(
    uint8_t channel, uint8_t error_type, uint32_t error_addr);

/****************************************************************************
 * Name: fc7300_erm_safety_initialize
 *
 * Description:
 *   Initialize the ERM module.
 *
 ****************************************************************************/

int fc7300_erm_safety_initialize(void);

#undef EXTERN
#if defined(__cplusplus)
}
#endif

#endif /* __ASSEMBLY__ */
#endif /* __VENDOR_FLAGCHIP_CHIPS_FC7300_FC7300_ERM_USER_H */
