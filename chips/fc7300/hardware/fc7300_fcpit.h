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

#ifndef __VENDOR_FLAGCHIP_CHIPS_FC7300_HARDWARE_FC7300_FCPIT_H
#define __VENDOR_FLAGCHIP_CHIPS_FC7300_HARDWARE_FC7300_FCPIT_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>
#include "hardware/fc7300_memorymap.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Register Offsets *********************************************************/

/* Basic register offset */

#define FC7300_FCPIT_MCR_OFFSET            0x0008  /* Module control register, offset: 0x8 */
#define FC7300_FCPIT_MSR_OFFSET            0x000C  /* Module status register, offset: 0xC */
#define FC7300_FCPIT_MIER_OFFSET           0x0010  /* Module interrupt enable register, offset: 0x10 */
#define FC7300_FCPIT_SETTEN_OFFSET         0x0014  /* Set timer enable register, offset: 0x14 */
#define FC7300_FCPIT_CLRTEN_OFFSET         0x0018  /* Clear timer enable register, offset: 0x18 */
#define FC7300_FCPIT_CH0_TVAL_OFFSET       0x0020  /* Channel0 timer value register, offset: 0x20 */
#define FC7300_FCPIT_CH0_CVAL_OFFSET       0x0024  /* Channel0 current timer value register, offset: 0x24 */
#define FC7300_FCPIT_CH0_TCTRL_OFFSET      0x0028  /* Channel0 timer control register, offset: 0x28 */
#define FC7300_FCPIT_CH1_TVAL_OFFSET       0x0030  /* Channel1 timer value register, offset: 0x30 */
#define FC7300_FCPIT_CH1_CVAL_OFFSET       0x0034  /* Channel1 current timer value register, offset: 0x34 */
#define FC7300_FCPIT_CH1_TCTRL_OFFSET      0x0038  /* Channel1 timer control register, offset: 0x38 */
#define FC7300_FCPIT_CH2_TVAL_OFFSET       0x0040  /* Channel2 timer value register, offset: 0x40 */
#define FC7300_FCPIT_CH2_CVAL_OFFSET       0x0044  /* Channel2 current timer value register, offset: 0x44 */
#define FC7300_FCPIT_CH2_TCTRL_OFFSET      0x0048  /* Channel2 timer control register, offset: 0x48 */
#define FC7300_FCPIT_CH3_TVAL_OFFSET       0x0050  /* Channel3 timer value register, offset: 0x50 */
#define FC7300_FCPIT_CH3_CVAL_OFFSET       0x0054  /* Channel3 current timer value register, offset: 0x54 */
#define FC7300_FCPIT_CH3_TCTRL_OFFSET      0x0058  /* Channel3 timer control register, offset: 0x58 */

#define FC7300_FCPIT_TVAL_OFFSET           0x0  /* timer value register, offset: 0x0 */
#define FC7300_FCPIT_CVAL_OFFSET           0x4  /* current timer value register, offset: 0x4 */
#define FC7300_FCPIT_TCTRL_OFFSET          0x8  /* timer control register, offset: 0x8 */

/* Register Addresses *******************************************************/

#define FC7300_FCPIT0_CH0_CTRL_BASE         (FC7300_FCPIT0_BASE+FC7300_FCPIT_CH0_TVAL_OFFSET)
#define FC7300_FCPIT0_CH1_CTRL_BASE         (FC7300_FCPIT0_BASE+FC7300_FCPIT_CH1_TVAL_OFFSET)
#define FC7300_FCPIT0_CH2_CTRL_BASE         (FC7300_FCPIT0_BASE+FC7300_FCPIT_CH2_TVAL_OFFSET)
#define FC7300_FCPIT0_CH3_CTRL_BASE         (FC7300_FCPIT0_BASE+FC7300_FCPIT_CH3_TVAL_OFFSET)

