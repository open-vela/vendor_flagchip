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

#ifndef __VENDOR_FLAGCHIP_CHIPS_FC7300_HARDWARE_FC7300_SCG_H
#define __VENDOR_FLAGCHIP_CHIPS_FC7300_HARDWARE_FC7300_SCG_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>
#include <hardware/fc7300_memorymap.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* SCG Register Offsets *****************************************************/

#define FC7300_SCG_CSR_OFFSET         0x0010  /* Clock Status Register */
#define FC7300_SCG_CCR_OFFSET         0x0014  /* Run Clock Control Register */
#define FC7300_SCG_CLKOUTCFG_OFFSET   0x0020  /* SCG CLKOUT Configuration Register */
#define FC7300_SCG_SLPWDG_OFFSET      0x0024
#define FC7300_SCG_WKPWDG_OFFSET      0x0028
#define FC7300_SCG_CRCCSR_OFFSET      0x0030
#define FC7300_SCG_CRCRES_OFFSET      0x0034
#define FC7300_SCG_FOSCCSR_OFFSET     0x0100  /* System OSC Control Status Register */
#define FC7300_SCG_FOSCDIV_OFFSET     0x0104  /* System OSC Divide Register */
#define FC7300_SCG_FOSCCFG_OFFSET     0x0108  /* System Oscillator Configuration Register */
#define FC7300_SCG_SIRCCSR_OFFSET     0x0200  /* Slow IRC Control Status Register */
#define FC7300_SCG_SIRCDIV_OFFSET     0x0204  /* Slow IRC Divide Register */
#define FC7300_SCG_SIRCTCFG_OFFSET    0x020C  /* Slow IRC Auto Trim Configuration Register */
#define FC7300_SCG_SIRCSTAT_OFFSET    0x0218  /* Slow IRC Software Trim Status Register */
#define FC7300_SCG_FIRCCSR_OFFSET     0x0300  /* Fast IRC Control Status Register */
#define FC7300_SCG_FIRCDIV_OFFSET     0x0304  /* Fast IRC Divide Register */
#define FC7300_SCG_FIRCCFG_OFFSET     0x0308  /* Fast IRC Configuration Register */
#define FC7300_SCG_FIRCTCFG_OFFSET    0x030C  /* Fast IRC Auto Trim Configuration Register */
#define FC7300_SCG_FIRCSTAT_OFFSET    0x0318  /* Fast IRC Software Trim Status Register */
#define FC7300_SCG_SOSCCSR_OFFSET     0x0400  /* Slow OSC Control Status Register */
#define FC7300_SCG_SOSCCFG_OFFSET     0x0404  /* Slow OSC Configuration Register */
#define FC7300_SCG_PLL1CSR_OFFSET     0x0500  /* System PLL1 Control Status Register */
#define FC7300_SCG_PLL1DIV_OFFSET     0x0504  /* System PLL1 Divide Register */
#define FC7300_SCG_PLL1CFG_OFFSET     0x0508  /* System PLL1 Configuration Register */
#define FC7300_SCG_PLL0CSR_OFFSET     0x0600  /* System PLL0 Control Status Register */
#define FC7300_SCG_PLL0DIV_OFFSET     0x0604  /* System PLL0 Divide Register */
#define FC7300_SCG_PLL0CFG_OFFSET     0x0608  /* System PLL0 Configuration Register */
#define FC7300_SCG_SIRC32KCSR_OFFSET  0x0700  /* 32K Slow IRC Control Status Register */

/* SCG Register Addresses ***************************************************/

#define FC7300_SCG_CSR        (FC7300_SCG_BASE + FC7300_SCG_CSR_OFFSET)
#define FC7300_SCG_CCR        (FC7300_SCG_BASE + FC7300_SCG_CCR_OFFSET)
#define FC7300_SCG_CLKOUTCFG  (FC7300_SCG_BASE + FC7300_SCG_CLKOUTCFG_OFFSET)
#define FC7300_SCG_SLPWDG     (FC7300_SCG_BASE + FC7300_SCG_SLPWDG_OFFSET)
#define FC7300_SCG_WKPWDG     (FC7300_SCG_BASE + FC7300_SCG_WKPWDG_OFFSET)
#define FC7300_SCG_CRCCSR     (FC7300_SCG_BASE + FC7300_SCG_CRCCSR_OFFSET)
#define FC7300_SCG_CRCRES     (FC7300_SCG_BASE + FC7300_SCG_CRCRES_OFFSET)
#define FC7300_SCG_FOSCCSR    (FC7300_SCG_BASE + FC7300_SCG_FOSCCSR_OFFSET)
#define FC7300_SCG_FOSCDIV    (FC7300_SCG_BASE + FC7300_SCG_FOSCDIV_OFFSET)
#define FC7300_SCG_FOSCCFG    (FC7300_SCG_BASE + FC7300_SCG_FOSCCFG_OFFSET)
#define FC7300_SCG_SIRCCSR    (FC7300_SCG_BASE + FC7300_SCG_SIRCCSR_OFFSET)
#define FC7300_SCG_SIRCDIV    (FC7300_SCG_BASE + FC7300_SCG_SIRCDIV_OFFSET)
#define FC7300_SCG_SIRCTCFG   (FC7300_SCG_BASE + FC7300_SCG_SIRCTCFG_OFFSET)
#define FC7300_SCG_SIRCSTAT   (FC7300_SCG_BASE + FC7300_SCG_SIRCSTAT_OFFSET)
#define FC7300_SCG_FIRCCSR    (FC7300_SCG_BASE + FC7300_SCG_FIRCCSR_OFFSET)
#define FC7300_SCG_FIRCDIV    (FC7300_SCG_BASE + FC7300_SCG_FIRCDIV_OFFSET)
#define FC7300_SCG_FIRCCFG    (FC7300_SCG_BASE + FC7300_SCG_FIRCCFG_OFFSET)
#define FC7300_SCG_FIRCTCFG   (FC7300_SCG_BASE + FC7300_SCG_FIRCTCFG_OFFSET)
#define FC7300_SCG_FIRCSTAT   (FC7300_SCG_BASE + FC7300_SCG_FIRCSTAT_OFFSET)
#define FC7300_SCG_SOSCCSR    (FC7300_SCG_BASE + FC7300_SCG_SOSCCSR_OFFSET)
#define FC7300_SCG_SOSCCFG    (FC7300_SCG_BASE + FC7300_SCG_SOSCCFG_OFFSET)
#define FC7300_SCG_PLL1CSR    (FC7300_SCG_BASE + FC7300_SCG_PLL1CSR_OFFSET)
#define FC7300_SCG_PLL1DIV    (FC7300_SCG_BASE + FC7300_SCG_PLL1DIV_OFFSET)
#define FC7300_SCG_PLL1CFG    (FC7300_SCG_BASE + FC7300_SCG_PLL1CFG_OFFSET)
#define FC7300_SCG_PLL0CSR    (FC7300_SCG_BASE + FC7300_SCG_PLL0CSR_OFFSET)
#define FC7300_SCG_PLL0DIV    (FC7300_SCG_BASE + FC7300_SCG_PLL0DIV_OFFSET)
#define FC7300_SCG_PLL0CFG    (FC7300_SCG_BASE + FC7300_SCG_PLL0CFG_OFFSET)
#define FC7300_SCG_SIRC32KCSR (FC7300_SCG_BASE + FC7300_SCG_SIRC32KCSR_OFFSET)

/* SCG Register Bitfield Definitions ****************************************/

/* CSR Bit Fields */

#define SCG_CSR_CCR_UPRD_MASK          0x80000000u
#define SCG_CSR_CCR_UPRD_SHIFT         31u
#define SCG_CSR_CCR_UPRD_WIDTH         1u
#define SCG_CSR_CCR_UPRD(x)            (((uint32_t)(((uint32_t)(x))<<SCG_CSR_CCR_UPRD_SHIFT))&SCG_CSR_CCR_UPRD_MASK)
#define SCG_CSR_SCS_MASK               0xF000000u
#define SCG_CSR_SCS_SHIFT              24u
#define SCG_CSR_SCS_WIDTH              4u
#define SCG_CSR_SCS(x)                 (((uint32_t)(((uint32_t)(x))<<SCG_CSR_SCS_SHIFT))&SCG_CSR_SCS_MASK)
#define SCG_CSR_DIVCORE_MASK           0x30000u
#define SCG_CSR_DIVCORE_SHIFT          16u
#define SCG_CSR_DIVCORE_WIDTH          2u
#define SCG_CSR_DIVCORE(x)             (((uint32_t)(((uint32_t)(x))<<SCG_CSR_DIVCORE_SHIFT))&SCG_CSR_DIVCORE_MASK)
#define SCG_CSR_DIVBUS_MASK            0x30u
#define SCG_CSR_DIVBUS_SHIFT           4u
#define SCG_CSR_DIVBUS_WIDTH           2u
#define SCG_CSR_DIVBUS(x)              (((uint32_t)(((uint32_t)(x))<<SCG_CSR_DIVBUS_SHIFT))&SCG_CSR_DIVBUS_MASK)
#define SCG_CSR_DIVSLOW_MASK           0x3u
#define SCG_CSR_DIVSLOW_SHIFT          0u
#define SCG_CSR_DIVSLOW_WIDTH          2u
#define SCG_CSR_DIVSLOW(x)             (((uint32_t)(((uint32_t)(x))<<SCG_CSR_DIVSLOW_SHIFT))&SCG_CSR_DIVSLOW_MASK)

/* CSR Reg Mask */

#define SCG_CSR_MASK                   0x8F030033u

/* CCR Bit Fields */

