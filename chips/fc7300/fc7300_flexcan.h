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

#ifndef __VENDOR_FLAGCHIP_CHIPS_FC7300_FC7300_FLEXCAN_H
#define __VENDOR_FLAGCHIP_CHIPS_FC7300_FC7300_FLEXCAN_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>

#include "hardware/fc7300_flexcan.h"

#ifdef CONFIG_FC7300_FLEXCAN

/* CAN Timeout */

#define CAN_TIMEOUT_CYCLE 1500000U

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#if defined(CONFIG_FC7300_FLEXCAN0_CANFD_DATA_SIZE_8)
#  define FLEXCAN0_CANFD_DATA_SIZE 8
#elif defined(CONFIG_FC7300_FLEXCAN0_CANFD_DATA_SIZE_16)
#  define FLEXCAN0_CANFD_DATA_SIZE 16
#elif defined(CONFIG_FC7300_FLEXCAN0_CANFD_DATA_SIZE_32)
#  define FLEXCAN0_CANFD_DATA_SIZE 32
#elif defined(CONFIG_FC7300_FLEXCAN0_CANFD_DATA_SIZE_64)
#  define FLEXCAN0_CANFD_DATA_SIZE 64
#endif

#if defined(CONFIG_FC7300_FLEXCAN1_CANFD_DATA_SIZE_8)
#  define FLEXCAN1_CANFD_DATA_SIZE 8
#elif defined(CONFIG_FC7300_FLEXCAN1_CANFD_DATA_SIZE_16)
#  define FLEXCAN1_CANFD_DATA_SIZE 16
#elif defined(CONFIG_FC7300_FLEXCAN1_CANFD_DATA_SIZE_32)
#  define FLEXCAN1_CANFD_DATA_SIZE 32
#elif defined(CONFIG_FC7300_FLEXCAN1_CANFD_DATA_SIZE_64)
#  define FLEXCAN1_CANFD_DATA_SIZE 64
#endif

#if defined(CONFIG_FC7300_FLEXCAN2_CANFD_DATA_SIZE_8)
#  define FLEXCAN2_CANFD_DATA_SIZE 8
#elif defined(CONFIG_FC7300_FLEXCAN2_CANFD_DATA_SIZE_16)
#  define FLEXCAN2_CANFD_DATA_SIZE 16
#elif defined(CONFIG_FC7300_FLEXCAN2_CANFD_DATA_SIZE_32)
#  define FLEXCAN2_CANFD_DATA_SIZE 32
#elif defined(CONFIG_FC7300_FLEXCAN2_CANFD_DATA_SIZE_64)
#  define FLEXCAN2_CANFD_DATA_SIZE 64
#endif

#if defined(CONFIG_FC7300_FLEXCAN3_CANFD_DATA_SIZE_8)
#  define FLEXCAN3_CANFD_DATA_SIZE 8
#elif defined(CONFIG_FC7300_FLEXCAN3_CANFD_DATA_SIZE_16)
#  define FLEXCAN3_CANFD_DATA_SIZE 16
#elif defined(CONFIG_FC7300_FLEXCAN3_CANFD_DATA_SIZE_32)
#  define FLEXCAN3_CANFD_DATA_SIZE 32
#elif defined(CONFIG_FC7300_FLEXCAN3_CANFD_DATA_SIZE_64)
#  define FLEXCAN3_CANFD_DATA_SIZE 64
#endif

#if defined(CONFIG_FC7300_FLEXCAN4_CANFD_DATA_SIZE_8)
#  define FLEXCAN4_CANFD_DATA_SIZE 8
#elif defined(CONFIG_FC7300_FLEXCAN4_CANFD_DATA_SIZE_16)
#  define FLEXCAN4_CANFD_DATA_SIZE 16
#elif defined(CONFIG_FC7300_FLEXCAN4_CANFD_DATA_SIZE_32)
#  define FLEXCAN4_CANFD_DATA_SIZE 32
#elif defined(CONFIG_FC7300_FLEXCAN4_CANFD_DATA_SIZE_64)
#  define FLEXCAN4_CANFD_DATA_SIZE 64
#endif

