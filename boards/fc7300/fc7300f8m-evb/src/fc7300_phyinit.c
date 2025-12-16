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

#include <sys/types.h>
#include <stdint.h>
#include <debug.h>

#include "hardware/fc7300_pinmux.h"
#include "hardware/fc7300_pin.h"
#include <arch/board/board.h>
#include "fc7300_enet.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Public Functions
 ****************************************************************************/

#ifdef CONFIG_FC7300_ENET_PHY_BOADINIT

/****************************************************************************
 * Function: fc7300_phy_boardinitialize
 *
 * Description:
 *   Some boards require specialized initialization of the PHY before it can
 *   be be used.  This may include such things as configuring GPIOs,
 *   resetting the PHY, etc. If CONFIG_FC7300_ENET_PHY_BOADINIT is defined
 *   in the configuration then the board specific logic must provide
 *   fc7300_phy_boardinitialize(); The FC7300 Ethernet driver will call this
 *   function one time before it first uses the PHY.
 *
 * Input Parameters:
 *   intf - In the case where there are multiple ethernet devices, this
 *          value identifies which PHY connected to the ethernet device
 *          indexed by intf is to be initialized.
 *
 * Returned Value:
 *   OK on success; Negated errno on failure.
 *
 ****************************************************************************/

int fc7300_phy_boardinitialize(int intf)
{
  fc7300_pinconfig(GPIO_PHY_RESET);
  fc7300_gpiowrite(GPIO_PHY_RESET, GPIO_PHY_RESET_OUT);
  up_mdelay(20u);
  fc7300_gpiowrite(GPIO_PHY_RESET, !GPIO_PHY_RESET_OUT);
  up_mdelay(400u);
  return 0;
}

#endif /* CONFIG_FC7300_ENET_PHY_BOADINIT */
