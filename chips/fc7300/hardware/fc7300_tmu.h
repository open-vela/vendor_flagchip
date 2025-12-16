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

#ifndef __VENDOR_FLAGCHIP_CHIPS_FC7300_HARDWARE_FC7300_TMU_H
#define __VENDOR_FLAGCHIP_CHIPS_FC7300_HARDWARE_FC7300_TMU_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>
#include "hardware/fc7300_memorymap.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Register offsets *********************************************************/

#define FC7300_TMU_UNLOCK_OFFSET          0x0000  /* Unlock Register */
#define FC7300_TMU_TF_CTRL_OFFSET         0x0004  /* Temperature Flag Control Register */
#define FC7300_TMU_TF_STATUS_OFFSET       0x0008  /* Temperature Flag Status Register */
#define FC7300_TMU_TV_CTRL_OFFSET         0x000C  /* Temperature Voltage Control Register */
#define FC7300_TMU_TV_STATUS_OFFSET       0x0010  /* Temperature Voltage Status Register */

/* Register bit definitions *************************************************/

/* Unlock Register */

#define TMU_UNLOCK_UNLOCK               (1 << 0)  /* Bit 0:  TMU Control Register Unlock */
                                                  /* Bit 1-31: Reserved */

/* Temperature Flag Control Register */

#define TMU_TF_CTRL_TF_EN               (1 << 0)  /* Bit 0:  Flag-based Temperature Sensor Enable */
#define TMU_TF_CTRL_TF_FILT_BYP         (1 << 1)  /* Bit 1:  Flag-based Temperature Sensor Filter Bypass Control */
                                                  /* Bit 2-7:  Reserved */
#define TMU_TF_CTRL_TF_START_CNT_SHIFT  (8)       /* Bit 8-11: Flag-based Temperature Sensor Startup Counter */
#define TMU_TF_CTRL_TF_START_CNT_MASK   (0x0F << TMU_TF_CTRL_TF_START_CNT_SHIFT)
#  define TMU_TF_CTRL_TF_START_CNT(x)   (((x) << TMU_TF_CTRL_TF_START_CNT_SHIFT) & TMU_TF_CTRL_TF_START_CNT_MASK)
                                                  /* Bit 12-15: Reserved */
#define TMU_TF_CTRL_TF_HYSOFF           (1 << 16) /* Bit 16: Flag-based Temperature Sensor Hysteresis Control */
                                                  /* Bit 17-23: Reserved */
#define TMU_TF_CTRL_TF_RDYF_IE          (1 << 24) /* Bit 24: Flag-based Temperature Sensor Ready Interrupt Enable */
                                                  /* Bit 25: Reserved */
#define TMU_TF_CTRL_TF_125F_IE          (1 << 26) /* Bit 26: Temperature over 125 Interrupt Enable */
#define TMU_TF_CTRL_TF_150F_IE          (1 << 27) /* Bit 27: Temperature over 150 Interrupt Enable */
                                                  /* Bit 28-31: Reserved */

/* Temperature Flag Status Register */

#define TMU_TF_STATUS_TF_RDYF           (1 << 0)  /* Bit 0:  Flag-based Temperature Sensor Ready */
                                                  /* Bit 1:  Reserved */
#define TMU_TF_STATUS_TF_125F           (1 << 2)  /* Bit 2:  Temperature over 125 Flag */
#define TMU_TF_STATUS_TF_150F           (1 << 3)  /* Bit 3:  Temperature over 150 Flag */
                                                  /* Bit 4-9:  Reserved */
#define TMU_TF_STATUS_TF_125            (1 << 10) /* Bit 10: Temperature over 125 Status */
#define TMU_TF_STATUS_TF_150            (1 << 11) /* Bit 11: Temperature over 150 Status */
                                                  /* Bit 12-31: Reserved */

/* Temperature Voltage Control Register */

#define TMU_TV_CTRL_TV_EN               (1 << 0)  /* Bit 0:  Voltage-based Temperature Sensor Enable */
                                                  /* Bit 1-7:  Reserved */
#define TMU_TV_CTRL_TV_START_CNT_SHIFT  (8)       /* Bit 8-10: Voltage-based Temperature Sensor Startup Counter */
#define TMU_TV_CTRL_TV_START_CNT_MASK   (0x07 << TMU_TV_CTRL_TV_START_CNT_SHIFT)
#  define TMU_TV_CTRL_TV_START_CNT(x)   (((x) << TMU_TV_CTRL_TV_START_CNT_SHIFT) & TMU_TV_CTRL_TV_START_CNT_MASK)
                                                  /* Bit 11-23 Reserved */
#define TMU_TV_CTRL_TV_RDYF_IE          (1 << 24) /* Bit 24: Voltage-based Temperature Sensor Ready Interrupt Enable */
                                                  /* Bit 25-31: Reserved */

/* Temperature Voltage Status Register */

#define TMU_TV_STATUS_TV_RDYF           (1 << 0)  /* Bit 0:  Voltage-based Temperature Sensor Ready */
                                                  /* Bit 1-31: Reserved */

#endif /* __VENDOR_FLAGCHIP_CHIPS_FC7300_HARDWARE_FC7300_TMU_H */