#define SCG_CCR_SYSCLK_CME_MASK        0x80000000u
#define SCG_CCR_SYSCLK_CME_SHIFT       31u
#define SCG_CCR_SYSCLK_CME_WIDTH       1u
#define SCG_CCR_SYSCLK_CME(x)          (((uint32_t)(((uint32_t)(x))<<SCG_CCR_SYSCLK_CME_SHIFT))&SCG_CCR_SYSCLK_CME_MASK)
#define SCG_CCR_SCS_MASK               0xF000000u
#define SCG_CCR_SCS_SHIFT              24u
#define SCG_CCR_SCS_WIDTH              4u
#define SCG_CCR_SCS(x)                 (((uint32_t)(((uint32_t)(x))<<SCG_CCR_SCS_SHIFT))&SCG_CCR_SCS_MASK)
#define SCG_CCR_DIVCORE_MASK           0x30000u
#define SCG_CCR_DIVCORE_SHIFT          16u
#define SCG_CCR_DIVCORE_WIDTH          2u
#define SCG_CCR_DIVCORE(x)             (((uint32_t)(((uint32_t)(x))<<SCG_CCR_DIVCORE_SHIFT))&SCG_CCR_DIVCORE_MASK)
#define SCG_CCR_DIVBUS_MASK            0x30u
#define SCG_CCR_DIVBUS_SHIFT           4u
#define SCG_CCR_DIVBUS_WIDTH           2u
#define SCG_CCR_DIVBUS(x)              (((uint32_t)(((uint32_t)(x))<<SCG_CCR_DIVBUS_SHIFT))&SCG_CCR_DIVBUS_MASK)
#define SCG_CCR_DIVSLOW_MASK           0x3u
#define SCG_CCR_DIVSLOW_SHIFT          0u
#define SCG_CCR_DIVSLOW_WIDTH          2u
#define SCG_CCR_DIVSLOW(x)             (((uint32_t)(((uint32_t)(x))<<SCG_CCR_DIVSLOW_SHIFT))&SCG_CCR_DIVSLOW_MASK)

/* CCR Reg Mask */

#define SCG_CCR_MASK                   0x8F030033u

/* CLKOUTCFG Bit Fields */

#define SCG_CLKOUTCFG_NVMCLK_FIRC_MASK 0x80000000u
#define SCG_CLKOUTCFG_NVMCLK_FIRC_SHIFT 31u
#define SCG_CLKOUTCFG_NVMCLK_FIRC_WIDTH 1u
#define SCG_CLKOUTCFG_NVMCLK_FIRC(x)   (((uint32_t)(((uint32_t)(x))<<SCG_CLKOUTCFG_NVMCLK_FIRC_SHIFT))&SCG_CLKOUTCFG_NVMCLK_FIRC_MASK)
#define SCG_CLKOUTCFG_NVMCLK_SIRC_MASK 0x40000000u
#define SCG_CLKOUTCFG_NVMCLK_SIRC_SHIFT 30u
#define SCG_CLKOUTCFG_NVMCLK_SIRC_WIDTH 1u
#define SCG_CLKOUTCFG_NVMCLK_SIRC(x)   (((uint32_t)(((uint32_t)(x))<<SCG_CLKOUTCFG_NVMCLK_SIRC_SHIFT))&SCG_CLKOUTCFG_NVMCLK_SIRC_MASK)
#define SCG_CLKOUTCFG_CMU4CLK_FOSC_MASK 0x20000000u
#define SCG_CLKOUTCFG_CMU4CLK_FOSC_SHIFT 29u
#define SCG_CLKOUTCFG_CMU4CLK_FOSC_WIDTH 1u
#define SCG_CLKOUTCFG_CMU4CLK_FOSC(x)  (((uint32_t)(((uint32_t)(x))<<SCG_CLKOUTCFG_CMU4CLK_FOSC_SHIFT))&SCG_CLKOUTCFG_CMU4CLK_FOSC_MASK)
#define SCG_CLKOUTCFG_CMU4CLK_SIRC_MASK 0x10000000u
#define SCG_CLKOUTCFG_CMU4CLK_SIRC_SHIFT 28u
#define SCG_CLKOUTCFG_CMU4CLK_SIRC_WIDTH 1u
#define SCG_CLKOUTCFG_CMU4CLK_SIRC(x)  (((uint32_t)(((uint32_t)(x))<<SCG_CLKOUTCFG_CMU4CLK_SIRC_SHIFT))&SCG_CLKOUTCFG_CMU4CLK_SIRC_MASK)
#define SCG_CLKOUTCFG_CLKOUTSEL_MASK   0xF000000u
#define SCG_CLKOUTCFG_CLKOUTSEL_SHIFT  24u
#define SCG_CLKOUTCFG_CLKOUTSEL_WIDTH  4u
#define SCG_CLKOUTCFG_CLKOUTSEL(x)     (((uint32_t)(((uint32_t)(x))<<SCG_CLKOUTCFG_CLKOUTSEL_SHIFT))&SCG_CLKOUTCFG_CLKOUTSEL_MASK)

/* CLKOUTCFG Reg Mask */

#define SCG_CLKOUTCFG_MASK             0xFF000000u

/* SLPWDG Bit Fields */

#define SCG_SLPWDG_EN_MASK             0x80000000u
#define SCG_SLPWDG_EN_SHIFT            31u
#define SCG_SLPWDG_EN_WIDTH            1u
#define SCG_SLPWDG_EN(x)               (((uint32_t)(((uint32_t)(x))<<SCG_SLPWDG_EN_SHIFT))&SCG_SLPWDG_EN_MASK)
#define SCG_SLPWDG_MSB_MASK            0xFF0000u
#define SCG_SLPWDG_MSB_SHIFT           16u
#define SCG_SLPWDG_MSB_WIDTH           8u
#define SCG_SLPWDG_MSB(x)              (((uint32_t)(((uint32_t)(x))<<SCG_SLPWDG_MSB_SHIFT))&SCG_SLPWDG_MSB_MASK)

/* SLPWDG Reg Mask */

#define SCG_SLPWDG_MASK                0x80FF0000u

/* WKPWDG Bit Fields */

#define SCG_WKPWDG_EN_MASK             0x80000000u
#define SCG_WKPWDG_EN_SHIFT            31u
#define SCG_WKPWDG_EN_WIDTH            1u
#define SCG_WKPWDG_EN(x)               (((uint32_t)(((uint32_t)(x))<<SCG_WKPWDG_EN_SHIFT))&SCG_WKPWDG_EN_MASK)
#define SCG_WKPWDG_MSB_MASK            0xFF0000u
#define SCG_WKPWDG_MSB_SHIFT           16u
#define SCG_WKPWDG_MSB_WIDTH           8u
#define SCG_WKPWDG_MSB(x)              (((uint32_t)(((uint32_t)(x))<<SCG_WKPWDG_MSB_SHIFT))&SCG_WKPWDG_MSB_MASK)

/* WKPWDG Reg Mask */

#define SCG_WKPWDG_MASK                0x80FF0000u

/* CRCCSR Bit Fields */

#define SCG_CRCCSR_ERR_MASK            0x20u
#define SCG_CRCCSR_ERR_SHIFT           5u
#define SCG_CRCCSR_ERR_WIDTH           1u
#define SCG_CRCCSR_ERR(x)              (((uint32_t)(((uint32_t)(x))<<SCG_CRCCSR_ERR_SHIFT))&SCG_CRCCSR_ERR_MASK)
#define SCG_CRCCSR_BUSY_MASK           0x10u
#define SCG_CRCCSR_BUSY_SHIFT          4u
#define SCG_CRCCSR_BUSY_WIDTH          1u
#define SCG_CRCCSR_BUSY(x)             (((uint32_t)(((uint32_t)(x))<<SCG_CRCCSR_BUSY_SHIFT))&SCG_CRCCSR_BUSY_MASK)
#define SCG_CRCCSR_EOEN_MASK           0x8u
#define SCG_CRCCSR_EOEN_SHIFT          3u
#define SCG_CRCCSR_EOEN_WIDTH          1u
#define SCG_CRCCSR_EOEN(x)             (((uint32_t)(((uint32_t)(x))<<SCG_CRCCSR_EOEN_SHIFT))&SCG_CRCCSR_EOEN_MASK)
#define SCG_CRCCSR_CHKEN_MASK          0x4u
#define SCG_CRCCSR_CHKEN_SHIFT         2u
#define SCG_CRCCSR_CHKEN_WIDTH         1u
#define SCG_CRCCSR_CHKEN(x)            (((uint32_t)(((uint32_t)(x))<<SCG_CRCCSR_CHKEN_SHIFT))&SCG_CRCCSR_CHKEN_MASK)
#define SCG_CRCCSR_TRGEN_MASK          0x2u
#define SCG_CRCCSR_TRGEN_SHIFT         1u
#define SCG_CRCCSR_TRGEN_WIDTH         1u
#define SCG_CRCCSR_TRGEN(x)            (((uint32_t)(((uint32_t)(x))<<SCG_CRCCSR_TRGEN_SHIFT))&SCG_CRCCSR_TRGEN_MASK)
#define SCG_CRCCSR_GEN_MASK            0x1u
#define SCG_CRCCSR_GEN_SHIFT           0u
#define SCG_CRCCSR_GEN_WIDTH           1u
#define SCG_CRCCSR_GEN(x)              (((uint32_t)(((uint32_t)(x))<<SCG_CRCCSR_GEN_SHIFT))&SCG_CRCCSR_GEN_MASK)

/* CRCCSR Reg Mask */

#define SCG_CRCCSR_MASK                0x0000003Fu

/* CRCRES Bit Fields */

#define SCG_CRCRES_RESULT_MASK         0xFFFFFFFFu
#define SCG_CRCRES_RESULT_SHIFT        0u
#define SCG_CRCRES_RESULT_WIDTH        32u
#define SCG_CRCRES_RESULT(x)           (((uint32_t)(((uint32_t)(x))<<SCG_CRCRES_RESULT_SHIFT))&SCG_CRCRES_RESULT_MASK)

/* CRCRES Reg Mask */

#define SCG_CRCRES_MASK                0xFFFFFFFFu

/* FOSCCSR Bit Fields */

