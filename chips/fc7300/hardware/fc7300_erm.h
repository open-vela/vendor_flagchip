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

#ifndef __VENDOR_FLAGCHIP_CHIPS_FC7300_HARDWARE_FC7300_ERM_H
#define __VENDOR_FLAGCHIP_CHIPS_FC7300_HARDWARE_FC7300_ERM_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>
#include "hardware/fc7300_memorymap.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Register offsets *********************************************************/

#define FC7300_ERM_CR0_OFFSET        0x0000  /* Configuration Register 0 */
#define FC7300_ERM_CR1_OFFSET        0x0004  /* Configuration Register 1 */
#define FC7300_ERM_CR2_OFFSET        0x0008  /* Configuration Register 2 */
#define FC7300_ERM_CR3_OFFSET        0x000C  /* Configuration Register 3 */
#define FC7300_ERM_SR0_OFFSET        0x0010  /* Status Register 0 */
#define FC7300_ERM_SR1_OFFSET        0x0014  /* Status Register 1 */
#define FC7300_ERM_SR2_OFFSET        0x0018  /* Status Register 2 */
#define FC7300_ERM_SR3_OFFSET        0x001C  /* Status Register 3 */

#define FC7300_ERM_EAR_OFFSET(n)     (0x0100 + ((n) * 0x10))
#  define FC7300_ERM_EAR0_OFFSET     0x0100  /* Error Address Register: Reserved */
#  define FC7300_ERM_EAR1_OFFSET     0x0110  /* Error Address Register: PFlash0 ECC Error */
#  define FC7300_ERM_EAR2_OFFSET     0x0120  /* Error Address Register: PFlash1 ECC Error */
#  define FC7300_ERM_EAR3_OFFSET     0x0130  /* Error Address Register: DFlash ECC error */
#  define FC7300_ERM_EAR4_OFFSET     0x0140  /* Error Address Register: DMACFG0 ECC Error */
#  define FC7300_ERM_EAR5_OFFSET     0x0150  /* Error Address Register: DMACFG1 ECC Error */
#  define FC7300_ERM_EAR6_OFFSET     0x0160  /* Error Address Register: ROM ECC Error */
#  define FC7300_ERM_EAR7_OFFSET     0x0170  /* Error Address Register: Reserved */
#  define FC7300_ERM_EAR8_OFFSET     0x0180  /* Error Address Register: SysRAM0 ECC ERROR */
#  define FC7300_ERM_EAR9_OFFSET     0x0190  /* Error Address Register: SysRAM1 ECC ERROR */
#  define FC7300_ERM_EAR10_OFFSET    0x01A0  /* Error Address Register: SysRAM2 ECC ERROR */
#  define FC7300_ERM_EAR11_OFFSET    0x01B0  /* Error Address Register: CPU0 ITCM ECC Error */
#  define FC7300_ERM_EAR12_OFFSET    0x01C0  /* Error Address Register: CPU0 DTCM0 ECC Error */
#  define FC7300_ERM_EAR13_OFFSET    0x01D0  /* Error Address Register: CPU0 DTCM1 ECC Error */
#  define FC7300_ERM_EAR14_OFFSET    0x01E0  /* Error Address Register: CPU0 ICACHE ECC Error */
#  define FC7300_ERM_EAR15_OFFSET    0x01F0  /* Error Address Register: CPU0 DCACHE ECC Error */
#  define FC7300_ERM_EAR16_OFFSET    0x0200  /* Error Address Register: CPU1 ITCM ECC Error */
#  define FC7300_ERM_EAR17_OFFSET    0x0210  /* Error Address Register: CPU1 DTCM0 ECC Error */
#  define FC7300_ERM_EAR18_OFFSET    0x0220  /* Error Address Register: CPU1 DTCM1 ECC Error */
#  define FC7300_ERM_EAR19_OFFSET    0x0230  /* Error Address Register: CPU1 ICACHE ECC Error */
#  define FC7300_ERM_EAR20_OFFSET    0x0240  /* Error Address Register: CPU1 DCACHE ECC Error */
#  define FC7300_ERM_EAR21_OFFSET    0x0250  /* Error Address Register: CPU2 ITCM ECC Error */
#  define FC7300_ERM_EAR22_OFFSET    0x0260  /* Error Address Register: CPU2 DTCM0 ECC Error */
#  define FC7300_ERM_EAR23_OFFSET    0x0270  /* Error Address Register: CPU2 DTCM1 ECC Error */
#  define FC7300_ERM_EAR24_OFFSET    0x0280  /* Error Address Register: CPU2 ICACHE ECC Error */
#  define FC7300_ERM_EAR25_OFFSET    0x0290  /* Error Address Register: CPU2 DCACHE ECC Error */
#define FC7300_ERM_EAR_COUNT         26

