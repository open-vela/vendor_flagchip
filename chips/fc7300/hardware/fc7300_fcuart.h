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

#ifndef __VENDOR_FLAGCHIP_CHIPS_FC7300_HARDWARE_FC7300_FCUART_H
#define __VENDOR_FLAGCHIP_CHIPS_FC7300_HARDWARE_FC7300_FCUART_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>
#include "hardware/fc7300_memorymap.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Register offsets *********************************************************/

#define FC7300_FCUART_RST_OFFSET        0x0008  /* Software Reset Register */
#define FC7300_FCUART_PIN_CFG_OFFSET    0x000c  /* Pin Configuration Register */
#define FC7300_FCUART_BAUD_OFFSET       0x0010  /* Baud Rate Register */
#define FC7300_FCUART_STAT_OFFSET       0x0014  /* Status Register */
#define FC7300_FCUART_CTRL_OFFSET       0x0018  /* Control Register */
#define FC7300_FCUART_DATA_OFFSET       0x001c  /* Data Register */
#define FC7300_FCUART_MATCH_OFFSET      0x0020  /* Match Address Register */
#define FC7300_FCUART_MODIR_OFFSET      0x0024  /* Modem IrDA Register */
#define FC7300_FCUART_FIFO_OFFSET       0x0028  /* FIFO Register */
#define FC7300_FCUART_WATERMARK_OFFSET  0x002c  /* Watermark Register */

/* Register addresses *******************************************************/

#define FC7300_FCUART0_RST              (FC7300_FCUART0_BASE + FC7300_FCUART_RST_OFFSET)
#define FC7300_FCUART0_PIN_CFG          (FC7300_FCUART0_BASE + FC7300_FCUART_PIN_CFG_OFFSET)
#define FC7300_FCUART0_BAUD             (FC7300_FCUART0_BASE + FC7300_FCUART_BAUD_OFFSET)
#define FC7300_FCUART0_STAT             (FC7300_FCUART0_BASE + FC7300_FCUART_STAT_OFFSET)
#define FC7300_FCUART0_CTRL             (FC7300_FCUART0_BASE + FC7300_FCUART_CTRL_OFFSET)
#define FC7300_FCUART0_DATA             (FC7300_FCUART0_BASE + FC7300_FCUART_DATA_OFFSET)
#define FC7300_FCUART0_MATCH            (FC7300_FCUART0_BASE + FC7300_FCUART_MATCH_OFFSET)
#define FC7300_FCUART0_MODIR            (FC7300_FCUART0_BASE + FC7300_FCUART_MODIR_OFFSET)
#define FC7300_FCUART0_FIFO             (FC7300_FCUART0_BASE + FC7300_FCUART_FIFO_OFFSET)
#define FC7300_FCUART0_WATERMARK        (FC7300_FCUART0_BASE + FC7300_FCUART_WATERMARK_OFFSET)

#define FC7300_FCUART1_RST              (FC7300_FCUART1_BASE + FC7300_FCUART_RST_OFFSET)
#define FC7300_FCUART1_PIN_CFG          (FC7300_FCUART1_BASE + FC7300_FCUART_PIN_CFG_OFFSET)
#define FC7300_FCUART1_BAUD             (FC7300_FCUART1_BASE + FC7300_FCUART_BAUD_OFFSET)
#define FC7300_FCUART1_STAT             (FC7300_FCUART1_BASE + FC7300_FCUART_STAT_OFFSET)
#define FC7300_FCUART1_CTRL             (FC7300_FCUART1_BASE + FC7300_FCUART_CTRL_OFFSET)
#define FC7300_FCUART1_DATA             (FC7300_FCUART1_BASE + FC7300_FCUART_DATA_OFFSET)
#define FC7300_FCUART1_MATCH            (FC7300_FCUART1_BASE + FC7300_FCUART_MATCH_OFFSET)
#define FC7300_FCUART1_MODIR            (FC7300_FCUART1_BASE + FC7300_FCUART_MODIR_OFFSET)
#define FC7300_FCUART1_FIFO             (FC7300_FCUART1_BASE + FC7300_FCUART_FIFO_OFFSET)
#define FC7300_FCUART1_WATERMARK        (FC7300_FCUART1_BASE + FC7300_FCUART_WATERMARK_OFFSET)