#define FC7300_FCPIT0_MCR                   (FC7300_FCPIT0_BASE+FC7300_FCPIT_MCR_OFFSET)
#define FC7300_FCPIT0_MSR                   (FC7300_FCPIT0_BASE+FC7300_FCPIT_MSR_OFFSET)
#define FC7300_FCPIT0_MIER                  (FC7300_FCPIT0_BASE+FC7300_FCPIT_MIER_OFFSET)
#define FC7300_FCPIT0_SETTEN                (FC7300_FCPIT0_BASE+FC7300_FCPIT_SETTEN_OFFSET)
#define FC7300_FCPIT0_CLRTEN                (FC7300_FCPIT0_BASE+FC7300_FCPIT_CLRTEN_OFFSET)
#define FC7300_FCPIT0_CH0_TVAL              (FC7300_FCPIT0_BASE+FC7300_FCPIT_CH0_TVAL_OFFSET)
#define FC7300_FCPIT0_CH0_CVAL              (FC7300_FCPIT0_BASE+FC7300_FCPIT_CH0_CVAL_OFFSET)
#define FC7300_FCPIT0_CH0_TCTRL             (FC7300_FCPIT0_BASE+FC7300_FCPIT_CH0_TCTRL_OFFSET)
#define FC7300_FCPIT0_CH1_TVAL              (FC7300_FCPIT0_BASE+FC7300_FCPIT_CH1_TVAL_OFFSET)
#define FC7300_FCPIT0_CH1_CVAL              (FC7300_FCPIT0_BASE+FC7300_FCPIT_CH1_CVAL_OFFSET)
#define FC7300_FCPIT0_CH1_TCTRL             (FC7300_FCPIT0_BASE+FC7300_FCPIT_CH1_TCTRL_OFFSET)
#define FC7300_FCPIT0_CH2_TVAL              (FC7300_FCPIT0_BASE+FC7300_FCPIT_CH2_TVAL_OFFSET)
#define FC7300_FCPIT0_CH2_CVAL              (FC7300_FCPIT0_BASE+FC7300_FCPIT_CH2_CVAL_OFFSET)
#define FC7300_FCPIT0_CH2_TCTRL             (FC7300_FCPIT0_BASE+FC7300_FCPIT_CH2_TCTRL_OFFSET)
#define FC7300_FCPIT0_CH3_TVAL              (FC7300_FCPIT0_BASE+FC7300_FCPIT_CH3_TVAL_OFFSET)
#define FC7300_FCPIT0_CH3_CVAL              (FC7300_FCPIT0_BASE+FC7300_FCPIT_CH3_CVAL_OFFSET)
#define FC7300_FCPIT0_CH3_TCTRL             (FC7300_FCPIT0_BASE+FC7300_FCPIT_CH3_TCTRL_OFFSET)

#define FC7300_FCPIT1_CH0_CTRL_BASE         (FC7300_FCPIT1_BASE+FC7300_FCPIT_CH0_TVAL_OFFSET)
#define FC7300_FCPIT1_CH1_CTRL_BASE         (FC7300_FCPIT1_BASE+FC7300_FCPIT_CH1_TVAL_OFFSET)
#define FC7300_FCPIT1_CH2_CTRL_BASE         (FC7300_FCPIT1_BASE+FC7300_FCPIT_CH2_TVAL_OFFSET)
#define FC7300_FCPIT1_CH3_CTRL_BASE         (FC7300_FCPIT1_BASE+FC7300_FCPIT_CH3_TVAL_OFFSET)

