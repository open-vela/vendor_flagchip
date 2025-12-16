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

#include <inttypes.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <debug.h>
#include <errno.h>

#include <nuttx/lin.h>
#include <nuttx/irq.h>
#include <nuttx/arch.h>
#include <nuttx/wqueue.h>
#include <nuttx/net/net.h>
#include <nuttx/net/netdev.h>
#include <nuttx/net/netdev_lowerhalf.h>
#include <nuttx/net/pkt.h>
#include <nuttx/net/ioctl.h>
#include <net/if.h>

#include "arm_internal.h"
#include "chip.h"
#include "fc7300_config.h"
#include "hardware/fc7300_fcuart.h"
#include "hardware/fc7300_pinmux.h"
#include "fc7300_periphclocks.h"
#include "fc7300_clockconfig.h"
#include "hardware/fc7300_pin.h"
#include "fc7300_lin.h"
#include "fc7300_irq.h"
#include <arch/board/board.h>

#include <sys/time.h>

#ifdef CONFIG_FC7300_LIN

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* LIN driver internal macros */

#define LIN_MAX_BITS_PER_FRAME          (172u)
#define LIN_MAX_NUM_PID                 (1u << LIN_ID_BITS)
#define LIN_MAX_DATA_LENGTH             CAN_MAX_DLEN
#define LIN_ERR_DATA_LENGTH             CAN_ERR_DLC
#define LIN_FRAME_SYNC_BYTE             0x55u
#define LIN_MASTER_DIAG_REQ_PID         0x3cu
#define LIN_SLAVE_DIAG_RESP_PID         0x7du

/* Receive data availabl flags */

#define LIN_FRAME_RX_AVAILABLE(priv)    ((priv)->rx_ready)
#define LIN_FRAME_RX_VALIDATE(priv)     ((priv)->rx_ready = true)
#define LIN_FRAME_RX_INVALIDATE(priv)   ((priv)->rx_ready = false)

/* LIN error id or event id. The value 0 indicates successful operation, the
 * values 1~7 are consistent with enum lin_state_event_e for state change
 * events, and other values indicate error events defined in lin.h.
 */

#define FC7300_LIN_SUCCESS              0u
#define FC7300_LIN_EV_WKUP              1u
#define FC7300_LIN_EV_WKUP_BUS          2u
#define FC7300_LIN_EV_SLEEP             3u
#define FC7300_LIN_EV_SLEEP_BUS         4u
#define FC7300_LIN_EV_SLEEP_IDLE        5u
#define FC7300_LIN_EV_MAXID             6u
#define FC7300_LIN_TX_OK                7u
#define FC7300_LIN_ERR_START            8u
#define FC7300_LIN_ERR_BREAK_TMO        8u
#define FC7300_LIN_ERR_TXSYNC_TMO       9u
#define FC7300_LIN_ERR_TXPID_TMO        10u
#define FC7300_LIN_ERR_TXD_TMO          11u
#define FC7300_LIN_ERR_TXCKSUM_TMO      12u
#define FC7300_LIN_ERR_RXNORESP         16u
#define FC7300_LIN_ERR_RXRESPONSE       17u
#define FC7300_LIN_ERR_RXCKSUM_TMO      18u
#define FC7300_LIN_ERR_RXCKSUM          19u
#define FC7300_LIN_ERR_RXSYNC_TMO       20u
#define FC7300_LIN_ERR_RXSYNC           21u
#define FC7300_LIN_ERR_RXPID_TMO        22u
#define FC7300_LIN_ERR_PIDPARITY        23u
#define FC7300_LIN_ERR_TXPID            24u
#define FC7300_LIN_ERR_TXCKSUM          25u
#define FC7300_LIN_ERR_TXSYNC           26u
#define FC7300_LIN_ERR_TXDATA           27u
#define FC7300_LIN_ERR_TXLEN            28u
#define FC7300_LIN_ERR_RXLEN            29u
#define FC7300_LIN_ERR_SLVBUSY          30u
#define FC7300_LIN_ERR_CTRL_RXOVERFLOW  32u
#define FC7300_LIN_ERR_CTRL_FRAMEERROR  33u
#define FC7300_LIN_ERR_CTRL_NOISEERROR  34u
#define FC7300_LIN_ERR_UNKNOWN          255u

/* Get slave node attributes */

#define FC7300_LIN_EXT_CKSUM            0x01u
#define FC7300_LIN_RESP_RX              0x02u
#define FC7300_LIN_SINGLE_RESP          0x04u

#define LIN_CLEAR_SLV_ATTRIBUTE(priv,pid) \
            (((priv)->slv[(pid)].attr) = 0u)

#define LIN_SET_SLV_EXT_CKSUM(priv,pid) \
            (((priv)->slv[(pid)].attr) |= FC7300_LIN_EXT_CKSUM)

#define LIN_SET_SLV_RESP_RX(priv,pid) \
            (((priv)->slv[(pid)].attr) |= FC7300_LIN_RESP_RX)

#define LIN_SET_SLV_SINGLE_RESP(priv,pid) \
            (((priv)->slv[(pid)].attr) |= FC7300_LIN_SINGLE_RESP)

/* Macros for register access */

#define READ_REG_U32(a)         getreg32(a)
#define WRITE_REG_U32(a, v)     putreg32(v, a)
#define MODIFY_REG_U32(a, v, m) modreg32(v, m, a)
#define CLEAR_REG_BITS(a, m)    putreg32((getreg32(a) & ~(m)), (a))
#define SET_REG_BITS(a, m)      putreg32((getreg32(a) | (m)), (a))
#define READ_REG_BITS(a, m)     (getreg32(a) & (m))
#define READ_REG_SHIFT(a, m, s) ((getreg32(a) & (m)) >> (s))

/* LIN driver state */

#define FC7300_LIN_SETIFUP(p)     ((p)->state = CAN_STATE_SLEEP)
#define FC7300_LIN_SETIFDOWN(p)   ((p)->state = 0u)
#define FC7300_LIN_SETNORMAL(p)   ((p)->state = CAN_STATE_OPERATIONAL)
#define FC7300_LIN_SETBUSY(p)     ((p)->state = CAN_STATE_BUSY)
#define FC7300_LIN_SETSPENDING(p) ((p)->state = CAN_STATE_SPENDING)
#define FC7300_LIN_SETSLEEP(p)    ((p)->state = CAN_STATE_SLEEP)
#define FC7300_LIN_ISUP(p)        ((p)->state != 0u)
#define FC7300_LIN_ISNORMAL(p)    ((p)->state == CAN_STATE_OPERATIONAL)
#define FC7300_LIN_ISBUSY(p)      ((p)->state == CAN_STATE_BUSY)
#define FC7300_LIN_ISSPENDING(p)  ((p)->state == CAN_STATE_SPENDING)
#define FC7300_LIN_ISSLEEP(p)     ((p)->state == CAN_STATE_SLEEP)

/* LIN frame state */

#define FC7300_LIN_FRAME_IDLE         0u
#define FC7300_LIN_FRAME_PEOCESSING   1u
#define FC7300_LIN_FRAME_SEND_BREAK   2u
#define FC7300_LIN_FRAME_RECV_HEADER  3u
#define FC7300_LIN_FRAME_RECV_PID     4u
#define FC7300_LIN_FRAME_RECV_DATA    5u
#define FC7300_LIN_FRAME_SEND_DATA    6u
#define FC7300_LIN_FRAME_WAKEUP       7u
#define FC7300_LIN_FRAME_EVENT        8u

/* LIN TX RX buffer */

#define FC7300_LIN_BUF_LEN            15u
#define FC7300_LIN_BUF_FLAG(p)        ((p)->buf[0u])          /* Flag */
#define FC7300_LIN_BUF_DLC(p)         ((p)->buf[1u])          /* Length */
#define FC7300_LIN_BUF_IND(p)         ((p)->buf[2u])          /* Index */
#define FC7300_LIN_BUF_EVT(p)         ((p)->buf[3u])          /* Event */
#define FC7300_LIN_BUF_SYNC(p)        ((p)->buf[4u])          /* Sync */
#define FC7300_LIN_BUF_PSYNC(p)       (&((p)->buf[4u]))       /* pSync */
#define FC7300_LIN_BUF_PID(p)         ((p)->buf[5u])          /* PID */
#define FC7300_LIN_BUF_PPID(p)        (&((p)->buf[5u]))       /* PPID */
#define FC7300_LIN_BUF_DATA(p,i)      ((p)->buf[6u + (i)])    /* Data */
#define FC7300_LIN_BUF_PDATA(p,i)     (&((p)->buf[6u + (i)])) /* pData */
#define FC7300_LIN_BUF_CKSUM(p,l)     ((p)->buf[6u + (l)])    /* Checksum */

/* LIN hardware features */

#define FC7300_LIN_ERROR_MASK \
              (FCUART_STAT_FEF|FCUART_STAT_NF|FCUART_STAT_RORF)
#define FC7300_LIN_FIFO_DEPTH         16u

/****************************************************************************
 * Private Types
 ****************************************************************************/

/* LIN const configuration structure */

struct fc7300_linconfig_s
{
  int irq_num;            /* Interrupt number */
  int irq_priority;       /* Interrupt priority */
  uint32_t tx_pin;        /* GPIO configuration for TX */
  uint32_t rx_pin;        /* GPIO configuration for RX */
  uint32_t enable_pin;    /* GPIO configuration for LIN enable */
  uint32_t enable_level;  /* LIN enable pin output level(1/0) */
};

/* Data cache and node information in slave mode */

struct fc7300_linslv_s
{
  uint8_t data[LIN_MAX_DATA_LENGTH];  /* Slave cached response data */
  uint8_t len;                        /* Slave response data size */
  uint8_t attr;                       /* Pid attributes */
  uint8_t checksum;                   /* Data checksum */
};

/* The fc7300_lindev_s encapsulates all state information for a single
 * hardware interface
 */

struct fc7300_lindev_s
{
  struct netdev_lowerhalf_s dev;
  const struct fc7300_linconfig_s *cfg;
  uint32_t base;          /* FCUART base address */
  uint32_t srcclk;        /* LIN bus clock source frequency */
  uint32_t baudrate;      /* LIN bus baudrate */
  bool bmaster;           /* Master or slave node */
  uint8_t state;          /* Driver state */
  uint8_t frame_state;    /* Frame state */
  bool rx_ready;          /* Indicate frame is ready for receive */
  uint16_t timeout;       /* Time out value in tick */
  bool reporting;         /* The driver is reporting an error */
  uint8_t ovsmp;          /* Over sample vaue */
  uint16_t sbr;           /* Clock divisor */
 #ifdef CONFIG_FC7300_LIN_BREAK_SOFTWARE
  uint16_t sbr_brk;       /* Clock divisor when send break */
  uint32_t waitcycles;    /* Wait cycles when change baudrate */
#endif

  uint8_t buf[FC7300_LIN_BUF_LEN];  /* Send or receive buffer */

  /* Net packets need to be released at the end of the transmission,
   * regardless of whether the transmission was successful or failed.
   */

  netpkt_t *pkt;

  struct fc7300_linslv_s *slv; /* Slave node info and cache data */

  spinlock_t lock;
  struct wdog_s timer;    /* Frame or idle sleep timer */
};

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

static int fc7300_lin_ifup(struct netdev_lowerhalf_s *dev);

static int fc7300_lin_ifdown(struct netdev_lowerhalf_s *dev);

static int fc7300_lin_transmit(struct netdev_lowerhalf_s *dev,
                               netpkt_t *pkt);

static netpkt_t *fc7300_lin_receive(struct netdev_lowerhalf_s *dev);

#ifdef CONFIG_NETDEV_IOCTL
static int fc7300_lin_ioctl(struct netdev_lowerhalf_s *dev, int cmd,
                            unsigned long arg);
#endif /* CONFIG_NETDEV_IOCTL */

#ifdef CONFIG_NETDEV_CAN_STATE_IOCTL
static void fc7300_lin_sendwakeupsignal(struct fc7300_lindev_s *priv);

static int fc7300_lin_ioctlwakeup(struct fc7300_lindev_s *priv,
                                  bool wakeupinternal);

static int fc7300_lin_sendsleepsignal(struct fc7300_lindev_s *priv);

static int fc7300_lin_gotosleep(struct fc7300_lindev_s *priv,
                                bool sleepinternal);
#endif /* CONFIG_NETDEV_CAN_STATE_IOCTL */

static void fc7300_lin_gotosleepinternal(struct fc7300_lindev_s *priv);

static bool fc7300_lin_checksleepsignal(struct fc7300_lindev_s *priv,
                                        uint8_t event);

static bool fc7300_lin_checkwakeupsignal(struct fc7300_lindev_s *priv);

static void fc7300_lin_frametimeout(wdparm_t arg);

#ifdef CONFIG_FC7300_LIN_IDLE_TO_SLEEP
static void fc7300_lin_busidlesleep(wdparm_t arg);
#endif

static int fc7300_lin_startframetimer(struct fc7300_lindev_s *priv,
                                      uint32_t timeout);

static int fc7300_lin_startidlesleeptimer(struct fc7300_lindev_s *priv);

static void fc7300_lin_canceltimer(struct fc7300_lindev_s *priv);

static int fc7300_lin_errorhandler(struct fc7300_lindev_s *priv,
                                   uint32_t status);

static int fc7300_lin_rxdactivehandler(struct fc7300_lindev_s *priv);

static int fc7300_lin_idlelinehandler(struct fc7300_lindev_s *priv);

static int fc7300_lin_breakeventhandler(struct fc7300_lindev_s *priv);

static int fc7300_lin_receivesynchandler(struct fc7300_lindev_s *priv,
                                          uint8_t rxbyte);

static void fc7300_lin_masterpidhandler(struct fc7300_lindev_s *priv,
                                        uint8_t rxbyte);

static void fc7300_lin_slavepidhandler(struct fc7300_lindev_s *priv,
                                       uint8_t rxbyte);

static void fc7300_lin_receivepidhandler(struct fc7300_lindev_s *priv,
                                         uint8_t rxbyte);

static int fc7300_lin_framehandler(struct fc7300_lindev_s *priv);

#ifdef CONFIG_FC7300_LIN_ISR_THREAD
static int fc7300_lin_isr(int irq, void *context, void *arg);
#endif

static int fc7300_lin_interrupt(int irq, void *context, void *arg);

static uint8_t fc7300_lin_datalength(uint8_t pid, uint8_t can_dlc);

static uint8_t fc7300_lin_pidparity(uint8_t pid);

static uint8_t fc7300_lin_datachecksum(bool extcksum, uint8_t pid,
                                       const uint8_t *data, uint8_t len);

static void fc7300_lin_slavectrl(struct fc7300_lindev_s *priv,
                                 const struct can_frame *frame);

static void fc7300_lin_buildeventframe(struct fc7300_lindev_s *priv,
                                       struct can_frame *frame);

static void fc7300_lin_buildframe(struct fc7300_lindev_s *priv,
                                  struct can_frame *frame);

static inline void fc7300_lin_freenetpkt(struct fc7300_lindev_s *priv);

static void fc7300_lin_reportevent(struct fc7300_lindev_s *priv,
                                   uint8_t event);

static int fc7300_lin_sendheader(struct fc7300_lindev_s *priv);

static void fc7300_lin_senddata(struct fc7300_lindev_s *priv,
                                uint8_t *buf, uint8_t len);

static void fc7300_lin_receivedata(struct fc7300_lindev_s *priv,
                                   uint8_t *buf, uint8_t len);

static int fc7300_lin_parseframe(struct fc7300_lindev_s *priv,
                                 const struct can_frame *frame);

static struct fc7300_lindev_s *fc7300_lin_privsetup(int intf);

static void fc7300_lin_pinconfig(struct fc7300_lindev_s *priv);

static void fc7300_lin_setidle(struct fc7300_lindev_s *priv);

static inline void fc7300_lin_breakdetect(struct fc7300_lindev_s *priv,
                                          bool benable);

static inline void fc7300_lin_idlelinedetect(struct fc7300_lindev_s *priv);

static inline void fc7300_lin_setrxwatermark(struct fc7300_lindev_s *priv,
                                             uint8_t watermark);

static inline void fc7300_lin_filltxfifo(struct fc7300_lindev_s *priv,
                                        uint8_t *data, uint8_t len);

static inline uint8_t fc7300_lin_readrxfifo(struct fc7300_lindev_s *priv,
                                            uint8_t *buf);

static inline void fc7300_lin_softreset(struct fc7300_lindev_s *priv);

static int fc7300_lin_irqattach(struct fc7300_lindev_s *priv);

static int fc7300_lin_irqdetach(struct fc7300_lindev_s *priv);

static int fc7300_lin_calculate_baudrate(struct fc7300_lindev_s *priv,
                                    uint8_t *oversamp, uint16_t *sbr);

#ifdef CONFIG_FC7300_LIN_BREAK_SOFTWARE
static void fc7300_lin_sendbreakbybaud(struct fc7300_lindev_s *priv);

