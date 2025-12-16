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

#include <sys/types.h>
#include <byteswap.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <assert.h>
#include <errno.h>
#include <debug.h>
#include <endian.h>

#include <nuttx/irq.h>
#include <nuttx/arch.h>
#include <nuttx/mutex.h>
#include <nuttx/spinlock.h>
#include <nuttx/spi/spi.h>
#include <nuttx/power/pm.h>

#include "arm_internal.h"
#include "chip.h"

#include "hardware/fc7300_pin.h"
#include "hardware/fc7300_pinmux.h"
#include "hardware/fc7300_fcspi.h"
#include "fc7300_periphclocks.h"
#include "fc7300_fcspi.h"

#ifdef CONFIG_FC7300_FCSPI_DMA
#  include "hardware/fc7300_dmamux.h"
#  include "fc7300_dma.h"
#endif

#include <arch/board/board.h>

#if defined(CONFIG_FC7300_FCSPI0) || defined(CONFIG_FC7300_FCSPI1) || \
    defined(CONFIG_FC7300_FCSPI2) || defined(CONFIG_FC7300_FCSPI3) || \
    defined(CONFIG_FC7300_FCSPI4) || defined(CONFIG_FC7300_FCSPI5) || \
    defined(CONFIG_FC7300_FCSPI6) || defined(CONFIG_FC7300_FCSPI7)

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Configuration ************************************************************/

/* SPI interrupts */

#ifdef CONFIG_FC7300_FCSPI_INTERRUPTS
#  error "Interrupt driven SPI not yet supported"
#endif

/* Can't have both interrupt driven SPI and SPI DMA */

#if defined(CONFIG_FC7300_FCSPI_INTERRUPTS) && defined(CONFIG_FC7300_FCSPI_DMA)
#  error "Cannot enable both interrupt mode and DMA mode for SPI"
#endif

#define  SPI_STATUS_CLEAR   (FCSPI_STATUS_RX_WF | FCSPI_STATUS_FEF | \
                             FCSPI_STATUS_TCF   | FCSPI_STATUS_TX_FU | \
                             FCSPI_STATUS_RX_FO | FCSPI_STATUS_DMF)

/* Power management definitions */

#define INIT_DELAY_NS 1000000000U

#define FCSPI_GET_STATUS(priv) \
        fc7300_fcspi_getreg32(priv, FC7300_FCSPI_STATUS_OFFSET)

/****************************************************************************
 * Private Types
 ****************************************************************************/

struct fc7300_fcspidev_s
{
  struct spi_dev_s spidev;      /* Externally visible part of the SPI interface */
  uint32_t spibase;             /* SPIn base address */
#ifdef CONFIG_FC7300_FCSPI_INTERRUPTS
  uint8_t spiirq;               /* SPI IRQ number */
#endif
  mutex_t lock;                 /* Held while chip is selected for mutual exclusion */
  uint32_t frequency;           /* Requested clock frequency */
  uint32_t actual;              /* Actual clock frequency */
  int8_t nbits;                 /* Width of word in bits */
  uint8_t modeinit_flg;
  bool continuous;              /* Keep PCS select enable until transfer finish */
  uint8_t nwords;               /* Characterize the number of bytes transmitted */
  uint8_t prescale;             /* Prescale value in TR_CTRL register */
  uint32_t pcs;                 /* Peripheral Chip Select currently used */
  uint32_t csdelay;
  bool swap;                    /* MOSI swap with MISO */
#ifdef CONFIG_FC7300_FCSPI_DMA
  volatile uint32_t rxresult;   /* Result of the RX DMA */
  volatile uint32_t txresult;   /* Result of the TX DMA */
  const uint16_t    rxch;       /* The RX DMA channel number */
  const uint16_t    txch;       /* The TX DMA channel number */
  DMACH_HANDLE      rxdma;      /* DMA channel handle for RX transfers */
  DMACH_HANDLE      txdma;      /* DMA channel handle for TX transfers */
  sem_t             rxsem;      /* Wait for RX DMA to complete */
  sem_t             txsem;      /* Wait for TX DMA to complete */
#endif
  volatile uint32_t curframetrctrl;
};

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

/* Helpers */

static inline
uint32_t fc7300_fcspi_getreg32(struct fc7300_fcspidev_s *priv,
                                uint8_t offset);
static inline
void fc7300_fcspi_putreg32(struct fc7300_fcspidev_s *priv,
                            uint8_t offset, uint32_t value);
static inline
uint32_t fc7300_fcspi_readword(struct fc7300_fcspidev_s *priv);
static inline
void fc7300_fcspi_writeword(struct fc7300_fcspidev_s *priv,
                             uint32_t byte);
static inline
uint16_t fc7300_fcspi_9to16bitmode(struct fc7300_fcspidev_s *priv);
static uint32_t fc7300_fcspi_pckfreq(uintptr_t base);
static void fc7300_fcspi_set_delays(struct fc7300_fcspidev_s *priv,
                                    uint32_t delay_ns,
                                    enum fc7300_delay_e type);
static inline
void fc7300_fcspi_set_delay_scaler(struct fc7300_fcspidev_s *priv,
                                    uint32_t scaler,
                                    enum fc7300_delay_e type);

/* DMA support */

#ifdef CONFIG_FC7300_FCSPI_DMA
static int         spi_dmarxwait(struct fc7300_fcspidev_s *priv);
static int         spi_dmatxwait(struct fc7300_fcspidev_s *priv);
static inline void spi_dmarxwakeup(struct fc7300_fcspidev_s *priv);
static inline void spi_dmatxwakeup(struct fc7300_fcspidev_s *priv);
static void        spi_dmarxcallback(DMACH_HANDLE handle, void *arg,
                                     bool done, int result);
static void        spi_dmatxcallback(DMACH_HANDLE handle, void *arg,
                                     bool done, int result);
static inline void spi_dmarxstart(struct fc7300_fcspidev_s *priv);
static inline void spi_dmatxstart(struct fc7300_fcspidev_s *priv);
#endif

/* SPI methods */

static int fc7300_fcspi_lock(struct spi_dev_s *dev, bool lock);

#if defined(CONFIG_FC7300_FCSPI0_HWPCS) || \
    defined(CONFIG_FC7300_FCSPI1_HWPCS) || \
    defined(CONFIG_FC7300_FCSPI2_HWPCS) || \
    defined(CONFIG_FC7300_FCSPI3_HWPCS) || \
    defined(CONFIG_FC7300_FCSPI4_HWPCS) || \
    defined(CONFIG_FC7300_FCSPI5_HWPCS) || \
    defined(CONFIG_FC7300_FCSPI6_HWPCS) || \
    defined(CONFIG_FC7300_FCSPI7_HWPCS)
static void fc7300_fcspi_select(struct spi_dev_s *dev, uint32_t devid,
                                 bool selected);
#endif

static uint32_t fc7300_fcspi_setfrequency(struct spi_dev_s *dev,
              uint32_t frequency);
static void fc7300_fcspi_setmode(struct spi_dev_s *dev,
              enum spi_mode_e mode);
static void fc7300_fcspi_setbits(struct spi_dev_s *dev, int nbits);
#ifdef CONFIG_SPI_HWFEATURES
static int fc7300_fcspi_hwfeatures(struct spi_dev_s *dev,
              spi_hwfeatures_t features);
#endif
static uint32_t fc7300_fcspi_send(struct spi_dev_s *dev, uint32_t wd);
static void fc7300_fcspi_exchange(struct spi_dev_s *dev,
              const void *txbuffer, void *rxbuffer, size_t nwords);
#ifndef CONFIG_SPI_EXCHANGE
static void fc7300_fcspi_sndblock(struct spi_dev_s *dev,
              const void *txbuffer, size_t nwords);
static void fc7300_fcspi_recvblock(struct spi_dev_s *dev,
                                    void *rxbuffer,
                                    size_t nwords);
#endif
#ifdef CONFIG_SPI_DELAY_CONTROL
static int fc7300_fcspi_setdelay(struct spi_dev_s *dev, uint32_t startdelay,
                                 uint32_t stopdelay, uint32_t csdelay,
                                 uint32_t ifdelay);
#endif

/* Initialization */

static void
fc7300_fcspi_bus_initialize(struct fc7300_fcspidev_s *priv);

/****************************************************************************
 * Private Data
 ****************************************************************************/

#ifdef CONFIG_FC7300_FCSPI0
static const struct spi_ops_s g_spi0ops =
{
  .lock         = fc7300_fcspi_lock,
#ifdef CONFIG_FC7300_FCSPI0_HWPCS
  .select       = fc7300_fcspi_select,
#else
  .select       = fc7300_fcspi0select,
#endif
  .setfrequency = fc7300_fcspi_setfrequency,
  .setmode      = fc7300_fcspi_setmode,
  .setbits      = fc7300_fcspi_setbits,
#ifdef CONFIG_SPI_DELAY_CONTROL
  .setdelay     = fc7300_fcspi_setdelay,
#endif
#ifdef CONFIG_SPI_HWFEATURES
  .hwfeatures   = fc7300_fcspi_hwfeatures,
#endif
#ifdef CONFIG_SPI_CMDDATA
  .cmddata      = fc7300_fcspi0cmddata,
#endif
  .send         = fc7300_fcspi_send,
#ifdef CONFIG_SPI_EXCHANGE
  .exchange     = fc7300_fcspi_exchange,
#else
  .sndblock     = fc7300_fcspi_sndblock,
  .recvblock    = fc7300_fcspi_recvblock,
#endif
  .registercallback = NULL,  /* Not implemented */
};

static struct fc7300_fcspidev_s g_fcspi0dev =
{
  .spidev       =
  {
    .ops        = &g_spi0ops,
  },
  .spibase      = FC7300_FCSPI0_BASE,
  .modeinit_flg = SPIDEV_MODETI,
#ifdef CONFIG_FC7300_FCSPI0_PIN_SWAP
  .swap   = true,
#else
  .swap   = false,
#endif
#ifdef CONFIG_FC7300_FCSPI0_CONTEN
  .continuous   = true,
#else
  .continuous   = false,
#endif
  .csdelay      = CONFIG_FC7300_FCSPI0_CSDELAY,
#ifdef CONFIG_FC7300_FCSPI_INTERRUPTS
  .spiirq       = FC7300_IRQ_FCSPI0,
#endif
  .lock         = NXMUTEX_INITIALIZER,
#ifdef CONFIG_FC7300_FCSPI0_DMA
  .rxch         = FC7300_DMACHAN_FCSPI0_RX,
  .txch         = FC7300_DMACHAN_FCSPI0_TX,
#endif
};
#endif

