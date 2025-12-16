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

#include <byteswap.h>
#include <inttypes.h>

#include <nuttx/irq.h>
#include <nuttx/arch.h>
#include <nuttx/can/can.h>
#include <nuttx/config.h>
#include <nuttx/spinlock.h>

#ifdef CONFIG_CAN_TIMESTAMP
#include <time.h>
#endif

#include "arm_internal.h"
#include "chip.h"
#include "fc7300_periphclocks.h"
#include "hardware/fc7300_pin.h"
#include "hardware/fc7300_pinmux.h"
#include "hardware/fc7300_flexcan.h"
#include "fc7300_flexcan.h"
#include "fc7300_periphclocks.h"
#include "fc7300_clockconfig.h"
#include "fc7300_irq.h"
#include <arch/board/board.h>

#ifdef CONFIG_FC7300_FLEXCAN_CHARDRIVER

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#define MASKSTDID                   0x000007ff
#define MASKEXTID                   0x1fffffff

#define IFLAG1_RX                   (((1 << priv->rxmb_cnt) - 1) << priv->txmb_cnt)
#define IFLAG1_TX                   ((1 << priv->txmb_cnt) - 1)

/****************************************************************************
 * Public Data
 ****************************************************************************/

const static uint8_t g_fc7300_can_dlc_to_len[16] =
{
  0,
  1,
  2,
  3,
  4,
  5,
  6,
  7,
  8,
  12,
  16,
  20,
  24,
  32,
  48,
  64,
};

uint8_t g_charcan_timeoutflg;

/****************************************************************************
 * Private Types
 ****************************************************************************/

struct fc7300_flexcan_priv_s
{
  uint32_t base;                /* FLEXCAN base address */
  enum pcc_clksrctype_e clkname;
  uint32_t *rx;
  uint32_t *tx;
  unsigned long state;

  struct flexcan_timeseg arbi_timing; /* Timing for arbitration phase */
#ifdef CONFIG_CAN_FD
  bool canfd_en;                      /* true:use CAN FD, false:use CAN 2.0B */
  uint8_t data_size;                  /* data size when CAN FD */
#  ifdef CONFIG_FC7300_FLEXCAN_CANFD_SSP
  bool ssp_en;
  uint8_t ssp_offset;
#  endif
#  ifdef CONFIG_FC7300_FLEXCAN_CANFD_BRS
  bool canfdbrs_en;
  struct flexcan_timeseg data_timing; /* Timing for data phase */
#  endif
#endif
  const struct flexcan_config_s *config;

  uint8_t rxmb_cnt;
  uint8_t txmb_cnt;
  uint32_t txmb_sflags;

#ifdef CONFIG_FC7300_FLEXCAN_ENHANCED_RX_FIFO
  bool erf_en;
  uint32_t erfel_total_cnt;
  uint32_t erfel_extid_cnt;
  const struct can_erf_filter *erf_filter;     /* Enhanced Rx Fifo Filter Structre Pointer */
#endif

  const struct can_filter *rxmb_filter;        /* RX MB Filter Structure Pointer */
};

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/* Helper functions */

static int fc7300_flexcan_init_eccram(struct fc7300_flexcan_priv_s *priv);
static int fc7300_flexcan_initialize(struct fc7300_flexcan_priv_s *priv);
static void fc7300_flexcan_reset(struct fc7300_flexcan_priv_s *priv);
#if defined(CONFIG_CAN_ERRORS)
static void fc7300_flexcan_geterrinfo(uint32_t base,
                                      struct flexcan_errinfo *info);
#endif

/* CAN driver methods */

static void fc7300_flexcan_co_reset(struct can_dev_s *dev);
static int  fc7300_flexcan_co_setup(struct can_dev_s *dev);
static void fc7300_flexcan_co_shutdown(struct can_dev_s *dev);
static void fc7300_flexcan_co_rxint(struct can_dev_s *dev, bool enable);
static void fc7300_flexcan_co_txint(struct can_dev_s *dev, bool enable);
static int  fc7300_flexcan_co_ioctl(struct can_dev_s *dev, int cmd,
                                    unsigned long arg);
static int  fc7300_flexcan_co_remoterequest(struct can_dev_s *dev,
                                            uint16_t id);
static int  fc7300_flexcan_co_send(struct can_dev_s *dev,
                                   struct can_msg_s *msg);
static bool fc7300_flexcan_co_txready(struct can_dev_s *dev);
static bool fc7300_flexcan_co_txempty(struct can_dev_s *dev);
static bool fc7300_flexcan_co_cancel(struct can_dev_s *dev,
                                     struct can_msg_s *msg);
static void fc7300_flexcan_co_errhandle(struct can_dev_s *dev);

#if defined(CONFIG_CAN_ERRORS)
static void fc7300_flexcan_handle_error(struct can_dev_s *dev,
                                        struct flexcan_errinfo *info,
                                        enum flexcan_errmode mode);
#endif

#ifdef CONFIG_CAN_TXCONFIRM
static uint32_t fc7300_flexcan_tx_confirm(struct can_dev_s *dev,
                                          int mb_index);
#endif

static int fc7300_flexcan_clear_txmb(struct fc7300_flexcan_priv_s *priv);

#ifdef CONFIG_FC7300_FLEXCAN0_CHARDRIVER
extern struct flexcan_config_s g_fc7300_flexcan0_config;
#endif
#ifdef CONFIG_FC7300_FLEXCAN1_CHARDRIVER
extern struct flexcan_config_s g_fc7300_flexcan1_config;
#endif
#ifdef CONFIG_FC7300_FLEXCAN2_CHARDRIVER
extern struct flexcan_config_s g_fc7300_flexcan2_config;
#endif
#ifdef CONFIG_FC7300_FLEXCAN3_CHARDRIVER
extern struct flexcan_config_s g_fc7300_flexcan3_config;
#endif
#ifdef CONFIG_FC7300_FLEXCAN4_CHARDRIVER
extern struct flexcan_config_s g_fc7300_flexcan4_config;
#endif
#ifdef CONFIG_FC7300_FLEXCAN5_CHARDRIVER
extern struct flexcan_config_s g_fc7300_flexcan5_config;
#endif
#ifdef CONFIG_FC7300_FLEXCAN6_CHARDRIVER
extern struct flexcan_config_s g_fc7300_flexcan6_config;
#endif
#ifdef CONFIG_FC7300_FLEXCAN7_CHARDRIVER
extern struct flexcan_config_s g_fc7300_flexcan7_config;
#endif
#ifdef CONFIG_FC7300_FLEXCAN8_CHARDRIVER
extern struct flexcan_config_s g_fc7300_flexcan8_config;
#endif
#ifdef CONFIG_FC7300_FLEXCAN9_CHARDRIVER
extern struct flexcan_config_s g_fc7300_flexcan9_config;
#endif

#ifdef CONFIG_FC7300_FLEXCAN0_CHARDRIVER
locate_code(".dtcm_data") struct fc7300_flexcan_priv_s g_fc7300_flexcan0priv =
{
  .base        = FC7300_FLEXCAN0_BASE,
  .clkname     = PCC_CLK_FLEXCAN0,
#ifdef CONFIG_CAN_FD
#  ifdef CONFIG_FC7300_FLEXCAN0_CANFD
  .canfd_en    = true,
  .data_size   = FLEXCAN0_CANFD_DATA_SIZE,
#    ifdef CONFIG_FC7300_FLEXCAN0_CANFD_SSP
  .ssp_en      = true,
  .ssp_offset  = CONFIG_FC7300_FLEXCAN0_CANFD_SSP_OFFSET,
#    endif
#    ifdef CONFIG_FC7300_FLEXCAN_CANFD_BRS
#      ifdef CONFIG_FC7300_FLEXCAN0_CANFD_BRS
  .canfdbrs_en = true,
  .data_timing =
    {
      .presdiv = CONFIG_FC7300_FLEXCAN0_CANFD_DATA_PRESDIV,
      .propseg = CONFIG_FC7300_FLEXCAN0_CANFD_DATA_PROPSEG,
      .pseg1   = CONFIG_FC7300_FLEXCAN0_CANFD_DATA_PSEG1,
      .pseg2   = CONFIG_FC7300_FLEXCAN0_CANFD_DATA_PSEG2,
      .sjw     = CONFIG_FC7300_FLEXCAN0_CANFD_DATA_SJW,
    },
#      else
  .canfdbrs_en = false,
#      endif
#    endif
#  else
  .canfd_en = false,
#  endif
#endif

  .arbi_timing =
    {
      .presdiv = CONFIG_FC7300_FLEXCAN0_PRESDIV,
      .propseg = CONFIG_FC7300_FLEXCAN0_PROPSEG,
      .pseg1   = CONFIG_FC7300_FLEXCAN0_PSEG1,
      .pseg2   = CONFIG_FC7300_FLEXCAN0_PSEG2,
      .sjw     = CONFIG_FC7300_FLEXCAN0_SJW,
    },

  .rxmb_cnt    = FLEXCAN0_RXMB_COUNT,
  .txmb_cnt    = FLEXCAN0_TXMB_COUNT,
  .config      = &g_fc7300_flexcan0_config,
#ifdef CONFIG_FC7300_FLEXCAN_ENHANCED_RX_FIFO
  .erf_filter  = NULL,
#  ifdef CONFIG_FC7300_FLEXCAN0_ENHANCED_RX_FIFO
  .erf_en      = true,
  .erfel_total_cnt = CONFIG_FC7300_FLEXCAN0_ERFEL_TOTAL_COUNT,
#    ifdef CONFIG_CAN_EXTID
  .erfel_extid_cnt = CONFIG_FC7300_FLEXCAN0_ERFEL_EXTID_COUNT,
#    else
  .erfel_extid_cnt = 0,
#    endif
#  else
  .erf_en          = false,
  .erfel_total_cnt = 0,
  .erfel_extid_cnt = 0,
#  endif
#endif

  .rxmb_filter = NULL,
};
#endif

#ifdef CONFIG_FC7300_FLEXCAN1_CHARDRIVER
locate_code(".dtcm_data") struct fc7300_flexcan_priv_s g_fc7300_flexcan1priv =
{
  .base        = FC7300_FLEXCAN1_BASE,
  .clkname     = PCC_CLK_FLEXCAN1,
#ifdef CONFIG_CAN_FD
#  ifdef CONFIG_FC7300_FLEXCAN1_CANFD
  .canfd_en    = true,
  .data_size   = FLEXCAN1_CANFD_DATA_SIZE,
#    ifdef CONFIG_FC7300_FLEXCAN1_CANFD_SSP
  .ssp_en      = true,
  .ssp_offset  = CONFIG_FC7300_FLEXCAN1_CANFD_SSP_OFFSET,
#    endif
#    ifdef CONFIG_FC7300_FLEXCAN_CANFD_BRS
#      ifdef CONFIG_FC7300_FLEXCAN1_CANFD_BRS
  .canfdbrs_en = true,
  .data_timing =
    {
      .presdiv = CONFIG_FC7300_FLEXCAN1_CANFD_DATA_PRESDIV,
      .propseg = CONFIG_FC7300_FLEXCAN1_CANFD_DATA_PROPSEG,
      .pseg1   = CONFIG_FC7300_FLEXCAN1_CANFD_DATA_PSEG1,
      .pseg2   = CONFIG_FC7300_FLEXCAN1_CANFD_DATA_PSEG2,
      .sjw     = CONFIG_FC7300_FLEXCAN1_CANFD_DATA_SJW,
    },
#      else
  .canfdbrs_en = false,
#      endif
#    endif
#  else
  .canfd_en = false,
#  endif
#endif

  .arbi_timing =
    {
      .presdiv = CONFIG_FC7300_FLEXCAN1_PRESDIV,
      .propseg = CONFIG_FC7300_FLEXCAN1_PROPSEG,
      .pseg1   = CONFIG_FC7300_FLEXCAN1_PSEG1,
      .pseg2   = CONFIG_FC7300_FLEXCAN1_PSEG2,
      .sjw     = CONFIG_FC7300_FLEXCAN1_SJW,
    },

  .rxmb_cnt    = FLEXCAN1_RXMB_COUNT,
  .txmb_cnt    = FLEXCAN1_TXMB_COUNT,
  .config      = &g_fc7300_flexcan1_config,
#ifdef CONFIG_FC7300_FLEXCAN_ENHANCED_RX_FIFO
  .erf_filter  = NULL,
#  ifdef CONFIG_FC7300_FLEXCAN1_ENHANCED_RX_FIFO
  .erf_en      = true,
  .erfel_total_cnt = CONFIG_FC7300_FLEXCAN1_ERFEL_TOTAL_COUNT,
#    ifdef CONFIG_CAN_EXTID
  .erfel_extid_cnt = CONFIG_FC7300_FLEXCAN1_ERFEL_EXTID_COUNT,
#    else
  .erfel_extid_cnt = 0,
#    endif
#  else
  .erf_en          = false,
  .erfel_total_cnt = 0,
  .erfel_extid_cnt = 0,
#  endif
#endif

  .rxmb_filter = NULL,
};
#endif