static void fc7300_lin_breakeventbybaud(struct fc7300_lindev_s *priv);

static void fc7300_lin_change_baudrate(struct fc7300_lindev_s *priv);

static void fc7300_lin_recover_baudrate(struct fc7300_lindev_s *priv);
#endif /* CONFIG_FC7300_LIN_BREAK_SOFTWARE */

static void fc7300_lin_calculatetimeout(struct fc7300_lindev_s *priv);

static int fc7300_lin_controller_init(struct fc7300_lindev_s *priv);

/****************************************************************************
 * Private Data
 ****************************************************************************/

static const struct netdev_ops_s g_fc7300linops =
{
  fc7300_lin_ifup,      /* ifup */
  fc7300_lin_ifdown,    /* ifdown */
  fc7300_lin_transmit,  /* transmit */
  fc7300_lin_receive    /* receive */
#ifdef CONFIG_NETDEV_IOCTL
  ,
  fc7300_lin_ioctl      /* ioctl */
#endif
};

#ifdef CONFIG_FC7300_LIN0
static const struct fc7300_linconfig_s g_fc7300lin0config =
{
  .irq_num = FC7300_IRQ_FCUART0,
  .irq_priority = NVIC_LIN0_PRIORITY_DEFAULT,
  .tx_pin = PIN_LIN0_TX,
  .rx_pin = PIN_LIN0_RX,
#ifdef PIN_LIN0_ENABLE
  .enable_pin = PIN_LIN0_ENABLE,
  .enable_level = LIN0_ENABLE_OUT
#else
  .enable_pin = 0u,
  .enable_level = 0u
#endif /* PIN_LIN0_ENABLE */
};
#ifndef CONFIG_FC7300_LIN0_MASTER
static struct fc7300_linslv_s g_fc7300lin0slave[LIN_MAX_NUM_PID];
#endif
static struct fc7300_lindev_s g_fc7300lin0;
#endif

#ifdef CONFIG_FC7300_LIN1
static const struct fc7300_linconfig_s g_fc7300lin1config =
{
  .irq_num = FC7300_IRQ_FCUART1,
  .irq_priority = NVIC_LIN1_PRIORITY_DEFAULT,
  .tx_pin = PIN_LIN1_TX,
  .rx_pin = PIN_LIN1_RX,
#ifdef PIN_LIN1_ENABLE
  .enable_pin = PIN_LIN1_ENABLE,
  .enable_level = LIN1_ENABLE_OUT
#else
  .enable_pin = 0u,
  .enable_level = 0u
#endif /* PIN_LIN1_ENABLE */
};
#ifndef CONFIG_FC7300_LIN1_MASTER
static struct fc7300_linslv_s g_fc7300lin1slave[LIN_MAX_NUM_PID];
#endif
static struct fc7300_lindev_s g_fc7300lin1;
#endif

#ifdef CONFIG_FC7300_LIN2
static const struct fc7300_linconfig_s g_fc7300lin2config =
{
  .irq_num = FC7300_IRQ_FCUART2,
  .irq_priority = NVIC_LIN2_PRIORITY_DEFAULT,
  .tx_pin = PIN_LIN2_TX,
  .rx_pin = PIN_LIN2_RX,
#ifdef PIN_LIN2_ENABLE
  .enable_pin = PIN_LIN2_ENABLE,
  .enable_level = LIN2_ENABLE_OUT
#else
  .enable_pin = 0u,
  .enable_level = 0u
#endif /* PIN_LIN2_ENABLE */
};
#ifndef CONFIG_FC7300_LIN2_MASTER
static struct fc7300_linslv_s g_fc7300lin2slave[LIN_MAX_NUM_PID];
#endif
static struct fc7300_lindev_s g_fc7300lin2;
#endif

#ifdef CONFIG_FC7300_LIN3
static const struct fc7300_linconfig_s g_fc7300lin3config =
{
  .irq_num = FC7300_IRQ_FCUART3,
  .irq_priority = NVIC_LIN3_PRIORITY_DEFAULT,
  .tx_pin = PIN_LIN3_TX,
  .rx_pin = PIN_LIN3_RX,
#ifdef PIN_LIN3_ENABLE
  .enable_pin = PIN_LIN3_ENABLE,
  .enable_level = LIN3_ENABLE_OUT
#else
  .enable_pin = 0u,
  .enable_level = 0u
#endif /* PIN_LIN3_ENABLE */
};
#ifndef CONFIG_FC7300_LIN3_MASTER
static struct fc7300_linslv_s g_fc7300lin3slave[LIN_MAX_NUM_PID];
#endif
static struct fc7300_lindev_s g_fc7300lin3;
#endif

#ifdef CONFIG_FC7300_LIN4
static const struct fc7300_linconfig_s g_fc7300lin4config =
{
  .irq_num = FC7300_IRQ_FCUART4,
  .irq_priority = NVIC_LIN4_PRIORITY_DEFAULT,
  .tx_pin = PIN_LIN4_TX,
  .rx_pin = PIN_LIN4_RX,
#ifdef PIN_LIN4_ENABLE
  .enable_pin = PIN_LIN4_ENABLE,
  .enable_level = LIN4_ENABLE_OUT
#else
  .enable_pin = 0u,
  .enable_level = 0u
#endif /* PIN_LIN4_ENABLE */
};
#ifndef CONFIG_FC7300_LIN4_MASTER
static struct fc7300_linslv_s g_fc7300lin4slave[LIN_MAX_NUM_PID];
#endif
static struct fc7300_lindev_s g_fc7300lin4;
#endif

#ifdef CONFIG_FC7300_LIN5
static const struct fc7300_linconfig_s g_fc7300lin5config =
{
  .irq_num = FC7300_IRQ_FCUART5,
  .irq_priority = NVIC_LIN5_PRIORITY_DEFAULT,
  .tx_pin = PIN_LIN5_TX,
  .rx_pin = PIN_LIN5_RX,
#ifdef PIN_LIN5_ENABLE
  .enable_pin = PIN_LIN5_ENABLE,
  .enable_level = LIN5_ENABLE_OUT
#else
  .enable_pin = 0u,
  .enable_level = 0u
#endif /* PIN_LIN5_ENABLE */
};
#ifndef CONFIG_FC7300_LIN5_MASTER
static struct fc7300_linslv_s g_fc7300lin5slave[LIN_MAX_NUM_PID];
#endif
static struct fc7300_lindev_s g_fc7300lin5;
#endif

#ifdef CONFIG_FC7300_LIN6
static const struct fc7300_linconfig_s g_fc7300lin6config =
{
  .irq_num = FC7300_IRQ_FCUART6,
  .irq_priority = NVIC_LIN6_PRIORITY_DEFAULT,
  .tx_pin = PIN_LIN6_TX,
  .rx_pin = PIN_LIN6_RX,
#ifdef PIN_LIN6_ENABLE
  .enable_pin = PIN_LIN6_ENABLE,
  .enable_level = LIN6_ENABLE_OUT
#else
  .enable_pin = 0u,
  .enable_level = 0u
#endif /* PIN_LIN6_ENABLE */
};
#ifndef CONFIG_FC7300_LIN6_MASTER
static struct fc7300_linslv_s g_fc7300lin6slave[LIN_MAX_NUM_PID];
#endif
static struct fc7300_lindev_s g_fc7300lin6;
#endif

#ifdef CONFIG_FC7300_LIN7
static const struct fc7300_linconfig_s g_fc7300lin7config =
{
  .irq_num = FC7300_IRQ_FCUART7,
  .irq_priority = NVIC_LIN7_PRIORITY_DEFAULT,
  .tx_pin = PIN_LIN7_TX,
  .rx_pin = PIN_LIN7_RX,
#ifdef PIN_LIN7_ENABLE
  .enable_pin = PIN_LIN7_ENABLE,
  .enable_level = LIN7_ENABLE_OUT
#else
  .enable_pin = 0u,
  .enable_level = 0u
#endif /* PIN_LIN7_ENABLE */
};
#ifndef CONFIG_FC7300_LIN7_MASTER
static struct fc7300_linslv_s g_fc7300lin7slave[LIN_MAX_NUM_PID];
#endif
static struct fc7300_lindev_s g_fc7300lin7;
#endif

#ifdef CONFIG_FC7300_LIN8
static const struct fc7300_linconfig_s g_fc7300lin8config =
{
  .irq_num = FC7300_IRQ_FCUART8,
  .irq_priority = NVIC_LIN8_PRIORITY_DEFAULT,
  .tx_pin = PIN_LIN8_TX,
  .rx_pin = PIN_LIN8_RX,
#ifdef PIN_LIN8_ENABLE
  .enable_pin = PIN_LIN8_ENABLE,
  .enable_level = LIN8_ENABLE_OUT
#else
  .enable_pin = 0u,
  .enable_level = 0u
#endif /* PIN_LIN8_ENABLE */
};
#ifndef CONFIG_FC7300_LIN8_MASTER
static struct fc7300_linslv_s g_fc7300lin8slave[LIN_MAX_NUM_PID];
#endif
static struct fc7300_lindev_s g_fc7300lin8;
#endif

#ifdef CONFIG_FC7300_LIN9
static const struct fc7300_linconfig_s g_fc7300lin9config =
{
  .irq_num = FC7300_IRQ_FCUART9,
  .irq_priority = NVIC_LIN9_PRIORITY_DEFAULT,
  .tx_pin = PIN_LIN9_TX,
  .rx_pin = PIN_LIN9_RX,
#ifdef PIN_LIN9_ENABLE
  .enable_pin = PIN_LIN9_ENABLE,
  .enable_level = LIN9_ENABLE_OUT
#else
  .enable_pin = 0u,
  .enable_level = 0u
#endif /* PIN_LIN9_ENABLE */
};
#ifndef CONFIG_FC7300_LIN9_MASTER
static struct fc7300_linslv_s g_fc7300lin9slave[LIN_MAX_NUM_PID];
#endif
static struct fc7300_lindev_s g_fc7300lin9;
#endif

#ifdef CONFIG_FC7300_LIN10
static const struct fc7300_linconfig_s g_fc7300lin10config =
{
  .irq_num = FC7300_IRQ_FCUART10,
  .irq_priority = NVIC_LIN10_PRIORITY_DEFAULT,
  .tx_pin = PIN_LIN10_TX,
  .rx_pin = PIN_LIN10_RX,
#ifdef PIN_LIN10_ENABLE
  .enable_pin = PIN_LIN10_ENABLE,
  .enable_level = LIN10_ENABLE_OUT
#else
  .enable_pin = 0u,
  .enable_level = 0u
#endif /* PIN_LIN10_ENABLE */
};
#ifndef CONFIG_FC7300_LIN10_MASTER
static struct fc7300_linslv_s g_fc7300lin10slave[LIN_MAX_NUM_PID];
#endif
static struct fc7300_lindev_s g_fc7300lin10;
#endif

#ifdef CONFIG_FC7300_LIN11
static const struct fc7300_linconfig_s g_fc7300lin11config =
{
  .irq_num = FC7300_IRQ_FCUART11,
  .irq_priority = NVIC_LIN11_PRIORITY_DEFAULT,
  .tx_pin = PIN_LIN11_TX,
  .rx_pin = PIN_LIN11_RX,
#ifdef PIN_LIN11_ENABLE
  .enable_pin = PIN_LIN11_ENABLE,
  .enable_level = LIN11_ENABLE_OUT
#else
  .enable_pin = 0u,
  .enable_level = 0u
#endif /* PIN_LIN11_ENABLE */
};
#ifndef CONFIG_FC7300_LIN11_MASTER
static struct fc7300_linslv_s g_fc7300lin11slave[LIN_MAX_NUM_PID];
#endif
static struct fc7300_lindev_s g_fc7300lin11;
#endif

#ifdef CONFIG_FC7300_LIN12
static const struct fc7300_linconfig_s g_fc7300lin12config =
{
  .irq_num = FC7300_IRQ_FCUART12,
  .irq_priority = NVIC_LIN12_PRIORITY_DEFAULT,
  .tx_pin = PIN_LIN12_TX,
  .rx_pin = PIN_LIN12_RX,
#ifdef PIN_LIN12_ENABLE
  .enable_pin = PIN_LIN12_ENABLE,
  .enable_level = LIN12_ENABLE_OUT
#else
  .enable_pin = 0u,
  .enable_level = 0u
#endif /* PIN_LIN12_ENABLE */
};
#ifndef CONFIG_FC7300_LIN12_MASTER
static struct fc7300_linslv_s g_fc7300lin12slave[LIN_MAX_NUM_PID];
#endif
static struct fc7300_lindev_s g_fc7300lin12;
#endif

#ifdef CONFIG_FC7300_LIN13
static const struct fc7300_linconfig_s g_fc7300lin13config =
{
  .irq_num = FC7300_IRQ_FCUART13,
  .irq_priority = NVIC_LIN13_PRIORITY_DEFAULT,
  .tx_pin = PIN_LIN13_TX,
  .rx_pin = PIN_LIN13_RX,
#ifdef PIN_LIN13_ENABLE
  .enable_pin = PIN_LIN13_ENABLE,
  .enable_level = LIN13_ENABLE_OUT
#else
  .enable_pin = 0u,
  .enable_level = 0u
#endif /* PIN_LIN13_ENABLE */
};
#ifndef CONFIG_FC7300_LIN13_MASTER
static struct fc7300_linslv_s g_fc7300lin13slave[LIN_MAX_NUM_PID];
#endif
static struct fc7300_lindev_s g_fc7300lin13;
#endif

#ifdef CONFIG_FC7300_LIN14
static const struct fc7300_linconfig_s g_fc7300lin14config =
{
  .irq_num = FC7300_IRQ_FCUART14,
  .irq_priority = NVIC_LIN14_PRIORITY_DEFAULT,
  .tx_pin = PIN_LIN14_TX,
  .rx_pin = PIN_LIN14_RX,
#ifdef PIN_LIN14_ENABLE
  .enable_pin = PIN_LIN14_ENABLE,
  .enable_level = LIN14_ENABLE_OUT
#else
  .enable_pin = 0u,
  .enable_level = 0u
#endif /* PIN_LIN14_ENABLE */
};
#ifndef CONFIG_FC7300_LIN14_MASTER
static struct fc7300_linslv_s g_fc7300lin14slave[LIN_MAX_NUM_PID];
#endif
static struct fc7300_lindev_s g_fc7300lin14;
#endif

#ifdef CONFIG_FC7300_LIN15
static const struct fc7300_linconfig_s g_fc7300lin15config =
{
  .irq_num = FC7300_IRQ_FCUART15,
  .irq_priority = NVIC_LIN15_PRIORITY_DEFAULT,
  .tx_pin = PIN_LIN15_TX,
  .rx_pin = PIN_LIN15_RX,
#ifdef PIN_LIN15_ENABLE
  .enable_pin = PIN_LIN15_ENABLE,
  .enable_level = LIN15_ENABLE_OUT
#else
  .enable_pin = 0u,
  .enable_level = 0u
#endif /* PIN_LIN15_ENABLE */
};
#ifndef CONFIG_FC7300_LIN15_MASTER
static struct fc7300_linslv_s g_fc7300lin15slave[LIN_MAX_NUM_PID];
#endif
static struct fc7300_lindev_s g_fc7300lin15;
#endif

#ifdef CONFIG_FC7300_LIN16
static const struct fc7300_linconfig_s g_fc7300lin16config =
{
  .irq_num = FC7300_IRQ_FCUART16,
  .irq_priority = NVIC_LIN16_PRIORITY_DEFAULT,
  .tx_pin = PIN_LIN16_TX,
  .rx_pin = PIN_LIN16_RX,
#ifdef PIN_LIN16_ENABLE
  .enable_pin = PIN_LIN16_ENABLE,
  .enable_level = LIN16_ENABLE_OUT
#else
  .enable_pin = 0u,
  .enable_level = 0u
#endif /* PIN_LIN16_ENABLE */
};
#ifndef CONFIG_FC7300_LIN16_MASTER
static struct fc7300_linslv_s g_fc7300lin16slave[LIN_MAX_NUM_PID];
#endif
static struct fc7300_lindev_s g_fc7300lin16;
#endif

#ifdef CONFIG_FC7300_LIN17
static const struct fc7300_linconfig_s g_fc7300lin17config =
{
  .irq_num = FC7300_IRQ_FCUART17,
  .irq_priority = NVIC_LIN17_PRIORITY_DEFAULT,
  .tx_pin = PIN_LIN17_TX,
  .rx_pin = PIN_LIN17_RX,
#ifdef PIN_LIN17_ENABLE
  .enable_pin = PIN_LIN17_ENABLE,
  .enable_level = LIN17_ENABLE_OUT
#else
  .enable_pin = 0u,
  .enable_level = 0u
#endif /* PIN_LIN17_ENABLE */
};
#ifndef CONFIG_FC7300_LIN17_MASTER
static struct fc7300_linslv_s g_fc7300lin17slave[LIN_MAX_NUM_PID];
#endif
static struct fc7300_lindev_s g_fc7300lin17;
#endif

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Function: fc7300_lin_ifup
 *
 * Description:
 *   NuttX Callback: Bring up the LIN network interface
 *
 * Input Parameters:
 *   dev  - Reference to the NuttX driver state structure
 *
 * Returned Value:
 *   OK(0) on success; Negated errno on failure.
 *
 * Assumptions:
 *
 ****************************************************************************/

