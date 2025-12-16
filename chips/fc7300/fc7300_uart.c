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

#ifdef CONFIG_SERIAL_TERMIOS
#  include <termios.h>
#endif

#include <nuttx/irq.h>
#include <nuttx/arch.h>
#include <nuttx/compiler.h>
#include <nuttx/spinlock.h>
#include <nuttx/init.h>
#include <nuttx/power/pm.h>
#include <nuttx/fs/ioctl.h>
#include <nuttx/serial/serial.h>

#include <arch/board/board.h>

#include "chip.h"
#include "hardware/fc7300_pin.h"
#include "hardware/fc7300_pinmux.h"
#include "hardware/fc7300_fcuart.h"
#include "fc7300_dma.h"
#include "hardware/fc7300_dmamux.h"
#include "fc7300_lowputc.h"
#include "fc7300_uart.h"
#include "fc7300_irq.h"

#if defined(CONFIG_FC7300_FCUART)

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Pick the console. */

#if defined(CONFIG_FCUART0_SERIAL_CONSOLE)
#  define CONSOLE_DEV         g_fcuart0dev /* FCUART0 is console */
#  if defined(CONFIG_FCUART0_RXDMA)
#    define SERIAL_HAVE_CONSOLE_RXDMA 1
#  endif
#  if defined(CONFIG_FCUART0_TXDMA)
#    define SERIAL_HAVE_CONSOLE_TXDMA 1
#  endif
#elif defined(CONFIG_FCUART1_SERIAL_CONSOLE)
#  define CONSOLE_DEV         g_fcuart1dev /* FCUART1 is console */
#  if defined(CONFIG_FCUART1_RXDMA)
#    define SERIAL_HAVE_CONSOLE_RXDMA 1
#  endif
#  if defined(CONFIG_FCUART1_TXDMA)
#    define SERIAL_HAVE_CONSOLE_TXDMA 1
#  endif
#elif defined(CONFIG_FCUART2_SERIAL_CONSOLE)
#  define CONSOLE_DEV         g_fcuart2dev /* FCUART2 is console */
#  if defined(CONFIG_FCUART2_RXDMA)
#    define SERIAL_HAVE_CONSOLE_RXDMA 1
#  endif
#  if defined(CONFIG_FCUART2_TXDMA)
#    define SERIAL_HAVE_CONSOLE_TXDMA 1
#  endif
#elif defined(CONFIG_FCUART3_SERIAL_CONSOLE)
#  define CONSOLE_DEV         g_fcuart3dev /* FCUART3 is console */
#  if defined(CONFIG_FCUART3_RXDMA)
#    define SERIAL_HAVE_CONSOLE_RXDMA 1
#  endif
#  if defined(CONFIG_FCUART3_TXDMA)
#    define SERIAL_HAVE_CONSOLE_TXDMA 1
#  endif
#elif defined(CONFIG_FCUART4_SERIAL_CONSOLE)
#  define CONSOLE_DEV         g_fcuart4dev /* FCUART4 is console */
#  if defined(CONFIG_FCUART4_RXDMA)
#    define SERIAL_HAVE_CONSOLE_RXDMA 1
#  endif
#  if defined(CONFIG_FCUART4_TXDMA)
#    define SERIAL_HAVE_CONSOLE_TXDMA 1
#  endif
#elif defined(CONFIG_FCUART5_SERIAL_CONSOLE)
#  define CONSOLE_DEV         g_fcuart5dev /* FCUART5 is console */
#  if defined(CONFIG_FCUART5_RXDMA)
#    define SERIAL_HAVE_CONSOLE_RXDMA 1
#  endif
#  if defined(CONFIG_FCUART5_TXDMA)
#    define SERIAL_HAVE_CONSOLE_TXDMA 1
#  endif
#elif defined(CONFIG_FCUART6_SERIAL_CONSOLE)
#  define CONSOLE_DEV         g_fcuart6dev /* FCUART6 is console */
#  if defined(CONFIG_FCUART6_RXDMA)
#    define SERIAL_HAVE_CONSOLE_RXDMA 1
#  endif
#  if defined(CONFIG_FCUART6_TXDMA)
#    define SERIAL_HAVE_CONSOLE_TXDMA 1
#  endif
#elif defined(CONFIG_FCUART7_SERIAL_CONSOLE)
#  define CONSOLE_DEV         g_fcuart7dev /* FCUART7 is console */
#  if defined(CONFIG_FCUART7_RXDMA)
#    define SERIAL_HAVE_CONSOLE_RXDMA 1
#  endif
#  if defined(CONFIG_FCUART7_TXDMA)
#    define SERIAL_HAVE_CONSOLE_TXDMA 1
#  endif
#elif defined(CONFIG_FCUART8_SERIAL_CONSOLE)
#  define CONSOLE_DEV         g_fcuart8dev /* FCUART8 is console */
#  if defined(CONFIG_FCUART8_RXDMA)
#    define SERIAL_HAVE_CONSOLE_RXDMA 1
#  endif
#  if defined(CONFIG_FCUART8_TXDMA)
#    define SERIAL_HAVE_CONSOLE_TXDMA 1
#  endif
#elif defined(CONFIG_FCUART9_SERIAL_CONSOLE)
#  define CONSOLE_DEV         g_fcuart9dev /* FCUART9 is console */
#  if defined(CONFIG_FCUART9_RXDMA)
#    define SERIAL_HAVE_CONSOLE_RXDMA 1
#  endif
#  if defined(CONFIG_FCUART9_TXDMA)
#    define SERIAL_HAVE_CONSOLE_TXDMA 1
#  endif
#elif defined(CONFIG_FCUART10_SERIAL_CONSOLE)
#  define CONSOLE_DEV         g_fcuart10dev /* FCUART10 is console */
#  if defined(CONFIG_FCUART10_RXDMA)
#    define SERIAL_HAVE_CONSOLE_RXDMA 1
#  endif
#  if defined(CONFIG_FCUART10_TXDMA)
#    define SERIAL_HAVE_CONSOLE_TXDMA 1
#  endif
#elif defined(CONFIG_FCUART11_SERIAL_CONSOLE)
#  define CONSOLE_DEV         g_fcuart11dev /* FCUART11 is console */
#  if defined(CONFIG_FCUART11_RXDMA)
#    define SERIAL_HAVE_CONSOLE_RXDMA 1
#  endif
#  if defined(CONFIG_FCUART11_TXDMA)
#    define SERIAL_HAVE_CONSOLE_TXDMA 1
#  endif
#elif defined(CONFIG_FCUART12_SERIAL_CONSOLE)
#  define CONSOLE_DEV         g_fcuart12dev /* FCUART12 is console */
#  if defined(CONFIG_FCUART12_RXDMA)
#    define SERIAL_HAVE_CONSOLE_RXDMA 1
#  endif
#  if defined(CONFIG_FCUART12_TXDMA)
#    define SERIAL_HAVE_CONSOLE_TXDMA 1
#  endif
#elif defined(CONFIG_FCUART13_SERIAL_CONSOLE)
#  define CONSOLE_DEV         g_fcuart13dev /* FCUART13 is console */
#  if defined(CONFIG_FCUART13_RXDMA)
#    define SERIAL_HAVE_CONSOLE_RXDMA 1
#  endif
#  if defined(CONFIG_FCUART13_TXDMA)
#    define SERIAL_HAVE_CONSOLE_TXDMA 1
#  endif
#elif defined(CONFIG_FCUART14_SERIAL_CONSOLE)
#  define CONSOLE_DEV         g_fcuart14dev /* FCUART14 is console */
#  if defined(CONFIG_FCUART14_RXDMA)
#    define SERIAL_HAVE_CONSOLE_RXDMA 1
#  endif
#  if defined(CONFIG_FCUART14_TXDMA)
#    define SERIAL_HAVE_CONSOLE_TXDMA 1
#  endif
#elif defined(CONFIG_FCUART15_SERIAL_CONSOLE)
#  define CONSOLE_DEV         g_fcuart15dev /* FCUART15 is console */
#  if defined(CONFIG_FCUART15_RXDMA)
#    define SERIAL_HAVE_CONSOLE_RXDMA 1
#  endif
#  if defined(CONFIG_FCUART15_TXDMA)
#    define SERIAL_HAVE_CONSOLE_TXDMA 1
#  endif
#elif defined(CONFIG_FCUART16_SERIAL_CONSOLE)
#  define CONSOLE_DEV         g_fcuart16dev /* FCUART16 is console */
#  if defined(CONFIG_FCUART16_RXDMA)
#    define SERIAL_HAVE_CONSOLE_RXDMA 1
#  endif
#  if defined(CONFIG_FCUART16_TXDMA)
#    define SERIAL_HAVE_CONSOLE_TXDMA 1
#  endif
#elif defined(CONFIG_FCUART17_SERIAL_CONSOLE)
#  define CONSOLE_DEV         g_fcuart17dev /* FCUART17 is console */
#  if defined(CONFIG_FCUART17_RXDMA)
#    define SERIAL_HAVE_CONSOLE_RXDMA 1
#  endif
#  if defined(CONFIG_FCUART17_TXDMA)
#    define SERIAL_HAVE_CONSOLE_TXDMA 1
#  endif
#else
#  ifdef CONFIG_DEV_CONSOLE
#    warning "No valid CONFIG_FCUART[n]_SERIAL_CONSOLE Setting"
#  endif
#endif

#if defined(SERIAL_HAVE_CONSOLE_RXDMA) || defined(SERIAL_HAVE_CONSOLE_TXDMA)
#  define SERIAL_HAVE_CONSOLE_DMA
#endif

#if !defined(CONFIG_FC7300_FCUART_RXDMA_BUFFER_SIZE)
#  define CONFIG_FC7300_FCUART_RXDMA_BUFFER_SIZE 32
#endif

#define RXDMA_BUFFER_SIZE   CONFIG_FC7300_FCUART_RXDMA_BUFFER_SIZE

/****************************************************************************
 * Private Types
 ****************************************************************************/

struct fc7300_uart_s
{
  uint32_t uartbase;        /* Base address of UART registers */
  uint32_t baud;            /* Configured baud */
  uint32_t ie;              /* Saved enabled interrupts */
  uint8_t  irq;             /* IRQ associated with this UART */
  uint8_t  parity;          /* 0=none, 1=odd, 2=even */
  uint8_t  bits;            /* Number of bits (7 or 8) */
  bool     stopbits2;       /* True: Configure with 2 stop bits vs 1 */
#ifdef CONFIG_SERIAL_IFLOWCONTROL
  bool     iflow;           /* input flow control (RTS) enabled */
#endif
#ifdef CONFIG_SERIAL_OFLOWCONTROL
  bool     oflow;           /* output flow control (CTS) enabled */
#endif

#ifdef CONFIG_SERIAL_IFLOWCONTROL
  const uint32_t rts_gpio;  /* U[S]ART RTS GPIO pin configuration */
#endif
#ifdef CONFIG_SERIAL_OFLOWCONTROL
  const uint32_t cts_gpio;  /* U[S]ART CTS GPIO pin configuration */
#endif

  /* TX DMA state */

#ifdef SERIAL_HAVE_TXDMA
  const unsigned int dma_txreqsrc;  /* DMAMUX source of TX DMA request */
  const unsigned int dma_txchidx;   /* DMA channel for UART TX */
  struct dma_chan_s *txdma;         /* currently-open trasnmit DMA stream */
  sem_t              txdmasem;      /* Indicate TX DMA completion */
#endif

  /* RX DMA state */

#ifdef SERIAL_HAVE_RXDMA
  const unsigned int dma_rxreqsrc;  /* DMAMUX source of RX DMA request */
  const unsigned int dma_rxchidx;   /* DMA Channel for UART RX */
  struct dma_chan_s *rxdma;         /* currently-open receive DMA stream */
  bool               rxenable;      /* DMA-based reception en/disable */
  uint32_t           rxdmanext;     /* Next byte in the DMA buffer to be read */
  char *const        rxfifo;        /* Receive DMA buffer */
#endif
};

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

static inline uint32_t fc7300_serialin(struct fc7300_uart_s *priv,
                                      uint32_t offset);
static inline void fc7300_serialout(struct fc7300_uart_s *priv,
                                   uint32_t offset, uint32_t value);

#ifdef SERIAL_HAVE_RXDMA
static int fc7300_dma_nextrx(struct fc7300_uart_s *priv);
#endif

static inline void fc7300_disableuartint(struct fc7300_uart_s *priv,
                                        uint32_t *ie);
static inline void fc7300_restoreuartint(struct fc7300_uart_s *priv,
                                        uint32_t ie);
static int  fc7300_fcuart_configure(struct fc7300_uart_s *priv);
#ifdef CONFIG_FC7300_FCUART_ISR_THREAD
static int fc7300_fcuart_isr(int irq, void *context, void *arg);
#endif
static int  fc7300_interrupt(int irq, void *context, void *arg);

static int  fc7300_setup(struct uart_dev_s *dev);
static void fc7300_shutdown(struct uart_dev_s *dev);
static int  fc7300_attach(struct uart_dev_s *dev);
static void fc7300_detach(struct uart_dev_s *dev);
static int  fc7300_ioctl(struct file *filep, int cmd, unsigned long arg);
static void fc7300_send(struct uart_dev_s *dev, int ch);
static bool fc7300_txready(struct uart_dev_s *dev);
static bool fc7300_txempty(struct uart_dev_s *dev);

#if !defined(SERIAL_HAVE_ONLY_RXDMA)
static int  fc7300_receive(struct uart_dev_s *dev, unsigned int *status);
static void fc7300_rxint(struct uart_dev_s *dev, bool enable);
static bool fc7300_rxavailable(struct uart_dev_s *dev);
#endif

#if !defined(SERIAL_HAVE_ONLY_TXDMA)
static void fc7300_txint(struct uart_dev_s *dev, bool enable);
#endif

#ifdef CONFIG_SERIAL_IFLOWCONTROL
static bool fc7300_rxflowcontrol(struct uart_dev_s *dev,
                                  unsigned int nbuffered, bool upper);
#endif

#ifdef SERIAL_HAVE_TXDMA
static void fc7300_dma_send(struct uart_dev_s *dev);
static void fc7300_dma_txint(struct uart_dev_s *dev, bool enable);
static void fc7300_dma_txavailable(struct uart_dev_s *dev);
static void fc7300_dma_txcallback(struct dma_chan_s *chan, void *arg,
                                  ssize_t len);
#endif

#if defined(SERIAL_HAVE_RXDMA) || defined(SERIAL_HAVE_TXDMA)
static int  fc7300_dma_setup(struct uart_dev_s *dev);
static void fc7300_dma_shutdown(struct uart_dev_s *dev);
#endif

#ifdef SERIAL_HAVE_RXDMA
static int  fc7300_dma_receive(struct uart_dev_s *dev,
                                unsigned int *status);
static void fc7300_dma_rxint(struct uart_dev_s *dev, bool enable);
static bool fc7300_dma_rxavailable(struct uart_dev_s *dev);

static void fc7300_dma_rxcallback(struct dma_chan_s *chan, void *arg,
                                  ssize_t len);
#endif

/****************************************************************************
 * Private Data
 ****************************************************************************/

/* Serial driver UART operations */

#if !defined(SERIAL_HAVE_ONLY_TXDMA) && !defined(SERIAL_HAVE_ONLY_RXDMA)
static const struct uart_ops_s g_fcuart_ops =
{
  .setup          = fc7300_setup,
  .shutdown       = fc7300_shutdown,
  .attach         = fc7300_attach,
  .detach         = fc7300_detach,
  .ioctl          = fc7300_ioctl,
  .receive        = fc7300_receive,
  .rxint          = fc7300_rxint,
  .rxavailable    = fc7300_rxavailable,
#ifdef CONFIG_SERIAL_IFLOWCONTROL
  .rxflowcontrol  = fc7300_rxflowcontrol,
#endif
  .send           = fc7300_send,
  .txint          = fc7300_txint,
  .txready        = fc7300_txready,
  .txempty        = fc7300_txempty,
};
#endif

