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

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <debug.h>
#include <errno.h>

#include <nuttx/irq.h>
#include <nuttx/arch.h>
#include <nuttx/queue.h>
#include <nuttx/spinlock.h>
#include <nuttx/mutex.h>
#include <nuttx/semaphore.h>

#include "arm_internal.h"
#include "sched/sched.h"
#include <arch/barriers.h>

#include "chip.h"
#include "hardware/fc7300_dma.h"
#include "hardware/fc7300_dmamux.h"
#include "fc7300_dma.h"
#include "fc7300_irq.h"
#ifdef CONFIG_FC7300_DMA_SUPPORT_TCM
#  include "fc7300_multicore.h"
#endif

#ifdef CONFIG_FC7300_DMA

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#define FC7300_IRQ_DMACH(n)      (FC7300_IRQ_DMACH0 + (n))

#define FC7300_ITCM_START        0x00000000
#define FC7300_ITCM_END          0x0000FFFF
#define FC7300_DTCM_START        0x20000000
#define FC7300_DTCM_END          0x2001FFFF

/****************************************************************************
 * Private Functions
 ****************************************************************************/

static int fc7300_dma_chan_config(struct dma_chan_s *chan,
                                  const struct dma_config_s *cfg);
static int fc7300_dma_start(struct dma_chan_s *chan,
                            dma_callback_t callback, void *arg,
                            uintptr_t dst, uintptr_t src, size_t len);
static int fc7300_dma_start_cyclic(struct dma_chan_s *chan,
                                   dma_callback_t callback, void *arg,
                                   uintptr_t dst, uintptr_t src,
                                   size_t len, size_t period_len);
#ifdef CONFIG_DMA_LINK
static int fc7300_dma_start_link(struct dma_chan_s *chan,
                                 dma_callback_t callback, void *arg,
                                 unsigned int work_mode,
                                 struct dma_link_config_s *cfg);
#endif
static int fc7300_dma_stop(struct dma_chan_s *chan);
static int fc7300_dma_pause(struct dma_chan_s *chan);
static int fc7300_dma_resume(struct dma_chan_s *chan);
static size_t fc7300_dma_residual(struct dma_chan_s *chan);
#ifdef CONFIG_DMA_CYCBUF_TRANSMIT
static int fc7300_dma_chan_cycbuf_config(struct dma_chan_s *chan,
                                         unsigned int src,
                                         unsigned int src_len,
                                         unsigned int dst,
                                         unsigned int dst_len);
static unsigned int fc7300_dma_getrp(struct dma_chan_s *chan);
static unsigned int fc7300_dma_getwp(struct dma_chan_s *chan);
#endif

static int fc7300_dma_interrupt(int irq, void *context, void *arg);
static int fc7300_error_interrupt(int irq, void *context, void *arg);

static struct dma_chan_s *fc7300_dma_get_chan(struct dma_dev_s *dev,
                                              unsigned int ident);
static void fc7300_dma_put_chan(struct dma_dev_s *dev,
                                struct dma_chan_s *chan);

/****************************************************************************
 * Private Types
 ****************************************************************************/

/* State of a DMA channel */

enum fc7300_dmastate_e
{
  FC7300_DMA_UNINIT      = 0,    /* DMA is allocated, but not yet configured */
  FC7300_DMA_INITIALIZED = 1,    /* DMA configured, but not yet started or has stopped */
  FC7300_DMA_ACTIVE      = 2,    /* DMA has been started and is in progress */
  FC7300_DMA_PAUSED      = 3     /* DMA has been paused */
};

/* DMA transfer sizes */

enum fc7300_dma_sizes_e
{
  DMA_1BYTE   = 0,      /* Transfer data size 8 bits */
  DMA_2BYTE   = 1,      /* Transfer data size 16 bits */
  DMA_4BYTE   = 2,      /* Transfer data size 32 bits */
  DMA_8BYTE   = 3,      /* Transfer data size 64 bits */
  DMA_32BYTE  = 5       /* Transfer data size 256 bits */
};

/* This structure describes one DMA channel */

struct fc7300_dma_chan_s
{
  struct dma_chan_s chan;

  struct fc7300_dma_dev_s *priv;

  uint8_t  chidx;                 /* DMA channel number (0-FC7300_DMA_NCHANNELS) */
  uint8_t  state;                 /* Channel state.  See enum fc7300_dmastate_e */
  uint8_t  dmamux;                /* The DMAMUX channel selection */
  uint32_t option;                /* DMA channel flags */
  dma_callback_t callback;        /* Callback invoked when the DMA completes */
  void *arg;                      /* Argument passed to callback function */
};

/* This structure describes the state of the DMA controller */

struct fc7300_dma_dev_s
{
  struct dma_dev_s dev;

  uint32_t base;
  uint32_t muxbase;

  /* These mutex protect the DMA channel and descriptor tables */

  mutex_t chlock[FC7300_DMA_NCHANNELS];

#ifdef CONFIG_DMA_CYCBUF_TRANSMIT
  uint8_t dumo_refs[FC7300_DMA_NDUMOS];
#endif

  /* This array describes each DMA channel */

  struct fc7300_dma_chan_s *dmachs[FC7300_DMA_NCHANNELS];
#ifdef CONFIG_ARCH_IRQPRIO
  int dmachs_prio[FC7300_DMA_NCHANNELS];
  int error_isr_prio;
#endif

  bool cr_dbg;
  bool cr_rca;
  bool cr_hoe;
  bool cr_ctm;
};

/****************************************************************************
 * Private Data
 ****************************************************************************/

static const struct dma_ops_s g_fc7300_dma_ops =
{
  .config       = fc7300_dma_chan_config,
  .start        = fc7300_dma_start,
  .start_cyclic = fc7300_dma_start_cyclic,
#ifdef CONFIG_DMA_LINK
  .start_link   = fc7300_dma_start_link,
#endif
  .stop         = fc7300_dma_stop,
  .pause        = fc7300_dma_pause,
  .resume       = fc7300_dma_resume,
  .residual     = fc7300_dma_residual,
#ifdef CONFIG_DMA_CYCBUF_TRANSMIT
  .cycbuf_config  = fc7300_dma_chan_cycbuf_config,
  .cycbuf_getrp   = fc7300_dma_getrp,
  .cycbuf_getwp   = fc7300_dma_getwp,
#endif
};

#ifdef CONFIG_FC7300_DMA0
struct fc7300_dma_dev_s fc7300_dma0 =
{
  .dev =
    {
      .get_chan = fc7300_dma_get_chan,
      .put_chan = fc7300_dma_put_chan,
    },

  .base = FC7300_DMA0_BASE,
  .muxbase = FC7300_DMAMUX0_BASE,
#ifdef CONFIG_ARCH_IRQPRIO
  .dmachs_prio =
    {
      NVIC_DMA0_CHAN0_PRIORITY_DEFAULT,
      NVIC_DMA0_CHAN1_PRIORITY_DEFAULT,
      NVIC_DMA0_CHAN2_PRIORITY_DEFAULT,
      NVIC_DMA0_CHAN3_PRIORITY_DEFAULT,
      NVIC_DMA0_CHAN4_PRIORITY_DEFAULT,
      NVIC_DMA0_CHAN5_PRIORITY_DEFAULT,
      NVIC_DMA0_CHAN6_PRIORITY_DEFAULT,
      NVIC_DMA0_CHAN7_PRIORITY_DEFAULT,
      NVIC_DMA0_CHAN8_PRIORITY_DEFAULT,
      NVIC_DMA0_CHAN9_PRIORITY_DEFAULT,
      NVIC_DMA0_CHAN10_PRIORITY_DEFAULT,
      NVIC_DMA0_CHAN11_PRIORITY_DEFAULT,
      NVIC_DMA0_CHAN12_PRIORITY_DEFAULT,
      NVIC_DMA0_CHAN13_PRIORITY_DEFAULT,
      NVIC_DMA0_CHAN14_PRIORITY_DEFAULT,
      NVIC_DMA0_CHAN15_PRIORITY_DEFAULT,
      NVIC_DMA0_CHAN16_PRIORITY_DEFAULT,
      NVIC_DMA0_CHAN17_PRIORITY_DEFAULT,
      NVIC_DMA0_CHAN18_PRIORITY_DEFAULT,
      NVIC_DMA0_CHAN19_PRIORITY_DEFAULT,
      NVIC_DMA0_CHAN20_PRIORITY_DEFAULT,
      NVIC_DMA0_CHAN21_PRIORITY_DEFAULT,
      NVIC_DMA0_CHAN22_PRIORITY_DEFAULT,
      NVIC_DMA0_CHAN23_PRIORITY_DEFAULT,
      NVIC_DMA0_CHAN24_PRIORITY_DEFAULT,
      NVIC_DMA0_CHAN25_PRIORITY_DEFAULT,
      NVIC_DMA0_CHAN26_PRIORITY_DEFAULT,
      NVIC_DMA0_CHAN27_PRIORITY_DEFAULT,
      NVIC_DMA0_CHAN28_PRIORITY_DEFAULT,
      NVIC_DMA0_CHAN29_PRIORITY_DEFAULT,
      NVIC_DMA0_CHAN30_PRIORITY_DEFAULT,
      NVIC_DMA0_CHAN31_PRIORITY_DEFAULT
    },

  .error_isr_prio = NVIC_DMA0_ERROR_PRIORITY_DEFAULT,
#endif
#ifdef FC7300_DMA0_DBGS
  .cr_dbg = true,
#else
  .cr_dbg = false,
#endif

#ifdef FC7300_DMA0_ERCA
  .cr_rca = true,
#else
  .cr_rca = false,
#endif

#ifdef FC7300_DMA0_HOE
  .cr_hoe = true,
#else
  .cr_hoe = false,
#endif

#ifdef FC7300_DMA0_CTM
  .cr_ctm = true,
#else
  .cr_ctm = false,
#endif
};
#endif

#ifdef CONFIG_FC7300_DMA1
struct fc7300_dma_dev_s fc7300_dma1 =
{
  .dev =
    {
      .get_chan = fc7300_dma_get_chan,
      .put_chan = fc7300_dma_put_chan,
    },

  .base = FC7300_DMA1_BASE,
  .muxbase = FC7300_DMAMUX1_BASE,
#ifdef CONFIG_ARCH_IRQPRIO
  .dmachs_prio =
    {
      NVIC_DMA1_CHAN0_PRIORITY_DEFAULT,
      NVIC_DMA1_CHAN1_PRIORITY_DEFAULT,
      NVIC_DMA1_CHAN2_PRIORITY_DEFAULT,
      NVIC_DMA1_CHAN3_PRIORITY_DEFAULT,
      NVIC_DMA1_CHAN4_PRIORITY_DEFAULT,
      NVIC_DMA1_CHAN5_PRIORITY_DEFAULT,
      NVIC_DMA1_CHAN6_PRIORITY_DEFAULT,
      NVIC_DMA1_CHAN7_PRIORITY_DEFAULT,
      NVIC_DMA1_CHAN8_PRIORITY_DEFAULT,
      NVIC_DMA1_CHAN9_PRIORITY_DEFAULT,
      NVIC_DMA1_CHAN10_PRIORITY_DEFAULT,
      NVIC_DMA1_CHAN11_PRIORITY_DEFAULT,
      NVIC_DMA1_CHAN12_PRIORITY_DEFAULT,
      NVIC_DMA1_CHAN13_PRIORITY_DEFAULT,
      NVIC_DMA1_CHAN14_PRIORITY_DEFAULT,
      NVIC_DMA1_CHAN15_PRIORITY_DEFAULT,
      NVIC_DMA1_CHAN16_PRIORITY_DEFAULT,
      NVIC_DMA1_CHAN17_PRIORITY_DEFAULT,
      NVIC_DMA1_CHAN18_PRIORITY_DEFAULT,
      NVIC_DMA1_CHAN19_PRIORITY_DEFAULT,
      NVIC_DMA1_CHAN20_PRIORITY_DEFAULT,
      NVIC_DMA1_CHAN21_PRIORITY_DEFAULT,
      NVIC_DMA1_CHAN22_PRIORITY_DEFAULT,
      NVIC_DMA1_CHAN23_PRIORITY_DEFAULT,
      NVIC_DMA1_CHAN24_PRIORITY_DEFAULT,
      NVIC_DMA1_CHAN25_PRIORITY_DEFAULT,
      NVIC_DMA1_CHAN26_PRIORITY_DEFAULT,
      NVIC_DMA1_CHAN27_PRIORITY_DEFAULT,
      NVIC_DMA1_CHAN28_PRIORITY_DEFAULT,
      NVIC_DMA1_CHAN29_PRIORITY_DEFAULT,
      NVIC_DMA1_CHAN30_PRIORITY_DEFAULT,
      NVIC_DMA1_CHAN31_PRIORITY_DEFAULT
    },

