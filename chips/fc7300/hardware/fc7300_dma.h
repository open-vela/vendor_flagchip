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

#ifndef __VENDOR_FLAGCHIP_CHIPS_FC7300_HARDWARE_FC7300_DMA_H
#define __VENDOR_FLAGCHIP_CHIPS_FC7300_HARDWARE_FC7300_DMA_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>
#include "hardware/fc7300_memorymap.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#define FC7300_DMA_NCHANNELS                32
#define FC7300_DMA_NDUMES                   2
#define FC7300_DMA_NDUMOS                   4

/* DMA Register Offsets *****************************************************/

#define FC7300_DMA_CR_OFFSET                0x0000  /* Control */
#define FC7300_DMA_ES_OFFSET                0x0004  /* Error Status */
#define FC7300_DMA_ERQ_OFFSET               0x000C  /* Enable Request */
#define FC7300_DMA_EEI_OFFSET               0x0014  /* Enable Error Interrupt */
#define FC7300_DMA_CEEI_OFFSET              0x0018  /* Clear Enable Error Interrupt */
#define FC7300_DMA_SEEI_OFFSET              0x0019  /* Set Enable Error Interrupt */
#define FC7300_DMA_CERQ_OFFSET              0x001A  /* Clear Enable Request */
#define FC7300_DMA_SERQ_OFFSET              0x001B  /* Set Enable Request */
#define FC7300_DMA_CDNE_OFFSET              0x001C  /* Clear DONE Status Bit */
#define FC7300_DMA_SSRT_OFFSET              0x001D  /* Set START Bit */
#define FC7300_DMA_CERR_OFFSET              0x001E  /* Clear Error */
#define FC7300_DMA_CINT_OFFSET              0x001F  /* Clear Interrupt Request */
#define FC7300_DMA_INT_OFFSET               0x0024  /* Interrupt Request */
#define FC7300_DMA_ERR_OFFSET               0x002C  /* Error */
#define FC7300_DMA_HRS_OFFSET               0x0034  /* Hardware Request Status */

#define FC7300_DMA_DUME_OFFSET(n)           (0x0048 + ((n) * 4))
#define FC7300_DMA_DUME0_OFFSET             0x0048  /* DMA Unalign Modulo 0 Enable */
#define FC7300_DMA_DUME1_OFFSET             0x004C  /* DMA Unalign Modulo 1 Enable */

#define FC7300_DMA_DUMO_OFFSET(n)           (0x0050 + ((n) * 4))
#define FC7300_DMA_DUMO0_OFFSET             0x0050  /* DMA Unalign Modulo 0 Offset */
#define FC7300_DMA_DUMO1_OFFSET             0x0054  /* DMA Unalign Modulo 1 Offset */
#define FC7300_DMA_DUMO2_OFFSET             0x0058  /* DMA Unalign Modulo 2 Offset */
#define FC7300_DMA_DUMO3_OFFSET             0x005C  /* DMA Unalign Modulo 3 Offset */

#define FC7300_DMA_DCHPRI_OFFSET(n)         (0x0100 + ((n) & ~3) + (3 - ((n) & 3)))

