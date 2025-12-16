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

#ifndef __VENDOR_FLAGCHIP_CHIPS_FC7300_HARDWARE_FC7300_RGM_H
#define __VENDOR_FLAGCHIP_CHIPS_FC7300_HARDWARE_FC7300_RGM_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>
#include <hardware/fc7300_memorymap.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * RGM Register Offsets
 ****************************************************************************/

#define FC7300_RGM_SRS_OFFSET       0x0008  /* RGM System Reset Status Register */
#define FC7300_RGM_RSTFLT_OFFSET    0x000C  /* RGM Reset Pin Filter Register */
#define FC7300_RGM_SSRS_OFFSET      0x0018  /* RGM Sticky Reset Status Register */
#define FC7300_RGM_SRIE_OFFSET      0x001c  /* RGM System Reset Interrupt Enable Register */
#define FC7300_RGM_C0_CFG_OFFSET    0x0100  /* CPU0 Reset Configuration Register */
#define FC7300_RGM_C0_RST_OFFSET    0x0104  /* CPU0 Reset Register */
#define FC7300_RGM_C0_SRS_OFFSET    0x0108  /* CPU0 System Reset Status Register */
#define FC7300_RGM_C0_SSRS_OFFSET   0x0114  /* CPU0 Sticky System Reset Status Register */
#define FC7300_RGM_C1_CFG_OFFSET    0x0200  /* CPU1 Reset Configuration Register */
#define FC7300_RGM_C1_RST_OFFSET    0x0204  /* CPU1 Reset Register */
#define FC7300_RGM_C1_SRS_OFFSET    0x0208  /* CPU1 System Reset Status Register */
#define FC7300_RGM_C1_RLS_OFFSET    0x020C  /* CPU1 Release Register */
#define FC7300_RGM_C1_RIC_OFFSET    0x0210  /* CPU1 Reset Interrupt Control Register */
#define FC7300_RGM_C1_SSRS_OFFSET   0x0214  /* CPU1 Sticky System Reset Status Register */
#define FC7300_RGM_C2_CFG_OFFSET    0x0300  /* CPU2 Reset Configuration Register */
#define FC7300_RGM_C2_RST_OFFSET    0x0304  /* CPU2 Reset Register */
#define FC7300_RGM_C2_SRS_OFFSET    0x0308  /* CPU2 System Reset Status Register */
#define FC7300_RGM_C2_RLS_OFFSET    0x030C  /* CPU2 Release Register */
#define FC7300_RGM_C2_RIC_OFFSET    0x0310  /* CPU2 Reset Interrupt Control Register */
#define FC7300_RGM_C2_SSRS_OFFSET   0x0314  /* CPU2 Sticky System Reset Status Register */

/****************************************************************************
 * RGM Register Addresses
 ****************************************************************************/

#define FC7300_RGM_SRS         (FC7300_RGM_BASE + FC7300_RGM_SRS_OFFSET)
#define FC7300_RGM_RSTFLT      (FC7300_RGM_BASE + FC7300_RGM_RSTFLT_OFFSET)
#define FC7300_RGM_SSRS        (FC7300_RGM_BASE + FC7300_RGM_SSRS_OFFSET)
#define FC7300_RGM_SRIE        (FC7300_RGM_BASE + FC7300_RGM_SRIE_OFFSET)
#define FC7300_RGM_C0_CFG      (FC7300_RGM_BASE + FC7300_RGM_C0_CFG_OFFSET)
#define FC7300_RGM_C0_RST      (FC7300_RGM_BASE + FC7300_RGM_C0_RST_OFFSET)
#define FC7300_RGM_C0_SRS      (FC7300_RGM_BASE + FC7300_RGM_C0_SRS_OFFSET)
#define FC7300_RGM_C0_SSRS     (FC7300_RGM_BASE + FC7300_RGM_C0_SSRS_OFFSET)
#define FC7300_RGM_C1_CFG      (FC7300_RGM_BASE + FC7300_RGM_C1_CFG_OFFSET)
#define FC7300_RGM_C1_RST      (FC7300_RGM_BASE + FC7300_RGM_C1_RST_OFFSET)
#define FC7300_RGM_C1_SRS      (FC7300_RGM_BASE + FC7300_RGM_C1_SRS_OFFSET)
#define FC7300_RGM_C1_RLS      (FC7300_RGM_BASE + FC7300_RGM_C1_RLS_OFFSET)
#define FC7300_RGM_C1_RIC      (FC7300_RGM_BASE + FC7300_RGM_C1_RIC_OFFSET)
#define FC7300_RGM_C1_SSRS     (FC7300_RGM_BASE + FC7300_RGM_C1_SSRS_OFFSET)
#define FC7300_RGM_C2_CFG      (FC7300_RGM_BASE + FC7300_RGM_C2_CFG_OFFSET)
#define FC7300_RGM_C2_RST      (FC7300_RGM_BASE + FC7300_RGM_C2_RST_OFFSET)
#define FC7300_RGM_C2_SRS      (FC7300_RGM_BASE + FC7300_RGM_C2_SRS_OFFSET)
#define FC7300_RGM_C2_RLS      (FC7300_RGM_BASE + FC7300_RGM_C2_RLS_OFFSET)
#define FC7300_RGM_C2_RIC      (FC7300_RGM_BASE + FC7300_RGM_C2_RIC_OFFSET)
#define FC7300_RGM_C2_SSRS     (FC7300_RGM_BASE + FC7300_RGM_C2_SSRS_OFFSET)

