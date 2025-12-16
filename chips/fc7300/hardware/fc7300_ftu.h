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

#ifndef __ARCH_ARM_SRC_FC7300_HARDWARE_FC7300_FTU_H
#define __ARCH_ARM_SRC_FC7300_HARDWARE_FC7300_FTU_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include "hardware/fc7300_memorymap.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Register Offsets *********************************************************/

#define FC7300_FTU_SC_OFFSET            0x0000 /* Status And Control register offset */
#define FC7300_FTU_CNT_OFFSET           0x0004 /* Counter register offset */
#define FC7300_FTU_MOD_OFFSET           0x0008 /* Modulo register offset */
#define FC7300_FTU_C0SC_OFFSET          0x000c /* Channel 0 Status And Control register offset */
#define FC7300_FTU_C0V_OFFSET           0x0010 /* Channel 0 Value register offset */
#define FC7300_FTU_C1SC_OFFSET          0x0014 /* Channel 1 Status And Control register offset */
#define FC7300_FTU_C1V_OFFSET           0x0018 /* Channel 1 Value register offset */
#define FC7300_FTU_C2SC_OFFSET          0x001c /* Channel 2 Status And Control register offset */
#define FC7300_FTU_C2V_OFFSET           0x0020 /* Channel 2 Value register offset */
#define FC7300_FTU_C3SC_OFFSET          0x0024 /* Channel 3 Status And Control register offset */
#define FC7300_FTU_C3V_OFFSET           0x0028 /* Channel 3 Value register offset */
#define FC7300_FTU_C4SC_OFFSET          0x002c /* Channel 4 Status And Control register offset */
#define FC7300_FTU_C4V_OFFSET           0x0030 /* Channel 4 Value register offset */
#define FC7300_FTU_C5SC_OFFSET          0x0034 /* Channel 5 Status And Control register offset */
#define FC7300_FTU_C5V_OFFSET           0x0038 /* Channel 5 Value register offset */
#define FC7300_FTU_C6SC_OFFSET          0x003c /* Channel 6 Status And Control register offset */
#define FC7300_FTU_C6V_OFFSET           0x0040 /* Channel 6 Value register offset */
#define FC7300_FTU_C7SC_OFFSET          0x0044 /* Channel 7 Status And Control register offset */
#define FC7300_FTU_C7V_OFFSET           0x0048 /* Channel 7 Value register offset */
#define FC7300_FTU_CNTIN_OFFSET         0x004c /* Counter Initial Value register offset */
#define FC7300_FTU_STATUS_OFFSET        0x0050 /* Capture And Compare Status register offset */
#define FC7300_FTU_MODE_OFFSET          0x0054 /* Features Mode Selection register offset */
#define FC7300_FTU_SYNC_OFFSET          0x0058 /* Synchronization register offset */
#define FC7300_FTU_OUTINIT_OFFSET       0x005c /* Initial State For Channels Output register offset */
#define FC7300_FTU_OUTMASK_OFFSET       0x0060 /* Output Mask register offset */
#define FC7300_FTU_CHCTRL_OFFSET        0x0064 /* Channel control register offset */
#define FC7300_FTU_DEADTIME_OFFSET      0x0068 /* Deadtime Configuration register offset */
#define FC7300_FTU_TRIGCONF_OFFSET      0x006c /* FTU Trigger out configuration register offset */
#define FC7300_FTU_POL_OFFSET           0x0070 /* Channel Polarity register offset */
#define FC7300_FTU_FMS_OFFSET           0x0074 /* Fault Mode Status register offset */
#define FC7300_FTU_FILTER_OFFSET        0x0078 /* Input Capture Filter Control register offset */
#define FC7300_FTU_FLTCTRL_OFFSET       0x007c /* Fault Control register offset */
#define FC7300_FTU_QDCTRL_OFFSET        0x0080 /* Quadrature Decoder Control And Status register offset */
#define FC7300_FTU_CONF_OFFSET          0x0084 /* Configuration register offset */
#define FC7300_FTU_FLTPOL_OFFSET        0x0088 /* FTU Fault Input Polarity register offset */
#define FC7300_FTU_SYNCONF_OFFSET       0x008c /* Synchronization Configuration register offset */
#define FC7300_FTU_INVCTRL_OFFSET       0x0090 /* FTU Inverting Control register offset */
#define FC7300_FTU_SWOCTRL_OFFSET       0x0094 /* FTU Software Output Control register offset */
#define FC7300_FTU_PWMLOAD_OFFSET       0x0098 /* FTU PWM Load register offset */

#define FC7300_FTU_PAIRDEADTIME0_OFFSET 0x00a0 /* Pair 0 Deadtime Configuration register offset */
#define FC7300_FTU_PAIRDEADTIME1_OFFSET 0x00a8 /* Pair 1 Deadtime Configuration register offset */
#define FC7300_FTU_PAIRDEADTIME2_OFFSET 0x00b0 /* Pair 2 Deadtime Configuration register offset */
#define FC7300_FTU_PAIRDEADTIME3_OFFSET 0x00b8 /* Pair 3 Deadtime Configuration register offset */

/* Register Addresses *******************************************************/

/* FTU0 registers */

#define FC7300_FTU0_SC                  (FC7300_FTU0_BASE + FC7300_FTU_SC_OFFSET)            /* FTU0 Status And Control register */
#define FC7300_FTU0_CNT                 (FC7300_FTU0_BASE + FC7300_FTU_CNT_OFFSET)           /* FTU0 Counter register */
#define FC7300_FTU0_MOD                 (FC7300_FTU0_BASE + FC7300_FTU_MOD_OFFSET)           /* FTU0 Modulo register */
#define FC7300_FTU0_C0SC                (FC7300_FTU0_BASE + FC7300_FTU_C0SC_OFFSET)          /* FTU0 Channel 0 Status And Control register */
#define FC7300_FTU0_C0V                 (FC7300_FTU0_BASE + FC7300_FTU_C0V_OFFSET)           /* FTU0 Channel 0 Value register */
#define FC7300_FTU0_C1SC                (FC7300_FTU0_BASE + FC7300_FTU_C1SC_OFFSET)          /* FTU0 Channel 1 Status And Control register */
#define FC7300_FTU0_C1V                 (FC7300_FTU0_BASE + FC7300_FTU_C1V_OFFSET)           /* FTU0 Channel 1 Value register */
#define FC7300_FTU0_C2SC                (FC7300_FTU0_BASE + FC7300_FTU_C2SC_OFFSET)          /* FTU0 Channel 2 Status And Control register */
#define FC7300_FTU0_C2V                 (FC7300_FTU0_BASE + FC7300_FTU_C2V_OFFSET)           /* FTU0 Channel 2 Value register */
#define FC7300_FTU0_C3SC                (FC7300_FTU0_BASE + FC7300_FTU_C3SC_OFFSET)          /* FTU0 Channel 3 Status And Control register */
#define FC7300_FTU0_C3V                 (FC7300_FTU0_BASE + FC7300_FTU_C3V_OFFSET)           /* FTU0 Channel 3 Value register */
#define FC7300_FTU0_C4SC                (FC7300_FTU0_BASE + FC7300_FTU_C4SC_OFFSET)          /* FTU0 Channel 4 Status And Control register */
#define FC7300_FTU0_C4V                 (FC7300_FTU0_BASE + FC7300_FTU_C4V_OFFSET)           /* FTU0 Channel 4 Value register */
#define FC7300_FTU0_C5SC                (FC7300_FTU0_BASE + FC7300_FTU_C5SC_OFFSET)          /* FTU0 Channel 5 Status And Control register */
#define FC7300_FTU0_C5V                 (FC7300_FTU0_BASE + FC7300_FTU_C5V_OFFSET)           /* FTU0 Channel 5 Value register */
#define FC7300_FTU0_C6SC                (FC7300_FTU0_BASE + FC7300_FTU_C6SC_OFFSET)          /* FTU0 Channel 6 Status And Control register */
#define FC7300_FTU0_C6V                 (FC7300_FTU0_BASE + FC7300_FTU_C6V_OFFSET)           /* FTU0 Channel 6 Value register */
#define FC7300_FTU0_C7SC                (FC7300_FTU0_BASE + FC7300_FTU_C7SC_OFFSET)          /* FTU0 Channel 7 Status And Control register */
#define FC7300_FTU0_C7V                 (FC7300_FTU0_BASE + FC7300_FTU_C7V_OFFSET)           /* FTU0 Channel 7 Value register */
#define FC7300_FTU0_CNTIN               (FC7300_FTU0_BASE + FC7300_FTU_CNTIN_OFFSET)         /* FTU0 Counter Initial Value register */
#define FC7300_FTU0_STATUS              (FC7300_FTU0_BASE + FC7300_FTU_STATUS_OFFSET)        /* FTU0 Capture And Compare Status register */
#define FC7300_FTU0_MODE                (FC7300_FTU0_BASE + FC7300_FTU_MODE_OFFSET)          /* FTU0 Features Mode Selection register */
#define FC7300_FTU0_SYNC                (FC7300_FTU0_BASE + FC7300_FTU_SYNC_OFFSET)          /* FTU0 Synchronization register */
#define FC7300_FTU0_OUTINIT             (FC7300_FTU0_BASE + FC7300_FTU_OUTINIT_OFFSET)       /* FTU0 Initial State For Channels Output register */
#define FC7300_FTU0_OUTMASK             (FC7300_FTU0_BASE + FC7300_FTU_OUTMASK_OFFSET)       /* FTU0 Output Mask register */
#define FC7300_FTU0_CHCTRL              (FC7300_FTU0_BASE + FC7300_FTU_CHCTRL_OFFSET)        /* FTU0 Channel control register offset */
#define FC7300_FTU0_DEADTIME            (FC7300_FTU0_BASE + FC7300_FTU_DEADTIME_OFFSET)      /* FTU0 Deadtime Configuration register */
#define FC7300_FTU0_TRIGCONF            (FC7300_FTU0_BASE + FC7300_FTU_TRIGCONF_OFFSET)      /* FTU0 External Trigger register */
#define FC7300_FTU0_POL                 (FC7300_FTU0_BASE + FC7300_FTU_POL_OFFSET)           /* FTU0 Channel Polarity register */
#define FC7300_FTU0_FMS                 (FC7300_FTU0_BASE + FC7300_FTU_FMS_OFFSET)           /* FTU0 Fault Mode Status register */
#define FC7300_FTU0_FLTER               (FC7300_FTU0_BASE + FC7300_FTU_FILTER_OFFSET)        /* FTU0 Input Capture Filter Control register */
#define FC7300_FTU0_FLTCTRL             (FC7300_FTU0_BASE + FC7300_FTU_FLTCTRL_OFFSET)       /* FTU0 Fault Control register */
#define FC7300_FTU0_QDCTRL              (FC7300_FTU0_BASE + FC7300_FTU_QDCTRL_OFFSET)        /* FTU0 Quadrature Decoder Control And Status register */
#define FC7300_FTU0_CONF                (FC7300_FTU0_BASE + FC7300_FTU_CONF_OFFSET)          /* FTU0 Configuration register */
#define FC7300_FTU0_FLTPOL              (FC7300_FTU0_BASE + FC7300_FTU_FLTPOL_OFFSET)        /* FTU0 Fault Input Polarity register */
#define FC7300_FTU0_SYNCONF             (FC7300_FTU0_BASE + FC7300_FTU_SYNCONF_OFFSET)       /* FTU0 Synchronization Configuration register */
#define FC7300_FTU0_INVCTRL             (FC7300_FTU0_BASE + FC7300_FTU_INVCTRL_OFFSET)       /* FTU0 Inverting Control register */
#define FC7300_FTU0_SWOCTRL             (FC7300_FTU0_BASE + FC7300_FTU_SWOCTRL_OFFSET)       /* FTU0 Software Output Control register */
#define FC7300_FTU0_PWMLOAD             (FC7300_FTU0_BASE + FC7300_FTU_PWMLOAD_OFFSET)       /* FTU0 PWM Load register */

#define FC7300_FTU0_PAIR0DEADTIME       (FC7300_FTU0_BASE + FC7300_FTU_PAIRDEADTIME0_OFFSET) /* FTU0 Pair 0 Deadtime Configuration register */
#define FC7300_FTU0_PAIR1DEADTIME       (FC7300_FTU0_BASE + FC7300_FTU_PAIRDEADTIME1_OFFSET) /* FTU0 Pair 1 Deadtime Configuration register */
#define FC7300_FTU0_PAIR2DEADTIME       (FC7300_FTU0_BASE + FC7300_FTU_PAIRDEADTIME2_OFFSET) /* FTU0 Pair 2 Deadtime Configuration register */
#define FC7300_FTU0_PAIR3DEADTIME       (FC7300_FTU0_BASE + FC7300_FTU_PAIRDEADTIME3_OFFSET) /* FTU0 Pair 3 Deadtime Configuration register oset */

/* FTU1 registers */

#define FC7300_FTU1_SC                  (FC7300_FTU1_BASE + FC7300_FTU_SC_OFFSET)            /* FTU1 Status And Control register */
#define FC7300_FTU1_CNT                 (FC7300_FTU1_BASE + FC7300_FTU_CNT_OFFSET)           /* FTU1 Counter register */
#define FC7300_FTU1_MOD                 (FC7300_FTU1_BASE + FC7300_FTU_MOD_OFFSET)           /* FTU1 Modulo register */
#define FC7300_FTU1_C0SC                (FC7300_FTU1_BASE + FC7300_FTU_C0SC_OFFSET)          /* FTU1 Channel 0 Status And Control register */
#define FC7300_FTU1_C0V                 (FC7300_FTU1_BASE + FC7300_FTU_C0V_OFFSET)           /* FTU1 Channel 0 Value register */
#define FC7300_FTU1_C1SC                (FC7300_FTU1_BASE + FC7300_FTU_C1SC_OFFSET)          /* FTU1 Channel 1 Status And Control register */
#define FC7300_FTU1_C1V                 (FC7300_FTU1_BASE + FC7300_FTU_C1V_OFFSET)           /* FTU1 Channel 1 Value register */
#define FC7300_FTU1_C2SC                (FC7300_FTU1_BASE + FC7300_FTU_C2SC_OFFSET)          /* FTU1 Channel 2 Status And Control register */
#define FC7300_FTU1_C2V                 (FC7300_FTU1_BASE + FC7300_FTU_C2V_OFFSET)           /* FTU1 Channel 2 Value register */
#define FC7300_FTU1_C3SC                (FC7300_FTU1_BASE + FC7300_FTU_C3SC_OFFSET)          /* FTU1 Channel 3 Status And Control register */
#define FC7300_FTU1_C3V                 (FC7300_FTU1_BASE + FC7300_FTU_C3V_OFFSET)           /* FTU1 Channel 3 Value register */
#define FC7300_FTU1_C4SC                (FC7300_FTU1_BASE + FC7300_FTU_C4SC_OFFSET)          /* FTU1 Channel 4 Status And Control register */
#define FC7300_FTU1_C4V                 (FC7300_FTU1_BASE + FC7300_FTU_C4V_OFFSET)           /* FTU1 Channel 4 Value register */
#define FC7300_FTU1_C5SC                (FC7300_FTU1_BASE + FC7300_FTU_C5SC_OFFSET)          /* FTU1 Channel 5 Status And Control register */
#define FC7300_FTU1_C5V                 (FC7300_FTU1_BASE + FC7300_FTU_C5V_OFFSET)           /* FTU1 Channel 5 Value register */
#define FC7300_FTU1_C6SC                (FC7300_FTU1_BASE + FC7300_FTU_C6SC_OFFSET)          /* FTU1 Channel 6 Status And Control register */
#define FC7300_FTU1_C6V                 (FC7300_FTU1_BASE + FC7300_FTU_C6V_OFFSET)           /* FTU1 Channel 6 Value register */
#define FC7300_FTU1_C7SC                (FC7300_FTU1_BASE + FC7300_FTU_C7SC_OFFSET)          /* FTU1 Channel 7 Status And Control register */
#define FC7300_FTU1_C7V                 (FC7300_FTU1_BASE + FC7300_FTU_C7V_OFFSET)           /* FTU1 Channel 7 Value register */
#define FC7300_FTU1_CNTIN               (FC7300_FTU1_BASE + FC7300_FTU_CNTIN_OFFSET)         /* FTU1 Counter Initial Value register */
#define FC7300_FTU1_STATUS              (FC7300_FTU1_BASE + FC7300_FTU_STATUS_OFFSET)        /* FTU1 Capture And Compare Status register */
#define FC7300_FTU1_MODE                (FC7300_FTU1_BASE + FC7300_FTU_MODE_OFFSET)          /* FTU1 Features Mode Selection register */
#define FC7300_FTU1_SYNC                (FC7300_FTU1_BASE + FC7300_FTU_SYNC_OFFSET)          /* FTU1 Synchronization register */
#define FC7300_FTU1_OUTINIT             (FC7300_FTU1_BASE + FC7300_FTU_OUTINIT_OFFSET)       /* FTU1 Initial State For Channels Output register */
#define FC7300_FTU1_OUTMASK             (FC7300_FTU1_BASE + FC7300_FTU_OUTMASK_OFFSET)       /* FTU1 Output Mask register */
#define FC7300_FTU1_CHCTRL              (FC7300_FTU1_BASE + FC7300_FTU_CHCTRL_OFFSET)        /* FTU1 Channel control register offset */
#define FC7300_FTU1_DEADTIME            (FC7300_FTU1_BASE + FC7300_FTU_DEADTIME_OFFSET)      /* FTU1 Deadtime Configuration register */
#define FC7300_FTU1_TRIGCONF            (FC7300_FTU1_BASE + FC7300_FTU_TRIGCONF_OFFSET)      /* FTU1 External Trigger register */
#define FC7300_FTU1_POL                 (FC7300_FTU1_BASE + FC7300_FTU_POL_OFFSET)           /* FTU1 Channel Polarity register */
#define FC7300_FTU1_FMS                 (FC7300_FTU1_BASE + FC7300_FTU_FMS_OFFSET)           /* FTU1 Fault Mode Status register */
#define FC7300_FTU1_FLTER               (FC7300_FTU1_BASE + FC7300_FTU_FILTER_OFFSET)        /* FTU1 Input Capture Filter Control register */
#define FC7300_FTU1_FLTCTRL             (FC7300_FTU1_BASE + FC7300_FTU_FLTCTRL_OFFSET)       /* FTU1 Fault Control register */
#define FC7300_FTU1_QDCTRL              (FC7300_FTU1_BASE + FC7300_FTU_QDCTRL_OFFSET)        /* FTU1 Quadrature Decoder Control And Status register */
#define FC7300_FTU1_CONF                (FC7300_FTU1_BASE + FC7300_FTU_CONF_OFFSET)          /* FTU1 Configuration register */
#define FC7300_FTU1_FLTPOL              (FC7300_FTU1_BASE + FC7300_FTU_FLTPOL_OFFSET)        /* FTU1 Fault Input Polarity register */
#define FC7300_FTU1_SYNCONF             (FC7300_FTU1_BASE + FC7300_FTU_SYNCONF_OFFSET)       /* FTU1 Synchronization Configuration register */
#define FC7300_FTU1_INVCTRL             (FC7300_FTU1_BASE + FC7300_FTU_INVCTRL_OFFSET)       /* FTU1 Inverting Control register */
#define FC7300_FTU1_SWOCTRL             (FC7300_FTU1_BASE + FC7300_FTU_SWOCTRL_OFFSET)       /* FTU1 Software Output Control register */
#define FC7300_FTU1_PWMLOAD             (FC7300_FTU1_BASE + FC7300_FTU_PWMLOAD_OFFSET)       /* FTU1 PWM Load register */

#define FC7300_FTU1_PAIR0DEADTIME       (FC7300_FTU1_BASE + FC7300_FTU_PAIRDEADTIME0_OFFSET) /* FTU1 Pair 0 Deadtime Configuration register */
#define FC7300_FTU1_PAIR1DEADTIME       (FC7300_FTU1_BASE + FC7300_FTU_PAIRDEADTIME1_OFFSET) /* FTU1 Pair 1 Deadtime Configuration register */
#define FC7300_FTU1_PAIR2DEADTIME       (FC7300_FTU1_BASE + FC7300_FTU_PAIRDEADTIME2_OFFSET) /* FTU1 Pair 2 Deadtime Configuration register */
#define FC7300_FTU1_PAIR3DEADTIME       (FC7300_FTU1_BASE + FC7300_FTU_PAIRDEADTIME3_OFFSET) /* FTU1 Pair 3 Deadtime Configuration register oset */

