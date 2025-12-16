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

#include <nuttx/compiler.h>
#include <nuttx/config.h>

#include <assert.h>
#include <debug.h>
#include <stdint.h>

#include <arch/board/board.h>
#include <arch/board/common_inc.h>
#include <arch/board/memory_layout.h>
#include <arch/irq.h>
#include <nuttx/init.h>
#include <nuttx/kmalloc.h>

#include "arm_internal.h"
#include "dwt.h"
#include "nvic.h"

#ifdef CONFIG_FC7300_BOARD_PROFILING
#  include <hardware/fc7300_pin.h>
#endif

#ifdef CONFIG_BUILD_PROTECTED
#  include "fc7300_userspace.h"
#endif

#include "hardware/fc7300_pcc.h"
#include "hardware/fc7300_scm.h"
#include "hardware/fc7300_stcu.h"

#include "fc7300_clockconfig.h"
#include "fc7300_irq.h"
#include "fc7300_lowputc.h"
#include "fc7300_multicore.h"
#include "fc7300_start.h"
#include "fc7300_uart.h"
#include "fc7300_wdog.h"

#ifdef CONFIG_FC7300_PFLASH_PREFETCH
#  include "fc7300_norflash_mtd.h"
#endif

#ifdef CONFIG_FC7300_PROGMEM
#  include "fc7300_progmem.h"
#endif

#ifdef CONFIG_ARCH_USE_MPU
#  include "fc7300_mpu.h"
#endif

#ifdef CONFIG_FC7300_ERM
#include "fc7300_erm.h"
#endif

#ifdef CONFIG_FC7300_FCSMU
#include "fc7300_fcsmu.h"
#endif

#ifdef CONFIG_FC7300_EIM
#include "fc7300_eim.h"
#endif

#ifdef CONFIG_FC7300_CMU
#include "fc7300_cmu.h"
#endif

#ifdef CONFIG_FC7300_PMC
#include "fc7300_pmc.h"
#endif

#ifdef CONFIG_FC7300_STCU
#include "fc7300_stcu.h"
#endif

#ifdef CONFIG_FC7300_TMU
#include "fc7300_tmu.h"
#endif

#ifdef CONFIG_FC7300_MAM
#include "fc7300_mam.h"
#endif

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Memory Map ***************************************************************/

/* 0x0000:0000 - Beginning of the internal FLASH.   Address of vectors.
 *               Mapped as boot memory address 0x0000:0000 at reset.
 * 0x00ff:ffff - End of flash region (assuming the max of 2MiB of FLASH).
 * 0x2000:0000 - Start of internal SRAM and start of .data (_sdata)
 *
 *               The on-chip RAM is split in two regions: SRAM_L and SRAM_U.
 *               The RAM is implemented such that the SRAM_L and SRAM_U
 *               ranges form a contiguous block in the memory map.  Thus, the
 *               actual SRAM start address is SAM_L which some MCU-specific
 *               value in the range 0x1000:0000 and 0x1fff:ffff.  SRAM_U
 *               then always starts at 0x2000:0000

 *             - End of .data (_edata) and start of .bss (_sbss)
 *             - End of .bss (_ebss) and bottom of idle stack
 *             - _ebss + CONFIG_IDLETHREAD_STACKSIZE = end of idle stack,
 *               start of heap. NOTE that the ARM uses a decrement before
 *               store stack so that the correct initial value is the end of
 *               the stack + 4;
 * 0x200f:ffff - End of internal SRAM and end of heap.  The actual end of
 *               SRAM_U will depend on the amount of memory supported by the
 *               MCU/
 *
 * NOTE:  ARM EABI requires 64 bit stack alignment.
 */

#define HEAP_BASE ((uintptr_t)_ebss + CONFIG_IDLETHREAD_STACKSIZE)

/****************************************************************************
 * Name: showprogress
 *
 * Description:
 *   Print a character on the UART to show boot status.
 *
 ****************************************************************************/

#if !defined(CONFIG_NO_FCUART_SERIAL_CONSOLE) && defined(CONFIG_DEBUG_FEATURES)
#  define showprogress(c) fc7300_lowputc(c)
#else
#  define showprogress(c)
#endif

/****************************************************************************
 * Name: FC7300_STCU_SRAM_INI_SEL_ALL
 *
 * Description:
 *   Macro definition for clearing all memory regions with STCU
 *
 ****************************************************************************/