  .error_isr_prio = NVIC_DMA1_ERROR_PRIORITY_DEFAULT,
#endif
#ifdef FC7300_DMA1_DBGS
  .cr_dbg = true,
#else
  .cr_dbg = false,
#endif

#ifdef FC7300_DMA1_ERCA
  .cr_rca = true,
#else
  .cr_rca = false,
#endif

#ifdef FC7300_DMA1_HOE
  .cr_hoe = true,
#else
  .cr_hoe = false,
#endif

#ifdef FC7300_DMA1_CTM
  .cr_ctm = true,
#else
  .cr_ctm = false,
#endif
};
#endif

/* The DMA instance descriptor */

#ifdef CONFIG_FC7300_DMA0
static struct dma_dev_s *g_dma0;
#endif

#ifdef CONFIG_FC7300_DMA1
static struct dma_dev_s *g_dma1;
#endif

#ifdef CONFIG_FC7300_DMA_SUPPORT_TCM
static uint32_t fc7300_dtcm_offset[3] =
{
  FC7300_CORE0_DTCM_BACKDOOR_OFFSET,
  FC7300_CORE1_DTCM_BACKDOOR_OFFSET,
  FC7300_CORE2_DTCM_BACKDOOR_OFFSET
};

static uint32_t fc7300_itcm_offset[3] =
{
  FC7300_CORE0_ITCM_BACKDOOR_OFFSET,
  FC7300_CORE1_ITCM_BACKDOOR_OFFSET,
  FC7300_CORE2_ITCM_BACKDOOR_OFFSET
};
#endif

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: fc7300_dma_getsize
 *
 * Description:
 *   Returns the enum data size of the numeric size
 *
 * Input Parameters:
 *   width - The numeric size
 *
 * Returned Value:
 *   The enum size according to the register definitions.
 *
 ****************************************************************************/

static inline enum fc7300_dma_sizes_e fc7300_dma_getsize(uint32_t width)
{
  enum fc7300_dma_sizes_e size = DMA_1BYTE;

  switch (width)
    {
      case 1:
        size = DMA_1BYTE;
        break;

      case 2:
        size = DMA_2BYTE;
        break;

      case 4:
        size = DMA_4BYTE;
        break;

      case 8:
        size = DMA_8BYTE;
        break;

      case 32:
        size = DMA_32BYTE;
        break;

      default:
        break;
    }

  return size;
}

#ifdef CONFIG_DMA_CYCBUF_TRANSMIT

/****************************************************************************
 * Name: fc7300_dma_ispowerof2
 *
 * Description:
 *   Returns whether the value is power of 2.
 *
 * Input Parameters:
 *   val - The value to check
 *   log2 - The ceiling log2 result of the value
 *
 * Returned Value:
 *   True when the value is power of 2, false when not.
 *
 ****************************************************************************/

static inline bool fc7300_dma_ispowerof2(uint32_t val, uint8_t *log2)
{
  bool ret = false;
  *log2 = 0U;

  while (val > (1UL << *log2))
    {
      (*log2)++;
    }

  if ((val & ((1UL << *log2) - 1U)) == 0U)
    {
      ret = true;
    }

  return ret;
}
#endif

/****************************************************************************
 * Name: fc7300_dma_addr_convert
 *
 * Description:
 *   Convert the address in DTCM/ITCM to backdoor address for DMA access.
 *
 * Input Parameters:
 *   addr - The real address
 *
 * Returned Value:
 *   Backdoor address if the address is in DTCM/ITCM region and if
 *   FC7300_DMA_SUPPORT_TCM is enabled.
 *
 ****************************************************************************/

static inline uint32_t fc7300_dma_addr_convert(uint32_t addr)
{
  uint32_t mapped_addr;

#ifdef CONFIG_FC7300_DMA_SUPPORT_TCM
  int coreid = fc7300_cpu_index();

  if ((addr >= FC7300_ITCM_START) && (addr <= FC7300_ITCM_END))
    {
      mapped_addr = addr + fc7300_itcm_offset[coreid];
    }
  else if ((addr >= FC7300_DTCM_START) && (addr <= FC7300_DTCM_END))
    {
      mapped_addr = addr + fc7300_dtcm_offset[coreid];
    }
  else
    {
      mapped_addr = addr;
    }
#else
  //coverity[NO_EFFECT]
  DEBUGASSERT(addr > FC7300_ITCM_END);
  DEBUGASSERT((addr < FC7300_DTCM_START) || (addr > FC7300_DTCM_END));
  mapped_addr    = addr;
#endif

  return mapped_addr;
}

/****************************************************************************
 * Name: fc7300_dma_is_busy
 *
 * Description:
 *   Returns whether the DMA channel is avtively transferring.
 *
 * Input Parameters:
 *   dmach - Private dma channel handle
 *
 * Returned Value:
 *   True when the DMA channel is active, false when the channel is idle.
 *
 ****************************************************************************/

static inline bool fc7300_dma_is_busy(struct fc7300_dma_chan_s *dmach)
{
  struct fc7300_dma_dev_s *priv = dmach->priv;
  uint16_t regval16;
  bool isbusy;

  regval16 = getreg16(priv->base + FC7300_DMA_CFG_OFFSET(dmach->chidx) +
                      FC7300_DMA_CFG_CSR_OFFSET);
  isbusy   = (regval16 & DMA_CFG_CSR_ACTIVE) ? true : false;
  return isbusy;
}

/****************************************************************************
 * Name: fc7300_dma_delivered
 *
 * Description:
 *   Returns the number of bytes have been transferred.
 *
 * Input Parameters:
 *   dmach - Private dma channel handle
 *
 * Returned Value:
 *   The number of bytes have been transferred.
 *
 ****************************************************************************/

static inline size_t fc7300_dma_delivered(struct fc7300_dma_chan_s *dmach)
{
  struct fc7300_dma_dev_s *priv = dmach->priv;
  uint16_t clc;
  uint16_t blc;
  uint16_t regval16;
  uint32_t regval32;
  uintptr_t regaddr;
  uint32_t nbytes;
  size_t delivered = 0;

  DEBUGASSERT(dmach != NULL);

  regaddr          = priv->base + FC7300_DMA_CFG_OFFSET(dmach->chidx) +
                     FC7300_DMA_CFG_NBYTES_IL_OFFSET;
  regval32         = getreg32(regaddr);

#ifdef CONFIG_DMA_LINK
  if ((regval32 & (DMA_CFG_NBYTES_ILOFF_DILOE |
                   DMA_CFG_NBYTES_ILOFF_SILOE)) == 0)
    {
      nbytes       = regval32 & DMA_CFG_NBYTES_IL_NBYTES_MASK;
    }
  else
    {
      nbytes       = regval32 & DMA_CFG_NBYTES_ILOFF_NBYTES_MASK;
    }
#else
    nbytes         = regval32;
#endif

  regaddr          = priv->base + FC7300_DMA_CFG_OFFSET(dmach->chidx) +
                     FC7300_DMA_CFG_CSR_OFFSET;
  regval16         = getreg16(regaddr);

  if ((regval16 & DMA_CFG_CSR_DONE) == 0)
    {
      /* Get the finished and unfinished loops */

      regaddr      = priv->base + FC7300_DMA_CFG_OFFSET(dmach->chidx) +
                     FC7300_DMA_CFG_BLC_CHTRG_OFFSET;
      regval16     = getreg16(regaddr);

      if ((regval16 & DMA_CFG_BLC_CHTRG_CHTRGEN) != 0)
        {
          blc      = (regval16 & DMA_CFG_BLC_CHTRG_BLC_MASK) >>
                     DMA_CFG_BLC_CHTRG_BLC_SHIFT;
          regaddr  = priv->base + FC7300_DMA_CFG_OFFSET(dmach->chidx) +
                     FC7300_DMA_CFG_CLC_CHTRG_OFFSET;
          regval16 = getreg16(regaddr);
          clc      = (regval16 & DMA_CFG_CLC_CHTRG_CLC_MASK) >>
                     DMA_CFG_CLC_CHTRG_CLC_SHIFT;
        }
      else
        {
          blc      = (regval16 & DMA_CFG_BLC_BLC_MASK) >>
                     DMA_CFG_BLC_BLC_SHIFT;
          regaddr  = priv->base + FC7300_DMA_CFG_OFFSET(dmach->chidx) +
                     FC7300_DMA_CFG_CLC_CHTRG_OFFSET;
          regval16 = getreg16(regaddr);
          clc      = (regval16 & DMA_CFG_CLC_CLC_MASK) >>
                     DMA_CFG_CLC_CLC_SHIFT;
        }
    }
  else
    {
      /* Get the finished and unfinished loops */

      regaddr      = priv->base + FC7300_DMA_CFG_OFFSET(dmach->chidx) +
                     FC7300_DMA_CFG_BLC_CHTRG_OFFSET;
      regval16     = getreg16(regaddr);

      if ((regval16 & DMA_CFG_BLC_CHTRG_CHTRGEN) != 0)
        {
          blc      = (regval16 & DMA_CFG_BLC_CHTRG_BLC_MASK) >>
                     DMA_CFG_BLC_CHTRG_BLC_SHIFT;
        }
      else
        {
          blc      = (regval16 & DMA_CFG_BLC_BLC_MASK) >>
                     DMA_CFG_BLC_BLC_SHIFT;
        }

      /* If the DMA is done, then the remaining count is zero */

      clc          = 0;
    }

  delivered        = nbytes * (blc - clc);

  return delivered;
}

/****************************************************************************
 * Name: fc7300_dmaterminate
 *
 * Description:
 *   Terminate the DMA transfer
 *
 * Input Parameters:
 *   dmach - Private dma channel handle
 *
 ****************************************************************************/

