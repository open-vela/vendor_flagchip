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

#ifndef __VENDOR_FLAGCHIP_CHIPS_FC7300_HARDWARE_FC7300_ISM_H
#define __VENDOR_FLAGCHIP_CHIPS_FC7300_HARDWARE_FC7300_ISM_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>
#include <hardware/fc7300_memorymap.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* ISM Register Offsets */

#define FC7300_ISM_PARAM_OFFSET        0x0000  /* Parameter Register */
#define FC7300_ISM_CTRL_OFFSET         0x0004  /* Control Register */
#define FC7300_ISM_E_STATUS_OFFSET     0x0008  /* Event Status Register */
#define FC7300_ISM_E_CTRL_OFFSET       0x000c  /* Event Control Register */
#define FC7300_ISM_EC_CTRL_OFFSET      0x0010  /* Event Control Mode Control Register */
#define FC7300_ISM_FPC_STATUS0_OFFSET  0x100   /* FPC Status */
#define FC7300_ISM_FPC_CTRL0_OFFSET    0x104   /* FPC Control Register */
#define FC7300_ISM_FPC_CONFIG0_OFFSET  0x108   /* FPC Configure Register */
#define FC7300_ISM_FPC_TIMER0_OFFSET   0x10c   /* FPC Timer Register */
#define FC7300_ISM_FPC_STATUS1_OFFSET  0x110   /* FPC Status Register */
#define FC7300_ISM_FPC_CTRL1_OFFSET    0x114   /* FPC Control Register */
#define FC7300_ISM_FPC_CONFIG1_OFFSET  0x118   /* FPC Configure Register */
#define FC7300_ISM_FPC_TIMER1_OFFSET   0x11c   /* FPC Timer Register */
#define FC7300_ISM_FPC_STATUS2_OFFSET  0x120   /* FPC Status Register */
#define FC7300_ISM_FPC_CTRL2_OFFSET    0x124   /* FPC Control Register */
#define FC7300_ISM_FPC_CONFIG2_OFFSET  0x128   /* FPC Configure Register */
#define FC7300_ISM_FPC_TIMER2_OFFSET   0x12c   /* FPC Timer Register */
#define FC7300_ISM_FPC_STATUS3_OFFSET  0x130   /* FPC Status Register */
#define FC7300_ISM_FPC_CTRL3_OFFSET    0x134   /* FPC Control Register */
#define FC7300_ISM_FPC_CONFIG3_OFFSET  0x138   /* FPC Configure Register */
#define FC7300_ISM_FPC_TIMER3_OFFSET   0x13c   /* FPC Timer Register */
#define FC7300_ISM_FPC_STATUS4_OFFSET  0x140   /* FPC Status Register */
#define FC7300_ISM_FPC_CTRL4_OFFSET    0x144   /* FPC Control Register */
#define FC7300_ISM_FPC_CONFIG4_OFFSET  0x148   /* FPC Configure Register */
#define FC7300_ISM_FPC_TIMER4_OFFSET   0x14c   /* FPC Timer Register */
#define FC7300_ISM_FPC_STATUS5_OFFSET  0x150   /* FPC Status Register */
#define FC7300_ISM_FPC_CTRL5_OFFSET    0x154   /* FPC Control Register */
#define FC7300_ISM_FPC_CONFIG5_OFFSET  0x158   /* FPC Configure Register */
#define FC7300_ISM_FPC_TIMER5_OFFSET   0x15c   /* FPC Timer Register */
#define FC7300_ISM_FPC_STATUS6_OFFSET  0x160   /* FPC Status Register */
#define FC7300_ISM_FPC_CTRL6_OFFSET    0x164   /* FPC Control Register */
#define FC7300_ISM_FPC_CONFIG6_OFFSET  0x168   /* FPC Configure Register */
#define FC7300_ISM_FPC_TIMER6_OFFSET   0x16c   /* FPC Timer Register */
#define FC7300_ISM_FPC_STATUS7_OFFSET  0x170   /* FPC Status Register */
#define FC7300_ISM_FPC_CTRL7_OFFSET    0x174   /* FPC Control Register */
#define FC7300_ISM_FPC_CONFIG7_OFFSET  0x178   /* FPC Configure Register */
#define FC7300_ISM_FPC_TIMER7_OFFSET   0x17c   /* FPC Timer Register */
#define FC7300_ISM_FPC_STATUS8_OFFSET  0x180   /* FPC Status Register */
#define FC7300_ISM_FPC_CTRL8_OFFSET    0x184   /* FPC Control Register */
#define FC7300_ISM_FPC_CONFIG8_OFFSET  0x188   /* FPC Configure Register */
#define FC7300_ISM_FPC_TIMER8_OFFSET   0x18c   /* FPC Timer Register */
#define FC7300_ISM_FPC_STATUS9_OFFSET  0x190   /* FPC Status Register */
#define FC7300_ISM_FPC_CTRL9_OFFSET    0x194   /* FPC Control Register */
#define FC7300_ISM_FPC_CONFIG9_OFFSET  0x198   /* FPC Configure Register */
#define FC7300_ISM_FPC_TIMER9_OFFSET   0x19c   /* FPC Timer Register */
#define FC7300_ISM_FPC_STATUS10_OFFSET 0x1a0   /* FPC Status Register */
#define FC7300_ISM_FPC_CTRL10_OFFSET   0x1a4   /* FPC Control Register */
#define FC7300_ISM_FPC_CONFIG10_OFFSET 0x1a8   /* FPC Configure Register */
#define FC7300_ISM_FPC_TIMER10_OFFSET  0x1ac   /* FPC Timer Register */
#define FC7300_ISM_FPC_STATUS11_OFFSET 0x1b0   /* FPC Status Register */
#define FC7300_ISM_FPC_CTRL11_OFFSET   0x1b4   /* FPC Control Register */
#define FC7300_ISM_FPC_CONFIG11_OFFSET 0x1b8   /* FPC Configure Register */
#define FC7300_ISM_FPC_TIMER11_OFFSET  0x1bc   /* FPC Timer Register */
#define FC7300_ISM_FPC_STATUS12_OFFSET 0x1c0   /* FPC Status Register */
#define FC7300_ISM_FPC_CTRL12_OFFSET   0x1c4   /* FPC Control Register */
#define FC7300_ISM_FPC_CONFIG12_OFFSET 0x1c8   /* FPC Configure Register */
#define FC7300_ISM_FPC_TIMER12_OFFSET  0x1cc   /* FPC Timer Register */
#define FC7300_ISM_FPC_STATUS13_OFFSET 0x1d0   /* FPC Status Register */
#define FC7300_ISM_FPC_CTRL13_OFFSET   0x1d4   /* FPC Control Register */
#define FC7300_ISM_FPC_CONFIG13_OFFSET 0x1d8   /* FPC Configure Register */
#define FC7300_ISM_FPC_TIMER13_OFFSET  0x1dc   /* FPC Timer Register */
#define FC7300_ISM_FPC_STATUS14_OFFSET 0x1e0   /* FPC Status Register */
#define FC7300_ISM_FPC_CTRL14_OFFSET   0x1e4   /* FPC Control Register */
#define FC7300_ISM_FPC_CONFIG14_OFFSET 0x1e8   /* FPC Configure Register */
#define FC7300_ISM_FPC_TIMER14_OFFSET  0x1ec   /* FPC Timer Register */
#define FC7300_ISM_FPC_STATUS15_OFFSET 0x1f0   /* FPC Status Register */
#define FC7300_ISM_FPC_CTRL15_OFFSET   0x1f4   /* FPC Control Register */
#define FC7300_ISM_FPC_CONFIG15_OFFSET 0x1f8   /* FPC Configure Register */
#define FC7300_ISM_FPC_TIMER15_OFFSET  0x1fc   /* FPC Timer Register */
#define FC7300_ISM_FPC_STATUS16_OFFSET 0x200   /* FPC Status Register */
#define FC7300_ISM_FPC_CTRL16_OFFSET   0x204   /* FPC Control Register */
#define FC7300_ISM_FPC_CONFIG16_OFFSET 0x208   /* FPC Configure Register */
#define FC7300_ISM_FPC_TIMER16_OFFSET  0x20c   /* FPC Timer Register */
#define FC7300_ISM_FPC_STATUS17_OFFSET 0x210   /* FPC Status Register */
#define FC7300_ISM_FPC_CTRL17_OFFSET   0x214   /* FPC Control Register */
#define FC7300_ISM_FPC_CONFIG17_OFFSET 0x218   /* FPC Configure Register */
#define FC7300_ISM_FPC_TIMER17_OFFSET  0x21c   /* FPC Timer Register */
#define FC7300_ISM_FPC_STATUS18_OFFSET 0x220   /* FPC Status Register */
#define FC7300_ISM_FPC_CTRL18_OFFSET   0x224   /* FPC Control Register */
#define FC7300_ISM_FPC_CONFIG18_OFFSET 0x228   /* FPC Configure Register */
#define FC7300_ISM_FPC_TIMER18_OFFSET  0x22c   /* FPC Timer Register */
#define FC7300_ISM_FPC_STATUS19_OFFSET 0x230   /* FPC Status Register */
#define FC7300_ISM_FPC_CTRL19_OFFSET   0x234   /* FPC Control Register */
#define FC7300_ISM_FPC_CONFIG19_OFFSET 0x238   /* FPC Configure Register */
#define FC7300_ISM_FPC_TIMER19_OFFSET  0x23c   /* FPC Timer Register */
#define FC7300_ISM_FPC_STATUS20_OFFSET 0x240   /* FPC Status Register */
#define FC7300_ISM_FPC_CTRL20_OFFSET   0x244   /* FPC Control Register */
#define FC7300_ISM_FPC_CONFIG20_OFFSET 0x248   /* FPC Configure Register */
#define FC7300_ISM_FPC_TIMER20_OFFSET  0x24c   /* FPC Timer Register */
#define FC7300_ISM_FPC_STATUS21_OFFSET 0x250   /* FPC Status Register */
#define FC7300_ISM_FPC_CTRL21_OFFSET   0x254   /* FPC Control Register */
#define FC7300_ISM_FPC_CONFIG21_OFFSET 0x258   /* FPC Configure Register */
#define FC7300_ISM_FPC_TIMER21_OFFSET  0x25c   /* FPC Timer Register */
#define FC7300_ISM_FPC_STATUS22_OFFSET 0x260   /* FPC Status Register */
#define FC7300_ISM_FPC_CTRL22_OFFSET   0x264   /* FPC Control Register */
#define FC7300_ISM_FPC_CONFIG22_OFFSET 0x268   /* FPC Configure Register */
#define FC7300_ISM_FPC_TIMER22_OFFSET  0x26c   /* FPC Timer Register */
#define FC7300_ISM_FPC_STATUS23_OFFSET 0x270   /* FPC Status Register */
#define FC7300_ISM_FPC_CTRL23_OFFSET   0x274   /* FPC Control Register */
#define FC7300_ISM_FPC_CONFIG23_OFFSET 0x278   /* FPC Configure Register */
#define FC7300_ISM_FPC_TIMER23_OFFSET  0x27c   /* FPC Timer Register */
#define FC7300_ISM_FPC_STATUS24_OFFSET 0x280   /* FPC Status Register */
#define FC7300_ISM_FPC_CTRL24_OFFSET   0x284   /* FPC Control Register */
#define FC7300_ISM_FPC_CONFIG24_OFFSET 0x288   /* FPC Configure Register */
#define FC7300_ISM_FPC_TIMER24_OFFSET  0x28c   /* FPC Timer Register */
#define FC7300_ISM_FPC_STATUS25_OFFSET 0x290   /* FPC Status Register */
#define FC7300_ISM_FPC_CTRL25_OFFSET   0x294   /* FPC Control Register */
#define FC7300_ISM_FPC_CONFIG25_OFFSET 0x298   /* FPC Configure Register */
#define FC7300_ISM_FPC_TIMER25_OFFSET  0x29c   /* FPC Timer Register */
#define FC7300_ISM_FPC_STATUS26_OFFSET 0x2a0   /* FPC Status Register */
#define FC7300_ISM_FPC_CTRL26_OFFSET   0x2a4   /* FPC Control Register */
#define FC7300_ISM_FPC_CONFIG26_OFFSET 0x2a8   /* FPC Configure Register */
#define FC7300_ISM_FPC_TIMER26_OFFSET  0x2ac   /* FPC Timer Register */
#define FC7300_ISM_FPC_STATUS27_OFFSET 0x2b0   /* FPC Status Register */
#define FC7300_ISM_FPC_CTRL27OFFSET    0x2b4   /* FPC Control Register */
#define FC7300_ISM_FPC_CONFIG27_OFFSET 0x2b8   /* FPC Configure Register */
#define FC7300_ISM_FPC_TIMER27_OFFSET  0x2bc   /* FPC Timer Register */
#define FC7300_ISM_FPC_STATUS28_OFFSET 0x2c0   /* FPC Status Register */
#define FC7300_ISM_FPC_CTRL28_OFFSET   0x2c4   /* FPC Control Register */
#define FC7300_ISM_FPC_CONFIG28_OFFSET 0x2c8   /* FPC Configure Register */
#define FC7300_ISM_FPC_TIMER28_OFFSET  0x2cc   /* FPC Timer Register */
#define FC7300_ISM_FPC_STATUS29_OFFSET 0x2d0   /* FPC Status Register */
#define FC7300_ISM_FPC_CTRL29_OFFSET   0x2d4   /* FPC Control Register */
#define FC7300_ISM_FPC_CONFIG29_OFFSET 0x2d8   /* FPC Configure Register */
#define FC7300_ISM_FPC_TIMER29_OFFSET  0x2dc   /* FPC Timer Register */
#define FC7300_ISM_FPC_STATUS30_OFFSET 0x2e0   /* FPC Status Register */
#define FC7300_ISM_FPC_CTRL30_OFFSET   0x2e4   /* FPC Control Register */
#define FC7300_ISM_FPC_CONFIG30_OFFSET 0x2e8   /* FPC Configure Register */
#define FC7300_ISM_FPC_TIMER30_OFFSET  0x2ec   /* FPC Timer Register */
#define FC7300_ISM_FPC_STATUS31_OFFSET 0x2f0   /* FPC Status Register */
#define FC7300_ISM_FPC_CTRL31_OFFSET   0x2f4   /* FPC Control Register */
#define FC7300_ISM_FPC_CONFIG31_OFFSET 0x2f8   /* FPC Configure Register */
#define FC7300_ISM_FPC_TIMER31_OFFSET  0x2fc   /* FPC Timer Register */
#define FC7300_ISM_FPC_STATUSn_OFFSET(n)  (0x100 + 0x10 * n)   /* FPC Status Register */
#define FC7300_ISM_FPC_CTRLn_OFFSET(n)    (0x104 + 0x10 * n)   /* FPC Control Register */
#define FC7300_ISM_FPC_CONFIGn_OFFSET(n)  (0x108 + 0x10 * n)   /* FPC Configure Register */
#define FC7300_ISM_FPC_TIMERn_OFFSET(n)   (0x10c + 0x10 * n)   /* FPC Timer Register */

