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

#ifndef __VENDOR_FLAGCHIP_CHIPS_FC7300_HARDWARE_FC7300_STCU_H
#define __VENDOR_FLAGCHIP_CHIPS_FC7300_HARDWARE_FC7300_STCU_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>
#include <hardware/fc7300_memorymap.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Register Offsets *********************************************************/

#define FC7300_STCU_SELF_TEST_KEY_OFFSET                   0x0000  /* Self Test Safety Key Register */
#define FC7300_STCU_SELF_TEST_CTRL_OFFSET                  0x0004  /* Self Test Control Register */
#define FC7300_STCU_SELF_TEST_STATUS_OFFSET                0x0008  /* Self Test Status Register */
#define FC7300_STCU_SELF_TEST_TRIG_A_OFFSET                0x000C  /* Self Test Trigger A Register */
#define FC7300_STCU_SELF_TEST_TRIG_B_OFFSET                0x0010  /* Self Test Trigger B Register */
#define FC7300_STCU_SELF_TEST_RESET_OFFSET                 0x0014  /* Self Test Reset Register */
#define FC7300_STCU_LBIST_PAT_CTRL_OFFSET                  0x0018  /* LBIST Pattern Control Register */
#define FC7300_STCU_LBIST_EXP_MISR_OFFSET                  0x0020  /* LBIST Expected MISR Register */
#define FC7300_STCU_LBIST_ACT_MISR_OFFSET                  0x0024  /* LBIST Actual MISR Register */
#define FC7300_STCU_LBIST_STATUS_OFFSET                    0x0028  /* LBIST Status Register */
#define FC7300_STCU_MBIST_SEL_OFFSET                       0x0034  /* MBIST Select Register */
#define FC7300_STCU_MBIST_ALG_OFFSET                       0x0038  /* MBIST Algorithm Register */
#define FC7300_STCU_MBIST_DONE_STATUS_OFFSET               0x003C  /* MBIST Done Status Register */
#define FC7300_STCU_MBIST_FAIL_STATUS_OFFSET               0x0040  /* MBIST Fail Status Register */
#define FC7300_STCU_USER_GP_OFFSET                         0x0044  /* User General Purpose Register */
#define FC7300_STCU_SRAM_INI_CTRL_OFFSET                   0x0048  /* SRAM Initialization Control Register */
#define FC7300_STCU_SRAM_INI_STATUS_OFFSET                 0x004C  /* SRAM Initialization Status Register */
#define FC7300_STCU_SRAM_INI_SEL_OFFSET                    0x0050  /* SRAM Initialization Select Register */
#define FC7300_STCU_SRAM_INI_DONE_STATUS_OFFSET            0x0054  /* SRAM Initialization Done Status Register */
#define FC7300_STCU_IRQ_OFFSET                             0x0058  /* Interrupt Request Register */

/* Register Addresses *******************************************************/