static void fc7300_dmaterminate(struct fc7300_dma_chan_s *dmach)
{
  struct fc7300_dma_dev_s *priv = dmach->priv;
  uintptr_t regaddr;
  uint8_t regval8;
  uint16_t regval16;
  uint8_t chidx;

  /* Disable channel ERROR interrupts */

  chidx      = dmach->chidx;
  regval8    = DMA_CEEI(chidx);
  putreg8(regval8, priv->base + FC7300_DMA_CEEI_OFFSET);

  /* Disable channel IRQ requests */

  regval8    = DMA_CERQ(chidx);
  putreg8(regval8, priv->base + FC7300_DMA_CERQ_OFFSET);

  /* Clear CSR flags */

  regaddr    = priv->base + FC7300_DMA_CFG_OFFSET(chidx) +
               FC7300_DMA_CFG_CSR_OFFSET;
  regval16   = getreg16(regaddr);
  regval16   = regval16 & ~(DMA_CFG_CSR_DONE | DMA_CFG_CSR_ACTIVE |
                            DMA_CFG_CSR_INTHALF);
  putreg16(regval16, regaddr);

  /* Restore BLC and CLC to 0 */

  regaddr    = priv->base + FC7300_DMA_CFG_OFFSET(chidx) +
               FC7300_DMA_CFG_CLC_CHTRG_OFFSET;
  putreg16(0, regaddr);
  regaddr    = priv->base + FC7300_DMA_CFG_OFFSET(chidx) +
               FC7300_DMA_CFG_BLC_CHTRG_OFFSET;
  putreg16(0, regaddr);

  dmach->state = FC7300_DMA_INITIALIZED;
}

/****************************************************************************
 * Name: fc7300_dma_isr_handle
 *
 * Description:
 *   Disable dman interrupt.
 *
 * Input Parameters:
 *
 * Returned Value: void
 *
 ****************************************************************************/

#ifdef CONFIG_FC7300_DMA_IRQ_ATTACH_THREAD
__ramfunc__ static int fc7300_dma_isr_handle(int irq, void *regs, void *arg)
{
  up_disable_irq(irq);
  return IRQ_WAKE_THREAD;
}
#endif

/****************************************************************************
 * Name: fc7300_dma_get_chan
 *
 * Description:
 *   Get a DMA channel. This function gives the caller mutually exclusive
 *   access to the DMA channel specified by the 'ident' argument.
 *
 *   If the DMA channel is not available, then fc7300_dma_get_chan will wait
 *   until the holder of the channel relinquishes the channel by calling
 *   fc7300_dma_put_chan().
 *
 *   WARNING: If you have two devices sharing a DMA channel and the code
 *   never releases the channel, the fc7300_dma_get_chan call for the other
 *   will hang forever in this function!
 *
 * Input Parameters:
 *   dev - Handle of the DMA instance
 *   ident - Identifies the channel resource
 *
 * Returned Value
 *   The handle of the DMA channel on success, a NULL handle on any failure
 *
 ****************************************************************************/

static struct dma_chan_s *fc7300_dma_get_chan(struct dma_dev_s *dev,
                                              unsigned int ident)
{
  struct fc7300_dma_dev_s *priv = (struct fc7300_dma_dev_s *)dev;
  struct fc7300_dma_chan_s *dmach;
  uint8_t regval8;

  DEBUGASSERT(ident < FC7300_DMA_NCHANNELS);

  nxmutex_lock(&priv->chlock[ident]);

  dmach = kmm_zalloc(sizeof(struct fc7300_dma_chan_s));
  if (!dmach)
    {
      return NULL;
    }

  dmach->priv         = priv;
  dmach->chidx        = ident;
  dmach->chan.ops     = &g_fc7300_dma_ops;
  dmach->state        = FC7300_DMA_UNINIT;

  priv->dmachs[ident] = dmach;

  /* Clear any pending interrupts on the channel */

  putreg16(0, priv->base + FC7300_DMA_CFG_OFFSET(ident) +
              FC7300_DMA_CFG_CSR_OFFSET);

  /* Make sure that the channel is disabled. */

  regval8 = DMA_CERQ(ident);
  putreg8(regval8, priv->base + FC7300_DMA_CERQ_OFFSET);

  /* Disable the associated DMAMUX for now */

  putreg8(0, priv->muxbase + FC7300_DMAMUX_CHCFG_OFFSET(ident));

  /* Attach the DMA channel interrupt vector */
#ifdef CONFIG_FC7300_DMA_IRQ_ATTACH_THREAD
      irq_attach_wqueue(FC7300_IRQ_DMACH(ident), fc7300_dma_isr_handle,
                        fc7300_dma_interrupt, dmach,
                        isr_cfg[DMA_ISR_THREAD]);
#else
      irq_attach(FC7300_IRQ_DMACH(ident), fc7300_dma_interrupt, dmach);
#endif

#ifdef CONFIG_ARCH_IRQPRIO
  up_prioritize_irq(FC7300_IRQ_DMACH(ident), priv->dmachs_prio[ident]);
#endif

  up_enable_irq(FC7300_IRQ_DMACH(ident));

  return &dmach->chan;
}

/****************************************************************************
 * Name: fc7300_dma_put_chan
 *
 * Description:
 *   Release a DMA channel. If another thread is waiting for this DMA channel
 *   in a call to fc7300_dma_get_chan, then this function will re-assign the
 *   DMA channel to that thread and wake it up.
 *
 *   NOTE: The 'chan' used in this argument must NEVER be used again until
 *   fc7300_dma_get_chan() is called again to re-gain access to the channel.
 *
 * Input Parameters:
 *   dev - Handle of the DMA instance
 *   chan - Handle of the DMA channel
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/

static void fc7300_dma_put_chan(struct dma_dev_s *dev,
                                struct dma_chan_s *chan)
{
  struct fc7300_dma_dev_s *priv = (struct fc7300_dma_dev_s *)dev;
  struct fc7300_dma_chan_s *dmach = (struct fc7300_dma_chan_s *)chan;
  int i;

#ifdef CONFIG_DMA_CYCBUF_TRANSMIT
  uint32_t regval32;
  uint16_t regval16;
  uint8_t ums;
#endif

  if (!dmach)
    {
      return;
    }

  for (i = 0; i < FC7300_DMA_NCHANNELS; i++)
    {
      if (priv->dmachs[i] == dmach)
        {
          /* Make sure that the dmach is disabled. */

          fc7300_dmaterminate(dmach);

          /* Disable the associated DMAMUX */

          putreg8(0, priv->muxbase +
                     FC7300_DMAMUX_CHCFG_OFFSET(dmach->chidx));

          /* Disable the IRQ and restore the IRQ handler */

          up_disable_irq(FC7300_IRQ_DMACH(dmach->chidx));

#ifdef CONFIG_FC7300_DMA_IRQ_ATTACH_THREAD
          /* Detach the DMA interrupt */

          irq_detach_wqueue(FC7300_IRQ_DMACH(dmach->chidx));
#else
          irq_attach(FC7300_IRQ_DMACH(dmach->chidx), NULL, NULL);
#endif

#ifdef CONFIG_DMA_CYCBUF_TRANSMIT
          if (dmach->option & DMA_CONFIG_CYCBUF_SRC_UMO ||
              dmach->option & DMA_CONFIG_CYCBUF_DST_UMO)
            {
              regval16 = getreg16(priv->base +
                                  FC7300_DMA_CFG_OFFSET(dmach->chidx) +
                                  FC7300_DMA_CFG_CSR_OFFSET);
              ums = regval16 & DMA_CFG_CSR_UMS_MASK >> DMA_CFG_CSR_UMS_SHIFT;

              priv->dumo_refs[ums]--;

              if (priv->dumo_refs[ums] == 0)
                {
                  putreg32(0, priv->base +
                              FC7300_DMA_DUMO_OFFSET(ums));
                }
            }

          regval32 = getreg32(priv->base + FC7300_DMA_DUME_OFFSET);
          regval32 = regval32 & ~(DMA_DUME_DUME(dmach->chidx) |
                                  DMA_DUME_SUME(dmach->chidx));
          putreg32(regval32, priv->base + FC7300_DMA_DUME_OFFSET);
#endif

          putreg16(0, priv->base + FC7300_DMA_CFG_OFFSET(dmach->chidx) +
                      FC7300_DMA_CFG_CSR_OFFSET);

          nxmutex_unlock(&priv->chlock[dmach->chidx]);
          kmm_free(dmach);
          priv->dmachs[i] = NULL;
          break;
        }
    }
}

/****************************************************************************
 * Name: fc7300_dma_chan_config
 *
 * Description:
 *   This function adds the DMA transfer to the DMA sequence.  The request
 *   is setup according to the content of the transfer configuration
 *   structure. If fc7300_dma_chan_config is called multiple times, the
 *   latter configuration will overwrite the previous ones.
 *
 * Input Parameters:
 *   chan - DMA channel handle created by fc7300_dma_get_chan()
 *   cfg - A DMA transfer configuration instance, populated by the
 *         The content of 'config' describes the transfer
 *
 * Returned Value
 *   Zero (OK) is returned on success; a negated errno value is returned on
 *   any failure.
 *
 ****************************************************************************/

static int fc7300_dma_chan_config(struct dma_chan_s *chan,
                                  const struct dma_config_s *cfg)
{
  struct fc7300_dma_chan_s *dmach = (struct fc7300_dma_chan_s *)chan;
  struct fc7300_dma_dev_s *priv = dmach->priv;
#ifdef CONFIG_DMA_CYCBUF_TRANSMIT
  uint32_t regval32;
  uint16_t regval16;
  uint8_t ums;
#endif

  DEBUGASSERT(dmach != NULL);
  dmainfo("dmach%u: %p config: %p\n", dmach->chidx, dmach, cfg);

#ifdef CONFIG_DMA_CYCBUF_TRANSMIT
  if (dmach->option & DMA_CONFIG_CYCBUF_SRC_UMO ||
      dmach->option & DMA_CONFIG_CYCBUF_DST_UMO)
    {
      regval16 = getreg16(priv->base + FC7300_DMA_CFG_OFFSET(dmach->chidx) +
                          FC7300_DMA_CFG_CSR_OFFSET);
      ums = regval16 & DMA_CFG_CSR_UMS_MASK >> DMA_CFG_CSR_UMS_SHIFT;

      priv->dumo_refs[ums]--;

      if (priv->dumo_refs[ums] == 0)
        {
          putreg32(0, priv->base + FC7300_DMA_DUMO_OFFSET(ums));
        }
    }

  regval32 = getreg32(priv->base + FC7300_DMA_DUME_OFFSET);
  regval32 = regval32 & ~(DMA_DUME_DUME(dmach->chidx) |
                          DMA_DUME_SUME(dmach->chidx));
  putreg32(regval32, priv->base + FC7300_DMA_DUME_OFFSET);
#endif

  dmach->dmamux = cfg->src_drq;
  dmach->option = cfg->option;

  if ((dmach->state == FC7300_DMA_ACTIVE) ||
      (dmach->state == FC7300_DMA_PAUSED))
    {
      return -EBUSY;
    }

  /* Configure channel CFG registers to the values specified in config. */

  struct fc7300_dmacfg_s *tcd = (struct fc7300_dmacfg_s *)
                          (priv->base + FC7300_DMA_CFG_OFFSET(dmach->chidx));

  tcd->soff     = cfg->src_step;
  tcd->attr     = DMA_CFG_ATTR_SSIZE(fc7300_dma_getsize(cfg->src_width)) |
                  DMA_CFG_ATTR_DSIZE(fc7300_dma_getsize(cfg->dst_width));
  tcd->doff     = cfg->dst_step;

  tcd->csr      = cfg->option & DMA_CONFIG_AUTOSTOP ? DMA_CFG_CSR_DREQ : 0;

  /* Enable the DONE interrupt when the outer iteration count completes. */

  if (cfg->option & DMA_CONFIG_INTOUTER)
    {
      tcd->csr  |= DMA_CFG_CSR_INTOUTER;
    }

  /* Set the DMAMUX source */

  if (cfg->src_drq != FC7300_DMA_REQ_DISABLED)
    {
      putreg8(cfg->src_drq | DMAMUX_CHCFG_ENBL,
              priv->muxbase + FC7300_DMAMUX_CHCFG_OFFSET(dmach->chidx));
    }
  else
    {
      putreg8(0,
              priv->muxbase + FC7300_DMAMUX_CHCFG_OFFSET(dmach->chidx));
    }

  dmach->state = FC7300_DMA_INITIALIZED;

  return OK;
}

