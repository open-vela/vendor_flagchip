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

#ifndef __VENDOR_FLAGCHIP_CHIPS_FC7300_HARDWARE_FC7300_FCIIC_H
#define __VENDOR_FLAGCHIP_CHIPS_FC7300_HARDWARE_FC7300_FCIIC_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>
#include "hardware/fc7300_memorymap.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Register offsets *********************************************************/

#define FC7300_FCIIC_MCR_OFFSET        0x0010  /* Master Control Register */
#define FC7300_FCIIC_MSR_OFFSET        0x0014  /* Master Status Register */
#define FC7300_FCIIC_MIER_OFFSET       0x0018  /* Master Interrupt Enable Register */
#define FC7300_FCIIC_MDER_OFFSET       0x001C  /* Master DMA Enable Register */
#define FC7300_FCIIC_MCFGR0_OFFSET     0x0020  /* Master Configuration Register 0 */
#define FC7300_FCIIC_MCFGR1_OFFSET     0x0024  /* Master Configuration Register 1 */
#define FC7300_FCIIC_MCFGR2_OFFSET     0x0028  /* Master Configuration Register 2 */
#define FC7300_FCIIC_MCFGR3_OFFSET     0x002C  /* Master Configuration Register 3 */
#define FC7300_FCIIC_MDMR_OFFSET       0x0040  /* Master Data Match Register */
#define FC7300_FCIIC_MCCR_OFFSET       0x0048  /* Master Clock Configuration Register */
#define FC7300_FCIIC_MFCR_OFFSET       0x0058  /* Master FIFO Control Register */
#define FC7300_FCIIC_MFSR_OFFSET       0x005C  /* Master FIFO Status Register */
#define FC7300_FCIIC_MTDR_OFFSET       0x0060  /* Master Transmit Data Register */
#define FC7300_FCIIC_MRDR_OFFSET       0x0070  /* Master Receive Data Register */
#define FC7300_FCIIC_SCR_OFFSET        0x0110  /* Slave Control Register */
#define FC7300_FCIIC_SSR_OFFSET        0x0114  /* Slave Status Register */
#define FC7300_FCIIC_SIER_OFFSET       0x0118  /* Slave Interrupt Enable Register */
#define FC7300_FCIIC_SDER_OFFSET       0x011C  /* Slave DMA Enable Register */
#define FC7300_FCIIC_SCFGR1_OFFSET     0x0124  /* Slave Configuration Register 1 */
#define FC7300_FCIIC_SCFGR2_OFFSET     0x0128  /* Slave Configuration Register 2 */
#define FC7300_FCIIC_SAMR_OFFSET       0x0140  /* Slave Address Match Register */
#define FC7300_FCIIC_SASR_OFFSET       0x0150  /* Slave Address Status Register */
#define FC7300_FCIIC_STAR_OFFSET       0x0154  /* Slave Transmit ACK Register */
#define FC7300_FCIIC_STDR_OFFSET       0x0160  /* Slave Transmit Data Register */
#define FC7300_FCIIC_SRDR_OFFSET       0x0170  /* Slave Receive Data Register */

/* Register bit definitions *************************************************/

/* FCIIC Master Control Register */

#define FCIIC_MCR_MEN                  (1 << 0)  /* Bit 0:  Master Enable */
#define FCIIC_MCR_RST                  (1 << 1)  /* Bit 1:  Software Reset */
                                                 /* Bit 2:  Reserved */
#define FCIIC_MCR_DBGEN                (1 << 3)  /* Bit 3:  Debug Enable */
                                                 /* Bit 4-7: Reserved */
#define FCIIC_MCR_RTF                  (1 << 8)  /* Bit 8:  Reset Transmit FIFO */
#define FCIIC_MCR_RRF                  (1 << 9)  /* Bit 9:  Reset Receive FIFO */
                                                 /* Bit 10-31: Reserved */

/* FCIIC Master Status Register */

#define FCIIC_MSR_TDF                  (1 << 0)  /* Bit 0:  Transmit Data Flag */
#define FCIIC_MSR_RDF                  (1 << 1)  /* Bit 1:  Receive Data Flag */
                                                 /* Bit 2-7:  Reserved */