/* FTU2 registers */

#define FC7300_FTU2_SC                  (FC7300_FTU2_BASE + FC7300_FTU_SC_OFFSET)            /* FTU2 Status And Control register */
#define FC7300_FTU2_CNT                 (FC7300_FTU2_BASE + FC7300_FTU_CNT_OFFSET)           /* FTU2 Counter register */
#define FC7300_FTU2_MOD                 (FC7300_FTU2_BASE + FC7300_FTU_MOD_OFFSET)           /* FTU2 Modulo register */
#define FC7300_FTU2_C0SC                (FC7300_FTU2_BASE + FC7300_FTU_C0SC_OFFSET)          /* FTU2 Channel 0 Status And Control register */
#define FC7300_FTU2_C0V                 (FC7300_FTU2_BASE + FC7300_FTU_C0V_OFFSET)           /* FTU2 Channel 0 Value register */
#define FC7300_FTU2_C1SC                (FC7300_FTU2_BASE + FC7300_FTU_C1SC_OFFSET)          /* FTU2 Channel 1 Status And Control register */
#define FC7300_FTU2_C1V                 (FC7300_FTU2_BASE + FC7300_FTU_C1V_OFFSET)           /* FTU2 Channel 1 Value register */
#define FC7300_FTU2_C2SC                (FC7300_FTU2_BASE + FC7300_FTU_C2SC_OFFSET)          /* FTU2 Channel 2 Status And Control register */
#define FC7300_FTU2_C2V                 (FC7300_FTU2_BASE + FC7300_FTU_C2V_OFFSET)           /* FTU2 Channel 2 Value register */
#define FC7300_FTU2_C3SC                (FC7300_FTU2_BASE + FC7300_FTU_C3SC_OFFSET)          /* FTU2 Channel 3 Status And Control register */
#define FC7300_FTU2_C3V                 (FC7300_FTU2_BASE + FC7300_FTU_C3V_OFFSET)           /* FTU2 Channel 3 Value register */
#define FC7300_FTU2_C4SC                (FC7300_FTU2_BASE + FC7300_FTU_C4SC_OFFSET)          /* FTU2 Channel 4 Status And Control register */
#define FC7300_FTU2_C4V                 (FC7300_FTU2_BASE + FC7300_FTU_C4V_OFFSET)           /* FTU2 Channel 4 Value register */
#define FC7300_FTU2_C5SC                (FC7300_FTU2_BASE + FC7300_FTU_C5SC_OFFSET)          /* FTU2 Channel 5 Status And Control register */
#define FC7300_FTU2_C5V                 (FC7300_FTU2_BASE + FC7300_FTU_C5V_OFFSET)           /* FTU2 Channel 5 Value register */
#define FC7300_FTU2_C6SC                (FC7300_FTU2_BASE + FC7300_FTU_C6SC_OFFSET)          /* FTU2 Channel 6 Status And Control register */
#define FC7300_FTU2_C6V                 (FC7300_FTU2_BASE + FC7300_FTU_C6V_OFFSET)           /* FTU2 Channel 6 Value register */
#define FC7300_FTU2_C7SC                (FC7300_FTU2_BASE + FC7300_FTU_C7SC_OFFSET)          /* FTU2 Channel 7 Status And Control register */
#define FC7300_FTU2_C7V                 (FC7300_FTU2_BASE + FC7300_FTU_C7V_OFFSET)           /* FTU2 Channel 7 Value register */
#define FC7300_FTU2_CNTIN               (FC7300_FTU2_BASE + FC7300_FTU_CNTIN_OFFSET)         /* FTU2 Counter Initial Value register */
#define FC7300_FTU2_STATUS              (FC7300_FTU2_BASE + FC7300_FTU_STATUS_OFFSET)        /* FTU2 Capture And Compare Status register */
#define FC7300_FTU2_MODE                (FC7300_FTU2_BASE + FC7300_FTU_MODE_OFFSET)          /* FTU2 Features Mode Selection register */
#define FC7300_FTU2_SYNC                (FC7300_FTU2_BASE + FC7300_FTU_SYNC_OFFSET)          /* FTU2 Synchronization register */
#define FC7300_FTU2_OUTINIT             (FC7300_FTU2_BASE + FC7300_FTU_OUTINIT_OFFSET)       /* FTU2 Initial State For Channels Output register */
#define FC7300_FTU2_OUTMASK             (FC7300_FTU2_BASE + FC7300_FTU_OUTMASK_OFFSET)       /* FTU2 Output Mask register */
#define FC7300_FTU2_CHCTRL              (FC7300_FTU2_BASE + FC7300_FTU_CHCTRL_OFFSET)        /* FTU2 Channel control register offset */
#define FC7300_FTU2_DEADTIME            (FC7300_FTU2_BASE + FC7300_FTU_DEADTIME_OFFSET)      /* FTU2 Deadtime Configuration register */
#define FC7300_FTU2_TRIGCONF            (FC7300_FTU2_BASE + FC7300_FTU_TRIGCONF_OFFSET)      /* FTU2 External Trigger register */
#define FC7300_FTU2_POL                 (FC7300_FTU2_BASE + FC7300_FTU_POL_OFFSET)           /* FTU2 Channel Polarity register */
#define FC7300_FTU2_FMS                 (FC7300_FTU2_BASE + FC7300_FTU_FMS_OFFSET)           /* FTU2 Fault Mode Status register */
#define FC7300_FTU2_FLTER               (FC7300_FTU2_BASE + FC7300_FTU_FILTER_OFFSET)        /* FTU2 Input Capture Filter Control register */
#define FC7300_FTU2_FLTCTRL             (FC7300_FTU2_BASE + FC7300_FTU_FLTCTRL_OFFSET)       /* FTU2 Fault Control register */
#define FC7300_FTU2_QDCTRL              (FC7300_FTU2_BASE + FC7300_FTU_QDCTRL_OFFSET)        /* FTU2 Quadrature Decoder Control And Status register */
#define FC7300_FTU2_CONF                (FC7300_FTU2_BASE + FC7300_FTU_CONF_OFFSET)          /* FTU2 Configuration register */
#define FC7300_FTU2_FLTPOL              (FC7300_FTU2_BASE + FC7300_FTU_FLTPOL_OFFSET)        /* FTU2 Fault Input Polarity register */
#define FC7300_FTU2_SYNCONF             (FC7300_FTU2_BASE + FC7300_FTU_SYNCONF_OFFSET)       /* FTU2 Synchronization Configuration register */
#define FC7300_FTU2_INVCTRL             (FC7300_FTU2_BASE + FC7300_FTU_INVCTRL_OFFSET)       /* FTU2 Inverting Control register */
#define FC7300_FTU2_SWOCTRL             (FC7300_FTU2_BASE + FC7300_FTU_SWOCTRL_OFFSET)       /* FTU2 Software Output Control register */
#define FC7300_FTU2_PWMLOAD             (FC7300_FTU2_BASE + FC7300_FTU_PWMLOAD_OFFSET)       /* FTU2 PWM Load register */

#define FC7300_FTU2_PAIR0DEADTIME       (FC7300_FTU2_BASE + FC7300_FTU_PAIRDEADTIME0_OFFSET) /* FTU2 Pair 0 Deadtime Configuration register */
#define FC7300_FTU2_PAIR1DEADTIME       (FC7300_FTU2_BASE + FC7300_FTU_PAIRDEADTIME1_OFFSET) /* FTU2 Pair 1 Deadtime Configuration register */
#define FC7300_FTU2_PAIR2DEADTIME       (FC7300_FTU2_BASE + FC7300_FTU_PAIRDEADTIME2_OFFSET) /* FTU2 Pair 2 Deadtime Configuration register */
#define FC7300_FTU2_PAIR3DEADTIME       (FC7300_FTU2_BASE + FC7300_FTU_PAIRDEADTIME3_OFFSET) /* FTU2 Pair 3 Deadtime Configuration register oset */

/* FTU3 registers */

#define FC7300_FTU3_SC                  (FC7300_FTU3_BASE + FC7300_FTU_SC_OFFSET)            /* FTU3 Status And Control register */
#define FC7300_FTU3_CNT                 (FC7300_FTU3_BASE + FC7300_FTU_CNT_OFFSET)           /* FTU3 Counter register */
#define FC7300_FTU3_MOD                 (FC7300_FTU3_BASE + FC7300_FTU_MOD_OFFSET)           /* FTU3 Modulo register */
#define FC7300_FTU3_C0SC                (FC7300_FTU3_BASE + FC7300_FTU_C0SC_OFFSET)          /* FTU3 Channel 0 Status And Control register */
#define FC7300_FTU3_C0V                 (FC7300_FTU3_BASE + FC7300_FTU_C0V_OFFSET)           /* FTU3 Channel 0 Value register */
#define FC7300_FTU3_C1SC                (FC7300_FTU3_BASE + FC7300_FTU_C1SC_OFFSET)          /* FTU3 Channel 1 Status And Control register */
#define FC7300_FTU3_C1V                 (FC7300_FTU3_BASE + FC7300_FTU_C1V_OFFSET)           /* FTU3 Channel 1 Value register */
#define FC7300_FTU3_C2SC                (FC7300_FTU3_BASE + FC7300_FTU_C2SC_OFFSET)          /* FTU3 Channel 2 Status And Control register */
#define FC7300_FTU3_C2V                 (FC7300_FTU3_BASE + FC7300_FTU_C2V_OFFSET)           /* FTU3 Channel 2 Value register */
#define FC7300_FTU3_C3SC                (FC7300_FTU3_BASE + FC7300_FTU_C3SC_OFFSET)          /* FTU3 Channel 3 Status And Control register */
#define FC7300_FTU3_C3V                 (FC7300_FTU3_BASE + FC7300_FTU_C3V_OFFSET)           /* FTU3 Channel 3 Value register */
#define FC7300_FTU3_C4SC                (FC7300_FTU3_BASE + FC7300_FTU_C4SC_OFFSET)          /* FTU3 Channel 4 Status And Control register */
#define FC7300_FTU3_C4V                 (FC7300_FTU3_BASE + FC7300_FTU_C4V_OFFSET)           /* FTU3 Channel 4 Value register */
#define FC7300_FTU3_C5SC                (FC7300_FTU3_BASE + FC7300_FTU_C5SC_OFFSET)          /* FTU3 Channel 5 Status And Control register */
#define FC7300_FTU3_C5V                 (FC7300_FTU3_BASE + FC7300_FTU_C5V_OFFSET)           /* FTU3 Channel 5 Value register */
#define FC7300_FTU3_C6SC                (FC7300_FTU3_BASE + FC7300_FTU_C6SC_OFFSET)          /* FTU3 Channel 6 Status And Control register */
#define FC7300_FTU3_C6V                 (FC7300_FTU3_BASE + FC7300_FTU_C6V_OFFSET)           /* FTU3 Channel 6 Value register */
#define FC7300_FTU3_C7SC                (FC7300_FTU3_BASE + FC7300_FTU_C7SC_OFFSET)          /* FTU3 Channel 7 Status And Control register */
#define FC7300_FTU3_C7V                 (FC7300_FTU3_BASE + FC7300_FTU_C7V_OFFSET)           /* FTU3 Channel 7 Value register */
#define FC7300_FTU3_CNTIN               (FC7300_FTU3_BASE + FC7300_FTU_CNTIN_OFFSET)         /* FTU3 Counter Initial Value register */
#define FC7300_FTU3_STATUS              (FC7300_FTU3_BASE + FC7300_FTU_STATUS_OFFSET)        /* FTU3 Capture And Compare Status register */
#define FC7300_FTU3_MODE                (FC7300_FTU3_BASE + FC7300_FTU_MODE_OFFSET)          /* FTU3 Features Mode Selection register */
#define FC7300_FTU3_SYNC                (FC7300_FTU3_BASE + FC7300_FTU_SYNC_OFFSET)          /* FTU3 Synchronization register */
#define FC7300_FTU3_OUTINIT             (FC7300_FTU3_BASE + FC7300_FTU_OUTINIT_OFFSET)       /* FTU3 Initial State For Channels Output register */
#define FC7300_FTU3_OUTMASK             (FC7300_FTU3_BASE + FC7300_FTU_OUTMASK_OFFSET)       /* FTU3 Output Mask register */
#define FC7300_FTU3_CHCTRL              (FC7300_FTU3_BASE + FC7300_FTU_CHCTRL_OFFSET)        /* FTU3 Channel control register offset */
#define FC7300_FTU3_DEADTIME            (FC7300_FTU3_BASE + FC7300_FTU_DEADTIME_OFFSET)      /* FTU3 Deadtime Configuration register */
#define FC7300_FTU3_TRIGCONF            (FC7300_FTU3_BASE + FC7300_FTU_TRIGCONF_OFFSET)      /* FTU3 External Trigger register */
#define FC7300_FTU3_POL                 (FC7300_FTU3_BASE + FC7300_FTU_POL_OFFSET)           /* FTU3 Channel Polarity register */
#define FC7300_FTU3_FMS                 (FC7300_FTU3_BASE + FC7300_FTU_FMS_OFFSET)           /* FTU3 Fault Mode Status register */
#define FC7300_FTU3_FLTER               (FC7300_FTU3_BASE + FC7300_FTU_FILTER_OFFSET)        /* FTU3 Input Capture Filter Control register */
#define FC7300_FTU3_FLTCTRL             (FC7300_FTU3_BASE + FC7300_FTU_FLTCTRL_OFFSET)       /* FTU3 Fault Control register */
#define FC7300_FTU3_QDCTRL              (FC7300_FTU3_BASE + FC7300_FTU_QDCTRL_OFFSET)        /* FTU3 Quadrature Decoder Control And Status register */
#define FC7300_FTU3_CONF                (FC7300_FTU3_BASE + FC7300_FTU_CONF_OFFSET)          /* FTU3 Configuration register */
#define FC7300_FTU3_FLTPOL              (FC7300_FTU3_BASE + FC7300_FTU_FLTPOL_OFFSET)        /* FTU3 Fault Input Polarity register */
#define FC7300_FTU3_SYNCONF             (FC7300_FTU3_BASE + FC7300_FTU_SYNCONF_OFFSET)       /* FTU3 Synchronization Configuration register */
#define FC7300_FTU3_INVCTRL             (FC7300_FTU3_BASE + FC7300_FTU_INVCTRL_OFFSET)       /* FTU3 Inverting Control register */
#define FC7300_FTU3_SWOCTRL             (FC7300_FTU3_BASE + FC7300_FTU_SWOCTRL_OFFSET)       /* FTU3 Software Output Control register */
#define FC7300_FTU3_PWMLOAD             (FC7300_FTU3_BASE + FC7300_FTU_PWMLOAD_OFFSET)       /* FTU3 PWM Load register */

#define FC7300_FTU3_PAIR0DEADTIME       (FC7300_FTU3_BASE + FC7300_FTU_PAIRDEADTIME0_OFFSET) /* FTU3 Pair 0 Deadtime Configuration register */
#define FC7300_FTU3_PAIR1DEADTIME       (FC7300_FTU3_BASE + FC7300_FTU_PAIRDEADTIME1_OFFSET) /* FTU3 Pair 1 Deadtime Configuration register */
#define FC7300_FTU3_PAIR2DEADTIME       (FC7300_FTU3_BASE + FC7300_FTU_PAIRDEADTIME2_OFFSET) /* FTU3 Pair 2 Deadtime Configuration register */
#define FC7300_FTU3_PAIR3DEADTIME       (FC7300_FTU3_BASE + FC7300_FTU_PAIRDEADTIME3_OFFSET) /* FTU3 Pair 3 Deadtime Configuration register oset */

/* FTU4 registers */

#define FC7300_FTU4_SC                  (FC7300_FTU4_BASE + FC7300_FTU_SC_OFFSET)            /* FTU4 Status And Control register */
#define FC7300_FTU4_CNT                 (FC7300_FTU4_BASE + FC7300_FTU_CNT_OFFSET)           /* FTU4 Counter register */
#define FC7300_FTU4_MOD                 (FC7300_FTU4_BASE + FC7300_FTU_MOD_OFFSET)           /* FTU4 Modulo register */
#define FC7300_FTU4_C0SC                (FC7300_FTU4_BASE + FC7300_FTU_C0SC_OFFSET)          /* FTU4 Channel 0 Status And Control register */
#define FC7300_FTU4_C0V                 (FC7300_FTU4_BASE + FC7300_FTU_C0V_OFFSET)           /* FTU4 Channel 0 Value register */
#define FC7300_FTU4_C1SC                (FC7300_FTU4_BASE + FC7300_FTU_C1SC_OFFSET)          /* FTU4 Channel 1 Status And Control register */
#define FC7300_FTU4_C1V                 (FC7300_FTU4_BASE + FC7300_FTU_C1V_OFFSET)           /* FTU4 Channel 1 Value register */
#define FC7300_FTU4_C2SC                (FC7300_FTU4_BASE + FC7300_FTU_C2SC_OFFSET)          /* FTU4 Channel 2 Status And Control register */
#define FC7300_FTU4_C2V                 (FC7300_FTU4_BASE + FC7300_FTU_C2V_OFFSET)           /* FTU4 Channel 2 Value register */
#define FC7300_FTU4_C3SC                (FC7300_FTU4_BASE + FC7300_FTU_C3SC_OFFSET)          /* FTU4 Channel 3 Status And Control register */
#define FC7300_FTU4_C3V                 (FC7300_FTU4_BASE + FC7300_FTU_C3V_OFFSET)           /* FTU4 Channel 3 Value register */
#define FC7300_FTU4_C4SC                (FC7300_FTU4_BASE + FC7300_FTU_C4SC_OFFSET)          /* FTU4 Channel 4 Status And Control register */
#define FC7300_FTU4_C4V                 (FC7300_FTU4_BASE + FC7300_FTU_C4V_OFFSET)           /* FTU4 Channel 4 Value register */
#define FC7300_FTU4_C5SC                (FC7300_FTU4_BASE + FC7300_FTU_C5SC_OFFSET)          /* FTU4 Channel 5 Status And Control register */
#define FC7300_FTU4_C5V                 (FC7300_FTU4_BASE + FC7300_FTU_C5V_OFFSET)           /* FTU4 Channel 5 Value register */
#define FC7300_FTU4_C6SC                (FC7300_FTU4_BASE + FC7300_FTU_C6SC_OFFSET)          /* FTU4 Channel 6 Status And Control register */
#define FC7300_FTU4_C6V                 (FC7300_FTU4_BASE + FC7300_FTU_C6V_OFFSET)           /* FTU4 Channel 6 Value register */
#define FC7300_FTU4_C7SC                (FC7300_FTU4_BASE + FC7300_FTU_C7SC_OFFSET)          /* FTU4 Channel 7 Status And Control register */
#define FC7300_FTU4_C7V                 (FC7300_FTU4_BASE + FC7300_FTU_C7V_OFFSET)           /* FTU4 Channel 7 Value register */
#define FC7300_FTU4_CNTIN               (FC7300_FTU4_BASE + FC7300_FTU_CNTIN_OFFSET)         /* FTU4 Counter Initial Value register */
#define FC7300_FTU4_STATUS              (FC7300_FTU4_BASE + FC7300_FTU_STATUS_OFFSET)        /* FTU4 Capture And Compare Status register */
#define FC7300_FTU4_MODE                (FC7300_FTU4_BASE + FC7300_FTU_MODE_OFFSET)          /* FTU4 Features Mode Selection register */
#define FC7300_FTU4_SYNC                (FC7300_FTU4_BASE + FC7300_FTU_SYNC_OFFSET)          /* FTU4 Synchronization register */
#define FC7300_FTU4_OUTINIT             (FC7300_FTU4_BASE + FC7300_FTU_OUTINIT_OFFSET)       /* FTU4 Initial State For Channels Output register */
#define FC7300_FTU4_OUTMASK             (FC7300_FTU4_BASE + FC7300_FTU_OUTMASK_OFFSET)       /* FTU4 Output Mask register */
#define FC7300_FTU4_CHCTRL              (FC7300_FTU4_BASE + FC7300_FTU_CHCTRL_OFFSET)        /* FTU4 Channel control register offset */
#define FC7300_FTU4_DEADTIME            (FC7300_FTU4_BASE + FC7300_FTU_DEADTIME_OFFSET)      /* FTU4 Deadtime Configuration register */
#define FC7300_FTU4_TRIGCONF            (FC7300_FTU4_BASE + FC7300_FTU_TRIGCONF_OFFSET)      /* FTU4 External Trigger register */
#define FC7300_FTU4_POL                 (FC7300_FTU4_BASE + FC7300_FTU_POL_OFFSET)           /* FTU4 Channel Polarity register */
#define FC7300_FTU4_FMS                 (FC7300_FTU4_BASE + FC7300_FTU_FMS_OFFSET)           /* FTU4 Fault Mode Status register */
#define FC7300_FTU4_FLTER               (FC7300_FTU4_BASE + FC7300_FTU_FILTER_OFFSET)        /* FTU4 Input Capture Filter Control register */
#define FC7300_FTU4_FLTCTRL             (FC7300_FTU4_BASE + FC7300_FTU_FLTCTRL_OFFSET)       /* FTU4 Fault Control register */
#define FC7300_FTU4_QDCTRL              (FC7300_FTU4_BASE + FC7300_FTU_QDCTRL_OFFSET)        /* FTU4 Quadrature Decoder Control And Status register */
#define FC7300_FTU4_CONF                (FC7300_FTU4_BASE + FC7300_FTU_CONF_OFFSET)          /* FTU4 Configuration register */
#define FC7300_FTU4_FLTPOL              (FC7300_FTU4_BASE + FC7300_FTU_FLTPOL_OFFSET)        /* FTU4 Fault Input Polarity register */
#define FC7300_FTU4_SYNCONF             (FC7300_FTU4_BASE + FC7300_FTU_SYNCONF_OFFSET)       /* FTU4 Synchronization Configuration register */
#define FC7300_FTU4_INVCTRL             (FC7300_FTU4_BASE + FC7300_FTU_INVCTRL_OFFSET)       /* FTU4 Inverting Control register */
#define FC7300_FTU4_SWOCTRL             (FC7300_FTU4_BASE + FC7300_FTU_SWOCTRL_OFFSET)       /* FTU4 Software Output Control register */
#define FC7300_FTU4_PWMLOAD             (FC7300_FTU4_BASE + FC7300_FTU_PWMLOAD_OFFSET)       /* FTU4 PWM Load register */