#define FC7300_ISM_LAM_STATUS0_OFFSET  0x400 /* LAM Status Register */
#define FC7300_ISM_LAM_CTRL0_OFFSET    0x404 /* LAM Control Register */
#define FC7300_ISM_LAM_CONFIG0_OFFSET  0x408 /* LAM Configure Register */
#define FC7300_ISM_LAM_COUNTER0_OFFSET 0x40c /* LAM Counter Register */

#define FC7300_ISM_LAM_STATUS1_OFFSET  0x410 /* LAM Status Register */
#define FC7300_ISM_LAM_CTRL1_OFFSET    0x414 /* LAM Control Register */
#define FC7300_ISM_LAM_CONFIG1_OFFSET  0x418 /* LAM Configure Register */
#define FC7300_ISM_LAM_COUNTER1_OFFSET 0x41c /* LAM Counter Register */

#define FC7300_ISM_LAM_STATUS2_OFFSET  0x420 /* LAM Status Register */
#define FC7300_ISM_LAM_CTRL2_OFFSET    0x424 /* LAM Control Register */
#define FC7300_ISM_LAM_CONFIG2_OFFSET  0x428 /* LAM Configure Register */
#define FC7300_ISM_LAM_COUNTER2_OFFSET 0x42c /* LAM Counter Register */