#define FCIIC_MSR_EPF                  (1 << 8)  /* Bit 8:  End Packet Flag */
#define FCIIC_MSR_SDF                  (1 << 9)  /* Bit 9:  STOP Detect Flag */
#define FCIIC_MSR_NDF                  (1 << 10) /* Bit 10: NACK Detect Flag */
#define FCIIC_MSR_ALF                  (1 << 11) /* Bit 11: Arbitration Lost Flag */
#define FCIIC_MSR_FEF                  (1 << 12) /* Bit 12: FIFO Error Flag */
#define FCIIC_MSR_PLTF                 (1 << 13) /* Bit 13: Pin Low Timeout Flag */
#define FCIIC_MSR_DMF                  (1 << 14) /* Bit 14: Data Match Flag */
                                                 /* Bit 15-23: Reserved */
#define FCIIC_MSR_MBF                  (1 << 24) /* Bit 24: Master Busy Flag */
#define FCIIC_MSR_BBF                  (1 << 25) /* Bit 25: Bus Busy Flag */
                                                 /* Bit 26-31: Reserved */

/* FCIIC Master Interrupt Enable Register */

#define FCIIC_MIER_TDIE                (1 << 0)  /* Bit 0:  Transmit Data Interrupt Enable */
#define FCIIC_MIER_RDIE                (1 << 1)  /* Bit 1:  Receive Data Interrupt Enable */
                                                 /* Bit 2-7:  Reserved */
#define FCIIC_MIER_EPIE                (1 << 8)  /* Bit 8:  End Packet Interrupt Enable */
#define FCIIC_MIER_SDIE                (1 << 9)  /* Bit 9:  STOP Detect Interrupt Enable */
#define FCIIC_MIER_NDIE                (1 << 10) /* Bit 10: NACK Detect Interrupt Enable */
#define FCIIC_MIER_ALIE                (1 << 11) /* Bit 11: Arbitration Lost Interrupt Enable */
#define FCIIC_MIER_FEIE                (1 << 12) /* Bit 12: FIFO Error Interrupt Enable */
#define FCIIC_MIER_PLTIE               (1 << 13) /* Bit 13: Pin Low Timeout Interrupt Enable */
#define FCIIC_MIER_DMIE                (1 << 14) /* Bit 14: Data Match Interrupt Enable */
                                                 /* Bit 15-31: Reserved */

/* FCIIC Master DMA Enable Register */

#define FCIIC_MDER_TDDE                (1 << 0)  /* Bit 0:  Transmit Data DMA Enable */
#define FCIIC_MDER_RDDE                (1 << 1)  /* Bit 1:  Receive Data DMA Enable */
                                                 /* Bit 2-31: Reserved */

/* FCIIC Master Configuration Register 0 */

#define FCIIC_MCFGR0_TRGEN             (1 << 0)  /* Bit 0:  Trigger Enable */
                                                 /* Bit 1-8:  Reserved */
#define FCIIC_MCFGR0_RDMO              (1 << 9)  /* Bit 9:  Receive Data Match Only */
                                                 /* Bit 10-31: Reserved */

/* FCIIC Master Configuration Register 1 */

#define FCIIC_MCFGR1_PRESCALE_SHIFT    (0)       /* Bit 0-2:  Prescaler */
#define FCIIC_MCFGR1_PRESCALE_MASK     (0x07 << FCIIC_MCFGR1_PRESCALE_SHIFT)
#  define FCIIC_MCFGR1_PRESCALE(x)     (((x) << FCIIC_MCFGR1_PRESCALE_SHIFT) & FCIIC_MCFGR1_PRESCALE_MASK)
                                                 /* Bit 3-7:  Reserved */
#define FCIIC_MCFGR1_AUTOSTOP          (1 << 8)  /* Bit 8:  Automatic STOP Generation */
#define FCIIC_MCFGR1_IGNACK            (1 << 9)  /* Bit 9:  IGNACK */
#define FCIIC_MCFGR1_TIMECFG           (1 << 10) /* Bit 10: Timeout Configuration */
                                                 /* Bit 11-15: Reserved */
