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

#ifndef __VENDOR_FLAGCHIP_CHIPS_FC7300_HARDWARE_FC7300_TRGSEL_H
#define __VENDOR_FLAGCHIP_CHIPS_FC7300_HARDWARE_FC7300_TRGSEL_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>
#include <hardware/fc7300_memorymap.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* TRGSEL Register Offsets */

#define FC7300_TRGSEL_OUT_SEL_OFFSET(n)      (n * 4)  /* OUT SEL for N */

/* TRGSEL Register Addresses */

#define FC7300_TRGSEL0_OUT_SEL(n)       (FC7300_TRGSEL0_BASE + FC7300_TRGSEL_OUT_SEL_OFFSET(n))

#define FC7300_TRGSEL1_OUT_SEL(n)       (FC7300_TRGSEL1_BASE + FC7300_TRGSEL_OUT_SEL_OFFSET(n))

#define FC7300_TRGSEL2_OUT_SEL(n)       (FC7300_TRGSEL2_BASE + FC7300_TRGSEL_OUT_SEL_OFFSET(n))

#define FC7300_TRGSEL3_OUT_SEL(n)       (FC7300_TRGSEL3_BASE + FC7300_TRGSEL_OUT_SEL_OFFSET(n))

/* TRGSEL Register Bitfield Definitions */

/* OUT_SEL Bit Fields */

#define FC7300_TRGSEL_OUT_SEL_LOCK_MASK       0x80000000u
#define FC7300_TRGSEL_OUT_SEL_LOCK_SHIFT      31u
#define FC7300_TRGSEL_OUT_SEL_LOCK_WIDTH      1u
#define FC7300_TRGSEL_OUT_SEL_LOCK(x)         (((uint32_t)(((uint32_t)(x))<<FC7300_TRGSEL_OUT_SEL_LOCK_SHIFT))&FC7300_TRGSEL_OUT_SEL_LOCK_MASK)

#define FC7300_TRGSEL_OUT_SEL_SEL_3_MASK      0x7F000000u
#define FC7300_TRGSEL_OUT_SEL_SEL_3_SHIFT     24u
#define FC7300_TRGSEL_OUT_SEL_SEL_3_WIDTH     7u
#define FC7300_TRGSEL_OUT_SEL_SEL_3(x)        (((uint32_t)(((uint32_t)(x))<<FC7300_TRGSEL_OUT_SEL_SEL_3_SHIFT))&FC7300_TRGSEL_OUT_SEL_SEL_3_MASK)

#define FC7300_TRGSEL_OUT_SEL_SEL_2_MASK      0xFF0000u
#define FC7300_TRGSEL_OUT_SEL_SEL_2_SHIFT     16u
#define FC7300_TRGSEL_OUT_SEL_SEL_2_WIDTH     8u
#define FC7300_TRGSEL_OUT_SEL_SEL_2(x)        (((uint32_t)(((uint32_t)(x))<<FC7300_TRGSEL_OUT_SEL_SEL_2_SHIFT))&FC7300_TRGSEL_OUT_SEL_SEL_2_MASK)

#define FC7300_TRGSEL_OUT_SEL_SEL_1_MASK      0xFF00u
#define FC7300_TRGSEL_OUT_SEL_SEL_1_SHIFT     8u
#define FC7300_TRGSEL_OUT_SEL_SEL_1_WIDTH     8u
#define FC7300_TRGSEL_OUT_SEL_SEL_1(x)        (((uint32_t)(((uint32_t)(x))<<FC7300_TRGSEL_OUT_SEL_SEL_1_SHIFT))&FC7300_TRGSEL_OUT_SEL_SEL_1_MASK)

#define FC7300_TRGSEL_OUT_SEL_SEL_0_MASK      0xFFu
#define FC7300_TRGSEL_OUT_SEL_SEL_0_SHIFT     0u
#define FC7300_TRGSEL_OUT_SEL_SEL_0_WIDTH     8u
#define FC7300_TRGSEL_OUT_SEL_SEL_0(x)        (((uint32_t)(((uint32_t)(x))<<FC7300_TRGSEL_OUT_SEL_SEL_0_SHIFT))&FC7300_TRGSEL_OUT_SEL_SEL_0_MASK)

/* OUT_SEL Reg Mask */

#define FC7300_TRGSEL_OUT_SEL_MASK            0xFFFFFFFFu

#define FC7300_TRGSEL_REGSIZE 4U

/** Number of instances of the TRGSEL module. */

#define FC7300_TRGSEL_INSTANCE_COUNT                    (4u)

#endif /* __VENDOR_FLAGCHIP_CHIPS_FC7300_HARDWARE_FC7300_TRGSEL_H */