#define FC7300_DMA_DCHPRI3_OFFSET           0x0100  /* Channel 3 Priority */
#define FC7300_DMA_DCHPRI2_OFFSET           0x0101  /* Channel 2 Priority */
#define FC7300_DMA_DCHPRI1_OFFSET           0x0102  /* Channel 1 Priority */
#define FC7300_DMA_DCHPRI0_OFFSET           0x0103  /* Channel 0 Priority */
#define FC7300_DMA_DCHPRI7_OFFSET           0x0104  /* Channel 7 Priority */
#define FC7300_DMA_DCHPRI6_OFFSET           0x0105  /* Channel 6 Priority */
#define FC7300_DMA_DCHPRI5_OFFSET           0x0106  /* Channel 5 Priority */
#define FC7300_DMA_DCHPRI4_OFFSET           0x0107  /* Channel 4 Priority */
#define FC7300_DMA_DCHPRI11_OFFSET          0x0108  /* Channel 11 Priority */
#define FC7300_DMA_DCHPRI10_OFFSET          0x0109  /* Channel 10 Priority */
#define FC7300_DMA_DCHPRI9_OFFSET           0x010A  /* Channel 9 Priority */
#define FC7300_DMA_DCHPRI8_OFFSET           0x010B  /* Channel 8 Priority */
#define FC7300_DMA_DCHPRI15_OFFSET          0x010C  /* Channel 15 Priority */
#define FC7300_DMA_DCHPRI14_OFFSET          0x010D  /* Channel 14 Priority */
#define FC7300_DMA_DCHPRI13_OFFSET          0x010E  /* Channel 13 Priority */
#define FC7300_DMA_DCHPRI12_OFFSET          0x010F  /* Channel 12 Priority */
#define FC7300_DMA_DCHPRI19_OFFSET          0x0110  /* Channel 19 Priority */
#define FC7300_DMA_DCHPRI18_OFFSET          0x0111  /* Channel 18 Priority */
#define FC7300_DMA_DCHPRI17_OFFSET          0x0112  /* Channel 17 Priority */
#define FC7300_DMA_DCHPRI16_OFFSET          0x0113  /* Channel 16 Priority */
#define FC7300_DMA_DCHPRI23_OFFSET          0x0114  /* Channel 23 Priority */
#define FC7300_DMA_DCHPRI22_OFFSET          0x0115  /* Channel 22 Priority */
#define FC7300_DMA_DCHPRI21_OFFSET          0x0116  /* Channel 21 Priority */
#define FC7300_DMA_DCHPRI20_OFFSET          0x0117  /* Channel 20 Priority */
#define FC7300_DMA_DCHPRI27_OFFSET          0x0118  /* Channel 27 Priority */
#define FC7300_DMA_DCHPRI26_OFFSET          0x0119  /* Channel 26 Priority */
#define FC7300_DMA_DCHPRI25_OFFSET          0x011A  /* Channel 25 Priority */
#define FC7300_DMA_DCHPRI24_OFFSET          0x011B  /* Channel 24 Priority */
#define FC7300_DMA_DCHPRI31_OFFSET          0x011C  /* Channel 31 Priority */
#define FC7300_DMA_DCHPRI30_OFFSET          0x011D  /* Channel 30 Priority */
#define FC7300_DMA_DCHPRI29_OFFSET          0x011E  /* Channel 29 Priority */
#define FC7300_DMA_DCHPRI28_OFFSET          0x011F  /* Channel 28 Priority */

/* DMA Channel Configurations (CFG) register Offsets */

#define FC7300_DMA_CFG_OFFSET(n)            (0x1000 + ((n) << 5))

#define FC7300_DMA_CFG_SADDR_OFFSET         0x0000  /* CFG Source Address */
#define FC7300_DMA_CFG_SOFF_OFFSET          0x0004  /* CFG Signed Source Address Offset */
#define FC7300_DMA_CFG_ATTR_OFFSET          0x0006  /* CFG Transfer Attributes */
#define FC7300_DMA_CFG_NBYTES_IL_OFFSET     0x0008  /* CFG Signed Inner Loop Offset / Byte Count */
#define FC7300_DMA_CFG_SLAST_OFFSET         0x000C  /* CFG Last Source Address Adjustment */
#define FC7300_DMA_CFG_DADDR_OFFSET         0x0010  /* CFG Destination Address */
#define FC7300_DMA_CFG_DOFF_OFFSET          0x0014  /* CFG Signed Destination Address Offset */
#define FC7300_DMA_CFG_CLC_CHTRG_OFFSET     0x0016  /* CFG Current Inner Loop Trig, Outer Loop Count */
#define FC7300_DMA_CFG_DLAST_OFFSET         0x0018  /* CFG Last Destination Address Adjustment */
#define FC7300_DMA_CFG_CSR_OFFSET           0x001C  /* CFG Control and Status */
#define FC7300_DMA_CFG_BLC_CHTRG_OFFSET     0x001E  /* CFG Beginning Inner Loop Trig, Outer Loop Count */

/* DMA Monitor Register Offsets */

#define FC7300_DMA_MON_ERR_STATUS_OFFSET    0x1480  /* Monitor Error Status Register */
#define FC7300_DMA_MON_FAKE_SRC_ADDR_OFFSET 0x1484  /* Monitor Fake Source Address Register */
#define FC7300_DMA_MON_FAKE_DST_ADDR_OFFSET 0x1488  /* Monitor Fake Destination Address Register */
#define FC7300_DMA_MON_CTRL_OFFSET          0x148C  /* Monitor Control Register */

/* DMA Bit-Field Definitions ************************************************/

