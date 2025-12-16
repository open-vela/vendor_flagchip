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

#ifndef __VENDOR_FLAGCHIP_CHIPS_FC7300_HARDWARE_FC7300_WDOG_H
#define __VENDOR_FLAGCHIP_CHIPS_FC7300_HARDWARE_FC7300_WDOG_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>
#include <hardware/fc7300_memorymap.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* WDOG Register Offsets ****************************************************/

#define FC7300_WDOG_CS_OFFSET       0x0000  /* Watchdog Control and Status Register */
#define FC7300_WDOG_COUNTER_OFFSET  0x0004  /* Watchdog Counter Register */
#define FC7300_WDOG_TIMEOUT_OFFSET  0x0008  /* Watchdog Timeout Value Register */
#define FC7300_WDOG_WINDOW_OFFSET   0x000c  /* Watchdog Window Register */

/* WDOG Register Addresses **************************************************/

#define FC7300_WDOG0_CS             (FC7300_WDOG0_BASE + FC7300_WDOG_CS_OFFSET)
#define FC7300_WDOG0_COUNTER        (FC7300_WDOG0_BASE + FC7300_WDOG_COUNTER_OFFSET)
#define FC7300_WDOG0_TIMEOUT        (FC7300_WDOG0_BASE + FC7300_WDOG_TIMEOUT_OFFSET)
#define FC7300_WDOG0_WINDOW         (FC7300_WDOG0_BASE + FC7300_WDOG_WINDOW_OFFSET)

#define FC7300_WDOG1_CS             (FC7300_WDOG1_BASE + FC7300_WDOG_CS_OFFSET)
#define FC7300_WDOG1_COUNTER        (FC7300_WDOG1_BASE + FC7300_WDOG_COUNTER_OFFSET)
#define FC7300_WDOG1_TIMEOUT        (FC7300_WDOG1_BASE + FC7300_WDOG_TIMEOUT_OFFSET)
#define FC7300_WDOG1_WINDOW         (FC7300_WDOG1_BASE + FC7300_WDOG_WINDOW_OFFSET)

#define FC7300_WDOG2_CS             (FC7300_WDOG2_BASE + FC7300_WDOG_CS_OFFSET)
#define FC7300_WDOG2_COUNTER        (FC7300_WDOG2_BASE + FC7300_WDOG_COUNTER_OFFSET)
#define FC7300_WDOG2_TIMEOUT        (FC7300_WDOG2_BASE + FC7300_WDOG_TIMEOUT_OFFSET)
#define FC7300_WDOG2_WINDOW         (FC7300_WDOG2_BASE + FC7300_WDOG_WINDOW_OFFSET)

/* WDOG Register Bitfield Definitions ***************************************/

/* Watchdog Control and Status Register */

#define WDOG_CS_STOP                 (1 << 0)  /* Bit 0:  Stop Enable */
#define WDOG_CS_WAIT                 (1 << 1)  /* Bit 1:  Wait Enable */
#define WDOG_CS_DBG                  (1 << 2)  /* Bit 2:  Debug Enable */
#define WDOG_CS_TST_SHIFT            (3)       /* Bits 3-4:  Watchdog self check */
#define WDOG_CS_TST_MASK             (3 << WDOG_CS_TST_SHIFT)
#define WDOG_CS_TST_DISABLE          (0 << WDOG_CS_TST_SHIFT) /* Watchdog self check mode disabled */
#define WDOG_CS_TST_USER             (1 << WDOG_CS_TST_SHIFT) /* Watchdog user mode enabled */
#define WDOG_CS_TST_LOWBYTE          (2 << WDOG_CS_TST_SHIFT) /* Watchdog low byte self check mode */
#define WDOG_CS_TST_HIGHBYTE         (3 << WDOG_CS_TST_SHIFT) /* Watchdog high byte self check mode */

#define WDOG_CS_UPDATE               (1 << 5)  /* Bit 5:  Allow updates */
#define WDOG_CS_INT                  (1 << 6)  /* Bit 6:  Watchdog Interrupt */
#define WDOG_CS_ENABLE_SHIFT         (7)       /* Bit 7:  Watchdog Enable/Disable */
#define WDOG_CS_ENABLE_MASK          (1 << WDOG_CS_ENABLE_SHIFT)
#define WDOG_CS_ENABLE               (1 << WDOG_CS_ENABLE_SHIFT)
#define WDOG_CS_CLK_SEL_SHIFT        (8)       /* Bits 8-9: Watchdog Clock */
#define WDOG_CS_CLK_SEL_MASK         (3 << WDOG_CS_CLK_SEL_SHIFT)
#define WDOG_CS_CLK_SEL_BUS_CLK      (0 << WDOG_CS_CLK_SEL_SHIFT) /* Bus clock */
#define WDOG_CS_CLK_SEL_AON_CLK      (1 << WDOG_CS_CLK_SEL_SHIFT) /* AON clock */
#define WDOG_CS_CLK_SEL_SOSC_CLK     (2 << WDOG_CS_CLK_SEL_SHIFT) /* SOSC clock */
#define WDOG_CS_CLK_SEL_SIRC_CLK     (3 << WDOG_CS_CLK_SEL_SHIFT) /* SIRC clock */