/* Register bit definitions *************************************************/

/* Configuration Register 0 */

                                                 /* Bit 0-1:  Reserved */
#define ERM_CR0_ENCIE7                 (1 << 2)  /* Bit 2:  Enable Memory 7 Non-Correctable Interrupt Reporting */
#define ERM_CR0_ESCIE7                 (1 << 3)  /* Bit 3:  Enable Memory 7 Single Correction Interrupt Reporting */
                                                 /* Bit 4-5:  Reserved */
#define ERM_CR0_ENCIE6                 (1 << 6)  /* Bit 6:  Enable Memory 6 Non-Correctable Interrupt Reporting */
#define ERM_CR0_ESCIE6                 (1 << 7)  /* Bit 7:  Enable Memory 6 Single Correction Interrupt Reporting */
                                                 /* Bit 8-9:  Reserved */
#define ERM_CR0_ENCIE5                 (1 << 10) /* Bit 10: Enable Memory 5 Non-Correctable Interrupt Reporting */
#define ERM_CR0_ESCIE5                 (1 << 11) /* Bit 11: Enable Memory 5 Single Correction Interrupt Reporting */
                                                 /* Bit 12-13: Reserved */
#define ERM_CR0_ENCIE4                 (1 << 14) /* Bit 14: Enable Memory 4 Non-Correctable Interrupt Reporting */
#define ERM_CR0_ESCIE4                 (1 << 15) /* Bit 15: Enable Memory 4 Single Correction Interrupt Reporting */
                                                 /* Bit 16-17: Reserved */
#define ERM_CR0_ENCIE3                 (1 << 18) /* Bit 18: Enable Memory 3 Non-Correctable Interrupt Reporting */
#define ERM_CR0_ESCIE3                 (1 << 19) /* Bit 19: Enable Memory 3 Single Correction Interrupt Reporting */
                                                 /* Bit 20-21: Reserved */
#define ERM_CR0_ENCIE2                 (1 << 22) /* Bit 22: Enable Memory 2 Non-Correctable Interrupt Reporting */
#define ERM_CR0_ESCIE2                 (1 << 23) /* Bit 23: Enable Memory 2 Single Correction Interrupt Reporting */
                                                 /* Bit 24-25: Reserved */
#define ERM_CR0_ENCIE1                 (1 << 26) /* Bit 26: Enable Memory 1 Non-Correctable Interrupt Reporting */
#define ERM_CR0_ESCIE1                 (1 << 27) /* Bit 27: Enable Memory 1 Single Correction Interrupt Reporting */
                                                 /* Bit 28-29: Reserved */
#define ERM_CR0_ENCIE0                 (1 << 30) /* Bit 30: Enable Memory 0 Non-Correctable Interrupt Reporting */
#define ERM_CR0_ESCIE0                 (1 << 31) /* Bit 31: Enable Memory 0 Single Correction Interrupt Reporting */

/* Configuration Register 1 */

                                                 /* Bit 0-1:  Reserved */
#define ERM_CR1_ENCIE15                (1 << 2)  /* Bit 2:  Enable Memory 15 Non-Correctable Interrupt Reporting */
#define ERM_CR1_ESCIE15                (1 << 3)  /* Bit 3:  Enable Memory 15 Single Correction Interrupt Reporting */
                                                 /* Bit 4-5:  Reserved */
#define ERM_CR1_ENCIE14                (1 << 6)  /* Bit 6:  Enable Memory 14 Non-Correctable Interrupt Reporting */
#define ERM_CR1_ESCIE14                (1 << 7)  /* Bit 7:  Enable Memory 14 Single Correction Interrupt Reporting */
                                                 /* Bit 8-9:  Reserved */
#define ERM_CR1_ENCIE13                (1 << 10) /* Bit 10: Enable Memory 13 Non-Correctable Interrupt Reporting */
#define ERM_CR1_ESCIE13                (1 << 11) /* Bit 11: Enable Memory 13 Single Correction Interrupt Reporting */
                                                 /* Bit 12-13: Reserved */
