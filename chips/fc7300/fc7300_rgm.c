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
#include <sys/boardctl.h>
#include <assert.h>
#include "fc7300_rgm.h"
#include "fc7300_multicore.h"
#include "hardware/fc7300_rgm.h"
#include "arm_internal.h"

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Public Functions
 ****************************************************************************/

#ifdef CONFIG_BOARDCTL_RESET_CAUSE
int board_reset_cause(struct boardioc_reset_cause_s *cause)
{
  uint8_t cpu_id;
  uint32_t reset;

  cpu_id = fc7300_cpu_index();
  DEBUGASSERT(cpu_id < 3U);

  cause->cause = BOARDIOC_RESETCAUSE_NONE;

  if (cpu_id == 0U)
    reset = getreg32(FC7300_RGM_C0_SRS);
  else if (cpu_id == 1U)
    reset = getreg32(FC7300_RGM_C1_SRS);
  else if (cpu_id == 2U)
    reset = getreg32(FC7300_RGM_C2_SRS);

  if (reset & RGM_CX_SRS_POR)
    {
      cause->cause = BOARDIOC_RESETCAUSE_SYS_CHIPPOR;
    }
  else if(reset & RGM_CX_SRS_CX_WDOG)
    {
      cause->cause = BOARDIOC_RESETCAUSE_SYS_RWDT;
    }
  else if(reset & RGM_CX_SRS_PIN)
    {
      cause->cause = BOARDIOC_RESETCAUSE_PIN;
    }
  else if(reset & RGM_CX_SRS_LVR)
    {
      cause->cause = BOARDIOC_RESETCAUSE_LOWPOWER;
    }
  else if(reset & RGM_CX_SRS_CX_SWRST)
    {
      cause->cause = BOARDIOC_RESETCAUSE_CORE_SOFT;
    }
  else if(reset & RGM_CX_SRS_CX_SYSRST)
    {
      cause->cause = BOARDIOC_RESETCAUSE_CPU_SOFT;
    }

  return OK;
}
#endif