/* Control */

                                                      /* Bit 0:  Reserved */
#define DMA_CR_DBGS                         (1 << 1)  /* Bit 1:  Enable Debug */
#define DMA_CR_ERCA                         (1 << 2)  /* Bit 2:  Enable Round Robin Channel Arbitration */
                                                      /* Bit 3:  Reserved */
#define DMA_CR_HOE                          (1 << 4)  /* Bit 4:  Halt On Error */
#define DMA_CR_HALT                         (1 << 5)  /* Bit 5:  Halt DMA Operations */
#define DMA_CR_CTM                          (1 << 6)  /* Bit 6:  Continuous Trig Mode */
#define DMA_CR_EILM                         (1 << 7)  /* Bit 7:  Enable Inner Loop Mapping */
                                                      /* Bit 8-15:  Reserved */
#define DMA_CR_ECX                          (1 << 16) /* Bit 16: Error Cancel Transfer */
#define DMA_CR_CX                           (1 << 17) /* Bit 17: Cancel Transfer */
                                                      /* Bits 18-23: Reserved */
                                                      /* Bits 24-30: DMA version number (reserved) */
#define DMA_CR_ACTIVE                       (1 << 31) /* Bit 31: DMA Active Status */

/* Error Status */

#define DMA_ES_DBE                          (1 << 0)  /* Bit 0:  Destination Bus Error */
#define DMA_ES_SBE                          (1 << 1)  /* Bit 1:  Source Bus Error */
                                                      /* Bit 2:  Reserved */
#define DMA_ES_NCE                          (1 << 3)  /* Bit 3:  NBYTES/CLC Configuration Error */
#define DMA_ES_DOE                          (1 << 4)  /* Bit 4:  Destination Offset Error */
#define DMA_ES_DAE                          (1 << 5)  /* Bit 5:  Destination Address Error */
#define DMA_ES_SOE                          (1 << 6)  /* Bit 6:  Source Offset Error */
#define DMA_ES_SAE                          (1 << 7)  /* Bit 7:  Source Address Error */
#define DMA_ES_ERRCHN_SHIFT                 (8)       /* Bits 8-11: Error Channel Number or
                                                       *         Canceled Channel Number */
#define DMA_ES_ERRCHN_MASK                  (15 << DMA_ES_ERRCHN_SHIFT)
                                                      /* Bits 23-13: Reserved */
#define DMA_ES_CPE                          (1 << 14) /* Bit 14: Channel Priority Error */
                                                      /* Bit 15:  Reserved */
#define DMA_ES_ECX                          (1 << 16) /* Bit 16: Transfer Canceled */
                                                      /* Bits 17-30: Reserved */
#define DMA_ES_VLD                          (1 << 31) /* Bit 31: Logical OR of all ERR status bits */

/* Enable Request */

#define DMA_ERQ(n)                          ((uint32_t)1 << (n)) /* Bit n:  Enable DMA request n */

/* Enable Error Interrupt */

#define DMA_EEI(n)                          ((uint32_t)1 << (n)) /* Bit n:  Enable error interrupt n */

/* Clear Enable Error Interrupt */

#define DMA_CEEI_SHIFT                      (0)       /* Bits 0-3: Clear Enable Error Interrupt */
#define DMA_CEEI_MASK                       (15 << DMA_CEEI_SHIFT)
#  define DMA_CEEI(n)                       ((uint32_t)(n) << DMA_CEEI_SHIFT)
                                                      /* Bit 54-:  Reserved */
#define DMA_CEEI_CAEE                       (1 << 6)  /* Bit 6:  Clear All Enable Error Interrupts */
#define DMA_CEEI_NOP                        (1 << 7)  /* Bit 7:  No Op enable */

/* Set Enable Error Interrupt */

#define DMA_SEEI_SHIFT                      (0)       /* Bits 0-3: Set Enable Error Interrupt */
#define DMA_SEEI_MASK                       (15 << DMA_SEEI_SHIFT)
#  define DMA_SEEI(n)                       ((uint32_t)(n) << DMA_SEEI_SHIFT)
                                                      /* Bit 54-:  Reserved */
#define DMA_SEEI_SAEE                       (1 << 6)  /* Bit 6:  Set All Enable Error Interrupts */
#define DMA_SEEI_NOP                        (1 << 7)  /* Bit 7:  No Op enable */

/* Clear Enable Request */

