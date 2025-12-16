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

#ifndef __VENDOR_FLAGCHIP_CHIPS_FC7300_HARDWARE_FC7300_RTC_H
#define __VENDOR_FLAGCHIP_CHIPS_FC7300_HARDWARE_FC7300_RTC_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>
#include "hardware/fc7300_memorymap.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Register Offsets *********************************************************/

#define FC7300_RTC_SR_OFFSET       0x0000     /* Seconds register offset */
#define FC7300_RTC_PR_OFFSET       0x0004     /* Prescaler register offset */
#define FC7300_RTC_AR_OFFSET       0x0008     /* Alarm register offset */
#define FC7300_RTC_COMPR_OFFSET    0x000c     /* Compensation register offset */
#define FC7300_RTC_CR_OFFSET       0x0010     /* Control register offset */
#define FC7300_RTC_STR_OFFSET      0x0014     /* Status register offset */
#define FC7300_RTC_LR_CTRL_OFFSET  0x0018     /* Lock register offset */
#define FC7300_RTC_IER_TR_OFFSET   0x001c     /* Interrupt enable register offset */

/* Register Addresses *******************************************************/

/* RTC registers */

#define FC7300_RTC_SR                   (FC7300_RTC_BASE + FC7300_RTC_SR_OFFSET)             /* RTC Seconds register */
#define FC7300_RTC_PR                   (FC7300_RTC_BASE + FC7300_RTC_PR_OFFSET)             /* RTC Prescaler register */
#define FC7300_RTC_AR                   (FC7300_RTC_BASE + FC7300_RTC_AR_OFFSET)             /* RTC Alarm register */
#define FC7300_RTC_COMPR                (FC7300_RTC_BASE + FC7300_RTC_COMPR_OFFSET)          /* RTC Compensation register */
#define FC7300_RTC_CR                   (FC7300_RTC_BASE + FC7300_RTC_CR_OFFSET)             /* RTC Control register */
#define FC7300_RTC_STR                  (FC7300_RTC_BASE + FC7300_RTC_STR_OFFSET)            /* RTC Status register */
#define FC7300_RTC_LR                   (FC7300_RTC_BASE + FC7300_RTC_LR_CTRL_OFFSET)        /* RTC Lock register */
#define FC7300_RTC_IER                  (FC7300_RTC_BASE + FC7300_RTC_IER_TR_OFFSET)         /* RTC Interrupt enable register */

/* Register Bit Definitions *************************************************/

/* SR Bit Fields */

#define RTC_SR_SR_MASK                 0xFFFFFFFFu
#define RTC_SR_SR_SHIFT                0u
#define RTC_SR_SR_WIDTH                32u
#define RTC_SR_SR(x)                   (((uint32_t)(((uint32_t)(x))<<RTC_SR_SR_SHIFT))&RTC_SR_SR_MASK)

/* SR Reg Mask */

#define RTC_SR_MASK                    0xFFFFFFFFu

/* PR Bit Fields */

#define RTC_PR_PR_MASK                 0xFFFFu
#define RTC_PR_PR_SHIFT                0u
#define RTC_PR_PR_WIDTH                16u
#define RTC_PR_PR(x)                   (((uint32_t)(((uint32_t)(x))<<RTC_PR_PR_SHIFT))&RTC_PR_PR_MASK)

/* PR Reg Mask */

#define RTC_PR_MASK                    0x0000FFFFu

/* TAR Bit Fields */

#define RTC_TAR_AR_MASK                0xFFFFFFFFu
#define RTC_TAR_AR_SHIFT               0u
#define RTC_TAR_AR_WIDTH               32u
#define RTC_TAR_AR(x)                  (((uint32_t)(((uint32_t)(x))<<RTC_TAR_AR_SHIFT))&RTC_TAR_AR_MASK)

/* TAR Reg Mask */

#define RTC_TAR_MASK                   0xFFFFFFFFu

/* COMPR Bit Fields */

#define RTC_COMPR_CIC_MASK             0xFF000000u
#define RTC_COMPR_CIC_SHIFT            24u
#define RTC_COMPR_CIC_WIDTH            8u
#define RTC_COMPR_CIC(x)               (((uint32_t)(((uint32_t)(x))<<RTC_COMPR_CIC_SHIFT))&RTC_COMPR_CIC_MASK)

#define RTC_COMPR_TCV_MASK             0xFF0000u
#define RTC_COMPR_TCV_SHIFT            16u
#define RTC_COMPR_TCV_WIDTH            8u
#define RTC_COMPR_TCV(x)               (((uint32_t)(((uint32_t)(x))<<RTC_COMPR_TCV_SHIFT))&RTC_COMPR_TCV_MASK)

#define RTC_COMPR_CIR_MASK             0xFF00u
#define RTC_COMPR_CIR_SHIFT            8u
#define RTC_COMPR_CIR_WIDTH            8u
#define RTC_COMPR_CIR(x)               (((uint32_t)(((uint32_t)(x))<<RTC_COMPR_CIR_SHIFT))&RTC_COMPR_CIR_MASK)

#define RTC_COMPR_TCR_MASK             0xFFu
#define RTC_COMPR_TCR_SHIFT            0u
#define RTC_COMPR_TCR_WIDTH            8u
#define RTC_COMPR_TCR(x)               (((uint32_t)(((uint32_t)(x))<<RTC_COMPR_TCR_SHIFT))&RTC_COMPR_TCR_MASK)

/* COMPR Reg Mask */

#define RTC_COMPR_MASK                 0xFFFFFFFFu

/* CR Bit Fields */