#ifdef CONFIG_FC7300_FCSPI1
static const struct spi_ops_s g_spi1ops =
{
  .lock         = fc7300_fcspi_lock,
#ifdef CONFIG_FC7300_FCSPI1_HWPCS
  .select       = fc7300_fcspi_select,
#else
  .select       = fc7300_fcspi1select,
#endif
  .setfrequency = fc7300_fcspi_setfrequency,
  .setmode      = fc7300_fcspi_setmode,
  .setbits      = fc7300_fcspi_setbits,
#ifdef CONFIG_SPI_DELAY_CONTROL
  .setdelay     = fc7300_fcspi_setdelay,
#endif
#ifdef CONFIG_SPI_HWFEATURES
  .hwfeatures   = fc7300_fcspi_hwfeatures,
#endif
#ifdef CONFIG_SPI_CMDDATA
  .cmddata      = fc7300_fcspi1cmddata,
#endif
  .send         = fc7300_fcspi_send,
#ifdef CONFIG_SPI_EXCHANGE
  .exchange     = fc7300_fcspi_exchange,
#else
  .sndblock     = fc7300_fcspi_sndblock,
  .recvblock    = fc7300_fcspi_recvblock,
#endif
  .registercallback = NULL,  /* Not implemented */
};

static struct fc7300_fcspidev_s g_fcspi1dev =
{
  .spidev       =
  {
    .ops        = &g_spi1ops,
  },
  .spibase      = FC7300_FCSPI1_BASE,
  .modeinit_flg = SPIDEV_MODETI,
#ifdef CONFIG_FC7300_FCSPI1_PIN_SWAP
  .swap   = true,
#else
  .swap   = false,
#endif
#ifdef CONFIG_FC7300_FCSPI1_CONTEN
  .continuous   = true,
#else
  .continuous   = false,
#endif
  .csdelay      = CONFIG_FC7300_FCSPI1_CSDELAY,
#ifdef CONFIG_FC7300_FCSPI_INTERRUPTS
  .spiirq       = FC7300_IRQ_FCSPI1,
#endif
  .lock         = NXMUTEX_INITIALIZER,
#ifdef CONFIG_FC7300_FCSPI1_DMA
  .rxch         = FC7300_DMACHAN_FCSPI1_RX,
  .txch         = FC7300_DMACHAN_FCSPI1_TX,
#endif
};
#endif

#ifdef CONFIG_FC7300_FCSPI2
static const struct spi_ops_s g_spi2ops =
{
  .lock         = fc7300_fcspi_lock,
#ifdef CONFIG_FC7300_FCSPI2_HWPCS
  .select       = fc7300_fcspi_select,
#else
  .select       = fc7300_fcspi2select,
#endif
  .setfrequency = fc7300_fcspi_setfrequency,
  .setmode      = fc7300_fcspi_setmode,
  .setbits      = fc7300_fcspi_setbits,
#ifdef CONFIG_SPI_DELAY_CONTROL
  .setdelay     = fc7300_fcspi_setdelay,
#endif
#ifdef CONFIG_SPI_HWFEATURES
  .hwfeatures   = fc7300_fcspi_hwfeatures,
#endif
#ifdef CONFIG_SPI_CMDDATA
  .cmddata      = fc7300_fcspi2cmddata,
#endif
  .send         = fc7300_fcspi_send,
#ifdef CONFIG_SPI_EXCHANGE
  .exchange     = fc7300_fcspi_exchange,
#else
  .sndblock     = fc7300_fcspi_sndblock,
  .recvblock    = fc7300_fcspi_recvblock,
#endif
  .registercallback = NULL,  /* Not implemented */
};

static struct fc7300_fcspidev_s g_fcspi2dev =
{
  .spidev       =
  {
    .ops        = &g_spi2ops,
  },
  .spibase      = FC7300_FCSPI2_BASE,
  .modeinit_flg = SPIDEV_MODETI,
#ifdef CONFIG_FC7300_FCSPI2_PIN_SWAP
  .swap   = true,
#else
  .swap   = false,
#endif
#ifdef CONFIG_FC7300_FCSPI2_CONTEN
  .continuous   = true,
#else
  .continuous   = false,
#endif
  .csdelay      = CONFIG_FC7300_FCSPI2_CSDELAY,
#ifdef CONFIG_FC7300_FCSPI_INTERRUPTS
  .spiirq       = FC7300_IRQ_FCSPI2,
#endif
  .lock         = NXMUTEX_INITIALIZER,
#ifdef CONFIG_FC7300_FCSPI2_DMA
  .rxch         = FC7300_DMACHAN_FCSPI2_RX,
  .txch         = FC7300_DMACHAN_FCSPI2_TX,
#endif
};
#endif

#ifdef CONFIG_FC7300_FCSPI3
static const struct spi_ops_s g_spi3ops =
{
  .lock         = fc7300_fcspi_lock,
#ifdef CONFIG_FC7300_FCSPI3_HWPCS
  .select       = fc7300_fcspi_select,
#else
  .select       = fc7300_fcspi3select,
#endif
  .setfrequency = fc7300_fcspi_setfrequency,
  .setmode      = fc7300_fcspi_setmode,
  .setbits      = fc7300_fcspi_setbits,
#ifdef CONFIG_SPI_DELAY_CONTROL
  .setdelay     = fc7300_fcspi_setdelay,
#endif
#ifdef CONFIG_SPI_HWFEATURES
  .hwfeatures   = fc7300_fcspi_hwfeatures,
#endif
#ifdef CONFIG_SPI_CMDDATA
  .cmddata      = fc7300_fcspi3cmddata,
#endif
  .send         = fc7300_fcspi_send,
#ifdef CONFIG_SPI_EXCHANGE
  .exchange     = fc7300_fcspi_exchange,
#else
  .sndblock     = fc7300_fcspi_sndblock,
  .recvblock    = fc7300_fcspi_recvblock,
#endif
  .registercallback = NULL,  /* Not implemented */
};

static struct fc7300_fcspidev_s g_fcspi3dev =
{
  .spidev       =
  {
    .ops        = &g_spi3ops,
  },
  .spibase      = FC7300_FCSPI3_BASE,
  .modeinit_flg = SPIDEV_MODETI,
#ifdef CONFIG_FC7300_FCSPI3_PIN_SWAP
  .swap   = true,
#else
  .swap   = false,
#endif
#ifdef CONFIG_FC7300_FCSPI3_CONTEN
  .continuous   = true,
#else
  .continuous   = false,
#endif
  .csdelay      = CONFIG_FC7300_FCSPI3_CSDELAY,
#ifdef CONFIG_FC7300_FCSPI_INTERRUPTS
  .spiirq       = FC7300_IRQ_FCSPI3,
#endif
  .lock         = NXMUTEX_INITIALIZER,
#ifdef CONFIG_FC7300_FCSPI3_DMA
  .rxch         = FC7300_DMACHAN_FCSPI3_RX,
  .txch         = FC7300_DMACHAN_FCSPI3_TX,
#endif
};
#endif

#ifdef CONFIG_FC7300_FCSPI4
static const struct spi_ops_s g_spi4ops =
{
  .lock         = fc7300_fcspi_lock,
#ifdef CONFIG_FC7300_FCSPI4_HWPCS
  .select       = fc7300_fcspi_select,
#else
  .select       = fc7300_fcspi4select,
#endif
  .setfrequency = fc7300_fcspi_setfrequency,
  .setmode      = fc7300_fcspi_setmode,
  .setbits      = fc7300_fcspi_setbits,
#ifdef CONFIG_SPI_DELAY_CONTROL
  .setdelay     = fc7300_fcspi_setdelay,
#endif
#ifdef CONFIG_SPI_HWFEATURES
  .hwfeatures   = fc7300_fcspi_hwfeatures,
#endif
#ifdef CONFIG_SPI_CMDDATA
  .cmddata      = fc7300_fcspi4cmddata,
#endif
  .send         = fc7300_fcspi_send,
#ifdef CONFIG_SPI_EXCHANGE
  .exchange     = fc7300_fcspi_exchange,
#else
  .sndblock     = fc7300_fcspi_sndblock,
  .recvblock    = fc7300_fcspi_recvblock,
#endif
  .registercallback = NULL,  /* Not implemented */
};

static struct fc7300_fcspidev_s g_fcspi4dev =
{
  .spidev       =
  {
    .ops        = &g_spi4ops,
  },
  .spibase      = FC7300_FCSPI4_BASE,
  .modeinit_flg = SPIDEV_MODETI,
#ifdef CONFIG_FC7300_FCSPI4_PIN_SWAP
  .swap   = true,
#else
  .swap   = false,
#endif
#ifdef CONFIG_FC7300_FCSPI4_CONTEN
  .continuous   = true,
#else
  .continuous   = false,
#endif
  .csdelay      = CONFIG_FC7300_FCSPI4_CSDELAY,
#ifdef CONFIG_FC7300_FCSPI_INTERRUPTS
  .spiirq       = FC7300_IRQ_FCSPI4,
#endif
  .lock         = NXMUTEX_INITIALIZER,
#ifdef CONFIG_FC7300_FCSPI4_DMA
  .rxch         = FC7300_DMACHAN_FCSPI4_RX,
  .txch         = FC7300_DMACHAN_FCSPI4_TX,
#endif
};
#endif

#ifdef CONFIG_FC7300_FCSPI5
static const struct spi_ops_s g_spi5ops =
{
  .lock         = fc7300_fcspi_lock,
#ifdef CONFIG_FC7300_FCSPI5_HWPCS
  .select       = fc7300_fcspi_select,
#else
  .select       = fc7300_fcspi5select,
#endif
  .setfrequency = fc7300_fcspi_setfrequency,
  .setmode      = fc7300_fcspi_setmode,
  .setbits      = fc7300_fcspi_setbits,
#ifdef CONFIG_SPI_DELAY_CONTROL
  .setdelay     = fc7300_fcspi_setdelay,
#endif
#ifdef CONFIG_SPI_HWFEATURES
  .hwfeatures   = fc7300_fcspi_hwfeatures,
#endif
#ifdef CONFIG_SPI_CMDDATA
  .cmddata      = fc7300_fcspi5cmddata,
#endif
  .send         = fc7300_fcspi_send,
#ifdef CONFIG_SPI_EXCHANGE
  .exchange     = fc7300_fcspi_exchange,
#else
  .sndblock     = fc7300_fcspi_sndblock,
  .recvblock    = fc7300_fcspi_recvblock,
#endif
  .registercallback = NULL,  /* Not implemented */
};

static struct fc7300_fcspidev_s g_fcspi5dev =
{
  .spidev       =
  {
    .ops        = &g_spi5ops,
  },
  .spibase      = FC7300_FCSPI5_BASE,
  .modeinit_flg = SPIDEV_MODETI,
#ifdef CONFIG_FC7300_FCSPI5_PIN_SWAP
  .swap   = true,
#else
  .swap   = false,
#endif
#ifdef CONFIG_FC7300_FCSPI5_CONTEN
  .continuous   = true,
#else
  .continuous   = false,
#endif
  .csdelay      = CONFIG_FC7300_FCSPI5_CSDELAY,
#ifdef CONFIG_FC7300_FCSPI_INTERRUPTS
  .spiirq       = FC7300_IRQ_FCSPI5,
#endif
  .lock         = NXMUTEX_INITIALIZER,
#ifdef CONFIG_FC7300_FCSPI5_DMA
  .rxch         = FC7300_DMACHAN_FCSPI5_RX,
  .txch         = FC7300_DMACHAN_FCSPI5_TX,
#endif
};
#endif