#if defined(CONFIG_FC7300_FLEXCAN5_CANFD_DATA_SIZE_8)
#  define FLEXCAN5_CANFD_DATA_SIZE 8
#elif defined(CONFIG_FC7300_FLEXCAN5_CANFD_DATA_SIZE_16)
#  define FLEXCAN5_CANFD_DATA_SIZE 16
#elif defined(CONFIG_FC7300_FLEXCAN5_CANFD_DATA_SIZE_32)
#  define FLEXCAN5_CANFD_DATA_SIZE 32
#elif defined(CONFIG_FC7300_FLEXCAN5_CANFD_DATA_SIZE_64)
#  define FLEXCAN5_CANFD_DATA_SIZE 64
#endif

#if defined(CONFIG_FC7300_FLEXCAN6_CANFD_DATA_SIZE_8)
#  define FLEXCAN6_CANFD_DATA_SIZE 8
#elif defined(CONFIG_FC7300_FLEXCAN6_CANFD_DATA_SIZE_16)
#  define FLEXCAN6_CANFD_DATA_SIZE 16
#elif defined(CONFIG_FC7300_FLEXCAN6_CANFD_DATA_SIZE_32)
#  define FLEXCAN6_CANFD_DATA_SIZE 32
#elif defined(CONFIG_FC7300_FLEXCAN6_CANFD_DATA_SIZE_64)
#  define FLEXCAN6_CANFD_DATA_SIZE 64
#endif

#if defined(CONFIG_FC7300_FLEXCAN7_CANFD_DATA_SIZE_8)
#  define FLEXCAN7_CANFD_DATA_SIZE 8
#elif defined(CONFIG_FC7300_FLEXCAN7_CANFD_DATA_SIZE_16)
#  define FLEXCAN7_CANFD_DATA_SIZE 16
#elif defined(CONFIG_FC7300_FLEXCAN7_CANFD_DATA_SIZE_32)
#  define FLEXCAN7_CANFD_DATA_SIZE 32
#elif defined(CONFIG_FC7300_FLEXCAN7_CANFD_DATA_SIZE_64)
#  define FLEXCAN7_CANFD_DATA_SIZE 64
#endif

#if defined(CONFIG_FC7300_FLEXCAN8_CANFD_DATA_SIZE_8)
#  define FLEXCAN8_CANFD_DATA_SIZE 8
#elif defined(CONFIG_FC7300_FLEXCAN8_CANFD_DATA_SIZE_16)
#  define FLEXCAN8_CANFD_DATA_SIZE 16
#elif defined(CONFIG_FC7300_FLEXCAN8_CANFD_DATA_SIZE_32)
#  define FLEXCAN8_CANFD_DATA_SIZE 32
#elif defined(CONFIG_FC7300_FLEXCAN8_CANFD_DATA_SIZE_64)
#  define FLEXCAN8_CANFD_DATA_SIZE 64
#endif

#if defined(CONFIG_FC7300_FLEXCAN9_CANFD_DATA_SIZE_8)
#  define FLEXCAN9_CANFD_DATA_SIZE 8
#elif defined(CONFIG_FC7300_FLEXCAN9_CANFD_DATA_SIZE_16)
#  define FLEXCAN9_CANFD_DATA_SIZE 16
#elif defined(CONFIG_FC7300_FLEXCAN9_CANFD_DATA_SIZE_32)
#  define FLEXCAN9_CANFD_DATA_SIZE 32
#elif defined(CONFIG_FC7300_FLEXCAN9_CANFD_DATA_SIZE_64)
#  define FLEXCAN9_CANFD_DATA_SIZE 64
#endif