/****************************************************************************
 * RGM Register Bitfield Definitions
 ****************************************************************************/

/* RGM_SRS Bit Fields */

#define RGM_SRS_SYSRST_TOUT            (1 << 31) /* Bit 31:  System Reset Request Timeout Reset */
#define RGM_SRS_PINRST_TOUT            (1 << 30) /* Bit 30:  Pin Reset Request Timeout Reset */
#define RGM_SRS_FSM_ERR                (1 << 29) /* Bit 29:  FSM Error Reset */
#define RGM_SRS_LBIST                  (1 << 15) /* Bit 15:  Logic BIST Reset */
#define RGM_SRS_CMU                    (1 << 14) /* Bit 14:  CMU Reset */
#define RGM_SRS_SACKERR                (1 << 13) /* Bit 13:  Stop Acknowledge Error */
#define RGM_SRS_SYSAP                  (1 << 11) /* Bit 11:  SYSAP System Reset */
#define RGM_SRS_JTAG                   (1 << 8)  /* Bit 8 :  JTAG generated reset */
#define RGM_SRS_POR                    (1 << 7)  /* Bit 7 :  Power-On Reset */
#define RGM_SRS_PIN                    (1 << 6)  /* Bit 6 :  Pin Reset */
#define RGM_SRS_HSM_WDG                (1 << 5)  /* Bit 5 :  HSM Watchdog Reset */
#define RGM_SRS_FCSMU                  (1 << 4)  /* Bit 4 :  FCSMU Reset */
#define RGM_SRS_CLKERR0                (1 << 3)  /* Bit 3 :  Clock Error0 Reset */
#define RGM_SRS_CLKERR1                (1 << 2)  /* Bit 2 :  Clock Error1 Reset */
#define RGM_SRS_LVR                    (1 << 1)  /* Bit 1 :  Low Voltage Detect Reset */
#define RGM_SRS_WAKEUP                 (1 << 0)  /* Bit 0 :  WAKEUP Reset */

/* RGM_RSTFLT Bit Fields */

#define RGM_RSTFLT_RSTFLT_BUSW_MASK    (0x1F00u)
#define RGM_RSTFLT_RSTFLT_BUSW_SHIFT   (8u)
#define RGM_RSTFLT_RSTFLT_BUSW_WIDTH   (5u)
#  define RGM_RSTFLT_RSTFLT_BUSW(x)    (((uint32_t)(((uint32_t)(x))<<RGM_RSTFLT_RSTFLT_BUSW_SHIFT))&RGM_RSTFLT_RSTFLT_BUSW_MASK)
#define RGM_RSTFLT_RSTFLT_RSTFLT_AON_LP (1 << 2)
#define RGM_RSTFLT_RSTFLT_RSTFLT_AON   (1 << 1)
#define RGM_RSTFLT_RSTFLT_RSTFLT_BUS   (1 << 0)

/* RGM_SRIE Bit Fields */

