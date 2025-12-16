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
#include <nuttx/arch.h>
#include <nuttx/board.h>
#include <sys/boardctl.h>
#include <stdint.h>
#include <debug.h>
#include "nvic.h"
#include "arm_internal.h"
#include "hardware/fc7300_memorymap.h"

/****************************************************************************
 * Private Types
 ****************************************************************************/

typedef struct {
  uint8_t  RESERVED_0[12];
  volatile uint32_t PMCTRL                        ; /* Power Mode Control Register, offset: 0xc */
  volatile uint32_t STANDBY_CFG                   ; /* Standby Mode Configuration Register, offset: 0x10 */
} SMC_Type, *SMC_MemMapPtr;

typedef enum
{
    SMC_CFG_STANDBY_0 = 0U,
    SMC_CFG_STANDBY_1 = 1U,
    SMC_CFG_STANDBY_2 = 2U,
    SMC_CFG_STANDBY_3 = 3U
} SMC_StandbyModeType;

typedef enum
{
    SMC_STOP_MODE = 0U,
    SMC_STANDBY_MODE = 4U
} SMC_LpwModeCtrlType;

/****************************************************************************
 * LOCAL MACROS
 ****************************************************************************/

#define LOCAL_INLINE     __attribute__((always_inline)) static inline
#define SMC   ((SMC_Type *)FC7300_SMC_BASE)

#if defined (__GNUC__)
#define STANDBY() __asm volatile ("wfi")
#else
#define STANDBY() __asm("wfi")
#endif

/****************************************************************************
 * Private Functions
 ****************************************************************************/

LOCAL_INLINE void SMC_HWA_SetStandbyMode(SMC_StandbyModeType eMode)
{
    putreg32((uint32_t)eMode, &SMC->STANDBY_CFG);
}

LOCAL_INLINE void SMC_HWA_SetStopModeCtrl(SMC_LpwModeCtrlType eMode)
{
    putreg32((uint32_t)eMode, &SMC->PMCTRL);
}

LOCAL_INLINE void CM7_HWA_EnableDeepSleep(void)
{
    putreg32(NVIC_SYSCON_SLEEPDEEP, NVIC_SYSCON);
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: board_power_off
 *
 * Description:
 *   Power off the board.
 *
 *   If this function returns, then it was not possible to power-off the
 *   board due to some other constraints.
 *
 * Input Parameters:
 *   status - Status information provided with the power off event.
 *
 * Returned Value:
 *   If this function returns, then it was not possible to power-off the
 *   board due to some constraints.  The return value int this case is a
 *   board-specific reason for the failure to shutdown.
 *
 ****************************************************************************/

#ifdef CONFIG_BOARDCTL_POWEROFF
int board_power_off(int status)
{
  up_irq_disable();

  SMC_HWA_SetStandbyMode(SMC_CFG_STANDBY_3);

  /* switch smc mode to standby mode */
  SMC_HWA_SetStopModeCtrl(SMC_STANDBY_MODE);

  /* Set the SLEEPDEEP bit to enable deep sleep mode */
  CM7_HWA_EnableDeepSleep();

  /* Cpu is going into deep sleep state */
  STANDBY();

  return 0;
}
#endif

