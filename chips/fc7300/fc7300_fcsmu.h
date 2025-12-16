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

#ifndef __VENDOR_FLAGCHIP_CHIPS_FC7300_FC7300_FCSMU_H
#define __VENDOR_FLAGCHIP_CHIPS_FC7300_FC7300_FCSMU_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>
#include <stdint.h>

/****************************************************************************
 * Public Data
 ****************************************************************************/

#ifndef __ASSEMBLY__

#undef EXTERN
#if defined(__cplusplus)
#define EXTERN extern "C"
extern "C"
{
#else
#define EXTERN extern
#endif

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

/****************************************************************************
 * Name: fc7300_fcsmu_init
 *
 * Description:
 *   Hardware level initializes the FCSMU Driver settings based on the
 * configuration.
 *
 ****************************************************************************/

void fc7300_fcsmu_init(void);

/****************************************************************************
 * Name: fc7300_fcsmu_deinit
 *
 * Description:
 *   Hardware level ide-nitializes the FCSMU Driver.
 *
 ****************************************************************************/

void fc7300_fcsmu_deinit(void);

/****************************************************************************
 * Name: fc7300_fcsmu_get_error_status
 *
 * Description:
 *   Get fcsmu error after system reset.
 *
 ****************************************************************************/

uint32_t fc7300_fcsmu_get_error_status(void);

/****************************************************************************
 * Name: fc7300_fcsmu_software_trigger_error
 *
 * Description:
 *   Software trigger error for fcsmu channel 18.
 *
 ****************************************************************************/

void fc7300_fcsmu_software_trigger_error(void);

/****************************************************************************
 * Name: fc7300_fcsmu_irq_attach
 *
 * Description:
 *   Interrupt registration function after the system starts.
 *
 ****************************************************************************/

void fc7300_fcsmu_irq_attach(void);

/****************************************************************************
 * Name: fc7300_fcsmu_irq_handler
 *
 * Description:
 *   Interrupt entry function used by EIM.
 *
 ****************************************************************************/

void fc7300_fcsmu_irq_handler(void);

/****************************************************************************
 * Name: fc7300_fcsmu_crc_software_trigger
 *
 * Description:
 *   Software triggered CRC check.
 *
 ****************************************************************************/

void fc7300_fcsmu_crc_software_trigger(void);

/****************************************************************************
 * Name: fc7300_fcsmu_crc_is_busy
 *
 * Description:
 *   Get crc check whether it is completed.
 *
 ****************************************************************************/

uint32_t fc7300_fcsmu_crc_is_busy(void);

/****************************************************************************
 * Name: fc7300_fcsmu_crc_result
 *
 * Description:
 *   Get crc result.
 *
 ****************************************************************************/

uint32_t fc7300_fcsmu_crc_result(void);

/****************************************************************************
 * Name: fc7300_fcsmu_clear_fault
 *
 * Description:
 *  Clear channel fault.
 *
 * Input Parameters:
 *   fault_channel - FCSMU channel mask
 *
 ****************************************************************************/
int fc7300_fcsmu_clear_fault(uint32_t fault_channel);

#undef EXTERN
#if defined(__cplusplus)
}
#endif

#endif /* __ASSEMBLY__ */
#endif /* __VENDOR_FLAGCHIP_CHIPS_FC7300_FC7300_FCSMU_H */