#define DMA_CERQ_SHIFT                      (0)       /* Bits 0-3: Clear Enable Request */
#define DMA_CERQ_MASK                       (15 << DMA_CERQ_SHIFT)
#  define DMA_CERQ(n)                       ((uint32_t)(n) << DMA_CERQ_SHIFT)
                                                      /* Bit 4-5:  Reserved */
#define DMA_CERQ_CAER                       (1 << 6)  /* Bit 6:  Clear All Enable Requests */
#define DMA_CERQ_NOP                        (1 << 7)  /* Bit 7:  No Op enable */

/* Set Enable Request */

#define DMA_SERQ_SHIFT                      (0)       /* Bits 0-3: Set Enable Request */
#define DMA_SERQ_MASK                       (15 << DMA_SERQ_SHIFT)
#  define DMA_SERQ(n)                       ((uint32_t)(n) << DMA_SERQ_SHIFT)
                                                      /* Bit 4-5:  Reserved */
#define DMA_SERQ_SAER                       (1 << 6)  /* Bit 6:  Set All Enable Requests */
#define DMA_SERQ_NOP                        (1 << 7)  /* Bit 7:  No Op enable */

/* Clear DONE Status Bit */

#define DMA_CDNE_SHIFT                      (0)       /* Bits 0-3: Clear DONE Bit */
#define DMA_CDNE_MASK                       (15 << DMA_CDNE_SHIFT)
#  define DMA_CDNE(n)                       ((uint32_t)(n) << DMA_CDNE_SHIFT)
                                                      /* Bit 4-5:  Reserved */
#define DMA_CDNE_CADN                       (1 << 6)  /* Bit 6:  Clears All DONE Bits */
#define DMA_CDNE_NOP                        (1 << 7)  /* Bit 7:  No Op enable */

/* Set START Bit */

#define DMA_SSRT_SHIFT                      (0)       /* Bits 0-3: Set START Bit */
#define DMA_SSRT_MASK                       (15 << DMA_SSRT_SHIFT)
#  define DMA_SSRT(n)                       ((uint32_t)(n) << DMA_SSRT_SHIFT)
                                                      /* Bit 4-5:  Reserved */
#define DMA_SSRT_SAST                       (1 << 6)  /* Bit 6:  Set All START Bits (activates all channels) */
#define DMA_SSRT_NOP                        (1 << 7)  /* Bit 7:  No Op enable */

/* Clear Error */

#define DMA_CERR_SHIFT                      (0)       /* Bits 0-3: Clear Error Indicator */
#define DMA_CERR_MASK                       (15 << DMA_CERR_SHIFT)
#  define DMA_CERR(n)                       ((uint32_t)(n) << DMA_CERR_SHIFT)
                                                      /* Bit 4-5:  Reserved */
#define DMA_CERR_CAEI                       (1 << 6)  /* Bit 6:  Clear All Error Indicators */
#define DMA_CERR_NOP                        (1 << 7)  /* Bit 7:  No Op enable */

/* Clear Interrupt Request */

#define DMA_CINT_SHIFT                      (0)       /* Bits 0-3: Clear Interrupt Request */
#define DMA_CINT_MASK                       (15 << DMA_CINT_SHIFT)
#  define DMA_CINT(n)                       ((uint32_t)(n) << DMA_CINT_SHIFT)
                                                      /* Bit 4-5:  Reserved */
#define DMA_CINT_CAIR                       (1 << 6)  /* Bit 6:  Clear All Interrupt Requests */
#define DMA_CINT_NOP                        (1 << 7)  /* Bit 7:  No Op enable */

/* Interrupt Request */

#define DMA_INT(n)                          ((uint32_t)1 << (n)) /* Bit n:  Interrupt Request n */

/* Error */

#define DMA_ERR(n)                          ((uint32_t)1 << (n)) /* Bit n:  Error In Channel n */

/* Hardware Request Status */

#define DMA_HRS(n)                          ((uint32_t)1 << (n)) /* Bit n:  Hardware Request Status
                                                                  * Channel n */

/* DMA Unalign Modulo Enable */

#define DMA_DUME_DUME(n)                    ((uint32_t)1 << (2 * (n))) /* Bit 2n:  Enable destination address
                                                                        * unalign modulo for channel n */
#define DMA_DUME_SUME(n)                    ((uint32_t)2 << (2 * (n))) /* Bit 2n + 1:  Enable source address
                                                                        * unalign modulo for channel n */