#ifdef CONFIG_FC7300_FLEXCAN2_CHARDRIVER
locate_code(".dtcm_data") struct fc7300_flexcan_priv_s g_fc7300_flexcan2priv =
{
  .base        = FC7300_FLEXCAN2_BASE,
  .clkname     = PCC_CLK_FLEXCAN2,
#ifdef CONFIG_CAN_FD
#  ifdef CONFIG_FC7300_FLEXCAN2_CANFD
  .canfd_en    = true,
  .data_size   = FLEXCAN2_CANFD_DATA_SIZE,
#    ifdef CONFIG_FC7300_FLEXCAN2_CANFD_SSP
  .ssp_en      = true,
  .ssp_offset  = CONFIG_FC7300_FLEXCAN2_CANFD_SSP_OFFSET,
#    endif
#    ifdef CONFIG_FC7300_FLEXCAN_CANFD_BRS
#      ifdef CONFIG_FC7300_FLEXCAN2_CANFD_BRS
  .canfdbrs_en = true,
  .data_timing =
    {
      .presdiv = CONFIG_FC7300_FLEXCAN2_CANFD_DATA_PRESDIV,
      .propseg = CONFIG_FC7300_FLEXCAN2_CANFD_DATA_PROPSEG,
      .pseg1   = CONFIG_FC7300_FLEXCAN2_CANFD_DATA_PSEG1,
      .pseg2   = CONFIG_FC7300_FLEXCAN2_CANFD_DATA_PSEG2,
      .sjw     = CONFIG_FC7300_FLEXCAN2_CANFD_DATA_SJW,
    },
#      else
  .canfdbrs_en = false,
#      endif
#    endif
#  else
  .canfd_en = false,
#  endif
#endif

  .arbi_timing =
    {
      .presdiv = CONFIG_FC7300_FLEXCAN2_PRESDIV,
      .propseg = CONFIG_FC7300_FLEXCAN2_PROPSEG,
      .pseg1   = CONFIG_FC7300_FLEXCAN2_PSEG1,
      .pseg2   = CONFIG_FC7300_FLEXCAN2_PSEG2,
      .sjw     = CONFIG_FC7300_FLEXCAN2_SJW,
    },

  .rxmb_cnt    = FLEXCAN2_RXMB_COUNT,
  .txmb_cnt    = FLEXCAN2_TXMB_COUNT,
  .config      = &g_fc7300_flexcan2_config,
#ifdef CONFIG_FC7300_FLEXCAN_ENHANCED_RX_FIFO
  .erf_filter  = NULL,
#  ifdef CONFIG_FC7300_FLEXCAN2_ENHANCED_RX_FIFO
  .erf_en      = true,
  .erfel_total_cnt = CONFIG_FC7300_FLEXCAN2_ERFEL_TOTAL_COUNT,
#    ifdef CONFIG_CAN_EXTID
  .erfel_extid_cnt = CONFIG_FC7300_FLEXCAN2_ERFEL_EXTID_COUNT,
#    else
  .erfel_extid_cnt = 0,
#    endif
#  else
  .erf_en          = false,
  .erfel_total_cnt = 0,
  .erfel_extid_cnt = 0,
#  endif
#endif

  .rxmb_filter = NULL,
};
#endif

#ifdef CONFIG_FC7300_FLEXCAN3_CHARDRIVER
locate_code(".dtcm_data") struct fc7300_flexcan_priv_s g_fc7300_flexcan3priv =
{
  .base        = FC7300_FLEXCAN3_BASE,
  .clkname     = PCC_CLK_FLEXCAN3,
#ifdef CONFIG_CAN_FD
#  ifdef CONFIG_FC7300_FLEXCAN3_CANFD
  .canfd_en    = true,
  .data_size   = FLEXCAN3_CANFD_DATA_SIZE,
#    ifdef CONFIG_FC7300_FLEXCAN3_CANFD_SSP
  .ssp_en      = true,
  .ssp_offset  = CONFIG_FC7300_FLEXCAN3_CANFD_SSP_OFFSET,
#    endif
#    ifdef CONFIG_FC7300_FLEXCAN_CANFD_BRS
#      ifdef CONFIG_FC7300_FLEXCAN3_CANFD_BRS
  .canfdbrs_en = true,
  .data_timing =
    {
      .presdiv = CONFIG_FC7300_FLEXCAN3_CANFD_DATA_PRESDIV,
      .propseg = CONFIG_FC7300_FLEXCAN3_CANFD_DATA_PROPSEG,
      .pseg1   = CONFIG_FC7300_FLEXCAN3_CANFD_DATA_PSEG1,
      .pseg2   = CONFIG_FC7300_FLEXCAN3_CANFD_DATA_PSEG2,
      .sjw     = CONFIG_FC7300_FLEXCAN3_CANFD_DATA_SJW,
    },
#      else
  .canfdbrs_en = false,
#      endif
#    endif
#  else
  .canfd_en = false,
#  endif
#endif

  .arbi_timing =
    {
      .presdiv = CONFIG_FC7300_FLEXCAN3_PRESDIV,
      .propseg = CONFIG_FC7300_FLEXCAN3_PROPSEG,
      .pseg1   = CONFIG_FC7300_FLEXCAN3_PSEG1,
      .pseg2   = CONFIG_FC7300_FLEXCAN3_PSEG2,
      .sjw     = CONFIG_FC7300_FLEXCAN3_SJW,
    },

  .rxmb_cnt    = FLEXCAN3_RXMB_COUNT,
  .txmb_cnt    = FLEXCAN3_TXMB_COUNT,
  .config      = &g_fc7300_flexcan3_config,
#ifdef CONFIG_FC7300_FLEXCAN_ENHANCED_RX_FIFO
  .erf_filter  = NULL,
#  ifdef CONFIG_FC7300_FLEXCAN3_ENHANCED_RX_FIFO
  .erf_en      = true,
  .erfel_total_cnt = CONFIG_FC7300_FLEXCAN3_ERFEL_TOTAL_COUNT,
#    ifdef CONFIG_CAN_EXTID
  .erfel_extid_cnt = CONFIG_FC7300_FLEXCAN3_ERFEL_EXTID_COUNT,
#    else
  .erfel_extid_cnt = 0,
#    endif
#  else
  .erf_en          = false,
  .erfel_total_cnt = 0,
  .erfel_extid_cnt = 0,
#  endif
#endif

  .rxmb_filter = NULL,
};
#endif

#ifdef CONFIG_FC7300_FLEXCAN4_CHARDRIVER
locate_code(".dtcm_data") struct fc7300_flexcan_priv_s g_fc7300_flexcan4priv =
{
  .base        = FC7300_FLEXCAN4_BASE,
  .clkname     = PCC_CLK_FLEXCAN4,
#ifdef CONFIG_CAN_FD
#  ifdef CONFIG_FC7300_FLEXCAN4_CANFD
  .canfd_en    = true,
  .data_size   = FLEXCAN4_CANFD_DATA_SIZE,
#    ifdef CONFIG_FC7300_FLEXCAN4_CANFD_SSP
  .ssp_en      = true,
  .ssp_offset  = CONFIG_FC7300_FLEXCAN4_CANFD_SSP_OFFSET,
#    endif
#    ifdef CONFIG_FC7300_FLEXCAN_CANFD_BRS
#      ifdef CONFIG_FC7300_FLEXCAN4_CANFD_BRS
  .canfdbrs_en = true,
  .data_timing =
    {
      .presdiv = CONFIG_FC7300_FLEXCAN4_CANFD_DATA_PRESDIV,
      .propseg = CONFIG_FC7300_FLEXCAN4_CANFD_DATA_PROPSEG,
      .pseg1   = CONFIG_FC7300_FLEXCAN4_CANFD_DATA_PSEG1,
      .pseg2   = CONFIG_FC7300_FLEXCAN4_CANFD_DATA_PSEG2,
      .sjw     = CONFIG_FC7300_FLEXCAN4_CANFD_DATA_SJW,
    },
#      else
  .canfdbrs_en = false,
#      endif
#    endif
#  else
  .canfd_en = false,
#  endif
#endif

  .arbi_timing =
    {
      .presdiv = CONFIG_FC7300_FLEXCAN4_PRESDIV,
      .propseg = CONFIG_FC7300_FLEXCAN4_PROPSEG,
      .pseg1   = CONFIG_FC7300_FLEXCAN4_PSEG1,
      .pseg2   = CONFIG_FC7300_FLEXCAN4_PSEG2,
      .sjw     = CONFIG_FC7300_FLEXCAN4_SJW,
    },

  .rxmb_cnt    = FLEXCAN4_RXMB_COUNT,
  .txmb_cnt    = FLEXCAN4_TXMB_COUNT,
  .config      = &g_fc7300_flexcan4_config,
#ifdef CONFIG_FC7300_FLEXCAN_ENHANCED_RX_FIFO
  .erf_filter  = NULL,
#  ifdef CONFIG_FC7300_FLEXCAN4_ENHANCED_RX_FIFO
  .erf_en      = true,
  .erfel_total_cnt = CONFIG_FC7300_FLEXCAN4_ERFEL_TOTAL_COUNT,
#    ifdef CONFIG_CAN_EXTID
  .erfel_extid_cnt = CONFIG_FC7300_FLEXCAN4_ERFEL_EXTID_COUNT,
#    else
  .erfel_extid_cnt = 0,
#    endif
#  else
  .erf_en          = false,
  .erfel_total_cnt = 0,
  .erfel_extid_cnt = 0,
#  endif
#endif

  .rxmb_filter = NULL,
};
#endif

#ifdef CONFIG_FC7300_FLEXCAN5_CHARDRIVER
locate_code(".dtcm_data") struct fc7300_flexcan_priv_s g_fc7300_flexcan5priv =
{
  .base        = FC7300_FLEXCAN5_BASE,
  .clkname     = PCC_CLK_FLEXCAN5,
#ifdef CONFIG_CAN_FD
#  ifdef CONFIG_FC7300_FLEXCAN5_CANFD
  .canfd_en    = true,
  .data_size   = FLEXCAN5_CANFD_DATA_SIZE,
#    ifdef CONFIG_FC7300_FLEXCAN5_CANFD_SSP
  .ssp_en      = true,
  .ssp_offset  = CONFIG_FC7300_FLEXCAN5_CANFD_SSP_OFFSET,
#    endif
#    ifdef CONFIG_FC7300_FLEXCAN_CANFD_BRS
#      ifdef CONFIG_FC7300_FLEXCAN5_CANFD_BRS
  .canfdbrs_en = true,
  .data_timing =
    {
      .presdiv = CONFIG_FC7300_FLEXCAN5_CANFD_DATA_PRESDIV,
      .propseg = CONFIG_FC7300_FLEXCAN5_CANFD_DATA_PROPSEG,
      .pseg1   = CONFIG_FC7300_FLEXCAN5_CANFD_DATA_PSEG1,
      .pseg2   = CONFIG_FC7300_FLEXCAN5_CANFD_DATA_PSEG2,
      .sjw     = CONFIG_FC7300_FLEXCAN5_CANFD_DATA_SJW,
    },
#      else
  .canfdbrs_en = false,
#      endif
#    endif
#  else
  .canfd_en = false,
#  endif
#endif

  .arbi_timing =
    {
      .presdiv = CONFIG_FC7300_FLEXCAN5_PRESDIV,
      .propseg = CONFIG_FC7300_FLEXCAN5_PROPSEG,
      .pseg1   = CONFIG_FC7300_FLEXCAN5_PSEG1,
      .pseg2   = CONFIG_FC7300_FLEXCAN5_PSEG2,
      .sjw     = CONFIG_FC7300_FLEXCAN5_SJW,
    },

  .rxmb_cnt    = FLEXCAN5_RXMB_COUNT,
  .txmb_cnt    = FLEXCAN5_TXMB_COUNT,
  .config      = &g_fc7300_flexcan5_config,
#ifdef CONFIG_FC7300_FLEXCAN_ENHANCED_RX_FIFO
  .erf_filter  = NULL,
#  ifdef CONFIG_FC7300_FLEXCAN5_ENHANCED_RX_FIFO
  .erf_en      = true,
  .erfel_total_cnt = CONFIG_FC7300_FLEXCAN5_ERFEL_TOTAL_COUNT,
#    ifdef CONFIG_CAN_EXTID
  .erfel_extid_cnt = CONFIG_FC7300_FLEXCAN5_ERFEL_EXTID_COUNT,
#    else
  .erfel_extid_cnt = 0,
#    endif
#  else
  .erf_en          = false,
  .erfel_total_cnt = 0,
  .erfel_extid_cnt = 0,
#  endif
#endif

  .rxmb_filter = NULL,
};
#endif

