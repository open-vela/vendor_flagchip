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

#ifndef __VENDOR_FLAGCHIP_CHIPS_FC7300_FC7300_MULTICORE_H
#define __VENDOR_FLAGCHIP_CHIPS_FC7300_FC7300_MULTICORE_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>
#include <nuttx/arch.h>
#include <stdint.h>
#include <errno.h>
#include "arm_internal.h"

#include "hardware/fc7300_cpm.h"
#include "hardware/fc7300_scm.h"
#include "hardware/fc7300_rgm.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#define FC7300_CPM_MISCR_BASE_H      ((FC7300_CPM_BASE | FC7300_CPM_MISCR_OFFSET) >> 16)
#define FC7300_CPM_MISCR_BASE_L      ((FC7300_CPM_BASE | FC7300_CPM_MISCR_OFFSET) & 0xFFFF)
#define FC7300_CPM_LOCK_MISCR_OFFSET (FC7300_CPM_LOCK_OFFSET - FC7300_CPM_MISCR_OFFSET)

/****************************************************************************
 * Private Data
 ****************************************************************************/

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: fc7300_cpu_index
 *
 * Description:
 *   Return an index in the range of 0 through MAX_CPU_NUM that
 *   corresponds to the currently executing CPU.
 *
 ****************************************************************************/

static inline int fc7300_cpu_index(void)
{
  register int CoreId;

  __asm volatile(
    "movw  r0, %[miscr_base_l]                                       \n"
    "movt  r0, %[miscr_base_h]                                       \n"
    "ldr   %[core_id], [r0]                                          \n"
    "add   r0, r0, %[lock_base_offset]                               \n"
    "ldr   r0, [r0]                                                  \n"
    "ubfx  %[core_id], %[core_id], %[cpu_id_shift], %[cpu_id_len]    \n"
    : [core_id]"=r"(CoreId)
    : [miscr_base_l]"i"(FC7300_CPM_MISCR_BASE_L),
      [miscr_base_h]"i"(FC7300_CPM_MISCR_BASE_H),
      [lock_base_offset]"i"(FC7300_CPM_LOCK_MISCR_OFFSET),
      [cpu_id_shift]"i"(FC7300_CPM_MISCR_CPU_ID_SHIFT),
      [cpu_id_len]"i"(FC7300_CPM_MISCR_CPU_ID_WIDTH)
    : "r0"
  );

  return CoreId;
}

/****************************************************************************
 * Name: fc7300_cpu_start
 *
 * Description:
 *   Start the selected CPU.
 *
 * Input Parameters:
 *   cpu - The index of the CPU being started.  This will be a numeric
 *         value in the range of one to MAX_CPU_NUM.
 *         (CPU 0 is already active)
 *
 * Returned Value:
 *   Zero on success; a negated errno value on failure.
 *
 ****************************************************************************/

static inline int fc7300_cpu_start(int cpu)
{
#if (defined(CONFIG_CORE1_BOOTADDR) || defined(CONFIG_CORE2_BOOTADDR))
  uint32_t regval;
#endif
  int ret = OK;

  switch (cpu)
    {
#ifdef CONFIG_CORE1_BOOTADDR
      case 1:
        regval  = getreg32(FC7300_RGM_C1_CFG);
        regval &= ~(RGM_C1_CFG_C1_SWRST_EN | RGM_C1_CFG_C1_INTM_EN   |
                    RGM_C1_CFG_C1_WDOG_EN  | RGM_C1_CFG_C1_SYSRST_EN |
                    RGM_C1_CFG_C1_LOCKUP_EN);
        regval |=
#ifdef CONFIG_FC7300_SWRST_SYSTEM
                  RGM_C1_CFG_C1_SWRST_EN |
#endif
#ifdef CONFIG_FC7300_INTM_STSTEM
                  RGM_C1_CFG_C1_INTM_EN |
#endif
#ifdef CONFIG_FC7300_WDOG_SYSTEM
                  RGM_C1_CFG_C1_WDOG_EN |
#endif
#ifdef CONFIG_FC7300_SYSRST_SYSTEM
                  RGM_C1_CFG_C1_SYSRST_EN |
#endif
#ifdef CONFIG_FC7300_LOCKUP_SYSTEM
                  RGM_C1_CFG_C1_LOCKUP_EN |
#endif
                  0;
        putreg32(regval, FC7300_RGM_C1_CFG);

        regval = getreg32(FC7300_SCM_CPU1VTOR);
        regval = (regval & (~(uint32_t)SCM_CPU1VTOR_CPU1_INIT_VECTOR_MASK)) |
                 SCM_CPU1VTOR_CPU1_INIT_VECTOR(CONFIG_CORE1_BOOTADDR >> 7);
        putreg32(regval, FC7300_SCM_CPU1VTOR);

        regval = getreg32(FC7300_SCM_CORE_HOLD);
        regval &= ~((uint32_t)SCM_CORE_HOLD_CPU1_CORE_HOLD_MASK);
        putreg32(regval, FC7300_SCM_CORE_HOLD);

        putreg32(RGM_C1_RLS_C1_RELEASE, FC7300_RGM_C1_RLS);
        break;
#endif

#ifdef CONFIG_CORE2_BOOTADDR
      case 2:
        regval  = getreg32(FC7300_RGM_C2_CFG);
        regval &= ~(RGM_C2_CFG_C2_SWRST_EN | RGM_C2_CFG_C2_INTM_EN   |
                    RGM_C2_CFG_C2_WDOG_EN  | RGM_C2_CFG_C2_SYSRST_EN |
                    RGM_C2_CFG_C2_LOCKUP_EN);
        regval |=
#ifdef CONFIG_FC7300_SWRST_SYSTEM
                  RGM_C2_CFG_C2_SWRST_EN |
#endif
#ifdef CONFIG_FC7300_INTM_STSTEM
                  RGM_C2_CFG_C2_INTM_EN |
#endif
#ifdef CONFIG_FC7300_WDOG_SYSTEM
                  RGM_C2_CFG_C2_WDOG_EN |
#endif
#ifdef CONFIG_FC7300_SYSRST_SYSTEM
                  RGM_C2_CFG_C2_SYSRST_EN |
#endif
#ifdef CONFIG_FC7300_LOCKUP_SYSTEM
                  RGM_C2_CFG_C2_LOCKUP_EN |
#endif
                  0;
        putreg32(regval, FC7300_RGM_C2_CFG);

        regval = getreg32(FC7300_SCM_CPU2VTOR);
        regval = (regval & (~(uint32_t)SCM_CPU2VTOR_CPU2_INIT_VECTOR_MASK)) |
                 SCM_CPU2VTOR_CPU2_INIT_VECTOR(CONFIG_CORE2_BOOTADDR >> 7);
        putreg32(regval, FC7300_SCM_CPU2VTOR);

        regval = getreg32(FC7300_SCM_CORE_HOLD);
        regval &= ~((uint32_t)SCM_CORE_HOLD_CPU2_CORE_HOLD_MASK);
        putreg32(regval, FC7300_SCM_CORE_HOLD);

        putreg32(RGM_C2_RLS_C2_RELEASE, FC7300_RGM_C2_RLS);
        break;
#endif

      default:
        ret = -EINVAL;
        break;
    }

  return ret;
}

#endif /* __VENDOR_FLAGCHIP_CHIPS_FC7300_FC7300_MULTICORE_H */
