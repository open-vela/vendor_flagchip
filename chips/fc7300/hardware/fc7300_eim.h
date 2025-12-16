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

#ifndef __VENDOR_FLAGCHIP_CHIPS_FC7300_HARDWARE_FC7300_EIM_H
#define __VENDOR_FLAGCHIP_CHIPS_FC7300_HARDWARE_FC7300_EIM_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>
#include "hardware/fc7300_memorymap.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Register offsets *********************************************************/

#define FC7300_EIM_CR_OFFSET              0x0000  /* Configuration Register */

#define FC7300_EIM_CTRL_REG_OFFSET(n)     (0x0010 + ((n) * 0x4))
#  define FC7300_EIM_CTRL_REG0_OFFSET     0x0010  /* Channel Control Register: SCM Matrix Access Monitor Error Injection0 (MAM0_S0) */
#  define FC7300_EIM_CTRL_REG1_OFFSET     0x0014  /* Channel Control Register: SCM Matrix Access Monitor Error Injection1 (MAM0_S1) */
#  define FC7300_EIM_CTRL_REG2_OFFSET     0x0018  /* Channel Control Register: SCM Matrix Access Monitor Error Injection2 (MAM0_S2) */
#  define FC7300_EIM_CTRL_REG3_OFFSET     0x001C  /* Channel Control Register: SCM Matrix Access Monitor Error Injection3 (MAM0_S3) */
#  define FC7300_EIM_CTRL_REG4_OFFSET     0x0020  /* Channel Control Register: SCM Matrix Access Monitor Error Injection4 (MAM1_S0) */
#  define FC7300_EIM_CTRL_REG5_OFFSET     0x0024  /* Channel Control Register: SCM Matrix Access Monitor Error Injection5 (MAM1_S1) */
#  define FC7300_EIM_CTRL_REG6_OFFSET     0x0028  /* Channel Control Register: SCM Matrix Access Monitor Error Injection6 (MAM1_S2) */
#  define FC7300_EIM_CTRL_REG7_OFFSET     0x002C  /* Channel Control Register: SCM Matrix Access Monitor Error Injection7 (MAM1_S3) */
#  define FC7300_EIM_CTRL_REG8_OFFSET     0x0030  /* Channel Control Register: SCM Matrix Access Monitor Error Injection8 (MAM2_S0) */
#  define FC7300_EIM_CTRL_REG9_OFFSET     0x0034  /* Channel Control Register: SCM Matrix Access Monitor Error Injection9 (MAM2_S1) */
#  define FC7300_EIM_CTRL_REG10_OFFSET    0x0038  /* Channel Control Register: SCM Matrix Access Monitor Error Injection10 (MAM2_S2) */
#  define FC7300_EIM_CTRL_REG11_OFFSET    0x003C  /* Channel Control Register: SCM Matrix Access Monitor Error Injection11 (CPU0_AHBM) */
#  define FC7300_EIM_CTRL_REG12_OFFSET    0x0040  /* Channel Control Register: SCM Matrix Access Monitor Error Injection12 (CPU0_AHBP) */
#  define FC7300_EIM_CTRL_REG13_OFFSET    0x0044  /* Channel Control Register: SCM Matrix Access Monitor Error Injection13 (CPU1_AHBM) */
#  define FC7300_EIM_CTRL_REG14_OFFSET    0x0048  /* Channel Control Register: SCM Matrix Access Monitor Error Injection14 (CPU1_AHBP) */
#  define FC7300_EIM_CTRL_REG15_OFFSET    0x004C  /* Channel Control Register: SCM Matrix Access Monitor Error Injection15 (CPU2_AHBM) */
#  define FC7300_EIM_CTRL_REG16_OFFSET    0x0050  /* Channel Control Register: SCM Matrix Access Monitor Error Injection16 (CPU2_AHBP) */
#  define FC7300_EIM_CTRL_REG17_OFFSET    0x0054  /* Channel Control Register: SCM Matrix Access Monitor Error Injection17 (ENET) */
#  define FC7300_EIM_CTRL_REG18_OFFSET    0x0058  /* Channel Control Register: SCM Matrix Access Monitor Error Injection18 (HSM) */
#  define FC7300_EIM_CTRL_REG19_OFFSET    0x005C  /* Channel Control Register: SCM Matrix Access Monitor Error Injection19 (DMA0) */
#  define FC7300_EIM_CTRL_REG20_OFFSET    0x0060  /* Channel Control Register: SCM Matrix Access Monitor Error Injection20 (DMA1) */
#  define FC7300_EIM_CTRL_REG21_OFFSET    0x0064  /* Channel Control Register: SCM Matrix Access Monitor Error Injection21 (CPU0_AHBS) */
#  define FC7300_EIM_CTRL_REG22_OFFSET    0x0068  /* Channel Control Register: SCM Matrix Access Monitor Error Injection22 (CPU1_AHBS) */
#  define FC7300_EIM_CTRL_REG23_OFFSET    0x006C  /* Channel Control Register: SCM Matrix Access Monitor Error Injection23 (CPU2_AHBS) */
#  define FC7300_EIM_CTRL_REG24_OFFSET    0x0070  /* Channel Control Register: CPU0_ITCM */
#  define FC7300_EIM_CTRL_REG25_OFFSET    0x0074  /* Channel Control Register: CPU0_DTCM0 */
#  define FC7300_EIM_CTRL_REG26_OFFSET    0x0078  /* Channel Control Register: CPU0_DTCM1 */
#  define FC7300_EIM_CTRL_REG27_OFFSET    0x007C  /* Channel Control Register: CPU1_ITCM */
#  define FC7300_EIM_CTRL_REG28_OFFSET    0x0080  /* Channel Control Register: CPU1_DTCM0 */
#  define FC7300_EIM_CTRL_REG29_OFFSET    0x0084  /* Channel Control Register: CPU1_DTCM1 */
#  define FC7300_EIM_CTRL_REG30_OFFSET    0x0088  /* Channel Control Register: CPU2_ITCM */
#  define FC7300_EIM_CTRL_REG31_OFFSET    0x008C  /* Channel Control Register: CPU2_DTCM0 */
#  define FC7300_EIM_CTRL_REG32_OFFSET    0x0090  /* Channel Control Register: CPU2_DTCM1 */
#  define FC7300_EIM_CTRL_REG33_OFFSET    0x0094  /* Channel Control Register: SRAM0_INJECTION */
#  define FC7300_EIM_CTRL_REG34_OFFSET    0x0098  /* Channel Control Register: SRAM1_INJECTION */
#  define FC7300_EIM_CTRL_REG35_OFFSET    0x009C  /* Channel Control Register: SRAM2_INJECTION */
#  define FC7300_EIM_CTRL_REG36_OFFSET    0x00A0  /* Channel Control Register: CPU0_DCACHE_DATA0_01 Injection */
#  define FC7300_EIM_CTRL_REG37_OFFSET    0x00A4  /* Channel Control Register: CPU0_DCACHE_DATA0_23 Injection */
#  define FC7300_EIM_CTRL_REG38_OFFSET    0x00A8  /* Channel Control Register: CPU0_DCACHE_DATA1_01 Injection */
#  define FC7300_EIM_CTRL_REG39_OFFSET    0x00AC  /* Channel Control Register: CPU0_DCACHE_DATA1_23 Injection */
#  define FC7300_EIM_CTRL_REG40_OFFSET    0x00B0  /* Channel Control Register: CPU0_DCACHE_TAG_01 Injection */
#  define FC7300_EIM_CTRL_REG41_OFFSET    0x00B4  /* Channel Control Register: CPU0_DCACHE_TAG_23 Injection */
#  define FC7300_EIM_CTRL_REG42_OFFSET    0x00B8  /* Channel Control Register: CPU0_ICACHE_DATA0 Injection */
#  define FC7300_EIM_CTRL_REG43_OFFSET    0x00BC  /* Channel Control Register: CPU0_ICACHE_DATA1 Injection */
#  define FC7300_EIM_CTRL_REG44_OFFSET    0x00C0  /* Channel Control Register: CPU0_ICACHE_TAG Injection */
#  define FC7300_EIM_CTRL_REG45_OFFSET    0x00C4  /* Channel Control Register: CPU1_DCACHE_DATA0_01 Injection */
#  define FC7300_EIM_CTRL_REG46_OFFSET    0x00C8  /* Channel Control Register: CPU1_DCACHE_DATA0_23 Injection */
#  define FC7300_EIM_CTRL_REG47_OFFSET    0x00CC  /* Channel Control Register: CPU1_DCACHE_DATA1_01 Injection */
#  define FC7300_EIM_CTRL_REG48_OFFSET    0x00D0  /* Channel Control Register: CPU1_DCACHE_DATA1_23 Injection */
#  define FC7300_EIM_CTRL_REG49_OFFSET    0x00D4  /* Channel Control Register: CPU1_DCACHE_TAG_01 Injection */
#  define FC7300_EIM_CTRL_REG50_OFFSET    0x00D8  /* Channel Control Register: CPU1_DCACHE_TAG_23 Injection */
#  define FC7300_EIM_CTRL_REG51_OFFSET    0x00DC  /* Channel Control Register: CPU1_ICACHE_DATA0 Injection */
#  define FC7300_EIM_CTRL_REG52_OFFSET    0x00E0  /* Channel Control Register: CPU1_ICACHE_DATA1 Injection */
#  define FC7300_EIM_CTRL_REG53_OFFSET    0x00E4  /* Channel Control Register: CPU1_ICACHE_TAG Injection */
#  define FC7300_EIM_CTRL_REG54_OFFSET    0x00E8  /* Channel Control Register: CPU2_DCACHE_DATA0_01 Injection */
#  define FC7300_EIM_CTRL_REG55_OFFSET    0x00EC  /* Channel Control Register: CPU2_DCACHE_DATA0_23 Injection */
#  define FC7300_EIM_CTRL_REG56_OFFSET    0x00F0  /* Channel Control Register: CPU2_DCACHE_DATA1_01 Injection */
#  define FC7300_EIM_CTRL_REG57_OFFSET    0x00F4  /* Channel Control Register: CPU2_DCACHE_DATA1_23 Injection */
#  define FC7300_EIM_CTRL_REG58_OFFSET    0x00F8  /* Channel Control Register: CPU2_DCACHE_TAG_01 Injection */
#  define FC7300_EIM_CTRL_REG59_OFFSET    0x00FC  /* Channel Control Register: CPU2_DCACHE_TAG_23 Injection */
#  define FC7300_EIM_CTRL_REG60_OFFSET    0x0100  /* Channel Control Register: CPU2_ICACHE_DATA0 Injection */
#  define FC7300_EIM_CTRL_REG61_OFFSET    0x0104  /* Channel Control Register: CPU2_ICACHE_DATA1 Injection */
#  define FC7300_EIM_CTRL_REG62_OFFSET    0x0108  /* Channel Control Register: CPU2_ICACHE_TAG Injection */
#  define FC7300_EIM_CTRL_REG63_OFFSET    0x010C  /* Channel Control Register: SCM MAM1_S3 Slave DownSize Error Injection */
#  define FC7300_EIM_CTRL_REG64_OFFSET    0x0110  /* Channel Control Register: SCM MAM1_S3 Master DownSize Error Injection */
#  define FC7300_EIM_CTRL_REG65_OFFSET    0x0114  /* Channel Control Register: DMA0_CFG_ECC Injection */
#  define FC7300_EIM_CTRL_REG66_OFFSET    0x0118  /* Channel Control Register: DMA1_CFG_ECC Injection */
#  define FC7300_EIM_CTRL_REG67_OFFSET    0x011C  /* Channel Control Register: ROM_ECC Injection */
#  define FC7300_EIM_CTRL_REG68_OFFSET    0x0120  /* Channel Control Register: SCM MAM1_S3 Monitor Error Injection */
#  define FC7300_EIM_CTRL_REG69_OFFSET    0x0124  /* Channel Control Register: SCM CPU0 AHBM0 and AHBM1 Monitor Error Injection */
#  define FC7300_EIM_CTRL_REG70_OFFSET    0x0128  /* Channel Control Register: SCM CPU0 AHBP and AHBS Monitor Error Injection */
#  define FC7300_EIM_CTRL_REG71_OFFSET    0x012C  /* Channel Control Register: SCM CPU1 AHBM0 and AHBM1 Monitor Error Injection */
#  define FC7300_EIM_CTRL_REG72_OFFSET    0x0130  /* Channel Control Register: SCM CPU1 AHBP and AHBS Monitor Error Injection */
#  define FC7300_EIM_CTRL_REG73_OFFSET    0x0134  /* Channel Control Register: SCM CPU2 AHBM0 and AHBM1 Monitor Error Injection */
#  define FC7300_EIM_CTRL_REG74_OFFSET    0x0138  /* Channel Control Register: SCM CPU2 AHBP and AHBS Monitor Error Injection */
#  define FC7300_EIM_CTRL_REG75_OFFSET    0x013C  /* Channel Control Register: SCM MAM2_S2 Slave DownSize Error Injection */
#  define FC7300_EIM_CTRL_REG76_OFFSET    0x0140  /* Channel Control Register: SCM MAM2_S2 Master DownSize Error Injection */
#  define FC7300_EIM_CTRL_REG77_OFFSET    0x0147  /* Channel Control Register: RAM Decoder Monitor Error Injection */
#define FC7300_EIM_CTRL_REG_COUNT         78