#define RGM_SRIE_SACKERR_RIE           (1 << 13)
#define RGM_SRIE_INTM_TOUT_RIE         (1 << 12)
#define RGM_SRIE_SYSAP_RIE             (1 << 11)
#define RGM_SRIE_SW_RIE                (1 << 10)
#define RGM_SRIE_CPULOC_RIE            (1 << 9)
#define RGM_SRIE_JTAG_RIE              (1 << 8)
#define RGM_SRIE_GLOBAL_RIE            (1 << 7)
#define RGM_SRIE_WDG_RIE               (1 << 5)
#define RGM_SRIE_CLKERR0_RIE           (1 << 3)
#define RGM_SRIE_DELAY_MASK            (0x3u)
#define RGM_SRIE_DELAY_SHIFT           (0u)
#define RGM_SRIE_DELAY_WIDTH           (2u)
#  define RGM_SRIE_DELAY(x)            (((uint32_t)(((uint32_t)(x))<<RGM_SRIE_DELAY_SHIFT))&RGM_SRIE_DELAY_MASK)
#  define RGM_SRIE_DELAY_8_CLOCK       (0x0u)
#  define RGM_SRIE_DELAY_32_CLOCK      (0x1u)
#  define RGM_SRIE_DELAY_128_CLOCK     (0x2u)
#  define RGM_SRIE_DELAY_512_CLOCK     (0x3u)

/* RGM_C0_CFG Bit Fields */

#define RGM_C0_CFG_WPB_LOCK            (1 << 31)
#define RGM_C0_CFG_WPB_MASK            (0x70000000u)
#define RGM_C0_CFG_WPB_SHIFT           (28u)
#define RGM_C0_CFG_WPB_WIDTH           (3u)
#  define RGM_C0_CFG_WPB(x)            (((uint32_t)(((uint32_t)(x))<<RGM_C0_CFG_WPB_SHIFT))&RGM_C0_CFG_WPB_MASK)
#define RGM_C0_CFG_C0_SWRST_EN         (1 << 20)
#define RGM_C0_CFG_C0_INTM_EN          (1 << 19)
#define RGM_C0_CFG_C0_WDOG_EN          (1 << 18)
#define RGM_C0_CFG_C0_SYSRST_EN        (1 << 17)
#define RGM_C0_CFG_C0_LOCKUP_EN        (1 << 16)
#define RGM_C0_CFG_C0_SWRST_IE         (1 << 4)
#define RGM_C0_CFG_C0_INTM_IE          (1 << 3)
#define RGM_C0_CFG_C0_WDOG_IE          (1 << 2)
#define RGM_C0_CFG_C0_SYSRST_IE        (1 << 1)
#define RGM_C0_CFG_C0_LOCKUP_IE        (1 << 0)

/* RGM_C0_RST Bit Fields */

#define RGM_C0_RST_C0_OUT_OF_RST       (1 << 1)
#define RGM_C0_RST_C0_SWRST            (1 << 0)

/* RGM_C0_SRS Bit Fields */

#define RGM_C0_SRS_SYSRST_TOUT         (1 << 31) /* Bit 31:  System Reset Request Timeout Reset */
#define RGM_C0_SRS_PINRST_TOUT         (1 << 30) /* Bit 30:  Pin Reset Request Timeout Reset */
#define RGM_C0_SRS_FSM_ERR             (1 << 29) /* Bit 29:  FSM Error Reset */
#define RGM_C0_SRS_C0_SWRST            (1 << 20) /* Bit 20:  CPU0 SW Reset */
#define RGM_C0_SRS_C0_INTM             (1 << 19) /* Bit 19:  CPU0 INTM Reset */
#define RGM_C0_SRS_C0_WDOG             (1 << 18) /* Bit 18:  CPU0 WDOG Reset */
#define RGM_C0_SRS_C0_SYSRST           (1 << 17) /* Bit 17:  CPU0 SYSRST Reset */
#define RGM_C0_SRS_C0_LOCKUP           (1 << 16) /* Bit 16:  CPU0 LOCKUP Reset */
#define RGM_C0_SRS_LBIST               (1 << 15) /* Bit 15:  Logic BIST Reset */
#define RGM_C0_SRS_CMU                 (1 << 14) /* Bit 14:  CMU Reset */
#define RGM_C0_SRS_SACKERR             (1 << 13) /* Bit 13:  Stop Acknowledge Error */
#define RGM_C0_SRS_SYSAP               (1 << 11) /* Bit 11:  SYSAP System Reset */
#define RGM_C0_SRS_JTAG                (1 << 8)  /* Bit 8 :  JTAG generated reset */
#define RGM_C0_SRS_POR                 (1 << 7)  /* Bit 7 :  Power-On Reset */
#define RGM_C0_SRS_PIN                 (1 << 6)  /* Bit 6 :  Pin Reset */
#define RGM_C0_SRS_HSM_WDG             (1 << 5)  /* Bit 5 :  HSM Watchdog Reset */
#define RGM_C0_SRS_FCSMU               (1 << 4)  /* Bit 4 :  FCSMU Reset */
#define RGM_C0_SRS_CLKERR0             (1 << 3)  /* Bit 3 :  Clock Error0 Reset */
#define RGM_C0_SRS_CLKERR1             (1 << 2)  /* Bit 2 :  Clock Error1 Reset */
#define RGM_C0_SRS_LVR                 (1 << 1)  /* Bit 1 :  Low Voltage Detect Reset */
#define RGM_C0_SRS_WAKEUP              (1 << 0)  /* Bit 0 :  WAKEUP Reset */