#define SCG_FOSCCSR_ERR_MASK           0x4000000u
#define SCG_FOSCCSR_ERR_SHIFT          26u
#define SCG_FOSCCSR_ERR_WIDTH          1u
#define SCG_FOSCCSR_ERR(x)             (((uint32_t)(((uint32_t)(x))<<SCG_FOSCCSR_ERR_SHIFT))&SCG_FOSCCSR_ERR_MASK)
#define SCG_FOSCCSR_SEL_MASK           0x2000000u
#define SCG_FOSCCSR_SEL_SHIFT          25u
#define SCG_FOSCCSR_SEL_WIDTH          1u
#define SCG_FOSCCSR_SEL(x)             (((uint32_t)(((uint32_t)(x))<<SCG_FOSCCSR_SEL_SHIFT))&SCG_FOSCCSR_SEL_MASK)
#define SCG_FOSCCSR_VLD_MASK           0x1000000u
#define SCG_FOSCCSR_VLD_SHIFT          24u
#define SCG_FOSCCSR_VLD_WIDTH          1u
#define SCG_FOSCCSR_VLD(x)             (((uint32_t)(((uint32_t)(x))<<SCG_FOSCCSR_VLD_SHIFT))&SCG_FOSCCSR_VLD_MASK)
#define SCG_FOSCCSR_LK_MASK            0x800000u
#define SCG_FOSCCSR_LK_SHIFT           23u
#define SCG_FOSCCSR_LK_WIDTH           1u
#define SCG_FOSCCSR_LK(x)              (((uint32_t)(((uint32_t)(x))<<SCG_FOSCCSR_LK_SHIFT))&SCG_FOSCCSR_LK_MASK)
#define SCG_FOSCCSR_CMRE_MASK          0x20000u
#define SCG_FOSCCSR_CMRE_SHIFT         17u
#define SCG_FOSCCSR_CMRE_WIDTH         1u
#define SCG_FOSCCSR_CMRE(x)            (((uint32_t)(((uint32_t)(x))<<SCG_FOSCCSR_CMRE_SHIFT))&SCG_FOSCCSR_CMRE_MASK)
#define SCG_FOSCCSR_CM_MASK            0x10000u
#define SCG_FOSCCSR_CM_SHIFT           16u
#define SCG_FOSCCSR_CM_WIDTH           1u
#define SCG_FOSCCSR_CM(x)              (((uint32_t)(((uint32_t)(x))<<SCG_FOSCCSR_CM_SHIFT))&SCG_FOSCCSR_CM_MASK)
#define SCG_FOSCCSR_STEN_MASK          0x2u
#define SCG_FOSCCSR_STEN_SHIFT         1u
#define SCG_FOSCCSR_STEN_WIDTH         1u
#define SCG_FOSCCSR_STEN(x)            (((uint32_t)(((uint32_t)(x))<<SCG_FOSCCSR_STEN_SHIFT))&SCG_FOSCCSR_STEN_MASK)
#define SCG_FOSCCSR_EN_MASK            0x1u
#define SCG_FOSCCSR_EN_SHIFT           0u
#define SCG_FOSCCSR_EN_WIDTH           1u
#define SCG_FOSCCSR_EN(x)              (((uint32_t)(((uint32_t)(x))<<SCG_FOSCCSR_EN_SHIFT))&SCG_FOSCCSR_EN_MASK)

/* FOSCCSR Reg Mask */

#define SCG_FOSCCSR_MASK               0x07830003u

/* FOSCDIV Bit Fields */

#define SCG_FOSCDIV_DIVL_ACK_MASK      0x40000000u
#define SCG_FOSCDIV_DIVL_ACK_SHIFT     30u
#define SCG_FOSCDIV_DIVL_ACK_WIDTH     1u
#define SCG_FOSCDIV_DIVL_ACK(x)        (((uint32_t)(((uint32_t)(x))<<SCG_FOSCDIV_DIVL_ACK_SHIFT))&SCG_FOSCDIV_DIVL_ACK_MASK)
#define SCG_FOSCDIV_DIVM_ACK_MASK      0x20000000u
#define SCG_FOSCDIV_DIVM_ACK_SHIFT     29u
#define SCG_FOSCDIV_DIVM_ACK_WIDTH     1u
#define SCG_FOSCDIV_DIVM_ACK(x)        (((uint32_t)(((uint32_t)(x))<<SCG_FOSCDIV_DIVM_ACK_SHIFT))&SCG_FOSCDIV_DIVM_ACK_MASK)
#define SCG_FOSCDIV_DIVH_ACK_MASK      0x10000000u
#define SCG_FOSCDIV_DIVH_ACK_SHIFT     28u
#define SCG_FOSCDIV_DIVH_ACK_WIDTH     1u
#define SCG_FOSCDIV_DIVH_ACK(x)        (((uint32_t)(((uint32_t)(x))<<SCG_FOSCDIV_DIVH_ACK_SHIFT))&SCG_FOSCDIV_DIVH_ACK_MASK)
#define SCG_FOSCDIV_DIVL_EN_MASK       0x4000000u
#define SCG_FOSCDIV_DIVL_EN_SHIFT      26u
#define SCG_FOSCDIV_DIVL_EN_WIDTH      1u
#define SCG_FOSCDIV_DIVL_EN(x)         (((uint32_t)(((uint32_t)(x))<<SCG_FOSCDIV_DIVL_EN_SHIFT))&SCG_FOSCDIV_DIVL_EN_MASK)
#define SCG_FOSCDIV_DIVM_EN_MASK       0x2000000u
#define SCG_FOSCDIV_DIVM_EN_SHIFT      25u
#define SCG_FOSCDIV_DIVM_EN_WIDTH      1u
#define SCG_FOSCDIV_DIVM_EN(x)         (((uint32_t)(((uint32_t)(x))<<SCG_FOSCDIV_DIVM_EN_SHIFT))&SCG_FOSCDIV_DIVM_EN_MASK)
#define SCG_FOSCDIV_DIVH_EN_MASK       0x1000000u
#define SCG_FOSCDIV_DIVH_EN_SHIFT      24u
#define SCG_FOSCDIV_DIVH_EN_WIDTH      1u
#define SCG_FOSCDIV_DIVH_EN(x)         (((uint32_t)(((uint32_t)(x))<<SCG_FOSCDIV_DIVH_EN_SHIFT))&SCG_FOSCDIV_DIVH_EN_MASK)
#define SCG_FOSCDIV_DIVL_MASK          0x70000u
#define SCG_FOSCDIV_DIVL_SHIFT         16u
#define SCG_FOSCDIV_DIVL_WIDTH         3u
#define SCG_FOSCDIV_DIVL(x)            (((uint32_t)(((uint32_t)(x))<<SCG_FOSCDIV_DIVL_SHIFT))&SCG_FOSCDIV_DIVL_MASK)
#define SCG_FOSCDIV_DIVM_MASK          0x700u
#define SCG_FOSCDIV_DIVM_SHIFT         8u
#define SCG_FOSCDIV_DIVM_WIDTH         3u
#define SCG_FOSCDIV_DIVM(x)            (((uint32_t)(((uint32_t)(x))<<SCG_FOSCDIV_DIVM_SHIFT))&SCG_FOSCDIV_DIVM_MASK)
#define SCG_FOSCDIV_DIVH_MASK          0x7u
#define SCG_FOSCDIV_DIVH_SHIFT         0u
#define SCG_FOSCDIV_DIVH_WIDTH         3u
#define SCG_FOSCDIV_DIVH(x)            (((uint32_t)(((uint32_t)(x))<<SCG_FOSCDIV_DIVH_SHIFT))&SCG_FOSCDIV_DIVH_MASK)

/* FOSCDIV Reg Mask */

#define SCG_FOSCDIV_MASK               0x77070707u

/* FOSCCFG Bit Fields */

#define SCG_FOSCCFG_BYPASS_MASK        0x80000000u
#define SCG_FOSCCFG_BYPASS_SHIFT       31u
#define SCG_FOSCCFG_BYPASS_WIDTH       1u
#define SCG_FOSCCFG_BYPASS(x)          (((uint32_t)(((uint32_t)(x))<<SCG_FOSCCFG_BYPASS_SHIFT))&SCG_FOSCCFG_BYPASS_MASK)
#define SCG_FOSCCFG_COMP_EN_MASK       0x1000000u
#define SCG_FOSCCFG_COMP_EN_SHIFT      24u
#define SCG_FOSCCFG_COMP_EN_WIDTH      1u
#define SCG_FOSCCFG_COMP_EN(x)         (((uint32_t)(((uint32_t)(x))<<SCG_FOSCCFG_COMP_EN_SHIFT))&SCG_FOSCCFG_COMP_EN_MASK)
#define SCG_FOSCCFG_EOCV_MASK          0xFF0000u
#define SCG_FOSCCFG_EOCV_SHIFT         16u
#define SCG_FOSCCFG_EOCV_WIDTH         8u
#define SCG_FOSCCFG_EOCV(x)            (((uint32_t)(((uint32_t)(x))<<SCG_FOSCCFG_EOCV_SHIFT))&SCG_FOSCCFG_EOCV_MASK)
#define SCG_FOSCCFG_GM_SEL_MASK        0xF000u
#define SCG_FOSCCFG_GM_SEL_SHIFT       12u
#define SCG_FOSCCFG_GM_SEL_WIDTH       4u
#define SCG_FOSCCFG_GM_SEL(x)          (((uint32_t)(((uint32_t)(x))<<SCG_FOSCCFG_GM_SEL_SHIFT))&SCG_FOSCCFG_GM_SEL_MASK)
#define SCG_FOSCCFG_ALC_D_MASK         0x800u
#define SCG_FOSCCFG_ALC_D_SHIFT        11u
#define SCG_FOSCCFG_ALC_D_WIDTH        1u
#define SCG_FOSCCFG_ALC_D(x)           (((uint32_t)(((uint32_t)(x))<<SCG_FOSCCFG_ALC_D_SHIFT))&SCG_FOSCCFG_ALC_D_MASK)
#define SCG_FOSCCFG_HYST_D_MASK        0x400u
#define SCG_FOSCCFG_HYST_D_SHIFT       10u
#define SCG_FOSCCFG_HYST_D_WIDTH       1u
#define SCG_FOSCCFG_HYST_D(x)          (((uint32_t)(((uint32_t)(x))<<SCG_FOSCCFG_HYST_D_SHIFT))&SCG_FOSCCFG_HYST_D_MASK)

/* FOSCCFG Reg Mask */

#define SCG_FOSCCFG_MASK               0x81FFFC00u

/* SIRCCSR Bit Fields */