#define FC7300_ISM_LAM_STATUS3_OFFSET  0x430 /* LAM Status Register */
#define FC7300_ISM_LAM_CTRL3_OFFSET    0x434 /* LAM Control Register */
#define FC7300_ISM_LAM_CONFIG3_OFFSET  0x438 /* LAM Configure Register */
#define FC7300_ISM_LAM_COUNTER3_OFFSET 0x43c /* LAM Counter Register */

#define FC7300_ISM_LAM_STATUS4_OFFSET  0x440 /* LAM Status Register */
#define FC7300_ISM_LAM_CTRL4_OFFSET    0x444 /* LAM Control Register */
#define FC7300_ISM_LAM_CONFIG4_OFFSET  0x448 /* LAM Configure Register */
#define FC7300_ISM_LAM_COUNTER4_OFFSET 0x44c /* LAM Counter Register */

#define FC7300_ISM_LAM_STATUS5_OFFSET  0x450 /* LAM Status Register */
#define FC7300_ISM_LAM_CTRL5_OFFSET    0x454 /* LAM Control Register */
#define FC7300_ISM_LAM_CONFIG5_OFFSET  0x458 /* LAM Configure Register */
#define FC7300_ISM_LAM_COUNTER5_OFFSET 0x45c /* LAM Counter Register */

