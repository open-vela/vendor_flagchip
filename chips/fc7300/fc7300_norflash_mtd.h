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

#ifndef __VENDOR_FLAGCHIP_CHIPS_FC7300_FC7300_NORFLASH_MTD_H
#define __VENDOR_FLAGCHIP_CHIPS_FC7300_FC7300_NORFLASH_MTD_H

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

/****************************************************************************
 * LOCAL MACROS
 ****************************************************************************/

#define FC7300_FLASH_SECTOR_SIZE             0x2000U

#define FC7300_FLASH_PAGE_SIZE               16

#define FC7300_FLASH_PAGE_MAX_SIZE           0x80U    /* 128 bytes  */

#define FC7300_DFLASH_START_ADDR             0x04000000U

#define FC7300_DFLASH_BLOCK_SIZE             0x00040000U

#define FC7300_DFLASH_SECTOR_COUNT           FC7300_DFLASH_BLOCK_SIZE / FC7300_FLASH_SECTOR_SIZE

#define FC7300_PFLASH_START_ADDR             0x01000000U

#define FC7300_PFLASH_BLOCK_SIZE             0x007F8000U

#define FC7300_PFLASH_BANK_SIZE              0x200000U

#define FC7300_PFLASH_SECTOR_COUNT           FC7300_PFLASH_BLOCK_SIZE / FC7300_FLASH_SECTOR_SIZE

#define FC7300_PFLASH_256KB_SIZE             0x00040000U

#define FC7300_PFLASH_LAST_256KB_START       0x1C0000U

#define FC7300_NVR_START_ADDR                0x04400000U

#define FC7300_NVR_BLOCK_SIZE                0x00002000U

#define FC7300_NVR_SECTOR_COUNT              FC7300_NVR_BLOCK_SIZE / FC7300_FLASH_SECTOR_SIZE

#define MTD_ERASED_STATE                     (0xff)

#define FLASHDRV_ADDR                        0x04810200U

#define STATUS_SUCCESS                       0x001UL
#define STATUS_HVOP                          0x8001UL
#define STATUS_ECC_ERROR                     0x011UL

#define FLASH_ECC_CHECK_MASK                 0x011UL

#define FLASH_ABT_TIMEOUT_VALUE              0x1000000

#ifdef CONFIG_FC7300_DFLASH_INJECT_ECC
#define ECC_TEST_INJECT_DATA1                0x994CD030
#define ECC_TEST_INJECT_DATA2                0x781279F1
#define ECC_TEST_INJECT_DATA3                0x8D36371A
#define ECC_TEST_INJECT_DATA4                0x09FAE313
#define ECC_TEST_INJECT_DATA5                0x0c001551
#endif

#define FC7300_ERASE(x, y, z)                (s_flash_driver_func_header->erase_sector(x, y, z))
#define FC7300_ERASE_NVR(x)                  (s_flash_driver_func_header->erase_nvr(x))
#define FC7300_PEGM(x, y, z)                 (s_flash_driver_func_header->program(x, y, z))
#define FC7300_PEGM_NVR(x, y)                (s_flash_driver_func_header->program_nvr(x, y))
#define FC7300_HV_CHECK()                    (s_flash_driver_func_header->hv_status_check())
#define FC7300_ERASE_CLEAR()                 (s_flash_driver_func_header->erase_sector_clear())
#define FC7300_ERASE_NVR_CLEAR()             (s_flash_driver_func_header->erase_nvr_clear())
#define FC7300_PEGM_CLEAR()                  (s_flash_driver_func_header->program_clear())
#define FC7300_PEGM_NVR_CLEAR()              (s_flash_driver_func_header->program_nvr_clear())
#define FC7300_ENABLE_HOLD(x)                (s_flash_driver_func_header->enable_hold(x))
#define FC7300_DFLASH_STATUS_READ(x)         (s_flash_driver_func_header->read_dflash_status(x))
#define FC7300_DFLASH_STATUS_CLEAR(x)        (s_flash_driver_func_header->clear_dflash_status(x))
#ifdef CONFIG_FC7300_DFLASH_INJECT_ECC
#define FC7300_FLASH_INJECT_ECC(x, y)        (s_flash_driver_func_header->ecc_check_type(x, y))
#define FC7300_FLASH_TEST_MODE_OPEN()        (s_flash_driver_func_header->test_mode_open_type())
#define FC7300_FLASH_TEST_MODE_CLOSE()       (s_flash_driver_func_header->test_mode_close_type())
#endif

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

/****************************************************************************
 * Name: fc7300_norpflash_mtd
 *
 * Description:
 *   Get PFlash MTD.
 *
 * Input Parameters:
 *   None
 *
 * Returned Value:
 *   PFlash MTD pointer.
 *
 ****************************************************************************/

struct mtd_dev_s *fc7300_nordflash_mtd(void);

/****************************************************************************
 * Name: fc7300_nordflash_mtd
 *
 * Description:
 *   Get DFlash MTD.
 *
 * Input Parameters:
 *   None
 *
 * Returned Value:
 *   DFlash MTD pointer.
 *
 ****************************************************************************/

struct mtd_dev_s *fc7300_norpflash_mtd(void);

/****************************************************************************
 * Name: fc7300_nornvr_mtd
 *
 * Description:
 *   Get DFlash MTD.
 *
 * Input Parameters:
 *   None
 *
 * Returned Value:
 *   DFlash MTD pointer.
 *
 ****************************************************************************/

struct mtd_dev_s *fc7300_nornvr_mtd(void);

/****************************************************************************
 * Name: fc7300_pflash_dblecc_rsp_enable
 *
 * Description:
 *   Generated busfault response when a double-bit ECC error occurs.
 *
 * Input Parameters:
 *   None
 *
 * Returned Value:
 *   None.
 *
 ****************************************************************************/

void fc7300_pflash_dblecc_rsp_enable(void);

/****************************************************************************
 * Name: fc7300_pflash_dblecc_rsp_disable
 *
 * Description:
 *   Not generated busfault response when a double-bit ECC error occurs.
 *
 * Input Parameters:
 *   None
 *
 * Returned Value:
 *   None.
 *
 ****************************************************************************/

void fc7300_pflash_dblecc_rsp_disable(void);

/****************************************************************************
 * Name: fc7300_pflash_prefetch_enable
 *
 * Description:
 *   Enable prefetch for PFlash.
 *
 * Input Parameters:
 *   None
 *
 * Returned Value:
 *   None.
 *
 ****************************************************************************/

void fc7300_pflash_prefetch_enable(void);

/****************************************************************************
 * Name: fc7300_pflash_prefetch_disable
 *
 * Description:
 *   Disable prefetch for PFlash.
 *
 * Input Parameters:
 *   None
 *
 * Returned Value:
 *   None.
 *
 ****************************************************************************/

void fc7300_pflash_prefetch_disable(void);

#ifdef __cplusplus
}
#endif
#undef EXTERN

#endif /* __ASSEMBLY__ */
#endif /* __VENDOR_FLAGCHIP_CHIPS_FC7300_FC7300_NORFLASH_MTD_H */