#define FC7300_EIM_CPU_LOCKSTEP_OFFSET(n) (0x0400 + ((n) * 0x4))
#  define FC7300_EIM_CPU0_LOCKSTEP_OFFSET 0x0400  /* CPU0 LOCKSTEP Error Injection Register */
#  define FC7300_EIM_CPU1_LOCKSTEP_OFFSET 0x0404  /* CPU1 LOCKSTEP Error Injection Register */
#define FC7300_EIM_CPU_LOCKSTEP_REG_COUNT 2

#define FC7300_EIM_BUS_REG_OFFSET(n)      (0x0800 + ((n) * 0x4))
#  define FC7300_EIM_BUS_REG0_OFFSET      0x0800  /* Bus0 Register */
#  define FC7300_EIM_BUS_REG1_OFFSET      0x0804  /* Bus1 Register */
#  define FC7300_EIM_BUS_REG2_OFFSET      0x0808  /* Bus2 Register */
#  define FC7300_EIM_BUS_REG3_OFFSET      0x080C  /* Bus3 Register */
#define FC7300_EIM_BUS_REG_COUNT          4

/* Register bit definitions *************************************************/

/* Configuration Register */

#define EIM_CR_GEIEN                        (1 << 0)  /* Bit 0:  Global Error Injection Enable */
                                                      /* Bit 1-31: Reserved */

