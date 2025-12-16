#############################################################################
# Copyright (C) 2025 Xiaomi Corporation
#
# Licensed to the Apache Software Foundation (ASF) under one or more
# contributor license agreements.  See the NOTICE file distributed with
# this work for additional information regarding copyright ownership.  The
# ASF licenses this file to you under the Apache License, Version 2.0 (the
# "License"); you may not use this file except in compliance with the
# License.  You may obtain a copy of the License at
#
#   http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
# WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the
# License for the specific language governing permissions and limitations
# under the License.
#
#############################################################################

include $(TOPDIR)/Make.defs

CSRCS  = fc7300_boot.c fc7300_bringup.c
CSRCS += fc7300_clockconfig.c
CSRCS += fc7300_periphclocks.c

ifeq ($(CONFIG_DEV_GPIO),y)
CSRCS += fc7300_gpio.c
endif

ifeq ($(CONFIG_ARCH_LEDS),y)
CSRCS += fc7300_autoleds.c
endif

ifeq ($(CONFIG_BOARDCTL),y)
CSRCS += fc7300_appinit.c
endif

RCSRCS = etc/init.d/rc.sysinit etc/init.d/rcS
RCRAWS = etc/group

ifeq ($(CONFIG_ADC),y)
CSRCS += fc7300_adc.c
endif

ifeq ($(CONFIG_FC7300_PWM),y)
CSRCS += fc7300_pwm.c
endif

ifeq ($(CONFIG_CAPTURE),y)
CSRCS += fc7300_capture.c
endif

ifeq ($(CONFIG_ARCH_BUTTONS),y)
CSRCS += fc7300_buttons.c
endif

ifeq ($(CONFIG_WATCHDOG),y)
CSRCS += fc7300_wdog.c
endif

ifeq ($(CONFIG_SPI),y)
CSRCS += fc7300_fcspi.c
endif

ifeq ($(CONFIG_TIMER),y)
CSRCS += fc7300_timer.c
endif

ifeq ($(CONFIG_FC7300_ENET_PHY_BOADINIT),y)
CSRCS += fc7300_phyinit.c
endif

ifeq ($(CONFIG_NET_CAN)_$(CONFIG_NETDEV_LATEINIT),y_y)
FC7300_FLEXCAN_NET_CAN_LATEINIT = y
else
FC7300_FLEXCAN_NET_CAN_LATEINIT = n
endif

ifneq ($(CONFIG_CAN)_$(FC7300_FLEXCAN_NET_CAN_LATEINIT),n_n)
CSRCS += fc7300_flexcan.c
CSRCS += fc7300_flexcan_filter.c
endif

ifeq ($(CONFIG_FC7300_TIC12400),y)
CFLAGS += -I$(TOPDIR)/../vendor/xiaomi/micar/zcu/tic12400
endif

include $(TOPDIR)/boards/Board.mk