#define RTC_CR_CKO_MASK                0x200u
#define RTC_CR_CKO_SHIFT               9u
#define RTC_CR_CKO_WIDTH               1u
#define RTC_CR_CKO(x)                  (((uint32_t)(((uint32_t)(x))<<RTC_CR_CKO_SHIFT))&RTC_CR_CKO_MASK)

#define RTC_CR_CKPS_MASK               0x20u
#define RTC_CR_CKPS_SHIFT              5u
#define RTC_CR_CKPS_WIDTH              1u
#define RTC_CR_CKPS(x)                 (((uint32_t)(((uint32_t)(x))<<RTC_CR_CKPS_SHIFT))&RTC_CR_CKPS_MASK)

#define RTC_CR_UM_MASK                 0x8u
#define RTC_CR_UM_SHIFT                3u
#define RTC_CR_UM_WIDTH                1u
#define RTC_CR_UM(x)                   (((uint32_t)(((uint32_t)(x))<<RTC_CR_UM_SHIFT))&RTC_CR_UM_MASK)

#define RTC_CR_SWRST_MASK              0x1u
#define RTC_CR_SWRST_SHIFT             0u
#define RTC_CR_SWRST_WIDTH             1u
#define RTC_CR_SWRST(x)                (((uint32_t)(((uint32_t)(x))<<RTC_CR_SWRST_SHIFT))&RTC_CR_SWRST_MASK)

/* CR Reg Mask */

#define RTC_CR_MASK                    0x00000229u

/* STR Bit Fields */

#define RTC_STR_TCE_MASK               0x10u
#define RTC_STR_TCE_SHIFT              4u
#define RTC_STR_TCE_WIDTH              1u
#define RTC_STR_TCE(x)                 (((uint32_t)(((uint32_t)(x))<<RTC_STR_TCE_SHIFT))&RTC_STR_TCE_MASK)

#define RTC_STR_TAF_MASK               0x4u
#define RTC_STR_TAF_SHIFT              2u
#define RTC_STR_TAF_WIDTH              1u
#define RTC_STR_TAF(x)                 (((uint32_t)(((uint32_t)(x))<<RTC_STR_TAF_SHIFT))&RTC_STR_TAF_MASK)

#define RTC_STR_TOF_MASK               0x2u
#define RTC_STR_TOF_SHIFT              1u
#define RTC_STR_TOF_WIDTH              1u
#define RTC_STR_TOF(x)                 (((uint32_t)(((uint32_t)(x))<<RTC_STR_TOF_SHIFT))&RTC_STR_TOF_MASK)

/* STR Reg Mask */

#define RTC_STR_MASK                   0x00000016u

/* LR Bit Fields */

#define RTC_LR_LRL_MASK                0x40u
#define RTC_LR_LRL_SHIFT               6u
#define RTC_LR_LRL_WIDTH               1u
#define RTC_LR_LRL(x)                  (((uint32_t)(((uint32_t)(x))<<RTC_LR_LRL_SHIFT))&RTC_LR_LRL_MASK)
#define RTC_LR_STRL_MASK               0x20u
#define RTC_LR_STRL_SHIFT              5u
#define RTC_LR_STRL_WIDTH              1u
#define RTC_LR_STRL(x)                 (((uint32_t)(((uint32_t)(x))<<RTC_LR_STRL_SHIFT))&RTC_LR_STRL_MASK)
#define RTC_LR_CRL_MASK                0x10u
#define RTC_LR_CRL_SHIFT               4u
#define RTC_LR_CRL_WIDTH               1u
#define RTC_LR_CRL(x)                  (((uint32_t)(((uint32_t)(x))<<RTC_LR_CRL_SHIFT))&RTC_LR_CRL_MASK)
#define RTC_LR_CPL_MASK                0x8u
#define RTC_LR_CPL_SHIFT               3u
#define RTC_LR_CPL_WIDTH               1u
#define RTC_LR_CPL(x)                  (((uint32_t)(((uint32_t)(x))<<RTC_LR_CPL_SHIFT))&RTC_LR_CPL_MASK)

/* LR Reg Mask */

#define RTC_LR_MASK                    0x00000078u

/* IER Bit Fields */

#define RTC_IER_TSIC_MASK              0x70000u
#define RTC_IER_TSIC_SHIFT             16u
#define RTC_IER_TSIC_WIDTH             3u
#define RTC_IER_TSIC(x)                (((uint32_t)(((uint32_t)(x))<<RTC_IER_TSIC_SHIFT))&RTC_IER_TSIC_MASK)
#define RTC_IER_TSIE_MASK              0x10u
#define RTC_IER_TSIE_SHIFT             4u
#define RTC_IER_TSIE_WIDTH             1u
#define RTC_IER_TSIE(x)                (((uint32_t)(((uint32_t)(x))<<RTC_IER_TSIE_SHIFT))&RTC_IER_TSIE_MASK)
#define RTC_IER_TAIE_MASK              0x4u
#define RTC_IER_TAIE_SHIFT             2u
#define RTC_IER_TAIE_WIDTH             1u
#define RTC_IER_TAIE(x)                (((uint32_t)(((uint32_t)(x))<<RTC_IER_TAIE_SHIFT))&RTC_IER_TAIE_MASK)
#define RTC_IER_TOIE_MASK              0x2u
#define RTC_IER_TOIE_SHIFT             1u
#define RTC_IER_TOIE_WIDTH             1u
#define RTC_IER_TOIE(x)                (((uint32_t)(((uint32_t)(x))<<RTC_IER_TOIE_SHIFT))&RTC_IER_TOIE_MASK)

/* IER Reg Mask */

#define RTC_IER_MASK                   0x00070016u

#endif /* __VENDOR_FLAGCHIP_CHIPS_FC7300_HARDWARE_FC7300_RTC_H */