#if defined(CONFIG_BOOT_SHARE_RAM_DOMAIN_APP)
#define  FC7300_STCU_SRAM_INI_SEL_ALL                                           \
  (                                                                            \
   FC7300_STCU_SRAM_INI_SEL_ITCM_CPU0 | FC7300_STCU_SRAM_INI_SEL_DTCM0_CPU0 |  \
   FC7300_STCU_SRAM_INI_SEL_DTCM1_CPU0 | FC7300_STCU_SRAM_INI_SEL_ITCM_CPU1 |  \
   FC7300_STCU_SRAM_INI_SEL_DTCM0_CPU1 | FC7300_STCU_SRAM_INI_SEL_DTCM1_CPU1 | \
   FC7300_STCU_SRAM_INI_SEL_ITCM_CPU2 | FC7300_STCU_SRAM_INI_SEL_DTCM0_CPU2 |  \
   FC7300_STCU_SRAM_INI_SEL_DTCM1_CPU2)
#else
#define  FC7300_STCU_SRAM_INI_SEL_ALL                                           \
  (FC7300_STCU_SRAM_INI_SEL_SRAM0 | FC7300_STCU_SRAM_INI_SEL_SRAM1 |           \
   FC7300_STCU_SRAM_INI_SEL_SRAM2_1 | FC7300_STCU_SRAM_INI_SEL_SRAM2_2 |       \
   FC7300_STCU_SRAM_INI_SEL_ITCM_CPU0 | FC7300_STCU_SRAM_INI_SEL_DTCM0_CPU0 |  \
   FC7300_STCU_SRAM_INI_SEL_DTCM1_CPU0 | FC7300_STCU_SRAM_INI_SEL_ITCM_CPU1 |  \
   FC7300_STCU_SRAM_INI_SEL_DTCM0_CPU1 | FC7300_STCU_SRAM_INI_SEL_DTCM1_CPU1 | \
   FC7300_STCU_SRAM_INI_SEL_ITCM_CPU2 | FC7300_STCU_SRAM_INI_SEL_DTCM0_CPU2 |  \
   FC7300_STCU_SRAM_INI_SEL_DTCM1_CPU2)
#endif

/****************************************************************************
 * Name: SCM_MAMECCEN0_DISABLE
 *
 * Description:
 *   Macro definition for disable all ECC raad error in MAM 0
 *
 ****************************************************************************/

#define SCM_MAMECCEN0_DISABLE_ALL                                \
  (SCM_MAMECCEN0_MAM0_S0_ECC(2) | SCM_MAMECCEN0_MAM0_S1_ECC(2) | \
   SCM_MAMECCEN0_MAM0_S2_ECC(2) | SCM_MAMECCEN0_MAM0_S3_ECC(2) | \
   SCM_MAMECCEN0_MAM1_S0_ECC(2) | SCM_MAMECCEN0_MAM1_S1_ECC(2) | \
   SCM_MAMECCEN0_MAM1_S2_ECC(2) | SCM_MAMECCEN0_MAM1_S3_ECC(2) | \
   SCM_MAMECCEN0_MAM2_S0_ECC(2) | SCM_MAMECCEN0_MAM2_S1_ECC(2) | \
   SCM_MAMECCEN0_MAM2_S2_ECC(2) | SCM_MAMECCEN0_SRAM0_ECC(2) |   \
   SCM_MAMECCEN0_SRAM1_ECC(2) | SCM_MAMECCEN0_SRAM2_ECC(2))

/****************************************************************************
 * Name: SCM_MAMECCEN1_DISABLE
 *
 * Description:
 *   Macro definition for disable all ECC raad error in MAM 1
 *
 ****************************************************************************/

#define SCM_MAMECCEN1_DISABLE_ALL                          \
  (SCM_MAMECCEN1_HSM_ECC(2) | SCM_MAMECCEN1_ENET_ECC(2) |  \
   SCM_MAMECCEN1_DMA0_ECC(2) | SCM_MAMECCEN1_DMA1_ECC(2) | \
   SCM_MAMECCEN1_DMA0_CFG_ECC(2) | SCM_MAMECCEN1_DMA1_CFG_ECC(2))

/****************************************************************************
 * Name: SCM_CPU0ECCEN_DISABLE
 *
 * Description:
 *   Macro definition for disable all ECC raad error in CPU 0
 *
 ****************************************************************************/

#define SCM_CPU0ECCEN_DISABLE_ALL                                     \
  (SCM_CPU0ECCEN_CPU0_AHBM_ECC(2) | SCM_CPU0ECCEN_CPU0_AHBP_ECC(2) |  \
   SCM_CPU0ECCEN_CPU0_AHBS_ECC(2) | SCM_CPU0ECCEN_CPU0_DTCM0_ECC(2) | \
   SCM_CPU0ECCEN_CPU0_DTCM1_ECC(2) | SCM_CPU0ECCEN_CPU0_ITCM_ECC(2))