#define FC7300_STCU_SELF_TEST_KEY                          (FC7300_STCU_BASE + FC7300_STCU_SELF_TEST_KEY_OFFSET)
#define FC7300_STCU_SELF_TEST_CTRL                         (FC7300_STCU_BASE + FC7300_STCU_SELF_TEST_CTRL_OFFSET)
#define FC7300_STCU_SELF_TEST_STATUS                       (FC7300_STCU_BASE + FC7300_STCU_SELF_TEST_STATUS_OFFSET)
#define FC7300_STCU_SELF_TEST_TRIG_A                       (FC7300_STCU_BASE + FC7300_STCU_SELF_TEST_TRIG_A_OFFSET)
#define FC7300_STCU_SELF_TEST_TRIG_B                       (FC7300_STCU_BASE + FC7300_STCU_SELF_TEST_TRIG_B_OFFSET)
#define FC7300_STCU_SELF_TEST_RESET                        (FC7300_STCU_BASE + FC7300_STCU_SELF_TEST_RESET_OFFSET)
#define FC7300_STCU_LBIST_PAT_CTRL                         (FC7300_STCU_BASE + FC7300_STCU_LBIST_PAT_CTRL_OFFSET)
#define FC7300_STCU_LBIST_EXP_MISR                         (FC7300_STCU_BASE + FC7300_STCU_LBIST_EXP_MISR_OFFSET)
#define FC7300_STCU_LBIST_ACT_MISR                         (FC7300_STCU_BASE + FC7300_STCU_LBIST_ACT_MISR_OFFSET)
#define FC7300_STCU_LBIST_STATUS                           (FC7300_STCU_BASE + FC7300_STCU_LBIST_STATUS_OFFSET)
#define FC7300_STCU_MBIST_SEL                              (FC7300_STCU_BASE + FC7300_STCU_MBIST_SEL_OFFSET)
#define FC7300_STCU_MBIST_ALG                              (FC7300_STCU_BASE + FC7300_STCU_MBIST_ALG_OFFSET)
#define FC7300_STCU_MBIST_DONE_STATUS                      (FC7300_STCU_BASE + FC7300_STCU_MBIST_DONE_STATUS_OFFSET)
#define FC7300_STCU_MBIST_FAIL_STATUS                      (FC7300_STCU_BASE + FC7300_STCU_MBIST_FAIL_STATUS_OFFSET)
#define FC7300_STCU_USER_GP                                (FC7300_STCU_BASE + FC7300_STCU_USER_GP_OFFSET)
#define FC7300_STCU_SRAM_INI_CTRL                          (FC7300_STCU_BASE + FC7300_STCU_SRAM_INI_CTRL_OFFSET)
#define FC7300_STCU_SRAM_INI_STATUS                        (FC7300_STCU_BASE + FC7300_STCU_SRAM_INI_STATUS_OFFSET)
#define FC7300_STCU_SRAM_INI_SEL                           (FC7300_STCU_BASE + FC7300_STCU_SRAM_INI_SEL_OFFSET)
#define FC7300_STCU_SRAM_INI_DONE_STATUS                   (FC7300_STCU_BASE + FC7300_STCU_SRAM_INI_DONE_STATUS_OFFSET)
#define FC7300_STCU_IRQ                                    (FC7300_STCU_BASE + FC7300_STCU_IRQ_OFFSET)

/* Register Bit Definitions *************************************************/

/* STCU_SELF_TEST_KEY Fields */

#define FC7300_STCU_SELF_TEST_KEY_KEY                      (0xBB40E64D)

/* STCU_SELF_TEST_CTRL Fields */

#define FC7300_STCU_SELF_TEST_CTRL_MT_MASK                 0xFFFu
#define FC7300_STCU_SELF_TEST_CTRL_MT_SHIFT                0u
#define FC7300_STCU_SELF_TEST_CTRL_MT_WIDTH                12u
#  define FC7300_STCU_SELF_TEST_CTRL_MT(x)                 (((uint32_t)(((uint32_t)(x))<<STCU_SELF_TEST_CTRL_MT_SHIFT))&STCU_SELF_TEST_CTRL_MT_MASK)
#define FC7300_STCU_SELF_TEST_CTRL_MT_FCSMU_PIN0_VAL       (1 << 12)
#define FC7300_STCU_SELF_TEST_CTRL_MT_FCSMU_PIN1_VAL       (1 << 13)
#define FC7300_STCU_SELF_TEST_CTRL_MT_FCSMU_PIN0_EN        (1 << 14)
#define FC7300_STCU_SELF_TEST_CTRL_MT_FCSMU_PIN1_EN        (1 << 15)
#define FC7300_STCU_SELF_TEST_CTRL_MT_STEST_ST             (1 << 16)
#define FC7300_STCU_SELF_TEST_CTRL_MT_PE                   (1 << 17)
#define FC7300_STCU_SELF_TEST_CTRL_MT_PS                   (1 << 18)
#define FC7300_STCU_SELF_TEST_CTRL_MT_STEST_BYPASS         (1 << 19)
#define FC7300_STCU_SELF_TEST_CTRL_MT_RST_TRIG             (1 << 20)
#define FC7300_STCU_SELF_TEST_CTRL_MT_CLK_SEL              (1 << 21)
#define FC7300_STCU_SELF_TEST_CTRL_MT_MBIST_EN             (1 << 24)
#define FC7300_STCU_SELF_TEST_CTRL_MBIST_EN_SHIFT          24u
#define FC7300_STCU_SELF_TEST_CTRL_MT_MBIST_LP             (1 << 25)
#define FC7300_STCU_SELF_TEST_CTRL_MT_SCHK_EN              (1 << 26)
#define FC7300_STCU_SELF_TEST_CTRL_MT_LBIST_TD             (1 << 27)
#define FC7300_STCU_SELF_TEST_CTRL_MT_LBIST_EN             (1 << 28)
#define FC7300_STCU_SELF_TEST_CTRL_LBIST_EN_SHIFT          28u
#define FC7300_STCU_SELF_TEST_CTRL_MT_LBIST_LP             (1 << 29)
#define FC7300_STCU_SELF_TEST_CTRL_MT_LBIST_CLKDIV_MASK    (3 << 30)
#define FC7300_STCU_SELF_TEST_CTRL_MT_LBIST_CLKDIV_SHIFT   30u
#define FC7300_STCU_SELF_TEST_CTRL_MT_LBIST_CLKDIV_WIDTH   2u
#  define FC7300_STCU_SELF_TEST_CTRL_MT_LBIST_CLKDIV(x)    (((uint32_t)(((uint32_t)(x))<<STCU_SELF_TEST_CTRL_MT_LBIST_CLKDIV_SHIFT))&STCU_SELF_TEST_CTRL_MT_LBIST_CLKDIV_MASK)

