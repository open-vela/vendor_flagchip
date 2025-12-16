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

#ifndef __VENDOR_FLAGCHIP_CHIPS_FC7300_HARDWARE_FC7300_PMC_H
#define __VENDOR_FLAGCHIP_CHIPS_FC7300_HARDWARE_FC7300_PMC_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>
#include "hardware/fc7300_memorymap.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Register offsets *********************************************************/

#define FC7300_PMC_LVSCR_OFFSET           0x0000  /* Low Voltage Status and Control Register */
#define FC7300_PMC_CONFIG_OFFSET          0x0004  /* PMC Configuration Register */

/* Register bit definitions *************************************************/

/* Low Voltage Status and Control Register */

#define PMC_LVSCR_HVD5V_FLAG            (1 << 0)  /* Bit 0:  HVD5V Interrupt Flag on V5 Domain in FPM */
#define PMC_LVSCR_HVD5VB_FLAG           (1 << 1)  /* Bit 1:  HVD5VB Interrupt flag on V5B Domain in FPM */
#define PMC_LVSCR_HVD2P5V_FLAG          (1 << 2)  /* Bit 2:  HVD2P5V Interrupt Flag on V25 Domain in FPM */
#define PMC_LVSCR_HVD1P1V_FLAG          (1 << 3)  /* Bit 3:  HVD1P1V Interrupt Flag on V11 Domain in FPM */
#define PMC_LVSCR_LVD5V_FLAG            (1 << 4)  /* Bit 4:  LVD5V Interrupt Flag on V5 Domain in FPM */
#define PMC_LVSCR_LVD1P5V_FLAG          (1 << 5)  /* Bit 5:  LVD1P5V Interrupt Flag on V15 Domain in FPM */
#define PMC_LVSCR_HVD1P5V_FLAG          (1 << 6)  /* Bit 6:  HVD1P5V Interrupt Flag on V15 Domain in FPM */
                                                  /* Bit 7:  Reserved */
#define PMC_LVSCR_HVD5V_STATUS          (1 << 8)  /* Bit 8:  HVD5V Status on V5 Domain in FPM */
#define PMC_LVSCR_HVD5VB_STATUS         (1 << 9)  /* Bit 9:  HVD5VB Status on V5B Domain in FPM */
#define PMC_LVSCR_HVD2P5V_STATUS        (1 << 10) /* Bit 10: HVD25 Status on V25 Domain in FPM */
#define PMC_LVSCR_HVD1P1V_STATUS        (1 << 11) /* Bit 11: HVD11 Status on V11 Domain in FPM */
#define PMC_LVSCR_LVD5V_STATUS          (1 << 12) /* Bit 12: LVD5V Status on V5 Domain in FPM */
#define PMC_LVSCR_LVD1P5V_STATUS        (1 << 13) /* Bit 13: LVD1P5V Status on V15 Domain in FPM */
#define PMC_LVSCR_HVD1P5V_STATUS        (1 << 14) /* Bit 14: HVD1P5V Status on V15 Domain in FPM */
                                                  /* Bit 15: Reserved */
#define PMC_LVSCR_LVR5V_FPM_FLAG        (1 << 16) /* Bit 16: Low Voltage Reset Flag of V5 Domain in FPM */
#define PMC_LVSCR_LVR5V_RPM_FLAG        (1 << 17) /* Bit 17: Low Voltage Reset Flag of V5 Domain in RPM */
#define PMC_LVSCR_LVR5VB_FPM_FLAG       (1 << 18) /* Bit 18: Low Voltage Reset Flag of V5B Domain in FPM */
#define PMC_LVSCR_LVR5VB_RPM_FLAG       (1 << 19) /* Bit 19: Low Voltage Reset Flag of V5B Domain in RPM */
#define PMC_LVSCR_LVR2P5V_FPM_FLAG      (1 << 20) /* Bit 20: Low Voltage Reset Flag of V25 Domain in FPM */
#define PMC_LVSCR_LVR2P5V_RPM_FLAG      (1 << 21) /* Bit 21: Low Voltage Reset Flag of V25 Domain in RPM */
#define PMC_LVSCR_LVR1P1V_FPM_FLAG      (1 << 22) /* Bit 22: Low Voltage Reset Flag of V11 Domain in FPM */
#define PMC_LVSCR_LVR1P1V_RPM_FLAG      (1 << 23) /* Bit 23: Low Voltage Reset Flag of V11 Domain in RPM */
                                                  /* Bit 24-25: Reserved */
#define PMC_LVSCR_V15_STATUS_SHIFT      (26)      /* Bit 26-27: V15 Supply Driver Status */
#define PMC_LVSCR_V15_STATUS_MASK       (0x03 << PMC_LVSCR_V15_STATUS_SHIFT)
#  define PMC_LVSCR_V15_STATUS(x)       (((x) << PMC_LVSCR_V15_STATUS_SHIFT) & PMC_LVSCR_V15_STATUS_MASK)
                                                  /* Bit 28-30: Reserved */
#define PMC_LVSCR_POR_FLAG              (1 << 31) /* Bit 31: POR Flag */

/* PMC Configuration Register */

                                                  /* Bit 0-2:  Reserved */
#define PMC_CONFIG_RPM_VDD2P5_EN        (1 << 3)  /* Bit 3:  V25 Domain Enable during RPM */
#define PMC_CONFIG_V15_AUTOSW           (1 << 4)  /* Bit 4:  V15 Auto Switch Enable */
#define PMC_CONFIG_V15_CTRL_EN          (1 << 5)  /* Bit 5:  V15 Controller with Onboard NPN Enable */
#define PMC_CONFIG_5VB_MON_EN           (1 << 6)  /* Bit 6:  VDD5V_B LVR Monitor Enable during RPM */
                                                  /* Bit 7:  Reserved */
#define PMC_CONFIG_HVD_IE               (1 << 8)  /* Bit 8:  High Voltage Detect Interrupt Enable */
#define PMC_CONFIG_LVD_IE               (1 << 9)  /* Bit 9:  Low Voltage Detect Interrupt Enable */
                                                  /* Bit 10-14: Reserved */
#define PMC_CONFIG_V15_LOCK             (1 << 15) /* Bit 15: Lock Control for V15_EXT_EN and V15_AUTOSW */
                                                  /* Bit 26-31: Reserved */
#endif /* __VENDOR_FLAGCHIP_CHIPS_FC7300_HARDWARE_FC7300_PMC_H */
