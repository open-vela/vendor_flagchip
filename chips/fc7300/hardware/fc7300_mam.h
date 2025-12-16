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

#ifndef __VENDOR_FLAGCHIP_CHIPS_FC7300_HARDWARE_FC7300_MAM_H
#define __VENDOR_FLAGCHIP_CHIPS_FC7300_HARDWARE_FC7300_MAM_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>
#include "hardware/fc7300_memorymap.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Register offsets *********************************************************/

#define FC7300_MAM_MAXCFG_OFFSET          0x0000                  /* Matrix Configuration Register */
#define FC7300_MAM_BSTCR_OFFSET           0x0004                  /* Burst Control Register */
#define FC7300_MAM_WDGCR_OFFSET           0x0100                  /* Watchdog Control Register */
#define FC7300_MAM_TOCR_OFFSET            0x0104                  /* Timeout Control Register */
#define FC7300_MAM_ACL_REG_OFFSET(n)      (0x0700 + ((n) * 0x4))  /* Access Control Lock Register */
#define FC7300_MAM_ACR_REG_OFFSET(n)      (0x0800 + ((n) * 0x4))  /* Access Control Register */

/* Register bit definitions *************************************************/

/* Matrix Configuration Register */

#define MAM_MAXCFG_SOFTRESET            (1 << 0)  /* Bit 0:  Software Reset Mode */
                                                  /* Bit 1-7:  Reserved */
#define MAM_MAXCFG_SLAVENUM_SHIFT       (8)       /* Bit 8-15: Slave Number in Matrix */
#define MAM_MAXCFG_SLAVENUM_MASK        (0xFF << MAM_MAXCFG_SLAVENUM_SHIFT)
#  define MAM_MAXCFG_SLAVENUM(x)        (((x) << MAM_MAXCFG_SLAVENUM_SHIFT) & MAM_MAXCFG_SLAVENUM_MASK)
                                                  /* Bit 16-23: Reserved */
#define MAM_MAXCFG_MASTERNUM_SHIFT      (24)      /* Bit 24-31: Master Number in Matrix */
#define MAM_MAXCFG_MASTERNUM_MASK       (0xFF << MAM_MAXCFG_MASTERNUM_SHIFT)
#  define MAM_MAXCFG_MASTERNUM(x)       (((x) << MAM_MAXCFG_MASTERNUM_SHIFT) & MAM_MAXCFG_MASTERNUM_MASK)

/* Burst Control Register */