/* STCU_SELF_TEST_STATUS Fields */

#define FC7300_STCU_SELF_TEST_STATUS_STEST_DONE            (1 << 0)
#define FC7300_STCU_SELF_TEST_STATUS_STEST_ABORT           (1 << 1)
#define FC7300_STCU_SELF_TEST_STATUS_LBIST_ERR             (1 << 4)
#define FC7300_STCU_SELF_TEST_STATUS_MBIST_ERR             (1 << 5)
#define FC7300_STCU_SELF_TEST_STATUS_TIMEOUT_ERR           (1 << 6)
#define FC7300_STCU_SELF_TEST_STATUS_NVR_LD_ERR            (1 << 8)
#define FC7300_STCU_SELF_TEST_STATUS_SCHK_ERR              (1 << 9)

/* STCU_SELF_TEST_TRIG_A Fields */

#define FC7300_STCU_SELF_TEST_TRIG_A_TRIG                  (1 << 0)

/* STCU_SELF_TEST_TRIG_B Fields */

#define FC7300_STCU_SELF_TEST_TRIG_B_TRIG                  (1 << 0)

/* STCU_SELF_TEST_RESET Fields */

#define FC7300_STCU_SELF_TEST_RESET_RST                    (1 << 0)

/* STCU_LBIST_PAT_CTRL Fields */

#define FC7300_STCU_LBIST_PAT_CTRL_N_MASK                  0x3FFu
#define FC7300_STCU_LBIST_PAT_CTRL_N_SHIFT                 0u
#define FC7300_STCU_LBIST_PAT_CTRL_N_WIDTH                 10u
#  define FC7300_STCU_LBIST_PAT_CTRL_N(x)                  (((uint32_t)(((uint32_t)(x))<<FC7300_STCU_LBIST_PAT_CTRL_N_SHIFT))&FC7300_STCU_LBIST_PAT_CTRL_N_MASK)

/* STCU_LBIST_EXP_MISR Fields */

/* STCU_LBIST_ACT_MISR Fields */

/* STCU_LBIST_STATUS Fields */
#define FC7300_STCU_LBIST_STATUS_DONE_MASK                 0x01u
#define FC7300_STCU_LBIST_FAIL_DONE_MASK                   0x10000u
#define FC7300_STCU_LBIST_FAIL_DONE_SHIFT                  16u
#define FC7300_STCU_LBIST_STATUS_DONE_SHIFT                0u
#define FC7300_STCU_LBIST_STATUS_DONE                      (1 << 0)
#define FC7300_STCU_LBIST_FAIL_DONE                        (1 << 16)

/* STCU_MBIST_SEL Fields */

