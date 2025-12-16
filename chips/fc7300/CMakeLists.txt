# ##############################################################################
# Copyright (C) 2025 Xiaomi Corporation
#
# Licensed to the Apache Software Foundation (ASF) under one or more contributor
# license agreements.  See the NOTICE file distributed with this work for
# additional information regarding copyright ownership.  The ASF licenses this
# file to you under the Apache License, Version 2.0 (the "License"); you may not
# use this file except in compliance with the License.  You may obtain a copy of
# the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
# WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the
# License for the specific language governing permissions and limitations under
# the License.
#
# ##############################################################################

set(SRCS
    fc7300_start.c
    fc7300_clockconfig.c
    fc7300_periphclocks.c
    fc7300_lowputc.c
    fc7300_irq.c
    fc7300_wdog.c
    fc7300_pin.c
    fc7300_gpio.c
    fc7300_trgsel.c
    fc7300_net.c
    fc7300_uart.c
    fc7300_timerisr.c)

if(CONFIG_FC7300_MB)
  list(APPEND SRCS fc7300_mb.c)
endif()

if(CONFIG_FC7300_HWSPINLOCK)
  list(APPEND SRCS fc7300_hwspinlock.c)
endif()

if(CONFIG_ARCH_USE_MPU)
  list(APPEND SRCS fc7300_mpu.c)
endif()

if(CONFIG_FC7300_ADC)
  list(APPEND SRCS fc7300_adc.c)
endif()

if(CONFIG_FC7300_PWM)
  list(APPEND SRCS fc7300_pwm.c)
endif()

if(CONFIG_FC7300_CAP)
  list(APPEND SRCS fc7300_capture.c)
endif()

if(CONFIG_FC7300_CAP)
  list(APPEND SRCS fc7300_capture_lowerhalf.c)
endif()

if(CONFIG_FC7300_GPIOIRQ)
  list(APPEND SRCS fc7300_pinirq.c)
endif()

if(CONFIG_FC7300_DMA)
  list(APPEND SRCS fc7300_dma.c)
endif()

if(CONFIG_DEBUG_GPIO_INFO)
  list(APPEND SRCS fc7300_pindump.c)
endif()

if(CONFIG_FC7300_FCIIC)
  list(APPEND SRCS fc7300_fciic.c)
endif()

if(CONFIG_FC7300_FCPIT)
  list(APPEND SRCS fc7300_fcpit.c fc7300_fcpit_lowerhalf.c)
endif()

if(CONFIG_FC7300_FCSPI)
  list(APPEND SRCS fc7300_fcspi.c)
endif()

if(CONFIG_FC7300_FLEXCAN)
  list(APPEND SRCS fc7300_flexcan_common.c fc7300_flexcan_config.c)
endif()

if(CONFIG_FC7300_FLEXCAN_SOCKET)
  list(APPEND SRCS fc7300_flexcan_socket.c)
endif()

if(CONFIG_FC7300_FLEXCAN_CHARDRIVER)
  list(APPEND SRCS fc7300_flexcan_chardriver.c)
endif()

if(CONFIG_FC7300_LIN)
  list(APPEND SRCS fc7300_lin.c)
endif()

if(CONFIG_FC7300_RTC)
  list(APPEND SRCS fc7300_rtc.c)
endif()

if(CONFIG_FC7300_PROGMEM)
  list(APPEND SRCS fc7300_progmem.c)
endif()

if(CONFIG_FC7300_EEEPROM)
  list(APPEND SRCS fc7300_eeeprom.c)
endif()

if(CONFIG_FC7300_NORFLASH_MTD)
  list(APPEND SRCS fc7300_norflash_mtd.c fc7300_partition.c)
endif()

if(CONFIG_FC7300_RESETCAUSE_PROCFS)
  list(APPEND SRCS fc7300_resetcause_procfs.c)
endif()

if(CONFIG_BOARDCTL_RESET_CAUSE)
  list(APPEND SRCS fc7300_rgm.c)
endif()

if(CONFIG_FC7300_TSTMP)
  list(APPEND SRCS fc7300_tstmp.c)
endif()

if(CONFIG_ONESHOT)
  list(APPEND SRCS fc7300_oneshot_lowerhalf.c fc7300_oneshot.c)
endif()

if(CONFIG_BOARDCTL_RESET)
  list(APPEND SRCS fc7300_reset.c)
endif()

if(CONFIG_BOARDCTL_POWEROFF)
  list(APPEND SRCS fc7300_pm.c)
endif()

if(CONFIG_FC7300_RPTUN)
  list(APPEND SRCS fc7300_rptun.c)
endif()

if(CONFIG_FC7300_RPMSG_VIRTIO)
  list(APPEND SRCS fc7300_rpmsg_virtio.c)
endif()

if(CONFIG_BUILD_PROTECTED)
  list(APPEND SRCS fc7300_allocateheap.c)
  list(APPEND SRCS fc7300_userspace.c)
endif()

if(CONFIG_CRYPTO_CRYPTODEV_HARDWARE)
if(CONFIG_FC7300_HSM OR CONFIG_FC7300_CRC32)
  list(APPEND SRCS fc7300_crypto.c)
endif()
if(CONFIG_FC7300_HSM)
  list(APPEND SRCS fc7300_crypto_level0_api.c)
  list(APPEND SRCS fc7300_crypto_level1_api.c)
  list(APPEND SRCS fc7300_crypto_level2_api.c)
  list(APPEND SRCS fc7300_crypto_level3_api.c)
  list(APPEND SRCS fc7300_crypto_levelN_api.c)
  list(APPEND SRCS fc7300_crypto_hsm.c)
  list(APPEND SRCS fc7300_crypto_interface.c)
endif()
if(CONFIG_FC7300_CRC32)
  list(APPEND SRCS fc7300_crc.c)
endif()
endif()

if(CONFIG_FC7300_STCU)
  list(APPEND SRCS fc7300_stcu.c)
endif()

if(CONFIG_FC7300_ERM)
  list(APPEND SRCS fc7300_erm.c)
  list(APPEND SRCS fc7300_erm_user.c)
endif()

if(CONFIG_FC7300_FCSMU)
  list(APPEND SRCS fc7300_fcsmu.c)
  list(APPEND SRCS fc7300_fcsmu_user.c)
endif()

if(CONFIG_FC7300_EIM)
  list(APPEND SRCS fc7300_eim.c)
  list(APPEND SRCS fc7300_eim_asm.S)
endif()

if(CONFIG_FC7300_CMU)
  list(APPEND SRCS fc7300_cmu.c)
  list(APPEND SRCS fc7300_cmu_user.c)
endif()

if(CONFIG_FC7300_PMC)
  list(APPEND SRCS fc7300_pmc.c)
  list(APPEND SRCS fc7300_pmc_user.c)
endif()

if(CONFIG_FC7300_TMU)
  list(APPEND SRCS fc7300_tmu.c)
  list(APPEND SRCS fc7300_tmu_user.c)
endif()

if(CONFIG_FC7300_MAM)
  list(APPEND SRCS fc7300_mam.c)
  list(APPEND SRCS fc7300_mam_user.c)
endif()

target_sources(arch PRIVATE ${SRCS})

target_include_directories(arch PRIVATE ${NUTTX_CHIP_ABS_DIR}/include)

if(CONFIG_SYSTEM_GCOV)
set(FLAGS
    -fprofile-arcs
    -ftest-coverage
    -fno-inline)

target_compile_options(arch PRIVATE ${FLAGS})
endif()