/****************************************************************************
 * Name: fc7300_dma_start
 *
 * Description:
 *   Start the DMA transfer. This function should be called after the final
 *   call to fc7300_dma_chan_config. After the transfer is finished, a
 *   callback to the user-provided function is made.
 *
 *   NOTE: The DMA module does *NOT* perform any cache operations. It is the
 *   responsibility of the DMA client to clean DMA buffers after staring of
 *   the DMA TX operations.
 *
 * Input Parameters:
 *   chan     - The channel to start
 *   callback - The callback when the transfer finish
 *   arg      - The argument will pass to callback
 *   dst      - The destination address
 *   src      - The source address
 *   len      - The length to transfer
 *
 * Returned Value:
 *   Zero (OK) is returned on success; a negated errno value is returned on
 *   any failure.
 *
 ****************************************************************************/

static int fc7300_dma_start(struct dma_chan_s *chan,
                            dma_callback_t callback, void *arg,
                            uintptr_t dst, uintptr_t src, size_t len)
{
  struct fc7300_dma_chan_s *dmach = (struct fc7300_dma_chan_s *)chan;
  struct fc7300_dma_dev_s *priv = dmach->priv;
  irqstate_t flags;
  uint16_t src_width;
  uint16_t dst_width;
  uint16_t regval16;
  uint8_t regval8;
  uint8_t chidx;

  DEBUGASSERT(dmach != NULL && dmach->state == FC7300_DMA_INITIALIZED);
  chidx         = dmach->chidx;
  dmainfo("dmach%u: %p callback: %p arg: %p\n", chidx, dmach, callback, arg);

  regval16      = getreg16(priv->base + FC7300_DMA_CFG_OFFSET(dmach->chidx) +
                           FC7300_DMA_CFG_ATTR_OFFSET);
  src_width     = 1U << ((regval16 & DMA_CFG_ATTR_SSIZE_MASK) >>
                         DMA_CFG_ATTR_SSIZE_SHIFT);
  dst_width     = 1U << ((regval16 & DMA_CFG_ATTR_DSIZE_MASK) >>
                         DMA_CFG_ATTR_DSIZE_SHIFT);

  DEBUGASSERT((len % src_width == 0) && (len % dst_width == 0));

  /* Save the callback info. This will be invoked when the DMA completes. */

  flags           = spin_lock_irqsave(NULL);
  dmach->callback = callback;
  dmach->arg      = arg;
  dmach->state    = FC7300_DMA_ACTIVE;

  /* Configure channel CFG registers */

  struct fc7300_dmacfg_s *tcd = (struct fc7300_dmacfg_s *)
                          (priv->base + FC7300_DMA_CFG_OFFSET(dmach->chidx));

  tcd->saddr    = fc7300_dma_addr_convert(src);
  tcd->daddr    = fc7300_dma_addr_convert(dst);
  tcd->slast    = dmach->option & DMA_CONFIG_LOOPSRC ?
                  -(len / src_width * (int16_t)tcd->soff) : 0;
  tcd->dlast    = dmach->option & DMA_CONFIG_LOOPDEST ?
                  -(len / dst_width * (int16_t)tcd->doff) : 0;
  tcd->nbytes   = len;
  tcd->clc      = 1;
  tcd->blc      = 1;

  /* Enable channel ERROR interrupts */

  regval8       = DMA_SEEI(chidx);
  putreg8(regval8, priv->base + FC7300_DMA_SEEI_OFFSET);

  UP_DSB();

  if (dmach->dmamux == FC7300_DMA_REQ_DISABLED)
    {
      /* For software trigger, start the channel */

      regval8   = DMA_SSRT(chidx);
      putreg8(regval8, priv->base + FC7300_DMA_SSRT_OFFSET);
    }
  else
    {
      /* For hardware trigger, enable the DMA request for this channel */

      regval8   = DMA_SERQ(chidx);
      putreg8(regval8, priv->base + FC7300_DMA_SERQ_OFFSET);
    }

  spin_unlock_irqrestore(NULL, flags);

  return OK;
}

/****************************************************************************
 * Name: fc7300_dma_start
 *
 * Description:
 *   Start the cyclic DMA transfer. This function should be called after the
 *   final call to fc7300_dma_chan_config. After the transfer is finished, a
 *   callback to the user-provided function is made. If DMA_CONFIG_INTHALF
 *   flag is set  when calling fc7300_dma_chan_config, callback will alse get
 *   called when the DMA transfer is half finished.
 *
 *   NOTE: The DMA module does *NOT* perform any cache operations. It is the
 *   responsibility of the DMA client to clean DMA buffers after staring of
 *   the DMA TX operations.
 *
 * Input Parameters:
 *   chan       - The channel to start
 *   callback   - The callback when the transfer finish
 *   arg        - The argument will pass to callback
 *   dst        - The destination address
 *   src        - The source address
 *   len        - The total length to transfer
 *   period_len - The lenth to transfer in each inner loop
 *
 * Returned Value:
 *   Zero (OK) is returned on success; a negated errno value is returned on
 *   any failure.
 *
 ****************************************************************************/

static int fc7300_dma_start_cyclic(struct dma_chan_s *chan,
                                   dma_callback_t callback, void *arg,
                                   uintptr_t dst, uintptr_t src,
                                   size_t len, size_t period_len)
{
  struct fc7300_dma_chan_s *dmach = (struct fc7300_dma_chan_s *)chan;
  struct fc7300_dma_dev_s *priv = dmach->priv;
  irqstate_t flags;
  uint16_t src_width;
  uint16_t dst_width;
  uint32_t iter;
  uint16_t regval16;
  uint8_t regval8;
  uint8_t chidx;

  DEBUGASSERT((dmach != NULL) && (dmach->state == FC7300_DMA_INITIALIZED));
  DEBUGASSERT((len >= period_len) && (len % period_len == 0));
  chidx            = dmach->chidx;
  dmainfo("dmach%u: %p callback: %p arg: %p\n", chidx, dmach, callback, arg);

  regval16      = getreg16(priv->base + FC7300_DMA_CFG_OFFSET(chidx) +
                            FC7300_DMA_CFG_ATTR_OFFSET);
  src_width     = 1U << ((regval16 & DMA_CFG_ATTR_SSIZE_MASK) >>
                         DMA_CFG_ATTR_SSIZE_SHIFT);
  dst_width     = 1U << ((regval16 & DMA_CFG_ATTR_DSIZE_MASK) >>
                         DMA_CFG_ATTR_DSIZE_SHIFT);

  DEBUGASSERT((period_len % src_width == 0) &&
              (period_len % dst_width == 0));

  /* Save the callback info. This will be invoked when the DMA completes */

  flags           = spin_lock_irqsave(NULL);
  dmach->callback = callback;
  dmach->arg      = arg;
  dmach->state    = FC7300_DMA_ACTIVE;

  iter = len / period_len;

  /* Configure channel CFG registers */

  struct fc7300_dmacfg_s *tcd = (struct fc7300_dmacfg_s *)
                                (priv->base + FC7300_DMA_CFG_OFFSET(chidx));

  tcd->saddr    = fc7300_dma_addr_convert(src);
  tcd->daddr    = fc7300_dma_addr_convert(dst);
  tcd->slast    = dmach->option & DMA_CONFIG_LOOPSRC ?
                  -(len / src_width * (int16_t)tcd->soff) : 0;
  tcd->dlast    = dmach->option & DMA_CONFIG_LOOPDEST ?
                  -(len / dst_width * (int16_t)tcd->doff) : 0;
  tcd->nbytes   = period_len;

  if (dmach->dmamux == FC7300_DMA_REQ_DISABLED && iter > 1)
    {
      /* If it is software trigger, make the channel trig itself when inner
       * loop finishes.
       */

      tcd->clc = DMA_CFG_CLC_CHTRG_CHTRGEN | DMA_CFG_CLC_CHTRG_TRGCH(chidx) |
                 (iter & DMA_CFG_CLC_CHTRG_CLC_MASK);
      tcd->blc = DMA_CFG_BLC_CHTRG_CHTRGEN | DMA_CFG_BLC_CHTRG_TRGCH(chidx) |
                 (iter & DMA_CFG_BLC_CHTRG_BLC_MASK);
    }
  else
    {
      tcd->clc = iter & DMA_CFG_CLC_CLC_MASK;
      tcd->blc = iter & DMA_CFG_CLC_CLC_MASK;
    }

  if (iter > 1)
    {
      /* The half-point interrupt can only be enabled when iter > 1 */

      tcd->csr |= dmach->option & DMA_CONFIG_INTHALF ? DMA_CFG_CSR_INTHALF :
                                                      0;
    }

  /* Enable channel ERROR interrupts */

  regval8       = DMA_SEEI(chidx);
  putreg8(regval8, priv->base + FC7300_DMA_SEEI_OFFSET);

  UP_DSB();

  if (dmach->dmamux == FC7300_DMA_REQ_DISABLED)
    {
      /* For software trigger, start the channel */

      regval8   = DMA_SSRT(chidx);
      putreg8(regval8, priv->base + FC7300_DMA_SSRT_OFFSET);
    }
  else
    {
      /* For hardware trigger, enable the DMA request for this channel */

      regval8   = DMA_SERQ(chidx);
      putreg8(regval8, priv->base + FC7300_DMA_SERQ_OFFSET);
    }

  spin_unlock_irqrestore(NULL, flags);

  return OK;
}

#ifdef CONFIG_DMA_LINK

/****************************************************************************
 * Name: fc7300_dma_start_link
 *
 * Description:
 *   Start the DMA link transfer. This function should be called after the
 *   final call to fc7300_dma_chan_config. After the transfer is finished, a
 *   callback to the user-provided function is made. If DMA_CONFIG_INTHALF
 *   flag is set  when calling fc7300_dma_chan_config, callback will alse get
 *   called when the DMA transfer is half finished.
 *
 *   NOTE: The DMA module does *NOT* perform any cache operations. It is the
 *   responsibility of the DMA client to clean DMA buffers after staring of
 *   the DMA TX operations.
 *
 *   NOTE: When using this function,The DMA module only allows the scattered
 *   memory to be of the same size and same distance.
 *
 * Input Parameters:
 *   chan       - The channel to start
 *   callback   - The callback when the transfer finish
 *   arg        - The argument will pass to callback
 *   work_mode  - The work mode of the linked transfer
 *   cfg        - The linked transfer descriptor
 *
 * Returned Value:
 *   Zero (OK) is returned on success; a negated errno value is returned on
 *   any failure.
 *
 ****************************************************************************/

static int fc7300_dma_start_link(struct dma_chan_s *chan,
                                 dma_callback_t callback, void *arg,
                                 unsigned int work_mode,
                                 struct dma_link_config_s *cfg)
{
  struct fc7300_dma_chan_s *dmach = (struct fc7300_dma_chan_s *)chan;
  struct fc7300_dma_dev_s *priv = dmach->priv;
  uint8_t chidx = dmach->chidx;
  uint32_t src = 0;
  uint32_t dst = 0;
  uint32_t nbytes_il = 0;
  int32_t slast = 0;
  int32_t dlast = 0;
  irqstate_t flags;
  uint16_t src_width;
  uint16_t dst_width;
  uint32_t iter;
  int32_t iloff;
  uint32_t nbytes;
  uint16_t regval16;
  uint8_t regval8;

  struct fc7300_dmacfg_s *tcd = (struct fc7300_dmacfg_s *)
                                (priv->base + FC7300_DMA_CFG_OFFSET(chidx));

