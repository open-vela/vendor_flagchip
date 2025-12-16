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

#ifndef __VENDOR_FLAGCHIP_BOARDS_FC7300_FC7300F8M_EVB_INCLUDE_MEMORY_LAYOUT_H
#define __VENDOR_FLAGCHIP_BOARDS_FC7300_FC7300F8M_EVB_INCLUDE_MEMORY_LAYOUT_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include "nuttx/config.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Memory Layout of FC7300F8M
 *   ITCM     0x00000000 - 0x0000FFFF 64KB
 *   DTCM     0x20000000 - 0x2001FFFF 128KB
 *   PFLASH_0 0x01000000 - 0x011FFFFF 2MB
 *   PFLASH_1 0x01200000 - 0x013FFFFF 2MB
 *   PFLASH_2 0x01400000 - 0x015FFFFF 2MB
 *   PFLASH_3 0x01600000 - 0x017FFFFF 2MB
 *   DFLASH   0x04000000 - 0x0403FFFF 256KB
 *   SRAM_0   0x21000000 - 0x2101FFFF 128KB
 *   SRAM_1   0x21020000 - 0x2103FFFF 128KB
 *   SRAM_2   0x21040000 - 0x2107FFFF 256KB
 */

#define DFLASH_START 0x04000000
#define DFLASH_SIZE 0x40000 /* 256KB */
#define VRAM_START 0x00000000
#define VRAM_SIZE 0x400 /* 1KB   */
#define ITCM_START 0x00000400
#define ITCM_SIZE 0xFC00 /* 63KB  */
#define ITCM_FUNC_START 0x00000400
#define ITCM_FUNC_SIZE 0x7C00 /* 31KB  */
#define ITCM_HEAP_START 0x00008000
#define ITCM_HEAP_SIZE 0x8000 /* 32KB  */
#define DTCM_START 0x20000000
#define DTCM_SIZE 0x20000 /* 128KB  */
#define DTCM_DATA_START 0x2000C000
#define DTCM_DATA_SIZE 0x14000 /* 80KB  */
#define APP_MANUAL_QM_START 0x21002000
#define APP_MANUAL_QM_SIZE 0x1400 /* 5KB   */
#define BSW_MANUAL_QM_START 0x21003400
#define BSW_MANUAL_QM_SIZE 0x400 /* 1KB   */
#define RTE_QM_START 0x21003800
#define RTE_QM_SIZE 0x2800 /* 10KB  */
#define BSW_MANUAL_ASIL_START 0x2101E000
#define BSW_MANUAL_ASIL_SIZE 0x1400 /* 5KB   */
#define RTE_ASIL_START 0x2101F400
#define RTE_ASIL_SIZE 0x800 /* 2KB   */
#define BOOT_SHARE_START 0x2101FC00
#define BOOT_SHARE_SIZE 0x400 /* 1KB   */
#define COREDUMP_START 0x21020000
#define COREDUMP_SIZE 0x3000 /* 12KB   */
#define NVR_START 0x04400000
#define NVR_SIZE 0x2000 /* 8KB   */
#define BANK0_BOOTLOADER_START 0x01000000
#define BANK0_BOOTLOADER_SIZE 0x40000 /* 256KB */
#define BANK0_CORE0_KFLASH_START 0x01040000
#define BANK0_CORE0_KFLASH_SIZE 0x180000 /* 1536KB*/
#define BANK0_CORE0_UFLASH_START 0x011C0000
#define BANK0_CORE0_UFLASH_SIZE 0x3E000 /* 248KB */
#define BANK0_OTA_START 0x011FE000
#define BANK0_OTA_SIZE 0x2000 /* 8KB   */
#define BANK1_BOOTLOADER_START 0x01200000
#define BANK1_BOOTLOADER_SIZE 0x40000 /* 256KB */
#define BANK1_CORE0_KFLASH_START 0x01240000
#define BANK1_CORE0_KFLASH_SIZE 0x180000 /* 1536KB*/
#define BANK1_CORE0_UFLASH_START 0x013C0000
#define BANK1_CORE0_UFLASH_SIZE 0x40000 /* 256KB */
#define BANK1_OTA_START 0x013FE000
#define BANK1_OTA_SIZE 0x2000 /* 8KB   */
#define BANK2_CORE1_KFLASH_START 0x01400000
#define BANK2_CORE1_KFLASH_SIZE 0x8A000 /* 552KB */
#define BANK2_CORE1_UFLASH_START 0x0148A000
#define BANK2_CORE1_UFLASH_SIZE 0x16C000 /* 1456KB*/
#define BANK2_OTA_START 0x015F6000
#define BANK2_OTA_SIZE 0x2000 /* 8KB   */
#define BANK2_HSM_START 0x015F8000
#define BANK2_HSM_SIZE 0x8000 /* 32KB  */
#define BANK3_CORE1_KFLASH_START 0x01600000
#define BANK3_CORE1_KFLASH_SIZE 0x8A000 /* 552KB */
#define BANK3_CORE1_UFLASH_START 0x0168A000
#define BANK3_CORE1_UFLASH_SIZE 0x176000 /* 1496KB*/
#define BANK3_OTA_START 0x017F6000
#define BANK3_OTA_SIZE 0x2000 /* 8KB   */
#define CORE0_PFLASH_START 0x01000000
#define CORE0_KERNEL_SRAM_START 0x21023000
#define CORE0_KERNEL_SRAM_SIZE 0x35000 /* 212KB */
#define CORE0_USER_SRAM_START 0x21058000
#define CORE0_USER_SRAM_SIZE 0x8000 /* 32KB  */
#define CORE0_CALIB_KERNEL_START 0x21016000
#define CORE0_CALIB_KERNEL_SIZE 0x5000 /* 20KB  */
#define CORE0_CALIB_USER_START 0x21006000
#define CORE0_CALIB_USER_SIZE 0x800 /* 2KB   */
#define CORE0_STANDBY_RAM_START 0x21000000
#define CORE0_STANDBY_RAM_SIZE 0xE00 /* 3.5KB   */
#define CORE0_STANDBY_RAM_FIX_START 0x21000E00
#define CORE0_STANDBY_RAM_FIX_SIZE 0x200 /* 0.5KB   */
#define CORE0_DTCM_STACK_START 0x20000000
#define CORE0_DTCM_STACK_SIZE 0x8000 /* 32KB  */
#define CORE0_DTCM_USER_START 0x20008000
#define CORE0_DTCM_USER_SIZE 0x4000 /* 16KB  */
#define CORE1_PFLASH_START 0x01400000
#define CORE1_KERNEL_SRAM_START 0x21060000
#define CORE1_KERNEL_SRAM_SIZE 0x8000 /* 32KB  */
#define CORE1_USER_SRAM_START 0x21068000
#define CORE1_USER_SRAM_SIZE 0x18000 /* 96KB  */
#define CORE1_CALIB_KERNEL_START 0x2101B000
#define CORE1_CALIB_KERNEL_SIZE 0x3000 /* 12KB   */
#define CORE1_CALIB_USER_START 0x21006800
#define CORE1_CALIB_USER_SIZE 0xF800 /* 62KB  */
#define CORE1_STANDBY_RAM_START 0x21001000
#define CORE1_STANDBY_RAM_SIZE 0xE00 /* 3.5KB   */
#define CORE1_STANDBY_RAM_FIX_START 0x21001E00
#define CORE1_STANDBY_RAM_FIX_SIZE 0x200 /* 0.5KB   */
#define CORE1_DTCM_STACK_START 0x20000000
#define CORE1_DTCM_STACK_SIZE 0x4000 /* 16KB  */
#define CORE1_DTCM_USER_START 0x20004000
#define CORE1_DTCM_USER_SIZE 0x8000 /* 32KB  */
#define BOOT_STANDBY_RAM_START 0x21000000
#define BOOT_STANDBY_RAM_SIZE 0xE00 /* 3.5KB   */
#define BOOT_STANDBY_RAM_FIX_START 0x21000E00
#define BOOT_STANDBY_RAM_FIX_SIZE 0x200 /* 0.5KB   */