#define FC7300_FCUART2_RST              (FC7300_FCUART2_BASE + FC7300_FCUART_RST_OFFSET)
#define FC7300_FCUART2_PIN_CFG          (FC7300_FCUART2_BASE + FC7300_FCUART_PIN_CFG_OFFSET)
#define FC7300_FCUART2_BAUD             (FC7300_FCUART2_BASE + FC7300_FCUART_BAUD_OFFSET)
#define FC7300_FCUART2_STAT             (FC7300_FCUART2_BASE + FC7300_FCUART_STAT_OFFSET)
#define FC7300_FCUART2_CTRL             (FC7300_FCUART2_BASE + FC7300_FCUART_CTRL_OFFSET)
#define FC7300_FCUART2_DATA             (FC7300_FCUART2_BASE + FC7300_FCUART_DATA_OFFSET)
#define FC7300_FCUART2_MATCH            (FC7300_FCUART2_BASE + FC7300_FCUART_MATCH_OFFSET)
#define FC7300_FCUART2_MODIR            (FC7300_FCUART2_BASE + FC7300_FCUART_MODIR_OFFSET)
#define FC7300_FCUART2_FIFO             (FC7300_FCUART2_BASE + FC7300_FCUART_FIFO_OFFSET)
#define FC7300_FCUART2_WATERMARK        (FC7300_FCUART2_BASE + FC7300_FCUART_WATERMARK_OFFSET)

#define FC7300_FCUART3_RST              (FC7300_FCUART3_BASE + FC7300_FCUART_RST_OFFSET)
#define FC7300_FCUART3_PIN_CFG          (FC7300_FCUART3_BASE + FC7300_FCUART_PIN_CFG_OFFSET)
#define FC7300_FCUART3_BAUD             (FC7300_FCUART3_BASE + FC7300_FCUART_BAUD_OFFSET)
#define FC7300_FCUART3_STAT             (FC7300_FCUART3_BASE + FC7300_FCUART_STAT_OFFSET)
#define FC7300_FCUART3_CTRL             (FC7300_FCUART3_BASE + FC7300_FCUART_CTRL_OFFSET)
#define FC7300_FCUART3_DATA             (FC7300_FCUART3_BASE + FC7300_FCUART_DATA_OFFSET)
#define FC7300_FCUART3_MATCH            (FC7300_FCUART3_BASE + FC7300_FCUART_MATCH_OFFSET)
#define FC7300_FCUART3_MODIR            (FC7300_FCUART3_BASE + FC7300_FCUART_MODIR_OFFSET)
#define FC7300_FCUART3_FIFO             (FC7300_FCUART3_BASE + FC7300_FCUART_FIFO_OFFSET)
#define FC7300_FCUART3_WATERMARK        (FC7300_FCUART3_BASE + FC7300_FCUART_WATERMARK_OFFSET)

#define FC7300_FCUART4_RST              (FC7300_FCUART4_BASE + FC7300_FCUART_RST_OFFSET)
#define FC7300_FCUART4_PIN_CFG          (FC7300_FCUART4_BASE + FC7300_FCUART_PIN_CFG_OFFSET)
#define FC7300_FCUART4_BAUD             (FC7300_FCUART4_BASE + FC7300_FCUART_BAUD_OFFSET)
#define FC7300_FCUART4_STAT             (FC7300_FCUART4_BASE + FC7300_FCUART_STAT_OFFSET)
#define FC7300_FCUART4_CTRL             (FC7300_FCUART4_BASE + FC7300_FCUART_CTRL_OFFSET)
#define FC7300_FCUART4_DATA             (FC7300_FCUART4_BASE + FC7300_FCUART_DATA_OFFSET)
#define FC7300_FCUART4_MATCH            (FC7300_FCUART4_BASE + FC7300_FCUART_MATCH_OFFSET)
#define FC7300_FCUART4_MODIR            (FC7300_FCUART4_BASE + FC7300_FCUART_MODIR_OFFSET)
#define FC7300_FCUART4_FIFO             (FC7300_FCUART4_BASE + FC7300_FCUART_FIFO_OFFSET)
#define FC7300_FCUART4_WATERMARK        (FC7300_FCUART4_BASE + FC7300_FCUART_WATERMARK_OFFSET)