#ifdef CONFIG_FC7300_FLEXCAN6_CHARDRIVER
locate_code(".dtcm_data") struct fc7300_flexcan_priv_s g_fc7300_flexcan6priv =
{
  .base        = FC7300_FLEXCAN6_BASE,
  .clkname     = PCC_CLK_FLEXCAN6,
#ifdef CONFIG_CAN_FD
#  ifdef CONFIG_FC7300_FLEXCAN6_CANFD
  .canfd_en    = true,
  .data_size   = FLEXCAN6_CANFD_DATA_SIZE,
#    ifdef CONFIG_FC7300_FLEXCAN6_CANFD_SSP
  .ssp_en      = true,
  .ssp_offset  = CONFIG_FC7300_FLEXCAN6_CANFD_SSP_OFFSET,
#    endif
#    ifdef CONFIG_FC7300_FLEXCAN_CANFD_BRS
#      ifdef CONFIG_FC7300_FLEXCAN6_CANFD_BRS
  .canfdbrs_en = true,
  .data_timing =
    {
      .presdiv = CONFIG_FC7300_FLEXCAN6_CANFD_DATA_PRESDIV,
      .propseg = CONFIG_FC7300_FLEXCAN6_CANFD_DATA_PROPSEG,
      .pseg1   = CONFIG_FC7300_FLEXCAN6_CANFD_DATA_PSEG1,
      .pseg2   = CONFIG_FC7300_FLEXCAN6_CANFD_DATA_PSEG2,
      .sjw     = CONFIG_FC7300_FLEXCAN6_CANFD_DATA_SJW,
    },
#      else
  .canfdbrs_en = false,
#      endif
#    endif
#  else
  .canfd_en = false,
#  endif
#endif

  .arbi_timing =
    {
      .presdiv = CONFIG_FC7300_FLEXCAN6_PRESDIV,
      .propseg = CONFIG_FC7300_FLEXCAN6_PROPSEG,
      .pseg1   = CONFIG_FC7300_FLEXCAN6_PSEG1,
      .pseg2   = CONFIG_FC7300_FLEXCAN6_PSEG2,
      .sjw     = CONFIG_FC7300_FLEXCAN6_SJW,
    },

  .rxmb_cnt    = FLEXCAN6_RXMB_COUNT,
  .txmb_cnt    = FLEXCAN6_TXMB_COUNT,
  .config      = &g_fc7300_flexcan6_config,
#ifdef CONFIG_FC7300_FLEXCAN_ENHANCED_RX_FIFO
  .erf_filter  = NULL,
#  ifdef CONFIG_FC7300_FLEXCAN6_ENHANCED_RX_FIFO
  .erf_en      = true,
  .erfel_total_cnt = CONFIG_FC7300_FLEXCAN6_ERFEL_TOTAL_COUNT,
#    ifdef CONFIG_CAN_EXTID
  .erfel_extid_cnt = CONFIG_FC7300_FLEXCAN6_ERFEL_EXTID_COUNT,
#    else
  .erfel_extid_cnt = 0,
#    endif
#  else
  .erf_en          = false,
  .erfel_total_cnt = 0,
  .erfel_extid_cnt = 0,
#  endif
#endif

  .rxmb_filter = NULL,
};
#endif

#ifdef CONFIG_FC7300_FLEXCAN7_CHARDRIVER
locate_code(".dtcm_data") struct fc7300_flexcan_priv_s g_fc7300_flexcan7priv =
{
  .base        = FC7300_FLEXCAN7_BASE,
  .clkname     = PCC_CLK_FLEXCAN7,
#ifdef CONFIG_CAN_FD
#  ifdef CONFIG_FC7300_FLEXCAN7_CANFD
  .canfd_en    = true,
  .data_size   = FLEXCAN7_CANFD_DATA_SIZE,
#    ifdef CONFIG_FC7300_FLEXCAN7_CANFD_SSP
  .ssp_en      = true,
  .ssp_offset  = CONFIG_FC7300_FLEXCAN7_CANFD_SSP_OFFSET,
#    endif
#    ifdef CONFIG_FC7300_FLEXCAN_CANFD_BRS
#      ifdef CONFIG_FC7300_FLEXCAN7_CANFD_BRS
  .canfdbrs_en = true,
  .data_timing =
    {
      .presdiv = CONFIG_FC7300_FLEXCAN7_CANFD_DATA_PRESDIV,
      .propseg = CONFIG_FC7300_FLEXCAN7_CANFD_DATA_PROPSEG,
      .pseg1   = CONFIG_FC7300_FLEXCAN7_CANFD_DATA_PSEG1,
      .pseg2   = CONFIG_FC7300_FLEXCAN7_CANFD_DATA_PSEG2,
      .sjw     = CONFIG_FC7300_FLEXCAN7_CANFD_DATA_SJW,
    },
#      else
  .canfdbrs_en = false,
#      endif
#    endif
#  else
  .canfd_en = false,
#  endif
#endif

  .arbi_timing =
    {
      .presdiv = CONFIG_FC7300_FLEXCAN7_PRESDIV,
      .propseg = CONFIG_FC7300_FLEXCAN7_PROPSEG,
      .pseg1   = CONFIG_FC7300_FLEXCAN7_PSEG1,
      .pseg2   = CONFIG_FC7300_FLEXCAN7_PSEG2,
      .sjw     = CONFIG_FC7300_FLEXCAN7_SJW,
    },

  .rxmb_cnt    = FLEXCAN7_RXMB_COUNT,
  .txmb_cnt    = FLEXCAN7_TXMB_COUNT,
  .config      = &g_fc7300_flexcan7_config,
#ifdef CONFIG_FC7300_FLEXCAN_ENHANCED_RX_FIFO
  .erf_filter  = NULL,
#  ifdef CONFIG_FC7300_FLEXCAN7_ENHANCED_RX_FIFO
  .erf_en      = true,
  .erfel_total_cnt = CONFIG_FC7300_FLEXCAN7_ERFEL_TOTAL_COUNT,
#    ifdef CONFIG_CAN_EXTID
  .erfel_extid_cnt = CONFIG_FC7300_FLEXCAN7_ERFEL_EXTID_COUNT,
#    else
  .erfel_extid_cnt = 0,
#    endif
#  else
  .erf_en          = false,
  .erfel_total_cnt = 0,
  .erfel_extid_cnt = 0,
#  endif
#endif

  .rxmb_filter = NULL,
};
#endif

#ifdef CONFIG_FC7300_FLEXCAN8_CHARDRIVER
locate_code(".dtcm_data") struct fc7300_flexcan_priv_s g_fc7300_flexcan8priv =
{
  .base        = FC7300_FLEXCAN8_BASE,
  .clkname     = PCC_CLK_FLEXCAN8,
#ifdef CONFIG_CAN_FD
#  ifdef CONFIG_FC7300_FLEXCAN8_CANFD
  .canfd_en    = true,
  .data_size   = FLEXCAN8_CANFD_DATA_SIZE,
#    ifdef CONFIG_FC7300_FLEXCAN8_CANFD_SSP
  .ssp_en      = true,
  .ssp_offset  = CONFIG_FC7300_FLEXCAN8_CANFD_SSP_OFFSET,
#    endif
#    ifdef CONFIG_FC7300_FLEXCAN_CANFD_BRS
#      ifdef CONFIG_FC7300_FLEXCAN8_CANFD_BRS
  .canfdbrs_en = true,
  .data_timing =
    {
      .presdiv = CONFIG_FC7300_FLEXCAN8_CANFD_DATA_PRESDIV,
      .propseg = CONFIG_FC7300_FLEXCAN8_CANFD_DATA_PROPSEG,
      .pseg1   = CONFIG_FC7300_FLEXCAN8_CANFD_DATA_PSEG1,
      .pseg2   = CONFIG_FC7300_FLEXCAN8_CANFD_DATA_PSEG2,
      .sjw     = CONFIG_FC7300_FLEXCAN8_CANFD_DATA_SJW,
    },
#      else
  .canfdbrs_en = false,
#      endif
#    endif
#  else
  .canfd_en = false,
#  endif
#endif

  .arbi_timing =
    {
      .presdiv = CONFIG_FC7300_FLEXCAN8_PRESDIV,
      .propseg = CONFIG_FC7300_FLEXCAN8_PROPSEG,
      .pseg1   = CONFIG_FC7300_FLEXCAN8_PSEG1,
      .pseg2   = CONFIG_FC7300_FLEXCAN8_PSEG2,
      .sjw     = CONFIG_FC7300_FLEXCAN8_SJW,
    },

  .rxmb_cnt    = FLEXCAN8_RXMB_COUNT,
  .txmb_cnt    = FLEXCAN8_TXMB_COUNT,
  .config      = &g_fc7300_flexcan8_config,
#ifdef CONFIG_FC7300_FLEXCAN_ENHANCED_RX_FIFO
  .erf_filter  = NULL,
#  ifdef CONFIG_FC7300_FLEXCAN8_ENHANCED_RX_FIFO
  .erf_en      = true,
  .erfel_total_cnt = CONFIG_FC7300_FLEXCAN8_ERFEL_TOTAL_COUNT,
#    ifdef CONFIG_CAN_EXTID
  .erfel_extid_cnt = CONFIG_FC7300_FLEXCAN8_ERFEL_EXTID_COUNT,
#    else
  .erfel_extid_cnt = 0,
#    endif
#  else
  .erf_en          = false,
  .erfel_total_cnt = 0,
  .erfel_extid_cnt = 0,
#  endif
#endif

  .rxmb_filter = NULL,
};
#endif

#ifdef CONFIG_FC7300_FLEXCAN9_CHARDRIVER
locate_code(".dtcm_data") struct fc7300_flexcan_priv_s g_fc7300_flexcan9priv =
{
  .base        = FC7300_FLEXCAN9_BASE,
  .clkname     = PCC_CLK_FLEXCAN9,
#ifdef CONFIG_CAN_FD
#  ifdef CONFIG_FC7300_FLEXCAN9_CANFD
  .canfd_en    = true,
  .data_size   = FLEXCAN9_CANFD_DATA_SIZE,
#    ifdef CONFIG_FC7300_FLEXCAN9_CANFD_SSP
  .ssp_en      = true,
  .ssp_offset  = CONFIG_FC7300_FLEXCAN9_CANFD_SSP_OFFSET,
#    endif
#    ifdef CONFIG_FC7300_FLEXCAN_CANFD_BRS
#      ifdef CONFIG_FC7300_FLEXCAN9_CANFD_BRS
  .canfdbrs_en = true,
  .data_timing =
    {
      .presdiv = CONFIG_FC7300_FLEXCAN9_CANFD_DATA_PRESDIV,
      .propseg = CONFIG_FC7300_FLEXCAN9_CANFD_DATA_PROPSEG,
      .pseg1   = CONFIG_FC7300_FLEXCAN9_CANFD_DATA_PSEG1,
      .pseg2   = CONFIG_FC7300_FLEXCAN9_CANFD_DATA_PSEG2,
      .sjw     = CONFIG_FC7300_FLEXCAN9_CANFD_DATA_SJW,
    },
#      else
  .canfdbrs_en = false,
#      endif
#    endif
#  else
  .canfd_en = false,
#  endif
#endif

  .arbi_timing =
    {
      .presdiv = CONFIG_FC7300_FLEXCAN9_PRESDIV,
      .propseg = CONFIG_FC7300_FLEXCAN9_PROPSEG,
      .pseg1   = CONFIG_FC7300_FLEXCAN9_PSEG1,
      .pseg2   = CONFIG_FC7300_FLEXCAN9_PSEG2,
      .sjw     = CONFIG_FC7300_FLEXCAN9_SJW,
    },

  .rxmb_cnt    = FLEXCAN9_RXMB_COUNT,
  .txmb_cnt    = FLEXCAN9_TXMB_COUNT,
  .config      = &g_fc7300_flexcan9_config,
#ifdef CONFIG_FC7300_FLEXCAN_ENHANCED_RX_FIFO
  .erf_filter  = NULL,
#  ifdef CONFIG_FC7300_FLEXCAN9_ENHANCED_RX_FIFO
  .erf_en      = true,
  .erfel_total_cnt = CONFIG_FC7300_FLEXCAN9_ERFEL_TOTAL_COUNT,
#    ifdef CONFIG_CAN_EXTID
  .erfel_extid_cnt = CONFIG_FC7300_FLEXCAN9_ERFEL_EXTID_COUNT,
#    else
  .erfel_extid_cnt = 0,
#    endif
#  else
  .erf_en          = false,
  .erfel_total_cnt = 0,
  .erfel_extid_cnt = 0,
#  endif
#endif

  .rxmb_filter = NULL,
};
#endif

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
 * Name: fc7300_flexcan_co_reset
 *
 * Description:
 *   Reset the CAN device. Called early to initialize the hardware.
 *
 * Input Parameters:
 *   dev - An instance of the "upper half" can driver state structure.
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/

static void fc7300_flexcan_co_reset(struct can_dev_s *dev)
{
  struct fc7300_flexcan_priv_s *priv;

  priv = (struct fc7300_flexcan_priv_s *)dev->cd_priv;

  /* Ensure diabling interrupt firstly */

  up_disable_irq(priv->config->irq);

  fc7300_reset_pcc(priv->clkname);

  return;
}

/****************************************************************************
 * Name: fc7300_flexcan_co_setup
 *
 * Description:
 *   Configure the CAN. This method is called the first time that the CAN
 *   device is opened. This will occur when the port is first opened.
 *   This setup includes configuring and enabling CAN NVIC interrupts.
 *   All CAN interrupts are disabled upon return.
 *
 * Input Parameters:
 *   dev - An instance of the "upper half" can driver state structure.
 *
 * Returned Value:
 *   Zero on success; a negated errno on failure
 *
 ****************************************************************************/

