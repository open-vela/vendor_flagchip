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

#ifndef __VENDOR_FLAGCHIP_CHIPS_FC7300_HARDWARE_FC7300_FCSMU_H
#define __VENDOR_FLAGCHIP_CHIPS_FC7300_HARDWARE_FC7300_FCSMU_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>
#include "hardware/fc7300_memorymap.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Register offsets *********************************************************/

#define FC7300_FCSMU_CTRL_OFFSET            0x0000  /* Control Register */
#define FC7300_FCSMU_OPRK_OFFSET            0x0004  /* Operation Key Register */
#define FC7300_FCSMU_SOCTRL_OFFSET          0x0008  /* Status Output Control Register */
#define FC7300_FCSMU_FCCR0_OFFSET           0x000C  /* Fault Clear Control Register 0 */
#define FC7300_FCSMU_FRST0_OFFSET           0x001C  /* Fault Reset Control Register 0 */
#define FC7300_FCSMU_FST0_OFFSET            0x002C  /* Fault Status Register */
#define FC7300_FCSMU_FST_UNLK_OFFSET        0x003C  /* Fault Status Register Unlock Register */
#define FC7300_FCSMU_FE0_OFFSET             0x0040  /* Fault Enable Register */
#define FC7300_FCSMU_WARNING_EN0_OFFSET     0x0050  /* Warning State Enable Register 0 */
#define FC7300_FCSMU_WARNING_TO_OFFSET      0x0060  /* Warning Timeout Interval Register */
#define FC7300_FCSMU_CFG_TO_OFFSET          0x0064  /* Configuration State Timeout Interval Register */
#define FC7300_FCSMU_SOUT_DIAG_OFFSET       0x0068  /* SOUT Diagnostic Register */
#define FC7300_FCSMU_STATUS_OFFSET          0x006C  /* Status Register */
#define FC7300_FCSMU_NTW_OFFSET             0x0070  /* Normal to Warning Register */
#define FC7300_FCSMU_WTF_OFFSET             0x0074  /* Warning to Fault Register */
#define FC7300_FCSMU_NTF_OFFSET             0x0078  /* Normal to Fault Register */
#define FC7300_FCSMU_FTW_OFFSET             0x007C  /* Fault to Warning Register */
#define FC7300_FCSMU_INJECT_OFFSET          0x0084  /* Fault Injection Register */
#define FC7300_FCSMU_IRQ_STAT_OFFSET        0x0088  /* IRQ Status Register */
#define FC7300_FCSMU_IRQ_EN_OFFSET          0x008C  /* IRQ Enable Register */
#define FC7300_FCSMU_TEMP_UNLK_OFFSET       0x0094  /* Temporary Configuration State Unlock Register */
#define FC7300_FCSMU_PERMNT_LOCK_OFFSET     0x0098  /* Permanent Configuration State Lock Register */
#define FC7300_FCSMU_STMR_OFFSET            0x009C  /* SOUT Timer Interval Register */
#define FC7300_FCSMU_WARNING_IEN0_OFFSET    0x00A0  /* Warning State Interrupt Enable Register */
#define FC7300_FCSMU_FAULT_IEN0_OFFSET      0x00B0  /* Fault State Interrupt Enable Register */
#define FC7300_FCSMU_SOUT_EN0_OFFSET        0x00C0  /* SOUT Enable Register */
#define FC7300_FCSMU_WARNING_TMR_OFFSET     0x00D0  /* Warning State Timer Register */
#define FC7300_FCSMU_SM_TMR_OFFSET          0x00D4  /* Safe Mode State Timer Register */
#define FC7300_FCSMU_CFG_TMR_OFFSET         0x00D8  /* Configuration State Timer Register */
#define FC7300_FCSMU_SOUT_TMR_OFFSET        0x00DC  /* SOUT Timer Register */
#define FC7300_FCSMU_CRC_CTRL_OFFSET        0x00E0  /* CRC Control Register */
#define FC7300_FCSMU_CRC_RES_OFFSET         0x00E4  /* CRC Result Register */

/* Register bit definitions *************************************************/

