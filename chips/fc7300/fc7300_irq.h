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

#ifndef __VENDOR_FLAGCHIP_CHIPS_FC7300_FC7300_IRQ_H
#define __VENDOR_FLAGCHIP_CHIPS_FC7300_FC7300_IRQ_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>
#include <stdint.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#define OS_NISRTHREAD 9

/****************************************************************************
 * Public Data
 ****************************************************************************/

extern uint8_t isr_cfg[OS_NISRTHREAD];

enum isr_thread_e {
    TSTMP_ISR_THREAD,
    DMA_ISR_THREAD,
    FCIIC_ISR_THREAD,
    FLEXCAN_ISR_THREAD,
    LIN_ISR_THREAD,
    GPIO_ISR_THREAD,
    FCUART_ISR_THREAD,
    ADC_ISR_THREAD,
    CAP_ISR_THREAD
};

/****************************************************************************
 * Public Functions Prototypes
 ****************************************************************************/

void fc7300_clearpendingisr(int irq);

#endif /* __VENDOR_FLAGCHIP_CHIPS_FC7300_FC7300_IRQ_H */
