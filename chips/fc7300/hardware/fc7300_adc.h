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

#ifndef __ARCH_ARM_SRC_FC7300_HARDWARE_FC7300_ADC_H
#define __ARCH_ARM_SRC_FC7300_HARDWARE_FC7300_ADC_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include "hardware/fc7300_memorymap.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Register Offsets *********************************************************/

#define FC7300_ADC_INT_STATUS_OFFSET    0x0000     /* Interrupt status register offset */
#define FC7300_ADC_INT_ENABLE_OFFSET    0x0004     /* Interrupt enable register offset */
#define FC7300_ADC_CONTROL_OFFSET       0x0008     /* Control register offset */
#define FC7300_ADC_CFG1_OFFSET          0x000c     /* Configuration1 register offset */
#define FC7300_ADC_CFG2_OFFSET          0x0010     /* Configuration2 register offset */
#define FC7300_ADC_SMPR_OFFSET          0x0014     /* Sampling rate register offset */
#define FC7300_ADC_CMP_CTRL_OFFSET      0x0018     /* Compare control register offset */
#define FC7300_ADC_CMP_TR_OFFSET        0x001c     /* Compare threshold register offset */
#define FC7300_ADC_FIFO_DATA_OFFSET     0x004c     /* FIFO data register offset */
#define FC7300_ADC_SC_OFFSET(N)         0x0050+4*N /* Sequence Configuration register offset */
#define FC7300_ADC_RESULT_OFFSET(N)     0x00d0+4*N /* Result register offset */

/* Register Addresses *******************************************************/

/* ADC0 registers */

#define FC7300_ADC0_INT_STATUS          (FC7300_ADC0_BASE + FC7300_ADC_INT_STATUS_OFFSET)    /* ADC0 interrupt status register */
#define FC7300_ADC0_INT_ENABLE          (FC7300_ADC0_BASE + FC7300_ADC_INT_ENABLE_OFFSET)    /* ADC0 interrupt enable register */
#define FC7300_ADC0_CONTROL             (FC7300_ADC0_BASE + FC7300_ADC_CONTROL_OFFSET)       /* ADC0 Modulo register */
#define FC7300_ADC0_CFG1                (FC7300_ADC0_BASE + FC7300_ADC_CFG1_OFFSET)          /* ADC0 Configuration1 register */
#define FC7300_ADC0_CFG2                (FC7300_ADC0_BASE + FC7300_ADC_CFG2_OFFSET)          /* ADC0 Configuration2 register */
#define FC7300_ADC0_SMPR                (FC7300_ADC0_BASE + FC7300_ADC_SMPR_OFFSET)          /* ADC0 Sampling rate register */
#define FC7300_ADC0_CMP_CTRL            (FC7300_ADC0_BASE + FC7300_ADC_CMP_CTRL_OFFSET)      /* ADC0 Compare control register */
#define FC7300_ADC0_CMP_TR              (FC7300_ADC0_BASE + FC7300_ADC_CMP_TR_OFFSET)        /* ADC0 Compare threshold register */
#define FC7300_ADC0_FIFO_DATA           (FC7300_ADC0_BASE + FC7300_ADC_FIFO_DATA_OFFSET)     /* ADC0 FIFO data register */
#define FC7300_ADC0_SC(N)               (FC7300_ADC0_BASE + FC7300_ADC_SC_OFFSET(N))         /* ADC0 sequence configuration register */
#define FC7300_ADC0_RESULT(N)           (FC7300_ADC0_BASE + FC7300_ADC_RESULT_OFFSET(N))     /* ADC0 Result register */

/* ADC1 registers */

