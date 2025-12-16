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

#include <byteswap.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <debug.h>
#include <errno.h>

#include <nuttx/can.h>
#include <nuttx/wdog.h>
#include <nuttx/irq.h>
#include <nuttx/arch.h>
#include <nuttx/wqueue.h>
#include <nuttx/signal.h>
#include <nuttx/net/net.h>
#include <nuttx/net/netdev.h>
#include <nuttx/net/netdev_lowerhalf.h>
#include <nuttx/net/pkt.h>
#include <nuttx/net/can.h>

#include "arm_internal.h"
#include "chip.h"
#include "fc7300_config.h"
#include "hardware/fc7300_flexcan.h"
#include "hardware/fc7300_pinmux.h"
#include "fc7300_periphclocks.h"
#include "fc7300_clockconfig.h"
#include "hardware/fc7300_pin.h"
#include "fc7300_flexcan.h"

#include <arch/board/board.h>

#include <sys/time.h>

#ifdef CONFIG_FC7300_FLEXCAN_SOCKET

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#define MASKSTDID                   0x000007ff
#define MASKEXTID                   0x1fffffff
#define FLAGEFF                     (1 << 31) /* Extended frame format */
#define FLAGRTR                     (1 << 30) /* Remote transmission request */

#define IFLAG1_RX                   ((1 << priv->rxmb_cnt) - 1)
#define IFLAG1_TX                   (((1 << priv->txmb_cnt) - 1) << priv->rxmb_cnt)

#define CAN_FIFO_NE                 (1 << 5)
#define CAN_FIFO_OV                 (1 << 6)
#define CAN_FIFO_WARN               (1 << 7)
#define CAN_EFF_FLAG                0x80000000 /* EFF/SFF is set in the MSB */

#define POOL_SIZE                   1

#define MSG_DATA                    0

#ifdef CONFIG_NET_CAN_CANFD
#  define FLEXCAN_NETDEV_BUFSIZE   ((72 + MSG_DATA) * POOL_SIZE + \
                                    CONFIG_NET_GUARDSIZE)
#else
#  define FLEXCAN_NETDEV_BUFSIZE   ((16 + MSG_DATA) * POOL_SIZE + \
                                    CONFIG_NET_GUARDSIZE)
#endif

#if NETPKT_BUFLEN - CONFIG_NET_LL_GUARDSIZE >= FLEXCAN_NETDEV_BUFSIZE
#  define FLEXCAN_NETDEV_RECV_OFFLOAD
#endif

/****************************************************************************
 * Private Types
 ****************************************************************************/

/* The fc7300_netdev_s encapsulates all state information for a single
 * hardware interface
 */

struct fc7300_netdev_s
{
  struct netdev_lowerhalf_s dev;

  uint32_t base;                      /* FLEXCAN base address */
  bool bifup;                         /* true:ifup false:ifdown */
#ifdef CONFIG_NET_CAN_CANFD
  bool canfd_en;                      /* true:use CAN FD, false:use CAN 2.0B */
#  ifdef CONFIG_FC7300_FLEXCAN_CANFD_BRS
  bool canfdbrs_en;
  struct flexcan_timeseg data_timing; /* Timing for data phase */
#  endif
#endif

  uint8_t *txdesc;                    /* A pointer to the list of TX descriptor */
#ifndef FLEXCAN_NETDEV_RECV_OFFLOAD
  uint8_t *rxdesc;                    /* A pointer to the list of RX descriptors */
#endif

  uint32_t *rx;
  uint32_t *tx;

  struct flexcan_timeseg arbi_timing; /* Timing for arbitration phase */
  const struct flexcan_config_s *config;

  uint8_t *avail_rxmb;
  uint8_t rxmb_cnt;
  uint8_t rxmb_head;
  uint8_t rxmb_tail;
  uint32_t rxmb_pending;              /* Pending rx masks */

  netpkt_t **tx_pkt_pending;
  uint8_t txmb_cnt;

  bool erf_en;
  bool erf_pending;
#ifdef CONFIG_FC7300_FLEXCAN_ENHANCED_RX_FIFO
  uint32_t erfel_total_cnt;
#endif
};

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

/* Helper functions */

static inline uint32_t arm_lsb(unsigned int value);

/* Common TX logic */

static int fc7300_flexcan_send(struct fc7300_netdev_s *priv, uint8_t *txbuf,
                               uint32_t pkt_len);
static int  fc7300_transmit(struct netdev_lowerhalf_s *dev, netpkt_t *pkt);

/* Helper functions */

/* Interrupt handling */

static netpkt_t *fc7300_receive(struct netdev_lowerhalf_s *priv);

static int  fc7300_flexcan_interrupt(int irq, void *context, void *arg);

/* NuttX callback functions */

static int  fc7300_ifup(struct netdev_lowerhalf_s *dev);
static int  fc7300_ifdown(struct netdev_lowerhalf_s *dev);

#ifdef CONFIG_NETDEV_IOCTL
static int  fc7300_ioctl(struct netdev_lowerhalf_s *dev, int cmd,
                         unsigned long arg);
#endif

/* Initialization */

static int  fc7300_flexcan_initialize(struct fc7300_netdev_s *priv);
static void fc7300_flexcan_reset(struct fc7300_netdev_s *priv);

/****************************************************************************
 * Private Data
 ****************************************************************************/

#ifdef CONFIG_FC7300_FLEXCAN0_SOCKET
static uint8_t flexcan0_avail_rxmb[CONFIG_FC7300_FLEXCAN0_RXMB_COUNT];
static netpkt_t *flexcan0_tx_pkt_pending[FLEXCAN0_TXMB_COUNT];
#endif

#ifdef CONFIG_FC7300_FLEXCAN1_SOCKET
static uint8_t flexcan1_avail_rxmb[CONFIG_FC7300_FLEXCAN1_RXMB_COUNT];
static netpkt_t *flexcan1_tx_pkt_pending[FLEXCAN1_TXMB_COUNT];
#endif

#ifdef CONFIG_FC7300_FLEXCAN2_SOCKET
static uint8_t flexcan2_avail_rxmb[CONFIG_FC7300_FLEXCAN2_RXMB_COUNT];
static netpkt_t *flexcan2_tx_pkt_pending[FLEXCAN2_TXMB_COUNT];
#endif

#ifdef CONFIG_FC7300_FLEXCAN3_SOCKET
static uint8_t flexcan3_avail_rxmb[CONFIG_FC7300_FLEXCAN3_RXMB_COUNT];
static netpkt_t *flexcan3_tx_pkt_pending[FLEXCAN3_TXMB_COUNT];
#endif

#ifdef CONFIG_FC7300_FLEXCAN4_SOCKET
static uint8_t flexcan4_avail_rxmb[CONFIG_FC7300_FLEXCAN4_RXMB_COUNT];
static netpkt_t *flexcan4_tx_pkt_pending[FLEXCAN4_TXMB_COUNT];
#endif

#ifdef CONFIG_FC7300_FLEXCAN5_SOCKET
static uint8_t flexcan5_avail_rxmb[CONFIG_FC7300_FLEXCAN5_RXMB_COUNT];
static netpkt_t *flexcan5_tx_pkt_pending[FLEXCAN5_TXMB_COUNT];
#endif

#ifdef CONFIG_FC7300_FLEXCAN6_SOCKET
static uint8_t flexcan6_avail_rxmb[CONFIG_FC7300_FLEXCAN6_RXMB_COUNT];
static netpkt_t *flexcan6_tx_pkt_pending[FLEXCAN6_TXMB_COUNT];
#endif

#ifdef CONFIG_FC7300_FLEXCAN7_SOCKET
static uint8_t flexcan7_avail_rxmb[CONFIG_FC7300_FLEXCAN7_RXMB_COUNT];
static netpkt_t *flexcan7_tx_pkt_pending[FLEXCAN7_TXMB_COUNT];
#endif

#ifdef CONFIG_FC7300_FLEXCAN8_SOCKET
static uint8_t flexcan8_avail_rxmb[CONFIG_FC7300_FLEXCAN8_RXMB_COUNT];
static netpkt_t *flexcan8_tx_pkt_pending[FLEXCAN8_TXMB_COUNT];
#endif

#ifdef CONFIG_FC7300_FLEXCAN9_SOCKET
static uint8_t flexcan9_avail_rxmb[CONFIG_FC7300_FLEXCAN9_RXMB_COUNT];
static netpkt_t *flexcan9_tx_pkt_pending[FLEXCAN9_TXMB_COUNT];
#endif

/* FlexCAN device structures */

#ifdef CONFIG_FC7300_FLEXCAN0_SOCKET
static struct fc7300_netdev_s g_flexcan0;
#endif

#ifdef CONFIG_FC7300_FLEXCAN1_SOCKET
static struct fc7300_netdev_s g_flexcan1;
#endif

#ifdef CONFIG_FC7300_FLEXCAN2_SOCKET
static struct fc7300_netdev_s g_flexcan2;
#endif

#ifdef CONFIG_FC7300_FLEXCAN3_SOCKET
static struct fc7300_netdev_s g_flexcan3;
#endif

#ifdef CONFIG_FC7300_FLEXCAN4_SOCKET
static struct fc7300_netdev_s g_flexcan4;
#endif

#ifdef CONFIG_FC7300_FLEXCAN5_SOCKET
static struct fc7300_netdev_s g_flexcan5;
#endif

#ifdef CONFIG_FC7300_FLEXCAN6_SOCKET
static struct fc7300_netdev_s g_flexcan6;
#endif

#ifdef CONFIG_FC7300_FLEXCAN7_SOCKET
static struct fc7300_netdev_s g_flexcan7;
#endif

#ifdef CONFIG_FC7300_FLEXCAN8_SOCKET
static struct fc7300_netdev_s g_flexcan8;
#endif

#ifdef CONFIG_FC7300_FLEXCAN9_SOCKET
static struct fc7300_netdev_s g_flexcan9;
#endif

#ifdef CONFIG_NET_CAN_CANFD
static uint8_t g_tx_pool[(sizeof(struct canfd_frame)+MSG_DATA)*POOL_SIZE];
#  ifndef FLEXCAN_NETDEV_RECV_OFFLOAD
static uint8_t g_rx_pool[(sizeof(struct canfd_frame)+MSG_DATA)*POOL_SIZE];
#  endif
#else
static uint8_t g_tx_pool[(sizeof(struct can_frame)+MSG_DATA)*POOL_SIZE];
#  ifndef FLEXCAN_NETDEV_RECV_OFFLOAD
static uint8_t g_rx_pool[(sizeof(struct can_frame)+MSG_DATA)*POOL_SIZE];
#  endif
#endif

static const struct netdev_ops_s g_ops =
{
  fc7300_ifup,     /* ifup */
  fc7300_ifdown,   /* ifdown */
  fc7300_transmit, /* transmit */
  fc7300_receive   /* receive */
#ifdef CONFIG_NETDEV_IOCTL
  ,
  fc7300_ioctl
#endif
};

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: arm_lsb
 *
 * Description:
 *   Calculate position of lsb that's equal to 1
 *
 * Input Parameters:
 *   value - The value to perform the operation on
 *
 * Returned Value:
 *   location of lsb which is equal to 1, returns 32 when value is 0
 *
 ****************************************************************************/