#define FC7300_ISM_LAM_STATUS6_OFFSET  0x460 /* LAM Status Register */
#define FC7300_ISM_LAM_CTRL6_OFFSET    0x464 /* LAM Control Register */
#define FC7300_ISM_LAM_CONFIG6_OFFSET  0x468 /* LAM Configure Register */
#define FC7300_ISM_LAM_COUNTER6_OFFSET 0x46c /* LAM Counter Register */

#define FC7300_ISM_LAM_STATUS7_OFFSET  0x470 /* LAM Status Register */
#define FC7300_ISM_LAM_CTRL7_OFFSET    0x474 /* LAM Control Register */
#define FC7300_ISM_LAM_CONFIG7_OFFSET  0x478 /* LAM Configure Register */
#define FC7300_ISM_LAM_COUNTER7_OFFSET 0x47c /* LAM Counter Register */

#define FC7300_ISM_LAM_STATUS8_OFFSET  0x480 /* LAM Status Register */
#define FC7300_ISM_LAM_CTRL8_OFFSET    0x484 /* LAM Control Register */
#define FC7300_ISM_LAM_CONFIG8_OFFSET  0x488 /* LAM Configure Register */
#define FC7300_ISM_LAM_COUNTER8_OFFSET 0x48c /* LAM Counter Register */

#define FC7300_ISM_LAM_STATUS9_OFFSET  0x490 /* LAM Status Register */
#define FC7300_ISM_LAM_CTRL9_OFFSET    0x494 /* LAM Control Register */
#define FC7300_ISM_LAM_CONFIG9_OFFSET  0x498 /* LAM Configure Register */
#define FC7300_ISM_LAM_COUNTER9_OFFSET 0x49c /* LAM Counter Register */

#define FC7300_ISM_LAM_STATUS10_OFFSET  0x4a0 /* LAM Status Register */
#define FC7300_ISM_LAM_CTRL10 _OFFSET   0x4a4 /* LAM Control Register */
#define FC7300_ISM_LAM_CONFIG10_OFFSET  0x4a8 /* LAM Configure Register */
#define FC7300_ISM_LAM_COUNTER10_OFFSET 0x4ac /* LAM Counter Register */

#define FC7300_ISM_LAM_STATUS11_OFFSET  0x4b0 /* LAM Status Register */
#define FC7300_ISM_LAM_CTRL11_OFFSET    0x4b4 /* LAM Control Register */
#define FC7300_ISM_LAM_CONFIG11_OFFSET  0x4b8 /* LAM Configure Register */
#define FC7300_ISM_LAM_COUNTER11_OFFSET 0x4bc /* LAM Counter Register */

#define FC7300_ISM_LAM_STATUS12_OFFSET  0x4c0 /* LAM Status Register */
#define FC7300_ISM_LAM_CTRL12_OFFSET    0x4c4 /* LAM Control Register */
#define FC7300_ISM_LAM_CONFIG12_OFFSET  0x4c8 /* LAM Configure Register */
#define FC7300_ISM_LAM_COUNTER12_OFFSET 0x4cc /* LAM Counter Register */

#define FC7300_ISM_LAM_STATUS13_OFFSET  0x4d0 /* LAM Status Register */
#define FC7300_ISM_LAM_CTRL13_OFFSET    0x4d4 /* LAM Control Register */
#define FC7300_ISM_LAM_CONFIG13_OFFSET  0x4d8 /* LAM Configure Register */
#define FC7300_ISM_LAM_COUNTER13_OFFSET 0x4dc /* LAM Counter Register */

#define FC7300_ISM_LAM_STATUS14_OFFSET  0x4e0 /* LAM Status Register */
#define FC7300_ISM_LAM_CTRL14_OFFSET    0x4e4 /* LAM Control Register */
#define FC7300_ISM_LAM_CONFIG14_OFFSET  0x4e8 /* LAM Configure Register */
#define FC7300_ISM_LAM_COUNTER14_OFFSET 0x4ec /* LAM Counter Register */

#define FC7300_ISM_LAM_STATUS15_OFFSET  0x4f0 /* LAM Status Register */
#define FC7300_ISM_LAM_CTRL15_OFFSET    0x4f4 /* LAM Control Register */
#define FC7300_ISM_LAM_CONFIG15_OFFSET  0x4f8 /* LAM Configure Register */
#define FC7300_ISM_LAM_COUNTER15_OFFSET 0x4fc /* LAM Counter Register */

#define FC7300_ISM_LAM_STATUSn_OFFSET(n)  (0x400 + (0x10 * n))   /* LAM Status Register */
#define FC7300_ISM_LAM_CTRLn_OFFSET(n)    (0x404 + (0x10 * n))   /* LAM Control Register */
#define FC7300_ISM_LAM_CONFIGn_OFFSET(n)  (0x408 + (0x10 * n))   /* LAM Configure Register */
#define FC7300_ISM_LAM_COUNTERn_OFFSET(n) (0x40c + (0x10 * n))   /* LAM Counter Register */

/* ISM Register Addresses **************************************************/

#define FC7300_ISM_PARAM          (FC7300_ISM_BASE + FC7300_ISM_PARAM_OFFSET)
#define FC7300_ISM_CTRL           (FC7300_ISM_BASE + FC7300_ISM_CTRL_OFFSET)
#define FC7300_ISM_E_STATUS       (FC7300_ISM_BASE + FC7300_ISM_E_STATUS_OFFSET)
#define FC7300_ISM_E_CTRL         (FC7300_ISM_BASE + FC7300_ISM_E_CTRL_OFFSET)
#define FC7300_ISM_EC_CTRL        (FC7300_ISM_BASE + FC7300_ISM_EC_CTRL_OFFSET)

#define FC7300_ISM_FPC_STATUS(n)  (FC7300_ISM_BASE + FC7300_ISM_FPC_STATUSn_OFFSET(n))
#define FC7300_ISM_FPC_CTRL(n)    (FC7300_ISM_BASE + FC7300_ISM_FPC_CTRLn_OFFSET(n))
#define FC7300_ISM_FPC_CONFIG(n)  (FC7300_ISM_BASE + FC7300_ISM_FPC_CONFIGn_OFFSET(n))
#define FC7300_ISM_FPC_TIMER(n)   (FC7300_ISM_BASE + FC7300_ISM_FPC_TIMERn_OFFSET(n))