  DEBUGASSERT((dmach != NULL) && (dmach->state == FC7300_DMA_INITIALIZED));
  DEBUGASSERT(cfg != NULL);
  DEBUGASSERT((cfg->src_link != NULL) && (cfg->dst_link != NULL));

  dmainfo("dmach%u: %p callback: %p arg: %p\n", chidx, dmach, callback, arg);

  regval16      = getreg16(priv->base + FC7300_DMA_CFG_OFFSET(chidx) +
                            FC7300_DMA_CFG_ATTR_OFFSET);
  src_width     = 1U << ((regval16 & DMA_CFG_ATTR_SSIZE_MASK) >>
                         DMA_CFG_ATTR_SSIZE_SHIFT);
  dst_width     = 1U << ((regval16 & DMA_CFG_ATTR_DSIZE_MASK) >>
                         DMA_CFG_ATTR_DSIZE_SHIFT);
  DEBUGASSERT((cfg->src_link[0].link_size % src_width == 0) &&
              (cfg->dst_link[0].link_size % dst_width == 0));

  switch (work_mode)
    {
      case DMA_BLOCK_MODE:
        {
          if (cfg->src_link_num != 1 || cfg->dst_link_num != 1)
            {
              return -EINVAL;
            }

          if (cfg->src_link[0].link_size != cfg->dst_link[0].link_size)
            {
              return -EINVAL;
            }

          src = cfg->src_link[0].addr;
          dst = cfg->dst_link[0].addr;
          nbytes = cfg->src_link[0].link_size;
          iter = 1;
          slast = dmach->option & DMA_CONFIG_LOOPSRC ?
                  -(nbytes / src_width * (int16_t)tcd->soff) : 0;
          dlast = dmach->option & DMA_CONFIG_LOOPDEST ?
                  -(nbytes / dst_width * (int16_t)tcd->doff) : 0;
          nbytes_il = nbytes;
        }
        break;

      case DMA_SRC_LINK_MODE:
        {
          if (cfg->src_link_num <= 1 || cfg->dst_link_num != 1)
            {
              return -EINVAL;
            }

          if (cfg->src_link[0].link_size * cfg->src_link_num !=
              cfg->dst_link[0].link_size)
            {
              return -EINVAL;
            }

          src = cfg->src_link[0].addr;
          dst = cfg->dst_link[0].addr;
          nbytes = cfg->src_link[0].link_size;

          if (dmach->option & DMA_CONFIG_CYCBUF_SRC)
            {
              iloff = cfg->src_link[1].addr - cfg->src_link[0].addr;
            }
          else
            {
              iloff = cfg->src_link[1].addr - (cfg->src_link[0].addr +
                      nbytes / src_width * (int16_t)tcd->soff);
            }

          iter = cfg->src_link_num;
          slast = dmach->option & DMA_CONFIG_LOOPSRC ?
                  -(nbytes * iter / src_width * (int16_t)tcd->soff -
                    iloff * (iter - 1)) : 0;
          dlast = dmach->option & DMA_CONFIG_LOOPDEST ?
                  -(nbytes * iter / dst_width * (int16_t)tcd->doff) : 0;
          nbytes_il = DMA_CFG_NBYTES_IL_SILOE |
                      DMA_CFG_NBYTES_ILOFF_ILOFF(iloff) |
                      DMA_CFG_NBYTES_ILOFF_NBYTES(nbytes);
        }
        break;

      case DMA_DST_LINK_MODE:
        {
          if (cfg->src_link_num != 1 || cfg->dst_link_num <= 1)
            {
              return -EINVAL;
            }

          if (cfg->src_link[0].link_size !=
              cfg->dst_link[0].link_size * cfg->dst_link_num)
            {
              return -EINVAL;
            }

          src = cfg->src_link[0].addr;
          dst = cfg->dst_link[0].addr;
          nbytes = cfg->dst_link[0].link_size;

          if (dmach->option & DMA_CONFIG_CYCBUF_DST)
            {
              iloff = cfg->dst_link[1].addr - cfg->dst_link[0].addr;
            }
          else
            {
              iloff = cfg->dst_link[1].addr - (cfg->dst_link[0].addr +
                      nbytes / dst_width * (int16_t)tcd->doff);
            }

          iter = cfg->dst_link_num;
          slast = dmach->option & DMA_CONFIG_LOOPSRC ?
                  -(nbytes * iter / src_width * (int16_t)tcd->soff) : 0;
          dlast = dmach->option & DMA_CONFIG_LOOPSRC ?
                  -(nbytes * iter / dst_width * (int16_t)tcd->doff -
                    iloff * (iter - 1)) : 0;
          nbytes_il = DMA_CFG_NBYTES_IL_DILOE |
                      DMA_CFG_NBYTES_ILOFF_ILOFF(iloff) |
                      DMA_CFG_NBYTES_ILOFF_NBYTES(nbytes);
        }
        break;

      case DMA_DUAL_LINK_MODE:
        {
          if (cfg->src_link_num <= 1 || cfg->dst_link_num <= 1 ||
              cfg->src_link_num != cfg->dst_link_num)
            {
              return -EINVAL;
            }

          if (cfg->src_link[0].link_size != cfg->dst_link[0].link_size)
            {
              return -EINVAL;
            }

          if (cfg->src_link[1].addr - cfg->src_link[0].addr !=
              cfg->dst_link[1].addr - cfg->dst_link[0].addr)
            {
              return -EINVAL;
            }

          src = cfg->src_link[0].addr;
          dst = cfg->dst_link[0].addr;
          nbytes = cfg->src_link[0].link_size;

          if ((dmach->option & (DMA_CONFIG_CYCBUF_SRC |
                               DMA_CONFIG_CYCBUF_DST)) ==
              (DMA_CONFIG_CYCBUF_SRC | DMA_CONFIG_CYCBUF_DST))
            {
              iloff = cfg->src_link[1].addr - cfg->src_link[0].addr;
            }
          else if ((dmach->option & (DMA_CONFIG_CYCBUF_SRC |
                                    DMA_CONFIG_CYCBUF_DST)) == 0)
            {
              iloff = cfg->src_link[1].addr - (cfg->src_link[0].addr +
                      nbytes / src_width * (int16_t)tcd->soff);
            }
          else
            {
              return -EINVAL;
            }

          iter = cfg->src_link_num;
          slast = dmach->option & DMA_CONFIG_LOOPSRC ?
                  -(nbytes *iter / src_width * (int16_t)tcd->soff -
                    iloff * (iter - 1)) : 0;
          dlast = dmach->option & DMA_CONFIG_LOOPSRC ?
                  -(nbytes *iter / dst_width * (int16_t)tcd->doff -
                    iloff * (iter - 1)) : 0;
          nbytes_il = DMA_CFG_NBYTES_IL_SILOE |
                      DMA_CFG_NBYTES_IL_DILOE |
                      DMA_CFG_NBYTES_ILOFF_ILOFF(iloff) |
                      DMA_CFG_NBYTES_ILOFF_NBYTES(nbytes);
        }
        break;

      default:
        {
          return -EINVAL;
        }
        break;
    }

  /* Save the callback info. This will be invoked when the DMA completes */

  flags           = spin_lock_irqsave(NULL);
  dmach->callback = callback;
  dmach->arg      = arg;
  dmach->state    = FC7300_DMA_ACTIVE;

  tcd->saddr    = fc7300_dma_addr_convert(src);
  tcd->daddr    = fc7300_dma_addr_convert(dst);
  tcd->slast    = slast;
  tcd->dlast    = dlast;
  tcd->nbytes   = nbytes_il;

  if (dmach->dmamux == FC7300_DMA_REQ_DISABLED && iter > 1)
    {
      /* If it is software trigger, make the channel trig itself when inner
       * loop finishes.
       */

      tcd->clc = DMA_CFG_CLC_CHTRG_CHTRGEN | DMA_CFG_CLC_CHTRG_TRGCH(chidx) |
                 (iter & DMA_CFG_CLC_CHTRG_CLC_MASK);
      tcd->blc = DMA_CFG_BLC_CHTRG_CHTRGEN | DMA_CFG_BLC_CHTRG_TRGCH(chidx) |
                 (iter & DMA_CFG_BLC_CHTRG_BLC_MASK);
    }
  else
    {
      tcd->clc = iter & DMA_CFG_CLC_CLC_MASK;
      tcd->blc = iter & DMA_CFG_CLC_CLC_MASK;
    }

  if (iter > 1)
    {
      /* The half-point interrupt can only be enabled when iter > 1 */

      tcd->csr |= dmach->option & DMA_CONFIG_INTHALF ? DMA_CFG_CSR_INTHALF :
                                                      0;
    }

  /* Enable channel ERROR interrupts */

  regval8       = DMA_SEEI(chidx);
  putreg8(regval8, priv->base + FC7300_DMA_SEEI_OFFSET);

  UP_DSB();

  if (dmach->dmamux == FC7300_DMA_REQ_DISABLED)
    {
      /* For software trigger, start the channel */

      regval8   = DMA_SSRT(chidx);
      putreg8(regval8, priv->base + FC7300_DMA_SSRT_OFFSET);
    }
  else
    {
      /* For hardware trigger, enable the DMA request for this channel */

      regval8   = DMA_SERQ(chidx);
      putreg8(regval8, priv->base + FC7300_DMA_SERQ_OFFSET);
    }

  spin_unlock_irqrestore(NULL, flags);

  return OK;
}
#endif

/****************************************************************************
 * Name: fc7300_dma_stop
 *
 * Description:
 *   Stop the DMA transfer.
 *
 * Input Parameters:
 *   chan   - DMA channel handle created by fc7300_dma_get_chan()
 *
 * Returned Value:
 *   Zero (OK) is returned on success; a negated errno value is returned on
 *   any failure.
 *
 ****************************************************************************/

static int fc7300_dma_stop(struct dma_chan_s *chan)
{
  struct fc7300_dma_chan_s *dmach = (struct fc7300_dma_chan_s *)chan;
  irqstate_t flags;

  dmainfo("dmach: %p\n", dmach);
  DEBUGASSERT(dmach != NULL);

  flags = spin_lock_irqsave(NULL);

  fc7300_dmaterminate(dmach);

  dmach->callback = NULL;
  dmach->arg      = NULL;

  spin_unlock_irqrestore(NULL, flags);

  return OK;
}

/****************************************************************************
 * Name: fc7300_dma_pause
 *
 * Description:
 *   Pause the DMA transfer. After fc7300_dma_pause() is called, you can call
 *   fc7300_dma_resume() to restart the transfer again. Or you could call
 *   fc7300_dma_stop() to stop the transfer.
 *
 * Input Parameters:
 *   chan   - DMA channel handle created by fc7300_dma_get_chan()
 *
 * Returned Value:
 *   Zero (OK) is returned on success; a negated errno value is returned on
 *   any failure.
 *
 ****************************************************************************/