static inline uint32_t arm_lsb(unsigned int value)
{
  uint32_t ret;
  volatile uint32_t rvalue = value;
  __asm__ __volatile__ ("rbit %1,%0" : "=r" (rvalue) : "r" (rvalue));
  __asm__ __volatile__ ("clz %0, %1" : "=r"(ret) : "r"(rvalue));
  return ret;
}

/****************************************************************************
 * Function: fc7300_flexcan_send
 *
 * Description:
 *   Start hardware transmission.
 *
 * Input Parameters:
 *   priv  - Reference to the driver state structure
 *   txbuf - transmit packet
 *   pkt_len - packet length
 *
 * Returned Value:
 *   OK on success; a negated errno on failure
 *
 * Assumptions:
 *   May or may not be called from an interrupt handler.  In either case,
 *   global interrupts are disabled, either explicitly or indirectly through
 *   interrupt handling logic.
 *
 ****************************************************************************/

static int fc7300_flexcan_send(struct fc7300_netdev_s *priv, uint8_t *txbuf,
                               uint32_t pkt_len)
{
  /* Attempt to write frame */

  uint32_t mbi = 0;
  uint32_t mb_bit;
  uint32_t regval;
#ifdef CONFIG_NET_CAN_CANFD
  uint32_t *frame_data_word;
  uint32_t i;
#endif

  irqstate_t flags;

#ifdef CONFIG_NET_CAN_CANFD
  if ((pkt_len > sizeof(struct can_frame)) &&
      (priv->canfd_en == false))
    {
      /* Invalid to transmit CAN FD frames when the device did not enable
       * CAN FD support.
       */

      nerr("CAN FD frames not enabled in CAN 0x%" PRIx32 "\n", priv->base);
      return -ENOTSUP;
    }
#endif

  if ((getreg32(priv->base + FC7300_CAN_ESR2_OFFSET) &
       (CAN_ESR2_IMB | CAN_ESR2_VPS)) ==
      (CAN_ESR2_IMB | CAN_ESR2_VPS))
    {
      mbi  = ((getreg32(priv->base + FC7300_CAN_ESR2_OFFSET) &
        CAN_ESR2_LPTM_MASK) >> CAN_ESR2_LPTM_SHIFT);
      mbi -= priv->rxmb_cnt;
    }

  mb_bit = 1 << (priv->rxmb_cnt + mbi);

#ifdef CONFIG_NET_CAN_CANFD
  if (priv->canfd_en)
    {
      struct canfdmb_s *mb = (struct canfdmb_s *)priv->tx;
      while (mbi < priv->txmb_cnt)
        {
          if (mb[mbi].cs.code != CAN_TXMB_DATAORREMOTE)
            {
              putreg32(mb_bit, priv->base + FC7300_CAN_IFLAG1_OFFSET);
              break;
            }

          mb_bit <<= 1;
          mbi++;
        }
    }
  else
#endif
    {
      struct canmb_s *mb = (struct canmb_s *)priv->tx;
      while (mbi < priv->txmb_cnt)
        {
          if (mb[mbi].cs.code != CAN_TXMB_DATAORREMOTE)
            {
              putreg32(mb_bit, priv->base + FC7300_CAN_IFLAG1_OFFSET);
              break;
            }

          mb_bit <<= 1;
          mbi++;
        }
    }

  if (mbi == priv->txmb_cnt)
    {
      nwarn("No TX MB available mbi %" PRIu32 "\n", mbi);
      NETDEV_TXERRORS(&priv->dev);
      return -EBUSY;       /* No transmission for you! */
    }

  union cs_e cs = {
    0
  };

  cs.code = CAN_TXMB_DATAORREMOTE;

#ifdef CONFIG_NET_CAN_CANFD
  if (priv->canfd_en)
    {
      struct canfdmb_s *mb = &((struct canfdmb_s *)priv->tx)[mbi];
      mb->cs.code = CAN_TXMB_INACTIVE;
      if (pkt_len <= sizeof(struct can_frame))
        {
          struct can_frame *frame = (struct can_frame *)txbuf;

          if (frame->can_id & CAN_EFF_FLAG)
            {
              cs.ide = 1;
              mb->id.ext = frame->can_id & MASKEXTID;
            }
          else
            {
              mb->id.std = frame->can_id & MASKSTDID;
            }

          cs.rtr = frame->can_id & FLAGRTR ? 1 : 0;
          cs.dlc = frame->can_dlc;

          mb->data[0].w00 = bswap_32(*(uint32_t *)&frame->data[0]);
          mb->data[1].w00 = bswap_32(*(uint32_t *)&frame->data[4]);
        }
      else
        {
          struct canfd_frame *frame = (struct canfd_frame *)txbuf;

          cs.edl = 1; /* CAN FD Frame */

          if (frame->can_id & CAN_EFF_FLAG)
            {
              cs.ide = 1;
              mb->id.ext = frame->can_id & MASKEXTID;
            }
          else
            {
              mb->id.std = frame->can_id & MASKSTDID;
            }

#ifdef CONFIG_FC7300_FLEXCAN_CANFD_BRS
          if (priv->canfdbrs_en && (frame->flags & CANFD_BRS))
            {
              cs.brs = 1;
            }
#endif

          cs.rtr = frame->can_id & FLAGRTR ? 1 : 0;

          cs.dlc = g_len_to_can_dlc[frame->len];

          frame_data_word = (uint32_t *)&frame->data[0];

          for (i = 0; i < (frame->len + 4 - 1) / 4; i++)
            {
              mb->data[i].w00 = bswap_32(frame_data_word[i]);
            }
        }

      mb->cs = cs; /* Go. */
    }
  else
#endif
    {
      struct canmb_s *mb = &((struct canmb_s *)priv->tx)[mbi];
      mb->cs.code = CAN_TXMB_INACTIVE;

      if (pkt_len <= sizeof(struct can_frame))
        {
          struct can_frame *frame = (struct can_frame *)txbuf;

          if (frame->can_id & CAN_EFF_FLAG)
            {
              cs.ide = 1;
              mb->id.ext = frame->can_id & MASKEXTID;
            }
          else
            {
              mb->id.std = frame->can_id & MASKSTDID;
            }

          cs.rtr = frame->can_id & FLAGRTR ? 1 : 0;
          cs.dlc = frame->can_dlc;

          mb->data[0].w00 = bswap_32(*(uint32_t *)&frame->data[0]);
          mb->data[1].w00 = bswap_32(*(uint32_t *)&frame->data[4]);
        }

      mb->cs = cs; /* Go. */
    }

  flags = enter_critical_section();

  regval = getreg32(priv->base + FC7300_CAN_IMASK1_OFFSET);
  regval |= mb_bit;
  putreg32(regval, priv->base + FC7300_CAN_IMASK1_OFFSET);

  leave_critical_section(flags);

  return mbi;
}

/****************************************************************************
 * Function: fc7300_transmit
 *
 * Description:
 *   Start hardware transmission.  Called by netdev upperhalf driver
 *
 * Input Parameters:
 *   dev  - Reference to the netdev lowerhalf structure
 *   pkt  - the packet to be transmitted
 *
 * Returned Value:
 *   OK on success; a negated errno on failure
 *
 * Assumptions:
 *   May or may not be called from an interrupt handler.  In either case,
 *   global interrupts are disabled, either explicitly or indirectly through
 *   interrupt handling logic.
 *
 ****************************************************************************/

static int fc7300_transmit(struct netdev_lowerhalf_s *dev, netpkt_t *pkt)
{
  int ret;
  uint32_t pkt_len;
  struct fc7300_netdev_s *priv = (struct fc7300_netdev_s *)dev;

  pkt_len = netpkt_getdatalen(dev, pkt);

  if (netpkt_is_fragmented(pkt))
    {
      netpkt_copyout(dev, priv->txdesc, pkt, pkt_len, 0);
      ret = fc7300_flexcan_send(priv, priv->txdesc, pkt_len);
    }
  else
    {
      ret = fc7300_flexcan_send(priv, netpkt_getdata(dev, pkt), pkt_len);
    }

  if (ret >= 0)
    {
      priv->tx_pkt_pending[ret] = pkt;
      return 0;
    }

  return ret;
}

/****************************************************************************
 * Function: fc7300_flexcan_read
 *
 * Description:
 *   Read the mailbox to get the data
 *
 * Input Parameters:
 *   priv  - Reference to the driver state structure
 *   buf   - the buffer for received message
 *   mb_index - the mailbox index
 *
 * Returned Value:
 *   OK on success; a negated errno on failure
 *
 * Assumptions:
 *   Global interrupts are disabled by interrupt handling logic.
 *
 ****************************************************************************/

uint32_t fc7300_flexcan_read_mb(struct fc7300_netdev_s *priv, uint8_t *buf,
                             uint8_t mb_index)
{
  uint32_t len;
  irqstate_t flags;
#ifdef CONFIG_NET_CAN_CANFD
  uint32_t *frame_data_word;
  uint32_t i;
  uint32_t mb_flags;
#endif

#ifdef CONFIG_NET_CAN_CANFD
  if (priv->canfd_en)
    {
      struct canfdmb_s *rf = &((struct canfdmb_s *)priv->rx)[mb_index];

      /* Read the frame contents */

      if (rf->cs.edl) /* CAN FD frame */
        {
          struct canfd_frame *frame = (struct canfd_frame *)buf;

          if (rf->cs.ide)
            {
              frame->can_id = MASKEXTID & rf->id.ext;
              frame->can_id |= FLAGEFF;
            }
          else
            {
              frame->can_id = MASKSTDID & rf->id.std;
            }

          if (rf->cs.rtr)
            {
              frame->can_id |= FLAGRTR;
            }

          frame->len = g_can_dlc_to_len[rf->cs.dlc];
          frame_data_word = (uint32_t *)&frame->data[0];

          for (i = 0; i < (frame->len + 4 - 1) / 4; i++)
            {
              frame_data_word[i] = bswap_32(rf->data[i].w00);
            }

          len = sizeof(struct canfd_frame);
        }
      else
        {
          struct can_frame *frame = (struct can_frame *)buf;

          if (rf->cs.ide)
            {
              frame->can_id = MASKEXTID & rf->id.ext;
              frame->can_id |= FLAGEFF;
            }
          else
            {
              frame->can_id = MASKSTDID & rf->id.std;
            }

          if (rf->cs.rtr)
            {
              frame->can_id |= FLAGRTR;
            }

          frame->can_dlc = rf->cs.dlc;

          *(uint32_t *)&frame->data[0] = bswap_32(rf->data[0].w00);
          *(uint32_t *)&frame->data[4] = bswap_32(rf->data[1].w00);

          len = sizeof(struct can_frame);
        }
    }
  else
#endif
    {
      struct canmb_s *rf = &((struct canmb_s *)priv->rx)[mb_index];
      struct can_frame *frame = (struct can_frame *)buf;

      if (rf->cs.ide)
        {
          frame->can_id = MASKEXTID & rf->id.ext;
          frame->can_id |= FLAGEFF;
        }
      else
        {
          frame->can_id = MASKSTDID & rf->id.std;
        }

      if (rf->cs.rtr)
        {
          frame->can_id |= FLAGRTR;
        }

      frame->can_dlc = rf->cs.dlc;

      *(uint32_t *)&frame->data[0] = bswap_32(rf->data[0].w00);
      *(uint32_t *)&frame->data[4] = bswap_32(rf->data[1].w00);

      len = sizeof(struct can_frame);
    }

  /* Read the free run timer to unlock the mb */

  (void)getreg32(priv->base + FC7300_CAN_TIMER_OFFSET);

  /* Clean the MB interrupt flag and re-enable interrupt */

  flags = enter_critical_section();

  priv->rxmb_pending &= ~(1 << mb_index);

  /* Clear MB interrupt flag */

  putreg32(1 << mb_index, priv->base + FC7300_CAN_IFLAG1_OFFSET);

  mb_flags = getreg32(priv->base + FC7300_CAN_IMASK1_OFFSET);
  mb_flags |= (1 << mb_index);
  putreg32(mb_flags, priv->base + FC7300_CAN_IMASK1_OFFSET);

  leave_critical_section(flags);

  return len;
}