#define FC7300_FCUART5_RST              (FC7300_FCUART5_BASE + FC7300_FCUART_RST_OFFSET)
#define FC7300_FCUART5_PIN_CFG          (FC7300_FCUART5_BASE + FC7300_FCUART_PIN_CFG_OFFSET)
#define FC7300_FCUART5_BAUD             (FC7300_FCUART5_BASE + FC7300_FCUART_BAUD_OFFSET)
#define FC7300_FCUART5_STAT             (FC7300_FCUART5_BASE + FC7300_FCUART_STAT_OFFSET)
#define FC7300_FCUART5_CTRL             (FC7300_FCUART5_BASE + FC7300_FCUART_CTRL_OFFSET)
#define FC7300_FCUART5_DATA             (FC7300_FCUART5_BASE + FC7300_FCUART_DATA_OFFSET)
#define FC7300_FCUART5_MATCH            (FC7300_FCUART5_BASE + FC7300_FCUART_MATCH_OFFSET)
#define FC7300_FCUART5_MODIR            (FC7300_FCUART5_BASE + FC7300_FCUART_MODIR_OFFSET)
#define FC7300_FCUART5_FIFO             (FC7300_FCUART5_BASE + FC7300_FCUART_FIFO_OFFSET)
#define FC7300_FCUART5_WATERMARK        (FC7300_FCUART5_BASE + FC7300_FCUART_WATERMARK_OFFSET)

/* Register bit definitions *************************************************/

/* FCUART Software Reset Register */

                                                 /* Bit 0:  Reserved */
#define FCUART_RST_RST                  (1 << 1) /* Bit 1: Software Reset */
                                                 /* Bits 2-31:  Reserved */

/* FCUART Pin Configuration Register */

#define FCUART_PIN_CFG_TRGSEL_SHIFT    (0)       /* Bits 0-1:  Trigger Select */
#define FCUART_PIN_CFG_TRGSEL_MASK     (3 << FCUART_PIN_CFG_TRGSEL_SHIFT)
#define FCUART_PIN_CFG_TRGSEL_DISABLE  (0 << FCUART_PIN_CFG_TRGSEL_SHIFT) /* Trigger disabled */
#define FCUART_PIN_CFG_TRGSEL_RXD      (1 << FCUART_PIN_CFG_TRGSEL_SHIFT) /* Trigger used instead of RXD pin */
#define FCUART_PIN_CFG_TRGSEL_CTSB     (2 << FCUART_PIN_CFG_TRGSEL_SHIFT) /* Trigger used instead of CTS_B pin */
#define FCUART_PIN_CFG_TRGSEL_TXDMOD   (3 << FCUART_PIN_CFG_TRGSEL_SHIFT) /* Trigger used to modulate the TXD output */

                                                 /* Bits 2-31:  Reserved */

/* FCUART Baud Rate Register */

#define FCUART_BAUD_SBR_SHIFT          (0)       /* Bits 0-12: Baud Rate Modulo Divisor. */
#define FCUART_BAUD_SBR_MASK           (0x1fff << FCUART_BAUD_SBR_SHIFT)
#define FCUART_BAUD_SBR(n)             (((uint32_t)(n) << FCUART_BAUD_SBR_SHIFT) & FCUART_BAUD_SBR_MASK)
#define FCUART_BAUD_SBNS               (1 << 13) /* Bit 13: Stop Bit Number Select */
#define FCUART_BAUD_RIAEIE             (1 << 14) /* Bit 14: RX Input Active Edge Interrupt Enable */
#define FCUART_BAUD_LBKDIE             (1 << 15) /* Bit 15: LIN Break Detect Interrupt Enable */
#define FCUART_BAUD_RESYNC_DIS         (1 << 16) /* Bit 16: Resynchronization Disable */
#define FCUART_BAUD_BEDGE_SAMP         (1 << 17) /* Bit 17: Both Edge Sampling */
#define FCUART_BAUD_MATCH_CFG_SHIFT    (18)      /* Bits 18-19: Match Configuration */
#define FCUART_BAUD_MATCH_CFG_MASK     (3 << FCUART_BAUD_MATCH_CFG_SHIFT)
#define FCUART_BAUD_MATCH_CFG_ADDR     (0 << FCUART_BAUD_MATCH_CFG_SHIFT)  /* Address Match Wakeup */
#define FCUART_BAUD_MATCH_CFG_IDLE     (1 << FCUART_BAUD_MATCH_CFG_SHIFT)  /* Idle Match Wakeup */
#define FCUART_BAUD_MATCH_CFG_ALT      (2 << FCUART_BAUD_MATCH_CFG_SHIFT)  /* Alternate Match */
#define FCUART_BAUD_MATCH_CFG_RWCEN    (3 << FCUART_BAUD_MATCH_CFG_SHIFT)  /* Enables RWC on Data Match and Match
                                                                            * On/Off for transmitter CTS input */

