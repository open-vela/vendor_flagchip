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
#include <errno.h>
#include <debug.h>

#include <sys/param.h>

#include <nuttx/spi/spi.h>
#include <nuttx/spi/spi_transfer.h>
#include <arch/board/board.h>

#include "arm_internal.h"
#include "chip.h"
#include "fc7300_fcspi.h"

#include "fc7300f8m-evb.h"

#ifdef CONFIG_FC7300_TIC12400
#include <tic12400.h>
#endif

#if defined(CONFIG_SPI)

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Private Data
 ****************************************************************************/

#if defined(CONFIG_FC7300_FCSPI0)
static const uint32_t g_spi0gpio[] =
{
#if defined(GPIO_SPI0_CS0)
  GPIO_SPI0_CS0,
#else
  0,
#endif
#if defined(GPIO_SPI0_CS1)
  GPIO_SPI0_CS1,
#else
  0,
#endif
#if defined(GPIO_SPI0_CS2)
  GPIO_SPI0_CS2,
#else
  0,
#endif
#if defined(GPIO_SPI0_CS3)
  GPIO_SPI0_CS3,
#else
  0,
#endif
#if defined(GPIO_SPI0_CS4)
  GPIO_SPI0_CS4,
#else
  0,
#endif
#if defined(GPIO_SPI0_CS5)
  GPIO_SPI0_CS5
#else
  0
#endif
};
#endif

#if defined(CONFIG_FC7300_FCSPI1)
static const uint32_t g_spi1gpio[] =
{
#if defined(GPIO_SPI1_CS0)
  GPIO_SPI1_CS0,
#else
  0,
#endif
#if defined(GPIO_SPI1_CS1)
  GPIO_SPI1_CS1,
#else
  0,
#endif
#if defined(GPIO_SPI1_CS2)
  GPIO_SPI1_CS2,
#else
  0,
#endif
#if defined(GPIO_SPI1_CS3)
  GPIO_SPI1_CS3,
#else
  0,
#endif
#if defined(GPIO_SPI1_CS4)
  GPIO_SPI1_CS4,
#else
  0,
#endif
#if defined(GPIO_SPI1_CS5)
  GPIO_SPI1_CS5
#else
  0
#endif
};
#endif

#if defined(CONFIG_FC7300_FCSPI2)
static const uint32_t g_spi2gpio[] =
{
#if defined(GPIO_SPI2_CS0)
  GPIO_SPI2_CS0,
#else
  0,
#endif
#if defined(GPIO_SPI2_CS1)
  GPIO_SPI2_CS1,
#else
  0,
#endif
#if defined(GPIO_SPI2_CS2)
  GPIO_SPI2_CS2,
#else
  0,
#endif
#if defined(GPIO_SPI2_CS3)
  GPIO_SPI2_CS3,
#else
  0,
#endif
#if defined(GPIO_SPI2_CS4)
  GPIO_SPI2_CS4,
#else
  0,
#endif
#if defined(GPIO_SPI2_CS5)
  GPIO_SPI2_CS5
#else
  0
#endif
};
#endif

#if defined(CONFIG_FC7300_FCSPI3)
static const uint32_t g_spi3gpio[] =
{
#if defined(GPIO_SPI3_CS0)
  GPIO_SPI3_CS0,
#else
  0,
#endif
#if defined(GPIO_SPI3_CS1)
  GPIO_SPI3_CS1,
#else
  0,
#endif
#if defined(GPIO_SPI3_CS2)
  GPIO_SPI3_CS2,
#else
  0,
#endif
#if defined(GPIO_SPI3_CS3)
  GPIO_SPI3_CS3,
#else
  0,
#endif
#if defined(GPIO_SPI3_CS4)
  GPIO_SPI3_CS4,
#else
  0,
#endif
#if defined(GPIO_SPI3_CS5)
  GPIO_SPI3_CS5
#else
  0
#endif
};
#endif