/****************************************************************************
 * Function: fc7300_flexcan_read_fifo
 *
 * Description:
 *   Read the fifo to get the data
 *
 * Input Parameters:
 *   priv  - Reference to the driver state structure
 *   buf   - the buffer for received message
 *
 * Returned Value:
 *   OK on success; a negated errno on failure
 *
 * Assumptions:
 *   Global interrupts are disabled by interrupt handling logic.
 *
 ****************************************************************************/

uint32_t fc7300_flexcan_read_fifo(struct fc7300_netdev_s *priv, uint8_t *buf)
{
  uint32_t len = 0;
  irqstate_t flags;
#ifdef CONFIG_NET_CAN_CANFD
  uint32_t *frame_data_word;
  uint32_t i;
#endif
  uint32_t regval;

#ifdef CONFIG_NET_CAN_CANFD
  if (priv->canfd_en)
    {
      struct canfderf_s *erf = (struct canfderf_s *)(priv->base +
                                FC7300_CAN_ERX_FIFO_OFFSET);

      /* Read the frame contents */

      if (erf->cs.edl) /* CAN FD frame */
        {
          struct canfd_frame *frame = (struct canfd_frame *)buf;

          if (erf->cs.ide)
            {
              frame->can_id = MASKEXTID & erf->id.ext;
              frame->can_id |= FLAGEFF;
            }
          else
            {
              frame->can_id = MASKSTDID & erf->id.std;
            }

          if (erf->cs.rtr)
            {
              frame->can_id |= FLAGRTR;
            }

          frame->len = g_can_dlc_to_len[erf->cs.dlc];
          frame_data_word = (uint32_t *)&frame->data[0];

          for (i = 0; i < (frame->len + 4 - 1) / 4; i++)
            {
              frame_data_word[i] = bswap_32(erf->data[i].w00);
            }

          len = sizeof(struct canfd_frame);
        }
      else
        {
          struct can_frame *frame = (struct can_frame *)buf;

          if (erf->cs.ide)
            {
              frame->can_id = MASKEXTID & erf->id.ext;
              frame->can_id |= FLAGEFF;
            }
          else
            {
              frame->can_id = MASKSTDID & erf->id.std;
            }

          if (erf->cs.rtr)
            {
              frame->can_id |= FLAGRTR;
            }

          frame->can_dlc = erf->cs.dlc;

          *(uint32_t *)&frame->data[0] = bswap_32(erf->data[0].w00);
          *(uint32_t *)&frame->data[4] = bswap_32(erf->data[1].w00);

          len = sizeof(struct can_frame);
        }
    }
  else
#endif
    {
      struct canerf_s *erf = (struct canerf_s *)(priv->base +
                              FC7300_CAN_ERX_FIFO_OFFSET);
      struct can_frame *frame = (struct can_frame *)buf;

      if (erf->cs.ide)
        {
          frame->can_id = MASKEXTID & erf->id.ext;
          frame->can_id |= FLAGEFF;
        }
      else
        {
          frame->can_id = MASKSTDID & erf->id.std;
        }

      if (erf->cs.rtr)
        {
          frame->can_id |= FLAGRTR;
        }

      frame->can_dlc = erf->cs.dlc;

      *(uint32_t *)&frame->data[0] = bswap_32(erf->data[0].w00);
      *(uint32_t *)&frame->data[4] = bswap_32(erf->data[1].w00);

      len = sizeof(struct can_frame);
    }

  /* Clean the ERFF interrupt flag and re-enable interrupt */

  flags = enter_critical_section();

  priv->erf_pending = false;

  /* Clear ERF interrupt flag */

  regval  = getreg32(priv->base + FC7300_CAN_ERFSR_OFFSET);
  regval |= (CAN_ERFSR_ERFDA);
  putreg32(regval, priv->base + FC7300_CAN_ERFSR_OFFSET);

  /* Enable Enhanced Rx FIFO Data Available Interrupt */

  regval  = getreg32(priv->base + FC7300_CAN_ERFIER_OFFSET);
  regval |= (CAN_ERFIER_ERFDAIE);
  putreg32(regval, priv->base + FC7300_CAN_ERFIER_OFFSET);

  leave_critical_section(flags);

  return len;
}

/****************************************************************************
 * Function: fc7300_receive
 *
 * Description:
 *   Receive the FlexCAN message
 *
 * Input Parameters:
 *   dev  - Reference to the netdev lowerhalf structure
 *
 * Returned Value:
 *   netpkt_t - Reference to the received packet
 *
 * Assumptions:
 *   Global interrupts are disabled by interrupt handling logic.
 *
 ****************************************************************************/

static netpkt_t *fc7300_receive(struct netdev_lowerhalf_s *dev)
{
  struct fc7300_netdev_s *priv = (struct fc7300_netdev_s *)dev;
  uint8_t mb_index;
  uint32_t len = 0;
  netpkt_t *pkt = NULL;

  /* No Rx mailbox ready */

  if (priv->rxmb_pending == 0 && priv->erf_pending == false)
    {
      return NULL;
    }

  pkt = netpkt_alloc(dev, NETPKT_RX);
  if (pkt == NULL)
    {
      nwarn("Allocate RX MB buffer failed\n");
      return NULL;
    }

  if (priv->rxmb_pending > 0)
    {
      mb_index = priv->avail_rxmb[priv->rxmb_head];
      priv->rxmb_head++;
      if (priv->rxmb_head == priv->rxmb_cnt)
        {
          priv->rxmb_head = 0;
        }

#ifdef FLEXCAN_NETDEV_RECV_OFFLOAD
      len = fc7300_flexcan_read_mb(priv, netpkt_getdata(dev, pkt), mb_index);
#else
      len = fc7300_flexcan_read_mb(priv, priv->rxdesc, mb_index);
#endif
    }

  if (priv->erf_pending)
    {
#ifdef FLEXCAN_NETDEV_RECV_OFFLOAD
      len = fc7300_flexcan_read_fifo(priv, netpkt_getdata(dev, pkt));
#else
      len = fc7300_flexcan_read_fifo(priv, priv->rxdesc);
#endif
    }

  if (len == 0)
    {
      netpkt_free(dev, pkt, NETPKT_RX);
      return NULL;
    }

#ifdef FLEXCAN_NETDEV_RECV_OFFLOAD
  netpkt_setdatalen(dev, pkt, len);
#else
  netpkt_copyin(dev, pkt, priv->rxdesc, len, 0);
#endif

  return pkt;
}

/****************************************************************************
 * Function: fc7300_txdone
 *
 * Description:
 *   Check transmit interrupt flags and clear them
 *
 * Input Parameters:
 *   priv  - Reference to the driver state structure
 *
 * Returned Value:
 *   None
 *
 * Assumptions:
 *   None
 *
 ****************************************************************************/

static void fc7300_txdone(struct fc7300_netdev_s *priv)
{
  uint32_t flags;
  uint32_t mbi;
  uint32_t mb_bit;
  struct netdev_lowerhalf_s *dev = &priv->dev;

  flags  = getreg32(priv->base + FC7300_CAN_IFLAG1_OFFSET);
  flags &= IFLAG1_TX;

  /* TODO First Process Error aborts */

  /* Process TX completions */

  mb_bit = 1 << priv->rxmb_cnt;
  for (mbi = 0; flags && mbi < priv->txmb_cnt; mbi++)
    {
      if (flags & mb_bit)
        {
          putreg32(mb_bit, priv->base + FC7300_CAN_IFLAG1_OFFSET);
          flags &= ~mb_bit;

          netpkt_free(dev, priv->tx_pkt_pending[mbi], NETPKT_TX);
        }

      mb_bit <<= 1;
    }
}

/****************************************************************************
 * Function: fc7300_flexcan_interrupt
 *
 * Description:
 *   Three interrupt sources will vector to this function:
 *   1. CAN MB transmit interrupt handler
 *   2. CAN MB receive interrupt handler
 *   3.
 *
 * Input Parameters:
 *   irq     - Number of the IRQ that generated the interrupt
 *   context - Interrupt register state save info (architecture-specific)
 *
 * Returned Value:
 *   OK on success
 *
 * Assumptions:
 *
 ****************************************************************************/

static int fc7300_flexcan_interrupt(int irq, void *context, void *arg)
{
  struct fc7300_netdev_s *priv = (struct fc7300_netdev_s *)arg;
  uint32_t mb_index;
  uint32_t imask;

  if (irq == priv->config->irq)
    {
      uint32_t flags;
      flags = getreg32(priv->base + FC7300_CAN_IFLAG1_OFFSET);
      flags = flags & IFLAG1_RX & (~priv->rxmb_pending);

      if (flags)
        {
          while ((mb_index = arm_lsb(flags)) != 32)
            {
              /* Disable further rx mb interrupts */

              imask  = getreg32(priv->base + FC7300_CAN_IMASK1_OFFSET);
              imask &= ~(1 << mb_index);
              putreg32(imask, priv->base + FC7300_CAN_IMASK1_OFFSET);

              priv->rxmb_pending |= 1 << mb_index;
              priv->avail_rxmb[priv->rxmb_tail] = mb_index;
              priv->rxmb_tail++;
              if (priv->rxmb_tail == priv->rxmb_cnt)
                {
                  priv->rxmb_tail = 0;
                }

              flags &= ~(1 << mb_index);
            }

          netdev_lower_rxready((struct netdev_lowerhalf_s *)priv);
        }

      flags = getreg32(priv->base + FC7300_CAN_ERFSR_OFFSET);
      flags = flags & CAN_ERFSR_ERFDA;

      if (flags)
        {
          /* Disable further enhanced rx fifo interrupts */

          imask  = getreg32(priv->base + FC7300_CAN_ERFIER_OFFSET);
          imask &= ~CAN_ERFIER_ERFDAIE;
          putreg32(imask, priv->base + FC7300_CAN_ERFIER_OFFSET);

          priv->erf_pending = true;
          netdev_lower_rxready((struct netdev_lowerhalf_s *)priv);
        }

      flags  = getreg32(priv->base + FC7300_CAN_IFLAG1_OFFSET);
      flags &= IFLAG1_TX;

      if (flags)
        {
          /* Disable further TX MB CAN interrupts. here can be no race
           * condition here.
           */

          imask = getreg32(priv->base + FC7300_CAN_IMASK1_OFFSET);
          imask &= ~flags;
          putreg32(imask, priv->base + FC7300_CAN_IMASK1_OFFSET);
          fc7300_txdone(priv);
          netdev_lower_txdone((struct netdev_lowerhalf_s *)priv);
        }
    }

  return OK;
}

