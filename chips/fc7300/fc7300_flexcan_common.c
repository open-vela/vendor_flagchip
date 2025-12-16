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
#include <arch/barriers.h>

#include "fc7300_flexcan.h"

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Function: fc7300_flexcan_waitmcr_change
 *
 * Description:
 *   Wait the CAN controller mcr register mode change done.
 *
 * Input Parameters:
 *   base - CAN controller register base address.
 *   mask - The excepted mode to change.
 *   target_state - Target mode state.
 *
 * Returned Value:
 *   TRUE on success; FALSE on failure.
 *
 ****************************************************************************/

uint32_t fc7300_flexcan_waitmcr_change(uint32_t base, uint32_t mask,
                                       uint32_t target_state)
{
  bool state;
  uint32_t wait_ack;
  const uint32_t timeout = 1000;

  for (wait_ack = 0; wait_ack < timeout; wait_ack++)
    {
      state = (getreg32(base + FC7300_CAN_MCR_OFFSET) & mask) != 0;
      if (state == target_state)
        {
          return true;
        }

      up_udelay(10);
    }

  return false;
}

/****************************************************************************
 * Function: fc7300_flexcan_setfreeze
 *
 * Description:
 *   Enter or leave freeze mode.
 *
 * Input Parameters:
 *   base - CAN controller register base address.
 *   freeze - Enter or leave flag.
 *
 * Returned Value:
 *   Zeor on success, -1 on failure.
 *
 ****************************************************************************/

int fc7300_flexcan_setfreeze(uint32_t base, uint32_t freeze)
{
  uint32_t regval;

  if (freeze)
    {
      /* Enter freeze mode */

      regval  = getreg32(base + FC7300_CAN_MCR_OFFSET);
      regval |= (CAN_MCR_HALT | CAN_MCR_FRZ);
      putreg32(regval, base + FC7300_CAN_MCR_OFFSET);
    }
  else
    {
      /* Exit freeze mode */

      regval  = getreg32(base + FC7300_CAN_MCR_OFFSET);
      regval &= ~(CAN_MCR_HALT | CAN_MCR_FRZ);
      putreg32(regval, base + FC7300_CAN_MCR_OFFSET);
    }

  UP_ISB();

  if (!fc7300_flexcan_waitmcr_change(base, CAN_MCR_FRZACK, freeze))
    {
      return -1;
    }

  return 0;
}

/****************************************************************************
 * Function: fc7300_flexcan_setenable
 *
 * Description:
 *   Enable or disable CAN controller.
 *
 * Input Parameters:
 *   base - CAN controller register base address.
 *   enable - Enable or disable flag.
 *
 * Returned Value:
 *   NONE
 *
 ****************************************************************************/

int fc7300_flexcan_setenable(uint32_t base, uint32_t enable)
{
  uint32_t regval;

  if (enable)
    {
      regval  = getreg32(base + FC7300_CAN_MCR_OFFSET);
      regval &= ~(CAN_MCR_MDIS);
      putreg32(regval, base + FC7300_CAN_MCR_OFFSET);
    }
  else
    {
      regval  = getreg32(base + FC7300_CAN_MCR_OFFSET);
      regval |= CAN_MCR_MDIS;
      putreg32(regval, base + FC7300_CAN_MCR_OFFSET);
    }

  if (!fc7300_flexcan_waitmcr_change(base, CAN_MCR_LPMACK, enable ? 0 : 1))
    {
      return -1;
    }

  return 0;
}
