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

#ifndef __VENDOR_FLAGCHIP_CHIPS_FC7300_HARDWARE_FC7300_CSC_H
#define __VENDOR_FLAGCHIP_CHIPS_FC7300_HARDWARE_FC7300_CSC_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>
#include <hardware/fc7300_memorymap.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * CSC Register Offset
 ****************************************************************************/

#define FC7300_CSC_STOP_MODER0_OFFSET   0x0000
#define FC7300_CSC_STOP_REQR0_OFFSET    0x0004
#define FC7300_CSC_STOP_ACKR0_OFFSET    0x0008
#define FC7300_CSC_STOP_MODER1_OFFSET   0x000C
#define FC7300_CSC_STOP_REQR1_OFFSET    0x0010
#define FC7300_CSC_STOP_ACKR1_OFFSET    0x0014
#define FC7300_CSC_STOP_MODER2_OFFSET   0x0018
#define FC7300_CSC_STOP_REQR2_OFFSET    0x001C
#define FC7300_CSC_STOP_ACKR2_OFFSET    0x0020
#define FC7300_CSC_CCM_CFG_OFFSET       0x0024
#define FC7300_CSC_CCM_STATUS_OFFSET    0x0028
#define FC7300_CSC_SCG_MAM_STALL_OFFSET 0x002C
#define FC7300_CSC_CPU_INT_OFFSET       0x0030
#define FC7300_CSC_CLKOUT_CTRL_OFFSET   0x0040
#define FC7300_CSC_AONCLKSR_OFFSET      0x0044
#define FC7300_CSC_PCU_CTRL_OFFSET      0x0048
#define FC7300_CSC_SMU_CTRL0_OFFSET     0x0050
#define FC7300_CSC_SMU_CTRL1_OFFSET     0x0054
#define FC7300_CSC_SMU_CTRL2_OFFSET     0x0058
#define FC7300_CSC_SMU_CTRL3_OFFSET     0x005C
#define FC7300_CSC_SMU_CTRL4_OFFSET     0x0060
#define FC7300_CSC_CMU_CTRL_OFFSET      0x0064

/****************************************************************************
 * CSC0 Register Address
 ****************************************************************************/