/* Control Register */

#define FCSMU_CTRL_OPC_SHIFT           (0)       /* Bit 0-4:  Operation Code */
#define FCSMU_CTRL_OPC_MASK            (0x1F << FCSMU_CTRL_OPC_SHIFT)
#  define FCSMU_CTRL_OPC(x)            (((x) << FCSMU_CTRL_OPC_SHIFT) & FCSMU_CTRL_OPC_MASK)
                                                 /* Bit 5:  Reserved */
#define FCSMU_CTRL_OPS_SHIFT           (6)       /* Bit 6-7:  Operation Status */
#define FCSMU_CTRL_OPS_MASK            (0x03 << FCSMU_CTRL_OPS_SHIFT)
#  define FCSMU_CTRL_OPS(x)            (((x) << FCSMU_CTRL_OPS_SHIFT) & FCSMU_CTRL_OPS_MASK)
                                                 /* Bit 8:  Reserved */
#define FCSMU_CTRL_DBGEN               (1 << 9)  /* Bit 9:  Debug Mode Enable */
                                                 /* Bit 10-31: Reserved */

/* Operation Key Register */

#define FCSMU_OPRK_OPKEY_SHIFT         (0)       /* Bit 0-31: Operation Unlock Key */
#define FCSMU_OPRK_OPKEY_MASK          (0xFFFFFFFF << FCSMU_OPRK_OPKEY_SHIFT)
#  define FCSMU_OPRK_OPKEY(x)          (((x) << FCSMU_OPRK_OPKEY_SHIFT) & FCSMU_OPRK_OPKEY_MASK)

/* Status Output Control Register */

#define FCSMU_SOCTRL_SOUT_DIV_SHIFT    (0)       /* Bit 0-9:  SOUT Divider Ratio */
#define FCSMU_SOCTRL_SOUT_DIV_MASK     (0x03FF << FCSMU_SOCTRL_SOUT_DIV_SHIFT)
#  define FCSMU_SOCTRL_SOUT_DIV(x)     (((x) << FCSMU_SOCTRL_SOUT_DIV_SHIFT) & FCSMU_SOCTRL_SOUT_DIV_MASK)
#define FCSMU_SOCTRL_SOUT_PTC_SHIFT    (10)      /* Bit 10-12: SOUT Protocol */
#define FCSMU_SOCTRL_SOUT_PTC_MASK     (0x07 << FCSMU_SOCTRL_SOUT_PTC_SHIFT)
#  define FCSMU_SOCTRL_SOUT_PTC(x)     (((x) << FCSMU_SOCTRL_SOUT_PTC_SHIFT) & FCSMU_SOCTRL_SOUT_PTC_MASK)
#define FCSMU_SOCTRL_POLSW             (1 << 13) /* Bit 13: Polarity Switch */
#define FCSMU_SOCTRL_FASTEN            (1 << 14) /* Bit 14: Fast Mode Enable */
                                                 /* Bit 15-16: Reserved */
#define FCSMU_SOCTRL_SMRDT_SHIFT       (17)      /* Bit 17-20: Safe Mode Request Delay Timer */
#define FCSMU_SOCTRL_SMRDT_MASK        (0x0F << FCSMU_SOCTRL_SMRDT_SHIFT)
#  define FCSMU_SOCTRL_SMRDT(x)        (((x) << FCSMU_SOCTRL_SMRDT_SHIFT) & FCSMU_SOCTRL_SMRDT_MASK)
                                                 /* Bit 21-22: Reserved */
#define FCSMU_SOCTRL_SOUT_CTRL_SHIFT   (23)      /* Bit 23-24: SOUT Control */
#define FCSMU_SOCTRL_SOUT_CTRL_MASK    (0x03 << FCSMU_SOCTRL_SOUT_CTRL_SHIFT)
#  define FCSMU_SOCTRL_SOUT_CTRL(x)    (((x) << FCSMU_SOCTRL_SOUT_CTRL_SHIFT) & FCSMU_SOCTRL_SOUT_CTRL_MASK)
#define FCSMU_SOCTRL_SOUT_PEN          (1 << 25) /* Bit 25: SOUT PINs Enable */
                                                 /* Bit 26-31: Reserved */