#define FC7300_ISM_LAM_STATUS(n)  (FC7300_ISM_BASE + FC7300_ISM_LAM_STATUSn_OFFSET(n))
#define FC7300_ISM_LAM_CTRL(n)    (FC7300_ISM_BASE + FC7300_ISM_LAM_CTRLn_OFFSET(n))
#define FC7300_ISM_LAM_CONFIG(n)  (FC7300_ISM_BASE + FC7300_ISM_LAM_CONFIGn_OFFSET(n))
#define FC7300_ISM_LAM_COUNTER(n) (FC7300_ISM_BASE + FC7300_ISM_LAM_COUNTERn_OFFSET(n))

#define FC7300_ISM_ECM_CHANNEL(x) (1<<(x))
#define FC7300_ISM_LAM_CHANNEL(x) (1<<(x))
#define FC7300_ISM_FPC_CHANNEL(x) (1<<(x))

/* ISM Register Bitfield Definitions ***************************************/

/* PARAM Bit Fields */

#define FC7300_ISM_PARAM_LAM_MASK             0x1F000000u
#define FC7300_ISM_PARAM_LAM_SHIFT            24u
#define FC7300_ISM_PARAM_LAM_WIDTH            5u
#define FC7300_ISM_PARAM_LAM(x)               (((uint32_t)(((uint32_t)(x))<<FC7300_ISM_PARAM_LAM_SHIFT))&FC7300_ISM_PARAM_LAM_MASK)

#define FC7300_ISM_PARAM_FPC_MASK             0x3F0000u
#define FC7300_ISM_PARAM_FPC_SHIFT            16u
#define FC7300_ISM_PARAM_FPC_WIDTH            6u
#define FC7300_ISM_PARAM_FPC(x)               (((uint32_t)(((uint32_t)(x))<<FC7300_ISM_PARAM_FPC_SHIFT))&FC7300_ISM_PARAM_FPC_MASK)

#define FC7300_ISM_PARAM_ECMC_MASK            0xFu
#define FC7300_ISM_PARAM_ECMC_SHIFT           0u
#define FC7300_ISM_PARAM_ECMC_WIDTH           4u
#define FC7300_ISM_PARAM_ECMC(x)              (((uint32_t)(((uint32_t)(x))<<FC7300_ISM_PARAM_ECMC_SHIFT))&FC7300_ISM_PARAM_ECMC_MASK)

/* PARAM Reg Mask */

#define FC7300_ISM_PARAM_MASK                 0x1F3F000Fu

/* CTRL Bit Fields */

#define FC7300_ISM_CTRL_EN_MASK               0x80000000u
#define FC7300_ISM_CTRL_EN_SHIFT              31u
#define FC7300_ISM_CTRL_EN_WIDTH              1u
#define FC7300_ISM_CTRL_EN(x)                 (((uint32_t)(((uint32_t)(x))<<FC7300_ISM_CTRL_EN_SHIFT))&FC7300_ISM_CTRL_EN_MASK)

#define FC7300_ISM_CTRL_IEN_MASK              0x1u
#define FC7300_ISM_CTRL_IEN_SHIFT             0u
#define FC7300_ISM_CTRL_IEN_WIDTH             1u
#define FC7300_ISM_CTRL_IEN(x)                (((uint32_t)(((uint32_t)(x))<<FC7300_ISM_CTRL_IEN_SHIFT))&FC7300_ISM_CTRL_IEN_MASK)

/* CTRL Reg Mask */

#define FC7300_ISM_CTRL_MASK                  0x80000001u

/* E_STATUS Bit Fields */

#define FC7300_ISM_E_STATUS_ECS_MASK          0xF0000u
#define FC7300_ISM_E_STATUS_ECS_SHIFT         16u
#define FC7300_ISM_E_STATUS_ECS_WIDTH         4u
#define FC7300_ISM_E_STATUS_ECS(x)            (((uint32_t)(((uint32_t)(x))<<FC7300_ISM_E_STATUS_ECS_SHIFT))&FC7300_ISM_E_STATUS_ECS_MASK)

#define FC7300_ISM_E_STATUS_ES_MASK           0xFFFFu
#define FC7300_ISM_E_STATUS_ES_SHIFT          0u
#define FC7300_ISM_E_STATUS_ES_WIDTH          16u
#define FC7300_ISM_E_STATUS_ES(x)             (((uint32_t)(((uint32_t)(x))<<FC7300_ISM_E_STATUS_ES_SHIFT))&FC7300_ISM_E_STATUS_ES_MASK)

/* E_STATUS Reg Mask */

#define FC7300_ISM_E_STATUS_MASK              0x000FFFFFu

/* E_CTRL Bit Fields */

#define FC7300_ISM_E_CTRL_ECE_MASK            0xF0000u
#define FC7300_ISM_E_CTRL_ECE_SHIFT           16u
#define FC7300_ISM_E_CTRL_ECE_WIDTH           4u
#define FC7300_ISM_E_CTRL_ECE(x)              (((uint32_t)(((uint32_t)(x))<<FC7300_ISM_E_CTRL_ECE_SHIFT))&FC7300_ISM_E_CTRL_ECE_MASK)

#define FC7300_ISM_E_CTRL_EE_MASK             0xFFFFu
#define FC7300_ISM_E_CTRL_EE_SHIFT            0u
#define FC7300_ISM_E_CTRL_EE_WIDTH            16u
#define FC7300_ISM_E_CTRL_EE(x)               (((uint32_t)(((uint32_t)(x))<<FC7300_ISM_E_CTRL_EE_SHIFT))&FC7300_ISM_E_CTRL_EE_MASK)

/* E_CTRL Reg Mask */

#define FC7300_ISM_E_CTRL_MASK                0x000FFFFFu

/* EC_CTRL Bit Fields */

