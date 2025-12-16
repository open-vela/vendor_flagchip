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

#ifndef __VENDOR_FLAGCHIP_CHIPS_FC7300_FC7300_PMC_USER_H
#define __VENDOR_FLAGCHIP_CHIPS_FC7300_FC7300_PMC_USER_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>
#include <stdint.h>

/****************************************************************************
 * Public Data
 ****************************************************************************/

struct pmc_fault_data_s
{
  uint8_t  err_type;
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

/* error type define */

#define PMC_HVD_1P1V_HAPPEN     0x01U
#define PMC_HVD_1P1V_RESUME     0x02U
#define PMC_HVD_1P5V_HAPPEN     0x03U
#define PMC_HVD_1P5V_RESUME     0x04U
#define PMC_HVD_2P5V_HAPPEN     0x05U
#define PMC_HVD_2P5V_RESUME     0x06U
#define PMC_HVD_5V_HAPPEN       0x07U
#define PMC_HVD_5V_RESUME       0x08U
#define PMC_HVD_5VB_HAPPEN      0x09U
#define PMC_HVD_5VB_RESUME      0x0AU
#define PMC_LVD_1P5V_HAPPEN     0x0BU
#define PMC_LVD_1P5V_RESUME     0x0CU
#define PMC_LVD_5V_HAPPEN       0x0DU
#define PMC_LVD_5V_RESUME       0x0EU

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

/****************************************************************************
 * Name: fc7300_pmc_user_callback
 *
 * Description:
 *   User-level callback function after PMC interrupt.
 *
 * Input Parameters:
 *   error_type - Error Type, detail see error type define.
 *
 * Return Value:
 *   0: Clear interrupt flag by driver.
 *   1: Clear interrupt flag by user.
 *
 ****************************************************************************/

int fc7300_pmc_user_callback(uint8_t error_type);

/****************************************************************************
 * Name: fc7300_pmc_safety_initialize
 *
 * Description:
 *   Initializes the PMC (Power Management Controller) module. This function
 *   sets up the PMC private data structure, registers the PMC with the
 *   safety core, and prepares it for operation.
 *
 ****************************************************************************/

int fc7300_pmc_safety_initialize(void);

#undef EXTERN
#if defined(__cplusplus)
}
#endif

#endif /* __ASSEMBLY__ */
#endif /* __VENDOR_FLAGCHIP_CHIPS_FC7300_FC7300_PMC_USER_H */
