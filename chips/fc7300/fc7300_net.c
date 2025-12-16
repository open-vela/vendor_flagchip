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

#ifdef CONFIG_FC7300_FLEXCAN
#include "fc7300_flexcan.h"
#endif
#ifdef CONFIG_FC7300_LIN
#include "fc7300_lin.h"
#endif
#ifdef CONFIG_FC7300_ENET
#include "fc7300_enet.h"
#endif

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: arm_netinitialize
 *
 * Description:
 *   Initialize the enabled net device interfaces.  If there are more
 *   different network devices in the chip, then board-specific logic will
 *   have to provide this function to determine which, if any, network
 *   devices should be initialized.
 *
 ****************************************************************************/

#if defined(CONFIG_NET) && !defined(CONFIG_NETDEV_LATEINIT)
void arm_netinitialize(void)
{
#ifdef CONFIG_NET_CAN
#ifdef CONFIG_FC7300_FLEXCAN0_SOCKET
  fc7300_socket_caninitialize(0);
#endif

#ifdef CONFIG_FC7300_FLEXCAN1_SOCKET
  fc7300_socket_caninitialize(1);
#endif

#ifdef CONFIG_FC7300_FLEXCAN2_SOCKET
  fc7300_socket_caninitialize(2);
#endif

#ifdef CONFIG_FC7300_FLEXCAN3_SOCKET
  fc7300_socket_caninitialize(3);
#endif

#ifdef CONFIG_FC7300_FLEXCAN4_SOCKET
  fc7300_socket_caninitialize(4);
#endif

#ifdef CONFIG_FC7300_FLEXCAN5_SOCKET
  fc7300_socket_caninitialize(5);
#endif

#ifdef CONFIG_FC7300_FLEXCAN6_SOCKET
  fc7300_socket_caninitialize(6);
#endif

#ifdef CONFIG_FC7300_FLEXCAN7_SOCKET
  fc7300_socket_caninitialize(7);
#endif

#ifdef CONFIG_FC7300_FLEXCAN8_SOCKET
  fc7300_socket_caninitialize(8);
#endif

#ifdef CONFIG_FC7300_FLEXCAN9_SOCKET
  fc7300_socket_caninitialize(9);
#endif

#ifdef CONFIG_FC7300_LIN0
  fc7300_lininitialize(0);
#endif

#ifdef CONFIG_FC7300_LIN1
  fc7300_lininitialize(1);
#endif

#ifdef CONFIG_FC7300_LIN2
  fc7300_lininitialize(2);
#endif

#ifdef CONFIG_FC7300_LIN3
  fc7300_lininitialize(3);
#endif

#ifdef CONFIG_FC7300_LIN4
  fc7300_lininitialize(4);
#endif

#ifdef CONFIG_FC7300_LIN5
  fc7300_lininitialize(5);
#endif

#ifdef CONFIG_FC7300_LIN6
  fc7300_lininitialize(6);
#endif

#ifdef CONFIG_FC7300_LIN7
  fc7300_lininitialize(7);
#endif

#ifdef CONFIG_FC7300_LIN8
  fc7300_lininitialize(8);
#endif

#ifdef CONFIG_FC7300_LIN9
  fc7300_lininitialize(9);
#endif

#ifdef CONFIG_FC7300_LIN10
  fc7300_lininitialize(10);
#endif

#ifdef CONFIG_FC7300_LIN11
  fc7300_lininitialize(11);
#endif

#ifdef CONFIG_FC7300_LIN12
  fc7300_lininitialize(12);
#endif

#ifdef CONFIG_FC7300_LIN13
  fc7300_lininitialize(13);
#endif

#ifdef CONFIG_FC7300_LIN14
  fc7300_lininitialize(14);
#endif

#ifdef CONFIG_FC7300_LIN15
  fc7300_lininitialize(15);
#endif

#ifdef CONFIG_FC7300_LIN16
  fc7300_lininitialize(16);
#endif

#ifdef CONFIG_FC7300_LIN17
  fc7300_lininitialize(17);
#endif
#ifdef CONFIG_FC7300_ENET0
  fc7300_enetinitialize(0);
#endif
#endif
}
#endif /* CONFIG_NETDEV_LATEINIT */