/****************************************************************************
 * Name: SCM_CPU1ECCEN_DISABLE
 *
 * Description:
 *   Macro definition for disable all ECC raad error in CPU 1
 *
 ****************************************************************************/

#define SCM_CPU1ECCEN_DISABLE_ALL                                     \
  (SCM_CPU1ECCEN_CPU1_AHBM_ECC(2) | SCM_CPU1ECCEN_CPU1_AHBP_ECC(2) |  \
   SCM_CPU1ECCEN_CPU1_AHBS_ECC(2) | SCM_CPU1ECCEN_CPU1_DTCM0_ECC(2) | \
   SCM_CPU1ECCEN_CPU1_DTCM1_ECC(2) | SCM_CPU1ECCEN_CPU1_ITCM_ECC(2))

/****************************************************************************
 * Name: SCM_CPU2ECCEN_DISABLE
 *
 * Description:
 *   Macro definition for disable all ECC raad error in CPU 2
 *
 ****************************************************************************/

#define SCM_CPU2ECCEN_DISABLE_ALL                                     \
  (SCM_CPU2ECCEN_CPU2_AHBM_ECC(2) | SCM_CPU2ECCEN_CPU2_AHBP_ECC(2) |  \
   SCM_CPU2ECCEN_CPU2_AHBS_ECC(2) | SCM_CPU2ECCEN_CPU2_DTCM0_ECC(2) | \
   SCM_CPU2ECCEN_CPU2_DTCM1_ECC(2) | SCM_CPU2ECCEN_CPU2_ITCM_ECC(2))

/****************************************************************************
 * Name: SCM_MAMECCEN0_ENABLE
 *
 * Description:
 *   Macro definition for enable all ECC raad error in MAM 0
 *
 ****************************************************************************/

#define SCM_MAMECCEN0_ENABLE_ALL                                 \
  (SCM_MAMECCEN0_MAM0_S0_ECC(3) | SCM_MAMECCEN0_MAM0_S1_ECC(3) | \
   SCM_MAMECCEN0_MAM0_S2_ECC(3) | SCM_MAMECCEN0_MAM0_S3_ECC(3) | \
   SCM_MAMECCEN0_MAM1_S0_ECC(3) | SCM_MAMECCEN0_MAM1_S1_ECC(3) | \
   SCM_MAMECCEN0_MAM1_S2_ECC(3) | SCM_MAMECCEN0_MAM1_S3_ECC(3) | \
   SCM_MAMECCEN0_MAM2_S0_ECC(3) | SCM_MAMECCEN0_MAM2_S1_ECC(3) | \
   SCM_MAMECCEN0_MAM2_S2_ECC(3) | SCM_MAMECCEN0_SRAM0_ECC(3) |   \
   SCM_MAMECCEN0_SRAM1_ECC(3) | SCM_MAMECCEN0_SRAM2_ECC(3))

/****************************************************************************
 * Name: SCM_MAMECCEN1_ENABLE
 *
 * Description:
 *   Macro definition for enable all ECC raad error in MAM 1
 *
 ****************************************************************************/

#define SCM_MAMECCEN1_ENABLE_ALL                           \
  (SCM_MAMECCEN1_HSM_ECC(3) | SCM_MAMECCEN1_ENET_ECC(3) |  \
   SCM_MAMECCEN1_DMA0_ECC(3) | SCM_MAMECCEN1_DMA1_ECC(3) | \
   SCM_MAMECCEN1_DMA0_CFG_ECC(3) | SCM_MAMECCEN1_DMA1_CFG_ECC(3))

/****************************************************************************
 * Name: SCM_CPU0ECCEN_ENABLE
 *
 * Description:
 *   Macro definition for enable all ECC raad error in CPU 0
 *
 ****************************************************************************/

#define SCM_CPU0ECCEN_ENABLE_ALL                                      \
  (SCM_CPU0ECCEN_CPU0_AHBM_ECC(3) | SCM_CPU0ECCEN_CPU0_AHBP_ECC(3) |  \
   SCM_CPU0ECCEN_CPU0_AHBS_ECC(3) | SCM_CPU0ECCEN_CPU0_DTCM0_ECC(3) | \
   SCM_CPU0ECCEN_CPU0_DTCM1_ECC(3) | SCM_CPU0ECCEN_CPU0_ITCM_ECC(3))