#if defined(SERIAL_HAVE_RXDMA) && defined(SERIAL_HAVE_TXDMA)
static const struct uart_ops_s g_fcuart_rxtxdma_ops =
{
  .setup          = fc7300_dma_setup,
  .shutdown       = fc7300_dma_shutdown,
  .attach         = fc7300_attach,
  .detach         = fc7300_detach,
  .ioctl          = fc7300_ioctl,
  .receive        = fc7300_dma_receive,
  .rxint          = fc7300_dma_rxint,
  .rxavailable    = fc7300_dma_rxavailable,
#ifdef CONFIG_SERIAL_IFLOWCONTROL
  .rxflowcontrol  = fc7300_rxflowcontrol,
#endif
  .send           = fc7300_send,
  .txint          = fc7300_dma_txint,
  .txready        = fc7300_txready,
  .txempty        = fc7300_txempty,
  .dmatxavail     = fc7300_dma_txavailable,
  .dmasend        = fc7300_dma_send,
};
#endif

#if !defined(SERIAL_HAVE_ONLY_DMA) && defined(SERIAL_HAVE_RXDMA)
static const struct uart_ops_s g_fcuart_rxdma_ops =
{
  .setup          = fc7300_dma_setup,
  .shutdown       = fc7300_dma_shutdown,
  .attach         = fc7300_attach,
  .detach         = fc7300_detach,
  .ioctl          = fc7300_ioctl,
  .receive        = fc7300_dma_receive,
  .rxint          = fc7300_dma_rxint,
  .rxavailable    = fc7300_dma_rxavailable,
#ifdef CONFIG_SERIAL_IFLOWCONTROL
  .rxflowcontrol  = fc7300_rxflowcontrol,
#endif
  .send           = fc7300_send,
  .txint          = fc7300_txint,
  .txready        = fc7300_txready,
  .txempty        = fc7300_txempty,
};
#endif

#if !defined(SERIAL_HAVE_ONLY_DMA) && defined(SERIAL_HAVE_TXDMA)
static const struct uart_ops_s g_fcuart_txdma_ops =
{
  .setup          = fc7300_dma_setup,
  .shutdown       = fc7300_dma_shutdown,
  .attach         = fc7300_attach,
  .detach         = fc7300_detach,
  .ioctl          = fc7300_ioctl,
  .receive        = fc7300_receive,
  .rxint          = fc7300_rxint,
  .rxavailable    = fc7300_rxavailable,
#ifdef CONFIG_SERIAL_IFLOWCONTROL
  .rxflowcontrol  = fc7300_rxflowcontrol,
#endif
  .send           = fc7300_send,
  .txint          = fc7300_dma_txint,
  .txready        = fc7300_txready,
  .txempty        = fc7300_txempty,
  .dmatxavail     = fc7300_dma_txavailable,
  .dmasend        = fc7300_dma_send,
};
#endif

/* Avoid unused warning */
#if !defined(SERIAL_HAVE_ONLY_DMA) && defined(SERIAL_HAVE_RXDMA)
const struct uart_ops_s *g_o0 = &g_fcuart_rxdma_ops;
#endif

#if !defined(SERIAL_HAVE_ONLY_DMA) && defined(SERIAL_HAVE_TXDMA)
const struct uart_ops_s *g_o1 = &g_fcuart_txdma_ops;
#endif

/* I/O buffers */

#ifdef CONFIG_FCUART0_RXDMA
locate_code(".dtcm_data") static char g_fcuart0rxfifo[RXDMA_BUFFER_SIZE] = {0, };
#endif

# ifdef CONFIG_FCUART1_RXDMA
locate_code(".dtcm_data") static char g_fcuart1rxfifo[RXDMA_BUFFER_SIZE] = {0, };
#endif

#ifdef CONFIG_FCUART2_RXDMA
locate_code(".dtcm_data") static char g_fcuart2rxfifo[RXDMA_BUFFER_SIZE] = {0, };
#endif

#ifdef CONFIG_FCUART3_RXDMA
locate_code(".dtcm_data") static char g_fcuart3rxfifo[RXDMA_BUFFER_SIZE] = {0, };
#endif

#ifdef CONFIG_FCUART4_RXDMA
locate_code(".dtcm_data") static char g_fcuart4rxfifo[RXDMA_BUFFER_SIZE] = {0, };
#endif

#ifdef CONFIG_FCUART5_RXDMA
locate_code(".dtcm_data") static char g_fcuart5rxfifo[RXDMA_BUFFER_SIZE] = {0, };
#endif

#ifdef CONFIG_FCUART6_RXDMA
locate_code(".dtcm_data") static char g_fcuart6rxfifo[RXDMA_BUFFER_SIZE] = {0, };
#endif

#ifdef CONFIG_FCUART7_RXDMA
locate_code(".dtcm_data") static char g_fcuart7rxfifo[RXDMA_BUFFER_SIZE] = {0, };
#endif

#ifdef CONFIG_FCUART8_RXDMA
locate_code(".dtcm_data") static char g_fcuart8rxfifo[RXDMA_BUFFER_SIZE] = {0, };
#endif

#ifdef CONFIG_FCUART9_RXDMA
locate_code(".dtcm_data") static char g_fcuart9rxfifo[RXDMA_BUFFER_SIZE] = {0, };
#endif

#ifdef CONFIG_FCUART10_RXDMA
locate_code(".dtcm_data") static char g_fcuart10rxfifo[RXDMA_BUFFER_SIZE] = {0, };
#endif

#ifdef CONFIG_FCUART11_RXDMA
locate_code(".dtcm_data") static char g_fcuart11rxfifo[RXDMA_BUFFER_SIZE] = {0, };
#endif

#ifdef CONFIG_FCUART12_RXDMA
locate_code(".dtcm_data") static char g_fcuart12rxfifo[RXDMA_BUFFER_SIZE] = {0, };
#endif

#ifdef CONFIG_FCUART13_RXDMA
locate_code(".dtcm_data") static char g_fcuart13rxfifo[RXDMA_BUFFER_SIZE] = {0, };
#endif

#ifdef CONFIG_FCUART14_RXDMA
locate_code(".dtcm_data") static char g_fcuart14rxfifo[RXDMA_BUFFER_SIZE] = {0, };
#endif

#ifdef CONFIG_FCUART15_RXDMA
locate_code(".dtcm_data") static char g_fcuart15rxfifo[RXDMA_BUFFER_SIZE] = {0, };
#endif

#ifdef CONFIG_FCUART16_RXDMA
locate_code(".dtcm_data") static char g_fcuart16rxfifo[RXDMA_BUFFER_SIZE] = {0, };
#endif

#ifdef CONFIG_FCUART17_RXDMA
locate_code(".dtcm_data") static char g_fcuart17rxfifo[RXDMA_BUFFER_SIZE] = {0, };
#endif

/* I/O buffers */

#ifdef CONFIG_FC7300_FCUART0
locate_code(".dtcm_data") static char g_fcuart0rxbuffer[CONFIG_FCUART0_RXBUFSIZE] = {0, };
locate_code(".dtcm_data") static char g_fcuart0txbuffer[CONFIG_FCUART0_TXBUFSIZE] = {0, };
#endif

#ifdef CONFIG_FC7300_FCUART1
locate_code(".dtcm_data") static char g_fcuart1rxbuffer[CONFIG_FCUART1_RXBUFSIZE] = {0, };
locate_code(".dtcm_data") static char g_fcuart1txbuffer[CONFIG_FCUART1_TXBUFSIZE] = {0, };
#endif

#ifdef CONFIG_FC7300_FCUART2
locate_code(".dtcm_data") static char g_fcuart2rxbuffer[CONFIG_FCUART2_RXBUFSIZE] = {0, };
locate_code(".dtcm_data") static char g_fcuart2txbuffer[CONFIG_FCUART2_TXBUFSIZE] = {0, };
#endif

#ifdef CONFIG_FC7300_FCUART3
locate_code(".dtcm_data") static char g_fcuart3rxbuffer[CONFIG_FCUART3_RXBUFSIZE] = {0, };
locate_code(".dtcm_data") static char g_fcuart3txbuffer[CONFIG_FCUART3_TXBUFSIZE] = {0, };
#endif

#ifdef CONFIG_FC7300_FCUART4
locate_code(".dtcm_data") static char g_fcuart4rxbuffer[CONFIG_FCUART4_RXBUFSIZE] = {0, };
locate_code(".dtcm_data") static char g_fcuart4txbuffer[CONFIG_FCUART4_TXBUFSIZE] = {0, };
#endif

#ifdef CONFIG_FC7300_FCUART5
locate_code(".dtcm_data") static char g_fcuart5rxbuffer[CONFIG_FCUART5_RXBUFSIZE] = {0, };
locate_code(".dtcm_data") static char g_fcuart5txbuffer[CONFIG_FCUART5_TXBUFSIZE] = {0, };
#endif

#ifdef CONFIG_FC7300_FCUART6
locate_code(".dtcm_data") static char g_fcuart6rxbuffer[CONFIG_FCUART6_RXBUFSIZE] = {0, };
locate_code(".dtcm_data") static char g_fcuart6txbuffer[CONFIG_FCUART6_TXBUFSIZE] = {0, };
#endif

#ifdef CONFIG_FC7300_FCUART7
locate_code(".dtcm_data") static char g_fcuart7rxbuffer[CONFIG_FCUART7_RXBUFSIZE] = {0, };
locate_code(".dtcm_data") static char g_fcuart7txbuffer[CONFIG_FCUART7_TXBUFSIZE] = {0, };
#endif

#ifdef CONFIG_FC7300_FCUART8
locate_code(".dtcm_data") static char g_fcuart8rxbuffer[CONFIG_FCUART8_RXBUFSIZE] = {0, };
locate_code(".dtcm_data") static char g_fcuart8txbuffer[CONFIG_FCUART8_TXBUFSIZE] = {0, };
#endif

#ifdef CONFIG_FC7300_FCUART9
locate_code(".dtcm_data") static char g_fcuart9rxbuffer[CONFIG_FCUART9_RXBUFSIZE] = {0, };
locate_code(".dtcm_data") static char g_fcuart9txbuffer[CONFIG_FCUART9_TXBUFSIZE] = {0, };
#endif

#ifdef CONFIG_FC7300_FCUART10
locate_code(".dtcm_data") static char g_fcuart10rxbuffer[CONFIG_FCUART10_RXBUFSIZE] = {0, };
locate_code(".dtcm_data") static char g_fcuart10txbuffer[CONFIG_FCUART10_TXBUFSIZE] = {0, };
#endif

#ifdef CONFIG_FC7300_FCUART11
locate_code(".dtcm_data") static char g_fcuart11rxbuffer[CONFIG_FCUART11_RXBUFSIZE] = {0, };
locate_code(".dtcm_data") static char g_fcuart11txbuffer[CONFIG_FCUART11_TXBUFSIZE] = {0, };
#endif

#ifdef CONFIG_FC7300_FCUART12
locate_code(".dtcm_data") static char g_fcuart12rxbuffer[CONFIG_FCUART12_RXBUFSIZE] = {0, };
locate_code(".dtcm_data") static char g_fcuart12txbuffer[CONFIG_FCUART12_TXBUFSIZE] = {0, };
#endif

#ifdef CONFIG_FC7300_FCUART13
locate_code(".dtcm_data") static char g_fcuart13rxbuffer[CONFIG_FCUART13_RXBUFSIZE] = {0, };
locate_code(".dtcm_data") static char g_fcuart13txbuffer[CONFIG_FCUART13_TXBUFSIZE] = {0, };
#endif

#ifdef CONFIG_FC7300_FCUART14
locate_code(".dtcm_data") static char g_fcuart14rxbuffer[CONFIG_FCUART14_RXBUFSIZE] = {0, };
locate_code(".dtcm_data") static char g_fcuart14txbuffer[CONFIG_FCUART14_TXBUFSIZE] = {0, };
#endif

#ifdef CONFIG_FC7300_FCUART15
locate_code(".dtcm_data") static char g_fcuart15rxbuffer[CONFIG_FCUART15_RXBUFSIZE] = {0, };
locate_code(".dtcm_data") static char g_fcuart15txbuffer[CONFIG_FCUART15_TXBUFSIZE] = {0, };
#endif

#ifdef CONFIG_FC7300_FCUART16
locate_code(".dtcm_data") static char g_fcuart16rxbuffer[CONFIG_FCUART16_RXBUFSIZE] = {0, };
locate_code(".dtcm_data") static char g_fcuart16txbuffer[CONFIG_FCUART16_TXBUFSIZE] = {0, };
#endif

#ifdef CONFIG_FC7300_FCUART17
locate_code(".dtcm_data") static char g_fcuart17rxbuffer[CONFIG_FCUART17_RXBUFSIZE] = {0, };
locate_code(".dtcm_data") static char g_fcuart17txbuffer[CONFIG_FCUART17_TXBUFSIZE] = {0, };
#endif

/* This describes the state of the FC7300 FCUART0 port. */

#ifdef CONFIG_FC7300_FCUART0
static struct fc7300_uart_s g_fcuart0priv =
{
  .uartbase     = FC7300_FCUART0_BASE,
  .baud         = CONFIG_FCUART0_BAUD,
  .irq          = FC7300_IRQ_FCUART0,
  .parity       = CONFIG_FCUART0_PARITY,
  .bits         = CONFIG_FCUART0_BITS,
  .stopbits2    = CONFIG_FCUART0_2STOP,
#  if defined(CONFIG_SERIAL_IFLOWCONTROL) && defined(CONFIG_FCUART0_IFLOWCONTROL)
  .iflow        = true,
#  endif
#  if defined(CONFIG_SERIAL_OFLOWCONTROL) && defined(CONFIG_FCUART0_OFLOWCONTROL)
  .oflow        = true,
#  endif
#  if defined(CONFIG_SERIAL_IFLOWCONTROL) && defined(CONFIG_FCUART0_IFLOWCONTROL)
  .rts_gpio     = PIN_FCUART0_RTS,
#  endif
#  if defined(CONFIG_SERIAL_OFLOWCONTROL) && defined(CONFIG_FCUART0_OFLOWCONTROL)
  .cts_gpio     = PIN_FCUART0_CTS,
#  endif

#  ifdef CONFIG_FCUART0_TXDMA
  .dma_txreqsrc = FC7300_DMA_REQ_FCUART0_TX,
  .dma_txchidx  = CONFIG_FCUART0_TXDMA_CHANNEL,
#  endif
#  ifdef CONFIG_FCUART0_RXDMA
  .dma_rxreqsrc = FC7300_DMA_REQ_FCUART0_RX,
  .dma_rxchidx  = CONFIG_FCUART0_RXDMA_CHANNEL,
  .rxfifo       = g_fcuart0rxfifo,
#  endif
};

static struct uart_dev_s g_fcuart0dev =
{
  .recv         =
    {
      .size     = CONFIG_FCUART0_RXBUFSIZE,
      .buffer   = g_fcuart0rxbuffer,
    },
  .xmit         =
    {
      .size     = CONFIG_FCUART0_TXBUFSIZE,
      .buffer   = g_fcuart0txbuffer,
    },
#  if defined(CONFIG_FCUART0_RXDMA) && defined(CONFIG_FCUART0_TXDMA)
  .ops          = &g_fcuart_rxtxdma_ops,
#  elif defined(CONFIG_FCUART0_RXDMA) && !defined(CONFIG_FCUART0_TXDMA)
  .ops          = &g_fcuart_rxdma_ops,
#  elif !defined(CONFIG_FCUART0_RXDMA) && defined(CONFIG_FCUART0_TXDMA)
  .ops          = &g_fcuart_txdma_ops,
#  else
  .ops          = &g_fcuart_ops,
#  endif
  .priv         = &g_fcuart0priv,
};
#endif

