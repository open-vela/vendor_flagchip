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

add_subdirectory(src)

file(GLOB EXTRA_LIBS
     ${NUTTX_BOARD_ABS_DIR}/libs/${CONFIG_ARCH_BOARD_CUSTOM_NAME}/*.a)
file(COPY
     ${NUTTX_BOARD_ABS_DIR}/include/memory_layout.h
     DESTINATION ${CMAKE_BINARY_DIR}/include)

set_property(GLOBAL APPEND PROPERTY NUTTX_EXTRA_LIBRARIES ${EXTRA_LIBS})

if(CONFIG_BUILD_PROTECTED)
  add_subdirectory(kernel)
  set_property(
    GLOBAL
    PROPERTY LD_SCRIPT_USER
              ${NUTTX_BOARD_ABS_DIR}/scripts/user-space-cmake.ld.multi)
  set_property(
    GLOBAL
    PROPERTY LD_SCRIPT
              ${NUTTX_BOARD_ABS_DIR}/scripts/kernel-space-cmake.ld.multi)
else()
  if("${CONFIG_ARCH_BOARD_CUSTOM_NAME}" STREQUAL "nsh")
    set_property(GLOBAL PROPERTY LD_SCRIPT
                                 ${NUTTX_BOARD_ABS_DIR}/scripts/ld.script)
  else()
    set_property(
      GLOBAL PROPERTY LD_SCRIPT
                      ${NUTTX_BOARD_ABS_DIR}/scripts/ld.script.multi)
  endif()
endif()

set(CMAKE_BIN_CMD ${CMAKE_OBJCOPY} -O binary nuttx nuttx.bin)
set(CMAKE_HEX_CMD ${CMAKE_OBJCOPY} --change-addresses 0x`${CMAKE_OBJDUMP} -h vela_${CONFIG_ARCH_BOARD_CUSTOM_NAME}.elf | grep "\.text" | awk '{print $$4}'`
                                   -I binary -O ihex nuttx.bin vela_${CONFIG_ARCH_BOARD_CUSTOM_NAME}.hex)

set(CMAKE_USER_BIN_CMD ${CMAKE_OBJCOPY} -O binary nuttx_user nuttx_user.bin)
set(CMAKE_USER_HEX_CMD ${CMAKE_OBJCOPY} --change-addresses 0x`${CMAKE_OBJDUMP} -h vela_${CONFIG_ARCH_BOARD_CUSTOM_NAME}_user.elf | grep "\.userspace" | awk '{print $$4}'`
                                        -I binary -O ihex nuttx_user.bin vela_${CONFIG_ARCH_BOARD_CUSTOM_NAME}_user.hex)

if(CONFIG_BUILD_PROTECTED)
  add_custom_target(
    nuttx_post_build
    DEPENDS nuttx systemmap nuttx_user usermap
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMAND ${CMAKE_BIN_CMD}
    COMMAND cp nuttx vela_${CONFIG_ARCH_BOARD_CUSTOM_NAME}.elf
    COMMAND ${CMAKE_HEX_CMD}
    COMMAND ${CMAKE_USER_BIN_CMD}
    COMMAND cp nuttx_user vela_${CONFIG_ARCH_BOARD_CUSTOM_NAME}_user.elf
    COMMAND ${CMAKE_USER_HEX_CMD})
else()
  add_custom_target(
    nuttx_post_build
    DEPENDS nuttx systemmap
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMAND ${CMAKE_BIN_CMD}
    COMMAND cp nuttx vela_${CONFIG_ARCH_BOARD_CUSTOM_NAME}.elf
    COMMAND ${CMAKE_HEX_CMD})
endif()

add_custom_command(
  TARGET nuttx_post_build
  DEPENDS nuttx systemmap
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
  COMMAND cp nuttx.bin vela_${CONFIG_ARCH_BOARD_CUSTOM_NAME}.bin
  COMMAND cp nuttx.map vela_${CONFIG_ARCH_BOARD_CUSTOM_NAME}.mapfile
  COMMAND cp System.map vela_${CONFIG_ARCH_BOARD_CUSTOM_NAME}.SystemMap)

if(CONFIG_BUILD_PROTECTED)
  add_custom_command(
    TARGET nuttx_post_build
    DEPENDS nuttx usermap
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    # COMMAND cp nuttx_user vela_${CONFIG_ARCH_BOARD_CUSTOM_NAME}_user.elf
    COMMAND cp nuttx_user.bin vela_${CONFIG_ARCH_BOARD_CUSTOM_NAME}_user.bin
    # COMMAND cp nuttx_user.hex vela_${CONFIG_ARCH_BOARD_CUSTOM_NAME}_user.hex
    COMMAND cp nuttx_user.map vela_${CONFIG_ARCH_BOARD_CUSTOM_NAME}_user.mapfile
    COMMAND cp User.map vela_${CONFIG_ARCH_BOARD_CUSTOM_NAME}_user.SystemMap)
endif()