static int fc7300_lin_ifup(struct netdev_lowerhalf_s *dev)
{
  int ret;
  irqstate_t curr_state;
  struct fc7300_lindev_s *priv = (struct fc7300_lindev_s *)dev;

  /* If the lin controller is busy now, return error */

  curr_state = spin_lock_irqsave(&priv->lock);
  if (READ_REG_U32(priv->base + FC7300_FCUART_CTRL_OFFSET) &
      (FCUART_CTRL_RE | FCUART_CTRL_TE))
    {
      spin_unlock_irqrestore(&priv->lock, curr_state);
      nerr("%d:ERROR: The lin controller is owned by another processor\n",
            __LINE__);
      return -EBUSY;
    }

  /* Reset controller and initialize the controller */

  fc7300_lin_softreset(priv);
  ret = fc7300_lin_controller_init(priv);
  if (ret)
    {
      spin_unlock_irqrestore(&priv->lock, curr_state);
      nerr("%d:ERROR: controller init ret = %d", __LINE__, ret);
      return ret;
    }

  /* Attach interrupt hndler */

  (void)fc7300_lin_irqattach(priv);

  /* Initialize the driver state */

  priv->frame_state = FC7300_LIN_FRAME_IDLE;
  priv->pkt = NULL;
  LIN_FRAME_RX_INVALIDATE(priv);

  /* If slave node, reset the slave information */

  if (!priv->bmaster)
    {
      memset(priv->slv, 0, sizeof(struct fc7300_linslv_s) * LIN_MAX_NUM_PID);
      priv->slv[0x3cu].len = LIN_MAX_DATA_LENGTH;
      priv->slv[0x3cu].attr = FC7300_LIN_RESP_RX;
      priv->slv[0x3du].len = LIN_MAX_DATA_LENGTH;
    }

  FC7300_LIN_SETIFUP(priv);

  spin_unlock_irqrestore(&priv->lock, curr_state);

  fc7300_lin_startidlesleeptimer(priv);

  ninfo("%d:INFO: linif up\n", __LINE__);
  return ret;
}

/****************************************************************************
 * Function: fc7300_lin_ifdown
 *
 * Description:
 *   NuttX Callback: Stop the LIN network interface
 *
 * Input Parameters:
 *   dev  - Reference to the NuttX driver state structure
 *
 * Returned Value:
 *   OK(0) on success; Negated errno on failure.
 *
 * Assumptions:
 *
 ****************************************************************************/

static int fc7300_lin_ifdown(struct netdev_lowerhalf_s *dev)
{
  irqstate_t curr_state;
  struct fc7300_lindev_s *priv = (struct fc7300_lindev_s *)dev;

  curr_state = spin_lock_irqsave(&priv->lock);

  /* If the lin interface is down now, return OK */

  if (!FC7300_LIN_ISUP(priv))
    {
      spin_unlock_irqrestore(&priv->lock, curr_state);
      ninfo("%d:INFO: lin if is alredy down", __LINE__);
      return OK;
    }

  /* If the lin interface is busy now, wait the current business complete */

  if (FC7300_LIN_ISBUSY(priv))
    {
      spin_unlock_irqrestore(&priv->lock, curr_state);
      nerr("%d:ERROR: linif is busy, try later\n", __LINE__);
      return -EBUSY;
    }

  FC7300_LIN_SETIFDOWN(priv);
  (void)fc7300_lin_irqdetach(priv);

  /* Disable receiver and transmitter */

  CLEAR_REG_BITS(priv->base + FC7300_FCUART_CTRL_OFFSET,
                 FCUART_CTRL_RE | FCUART_CTRL_TE);

  fc7300_lin_softreset(priv);

  spin_unlock_irqrestore(&priv->lock, curr_state);

  ninfo("%d:INFO: linif down", __LINE__);
  return OK;
}

/****************************************************************************
 * Function: fc7300_lin_transmit
 *
 * Description:
 *   NuttX Callback: Start hardware transmission
 *
 * Input Parameters:
 *   dev  - Reference to the NuttX driver state structure
 *   pkt  - the packet to be transmitted
 *
 * Returned Value:
 *   OK(0) on success; Negated errno on failure.
 *
 * Assumptions:
 *
 ****************************************************************************/

static int fc7300_lin_transmit(struct netdev_lowerhalf_s *dev,
                               netpkt_t *pkt)
{
  struct fc7300_lindev_s *priv = (struct fc7300_lindev_s *)dev;
  uint32_t pkt_len;
  irqstate_t curr_state;
  int ret;

  /* Check the frame length */

  pkt_len = netpkt_getdatalen(dev, pkt);
  if (pkt_len != sizeof(struct can_frame))
    {
      nwarn("%d: ERROR: frame size error %lu\n", __LINE__, pkt_len);
      return -EINVAL;
    }

  /* Check the LIN controller state */

  curr_state = enter_critical_section();
  if (!FC7300_LIN_ISNORMAL(priv))
    {
      leave_critical_section(curr_state);
      nwarn("%d:ERROR: %lx is not in operational state %u\n",
                          __LINE__, priv->base, priv->state);
      return -EIO;
    }

  FC7300_LIN_SETBUSY(priv);
  priv->frame_state = FC7300_LIN_FRAME_PEOCESSING;
  leave_critical_section(curr_state);

  /* Parse the lin frame */

  ret = fc7300_lin_parseframe(priv,
              (struct can_frame *)netpkt_getdata(dev, pkt));
  if (ret == OK)
    {
      netpkt_free(dev, pkt, NETPKT_TX);
    }

  if (ret > 0u)
    {
      /* Send the header if driver is configured as master node */

      priv->pkt = pkt;
      ret = fc7300_lin_sendheader(priv);
      if (ret == OK)
        {
          return OK;
        }
    }

  /* If the transmission is on going, the net packet and the driver state
   * are releaseed when the transmission is finished. If the return value
   * is OK, the control frame is saved, release the net packet and the
   * driver state, otherwise an error occured, only release the driver
   * state.
   */

  if (ret <= 0)
    {
      curr_state = enter_critical_section();
      fc7300_lin_setidle(priv);
      leave_critical_section(curr_state);

      fc7300_lin_startidlesleeptimer(priv);
    }

  return ret;
}

/****************************************************************************
 * Function: fc7300_lin_receive
 *
 * Description:
 *   NuttX Callback: Receive the LIN message
 *
 * Input Parameters:
 *   dev  - Reference to the NuttX driver state structure
 *
 * Returned Value:
 *   netpkt_t - Reference to the received packet
 *
 * Assumptions:
 *
 ****************************************************************************/

static netpkt_t *fc7300_lin_receive(struct netdev_lowerhalf_s *dev)
{
  struct fc7300_lindev_s *priv = (struct fc7300_lindev_s *)dev;
  netpkt_t *pkt = NULL;
  irqstate_t curr_state;
  struct can_frame *frame;
  uint8_t checksum;
  uint8_t dlen;

  /* Check the frame available for receive */

  if (!LIN_FRAME_RX_AVAILABLE(priv))
    {
      return NULL;
    }

  /* Allocate net packet  */

  pkt = netpkt_alloc(dev, NETPKT_RX);
  if (pkt == NULL)
    {
      nwarn("packet alloc error\n");
      return NULL;
    }

  /* Copy the frame info to pkt */

  frame = (struct can_frame *)netpkt_getdata(dev, pkt);

  /* Check the frame has error or not */

  if (FC7300_LIN_BUF_EVT(priv) != 0u)
    {
      fc7300_lin_buildeventframe(priv, frame);
    }
  else
    {
      /* Check the data checksum */

      dlen = FC7300_LIN_BUF_DLC(priv) - 1u;
      checksum = fc7300_lin_datachecksum(
          FC7300_LIN_BUF_FLAG(priv) & FC7300_LIN_EXT_CKSUM,
          FC7300_LIN_BUF_PID(priv), FC7300_LIN_BUF_PDATA(priv, 0u), dlen);

      if (checksum == FC7300_LIN_BUF_CKSUM(priv, dlen))
        {
          /* Checksum success, copy the frame to pkt */

          fc7300_lin_buildframe(priv, frame);
        }
      else
        {
          /* Checksum error, build checksum error frame */

          FC7300_LIN_BUF_EVT(priv) = FC7300_LIN_ERR_RXCKSUM;
          fc7300_lin_buildeventframe(priv, frame);
        }
    }

  netpkt_setdatalen(dev, pkt, sizeof(struct can_frame));

  /* Release the driver state to idle, and invalidate the RX frame */

  curr_state = enter_critical_section();
  LIN_FRAME_RX_INVALIDATE(priv);
  if (!FC7300_LIN_ISSLEEP(priv))
    {
      fc7300_lin_setidle(priv);
      leave_critical_section(curr_state);

      fc7300_lin_startidlesleeptimer(priv);
    }
  else
    {
      leave_critical_section(curr_state);
    }

  return pkt;
}

#ifdef CONFIG_NETDEV_IOCTL
/****************************************************************************
 * Function: fc7300_lin_ioctl
 *
 * Description:
 *   NuttX Callback: LIN ioctl command handler
 *
 * Input Parameters:
 *   dev  - Reference to the NuttX driver state structure
 *   cmd  - ioctl command
 *   arg  - Argument accompanying the command
 *
 * Returned Value:
 *   OK(0) on success; Negated errno on failure.
 *
 * Assumptions:
 *
 ****************************************************************************/

static int fc7300_lin_ioctl(struct netdev_lowerhalf_s *dev, int cmd,
                            unsigned long arg)
{
  struct fc7300_lindev_s *priv = (struct fc7300_lindev_s *)dev;
  int ret = -ENOTTY;
  struct can_ioctl_state_s *ioctl_state = (struct can_ioctl_state_s *)arg;

  switch (cmd)
    {
#ifdef CONFIG_NETDEV_CAN_STATE_IOCTL
      case SIOCGCANSTATE:
        ioctl_state->state = (enum can_ioctl_state_e)priv->state;
        ret = OK;
        break;
      case SIOCSCANSTATE:
        if (ioctl_state->state == CAN_STATE_SLEEP)
          {
            ret = fc7300_lin_gotosleep(priv, (bool)ioctl_state->priv);
          }
        else if (ioctl_state->state == CAN_STATE_OPERATIONAL)
          {
            ret = fc7300_lin_ioctlwakeup(priv, (bool)ioctl_state->priv);
          }
        else
          {
            ret = -ENOTSUP;
          }
        break;
#endif /* CONFIG_NETDEV_CAN_STATE_IOCTL */
      default:
        (void)priv;
        (void)ioctl_state;
        break;
    }

  return ret;
}
#endif /* CONFIG_NETDEV_IOCTL */

#ifdef CONFIG_NETDEV_CAN_STATE_IOCTL
/****************************************************************************
 * Function: fc7300_lin_sendwakeupsignal
 *
 * Description:
 *   Send wake up signal to LIN bus
 *
 * Input Parameters:
 *   priv - Reference to the private data of fc7300 lin driver
 *
 * Returned Value:
 *   NONE.
 *
 * Assumptions:
 *
 ****************************************************************************/

static void fc7300_lin_sendwakeupsignal(struct fc7300_lindev_s *priv)
{
  priv->frame_state = FC7300_LIN_FRAME_WAKEUP;

  /* Flush the transmit and receive FIFO */

  SET_REG_BITS(priv->base + FC7300_FCUART_FIFO_OFFSET,
               FCUART_FIFO_RXFLUSH | FCUART_FIFO_TXFLUSH);

  /* Generate wakeup signal */

  if (priv->baudrate > 10000u)
    {
      WRITE_REG_U32(priv->base + FC7300_FCUART_DATA_OFFSET, 0x80u);
    }
  else
    {
      WRITE_REG_U32(priv->base + FC7300_FCUART_DATA_OFFSET, 0xf8u);
    }
}

/****************************************************************************
 * Function: fc7300_lin_ioctlwakeup
 *
 * Description:
 *   Send wakeup signal to LIN bus and go to operational mode
 *
 * Input Parameters:
 *   priv - Reference to the private data of fc7300 lin driver
 *   wakeupinternal - Wakeup internal, and do not send wake up signal to
 *   the LIN bus.
 *
 * Returned Value:
 *   OK(0) on success; Negated errno on failure.
 *
 * Assumptions:
 *
 ****************************************************************************/

static int fc7300_lin_ioctlwakeup(struct fc7300_lindev_s *priv,
                                  bool wakeupinternal)
{
  irqstate_t curr_state;
  int ret = OK;

  curr_state = enter_critical_section();
  if (FC7300_LIN_ISNORMAL(priv))
    {
      /* Do nothing if LIN is already in operational mode */

      leave_critical_section(curr_state);
    }
  else if (!FC7300_LIN_ISSLEEP(priv))
    {
      /* Return -EBUSY if LIN is in busy or sleeppending */

      ret = -EBUSY;
      leave_critical_section(curr_state);
    }
  else
    {
      if (wakeupinternal)
        {
          FC7300_LIN_SETNORMAL(priv);
#ifdef CONFIG_FC7300_LIN_REPORT_STATE_EVENT
          leave_critical_section(curr_state);
          fc7300_lin_reportevent(priv, FC7300_LIN_EV_WKUP);
#else
          fc7300_lin_setidle(priv);
          leave_critical_section(curr_state);
          fc7300_lin_startidlesleeptimer(priv);
#endif
        }
      else
        {
          /* Send wakeup signal to the bus */

          fc7300_lin_sendwakeupsignal(priv);
          leave_critical_section(curr_state);
        }
    }

  return ret;
}

/****************************************************************************
 * Function: fc7300_lin_sendsleepsignal
 *
 * Description:
 *   Send sleep signal to LIN bus
 *
 * Input Parameters:
 *   priv - Reference to the private data of fc7300 lin driver
 *
 * Returned Value:
 *   OK(0) on success; Negated errno on failure.
 *
 * Assumptions:
 *
 ****************************************************************************/

static int fc7300_lin_sendsleepsignal(struct fc7300_lindev_s *priv)
{
  /* Send pid 0x3c, data 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF */

  FC7300_LIN_BUF_FLAG(priv) = 0u;
  FC7300_LIN_BUF_DLC(priv) = 9u;
  FC7300_LIN_BUF_IND(priv) = 0u;
  FC7300_LIN_BUF_EVT(priv) = 0u;
  FC7300_LIN_BUF_SYNC(priv) = LIN_FRAME_SYNC_BYTE;
  FC7300_LIN_BUF_PID(priv) = LIN_MASTER_DIAG_REQ_PID;
  FC7300_LIN_BUF_DATA(priv, 0u) = 0x00u;
  FC7300_LIN_BUF_DATA(priv, 1u) = 0xffu;
  FC7300_LIN_BUF_DATA(priv, 2u) = 0xffu;
  FC7300_LIN_BUF_DATA(priv, 3u) = 0xffu;
  FC7300_LIN_BUF_DATA(priv, 4u) = 0xffu;
  FC7300_LIN_BUF_DATA(priv, 5u) = 0xffu;
  FC7300_LIN_BUF_DATA(priv, 6u) = 0xffu;
  FC7300_LIN_BUF_DATA(priv, 7u) = 0xffu;
  FC7300_LIN_BUF_CKSUM(priv, 8u) =  fc7300_lin_datachecksum(false,
    LIN_MASTER_DIAG_REQ_PID, FC7300_LIN_BUF_PDATA(priv, 0u), 8u);

  return fc7300_lin_sendheader(priv);
}

/****************************************************************************
 * Function: fc7300_lin_gotosleep
 *
 * Description:
 *   Send sleep signal to LIN bus and go to sleep mode
 *
 * Input Parameters:
 *   priv - Reference to the private data of fc7300 lin driver
 *   sleepinternal - Sleep internal and do not send sleep command to the bus
 *
 * Returned Value:
 *   OK(0) on success; Negated errno on failure.
 *
 * Assumptions:
 *
 ****************************************************************************/