#define FC7300_ADC1_INT_STATUS          (FC7300_ADC1_BASE + FC7300_ADC_INT_STATUS_OFFSET)    /* ADC1 interrupt status register */
#define FC7300_ADC1_INT_ENABLE          (FC7300_ADC1_BASE + FC7300_ADC_INT_ENABLE_OFFSET)    /* ADC1 interrupt enable register */
#define FC7300_ADC1_CONTROL             (FC7300_ADC1_BASE + FC7300_ADC_CONTROL_OFFSET)       /* ADC1 Modulo register */
#define FC7300_ADC1_CFG1                (FC7300_ADC1_BASE + FC7300_ADC_CFG1_OFFSET)          /* ADC1 Configuration1 register */
#define FC7300_ADC1_CFG2                (FC7300_ADC1_BASE + FC7300_ADC_CFG2_OFFSET)          /* ADC1 Configuration2 register */
#define FC7300_ADC1_SMPR                (FC7300_ADC1_BASE + FC7300_ADC_SMPR_OFFSET)          /* ADC1 Sampling rate register */
#define FC7300_ADC1_CMP_CTRL            (FC7300_ADC1_BASE + FC7300_ADC_CMP_CTRL_OFFSET)      /* ADC1 Compare control register */
#define FC7300_ADC1_CMP_TR              (FC7300_ADC1_BASE + FC7300_ADC_CMP_TR_OFFSET)        /* ADC1 Compare threshold register */
#define FC7300_ADC1_FIFO_DATA           (FC7300_ADC1_BASE + FC7300_ADC_FIFO_DATA_OFFSET)     /* ADC1 FIFO data register */
#define FC7300_ADC1_SC(N)               (FC7300_ADC1_BASE + FC7300_ADC_SC_OFFSET(N))         /* ADC1 sequence configuration register */
#define FC7300_ADC1_RESULT(N)           (FC7300_ADC1_BASE + FC7300_ADC_RESULT_OFFSET(N))     /* ADC1 Result register */

/* ADC2 registers */

#define FC7300_ADC2_INT_STATUS          (FC7300_ADC2_BASE + FC7300_ADC_INT_STATUS_OFFSET)    /* ADC2 interrupt status register */
#define FC7300_ADC2_INT_ENABLE          (FC7300_ADC2_BASE + FC7300_ADC_INT_ENABLE_OFFSET)    /* ADC2 interrupt enable register */
#define FC7300_ADC2_CONTROL             (FC7300_ADC2_BASE + FC7300_ADC_CONTROL_OFFSET)       /* ADC2 Modulo register */
#define FC7300_ADC2_CFG1                (FC7300_ADC2_BASE + FC7300_ADC_CFG1_OFFSET)          /* ADC2 Configuration1 register */
#define FC7300_ADC2_CFG2                (FC7300_ADC2_BASE + FC7300_ADC_CFG2_OFFSET)          /* ADC2 Configuration2 register */
#define FC7300_ADC2_SMPR                (FC7300_ADC2_BASE + FC7300_ADC_SMPR_OFFSET)          /* ADC2 Sampling rate register */
#define FC7300_ADC2_CMP_CTRL            (FC7300_ADC2_BASE + FC7300_ADC_CMP_CTRL_OFFSET)      /* ADC2 Compare control register */
#define FC7300_ADC2_CMP_TR              (FC7300_ADC2_BASE + FC7300_ADC_CMP_TR_OFFSET)        /* ADC2 Compare threshold register */
#define FC7300_ADC2_FIFO_DATA           (FC7300_ADC2_BASE + FC7300_ADC_FIFO_DATA_OFFSET)     /* ADC2 FIFO data register */
#define FC7300_ADC2_SC(N)               (FC7300_ADC2_BASE + FC7300_ADC_SC_OFFSET(N))         /* ADC2 sequence configuration register */
#define FC7300_ADC2_RESULT(N)           (FC7300_ADC2_BASE + FC7300_ADC_RESULT_OFFSET(N))     /* ADC2 Result register */

/* ADC3 registers */

#define FC7300_ADC3_INT_STATUS          (FC7300_ADC3_BASE + FC7300_ADC_INT_STATUS_OFFSET)    /* ADC3 interrupt status register */
#define FC7300_ADC3_INT_ENABLE          (FC7300_ADC3_BASE + FC7300_ADC_INT_ENABLE_OFFSET)    /* ADC3 interrupt enable register */
#define FC7300_ADC3_CONTROL             (FC7300_ADC3_BASE + FC7300_ADC_CONTROL_OFFSET)       /* ADC3 Modulo register */
#define FC7300_ADC3_CFG1                (FC7300_ADC3_BASE + FC7300_ADC_CFG1_OFFSET)          /* ADC3 Configuration1 register */
#define FC7300_ADC3_CFG2                (FC7300_ADC3_BASE + FC7300_ADC_CFG2_OFFSET)          /* ADC3 Configuration2 register */
#define FC7300_ADC3_SMPR                (FC7300_ADC3_BASE + FC7300_ADC_SMPR_OFFSET)          /* ADC3 Sampling rate register */
#define FC7300_ADC3_CMP_CTRL            (FC7300_ADC3_BASE + FC7300_ADC_CMP_CTRL_OFFSET)      /* ADC3 Compare control register */
#define FC7300_ADC3_CMP_TR              (FC7300_ADC3_BASE + FC7300_ADC_CMP_TR_OFFSET)        /* ADC3 Compare threshold register */
#define FC7300_ADC3_FIFO_DATA           (FC7300_ADC3_BASE + FC7300_ADC_FIFO_DATA_OFFSET)     /* ADC3 FIFO data register */
#define FC7300_ADC3_SC(N)               (FC7300_ADC3_BASE + FC7300_ADC_SC_OFFSET(N))         /* ADC3 sequence configuration register */
#define FC7300_ADC3_RESULT(N)           (FC7300_ADC3_BASE + FC7300_ADC_RESULT_OFFSET(N))     /* ADC3 Result register */