#ifdef CONFIG_FC7300_FCSPI6
static const struct spi_ops_s g_spi6ops =
{
  .lock         = fc7300_fcspi_lock,
#ifdef CONFIG_FC7300_FCSPI6_HWPCS
  .select       = fc7300_fcspi_select,
#else
  .select       = fc7300_fcspi6select,
#endif
  .setfrequency = fc7300_fcspi_setfrequency,
  .setmode      = fc7300_fcspi_setmode,
  .setbits      = fc7300_fcspi_setbits,
#ifdef CONFIG_SPI_DELAY_CONTROL
  .setdelay     = fc7300_fcspi_setdelay,
#endif
#ifdef CONFIG_SPI_HWFEATURES
  .hwfeatures   = fc7300_fcspi_hwfeatures,
#endif
#ifdef CONFIG_SPI_CMDDATA
  .cmddata      = fc7300_fcspi6cmddata,
#endif
  .send         = fc7300_fcspi_send,
#ifdef CONFIG_SPI_EXCHANGE
  .exchange     = fc7300_fcspi_exchange,
#else
  .sndblock     = fc7300_fcspi_sndblock,
  .recvblock    = fc7300_fcspi_recvblock,
#endif
  .registercallback = NULL,  /* Not implemented */
};

static struct fc7300_fcspidev_s g_fcspi6dev =
{
  .spidev       =
  {
    .ops        = &g_spi6ops,
  },
  .spibase      = FC7300_FCSPI6_BASE,
  .modeinit_flg = SPIDEV_MODETI,
#ifdef CONFIG_FC7300_FCSPI6_PIN_SWAP
  .swap   = true,
#else
  .swap   = false,
#endif
#ifdef CONFIG_FC7300_FCSPI6_CONTEN
  .continuous   = true,
#else
  .continuous   = false,
#endif
  .csdelay      = CONFIG_FC7300_FCSPI6_CSDELAY,
#ifdef CONFIG_FC7300_FCSPI_INTERRUPTS
  .spiirq       = FC7300_IRQ_FCSPI6,
#endif
  .lock         = NXMUTEX_INITIALIZER,
#ifdef CONFIG_FC7300_FCSPI6_DMA
  .rxch         = FC7300_DMACHAN_FCSPI6_RX,
  .txch         = FC7300_DMACHAN_FCSPI6_TX,
#endif
};
#endif

#ifdef CONFIG_FC7300_FCSPI7
static const struct spi_ops_s g_spi7ops =
{
  .lock         = fc7300_fcspi_lock,
#ifdef CONFIG_FC7300_FCSPI7_HWPCS
  .select       = fc7300_fcspi_select,
#else
  .select       = fc7300_fcspi7select,
#endif
  .setfrequency = fc7300_fcspi_setfrequency,
  .setmode      = fc7300_fcspi_setmode,
  .setbits      = fc7300_fcspi_setbits,
#ifdef CONFIG_SPI_DELAY_CONTROL
  .setdelay     = fc7300_fcspi_setdelay,
#endif
#ifdef CONFIG_SPI_HWFEATURES
  .hwfeatures   = fc7300_fcspi_hwfeatures,
#endif
#ifdef CONFIG_SPI_CMDDATA
  .cmddata      = fc7300_fcspi7cmddata,
#endif
  .send         = fc7300_fcspi_send,
#ifdef CONFIG_SPI_EXCHANGE
  .exchange     = fc7300_fcspi_exchange,
#else
  .sndblock     = fc7300_fcspi_sndblock,
  .recvblock    = fc7300_fcspi_recvblock,
#endif
  .registercallback = NULL,  /* Not implemented */
};

static struct fc7300_fcspidev_s g_fcspi7dev =
{
  .spidev       =
  {
    .ops        = &g_spi7ops,
  },
  .spibase      = FC7300_FCSPI7_BASE,
  .modeinit_flg = SPIDEV_MODETI,
#ifdef CONFIG_FC7300_FCSPI7_PIN_SWAP
  .swap   = true,
#else
  .swap   = false,
#endif
#ifdef CONFIG_FC7300_FCSPI7_CONTEN
  .continuous   = true,
#else
  .continuous   = false,
#endif
  .csdelay      = CONFIG_FC7300_FCSPI7_CSDELAY,
#ifdef CONFIG_FC7300_FCSPI_INTERRUPTS
  .spiirq       = FC7300_IRQ_FCSPI7,
#endif
  .lock         = NXMUTEX_INITIALIZER,
#ifdef CONFIG_FC7300_FCSPI7_DMA
  .rxch         = FC7300_DMACHAN_FCSPI7_RX,
  .txch         = FC7300_DMACHAN_FCSPI7_TX,
#endif
};
#endif

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: fc7300_fcspi_getreg
 *
 * Description:
 *   Get the contents of the SPI register at offset
 *
 * Input Parameters:
 *   priv   - private SPI device structure
 *   offset - offset to the register of interest
 *
 * Returned Value:
 *   The contents of the 32-bit register
 *
 ****************************************************************************/

static inline
uint32_t fc7300_fcspi_getreg32(struct fc7300_fcspidev_s *priv,
                                uint8_t offset)
{
  return getreg32(priv->spibase + offset);
}

/****************************************************************************
 * Name: fc7300_fcspi_putreg
 *
 * Description:
 *   Write a 16-bit value to the SPI register at offset
 *
 * Input Parameters:
 *   priv   - private SPI device structure
 *   offset - offset to the register of interest
 *   value  - the 32-bit value to be written
 *
 * Returned Value:
 *   The contents of the 32-bit register
 *
 ****************************************************************************/

static inline
void fc7300_fcspi_putreg32(struct fc7300_fcspidev_s *priv,
                            uint8_t offset, uint32_t value)
{
  putreg32(value, priv->spibase + offset);
}

/****************************************************************************
 * Name: fc7300_fcspi_readword
 *
 * Description:
 *   Read one word from SPI
 *
 * Input Parameters:
 *   priv - Device-specific state data
 *
 * Returned Value:
 *   word as read
 *
 ****************************************************************************/

static inline
uint32_t fc7300_fcspi_readword(struct fc7300_fcspidev_s *priv)
{
  /* Then return the received byte */

  return (uint32_t) fc7300_fcspi_getreg32(priv, FC7300_FCSPI_RX_DATA_OFFSET);
}

/****************************************************************************
 * Name: fc7300_fcspi_writeword
 *
 * Description:
 *   Write one word to SPI
 *
 * Input Parameters:
 *   priv - Device-specific state data
 *   word - word to send
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/

static inline void fc7300_fcspi_writeword(struct fc7300_fcspidev_s *priv,
                                           uint32_t word)
{
  /* Then send the word */

  fc7300_fcspi_putreg32(priv, FC7300_FCSPI_TX_DATA_OFFSET, word);
}

/****************************************************************************
 * Name: fc7300_fcspi_gettrctrlreg32
 *
 * Description:
 *   Due to hardware features, TR_CTRL can not be certainty write in, so
 *   the frame command value to be used for the transmission
 * Input Parameters:
 *   priv    - Device-specific state data
 *
 * Returned Value:
 *   uint32_t
 *
 ****************************************************************************/

static inline
uint32_t fc7300_fcspi_gettrctrlreg32(struct fc7300_fcspidev_s *priv)
{
  return priv->curframetrctrl;
}

/****************************************************************************
 * Name: fc7300_fcspi_puttrctrlreg32
 *
 * Description:
 *   Due to hardware features, TR_CTRL can not be certainty write in, so
 *   the frame command value to be used for the transmission
 * Input Parameters:
 *   priv    - Device-specific state data
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/

static inline
void fc7300_fcspi_puttrctrlreg32(struct fc7300_fcspidev_s *priv)
{
  putreg32(priv->curframetrctrl, priv->spibase + FC7300_FCSPI_TR_CTRL_OFFSET);
}

/****************************************************************************
 * Name: fc7300_fcspi_9to16bitmode
 *
 * Description:
 *   Check if the SPI is operating in more then 8 bit mode
 *   On the FC7300 the frame size can grow to 4096 bit/frame
 *
 * Input Parameters:
 *   priv     - Device-specific state data
 *
 * Returned Value:
 *   value: frame size
 *
 ****************************************************************************/

static inline uint16_t
  fc7300_fcspi_9to16bitmode(struct fc7300_fcspidev_s *priv)
{
  return ((fc7300_fcspi_gettrctrlreg32(priv) & FCSPI_TR_CTRL_FRM_SZ_MASK) + 1);
}

/****************************************************************************
 * Name: fc7300_fcspi_modifyreg
 *
 * Description:
 *   Clear and set bits in register
 *
 * Input Parameters:
 *   priv    - Device-specific state data
 *   offset  - Register offset
 *   clrbits - The bits to clear
 *   setbits - The bits to set
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/

static void fc7300_fcspi_modifyreg32(struct fc7300_fcspidev_s *priv,
                                      uint8_t offset, uint32_t clrbits,
                                      uint32_t setbits)
{
  modifyreg32(priv->spibase + offset, clrbits, setbits);
}

/****************************************************************************
 * Name: fc7300_fcspi_pckfreq
 *
 * Description:
 *   Get the peripheral clock frequency for the FCSPI peripheral
 *
 * Input Parameters:
 *   base - The base address of the FCSPI peripheral registers
 *
 * Returned Value:
 *   The frequency of the FCSPI functional input frequency
 *   (or zero on a failure)
 *
 ****************************************************************************/

static uint32_t fc7300_fcspi_pckfreq(uintptr_t base)
{
  enum pcc_clksrctype_e clkname;
  uint32_t pccclk;
  int ret;

  /* Get the PCC source clock */

  switch (base)
    {
#ifdef CONFIG_FC7300_FCSPI0
      case FC7300_FCSPI0_BASE:
        clkname = PCC_CLK_FCSPI0;
        break;
#endif
#ifdef CONFIG_FC7300_FCSPI1
      case FC7300_FCSPI1_BASE:
        clkname = PCC_CLK_FCSPI1;
        break;
#endif
#ifdef CONFIG_FC7300_FCSPI2
      case FC7300_FCSPI2_BASE:
        clkname = PCC_CLK_FCSPI2;
        break;
#endif
#ifdef CONFIG_FC7300_FCSPI3
      case FC7300_FCSPI3_BASE:
        clkname = PCC_CLK_FCSPI3;
        break;
#endif
#ifdef CONFIG_FC7300_FCSPI4
      case FC7300_FCSPI4_BASE:
        clkname = PCC_CLK_FCSPI4;
        break;
#endif
#ifdef CONFIG_FC7300_FCSPI5
      case FC7300_FCSPI5_BASE:
        clkname = PCC_CLK_FCSPI5;
        break;
#endif
#ifdef CONFIG_FC7300_FCSPI6
      case FC7300_FCSPI6_BASE:
        clkname = PCC_CLK_FCSPI6;
        break;
#endif
#ifdef CONFIG_FC7300_FCSPI7
      case FC7300_FCSPI7_BASE:
        clkname = PCC_CLK_FCSPI7;
        break;
#endif
    default:
      DEBUGPANIC();
      return -EINVAL;
    }

  ret = fc7300_get_pclkfreq(clkname, &pccclk);
  DEBUGASSERT(ret >= 0);
  if (ret < 0)
    {
      return 0;
    }

  return pccclk;
}