#define FC7300_ISM_EC_CTRL_THRL_3_MASK        0xF0000000u
#define FC7300_ISM_EC_CTRL_THRL_3_SHIFT       28u
#define FC7300_ISM_EC_CTRL_THRL_3_WIDTH       4u
#define FC7300_ISM_EC_CTRL_THRL_3(x)          (((uint32_t)(((uint32_t)(x))<<FC7300_ISM_EC_CTRL_THRL_3_SHIFT))&FC7300_ISM_EC_CTRL_THRL_3_MASK)

#define FC7300_ISM_EC_CTRL_SEL_3_MASK         0xF000000u
#define FC7300_ISM_EC_CTRL_SEL_3_SHIFT        24u
#define FC7300_ISM_EC_CTRL_SEL_3_WIDTH        4u
#define FC7300_ISM_EC_CTRL_SEL_3(x)           (((uint32_t)(((uint32_t)(x))<<FC7300_ISM_EC_CTRL_SEL_3_SHIFT))&FC7300_ISM_EC_CTRL_SEL_3_MASK)

#define FC7300_ISM_EC_CTRL_THRL_2_MASK        0xF00000u
#define FC7300_ISM_EC_CTRL_THRL_2_SHIFT       20u
#define FC7300_ISM_EC_CTRL_THRL_2_WIDTH       4u
#define FC7300_ISM_EC_CTRL_THRL_2(x)          (((uint32_t)(((uint32_t)(x))<<FC7300_ISM_EC_CTRL_THRL_2_SHIFT))&FC7300_ISM_EC_CTRL_THRL_2_MASK)

#define FC7300_ISM_EC_CTRL_SEL_2_MASK         0xF0000u
#define FC7300_ISM_EC_CTRL_SEL_2_SHIFT        16u
#define FC7300_ISM_EC_CTRL_SEL_2_WIDTH        4u
#define FC7300_ISM_EC_CTRL_SEL_2(x)           (((uint32_t)(((uint32_t)(x))<<FC7300_ISM_EC_CTRL_SEL_2_SHIFT))&FC7300_ISM_EC_CTRL_SEL_2_MASK)

#define FC7300_ISM_EC_CTRL_THRL_1_MASK        0xF000u
#define FC7300_ISM_EC_CTRL_THRL_1_SHIFT       12u
#define FC7300_ISM_EC_CTRL_THRL_1_WIDTH       4u
#define FC7300_ISM_EC_CTRL_THRL_1(x)          (((uint32_t)(((uint32_t)(x))<<FC7300_ISM_EC_CTRL_THRL_1_SHIFT))&FC7300_ISM_EC_CTRL_THRL_1_MASK)

#define FC7300_ISM_EC_CTRL_SEL_1_MASK         0xF00u
#define FC7300_ISM_EC_CTRL_SEL_1_SHIFT        8u
#define FC7300_ISM_EC_CTRL_SEL_1_WIDTH        4u
#define FC7300_ISM_EC_CTRL_SEL_1(x)           (((uint32_t)(((uint32_t)(x))<<FC7300_ISM_EC_CTRL_SEL_1_SHIFT))&FC7300_ISM_EC_CTRL_SEL_1_MASK)

#define FC7300_ISM_EC_CTRL_THRL_0_MASK        0xF0u
#define FC7300_ISM_EC_CTRL_THRL_0_SHIFT       4u
#define FC7300_ISM_EC_CTRL_THRL_0_WIDTH       4u
#define FC7300_ISM_EC_CTRL_THRL_0(x)          (((uint32_t)(((uint32_t)(x))<<FC7300_ISM_EC_CTRL_THRL_0_SHIFT))&FC7300_ISM_EC_CTRL_THRL_0_MASK)

#define FC7300_ISM_EC_CTRL_SEL_0_MASK         0xFu
#define FC7300_ISM_EC_CTRL_SEL_0_SHIFT        0u
#define FC7300_ISM_EC_CTRL_SEL_0_WIDTH        4u
#define FC7300_ISM_EC_CTRL_SEL_0(x)           (((uint32_t)(((uint32_t)(x))<<FC7300_ISM_EC_CTRL_SEL_0_SHIFT))&FC7300_ISM_EC_CTRL_SEL_0_MASK)

/* EC_CTRL Reg Mask */

#define FC7300_ISM_EC_CTRL_MASK               0xFFFFFFFFu

/* FPC_STATUS Bit Fields */

#define FC7300_ISM_FPC_STATUS_RGD_MASK        0x2u
#define FC7300_ISM_FPC_STATUS_RGD_SHIFT       1u
#define FC7300_ISM_FPC_STATUS_RGD_WIDTH       1u
#define FC7300_ISM_FPC_STATUS_RGD(x)          (((uint32_t)(((uint32_t)(x))<<FC7300_ISM_FPC_STATUS_RGD_SHIFT))&FC7300_ISM_FPC_STATUS_RGD_MASK)

#define FC7300_ISM_FPC_STATUS_FGD_MASK        0x1u
#define FC7300_ISM_FPC_STATUS_FGD_SHIFT       0u
#define FC7300_ISM_FPC_STATUS_FGD_WIDTH       1u
#define FC7300_ISM_FPC_STATUS_FGD(x)          (((uint32_t)(((uint32_t)(x))<<FC7300_ISM_FPC_STATUS_FGD_SHIFT))&FC7300_ISM_FPC_STATUS_FGD_MASK)

/* FPC_STATUS0 Reg Mask */

#define FC7300_ISM_FPC_STATUS_MASK            0x00000003u

/* FPC_CTRL Bit Fields */

#define FC7300_ISM_FPC_CTRL_IEN_MASK          0x2u
#define FC7300_ISM_FPC_CTRL_IEN_SHIFT         1u
#define FC7300_ISM_FPC_CTRL_IEN_WIDTH         1u
#define FC7300_ISM_FPC_CTRL_IEN(x)            (((uint32_t)(((uint32_t)(x))<<FC7300_ISM_FPC_CTRL_IEN_SHIFT))&FC7300_ISM_FPC_CTRL_IEN_MASK)