/* Register Bit Definitions *************************************************/

/* INT_STATUS Bit Fields */

#define ADC_INT_STATUS_FIFO_RDY_MASK   0x100u
#define ADC_INT_STATUS_FIFO_RDY_SHIFT  8u
#define ADC_INT_STATUS_FIFO_RDY_WIDTH  1u
#define ADC_INT_STATUS_FIFO_RDY(x)     (((uint32_t)(((uint32_t)(x))<<ADC_INT_STATUS_FIFO_RDY_SHIFT))&ADC_INT_STATUS_FIFO_RDY_MASK)

#define ADC_INT_STATUS_ACMP_MASK       0x80u
#define ADC_INT_STATUS_ACMP_SHIFT      7u
#define ADC_INT_STATUS_ACMP_WIDTH      1u
#define ADC_INT_STATUS_ACMP(x)         (((uint32_t)(((uint32_t)(x))<<ADC_INT_STATUS_ACMP_SHIFT))&ADC_INT_STATUS_ACMP_MASK)

#define ADC_INT_STATUS_EMPTY_MASK      0x40u
#define ADC_INT_STATUS_EMPTY_SHIFT     6u
#define ADC_INT_STATUS_EMPTY_WIDTH     1u
#define ADC_INT_STATUS_EMPTY(x)        (((uint32_t)(((uint32_t)(x))<<ADC_INT_STATUS_EMPTY_SHIFT))&ADC_INT_STATUS_EMPTY_MASK)

#define ADC_INT_STATUS_FULL_MASK       0x20u
#define ADC_INT_STATUS_FULL_SHIFT      5u
#define ADC_INT_STATUS_FULL_WIDTH      1u
#define ADC_INT_STATUS_FULL(x)         (((uint32_t)(((uint32_t)(x))<<ADC_INT_STATUS_FULL_SHIFT))&ADC_INT_STATUS_FULL_MASK)

#define ADC_INT_STATUS_OVR_MASK        0x10u
#define ADC_INT_STATUS_OVR_SHIFT       4u
#define ADC_INT_STATUS_OVR_WIDTH       1u
#define ADC_INT_STATUS_OVR(x)          (((uint32_t)(((uint32_t)(x))<<ADC_INT_STATUS_OVR_SHIFT))&ADC_INT_STATUS_OVR_MASK)

#define ADC_INT_STATUS_EOSEQ_MASK      0x8u
#define ADC_INT_STATUS_EOSEQ_SHIFT     3u
#define ADC_INT_STATUS_EOSEQ_WIDTH     1u
#define ADC_INT_STATUS_EOSEQ(x)        (((uint32_t)(((uint32_t)(x))<<ADC_INT_STATUS_EOSEQ_SHIFT))&ADC_INT_STATUS_EOSEQ_MASK)

#define ADC_INT_STATUS_EOC_MASK        0x4u
#define ADC_INT_STATUS_EOC_SHIFT       2u
#define ADC_INT_STATUS_EOC_WIDTH       1u
#define ADC_INT_STATUS_EOC(x)          (((uint32_t)(((uint32_t)(x))<<ADC_INT_STATUS_EOC_SHIFT))&ADC_INT_STATUS_EOC_MASK)

#define ADC_INT_STATUS_EOSMP_MASK      0x2u
#define ADC_INT_STATUS_EOSMP_SHIFT     1u
#define ADC_INT_STATUS_EOSMP_WIDTH     1u
#define ADC_INT_STATUS_EOSMP(x)        (((uint32_t)(((uint32_t)(x))<<ADC_INT_STATUS_EOSMP_SHIFT))&ADC_INT_STATUS_EOSMP_MASK)