static int fc7300_lin_gotosleep(struct fc7300_lindev_s *priv,
                                bool sleepinternal)
{
  irqstate_t curr_state;
  int ret = OK;

  curr_state = enter_critical_section();
  if (FC7300_LIN_ISSLEEP(priv))
    {
      /* Return OK if LIN is already in sleep mode */

      leave_critical_section(curr_state);
    }
  else if (!FC7300_LIN_ISNORMAL(priv))
    {
      /* Return -EBUSY if LIN is in busy or sleeppending */

      ret = -EBUSY;
      leave_critical_section(curr_state);
    }
  else
    {
      /* LIN is Idle now, changing state is available */

      if (sleepinternal || (!priv->bmaster))
        {
          /* Set sleep mode internal and report sleep event */

          fc7300_lin_gotosleepinternal(priv);
          FC7300_LIN_SETSLEEP(priv);
          leave_critical_section(curr_state);

#ifdef CONFIG_FC7300_LIN_REPORT_STATE_EVENT
          fc7300_lin_reportevent(priv, FC7300_LIN_EV_SLEEP);
#endif
        }
      else
        {
          /* Master node switch to sleep pending state and send sleep
           * signal to the bus. The driver is switched to sleep mode
           * when the sleep signal is completed.
           */

          FC7300_LIN_SETSPENDING(priv);
          priv->frame_state = FC7300_LIN_FRAME_PEOCESSING;
          leave_critical_section(curr_state);
          ret = fc7300_lin_sendsleepsignal(priv);
        }
    }

  return ret;
}
#endif /* CONFIG_NETDEV_CAN_STATE_IOCTL */

/****************************************************************************
 * Function: fc7300_lin_gotosleepinternal
 *
 * Description:
 *   Set the LIN driver to sleep mode
 *
 * Input Parameters:
 *   priv - Reference to the private data of fc7300 lin driver
 *
 * Returned Value:
 *   NONE.
 *
 * Assumptions:
 *
 ****************************************************************************/

static void fc7300_lin_gotosleepinternal(struct fc7300_lindev_s *priv)
{
  uint32_t val;

  /* Disable receive, idle and frame error interrupts */

  CLEAR_REG_BITS(priv->base + FC7300_FCUART_CTRL_OFFSET, FCUART_CTRL_IIE |
                 FCUART_CTRL_RIE | FCUART_CTRL_FEIE | FCUART_CTRL_ORIE |
                 FCUART_CTRL_NEIE);

  /* Flush the transmit and receive FIFO */

  SET_REG_BITS(priv->base + FC7300_FCUART_FIFO_OFFSET,
               FCUART_FIFO_RXFLUSH | FCUART_FIFO_TXFLUSH);

  /* Clear status and disable break detect */

  val = READ_REG_U32(priv->base + FC7300_FCUART_STAT_OFFSET) &
        (~(FCUART_STAT_RXINV | FCUART_STAT_LBKDE));
  WRITE_REG_U32(priv->base + FC7300_FCUART_STAT_OFFSET, val);

  /* Disable break detect interrupt and enable rx input active edge
   * interrupt
   */

  val = READ_REG_U32(priv->base + FC7300_FCUART_BAUD_OFFSET);
  val = (val & (~(FCUART_BAUD_LBKDIE))) | FCUART_BAUD_RIAEIE;
  WRITE_REG_U32(priv->base + FC7300_FCUART_BAUD_OFFSET, val);
}

/****************************************************************************
 * Function: fc7300_lin_checksleepsignal
 *
 * Description:
 *   Check if the received frame is a sleep signal, switch to sleep mode
 *   and report state changed event to upper layer
 *
 * Input Parameters:
 *   priv  - Reference to the private data of fc7300 lin driver
 *   event - The sleep event to report to upper layer
 *
 * Returned Value:
 *   true:  The received frame is a sleep signal
 *   false: The received frame is a normal frame
 *
 * Assumptions:
 *
 ****************************************************************************/

static bool fc7300_lin_checksleepsignal(struct fc7300_lindev_s *priv,
                                        uint8_t event)
{
  irqstate_t curr_state;
  uint8_t seq[10u] =
    {
      0x3cu, 0x00u, 0xffu, 0xffu, 0xffu, 0xffu, 0xffu, 0xffu, 0xffu, 0x00u
    };

  /* Current state is sleep pending, the sleep signal is send to
   * LIN bus successfully, set driver to sleep mode
   */

  if (!memcmp(FC7300_LIN_BUF_PPID(priv), seq, 10u))
    {
      curr_state = enter_critical_section();
      fc7300_lin_gotosleepinternal(priv);
      priv->frame_state = FC7300_LIN_FRAME_IDLE;
      FC7300_LIN_SETSLEEP(priv);
      leave_critical_section(curr_state);

#ifdef CONFIG_FC7300_LIN_REPORT_STATE_EVENT
      fc7300_lin_reportevent(priv, event);
#else
      fc7300_lin_canceltimer(priv);
#endif
      return true;
    }

  return false;
}

/****************************************************************************
 * Function: fc7300_lin_checkwakeupsignal
 *
 * Description:
 *   Check whether the received data is wake up signal
 *
 * Input Parameters:
 *   buf   - The received data buffer
 *   count - The received data count
 *
 * Returned Value:
 *   OK(0) on success; Negated errno on failure.
 *
 * Assumptions:
 *
 ****************************************************************************/

static bool fc7300_lin_checkwakeupsignal(struct fc7300_lindev_s *priv)
{
  uint8_t buf[FC7300_LIN_FIFO_DEPTH];
  uint8_t count;

  count = fc7300_lin_readrxfifo(priv, buf);

  if ((count == 1u) && ((buf[0u] == 0xfcu) || (buf[0u] == 0xf8u) ||
      (buf[0u] == 0xf0u) || (buf[0u] == 0xe0u) || (buf[0u] == 0xc0u) ||
      (buf[0u] == 0x80u) || (buf[0u] == 0x00u)))
    {
      return true;
    }

  return false;
}

/****************************************************************************
 * Function: fc7300_lin_frametimeout
 *
 * Description:
 *   The function to handle LIN frame timeout events
 *
 * Input Parameters:
 *   arg - Parameter passed to the frame timer
 *
 * Returned Value:
 *   None.
 *
 * Assumptions:
 *
 ****************************************************************************/

static void fc7300_lin_frametimeout(wdparm_t arg)
{
  irqstate_t curr_state;
  struct fc7300_lindev_s *priv = (struct fc7300_lindev_s *)arg;
  uint8_t n;

  if (FC7300_LIN_ISBUSY(priv))
    {
      switch (priv->frame_state)
        {
          case FC7300_LIN_FRAME_SEND_BREAK:
            fc7300_lin_reportevent(priv, FC7300_LIN_ERR_BREAK_TMO);
            break;
          case FC7300_LIN_FRAME_RECV_HEADER:
            fc7300_lin_reportevent(priv, priv->bmaster ?
              FC7300_LIN_ERR_TXSYNC_TMO : FC7300_LIN_ERR_RXSYNC_TMO);
            break;
          case FC7300_LIN_FRAME_RECV_PID:
            fc7300_lin_reportevent(priv, priv->bmaster ?
              FC7300_LIN_ERR_TXPID_TMO : FC7300_LIN_ERR_RXPID_TMO);
            break;
          case FC7300_LIN_FRAME_SEND_DATA:
            n = READ_REG_SHIFT(priv->base + FC7300_FCUART_WATERMARK_OFFSET,
                               FCUART_WATERMARK_RXCOUNT_MASK,
                               FCUART_WATERMARK_RXCOUNT_SHIFT);
            if (n == (FC7300_LIN_BUF_DLC(priv) - 1u))
              {
                fc7300_lin_reportevent(priv, FC7300_LIN_ERR_TXCKSUM_TMO);
              }
            else
              {
                fc7300_lin_reportevent(priv, FC7300_LIN_ERR_TXD_TMO);
              }
            break;
          case FC7300_LIN_FRAME_RECV_DATA:
            n = READ_REG_SHIFT(priv->base + FC7300_FCUART_WATERMARK_OFFSET,
                               FCUART_WATERMARK_RXCOUNT_MASK,
                               FCUART_WATERMARK_RXCOUNT_SHIFT);
            if (n == 0u)
              {
                fc7300_lin_reportevent(priv, FC7300_LIN_ERR_RXNORESP);
              }
            else if (n == (FC7300_LIN_BUF_DLC(priv) - 1u))
              {
                fc7300_lin_reportevent(priv, FC7300_LIN_ERR_RXCKSUM_TMO);
              }
            else
              {
                fc7300_lin_reportevent(priv, FC7300_LIN_ERR_RXRESPONSE);
              }
            break;
          default:
            curr_state = enter_critical_section();
            fc7300_lin_setidle(priv);
            leave_critical_section(curr_state);
            break;
        }
    }
  else if (FC7300_LIN_ISSPENDING(priv))
    {
      /* Send sleep signal failed, reset to operational state */

      curr_state = enter_critical_section();
      fc7300_lin_setidle(priv);
      leave_critical_section(curr_state);
    }
  else
    {
      /* Do nothing */
    }
}

#ifdef CONFIG_FC7300_LIN_IDLE_TO_SLEEP
/****************************************************************************
 * Function: fc7300_lin_busidlesleep
 *
 * Description:
 *   LIN delayed work to go to sleep mode due to bus idle
 *
 * Input Parameters:
 *   arg - Parameter passed to the delayed work
 *
 * Returned Value:
 *   None.
 *
 * Assumptions:
 *
 ****************************************************************************/

static void fc7300_lin_busidlesleep(wdparm_t arg)
{
  irqstate_t curr_state;
  struct fc7300_lindev_s *priv = (struct fc7300_lindev_s *)arg;

  if (FC7300_LIN_ISNORMAL(priv))
    {
      /* Bus idle 4 - 10 seconds reached, go to sleep */

      curr_state = enter_critical_section();
      fc7300_lin_gotosleepinternal(priv);
      FC7300_LIN_SETSLEEP(priv);
      leave_critical_section(curr_state);

#ifdef CONFIG_FC7300_LIN_REPORT_STATE_EVENT
      fc7300_lin_reportevent(priv, FC7300_LIN_EV_SLEEP_IDLE);
#endif
    }
}
#endif

/****************************************************************************
 * Function: fc7300_lin_startframetimer
 *
 * Description:
 *   Start a timer to process frame timeout events when frame transmission
 * is time out.
 *
 * Input Parameters:
 *   priv - Reference to the private data of fc7300 lin driver
 *   timeout - Frame transmission timeout value
 *
 * Returned Value:
 *   OK(0) on success; Negated errno on failure.
 *
 * Assumptions:
 *
 ****************************************************************************/

static int fc7300_lin_startframetimer(struct fc7300_lindev_s *priv,
                                      uint32_t timeout)
{
  return wd_start(&priv->timer, timeout, fc7300_lin_frametimeout,
                  (wdparm_t)priv);
}

/****************************************************************************
 * Function: fc7300_lin_startidlesleeptimer
 *
 * Description:
 *   Start a timer, when the timer expired, the driver go to sleep mode.
 *
 * Input Parameters:
 *   priv - Reference to the private data of fc7300 lin driver
 *
 * Returned Value:
 *   OK(0) on success; Negated errno on failure.
 *
 * Assumptions:
 *
 ****************************************************************************/

static int fc7300_lin_startidlesleeptimer(struct fc7300_lindev_s *priv)
{
#ifdef CONFIG_FC7300_LIN_IDLE_TO_SLEEP
  return wd_start(&priv->timer, SEC2TICK(CONFIG_FC7300_LIN_IDLE_SLEEP_TIME),
                  fc7300_lin_busidlesleep, (wdparm_t)priv);
#endif
  return OK;
}

/****************************************************************************
 * Function: fc7300_lin_canceltimer
 *
 * Description:
 *   Cancel the started timer.
 *
 * Input Parameters:
 *   priv - Reference to the private data of fc7300 lin driver
 *
 * Returned Value:
 *   None.
 *
 * Assumptions:
 *
 ****************************************************************************/

static void fc7300_lin_canceltimer(struct fc7300_lindev_s *priv)
{
  wd_cancel(&priv->timer);
}

/****************************************************************************
 * Function: fc7300_lin_errorhandler
 *
 * Description:
 *   Error interrupt handler
 *
 * Input Parameters:
 *   priv - Reference to the private data of fc7300 lin driver
 *
 * Returned Value:
 *   OK(0) on success; Negated errno on failure.
 *
 * Assumptions:
 *
 ****************************************************************************/

static int fc7300_lin_errorhandler(struct fc7300_lindev_s *priv,
                                   uint32_t status)
{
  uint32_t regval = READ_REG_U32(priv->base + FC7300_FCUART_CTRL_OFFSET);

  /* The controller errors (frame error, noise error, rx overflow error) are
   * only handled when a frame transmission is in progress (busy or sleep
   * pending). These errors are disabled when the driver switched to other
   * states.
   */

  if ((!FC7300_LIN_ISBUSY(priv)) && (!(FC7300_LIN_ISSPENDING(priv))))
    {
      return OK;
    }

  if ((status & FCUART_STAT_FEF) && (regval & FCUART_CTRL_FEIE))
    {
      switch (priv->frame_state)
        {
          case FC7300_LIN_FRAME_SEND_BREAK:
            fc7300_lin_reportevent(priv, FC7300_LIN_ERR_BREAK_TMO);
            break;
          case FC7300_LIN_FRAME_RECV_HEADER:
            fc7300_lin_reportevent(priv, priv->bmaster ?
              FC7300_LIN_ERR_TXSYNC_TMO : FC7300_LIN_ERR_RXSYNC_TMO);
            break;
          case FC7300_LIN_FRAME_RECV_PID:
            fc7300_lin_reportevent(priv, priv->bmaster ?
              FC7300_LIN_ERR_TXPID_TMO : FC7300_LIN_ERR_RXPID_TMO);
            break;
          case FC7300_LIN_FRAME_SEND_DATA:
            fc7300_lin_reportevent(priv, FC7300_LIN_ERR_TXDATA);
            break;
          case FC7300_LIN_FRAME_RECV_DATA:
            fc7300_lin_reportevent(priv, FC7300_LIN_ERR_RXRESPONSE);
            break;
          default:
            break;
        }
    }
  else if ((status & FCUART_STAT_RORF) && (regval & FCUART_CTRL_ORIE))
    {
      fc7300_lin_reportevent(priv, FC7300_LIN_ERR_CTRL_RXOVERFLOW);
    }
  else if ((status & FCUART_STAT_NF) && (regval & FCUART_CTRL_NEIE))
    {
      fc7300_lin_reportevent(priv, FC7300_LIN_ERR_CTRL_NOISEERROR);
    }
  else
    {
      /* Other error interrupts will reported as unknown */

      fc7300_lin_reportevent(priv, FC7300_LIN_ERR_UNKNOWN);
    }

  return OK;
}

/****************************************************************************
 * Function: fc7300_lin_rxdactivehandler
 *
 * Description:
 *   RXD pin active interrupt handler
 *
 * Input Parameters:
 *   priv - Reference to the private data of fc7300 lin driver
 *
 * Returned Value:
 *   OK(0) on success; Negated errno on failure.
 *
 * Assumptions:
 *
 ****************************************************************************/

static int fc7300_lin_rxdactivehandler(struct fc7300_lindev_s *priv)
{
  irqstate_t curr_state;

  curr_state = enter_critical_section();
  if (!FC7300_LIN_ISSLEEP(priv))
    {
      /* Only disable RX input active edge interrupt if the driver is not
       * in sleep mode
       */

      CLEAR_REG_BITS(priv->base + FC7300_FCUART_BAUD_OFFSET,
                     FCUART_BAUD_RIAEIE);
      leave_critical_section(curr_state);
      return OK;
    }

  /* Enable idle line detect to check wake up signal */

  fc7300_lin_setrxwatermark(priv, FC7300_LIN_FIFO_DEPTH - 1u);
  fc7300_lin_idlelinedetect(priv);
  leave_critical_section(curr_state);
  return OK;
}

/****************************************************************************
 * Function: fc7300_lin_idlelinehandler
 *
 * Description:
 *   LIN idle line interrupt handler
 *
 * Input Parameters:
 *   priv - Reference to the private data of fc7300 lin driver
 *
 * Returned Value:
 *   OK(0) on success; Negated errno on failure.
 *
 * Assumptions:
 *
 ****************************************************************************/

static int fc7300_lin_idlelinehandler(struct fc7300_lindev_s *priv)
{
  irqstate_t curr_state;

  /* Disable idle interrupt  */

  CLEAR_REG_BITS(priv->base + FC7300_FCUART_CTRL_OFFSET, FCUART_CTRL_IIE);

  curr_state = enter_critical_section();
  if (!FC7300_LIN_ISSLEEP(priv))
    {
      /* Do nothing if the driver is not in sleep mode */

      leave_critical_section(curr_state);
      return OK;
    }

  if (fc7300_lin_checkwakeupsignal(priv))
    {
      if (priv->frame_state == FC7300_LIN_FRAME_WAKEUP)
        {
          /* Wakeup signal is sent by ourself, switch to operational */

          FC7300_LIN_SETNORMAL(priv);
#ifdef CONFIG_FC7300_LIN_REPORT_STATE_EVENT
          leave_critical_section(curr_state);
          fc7300_lin_reportevent(priv, FC7300_LIN_EV_WKUP);
#else
          fc7300_lin_setidle(priv);
          leave_critical_section(curr_state);
          fc7300_lin_startidlesleeptimer(priv);
#endif
        }
      else
        {
          /* Wakeup signal is detected from the LIN bus, report the event
           * to the upper layer, and wait the wakeup or wakeup_internal
           * command from the upper layer.
           */

          leave_critical_section(curr_state);
          fc7300_lin_reportevent(priv, FC7300_LIN_EV_WKUP_BUS);
        }
    }
  else
    {
      leave_critical_section(curr_state);
    }

  fc7300_lin_setrxwatermark(priv, 0u);
  return OK;
}