#define FC7300_ISM_FPC_CTRL_EN_MASK           0x1u
#define FC7300_ISM_FPC_CTRL_EN_SHIFT          0u
#define FC7300_ISM_FPC_CTRL_EN_WIDTH          1u
#define FC7300_ISM_FPC_CTRL_EN(x)             (((uint32_t)(((uint32_t)(x))<<FC7300_ISM_FPC_CTRL_EN_SHIFT))&FC7300_ISM_FPC_CTRL_EN_MASK)

/* FPC_CTRL0 Reg Mask */

#define FC7300_ISM_FPC_CTRL_MASK              0x00000003u

/* FPC_CONFIG Bit Fields */

#define FC7300_ISM_FPC_CONFIG_FEG_MASK        0xC00000u
#define FC7300_ISM_FPC_CONFIG_FEG_SHIFT       22u
#define FC7300_ISM_FPC_CONFIG_FEG_WIDTH       2u
#define FC7300_ISM_FPC_CONFIG_FEG(x)          (((uint32_t)(((uint32_t)(x))<<FC7300_ISM_FPC_CONFIG_FEG_SHIFT))&FC7300_ISM_FPC_CONFIG_FEG_MASK)

#define FC7300_ISM_FPC_CONFIG_FED_MASK        0x300000u
#define FC7300_ISM_FPC_CONFIG_FED_SHIFT       20u
#define FC7300_ISM_FPC_CONFIG_FED_WIDTH       2u
#define FC7300_ISM_FPC_CONFIG_FED(x)          (((uint32_t)(((uint32_t)(x))<<FC7300_ISM_FPC_CONFIG_FED_SHIFT))&FC7300_ISM_FPC_CONFIG_FED_MASK)

#define FC7300_ISM_FPC_CONFIG_REG_MASK        0xC0000u
#define FC7300_ISM_FPC_CONFIG_REG_SHIFT       18u
#define FC7300_ISM_FPC_CONFIG_REG_WIDTH       2u
#define FC7300_ISM_FPC_CONFIG_REG(x)          (((uint32_t)(((uint32_t)(x))<<FC7300_ISM_FPC_CONFIG_REG_SHIFT))&FC7300_ISM_FPC_CONFIG_REG_MASK)

#define FC7300_ISM_FPC_CONFIG_RED_MASK        0x30000u
#define FC7300_ISM_FPC_CONFIG_RED_SHIFT       16u
#define FC7300_ISM_FPC_CONFIG_RED_WIDTH       2u
#define FC7300_ISM_FPC_CONFIG_RED(x)          (((uint32_t)(((uint32_t)(x))<<FC7300_ISM_FPC_CONFIG_RED_SHIFT))&FC7300_ISM_FPC_CONFIG_RED_MASK)

#define FC7300_ISM_FPC_CONFIG_CMP_MASK        0xFFFFu
#define FC7300_ISM_FPC_CONFIG_CMP_SHIFT       0u
#define FC7300_ISM_FPC_CONFIG_CMP_WIDTH       16u
#define FC7300_ISM_FPC_CONFIG_CMP(x)          (((uint32_t)(((uint32_t)(x))<<FC7300_ISM_FPC_CONFIG_CMP_SHIFT))&FC7300_ISM_FPC_CONFIG_CMP_MASK)

/* FPC_CONFIG0 Reg Mask */

#define FC7300_ISM_FPC_CONFIG_MASK            0x00FFFFFFu

/* FPC_TIMER Bit Fields */

#define FC7300_ISM_FPC_TIMER_TIM_MASK         0xFFFFu
#define FC7300_ISM_FPC_TIMER_TIM_SHIFT        0u
#define FC7300_ISM_FPC_TIMER_TIM_WIDTH        16u
#define FC7300_ISM_FPC_TIMER_TIM(x)           (((uint32_t)(((uint32_t)(x))<<FC7300_ISM_FPC_TIMER_TIM_SHIFT))&FC7300_ISM_FPC_TIMER_TIM_MASK)

/* FPC_TIMER0 Reg Mask */

#define FC7300_ISM_FPC_TIMER_MASK             0x0000FFFFu

/* LAM_STATUS Bit Fields */

#define FC7300_ISM_LAM_STATUS_OVFL_MASK       0x80000000u
#define FC7300_ISM_LAM_STATUS_OVFL_SHIFT      31u
#define FC7300_ISM_LAM_STATUS_OVFL_WIDTH      1u
#define FC7300_ISM_LAM_STATUS_OVFL(x)         (((uint32_t)(((uint32_t)(x))<<FC7300_ISM_LAM_STATUS_OVFL_SHIFT))&FC7300_ISM_LAM_STATUS_OVFL_MASK)

#define FC7300_ISM_LAM_STATUS_COUNT_MASK      0xFFFFFFu
#define FC7300_ISM_LAM_STATUS_COUNT_SHIFT     0u
#define FC7300_ISM_LAM_STATUS_COUNT_WIDTH     24u
#define FC7300_ISM_LAM_STATUS_COUNT(x)        (((uint32_t)(((uint32_t)(x))<<FC7300_ISM_LAM_STATUS_COUNT_SHIFT))&FC7300_ISM_LAM_STATUS_COUNT_MASK)

/* LAM_STATUS0 Reg Mask */

#define FC7300_ISM_LAM_STATUS_MASK            0x80FFFFFFu

/* LAM_CTRL Bit Fields */

#define FC7300_ISM_LAM_CTRL_IEN_MASK          0x2u
#define FC7300_ISM_LAM_CTRL_IEN_SHIFT         1u
#define FC7300_ISM_LAM_CTRL_IEN_WIDTH         1u
#define FC7300_ISM_LAM_CTRL_IEN(x)            (((uint32_t)(((uint32_t)(x))<<FC7300_ISM_LAM_CTRL_IEN_SHIFT))&FC7300_ISM_LAM_CTRL_IEN_MASK)

#define FC7300_ISM_LAM_CTRL_EN_MASK           0x1u
#define FC7300_ISM_LAM_CTRL_EN_SHIFT          0u
#define FC7300_ISM_LAM_CTRL_EN_WIDTH          1u
#define FC7300_ISM_LAM_CTRL_EN(x)             (((uint32_t)(((uint32_t)(x))<<FC7300_ISM_LAM_CTRL_EN_SHIFT))&FC7300_ISM_LAM_CTRL_EN_MASK)