#if defined(CONFIG_FC7300_FCSPI4)
static const uint32_t g_spi4gpio[] =
{
#if defined(GPIO_SPI4_CS0)
  GPIO_SPI4_CS0,
#else
  0,
#endif
#if defined(GPIO_SPI4_CS1)
  GPIO_SPI4_CS1,
#else
  0,
#endif
#if defined(GPIO_SPI4_CS2)
  GPIO_SPI4_CS2,
#else
  0,
#endif
#if defined(GPIO_SPI4_CS3)
  GPIO_SPI4_CS3,
#else
  0,
#endif
#if defined(GPIO_SPI4_CS4)
  GPIO_SPI4_CS4,
#else
  0,
#endif
#if defined(GPIO_SPI4_CS5)
  GPIO_SPI4_CS5
#else
  0
#endif
};
#endif

#if defined(CONFIG_FC7300_FCSPI5)
static const uint32_t g_spi5gpio[] =
{
#if defined(GPIO_SPI5_CS0)
  GPIO_SPI5_CS0,
#else
  0,
#endif
#if defined(GPIO_SPI5_CS1)
  GPIO_SPI5_CS1,
#else
  0,
#endif
#if defined(GPIO_SPI5_CS2)
  GPIO_SPI5_CS2,
#else
  0,
#endif
#if defined(GPIO_SPI5_CS3)
  GPIO_SPI5_CS3,
#else
  0,
#endif
#if defined(GPIO_SPI5_CS4)
  GPIO_SPI5_CS4,
#else
  0,
#endif
#if defined(GPIO_SPI5_CS5)
  GPIO_SPI5_CS5
#else
  0
#endif
};
#endif

#if defined(CONFIG_FC7300_FCSPI6)
static const uint32_t g_spi6gpio[] =
{
#if defined(GPIO_SPI6_CS0)
  GPIO_SPI6_CS0,
#else
  0,
#endif
#if defined(GPIO_SPI6_CS1)
  GPIO_SPI6_CS1,
#else
  0,
#endif
#if defined(GPIO_SPI6_CS2)
  GPIO_SPI6_CS2,
#else
  0,
#endif
#if defined(GPIO_SPI6_CS3)
  GPIO_SPI6_CS3,
#else
  0,
#endif
#if defined(GPIO_SPI6_CS4)
  GPIO_SPI6_CS4,
#else
  0,
#endif
#if defined(GPIO_SPI6_CS5)
  GPIO_SPI6_CS5
#else
  0
#endif
};
#endif

#if defined(CONFIG_FC7300_FCSPI7)
static const uint32_t g_spi7gpio[] =
{
#if defined(GPIO_SPI7_CS0)
  GPIO_SPI7_CS0,
#else
  0,
#endif
#if defined(GPIO_SPI7_CS1)
  GPIO_SPI7_CS1,
#else
  0,
#endif
#if defined(GPIO_SPI7_CS2)
  GPIO_SPI7_CS2,
#else
  0,
#endif
#if defined(GPIO_SPI7_CS3)
  GPIO_SPI7_CS3,
#else
  0,
#endif
#if defined(GPIO_SPI7_CS4)
  GPIO_SPI7_CS4,
#else
  0,
#endif
#if defined(GPIO_SPI7_CS5)
  GPIO_SPI7_CS5
#else
  0
#endif
};
#endif

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: fc7300_spidev_initialize
 *
 * Description:
 *   Configure chip select pins, initialize the SPI driver and register
 *   /dev/spiN devices.
 *
 ****************************************************************************/

