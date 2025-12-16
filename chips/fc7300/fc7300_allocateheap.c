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

#include <nuttx/userspace.h>
#include <arch/board/board.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Configuration ************************************************************/

/* Terminology.
 * In the flat build (CONFIG_BUILD_FLAT=y), there is only a
 * single heap access with the standard allocations (malloc/free).  This
 * heap is referred to as the user heap.
 * In the protected build (CONFIG_BUILD_PROTECTED=y) where an MPU is
 * used to protect a region of otherwise flat memory, there will
 * be two allocators:  One that allocates protected (kernel) memory and
 * one that allocates unprotected (user) memory. These are referred to
 * as the kernel and user heaps, respectively.
 *
 * For processor(armv7-a/armv8-a) which has no MPU but does have an MMU.
 * Without an MMU, it cannot support the kernel
 * build (CONFIG_BUILD_KERNEL=y). In that configuration,
 * there would is one kernel heap but multiple user heaps: One per
 * task group. However, in this case, we need only be concerned
 * about initializing the single kernel heap here.
 *
 * Primary RAM:  The Linker script positions the system BLOB's .data and
 * .bss in some RAM.  We refer to that RAM as the primary RAM.  It also
 * holds the IDLE threads stack and any remaining portion of the primary
 * OCRAM is automatically added to the heap.  The linker provided address,
 * ... .sbss, .ebss, .sdat, etc. ...  are expected to lie in the the region
 * defined by the OCRAM configuration settings.
 *
 * Other RAM regions must be selected use configuration options and the
 * start and end of those RAM regions must also be provided in the
 * configuration.  CONFIG_MM_REGIONS must also be set to determined the
 * number of regions to be added to the heap.
 */

/****************************************************************************
 * Public Data
 ****************************************************************************/

extern uint8_t  _ksram_end[];
extern uint8_t  _usram_end[];

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: up_allocate_heap
 *
 * Description:
 *   This function will be called to dynamically set aside the heap region.
 *
 *   - For the normal "flat" build, this function returns the size of the
 *     single heap.
 *   - For the protected build (CONFIG_BUILD_PROTECTED=y) with both kernel-
 *     and user-space heaps (CONFIG_MM_KERNEL_HEAP=y), this function
 *     provides the size of the user-space heap.
 *
 ****************************************************************************/

void up_allocate_heap(void **heap_start, size_t *heap_size)
{
#if defined(CONFIG_BUILD_PROTECTED) && defined(CONFIG_MM_KERNEL_HEAP)

  /* Get the unaligned size and position of the user-space heap.
   * This heap begins after the user-space .bss section.
   */

  uintptr_t ubase = (uintptr_t)USERSPACE->us_bssend;

  DEBUGASSERT(ubase < (uintptr_t)_usram_end);

  /* Return the user-space heap settings */

  *heap_start = (void *)ubase;
  *heap_size  = (uintptr_t)_usram_end - ubase;
#else

  /* Return the heap settings */

  *heap_start = (void *)g_idle_topstack;
  *heap_size  = CONFIG_RAM_END - g_idle_topstack;
#endif
}

/****************************************************************************
 * Name: up_allocate_kheap
 *
 * Description:
 *   For the kernel build (CONFIG_BUILD_PROTECTED/KERNEL=y) with both kernel-
 *   and user-space heaps (CONFIG_MM_KERNEL_HEAP=y), this function allocates
 *   the kernel-space heap.
 *
 ****************************************************************************/
#if defined(CONFIG_BUILD_PROTECTED) && defined(CONFIG_MM_KERNEL_HEAP)
void up_allocate_kheap(void **heap_start, size_t *heap_size)
{
  /* Get the unaligned size and position of the kernel-space heap.
   * This heap begins after the kernel-space idle stack.
   */

  uintptr_t ubase = g_idle_topstack;
  DEBUGASSERT(ubase < (uintptr_t)_ksram_end);

  /* Return the kernel heap settings */

  *heap_start = (void *)ubase;
  *heap_size  = (uintptr_t)_ksram_end - ubase;
}
#endif
