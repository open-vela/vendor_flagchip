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

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>

#include <stdint.h>
#include <fixedmath.h>
#include <assert.h>
#include <errno.h>

#include <nuttx/userspace.h>

#include "mpu.h"
#include <arch/barriers.h>

#include "fc7300_config.h"
#include "fc7300_mpu.h"
#include "arm_internal.h"

#include <arch/board/board.h> /* Include last:  has dependencies */

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#ifndef CONFIG_ARMV7M_DCACHE
  /*  With Dcache off:
   *  Cacheable (MPU_RASR_C) and Bufferable (MPU_RASR_B) needs to be off
   */
#  undef  MPU_RASR_B
#  define MPU_RASR_B    0
#  define RASR_B_VALUE  0
#  define RASR_C_VALUE  0
#else
#  ifndef CONFIG_ARMV7M_DCACHE_WRITETHROUGH
  /*  With Dcache on:
   *  Cacheable (MPU_RASR_C) and Bufferable (MPU_RASR_B) needs to be on
   */
#  define RASR_B_VALUE  MPU_RASR_B
#  define RASR_C_VALUE  MPU_RASR_C

#  else
  /*  With Dcache in WRITETHROUGH Bufferable (MPU_RASR_B)
   * needs to be off, except for FLASH for alignment leniency
   */
#  define RASR_B_VALUE  0
#  define RASR_C_VALUE  MPU_RASR_C
#  endif
#endif

#define FC7300_ROM_BASE_ADDR        0x04800000
#define FC7300_ROM_SIZE             0x20000
#define FC7300_PERIPHERAL_BASE_ADDR 0x40000000
#define FC7300_PERIPHERAL_SIZE      0x2000000

#define SIZE_8KB                    0x2000
#define SIZE_16KB                   0x4000
#define SIZE_64KB                   0x10000
#define SIZE_128KB                  0x20000
#define SIZE_256KB                  0x40000

/****************************************************************************
 * Private Types
 ****************************************************************************/

extern uint8_t _pflash_start[];
extern uint8_t _pflash_end[];
extern uint8_t _dflash_start[];
extern uint8_t _dflash_end[];
extern uint8_t _itcm_start[];
extern uint8_t _itcm_end[];
extern uint8_t _dtcm_start[];
extern uint8_t _dtcm_end[];
extern uint8_t _sram_shr_start[];
extern uint8_t _sram_shr_end[];

#if defined(CONFIG_BUILD_FLAT)
extern uint8_t _sram_start[];
extern uint8_t _sram_end[];
#else
extern uint8_t _ksram_start[];
extern uint8_t _ksram_end[];
extern uint8_t _dtcm_user_stack_start[];
extern uint8_t _dtcm_user_stack_end[];
extern uint8_t _usram_start[];
extern uint8_t _usram_end[];
extern uint8_t _vram_start[];
extern uint8_t _vram_end[];
extern uint8_t _coredump_start[];
extern uint8_t _coredump_end[];
extern uint8_t _standby_ram_start[];
extern uint8_t _boot_shr_end[];
extern uint8_t _core0_kcalib_start[];
extern uint8_t _itcm_heap_start[];
extern uint8_t _itcm_heap_size[];

static unsigned int g_dtcm_mpu_region;

#endif

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: fc7300_mpuinit
 *
 * Description:
 *   Configure the MPU to for different RAM region allocation.
 *
 ****************************************************************************/