/****************************************************************************
 * Name: fc7300_fcspi_set_delay_scaler
 *
 * Description:
 *   SET FCSPI Delay times
 *
 * Input Parameters:
 *   priv   - Device-specific state data
 *   scaler - scaler value
 *   type   - delay time type
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/

static inline void fc7300_fcspi_set_delay_scaler(struct
                                                  fc7300_fcspidev_s *priv,
                                                  uint32_t scaler,
                                                  enum fc7300_delay_e type)
{
  switch (type)
    {
    case FCSPI_PCS_TO_SCK:
      fc7300_fcspi_modifyreg32(priv, FC7300_FCSPI_CLK_CFG_OFFSET,
                               FCSPI_CLK_CFG_PCSSCK_MASK,
                               FCSPI_CLK_CFG_PCSSCK(scaler));
      break;

    case FCSPI_LAST_SCK_TO_PCS:
      fc7300_fcspi_modifyreg32(priv, FC7300_FCSPI_CLK_CFG_OFFSET,
                               FCSPI_CLK_CFG_SCKPCS_MASK,
                               FCSPI_CLK_CFG_SCKPCS(scaler));
      break;

    case FCSPI_BETWEEN_TRANSFER:
      fc7300_fcspi_modifyreg32(priv, FC7300_FCSPI_CLK_CFG_OFFSET,
                               FCSPI_CLK_CFG_PCSPCS_MASK,
                               FCSPI_CLK_CFG_PCSPCS(scaler));
      break;
    default:
      return;
    }
}

/****************************************************************************
 * Name: fc7300_fcspi_set_delays
 *
 * Description:
 *   SET FCSPI Delay times
 *
 * Input Parameters:
 *   priv     - Device-specific state data
 *   delay_ns - delay time in nano seconds
 *   type     - delay time type
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/

static void fc7300_fcspi_set_delays(struct fc7300_fcspidev_s *priv,
                                     uint32_t delay_ns,
                                     enum fc7300_delay_e type)
{
  uint32_t inclock;
  uint64_t real_delay;
  uint32_t scaler;
  uint32_t best_scaler;
  uint32_t diff;
  uint32_t min_diff = UINT32_MAX;
  uint64_t initial_delay_ns = INIT_DELAY_NS;
  uint32_t clock_div_prescaler;
  uint32_t additional_scaler;

  /* Get the frequency of the FCSPI functional input clock */

  inclock = fc7300_fcspi_pckfreq(priv->spibase);
  DEBUGASSERT(inclock != 0);

  /* Get the pre-scaled input clock */

  clock_div_prescaler = inclock /
          (1 << ((fc7300_fcspi_gettrctrlreg32(priv) &
           FCSPI_TR_CTRL_PRESCALE_MASK) >> FCSPI_TR_CTRL_PRESCALE_SHIFT));

  /* Initialize scaler to max value to generate the max delay */

  best_scaler = 0xff;

  if (type == FCSPI_BETWEEN_TRANSFER)
    {
      /* First calculate the initial, default delay, note min delay is 2
       * clock cycles. Due to large size of * calculated values (uint64_t),
       * we need to break up the calculation into several steps to ensure
       * accurate calculated results
       */

      initial_delay_ns *= 2;
      initial_delay_ns /= clock_div_prescaler;

      additional_scaler = 1U;
    }
  else
    {
      /* First calculate the initial, default delay, min delay is 1 clock
       * cycle. Due to large size of calculated values (uint64_t), we need to
       * break up the calculation into several steps to ensure accurate
       * calculated * results.
       */

      initial_delay_ns /= clock_div_prescaler;

      additional_scaler = 0;
    }

  /* If the initial, default delay is already greater than the desired delay,
   * then set the delay to their initial value (0) and return the delay. In
   * other words, there is no way to decrease the delay value further.
   */

  if (initial_delay_ns >= delay_ns)
    {
      fc7300_fcspi_set_delay_scaler(priv, 0, type);
    }
  else
    {
      /* If min_diff = 0, the exit for loop */

      for (scaler = 0; (scaler < 256) && min_diff; scaler++)
        {
          /* Calculate the real delay value as we cycle through the scaler
           * values. Due to large size of calculated values (uint64_t),
           * we need to break up the calculation into several steps to ensure
           * accurate calculated results
           */

          real_delay  = INIT_DELAY_NS;
          real_delay *= (scaler + 1 + additional_scaler);
          real_delay /= clock_div_prescaler;

          /* calculate the delay difference based on the conditional
           * statement that states that the calculated delay must not be less
           * then the desired delay
           */

          if (real_delay >= delay_ns)
            {
              diff = real_delay - delay_ns;
              if (min_diff > diff)
                {
                  /* A better match found */

                  min_diff = diff;
                  best_scaler = scaler;
                }
            }
        }

      fc7300_fcspi_set_delay_scaler(priv, best_scaler, type);
    }
}

/****************************************************************************
 * Name: fc7300_fcspi_lock
 *
 * Description:
 *   On SPI buses where there are multiple devices, it will be necessary to
 *   lock SPI to have exclusive access to the buses for a sequence of
 *   transfers.  The bus should be locked before the chip is selected. After
 *   locking the SPI bus, the caller should then also call the setfrequency,
 *   setbits, and setmode methods to make sure that the SPI is properly
 *   configured for the device.  If the SPI bus is being shared, then it
 *   may have been left in an incompatible state.
 *
 * Input Parameters:
 *   dev  - Device-specific state data
 *   lock - true: Lock spi bus, false: unlock SPI bus
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/

static int fc7300_fcspi_lock(struct spi_dev_s *dev, bool lock)
{
  struct fc7300_fcspidev_s *priv = (struct fc7300_fcspidev_s *)dev;
  int ret;

  /* It could be that this needs to be disabled for low level debugging */

  if (lock)
    {
      ret = nxmutex_lock(&priv->lock);
    }
  else
    {
      ret = nxmutex_unlock(&priv->lock);
    }

  return ret;
}

#if defined(CONFIG_FC7300_FCSPI0_HWPCS) || \
    defined(CONFIG_FC7300_FCSPI1_HWPCS) || \
    defined(CONFIG_FC7300_FCSPI2_HWPCS) || \
    defined(CONFIG_FC7300_FCSPI3_HWPCS) || \
    defined(CONFIG_FC7300_FCSPI4_HWPCS) || \
    defined(CONFIG_FC7300_FCSPI5_HWPCS) || \
    defined(CONFIG_FC7300_FCSPI6_HWPCS) || \
    defined(CONFIG_FC7300_FCSPI7_HWPCS)
/****************************************************************************
 * Name: fc7300_fcspi_select
 *
 * Description:
 *   Change to another SPI chip select (hardware/native, not emulated with
 *   GPIO) to select another device.  The hardware itself controls when
 *   the chip select is enabled or disabled.
 *
 * Input Parameters:
 *   dev -      Device-specific state data
 *   devid -    Identifies the device to select
 *   selected - Ignored, selection is controlled by hardware
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/

static void fc7300_fcspi_select(struct spi_dev_s *dev, uint32_t devid,
                                 bool selected)
{
  struct fc7300_fcspidev_s *priv = (struct fc7300_fcspidev_s *)dev;

  /* FCSPI on FC7300 supports PCS 0-3 */

  DEBUGASSERT(SPIDEVID_INDEX(devid) <= 3);

  /* Has the Peripheral Chip Select changed? */

  if (devid != priv->pcs)
    {
      priv->curframetrctrl &= ~FCSPI_TR_CTRL_PCS_MASK;
      priv->curframetrctrl |= FCSPI_TR_CTRL_PCS(SPIDEVID_INDEX(devid));
      fc7300_fcspi_puttrctrlreg32(priv);
      priv->pcs = devid;
    }

  spiinfo("devid: %" PRId32 ", CS: hardware-controlled\n", devid);
}
#endif /* CONFIG_FC7300_FCSPI HWPCS */

/****************************************************************************
 * Name: fc7300_fcspi_setfrequency
 *
 * Description:
 *   Set the SPI frequency.
 *
 * Input Parameters:
 *   dev -       Device-specific state data
 *   frequency - The SPI frequency requested
 *
 * Returned Value:
 *   Returns the actual frequency selected
 *
 ****************************************************************************/

static uint32_t fc7300_fcspi_setfrequency(struct spi_dev_s *dev,
                                           uint32_t frequency)
{
  struct fc7300_fcspidev_s *priv = (struct fc7300_fcspidev_s *)dev;

  uint32_t men;
  uint32_t inclock;
  uint32_t prescaler;
  uint32_t best_prescaler;
  uint32_t scaler;
  uint32_t best_scaler;
  uint32_t real_frequency;
  uint32_t best_frequency;
  uint32_t diff;
  uint32_t min_diff;
  uint32_t tmpcsdelay;
  uint32_t regval;

  /* Has the FCSPI bus frequency changed? */

  if (frequency != priv->frequency)
    {
      /* Disable FCSPI if it is enabled */

      men = fc7300_fcspi_getreg32(priv, FC7300_FCSPI_CTRL_OFFSET) &
                                   FCSPI_CTRL_M_EN_MASK;
      if (men)
        {
          fc7300_fcspi_modifyreg32(priv, FC7300_FCSPI_CTRL_OFFSET,
                                    FCSPI_CTRL_M_EN_MASK, 0);
        }

      /* Get the frequency of the FCSPI functional input clock */

      inclock = fc7300_fcspi_pckfreq(priv->spibase);
      DEBUGASSERT(inclock != 0);

      min_diff       = UINT32_MAX;
      best_prescaler = 7;
      best_scaler    = 255;
      best_frequency = 0;

      for (prescaler = 0; (prescaler < 8) && min_diff; prescaler++)
        {
          for (scaler = 0; (scaler < 256) && min_diff; scaler++)
            {
              real_frequency = inclock / ((1 << prescaler) * (scaler + 2));

              /* Calculate the frequency difference based on conditional
               * statement that states that the calculated frequency must not
               * exceed desired frequency.
               */

              if (frequency >= real_frequency)
                {
                  diff = frequency - real_frequency;
                  if (min_diff > diff)
                    {
                      /* A better match found */

                      min_diff = diff;
                      best_prescaler = prescaler;
                      best_scaler = scaler;
                      best_frequency = real_frequency;
                    }
                }
            }
        }

      /* Write the best values in the CLK_CFG register */

      priv->curframetrctrl &= ~FCSPI_TR_CTRL_PRESCALE_MASK;
      priv->curframetrctrl |= FCSPI_TR_CTRL_PRESCALE(best_prescaler);
      fc7300_fcspi_puttrctrlreg32(priv);

      priv->prescale = best_prescaler;
      priv->frequency = frequency;
      priv->actual = best_frequency;

      fc7300_fcspi_set_delays(priv, INIT_DELAY_NS / best_frequency,
                                    FCSPI_PCS_TO_SCK);
      fc7300_fcspi_set_delays(priv, INIT_DELAY_NS / best_frequency,
                                    FCSPI_LAST_SCK_TO_PCS);
      tmpcsdelay = 1000000000 / inclock;
      if ((priv->csdelay / tmpcsdelay) > 1)
        {
          regval = (priv->csdelay / tmpcsdelay) - 2;
          if (regval > 0xFF)
            {
              regval = 0xFF;
            }
          fc7300_fcspi_set_delay_scaler(priv, regval, FCSPI_BETWEEN_TRANSFER);
        }
      else
        {
          fc7300_fcspi_set_delays(priv, INIT_DELAY_NS / best_frequency,
                                  FCSPI_BETWEEN_TRANSFER);
        }
      fc7300_fcspi_modifyreg32(priv, FC7300_FCSPI_CLK_CFG_OFFSET,
                                FCSPI_CLK_CFG_SCKDIV_MASK,
                                FCSPI_CLK_CFG_SCKDIV(best_scaler));

      /* Re-enable FCSPI if it was enabled previously */

      if (men)
        {
          fc7300_fcspi_modifyreg32(priv, FC7300_FCSPI_CTRL_OFFSET,
                                   FCSPI_CTRL_M_EN_MASK, FCSPI_CTRL_M_EN(1));
        }
    }

  return priv->actual;
}

