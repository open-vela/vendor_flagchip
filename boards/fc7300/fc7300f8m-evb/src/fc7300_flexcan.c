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

#include "fc7300_flexcan.h"

#include <nuttx/config.h>

#include <errno.h>
#include <debug.h>
#include <stdio.h>

#include <nuttx/board.h>
#include <nuttx/can/can.h>
#include <arch/board/board.h>

#include "chip.h"
#include "fc7300f8m-evb.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#ifdef CONFIG_FC7300_FLEXCAN0
extern const struct can_filter flexcan0_rxmb_filter[];
#ifdef CONFIG_FC7300_FLEXCAN0_ENHANCED_RX_FIFO
extern const struct can_erf_filter flexcan0_erf_filter[];
#endif
#endif

#ifdef CONFIG_FC7300_FLEXCAN1
extern const struct can_filter flexcan1_rxmb_filter[];
#ifdef CONFIG_FC7300_FLEXCAN1_ENHANCED_RX_FIFO
extern const struct can_erf_filter flexcan1_erf_filter[];
#endif
#endif

#ifdef CONFIG_FC7300_FLEXCAN2
extern const struct can_filter flexcan2_rxmb_filter[];
#ifdef CONFIG_FC7300_FLEXCAN2_ENHANCED_RX_FIFO
extern const struct can_erf_filter flexcan2_erf_filter[];
#endif
#endif

#ifdef CONFIG_FC7300_FLEXCAN3
extern const struct can_filter flexcan3_rxmb_filter[];
#ifdef CONFIG_FC7300_FLEXCAN3_ENHANCED_RX_FIFO
extern const struct can_erf_filter flexcan3_erf_filter[];
#endif
#endif

#ifdef CONFIG_FC7300_FLEXCAN4
extern const struct can_filter flexcan4_rxmb_filter[];
#ifdef CONFIG_FC7300_FLEXCAN4_ENHANCED_RX_FIFO
extern const struct can_erf_filter flexcan4_erf_filter[];
#endif
#endif

#ifdef CONFIG_FC7300_FLEXCAN5
extern const struct can_filter flexcan5_rxmb_filter[];
#ifdef CONFIG_FC7300_FLEXCAN5_ENHANCED_RX_FIFO
extern const struct can_erf_filter flexcan5_erf_filter[];
#endif
#endif

#ifdef CONFIG_FC7300_FLEXCAN6
extern const struct can_filter flexcan6_rxmb_filter[];
#ifdef CONFIG_FC7300_FLEXCAN6_ENHANCED_RX_FIFO
extern const struct can_erf_filter flexcan6_erf_filter[];
#endif
#endif

#ifdef CONFIG_FC7300_FLEXCAN7
extern const struct can_filter flexcan7_rxmb_filter[];
#ifdef CONFIG_FC7300_FLEXCAN7_ENHANCED_RX_FIFO
extern const struct can_erf_filter flexcan7_erf_filter[];
#endif
#endif

#ifdef CONFIG_FC7300_FLEXCAN8
extern const struct can_filter flexcan8_rxmb_filter[];
#ifdef CONFIG_FC7300_FLEXCAN8_ENHANCED_RX_FIFO
extern const struct can_erf_filter flexcan8_erf_filter[];
#endif
#endif

#ifdef CONFIG_FC7300_FLEXCAN9
extern const struct can_filter flexcan9_rxmb_filter[];
#ifdef CONFIG_FC7300_FLEXCAN9_ENHANCED_RX_FIFO
extern const struct can_erf_filter flexcan9_erf_filter[];
#endif
#endif

/****************************************************************************
 * Public Functions
 ****************************************************************************/

#if defined(CONFIG_NET_CAN) && defined(CONFIG_NETDEV_LATEINIT)
/****************************************************************************
 * Name: fc7300_flexcan_socket_setup
 *
 * Description:
 *  Initialize CAN socket driver and register the CAN device
 *
 ****************************************************************************/

#if defined(CONFIG_FC7300_FLEXCAN_SOCKET)

int fc7300_flexcan_socket_setup(void)
{
#  ifdef CONFIG_FC7300_FLEXCAN0_SOCKET
  fc7300_socket_caninitialize(0);
#  endif /* CONFIG_FC7300_FLEXCAN0 */
#  ifdef CONFIG_FC7300_FLEXCAN1_SOCKET
  fc7300_socket_caninitialize(1);
#  endif /* CONFIG_FC7300_FLEXCAN1 */
#  ifdef CONFIG_FC7300_FLEXCAN2_SOCKET
  fc7300_socket_caninitialize(2);
#  endif /* CONFIG_FC7300_FLEXCAN2 */
#  ifdef CONFIG_FC7300_FLEXCAN3_SOCKET
  fc7300_socket_caninitialize(3);
#  endif /* CONFIG_FC7300_FLEXCAN3 */
#  ifdef CONFIG_FC7300_FLEXCAN4_SOCKET
  fc7300_socket_caninitialize(4);
#  endif /* CONFIG_FC7300_FLEXCAN4 */
#  ifdef CONFIG_FC7300_FLEXCAN5_SOCKET
  fc7300_socket_caninitialize(5);
#  endif /* CONFIG_FC7300_FLEXCAN5 */
#  ifdef CONFIG_FC7300_FLEXCAN6_SOCKET
  fc7300_socket_caninitialize(6);
#  endif /* CONFIG_FC7300_FLEXCAN6 */
#  ifdef CONFIG_FC7300_FLEXCAN7_SOCKET
  fc7300_socket_caninitialize(7);
#  endif /* CONFIG_FC7300_FLEXCAN7 */
#  ifdef CONFIG_FC7300_FLEXCAN8_SOCKET
  fc7300_socket_caninitialize(8);
#  endif /* CONFIG_FC7300_FLEXCAN8 */
#  ifdef CONFIG_FC7300_FLEXCAN9_SOCKET
  fc7300_socket_caninitialize(9);
#  endif /* CONFIG_FC7300_FLEXCAN9 */
  return OK;
}

