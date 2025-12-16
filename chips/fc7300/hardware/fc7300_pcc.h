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

#ifndef __VENDOR_FLAGCHIP_CHIPS_FC7300_HARDWARE_FC7300_PCC_H
#define __VENDOR_FLAGCHIP_CHIPS_FC7300_HARDWARE_FC7300_PCC_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>
#include "fc7300_memorymap.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* PCC Register Bitfield Definitions ****************************************/

#define PCC_DWPLK_MASK                      0x80000000u
#define PCC_DWPLK_SHIFT                     31u
#define PCC_DWPLK_WIDTH                     1u
#define PCC_DWP_MASK                        0x70000000u
#define PCC_DWP_SHIFT                       28u
#define PCC_DWP_WIDTH                       3u
#define PCC_DWP(x)                          (((uint32_t)(((uint32_t)(x))<<PCC_DWP_SHIFT))&PCC_DWP_MASK)
#define PCC_CGC_MASK                        0x800000U
#define PCC_CGC_SHIFT                       23U
#define PCC_CGC_WIDTH                       1U
#define PCC_CGC(x)                          (((uint32_t)(((uint32_t)(x))<<PCC_CGC_SHIFT))&PCC_CGC_MASK)
#define PCC_SEL_MASK                        0x700000U
#define PCC_SEL_SHIFT                       20U
#define PCC_SEL_WIDTH                       3U
#define PCC_SEL(x)                          (((uint32_t)(((uint32_t)(x))<<PCC_SEL_SHIFT))&PCC_SEL_MASK)
#define PCC_GetSEL(x)                       ((((uint32_t)(x))&PCC_SEL_MASK)>>PCC_SEL_SHIFT)
#define PCC_DIV_MASK                        0x7U
#define PCC_DIV_SHIFT                       0U
#define PCC_DIV_WIDTH                       3U
#define PCC_DIV(x)                          (((uint32_t)(((uint32_t)(x))<<PCC_DIV_SHIFT))&PCC_DIV_MASK)
#define PCC_GetDIV(x)                       ((((uint32_t)(x))&PCC_DIV_MASK)>>PCC_DIV_SHIFT)
#define PCC_SWR_MASK                        0x10000u

#endif /* __VENDOR_FLAGCHIP_CHIPS_FC7300_HARDWARE_FC7300_PCC_H */
