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

#ifndef __VENDOR_FLAGCHIP_CHIPS_FC7300_HARDWARE_FC7300_TSTMP_H
#define __VENDOR_FLAGCHIP_CHIPS_FC7300_HARDWARE_FC7300_TSTMP_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>
#include "hardware/fc7300_memorymap.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Register Offsets *********************************************************/

#define FC7300_TSTMP_VALL_OFFSET            0x0000  /* Low Value register */
#define FC7300_TSTMP_VALH_OFFSET            0x0004  /* High Value register */
#define FC7300_TSTMP_MOD_INTEN_OFFSET       0x0008  /* Modulate Interrupt Enable register */
#define FC7300_TSTMP_MOD_STATUS_OFFSET      0x000C  /* Modulate Status register */
#define FC7300_TSTMP_MOD_SETVAL_OFFSET(n)   (0x0010 + ((n) * 0x04))
#  define FC7300_TSTMP_MOD0_SETVAL_OFFSET   0x0010  /* Modulate0 Set Value Register */
#  define FC7300_TSTMP_MOD1_SETVAL_OFFSET   0x0014  /* Modulate1 Set Value Register */
#  define FC7300_TSTMP_MOD2_SETVAL_OFFSET   0x0018  /* Modulate2 Set Value Register */
#  define FC7300_TSTMP_MOD3_SETVAL_OFFSET   0x001C  /* Modulate3 Set Value Register */
#define FC7300_TSTMP_MOD0_SETVAL_COUNT      4

/* Register bit definitions *************************************************/

/* Low Value register */

#define TSTMP_VALL_TVALL_SHIFT         (0)       /* Bit 0-31: Low 32 bit of the 56 bit TSTMP timer value. */
#define  TSTMP_VALL_TVALL_MASK         (0xFFFFFFFF << TSTMP_VALL_TVALL_SHIFT)
#  define  TSTMP_VALL_TVALL(x)         (((x) << TSTMP_VALL_TVALL_SHIFT) & TSTMP_VALL_TVALL_MASK)

/* High Value Register */

#define TSTMP_VALH_TVALH_SHIFT         (0)       /* Bit 0-31: High 24 bit of the 56 bit TSTMP timer value. */
#define  TSTMP_VALH_TVALH_MASK         (0xFFFFFFFF << TSTMP_VALH_TVALH_SHIFT)
#  define  TSTMP_VALH_TVALH(x)         (((x) << TSTMP_VALH_TVALH_SHIFT) & TSTMP_VALH_TVALH_MASK)

/* Module Interrupt Enable Register */

#define TSTMP_MOD_INTEN_MOD_INTEN(n)   (0x01 << n)       /* Bit 0-3:  Modulate Timer Counter n Interrupt Enable */
                                                         /* Bit 4-7:  Reserved */
#define TSTMP_MOD_INTEN_MOD_MODE(n)    (0x0100 << n)     /* Bit 8-11: Modulate Timer Counter n Counter Mode */
                                                         /* Bit 12-15: Reserved */
#define TSTMP_MOD_INTEN_MOD_ENABLE(n)  (0x00010000 << n) /* Bit 16-19: Modulate Timer Counter n Enable */
                                                         /* Bit 20-31: Reserved */

/* Modulate Status Register */

#define TSTMP_MOD_STATUS_MOD_MATCH(n)  (0x01 << n)  /* Bit 0-3:  Modulate Timer Counter 0 Match Flag */
                                                    /* Bit 4-31: Reserved */

/* Modulate Set Value Register */

#define TSTMP_MOD_SETVAL_MOD_SETVAL_SHIFT   (0)       /* Bit 0-31: Set Modulate Timer Counter Value */
#define  TSTMP_MOD_SETVAL_MOD_SETVAL_MASK   (0xFFFFFFFF << TSTMP_MOD_SETVAL_MOD_SETVAL_SHIFT)
#  define  TSTMP_MOD_SETVAL_MOD_SETVAL(x)   (((x) << TSTMP_MOD_SETVAL_MOD_SETVAL_SHIFT) & TSTMP_MOD_SETVAL_MOD_SETVAL_MASK)

#endif /* __VENDOR_FLAGCHIP_CHIPS_FC7300_HARDWARE_FC7300_TSTMP_H */