static int fc7300_dma_pause(struct dma_chan_s *chan)
{
  struct fc7300_dma_chan_s *dmach = (struct fc7300_dma_chan_s *)chan;
  struct fc7300_dma_dev_s *priv = dmach->priv;
  uint32_t regaddr;
  uint32_t regval32;
  uint16_t regval16;
  uint8_t regval8;
  uint32_t timeout = 0xffff;

  DEBUGASSERT(dmach != NULL);

  if (dmach->state != FC7300_DMA_ACTIVE)
    {
      return -EPERM;
    }

  regval32 = getreg32(priv->base + FC7300_DMA_CR_OFFSET);
  regval32 |= DMA_CR_HALT;
  putreg32(regval32, priv->base + FC7300_DMA_CR_OFFSET);

  if (dmach->dmamux == FC7300_DMA_REQ_DISABLED)
    {
      /* For software trigger, pause means disable inner loop trigger */

      regaddr = priv->base + FC7300_DMA_CFG_OFFSET(dmach->chidx) +
                FC7300_DMA_CFG_CLC_CHTRG_OFFSET;
      regval16 = getreg16(regaddr);
      if (regval16 & DMA_CFG_CLC_CHTRG_CHTRGEN)
        {
          regval16 = regval16 & ~(DMA_CFG_CLC_CHTRG_CHTRGEN |
                                  DMA_CFG_CLC_CHTRG_TRGCH_MASK);
          putreg16(regval16, regaddr);
          regaddr = priv->base + FC7300_DMA_CFG_OFFSET(dmach->chidx) +
                    FC7300_DMA_CFG_BLC_CHTRG_OFFSET;
          regval16 = getreg16(regaddr);
          regval16 = regval16 & ~(DMA_CFG_BLC_CHTRG_CHTRGEN |
                                  DMA_CFG_BLC_CHTRG_TRGCH_MASK);
          putreg16(regval16, regaddr);
        }
    }

  /* If the inner loop is ongoing, waiting the inner loop to finish */

  while (fc7300_dma_is_busy(dmach) && timeout != 0)
    {
      timeout--;
    }

  /* If timeout occurs, abort the current transfer. This could occur when
   * nbytes is very large and will result in data inconsistant
   */

  if (timeout == 0)
    {
      regval32 = getreg32(priv->base + FC7300_DMA_CR_OFFSET);
      regval32 |= DMA_CR_CX;
      putreg32(regval32, priv->base + FC7300_DMA_CR_OFFSET);
    }

  if (dmach->dmamux != FC7300_DMA_REQ_DISABLED)
    {
      /* For hardware trigger, pasuse means disable hardware trigger */

      regval8 = DMA_CERQ(dmach->chidx);
      putreg8(regval8, priv->base + FC7300_DMA_CERQ_OFFSET);
    }

  regval32 = getreg32(priv->base + FC7300_DMA_CR_OFFSET);
  regval32 &= ~(DMA_CR_HALT | DMA_CR_CX);
  putreg32(regval32, priv->base + FC7300_DMA_CR_OFFSET);

  dmach->state = FC7300_DMA_PAUSED;

  return OK;
}

/****************************************************************************
 * Name: fc7300_dma_resume
 *
 * Description:
 *   Resume the DMA transfer.
 *
 * Input Parameters:
 *   chan   - DMA channel handle created by fc7300_dma_get_chan()
 *
 * Returned Value:
 *   Zero (OK) is returned on success; a negated errno value is returned on
 *   any failure.
 *
 ****************************************************************************/

static int fc7300_dma_resume(struct dma_chan_s *chan)
{
  struct fc7300_dma_chan_s *dmach = (struct fc7300_dma_chan_s *)chan;
  struct fc7300_dma_dev_s *priv = dmach->priv;
  uint32_t regaddr;
  uint8_t regval8;
  uint16_t regval16;

  DEBUGASSERT(dmach != NULL);

  if (dmach->state != FC7300_DMA_PAUSED)
    {
      return -EPERM;
    }

  if (dmach->dmamux == FC7300_DMA_REQ_DISABLED)
    {
      regaddr = priv->base + FC7300_DMA_CFG_OFFSET(dmach->chidx) +
                FC7300_DMA_CFG_CLC_CHTRG_OFFSET;
      regval16 = getreg16(regaddr);
      regval16 |= DMA_CFG_CLC_CHTRG_CHTRGEN |
                  DMA_CFG_CLC_CHTRG_TRGCH(dmach->chidx);
      putreg16(regval16, regaddr);
      regaddr = priv->base + FC7300_DMA_CFG_OFFSET(dmach->chidx) +
                FC7300_DMA_CFG_BLC_CHTRG_OFFSET;
      regval16 = getreg16(regaddr);
      regval16 |= DMA_CFG_BLC_CHTRG_CHTRGEN |
                  DMA_CFG_BLC_CHTRG_TRGCH(dmach->chidx);
      putreg16(regval16, regaddr);
      regval8 = DMA_SSRT(dmach->chidx);
      putreg8(regval8, priv->base + FC7300_DMA_SSRT_OFFSET);
    }
  else
    {
      regval8 = DMA_SERQ(dmach->chidx);
      putreg8(regval8, priv->base + FC7300_DMA_SERQ_OFFSET);
    }

  dmach->state = FC7300_DMA_ACTIVE;

  return OK;
}

/****************************************************************************
 * Name: fc7300_dma_residual
 *
 * Description:
 *   Returns the number of bytes remaining to be transferred.
 *
 *   NOTES:
 *   1. This function can only be used to get unfinished bytes when the
 *      channel is started with fc7300_dma_start_cyclic. And the returned
 *      value is the count of unfinished cycles multiplies the period length.
 *   2. The unfinished/remaining transfer bytes cannot be obtained directly
 *      from registers while the channel is running.
 *
 * Input Parameters:
 *   chan   - DMA channel handle created by fc7300_dma_get_chan()
 *
 * Returned Value:
 *   The number of bytes remaining to be transferred.
 *
 ****************************************************************************/

static size_t fc7300_dma_residual(struct dma_chan_s *chan)
{
  struct fc7300_dma_chan_s *dmach = (struct fc7300_dma_chan_s *)chan;
  struct fc7300_dma_dev_s *priv = dmach->priv;
  uint16_t clc;
  uint16_t regval16;
  uint32_t regval32;
  uintptr_t regaddr;
  uint32_t nbytes;
  size_t residual = 0;

  DEBUGASSERT(dmach != NULL);

  /* If the DMA is done, then the remaining count is zero */

  regaddr        = priv->base + FC7300_DMA_CFG_OFFSET(dmach->chidx) +
                   FC7300_DMA_CFG_CSR_OFFSET;
  regval16       = getreg16(regaddr);

  if ((regval16 & DMA_CFG_CSR_DONE) == 0)
    {
      /* Calculate the unfinished bytes */

      regaddr    = priv->base + FC7300_DMA_CFG_OFFSET(dmach->chidx) +
                   FC7300_DMA_CFG_CLC_CHTRG_OFFSET;
      regval16   = getreg16(regaddr);

      if ((regval16 & DMA_CFG_CLC_CHTRG_CHTRGEN) != 0)
        {
          clc    = (regval16 & DMA_CFG_CLC_CHTRG_CLC_MASK) >>
                   DMA_CFG_CLC_CHTRG_CLC_SHIFT;
        }
      else
        {
          clc    = (regval16 & DMA_CFG_CLC_CLC_MASK) >>
                   DMA_CFG_CLC_CLC_SHIFT;
        }

      regaddr    = priv->base + FC7300_DMA_CFG_OFFSET(dmach->chidx) +
                   FC7300_DMA_CFG_NBYTES_IL_OFFSET;
      regval32   = getreg32(regaddr);

#ifdef CONFIG_DMA_LINK
      if ((regval32 & (DMA_CFG_NBYTES_ILOFF_DILOE |
                       DMA_CFG_NBYTES_ILOFF_SILOE)) == 0)
        {
          nbytes = regval32 & DMA_CFG_NBYTES_IL_NBYTES_MASK;
        }
      else
        {
          nbytes = regval32 & DMA_CFG_NBYTES_ILOFF_NBYTES_MASK;
        }
#else
      nbytes     = regval32;
#endif

      residual   = clc * nbytes;
    }

  return residual;
}

#ifdef CONFIG_DMA_CYCBUF_TRANSMIT

/****************************************************************************
 * Name: fc7300_dma_chan_cycbuf_config
 *
 * Description:
 *   Config the circular buffer for DMA.
 *
 * Input Parameters:
 *   chan    - DMA channel handle created by fc7300_dma_get_chan()
 *   src     - The source start address of the circular buffer
 *   src_len - The circular buffer size, > 0 means src buffer is circular
 *             buffer, = 0 means normal
 *   dst     - The destination start address of the circular buffer
 *   dst_len - The circular buffer size, > 0 means dst buffer is circular
 *             buffer, = 0 means normal
 *
 *   NOTES:
 *   1. The start address of the circular buffer shall be power of 2 aligned
 *      to the size of the buffer. That's to say, if the buffer size is 64
 *      bytes, then the start address shall be 64 byte aligned. And if the
 *      buffer size is 72 bytes, then the start address shall be 128 bytes
 *      aligned.
 *   2. The buffer size is also suggested to be power of 2 aligned. If the
 *      buffer size is not power of 2 aligned, then unaligned modulo will be
 *      used. However, there are only 4 groups of unaligned modulo and if the
 *      variants of modulos ecceed the number, this function will return with
 *      error.
 *
 * Returned Value:
 *   Zero (OK) is returned on success; a negated errno value is returned on
 *   any failure.
 *
 ****************************************************************************/