#define FCUART_BAUD_RIDMAEN            (1 << 20) /* Bit 20: Receiver Idle DMA Enable */
#define FCUART_BAUD_RDMAEN             (1 << 21) /* Bit 21: Receiver Full DMA Enable */
                                                 /* Bit 22: Reserved */
#define FCUART_BAUD_TDMAEN             (1 << 23) /* Bit 23: Transmitter DMA Enable */
#define FCUART_BAUD_OVR_SAMP_SHIFT     (24)      /* Bits 24-28: Oversampling Ratio */
#define FCUART_BAUD_OVR_SAMP_MASK      (0x1F << FCUART_BAUD_OVR_SAMP_SHIFT)
#define FCUART_BAUD_OVR_SAMP(n)        (((uint32_t)(n) << FCUART_BAUD_OVR_SAMP_SHIFT) & FCUART_BAUD_OVR_SAMP_MASK)

#define FCUART_BAUD_10BIT_MODE         (1 << 29) /* Bit 20: 10-bit Mode select */
#define FCUART_BAUD_MAEN1              (1 << 30) /* Bit 30: Match Address Mode Enable 1 */
#define FCUART_BAUD_MAEN0              (1 << 31) /* Bit 31: Match Address Mode Enable 0 */

/* FCUART Status Register */

                                                 /* Bits 0-1:  Reserved */
#define FCUART_STAT_TPEF               (1 << 2)  /* Bit 2:  Transmit Data Parity Error Flag */
#define FCUART_STAT_RPEF               (1 << 3)  /* Bit 3:  Receive Data Parity Error Flag */
#define FCUART_STAT_M1F                (1 << 14) /* Bit 14: Match Address 1 Flag */
#define FCUART_STAT_M0F                (1 << 15) /* Bit 15: Match Address 0 Flag */
#define FCUART_STAT_PEF                (1 << 16) /* Bit 16: Parity Error Flag */
#define FCUART_STAT_FEF                (1 << 17) /* Bit 17: Framing Error Flag */
#define FCUART_STAT_NF                 (1 << 18) /* Bit 18: Noise Flag */
#define FCUART_STAT_RORF               (1 << 19) /* Bit 19: Receiver Overrun Flag */
#define FCUART_STAT_IDLEF              (1 << 20) /* Bit 20: Idle Line Flag */
#define FCUART_STAT_RDRFF              (1 << 21) /* Bit 21: Receive Data Register Full Flag */
#define FCUART_STAT_TCF                (1 << 22) /* Bit 22: Transmission Complete Flag */
#define FCUART_STAT_TDREF              (1 << 23) /* Bit 23: Transmit Data Register Empty Flag */
#define FCUART_STAT_RAF                (1 << 24) /* Bit 24: Receiver Active Flag */
#define FCUART_STAT_LBKDE              (1 << 25) /* Bit 25: LIN Break Detection Enable */
#define FCUART_STAT_BCGL_SHIFT         (26)      /* Bit 26: Break Character Generation Length */
#define FCUART_STAT_BCGL_MASK          (1 << FCUART_STAT_BCGL_SHIFT)
#define FCUART_STAT_BCGL_9_13          (0 << FCUART_STAT_BCGL_SHIFT) /* Set break character length
                                                                      * to 9 to 13 bit times */
#define FCUART_STAT_BCGL_12_15         (1 << FCUART_STAT_BCGL_SHIFT) /* Set break character length
                                                                      * to 12 to 15 bit times */
#define FCUART_STAT_RWUID              (1 << 27)                     /* Bit 27: Receive Wake Up Idle Detect */
#define FCUART_STAT_RXINV              (1 << 28)                     /* Bit 28: Receive Data Inversion */
#define FCUART_STAT_MSBF               (1 << 29)                     /* Bit 29: MSB First */
#define FCUART_STAT_RPAEIF             (1 << 30)                     /* Bit 30: RXD Pin Active Edge Interrupt Flag */
#define FCUART_STAT_LBKDIF             (1 << 31)                     /* Bit 31: LIN Break Detect Interrupt Flag */

/* FCUART Control Register */