#define FCIIC_MCFGR1_MATCFG_SHIFT      (16)      /* Bit 16-18: Match Configuration */
#define FCIIC_MCFGR1_MATCFG_MASK       (0x07 << FCIIC_MCFGR1_MATCFG_SHIFT)
#  define FCIIC_MCFGR1_MATCFG(x)       (((x) << FCIIC_MCFGR1_MATCFG_SHIFT) & FCIIC_MCFGR1_MATCFG_MASK)
                                                 /* Bit 19-24: Reserved */
#define FCIIC_MCFGR1_PINCFG            (1 << 25) /* Bit 25: Pin Configuration */
                                                 /* Bit 26-31: Reserved */

/* FCIIC Master Configuration Register 2 */

#define FCIIC_MCFGR2_BUSIDLE_SHIFT     (0)       /* Bit 0-11: Bus Idle Timeout */
#define FCIIC_MCFGR2_BUSIDLE_MASK      (0x0FFF << FCIIC_MCFGR2_BUSIDLE_SHIFT)
#  define FCIIC_MCFGR2_BUSIDLE(x)      (((x) << FCIIC_MCFGR2_BUSIDLE_SHIFT) & FCIIC_MCFGR2_BUSIDLE_MASK)
                                                 /* Bit 12-15: Reserved */
#define FCIIC_MCFGR2_FILTSCL_SHIFT     (16)      /* Bit 16-19: Glitch Filter SCL */
#define FCIIC_MCFGR2_FILTSCL_MASK      (0x0F << FCIIC_MCFGR2_FILTSCL_SHIFT)
#  define FCIIC_MCFGR2_FILTSCL(x)      (((x) << FCIIC_MCFGR2_FILTSCL_SHIFT) & FCIIC_MCFGR2_FILTSCL_MASK)
                                                 /* Bit 20-23: Reserved */
#define FCIIC_MCFGR2_FILTSDA_SHIFT     (24)      /* Bit 24-27: Glitch Filter SDA */
#define FCIIC_MCFGR2_FILTSDA_MASK      (0x0F << FCIIC_MCFGR2_FILTSDA_SHIFT)
#  define FCIIC_MCFGR2_FILTSDA(x)      (((x) << FCIIC_MCFGR2_FILTSDA_SHIFT) & FCIIC_MCFGR2_FILTSDA_MASK)
                                                 /* Bit 28-31: Reserved */

/* FCIIC Master Configuration Register 3 */

                                                 /* Bit 0-7:  Reserved */
#define FCIIC_MCFGR3_PINLOW_SHIFT      (8)       /* Bit 8-19: Pin Low Timeout */
#define FCIIC_MCFGR3_PINLOW_MASK       (0x0FFF << FCIIC_MCFGR3_PINLOW_SHIFT)
#  define FCIIC_MCFGR3_PINLOW(x)       (((x) << FCIIC_MCFGR3_PINLOW_SHIFT) & FCIIC_MCFGR3_PINLOW_MASK)
                                                 /* Bits 20-31: Reserved */

/* FCIIC Master Data Match Register */

#define FCIIC_MDMR_MATCH0_SHIFT        (0)       /* Bit 0-7:  Match 0 Value */
#define FCIIC_MDMR_MATCH0_MASK         (0xFF << FCIIC_MDMR_MATCH0_SHIFT)
#  define FCIIC_MDMR_MATCH0(x)         (((x) << FCIIC_MDMR_MATCH0_SHIFT) & FCIIC_MDMR_MATCH0_MASK)
                                                 /* Bits 8-15: Reserved */
#define FCIIC_MDMR_MATCH1_SHIFT        (16)      /* Bit 16-23: Match 1 Value */
#define FCIIC_MDMR_MATCH1_MASK         (0xFF << FCIIC_MDMR_MATCH1_SHIFT)
#  define FCIIC_MDMR_MATCH1(x)         (((x) << FCIIC_MDMR_MATCH1_SHIFT) & FCIIC_MDMR_MATCH1_MASK)
                                                 /* Bits 24-31: Reserved */

