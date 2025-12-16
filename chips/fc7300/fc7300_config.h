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

#ifndef __VENDOR_FLAGCHIP_CHIPS_FC7300_FC7300_CONFIG_H
#define __VENDOR_FLAGCHIP_CHIPS_FC7300_FC7300_CONFIG_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Is there a serial console? There should be at most one defined.
 * It could be on any FCUARTn, n=0-17
 */

#if !defined(CONFIG_NO_FCUART_SERIAL_CONSOLE)
#  if defined(CONFIG_FCUART0_SERIAL_CONSOLE)
#    define FC7300_CONSOLE_BASE     FC7300_FCUART0_BASE
#    define FC7300_CONSOLE_BAUD     CONFIG_FCUART0_BAUD
#    define FC7300_CONSOLE_BITS     CONFIG_FCUART0_BITS
#    define FC7300_CONSOLE_PARITY   CONFIG_FCUART0_PARITY
#    define FC7300_CONSOLE_2STOP    CONFIG_FCUART0_2STOP
#  elif defined(CONFIG_FCUART1_SERIAL_CONSOLE)
#    define FC7300_CONSOLE_BASE     FC7300_FCUART1_BASE
#    define FC7300_CONSOLE_BAUD     CONFIG_FCUART1_BAUD
#    define FC7300_CONSOLE_BITS     CONFIG_FCUART1_BITS
#    define FC7300_CONSOLE_PARITY   CONFIG_FCUART1_PARITY
#    define FC7300_CONSOLE_2STOP    CONFIG_FCUART1_2STOP
#  elif defined(CONFIG_FCUART2_SERIAL_CONSOLE)
#    define FC7300_CONSOLE_BASE     FC7300_FCUART2_BASE
#    define FC7300_CONSOLE_BAUD     CONFIG_FCUART2_BAUD
#    define FC7300_CONSOLE_BITS     CONFIG_FCUART2_BITS
#    define FC7300_CONSOLE_PARITY   CONFIG_FCUART2_PARITY
#    define FC7300_CONSOLE_2STOP    CONFIG_FCUART2_2STOP
#  elif defined(CONFIG_FCUART3_SERIAL_CONSOLE)
#    define FC7300_CONSOLE_BASE     FC7300_FCUART3_BASE
#    define FC7300_CONSOLE_BAUD     CONFIG_FCUART3_BAUD
#    define FC7300_CONSOLE_BITS     CONFIG_FCUART3_BITS
#    define FC7300_CONSOLE_PARITY   CONFIG_FCUART3_PARITY
#    define FC7300_CONSOLE_2STOP    CONFIG_FCUART3_2STOP
#  elif defined(CONFIG_FCUART4_SERIAL_CONSOLE)
#    define FC7300_CONSOLE_BASE     FC7300_FCUART4_BASE
#    define FC7300_CONSOLE_BAUD     CONFIG_FCUART4_BAUD
#    define FC7300_CONSOLE_BITS     CONFIG_FCUART4_BITS
#    define FC7300_CONSOLE_PARITY   CONFIG_FCUART4_PARITY
#    define FC7300_CONSOLE_2STOP    CONFIG_FCUART4_2STOP
#  elif defined(CONFIG_FCUART5_SERIAL_CONSOLE)
#    define FC7300_CONSOLE_BASE     FC7300_FCUART5_BASE
#    define FC7300_CONSOLE_BAUD     CONFIG_FCUART5_BAUD
#    define FC7300_CONSOLE_BITS     CONFIG_FCUART5_BITS
#    define FC7300_CONSOLE_PARITY   CONFIG_FCUART5_PARITY
#    define FC7300_CONSOLE_2STOP    CONFIG_FCUART5_2STOP
#  elif defined(CONFIG_FCUART6_SERIAL_CONSOLE)
#    define FC7300_CONSOLE_BASE     FC7300_FCUART6_BASE
#    define FC7300_CONSOLE_BAUD     CONFIG_FCUART6_BAUD
#    define FC7300_CONSOLE_BITS     CONFIG_FCUART6_BITS
#    define FC7300_CONSOLE_PARITY   CONFIG_FCUART6_PARITY
#    define FC7300_CONSOLE_2STOP    CONFIG_FCUART6_2STOP
#  elif defined(CONFIG_FCUART7_SERIAL_CONSOLE)
#    define FC7300_CONSOLE_BASE     FC7300_FCUART7_BASE
#    define FC7300_CONSOLE_BAUD     CONFIG_FCUART7_BAUD
#    define FC7300_CONSOLE_BITS     CONFIG_FCUART7_BITS
#    define FC7300_CONSOLE_PARITY   CONFIG_FCUART7_PARITY
#    define FC7300_CONSOLE_2STOP    CONFIG_FCUART7_2STOP
#  elif defined(CONFIG_FCUART8_SERIAL_CONSOLE)
#    define FC7300_CONSOLE_BASE     FC7300_FCUART8_BASE
#    define FC7300_CONSOLE_BAUD     CONFIG_FCUART8_BAUD
#    define FC7300_CONSOLE_BITS     CONFIG_FCUART8_BITS
#    define FC7300_CONSOLE_PARITY   CONFIG_FCUART8_PARITY
#    define FC7300_CONSOLE_2STOP    CONFIG_FCUART8_2STOP
#  elif defined(CONFIG_FCUART9_SERIAL_CONSOLE)
#    define FC7300_CONSOLE_BASE     FC7300_FCUART9_BASE
#    define FC7300_CONSOLE_BAUD     CONFIG_FCUART9_BAUD
#    define FC7300_CONSOLE_BITS     CONFIG_FCUART9_BITS
#    define FC7300_CONSOLE_PARITY   CONFIG_FCUART9_PARITY
#    define FC7300_CONSOLE_2STOP    CONFIG_FCUART9_2STOP
#  elif defined(CONFIG_FCUART10_SERIAL_CONSOLE)
#    define FC7300_CONSOLE_BASE     FC7300_FCUART10_BASE
#    define FC7300_CONSOLE_BAUD     CONFIG_FCUART10_BAUD
#    define FC7300_CONSOLE_BITS     CONFIG_FCUART10_BITS
#    define FC7300_CONSOLE_PARITY   CONFIG_FCUART10_PARITY
#    define FC7300_CONSOLE_2STOP    CONFIG_FCUART10_2STOP
#  elif defined(CONFIG_FCUART11_SERIAL_CONSOLE)
#    define FC7300_CONSOLE_BASE     FC7300_FCUART11_BASE
#    define FC7300_CONSOLE_BAUD     CONFIG_FCUART11_BAUD
#    define FC7300_CONSOLE_BITS     CONFIG_FCUART11_BITS
#    define FC7300_CONSOLE_PARITY   CONFIG_FCUART11_PARITY
#    define FC7300_CONSOLE_2STOP    CONFIG_FCUART11_2STOP
#  elif defined(CONFIG_FCUART12_SERIAL_CONSOLE)
#    define FC7300_CONSOLE_BASE     FC7300_FCUART12_BASE
#    define FC7300_CONSOLE_BAUD     CONFIG_FCUART12_BAUD
#    define FC7300_CONSOLE_BITS     CONFIG_FCUART12_BITS
#    define FC7300_CONSOLE_PARITY   CONFIG_FCUART12_PARITY
#    define FC7300_CONSOLE_2STOP    CONFIG_FCUART12_2STOP
#  elif defined(CONFIG_FCUART13_SERIAL_CONSOLE)
#    define FC7300_CONSOLE_BASE     FC7300_FCUART13_BASE
#    define FC7300_CONSOLE_BAUD     CONFIG_FCUART13_BAUD
#    define FC7300_CONSOLE_BITS     CONFIG_FCUART13_BITS
#    define FC7300_CONSOLE_PARITY   CONFIG_FCUART13_PARITY
#    define FC7300_CONSOLE_2STOP    CONFIG_FCUART13_2STOP
#  elif defined(CONFIG_FCUART14_SERIAL_CONSOLE)
#    define FC7300_CONSOLE_BASE     FC7300_FCUART14_BASE
#    define FC7300_CONSOLE_BAUD     CONFIG_FCUART14_BAUD
#    define FC7300_CONSOLE_BITS     CONFIG_FCUART14_BITS
#    define FC7300_CONSOLE_PARITY   CONFIG_FCUART14_PARITY
#    define FC7300_CONSOLE_2STOP    CONFIG_FCUART14_2STOP
#  elif defined(CONFIG_FCUART15_SERIAL_CONSOLE)
#    define FC7300_CONSOLE_BASE     FC7300_FCUART15_BASE
#    define FC7300_CONSOLE_BAUD     CONFIG_FCUART15_BAUD
#    define FC7300_CONSOLE_BITS     CONFIG_FCUART15_BITS
#    define FC7300_CONSOLE_PARITY   CONFIG_FCUART15_PARITY
#    define FC7300_CONSOLE_2STOP    CONFIG_FCUART15_2STOP
#  elif defined(CONFIG_FCUART16_SERIAL_CONSOLE)
#    define FC7300_CONSOLE_BASE     FC7300_FCUART16_BASE
#    define FC7300_CONSOLE_BAUD     CONFIG_FCUART16_BAUD
#    define FC7300_CONSOLE_BITS     CONFIG_FCUART16_BITS
#    define FC7300_CONSOLE_PARITY   CONFIG_FCUART16_PARITY
#    define FC7300_CONSOLE_2STOP    CONFIG_FCUART16_2STOP
#  elif defined(CONFIG_FCUART17_SERIAL_CONSOLE)
#    define FC7300_CONSOLE_BASE     FC7300_FCUART17_BASE
#    define FC7300_CONSOLE_BAUD     CONFIG_FCUART17_BAUD
#    define FC7300_CONSOLE_BITS     CONFIG_FCUART17_BITS
#    define FC7300_CONSOLE_PARITY   CONFIG_FCUART17_PARITY
#    define FC7300_CONSOLE_2STOP    CONFIG_FCUART17_2STOP
#  endif
#else
#  ifdef CONFIG_DEV_CONSOLE
#    warning "No valid CONFIG_FCUART[n]_SERIAL_CONSOLE Setting"
#  endif
#endif

/****************************************************************************
 * Public Functions Prototypes
 ****************************************************************************/

#endif /* __VENDOR_FLAGCHIP_CHIPS_FC7300_FC7300_CONFIG_H */