/****************************************************************************
 * Function: fc7300_lin_breakeventhandler
 *
 * Description:
 *   Break detected interrupt handler
 *
 * Input Parameters:
 *   priv - Reference to the private data of fc7300 lin driver
 *
 * Returned Value:
 *   OK(0) on success; Negated errno on failure.
 *
 * Assumptions:
 *
 ****************************************************************************/

static int fc7300_lin_breakeventhandler(struct fc7300_lindev_s *priv)
{
  irqstate_t curr_state;

  /* Disable break detect feature */

  fc7300_lin_breakdetect(priv, false);

  if (priv->bmaster)
    {
      if (priv->frame_state == FC7300_LIN_FRAME_SEND_BREAK)
        {
          /* Set watermark to 1, then send sync and pid */

          fc7300_lin_setrxwatermark(priv, 1u);
          priv->frame_state = FC7300_LIN_FRAME_RECV_HEADER;
          fc7300_lin_filltxfifo(priv, FC7300_LIN_BUF_PSYNC(priv), 2u);
        }
      else
        {
          /* Ignore unexpected break event */
        }
    }
  else
    {
      curr_state = enter_critical_section();
      if (FC7300_LIN_ISNORMAL(priv))
        {
          /* Set watermark to 1 to receive 2 bytes sync and pid */

          fc7300_lin_setrxwatermark(priv, 1u);
          FC7300_LIN_SETBUSY(priv);
          priv->frame_state = FC7300_LIN_FRAME_RECV_HEADER;
          priv->reporting = false;

          /* Clear the status of frame error, noise error, rx overflow error,
           * receive data flag and enable these interrupts.
           */

          SET_REG_BITS(priv->base + FC7300_FCUART_STAT_OFFSET,
                       FCUART_STAT_FEF | FCUART_STAT_RORF |
                       FCUART_STAT_NF | FCUART_STAT_RDRFF);
          SET_REG_BITS(priv->base + FC7300_FCUART_CTRL_OFFSET,
                       FCUART_CTRL_FEIE | FCUART_CTRL_ORIE |
                       FCUART_CTRL_NEIE | FCUART_CTRL_RIE);
          leave_critical_section(curr_state);

          /* Start a timer to handle the timeout events */

          fc7300_lin_startframetimer(priv, priv->timeout);
        }
      else
        {
          /* Ignore another break event when the lin bus is busy. */

          leave_critical_section(curr_state);
          fc7300_lin_reportevent(priv, FC7300_LIN_ERR_SLVBUSY);
        }
    }

  return OK;
}

/****************************************************************************
 * Function: fc7300_lin_receivesynchandler
 *
 * Description:
 *   LIN sync field received event handler
 *
 * Input Parameters:
 *   priv - Reference to the private data of fc7300 lin driver
 *
 * Returned Value:
 *   OK(0) on success; Negated errno on failure.
 *
 * Assumptions:
 *
 ****************************************************************************/

static int fc7300_lin_receivesynchandler(struct fc7300_lindev_s *priv,
                                          uint8_t rxbyte)
{
  if (LIN_FRAME_SYNC_BYTE == rxbyte)
    {
      priv->frame_state = FC7300_LIN_FRAME_RECV_PID;
      return OK;
    }

  /* Received sync byte is not 0x55, report an error frame, and the
   * driver state will be released to idle state after reading the
   * error frame by the upper layer.
   */

  fc7300_lin_reportevent(priv, priv->bmaster ?
    FC7300_LIN_ERR_TXSYNC : FC7300_LIN_ERR_RXSYNC);

  return ERROR;
}

/****************************************************************************
 * Function: fc7300_lin_masterpidhandler
 *
 * Description:
 *   LIN pid event handler for master node
 *
 * Input Parameters:
 *   priv - Reference to the private data of fc7300 lin driver
 *
 * Returned Value:
 *   None.
 *
 * Assumptions:
 *
 ****************************************************************************/

static void fc7300_lin_masterpidhandler(struct fc7300_lindev_s *priv,
                                        uint8_t rxbyte)
{
  /* Prepare for the master node data transfer */

  FC7300_LIN_BUF_IND(priv) = 0u;
  fc7300_lin_setrxwatermark(priv, FC7300_LIN_BUF_DLC(priv) - 1u);

  if (FC7300_LIN_BUF_FLAG(priv) & FC7300_LIN_RESP_RX)
    {
      priv->frame_state = FC7300_LIN_FRAME_RECV_DATA;
    }
  else
    {
      priv->frame_state = FC7300_LIN_FRAME_SEND_DATA;

      /* Fill data to the transmit FIFO */

      fc7300_lin_filltxfifo(priv, FC7300_LIN_BUF_PDATA(priv, 0u),
                            FC7300_LIN_BUF_DLC(priv));
    }
}

/****************************************************************************
 * Function: fc7300_lin_masterpidhandler
 *
 * Description:
 *   LIN pid event handler for slave node
 *
 * Input Parameters:
 *   priv - Reference to the private data of fc7300 lin driver
 *
 * Returned Value:
 *   None.
 *
 * Assumptions:
 *
 ****************************************************************************/

static void fc7300_lin_slavepidhandler(struct fc7300_lindev_s *priv,
                                       uint8_t rxbyte)
{
  irqstate_t curr_state;
  uint8_t pid;
  uint8_t len;

  /* Prepare for the slave node data transfer */

  pid = rxbyte & LIN_ID_MASK;
  len = priv->slv[pid].len;
  FC7300_LIN_BUF_EVT(priv) = 0u;
  FC7300_LIN_BUF_PID(priv) = rxbyte;
  FC7300_LIN_BUF_DLC(priv) = len + 1u;
  FC7300_LIN_BUF_IND(priv) = 0u;
  fc7300_lin_setrxwatermark(priv, len);
  FC7300_LIN_BUF_FLAG(priv) = priv->slv[pid].attr;

  if (FC7300_LIN_BUF_FLAG(priv) & FC7300_LIN_RESP_RX)
    {
      /* Slave mode prepare to receive response */

      priv->frame_state = FC7300_LIN_FRAME_RECV_DATA;
    }
  else
    {
      priv->frame_state = FC7300_LIN_FRAME_SEND_DATA;

      /* If there is data in the cache, send the data, otherwise release
       * the driver to idle state.
       */

      if (len > 0u)
        {
          /* Copy data and checksum */

          memcpy(FC7300_LIN_BUF_PDATA(priv, 0u), priv->slv[pid].data, len);
          FC7300_LIN_BUF_CKSUM(priv, len) = priv->slv[pid].checksum;

          fc7300_lin_filltxfifo(priv, FC7300_LIN_BUF_PDATA(priv, 0u),
                                len + 1u);
        }
      else
        {
          curr_state = enter_critical_section();
          fc7300_lin_setidle(priv);
          leave_critical_section(curr_state);

          fc7300_lin_canceltimer(priv);
          fc7300_lin_startidlesleeptimer(priv);
        }
    }
}

/****************************************************************************
 * Function: fc7300_lin_receivepidhandler
 *
 * Description:
 *   LIN pid field received event handler
 *
 * Input Parameters:
 *   priv - Reference to the private data of fc7300 lin driver
 *
 * Returned Value:
 *   None.
 *
 * Assumptions:
 *
 ****************************************************************************/

static void fc7300_lin_receivepidhandler(struct fc7300_lindev_s *priv,
                                         uint8_t rxbyte)
{
  uint8_t pid;

  pid = rxbyte & LIN_ID_MASK;
  if (priv->bmaster)
    {
      /* Master mode: check the received byte is the pid sent before */

      if (FC7300_LIN_BUF_PID(priv) == rxbyte)
        {
          fc7300_lin_masterpidhandler(priv, rxbyte);
        }
      else
        {
          /* Received pid error, report an error frame. */

          fc7300_lin_reportevent(priv, FC7300_LIN_ERR_TXPID);
        }
    }
  else
    {
      /* Slave mode: check the parity bits of the pid */

      if (rxbyte == fc7300_lin_pidparity(pid))
        {
          fc7300_lin_slavepidhandler(priv, rxbyte);
        }
      else
        {
          /* A pid with parity error is received by the slave node,
           * report an error frame.
           */

          fc7300_lin_reportevent(priv, FC7300_LIN_ERR_PIDPARITY);
        }
    }
}

/****************************************************************************
 * Function: fc7300_lin_framehandler
 *
 * Description:
 *   Receive frame interrupt handler
 *
 * Input Parameters:
 *   priv - Reference to the private data of fc7300 lin driver
 *
 * Returned Value:
 *   OK(0) on success; Negated errno on failure.
 *
 * Assumptions:
 *
 ****************************************************************************/

static int fc7300_lin_framehandler(struct fc7300_lindev_s *priv)
{
  uint8_t buf[FC7300_LIN_FIFO_DEPTH];
  uint8_t count;

  /* Read data from the receive FIFO */

  count = fc7300_lin_readrxfifo(priv, buf);
  if (0u == count)
    {
      return OK;
    }

  switch (priv->frame_state)
    {
      case FC7300_LIN_FRAME_RECV_HEADER:
        {
          if ((OK == fc7300_lin_receivesynchandler(priv, buf[0u])) &&
              (count == 2u))
            {
              fc7300_lin_receivepidhandler(priv, buf[1u]);
            }
        }
        break;
      case FC7300_LIN_FRAME_RECV_DATA:
        {
          //coverity[OVERRUN:SUPPRESS]
          fc7300_lin_receivedata(priv, buf, count);
        }
        break;
      case FC7300_LIN_FRAME_SEND_DATA:
        {
          //coverity[OVERRUN:SUPPRESS]
          fc7300_lin_senddata(priv, buf, count);
        }
        break;
#ifndef CONFIG_FC7300_LIN_ISR_THREAD
#ifdef CONFIG_FC7300_LIN_BREAK_SOFTWARE
      case FC7300_LIN_FRAME_SEND_BREAK:
        {
          fc7300_lin_breakeventbybaud(priv);
        }
        break;
#endif
#endif
      default:
        break;
    }

  return OK;
}

#ifdef CONFIG_FC7300_LIN_ISR_THREAD
/****************************************************************************
 * Function: fc7300_lin_isr
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

__ramfunc__ static int fc7300_lin_isr(int irq, void *context, void *arg)
{
  struct fc7300_lindev_s *priv = (struct fc7300_lindev_s *)arg;
  (void)context;
  (void)priv;
  up_disable_irq(irq);

#ifdef CONFIG_FC7300_LIN_BREAK_SOFTWARE
  if (priv->frame_state == FC7300_LIN_FRAME_SEND_BREAK)
    {
      if (READ_REG_BITS(priv->base + FC7300_FCUART_STAT_OFFSET,
                        FCUART_STAT_RDRFF))
        {
          SET_REG_BITS(priv->base + FC7300_FCUART_STAT_OFFSET,
                       FCUART_STAT_RDRFF);
          fc7300_lin_breakeventbybaud(priv);
        }
    }
#endif

  return IRQ_WAKE_THREAD;
}
#endif

/****************************************************************************
 * Function: fc7300_lin_interrupt
 *
 * Description:
 *   The interrupt handler of the LIN controller
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

#ifdef CONFIG_FC7300_LIN_INTERRUPT_EXECUT_TIMEMAX
static clock_t g_lin_isr_max_ts ;
#endif

__ramfunc__ static int fc7300_lin_interrupt(int irq, void *context, void *arg)
{
#ifdef CONFIG_FC7300_LIN_INTERRUPT_EXECUT_TIMEMAX
  clock_t st_time;
  clock_t end_time;
  st_time = up_perf_gettime();
#endif
  struct fc7300_lindev_s *priv = (struct fc7300_lindev_s *)arg;
  int ret = OK;
  uint32_t status;

  /* Read status and clear status flag */

  status = READ_REG_U32(priv->base + FC7300_FCUART_STAT_OFFSET);
  WRITE_REG_U32(priv->base + FC7300_FCUART_STAT_OFFSET, status);

  /* Idle line interrupt */

  if (status & FCUART_STAT_IDLEF)
    {
      if (READ_REG_BITS(priv->base + FC7300_FCUART_CTRL_OFFSET,
                        FCUART_CTRL_IIE))
        {
          ret = fc7300_lin_idlelinehandler(priv);
        }
    }

  /* RXD pin active edge is detected */

  if (status & FCUART_STAT_RPAEIF)
    {
      if (READ_REG_BITS(priv->base + FC7300_FCUART_BAUD_OFFSET,
                        FCUART_BAUD_RIAEIE))
        {
          ret = fc7300_lin_rxdactivehandler(priv);
        }
    }

  /* Receive data is available */

  if (status & FCUART_STAT_RDRFF)
    {
      ret = fc7300_lin_framehandler(priv);
    }

  /* Break character is detected */

  if (status & FCUART_STAT_LBKDIF)
    {
      if (READ_REG_BITS(priv->base + FC7300_FCUART_BAUD_OFFSET,
                        FCUART_BAUD_LBKDIE))
        {
          ret = fc7300_lin_breakeventhandler(priv);
        }
    }

  /* Errors occurred
   * FEF：  frame error
   * RORF:  receiver overrun
   */

  if (status & FC7300_LIN_ERROR_MASK)
    {
      ret = fc7300_lin_errorhandler(priv, status);
    }

#ifdef CONFIG_FC7300_LIN_ISR_THREAD
  fc7300_clearpendingisr(irq);
  up_enable_irq(irq);
#endif

#ifdef CONFIG_FC7300_LIN_INTERRUPT_EXECUT_TIMEMAX
  end_time = up_perf_gettime();
  g_lin_isr_max_ts = MAX(g_lin_isr_max_ts, end_time - st_time);
#endif

  return ret;
}

/****************************************************************************
 * Function: fc7300_lin_datalength
 *
 * Description:
 *   Get the length of LIN frame based on can_dlc
 *
 * Input Parameters:
 *   pid  - LIN pid
 *   can_dlc  - can_dlc member of struct can_frame
 *
 * Returned Value:
 *   The length of LIN frame.
 *
 * Assumptions:
 *
 ****************************************************************************/

static uint8_t fc7300_lin_datalength(uint8_t pid, uint8_t can_dlc)
{
  if (can_dlc == 0u)
    {
      switch ((pid >> 4u) & 0x03u)
        {
          case 3u:
            can_dlc = 8u;
            break;
          case 2u:
            can_dlc = 4u;
            break;
          default:
            can_dlc = 2u;
            break;
        }
    }

  return can_dlc;
}

/****************************************************************************
 * Function: fc7300_lin_pidparity
 *
 * Description:
 *   Calculate the parity of LIN pid
 *
 * Input Parameters:
 *   uint8_t  - LIN pid value
 *
 * Returned Value:
 *   Returns the LIN pid with parity.
 *
 * Assumptions:
 *
 ****************************************************************************/

static uint8_t fc7300_lin_pidparity(uint8_t pid)
{
  uint8_t parity;

  parity = (uint8_t)((((0x1u & (pid >> 0u)) ^ (0x1u & (pid >> 1u)) ^
                    (0x1u & (pid >> 2u)) ^ (0x1u & (pid >> 4u))) << 6u) |
                    ((0xffu ^ (0x1u & (pid >> 1U)) ^ (0x1u & (pid >> 3U)) ^
                    (0x1u & (pid >> 4U)) ^ (0x1u & (pid >> 5U))) << 7u));

  return (uint8_t)(pid | parity);
}

/****************************************************************************
 * Function: fc7300_lin_datachecksum
 *
 * Description:
 *   Calculate the parity of LIN pid
 *
 * Input Parameters:
 *   extcksum - Enhanced checksum or classic checksum
 *   pid  - LIN pid with parity
 *   data - Pointer to data address
 *   len  - Data length
 *
 * Returned Value:
 *   Returns the checksum of LIN data field.
 *
 * Assumptions:
 *
 ****************************************************************************/

static uint8_t fc7300_lin_datachecksum(bool extcksum, uint8_t pid,
                                       const uint8_t *data, uint8_t len)
{
  uint16_t checksum = 0u;
  uint8_t i;

  if (extcksum)
    {
      checksum = pid;
    }

  /* If pid is 0x3c(0x3c), 0x7d(0x3d), 0xfe(0x3e), 0xbf(0x3f), force the
   * classic checksum to be used
   */

  if ((0x3cu == pid) || (0x7du == pid) || (0xfeu == pid) || (0xbfu == pid))
    {
      checksum = 0U;
    }

  for (i = 0U; i < len; ++i)
    {
      checksum += data[i];
      if (checksum > 0xffu)
        {
          checksum -= 0xffu;
        }
    }

  return (uint8_t)(~checksum);
}