/****************************************************************************
 * Function: fc7300_ifup
 *
 * Description:
 *   NuttX Callback: Bring up the CAN network interface
 *
 * Input Parameters:
 *   dev  - Reference to the NuttX driver state structure
 *
 * Returned Value:
 *   None
 *
 * Assumptions:
 *
 ****************************************************************************/

static int fc7300_ifup(struct netdev_lowerhalf_s *dev)
{
  struct fc7300_netdev_s *priv = (struct fc7300_netdev_s *)dev;

  if (!fc7300_flexcan_initialize(priv))
    {
      nerr("initialize failed");
      return -1;
    }

  priv->bifup = true;

  priv->txdesc = g_tx_pool;
#ifndef FLEXCAN_NETDEV_RECV_OFFLOAD
  priv->rxdesc = g_rx_pool;
#endif

#ifdef CONFIG_NET_CAN_CANFD
  if (priv->canfd_en)
    {
      priv->dev.netdev.d_pktsize = CANFD_MTU;
    }
    else
#endif
    {
      priv->dev.netdev.d_pktsize = CAN_MTU;
    }

  /* Set interrupts */

#ifdef CONFIG_ARCH_IRQPRIO
  up_prioritize_irq(priv->config->irq, priv->config->irq_pri);
#endif
  up_enable_irq(priv->config->irq);

  return OK;
}

/****************************************************************************
 * Function: fc7300_ifdown
 *
 * Description:
 *   NuttX Callback: Stop the interface.
 *
 * Input Parameters:
 *   dev  - Reference to the NuttX driver state structure
 *
 * Returned Value:
 *   None
 *
 * Assumptions:
 *
 ****************************************************************************/

static int fc7300_ifdown(struct netdev_lowerhalf_s *dev)
{
  struct fc7300_netdev_s *priv = (struct fc7300_netdev_s *)dev;

  fc7300_flexcan_reset(priv);

  priv->bifup = false;

  return OK;
}

/****************************************************************************
 * Function: fc7300_ioctl
 *
 * Description:
 *   CAN ioctl command handler
 *
 * Input Parameters:
 *   dev  - Reference to the NuttX driver state structure
 *   cmd  - ioctl command
 *   arg  - Argument accompanying the command
 *
 * Returned Value:
 *   Zero (OK) on success; a negated errno value on failure.
 *
 * Assumptions:
 *
 ****************************************************************************/

#ifdef CONFIG_NETDEV_IOCTL
static int fc7300_ioctl(struct netdev_lowerhalf_s *dev, int cmd,
                        unsigned long arg)
{
  struct fc7300_netdev_s *priv = (struct fc7300_netdev_s *)dev;

  int ret;

  switch (cmd)
    {
#ifdef CONFIG_NETDEV_CAN_BITRATE_IOCTL
      case SIOCGCANBITRATE: /* Get bitrate from a CAN controller */
        {
          struct can_ioctl_data_s *req =
              (struct can_ioctl_data_s *)((uintptr_t)arg);
          req->arbi_bitrate = priv->arbi_timing.bitrate / 1000; /* kbit/s */
          req->arbi_samplep = priv->arbi_timing.samplep;
#  ifdef CONFIG_NET_CAN_CANFD
          if (priv->canfd_en)
            {
              req->data_bitrate = priv->data_timing.bitrate / 1000; /* kbit/s */
              req->data_samplep = priv->data_timing.samplep;
            }
          else
            {
              req->data_bitrate = 0;
              req->data_samplep = 0;
            }
#  else
          req->data_bitrate = 0;
          req->data_samplep = 0;
#  endif
          ret = OK;
        }
        break;

      case SIOCSCANBITRATE: /* Set bitrate of a CAN controller */
        {
          struct can_ioctl_data_s *req =
              (struct can_ioctl_data_s *)((uintptr_t)arg);

          struct flexcan_timeseg arbi_timing;
          arbi_timing.bitrate = req->arbi_bitrate * 1000;
          arbi_timing.samplep = req->arbi_samplep;

          if (fc7300_bitratetotimeseg(&arbi_timing, 10, 0))
            {
              ret = OK;
            }
          else
            {
              ret = -EINVAL;
            }

#  ifdef CONFIG_NET_CAN_CANFD
          if (priv->canfd_en)
            {
              struct flexcan_timeseg data_timing;
              data_timing.bitrate = req->data_bitrate * 1000;
              data_timing.samplep = req->data_samplep;

              if (ret == OK && fc7300_bitratetotimeseg(&data_timing, 10, 1))
                {
                  ret = OK;
                }
              else
                {
                  ret = -EINVAL;
                }
            }
          else
            {
              data_timing.bitrate = 0;
              data_timing.samplep = 0;
              ret = OK;
            }
#  endif

          if (ret == OK)
            {
              /* Reset CAN controller and start with new timings */

              priv->arbi_timing = arbi_timing;
#  ifdef CONFIG_NET_CAN_CANFD
              priv->data_timing = data_timing;
#  endif
              fc7300_ifup(dev);
            }
        }
        break;
#endif

      default:
        ret = -ENOTTY;
        break;
    }

  return ret;
}
#endif /* CONFIG_NETDEV_IOCTL */

/****************************************************************************
 * Function: fc7300_flexcan_init_eccram
 *
 * Description:
 *   Initialize FLEXCAN ECC RAM
 *
 * Input Parameters:
 *   priv - Reference to the private FLEXCAN driver state structure
 *
 * Returned Value:
 *   None
 *
 * Assumptions:
 *
 ****************************************************************************/

static int fc7300_flexcan_init_eccram(struct fc7300_netdev_s *priv)
{
  uint32_t i;
  uint32_t regval;
  irqstate_t flags;

  flags = enter_critical_section();

  regval = getreg32(priv->base + FC7300_CAN_CTRL2_OFFSET);

  /* Set WRMFRZ bit in CTRL2 Register to grant write access to memory */

  regval |= CAN_CTRL2_WRMFRZ;

  putreg32(regval, priv->base + FC7300_CAN_CTRL2_OFFSET);

  /* Initial Embedded ECC RAM for CAN */

  for (i = 0; i < FC7300_CAN_MB_WORD_COUNT * 4; i += 4)
    {
      putreg32(0, priv->base + FC7300_CAN_MB_OFFSET + i);
    }

  for (i = 0; i < FC7300_CAN_RXIMR_COUNT * 4; i += 4)
    {
      putreg32(0, priv->base + FC7300_CAN_RXIMR0_OFFSET + i);
    }

  for (i = 0; i < FC7300_CAN_RXFIR_TEST_WORD_COUNT * 4; i += 4)
    {
      putreg32(0, priv->base + FC7300_CAN_RXFIR_TEST_OFFSET + i);
    }

  for (i = 0; i < FC7300_CAN_MASK_WORD_COUNT * 4; i += 4)
    {
      putreg32(0, priv->base + FC7300_CAN_MASK_OFFSET + i);
    }

  for (i = 0; i < FC7300_CAN_SMBTX_WORD_COUNT * 4; i += 4)
    {
      putreg32(0, priv->base + FC7300_CAN_SMBTX_OFFSET + i);
    }

  for (i = 0; i < FC7300_CAN_SMBRX0_WORD_COUNT * 4; i += 4)
    {
      putreg32(0, priv->base + FC7300_CAN_SMBRX0_OFFSET + i);
    }

  for (i = 0; i < FC7300_CAN_SMBRX1_WORD_COUNT * 4; i += 4)
    {
      putreg32(0, priv->base + FC7300_CAN_SMBRX1_OFFSET + i);
    }

#ifdef CONFIG_NET_CAN_CANFD
  if (priv->canfd_en)
    {
      /* Initial Embedded ECC RAM if CAN FD Enabled */

      for (i = 0; i < FC7300_CAN_FDSMBTX_WORD_COUNT * 4; i += 4)
        {
          putreg32(0, priv->base + FC7300_CAN_FDSMBTX_OFFSET + i);
        }

      for (i = 0; i < FC7300_CAN_FDSMBRX0_WORD_COUNT * 4; i += 4)
        {
          putreg32(0, priv->base + FC7300_CAN_FDSMBRX0_OFFSET + i);
        }

      for (i = 0; i < FC7300_CAN_FDSMBRX1_WORD_COUNT * 4; i += 4)
        {
          putreg32(0, priv->base + FC7300_CAN_FDSMBRX1_OFFSET + i);
        }

      /* Initial Rx FIFO */

      for (i = 0; i < FC7300_CAN_ERX_FIFO_WORD_COUNT * 4; i += 4)
        {
          putreg32(0, priv->base + FC7300_CAN_ERX_FIFO_OFFSET + i);
        }

      for (i = 0; i < FC7300_CAN_ERFFEL_COUNT * 4; i += 4)
        {
          putreg32(0, priv->base + FC7300_CAN_ERFFEL_OFFSET + i);
        }
    }
#endif

  leave_critical_section(flags);

  return 0;
}

/****************************************************************************
 * Function: fc7300_initalize
 *
 * Description:
 *   Initialize FLEXCAN device
 *
 * Input Parameters:
 *   priv - Reference to the private FLEXCAN driver state structure
 *
 * Returned Value:
 *   None
 *
 * Assumptions:
 *
 ****************************************************************************/