#define FC7300_FTU4_PAIR0DEADTIME       (FC7300_FTU4_BASE + FC7300_FTU_PAIRDEADTIME0_OFFSET) /* FTU4 Pair 0 Deadtime Configuration register */
#define FC7300_FTU4_PAIR1DEADTIME       (FC7300_FTU4_BASE + FC7300_FTU_PAIRDEADTIME1_OFFSET) /* FTU4 Pair 1 Deadtime Configuration register */
#define FC7300_FTU4_PAIR2DEADTIME       (FC7300_FTU4_BASE + FC7300_FTU_PAIRDEADTIME2_OFFSET) /* FTU4 Pair 2 Deadtime Configuration register */
#define FC7300_FTU4_PAIR3DEADTIME       (FC7300_FTU4_BASE + FC7300_FTU_PAIRDEADTIME3_OFFSET) /* FTU4 Pair 3 Deadtime Configuration register oset */

/* FTU5 registers */

#define FC7300_FTU5_SC                  (FC7300_FTU5_BASE + FC7300_FTU_SC_OFFSET)            /* FTU5 Status And Control register */
#define FC7300_FTU5_CNT                 (FC7300_FTU5_BASE + FC7300_FTU_CNT_OFFSET)           /* FTU5 Counter register */
#define FC7300_FTU5_MOD                 (FC7300_FTU5_BASE + FC7300_FTU_MOD_OFFSET)           /* FTU5 Modulo register */
#define FC7300_FTU5_C0SC                (FC7300_FTU5_BASE + FC7300_FTU_C0SC_OFFSET)          /* FTU5 Channel 0 Status And Control register */
#define FC7300_FTU5_C0V                 (FC7300_FTU5_BASE + FC7300_FTU_C0V_OFFSET)           /* FTU5 Channel 0 Value register */
#define FC7300_FTU5_C1SC                (FC7300_FTU5_BASE + FC7300_FTU_C1SC_OFFSET)          /* FTU5 Channel 1 Status And Control register */
#define FC7300_FTU5_C1V                 (FC7300_FTU5_BASE + FC7300_FTU_C1V_OFFSET)           /* FTU5 Channel 1 Value register */
#define FC7300_FTU5_C2SC                (FC7300_FTU5_BASE + FC7300_FTU_C2SC_OFFSET)          /* FTU5 Channel 2 Status And Control register */
#define FC7300_FTU5_C2V                 (FC7300_FTU5_BASE + FC7300_FTU_C2V_OFFSET)           /* FTU5 Channel 2 Value register */
#define FC7300_FTU5_C3SC                (FC7300_FTU5_BASE + FC7300_FTU_C3SC_OFFSET)          /* FTU5 Channel 3 Status And Control register */
#define FC7300_FTU5_C3V                 (FC7300_FTU5_BASE + FC7300_FTU_C3V_OFFSET)           /* FTU5 Channel 3 Value register */
#define FC7300_FTU5_C4SC                (FC7300_FTU5_BASE + FC7300_FTU_C4SC_OFFSET)          /* FTU5 Channel 4 Status And Control register */
#define FC7300_FTU5_C4V                 (FC7300_FTU5_BASE + FC7300_FTU_C4V_OFFSET)           /* FTU5 Channel 4 Value register */
#define FC7300_FTU5_C5SC                (FC7300_FTU5_BASE + FC7300_FTU_C5SC_OFFSET)          /* FTU5 Channel 5 Status And Control register */
#define FC7300_FTU5_C5V                 (FC7300_FTU5_BASE + FC7300_FTU_C5V_OFFSET)           /* FTU5 Channel 5 Value register */
#define FC7300_FTU5_C6SC                (FC7300_FTU5_BASE + FC7300_FTU_C6SC_OFFSET)          /* FTU5 Channel 6 Status And Control register */
#define FC7300_FTU5_C6V                 (FC7300_FTU5_BASE + FC7300_FTU_C6V_OFFSET)           /* FTU5 Channel 6 Value register */
#define FC7300_FTU5_C7SC                (FC7300_FTU5_BASE + FC7300_FTU_C7SC_OFFSET)          /* FTU5 Channel 7 Status And Control register */
#define FC7300_FTU5_C7V                 (FC7300_FTU5_BASE + FC7300_FTU_C7V_OFFSET)           /* FTU5 Channel 7 Value register */
#define FC7300_FTU5_CNTIN               (FC7300_FTU5_BASE + FC7300_FTU_CNTIN_OFFSET)         /* FTU5 Counter Initial Value register */
#define FC7300_FTU5_STATUS              (FC7300_FTU5_BASE + FC7300_FTU_STATUS_OFFSET)        /* FTU5 Capture And Compare Status register */
#define FC7300_FTU5_MODE                (FC7300_FTU5_BASE + FC7300_FTU_MODE_OFFSET)          /* FTU5 Features Mode Selection register */
#define FC7300_FTU5_SYNC                (FC7300_FTU5_BASE + FC7300_FTU_SYNC_OFFSET)          /* FTU5 Synchronization register */
#define FC7300_FTU5_OUTINIT             (FC7300_FTU5_BASE + FC7300_FTU_OUTINIT_OFFSET)       /* FTU5 Initial State For Channels Output register */
#define FC7300_FTU5_OUTMASK             (FC7300_FTU5_BASE + FC7300_FTU_OUTMASK_OFFSET)       /* FTU5 Output Mask register */
#define FC7300_FTU5_CHCTRL              (FC7300_FTU5_BASE + FC7300_FTU_CHCTRL_OFFSET)        /* FTU5 Channel control register offset */
#define FC7300_FTU5_DEADTIME            (FC7300_FTU5_BASE + FC7300_FTU_DEADTIME_OFFSET)      /* FTU5 Deadtime Configuration register */
#define FC7300_FTU5_TRIGCONF            (FC7300_FTU5_BASE + FC7300_FTU_TRIGCONF_OFFSET)      /* FTU5 External Trigger register */
#define FC7300_FTU5_POL                 (FC7300_FTU5_BASE + FC7300_FTU_POL_OFFSET)           /* FTU5 Channel Polarity register */
#define FC7300_FTU5_FMS                 (FC7300_FTU5_BASE + FC7300_FTU_FMS_OFFSET)           /* FTU5 Fault Mode Status register */
#define FC7300_FTU5_FLTER               (FC7300_FTU5_BASE + FC7300_FTU_FILTER_OFFSET)        /* FTU5 Input Capture Filter Control register */
#define FC7300_FTU5_FLTCTRL             (FC7300_FTU5_BASE + FC7300_FTU_FLTCTRL_OFFSET)       /* FTU5 Fault Control register */
#define FC7300_FTU5_QDCTRL              (FC7300_FTU5_BASE + FC7300_FTU_QDCTRL_OFFSET)        /* FTU5 Quadrature Decoder Control And Status register */
#define FC7300_FTU5_CONF                (FC7300_FTU5_BASE + FC7300_FTU_CONF_OFFSET)          /* FTU5 Configuration register */
#define FC7300_FTU5_FLTPOL              (FC7300_FTU5_BASE + FC7300_FTU_FLTPOL_OFFSET)        /* FTU5 Fault Input Polarity register */
#define FC7300_FTU5_SYNCONF             (FC7300_FTU5_BASE + FC7300_FTU_SYNCONF_OFFSET)       /* FTU5 Synchronization Configuration register */
#define FC7300_FTU5_INVCTRL             (FC7300_FTU5_BASE + FC7300_FTU_INVCTRL_OFFSET)       /* FTU5 Inverting Control register */
#define FC7300_FTU5_SWOCTRL             (FC7300_FTU5_BASE + FC7300_FTU_SWOCTRL_OFFSET)       /* FTU5 Software Output Control register */
#define FC7300_FTU5_PWMLOAD             (FC7300_FTU5_BASE + FC7300_FTU_PWMLOAD_OFFSET)       /* FTU5 PWM Load register */

#define FC7300_FTU5_PAIR0DEADTIME       (FC7300_FTU5_BASE + FC7300_FTU_PAIRDEADTIME0_OFFSET) /* FTU5 Pair 0 Deadtime Configuration register */
#define FC7300_FTU5_PAIR1DEADTIME       (FC7300_FTU5_BASE + FC7300_FTU_PAIRDEADTIME1_OFFSET) /* FTU5 Pair 1 Deadtime Configuration register */
#define FC7300_FTU5_PAIR2DEADTIME       (FC7300_FTU5_BASE + FC7300_FTU_PAIRDEADTIME2_OFFSET) /* FTU5 Pair 2 Deadtime Configuration register */
#define FC7300_FTU5_PAIR3DEADTIME       (FC7300_FTU5_BASE + FC7300_FTU_PAIRDEADTIME3_OFFSET) /* FTU5 Pair 3 Deadtime Configuration register oset */

/* FTU6 registers */

#define FC7300_FTU6_SC                  (FC7300_FTU6_BASE + FC7300_FTU_SC_OFFSET)            /* FTU6 Status And Control register */
#define FC7300_FTU6_CNT                 (FC7300_FTU6_BASE + FC7300_FTU_CNT_OFFSET)           /* FTU6 Counter register */
#define FC7300_FTU6_MOD                 (FC7300_FTU6_BASE + FC7300_FTU_MOD_OFFSET)           /* FTU6 Modulo register */
#define FC7300_FTU6_C0SC                (FC7300_FTU6_BASE + FC7300_FTU_C0SC_OFFSET)          /* FTU6 Channel 0 Status And Control register */
#define FC7300_FTU6_C0V                 (FC7300_FTU6_BASE + FC7300_FTU_C0V_OFFSET)           /* FTU6 Channel 0 Value register */
#define FC7300_FTU6_C1SC                (FC7300_FTU6_BASE + FC7300_FTU_C1SC_OFFSET)          /* FTU6 Channel 1 Status And Control register */
#define FC7300_FTU6_C1V                 (FC7300_FTU6_BASE + FC7300_FTU_C1V_OFFSET)           /* FTU6 Channel 1 Value register */
#define FC7300_FTU6_C2SC                (FC7300_FTU6_BASE + FC7300_FTU_C2SC_OFFSET)          /* FTU6 Channel 2 Status And Control register */
#define FC7300_FTU6_C2V                 (FC7300_FTU6_BASE + FC7300_FTU_C2V_OFFSET)           /* FTU6 Channel 2 Value register */
#define FC7300_FTU6_C3SC                (FC7300_FTU6_BASE + FC7300_FTU_C3SC_OFFSET)          /* FTU6 Channel 3 Status And Control register */
#define FC7300_FTU6_C3V                 (FC7300_FTU6_BASE + FC7300_FTU_C3V_OFFSET)           /* FTU6 Channel 3 Value register */
#define FC7300_FTU6_C4SC                (FC7300_FTU6_BASE + FC7300_FTU_C4SC_OFFSET)          /* FTU6 Channel 4 Status And Control register */
#define FC7300_FTU6_C4V                 (FC7300_FTU6_BASE + FC7300_FTU_C4V_OFFSET)           /* FTU6 Channel 4 Value register */
#define FC7300_FTU6_C5SC                (FC7300_FTU6_BASE + FC7300_FTU_C5SC_OFFSET)          /* FTU6 Channel 5 Status And Control register */
#define FC7300_FTU6_C5V                 (FC7300_FTU6_BASE + FC7300_FTU_C5V_OFFSET)           /* FTU6 Channel 5 Value register */
#define FC7300_FTU6_C6SC                (FC7300_FTU6_BASE + FC7300_FTU_C6SC_OFFSET)          /* FTU6 Channel 6 Status And Control register */
#define FC7300_FTU6_C6V                 (FC7300_FTU6_BASE + FC7300_FTU_C6V_OFFSET)           /* FTU6 Channel 6 Value register */
#define FC7300_FTU6_C7SC                (FC7300_FTU6_BASE + FC7300_FTU_C7SC_OFFSET)          /* FTU6 Channel 7 Status And Control register */
#define FC7300_FTU6_C7V                 (FC7300_FTU6_BASE + FC7300_FTU_C7V_OFFSET)           /* FTU6 Channel 7 Value register */
#define FC7300_FTU6_CNTIN               (FC7300_FTU6_BASE + FC7300_FTU_CNTIN_OFFSET)         /* FTU6 Counter Initial Value register */
#define FC7300_FTU6_STATUS              (FC7300_FTU6_BASE + FC7300_FTU_STATUS_OFFSET)        /* FTU6 Capture And Compare Status register */
#define FC7300_FTU6_MODE                (FC7300_FTU6_BASE + FC7300_FTU_MODE_OFFSET)          /* FTU6 Features Mode Selection register */
#define FC7300_FTU6_SYNC                (FC7300_FTU6_BASE + FC7300_FTU_SYNC_OFFSET)          /* FTU6 Synchronization register */
#define FC7300_FTU6_OUTINIT             (FC7300_FTU6_BASE + FC7300_FTU_OUTINIT_OFFSET)       /* FTU6 Initial State For Channels Output register */
#define FC7300_FTU6_OUTMASK             (FC7300_FTU6_BASE + FC7300_FTU_OUTMASK_OFFSET)       /* FTU6 Output Mask register */
#define FC7300_FTU6_CHCTRL              (FC7300_FTU6_BASE + FC7300_FTU_CHCTRL_OFFSET)        /* FTU6 Channel control register offset */
#define FC7300_FTU6_DEADTIME            (FC7300_FTU6_BASE + FC7300_FTU_DEADTIME_OFFSET)      /* FTU6 Deadtime Configuration register */
#define FC7300_FTU6_TRIGCONF            (FC7300_FTU6_BASE + FC7300_FTU_TRIGCONF_OFFSET)      /* FTU6 External Trigger register */
#define FC7300_FTU6_POL                 (FC7300_FTU6_BASE + FC7300_FTU_POL_OFFSET)           /* FTU6 Channel Polarity register */
#define FC7300_FTU6_FMS                 (FC7300_FTU6_BASE + FC7300_FTU_FMS_OFFSET)           /* FTU6 Fault Mode Status register */
#define FC7300_FTU6_FLTER               (FC7300_FTU6_BASE + FC7300_FTU_FILTER_OFFSET)        /* FTU6 Input Capture Filter Control register */
#define FC7300_FTU6_FLTCTRL             (FC7300_FTU6_BASE + FC7300_FTU_FLTCTRL_OFFSET)       /* FTU6 Fault Control register */
#define FC7300_FTU6_QDCTRL              (FC7300_FTU6_BASE + FC7300_FTU_QDCTRL_OFFSET)        /* FTU6 Quadrature Decoder Control And Status register */
#define FC7300_FTU6_CONF                (FC7300_FTU6_BASE + FC7300_FTU_CONF_OFFSET)          /* FTU6 Configuration register */
#define FC7300_FTU6_FLTPOL              (FC7300_FTU6_BASE + FC7300_FTU_FLTPOL_OFFSET)        /* FTU6 Fault Input Polarity register */
#define FC7300_FTU6_SYNCONF             (FC7300_FTU6_BASE + FC7300_FTU_SYNCONF_OFFSET)       /* FTU6 Synchronization Configuration register */
#define FC7300_FTU6_INVCTRL             (FC7300_FTU6_BASE + FC7300_FTU_INVCTRL_OFFSET)       /* FTU6 Inverting Control register */
#define FC7300_FTU6_SWOCTRL             (FC7300_FTU6_BASE + FC7300_FTU_SWOCTRL_OFFSET)       /* FTU6 Software Output Control register */
#define FC7300_FTU6_PWMLOAD             (FC7300_FTU6_BASE + FC7300_FTU_PWMLOAD_OFFSET)       /* FTU6 PWM Load register */

#define FC7300_FTU6_PAIR0DEADTIME       (FC7300_FTU6_BASE + FC7300_FTU_PAIRDEADTIME0_OFFSET) /* FTU6 Pair 0 Deadtime Configuration register */
#define FC7300_FTU6_PAIR1DEADTIME       (FC7300_FTU6_BASE + FC7300_FTU_PAIRDEADTIME1_OFFSET) /* FTU6 Pair 1 Deadtime Configuration register */
#define FC7300_FTU6_PAIR2DEADTIME       (FC7300_FTU6_BASE + FC7300_FTU_PAIRDEADTIME2_OFFSET) /* FTU6 Pair 2 Deadtime Configuration register */
#define FC7300_FTU6_PAIR3DEADTIME       (FC7300_FTU6_BASE + FC7300_FTU_PAIRDEADTIME3_OFFSET) /* FTU6 Pair 3 Deadtime Configuration register oset */

/* FTU7 registers */