#define ERM_CR1_ENCIE12                (1 << 14) /* Bit 14: Enable Memory 12 Non-Correctable Interrupt Reporting */
#define ERM_CR1_ESCIE12                (1 << 15) /* Bit 15: Enable Memory 12 Single Correction Interrupt Reporting */
                                                 /* Bit 16-17: Reserved */
#define ERM_CR1_ENCIE11                (1 << 18) /* Bit 18: Enable Memory 11 Non-Correctable Interrupt Reporting */
#define ERM_CR1_ESCIE11                (1 << 19) /* Bit 19: Enable Memory 11 Single Correction Interrupt Reporting */
                                                 /* Bit 20-21: Reserved */
#define ERM_CR1_ENCIE10                (1 << 22) /* Bit 22: Enable Memory 10 Non-Correctable Interrupt Reporting */
#define ERM_CR1_ESCIE10                (1 << 23) /* Bit 23: Enable Memory 10 Single Correction Interrupt Reporting */
                                                 /* Bit 24-25: Reserved */
#define ERM_CR1_ENCIE9                 (1 << 26) /* Bit 26: Enable Memory 9 Non-Correctable Interrupt Reporting */
#define ERM_CR1_ESCIE9                 (1 << 27) /* Bit 27: Enable Memory 9 Single Correction Interrupt Reporting */
                                                 /* Bit 28-29: Reserved */
#define ERM_CR1_ENCIE8                 (1 << 30) /* Bit 30: Enable Memory 8 Non-Correctable Interrupt Reporting */
#define ERM_CR1_ESCIE8                 (1 << 31) /* Bit 31: Enable Memory 8 Single Correction Interrupt Reporting */

/* Configuration Register 2 */

                                                 /* Bit 0-1:  Reserved */
#define ERM_CR2_ENCIE23                (1 << 2)  /* Bit 2:  Enable Memory 23 Non-Correctable Interrupt Reporting */
#define ERM_CR2_ESCIE23                (1 << 3)  /* Bit 3:  Enable Memory 23 Single Correction Interrupt Reporting */
                                                 /* Bit 4-5:  Reserved */
#define ERM_CR2_ENCIE22                (1 << 6)  /* Bit 6:  Enable Memory 22 Non-Correctable Interrupt Reporting */
#define ERM_CR2_ESCIE22                (1 << 7)  /* Bit 7:  Enable Memory 22 Single Correction Interrupt Reporting */
                                                 /* Bit 8-9:  Reserved */
#define ERM_CR2_ENCIE21                (1 << 10) /* Bit 10: Enable Memory 21 Non-Correctable Interrupt Reporting */
#define ERM_CR2_ESCIE21                (1 << 11) /* Bit 11: Enable Memory 21 Single Correction Interrupt Reporting */
                                                 /* Bit 12-13: Reserved */
#define ERM_CR2_ENCIE20                (1 << 14) /* Bit 14: Enable Memory 20 Non-Correctable Interrupt Reporting */
#define ERM_CR2_ESCIE20                (1 << 15) /* Bit 15: Enable Memory 20 Single Correction Interrupt Reporting */
                                                 /* Bit 16-17: Reserved */
#define ERM_CR2_ENCIE19                (1 << 18) /* Bit 18: Enable Memory 19 Non-Correctable Interrupt Reporting */
#define ERM_CR2_ESCIE19                (1 << 19) /* Bit 19: Enable Memory 19 Single Correction Interrupt Reporting */
                                                 /* Bit 20-21: Reserved */
#define ERM_CR2_ENCIE18                (1 << 22) /* Bit 22: Enable Memory 18 Non-Correctable Interrupt Reporting */
#define ERM_CR2_ESCIE18                (1 << 23) /* Bit 23: Enable Memory 18 Single Correction Interrupt Reporting */
                                                 /* Bit 24-25: Reserved */
#define ERM_CR2_ENCIE17                (1 << 26) /* Bit 26: Enable Memory 17 Non-Correctable Interrupt Reporting */
#define ERM_CR2_ESCIE17                (1 << 27) /* Bit 27: Enable Memory 17 Single Correction Interrupt Reporting */
                                                 /* Bit 28-29: Reserved */
#define ERM_CR2_ENCIE16                (1 << 30) /* Bit 30: Enable Memory 16 Non-Correctable Interrupt Reporting */
#define ERM_CR2_ESCIE16                (1 << 31) /* Bit 31: Enable Memory 16 Single Correction Interrupt Reporting */