static int fc7300_flexcan_co_setup(struct can_dev_s *dev)
{
  int ret;
  struct fc7300_flexcan_priv_s *priv;

  priv = (struct fc7300_flexcan_priv_s *)dev->cd_priv;

  if (priv->state == CAN_STATE_START)
      return OK;

  ret = fc7300_flexcan_initialize(priv);
  if (ret < 0)
    {
      canerr("ERROR: initialize fail, base = 0x%08" PRIx16 "\n", (unsigned int)priv->base);
      return -EAGAIN;
    }

  /* Enable NVIC Interrupt */

#ifdef CONFIG_ARCH_IRQPRIO
  up_prioritize_irq(priv->config->irq, priv->config->irq_pri);
#endif
  up_enable_irq(priv->config->irq);

  /* Clear Flexcan internal interrupt mask */

  putreg32(0xffffffff, priv->base + FC7300_CAN_IFLAG1_OFFSET);
  putreg32(0x0, priv->base + FC7300_CAN_IMASK1_OFFSET);

  priv->txmb_sflags = 0;
  priv->state = CAN_STATE_START;

  return OK;
}

/****************************************************************************
 * Name: fc7300_flexcan_co_shutdown
 *
 * Description:
 *   Disable the CAN. This method is called when the CAN device is closed.
 *   This method reverses the operation the setup method.
 *
 * Input Parameters:
 *   dev - An instance of the "upper half" can driver state structure.
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/

static void fc7300_flexcan_co_shutdown(struct can_dev_s *dev)
{
  struct fc7300_flexcan_priv_s *priv;

  priv = (struct fc7300_flexcan_priv_s *)dev->cd_priv;

  if (priv->state == CAN_STATE_STOP)
    return;

  fc7300_flexcan_setenable(priv->base, 0);

  up_disable_irq(priv->config->irq);

  /* Reset all registers */

  fc7300_reset_pcc(priv->clkname);

  /* Clear Flexcan internal interrupt mask */

  putreg32(0xffffffff, priv->base + FC7300_CAN_IFLAG1_OFFSET);
  putreg32(0x0, priv->base + FC7300_CAN_IMASK1_OFFSET);

  priv->state = CAN_STATE_STOP;
}

/****************************************************************************
 * Name: fc7300_flexcan_co_rxint
 *
 * Description:
 *   Call to enable or disable RX interrupts.
 *
 * Input Parameters:
 *   dev    - An instance of the "upper half" can driver state structure.
 *   enable - Enable or disable flag.
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/

static void fc7300_flexcan_co_rxint(struct can_dev_s *dev, bool enable)
{
  uint32_t masks;
  irqstate_t irqflags;
  struct fc7300_flexcan_priv_s *priv;

  priv = (struct fc7300_flexcan_priv_s *)dev->cd_priv;

  irqflags = enter_critical_section();

  masks = getreg32(priv->base + FC7300_CAN_IMASK1_OFFSET);

  if (enable)
      masks |= IFLAG1_RX;
  else
      masks &= ~IFLAG1_RX;

  putreg32(masks, priv->base + FC7300_CAN_IMASK1_OFFSET);

#ifdef CONFIG_FC7300_FLEXCAN_ENHANCED_RX_FIFO
  if (priv->erf_en == true)
    {
      masks = getreg32(priv->base + FC7300_CAN_ERFIER_OFFSET);

      if (enable)
          masks |= CAN_ERFIER_ERFDAIE;
      else
          masks &= ~CAN_ERFIER_ERFDAIE;

      putreg32(masks, priv->base + FC7300_CAN_ERFIER_OFFSET);
    }
#endif

  leave_critical_section(irqflags);
}

/****************************************************************************
 * Name: fc7300_flexcan_co_txint
 *
 * Description:
 *   Call to enable or disable TX interrupts.
 *
 * Input Parameters:
 *   dev    - An instance of the "upper half" can driver state structure.
 *   enable - Enable or disable flag.
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/

static void fc7300_flexcan_co_txint(struct can_dev_s *dev, bool enable)
{
  uint32_t masks;
  irqstate_t irqflags;
  struct fc7300_flexcan_priv_s *priv;

  priv = (struct fc7300_flexcan_priv_s *)dev->cd_priv;

  irqflags = enter_critical_section();

  masks = getreg32(priv->base + FC7300_CAN_IMASK1_OFFSET);

  if (enable)
      masks |= IFLAG1_TX;
  else
      masks &= ~IFLAG1_TX;

  putreg32(masks, priv->base + FC7300_CAN_IMASK1_OFFSET);

  leave_critical_section(irqflags);
}

/****************************************************************************
 * Name: fc7300_flexcan_setmode
 *
 * Description:
 *   Set fc7300 flexcan state machine mode
 *
 * Input Parameters:
 *   dev - An instance of the "upper half" can driver state structure.
 *   arg - Mode.
 *
 * Returned Value:
 *   Zero on success; a negated errno on failure
 *
 ****************************************************************************/

static int fc7300_flexcan_setmode(struct can_dev_s *dev,
                                  unsigned long arg)
{
  int ret = 0;
  struct fc7300_flexcan_priv_s *priv;

  priv = (struct fc7300_flexcan_priv_s *)dev->cd_priv;

  switch (arg)
  {
    case CAN_STATE_START:
      if (priv->state != CAN_STATE_START)
        {
          if (!fc7300_flexcan_setenable(priv->base, 1))
              priv->state = CAN_STATE_START;
          else
              ret = -EAGAIN;
        }
      break;
    case CAN_STATE_STOP:
      if (priv->state != CAN_STATE_STOP)
        {
          if (!fc7300_flexcan_setenable(priv->base, 0))
              priv->state = CAN_STATE_STOP;
          else
              ret = -EAGAIN;
        }
      break;
    default:
      ret = -EINVAL;
      break;
  }

  return ret;
}

/****************************************************************************
 * Name: fc7300_flexcan_getmode
 *
 * Description:
 *   Set fc7300 flexcan state machine mode
 *
 * Input Parameters:
 *   dev   - An instance of the "upper half" can driver state structure.
 *   state - Mode value pointer.
 *
 * Returned Value:
 *   Zero on success; a negated errno on failure
 *
 ****************************************************************************/

static int fc7300_flexcan_getmode(struct can_dev_s *dev,
                                  unsigned long *state)
{
  struct fc7300_flexcan_priv_s *priv;

  priv = (struct fc7300_flexcan_priv_s *)dev->cd_priv;

  if (getreg32(priv->base + FC7300_CAN_MCR_OFFSET) & CAN_MCR_MDIS)
    {
      *state = CAN_STATE_STOP;
    }
  else
    {
      *state = CAN_STATE_START;
    }

  return 0;
}

/****************************************************************************
 * Name: fc7300_clear_rxmb
 *
 * Description:
 *   Clear fc7300 flexcan txmb. This function adopt asynchronous method.
 *   Filter frame that was aborted successfully in confirm function.
 *
 * Input Parameters:
 *   dev - An instance of the "upper half" can driver state structure.
 *
 * Returned Value:
 *   Zero on success; a negated errno on failure.
 *
 ****************************************************************************/

static int fc7300_flexcan_clear_txmb(struct fc7300_flexcan_priv_s *priv)
{
  uint32_t mbi;

#ifdef CONFIG_CAN_FD
  if (priv->canfd_en)
    {
      struct canfdmb_s *mb = (struct canfdmb_s *)priv->tx;
      for (mbi = 0; mbi < priv->txmb_cnt; mbi++)
        {
          if ((1 << mbi) & priv->txmb_sflags)
              mb[mbi].cs.code = CAN_TXMB_ABORT; /* Abort */
        }
    }
  else
#endif
    {
      struct canmb_s *mb = (struct canmb_s *)priv->tx;
      for (mbi = 0; mbi < priv->txmb_cnt; mbi++)
        {
          if ((1 << mbi) & priv->txmb_sflags)
              mb[mbi].cs.code = CAN_TXMB_ABORT; /* Abort */
        }
    }

  /* Asynchronous operation, do nothing below here, clear flag in isr. */

  return 0;
}

/****************************************************************************
 * Name: fc7300_flexcan_co_ioctl
 *
 * Description:
 *   All ioctl calls will be routed through this method
 *
 * Input Parameters:
 *   dev - An instance of the "upper half" can driver state structure.
 *   cmd - Command value.
 *   arg - Command arguments.
 *
 * Returned Value:
 *   Zero on success; a negated errno on failure
 *
 ****************************************************************************/

static int fc7300_flexcan_co_ioctl(struct can_dev_s *dev, int cmd,
                          unsigned long arg)
{
  int ret  = -ENOTTY;
  unsigned long *state;
  struct fc7300_flexcan_priv_s *priv;

  priv = (struct fc7300_flexcan_priv_s *)dev->cd_priv;

  switch (cmd)
    {
      case CANIOC_BUSOFF_RECOVERY:
        ret = fc7300_flexcan_initialize(priv);
        if (ret == 0)
          {
            /* Enable recevie interrupt */

            fc7300_flexcan_co_rxint(dev, true);

            /* TODO : Transmit interrupt will be enabled in nuttx upper
             *        driver can_xmit api. But if we want, we can enable
             *        it here.
             */
          }
        break;
      case CANIOC_SET_STATE:
        ret = fc7300_flexcan_setmode(dev, arg);
        break;
      case CANIOC_GET_STATE:
        state = (unsigned long *)arg;
        ret = fc7300_flexcan_getmode(dev, state);
        break;
      case CANIOC_OFLUSH:
        ret = fc7300_flexcan_clear_txmb(priv);
        break;
    }

  return ret;
}

/****************************************************************************
 * Name: fc7300_flexcan_co_remoterequest
 *
 * Description:
 *   Send a remote request
 *
 * Input Parameters:
 *   dev - An instance of the "upper half" can driver state structure.
 *   id  - ID.
 *
 * Returned Value:
 *   Zero on success; a negated errno on failure
 *
 ****************************************************************************/

static int fc7300_flexcan_co_remoterequest(struct can_dev_s *dev,
                                           uint16_t id)
{
  return -ENOSYS;
}

/****************************************************************************
 * Name: fc7300_flexcan_find_txmb
 *
 * Description:
 *   Find one inactive and empty txmb.
 *
 * Input Parameters:
 *   dev - An instance of the "upper half" can driver state structure.
 *   msg - Can message
 *
 * Returned Value:
 *   <0  - failed
 *   >=0 - txmb index
 *
 ****************************************************************************/

static uint32_t fc7300_flexcan_find_txmb(struct can_dev_s *dev,
                                         struct can_msg_s *msg)
{
  uint32_t len;
  uint32_t mbi;
  uint32_t available_mbi;
  bool available_mb;
  struct fc7300_flexcan_priv_s *priv;

  priv = (struct fc7300_flexcan_priv_s *)dev->cd_priv;
  len  = g_fc7300_can_dlc_to_len[msg->cm_hdr.ch_dlc];

  available_mb  = false;
  available_mbi = 0;

  if ((len > 8)
#ifdef CONFIG_CAN_FD
    && (priv->canfd_en == false)
#endif
    )
    {
      canerr("ERROR: CAN FD is not enabled, base = 0x%08" PRIx16 "\n", (unsigned int)priv->base);
      return -EINVAL;
    }

  available_mb = (priv->txmb_sflags ^ IFLAG1_TX) ? true : false;
  if (!available_mb)
      return -EINVAL;

  available_mbi = arm_lsb(priv->txmb_sflags ^ IFLAG1_TX);

#ifdef CONFIG_CAN_FD
  if (priv->canfd_en)
    {
      struct canfdmb_s *mb = (struct canfdmb_s *)priv->tx;

      for (mbi = 0; mbi < priv->txmb_cnt; mbi++)
        {
          if ((1 << mbi) & priv->txmb_sflags)
            {
#ifdef CONFIG_CAN_EXTID
              if (msg->cm_hdr.ch_extid)
                {
                  if (mb[mbi].cs.ide && (mb[mbi].id.ext == (msg->cm_hdr.ch_id & MASKEXTID)))
                    {
                      available_mb = false;
                      break;
                    }
                }
              else
#endif
                {
                  if (mb[mbi].id.std == (msg->cm_hdr.ch_id & MASKSTDID))
                    {
                      available_mb = false;
                      break;
                    }
                }
            }
        }
    }
  else
#endif
    {
      struct canmb_s *mb = (struct canmb_s *)priv->tx;

      for (mbi = 0; mbi < priv->txmb_cnt; mbi++)
        {
          if ((1 << mbi) & priv->txmb_sflags)
            {
#ifdef CONFIG_CAN_EXTID
              if (msg->cm_hdr.ch_extid)
                {
                  if (mb[mbi].cs.ide && (mb[mbi].id.ext == (msg->cm_hdr.ch_id & MASKEXTID)))
                    {
                      available_mb = false;
                      break;
                    }
                }
              else
#endif
                {
                  if (mb[mbi].id.std == (msg->cm_hdr.ch_id & MASKSTDID))
                    {
                      available_mb = false;
                      break;
                    }
                }
            }
        }
    }

  if (!available_mb)
      return -EINVAL;

  return available_mbi;
}

/****************************************************************************
 * Name: fc7300_flexcan_co_send
 *
 * Description:
 *   Send one can message.
 *
 * Input Parameters:
 *   dev - An instance of the "upper half" can driver state structure.
 *   msg - Can message structure.
 *
 * Returned Value:
 *   Zero on success; a negated errno on failure
 *
 ****************************************************************************/

