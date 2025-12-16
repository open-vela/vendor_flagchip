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

#include <assert.h>
#include <errno.h>

#include "arm_internal.h"
#include "hardware/fc7300_gpio.h"
#include "hardware/fc7300_pin.h"

#include <arch/board/board.h>

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: fc7300_gpiowrite
 *
 * Description:
 *   Write one or zero to the selected GPIO pin
 *
 ****************************************************************************/

void fc7300_gpiowrite(uint32_t pinset, bool value)
{
  uintptr_t    base;
  unsigned int port;
  unsigned int pin;

  DEBUGASSERT((pinset & _PIN_MODE_MASK) == _PIN_MODE_GPIO);
  DEBUGASSERT((pinset & _PIN_IO_MASK) == _PIN_OUTPUT);

  /* Get the port number and pin number */

  port = (pinset & _PIN_PORT_MASK) >> _PIN_PORT_SHIFT;
  pin  = (pinset & _PIN_MASK)      >> _PIN_SHIFT;

  DEBUGASSERT(port < FC7300_NPORTS);
  if (port < FC7300_NPORTS)
    {
      /* Get the base address of GPIO block for this port */

      base = FC7300_GPIO_BASE(port);

      /* Set or clear the output */

      if (value)
        {
          putreg32((1 << pin), base + FC7300_GPIO_PSOR_OFFSET);
        }
      else
        {
          putreg32((1 << pin), base + FC7300_GPIO_PCOR_OFFSET);
        }
    }
}

/****************************************************************************
 * Name: fc7300_gpioread
 *
 * Description:
 *   Read one or zero from the selected GPIO pin
 *
 ****************************************************************************/

bool fc7300_gpioread(uint32_t pinset)
{
  uintptr_t    base;
  uint32_t     regval;
  unsigned int port;
  unsigned int pin;
  bool         ret = false;

  DEBUGASSERT((pinset & _PIN_MODE_MASK) != _PIN_MODE_ANALOG);

  /* Get the port number and pin number */

  port = (pinset & _PIN_PORT_MASK) >> _PIN_PORT_SHIFT;
  pin  = (pinset & _PIN_MASK)      >> _PIN_SHIFT;

  DEBUGASSERT(port < FC7300_NPORTS);
  if (port < FC7300_NPORTS)
    {
      /* Get the base address of GPIO block for this port */

      base = FC7300_GPIO_BASE(port);

      /* return the state of the pin */

      regval = getreg32(base + FC7300_GPIO_PDIR_OFFSET);
      ret    = ((regval & (1 << pin)) != 0);
    }

  return ret;
}