/* This describes the state of the FC7300 FCUART1 port. */

#ifdef CONFIG_FC7300_FCUART1
static struct fc7300_uart_s g_fcuart1priv =
{
  .uartbase     = FC7300_FCUART1_BASE,
  .baud         = CONFIG_FCUART1_BAUD,
  .irq          = FC7300_IRQ_FCUART1,
  .parity       = CONFIG_FCUART1_PARITY,
  .bits         = CONFIG_FCUART1_BITS,
  .stopbits2    = CONFIG_FCUART1_2STOP,
#  if defined(CONFIG_SERIAL_IFLOWCONTROL) && defined(CONFIG_FCUART1_IFLOWCONTROL)
  .iflow        = true,
#  endif
#  if defined(CONFIG_SERIAL_OFLOWCONTROL) && defined(CONFIG_FCUART1_OFLOWCONTROL)
  .oflow        = true,
#  endif
#  if defined(CONFIG_SERIAL_IFLOWCONTROL) && defined(CONFIG_FCUART1_IFLOWCONTROL)
  .rts_gpio     = PIN_FCUART1_RTS,
#  endif
#  if defined(CONFIG_SERIAL_OFLOWCONTROL) && defined(CONFIG_FCUART1_OFLOWCONTROL)
  .cts_gpio     = PIN_FCUART1_CTS,
#  endif

#  ifdef CONFIG_FCUART1_TXDMA
  .dma_txreqsrc = FC7300_DMA_REQ_FCUART1_TX,
  .dma_txchidx  = CONFIG_FCUART1_TXDMA_CHANNEL,
#  endif
#  ifdef CONFIG_FCUART1_RXDMA
  .dma_rxreqsrc = FC7300_DMA_REQ_FCUART1_RX,
  .dma_rxchidx  = CONFIG_FCUART1_RXDMA_CHANNEL,
  .rxfifo       = g_fcuart1rxfifo,
#  endif
};

static struct uart_dev_s g_fcuart1dev =
{
  .recv         =
    {
      .size     = CONFIG_FCUART1_RXBUFSIZE,
      .buffer   = g_fcuart1rxbuffer,
    },
  .xmit         =
    {
      .size     = CONFIG_FCUART1_TXBUFSIZE,
      .buffer   = g_fcuart1txbuffer,
    },
#  if defined(CONFIG_FCUART1_RXDMA) && defined(CONFIG_FCUART1_TXDMA)
  .ops          = &g_fcuart_rxtxdma_ops,
#  elif defined(CONFIG_FCUART1_RXDMA) && !defined(CONFIG_FCUART1_TXDMA)
  .ops          = &g_fcuart_rxdma_ops,
#  elif !defined(CONFIG_FCUART1_RXDMA) && defined(CONFIG_FCUART1_TXDMA)
  .ops          = &g_fcuart_txdma_ops,
#  else
  .ops          = &g_fcuart_ops,
#  endif
  .priv         = &g_fcuart1priv,
};
#endif

/* This describes the state of the FC7300 FCUART2 port. */

#ifdef CONFIG_FC7300_FCUART2
static struct fc7300_uart_s g_fcuart2priv =
{
  .uartbase     = FC7300_FCUART2_BASE,
  .baud         = CONFIG_FCUART2_BAUD,
  .irq          = FC7300_IRQ_FCUART2,
  .parity       = CONFIG_FCUART2_PARITY,
  .bits         = CONFIG_FCUART2_BITS,
  .stopbits2    = CONFIG_FCUART2_2STOP,
#  if defined(CONFIG_SERIAL_IFLOWCONTROL) && defined(CONFIG_FCUART2_IFLOWCONTROL)
  .iflow        = true,
#  endif
#  if defined(CONFIG_SERIAL_OFLOWCONTROL) && defined(CONFIG_FCUART2_OFLOWCONTROL)
  .oflow        = true,
#  endif
#  if defined(CONFIG_SERIAL_IFLOWCONTROL) && defined(CONFIG_FCUART2_IFLOWCONTROL)
  .rts_gpio     = PIN_FCUART2_RTS,
#  endif
#  if defined(CONFIG_SERIAL_OFLOWCONTROL) && defined(CONFIG_FCUART2_OFLOWCONTROL)
  .cts_gpio     = PIN_FCUART2_CTS,
#  endif

#  ifdef CONFIG_FCUART2_TXDMA
  .dma_txreqsrc = FC7300_DMA_REQ_FCUART2_TX,
  .dma_txchidx  = CONFIG_FCUART2_TXDMA_CHANNEL,
#  endif
#  ifdef CONFIG_FCUART2_RXDMA
  .dma_rxreqsrc = FC7300_DMA_REQ_FCUART2_RX,
  .dma_rxchidx  = CONFIG_FCUART2_RXDMA_CHANNEL,
  .rxfifo       = g_fcuart2rxfifo,
#  endif
};

static struct uart_dev_s g_fcuart2dev =
{
  .recv         =
    {
      .size     = CONFIG_FCUART2_RXBUFSIZE,
      .buffer   = g_fcuart2rxbuffer,
    },
  .xmit         =
    {
      .size     = CONFIG_FCUART2_TXBUFSIZE,
      .buffer   = g_fcuart2txbuffer,
    },
#  if defined(CONFIG_FCUART2_RXDMA) && defined(CONFIG_FCUART2_TXDMA)
  .ops          = &g_fcuart_rxtxdma_ops,
#  elif defined(CONFIG_FCUART2_RXDMA) && !defined(CONFIG_FCUART2_TXDMA)
  .ops          = &g_fcuart_rxdma_ops,
#  elif !defined(CONFIG_FCUART2_RXDMA) && defined(CONFIG_FCUART2_TXDMA)
  .ops          = &g_fcuart_txdma_ops,
#  else
  .ops          = &g_fcuart_ops,
#  endif
  .priv         = &g_fcuart2priv,
};
#endif

/* This describes the state of the FC7300 FCUART3 port. */

#ifdef CONFIG_FC7300_FCUART3
static struct fc7300_uart_s g_fcuart3priv =
{
  .uartbase     = FC7300_FCUART3_BASE,
  .baud         = CONFIG_FCUART3_BAUD,
  .irq          = FC7300_IRQ_FCUART3,
  .parity       = CONFIG_FCUART3_PARITY,
  .bits         = CONFIG_FCUART3_BITS,
  .stopbits2    = CONFIG_FCUART3_2STOP,
#  if defined(CONFIG_SERIAL_IFLOWCONTROL) && defined(CONFIG_FCUART3_IFLOWCONTROL)
  .iflow        = true,
#  endif
#  if defined(CONFIG_SERIAL_OFLOWCONTROL) && defined(CONFIG_FCUART3_OFLOWCONTROL)
  .oflow        = true,
#  endif
#  if defined(CONFIG_SERIAL_IFLOWCONTROL) && defined(CONFIG_FCUART3_IFLOWCONTROL)
  .rts_gpio     = PIN_FCUART3_RTS,
#  endif
#  if defined(CONFIG_SERIAL_OFLOWCONTROL) && defined(CONFIG_FCUART3_OFLOWCONTROL)
  .cts_gpio     = PIN_FCUART3_CTS,
#  endif

#  ifdef CONFIG_FCUART3_TXDMA
  .dma_txreqsrc = FC7300_DMA_REQ_FCUART3_TX,
  .dma_txchidx  = CONFIG_FCUART3_TXDMA_CHANNEL,
#  endif
#  ifdef CONFIG_FCUART3_RXDMA
  .dma_rxreqsrc = FC7300_DMA_REQ_FCUART3_RX,
  .dma_rxchidx  = CONFIG_FCUART3_RXDMA_CHANNEL,
  .rxfifo       = g_fcuart3rxfifo,
#  endif
};

static struct uart_dev_s g_fcuart3dev =
{
  .recv         =
    {
      .size     = CONFIG_FCUART3_RXBUFSIZE,
      .buffer   = g_fcuart3rxbuffer,
    },
  .xmit         =
    {
      .size     = CONFIG_FCUART3_TXBUFSIZE,
      .buffer   = g_fcuart3txbuffer,
    },
#  if defined(CONFIG_FCUART3_RXDMA) && defined(CONFIG_FCUART3_TXDMA)
  .ops          = &g_fcuart_rxtxdma_ops,
#  elif defined(CONFIG_FCUART3_RXDMA) && !defined(CONFIG_FCUART3_TXDMA)
  .ops          = &g_fcuart_rxdma_ops,
#  elif !defined(CONFIG_FCUART3_RXDMA) && defined(CONFIG_FCUART3_TXDMA)
  .ops          = &g_fcuart_txdma_ops,
#  else
  .ops          = &g_fcuart_ops,
#  endif
  .priv         = &g_fcuart3priv,
};
#endif

/* This describes the state of the FC7300 FCUART4 port. */

#ifdef CONFIG_FC7300_FCUART4
static struct fc7300_uart_s g_fcuart4priv =
{
  .uartbase     = FC7300_FCUART4_BASE,
  .baud         = CONFIG_FCUART4_BAUD,
  .irq          = FC7300_IRQ_FCUART4,
  .parity       = CONFIG_FCUART4_PARITY,
  .bits         = CONFIG_FCUART4_BITS,
  .stopbits2    = CONFIG_FCUART4_2STOP,
#  if defined(CONFIG_SERIAL_IFLOWCONTROL) && defined(CONFIG_FCUART4_IFLOWCONTROL)
  .iflow        = true,
#  endif
#  if defined(CONFIG_SERIAL_OFLOWCONTROL) && defined(CONFIG_FCUART4_OFLOWCONTROL)
  .oflow        = true,
#  endif
#  if defined(CONFIG_SERIAL_IFLOWCONTROL) && defined(CONFIG_FCUART4_IFLOWCONTROL)
  .rts_gpio     = PIN_FCUART4_RTS,
#  endif
#  if defined(CONFIG_SERIAL_OFLOWCONTROL) && defined(CONFIG_FCUART4_OFLOWCONTROL)
  .cts_gpio     = PIN_FCUART4_CTS,
#  endif

#  ifdef CONFIG_FCUART4_TXDMA
  .dma_txreqsrc = FC7300_DMA_REQ_FCUART4_TX,
  .dma_txchidx  = CONFIG_FCUART4_TXDMA_CHANNEL,
#  endif
#  ifdef CONFIG_FCUART4_RXDMA
  .dma_rxreqsrc = FC7300_DMA_REQ_FCUART4_RX,
  .dma_rxchidx  = CONFIG_FCUART4_RXDMA_CHANNEL,
  .rxfifo       = g_fcuart4rxfifo,
#  endif
};

static struct uart_dev_s g_fcuart4dev =
{
  .recv         =
    {
      .size     = CONFIG_FCUART4_RXBUFSIZE,
      .buffer   = g_fcuart4rxbuffer,
    },
  .xmit         =
    {
      .size     = CONFIG_FCUART4_TXBUFSIZE,
      .buffer   = g_fcuart4txbuffer,
    },
#  if defined(CONFIG_FCUART4_RXDMA) && defined(CONFIG_FCUART4_TXDMA)
  .ops          = &g_fcuart_rxtxdma_ops,
#  elif defined(CONFIG_FCUART4_RXDMA) && !defined(CONFIG_FCUART4_TXDMA)
  .ops          = &g_fcuart_rxdma_ops,
#  elif !defined(CONFIG_FCUART4_RXDMA) && defined(CONFIG_FCUART4_TXDMA)
  .ops          = &g_fcuart_txdma_ops,
#  else
  .ops          = &g_fcuart_ops,
#  endif
  .priv         = &g_fcuart4priv,
};
#endif

/* This describes the state of the FC7300 FCUART5 port. */

#ifdef CONFIG_FC7300_FCUART5
static struct fc7300_uart_s g_fcuart5priv =
{
  .uartbase     = FC7300_FCUART5_BASE,
  .baud         = CONFIG_FCUART5_BAUD,
  .irq          = FC7300_IRQ_FCUART5,
  .parity       = CONFIG_FCUART5_PARITY,
  .bits         = CONFIG_FCUART5_BITS,
  .stopbits2    = CONFIG_FCUART5_2STOP,
#  if defined(CONFIG_SERIAL_IFLOWCONTROL) && defined(CONFIG_FCUART5_IFLOWCONTROL)
  .iflow        = true,
#  endif
#  if defined(CONFIG_SERIAL_OFLOWCONTROL) && defined(CONFIG_FCUART5_OFLOWCONTROL)
  .oflow        = true,
#  endif
#  if defined(CONFIG_SERIAL_IFLOWCONTROL) && defined(CONFIG_FCUART5_IFLOWCONTROL)
  .rts_gpio     = PIN_FCUART5_RTS,
#  endif
#  if defined(CONFIG_SERIAL_OFLOWCONTROL) && defined(CONFIG_FCUART5_OFLOWCONTROL)
  .cts_gpio     = PIN_FCUART5_CTS,
#  endif

#  ifdef CONFIG_FCUART5_TXDMA
  .dma_txreqsrc = FC7300_DMA_REQ_FCUART5_TX,
  .dma_txchidx  = CONFIG_FCUART5_TXDMA_CHANNEL,
#  endif
#  ifdef CONFIG_FCUART5_RXDMA
  .dma_rxreqsrc = FC7300_DMA_REQ_FCUART5_RX,
  .dma_rxchidx  = CONFIG_FCUART5_RXDMA_CHANNEL,
  .rxfifo       = g_fcuart5rxfifo,
#  endif
};

static struct uart_dev_s g_fcuart5dev =
{
  .recv         =
    {
      .size     = CONFIG_FCUART5_RXBUFSIZE,
      .buffer   = g_fcuart5rxbuffer,
    },
  .xmit         =
    {
      .size     = CONFIG_FCUART5_TXBUFSIZE,
      .buffer   = g_fcuart5txbuffer,
    },
#  if defined(CONFIG_FCUART5_RXDMA) && defined(CONFIG_FCUART5_TXDMA)
  .ops          = &g_fcuart_rxtxdma_ops,
#  elif defined(CONFIG_FCUART5_RXDMA) && !defined(CONFIG_FCUART5_TXDMA)
  .ops          = &g_fcuart_rxdma_ops,
#  elif !defined(CONFIG_FCUART5_RXDMA) && defined(CONFIG_FCUART5_TXDMA)
  .ops          = &g_fcuart_txdma_ops,
#  else
  .ops          = &g_fcuart_ops,
#  endif
  .priv         = &g_fcuart5priv,
};
#endif

/* This describes the state of the FC7300 FCUART6 port. */

#ifdef CONFIG_FC7300_FCUART6
static struct fc7300_uart_s g_fcuart6priv =
{
  .uartbase     = FC7300_FCUART6_BASE,
  .baud         = CONFIG_FCUART6_BAUD,
  .irq          = FC7300_IRQ_FCUART6,
  .parity       = CONFIG_FCUART6_PARITY,
  .bits         = CONFIG_FCUART6_BITS,
  .stopbits2    = CONFIG_FCUART6_2STOP,
#  if defined(CONFIG_SERIAL_IFLOWCONTROL) && defined(CONFIG_FCUART6_IFLOWCONTROL)
  .iflow        = true,
#  endif
#  if defined(CONFIG_SERIAL_OFLOWCONTROL) && defined(CONFIG_FCUART6_OFLOWCONTROL)
  .oflow        = true,
#  endif
#  if defined(CONFIG_SERIAL_IFLOWCONTROL) && defined(CONFIG_FCUART6_IFLOWCONTROL)
  .rts_gpio     = PIN_FCUART6_RTS,
#  endif
#  if defined(CONFIG_SERIAL_OFLOWCONTROL) && defined(CONFIG_FCUART6_OFLOWCONTROL)
  .cts_gpio     = PIN_FCUART6_CTS,
#  endif

#  ifdef CONFIG_FCUART6_TXDMA
  .dma_txreqsrc = FC7300_DMA_REQ_FCUART6_TX,
  .dma_txchidx  = CONFIG_FCUART6_TXDMA_CHANNEL,
#  endif
#  ifdef CONFIG_FCUART6_RXDMA
  .dma_rxreqsrc = FC7300_DMA_REQ_FCUART6_RX,
  .dma_rxchidx  = CONFIG_FCUART6_RXDMA_CHANNEL,
  .rxfifo       = g_fcuart6rxfifo,
#  endif
};