#define FC7300_FTU7_SC                  (FC7300_FTU7_BASE + FC7300_FTU_SC_OFFSET)            /* FTU7 Status And Control register */
#define FC7300_FTU7_CNT                 (FC7300_FTU7_BASE + FC7300_FTU_CNT_OFFSET)           /* FTU7 Counter register */
#define FC7300_FTU7_MOD                 (FC7300_FTU7_BASE + FC7300_FTU_MOD_OFFSET)           /* FTU7 Modulo register */
#define FC7300_FTU7_C0SC                (FC7300_FTU7_BASE + FC7300_FTU_C0SC_OFFSET)          /* FTU7 Channel 0 Status And Control register */
#define FC7300_FTU7_C0V                 (FC7300_FTU7_BASE + FC7300_FTU_C0V_OFFSET)           /* FTU7 Channel 0 Value register */
#define FC7300_FTU7_C1SC                (FC7300_FTU7_BASE + FC7300_FTU_C1SC_OFFSET)          /* FTU7 Channel 1 Status And Control register */
#define FC7300_FTU7_C1V                 (FC7300_FTU7_BASE + FC7300_FTU_C1V_OFFSET)           /* FTU7 Channel 1 Value register */
#define FC7300_FTU7_C2SC                (FC7300_FTU7_BASE + FC7300_FTU_C2SC_OFFSET)          /* FTU7 Channel 2 Status And Control register */
#define FC7300_FTU7_C2V                 (FC7300_FTU7_BASE + FC7300_FTU_C2V_OFFSET)           /* FTU7 Channel 2 Value register */
#define FC7300_FTU7_C3SC                (FC7300_FTU7_BASE + FC7300_FTU_C3SC_OFFSET)          /* FTU7 Channel 3 Status And Control register */
#define FC7300_FTU7_C3V                 (FC7300_FTU7_BASE + FC7300_FTU_C3V_OFFSET)           /* FTU7 Channel 3 Value register */
#define FC7300_FTU7_C4SC                (FC7300_FTU7_BASE + FC7300_FTU_C4SC_OFFSET)          /* FTU7 Channel 4 Status And Control register */
#define FC7300_FTU7_C4V                 (FC7300_FTU7_BASE + FC7300_FTU_C4V_OFFSET)           /* FTU7 Channel 4 Value register */
#define FC7300_FTU7_C5SC                (FC7300_FTU7_BASE + FC7300_FTU_C5SC_OFFSET)          /* FTU7 Channel 5 Status And Control register */
#define FC7300_FTU7_C5V                 (FC7300_FTU7_BASE + FC7300_FTU_C5V_OFFSET)           /* FTU7 Channel 5 Value register */
#define FC7300_FTU7_C6SC                (FC7300_FTU7_BASE + FC7300_FTU_C6SC_OFFSET)          /* FTU7 Channel 6 Status And Control register */
#define FC7300_FTU7_C6V                 (FC7300_FTU7_BASE + FC7300_FTU_C6V_OFFSET)           /* FTU7 Channel 6 Value register */
#define FC7300_FTU7_C7SC                (FC7300_FTU7_BASE + FC7300_FTU_C7SC_OFFSET)          /* FTU7 Channel 7 Status And Control register */
#define FC7300_FTU7_C7V                 (FC7300_FTU7_BASE + FC7300_FTU_C7V_OFFSET)           /* FTU7 Channel 7 Value register */
#define FC7300_FTU7_CNTIN               (FC7300_FTU7_BASE + FC7300_FTU_CNTIN_OFFSET)         /* FTU7 Counter Initial Value register */
#define FC7300_FTU7_STATUS              (FC7300_FTU7_BASE + FC7300_FTU_STATUS_OFFSET)        /* FTU7 Capture And Compare Status register */
#define FC7300_FTU7_MODE                (FC7300_FTU7_BASE + FC7300_FTU_MODE_OFFSET)          /* FTU7 Features Mode Selection register */
#define FC7300_FTU7_SYNC                (FC7300_FTU7_BASE + FC7300_FTU_SYNC_OFFSET)          /* FTU7 Synchronization register */
#define FC7300_FTU7_OUTINIT             (FC7300_FTU7_BASE + FC7300_FTU_OUTINIT_OFFSET)       /* FTU7 Initial State For Channels Output register */
#define FC7300_FTU7_OUTMASK             (FC7300_FTU7_BASE + FC7300_FTU_OUTMASK_OFFSET)       /* FTU7 Output Mask register */
#define FC7300_FTU7_CHCTRL              (FC7300_FTU7_BASE + FC7300_FTU_CHCTRL_OFFSET)        /* FTU7 Channel control register offset */
#define FC7300_FTU7_DEADTIME            (FC7300_FTU7_BASE + FC7300_FTU_DEADTIME_OFFSET)      /* FTU7 Deadtime Configuration register */
#define FC7300_FTU7_TRIGCONF            (FC7300_FTU7_BASE + FC7300_FTU_TRIGCONF_OFFSET)      /* FTU7 External Trigger register */
#define FC7300_FTU7_POL                 (FC7300_FTU7_BASE + FC7300_FTU_POL_OFFSET)           /* FTU7 Channel Polarity register */
#define FC7300_FTU7_FMS                 (FC7300_FTU7_BASE + FC7300_FTU_FMS_OFFSET)           /* FTU7 Fault Mode Status register */
#define FC7300_FTU7_FLTER               (FC7300_FTU7_BASE + FC7300_FTU_FILTER_OFFSET)        /* FTU7 Input Capture Filter Control register */
#define FC7300_FTU7_FLTCTRL             (FC7300_FTU7_BASE + FC7300_FTU_FLTCTRL_OFFSET)       /* FTU7 Fault Control register */
#define FC7300_FTU7_QDCTRL              (FC7300_FTU7_BASE + FC7300_FTU_QDCTRL_OFFSET)        /* FTU7 Quadrature Decoder Control And Status register */
#define FC7300_FTU7_CONF                (FC7300_FTU7_BASE + FC7300_FTU_CONF_OFFSET)          /* FTU7 Configuration register */
#define FC7300_FTU7_FLTPOL              (FC7300_FTU7_BASE + FC7300_FTU_FLTPOL_OFFSET)        /* FTU7 Fault Input Polarity register */
#define FC7300_FTU7_SYNCONF             (FC7300_FTU7_BASE + FC7300_FTU_SYNCONF_OFFSET)       /* FTU7 Synchronization Configuration register */
#define FC7300_FTU7_INVCTRL             (FC7300_FTU7_BASE + FC7300_FTU_INVCTRL_OFFSET)       /* FTU7 Inverting Control register */
#define FC7300_FTU7_SWOCTRL             (FC7300_FTU7_BASE + FC7300_FTU_SWOCTRL_OFFSET)       /* FTU7 Software Output Control register */
#define FC7300_FTU7_PWMLOAD             (FC7300_FTU7_BASE + FC7300_FTU_PWMLOAD_OFFSET)       /* FTU7 PWM Load register */

#define FC7300_FTU7_PAIR0DEADTIME       (FC7300_FTU7_BASE + FC7300_FTU_PAIRDEADTIME0_OFFSET) /* FTU7 Pair 0 Deadtime Configuration register */
#define FC7300_FTU7_PAIR1DEADTIME       (FC7300_FTU7_BASE + FC7300_FTU_PAIRDEADTIME1_OFFSET) /* FTU7 Pair 1 Deadtime Configuration register */
#define FC7300_FTU7_PAIR2DEADTIME       (FC7300_FTU7_BASE + FC7300_FTU_PAIRDEADTIME2_OFFSET) /* FTU7 Pair 2 Deadtime Configuration register */
#define FC7300_FTU7_PAIR3DEADTIME       (FC7300_FTU7_BASE + FC7300_FTU_PAIRDEADTIME3_OFFSET) /* FTU7 Pair 3 Deadtime Configuration register oset */

/* FTU7 registers */

#define FC7300_FTU7_SC                  (FC7300_FTU7_BASE + FC7300_FTU_SC_OFFSET)            /* FTU7 Status And Control register */
#define FC7300_FTU7_CNT                 (FC7300_FTU7_BASE + FC7300_FTU_CNT_OFFSET)           /* FTU7 Counter register */
#define FC7300_FTU7_MOD                 (FC7300_FTU7_BASE + FC7300_FTU_MOD_OFFSET)           /* FTU7 Modulo register */
#define FC7300_FTU7_C0SC                (FC7300_FTU7_BASE + FC7300_FTU_C0SC_OFFSET)          /* FTU7 Channel 0 Status And Control register */
#define FC7300_FTU7_C0V                 (FC7300_FTU7_BASE + FC7300_FTU_C0V_OFFSET)           /* FTU7 Channel 0 Value register */
#define FC7300_FTU7_C1SC                (FC7300_FTU7_BASE + FC7300_FTU_C1SC_OFFSET)          /* FTU7 Channel 1 Status And Control register */
#define FC7300_FTU7_C1V                 (FC7300_FTU7_BASE + FC7300_FTU_C1V_OFFSET)           /* FTU7 Channel 1 Value register */
#define FC7300_FTU7_C2SC                (FC7300_FTU7_BASE + FC7300_FTU_C2SC_OFFSET)          /* FTU7 Channel 2 Status And Control register */
#define FC7300_FTU7_C2V                 (FC7300_FTU7_BASE + FC7300_FTU_C2V_OFFSET)           /* FTU7 Channel 2 Value register */
#define FC7300_FTU7_C3SC                (FC7300_FTU7_BASE + FC7300_FTU_C3SC_OFFSET)          /* FTU7 Channel 3 Status And Control register */
#define FC7300_FTU7_C3V                 (FC7300_FTU7_BASE + FC7300_FTU_C3V_OFFSET)           /* FTU7 Channel 3 Value register */
#define FC7300_FTU7_C4SC                (FC7300_FTU7_BASE + FC7300_FTU_C4SC_OFFSET)          /* FTU7 Channel 4 Status And Control register */
#define FC7300_FTU7_C4V                 (FC7300_FTU7_BASE + FC7300_FTU_C4V_OFFSET)           /* FTU7 Channel 4 Value register */
#define FC7300_FTU7_C5SC                (FC7300_FTU7_BASE + FC7300_FTU_C5SC_OFFSET)          /* FTU7 Channel 5 Status And Control register */
#define FC7300_FTU7_C5V                 (FC7300_FTU7_BASE + FC7300_FTU_C5V_OFFSET)           /* FTU7 Channel 5 Value register */
#define FC7300_FTU7_C6SC                (FC7300_FTU7_BASE + FC7300_FTU_C6SC_OFFSET)          /* FTU7 Channel 6 Status And Control register */
#define FC7300_FTU7_C6V                 (FC7300_FTU7_BASE + FC7300_FTU_C6V_OFFSET)           /* FTU7 Channel 6 Value register */
#define FC7300_FTU7_C7SC                (FC7300_FTU7_BASE + FC7300_FTU_C7SC_OFFSET)          /* FTU7 Channel 7 Status And Control register */
#define FC7300_FTU7_C7V                 (FC7300_FTU7_BASE + FC7300_FTU_C7V_OFFSET)           /* FTU7 Channel 7 Value register */
#define FC7300_FTU7_CNTIN               (FC7300_FTU7_BASE + FC7300_FTU_CNTIN_OFFSET)         /* FTU7 Counter Initial Value register */
#define FC7300_FTU7_STATUS              (FC7300_FTU7_BASE + FC7300_FTU_STATUS_OFFSET)        /* FTU7 Capture And Compare Status register */
#define FC7300_FTU7_MODE                (FC7300_FTU7_BASE + FC7300_FTU_MODE_OFFSET)          /* FTU7 Features Mode Selection register */
#define FC7300_FTU7_SYNC                (FC7300_FTU7_BASE + FC7300_FTU_SYNC_OFFSET)          /* FTU7 Synchronization register */
#define FC7300_FTU7_OUTINIT             (FC7300_FTU7_BASE + FC7300_FTU_OUTINIT_OFFSET)       /* FTU7 Initial State For Channels Output register */
#define FC7300_FTU7_OUTMASK             (FC7300_FTU7_BASE + FC7300_FTU_OUTMASK_OFFSET)       /* FTU7 Output Mask register */
#define FC7300_FTU7_CHCTRL              (FC7300_FTU7_BASE + FC7300_FTU_CHCTRL_OFFSET)        /* FTU7 Channel control register offset */
#define FC7300_FTU7_DEADTIME            (FC7300_FTU7_BASE + FC7300_FTU_DEADTIME_OFFSET)      /* FTU7 Deadtime Configuration register */
#define FC7300_FTU7_TRIGCONF            (FC7300_FTU7_BASE + FC7300_FTU_TRIGCONF_OFFSET)      /* FTU7 External Trigger register */
#define FC7300_FTU7_POL                 (FC7300_FTU7_BASE + FC7300_FTU_POL_OFFSET)           /* FTU7 Channel Polarity register */
#define FC7300_FTU7_FMS                 (FC7300_FTU7_BASE + FC7300_FTU_FMS_OFFSET)           /* FTU7 Fault Mode Status register */
#define FC7300_FTU7_FLTER               (FC7300_FTU7_BASE + FC7300_FTU_FILTER_OFFSET)        /* FTU7 Input Capture Filter Control register */
#define FC7300_FTU7_FLTCTRL             (FC7300_FTU7_BASE + FC7300_FTU_FLTCTRL_OFFSET)       /* FTU7 Fault Control register */
#define FC7300_FTU7_QDCTRL              (FC7300_FTU7_BASE + FC7300_FTU_QDCTRL_OFFSET)        /* FTU7 Quadrature Decoder Control And Status register */
#define FC7300_FTU7_CONF                (FC7300_FTU7_BASE + FC7300_FTU_CONF_OFFSET)          /* FTU7 Configuration register */
#define FC7300_FTU7_FLTPOL              (FC7300_FTU7_BASE + FC7300_FTU_FLTPOL_OFFSET)        /* FTU7 Fault Input Polarity register */
#define FC7300_FTU7_SYNCONF             (FC7300_FTU7_BASE + FC7300_FTU_SYNCONF_OFFSET)       /* FTU7 Synchronization Configuration register */
#define FC7300_FTU7_INVCTRL             (FC7300_FTU7_BASE + FC7300_FTU_INVCTRL_OFFSET)       /* FTU7 Inverting Control register */
#define FC7300_FTU7_SWOCTRL             (FC7300_FTU7_BASE + FC7300_FTU_SWOCTRL_OFFSET)       /* FTU7 Software Output Control register */
#define FC7300_FTU7_PWMLOAD             (FC7300_FTU7_BASE + FC7300_FTU_PWMLOAD_OFFSET)       /* FTU7 PWM Load register */

#define FC7300_FTU7_PAIR0DEADTIME       (FC7300_FTU7_BASE + FC7300_FTU_PAIRDEADTIME0_OFFSET) /* FTU7 Pair 0 Deadtime Configuration register */
#define FC7300_FTU7_PAIR1DEADTIME       (FC7300_FTU7_BASE + FC7300_FTU_PAIRDEADTIME1_OFFSET) /* FTU7 Pair 1 Deadtime Configuration register */
#define FC7300_FTU7_PAIR2DEADTIME       (FC7300_FTU7_BASE + FC7300_FTU_PAIRDEADTIME2_OFFSET) /* FTU7 Pair 2 Deadtime Configuration register */
#define FC7300_FTU7_PAIR3DEADTIME       (FC7300_FTU7_BASE + FC7300_FTU_PAIRDEADTIME3_OFFSET) /* FTU7 Pair 3 Deadtime Configuration register oset */

/* FTU8 registers */

#define FC7300_FTU8_SC                  (FC7300_FTU8_BASE + FC7300_FTU_SC_OFFSET)            /* FTU8 Status And Control register */
#define FC7300_FTU8_CNT                 (FC7300_FTU8_BASE + FC7300_FTU_CNT_OFFSET)           /* FTU8 Counter register */
#define FC7300_FTU8_MOD                 (FC7300_FTU8_BASE + FC7300_FTU_MOD_OFFSET)           /* FTU8 Modulo register */
#define FC7300_FTU8_C0SC                (FC7300_FTU8_BASE + FC7300_FTU_C0SC_OFFSET)          /* FTU8 Channel 0 Status And Control register */
#define FC7300_FTU8_C0V                 (FC7300_FTU8_BASE + FC7300_FTU_C0V_OFFSET)           /* FTU8 Channel 0 Value register */
#define FC7300_FTU8_C1SC                (FC7300_FTU8_BASE + FC7300_FTU_C1SC_OFFSET)          /* FTU8 Channel 1 Status And Control register */
#define FC7300_FTU8_C1V                 (FC7300_FTU8_BASE + FC7300_FTU_C1V_OFFSET)           /* FTU8 Channel 1 Value register */
#define FC7300_FTU8_C2SC                (FC7300_FTU8_BASE + FC7300_FTU_C2SC_OFFSET)          /* FTU8 Channel 2 Status And Control register */
#define FC7300_FTU8_C2V                 (FC7300_FTU8_BASE + FC7300_FTU_C2V_OFFSET)           /* FTU8 Channel 2 Value register */
#define FC7300_FTU8_C3SC                (FC7300_FTU8_BASE + FC7300_FTU_C3SC_OFFSET)          /* FTU8 Channel 3 Status And Control register */
#define FC7300_FTU8_C3V                 (FC7300_FTU8_BASE + FC7300_FTU_C3V_OFFSET)           /* FTU8 Channel 3 Value register */
#define FC7300_FTU8_C4SC                (FC7300_FTU8_BASE + FC7300_FTU_C4SC_OFFSET)          /* FTU8 Channel 4 Status And Control register */
#define FC7300_FTU8_C4V                 (FC7300_FTU8_BASE + FC7300_FTU_C4V_OFFSET)           /* FTU8 Channel 4 Value register */
#define FC7300_FTU8_C5SC                (FC7300_FTU8_BASE + FC7300_FTU_C5SC_OFFSET)          /* FTU8 Channel 5 Status And Control register */
#define FC7300_FTU8_C5V                 (FC7300_FTU8_BASE + FC7300_FTU_C5V_OFFSET)           /* FTU8 Channel 5 Value register */
#define FC7300_FTU8_C6SC                (FC7300_FTU8_BASE + FC7300_FTU_C6SC_OFFSET)          /* FTU8 Channel 6 Status And Control register */
#define FC7300_FTU8_C6V                 (FC7300_FTU8_BASE + FC7300_FTU_C6V_OFFSET)           /* FTU8 Channel 6 Value register */
#define FC7300_FTU8_C7SC                (FC7300_FTU8_BASE + FC7300_FTU_C7SC_OFFSET)          /* FTU8 Channel 7 Status And Control register */
#define FC7300_FTU8_C7V                 (FC7300_FTU8_BASE + FC7300_FTU_C7V_OFFSET)           /* FTU8 Channel 7 Value register */
#define FC7300_FTU8_CNTIN               (FC7300_FTU8_BASE + FC7300_FTU_CNTIN_OFFSET)         /* FTU8 Counter Initial Value register */
#define FC7300_FTU8_STATUS              (FC7300_FTU8_BASE + FC7300_FTU_STATUS_OFFSET)        /* FTU8 Capture And Compare Status register */
#define FC7300_FTU8_MODE                (FC7300_FTU8_BASE + FC7300_FTU_MODE_OFFSET)          /* FTU8 Features Mode Selection register */
#define FC7300_FTU8_SYNC                (FC7300_FTU8_BASE + FC7300_FTU_SYNC_OFFSET)          /* FTU8 Synchronization register */
#define FC7300_FTU8_OUTINIT             (FC7300_FTU8_BASE + FC7300_FTU_OUTINIT_OFFSET)       /* FTU8 Initial State For Channels Output register */
#define FC7300_FTU8_OUTMASK             (FC7300_FTU8_BASE + FC7300_FTU_OUTMASK_OFFSET)       /* FTU8 Output Mask register */
#define FC7300_FTU8_CHCTRL              (FC7300_FTU8_BASE + FC7300_FTU_CHCTRL_OFFSET)        /* FTU8 Channel control register offset */
#define FC7300_FTU8_DEADTIME            (FC7300_FTU8_BASE + FC7300_FTU_DEADTIME_OFFSET)      /* FTU8 Deadtime Configuration register */
#define FC7300_FTU8_TRIGCONF            (FC7300_FTU8_BASE + FC7300_FTU_TRIGCONF_OFFSET)      /* FTU8 External Trigger register */
#define FC7300_FTU8_POL                 (FC7300_FTU8_BASE + FC7300_FTU_POL_OFFSET)           /* FTU8 Channel Polarity register */
#define FC7300_FTU8_FMS                 (FC7300_FTU8_BASE + FC7300_FTU_FMS_OFFSET)           /* FTU8 Fault Mode Status register */
#define FC7300_FTU8_FLTER               (FC7300_FTU8_BASE + FC7300_FTU_FILTER_OFFSET)        /* FTU8 Input Capture Filter Control register */
#define FC7300_FTU8_FLTCTRL             (FC7300_FTU8_BASE + FC7300_FTU_FLTCTRL_OFFSET)       /* FTU8 Fault Control register */
#define FC7300_FTU8_QDCTRL              (FC7300_FTU8_BASE + FC7300_FTU_QDCTRL_OFFSET)        /* FTU8 Quadrature Decoder Control And Status register */
#define FC7300_FTU8_CONF                (FC7300_FTU8_BASE + FC7300_FTU_CONF_OFFSET)          /* FTU8 Configuration register */
#define FC7300_FTU8_FLTPOL              (FC7300_FTU8_BASE + FC7300_FTU_FLTPOL_OFFSET)        /* FTU8 Fault Input Polarity register */
#define FC7300_FTU8_SYNCONF             (FC7300_FTU8_BASE + FC7300_FTU_SYNCONF_OFFSET)       /* FTU8 Synchronization Configuration register */
#define FC7300_FTU8_INVCTRL             (FC7300_FTU8_BASE + FC7300_FTU_INVCTRL_OFFSET)       /* FTU8 Inverting Control register */
#define FC7300_FTU8_SWOCTRL             (FC7300_FTU8_BASE + FC7300_FTU_SWOCTRL_OFFSET)       /* FTU8 Software Output Control register */
#define FC7300_FTU8_PWMLOAD             (FC7300_FTU8_BASE + FC7300_FTU_PWMLOAD_OFFSET)       /* FTU8 PWM Load register */

