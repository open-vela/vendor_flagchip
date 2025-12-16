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

#ifndef __VENDOR_FLAGCHIP_CHIPS_FC7300_HARDWARE_FC7300_CPM_H
#define __VENDOR_FLAGCHIP_CHIPS_FC7300_HARDWARE_FC7300_CPM_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include "hardware/fc7300_memorymap.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Register Offsets *********************************************************/

#define FC7300_CPM_FISCR_OFFSET         0x0000      /* FPU Interrupt Status and Control Register */
#define FC7300_CPM_MISCR_OFFSET         0x0004      /* Miscellaneous Control Register */
#define FC7300_CPM_LOCK_OFFSET          0x0020      /* CPM Lock Register */

/* Register Bit Definitions *************************************************/

/* CPM_FISCR Fields */

#define FC7300_CPM_FISCR_FIOC           (1 << 0)    /* Bit 0:  FPU Invalid Operation Interrupt Status */
#define FC7300_CPM_FISCR_FDZC           (1 << 1)    /* Bit 1:  FPU Divide-by-Zero Interrupt Status */
#define FC7300_CPM_FISCR_FUFC           (1 << 2)    /* Bit 2:  FPU Underflow Interrupt Status */
#define FC7300_CPM_FISCR_FOFC           (1 << 3)    /* Bit 3:  FPU Overflow Interrupt Status */
#define FC7300_CPM_FISCR_FIDC           (1 << 4)    /* Bit 4:  FPU Input Denormal Interrupt Status */
#define FC7300_CPM_FISCR_FIXC           (1 << 5)    /* Bit 5:  FPU Inexact Interrupt Status */
                                                    /* Bits 6-15:  Reserved */
#define FC7300_CPM_FISCR_FIOCE          (1 << 16)   /* Bit 16: FPU Invalid Operation Interrupt Enable */
#define FC7300_CPM_FISCR_FDZCE          (1 << 17)   /* Bit 17: FPU Divide-by-Zero Interrupt Enable */
#define FC7300_CPM_FISCR_FUFCE          (1 << 18)   /* Bit 18: FPU Underflow Interrupt Enable */
#define FC7300_CPM_FISCR_FOFCE          (1 << 19)   /* Bit 19: FPU Overflow Interrupt Enable */
#define FC7300_CPM_FISCR_FIDCE          (1 << 20)   /* Bit 20: FPU Input Denormal Interrupt Enable */
#define FC7300_CPM_FISCR_FIXCE          (1 << 21)   /* Bit 21: FPU Inexact Interrupt Enable */
                                                    /* Bits 22-31: Reserved */

/* CPM_MISCR Fields */

#define FC7300_CPM_MISCR_AHBS_PRIORITY  (1 << 0)    /* Bit 0:  AHBS Priority Control */
                                                    /* Bits 1-16:  Reserved */
#define FC7300_CPM_MISCR_CPU_ID_MASK    0x000E0000u /* Bits 17-19: CPU Index */
#define FC7300_CPM_MISCR_CPU_ID_SHIFT   17u
#define FC7300_CPM_MISCR_CPU_ID_WIDTH   3u
                                                    /* Bits 20-31: Reserved */

#endif /* __VENDOR_FLAGCHIP_CHIPS_FC7300_HARDWARE_FC7300_CPM_H */