static struct uart_dev_s g_fcuart6dev =
{
  .recv         =
    {
      .size     = CONFIG_FCUART6_RXBUFSIZE,
      .buffer   = g_fcuart6rxbuffer,
    },
  .xmit         =
    {
      .size     = CONFIG_FCUART6_TXBUFSIZE,
      .buffer   = g_fcuart6txbuffer,
    },
#  if defined(CONFIG_FCUART6_RXDMA) && defined(CONFIG_FCUART6_TXDMA)
  .ops          = &g_fcuart_rxtxdma_ops,
#  elif defined(CONFIG_FCUART6_RXDMA) && !defined(CONFIG_FCUART6_TXDMA)
  .ops          = &g_fcuart_rxdma_ops,
#  elif !defined(CONFIG_FCUART6_RXDMA) && defined(CONFIG_FCUART6_TXDMA)
  .ops          = &g_fcuart_txdma_ops,
#  else
  .ops          = &g_fcuart_ops,
#  endif
  .priv         = &g_fcuart6priv,
};
#endif

/* This describes the state of the FC7300 FCUART7 port. */

#ifdef CONFIG_FC7300_FCUART7
static struct fc7300_uart_s g_fcuart7priv =
{
  .uartbase     = FC7300_FCUART7_BASE,
  .baud         = CONFIG_FCUART7_BAUD,
  .irq          = FC7300_IRQ_FCUART7,
  .parity       = CONFIG_FCUART7_PARITY,
  .bits         = CONFIG_FCUART7_BITS,
  .stopbits2    = CONFIG_FCUART7_2STOP,
#  if defined(CONFIG_SERIAL_IFLOWCONTROL) && defined(CONFIG_FCUART7_IFLOWCONTROL)
  .iflow        = true,
#  endif
#  if defined(CONFIG_SERIAL_OFLOWCONTROL) && defined(CONFIG_FCUART7_OFLOWCONTROL)
  .oflow        = true,
#  endif
#  if  defined(CONFIG_SERIAL_IFLOWCONTROL) && defined(CONFIG_FCUART7_IFLOWCONTROL)
  .rts_gpio     = PIN_FCUART7_RTS,
#  endif
#  if defined(CONFIG_SERIAL_OFLOWCONTROL) && defined(CONFIG_FCUART7_OFLOWCONTROL)
  .cts_gpio     = PIN_FCUART7_CTS,
#  endif

#  ifdef CONFIG_FCUART7_TXDMA
  .dma_txreqsrc = FC7300_DMA_REQ_FCUART7_TX,
  .dma_txchidx  = CONFIG_FCUART7_TXDMA_CHANNEL,
#  endif
#  ifdef CONFIG_FCUART7_RXDMA
  .dma_rxreqsrc = FC7300_DMA_REQ_FCUART7_RX,
  .dma_rxchidx  = CONFIG_FCUART7_RXDMA_CHANNEL,
  .rxfifo       = g_fcuart7rxfifo,
#  endif
};

static struct uart_dev_s g_fcuart7dev =
{
  .recv         =
    {
      .size     = CONFIG_FCUART7_RXBUFSIZE,
      .buffer   = g_fcuart7rxbuffer,
    },
  .xmit         =
    {
      .size     = CONFIG_FCUART7_TXBUFSIZE,
      .buffer   = g_fcuart7txbuffer,
    },
#  if defined(CONFIG_FCUART7_RXDMA) && defined(CONFIG_FCUART7_TXDMA)
  .ops          = &g_fcuart_rxtxdma_ops,
#  elif defined(CONFIG_FCUART7_RXDMA) && !defined(CONFIG_FCUART7_TXDMA)
  .ops          = &g_fcuart_rxdma_ops,
#  elif !defined(CONFIG_FCUART7_RXDMA) && defined(CONFIG_FCUART7_TXDMA)
  .ops          = &g_fcuart_txdma_ops,
#  else
  .ops          = &g_fcuart_ops,
#  endif
  .priv         = &g_fcuart7priv,
};
#endif

/* This describes the state of the FC7300 FCUART8 port. */

#ifdef CONFIG_FC7300_FCUART8
static struct fc7300_uart_s g_fcuart8priv =
{
  .uartbase     = FC7300_FCUART8_BASE,
  .baud         = CONFIG_FCUART8_BAUD,
  .irq          = FC7300_IRQ_FCUART8,
  .parity       = CONFIG_FCUART8_PARITY,
  .bits         = CONFIG_FCUART8_BITS,
  .stopbits2    = CONFIG_FCUART8_2STOP,
#  if defined(CONFIG_SERIAL_IFLOWCONTROL) && defined(CONFIG_FCUART8_IFLOWCONTROL)
  .iflow        = true,
#  endif
#  if defined(CONFIG_SERIAL_OFLOWCONTROL) && defined(CONFIG_FCUART8_OFLOWCONTROL)
  .oflow        = true,
#  endif
#  if  defined(CONFIG_SERIAL_IFLOWCONTROL) && defined(CONFIG_FCUART8_IFLOWCONTROL)
  .rts_gpio     = PIN_FCUART8_RTS,
#  endif
#  if defined(CONFIG_SERIAL_OFLOWCONTROL) && defined(CONFIG_FCUART8_OFLOWCONTROL)
  .cts_gpio     = PIN_FCUART8_CTS,
#  endif

#  ifdef CONFIG_FCUART8_TXDMA
  .dma_txreqsrc = FC7300_DMA_REQ_FCUART8_TX,
  .dma_txchidx  = CONFIG_FCUART8_TXDMA_CHANNEL,
#  endif
#  ifdef CONFIG_FCUART8_RXDMA
  .dma_rxreqsrc = FC7300_DMA_REQ_FCUART8_RX,
  .dma_rxchidx  = CONFIG_FCUART8_RXDMA_CHANNEL,
  .rxfifo       = g_fcuart8rxfifo,
#  endif
};

static struct uart_dev_s g_fcuart8dev =
{
  .recv         =
    {
      .size     = CONFIG_FCUART8_RXBUFSIZE,
      .buffer   = g_fcuart8rxbuffer,
    },
  .xmit         =
    {
      .size     = CONFIG_FCUART8_TXBUFSIZE,
      .buffer   = g_fcuart8txbuffer,
    },
#  if defined(CONFIG_FCUART8_RXDMA) && defined(CONFIG_FCUART8_TXDMA)
  .ops          = &g_fcuart_rxtxdma_ops,
#  elif defined(CONFIG_FCUART8_RXDMA) && !defined(CONFIG_FCUART8_TXDMA)
  .ops          = &g_fcuart_rxdma_ops,
#  elif !defined(CONFIG_FCUART8_RXDMA) && defined(CONFIG_FCUART8_TXDMA)
  .ops          = &g_fcuart_txdma_ops,
#  else
  .ops          = &g_fcuart_ops,
#  endif
  .priv         = &g_fcuart8priv,
};
#endif

/* This describes the state of the FC7300 FCUART9 port. */

#ifdef CONFIG_FC7300_FCUART9
static struct fc7300_uart_s g_fcuart9priv =
{
  .uartbase     = FC7300_FCUART9_BASE,
  .baud         = CONFIG_FCUART9_BAUD,
  .irq          = FC7300_IRQ_FCUART9,
  .parity       = CONFIG_FCUART9_PARITY,
  .bits         = CONFIG_FCUART9_BITS,
  .stopbits2    = CONFIG_FCUART9_2STOP,
#  if defined(CONFIG_SERIAL_IFLOWCONTROL) && defined(CONFIG_FCUART9_IFLOWCONTROL)
  .iflow        = true,
#  endif
#  if defined(CONFIG_SERIAL_OFLOWCONTROL) && defined(CONFIG_FCUART9_OFLOWCONTROL)
  .oflow        = true,
#  endif
#  if  defined(CONFIG_SERIAL_IFLOWCONTROL) && defined(CONFIG_FCUART9_IFLOWCONTROL)
  .rts_gpio     = PIN_FCUART9_RTS,
#  endif
#  if defined(CONFIG_SERIAL_OFLOWCONTROL) && defined(CONFIG_FCUART9_OFLOWCONTROL)
  .cts_gpio     = PIN_FCUART9_CTS,
#  endif

#  ifdef CONFIG_FCUART9_TXDMA
  .dma_txreqsrc = FC7300_DMA_REQ_FCUART9_TX,
  .dma_txchidx  = CONFIG_FCUART9_TXDMA_CHANNEL,
#  endif
#  ifdef CONFIG_FCUART9_RXDMA
  .dma_rxreqsrc = FC7300_DMA_REQ_FCUART9_RX,
  .dma_rxchidx  = CONFIG_FCUART9_RXDMA_CHANNEL,
  .rxfifo       = g_fcuart9rxfifo,
#  endif
};

static struct uart_dev_s g_fcuart9dev =
{
  .recv         =
    {
      .size     = CONFIG_FCUART9_RXBUFSIZE,
      .buffer   = g_fcuart9rxbuffer,
    },
  .xmit         =
    {
      .size     = CONFIG_FCUART9_TXBUFSIZE,
      .buffer   = g_fcuart9txbuffer,
    },
#  if defined(CONFIG_FCUART9_RXDMA) && defined(CONFIG_FCUART9_TXDMA)
  .ops          = &g_fcuart_rxtxdma_ops,
#  elif defined(CONFIG_FCUART9_RXDMA) && !defined(CONFIG_FCUART9_TXDMA)
  .ops          = &g_fcuart_rxdma_ops,
#  elif !defined(CONFIG_FCUART9_RXDMA) && defined(CONFIG_FCUART9_TXDMA)
  .ops          = &g_fcuart_txdma_ops,
#  else
  .ops          = &g_fcuart_ops,
#  endif
  .priv         = &g_fcuart9priv,
};
#endif

/* This describes the state of the FC7300 FCUART10 port. */

#ifdef CONFIG_FC7300_FCUART10
static struct fc7300_uart_s g_fcuart10priv =
{
  .uartbase     = FC7300_FCUART10_BASE,
  .baud         = CONFIG_FCUART10_BAUD,
  .irq          = FC7300_IRQ_FCUART10,
  .parity       = CONFIG_FCUART10_PARITY,
  .bits         = CONFIG_FCUART10_BITS,
  .stopbits2    = CONFIG_FCUART10_2STOP,
#  if defined(CONFIG_SERIAL_IFLOWCONTROL) && defined(CONFIG_FCUART10_IFLOWCONTROL)
  .iflow        = true,
#  endif
#  if defined(CONFIG_SERIAL_OFLOWCONTROL) && defined(CONFIG_FCUART10_OFLOWCONTROL)
  .oflow        = true,
#  endif
#  if  defined(CONFIG_SERIAL_IFLOWCONTROL) && defined(CONFIG_FCUART10_IFLOWCONTROL)
  .rts_gpio     = PIN_FCUART10_RTS,
#  endif
#  if defined(CONFIG_SERIAL_OFLOWCONTROL) && defined(CONFIG_FCUART10_OFLOWCONTROL)
  .cts_gpio     = PIN_FCUART10_CTS,
#  endif

#  ifdef CONFIG_FCUART10_TXDMA
  .dma_txreqsrc = FC7300_DMA_REQ_FCUART10_TX,
  .dma_txchidx  = CONFIG_FCUART10_TXDMA_CHANNEL,
#  endif
#  ifdef CONFIG_FCUART10_RXDMA
  .dma_rxreqsrc = FC7300_DMA_REQ_FCUART10_RX,
  .dma_rxchidx  = CONFIG_FCUART10_RXDMA_CHANNEL,
  .rxfifo       = g_fcuart10rxfifo,
#  endif
};

static struct uart_dev_s g_fcuart10dev =
{
  .recv         =
    {
      .size     = CONFIG_FCUART10_RXBUFSIZE,
      .buffer   = g_fcuart10rxbuffer,
    },
  .xmit         =
    {
      .size     = CONFIG_FCUART10_TXBUFSIZE,
      .buffer   = g_fcuart10txbuffer,
    },
#  if defined(CONFIG_FCUART10_RXDMA) && defined(CONFIG_FCUART10_TXDMA)
  .ops          = &g_fcuart_rxtxdma_ops,
#  elif defined(CONFIG_FCUART10_RXDMA) && !defined(CONFIG_FCUART10_TXDMA)
  .ops          = &g_fcuart_rxdma_ops,
#  elif !defined(CONFIG_FCUART10_RXDMA) && defined(CONFIG_FCUART10_TXDMA)
  .ops          = &g_fcuart_txdma_ops,
#  else
  .ops          = &g_fcuart_ops,
#  endif
  .priv         = &g_fcuart10priv,
};
#endif

/* This describes the state of the FC7300 FCUART11 port. */

#ifdef CONFIG_FC7300_FCUART11
static struct fc7300_uart_s g_fcuart11priv =
{
  .uartbase     = FC7300_FCUART11_BASE,
  .baud         = CONFIG_FCUART11_BAUD,
  .irq          = FC7300_IRQ_FCUART11,
  .parity       = CONFIG_FCUART11_PARITY,
  .bits         = CONFIG_FCUART11_BITS,
  .stopbits2    = CONFIG_FCUART11_2STOP,
#  if defined(CONFIG_SERIAL_IFLOWCONTROL) && defined(CONFIG_FCUART11_IFLOWCONTROL)
  .iflow        = true,
#  endif
#  if defined(CONFIG_SERIAL_OFLOWCONTROL) && defined(CONFIG_FCUART11_OFLOWCONTROL)
  .oflow        = true,
#  endif
#  if  defined(CONFIG_SERIAL_IFLOWCONTROL) && defined(CONFIG_FCUART11_IFLOWCONTROL)
  .rts_gpio     = PIN_FCUART11_RTS,
#  endif
#  if defined(CONFIG_SERIAL_OFLOWCONTROL) && defined(CONFIG_FCUART11_OFLOWCONTROL)
  .cts_gpio     = PIN_FCUART11_CTS,
#  endif

#  ifdef CONFIG_FCUART11_TXDMA
  .dma_txreqsrc = FC7300_DMA_REQ_FCUART11_TX,
  .dma_txchidx  = CONFIG_FCUART11_TXDMA_CHANNEL,
#  endif
#  ifdef CONFIG_FCUART11_RXDMA
  .dma_rxreqsrc = FC7300_DMA_REQ_FCUART11_RX,
  .dma_rxchidx  = CONFIG_FCUART11_RXDMA_CHANNEL,
  .rxfifo       = g_fcuart11rxfifo,
#  endif
};

static struct uart_dev_s g_fcuart11dev =
{
  .recv         =
    {
      .size     = CONFIG_FCUART11_RXBUFSIZE,
      .buffer   = g_fcuart11rxbuffer,
    },
  .xmit         =
    {
      .size     = CONFIG_FCUART11_TXBUFSIZE,
      .buffer   = g_fcuart11txbuffer,
    },
#  if defined(CONFIG_FCUART11_RXDMA) && defined(CONFIG_FCUART11_TXDMA)
  .ops          = &g_fcuart_rxtxdma_ops,
#  elif defined(CONFIG_FCUART11_RXDMA) && !defined(CONFIG_FCUART11_TXDMA)
  .ops          = &g_fcuart_rxdma_ops,
#  elif !defined(CONFIG_FCUART11_RXDMA) && defined(CONFIG_FCUART11_TXDMA)
  .ops          = &g_fcuart_txdma_ops,
#  else
  .ops          = &g_fcuart_ops,
#  endif
  .priv         = &g_fcuart11priv,
};
#endif