#ifdef CONFIG_SPI_DELAY_CONTROL
/****************************************************************************
 * Name: fc7300_fcspi_setdelay
 *
 * Description:
 *   Set the SPI delay param.  see struct spi_sequence_s
 *
 * Input Parameters:
 *   dev  - Device-specific state data
 *   startdelay - The CS to CLK delay time in ns
 *   stopdelay - The CLK to CS delay time in ns
 *   csdelay - The CS to CS delay in ns
 *   ifdelay - unsupported
 *
 * Returned Value:
 *   none
 *
 ****************************************************************************/

static int fc7300_fcspi_setdelay(struct spi_dev_s *dev, uint32_t startdelay,
                        uint32_t stopdelay, uint32_t csdelay,
                        uint32_t ifdelay)
{
  uint32_t men;
  struct fc7300_fcspidev_s *priv = (struct fc7300_fcspidev_s *)dev;
  uint32_t inclock = fc7300_fcspi_pckfreq(priv->spibase);
  uint32_t tmpcsdelay;
  uint32_t regval;

  tmpcsdelay = 1000000000 / inclock;
  if ((csdelay / tmpcsdelay) < 2)
    {
      return ERROR;
    }
  else
    {
      regval = (csdelay / tmpcsdelay) - 2;
      if (regval > 0xFF)
        {
          regval = 0xFF;
        }
      men = fc7300_fcspi_getreg32(priv, FC7300_FCSPI_CTRL_OFFSET) &
                                      FCSPI_CTRL_M_EN_MASK;

      /* Disable FCSPI first if it was enabled previously */

      if (men)
        {
          fc7300_fcspi_modifyreg32(priv, FC7300_FCSPI_CTRL_OFFSET,
                                    FCSPI_CTRL_M_EN_MASK, 0);
        }

      fc7300_fcspi_set_delay_scaler(priv, regval, FCSPI_BETWEEN_TRANSFER);

      /* Re-enable FCSPI if it was enabled previously */

      if (men)
        {
          fc7300_fcspi_modifyreg32(priv, FC7300_FCSPI_CTRL_OFFSET,
                                    FCSPI_CTRL_M_EN_MASK, FCSPI_CTRL_M_EN(1));
        }
      return OK;
    }
}
#endif

/****************************************************************************
 * Name: fc7300_fcspi_setmode
 *
 * Description:
 *   Set the SPI mode.  see enum spi_mode_e mode for mode definitions
 *
 * Input Parameters:
 *   dev  - Device-specific state data
 *   mode - The SPI mode requested
 *
 * Returned Value:
 *   none
 *
 ****************************************************************************/

static void fc7300_fcspi_setmode(struct spi_dev_s *dev,
                                  enum spi_mode_e mode)
{
  struct fc7300_fcspidev_s *priv = (struct fc7300_fcspidev_s *)dev;
  uint32_t setbits;
  uint32_t clrbits;
  uint32_t men;

  spiinfo("mode=%d\n", mode);

  /* Has the mode changed? */

  if (priv->modeinit_flg != mode)
    {
      /* Disable FCSPI if it is enabled */

      men = fc7300_fcspi_getreg32(priv, FC7300_FCSPI_CTRL_OFFSET) &
                                   FCSPI_CTRL_M_EN_MASK;
      if (men)
        {
          fc7300_fcspi_modifyreg32(priv, FC7300_FCSPI_CTRL_OFFSET,
                                    FCSPI_CTRL_M_EN_MASK, 0);
        }

      switch (mode)
        {
          case SPIDEV_MODE0:     /* CPOL=0; CPHA=0 */
            setbits = 0;
            clrbits = FCSPI_TR_CTRL_SCK_POL(1) | FCSPI_TR_CTRL_SCK_PHA(1);
            break;

          case SPIDEV_MODE1:     /* CPOL=0; CPHA=1 */
            setbits = FCSPI_TR_CTRL_SCK_PHA(1);
            clrbits = FCSPI_TR_CTRL_SCK_POL(1);
            break;

          case SPIDEV_MODE2:     /* CPOL=1; CPHA=0 */
            setbits = FCSPI_TR_CTRL_SCK_POL(1);
            clrbits = FCSPI_TR_CTRL_SCK_PHA(1);
            break;

          case SPIDEV_MODE3:     /* CPOL=1; CPHA=1 */
            setbits = FCSPI_TR_CTRL_SCK_POL(1) | FCSPI_TR_CTRL_SCK_PHA(1);
            clrbits = 0;
            break;

          default:
            return;
        }

      if (priv->continuous)
        {
          setbits |= FCSPI_TR_CTRL_CT_EN(1);
        }

      clrbits |= FCSPI_TR_CTRL_FRM_SZ_MASK;
      setbits |= FCSPI_TR_CTRL_FRM_SZ(priv->nbits - 1);
      clrbits |= FCSPI_TR_CTRL_PRESCALE_MASK,
      setbits |= FCSPI_TR_CTRL_PRESCALE(priv->prescale);

      priv->curframetrctrl &= ~clrbits;
      priv->curframetrctrl |= setbits;
      fc7300_fcspi_puttrctrlreg32(priv);

      while ((fc7300_fcspi_getreg32(priv, FC7300_FCSPI_FIFO_STATUS_OFFSET) &
            FCSPI_FIFO_STATUS_RXCNT_MASK) != 0)
        {
          /* Flush SPI read FIFO */

          (void)fc7300_fcspi_getreg32(priv, FC7300_FCSPI_RX_DATA_OFFSET);
        }

      /* Re-enable FCSPI if it was enabled previously */

      if (men)
        {
          fc7300_fcspi_modifyreg32(priv, FC7300_FCSPI_CTRL_OFFSET,
                                   FCSPI_CTRL_M_EN_MASK, FCSPI_CTRL_M_EN(1));
        }

      priv->modeinit_flg = mode;
    }
}

/****************************************************************************
 * Name: fc7300_fcspi_setbits
 *
 * Description:
 *   Set the number of bits per word.
 *
 * Input Parameters:
 *   dev   - Device-specific state data
 *   nbits - The number of bits requested
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/

static void fc7300_fcspi_setbits(struct spi_dev_s *dev, int nbits)
{
  struct fc7300_fcspidev_s *priv = (struct fc7300_fcspidev_s *)dev;
  uint32_t men;
  uint8_t savbits = nbits;

  spiinfo("nbits=%d\n", nbits);

  /* Has the number of bits changed? */

  if (nbits != priv->nbits)
    {
      if (nbits < 2 || nbits > 4096)
        {
          return;
        }

      /* Disable FCSPI if it is enabled */

      men = fc7300_fcspi_getreg32(priv, FC7300_FCSPI_CTRL_OFFSET) &
                                   FCSPI_CTRL_M_EN_MASK;
      if (men)
        {
          fc7300_fcspi_modifyreg32(priv, FC7300_FCSPI_CTRL_OFFSET,
                                    FCSPI_CTRL_M_EN_MASK, 0);
        }

      priv->curframetrctrl &= ~FCSPI_TR_CTRL_FRM_SZ_MASK;
      priv->curframetrctrl |= FCSPI_TR_CTRL_FRM_SZ(savbits - 1);
      fc7300_fcspi_puttrctrlreg32(priv);

      /* Save the selection so that subsequent re-configurations will
       * be faster.
       */

      priv->nbits = savbits;

      /* Re-enable FCSPI if it was enabled previously */

      if (men)
        {
          fc7300_fcspi_modifyreg32(priv, FC7300_FCSPI_CTRL_OFFSET,
                                   FCSPI_CTRL_M_EN_MASK, FCSPI_CTRL_M_EN(1));
        }
    }
}

/****************************************************************************
 * Name: fc7300_fcspi_hwfeatures
 *
 * Description:
 *   Set hardware-specific feature flags.
 *
 * Input Parameters:
 *   dev      - Device-specific state data
 *   features - H/W feature flags
 *
 * Returned Value:
 *   Zero (OK) if the selected H/W features are enabled; A negated errno
 *   value if any H/W feature is not supportable.
 *
 ****************************************************************************/

#ifdef CONFIG_SPI_HWFEATURES
static int fc7300_fcspi_hwfeatures(struct spi_dev_s *dev,
                                    spi_hwfeatures_t features)
{
#ifdef CONFIG_SPI_BITORDER
  struct fc7300_fcspidev_s *priv = (struct fc7300_fcspidev_s *)dev;
  uint32_t setbits;
  uint32_t clrbits;

  spiinfo("features=%08x\n", features);

  /* Transfer data LSB first? */

  if ((features & HWFEAT_LSBFIRST) != 0)
    {
      setbits = FCSPI_TR_CTRL_LSBF_MASK;
      clrbits = 0;
    }
  else
    {
      setbits = 0;
      clrbits = FCSPI_TR_CTRL_LSBF_MASK;
    }

  priv->curframetrctrl &= ~clrbits;
  priv->curframetrctrl |= setbits;
  fc7300_fcspi_puttrctrlreg32(priv);

  /* Other H/W features are not supported */

  return ((features & ~HWFEAT_LSBFIRST) == 0) ? OK : -ENOSYS;
#else
  return -ENOSYS;
#endif
}
#endif