#if defined(CONFIG_ARCH_USE_MPU)
void fc7300_mpuinit(void)
{
  uint32_t regval;
  uint32_t region;

  /* Show MPU information */

  mpu_showtype();

#ifdef CONFIG_ARMV7M_DCACHE
  /* Memory barrier */

  UP_DMB();

#endif

  /* Reset MPU if enabled */

  mpu_reset();

  /* ARM erratum 1013783 Workaround */

  region = mpu_allocregion();
  DEBUGASSERT(region == 0);

  /* Select the region */

  putreg32(region, MPU_RNR);

  /* Select the region base address */

  putreg32(0 | MPU_RBAR_VALID, MPU_RBAR);

  /* The configure the region */

#if defined(CONFIG_BUILD_FLAT)
  regval = MPU_RASR_ENABLE        | /* Enable region  */
           MPU_RASR_SIZE_LOG2(32) | /* entire memory */
           MPU_RASR_TEX_NOR       | /* Normal        */
           MPU_RASR_AP_RWRW       | /* P:RW   U:RW */
           MPU_RASR_XN;             /* Execute-never to prevent instruction fetch */
  putreg32(regval, MPU_RASR);

  mpu_configure_region((uintptr_t)_pflash_start,
                       _pflash_end - _pflash_start,
                       MPU_RASR_TEX_SO   | /* Normal             */
                       RASR_C_VALUE      | /* Cacheable          */
                       MPU_RASR_B        | /* Bufferable         */
                                           /* Non-Shareable      */
                       MPU_RASR_AP_RORO);  /* P:RO   U:RO
                                            * Instruction access */

  mpu_configure_region((uintptr_t)_dflash_start,
                       _dflash_end - _dflash_start,
                       MPU_RASR_TEX_NOR  | /* Normal             */
                                           /* Non-Cacheable      */
                                           /* Non-Bufferable     */
                                           /* Non-Shareable      */
                       MPU_RASR_AP_RORO);  /* P:RO   U:RO
                                            * Instruction access */

  mpu_configure_region((uintptr_t)_itcm_start,
                       _itcm_end - _itcm_start,
                       MPU_RASR_TEX_NOR  | /* Normal             */
                                           /* Non-Cacheable      */
                                           /* Non-Bufferable     */
                                           /* Non-Shareable      */
                       MPU_RASR_AP_RWRW);  /* P:RW   U:RW
                                            * Instruction access */

  mpu_configure_region((uintptr_t)_dtcm_start,
                       _dtcm_end - _dtcm_start,
                       MPU_RASR_TEX_NOR  | /* Normal             */
                                           /* Non-Cacheable      */
                                           /* Non-Bufferable
                                            * Non-Shareable      */
                       MPU_RASR_AP_RWRW);  /* P:RW   U:RW
                                            * Instruction access */

#if CONFIG_CPU_COREID == 1
  /* Core1 is actually allocated 112KB of SRAM with a starting address of 0x21044000.
     Based on these two pieces of data, the mpu_modify_region function calculates an MPU region
     with a starting address of 0x21040000 and a size of 128KB. This exceeds the size of
     core1's SRAM by 16KB. Therefore, it's necessary to set this extra 16KB as inaccessible
     through an additional region. This is the origin of the subtraction operation and the 0x4000 */

  mpu_configure_region((uintptr_t)_sram_start,
                       _sram_end - _sram_start,
                       MPU_RASR_TEX_SO   | /* Normal             */
                       RASR_C_VALUE      | /* Cacheable          */
                                           /* Non-Bufferable     */
                                           /* Non-Shareable      */
                       MPU_RASR_AP_RWRW);  /* P:RW   U:RW
                                            * Instruction access */

  mpu_configure_region((uintptr_t)((uint32_t)_sram_start - 0x4000),
                      0x4000,
                      MPU_RASR_TEX_NOR   | /* Normal             */
                                           /* Non-Cacheable      */
                                           /* Non-Bufferable     */
                                           /* Non-Shareable      */
                      MPU_RASR_AP_NONO);   /* P:None   U:None
                                            * Instruction access */
# else
  /* Core0 has been allocated 272KB of RAM. However, due to the MPU alignment requirements,
     we can't use this as a single continuous block. Instead, we need to split it into two
     separate parts:
          256KB (which is 0x40000 in hexadecimal)
          16KB (which is 0x4000 in hexadecimal)
     This division is necessary to comply with the MPU's alignment rules, which typically
     require memory regions to be sized in powers of 2. That's why we see these specific
     hexadecimal values - 0x40000 and 0x4000 - representing the two parts of the allocated RAM */

  mpu_configure_region((uintptr_t)_sram_start,
                       0x40000,
                       MPU_RASR_TEX_SO   | /* Normal             */
                       RASR_C_VALUE      | /* Cacheable          */
                       MPU_RASR_B        | /* Bufferable         */
                                           /* Non-Shareable      */
                       MPU_RASR_AP_RWRW);  /* P:RW   U:RW
                                            * Instruction access */

  mpu_configure_region((uintptr_t)(_sram_start + 0x40000),
                       0x4000,
                       MPU_RASR_TEX_SO   | /* Normal             */
                       RASR_C_VALUE      | /* Cacheable          */
                       MPU_RASR_B        | /* Bufferable         */
                                           /* Non-Shareable      */
                       MPU_RASR_AP_RWRW);  /* P:RW   U:RW
                                            * Instruction access */
#endif

  mpu_configure_region((uintptr_t)_sram_shr_start,
                       _sram_shr_end - _sram_shr_start,
                       MPU_RASR_TEX_NOR  | /* Normal   */
                                           /* Non-Cacheable      */
                                           /* Non-Bufferable     */
                                           /* Shareable          */
                       MPU_RASR_AP_RWRW);  /* P:RW   U:RW
                                            * Instruction access */

  mpu_configure_region((uintptr_t)FC7300_ROM_BASE_ADDR,
                       FC7300_ROM_SIZE,
                       MPU_RASR_TEX_NOR  | /* Normal             */
                                           /* Non-Cacheable      */
                                           /* Non-Bufferable     */
                                           /* Non-Shareable      */
                       MPU_RASR_AP_RORO);  /* P:RO   U:RO
                                            * Instruction access */

  mpu_configure_region((uintptr_t)FC7300_PERIPHERAL_BASE_ADDR,
                       FC7300_PERIPHERAL_SIZE,
                       MPU_RASR_TEX_DEV  | /* Device             */
                                           /* Non-Cacheable      */
                                           /* Non-Bufferable     */
                                           /* Non-Shareable      */
                       MPU_RASR_AP_RWRW);  /* P:RW   U:RW
                                            * Instruction access */
#elif defined(CONFIG_BUILD_PROTECTED)
  regval = MPU_RASR_ENABLE        | /* Enable region    */
           MPU_RASR_SIZE_LOG2(32) | /* entire memory    */
           MPU_RASR_TEX_NOR       | /* Normal           */
                                    /* Non-Cacheable    */
                                    /* Non-Bufferable   */
                                    /* Non-Shareable    */
           MPU_RASR_AP_RORO       | /* P:RO   U:RO      */
           MPU_RASR_XN;             /* Execute-never to prevent instruction fetch */
  putreg32(regval, MPU_RASR);

  mpu_configure_region((uintptr_t)_pflash_start,
                       _pflash_end - _pflash_start,
                       MPU_RASR_TEX_SO   | /* Normal             */
                       RASR_C_VALUE      | /* Cacheable          */
                       MPU_RASR_B        | /* Bufferable         */
                                           /* Non-Shareable      */
                       MPU_RASR_AP_RORO);  /* P:RO   U:RO        */
                                           /* Instruction access */

  mpu_configure_region((uintptr_t)_itcm_start,
                       _itcm_end - _itcm_start,
                       MPU_RASR_TEX_NOR  | /* Normal             */
                                           /* Non-Cacheable      */
                                           /* Non-Bufferable     */
                                           /* Non-Shareable      */
                       MPU_RASR_AP_RORO);  /* P:RO   U:RO        */
                                           /* Instruction access */

  mpu_configure_region((uintptr_t)_vram_start,
                       _vram_end - _vram_start,
                       MPU_RASR_TEX_NOR  | /* Normal             */
                                           /* Non-Cacheable      */
                                           /* Non-Bufferable     */
                                           /* Non-Shareable      */
                       MPU_RASR_AP_RWRO);  /* P:RW   U:RO        */
                                           /* Instruction access */

  mpu_configure_region((uintptr_t)_itcm_heap_start,
                       (size_t)_itcm_heap_size,
                       MPU_RASR_TEX_NOR  | /* Normal             */
                                           /* Non-Cacheable      */
                                           /* Non-Bufferable     */
                                           /* Non-Shareable      */
                       MPU_RASR_AP_RWNO  | /* P:RW   U:None      */
                       MPU_RASR_XN);      /* Execute-never to prevent instruction fetch */

  g_dtcm_mpu_region = mpu_configure_region((uintptr_t)_dtcm_start,
                       _dtcm_end - _dtcm_start,
                       MPU_RASR_TEX_NOR  | /* Normal             */
                                           /* Non-Cacheable      */
                                           /* Non-Bufferable     */
                                           /* Non-Shareable      */
#  ifdef CONFIG_ARCH_STACK_PROTECT
                       MPU_RASR_AP_RWRO  | /* P:RW   U:RO        */
#  else
                       MPU_RASR_AP_RWRW  | /* P:RW   U:RW        */
#  endif
                       MPU_RASR_XN);       /* Execute-never to prevent instruction fetch */

  mpu_configure_region((uintptr_t)_standby_ram_start,
                       SIZE_128KB,
                       MPU_RASR_TEX_NOR  | /* Normal             */
                                           /* Non-Cacheable      */
                                           /* Non-Bufferable     */
                                           /* Non-Shareable      */
                       MPU_RASR_AP_RWRW  | /* P:RW   U:RW        */
                       MPU_RASR_XN);       /* Execute-never to prevent instruction fetch */

  mpu_configure_region((uintptr_t)_core0_kcalib_start,
                       _boot_shr_end - _core0_kcalib_start,
                       MPU_RASR_TEX_NOR  | /* Normal             */
                                           /* Non-Cacheable      */
                                           /* Non-Bufferable     */
                                           /* Non-Shareable      */
                       MPU_RASR_AP_RWRO  | /* P:RW   U:RW        */
                       MPU_RASR_XN);       /* Execute-never to prevent instruction fetch */
#if CONFIG_CPU_COREID == 1
  mpu_configure_region((uintptr_t)_ksram_start,
                       _ksram_end - _ksram_start,
                       MPU_RASR_TEX_SO   | /* Normal             */
                       RASR_C_VALUE      | /* Cacheable          */
                                           /* Non-Bufferable     */
                                           /* Non-Shareable      */
                       MPU_RASR_AP_RWNO  | /* P:RW   U:None      */
                       MPU_RASR_XN);       /* Execute-never to prevent instruction fetch */

  mpu_configure_region((uintptr_t)_usram_start,
                       _usram_end - _usram_start,
                       MPU_RASR_TEX_SO   | /* Normal             */
                       RASR_C_VALUE      | /* Cacheable          */
                                           /* Non-Bufferable     */
                                           /* Non-Shareable      */
                       MPU_RASR_AP_RWRW  | /* P:RW   U:RW        */
                       MPU_RASR_XN);       /* Execute-never to prevent instruction fetch */
#else
  mpu_configure_region((uintptr_t)_coredump_start,
                       SIZE_256KB,
                       MPU_RASR_TEX_SO   | /* Normal             */
                       RASR_C_VALUE      | /* Cacheable          */
                       MPU_RASR_B        | /* Bufferable         */
                                           /* Non-Shareable      */
                       MPU_RASR_AP_RWNO  | /* P:RW   U:None      */
                       MPU_RASR_XN);       /* Execute-never to prevent instruction fetch */

  mpu_configure_region((uintptr_t)_usram_start,
                       _usram_end - _usram_start,
                       MPU_RASR_TEX_SO   | /* Normal             */
                       RASR_C_VALUE      | /* Cacheable          */
                       MPU_RASR_B        | /* Bufferable         */
                                           /* Non-Shareable      */
                       MPU_RASR_AP_RWRW  | /* P:RW   U:RW        */
                       MPU_RASR_XN);       /* Execute-never to prevent instruction fetch */
#endif
  mpu_configure_region((uintptr_t)_coredump_start,
                       _coredump_end - _coredump_start,
                       MPU_RASR_TEX_NOR  | /* Normal             */
                                           /* Non-Cacheable      */
                                           /* Non-Bufferable     */
                                           /* Non-Shareable      */
                       MPU_RASR_AP_RWRO  | /* P:RW   U:RO        */
                       MPU_RASR_XN);       /* Execute-never to prevent instruction fetch */

  mpu_configure_region((uintptr_t)FC7300_ROM_BASE_ADDR,
                       FC7300_ROM_SIZE,
                       MPU_RASR_TEX_NOR  | /* Normal             */
                                           /* Non-Cacheable      */
                                           /* Non-Bufferable     */
                                           /* Non-Shareable      */
                       MPU_RASR_AP_RORO);  /* P:RO   U:RO        */
                                           /* Instruction access */

  mpu_configure_region((uintptr_t)FC7300_PERIPHERAL_BASE_ADDR,
                       FC7300_PERIPHERAL_SIZE,
                       MPU_RASR_TEX_DEV  | /* Device             */
                                           /* Non-Cacheable      */
                                           /* Non-Bufferable     */
                                           /* Non-Shareable      */
                       MPU_RASR_AP_RWRO  | /* P:RW   U:RO        */
                       MPU_RASR_XN);       /* Execute-never to prevent instruction fetch */
#endif

  /* Then enable the MPU */
  mpu_control(true, true, true);
}

#ifdef CONFIG_ARCH_KSTACK_PROTECT
void os_kstack_protect(void)
{
  mpu_modify_region(g_dtcm_mpu_region,
                    (uintptr_t)_dtcm_user_stack_start,
                    _dtcm_end - _dtcm_user_stack_start,
                    MPU_RASR_TEX_NOR  | /* Normal             */
                                        /* Non-Cacheable      */
                                        /* Non-Bufferable     */
                                        /* Non-Shareable      */
                    MPU_RASR_AP_RWRO  | /* P:RW   U:RO        */
                    MPU_RASR_XN);       /* Execute-never to prevent instruction fetch */
}
#endif

#endif