/* RGM_C0_SSRS Bit Fields */

#define RGM_C0_SSRS_SYSRST_TOUT        (1 << 31) /* Bit 31:  System Reset Request Timeout Reset */
#define RGM_C0_SSRS_PINRST_TOUT        (1 << 30) /* Bit 30:  Pin Reset Request Timeout Reset */
#define RGM_C0_SSRS_FSM_ERR            (1 << 29) /* Bit 29:  FSM Error Reset */
#define RGM_C0_SSRS_C0_SWRST           (1 << 20) /* Bit 20:  CPU0 SW Reset */
#define RGM_C0_SSRS_C0_INTM            (1 << 19) /* Bit 19:  CPU0 INTM Reset */
#define RGM_C0_SSRS_C0_WDOG            (1 << 18) /* Bit 18:  CPU0 WDOG Reset */
#define RGM_C0_SSRS_C0_SYSRST          (1 << 17) /* Bit 17:  CPU0 SYSRST Reset */
#define RGM_C0_SSRS_C0_LOCKUP          (1 << 16) /* Bit 16:  CPU0 LOCKUP Reset */
#define RGM_C0_SSRS_LBIST              (1 << 15) /* Bit 15:  Logic BIST Reset */
#define RGM_C0_SSRS_CMU                (1 << 14) /* Bit 14:  CMU Reset */
#define RGM_C0_SSRS_SACKERR            (1 << 13) /* Bit 13:  Stop Acknowledge Error */
#define RGM_C0_SSRS_SYSAP              (1 << 11) /* Bit 11:  SYSAP System Reset */
#define RGM_C0_SSRS_JTAG               (1 << 8)  /* Bit 8 :  JTAG generated reset */
#define RGM_C0_SSRS_POR                (1 << 7)  /* Bit 7 :  Power-On Reset */
#define RGM_C0_SSRS_PIN                (1 << 6)  /* Bit 6 :  Pin Reset */
#define RGM_C0_SSRS_HSM_WDG            (1 << 5)  /* Bit 5 :  HSM Watchdog Reset */
#define RGM_C0_SSRS_FCSMU              (1 << 4)  /* Bit 4 :  FCSMU Reset */
#define RGM_C0_SSRS_CLKERR0            (1 << 3)  /* Bit 3 :  Clock Error0 Reset */
#define RGM_C0_SSRS_CLKERR1            (1 << 2)  /* Bit 2 :  Clock Error1 Reset */
#define RGM_C0_SSRS_LVR                (1 << 1)  /* Bit 1 :  Low Voltage Detect Reset */
#define RGM_C0_SSRS_WAKEUP             (1 << 0)  /* Bit 0 :  WAKEUP Reset */

/* RGM_C1_CFG Bit Fields */

