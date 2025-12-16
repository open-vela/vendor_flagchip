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
#include <nuttx/irq.h>

/****************************************************************************
 * Public Symbols
 ****************************************************************************/

  .globl    fc7300_memmanage_isr
  .globl    fc7300_busfault_isr
#ifdef __ghs__
  .thumb2
#else
  .syntax    unified
  .thumb
#endif
  .file    "fc7300_eim_asm.S"

/****************************************************************************
 * .text
 ****************************************************************************/

  .text

#ifdef __ghs__
  .type  fc7300_memmanage_isr, $function
#else
  .thumb_func
  .type  fc7300_memmanage_isr, function
#endif

fc7300_memmanage_isr:
  tst LR, #4
  ite eq
  mrseq r0, msp
  mrsne r0, psp
  B fc7300_memmanage_process
  .size  fc7300_memmanage_isr, . - fc7300_memmanage_isr

#ifdef __ghs__
  .type  fc7300_busfault_isr, $function
#else
  .thumb_func
  .type  fc7300_busfault_isr, function
#endif

fc7300_busfault_isr:
  tst LR, #4
  ite eq
  mrseq r0, msp
  mrsne r0, psp
  B fc7300_busfault_process
  .size  fc7300_busfault_isr, . - fc7300_busfault_isr

  .end