/* Configuration Register 3 */

                                                 /* Bit 0-1:  Reserved */
#define ERM_CR3_ENCIE31                (1 << 2)  /* Bit 2:  Enable Memory 31 Non-Correctable Interrupt Reporting */
#define ERM_CR3_ESCIE31                (1 << 3)  /* Bit 3:  Enable Memory 31 Single Correction Interrupt Reporting */
                                                 /* Bit 4-5:  Reserved */
#define ERM_CR3_ENCIE30                (1 << 6)  /* Bit 6:  Enable Memory 30 Non-Correctable Interrupt Reporting */
#define ERM_CR3_ESCIE30                (1 << 7)  /* Bit 7:  Enable Memory 30 Single Correction Interrupt Reporting */
                                                 /* Bit 8-9:  Reserved */
#define ERM_CR3_ENCIE29                (1 << 10) /* Bit 10: Enable Memory 29 Non-Correctable Interrupt Reporting */
#define ERM_CR3_ESCIE29                (1 << 11) /* Bit 11: Enable Memory 29 Single Correction Interrupt Reporting */
                                                 /* Bit 12-13: Reserved */
#define ERM_CR3_ENCIE28                (1 << 14) /* Bit 14: Enable Memory 28 Non-Correctable Interrupt Reporting */
#define ERM_CR3_ESCIE28                (1 << 15) /* Bit 15: Enable Memory 28 Single Correction Interrupt Reporting */
                                                 /* Bit 16-17: Reserved */
#define ERM_CR3_ENCIE27                (1 << 18) /* Bit 18: Enable Memory 27 Non-Correctable Interrupt Reporting */
#define ERM_CR3_ESCIE27                (1 << 19) /* Bit 19: Enable Memory 27 Single Correction Interrupt Reporting */
                                                 /* Bit 20-21: Reserved */
#define ERM_CR3_ENCIE26                (1 << 22) /* Bit 22: Enable Memory 26 Non-Correctable Interrupt Reporting */
#define ERM_CR3_ESCIE26                (1 << 23) /* Bit 23: Enable Memory 26 Single Correction Interrupt Reporting */
                                                 /* Bit 24-25: Reserved */
#define ERM_CR3_ENCIE25                (1 << 26) /* Bit 26: Enable Memory 25 Non-Correctable Interrupt Reporting */
#define ERM_CR3_ESCIE25                (1 << 27) /* Bit 27: Enable Memory 25 Single Correction Interrupt Reporting */
                                                 /* Bit 28-29: Reserved */
#define ERM_CR3_ENCIE24                (1 << 30) /* Bit 30: Enable Memory 24 Non-Correctable Interrupt Reporting */
#define ERM_CR3_ESCIE24                (1 << 31) /* Bit 31: Enable Memory 24 Single Correction Interrupt Reporting */

/* Status Register 0 */

                                                 /* Bit 0-1:  Reserved */
#define ERM_SR0_NCE7                   (1 << 2)  /* Bit 2:  Memory 7 Non-Correctable Error Event */
#define ERM_SR0_SBC7                   (1 << 3)  /* Bit 3:  Memory 7 Single-Bit Correction Event */
                                                 /* Bit 4-5:  Reserved */
#define ERM_SR0_NCE6                   (1 << 6)  /* Bit 6:  Memory 6 Non-Correctable Error Event */
#define ERM_SR0_SBC6                   (1 << 7)  /* Bit 7:  Memory 6 Single-Bit Correction Event */
                                                 /* Bit 8-9:  Reserved */
#define ERM_SR0_NCE5                   (1 << 10) /* Bit 10: Memory 5 Non-Correctable Error Event */
#define ERM_SR0_SBC5                   (1 << 11) /* Bit 11: Memory 5 Single-Bit Correction Event */
                                                 /* Bit 12-13: Reserved */
#define ERM_SR0_NCE4                   (1 << 14) /* Bit 14: Memory 4 Non-Correctable Error Event */
#define ERM_SR0_SBC4                   (1 << 15) /* Bit 15: Memory 4 Single-Bit Correction Event */
                                                 /* Bit 16-17: Reserved */
#define ERM_SR0_NCE3                   (1 << 18) /* Bit 18: Memory 3 Non-Correctable Error Event */
#define ERM_SR0_SBC3                   (1 << 19) /* Bit 19: Memory 3 Single-Bit Correction Event */
                                                 /* Bit 20-21: Reserved */