#define ADC_INT_STATUS_ADRDY_MASK      0x1u
#define ADC_INT_STATUS_ADRDY_SHIFT     0u
#define ADC_INT_STATUS_ADRDY_WIDTH     1u
#define ADC_INT_STATUS_ADRDY(x)        (((uint32_t)(((uint32_t)(x))<<ADC_INT_STATUS_ADRDY_SHIFT))&ADC_INT_STATUS_ADRDY_MASK)

/* INT_STATUS Reg Mask */

#define ADC_INT_STATUS_MASK            0x000001FFu

/* INT_ENABLE Bit Fields */

#define ADC_INT_ENABLE_FIFO_RDY_IE_MASK 0x100u
#define ADC_INT_ENABLE_FIFO_RDY_IE_SHIFT 8u
#define ADC_INT_ENABLE_FIFO_RDY_IE_WIDTH 1u
#define ADC_INT_ENABLE_FIFO_RDY_IE(x)  (((uint32_t)(((uint32_t)(x))<<ADC_INT_ENABLE_FIFO_RDY_IE_SHIFT))&ADC_INT_ENABLE_FIFO_RDY_IE_MASK)

#define ADC_INT_ENABLE_ACMP_IE_MASK    0x80u
#define ADC_INT_ENABLE_ACMP_IE_SHIFT   7u
#define ADC_INT_ENABLE_ACMP_IE_WIDTH   1u
#define ADC_INT_ENABLE_ACMP_IE(x)      (((uint32_t)(((uint32_t)(x))<<ADC_INT_ENABLE_ACMP_IE_SHIFT))&ADC_INT_ENABLE_ACMP_IE_MASK)

#define ADC_INT_ENABLE_OVRIE_MASK      0x10u
#define ADC_INT_ENABLE_OVRIE_SHIFT     4u
#define ADC_INT_ENABLE_OVRIE_WIDTH     1u
#define ADC_INT_ENABLE_OVRIE(x)        (((uint32_t)(((uint32_t)(x))<<ADC_INT_ENABLE_OVRIE_SHIFT))&ADC_INT_ENABLE_OVRIE_MASK)

#define ADC_INT_ENABLE_EOSEQIE_MASK    0x8u
#define ADC_INT_ENABLE_EOSEQIE_SHIFT   3u
#define ADC_INT_ENABLE_EOSEQIE_WIDTH   1u
#define ADC_INT_ENABLE_EOSEQIE(x)      (((uint32_t)(((uint32_t)(x))<<ADC_INT_ENABLE_EOSEQIE_SHIFT))&ADC_INT_ENABLE_EOSEQIE_MASK)

#define ADC_INT_ENABLE_EOCIE_MASK      0x4u
#define ADC_INT_ENABLE_EOCIE_SHIFT     2u
#define ADC_INT_ENABLE_EOCIE_WIDTH     1u
#define ADC_INT_ENABLE_EOCIE(x)        (((uint32_t)(((uint32_t)(x))<<ADC_INT_ENABLE_EOCIE_SHIFT))&ADC_INT_ENABLE_EOCIE_MASK)

#define ADC_INT_ENABLE_EOSMPIE_MASK    0x2u
#define ADC_INT_ENABLE_EOSMPIE_SHIFT   1u
#define ADC_INT_ENABLE_EOSMPIE_WIDTH   1u
#define ADC_INT_ENABLE_EOSMPIE(x)      (((uint32_t)(((uint32_t)(x))<<ADC_INT_ENABLE_EOSMPIE_SHIFT))&ADC_INT_ENABLE_EOSMPIE_MASK)

#define ADC_INT_ENABLE_ADRDYIE_MASK    0x1u
#define ADC_INT_ENABLE_ADRDYIE_SHIFT   0u
#define ADC_INT_ENABLE_ADRDYIE_WIDTH   1u
#define ADC_INT_ENABLE_ADRDYIE(x)      (((uint32_t)(((uint32_t)(x))<<ADC_INT_ENABLE_ADRDYIE_SHIFT))&ADC_INT_ENABLE_ADRDYIE_MASK)

/* INT_ENABLE Reg Mask */

#define ADC_INT_ENABLE_MASK            0x0000019Fu

/* CONTROL Bit Fields */