#define FC7300_FTU8_PAIR0DEADTIME       (FC7300_FTU8_BASE + FC7300_FTU_PAIRDEADTIME0_OFFSET) /* FTU8 Pair 0 Deadtime Configuration register */
#define FC7300_FTU8_PAIR1DEADTIME       (FC7300_FTU8_BASE + FC7300_FTU_PAIRDEADTIME1_OFFSET) /* FTU8 Pair 1 Deadtime Configuration register */
#define FC7300_FTU8_PAIR2DEADTIME       (FC7300_FTU8_BASE + FC7300_FTU_PAIRDEADTIME2_OFFSET) /* FTU8 Pair 2 Deadtime Configuration register */
#define FC7300_FTU8_PAIR3DEADTIME       (FC7300_FTU8_BASE + FC7300_FTU_PAIRDEADTIME3_OFFSET) /* FTU8 Pair 3 Deadtime Configuration register oset */

/* FTU9 registers */

#define FC7300_FTU9_SC                  (FC7300_FTU9_BASE + FC7300_FTU_SC_OFFSET)            /* FTU9 Status And Control register */
#define FC7300_FTU9_CNT                 (FC7300_FTU9_BASE + FC7300_FTU_CNT_OFFSET)           /* FTU9 Counter register */
#define FC7300_FTU9_MOD                 (FC7300_FTU9_BASE + FC7300_FTU_MOD_OFFSET)           /* FTU9 Modulo register */
#define FC7300_FTU9_C0SC                (FC7300_FTU9_BASE + FC7300_FTU_C0SC_OFFSET)          /* FTU9 Channel 0 Status And Control register */
#define FC7300_FTU9_C0V                 (FC7300_FTU9_BASE + FC7300_FTU_C0V_OFFSET)           /* FTU9 Channel 0 Value register */
#define FC7300_FTU9_C1SC                (FC7300_FTU9_BASE + FC7300_FTU_C1SC_OFFSET)          /* FTU9 Channel 1 Status And Control register */
#define FC7300_FTU9_C1V                 (FC7300_FTU9_BASE + FC7300_FTU_C1V_OFFSET)           /* FTU9 Channel 1 Value register */
#define FC7300_FTU9_C2SC                (FC7300_FTU9_BASE + FC7300_FTU_C2SC_OFFSET)          /* FTU9 Channel 2 Status And Control register */
#define FC7300_FTU9_C2V                 (FC7300_FTU9_BASE + FC7300_FTU_C2V_OFFSET)           /* FTU9 Channel 2 Value register */
#define FC7300_FTU9_C3SC                (FC7300_FTU9_BASE + FC7300_FTU_C3SC_OFFSET)          /* FTU9 Channel 3 Status And Control register */
#define FC7300_FTU9_C3V                 (FC7300_FTU9_BASE + FC7300_FTU_C3V_OFFSET)           /* FTU9 Channel 3 Value register */
#define FC7300_FTU9_C4SC                (FC7300_FTU9_BASE + FC7300_FTU_C4SC_OFFSET)          /* FTU9 Channel 4 Status And Control register */
#define FC7300_FTU9_C4V                 (FC7300_FTU9_BASE + FC7300_FTU_C4V_OFFSET)           /* FTU9 Channel 4 Value register */
#define FC7300_FTU9_C5SC                (FC7300_FTU9_BASE + FC7300_FTU_C5SC_OFFSET)          /* FTU9 Channel 5 Status And Control register */
#define FC7300_FTU9_C5V                 (FC7300_FTU9_BASE + FC7300_FTU_C5V_OFFSET)           /* FTU9 Channel 5 Value register */
#define FC7300_FTU9_C6SC                (FC7300_FTU9_BASE + FC7300_FTU_C6SC_OFFSET)          /* FTU9 Channel 6 Status And Control register */
#define FC7300_FTU9_C6V                 (FC7300_FTU9_BASE + FC7300_FTU_C6V_OFFSET)           /* FTU9 Channel 6 Value register */
#define FC7300_FTU9_C7SC                (FC7300_FTU9_BASE + FC7300_FTU_C7SC_OFFSET)          /* FTU9 Channel 7 Status And Control register */
#define FC7300_FTU9_C7V                 (FC7300_FTU9_BASE + FC7300_FTU_C7V_OFFSET)           /* FTU9 Channel 7 Value register */
#define FC7300_FTU9_CNTIN               (FC7300_FTU9_BASE + FC7300_FTU_CNTIN_OFFSET)         /* FTU9 Counter Initial Value register */
#define FC7300_FTU9_STATUS              (FC7300_FTU9_BASE + FC7300_FTU_STATUS_OFFSET)        /* FTU9 Capture And Compare Status register */
#define FC7300_FTU9_MODE                (FC7300_FTU9_BASE + FC7300_FTU_MODE_OFFSET)          /* FTU9 Features Mode Selection register */
#define FC7300_FTU9_SYNC                (FC7300_FTU9_BASE + FC7300_FTU_SYNC_OFFSET)          /* FTU9 Synchronization register */
#define FC7300_FTU9_OUTINIT             (FC7300_FTU9_BASE + FC7300_FTU_OUTINIT_OFFSET)       /* FTU9 Initial State For Channels Output register */
#define FC7300_FTU9_OUTMASK             (FC7300_FTU9_BASE + FC7300_FTU_OUTMASK_OFFSET)       /* FTU9 Output Mask register */
#define FC7300_FTU9_CHCTRL              (FC7300_FTU9_BASE + FC7300_FTU_CHCTRL_OFFSET)        /* FTU9 Channel control register offset */
#define FC7300_FTU9_DEADTIME            (FC7300_FTU9_BASE + FC7300_FTU_DEADTIME_OFFSET)      /* FTU9 Deadtime Configuration register */
#define FC7300_FTU9_TRIGCONF            (FC7300_FTU9_BASE + FC7300_FTU_TRIGCONF_OFFSET)      /* FTU9 External Trigger register */
#define FC7300_FTU9_POL                 (FC7300_FTU9_BASE + FC7300_FTU_POL_OFFSET)           /* FTU9 Channel Polarity register */
#define FC7300_FTU9_FMS                 (FC7300_FTU9_BASE + FC7300_FTU_FMS_OFFSET)           /* FTU9 Fault Mode Status register */
#define FC7300_FTU9_FLTER               (FC7300_FTU9_BASE + FC7300_FTU_FILTER_OFFSET)        /* FTU9 Input Capture Filter Control register */
#define FC7300_FTU9_FLTCTRL             (FC7300_FTU9_BASE + FC7300_FTU_FLTCTRL_OFFSET)       /* FTU9 Fault Control register */
#define FC7300_FTU9_QDCTRL              (FC7300_FTU9_BASE + FC7300_FTU_QDCTRL_OFFSET)        /* FTU9 Quadrature Decoder Control And Status register */
#define FC7300_FTU9_CONF                (FC7300_FTU9_BASE + FC7300_FTU_CONF_OFFSET)          /* FTU9 Configuration register */
#define FC7300_FTU9_FLTPOL              (FC7300_FTU9_BASE + FC7300_FTU_FLTPOL_OFFSET)        /* FTU9 Fault Input Polarity register */
#define FC7300_FTU9_SYNCONF             (FC7300_FTU9_BASE + FC7300_FTU_SYNCONF_OFFSET)       /* FTU9 Synchronization Configuration register */
#define FC7300_FTU9_INVCTRL             (FC7300_FTU9_BASE + FC7300_FTU_INVCTRL_OFFSET)       /* FTU9 Inverting Control register */
#define FC7300_FTU9_SWOCTRL             (FC7300_FTU9_BASE + FC7300_FTU_SWOCTRL_OFFSET)       /* FTU9 Software Output Control register */
#define FC7300_FTU9_PWMLOAD             (FC7300_FTU9_BASE + FC7300_FTU_PWMLOAD_OFFSET)       /* FTU9 PWM Load register */

#define FC7300_FTU9_PAIR0DEADTIME       (FC7300_FTU9_BASE + FC7300_FTU_PAIRDEADTIME0_OFFSET) /* FTU9 Pair 0 Deadtime Configuration register */
#define FC7300_FTU9_PAIR1DEADTIME       (FC7300_FTU9_BASE + FC7300_FTU_PAIRDEADTIME1_OFFSET) /* FTU9 Pair 1 Deadtime Configuration register */
#define FC7300_FTU9_PAIR2DEADTIME       (FC7300_FTU9_BASE + FC7300_FTU_PAIRDEADTIME2_OFFSET) /* FTU9 Pair 2 Deadtime Configuration register */
#define FC7300_FTU9_PAIR3DEADTIME       (FC7300_FTU9_BASE + FC7300_FTU_PAIRDEADTIME3_OFFSET) /* FTU9 Pair 3 Deadtime Configuration register oset */

/* FTU10 registers */

#define FC7300_FTU10_SC                  (FC7300_FTU10_BASE + FC7300_FTU_SC_OFFSET)            /* FTU10 Status And Control register */
#define FC7300_FTU10_CNT                 (FC7300_FTU10_BASE + FC7300_FTU_CNT_OFFSET)           /* FTU10 Counter register */
#define FC7300_FTU10_MOD                 (FC7300_FTU10_BASE + FC7300_FTU_MOD_OFFSET)           /* FTU10 Modulo register */
#define FC7300_FTU10_C0SC                (FC7300_FTU10_BASE + FC7300_FTU_C0SC_OFFSET)          /* FTU10 Channel 0 Status And Control register */
#define FC7300_FTU10_C0V                 (FC7300_FTU10_BASE + FC7300_FTU_C0V_OFFSET)           /* FTU10 Channel 0 Value register */
#define FC7300_FTU10_C1SC                (FC7300_FTU10_BASE + FC7300_FTU_C1SC_OFFSET)          /* FTU10 Channel 1 Status And Control register */
#define FC7300_FTU10_C1V                 (FC7300_FTU10_BASE + FC7300_FTU_C1V_OFFSET)           /* FTU10 Channel 1 Value register */
#define FC7300_FTU10_C2SC                (FC7300_FTU10_BASE + FC7300_FTU_C2SC_OFFSET)          /* FTU10 Channel 2 Status And Control register */
#define FC7300_FTU10_C2V                 (FC7300_FTU10_BASE + FC7300_FTU_C2V_OFFSET)           /* FTU10 Channel 2 Value register */
#define FC7300_FTU10_C3SC                (FC7300_FTU10_BASE + FC7300_FTU_C3SC_OFFSET)          /* FTU10 Channel 3 Status And Control register */
#define FC7300_FTU10_C3V                 (FC7300_FTU10_BASE + FC7300_FTU_C3V_OFFSET)           /* FTU10 Channel 3 Value register */
#define FC7300_FTU10_C4SC                (FC7300_FTU10_BASE + FC7300_FTU_C4SC_OFFSET)          /* FTU10 Channel 4 Status And Control register */
#define FC7300_FTU10_C4V                 (FC7300_FTU10_BASE + FC7300_FTU_C4V_OFFSET)           /* FTU10 Channel 4 Value register */
#define FC7300_FTU10_C5SC                (FC7300_FTU10_BASE + FC7300_FTU_C5SC_OFFSET)          /* FTU10 Channel 5 Status And Control register */
#define FC7300_FTU10_C5V                 (FC7300_FTU10_BASE + FC7300_FTU_C5V_OFFSET)           /* FTU10 Channel 5 Value register */
#define FC7300_FTU10_C6SC                (FC7300_FTU10_BASE + FC7300_FTU_C6SC_OFFSET)          /* FTU10 Channel 6 Status And Control register */
#define FC7300_FTU10_C6V                 (FC7300_FTU10_BASE + FC7300_FTU_C6V_OFFSET)           /* FTU10 Channel 6 Value register */
#define FC7300_FTU10_C7SC                (FC7300_FTU10_BASE + FC7300_FTU_C7SC_OFFSET)          /* FTU10 Channel 7 Status And Control register */
#define FC7300_FTU10_C7V                 (FC7300_FTU10_BASE + FC7300_FTU_C7V_OFFSET)           /* FTU10 Channel 7 Value register */
#define FC7300_FTU10_CNTIN               (FC7300_FTU10_BASE + FC7300_FTU_CNTIN_OFFSET)         /* FTU10 Counter Initial Value register */
#define FC7300_FTU10_STATUS              (FC7300_FTU10_BASE + FC7300_FTU_STATUS_OFFSET)        /* FTU10 Capture And Compare Status register */
#define FC7300_FTU10_MODE                (FC7300_FTU10_BASE + FC7300_FTU_MODE_OFFSET)          /* FTU10 Features Mode Selection register */
#define FC7300_FTU10_SYNC                (FC7300_FTU10_BASE + FC7300_FTU_SYNC_OFFSET)          /* FTU10 Synchronization register */
#define FC7300_FTU10_OUTINIT             (FC7300_FTU10_BASE + FC7300_FTU_OUTINIT_OFFSET)       /* FTU10 Initial State For Channels Output register */
#define FC7300_FTU10_OUTMASK             (FC7300_FTU10_BASE + FC7300_FTU_OUTMASK_OFFSET)       /* FTU10 Output Mask register */
#define FC7300_FTU10_CHCTRL              (FC7300_FTU10_BASE + FC7300_FTU_CHCTRL_OFFSET)        /* FTU10 Channel control register offset */
#define FC7300_FTU10_DEADTIME            (FC7300_FTU10_BASE + FC7300_FTU_DEADTIME_OFFSET)      /* FTU10 Deadtime Configuration register */
#define FC7300_FTU10_TRIGCONF            (FC7300_FTU10_BASE + FC7300_FTU_TRIGCONF_OFFSET)      /* FTU10 External Trigger register */
#define FC7300_FTU10_POL                 (FC7300_FTU10_BASE + FC7300_FTU_POL_OFFSET)           /* FTU10 Channel Polarity register */
#define FC7300_FTU10_FMS                 (FC7300_FTU10_BASE + FC7300_FTU_FMS_OFFSET)           /* FTU10 Fault Mode Status register */
#define FC7300_FTU10_FLTER               (FC7300_FTU10_BASE + FC7300_FTU_FILTER_OFFSET)        /* FTU10 Input Capture Filter Control register */
#define FC7300_FTU10_FLTCTRL             (FC7300_FTU10_BASE + FC7300_FTU_FLTCTRL_OFFSET)       /* FTU10 Fault Control register */
#define FC7300_FTU10_QDCTRL              (FC7300_FTU10_BASE + FC7300_FTU_QDCTRL_OFFSET)        /* FTU10 Quadrature Decoder Control And Status register */
#define FC7300_FTU10_CONF                (FC7300_FTU10_BASE + FC7300_FTU_CONF_OFFSET)          /* FTU10 Configuration register */
#define FC7300_FTU10_FLTPOL              (FC7300_FTU10_BASE + FC7300_FTU_FLTPOL_OFFSET)        /* FTU10 Fault Input Polarity register */
#define FC7300_FTU10_SYNCONF             (FC7300_FTU10_BASE + FC7300_FTU_SYNCONF_OFFSET)       /* FTU10 Synchronization Configuration register */
#define FC7300_FTU10_INVCTRL             (FC7300_FTU10_BASE + FC7300_FTU_INVCTRL_OFFSET)       /* FTU10 Inverting Control register */
#define FC7300_FTU10_SWOCTRL             (FC7300_FTU10_BASE + FC7300_FTU_SWOCTRL_OFFSET)       /* FTU10 Software Output Control register */
#define FC7300_FTU10_PWMLOAD             (FC7300_FTU10_BASE + FC7300_FTU_PWMLOAD_OFFSET)       /* FTU10 PWM Load register */

#define FC7300_FTU10_PAIR0DEADTIME       (FC7300_FTU10_BASE + FC7300_FTU_PAIRDEADTIME0_OFFSET) /* FTU10 Pair 0 Deadtime Configuration register */
#define FC7300_FTU10_PAIR1DEADTIME       (FC7300_FTU10_BASE + FC7300_FTU_PAIRDEADTIME1_OFFSET) /* FTU10 Pair 1 Deadtime Configuration register */
#define FC7300_FTU10_PAIR2DEADTIME       (FC7300_FTU10_BASE + FC7300_FTU_PAIRDEADTIME2_OFFSET) /* FTU10 Pair 2 Deadtime Configuration register */
#define FC7300_FTU10_PAIR3DEADTIME       (FC7300_FTU10_BASE + FC7300_FTU_PAIRDEADTIME3_OFFSET) /* FTU10 Pair 3 Deadtime Configuration register oset */

/* FTU11 registers */