/* Fault Clear Control Register 0 */

#define FCSMU_FCCR0_FCC_SHIFT          (0)       /* Bit 0-31: Fault Clear Control */
#define FCSMU_FCCR0_FCC_MASK           (0xFFFFFFFF << FCSMU_FCCR0_FCC_SHIFT)
#  define FCSMU_FCCR0_FCC(x)           (((x) << FCSMU_FCCR0_FCC_SHIFT) & FCSMU_FCCR0_FCC_MASK)

/* Fault Reset Control Register 0 */

#define FCSMU_FRST0_FRST_SHIFT         (0)       /* Bit 0-31: Fault Reset Control */
#define FCSMU_FRST0_FRST_MASK          (0xFFFFFFFF << FCSMU_FRST0_FRST_SHIFT)
#  define FCSMU_FRST0_FRST(x)          (((x) << FCSMU_FRST0_FRST_SHIFT) & FCSMU_FRST0_FRST_MASK)

/* Fault Status Register */

#define FCSMU_FST0_ST_SHIFT            (0)       /* Bit 0-31: Fault Status */
#define FCSMU_FST0_ST_MASK             (0xFFFFFFFF << FCSMU_FST0_ST_SHIFT)
#  define FCSMU_FST0_ST(x)             (((x) << FCSMU_FST0_ST_SHIFT) & FCSMU_FST0_ST_MASK)

/* Fault Status Register Unlock Register */

#define FCSMU_FST_UNLK_KEY_SHIFT       (0)       /* Bit 0-31: Key */
#define FCSMU_FST_UNLK_KEY_MASK        (0xFFFFFFFF << FCSMU_FST_UNLK_KEY_SHIFT)
#  define FCSMU_FST_UNLK_KEY(x)        (((x) << FCSMU_FST_UNLK_KEY_SHIFT) & FCSMU_FST_UNLK_KEY_MASK)

/* Fault Enable Register */

#define FCSMU_FE0_EN_SHIFT             (0)       /* Bit 0-31: Enable Bits */
#define FCSMU_FE0_EN_MASK              (0xFFFFFFFF << FCSMU_FE0_EN_SHIFT)
#  define FCSMU_FE0_EN(x)              (((x) << FCSMU_FE0_EN_SHIFT) & FCSMU_FE0_EN_MASK)

/* Warning State Enable Register 0 */

#define FCSMU_WARNING_EN0_EN_SHIFT     (0)       /* Bit 0-31: Enable Bits */
#define FCSMU_WARNING_EN0_EN_MASK      (0xFFFFFFFF << FCSMU_WARNING_EN0_EN_SHIFT)
#  define FCSMU_WARNING_EN0_EN(x)      (((x) << FCSMU_WARNING_EN0_EN_SHIFT) & FCSMU_WARNING_EN0_EN_MASK)

/* Warning Timeout Interval Register */

#define FCSMU_WARNING_TO_TIME_SHIFT    (0)       /* Bit 0-31: Warning Timeout Interval */
#define FCSMU_WARNING_TO_TIME_MASK     (0xFFFFFFFF << FCSMU_WARNING_TO_TIME_SHIFT)
#  define FCSMU_WARNING_TO_TIME(x)     (((x) << FCSMU_WARNING_TO_TIME_SHIFT) & FCSMU_WARNING_TO_TIME_MASK)

/* Configuration State Timeout Interval Register */

#define FCSMU_CFG_TO_TIME_SHIFT        (0)       /* Bit 0-2:  Configuration State Timeout Interval */
#define FCSMU_CFG_TO_TIME_MASK         (0x07 << FCSMU_CFG_TO_TIME_SHIFT)
#  define FCSMU_CFG_TO_TIME(x)         (((x) << FCSMU_CFG_TO_TIME_SHIFT) & FCSMU_CFG_TO_TIME_MASK)
                                                 /* Bit 3-31: Reserved */