#define WDOG_CS_RECFG_STAT           (1 << 10) /* Bit 10: Reconfiguration Success */
#define WDOG_CS_ULK_STAT             (1 << 11) /* Bit 11: Unlock status */
#define WDOG_CS_PRESCALER_SHIFT      (12)      /* Bit 12: Watchdog 256 prescale enable/disable */
#define WDOG_CS_PRESCALER_MASK       (1 << WDOG_CS_PRESCALER_SHIFT)
#define WDOG_CS_PRESCALER_ENABLE     (1 << WDOG_CS_PRESCALER_SHIFT) /* Bit 12: Watchdog prescalr */
#define WDOG_CS_FLAG                 (1 << 14)                      /* Bit 14: Watchdog Interrupt Flag */
#define WDOG_CS_WIN                  (1 << 15)                      /* Bit 15: Watchdog Window */

#define WDOG_CS_DLY_CNT_MSB_SHIFT    (16)      /* Bit 16-17: Reset delay counter MSB */
#define WDOG_CS_DLY_CNT_MSB_MASK     (3 << WDOG_CS_DLY_CNT_MSB_SHIFT)
#define WDOG_CS_DLY_CNT_MSB_128      (0 << WDOG_CS_DLY_CNT_MSB_SHIFT)
#define WDOG_CS_DLY_CNT_MSB_256      (1 << WDOG_CS_DLY_CNT_MSB_SHIFT)
#define WDOG_CS_DLY_CNT_MSB_512      (2 << WDOG_CS_DLY_CNT_MSB_SHIFT)
#define WDOG_CS_DLY_CNT_MSB_1024     (3 << WDOG_CS_DLY_CNT_MSB_SHIFT)

/* Watchdog Counter Register (16-bit counter value) */

#define WDOG_COUNTER_CNTL_SHIFT      (0)       /* Bits 0-7:  Low 8 bit of the Watchdog Counter */
#define WDOG_COUNTER_CNTL_MASK       (0xff << WDOG_COUNTER_CNTL_SHIFT)
#define WDOG_COUNTER_CNTH_SHIFT      (8)       /* Bits 8-15: High 8 bit of the Watchdog Counter */
#define WDOG_COUNTER_CNTH_MASK       (0xff << WDOG_COUNTER_CNTH_SHIFT)

/* The refresh write sequence is a 32-bit write of 0x20cffc20 */

#define WDOG_COUNTER_REFRESH         0x20cffc20

/* Value to unlock the watchdog registers */

#define WDOG_COUNTER_UNLOCK          0x08181982

/* Watchdog Timeout Value Register */

#define WDOG_TIMEOUT_TIMEOUT_VALL_SHIFT   (0)       /* Bits 0-7:  Low 8 bit of the timeout value */
#define WDOG_TIMEOUT_TIMEOUT_VALL_MASK    (0xff << WDOG_TIMEOUT_TIMEOUT_VALL_SHIFT)
#define WDOG_TIMEOUT_TIMEOUT_VALH_SHIFT   (8)       /* Bits 8-15: High 8 bit of the timeout value */
#define WDOG_TIMEOUT_TIMEOUT_VALH_MASK    (0xff << WDOG_TIMEOUT_TIMEOUT_VALH_SHIFT)

/* Watchdog Window Register */

#define WDOG_WINDOW_WINDOW_LOW_SHIFT      (0)       /* Bits 0-7:  Low 8 bit of Watchdog Window */
#define WDOG_WINDOW_WINDOW_LOW_MASK       (0xff << WDOG_WINDOW_WINDOW_LOW_SHIFT)
#define WDOG_WINDOW_WINDOW_HIGH_SHIFT     (8)       /* Bits 8-15: High 8 bit of Watchdog Window */
#define WDOG_WINDOW_WINDOW_HIGH_MASK      (0xff << WDOG_WINDOW_WINDOW_HIGH_SHIFT)

#endif /* __VENDOR_FLAGCHIP_CHIPS_FC7300_HARDWARE_FC7300_WDOG_H */