/* FCIIC Master Clock Configuration Register */

#define FCIIC_MCCR_CLKLO_SHIFT         (0)       /* Bit 0-5:  Clock Low Period */
#define FCIIC_MCCR_CLKLO_MASK          (0x3F << FCIIC_MCCR_CLKLO_SHIFT)
#  define FCIIC_MCCR_CLKLO(x)          (((x) << FCIIC_MCCR_CLKLO_SHIFT) & FCIIC_MCCR_CLKLO_MASK)
                                                 /* Bits 6-7:  Reserved */
#define FCIIC_MCCR_CLKHI_SHIFT         (8)       /* Bit 8-13: Clock High Period */
#define FCIIC_MCCR_CLKHI_MASK          (0x3F << FCIIC_MCCR_CLKHI_SHIFT)
#  define FCIIC_MCCR_CLKHI(x)          (((x) << FCIIC_MCCR_CLKHI_SHIFT) & FCIIC_MCCR_CLKHI_MASK)
                                                 /* Bits 14-15: Reserved */
#define FCIIC_MCCR_SETHOLD_SHIFT       (16)      /* Bit 16-21: Setup Hold Delay */
#define FCIIC_MCCR_SETHOLD_MASK        (0x3F << FCIIC_MCCR_SETHOLD_SHIFT)
#  define FCIIC_MCCR_SETHOLD(x)        (((x) << FCIIC_MCCR_SETHOLD_SHIFT) & FCIIC_MCCR_SETHOLD_MASK)
                                                 /* Bits 22-23: Reserved */
#define FCIIC_MCCR_DATAVD_SHIFT        (24)      /* Bit 24-29: Data Valid Delay */
#define FCIIC_MCCR_DATAVD_MASK         (0x3F << FCIIC_MCCR_DATAVD_SHIFT)
#  define FCIIC_MCCR_DATAVD(x)         (((x) << FCIIC_MCCR_DATAVD_SHIFT) & FCIIC_MCCR_DATAVD_MASK)
                                                 /* Bits 30-31: Reserved */

/* FCIIC Master FIFO Control Register */

#define FCIIC_MFCR_TXWATER_SHIFT       (0)       /* Bit 0-3:  Transmit FIFO Watermark */
#define FCIIC_MFCR_TXWATER_MASK        (0x0F << FCIIC_MFCR_TXWATER_SHIFT)
#  define FCIIC_MFCR_TXWATER(x)        (((x) << FCIIC_MFCR_TXWATER_SHIFT) & FCIIC_MFCR_TXWATER_MASK)
                                                 /* Bits 4-15: Reserved */
#define FCIIC_MFCR_RXWATER_SHIFT       (16)      /* Bit 16-19: Receive FIFO Watermark */
#define FCIIC_MFCR_RXWATER_MASK        (0x0F << FCIIC_MFCR_RXWATER_SHIFT)
#  define FCIIC_MFCR_RXWATER(x)        (((x) << FCIIC_MFCR_RXWATER_SHIFT) & FCIIC_MFCR_RXWATER_MASK)
                                                 /* Bits 20-31: Reserved */

/* FCIIC Master FIFO Status Register */

#define FCIIC_MFSR_TXCOUNT_SHIFT       (0)       /* Bit 0-3:  Transmit FIFO Count */
#define FCIIC_MFSR_TXCOUNT_MASK        (0x0F << FCIIC_MFSR_TXCOUNT_SHIFT)
#  define FCIIC_MFSR_TXCOUNT(x)        (((x) << FCIIC_MFSR_TXCOUNT_SHIFT) & FCIIC_MFSR_TXCOUNT_MASK)
                                                 /* Bits 4-15: Reserved */
#define FCIIC_MFSR_RXCOUNT_SHIFT       (16)      /* Bit 16-19: Receive FIFO Count */
#define FCIIC_MFSR_RXCOUNT_MASK        (0x0F << FCIIC_MFSR_RXCOUNT_SHIFT)
#  define FCIIC_MFSR_RXCOUNT(x)        (((x) << FCIIC_MFSR_RXCOUNT_SHIFT) & FCIIC_MFSR_RXCOUNT_MASK)
                                                 /* Bits 20-31: Reserved */