#define FC7300_FCPIT1_MCR                   (FC7300_FCPIT1_BASE+FC7300_FCPIT_MCR_OFFSET)
#define FC7300_FCPIT1_MSR                   (FC7300_FCPIT1_BASE+FC7300_FCPIT_MSR_OFFSET)
#define FC7300_FCPIT1_MIER                  (FC7300_FCPIT1_BASE+FC7300_FCPIT_MIER_OFFSET)
#define FC7300_FCPIT1_SETTEN                (FC7300_FCPIT1_BASE+FC7300_FCPIT_SETTEN_OFFSET)
#define FC7300_FCPIT1_CLRTEN                (FC7300_FCPIT1_BASE+FC7300_FCPIT_CLRTEN_OFFSET)
#define FC7300_FCPIT1_CH0_TVAL              (FC7300_FCPIT1_BASE+FC7300_FCPIT_CH0_TVAL_OFFSET)
#define FC7300_FCPIT1_CH0_CVAL              (FC7300_FCPIT1_BASE+FC7300_FCPIT_CH0_CVAL_OFFSET)
#define FC7300_FCPIT1_CH0_TCTRL             (FC7300_FCPIT1_BASE+FC7300_FCPIT_CH0_TCTRL_OFFSET)
#define FC7300_FCPIT1_CH1_TVAL              (FC7300_FCPIT1_BASE+FC7300_FCPIT_CH1_TVAL_OFFSET)
#define FC7300_FCPIT1_CH1_CVAL              (FC7300_FCPIT1_BASE+FC7300_FCPIT_CH1_CVAL_OFFSET)
#define FC7300_FCPIT1_CH1_TCTRL             (FC7300_FCPIT1_BASE+FC7300_FCPIT_CH1_TCTRL_OFFSET)
#define FC7300_FCPIT1_CH2_TVAL              (FC7300_FCPIT1_BASE+FC7300_FCPIT_CH2_TVAL_OFFSET)
#define FC7300_FCPIT1_CH2_CVAL              (FC7300_FCPIT1_BASE+FC7300_FCPIT_CH2_CVAL_OFFSET)
#define FC7300_FCPIT1_CH2_TCTRL             (FC7300_FCPIT1_BASE+FC7300_FCPIT_CH2_TCTRL_OFFSET)
#define FC7300_FCPIT1_CH3_TVAL              (FC7300_FCPIT1_BASE+FC7300_FCPIT_CH3_TVAL_OFFSET)
#define FC7300_FCPIT1_CH3_CVAL              (FC7300_FCPIT1_BASE+FC7300_FCPIT_CH3_CVAL_OFFSET)
#define FC7300_FCPIT1_CH3_TCTRL             (FC7300_FCPIT1_BASE+FC7300_FCPIT_CH3_TCTRL_OFFSET)

/* MCR Bit Fields */

#define FCPIT_MCR_DBG_EN_MASK          0x8u
#define FCPIT_MCR_DBG_EN_SHIFT         3u
#define FCPIT_MCR_DBG_EN_WIDTH         1u
#define FCPIT_MCR_DBG_EN(x)            (((uint32_t)(((uint32_t)(x))<<FCPIT_MCR_DBG_EN_SHIFT))&FCPIT_MCR_DBG_EN_MASK)
#define FCPIT_MCR_LPM_EN_MASK          0x4u
#define FCPIT_MCR_LPM_EN_SHIFT         2u
#define FCPIT_MCR_LPM_EN_WIDTH         1u
#define FCPIT_MCR_LPM_EN(x)            (((uint32_t)(((uint32_t)(x))<<FCPIT_MCR_LPM_EN_SHIFT))&FCPIT_MCR_LPM_EN_MASK)
#define FCPIT_MCR_SW_RST_MASK          0x2u
#define FCPIT_MCR_SW_RST_SHIFT         1u
#define FCPIT_MCR_SW_RST_WIDTH         1u
#define FCPIT_MCR_SW_RST(x)            (((uint32_t)(((uint32_t)(x))<<FCPIT_MCR_SW_RST_SHIFT))&FCPIT_MCR_SW_RST_MASK)
#define FCPIT_MCR_M_CEN_MASK           0x1u
#define FCPIT_MCR_M_CEN_SHIFT          0u
#define FCPIT_MCR_M_CEN_WIDTH          1u
#define FCPIT_MCR_M_CEN(x)             (((uint32_t)(((uint32_t)(x))<<FCPIT_MCR_M_CEN_SHIFT))&FCPIT_MCR_M_CEN_MASK)

/* MCR Reg Mask */

#define FCPIT_MCR_MASK                 0x0000000Fu

/* MSR Bit Fields */