int fc7300_spidev_initialize(void)
{
  /* Configure SPI CS GPIO for output */

  int ret = OK;

#if defined(CONFIG_FC7300_FCSPI0)
  for (int i = 0; i < nitems(g_spi0gpio); i++)
    {
      if (g_spi0gpio[i] != 0)
        {
          fc7300_pinconfig(g_spi0gpio[i]);
        }
    }
#  ifdef CONFIG_SPI_DRIVER

  /* Initialize the SPI driver for FCSPI0 */

  struct spi_dev_s *g_fcspi0 = fc7300_fcspibus_initialize(0);
  if (g_fcspi0 == NULL)
    {
      spierr("ERROR: FAILED to initialize FCSPI0\n");
      return -ENODEV;
    }

#ifdef CONFIG_FC7300_TIC12400

  ret = tic12400_register("/dev/tic12400", g_fcspi0, 0);
  if (ret < 0)
  {
    spierr("ERROR: FAILED to register tic12400 driver\n");
    return ret;
  }

#else
  ret = spi_register(g_fcspi0, 0);
  if (ret < 0)
    {
      spierr("ERROR: FAILED to register FCSPI0 driver\n");
      return ret;
    }
#endif /* CONFIG_FC7300_TIC12400 */
#  endif  /* CONFIG_SPI_DRIVER */
#endif

#if defined(CONFIG_FC7300_FCSPI1)
  for (int i = 0; i < nitems(g_spi1gpio); i++)
    {
      if (g_spi1gpio[i] != 0)
        {
          fc7300_pinconfig(g_spi1gpio[i]);
        }
    }
#  ifdef CONFIG_SPI_DRIVER

  /* Initialize the SPI driver for FCSPI1 */

  struct spi_dev_s *g_fcspi1 = fc7300_fcspibus_initialize(1);
  if (g_fcspi1 == NULL)
    {
      spierr("ERROR: FAILED to initialize FCSPI1\n");
      return -ENODEV;
    }

  ret = spi_register(g_fcspi1, 1);
  if (ret < 0)
    {
      spierr("ERROR: FAILED to register FCSPI1 driver\n");
      return ret;
    }
#  endif /* CONFIG_SPI_DRIVER */
#endif

#if defined(CONFIG_FC7300_FCSPI2)
  for (int i = 0; i < nitems(g_spi2gpio); i++)
    {
      if (g_spi2gpio[i] != 0)
        {
          fc7300_pinconfig(g_spi2gpio[i]);
        }
    }
#  ifdef CONFIG_SPI_DRIVER

  /* Initialize the SPI driver for FCSPI2 */

  struct spi_dev_s *g_fcspi2 = fc7300_fcspibus_initialize(2);
  if (g_fcspi2 == NULL)
    {
      spierr("ERROR: FAILED to initialize FCSPI2\n");
      return -ENODEV;
    }

  ret = spi_register(g_fcspi2, 2);
  if (ret < 0)
    {
      spierr("ERROR: FAILED to register FCSPI2 driver\n");
      return ret;
    }
#  endif /* CONFIG_SPI_DRIVER */
#endif

#if defined(CONFIG_FC7300_FCSPI3)
  for (int i = 0; i < nitems(g_spi3gpio); i++)
    {
      if (g_spi3gpio[i] != 0)
        {
          fc7300_pinconfig(g_spi3gpio[i]);
        }
    }
#  ifdef CONFIG_SPI_DRIVER

  /* Initialize the SPI driver for FCSPI3 */

  struct spi_dev_s *g_fcspi3 = fc7300_fcspibus_initialize(3);
  if (g_fcspi3 == NULL)
    {
      spierr("ERROR: FAILED to initialize FCSPI3\n");
      return -ENODEV;
    }

  ret = spi_register(g_fcspi3, 3);
  if (ret < 0)
    {
      spierr("ERROR: FAILED to register FCSPI3 driver\n");
      return ret;
    }
#  endif /* CONFIG_SPI_DRIVER */
#endif

#if defined(CONFIG_FC7300_FCSPI4)
  for (int i = 0; i < nitems(g_spi4gpio); i++)
    {
      if (g_spi4gpio[i] != 0)
        {
          fc7300_pinconfig(g_spi4gpio[i]);
        }
    }
#  ifdef CONFIG_SPI_DRIVER

  /* Initialize the SPI driver for FCSPI4 */

  struct spi_dev_s *g_fcspi4 = fc7300_fcspibus_initialize(4);
  if (g_fcspi4 == NULL)
    {
      spierr("ERROR: FAILED to initialize FCSPI4\n");
      return -ENODEV;
    }

  ret = spi_register(g_fcspi4, 4);
  if (ret < 0)
    {
      spierr("ERROR: FAILED to register FCSPI4 driver\n");
      return ret;
    }
#  endif /* CONFIG_SPI_DRIVER */
#endif

#if defined(CONFIG_FC7300_FCSPI5)
  for (int i = 0; i < nitems(g_spi5gpio); i++)
    {
      if (g_spi5gpio[i] != 0)
        {
          fc7300_pinconfig(g_spi5gpio[i]);
        }
    }
#  ifdef CONFIG_SPI_DRIVER

  /* Initialize the SPI driver for FCSPI5 */

  struct spi_dev_s *g_fcspi5 = fc7300_fcspibus_initialize(5);
  if (g_fcspi5 == NULL)
    {
      spierr("ERROR: FAILED to initialize FCSPI5\n");
      return -ENODEV;
    }

  ret = spi_register(g_fcspi5, 5);
  if (ret < 0)
    {
      spierr("ERROR: FAILED to register FCSPI5 driver\n");
      return ret;
    }
#  endif /* CONFIG_SPI_DRIVER */
#endif

#if defined(CONFIG_FC7300_FCSPI6)
  for (int i = 0; i < nitems(g_spi6gpio); i++)
    {
      if (g_spi6gpio[i] != 0)
        {
          fc7300_pinconfig(g_spi6gpio[i]);
        }
    }
#  ifdef CONFIG_SPI_DRIVER

  /* Initialize the SPI driver for FCSPI6 */

  struct spi_dev_s *g_fcspi6 = fc7300_fcspibus_initialize(6);
  if (g_fcspi6 == NULL)
    {
      spierr("ERROR: FAILED to initialize FCSPI6\n");
      return -ENODEV;
    }

  ret = spi_register(g_fcspi6, 6);
  if (ret < 0)
    {
      spierr("ERROR: FAILED to register FCSPI6 driver\n");
      return ret;
    }
#  endif /* CONFIG_SPI_DRIVER */
#endif

#if defined(CONFIG_FC7300_FCSPI7)
  for (int i = 0; i < nitems(g_spi7gpio); i++)
    {
      if (g_spi7gpio[i] != 0)
        {
          fc7300_pinconfig(g_spi7gpio[i]);
        }
    }
#  ifdef CONFIG_SPI_DRIVER

  /* Initialize the SPI driver for FCSPI6 */

  struct spi_dev_s *g_fcspi7 = fc7300_fcspibus_initialize(7);
  if (g_fcspi7 == NULL)
    {
      spierr("ERROR: FAILED to initialize FCSPI7\n");
      return -ENODEV;
    }

  ret = spi_register(g_fcspi7, 7);
  if (ret < 0)
    {
      spierr("ERROR: FAILED to register FCSPI7 driver\n");
      return ret;
    }
#  endif /* CONFIG_SPI_DRIVER */

#endif
  return ret;
}

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
#if defined(CONFIG_FC7300_FCSPI0)
#  ifndef CONFIG_FC7300_FCSPI0_HWPCS
void fc7300_fcspi0select(struct spi_dev_s *dev, uint32_t devid,
                                 bool selected)
{
  uint32_t index = SPIDEVID_INDEX(devid);

  spiinfo("devid: %d CS: %s\n",
          (int)devid, selected ? "assert" : "de-assert");

  if (g_spi0gpio[index] != 0)
    {
      fc7300_gpiowrite(g_spi0gpio[index], !selected);
    }
}
#  endif
#endif

