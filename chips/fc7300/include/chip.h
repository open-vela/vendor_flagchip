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

#ifndef __VENDOR_FLAGCHIP_CHIPS_FC7300_INCLUDE_CHIP_H
#define __VENDOR_FLAGCHIP_CHIPS_FC7300_INCLUDE_CHIP_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>

/****************************************************************************
 * Pre-processor Prototypes
 ****************************************************************************/

/* Check the FC7300 family configuration.
 * It must be done in vendor/flagchip/chips/fc7300/Kconfig !
 */

#if defined(CONFIG_ARCH_CHIP_FC7300F8M)
#  define FC7300_NPORTS              9   /* PORT A-I */
#  define FC7300_NCRC                2   /* CRC 2*/
#  define FC7300_NDMA                2   /* 2 DMA with 32 channels each */
#  define FC7300_NTRGSEL             4   /* Trigger Select 0-3 */
#  define FC7300_NLU                 2   /* Logic Unit 0-1 */
#  define FC7300_NADC                4   /* 12-bit ADC 0-3, each with 32 channels */
#  define FC7300_NCMP                3   /* 8-bit CMP 0-2, each with 8 channels */
#  define FC7300_NFTU                12  /* 16-bit Flexible Timer Unit 0-11, each with 8 channels */
#  define FC7300_NFCPIT              2   /* 16-bit Programmable Interrupt Timer 0-1 */
#  define FC7300_NRTC                1   /* Real-Time Clock 0 */
#  define FC7300_NAONTIMER           1   /* Always-on Timer 0 */
#  define FC7300_NPTIMER             4   /* Programmable Timer 0-3 */
#  define FC7300_NFCSPI              8   /* FCSPI 0-7 */
#  define FC7300_NIIC                2   /* Inter-Integrated Circuit 0-1 */
#  define FC7300_NFCUART             18  /* Universal Asynchronous Receiver/Transmitter 0-17 */
#  define FC7300_NFLEXCAN            10  /* FLEX Control Area Network 0-9 */
#  define FC7300_NFLEXCANFD          10  /* FLEXCAN 0-9 supports CAN FD */
#  define FC7300_NHSM                1   /* Hardware Secure Module 0 */
#  define FC7300_NWDOG               3   /* Watchdog 0-2 */
#  define FC7300_NCMU                5   /* Clock Monitor Unit 0-4 */
#  define FC7300_NISM                1   /* Interface Satety Monitor 0 */

#else
#  error "Unrecognized FC7300 chip"
#endif

/* NVIC priority levels *****************************************************/

/* Each priority field holds a priority value. The lower the value, the
 * greater the priority of the corresponding interrupt.
 */

/* FC7300 series supports 8 programmable interrupt priority levels. */

#define NVIC_SYSH_PRIORITY_MIN     0xe0 /* All bits set in minimum priority */
#define NVIC_SYSH_PRIORITY_DEFAULT 0x80 /* Midpoint is the default */
#define NVIC_SYSH_PRIORITY_MAX     0x00 /* Zero is maximum priority */
#define NVIC_SYSH_PRIORITY_STEP    0x20 /* Steps between priorities */

/****************************************************************************
 * Public Types
 ****************************************************************************/

/****************************************************************************
 * Public Data
 ****************************************************************************/

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

#endif /* __ARCH_ARM_INCLUDE_S32K1XX_CHIP_H */