#define FCPIT_MSR_TIF3_MASK            0x8u
#define FCPIT_MSR_TIF3_SHIFT           3u
#define FCPIT_MSR_TIF3_WIDTH           1u
#define FCPIT_MSR_TIF3(x)              (((uint32_t)(((uint32_t)(x))<<FCPIT_MSR_TIF3_SHIFT))&FCPIT_MSR_TIF3_MASK)
#define FCPIT_MSR_TIF2_MASK            0x4u
#define FCPIT_MSR_TIF2_SHIFT           2u
#define FCPIT_MSR_TIF2_WIDTH           1u
#define FCPIT_MSR_TIF2(x)              (((uint32_t)(((uint32_t)(x))<<FCPIT_MSR_TIF2_SHIFT))&FCPIT_MSR_TIF2_MASK)
#define FCPIT_MSR_TIF1_MASK            0x2u
#define FCPIT_MSR_TIF1_SHIFT           1u
#define FCPIT_MSR_TIF1_WIDTH           1u
#define FCPIT_MSR_TIF1(x)              (((uint32_t)(((uint32_t)(x))<<FCPIT_MSR_TIF1_SHIFT))&FCPIT_MSR_TIF1_MASK)
#define FCPIT_MSR_TIF0_MASK            0x1u
#define FCPIT_MSR_TIF0_SHIFT           0u
#define FCPIT_MSR_TIF0_WIDTH           1u
#define FCPIT_MSR_TIF0(x)              (((uint32_t)(((uint32_t)(x))<<FCPIT_MSR_TIF0_SHIFT))&FCPIT_MSR_TIF0_MASK)

/* MSR Reg Mask */

#define FCPIT_MSR_MASK                 0x0000000Fu

/* MIER Bit Fields */

#define FCPIT_MIER_TIE3_MASK           0x8u
#define FCPIT_MIER_TIE3_SHIFT          3u
#define FCPIT_MIER_TIE3_WIDTH          1u
#define FCPIT_MIER_TIE3(x)             (((uint32_t)(((uint32_t)(x))<<FCPIT_MIER_TIE3_SHIFT))&FCPIT_MIER_TIE3_MASK)
#define FCPIT_MIER_TIE2_MASK           0x4u
#define FCPIT_MIER_TIE2_SHIFT          2u
#define FCPIT_MIER_TIE2_WIDTH          1u
#define FCPIT_MIER_TIE2(x)             (((uint32_t)(((uint32_t)(x))<<FCPIT_MIER_TIE2_SHIFT))&FCPIT_MIER_TIE2_MASK)
#define FCPIT_MIER_TIE1_MASK           0x2u
#define FCPIT_MIER_TIE1_SHIFT          1u
#define FCPIT_MIER_TIE1_WIDTH          1u
#define FCPIT_MIER_TIE1(x)             (((uint32_t)(((uint32_t)(x))<<FCPIT_MIER_TIE1_SHIFT))&FCPIT_MIER_TIE1_MASK)
#define FCPIT_MIER_TIE0_MASK           0x1u
#define FCPIT_MIER_TIE0_SHIFT          0u
#define FCPIT_MIER_TIE0_WIDTH          1u
#define FCPIT_MIER_TIE0(x)             (((uint32_t)(((uint32_t)(x))<<FCPIT_MIER_TIE0_SHIFT))&FCPIT_MIER_TIE0_MASK)

/* MIER Reg Mask */

#define FCPIT_MIER_MASK                0x0000000Fu

/* SETTEN Bit Fields */