/****************************************************************************
 * Function: fc7300_lin_slavectrl
 *
 * Description:
 *   Set the response direction and checksum mode for slave node
 *
 * Input Parameters:
 *   priv       - Reference to the LIN driver private data structure
 *   frame      - The LIN frame
 *
 * Returned Value:
 *   None.
 *
 * Assumptions:
 *
 ****************************************************************************/

static void fc7300_lin_slavectrl(struct fc7300_lindev_s *priv,
                                 const struct can_frame *frame)
{
  uint8_t pid;

  pid = frame->can_id & LIN_ID_MASK;
  LIN_CLEAR_SLV_ATTRIBUTE(priv, pid);

  /* Save the response direction */

  if (frame->can_id & LIN_RTR_FLAG)
    {
      LIN_SET_SLV_RESP_RX(priv, pid);
    }

  /* Save the checksum mode */

  if (frame->can_id & LIN_CHECKSUM_EXTENDED)
    {
      LIN_SET_SLV_EXT_CKSUM(priv, pid);
    }

  /* Save the single response mode */

  if (frame->can_id & LIN_SINGLE_RESPONSE)
    {
      LIN_SET_SLV_SINGLE_RESP(priv, pid);
    }
}

/****************************************************************************
 * Function: fc7300_lin_buildeventframe
 *
 * Description:
 *   Build an event frame.
 *
 * Input Parameters:
 *   priv  - Reference to the LIN driver private data structure
 *   frame - The dstination lin frame
 *   event   - The error id
 *
 * Returned Value:
 *   None.
 *
 * Assumptions:
 *
 ****************************************************************************/

static void fc7300_lin_buildeventframe(struct fc7300_lindev_s *priv,
                                       struct can_frame *frame)
{
  uint8_t event;

  event = FC7300_LIN_BUF_EVT(priv);
  memset(frame, 0, sizeof(struct can_frame));

  if (event == FC7300_LIN_TX_OK)
    {
      frame->can_id = (FC7300_LIN_BUF_PID(priv) & LIN_ID_MASK) |
                      LIN_TCF_FLAG;
    }
  else if (event <= FC7300_LIN_EV_MAXID)
    {
      /* State change event, build a state change event frame */

      frame->can_id = LIN_EVT_FLAG;
      frame->can_dlc = LIN_MAX_DATA_LENGTH;
      frame->data[0u] = 1u << (event - 1u);
    }
  else
    {
      /* Error event, build an error frame */

      frame->can_id = (FC7300_LIN_BUF_PID(priv) & LIN_ID_MASK) |
                      LIN_ERR_FLAG;
      frame->can_dlc = LIN_ERR_DATA_LENGTH;
      frame->data[0] = 1u << (event / 8u - 1u);
      frame->data[event / 8u] = 1u << (event % 8u);
    }
}

/****************************************************************************
 * Function: fc7300_lin_buildframe
 *
 * Description:
 *   Build lin frame.
 *
 * Input Parameters:
 *   priv  - Reference to the LIN driver private data structure
 *   frame - The dstination lin frame
 *
 * Returned Value:
 *   None.
 *
 * Assumptions:
 *
 ****************************************************************************/

static void fc7300_lin_buildframe(struct fc7300_lindev_s *priv,
                                  struct can_frame *frame)
{
  uint8_t len;

  memset(frame, 0, sizeof(struct can_frame));
  len = FC7300_LIN_BUF_DLC(priv) - 1u;
  frame->can_id = FC7300_LIN_BUF_PID(priv) & LIN_ID_MASK;
  frame->can_dlc = len;

  if (len > 0u)
    {
      memcpy(frame->data, FC7300_LIN_BUF_PDATA(priv, 0u), len);
    }
}

/****************************************************************************
 * Function: fc7300_lin_freenetpkt
 *
 * Description:
 *   Free the netpacket hold by the driver.
 *
 * Input Parameters:
 *   priv  - Reference to the LIN driver private data structure
 *
 * Returned Value:
 *   None.
 *
 * Assumptions:
 *
 ****************************************************************************/

static inline void fc7300_lin_freenetpkt(struct fc7300_lindev_s *priv)
{
  if (priv->pkt)
    {
      netpkt_free(&priv->dev, priv->pkt, NETPKT_TX);
      priv->pkt = NULL;
    }
}

/****************************************************************************
 * Function: fc7300_lin_reportevent
 *
 * Description:
 *   Report an event to the upper layer, and the upper layer call receive
 *   to read the event frame(state changed event or errors).
 *
 * Input Parameters:
 *   priv  - Reference to the LIN driver private data structure
 *   event - The event id
 *
 * Returned Value:
 *   None.
 *
 * Assumptions:
 *
 ****************************************************************************/

static void fc7300_lin_reportevent(struct fc7300_lindev_s *priv,
                                   uint8_t event)
{
  irqstate_t curr_state;

  curr_state = enter_critical_section();
  if (event >= FC7300_LIN_ERR_START)
    {
      /* If the event is an error, check whether there are any errors being
       * reported earlier. The driver only reports the first error during
       * frame transmission.
       */

      if (priv->reporting)
        {
          leave_critical_section(curr_state);
          return;
        }

      priv->reporting = true;
    }

  fc7300_lin_canceltimer(priv);
  fc7300_lin_freenetpkt(priv);
  priv->frame_state = FC7300_LIN_FRAME_EVENT;
  if (!LIN_FRAME_RX_AVAILABLE(priv))
    {
      /* Report event frame if receive buffer is available. */

      FC7300_LIN_BUF_EVT(priv) = event;
      LIN_FRAME_RX_VALIDATE(priv);
      leave_critical_section(curr_state);
      netdev_lower_rxready(&priv->dev);
    }
  else
    {
      /* Release the driver if receive buffer is unavailable. */

      if (!FC7300_LIN_ISSLEEP(priv))
        {
          fc7300_lin_setidle(priv);
          leave_critical_section(curr_state);
          fc7300_lin_startidlesleeptimer(priv);
        }
      else
        {
          leave_critical_section(curr_state);
        }
    }
}

/****************************************************************************
 * Function: fc7300_lin_sendheader
 *
 * Description:
 *   Start a LIN frame header (break field)
 *
 * Input Parameters:
 *   priv  - Reference to the LIN driver private data structure
 *
 * Returned Value:
 *   OK(0) on success; Negated errno on failure.
 *
 * Assumptions:
 *
 ****************************************************************************/

static int fc7300_lin_sendheader(struct fc7300_lindev_s *priv)
{
  int ret = -EAGAIN;

  if (priv->frame_state == FC7300_LIN_FRAME_PEOCESSING)
    {
      priv->reporting = false;

#ifdef CONFIG_FC7300_LIN_BREAK_SOFTWARE
      priv->frame_state = FC7300_LIN_FRAME_SEND_BREAK;
      fc7300_lin_sendbreakbybaud(priv);
#else

      /* Enable break detect and send break character */

      fc7300_lin_breakdetect(priv, true);

      /* Send break character */

      priv->frame_state = FC7300_LIN_FRAME_SEND_BREAK;
      WRITE_REG_U32(priv->base + FC7300_FCUART_DATA_OFFSET,
                    FCUART_DATA_FETSC);
#endif /* CONFIG_FC7300_LIN_BREAK_SOFTWARE */

      /* Clear the status of frame error, noise error, rx overflow error,
       * receive data flag and enable these interrupts.
       */

      SET_REG_BITS(priv->base + FC7300_FCUART_STAT_OFFSET,
                   FCUART_STAT_FEF | FCUART_STAT_RORF |
                   FCUART_STAT_NF | FCUART_STAT_RDRFF);
      SET_REG_BITS(priv->base + FC7300_FCUART_CTRL_OFFSET,
                   FCUART_CTRL_FEIE | FCUART_CTRL_ORIE |
                   FCUART_CTRL_NEIE | FCUART_CTRL_RIE);

      /* Start a timer to handle the timeout events */

      ret = fc7300_lin_startframetimer(priv, priv->timeout);
    }

  return ret;
}

/****************************************************************************
 * Function: fc7300_lin_senddata
 *
 * Description:
 *   Send LIN frame response data field, this function is called in
 *   interrupt context
 *
 * Input Parameters:
 *   priv   - Reference to the LIN driver private data structure
 *   buf    - The received back data buffer
 *   len    - The received back data length
 *
 * Returned Value:
 *   None.
 *
 * Assumptions:
 *
 ****************************************************************************/

static void fc7300_lin_senddata(struct fc7300_lindev_s *priv,
                                uint8_t *buf, uint8_t len)
{
  irqstate_t curr_state;
  uint8_t pid;

  if (len == FC7300_LIN_BUF_DLC(priv))
    {
      if (!memcmp(buf, FC7300_LIN_BUF_PDATA(priv, 0u), len))
        {
          if (priv->bmaster)
            {
              if (fc7300_lin_checksleepsignal(priv, FC7300_LIN_EV_SLEEP))
                {
                  return;
                }

              /* Master sent sucessfully, free net packet, release the driver
               * and report transmit done to the upper layer
               */

              netdev_lower_txdone(&priv->dev);
              fc7300_lin_reportevent(priv, FC7300_LIN_TX_OK);
            }
          else
            {
              /* Slave node check single response mode */

              pid = FC7300_LIN_BUF_PID(priv) & LIN_ID_MASK;
              if (FC7300_LIN_BUF_FLAG(priv) & FC7300_LIN_SINGLE_RESP)
                {
                  priv->slv[pid].len = 0u;
                }

              fc7300_lin_canceltimer(priv);
              curr_state = enter_critical_section();
              fc7300_lin_setidle(priv);
              leave_critical_section(curr_state);

              fc7300_lin_startidlesleeptimer(priv);
            }
        }
      else
        {
          /* The data received back is not equal to the data sent, report an
           * error frame
           */

          if (buf[len - 1u] == FC7300_LIN_BUF_DATA(priv, len - 1u))
            {
              fc7300_lin_reportevent(priv, FC7300_LIN_ERR_TXDATA);
            }
          else
            {
              fc7300_lin_reportevent(priv, FC7300_LIN_ERR_TXCKSUM);
            }
        }
    }
  else
    {
      /* The length received back is not as expected, report an error frame,
       * and set the driver to idle state
       */

      fc7300_lin_reportevent(priv, FC7300_LIN_ERR_TXLEN);
    }
}

/****************************************************************************
 * Function: fc7300_lin_receivedata
 *
 * Description:
 *   Receive LIN frame response data field
 *
 * Input Parameters:
 *   priv   - Reference to the LIN driver private data structure
 *   buf    - The received data buffer
 *   len    - The received data length
 *
 * Returned Value:
 *   None.
 *
 * Assumptions:
 *
 ****************************************************************************/

static void fc7300_lin_receivedata(struct fc7300_lindev_s *priv,
                                   uint8_t *buf, uint8_t len)
{
  uint8_t index = FC7300_LIN_BUF_IND(priv);

  if ((index + len) == FC7300_LIN_BUF_DLC(priv))
    {
      memcpy(FC7300_LIN_BUF_PDATA(priv, index), buf, len);

      if (!priv->bmaster)
        {
          if (fc7300_lin_checksleepsignal(priv, FC7300_LIN_EV_SLEEP_BUS))
            {
              return;
            }
        }

      fc7300_lin_reportevent(priv, FC7300_LIN_SUCCESS);
    }
  else if ((index + len) < FC7300_LIN_BUF_DLC(priv))
    {
      memcpy(FC7300_LIN_BUF_PDATA(priv, index), buf, len);
      FC7300_LIN_BUF_IND(priv) = index + len;
    }
  else
    {
      /* It won't come here in normal work state, report error frame */

      fc7300_lin_reportevent(priv, FC7300_LIN_ERR_RXLEN);
    }
}

/****************************************************************************
 * Function: fc7300_lin_parseframe
 *
 * Description:
 *   Parse the frame and save the fram information to the driver private
 *   structure
 *
 * Input Parameters:
 *   priv  - Reference to the LIN driver private data structure
 *   frame   The frame information structure
 *
 * Returned Value:
 *   OK(0) on success;
 *   FC7300_LIN_FRAME_PEOCESSING the transmission is on going;
 *   Negated errno on failure.
 *
 * Assumptions:
 *
 ****************************************************************************/

static int fc7300_lin_parseframe(struct fc7300_lindev_s *priv,
                                 const struct can_frame *frame)
{
  int ret;
  uint8_t pid;
  uint8_t data_len;

  /* Parse the frame */

  pid = frame->can_id & LIN_ID_MASK;
  data_len = fc7300_lin_datalength(pid, frame->can_dlc);
  if (data_len > 8u)
    {
      nwarn("%d:ERROR: invalid dlc  %u\n", __LINE__, data_len);
      return -EINVAL;
    }

  if (priv->bmaster)
    {
      /* Save the frame information to transmit buffer */

      FC7300_LIN_BUF_FLAG(priv) =
        (frame->can_id & LIN_CHECKSUM_EXTENDED) ? FC7300_LIN_EXT_CKSUM : 0u;
      FC7300_LIN_BUF_DLC(priv) = data_len + 1u;
      FC7300_LIN_BUF_IND(priv) = 0u;
      FC7300_LIN_BUF_EVT(priv) = 0u;
      FC7300_LIN_BUF_SYNC(priv) = LIN_FRAME_SYNC_BYTE;
      FC7300_LIN_BUF_PID(priv) = fc7300_lin_pidparity(pid);

      /* If master send response, copy the response to tx buffer */

      if (frame->can_id & LIN_RTR_FLAG)
        {
          FC7300_LIN_BUF_FLAG(priv) |= FC7300_LIN_RESP_RX;
        }
      else
        {
          memcpy(FC7300_LIN_BUF_PDATA(priv, 0u), frame->data, data_len);
          FC7300_LIN_BUF_CKSUM(priv, data_len) =
            fc7300_lin_datachecksum(frame->can_id & LIN_CHECKSUM_EXTENDED,
                          FC7300_LIN_BUF_PID(priv), frame->data, data_len);
        }

      ret = 1u;
    }
  else
    {
      if ((frame->can_id & LIN_RTR_FLAG) &&
          (frame->can_id & LIN_CACHE_RESPONSE))
        {
          /* It is incorrect to set both the LIN_RTR_FLAG and
           * LIN_CACHE_RESPONSE flag, return error
           */

          nwarn("%d:ERROR: control flag error\n", __LINE__);
          ret = -EINVAL;
        }
      else
        {
          /* Slave node: Writing a frame means passing control parameters,
           * and if LIN_CACHE_RESPONSE is set, the frame data will be cached.
           */

          priv->slv[pid].len = data_len;
          fc7300_lin_slavectrl(priv, frame);

          if (frame->can_id & LIN_CACHE_RESPONSE)
            {
              /* Copy the response data to cache buffer */

              memcpy(priv->slv[pid].data, frame->data, data_len);
              priv->slv[pid].checksum = fc7300_lin_datachecksum(
                    frame->can_id & LIN_CHECKSUM_EXTENDED,
                    fc7300_lin_pidparity(pid), frame->data, data_len);
            }

          ret = OK;
        }
    }

  return ret;
}

/****************************************************************************
 * Function: fc7300_lin_privsetup
 *
 * Description:
 *   Setup the private data structure of fc7300 lin driver
 *
 * Input Parameters:
 *   priv - Reference to the private data of fc7300 lin driver
 *
 * Returned Value:
 *   return the pointer to the private data structure of the indexed lin
 *
 * Assumptions:
 *
 ****************************************************************************/

static struct fc7300_lindev_s *fc7300_lin_privsetup(int intf)
{
  struct fc7300_lindev_s *priv;
  enum pcc_clksrctype_e clksrc;

