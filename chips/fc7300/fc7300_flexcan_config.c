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
#include <nuttx/can.h>
#include <arch/board/board.h>

#include "fc7300_flexcan.h"
#include "hardware/fc7300_pin.h"
#include "hardware/fc7300_pinmux.h"

#ifdef CONFIG_FC7300_FLEXCAN0
const struct flexcan_config_s g_fc7300_flexcan0_config =
{
  .tx_pin      = PIN_CAN0_TX,
  .rx_pin      = PIN_CAN0_RX,
  .no_buffers  = 32,
  .irq         = FC7300_IRQ_FLEXCAN0,
  .irq_pri     = NVIC_FLEXCAN0_PRIORITY_DEFAULT,
};
#endif

#ifdef CONFIG_FC7300_FLEXCAN1
const struct flexcan_config_s g_fc7300_flexcan1_config =
{
  .tx_pin      = PIN_CAN1_TX,
  .rx_pin      = PIN_CAN1_RX,
  .no_buffers  = 32,
  .irq         = FC7300_IRQ_FLEXCAN1,
  .irq_pri     = NVIC_FLEXCAN1_PRIORITY_DEFAULT,
};
#endif

#ifdef CONFIG_FC7300_FLEXCAN2
const struct flexcan_config_s g_fc7300_flexcan2_config =
{
  .tx_pin      = PIN_CAN2_TX,
  .rx_pin      = PIN_CAN2_RX,
  .no_buffers  = 32,
  .irq         = FC7300_IRQ_FLEXCAN2,
  .irq_pri     = NVIC_FLEXCAN2_PRIORITY_DEFAULT,
};
#endif

#ifdef CONFIG_FC7300_FLEXCAN3
const struct flexcan_config_s g_fc7300_flexcan3_config =
{
  .tx_pin      = PIN_CAN3_TX,
  .rx_pin      = PIN_CAN3_RX,
  .no_buffers  = 32,
  .irq         = FC7300_IRQ_FLEXCAN3,
  .irq_pri     = NVIC_FLEXCAN3_PRIORITY_DEFAULT,
};
#endif

#ifdef CONFIG_FC7300_FLEXCAN4
const struct flexcan_config_s g_fc7300_flexcan4_config =
{
  .tx_pin      = PIN_CAN4_TX,
  .rx_pin      = PIN_CAN4_RX,
  .no_buffers  = 32,
  .irq         = FC7300_IRQ_FLEXCAN4,
  .irq_pri     = NVIC_FLEXCAN4_PRIORITY_DEFAULT,
};
#endif

#ifdef CONFIG_FC7300_FLEXCAN5
const struct flexcan_config_s g_fc7300_flexcan5_config =
{
  .tx_pin      = PIN_CAN5_TX,
  .rx_pin      = PIN_CAN5_RX,
  .no_buffers  = 32,
  .irq         = FC7300_IRQ_FLEXCAN5,
  .irq_pri     = NVIC_FLEXCAN5_PRIORITY_DEFAULT,
};
#endif

#ifdef CONFIG_FC7300_FLEXCAN6
const struct flexcan_config_s g_fc7300_flexcan6_config =
{
  .tx_pin      = PIN_CAN6_TX,
  .rx_pin      = PIN_CAN6_RX,
  .no_buffers  = 32,
  .irq         = FC7300_IRQ_FLEXCAN6,
  .irq_pri     = NVIC_FLEXCAN6_PRIORITY_DEFAULT,
};
#endif

#ifdef CONFIG_FC7300_FLEXCAN7
const struct flexcan_config_s g_fc7300_flexcan7_config =
{
  .tx_pin      = PIN_CAN7_TX,
  .rx_pin      = PIN_CAN7_RX,
  .no_buffers  = 32,
  .irq         = FC7300_IRQ_FLEXCAN7,
  .irq_pri     = NVIC_FLEXCAN7_PRIORITY_DEFAULT,
};
#endif

#ifdef CONFIG_FC7300_FLEXCAN8
const struct flexcan_config_s g_fc7300_flexcan8_config =
{
  .tx_pin      = PIN_CAN8_TX,
  .rx_pin      = PIN_CAN8_RX,
  .no_buffers  = 32,
  .irq         = FC7300_IRQ_FLEXCAN8,
  .irq_pri     = NVIC_FLEXCAN8_PRIORITY_DEFAULT,
};
#endif

#ifdef CONFIG_FC7300_FLEXCAN9
const struct flexcan_config_s g_fc7300_flexcan9_config =
{
  .tx_pin      = PIN_CAN9_TX,
  .rx_pin      = PIN_CAN9_RX,
  .no_buffers  = 32,
  .irq         = FC7300_IRQ_FLEXCAN9,
  .irq_pri     = NVIC_FLEXCAN9_PRIORITY_DEFAULT,
};
#endif

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Public Functions
 ****************************************************************************/