#define FCPIT_SETTEN_SET_T_EN_3_MASK   0x8u
#define FCPIT_SETTEN_SET_T_EN_3_SHIFT  3u
#define FCPIT_SETTEN_SET_T_EN_3_WIDTH  1u
#define FCPIT_SETTEN_SET_T_EN_3(x)     (((uint32_t)(((uint32_t)(x))<<FCPIT_SETTEN_SET_T_EN_3_SHIFT))&FCPIT_SETTEN_SET_T_EN_3_MASK)
#define FCPIT_SETTEN_SET_T_EN_2_MASK   0x4u
#define FCPIT_SETTEN_SET_T_EN_2_SHIFT  2u
#define FCPIT_SETTEN_SET_T_EN_2_WIDTH  1u
#define FCPIT_SETTEN_SET_T_EN_2(x)     (((uint32_t)(((uint32_t)(x))<<FCPIT_SETTEN_SET_T_EN_2_SHIFT))&FCPIT_SETTEN_SET_T_EN_2_MASK)
#define FCPIT_SETTEN_SET_T_EN_1_MASK   0x2u
#define FCPIT_SETTEN_SET_T_EN_1_SHIFT  1u
#define FCPIT_SETTEN_SET_T_EN_1_WIDTH  1u
#define FCPIT_SETTEN_SET_T_EN_1(x)     (((uint32_t)(((uint32_t)(x))<<FCPIT_SETTEN_SET_T_EN_1_SHIFT))&FCPIT_SETTEN_SET_T_EN_1_MASK)
#define FCPIT_SETTEN_SET_T_EN_0_MASK   0x1u
#define FCPIT_SETTEN_SET_T_EN_0_SHIFT  0u
#define FCPIT_SETTEN_SET_T_EN_0_WIDTH  1u
#define FCPIT_SETTEN_SET_T_EN_0(x)     (((uint32_t)(((uint32_t)(x))<<FCPIT_SETTEN_SET_T_EN_0_SHIFT))&FCPIT_SETTEN_SET_T_EN_0_MASK)

/* SETTEN Reg Mask */

#define FCPIT_SETTEN_MASK              0x0000000Fu

/* CLRTEN Bit Fields */

#define FCPIT_CLRTEN_CLR_T_EN_3_MASK   0x8u
#define FCPIT_CLRTEN_CLR_T_EN_3_SHIFT  3u
#define FCPIT_CLRTEN_CLR_T_EN_3_WIDTH  1u
#define FCPIT_CLRTEN_CLR_T_EN_3(x)     (((uint32_t)(((uint32_t)(x))<<FCPIT_CLRTEN_CLR_T_EN_3_SHIFT))&FCPIT_CLRTEN_CLR_T_EN_3_MASK)
#define FCPIT_CLRTEN_CLR_T_EN_2_MASK   0x4u
#define FCPIT_CLRTEN_CLR_T_EN_2_SHIFT  2u
#define FCPIT_CLRTEN_CLR_T_EN_2_WIDTH  1u
#define FCPIT_CLRTEN_CLR_T_EN_2(x)     (((uint32_t)(((uint32_t)(x))<<FCPIT_CLRTEN_CLR_T_EN_2_SHIFT))&FCPIT_CLRTEN_CLR_T_EN_2_MASK)
#define FCPIT_CLRTEN_CLR_T_EN_1_MASK   0x2u
#define FCPIT_CLRTEN_CLR_T_EN_1_SHIFT  1u
#define FCPIT_CLRTEN_CLR_T_EN_1_WIDTH  1u
#define FCPIT_CLRTEN_CLR_T_EN_1(x)     (((uint32_t)(((uint32_t)(x))<<FCPIT_CLRTEN_CLR_T_EN_1_SHIFT))&FCPIT_CLRTEN_CLR_T_EN_1_MASK)
#define FCPIT_CLRTEN_CLR_T_EN_0_MASK   0x1u
#define FCPIT_CLRTEN_CLR_T_EN_0_SHIFT  0u
#define FCPIT_CLRTEN_CLR_T_EN_0_WIDTH  1u
#define FCPIT_CLRTEN_CLR_T_EN_0(x)     (((uint32_t)(((uint32_t)(x))<<FCPIT_CLRTEN_CLR_T_EN_0_SHIFT))&FCPIT_CLRTEN_CLR_T_EN_0_MASK)

/* CLRTEN Reg Mask */

#define FCPIT_CLRTEN_MASK              0x0000000Fu

/* TVAL Bit Fields */

#define FCPIT_TVAL_TMR_VAL_MASK        0xFFFFFFFFu
#define FCPIT_TVAL_TMR_VAL_SHIFT       0u
#define FCPIT_TVAL_TMR_VAL_WIDTH       32u
#define FCPIT_TVAL_TMR_VAL(x)          (((uint32_t)(((uint32_t)(x))<<FCPIT_TVAL_TMR_VAL_SHIFT))&FCPIT_TVAL_TMR_VAL_MASK)

