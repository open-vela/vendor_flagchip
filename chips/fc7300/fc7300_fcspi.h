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

#ifndef __VENDOR_FLAGCHIP_CHIPS_FC7300_FC7300_FCSPI_H
#define __VENDOR_FLAGCHIP_CHIPS_FC7300_FC7300_FCSPI_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>

#include <stdbool.h>

#include <nuttx/spi/spi.h>

#include "chip.h"
#include "hardware/fc7300_fcspi.h"

/****************************************************************************
 * Public Functions Prototypes
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

struct spi_dev_s; /* Forward reference */

/****************************************************************************
 * Public Types
 ****************************************************************************/

enum fc7300_delay_e
{
  FCSPI_PCS_TO_SCK = 1,       /* PCS-to-SCK delay. */
  FCSPI_LAST_SCK_TO_PCS,      /* Last SCK edge to PCS delay. */
  FCSPI_BETWEEN_TRANSFER      /* Delay between transfers. */
};

/****************************************************************************
 * Name: fc7300_fcspibus_initialize
 *
 * Description:
 *   Initialize the selected SPI bus
 *
 * Input Parameters:
 *   bus number (for hardware that has multiple SPI interfaces)
 *
 * Returned Value:
 *   Valid SPI device structure reference on success; a NULL on failure
 *
 ****************************************************************************/

struct spi_dev_s *fc7300_fcspibus_initialize(int bus);

/****************************************************************************
 * Name: fc7300_spidev_initialize
 *
 * Description:
 *   Called to configure SPI chip select GPIO pins for the fc7300f8m board.
 *
 ****************************************************************************/

#ifdef CONFIG_SPI
int fc7300_spidev_initialize(void);
#endif

/****************************************************************************
 * Name:  fc7300_spi0/1/2/3/4/5/6/7select
 *
 * Description:
 *   The external functions, fc7300spi1/2/3/4/5/6/7 select must be provided
 *   by board-specific logic.
 *   They are implementations of the select and status methods of
 *   the SPI interface defined by struct spi_ops_s
 *   (see include/nuttx/spi/spi.h). All other methods
 *   (including fc7300spibus_initialize()) are provided by common
 *   logic. To use this common SPI logic on your board:
 *
 *   1. Provide logic in fc7300boardinitialize() to configure SPI chip select
 *      pins.
 *   2. Provide fc7300_fcspi1/2/3/4/5/6/7 select()
 *      functions in your board-specific logic. These functions will
 *      perform chip selection and status operations using GPIOs in
 *      the way your board is configured.
 *   3. Add a calls to fc7300spibus_initialize() in your low level
 *      application initialization logic
 *   4. The handle returned by fc7300_fcspibus_initialize() may then be
 *      used to bind the SPI driver to higher level logic (e.g., calling
 *      mmcsd_spislotinitialize(), for example, will bind the SPI
 *      driver to the SPI MMC/SD driver).
 *
 ****************************************************************************/

#ifndef CONFIG_FC7300_FCSPI0_HWPCS
void fc7300_fcspi0select(struct spi_dev_s *dev, uint32_t devid,
                      bool selected);
#endif

#ifndef CONFIG_FC7300_FCSPI1_HWPCS
void fc7300_fcspi1select(struct spi_dev_s *dev, uint32_t devid,
                      bool selected);
#endif

#ifndef CONFIG_FC7300_FCSPI2_HWPCS
void fc7300_fcspi2select(struct spi_dev_s *dev, uint32_t devid,
                      bool selected);
#endif

#ifndef CONFIG_FC7300_FCSPI3_HWPCS
void fc7300_fcspi3select(struct spi_dev_s *dev, uint32_t devid,
                      bool selected);
#endif

#ifndef CONFIG_FC7300_FCSPI4_HWPCS
void fc7300_fcspi4select(struct spi_dev_s *dev, uint32_t devid,
                      bool selected);
#endif

#ifndef CONFIG_FC7300_FCSPI5_HWPCS
void fc7300_fcspi5select(struct spi_dev_s *dev, uint32_t devid,
                      bool selected);
#endif

#ifndef CONFIG_FC7300_FCSPI6_HWPCS
void fc7300_fcspi6select(struct spi_dev_s *dev, uint32_t devid,
                      bool selected);
#endif

#ifndef CONFIG_FC7300_FCSPI7_HWPCS
void fc7300_fcspi7select(struct spi_dev_s *dev, uint32_t devid,
                      bool selected);
#endif

#undef EXTERN
#if defined(__cplusplus)
}
#endif

#endif /* __ASSEMBLY__ */
#endif /* __VENDOR_FLAGCHIP_CHIPS_FC7300_FC7300_FCSPI_H */