#define RGM_C1_CFG_WPB_LOCK            (1 << 31)
#define RGM_C1_CFG_WPB_MASK            (0x70000000u)
#define RGM_C1_CFG_WPB_SHIFT           (28u)
#define RGM_C1_CFG_WPB_WIDTH           (3u)
#  define RGM_C1_CFG_WPB(x)            (((uint32_t)(((uint32_t)(x))<<RGM_C1_CFG_WPB_SHIFT))&RGM_C1_CFG_WPB_MASK)
#define RGM_C1_CFG_C1_SWRST_EN         (1 << 20)
#define RGM_C1_CFG_C1_INTM_EN          (1 << 19)
#define RGM_C1_CFG_C1_WDOG_EN          (1 << 18)
#define RGM_C1_CFG_C1_SYSRST_EN        (1 << 17)
#define RGM_C1_CFG_C1_LOCKUP_EN        (1 << 16)
#define RGM_C1_CFG_C1_SWRST_IE         (1 << 4)
#define RGM_C1_CFG_C1_INTM_IE          (1 << 3)
#define RGM_C1_CFG_C1_WDOG_IE          (1 << 2)
#define RGM_C1_CFG_C1_SYSRST_IE        (1 << 1)
#define RGM_C1_CFG_C1_LOCKUP_IE        (1 << 0)

/* RGM_C1_RST Bit Fields */

#define RGM_C1_RST_C1_OUT_OF_RST       (1 << 1)
#define RGM_C1_RST_C1_SWRST            (1 << 0)

/* RGM_C1_SRS Bit Fields */

#define RGM_C1_SRS_SYSRST_TOUT         (1 << 31) /* Bit 31:  System Reset Request Timeout Reset */
#define RGM_C1_SRS_PINRST_TOUT         (1 << 30) /* Bit 30:  Pin Reset Request Timeout Reset */
#define RGM_C1_SRS_FSM_ERR             (1 << 29) /* Bit 29:  FSM Error Reset */
#define RGM_C1_SRS_C1_SWRST            (1 << 20) /* Bit 20:  CPU0 SW Reset */
#define RGM_C1_SRS_C1_INTM             (1 << 19) /* Bit 19:  CPU0 INTM Reset */
#define RGM_C1_SRS_C1_WDOG             (1 << 18) /* Bit 18:  CPU0 WDOG Reset */
#define RGM_C1_SRS_C1_SYSRST           (1 << 17) /* Bit 17:  CPU0 SYSRST Reset */
#define RGM_C1_SRS_C1_LOCKUP           (1 << 16) /* Bit 16:  CPU0 LOCKUP Reset */
#define RGM_C1_SRS_LBIST               (1 << 15) /* Bit 15:  Logic BIST Reset */
#define RGM_C1_SRS_CMU                 (1 << 14) /* Bit 14:  CMU Reset */
#define RGM_C1_SRS_SACKERR             (1 << 13) /* Bit 13:  Stop Acknowledge Error */
#define RGM_C1_SRS_SYSAP               (1 << 11) /* Bit 11:  SYSAP System Reset */
#define RGM_C1_SRS_JTAG                (1 << 8)  /* Bit 8 :  JTAG generated reset */
#define RGM_C1_SRS_POR                 (1 << 7)  /* Bit 7 :  Power-On Reset */
#define RGM_C1_SRS_PIN                 (1 << 6)  /* Bit 6 :  Pin Reset */
#define RGM_C1_SRS_HSM_WDG             (1 << 5)  /* Bit 5 :  HSM Watchdog Reset */
#define RGM_C1_SRS_FCSMU               (1 << 4)  /* Bit 4 :  FCSMU Reset */
#define RGM_C1_SRS_CLKERR0             (1 << 3)  /* Bit 3 :  Clock Error0 Reset */
#define RGM_C1_SRS_CLKERR1             (1 << 2)  /* Bit 2 :  Clock Error1 Reset */
#define RGM_C1_SRS_LVR                 (1 << 1)  /* Bit 1 :  Low Voltage Detect Reset */
#define RGM_C1_SRS_WAKEUP              (1 << 0)  /* Bit 0 :  WAKEUP Reset */

/* RGM_C1_RLS Bit Fields */

#define RGM_C1_RLS_C1_RELEASE          (1 << 0)

/* RGM_C1_RIC Bit Fields */

#define RGM_C1_RIC_C1_RIC_RST          (1 << 2)
#define RGM_C1_RIC_C1_RIC_TOC2         (1 << 1)
#define RGM_C1_RIC_C1_RIC_TOC0         (1 << 0)

/* RGM_C1_SSRS Bit Fields */

