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

#ifndef __VENDOR_FLAGCHIP_CHIPS_FC7300_FC7300_DMA_H
#define __VENDOR_FLAGCHIP_CHIPS_FC7300_FC7300_DMA_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>
#include <nuttx/dma/dma.h>

#include <stdint.h>
#include "hardware/fc7300_dma.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* DMA instance index */

#define DMA_INSTANCE_0                   (0)
#define DMA_INSTANCE_1                   (1)

/* Configuration flags. */

#define DMA_CONFIG_LOOP_SHIFT            (0) /* Bits 0-1: Loop type */
#define DMA_CONFIG_LOOP_MASK             (3 << DMA_CONFIG_LOOP_SHIFT)
#  define DMA_CONFIG_LOOPNONE            (0 << DMA_CONFIG_LOOP_SHIFT) /* No looping */
#  define DMA_CONFIG_LOOPSRC             (1 << DMA_CONFIG_LOOP_SHIFT) /* Source looping */
#  define DMA_CONFIG_LOOPDEST            (2 << DMA_CONFIG_LOOP_SHIFT) /* Dest looping */

#define DMA_CONFIG_INTHALF               (1 << 2) /* Bits 2: Int on HALF */
#define DMA_CONFIG_INTOUTER              (1 << 3) /* Bits 3: Int on outer loop completion */
#define DMA_CONFIG_AUTOSTOP              (1 << 4) /* Bits 4: Autostop when outer loop completed */

/* Circular Buffer Options */

#define DMA_CONFIG_CYCBUF_SRC            (1 << 5) /* Bit 5: Config source buffer as circular buffer */
#define DMA_CONFIG_CYCBUF_SRC_UMO        (1 << 6) /* Bit 6: Config source buffer as unaligned circular buffer */
#define DMA_CONFIG_CYCBUF_DST            (1 << 7) /* Bit 7: Config destination buffer as circular buffer */
#define DMA_CONFIG_CYCBUF_DST_UMO        (1 << 8) /* Bit 8: Config destination buffer as unaligned circular buffer */

/****************************************************************************
 * Public Types
 ****************************************************************************/

/* The following is used for sampling DMA registers when CONFIG DEBUG_DMA
 * is selected
 */

#ifdef CONFIG_DEBUG_DMA
struct fc7300_dmaregs_s
{
  uint8_t chidx;         /* Sampled channel */

  /* DMA Global Registers */

  uint32_t cr;           /* Control */
  uint32_t es;           /* Error Status */
  uint32_t erq;          /* Enable Request */
  uint32_t irq;          /* Interrupt Request */
  uint32_t err;          /* Error */
  uint32_t hrs;          /* Hardware Request Status */
  uint32_t dume;         /* DMA Unalign Modulo Enable */

  /* DMA Channel registers */

  uint8_t dchpri;        /* Channel priority */

  /* DMA CFG */

  uint32_t saddr;        /* CFG Source Address */
  uint16_t soff;         /* CFG Signed Source Address Offset */
  uint16_t attr;         /* CFG Transfer Attributes */
  uint32_t nbytes;       /* CFG Signed Inner Loop Offset / Byte Count */
  uint32_t slast;        /* CFG Last Source Address Adjustment */
  uint32_t daddr;        /* CFG Destination Address */
  uint16_t doff;         /* CFG Signed Destination Address Offset */
  uint16_t clc;          /* CFG Current Inner Loop Link, Outer Loop Count */
  uint32_t dlast;        /* CFG Last Destination Address Adjustment */
  uint16_t csr;          /* CFG Control and Status */
  uint16_t blc;          /* CFG Beginning Inner Loop Link, Outer Loop Count */

  /* DMAMUX registers */

  uint32_t dmamux;       /* Channel configuration */
};
#endif /* CONFIG_DEBUG_DMA */

/****************************************************************************
 * Inline Functions
 ****************************************************************************/

#ifndef __ASSEMBLY__

/****************************************************************************
 * Public Data
 ****************************************************************************/

#undef EXTERN
#if defined(__cplusplus)
#define EXTERN extern "C"
extern "C"
{
#else
#define EXTERN extern
#endif

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

/****************************************************************************
 * Name: fc7300_dma_initialize
 *
 * Description:
 *   Initialize the DMA instance
 *
 *   NOTE:  This assumes that board-specific logic has enabled clocking to
 *   the DMA module as part of the clock initialization logic.
 *
 * Input Parameters:
 *   dma_instance - The index of the DMA index
 *
 * Returned Value:
 *   The handle of the DMA instance.
 *
 ****************************************************************************/

struct dma_dev_s *fc7300_dma_initialize(uint8_t dma_instance);

/****************************************************************************
 * Name: fc7300_dma_getdev
 *
 * Description:
 *   Get the handle of the DMA instance
 *
 * Input Parameters:
 *   dma_instance - The index of the DMA index
 *
 * Returned Value:
 *   The handle of the DMA instance.
 *
 ****************************************************************************/

struct dma_dev_s *fc7300_dma_getdev(uint8_t dma_instance);

/****************************************************************************
 * Name: fc7300_dmasample
 *
 * Description:
 *   Sample DMA register contents
 *
 ****************************************************************************/

#ifdef CONFIG_DEBUG_DMA
void fc7300_dmasample(DMACH_HANDLE handle, struct fc7300_dmaregs_s *regs);
#else
#  define fc7300_dmasample(handle,regs)
#endif

/****************************************************************************
 * Name: fc7300_dmadump
 *
 * Description:
 *   Dump previously sampled DMA register contents
 *
 ****************************************************************************/

#ifdef CONFIG_DEBUG_DMA
void fc7300_dmadump(const struct fc7300_dmaregs_s *regs, const char *msg);
#else
#  define fc7300_dmadump(handle,regs,msg)
#endif

#undef EXTERN
#if defined(__cplusplus)
}
#endif

#endif /* __ASSEMBLY__ */
#endif /* __VENDOR_FLAGCHIP_CHIPS_FC7300_FC7300_DMA_H */