/****************************************************************************
 * Name: fc7300_fcspi_send
 *
 * Description:
 * Exchange one word on SPI, Not recommended, exchange interface is prefered
 *
 * Input Parameters:
 *   dev - Device-specific state data
 *   wd  - The word to send.  the size of the data is determined by the
 *         number of bits selected for the SPI interface.
 *
 * Returned Value:
 *   response
 *
 ****************************************************************************/

static uint32_t fc7300_fcspi_send(struct spi_dev_s *dev, uint32_t wd)
{
  struct fc7300_fcspidev_s *priv = (struct fc7300_fcspidev_s *)dev;
  uint32_t regval;
  uint32_t ret = OK;

  DEBUGASSERT(priv && priv->spibase);

  if ((priv->continuous) && (priv->nwords == 1))
    {
      priv->curframetrctrl &= ~FCSPI_TR_CTRL_CT_GO_MASK;
      fc7300_fcspi_puttrctrlreg32(priv);
    }

  if ((FCSPI_GET_STATUS(priv) & FCSPI_STATUS_TX_FF_MASK) != 0)
    {
      fc7300_fcspi_writeword(priv, wd);
    }

  if ((FCSPI_GET_STATUS(priv) & FCSPI_STATUS_RX_FF_MASK) != 0)
    {
      ret = fc7300_fcspi_readword(priv);
    }

  /* Check and clear any error flags
   * (Reading from the STATUS clears the error flags).
   */

  regval = FCSPI_GET_STATUS(priv);

  spiinfo("Sent: %04" PRIx32 " Return: %04" PRIx32
          " Status: %02" PRIx32 "\n", wd, ret, regval);

  UNUSED(regval);
  return ret;
}

/****************************************************************************
 * Name: fc7300_fcspi_exchange (no DMA).  aka fc7300_fcspi_exchange_nodma
 *
 * Description:
 *   Exchange a block of data on SPI without using DMA
 *
 * Input Parameters:
 *   dev      - Device-specific state data
 *   txbuffer - A pointer to the buffer of data to be sent
 *   rxbuffer - A pointer to a buffer in which to receive data
 *   nwords   - the length of data to be exchanged in units of words.
 *              The wordsize is determined by the number of bits-per-word
 *              selected for the SPI interface.  If nbits <= 8, the data is
 *              packed into uint8_t's; if nbits >8, the data is packed into
 *              uint16_t's
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/

#if !defined(CONFIG_FC7300_FCSPI_DMA)

static void fc7300_fcspi_exchange(struct spi_dev_s *dev,
                                   const void *txbuffer,
                                   void *rxbuffer,
                                   size_t nwords)
#else
static void fc7300_fcspi_exchange_nodma(struct spi_dev_s *dev,
                                         const void *txbuffer,
                                         void *rxbuffer, size_t nwords)
#endif
{
  struct fc7300_fcspidev_s *priv = (struct fc7300_fcspidev_s *)dev;
  uint16_t framesize;
  uint8_t txnwords;

  DEBUGASSERT(priv && priv->spibase);

  spiinfo("txbuffer=%p rxbuffer=%p nwords=%d\n", txbuffer, rxbuffer, nwords);

  priv->nwords = nwords;
  txnwords = priv->nwords;

  if (priv->continuous)
    {
      priv->curframetrctrl |= (FCSPI_TR_CTRL_CT_GO(1) | FCSPI_TR_CTRL_CT_EN(1));
      fc7300_fcspi_puttrctrlreg32(priv);
    }

  /* bit mode */

  framesize = fc7300_fcspi_9to16bitmode(priv);

  if (framesize > 16)
    {
      /* 32-bit or 64 bit, word size memory transfers */

      const uint32_t *src = txbuffer;
      uint32_t *dest = rxbuffer;
      uint32_t word = 0x0;

      while (priv->nwords > 0)
        {
          if (((FCSPI_GET_STATUS(priv) & FCSPI_STATUS_TX_FF_MASK) != 0) &&
             (txnwords > 0))
            {
              /* Get the next word to write.  Is there a source buffer? */

              if (src)
                {
                  /* read the required number of bytes */
#ifdef CONFIG_FC7300_FCSPI_BYTESWAP
                  word = bswap_32(*src);
#else
                  word = (*src);
#endif
                  src ++;
                }
              else
                {
                  word = UINT32_MAX;
                }

              fc7300_fcspi_writeword(priv, (uint32_t)word);
              txnwords--;
              if ((txnwords == 0) && (priv->continuous))
                {
                  priv->curframetrctrl &= ~(FCSPI_TR_CTRL_CT_EN_MASK |
                                            FCSPI_TR_CTRL_CT_GO_MASK);
                  fc7300_fcspi_puttrctrlreg32(priv);
                }
            }

          if (((fc7300_fcspi_getreg32(priv, FC7300_FCSPI_FIFO_STATUS_OFFSET) &
                FCSPI_FIFO_STATUS_RXCNT_MASK) != 0) && (priv->nwords) > 0)
            {
              word = fc7300_fcspi_readword(priv);
              priv->nwords--;

              /* Check and clear any error flags
               * (Reading from the STATUS clears the error flags).
               */

              (void)FCSPI_GET_STATUS(priv);

              /* Is there a buffer to receive the return value? */

              if (dest)
                {
#ifdef CONFIG_FC7300_FCSPI_BYTESWAP
                  *dest = bswap_32(word);
#else
                  *dest = (word);
#endif
                  dest ++;
                }
            }
        }
    }
  else if (framesize > 8)
    {
      /* 16-bit mode */

      const uint16_t *src = txbuffer;
      uint16_t *dest = rxbuffer;
      uint16_t word;

      while (priv->nwords > 0)
        {
          if (((FCSPI_GET_STATUS(priv) & FCSPI_STATUS_TX_FF_MASK) != 0) &&
             (txnwords > 0))
            {
              /* Get the next word to write.  Is there a source buffer? */

              if (src)
                {
                  /* read the required number of bytes */
#ifdef CONFIG_FC7300_FCSPI_BYTESWAP
                  word = swap16(*src++);
#else
                  word = (*src++);
#endif
                }
              else
                {
                  word = 0xffff;
                }

              fc7300_fcspi_writeword(priv, (uint32_t)word);
              txnwords--;
              if ((txnwords == 0) && (priv->continuous))
                {
                  priv->curframetrctrl &= ~(FCSPI_TR_CTRL_CT_EN_MASK |
                                            FCSPI_TR_CTRL_CT_GO_MASK);
                  fc7300_fcspi_puttrctrlreg32(priv);
                }
            }

          if (((fc7300_fcspi_getreg32(priv, FC7300_FCSPI_FIFO_STATUS_OFFSET) &
                FCSPI_FIFO_STATUS_RXCNT_MASK) != 0) && (priv->nwords) > 0)
            {
              word = fc7300_fcspi_readword(priv);
              priv->nwords--;

              /* Check and clear any error flags
               * (Reading from the STATUS clears the error flags).
               */

              (void)FCSPI_GET_STATUS(priv);

              /* Is there a buffer to receive the return value? */

              if (dest)
                {
#ifdef CONFIG_FC7300_FCSPI_BYTESWAP
                  *dest++ = swap16(word);
#else
                  *dest++ = (word);
#endif
                }
            }
        }
    }
  else
    {
      /* 8-bit mode */

      const uint8_t *src = txbuffer;
      uint8_t *dest = rxbuffer;
      uint8_t word;

      while (priv->nwords > 0)
        {
          /* Exchange one word */

          if (((FCSPI_GET_STATUS(priv) & FCSPI_STATUS_TX_FF_MASK) != 0) &&
             (txnwords > 0))
            {
              /* Get the next word to write.  Is there a source buffer? */

              if (src)
                {
                  word = *src++;
                }
              else
                {
                  word = 0xff;
                }

              fc7300_fcspi_writeword(priv, (uint32_t)word);
              txnwords--;
              if ((txnwords == 0) && (priv->continuous))
                {
                  priv->curframetrctrl &= ~(FCSPI_TR_CTRL_CT_EN_MASK |
                                            FCSPI_TR_CTRL_CT_GO_MASK);
                  fc7300_fcspi_puttrctrlreg32(priv);
                }
            }

          if (((fc7300_fcspi_getreg32(priv, FC7300_FCSPI_FIFO_STATUS_OFFSET) &
                FCSPI_FIFO_STATUS_RXCNT_MASK) != 0) && (priv->nwords) > 0)
            {
              word = fc7300_fcspi_readword(priv);
              priv->nwords--;

              /* Check and clear any error flags
               * (Reading from the STATUS clears the error flags).
               */

              (void)FCSPI_GET_STATUS(priv);

              if (dest)
                {
                  *dest++ = word;
                }
            }
        }
    }
}

/****************************************************************************
 * Name: spi_exchange (with DMA capability)
 *
 * Description:
 *   Exchange a block of data on SPI using DMA
 *
 * Input Parameters:
 *   dev      - Device-specific state data
 *   txbuffer - A pointer to the buffer of data to be sent
 *   rxbuffer - A pointer to a buffer in which to receive data
 *   nwords   - the length of data to be exchanged in units of words.
 *              The wordsize is determined by the number of bits-per-word
 *              selected for the SPI interface.  If nbits <= 8, the data is
 *              packed into uint8_t's; if nbits > 8, the data is packed into
 *              uint16_t's
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/

#ifdef CONFIG_FC7300_FCSPI_DMA
static void fc7300_fcspi_exchange(struct spi_dev_s *dev,
                                   const void *txbuffer, void *rxbuffer,
                                   size_t nwords)
{
  int                       ret;
  size_t                    adjust;
  ssize_t                   nbytes;
  static uint8_t            rxdummy[4] aligned_data(4);
  static const uint16_t     txdummy = 0xffff;
  uint32_t                  regval;
  struct fc7300_fcspidev_s *priv = (struct fc7300_fcspidev_s *)dev;

  DEBUGASSERT(priv != NULL);
  DEBUGASSERT(priv && priv->spibase);
  spiinfo("txbuffer=%p rxbuffer=%p nwords=%d\n", txbuffer, rxbuffer, nwords);

  /* Convert the number of word to a number of bytes */

  nbytes = (priv->nbits > 8) ? nwords << 2 : nwords;

  /* Invalid DMA channels fall back to non-DMA method. */

  if (priv->rxdma == NULL || priv->txdma == NULL
#ifdef CONFIG_FC7300_FCSPI_DMATHRESHOLD
      /* If this is a small SPI transfer, then let
       * fc7300_fcspi_exchange_nodma() do the work.
       */

      || nbytes <= CONFIG_FC7300_FCSPI_DMATHRESHOLD
#endif
      )
    {
      fc7300_fcspi_exchange_nodma(dev, txbuffer, rxbuffer, nwords);
      return;
    }

  /* ERR050456 workaround: Reset FIFOs using CTRL[RST] bit */

  regval = fc7300_fcspi_getreg32(priv, FC7300_FCSPI_CFG1_OFFSET);

  fc7300_fcspi_modifyreg32(priv, FC7300_FCSPI_CTRL_OFFSET,
                            FCSPI_CTRL_RST_TF(1) | FCSPI_CTRL_RST_RF(1),
                            FCSPI_CTRL_RST_TF(1) | FCSPI_CTRL_RST_RF(1));

  fc7300_fcspi_putreg32(priv, FC7300_FCSPI_CFG1_OFFSET, regval);

  /* Clear all status bits */

  fc7300_fcspi_putreg32(priv, FC7300_FCSPI_STATUS_OFFSET, SPI_SR_CLEAR);

  /* disable DMA */

  fc7300_fcspi_putreg32(priv, FC7300_FCSPI_DMA_EN_OFFSET, 0);

  /* Set up the DMA */

  adjust = (priv->nbits > 8) ? 2 : 1;

  struct fc7300_dma_xfrconfig_s config;

  config.saddr  = priv->spibase + FC7300_FCSPI_RX_DATA_OFFSET;
  config.daddr  = (uint32_t)(rxbuffer ? rxbuffer : rxdummy);
  config.soff   = 0;
  config.doff   = rxbuffer ? adjust : 0;
  config.iter   = nbytes;
  config.flags  = DMA_CONFIG_LINKTYPE_LINKNONE;
  config.ssize  = adjust == 1 ? DMA_8BIT : DMA_16BIT;
  config.dsize  = adjust == 1 ? DMA_8BIT : DMA_16BIT;
  config.nbytes = adjust;