/* SOUT Diagnostic Register */
#define FCSMU_SOUT_DIAG_SOUT0          (1 << 0)  /* Bit 0:  SOUT Output 0 */
#define FCSMU_SOUT_DIAG_SOUT1          (1 << 1)  /* Bit 1:  SOUT Output 1 */
                                                 /* Bit 2-3:  Reserved */
#define FCSMU_SOUT_DIAG_SIN0           (1 << 4)  /* Bit 4:  SOUT Input 0 */
#define FCSMU_SOUT_DIAG_SIN1           (1 << 5)  /* Bit 5:  SOUT Input 1 */
                                                 /* Bit 6-31: Reserved */

/* Status Register */

#define FCSMU_STATUS_STAT_SHIFT        (0)       /* Bit 0-2:  FCSMU State */
#define FCSMU_STATUS_STAT_MASK         (0x07 << FCSMU_STATUS_STAT_SHIFT)
#  define FCSMU_STATUS_STAT(x)         (((x) << FCSMU_STATUS_STAT_SHIFT) & FCSMU_STATUS_STAT_MASK)
#define FCSMU_STATUS_FIF               (1 << 3)  /* Bit 3:  FCSMU In Fault */
#define FCSMU_STATUS_SOUTPIN_SHIFT     (4)       /* Bit 4-5:  SOUT PINs */
#define FCSMU_STATUS_SOUTPIN_MASK      (0x03 << FCSMU_STATUS_SOUTPIN_SHIFT)
#  define FCSMU_STATUS_SOUTPIN(x)      (((x) << FCSMU_STATUS_SOUTPIN_SHIFT) & FCSMU_STATUS_SOUTPIN_MASK)
                                                 /* Bit 6-31: Reserved */

/* Normal to Warning Register */

#define FCSMU_NTW_INDEX_SHIFT          (0)       /* Bit 0-7:  Fault Channel Index */
#define FCSMU_NTW_INDEX_MASK           (0xFF << FCSMU_NTW_INDEX_SHIFT)
#  define FCSMU_NTW_INDEX(x)           (((x) << FCSMU_NTW_INDEX_SHIFT) & FCSMU_NTW_INDEX_MASK)
                                                 /* Bit 8-31: Reserved */

/* Warning to Fault Register */

#define FCSMU_WTF_INDEX_SHIFT          (0)       /* Bit 0-7:  Fault Index */
#define FCSMU_WTF_INDEX_MASK           (0xFF << FCSMU_WTF_INDEX_SHIFT)
#  define FCSMU_WTF_INDEX(x)           (((x) << FCSMU_WTF_INDEX_SHIFT) & FCSMU_WTF_INDEX_MASK)
                                                 /* Bit 8:  Reserved */
#define FCSMU_WTF_FLAG                 (1 << 9)  /* Bit 9:  Warning to Fault State Flag */
                                                 /* Bit 10-31: Reserved */

/* Normal to Fault Register */

#define FCSMU_NTF_INDEX_SHIFT          (0)       /* Bit 0-7:  Fault Index */
#define FCSMU_NTF_INDEX_MASK           (0xFF << FCSMU_NTF_INDEX_SHIFT)
#  define FCSMU_NTF_INDEX(x)           (((x) << FCSMU_NTF_INDEX_SHIFT) & FCSMU_NTF_INDEX_MASK)
                                                 /* Bit 8:  Reserved */
#define FCSMU_NTF_FLAG                 (1 << 9)  /* Bit 9:  Normal to Fault State Flag */
                                                 /* Bit 10-31: Reserved */

/* Fault to Warning Register */

#define FCSMU_FTW_INDEX_SHIFT          (0)       /* Bit 0-7:  Fault Index */
#define FCSMU_FTW_INDEX_MASK           (0xFF << FCSMU_FTW_INDEX_SHIFT)
#  define FCSMU_FTW_INDEX(x)           (((x) << FCSMU_FTW_INDEX_SHIFT) & FCSMU_FTW_INDEX_MASK)
                                                 /* Bit 8-31: Reserved */

/* Fault Injection Register */