#define FC7300_STCU_MBIST_SEL_SRAM0                        (1 << 0)
#define FC7300_STCU_MBIST_SEL_SRAM1                        (1 << 1)
#define FC7300_STCU_MBIST_SEL_SRAM2_1                      (1 << 2)
#define FC7300_STCU_MBIST_SEL_SRAM2_2                      (1 << 3)
#define FC7300_STCU_MBIST_SEL_ITCM_CPU0                    (1 << 4)
#define FC7300_STCU_MBIST_SEL_DTCM0_CPU0                   (1 << 5)
#define FC7300_STCU_MBIST_SEL_DTCM1_CPU0                   (1 << 6)
#define FC7300_STCU_MBIST_SEL_ITCM_CPU1                    (1 << 7)
#define FC7300_STCU_MBIST_SEL_DTCM0_CPU1                   (1 << 8)
#define FC7300_STCU_MBIST_SEL_DTCM1_CPU1                   (1 << 9)
#define FC7300_STCU_MBIST_SEL_ITCM_CPU2                    (1 << 10)
#define FC7300_STCU_MBIST_SEL_DTCM0_CPU2                   (1 << 11)
#define FC7300_STCU_MBIST_SEL_DTCM1_CPU2                   (1 << 12)
#define FC7300_STCU_MBIST_SEL_CACHE_CPU0                   (1 << 13)
#define FC7300_STCU_MBIST_SEL_CACHE_CPU1                   (1 << 14)
#define FC7300_STCU_MBIST_SEL_CACHE_CPU2                   (1 << 15)
#define FC7300_STCU_MBIST_SEL_HSM                          (1 << 16)
#define FC7300_STCU_MBIST_SEL_DMA_FLEXCAN                  (1 << 17)
#define FC7300_STCU_MBIST_SEL_ENET                         (1 << 18)
#define FC7300_STCU_MBIST_SEL_ETB                          (1 << 19)
#define FC7300_STCU_MBIST_SEL_SYS_ROM                      (1 << 20)
#define FC7300_STCU_MBIST_SEL_HSM_ROM                      (1 << 21)

/* STCU_MBIST_ALG Fields */

#define FC7300_STCU_MBIST_ALG_POR_ALG_SEL_MASK             (0x1F)
#define FC7300_STCU_MBIST_ALG_POR_ALG_SEL_SHIFT            (0)
#define FC7300_STCU_MBIST_ALG_POR_ALG_SEL_WIDTH            (5)
#  define FC7300_STCU_MBIST_ALG_POR_ALG_SEL(x)             (((uint32_t)(((uint32_t)(x))<<FC7300_STCU_MBIST_ALG_POR_ALG_SEL_SHIFT))&FC7300_STCU_MBIST_ALG_POR_ALG_SEL_MASK)
#  define FC7300_STCU_MBIST_ALG_POR_ALG_SEL_BASIC          (0x19)
#  define FC7300_STCU_MBIST_ALG_POR_ALG_SEL_FULL           (0x1F)
#define FC7300_STCU_MBIST_ALG_POR_INI_EN                   (1 << 5)
#define FC7300_STCU_MBIST_ALG_TRIG_ALG_SEL_MASK            (0x1F00)
#define FC7300_STCU_MBIST_ALG_TRIG_ALG_SEL_SHIFT           (8)
#define FC7300_STCU_MBIST_ALG_TRIG_ALG_SEL_WIDTH           (5)
#  define FC7300_STCU_MBIST_ALG_TRIG_ALG_SEL(x)            (((uint32_t)(((uint32_t)(x))<<FC7300_STCU_MBIST_ALG_TRIG_ALG_SEL_SHIFT))&FC7300_STCU_MBIST_ALG_TRIG_ALG_SEL_MASK)
#  define FC7300_STCU_MBIST_ALG_TRIG_ALG_SEL_BASIC         (0x1900)
#  define FC7300_STCU_MBIST_ALG_TRIG_ALG_SEL_FULL          (0x1F00)
#define FC7300_STCU_MBIST_ALG_TRIG_INI_EN                  (1 << 13)

/* STCU_MBIST_DONE_STATUS Fields */