/* This describes the state of the FC7300 FCUART12 port. */

#ifdef CONFIG_FC7300_FCUART12
static struct fc7300_uart_s g_fcuart12priv =
{
  .uartbase     = FC7300_FCUART12_BASE,
  .baud         = CONFIG_FCUART12_BAUD,
  .irq          = FC7300_IRQ_FCUART12,
  .parity       = CONFIG_FCUART12_PARITY,
  .bits         = CONFIG_FCUART12_BITS,
  .stopbits2    = CONFIG_FCUART12_2STOP,
#  if defined(CONFIG_SERIAL_IFLOWCONTROL) && defined(CONFIG_FCUART12_IFLOWCONTROL)
  .iflow        = true,
#  endif
#  if defined(CONFIG_SERIAL_OFLOWCONTROL) && defined(CONFIG_FCUART12_OFLOWCONTROL)
  .oflow        = true,
#  endif
#  if  defined(CONFIG_SERIAL_IFLOWCONTROL) && defined(CONFIG_FCUART12_IFLOWCONTROL)
  .rts_gpio     = PIN_FCUART12_RTS,
#  endif
#  if defined(CONFIG_SERIAL_OFLOWCONTROL) && defined(CONFIG_FCUART12_OFLOWCONTROL)
  .cts_gpio     = PIN_FCUART12_CTS,
#  endif

#  ifdef CONFIG_FCUART12_TXDMA
  .dma_txreqsrc = FC7300_DMA_REQ_FCUART12_TX,
  .dma_txchidx  = CONFIG_FCUART12_TXDMA_CHANNEL,
#  endif
#  ifdef CONFIG_FCUART12_RXDMA
  .dma_rxreqsrc = FC7300_DMA_REQ_FCUART12_RX,
  .dma_rxchidx  = CONFIG_FCUART12_RXDMA_CHANNEL,
  .rxfifo       = g_fcuart12rxfifo,
#  endif
};

static struct uart_dev_s g_fcuart12dev =
{
  .recv         =
    {
      .size     = CONFIG_FCUART12_RXBUFSIZE,
      .buffer   = g_fcuart12rxbuffer,
    },
  .xmit         =
    {
      .size     = CONFIG_FCUART12_TXBUFSIZE,
      .buffer   = g_fcuart12txbuffer,
    },
#  if defined(CONFIG_FCUART12_RXDMA) && defined(CONFIG_FCUART12_TXDMA)
  .ops          = &g_fcuart_rxtxdma_ops,
#  elif defined(CONFIG_FCUART12_RXDMA) && !defined(CONFIG_FCUART12_TXDMA)
  .ops          = &g_fcuart_rxdma_ops,
#  elif !defined(CONFIG_FCUART12_RXDMA) && defined(CONFIG_FCUART12_TXDMA)
  .ops          = &g_fcuart_txdma_ops,
#  else
  .ops          = &g_fcuart_ops,
#  endif
  .priv         = &g_fcuart12priv,
};
#endif

/* This describes the state of the FC7300 FCUART13 port. */

#ifdef CONFIG_FC7300_FCUART13
static struct fc7300_uart_s g_fcuart13priv =
{
  .uartbase     = FC7300_FCUART13_BASE,
  .baud         = CONFIG_FCUART13_BAUD,
  .irq          = FC7300_IRQ_FCUART13,
  .parity       = CONFIG_FCUART13_PARITY,
  .bits         = CONFIG_FCUART13_BITS,
  .stopbits2    = CONFIG_FCUART13_2STOP,
#  if defined(CONFIG_SERIAL_IFLOWCONTROL) && defined(CONFIG_FCUART13_IFLOWCONTROL)
  .iflow        = true,
#  endif
#  if defined(CONFIG_SERIAL_OFLOWCONTROL) && defined(CONFIG_FCUART13_OFLOWCONTROL)
  .oflow        = true,
#  endif
#  if  defined(CONFIG_SERIAL_IFLOWCONTROL) && defined(CONFIG_FCUART13_IFLOWCONTROL)
  .rts_gpio     = PIN_FCUART13_RTS,
#  endif
#  if defined(CONFIG_SERIAL_OFLOWCONTROL) && defined(CONFIG_FCUART13_OFLOWCONTROL)
  .cts_gpio     = PIN_FCUART13_CTS,
#  endif

#  ifdef CONFIG_FCUART13_TXDMA
  .dma_txreqsrc = FC7300_DMA_REQ_FCUART13_TX,
  .dma_txchidx  = CONFIG_FCUART13_TXDMA_CHANNEL,
#  endif
#  ifdef CONFIG_FCUART13_RXDMA
  .dma_rxreqsrc = FC7300_DMA_REQ_FCUART13_RX,
  .dma_rxchidx  = CONFIG_FCUART13_RXDMA_CHANNEL,
  .rxfifo       = g_fcuart13rxfifo,
#  endif
};

static struct uart_dev_s g_fcuart13dev =
{
  .recv         =
    {
      .size     = CONFIG_FCUART13_RXBUFSIZE,
      .buffer   = g_fcuart13rxbuffer,
    },
  .xmit         =
    {
      .size     = CONFIG_FCUART13_TXBUFSIZE,
      .buffer   = g_fcuart13txbuffer,
    },
#  if defined(CONFIG_FCUART13_RXDMA) && defined(CONFIG_FCUART13_TXDMA)
  .ops          = &g_fcuart_rxtxdma_ops,
#  elif defined(CONFIG_FCUART13_RXDMA) && !defined(CONFIG_FCUART13_TXDMA)
  .ops          = &g_fcuart_rxdma_ops,
#  elif !defined(CONFIG_FCUART13_RXDMA) && defined(CONFIG_FCUART13_TXDMA)
  .ops          = &g_fcuart_txdma_ops,
#  else
  .ops          = &g_fcuart_ops,
#  endif
  .priv         = &g_fcuart13priv,
};
#endif

/* This describes the state of the FC7300 FCUART14 port. */

#ifdef CONFIG_FC7300_FCUART14
static struct fc7300_uart_s g_fcuart14priv =
{
  .uartbase     = FC7300_FCUART14_BASE,
  .baud         = CONFIG_FCUART14_BAUD,
  .irq          = FC7300_IRQ_FCUART14,
  .parity       = CONFIG_FCUART14_PARITY,
  .bits         = CONFIG_FCUART14_BITS,
  .stopbits2    = CONFIG_FCUART14_2STOP,
#  if defined(CONFIG_SERIAL_IFLOWCONTROL) && defined(CONFIG_FCUART14_IFLOWCONTROL)
  .iflow        = true,
#  endif
#  if defined(CONFIG_SERIAL_OFLOWCONTROL) && defined(CONFIG_FCUART14_OFLOWCONTROL)
  .oflow        = true,
#  endif
#  if  defined(CONFIG_SERIAL_IFLOWCONTROL) && defined(CONFIG_FCUART14_IFLOWCONTROL)
  .rts_gpio     = PIN_FCUART14_RTS,
#  endif
#  if defined(CONFIG_SERIAL_OFLOWCONTROL) && defined(CONFIG_FCUART14_OFLOWCONTROL)
  .cts_gpio     = PIN_FCUART14_CTS,
#  endif

#  ifdef CONFIG_FCUART14_TXDMA
  .dma_txreqsrc = FC7300_DMA_REQ_FCUART14_TX,
  .dma_txchidx  = CONFIG_FCUART14_TXDMA_CHANNEL,
#  endif
#  ifdef CONFIG_FCUART14_RXDMA
  .dma_rxreqsrc = FC7300_DMA_REQ_FCUART14_RX,
  .dma_rxchidx  = CONFIG_FCUART14_RXDMA_CHANNEL,
  .rxfifo       = g_fcuart14rxfifo,
#  endif
};

static struct uart_dev_s g_fcuart14dev =
{
  .recv         =
    {
      .size     = CONFIG_FCUART14_RXBUFSIZE,
      .buffer   = g_fcuart14rxbuffer,
    },
  .xmit         =
    {
      .size     = CONFIG_FCUART14_TXBUFSIZE,
      .buffer   = g_fcuart14txbuffer,
    },
#  if defined(CONFIG_FCUART14_RXDMA) && defined(CONFIG_FCUART14_TXDMA)
  .ops          = &g_fcuart_rxtxdma_ops,
#  elif defined(CONFIG_FCUART14_RXDMA) && !defined(CONFIG_FCUART14_TXDMA)
  .ops          = &g_fcuart_rxdma_ops,
#  elif !defined(CONFIG_FCUART14_RXDMA) && defined(CONFIG_FCUART14_TXDMA)
  .ops          = &g_fcuart_txdma_ops,
#  else
  .ops          = &g_fcuart_ops,
#  endif
  .priv         = &g_fcuart14priv,
};
#endif

/* This describes the state of the FC7300 FCUART15 port. */

#ifdef CONFIG_FC7300_FCUART15
static struct fc7300_uart_s g_fcuart15priv =
{
  .uartbase     = FC7300_FCUART15_BASE,
  .baud         = CONFIG_FCUART15_BAUD,
  .irq          = FC7300_IRQ_FCUART15,
  .parity       = CONFIG_FCUART15_PARITY,
  .bits         = CONFIG_FCUART15_BITS,
  .stopbits2    = CONFIG_FCUART15_2STOP,
#  if defined(CONFIG_SERIAL_IFLOWCONTROL) && defined(CONFIG_FCUART15_IFLOWCONTROL)
  .iflow        = true,
#  endif
#  if defined(CONFIG_SERIAL_OFLOWCONTROL) && defined(CONFIG_FCUART15_OFLOWCONTROL)
  .oflow        = true,
#  endif
#  if  defined(CONFIG_SERIAL_IFLOWCONTROL) && defined(CONFIG_FCUART15_IFLOWCONTROL)
  .rts_gpio     = PIN_FCUART15_RTS,
#  endif
#  if defined(CONFIG_SERIAL_OFLOWCONTROL) && defined(CONFIG_FCUART15_OFLOWCONTROL)
  .cts_gpio     = PIN_FCUART15_CTS,
#  endif

#  ifdef CONFIG_FCUART15_TXDMA
  .dma_txreqsrc = FC7300_DMA_REQ_FCUART15_TX,
  .dma_txchidx  = CONFIG_FCUART15_TXDMA_CHANNEL,
#  endif
#  ifdef CONFIG_FCUART15_RXDMA
  .dma_rxreqsrc = FC7300_DMA_REQ_FCUART15_RX,
  .dma_rxchidx  = CONFIG_FCUART15_RXDMA_CHANNEL,
  .rxfifo       = g_fcuart15rxfifo,
#  endif
};

static struct uart_dev_s g_fcuart15dev =
{
  .recv         =
    {
      .size     = CONFIG_FCUART15_RXBUFSIZE,
      .buffer   = g_fcuart15rxbuffer,
    },
  .xmit         =
    {
      .size     = CONFIG_FCUART15_TXBUFSIZE,
      .buffer   = g_fcuart15txbuffer,
    },
#  if defined(CONFIG_FCUART15_RXDMA) && defined(CONFIG_FCUART15_TXDMA)
  .ops          = &g_fcuart_rxtxdma_ops,
#  elif defined(CONFIG_FCUART15_RXDMA) && !defined(CONFIG_FCUART15_TXDMA)
  .ops          = &g_fcuart_rxdma_ops,
#  elif !defined(CONFIG_FCUART15_RXDMA) && defined(CONFIG_FCUART15_TXDMA)
  .ops          = &g_fcuart_txdma_ops,
#  else
  .ops          = &g_fcuart_ops,
#  endif
  .priv         = &g_fcuart15priv,
};
#endif

/* This describes the state of the FC7300 FCUART16 port. */

#ifdef CONFIG_FC7300_FCUART16
static struct fc7300_uart_s g_fcuart16priv =
{
  .uartbase     = FC7300_FCUART16_BASE,
  .baud         = CONFIG_FCUART16_BAUD,
  .irq          = FC7300_IRQ_FCUART16,
  .parity       = CONFIG_FCUART16_PARITY,
  .bits         = CONFIG_FCUART16_BITS,
  .stopbits2    = CONFIG_FCUART16_2STOP,
#  if defined(CONFIG_SERIAL_IFLOWCONTROL) && defined(CONFIG_FCUART16_IFLOWCONTROL)
  .iflow        = true,
#  endif
#  if defined(CONFIG_SERIAL_OFLOWCONTROL) && defined(CONFIG_FCUART16_OFLOWCONTROL)
  .oflow        = true,
#  endif
#  if  defined(CONFIG_SERIAL_IFLOWCONTROL) && defined(CONFIG_FCUART16_IFLOWCONTROL)
  .rts_gpio     = PIN_FCUART16_RTS,
#  endif
#  if defined(CONFIG_SERIAL_OFLOWCONTROL) && defined(CONFIG_FCUART16_OFLOWCONTROL)
  .cts_gpio     = PIN_FCUART16_CTS,
#  endif

#  ifdef CONFIG_FCUART16_TXDMA
  .dma_txreqsrc = FC7300_DMA_REQ_FCUART16_TX,
  .dma_txchidx  = CONFIG_FCUART16_TXDMA_CHANNEL,
#  endif
#  ifdef CONFIG_FCUART16_RXDMA
  .dma_rxreqsrc = FC7300_DMA_REQ_FCUART16_RX,
  .dma_rxchidx  = CONFIG_FCUART16_RXDMA_CHANNEL,
  .rxfifo       = g_fcuart16rxfifo,
#  endif
};

static struct uart_dev_s g_fcuart16dev =
{
  .recv         =
    {
      .size     = CONFIG_FCUART16_RXBUFSIZE,
      .buffer   = g_fcuart16rxbuffer,
    },
  .xmit         =
    {
      .size     = CONFIG_FCUART16_TXBUFSIZE,
      .buffer   = g_fcuart16txbuffer,
    },
#  if defined(CONFIG_FCUART16_RXDMA) && defined(CONFIG_FCUART16_TXDMA)
  .ops          = &g_fcuart_rxtxdma_ops,
#  elif defined(CONFIG_FCUART16_RXDMA) && !defined(CONFIG_FCUART16_TXDMA)
  .ops          = &g_fcuart_rxdma_ops,
#  elif !defined(CONFIG_FCUART16_RXDMA) && defined(CONFIG_FCUART16_TXDMA)
  .ops          = &g_fcuart_txdma_ops,
#  else
  .ops          = &g_fcuart_ops,
#  endif
  .priv         = &g_fcuart16priv,
};
#endif

/* This describes the state of the FC7300 FCUART17 port. */

#ifdef CONFIG_FC7300_FCUART17
static struct fc7300_uart_s g_fcuart17priv =
{
  .uartbase     = FC7300_FCUART17_BASE,
  .baud         = CONFIG_FCUART17_BAUD,
  .irq          = FC7300_IRQ_FCUART17,
  .parity       = CONFIG_FCUART17_PARITY,
  .bits         = CONFIG_FCUART17_BITS,
  .stopbits2    = CONFIG_FCUART17_2STOP,
#  if defined(CONFIG_SERIAL_IFLOWCONTROL) && defined(CONFIG_FCUART17_IFLOWCONTROL)
  .iflow        = true,
#  endif
#  if defined(CONFIG_SERIAL_OFLOWCONTROL) && defined(CONFIG_FCUART17_OFLOWCONTROL)
  .oflow        = true,
#  endif
#  if  defined(CONFIG_SERIAL_IFLOWCONTROL) && defined(CONFIG_FCUART17_IFLOWCONTROL)
  .rts_gpio     = PIN_FCUART17_RTS,
#  endif
#  if defined(CONFIG_SERIAL_OFLOWCONTROL) && defined(CONFIG_FCUART17_OFLOWCONTROL)
  .cts_gpio     = PIN_FCUART17_CTS,
#  endif

#  ifdef CONFIG_FCUART17_TXDMA
  .dma_txreqsrc = FC7300_DMA_REQ_FCUART17_TX,
  .dma_txchidx  = CONFIG_FCUART17_TXDMA_CHANNEL,
#  endif
#  ifdef CONFIG_FCUART17_RXDMA
  .dma_rxreqsrc = FC7300_DMA_REQ_FCUART17_RX,
  .dma_rxchidx  = CONFIG_FCUART17_RXDMA_CHANNEL,
  .rxfifo       = g_fcuart17rxfifo,
#  endif
};

