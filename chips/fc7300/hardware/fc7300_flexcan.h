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

#ifndef __VENDOR_FLAGCHIP_CHIPS_FC7300_HARDWARE_FC7300_FLEXCAN_H
#define __VENDOR_FLAGCHIP_CHIPS_FC7300_HARDWARE_FC7300_FLEXCAN_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/can.h>
#include <nuttx/config.h>
#include <nuttx/net/can.h>
#include <hardware/fc7300_memorymap.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Register Offsets *********************************************************/

#define FC7300_CAN_MCR_OFFSET        0x0000  /* Module Configuration Register */
#define FC7300_CAN_CTRL1_OFFSET      0x0004  /* Control 1 Register */
#define FC7300_CAN_TIMER_OFFSET      0x0008  /* Free Running Timer */
#define FC7300_CAN_RXMGMASK_OFFSET   0x0010  /* Rx Mailboxes Global Mask Register */
#define FC7300_CAN_RX14MASK_OFFSET   0x0014  /* Rx 14 Mask Register */
#define FC7300_CAN_RX15MASK_OFFSET   0x0018  /* Rx 15 Mask Register */
#define FC7300_CAN_ECR_OFFSET        0x001c  /* Error Counter */
#define FC7300_CAN_ESR1_OFFSET       0x0020  /* Error and Status 1 Register */
#define FC7300_CAN_IMASK1_OFFSET     0x0028  /* Interrupt Masks 1 Register */
#define FC7300_CAN_IFLAG1_OFFSET     0x0030  /* Interrupt Flags 1 Register */
#define FC7300_CAN_CTRL2_OFFSET      0x0034  /* Control 2 Register */
#define FC7300_CAN_ESR2_OFFSET       0x0038  /* Error and Status 2 Register */
#define FC7300_CAN_CRCR_OFFSET       0x0044  /* CRC Register */
#define FC7300_CAN_RXFGMASK_OFFSET   0x0048  /* Rx FIFO Global Mask Register */
#define FC7300_CAN_RXFIR_OFFSET      0x004c  /* Rx FIFO Information Register */
#define FC7300_CAN_CBT_OFFSET        0x0050  /* CAN Bit Timing register */

#define FC7300_CAN_MB_OFFSET         0x0080  /* CAN MB Registers */
#define FC7300_CAN_MB_WORD_COUNT     128     /* CAN MB Registers Count in Word */

#define FC7300_CAN_RXIMR_OFFSET(n)   (0x0880 + ((n) << 2))
#  define FC7300_CAN_RXIMR0_OFFSET   0x0880  /* R0 Individual Mask Registers */
#  define FC7300_CAN_RXIMR1_OFFSET   0x0884  /* R1 Individual Mask Registers */
#  define FC7300_CAN_RXIMR2_OFFSET   0x0888  /* R2 Individual Mask Registers */
#  define FC7300_CAN_RXIMR3_OFFSET   0x088c  /* R3 Individual Mask Registers */
#  define FC7300_CAN_RXIMR4_OFFSET   0x0890  /* R4 Individual Mask Registers */
#  define FC7300_CAN_RXIMR5_OFFSET   0x0894  /* R5 Individual Mask Registers */
#  define FC7300_CAN_RXIMR6_OFFSET   0x0898  /* R6 Individual Mask Registers */
#  define FC7300_CAN_RXIMR7_OFFSET   0x089c  /* R7 Individual Mask Registers */
#  define FC7300_CAN_RXIMR8_OFFSET   0x08a0  /* R8 Individual Mask Registers */
#  define FC7300_CAN_RXIMR9_OFFSET   0x08a4  /* R9 Individual Mask Registers */
#  define FC7300_CAN_RXIMR10_OFFSET  0x08a8  /* R10 Individual Mask Registers */
#  define FC7300_CAN_RXIMR11_OFFSET  0x08ac  /* R11 Individual Mask Registers */
#  define FC7300_CAN_RXIMR12_OFFSET  0x08b0  /* R12 Individual Mask Registers */
#  define FC7300_CAN_RXIMR13_OFFSET  0x08b4  /* R13 Individual Mask Registers */
#  define FC7300_CAN_RXIMR14_OFFSET  0x08b8  /* R14 Individual Mask Registers */
#  define FC7300_CAN_RXIMR15_OFFSET  0x08bc  /* R15 Individual Mask Registers */
#  define FC7300_CAN_RXIMR16_OFFSET  0x08c0  /* R16 Individual Mask Registers */
#  define FC7300_CAN_RXIMR17_OFFSET  0x08c4  /* R17 Individual Mask Registers */
#  define FC7300_CAN_RXIMR18_OFFSET  0x08c8  /* R18 Individual Mask Registers */
#  define FC7300_CAN_RXIMR19_OFFSET  0x08cc  /* R19 Individual Mask Registers */
#  define FC7300_CAN_RXIMR20_OFFSET  0x08d0  /* R20 Individual Mask Registers */
#  define FC7300_CAN_RXIMR21_OFFSET  0x08d4  /* R21 Individual Mask Registers */
#  define FC7300_CAN_RXIMR22_OFFSET  0x08d8  /* R22 Individual Mask Registers */
#  define FC7300_CAN_RXIMR23_OFFSET  0x08dc  /* R23 Individual Mask Registers */
#  define FC7300_CAN_RXIMR24_OFFSET  0x08e0  /* R24 Individual Mask Registers */
#  define FC7300_CAN_RXIMR25_OFFSET  0x08e4  /* R25 Individual Mask Registers */
#  define FC7300_CAN_RXIMR26_OFFSET  0x08e8  /* R26 Individual Mask Registers */
#  define FC7300_CAN_RXIMR27_OFFSET  0x08ec  /* R27 Individual Mask Registers */
#  define FC7300_CAN_RXIMR28_OFFSET  0x08f0  /* R28 Individual Mask Registers */
#  define FC7300_CAN_RXIMR29_OFFSET  0x08f4  /* R29 Individual Mask Registers */
#  define FC7300_CAN_RXIMR30_OFFSET  0x08f8  /* R30 Individual Mask Registers */
#  define FC7300_CAN_RXIMR31_OFFSET  0x08fc  /* R31 Individual Mask Registers */
#define FC7300_CAN_RXIMR_COUNT       32      /* Individual Mask Registers Count */

#define FC7300_CAN_RXFIR_TEST_OFFSET 0x0a80  /* Rx FIFO Information Registers */
#define FC7300_CAN_RXFIR_TEST_WORD_COUNT 8   /* Rx FIFO Information Registers Count in Word */

#define FC7300_CAN_MASK_OFFSET       0x0aa0  /* Rx FIFO Information Registers */
#define FC7300_CAN_MASK_WORD_COUNT       4   /* Rx FIFO Information Registers Count in Word */

#define FC7300_CAN_SMBTX_OFFSET      0x0ab0  /* Tx Serial Message Buffer Registers */
#define FC7300_CAN_SMBTX_WORD_COUNT  4       /* Tx Serial Message Buffer Registers Count in Word */

#define FC7300_CAN_SMBRX0_OFFSET     0x0ac0  /* Rx Serial Message Buffer 0 Registers */
#define FC7300_CAN_SMBRX0_WORD_COUNT 4       /* Rx Serial Message Buffer 0 Registers Count in Word */

#define FC7300_CAN_SMBRX1_OFFSET     0x0ad0  /* Rx Serial Message Buffer 1 Registers */
#define FC7300_CAN_SMBRX1_WORD_COUNT 4       /* Rx Serial Message Buffer 1 Registers Count in Word */

#define FC7300_CAN_MECR_OFFSET       0x0ae0  /* Memory Error Control Register */
#define FC1450_CAN_ERRIAR_OFFSET     0x0ae4  /* Error Injection Address Register */
#define FC7300_CAN_ERRIDPR_OFFSET    0x0ae8  /* Error Injection Data Pattern Register */
#define FC7300_CAN_ERRIPPR_OFFSET    0x0aec  /* Error Injection Parity Pattern Register */
#define FC7300_CAN_RERRAR_OFFSET     0x0af0  /* Error Report Address Register */
#define FC7300_CAN_RERRDR_OFFSET     0x0af4  /* Error Report Data Register */
#define FC7300_CAN_RERRSYNR_OFFSET   0x0af8  /* Error Report Syndrome Register */
#define FC7300_CAN_ERRSR_OFFSET      0x0afc  /* Error Status Register */

#define FC7300_CAN_CTRL1_PN_OFFSET       0x0b00  /* Pretended Networking Control 1 register */
#define FC7300_CAN_CTRL2_PN_OFFSET       0x0b04  /* Pretended Networking Control 2 register */
#define FC7300_CAN_WU_MTC_OFFSET         0x0b08  /* Pretended Networking Wake Up Match register */
#define FC7300_CAN_FLT_ID1_OFFSET        0x0b0c  /* Pretended Networking ID Filter 1 register */
#define FC7300_CAN_FLT_DLC_OFFSET        0x0b10  /* Pretended Networking DLC Filter register */
#define FC7300_CAN_PL1_LO_OFFSET         0x0b14  /* Pretended Networking Payload Low Filter 1 register */
#define FC7300_CAN_PL1_HI_OFFSET         0x0b18  /* Pretended Networking Payload High Filter 1 register */
#define FC7300_CAN_FLT_ID2_IDMASK_OFFSET 0x0b1c  /* Pretended Networking ID Filter 2 ID Mask register */
#define FC7300_CAN_PL2_PLMASK_LO_OFFSET  0x0b20  /* Pretended Networking Payload Low Filter 2 Payload Low Mask register */
#define FC7300_CAN_PL2_PLMASK_HI_OFFSET  0x0b24  /* Pretended Networking Payload High Filter 2 low order bits / Payload High Mask register */

#define FC7300_CAN_WMB_OFFSET(n)     (0x0b40 + ((n) << 4))
#  define FC7300_CAN_WMB_CS_OFFSET   0x0000  /* Wake Up Message Buffer register for C/S */
#  define FC7300_CAN_WMB_ID_OFFSET   0x0004  /* Wake Up Message Buffer Register for ID */
#  define FC7300_CAN_WMB_D03_OFFSET  0x0008  /* Wake Up Message Buffer Register for Data 0-3 */
#  define FC7300_CAN_WMB_D47_OFFSET  0x000c  /* Wake Up Message Buffer Register Data 4-7 */