/* FCIIC Master Transmit Data Register */

#define FCIIC_MTDR_DATA_SHIFT          (0)                                      /* Bit 0-7:  Transmit Data */
#define FCIIC_MTDR_DATA_MASK           (0xFF << FCIIC_MTDR_DATA_SHIFT)
#  define FCIIC_MTDR_DATA(x)           (((x) << FCIIC_MTDR_DATA_SHIFT) & FCIIC_MTDR_DATA_MASK)
#define FCIIC_MTDR_CMD_SHIFT           (8)                                      /* Bit 8-10: Command Data */
#define FCIIC_MTDR_CMD_MASK            (0x07 << FCIIC_MTDR_CMD_SHIFT)
#  define FCIIC_MTDR_CMD_TRANSMIT                (0x00 << FCIIC_MTDR_CMD_SHIFT) /* Transmit DATA */
#  define FCIIC_MTDR_CMD_RECEIVE                 (0x01 << FCIIC_MTDR_CMD_SHIFT) /* Receive (DATA[7:0] + 1) bytes */
#  define FCIIC_MTDR_CMD_STOP                    (0x02 << FCIIC_MTDR_CMD_SHIFT) /* Generate STOP condition */
#  define FCIIC_MTDR_CMD_RECANDDISCARD           (0x03 << FCIIC_MTDR_CMD_SHIFT) /* Receive and discard (DATA[7:0] + 1) bytes */
#  define FCIIC_MTDR_CMD_STARTANDTRANSMIT        (0x04 << FCIIC_MTDR_CMD_SHIFT) /* Generate (repeated) START and transmit address in DATA[7:0] */
#  define FCIIC_MTDR_CMD_STARTANDTRANSMITWITHNAK (0x05 << FCIIC_MTDR_CMD_SHIFT) /* Generate (repeated) START and transmit address in DATA[7:0]. This transfer expects a NACK to be returned. */
                                                                                /* Bits 11-31: Reserved */

/* FCIIC Master Receive Data Register */

#define FCIIC_MRDR_DATA_SHIFT          (0)       /* Bit 0-7:  Receive Data */
#define FCIIC_MRDR_DATA_MASK           (0xFF << FCIIC_MRDR_DATA_SHIFT)
#  define FCIIC_MRDR_DATA(x)           (((x) << FCIIC_MRDR_DATA_SHIFT) & FCIIC_MRDR_DATA_MASK)
                                                 /* Bits 8-13: Reserved */
#define FCIIC_MRDR_RXEMPTY             (1 << 14) /* Bit 14: RX Empty */
                                                 /* Bits 15-31: Reserved */

/* FCIIC Slave Control Register */

#define FCIIC_SCR_SEN                  (1 << 0)  /* Bit 0:  Slave Enable */
#define FCIIC_SCR_RST                  (1 << 1)  /* Bit 1:  Software Reset */
                                                 /* Bit 2-3:  Reserved */
#define FCIIC_SCR_FILTEN               (1 << 4)  /* Bit 4:  Filter Enable */
                                                 /* Bit 5-31: Reserved */

/* FCIIC Slave Status Register */

#define FCIIC_SSR_TDF                  (1 << 0)  /* Bit 0:  Transmit Data Flag */
#define FCIIC_SSR_RDF                  (1 << 1)  /* Bit 1:  Receive Data Flag */
#define FCIIC_SSR_AVF                  (1 << 2)  /* Bit 2:  Address Valid Flag */
#define FCIIC_SSR_TAF                  (1 << 3)  /* Bit 3:  Transmit ACK Flag */
                                                 /* Bit 4-7:  Reserved */