#define SCG_SIRCCSR_ERR_MASK           0x4000000u
#define SCG_SIRCCSR_ERR_SHIFT          26u
#define SCG_SIRCCSR_ERR_WIDTH          1u
#define SCG_SIRCCSR_ERR(x)             (((uint32_t)(((uint32_t)(x))<<SCG_SIRCCSR_ERR_SHIFT))&SCG_SIRCCSR_ERR_MASK)
#define SCG_SIRCCSR_VLD_MASK           0x1000000u
#define SCG_SIRCCSR_VLD_SHIFT          24u
#define SCG_SIRCCSR_VLD_WIDTH          1u
#define SCG_SIRCCSR_VLD(x)             (((uint32_t)(((uint32_t)(x))<<SCG_SIRCCSR_VLD_SHIFT))&SCG_SIRCCSR_VLD_MASK)
#define SCG_SIRCCSR_LK_MASK            0x800000u
#define SCG_SIRCCSR_LK_SHIFT           23u
#define SCG_SIRCCSR_LK_WIDTH           1u
#define SCG_SIRCCSR_LK(x)              (((uint32_t)(((uint32_t)(x))<<SCG_SIRCCSR_LK_SHIFT))&SCG_SIRCCSR_LK_MASK)
#define SCG_SIRCCSR_CM_MASK            0x10000u
#define SCG_SIRCCSR_CM_SHIFT           16u
#define SCG_SIRCCSR_CM_WIDTH           1u
#define SCG_SIRCCSR_CM(x)              (((uint32_t)(((uint32_t)(x))<<SCG_SIRCCSR_CM_SHIFT))&SCG_SIRCCSR_CM_MASK)
#define SCG_SIRCCSR_TRLK_MASK          0x400u
#define SCG_SIRCCSR_TRLK_SHIFT         10u
#define SCG_SIRCCSR_TRLK_WIDTH         1u
#define SCG_SIRCCSR_TRLK(x)            (((uint32_t)(((uint32_t)(x))<<SCG_SIRCCSR_TRLK_SHIFT))&SCG_SIRCCSR_TRLK_MASK)
#define SCG_SIRCCSR_TRUP_MASK          0x200u
#define SCG_SIRCCSR_TRUP_SHIFT         9u
#define SCG_SIRCCSR_TRUP_WIDTH         1u
#define SCG_SIRCCSR_TRUP(x)            (((uint32_t)(((uint32_t)(x))<<SCG_SIRCCSR_TRUP_SHIFT))&SCG_SIRCCSR_TRUP_MASK)
#define SCG_SIRCCSR_TREN_MASK          0x100u
#define SCG_SIRCCSR_TREN_SHIFT         8u
#define SCG_SIRCCSR_TREN_WIDTH         1u
#define SCG_SIRCCSR_TREN(x)            (((uint32_t)(((uint32_t)(x))<<SCG_SIRCCSR_TREN_SHIFT))&SCG_SIRCCSR_TREN_MASK)
#define SCG_SIRCCSR_LPEN_MASK          0x4u
#define SCG_SIRCCSR_LPEN_SHIFT         2u
#define SCG_SIRCCSR_LPEN_WIDTH         1u
#define SCG_SIRCCSR_LPEN(x)            (((uint32_t)(((uint32_t)(x))<<SCG_SIRCCSR_LPEN_SHIFT))&SCG_SIRCCSR_LPEN_MASK)
#define SCG_SIRCCSR_STEN_MASK          0x2u
#define SCG_SIRCCSR_STEN_SHIFT         1u
#define SCG_SIRCCSR_STEN_WIDTH         1u
#define SCG_SIRCCSR_STEN(x)            (((uint32_t)(((uint32_t)(x))<<SCG_SIRCCSR_STEN_SHIFT))&SCG_SIRCCSR_STEN_MASK)

/* SIRCCSR Reg Mask */

#define SCG_SIRCCSR_MASK               0x05810706u

/* SIRCDIV Bit Fields */

#define SCG_SIRCDIV_DIVL_ACK_MASK      0x40000000u
#define SCG_SIRCDIV_DIVL_ACK_SHIFT     30u
#define SCG_SIRCDIV_DIVL_ACK_WIDTH     1u
#define SCG_SIRCDIV_DIVL_ACK(x)        (((uint32_t)(((uint32_t)(x))<<SCG_SIRCDIV_DIVL_ACK_SHIFT))&SCG_SIRCDIV_DIVL_ACK_MASK)
#define SCG_SIRCDIV_DIVM_ACK_MASK      0x20000000u
#define SCG_SIRCDIV_DIVM_ACK_SHIFT     29u
#define SCG_SIRCDIV_DIVM_ACK_WIDTH     1u
#define SCG_SIRCDIV_DIVM_ACK(x)        (((uint32_t)(((uint32_t)(x))<<SCG_SIRCDIV_DIVM_ACK_SHIFT))&SCG_SIRCDIV_DIVM_ACK_MASK)
#define SCG_SIRCDIV_DIVH_ACK_MASK      0x10000000u
#define SCG_SIRCDIV_DIVH_ACK_SHIFT     28u
#define SCG_SIRCDIV_DIVH_ACK_WIDTH     1u
#define SCG_SIRCDIV_DIVH_ACK(x)        (((uint32_t)(((uint32_t)(x))<<SCG_SIRCDIV_DIVH_ACK_SHIFT))&SCG_SIRCDIV_DIVH_ACK_MASK)
#define SCG_SIRCDIV_DIVL_EN_MASK       0x4000000u
#define SCG_SIRCDIV_DIVL_EN_SHIFT      26u
#define SCG_SIRCDIV_DIVL_EN_WIDTH      1u
#define SCG_SIRCDIV_DIVL_EN(x)         (((uint32_t)(((uint32_t)(x))<<SCG_SIRCDIV_DIVL_EN_SHIFT))&SCG_SIRCDIV_DIVL_EN_MASK)
#define SCG_SIRCDIV_DIVM_EN_MASK       0x2000000u
#define SCG_SIRCDIV_DIVM_EN_SHIFT      25u
#define SCG_SIRCDIV_DIVM_EN_WIDTH      1u
#define SCG_SIRCDIV_DIVM_EN(x)         (((uint32_t)(((uint32_t)(x))<<SCG_SIRCDIV_DIVM_EN_SHIFT))&SCG_SIRCDIV_DIVM_EN_MASK)
#define SCG_SIRCDIV_DIVH_EN_MASK       0x1000000u
#define SCG_SIRCDIV_DIVH_EN_SHIFT      24u
#define SCG_SIRCDIV_DIVH_EN_WIDTH      1u
#define SCG_SIRCDIV_DIVH_EN(x)         (((uint32_t)(((uint32_t)(x))<<SCG_SIRCDIV_DIVH_EN_SHIFT))&SCG_SIRCDIV_DIVH_EN_MASK)
#define SCG_SIRCDIV_DIVL_MASK          0x70000u
#define SCG_SIRCDIV_DIVL_SHIFT         16u
#define SCG_SIRCDIV_DIVL_WIDTH         3u
#define SCG_SIRCDIV_DIVL(x)            (((uint32_t)(((uint32_t)(x))<<SCG_SIRCDIV_DIVL_SHIFT))&SCG_SIRCDIV_DIVL_MASK)
#define SCG_SIRCDIV_DIVM_MASK          0x700u
#define SCG_SIRCDIV_DIVM_SHIFT         8u
#define SCG_SIRCDIV_DIVM_WIDTH         3u
#define SCG_SIRCDIV_DIVM(x)            (((uint32_t)(((uint32_t)(x))<<SCG_SIRCDIV_DIVM_SHIFT))&SCG_SIRCDIV_DIVM_MASK)
#define SCG_SIRCDIV_DIVH_MASK          0x7u
#define SCG_SIRCDIV_DIVH_SHIFT         0u
#define SCG_SIRCDIV_DIVH_WIDTH         3u
#define SCG_SIRCDIV_DIVH(x)            (((uint32_t)(((uint32_t)(x))<<SCG_SIRCDIV_DIVH_SHIFT))&SCG_SIRCDIV_DIVH_MASK)

/* SIRCDIV Reg Mask */

#define SCG_SIRCDIV_MASK               0x77070707u

/* SIRCTCFG Bit Fields */

#define SCG_SIRCTCFG_TRIMDIV_MASK      0x7FF0000u
#define SCG_SIRCTCFG_TRIMDIV_SHIFT     16u
#define SCG_SIRCTCFG_TRIMDIV_WIDTH     11u
#define SCG_SIRCTCFG_TRIMDIV(x)        (((uint32_t)(((uint32_t)(x))<<SCG_SIRCTCFG_TRIMDIV_SHIFT))&SCG_SIRCTCFG_TRIMDIV_MASK)
#define SCG_SIRCTCFG_TRIMSRC_MASK      0x3u
#define SCG_SIRCTCFG_TRIMSRC_SHIFT     0u
#define SCG_SIRCTCFG_TRIMSRC_WIDTH     2u
#define SCG_SIRCTCFG_TRIMSRC(x)        (((uint32_t)(((uint32_t)(x))<<SCG_SIRCTCFG_TRIMSRC_SHIFT))&SCG_SIRCTCFG_TRIMSRC_MASK)

/* SIRCTCFG Reg Mask */

#define SCG_SIRCTCFG_MASK              0x07FF0003u

/* SIRCSTAT Bit Fields */

#define SCG_SIRCSTAT_TRIMFINE_MASK     0x3F00u
#define SCG_SIRCSTAT_TRIMFINE_SHIFT    8u
#define SCG_SIRCSTAT_TRIMFINE_WIDTH    6u
#define SCG_SIRCSTAT_TRIMFINE(x)       (((uint32_t)(((uint32_t)(x))<<SCG_SIRCSTAT_TRIMFINE_SHIFT))&SCG_SIRCSTAT_TRIMFINE_MASK)
#define SCG_SIRCSTAT_TRIMCOAR_MASK     0x3Fu
#define SCG_SIRCSTAT_TRIMCOAR_SHIFT    0u
#define SCG_SIRCSTAT_TRIMCOAR_WIDTH    6u
#define SCG_SIRCSTAT_TRIMCOAR(x)       (((uint32_t)(((uint32_t)(x))<<SCG_SIRCSTAT_TRIMCOAR_SHIFT))&SCG_SIRCSTAT_TRIMCOAR_MASK)