static struct uart_dev_s g_fcuart17dev =
{
  .recv         =
    {
      .size     = CONFIG_FCUART17_RXBUFSIZE,
      .buffer   = g_fcuart17rxbuffer,
    },
  .xmit         =
    {
      .size     = CONFIG_FCUART17_TXBUFSIZE,
      .buffer   = g_fcuart17txbuffer,
    },
#  if defined(CONFIG_FCUART17_RXDMA) && defined(CONFIG_FCUART17_TXDMA)
  .ops          = &g_fcuart_rxtxdma_ops,
#  elif defined(CONFIG_FCUART17_RXDMA) && !defined(CONFIG_FCUART17_TXDMA)
  .ops          = &g_fcuart_rxdma_ops,
#  elif !defined(CONFIG_FCUART17_RXDMA) && defined(CONFIG_FCUART17_TXDMA)
  .ops          = &g_fcuart_txdma_ops,
#  else
  .ops          = &g_fcuart_ops,
#  endif
  .priv         = &g_fcuart17priv,
};
#endif

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: fc7300_serialin
 ****************************************************************************/

static inline uint32_t fc7300_serialin(struct fc7300_uart_s *priv,
                                      uint32_t offset)
{
  return getreg32(priv->uartbase + offset);
}

/****************************************************************************
 * Name: fc7300_serialout
 ****************************************************************************/

static inline void fc7300_serialout(struct fc7300_uart_s *priv,
                                     uint32_t offset, uint32_t value)
{
  putreg32(value, priv->uartbase + offset);
}

/****************************************************************************
 * Name: fc7300_dma_nextrx
 *
 * Description:
 *   Returns the index into the RX FIFO where the DMA will place the next
 *   byte that it receives.
 *
 ****************************************************************************/

#ifdef SERIAL_HAVE_RXDMA
static int fc7300_dma_nextrx(struct fc7300_uart_s *priv)
{
  int dmaresidual = DMA_RESIDUAL(priv->rxdma);

  return RXDMA_BUFFER_SIZE - dmaresidual;
}
#endif

/****************************************************************************
 * Name: fc7300_disableuartint
 ****************************************************************************/

static inline void fc7300_disableuartint(struct fc7300_uart_s *priv,
                                          uint32_t *ie)
{
  irqstate_t flags;
  uint32_t regval;

  flags  = spin_lock_irqsave(NULL);
  regval = fc7300_serialin(priv, FC7300_FCUART_CTRL_OFFSET);

  /* Return the current Rx and Tx interrupt state */

  if (ie != NULL)
    {
      *ie = regval & FCUART_ALL_INTS;
    }

  regval &= ~FCUART_ALL_INTS;
  fc7300_serialout(priv, FC7300_FCUART_CTRL_OFFSET, regval);
  spin_unlock_irqrestore(NULL, flags);
}

/****************************************************************************
 * Name: fc7300_restoreuartint
 ****************************************************************************/

static inline void fc7300_restoreuartint(struct fc7300_uart_s *priv,
                                        uint32_t ie)
{
  irqstate_t flags;
  uint32_t regval;

  /* Enable/disable any interrupts that are currently disabled but should be
   * enabled/disabled.
   */

  flags   = spin_lock_irqsave(NULL);
  regval  = fc7300_serialin(priv, FC7300_FCUART_CTRL_OFFSET);
  regval &= ~FCUART_ALL_INTS;
  regval |= ie;
  fc7300_serialout(priv, FC7300_FCUART_CTRL_OFFSET, regval);
  spin_unlock_irqrestore(NULL, flags);
}

/****************************************************************************
 * Name: fc7300_dma_setup
 *
 * Description:
 *   Configure the FCUART baud, bits, parity, etc. This method is called the
 *   first time that the serial port is opened.
 *
 ****************************************************************************/
#if defined(SERIAL_HAVE_RXDMA) || defined(SERIAL_HAVE_TXDMA)
static int fc7300_dma_setup(struct uart_dev_s *dev)
{
  struct fc7300_uart_s *priv = (struct fc7300_uart_s *)dev->priv;
  struct dma_dev_s *dma_dev  = fc7300_dma_getdev(DMA_INSTANCE_0);
#if defined(SERIAL_HAVE_RXDMA)
  struct dma_config_s config;
#endif
  int result;

  /* Do the basic UART setup first, unless we are the console */

  if (!dev->isconsole)
    {
      result = fc7300_setup(dev);
      if (result != OK)
        {
          return result;
        }
    }

#if defined(SERIAL_HAVE_TXDMA)
  /* Acquire the Tx DMA channel.  This should always succeed. */

  if (priv->dma_txreqsrc != 0)
    {
      if (priv->txdma == NULL)
        {
          priv->txdma = DMA_GET_CHAN(dma_dev, priv->dma_txchidx);
          if (priv->txdma == NULL)
            {
              return -EBUSY;
            }

          nxsem_init(&priv->txdmasem, 0, 1);
        }

      /* Enable Tx DMA for the UART */

      modifyreg32(priv->uartbase + FC7300_FCUART_BAUD_OFFSET,
                  0, FCUART_BAUD_TDMAEN);
    }
#endif

#if defined(SERIAL_HAVE_RXDMA)
  /* Acquire the Rx DMA channel.  This should always succeed. */

  if (priv->dma_rxreqsrc != 0)
    {
      if (priv->rxdma == NULL)
        {
          priv->rxdma = DMA_GET_CHAN(dma_dev, priv->dma_rxchidx);
          if (priv->rxdma == NULL)
            {
              return -EBUSY;
            }
        }
      else
        {
          DMA_STOP(priv->rxdma);
        }

      /* Configure for circular DMA reception into the RX FIFO */

      config.src_width = 1;
      config.dst_width = 1;
      config.src_drq   = priv->dma_rxreqsrc;
      config.src_step  = 0;
      config.dst_step  = 1;
      config.option    = DMA_CONFIG_LOOPDEST |
                         DMA_CONFIG_INTHALF  |
                         DMA_CONFIG_INTOUTER;

      DMA_CONFIG(priv->rxdma , &config);

      /* Reset our DMA shadow pointer and Rx data availability count to
       * match the address just programmed above.
       */

      priv->rxdmanext = 0;

      /* Enable receive Rx DMA for the UART */

      modifyreg32(priv->uartbase + FC7300_FCUART_BAUD_OFFSET,
                  0, FCUART_BAUD_RDMAEN);

      /* Enable interrupt on idle and errors */

      priv->ie |= FCUART_CTRL_PEIE |
                  FCUART_CTRL_FEIE |
                  FCUART_CTRL_NEIE |
                  FCUART_CTRL_IIE;

      modifyreg32(priv->uartbase + FC7300_FCUART_CTRL_OFFSET, 0,
                  FCUART_CTRL_PEIE |
                  FCUART_CTRL_FEIE |
                  FCUART_CTRL_NEIE |
                  FCUART_CTRL_IIE  |
                  FCUART_CTRL_ITS);

      /* Start the DMA channel, and arrange for callbacks at the half and
       * full points in the FIFO.  This ensures that we have half a FIFO
       * worth of time to claim bytes before they are overwritten.
       */

      DMA_START_CYCLIC(priv->rxdma, fc7300_dma_rxcallback, (void *)dev,
                       (uintptr_t)priv->rxfifo,
                       (uintptr_t)(priv->uartbase +
                                   FC7300_FCUART_DATA_OFFSET),
                       RXDMA_BUFFER_SIZE, 1);
    }
#endif

  return OK;
}
#endif

/****************************************************************************
 * Name: fc7300_fcuart_configure
 *
 * Description:
 *   Configure a UART for non-interrupt driven operation
 *
 ****************************************************************************/

static int fc7300_fcuart_configure(struct fc7300_uart_s *priv)
{
  uint32_t osr;
  uint16_t sbr;
  uint32_t regval;
  int ret;

  ret = fc7300_fcuart_processbaud(priv->uartbase, priv->baud, &osr, &sbr);

  if (ret != OK)
    {
      return ret;
    }

  /* Reset all internal logic and registers, except the Global Register */

  regval  = fc7300_serialin(priv, FC7300_FCUART_RST_OFFSET);
  regval |= FCUART_RST_RST;
  fc7300_serialout(priv, FC7300_FCUART_RST_OFFSET, regval);

  regval &= ~FCUART_RST_RST;
  fc7300_serialout(priv, FC7300_FCUART_RST_OFFSET, regval);

  /* Configure MODIR register */

  regval = 0;

#ifdef CONFIG_SERIAL_OFLOWCONTROL
  /* Flow control on outbound side */

  if (priv->oflow)
    {
      regval |= FCUART_MODIR_TXCTSEN;
    }
#endif

#ifdef CONFIG_SERIAL_IFLOWCONTROL
  /* Flow control on inbound side if not GPIO based */

  if ((priv->iflow) &&
      ((priv->rts_gpio & _PIN_MODE_MASK) != _PIN_MODE_GPIO))
    {
      regval |= FCUART_MODIR_RXRTSEN;
    }
#endif

  fc7300_serialout(priv, FC7300_FCUART_MODIR_OFFSET, regval);

  /* Configure BAUD register */

  regval = 0;

  if ((osr > 3) && (osr < 8))
    {
      regval |= FCUART_BAUD_BEDGE_SAMP;
    }

  if (priv->stopbits2)
    {
      regval |= FCUART_BAUD_SBNS;
    }

  regval |= FCUART_BAUD_OVR_SAMP(osr - 1) | FCUART_BAUD_SBR(sbr);
  fc7300_serialout(priv, FC7300_FCUART_BAUD_OFFSET, regval);

  /* Configure CTRL register */

  regval = 0;
  if (priv->parity == 1)
    {
      regval |= FCUART_CTRL_PE | FCUART_CTRL_PT_ODD;
    }
  else if (priv->parity == 2)
    {
      regval |= FCUART_CTRL_PE | FCUART_CTRL_PT_EVEN;
    }

  if (priv->bits == 7)
    {
      regval |= FCUART_CTRL_7BMS;
    }
  else if (priv->bits == 8)
    {
      regval |= FCUART_CTRL_BMSEL_8BIT;
    }
  else if (priv->bits == 9)
    {
      regval |= FCUART_CTRL_BMSEL_9BIT;
    }
  else
    {
      return -ENOSYS;
    }

  regval |= FCUART_CTRL_RE | FCUART_CTRL_TE;
  fc7300_serialout(priv, FC7300_FCUART_CTRL_OFFSET, regval);

  return OK;
}

/****************************************************************************
 * Name: fc7300_setup
 *
 * Description:
 *   Configure the UART baud, bits, parity, fifos, etc. This
 *   method is called the first time that the serial port is
 *   opened.
 *
 ****************************************************************************/

static int fc7300_setup(struct uart_dev_s *dev)
{
  struct fc7300_uart_s *priv = (struct fc7300_uart_s *)dev->priv;
  int ret;

  /* configure the FCUART */

  ret = fc7300_fcuart_configure(priv);

  /* get the current interrupt bits and place them in ie */

  priv->ie = fc7300_serialin(priv, FC7300_FCUART_CTRL_OFFSET) & \
             FCUART_ALL_INTS;
  return ret;
}

/****************************************************************************
 * Name: fc7300_shutdown
 *
 * Description:
 *   Disable the UART.  This method is called when the serial
 *   port is closed
 *
 ****************************************************************************/

static void fc7300_shutdown(struct uart_dev_s *dev)
{
  struct fc7300_uart_s *priv = (struct fc7300_uart_s *)dev->priv;

  /* Disable the UART */

  /* set the reset bit  */

  fc7300_serialout(priv, FC7300_FCUART_RST_OFFSET, FCUART_RST_RST);

  /* clear the reset bit again */

  fc7300_serialout(priv, FC7300_FCUART_RST_OFFSET, 0);
}

/****************************************************************************
 * Name: fc7300_dma_shutdown
 *
 * Description:
 *   Disable the FCUART.  This method is called when the serial
 *   port is closed
 *
 ****************************************************************************/

#if defined(SERIAL_HAVE_RXDMA) || defined(SERIAL_HAVE_TXDMA)
static void fc7300_dma_shutdown(struct uart_dev_s *dev)
{
  struct fc7300_uart_s *priv = (struct fc7300_uart_s *)dev->priv;
  struct dma_dev_s *dma_dev  = fc7300_dma_getdev(DMA_INSTANCE_0);

  /* Perform the normal UART shutdown */

  fc7300_shutdown(dev);

#if defined(SERIAL_HAVE_RXDMA)
  /* Stop the RX DMA channel */

  if (priv->dma_rxreqsrc != 0)
    {
      DMA_STOP(priv->rxdma);

      /* Release the RX DMA channel */

      DMA_PUT_CHAN(dma_dev, priv->rxdma);
      priv->rxdma = NULL;
    }
#endif

#if defined(SERIAL_HAVE_TXDMA)
  /* Stop the TX DMA channel */

  if (priv->dma_txreqsrc != 0)
    {
      DMA_STOP(priv->txdma);

      /* Release the TX DMA channel */

      DMA_PUT_CHAN(dma_dev, priv->txdma);
      priv->txdma = NULL;
      nxsem_destroy(&priv->txdmasem);
    }
#endif
}
#endif

/****************************************************************************
 * Name: fc7300_attach
 *
 * Description:
 *   Configure the UART to operation in interrupt driven mode.  This method
 *   is called when the serial port is opened.  Normally, this is just after
 *   the setup() method is called, however, the serial console may operate
 *   in a non-interrupt driven mode during the boot phase.
 *
 *   RX and TX interrupts are not enabled when by the attach method (unless
 *   the hardware supports multiple levels of interrupt enabling).  The RX
 *   and TX interrupts are not enabled until the txint() and rxint() methods
 *   are called.
 *
 ****************************************************************************/

static int fc7300_attach(struct uart_dev_s *dev)
{
  struct fc7300_uart_s *priv = (struct fc7300_uart_s *)dev->priv;
  int ret;

  /* Attach and enable the IRQ */

#ifdef CONFIG_FC7300_FCUART_ISR_THREAD
  ret = irq_attach_wqueue(priv->irq, fc7300_fcuart_isr,
                          fc7300_interrupt, dev,
                          isr_cfg[FCUART_ISR_THREAD]);
#else
  ret = irq_attach(priv->irq, fc7300_interrupt, dev);
#endif
  if (ret == OK)
    {
      /* Enable the interrupt (RX and TX interrupts are still disabled
       * in the UART
       */

#ifdef CONFIG_ARCH_IRQPRIO
      up_prioritize_irq(priv->irq, NVIC_FCUART_PRIORITY_DEFAULT);
#endif
      up_enable_irq(priv->irq);
    }

  return ret;
}

/****************************************************************************
 * Name: fc7300_detach
 *
 * Description:
 *   Detach UART interrupts.  This method is called when the serial port is
 *   closed normally just before the shutdown method is called.  The
 *   exception is the serial console which is never shutdown.
 *
 ****************************************************************************/

static void fc7300_detach(struct uart_dev_s *dev)
{
  struct fc7300_uart_s *priv = (struct fc7300_uart_s *)dev->priv;

  up_disable_irq(priv->irq);
  irq_detach(priv->irq);
}

#ifdef CONFIG_FC7300_FCUART_ISR_THREAD
/****************************************************************************
 * Function: fc7300_fcuart_isr
 *
 * Description:
 *   The top half of the interrupt handler when thread isr is on.
 *
 * Input Parameters:
 *   irq     - Number of the IRQ that generated the interrupt
 *   context - Interrupt register state save info (architecture-specific)
 *   arg     - driver private parameter
 *
 * Returned Value:
 *   OK(0) on success; Negated errno on failure.
 *
 * Assumptions:
 *
 ****************************************************************************/