#  define FC7300_CAN_WMB0_CS_OFFSET  0x0b40  /* Wake Up Message Buffer register for C/S */
#  define FC7300_CAN_WMB0_ID_OFFSET  0x0b44  /* Wake Up Message Buffer Register for ID */
#  define FC7300_CAN_WMB0_D03_OFFSET 0x0b48  /* Wake Up Message Buffer Register for Data 0-3 */
#  define FC7300_CAN_WMB0_D47_OFFSET 0x0b4c  /* Wake Up Message Buffer Register Data 4-7 */
#  define FC7300_CAN_WMB1_CS_OFFSET  0x0b50  /* Wake Up Message Buffer register for C/S */
#  define FC7300_CAN_WMB1_ID_OFFSET  0x0b54  /* Wake Up Message Buffer Register for ID */
#  define FC7300_CAN_WMB1_D03_OFFSET 0x0b58  /* Wake Up Message Buffer Register for Data 0-3 */
#  define FC7300_CAN_WMB1_D47_OFFSET 0x0b5c  /* Wake Up Message Buffer Register Data 4-7 */
#  define FC7300_CAN_WMB2_CS_OFFSET  0x0b60  /* Wake Up Message Buffer register for C/S */
#  define FC7300_CAN_WMB2_ID_OFFSET  0x0b64  /* Wake Up Message Buffer Register for ID */
#  define FC7300_CAN_WMB2_D03_OFFSET 0x0b68  /* Wake Up Message Buffer Register for Data 0-3 */
#  define FC7300_CAN_WMB2_D47_OFFSET 0x0b6c  /* Wake Up Message Buffer Register Data 4-7 */
#  define FC7300_CAN_WMB3_CS_OFFSET  0x0b70  /* Wake Up Message Buffer register for C/S */
#  define FC7300_CAN_WMB3_ID_OFFSET  0x0b74  /* Wake Up Message Buffer Register for ID */
#  define FC7300_CAN_WMB3_D03_OFFSET 0x0b78  /* Wake Up Message Buffer Register for Data 0-3 */
#  define FC7300_CAN_WMB3_D47_OFFSET 0x0b7c  /* Wake Up Message Buffer Register Data 4-7 */

#define FC7300_CAN_EPRS_OFFSET       0x0bf0  /* Enhanced CAN Bit Timing Prescalers Register */
#define FC7300_CAN_ENCBT_OFFSET      0x0bf4  /* Enhanced Nominal CAN Bit Timing Register */
#define FC7300_CAN_EDCBT_OFFSET      0x0bf8  /* Enhanced Data Phase CAN Bit Timing Register */
#define FC7300_CAN_ETDC_OFFSET       0x0bfc  /* Enhanced Transceiver Delay Compensation Register */

#define FC7300_CAN_FDCTRL_OFFSET     0x0c00  /* CAN FD Control Register */
#define FC7300_CAN_FDCBT_OFFSET      0x0c04  /* CAN FD Bit Timing Register */
#define FC7300_CAN_FDCRC_OFFSET      0x0c08  /* CAN FD CRC Register */

#define FC7300_CAN_ERFCR_OFFSET      0x0c0c  /* Enhanced Rx FIFO Control Register*/
#define FC7300_CAN_ERFIER_OFFSET     0x0c10  /* Enhanced Rx FIFO Interrupt Enable Register */
#define FC7300_CAN_ERFSR_OFFSET      0x0c14  /* Enhanced Rx FIFO Status Register */

#define FC7300_CAN_FDSMBTX_OFFSET    0x0f28  /* CAN FD Tx Serial Message Buffer Registers */
#define FC7300_CAN_FDSMBTX_WORD_COUNT  18    /* CAN FD Tx Serial Message Buffer Registers Count in Word */

#define FC7300_CAN_FDSMBRX0_OFFSET   0x0f70  /* CAN FD Rx Serial Message Buffer 0 Registers */
#define FC7300_CAN_FDSMBRX0_WORD_COUNT 18    /* CAN FD Rx Serial Message Buffer 0 Registers Count in Word */

#define FC7300_CAN_FDSMBRX1_OFFSET   0x0fb8  /* CAN FD Rx Serial Message Buffer 1 Registers */
#define FC7300_CAN_FDSMBRX1_WORD_COUNT 18    /* CAN FD Rx Serial Message Buffer 1 Registers Count in Word */

#define FC7300_CAN_ERX_FIFO_OFFSET   0x2000  /* Enhanced Rx FIFO registers */
#define FC7300_CAN_ERX_FIFO_WORD_COUNT 240   /* Enhanced Rx FIFO registers Count in Word */

#define FC7300_CAN_ERFFEL_OFFSET     0x3000  /* Enhanced Rx FIFO Filter Element Registers */
#define FC7300_CAN_ERFFEL_COUNT      32      /* Enhanced Rx FIFO Filter Element Registers Count */

/* Register Bit Definitions *************************************************/

/* Module Configuration Register */

#define CAN_MCR_MAXMB_SHIFT           (0)       /* Bits 0-6: Number of the Last Message Buffer */
#define CAN_MCR_MAXMB_MASK            (0x7f << CAN_MCR_MAXMB_SHIFT)
#define CAN_MCR_MAXMB(x)              (((x) << CAN_MCR_MAXMB_SHIFT) & CAN_MCR_MAXMB_MASK)
                                                /* Bit 7:  Reserved */
#define CAN_MCR_IDAM_SHIFT            (8)       /* Bits 8-9: ID Acceptance Mode */

#define CAN_MCR_IDAM_MASK             (3 << CAN_MCR_IDAM_SHIFT)
#  define CAN_MCR_IDAM_FMTA           (0 << CAN_MCR_IDAM_SHIFT) /* Format A: One full ID  */
#  define CAN_MCR_IDAM_FMTB           (1 << CAN_MCR_IDAM_SHIFT) /* Format B: Two full (or partial) IDs */
#  define CAN_MCR_IDAM_FMTC           (2 << CAN_MCR_IDAM_SHIFT) /* Format C: Four partial IDs */
#  define CAN_MCR_IDAM_FMTD           (3 << CAN_MCR_IDAM_SHIFT) /* Format D: All frames rejected */

                                                /* Bit 10: Reserved */
#define CAN_MCR_FDEN                  (1 << 11) /* Bit 11: CAN FD operation enable */
#define CAN_MCR_AEN                   (1 << 12) /* Bit 12: Abort Enable */
#define CAN_MCR_LPRIOEN               (1 << 13) /* Bit 13: Local Priority Enable */
                                                /* Bits 14-15: Reserved */
#define CAN_MCR_IRMQ                  (1 << 16) /* Bit 16: Individual Rx Masking and Queue Enable */
#define CAN_MCR_SRXDIS                (1 << 17) /* Bit 17: Self Reception Disable */
#define CAN_MCR_DOZE                  (1 << 18) /* Bit 18: Doze Mode Enable */
                                                /* Bit 19: Reserved */
#define CAN_MCR_LPMACK                (1 << 20) /* Bit 20: Low Power Mode Acknowledge */
#define CAN_MCR_WRNEN                 (1 << 21) /* Bit 21: Warning Interrupt Enable */
#define CAN_MCR_SLFWAK                (1 << 22) /* Bit 22: Self Wake Up */
#define CAN_MCR_SUPV                  (1 << 23) /* Bit 23: Supervisor Mode */
#define CAN_MCR_FRZACK                (1 << 24) /* Bit 24: Freeze Mode Acknowledge */
#define CAN_MCR_SOFTRST               (1 << 25) /* Bit 25: Soft Reset */
#define CAN_MCR_WAKMSK                (1 << 26) /* Bit 26: Wake Up Interrupt Mask */
#define CAN_MCR_NOTRDY                (1 << 27) /* Bit 27: FlexCAN Not Ready */
#define CAN_MCR_HALT                  (1 << 28) /* Bit 28: Halt FlexCAN */
#define CAN_MCR_RFEN                  (1 << 29) /* Bit 29: Rx FIFO Enable */
#define CAN_MCR_FRZ                   (1 << 30) /* Bit 30: Freeze Enable */
#define CAN_MCR_MDIS                  (1 << 31) /* Bit 31: Module Disable */

/* Control 1 Register */

#define CAN_CTRL1_PROPSEG_SHIFT       (0)       /* Bits 0-2: Propagation Segment */
#define CAN_CTRL1_PROPSEG_MASK        (0x07 << CAN_CTRL1_PROPSEG_SHIFT)
#define CAN_CTRL1_PROPSEG(x)          (((x) << CAN_CTRL1_PROPSEG_SHIFT) & CAN_CTRL1_PROPSEG_MASK)
#define CAN_CTRL1_LOM                 (1 << 3)  /* Bit 3:  Listen-Only Mode */
#define CAN_CTRL1_LBUF                (1 << 4)  /* Bit 4:  Lowest Buffer Transmitted First */
#define CAN_CTRL1_TSYN                (1 << 5)  /* Bit 5:  Timer Sync */
#define CAN_CTRL1_BOFFREC             (1 << 6)  /* Bit 6:  Bus Off Recovery */
#define CAN_CTRL1_SMP                 (1 << 7)  /* Bit 7:  CAN Bit Sampling */
                                                /* Bits 8-9: Reserved */