/* SIRCSTAT Reg Mask */

#define SCG_SIRCSTAT_MASK              0x00003F3Fu

/* FIRCCSR Bit Fields */

#define SCG_FIRCCSR_ERR_MASK           0x4000000u
#define SCG_FIRCCSR_ERR_SHIFT          26u
#define SCG_FIRCCSR_ERR_WIDTH          1u
#define SCG_FIRCCSR_ERR(x)             (((uint32_t)(((uint32_t)(x))<<SCG_FIRCCSR_ERR_SHIFT))&SCG_FIRCCSR_ERR_MASK)
#define SCG_FIRCCSR_SEL_MASK           0x2000000u
#define SCG_FIRCCSR_SEL_SHIFT          25u
#define SCG_FIRCCSR_SEL_WIDTH          1u
#define SCG_FIRCCSR_SEL(x)             (((uint32_t)(((uint32_t)(x))<<SCG_FIRCCSR_SEL_SHIFT))&SCG_FIRCCSR_SEL_MASK)
#define SCG_FIRCCSR_VLD_MASK           0x1000000u
#define SCG_FIRCCSR_VLD_SHIFT          24u
#define SCG_FIRCCSR_VLD_WIDTH          1u
#define SCG_FIRCCSR_VLD(x)             (((uint32_t)(((uint32_t)(x))<<SCG_FIRCCSR_VLD_SHIFT))&SCG_FIRCCSR_VLD_MASK)
#define SCG_FIRCCSR_LK_MASK            0x800000u
#define SCG_FIRCCSR_LK_SHIFT           23u
#define SCG_FIRCCSR_LK_WIDTH           1u
#define SCG_FIRCCSR_LK(x)              (((uint32_t)(((uint32_t)(x))<<SCG_FIRCCSR_LK_SHIFT))&SCG_FIRCCSR_LK_MASK)
#define SCG_FIRCCSR_CM_MASK            0x10000u
#define SCG_FIRCCSR_CM_SHIFT           16u
#define SCG_FIRCCSR_CM_WIDTH           1u
#define SCG_FIRCCSR_CM(x)              (((uint32_t)(((uint32_t)(x))<<SCG_FIRCCSR_CM_SHIFT))&SCG_FIRCCSR_CM_MASK)
#define SCG_FIRCCSR_TRLK_MASK          0x400u
#define SCG_FIRCCSR_TRLK_SHIFT         10u
#define SCG_FIRCCSR_TRLK_WIDTH         1u
#define SCG_FIRCCSR_TRLK(x)            (((uint32_t)(((uint32_t)(x))<<SCG_FIRCCSR_TRLK_SHIFT))&SCG_FIRCCSR_TRLK_MASK)
#define SCG_FIRCCSR_TRUP_MASK          0x200u
#define SCG_FIRCCSR_TRUP_SHIFT         9u
#define SCG_FIRCCSR_TRUP_WIDTH         1u
#define SCG_FIRCCSR_TRUP(x)            (((uint32_t)(((uint32_t)(x))<<SCG_FIRCCSR_TRUP_SHIFT))&SCG_FIRCCSR_TRUP_MASK)
#define SCG_FIRCCSR_TREN_MASK          0x100u
#define SCG_FIRCCSR_TREN_SHIFT         8u
#define SCG_FIRCCSR_TREN_WIDTH         1u
#define SCG_FIRCCSR_TREN(x)            (((uint32_t)(((uint32_t)(x))<<SCG_FIRCCSR_TREN_SHIFT))&SCG_FIRCCSR_TREN_MASK)
#define SCG_FIRCCSR_STEN_MASK          0x2u
#define SCG_FIRCCSR_STEN_SHIFT         1u
#define SCG_FIRCCSR_STEN_WIDTH         1u
#define SCG_FIRCCSR_STEN(x)            (((uint32_t)(((uint32_t)(x))<<SCG_FIRCCSR_STEN_SHIFT))&SCG_FIRCCSR_STEN_MASK)
#define SCG_FIRCCSR_EN_MASK            0x1u
#define SCG_FIRCCSR_EN_SHIFT           0u
#define SCG_FIRCCSR_EN_WIDTH           1u
#define SCG_FIRCCSR_EN(x)              (((uint32_t)(((uint32_t)(x))<<SCG_FIRCCSR_EN_SHIFT))&SCG_FIRCCSR_EN_MASK)

/* FIRCCSR Reg Mask */

#define SCG_FIRCCSR_MASK               0x07810703u

/* FIRCDIV Bit Fields */

#define SCG_FIRCDIV_DIVL_ACK_MASK      0x40000000u
#define SCG_FIRCDIV_DIVL_ACK_SHIFT     30u
#define SCG_FIRCDIV_DIVL_ACK_WIDTH     1u
#define SCG_FIRCDIV_DIVL_ACK(x)        (((uint32_t)(((uint32_t)(x))<<SCG_FIRCDIV_DIVL_ACK_SHIFT))&SCG_FIRCDIV_DIVL_ACK_MASK)
#define SCG_FIRCDIV_DIVM_ACK_MASK      0x20000000u
#define SCG_FIRCDIV_DIVM_ACK_SHIFT     29u
#define SCG_FIRCDIV_DIVM_ACK_WIDTH     1u
#define SCG_FIRCDIV_DIVM_ACK(x)        (((uint32_t)(((uint32_t)(x))<<SCG_FIRCDIV_DIVM_ACK_SHIFT))&SCG_FIRCDIV_DIVM_ACK_MASK)
#define SCG_FIRCDIV_DIVH_ACK_MASK      0x10000000u
#define SCG_FIRCDIV_DIVH_ACK_SHIFT     28u
#define SCG_FIRCDIV_DIVH_ACK_WIDTH     1u
#define SCG_FIRCDIV_DIVH_ACK(x)        (((uint32_t)(((uint32_t)(x))<<SCG_FIRCDIV_DIVH_ACK_SHIFT))&SCG_FIRCDIV_DIVH_ACK_MASK)
#define SCG_FIRCDIV_DIVL_EN_MASK       0x4000000u
#define SCG_FIRCDIV_DIVL_EN_SHIFT      26u
#define SCG_FIRCDIV_DIVL_EN_WIDTH      1u
#define SCG_FIRCDIV_DIVL_EN(x)         (((uint32_t)(((uint32_t)(x))<<SCG_FIRCDIV_DIVL_EN_SHIFT))&SCG_FIRCDIV_DIVL_EN_MASK)
#define SCG_FIRCDIV_DIVM_EN_MASK       0x2000000u
#define SCG_FIRCDIV_DIVM_EN_SHIFT      25u
#define SCG_FIRCDIV_DIVM_EN_WIDTH      1u
#define SCG_FIRCDIV_DIVM_EN(x)         (((uint32_t)(((uint32_t)(x))<<SCG_FIRCDIV_DIVM_EN_SHIFT))&SCG_FIRCDIV_DIVM_EN_MASK)
#define SCG_FIRCDIV_DIVH_EN_MASK       0x1000000u
#define SCG_FIRCDIV_DIVH_EN_SHIFT      24u
#define SCG_FIRCDIV_DIVH_EN_WIDTH      1u
#define SCG_FIRCDIV_DIVH_EN(x)         (((uint32_t)(((uint32_t)(x))<<SCG_FIRCDIV_DIVH_EN_SHIFT))&SCG_FIRCDIV_DIVH_EN_MASK)
#define SCG_FIRCDIV_DIVL_MASK          0x70000u
#define SCG_FIRCDIV_DIVL_SHIFT         16u
#define SCG_FIRCDIV_DIVL_WIDTH         3u
#define SCG_FIRCDIV_DIVL(x)            (((uint32_t)(((uint32_t)(x))<<SCG_FIRCDIV_DIVL_SHIFT))&SCG_FIRCDIV_DIVL_MASK)
#define SCG_FIRCDIV_DIVM_MASK          0x700u
#define SCG_FIRCDIV_DIVM_SHIFT         8u
#define SCG_FIRCDIV_DIVM_WIDTH         3u
#define SCG_FIRCDIV_DIVM(x)            (((uint32_t)(((uint32_t)(x))<<SCG_FIRCDIV_DIVM_SHIFT))&SCG_FIRCDIV_DIVM_MASK)
#define SCG_FIRCDIV_DIVH_MASK          0x7u
#define SCG_FIRCDIV_DIVH_SHIFT         0u
#define SCG_FIRCDIV_DIVH_WIDTH         3u
#define SCG_FIRCDIV_DIVH(x)            (((uint32_t)(((uint32_t)(x))<<SCG_FIRCDIV_DIVH_SHIFT))&SCG_FIRCDIV_DIVH_MASK)

/* FIRCDIV Reg Mask */

#define SCG_FIRCDIV_MASK               0x77070707u

/* FIRCCFG Bit Fields */

#define SCG_FIRCCFG_CLKEN_MASK         0x3u
#define SCG_FIRCCFG_CLKEN_SHIFT        0u
#define SCG_FIRCCFG_CLKEN_WIDTH        2u
#define SCG_FIRCCFG_CLKEN(x)           (((uint32_t)(((uint32_t)(x))<<SCG_FIRCCFG_CLKEN_SHIFT))&SCG_FIRCCFG_CLKEN_MASK)

/* FIRCCFG Reg Mask */

#define SCG_FIRCCFG_MASK               0x00000003u

/* FIRCTCFG Bit Fields */

#define SCG_FIRCTCFG_TRIMDIV_MASK      0x7FF0000u
#define SCG_FIRCTCFG_TRIMDIV_SHIFT     16u
#define SCG_FIRCTCFG_TRIMDIV_WIDTH     11u
#define SCG_FIRCTCFG_TRIMDIV(x)        (((uint32_t)(((uint32_t)(x))<<SCG_FIRCTCFG_TRIMDIV_SHIFT))&SCG_FIRCTCFG_TRIMDIV_MASK)
#define SCG_FIRCTCFG_TRIMSRC_MASK      0x3u
#define SCG_FIRCTCFG_TRIMSRC_SHIFT     0u
#define SCG_FIRCTCFG_TRIMSRC_WIDTH     2u
#define SCG_FIRCTCFG_TRIMSRC(x)        (((uint32_t)(((uint32_t)(x))<<SCG_FIRCTCFG_TRIMSRC_SHIFT))&SCG_FIRCTCFG_TRIMSRC_MASK)