#if defined(CONFIG_FC7300_FCSPI1)
#  ifndef CONFIG_FC7300_FCSPI1_HWPCS
void fc7300_fcspi1select(struct spi_dev_s *dev,
                      uint32_t devid, bool selected)
{
  uint32_t index = SPIDEVID_INDEX(devid);

  spiinfo("devid: %d CS: %s\n",
          (int)devid, selected ? "assert" : "de-assert");

  if (g_spi1gpio[index] != 0)
    {
      fc7300_gpiowrite(g_spi1gpio[index], !selected);
    }
}
#  endif
#endif

#if defined(CONFIG_FC7300_FCSPI2)
#  ifndef CONFIG_FC7300_FCSPI2_HWPCS
void fc7300_fcspi2select(struct spi_dev_s *dev,
                      uint32_t devid, bool selected)
{
  uint32_t index = SPIDEVID_INDEX(devid);

  spiinfo("devid: %d CS: %s\n",
          (int)devid, selected ? "assert" : "de-assert");

  if (g_spi2gpio[index] != 0)
    {
      fc7300_gpiowrite(g_spi2gpio[index], !selected);
    }
}
#  endif
#endif

#if defined(CONFIG_FC7300_FCSPI3)
#ifndef CONFIG_FC7300_FCSPI3_HWPCS
void fc7300_fcspi3select(struct spi_dev_s *dev,
                      uint32_t devid, bool selected)
{
  uint32_t index = SPIDEVID_INDEX(devid);

  spiinfo("devid: %d CS: %s\n",
          (int)devid, selected ? "assert" : "de-assert");

  if (g_spi3gpio[index] != 0)
    {
      fc7300_gpiowrite(g_spi3gpio[index], !selected);
    }
}
#  endif
#endif