static int fc7300_fcuart_isr(int irq, void *context, void *arg)
{
  (void)context;
  (void)arg;
  up_disable_irq(irq);
  return IRQ_WAKE_THREAD;
}
#endif

/****************************************************************************
 * Name: fc7300_interrupt (and front-ends)
 *
 * Description:
 *   This is the common UART interrupt handler.  It will be invoked when an
 *   interrupt is received on the 'irq'.  It should call uart_xmitchars or
 *   uart_recvchars to perform the appropriate data transfers.  The
 *   interrupt handling logic must be able to map the 'arg' to the
 *   appropriate uart_dev_s structure in order to call these functions.
 *
 ****************************************************************************/

__ramfunc__ static int fc7300_interrupt(int irq, void *context, void *arg)
{
  struct uart_dev_s *dev = (struct uart_dev_s *)arg;
  struct fc7300_uart_s *priv;
  uint32_t usr;
  int passes = 0;
  bool handled;

  DEBUGASSERT(dev != NULL && dev->priv != NULL);
  priv = (struct fc7300_uart_s *)dev->priv;

  /* Loop until there are no characters to be transferred or,
   * until we have been looping for a long time.
   */

  handled = true;
  for (passes = 0; passes < 256 && handled; passes++)
    {
      handled = false;

      /* Get the current UART status and check for loop
       * termination conditions
       */

      usr  = fc7300_serialin(priv, FC7300_FCUART_STAT_OFFSET);
      usr &= (FCUART_STAT_RDRFF | FCUART_STAT_TDREF | FCUART_STAT_RORF |
              FCUART_STAT_FEF | FCUART_STAT_NF | FCUART_STAT_PEF |
              FCUART_STAT_IDLEF);

      /* Clear serial overrun, parity and framing errors */

      if ((usr & FCUART_STAT_RORF) != 0)
        {
          fc7300_serialout(priv, FC7300_FCUART_STAT_OFFSET,
                            FCUART_STAT_RORF);
        }

      if ((usr & FCUART_STAT_NF) != 0)
        {
          fc7300_serialout(priv, FC7300_FCUART_STAT_OFFSET,
                            FCUART_STAT_NF);
        }

      if ((usr & FCUART_STAT_PEF) != 0)
        {
          fc7300_serialout(priv, FC7300_FCUART_STAT_OFFSET,
                            FCUART_STAT_PEF);
        }

      if ((usr & FCUART_STAT_FEF) != 0)
        {
          fc7300_serialout(priv, FC7300_FCUART_STAT_OFFSET,
                            FCUART_STAT_FEF);
        }

      if ((usr & (FCUART_STAT_FEF | FCUART_STAT_PEF | FCUART_STAT_NF)) != 0)
        {
          /* Discard data */

          fc7300_serialin(priv, FC7300_FCUART_DATA_OFFSET);
        }

#ifdef SERIAL_HAVE_RXDMA
      /* The line going to idle, deliver any fractions of RX data */

      if ((usr & FCUART_STAT_IDLEF) != 0)
        {
          fc7300_serialout(priv, FC7300_FCUART_STAT_OFFSET,
                            FCUART_STAT_IDLEF);
          fc7300_dma_rxcallback(priv->rxdma, dev, 0);
        }
#endif

      /* Handle incoming, receive bytes */

      if ((usr & FCUART_STAT_RDRFF) != 0 &&
          (priv->ie & FCUART_CTRL_RIE) != 0)
        {
          uart_recvchars(dev);
          handled = true;
        }

      /* Handle outgoing, transmit bytes */

      if ((usr & FCUART_STAT_TDREF) != 0 &&
          (priv->ie & FCUART_CTRL_TIE) != 0)
        {
          uart_xmitchars(dev);
          handled = true;
        }
    }

#ifdef CONFIG_FC7300_FCUART_ISR_THREAD
  fc7300_clearpendingisr(irq);
  up_enable_irq(irq);
#endif

  return OK;
}

/****************************************************************************
 * Name: fc7300_ioctl
 *
 * Description:
 *   All ioctl calls will be routed through this method
 *
 ****************************************************************************/

static int fc7300_ioctl(struct file *filep, int cmd, unsigned long arg)
{
#if defined(CONFIG_SERIAL_TIOCSERGSTRUCT) || defined(CONFIG_SERIAL_TERMIOS)
  struct inode *inode = filep->f_inode;
  struct uart_dev_s *dev = inode->i_private;
  irqstate_t flags;
#endif
  int ret   = OK;

  switch (cmd)
    {
#ifdef CONFIG_SERIAL_TIOCSERGSTRUCT
    case TIOCSERGSTRUCT:
      {
         struct fc7300_uart_s *user = (struct fc7300_uart_s *)arg;
         struct fc7300_uart_s *priv = (struct fc7300_uart_s *)dev->priv;
         if (!user)
           {
             ret = -EINVAL;
           }
         else
           {
             memcpy(user, priv, sizeof(struct fc7300_uart_s));
           }
       }
       break;
#endif

#ifdef CONFIG_SERIAL_TERMIOS
    case TCGETS:
      {
        struct termios  *termiosp = (struct termios *)arg;
        struct fc7300_uart_s *priv = (struct fc7300_uart_s *)dev->priv;

        if (!termiosp)
          {
            ret = -EINVAL;
            break;
          }

        /* Return parity */

        termiosp->c_cflag = ((priv->parity != 0) ? PARENB : 0) |
                            ((priv->parity == 1) ? PARODD : 0);

        /* Return stop bits */

        termiosp->c_cflag |= (priv->stopbits2) ? CSTOPB : 0;

        /* Return flow control */

#ifdef CONFIG_SERIAL_OFLOWCONTROL
        termiosp->c_cflag |= ((priv->oflow) ? CCTS_OFLOW : 0);
#endif
#ifdef CONFIG_SERIAL_IFLOWCONTROL
        termiosp->c_cflag |= ((priv->iflow) ? CRTS_IFLOW : 0);
#endif
        /* Return baud */

        cfsetispeed(termiosp, priv->baud);

        /* Return number of bits */

        switch (priv->bits)
          {
          case 5:
            termiosp->c_cflag |= CS5;
            break;

          case 6:
            termiosp->c_cflag |= CS6;
            break;

          case 7:
            termiosp->c_cflag |= CS7;
            break;

          default:
          case 8:
            termiosp->c_cflag |= CS8;
            break;

          case 9:
            termiosp->c_cflag |= CS8; /* CS9 */
            break;
          }
      }
      break;

    case TCSETS:
      {
        struct termios  *termiosp = (struct termios *)arg;
        struct fc7300_uart_s *priv = (struct fc7300_uart_s *)dev->priv;
        uint32_t baud;
        uint32_t ie;
        uint8_t parity;
        uint8_t nbits;
        bool stop2;

        if ((!termiosp)
#ifdef CONFIG_SERIAL_OFLOWCONTROL
            || ((termiosp->c_cflag & CCTS_OFLOW) && (priv->cts_gpio == 0))
#endif
#ifdef CONFIG_SERIAL_IFLOWCONTROL
            || ((termiosp->c_cflag & CRTS_IFLOW) && (priv->rts_gpio == 0))
#endif
           )
          {
            ret = -EINVAL;
            break;
          }

        /* Decode baud. */

        ret = OK;
        baud = cfgetispeed(termiosp);

        /* Decode number of bits */

        switch (termiosp->c_cflag & CSIZE)
          {
          case CS5:
            nbits = 5;
            break;

          case CS6:
            nbits = 6;
            break;

          case CS7:
            nbits = 7;
            break;

          case CS8:
            nbits = 8;
            break;
#if 0
          case CS9:
            nbits = 9;
            break;
#endif
          default:
            ret = -EINVAL;
            break;
          }

        /* Decode parity */

        if ((termiosp->c_cflag & PARENB) != 0)
          {
            parity = (termiosp->c_cflag & PARODD) ? 1 : 2;
          }
        else
          {
            parity = 0;
          }

        /* Decode stop bits */

        stop2 = (termiosp->c_cflag & CSTOPB) != 0;

        /* Verify that all settings are valid before committing */

        if (ret == OK)
          {
            /* Commit */

            priv->baud      = baud;
            priv->parity    = parity;
            priv->bits      = nbits;
            priv->stopbits2 = stop2;
#ifdef CONFIG_SERIAL_OFLOWCONTROL
            priv->oflow     = (termiosp->c_cflag & CCTS_OFLOW) != 0;
#endif
#ifdef CONFIG_SERIAL_IFLOWCONTROL
            priv->iflow     = (termiosp->c_cflag & CRTS_IFLOW) != 0;
#endif
            /* effect the changes immediately - note that we do not
             * implement TCSADRAIN / TCSAFLUSH
             */

            flags = spin_lock_irqsave(NULL);
            fc7300_disableuartint(priv, &ie);
            ret = dev->ops->setup(dev);

            /* Restore the interrupt state */

            fc7300_restoreuartint(priv, ie);
            priv->ie = ie;
            spin_unlock_irqrestore(NULL, flags);
          }
      }
      break;
#endif /* CONFIG_SERIAL_TERMIOS */

#ifdef CONFIG_FC7300_FCUART_INVERT
    case TIOCSINVERT:
      {
        uint32_t ctrl;
        uint32_t stat;
        uint32_t regval;
        irqstate_t flags;
        struct fc7300_uart_s *priv = (struct fc7300_uart_s *)dev->priv;

        flags  = spin_lock_irqsave(NULL);
        ctrl   = fc7300_serialin(priv, FC7300_FCUART_CTRL_OFFSET);
        stat   = fc7300_serialin(priv, FC7300_FCUART_STAT_OFFSET);
        regval = ctrl;

        /* {R|T}XINV bit field can only be written when the receiver is
        * disabled (RE=0).
        */

        regval &= ~FCUART_CTRL_RE;

        fc7300_serialout(priv, FC7300_FCUART_CTRL_OFFSET, regval);

        /* Enable/disable signal inversion. */

        if (arg & SER_INVERT_ENABLED_RX)
          {
            stat |= FCUART_STAT_RXINV;
          }
        else
          {
            stat &= ~FCUART_STAT_RXINV;
          }

        if (arg & SER_INVERT_ENABLED_TX)
          {
            ctrl |= FCUART_CTRL_TXINV;
          }
        else
          {
            ctrl &= ~FCUART_CTRL_TXINV;
          }

        fc7300_serialout(priv, FC7300_FCUART_STAT_OFFSET, stat);
        fc7300_serialout(priv, FC7300_FCUART_CTRL_OFFSET, ctrl);

        spin_unlock_irqrestore(NULL, flags);
      }
      break;
#endif

    case TIOCSBRK:  /* BSD compatibility: Turn break on, unconditionally */
    case TIOCCBRK:  /* BSD compatibility: Turn break off, unconditionally */
    default:
      ret = -ENOTTY;
      break;
    }

  return ret;
}

/****************************************************************************
 * Name: fc7300_receive
 *
 * Description:
 *   Called (usually) from the interrupt level to receive one
 *   character from the UART.  Error bits associated with the
 *   receipt are provided in the return 'status'.
 *
 ****************************************************************************/

#ifndef SERIAL_HAVE_ONLY_RXDMA
static int fc7300_receive(struct uart_dev_s *dev, unsigned int *status)
{
  struct fc7300_uart_s *priv = (struct fc7300_uart_s *)dev->priv;
  uint32_t rxd;

  rxd     = fc7300_serialin(priv, FC7300_FCUART_DATA_OFFSET);
  *status = rxd >> FCUART_DATA_STATUS_SHIFT;
  return (rxd & FCUART_DATA_RXTXDATA_MASK) >> FCUART_DATA_RXTXDATA_SHIFT;
}
#endif

/****************************************************************************
 * Name: fc7300_rxint
 *
 * Description:
 *   Call to enable or disable RX interrupts
 *
 ****************************************************************************/

#ifndef SERIAL_HAVE_ONLY_RXDMA
static void fc7300_rxint(struct uart_dev_s *dev, bool enable)
{
  struct fc7300_uart_s *priv = (struct fc7300_uart_s *)dev->priv;
  irqstate_t flags;
  uint32_t regval;

  /* Enable interrupts for data available at Rx */

  flags = spin_lock_irqsave(NULL);
  if (enable)
    {
#ifndef CONFIG_SUPPRESS_SERIAL_INTS
      priv->ie |= FCUART_CTRL_RIE | FCUART_CTRL_FEIE | FCUART_CTRL_ORIE;
#endif
    }
  else
    {
      priv->ie &= ~(FCUART_CTRL_RIE | FCUART_CTRL_FEIE | FCUART_CTRL_ORIE);
    }

  regval  = fc7300_serialin(priv, FC7300_FCUART_CTRL_OFFSET);
  regval &= ~FCUART_ALL_INTS;
  regval |= priv->ie;
  fc7300_serialout(priv, FC7300_FCUART_CTRL_OFFSET, regval);
  spin_unlock_irqrestore(NULL, flags);
}
#endif

/****************************************************************************
 * Name: fc7300_rxavailable
 *
 * Description:
 *   Return true if the receive fifo is not empty
 *
 ****************************************************************************/

#ifndef SERIAL_HAVE_ONLY_RXDMA
static bool fc7300_rxavailable(struct uart_dev_s *dev)
{
  struct fc7300_uart_s *priv = (struct fc7300_uart_s *)dev->priv;
  uint32_t regval;

  /* Return true is data is ready in the Rx FIFO */

  regval = fc7300_serialin(priv, FC7300_FCUART_STAT_OFFSET);
  return ((regval & FCUART_STAT_RDRFF) != 0);
}
#endif

/****************************************************************************
 * Name: fc7300_rxflowcontrol
 *
 * Description:
 *   Called when Rx buffer is full (or exceeds configured watermark levels
 *   if CONFIG_SERIAL_IFLOWCONTROL_WATERMARKS is defined).
 *   Return true if UART activated RX flow control to block more incoming
 *   data
 *
 * Input Parameters:
 *   dev       - UART device instance
 *   nbuffered - the number of characters currently buffered
 *               (if CONFIG_SERIAL_IFLOWCONTROL_WATERMARKS is
 *               not defined the value will be 0 for an empty buffer or the
 *               defined buffer size for a full buffer)
 *   upper     - true indicates the upper watermark was crossed where
 *               false indicates the lower watermark has been crossed
 *
 * Returned Value:
 *   true if RX flow control activated.
 *
 ****************************************************************************/

#ifdef CONFIG_SERIAL_IFLOWCONTROL
static bool fc7300_rxflowcontrol(struct uart_dev_s *dev,
                                  unsigned int nbuffered, bool upper)
{
  struct fc7300_uart_s *priv = (struct fc7300_uart_s *)dev->priv;
  bool use_swhs = false;

#if defined(CONFIG_SERIAL_IFLOWCONTROL_WATERMARKS)
  use_swhs = (priv->rts_gpio & _PIN_MODE_MASK) == _PIN_MODE_GPIO;
#endif

  if (use_swhs && priv->iflow && (priv->rts_gpio != 0))
    {
      /* Assert/de-assert nRTS set it high resume/stop sending */

      fc7300_gpiowrite(priv->rts_gpio, upper);

      if (upper)
        {
          /* With heavy Rx traffic, RXNE might be set and data pending.
           * Returning 'true' in such case would cause RXNE left unhandled
           * and causing interrupt storm. Sending end might be also be slow
           * to react on nRTS, and returning 'true' here would prevent
           * processing that data.
           *
           * Therefore, return 'false' so input data is still being processed
           * until sending end reacts on nRTS signal and stops sending more.
           */

          return false;
        }

      return upper;
    }
  else
    {
      /* Is the RX buffer full? */

      if (upper)
        {
          /* Disable Rx interrupt to prevent more data being from
           * peripheral.  When hardware RTS is enabled, this will
           * prevent more data from coming in.
           *
           * This function is only called when UART recv buffer is full,
           * that is: "dev->recv.head + 1 == dev->recv.tail".
           *
           * Logic in "uart_read" will automatically toggle Rx interrupts
           * when buffer is read empty and thus we do not have to re-
           * enable Rx interrupts.
           */

          uart_disablerxint(dev);
          return true;
        }

      /* No.. The RX buffer is empty */

      else
        {
          /* We might leave Rx interrupt disabled if full recv buffer was
           * read empty.  Enable Rx interrupt to make sure that more input is
           * received.
           */

          uart_enablerxint(dev);
        }
    }

  return false;
}
#endif