#define FC7300_CSC0_STOP_MODER0   (FC7300_CSC0_BASE + FC7300_CSC_STOP_MODER0_OFFSET)    /* STOP Mode Control Register 0 */
#define FC7300_CSC0_STOP_REQR0    (FC7300_CSC0_BASE + FC7300_CSC_STOP_REQR0_OFFSET)     /* STOP Request Register 0 */
#define FC7300_CSC0_STOP_ACKR0    (FC7300_CSC0_BASE + FC7300_CSC_STOP_ACKR0_OFFSET)     /* STOP Acknowledge Register 0 */
#define FC7300_CSC0_STOP_MODER1   (FC7300_CSC0_BASE + FC7300_CSC_STOP_MODER1_OFFSET)    /* STOP Mode Control Register 1 */
#define FC7300_CSC0_STOP_REQR1    (FC7300_CSC0_BASE + FC7300_CSC_STOP_REQR1_OFFSET)     /* STOP Request Register 1 */
#define FC7300_CSC0_STOP_ACKR1    (FC7300_CSC0_BASE + FC7300_CSC_STOP_ACKR1_OFFSET)     /* STOP Acknowledge Register 1 */
#define FC7300_CSC0_STOP_MODER2   (FC7300_CSC0_BASE + FC7300_CSC_STOP_MODER2_OFFSET)    /* STOP Mode Control Register 2 */
#define FC7300_CSC0_STOP_REQR2    (FC7300_CSC0_BASE + FC7300_CSC_STOP_REQR2_OFFSET)     /* STOP Request Register 2 */
#define FC7300_CSC0_STOP_ACKR2    (FC7300_CSC0_BASE + FC7300_CSC_STOP_ACKR2_OFFSET)     /* STOP Acknowledge Register 2 */
#define FC7300_CSC0_CCM0_CFG      (FC7300_CSC0_BASE + FC7300_CSC_CCM_CFG_OFFSET)        /* CCM0 Configuration Register */
#define FC7300_CSC0_CCM0_STATUS   (FC7300_CSC0_BASE + FC7300_CSC_CCM_STATUS_OFFSET)     /* CCM0 Status Register */
#define FC7300_CSC0_SCG_MAM_STALL (FC7300_CSC0_BASE + FC7300_CSC_SCG_MAM_STALL_OFFSET)  /* SCG MAM Stall Request Register */
#define FC7300_CSC0_CPU0_INT      (FC7300_CSC0_BASE + FC7300_CSC_CPU_INT_OFFSET)        /* CPU0 Software Interrupt Register */
#define FC7300_CSC0_CLKOUT_CTRL   (FC7300_CSC0_BASE + FC7300_CSC_CLKOUT_CTRL_OFFSET)    /* Clkout Control Register */
#define FC7300_CSC0_AONCLKSR      (FC7300_CSC0_BASE + FC7300_CSC_AONCLKSR_OFFSET)       /* AON Clock Select Register */
#define FC7300_CSC0_PCU_CTRL      (FC7300_CSC0_BASE + FC7300_CSC_PCU_CTRL_OFFSET)       /* PCU Control Register */
#define FC7300_CSC0_SMU_CTRL0     (FC7300_CSC0_BASE + FC7300_CSC_SMU_CTRL0_OFFSET)      /* FCSMU Control Register 0 */
#define FC7300_CSC0_SMU_CTRL1     (FC7300_CSC0_BASE + FC7300_CSC_SMU_CTRL1_OFFSET)      /* FCSMU Control Register 1 */
#define FC7300_CSC0_SMU_CTRL2     (FC7300_CSC0_BASE + FC7300_CSC_SMU_CTRL2_OFFSET)      /* FCSMU Control Register 2 */
#define FC7300_CSC0_SMU_CTRL3     (FC7300_CSC0_BASE + FC7300_CSC_SMU_CTRL3_OFFSET)      /* FCSMU Control Register 3 */
#define FC7300_CSC0_SMU_CTRL4     (FC7300_CSC0_BASE + FC7300_CSC_SMU_CTRL4_OFFSET)      /* FCSMU Control Register 4 */
#define FC7300_CSC0_CMU_CTRL      (FC7300_CSC0_BASE + FC7300_CSC_CMU_CTRL_OFFSET)       /* CMU Control Register */

/****************************************************************************
 * CSC1 Register Address
 ****************************************************************************/

#define FC7300_CSC1_STOP_MODER0   (FC7300_CSC1_BASE + FC7300_CSC_STOP_MODER0_OFFSET)    /* STOP Mode Control Register 0 */
#define FC7300_CSC1_STOP_REQR0    (FC7300_CSC1_BASE + FC7300_CSC_STOP_REQR0_OFFSET)     /* STOP Request Register 0 */
#define FC7300_CSC1_STOP_ACKR0    (FC7300_CSC1_BASE + FC7300_CSC_STOP_ACKR0_OFFSET)     /* STOP Acknowledge Register 0 */
#define FC7300_CSC1_STOP_MODER1   (FC7300_CSC1_BASE + FC7300_CSC_STOP_MODER1_OFFSET)    /* STOP Mode Control Register 1 */
#define FC7300_CSC1_STOP_REQR1    (FC7300_CSC1_BASE + FC7300_CSC_STOP_REQR1_OFFSET)     /* STOP Request Register 1 */
#define FC7300_CSC1_STOP_ACKR1    (FC7300_CSC1_BASE + FC7300_CSC_STOP_ACKR1_OFFSET)     /* STOP Acknowledge Register 1 */
#define FC7300_CSC1_STOP_MODER2   (FC7300_CSC1_BASE + FC7300_CSC_STOP_MODER2_OFFSET)    /* STOP Mode Control Register 2 */
#define FC7300_CSC1_STOP_REQR2    (FC7300_CSC1_BASE + FC7300_CSC_STOP_REQR2_OFFSET)     /* STOP Request Register 2 */
#define FC7300_CSC1_STOP_ACKR2    (FC7300_CSC1_BASE + FC7300_CSC_STOP_ACKR2_OFFSET)     /* STOP Acknowledge Register 2 */
#define FC7300_CSC1_CCM1_CFG      (FC7300_CSC1_BASE + FC7300_CSC_CCM_CFG_OFFSET)        /* CCM1 Configuration Register */
#define FC7300_CSC1_CCM1_STATUS   (FC7300_CSC1_BASE + FC7300_CSC_CCM_STATUS_OFFSET)     /* CCM1 Status Register */
#define FC7300_CSC1_CPU1_INT      (FC7300_CSC1_BASE + FC7300_CSC_CPU_INT_OFFSET)        /* CPU1 Software Interrupt Register */