#define ERM_SR0_NCE2                   (1 << 22) /* Bit 22: Memory 2 Non-Correctable Error Event */
#define ERM_SR0_SBC2                   (1 << 23) /* Bit 23: Memory 2 Single-Bit Correction Event */
                                                 /* Bit 24-25: Reserved */
#define ERM_SR0_NCE1                   (1 << 26) /* Bit 26: Memory 1 Non-Correctable Error Event */
#define ERM_SR0_SBC1                   (1 << 27) /* Bit 27: Memory 1 Single-Bit Correction Event */
                                                 /* Bit 28-29: Reserved */
#define ERM_SR0_NCE0                   (1 << 30) /* Bit 30: Memory 0 Non-Correctable Error Event */
#define ERM_SR0_SBC0                   (1 << 31) /* Bit 31: Memory 0 Single-Bit Correction Event */

/* Status Register 1 */

                                                 /* Bit 0-1:  Reserved */
#define ERM_SR1_NCE15                  (1 << 2)  /* Bit 2:  Memory 15 Non-Correctable Error Event */
#define ERM_SR1_SBC15                  (1 << 3)  /* Bit 3:  Memory 15 Single-Bit Correction Event */
                                                 /* Bit 4-5:  Reserved */
#define ERM_SR1_NCE14                  (1 << 6)  /* Bit 6:  Memory 14 Non-Correctable Error Event */
#define ERM_SR1_SBC14                  (1 << 7)  /* Bit 7:  Memory 14 Single-Bit Correction Event */
                                                 /* Bit 8-9:  Reserved */
#define ERM_SR1_NCE13                  (1 << 10) /* Bit 10: Memory 13 Non-Correctable Error Event */
#define ERM_SR1_SBC13                  (1 << 11) /* Bit 11: Memory 13 Single-Bit Correction Event */
                                                 /* Bit 12-13: Reserved */
#define ERM_SR1_NCE12                  (1 << 14) /* Bit 14: Memory 12 Non-Correctable Error Event */
#define ERM_SR1_SBC12                  (1 << 15) /* Bit 15: Memory 12 Single-Bit Correction Event */
                                                 /* Bit 16-17: Reserved */
#define ERM_SR1_NCE11                  (1 << 18) /* Bit 18: Memory 11 Non-Correctable Error Event */
#define ERM_SR1_SBC11                  (1 << 19) /* Bit 19: Memory 11 Single-Bit Correction Event */
                                                 /* Bit 20-21: Reserved */
#define ERM_SR1_NCE10                  (1 << 22) /* Bit 22: Memory 10 Non-Correctable Error Event */
#define ERM_SR1_SBC10                  (1 << 23) /* Bit 23: Memory 10 Single-Bit Correction Event */
                                                 /* Bit 24-25: Reserved */
#define ERM_SR1_NCE9                   (1 << 26) /* Bit 26: Memory 9 Non-Correctable Error Event */
#define ERM_SR1_SBC9                   (1 << 27) /* Bit 27: Memory 9 Single-Bit Correction Event */
                                                 /* Bit 28-29: Reserved */
#define ERM_SR1_NCE8                   (1 << 30) /* Bit 30: Memory 8 Non-Correctable Error Event */
#define ERM_SR1_SBC8                   (1 << 31) /* Bit 31: Memory 8 Single-Bit Correction Event */

/* Status Register 2 */

                                                 /* Bit 0-1:  Reserved */
#define ERM_SR2_NCE23                  (1 << 2)  /* Bit 2:  Memory 23 Non-Correctable Error Event */
#define ERM_SR2_SBC23                  (1 << 3)  /* Bit 3:  Memory 23 Single-Bit Correction Event */
                                                 /* Bit 4-5:  Reserved */
#define ERM_SR2_NCE22                  (1 << 6)  /* Bit 6:  Memory 22 Non-Correctable Error Event */
#define ERM_SR2_SBC22                  (1 << 7)  /* Bit 7:  Memory 22 Single-Bit Correction Event */
                                                 /* Bit 8-9:  Reserved */
#define ERM_SR2_NCE21                  (1 << 10) /* Bit 10: Memory 21 Non-Correctable Error Event */
#define ERM_SR2_SBC21                  (1 << 11) /* Bit 11: Memory 21 Single-Bit Correction Event */
                                                 /* Bit 12-13: Reserved */