#endif  /* CONFIG_FC7300_FLEXCAN_SOCKET */
#endif  /* CONFIG_NET_CAN && CONFIG_NETDEV_LATEINIT*/

#ifdef CONFIG_CAN
/****************************************************************************
 * Name: fc7300_flexcan_chardriver_setup
 *
 * Description:
 *  Initialize CAN char driver and register the CAN device
 *
 ****************************************************************************/

#if defined(CONFIG_FC7300_FLEXCAN_CHARDRIVER)

struct can_dev
{
  uint32_t intf;
  const struct can_filter * rxmb_filter;
  const struct can_erf_filter * erf_filter;
};

static struct can_dev g_can_devs[] =
{
#if defined(CONFIG_FC7300_FLEXCAN0_CHARDRIVER)
    {
      .intf = 0,
      .rxmb_filter = flexcan0_rxmb_filter,
      .erf_filter  = flexcan0_erf_filter,
    },
#endif

#if defined(CONFIG_FC7300_FLEXCAN1_CHARDRIVER)
    {
      .intf = 1,
      .rxmb_filter = flexcan1_rxmb_filter,
      .erf_filter  = flexcan1_erf_filter,
    },
#endif

#if defined(CONFIG_FC7300_FLEXCAN2_CHARDRIVER)
    {
      .intf = 2,
      .rxmb_filter = flexcan2_rxmb_filter,
      .erf_filter  = flexcan2_erf_filter,
    },
#endif

#if defined(CONFIG_FC7300_FLEXCAN3_CHARDRIVER)
    {
      .intf = 3,
      .rxmb_filter = flexcan3_rxmb_filter,
      .erf_filter  = flexcan3_erf_filter,
    },
#endif

#if defined(CONFIG_FC7300_FLEXCAN4_CHARDRIVER)
    {
      .intf = 4,
      .rxmb_filter = flexcan4_rxmb_filter,
      .erf_filter  = flexcan4_erf_filter,
    },
#endif

#if defined(CONFIG_FC7300_FLEXCAN5_CHARDRIVER)
    {
      .intf = 5,
      .rxmb_filter = flexcan5_rxmb_filter,
      .erf_filter  = flexcan5_erf_filter,
    },
#endif

#if defined(CONFIG_FC7300_FLEXCAN6_CHARDRIVER)
    {
      .intf = 6,
      .rxmb_filter = flexcan6_rxmb_filter,
      .erf_filter  = flexcan6_erf_filter,
    },
#endif

#if defined(CONFIG_FC7300_FLEXCAN7_CHARDRIVER)
    {
      .intf = 7,
      .rxmb_filter = flexcan7_rxmb_filter,
      .erf_filter  = flexcan7_erf_filter,
    },
#endif

#if defined(CONFIG_FC7300_FLEXCAN8_CHARDRIVER)
    {
      .intf = 8,
      .rxmb_filter = flexcan8_rxmb_filter,
      .erf_filter  = flexcan8_erf_filter,
    },
#endif

#if defined(CONFIG_FC7300_FLEXCAN9_CHARDRIVER)
    {
      .intf = 9,
      .rxmb_filter = flexcan9_rxmb_filter,
      .erf_filter  = flexcan9_erf_filter,
    },
#endif
};

#define CAN_CHARDEV_NUMS (sizeof(g_can_devs) / sizeof(g_can_devs[0]))

int fc7300_flexcan_chardriver_setup(void)
{
  int i;
  int ret;
  char name[16] =
  {
    0
  };

  struct can_dev_s *can;

#ifdef CONFIG_FC7300_FLEXCAN1_CHARDRIVER
  fc7300_pinconfig(PIN_CAN1_STB);
  fc7300_gpiowrite(PIN_CAN1_STB, 0);
#endif

  for (i = 0; i < CAN_CHARDEV_NUMS; i++)
    {
      can = fc7300_chardriver_caninitialize(g_can_devs[i].intf, \
                                            g_can_devs[i].rxmb_filter, \
                                            g_can_devs[i].erf_filter);
      if (can == NULL)
        {
          canerr("ERROR: Failed to get CAN interface %d!\n", i);
          continue;
        }

      snprintf(name , sizeof(name), "/dev/can%ld", g_can_devs[i].intf);

      ret = can_register(name , can);
      if (ret < 0)
        {
          canerr("ERROR: can_register failed: %d\n", ret);
        }
    }

  return OK;
}

#endif /* CONFIG_FC7300_FLEXCAN_CHARDRIVER */
#endif /* CONFIG_CAN */