#define ADC_CONTROL_ADRST_MASK         0x10u
#define ADC_CONTROL_ADRST_SHIFT        4u
#define ADC_CONTROL_ADRST_WIDTH        1u
#define ADC_CONTROL_ADRST(x)           (((uint32_t)(((uint32_t)(x))<<ADC_CONTROL_ADRST_SHIFT))&ADC_CONTROL_ADRST_MASK)

#define ADC_CONTROL_ADSTP_MASK         0x8u
#define ADC_CONTROL_ADSTP_SHIFT        3u
#define ADC_CONTROL_ADSTP_WIDTH        1u
#define ADC_CONTROL_ADSTP(x)           (((uint32_t)(((uint32_t)(x))<<ADC_CONTROL_ADSTP_SHIFT))&ADC_CONTROL_ADSTP_MASK)

#define ADC_CONTROL_ADSTART_MASK       0x4u
#define ADC_CONTROL_ADSTART_SHIFT      2u
#define ADC_CONTROL_ADSTART_WIDTH      1u
#define ADC_CONTROL_ADSTART(x)         (((uint32_t)(((uint32_t)(x))<<ADC_CONTROL_ADSTART_SHIFT))&ADC_CONTROL_ADSTART_MASK)

#define ADC_CONTROL_ADDIS_MASK         0x2u
#define ADC_CONTROL_ADDIS_SHIFT        1u
#define ADC_CONTROL_ADDIS_WIDTH        1u
#define ADC_CONTROL_ADDIS(x)           (((uint32_t)(((uint32_t)(x))<<ADC_CONTROL_ADDIS_SHIFT))&ADC_CONTROL_ADDIS_MASK)

#define ADC_CONTROL_ADEN_MASK          0x1u
#define ADC_CONTROL_ADEN_SHIFT         0u
#define ADC_CONTROL_ADEN_WIDTH         1u
#define ADC_CONTROL_ADEN(x)            (((uint32_t)(((uint32_t)(x))<<ADC_CONTROL_ADEN_SHIFT))&ADC_CONTROL_ADEN_MASK)

/* CONTROL Reg Mask */

#define ADC_CONTROL_MASK               0x0000001Fu

/* CFG1 Bit Fields */

#define ADC_CFG1_DIFF_MASK             0x80000000u
#define ADC_CFG1_DIFF_SHIFT            31u
#define ADC_CFG1_DIFF_WIDTH            1u
#define ADC_CFG1_DIFF(x)               (((uint32_t)(((uint32_t)(x))<<ADC_CFG1_DIFF_SHIFT))&ADC_CFG1_DIFF_MASK)

#define ADC_CFG1_OVRMOD_MASK           0x40000000u
#define ADC_CFG1_OVRMOD_SHIFT          30u
#define ADC_CFG1_OVRMOD_WIDTH          1u
#define ADC_CFG1_OVRMOD(x)             (((uint32_t)(((uint32_t)(x))<<ADC_CFG1_OVRMOD_SHIFT))&ADC_CFG1_OVRMOD_MASK)

#define ADC_CFG1_SEQ_LEN_MASK          0x1F000000u
#define ADC_CFG1_SEQ_LEN_SHIFT         24u
#define ADC_CFG1_SEQ_LEN_WIDTH         5u
#define ADC_CFG1_SEQ_LEN(x)            (((uint32_t)(((uint32_t)(x))<<ADC_CFG1_SEQ_LEN_SHIFT))&ADC_CFG1_SEQ_LEN_MASK)

#define ADC_CFG1_SEQ_MOD_MASK          0xC00000u
#define ADC_CFG1_SEQ_MOD_SHIFT         22u
#define ADC_CFG1_SEQ_MOD_WIDTH         2u
#define ADC_CFG1_SEQ_MOD(x)            (((uint32_t)(((uint32_t)(x))<<ADC_CFG1_SEQ_MOD_SHIFT))&ADC_CFG1_SEQ_MOD_MASK)

#define ADC_CFG1_AUTO_DIS_MASK         0x200000u
#define ADC_CFG1_AUTO_DIS_SHIFT        21u
#define ADC_CFG1_AUTO_DIS_WIDTH        1u
#define ADC_CFG1_AUTO_DIS(x)           (((uint32_t)(((uint32_t)(x))<<ADC_CFG1_AUTO_DIS_SHIFT))&ADC_CFG1_AUTO_DIS_MASK)