#define CAN_CTRL1_RWRNMSK             (1 << 10) /* Bit 10: Rx Warning Interrupt Mask */
#define CAN_CTRL1_TWRNMSK             (1 << 11) /* Bit 11: Tx Warning Interrupt Mask */
#define CAN_CTRL1_LPB                 (1 << 12) /* Bit 12: Loop Back Mode */
#define CAN_CTRL1_CLKSRC              (1 << 13) /* Bit 13: CAN Engine Clock Source */
#define CAN_CTRL1_ERRMSK              (1 << 14) /* Bit 14: Error Mask */
#define CAN_CTRL1_BOFFMSK             (1 << 15) /* Bit 15: Bus Off Mask */
#define CAN_CTRL1_TIMINGMSK           (0xffff << 16)
#define CAN_CTRL1_PSEG2_SHIFT         (16)      /* Bits 16-18: Phase Segment 2 */
#define CAN_CTRL1_PSEG2_MASK          (0x07 << CAN_CTRL1_PSEG2_SHIFT)
#define CAN_CTRL1_PSEG2(x)            (((x) << CAN_CTRL1_PSEG2_SHIFT) & CAN_CTRL1_PSEG2_MASK)
#define CAN_CTRL1_PSEG1_SHIFT         (19)      /* Bits 19-21: Phase Segment 1 */
#define CAN_CTRL1_PSEG1_MASK          (0x07 << CAN_CTRL1_PSEG1_SHIFT)
#define CAN_CTRL1_PSEG1(x)            (((x) << CAN_CTRL1_PSEG1_SHIFT) & CAN_CTRL1_PSEG1_MASK)
#define CAN_CTRL1_RJW_SHIFT           (22)      /* Bits 22-23: Resync Jump Width */
#define CAN_CTRL1_RJW_MASK            (0x03 << CAN_CTRL1_RJW_SHIFT)
#define CAN_CTRL1_RJW(x)              (((x) << CAN_CTRL1_RJW_SHIFT) & CAN_CTRL1_RJW_MASK)
#define CAN_CTRL1_PRESDIV_SHIFT       (24)      /* Bits 24-31: Prescaler Division Factor */
#define CAN_CTRL1_PRESDIV_MASK        (0xff << CAN_CTRL1_PRESDIV_SHIFT)
#define CAN_CTRL1_PRESDIV(x)          (((x) << CAN_CTRL1_PRESDIV_SHIFT) & CAN_CTRL1_PRESDIV_MASK)

/* Free Running Timer */

#define CAN_TIMER_SHIFT               (0)       /* Bits 0-15: Timer value */
#define CAN_TIMER_MASK                (0xffff << CAN_TIMER_SHIFT)
                                                /* Bits 16-31: Reserved */

/* Rx Mailboxes Global Mask Register (32 Rx Mailboxes Global Mask Bits) */

#define CAN_RXMGMASK(n)               (1 << (n)) /* Bit n: Rx Mailboxe n Global Mask Bit */

/* Rx 14 Mask Register */

#define CAN_RX14MASK(n)               (1 << (n)) /* Bit n: Rx Buffer 14 Mask Bit n */

/* Rx 15 Mask Register */

#define CAN_RX15MASK(n)               (1 << (n)) /* Bit n: Rx Buffer 15 Mask Bit n */

/* Error Counter */

#define CAN_ECR_TXERRCNT_SHIFT        (0)       /* Bits 0-7: Transmit Error Counter */
#define CAN_ECR_TXERRCNT_MASK         (0xff << CAN_ECR_TXERRCNT_SHIFT)
#define CAN_ECR_RXERRCNT_SHIFT        (8)       /* Bits 8-15: Receive Error Counter */
#define CAN_ECR_RXERRCNT_MASK         (0xff << CAN_ECR_RXERRCNT_SHIFT)
#define CAN_ECR_TXERRCNT_FAST_SHIFT   (16)       /* Bits 16-23: Transmit Error Counter For Fast Bits */
#define CAN_ECR_TXERRCNT_FAST_MASK    (0xff << CAN_ECR_TXERRCNT_FAST_SHIFT)
#define CAN_ECR_RXERRCNT_FAST_SHIFT   (24)       /* Bits 24-31: Receive Error Counter For Fast Bits */
#define CAN_ECR_RXERRCNT_FAST_MASK    (0xff << CAN_ECR_RXERRCNT_FAST_SHIFT)

/* Error and Status 1 Register */

#define CAN_ESR1_ERRINT               (1 << 1)  /* Bit 1:  Error Interrupt */
#define CAN_ESR1_BOFFINT              (1 << 2)  /* Bit 2:  'Bus Off' Interrupt */
#define CAN_ESR1_RX                   (1 << 3)  /* Bit 3:  FlexCAN in Reception */
#define CAN_ESR1_FLTCONF_SHIFT        (4)       /* Bits 4-5: Fault Confinement State */

#define CAN_ESR1_FLTCONF_MASK         (3 << CAN_ESR1_FLTCONF_SHIFT)
#  define CAN_ESR1_FLTCONF_ACTV       (0 << CAN_ESR1_FLTCONF_SHIFT) /* Error Active */
#  define CAN_ESR1_FLTCONF_PASV       (1 << CAN_ESR1_FLTCONF_SHIFT) /* Error Passive */
#  define CAN_ESR1_FLTCONF_OFF        (2 << CAN_ESR1_FLTCONF_SHIFT) /* Bus Off */

#define CAN_ESR1_TX                   (1 << 6)  /* Bit 6:  FlexCAN in Transmission */
#define CAN_ESR1_IDLE                 (1 << 7)  /* Bit 7:  CAN bus is in IDLE state */
#define CAN_ESR1_RXWRN                (1 << 8)  /* Bit 8:  Rx Error Warning */
#define CAN_ESR1_TXWRN                (1 << 9)  /* Bit 9:  TX Error Warning */
#define CAN_ESR1_STFERR               (1 << 10) /* Bit 10: Stuffing Error */
#define CAN_ESR1_FRMERR               (1 << 11) /* Bit 11: Form Error */
#define CAN_ESR1_CRCERR               (1 << 12) /* Bit 12: Cyclic Redundancy Check Error */
#define CAN_ESR1_ACKERR               (1 << 13) /* Bit 13: Acknowledge Error */
#define CAN_ESR1_BIT0ERR              (1 << 14) /* Bit 14: Bit0 Error */
#define CAN_ESR1_BIT1ERR              (1 << 15) /* Bit 15: Bit1 Error */
#define CAN_ESR1_RWRNINT              (1 << 16) /* Bit 16: Rx Warning Interrupt Flag */
#define CAN_ESR1_TWRNINT              (1 << 17) /* Bit 17: Tx Warning Interrupt Flag */
#define CAN_ESR1_SYNCH                (1 << 18) /* Bit 18: CAN Synchronization Status */
#define CAN_ESR1_BOFFDONEINT          (1 << 19) /* Bit 19: Bus Off Done Interrupt */
#define CAN_ESR1_ERRINT_FAST          (1 << 20) /* Bit 20: CAN FD Error Interrupt */
#define CAN_ESR1_ERROVR               (1 << 21) /* Bit 21: Overrun Error */
#define CAN_ESR1_STFERR_FAST          (1 << 26) /* Bit 26: CAN FD Stuffing Error */
#define CAN_ESR1_FRMERR_FAST          (1 << 27) /* Bit 27: CAN FD Form Error */
#define CAN_ESR1_CRCERR_FAST          (1 << 28) /* Bit 28: CAN FD Cyclic Redundancy Check Error */
#define CAN_ESR1_BIT0ERR_FAST         (1 << 30) /* Bit 30: CAN FD Bit0 Error */
#define CAN_ESR1_BIT1ERR_FAST         (1 << 31) /* Bit 31: CAN FD Bit0 Error */

/* Interrupt Masks 2 Register */

#define CAN_IMASK2(n)                 (1 << (n)) /* Bit n: Buffer MBn Mask */

/* Interrupt Masks 1 Register */

#define CAN_IMASK1(n)                 (1 << (n)) /* Bit n: Buffer MBn Mask */

/* Interrupt Flags 2 Register */

#define CAN_IFLAG2(n)                 (1 << (n)) /* Bit n: Buffer MBn Interrupt */

/* Interrupt Flags 1 Register */

#define CAN_IFLAG1(n)                 (1 << (n)) /* Bit n: When Rx FIFO disabled, this bit indicates buffer MBn interrupt, n=0..31 */
#define CAN_IFLAG1_CLEAR_RX_FIFO      (1 << 0)   /* Bit 0: When Rx FIFO enabled, this bit is used to trigger the clear FIFO operation */
#define CAN_IFLAG1_RX_FIFO_READY      (1 << 5)   /* Bit 5: When Rx FIFO enabled, this bit indicates frames available in Rx FIFO */
#define CAN_IFLAG1_RX_FIFO_WARNING    (1 << 6)   /* Bit 6: When Rx FIFO enabled, this bit indicates Rx FIFO is almost full */
#define CAN_IFLAG1_RX_FIFO_OVERFLOW   (1 << 7)   /* Bit 7: When Rx FIFO enabled, this bit indicates Rx FIFO is overflow */

/* Control 2 Register */

                                                /* Bits 0-10: Reserved */
#define CAN_CTRL2_EDFLTDIS            (1 << 11) /* Bit 11:  Edge Filter Disable */
#define CAN_CTRL2_ISOCANFDEN          (1 << 12) /* Bit 12:  ISO CAN FD Enable */
#define CAN_CTRL2_BTE                 (1 << 13) /* Bit 13:  Bit Timing Expansion Enable */
#define CAN_CTRL2_PREXCEN             (1 << 14) /* Bit 14:  Protocol Exception Enable */
#define CAN_CTRL2_TIMER_SRC           (1 << 15) /* Bit 15:  Timer Source */
#define CAN_CTRL2_EACEN               (1 << 16) /* Bit 16:  Entire Frame Arbitration Field Comparison Enable (Rx) */
#define CAN_CTRL2_RRS                 (1 << 17) /* Bit 17:  Remote Request Storing */
#define CAN_CTRL2_MRP                 (1 << 18) /* Bit 18:  Mailboxes Reception Priority */
#define CAN_CTRL2_TASD_SHIFT          (19)      /* Bits 19-23: Tx Arbitration Start Delay */
#define CAN_CTRL2_TASD_MASK           (31 << CAN_CTRL2_TASD_SHIFT)
#define CAN_CTRL2_RFFN_SHIFT          (24)      /* Bits 24-27: Number of Rx FIFO Filters */
#define CAN_CTRL2_RFFN_MASK           (15 << CAN_CTRL2_RFFN_SHIFT)
#  define CAN_CTRL2_RFFN_8MB          (0 << CAN_CTRL2_RFFN_SHIFT)
#  define CAN_CTRL2_RFFN_16MB         (1 << CAN_CTRL2_RFFN_SHIFT)
#  define CAN_CTRL2_RFFN_24MB         (2 << CAN_CTRL2_RFFN_SHIFT)
#  define CAN_CTRL2_RFFN_32MB         (3 << CAN_CTRL2_RFFN_SHIFT)
#  define CAN_CTRL2_RFFN_40MB         (4 << CAN_CTRL2_RFFN_SHIFT)
#  define CAN_CTRL2_RFFN_48MB         (5 << CAN_CTRL2_RFFN_SHIFT)
#  define CAN_CTRL2_RFFN_56MB         (6 << CAN_CTRL2_RFFN_SHIFT)
#  define CAN_CTRL2_RFFN_64MB         (7 << CAN_CTRL2_RFFN_SHIFT)
#  define CAN_CTRL2_RFFN_72MB         (8 << CAN_CTRL2_RFFN_SHIFT)
#  define CAN_CTRL2_RFFN_80MB         (9 << CAN_CTRL2_RFFN_SHIFT)
#  define CAN_CTRL2_RFFN_88MB         (10 << CAN_CTRL2_RFFN_SHIFT)
#  define CAN_CTRL2_RFFN_96MB         (11 << CAN_CTRL2_RFFN_SHIFT)
#  define CAN_CTRL2_RFFN_104MB        (12 << CAN_CTRL2_RFFN_SHIFT)
#  define CAN_CTRL2_RFFN_112MB        (13 << CAN_CTRL2_RFFN_SHIFT)
#  define CAN_CTRL2_RFFN_120MB        (14 << CAN_CTRL2_RFFN_SHIFT)
#  define CAN_CTRL2_RFFN_128MB        (15 << CAN_CTRL2_RFFN_SHIFT)
#define CAN_CTRL2_WRMFRZ              (1 << 28) /* Bit 28: Write-Access to Memory in Freeze mode */
#define CAN_CTRL2_ECRWRE              (1 << 29) /* Bit 29: Error-correction Configuration Register Write Enable */
#define CAN_CTRL2_BOFFDONEMSK         (1 << 30) /* Bit 30: Bus Off Done Interrupt Mask */
#define CAN_CTRL2_ERRMSK_FAST         (1 << 31) /* Bit 31: Error Interrupt Mask in CAN FD frames */