static int fc7300_flexcan_co_send(struct can_dev_s *dev,
                                  struct can_msg_s *msg)
{
  uint32_t i;
  uint32_t mbi;
  uint32_t len;
  uint32_t *frame_data_word;
  struct fc7300_flexcan_priv_s *priv;

  priv = (struct fc7300_flexcan_priv_s *)dev->cd_priv;

  mbi = fc7300_flexcan_find_txmb(dev, msg);
  if (mbi >= priv->txmb_cnt)
    {
      canwarn("WARN: No TX MB available, base = 0x%08" PRIx16 "\n", (unsigned int)priv->base);
      return -EBUSY;       /* No transmission for you! */
    }

  len = g_fc7300_can_dlc_to_len[msg->cm_hdr.ch_dlc];

  union cs_e cs = {
    0
  };

  cs.code = CAN_TXMB_DATAORREMOTE;

#ifdef CONFIG_CAN_FD
  if (priv->canfd_en)
    {
      struct canfdmb_s *mb = &((struct canfdmb_s *)priv->tx)[mbi];

      mb->cs.code = CAN_TXMB_INACTIVE;

      cs.edl  = 1;  /* CAN FD frame */

#ifdef CONFIG_CAN_EXTID
      if (msg->cm_hdr.ch_extid)
        {
          cs.ide = 1; /* ID Extended Bit */
          cs.srr = 1; /* Fiexed recessive bit when transmit a extend id frame */
          mb->id.ext = msg->cm_hdr.ch_id & MASKEXTID;
        }
      else
#endif
        {
          mb->id.std = msg->cm_hdr.ch_id & MASKSTDID;
        }

      if (msg->cm_hdr.ch_brs)
        {
#ifdef CONFIG_FC7300_FLEXCAN_CANFD_BRS
          if (priv->canfdbrs_en)
            {
              cs.brs = 1;
            }
          else
#endif
            {
              canwarn("WARN: BRS is not enabled, base = 0x%08" PRIx16 "\n",
                       (unsigned int)priv->base);
            }
        }

      cs.rtr = msg->cm_hdr.ch_rtr ? 1 : 0;
      cs.dlc = msg->cm_hdr.ch_dlc;

      if (len <= 8)
        {
          if (!msg->cm_hdr.ch_edl)
            cs.edl = 0;

          mb->data[0].w00 = bswap_32(*(uint32_t *)&msg->cm_data[0]);
          mb->data[1].w00 = bswap_32(*(uint32_t *)&msg->cm_data[4]);
        }
      else
        {
          frame_data_word = (uint32_t *)&msg->cm_data[0];

          for (i = 0; i < (len + 4 - 1) / 4; i++)
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

      if (len <= 8)
        {
#ifdef CONFIG_CAN_EXTID
          if (msg->cm_hdr.ch_extid)
            {
              cs.ide = 1; /* ID Extended Bit */
              cs.srr = 1; /* Fiexed recessive bit when transmit a extend id frame */
              mb->id.ext = msg->cm_hdr.ch_id & MASKEXTID;
            }
          else
#endif
            {
              mb->id.std = msg->cm_hdr.ch_id & MASKSTDID;
            }

          cs.rtr = msg->cm_hdr.ch_rtr ? 1 : 0;
          cs.dlc = msg->cm_hdr.ch_dlc;

          mb->data[0].w00 = bswap_32(*(uint32_t *)&msg->cm_data[0]);
          mb->data[1].w00 = bswap_32(*(uint32_t *)&msg->cm_data[4]);
        }

      mb->cs = cs; /* Go. */
    }

  priv->txmb_sflags |= (1 << mbi);

  return 0;
}

/****************************************************************************
 * Name: fc7300_flexcan_co_txready
 *
 * Description:
 *   Return true if the CAN hardware can accept another TX message.
 *
 * Input Parameters:
 *   dev - An instance of the "upper half" can driver state structure.
 *
 * Returned Value:
 *   True if the CAN hardware is ready to accept another TX message.
 *
 ****************************************************************************/

static bool fc7300_flexcan_co_txready(struct can_dev_s *dev)
{
  struct fc7300_flexcan_priv_s *priv;

  priv = (struct fc7300_flexcan_priv_s *)dev->cd_priv;

  return (priv->txmb_sflags == IFLAG1_TX) ? false : true;
}

/****************************************************************************
 * Name: fc7300_flexcan_co_txempty
 *
 * Description:
 *   Return true if all message have been sent.  If for example, the CAN
 *   hardware implements FIFOs, then this would mean the transmit FIFO is
 *   empty.  This method is called when the driver needs to make sure that
 *   all characters are "drained" from the TX hardware before calling
 *   co_shutdown().
 *
 * Input Parameters:
 *   dev - An instance of the "upper half" can driver state structure.
 *
 * Returned Value:
 *   True if there are no pending TX transfers in the CAN hardware.
 *
 ****************************************************************************/

static bool fc7300_flexcan_co_txempty(struct can_dev_s *dev)
{
  struct fc7300_flexcan_priv_s *priv;

  priv = (struct fc7300_flexcan_priv_s *)dev->cd_priv;

  return priv->txmb_sflags ? false : true;
}

/****************************************************************************
 * Name: fc7300_flexcan_co_cancel
 *
 * Description:
 *   Cancel one can message.
 *
 * Input Parameters:
 *   dev - An instance of the "upper half" can driver state structure.
 *   msg - One can message.
 *
 * Returned Value:
 *   Zero on success; a negated errno on failure.
 *
 ****************************************************************************/

static bool fc7300_flexcan_co_cancel(struct can_dev_s *dev,
                                     struct can_msg_s *msg)
{
  bool status;
  bool available;
  uint32_t mbi;
  uint32_t mask;
  uint32_t mb_bit;
  uint32_t iflags;
  irqstate_t flags;
  struct fc7300_flexcan_priv_s *priv;
  uint32_t timeout = CAN_TIMEOUT_CYCLE;

  priv = (struct fc7300_flexcan_priv_s *)dev->cd_priv;
  mbi       = 0;
  mb_bit    = 0;
  status    = true;
  available = false;

  flags = enter_critical_section();

  /* Disable all TXMB interrupt firstly, if not, abort success operation will trigger isr */

  mask = getreg32(priv->base + FC7300_CAN_IMASK1_OFFSET);
  mask = mask & ~IFLAG1_TX;
  putreg32(mask, priv->base + FC7300_CAN_IMASK1_OFFSET);

#ifdef CONFIG_CAN_FD
  if (priv->canfd_en)
    {
      struct canfdmb_s *mb = (struct canfdmb_s *)priv->tx;
      while (mbi < priv->txmb_cnt)
        {
#ifdef CONFIG_CAN_EXTID
          if (msg->cm_hdr.ch_extid && mb[mbi].cs.ide)
            {
              if (mb[mbi].id.ext == (msg->cm_hdr.ch_id & MASKEXTID))
                {
                  available = true;
                }
            }
          else
#endif
            {
              if (mb[mbi].id.std == (msg->cm_hdr.ch_id & MASKSTDID))
                {
                  available = true;
                }
            }

          if (available && ((1 << mbi) & priv->txmb_sflags))
            {
              break;
            }

          available = false;
          mbi++;
        }
    }
  else
#endif
    {
      struct canmb_s *mb = (struct canmb_s *)priv->tx;
      while (mbi < priv->txmb_cnt)
        {
#ifdef CONFIG_CAN_EXTID
          if (msg->cm_hdr.ch_extid && mb[mbi].cs.ide)
            {
              if (mb[mbi].id.ext == (msg->cm_hdr.ch_id & MASKEXTID))
                {
                  available = true;
                }
            }
          else
#endif
            {
              if (mb[mbi].id.std == (msg->cm_hdr.ch_id & MASKSTDID))
                {
                  available = true;
                }
            }

          if (available && ((1 << mbi) & priv->txmb_sflags))
            {
              break;
            }

          available = false;
          mbi++;
        }
    }

  if (!available)
    {
      /* Enable all TXMB interrupt. */

      mask = getreg32(priv->base + FC7300_CAN_IMASK1_OFFSET);
      mask = mask | IFLAG1_TX;
      putreg32(mask, priv->base + FC7300_CAN_IMASK1_OFFSET);

      leave_critical_section(flags);

      return false;
    }

#ifdef CONFIG_CAN_FD
  if (priv->canfd_en)
    {
      struct canfdmb_s *mb = &((struct canfdmb_s *)priv->tx)[mbi];
      mb->cs.code = CAN_TXMB_ABORT;   /* Abort */
    }
  else
#endif
    {
      struct canmb_s *mb = &((struct canmb_s *)priv->tx)[mbi];
      mb->cs.code = CAN_TXMB_ABORT;   /* Abort */
    }

  mb_bit = 1 << mbi;

  /* Polling to wait cancel done */

  timeout = CAN_TIMEOUT_CYCLE;
  while (timeout != 0U)
    {
      iflags = getreg32(priv->base + FC7300_CAN_IFLAG1_OFFSET);
      if (iflags & mb_bit)
          break;
      else
          up_udelay(10);
      timeout --;
    }

  /* If timeout, do nothing, the following will judge MB cs.code. */

#ifdef CONFIG_CAN_FD
  if (priv->canfd_en)
    {
      struct canfdmb_s *mb = &((struct canfdmb_s *)priv->tx)[mbi];
      if (mb->cs.code == CAN_TXMB_INACTIVE)
          status = false;      /* Abort failed. */
    }
  else
#endif
    {
      struct canmb_s *mb = &((struct canmb_s *)priv->tx)[mbi];
      if (mb->cs.code == CAN_TXMB_INACTIVE)
          status = false;      /* Abort failed. */
    }

  if (status)
    {
      /* If abort successfully, clear flag. */

      putreg32(mb_bit, priv->base + FC7300_CAN_IFLAG1_OFFSET);
      priv->txmb_sflags &= ~mb_bit;
    }

  /* Enable all TXMB interrupt. */

  mask = getreg32(priv->base + FC7300_CAN_IMASK1_OFFSET);
  mask = mask | IFLAG1_TX;
  putreg32(mask, priv->base + FC7300_CAN_IMASK1_OFFSET);

  leave_critical_section(flags);

  return status;
}

static void fc7300_flexcan_co_errhandle(struct can_dev_s *dev)
{
#if defined(CONFIG_CAN_ERRORS)
  struct flexcan_errinfo errinfo;
  struct fc7300_flexcan_priv_s *priv;

  priv = (struct fc7300_flexcan_priv_s *)dev->cd_priv;

  /* Handle can error information */

  fc7300_flexcan_geterrinfo(priv->base, &errinfo);
  fc7300_flexcan_handle_error(dev, &errinfo, FLEXCAN_ERROR_POLLING);
#endif
}

static const struct can_ops_s g_fc7300_flexcanops =
{
  .co_reset         = fc7300_flexcan_co_reset,
  .co_setup         = fc7300_flexcan_co_setup,
  .co_shutdown      = fc7300_flexcan_co_shutdown,
  .co_rxint         = fc7300_flexcan_co_rxint,
  .co_txint         = fc7300_flexcan_co_txint,
  .co_ioctl         = fc7300_flexcan_co_ioctl,
  .co_remoterequest = fc7300_flexcan_co_remoterequest,
  .co_send          = fc7300_flexcan_co_send,
  .co_txready       = fc7300_flexcan_co_txready,
  .co_txempty       = fc7300_flexcan_co_txempty,
  .co_cancel        = fc7300_flexcan_co_cancel,
  .co_errhandle     = fc7300_flexcan_co_errhandle,
};

#ifdef CONFIG_FC7300_FLEXCAN0_CHARDRIVER
locate_code(".dtcm_data") struct can_dev_s g_fc7300_flexcan0dev =
{
  .cd_ops  = &g_fc7300_flexcanops,
  .cd_priv = &g_fc7300_flexcan0priv,
};
#endif

#ifdef CONFIG_FC7300_FLEXCAN1_CHARDRIVER
locate_code(".dtcm_data") struct can_dev_s g_fc7300_flexcan1dev =
{
  .cd_ops  = &g_fc7300_flexcanops,
  .cd_priv = &g_fc7300_flexcan1priv,
};
#endif

#ifdef CONFIG_FC7300_FLEXCAN2_CHARDRIVER
locate_code(".dtcm_data") struct can_dev_s g_fc7300_flexcan2dev =
{
  .cd_ops  = &g_fc7300_flexcanops,
  .cd_priv = &g_fc7300_flexcan2priv,
};
#endif

#ifdef CONFIG_FC7300_FLEXCAN3_CHARDRIVER
locate_code(".dtcm_data") struct can_dev_s g_fc7300_flexcan3dev =
{
  .cd_ops  = &g_fc7300_flexcanops,
  .cd_priv = &g_fc7300_flexcan3priv,
};
#endif

#ifdef CONFIG_FC7300_FLEXCAN4_CHARDRIVER
locate_code(".dtcm_data") struct can_dev_s g_fc7300_flexcan4dev =
{
  .cd_ops  = &g_fc7300_flexcanops,
  .cd_priv = &g_fc7300_flexcan4priv,
};
#endif

#ifdef CONFIG_FC7300_FLEXCAN5_CHARDRIVER
locate_code(".dtcm_data") struct can_dev_s g_fc7300_flexcan5dev =
{
  .cd_ops  = &g_fc7300_flexcanops,
  .cd_priv = &g_fc7300_flexcan5priv,
};
#endif

#ifdef CONFIG_FC7300_FLEXCAN6_CHARDRIVER
locate_code(".dtcm_data") struct can_dev_s g_fc7300_flexcan6dev =
{
  .cd_ops  = &g_fc7300_flexcanops,
  .cd_priv = &g_fc7300_flexcan6priv,
};
#endif

#ifdef CONFIG_FC7300_FLEXCAN7_CHARDRIVER
locate_code(".dtcm_data") struct can_dev_s g_fc7300_flexcan7dev =
{
  .cd_ops  = &g_fc7300_flexcanops,
  .cd_priv = &g_fc7300_flexcan7priv,
};
#endif

#ifdef CONFIG_FC7300_FLEXCAN8_CHARDRIVER
locate_code(".dtcm_data") struct can_dev_s g_fc7300_flexcan8dev =
{
  .cd_ops  = &g_fc7300_flexcanops,
  .cd_priv = &g_fc7300_flexcan8priv,
};
#endif

#ifdef CONFIG_FC7300_FLEXCAN9_CHARDRIVER
locate_code(".dtcm_data") struct can_dev_s g_fc7300_flexcan9dev =
{
  .cd_ops  = &g_fc7300_flexcanops,
  .cd_priv = &g_fc7300_flexcan9priv,
};
#endif

#if defined(CONFIG_CAN_ERRORS)
static void fc7300_flexcan_geterrinfo(uint32_t base,
                                      struct flexcan_errinfo *info)
{
  uint32_t regval;