/****************************************************************************
 * CSC2 Register Address
 ****************************************************************************/

#define FC7300_CSC2_STOP_MODER0   (FC7300_CSC2_BASE + FC7300_CSC_STOP_MODER0_OFFSET)    /* STOP Mode Control Register 0 */
#define FC7300_CSC2_STOP_REQR0    (FC7300_CSC2_BASE + FC7300_CSC_STOP_REQR0_OFFSET)     /* STOP Request Register 0 */
#define FC7300_CSC2_STOP_ACKR0    (FC7300_CSC2_BASE + FC7300_CSC_STOP_ACKR0_OFFSET)     /* STOP Acknowledge Register 0 */
#define FC7300_CSC2_STOP_MODER1   (FC7300_CSC2_BASE + FC7300_CSC_STOP_MODER1_OFFSET)    /* STOP Mode Control Register 1 */
#define FC7300_CSC2_STOP_REQR1    (FC7300_CSC2_BASE + FC7300_CSC_STOP_REQR1_OFFSET)     /* STOP Request Register 1 */
#define FC7300_CSC2_STOP_ACKR1    (FC7300_CSC2_BASE + FC7300_CSC_STOP_ACKR1_OFFSET)     /* STOP Acknowledge Register 1 */
#define FC7300_CSC2_STOP_MODER2   (FC7300_CSC2_BASE + FC7300_CSC_STOP_MODER2_OFFSET)    /* STOP Mode Control Register 2 */
#define FC7300_CSC2_STOP_REQR2    (FC7300_CSC2_BASE + FC7300_CSC_STOP_REQR2_OFFSET)     /* STOP Request Register 2 */
#define FC7300_CSC2_STOP_ACKR2    (FC7300_CSC2_BASE + FC7300_CSC_STOP_ACKR2_OFFSET)     /* STOP Acknowledge Register 2 */
#define FC7300_CSC2_CCM2_CFG      (FC7300_CSC2_BASE + FC7300_CSC_CCM_CFG_OFFSET)        /* CCM2 Configuration Register */
#define FC7300_CSC2_CCM2_STATUS   (FC7300_CSC2_BASE + FC7300_CSC_CCM_STATUS_OFFSET)     /* CCM2 Status Register */
#define FC7300_CSC2_CPU2_INT      (FC7300_CSC2_BASE + FC7300_CSC_CPU_INT_OFFSET)        /* CPU2 Software Interrupt Register */

/****************************************************************************
 * CSC0 AONCLKSR Register Bitfield Definitions
 ****************************************************************************/

#define CSC0_AONCLKSR_RTCCLKSEL_SHIFT       (12)       /* Bits 10-12: Propagation Segment */
#define CSC0_AONCLKSR_RTCCLKSEL_MASK        (0x03 << CSC0_AONCLKSR_RTCCLKSEL_SHIFT)
#define CSC0_AONCLKSR_RTCCLKSEL(x)          (((x) << CSC0_AONCLKSR_RTCCLKSEL_SHIFT) & CSC0_AONCLKSR_RTCCLKSEL_MASK)

#define CSC0_AONCLKSR_AONCLKSEL_SHIFT       (10)       /* Bits 10-12: Propagation Segment */
#define CSC0_AONCLKSR_AONCLKSEL_MASK        (0x03 << CSC0_AONCLKSR_AONCLKSEL_SHIFT)
#define CSC0_AONCLKSR_AONCLKSEL(x)          (((x) << CSC0_AONCLKSR_AONCLKSEL_SHIFT) & CSC0_AONCLKSR_AONCLKSEL_MASK)

/* TODO */

#endif /* __VENDOR_FLAGCHIP_CHIPS_FC7300_HARDWARE_FC7300_CSC_H */