#define FCUART_CTRL_PT                 (1 << 0)  /* Bit 0:  Parity Type */
#define FCUART_CTRL_PT_EVEN            (0 << 0)  /*         Even parity */
#define FCUART_CTRL_PT_ODD             (1 << 0)  /*         Odd parity */
#define FCUART_CTRL_PE                 (1 << 1)  /* Bit 1:  Parity Enable */
#define FCUART_CTRL_ITS                (1 << 2)  /* Bit 2:  Idle Line Type Select */
#define FCUART_CTRL_RSWMS              (1 << 3)  /* Bit 3:  Receiver Wakeup Method Select */
#define FCUART_CTRL_BMSEL_SHIFT        (4)       /* Bit 4:  9-Bit or 8-Bit Mode Select */
#define FCUART_CTRL_BMSEL_MASK         (1 << FCUART_CTRL_BMSEL_SHIFT)
#define FCUART_CTRL_BMSEL_8BIT         (0 << FCUART_CTRL_BMSEL_SHIFT) /* 8-Bit Mode */
#define FCUART_CTRL_BMSEL_9BIT         (1 << FCUART_CTRL_BMSEL_SHIFT) /* 9-Bit Mode */
#define FCUART_CTRL_RXSRC              (1 << 5)                       /* Bit 5:  Receiver Source Select */
#define FCUART_CTRL_WAITEN             (1 << 6)                       /* Bit 6:  Wait Mode Enable */
#define FCUART_CTRL_LOOPMS             (1 << 7)                       /* Bit 7:  Loop Mode Select */
#define FCUART_CTRL_IDLECFG_SHIFT      (8)                            /* Bits 8-10: Idle Configuration */
#define FCUART_CTRL_IDLECFG_MASK       (7 << FCUART_CTRL_IDLECFG_SHIFT)
#define FCUART_CTRL_IDLECFG_1          (0 << FCUART_CTRL_IDLECFG_SHIFT) /* 1 idle character */
#define FCUART_CTRL_IDLECFG_2          (1 << FCUART_CTRL_IDLECFG_SHIFT) /* 2 idle characters */
#define FCUART_CTRL_IDLECFG_4          (2 << FCUART_CTRL_IDLECFG_SHIFT) /* 4 idle characters */
#define FCUART_CTRL_IDLECFG_8          (3 << FCUART_CTRL_IDLECFG_SHIFT) /* 8 idle characters */
#define FCUART_CTRL_IDLECFG_16         (4 << FCUART_CTRL_IDLECFG_SHIFT) /* 6 idle characters */
#define FCUART_CTRL_IDLECFG_32         (5 << FCUART_CTRL_IDLECFG_SHIFT) /* 32 idle characters */
#define FCUART_CTRL_IDLECFG_64         (6 << FCUART_CTRL_IDLECFG_SHIFT) /* 64 idle characters */
#define FCUART_CTRL_IDLECFG_128        (7 << FCUART_CTRL_IDLECFG_SHIFT) /* 128 idle characters */

#define FCUART_CTRL_7BMS               (1 << 11) /* Bit 11: 7-Bit Mode Select */
                                                 /* Bits 12-13:  Reserved */
#define FCUART_CTRL_M1IE               (1 << 14) /* Bit 14: Match Address 1 Interrupt Enable */
#define FCUART_CTRL_M0IE               (1 << 15) /* Bit 15: Match Address 0 Interrupt Enable */
#define FCUART_CTRL_SBK                (1 << 16) /* Bit 16: Send Break */
#define FCUART_CTRL_RWC                (1 << 17) /* Bit 17: Receiver Wakeup Control */
#define FCUART_CTRL_RE                 (1 << 18) /* Bit 18: Receiver Enable */
#define FCUART_CTRL_TE                 (1 << 19) /* Bit 19: Transmitter Enable */
#define FCUART_CTRL_IIE                (1 << 20) /* Bit 20: Idle Line Interrupt Enable */
#define FCUART_CTRL_RIE                (1 << 21) /* Bit 21: Receiver Interrupt Enable */
#define FCUART_CTRL_TCIE               (1 << 22) /* Bit 22: Transmission Complete Interrupt Enable */
#define FCUART_CTRL_TIE                (1 << 23) /* Bit 23: Transmit Interrupt Enable */
#define FCUART_CTRL_PEIE               (1 << 24) /* Bit 24: Parity Error Interrupt Enable */
#define FCUART_CTRL_FEIE               (1 << 25) /* Bit 25: Framing Error Interrupt Enable */
#define FCUART_CTRL_NEIE               (1 << 26) /* Bit 26: Noise Error Interrupt Enable */
#define FCUART_CTRL_ORIE               (1 << 27) /* Bit 27: Overrun Interrupt Enable */
#define FCUART_CTRL_TXINV              (1 << 28) /* Bit 28: Transmit Data Inversion */
#define FCUART_CTRL_TXDIR              (1 << 29) /* Bit 29: TXD Pin Direction in Single-Wire Mode */
#define FCUART_CTRL_R9T8               (1 << 30) /* Bit 30: Receive Bit 9 / Transmit Bit 8 */
#define FCUART_CTRL_R8T9               (1 << 31) /* Bit 31: Receive Bit 8 / Transmit Bit 9 */