/* FIRCTCFG Reg Mask */

#define SCG_FIRCTCFG_MASK              0x07FF0003u

/* FIRCSTAT Bit Fields */

#define SCG_FIRCSTAT_TRIMCOAR_MASK     0x3F00u
#define SCG_FIRCSTAT_TRIMCOAR_SHIFT    8u
#define SCG_FIRCSTAT_TRIMCOAR_WIDTH    6u
#define SCG_FIRCSTAT_TRIMCOAR(x)       (((uint32_t)(((uint32_t)(x))<<SCG_FIRCSTAT_TRIMCOAR_SHIFT))&SCG_FIRCSTAT_TRIMCOAR_MASK)
#define SCG_FIRCSTAT_TRIMFINE_MASK     0xFFu
#define SCG_FIRCSTAT_TRIMFINE_SHIFT    0u
#define SCG_FIRCSTAT_TRIMFINE_WIDTH    8u
#define SCG_FIRCSTAT_TRIMFINE(x)       (((uint32_t)(((uint32_t)(x))<<SCG_FIRCSTAT_TRIMFINE_SHIFT))&SCG_FIRCSTAT_TRIMFINE_MASK)

/* FIRCSTAT Reg Mask */

#define SCG_FIRCSTAT_MASK              0x00003FFFu

/* SOSCCSR Bit Fields */

#define SCG_SOSCCSR_ERR_MASK           0x4000000u
#define SCG_SOSCCSR_ERR_SHIFT          26u
#define SCG_SOSCCSR_ERR_WIDTH          1u
#define SCG_SOSCCSR_ERR(x)             (((uint32_t)(((uint32_t)(x))<<SCG_SOSCCSR_ERR_SHIFT))&SCG_SOSCCSR_ERR_MASK)
#define SCG_SOSCCSR_VLD_MASK           0x1000000u
#define SCG_SOSCCSR_VLD_SHIFT          24u
#define SCG_SOSCCSR_VLD_WIDTH          1u
#define SCG_SOSCCSR_VLD(x)             (((uint32_t)(((uint32_t)(x))<<SCG_SOSCCSR_VLD_SHIFT))&SCG_SOSCCSR_VLD_MASK)
#define SCG_SOSCCSR_LK_MASK            0x800000u
#define SCG_SOSCCSR_LK_SHIFT           23u
#define SCG_SOSCCSR_LK_WIDTH           1u
#define SCG_SOSCCSR_LK(x)              (((uint32_t)(((uint32_t)(x))<<SCG_SOSCCSR_LK_SHIFT))&SCG_SOSCCSR_LK_MASK)
#define SCG_SOSCCSR_CMRE_MASK          0x20000u
#define SCG_SOSCCSR_CMRE_SHIFT         17u
#define SCG_SOSCCSR_CMRE_WIDTH         1u
#define SCG_SOSCCSR_CMRE(x)            (((uint32_t)(((uint32_t)(x))<<SCG_SOSCCSR_CMRE_SHIFT))&SCG_SOSCCSR_CMRE_MASK)
#define SCG_SOSCCSR_CM_MASK            0x10000u
#define SCG_SOSCCSR_CM_SHIFT           16u
#define SCG_SOSCCSR_CM_WIDTH           1u
#define SCG_SOSCCSR_CM(x)              (((uint32_t)(((uint32_t)(x))<<SCG_SOSCCSR_CM_SHIFT))&SCG_SOSCCSR_CM_MASK)
#define SCG_SOSCCSR_BYPASS_MASK        0x2u
#define SCG_SOSCCSR_BYPASS_SHIFT       1u
#define SCG_SOSCCSR_BYPASS_WIDTH       1u
#define SCG_SOSCCSR_BYPASS(x)          (((uint32_t)(((uint32_t)(x))<<SCG_SOSCCSR_BYPASS_SHIFT))&SCG_SOSCCSR_BYPASS_MASK)
#define SCG_SOSCCSR_EN_MASK            0x1u
#define SCG_SOSCCSR_EN_SHIFT           0u
#define SCG_SOSCCSR_EN_WIDTH           1u
#define SCG_SOSCCSR_EN(x)              (((uint32_t)(((uint32_t)(x))<<SCG_SOSCCSR_EN_SHIFT))&SCG_SOSCCSR_EN_MASK)

/* SOSCCSR Reg Mask */

#define SCG_SOSCCSR_MASK               0x05830003u

/* SOSCCFG Bit Fields */

#define SCG_SOSCCFG_CURPRG_SF_MASK     0xC000000u
#define SCG_SOSCCFG_CURPRG_SF_SHIFT    26u
#define SCG_SOSCCFG_CURPRG_SF_WIDTH    2u
#define SCG_SOSCCFG_CURPRG_SF(x)       (((uint32_t)(((uint32_t)(x))<<SCG_SOSCCFG_CURPRG_SF_SHIFT))&SCG_SOSCCFG_CURPRG_SF_MASK)
#define SCG_SOSCCFG_CURPRG_COMP_MASK   0x3000000u
#define SCG_SOSCCFG_CURPRG_COMP_SHIFT  24u
#define SCG_SOSCCFG_CURPRG_COMP_WIDTH  2u
#define SCG_SOSCCFG_CURPRG_COMP(x)     (((uint32_t)(((uint32_t)(x))<<SCG_SOSCCFG_CURPRG_COMP_SHIFT))&SCG_SOSCCFG_CURPRG_COMP_MASK)
#define SCG_SOSCCFG_EOCV_MASK          0xFF0000u
#define SCG_SOSCCFG_EOCV_SHIFT         16u
#define SCG_SOSCCFG_EOCV_WIDTH         8u
#define SCG_SOSCCFG_EOCV(x)            (((uint32_t)(((uint32_t)(x))<<SCG_SOSCCFG_EOCV_SHIFT))&SCG_SOSCCFG_EOCV_MASK)
#define SCG_SOSCCFG_GM_SEL_MASK        0xC0u
#define SCG_SOSCCFG_GM_SEL_SHIFT       6u
#define SCG_SOSCCFG_GM_SEL_WIDTH       2u
#define SCG_SOSCCFG_GM_SEL(x)          (((uint32_t)(((uint32_t)(x))<<SCG_SOSCCFG_GM_SEL_SHIFT))&SCG_SOSCCFG_GM_SEL_MASK)

/* SOSCCFG Reg Mask */

#define SCG_SOSCCFG_MASK               0x0FFF00C0u

/* PLL1CSR Bit Fields */

#define SCG_PLL1CSR_PLLLK_MASK         0x8000000u
#define SCG_PLL1CSR_PLLLK_SHIFT        27u
#define SCG_PLL1CSR_PLLLK_WIDTH        1u
#define SCG_PLL1CSR_PLLLK(x)           (((uint32_t)(((uint32_t)(x))<<SCG_PLL1CSR_PLLLK_SHIFT))&SCG_PLL1CSR_PLLLK_MASK)
#define SCG_PLL1CSR_ERR_MASK           0x4000000u
#define SCG_PLL1CSR_ERR_SHIFT          26u
#define SCG_PLL1CSR_ERR_WIDTH          1u
#define SCG_PLL1CSR_ERR(x)             (((uint32_t)(((uint32_t)(x))<<SCG_PLL1CSR_ERR_SHIFT))&SCG_PLL1CSR_ERR_MASK)
#define SCG_PLL1CSR_VLD_MASK           0x1000000u
#define SCG_PLL1CSR_VLD_SHIFT          24u
#define SCG_PLL1CSR_VLD_WIDTH          1u
#define SCG_PLL1CSR_VLD(x)             (((uint32_t)(((uint32_t)(x))<<SCG_PLL1CSR_VLD_SHIFT))&SCG_PLL1CSR_VLD_MASK)
#define SCG_PLL1CSR_LK_MASK            0x800000u
#define SCG_PLL1CSR_LK_SHIFT           23u
#define SCG_PLL1CSR_LK_WIDTH           1u
#define SCG_PLL1CSR_LK(x)              (((uint32_t)(((uint32_t)(x))<<SCG_PLL1CSR_LK_SHIFT))&SCG_PLL1CSR_LK_MASK)
#define SCG_PLL1CSR_CMRE_MASK          0x20000u
#define SCG_PLL1CSR_CMRE_SHIFT         17u
#define SCG_PLL1CSR_CMRE_WIDTH         1u
#define SCG_PLL1CSR_CMRE(x)            (((uint32_t)(((uint32_t)(x))<<SCG_PLL1CSR_CMRE_SHIFT))&SCG_PLL1CSR_CMRE_MASK)
#define SCG_PLL1CSR_CM_MASK            0x10000u
#define SCG_PLL1CSR_CM_SHIFT           16u
#define SCG_PLL1CSR_CM_WIDTH           1u
#define SCG_PLL1CSR_CM(x)              (((uint32_t)(((uint32_t)(x))<<SCG_PLL1CSR_CM_SHIFT))&SCG_PLL1CSR_CM_MASK)
#define SCG_PLL1CSR_STEN_MASK          0x2u
#define SCG_PLL1CSR_STEN_SHIFT         1u
#define SCG_PLL1CSR_STEN_WIDTH         1u
#define SCG_PLL1CSR_STEN(x)            (((uint32_t)(((uint32_t)(x))<<SCG_PLL1CSR_STEN_SHIFT))&SCG_PLL1CSR_STEN_MASK)
#define SCG_PLL1CSR_EN_MASK            0x1u
#define SCG_PLL1CSR_EN_SHIFT           0u
#define SCG_PLL1CSR_EN_WIDTH           1u
#define SCG_PLL1CSR_EN(x)              (((uint32_t)(((uint32_t)(x))<<SCG_PLL1CSR_EN_SHIFT))&SCG_PLL1CSR_EN_MASK)

/* PLL1CSR Reg Mask */

#define SCG_PLL1CSR_MASK               0x0D830003u

/* PLL1DIV Bit Fields */

