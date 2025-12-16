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

#ifndef __VENDOR_FLAGCHIP_CHIPS_FC7300_HARDWARE_FC7300_FMC_H
#define __VENDOR_FLAGCHIP_CHIPS_FC7300_HARDWARE_FC7300_FMC_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>
#include <hardware/fc7300_memorymap.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* FMC Register Offsets *****************************************************/

#define FC7300_FAPC0_OFFSET             0x0000  /* Flash Access Port Control 0 */
#define FC7300_FAPC1_OFFSET             0x0004  /* Flash Access Port Control 1 */
#define FC7300_FAPC2_OFFSET             0x0008  /* Flash Access Port Control 1 */
#define FC7300_FEEC_OFFSET              0x0010  /* Flash ECC Error Control */
#define FC7300_FEIPC_OFFSET             0x0018  /* Flash ECC Inject Position Control */
#define FC7300_FPESA_L_OFFSET           0x0300  /* Flash Program Erase Start Address Logical */
#define FC7300_FPESA_P_OFFSET           0x0304  /* Flash Program Erase Start Address Physical */
#define FC7300_FB_FPELCK0_OFFSET        0x0340  /* Flash Block 1 Fine Program Erase Lock */
#define FC7300_FB_FPELCK1_OFFSET        0x0344  /* Flash Block 2 Fine Program Erase Lock */
#define FC7300_FB_FPELCK2_OFFSET        0x0348  /* Flash Block 3 Fine Program Erase Lock */
#define FC7300_FB_FPELCK3_OFFSET        0x034C  /* Flash Block 4 Fine Program Erase Lock */
#define FC7300_FB_FPELCK4_OFFSET        0x0350  /* Flash Block n Fine Program Erase Lock */
#define FC7300_FN_FPELCK_OFFSET         0x0358  /* Flash NVR Fine Program Erase Lock */
#define FC7300_FB_CPELCK0_OFFSET        0x035c  /* Flash Block 1 Coarse Program Erase Lock */
#define FC7300_FB_CPELCK1_OFFSET        0x0360  /* Flash Block 2 Coarse Program Erase Lock */
#define FC7300_FB_CPELCK2_OFFSET        0x0364  /* Flash Block 3 Coarse Program Erase Lock */
#define FC7300_FB_CPELCK3_OFFSET        0x0368  /* Flash Block 4 Coarse Program Erase Lock */

/* FMC Register Addresses ***************************************************/

#define FC7300_FAPC0                    (FC7300_FMC_BASE + FC7300_FAPC0_OFFSET)       /* Flash Access Port Control 0 */
#define FC7300_FAPC1                    (FC7300_FMC_BASE + FC7300_FAPC1_OFFSET)       /* Flash Access Port Control 1 */
#define FC7300_FAPC2                    (FC7300_FMC_BASE + FC7300_FAPC2_OFFSET)       /* Flash Access Port Control 2 */
#define FC7300_FEEC                     (FC7300_FMC_BASE + FC7300_FEEC_OFFSET)        /* Flash ECC Error Control */
#define FC7300_FEIPC                    (FC7300_FMC_BASE + FC7300_FEIPC_OFFSET)       /* Flash ECC Inject Position Control */
#define FC7300_FPESA_L                  (FC7300_FMC_BASE + FC7300_FPESA_L_OFFSET)     /* Flash Program Erase Start Address Logical */
#define FC7300_FPESA_P                  (FC7300_FMC_BASE + FC7300_FPESA_P_OFFSET)     /* Flash Program Erase Start Address Physical */
#define FC7300_FB_FPELCK0               (FC7300_FMC_BASE + FC7300_FB_FPELCK0_OFFSET)  /* Flash Block 1 Fine Program Erase Lock */
#define FC7300_FB_FPELCK1               (FC7300_FMC_BASE + FC7300_FB_FPELCK1_OFFSET)  /* Flash Block 2 Fine Program Erase Lock */
#define FC7300_FB_FPELCK2               (FC7300_FMC_BASE + FC7300_FB_FPELCK2_OFFSET)  /* Flash Block 3 Fine Program Erase Lock */
#define FC7300_FB_FPELCK3               (FC7300_FMC_BASE + FC7300_FB_FPELCK3_OFFSET)  /* Flash Block 4 Fine Program Erase Lock */
#define FC7300_FB_FPELCK4               (FC7300_FMC_BASE + FC7300_FB_FPELCK4_OFFSET)  /* Flash Block n Fine Program Erase Lock */
#define FC7300_FN_FPELCK                (FC7300_FMC_BASE + FC7300_FN_FPELCK_OFFSET )  /* Flash NVR Fine Program Erase Lock */
#define FC7300_FB_CPELCK0               (FC7300_FMC_BASE + FC7300_FB_CPELCK0_OFFSET)  /* Flash Block 1 Coarse Program Erase Lock */
#define FC7300_FB_CPELCK1               (FC7300_FMC_BASE + FC7300_FB_CPELCK1_OFFSET)  /* Flash Block 2 Coarse Program Erase Lock */
#define FC7300_FB_CPELCK2               (FC7300_FMC_BASE + FC7300_FB_CPELCK2_OFFSET)  /* Flash Block 3 Coarse Program Erase Lock */
#define FC7300_FB_CPELCK3               (FC7300_FMC_BASE + FC7300_FB_CPELCK3_OFFSET)  /* Flash Block 4 Coarse Program Erase Lock */

/* FMC_FAPC0 Bit Definitions ***********************************************/

#define FMC_FAPC0_DBPEN_MASK            (1 << 5U) /* Bit  5: Data Bus Prefetch Enable */
#define FMC_FAPC0_CBPEN_MASK            (1 << 4U) /* Bit  4: Code Bus Prefetch Enable */
#define FMC_FAPC0_DBBEN_MASK            (1 << 1U) /* Bit  1: Data Bus Read Buffer Enable */
#define FMC_FAPC0_CBBEN_MASK            (1 << 0U) /* Bit  0: Code Bus Read Buffer Enable */

/* FMC_FAPC1 Bit Definitions ***********************************************/

#define FMC_FAPC1_DBPEN_MASK            (1 << 5U) /* Bit  5: Data Bus Prefetch Enable */
#define FMC_FAPC1_CBPEN_MASK            (1 << 4U) /* Bit  4: Code Bus Prefetch Enable */
#define FMC_FAPC1_DBBEN_MASK            (1 << 1U) /* Bit  1: Data Bus Read Buffer Enable */
#define FMC_FAPC1_CBBEN_MASK            (1 << 0U) /* Bit  0: Code Bus Read Buffer Enable */

/* FMC_FEEC Bit Definitions ************************************************/

#define FMC_FEEC_DES_MASK               (1 << 0U) /* Bit  0: Data Error Supression */
#define FMC_FEEC_DDBEE_MASK             (1 << 7U) /* Bit  7: Disable Double-Bit ECC Error Exception */

#endif /* __VENDOR_FLAGCHIP_CHIPS_FC7300_HARDWARE_FC7300_FMC_H */