  DEBUGASSERT(info != NULL);

  memset(info, 0, sizeof(struct flexcan_errinfo));

  regval = getreg32(base + FC7300_CAN_ESR1_OFFSET);
  regval &= CAN_ESR1_ERRINT | CAN_ESR1_BOFFINT | CAN_ESR1_FLTCONF_MASK |
            CAN_ESR1_RXWRN | CAN_ESR1_TXWRN | CAN_ESR1_STFERR |
            CAN_ESR1_FRMERR | CAN_ESR1_CRCERR | CAN_ESR1_ACKERR |
            CAN_ESR1_BIT0ERR | CAN_ESR1_BIT1ERR | CAN_ESR1_RWRNINT |
            CAN_ESR1_TWRNINT | CAN_ESR1_BOFFDONEINT | CAN_ESR1_ERRINT_FAST |
            CAN_ESR1_ERROVR | CAN_ESR1_STFERR_FAST | CAN_ESR1_FRMERR_FAST |
            CAN_ESR1_CRCERR_FAST | CAN_ESR1_BIT0ERR_FAST;
  info->esr1value = regval;

#if 0
  regval = getreg32(base + FC7300_CAN_ECR_OFFSET);
  info->rxerrcnt = (regval & CAN_ECR_RXERRCNT_MASK) >>
                             CAN_ECR_RXERRCNT_SHIFT;
  info->rxerrcnt_fast = (regval & CAN_ECR_TXERRCNT_FAST_MASK) >>
                                  CAN_ECR_TXERRCNT_FAST_SHIFT;
  info->txerrcnt = (regval & CAN_ECR_TXERRCNT_MASK) >>
                             CAN_ECR_TXERRCNT_SHIFT;
  info->txerrcnt_fast = (regval & CAN_ECR_TXERRCNT_FAST_MASK) >>
                                  CAN_ECR_TXERRCNT_FAST_SHIFT;
#endif

#ifdef CONFIG_FC7300_FLEXCAN_ENHANCED_RX_FIFO
  regval = getreg32(base + FC7300_CAN_ERFSR_OFFSET);
  info->erfsrvalue = (regval & CAN_ERFSR_ERFOVF);
#endif
}
#endif

#if defined(CONFIG_CAN_ERRORS)
__ramfunc__ static void fc7300_flexcan_handle_error(struct can_dev_s *dev,
                        struct flexcan_errinfo *info, enum flexcan_errmode mode)
{
  uint32_t status;
  uint32_t esr1flags;
  uint32_t erfsrflags;
  uint16_t errbits;
  uint8_t data[CAN_ERROR_DLC];
  struct fc7300_flexcan_priv_s *priv;
  struct can_hdr_s hdr =
  {
    0
  };

  status     = 0;
  errbits    = 0;
  esr1flags  = 0;
  erfsrflags = 0;
  memset(data, 0, sizeof(data));

  priv = (struct fc7300_flexcan_priv_s *)dev->cd_priv;

  if (mode == FLEXCAN_ERROR_INTERRUPT)
    {
      if (info->esr1value & CAN_ESR1_BOFFINT)
        {
          errbits   |= CAN_ERROR_BUSOFF;
          esr1flags |= CAN_ESR1_BOFFINT;
        }

#ifdef CONFIG_FC7300_FLEXCAN_CAN_ERROR_INTERRUPT

      if (info->esr1value & CAN_ESR1_ERRINT)
        {
          if (info->esr1value & CAN_ESR1_STFERR)
            {
              /* Stuff Error */

              data[2] |= CAN_ERROR2_STUFF;
              errbits |= CAN_ERROR_PROTOCOL;
            }

          if (info->esr1value & CAN_ESR1_FRMERR)
            {
              /* Format Error */

              data[2] |= CAN_ERROR2_FORM;
              errbits |= CAN_ERROR_PROTOCOL;
            }

          if (info->esr1value & CAN_ESR1_ACKERR)
            {
              /* Acknowledge Error */

              errbits |= CAN_ERROR_NOACK;
            }

          if (info->esr1value & CAN_ESR1_BIT0ERR)
            {
              /* Bit dominant Error */

              data[2] |= CAN_ERROR2_BIT;
              data[2] |= CAN_ERROR2_BIT0;
              errbits |= CAN_ERROR_PROTOCOL;
            }

          if (info->esr1value & CAN_ESR1_BIT1ERR)
            {
              /* Bit recessive Error */

              data[2] |= CAN_ERROR2_BIT;
              data[2] |= CAN_ERROR2_BIT1;
              errbits |= CAN_ERROR_PROTOCOL;
            }

#if 0
          if (info->esr1value & CAN_ESR1_CRCERR)
            {
              /* Receive CRC Error */

              data[3] |= CAN_ERROR3_CRCSEQ;
              errbits |= CAN_ERROR_PROTOCOL;
            }
#endif

          esr1flags = CAN_ESR1_ERRINT;
        }
#endif
    }
  else
    {
#ifndef CONFIG_FC7300_FLEXCAN_CAN_ERROR_INTERRUPT
      if (info->esr1value & CAN_ESR1_ERRINT)
        {
          if (info->esr1value & CAN_ESR1_STFERR)
            {
              /* Stuff Error */

              data[2] |= CAN_ERROR2_STUFF;
              errbits |= CAN_ERROR_PROTOCOL;
            }

          if (info->esr1value & CAN_ESR1_FRMERR)
            {
              /* Format Error */

              data[2] |= CAN_ERROR2_FORM;
              errbits |= CAN_ERROR_PROTOCOL;
            }

          if (info->esr1value & CAN_ESR1_ACKERR)
            {
              /* Acknowledge Error */

              errbits |= CAN_ERROR_NOACK;
            }

          if (info->esr1value & CAN_ESR1_BIT0ERR)
            {
              /* Bit dominant Error */

              data[2] |= CAN_ERROR2_BIT;
              data[2] |= CAN_ERROR2_BIT0;
              errbits |= CAN_ERROR_PROTOCOL;
            }

          if (info->esr1value & CAN_ESR1_BIT1ERR)
            {
              /* Bit recessive Error */

              data[2] |= CAN_ERROR2_BIT;
              data[2] |= CAN_ERROR2_BIT1;
              errbits |= CAN_ERROR_PROTOCOL;
            }

#if 0
          if (info->esr1value & CAN_ESR1_CRCERR)
            {
              /* Receive CRC Error */

              data[3] |= CAN_ERROR3_CRCSEQ;
              errbits |= CAN_ERROR_PROTOCOL;
            }
#endif

          esr1flags = CAN_ESR1_ERRINT;
        }

#endif

      if (info->esr1value & CAN_ESR1_RXWRN)
        {
          /* RX error warning flag */

          data[1]   |= CAN_ERROR1_RXWARNING;
          errbits   |= CAN_ERROR_CONTROLLER;
          esr1flags |= CAN_ESR1_RXWRN;
        }

      if (info->esr1value & CAN_ESR1_TXWRN)
        {
          /* TX error warning flag */

          data[1]   |= CAN_ERROR1_TXWARNING;
          errbits   |= CAN_ERROR_CONTROLLER;
          esr1flags |= CAN_ESR1_TXWRN;
        }

      status = info->esr1value & CAN_ESR1_FLTCONF_MASK;
      if (status == CAN_ESR1_FLTCONF_PASV)
        {
          data[1] |= CAN_ERROR1_RXPASSIVE;  /* ? */
          data[1] |= CAN_ERROR1_TXPASSIVE;  /* ? */
          errbits |= CAN_ERROR_CONTROLLER;
        }

#ifdef CONFIG_FC7300_FLEXCAN_ENHANCED_RX_FIFO
      if (priv->erf_en == true)
        {
          if (info->erfsrvalue & CAN_ERFSR_ERFOVF)
            {
              data[1]    |= CAN_ERROR1_RXOVERFLOW;
              errbits    |= CAN_ERROR_CONTROLLER;
              erfsrflags |= CAN_ERFSR_ERFOVF;
            }
        }
#endif
    }

  /* Report a CAN error */

  if (errbits != 0)
    {
      canerr("ERROR: errbits = 0x%08" PRIx16 ", base = 0x%08" PRIx16 "\n",
              errbits, (unsigned int)priv->base);

      /* Format the CAN header for the error report */

      hdr.ch_id     = errbits;
      hdr.ch_dlc    = CAN_ERROR_DLC;
      hdr.ch_rtr    = 0;
      hdr.ch_error  = 1;
#ifdef CONFIG_CAN_EXTID
      hdr.ch_extid  = 0;
#endif
      hdr.ch_tcf    = 0;

      can_receive(dev, &hdr, data);
    }

  /* Clear esr1 flags */

  if (esr1flags != 0)
      putreg32(esr1flags, priv->base + FC7300_CAN_ESR1_OFFSET);

#ifdef CONFIG_FC7300_FLEXCAN_ENHANCED_RX_FIFO

  /* Clear erfsr flags */