/* TVAL0 Reg Mask */

#define FCPIT_TVAL_MASK                0xFFFFFFFFu

/* TCTRL Bit Fields */

#define FCPIT_TCTRL_TRG_SEL_MASK       0xF000000u
#define FCPIT_TCTRL_TRG_SEL_SHIFT      24u
#define FCPIT_TCTRL_TRG_SEL_WIDTH      4u
#define FCPIT_TCTRL_TRG_SEL(x)         (((uint32_t)(((uint32_t)(x))<<FCPIT_TCTRL_TRG_SEL_SHIFT))&FCPIT_TCTRL_TRG_SEL_MASK)
#define FCPIT_TCTRL_TRG_SRC_MASK       0x800000u
#define FCPIT_TCTRL_TRG_SRC_SHIFT      23u
#define FCPIT_TCTRL_TRG_SRC_WIDTH      1u
#define FCPIT_TCTRL_TRG_SRC(x)         (((uint32_t)(((uint32_t)(x))<<FCPIT_TCTRL_TRG_SRC_SHIFT))&FCPIT_TCTRL_TRG_SRC_MASK)
#define FCPIT_TCTRL_TROT_MASK          0x40000u
#define FCPIT_TCTRL_TROT_SHIFT         18u
#define FCPIT_TCTRL_TROT_WIDTH         1u
#define FCPIT_TCTRL_TROT(x)            (((uint32_t)(((uint32_t)(x))<<FCPIT_TCTRL_TROT_SHIFT))&FCPIT_TCTRL_TROT_MASK)
#define FCPIT_TCTRL_TSOI_MASK          0x20000u
#define FCPIT_TCTRL_TSOI_SHIFT         17u
#define FCPIT_TCTRL_TSOI_WIDTH         1u
#define FCPIT_TCTRL_TSOI(x)            (((uint32_t)(((uint32_t)(x))<<FCPIT_TCTRL_TSOI_SHIFT))&FCPIT_TCTRL_TSOI_MASK)
#define FCPIT_TCTRL_TSOT_MASK          0x10000u
#define FCPIT_TCTRL_TSOT_SHIFT         16u
#define FCPIT_TCTRL_TSOT_WIDTH         1u
#define FCPIT_TCTRL_TSOT(x)            (((uint32_t)(((uint32_t)(x))<<FCPIT_TCTRL_TSOT_SHIFT))&FCPIT_TCTRL_TSOT_MASK)
#define FCPIT_TCTRL_MODE_MASK          0xCu
#define FCPIT_TCTRL_MODE_SHIFT         2u
#define FCPIT_TCTRL_MODE_WIDTH         2u
#define FCPIT_TCTRL_MODE(x)            (((uint32_t)(((uint32_t)(x))<<FCPIT_TCTRL_MODE_SHIFT))&FCPIT_TCTRL_MODE_MASK)
#define FCPIT_TCTRL_CHAIN_MASK         0x2u
#define FCPIT_TCTRL_CHAIN_SHIFT        1u
#define FCPIT_TCTRL_CHAIN_WIDTH        1u
#define FCPIT_TCTRL_CHAIN(x)           (((uint32_t)(((uint32_t)(x))<<FCPIT_TCTRL_CHAIN_SHIFT))&FCPIT_TCTRL_CHAIN_MASK)
#define FCPIT_TCTRL_T_EN_MASK          0x1u
#define FCPIT_TCTRL_T_EN_SHIFT         0u
#define FCPIT_TCTRL_T_EN_WIDTH         1u
#define FCPIT_TCTRL_T_EN(x)            (((uint32_t)(((uint32_t)(x))<<FCPIT_TCTRL_T_EN_SHIFT))&FCPIT_TCTRL_T_EN_MASK)

/* TCTRL0 Reg Mask */

#define FCPIT_TCTRL_MASK               0x0F87000Fu

#endif /* __VENDOR_FLAGCHIP_CHIPS_FC7300_HARDWARE_FC7300_FCPIT_H */