/* Channel N Control Register */

#define EIM_CTRL_REG_DATA0EIE               (1 << 0)  /* Bit 0:  Channel N data0 error injection with CR[GEIEN] field must also be asserted */
#define EIM_CTRL_REG_DATA1EIE               (1 << 1)  /* Bit 1:  Channel N data1 error injection with CR[GEIEN] field must also be asserted */
#define EIM_CTRL_REG_ADDREIE                (1 << 2)  /* Bit 2:  Channel N address error injection with CR[GEIEN] field must also be asserted */
#define EIM_CTRL_REG_ATTREIE                (1 << 3)  /* Bit 3:  Channel N attribute error injection with CR[GEIEN] field must also be asserted */
#define EIM_CTRL_REG_BUS_SEL_SHIFT          (4)       /* Bit 4-5:  Error Injection Bus Select */
#define EIM_CTRL_REG_BUS_SEL_MASK           (0x03 << EIM_CTRL_REG_BUS_SEL_SHIFT)
#  define EIM_CTRL_REG_BUS_SEL(x)           (((x) << EIM_CTRL_REG_BUS_SEL_SHIFT) & EIM_CTRL_REG_BUS_SEL_MASK)
                                                      /* Bit 6-27: Reserved */
#define EIM_CTRL_REG_DWP_SHIFT              (28)      /* Bit 28-30: Domain Write Protection */
#define EIM_CTRL_REG_DWP_MASK               (0x07 << EIM_CTRL_REG_DWP_SHIFT)
#  define EIM_CTRL_REG_DWP(x)               (((x) << EIM_CTRL_REG_DWP_SHIFT) & EIM_CTRL_REG_DWP_MASK)
#define EIM_CTRL_REG_DWP_LOCK               (1 << 31) /* Bit 31: Domain Write Protection Lock */

