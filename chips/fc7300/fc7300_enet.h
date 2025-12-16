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

#ifndef __VENDOR_FLAGCHIP_CHIPS_FC7300_FC7300_ENET_H
#define __VENDOR_FLAGCHIP_CHIPS_FC7300_FC7300_ENET_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>

#include "hardware/fc7300_enet.h"

#ifdef CONFIG_FC7300_ENET

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
 * Function: fc7300_enetinitialize
 *
 * Description:
 *   Initialize the ethernet controller and driver
 *
 * Input Parameters:
 *   intf - In the case where there are multiple ethernet devices, this
 *          value identifies which ethernet device is to be initialized.
 *
 * Returned Value:
 *   0 on success; Negated errno on failure.
 *
 * Assumptions:
 *
 ****************************************************************************/

int fc7300_enetinitialize(int intf);

#ifdef CONFIG_FC7300_ENET_PHY_BOADINIT

/****************************************************************************
 * Function: fc7300_phy_boardinitialize
 *
 * Description:
 *   Some boards require specialized initialization of the PHY before it can
 *   be be used.  This may include such things as configuring GPIOs,
 *   resetting the PHY, etc.  If CONFIG_FC7300_ENET_PHY_BOADINIT is defined
 *   in the configuration then the board specific logic must provide
 *   fc7300_phy_boardinitialize();  The flagchip Ethernet driver will call
 *   this function one time before it first uses the PHY.
 *
 * Input Parameters:
 *   intf - Always zero for now.
 *
 * Returned Value:
 *   OK on success; Negated errno on failure.
 *
 ****************************************************************************/

int fc7300_phy_boardinitialize(int intf);
#endif

#undef EXTERN
#if defined(__cplusplus)
}
#endif

#endif /* __ASSEMBLY__ */
#endif /* CONFIG_FC7300_ENET */
#endif /* __VENDOR_FLAGCHIP_CHIPS_FC7300_FC7300_ENET_H */
