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
#include "fc7300_pmc.h"
#include "fc7300_pmc_user.h"

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

/* PMC Device Private Data */

struct fc7300_pmc_priv_s
{
  struct safety_lowerhalf_s lower;    /* Lower half driver structure */
  safety_handler_t          handler;  /* Callback function */
  FAR void                  *arg;     /* Callback argument */
  uint8_t                   err_type; /* Error type */
  bool                      flag;     /* Fault flag */
};

/****************************************************************************
 * Private Functions
 ****************************************************************************/

static int fc7300_pmc_setup(FAR struct safety_lowerhalf_s *lower);
static int fc7300_pmc_set_callback(FAR struct safety_lowerhalf_s *lower,
                                   safety_handler_t handler,  FAR void *arg);
static int fc7300_pmc_selftest(FAR struct safety_lowerhalf_s *lower);

/****************************************************************************
 * Private Data
 ****************************************************************************/

static const struct safety_ops_s g_pmc_ops =
{
  .setup        = fc7300_pmc_setup,
  .selftest     = fc7300_pmc_selftest,
  .set_callback = fc7300_pmc_set_callback,
};

static struct fc7300_pmc_priv_s g_pmc_priv;

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: fc7300_pmc_callback
 *
 * Description:
 *   Sets the callback function that will be called when a PMC event occurs.
 *
 ****************************************************************************/

static int fc7300_pmc_set_callback(FAR struct safety_lowerhalf_s *lower,
                               safety_handler_t handler, FAR void *arg)
{
  FAR struct fc7300_pmc_priv_s *priv = (FAR struct fc7300_pmc_priv_s *)lower;
  priv->handler = handler;
  return OK;
}

/****************************************************************************
 * Name: fc7300_pmc_selftest
 *
 * Description:
 *   Performs a self-test on the PMC module to verify its operational status.
 *   This function can be used during system startup or maintenance checks.
 *
 ****************************************************************************/

static int fc7300_pmc_selftest(FAR struct safety_lowerhalf_s *lower)
{
  FAR struct fc7300_pmc_priv_s *priv = (FAR struct fc7300_pmc_priv_s *)lower;
  return priv->flag;
}

/****************************************************************************
 * Name: fc7300_pmc_setup
 *
 * Description:
 *   Initializes the PMC module by setting up necessary configurations and
 *   attaching interrupt handlers.
 *
 ****************************************************************************/

static int fc7300_pmc_setup(FAR struct safety_lowerhalf_s *lower)
{
  fc7300_pmc_init();
  fc7300_pmc_irq_attach();
  return OK;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: fc7300_pmc_user_callback
 *
 * Description:
 *   User-level callback function after PMC interrupt.
 *
 ****************************************************************************/

int fc7300_pmc_user_callback(uint8_t error_type)
{
  FAR struct fc7300_pmc_priv_s *priv = &g_pmc_priv;
  struct pmc_fault_data_s fault_data;

  fault_data.err_type = error_type;
  priv->flag = 1;
  if (priv->handler != NULL)
    {
      return priv->handler(priv->arg, &fault_data,
                    0, sizeof(struct pmc_fault_data_s));
    }

  return 0;
}

/****************************************************************************
 * Name: fc7300_pmc_safety_initialize
 *
 * Description:
 *   Initializes the PMC (Power Management Controller) module. This function
 *   sets up the PMC private data structure, registers the PMC with the
 *   safety core, and prepares it for operation.
 *
 ****************************************************************************/

int fc7300_pmc_safety_initialize(void)
{
  g_pmc_priv.lower.ops  = &g_pmc_ops;

  return safety_register(&g_pmc_priv.lower,
                         SAFETY_MODULE_POWER,
                         sizeof(struct pmc_fault_data_s));
}