#define FLEXCAN0_RXMB_COUNT CONFIG_FC7300_FLEXCAN0_RXMB_COUNT
#define FLEXCAN1_RXMB_COUNT CONFIG_FC7300_FLEXCAN1_RXMB_COUNT
#define FLEXCAN2_RXMB_COUNT CONFIG_FC7300_FLEXCAN2_RXMB_COUNT
#define FLEXCAN3_RXMB_COUNT CONFIG_FC7300_FLEXCAN3_RXMB_COUNT
#define FLEXCAN4_RXMB_COUNT CONFIG_FC7300_FLEXCAN4_RXMB_COUNT
#define FLEXCAN5_RXMB_COUNT CONFIG_FC7300_FLEXCAN5_RXMB_COUNT
#define FLEXCAN6_RXMB_COUNT CONFIG_FC7300_FLEXCAN6_RXMB_COUNT
#define FLEXCAN7_RXMB_COUNT CONFIG_FC7300_FLEXCAN7_RXMB_COUNT
#define FLEXCAN8_RXMB_COUNT CONFIG_FC7300_FLEXCAN8_RXMB_COUNT
#define FLEXCAN9_RXMB_COUNT CONFIG_FC7300_FLEXCAN9_RXMB_COUNT

#if defined(CONFIG_FC7300_FLEXCAN0_CANFD)
#  define FLEXCAN0_TXMB_COUNT ((512/(FLEXCAN0_CANFD_DATA_SIZE+8))-CONFIG_FC7300_FLEXCAN0_RXMB_COUNT)
#else
#  define FLEXCAN0_TXMB_COUNT (32-CONFIG_FC7300_FLEXCAN0_RXMB_COUNT)
#endif

#if defined(CONFIG_FC7300_FLEXCAN1_CANFD)
#  define FLEXCAN1_TXMB_COUNT ((512/(FLEXCAN1_CANFD_DATA_SIZE+8))-CONFIG_FC7300_FLEXCAN1_RXMB_COUNT)
#else
#  define FLEXCAN1_TXMB_COUNT (32-CONFIG_FC7300_FLEXCAN1_RXMB_COUNT)
#endif

#if defined(CONFIG_FC7300_FLEXCAN2_CANFD)
#  define FLEXCAN2_TXMB_COUNT ((512/(FLEXCAN2_CANFD_DATA_SIZE+8))-CONFIG_FC7300_FLEXCAN2_RXMB_COUNT)
#else
#  define FLEXCAN2_TXMB_COUNT (32-CONFIG_FC7300_FLEXCAN2_RXMB_COUNT)
#endif

#if defined(CONFIG_FC7300_FLEXCAN3_CANFD)
#  define FLEXCAN3_TXMB_COUNT ((512/(FLEXCAN3_CANFD_DATA_SIZE+8))-CONFIG_FC7300_FLEXCAN3_RXMB_COUNT)
#else
#  define FLEXCAN3_TXMB_COUNT (32-CONFIG_FC7300_FLEXCAN3_RXMB_COUNT)
#endif

#if defined(CONFIG_FC7300_FLEXCAN4_CANFD)
#  define FLEXCAN4_TXMB_COUNT ((512/(FLEXCAN4_CANFD_DATA_SIZE+8))-CONFIG_FC7300_FLEXCAN4_RXMB_COUNT)
#else
#  define FLEXCAN4_TXMB_COUNT (32-CONFIG_FC7300_FLEXCAN4_RXMB_COUNT)
#endif

#if defined(CONFIG_FC7300_FLEXCAN5_CANFD)
#  define FLEXCAN5_TXMB_COUNT ((512/(FLEXCAN5_CANFD_DATA_SIZE+8))-CONFIG_FC7300_FLEXCAN5_RXMB_COUNT)
#else
#  define FLEXCAN5_TXMB_COUNT (32-CONFIG_FC7300_FLEXCAN5_RXMB_COUNT)
#endif

#if defined(CONFIG_FC7300_FLEXCAN6_CANFD)
#  define FLEXCAN6_TXMB_COUNT ((512/(FLEXCAN6_CANFD_DATA_SIZE+8))-CONFIG_FC7300_FLEXCAN6_RXMB_COUNT)
#else
#  define FLEXCAN6_TXMB_COUNT (32-CONFIG_FC7300_FLEXCAN6_RXMB_COUNT)
#endif

#if defined(CONFIG_FC7300_FLEXCAN7_CANFD)
#  define FLEXCAN7_TXMB_COUNT ((512/(FLEXCAN7_CANFD_DATA_SIZE+8))-CONFIG_FC7300_FLEXCAN7_RXMB_COUNT)
#else
#  define FLEXCAN7_TXMB_COUNT (32-CONFIG_FC7300_FLEXCAN7_RXMB_COUNT)
#endif

