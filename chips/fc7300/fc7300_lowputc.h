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

#ifndef __VENDOR_FLAGCHIP_CHIPS_FC7300_FC7300_LOWPUTC_H
#define __VENDOR_FLAGCHIP_CHIPS_FC7300_FC7300_LOWPUTC_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>
#include <nuttx/arch.h>
#include <nuttx/irq.h>

#include "chip.h"
#include "fc7300_config.h"

/****************************************************************************
 * Public Types
 ****************************************************************************/

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

/****************************************************************************
 * Name: fc7300_lowsetup
 *
 * Description:
 *   Called at the very beginning of _start.  Performs low level
 *   initialization including setup of the console UART.  This UART done
 *   early so that the serial console is available for debugging very early
 *   in the boot sequence.
 *
 ****************************************************************************/

void fc7300_lowsetup(void);

/****************************************************************************
 * Name: fc7300_fcuart_processbaud
 *
 * Description:
 *   Process get OverSamp and SBR
 *
 ****************************************************************************/

#if defined(USE_SERIALDRIVER) || defined(CONFIG_FC7300_FCUART)
int fc7300_fcuart_processbaud(uint32_t base, uint32_t baud,
                              uint32_t *osr, uint16_t *sbr);
#endif

/****************************************************************************
 * Name: fc7300_lowputc
 *
 * Description:
 *   Output a byte with as few system dependencies as possible.  This will
 *   even work BEFORE the console is initialized if we are booting from U-
 *   Boot (and the same UART is used for the console, of course.)
 *
 ****************************************************************************/

#if !defined(CONFIG_NO_FCUART_SERIAL_CONSOLE) && defined(CONFIG_DEBUG_FEATURES)
void fc7300_lowputc(int ch);
#else
#  define fc7300_lowputc(ch)
#endif

#endif /* __VENDOR_FLAGCHIP_CHIPS_FC7300_FC7300_LOWPUTC_H */