/* CPU n LOCKSTEP Error Injection Register */

#define EIM_CPU_LOCKSTEP_LOCKSTEP_MON1_CLR  (1 << 0)  /* Bit 0:  CPU n Lockstep Monitor1 Error Clear */
#define EIM_CPU_LOCKSTEP_LOCKSTEP_MON0_CLR  (1 << 1)  /* Bit 1:  CPU n Lockstep Monitor0 Error Clear */
#define EIM_CPU_LOCKSTEP_LOCKSTEP_MON1_SET  (1 << 2)  /* Bit 2:  CPU n Lockstep Monitor1 Error Injection Set */
#define EIM_CPU_LOCKSTEP_LOCKSTEP_MON0_SET  (1 << 3)  /* Bit 3:  CPU n Lockstep Monitor0 Error Injection Set */
                                                      /* Bit 4-27: Reserved */
#define EIM_CPU_LOCKSTEP_DWP_SHIFT          (28)      /* Bit 28-30: Domain Write Protection */
#define EIM_CPU_LOCKSTEP_DWP_MASK           (0x07 << EIM_CPU_LOCKSTEP_DWP_SHIFT)
#  define EIM_CPU_LOCKSTEP_DWP(x)           (((x) << EIM_CPU_LOCKSTEP_DWP_SHIFT) & EIM_CPU_LOCKSTEP_DWP_MASK)
#define EIM_CPU_LOCKSTEP_DWP_LOCK           (1 << 31) /* Bit 31: Domain Write Protection Lock */