  switch (intf)
    {
#ifdef CONFIG_FC7300_LIN0
    case 0:
      {
        priv = &g_fc7300lin0;
        priv->cfg = &g_fc7300lin0config;
        priv->base = FC7300_FCUART0_BASE;
        priv->baudrate = CONFIG_FC7300_LIN0_DEFAULT_BAUDRATE;

#ifdef CONFIG_FC7300_LIN0_MASTER
        priv->bmaster = true;
#else
        priv->bmaster = false;
        priv->slv = g_fc7300lin0slave;
#endif /* CONFIG_FC7300_LIN0_MASTER */

        clksrc = PCC_CLK_FCUART0;
      }
      break;
#endif /* CONFIG_FC7300_LIN0 */

#ifdef CONFIG_FC7300_LIN1
    case 1:
      {
        priv = &g_fc7300lin1;
        priv->cfg = &g_fc7300lin1config;
        priv->base = FC7300_FCUART1_BASE;
        priv->baudrate = CONFIG_FC7300_LIN1_DEFAULT_BAUDRATE;

#ifdef CONFIG_FC7300_LIN1_MASTER
        priv->bmaster = true;
#else
        priv->bmaster = false;
        priv->slv = g_fc7300lin1slave;
#endif /* CONFIG_FC7300_LIN1_MASTER */

        clksrc = PCC_CLK_FCUART1;
      }
      break;
#endif /* CONFIG_FC7300_LIN1 */

#ifdef CONFIG_FC7300_LIN2
    case 2:
      {
        priv = &g_fc7300lin2;
        priv->cfg = &g_fc7300lin2config;
        priv->base = FC7300_FCUART2_BASE;
        priv->baudrate = CONFIG_FC7300_LIN2_DEFAULT_BAUDRATE;

#ifdef CONFIG_FC7300_LIN2_MASTER
        priv->bmaster = true;
#else
        priv->bmaster = false;
        priv->slv = g_fc7300lin2slave;
#endif /* CONFIG_FC7300_LIN2_MASTER */

        clksrc = PCC_CLK_FCUART2;
      }
      break;
#endif /* CONFIG_FC7300_LIN2 */

#ifdef CONFIG_FC7300_LIN3
    case 3:
      {
        priv = &g_fc7300lin3;
        priv->cfg = &g_fc7300lin3config;
        priv->base = FC7300_FCUART3_BASE;
        priv->baudrate = CONFIG_FC7300_LIN3_DEFAULT_BAUDRATE;

#ifdef CONFIG_FC7300_LIN3_MASTER
        priv->bmaster = true;
#else
        priv->bmaster = false;
        priv->slv = g_fc7300lin3slave;
#endif /* CONFIG_FC7300_LIN3_MASTER */

        clksrc = PCC_CLK_FCUART3;
      }
      break;
#endif /* CONFIG_FC7300_LIN3 */

#ifdef CONFIG_FC7300_LIN4
    case 4:
      {
        priv = &g_fc7300lin4;
        priv->cfg = &g_fc7300lin4config;
        priv->base = FC7300_FCUART4_BASE;
        priv->baudrate = CONFIG_FC7300_LIN4_DEFAULT_BAUDRATE;

#ifdef CONFIG_FC7300_LIN4_MASTER
        priv->bmaster = true;
#else
        priv->bmaster = false;
        priv->slv = g_fc7300lin4slave;
#endif /* CONFIG_FC7300_LIN4_MASTER */

        clksrc = PCC_CLK_FCUART4;
      }
      break;
#endif /* CONFIG_FC7300_LIN4 */

#ifdef CONFIG_FC7300_LIN5
    case 5:
      {
        priv = &g_fc7300lin5;
        priv->cfg = &g_fc7300lin5config;
        priv->base = FC7300_FCUART5_BASE;
        priv->baudrate = CONFIG_FC7300_LIN5_DEFAULT_BAUDRATE;

#ifdef CONFIG_FC7300_LIN5_MASTER
        priv->bmaster = true;
#else
        priv->bmaster = false;
        priv->slv = g_fc7300lin5slave;
#endif /* CONFIG_FC7300_LIN5_MASTER */

        clksrc = PCC_CLK_FCUART5;
      }
      break;
#endif /* CONFIG_FC7300_LIN5 */

#ifdef CONFIG_FC7300_LIN6
    case 6:
      {
        priv = &g_fc7300lin6;
        priv->cfg = &g_fc7300lin6config;
        priv->base = FC7300_FCUART6_BASE;
        priv->baudrate = CONFIG_FC7300_LIN6_DEFAULT_BAUDRATE;

#ifdef CONFIG_FC7300_LIN6_MASTER
        priv->bmaster = true;
#else
        priv->bmaster = false;
        priv->slv = g_fc7300lin6slave;
#endif /* CONFIG_FC7300_LIN6_MASTER */

        clksrc = PCC_CLK_FCUART6;
      }
      break;
#endif /* CONFIG_FC7300_LIN6 */

#ifdef CONFIG_FC7300_LIN7
    case 7:
      {
        priv = &g_fc7300lin7;
        priv->cfg = &g_fc7300lin7config;
        priv->base = FC7300_FCUART7_BASE;
        priv->baudrate = CONFIG_FC7300_LIN7_DEFAULT_BAUDRATE;

#ifdef CONFIG_FC7300_LIN7_MASTER
        priv->bmaster = true;
#else
        priv->bmaster = false;
        priv->slv = g_fc7300lin7slave;
#endif /* CONFIG_FC7300_LIN7_MASTER */

        clksrc = PCC_CLK_FCUART7;
      }
      break;
#endif /* CONFIG_FC7300_LIN7 */

#ifdef CONFIG_FC7300_LIN8
    case 8:
      {
        priv = &g_fc7300lin8;
        priv->cfg = &g_fc7300lin8config;
        priv->base = FC7300_FCUART8_BASE;
        priv->baudrate = CONFIG_FC7300_LIN8_DEFAULT_BAUDRATE;

#ifdef CONFIG_FC7300_LIN8_MASTER
        priv->bmaster = true;
#else
        priv->bmaster = false;
        priv->slv = g_fc7300lin8slave;
#endif /* CONFIG_FC7300_LIN8_MASTER */

        clksrc = PCC_CLK_FCUART8;
      }
      break;
#endif /* CONFIG_FC7300_LIN8 */

#ifdef CONFIG_FC7300_LIN9
    case 9:
      {
        priv = &g_fc7300lin9;
        priv->cfg = &g_fc7300lin9config;
        priv->base = FC7300_FCUART9_BASE;
        priv->baudrate = CONFIG_FC7300_LIN9_DEFAULT_BAUDRATE;

#ifdef CONFIG_FC7300_LIN9_MASTER
        priv->bmaster = true;
#else
        priv->bmaster = false;
        priv->slv = g_fc7300lin9slave;
#endif /* CONFIG_FC7300_LIN9_MASTER */

        clksrc = PCC_CLK_FCUART9;
      }
      break;
#endif /* CONFIG_FC7300_LIN9 */

#ifdef CONFIG_FC7300_LIN10
    case 10:
      {
        priv = &g_fc7300lin10;
        priv->cfg = &g_fc7300lin10config;
        priv->base = FC7300_FCUART10_BASE;
        priv->baudrate = CONFIG_FC7300_LIN10_DEFAULT_BAUDRATE;

#ifdef CONFIG_FC7300_LIN10_MASTER
        priv->bmaster = true;
#else
        priv->bmaster = false;
        priv->slv = g_fc7300lin10slave;
#endif /* CONFIG_FC7300_LIN10_MASTER */

        clksrc = PCC_CLK_FCUART10;
      }
      break;
#endif /* CONFIG_FC7300_LIN10 */

#ifdef CONFIG_FC7300_LIN11
    case 11:
      {
        priv = &g_fc7300lin11;
        priv->cfg = &g_fc7300lin11config;
        priv->base = FC7300_FCUART11_BASE;
        priv->baudrate = CONFIG_FC7300_LIN11_DEFAULT_BAUDRATE;

#ifdef CONFIG_FC7300_LIN11_MASTER
        priv->bmaster = true;
#else
        priv->bmaster = false;
        priv->slv = g_fc7300lin11slave;
#endif /* CONFIG_FC7300_LIN11_MASTER */

        clksrc = PCC_CLK_FCUART11;
      }
      break;
#endif /* CONFIG_FC7300_LIN11 */

#ifdef CONFIG_FC7300_LIN12
    case 12:
      {
        priv = &g_fc7300lin12;
        priv->cfg = &g_fc7300lin12config;
        priv->base = FC7300_FCUART12_BASE;
        priv->baudrate = CONFIG_FC7300_LIN12_DEFAULT_BAUDRATE;

#ifdef CONFIG_FC7300_LIN12_MASTER
        priv->bmaster = true;
#else
        priv->bmaster = false;
        priv->slv = g_fc7300lin12slave;
#endif /* CONFIG_FC7300_LIN12_MASTER */

        clksrc = PCC_CLK_FCUART12;
      }
      break;
#endif /* CONFIG_FC7300_LIN12 */

#ifdef CONFIG_FC7300_LIN13
    case 13:
      {
        priv = &g_fc7300lin13;
        priv->cfg = &g_fc7300lin13config;
        riv->base = FC7300_FCUART13_BASE;
        priv->baudrate = CONFIG_FC7300_LIN13_DEFAULT_BAUDRATE;

#ifdef CONFIG_FC7300_LIN13_MASTER
        priv->bmaster = true;
#else
        priv->bmaster = false;
        priv->slv = g_fc7300lin13slave;
#endif /* CONFIG_FC7300_LIN13_MASTER */

        clksrc = PCC_CLK_FCUART13;
      }
      break;
#endif /* CONFIG_FC7300_LIN13 */

#ifdef CONFIG_FC7300_LIN14
    case 14:
      {
        priv = &g_fc7300lin14;
        priv->cfg = &g_fc7300lin14config;
        priv->base = FC7300_FCUART14_BASE;
        priv->baudrate = CONFIG_FC7300_LIN14_DEFAULT_BAUDRATE;

#ifdef CONFIG_FC7300_LIN14_MASTER
        priv->bmaster = true;
#else
        priv->bmaster = false;
        priv->slv = g_fc7300lin14slave;
#endif /* CONFIG_FC7300_LIN14_MASTER */

        clksrc = PCC_CLK_FCUART14;
      }
      break;
#endif /* CONFIG_FC7300_LIN14 */

#ifdef CONFIG_FC7300_LIN15
    case 15:
      {
        priv = &g_fc7300lin15;
        priv->cfg = &g_fc7300lin15config;
        priv->base = FC7300_FCUART15_BASE;
        priv->baudrate = CONFIG_FC7300_LIN15_DEFAULT_BAUDRATE;

#ifdef CONFIG_FC7300_LIN15_MASTER
        priv->bmaster = true;
#else
        priv->bmaster = false;
        priv->slv = g_fc7300lin15slave;
#endif /* CONFIG_FC7300_LIN15_MASTER */

        clksrc = PCC_CLK_FCUART15;
      }
      break;
#endif /* CONFIG_FC7300_LIN15 */

#ifdef CONFIG_FC7300_LIN16
    case 16:
      {
        priv = &g_fc7300lin16;
        priv->cfg = &g_fc7300lin16config;
        priv->base = FC7300_FCUART16_BASE;
        priv->baudrate = CONFIG_FC7300_LIN16_DEFAULT_BAUDRATE;

#ifdef CONFIG_FC7300_LIN16_MASTER
        priv->bmaster = true;
#else
        priv->bmaster = false;
        priv->slv = g_fc7300lin16slave;
#endif /* CONFIG_FC7300_LIN16_MASTER */

        clksrc = PCC_CLK_FCUART16;
      }
      break;
#endif /* CONFIG_FC7300_LIN16 */

#ifdef CONFIG_FC7300_LIN17
    case 17:
      {
        priv = &g_fc7300lin17;
        priv->cfg = &g_fc7300lin17config;
        priv->base = FC7300_FCUART17_BASE;
        priv->baudrate = CONFIG_FC7300_LIN17_DEFAULT_BAUDRATE;

#ifdef CONFIG_FC7300_LIN17_MASTER
        priv->bmaster = true;
#else
        priv->bmaster = false;
        priv->slv = g_fc7300lin17slave;
#endif /* CONFIG_FC7300_LIN17_MASTER */

        clksrc = PCC_CLK_FCUART17;
      }
      break;
#endif /* CONFIG_FC7300_LIN17 */

    default:
      priv = NULL;
      break;
  }

  /* Get the source clock frequency of LIN controller */

  if (priv)
    {
      if (fc7300_get_pclkfreq(clksrc, &priv->srcclk))
        {
          priv = NULL;
        }
      else
        {
          fc7300_lin_calculatetimeout(priv);
        }
    }

  return priv;
}

/****************************************************************************
 * Function: fc7300_lin_pinconfig
 *
 * Description:
 *   Configure the fc7300 lin IO pin mode
 *
 * Input Parameters:
 *   priv - Reference to the private data of fc7300 lin driver
 *
 * Returned Value:
 *   None
 *
 * Assumptions:
 *
 ****************************************************************************/

static void fc7300_lin_pinconfig(struct fc7300_lindev_s *priv)
{
  fc7300_pinconfig(priv->cfg->tx_pin);
  fc7300_pinconfig(priv->cfg->rx_pin);
  if (priv->cfg->enable_pin > 0u)
    {
      fc7300_pinconfig(priv->cfg->enable_pin);
#ifdef CONFIG_FC7300_LIN_STB_PIN_CFG
      fc7300_gpiowrite(priv->cfg->enable_pin, (bool)priv->cfg->enable_level);
#endif
    }
}

/****************************************************************************
 * Function: fc7300_lin_setidle
 *
 * Description:
 *   Set the LIN controller into idle state
 *
 * Input Parameters:
 *   priv - Reference to the private data of fc7300 lin driver
 *
 * Returned Value:
 *   None
 *
 * Assumptions:
 *
 ****************************************************************************/

static void fc7300_lin_setidle(struct fc7300_lindev_s *priv)
{
  uint32_t val;

  /* Master node: disable the break detect and break detect interrupt
   * slave node: enable the break detect and break detect interrupt
   */

  fc7300_lin_breakdetect(priv, !priv->bmaster);

  /* Set receive watermark to 0 */

  fc7300_lin_setrxwatermark(priv, 0);

  /* Disable receive interrupt, idle interrupt and error interrupts */

  val = READ_REG_U32(priv->base + FC7300_FCUART_CTRL_OFFSET);
  val = val & (~(FCUART_CTRL_IIE | FCUART_CTRL_FEIE | FCUART_CTRL_ORIE |
        FCUART_CTRL_NEIE | FCUART_CTRL_RIE));
  WRITE_REG_U32(priv->base + FC7300_FCUART_CTRL_OFFSET, val);

  /* Disable rx input active edge interrupt */

  CLEAR_REG_BITS(priv->base + FC7300_FCUART_BAUD_OFFSET, FCUART_BAUD_RIAEIE);

  /* Clear status */

  val = READ_REG_U32(priv->base + FC7300_FCUART_STAT_OFFSET) &
        (~FCUART_STAT_RXINV);
  WRITE_REG_U32(priv->base + FC7300_FCUART_STAT_OFFSET, val);

  /* Flush the transmit and receive FIFO */

  SET_REG_BITS(priv->base + FC7300_FCUART_FIFO_OFFSET,
               FCUART_FIFO_RXFLUSH | FCUART_FIFO_TXFLUSH);

  priv->frame_state = FC7300_LIN_FRAME_IDLE;
  priv->reporting = false;
  FC7300_LIN_SETNORMAL(priv);
}

/****************************************************************************
 * Function: fc7300_lin_breakdetect
 *
 * Description:
 *   Enable or disable the break detect and corresponding interrupt
 *
 * Input Parameters:
 *   priv    - Reference to the private data of fc7300 lin driver
 *   benable - true: enable, false: disable
 *
 * Returned Value:
 *   None
 *
 * Assumptions:
 *
 ****************************************************************************/

static inline void fc7300_lin_breakdetect(struct fc7300_lindev_s *priv,
                                   bool benable)
{
  if (benable)
    {
      SET_REG_BITS(priv->base + FC7300_FCUART_BAUD_OFFSET,
                   FCUART_BAUD_LBKDIE);
      SET_REG_BITS(priv->base + FC7300_FCUART_STAT_OFFSET,
                   FCUART_STAT_LBKDE);
    }
  else
    {
      CLEAR_REG_BITS(priv->base + FC7300_FCUART_BAUD_OFFSET,
                     FCUART_BAUD_LBKDIE);
      CLEAR_REG_BITS(priv->base + FC7300_FCUART_STAT_OFFSET,
                     FCUART_STAT_LBKDE);
    }
}

/****************************************************************************
 * Function: fc7300_lin_idlelinedetect
 *
 * Description:
 *   Enable the idle detect and corresponding interrupt
 *
 * Input Parameters:
 *   priv    - Reference to the private data of fc7300 lin driver
 *
 * Returned Value:
 *   None
 *
 * Assumptions:
 *
 ****************************************************************************/

static inline void fc7300_lin_idlelinedetect(struct fc7300_lindev_s *priv)
{
  uint32_t val;

  /* Write 1 clear idle interrupt flag */

  SET_REG_BITS(priv->base + FC7300_FCUART_STAT_OFFSET,
                FCUART_STAT_IDLEF);

  /* Enable idle interrupt */

  val = READ_REG_U32(priv->base + FC7300_FCUART_CTRL_OFFSET);
  val = (val & (~(FCUART_CTRL_IDLECFG_MASK))) |
        FCUART_CTRL_IDLECFG_16 | FCUART_CTRL_IIE;
  WRITE_REG_U32(priv->base + FC7300_FCUART_CTRL_OFFSET, val);
}

/****************************************************************************
 * Function: fc7300_lin_setrxwatermark
 *
 * Description:
 *   Set receive FIFO water mark.
 *
 * Input Parameters:
 *   priv    - Reference to the private data of fc7300 lin driver
 *   watermark - receive FIFO water mark
 *
 * Returned Value:
 *   None
 *
 * Assumptions:
 *
 ****************************************************************************/

