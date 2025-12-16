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

set(SRCS fc7300_boot.c fc7300_bringup.c fc7300_clockconfig.c
         fc7300_periphclocks.c)

if(CONFIG_DEV_GPIO)
  list(APPEND SRCS fc7300_gpio.c)
endif()

if(CONFIG_ARCH_LEDS)
  list(APPEND SRCS fc7300_autoleds.c)
endif()

if(CONFIG_BOARDCTL)
  list(APPEND SRCS fc7300_appinit.c)
endif()

if(CONFIG_ADC)
  list(APPEND SRCS fc7300_adc.c)
endif()

if(CONFIG_FC7300_PWM)
  list(APPEND SRCS fc7300_pwm.c)
endif()

if(CONFIG_CAPTURE)
  list(APPEND SRCS fc7300_capture.c)
endif()

if(CONFIG_ARCH_BUTTONS)
  list(APPEND SRCS fc7300_buttons.c)
endif()

if(CONFIG_WATCHDOG)
  list(APPEND SRCS fc7300_wdog.c)
endif()

if(CONFIG_SPI)
  list(APPEND SRCS fc7300_fcspi.c)
endif()

if((CONFIG_NET_CAN AND CONFIG_NETDEV_LATEINIT) OR CONFIG_CAN)
  list(APPEND SRCS fc7300_flexcan.c fc7300_flexcan_filter.c)
endif()

if(CONFIG_FC7300_ENET_PHY_BOADINIT)
  list(APPEND SRCS fc7300_phyinit.c)
endif()

if(CONFIG_FC7300_TIC12400)
  set(TIC12400_DIR ${NUTTX_DIR}/../vendor/xiaomi/micar/zcu/tic12400)
  list(APPEND SRCS
    ${TIC12400_DIR}/tic12400.c
    ${TIC12400_DIR}/tic12400_spi.c
    ${TIC12400_DIR}/tic12400_cfg.c
    ${TIC12400_DIR}/tic12400_drv.c
  )
  target_include_directories(board PRIVATE ${TIC12400_DIR})
endif()

nuttx_add_romfs(
  NAME
  etc
  MOUNTPOINT
  etc
  RCSRCS
  etc/init.d/rcS
  etc/init.d/rc.sysinit
  RCRAWS
  etc/group
  PATH
  ${CMAKE_CURRENT_BINARY_DIR}/etc)

target_link_libraries(board PRIVATE romfs_etc)

target_sources(board PRIVATE ${SRCS})