  if (erfsrflags != 0)
      putreg32(erfsrflags, priv->base + FC7300_CAN_ERFSR_OFFSET);

#endif
}
#endif

/****************************************************************************
 * Function: fc7300_rxmb_interrupt
 *
 * Description:
 *   FlexCAN rx message buffer interrupt handler
 *
 * Input Parameters:
 *   dev  - Reference to the can device structure.
 *   mb_index - The RX mailbox number.
 *
 * Returned Value:
 *   Zero on success; a negated errno on failure
 *
 ****************************************************************************/

__ramfunc__ static int fc7300_rxmb_interrupt(struct can_dev_s *dev, int mb_index)
{
#ifdef CONFIG_CAN_TIMESTAMP
  clock_t clkval;
  struct timespec ts;
#endif
  uint32_t i;
  uint32_t len;
  struct fc7300_flexcan_priv_s *priv;
  struct can_hdr_s hdr =
  {
    0
  };

  uint8_t data[CAN_MAXDATALEN] =
  {
    0
  };

#ifdef CONFIG_CAN_TIMESTAMP
  clkval = up_perf_gettime();
  up_perf_convert(clkval, &ts);
#endif

  priv = (struct fc7300_flexcan_priv_s *)dev->cd_priv;

#ifdef CONFIG_CAN_FD
  if (priv->canfd_en)
    {
      struct canfdmb_s *rf = &((struct canfdmb_s *)priv->rx)[mb_index];

      /* Read the frame contents */

      if (rf->cs.edl) /* CAN FD frame */
        {
#ifdef CONFIG_CAN_EXTID
          if (rf->cs.ide)
            {
              hdr.ch_id = MASKEXTID & rf->id.ext;
              hdr.ch_extid = true;
            }
          else
            {
              hdr.ch_id = MASKSTDID & rf->id.std;
              hdr.ch_extid = false;
            }
#else
          if (rf->cs.ide)
            {
              canerr("ERROR: Received message with extended identifier.\n");
            }

          hdr.ch_id = MASKSTDID & rf->id.std;
#endif

          hdr.ch_dlc = rf->cs.dlc;
          hdr.ch_rtr = rf->cs.rtr;
          hdr.ch_brs = rf->cs.brs;
          hdr.ch_edl = rf->cs.edl;

          len = g_fc7300_can_dlc_to_len[rf->cs.dlc];
          for (i = 0; i < (len + 4 - 1) / 4; i++)
            {
              *(uint32_t *)&data[i * 4] = bswap_32(rf->data[i].w00);
            }
        }
      else
        {
#ifdef CONFIG_CAN_EXTID
          if (rf->cs.ide)
            {
              hdr.ch_id = MASKEXTID & rf->id.ext;
              hdr.ch_extid = true;
            }
          else
            {
              hdr.ch_id = MASKSTDID & rf->id.std;
              hdr.ch_extid = false;
            }
#else
          if (rf->cs.ide)
            {
              canerr("ERROR: Received message with extended identifier.\n");
            }

          hdr.ch_id = MASKSTDID & rf->id.std;
#endif

          hdr.ch_dlc = rf->cs.dlc;
          hdr.ch_rtr = rf->cs.rtr;

          *(uint32_t *)&data[0] = bswap_32(rf->data[0].w00);
          *(uint32_t *)&data[4] = bswap_32(rf->data[1].w00);
        }
    }
  else
#endif
    {
      struct canmb_s *rf = &((struct canmb_s *)priv->rx)[mb_index];

      /* Read the frame contents */

#ifdef CONFIG_CAN_EXTID
      if (rf->cs.ide)
        {
          hdr.ch_id = MASKEXTID & rf->id.ext;
          hdr.ch_extid = true;
        }
      else
        {
          hdr.ch_id = MASKSTDID & rf->id.std;
          hdr.ch_extid = false;
        }
#else
      if (rf->cs.ide)
        {
          canerr("ERROR: Received message with extended identifier.\n");
        }

      hdr.ch_id = MASKSTDID & rf->id.std;
#endif

      hdr.ch_dlc = rf->cs.dlc;
      hdr.ch_rtr = rf->cs.rtr;

      *(uint32_t *)&data[0] = bswap_32(rf->data[0].w00);
      *(uint32_t *)&data[4] = bswap_32(rf->data[1].w00);
    }

#ifdef CONFIG_CAN_TIMESTAMP
    hdr.ch_ts.tv_sec = ts.tv_sec;
    hdr.ch_ts.tv_usec = ts.tv_nsec / 1000u;
#endif

  /* Provide the data to the upper half driver */

  can_receive(dev, &hdr, data);

  /* Read the free run timer to unlock the mb */

  (void)getreg32(priv->base + FC7300_CAN_TIMER_OFFSET);

  return 0;
}

/****************************************************************************
 * Function: fc7300_erf_interrupt
 *
 * Description:
 *   FlexCAN enhanced rx fifo interrupt handler
 *
 * Input Parameters:
 *   dev  - Reference to the can device structure.
 *
 * Returned Value:
 *   Zero on success; a negated errno on failure
 *
 ****************************************************************************/

__ramfunc__ static int fc7300_erf_interrupt(struct can_dev_s *dev)
{
#ifdef CONFIG_CAN_TIMESTAMP
  clock_t clkval;
  struct timespec ts;
#endif
  uint32_t i;
  uint32_t len;
  uint32_t regval;
  struct fc7300_flexcan_priv_s *priv;
  struct can_hdr_s hdr =
  {
    0
  };

  uint8_t data[CAN_MAXDATALEN] =
  {
    0
  };

#ifdef CONFIG_CAN_TIMESTAMP
  clkval = up_perf_gettime();
  up_perf_convert(clkval, &ts);
#endif

  priv = (struct fc7300_flexcan_priv_s *)dev->cd_priv;

#ifdef CONFIG_CAN_FD
  if (priv->canfd_en)
    {
      struct canfderf_s *erf = (struct canfderf_s *)(priv->base +
                                FC7300_CAN_ERX_FIFO_OFFSET);

      /* Read the frame contents */

      if (erf->cs.edl) /* CAN FD frame */
        {
#ifdef CONFIG_CAN_EXTID
          if (erf->cs.ide)
            {
              hdr.ch_id = MASKEXTID & erf->id.ext;
              hdr.ch_extid = true;
            }
          else
            {
              hdr.ch_id = MASKSTDID & erf->id.std;
              hdr.ch_extid = false;
            }
#else
          if (erf->cs.ide)
            {
              canerr("ERROR: Received message with extended identifier.\n");
            }

          hdr.ch_id = MASKSTDID & erf->id.std;
#endif

          hdr.ch_dlc = erf->cs.dlc;
          hdr.ch_rtr = erf->cs.rtr;
          hdr.ch_brs = erf->cs.brs;
          hdr.ch_edl = erf->cs.edl;

          len = g_fc7300_can_dlc_to_len[erf->cs.dlc];
          for (i = 0; i < (len + 4 - 1) / 4; i++)
            {
              *(uint32_t *)&data[i * 4] =
                            bswap_32(erf->data[i].w00);
            }
        }
      else
        {
#ifdef CONFIG_CAN_EXTID
          if (erf->cs.ide)
            {
              hdr.ch_id = MASKEXTID & erf->id.ext;
              hdr.ch_extid = true;
            }
          else
            {
              hdr.ch_id = MASKSTDID & erf->id.std;
              hdr.ch_extid = false;
            }
#else
          if (erf->cs.ide)
            {
              canerr("ERROR: Received message with extended identifier.\n");
            }

          hdr.ch_id = MASKSTDID & erf->id.std;
#endif

          hdr.ch_dlc = erf->cs.dlc;
          hdr.ch_rtr = erf->cs.rtr;

          *(uint32_t *)&data[0] = bswap_32(erf->data[0].w00);
          *(uint32_t *)&data[4] = bswap_32(erf->data[1].w00);
        }
    }
  else
#endif
    {
      struct canerf_s *erf = (struct canerf_s *)(priv->base +
                              FC7300_CAN_ERX_FIFO_OFFSET);

      /* Read the frame contents */

#ifdef CONFIG_CAN_EXTID
      if (erf->cs.ide)
        {
          hdr.ch_id = MASKEXTID & erf->id.ext;
          hdr.ch_extid = true;
        }
      else
        {
          hdr.ch_id = MASKSTDID & erf->id.std;
          hdr.ch_extid = false;
        }
#else
      if (erf->cs.ide)
        {
          canerr("ERROR: Received message with extended identifier.\n");
        }

      hdr.ch_id = MASKSTDID & erf->id.std;
#endif

      hdr.ch_dlc = erf->cs.dlc;
      hdr.ch_rtr = erf->cs.rtr;

      *(uint32_t *)&data[0] = bswap_32(erf->data[0].w00);
      *(uint32_t *)&data[4] = bswap_32(erf->data[1].w00);
    }

#ifdef CONFIG_CAN_TIMESTAMP
    hdr.ch_ts.tv_sec = ts.tv_sec;
    hdr.ch_ts.tv_usec = ts.tv_nsec / 1000u;
#endif

  /* Provide the data to the upper half driver */

  can_receive(dev, &hdr, data);

  /* Clear ERF interrupt flag */

  regval  = getreg32(priv->base + FC7300_CAN_ERFSR_OFFSET);
  regval &= 0x0fffffff;
  regval |= (CAN_ERFSR_ERFDA);
  putreg32(regval, priv->base + FC7300_CAN_ERFSR_OFFSET);

  return 0;
}

#ifdef CONFIG_FC7300_FLEXCAN_ISR_THREAD
/****************************************************************************
 * Function: fc7300_flexcan_isr
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
 *   IRQ_WAKE_THREAD to wake up the thread.
 *
 ****************************************************************************/

static int fc7300_flexcan_isr(int irq, void *context, void *arg)
{
  (void)context;
  (void)arg;
  up_disable_irq(irq);
  return IRQ_WAKE_THREAD;
}
#endif

#ifdef CONFIG_CAN_TXCONFIRM
/****************************************************************************
 * Function: fc7300_flexcan_tx_confirm
 *
 * Description:
 *   Get the can id when tx interrupt occured.
 *
 * Input Parameters:
 *   dev  - Reference to the can device structure.
 *   mb_index - The TX mailbox number.
 *
 * Returned Value:
 *   OK(0) on success; Negated errno on failure.
 *
 ****************************************************************************/

__ramfunc__ static uint32_t fc7300_flexcan_tx_confirm(struct can_dev_s *dev,
                                          int mb_index)
{
  struct fc7300_flexcan_priv_s *priv;
  struct can_hdr_s hdr =
  {
    0
  };

  /* fill candriver txconfirmation msg */

  hdr.ch_dlc = 0;
  hdr.ch_tcf = 1;

  priv = (struct fc7300_flexcan_priv_s *)dev->cd_priv;

#ifdef CONFIG_CAN_FD
  if (priv->canfd_en)
    {
      struct canfdmb_s *tf = &((struct canfdmb_s *)priv->tx)[mb_index];

      if (tf->cs.code != CAN_TXMB_INACTIVE)
          return OK;

#ifdef CONFIG_CAN_EXTID
      if (tf->cs.ide)
        {
          hdr.ch_id = MASKEXTID & tf->id.ext;
          hdr.ch_extid = true;
        }
      else
        {
          hdr.ch_id = MASKSTDID & tf->id.std;
          hdr.ch_extid = false;
        }
#else
      hdr.ch_id = MASKSTDID & tf->id.std;
#endif
    }
  else
#endif
    {
      struct canmb_s *tf = &((struct canmb_s *)priv->tx)[mb_index];

      if (tf->cs.code != CAN_TXMB_INACTIVE)
          return OK;

#ifdef CONFIG_CAN_EXTID
      if (tf->cs.ide)
        {
          hdr.ch_id = MASKEXTID & tf->id.ext;
          hdr.ch_extid = true;
        }
      else
        {
          hdr.ch_id = MASKSTDID & tf->id.std;
          hdr.ch_extid = false;
        }
#else
      hdr.ch_id = MASKSTDID & tf->id.std;
#endif
    }

  return can_receive(dev, &hdr, NULL);
}
#endif

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
 ****************************************************************************/

__ramfunc__ static int fc7300_flexcan_interrupt(int irq, void *context, void *arg)
{
  uint32_t flags;
  uint32_t fflags;
  uint32_t mb_masks;
  uint32_t erf_masks;
  uint32_t mb_index;
  struct can_dev_s *dev;
  struct fc7300_flexcan_priv_s *priv;
#if defined(CONFIG_CAN_ERRORS)
  struct flexcan_errinfo errinfo;
#endif

  dev = (struct can_dev_s *)arg;
  priv = (struct fc7300_flexcan_priv_s *)dev->cd_priv;

  if (irq == priv->config->irq)
    {
#if defined(CONFIG_CAN_ERRORS)
      /* Handle can bus off interrupt */

      fc7300_flexcan_geterrinfo(priv->base, &errinfo);
      fc7300_flexcan_handle_error(dev, &errinfo, FLEXCAN_ERROR_INTERRUPT);
#endif

      mb_masks = getreg32(priv->base + FC7300_CAN_IMASK1_OFFSET);

      flags = getreg32(priv->base + FC7300_CAN_IFLAG1_OFFSET);
      flags = flags & IFLAG1_RX;
      fflags = flags & mb_masks;

      if (flags != fflags)
          canerr("ERROR: RXMB interrupt mask is disabled,"
                 "flags = %lu, fflags = %lu\n", flags, fflags);

      if (fflags)
        {
          while ((mb_index = arm_lsb(fflags)) != 32)
            {
              putreg32(1 << mb_index, priv->base + FC7300_CAN_IFLAG1_OFFSET);
              fc7300_rxmb_interrupt(dev, mb_index - priv->txmb_cnt);
              fflags &= ~(1 << mb_index);
            }
        }

      erf_masks = getreg32(priv->base + FC7300_CAN_ERFIER_OFFSET);
      erf_masks = erf_masks & CAN_ERFIER_ERFDAIE;

      flags = getreg32(priv->base + FC7300_CAN_ERFSR_OFFSET);
      flags = flags & CAN_ERFSR_ERFDA;
      fflags = flags & erf_masks;

      if (flags != fflags)
          canerr("ERROR: ERF interrupt mask is disabled,"
                 "flags = %lu, fflags = %lu\n", flags, fflags);

      if (fflags)
        {
          fc7300_erf_interrupt(dev);
        }

      flags = getreg32(priv->base + FC7300_CAN_IFLAG1_OFFSET);
      flags = flags & IFLAG1_TX;
      fflags = flags & mb_masks;

      if (flags != fflags)
          canerr("ERROR: TXMB interrupt mask is disabled,"
                 "flags = %lu, fflags = %lu\n", flags, fflags);

      if (fflags)
        {
          while ((mb_index = arm_lsb(fflags)) != 32)
            {
#ifdef CONFIG_CAN_TXCONFIRM
              fc7300_flexcan_tx_confirm(dev, mb_index);
#endif

              putreg32((1 << mb_index), priv->base + FC7300_CAN_IFLAG1_OFFSET);
              priv->txmb_sflags &= ~(1 << mb_index);
              can_txdone(dev);
              fflags &= ~(1 << mb_index);
            }
        }
    }

#ifdef CONFIG_FC7300_FLEXCAN_ISR_THREAD
  fc7300_clearpendingisr(irq);
  up_enable_irq(irq);
#endif

  return OK;
}

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
 *   Zero on success; a negated errno on failure
 *
 ****************************************************************************/

static int fc7300_flexcan_init_eccram(struct fc7300_flexcan_priv_s *priv)
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

#ifdef CONFIG_CAN_FD
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
 *   Zero on success; a negated errno on failure
 *
 ****************************************************************************/

static int fc7300_flexcan_initialize(struct fc7300_flexcan_priv_s *priv)
{
  uint32_t i;
  int ret;
  uint32_t regval;
  uint32_t ndtseg1;

  ret = fc7300_flexcan_setenable(priv->base, 0);
  if (ret < 0)
    {
      canerr("ERROR: Disable flexcan failed, base = 0x%08" PRIx16 "\n",
            (unsigned int)priv->base);
      return -EAGAIN;
    }

  /* Always use PCC as clock src */

  regval  = getreg32(priv->base + FC7300_CAN_CTRL1_OFFSET);
  regval &= ~CAN_CTRL1_CLKSRC;
  putreg32(regval, priv->base + FC7300_CAN_CTRL1_OFFSET);

  /* Initialize CAN device */

  ret = fc7300_flexcan_setenable(priv->base, 1);
  if (ret < 0)
    {
      canerr("ERROR: Enable flexcan failed, base = 0x%08" PRIx16 "\n",
            (unsigned int)priv->base);
      return -EAGAIN;
    }

  /* Enter freeze mode */

  ret = fc7300_flexcan_setfreeze(priv->base, 1);
  if (ret < 0)
    {
      canerr("ERROR: Set freeze mode failed, base = 0x%08" PRIx16 "\n",
            (unsigned int)priv->base);
      return -EAGAIN;
    }

  fc7300_flexcan_init_eccram(priv);

  fc7300_flexcan_reset(priv);

  /* Reset CTRL1 register to reset value */

  regval  = getreg32(priv->base + FC7300_CAN_CTRL1_OFFSET);
  regval &= ~(CAN_CTRL1_LOM | CAN_CTRL1_LBUF | CAN_CTRL1_TSYN |
              CAN_CTRL1_BOFFREC | CAN_CTRL1_SMP | CAN_CTRL1_RWRNMSK |
              CAN_CTRL1_TWRNMSK | CAN_CTRL1_LPB | CAN_CTRL1_ERRMSK |
              CAN_CTRL1_BOFFMSK);
  putreg32(regval, priv->base + FC7300_CAN_CTRL1_OFFSET);

#ifdef CONFIG_CAN_FD
  if (priv->canfd_en)
    {
      regval  = getreg32(priv->base + FC7300_CAN_CTRL1_OFFSET);
      regval |= CAN_CTRL1_BOFFREC |   /* Always disable bus off auto recovery */
#ifdef CONFIG_FC7300_FLEXCAN_CAN_ERROR_INTERRUPT
                CAN_CTRL1_ERRMSK  |   /* Enable error interrupt */
#endif
                CAN_CTRL1_BOFFMSK;    /* Always enable bus off interrupt */

      putreg32(regval, priv->base + FC7300_CAN_CTRL1_OFFSET);

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
                CAN_FDCTRL_MBDSR0(priv->data_size / 8 - 1);
      putreg32(regval, priv->base + FC7300_CAN_FDCTRL_OFFSET);

#ifdef CONFIG_FC7300_FLEXCAN_CANFD_SSP
      if (priv->ssp_en)
        {
          regval = CAN_ETDC_ETDCEN | CAN_ETDC_ETDCOFF(priv->ssp_offset);
          putreg32(regval, priv->base + FC7300_CAN_ETDC_OFFSET);
        }
#endif
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
               CAN_ENCBT_NTSEG2(priv->arbi_timing.pseg2 - 1) |
               CAN_ENCBT_NRJW(priv->arbi_timing.sjw - 1);
      putreg32(regval, priv->base + FC7300_CAN_ENCBT_OFFSET);

      regval  = getreg32(priv->base + FC7300_CAN_CTRL1_OFFSET);
      regval |= CAN_CTRL1_BOFFREC |   /* Always disable bus off auto recovery */
#ifdef CONFIG_FC7300_FLEXCAN_CAN_ERROR_INTERRUPT
                CAN_CTRL1_ERRMSK  |   /* Enable error interrupt */
#endif
                CAN_CTRL1_BOFFMSK;    /* Always enable bus off interrupt */

      putreg32(regval, priv->base + FC7300_CAN_CTRL1_OFFSET);
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
      regval |= CAN_ERFCR_NFE(priv->erfel_total_cnt - 1) | \
                CAN_ERFCR_NEXIF(priv->erfel_extid_cnt);
      regval |= CAN_ERFCR_ERFWM(0);                 /* Enhanced Rx FIFO Watermaker = 1 */
      putreg32(regval, priv->base + FC7300_CAN_ERFCR_OFFSET);

      for (i = 0; i < priv->erfel_extid_cnt; i++)
        {
          regval = ((priv->erf_filter[i].format & 0x3) << 30) | \
                   ((priv->erf_filter[i].filter.can_id) << 0);
          putreg32(regval, priv->base + FC7300_CAN_ERFFEL_OFFSET + i * 8);
          regval = ((priv->erf_filter[i].filter.can_mask) << 0);
          putreg32(regval, priv->base + FC7300_CAN_ERFFEL_OFFSET + i * 8 + \
                           4);
        }

      for (i = priv->erfel_extid_cnt; i < (priv->erfel_total_cnt * 2 - \
                                           priv->erfel_extid_cnt); i++)
        {
          regval = ((priv->erf_filter[i].format & 0x3) << 30) | \
                   ((priv->erf_filter[i].filter.can_id) << 16) | \
                     priv->erf_filter[i].filter.can_mask;
          putreg32(regval, priv->base + FC7300_CAN_ERFFEL_OFFSET + \
                           priv->erfel_extid_cnt * 8  + \
                           (i - priv->erfel_extid_cnt) * 4);
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
#ifdef CONFIG_CAN_EXTID
      putreg32((priv->rxmb_filter[i].can_mask & MASKEXTID),
                priv->base + FC7300_CAN_RXIMR_OFFSET(i + priv->txmb_cnt));
#else
      putreg32((priv->rxmb_filter[i].can_mask & MASKSTDID) << 18,
                priv->base + FC7300_CAN_RXIMR_OFFSET(i + priv->txmb_cnt));
#endif
    }

#ifdef CONFIG_CAN_FD
  if (priv->canfd_en)
    {
      struct canfdmb_s *mb = (struct canfdmb_s *)priv->rx;
      for (i = 0; i < priv->rxmb_cnt; i++)
        {
          /* If the fields are configured in separate writes,
           * the MB_CS[CODE] must be the last write in the C/S word.
           */

          mb[i].id.w    = 0x0;
#ifdef CONFIG_CAN_EXTID
          mb[i].id.ext  = priv->rxmb_filter[i].can_id & MASKEXTID;
#else
          mb[i].id.std  = priv->rxmb_filter[i].can_id & MASKSTDID;
#endif
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
          /* If the fields are configured in separate writes,
           * the MB_CS[CODE] must be the last write in the C/S word.
           */

          mb[i].id.w    = 0x0;
#ifdef CONFIG_CAN_EXTID
          mb[i].id.ext  = priv->rxmb_filter[i].can_id & MASKEXTID;
#else
          mb[i].id.std  = priv->rxmb_filter[i].can_id & MASKSTDID;
#endif
          mb[i].cs.edl  = 0x1;
          mb[i].cs.brs  = 0x1;
          mb[i].cs.esi  = 0x0;
          mb[i].cs.srr  = 0x0;
          mb[i].cs.ide  = 0x1;
          mb[i].cs.rtr  = 0x0;
          mb[i].cs.code = 0x4;
        }
    }

  /* Exit freeze mode */

  ret = fc7300_flexcan_setfreeze(priv->base, 0);
  if (ret < 0)
    {
      canerr("ERROR: Exit freeze mode failed, base = 0x%08" PRIx16 "\n",
            (unsigned int)priv->base);
      return -EAGAIN;
    }

  return 0;
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
 ****************************************************************************/

static void fc7300_flexcan_reset(struct fc7300_flexcan_priv_s *priv)
{
  uint32_t i;
  uint32_t regval;
  uint32_t total_mb_cnt;

  total_mb_cnt = priv->txmb_cnt + priv->rxmb_cnt;

  regval  = getreg32(priv->base + FC7300_CAN_MCR_OFFSET);
  regval |= CAN_MCR_SOFTRST;
  putreg32(regval, priv->base + FC7300_CAN_MCR_OFFSET);

  if (!fc7300_flexcan_waitmcr_change(priv->base, CAN_MCR_SOFTRST, 0))
    {
      canerr("ERROR: Reset failed, base = 0x%08" PRIx16 "\n",
            (unsigned int)priv->base);
      return;
    }

  regval  = getreg32(priv->base + FC7300_CAN_MCR_OFFSET);
  regval &= ~(CAN_MCR_SUPV);
  putreg32(regval, priv->base + FC7300_CAN_MCR_OFFSET);

  /* Initialize all MB rx and tx */

#ifdef CONFIG_CAN_FD
  if (priv->canfd_en)
    {
      uint32_t j;
      struct canfdmb_s *mb = (struct canfdmb_s *)priv->rx;

      for (i = 0; i < total_mb_cnt; i++)
        {
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
            CAN_MCR_IRMQ | CAN_MCR_AEN | CAN_MCR_MAXMB(total_mb_cnt - 1);
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
 * Function: fc7300_chardriver_caninitialize
 *
 * Description:
 *   Initialize the CAN controller and driver
 *
 * Input Parameters:
 *   intf        - In the case where there are multiple CAN devices,
 *                 this value identifies which CAN device is to be
 *                 initialized.
 *   rxmb_filter - RXMB filter array.
 *   erf_filter  - Enhaned RX FIFO filter array.
 *
 * Returned Value:
 *   Valid CAN device structure reference on success; a NULL on failure.
 *
 ****************************************************************************/

struct can_dev_s *fc7300_chardriver_caninitialize(const int intf, \
                  const struct can_filter *rxmb_filter, \
                  const struct can_erf_filter *erf_filter)
{
  struct can_dev_s *dev;
  struct fc7300_flexcan_priv_s *priv;
  const struct flexcan_config_s *config;

  switch (intf)
    {
#ifdef CONFIG_FC7300_FLEXCAN0_CHARDRIVER
    case 0:
      dev = &g_fc7300_flexcan0dev;
      break;
#endif

#ifdef CONFIG_FC7300_FLEXCAN1_CHARDRIVER
    case 1:
      dev = &g_fc7300_flexcan1dev;
      break;
#endif

#ifdef CONFIG_FC7300_FLEXCAN2_CHARDRIVER
    case 2:
      dev = &g_fc7300_flexcan2dev;
      break;
#endif

#ifdef CONFIG_FC7300_FLEXCAN3_CHARDRIVER
    case 3:
      dev = &g_fc7300_flexcan3dev;
      break;
#endif

#ifdef CONFIG_FC7300_FLEXCAN4_CHARDRIVER
    case 4:
      dev = &g_fc7300_flexcan4dev;
      break;
#endif

#ifdef CONFIG_FC7300_FLEXCAN5_CHARDRIVER
    case 5:
      dev = &g_fc7300_flexcan5dev;
      break;
#endif

#ifdef CONFIG_FC7300_FLEXCAN6_CHARDRIVER
    case 6:
      dev = &g_fc7300_flexcan6dev;
      break;
#endif

#ifdef CONFIG_FC7300_FLEXCAN7_CHARDRIVER
    case 7:
      dev = &g_fc7300_flexcan7dev;
      break;
#endif

#ifdef CONFIG_FC7300_FLEXCAN8_CHARDRIVER
    case 8:
      dev = &g_fc7300_flexcan8dev;
      break;
#endif

#ifdef CONFIG_FC7300_FLEXCAN9_CHARDRIVER
    case 9:
      dev = &g_fc7300_flexcan9dev;
      break;
#endif

    default:
      return NULL;
    }

  priv = (struct fc7300_flexcan_priv_s *)dev->cd_priv;
  priv->rxmb_filter = rxmb_filter;
#ifdef CONFIG_FC7300_FLEXCAN_ENHANCED_RX_FIFO
  if (priv->erf_en)
    priv->erf_filter  = erf_filter;
#endif

  config = priv->config;
  fc7300_pinconfig(config->tx_pin);
  fc7300_pinconfig(config->rx_pin);

  if (config->irq > 0)
    {
      /* Attach interrupt handler. */

#ifdef CONFIG_FC7300_FLEXCAN_ISR_THREAD
      if (irq_attach_wqueue(config->irq, fc7300_flexcan_isr,
                     fc7300_flexcan_interrupt, dev,
                     isr_cfg[FLEXCAN_ISR_THREAD]))
#else
      if (irq_attach(config->irq, fc7300_flexcan_interrupt, dev))
#endif
        {
          canerr("ERROR: Failed to attach FLEXCAN IRQ, base = 0x%08" PRIx16 "\n",
                (unsigned int)priv->base);
          return NULL;
        }
    }

  /* MB5 can't be use as TXMB. */

  if (priv->txmb_cnt > 5)
      priv->txmb_cnt = 5;

  /* Fix errata, RXMB locate in the head, TXMB locate in the tail. */

  priv->tx = (uint32_t *)(priv->base + FC7300_CAN_MB_OFFSET);

#ifdef CONFIG_CAN_FD
  if (priv->canfd_en)
    {
      priv->rx = (uint32_t *)(priv->base + FC7300_CAN_MB_OFFSET +
                 (sizeof(struct canfdmb_s) * priv->txmb_cnt));
    }
  else
#endif
    {
      priv->rx = (uint32_t *)(priv->base + FC7300_CAN_MB_OFFSET +
                 (sizeof(struct canmb_s) * priv->txmb_cnt));
    }

  priv->txmb_sflags = 0;
  priv->state = CAN_STATE_STOP;

  return dev;
}

#endif