#define RGM_C1_SSRS_SYSRST_TOUT        (1 << 31) /* Bit 31:  System Reset Request Timeout Reset */
#define RGM_C1_SSRS_PINRST_TOUT        (1 << 30) /* Bit 30:  Pin Reset Request Timeout Reset */
#define RGM_C1_SSRS_FSM_ERR            (1 << 29) /* Bit 29:  FSM Error Reset */
#define RGM_C1_SSRS_C1_SWRST           (1 << 20) /* Bit 20:  CPU0 SW Reset */
#define RGM_C1_SSRS_C1_INTM            (1 << 19) /* Bit 19:  CPU0 INTM Reset */
#define RGM_C1_SSRS_C1_WDOG            (1 << 18) /* Bit 18:  CPU0 WDOG Reset */
#define RGM_C1_SSRS_C1_SYSRST          (1 << 17) /* Bit 17:  CPU0 SYSRST Reset */
#define RGM_C1_SSRS_C1_LOCKUP          (1 << 16) /* Bit 16:  CPU0 LOCKUP Reset */
#define RGM_C1_SSRS_LBIST              (1 << 15) /* Bit 15:  Logic BIST Reset */
#define RGM_C1_SSRS_CMU                (1 << 14) /* Bit 14:  CMU Reset */
#define RGM_C1_SSRS_SACKERR            (1 << 13) /* Bit 13:  Stop Acknowledge Error */
#define RGM_C1_SSRS_SYSAP              (1 << 11) /* Bit 11:  SYSAP System Reset */
#define RGM_C1_SSRS_JTAG               (1 << 8)  /* Bit 8 :  JTAG generated reset */
#define RGM_C1_SSRS_POR                (1 << 7)  /* Bit 7 :  Power-On Reset */
#define RGM_C1_SSRS_PIN                (1 << 6)  /* Bit 6 :  Pin Reset */
#define RGM_C1_SSRS_HSM_WDG            (1 << 5)  /* Bit 5 :  HSM Watchdog Reset */
#define RGM_C1_SSRS_FCSMU              (1 << 4)  /* Bit 4 :  FCSMU Reset */
#define RGM_C1_SSRS_CLKERR0            (1 << 3)  /* Bit 3 :  Clock Error0 Reset */
#define RGM_C1_SSRS_CLKERR1            (1 << 2)  /* Bit 2 :  Clock Error1 Reset */
#define RGM_C1_SSRS_LVR                (1 << 1)  /* Bit 1 :  Low Voltage Detect Reset */
#define RGM_C1_SSRS_WAKEUP             (1 << 0)  /* Bit 0 :  WAKEUP Reset */

/* RGM_C2_CFG Bit Fields */

#define RGM_C2_CFG_WPB_LOCK            (1 << 31)
#define RGM_C2_CFG_WPB_MASK            (0x70000000u)
#define RGM_C2_CFG_WPB_SHIFT           (28u)
#define RGM_C2_CFG_WPB_WIDTH           (3u)
#  define RGM_C2_CFG_WPB(x)            (((uint32_t)(((uint32_t)(x))<<RGM_C2_CFG_WPB_SHIFT))&RGM_C2_CFG_WPB_MASK)
#define RGM_C2_CFG_C2_SWRST_EN         (1 << 20)
#define RGM_C2_CFG_C2_INTM_EN          (1 << 19)
#define RGM_C2_CFG_C2_WDOG_EN          (1 << 18)
#define RGM_C2_CFG_C2_SYSRST_EN        (1 << 17)
#define RGM_C2_CFG_C2_LOCKUP_EN        (1 << 16)
#define RGM_C2_CFG_C2_SWRST_IE         (1 << 4)
#define RGM_C2_CFG_C2_INTM_IE          (1 << 3)
#define RGM_C2_CFG_C2_WDOG_IE          (1 << 2)
#define RGM_C2_CFG_C2_SYSRST_IE        (1 << 1)
#define RGM_C2_CFG_C2_LOCKUP_IE        (1 << 0)

/* RGM_C2_RST Bit Fields */

#define RGM_C2_RST_C2_OUT_OF_RST       (1 << 1)
#define RGM_C2_RST_C2_SWRST            (1 << 0)

/* RGM_C2_SRS Bit Fields */