/* DMA Unalign Modulo */
#define DMA_DUMO_DUMO_SHIFT                 (0)       /* Bits 0-7: Destination address unalign modulo */
#define DMA_DUMO_DUMO_MASK                  (0xff << DMA_DUMO_DUMO_SHIFT)
#  define DMA_DUMO_DUMO(n)                  ((uint32_t)(n) << DMA_DUMO_DUMO_SHIFT)
#define DMA_DUMO_SUMO_SHIFT                 (8)       /* Bits 0-7: Source address unalign modulo */
#define DMA_DUMO_SUMO_MASK                  (0xff << DMA_DUMO_SUMO_SHIFT)
#  define DMA_DUMO_SUMO(n)                  ((uint32_t)(n) << DMA_DUMO_SUMO_SHIFT)

/* Channel n Priority */

#define DMA_DCHPRI_CHPRI_SHIFT              (0)       /* Bits 0-3: Channel n Arbitration Priority */
#define DMA_DCHPRI_CHPRI_MASK               (15 << DMA_DCHPRI_CHPRI_SHIFT)
#  define DMA_DCHPRI_CHPRI(n)               ((uint32_t)(n) << DMA_DCHPRI_CHPRI_SHIFT)
                                                      /* Bit 4-7:  Reserved */

/* CFG Source Address (32-bit address) */

/* CFG Signed Source Address Offset (16-bit offset) */

/* CFG Transfer Attributes */

#define CFG_ATTR_SIZE_8BIT                  (0)       /* 8-bit */
#define CFG_ATTR_SIZE_16BIT                 (1)       /* 16-bit */
#define CFG_ATTR_SIZE_32BIT                 (2)       /* 32-bit */
#define CFG_ATTR_SIZE_16BYTE                (4)       /* 16-byte burst */
#define CFG_ATTR_SIZE_32BYTE                (5)       /* 32-byte burst */

#define DMA_CFG_ATTR_DSIZE_SHIFT            (0)       /* Bits 0-2: Destination data transfer size */
#define DMA_CFG_ATTR_DSIZE_MASK             (7 << DMA_CFG_ATTR_DSIZE_SHIFT)
#  define DMA_CFG_ATTR_DSIZE(n)             ((uint32_t)(n)        << DMA_CFG_ATTR_DSIZE_SHIFT) /* 8-bit */
#  define DMA_CFG_ATTR_DSIZE_8BIT           (CFG_ATTR_SIZE_8BIT   << DMA_CFG_ATTR_DSIZE_SHIFT) /* 8-bit */
#  define DMA_CFG_ATTR_DSIZE_16BIT          (CFG_ATTR_SIZE_16BIT  << DMA_CFG_ATTR_DSIZE_SHIFT) /* 16-bit */
#  define DMA_CFG_ATTR_DSIZE_32BIT          (CFG_ATTR_SIZE_32BIT  << DMA_CFG_ATTR_DSIZE_SHIFT) /* 32-bit */
#  define DMA_CFG_ATTR_DSIZE_16BYTE         (CFG_ATTR_SIZE_16BYTE << DMA_CFG_ATTR_DSIZE_SHIFT) /* 64-bit */
#  define DMA_CFG_ATTR_DSIZE_32BYTE         (CFG_ATTR_SIZE_32BYTE << DMA_CFG_ATTR_DSIZE_SHIFT) /* 32-byte burst */

#define DMA_CFG_ATTR_DMOD_SHIFT             (3)       /* Bits 3-7: Destination Address Modulo */
#define DMA_CFG_ATTR_DMOD_MASK              (31 << DMA_CFG_ATTR_DMOD_SHIFT)
#  define DMA_CFG_ATTR_DMOD(n)              ((uint32_t)(n) << DMA_CFG_ATTR_DMOD_SHIFT)