#define ERM_SR2_NCE20                  (1 << 14) /* Bit 14: Memory 20 Non-Correctable Error Event */
#define ERM_SR2_SBC20                  (1 << 15) /* Bit 15: Memory 20 Single-Bit Correction Event */
                                                 /* Bit 16-17: Reserved */
#define ERM_SR2_NCE19                  (1 << 18) /* Bit 18: Memory 19 Non-Correctable Error Event */
#define ERM_SR2_SBC19                  (1 << 19) /* Bit 19: Memory 19 Single-Bit Correction Event */
                                                 /* Bit 20-21: Reserved */
#define ERM_SR2_NCE18                  (1 << 22) /* Bit 22: Memory 18 Non-Correctable Error Event */
#define ERM_SR2_SBC18                  (1 << 23) /* Bit 23: Memory 18 Single-Bit Correction Event */
                                                 /* Bit 24-25: Reserved */
#define ERM_SR2_NCE17                  (1 << 26) /* Bit 26: Memory 17 Non-Correctable Error Event */
#define ERM_SR2_SBC17                  (1 << 27) /* Bit 27: Memory 17 Single-Bit Correction Event */
                                                 /* Bit 28-29: Reserved */
#define ERM_SR2_NCE16                  (1 << 30) /* Bit 30: Memory 16 Non-Correctable Error Event */
#define ERM_SR2_SBC16                  (1 << 31) /* Bit 31: Memory 16 Single-Bit Correction Event */

/* Status Register 3 */

                                                 /* Bit 0-1:  Reserved */
#define ERM_SR3_NCE31                  (1 << 2)  /* Bit 2:  Memory 31 Non-Correctable Error Event */
#define ERM_SR3_SBC31                  (1 << 3)  /* Bit 3:  Memory 31 Single-Bit Correction Event */
                                                 /* Bit 4-5:  Reserved */
#define ERM_SR3_NCE30                  (1 << 6)  /* Bit 6:  Memory 30 Non-Correctable Error Event */
#define ERM_SR3_SBC30                  (1 << 7)  /* Bit 7:  Memory 30 Single-Bit Correction Event */
                                                 /* Bit 8-9:  Reserved */
#define ERM_SR3_NCE29                  (1 << 10) /* Bit 10: Memory 29 Non-Correctable Error Event */
#define ERM_SR3_SBC29                  (1 << 11) /* Bit 11: Memory 29 Single-Bit Correction Event */
                                                 /* Bit 12-13: Reserved */
#define ERM_SR3_NCE28                  (1 << 14) /* Bit 14: Memory 28 Non-Correctable Error Event */
#define ERM_SR3_SBC28                  (1 << 15) /* Bit 15: Memory 28 Single-Bit Correction Event */
                                                 /* Bit 16-17: Reserved */
#define ERM_SR3_NCE27                  (1 << 18) /* Bit 18: Memory 27 Non-Correctable Error Event */
#define ERM_SR3_SBC27                  (1 << 19) /* Bit 19: Memory 27 Single-Bit Correction Event */
                                                 /* Bit 20-21: Reserved */
#define ERM_SR3_NCE26                  (1 << 22) /* Bit 22: Memory 26 Non-Correctable Error Event */
#define ERM_SR3_SBC26                  (1 << 23) /* Bit 23: Memory 26 Single-Bit Correction Event */
                                                 /* Bit 24-25: Reserved */
#define ERM_SR3_NCE25                  (1 << 26) /* Bit 26: Memory 25 Non-Correctable Error Event */
#define ERM_SR3_SBC25                  (1 << 27) /* Bit 27: Memory 25 Single-Bit Correction Event */
                                                 /* Bit 28-29: Reserved */
#define ERM_SR3_NCE24                  (1 << 30) /* Bit 30: Memory 24 Non-Correctable Error Event */
#define ERM_SR3_SBC24                  (1 << 31) /* Bit 31: Memory 24 Single-Bit Correction Event */

/* Error Address Register */

#define ERM_EAR_EAR_SHIFT              (0)       /* Bit 0-31: Memory n Error Address */
#define ERM_EAR_EAR_MASK               (0xFFFFFFFF << ERM_EAR_EAR_SHIFT)
#  define ERM_EAR_EAR(x)               (((x) << ERM_EAR_EAR_SHIFT) & ERM_EAR_EAR_MASK)

#endif /* __VENDOR_FLAGCHIP_CHIPS_FC7300_HARDWARE_FC7300_ERM_H */