static int fc7300_dma_chan_cycbuf_config(struct dma_chan_s *chan,
                                         unsigned int src,
                                         unsigned int src_len,
                                         unsigned int dst,
                                         unsigned int dst_len)
{
  struct fc7300_dma_chan_s *dmach = (struct fc7300_dma_chan_s *)chan;
  struct fc7300_dma_dev_s *priv = dmach->priv;
  uint32_t option = dmach->option & ~(DMA_CONFIG_CYCBUF_SRC |
                                      DMA_CONFIG_CYCBUF_SRC_UMO |
                                      DMA_CONFIG_CYCBUF_DST |
                                      DMA_CONFIG_CYCBUF_DST_UMO);
  uint8_t smod = 0;
  uint8_t dmod = 0;
  bool usesumo = false;
  bool usedumo = false;
  uint16_t sumo = 0;
  uint16_t dumo = 0;
  uint32_t umo = 0;
  uint8_t ums = 0;
  uint32_t regval32;
  uint16_t regval16;
  int16_t data_off;
  int i = 0;

  DEBUGASSERT(dmach != NULL);

  if (dmach->state != FC7300_DMA_INITIALIZED)
    {
      return -EPERM;
    }

  if (src_len > 0)
    {
      option |= DMA_CONFIG_CYCBUF_SRC;

      usesumo = !fc7300_dma_ispowerof2(src_len, &smod);

      if ((src & ((1 << smod) - 1)) != 0)
        {
          return -EINVAL;
        }

      if (usesumo)
        {
          option |= DMA_CONFIG_CYCBUF_SRC_UMO;

          data_off = (int16_t)getreg16(priv->base +
                                       FC7300_DMA_CFG_OFFSET(dmach->chidx) +
                                       FC7300_DMA_CFG_SOFF_OFFSET);

          if (data_off <= 0)
            {
              return -EINVAL;
            }

          sumo = (src_len / data_off - 1) * data_off;
        }
    }

  if (dst_len > 0)
    {
      option |= DMA_CONFIG_CYCBUF_DST;

      usedumo = !fc7300_dma_ispowerof2(dst_len, &dmod);

      if ((dst & ((1 << dmod) - 1)) != 0)
        {
          return -EINVAL;
        }

      if (usedumo)
        {
          option |= DMA_CONFIG_CYCBUF_SRC_UMO;

          data_off = (int16_t)getreg16(priv->base +
                                       FC7300_DMA_CFG_OFFSET(dmach->chidx) +
                                       FC7300_DMA_CFG_DOFF_OFFSET);

          if (data_off <= 0)
            {
              return -EINVAL;
            }

          dumo = (dst_len / data_off - 1) * data_off;
        }
    }

  if (usesumo && usedumo)
    {
      umo = DMA_DUMO_SUMO(sumo) | DMA_DUMO_DUMO(dumo);

      for (i = 0; i < FC7300_DMA_NDUMOS; i++)
        {
          /* We will try to reuse the umo with the same dumo and sumo
           * settings, or with the same dumo and sumo is 0, or vice versa.
           */

          regval32 = getreg32(priv->base + FC7300_DMA_DUMO_OFFSET(i));

          if ((regval32 == umo) ||
              ((regval32 & DMA_DUMO_SUMO_MASK) == (umo & DMA_DUMO_SUMO_MASK)
              && (regval32 & DMA_DUMO_DUMO_MASK) == 0) ||
              ((regval32 & DMA_DUMO_DUMO_MASK) == (umo & DMA_DUMO_DUMO_MASK)
              && (regval32 & DMA_DUMO_SUMO_MASK) == 0))
            {
              break;
            }
        }

      if (i == FC7300_DMA_NDUMOS)
        {
          for (i = 0; i < FC7300_DMA_NDUMOS; i++)
            {
              /* If not found, then try to occupy an empty umo */

              if (priv->dumo_refs[i] == 0)
                {
                  break;
                }
            }
        }

      if (i == FC7300_DMA_NDUMOS)
        {
          return -EPERM;
        }
    }
  else if (usesumo)
    {
      for (i = 0; i < FC7300_DMA_NDUMOS; i++)
        {
          /* We will try to reuse the umo with the same sumo settings */

          regval32 = getreg32(priv->base + FC7300_DMA_DUMO_OFFSET(i));

          if ((regval32 & DMA_DUMO_SUMO_MASK) == DMA_DUMO_SUMO(sumo))
            {
              umo = regval32;
              break;
            }
        }

      if (i == FC7300_DMA_NDUMOS)
        {
          for (i = 0; i < FC7300_DMA_NDUMOS; i++)
            {
              /* If not found, then try to occupy an empty sumo */

              regval32 = getreg32(priv->base + FC7300_DMA_DUMO_OFFSET(i));

              if ((regval32 & DMA_DUMO_SUMO_MASK) == 0)
                {
                  umo = regval32 | DMA_DUMO_SUMO(sumo);
                  break;
                }
            }
        }

      if (i == FC7300_DMA_NDUMOS)
        {
          return -EPERM;
        }
    }
  else if (usedumo)
    {
      for (i = 0; i < FC7300_DMA_NDUMOS; i++)
        {
          /* We will try to reuse the umo with the same dumo settings */

          regval32 = getreg32(priv->base + FC7300_DMA_DUMO_OFFSET(i));

          if ((regval32 & DMA_DUMO_DUMO_MASK) == DMA_DUMO_DUMO(dumo))
            {
              umo = regval32;
              break;
            }
        }

      if (i == FC7300_DMA_NDUMOS)
        {
          for (i = 0; i < FC7300_DMA_NDUMOS; i++)
            {
              /* If not found, then try to occupy an empty dumo */

              regval32 = getreg32(priv->base + FC7300_DMA_DUMO_OFFSET(i));

              if ((regval32 & DMA_DUMO_DUMO_MASK) == 0)
                {
                  umo = regval32 | DMA_DUMO_DUMO(dumo);
                  break;
                }
            }
        }

      if (i == FC7300_DMA_NDUMOS)
        {
          return -EPERM;
        }
    }

  if ((dmach->option & (DMA_CONFIG_CYCBUF_SRC_UMO |
                       DMA_CONFIG_CYCBUF_DST_UMO)) != 0)
    {
      /* If the previous configuation uses sumo or dumo, then decrease the
       * duno_refs counter.
       */

      regval16 = getreg16(priv->base +
                          FC7300_DMA_CFG_OFFSET(dmach->chidx) +
                          FC7300_DMA_CFG_CSR_OFFSET);
      ums = regval16 & DMA_CFG_CSR_UMS_MASK >> DMA_CFG_CSR_UMS_SHIFT;

      priv->dumo_refs[ums]--;

      if (priv->dumo_refs[ums] == 0)
        {
          putreg32(0, priv->base + FC7300_DMA_DUMO_OFFSET(ums));
        }
    }

  ums = i;

  /* Set the channel modulo for circular buffer */

  regval16 = getreg16(priv->base +
                      FC7300_DMA_CFG_OFFSET(dmach->chidx) +
                      FC7300_DMA_CFG_ATTR_OFFSET);
  regval16 = (regval16 &
              ~(DMA_CFG_ATTR_SMOD_MASK | DMA_CFG_ATTR_DMOD_MASK)) |
             (DMA_CFG_ATTR_SMOD(smod) | DMA_CFG_ATTR_DMOD(dmod));
  putreg16(regval16, priv->base +
                     FC7300_DMA_CFG_OFFSET(dmach->chidx) +
                     FC7300_DMA_CFG_ATTR_OFFSET);

  /* Enable/Disable the unaligned modulo flag for the channel */

  regval32 = getreg32(priv->base + FC7300_DMA_DUME_OFFSET);
  regval32 = (regval32 &
              ~(DMA_DUME_SUME(dmach->chidx) | DMA_DUME_DUME(dmach->chidx))) |
             (usesumo ? DMA_DUME_SUME(dmach->chidx) : 0) |
             (usedumo ? DMA_DUME_DUME(dmach->chidx) : 0);
  putreg32(regval32, priv->base + FC7300_DMA_DUME_OFFSET);

  if (usesumo || usedumo)
    {
      /* Set the unaligned modulo value */

      putreg32(umo, priv->base + FC7300_DMA_DUMO_OFFSET(ums));

      /* Select the unaligned modulo index for the channel */

      regval16 = getreg16(priv->base +
                          FC7300_DMA_CFG_OFFSET(dmach->chidx) +
                          FC7300_DMA_CFG_CSR_OFFSET);
      regval16 = (regval16 & ~ DMA_CFG_CSR_UMS_MASK) |
                 DMA_CFG_CSR_UMS(ums);
      putreg32(regval16, priv->base +
                         FC7300_DMA_CFG_OFFSET(dmach->chidx) +
                         FC7300_DMA_CFG_CSR_OFFSET);

      priv->dumo_refs[ums]++;
    }

  dmach->option = option;

  return OK;
}

/****************************************************************************
 * Name: fc7300_dma_getrp
 *
 * Description:
 *   Used to get circular buffer transmit source ptr
 *
 * Input Parameters:
 *   chan    - DMA channel handle created by fc7300_dma_get_chan()
 *
 * Returned Value:
 *   The DMA write ptr of the data to be transmitted.
 *
 ****************************************************************************/

static unsigned int fc7300_dma_getrp(struct dma_chan_s *chan)
{
  struct fc7300_dma_chan_s *dmach = (struct fc7300_dma_chan_s *)chan;
  struct fc7300_dma_dev_s *priv = dmach->priv;

  return getreg32(priv->base + FC7300_DMA_CFG_OFFSET(dmach->chidx) +
                      FC7300_DMA_CFG_SADDR_OFFSET);
}

/****************************************************************************
 * Name: fc7300_dma_getwp
 *
 * Description:
 *   Used to get circular buffer transmit destination ptr
 *
 * Input Parameters:
 *   chan    - DMA channel handle created by fc7300_dma_get_chan()
 *
 * Returned Value:
 *   The DMA read ptr of the data to be transmitted.
 *
 ****************************************************************************/

static unsigned int fc7300_dma_getwp(struct dma_chan_s *chan)
{
  struct fc7300_dma_chan_s *dmach = (struct fc7300_dma_chan_s *)chan;
  struct fc7300_dma_dev_s *priv = dmach->priv;

  return getreg32(priv->base + FC7300_DMA_CFG_OFFSET(dmach->chidx) +
                      FC7300_DMA_CFG_DADDR_OFFSET);
}
#endif

/****************************************************************************
 * Name: fc7300_dma_interrupt
 *
 * Description:
 *   DMA interrupt handler.  This function clears the channel outer
 *   interrupt flag and calls the callback function if it is not NULL.
 *
 *   NOTE:  For the case using CFG queue, when the outer iteration count is
 *   exhausted, additional operations are performed.  These include the
 *   final address adjustments and reloading of the BLC field into the
 *   CLC.  Assertion of an optional interrupt request also occurs at this
 *   time.
 *
 ****************************************************************************/

__ramfunc__ static int fc7300_dma_interrupt(int irq, void *context, void *arg)
{
  struct fc7300_dma_chan_s *dmach = (struct fc7300_dma_chan_s *)arg;
  struct fc7300_dma_dev_s *priv = dmach->priv;
  ssize_t delivered;
  uintptr_t regaddr;
  uint32_t  regval32;
  uint16_t  regval16;
  uint8_t   regval8;
  uint8_t   chidx;

  /* 'arg' should be the DMA channel instance. */

  DEBUGASSERT(dmach != NULL);

  chidx  = dmach->chidx;
  DEBUGASSERT(chidx < FC7300_DMA_NCHANNELS && dmach == priv->dmachs[chidx]);

  /* Check for an DMA pending interrupt on this channel */

  regval32 = getreg32(priv->base + FC7300_DMA_INT_OFFSET);
  if ((regval32 & DMA_INT(chidx)) != 0)
    {
      /* An interrupt is pending.  This should only happen if the channel is
       * active.
       */

      DEBUGASSERT(dmach->state == FC7300_DMA_ACTIVE);

      /* Clear the pending DMA channel interrupt */

      regval8 = DMA_CINT(chidx);
      putreg8(regval8, priv->base + FC7300_DMA_CINT_OFFSET);

      /* Get the transfered bytes */

      delivered = fc7300_dma_delivered(dmach);

      /* Get the DMA CFG Control and Status register value. */

      regaddr  = priv->base + FC7300_DMA_CFG_OFFSET(chidx) +
                 FC7300_DMA_CFG_CSR_OFFSET;
      regval16 = getreg16(regaddr);

      /* Check if transfer has finished. */

      if ((regval16 & DMA_CFG_CSR_DONE) != 0)
        {
          /* Clear the pending DONE interrupt status. */

          regval8 = DMA_CDNE(chidx);
          putreg8(regval8, priv->base + FC7300_DMA_CDNE_OFFSET);

          /* Terminate the transfer when it is done. */

          if (dmach->option & DMA_CONFIG_AUTOSTOP)
            {
              fc7300_dmaterminate(dmach);
            }

          if (dmach->callback != NULL)
            {
              dmach->callback(&dmach->chan, dmach->arg, delivered);
            }
        }
      else if ((regval16 & DMA_CFG_CSR_INTHALF) != 0)
        {
          /* Probably because half interrupt enabled */

          if (dmach->callback != NULL)
            {
              /* The transfer is half done, and thus it has not done */

              dmach->callback(&dmach->chan, dmach->arg, delivered);
            }
        }
      else
        {
          /* Done flg is activated then cleared by hw before isr entered */
        }
    }

#ifdef CONFIG_FC7300_DMA_IRQ_ATTACH_THREAD
  fc7300_clearpendingisr(irq);
  up_enable_irq(irq);
#endif

  return OK;
}

/****************************************************************************
 * Name: fc7300_error_interrupt
 *
 * Description:
 *  DMA error interrupt handler
 *
 ****************************************************************************/