/****************************************************************************
 * Name: SCM_CPU1ECCEN_ENABLE
 *
 * Description:
 *   Macro definition for enable all ECC raad error in CPU 1
 *
 ****************************************************************************/

#define SCM_CPU1ECCEN_ENABLE_ALL                                      \
  (SCM_CPU1ECCEN_CPU1_AHBM_ECC(3) | SCM_CPU1ECCEN_CPU1_AHBP_ECC(3) |  \
   SCM_CPU1ECCEN_CPU1_AHBS_ECC(3) | SCM_CPU1ECCEN_CPU1_DTCM0_ECC(3) | \
   SCM_CPU1ECCEN_CPU1_DTCM1_ECC(3) | SCM_CPU1ECCEN_CPU1_ITCM_ECC(3))

/****************************************************************************
 * Name: SCM_CPU2ECCEN_ENABLE
 *
 * Description:
 *   Macro definition for enable all ECC raad error in CPU 2
 *
 ****************************************************************************/

#define SCM_CPU2ECCEN_ENABLE_ALL                                      \
  (SCM_CPU2ECCEN_CPU2_AHBM_ECC(3) | SCM_CPU2ECCEN_CPU2_AHBP_ECC(3) |  \
   SCM_CPU2ECCEN_CPU2_AHBS_ECC(3) | SCM_CPU2ECCEN_CPU2_DTCM0_ECC(3) | \
   SCM_CPU2ECCEN_CPU2_DTCM1_ECC(3) | SCM_CPU2ECCEN_CPU2_ITCM_ECC(3))


/****************************************************************************
 * Name: STCU_RAM_INIT_TOUT
 *
 * Description:
 *   STCU RAM initialization timeout
 *   When bus clock is 150MHz, STCU needs about 120us for RAM initialization
 *
 ****************************************************************************/

#define STCU_RAM_INIT_TOUT 0xFFFFu

/****************************************************************************
 * Name: FC7300_PCC_PCC_STCU
 *
 * Description:
 *   PCC STCU base address
 *
 ****************************************************************************/

#define FC7300_PCC_PCC_STCU (FC7300_PCC_BASE + 0x1FCu)

/****************************************************************************
 * Public Data
 ****************************************************************************/

/* g_idle_topstack: _sbss is the start of the BSS region as defined by the
 * linker script. _ebss lies at the end of the BSS region. The idle task
 * stack starts at the end of BSS and is of size CONFIG_IDLETHREAD_STACKSIZE.
 * The IDLE thread is the thread that the system boots on and, eventually,
 * becomes the IDLE, do nothing task that runs only when there is nothing
 * else to run.  The heap continues from there until the end of memory.
 * g_idle_topstack is a read-only variable the provides this computed
 * address.
 */

const uintptr_t g_idle_topstack = HEAP_BASE;

uint8_t isr_cfg[OS_NISRTHREAD];

extern const uint8_t _sdtcmdata_rom[];
extern uint8_t       _sdtcm_data[];
extern uint8_t       _edtcm_data[];
/****************************************************************************
 * Private Functions
 ****************************************************************************/

void weak_function isr_cfg_init(uint8_t *isrcfg, int n)
{
#ifdef CONFIG_FC7300_TSTMP_IRQ_ATTACH_THREAD
  isrcfg[TSTMP_ISR_THREAD] = CONFIG_FC7300_TSTMP_THREAD_PRIORITY;
#endif

#ifdef CONFIG_FC7300_DMA_IRQ_ATTACH_THREAD
  isrcfg[DMA_ISR_THREAD] = CONFIG_FC7300_DMA_THREAD_PRIORITY;
#endif

#ifdef CONFIG_FC7300_FCIIC_IRQ_ATTACH_THREAD
  isrcfg[FCIIC_ISR_THREAD] = CONFIG_FC7300_FCIIC_THREAD_PRIORITY;
#endif

#ifdef CONFIG_FC7300_FLEXCAN_ISR_THREAD
  isrcfg[FLEXCAN_ISR_THREAD] = CONFIG_FC7300_FLEXCAN_ISR_THREAD_PRIORITY;
#endif

#ifdef CONFIG_FC7300_LIN_ISR_THREAD
  isrcfg[LIN_ISR_THREAD] = CONFIG_FC7300_LIN_ISR_THREAD_PRIORITY;
#endif

#ifdef FC7300_GPIO_IRQ_ATTACH_THREAD
  isrcfg[GPIO_ISR_THREAD] = CONFIG_FC7300_GPIO_THREAD_PRIORITY;
#endif

#ifdef CONFIG_FC7300_FCUART_ISR_THREAD
  isrcfg[FCUART_ISR_THREAD] = CONFIG_FC7300_FCUART_ISR_THREAD_PRIORITY;
#endif

#ifdef CONFIG_FC7300_ADC_IRQ_ATTACH_THREAD
  isrcfg[ADC_ISR_THREAD] = CONFIG_FC7300_ADC_THREAD_PRIORITY;
#endif

#ifdef CONFIG_FC7300_CAP_IRQ_ATTACH_THREAD
  isrcfg[CAP_ISR_THREAD] = CONFIG_FC7300_CAP_THREAD_PRIORITY;
#endif
}

