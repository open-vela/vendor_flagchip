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

#ifndef __VENDOR_FLAGCHIP_CHIPS_FC7300_FC7300_UART_H
#define __VENDOR_FLAGCHIP_CHIPS_FC7300_FC7300_UART_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>

#include "arm_internal.h"
#include "fc7300_config.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Assume DMA is not used on the console UART */

#undef SERIAL_HAVE_CONSOLE_RXDMA
#undef SERIAL_HAVE_CONSOLE_TXDMA

#if !defined(CONFIG_FC7300_FCUART) || !defined(CONFIG_ARCH_DMA)
#  undef CONFIG_FCUART0_RXDMA
#  undef CONFIG_FCUART0_TXDMA
#  undef CONFIG_FCUART1_RXDMA
#  undef CONFIG_FCUART1_TXDMA
#  undef CONFIG_FCUART2_RXDMA
#  undef CONFIG_FCUART2_TXDMA
#  undef CONFIG_FCUART3_RXDMA
#  undef CONFIG_FCUART3_TXDMA
#  undef CONFIG_FCUART4_RXDMA
#  undef CONFIG_FCUART4_TXDMA
#  undef CONFIG_FCUART5_RXDMA
#  undef CONFIG_FCUART5_TXDMA
#  undef CONFIG_FCUART6_RXDMA
#  undef CONFIG_FCUART6_TXDMA
#  undef CONFIG_FCUART7_RXDMA
#  undef CONFIG_FCUART7_TXDMA
#  undef CONFIG_FCUART8_RXDMA
#  undef CONFIG_FCUART8_TXDMA
#  undef CONFIG_FCUART9_RXDMA
#  undef CONFIG_FCUART9_TXDMA
#  undef CONFIG_FCUART10_RXDMA
#  undef CONFIG_FCUART10_TXDMA
#  undef CONFIG_FCUART11_RXDMA
#  undef CONFIG_FCUART11_TXDMA
#  undef CONFIG_FCUART12_RXDMA
#  undef CONFIG_FCUART12_TXDMA
#  undef CONFIG_FCUART13_RXDMA
#  undef CONFIG_FCUART13_TXDMA
#  undef CONFIG_FCUART14_RXDMA
#  undef CONFIG_FCUART14_TXDMA
#  undef CONFIG_FCUART15_RXDMA
#  undef CONFIG_FCUART15_TXDMA
#  undef CONFIG_FCUART16_RXDMA
#  undef CONFIG_FCUART16_TXDMA
#  undef CONFIG_FCUART17_RXDMA
#  undef CONFIG_FCUART17_TXDMA
#endif

/* Disable the DMA configuration on all unused FCUARTs */

#ifndef CONFIG_FC7300_FCUART0
#  undef CONFIG_FCUART0_RXDMA
#  undef CONFIG_FCUART0_TXDMA
#endif

#ifndef CONFIG_FC7300_FCUART1
#  undef CONFIG_FCUART1_RXDMA
#  undef CONFIG_FCUART1_TXDMA
#endif

#ifndef CONFIG_FC7300_FCUART2
#  undef CONFIG_FCUART2_RXDMA
#  undef CONFIG_FCUART2_TXDMA
#endif

#ifndef CONFIG_FC7300_FCUART3
#  undef CONFIG_FCUART3_RXDMA
#  undef CONFIG_FCUART3_TXDMA
#endif

#ifndef CONFIG_FC7300_FCUART4
#  undef CONFIG_FCUART4_RXDMA
#  undef CONFIG_FCUART4_TXDMA
#endif

#ifndef CONFIG_FC7300_FCUART5
#  undef CONFIG_FCUART5_RXDMA
#  undef CONFIG_FCUART5_TXDMA
#endif

#ifndef CONFIG_FC7300_FCUART6
#  undef CONFIG_FCUART6_RXDMA
#  undef CONFIG_FCUART6_TXDMA
#endif

#ifndef CONFIG_FC7300_FCUART7
#  undef CONFIG_FCUART7_RXDMA
#  undef CONFIG_FCUART7_TXDMA
#endif

#ifndef CONFIG_FC7300_FCUART8
#  undef CONFIG_FCUART8_RXDMA
#  undef CONFIG_FCUART8_TXDMA
#endif

