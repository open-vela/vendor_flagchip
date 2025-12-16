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

#ifndef __VENDOR_FLAGCHIP_CHIPS_FC7300_FC7300_ERM_H
#define __VENDOR_FLAGCHIP_CHIPS_FC7300_FC7300_ERM_H

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

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

/****************************************************************************
 * Name: fc7300_erm_init
 *
 * Description:
 *   Hardware level initializes the ERM Driver settings based on the
 * configuration.
 *
 ****************************************************************************/

void fc7300_erm_init(void);

/****************************************************************************
 * Name: fc7300_erm_irq_attach
 *
 * Description:
 *   Interrupt registration function after the system starts.
 *
 ****************************************************************************/

void fc7300_erm_irq_attach(void);

/****************************************************************************
 * Name: fc7300_erm_irq_handler
 *
 * Description:
 *   Interrupt entry function used by EIM.
 *
 ****************************************************************************/

void fc7300_erm_irq_handler(void);

/****************************************************************************
 * Name: fc7300_erm_clear_flag
 *
 * Description:
 *   Clear all error flags in the ERM (Error Reporting Module) status registers.
 *
 ****************************************************************************/

void fc7300_erm_clear_flag(void);

#undef EXTERN
#if defined(__cplusplus)
}
#endif

#endif /* __ASSEMBLY__ */
#endif /* __VENDOR_FLAGCHIP_CHIPS_FC7300_FC7300_ERM_H */