#define FCIIC_SSR_RSF                  (1 << 8)  /* Bit 8:  Repeated Start Flag */
#define FCIIC_SSR_SDF                  (1 << 9)  /* Bit 9:  STOP Detect Flag */
#define FCIIC_SSR_BEF                  (1 << 10) /* Bit 10: Bit Error Flag */
#define FCIIC_SSR_TREF                 (1 << 11) /* Bit 11: Transmit or Receive Error Flag */
#define FCIIC_SSR_AM0F                 (1 << 12) /* Bit 12: Address Match 0 Flag */
#define FCIIC_SSR_AM1F                 (1 << 13) /* Bit 13: Address Match 1 Flag */
#define FCIIC_SSR_GCF                  (1 << 14) /* Bit 14: General Call Flag */
#define FCIIC_SSR_SARF                 (1 << 15) /* Bit 15: SMBus Alert Response Flag */
                                                 /* Bit 16-23: Reserved */
#define FCIIC_SSR_SBF                  (1 << 24) /* Bit 24: Slave Busy Flag */
#define FCIIC_SSR_BBF                  (1 << 25) /* Bit 25: Bus Busy Flag */
                                                 /* Bit 26-31: Reserved */

/* FCIIC Slave Interrupt Enable Register */

#define FCIIC_SIER_TDIE                (1 << 0)  /* Bit 0:  Transmit Data Interrupt Enable */
#define FCIIC_SIER_RDIE                (1 << 1)  /* Bit 1:  Receive Data Interrupt Enable */
#define FCIIC_SIER_AVIE                (1 << 2)  /* Bit 2:  Address Valid Interrupt Enable */
#define FCIIC_SIER_TAIE                (1 << 3)  /* Bit 3:  Transmit ACK Interrupt Enable */
                                                 /* Bit 4-7:  Reserved */
#define FCIIC_SIER_RSIE                (1 << 8)  /* Bit 8:  Repeated Start Interrupt Enable */
#define FCIIC_SIER_SDIE                (1 << 9)  /* Bit 9:  STOP Detect Interrupt Enable */
#define FCIIC_SIER_BEIE                (1 << 10) /* Bit 10: Bit Error Interrupt Enable */
#define FCIIC_SIER_TREIE               (1 << 11) /* Bit 11: Transmit or Receive Error Interrupt Enable */
#define FCIIC_SIER_AM0IE               (1 << 12) /* Bit 12: Address Match 0 Interrupt Enable */
#define FCIIC_SIER_AM1IE               (1 << 13) /* Bit 13: Address Match 1 Interrupt Enable */
#define FCIIC_SIER_GCIE                (1 << 14) /* Bit 14: General Call Interrupt Enable */
#define FCIIC_SIER_SARIE               (1 << 15) /* Bit 15: SMBus Alert Response Interrupt Enable */
                                                 /* Bit 16-31: Reserved */

/* FCIIC Slave DMA Enable Register */

#define FCIIC_SDER_TDDE                (1 << 0)  /* Bit 0:  Transmit Data DMA Enable */
#define FCIIC_SDER_RDDE                (1 << 1)  /* Bit 1:  Receive Data DMA Enable */
#define FCIIC_SDER_AVDE                (1 << 2)  /* Bit 2:  Address Valid DMA Enable */
                                                 /* Bit 3-31: Reserved */

/* FCIIC Slave Configuration Register 1 */

#define FCIIC_SCFGR1_ADDRSTALL         (1 << 0)                                                 /* Bit 0:  Address SCL Stall */
#define FCIIC_SCFGR1_RXSTALL           (1 << 1)                                                 /* Bit 1:  RX SCL Stall */
#define FCIIC_SCFGR1_TXDSTALL          (1 << 2)                                                 /* Bit 2:  TX Data SCL Stall */
#define FCIIC_SCFGR1_ACKSTALL          (1 << 3)                                                 /* Bit 3:  ACK SCL Stall */
                                                                                                /* Bit 4-7:  Reserved */
#define FCIIC_SCFGR1_GCEN              (1 << 8)                                                 /* Bit 8:  General Call Enable */
#define FCIIC_SCFGR1_SAEN              (1 << 9)                                                 /* Bit 9:  SMBus Alert Enable */
#define FCIIC_SCFGR1_TXCFG             (1 << 10)                                                /* Bit 10: Transmit Flag Configuration */
#define FCIIC_SCFGR1_RXCFG             (1 << 11)                                                /* Bit 11: Receive Data Configuration */
#define FCIIC_SCFGR1_IGNACK            (1 << 12)                                                /* Bit 12: Ignore NACK */
#define FCIIC_SCFGR1_HSMEN             (1 << 13)                                                /* Bit 13: High Speed Mode Enable */
                                                                                                /* Bit 14-15: Reserved */