#ifdef CONFIG_KINETIS_DMA_ELINK
  config.linkch = NULL;
#endif
  fc7300_dmach_xfrsetup(priv->rxdma, &config);

  config.saddr  = (uint32_t)(txbuffer ? txbuffer : &txdummy);
  config.daddr  = priv->spibase + FC7300_FCSPI_TX_DATA_OFFSET;
  config.soff   = txbuffer ? adjust : 0;
  config.doff   = 0;
  config.iter   = nbytes;
  config.flags  = DMA_CONFIG_LINKTYPE_LINKNONE;
  config.ssize  = adjust == 1 ? DMA_8BIT : DMA_16BIT;
  config.dsize  = adjust == 1 ? DMA_8BIT : DMA_16BIT;
  config.nbytes = adjust;
#ifdef CONFIG_KINETIS_DMA_ELINK
  config.linkch = NULL;
#endif
  fc7300_dmach_xfrsetup(priv->txdma, &config);

  /* Start the DMAs */

  spi_dmarxstart(priv);
  spi_dmatxstart(priv);

  /* Invoke SPI DMA */

  fc7300_fcspi_modifyreg32(priv, FC7300_FCSPI_DMA_EN_OFFSET,
                            0, FCSPI_DMA_EN_TFDE | FCSPI_DMA_EN_RFDE);

  /* Then wait for each to complete */

  ret = spi_dmatxwait(priv);

  if (ret < 0)
    {
      ret = spi_dmarxwait(priv);
    }

  /* Reset any status */

  fc7300_fcspi_putreg32(priv, FC7300_FCSPI_STATUS_OFFSET,
                         fc7300_fcspi_getreg32(priv,
                                                FC7300_FCSPI_STATUS_OFFSET));

  /* Disable DMA */

  fc7300_fcspi_putreg32(priv, FC7300_FCSPI_DMA_EN_OFFSET, 0);

  up_invalidate_dcache((uintptr_t)rxbuffer,
                       (uintptr_t)rxbuffer + nbytes);
}

#endif  /* CONFIG_FC7300_SPI_DMA */

/****************************************************************************
 * Name: fc7300_fcspi_sndblock
 *
 * Description:
 *   Send a block of data on SPI
 *
 * Input Parameters:
 *   dev      - Device-specific state data
 *   txbuffer - A pointer to the buffer of data to be sent
 *   nwords   - the length of data to send from the buffer in number of
 *              words. The wordsize is determined by the number of
 *              bits-per-word selected for the SPI interface.  If nbits <= 8,
 *              the data is packed into uint8_t's; if nbits >8, the data is
 *              packed into uint16_t's
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/

#ifndef CONFIG_SPI_EXCHANGE
static void fc7300_fcspi_sndblock(struct spi_dev_s *dev,
                                   const void *txbuffer, size_t nwords)
{
  spiinfo("txbuffer=%p nwords=%d\n", txbuffer, nwords);
  return fc7300_fcspi_exchange(dev, txbuffer, NULL, nwords);
}
#endif

/****************************************************************************
 * Name: fc7300_fcspi_recvblock
 *
 * Description:
 *   Receive a block of data from SPI
 *
 * Input Parameters:
 *   dev      - Device-specific state data
 *   rxbuffer - A pointer to the buffer in which to receive data
 *   nwords   - the length of data that can be received in the buffer in
 *              number of words.  The wordsize is determined by the number
 *              of bits-per-word selected for the SPI interface.  If
 *              nbits <= 8, the data is packed into uint8_t's; if nbits >8,
 *              the data is packed into uint16_t's
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/

#ifndef CONFIG_SPI_EXCHANGE
static void fc7300_fcspi_recvblock(struct spi_dev_s *dev,
                                    void *rxbuffer,
                                    size_t nwords)
{
  spiinfo("rxbuffer=%p nwords=%d\n", rxbuffer, nwords);
  return fc7300_fcspi_exchange(dev, NULL, rxbuffer, nwords);
}
#endif

/****************************************************************************
 * Name: fc7300_fcspi_bus_initialize
 *
 * Description:
 *   Initialize the selected SPI bus in its default state (Master, 8-bit,
 *   mode 0, etc.)
 *
 * Input Parameters:
 *   priv   - private SPI device structure
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/

static void fc7300_fcspi_bus_initialize(struct fc7300_fcspidev_s *priv)
{
  /* NOTE:
   * Clocking to the FCSPI peripheral must be provided by board-specific
   * logic as part of the clock configuration logic.
   */

  /* Reset to known status */

  fc7300_fcspi_modifyreg32(priv, FC7300_FCSPI_CTRL_OFFSET, 0,
                           FCSPI_CTRL_SW_RST(1));
  fc7300_fcspi_modifyreg32(priv, FC7300_FCSPI_CTRL_OFFSET, 0,
                           FCSPI_CTRL_RST_TF(1) | FCSPI_CTRL_RST_RF(1));
  fc7300_fcspi_putreg32(priv, FC7300_FCSPI_CTRL_OFFSET, 0x00);

  /* Always set spi debug mode */

  fc7300_fcspi_modifyreg32(priv, FC7300_FCSPI_CTRL_OFFSET, 0,
                          FCSPI_CTRL_DBG_EN(1));

  /* Set FCSPI to master */

  fc7300_fcspi_modifyreg32(priv, FC7300_FCSPI_CFG1_OFFSET, 0,
                          FCSPI_CFG1_MASTER(1));

  /* Set FCSPI swap function if neccessary */

  if (priv->swap)
    {
      fc7300_fcspi_modifyreg32(priv, FC7300_FCSPI_CFG1_OFFSET, 0,
                              FCSPI_CFG1_PIN_CFG(3));
    }
  else
    {
      fc7300_fcspi_modifyreg32(priv, FC7300_FCSPI_CFG1_OFFSET, 0,
                              FCSPI_CFG1_PIN_CFG(0));
    }

  /* Set specific PCS to active high or low */

  /* TODO: Not needed for now */

  /* Set watermark */

  fc7300_fcspi_modifyreg32(priv, FC7300_FCSPI_FIFO_WTM_OFFSET,
                (FCSPI_FIFO_WTM_RXWATER_MASK | FCSPI_FIFO_WTM_TXWATER_MASK),
                (FCSPI_FIFO_WTM_RXWATER(0) | FCSPI_FIFO_WTM_TXWATER(2)));

  /* Set Configuration Register 1 related setting. */

  fc7300_fcspi_modifyreg32(priv, FC7300_FCSPI_CFG1_OFFSET,
                           FCSPI_CFG1_OUT_CFG_MASK,
                           FCSPI_CFG1_SCK_LB(1));

  /* Set frequency and delay times */

  fc7300_fcspi_setfrequency((struct spi_dev_s *)priv, 400000);

  /* Set Transmit Command Register */

  fc7300_fcspi_setbits((struct spi_dev_s *)priv, 8);

  fc7300_fcspi_setmode((struct spi_dev_s *)priv, SPIDEV_MODE0);

  /* Enable FCSPI */

  fc7300_fcspi_modifyreg32(priv, FC7300_FCSPI_CTRL_OFFSET, 0,
                           FCSPI_CTRL_M_EN(1));
}

/****************************************************************************
 * Name: spi_dmarxwait
 *
 * Description:
 *   Wait for DMA to complete.
 *
 ****************************************************************************/

#ifdef CONFIG_FC7300_FCSPI_DMA
static int spi_dmarxwait(struct fc7300_fcspidev_s *priv)
{
  int ret;

  /* Take the semaphore (perhaps waiting).  If the result is zero, then the
   *  DMA must not really have completed.
   */

  do
    {
      ret = nxsem_wait_uninterruptible(&priv->rxsem);

      /* The only expected error is ECANCELED which would occur if the
       * calling thread were canceled.
       */

      DEBUGASSERT(ret == OK || ret == -ECANCELED);
    }
  while (priv->rxresult == 0 && ret == OK);

  return ret;
}
#endif

/****************************************************************************
 * Name: spi_dmatxwait
 *
 * Description:
 *   Wait for DMA to complete.
 *
 ****************************************************************************/

#ifdef CONFIG_FC7300_FCSPI_DMA
static int spi_dmatxwait(struct fc7300_fcspidev_s *priv)
{
  int ret;

  DEBUGASSERT(priv != NULL);

  /* Take the semaphore (perhaps waiting).  If the result is zero, then the
   * DMA must not really have completed.
   */

  do
    {
      ret = nxsem_wait_uninterruptible(&priv->txsem);

      /* The only expected error is ECANCELED which would occur if the
       * calling thread were canceled.
       */

      DEBUGASSERT(ret == OK || ret == -ECANCELED);
    }
  while (priv->txresult == 0 && ret == OK);

  return ret;
}
#endif

/****************************************************************************
 * Name: spi_dmarxwakeup
 *
 * Description:
 *   Signal that DMA is complete
 *
 ****************************************************************************/

#ifdef CONFIG_FC7300_FCSPI_DMA
static inline void spi_dmarxwakeup(struct fc7300_fcspidev_s *priv)
{
  DEBUGASSERT(priv != NULL);
  nxsem_post(&priv->rxsem);
}
#endif

/****************************************************************************
 * Name: spi_dmatxwakeup
 *
 * Description:
 *   Signal that DMA is complete
 *
 ****************************************************************************/

#ifdef CONFIG_FC7300_FCSPI_DMA
static inline void spi_dmatxwakeup(struct fc7300_fcspidev_s *priv)
{
  DEBUGASSERT(priv != NULL);
  nxsem_post(&priv->txsem);
}
#endif

/****************************************************************************
 * Name: spi_dmarxcallback
 *
 * Description:
 *   Called when the RX DMA completes
 *
 ****************************************************************************/

