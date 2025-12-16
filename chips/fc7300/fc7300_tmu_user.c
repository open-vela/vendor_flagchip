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

#include <nuttx/safety/safety.h>

#include "fc7300_tmu.h"
#include "fc7300_tmu_user.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Private Types
 ****************************************************************************/

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

/****************************************************************************
 * Private Data
 ****************************************************************************/

/* TMU Device Private Data */

typedef struct
{
  FAR struct safety_lowerhalf_s lower;            /* Lower half driver structure */
  safety_handler_t              handler;          /* Callback function */
  struct tmu_fault_data_s       fault_data;       /* Fault data */
}fc7300_tmu_priv_t;

static fc7300_tmu_priv_t g_tmu_priv;

/****************************************************************************
 *Private Function Prototypes
 ****************************************************************************/

static int fc7300_tmu_set_callback(struct safety_lowerhalf_s *lower,
                                   safety_handler_t handler,  FAR void *arg);
static int fc7300_tmu_selftest(struct safety_lowerhalf_s *lower);
static int fc7300_tmu_setup(FAR struct safety_lowerhalf_s *lower);
/****************************************************************************
 * Private Data
 ****************************************************************************/

static const struct safety_ops_s g_tmu_ops =
{
  .setup        = fc7300_tmu_setup,
  .selftest     = fc7300_tmu_selftest,
  .set_callback = fc7300_tmu_set_callback,
};

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: fc7300_tmu_setup
 *
 * Description:
 *   Setup the TMU driver.
 *
 ****************************************************************************/

static int fc7300_tmu_setup(FAR struct safety_lowerhalf_s *lower)
{
  fc7300_tmu_init();
  fc7300_tmu_irq_attach();

  return OK;
}

/****************************************************************************
 * Name: fc7300_tmu_callback
 *
 * Description:
 *   Sets the callback function that will be called when a TMU event occurs.
 *
 * Input Parameters:
 *   lower - Pointer to the lower half driver structure for the TMU.
 *   handler - Pointer to the callback function.
 *
 * Returned Value:
 *   OK on success, or ERROR if the callback cannot be set.
 ****************************************************************************/

static int fc7300_tmu_set_callback(struct safety_lowerhalf_s *lower,
                                   safety_handler_t handler,  FAR void *arg)
{
  fc7300_tmu_priv_t *priv = (fc7300_tmu_priv_t *)lower;
  priv->handler           = handler;
  return OK;
}

/****************************************************************************
 * Name: fc7300_tmu_selftest
 *
 * Description:
 *   Performs a self-test on the TMU module to verify its operational status.
 *   This function can be used during system startup or maintenance checks.
 *
 * Input Parameters:
 *   lower - Pointer to the lower half driver structure for the TMU.
 *
 * Returned Value:
 *   Current error type or status of the TMU.
 ****************************************************************************/

static int fc7300_tmu_selftest(struct safety_lowerhalf_s *lower)
{
  fc7300_tmu_priv_t *priv = (fc7300_tmu_priv_t *)lower;
  return priv->fault_data.tmu_fault_type;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: fc7300_tmu_user_callback
 *
 * Description:
 *   User-level callback function after TMU interrupt.
 *
 * Input Parameters:
 *   error_type - Error Type
 *                  (0x0: No error
 *                   0x1: 125 flag has beed set
 *                   0x2: 125 and 150 flag has beed set)
 *
 * Return Value:
 *   0: Clear interrupt flag by driver.
 *   1: Clear interrupt flag by user.
 *
 ****************************************************************************/

int fc7300_tmu_user_callback(uint8_t error_type)
{
  fc7300_tmu_priv_t *priv = &g_tmu_priv;

  priv->fault_data.tmu_fault_type = error_type;

  if (priv->handler)
    {
      return priv->handler(&g_tmu_priv.lower, &priv->fault_data,
                           0, sizeof(struct tmu_fault_data_s));
    }

  return 0;
}

/****************************************************************************
 * Name: fc7300_tmu_initialize
 *
 * Description:
 *   Initializes the TMU (Temperature Monitoring Unit) module. This function
 *   sets up the TMU private data structure, registers the TMU with the
 *   safety core, and prepares it for operation.
 *
 * Returned Value:
 *   OK on success, or an error code if initialization fails.
 ****************************************************************************/

int fc7300_tmu_safety_initialize(void)
{
  g_tmu_priv.lower.ops = &g_tmu_ops;

  return safety_register(&g_tmu_priv.lower, SAFETY_MODULE_TEMPERATURE,
                         sizeof(struct tmu_fault_data_s));
}