static inline void fc7300_lin_setrxwatermark(struct fc7300_lindev_s *priv,
                                             uint8_t watermark)
{
  uint32_t val;

  /* Set receive water mark */

  val = READ_REG_U32(priv->base + FC7300_FCUART_WATERMARK_OFFSET);
  val = (val & (~FCUART_WATERMARK_RXWATER_MASK)) |
      (FCUART_WATERMARK_RXWATER(watermark) & FCUART_WATERMARK_RXWATER_MASK);
  WRITE_REG_U32(priv->base + FC7300_FCUART_WATERMARK_OFFSET, val);
}

/****************************************************************************
 * Function: fc7300_lin_filltxfifo
 *
 * Description:
 *   Fill LIN frame data to transmit FIFO.
 *
 * Input Parameters:
 *   priv    - Reference to the private data of fc7300 lin driver
 *   data    - The data to transmit
 *   len     - The length of data
 *
 * Returned Value:
 *   None.
 *
 * Assumptions:
 *
 ****************************************************************************/

static inline void fc7300_lin_filltxfifo(struct fc7300_lindev_s *priv,
                                        uint8_t *data, uint8_t len)
{
  uint8_t i;

  for (i = 0u; i < len; ++i)
    {
      WRITE_REG_U32(priv->base + FC7300_FCUART_DATA_OFFSET, data[i]);
    }
}

/****************************************************************************
 * Function: fc7300_lin_readrxfifo
 *
 * Description:
 *   Read LIN data frome the receive FIFO.
 *
 * Input Parameters:
 *   priv    - Reference to the private data of fc7300 lin driver
 *   buf     - The data buffer to save the received data
 *
 * Returned Value:
 *   Return the number of bytes received to data buffer.
 *
 * Assumptions:
 *
 ****************************************************************************/

static inline uint8_t fc7300_lin_readrxfifo(struct fc7300_lindev_s *priv,
                                            uint8_t *buf)
{
  uint8_t i;
  uint8_t count;

  count = READ_REG_SHIFT(priv->base + FC7300_FCUART_WATERMARK_OFFSET,
          FCUART_WATERMARK_RXCOUNT_MASK, FCUART_WATERMARK_RXCOUNT_SHIFT);

  for (i = 0u; i < count; ++i)
    {
      buf[i] = READ_REG_U32(priv->base + FC7300_FCUART_DATA_OFFSET) & 0xffu;
    }

  return count;
}

/****************************************************************************
 * Function: fc7300_lin_softreset
 *
 * Description:
 *   Reset the LIN controller
 *
 * Input Parameters:
 *   priv - Reference to the private data of fc7300 lin driver
 *
 * Returned Value:
 *   None
 *
 * Assumptions:
 *
 ****************************************************************************/

static inline void fc7300_lin_softreset(struct fc7300_lindev_s *priv)
{
  SET_REG_BITS(priv->base + FC7300_FCUART_RST_OFFSET, FCUART_RST_RST);
  CLEAR_REG_BITS(priv->base + FC7300_FCUART_RST_OFFSET, FCUART_RST_RST);
}

/****************************************************************************
 * Function: fc7300_lin_irqattach
 *
 * Description:
 *   Attach the interrupt handler, and enable the interrupt
 *
 * Input Parameters:
 *   priv - Reference to the private data of fc7300 lin driver
 *
 * Returned Value:
 *   OK(0) on success; Negated errno on failure.
 *
 * Assumptions:
 *
 ****************************************************************************/

static int fc7300_lin_irqattach(struct fc7300_lindev_s *priv)
{
  /* Attach the interrupt handler */

#ifdef CONFIG_FC7300_LIN_ISR_THREAD
  if (irq_attach_wqueue(priv->cfg->irq_num, fc7300_lin_isr,
                        fc7300_lin_interrupt, (void *)priv,
                        isr_cfg[LIN_ISR_THREAD]))
#else
  if (irq_attach(priv->cfg->irq_num, fc7300_lin_interrupt, (void *)priv))
#endif
    {
      return -EAGAIN;
    }

#ifdef CONFIG_ARCH_IRQPRIO
  up_prioritize_irq(priv->cfg->irq_num, priv->cfg->irq_priority);
#endif
  up_enable_irq(priv->cfg->irq_num);
  return OK;
}

/****************************************************************************
 * Function: fc7300_lin_irqdetach
 *
 * Description:
 *   Detach the interrupt handler, and disable the interrupt
 *
 * Input Parameters:
 *   priv - Reference to the private data of fc7300 lin driver
 *
 * Returned Value:
 *   OK(0) on success; Negated errno on failure.
 *
 * Assumptions:
 *
 ****************************************************************************/

static int fc7300_lin_irqdetach(struct fc7300_lindev_s *priv)
{
  int ret;

  up_disable_irq(priv->cfg->irq_num);

#ifdef CONFIG_FC7300_LIN_ISR_THREAD
  ret = irq_detach_wqueue(priv->cfg->irq_num);
#else
  ret = irq_detach(priv->cfg->irq_num);
#endif

  return ret;
}

/****************************************************************************
 * Function: fc7300_lin_calculate_baudrate
 *
 * Description:
 *   Calculate the over sampling ratio and modulo divisor based on the baud
 *   rate.
 *
 * Input Parameters:
 *   priv - Reference to the private data of fc4150 lin driver
 *   oversamp - The oversampling ratio for the receiver.
 *   sbr - The Baud Rate Modulo Divisor.
 *
 * Returned Value:
 *   OK(0) on success; Negated errno on failure.
 *
 * Assumptions:
 *
 ****************************************************************************/

static int fc7300_lin_calculate_baudrate(struct fc7300_lindev_s *priv,
                                    uint8_t *oversamp, uint16_t *sbr)
{
  uint32_t clkfreq;
  uint32_t ovsamp;
  uint32_t divisor;
  uint32_t temp;
  uint32_t diff;
  uint32_t mindiff = 0xffffffffu;

  clkfreq = priv->srcclk;

  /* Find the best oversample and divisor for target baudrate */

  for (ovsamp = 16u; ovsamp <= 32u; ++ovsamp)
    {
      temp = priv->baudrate * ovsamp;
      divisor = (clkfreq + (temp >> 1u)) / temp;
      temp = temp * divisor;
      if (temp == clkfreq)
        {
          mindiff = 0u;
          *oversamp = ovsamp;
          *sbr = divisor;
          break;
        }
      else if (temp > clkfreq)
        {
          diff = temp - clkfreq;
        }
      else
        {
          diff = clkfreq - temp;
        }

      if (diff < mindiff)
        {
          mindiff = diff;
          *oversamp = ovsamp;
          *sbr = divisor;
        }
    }

  /* The sbr must not exceed 8192 */

  if (*sbr >= 8192u)
    {
      return -EINVAL;
    }

  return OK;
}

#ifdef CONFIG_FC7300_LIN_BREAK_SOFTWARE
/****************************************************************************
 * Function: fc7300_lin_sendbreakbybaud
 *
 * Description:
 *   Change the over sampling ratio and modulo divisor to send break field
 *   by software.
 *
 * Input Parameters:
 *   priv - Reference to the private data of fc7300 lin driver
 *
 * Returned Value:
 *   None.
 *
 * Assumptions:
 *
 ****************************************************************************/

static void fc7300_lin_sendbreakbybaud(struct fc7300_lindev_s *priv)
{
  /* Flush the transmit and receive FIFO */

  SET_REG_BITS(priv->base + FC7300_FCUART_FIFO_OFFSET,
               FCUART_FIFO_RXFLUSH | FCUART_FIFO_TXFLUSH);

  fc7300_lin_change_baudrate(priv);
  fc7300_lin_setrxwatermark(priv, 0u);
  fc7300_lin_breakdetect(priv, false);

  WRITE_REG_U32(priv->base + FC7300_FCUART_DATA_OFFSET, 0u);
}

/****************************************************************************
 * Function: fc7300_lin_breakeventbybaud
 *
 * Description:
 *   Handle the break event sent by change baudrate.
 *
 * Input Parameters:
 *   priv - Reference to the private data of fc7300 lin driver
 *
 * Returned Value:
 *   None.
 *
 * Assumptions:
 *
 ****************************************************************************/

static void fc7300_lin_breakeventbybaud(struct fc7300_lindev_s *priv)
{
  SET_REG_BITS(priv->base + FC7300_FCUART_FIFO_OFFSET,
               FCUART_FIFO_RXFLUSH);
  fc7300_lin_recover_baudrate(priv);

  /* Set watermark to 1, then send sync and pid */

  fc7300_lin_setrxwatermark(priv, 1u);
  priv->frame_state = FC7300_LIN_FRAME_RECV_HEADER;
  fc7300_lin_filltxfifo(priv, FC7300_LIN_BUF_PSYNC(priv), 2u);
}

/****************************************************************************
 * Function: fc7300_lin_change_baudrate
 *
 * Description:
 *   Change the over sampling ratio and modulo divisor to send break field
 *   by software.
 *
 * Input Parameters:
 *   priv - Reference to the private data of fc7300 lin driver
 *
 * Returned Value:
 *   None.
 *
 * Assumptions:
 *
 ****************************************************************************/

static void fc7300_lin_change_baudrate(struct fc7300_lindev_s *priv)
{
  uint32_t val;

  val = READ_REG_U32(priv->base + FC7300_FCUART_BAUD_OFFSET);
  val = (val & (~FCUART_BAUD_SBR_MASK)) |
        FCUART_BAUD_SBR(priv->sbr_brk);
  WRITE_REG_U32(priv->base + FC7300_FCUART_BAUD_OFFSET, val);

  for (val = 0u; val < priv->waitcycles; ++val)
    {
      __asm__ __volatile__ ("nop\n");
    }
}

/****************************************************************************
 * Function: fc7300_lin_recover_baudrate
 *
 * Description:
 *   Reover the over sampling ratio and modulo divisor based on the baud
 *   rate.
 *
 * Input Parameters:
 *   priv - Reference to the private data of fc7300 lin driver
 *
 * Returned Value:
 *   None.
 *
 * Assumptions:
 *
 ****************************************************************************/

static void fc7300_lin_recover_baudrate(struct fc7300_lindev_s *priv)
{
  uint32_t val;

  val = READ_REG_U32(priv->base + FC7300_FCUART_BAUD_OFFSET);
  val = (val & (~FCUART_BAUD_SBR_MASK)) |
        FCUART_BAUD_SBR(priv->sbr);
  WRITE_REG_U32(priv->base + FC7300_FCUART_BAUD_OFFSET, val);

  for (val = 0u; val <= priv->waitcycles; ++val)
    {
      __asm__ __volatile__ ("nop\n");
    }
}
#endif /* CONFIG_FC7300_LIN_BREAK_SOFTWARE */

/****************************************************************************
 * Function: fc7300_lin_calculatetimeout
 *
 * Description:
 *   Calculate the timeout value for communication based on the baud rate
 *
 * Input Parameters:
 *   priv - Reference to the private data of fc7300 lin driver
 *
 * Returned Value:
 *   None.
 *
 * Assumptions:
 *
 ****************************************************************************/

static void fc7300_lin_calculatetimeout(struct fc7300_lindev_s *priv)
{
  uint32_t usec;

  usec = USEC_PER_SEC * LIN_MAX_BITS_PER_FRAME / priv->baudrate;
  priv->timeout = (uint16_t)USEC2TICK(usec);
}

/****************************************************************************
 * Function: fc7300_lin_controller_init
 *
 * Description:
 *   configure the fc7300 lin controller registers
 *
 * Input Parameters:
 *   priv - Reference to the private data of fc7300 lin driver
 *
 * Returned Value:
 *   OK(0) on success; Negated errno on failure.
 *
 * Assumptions:
 *
 ****************************************************************************/

static int fc7300_lin_controller_init(struct fc7300_lindev_s *priv)
{
  int ret;
  uint32_t regval;

  priv->sbr = 125u;
  priv->ovsmp = 10u;

  /* Calculate the oversampling ratio and baud rate modulo divisor */

  ret = fc7300_lin_calculate_baudrate(priv, &priv->ovsmp, &priv->sbr);
  if (ret != OK)
    {
      return ret;
    }

#ifdef CONFIG_FC7300_LIN_BREAK_SOFTWARE
  priv->waitcycles = fc7300_get_coreclk() / priv->srcclk;
  priv->sbr_brk = (priv->sbr * 13u + 4) / 9u;
  if (priv->sbr_brk >= 8192u)
    {
      return -EINVAL;
    }
#endif

  /* Disable receiver and transmitter */

  CLEAR_REG_BITS(priv->base + FC7300_FCUART_CTRL_OFFSET,
                 FCUART_CTRL_RE | FCUART_CTRL_TE);

  /* Set baudrate divisor and oversample
   * enable rx input active edge interrupt
   * disable match mode
   * disable 10bit mode
   * disable TX DMA and RX DMA
   * disable break detect interrupt
   * one stop bit
   */

  regval = FCUART_BAUD_RIAEIE | FCUART_BAUD_SBR(priv->sbr) |
           ((priv->ovsmp <= 6u) ? FCUART_BAUD_BEDGE_SAMP : 0u) |
           FCUART_BAUD_OVR_SAMP(priv->ovsmp - 1u);
  WRITE_REG_U32(priv->base + FC7300_FCUART_BAUD_OFFSET, regval);

  /* Set fifo configuration, flush buffer and clear FIFO flag */

  regval = READ_REG_U32(priv->base + FC7300_FCUART_FIFO_OFFSET);
  regval &= ~(FCUART_FIFO_RXUFIE | FCUART_FIFO_TXOFIE |
              FCUART_FIFO_RXIDEN_MASK);
  regval |= FCUART_FIFO_RXFEN | FCUART_FIFO_TXFEN |
            FCUART_FIFO_RXUF | FCUART_FIFO_TXOF |
            FCUART_FIFO_RXFLUSH | FCUART_FIFO_TXFLUSH;
  WRITE_REG_U32(priv->base + FC7300_FCUART_FIFO_OFFSET, regval);

  /* Set TX RX watermark to 0 */

  WRITE_REG_U32(priv->base + FC7300_FCUART_WATERMARK_OFFSET, 0u);

  /* Set control register
   * disable idle line(IIE),frame error(FEIE), receive(RIE) interrupts
   * disable transmit(TIE) and transmit complete(TCIE) interrupts
   * disable parity error(PEIE) and noise error(NEIE) interrupts
   * disable Overrun Interrupt(ORIE) interrupt
   * disable match address
   * 8 bits data mode
   * disable parity and parity check
   */

  regval = (FCUART_CTRL_IDLECFG_16 & FCUART_CTRL_IDLECFG_MASK) |
           FCUART_CTRL_ITS;
  WRITE_REG_U32(priv->base + FC7300_FCUART_CTRL_OFFSET, regval);

  /* Set stat register, clear status flag
   * set break length 13bits according to reference manual :
   * BCGL = 1, BMSEL = 0, 10BIT_MODE = 0, 7BMS = 0, SBNS = 0
   */

  regval = READ_REG_U32(priv->base + FC7300_FCUART_STAT_OFFSET);
  regval &= ~(FCUART_STAT_LBKDE | FCUART_STAT_RWUID |
              FCUART_STAT_RXINV | FCUART_STAT_MSBF);
  regval |= FCUART_STAT_BCGL_MASK;
  WRITE_REG_U32(priv->base + FC7300_FCUART_STAT_OFFSET, regval);

  /* Enable receiver and transmitter */

  SET_REG_BITS(priv->base + FC7300_FCUART_CTRL_OFFSET,
               FCUART_CTRL_RE | FCUART_CTRL_TE);

  return OK;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Function: fc7300_lininitialize
 *
 * Description:
 *   Initialize the LIN controller and driver
 *
 * Input Parameters:
 *   intf - In the case where there are multiple LIN devices, this value
 *          identifies which LIN device is to be initialized.
 *
 * Returned Value:
 *   OK(0) on success; Negated errno on failure.
 *
 * Assumptions:
 *
 ****************************************************************************/

int fc7300_lininitialize(int intf)
{
  struct fc7300_lindev_s *priv;

  /* Setup and return the driver private structure of the LIN instance */

  priv = fc7300_lin_privsetup(intf);
  if (priv == NULL)
    {
      nerr("%d:ERROR: privsetup intf = %d", __LINE__, intf);
      return -ENODEV;
    }

  /* Initialize the spin lock */

  spin_lock_init(&priv->lock);

  /* Initialize the IO PINs */

  fc7300_lin_pinconfig(priv);

  FC7300_LIN_SETIFDOWN(priv);
  snprintf(priv->dev.netdev.d_ifname, IFNAMSIZ, "lin%d", intf);
  priv->dev.netdev.d_pktsize = CAN_MTU;
  priv->dev.quota[NETPKT_TX] = 1u;
  priv->dev.quota[NETPKT_RX] = 1u;
  priv->dev.ops = &g_fc7300linops;

  /* Register the device with the OS so that socket APIs can be performed */

  return netdev_lower_register(&priv->dev, NET_LL_CAN);
}

#endif /* CONFIG_FC7300_LIN */