#define RGM_C2_SRS_SYSRST_TOUT         (1 << 31) /* Bit 31:  System Reset Request Timeout Reset */
#define RGM_C2_SRS_PINRST_TOUT         (1 << 30) /* Bit 30:  Pin Reset Request Timeout Reset */
#define RGM_C2_SRS_FSM_ERR             (1 << 29) /* Bit 29:  FSM Error Reset */
#define RGM_C2_SRS_C2_SWRST            (1 << 20) /* Bit 20:  CPU0 SW Reset */
#define RGM_C2_SRS_C2_INTM             (1 << 19) /* Bit 19:  CPU0 INTM Reset */
#define RGM_C2_SRS_C2_WDOG             (1 << 18) /* Bit 18:  CPU0 WDOG Reset */
#define RGM_C2_SRS_C2_SYSRST           (1 << 17) /* Bit 17:  CPU0 SYSRST Reset */
#define RGM_C2_SRS_C2_LOCKUP           (1 << 16) /* Bit 16:  CPU0 LOCKUP Reset */
#define RGM_C2_SRS_LBIST               (1 << 15) /* Bit 15:  Logic BIST Reset */
#define RGM_C2_SRS_CMU                 (1 << 14) /* Bit 14:  CMU Reset */
#define RGM_C2_SRS_SACKERR             (1 << 13) /* Bit 13:  Stop Acknowledge Error */
#define RGM_C2_SRS_SYSAP               (1 << 11) /* Bit 11:  SYSAP System Reset */
#define RGM_C2_SRS_JTAG                (1 << 8)  /* Bit 8 :  JTAG generated reset */
#define RGM_C2_SRS_POR                 (1 << 7)  /* Bit 7 :  Power-On Reset */
#define RGM_C2_SRS_PIN                 (1 << 6)  /* Bit 6 :  Pin Reset */
#define RGM_C2_SRS_HSM_WDG             (1 << 5)  /* Bit 5 :  HSM Watchdog Reset */
#define RGM_C2_SRS_FCSMU               (1 << 4)  /* Bit 4 :  FCSMU Reset */
#define RGM_C2_SRS_CLKERR0             (1 << 3)  /* Bit 3 :  Clock Error0 Reset */
#define RGM_C2_SRS_CLKERR1             (1 << 2)  /* Bit 2 :  Clock Error1 Reset */
#define RGM_C2_SRS_LVR                 (1 << 1)  /* Bit 1 :  Low Voltage Detect Reset */
#define RGM_C2_SRS_WAKEUP              (1 << 0)  /* Bit 0 :  WAKEUP Reset */

/* RGM_C2_RLS Bit Fields */

#define RGM_C2_RLS_C2_RELEASE          (1 << 0)

/* RGM_C2_RIC Bit Fields */

#define RGM_C2_RIC_C2_RIC_RST          (1 << 2)
#define RGM_C2_RIC_C2_RIC_TOC1         (1 << 1)
#define RGM_C2_RIC_C2_RIC_TOC0         (1 << 0)

/* RGM_C2_SSRS Bit Fields */