#define ADC_CFG1_WAIT_MASK             0x100000u
#define ADC_CFG1_WAIT_SHIFT            20u
#define ADC_CFG1_WAIT_WIDTH            1u
#define ADC_CFG1_WAIT(x)               (((uint32_t)(((uint32_t)(x))<<ADC_CFG1_WAIT_SHIFT))&ADC_CFG1_WAIT_MASK)

#define ADC_CFG1_TRIGSRC_MASK          0x70000u
#define ADC_CFG1_TRIGSRC_SHIFT         16u
#define ADC_CFG1_TRIGSRC_WIDTH         3u
#define ADC_CFG1_TRIGSRC(x)            (((uint32_t)(((uint32_t)(x))<<ADC_CFG1_TRIGSRC_SHIFT))&ADC_CFG1_TRIGSRC_MASK)

#define ADC_CFG1_TRIGMODE_MASK         0x3800u
#define ADC_CFG1_TRIGMODE_SHIFT        11u
#define ADC_CFG1_TRIGMODE_WIDTH        3u
#define ADC_CFG1_TRIGMODE(x)           (((uint32_t)(((uint32_t)(x))<<ADC_CFG1_TRIGMODE_SHIFT))&ADC_CFG1_TRIGMODE_MASK)

#define ADC_CFG1_ALIGN_MASK            0x400u
#define ADC_CFG1_ALIGN_SHIFT           10u
#define ADC_CFG1_ALIGN_WIDTH           1u
#define ADC_CFG1_ALIGN(x)              (((uint32_t)(((uint32_t)(x))<<ADC_CFG1_ALIGN_SHIFT))&ADC_CFG1_ALIGN_MASK)

#define ADC_CFG1_RES_MASK              0x300u
#define ADC_CFG1_RES_SHIFT             8u
#define ADC_CFG1_RES_WIDTH             2u
#define ADC_CFG1_RES(x)                (((uint32_t)(((uint32_t)(x))<<ADC_CFG1_RES_SHIFT))&ADC_CFG1_RES_MASK)

#define ADC_CFG1_DMAEN_MASK            0x1u
#define ADC_CFG1_DMAEN_SHIFT           0u
#define ADC_CFG1_DMAEN_WIDTH           1u
#define ADC_CFG1_DMAEN(x)              (((uint32_t)(((uint32_t)(x))<<ADC_CFG1_DMAEN_SHIFT))&ADC_CFG1_DMAEN_MASK)

/* CFG1 Reg Mask */

#define ADC_CFG1_MASK                  0xDFF73F01u

/* CFG2 Bit Fields */

#define ADC_CFG2_FWMARK_MASK           0x1F000000u
#define ADC_CFG2_FWMARK_SHIFT          24u
#define ADC_CFG2_FWMARK_WIDTH          5u
#define ADC_CFG2_FWMARK(x)             (((uint32_t)(((uint32_t)(x))<<ADC_CFG2_FWMARK_SHIFT))&ADC_CFG2_FWMARK_MASK)

#define ADC_CFG2_AVG_EN_MASK           0x40000u
#define ADC_CFG2_AVG_EN_SHIFT          18u
#define ADC_CFG2_AVG_EN_WIDTH          1u
#define ADC_CFG2_AVG_EN(x)             (((uint32_t)(((uint32_t)(x))<<ADC_CFG2_AVG_EN_SHIFT))&ADC_CFG2_AVG_EN_MASK)

#define ADC_CFG2_AVG_LEN_MASK          0x30000u
#define ADC_CFG2_AVG_LEN_SHIFT         16u
#define ADC_CFG2_AVG_LEN_WIDTH         2u
#define ADC_CFG2_AVG_LEN(x)            (((uint32_t)(((uint32_t)(x))<<ADC_CFG2_AVG_LEN_SHIFT))&ADC_CFG2_AVG_LEN_MASK)

#define ADC_CFG2_CG_ACK_MASK           0x4000u
#define ADC_CFG2_CG_ACK_SHIFT          14u
#define ADC_CFG2_CG_ACK_WIDTH          1u
#define ADC_CFG2_CG_ACK(x)             (((uint32_t)(((uint32_t)(x))<<ADC_CFG2_CG_ACK_SHIFT))&ADC_CFG2_CG_ACK_MASK)

#define ADC_CFG2_CG_MASK               0x2000u
#define ADC_CFG2_CG_SHIFT              13u
#define ADC_CFG2_CG_WIDTH              1u
#define ADC_CFG2_CG(x)                 (((uint32_t)(((uint32_t)(x))<<ADC_CFG2_CG_SHIFT))&ADC_CFG2_CG_MASK)

