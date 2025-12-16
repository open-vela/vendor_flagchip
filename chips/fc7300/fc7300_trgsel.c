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

#include <debug.h>
#include <arch/types.h>
#include <nuttx/config.h>
#include <nuttx/arch.h>
#include <nuttx/irq.h>
#include "fc7300_trgsel.h"
#include "fc7300_clockconfig.h"
#include "fc7300_periphclocks.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: fc7300_trgsel0_settriggersource
 *
 * Description:
 *   Initialize trgsel0.
 *
 * Input Parameters:
 *   target - A number identifying the trgsel output.
 *   source - A number identifying the trgsel input.
 *
 * Returned Value:
 *   N/A
 *
 ****************************************************************************/

void fc7300_trgsel0_settriggersource(uint32_t target, uint32_t source)
{
  uint32_t regidx = target / 4;
  uint32_t selidx = target % 4;
  uint32_t temp = getreg32(FC7300_TRGSEL0_OUT_SEL(regidx));

  switch (selidx)
    {
      case 0U:
        {
          temp = (temp & ~FC7300_TRGSEL_OUT_SEL_SEL_0_MASK) |
                  FC7300_TRGSEL_OUT_SEL_SEL_0(source);
        }
        break;

      case 1U:
        {
          temp = (temp & ~FC7300_TRGSEL_OUT_SEL_SEL_1_MASK) |
                  FC7300_TRGSEL_OUT_SEL_SEL_1(source);
        }
        break;

      case 2U:
        {
          temp = (temp & ~FC7300_TRGSEL_OUT_SEL_SEL_2_MASK) |
                  FC7300_TRGSEL_OUT_SEL_SEL_2(source);
        }
        break;

      case 3U:
        {
          temp = (temp & ~FC7300_TRGSEL_OUT_SEL_SEL_3_MASK) |
                  FC7300_TRGSEL_OUT_SEL_SEL_3(source);
        }
        break;

      //coverity[DEADCODE:SUPPRESS]
      default:
          break;
    }

  putreg32(temp, FC7300_TRGSEL0_OUT_SEL(regidx));
}

/****************************************************************************
 * Name: fc7300_trgsel3_settriggersource
 *
 * Description:
 *   Initialize trgsel3.
 *
 * Input Parameters:
 *   target - A number identifying the trgsel output.
 *   source - A number identifying the trgsel input.
 *
 * Returned Value:
 *   N/A
 *
 ****************************************************************************/

void fc7300_trgsel3_settriggersource(uint32_t target, uint32_t source)
{
  uint32_t regidx = target / 4;
  uint32_t selidx = target % 4;
  uint32_t temp = getreg32(FC7300_TRGSEL3_OUT_SEL(regidx));

  switch (selidx)
    {
      case 0U:
        {
          temp = (temp & ~FC7300_TRGSEL_OUT_SEL_SEL_0_MASK) |
                  FC7300_TRGSEL_OUT_SEL_SEL_0(source);
        }
        break;

      case 1U:
        {
          temp = (temp & ~FC7300_TRGSEL_OUT_SEL_SEL_1_MASK) |
                  FC7300_TRGSEL_OUT_SEL_SEL_1(source);
        }
        break;

      case 2U:
        {
          temp = (temp & ~FC7300_TRGSEL_OUT_SEL_SEL_2_MASK) |
                  FC7300_TRGSEL_OUT_SEL_SEL_2(source);
        }
        break;

      case 3U:
        {
          temp = (temp & ~FC7300_TRGSEL_OUT_SEL_SEL_3_MASK) |
                  FC7300_TRGSEL_OUT_SEL_SEL_3(source);
        }
        break;
      //coverity[DEADCODE:SUPPRESS]
      default:
          break;
    }

  putreg32(temp, FC7300_TRGSEL3_OUT_SEL(regidx));
}