static int fc7300_flexcan_initialize(struct fc7300_netdev_s *priv)
{
  uint32_t regval;
  uint32_t i;
  uint32_t ndtseg1;

  /* Set SYS_CLOCK src */

  fc7300_flexcan_setenable(priv->base, 0);

  /* Always use PCC as clock src */

  regval  = getreg32(priv->base + FC7300_CAN_CTRL1_OFFSET);
  regval &= ~CAN_CTRL1_CLKSRC;
  putreg32(regval, priv->base + FC7300_CAN_CTRL1_OFFSET);

  /* Initialize CAN device */

  fc7300_flexcan_setenable(priv->base, 1);

  /* Enter freeze mode */

  fc7300_flexcan_setfreeze(priv->base, 1);

  fc7300_flexcan_init_eccram(priv);

  fc7300_flexcan_reset(priv);

  /* Reset CTRL1 register to reset value */

  regval  = getreg32(priv->base + FC7300_CAN_CTRL1_OFFSET);
  regval &= ~(CAN_CTRL1_LOM | CAN_CTRL1_LBUF | CAN_CTRL1_TSYN |
              CAN_CTRL1_BOFFREC | CAN_CTRL1_SMP | CAN_CTRL1_RWRNMSK |
              CAN_CTRL1_TWRNMSK | CAN_CTRL1_LPB | CAN_CTRL1_ERRMSK |
              CAN_CTRL1_BOFFMSK);
  putreg32(regval, priv->base + FC7300_CAN_CTRL1_OFFSET);

#ifdef CONFIG_NET_CAN_CANFD
  if (priv->canfd_en)
    {
      regval  = getreg32(priv->base + FC7300_CAN_CTRL2_OFFSET);
      regval |= CAN_CTRL2_BTE | CAN_CTRL2_ISOCANFDEN;
      putreg32(regval, priv->base + FC7300_CAN_CTRL2_OFFSET);

      regval  = getreg32(priv->base + FC7300_CAN_EPRS_OFFSET);
      regval &= ~CAN_EPRS_ENPRESDIV_MASK;
      regval |= CAN_EPRS_ENPRESDIV(priv->arbi_timing.presdiv - 1);
      putreg32(regval, priv->base + FC7300_CAN_EPRS_OFFSET);

      ndtseg1 = priv->arbi_timing.pseg1 + priv->arbi_timing.propseg;
      regval  = CAN_ENCBT_NTSEG1(ndtseg1 - 1) |
                CAN_ENCBT_NTSEG2(priv->arbi_timing.pseg2 - 1) |
                CAN_ENCBT_NRJW(priv->arbi_timing.sjw - 1);
      putreg32(regval, priv->base + FC7300_CAN_ENCBT_OFFSET);

#ifdef CONFIG_FC7300_FLEXCAN_CANFD_BRS
      if (priv->canfdbrs_en)
        {
          regval  = getreg32(priv->base + FC7300_CAN_EPRS_OFFSET);
          regval &= ~CAN_EPRS_EDPRESDIV_MASK;
          regval |= CAN_EPRS_EDPRESDIV(priv->data_timing.presdiv - 1);
          putreg32(regval, priv->base + FC7300_CAN_EPRS_OFFSET);

          ndtseg1 = priv->data_timing.pseg1 + priv->data_timing.propseg;
          regval  = CAN_EDCBT_DTSEG1(ndtseg1 - 1) |
                    CAN_EDCBT_DTSEG2(priv->data_timing.pseg2 - 1) |
                    CAN_EDCBT_DRJW(priv->data_timing.sjw - 1);
          putreg32(regval, priv->base + FC7300_CAN_EDCBT_OFFSET);
        }
#endif

      /* Enable CAN FD feature */

      regval  = getreg32(priv->base + FC7300_CAN_MCR_OFFSET);
      regval |= CAN_MCR_FDEN;
      putreg32(regval, priv->base + FC7300_CAN_MCR_OFFSET);

      /* Additional CAN-FD configurations */

      regval  = CAN_FDCTRL_FDRATE |     /* Enable bit rate switch in data phase of frame */
                CAN_FDCTRL_TDCEN |      /* Enable transceiver delay compensation */
                CAN_FDCTRL_TDCOFF(5) |  /* Setup 5 cycles for data phase sampling delay */
                CAN_FDCTRL_MBDSR0(3);   /* Setup 64 bytes per message buffer (7 MB's) */
      putreg32(regval, priv->base + FC7300_CAN_FDCTRL_OFFSET);
    }
  else
#endif
    {
      regval  = getreg32(priv->base + FC7300_CAN_CTRL2_OFFSET);
      regval |= CAN_CTRL2_BTE;
      putreg32(regval, priv->base + FC7300_CAN_CTRL2_OFFSET);

      regval = getreg32(priv->base + FC7300_CAN_EPRS_OFFSET);
      regval &= ~CAN_EPRS_ENPRESDIV_MASK;
      regval |= CAN_EPRS_ENPRESDIV(priv->arbi_timing.presdiv - 1);
      putreg32(regval, priv->base + FC7300_CAN_EPRS_OFFSET);

      ndtseg1 = priv->arbi_timing.pseg1 + priv->arbi_timing.propseg;
      regval = CAN_ENCBT_NTSEG1(ndtseg1 - 1) |
               CAN_ENCBT_NTSEG2(priv->arbi_timing.pseg1 - 1) |
               CAN_ENCBT_NRJW(priv->arbi_timing.sjw - 1);
      putreg32(regval, priv->base + FC7300_CAN_ENCBT_OFFSET);
    }

#ifdef CONFIG_FC7300_FLEXCAN_ENHANCED_RX_FIFO
  if (priv->erf_en == true)
    {
      /* Configure Enhanced Rx FIFO */

      regval  = getreg32(priv->base + FC7300_CAN_MCR_OFFSET);
      regval &= ~(CAN_MCR_RFEN);      /* Disable Classical Rx FIFO */
      putreg32(regval, priv->base + FC7300_CAN_MCR_OFFSET);

      regval  = getreg32(priv->base + FC7300_CAN_ERFCR_OFFSET);
      regval |= CAN_ERFCR_ERFEN;      /* Enable Enhanced Rx FIFO */
      putreg32(regval, priv->base + FC7300_CAN_ERFCR_OFFSET);

      regval  = getreg32(priv->base + FC7300_CAN_ERFSR_OFFSET);
      regval |= CAN_ERFSR_ERFCLR;     /* Clear Enhanced Rx FIFO Content */
      putreg32(regval, priv->base + FC7300_CAN_ERFSR_OFFSET);

      regval  = getreg32(priv->base + FC7300_CAN_ERFSR_OFFSET);
      regval &= ~(CAN_ERFSR_ERFUFW | CAN_ERFSR_ERFOVF | \
                  CAN_ERFSR_ERFWMI | CAN_ERFSR_ERFDA);
      putreg32(regval, priv->base + FC7300_CAN_ERFSR_OFFSET);

      regval  = getreg32(priv->base + FC7300_CAN_ERFCR_OFFSET);
      regval &= ~((0x7f << CAN_ERFCR_NEXIF_SHIFT) | \
                  (0x3f << CAN_ERFCR_NFE_SHIFT));
      regval |= ((priv->erfel_total_cnt - 1) << CAN_ERFCR_NFE_SHIFT);
      regval |= CAN_ERFCR_ERFWM(0);                 /* Enhanced Rx FIFO Watermaker = 1 */
      putreg32(regval, priv->base + FC7300_CAN_ERFCR_OFFSET);

      regval  = getreg32(priv->base + FC7300_CAN_ERFIER_OFFSET);
      regval |= CAN_ERFIER_ERFDAIE;  /* Enable Enhanced Rx FIFO Data Available Interrupt */
      putreg32(regval, priv->base + FC7300_CAN_ERFIER_OFFSET);

      for (i = 0; i < priv->erfel_total_cnt * 2; i++)
        {
          regval = ((priv->config->erf_filter[i].format & 0x3) << 30) | \
                   ((priv->config->erf_filter[i].filter.can_id) << 16) | \
                     priv->config->erf_filter[i].filter.can_mask;
          putreg32(regval, priv->base + FC7300_CAN_ERFFEL_OFFSET + i * 4);
        }
    }
  #endif

  putreg32(0x0, priv->base + FC7300_CAN_RXFGMASK_OFFSET);

  for (i = 0; i < priv->config->no_buffers; i++)
    {
      putreg32(0, priv->base + FC7300_CAN_RXIMR_OFFSET(i));
    }

  for (i = 0; i < priv->rxmb_cnt; i++)
    {
      putreg32((priv->config->rxmb_filter[i].can_mask & 0x7ff) << 18,
                priv->base + FC7300_CAN_RXIMR_OFFSET(i));
    }

#ifdef CONFIG_NET_CAN_CANFD
  if (priv->canfd_en)
    {
      struct canfdmb_s *mb = (struct canfdmb_s *)priv->rx;
      for (i = 0; i < priv->rxmb_cnt; i++)
        {
          ninfo("Set MB%" PRIu32 " to receive %p\n", i, &mb[i]);

          /* If the fields are configured in separate writes,
           * the MB_CS[CODE] must be the last write in the C/S word.
           */

          mb[i].id.w    = 0x0;
          mb[i].id.std  = priv->config->rxmb_filter[i].can_id & 0x7ff;
          mb[i].cs.edl  = 0x1;
          mb[i].cs.brs  = 0x1;
          mb[i].cs.esi  = 0x0;
          mb[i].cs.srr  = 0x0;
          mb[i].cs.ide  = 0x1;
          mb[i].cs.rtr  = 0x0;
          mb[i].cs.code = 0x4;
        }
    }
  else
#endif
    {
      struct canmb_s *mb = (struct canmb_s *)priv->rx;
      for (i = 0; i < priv->rxmb_cnt; i++)
        {
          ninfo("Set MB%" PRIu32 " to receive %p\n", i, &mb[i]);

          /* If the fields are configured in separate writes,
           * the MB_CS[CODE] must be the last write in the C/S word.
           */

          mb[i].id.w    = 0x0;
          mb[i].cs.edl  = 0x1;
          mb[i].cs.brs  = 0x1;
          mb[i].cs.esi  = 0x0;
          mb[i].cs.srr  = 0x0;
          mb[i].cs.ide  = 0x1;
          mb[i].cs.rtr  = 0x0;
          mb[i].cs.code = 0x4;
        }
    }

  putreg32(IFLAG1_RX, priv->base + FC7300_CAN_IFLAG1_OFFSET);
  putreg32(IFLAG1_RX, priv->base + FC7300_CAN_IMASK1_OFFSET);

  /* Exit freeze mode */

  fc7300_flexcan_setfreeze(priv->base, 0);

  return 1;
}

/****************************************************************************
 * Function: fc7300_flexcan_reset
 *
 * Description:
 *   Put the FLEXCAN in the non-operational, reset state
 *
 * Input Parameters:
 *   priv - Reference to the private FLEXCAN driver state structure
 *
 * Returned Value:
 *   None
 *
 * Assumptions:
 *
 ****************************************************************************/