#define DMA_CFG_ATTR_SSIZE_SHIFT            (8)       /* Bits 8-10: Source data transfer size */
#define DMA_CFG_ATTR_SSIZE_MASK             (7 << DMA_CFG_ATTR_SSIZE_SHIFT)
#  define DMA_CFG_ATTR_SSIZE(n)             ((uint32_t)(n)        << DMA_CFG_ATTR_SSIZE_SHIFT) /* 8-bit */
#  define DMA_CFG_ATTR_SSIZE_8BIT           (CFG_ATTR_SIZE_8BIT   << DMA_CFG_ATTR_SSIZE_SHIFT) /* 8-bit */
#  define DMA_CFG_ATTR_SSIZE_16BIT          (CFG_ATTR_SIZE_16BIT  << DMA_CFG_ATTR_SSIZE_SHIFT) /* 16-bit */
#  define DMA_CFG_ATTR_SSIZE_32BIT          (CFG_ATTR_SIZE_32BIT  << DMA_CFG_ATTR_SSIZE_SHIFT) /* 32-bit */
#  define DMA_CFG_ATTR_SSIZE_16BYTE         (CFG_ATTR_SIZE_16BYTE << DMA_CFG_ATTR_SSIZE_SHIFT) /* 64-bit */
#  define DMA_CFG_ATTR_SSIZE_32BYTE         (CFG_ATTR_SIZE_32BYTE << DMA_CFG_ATTR_SSIZE_SHIFT) /* 32-byte burst */

#define DMA_CFG_ATTR_SMOD_SHIFT             (11)      /* Bits 11-15: Source Address Modulo */
#define DMA_CFG_ATTR_SMOD_MASK              (31 << DMA_CFG_ATTR_SMOD_SHIFT)
#  define DMA_CFG_ATTR_SMOD(n)              ((uint32_t)(n) << DMA_CFG_ATTR_SMOD_SHIFT)

/* CFG Signed Inner Loop Offset / Byte Count */

/* Inner Loop Byte Count (Inner Loop Mapping Disabled -- 32-bit byte count) */

/* CFG Signed Inner Loop Offset / Byte Count */

/* Inner Loop Byte Count (Inner Loop Mapping Enabled, offset disabled) */

#define DMA_CFG_NBYTES_IL_NBYTES_SHIFT      (0)       /* Bits 0-29: Inner Loop Byte Transfer Count */
#define DMA_CFG_NBYTES_IL_NBYTES_MASK       (0x3fffffff << DMA_CFG_NBYTES_IL_NBYTES_SHIFT)
#  define DMA_CFG_NBYTES_IL_NBYTES(n)       ((uint32_t)(n) << DMA_CFG_NBYTES_IL_NBYTES_SHIFT)
#define DMA_CFG_NBYTES_IL_DILOE             (1 << 30) /* Bit 30: Destination Inner Loop Offset enable */
#define DMA_CFG_NBYTES_IL_SILOE             (1 << 31) /* Bit 31: Source Inner Loop Offset Enable */

/* CFG Signed Inner Loop Offset / Byte Count */

/* Inner Loop Byte Count (Inner Loop Mapping Enabled, offset enabled) */

#define DMA_CFG_NBYTES_ILOFF_NBYTES_SHIFT   (0)      /* Bits 0-9: Inner Loop Byte Transfer Count */
#define DMA_CFG_NBYTES_ILOFF_NBYTES_MASK    (0x3ff << DMA_CFG_NBYTES_ILOFF_NBYTES_SHIFT)
#  define DMA_CFG_NBYTES_ILOFF_NBYTES(n)    ((uint32_t)(n) << DMA_CFG_NBYTES_ILOFF_NBYTES_SHIFT)
#define DMA_CFG_NBYTES_ILOFF_ILOFF_SHIFT    (10)     /* Bits 10-29: Inner Loop Byte Transfer Count */
#define DMA_CFG_NBYTES_ILOFF_ILOFF_MASK     (0xfffff << DMA_CFG_NBYTES_ILOFF_ILOFF_SHIFT)
#  define DMA_CFG_NBYTES_ILOFF_ILOFF(n)     ((uint32_t)(n) << DMA_CFG_NBYTES_ILOFF_ILOFF_SHIFT)
#define DMA_CFG_NBYTES_ILOFF_DILOE          (1 << 30) /* Bit 30: Destination Inner Loop Offset enable */
#define DMA_CFG_NBYTES_ILOFF_SILOE          (1 << 31) /* Bit 31: Source Inner Loop Offset Enable */

/* CFG Last Source Address Adjustment (32-bit address adjustment) */

/* CFG Destination Address (32-bit address) */

/* CFG Signed Destination Address Offset (16-bit signed address offset) */

/* CFG Current Inner Loop Trig, Outer Loop Count (Channel trig disabled) */