#define FC7300_FTU11_SC                  (FC7300_FTU11_BASE + FC7300_FTU_SC_OFFSET)            /* FTU11 Status And Control register */
#define FC7300_FTU11_CNT                 (FC7300_FTU11_BASE + FC7300_FTU_CNT_OFFSET)           /* FTU11 Counter register */
#define FC7300_FTU11_MOD                 (FC7300_FTU11_BASE + FC7300_FTU_MOD_OFFSET)           /* FTU11 Modulo register */
#define FC7300_FTU11_C0SC                (FC7300_FTU11_BASE + FC7300_FTU_C0SC_OFFSET)          /* FTU11 Channel 0 Status And Control register */
#define FC7300_FTU11_C0V                 (FC7300_FTU11_BASE + FC7300_FTU_C0V_OFFSET)           /* FTU11 Channel 0 Value register */
#define FC7300_FTU11_C1SC                (FC7300_FTU11_BASE + FC7300_FTU_C1SC_OFFSET)          /* FTU11 Channel 1 Status And Control register */
#define FC7300_FTU11_C1V                 (FC7300_FTU11_BASE + FC7300_FTU_C1V_OFFSET)           /* FTU11 Channel 1 Value register */
#define FC7300_FTU11_C2SC                (FC7300_FTU11_BASE + FC7300_FTU_C2SC_OFFSET)          /* FTU11 Channel 2 Status And Control register */
#define FC7300_FTU11_C2V                 (FC7300_FTU11_BASE + FC7300_FTU_C2V_OFFSET)           /* FTU11 Channel 2 Value register */
#define FC7300_FTU11_C3SC                (FC7300_FTU11_BASE + FC7300_FTU_C3SC_OFFSET)          /* FTU11 Channel 3 Status And Control register */
#define FC7300_FTU11_C3V                 (FC7300_FTU11_BASE + FC7300_FTU_C3V_OFFSET)           /* FTU11 Channel 3 Value register */
#define FC7300_FTU11_C4SC                (FC7300_FTU11_BASE + FC7300_FTU_C4SC_OFFSET)          /* FTU11 Channel 4 Status And Control register */
#define FC7300_FTU11_C4V                 (FC7300_FTU11_BASE + FC7300_FTU_C4V_OFFSET)           /* FTU11 Channel 4 Value register */
#define FC7300_FTU11_C5SC                (FC7300_FTU11_BASE + FC7300_FTU_C5SC_OFFSET)          /* FTU11 Channel 5 Status And Control register */
#define FC7300_FTU11_C5V                 (FC7300_FTU11_BASE + FC7300_FTU_C5V_OFFSET)           /* FTU11 Channel 5 Value register */
#define FC7300_FTU11_C6SC                (FC7300_FTU11_BASE + FC7300_FTU_C6SC_OFFSET)          /* FTU11 Channel 6 Status And Control register */
#define FC7300_FTU11_C6V                 (FC7300_FTU11_BASE + FC7300_FTU_C6V_OFFSET)           /* FTU11 Channel 6 Value register */
#define FC7300_FTU11_C7SC                (FC7300_FTU11_BASE + FC7300_FTU_C7SC_OFFSET)          /* FTU11 Channel 7 Status And Control register */
#define FC7300_FTU11_C7V                 (FC7300_FTU11_BASE + FC7300_FTU_C7V_OFFSET)           /* FTU11 Channel 7 Value register */
#define FC7300_FTU11_CNTIN               (FC7300_FTU11_BASE + FC7300_FTU_CNTIN_OFFSET)         /* FTU11 Counter Initial Value register */
#define FC7300_FTU11_STATUS              (FC7300_FTU11_BASE + FC7300_FTU_STATUS_OFFSET)        /* FTU11 Capture And Compare Status register */
#define FC7300_FTU11_MODE                (FC7300_FTU11_BASE + FC7300_FTU_MODE_OFFSET)          /* FTU11 Features Mode Selection register */
#define FC7300_FTU11_SYNC                (FC7300_FTU11_BASE + FC7300_FTU_SYNC_OFFSET)          /* FTU11 Synchronization register */
#define FC7300_FTU11_OUTINIT             (FC7300_FTU11_BASE + FC7300_FTU_OUTINIT_OFFSET)       /* FTU11 Initial State For Channels Output register */
#define FC7300_FTU11_OUTMASK             (FC7300_FTU11_BASE + FC7300_FTU_OUTMASK_OFFSET)       /* FTU11 Output Mask register */
#define FC7300_FTU11_CHCTRL              (FC7300_FTU11_BASE + FC7300_FTU_CHCTRL_OFFSET)        /* FTU11 Channel control register offset */
#define FC7300_FTU11_DEADTIME            (FC7300_FTU11_BASE + FC7300_FTU_DEADTIME_OFFSET)      /* FTU11 Deadtime Configuration register */
#define FC7300_FTU11_TRIGCONF            (FC7300_FTU11_BASE + FC7300_FTU_TRIGCONF_OFFSET)      /* FTU11 External Trigger register */
#define FC7300_FTU11_POL                 (FC7300_FTU11_BASE + FC7300_FTU_POL_OFFSET)           /* FTU11 Channel Polarity register */
#define FC7300_FTU11_FMS                 (FC7300_FTU11_BASE + FC7300_FTU_FMS_OFFSET)           /* FTU11 Fault Mode Status register */
#define FC7300_FTU11_FLTER               (FC7300_FTU11_BASE + FC7300_FTU_FILTER_OFFSET)        /* FTU11 Input Capture Filter Control register */
#define FC7300_FTU11_FLTCTRL             (FC7300_FTU11_BASE + FC7300_FTU_FLTCTRL_OFFSET)       /* FTU11 Fault Control register */
#define FC7300_FTU11_QDCTRL              (FC7300_FTU11_BASE + FC7300_FTU_QDCTRL_OFFSET)        /* FTU11 Quadrature Decoder Control And Status register */
#define FC7300_FTU11_CONF                (FC7300_FTU11_BASE + FC7300_FTU_CONF_OFFSET)          /* FTU11 Configuration register */
#define FC7300_FTU11_FLTPOL              (FC7300_FTU11_BASE + FC7300_FTU_FLTPOL_OFFSET)        /* FTU11 Fault Input Polarity register */
#define FC7300_FTU11_SYNCONF             (FC7300_FTU11_BASE + FC7300_FTU_SYNCONF_OFFSET)       /* FTU11 Synchronization Configuration register */
#define FC7300_FTU11_INVCTRL             (FC7300_FTU11_BASE + FC7300_FTU_INVCTRL_OFFSET)       /* FTU11 Inverting Control register */
#define FC7300_FTU11_SWOCTRL             (FC7300_FTU11_BASE + FC7300_FTU_SWOCTRL_OFFSET)       /* FTU11 Software Output Control register */
#define FC7300_FTU11_PWMLOAD             (FC7300_FTU11_BASE + FC7300_FTU_PWMLOAD_OFFSET)       /* FTU11 PWM Load register */

#define FC7300_FTU11_PAIR0DEADTIME       (FC7300_FTU11_BASE + FC7300_FTU_PAIRDEADTIME0_OFFSET) /* FTU11 Pair 0 Deadtime Configuration register */
#define FC7300_FTU11_PAIR1DEADTIME       (FC7300_FTU11_BASE + FC7300_FTU_PAIRDEADTIME1_OFFSET) /* FTU11 Pair 1 Deadtime Configuration register */
#define FC7300_FTU11_PAIR2DEADTIME       (FC7300_FTU11_BASE + FC7300_FTU_PAIRDEADTIME2_OFFSET) /* FTU11 Pair 2 Deadtime Configuration register */
#define FC7300_FTU11_PAIR3DEADTIME       (FC7300_FTU11_BASE + FC7300_FTU_PAIRDEADTIME3_OFFSET) /* FTU11 Pair 3 Deadtime Configuration register oset */

/* Register Bit Definitions *************************************************/

/* SC Bit Fields */

#define FTU_SC_FLTPS_MASK              0xF000000u
#define FTU_SC_FLTPS_SHIFT             24u
#define FTU_SC_FLTPS_WIDTH             4u
#define FTU_SC_FLTPS(x)                (((uint32_t)(((uint32_t)(x))<<FTU_SC_FLTPS_SHIFT))&FTU_SC_FLTPS_MASK)
#define FTU_SC_CHNOUTEN_MASK           0xFF0000u
#define FTU_SC_CHNOUTEN_SHIFT          16u
#define FTU_SC_CHNOUTEN_WIDTH          8u
#define FTU_SC_CHNOUTEN(x)             (((uint32_t)(((uint32_t)(x))<<FTU_SC_CHNOUTEN_SHIFT))&FTU_SC_CHNOUTEN_MASK)
#define FTU_SC_TCKSEL_MASK             0xC000u
#define FTU_SC_TCKSEL_SHIFT            14u
#define FTU_SC_TCKSEL_WIDTH            2u
#define FTU_SC_TCKSEL(x)               (((uint32_t)(((uint32_t)(x))<<FTU_SC_TCKSEL_SHIFT))&FTU_SC_TCKSEL_MASK)
#define FTU_SC_OBECTL_MASK             0x800u
#define FTU_SC_OBECTL_SHIFT            11u
#define FTU_SC_OBECTL_WIDTH            1u
#define FTU_SC_OBECTL(x)               (((uint32_t)(((uint32_t)(x))<<FTU_SC_OBECTL_SHIFT))&FTU_SC_OBECTL_MASK)
#define FTU_SC_TOF_MASK                0x200u
#define FTU_SC_TOF_SHIFT               9u
#define FTU_SC_TOF_WIDTH               1u
#define FTU_SC_TOF(x)                  (((uint32_t)(((uint32_t)(x))<<FTU_SC_TOF_SHIFT))&FTU_SC_TOF_MASK)
#define FTU_SC_TOIE_MASK               0x100u
#define FTU_SC_TOIE_SHIFT              8u
#define FTU_SC_TOIE_WIDTH              1u
#define FTU_SC_TOIE(x)                 (((uint32_t)(((uint32_t)(x))<<FTU_SC_TOIE_SHIFT))&FTU_SC_TOIE_MASK)
#define FTU_SC_RF_MASK                 0x80u
#define FTU_SC_RF_SHIFT                7u
#define FTU_SC_RF_WIDTH                1u
#define FTU_SC_RF(x)                   (((uint32_t)(((uint32_t)(x))<<FTU_SC_RF_SHIFT))&FTU_SC_RF_MASK)
#define FTU_SC_RIE_MASK                0x40u
#define FTU_SC_RIE_SHIFT               6u
#define FTU_SC_RIE_WIDTH               1u
#define FTU_SC_RIE(x)                  (((uint32_t)(((uint32_t)(x))<<FTU_SC_RIE_SHIFT))&FTU_SC_RIE_MASK)
#define FTU_SC_CPWMS_MASK              0x20u
#define FTU_SC_CPWMS_SHIFT             5u
#define FTU_SC_CPWMS_WIDTH             1u
#define FTU_SC_CPWMS(x)                (((uint32_t)(((uint32_t)(x))<<FTU_SC_CPWMS_SHIFT))&FTU_SC_CPWMS_MASK)
#define FTU_SC_CLKS_MASK               0x18u
#define FTU_SC_CLKS_SHIFT              3u
#define FTU_SC_CLKS_WIDTH              2u
#define FTU_SC_CLKS_FTUINPUT           1u
#define FTU_SC_CLKS(x)                 (((uint32_t)(((uint32_t)(x))<<FTU_SC_CLKS_SHIFT))&FTU_SC_CLKS_MASK)
#define FTU_SC_PS_MASK                 0x7u
#define FTU_SC_PS_SHIFT                0u
#define FTU_SC_PS_WIDTH                3u
#define FTU_SC_PS(x)                   (((uint32_t)(((uint32_t)(x))<<FTU_SC_PS_SHIFT))&FTU_SC_PS_MASK)

/* SC Reg Mask */

#define FTU_SC_MASK                    0x0FFFCBFFu

/* CNT Bit Fields */

#define FTU_CNT_COUNT_MASK             0xFFFFu
#define FTU_CNT_COUNT_SHIFT            0u
#define FTU_CNT_COUNT_WIDTH            16u
#define FTU_CNT_COUNT(x)               (((uint32_t)(((uint32_t)(x))<<FTU_CNT_COUNT_SHIFT))&FTU_CNT_COUNT_MASK)

/* CNT Reg Mask */

#define FTU_CNT_COUNT_MASK             0xFFFFu
#define FTU_CNT_COUNT_SHIFT            0u
#define FTU_CNT_COUNT_WIDTH            16u
#define FTU_CNT_COUNT(x)               (((uint32_t)(((uint32_t)(x))<<FTU_CNT_COUNT_SHIFT))&FTU_CNT_COUNT_MASK)

/* MOD Bit Fields */

#define FTU_MOD_MOD_MASK               0xFFFFu
#define FTU_MOD_MOD_SHIFT              0u
#define FTU_MOD_MOD_WIDTH              16u
#define FTU_MOD_MOD(x)                 (((uint32_t)(((uint32_t)(x))<<FTU_MOD_MOD_SHIFT))&FTU_MOD_MOD_MASK)

/* MOD Reg Mask */

#define FTU_MOD_MASK                   0x0000FFFFu

/* CSC Bit Fields */

#define FTU_CSC_CHOV_MASK              0x400u
#define FTU_CSC_CHOV_SHIFT             10u
#define FTU_CSC_CHOV_WIDTH             1u
#define FTU_CSC_CHOV(x)                (((uint32_t)(((uint32_t)(x))<<FTU_CSC_CHOV_SHIFT))&FTU_CSC_CHOV_MASK)
#define FTU_CSC_CHIS_MASK              0x200u
#define FTU_CSC_CHIS_SHIFT             9u
#define FTU_CSC_CHIS_WIDTH             1u
#define FTU_CSC_CHIS(x)                (((uint32_t)(((uint32_t)(x))<<FTU_CSC_CHIS_SHIFT))&FTU_CSC_CHIS_MASK)
#define FTU_CSC_TRIGMODE_MASK          0x100u
#define FTU_CSC_TRIGMODE_SHIFT         8u
#define FTU_CSC_TRIGMODE_WIDTH         1u
#define FTU_CSC_TRIGMODE(x)            (((uint32_t)(((uint32_t)(x))<<FTU_CSC_TRIGMODE_SHIFT))&FTU_CSC_TRIGMODE_MASK)
#define FTU_CSC_CHF_MASK               0x80u
#define FTU_CSC_CHF_SHIFT              7u
#define FTU_CSC_CHF_WIDTH              1u
#define FTU_CSC_CHF(x)                 (((uint32_t)(((uint32_t)(x))<<FTU_CSC_CHF_SHIFT))&FTU_CSC_CHF_MASK)
#define FTU_CSC_CHIE_MASK              0x40u
#define FTU_CSC_CHIE_SHIFT             6u
#define FTU_CSC_CHIE_WIDTH             1u
#define FTU_CSC_CHIE(x)                (((uint32_t)(((uint32_t)(x))<<FTU_CSC_CHIE_SHIFT))&FTU_CSC_CHIE_MASK)
#define FTU_CSC_MSB_MASK               0x20u
#define FTU_CSC_MSB_SHIFT              5u
#define FTU_CSC_MSB_WIDTH              1u
#define FTU_CSC_MSB(x)                 (((uint32_t)(((uint32_t)(x))<<FTU_CSC_MSB_SHIFT))&FTU_CSC_MSB_MASK)
#define FTU_CSC_MSA_MASK               0x10u
#define FTU_CSC_MSA_SHIFT              4u
#define FTU_CSC_MSA_WIDTH              1u
#define FTU_CSC_MSA(x)                 (((uint32_t)(((uint32_t)(x))<<FTU_CSC_MSA_SHIFT))&FTU_CSC_MSA_MASK)
#define FTU_CSC_ELSB_MASK              0x8u
#define FTU_CSC_ELSB_SHIFT             3u
#define FTU_CSC_ELSB_WIDTH             1u
#define FTU_CSC_ELSB(x)                (((uint32_t)(((uint32_t)(x))<<FTU_CSC_ELSB_SHIFT))&FTU_CSC_ELSB_MASK)
#define FTU_CSC_ELSA_MASK              0x4u
#define FTU_CSC_ELSA_SHIFT             2u
#define FTU_CSC_ELSA_WIDTH             1u
#define FTU_CSC_ELSA(x)                (((uint32_t)(((uint32_t)(x))<<FTU_CSC_ELSA_SHIFT))&FTU_CSC_ELSA_MASK)
#define FTU_CSC_ICRST_MASK             0x2u
#define FTU_CSC_ICRST_SHIFT            1u
#define FTU_CSC_ICRST_WIDTH            1u
#define FTU_CSC_ICRST(x)               (((uint32_t)(((uint32_t)(x))<<FTU_CSC_ICRST_SHIFT))&FTU_CSC_ICRST_MASK)
#define FTU_CSC_DMA_MASK               0x1u
#define FTU_CSC_DMA_SHIFT              0u
#define FTU_CSC_DMA_WIDTH              1u
#define FTU_CSC_DMA(x)                 (((uint32_t)(((uint32_t)(x))<<FTU_CSC_DMA_SHIFT))&FTU_CSC_DMA_MASK)

/* CSC0 Reg Mask */

#define FTU_CSC_MASK                   0x000007FFu

/* CV Bit Fields */

#define FTU_CV_VAL_MASK                0xFFFFu
#define FTU_CV_VAL_SHIFT               0u
#define FTU_CV_VAL_WIDTH               16u
#define FTU_CV_VAL(x)                  (((uint32_t)(((uint32_t)(x))<<FTU_CV_VAL_SHIFT))&FTU_CV_VAL_MASK)

/* CV0 Reg Mask */

#define FTU_CV_MASK                    0x0000FFFFu

/* CNTIN Bit Fields */

#define FTU_CNTIN_INIT_MASK            0xFFFFu
#define FTU_CNTIN_INIT_SHIFT           0u
#define FTU_CNTIN_INIT_WIDTH           16u
#define FTU_CNTIN_INIT(x)              (((uint32_t)(((uint32_t)(x))<<FTU_CNTIN_INIT_SHIFT))&FTU_CNTIN_INIT_MASK)

/* CNTIN Reg Mask */

#define FTU_CNTIN_MASK                 0x0000FFFFu

/* STATUS Bit Fields */

#define FTU_STATUS_CHNF_MASK           0xFFu
#define FTU_STATUS_CHNF_SHIFT          0u
#define FTU_STATUS_CHNF_WIDTH          8u
#define FTU_STATUS_CHNF(x)             (((uint32_t)(((uint32_t)(x))<<FTU_STATUS_CHNF_SHIFT))&FTU_STATUS_CHNF_MASK)

/* STATUS Reg Mask */

#define FTU_STATUS_MASK                0x000000FFu

/* MODE Bit Fields */

#define FTU_MODE_FAULTIE_MASK          0x80u
#define FTU_MODE_FAULTIE_SHIFT         7u
#define FTU_MODE_FAULTIE_WIDTH         1u
#define FTU_MODE_FAULTIE(x)            (((uint32_t)(((uint32_t)(x))<<FTU_MODE_FAULTIE_SHIFT))&FTU_MODE_FAULTIE_MASK)
#define FTU_MODE_FAULTM_MASK           0x60u
#define FTU_MODE_FAULTM_SHIFT          5u
#define FTU_MODE_FAULTM_WIDTH          2u
#define FTU_MODE_FAULTM(x)             (((uint32_t)(((uint32_t)(x))<<FTU_MODE_FAULTM_SHIFT))&FTU_MODE_FAULTM_MASK)
#define FTU_MODE_PWMSYNC_MASK          0x8u
#define FTU_MODE_PWMSYNC_SHIFT         3u
#define FTU_MODE_PWMSYNC_WIDTH         1u
#define FTU_MODE_PWMSYNC(x)            (((uint32_t)(((uint32_t)(x))<<FTU_MODE_PWMSYNC_SHIFT))&FTU_MODE_PWMSYNC_MASK)
#define FTU_MODE_WPDIS_MASK            0x4u
#define FTU_MODE_WPDIS_SHIFT           2u
#define FTU_MODE_WPDIS_WIDTH           1u
#define FTU_MODE_WPDIS(x)              (((uint32_t)(((uint32_t)(x))<<FTU_MODE_WPDIS_SHIFT))&FTU_MODE_WPDIS_MASK)
#define FTU_MODE_INIT_MASK             0x2u
#define FTU_MODE_INIT_SHIFT            1u
#define FTU_MODE_INIT_WIDTH            1u
#define FTU_MODE_INIT(x)               (((uint32_t)(((uint32_t)(x))<<FTU_MODE_INIT_SHIFT))&FTU_MODE_INIT_MASK)
#define FTU_MODE_FTUEN_MASK            0x1u
#define FTU_MODE_FTUEN_SHIFT           0u
#define FTU_MODE_FTUEN_WIDTH           1u
#define FTU_MODE_FTUEN(x)              (((uint32_t)(((uint32_t)(x))<<FTU_MODE_FTUEN_SHIFT))&FTU_MODE_FTUEN_MASK)

/* MODE Reg Mask */

#define FTU_MODE_MASK                  0x000000EFu

/* SYNC Bit Fields */

#define FTU_SYNC_SWSYNC_MASK           0x80u
#define FTU_SYNC_SWSYNC_SHIFT          7u
#define FTU_SYNC_SWSYNC_WIDTH          1u
#define FTU_SYNC_SWSYNC(x)             (((uint32_t)(((uint32_t)(x))<<FTU_SYNC_SWSYNC_SHIFT))&FTU_SYNC_SWSYNC_MASK)
#define FTU_SYNC_TRIG2_MASK            0x40u
#define FTU_SYNC_TRIG2_SHIFT           6u
#define FTU_SYNC_TRIG2_WIDTH           1u
#define FTU_SYNC_TRIG2(x)              (((uint32_t)(((uint32_t)(x))<<FTU_SYNC_TRIG2_SHIFT))&FTU_SYNC_TRIG2_MASK)
#define FTU_SYNC_TRIG1_MASK            0x20u
#define FTU_SYNC_TRIG1_SHIFT           5u
#define FTU_SYNC_TRIG1_WIDTH           1u
#define FTU_SYNC_TRIG1(x)              (((uint32_t)(((uint32_t)(x))<<FTU_SYNC_TRIG1_SHIFT))&FTU_SYNC_TRIG1_MASK)
#define FTU_SYNC_TRIG0_MASK            0x10u
#define FTU_SYNC_TRIG0_SHIFT           4u
#define FTU_SYNC_TRIG0_WIDTH           1u
#define FTU_SYNC_TRIG0(x)              (((uint32_t)(((uint32_t)(x))<<FTU_SYNC_TRIG0_SHIFT))&FTU_SYNC_TRIG0_MASK)
#define FTU_SYNC_SYNCHOM_MASK          0x8u
#define FTU_SYNC_SYNCHOM_SHIFT         3u
#define FTU_SYNC_SYNCHOM_WIDTH         1u
#define FTU_SYNC_SYNCHOM(x)            (((uint32_t)(((uint32_t)(x))<<FTU_SYNC_SYNCHOM_SHIFT))&FTU_SYNC_SYNCHOM_MASK)
#define FTU_SYNC_REINIT_MASK           0x4u
#define FTU_SYNC_REINIT_SHIFT          2u
#define FTU_SYNC_REINIT_WIDTH          1u
#define FTU_SYNC_REINIT(x)             (((uint32_t)(((uint32_t)(x))<<FTU_SYNC_REINIT_SHIFT))&FTU_SYNC_REINIT_MASK)
#define FTU_SYNC_CNTMAX_MASK           0x2u
#define FTU_SYNC_CNTMAX_SHIFT          1u
#define FTU_SYNC_CNTMAX_WIDTH          1u
#define FTU_SYNC_CNTMAX(x)             (((uint32_t)(((uint32_t)(x))<<FTU_SYNC_CNTMAX_SHIFT))&FTU_SYNC_CNTMAX_MASK)
#define FTU_SYNC_CNTMIN_MASK           0x1u
#define FTU_SYNC_CNTMIN_SHIFT          0u
#define FTU_SYNC_CNTMIN_WIDTH          1u
#define FTU_SYNC_CNTMIN(x)             (((uint32_t)(((uint32_t)(x))<<FTU_SYNC_CNTMIN_SHIFT))&FTU_SYNC_CNTMIN_MASK)