#define FC7300_STCU_MBIST_DONE_STATUS_SRAM0                (1 << 0)
#define FC7300_STCU_MBIST_DONE_STATUS_SRAM1                (1 << 1)
#define FC7300_STCU_MBIST_DONE_STATUS_SRAM2_1              (1 << 2)
#define FC7300_STCU_MBIST_DONE_STATUS_SRAM2_2              (1 << 3)
#define FC7300_STCU_MBIST_DONE_STATUS_ITCM_CPU0            (1 << 4)
#define FC7300_STCU_MBIST_DONE_STATUS_DTCM0_CPU0           (1 << 5)
#define FC7300_STCU_MBIST_DONE_STATUS_DTCM1_CPU0           (1 << 6)
#define FC7300_STCU_MBIST_DONE_STATUS_ITCM_CPU1            (1 << 7)
#define FC7300_STCU_MBIST_DONE_STATUS_DTCM0_CPU1           (1 << 8)
#define FC7300_STCU_MBIST_DONE_STATUS_DTCM1_CPU1           (1 << 9)
#define FC7300_STCU_MBIST_DONE_STATUS_ITCM_CPU2            (1 << 10)
#define FC7300_STCU_MBIST_DONE_STATUS_DTCM0_CPU2           (1 << 11)
#define FC7300_STCU_MBIST_DONE_STATUS_DTCM1_CPU2           (1 << 12)
#define FC7300_STCU_MBIST_DONE_STATUS_CACHE_CPU0           (1 << 13)
#define FC7300_STCU_MBIST_DONE_STATUS_CACHE_CPU1           (1 << 14)
#define FC7300_STCU_MBIST_DONE_STATUS_CACHE_CPU2           (1 << 15)
#define FC7300_STCU_MBIST_DONE_STATUS_HSM                  (1 << 16)
#define FC7300_STCU_MBIST_DONE_STATUS_DMA_FLEXCAN          (1 << 17)
#define FC7300_STCU_MBIST_DONE_STATUS_ENET                 (1 << 18)
#define FC7300_STCU_MBIST_DONE_STATUS_ETB                  (1 << 19)
#define FC7300_STCU_MBIST_DONE_STATUS_SYS_ROM              (1 << 20)
#define FC7300_STCU_MBIST_DONE_STATUS_HSM_ROM              (1 << 21)

/* STCU_MBIST_FAIL_STATUS Fields */

#define FC7300_STCU_MBIST_FAIL_STATUS_SRAM0                (1 << 0)
#define FC7300_STCU_MBIST_FAIL_STATUS_SRAM1                (1 << 1)
#define FC7300_STCU_MBIST_FAIL_STATUS_SRAM2_1              (1 << 2)
#define FC7300_STCU_MBIST_FAIL_STATUS_SRAM2_2              (1 << 3)
#define FC7300_STCU_MBIST_FAIL_STATUS_ITCM_CPU0            (1 << 4)
#define FC7300_STCU_MBIST_FAIL_STATUS_DTCM0_CPU0           (1 << 5)
#define FC7300_STCU_MBIST_FAIL_STATUS_DTCM1_CPU0           (1 << 6)
#define FC7300_STCU_MBIST_FAIL_STATUS_ITCM_CPU1            (1 << 7)
#define FC7300_STCU_MBIST_FAIL_STATUS_DTCM0_CPU1           (1 << 8)
#define FC7300_STCU_MBIST_FAIL_STATUS_DTCM1_CPU1           (1 << 9)
#define FC7300_STCU_MBIST_FAIL_STATUS_ITCM_CPU2            (1 << 10)
#define FC7300_STCU_MBIST_FAIL_STATUS_DTCM0_CPU2           (1 << 11)
#define FC7300_STCU_MBIST_FAIL_STATUS_DTCM1_CPU2           (1 << 12)
#define FC7300_STCU_MBIST_FAIL_STATUS_CACHE_CPU0           (1 << 13)
#define FC7300_STCU_MBIST_FAIL_STATUS_CACHE_CPU1           (1 << 14)
#define FC7300_STCU_MBIST_FAIL_STATUS_CACHE_CPU2           (1 << 15)
#define FC7300_STCU_MBIST_FAIL_STATUS_HSM                  (1 << 16)
#define FC7300_STCU_MBIST_FAIL_STATUS_DMA_FLEXCAN          (1 << 17)
#define FC7300_STCU_MBIST_FAIL_STATUS_ENET                 (1 << 18)
#define FC7300_STCU_MBIST_FAIL_STATUS_ETB                  (1 << 19)
#define FC7300_STCU_MBIST_FAIL_STATUS_SYS_ROM              (1 << 20)
#define FC7300_STCU_MBIST_FAIL_STATUS_HSM_ROM              (1 << 21)

/* STCU_USER_GP Fields */

/* STCU_SRAM_INI_CTRL Fields */

