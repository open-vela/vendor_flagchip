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

#ifndef __VENDOR_FLAGCHIP_CHIPS_FC7300_FC7300_PROGMEM_H
#define __VENDOR_FLAGCHIP_CHIPS_FC7300_FC7300_PROGMEM_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>
#include <nuttx/compiler.h>

#include <sys/types.h>
#include <stdint.h>
#include <stdbool.h>

#include "arm_internal.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Base address of the flash segment used for progmem. */

#define FC7300_PROGMEM_START_ADDR            0x04000000U

#define FC7300_PROGMEM_BLOCK_COUNT           1

#define FC7300_PROGMEM_BLOCK_SIZE            0x00040000U

#define FC7300_PROGMEM_BLOCK_SECTOR_SIZE     0x2000U

#define FC7300_PROGMEM_PAGE_SIZE             16

#define FC7300_PROGRAM_PAGE_MAX_SIZE         0x80U    /* 128 bytes  */

#define FC7300_PROGMEM_SECTOR_COUNT          FC7300_PROGMEM_BLOCK_SIZE / FC7300_PROGMEM_BLOCK_SECTOR_SIZE

#define FC7300_PROGMEM_PAGE_COUNT            (FC7300_PROGMEM_BLOCK_SIZE / FC7300_PROGMEM_PAGE_SIZE)

#define FC7300_PROGMEM_ERASEDVAL             (0xffu)

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

void fc7300_progmem_init(void);

#endif /* __VENDOR_FLAGCHIP_CHIPS_FC7300_FC7300_PROGMEM_H */