/****************************************************************************
 * Modify only in cmocka test version
 *
 * Currently, the total size of core0 user space in protected build mode is only 32KB,
 * and the remaining heap is about 20KB. CMOCKA test tasks run in user space,
 * and each cmocka task will request a stack from the user heap.
 * As the number of cmocka tasks increases, the remaining space in the user heap will
 * be insufficient, which will cause cmocka to fail in parsing the test commands
 * (cmocka will match the input commands by compiling regular expressions,
 * and execute the corresponding test cases if the match is successful. insufficient heap
 * will cause the regular matching to fail).
 *
 ****************************************************************************/

#define CMOCKA_CORE0_KERNEL_SRAM_SIZE 0x2D000
#define CMOCKA_CORE0_USER_SRAM_START 0x21050000
#define CMOCKA_CORE0_USER_SRAM_SIZE 0x10000

#if defined(CONFIG_CPU_COREID)
#if (CONFIG_CPU_COREID == 0)
#define PFLASH_START CORE0_PFLASH_START
#define KERNEL_PFLASH_START BANK0_CORE0_KFLASH_START
#define KERNEL_PFLASH_SIZE BANK0_CORE0_KFLASH_SIZE
#define USER_PFLASH_START BANK0_CORE0_UFLASH_START
#define USER_PFLASH_SIZE BANK0_CORE0_UFLASH_SIZE
#define KERNEL_SRAM_START CORE0_KERNEL_SRAM_START
#if defined(CONFIG_TESTING_CMOCKA)
#define KERNEL_SRAM_SIZE CMOCKA_CORE0_KERNEL_SRAM_SIZE
#define USER_SRAM_START CMOCKA_CORE0_USER_SRAM_START
#define USER_SRAM_SIZE CMOCKA_CORE0_USER_SRAM_SIZE
#else
#define KERNEL_SRAM_SIZE CORE0_KERNEL_SRAM_SIZE
#define USER_SRAM_START CORE0_USER_SRAM_START
#define USER_SRAM_SIZE CORE0_USER_SRAM_SIZE
#endif
#define CALIB_KERNEL_START CORE0_CALIB_KERNEL_START
#define CALIB_KERNEL_SIZE CORE0_CALIB_KERNEL_SIZE
#define CALIB_USER_START CORE0_CALIB_USER_START
#define CALIB_USER_SIZE CORE0_CALIB_USER_SIZE
#define STANDBY_RAM_START CORE0_STANDBY_RAM_START
#define STANDBY_RAM_SIZE CORE0_STANDBY_RAM_SIZE
#define STANDBY_RAM_FIX_START CORE0_STANDBY_RAM_FIX_START
#define STANDBY_RAM_FIX_SIZE CORE0_STANDBY_RAM_FIX_SIZE
#define DTCM_STACK_START CORE0_DTCM_STACK_START
#define DTCM_STACK_SIZE CORE0_DTCM_STACK_SIZE
#define DTCM_USER_START CORE0_DTCM_USER_START
#define DTCM_USER_SIZE CORE0_DTCM_USER_SIZE
#elif (CONFIG_CPU_COREID == 1)
#define PFLASH_START CORE1_PFLASH_START
#define KERNEL_PFLASH_START BANK2_CORE1_KFLASH_START
#define KERNEL_PFLASH_SIZE BANK2_CORE1_KFLASH_SIZE
#define USER_PFLASH_START BANK2_CORE1_UFLASH_START
#define USER_PFLASH_SIZE BANK2_CORE1_UFLASH_SIZE
#define KERNEL_SRAM_START CORE1_KERNEL_SRAM_START
#define KERNEL_SRAM_SIZE CORE1_KERNEL_SRAM_SIZE
#define USER_SRAM_START CORE1_USER_SRAM_START
#define USER_SRAM_SIZE CORE1_USER_SRAM_SIZE
#define CALIB_KERNEL_START CORE1_CALIB_KERNEL_START
#define CALIB_KERNEL_SIZE CORE1_CALIB_KERNEL_SIZE
#define CALIB_USER_START CORE1_CALIB_USER_START
#define CALIB_USER_SIZE CORE1_CALIB_USER_SIZE
#define STANDBY_RAM_START CORE1_STANDBY_RAM_START
#define STANDBY_RAM_SIZE CORE1_STANDBY_RAM_SIZE
#define STANDBY_RAM_FIX_START CORE1_STANDBY_RAM_FIX_START
#define STANDBY_RAM_FIX_SIZE CORE1_STANDBY_RAM_FIX_SIZE
#define DTCM_STACK_START CORE1_DTCM_STACK_START
#define DTCM_STACK_SIZE CORE1_DTCM_STACK_SIZE
#define DTCM_USER_START CORE1_DTCM_USER_START
#define DTCM_USER_SIZE CORE1_DTCM_USER_SIZE
#elif (CONFIG_CPU_COREID == -1)
#define STANDBY_RAM_START BOOT_STANDBY_RAM_START
#define STANDBY_RAM_SIZE BOOT_STANDBY_RAM_SIZE
#define STANDBY_RAM_FIX_START BOOT_STANDBY_RAM_FIX_START
#define STANDBY_RAM_FIX_SIZE BOOT_STANDBY_RAM_FIX_SIZE
#endif
#endif