#define FCSMU_INJECT_INDEX_SHIFT       (0)       /* Bit 0-6:  Fault Injection Index */
#define FCSMU_INJECT_INDEX_MASK        (0x7F << FCSMU_INJECT_INDEX_SHIFT)
#  define FCSMU_INJECT_INDEX(x)        (((x) << FCSMU_INJECT_INDEX_SHIFT) & FCSMU_INJECT_INDEX_MASK)
                                                 /* Bit 7-31: Reserved */

/* IRQ Status Register */

#define FCSMU_IRQ_STAT_CFG_TO_IRQ      (1 << 0)  /* Bit 0:  Configuration Timeout Interrupt Request */
#define FCSMU_IRQ_STAT_WARNING_IRQ     (1 << 1)  /* Bit 1:  Warning Interrupt Request */
#define FCSMU_IRQ_STAT_FAULT_IRQ       (1 << 2)  /* Bit 2:  Fault Interrupt Request */
                                                 /* Bit 3-31: Reserved */

/* IRQ Enable Register */

#define FCSMU_IRQ_EN_CFG_TO_IEN        (1 << 0)  /* Bit 0:  Configuration Timeout Interrupt Enable */
                                                 /* Bit 1-31: Reserved */

/* Temporary Configuration State Unlock Register */

#define FCSMU_TEMP_UNLK_KEY_SHIFT      (0)       /* Bit 0-8:  Fault Index */
#define FCSMU_TEMP_UNLK_KEY_MASK       (0x01FF << FCSMU_TEMP_UNLK_KEY_SHIFT)
#  define FCSMU_TEMP_UNLK_KEY(x)       (((x) << FCSMU_TEMP_UNLK_KEY_SHIFT) & FCSMU_TEMP_UNLK_KEY_MASK)
                                                 /* Bit 9-31: Reserved */

/* Permanent Configuration State Lock Register */

#define FCSMU_PERMNT_LOCK_KEY_SHIFT    (0)       /* Bit 0-8:  Permanent Configuration State Lock Key */
#define FCSMU_PERMNT_LOCK_KEY_MASK     (0x01FF << FCSMU_PERMNT_LOCK_KEY_SHIFT)
#  define FCSMU_PERMNT_LOCK_KEY(x)     (((x) << FCSMU_PERMNT_LOCK_KEY_SHIFT) & FCSMU_PERMNT_LOCK_KEY_MASK)
                                                 /* Bit 9-31: Reserved */

/* SOUT Timer Interval Register */

#define FCSMU_STMR_MINI_TIME_SHIFT     (0)       /* Bit 0-13: Minimum SOUT Timer Interval */
#define FCSMU_STMR_MINI_TIME_MASK      (0x3FFF << FCSMU_STMR_MINI_TIME_SHIFT)
#  define FCSMU_STMR_MINI_TIME(x)      (((x) << FCSMU_STMR_MINI_TIME_SHIFT) & FCSMU_STMR_MINI_TIME_MASK)
                                                 /* Bit 14-31: Reserved */

/* Warning State Interrupt Enable Register */

#define FCSMU_WARNING_IEN0_EN_SHIFT    (0)       /* Bit 0-31: Enable Bits */
#define FCSMU_WARNING_IEN0_EN_MASK     (0xFFFFFFFF << FCSMU_WARNING_IEN0_EN_SHIFT)
#  define FCSMU_WARNING_IEN0_EN(x)     (((x) << FCSMU_WARNING_IEN0_EN_SHIFT) & FCSMU_WARNING_IEN0_EN_MASK)

/* Fault State Interrupt Enable Register */

#define FCSMU_FAULT_IEN0_EN_SHIFT      (0)       /* Bit 0-31: Enable Bits */
#define FCSMU_FAULT_IEN0_EN_MASK       (0xFFFFFFFF << FCSMU_FAULT_IEN0_EN_SHIFT)
#  define FCSMU_FAULT_IEN0_EN(x)       (((x) << FCSMU_FAULT_IEN0_EN_SHIFT) & FCSMU_FAULT_IEN0_EN_MASK)

/* SOUT Enable Register */

