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

#ifndef __VENDOR_FLAGCHIP_CHIPS_FC7300_FC7300_EIM_H
#define __VENDOR_FLAGCHIP_CHIPS_FC7300_FC7300_EIM_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>
#include <stdint.h>

/****************************************************************************
 * Public Data
 ****************************************************************************/

#ifndef __ASSEMBLY__

#undef EXTERN
#if defined(__cplusplus)
#define EXTERN extern "C"
extern "C"
{
#else
#define EXTERN extern
#endif

typedef struct
{
  unsigned int r0;
  unsigned int r1;
  unsigned int r2;
  unsigned int r3;
  unsigned int r12;
  unsigned int lr;
  unsigned int pc;
  unsigned int psr;
} Exception_Type;

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

/****************************************************************************
 * Name: fc7300_eim_run
 *
 * Description:
 *   Performing injection self-test using the EIM module.
 *
 ****************************************************************************/

void fc7300_eim_run(void);

/****************************************************************************
 * Name: fc7300_memmanage_isr
 *
 * Description:
 *   memmanage fault handling assembly function.
 *
 ****************************************************************************/

extern void fc7300_memmanage_isr(void);

/****************************************************************************
 * Name: fc7300_memmanage_process
 *
 * Description:
 *   memmanage fault handling c function.
 *
 ****************************************************************************/

void fc7300_memmanage_process(Exception_Type *pFrame);

/****************************************************************************
 * Name: fc7300_busfault_isr
 *
 * Description:
 *   bus fault handling assembly function.
 *
 ****************************************************************************/

extern void fc7300_busfault_isr(void);

/****************************************************************************
 * Name: fc7300_busfault_process
 *
 * Description:
 *   bus fault handling c function.
 *
 ****************************************************************************/

void fc7300_busfault_process(Exception_Type *pFrame);

#undef EXTERN
#if defined(__cplusplus)
}
#endif

#endif /* __ASSEMBLY__ */
#endif /* __VENDOR_FLAGCHIP_CHIPS_FC7300_FC7300_EIM_H */