#define FCIIC_SCFGR1_ADDRCFG_SHIFT     (16)                                                     /* Bit 16-18: Address Configuration */
#define FCIIC_SCFGR1_ADDRCFG_MASK      (0x07 << FCIIC_SCFGR1_ADDRCFG_SHIFT)
#  define FCIIC_SCFGR1_ADDRCFG_ADDR7MATCH0                 (0x00 << FCIIC_SCFGR1_ADDRCFG_SHIFT) /* Address match 0 (7-bit) */
#  define FCIIC_SCFGR1_ADDRCFG_ADDR10MATCH0                (0x01 << FCIIC_SCFGR1_ADDRCFG_SHIFT) /* Address match 0 (10-bit) */
#  define FCIIC_SCFGR1_ADDRCFG_ADDR7MATCH0ORADDR7MATCH1    (0x02 << FCIIC_SCFGR1_ADDRCFG_SHIFT) /* Address match 0 (7-bit) or Address match 1 (7-bit) */
#  define FCIIC_SCFGR1_ADDRCFG_ADDR10MATCH0ORADDR10MATCH1  (0x03 << FCIIC_SCFGR1_ADDRCFG_SHIFT) /* Address match 0 (10-bit) or Address match 1 (10-bit) */
#  define FCIIC_SCFGR1_ADDRCFG_ADDR7MATCH0ORADDR10MATCH1   (0x04 << FCIIC_SCFGR1_ADDRCFG_SHIFT) /* Address match 0 (7-bit) or Address match 1 (10-bit) */
#  define FCIIC_SCFGR1_ADDRCFG_ADDR10MATCH0ORADDR7MATCH1   (0x05 << FCIIC_SCFGR1_ADDRCFG_SHIFT) /* Address match 0 (10-bit) or Address match 1 (7-bit) */
#  define FCIIC_SCFGR1_ADDRCFG_ADDR7MATCH0TOMATCH1         (0x06 << FCIIC_SCFGR1_ADDRCFG_SHIFT) /* From Address match 0 (7-bit) to Address match 1 (7-bit) */
#  define FCIIC_SCFGR1_ADDRCFG_ADDR10MATCH0TOMATCH1        (0x07 << FCIIC_SCFGR1_ADDRCFG_SHIFT) /* From Address match 0 (10-bit) to Address match 1 (10-bit) */
                                                                                                /* Bit 19-31: Reserved */

/* FCIIC Slave Configuration Register 2 */

#define FCIIC_SCFGR2_CLKHOLD_SHIFT     (0)       /* Bit 0-3:  Clock Hold Time */
#define FCIIC_SCFGR2_CLKHOLD_MASK      (0x0F << FCIIC_SCFGR2_CLKHOLD_SHIFT)
#  define FCIIC_SCFGR2_CLKHOLD(x)      (((x) << FCIIC_SCFGR2_CLKHOLD_SHIFT) & FCIIC_SCFGR2_CLKHOLD_MASK)
                                                 /* Bits 4-7:  Reserved */
#define FCIIC_SCFGR2_DATAVD_SHIFT      (8)       /* Bit 8-13: Data Valid Delay */
#define FCIIC_SCFGR2_DATAVD_MASK       (0x3F << FCIIC_SCFGR2_DATAVD_SHIFT)
#  define FCIIC_SCFGR2_DATAVD(x)       (((x) << FCIIC_SCFGR2_DATAVD_SHIFT) & FCIIC_SCFGR2_DATAVD_MASK)
                                                 /* Bits 14-15: Reserved */
#define FCIIC_SCFGR2_FILTSCL_SHIFT     (16)      /* Bit 16-19: Glitch Filter SCL */
#define FCIIC_SCFGR2_FILTSCL_MASK      (0x0F << FCIIC_SCFGR2_FILTSCL_SHIFT)
#  define FCIIC_SCFGR2_FILTSCL(x)      (((x) << FCIIC_SCFGR2_FILTSCL_SHIFT) & FCIIC_SCFGR2_FILTSCL_MASK)
                                                 /* Bits 20-23: Reserved */
