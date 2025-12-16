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

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <debug.h>
#include <errno.h>

#include <nuttx/irq.h>
#include <nuttx/arch.h>
#include <nuttx/queue.h>
#include <nuttx/spinlock.h>
#include <nuttx/mutex.h>
#include <nuttx/semaphore.h>

#include "arm_internal.h"
#include "sched/sched.h"

#include "chip.h"
#include "fc7300_stcu.h"
#include "hardware/fc7300_stcu.h"

#if defined(CONFIG_FC7300_STCU)

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

static uint32_t stcu_getself_status(void);
static uint32_t stcu_getmbist_done(void);
static uint8_t stcu_getlbist_failresult(void);
static uint32_t stcu_getmbist_fail(void);
static uint32_t stcu_gettst_result(void);
static uint32_t stcu_getmbist_failresult(void);

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: stcu_getself_status
 *
 * Description:
 *   Get the SELF_TEST_STATUS register value
 *
 * Input Parameters:
 *   None
 *
 * Returned Value:
 *   Return the value of SELF_TEST_STATUS register
 *
 ****************************************************************************/

static uint32_t stcu_getself_status()
{
  return getreg32(FC7300_STCU_SELF_TEST_STATUS);
}

/****************************************************************************
 * Name: stcu_getmbist_done
 *
 * Description:
 *   Get the STCU_MBIST_DONE_STATUS register value
 *
 * Input Parameters:
 *   None
 *
 * Returned Value:
 *   Return the value of STCU_MBIST_DONE_STATUS register
 *
 ****************************************************************************/

static uint32_t stcu_getmbist_done()
{
  return getreg32(FC7300_STCU_MBIST_DONE_STATUS);
}

/****************************************************************************
 * Name: stcu_getmbist_fail
 *
 * Description:
 *   Get the STCU_MBIST_FAIL_STATUS register value
 *
 * Input Parameters:
 *   None
 *
 * Returned Value:
 *   Return the value of STCU_MBIST_FAIL_STATUS register
 *
 ****************************************************************************/

static uint32_t stcu_getmbist_fail()
{
  return getreg32(FC7300_STCU_MBIST_FAIL_STATUS);
}

/****************************************************************************
 * Name: stcu_getmbist_failresult
 *
 * Description:
 *   Get the SelfTest result of Mbist
 *
 * Input Parameters:
 *   None
 *
 * Returned Value:
 *   Mbist selftest result
 *
 ****************************************************************************/

static uint32_t stcu_getmbist_failresult()
{
  uint32_t done;
  uint32_t fail;
  uint32_t result;

  done = stcu_getmbist_done();
  fail = stcu_getmbist_fail();
  result = done & fail;
  return result;
}

/****************************************************************************
 * Name: stcu_getlbist_failresult
 *
 * Description:
 *   Get the SelfTest result of Lbist
 *
 * Input Parameters:
 *   None
 *
 * Returned Value:
 *   Lbist selftest result
 *
 ****************************************************************************/

static uint8_t stcu_getlbist_failresult()
{
  uint32_t status;
  uint8_t bl_result;

  status = getreg32(FC7300_STCU_LBIST_STATUS);
  status = (status >> FC7300_STCU_LBIST_FAIL_DONE_SHIFT) &
           (status & FC7300_STCU_LBIST_STATUS_DONE_MASK);
  bl_result = (uint8_t)(0u == status?(uint8_t)false:(uint8_t)true);
  return bl_result;
}

/****************************************************************************
 * Name: stcu_gettst_result
 *
 * Description:
 *   Get the SelfTest result of Lbist
 *
 * Input Parameters:
 *   None
 *
 * Returned Value:
 *   Lbist selftest result
 *
 ****************************************************************************/

static uint32_t stcu_gettst_result()
{
  uint8_t lbistresult;
  uint32_t mbistresult;
  uint32_t result = 0u;
  uint32_t self_test_stauts;
  uint32_t lbist_en;
  uint32_t mbist_en;

  lbist_en = ((getreg32(FC7300_STCU_SELF_TEST_CTRL) &
               FC7300_STCU_SELF_TEST_CTRL_MT_LBIST_EN) >>
               FC7300_STCU_SELF_TEST_CTRL_LBIST_EN_SHIFT);

  mbist_en = ((getreg32(FC7300_STCU_SELF_TEST_CTRL) &
               FC7300_STCU_SELF_TEST_CTRL_MT_MBIST_EN) >>
               FC7300_STCU_SELF_TEST_CTRL_MBIST_EN_SHIFT);

  self_test_stauts = stcu_getself_status();

  /* if the LBIST is enabled */

  if (lbist_en)
    {
      lbistresult = stcu_getlbist_failresult();
      if (((uint8_t)false == lbistresult) &&
         ((uint32_t)STCU_SELFTEST_DONE == self_test_stauts))
        {
          result |= STCU_LBIST_SUCCESS_FLAG;
        }

      else if (STCU_SELFTEST_LBIST_ERROR & self_test_stauts)
        {
          result |= STCU_LBIST_FAIL_FLAG;
        }
    }

  if (mbist_en)
    {
      mbistresult = stcu_getmbist_failresult();

      if (((uint32_t)STCU_MBIST_FAIL_NONE == mbistresult) &&
         ((uint32_t)STCU_SELFTEST_DONE == self_test_stauts))
        {
          result |= STCU_MBIST_SUCCESS_FLAG;
        }

      else if (STCU_SELFTEST_MBIST_ERROR & self_test_stauts)
        {
          result |= STCU_MBIST_FAIL_FLAG;
        }
    }

  return result;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: fc7300_get_stcu_selftest_result
 *
 * Description:
 *   Get the stcu selftest result
 *
 * Input Parameters:
 *   None
 *
 * Returned Value:
 *   The self-test result
 *
 ****************************************************************************/

uint32_t fc7300_get_stcu_selftest_result()
{
  uint32_t result;
  result = stcu_gettst_result();
  return result;
}

#endif