#define RGM_C2_SSRS_SYSRST_TOUT        (1 << 31) /* Bit 31:  System Reset Request Timeout Reset */
#define RGM_C2_SSRS_PINRST_TOUT        (1 << 30) /* Bit 30:  Pin Reset Request Timeout Reset */
#define RGM_C2_SSRS_FSM_ERR            (1 << 29) /* Bit 29:  FSM Error Reset */
#define RGM_C2_SSRS_C2_SWRST           (1 << 20) /* Bit 20:  CPU0 SW Reset */
#define RGM_C2_SSRS_C2_INTM            (1 << 19) /* Bit 19:  CPU0 INTM Reset */
#define RGM_C2_SSRS_C2_WDOG            (1 << 18) /* Bit 18:  CPU0 WDOG Reset */
#define RGM_C2_SSRS_C2_SYSRST          (1 << 17) /* Bit 17:  CPU0 SYSRST Reset */
#define RGM_C2_SSRS_C2_LOCKUP          (1 << 16) /* Bit 16:  CPU0 LOCKUP Reset */
#define RGM_C2_SSRS_LBIST              (1 << 15) /* Bit 15:  Logic BIST Reset */
#define RGM_C2_SSRS_CMU                (1 << 14) /* Bit 14:  CMU Reset */
#define RGM_C2_SSRS_SACKERR            (1 << 13) /* Bit 13:  Stop Acknowledge Error */
#define RGM_C2_SSRS_SYSAP              (1 << 11) /* Bit 11:  SYSAP System Reset */
#define RGM_C2_SSRS_JTAG               (1 << 8)  /* Bit 8 :  JTAG generated reset */
#define RGM_C2_SSRS_POR                (1 << 7)  /* Bit 7 :  Power-On Reset */
#define RGM_C2_SSRS_PIN                (1 << 6)  /* Bit 6 :  Pin Reset */
#define RGM_C2_SSRS_HSM_WDG            (1 << 5)  /* Bit 5 :  HSM Watchdog Reset */
#define RGM_C2_SSRS_FCSMU              (1 << 4)  /* Bit 4 :  FCSMU Reset */
#define RGM_C2_SSRS_CLKERR0            (1 << 3)  /* Bit 3 :  Clock Error0 Reset */
#define RGM_C2_SSRS_CLKERR1            (1 << 2)  /* Bit 2 :  Clock Error1 Reset */
#define RGM_C2_SSRS_LVR                (1 << 1)  /* Bit 1 :  Low Voltage Detect Reset */
#define RGM_C2_SSRS_WAKEUP             (1 << 0)  /* Bit 0 :  WAKEUP Reset */

/* RGM_CX_SRS Bit Fields ( 0 <= X <= 2) */

#define RGM_CX_SRS_SYSRST_TOUT         (1 << 31) /* Bit 31:  System Reset Request Timeout Reset */
#define RGM_CX_SRS_PINRST_TOUT         (1 << 30) /* Bit 30:  Pin Reset Request Timeout Reset */
#define RGM_CX_SRS_FSM_ERR             (1 << 29) /* Bit 29:  FSM Error Reset */
#define RGM_CX_SRS_CX_SWRST            (1 << 20) /* Bit 20:  CPUX SW Reset ( 0 <= X <= 2) */
#define RGM_CX_SRS_CX_INTM             (1 << 19) /* Bit 19:  CPUX INTM Reset ( 0 <= X <= 2) */
#define RGM_CX_SRS_CX_WDOG             (1 << 18) /* Bit 18:  CPUX WDOG Reset ( 0 <= X <= 2) */
#define RGM_CX_SRS_CX_SYSRST           (1 << 17) /* Bit 17:  CPUX SYSRST Reset ( 0 <= X <= 2) */
#define RGM_CX_SRS_CX_LOCKUP           (1 << 16) /* Bit 16:  CPUX LOCKUP Reset ( 0 <= X <= 2) */
#define RGM_CX_SRS_LBIST               (1 << 15) /* Bit 15:  Logic BIST Reset */
#define RGM_CX_SRS_CMU                 (1 << 14) /* Bit 14:  CMU Reset */
#define RGM_CX_SRS_SACKERR             (1 << 13) /* Bit 13:  Stop Acknowledge Error */
#define RGM_CX_SRS_SYSAP               (1 << 11) /* Bit 11:  SYSAP System Reset */
#define RGM_CX_SRS_JTAG                (1 << 8)  /* Bit 8 :  JTAG generated reset */
#define RGM_CX_SRS_POR                 (1 << 7)  /* Bit 7 :  Power-On Reset */
#define RGM_CX_SRS_PIN                 (1 << 6)  /* Bit 6 :  Pin Reset */
#define RGM_CX_SRS_HSM_WDG             (1 << 5)  /* Bit 5 :  HSM Watchdog Reset */
#define RGM_CX_SRS_FCSMU               (1 << 4)  /* Bit 4 :  FCSMU Reset */
#define RGM_CX_SRS_CLKERR0             (1 << 3)  /* Bit 3 :  Clock Error0 Reset */
#define RGM_CX_SRS_CLKERR1             (1 << 2)  /* Bit 2 :  Clock Error1 Reset */
#define RGM_CX_SRS_LVR                 (1 << 1)  /* Bit 1 :  Low Voltage Detect Reset */
#define RGM_CX_SRS_WAKEUP              (1 << 0)  /* Bit 0 :  WAKEUP Reset */

#endif /* __VENDOR_FLAGCHIP_CHIPS_FC7300_HARDWARE_FC7300_RGM_H */