#define ADC_CFG2_REF_EXT_MASK          0x1000u
#define ADC_CFG2_REF_EXT_SHIFT         12u
#define ADC_CFG2_REF_EXT_WIDTH         1u
#define ADC_CFG2_REF_EXT(x)            (((uint32_t)(((uint32_t)(x))<<ADC_CFG2_REF_EXT_SHIFT))&ADC_CFG2_REF_EXT_MASK)

#define ADC_CFG2_DIV_MASK              0x300u
#define ADC_CFG2_DIV_SHIFT             8u
#define ADC_CFG2_DIV_WIDTH             2u
#define ADC_CFG2_DIV(x)                (((uint32_t)(((uint32_t)(x))<<ADC_CFG2_DIV_SHIFT))&ADC_CFG2_DIV_MASK)

#define ADC_CFG2_STCNT_MASK            0xFFu
#define ADC_CFG2_STCNT_SHIFT           0u
#define ADC_CFG2_STCNT_WIDTH           8u
#define ADC_CFG2_STCNT(x)              (((uint32_t)(((uint32_t)(x))<<ADC_CFG2_STCNT_SHIFT))&ADC_CFG2_STCNT_MASK)

/* CFG2 Reg Mask */

#define ADC_CFG2_MASK                  0x1F0773FFu

/* SMPR Bit Fields */

#define ADC_SMPR_SMP_OPT3_MASK         0xFF000000u
#define ADC_SMPR_SMP_OPT3_SHIFT        24u
#define ADC_SMPR_SMP_OPT3_WIDTH        8u
#define ADC_SMPR_SMP_OPT3(x)           (((uint32_t)(((uint32_t)(x))<<ADC_SMPR_SMP_OPT3_SHIFT))&ADC_SMPR_SMP_OPT3_MASK)

#define ADC_SMPR_SMP_OPT2_MASK         0xFF0000u
#define ADC_SMPR_SMP_OPT2_SHIFT        16u
#define ADC_SMPR_SMP_OPT2_WIDTH        8u
#define ADC_SMPR_SMP_OPT2(x)           (((uint32_t)(((uint32_t)(x))<<ADC_SMPR_SMP_OPT2_SHIFT))&ADC_SMPR_SMP_OPT2_MASK)

#define ADC_SMPR_SMP_OPT1_MASK         0xFF00u
#define ADC_SMPR_SMP_OPT1_SHIFT        8u
#define ADC_SMPR_SMP_OPT1_WIDTH        8u
#define ADC_SMPR_SMP_OPT1(x)           (((uint32_t)(((uint32_t)(x))<<ADC_SMPR_SMP_OPT1_SHIFT))&ADC_SMPR_SMP_OPT1_MASK)

#define ADC_SMPR_SMP_OPT0_MASK         0xFFu
#define ADC_SMPR_SMP_OPT0_SHIFT        0u
#define ADC_SMPR_SMP_OPT0_WIDTH        8u
#define ADC_SMPR_SMP_OPT0(x)           (((uint32_t)(((uint32_t)(x))<<ADC_SMPR_SMP_OPT0_SHIFT))&ADC_SMPR_SMP_OPT0_MASK)

/* SMPR Reg Mask */

#define ADC_SMPR_MASK                  0xFFFFFFFFu

/* CMP_CTRL Bit Fields */

#define ADC_CMP_CTRL_ACMPEN_MASK       0x80u
#define ADC_CMP_CTRL_ACMPEN_SHIFT      7u
#define ADC_CMP_CTRL_ACMPEN_WIDTH      1u
#define ADC_CMP_CTRL_ACMPEN(x)         (((uint32_t)(((uint32_t)(x))<<ADC_CMP_CTRL_ACMPEN_SHIFT))&ADC_CMP_CTRL_ACMPEN_MASK)

#define ADC_CMP_CTRL_ACMPSGL_MASK      0x40u
#define ADC_CMP_CTRL_ACMPSGL_SHIFT     6u
#define ADC_CMP_CTRL_ACMPSGL_WIDTH     1u
#define ADC_CMP_CTRL_ACMPSGL(x)        (((uint32_t)(((uint32_t)(x))<<ADC_CMP_CTRL_ACMPSGL_SHIFT))&ADC_CMP_CTRL_ACMPSGL_MASK)