#ifdef CONFIG_FC7300_FCSPI_DMA
static void spi_dmarxcallback(DMACH_HANDLE handle, void *arg, bool done,
                              int result)
{
  struct fc7300_fcspidev_s *priv = (struct fc7300_fcspidev_s *)arg;

  priv->rxresult = result | 0x80000000;  /* assure non-zero */
  spi_dmarxwakeup(priv);
}
#endif

/****************************************************************************
 * Name: spi_dmatxcallback
 *
 * Description:
 *   Called when the RX DMA completes
 *
 ****************************************************************************/

#ifdef CONFIG_FC7300_FCSPI_DMA
static void spi_dmatxcallback(DMACH_HANDLE handle, void *arg, bool done,
                              int result)
{
  struct fc7300_fcspidev_s *priv = (struct fc7300_fcspidev_s *)arg;

  /* Wake-up the SPI driver */

  priv->txresult = result | 0x80000000;  /* assure non-zero */
  spi_dmatxwakeup(priv);
}
#endif

/****************************************************************************
 * Name: spi_dmarxstart
 *
 * Description:
 *   Start RX DMA
 *
 ****************************************************************************/

#ifdef CONFIG_FC7300_FCSPI_DMA
static inline void spi_dmarxstart(struct fc7300_fcspidev_s *priv)
{
  DEBUGASSERT(priv != NULL);
  priv->rxresult = 0;
  fc7300_dmach_start(priv->rxdma, spi_dmarxcallback, priv);
}
#endif

/****************************************************************************
 * Name: spi_dmatxstart
 *
 * Description:
 *   Start TX DMA
 *
 ****************************************************************************/

#ifdef CONFIG_FC7300_FCSPI_DMA
static inline void spi_dmatxstart(struct fc7300_fcspidev_s *priv)
{
  DEBUGASSERT(priv != NULL);
  priv->txresult = 0;
  fc7300_dmach_start(priv->txdma, spi_dmatxcallback, priv);
}
#endif

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: fc7300_fcspibus_initialize
 *
 * Description:
 *   Initialize the selected SPI bus
 *
 * Input Parameters:
 *   Port number (for hardware that has multiple SPI interfaces)
 *
 * Returned Value:
 *   Valid SPI device structure reference on success; a NULL on failure
 *
 ****************************************************************************/

struct spi_dev_s *fc7300_fcspibus_initialize(int bus)
{
  struct fc7300_fcspidev_s *priv = NULL;

  irqstate_t flags = enter_critical_section();

  switch (bus)
    {
#ifdef CONFIG_FC7300_FCSPI0
      case 0:
        {
          /* Select SPI0 */

          priv = &g_fcspi0dev;

          /* Only configure if the bus is not already configured */

          if ((fc7300_fcspi_getreg32(priv, FC7300_FCSPI_CTRL_OFFSET) &
              FCSPI_CTRL_M_EN(1)) == 0)
            {
              /* Configure SPI0 pins: SCK, MISO, and MOSI */

              fc7300_pinconfig(PIN_FCSPI0_SCK);
              fc7300_pinconfig(PIN_FCSPI0_MISO);
              fc7300_pinconfig(PIN_FCSPI0_MOSI);

              /* Set up default configuration: Master, 8-bit, etc. */

              fc7300_fcspi_bus_initialize(priv);
            }
          break;
        }
#endif /* CONFIG_FC7300_FCSPI0 */

#ifdef CONFIG_FC7300_FCSPI1
      case 1:
        {
          /* Select SPI1 */

          priv = &g_fcspi1dev;

          /* Only configure if the bus is not already configured */

          if ((fc7300_fcspi_getreg32(priv, FC7300_FCSPI_CTRL_OFFSET) &
              FCSPI_CTRL_M_EN(1)) == 0)
            {
              /* Configure SPI1 pins: SCK, MISO, and MOSI */

              fc7300_pinconfig(PIN_FCSPI1_SCK);
              fc7300_pinconfig(PIN_FCSPI1_MISO);
              fc7300_pinconfig(PIN_FCSPI1_MOSI);

              /* Set up default configuration: Master, 8-bit, etc. */

              fc7300_fcspi_bus_initialize(priv);
            }
          break;
        }
#endif /* CONFIG_FC7300_FCSPI1 */

#ifdef CONFIG_FC7300_FCSPI2
      case 2:
        {
          /* Select SPI2 */

          priv = &g_fcspi2dev;

          /* Only configure if the bus is not already configured */

          if ((fc7300_fcspi_getreg32(priv, FC7300_FCSPI_CTRL_OFFSET) &
              FCSPI_CTRL_M_EN(1)) == 0)
            {
              /* Configure SPI2 pins: SCK, MISO, and MOSI */

              fc7300_pinconfig(PIN_FCSPI2_SCK);
              fc7300_pinconfig(PIN_FCSPI2_MISO);
              fc7300_pinconfig(PIN_FCSPI2_MOSI);

              /* Set up default configuration: Master, 8-bit, etc. */

              fc7300_fcspi_bus_initialize(priv);
            }
          break;
        }
#endif /* CONFIG_FC7300_FCSPI2 */

#ifdef CONFIG_FC7300_FCSPI3
      case 3:
        {
          /* Select SPI3 */

          priv = &g_fcspi3dev;

          /* Only configure if the bus is not already configured */

          if ((fc7300_fcspi_getreg32(priv, FC7300_FCSPI_CTRL_OFFSET) &
              FCSPI_CTRL_M_EN(1)) == 0)
            {
              /* Configure SPI3 pins: SCK, MISO, and MOSI */

              fc7300_pinconfig(PIN_FCSPI3_SCK);
              fc7300_pinconfig(PIN_FCSPI3_MISO);
              fc7300_pinconfig(PIN_FCSPI3_MOSI);

              /* Set up default configuration: Master, 8-bit, etc. */

              fc7300_fcspi_bus_initialize(priv);
            }
          break;
        }
#endif /* CONFIG_FC7300_FCSPI3 */

#ifdef CONFIG_FC7300_FCSPI4
      case 4:
        {
          /* Select SPI4 */

          priv = &g_fcspi4dev;

          /* Only configure if the bus is not already configured */

          if ((fc7300_fcspi_getreg32(priv, FC7300_FCSPI_CTRL_OFFSET) &
              FCSPI_CTRL_M_EN(1)) == 0)
            {
              /* Configure SPI3 pins: SCK, MISO, and MOSI */

              fc7300_pinconfig(PIN_FCSPI4_SCK);
              fc7300_pinconfig(PIN_FCSPI4_MISO);
              fc7300_pinconfig(PIN_FCSPI4_MOSI);

              /* Set up default configuration: Master, 8-bit, etc. */

              fc7300_fcspi_bus_initialize(priv);
            }
          break;
        }
#endif /* CONFIG_FC7300_FCSPI4 */

#ifdef CONFIG_FC7300_FCSPI5
      case 5:
        {
          /* Select SPI5 */

          priv = &g_fcspi5dev;

          /* Only configure if the bus is not already configured */

          if ((fc7300_fcspi_getreg32(priv, FC7300_FCSPI_CTRL_OFFSET) &
              FCSPI_CTRL_M_EN(1)) == 0)
            {
              /* Configure SPI3 pins: SCK, MISO, and MOSI */

              fc7300_pinconfig(PIN_FCSPI5_SCK);
              fc7300_pinconfig(PIN_FCSPI5_MISO);
              fc7300_pinconfig(PIN_FCSPI5_MOSI);

              /* Set up default configuration: Master, 8-bit, etc. */

              fc7300_fcspi_bus_initialize(priv);
            }
          break;
        }
#endif /* CONFIG_FC7300_FCSPI5 */

#ifdef CONFIG_FC7300_FCSPI6
      case 6:
        {
          /* Select SPI6 */

          priv = &g_fcspi6dev;

          /* Only configure if the bus is not already configured */

          if ((fc7300_fcspi_getreg32(priv, FC7300_FCSPI_CTRL_OFFSET) &
              FCSPI_CTRL_M_EN(1)) == 0)
            {
              /* Configure SPI3 pins: SCK, MISO, and MOSI */

              fc7300_pinconfig(PIN_FCSPI6_SCK);
              fc7300_pinconfig(PIN_FCSPI6_MISO);
              fc7300_pinconfig(PIN_FCSPI6_MOSI);

              /* Set up default configuration: Master, 8-bit, etc. */

              fc7300_fcspi_bus_initialize(priv);
            }
          break;
        }
#endif /* CONFIG_FC7300_FCSPI6 */

#ifdef CONFIG_FC7300_FCSPI7
      case 7:
        {
          /* Select SPI7 */

          priv = &g_fcspi7dev;

          /* Only configure if the bus is not already configured */

          if ((fc7300_fcspi_getreg32(priv, FC7300_FCSPI_CTRL_OFFSET) &
              FCSPI_CTRL_M_EN(1)) == 0)
            {
              /* Configure SPI3 pins: SCK, MISO, and MOSI */

              fc7300_pinconfig(PIN_FCSPI7_SCK);
              fc7300_pinconfig(PIN_FCSPI7_MISO);
              fc7300_pinconfig(PIN_FCSPI7_MOSI);

              /* Set up default configuration: Master, 8-bit, etc. */

              fc7300_fcspi_bus_initialize(priv);
            }
          break;
        }
#endif /* CONFIG_FC7300_FCSPI7 */

      default:
        {
          spierr("ERROR: Unsupported SPI bus: %d\n", bus);
          leave_critical_section(flags);
          return NULL;
        }
    }

#ifdef CONFIG_FC7300_FCSPI_DMA
  /* Initialize the SPI semaphores that is used to wait for DMA completion.
   * This semaphore is used for signaling and, hence, should not have
   * priority inheritance enabled.
   */

  if (priv->rxch && priv->txch)
    {
      if (priv->txdma == NULL && priv->rxdma == NULL)
        {
          nxsem_init(&priv->rxsem, 0, 0);
          nxsem_init(&priv->txsem, 0, 0);

          priv->txdma = fc7300_dmach_alloc(priv->txch | DMAMUX_CHCFG_ENBL,
                                            0);
          priv->rxdma = fc7300_dmach_alloc(priv->rxch | DMAMUX_CHCFG_ENBL,
                                            0);
          DEBUGASSERT(priv->rxdma && priv->txdma);
        }
    }
  else
    {
      priv->rxdma = NULL;
      priv->txdma = NULL;
    }
#endif

  leave_critical_section(flags);

  return (struct spi_dev_s *)priv;
}

#endif /* CONFIG_FC7300_FCSPI0 || CONFIG_FC7300_FCSPI1 ||
        * CONFIG_FC7300_FCSPI2 || CONFIG_FC7300_FCSPI3 ||
        * CONFIG_FC7300_FCSPI4 || CONFIG_FC7300_FCSPI5 ||
        * CONFIG_FC7300_FCSPI6 || CONFIG_FC7300_FCSPI7 */
