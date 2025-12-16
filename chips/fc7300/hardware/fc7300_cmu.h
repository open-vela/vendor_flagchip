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

#ifndef __VENDOR_FLAGCHIP_CHIPS_FC7300_HARDWARE_FC7300_CMU_H
#define __VENDOR_FLAGCHIP_CHIPS_FC7300_HARDWARE_FC7300_CMU_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>
#include "hardware/fc7300_memorymap.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Register offsets *********************************************************/

#define FC7300_CMU_CTRL_OFFSET            0x0008  /* Control Register */
#define FC7300_CMU_MIN_OFFSET             0x0010  /* Minimum Threshold Register */
#define FC7300_CMU_MAX_OFFSET             0x0014  /* Maximum Threshold Register */
#define FC7300_CMU_REF_WINDOW_OFFSET      0x0018  /* Reference Window Register */
#define FC7300_CMU_MON_CNT_OFFSET         0x001C  /* Monitor Counter Register */
#define FC7300_CMU_ST_OFFSET              0x0020  /* Status Register */
#define FC7300_CMU_PERIOD_OFFSET          0x0024  /* Period Monitor Mode Configuration Register */

/* Register bit definitions *************************************************/

/* Control Register */

#define CMU_CTRL_ENABLE                 (1 << 0)  /* Bit 0:  CMU Enable */
#define CMU_CTRL_SW_RST                 (1 << 1)  /* Bit 1:  Software Reset */
                                                  /* Bit 2:  Reserved */
#define CMU_CTRL_STOP_EN                (1 << 3)  /* Bit 3:  Stop Mode Enable */
#define CMU_CTRL_LP_EN                  (1 << 4)  /* Bit 4:  Standby Mode Enable */
#define CMU_CTRL_RESTART_EN             (1 << 5)  /* Bit 5:  Indicates whether the CMU needs to be re-enabled after exiting from the low-power mode */
#define CMU_CTRL_IRQ_EN                 (1 << 6)  /* Bit 6:  Interrupt Enable */
                                                  /* Bit 7-15: Reserved */
#define CMU_CTRL_REF_DIV_SHIFT          (16)      /* Bit 16-18: Divider for Reference Clock */
#define CMU_CTRL_REF_DIV_MASK           (0x07 << CMU_CTRL_REF_DIV_SHIFT)
#  define CMU_CTRL_REF_DIV(x)           (((x) << CMU_CTRL_REF_DIV_SHIFT) & CMU_CTRL_REF_DIV_MASK)
                                                  /* Bit 19-31: Reserved */

/* Minimum Threshold Register */

#define CMU_MIN_MIN_SHIFT               (0)       /* Bit 0-23: The minimum threshold for the monitor counter */
#define CMU_MIN_MIN_MASK                (0x00FFFFFF << CMU_MIN_MIN_SHIFT)
#  define CMU_MIN_MIN(x)                (((x) << CMU_MIN_MIN_SHIFT) & CMU_MIN_MIN_MASK)
                                                  /* Bit 24-31: Reserved */

/* Maximum Threshold Register */

#define CMU_MAX_MAX_SHIFT               (0)       /* Bit 0-23: The maximum threshold for the monitor counter */
#define CMU_MAX_MAX_MASK                (0x00FFFFFF << CMU_MAX_MAX_SHIFT)
#  define CMU_MAX_MAX(x)                (((x) << CMU_MAX_MAX_SHIFT) & CMU_MAX_MAX_MASK)
                                                  /* Bit 24-31: Reserved */

/* Reference Window Register */

#define CMU_REF_WINDOW_REF_WINDOW_SHIFT (0)       /* Bit 0-23: The sample window for the reference clock */
#define CMU_REF_WINDOW_REF_WINDOW_MASK  (0x00FFFFFF << CMU_REF_WINDOW_REF_WINDOW_SHIFT)
#  define CMU_REF_WINDOW_REF_WINDOW(x)  (((x) << CMU_REF_WINDOW_REF_WINDOW_SHIFT) & CMU_REF_WINDOW_REF_WINDOW_MASK)
                                                 /* Bit 24-31: Reserved */

/* Reference Window Register */

#define CMU_MON_CNT_MON_CNT_SHIFT       (0)       /* Bit 0-23: The monitor counter value */
#define CMU_MON_CNT_MON_CNT_MASK        (0x00FFFFFF << CMU_MON_CNT_MON_CNT_SHIFT)
#  define CMU_MON_CNT_MON_CNT(x)        (((x) << CMU_MON_CNT_MON_CNT_SHIFT) & CMU_MON_CNT_MON_CNT_MASK)
                                                  /* Bit 24-31: Reserved */

/* Status Register */

                                                  /* Bit 0-7:  Reserved */
#define CMU_ST_MIS                      (1 << 8)  /* Bit 8:  Monitor clock is mismatched */
                                                  /* Bit 9-14: Reserved */
#define CMU_ST_LOC                      (1 << 15) /* Bit 15: Monitor clock is lost */
                                                  /* Bit 16-31: Reserved */

/* Period Monitor Mode Configuration Register */

#define CMU_PERIOD_EN                   (1 << 0)  /* Bit 0:  Period Monitor Mode ENABLE */
                                                  /* Bit 1-15: Reserved */
#define CMU_PERIOD_WINDOW_SHIFT         (16)      /* Bit 16-19: Period Monitor Mode WIDOW */
#define CMU_PERIOD_WINDOW_MASK          (0x0F << CMU_PERIOD_WINDOW_SHIFT)
#  define CMU_PERIOD_WINDOW(x)          (((x) << CMU_PERIOD_WINDOW_SHIFT) & CMU_PERIOD_WINDOW_MASK)
                                                  /* Bit 20-31: Reserved */

#endif /* __VENDOR_FLAGCHIP_CHIPS_FC7300_HARDWARE_FC7300_CMU_H */