#ifdef CONFIG_BOOT_SHARE_RAM_DOMAIN_BOOT
#ifndef FC7300_RGM_POR_LVR
#define FC7300_RGM_POR_LVR 0x82
#endif
static void Boot_StandbyRamInit(void)
{
  volatile uint8_t* standbyRamAddrCore0 = (volatile uint8_t *)CORE0_STANDBY_RAM_START;
  volatile uint8_t* standbyRamAddrEndCore0 = (volatile uint8_t *)(CORE0_STANDBY_RAM_START + CORE0_STANDBY_RAM_SIZE + CORE0_STANDBY_RAM_FIX_SIZE);
  volatile uint8_t* standbyRamAddrCore1 = (volatile uint8_t *)CORE1_STANDBY_RAM_START;
  volatile uint8_t* standbyRamAddrEndCore1 = (volatile uint8_t *)(CORE1_STANDBY_RAM_START + CORE1_STANDBY_RAM_SIZE + CORE1_STANDBY_RAM_FIX_SIZE);
  uint32_t bootRstReason = (uint32_t)(*((volatile uint32_t*)FC7300_RGM_SRS));
  uint32_t pwrDirectRstFlag = (uint32_t)(*((volatile uint32_t*)STANDBY_RAM_FIX_PWR_DIRECT_RST_FLAG_ADDR));
  if (((bootRstReason & FC7300_RGM_POR_LVR) == FC7300_RGM_POR_LVR) || (pwrDirectRstFlag == PWR_DIRECT_RST_FLAG))
  {
    for ( ; standbyRamAddrCore0 < standbyRamAddrEndCore0; standbyRamAddrCore0++)
    {
      *standbyRamAddrCore0 = 0;
    }
    for ( ; standbyRamAddrCore1 < standbyRamAddrEndCore1; standbyRamAddrCore1++)
    {
      *standbyRamAddrCore1 = 0;
    }
  }
}
#endif

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: up_cpu_index
 *
 * Description:
 *   Return the index of the physical core index, will override the default
 *   value in arch.
 *
 ****************************************************************************/

#ifdef CONFIG_ARCH_HAVE_MULTICPU
int up_cpu_index(void)
{
  return fc7300_cpu_index();
}
#endif /* CONFIG_ARCH_HAVE_MULTICPU */

/****************************************************************************
 * Name: __start
 *
 * Description:
 *   This is the reset entry point.
 *
 ****************************************************************************/