#define FCUART_ALL_INTS (FCUART_CTRL_ORIE | FCUART_CTRL_NEIE | FCUART_CTRL_FEIE |  \
                         FCUART_CTRL_PEIE | FCUART_CTRL_TIE  | FCUART_CTRL_TCIE |  \
                         FCUART_CTRL_RIE  | FCUART_CTRL_IIE | FCUART_CTRL_M0IE |  \
                         FCUART_CTRL_M1IE)

/* FCUART Data Register */

#define FCUART_DATA_RXTXDATA_SHIFT     (0)       /* Bits 0-9: Data bits 0-9 */
#define FCUART_DATA_RXTXDATA_MASK      (0x3ff << FCUART_DATA_RXTXDATA_SHIFT)
                                                 /* Bit 10:  Reserved */
#define FCUART_DATA_STATUS_SHIFT       (11)      /* Bits 11-15: Receive data status */
#define FCUART_DATA_STATUS_MASK        (0x1f << FCUART_DATA_STATUS_SHIFT)
#define FCUART_DATA_IDLE               (1 << 11) /* Bit 11: Idle Line */
#define FCUART_DATA_RXEMPT             (1 << 12) /* Bit 12: Receive Buffer Empty */
#define FCUART_DATA_FETSC              (1 << 13) /* Bit 13: Frame Error / Transmit Special Character */
#define FCUART_DATA_PARITYE            (1 << 14) /* Bit 14: Parity Error */
#define FCUART_DATA_NOISY              (1 << 15) /* Bit 15: Noisy */
                                                 /* Bits 16-31:  Reserved */

/* FCUART Match Address Register */

#define FCUART_MATCH_MATCH0_SHIFT      (0)       /* Bits 0-9: Match Address 0 */
#define FCUART_MATCH_MATCH0_MASK       (0x3ff << FCUART_MATCH_MATCH0_SHIFT)
#define FCUART_MATCH_MATCH0(n)         (((uint32_t)(n) << FCUART_MATCH_MATCH0_SHIFT) & FCUART_MATCH_MATCH0_MASK)
                                                 /* Bits 10-15:  Reserved */
#define FCUART_MATCH_MATCH1_SHIFT      (16)      /* Bits 16-25: Match Address 1 */
#define FCUART_MATCH_MATCH1_MASK       (0x3ff << FCUART_MATCH_MATCH1_SHIFT)
#define FCUART_MATCH_MATCH1(n)         (((uint32_t)(n) << FCUART_MATCH_MATCH1_SHIFT) & FCUART_MATCH_MATCH1_MASK)
                                                 /* Bits 26-31:  Reserved */

/* FCUART Modem IrDA Register */

#define FCUART_MODIR_TXCTSEN           (1 << 0)  /* Bit 0:  Transmitter clear-to-send enable */
#define FCUART_MODIR_TXRTSEN           (1 << 1)  /* Bit 1:  Transmitter request-to-send enable */
#define FCUART_MODIR_TXRTSPOL          (1 << 2)  /* Bit 2:  Transmitter request-to-send polarity */
#define FCUART_MODIR_RXRTSEN           (1 << 3)  /* Bit 3:  Receiver request-to-send enable */
#define FCUART_MODIR_TXCTSCFG          (1 << 4)  /* Bit 4:  Transmit CTS Configuration */
#define FCUART_MODIR_TXCTSCFG_START    (0 << 4)  /*         CTS sampled at start of character */
#define FCUART_MODIR_TXCTSCFG_IDLE     (1 << 4)  /*         CTS sampled when transmitter idle */
#define FCUART_MODIR_TXCTSSRC          (1 << 5)  /* Bit 5:  Transmit CTS Source */
#define FCUART_MODIR_TXCTSSRC_CTSB     (0 << 5)  /*         CTS input is CTS_B pin */
#define FCUART_MODIR_TXCTSSRC_RXMAT    (1 << 5)  /*         Transmit CTS Source */
                                                 /* Bits 6-7:  Reserved */