/* Error and Status 2 Register */

                                                /* Bits 0-12: Reserved */
#define CAN_ESR2_IMB                  (1 << 13) /* Bit 13: Inactive Mailbox */
#define CAN_ESR2_VPS                  (1 << 14) /* Bit 14: Valid Priority Status */
                                                /* Bit 15: Reserved */
#define CAN_ESR2_LPTM_SHIFT           (16)      /* Bits 16-22: Lowest Priority Tx Mailbox */
#define CAN_ESR2_LPTM_MASK            (0x7f << CAN_ESR2_LPTM_SHIFT)
                                                /* Bits 23-31: Reserved */

/* CRC Register */

#define CAN_CRCR_TXCRC_SHIFT          (0)       /* Bits 0-14: CRC Transmitted */
#define CAN_CRCR_TXCRC_MASK           (0x7fff << CAN_CRCR_TXCRC_SHIFT)
                                                /* Bit  15: Reserved */
#define CAN_CRCR_MBCRC_SHIFT          (16)      /* Bits 16-22: CRC Mailbox */
#define CAN_CRCR_MBCRC_MASK           (0x7f << CAN_CRCR_MBCRC_SHIFT)
                                                /* Bits 23-31: Reserved */

/* Rx FIFO Global Mask Register (32 Rx FIFO Global Mask Bits) */

/* Rx FIFO Information Register */

#define CAN_RXFIR_IDHIT_SHIFT         (0)       /* Bits 0-8: Identifier Acceptance Filter Hit Indicator */
#define CAN_RXFIR_IDHIT_MASK          (0x1ff << CAN_RXFIR_IDHIT_SHIFT)
                                                /* Bits 9-31: Reserved */

/* CAN Bit Timing register */

#define CAN_CBT_EPSEG2_SHIFT          (0)       /* Bits 0-4: Extended Phase Segment 2 */
#define CAN_CBT_EPSEG2_MASK           (0x1f << CAN_CBT_EPSEG2_SHIFT)
#define CAN_CBT_EPSEG2(x)             (((x) << CAN_CBT_EPSEG2_SHIFT) & CAN_CBT_EPSEG2_MASK)
#define CAN_CBT_EPSEG1_SHIFT          (5)       /* Bits 5-9: Extended Phase Segment 1 */
#define CAN_CBT_EPSEG1_MASK           (0x1f << CAN_CBT_EPSEG1_SHIFT)
#define CAN_CBT_EPSEG1(x)             (((x) << CAN_CBT_EPSEG1_SHIFT) & CAN_CBT_EPSEG1_MASK)
#define CAN_CBT_EPROPSEG_SHIFT        (10)      /* Bits 10-15: Extended Propagation Segment */
#define CAN_CBT_EPROPSEG_MASK         (0x3f << CAN_CBT_EPROPSEG_SHIFT)
#define CAN_CBT_EPROPSEG(x)           (((x) << CAN_CBT_EPROPSEG_SHIFT) & CAN_CBT_EPROPSEG_MASK)
#define CAN_CBT_ERJW_SHIFT            (16)      /* Bits 16-20: Extended Resync Jump Width */
#define CAN_CBT_ERJW_MASK             (0x1f << CAN_CBT_ERJW_SHIFT)
#define CAN_CBT_ERJW(x)               (((x) << CAN_CBT_ERJW_SHIFT) & CAN_CBT_ERJW_MASK)
#define CAN_CBT_EPRESDIV_SHIFT        (21)      /* Bits 21-30: Extended Prescaler Division Factor */
#define CAN_CBT_EPRESDIV_MASK         (0x03ff << CAN_CBT_EPRESDIV_SHIFT)
#define CAN_CBT_EPRESDIV(x)           (((x) << CAN_CBT_EPRESDIV_SHIFT) & CAN_CBT_EPRESDIV_MASK)
#define CAN_CBT_BTF                   (1 << 31) /* Bit 31: Bit Timing Format Enable */

/* Rn Individual Mask Registers */

#define CAN_RXIMR(n)                  (1 << (n)) /* Bit n: Individual Mask Bits */

/* Memory Error Control Register */

                                                /* Bit 0-6: Reserved */
#define CAN_MECR_NCEFAFRZ             (1 << 7)  /* Bit 7: Non-Correctable Errors in FlexCAN Access Put Device in Freeze Mode */
#define CAN_MECR_ECCDIS               (1 << 8)  /* Bit 8: Error Correction Disable */
#define CAN_MECR_RERRDIS              (1 << 9)  /* Bit 9: Error Report Disable */
                                                /* Bits 10-12: Reserved */
#define CAN_MECR_EXTERRIE             (1 << 13) /* Bit 13: Extended Error Injection Enable */
#define CAN_MECR_FAERRIE              (1 << 14) /* Bit 14: FlexCAN Access Error Injection Enable */
#define CAN_MECR_HAERRIE              (1 << 15) /* Bit 15: Host Access Error Injection Enable */
#define CAN_MECR_CEI_MSK              (1 << 16) /* Bit 16: Correctable Errors Interrupt Mask */
                                                /* Bit 17: Reserved */
#define CAN_MECR_FANCEI_MSK           (1 << 18) /* Bit 18: FlexCAN Access With Non-Correctable Errors Interrupt Mask */
#define CAN_MECR_HANCEI_MSK           (1 << 19) /* Bit 19: Host Access With Non-Correctable Errors Interrupt Mask */
                                                /* Bit 20-30: Reserved */
#define CAN_MECR_ECRWRDIS             (1 << 31) /* Bit 31: Error Configuration Register Write Disable */

/* Error Injection Address Register */

#define CAN_ERRIAR_INJADDR_L_SHIFT    (0)       /* Bits 0-1: Error Injection Address Low */
#define CAN_ERRIAR_INJADDR_L_MASK     (0x03 << CAN_ERRIAR_INJADDR_L_SHIFT)
#define CAN_ERRIAR_INJADDR_H_SHIFT    (2)       /* Bits 2-13: Error Injection Address High */
#define CAN_ERRIAR_INJADDR_H_MASK     (0x0fff << CAN_ERRIAR_INJADDR_H_SHIFT)
#define CAN_ERRIAR_INJADDR_H(x)       (((x) << CAN_ERRIAR_INJADDR_H_SHIFT) & CAN_ERRIAR_INJADDR_H_MASK)
                                                /* Bits 14-31: Reserved */

/* Error Injection Data Pattern Register */

/* Error Injection Parity Pattern Register */

#define CAN_ERRIPPR_PFLIP0_SHIFT      (0)       /* Bits 0-4: Parity Flip Pattern For Byte 0 (Least Significant) */
#define CAN_ERRIPPR_PFLIP0_MASK       (0x1f << CAN_ERRIPPR_PFLIP0_SHIFT)
                                                /* Bits 5-7: Reserved */
#define CAN_ERRIPPR_PFLIP1_SHIFT      (8)       /* Bits 8-12: Parity Flip Pattern For Byte 1 */
#define CAN_ERRIPPR_PFLIP1_MASK       (0x1f << CAN_ERRIPPR_PFLIP1_SHIFT)
                                                /* Bits 13-15: Reserved */
#define CAN_ERRIPPR_PFLIP2_SHIFT      (16)      /* Bits 16-20: Parity Flip Pattern For Byte 2 */
#define CAN_ERRIPPR_PFLIP2_MASK       (0x1f << CAN_ERRIPPR_PFLIP2_SHIFT)
                                                /* Bits 21-23: Reserved */
#define CAN_ERRIPPR_PFLIP3_SHIFT      (24)      /* Bits 24-28: Parity Flip Pattern For Byte 3 (Most Significant) */
#define CAN_ERRIPPR_PFLIP3_MASK       (0x1f << CAN_ERRIPPR_PFLIP3_SHIFT)
                                                /* Bits 29-31: Reserved */

/* Error Report Address Register */

#define CAN_RERRAR_ERRADDR_SHIFT      (0)       /* Bits 0-13: Address Where Error Detected */
#define CAN_RERRAR_ERRADDR_MASK       (0x3fff << CAN_RERRAR_ERRADDR_SHIFT)
                                                /* Bits 14-15: Reserved */
#define CAN_RERRAR_SAID_SHIFT         (16)      /* Bits 16-18: SAID */
#define CAN_RERRAR_SAID_MASK          (0x07 << CAN_RERRAR_SAID_SHIFT)
                                                /* Bits 19-23: Reserved */