#ifndef CONFIG_FC7300_FCUART9
#  undef CONFIG_FCUART9_RXDMA
#  undef CONFIG_FCUART9_TXDMA
#endif

#ifndef CONFIG_FC7300_FCUART10
#  undef CONFIG_FCUART10_RXDMA
#  undef CONFIG_FCUART10_TXDMA
#endif

#ifndef CONFIG_FC7300_FCUART11
#  undef CONFIG_FCUART11_RXDMA
#  undef CONFIG_FCUART11_TXDMA
#endif

#ifndef CONFIG_FC7300_FCUART12
#  undef CONFIG_FCUART12_RXDMA
#  undef CONFIG_FCUART12_TXDMA
#endif

#ifndef CONFIG_FC7300_FCUART13
#  undef CONFIG_FCUART13_RXDMA
#  undef CONFIG_FCUART13_TXDMA
#endif

#ifndef CONFIG_FC7300_FCUART14
#  undef CONFIG_FCUART14_RXDMA
#  undef CONFIG_FCUART14_TXDMA
#endif

#ifndef CONFIG_FC7300_FCUART15
#  undef CONFIG_FCUART15_RXDMA
#  undef CONFIG_FCUART15_TXDMA
#endif

#ifndef CONFIG_FC7300_FCUART16
#  undef CONFIG_FCUART16_RXDMA
#  undef CONFIG_FCUART16_TXDMA
#endif

#ifndef CONFIG_FC7300_FCUART17
#  undef CONFIG_FCUART17_RXDMA
#  undef CONFIG_FCUART17_TXDMA
#endif

/* Is RX DMA available on any (enabled) FCUART? */

#undef SERIAL_HAVE_RXDMA
#if defined(CONFIG_FCUART0_RXDMA) || defined(CONFIG_FCUART1_RXDMA) || \
    defined(CONFIG_FCUART2_RXDMA) || defined(CONFIG_FCUART3_RXDMA) || \
    defined(CONFIG_FCUART4_RXDMA) || defined(CONFIG_FCUART5_RXDMA) || \
    defined(CONFIG_FCUART6_RXDMA) || defined(CONFIG_FCUART7_RXDMA) || \
    defined(CONFIG_FCUART8_RXDMA) || defined(CONFIG_FCUART9_RXDMA) || \
    defined(CONFIG_FCUART10_RXDMA) || defined(CONFIG_FCUART11_RXDMA) || \
    defined(CONFIG_FCUART12_RXDMA) || defined(CONFIG_FCUART13_RXDMA) || \
    defined(CONFIG_FCUART14_RXDMA) || defined(CONFIG_FCUART15_RXDMA) || \
    defined(CONFIG_FCUART16_RXDMA) || defined(CONFIG_FCUART17_RXDMA)
#  define SERIAL_HAVE_RXDMA 1
#endif

/* Is TX DMA available on any (enabled) FCUART? */
#undef SERIAL_HAVE_TXDMA
#if defined(CONFIG_FCUART0_TXDMA) || defined(CONFIG_FCUART1_TXDMA) || \
    defined(CONFIG_FCUART2_TXDMA) || defined(CONFIG_FCUART3_TXDMA) || \
    defined(CONFIG_FCUART4_TXDMA) || defined(CONFIG_FCUART5_TXDMA) || \
    defined(CONFIG_FCUART6_TXDMA) || defined(CONFIG_FCUART7_TXDMA) || \
    defined(CONFIG_FCUART8_TXDMA) || defined(CONFIG_FCUART9_TXDMA) || \
    defined(CONFIG_FCUART10_TXDMA) || defined(CONFIG_FCUART11_TXDMA) || \
    defined(CONFIG_FCUART12_TXDMA) || defined(CONFIG_FCUART13_TXDMA) || \
    defined(CONFIG_FCUART14_TXDMA) || defined(CONFIG_FCUART15_TXDMA) || \
    defined(CONFIG_FCUART16_TXDMA) || defined(CONFIG_FCUART17_TXDMA)
#    define SERIAL_HAVE_TXDMA 1
#endif

/* Is RX DMA used on all (enabled) FCUARTs */