#define FCIIC_SCFGR2_FILTSDA_SHIFT     (24)      /* Bit 24-27: Glitch Filter SDA */
#define FCIIC_SCFGR2_FILTSDA_MASK      (0x0F << FCIIC_SCFGR2_FILTSDA_SHIFT)
#  define FCIIC_SCFGR2_FILTSDA(x)      (((x) << FCIIC_SCFGR2_FILTSDA_SHIFT) & FCIIC_SCFGR2_FILTSDA_MASK)
                                                 /* Bits 28-31: Reserved */

/* FCIIC Slave Address Match Register */

                                                 /* Bits 0:  Reserved */
#define FCIIC_SAMR_ADDR0_SHIFT         (1)       /* Bit 1-10: Address 0 Value */
#define FCIIC_SAMR_ADDR0_MASK          (0x03FF << FCIIC_SAMR_ADDR0_SHIFT)
#  define FCIIC_SAMR_ADDR0(x)          (((x) << FCIIC_SAMR_ADDR0_SHIFT) & FCIIC_SAMR_ADDR0_MASK)
                                                 /* Bits 11-16: Reserved */
#define FCIIC_SAMR_ADDR1_SHIFT         (17)      /* Bit 17-26: Address 0 Value */
#define FCIIC_SAMR_ADDR1_MASK          (0x03FF << FCIIC_SAMR_ADDR1_SHIFT)
#  define FCIIC_SAMR_ADDR1(x)          (((x) << FCIIC_SAMR_ADDR1_SHIFT) & FCIIC_SAMR_ADDR1_MASK)
                                                 /* Bits 27-31: Reserved */

/* FCIIC Slave Address Status Register */

#define FCIIC_SASR_RADDR_SHIFT         (0)       /* Bit 0-10: Received Address */
#define FCIIC_SASR_RADDR_MASK          (0x07FF << FCIIC_SASR_RADDR_SHIFT)
#  define FCIIC_SASR_RADDR(x)          (((x) << FCIIC_SASR_RADDR_SHIFT) & FCIIC_SASR_RADDR_MASK)
                                                 /* Bits 11-13: Reserved */
#define FCIIC_SASR_ANV                 (1 << 14) /* Bit 14: Address Not Valid */
                                                 /* Bits 15-31: Reserved */

/* FCIIC Slave Transmit ACK Register */
#define FCIIC_STAR_TXNACK              (1 << 0)  /* Bit 0:  Transmit NACK */
                                                 /* Bits 1-31: Reserved */

/* FCIIC Slave Transmit Data Register */

#define FCIIC_STDR_DATA_SHIFT          (0)       /* Bit 0-7:  Transmit Data */
#define FCIIC_STDR_DATA_MASK           (0xFF << FCIIC_STDR_DATA_SHIFT)
#  define FCIIC_STDR_DATA(x)           (((x) << FCIIC_STDR_DATA_SHIFT) & FCIIC_STDR_DATA_MASK)
                                                 /* Bits 8-31: Reserved */

/* FCIIC Slave Receive Data Register */

#define FCIIC_SRDR_DATA_SHIFT          (0)       /* Bit 0-7:  Transmit Data */
#define FCIIC_SRDR_DATA_MASK           (0xFF << FCIIC_SRDR_DATA_SHIFT)
#  define FCIIC_SRDR_DATA(x)           (((x) << FCIIC_SRDR_DATA_SHIFT) & FCIIC_SRDR_DATA_MASK)
                                                 /* Bits 8-13: Reserved */
#define FCIIC_SRDR_RXEMPTY             (1 << 14) /* Bit 14: RX Empty */
#define FCIIC_SRDR_SOF                 (1 << 15) /* Bit 15: Start Of Frame */
                                                 /* Bits 16-31: Reserved */

#endif /* __VENDOR_FLAGCHIP_CHIPS_FC7300_HARDWARE_FC7300_FCIIC_H */