#define FCUART_MODIR_RXRTSCFG_SHIFT    (8)       /* Bits 8-9: Receive RTS Configuration */
#define FCUART_MODIR_RXRTSCFG_MASK     (3 << FCUART_MODIR_RXRTSCFG_SHIFT)
#define FCUART_MODIR_RXRTSCFG(n)       (((uint32_t)(n) << FCUART_MODIR_RXRTSCFG_SHIFT) & FCUART_MODIR_RXRTSCFG_MASK)
                                                 /* Bits 10-31:  Reserved */

/* FCUART FIFO Register */

#define FCUART_FIFO_RXFIFODEP_SHIFT    (0)       /* Bits 0-2: Receive FIFO. Buffer Depth */
#define FCUART_FIFO_RXFIFODEP_MASK     (7 << FCUART_FIFO_RXFIFODEP_SHIFT)
#define FCUART_FIFO_RXFIFODEP_1        (0 << FCUART_FIFO_RXFIFODEP_SHIFT) /* 1 datawords */
#define FCUART_FIFO_RXFIFODEP_4        (1 << FCUART_FIFO_RXFIFODEP_SHIFT) /* 4 datawords */
#define FCUART_FIFO_RXFIFODEP_8        (2 << FCUART_FIFO_RXFIFODEP_SHIFT) /* 8 datawords */
#define FCUART_FIFO_RXFIFODEP_16       (3 << FCUART_FIFO_RXFIFODEP_SHIFT) /* 16 datawords */
#define FCUART_FIFO_RXFIFODEP_32       (4 << FCUART_FIFO_RXFIFODEP_SHIFT) /* 32 datawords */
#define FCUART_FIFO_RXFIFODEP_64       (5 << FCUART_FIFO_RXFIFODEP_SHIFT) /* 64 datawords */
#define FCUART_FIFO_RXFIFODEP_128      (6 << FCUART_FIFO_RXFIFODEP_SHIFT) /* 128 datawords */
#define FCUART_FIFO_RXFIFODEP_256      (7 << FCUART_FIFO_RXFIFODEP_SHIFT) /* 256 datawords */

#define FCUART_FIFO_RXFEN              (1 << 3)  /* Bit 3:  Receive FIFO Enable */
#define FCUART_FIFO_TXFIFODEP_SHIFT    (4)       /* Bits 4-6: Transmit FIFO. Buffer Depth */
#define FCUART_FIFO_TXFIFODEP_MASK     (7 << FCUART_FIFO_TXFIFODEP_SHIFT)
#define FCUART_FIFO_TXFIFODEP_1        (0 << FCUART_FIFO_TXFIFODEP_SHIFT) /* 1 datawords */
#define FCUART_FIFO_TXFIFODEP_4        (1 << FCUART_FIFO_TXFIFODEP_SHIFT) /* 4 datawords */
#define FCUART_FIFO_TXFIFODEP_8        (2 << FCUART_FIFO_TXFIFODEP_SHIFT) /* 8 datawords */
#define FCUART_FIFO_TXFIFODEP_16       (3 << FCUART_FIFO_TXFIFODEP_SHIFT) /* 16 datawords */
#define FCUART_FIFO_TXFIFODEP_32       (4 << FCUART_FIFO_TXFIFODEP_SHIFT) /* 32 datawords */
#define FCUART_FIFO_TXFIFODEP_64       (5 << FCUART_FIFO_TXFIFODEP_SHIFT) /* 64 datawords */
#define FCUART_FIFO_TXFIFODEP_128      (6 << FCUART_FIFO_TXFIFODEP_SHIFT) /* 128 datawords */
#define FCUART_FIFO_TXFIFODEP_256      (7 << FCUART_FIFO_TXFIFODEP_SHIFT) /* 256 datawords */