void __start(void)
{
#ifdef CONFIG_FC7300_BOARD_PROFILING
  fc7300_gpiowrite(GPIO_BOARD_PROFILING, true);
#endif

#ifdef CONFIG_BOOT_RUNFROMFLASH
  const uint32_t *src;
#endif
  uint32_t *dest;

  /* Make sure that interrupts are disabled */

  __asm__ __volatile__("\tcpsid  i\n");

  /* FC7300 erratum ERR_Debug_001 workaround. Since the value of DWT_CYCCNT is
     unknown after reset, clear it in the reset handler. */

  putreg32(NVIC_DEMCR_TRCENA, NVIC_DEMCR);
  putreg32(0, DWT_CYCCNT);

  if (fc7300_cpu_index() == 0)
    {
      /* Disable the watchdog timer */

#ifdef CONFIG_FC7300_WATCHDOG_OPERATION_IN_START
  #if (CONFIG_FC7300_WATCHDOG_TIMEOUT_IN_START == 0)
      FC7300_Close_Internal_Watchdog(0);
  #else
      FC7300_Open_Internal_Watchdog(0, CONFIG_FC7300_WATCHDOG_TIMEOUT_IN_START);
  #endif
#endif

#ifdef CONFIG_BOOT_RUNFROMFLASH
      /* Disable ECC read error check for all memory regions */

      putreg32(SCM_MAMECCEN0_DISABLE_ALL, FC7300_SCM_MAMECCEN0);
      putreg32(SCM_MAMECCEN1_DISABLE_ALL, FC7300_SCM_MAMECCEN1);
      putreg32(SCM_CPU0ECCEN_DISABLE_ALL, FC7300_SCM_CPU0ECCEN);
      putreg32(SCM_CPU1ECCEN_DISABLE_ALL, FC7300_SCM_CPU1ECCEN);
      putreg32(SCM_CPU2ECCEN_DISABLE_ALL, FC7300_SCM_CPU2ECCEN);

      /* Enable STCU PCC Clock */

      putreg32(PCC_CGC(1), FC7300_PCC_PCC_STCU);

      /* Clear All RAM regions to 0 with STCU */

      putreg32(FC7300_STCU_SRAM_INI_SEL_ALL, FC7300_STCU_SRAM_INI_SEL);
      putreg32(FC7300_STCU_SRAM_INI_CTRL_MODE_STANDBY1,
               FC7300_STCU_SRAM_INI_CTRL);

      /* Start the STCU RAM clear process and wait the progress to finish.
         Using inline assembly to gurantee that no RAM will be accessed when
         clearing. */

      __asm__ __volatile__(
        "movw r0, %[stcu_base_l]                       \n"
        "movt r0, %[stcu_base_h]                       \n"
        "ldr  r1, [r0, %[stcu_sram_ini_ctrl_offset]]   \n"
        "orr  r1, r1, %[stcu_sram_ini_ctrl_en]         \n"
        "str  r1, [r0, %[stcu_sram_ini_ctrl_offset]]   \n"
        "mov  r2, %[stcu_ram_init_tout]                \n"
        "1:                                            \n"
        "ldr  r1, [r0, %[stcu_sram_ini_status_offset]] \n"
        "tst  r1, %[stcu_sram_ini_status_busy]         \n"
        "bne  2f                                       \n"
        "subs r2, r2, #1                               \n"
        "bhs  1b                                       \n"
        "2:                                            \n"
        "mov  r2, %[stcu_ram_init_tout]                \n"
        "3:                                            \n"
        "ldr  r1, [r0, %[stcu_sram_ini_status_offset]] \n"
        "tst  r1, %[stcu_sram_ini_status_done]         \n"
        "bne  4f                                       \n"
        "subs r2, r2, #1                               \n"
        "bhs  3b                                       \n"
        "4:                                            \n"
        ::[stcu_base_l]"i"(FC7300_STCU_BASE & 0xFFFF),
        [stcu_base_h]"i"(FC7300_STCU_BASE >> 16),
        [stcu_sram_ini_ctrl_offset]"i"(FC7300_STCU_SRAM_INI_CTRL_OFFSET),
        [stcu_sram_ini_ctrl_en]"i"(FC7300_STCU_SRAM_INI_CTRL_EN),
        [stcu_sram_ini_status_offset]"i"(FC7300_STCU_SRAM_INI_STATUS_OFFSET),
        [stcu_sram_ini_status_busy]"i"(FC7300_STCU_SRAM_INI_STATUS_BUSY),
        [stcu_sram_ini_status_done]"i"(FC7300_STCU_SRAM_INI_STATUS_DONE),
        [stcu_ram_init_tout]"i"(STCU_RAM_INIT_TOUT)
        :"r0", "r1", "r2"
      );


      /* Clear the SRAM_INI_SEL to prevent unexpected trigger */

      putreg32(0U, FC7300_STCU_SRAM_INI_SEL);

      /* Enable ECC read error check for all memory regions */

      putreg32(SCM_MAMECCEN0_ENABLE_ALL, FC7300_SCM_MAMECCEN0);
      putreg32(SCM_MAMECCEN1_ENABLE_ALL, FC7300_SCM_MAMECCEN1);
      putreg32(SCM_CPU0ECCEN_ENABLE_ALL, FC7300_SCM_CPU0ECCEN);
      putreg32(SCM_CPU1ECCEN_ENABLE_ALL, FC7300_SCM_CPU1ECCEN);
      putreg32(SCM_CPU2ECCEN_ENABLE_ALL, FC7300_SCM_CPU2ECCEN);

      /* Since All RAM regions are cleared to 0, no need to clear bss */
#endif
    }
  else if (fc7300_cpu_index() == 1)
    {
      /* Disable the watchdog timer */

#ifdef CONFIG_FC7300_WATCHDOG_OPERATION_IN_START
  #if (CONFIG_FC7300_WATCHDOG_TIMEOUT_IN_START == 0)
      FC7300_Close_Internal_Watchdog(1);
  #else
      FC7300_Open_Internal_Watchdog(1, CONFIG_FC7300_WATCHDOG_TIMEOUT_IN_START);
  #endif
#endif

      /* Note: If core 1 will be reset standalone, then bss of core 1 shall be
       * cleared in this case */
    }
  else if (fc7300_cpu_index() == 2)
    {
      /* Disable the watchdog timer */

#ifdef CONFIG_FC7300_WATCHDOG_OPERATION_IN_START
  #if (CONFIG_FC7300_WATCHDOG_TIMEOUT_IN_START == 0)
      FC7300_Close_Internal_Watchdog(2);
  #else
      FC7300_Open_Internal_Watchdog(2, CONFIG_FC7300_WATCHDOG_TIMEOUT_IN_START);
  #endif
#endif

      /* Note: If core 2 will be reset standalone, then bss of core 2 shall be
       * cleared in this case */
    }
  else
    {
      /* This shall never be reached */
    }

#ifdef CONFIG_BOOT_SHARE_RAM_DOMAIN_BOOT
  Boot_StandbyRamInit();
#endif

#ifdef CONFIG_BOOT_RUNFROMFLASH
  /* Move the initialized data section from his temporary holding spot in
   * FLASH into the correct place in SRAM.  The correct place in SRAM is
   * give by _sdata and _edata.  The temporary location is in FLASH at the
   * end of all of the other read-only data (.text, .rodata) at _eronly.
   */

  for (src = (const uint32_t *)_eronly, dest = (uint32_t *)_sdata;
       dest < (uint32_t *)_edata;)
    {
      *dest++ = *src++;
    }

  extern const uint8_t _calib_pflash[];
  extern uint8_t       _scalibra[];
  extern uint8_t       _ecalibra[];

  for (src = (const uint32_t *)_calib_pflash, dest = (uint32_t *)_scalibra;
       dest < (uint32_t *)_ecalibra;)
    {
      *dest++ = *src++;
    }

  for (src = (const uint32_t *)_sdtcmdata_rom, dest = (uint32_t *)_sdtcm_data;
       dest < (uint32_t *)_edtcm_data;)
    {
      *dest++ = *src++;
    }
#if defined(CONFIG_BOOT_SHARE_RAM_DOMAIN_APP)
  if (fc7300_cpu_index() == 0)
    {
    #ifdef CONFIG_BUILD_PROTECTED
      extern const uint8_t _share_pflash_app_manual_qm[];
      extern const uint8_t _share_pflash_bsw_manual_qm[];
      extern const uint8_t _share_pflash_rte_qm[];
      extern const uint8_t _share_pflash_bsw_manual_asil[];
      extern const uint8_t _share_pflash_rte_asil[];
      extern uint8_t       _sioc_app_manual_qm[];
      extern uint8_t       _eioc_app_manual_qm[];
      extern uint8_t       _sioc_bsw_manual_qm[];
      extern uint8_t       _eioc_bsw_manual_qm[];
      extern uint8_t       _sioc_rte_qm[];
      extern uint8_t       _eioc_rte_qm[];
      extern uint8_t       _sioc_bsw_manual_asil[];
      extern uint8_t       _eioc_bsw_manual_asil[];
      extern uint8_t       _sioc_rte_asil[];
      extern uint8_t       _eioc_rte_asil[];

      for (src = (const uint32_t *)_share_pflash_app_manual_qm, dest = (uint32_t *)_sioc_app_manual_qm;
           dest < (uint32_t *)_eioc_app_manual_qm;)
        {
          *dest++ = *src++;
        }
      for (src = (const uint32_t *)_share_pflash_bsw_manual_qm, dest = (uint32_t *)_sioc_bsw_manual_qm;
           dest < (uint32_t *)_eioc_bsw_manual_qm;)
        {
          *dest++ = *src++;
        }
      for (src = (const uint32_t *)_share_pflash_rte_qm, dest = (uint32_t *)_sioc_rte_qm;
           dest < (uint32_t *)_eioc_rte_qm;)
        {
          *dest++ = *src++;
        }
      for (src = (const uint32_t *)_share_pflash_bsw_manual_asil, dest = (uint32_t *)_sioc_bsw_manual_asil;
           dest < (uint32_t *)_eioc_bsw_manual_asil;)
        {
          *dest++ = *src++;
        }
      for (src = (const uint32_t *)_share_pflash_rte_asil, dest = (uint32_t *)_sioc_rte_asil;
           dest < (uint32_t *)_eioc_rte_asil;)
        {
          *dest++ = *src++;
        }

    #else
      extern const uint8_t _share_pflash[];
      extern uint8_t       _sdatashare[];
      extern uint8_t       _edatashare[];

      for (src = (const uint32_t *)_share_pflash, dest = (uint32_t *)_sdatashare;
           dest < (uint32_t *)_edatashare;)
        {
          *dest++ = *src++;
        }
    #endif
    }
#  endif

  /* Clear .bss.  We'll do this inline (vs. calling memset) just to be
   * certain that there are no issues with the state of global variables.
   */

  for (dest = (uint32_t *)_sbss; dest < (uint32_t *)_ebss; )
    {
      *dest++ = 0;
    }

#endif

    /* Copy any necessary code sections from FLASH to RAM.  The correct
     * destination in SRAM is given by _sramfuncs and _eramfuncs.  The
     * temporary location is in flash after the data initialization code
     * at _framfuncs.  This should be done before fc7300_clockconfig() is
     * called (in case it has some dependency on initialized C variables).
     */

#ifdef CONFIG_ARCH_RAMFUNCS
  for (src = (const uint32_t *)_framfuncs, dest = (uint32_t *)_sramfuncs;
       dest < (uint32_t *)_eramfuncs;)
    {
      *dest++ = *src++;
    }
#endif

  if (fc7300_cpu_index() == 0)
    {
      /* Configure the clocking and the console uart so that we can get debug
       * output as soon as possible.  NOTE: That this logic must not assume that
       * .bss or .data have been initialized.
       */

      DEBUGVERIFY(fc7300_clockconfig(&g_initial_clkconfig));
    }

#ifdef CONFIG_ARCH_PERF_EVENTS
  up_perf_init((void *)FC7300_SYSCLK_FREQUENCY);
#endif

  fc7300_lowsetup();

  LOG_TRAP_UP(STARTUP_0);
  showprogress('B');

  /* Initialize the FPU (if configured) */

  arm_fpuconfig();

#ifdef CONFIG_FC7300_ERM
  fc7300_erm_init();
#endif

#ifdef CONFIG_FC7300_FCSMU
  fc7300_fcsmu_init();
#endif

#ifdef CONFIG_FC7300_CMU
  fc7300_cmu_init();
#endif

#ifdef CONFIG_FC7300_EIM
  fc7300_eim_run();
#endif

#ifdef CONFIG_ARCH_USE_MPU
  fc7300_mpuinit();
#endif

#ifdef CONFIG_BUILD_PROTECTED
  /* For the case of the separate user-/kernel-space build, perform whatever
   * platform specific initialization of the user memory is required.
   * Normally this just means initializing the user space .data and .bss
   * segments.
   */

  fc7300_userspace();
#endif

#ifdef CONFIG_FC7300_PMC
  fc7300_pmc_init();
#endif

#ifdef CONFIG_FC7300_STCU
  fc7300_get_stcu_selftest_result();
#endif

#ifdef CONFIG_FC7300_TMU
  fc7300_tmu_init();
#endif

#ifdef CONFIG_FC7300_MAM
  fc7300_mam_init();
#endif

#ifdef CONFIG_FC7300_PFLASH_PREFETCH
  fc7300_pflash_prefetch_enable();
#endif

  /* Enable I-Cache and D-Cache */

#ifdef CONFIG_ARMV7M_ICACHE
  up_enable_icache();
#endif

#ifdef CONFIG_ARMV7M_DCACHE
  up_enable_dcache();
#endif

  LOG_TRAP_UP(STARTUP_1);
  showprogress('C');

  /* Perform early serial initialization */
#ifdef USE_EARLYSERIALINIT
  arm_earlyserialinit();
#endif

  LOG_TRAP_UP(STARTUP_2);
  showprogress('E');

#ifdef CONFIG_FC7300_PROGMEM
  fc7300_progmem_init();
#endif

  /* Then start NuttX */

  showprogress('\r');
  showprogress('\n');
  isr_cfg_init(isr_cfg, OS_NISRTHREAD);

  LOG_TRAP_UP(STARTUP_3);
  nx_start();

  /* Shouldn't get here */

  for (;;)
    ;
}

#if CONFIG_MM_REGIONS > 1
void arm_addregion(void)
{
  extern uint8_t _dtcm_heap_start[];
  extern uint8_t _dtcm_heap_size[];
  if((size_t)_dtcm_heap_size > 0)
    {
      kmm_addregion((void *)_dtcm_heap_start, (size_t)_dtcm_heap_size);
    }

  extern uint8_t _itcm_heap_start[];
  extern uint8_t _itcm_heap_size[];
  if((size_t)_itcm_heap_size > 0)
    {
      kmm_addregion((void *)_itcm_heap_start, (size_t)_itcm_heap_size);
    }
}
#endif