#define DMA_CFG_CLC_CLC_SHIFT               (0)       /* Bit 0-14: Starting Major Iteration Count */
#define DMA_CFG_CLC_CLC_MASK                (0x7fff << DMA_CFG_CLC_CLC_SHIFT)
#  define DMA_CFG_CLC_CLC(n)                ((uint32_t)(n) << DMA_CFG_CLC_CLC_SHIFT)
#define DMA_CFG_CLC_CHTRGEN                 (1 << 15) /* Bit 15: Enable channel-to-channel trig on
                                                       * inner-loop complete */

/* CFG Current Inner Loop Trig, Outer Loop Count
 * (Channel trig enabled)
 */

#define DMA_CFG_CLC_CHTRG_CLC_SHIFT         (0)       /* Bit 0-8: Current outer iteration count */
#define DMA_CFG_CLC_CHTRG_CLC_MASK          (0x1ff << DMA_CFG_CLC_CHTRG_CLC_SHIFT)
#  define DMA_CFG_CLC_CHTRG_CLC(n)          ((uint32_t)(n) << DMA_CFG_CLC_CHTRG_CLC_SHIFT)
#define DMA_CFG_CLC_CHTRG_TRGCH_SHIFT       (9)       /* Bit 9-12: Inner Loop Trig Channel Number */
#define DMA_CFG_CLC_CHTRG_TRGCH_MASK        (15 << DMA_CFG_CLC_CHTRG_TRGCH_SHIFT)
#  define DMA_CFG_CLC_CHTRG_TRGCH(n)        ((uint32_t)(n) << DMA_CFG_CLC_CHTRG_TRGCH_SHIFT)
                                                      /* Bit 13-14: Reserved */
#define DMA_CFG_CLC_CHTRG_CHTRGEN           (1 << 15) /* Bit 15: Enable channel-to-channel trig on
                                                       * inner-loop complete */

/* CFG Last Destination Address Adjustment
 * (32-bit address)
 */

/* CFG Control and Status */

#define DMA_CFG_CSR_START                   (1 << 0)  /* Bit 0:  Channel Start */
#define DMA_CFG_CSR_INTOUTER                (1 << 1)  /* Bit 1:  Enable an interrupt when outer
                                                       *         loop count completes */
#define DMA_CFG_CSR_INTHALF                 (1 << 2)  /* Bit 2:  Enable an interrupt when outer
                                                       *         loop is half complete */
#define DMA_CFG_CSR_DREQ                    (1 << 3)  /* Bit 3:  Disable Request */
                                                      /* Bit 4:  Reserved */
#define DMA_CFG_CSR_OCHTRGEN                (1 << 5)  /* Bit 5:  Enable channel-to-channel trig
                                                       *         on outer loop complete */
#define DMA_CFG_CSR_ACTIVE                  (1 << 6)  /* Bit 6:  Channel Active */
#define DMA_CFG_CSR_DONE                    (1 << 7)  /* Bit 7:  Channel Done */
#define DMA_CFG_CSR_OTRGCH_SHIFT            (8)       /* Bits 8-11: Outer Loop Trig Channel Number */
#define DMA_CFG_CSR_OTRGCH_MASK             (15 << DMA_CFG_CSR_OTRGCH_SHIFT)
#  define DMA_CFG_CSR_OTRGCH(n)             ((uint32_t)(n) << DMA_CFG_CSR_OTRGCH_SHIFT)
#define DMA_CFG_CSR_UMS_SHIFT               (12)      /* Bits 12-13: Unalign Modulo Offset Select */
#define DMA_CFG_CSR_UMS_MASK                (3 << DMA_CFG_CSR_UMS_SHIFT)
#  define DMA_CFG_CSR_UMS(n)                ((uint32_t)(n) << DMA_CFG_CSR_UMS_SHIFT)
                                                       /* Bits 14-15: Reserved */

/* CFG Beginning Inner Loop Trig, Outer Loop Count
 * (Channel trig disabled)
 */

#define DMA_CFG_BLC_BLC_SHIFT               (0)       /* Bit 0-14: Starting Outer Loop Count */
#define DMA_CFG_BLC_BLC_MASK                (0x7fff << DMA_CFG_BLC_BLC_SHIFT)
#  define DMA_CFG_BLC_BLC(n)                ((uint32_t)(n) << DMA_CFG_BLC_BLC_SHIFT)
#define DMA_CFG_BLC_CHTRGEN                 (1 << 15) /* Bit 15: Enable channel-to-channel trig on
                                                       * inner-loop complete */