#define SCG_PLL1DIV_DIVL_ACK_MASK      0x40000000u
#define SCG_PLL1DIV_DIVL_ACK_SHIFT     30u
#define SCG_PLL1DIV_DIVL_ACK_WIDTH     1u
#define SCG_PLL1DIV_DIVL_ACK(x)        (((uint32_t)(((uint32_t)(x))<<SCG_PLL1DIV_DIVL_ACK_SHIFT))&SCG_PLL1DIV_DIVL_ACK_MASK)
#define SCG_PLL1DIV_DIVM_ACK_MASK      0x20000000u
#define SCG_PLL1DIV_DIVM_ACK_SHIFT     29u
#define SCG_PLL1DIV_DIVM_ACK_WIDTH     1u
#define SCG_PLL1DIV_DIVM_ACK(x)        (((uint32_t)(((uint32_t)(x))<<SCG_PLL1DIV_DIVM_ACK_SHIFT))&SCG_PLL1DIV_DIVM_ACK_MASK)
#define SCG_PLL1DIV_DIVH_ACK_MASK      0x10000000u
#define SCG_PLL1DIV_DIVH_ACK_SHIFT     28u
#define SCG_PLL1DIV_DIVH_ACK_WIDTH     1u
#define SCG_PLL1DIV_DIVH_ACK(x)        (((uint32_t)(((uint32_t)(x))<<SCG_PLL1DIV_DIVH_ACK_SHIFT))&SCG_PLL1DIV_DIVH_ACK_MASK)
#define SCG_PLL1DIV_DIVL_EN_MASK       0x4000000u
#define SCG_PLL1DIV_DIVL_EN_SHIFT      26u
#define SCG_PLL1DIV_DIVL_EN_WIDTH      1u
#define SCG_PLL1DIV_DIVL_EN(x)         (((uint32_t)(((uint32_t)(x))<<SCG_PLL1DIV_DIVL_EN_SHIFT))&SCG_PLL1DIV_DIVL_EN_MASK)
#define SCG_PLL1DIV_DIVM_EN_MASK       0x2000000u
#define SCG_PLL1DIV_DIVM_EN_SHIFT      25u
#define SCG_PLL1DIV_DIVM_EN_WIDTH      1u
#define SCG_PLL1DIV_DIVM_EN(x)         (((uint32_t)(((uint32_t)(x))<<SCG_PLL1DIV_DIVM_EN_SHIFT))&SCG_PLL1DIV_DIVM_EN_MASK)
#define SCG_PLL1DIV_DIVH_EN_MASK       0x1000000u
#define SCG_PLL1DIV_DIVH_EN_SHIFT      24u
#define SCG_PLL1DIV_DIVH_EN_WIDTH      1u
#define SCG_PLL1DIV_DIVH_EN(x)         (((uint32_t)(((uint32_t)(x))<<SCG_PLL1DIV_DIVH_EN_SHIFT))&SCG_PLL1DIV_DIVH_EN_MASK)
#define SCG_PLL1DIV_DIVL_MASK          0x70000u
#define SCG_PLL1DIV_DIVL_SHIFT         16u
#define SCG_PLL1DIV_DIVL_WIDTH         3u
#define SCG_PLL1DIV_DIVL(x)            (((uint32_t)(((uint32_t)(x))<<SCG_PLL1DIV_DIVL_SHIFT))&SCG_PLL1DIV_DIVL_MASK)
#define SCG_PLL1DIV_DIVM_MASK          0x700u
#define SCG_PLL1DIV_DIVM_SHIFT         8u
#define SCG_PLL1DIV_DIVM_WIDTH         3u
#define SCG_PLL1DIV_DIVM(x)            (((uint32_t)(((uint32_t)(x))<<SCG_PLL1DIV_DIVM_SHIFT))&SCG_PLL1DIV_DIVM_MASK)
#define SCG_PLL1DIV_DIVH_MASK          0x7u
#define SCG_PLL1DIV_DIVH_SHIFT         0u
#define SCG_PLL1DIV_DIVH_WIDTH         3u
#define SCG_PLL1DIV_DIVH(x)            (((uint32_t)(((uint32_t)(x))<<SCG_PLL1DIV_DIVH_SHIFT))&SCG_PLL1DIV_DIVH_MASK)

/* PLL1DIV Reg Mask */

#define SCG_PLL1DIV_MASK               0x77070707u

/* PLL1CFG Bit Fields */

#define SCG_PLL1CFG_MULT_MASK          0x1FF0000u
#define SCG_PLL1CFG_MULT_SHIFT         16u
#define SCG_PLL1CFG_MULT_WIDTH         9u
#define SCG_PLL1CFG_MULT(x)            (((uint32_t)(((uint32_t)(x))<<SCG_PLL1CFG_MULT_SHIFT))&SCG_PLL1CFG_MULT_MASK)
#define SCG_PLL1CFG_PREDIV_MASK        0x1F00u
#define SCG_PLL1CFG_PREDIV_SHIFT       8u
#define SCG_PLL1CFG_PREDIV_WIDTH       5u
#define SCG_PLL1CFG_PREDIV(x)          (((uint32_t)(((uint32_t)(x))<<SCG_PLL1CFG_PREDIV_SHIFT))&SCG_PLL1CFG_PREDIV_MASK)
#define SCG_PLL1CFG_PSTDIV_MASK        0x30u
#define SCG_PLL1CFG_PSTDIV_SHIFT       4u
#define SCG_PLL1CFG_PSTDIV_WIDTH       2u
#define SCG_PLL1CFG_PSTDIV(x)          (((uint32_t)(((uint32_t)(x))<<SCG_PLL1CFG_PSTDIV_SHIFT))&SCG_PLL1CFG_PSTDIV_MASK)
#define SCG_PLL1CFG_SOURCE_MASK        0x1u
#define SCG_PLL1CFG_SOURCE_SHIFT       0u
#define SCG_PLL1CFG_SOURCE_WIDTH       1u
#define SCG_PLL1CFG_SOURCE(x)          (((uint32_t)(((uint32_t)(x))<<SCG_PLL1CFG_SOURCE_SHIFT))&SCG_PLL1CFG_SOURCE_MASK)

/* PLL1CFG Reg Mask */

#define SCG_PLL1CFG_MASK               0x01FF1F31u

/* PLL0CSR Bit Fields */

#define SCG_PLL0CSR_PLLLK_MASK         0x8000000u
#define SCG_PLL0CSR_PLLLK_SHIFT        27u
#define SCG_PLL0CSR_PLLLK_WIDTH        1u
#define SCG_PLL0CSR_PLLLK(x)           (((uint32_t)(((uint32_t)(x))<<SCG_PLL0CSR_PLLLK_SHIFT))&SCG_PLL0CSR_PLLLK_MASK)
#define SCG_PLL0CSR_ERR_MASK           0x4000000u
#define SCG_PLL0CSR_ERR_SHIFT          26u
#define SCG_PLL0CSR_ERR_WIDTH          1u
#define SCG_PLL0CSR_ERR(x)             (((uint32_t)(((uint32_t)(x))<<SCG_PLL0CSR_ERR_SHIFT))&SCG_PLL0CSR_ERR_MASK)
#define SCG_PLL0CSR_SEL_MASK           0x2000000u
#define SCG_PLL0CSR_SEL_SHIFT          25u
#define SCG_PLL0CSR_SEL_WIDTH          1u
#define SCG_PLL0CSR_SEL(x)             (((uint32_t)(((uint32_t)(x))<<SCG_PLL0CSR_SEL_SHIFT))&SCG_PLL0CSR_SEL_MASK)
#define SCG_PLL0CSR_VLD_MASK           0x1000000u
#define SCG_PLL0CSR_VLD_SHIFT          24u
#define SCG_PLL0CSR_VLD_WIDTH          1u
#define SCG_PLL0CSR_VLD(x)             (((uint32_t)(((uint32_t)(x))<<SCG_PLL0CSR_VLD_SHIFT))&SCG_PLL0CSR_VLD_MASK)
#define SCG_PLL0CSR_LK_MASK            0x800000u
#define SCG_PLL0CSR_LK_SHIFT           23u
#define SCG_PLL0CSR_LK_WIDTH           1u
#define SCG_PLL0CSR_LK(x)              (((uint32_t)(((uint32_t)(x))<<SCG_PLL0CSR_LK_SHIFT))&SCG_PLL0CSR_LK_MASK)
#define SCG_PLL0CSR_CMRE_MASK          0x20000u
#define SCG_PLL0CSR_CMRE_SHIFT         17u
#define SCG_PLL0CSR_CMRE_WIDTH         1u
#define SCG_PLL0CSR_CMRE(x)            (((uint32_t)(((uint32_t)(x))<<SCG_PLL0CSR_CMRE_SHIFT))&SCG_PLL0CSR_CMRE_MASK)
#define SCG_PLL0CSR_CM_MASK            0x10000u
#define SCG_PLL0CSR_CM_SHIFT           16u
#define SCG_PLL0CSR_CM_WIDTH           1u
#define SCG_PLL0CSR_CM(x)              (((uint32_t)(((uint32_t)(x))<<SCG_PLL0CSR_CM_SHIFT))&SCG_PLL0CSR_CM_MASK)
#define SCG_PLL0CSR_STEN_MASK          0x2u
#define SCG_PLL0CSR_STEN_SHIFT         1u
#define SCG_PLL0CSR_STEN_WIDTH         1u
#define SCG_PLL0CSR_STEN(x)            (((uint32_t)(((uint32_t)(x))<<SCG_PLL0CSR_STEN_SHIFT))&SCG_PLL0CSR_STEN_MASK)
#define SCG_PLL0CSR_EN_MASK            0x1u
#define SCG_PLL0CSR_EN_SHIFT           0u
#define SCG_PLL0CSR_EN_WIDTH           1u
#define SCG_PLL0CSR_EN(x)              (((uint32_t)(((uint32_t)(x))<<SCG_PLL0CSR_EN_SHIFT))&SCG_PLL0CSR_EN_MASK)

/* PLL0CSR Reg Mask */

#define SCG_PLL0CSR_MASK               0x0F830003u

/* PLL0DIV Bit Fields */

