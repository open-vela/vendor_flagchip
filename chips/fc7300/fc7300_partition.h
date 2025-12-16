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

#ifndef __VENDOR_FLAGCHIP_CHIPS_FC7300_FC7300_PARTITION_H
#define __VENDOR_FLAGCHIP_CHIPS_FC7300_FC7300_PARTITION_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>

#include <sys/types.h>
#include <stdint.h>
#include <nuttx/mtd/mtd.h>

#ifndef __ASSEMBLY__

#undef EXTERN
#if defined(__cplusplus)
#define EXTERN extern "C"
extern "C"
{
#else
#define EXTERN extern
#endif

#define FC7300_PFLASH_A_BANK0_SIZE         (CONFIG_KFLASH_A_BANK0_SIZE*1024)

#define FC7300_PFLASH_OTAVER_SIZE          0x2000

#define FC7300_PFLASH_A_AP0_OFFSET         0x040000
#define FC7300_PFLASH_A_OTAVER1_OFFSET     0x1FE000
#define FC7300_PFLASH_B_OTAVER1_OFFSET     0x3FE000
#define FC7300_PFLASH_B_OTAVER2_OFFSET     0x7F6000

#define FC7300_PFLASH_A_TSW_OFFSET         0x400000
#define FC7300_PFLASH_B_TSW_OFFSET         0x600000
#define FC7300_PFLASH_TSW_SIZE             0x40000

#define FC7300_DFLASH_NVS_SIZE             0x20000
#define FC7300_DFLASH_NVS_OFFSET           0x00000

#define FC7300_DFLASH_MANUFACTURY_DATA_SIZE   0xA000
#define FC7300_DFLASH_MANUFACTURY_DATA_OFFSET 0x22000

#define FC7300_DFLASH_TRAPINFO_SIZE        0x4000
#define FC7300_DFLASH_TRAPINFO_OFFSET      0x2C000

#define FC7300_DFLASH_BSWLOG_SIZE           0x4000
#define FC7300_DFLASH_BSWLOG_OFFSET         0x30000

#define FC7300_NVR_TEST_OFFSET              0x0000
#define FC7300_NVR_TEST_SIZE                0x2000

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

/****************************************************************************
 * Name: fc7300_partition_init
 *
 * Description:
 *   Initialize FC7300 partition. Read partition information
 *   and use these data for creating MTD.
 *
 * Input Parameters:
 *   None
 *
 * Returned Value:
 *   0 if success or a negative value if fail.
 *
 ****************************************************************************/

void fc7300_partition_init(void);

#ifdef __cplusplus
}
#endif
#undef EXTERN

#endif /* __ASSEMBLY__ */
#endif /* __VENDOR_FLAGCHIP_CHIPS_FC7300_FC7300_PARTITION_H */