#define MAM_BSTCR_INCRBREAK0_SHIFT      (0)       /* Bit 0-1:  Master0 Burst (INCR) Transfer Control Register Bit */
#define MAM_BSTCR_INCRBREAK0_MASK       (0x03 << MAM_BSTCR_INCRBREAK0_SHIFT)
#  define MAM_BSTCR_INCRBREAK0(x)       (((x) << MAM_BSTCR_INCRBREAK0_SHIFT) & MAM_BSTCR_INCRBREAK0_MASK)
#define MAM_BSTCR_INCRBREAK1_SHIFT      (2)       /* Bit 2-3:  Master1 Burst (INCR) Transfer Control Register Bit */
#define MAM_BSTCR_INCRBREAK1_MASK       (0x03 << MAM_BSTCR_INCRBREAK1_SHIFT)
#  define MAM_BSTCR_INCRBREAK1(x)       (((x) << MAM_BSTCR_INCRBREAK1_SHIFT) & MAM_BSTCR_INCRBREAK1_MASK)
#define MAM_BSTCR_INCRBREAK2_SHIFT      (4)       /* Bit 4-5:  Master2 Burst (INCR) Transfer Control Register Bit */
#define MAM_BSTCR_INCRBREAK2_MASK       (0x03 << MAM_BSTCR_INCRBREAK2_SHIFT)
#  define MAM_BSTCR_INCRBREAK2(x)       (((x) << MAM_BSTCR_INCRBREAK2_SHIFT) & MAM_BSTCR_INCRBREAK2_MASK)
#define MAM_BSTCR_INCRBREAK3_SHIFT      (6)       /* Bit 6-7:  Master3 Burst (INCR) Transfer Control Register Bit */
#define MAM_BSTCR_INCRBREAK3_MASK       (0x03 << MAM_BSTCR_INCRBREAK3_SHIFT)
#  define MAM_BSTCR_INCRBREAK3(x)       (((x) << MAM_BSTCR_INCRBREAK3_SHIFT) & MAM_BSTCR_INCRBREAK3_MASK)
#define MAM_BSTCR_INCRBREAK4_SHIFT      (8)       /* Bit 8-9:  Master4 Burst (INCR) Transfer Control Register Bit */
#define MAM_BSTCR_INCRBREAK4_MASK       (0x03 << MAM_BSTCR_INCRBREAK4_SHIFT)
#  define MAM_BSTCR_INCRBREAK4(x)       (((x) << MAM_BSTCR_INCRBREAK4_SHIFT) & MAM_BSTCR_INCRBREAK4_MASK)
#define MAM_BSTCR_INCRBREAK5_SHIFT      (10)      /* Bit 10-11: Master5 Burst (INCR) Transfer Control Register Bit */
#define MAM_BSTCR_INCRBREAK5_MASK       (0x03 << MAM_BSTCR_INCRBREAK5_SHIFT)
#  define MAM_BSTCR_INCRBREAK5(x)       (((x) << MAM_BSTCR_INCRBREAK5_SHIFT) & MAM_BSTCR_INCRBREAK5_MASK)
#define MAM_BSTCR_INCRBREAK6_SHIFT      (12)      /* Bit 12-13: Master6 Burst (INCR) Transfer Control Register Bit */
#define MAM_BSTCR_INCRBREAK6_MASK       (0x03 << MAM_BSTCR_INCRBREAK6_SHIFT)
#  define MAM_BSTCR_INCRBREAK6(x)       (((x) << MAM_BSTCR_INCRBREAK6_SHIFT) & MAM_BSTCR_INCRBREAK6_MASK)
                                                  /* Bit 14-31: Reserved */

/* Watchdog Control Register */

#define MAM_WDGCR_WDOGCTRL0             (1 << 0)  /* Bit 0:  Master0 WDOG Control Bit */
#define MAM_WDGCR_WDOGCTRL1             (1 << 1)  /* Bit 1:  Master1 WDOG Control Bit */
#define MAM_WDGCR_WDOGCTRL2             (1 << 2)  /* Bit 2:  Master2 WDOG Control Bit */
#define MAM_WDGCR_WDOGCTRL3             (1 << 3)  /* Bit 3:  Master3 WDOG Control Bit */
#define MAM_WDGCR_WDOGCTRL4             (1 << 4)  /* Bit 4:  Master4 WDOG Control Bit */
#define MAM_WDGCR_WDOGCTRL5             (1 << 5)  /* Bit 5:  Master5 WDOG Control Bit */
#define MAM_WDGCR_WDOGCTRL6             (1 << 6)  /* Bit 6:  Master6 WDOG Control Bit */
                                                  /* Bit 7-15: Reserved */
#define MAM_WDGCR_WDOGSTATUS0           (1 << 16) /* Bit 16: Master0 WDOG Status Bit */
#define MAM_WDGCR_WDOGSTATUS1           (1 << 17) /* Bit 17: Master1 WDOG Status Bit */
#define MAM_WDGCR_WDOGSTATUS2           (1 << 18) /* Bit 18: Master2 WDOG Status Bit */
#define MAM_WDGCR_WDOGSTATUS3           (1 << 19) /* Bit 19: Master3 WDOG Status Bit */
#define MAM_WDGCR_WDOGSTATUS4           (1 << 20) /* Bit 20: Master4 WDOG Status Bit */
#define MAM_WDGCR_WDOGSTATUS5           (1 << 21) /* Bit 21: Master5 WDOG Status Bit */
#define MAM_WDGCR_WDOGSTATUS6           (1 << 22) /* Bit 22: Master6 WDOG Status Bit */
                                                  /* Bit 23-31: Reserved */