#define SCG_PLL0DIV_DIVL_ACK_MASK      0x40000000u
#define SCG_PLL0DIV_DIVL_ACK_SHIFT     30u
#define SCG_PLL0DIV_DIVL_ACK_WIDTH     1u
#define SCG_PLL0DIV_DIVL_ACK(x)        (((uint32_t)(((uint32_t)(x))<<SCG_PLL0DIV_DIVL_ACK_SHIFT))&SCG_PLL0DIV_DIVL_ACK_MASK)
#define SCG_PLL0DIV_DIVM_ACK_MASK      0x20000000u
#define SCG_PLL0DIV_DIVM_ACK_SHIFT     29u
#define SCG_PLL0DIV_DIVM_ACK_WIDTH     1u
#define SCG_PLL0DIV_DIVM_ACK(x)        (((uint32_t)(((uint32_t)(x))<<SCG_PLL0DIV_DIVM_ACK_SHIFT))&SCG_PLL0DIV_DIVM_ACK_MASK)
#define SCG_PLL0DIV_DIVH_ACK_MASK      0x10000000u
#define SCG_PLL0DIV_DIVH_ACK_SHIFT     28u
#define SCG_PLL0DIV_DIVH_ACK_WIDTH     1u
#define SCG_PLL0DIV_DIVH_ACK(x)        (((uint32_t)(((uint32_t)(x))<<SCG_PLL0DIV_DIVH_ACK_SHIFT))&SCG_PLL0DIV_DIVH_ACK_MASK)
#define SCG_PLL0DIV_DIVL_EN_MASK       0x4000000u
#define SCG_PLL0DIV_DIVL_EN_SHIFT      26u
#define SCG_PLL0DIV_DIVL_EN_WIDTH      1u
#define SCG_PLL0DIV_DIVL_EN(x)         (((uint32_t)(((uint32_t)(x))<<SCG_PLL0DIV_DIVL_EN_SHIFT))&SCG_PLL0DIV_DIVL_EN_MASK)
#define SCG_PLL0DIV_DIVM_EN_MASK       0x2000000u
#define SCG_PLL0DIV_DIVM_EN_SHIFT      25u
#define SCG_PLL0DIV_DIVM_EN_WIDTH      1u
#define SCG_PLL0DIV_DIVM_EN(x)         (((uint32_t)(((uint32_t)(x))<<SCG_PLL0DIV_DIVM_EN_SHIFT))&SCG_PLL0DIV_DIVM_EN_MASK)
#define SCG_PLL0DIV_DIVH_EN_MASK       0x1000000u
#define SCG_PLL0DIV_DIVH_EN_SHIFT      24u
#define SCG_PLL0DIV_DIVH_EN_WIDTH      1u
#define SCG_PLL0DIV_DIVH_EN(x)         (((uint32_t)(((uint32_t)(x))<<SCG_PLL0DIV_DIVH_EN_SHIFT))&SCG_PLL0DIV_DIVH_EN_MASK)
#define SCG_PLL0DIV_DIVL_MASK          0x70000u
#define SCG_PLL0DIV_DIVL_SHIFT         16u
#define SCG_PLL0DIV_DIVL_WIDTH         3u
#define SCG_PLL0DIV_DIVL(x)            (((uint32_t)(((uint32_t)(x))<<SCG_PLL0DIV_DIVL_SHIFT))&SCG_PLL0DIV_DIVL_MASK)
#define SCG_PLL0DIV_DIVM_MASK          0x700u
#define SCG_PLL0DIV_DIVM_SHIFT         8u
#define SCG_PLL0DIV_DIVM_WIDTH         3u
#define SCG_PLL0DIV_DIVM(x)            (((uint32_t)(((uint32_t)(x))<<SCG_PLL0DIV_DIVM_SHIFT))&SCG_PLL0DIV_DIVM_MASK)
#define SCG_PLL0DIV_DIVH_MASK          0x7u
#define SCG_PLL0DIV_DIVH_SHIFT         0u
#define SCG_PLL0DIV_DIVH_WIDTH         3u
#define SCG_PLL0DIV_DIVH(x)            (((uint32_t)(((uint32_t)(x))<<SCG_PLL0DIV_DIVH_SHIFT))&SCG_PLL0DIV_DIVH_MASK)

/* PLL0DIV Reg Mask */

#define SCG_PLL0DIV_MASK               0x77070707u

/* PLL0CFG Bit Fields */

#define SCG_PLL0CFG_MULT_MASK          0x1FF0000u
#define SCG_PLL0CFG_MULT_SHIFT         16u
#define SCG_PLL0CFG_MULT_WIDTH         9u
#define SCG_PLL0CFG_MULT(x)            (((uint32_t)(((uint32_t)(x))<<SCG_PLL0CFG_MULT_SHIFT))&SCG_PLL0CFG_MULT_MASK)
#define SCG_PLL0CFG_PREDIV_MASK        0x1F00u
#define SCG_PLL0CFG_PREDIV_SHIFT       8u
#define SCG_PLL0CFG_PREDIV_WIDTH       5u
#define SCG_PLL0CFG_PREDIV(x)          (((uint32_t)(((uint32_t)(x))<<SCG_PLL0CFG_PREDIV_SHIFT))&SCG_PLL0CFG_PREDIV_MASK)
#define SCG_PLL0CFG_PSTDIV_MASK        0x30u
#define SCG_PLL0CFG_PSTDIV_SHIFT       4u
#define SCG_PLL0CFG_PSTDIV_WIDTH       2u
#define SCG_PLL0CFG_PSTDIV(x)          (((uint32_t)(((uint32_t)(x))<<SCG_PLL0CFG_PSTDIV_SHIFT))&SCG_PLL0CFG_PSTDIV_MASK)
#define SCG_PLL0CFG_SOURCE_MASK        0x1u
#define SCG_PLL0CFG_SOURCE_SHIFT       0u
#define SCG_PLL0CFG_SOURCE_WIDTH       1u
#define SCG_PLL0CFG_SOURCE(x)          (((uint32_t)(((uint32_t)(x))<<SCG_PLL0CFG_SOURCE_SHIFT))&SCG_PLL0CFG_SOURCE_MASK)

/* PLL0CFG Reg Mask */

#define SCG_PLL0CFG_MASK               0x01FF1F31u

/* SIRC32KCSR Bit Fields */

#define SCG_SIRC32KCSR_VLD_MASK        0x1000000u
#define SCG_SIRC32KCSR_VLD_SHIFT       24u
#define SCG_SIRC32KCSR_VLD_WIDTH       1u
#define SCG_SIRC32KCSR_VLD(x)          (((uint32_t)(((uint32_t)(x))<<SCG_SIRC32KCSR_VLD_SHIFT))&SCG_SIRC32KCSR_VLD_MASK)
#define SCG_SIRC32KCSR_LK_MASK         0x800000u
#define SCG_SIRC32KCSR_LK_SHIFT        23u
#define SCG_SIRC32KCSR_LK_WIDTH        1u
#define SCG_SIRC32KCSR_LK(x)           (((uint32_t)(((uint32_t)(x))<<SCG_SIRC32KCSR_LK_SHIFT))&SCG_SIRC32KCSR_LK_MASK)
#define SCG_SIRC32KCSR_EN_MASK         0x1u
#define SCG_SIRC32KCSR_EN_SHIFT        0u
#define SCG_SIRC32KCSR_EN_WIDTH        1u
#define SCG_SIRC32KCSR_EN(x)           (((uint32_t)(((uint32_t)(x))<<SCG_SIRC32KCSR_EN_SHIFT))&SCG_SIRC32KCSR_EN_MASK)

/* SIRC32KCSR Reg Mask */

#define SCG_SIRC32KCSR_MASK            0x01800001u

#define UNKNOWN_CLOCK    0xFFFFFFFFU

#define SCG_CHECK_DIVL_EN(reg_val)     ((reg_val & SCG_FOSCDIV_DIVL_EN_MASK) >> SCG_FOSCDIV_DIVL_EN_SHIFT)
#define SCG_CHECK_DIVM_EN(reg_val)     ((reg_val & SCG_FOSCDIV_DIVM_EN_MASK) >> SCG_FOSCDIV_DIVM_EN_SHIFT)
#define SCG_CHECK_DIVH_EN(reg_val)     ((reg_val & SCG_FOSCDIV_DIVH_EN_MASK) >> SCG_FOSCDIV_DIVH_EN_SHIFT)
#define SCG_GET_DIVL_VAL(reg_val)      ((reg_val & SCG_FOSCDIV_DIVL_MASK) >> SCG_FOSCDIV_DIVL_SHIFT)
#define SCG_GET_DIVM_VAL(reg_val)      ((reg_val & SCG_FOSCDIV_DIVM_MASK) >> SCG_FOSCDIV_DIVM_SHIFT)
#define SCG_GET_DIVH_VAL(reg_val)      ((reg_val & SCG_FOSCDIV_DIVH_MASK) >> SCG_FOSCDIV_DIVH_SHIFT)

#define SCG_CALCULATE_DIVL_FREQ(clk_freq, reg_val) \
        ((SCG_CHECK_DIVL_EN(reg_val) && (clk_freq != UNKNOWN_CLOCK)) ? \
        (uint32_t)clk_freq >> (SCG_GET_DIVL_VAL(reg_val) - 1) : (uint32_t)UNKNOWN_CLOCK)

#define SCG_CALCULATE_DIVM_FREQ(clk_freq, reg_val) \
        ((SCG_CHECK_DIVM_EN(reg_val) && (clk_freq != UNKNOWN_CLOCK)) ? \
        (uint32_t)clk_freq >> (SCG_GET_DIVM_VAL(reg_val) - 1) : (uint32_t)UNKNOWN_CLOCK)

#define SCG_CALCULATE_DIVH_FREQ(clk_freq, reg_val) \
        ((SCG_CHECK_DIVH_EN(reg_val) && (clk_freq != UNKNOWN_CLOCK)) ? \
        (uint32_t)clk_freq >> (SCG_GET_DIVH_VAL(reg_val) - 1) : (uint32_t)UNKNOWN_CLOCK)

#endif /* __VENDOR_FLAGCHIP_CHIPS_FC7300_HARDWARE_FC7300_SCG_H */