#define ADC_CMP_CTRL_ACMPCH_MASK       0x3Fu
#define ADC_CMP_CTRL_ACMPCH_SHIFT      0u
#define ADC_CMP_CTRL_ACMPCH_WIDTH      6u
#define ADC_CMP_CTRL_ACMPCH(x)         (((uint32_t)(((uint32_t)(x))<<ADC_CMP_CTRL_ACMPCH_SHIFT))&ADC_CMP_CTRL_ACMPCH_MASK)

/* CMP_CTRL Reg Mask */

#define ADC_CMP_CTRL_MASK              0x000000FFu

/* CMP_TR Bit Fields */

#define ADC_CMP_TR_HT_MASK             0xFFF0000u
#define ADC_CMP_TR_HT_SHIFT            16u
#define ADC_CMP_TR_HT_WIDTH            12u
#define ADC_CMP_TR_HT(x)               (((uint32_t)(((uint32_t)(x))<<ADC_CMP_TR_HT_SHIFT))&ADC_CMP_TR_HT_MASK)

#define ADC_CMP_TR_LT_MASK             0xFFFu
#define ADC_CMP_TR_LT_SHIFT            0u
#define ADC_CMP_TR_LT_WIDTH            12u
#define ADC_CMP_TR_LT(x)               (((uint32_t)(((uint32_t)(x))<<ADC_CMP_TR_LT_SHIFT))&ADC_CMP_TR_LT_MASK)

/* CMP_TR Reg Mask */

#define ADC_CMP_TR_MASK                0x0FFF0FFFu

/* FIFO_DATA Bit Fields */

#define ADC_FIFO_DATA_FIFO_DATA_MASK   0xFFFFu
#define ADC_FIFO_DATA_FIFO_DATA_SHIFT  0u
#define ADC_FIFO_DATA_FIFO_DATA_WIDTH  16u
#define ADC_FIFO_DATA_FIFO_DATA(x)     (((uint32_t)(((uint32_t)(x))<<ADC_FIFO_DATA_FIFO_DATA_SHIFT))&ADC_FIFO_DATA_FIFO_DATA_MASK)

/* FIFO_DATA Reg Mask */

#define ADC_FIFO_DATA_MASK             0x0000FFFFu

/* SC Bit Fields */

#define ADC_SC_SMPSEL_MASK             0x300u
#define ADC_SC_SMPSEL_SHIFT            8u
#define ADC_SC_SMPSEL_WIDTH            2u
#define ADC_SC_SMPSEL(x)               (((uint32_t)(((uint32_t)(x))<<ADC_SC_SMPSEL_SHIFT))&ADC_SC_SMPSEL_MASK)

#define ADC_SC_COCO_MASK               0x80u
#define ADC_SC_COCO_SHIFT              7u
#define ADC_SC_COCO_WIDTH              1u
#define ADC_SC_COCO(x)                 (((uint32_t)(((uint32_t)(x))<<ADC_SC_COCO_SHIFT))&ADC_SC_COCO_MASK)

#define ADC_SC_AIEN_MASK               0x40u
#define ADC_SC_AIEN_SHIFT              6u
#define ADC_SC_AIEN_WIDTH              1u
#define ADC_SC_AIEN(x)                 (((uint32_t)(((uint32_t)(x))<<ADC_SC_AIEN_SHIFT))&ADC_SC_AIEN_MASK)

#define ADC_SC_CHS_MASK                0x3Fu
#define ADC_SC_CHS_SHIFT               0u
#define ADC_SC_CHS_WIDTH               6u
#define ADC_SC_CHS(x)                  (((uint32_t)(((uint32_t)(x))<<ADC_SC_CHS_SHIFT))&ADC_SC_CHS_MASK)

/* SC Reg Mask */

#define ADC_SC_MASK                    0x000003FFu

/* RESULT Bit Fields */

#define ADC_RESULT_RESULT_MASK         0xFFFFu
#define ADC_RESULT_RESULT_SHIFT        0u
#define ADC_RESULT_RESULT_WIDTH        16u
#define ADC_RESULT_RESULT(x)           (((uint32_t)(((uint32_t)(x))<<ADC_RESULT_RESULT_SHIFT))&ADC_RESULT_RESULT_MASK)

/* RESULT0 Reg Mask */

#define ADC_RESULT_MASK                0x0000FFFFu

#endif /* __ARCH_ARM_SRC_FC7300_HARDWARE_FC7300_ADC_H */