#define FC7300_STCU_SRAM_INI_CTRL_EN                       (1 << 0)
#define FC7300_STCU_SRAM_INI_CTRL_LOCK                     (1 << 1)
#define FC7300_STCU_SRAM_INI_CTRL_MODE_MASK                (0x30000)
#define FC7300_STCU_SRAM_INI_CTRL_MODE_SHIFT               (16)
#define FC7300_STCU_SRAM_INI_CTRL_MODE_WIDTH               (2)
#  define FC7300_STCU_SRAM_INI_CTRL_MODE(x)                (((uint32_t)(((uint32_t)(x))<<FC7300_STCU_SRAM_INI_CTRL_MODE_SHIFT))&FC7300_STCU_SRAM_INI_CTRL_MODE_MASK)
#  define FC7300_STCU_SRAM_INI_CTRL_MODE_POR_STANDBY0      (0x0)
#  define FC7300_STCU_SRAM_INI_CTRL_MODE_STANDBY1          (0x10000)
#  define FC7300_STCU_SRAM_INI_CTRL_MODE_STANDBY2          (0x20000)
#  define FC7300_STCU_SRAM_INI_CTRL_MODE_STANDBY3          (0x30000)

/* STCU_SRAM_INI_STATUS Fields */

#define FC7300_STCU_SRAM_INI_STATUS_DONE                   (1 << 0)
#define FC7300_STCU_SRAM_INI_STATUS_BUSY                   (1 << 1)
#define FC7300_STCU_SRAM_INI_STATUS_ABORT                  (1 << 2)

/* STCU_SRAM_INI_SEL Fields */

#define FC7300_STCU_SRAM_INI_SEL_SRAM0                     (1 << 0)
#define FC7300_STCU_SRAM_INI_SEL_SRAM1                     (1 << 1)
#define FC7300_STCU_SRAM_INI_SEL_SRAM2_1                   (1 << 2)
#define FC7300_STCU_SRAM_INI_SEL_SRAM2_2                   (1 << 3)
#define FC7300_STCU_SRAM_INI_SEL_ITCM_CPU0                 (1 << 4)
#define FC7300_STCU_SRAM_INI_SEL_DTCM0_CPU0                (1 << 5)
#define FC7300_STCU_SRAM_INI_SEL_DTCM1_CPU0                (1 << 6)
#define FC7300_STCU_SRAM_INI_SEL_ITCM_CPU1                 (1 << 7)
#define FC7300_STCU_SRAM_INI_SEL_DTCM0_CPU1                (1 << 8)
#define FC7300_STCU_SRAM_INI_SEL_DTCM1_CPU1                (1 << 9)
#define FC7300_STCU_SRAM_INI_SEL_ITCM_CPU2                 (1 << 10)
#define FC7300_STCU_SRAM_INI_SEL_DTCM0_CPU2                (1 << 11)
#define FC7300_STCU_SRAM_INI_SEL_DTCM1_CPU2                (1 << 12)

/* STCU_SRAM_INI_DONE_STATUS Fields */

#define FC7300_STCU_SRAM_INI_DONE_STATUS_SRAM0             (1 << 0)
#define FC7300_STCU_SRAM_INI_DONE_STATUS_SRAM1             (1 << 1)
#define FC7300_STCU_SRAM_INI_DONE_STATUS_SRAM2_1           (1 << 2)
#define FC7300_STCU_SRAM_INI_DONE_STATUS_SRAM2_2           (1 << 3)
#define FC7300_STCU_SRAM_INI_DONE_STATUS_ITCM_CPU0         (1 << 4)
#define FC7300_STCU_SRAM_INI_DONE_STATUS_DTCM0_CPU0        (1 << 5)
#define FC7300_STCU_SRAM_INI_DONE_STATUS_DTCM1_CPU0        (1 << 6)
#define FC7300_STCU_SRAM_INI_DONE_STATUS_ITCM_CPU1         (1 << 7)
#define FC7300_STCU_SRAM_INI_DONE_STATUS_DTCM0_CPU1        (1 << 8)
#define FC7300_STCU_SRAM_INI_DONE_STATUS_DTCM1_CPU1        (1 << 9)
#define FC7300_STCU_SRAM_INI_DONE_STATUS_ITCM_CPU2         (1 << 10)
#define FC7300_STCU_SRAM_INI_DONE_STATUS_DTCM0_CPU2        (1 << 11)

/* STCU_IRQ Fields */

#define FC7300_STCU_IRQ_SIZE_ERR                           (1 << 0)
#define FC7300_STCU_IRQ_SEQ_ERR                            (1 << 1)
#define FC7300_STCU_IRQ_EN                                 (1 << 31)

#endif /* __VENDOR_FLAGCHIP_CHIPS_FC7300_HARDWARE_FC7300_STCU_H */
