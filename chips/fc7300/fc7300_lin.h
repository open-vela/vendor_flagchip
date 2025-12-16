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

#ifndef __VENDOR_FLAGCHIP_CHIPS_FC7300_FC7300_LIN_H
#define __VENDOR_FLAGCHIP_CHIPS_FC7300_FC7300_LIN_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>

#include "hardware/fc7300_fcuart.h"

#ifdef CONFIG_FC7300_LIN

/****************************************************************************
 * Public Function Prototypes
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
 * Function: fc7300_lininitialize
 *
 * Description:
 *   Initialize the LIN controller and driver
 *
 * Input Parameters:
 *   intf - In the case where there are multiple LIN devices, this value
 *          identifies which LIN device is to be initialized.
 *
 * Returned Value:
 *   0 on success; Negated errno on failure.
 *
 * Assumptions:
 *
 ****************************************************************************/

int fc7300_lininitialize(int intf);

#undef EXTERN
#if defined(__cplusplus)
}
#endif

#endif /* __ASSEMBLY__ */
#endif /* CONFIG_FC7300_LIN */
#endif /* __VENDOR_FLAGCHIP_CHIPS_FC7300_FC7300_LIN_H */