/****************************************************************************
 * Name: fc7300_dma_receive
 *
 * Description:
 *   Called (usually) from the interrupt level to receive one
 *   character from the FCUART.  Error bits associated with the
 *   receipt are provided in the return 'status'.
 *
 ****************************************************************************/

#ifdef SERIAL_HAVE_RXDMA
static int fc7300_dma_receive(struct uart_dev_s *dev, unsigned int *status)
{
  struct fc7300_uart_s *priv = (struct fc7300_uart_s *)dev->priv;
  uint32_t nextrx = fc7300_dma_nextrx(priv);
  int c = 0;

  /* Check if more data is available */

  if (nextrx != priv->rxdmanext)
    {
      /* Now read from the DMA buffer */

      c = priv->rxfifo[priv->rxdmanext];

      priv->rxdmanext++;

      if (priv->rxdmanext == RXDMA_BUFFER_SIZE)
        {
          priv->rxdmanext = 0;
        }
    }

  /* NOTE:  If no data is available, then we would return NULL which is,
   * of course, valid binary data.  The protocol is that the upper half
   * driver must call fc7300_dma_rxavailable prior to calling this
   * function to assure that this never happens.
   */

  return c;
}
#endif

/****************************************************************************
 * Name: fc7300_dma_rxint
 *
 * Description:
 *   Call to enable or disable RX interrupts
 *
 ****************************************************************************/

#ifdef SERIAL_HAVE_RXDMA
static void fc7300_dma_rxint(struct uart_dev_s *dev, bool enable)
{
  struct fc7300_uart_s *priv = (struct fc7300_uart_s *)dev->priv;

  /* Enable/disable DMA reception.
   *
   * Note that it is not safe to check for available bytes and immediately
   * pass them to uart_recvchars as that could potentially recurse back
   * to us again.  Instead, bytes must wait until the next up_dma_poll or
   * DMA event.
   */

  priv->rxenable = enable;
}
#endif

/****************************************************************************
 * Name: fc7300_dma_rxavailable
 *
 * Description:
 *   Return true if the receive register is not empty
 *
 ****************************************************************************/

#ifdef SERIAL_HAVE_RXDMA
static bool fc7300_dma_rxavailable(struct uart_dev_s *dev)
{
  struct fc7300_uart_s *priv = (struct fc7300_uart_s *)dev->priv;

  /* Compare our receive pointer to the current DMA pointer, if they
   * do not match, then there are bytes to be received.
   */

  return (fc7300_dma_nextrx(priv) != priv->rxdmanext);
}
#endif

/****************************************************************************
 * Name: fc7300_dma_txcallback
 *
 * Description:
 *   This function clears dma buffer at complete of DMA transfer and wakes up
 *   threads waiting for space in buffer.
 *
 ****************************************************************************/

#ifdef SERIAL_HAVE_TXDMA
static void fc7300_dma_txcallback(struct dma_chan_s *chan, void *arg,
                                  ssize_t len)
{
  struct uart_dev_s *dev = (struct uart_dev_s *)arg;
  struct fc7300_uart_s *priv = (struct fc7300_uart_s *)dev->priv;

  /* Update 'nbytes' indicating number of bytes actually transferred by DMA.
   * This is important to free TX buffer space by 'uart_xmitchars_done'.
   */

  dev->dmatx.nbytes += len;

  if ((dev->dmatx.nlength > 0) && (dev->dmatx.nbytes == len))
    {
      /* We need to stop DMA before restart */

      DMA_STOP(priv->txdma);

      /* Start transmission with the callback on DMA completion */

      DMA_START_CYCLIC(priv->txdma, fc7300_dma_txcallback, (void *)dev,
                        (uintptr_t)(priv->uartbase +
                                    FC7300_FCUART_DATA_OFFSET),
                        (uintptr_t)dev->dmatx.nbuffer,
                        dev->dmatx.nlength *
                        sizeof(dev->dmatx.nbuffer[0]),
                        sizeof(dev->dmatx.nbuffer[0]));
    }
  else
    {
      /* Adjust the pointers */

      uart_xmitchars_done(dev);

      /* Release waiter */

      nxsem_post(&priv->txdmasem);
    }
}
#endif

/****************************************************************************
 * Name: fc7300_dma_txavailable
 *
 * Description:
 *        Informs DMA that Tx data is available and is ready for transfer.
 *
 ****************************************************************************/

#ifdef SERIAL_HAVE_TXDMA
static void fc7300_dma_txavailable(struct uart_dev_s *dev)
{
  struct fc7300_uart_s *priv = (struct fc7300_uart_s *)dev->priv;

  /* Only send when the DMA is idle */

  nxsem_wait(&priv->txdmasem);

  if (dev->xmit.head != dev->xmit.tail)
    {
      uart_xmitchars_dma(dev);
    }
  else
    {
      nxsem_post(&priv->txdmasem);
    }
}
#endif

/****************************************************************************
 * Name: fc7300_dma_send
 *
 * Description:
 *   Called (usually) from the interrupt level to start DMA transfer.
 *   (Re-)Configures DMA Stream updating buffer and buffer length.
 *
 ****************************************************************************/

#ifdef SERIAL_HAVE_TXDMA
static void fc7300_dma_send(struct uart_dev_s *dev)
{
  struct fc7300_uart_s *priv = (struct fc7300_uart_s *)dev->priv;
  struct dma_config_s config;

  /* We need to stop DMA before reconfiguration */

  DMA_STOP(priv->txdma);

  /* Reset the number sent */

  dev->dmatx.nbytes = 0;

  /* Make use of setup function to update buffer and its length for next
   * transfer
   */

  config.src_width = 1;
  config.dst_width = 1;
  config.src_drq   = priv->dma_txreqsrc;
  config.src_step  = 1;
  config.dst_step  = 0;
  config.option    = DMA_CONFIG_INTOUTER | DMA_CONFIG_AUTOSTOP;

  /* Setup DMA */

  DMA_CONFIG(priv->txdma, &config);

  /* Start transmission with the callback on DMA completion */

  DMA_START_CYCLIC(priv->txdma, fc7300_dma_txcallback, (void *)dev,
                   (uintptr_t)(priv->uartbase + FC7300_FCUART_DATA_OFFSET),
                   (uintptr_t)dev->dmatx.buffer,
                   dev->dmatx.length * sizeof(dev->dmatx.buffer[0]),
                   sizeof(dev->dmatx.buffer[0]));
}
#endif

/****************************************************************************
 * Name: fc7300_send
 *
 * Description:
 *   This method will send one byte on the UART
 *
 ****************************************************************************/

static void fc7300_send(struct uart_dev_s *dev, int ch)
{
  struct fc7300_uart_s *priv = (struct fc7300_uart_s *)dev->priv;
  fc7300_serialout(priv, FC7300_FCUART_DATA_OFFSET, (uint32_t)ch);
}

/****************************************************************************
 * Name: fc7300_dma_txint
 *
 * Description:
 *   Call to enable or disable TX interrupts from the UART.
 *
 ****************************************************************************/

#ifdef SERIAL_HAVE_TXDMA
static void fc7300_dma_txint(struct uart_dev_s *dev, bool enable)
{
  /* Nothing to do. */

  /* In case of DMA transfer we do not want to make use of UART interrupts.
   * Instead, we use DMA interrupts that are activated once during boot
   * sequence. Furthermore we can use fc7300_dma_txcallback() to handle
   * stuff at half DMA transfer or after transfer completion (depending on
   * the configuration).
   */
}
#endif

/****************************************************************************
 * Name: fc7300_txint
 *
 * Description:
 *   Call to enable or disable TX interrupts
 *
 ****************************************************************************/

#if !defined(SERIAL_HAVE_ONLY_TXDMA)
static void fc7300_txint(struct uart_dev_s *dev, bool enable)
{
  struct fc7300_uart_s *priv = (struct fc7300_uart_s *)dev->priv;
  irqstate_t flags;
  uint32_t regval;

  /* Enable interrupt for TX complete */

  flags = spin_lock_irqsave(NULL);
  if (enable)
    {
#ifndef CONFIG_SUPPRESS_SERIAL_INTS
      priv->ie |= FCUART_CTRL_TIE;
#endif
    }
  else
    {
      priv->ie &= ~FCUART_CTRL_TIE;
    }

  regval  = fc7300_serialin(priv, FC7300_FCUART_CTRL_OFFSET);
  regval &= ~FCUART_ALL_INTS;
  regval |= priv->ie;
  fc7300_serialout(priv, FC7300_FCUART_CTRL_OFFSET, regval);
  spin_unlock_irqrestore(NULL, flags);
}
#endif

/****************************************************************************
 * Name: fc7300_txready
 *
 * Description:
 *   Return true if the transmit is completed
 *
 ****************************************************************************/

static bool fc7300_txready(struct uart_dev_s *dev)
{
  struct fc7300_uart_s *priv = (struct fc7300_uart_s *)dev->priv;
  uint32_t regval;

  regval = fc7300_serialin(priv, FC7300_FCUART_STAT_OFFSET);
  return ((regval & FCUART_STAT_TCF) != 0);
}

/****************************************************************************
 * Name: fc7300_txempty
 *
 * Description:
 *   Return true if the transmit reg is empty
 *
 ****************************************************************************/

static bool fc7300_txempty(struct uart_dev_s *dev)
{
  struct fc7300_uart_s *priv = (struct fc7300_uart_s *)dev->priv;
  uint32_t regval;

  regval = fc7300_serialin(priv, FC7300_FCUART_STAT_OFFSET);
  return ((regval & FCUART_STAT_TDREF) != 0);
}

/****************************************************************************
 * Name: fc7300_dma_rxcallback
 *
 * Description:
 *   This function checks the current DMA state and calls the generic
 *   serial stack when bytes appear to be available.
 *
 ****************************************************************************/

#ifdef SERIAL_HAVE_RXDMA
static void fc7300_dma_rxcallback(struct dma_chan_s *chan, void *arg,
                                  ssize_t len)
{
  struct uart_dev_s *dev = (struct uart_dev_s *)arg;
  struct fc7300_uart_s *priv = (struct fc7300_uart_s *)dev->priv;
  uint32_t sr;

  if (priv->rxenable && fc7300_dma_rxavailable(dev))
    {
      uart_recvchars(dev);
    }

  /* Get the masked FCUART status word to check and clear error flags.
   *
   * When wake-up from low power mode was not fast enough, UART is resumed
   * too late and sometimes exactly when character was coming over UART,
   * resulting to frame error.
   * If error flag is not cleared, Rx DMA will be stuck. Clearing errors
   * will release Rx DMA.
   */

  sr = fc7300_serialin(priv, FC7300_FCUART_STAT_OFFSET);

  if ((sr & (FCUART_STAT_RORF | FCUART_STAT_NF | FCUART_STAT_FEF)) != 0)
    {
      fc7300_serialout(priv, FC7300_FCUART_STAT_OFFSET,
                       sr & (FCUART_STAT_RORF |
                             FCUART_STAT_NF |
                             FCUART_STAT_FEF));
    }
}
#endif

/****************************************************************************
 * Name: up_putc
 *
 * Description:
 *   Provide priority, low-level access to support OS debug  writes
 *
 ****************************************************************************/

void up_putc(int ch)
{
#ifdef CONSOLE_DEV
  struct fc7300_uart_s *priv =
      (struct fc7300_uart_s *)CONSOLE_DEV.priv;
  uint32_t ie;

  fc7300_disableuartint(priv, &ie);

  /* Check for LF */

  if (ch == '\n')
    {
      /* Add CR */

      fc7300_lowputc('\r');
    }

  fc7300_lowputc(ch);
  fc7300_restoreuartint(priv, ie);
#endif

//   return ch;
}

#endif /* CONFIG_FC7300_FCUART */

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: arm_earlyserialinit
 *
 * Description:
 *   Performs the low level USART initialization early in debug so that the
 *   serial console will be available during bootup.  This must be called
 *   before arm_serialinit.
 *
 ****************************************************************************/

#ifdef USE_EARLYSERIALINIT
void arm_earlyserialinit(void)
{
#ifdef CONSOLE_DEV
  CONSOLE_DEV.isconsole = true;
  fc7300_setup(&CONSOLE_DEV);
#endif
}
#endif

/****************************************************************************
 * Name: arm_serialinit
 *
 * Description:
 *   Register serial console and serial ports.  This assumes
 *   that arm_earlyserialinit was called previously.
 *
 ****************************************************************************/

void arm_serialinit(void)
{
#ifdef CONSOLE_DEV
  uart_register("/dev/console", &CONSOLE_DEV);
#  if defined(SERIAL_HAVE_CONSOLE_DMA)
  fc7300_dma_setup(&CONSOLE_DEV);
#  endif
#endif

  /* Register all UARTs */

#ifdef CONFIG_FC7300_FCUART0
  uart_register("/dev/ttyS0", &g_fcuart0dev);
#endif
#ifdef CONFIG_FC7300_FCUART1
  uart_register("/dev/ttyS1", &g_fcuart1dev);
#endif
#ifdef CONFIG_FC7300_FCUART2
  uart_register("/dev/ttyS2", &g_fcuart2dev);
#endif
#ifdef CONFIG_FC7300_FCUART3
  uart_register("/dev/ttyS3", &g_fcuart3dev);
#endif
#ifdef CONFIG_FC7300_FCUART4
  uart_register("/dev/ttyS4", &g_fcuart4dev);
#endif
#ifdef CONFIG_FC7300_FCUART5
  uart_register("/dev/ttyS5", &g_fcuart5dev);
#endif
#ifdef CONFIG_FC7300_FCUART6
  uart_register("/dev/ttyS6", &g_fcuart6dev);
#endif
#ifdef CONFIG_FC7300_FCUART7
  uart_register("/dev/ttyS7", &g_fcuart7dev);
#endif
#ifdef CONFIG_FC7300_FCUART8
  uart_register("/dev/ttyS8", &g_fcuart8dev);
#endif
#ifdef CONFIG_FC7300_FCUART9
  uart_register("/dev/ttyS9", &g_fcuart9dev);
#endif
#ifdef CONFIG_FC7300_FCUART10
  uart_register("/dev/ttyS10", &g_fcuart10dev);
#endif
#ifdef CONFIG_FC7300_FCUART11
  uart_register("/dev/ttyS11", &g_fcuart11dev);
#endif
#ifdef CONFIG_FC7300_FCUART12
  uart_register("/dev/ttyS12", &g_fcuart12dev);
#endif
#ifdef CONFIG_FC7300_FCUART13
  uart_register("/dev/ttyS13", &g_fcuart13dev);
#endif
#ifdef CONFIG_FC7300_FCUART14
  uart_register("/dev/ttyS14", &g_fcuart14dev);
#endif
#ifdef CONFIG_FC7300_FCUART15
  uart_register("/dev/ttyS15", &g_fcuart15dev);
#endif
#ifdef CONFIG_FC7300_FCUART16
  uart_register("/dev/ttyS16", &g_fcuart16dev);
#endif
#ifdef CONFIG_FC7300_FCUART17
  uart_register("/dev/ttyS17", &g_fcuart17dev);
#endif
}
