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
#include <nuttx/arch.h>
#include <nuttx/board.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <arch/board/board.h>
#include "fc7300f8m-evb.h"
#include "fc7300_multicore.h"
#include "nvic.h"
#include <stdio.h>

/****************************************************************************
 * Private Data
 ****************************************************************************/

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: board_early_initialize
 *
 * Description:
 *   If CONFIG_BOARD_EARLY_INITIALIZE is selected, then an additional
 *   initialization call will be performed in the boot-up sequence to a
 *   function called board_early_initialize().
 *
 ****************************************************************************/

#ifdef CONFIG_BOARD_EARLY_INITIALIZE
void board_early_initialize(void)
{
#ifdef CONFIG_ARCH_PERF_EVENTS
  up_perf_init((void *)FC7300_SYSCLK_FREQUENCY);
#endif

#ifdef CONFIG_ARCH_LEDS
  /* Configure on-board LEDs if LED support has been selected */

  board_autoled_initialize();
#endif
}
#endif

/****************************************************************************
 * Name: board_late_initialize
 *
 * Description:
 *   If CONFIG_BOARD_LATE_INITIALIZE is selected, then an additional
 *   initialization call will be performed in the boot-up sequence to a
 *   function called board_late_initialize().  board_late_initialize() will
 *   be called immediately after up_initialize() is called and just before
 *   the initial application is started.  This additional initialization
 *   phase may be used, for example, to initialize board-specific device
 *   drivers.
 *
 ****************************************************************************/

#ifdef CONFIG_BOARD_LATE_INITIALIZE
void board_late_initialize(void)
{
  /* Perform board-specific initialization */

  fc7300_bringup();
}
#endif

#ifdef CONFIG_BOARDCTL_BOOT_IMAGE

int board_boot_image(const char *path, uint32_t hdr_size)
{
  uint32_t msp;
  uint32_t reset_handler;
  uint32_t app_addr;
  int32_t ret;
  struct file f;

  ret = file_open(&f, "/dev/ap0", O_RDONLY);
  if (ret < 0)
    {
      serr("ERROR: open ap0 failed\n");
      return -1;
    }
  
  ret = file_mmap(&f, NULL, 1024, PROT_READ, MAP_SHARED,
                  0, (FAR void **)&app_addr);
  if (ret < 0)
    {
      serr("Failed to file_mmap: %ld\n", ret);
      return ret;
    }

  msp           = getreg32(app_addr);
  reset_handler = getreg32(app_addr + 4U);
  file_munmap((FAR void *)app_addr, 1024);
  file_close(&f);

/****************************************************************************
 * systick_disable();
 * reset the norflash if needed
 * reset the EMMC if needed
 * reset the UART2 (console) if needed
 * reset the stimer system timer
 * Back to normal mode if HP is enabled
 * cleanup_arm_nvic();

 *   #ifdef CONFIG_ARMV7M_DCACHE
 *     up_disable_dcache();
 *   #endif
 *    #ifdef CONFIG_ARMV7M_ICACHE
 *      up_disable_icache();
 *    #endif

 *    #ifdef CONFIG_ARM_MPU
 *      mpu_control(false, false, false);
 *    #endif

 * setcontrol(0x00);
 * UP_ISM();
 ****************************************************************************/

  asm volatile(
                "mov     sp, %0\n\t"
                "mov     pc, %1\n\t"
                :
                : "r"(msp), "r"(reset_handler)
                : "memory");

  return OK;
}
#endif

typedef struct
{
  uint32_t OtaVerValid0;
  uint32_t OtaVerValid1;
  uint32_t OtaIndicator0;
  uint32_t OtaIndicator1;
}OTA_VersionType;

int board_switch_boot(FAR const char *system)
{
  int ret;
  struct file f;
  extern uint8_t _sotabank0[];
  extern uint8_t _sotabank1[];
  extern uint8_t _sotabank2[];
  extern uint8_t _sotabank3[];

  _info("[%d] bank0 curr OTA_VERSION value:0x%lx\n", up_cpu_index(), *(uint32_t *)_sotabank0);
  _info("[%d] bank1 before OTA_VERSION value:0x%lx\n", up_cpu_index(), *(uint32_t *)_sotabank1);

  uint32_t ov = ((OTA_VersionType *)_sotabank0)->OtaVerValid0;
  ov += 1;
  OTA_VersionType ota_tmp = {
    ov,
    ~ov,
    0x88886666,
    0xFC20FACE
  };

  ret = file_open(&f, "/dev/ota_b1", O_RDWR);
  if (ret < 0)
    {
      _info("error: cannot open ota_b1, ret:%d\n", ret);
      return -1;
    }

  ret = file_write(&f, &ota_tmp, sizeof(OTA_VersionType));
  if (ret != sizeof(OTA_VersionType))
    {
      _info("error: write /dev/ota_b1 failed, ret:%d\n", ret);
      file_close(&f);
      return -1;
    }

  file_close(&f);

  _info("[%d] bank1 after OTA_VERSION value:0x%lx\n", up_cpu_index(), *(uint32_t *)_sotabank1);

  _info("[%d] bank2 curr OTA_VERSION value:0x%lx\n", up_cpu_index(), *(uint32_t *)_sotabank2);
  _info("[%d] bank3 before OTA_VERSION value:0x%lx\n", up_cpu_index(), *(uint32_t *)_sotabank3);

  ov = ((OTA_VersionType *)_sotabank2)->OtaVerValid0;
  ov += 1;

  ota_tmp = (OTA_VersionType){
    ov,
    ~ov,
    0x88886666,
    0xFC20FACE
  };

  ret = file_open(&f, "/dev/ota_b2", O_RDWR);
  if (ret < 0)
    {
      _info("error: cannot open ota_b2, ret:%d\n", ret);
      return -1;
    }

  ret = file_write(&f, &ota_tmp, sizeof(OTA_VersionType));
  if (ret != sizeof(OTA_VersionType))
    {
      _info("error: write /dev/ota_b2 failed, ret:%d\n", ret);
      file_close(&f);
      return -1;
    }

  file_close(&f);
  _info("[%d] bank3 after OTA_VERSION value:0x%lx\n", up_cpu_index(), *(uint32_t *)_sotabank3);

  return 0;
}

#ifdef CONFIG_BOARDCTL_START_CPU

int board_start_cpu(int cpuid)
{
  return fc7300_cpu_start(cpuid);
}

#endif