#if defined(CONFIG_FC7300_FCSPI4)
#ifndef CONFIG_FC7300_FCSPI4_HWPCS
void fc7300_fcspi4select(struct spi_dev_s *dev,
                      uint32_t devid, bool selected)
{
  uint32_t index = SPIDEVID_INDEX(devid);

  spiinfo("devid: %d CS: %s\n",
           (int)devid, selected ? "assert" : "de-assert");

  if (g_spi4gpio[index] != 0)
    {
      fc7300_gpiowrite(g_spi4gpio[index], !selected);
    }
}
#  endif
#endif

#if defined(CONFIG_FC7300_FCSPI5)
#ifndef CONFIG_FC7300_FCSPI5_HWPCS
void fc7300_fcspi5select(struct spi_dev_s *dev,
                      uint32_t devid, bool selected)
{
  uint32_t index = SPIDEVID_INDEX(devid);

  spiinfo("devid: %d CS: %s\n",
          (int)devid, selected ? "assert" : "de-assert");

  if (g_spi5gpio[index] != 0)
    {
      fc7300_gpiowrite(g_spi5gpio[index], !selected);
    }
}
#  endif
#endif

#if defined(CONFIG_FC7300_FCSPI6)
#ifndef CONFIG_FC7300_FCSPI6_HWPCS
void fc7300_fcspi6select(struct spi_dev_s *dev,
                      uint32_t devid, bool selected)
{
  uint32_t index = SPIDEVID_INDEX(devid);

  spiinfo("devid: %d CS: %s\n",
          (int)devid, selected ? "assert" : "de-assert");

  if (g_spi6gpio[index] != 0)
    {
      fc7300_gpiowrite(g_spi6gpio[index], !selected);
    }
}
#  endif
#endif

#if defined(CONFIG_FC7300_FCSPI7)
#ifndef CONFIG_FC7300_FCSPI7_HWPCS
void fc7300_fcspi7select(struct spi_dev_s *dev,
                      uint32_t devid, bool selected)
{
  uint32_t index = SPIDEVID_INDEX(devid);

  spiinfo("devid: %d CS: %s\n",
          (int)devid, selected ? "assert" : "de-assert");

  if (g_spi7gpio[index] != 0)
    {
      fc7300_gpiowrite(g_spi7gpio[index], !selected);
    }
}
#  endif
#endif

#endif /* defined(CONFIG_SPI) */