#if defined(CONFIG_FC7300_FLEXCAN8_CANFD)
#  define FLEXCAN8_TXMB_COUNT ((512/(FLEXCAN8_CANFD_DATA_SIZE+8))-CONFIG_FC7300_FLEXCAN8_RXMB_COUNT)
#else
#  define FLEXCAN8_TXMB_COUNT (32-CONFIG_FC7300_FLEXCAN8_RXMB_COUNT)
#endif

#if defined(CONFIG_FC7300_FLEXCAN9_CANFD)
#  define FLEXCAN9_TXMB_COUNT ((512/(FLEXCAN9_CANFD_DATA_SIZE+8))-CONFIG_FC7300_FLEXCAN9_RXMB_COUNT)
#else
#  define FLEXCAN9_TXMB_COUNT (32-CONFIG_FC7300_FLEXCAN9_RXMB_COUNT)
#endif

/****************************************************************************
 * Public Types
 ****************************************************************************/

typedef enum
{
  ID_MASK_FORMAT  = 0,
  ID_RANGE_FORMAT = 1,
  TWO_ID_FORMAT   = 2,
}erfff_t;

struct can_erf_filter
{
  erfff_t format;
  struct can_filter filter;
};

union cs_e
{
  volatile uint32_t cs;
  struct
  {
    volatile uint32_t time_stamp : 16;
    volatile uint32_t dlc : 4;
    volatile uint32_t rtr : 1;
    volatile uint32_t ide : 1;
    volatile uint32_t srr : 1;
    volatile uint32_t res : 1;
    volatile uint32_t code : 4;
    volatile uint32_t res2 : 1;
    volatile uint32_t esi : 1;
    volatile uint32_t brs : 1;
    volatile uint32_t edl : 1;
  };
};

union id_e
{
  volatile uint32_t w;
  struct
  {
    volatile uint32_t ext : 29;
    volatile uint32_t resex : 3;
  };
  struct
  {
    volatile uint32_t res : 18;
    volatile uint32_t std : 11;
    volatile uint32_t resstd : 3;
  };
};

union data_e
{
  volatile uint32_t w00;
  struct
  {
    volatile uint32_t b03 : 8;
    volatile uint32_t b02 : 8;
    volatile uint32_t b01 : 8;
    volatile uint32_t b00 : 8;
  };
};

struct canmb_s
{
  union cs_e cs;
  union id_e id;
  union data_e data[2];
};

union erf_cs_e
{
  volatile uint32_t cs;
  struct
  {
    volatile uint32_t time_stamp : 16;
    volatile uint32_t dlc : 4;
    volatile uint32_t rtr : 1;
    volatile uint32_t ide : 1;
    volatile uint32_t srr : 1;
    volatile uint32_t res : 6;
    volatile uint32_t esi : 1;
    volatile uint32_t brs : 1;
    volatile uint32_t edl : 1;
  };
};

union erf_id_e
{
  volatile uint32_t w;
  struct
  {
    volatile uint32_t ext : 29;
    volatile uint32_t resex : 3;
  };
  struct
  {
    volatile uint32_t res : 18;
    volatile uint32_t std : 11;
    volatile uint32_t resstd : 3;
  };
};

struct canerf_s
{
  union erf_cs_e cs;
  union erf_id_e id;
  union data_e data[2];
};

struct canfdmb_s
{
  union cs_e cs;
  union id_e id;
  union data_e data[16];
};

struct canfderf_s
{
  union erf_cs_e cs;
  union erf_id_e id;
  union data_e data[16];
};

/* FlexCAN Device hardware configuration */

struct flexcan_config_s
{
  uint32_t tx_pin;           /* GPIO configuration for TX */
  uint32_t rx_pin;           /* GPIO configuration for RX */
  uint32_t no_buffers;       /* Number of message buffers */
  uint32_t irq;              /* IRQ */
  uint32_t irq_pri;          /* IRQ Priority */
};