__ramfunc__ static int fc7300_error_interrupt(int irq, void *context, void *arg)
{
  uint32_t errstatus;
  uint32_t errmask;
  uint8_t regval8;
  unsigned int chan;
  struct fc7300_dma_chan_s *dmach;
  struct fc7300_dma_dev_s *priv = (struct fc7300_dma_dev_s *)arg;

  /* Get the set of pending error interrupts */

  errstatus = getreg32(priv->base + FC7300_DMA_ERR_OFFSET);

  /* Check for an error on each channel */

  for (chan = 0; chan < FC7300_DMA_NCHANNELS && errstatus != 0; chan++)
    {
      /* Check for a pending error interrupt on each channel */

      errmask = DMA_ERR(chan);
      if ((errstatus & errmask) != 0)
        {
          dmaerr("ERROR: DMACH%u ES=%08lx\n", chan,
                 (unsigned long)getreg32(priv->base + FC7300_DMA_ES_OFFSET));

          /* Clear the pending error interrupt status. */

          regval8 = DMA_CERR(chan);
          putreg8(regval8, priv->base + FC7300_DMA_CERR_OFFSET);

          /* Remove the bit from the sample ERR register so that perhaps we
           * can exit this loop early.
           */

          errstatus &= ~errmask;

          /* Terminate the transfer on any error */

          dmach = priv->dmachs[chan];

          fc7300_dmaterminate(dmach);

          if (dmach->callback != NULL)
            {
              dmach->callback(&dmach->chan, dmach->arg, -EIO);
            }
        }
    }

  return OK;
}

/****************************************************************************
 * Public Functions
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

struct dma_dev_s *fc7300_dma_initialize(uint8_t dma_instance)
{
  struct fc7300_dma_dev_s *priv = NULL;
  uint32_t regval;
  uint32_t dma_base;
  int i;

  switch (dma_instance)
  {
    case 0:
#if defined(CONFIG_FC7300_DMA0)
      priv = &fc7300_dma0;
#endif
      break;
    case 1:
#if defined(CONFIG_FC7300_DMA1)
      priv = &fc7300_dma1;
#endif
      break;
    default:
      break;
  }

  if (!priv)
    {
      dmaerr("DMA%d Initialize failed!", dma_instance);
      return NULL;
    }

  dma_base = priv->base;

  for (i = 0; i < FC7300_DMA_NCHANNELS; i++)
    {
      nxmutex_init(&priv->chlock[i]);
    }

  /* Configure the DMA controller */

  regval  = getreg32(dma_base + FC7300_DMA_CR_OFFSET);
  regval &= ~(DMA_CR_DBGS | DMA_CR_ERCA | DMA_CR_HOE | DMA_CR_CTM |
              DMA_CR_EILM);

#ifdef CONFIG_DMA_LINK
  regval |= DMA_CR_EILM;   /* Enable Inner Loop Mapping */
#endif

  if (priv->cr_dbg)
    regval |= DMA_CR_DBGS;   /* Enable Debug */

  if (priv->cr_rca)
    regval |= DMA_CR_ERCA;   /* Enable Round Robin Channel Arbitration */

  if (priv->cr_hoe)
    regval |= DMA_CR_HOE;    /* Halt On Error */

  if (priv->cr_ctm)
    regval |= DMA_CR_CTM;    /* Continuous Trig Mode */

  putreg32(regval, dma_base + FC7300_DMA_CR_OFFSET);

  for (i = 0; i < FC7300_DMA_NDUMES; i++)
    {
      putreg32(0, dma_base + FC7300_DMA_DUME_OFFSET(i));
    }

  /* Disable and clear all error interrupts */

  putreg32(0, dma_base + FC7300_DMA_EEI_OFFSET);
  putreg32(0xffffffff, dma_base + FC7300_DMA_ERR_OFFSET);

  /* Clear all DMA channel CFG entries */

  for (i = 0; i < FC7300_DMA_NCHANNELS; i++)
    {
      /* Set all CFG entries to 0 to avoid ECC error */

      memset((void *)(dma_base + FC7300_DMA_CFG_OFFSET(i)), 0,
             sizeof(struct fc7300_dmacfg_s));
    }

#ifdef CONFIG_DMA_CYCBUF_TRANSMIT
  for (i = 0; i < FC7300_DMA_NDUMOS; i++)
    {
      putreg32(0, dma_base + FC7300_DMA_DUMO_OFFSET(i));
      priv->dumo_refs[i] = 0;
    }
#endif

  /* Disable DMA Monitor Check */

  regval  = getreg32(dma_base + FC7300_DMA_MON_CTRL_OFFSET);
  regval &= ~DMA_MON_CTRL_CHK_EN;
  putreg32(regval, dma_base + FC7300_DMA_MON_CTRL_OFFSET);

  /* Clear all pending DMA channel interrupts */

  putreg32(0xffffffff, dma_base + FC7300_DMA_INT_OFFSET);

  irq_attach(FC7300_IRQ_DMACH_ERR, fc7300_error_interrupt, priv);

#ifdef CONFIG_ARCH_IRQPRIO
  up_prioritize_irq(FC7300_IRQ_DMACH_ERR, priv->error_isr_prio);
#endif

  /* Enable the DMA error interrupt */

  up_enable_irq(FC7300_IRQ_DMACH_ERR);

  return &priv->dev;
}

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

struct dma_dev_s *fc7300_dma_getdev(uint8_t dma_instance)
{
  struct dma_dev_s *handle = NULL;

  switch (dma_instance)
    {
#ifdef CONFIG_FC7300_DMA0
      case DMA_INSTANCE_0:
        handle = g_dma0;
        break;
#endif

#ifdef CONFIG_FC7300_DMA1
      case DMA_INSTANCE_1:
        handle = g_dma1;
        break;
#endif

      default:
        break;
    }

  return handle;
}

/****************************************************************************
 * Name: arm_dma_initialize
 *
 * Description:
 *   Initialize the DMA subsystem
 *
 *   NOTE:  This assumes that board-specific logic has enabled clocking to
 *   the DMA module as part of the clock initialization logic.
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/

void weak_function arm_dma_initialize(void)
{
  dmainfo("Initialize DMA\n");

#ifdef CONFIG_FC7300_DMA0
  g_dma0 = fc7300_dma_initialize(DMA_INSTANCE_0);
#endif

#ifdef CONFIG_FC7300_DMA1
  g_dma1 = fc7300_dma_initialize(DMA_INSTANCE_1);
#endif
}

/****************************************************************************
 * Name: fc7300_dmasample
 *
 * Description:
 *   Sample DMA register contents
 *
 * Assumptions:
 *   - DMA handle allocated by fc7300_dmach_alloc()
 *
 ****************************************************************************/

#ifdef CONFIG_DEBUG_DMA
void fc7300_dmasample(struct dma_chan_s *chan, struct fc7300_dmaregs_s *regs)
{
  struct fc7300_dma_chan_s *dmach = (struct fc7300_dma_chan_s *)chan;
  struct fc7300_dma_dev_s *priv = dmach->priv;
  uintptr_t regaddr;
  unsigned int chidx;
  irqstate_t flags;

  DEBUGASSERT(dmach != NULL && regs != NULL);
  chidx          = dmach->chidx;
  regs->chidx    = chidx;

  /* DMA Global Registers */

  flags          = spin_lock_irqsave(NULL);

  regs->cr       = getreg32(priv->base + FC7300_DMA_CR_OFFSET);   /* Control */
  regs->es       = getreg32(priv->base + FC7300_DMA_ES_OFFSET);   /* Error Status */
  regs->erq      = getreg32(priv->base + FC7300_DMA_ERQ_OFFSET);  /* Enable Request */
  regs->irq      = getreg32(priv->base + FC7300_DMA_INT_OFFSET);  /* Interrupt Request */
  regs->err      = getreg32(priv->base + FC7300_DMA_ERR_OFFSET);  /* Error */
  regs->hrs      = getreg32(priv->base + FC7300_DMA_HRS_OFFSET);  /* Hardware Request Status */
  regs->dume     = getreg32(priv->base + FC7300_DMA_DUME_OFFSET); /* DMA Unalign Modulo Enable */

  /* DMA Channel registers */

  regaddr        = priv->base + FC7300_DMA_DCHPRI_OFFSET(chidx);
  regs->dchpri   = getreg8(regaddr);          /* Channel priority */

  /* DMA CFG */

  regaddr        = priv->base + FC7300_DMA_CFG_OFFSET(chidx);
  regs->saddr    = getreg32(regaddr + FC7300_DMA_CFG_SADDR_OFFSET);
  regs->soff     = getreg16(regaddr + FC7300_DMA_CFG_SOFF_OFFSET);
  regs->attr     = getreg16(regaddr + FC7300_DMA_CFG_ATTR_OFFSET);
  regs->nbytes   = getreg32(regaddr + FC7300_DMA_CFG_NBYTES_IL_OFFSET);
  regs->slast    = getreg32(regaddr + FC7300_DMA_CFG_SLAST_OFFSET);
  regs->daddr    = getreg32(regaddr + FC7300_DMA_CFG_DADDR_OFFSET);
  regs->doff     = getreg16(regaddr + FC7300_DMA_CFG_DOFF_OFFSET);
  regs->clc      = getreg16(regaddr + FC7300_DMA_CFG_CLC_CHTRG_OFFSET);
  regs->dlast    = getreg32(regaddr + FC7300_DMA_CFG_DLAST_OFFSET);
  regs->csr      = getreg16(regaddr + FC7300_DMA_CFG_CSR_OFFSET);
  regs->blc      = getreg16(regaddr + FC7300_DMA_CFG_BLC_CHTRG_OFFSET);

  /* DMAMUX registers */

  regaddr        = priv->muxbase + FC7300_DMAMUX_CHCFG_OFFSET(chan);
  regs->dmamux   = getreg32(regaddr);         /* Channel configuration */

  spin_unlock_irqrestore(NULL, flags);
}

/****************************************************************************
 * Name: fc7300_dmadump
 *
 * Description:
 *   Dump previously sampled DMA register contents
 *
 * Assumptions:
 *   - DMA handle allocated by fc7300_dmach_alloc()
 *
 ****************************************************************************/

void fc7300_dmadump(const struct fc7300_dmaregs_s *regs, const char *msg)
{
  unsigned int chan;

  DEBUGASSERT(regs != NULL && msg != NULL);

  chan = regs->chan;
  DEBUGASSERT(chan < FC7300_DMA_NCHANNELS);

  dmainfo("%s\n", msg);
  dmainfo("  DMA Global Registers:\n");
  dmainfo("          CR: %08x\n", regs->cr);
  dmainfo("          ES: %08x\n", regs->es);
  dmainfo("         ERQ: %08x\n", regs->erq);
  dmainfo("         INT: %08x\n", regs->irq);
  dmainfo("         ERR: %08x\n", regs->err);
  dmainfo("         HRS: %08x\n", regs->hrs);

  /* DMA Channel registers */

  dmainfo("  DMA Channel %u Registers:\n", chan);
  dmainfo("    DCHPRI: %02x\n", regs->dchpri);

  /* DMA CFG */

  dmainfo("  DMA Channel %u CFG Registers:\n", chan);
  dmainfo("       SADDR: %08x\n", regs->saddr);
  dmainfo("        SOFF: %04x\n", regs->soff);
  dmainfo("        ATTR: %04x\n", regs->attr);
  dmainfo("        NBML: %05x\n", regs->nbytes);
  dmainfo("       SLAST: %05x\n", regs->slast);
  dmainfo("       DADDR: %05x\n", regs->daddr);
  dmainfo("        DOFF: %04x\n", regs->doff);
  dmainfo("         CLC: %04x\n", regs->clc);
  dmainfo("       DLAST: %08x\n", regs->dlast);
  dmainfo("         CSR: %04x\n", regs->csr);
  dmainfo("         BLC: %04x\n", regs->blc);

  /* DMAMUX registers */

  dmainfo("  DMAMUX Channel %u Registers:\n", chan);
  dmainfo("      DMAMUX: %08x\n", regs->dmamux);
}
#endif /* CONFIG_DEBUG_DMA */
#endif /* CONFIG_FC7300_DMA */