/* Timeout Control Register */

#define MAM_TOCR_TIMEOUTCTRL0_SHIFT     (0)       /* Bit 0-1:  Master0 WDOG Timeout Control Bit */
#define MAM_TOCR_TIMEOUTCTRL0_MASK      (0x03 << MAM_TOCR_TIMEOUTCTRL0_SHIFT)
#  define MAM_TOCR_TIMEOUTCTRL0(x)      (((x) << MAM_TOCR_TIMEOUTCTRL0_SHIFT) & MAM_TOCR_TIMEOUTCTRL0_MASK)
#define MAM_TOCR_TIMEOUTCTRL1_SHIFT     (2)       /* Bit 2-3:  Master1 WDOG Timeout Control Bit */
#define MAM_TOCR_TIMEOUTCTRL1_MASK      (0x03 << MAM_TOCR_TIMEOUTCTRL1_SHIFT)
#  define MAM_TOCR_TIMEOUTCTRL1(x)      (((x) << MAM_TOCR_TIMEOUTCTRL1_SHIFT) & MAM_TOCR_TIMEOUTCTRL1_MASK)
#define MAM_TOCR_TIMEOUTCTRL2_SHIFT     (4)       /* Bit 4-5:  Master2 WDOG Timeout Control Bit */
#define MAM_TOCR_TIMEOUTCTRL2_MASK      (0x03 << MAM_TOCR_TIMEOUTCTRL2_SHIFT)
#  define MAM_TOCR_TIMEOUTCTRL2(x)      (((x) << MAM_TOCR_TIMEOUTCTRL2_SHIFT) & MAM_TOCR_TIMEOUTCTRL2_MASK)
#define MAM_TOCR_TIMEOUTCTRL3_SHIFT     (6)       /* Bit 6-7:  Master3 WDOG Timeout Control Bit */
#define MAM_TOCR_TIMEOUTCTRL3_MASK      (0x03 << MAM_TOCR_TIMEOUTCTRL3_SHIFT)
#  define MAM_TOCR_TIMEOUTCTRL3(x)      (((x) << MAM_TOCR_TIMEOUTCTRL3_SHIFT) & MAM_TOCR_TIMEOUTCTRL3_MASK)
#define MAM_TOCR_TIMEOUTCTRL4_SHIFT     (8)       /* Bit 8-9:  Master4 WDOG Timeout Control Bit */
#define MAM_TOCR_TIMEOUTCTRL4_MASK      (0x03 << MAM_TOCR_TIMEOUTCTRL4_SHIFT)
#  define MAM_TOCR_TIMEOUTCTRL4(x)      (((x) << MAM_TOCR_TIMEOUTCTRL4_SHIFT) & MAM_TOCR_TIMEOUTCTRL4_MASK)
#define MAM_TOCR_TIMEOUTCTRL5_SHIFT     (10)      /* Bit 10-11: Master5 WDOG Timeout Control Bit */
#define MAM_TOCR_TIMEOUTCTRL5_MASK      (0x03 << MAM_TOCR_TIMEOUTCTRL5_SHIFT)
#  define MAM_TOCR_TIMEOUTCTRL5(x)      (((x) << MAM_TOCR_TIMEOUTCTRL5_SHIFT) & MAM_TOCR_TIMEOUTCTRL5_MASK)
#define MAM_TOCR_TIMEOUTCTRL6_SHIFT     (12)      /* Bit 12-13: Master6 WDOG Timeout Control Bit */
#define MAM_TOCR_TIMEOUTCTRL6_MASK      (0x03 << MAM_TOCR_TIMEOUTCTRL6_SHIFT)
#  define MAM_TOCR_TIMEOUTCTRL6(x)      (((x) << MAM_TOCR_TIMEOUTCTRL6_SHIFT) & MAM_TOCR_TIMEOUTCTRL6_MASK)
                                                  /* Bit 14-31: Reserved */

/* Access Control Lock Register */