#define FCUART_FIFO_TXFEN              (1 << 7)  /* Bit 7:  Transmit FIFO Enable */
#define FCUART_FIFO_RXUFIE             (1 << 8)  /* Bit 8:  Receive FIFO Underflow Interrupt Enable */
#define FCUART_FIFO_TXOFIE             (1 << 9)  /* Bit 9:  Transmit FIFO Overflow Interrupt Enable */
#define FCUART_FIFO_RXIDEN_SHIFT       (10)      /* Bits 10-12:  Receiver Idle Empty Enable */
#define FCUART_FIFO_RXIDEN_MASK        (7 << FCUART_FIFO_RXIDEN_SHIFT)
#define FCUART_FIFO_RXIDEN_DISABLE     (0 << FCUART_FIFO_RXIDEN_SHIFT) /* Disable RDRF assertion when receiver is idle */
#define FCUART_FIFO_RXIDEN_1           (1 << FCUART_FIFO_RXIDEN_SHIFT) /* Enable RDRF assertion when receiver idle for 1 word */
#define FCUART_FIFO_RXIDEN_2           (2 << FCUART_FIFO_RXIDEN_SHIFT) /* Enable RDRF assertion when receiver idle for 2 words */
#define FCUART_FIFO_RXIDEN_4           (3 << FCUART_FIFO_RXIDEN_SHIFT) /* Enable RDRF assertion when receiver idle for 4 words */
#define FCUART_FIFO_RXIDEN_8           (4 << FCUART_FIFO_RXIDEN_SHIFT) /* Enable RDRF assertion when receiver idle for 8 words */
#define FCUART_FIFO_RXIDEN_16          (5 << FCUART_FIFO_RXIDEN_SHIFT) /* Enable RDRF assertion when receiver idle for 16 words */
#define FCUART_FIFO_RXIDEN_32          (6 << FCUART_FIFO_RXIDEN_SHIFT) /* Enable RDRF assertion when receiver idle for 32 words */
#define FCUART_FIFO_RXIDEN_64          (7 << FCUART_FIFO_RXIDEN_SHIFT) /* Enable RDRF assertion when receiver idle for 64 words */

#define FCUART_FIFO_RXFLUSH            (1 << 14) /* Bit 14: Receive FIFO/Buffer Flush */
#define FCUART_FIFO_TXFLUSH            (1 << 15) /* Bit 15: Transmit FIFO/Buffer Flush */
#define FCUART_FIFO_RXUF               (1 << 16) /* Bit 16: Receiver Buffer Underflow Flag */
#define FCUART_FIFO_TXOF               (1 << 17) /* Bit 17: Transmitter Buffer Overflow Flag */
                                                 /* Bits 18-21:  Reserved */
#define FCUART_FIFO_RXEMPTY            (1 << 22) /* Bit 22: Receive Buffer/FIFO Empty */
#define FCUART_FIFO_TXEMPTY            (1 << 23) /* Bit 23: Transmit Buffer/FIFO Empty */
                                                 /* Bits 24-31:  Reserved */

/* FCUART Watermark Register */

#define FCUART_WATERMARK_TXWATER_SHIFT (0)       /* Bits 0-1: Transmit Watermark */
#define FCUART_WATERMARK_TXWATER_MASK  (0xF << FCUART_WATERMARK_TXWATER_SHIFT)
#define FCUART_WATERMARK_TXWATER(n)    ((uint32_t)(n) << FCUART_WATERMARK_TXWATER_SHIFT)
                                                 /* Bits 2-7:  Reserved */
#define FCUART_WATERMARK_TXCOUNT_SHIFT (8)       /* Bits 8-10:Transmit Counter */
#define FCUART_WATERMARK_TXCOUNT_MASK  (0x1F << FCUART_WATERMARK_TXCOUNT_SHIFT)
                                                 /* Bits 11-15:  Reserved */
#define FCUART_WATERMARK_RXWATER_SHIFT (16)      /* Bits 16-17: Receive Watermark */
#define FCUART_WATERMARK_RXWATER_MASK  (0xF << FCUART_WATERMARK_RXWATER_SHIFT)
#define FCUART_WATERMARK_RXWATER(n)    ((uint32_t)(n) << FCUART_WATERMARK_RXWATER_SHIFT)
                                                 /* Bits 18-23:  Reserved */
#define FCUART_WATERMARK_RXCOUNT_SHIFT (24)      /* Bits 24-26: Receive Counter */
#define FCUART_WATERMARK_RXCOUNT_MASK  (0x1F << FCUART_WATERMARK_RXCOUNT_SHIFT)
                                                 /* Bits 27-31:  Reserved */

#endif /* __VENDOR_FLAGCHIP_CHIPS_FC7300_HARDWARE_FC7300_FCUART_H */