/* CFG Beginning Inner Loop Trig, Outer Loop Count
 * (Channel trig enabled)
 */

#define DMA_CFG_BLC_CHTRG_BLC_SHIFT         (0)       /* Bit 0-8: Starting Outer Loop Count */
#define DMA_CFG_BLC_CHTRG_BLC_MASK          (0x1ff << DMA_CFG_BLC_CHTRG_BLC_SHIFT)
#  define DMA_CFG_BLC_CHTRG_BLC(n)          ((uint32_t)(n) << DMA_CFG_BLC_CHTRG_BLC_SHIFT)
#define DMA_CFG_BLC_CHTRG_TRGCH_SHIFT       (9)       /* Bit 9-12: Trig Channel Number */
#define DMA_CFG_BLC_CHTRG_TRGCH_MASK        (15 << DMA_CFG_BLC_CHTRG_TRGCH_SHIFT)
#  define DMA_CFG_BLC_CHTRG_TRGCH(n)        ((uint32_t)(n) << DMA_CFG_BLC_CHTRG_TRGCH_SHIFT)
                                                      /* Bit 13-14: Reserved */
#define DMA_CFG_BLC_CHTRG_CHTRGEN           (1 << 15) /* Bit 15: Enable channel-to-channel trig on
                                                       * inner-loop complete */

/* Monitor Error Status Register */

#define DMA_MON_ERR_STATUS_D_ERROR          (1 << 0)  /* Bit 0:  Data Error */
#define DMA_MON_ERR_STATUS_WR_ADDR_CNT_ERR  (1 << 1)  /* Bit 1:  Write Address Count Error */
#define DMA_MON_ERR_STATUS_WR_ADDR_CMP_ERR  (1 << 2)  /* Bit 2:  Write Address Compare Error */
#define DMA_MON_ERR_STATUS_RD_ADDR_CNT_ERR  (1 << 3)  /* Bit 3:  Read Address Count Error */
#define DMA_MON_ERR_STATUS_RD_ADDR_CMP_ERR  (1 << 4)  /* Bit 4:  Read Address Compare Error */
                                                      /* Bits 5-31: Reserved */

/* Monitor Fake Source Address Register */

/* Monitor Fake Destination Address Register */

/* Monitor Control Register */

#define DMA_MON_CTRL_ERR_CLR                (1 << 0)  /* Bit 0:  DMA Monitor Error Status Clear */
                                                      /* Bits 1-28: Reserved */
#define DMA_MON_CTRL_MON_SCHK_LOCK          (1 << 29) /* Bit 29: DMA Monitor Self Check Enable Lock */
#define DMA_MON_CTRL_MON_SCHK_EN            (1 << 30) /* Bit 30: DMA Monitor Self Check Enable */
#define DMA_MON_CTRL_CHK_EN                 (1 << 31) /* Bit 31: DMA Monitor Check Enable */

/****************************************************************************
 * Public Types
 ****************************************************************************/

/* Hardware representation of the 32-byte DMA Channel Configuration (CFG)
 */

struct fc7300_dmacfg_s
{
  uint32_t saddr;         /* Offset: 0x0000  CFG Source Address */
  uint16_t soff;          /* Offset: 0x0004  CFG Signed Source Address Offset */
  uint16_t attr;          /* Offset: 0x0006  CFG Transfer Attributes */
  uint32_t nbytes;        /* Offset: 0x0008  CFG Signed Inner Loop Offset / Byte Count */
  uint32_t slast;         /* Offset: 0x000c  CFG Last Source Address Adjustment */
  uint32_t daddr;         /* Offset: 0x0010  CFG Destination Address */
  uint16_t doff;          /* Offset: 0x0014  CFG Signed Destination Address Offset */
  uint16_t clc;           /* Offset: 0x0016  CFG Current Inner Loop Trig, Outer Loop Count */
  uint32_t dlast;         /* Offset: 0x0018  CFG Last Destination Address Adjustment */
  uint16_t csr;           /* Offset: 0x001c  CFG Control and Status */
  uint16_t blc;           /* Offset: 0x001e  CFG Beginning Inner Loop Trig, Outer Loop Count */
};

#endif /* __VENDOR_FLAGCHIP_CHIPS_FC7300_HARDWARE_FC7300_DMA_H */