#define MAM_ACL_LOCKBLOCK0              (1 << 0)  /* Bit 0:  Block0 Lock Control Bit */
#define MAM_ACL_LOCKBLOCK1              (1 << 1)  /* Bit 1:  Block1 Lock Control Bit */
#define MAM_ACL_LOCKBLOCK2              (1 << 2)  /* Bit 2:  Block2 Lock Control Bit */
#define MAM_ACL_LOCKBLOCK3              (1 << 3)  /* Bit 3:  Block3 Lock Control Bit */
#define MAM_ACL_LOCKBLOCK4              (1 << 4)  /* Bit 4:  Block4 Lock Control Bit */
#define MAM_ACL_LOCKBLOCK5              (1 << 5)  /* Bit 5:  Block5 Lock Control Bit */
#define MAM_ACL_LOCKBLOCK6              (1 << 6)  /* Bit 6:  Block6 Lock Control Bit */
#define MAM_ACL_LOCKBLOCK7              (1 << 7)  /* Bit 7:  Block7 Lock Control Bit */
#define MAM_ACL_LOCKBLOCK8              (1 << 8)  /* Bit 8:  Block8 Lock Control Bit */
#define MAM_ACL_LOCKBLOCK9              (1 << 9)  /* Bit 9:  Block9 Lock Control Bit */
#define MAM_ACL_LOCKBLOCK10             (1 << 10) /* Bit 10: Block10 Lock Control Bit */
#define MAM_ACL_LOCKBLOCK11             (1 << 11) /* Bit 11: Block11 Lock Control Bit */
#define MAM_ACL_LOCKBLOCK12             (1 << 12) /* Bit 12: Block12 Lock Control Bit */
#define MAM_ACL_LOCKBLOCK13             (1 << 13) /* Bit 13: Block13 Lock Control Bit */
#define MAM_ACL_LOCKBLOCK14             (1 << 14) /* Bit 14: Block14 Lock Control Bit */
#define MAM_ACL_LOCKBLOCK15             (1 << 15) /* Bit 15: Block15 Lock Control Bit */
#define MAM_ACL_LOCKBLOCK16             (1 << 16) /* Bit 16: Block16 Lock Control Bit */
#define MAM_ACL_LOCKBLOCK17             (1 << 17) /* Bit 17: Block17 Lock Control Bit */
#define MAM_ACL_LOCKBLOCK18             (1 << 18) /* Bit 18: Block18 Lock Control Bit */
#define MAM_ACL_LOCKBLOCK19             (1 << 19) /* Bit 19: Block19 Lock Control Bit */
#define MAM_ACL_LOCKBLOCK20             (1 << 20) /* Bit 20: Block20 Lock Control Bit */
#define MAM_ACL_LOCKBLOCK21             (1 << 21) /* Bit 21: Block21 Lock Control Bit */
#define MAM_ACL_LOCKBLOCK22             (1 << 22) /* Bit 22: Block22 Lock Control Bit */
#define MAM_ACL_LOCKBLOCK23             (1 << 23) /* Bit 23: Block23 Lock Control Bit */
#define MAM_ACL_LOCKBLOCK24             (1 << 24) /* Bit 24: Block24 Lock Control Bit */
#define MAM_ACL_LOCKBLOCK25             (1 << 25) /* Bit 25: Block25 Lock Control Bit */
#define MAM_ACL_LOCKBLOCK26             (1 << 26) /* Bit 26: Block26 Lock Control Bit */
#define MAM_ACL_LOCKBLOCK27             (1 << 27) /* Bit 27: Block27 Lock Control Bit */
#define MAM_ACL_LOCKBLOCK28             (1 << 28) /* Bit 28: Block28 Lock Control Bit */
#define MAM_ACL_LOCKBLOCK29             (1 << 29) /* Bit 29: Block29 Lock Control Bit */
#define MAM_ACL_LOCKBLOCK30             (1 << 30) /* Bit 30: Block30 Lock Control Bit */
#define MAM_ACL_LOCKBLOCK31             (1 << 31) /* Bit 31: Block31 Lock Control Bit */

/* Access Control Register */