#define SERIAL_HAVE_ONLY_RXDMA 1
#if defined(CONFIG_FC7300_FCUART0) && !defined(CONFIG_FCUART0_RXDMA)
#  undef SERIAL_HAVE_ONLY_RXDMA
#elif defined(CONFIG_FC7300_FCUART1) && !defined(CONFIG_FCUART1_RXDMA)
#  undef SERIAL_HAVE_ONLY_RXDMA
#elif defined(CONFIG_FC7300_FCUART2) && !defined(CONFIG_FCUART2_RXDMA)
#  undef SERIAL_HAVE_ONLY_RXDMA
#elif defined(CONFIG_FC7300_FCUART3) && !defined(CONFIG_FCUART3_RXDMA)
#  undef SERIAL_HAVE_ONLY_RXDMA
#elif defined(CONFIG_FC7300_FCUART4) && !defined(CONFIG_FCUART4_RXDMA)
#  undef SERIAL_HAVE_ONLY_RXDMA
#elif defined(CONFIG_FC7300_FCUART5) && !defined(CONFIG_FCUART5_RXDMA)
#  undef SERIAL_HAVE_ONLY_RXDMA
#elif defined(CONFIG_FC7300_FCUART6) && !defined(CONFIG_FCUART6_RXDMA)
#  undef SERIAL_HAVE_ONLY_RXDMA
#elif defined(CONFIG_FC7300_FCUART7) && !defined(CONFIG_FCUART7_RXDMA)
#  undef SERIAL_HAVE_ONLY_RXDMA
#elif defined(CONFIG_FC7300_FCUART8) && !defined(CONFIG_FCUART8_RXDMA)
#  undef SERIAL_HAVE_ONLY_RXDMA
#elif defined(CONFIG_FC7300_FCUART9) && !defined(CONFIG_FCUART9_RXDMA)
#  undef SERIAL_HAVE_ONLY_RXDMA
#elif defined(CONFIG_FC7300_FCUART10) && !defined(CONFIG_FCUART10_RXDMA)
#  undef SERIAL_HAVE_ONLY_RXDMA
#elif defined(CONFIG_FC7300_FCUART11) && !defined(CONFIG_FCUART11_RXDMA)
#  undef SERIAL_HAVE_ONLY_RXDMA
#elif defined(CONFIG_FC7300_FCUART12) && !defined(CONFIG_FCUART12_RXDMA)
#  undef SERIAL_HAVE_ONLY_RXDMA
#elif defined(CONFIG_FC7300_FCUART13) && !defined(CONFIG_FCUART13_RXDMA)
#  undef SERIAL_HAVE_ONLY_RXDMA
#elif defined(CONFIG_FC7300_FCUART14) && !defined(CONFIG_FCUART14_RXDMA)
#  undef SERIAL_HAVE_ONLY_RXDMA
#elif defined(CONFIG_FC7300_FCUART15) && !defined(CONFIG_FCUART15_RXDMA)
#  undef SERIAL_HAVE_ONLY_RXDMA
#elif defined(CONFIG_FC7300_FCUART16) && !defined(CONFIG_FCUART16_RXDMA)
#  undef SERIAL_HAVE_ONLY_RXDMA
#elif defined(CONFIG_FC7300_FCUART17) && !defined(CONFIG_FCUART17_RXDMA)
#  undef SERIAL_HAVE_ONLY_RXDMA
#endif

/* Is TX DMA used on all (enabled) FCUARTs */