/* SYNC Reg Mask */

#define FTU_SYNC_MASK                  0x000000FFu

/* OUTINIT Bit Fields */

#define FTU_OUTINIT_CHNOI_MASK         0xFFu
#define FTU_OUTINIT_CHNOI_SHIFT        0u
#define FTU_OUTINIT_CHNOI_WIDTH        8u
#define FTU_OUTINIT_CHNOI(x)           (((uint32_t)(((uint32_t)(x))<<FTU_OUTINIT_CHNOI_SHIFT))&FTU_OUTINIT_CHNOI_MASK)

/* OUTINIT Reg Mask */

#define FTU_OUTINIT_MASK               0x000000FFu

/* OUTMASK Bit Fields */

#define FTU_OUTMASK_CHNOM_MASK         0xFFu
#define FTU_OUTMASK_CHNOM_SHIFT        0u
#define FTU_OUTMASK_CHNOM_WIDTH        8u
#define FTU_OUTMASK_CHNOM(x)           (((uint32_t)(((uint32_t)(x))<<FTU_OUTMASK_CHNOM_SHIFT))&FTU_OUTMASK_CHNOM_MASK)

/* OUTMASK Reg Mask */

#define FTU_OUTMASK_MASK               0x000000FFu

/* CHCTRL Bit Fields */

#define FTU_CHCTRL_FAULTEN3_MASK       0x40000000u
#define FTU_CHCTRL_FAULTEN3_SHIFT      30u
#define FTU_CHCTRL_FAULTEN3_WIDTH      1u
#define FTU_CHCTRL_FAULTEN3(x)         (((uint32_t)(((uint32_t)(x))<<FTU_CHCTRL_FAULTEN3_SHIFT))&FTU_CHCTRL_FAULTEN3_MASK)
#define FTU_CHCTRL_SYNCEN3_MASK        0x20000000u
#define FTU_CHCTRL_SYNCEN3_SHIFT       29u
#define FTU_CHCTRL_SYNCEN3_WIDTH       1u
#define FTU_CHCTRL_SYNCEN3(x)          (((uint32_t)(((uint32_t)(x))<<FTU_CHCTRL_SYNCEN3_SHIFT))&FTU_CHCTRL_SYNCEN3_MASK)
#define FTU_CHCTRL_DTEN3_MASK          0x10000000u
#define FTU_CHCTRL_DTEN3_SHIFT         28u
#define FTU_CHCTRL_DTEN3_WIDTH         1u
#define FTU_CHCTRL_DTEN3(x)            (((uint32_t)(((uint32_t)(x))<<FTU_CHCTRL_DTEN3_SHIFT))&FTU_CHCTRL_DTEN3_MASK)
#define FTU_CHCTRL_COMP3_MASK          0x2000000u
#define FTU_CHCTRL_COMP3_SHIFT         25u
#define FTU_CHCTRL_COMP3_WIDTH         1u
#define FTU_CHCTRL_COMP3(x)            (((uint32_t)(((uint32_t)(x))<<FTU_CHCTRL_COMP3_SHIFT))&FTU_CHCTRL_COMP3_MASK)
#define FTU_CHCTRL_FAULTEN2_MASK       0x400000u
#define FTU_CHCTRL_FAULTEN2_SHIFT      22u
#define FTU_CHCTRL_FAULTEN2_WIDTH      1u
#define FTU_CHCTRL_FAULTEN2(x)         (((uint32_t)(((uint32_t)(x))<<FTU_CHCTRL_FAULTEN2_SHIFT))&FTU_CHCTRL_FAULTEN2_MASK)
#define FTU_CHCTRL_SYNCEN2_MASK        0x200000u
#define FTU_CHCTRL_SYNCEN2_SHIFT       21u
#define FTU_CHCTRL_SYNCEN2_WIDTH       1u
#define FTU_CHCTRL_SYNCEN2(x)          (((uint32_t)(((uint32_t)(x))<<FTU_CHCTRL_SYNCEN2_SHIFT))&FTU_CHCTRL_SYNCEN2_MASK)
#define FTU_CHCTRL_DTEN2_MASK          0x100000u
#define FTU_CHCTRL_DTEN2_SHIFT         20u
#define FTU_CHCTRL_DTEN2_WIDTH         1u
#define FTU_CHCTRL_DTEN2(x)            (((uint32_t)(((uint32_t)(x))<<FTU_CHCTRL_DTEN2_SHIFT))&FTU_CHCTRL_DTEN2_MASK)
#define FTU_CHCTRL_COMP2_MASK          0x20000u
#define FTU_CHCTRL_COMP2_SHIFT         17u
#define FTU_CHCTRL_COMP2_WIDTH         1u
#define FTU_CHCTRL_COMP2(x)            (((uint32_t)(((uint32_t)(x))<<FTU_CHCTRL_COMP2_SHIFT))&FTU_CHCTRL_COMP2_MASK)
#define FTU_CHCTRL_FAULTEN1_MASK       0x4000u
#define FTU_CHCTRL_FAULTEN1_SHIFT      14u
#define FTU_CHCTRL_FAULTEN1_WIDTH      1u
#define FTU_CHCTRL_FAULTEN1(x)         (((uint32_t)(((uint32_t)(x))<<FTU_CHCTRL_FAULTEN1_SHIFT))&FTU_CHCTRL_FAULTEN1_MASK)
#define FTU_CHCTRL_SYNCEN1_MASK        0x2000u
#define FTU_CHCTRL_SYNCEN1_SHIFT       13u
#define FTU_CHCTRL_SYNCEN1_WIDTH       1u
#define FTU_CHCTRL_SYNCEN1(x)          (((uint32_t)(((uint32_t)(x))<<FTU_CHCTRL_SYNCEN1_SHIFT))&FTU_CHCTRL_SYNCEN1_MASK)
#define FTU_CHCTRL_DTEN1_MASK          0x1000u
#define FTU_CHCTRL_DTEN1_SHIFT         12u
#define FTU_CHCTRL_DTEN1_WIDTH         1u
#define FTU_CHCTRL_DTEN1(x)            (((uint32_t)(((uint32_t)(x))<<FTU_CHCTRL_DTEN1_SHIFT))&FTU_CHCTRL_DTEN1_MASK)
#define FTU_CHCTRL_COMP1_MASK          0x200u
#define FTU_CHCTRL_COMP1_SHIFT         9u
#define FTU_CHCTRL_COMP1_WIDTH         1u
#define FTU_CHCTRL_COMP1(x)            (((uint32_t)(((uint32_t)(x))<<FTU_CHCTRL_COMP1_SHIFT))&FTU_CHCTRL_COMP1_MASK)
#define FTU_CHCTRL_FAULTEN0_MASK       0x40u
#define FTU_CHCTRL_FAULTEN0_SHIFT      6u
#define FTU_CHCTRL_FAULTEN0_WIDTH      1u
#define FTU_CHCTRL_FAULTEN0(x)         (((uint32_t)(((uint32_t)(x))<<FTU_CHCTRL_FAULTEN0_SHIFT))&FTU_CHCTRL_FAULTEN0_MASK)
#define FTU_CHCTRL_SYNCEN0_MASK        0x20u
#define FTU_CHCTRL_SYNCEN0_SHIFT       5u
#define FTU_CHCTRL_SYNCEN0_WIDTH       1u
#define FTU_CHCTRL_SYNCEN0(x)          (((uint32_t)(((uint32_t)(x))<<FTU_CHCTRL_SYNCEN0_SHIFT))&FTU_CHCTRL_SYNCEN0_MASK)
#define FTU_CHCTRL_DTEN0_MASK          0x10u
#define FTU_CHCTRL_DTEN0_SHIFT         4u
#define FTU_CHCTRL_DTEN0_WIDTH         1u
#define FTU_CHCTRL_DTEN0(x)            (((uint32_t)(((uint32_t)(x))<<FTU_CHCTRL_DTEN0_SHIFT))&FTU_CHCTRL_DTEN0_MASK)
#define FTU_CHCTRL_COMP0_MASK          0x2u
#define FTU_CHCTRL_COMP0_SHIFT         1u
#define FTU_CHCTRL_COMP0_WIDTH         1u
#define FTU_CHCTRL_COMP0(x)            (((uint32_t)(((uint32_t)(x))<<FTU_CHCTRL_COMP0_SHIFT))&FTU_CHCTRL_COMP0_MASK)

#define FTU_CHCTRL_DEADTIME_CHANNEL(x) ((uint32_t)(1U << ((((x) >> 1U) << 3U) + 4U)))
#define FTU_CHCTRL_COMPLEMENT_CHANNEL(x) (1U << ((((x) >> 1U) << 3U) + 1U))
#define FTU_CHCTRL_SYNCEN_CHANNEL(x) (1U << ((((x) >> 1U) << 3U) + 5U))
#define FTU_CHCTRL_FAULT_CHANNEL(x) (1U << ((((x) >> 1U) << 3U) + 6U))

/* CHCTRL Reg Mask */

#define FTU_CHCTRL_MASK                0x72727272u

/* DEADTIME Bit Fields */

#define FTU_DEADTIME_DTVALEX_MASK      0xF0000u
#define FTU_DEADTIME_DTVALEX_SHIFT     16u
#define FTU_DEADTIME_DTVALEX_WIDTH     4u
#define FTU_DEADTIME_DTVALEX(x)        (((uint32_t)(((uint32_t)(x))<<FTU_DEADTIME_DTVALEX_SHIFT))&FTU_DEADTIME_DTVALEX_MASK)
#define FTU_DEADTIME_DTPS_MASK         0xC0u
#define FTU_DEADTIME_DTPS_SHIFT        6u
#define FTU_DEADTIME_DTPS_WIDTH        2u
#define FTU_DEADTIME_DTPS(x)           (((uint32_t)(((uint32_t)(x))<<FTU_DEADTIME_DTPS_SHIFT))&FTU_DEADTIME_DTPS_MASK)
#define FTU_DEADTIME_DTVAL_MASK        0x3Fu
#define FTU_DEADTIME_DTVAL_SHIFT       0u
#define FTU_DEADTIME_DTVAL_WIDTH       6u
#define FTU_DEADTIME_DTVAL(x)          (((uint32_t)(((uint32_t)(x))<<FTU_DEADTIME_DTVAL_SHIFT))&FTU_DEADTIME_DTVAL_MASK)

/* DEADTIME Reg Mask */

#define FTU_DEADTIME_MASK              0x000F00FFu

/* TRIGCONF Bit Fields */

#define FTU_TRIGCONF_CH7TRIG_MASK      0x200u
#define FTU_TRIGCONF_CH7TRIG_SHIFT     9u
#define FTU_TRIGCONF_CH7TRIG_WIDTH     1u
#define FTU_TRIGCONF_CH7TRIG(x)        (((uint32_t)(((uint32_t)(x))<<FTU_TRIGCONF_CH7TRIG_SHIFT))&FTU_TRIGCONF_CH7TRIG_MASK)
#define FTU_TRIGCONF_CH6TRIG_MASK      0x100u
#define FTU_TRIGCONF_CH6TRIG_SHIFT     8u
#define FTU_TRIGCONF_CH6TRIG_WIDTH     1u
#define FTU_TRIGCONF_CH6TRIG(x)        (((uint32_t)(((uint32_t)(x))<<FTU_TRIGCONF_CH6TRIG_SHIFT))&FTU_TRIGCONF_CH6TRIG_MASK)
#define FTU_TRIGCONF_TRIGF_MASK        0x80u
#define FTU_TRIGCONF_TRIGF_SHIFT       7u
#define FTU_TRIGCONF_TRIGF_WIDTH       1u
#define FTU_TRIGCONF_TRIGF(x)          (((uint32_t)(((uint32_t)(x))<<FTU_TRIGCONF_TRIGF_SHIFT))&FTU_TRIGCONF_TRIGF_MASK)
#define FTU_TRIGCONF_RELOADTRIGEN_MASK 0x40u
#define FTU_TRIGCONF_RELOADTRIGEN_SHIFT 6u
#define FTU_TRIGCONF_RELOADTRIGEN_WIDTH 1u
#define FTU_TRIGCONF_RELOADTRIGEN(x)   (((uint32_t)(((uint32_t)(x))<<FTU_TRIGCONF_RELOADTRIGEN_SHIFT))&FTU_TRIGCONF_RELOADTRIGEN_MASK)
#define FTU_TRIGCONF_CH1TRIG_MASK      0x20u
#define FTU_TRIGCONF_CH1TRIG_SHIFT     5u
#define FTU_TRIGCONF_CH1TRIG_WIDTH     1u
#define FTU_TRIGCONF_CH1TRIG(x)        (((uint32_t)(((uint32_t)(x))<<FTU_TRIGCONF_CH1TRIG_SHIFT))&FTU_TRIGCONF_CH1TRIG_MASK)
#define FTU_TRIGCONF_CH0TRIG_MASK      0x10u
#define FTU_TRIGCONF_CH0TRIG_SHIFT     4u
#define FTU_TRIGCONF_CH0TRIG_WIDTH     1u
#define FTU_TRIGCONF_CH0TRIG(x)        (((uint32_t)(((uint32_t)(x))<<FTU_TRIGCONF_CH0TRIG_SHIFT))&FTU_TRIGCONF_CH0TRIG_MASK)
#define FTU_TRIGCONF_CH5TRIG_MASK      0x8u
#define FTU_TRIGCONF_CH5TRIG_SHIFT     3u
#define FTU_TRIGCONF_CH5TRIG_WIDTH     1u
#define FTU_TRIGCONF_CH5TRIG(x)        (((uint32_t)(((uint32_t)(x))<<FTU_TRIGCONF_CH5TRIG_SHIFT))&FTU_TRIGCONF_CH5TRIG_MASK)
#define FTU_TRIGCONF_CH4TRIG_MASK      0x4u
#define FTU_TRIGCONF_CH4TRIG_SHIFT     2u
#define FTU_TRIGCONF_CH4TRIG_WIDTH     1u
#define FTU_TRIGCONF_CH4TRIG(x)        (((uint32_t)(((uint32_t)(x))<<FTU_TRIGCONF_CH4TRIG_SHIFT))&FTU_TRIGCONF_CH4TRIG_MASK)
#define FTU_TRIGCONF_CH3TRIG_MASK      0x2u
#define FTU_TRIGCONF_CH3TRIG_SHIFT     1u
#define FTU_TRIGCONF_CH3TRIG_WIDTH     1u
#define FTU_TRIGCONF_CH3TRIG(x)        (((uint32_t)(((uint32_t)(x))<<FTU_TRIGCONF_CH3TRIG_SHIFT))&FTU_TRIGCONF_CH3TRIG_MASK)
#define FTU_TRIGCONF_CH2TRIG_MASK      0x1u
#define FTU_TRIGCONF_CH2TRIG_SHIFT     0u
#define FTU_TRIGCONF_CH2TRIG_WIDTH     1u
#define FTU_TRIGCONF_CH2TRIG(x)        (((uint32_t)(((uint32_t)(x))<<FTU_TRIGCONF_CH2TRIG_SHIFT))&FTU_TRIGCONF_CH2TRIG_MASK)

/* TRIGCONF Reg Mask */

#define FTU_TRIGCONF_MASK              0x000003FFu

/* POL Bit Fields */

#define FTU_POL_POLN_MASK              0xFFu
#define FTU_POL_POLN_SHIFT             0u
#define FTU_POL_POLN_WIDTH             8u
#define FTU_POL_POLN(x)                (((uint32_t)(((uint32_t)(x))<<FTU_POL_POLN_SHIFT))&FTU_POL_POLN_MASK)

/* POL Reg Mask */

#define FTU_POL_MASK                   0x000000FFu

/* FMS Bit Fields */

#define FTU_FMS_FAULTF_MASK            0x80u
#define FTU_FMS_FAULTF_SHIFT           7u
#define FTU_FMS_FAULTF_WIDTH           1u
#define FTU_FMS_FAULTF(x)              (((uint32_t)(((uint32_t)(x))<<FTU_FMS_FAULTF_SHIFT))&FTU_FMS_FAULTF_MASK)
#define FTU_FMS_WPEN_MASK              0x40u
#define FTU_FMS_WPEN_SHIFT             6u
#define FTU_FMS_WPEN_WIDTH             1u
#define FTU_FMS_WPEN(x)                (((uint32_t)(((uint32_t)(x))<<FTU_FMS_WPEN_SHIFT))&FTU_FMS_WPEN_MASK)
#define FTU_FMS_FAULTIN_MASK           0x20u
#define FTU_FMS_FAULTIN_SHIFT          5u
#define FTU_FMS_FAULTIN_WIDTH          1u
#define FTU_FMS_FAULTIN(x)             (((uint32_t)(((uint32_t)(x))<<FTU_FMS_FAULTIN_SHIFT))&FTU_FMS_FAULTIN_MASK)
#define FTU_FMS_FAULTF3_MASK           0x8u
#define FTU_FMS_FAULTF3_SHIFT          3u
#define FTU_FMS_FAULTF3_WIDTH          1u
#define FTU_FMS_FAULTF3(x)             (((uint32_t)(((uint32_t)(x))<<FTU_FMS_FAULTF3_SHIFT))&FTU_FMS_FAULTF3_MASK)
#define FTU_FMS_FAULTF2_MASK           0x4u
#define FTU_FMS_FAULTF2_SHIFT          2u
#define FTU_FMS_FAULTF2_WIDTH          1u
#define FTU_FMS_FAULTF2(x)             (((uint32_t)(((uint32_t)(x))<<FTU_FMS_FAULTF2_SHIFT))&FTU_FMS_FAULTF2_MASK)
#define FTU_FMS_FAULTF1_MASK           0x2u
#define FTU_FMS_FAULTF1_SHIFT          1u
#define FTU_FMS_FAULTF1_WIDTH          1u
#define FTU_FMS_FAULTF1(x)             (((uint32_t)(((uint32_t)(x))<<FTU_FMS_FAULTF1_SHIFT))&FTU_FMS_FAULTF1_MASK)
#define FTU_FMS_FAULTF0_MASK           0x1u
#define FTU_FMS_FAULTF0_SHIFT          0u
#define FTU_FMS_FAULTF0_WIDTH          1u
#define FTU_FMS_FAULTF0(x)             (((uint32_t)(((uint32_t)(x))<<FTU_FMS_FAULTF0_SHIFT))&FTU_FMS_FAULTF0_MASK)

/* FMS Reg Mask */

#define FTU_FMS_MASK                   0x000000EFu

/* FILTER Bit Fields */