/****************************************************************************
 * The memory layout address and size only for flat build mode
 *
 ****************************************************************************/

#define FLAT_SRAM_SHARE_START 0x2101A000
#define FLAT_SRAM_SHARE_SIZE 0x5C00
#define FLAT_COREDUMP_START 0x21020000
#define FLAT_COREDUMP_SIZE 0x2000
#define FLAT_CORE0_PFLASH_START 0x01040000
#define FLAT_CORE0_PFLASH_SIZE 0x1BE000
#define FLAT_CORE1_PFLASH_START 0x01440000
#define FLAT_CORE1_PFLASH_SIZE 0x1B6000
#define FLAT_CORE0_CALIB_START 0x21002000
#define FLAT_CORE0_CALIB_SIZE 0x8000
#define FLAT_CORE1_CALIB_START 0x2100A000
#define FLAT_CORE1_CALIB_SIZE 0x10000
#define FLAT_BL_PFLASH_START 0x01000000
#define FLAT_BL_PFLASH_SIZE 0x40000
#define FLAT_BL_CALIB_START 0x21002000
#define FLAT_BL_CALIB_SIZE 0x8000

#if defined(CONFIG_CPU_COREID)
#  if (CONFIG_CPU_COREID == 0)
#    define FLAT_PFLASH_START            FLAT_CORE0_PFLASH_START
#    define FLAT_PFLASH_SIZE             FLAT_CORE0_PFLASH_SIZE
#    define FLAT_CALIB_START             FLAT_CORE0_CALIB_START
#    define FLAT_CALIB_SIZE              FLAT_CORE0_CALIB_SIZE
#  elif (CONFIG_CPU_COREID == 1)
#    define FLAT_PFLASH_START            FLAT_CORE1_PFLASH_START
#    define FLAT_PFLASH_SIZE             FLAT_CORE1_PFLASH_SIZE
#    define FLAT_CALIB_START             FLAT_CORE1_CALIB_START
#    define FLAT_CALIB_SIZE              FLAT_CORE1_CALIB_SIZE
#  elif (CONFIG_CPU_COREID == -1)
#    define FLAT_PFLASH_START            FLAT_BL_PFLASH_START
#    define FLAT_PFLASH_SIZE             FLAT_BL_PFLASH_SIZE
#    define FLAT_CALIB_START             FLAT_BL_CALIB_START
#    define FLAT_CALIB_SIZE              FLAT_BL_CALIB_SIZE
#  endif
#endif

#endif /* __VENDOR_FLAGCHIP_BOARDS_FC7300_FC7300F8M_EVB_INCLUDE_MEMORY_LAYOUT_H */