#define CAN_RERRAR_NCE                (1 << 24) /* Bit 24: Non-Correctable Error */
                                                /* Bits 25-31: Reserved */

/* Error Report Data Register */

/* Error Report Syndrome Register */

#define CAN_RERRSYNR_SYND0_SHIFT      (0)       /* Bits 0-4: Error Syndrome For Byte 0 (Least Significant) */
#define CAN_RERRSYNR_SYND0_MASK       (0x1f << CAN_RERRSYNR_SYND0_SHIFT)
                                                /* Bits 5-6: Reserved */
#define CAN_RERRSYNR_BE0              (1 << 7)  /* Bit 7: Byte Enabled For Byte 0 (Least Significant) */
#define CAN_RERRSYNR_SYND1_SHIFT      (8)       /* Bits 8-12: Error Syndrome For Byte 1 */
#define CAN_RERRSYNR_SYND1_MASK       (0x1f << CAN_RERRSYNR_SYND1_SHIFT)
                                                /* Bits 13-14: Reserved */
#define CAN_RERRSYNR_BE1              (1 << 15) /* Bit 15: Byte Enabled For Byte 1 */
#define CAN_RERRSYNR_SYND2_SHIFT      (16)      /* Bits 16-20: Error Syndrome For Byte 2 */
#define CAN_RERRSYNR_SYND2_MASK       (0x1f << CAN_RERRSYNR_SYND2_SHIFT)
                                                /* Bits 21-22: Reserved */
#define CAN_RERRSYNR_BE2              (1 << 23) /* Bit 23: Byte Enabled For Byte 2 */
#define CAN_RERRSYNR_SYND3_SHIFT      (24)      /* Bits 24-28: Error Syndrome For Byte 3 (Most Significant) */
#define CAN_RERRSYNR_SYND3_MASK       (0x1f << CAN_RERRSYNR_SYND3_SHIFT)
                                                /* Bits 29-30: Reserved */
#define CAN_RERRSYNR_BE3              (1 << 31) /* Bit 31: Byte Enabled For Byte 3 (Most Significant) */

/* Error Status Register */

#define CAN_ERRSR_CEIOF               (1 << 0)  /* Bit 0: Correctable Error Interrupt Overrun Flag */
                                                /* Bit 1: Reserved */
#define CAN_ESSR_FANCEIOF             (1 << 2)  /* Bit 2: FlexCAN Access With Non-Correctable Error Interrupt Overrun Flag */
#define CAN_ESSR_HANCEIOF             (1 << 3)  /* Bit 3: Host Access With Non-Correctable Error Interrupt Overrun Flag */
                                                /* Bits 4-15: Reserved */
#define CAN_ESSR_CEIF                 (1 << 16) /* Bit 16: Correctable Error Interrupt Flag */
                                                /* Bit 17: Reserved */
#define CAN_ESSR_FANCEIF              (1 << 18) /* Bit 18: FlexCAN Access With Non-Correctable Error Interrupt Flag */
#define CAN_ESSR_HANCEIF              (1 << 19) /* Bit 19: Host Access With Non-Correctable Error Interrupt Flag */
                                                /* Bits 20-31: Reserved */

/* Pretended Networking Control 1 register */

#define CAN_CTRL1_PN_FCS_SHIFT        (0)       /* Bits 0-1: Filtering Combination Selection */
#define CAN_CTRL1_PN_FCS_MASK         (0x03 << CAN_CTRL1_PN_FCS_SHIFT)
#  define CAN_CTRL1_PN_FCS_MSG_ID               (0 << CAN_CTRL1_PN_FCS_SHIFT)
#  define CAN_CTRL1_PN_FCS_MSG_ID_PAYLOAD       (1 << CAN_CTRL1_PN_FCS_SHIFT)
#  define CAN_CTRL1_PN_FCS_MSG_ID_COUNT         (2 << CAN_CTRL1_PN_FCS_SHIFT)
#  define CAN_CTRL1_PN_FCS_MSG_ID_PAYLOAD_COUNT (3 << CAN_CTRL1_PN_FCS_SHIFT)
#define CAN_CTRL1_PN_IDFS_SHIFT       (2)       /* Bits 2-3: ID Filtering Selection */
#define CAN_CTRL1_PN_IDFS_MASK        (0x03 << CAN_CTRL1_PN_IDFS_SHIFT)
#  define CAN_CTRL1_PN_IDFS_EQUAL               (0 << CAN_CTRL1_PN_IDFS_SHIFT)
#  define CAN_CTRL1_PN_IDFS_GREATER_EQUAL       (1 << CAN_CTRL1_PN_IDFS_SHIFT)
#  define CAN_CTRL1_PN_IDFS_LOWER_EQUAL         (2 << CAN_CTRL1_PN_IDFS_SHIFT)
#  define CAN_CTRL1_PN_IDFS_BETWEEN             (3 << CAN_CTRL1_PN_IDFS_SHIFT)
#define CAN_CTRL1_PN_PLFS_SHIFT       (3)       /* Bits 4-5: Payload Filtering Selection */
#define CAN_CTRL1_PN_PLFS_MASK        (0x03 << CAN_CTRL1_PN_PLFS_SHIFT)
#  define CAN_CTRL1_PN_PLFS_EQUAL               (0 << CAN_CTRL1_PN_IDFS_SHIFT)
#  define CAN_CTRL1_PN_PLFS_GREATER_EQUAL       (1 << CAN_CTRL1_PN_IDFS_SHIFT)
#  define CAN_CTRL1_PN_PLFS_LOWER_EQUAL         (2 << CAN_CTRL1_PN_IDFS_SHIFT)
#  define CAN_CTRL1_PN_PLFS_BETWEEN             (3 << CAN_CTRL1_PN_IDFS_SHIFT)
                                                /* Bits 6-7: Reserved */
#define CAN_CTRL1_PN_NMATCH_SHIFT     (8)       /* Bits 8-15: Number of Messages Matching the Same Filtering Criteria */
#define CAN_CTRL1_PN_NMATCH_MASK      (0xff << CAN_CTRL1_PN_NMATCH_SHIFT)
#define CAN_CTRL1_PN_NMATCH(x)        (((x) << CAN_CTRL1_PN_NMATCH_SHIFT) & CAN_CTRL1_PN_NMATCH_MASK)
#define CAN_CTRL1_PN_WUMF_MSK         (1 << 16) /* Bits 16: Wake Up by Match Flag Mask Bit */
#define CAN_CTRL1_PN_WTOF_MSK         (1 << 17) /* Bits 17: Wake Up by Timeout Flag Mask Bit */
                                                /* Bits 18-31: Reserved */

/* Pretended Networking Control 2 register */

#define CAN_CTRL2_PN_MATCHTO_SHIFT    (0)       /* Bits 0-15: Timeout for No Message Matching the Filtering Criteria */
#define CAN_CTRL2_PN_MATCHTO_MASK     (0xffff << CAN_CTRL2_PN_MATCHTO_SHIFT)
#define CAN_CTRL2_PN_MATCHTO(x)       (((x) << CAN_CTRL2_PN_MATCHTO_SHIFT) & CAN_CTRL2_PN_MATCHTO_MASK)

/* Pretended Networking Wake Up Match register */

                                                /* Bits 0-7: Reserved */
#define CAN_WU_MTC_MCOUNTER_SHIFT     (8)       /* Bits 8-15: Number of Matches when in Pretended Networking */
#define CAN_WU_MTC_MCOUNTER_MASK      (0xff << CAN_WU_MTC_MCOUNTER_SHIFT)
#define CAN_WU_MTC_WUMF               (1 << 16) /* Bit 16: Wake Up by Match Flag Bit*/
#define CAN_WU_MTC_WTOF               (1 << 17) /* Bit 17: Wake Up by Timeout Flag Bit*/
                                                /* Bits 18-31: Reserved */

/* Pretended Networking ID Filter 1 register */

#define CAN_FLT_ID1_FLT_ID1_SHIFT     (0)       /* Bits 0-28: ID Filter 1 for Pretended Networking filtering */
#define CAN_FLT_ID1_FLT_ID1_MASK      (0x1fffffff << CAN_FLT_ID1_FLT_ID1_SHIFT)
#define CAN_FLT_ID1_FLT_ID1(x)        (((x) << CAN_FLT_ID1_FLT_ID1_SHIFT) & CAN_FLT_ID1_FLT_ID1_MASK)
#define CAN_FLT_ID1_FLT_RTR           (1 << 29) /* Bit 29: Remote Transmission Request Filter */
#define CAN_FLT_ID1_FLT_IDE           (1 << 30) /* Bit 30: ID Extended Filter */
                                                /* Bit 31: Reserved */

/* Pretended Networking DLC Filter register */

#define CAN_FLT_DLC_FLT_DLC_HI_SHIFT  (0)       /* Bits 0-3: Upper Limit for Length of Data Bytes Filter */
#define CAN_FLT_DLC_FLT_DLC_HI_MASK   (0x0f << CAN_PL1_LO_FLT_DLC_HI_SHIFT)
#define CAN_FLT_DLC_FLT_DLC_HI(x)     (((x) << CAN_PL1_LO_FLT_DLC_HI_SHIFT) & CAN_FLT_DLC_FLT_DLC_HI_MASK)
                                                /* Bits 4-15: Reserved */
#define CAN_FLT_DLC_FLT_DLC_LO_SHIFT  (16)      /* Bits 16-19: Lower Limit for Length of Data Bytes Filter */
#define CAN_FLT_DLC_FLT_DLC_LO_MASK   (0x0f << CAN_PL1_LO_FLT_DLC_HI_SHIFT)
#define CAN_FLT_DLC_FLT_DLC_LO(x)     (((x) << CAN_FLT_DLC_FLT_DLC_LO_SHIFT) & CAN_FLT_DLC_FLT_DLC_LO_MASK)
                                                /* Bits 20-31: Reserved */

/* Pretended Networking Payload Low Filter 1 register */

