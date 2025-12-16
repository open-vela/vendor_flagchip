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

#ifndef __VENDOR_FLAGCHIP_CHIPS_FC7300_HARDWARE_FC7300_CRC_H
#define __VENDOR_FLAGCHIP_CHIPS_FC7300_HARDWARE_FC7300_CRC_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>
#include "hardware/fc7300_memorymap.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Register offsets *********************************************************/

#define FC7300_CRC_DATA_OFFSET      0x00  /* Data Register Register */
#define FC7300_CRC_POLY_OFFSET      0x04  /* Polynomial Register Register */
#define FC7300_CRC_CR_OFFSET        0x08  /* Control Register Register */

/* Register bit definitions *************************************************/

/* Data Register Register */

#define CRC_DATA_LL_SHIFT              (0)       /* Bit 0-7:  CRC Low Lower Byte */
#define CRC_DATA_LL_MASK               (0xFF << CRC_DATA_LL_SHIFT)
#  define CRC_DATA_LL(x)               (((x) << CRC_DATA_LL_SHIFT) & CRC_DATA_LL_MASK)
#define CRC_DATA_LU_SHIFT              (8)       /* Bit 8-15:  CRC Low Upper Byte */
#define CRC_DATA_LU_MASK               (0xFF << CRC_DATA_LU_SHIFT)
#  define CRC_DATA_LU(x)               (((x) << CRC_DATA_LU_SHIFT) & CRC_DATA_LU_MASK)
#define CRC_DATA_HL_SHIFT              (16)      /* Bit 16-23: CRC High Lower Byte */
#define CRC_DATA_HL_MASK               (0xFF << CRC_DATA_HL_SHIFT)
#  define CRC_DATA_HL(x)               (((x) << CRC_DATA_HL_SHIFT) & CRC_DATA_HL_MASK)
#define CRC_DATA_HU_SHIFT              (24)      /* Bit 24-31: CRC High Upper Byte */
#define CRC_DATA_HU_MASK               (0xFF << CRC_DATA_HU_SHIFT)
#  define CRC_DATA_HU(x)               (((x) << CRC_DATA_HU_SHIFT) & CRC_DATA_HU_MASK)

/* Polynomial Register Register */

#define CRC_POLY_LOW_SHIFT             (0)       /* Bit 0-15:  CRC Low Lower Byte */
#define CRC_POLY_LOW_MASK              (0xFFFF << CRC_POLY_LOW_SHIFT)
#  define CRC_POLY_LOW(x)              (((x) << CRC_POLY_LOW_SHIFT) & CRC_POLY_LOW_MASK)
#define CRC_POLY_HIGH_SHIFT            (16)      /* Bit 16-31: CRC Low Upper Byte */
#define CRC_POLY_HIGH_MASK             (0xFFFF << CRC_POLY_HIGH_SHIFT)
#  define CRC_POLY_HIGH(x)             (((x) << CRC_POLY_HIGH_SHIFT) & CRC_POLY_HIGH_MASK)

/* Control Register Register */

                                                 /* Bit 0-23:  Reserved */
#define CRC_CR_TCRC                    (1 << 24) /* Bit 24: Width of CRC Protocol */
#define CRC_CR_WAS                     (1 << 25) /* Bit 25: Write CRC Data Register As Seed */
#define CRC_CR_FXOR                    (1 << 26) /* Bit 26: Complement Read of CRC Data Register */
                                                 /* Bit 27: Reserved */
#define CRC_CR_DSR_SHIFT               (28)      /* Bit 28-29: Data Swap for Read */
#define CRC_CR_DSR_MASK                (0x03 << CRC_CR_DSR_SHIFT)
#  define CRC_CR_DSR(x)                (((x) << CRC_CR_DSR_SHIFT) & CRC_CR_DSR_MASK)
#define CRC_CR_DSW_SHIFT               (30)      /* Bit 30-31: Data Swap for Writes */
#define CRC_CR_DSW_MASK                (0x03 << CRC_CR_DSW_SHIFT)
#  define CRC_CR_DSW(x)                (((x) << CRC_CR_DSW_SHIFT) & CRC_CR_DSW_MASK)

#endif /* __VENDOR_FLAGCHIP_CHIPS_FC7300_HARDWARE_FC7300_CRC_H */