#define MAM_ACR_ACCPOLICY0_SHIFT        (0)       /* Bit 0-3:  Block0 Access Control (Read, Write or Execute) */
#define MAM_ACR_ACCPOLICY0_MASK         (0x0F << MAM_ACR_ACCPOLICY0_SHIFT)
#  define MAM_ACR_ACCPOLICY0(x)         (((x) << MAM_ACR_ACCPOLICY0_SHIFT) & MAM_ACR_ACCPOLICY0_MASK)
#define MAM_ACR_ACCPOLICY1_SHIFT        (4)       /* Bit 4-7:  Block1 Access Control (Read, Write or Execute) */
#define MAM_ACR_ACCPOLICY1_MASK         (0x0F << MAM_ACR_ACCPOLICY1_SHIFT)
#  define MAM_ACR_ACCPOLICY1(x)         (((x) << MAM_ACR_ACCPOLICY1_SHIFT) & MAM_ACR_ACCPOLICY1_MASK)
#define MAM_ACR_ACCPOLICY2_SHIFT        (8)       /* Bit 8-11: Block2 Access Control (Read, Write or Execute) */
#define MAM_ACR_ACCPOLICY2_MASK         (0x0F << MAM_ACR_ACCPOLICY2_SHIFT)
#  define MAM_ACR_ACCPOLICY2(x)         (((x) << MAM_ACR_ACCPOLICY2_SHIFT) & MAM_ACR_ACCPOLICY2_MASK)
#define MAM_ACR_ACCPOLICY3_SHIFT        (12)      /* Bit 12-15: Block3 Access Control (Read, Write or Execute) */
#define MAM_ACR_ACCPOLICY3_MASK         (0x0F << MAM_ACR_ACCPOLICY3_SHIFT)
#  define MAM_ACR_ACCPOLICY3(x)         (((x) << MAM_ACR_ACCPOLICY3_SHIFT) & MAM_ACR_ACCPOLICY3_MASK)
#define MAM_ACR_ACCPOLICY4_SHIFT        (16)      /* Bit 16-19: Block4 Access Control (Read, Write or Execute) */
#define MAM_ACR_ACCPOLICY4_MASK         (0x0F << MAM_ACR_ACCPOLICY4_SHIFT)
#  define MAM_ACR_ACCPOLICY4(x)         (((x) << MAM_ACR_ACCPOLICY4_SHIFT) & MAM_ACR_ACCPOLICY4_MASK)
#define MAM_ACR_ACCPOLICY5_SHIFT        (20)      /* Bit 20-23: Block5 Access Control (Read, Write or Execute) */
#define MAM_ACR_ACCPOLICY5_MASK         (0x0F << MAM_ACR_ACCPOLICY5_SHIFT)
#  define MAM_ACR_ACCPOLICY5(x)         (((x) << MAM_ACR_ACCPOLICY5_SHIFT) & MAM_ACR_ACCPOLICY5_MASK)
#define MAM_ACR_ACCPOLICY6_SHIFT        (24)      /* Bit 24-27: Block6 Access Control (Read, Write or Execute) */
#define MAM_ACR_ACCPOLICY6_MASK         (0x0F << MAM_ACR_ACCPOLICY6_SHIFT)
#  define MAM_ACR_ACCPOLICY6(x)         (((x) << MAM_ACR_ACCPOLICY6_SHIFT) & MAM_ACR_ACCPOLICY6_MASK)
#define MAM_ACR_ACCPOLICY7_SHIFT        (28)      /* Bit 28-31: Block7 Access Control (Read, Write or Execute) */
#define MAM_ACR_ACCPOLICY7_MASK         (0x0F << MAM_ACR_ACCPOLICY7_SHIFT)
#  define MAM_ACR_ACCPOLICY7(x)         (((x) << MAM_ACR_ACCPOLICY7_SHIFT) & MAM_ACR_ACCPOLICY7_MASK)

#endif /* __VENDOR_FLAGCHIP_CHIPS_FC7300_HARDWARE_FC7300_MAM_H */