#define CAN_PL1_LO_DATA_BYTE_0_SHIFT  (24)      /* Bits 24-31: Payload Filter 1 low order bits for Pretended Networking payload filtering corresponding to data byte 0 */
#define CAN_PL1_LO_DATA_BYTE_0_MASK   (0xff << CAN_PL1_LO_DATA_BYTE_0_SHIFT)
#define CAN_PL1_LO_DATA_BYTE_0(x)     (((x) << CAN_PL1_LO_DATA_BYTE_0_SHIFT) & CAN_PL1_LO_DATA_BYTE_0_MASK)
#define CAN_PL1_LO_DATA_BYTE_1_SHIFT  (16)      /* Bits 16-23: Payload Filter 1 low order bits for Pretended Networking payload filtering corresponding to data byte 1 */
#define CAN_PL1_LO_DATA_BYTE_1_MASK   (0xff << CAN_PL1_LO_DATA_BYTE_1_SHIFT)
#define CAN_PL1_LO_DATA_BYTE_1(x)     (((x) << CAN_PL1_LO_DATA_BYTE_1_SHIFT) & CAN_PL1_LO_DATA_BYTE_1_MASK)
#define CAN_PL1_LO_DATA_BYTE_2_SHIFT  (8)       /* Bits 8-15: Payload Filter 1 low order bits for Pretended Networking payload filtering corresponding to data byte 2 */
#define CAN_PL1_LO_DATA_BYTE_2_MASK   (0xff << CAN_PL1_LO_DATA_BYTE_2_SHIFT)
#define CAN_PL1_LO_DATA_BYTE_2(x)     (((x) << CAN_PL1_LO_DATA_BYTE_2_SHIFT) & CAN_PL1_LO_DATA_BYTE_2_MASK)
#define CAN_PL1_LO_DATA_BYTE_3_SHIFT  (0)       /* Bits 0-7: Payload Filter 1 low order bits for Pretended Networking payload filtering corresponding to data byte 3 */
#define CAN_PL1_LO_DATA_BYTE_3_MASK   (0xff << CAN_PL1_LO_DATA_BYTE_3_SHIFT)
#define CAN_PL1_LO_DATA_BYTE_3(x)     (((x) << CAN_PL1_LO_DATA_BYTE_3_SHIFT) & CAN_PL1_LO_DATA_BYTE_3_MASK)

/* Pretended Networking Payload High Filter 1 register */

#define CAN_PL1_HI_DATA_BYTE_4_SHIFT  (24)      /* Bits 24-31: Payload Filter 1 high order bits for Pretended Networking payload filtering corresponding to data byte 4 */
#define CAN_PL1_HI_DATA_BYTE_4_MASK   (0xff << CAN_PL1_HI_DATA_BYTE_4_SHIFT)
#define CAN_PL1_HI_DATA_BYTE_4(x)     (((x) << CAN_PL1_HI_DATA_BYTE_4_SHIFT) & CAN_PL1_HI_DATA_BYTE_4_MASK)
#define CAN_PL1_HI_DATA_BYTE_5_SHIFT  (16)      /* Bits 16-23: Payload Filter 1 high order bits for Pretended Networking payload filtering corresponding to data byte 5 */
#define CAN_PL1_HI_DATA_BYTE_5_MASK   (0xff << CAN_PL1_HI_DATA_BYTE_5_SHIFT)
#define CAN_PL1_HI_DATA_BYTE_5(x)     (((x) << CAN_PL1_HI_DATA_BYTE_5_SHIFT) & CAN_PL1_HI_DATA_BYTE_5_MASK)
#define CAN_PL1_HI_DATA_BYTE_6_SHIFT  (8)       /* Bits 8-15: Payload Filter 1 high order bits for Pretended Networking payload filtering corresponding to data byte 6 */
#define CAN_PL1_HI_DATA_BYTE_6_MASK   (0xff << CAN_PL1_HI_DATA_BYTE_6_SHIFT)
#define CAN_PL1_HI_DATA_BYTE_6(x)     (((x) << CAN_PL1_HI_DATA_BYTE_6_SHIFT) & CAN_PL1_HI_DATA_BYTE_6_MASK)
#define CAN_PL1_HI_DATA_BYTE_7_SHIFT  (0)       /* Bits 0-7: Payload Filter 1 high order bits for Pretended Networking payload filtering corresponding to data byte 7 */
#define CAN_PL1_HI_DATA_BYTE_7_MASK   (0xff << CAN_PL1_HI_DATA_BYTE_7_SHIFT)
#define CAN_PL1_HI_DATA_BYTE_7(x)     (((x) << CAN_PL1_HI_DATA_BYTE_7_SHIFT) & CAN_PL1_HI_DATA_BYTE_7_MASK)

/* Pretended Networking ID Filter 2 ID Mask register */

#define CAN_FLT_ID2_IDMASK_FLT_ID2_IDMASK_SHIFT (0) /* Bits 0-28: ID Filter 2 for Pretended Networking Filtering / ID Mask Bits for Pretended Networking ID Filtering */
#define CAN_FLT_ID2_IDMASK_FLT_ID2_IDMASK_MASK  (0x1fffffff << CAN_FLT_ID2_IDMASK_FLT_ID2_IDMASK_SHIFT)
#define CAN_FLT_ID2_IDMASK_FLT_ID2_IDMASK(x)    (((x) << CAN_FLT_ID2_IDMASK_FLT_ID2_IDMASK_SHIFT) & CAN_FLT_ID2_IDMASK_FLT_ID2_IDMASK_MASK)
#define CAN_FLT_ID2_IDMASK_RTR_MSK    (1 << 29) /* Bit 29: Remote Transmission Request Mask Bit */
#define CAN_FLT_ID2_IDMASK_IDE_MSK    (1 << 30) /* Bit 30: ID Extended Mask Bit */
                                                /* Bit 31: Reserved */

/* Pretended Networking Payload Low Filter 2 Payload Low Mask register */

#define CAN_PL2_PLMASK_LO_DATA_BYTE_0_SHIFT (24) /* Bits 24-31: Payload Filter 2 low order bits / Payload Mask low order bits for Pretended Networking payload filtering corresponding to the data byte 0 */
#define CAN_PL2_PLMASK_LO_DATA_BYTE_0_MASK  (0xff << CAN_PL2_PLMASK_LO_DATA_BYTE_0_SHIFT)
#define CAN_PL2_PLMASK_LO_DATA_BYTE_0(x)    (((x) << CAN_PL2_PLMASK_LO_DATA_BYTE_0_SHIFT) & CAN_PL2_PLMASK_LO_DATA_BYTE_0_MASK)
#define CAN_PL2_PLMASK_LO_DATA_BYTE_1_SHIFT (16) /* Bits 16-23: Payload Filter 2 low order bits / Payload Mask low order bits for Pretended Networking payload filtering corresponding to the data byte 1 */
#define CAN_PL2_PLMASK_LO_DATA_BYTE_1_MASK  (0xff << CAN_PL2_PLMASK_LO_DATA_BYTE_1_SHIFT)
#define CAN_PL2_PLMASK_LO_DATA_BYTE_1(x)    (((x) << CAN_PL2_PLMASK_LO_DATA_BYTE_1_SHIFT) & CAN_PL2_PLMASK_LO_DATA_BYTE_1_MASK)
#define CAN_PL2_PLMASK_LO_DATA_BYTE_2_SHIFT (24) /* Bits 8-15: Payload Filter 2 low order bits / Payload Mask low order bits for Pretended Networking payload filtering corresponding to the data byte 2 */
#define CAN_PL2_PLMASK_LO_DATA_BYTE_2_MASK  (0xff << CAN_PL2_PLMASK_LO_DATA_BYTE_2_SHIFT)
#define CAN_PL2_PLMASK_LO_DATA_BYTE_2(x)    (((x) << CAN_PL2_PLMASK_LO_DATA_BYTE_2_SHIFT) & CAN_PL2_PLMASK_LO_DATA_BYTE_2_MASK)
#define CAN_PL2_PLMASK_LO_DATA_BYTE_3_SHIFT (16) /* Bits 0-7: Payload Filter 2 low order bits / Payload Mask low order bits for Pretended Networking payload filtering corresponding to the data byte 3 */
#define CAN_PL2_PLMASK_LO_DATA_BYTE_3_MASK  (0xff << CAN_PL2_PLMASK_LO_DATA_BYTE_3_SHIFT)
#define CAN_PL2_PLMASK_LO_DATA_BYTE_3(x)    (((x) << CAN_PL2_PLMASK_LO_DATA_BYTE_3_SHIFT) & CAN_PL2_PLMASK_LO_DATA_BYTE_3_MASK)

/* Pretended Networking Payload High Filter 2 low order bits
 * Payload High Mask register
 */

#define CAN_PL2_PLMASK_HI_DATA_BYTE_4_SHIFT (24) /* Bits 24-31: Payload Filter 2 high order bits / Payload Mask high order bits for Pretended Networking payload filtering corresponding to the data byte 0 */
#define CAN_PL2_PLMASK_HI_DATA_BYTE_4_MASK  (0xff << CAN_PL2_PLMASK_HI_DATA_BYTE_4_SHIFT)
#define CAN_PL2_PLMASK_HI_DATA_BYTE_4(x)    (((x) << CAN_PL2_PLMASK_HI_DATA_BYTE_4_SHIFT) & CAN_PL2_PLMASK_HI_DATA_BYTE_4_MASK)
#define CAN_PL2_PLMASK_HI_DATA_BYTE_5_SHIFT (16) /* Bits 16-23: Payload Filter 2 high order bits / Payload Mask high order bits for Pretended Networking payload filtering corresponding to the data byte 1 */
#define CAN_PL2_PLMASK_HI_DATA_BYTE_5_MASK  (0xff << CAN_PL2_PLMASK_HI_DATA_BYTE_5_SHIFT)
#define CAN_PL2_PLMASK_HI_DATA_BYTE_5(x)    (((x) << CAN_PL2_PLMASK_HI_DATA_BYTE_5_SHIFT) & CAN_PL2_PLMASK_HI_DATA_BYTE_5_MASK)
#define CAN_PL2_PLMASK_HI_DATA_BYTE_6_SHIFT (24) /* Bits 8-15: Payload Filter 2 high order bits / Payload Mask high order bits for Pretended Networking payload filtering corresponding to the data byte 2 */
#define CAN_PL2_PLMASK_HI_DATA_BYTE_6_MASK  (0xff << CAN_PL2_PLMASK_HI_DATA_BYTE_6_SHIFT)
#define CAN_PL2_PLMASK_HI_DATA_BYTE_6(x)    (((x) << CAN_PL2_PLMASK_HI_DATA_BYTE_6_SHIFT) & CAN_PL2_PLMASK_HI_DATA_BYTE_6_MASK)
#define CAN_PL2_PLMASK_HI_DATA_BYTE_7_SHIFT (16) /* Bits 0-7: Payload Filter 2 high order bits / Payload Mask high order bits for Pretended Networking payload filtering corresponding to the data byte 3 */
#define CAN_PL2_PLMASK_HI_DATA_BYTE_7_MASK  (0xff << CAN_PL2_PLMASK_HI_DATA_BYTE_7_SHIFT)
#define CAN_PL2_PLMASK_HI_DATA_BYTE_7(x)    (((x) << CAN_PL2_PLMASK_HI_DATA_BYTE_7_SHIFT) & CAN_PL2_PLMASK_HI_DATA_BYTE_7_MASK)