/* Bus Register */

#define EIM_BUS_REG_DATA0_SHIFT             (0)       /* Bit 0-6:  Data0 Single Bit Error Injection Position */
#define EIM_BUS_REG_DATA0_MASK              (0x7F << EIM_BUS_REG_DATA0_SHIFT)
#  define EIM_BUS_REG_DATA0(x)              (((x) << EIM_BUS_REG_DATA0_SHIFT) & EIM_BUS_REG_DATA0_MASK)
                                                      /* Bit 7:  Reserved */
#define EIM_BUS_REG_DATA1_SHIFT             (8)      /* Bit 8-14: Data1 Single Bit Error Injection Position */
#define EIM_BUS_REG_DATA1_MASK              (0x7F << EIM_BUS_REG_DATA1_SHIFT)
#  define EIM_BUS_REG_DATA1(x)              (((x) << EIM_BUS_REG_DATA1_SHIFT) & EIM_BUS_REG_DATA1_MASK)
                                                      /* Bit 15: Reserved */
#define EIM_BUS_REG_ADDR_SHIFT              (16)      /* Bit 16-20: Address Single Bit Error Injection Position */
#define EIM_BUS_REG_ADDR_MASK               (0x1F << EIM_BUS_REG_ADDR_SHIFT)
#  define EIM_BUS_REG_ADDR(x)               (((x) << EIM_BUS_REG_ADDR_SHIFT) & EIM_BUS_REG_ADDR_MASK)
                                                      /* Bit 21-23: Reserved */
#define EIM_BUS_REG_ATTR_SHIFT              (24)      /* Bit 24-28: Attribute Single Bit Error Injection Position */
#define EIM_BUS_REG_ATTR_MASK               (0x1F << EIM_BUS_REG_ATTR_SHIFT)
#  define EIM_BUS_REG_ATTR(x)               (((x) << EIM_BUS_REG_ATTR_SHIFT) & EIM_BUS_REG_ATTR_MASK)
                                                      /* Bit 29-31: Reserved */

#endif /* __VENDOR_FLAGCHIP_CHIPS_FC7300_HARDWARE_FC7300_EIM_H */