/* FlexCAN bit timing configs */

struct flexcan_timeseg
{
  uint32_t bitrate;
  int32_t samplep;
  uint8_t propseg;
  uint8_t pseg1;
  uint8_t pseg2;
  uint8_t presdiv;
  uint8_t sjw;
};

struct flexcan_errinfo
{
  uint32_t rxerrcnt_fast;  /* Receive Error Counter for fast bits  */
  uint32_t rxerrcnt;       /* Receive Error Counter                */
  uint32_t txerrcnt_fast;  /* Transmit Error Counter for fast bits */
  uint32_t txerrcnt;       /* Transmit Error Counter               */
  uint32_t esr1value;      /* Error Register ESR1 Value            */
  uint32_t errsrvalue;     /* Error Register ERRSR Value           */
  uint32_t rerrarvalue;    /* Error Register RERRAR Value          */
  uint32_t rerrdrvalue;    /* Error Register RERRDR Value          */
  uint32_t rerrsynrvalue;  /* Error Register RERRSYNR Value */
#ifdef CONFIG_FC7300_FLEXCAN_ENHANCED_RX_FIFO
  uint32_t erfsrvalue;     /* Error Register ERFSR Value */
#endif
};

enum flexcan_errmode
{
  FLEXCAN_ERROR_POLLING   = 0,
  FLEXCAN_ERROR_INTERRUPT = 1,
  FLEXCAN_ERROR_BOTH      = 2,
};

/****************************************************************************
 * Public Function Prototypes
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
 * Function: fc7300_flexcan_waitmcr_change
 *
 * Description:
 *   Wait the CAN controller mcr register mode change done.
 *
 * Input Parameters:
 *   base - CAN controller register base address.
 *   mask - The excepted mode to change.
 *   target_state - Target mode state.
 *
 * Returned Value:
 *   TRUE on success; FALSE on failure.
 *
 ****************************************************************************/

uint32_t fc7300_flexcan_waitmcr_change(uint32_t base, uint32_t mask,
                               uint32_t target_state);

/****************************************************************************
 * Function: fc7300_flexcan_setfreeze
 *
 * Description:
 *   Enter or leave freeze mode.
 *
 * Input Parameters:
 *   base - CAN controller register base address.
 *   freeze - Enter or leave flag.
 *
 * Returned Value:
 *   Zeor on success, -1 on failure.
 *
 ****************************************************************************/

int fc7300_flexcan_setfreeze(uint32_t base, uint32_t freeze);

/****************************************************************************
 * Function: fc7300_flexcan_setenable
 *
 * Description:
 *   Enable or disable CAN controller.
 *
 * Input Parameters:
 *   base - CAN controller register base address.
 *   enable - Enable or disable flag.
 *
 * Returned Value:
 *   NONE
 *
 ****************************************************************************/

int fc7300_flexcan_setenable(uint32_t base, uint32_t enable);

#if defined(CONFIG_NET_CAN)
/****************************************************************************
 * Function: fc7300_socket_caninitialize
 *
 * Description:
 *   Initialize the CAN controller and driver as socket device.
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

int fc7300_socket_caninitialize(int intf);
#endif

#if defined(CONFIG_CAN)
/****************************************************************************
 * Function: fc7300_chardriver_caninitialize
 *
 * Description:
 *   Initialize the CAN controller and driver as char driver device.
 *
 * Input Parameters:
 *   intf - In the case where there are multiple CAN devices, this value
 *          identifies which CAN device is to be initialized.
 *
 * Returned Value:
 *   Valid CAN device structure reference on success; a NULL on failure.
 *
 * Assumptions:
 *
 ****************************************************************************/

struct can_dev_s *fc7300_chardriver_caninitialize(const int intf,
                                const struct can_filter *rxmb_filter, \
                                const struct can_erf_filter *erf_filter);
#endif

#undef EXTERN
#if defined(__cplusplus)
}
#endif

#endif /* __ASSEMBLY__ */
#endif /* CONFIG_FC7300_FLEXCAN */
#endif /* __VENDOR_FLAGCHIP_CHIPS_FC7300_FC7300_FLEXCAN_H */