/* Wake Up Message Buffer register for C/S */

                                                /* Bits 0-15: Reserved */
#define CAN_WMB_CS_DLC_SHIFT          (16)      /* Bits 16-19: Length of Data in Bytes */
#define CAN_WMB_CS_DLC_MASK           (0x0f << CAN_WMB_CS_DLC_SHIFT)
#define CAN_WMB_CS_RTR                (1 << 20) /* Bit 20: Remote Transmission Request Bit */
#define CAN_WMB_CS_IDE                (1 << 21) /* Bit 21: ID Extended Bit */
#define CAN_WMB_CS_SRR                (1 << 22) /* Bit 22: Substitute Remote Request */
                                                /* Bits 23-31: Reserved */

/* Wake Up Message Buffer Register for ID */

#define CAN_WMB_ID_ID_SHIFT           (0)       /* Bits 16-19: Received ID under Pretended Networking mode */
#define CAN_WMB_ID_ID_MASK            (0x1fffffff << CAN_WMB_ID_ID_SHIFT)

/* Wake Up Message Buffer Register for Data 0-3 */

#define CAN_WMB_D03_DATA_BYTE_0_SHIFT (24)      /* Bits 24-31: Received payload corresponding to the data byte 0 under Pretended Networking mode */
#define CAN_WMB_D03_DATA_BYTE_0_MASK  (0xff << CAN_WMB_D03_DATA_BYTE_0_SHIFT)
#define CAN_WMB_D03_DATA_BYTE_1_SHIFT (16)      /* Bits 16-23: Received payload corresponding to the data byte 1 under Pretended Networking mode */
#define CAN_WMB_D03_DATA_BYTE_1_MASK  (0xff << CAN_WMB_D03_DATA_BYTE_1_SHIFT)
#define CAN_WMB_D03_DATA_BYTE_2_SHIFT (8)       /* Bits 8-15: Received payload corresponding to the data byte 2 under Pretended Networking mode */
#define CAN_WMB_D03_DATA_BYTE_2_MASK  (0xff << CAN_WMB_D03_DATA_BYTE_2_SHIFT)
#define CAN_WMB_D03_DATA_BYTE_3_SHIFT (0)       /* Bits 0-7: Received payload corresponding to the data byte 3 under Pretended Networking mode */
#define CAN_WMB_D03_DATA_BYTE_3_MASK  (0xff << CAN_WMB_D03_DATA_BYTE_3_SHIFT)

/* Wake Up Message Buffer Register Data 4-7 */

#define CAN_WMB_D47_DATA_BYTE_4_SHIFT (24)      /* Bits 24-31: Received payload corresponding to the data byte 4 under Pretended Networking mode */
#define CAN_WMB_D47_DATA_BYTE_4_MASK  (0xff << CAN_WMB_D47_DATA_BYTE_4_SHIFT)
#define CAN_WMB_D47_DATA_BYTE_5_SHIFT (16)      /* Bits 16-23: Received payload corresponding to the data byte 5 under Pretended Networking mode */
#define CAN_WMB_D47_DATA_BYTE_5_MASK  (0xff << CAN_WMB_D47_DATA_BYTE_5_SHIFT)
#define CAN_WMB_D47_DATA_BYTE_6_SHIFT (8)       /* Bits 8-15: Received payload corresponding to the data byte 6 under Pretended Networking mode */
#define CAN_WMB_D47_DATA_BYTE_6_MASK  (0xff << CAN_WMB_D47_DATA_BYTE_6_SHIFT)
#define CAN_WMB_D47_DATA_BYTE_7_SHIFT (0)       /* Bits 0-7: Received payload corresponding to the data byte 7 under Pretended Networking mode */
#define CAN_WMB_D47_DATA_BYTE_7_MASK  (0xff << CAN_WMB_D47_DATA_BYTE_7_SHIFT)

/* Enhanced CAN Bit Timing Prescalers */

#define CAN_EPRS_ENPRESDIV_SHIFT      (0)       /* Bits 0-9: Extended Nominal Prescaler Division Factor */
#define CAN_EPRS_ENPRESDIV_MASK       (0x03ff << CAN_EPRS_ENPRESDIV_SHIFT)
#define CAN_EPRS_ENPRESDIV(x)         (((x) << CAN_EPRS_ENPRESDIV_SHIFT) & CAN_EPRS_ENPRESDIV_MASK)
                                                /* Bits 10-15: Reserved */
#define CAN_EPRS_EDPRESDIV_SHIFT      (16)      /* Bits 16-25: Extended Data Phase Prescaler Division Factor */
#define CAN_EPRS_EDPRESDIV_MASK       (0x03ff << CAN_EPRS_EDPRESDIV_SHIFT)
#define CAN_EPRS_EDPRESDIV(x)         (((x) << CAN_EPRS_EDPRESDIV_SHIFT) & CAN_EPRS_EDPRESDIV_MASK)
                                                /* Bits 26-31: Reserved */

/* Enhanced Nominal CAN Bit Timing */

#define CAN_ENCBT_NTSEG1_SHIFT        (0)       /* Bits 0-7: Nominal Time Segment 1 */
#define CAN_ENCBT_NTSEG1_MASK         (0xff << CAN_ENCBT_NTSEG1_SHIFT)
#define CAN_ENCBT_NTSEG1(x)           (((x) << CAN_ENCBT_NTSEG1_SHIFT) & CAN_ENCBT_NTSEG1_MASK)
                                                /* Bits 8-11: Reserved */
#define CAN_ENCBT_NTSEG2_SHIFT        (12)      /* Bits 12-18: Nominal Time Segment 2 */
#define CAN_ENCBT_NTSEG2_MASK         (0x7f << CAN_ENCBT_NTSEG2_SHIFT)
#define CAN_ENCBT_NTSEG2(x)           (((x) << CAN_ENCBT_NTSEG2_SHIFT) & CAN_ENCBT_NTSEG2_MASK)
                                                /* Bits 19-21: Reserved */
#define CAN_ENCBT_NRJW_SHIFT          (22)      /* Bits 22-28: Nominal Resynchronization Jump Width */
#define CAN_ENCBT_NRJW_MASK           (0x7f << CAN_ENCBT_NRJW_SHIFT)
#define CAN_ENCBT_NRJW(x)             (((x) << CAN_ENCBT_NRJW_SHIFT) & CAN_ENCBT_NRJW_MASK)
                                                /* Bits 29-31: Reserved */

/* Enhanced Data Phase CAN Bit Timing */

#define CAN_EDCBT_DTSEG1_SHIFT        (0)       /* Bits 0-4: Data Phase Time Segment 1 */
#define CAN_EDCBT_DTSEG1_MASK         (0x1f << CAN_EDCBT_DTSEG1_SHIFT)
#define CAN_EDCBT_DTSEG1(x)           (((x) << CAN_EDCBT_DTSEG1_SHIFT) & CAN_EDCBT_DTSEG1_MASK)
                                                /* Bits 5-11: Reserved */
#define CAN_EDCBT_DTSEG2_SHIFT        (12)      /* Bits 12-15: Data Phase Time Segment 2 */
#define CAN_EDCBT_DTSEG2_MASK         (0x0f << CAN_EDCBT_DTSEG2_SHIFT)
#define CAN_EDCBT_DTSEG2(x)           (((x) << CAN_EDCBT_DTSEG2_SHIFT) & CAN_EDCBT_DTSEG2_MASK)
                                                /* Bits 16-21: Reserved */
#define CAN_EDCBT_DRJW_SHIFT          (22)      /* Bits 22-25: Data Phase Resynchronization Jump Width */
#define CAN_EDCBT_DRJW_MASK           (0x0f << CAN_EDCBT_DRJW_SHIFT)
#define CAN_EDCBT_DRJW(x)             (((x) << CAN_EDCBT_DRJW_SHIFT) & CAN_EDCBT_DRJW_MASK)
                                                /* Bits 26-31: Reserved */

/* Enhanced Transceiver Delay Compensation */

#define CAN_ETDC_ETDCVAL_SHIFT        (0)       /* Bits 0-7: Enhanced Transceiver Delay Compensation Value */
#define CAN_ETDC_ETDCVAL_MASK         (0xff << CAN_ETDC_ETDCVAL_SHIFT)
#define CAN_ETDC_ETDCVAL(x)           (((x) << CAN_ETDC_ETDCVAL_SHIFT) & CAN_ETDC_ETDCVAL_MASK)
                                                /* Bits 8-14: Reserved */
#define CAN_ETDC_ETDCFAIL             (1 << 15) /* Bit 15: Transceiver Delay Compensation Fail */
#define CAN_ETDC_ETDCOFF_SHIFT        (16)      /* Bits 16-22: Enhanced Transceiver Delay Compensation Offset */
#define CAN_ETDC_ETDCOFF_MASK         (0x7f << CAN_ETDC_ETDCOFF_SHIFT)
#define CAN_ETDC_ETDCOFF(x)           (((x) << CAN_ETDC_ETDCOFF_SHIFT) & CAN_ETDC_ETDCOFF_MASK)
                                                /* Bits 23-29: Reserved */
#define CAN_ETDC_TDMDIS               (1 << 30) /* Bit 30: Transceiver Delay Measurement Disable */
#define CAN_ETDC_ETDCEN               (1 << 31) /* Bit 31: Transceiver Delay Compensation Enable */

/* CAN FD Control register */