static void fc7300_flexcan_reset(struct fc7300_netdev_s *priv)
{
  uint32_t regval;
  uint32_t i;
  uint32_t total_mb_cnt;

  total_mb_cnt = priv->txmb_cnt + priv->rxmb_cnt;

  regval  = getreg32(priv->base + FC7300_CAN_MCR_OFFSET);
  regval |= CAN_MCR_SOFTRST;
  putreg32(regval, priv->base + FC7300_CAN_MCR_OFFSET);

  if (!fc7300_flexcan_waitmcr_change(priv->base, CAN_MCR_SOFTRST, 0))
    {
      nerr("Reset failed, base = %#lx", priv->base);
      return;
    }

  regval  = getreg32(priv->base + FC7300_CAN_MCR_OFFSET);
  regval &= ~(CAN_MCR_SUPV);
  putreg32(regval, priv->base + FC7300_CAN_MCR_OFFSET);

  /* Initialize all MB rx and tx */

#ifdef CONFIG_NET_CAN_CANFD
  if (priv->canfd_en)
    {
      uint32_t j;
      struct canfdmb_s *mb = (struct canfdmb_s *)priv->rx;

      for (i = 0; i < total_mb_cnt; i++)
        {
          ninfo("MB %" PRIu32 " %p\n", i, &mb[i]);
          ninfo("MB %" PRIu32 " %p\n", i, &mb[i].id.w);
          mb[i].cs.cs = 0x0;
          mb[i].id.w  = 0x0;

          for (j = 0; j < 16; j++)
            {
              mb[i].data[j].w00 = 0x0;
            }
        }
    }
  else
#endif
    {
      struct canmb_s *mb = (struct canmb_s *)priv->rx;

      for (i = 0; i < total_mb_cnt; i++)
        {
          ninfo("MB %" PRIu32 " %p\n", i, &mb[i]);
          ninfo("MB %" PRIu32 " %p\n", i, &mb[i].id.w);
          mb[i].cs.cs = 0x0;
          mb[i].id.w  = 0x0;
          mb[i].data[0].w00 = 0x0;
          mb[i].data[1].w00 = 0x0;
        }
    }

  regval  = getreg32(priv->base + FC7300_CAN_MCR_OFFSET);
  regval &= ~CAN_MCR_MAXMB_MASK; /* Zero MAXMB to ensure "bitwise or"
                                  * below sets the correct value.
                                  */
  regval |= CAN_MCR_SLFWAK | CAN_MCR_WRNEN | CAN_MCR_SRXDIS |
            CAN_MCR_IRMQ | CAN_MCR_AEN |
            (((total_mb_cnt - 1) << CAN_MCR_MAXMB_SHIFT) &
             CAN_MCR_MAXMB_MASK);
  putreg32(regval, priv->base + FC7300_CAN_MCR_OFFSET);

  regval  = CAN_CTRL2_RRS | CAN_CTRL2_EACEN;
  putreg32(regval, priv->base + FC7300_CAN_CTRL2_OFFSET);

  for (i = 0; i < total_mb_cnt; i++)
    {
      putreg32(0, priv->base + FC7300_CAN_RXIMR_OFFSET(i));
    }

  /* Filtering catchall */

  putreg32(0x3fffffff, priv->base + FC7300_CAN_RX14MASK_OFFSET);
  putreg32(0x3fffffff, priv->base + FC7300_CAN_RX15MASK_OFFSET);
  putreg32(0x3fffffff, priv->base + FC7300_CAN_RXMGMASK_OFFSET);
  putreg32(0x0, priv->base + FC7300_CAN_RXFGMASK_OFFSET);
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Function: fc7300_socket_caninitialize
 *
 * Description:
 *   Initialize the CAN controller and driver
 *
 * Input Parameters:
 *   intf - In the case where there are multiple CAN devices, this value
 *          identifies which CAN device is to be initialized.
 *
 * Returned Value:
 *   OK on success; Negated errno on failure.
 *
 * Assumptions:
 *
 ****************************************************************************/

#ifdef CONFIG_FC7300_FLEXCAN0_SOCKET
extern struct flexcan_config_s g_fc7300_flexcan0_config;
#endif
#ifdef CONFIG_FC7300_FLEXCAN1_SOCKET
extern struct flexcan_config_s g_fc7300_flexcan1_config;
#endif
#ifdef CONFIG_FC7300_FLEXCAN2_SOCKET
extern struct flexcan_config_s g_fc7300_flexcan2_config;
#endif
#ifdef CONFIG_FC7300_FLEXCAN3_SOCKET
extern struct flexcan_config_s g_fc7300_flexcan3_config;
#endif
#ifdef CONFIG_FC7300_FLEXCAN4_SOCKET
extern struct flexcan_config_s g_fc7300_flexcan4_config;
#endif
#ifdef CONFIG_FC7300_FLEXCAN5_SOCKET
extern struct flexcan_config_s g_fc7300_flexcan5_config;
#endif
#ifdef CONFIG_FC7300_FLEXCAN6_SOCKET
extern struct flexcan_config_s g_fc7300_flexcan6_config;
#endif
#ifdef CONFIG_FC7300_FLEXCAN7_SOCKET
extern struct flexcan_config_s g_fc7300_flexcan7_config;
#endif
#ifdef CONFIG_FC7300_FLEXCAN8_SOCKET
extern struct flexcan_config_s g_fc7300_flexcan8_config;
#endif
#ifdef CONFIG_FC7300_FLEXCAN9_SOCKET
extern struct flexcan_config_s g_fc7300_flexcan9_config;
#endif

int fc7300_socket_caninitialize(int intf)
{
  struct fc7300_netdev_s *priv;

  switch (intf)
    {
#ifdef CONFIG_FC7300_FLEXCAN0_SOCKET
    case 0:
      priv                 = &g_flexcan0;
      memset(priv, 0, sizeof(struct fc7300_netdev_s));
      priv->base           = FC7300_FLEXCAN0_BASE;
      priv->avail_rxmb     = flexcan0_avail_rxmb;
      priv->rxmb_cnt       = FLEXCAN0_RXMB_COUNT;
      priv->tx_pkt_pending = flexcan0_tx_pkt_pending;
      priv->txmb_cnt       = FLEXCAN0_TXMB_COUNT;
      priv->config         = &g_fc7300_flexcan0_config;
#ifdef CONFIG_FC7300_FLEXCAN0_ERFEL_TOTAL_COUNT
      priv->erfel_total_cnt = CONFIG_FC7300_FLEXCAN0_ERFEL_TOTAL_COUNT;
#endif

#ifdef CONFIG_FC7300_FLEXCAN0_ENHANCED_RX_FIFO
      priv->erf_en        = true;
#else
      priv->erf_en        = false;
#endif

      /* Default bitrate configuration */

#  ifdef CONFIG_NET_CAN_CANFD
#    ifdef CONFIG_FC7300_FLEXCAN0_CANFD
      priv->canfd_en = true;
#      ifdef CONFIG_FC7300_FLEXCAN_CANFD_BRS
#        ifdef CONFIG_FC7300_FLEXCAN0_CANFD_BRS
      priv->canfdbrs_en = true;
      priv->data_timing.presdiv = CONFIG_FC7300_FLEXCAN0_CANFD_DATA_PRESDIV;
      priv->data_timing.propseg = CONFIG_FC7300_FLEXCAN0_CANFD_DATA_PROPSEG;
      priv->data_timing.pseg1   = CONFIG_FC7300_FLEXCAN0_CANFD_DATA_PSEG1;
      priv->data_timing.pseg2   = CONFIG_FC7300_FLEXCAN0_CANFD_DATA_PSEG2;
      priv->data_timing.sjw     = CONFIG_FC7300_FLEXCAN0_CANFD_DATA_SJW;
#        else
      priv->canfdbrs_en = false;
#        endif
#      endif
#    else
      priv->canfd_en = false;
#    endif
#  endif
      priv->arbi_timing.presdiv = CONFIG_FC7300_FLEXCAN0_PRESDIV;
      priv->arbi_timing.propseg = CONFIG_FC7300_FLEXCAN0_PROPSEG;
      priv->arbi_timing.pseg1   = CONFIG_FC7300_FLEXCAN0_PSEG1;
      priv->arbi_timing.pseg2   = CONFIG_FC7300_FLEXCAN0_PSEG2;
      priv->arbi_timing.sjw     = CONFIG_FC7300_FLEXCAN0_SJW;
      break;
#endif

#ifdef CONFIG_FC7300_FLEXCAN1_SOCKET
    case 1:
      priv                 = &g_flexcan1;
      memset(priv, 0, sizeof(struct fc7300_netdev_s));
      priv->base           = FC7300_FLEXCAN1_BASE;
      priv->avail_rxmb     = flexcan1_avail_rxmb;
      priv->rxmb_cnt       = FLEXCAN1_RXMB_COUNT;
      priv->tx_pkt_pending = flexcan1_tx_pkt_pending;
      priv->txmb_cnt       = FLEXCAN1_TXMB_COUNT;
      priv->config         = &g_fc7300_flexcan1_config;
#ifdef CONFIG_FC7300_FLEXCAN1_ERFEL_TOTAL_COUNT
      priv->erfel_total_cnt = CONFIG_FC7300_FLEXCAN1_ERFEL_TOTAL_COUNT;
#endif

#ifdef CONFIG_FC7300_FLEXCAN1_ENHANCED_RX_FIFO
      priv->erf_en        = true;
#else
      priv->erf_en        = false;
#endif

      /* Default bitrate configuration */

#  ifdef CONFIG_NET_CAN_CANFD
#    ifdef CONFIG_FC7300_FLEXCAN1_CANFD
      priv->canfd_en = true;
#      ifdef CONFIG_FC7300_FLEXCAN_CANFD_BRS
#        ifdef CONFIG_FC7300_FLEXCAN1_CANFD_BRS
      priv->canfdbrs_en = true;
      priv->data_timing.presdiv = CONFIG_FC7300_FLEXCAN1_CANFD_DATA_PRESDIV;
      priv->data_timing.propseg = CONFIG_FC7300_FLEXCAN1_CANFD_DATA_PROPSEG;
      priv->data_timing.pseg1   = CONFIG_FC7300_FLEXCAN1_CANFD_DATA_PSEG1;
      priv->data_timing.pseg2   = CONFIG_FC7300_FLEXCAN1_CANFD_DATA_PSEG2;
      priv->data_timing.sjw     = CONFIG_FC7300_FLEXCAN1_CANFD_DATA_SJW;
#        else
      priv->canfdbrs_en = false;
#        endif
#      endif
#    else
      priv->canfd_en = false;
#    endif
#  endif
      priv->arbi_timing.presdiv = CONFIG_FC7300_FLEXCAN1_PRESDIV;
      priv->arbi_timing.propseg = CONFIG_FC7300_FLEXCAN1_PROPSEG;
      priv->arbi_timing.pseg1   = CONFIG_FC7300_FLEXCAN1_PSEG1;
      priv->arbi_timing.pseg2   = CONFIG_FC7300_FLEXCAN1_PSEG2;
      priv->arbi_timing.sjw     = CONFIG_FC7300_FLEXCAN1_SJW;
      break;
#endif

#ifdef CONFIG_FC7300_FLEXCAN2_SOCKET
    case 2:
      priv                 = &g_flexcan2;
      memset(priv, 0, sizeof(struct fc7300_netdev_s));
      priv->base           = FC7300_FLEXCAN2_BASE;
      priv->avail_rxmb     = flexcan2_avail_rxmb;
      priv->rxmb_cnt       = FLEXCAN2_RXMB_COUNT;
      priv->tx_pkt_pending = flexcan2_tx_pkt_pending;
      priv->txmb_cnt       = FLEXCAN2_TXMB_COUNT;
      priv->config         = &g_fc7300_flexcan2_config;
#ifdef CONFIG_FC7300_FLEXCAN2_ERFEL_TOTAL_COUNT
      priv->erfel_total_cnt = CONFIG_FC7300_FLEXCAN2_ERFEL_TOTAL_COUNT;
#endif

#ifdef CONFIG_FC7300_FLEXCAN2_ENHANCED_RX_FIFO
      priv->erf_en        = true;
#else
      priv->erf_en        = false;
#endif

      /* Default bitrate configuration */

#  ifdef CONFIG_NET_CAN_CANFD
#    ifdef CONFIG_FC7300_FLEXCAN2_CANFD
      priv->canfd_en = true;
#      ifdef CONFIG_FC7300_FLEXCAN_CANFD_BRS
#        ifdef CONFIG_FC7300_FLEXCAN2_CANFD_BRS
      priv->canfdbrs_en = true;
      priv->data_timing.presdiv = CONFIG_FC7300_FLEXCAN2_CANFD_DATA_PRESDIV;
      priv->data_timing.propseg = CONFIG_FC7300_FLEXCAN2_CANFD_DATA_PROPSEG;
      priv->data_timing.pseg1   = CONFIG_FC7300_FLEXCAN2_CANFD_DATA_PSEG1;
      priv->data_timing.pseg2   = CONFIG_FC7300_FLEXCAN2_CANFD_DATA_PSEG2;
      priv->data_timing.sjw     = CONFIG_FC7300_FLEXCAN2_CANFD_DATA_SJW;
#        else
      priv->canfdbrs_en = false;
#        endif
#      endif
#    else
      priv->canfd_en = false;
#    endif
#  endif
      priv->arbi_timing.presdiv = CONFIG_FC7300_FLEXCAN2_PRESDIV;
      priv->arbi_timing.propseg = CONFIG_FC7300_FLEXCAN2_PROPSEG;
      priv->arbi_timing.pseg1   = CONFIG_FC7300_FLEXCAN2_PSEG1;
      priv->arbi_timing.pseg2   = CONFIG_FC7300_FLEXCAN2_PSEG2;
      priv->arbi_timing.sjw     = CONFIG_FC7300_FLEXCAN2_SJW;
      break;
#endif

#ifdef CONFIG_FC7300_FLEXCAN3_SOCKET
    case 3:
      priv                 = &g_flexcan3;
      memset(priv, 0, sizeof(struct fc7300_netdev_s));
      priv->base           = FC7300_FLEXCAN3_BASE;
      priv->avail_rxmb     = flexcan3_avail_rxmb;
      priv->rxmb_cnt       = FLEXCAN3_RXMB_COUNT;
      priv->tx_pkt_pending = flexcan3_tx_pkt_pending;
      priv->txmb_cnt       = FLEXCAN3_TXMB_COUNT;
      priv->config         = &g_fc7300_flexcan3_config;
#ifdef CONFIG_FC7300_FLEXCAN3_ERFEL_TOTAL_COUNT
      priv->erfel_total_cnt = CONFIG_FC7300_FLEXCAN3_ERFEL_TOTAL_COUNT;
#endif

#ifdef CONFIG_FC7300_FLEXCAN3_ENHANCED_RX_FIFO
      priv->erf_en        = true;
#else
      priv->erf_en        = false;
#endif

      /* Default bitrate configuration */

#  ifdef CONFIG_NET_CAN_CANFD
#    ifdef CONFIG_FC7300_FLEXCAN3_CANFD
      priv->canfd_en = true;
#      ifdef CONFIG_FC7300_FLEXCAN_CANFD_BRS
#        ifdef CONFIG_FC7300_FLEXCAN3_CANFD_BRS
      priv->canfdbrs_en = true;
      priv->data_timing.presdiv = CONFIG_FC7300_FLEXCAN3_CANFD_DATA_PRESDIV;
      priv->data_timing.propseg = CONFIG_FC7300_FLEXCAN3_CANFD_DATA_PROPSEG;
      priv->data_timing.pseg1   = CONFIG_FC7300_FLEXCAN3_CANFD_DATA_PSEG1;
      priv->data_timing.pseg2   = CONFIG_FC7300_FLEXCAN3_CANFD_DATA_PSEG2;
      priv->data_timing.sjw     = CONFIG_FC7300_FLEXCAN3_CANFD_DATA_SJW;
#        else
      priv->canfdbrs_en = false;
#        endif
#      endif
#    else
      priv->canfd_en = false;
#    endif
#  endif
      priv->arbi_timing.presdiv = CONFIG_FC7300_FLEXCAN3_PRESDIV;
      priv->arbi_timing.propseg = CONFIG_FC7300_FLEXCAN3_PROPSEG;
      priv->arbi_timing.pseg1   = CONFIG_FC7300_FLEXCAN3_PSEG1;
      priv->arbi_timing.pseg2   = CONFIG_FC7300_FLEXCAN3_PSEG2;
      priv->arbi_timing.sjw     = CONFIG_FC7300_FLEXCAN3_SJW;
      break;
#endif

#ifdef CONFIG_FC7300_FLEXCAN4_SOCKET
    case 4:
      priv                 = &g_flexcan4;
      memset(priv, 0, sizeof(struct fc7300_netdev_s));
      priv->base           = FC7300_FLEXCAN4_BASE;
      priv->avail_rxmb     = flexcan4_avail_rxmb;
      priv->rxmb_cnt       = FLEXCAN4_RXMB_COUNT;
      priv->tx_pkt_pending = flexcan4_tx_pkt_pending;
      priv->txmb_cnt       = FLEXCAN4_TXMB_COUNT;
      priv->config         = &g_fc7300_flexcan4_config;
#ifdef CONFIG_FC7300_FLEXCAN4_ERFEL_TOTAL_COUNT
      priv->erfel_total_cnt = CONFIG_FC7300_FLEXCAN4_ERFEL_TOTAL_COUNT;
#endif

#ifdef CONFIG_FC7300_FLEXCAN4_ENHANCED_RX_FIFO
      priv->erf_en        = true;
#else
      priv->erf_en        = false;
#endif

      /* Default bitrate configuration */

#  ifdef CONFIG_NET_CAN_CANFD
#    ifdef CONFIG_FC7300_FLEXCAN4_CANFD
      priv->canfd_en = true;
#      ifdef CONFIG_FC7300_FLEXCAN_CANFD_BRS
#        ifdef CONFIG_FC7300_FLEXCAN4_CANFD_BRS
      priv->canfdbrs_en = true;
      priv->data_timing.presdiv = CONFIG_FC7300_FLEXCAN4_CANFD_DATA_PRESDIV;
      priv->data_timing.propseg = CONFIG_FC7300_FLEXCAN4_CANFD_DATA_PROPSEG;
      priv->data_timing.pseg1   = CONFIG_FC7300_FLEXCAN4_CANFD_DATA_PSEG1;
      priv->data_timing.pseg2   = CONFIG_FC7300_FLEXCAN4_CANFD_DATA_PSEG2;
      priv->data_timing.sjw     = CONFIG_FC7300_FLEXCAN4_CANFD_DATA_SJW;
#        else
      priv->canfdbrs_en = false;
#        endif
#      endif
#    else
      priv->canfd_en = false;
#    endif
#  endif
      priv->arbi_timing.presdiv = CONFIG_FC7300_FLEXCAN4_PRESDIV;
      priv->arbi_timing.propseg = CONFIG_FC7300_FLEXCAN4_PROPSEG;
      priv->arbi_timing.pseg1   = CONFIG_FC7300_FLEXCAN4_PSEG1;
      priv->arbi_timing.pseg2   = CONFIG_FC7300_FLEXCAN4_PSEG2;
      priv->arbi_timing.sjw     = CONFIG_FC7300_FLEXCAN4_SJW;
      break;
#endif

#ifdef CONFIG_FC7300_FLEXCAN5_SOCKET
    case 5:
      priv                 = &g_flexcan5;
      memset(priv, 0, sizeof(struct fc7300_netdev_s));
      priv->base           = FC7300_FLEXCAN5_BASE;
      priv->avail_rxmb     = flexcan5_avail_rxmb;
      priv->rxmb_cnt       = FLEXCAN5_RXMB_COUNT;
      priv->tx_pkt_pending = flexcan5_tx_pkt_pending;
      priv->txmb_cnt       = FLEXCAN5_TXMB_COUNT;
      priv->config         = &g_fc7300_flexcan5_config;
#ifdef CONFIG_FC7300_FLEXCAN5_ERFEL_TOTAL_COUNT
      priv->erfel_total_cnt = CONFIG_FC7300_FLEXCAN5_ERFEL_TOTAL_COUNT;
#endif

#ifdef CONFIG_FC7300_FLEXCAN5_ENHANCED_RX_FIFO
      priv->erf_en        = true;
#else
      priv->erf_en        = false;
#endif

      /* Default bitrate configuration */

#  ifdef CONFIG_NET_CAN_CANFD
#    ifdef CONFIG_FC7300_FLEXCAN5_CANFD
      priv->canfd_en = true;
#      ifdef CONFIG_FC7300_FLEXCAN_CANFD_BRS
#        ifdef CONFIG_FC7300_FLEXCAN5_CANFD_BRS
      priv->canfdbrs_en = true;
      priv->data_timing.presdiv = CONFIG_FC7300_FLEXCAN5_CANFD_DATA_PRESDIV;
      priv->data_timing.propseg = CONFIG_FC7300_FLEXCAN5_CANFD_DATA_PROPSEG;
      priv->data_timing.pseg1   = CONFIG_FC7300_FLEXCAN5_CANFD_DATA_PSEG1;
      priv->data_timing.pseg2   = CONFIG_FC7300_FLEXCAN5_CANFD_DATA_PSEG2;
      priv->data_timing.sjw     = CONFIG_FC7300_FLEXCAN5_CANFD_DATA_SJW;
#        else
      priv->canfdbrs_en = false;
#        endif
#      endif
#    else
      priv->canfd_en = false;
#    endif
#  endif
      priv->arbi_timing.presdiv = CONFIG_FC7300_FLEXCAN5_PRESDIV;
      priv->arbi_timing.propseg = CONFIG_FC7300_FLEXCAN5_PROPSEG;
      priv->arbi_timing.pseg1   = CONFIG_FC7300_FLEXCAN5_PSEG1;
      priv->arbi_timing.pseg2   = CONFIG_FC7300_FLEXCAN5_PSEG2;
      priv->arbi_timing.sjw     = CONFIG_FC7300_FLEXCAN5_SJW;
      break;
#endif

#ifdef CONFIG_FC7300_FLEXCAN6_SOCKET
    case 6:
      priv                 = &g_flexcan6;
      memset(priv, 0, sizeof(struct fc7300_netdev_s));
      priv->base           = FC7300_FLEXCAN6_BASE;
      priv->avail_rxmb     = flexcan6_avail_rxmb;
      priv->rxmb_cnt       = FLEXCAN6_RXMB_COUNT;
      priv->tx_pkt_pending = flexcan6_tx_pkt_pending;
      priv->txmb_cnt       = FLEXCAN6_TXMB_COUNT;
      priv->config         = &g_fc7300_flexcan6_config;
#ifdef CONFIG_FC7300_FLEXCAN6_ERFEL_TOTAL_COUNT
      priv->erfel_total_cnt = CONFIG_FC7300_FLEXCAN6_ERFEL_TOTAL_COUNT;
#endif

#ifdef CONFIG_FC7300_FLEXCAN6_ENHANCED_RX_FIFO
      priv->erf_en        = true;
#else
      priv->erf_en        = false;
#endif

      /* Default bitrate configuration */

#  ifdef CONFIG_NET_CAN_CANFD
#    ifdef CONFIG_FC7300_FLEXCAN6_CANFD
      priv->canfd_en = true;
#      ifdef CONFIG_FC7300_FLEXCAN_CANFD_BRS
#        ifdef CONFIG_FC7300_FLEXCAN6_CANFD_BRS
      priv->canfdbrs_en = true;
      priv->data_timing.presdiv = CONFIG_FC7300_FLEXCAN6_CANFD_DATA_PRESDIV;
      priv->data_timing.propseg = CONFIG_FC7300_FLEXCAN6_CANFD_DATA_PROPSEG;
      priv->data_timing.pseg1   = CONFIG_FC7300_FLEXCAN6_CANFD_DATA_PSEG1;
      priv->data_timing.pseg2   = CONFIG_FC7300_FLEXCAN6_CANFD_DATA_PSEG2;
      priv->data_timing.sjw     = CONFIG_FC7300_FLEXCAN6_CANFD_DATA_SJW;
#        else
      priv->canfdbrs_en = false;
#        endif
#      endif
#    else
      priv->canfd_en = false;
#    endif
#  endif
      priv->arbi_timing.presdiv = CONFIG_FC7300_FLEXCAN6_PRESDIV;
      priv->arbi_timing.propseg = CONFIG_FC7300_FLEXCAN6_PROPSEG;
      priv->arbi_timing.pseg1   = CONFIG_FC7300_FLEXCAN6_PSEG1;
      priv->arbi_timing.pseg2   = CONFIG_FC7300_FLEXCAN6_PSEG2;
      priv->arbi_timing.sjw     = CONFIG_FC7300_FLEXCAN6_SJW;
      break;
#endif

#ifdef CONFIG_FC7300_FLEXCAN7_SOCKET
    case 7:
      priv                 = &g_flexcan7;
      memset(priv, 0, sizeof(struct fc7300_netdev_s));
      priv->base           = FC7300_FLEXCAN7_BASE;
      priv->avail_rxmb     = flexcan7_avail_rxmb;
      priv->rxmb_cnt       = FLEXCAN7_RXMB_COUNT;
      priv->tx_pkt_pending = flexcan7_tx_pkt_pending;
      priv->txmb_cnt       = FLEXCAN7_TXMB_COUNT;
      priv->config         = &g_fc7300_flexcan7_config;
#ifdef CONFIG_FC7300_FLEXCAN7_ERFEL_TOTAL_COUNT
      priv->erfel_total_cnt = CONFIG_FC7300_FLEXCAN7_ERFEL_TOTAL_COUNT;
#endif

#ifdef CONFIG_FC7300_FLEXCAN7_ENHANCED_RX_FIFO
      priv->erf_en        = true;
#else
      priv->erf_en        = false;
#endif

      /* Default bitrate configuration */

#  ifdef CONFIG_NET_CAN_CANFD
#    ifdef CONFIG_FC7300_FLEXCAN7_CANFD
      priv->canfd_en = true;
#      ifdef CONFIG_FC7300_FLEXCAN_CANFD_BRS
#        ifdef CONFIG_FC7300_FLEXCAN7_CANFD_BRS
      priv->canfdbrs_en = true;
      priv->data_timing.presdiv = CONFIG_FC7300_FLEXCAN7_CANFD_DATA_PRESDIV;
      priv->data_timing.propseg = CONFIG_FC7300_FLEXCAN7_CANFD_DATA_PROPSEG;
      priv->data_timing.pseg1   = CONFIG_FC7300_FLEXCAN7_CANFD_DATA_PSEG1;
      priv->data_timing.pseg2   = CONFIG_FC7300_FLEXCAN7_CANFD_DATA_PSEG2;
      priv->data_timing.sjw     = CONFIG_FC7300_FLEXCAN7_CANFD_DATA_SJW;
#        else
      priv->canfdbrs_en = false;
#        endif
#      endif
#    else
      priv->canfd_en = false;
#    endif
#  endif
      priv->arbi_timing.presdiv = CONFIG_FC7300_FLEXCAN7_PRESDIV;
      priv->arbi_timing.propseg = CONFIG_FC7300_FLEXCAN7_PROPSEG;
      priv->arbi_timing.pseg1   = CONFIG_FC7300_FLEXCAN7_PSEG1;
      priv->arbi_timing.pseg2   = CONFIG_FC7300_FLEXCAN7_PSEG2;
      priv->arbi_timing.sjw     = CONFIG_FC7300_FLEXCAN7_SJW;
      break;
#endif

#ifdef CONFIG_FC7300_FLEXCAN8_SOCKET
    case 8:
      priv                 = &g_flexcan8;
      memset(priv, 0, sizeof(struct fc7300_netdev_s));
      priv->base           = FC7300_FLEXCAN8_BASE;
      priv->avail_rxmb     = flexcan8_avail_rxmb;
      priv->rxmb_cnt       = FLEXCAN8_RXMB_COUNT;
      priv->tx_pkt_pending = flexcan8_tx_pkt_pending;
      priv->txmb_cnt       = FLEXCAN8_TXMB_COUNT;
      priv->config         = &g_fc7300_flexcan8_config;
#ifdef CONFIG_FC7300_FLEXCAN8_ERFEL_TOTAL_COUNT
      priv->erfel_total_cnt = CONFIG_FC7300_FLEXCAN8_ERFEL_TOTAL_COUNT;
#endif

#ifdef CONFIG_FC7300_FLEXCAN8_ENHANCED_RX_FIFO
      priv->erf_en        = true;
#else
      priv->erf_en        = false;
#endif

      /* Default bitrate configuration */

#  ifdef CONFIG_NET_CAN_CANFD
#    ifdef CONFIG_FC7300_FLEXCAN8_CANFD
      priv->canfd_en = true;
#      ifdef CONFIG_FC7300_FLEXCAN_CANFD_BRS
#        ifdef CONFIG_FC7300_FLEXCAN8_CANFD_BRS
      priv->canfdbrs_en = true;
      priv->data_timing.presdiv = CONFIG_FC7300_FLEXCAN8_CANFD_DATA_PRESDIV;
      priv->data_timing.propseg = CONFIG_FC7300_FLEXCAN8_CANFD_DATA_PROPSEG;
      priv->data_timing.pseg1   = CONFIG_FC7300_FLEXCAN8_CANFD_DATA_PSEG1;
      priv->data_timing.pseg2   = CONFIG_FC7300_FLEXCAN8_CANFD_DATA_PSEG2;
      priv->data_timing.sjw     = CONFIG_FC7300_FLEXCAN8_CANFD_DATA_SJW;
#        else
      priv->canfdbrs_en = false;
#        endif
#      endif
#    else
      priv->canfd_en = false;
#    endif
#  endif
      priv->arbi_timing.presdiv = CONFIG_FC7300_FLEXCAN8_PRESDIV;
      priv->arbi_timing.propseg = CONFIG_FC7300_FLEXCAN8_PROPSEG;
      priv->arbi_timing.pseg1   = CONFIG_FC7300_FLEXCAN8_PSEG1;
      priv->arbi_timing.pseg2   = CONFIG_FC7300_FLEXCAN8_PSEG2;
      priv->arbi_timing.sjw     = CONFIG_FC7300_FLEXCAN8_SJW;
      break;
#endif

#ifdef CONFIG_FC7300_FLEXCAN9_SOCKET
    case 9:
      priv                 = &g_flexcan9;
      memset(priv, 0, sizeof(struct fc7300_netdev_s));
      priv->base           = FC7300_FLEXCAN9_BASE;
      priv->avail_rxmb     = flexcan9_avail_rxmb;
      priv->rxmb_cnt       = FLEXCAN9_RXMB_COUNT;
      priv->tx_pkt_pending = flexcan9_tx_pkt_pending;
      priv->txmb_cnt       = FLEXCAN9_TXMB_COUNT;
      priv->config         = &g_fc7300_flexcan9_config;
#ifdef CONFIG_FC7300_FLEXCAN9_ERFEL_TOTAL_COUNT
      priv->erfel_total_cnt = CONFIG_FC7300_FLEXCAN9_ERFEL_TOTAL_COUNT;
#endif

#ifdef CONFIG_FC7300_FLEXCAN9_ENHANCED_RX_FIFO
      priv->erf_en        = true;
#else
      priv->erf_en        = false;
#endif

      /* Default bitrate configuration */

#  ifdef CONFIG_NET_CAN_CANFD
#    ifdef CONFIG_FC7300_FLEXCAN9_CANFD
      priv->canfd_en = true;
#      ifdef CONFIG_FC7300_FLEXCAN_CANFD_BRS
#        ifdef CONFIG_FC7300_FLEXCAN9_CANFD_BRS
      priv->canfdbrs_en = true;
      priv->data_timing.presdiv = CONFIG_FC7300_FLEXCAN9_CANFD_DATA_PRESDIV;
      priv->data_timing.propseg = CONFIG_FC7300_FLEXCAN9_CANFD_DATA_PROPSEG;
      priv->data_timing.pseg1   = CONFIG_FC7300_FLEXCAN9_CANFD_DATA_PSEG1;
      priv->data_timing.pseg2   = CONFIG_FC7300_FLEXCAN9_CANFD_DATA_PSEG2;
      priv->data_timing.sjw     = CONFIG_FC7300_FLEXCAN9_CANFD_DATA_SJW;
#        else
      priv->canfdbrs_en = false;
#        endif
#      endif
#    else
      priv->canfd_en = false;
#    endif
#  endif
      priv->arbi_timing.presdiv = CONFIG_FC7300_FLEXCAN9_PRESDIV;
      priv->arbi_timing.propseg = CONFIG_FC7300_FLEXCAN9_PROPSEG;
      priv->arbi_timing.pseg1   = CONFIG_FC7300_FLEXCAN9_PSEG1;
      priv->arbi_timing.pseg2   = CONFIG_FC7300_FLEXCAN9_PSEG2;
      priv->arbi_timing.sjw     = CONFIG_FC7300_FLEXCAN9_SJW;
      break;
#endif

    default:
      return -ENODEV;
    }

  fc7300_pinconfig(priv->config->tx_pin);
  fc7300_pinconfig(priv->config->rx_pin);

  /* Attach the flexcan interrupt handler */

  if (irq_attach(priv->config->irq, fc7300_flexcan_interrupt, priv))
    {
      /* We could not attach the ISR to the interrupt */

      nerr("ERROR: Failed to attach CAN OR CANFD Message buffer IRQ\n");
      return -EAGAIN;
    }

  /* Initialize the driver structure */

  priv->dev.quota[NETPKT_TX] = priv->txmb_cnt;
  priv->dev.quota[NETPKT_RX] = priv->rxmb_cnt;
  priv->dev.ops       = &g_ops;
  priv->rx            = (uint32_t *)(priv->base + FC7300_CAN_MB_OFFSET);

#ifdef CONFIG_NET_CAN_CANFD
  if (priv->canfd_en)
    {
      priv->tx        = (uint32_t *)(priv->base + FC7300_CAN_MB_OFFSET +
                                (sizeof(struct canfdmb_s) * priv->rxmb_cnt));
    }
  else
#endif
    {
      priv->tx        = (uint32_t *)(priv->base + FC7300_CAN_MB_OFFSET +
                                (sizeof(struct canmb_s) * priv->rxmb_cnt));
    }

  priv->rxmb_head = 0;
  priv->rxmb_tail = 0;
  priv->rxmb_pending = 0;
  priv->erf_pending = false;

  /* Put the interface in the down state.  This usually amounts to resetting
   * the device and/or calling fc7300_ifdown().
   */

  fc7300_flexcan_initialize(priv);

  fc7300_ifdown(&priv->dev);

  /* Register the device with the OS so that socket IOCTLs can be performed */

  snprintf(priv->dev.netdev.d_ifname, IFNAMSIZ, "can%d", intf);
  netdev_lower_register(&priv->dev, NET_LL_CAN);

  return OK;
}

#endif /* CONFIG_FC7300_FLEXCAN */