/* LAM_CTRL0 Reg Mask */

#define FC7300_ISM_LAM_CTRL_MASK              0x00000003u

/* LAM_CONFIG Bit Fields */

#define FC7300_ISM_LAM_CONFIG_RCS_MASK        0x1F000000u
#define FC7300_ISM_LAM_CONFIG_RCS_SHIFT       24u
#define FC7300_ISM_LAM_CONFIG_RCS_WIDTH       5u
#define FC7300_ISM_LAM_CONFIG_RCS(x)          (((uint32_t)(((uint32_t)(x))<<FC7300_ISM_LAM_CONFIG_RCS_SHIFT))&FC7300_ISM_LAM_CONFIG_RCS_MASK)

#define FC7300_ISM_LAM_CONFIG_MCS_MASK        0x1F0000u
#define FC7300_ISM_LAM_CONFIG_MCS_SHIFT       16u
#define FC7300_ISM_LAM_CONFIG_MCS_WIDTH       5u
#define FC7300_ISM_LAM_CONFIG_MCS(x)          (((uint32_t)(((uint32_t)(x))<<FC7300_ISM_LAM_CONFIG_MCS_SHIFT))&FC7300_ISM_LAM_CONFIG_MCS_MASK)

#define FC7300_ISM_LAM_CONFIG_IVW_MASK        0x1000u
#define FC7300_ISM_LAM_CONFIG_IVW_SHIFT       12u
#define FC7300_ISM_LAM_CONFIG_IVW_WIDTH       1u
#define FC7300_ISM_LAM_CONFIG_IVW(x)          (((uint32_t)(((uint32_t)(x))<<FC7300_ISM_LAM_CONFIG_IVW_SHIFT))&FC7300_ISM_LAM_CONFIG_IVW_MASK)

#define FC7300_ISM_LAM_CONFIG_EDS_MASK        0xF00u
#define FC7300_ISM_LAM_CONFIG_EDS_SHIFT       8u
#define FC7300_ISM_LAM_CONFIG_EDS_WIDTH       4u
#define FC7300_ISM_LAM_CONFIG_EDS(x)          (((uint32_t)(((uint32_t)(x))<<FC7300_ISM_LAM_CONFIG_EDS_SHIFT))&FC7300_ISM_LAM_CONFIG_EDS_MASK)

#define FC7300_ISM_LAM_CONFIG_EWS_MASK        0x10u
#define FC7300_ISM_LAM_CONFIG_EWS_SHIFT       4u
#define FC7300_ISM_LAM_CONFIG_EWS_WIDTH       1u
#define FC7300_ISM_LAM_CONFIG_EWS(x)          (((uint32_t)(((uint32_t)(x))<<FC7300_ISM_LAM_CONFIG_EWS_SHIFT))&FC7300_ISM_LAM_CONFIG_EWS_MASK)

#define FC7300_ISM_LAM_CONFIG_RMS_MASK        0x8u
#define FC7300_ISM_LAM_CONFIG_RMS_SHIFT       3u
#define FC7300_ISM_LAM_CONFIG_RMS_WIDTH       1u
#define FC7300_ISM_LAM_CONFIG_RMS(x)          (((uint32_t)(((uint32_t)(x))<<FC7300_ISM_LAM_CONFIG_RMS_SHIFT))&FC7300_ISM_LAM_CONFIG_RMS_MASK)

#define FC7300_ISM_LAM_CONFIG_MOS_MASK        0x4u
#define FC7300_ISM_LAM_CONFIG_MOS_SHIFT       2u
#define FC7300_ISM_LAM_CONFIG_MOS_WIDTH       1u
#define FC7300_ISM_LAM_CONFIG_MOS(x)          (((uint32_t)(((uint32_t)(x))<<FC7300_ISM_LAM_CONFIG_MOS_SHIFT))&FC7300_ISM_LAM_CONFIG_MOS_MASK)

#define FC7300_ISM_LAM_CONFIG_IVM_MASK        0x2u
#define FC7300_ISM_LAM_CONFIG_IVM_SHIFT       1u
#define FC7300_ISM_LAM_CONFIG_IVM_WIDTH       1u
#define FC7300_ISM_LAM_CONFIG_IVM(x)          (((uint32_t)(((uint32_t)(x))<<FC7300_ISM_LAM_CONFIG_IVM_SHIFT))&FC7300_ISM_LAM_CONFIG_IVM_MASK)

#define FC7300_ISM_LAM_CONFIG_IVR_MASK        0x1u
#define FC7300_ISM_LAM_CONFIG_IVR_SHIFT       0u
#define FC7300_ISM_LAM_CONFIG_IVR_WIDTH       1u
#define FC7300_ISM_LAM_CONFIG_IVR(x)          (((uint32_t)(((uint32_t)(x))<<FC7300_ISM_LAM_CONFIG_IVR_SHIFT))&FC7300_ISM_LAM_CONFIG_IVR_MASK)

/* LAM_CONFIG0 Reg Mask */

#define FC7300_ISM_LAM_CONFIG_MASK            0x1F1F1F1Fu

/* LAM_COUNTER Bit Fields */

#define FC7300_ISM_LAM_COUNTER_CNT_MASK       0xFFFFFFu
#define FC7300_ISM_LAM_COUNTER_CNT_SHIFT      0u
#define FC7300_ISM_LAM_COUNTER_CNT_WIDTH      24u
#define FC7300_ISM_LAM_COUNTER_CNT(x)         (((uint32_t)(((uint32_t)(x))<<FC7300_ISM_LAM_COUNTER_CNT_SHIFT))&FC7300_ISM_LAM_COUNTER_CNT_MASK)

/* LAM_COUNTER0 Reg Mask */

#define FC7300_ISM_LAM_COUNTER_MASK           0x00FFFFFFu

#endif /* __VENDOR_FLAGCHIP_CHIPS_FC7300_HARDWARE_FC7300_ISM_H */