#define CAN_FDCTRL_TDCVAL_SHIFT       (0)       /* Bits 0-5: Transceiver Delay Compensation Value */
#define CAN_FDCTRL_TDCVAL_MASK        (0x3f << CAN_FDCTRL_TDCVAL_SHIFT)
#define CAN_FDCTRL_TDCVAL(x)          (((x) << CAN_FDCTRL_TDCVAL_SHIFT) & CAN_FDCTRL_TDCVAL_MASK)
                                                /* Bits 6-7: Reserved */
#define CAN_FDCTRL_TDCOFF_SHIFT       (8)       /* Bits 8-12: Transceiver Delay Compensation Offset */
#define CAN_FDCTRL_TDCOFF_MASK        (0x1f << CAN_FDCTRL_TDCOFF_SHIFT)
#define CAN_FDCTRL_TDCOFF(x)          (((x) << CAN_FDCTRL_TDCOFF_SHIFT) & CAN_FDCTRL_TDCOFF_MASK)
                                                /* Bit 13: Reserved */
#define CAN_FDCTRL_TDCFAIL            (1 << 14) /* Bit 14: Transceiver Delay Compensation Fail */
#define CAN_FDCTRL_TDCEN              (1 << 15) /* Bit 15: Transceiver Delay Compensation Enable */
#define CAN_FDCTRL_MBDSR0_SHIFT       (16)      /* Bits 16-17: Message Buffer Data Size for Region 0 */
#define CAN_FDCTRL_MBDSR0_MASK        (0x03 << CAN_FDCTRL_MBDSR0_SHIFT)
#define CAN_FDCTRL_MBDSR0(x)          (((x) << CAN_FDCTRL_MBDSR0_SHIFT) & CAN_FDCTRL_MBDSR0_MASK)
                                                /* Bits 18-30: Reserved */
#define CAN_FDCTRL_FDRATE             (1 << 31) /* Bit 31: Bit Rate Switch Enable */

/* CAN FD Bit Timing Register */

#define CAN_FDCBT_FPSEG2_SHIFT        (0)       /* Bits 0-2: Fast Phase Segment 2 */
#define CAN_FDCBT_FPSEG2_MASK         (0x07 << CAN_FDCBT_FPSEG2_SHIFT)
#define CAN_FDCBT_FPSEG2(x)           (((x) << CAN_FDCBT_FPSEG2_SHIFT) & CAN_FDCBT_FPSEG2_MASK)
                                                /* Bits 3-4: Reserved */
#define CAN_FDCBT_FPSEG1_SHIFT        (5)       /* Bits 5-7: Fast Phase Segment 1 */
#define CAN_FDCBT_FPSEG1_MASK         (0x07 << CAN_FDCBT_FPSEG1_SHIFT)
#define CAN_FDCBT_FPSEG1(x)           (((x) << CAN_FDCBT_FPSEG1_SHIFT) & CAN_FDCBT_FPSEG1_MASK)
                                                /* Bits 8-9: Reserved */
#define CAN_FDCBT_FPROPSEG_SHIFT      (10)      /* Bits 10-14: Fast Propagation Segment */
#define CAN_FDCBT_FPROPSEG_MASK       (0x1f << CAN_FDCBT_FPROPSEG_SHIFT)
#define CAN_FDCBT_FPROPSEG(x)         (((x) << CAN_FDCBT_FPROPSEG_SHIFT) & CAN_FDCBT_FPROPSEG_MASK)
                                                /* Bit 15: Reserved */
#define CAN_FDCBT_FRJW_SHIFT          (16)      /* Bits 16-18: Fast Resync Jump Width */
#define CAN_FDCBT_FRJW_MASK           (0x07 << CAN_FDCBT_FRJW_SHIFT)
#define CAN_FDCBT_FRJW(x)             (((x) << CAN_FDCBT_FRJW_SHIFT) & CAN_FDCBT_FRJW_MASK)
                                                /* Bit 19: Reserved */
#define CAN_FDCBT_FPRESDIV_SHIFT      (20)      /* Bits 20-29: Fast Prescaler Division Factor */
#define CAN_FDCBT_FPRESDIV_MASK       (0x03ff << CAN_FDCBT_FPRESDIV_SHIFT)
#define CAN_FDCBT_FPRESDIV(x)         (((x) << CAN_FDCBT_FPRESDIV_SHIFT) & CAN_FDCBT_FPRESDIV_MASK)
                                                /* Bits 30-31: Reserved */

/* CAN FD CRC Register */

#define CAN_FDCRC_FD_TXCRC_SHIFT      (0)       /* Bits 0-20: Extended Transmitted CRC value */
#define CAN_FDCRC_FD_TXCRC_MASK       (0x1fffff << CAN_FDCRC_FD_TXCRC_SHIFT)
#define CAN_FDCRC_FD_TXCRC(x)         (((x) << CAN_FDCRC_FD_TXCRC_SHIFT) & CAN_FDCRC_FD_TXCRC_MASK)
                                                /* Bits 21-23: Reserved */
#define CAN_FDCRC_FD_MBCRC_SHIFT      (24)      /* Bits 24-30: CRC Mailbox Number for FD_TXCRC */
#define CAN_FDCRC_FD_MBCRC_MASK       (0x7f << CAN_FDCRC_FD_MBCRC_SHIFT)
#define CAN_FDCRC_FD_MBCRC(x)         (((x) << CAN_FDCRC_FD_MBCRC_SHIFT) & CAN_FDCRC_FD_MBCRC_MASK)
                                                /* Bit 31: Reserved */

/* Enhanced Rx FIFO Control Register */

#define CAN_ERFCR_ERFWM_SHIFT         (0)       /* Bits 0-4: Enhanced Rx FIFO Watermark */
#define CAN_ERFCR_ERFWM_MASK          (0x1f << CAN_ERFCR_ERFWM_SHIFT)
#define CAN_ERFCR_ERFWM(x)            (((x) << CAN_ERFCR_ERFWM_SHIFT) & CAN_ERFCR_ERFWM_MASK)
                                                /* Bits 5-7: Reserved */
#define CAN_ERFCR_NFE_SHIFT           (8)       /* Bits 8-13: Number of Enhanced Rx FIFO Filter Elements */
#define CAN_ERFCR_NFE_MASK            (0x3f << CAN_ERFCR_NFE_SHIFT)
#define CAN_ERFCR_NFE(x)              (((x) << CAN_ERFCR_NFE_SHIFT) & CAN_ERFCR_NFE_MASK)
                                                /* Bits 14-15: Reserved */
#define CAN_ERFCR_NEXIF_SHIFT         (16)      /* Bits 16-22: Number of Extended ID Filter Elements */
#define CAN_ERFCR_NEXIF_MASK          (0x7f << CAN_ERFCR_NEXIF_SHIFT)
#define CAN_ERFCR_NEXIF(x)            (((x) << CAN_ERFCR_NEXIF_SHIFT) & CAN_ERFCR_NEXIF_MASK)
                                                /* Bits 23-25: Reserved */
#define CAN_ERFCR_DMALW_SHIFT         (26)      /* Bits 26-30: DMA Last Word */
#define CAN_ERFCR_DMALW_MASK          (0x1f << CAN_ERFCR_DMALW_SHIFT)
#define CAN_ERFCR_DMALW(x)            (((x) << CAN_ERFCR_DMALW_SHIFT) & CAN_ERFCR_DMALW_MASK)
#define CAN_ERFCR_ERFEN               (1 << 31) /* Bit 31: Enhanced Rx FIFO enable */

/* Enhanced Rx FIFO Interrupt Enable Register */

                                                /* Bits 0-27: Reserved */
#define CAN_ERFIER_ERFDAIE            (1 << 28) /* Bit 28: Enhanced Rx FIFO Data Available Interrupt Enable */
#define CAN_ERFIER_ERFWMIIE           (1 << 29) /* Bit 29: Enhanced Rx FIFO Watermark Indication Interrupt Enable */
#define CAN_ERFIER_ERFOVFIE           (1 << 30) /* Bit 30: Enhanced Rx FIFO Overflow Interrupt Enable */
#define CAN_ERFIER_ERFUFWIE           (1 << 31) /* Bit 31: Enhanced Rx FIFO Underflow Interrupt Enable */

/* Enhanced Rx FIFO Status Register */

#define CAN_ERFSR_ERFEL_SHIFT         (0)       /* Bits 0-5: Enhanced Rx FIFO Elements */
#define CAN_ERFSR_ERFEL_MASK          (0x3f << CAN_ERFSR_ERFEL_SHIFT)
                                                /* Bits 6-15: Reserved */
#define CAN_ERFSR_ERFF                (1 << 16) /* Bit 16: Enhanced Rx FIFO Full */
#define CAN_ERFSR_ERFE                (1 << 17) /* Bit 17: Enhanced Rx FIFO Empty */
                                                /* Bits 18-26: Reserved */
#define CAN_ERFSR_ERFCLR              (1 << 27) /* Bit 27: Enhanced Rx FIFO Clear */
#define CAN_ERFSR_ERFDA               (1 << 28) /* Bit 28: Enhanced Rx FIFO Data Available */
#define CAN_ERFSR_ERFWMI              (1 << 29) /* Bit 29: Enhanced Rx FIFO Watermark Indication */
#define CAN_ERFSR_ERFOVF              (1 << 30) /* Bit 30: Enhanced Rx FIFO Overflow */
#define CAN_ERFSR_ERFUFW              (1 << 31) /* Bit 31: Enhanced Rx FIFO Underflow */

/* Enhanced Rx FIFO Register */

/* Enhanced Rx FIFO Filter Element n */

/* CAN MB TX codes */

#define CAN_TXMB_INACTIVE             0x8       /* MB is not active. */
#define CAN_TXMB_ABORT                0x9       /* MB is aborted. */
#define CAN_TXMB_DATAORREMOTE         0xc       /* MB is a TX Data Frame(when MB RTR = 0) or
                                                 * MB is a TX Remote Request Frame (when MB RTR = 1). */
#define CAN_TXMB_TANSWER              0xe       /* MB is a TX Response Request Frame from an incoming Remote Request Frame. */
#define CAN_TXMB_NOTUSED              0xf       /* Not used. */

#endif /* __VENDOR_FLAGCHIP_CHIPS_FC7300_HARDWARE_FC7300_FLEXCAN_H */