#define SERIAL_HAVE_ONLY_TXDMA 1
#if defined(CONFIG_FC7300_FCUART0) && !defined(CONFIG_FCUART0_TXDMA)
#  undef SERIAL_HAVE_ONLY_TXDMA
#elif defined(CONFIG_FC7300_FCUART1) && !defined(CONFIG_FCUART1_TXDMA)
#  undef SERIAL_HAVE_ONLY_TXDMA
#elif defined(CONFIG_FC7300_FCUART2) && !defined(CONFIG_FCUART2_TXDMA)
#  undef SERIAL_HAVE_ONLY_TXDMA
#elif defined(CONFIG_FC7300_FCUART3) && !defined(CONFIG_FCUART3_TXDMA)
#  undef SERIAL_HAVE_ONLY_TXDMA
#elif defined(CONFIG_FC7300_FCUART4) && !defined(CONFIG_FCUART4_TXDMA)
#  undef SERIAL_HAVE_ONLY_TXDMA
#elif defined(CONFIG_FC7300_FCUART5) && !defined(CONFIG_FCUART5_TXDMA)
#  undef SERIAL_HAVE_ONLY_TXDMA
#elif defined(CONFIG_FC7300_FCUART6) && !defined(CONFIG_FCUART6_TXDMA)
#  undef SERIAL_HAVE_ONLY_TXDMA
#elif defined(CONFIG_FC7300_FCUART7) && !defined(CONFIG_FCUART7_TXDMA)
#  undef SERIAL_HAVE_ONLY_TXDMA
#elif defined(CONFIG_FC7300_FCUART8) && !defined(CONFIG_FCUART8_TXDMA)
#  undef SERIAL_HAVE_ONLY_TXDMA
#elif defined(CONFIG_FC7300_FCUART9) && !defined(CONFIG_FCUART9_TXDMA)
#  undef SERIAL_HAVE_ONLY_TXDMA
#elif defined(CONFIG_FC7300_FCUART10) && !defined(CONFIG_FCUART10_TXDMA)
#  undef SERIAL_HAVE_ONLY_TXDMA
#elif defined(CONFIG_FC7300_FCUART11) && !defined(CONFIG_FCUART11_TXDMA)
#  undef SERIAL_HAVE_ONLY_TXDMA
#elif defined(CONFIG_FC7300_FCUART12) && !defined(CONFIG_FCUART12_TXDMA)
#  undef SERIAL_HAVE_ONLY_TXDMA
#elif defined(CONFIG_FC7300_FCUART13) && !defined(CONFIG_FCUART13_TXDMA)
#  undef SERIAL_HAVE_ONLY_TXDMA
#elif defined(CONFIG_FC7300_FCUART14) && !defined(CONFIG_FCUART14_TXDMA)
#  undef SERIAL_HAVE_ONLY_TXDMA
#elif defined(CONFIG_FC7300_FCUART15) && !defined(CONFIG_FCUART15_TXDMA)
#  undef SERIAL_HAVE_ONLY_TXDMA
#elif defined(CONFIG_FC7300_FCUART16) && !defined(CONFIG_FCUART16_TXDMA)
#  undef SERIAL_HAVE_ONLY_TXDMA
#elif defined(CONFIG_FC7300_FCUART17) && !defined(CONFIG_FCUART17_TXDMA)
#  undef SERIAL_HAVE_ONLY_TXDMA
#endif

#undef SERIAL_HAVE_ONLY_DMA
#if defined(SERIAL_HAVE_ONLY_RXDMA) && defined(SERIAL_HAVE_ONLY_TXDMA)
#define SERIAL_HAVE_ONLY_DMA
#endif

/* Verify that DMA has been enabled and the DMA channel has been defined.
 */

#if defined(SERIAL_HAVE_TXDMA) || defined(SERIAL_HAVE_RXDMA)
#  ifndef CONFIG_FC7300_DMA
#    error FC7300 FCUART receive or transmit DMA requires CONFIG_FC7300_DMA
#  endif
#endif

/****************************************************************************
 * Public Types
 ****************************************************************************/

/****************************************************************************
 * Inline Functions
 ****************************************************************************/

#ifndef __ASSEMBLY__

/****************************************************************************
 * Public Data
 ****************************************************************************/

#undef EXTERN
#if defined(__cplusplus)
#define EXTERN extern "C"
extern "C"
{
#else
#define EXTERN extern
#endif

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

/****************************************************************************
 * Name: fc7300_earlyserialinit
 *
 * Description:
 *   Performs the low level UART initialization early in debug so that the
 *   serial console will be available during bootup.  This must be called
 *   before arm_serialinit.
 *
 ****************************************************************************/

#ifdef USE_EARLYSERIALINIT
void fc7300_earlyserialinit(void);
#endif

#undef EXTERN
#if defined(__cplusplus)
}
#endif

#endif /* __ASSEMBLY__ */
#endif /* __VENDOR_FLAGCHIP_CHIPS_FC7300_FC7300_UART_H */