#define FTU_FILTER_CH3FVAL_MASK        0xF000u
#define FTU_FILTER_CH3FVAL_SHIFT       12u
#define FTU_FILTER_CH3FVAL_WIDTH       4u
#define FTU_FILTER_CH3FVAL(x)          (((uint32_t)(((uint32_t)(x))<<FTU_FILTER_CH3FVAL_SHIFT))&FTU_FILTER_CH3FVAL_MASK)
#define FTU_FILTER_CH2FVAL_MASK        0xF00u
#define FTU_FILTER_CH2FVAL_SHIFT       8u
#define FTU_FILTER_CH2FVAL_WIDTH       4u
#define FTU_FILTER_CH2FVAL(x)          (((uint32_t)(((uint32_t)(x))<<FTU_FILTER_CH2FVAL_SHIFT))&FTU_FILTER_CH2FVAL_MASK)
#define FTU_FILTER_CH1FVAL_MASK        0xF0u
#define FTU_FILTER_CH1FVAL_SHIFT       4u
#define FTU_FILTER_CH1FVAL_WIDTH       4u
#define FTU_FILTER_CH1FVAL(x)          (((uint32_t)(((uint32_t)(x))<<FTU_FILTER_CH1FVAL_SHIFT))&FTU_FILTER_CH1FVAL_MASK)
#define FTU_FILTER_CH0FVAL_MASK        0xFu
#define FTU_FILTER_CH0FVAL_SHIFT       0u
#define FTU_FILTER_CH0FVAL_WIDTH       4u
#define FTU_FILTER_CH0FVAL(x)          (((uint32_t)(((uint32_t)(x))<<FTU_FILTER_CH0FVAL_SHIFT))&FTU_FILTER_CH0FVAL_MASK)

/* FILTER Reg Mask */

#define FTU_FILTER_MASK                0x0000FFFFu

/* FLTCTRL Bit Fields */

#define FTU_FLTCTRL_FSTATE_MASK        0x8000u
#define FTU_FLTCTRL_FSTATE_SHIFT       15u
#define FTU_FLTCTRL_FSTATE_WIDTH       1u
#define FTU_FLTCTRL_FSTATE(x)          (((uint32_t)(((uint32_t)(x))<<FTU_FLTCTRL_FSTATE_SHIFT))&FTU_FLTCTRL_FSTATE_MASK)
#define FTU_FLTCTRL_FFVAL_MASK         0xF00u
#define FTU_FLTCTRL_FFVAL_SHIFT        8u
#define FTU_FLTCTRL_FFVAL_WIDTH        4u
#define FTU_FLTCTRL_FFVAL(x)           (((uint32_t)(((uint32_t)(x))<<FTU_FLTCTRL_FFVAL_SHIFT))&FTU_FLTCTRL_FFVAL_MASK)
#define FTU_FLTCTRL_FLT3GFEN_MASK      0x80u
#define FTU_FLTCTRL_FLT3GFEN_SHIFT     7u
#define FTU_FLTCTRL_FLT3GFEN_WIDTH     1u
#define FTU_FLTCTRL_FLT3GFEN(x)        (((uint32_t)(((uint32_t)(x))<<FTU_FLTCTRL_FLT3GFEN_SHIFT))&FTU_FLTCTRL_FLT3GFEN_MASK)
#define FTU_FLTCTRL_FLT2GFEN_MASK      0x40u
#define FTU_FLTCTRL_FLT2GFEN_SHIFT     6u
#define FTU_FLTCTRL_FLT2GFEN_WIDTH     1u
#define FTU_FLTCTRL_FLT2GFEN(x)        (((uint32_t)(((uint32_t)(x))<<FTU_FLTCTRL_FLT2GFEN_SHIFT))&FTU_FLTCTRL_FLT2GFEN_MASK)
#define FTU_FLTCTRL_FLT1GFEN_MASK      0x20u
#define FTU_FLTCTRL_FLT1GFEN_SHIFT     5u
#define FTU_FLTCTRL_FLT1GFEN_WIDTH     1u
#define FTU_FLTCTRL_FLT1GFEN(x)        (((uint32_t)(((uint32_t)(x))<<FTU_FLTCTRL_FLT1GFEN_SHIFT))&FTU_FLTCTRL_FLT1GFEN_MASK)
#define FTU_FLTCTRL_FLT0GFEN_MASK      0x10u
#define FTU_FLTCTRL_FLT0GFEN_SHIFT     4u
#define FTU_FLTCTRL_FLT0GFEN_WIDTH     1u
#define FTU_FLTCTRL_FLT0GFEN(x)        (((uint32_t)(((uint32_t)(x))<<FTU_FLTCTRL_FLT0GFEN_SHIFT))&FTU_FLTCTRL_FLT0GFEN_MASK)
#define FTU_FLTCTRL_FAULT3EN_MASK      0x8u
#define FTU_FLTCTRL_FAULT3EN_SHIFT     3u
#define FTU_FLTCTRL_FAULT3EN_WIDTH     1u
#define FTU_FLTCTRL_FAULT3EN(x)        (((uint32_t)(((uint32_t)(x))<<FTU_FLTCTRL_FAULT3EN_SHIFT))&FTU_FLTCTRL_FAULT3EN_MASK)
#define FTU_FLTCTRL_FAULT2EN_MASK      0x4u
#define FTU_FLTCTRL_FAULT2EN_SHIFT     2u
#define FTU_FLTCTRL_FAULT2EN_WIDTH     1u
#define FTU_FLTCTRL_FAULT2EN(x)        (((uint32_t)(((uint32_t)(x))<<FTU_FLTCTRL_FAULT2EN_SHIFT))&FTU_FLTCTRL_FAULT2EN_MASK)
#define FTU_FLTCTRL_FAULT1EN_MASK      0x2u
#define FTU_FLTCTRL_FAULT1EN_SHIFT     1u
#define FTU_FLTCTRL_FAULT1EN_WIDTH     1u
#define FTU_FLTCTRL_FAULT1EN(x)        (((uint32_t)(((uint32_t)(x))<<FTU_FLTCTRL_FAULT1EN_SHIFT))&FTU_FLTCTRL_FAULT1EN_MASK)
#define FTU_FLTCTRL_FAULT0EN_MASK      0x1u
#define FTU_FLTCTRL_FAULT0EN_SHIFT     0u
#define FTU_FLTCTRL_FAULT0EN_WIDTH     1u
#define FTU_FLTCTRL_FAULT0EN(x)        (((uint32_t)(((uint32_t)(x))<<FTU_FLTCTRL_FAULT0EN_SHIFT))&FTU_FLTCTRL_FAULT0EN_MASK)

/* FLTCTRL Reg Mask */

#define FTU_FLTCTRL_MASK               0x00008FFFu

/* QDCTRL Bit Fields */

#define FTU_QDCTRL_PHAGFEN_MASK        0x80u
#define FTU_QDCTRL_PHAGFEN_SHIFT       7u
#define FTU_QDCTRL_PHAGFEN_WIDTH       1u
#define FTU_QDCTRL_PHAGFEN(x)          (((uint32_t)(((uint32_t)(x))<<FTU_QDCTRL_PHAGFEN_SHIFT))&FTU_QDCTRL_PHAGFEN_MASK)
#define FTU_QDCTRL_PHBGFEN_MASK        0x40u
#define FTU_QDCTRL_PHBGFEN_SHIFT       6u
#define FTU_QDCTRL_PHBGFEN_WIDTH       1u
#define FTU_QDCTRL_PHBGFEN(x)          (((uint32_t)(((uint32_t)(x))<<FTU_QDCTRL_PHBGFEN_SHIFT))&FTU_QDCTRL_PHBGFEN_MASK)
#define FTU_QDCTRL_PHAPOL_MASK         0x20u
#define FTU_QDCTRL_PHAPOL_SHIFT        5u
#define FTU_QDCTRL_PHAPOL_WIDTH        1u
#define FTU_QDCTRL_PHAPOL(x)           (((uint32_t)(((uint32_t)(x))<<FTU_QDCTRL_PHAPOL_SHIFT))&FTU_QDCTRL_PHAPOL_MASK)
#define FTU_QDCTRL_PHBPOL_MASK         0x10u
#define FTU_QDCTRL_PHBPOL_SHIFT        4u
#define FTU_QDCTRL_PHBPOL_WIDTH        1u
#define FTU_QDCTRL_PHBPOL(x)           (((uint32_t)(((uint32_t)(x))<<FTU_QDCTRL_PHBPOL_SHIFT))&FTU_QDCTRL_PHBPOL_MASK)
#define FTU_QDCTRL_QUADMODE_MASK       0x8u
#define FTU_QDCTRL_QUADMODE_SHIFT      3u
#define FTU_QDCTRL_QUADMODE_WIDTH      1u
#define FTU_QDCTRL_QUADMODE(x)         (((uint32_t)(((uint32_t)(x))<<FTU_QDCTRL_QUADMODE_SHIFT))&FTU_QDCTRL_QUADMODE_MASK)
#define FTU_QDCTRL_QUADIR_MASK         0x4u
#define FTU_QDCTRL_QUADIR_SHIFT        2u
#define FTU_QDCTRL_QUADIR_WIDTH        1u
#define FTU_QDCTRL_QUADIR(x)           (((uint32_t)(((uint32_t)(x))<<FTU_QDCTRL_QUADIR_SHIFT))&FTU_QDCTRL_QUADIR_MASK)
#define FTU_QDCTRL_TOFDIR_MASK         0x2u
#define FTU_QDCTRL_TOFDIR_SHIFT        1u
#define FTU_QDCTRL_TOFDIR_WIDTH        1u
#define FTU_QDCTRL_TOFDIR(x)           (((uint32_t)(((uint32_t)(x))<<FTU_QDCTRL_TOFDIR_SHIFT))&FTU_QDCTRL_TOFDIR_MASK)
#define FTU_QDCTRL_QUADEN_MASK         0x1u
#define FTU_QDCTRL_QUADEN_SHIFT        0u
#define FTU_QDCTRL_QUADEN_WIDTH        1u
#define FTU_QDCTRL_QUADEN(x)           (((uint32_t)(((uint32_t)(x))<<FTU_QDCTRL_QUADEN_SHIFT))&FTU_QDCTRL_QUADEN_MASK)

/* QDCTRL Reg Mask */

#define FTU_QDCTRL_MASK                0x000000FFu

/* CONF Bit Fields */

#define FTU_CONF_RTRIGR_MASK           0x800u
#define FTU_CONF_RTRIGR_SHIFT          11u
#define FTU_CONF_RTRIGR_WIDTH          1u
#define FTU_CONF_RTRIGR(x)             (((uint32_t)(((uint32_t)(x))<<FTU_CONF_RTRIGR_SHIFT))&FTU_CONF_RTRIGR_MASK)
#define FTU_CONF_GTBEEN_MASK           0x200u
#define FTU_CONF_GTBEEN_SHIFT          9u
#define FTU_CONF_GTBEEN_WIDTH          1u
#define FTU_CONF_GTBEEN(x)             (((uint32_t)(((uint32_t)(x))<<FTU_CONF_GTBEEN_SHIFT))&FTU_CONF_GTBEEN_MASK)
#define FTU_CONF_DBG_MASK              0xC0u
#define FTU_CONF_DBG_SHIFT             6u
#define FTU_CONF_DBG_WIDTH             2u
#define FTU_CONF_DBG(x)                (((uint32_t)(((uint32_t)(x))<<FTU_CONF_DBG_SHIFT))&FTU_CONF_DBG_MASK)
#define FTU_CONF_LDFQ_MASK             0x1Fu
#define FTU_CONF_LDFQ_SHIFT            0u
#define FTU_CONF_LDFQ_WIDTH            5u
#define FTU_CONF_LDFQ(x)               (((uint32_t)(((uint32_t)(x))<<FTU_CONF_LDFQ_SHIFT))&FTU_CONF_LDFQ_MASK)

/* CONF Reg Mask */

#define FTU_CONF_MASK                  0x00000ADFu

/* FLTPOL Bit Fields */

#define FTU_FLTPOL_FLT3POL_MASK        0x8u
#define FTU_FLTPOL_FLT3POL_SHIFT       3u
#define FTU_FLTPOL_FLT3POL_WIDTH       1u
#define FTU_FLTPOL_FLT3POL(x)          (((uint32_t)(((uint32_t)(x))<<FTU_FLTPOL_FLT3POL_SHIFT))&FTU_FLTPOL_FLT3POL_MASK)
#define FTU_FLTPOL_FLT2POL_MASK        0x4u
#define FTU_FLTPOL_FLT2POL_SHIFT       2u
#define FTU_FLTPOL_FLT2POL_WIDTH       1u
#define FTU_FLTPOL_FLT2POL(x)          (((uint32_t)(((uint32_t)(x))<<FTU_FLTPOL_FLT2POL_SHIFT))&FTU_FLTPOL_FLT2POL_MASK)
#define FTU_FLTPOL_FLT1POL_MASK        0x2u
#define FTU_FLTPOL_FLT1POL_SHIFT       1u
#define FTU_FLTPOL_FLT1POL_WIDTH       1u
#define FTU_FLTPOL_FLT1POL(x)          (((uint32_t)(((uint32_t)(x))<<FTU_FLTPOL_FLT1POL_SHIFT))&FTU_FLTPOL_FLT1POL_MASK)
#define FTU_FLTPOL_FLT0POL_MASK        0x1u
#define FTU_FLTPOL_FLT0POL_SHIFT       0u
#define FTU_FLTPOL_FLT0POL_WIDTH       1u
#define FTU_FLTPOL_FLT0POL(x)          (((uint32_t)(((uint32_t)(x))<<FTU_FLTPOL_FLT0POL_SHIFT))&FTU_FLTPOL_FLT0POL_MASK)

/* FLTPOL Reg Mask */

#define FTU_FLTPOL_MASK                0x0000000Fu

/* SYNCONF Bit Fields */

#define FTU_SYNCONF_CNTINC_MASK        0x4u
#define FTU_SYNCONF_CNTINC_SHIFT       2u
#define FTU_SYNCONF_CNTINC_WIDTH       1u
#define FTU_SYNCONF_CNTINC(x)          (((uint32_t)(((uint32_t)(x))<<FTU_SYNCONF_CNTINC_SHIFT))&FTU_SYNCONF_CNTINC_MASK)
#define FTU_SYNCONF_HWTRIGMODE_MASK    0x1u
#define FTU_SYNCONF_HWTRIGMODE_SHIFT   0u
#define FTU_SYNCONF_HWTRIGMODE_WIDTH   1u
#define FTU_SYNCONF_HWTRIGMODE(x)      (((uint32_t)(((uint32_t)(x))<<FTU_SYNCONF_HWTRIGMODE_SHIFT))&FTU_SYNCONF_HWTRIGMODE_MASK)

/* SYNCONF Reg Mask */

#define FTU_SYNCONF_MASK               0x00000005u

/* INVCTRL Bit Fields */

#define FTU_INVCTRL_INV3EN_MASK        0x8u
#define FTU_INVCTRL_INV3EN_SHIFT       3u
#define FTU_INVCTRL_INV3EN_WIDTH       1u
#define FTU_INVCTRL_INV3EN(x)          (((uint32_t)(((uint32_t)(x))<<FTU_INVCTRL_INV3EN_SHIFT))&FTU_INVCTRL_INV3EN_MASK)

#define FTU_INVCTRL_INV2EN_MASK        0x4u
#define FTU_INVCTRL_INV2EN_SHIFT       2u
#define FTU_INVCTRL_INV2EN_WIDTH       1u
#define FTU_INVCTRL_INV2EN(x)          (((uint32_t)(((uint32_t)(x))<<FTU_INVCTRL_INV2EN_SHIFT))&FTU_INVCTRL_INV2EN_MASK)

#define FTU_INVCTRL_INV1EN_MASK        0x2u
#define FTU_INVCTRL_INV1EN_SHIFT       1u
#define FTU_INVCTRL_INV1EN_WIDTH       1u
#define FTU_INVCTRL_INV1EN(x)          (((uint32_t)(((uint32_t)(x))<<FTU_INVCTRL_INV1EN_SHIFT))&FTU_INVCTRL_INV1EN_MASK)

#define FTU_INVCTRL_INV0EN_MASK        0x1u
#define FTU_INVCTRL_INV0EN_SHIFT       0u
#define FTU_INVCTRL_INV0EN_WIDTH       1u
#define FTU_INVCTRL_INV0EN(x)          (((uint32_t)(((uint32_t)(x))<<FTU_INVCTRL_INV0EN_SHIFT))&FTU_INVCTRL_INV0EN_MASK)

/* INVCTRL Reg Mask */

#define FTU_INVCTRL_MASK               0x0000000Fu

/* SWOCTRL Bit Fields */

#define FTU_SWOCTRL_CHNOCV_MASK        0xFF00u
#define FTU_SWOCTRL_CHNOCV_SHIFT       8u
#define FTU_SWOCTRL_CHNOCV_WIDTH       8u
#define FTU_SWOCTRL_CHNOCV(x)          (((uint32_t)(((uint32_t)(x))<<FTU_SWOCTRL_CHNOCV_SHIFT))&FTU_SWOCTRL_CHNOCV_MASK)
#define FTU_SWOCTRL_CHNOC_MASK         0xFFu
#define FTU_SWOCTRL_CHNOC_SHIFT        0u
#define FTU_SWOCTRL_CHNOC_WIDTH        8u
#define FTU_SWOCTRL_CHNOC(x)           (((uint32_t)(((uint32_t)(x))<<FTU_SWOCTRL_CHNOC_SHIFT))&FTU_SWOCTRL_CHNOC_MASK)

/* SWOCTRL Reg Mask */

#define FTU_SWOCTRL_MASK               0x0000FFFFu

/* PWMLOAD Bit Fields */

#define FTU_PWMLOAD_LDOK_MASK          0x200u
#define FTU_PWMLOAD_LDOK_SHIFT         9u
#define FTU_PWMLOAD_LDOK_WIDTH         1u
#define FTU_PWMLOAD_LDOK(x)            (((uint32_t)(((uint32_t)(x))<<FTU_PWMLOAD_LDOK_SHIFT))&FTU_PWMLOAD_LDOK_MASK)
#define FTU_PWMLOAD_CHNSEL_MASK        0xFFu
#define FTU_PWMLOAD_CHNSEL_SHIFT       0u
#define FTU_PWMLOAD_CHNSEL_WIDTH       8u
#define FTU_PWMLOAD_CHNSEL(x)          (((uint32_t)(((uint32_t)(x))<<FTU_PWMLOAD_CHNSEL_SHIFT))&FTU_PWMLOAD_CHNSEL_MASK)

/* PWMLOAD Reg Mask */

#define FTU_PWMLOAD_MASK               0x000002FFu

/* PAIRDEADTIME Bit Fields */

#define FTU_PAIRDEADTIME_DTVALEX_MASK  0xF0000u
#define FTU_PAIRDEADTIME_DTVALEX_SHIFT 16u
#define FTU_PAIRDEADTIME_DTVALEX_WIDTH 4u
#define FTU_PAIRDEADTIME_DTVALEX(x)    (((uint32_t)(((uint32_t)(x))<<FTU_PAIRDEADTIME_DTVALEX_SHIFT))&FTU_PAIRDEADTIME_DTVALEX_MASK)
#define FTU_PAIRDEADTIME_DTPS_MASK     0xC0u
#define FTU_PAIRDEADTIME_DTPS_SHIFT    6u
#define FTU_PAIRDEADTIME_DTPS_WIDTH    2u
#define FTU_PAIRDEADTIME_DTPS(x)       (((uint32_t)(((uint32_t)(x))<<FTU_PAIRDEADTIME_DTPS_SHIFT))&FTU_PAIRDEADTIME_DTPS_MASK)
#define FTU_PAIRDEADTIME_DTVAL_MASK    0x3Fu
#define FTU_PAIRDEADTIME_DTVAL_SHIFT   0u
#define FTU_PAIRDEADTIME_DTVAL_WIDTH   6u
#define FTU_PAIRDEADTIME_DTVAL(x)      (((uint32_t)(((uint32_t)(x))<<FTU_PAIRDEADTIME_DTVAL_SHIFT))&FTU_PAIRDEADTIME_DTVAL_MASK)

/* PAIRDEADTIME0 Reg Mask */

#define FTU_PAIRDEADTIME_MASK          0x000F00FFu

#endif /* __ARCH_ARM_SRC_FC7300_HARDWARE_FC7300_FTU_H */