#define FCSMU_SOUT_EN0_EN_SHIFT        (0)       /* Bit 0-31: Enable Bits */
#define FCSMU_SOUT_EN0_EN_MASK         (0xFFFFFFFF << FCSMU_SOUT_EN0_EN_SHIFT)
#  define FCSMU_SOUT_EN0_EN(x)         (((x) << FCSMU_SOUT_EN0_EN_SHIFT) & FCSMU_SOUT_EN0_EN_MASK)

/* Warning State Timer Register */

#define FCSMU_WARNING_TMR_VAL_SHIFT    (0)       /* Bit 0-31: Warning State Timer value */
#define FCSMU_WARNING_TMR_VAL_MASK     (0xFFFFFFFF << FCSMU_WARNING_TMR_VAL_SHIFT)
#  define FCSMU_WARNING_TMR_VAL(x)     (((x) << FCSMU_WARNING_TMR_VAL_SHIFT) & FCSMU_WARNING_TMR_VAL_MASK)

/* Safe Mode State Timer Register */

#define FCSMU_SM_TMR_VAL_SHIFT         (0)       /* Bit 0-15: Minimum SOUT Timer Interval */
#define FCSMU_SM_TMR_VAL_MASK          (0xFFFF << FCSMU_SM_TMR_VAL_SHIFT)
#  define FCSMU_SM_TMR_VAL(x)          (((x) << FCSMU_SM_TMR_VAL_SHIFT) & FCSMU_SM_TMR_VAL_MASK)
                                                 /* Bit 16-31: Reserved */

/* Configuration State Timer Register */

#define FCSMU_CFG_TMR_VAL_SHIFT        (0)       /* Bit 0-20: Configuration State Timer Value */
#define FCSMU_CFG_TMR_VAL_MASK         (0x1FFFFF << FCSMU_CFG_TMR_VAL_SHIFT)
#  define FCSMU_CFG_TMR_VAL(x)         (((x) << FCSMU_CFG_TMR_VAL_SHIFT) & FCSMU_CFG_TMR_VAL_MASK)
                                                 /* Bit 21-31: Reserved */

/* SOUT Timer Register */

#define FCSMU_SOUT_TMR_VAL_SHIFT       (0)       /* Bit 0-22: SOUT Timer value */
#define FCSMU_SOUT_TMR_VAL_MASK        (0x7FFFFF << FCSMU_SOUT_TMR_VAL_SHIFT)
#  define FCSMU_SOUT_TMR_VAL(x)        (((x) << FCSMU_SOUT_TMR_VAL_SHIFT) & FCSMU_SOUT_TMR_VAL_MASK)
                                                 /* Bit 21-31: Reserved */

/* CRC Control Register */

#define FCSMU_CRC_CTRL_GEN             (1 << 0)  /* Bit 0:  CRC Generate */
#define FCSMU_CRC_CTRL_TRGEN           (1 << 1)  /* Bit 1:  Trigger Enable */
#define FCSMU_CRC_CTRL_CHKEN           (1 << 2)  /* Bit 2:  CRC Checker Enable */
#define FCSMU_CRC_CTRL_EOEN            (1 << 3)  /* Bit 3:  Error Output Enable */
#define FCSMU_CRC_CTRL_BUSY            (1 << 4)  /* Bit 4:  CRC Busy Indicator */
#define FCSMU_CRC_CTRL_EF              (1 << 5)  /* Bit 5:  CRC Error Flag */
                                                 /* Bit 6-31: Reserved */

/* CRC Result Register */

#define FCSMU_CRC_RES_RESULT_SHIFT     (0)       /* Bit 0-31: CRC Result */
#define FCSMU_CRC_RES_RESULT_MASK      (0xFFFFFFFF << FCSMU_CRC_RES_RESULT_SHIFT)
#  define FCSMU_CRC_RES_RESULT(x)      (((x) << FCSMU_CRC_RES_RESULT_SHIFT) & FCSMU_CRC_RES_RESULT_MASK)

#endif /* __VENDOR_FLAGCHIP_CHIPS_FC7300_HARDWARE_FC7300_FCSMU_H */
