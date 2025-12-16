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

#include <sys/types.h>
#include <sys/ioctl.h>

#include <inttypes.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <assert.h>
#include <debug.h>
#include <unistd.h>

#include <arch/board/board.h>
#include <nuttx/irq.h>
#include <nuttx/arch.h>
#include <nuttx/fs/ioctl.h>
#include <nuttx/power/pm.h>
#include <nuttx/analog/adc.h>
#include <nuttx/analog/ioctl.h>
#include <nuttx/semaphore.h>
#include <nuttx/compiler.h>

#include "arm_internal.h"
#include "fc7300_clockconfig.h"
#include "fc7300_periphclocks.h"
#include "hardware/fc7300_pin.h"
#include "fc7300_adc.h"
#include "fc7300_dma.h"
#include "fc7300_fcpit.h"
#include "hardware/fc7300_dmamux.h"
#include "hardware/fc7300_fcpit.h"
#include "hardware/fc7300_pinmux.h"
#include "fc7300_irq.h"

/* Based on arch/src/fc7300/fc7300_adc.c */

/* FC7300 ADC "lower-half" support must be enabled */

#ifdef CONFIG_FC7300_ADC

/* This implementation is for the FC7300 ADC IP version 1 */

/* Supported ADC modes:
 *   - SW triggering with/without DMA transfer
 *   - HW triggering with DMA transfer
 *
 * (tested with ADC example app from NuttX apps repo).
 */

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* ADC Timeout */

#define ADC_TIMEOUT_CYCLE 15000000U

/* Get ADC int status */

#define CHECK_INT_REG(dev, X) \
          ({ \
            uint32_t regtmp = adc_getreg(dev, FC7300_ADC_INT_STATUS_OFFSET); \
            regtmp = (regtmp & X##_MASK) >> X##_SHIFT; \
            (regtmp != 0U) ? true : false; \
          })

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

/* ADC Register access */

static uint32_t adc_getreg(struct fc7300_dev_s *priv, int offset);
static void adc_putreg(struct fc7300_dev_s *priv, int offset,
                       uint32_t value);

/* ADC Interrupt Handler */

#if defined(CONFIG_FC7300_ADC0_IRQ) || defined(CONFIG_FC7300_ADC1_IRQ) || \
    defined(CONFIG_FC7300_ADC2_IRQ) || defined(CONFIG_FC7300_ADC3_IRQ)
static int fc7300_adc_interrupt_handle(int irq, void *context, void *arg);

static int adc_interrupt(struct adc_dev_s *dev);
#endif

static void adc_sampletime_cfg(struct fc7300_dev_s *priv, uint8_t select,
                               uint8_t sampletime);

/* ADC Driver Methods */

static int  adc_bind(struct adc_dev_s *dev,
                     const struct adc_callback_s *callback);
static void adc_reset(struct adc_dev_s *dev);
static int  adc_setup(struct adc_dev_s *dev);
static void adc_shutdown(struct adc_dev_s *dev);
static void adc_rxint(struct adc_dev_s *dev, bool enable);
static int  adc_ioctl(struct adc_dev_s *dev, int cmd, unsigned long arg);
static int  adc_enable(struct fc7300_dev_s *priv, bool enable);
static int  adc_set_ch(struct adc_dev_s *dev);

#if defined(ADC_HAVE_DMA)
static void adc_dmaconvcallback(struct dma_chan_s *chan, void *arg,
                                ssize_t len);
#endif

static void adc_reg_startconv(struct fc7300_dev_s *priv);
#if defined(CONFIG_FC7300_TMU)
static void adc_switchtmu(struct adc_dev_s *dev);
#endif

/****************************************************************************
 * Private Data
 ****************************************************************************/

#ifdef ADC0_HAVE_DMA
locate_code(".dtcm_data") uint32_t g_adc0dmabuffer[CONFIG_FC7300_ADC0_DMA_MAX_SAMPLES * CONFIG_FC7300_ADC0_DMA_NOTIFY_THRESH] = {0, };
locate_code(".dtcm_data") uint8_t g_adc0chlistbuffer[CONFIG_FC7300_ADC0_DMA_MAX_SAMPLES * CONFIG_FC7300_ADC0_DMA_NOTIFY_THRESH];
#endif
#ifdef ADC1_HAVE_DMA
locate_code(".dtcm_data") uint32_t g_adc1dmabuffer[CONFIG_FC7300_ADC1_DMA_MAX_SAMPLES * CONFIG_FC7300_ADC1_DMA_NOTIFY_THRESH] = {0, };
locate_code(".dtcm_data") uint8_t g_adc1chlistbuffer[CONFIG_FC7300_ADC1_DMA_MAX_SAMPLES * CONFIG_FC7300_ADC1_DMA_NOTIFY_THRESH];
#endif
#ifdef ADC2_HAVE_DMA
locate_code(".dtcm_data") uint32_t g_adc2dmabuffer[CONFIG_FC7300_ADC2_DMA_MAX_SAMPLES * CONFIG_FC7300_ADC2_DMA_NOTIFY_THRESH] = {0, };
locate_code(".dtcm_data") uint8_t g_adc2chlistbuffer[CONFIG_FC7300_ADC2_DMA_MAX_SAMPLES * CONFIG_FC7300_ADC2_DMA_NOTIFY_THRESH];
#endif
#ifdef ADC3_HAVE_DMA
locate_code(".dtcm_data") uint32_t g_adc3dmabuffer[CONFIG_FC7300_ADC3_DMA_MAX_SAMPLES * CONFIG_FC7300_ADC3_DMA_NOTIFY_THRESH] = {0, };
locate_code(".dtcm_data") uint8_t g_adc3chlistbuffer[CONFIG_FC7300_ADC3_DMA_MAX_SAMPLES * CONFIG_FC7300_ADC3_DMA_NOTIFY_THRESH] = {0, };
#endif

enum adc_checkstatustype_e adc_checkstatus[4u] = {CHECK_CFG1};
uint32_t adc_cfg1_regval[4u];
uint32_t adc_cfg2_regval[4u];
uint32_t adc_smpr_regval[4u];
uint8_t  adc_cfg1_retry_times[4u];
uint8_t  adc_cfg2_retry_times[4u];
uint8_t  adc_smpr_retry_times[4u];
#if defined(CONFIG_FC7300_TMU)
uint8_t  adc_toffset;
uint8_t  adc_tslope;
volatile uint16_t adc_tmuvalue;
#endif

/* ADC interface operations */

static const struct adc_ops_s g_adcops =
{
  .ao_bind        = adc_bind,
  .ao_reset       = adc_reset,
  .ao_setup       = adc_setup,
  .ao_shutdown    = adc_shutdown,
  .ao_rxint       = adc_rxint,
  .ao_ioctl       = adc_ioctl,
};

/* ADC0123 common data */

struct adccmn_data_s g_adc0_cmn =
{
  .refcount = 0,
  .lock = NXMUTEX_INITIALIZER,
};

struct adccmn_data_s g_adc1_cmn =
{
  .refcount = 0,
  .lock = NXMUTEX_INITIALIZER,
};

struct adccmn_data_s g_adc2_cmn =
{
  .refcount = 0,
  .lock = NXMUTEX_INITIALIZER,
};

struct adccmn_data_s g_adc3_cmn =
{
  .refcount = 0,
  .lock = NXMUTEX_INITIALIZER,
};

/* ADC instances are coupled in blocks */

#define ADC0CMN_DATA g_adc0_cmn
#define ADC1CMN_DATA g_adc1_cmn
#define ADC2CMN_DATA g_adc2_cmn
#define ADC3CMN_DATA g_adc3_cmn

/* ADC0 state */

#ifdef CONFIG_FC7300_ADC0

locate_code(".dtcm_data") uint32_t g_adc0_fifodata[CONFIG_FC7300_ADC0_FIFO_SIZE] = {0, };
locate_code(".dtcm_data") uint8_t g_adc0_fifochannel[CONFIG_FC7300_ADC0_FIFO_SIZE] = {0, };

static struct fc7300_dev_s g_adcpriv0 =
{
#ifdef CONFIG_FC7300_ADC0_IRQ
  .irq          = FC7300_IRQ_ADC0,
  .isr          = fc7300_adc_interrupt_handle,
#ifdef CONFIG_FC7300_ADC0_WDGINTEN
  .adcwdginten  = true,
#else
  .adcwdginten  = false,
#endif
#ifdef CONFIG_FC7300_ADC0_OVERRUNINTEN
  .overruninten = true,
#else
  .overruninten = false,
#endif
#ifdef CONFIG_FC7300_ADC0_CONVCOMPINTEN
  .convcompleteinten = true,
#else
  .convcompleteinten = false,
#endif
#endif /* CONFIG_FC7300_ADC0_IRQ */
  .cmn          = &ADC0CMN_DATA,
  .clockdivider = (enum adc_clockdivide_e)CONFIG_FC7300_ADC0_CLKDIV,
  .overrunmode  = ADC_OVERRUN_MODE_OVERWRITE,
  .sequencemode = (enum adc_seqmodetype_e)CONFIG_FC7300_ADC0_MODE,
  .waitenable   = false,
  .sample_time  =
                  {
                    CONFIG_FC7300_ADC0_SAMPTIME0,
                    CONFIG_FC7300_ADC0_SAMPTIME1,
                    CONFIG_FC7300_ADC0_SAMPTIME2,
                    CONFIG_FC7300_ADC0_SAMPTIME3
                  },
  .intf         = 0,
  .resolution   = (enum adc_resolutiontype_e)CONFIG_FC7300_ADC0_RESOLUTION,
  .base         = FC7300_ADC0_BASE,
  .refsel       = CONFIG_FC7300_ADC0_REF_SEL,
#ifdef CONFIG_FC7300_ADC0_HAVETIMER
  .tbase        = FC7300_ADC0_TIMER_BASE,
  .timeout      = CONFIG_FC7300_ADC0_TIMER_FREQ,
#endif
#ifdef ADC0_HAVE_DMA
  .dmachan      = CONFIG_FC7300_ADC0_DMA_CHANNEL,
  .dma_reqsrc   = FC7300_DMA_REQ_ADC0,
  .hasdma       = true,
  .pdmabuffer   = g_adc0dmabuffer,
  .pchlistsbuffer = g_adc0chlistbuffer,
  .thresh       = CONFIG_FC7300_ADC0_DMA_NOTIFY_THRESH,
#else
  .hasdma       = false,
#endif
#ifdef CONFIG_FC7300_ADC0_WDG
  .wdgchnsel    = CONFIG_FC7300_ADC0_WDG_CHANNEL,
  .highthres    = CONFIG_FC7300_ADC0_WDG_HIGHTHRES,
  .lowthres     = CONFIG_FC7300_ADC0_WDG_LOWTHRES,
#endif
  .fifo.fifochbuffer   = g_adc0_fifochannel,
  .fifo.fifodatabuffer = g_adc0_fifodata,
  .fifo.fifosize       = CONFIG_FC7300_ADC0_FIFO_SIZE,
};

locate_code(".dtcm_data") static struct adc_dev_s g_adcdev0 =
{
  .ad_ops      = &g_adcops,
  .ad_priv     = &g_adcpriv0,
};
#endif

/* ADC1 state */

#ifdef CONFIG_FC7300_ADC1

locate_code(".dtcm_data") uint32_t g_adc1_fifodata[CONFIG_FC7300_ADC1_FIFO_SIZE] = {0, };
locate_code(".dtcm_data") uint8_t g_adc1_fifochannel[CONFIG_FC7300_ADC1_FIFO_SIZE] = {0, };

static struct fc7300_dev_s g_adcpriv1 =
{
#ifdef CONFIG_FC7300_ADC1_IRQ
  .irq          = FC7300_IRQ_ADC1,
  .isr          = fc7300_adc_interrupt_handle,
#ifdef CONFIG_FC7300_ADC1_WDGINTEN
  .adcwdginten  = true,
#endif
#ifdef CONFIG_FC7300_ADC1_OVERRUNINTEN
  .overruninten = true,
#endif
#ifdef CONFIG_FC7300_ADC1_CONVCOMPINTEN
  .convcompleteinten = true,
#endif
#endif /* CONFIG_FC7300_ADC1_IRQ */
  .cmn          = &ADC1CMN_DATA,
  .clockdivider = (enum adc_clockdivide_e)CONFIG_FC7300_ADC1_CLKDIV,
  .overrunmode  = ADC_OVERRUN_MODE_OVERWRITE,
  .sequencemode = (enum adc_seqmodetype_e)CONFIG_FC7300_ADC1_MODE,
  .waitenable   = false,
  .sample_time  =
                  {
                    CONFIG_FC7300_ADC1_SAMPTIME0,
                    CONFIG_FC7300_ADC1_SAMPTIME1,
                    CONFIG_FC7300_ADC1_SAMPTIME2,
                    CONFIG_FC7300_ADC1_SAMPTIME3
                  },
  .intf         = 1,
  .resolution   = (enum adc_resolutiontype_e)CONFIG_FC7300_ADC1_RESOLUTION,
  .base         = FC7300_ADC1_BASE,
  .refsel       = CONFIG_FC7300_ADC1_REF_SEL,
#ifdef CONFIG_FC7300_ADC1_HAVETIMER
  .tbase        = FC7300_ADC1_TIMER_BASE,
  .timeout      = CONFIG_FC7300_ADC1_TIMER_FREQ,
#endif
#ifdef ADC1_HAVE_DMA
  .dmachan      = CONFIG_FC7300_ADC1_DMA_CHANNEL,
  .dma_reqsrc   = FC7300_DMA_REQ_ADC1,
  .hasdma       = true,
  .pdmabuffer   = g_adc1dmabuffer,
  .pchlistsbuffer = g_adc1chlistbuffer,
  .thresh       = CONFIG_FC7300_ADC1_DMA_NOTIFY_THRESH,
#else
  .hasdma       = false,
#endif
#ifdef CONFIG_FC7300_ADC1_WDG
  .wdgchnsel    = CONFIG_FC7300_ADC1_WDG_CHANNEL,
  .highthres    = CONFIG_FC7300_ADC1_WDG_HIGHTHRES,
  .lowthres     = CONFIG_FC7300_ADC1_WDG_LOWTHRES,
#endif
  .fifo.fifochbuffer   = g_adc1_fifochannel,
  .fifo.fifodatabuffer = g_adc1_fifodata,
  .fifo.fifosize       = CONFIG_FC7300_ADC1_FIFO_SIZE,
};

locate_code(".dtcm_data") static struct adc_dev_s g_adcdev1 =
{
  .ad_ops      = &g_adcops,
  .ad_priv     = &g_adcpriv1,
};
#endif

/* ADC2 state */

#ifdef CONFIG_FC7300_ADC2

locate_code(".dtcm_data") uint32_t g_adc2_fifodata[CONFIG_FC7300_ADC2_FIFO_SIZE] = {0, };
locate_code(".dtcm_data") uint8_t g_adc2_fifochannel[CONFIG_FC7300_ADC2_FIFO_SIZE] = {0, };

static struct fc7300_dev_s g_adcpriv2 =
{
#ifdef CONFIG_FC7300_ADC2_IRQ
  .irq          = FC7300_IRQ_ADC2,
  .isr          = fc7300_adc_interrupt_handle,
#ifdef CONFIG_FC7300_ADC2_WDGINTEN
  .adcwdginten  = true,
#endif
#ifdef CONFIG_FC7300_ADC2_OVERRUNINTEN
  .overruninten = true,
#endif
#ifdef CONFIG_FC7300_ADC2_CONVCOMPINTEN
  .convcompleteinten = true,
#endif
#endif /* CONFIG_FC7300_ADC2_IRQ */
  .cmn          = &ADC2CMN_DATA,
  .clockdivider = (enum adc_clockdivide_e)CONFIG_FC7300_ADC2_CLKDIV,
  .overrunmode  = ADC_OVERRUN_MODE_OVERWRITE,
  .sequencemode = (enum adc_seqmodetype_e)CONFIG_FC7300_ADC2_MODE,
  .waitenable   = false,
  .sample_time  =
                  {
                    CONFIG_FC7300_ADC2_SAMPTIME0,
                    CONFIG_FC7300_ADC2_SAMPTIME1,
                    CONFIG_FC7300_ADC2_SAMPTIME2,
                    CONFIG_FC7300_ADC2_SAMPTIME3
                  },
  .intf         = 2,
  .resolution   = (enum adc_resolutiontype_e)CONFIG_FC7300_ADC2_RESOLUTION,
  .base         = FC7300_ADC2_BASE,
  .refsel       = CONFIG_FC7300_ADC2_REF_SEL,
#ifdef CONFIG_FC7300_ADC2_HAVETIMER
  .tbase        = FC7300_ADC2_TIMER_BASE,
  .timeout      = CONFIG_FC7300_ADC2_TIMER_FREQ,
#endif
#ifdef ADC2_HAVE_DMA
  .dmachan      = CONFIG_FC7300_ADC2_DMA_CHANNEL,
  .dma_reqsrc   = FC7300_DMA_REQ_ADC2,
  .hasdma       = true,
  .pdmabuffer   = g_adc2dmabuffer,
  .pchlistsbuffer = g_adc2chlistbuffer,
  .thresh       = CONFIG_FC7300_ADC2_DMA_NOTIFY_THRESH,
#else
  .hasdma       = false,
#endif
#ifdef CONFIG_FC7300_ADC2_WDG
  .wdgchnsel    = CONFIG_FC7300_ADC2_WDG_CHANNEL,
  .highthres    = CONFIG_FC7300_ADC2_WDG_HIGHTHRES,
  .lowthres     = CONFIG_FC7300_ADC2_WDG_LOWTHRES,
#endif
  .fifo.fifochbuffer   = g_adc2_fifochannel,
  .fifo.fifodatabuffer = g_adc2_fifodata,
  .fifo.fifosize       = CONFIG_FC7300_ADC2_FIFO_SIZE,
};

locate_code(".dtcm_data") static struct adc_dev_s g_adcdev2 =
{
  .ad_ops      = &g_adcops,
  .ad_priv     = &g_adcpriv2,
};
#endif

/* ADC3 state */

#ifdef CONFIG_FC7300_ADC3

locate_code(".dtcm_data") uint32_t g_adc3_fifodata[CONFIG_FC7300_ADC3_FIFO_SIZE] = {0, };
locate_code(".dtcm_data") uint8_t g_adc3_fifochannel[CONFIG_FC7300_ADC3_FIFO_SIZE] = {0, };

static struct fc7300_dev_s g_adcpriv3 =
{
#ifdef CONFIG_FC7300_ADC3_IRQ
  .irq          = FC7300_IRQ_ADC3,
  .isr          = fc7300_adc_interrupt_handle,
#ifdef CONFIG_FC7300_ADC3_WDGINTEN
  .adcwdginten  = true,
#endif
#ifdef CONFIG_FC7300_ADC3_OVERRUNINTEN
  .overruninten = true,
#endif
#ifdef CONFIG_FC7300_ADC3_CONVCOMPINTEN
  .convcompleteinten = true,
#endif
#endif /* CONFIG_FC7300_ADC3_IRQ */
  .cmn          = &ADC3CMN_DATA,
  .clockdivider = (enum adc_clockdivide_e)CONFIG_FC7300_ADC3_CLKDIV,
  .overrunmode  = ADC_OVERRUN_MODE_OVERWRITE,
  .sequencemode = (enum adc_seqmodetype_e)CONFIG_FC7300_ADC3_MODE,
  .waitenable   = false,
  .sample_time  =
                  {
                    CONFIG_FC7300_ADC3_SAMPTIME0,
                    CONFIG_FC7300_ADC3_SAMPTIME1,
                    CONFIG_FC7300_ADC3_SAMPTIME3,
                    CONFIG_FC7300_ADC3_SAMPTIME3
                  },
  .intf         = 3,
  .resolution   = (enum adc_resolutiontype_e)CONFIG_FC7300_ADC3_RESOLUTION,
  .base         = FC7300_ADC3_BASE,
  .refsel       = CONFIG_FC7300_ADC3_REF_SEL,
#ifdef CONFIG_FC7300_ADC3_HAVETIMER
  .tbase        = FC7300_ADC3_TIMER_BASE,
  .timeout      = CONFIG_FC7300_ADC3_TIMER_FREQ,
#endif
#ifdef ADC3_HAVE_DMA
  .dmachan      = CONFIG_FC7300_ADC3_DMA_CHANNEL,
  .dma_reqsrc   = FC7300_DMA_REQ_ADC3,
  .hasdma       = true,
  .pdmabuffer   = g_adc3dmabuffer,
  .pchlistsbuffer = g_adc3chlistbuffer,
  .thresh       = CONFIG_FC7300_ADC3_DMA_NOTIFY_THRESH,
#else
  .hasdma       = false,
#endif
#ifdef CONFIG_FC7300_ADC3_WDG
  .wdgchnsel    = CONFIG_FC7300_ADC3_WDG_CHANNEL,
  .highthres    = CONFIG_FC7300_ADC3_WDG_HIGHTHRES,
  .lowthres     = CONFIG_FC7300_ADC3_WDG_LOWTHRES,
#endif
  .fifo.fifochbuffer   = g_adc3_fifochannel,
  .fifo.fifodatabuffer = g_adc3_fifodata,
  .fifo.fifosize       = CONFIG_FC7300_ADC3_FIFO_SIZE,
};

locate_code(".dtcm_data") static struct adc_dev_s g_adcdev3 =
{
  .ad_ops      = &g_adcops,
  .ad_priv     = &g_adcpriv3,
};
#endif

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: adc_getreg
 *
 * Description:
 *   Read the value of an ADC register.
 *
 * Input Parameters:
 *   priv   - A reference to the ADC block status
 *   offset - The offset to the register to read
 *
 * Returned Value:
 *   The current contents of the specified register
 *
 ****************************************************************************/

static uint32_t adc_getreg(struct fc7300_dev_s *priv, int offset)
{
  return getreg32(priv->base + offset);
}

/****************************************************************************
 * Name: adc_putreg
 *
 * Description:
 *   Write a value to an ADC register.
 *
 * Input Parameters:
 *   priv   - A reference to the ADC block status
 *   offset - The offset to the register to write to
 *   value  - The value to write to the register
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/

static void adc_putreg(struct fc7300_dev_s *priv, int offset,
                       uint32_t value)
{
  putreg32(value, priv->base + offset);
}

static void timer_putreg(struct fc7300_dev_s *priv, int offset,
                       uint32_t value)
{
  putreg32(value, priv->tbase + offset);
}

static uint32_t timer_getreg(struct fc7300_dev_s *priv, int offset)
{
  return getreg32(priv->tbase + offset);
}

/****************************************************************************
 * Name: fc7300_adc_pckfreq
 *
 * Description:
 *   Get the peripheral clock frequency for the ADCX peripheral
 *
 * Input Parameters:
 *   base - The base address of the ADCX peripheral registers
 *
 * Returned Value:
 *   The frequency of the ADCX functional input frequency
 *   (or zero on a failure)
 *
 ****************************************************************************/

static uint32_t fc7300_adc_pckfreq(uintptr_t base)
{
  enum pcc_clksrctype_e clkname;
  uint32_t pccclk;
  int ret;

  /* Get the PCC source clock */

  switch (base)
    {
#ifdef CONFIG_FC7300_ADC0
      case FC7300_ADC0_BASE:
        clkname = PCC_CLK_ADC0;
        break;
#endif
#ifdef CONFIG_FC7300_ADC1
      case FC7300_ADC1_BASE:
        clkname = PCC_CLK_ADC1;
        break;
#endif
#ifdef CONFIG_FC7300_ADC2
      case FC7300_ADC2_BASE:
        clkname = PCC_CLK_ADC2;
        break;
#endif
#ifdef CONFIG_FC7300_ADC3
      case FC7300_ADC3_BASE:
        clkname = PCC_CLK_ADC3;
        break;
#endif
    default:
      DEBUGPANIC();
      return -EINVAL;
    }

  ret = fc7300_get_pclkfreq(clkname, &pccclk);
  DEBUGASSERT(ret >= 0);
  if (ret < 0)
    {
      return 0;
    }

  return pccclk;
}

/****************************************************************************
 * Name: fc7300_adc_timer_pckfreq
 *
 * Description:
 *   Get the peripheral clock frequency for the FCPIT0 peripheral
 *
 * Input Parameters:
 *   base - The base address of the FCPIT0 peripheral registers
 *
 * Returned Value:
 *   The frequency of the FCPIT0 functional input frequency
 *   (or zero on a failure)
 *
 ****************************************************************************/

static uint32_t fc7300_adc_timer_pckfreq(void)
{
  uint32_t pccclk;
  int ret;

  /* Get the PCC source clock */

  ret = fc7300_get_pclkfreq(PCC_CLK_FCPIT0, &pccclk);
  DEBUGASSERT(ret >= 0);
  if (ret < 0)
    {
      return 0;
    }

  return pccclk;
}

/****************************************************************************
 * Name: adc_reg_startconv
 *
 * Description:
 *   Start (or stop) the ADC conversion process
 *
 * Input Parameters:
 *   priv - A reference to the ADC block status
 *
 * Returned Value:
 *
 ****************************************************************************/

static void adc_reg_startconv(struct fc7300_dev_s *priv)
{
  DEBUGASSERT(priv != NULL);

  /* Start the conversion of channels */

  adc_putreg(priv, FC7300_ADC_CONTROL_OFFSET, ADC_CONTROL_ADSTART(1U));
}

/****************************************************************************
 * Name: adc_hwreset
 *
 * Description:
 *   Reset the ADCn instance. It could reset the ADCn configured.
 *
 * Input Parameters:
 *   priv  - A reference to the ADC block status
 *
 * Returned Value:
 *
 ****************************************************************************/

static void adc_hwreset(struct fc7300_dev_s *priv)
{
  DEBUGASSERT(priv != NULL);

  /* Enable ADC reset state */

  adc_putreg(priv, FC7300_ADC_CONTROL_OFFSET, ADC_CONTROL_ADRST(1U));

  /* Release ADC from reset state */

  adc_putreg(priv, FC7300_ADC_CONTROL_OFFSET, 0U);
}

/****************************************************************************
 * Name: adc_getenable
 *
 * Description:
 *   It could get the enable status of ADCn configured.
 *
 * Input Parameters:
 *   priv  - A reference to the ADC block status
 *
 * Returned Value:
 *
 ****************************************************************************/

static inline bool adc_getenable(struct fc7300_dev_s *priv)
{
  uint32_t tmpval = adc_getreg(priv, FC7300_ADC_CONTROL_OFFSET);
  tmpval = (tmpval & ADC_CONTROL_ADEN_MASK) >> ADC_CONTROL_ADEN_SHIFT;

  return ((tmpval != 0U) ? true : false);
}

/****************************************************************************
 * Name: adc_getstart
 *
 * Description:
 *   It could get the start status of ADCn configured.
 *
 * Input Parameters:
 *   priv  - A reference to the ADC block status
 *
 * Returned Value:
 *
 ****************************************************************************/

static inline bool adc_getstart(struct fc7300_dev_s *priv)
{
  uint32_t tmpval = adc_getreg(priv, FC7300_ADC_CONTROL_OFFSET);
  tmpval = (tmpval & ADC_CONTROL_ADSTART_MASK) >> ADC_CONTROL_ADSTART_SHIFT;

  return ((tmpval != 0U) ? true : false);
}

/****************************************************************************
 * Name: adc_getstop
 *
 * Description:
 *   It could get the stop status of ADCn configured.
 *
 * Input Parameters:
 *   priv  - A reference to the ADC block status
 *
 * Returned Value:
 *
 ****************************************************************************/

static inline bool adc_getstop(struct fc7300_dev_s *priv)
{
  uint32_t tmpval = adc_getreg(priv, FC7300_ADC_CONTROL_OFFSET);
  tmpval = (tmpval & ADC_CONTROL_ADSTP_MASK) >> ADC_CONTROL_ADSTP_SHIFT;

  return ((tmpval != 0U) ? true : false);
}

/****************************************************************************
 * Name: adc_stop
 *
 * Description:
 *   It could stop the ADCn instance configured.
 *
 * Input Parameters:
 *   priv  - A reference to the ADC block status
 *
 * Returned Value:
 *
 ****************************************************************************/

static inline int adc_stop(struct fc7300_dev_s *priv)
{
  uint32_t timeout = ADC_TIMEOUT_CYCLE;
  int ret = OK;

  if (adc_getstart(priv) == true)
    {
      /* stop Adc instance */

      adc_putreg(priv, FC7300_ADC_CONTROL_OFFSET, ADC_CONTROL_ADSTP(1U));
      while ((adc_getstop(priv) == true) && (timeout != 0U))
        {
          timeout--;
        }

      if (timeout != 0U)
        {
#if !defined (CONFIG_ARCH_CHIP_FC7300F8M)
          adc_waitclk(priv);
#endif
        }
      else
        {
          ret = -ETIMEDOUT;
        }
    }

  return ret;
}

#if !defined (CONFIG_ARCH_CHIP_FC7300F8M)
/****************************************************************************
 * Name: adc_waitclk
 *
 * Description:
 *   Waiting for clock stable of ADCn.
 *
 * Input Parameters:
 *   priv  - A reference to the ADC block status
 *
 * Returned Value:
 *
 ****************************************************************************/

static inline void adc_waitclk(struct fc7300_dev_s *priv)
{
  uint32_t sysfreq;
  uint32_t adcfreq;
  uint32_t clkcnt;
  enum pcc_clksrctype_e adcclk = PCC_CLK_ADC0;
  clkcnt = 4U;

  switch (priv->base)
  {
    case FC7300_ADC0_BASE:
    {
        adcclk = PCC_CLK_ADC0;
    }
    break;

    case FC7300_ADC1_BASE:
    {
        adcclk = PCC_CLK_ADC1;
    }
    break;

    case FC7300_ADC2_BASE:
    {
        adcclk = PCC_CLK_ADC2;
    }
    break;

    case FC7300_ADC3_BASE:
    {
        adcclk = PCC_CLK_ADC3;
    }
    break;

    default:
        break;
  }

  if (OK == fc7300_get_pclkfreq(adcclk, &adcfreq))
    {
      clkcnt = (sysfreq % adcfreq == 0U) ? (sysfreq / adcfreq) :
               (sysfreq / adcfreq + 1U);

      /* Run this counter down to zero */

      while (clkcnt != 0U)
      {
        /* Since we need a three cycle delay, we assume the decrement is
         * one cycle and insert two NOP instructions. The actual delay will
         * be larger because of loop overhead and compiler optimization.
         */

        clkcnt--;
        __asm__ volatile("nop");
        __asm__ volatile("nop");
      }
    }
}
#else
/****************************************************************************
 * Name: adc_getclockgatingack
 *
 * Description:
 *   Whether clock gating is acknowledged.
 *
 * Input Parameters:
 *   priv  - A reference to the ADC block status
 *
 * Returned Value:
 *
 ****************************************************************************/

static inline bool adc_getclockgatingack(struct fc7300_dev_s *priv)
{
  uint32_t regtmp = adc_getreg(priv, FC7300_ADC_CFG2_OFFSET);
  regtmp = (regtmp & ADC_CFG2_CG_ACK_MASK) >> ADC_CFG2_CG_ACK_SHIFT;
  return (regtmp != 0U) ? true : false;
}
#endif

/****************************************************************************
 * Name: adc_clearendofseq
 *
 * Description:
 *   Clear the ADC conversion sequence complete flag.
 *
 * Input Parameters:
 *   priv  - A reference to the ADC block status
 *
 * Returned Value:
 *
 ****************************************************************************/

static inline void adc_clearendofseq(struct fc7300_dev_s *priv)
{
  adc_putreg(priv, FC7300_ADC_INT_STATUS_OFFSET, ADC_INT_STATUS_EOSEQ(1U));
}

/****************************************************************************
 * Name: adc_clearendofconv
 *
 * Description:
 *   Clear the ADC single conversion complete flag.
 *
 * Input Parameters:
 *   priv  - A reference to the ADC block status
 *
 * Returned Value:
 *
 ****************************************************************************/

static inline void adc_clearendofconv(struct fc7300_dev_s *priv)
{
  adc_putreg(priv, FC7300_ADC_INT_STATUS_OFFSET, ADC_INT_STATUS_EOC(1U));
}

/****************************************************************************
 * Name: adc_clearoverrun
 *
 * Description:
 *   Deinitializes the ADCx peripheral registers to their default
 *   reset values. It could set all the ADCs configured.
 *
 * Input Parameters:
 *   priv  - A reference to the ADC block status
 *
 * Returned Value:
 *
 ****************************************************************************/

static inline void adc_clearoverrun(struct fc7300_dev_s *priv)
{
  adc_putreg(priv, FC7300_ADC_INT_STATUS_OFFSET, ADC_INT_STATUS_OVR(1U));
}

/****************************************************************************
 * Name: adc_clearcompareflg
 *
 * Description:
 *   Clear the compare flag.
 *
 * Input Parameters:
 *   priv  - A reference to the ADC block status
 *
 * Returned Value:
 *
 ****************************************************************************/

static inline void adc_clearcompareflg(struct fc7300_dev_s *priv)
{
  adc_putreg(priv, FC7300_ADC_INT_STATUS_OFFSET, ADC_INT_STATUS_ACMP(1U));
}

/****************************************************************************
 * Name: adc_getfifodata
 *
 * Description:
 *   Get the conversion result FIFO data of the ADC instance.
 *
 * Input Parameters:
 *   priv  - A reference to the ADC block status
 *
 * Returned Value:
 *
 ****************************************************************************/

static inline uint32_t adc_getfifodata(struct fc7300_dev_s *priv)
{
  uint32_t regtmp = adc_getreg(priv, FC7300_ADC_FIFO_DATA_OFFSET);
  regtmp = (regtmp & ADC_FIFO_DATA_FIFO_DATA_MASK) >>
           ADC_FIFO_DATA_FIFO_DATA_SHIFT;
  return regtmp;
}

/****************************************************************************
 * Name: adc_enable
 *
 * Description:
 *   Enables or disables the specified ADC peripheral.  Also, starts a
 *   conversion when the ADC mode is ADC_SEQMODE_SINGLE.
 *
 * Input Parameters:
 *
 *   enable - true:  enable ADC conversion
 *            false: disable ADC conversion
 *
 * Returned Value:
 *
 ****************************************************************************/

static int adc_enable(struct fc7300_dev_s *priv, bool enable)
{
  DEBUGASSERT(priv != NULL);
  int ret = OK;
  uint32_t timeout = ADC_TIMEOUT_CYCLE;

  if (enable)
    {
      /* Enable Adc instance */

      adc_putreg(priv, FC7300_ADC_CONTROL_OFFSET, ADC_CONTROL_ADEN(1U));
      while ((CHECK_INT_REG(priv, ADC_INT_STATUS_ADRDY) != true) &&
             (timeout != 0U))
        {
          timeout--;
        }

      if (timeout != 0U)
        {
          /* Clear the ready flag of the Adc instance */

          adc_putreg(priv, FC7300_ADC_INT_STATUS_OFFSET,
                     ADC_INT_STATUS_ADRDY(1U));
        }
      else
        {
          ret = -ETIMEDOUT;
        }
    }
  else
    {
      if (adc_getstart(priv) == true)
        {
          /* stop Adc instance */

          ret = adc_stop(priv);
        }

      if (ret == OK)
        {
          /* Disable Adc instance */

          adc_putreg(priv, FC7300_ADC_CONTROL_OFFSET, ADC_CONTROL_ADDIS(1U));
          while ((adc_getenable(priv) == true) && (timeout != 0U))
            {
              timeout--;
            }

          if (timeout == 0U)
            {
              ret = -ETIMEDOUT;
            }
        }
    }

  return ret;
}

/****************************************************************************
 * Name: adc_bind
 *
 * Description:
 *   Bind the upper-half driver callbacks to the lower-half implementation.
 *   This must be called early in order to receive ADC event notifications.
 *
 ****************************************************************************/

static int adc_bind(struct adc_dev_s *dev,
                    const struct adc_callback_s *callback)
{
  struct fc7300_dev_s *priv = (struct fc7300_dev_s *)dev->ad_priv;

  DEBUGASSERT(priv != NULL);
  priv->cb = callback;

  return OK;
}

#if defined(CONFIG_FC7300_ADC_WDG)
/****************************************************************************
 * Name: adc_wdog_enable
 *
 * Description:
 *   Enable adc watchdog.
 *
 * Input Parameters:
 *
 *   priv  - A reference to the ADC block status
 *   enable - enable or disable the adc watchdog
 *
 * Returned Value: void
 *
 ****************************************************************************/

static void adc_wdog_enable (struct fc7300_dev_s *priv, bool enable)
{
  uint32_t regtemp;

  /* Enable or disable the watchdog  */

  regtemp = adc_getreg(priv, FC7300_ADC_CMP_CTRL_OFFSET);
  regtemp = (regtemp & ~ADC_CMP_CTRL_ACMPEN_MASK) |
            ADC_CMP_CTRL_ACMPEN(enable);
  adc_putreg(priv, FC7300_ADC_CMP_CTRL_OFFSET, regtemp);
}

/****************************************************************************
 * Name: adc_watchdog_cfg
 *
 * Description:
 *   Config adc watchdog.
 *
 * Input Parameters:
 *
 *   priv  - A reference to the ADC block status
 *
 * Returned Value: void
 *
 ****************************************************************************/

static void adc_watchdog_cfg(struct fc7300_dev_s *priv)
{
  uint32_t regtemp;

  /* Initialize the watchdog high and low threshold register */

  regtemp = ADC_CMP_TR_LT(priv->lowthres) | ADC_CMP_TR_HT(priv->highthres);
  adc_putreg(priv, FC7300_ADC_CMP_TR_OFFSET, regtemp);

  /* Initialize the watchdog channel register */

  regtemp = adc_getreg(priv, FC7300_ADC_CMP_CTRL_OFFSET);
  regtemp = (regtemp & (~ADC_CMP_CTRL_ACMPSGL_MASK) &
            (~ADC_CMP_CTRL_ACMPCH_MASK)) |
            ADC_CMP_CTRL_ACMPSGL(ADC_CMP_CHANNEL_SINGLE) |
            ADC_CMP_CTRL_ACMPCH(priv->wdgchnsel);
  adc_putreg(priv, FC7300_ADC_CMP_CTRL_OFFSET, regtemp);

  adc_wdog_enable(priv, true);
}
#endif

/****************************************************************************
 * Name: adc_sampletime_cfg
 *
 * Description:
 *   Config adc sample time.
 *
 * Input Parameters:
 *
 *   priv  - A reference to the ADC block status
 *   select - select one of the sample time options to be configured
 *   sampletime - the value of sampletime
 *
 * Returned Value: void
 *
 ****************************************************************************/

static void adc_sampletime_cfg(struct fc7300_dev_s *priv, uint8_t select,
                               uint8_t sampletime)
{
  /* Initialize the same sample time for each ADC.
   * During sample cycles channel selection bits must remain unchanged.
   */

  uint32_t regtemp;
  uint8_t instancenum = 0;

  DEBUGASSERT(select < ADC_SAMPLE_TIME_OPTION_CNT);

  regtemp = adc_getreg(priv, FC7300_ADC_SMPR_OFFSET);
  regtemp = (regtemp & ~(ADC_SMPR_SMP_OPT0_MASK << (8U * select))) |
            (ADC_SMPR_SMP_OPT0(sampletime) << (8U * select));
  adc_putreg(priv, FC7300_ADC_SMPR_OFFSET, regtemp);
  switch (priv->base)
  {
    case FC7300_ADC0_BASE:
    {
        instancenum = 0;
    }
    break;

    case FC7300_ADC1_BASE:
    {
        instancenum = 1;
    }
    break;

    case FC7300_ADC2_BASE:
    {
        instancenum = 2;
    }
    break;

    case FC7300_ADC3_BASE:
    {
        instancenum = 3;
    }
    break;

    default:
        break;
  }
  adc_smpr_regval[instancenum] |= ADC_SMPR_SMP_OPT0(sampletime) << (8U * select);
}

/****************************************************************************
 * Name: adc_module_cfg
 *
 * Description:
 *   Config adc instance.
 *
 * Input Parameters:
 *
 *   priv  - A reference to the ADC block status
 *
 * Returned Value: void
 *
 ****************************************************************************/

static void adc_module_cfg(struct fc7300_dev_s *priv)
{
  uint32_t adcfreq = fc7300_adc_pckfreq(priv->base);
  uint32_t clockdiv;
  uint32_t cfg1;
  uint32_t cfg2;
  uint32_t regtemp;
  uint8_t  smprindex;
  uint8_t instancenum = 0;
  enum pcc_clksrctype_e adcclk = PCC_CLK_ADC0;

#if defined (CONFIG_ARCH_CHIP_FC7300F8M)
  uint32_t timeout = ADC_TIMEOUT_CYCLE;
  clockdiv = 1U << priv->clockdivider;
#else
  clockdiv = 1U;
#endif

  switch (priv->base)
  {
    case FC7300_ADC0_BASE:
    {
        adcclk = PCC_CLK_ADC0;
        instancenum = 0;
    }
    break;

    case FC7300_ADC1_BASE:
    {
        adcclk = PCC_CLK_ADC1;
        instancenum = 1;
    }
    break;

    case FC7300_ADC2_BASE:
    {
        adcclk = PCC_CLK_ADC2;
        instancenum = 2;
    }
    break;

    case FC7300_ADC3_BASE:
    {
        adcclk = PCC_CLK_ADC3;
        instancenum = 3;
    }
    break;

    default:
        break;
  }

  if (OK == fc7300_get_pclkfreq(adcclk, &adcfreq))
    {
      priv->startupcnt = adcfreq / clockdiv / 1000000U * 5U + 1U;
    }
  else
    {
      return;
    }

  if (priv->startupcnt < 2U)
    {
      priv->startupcnt = 2U;
    }
  else if (priv->startupcnt > 255U)
    {
      priv->startupcnt = 255U;
    }
  else
    {
      /* do nothing */
    }

  if (priv->sequencemode)
    {
      cfg1 = ADC_CFG1_OVRMOD(priv->overrunmode) |
            ADC_CFG1_SEQ_LEN(0) |
            ADC_CFG1_SEQ_MOD(0) |
            ADC_CFG1_WAIT(priv->waitenable) |
    #if defined (CONFIG_ARCH_CHIP_FC7300F8M)
            ADC_CFG1_DIFF(false) |
    #endif
            ADC_CFG1_TRIGSRC(ADC_TRIGSRC_TRGSEL) |
            ADC_CFG1_TRIGMODE(ADC_TRIGMODE_RISING_EDGE) |
            ADC_CFG1_ALIGN(ADC_ALIGN_RIGHT) |
            ADC_CFG1_RES(priv->resolution) |
            ADC_CFG1_DMAEN(false);
      adc_putreg(priv, FC7300_ADC_CFG1_OFFSET, cfg1);
    }
  else
    {
      cfg1 = ADC_CFG1_OVRMOD(priv->overrunmode) |
            ADC_CFG1_SEQ_LEN(0) |
            ADC_CFG1_SEQ_MOD(0) |
            ADC_CFG1_WAIT(priv->waitenable) |
    #if defined (CONFIG_ARCH_CHIP_FC7300F8M)
            ADC_CFG1_DIFF(false) |
    #endif
            ADC_CFG1_TRIGSRC(ADC_TRIGSRC_TRGSEL) |
            ADC_CFG1_TRIGMODE(ADC_TRIGMODE_SW) |
            ADC_CFG1_ALIGN(ADC_ALIGN_RIGHT) |
            ADC_CFG1_RES(priv->resolution) |
            ADC_CFG1_DMAEN(false);
      adc_putreg(priv, FC7300_ADC_CFG1_OFFSET, cfg1);
    }

  cfg2 = ADC_CFG2_FWMARK(ADC_DEFAULT_WATER_MARK) |
         ADC_CFG2_AVG_EN(false) |
         ADC_CFG2_AVG_LEN(ADC_AVERAGE_4) |
         ADC_CFG2_REF_EXT(priv->refsel) |
         ADC_CFG2_STCNT(priv->startupcnt);
  adc_putreg(priv, FC7300_ADC_CFG2_OFFSET, cfg2);

#if defined (CONFIG_ARCH_CHIP_FC7300F8M)
  /* Set clock gating enable flag */

  regtemp = adc_getreg(priv, FC7300_ADC_CFG2_OFFSET);
  regtemp = (regtemp & ~ADC_CFG2_CG_MASK) | ADC_CFG2_CG(true);
  adc_putreg(priv, FC7300_ADC_CFG2_OFFSET, regtemp);

  while ((adc_getclockgatingack(priv) != true) && (timeout != 0))
    {
      timeout--;
    }

  if (adc_getclockgatingack(priv) == true)
    {
      /* Set clock divider */

      regtemp = adc_getreg(priv, FC7300_ADC_CFG2_OFFSET);
      regtemp = (regtemp & ~ADC_CFG2_DIV_MASK) |
                ADC_CFG2_DIV(priv->clockdivider);
      adc_putreg(priv, FC7300_ADC_CFG2_OFFSET, regtemp);
    }

  timeout = ADC_TIMEOUT_CYCLE;

  /* Set clock gating enable flag */

  regtemp = adc_getreg(priv, FC7300_ADC_CFG2_OFFSET);
  regtemp = (regtemp & ~ADC_CFG2_CG_MASK) | ADC_CFG2_CG(false);
  adc_putreg(priv, FC7300_ADC_CFG2_OFFSET, regtemp);

  while ((adc_getclockgatingack(priv) != false) && (timeout != 0))
    {
        timeout--;
    }
#endif

  for (smprindex = 0U; smprindex < ADC_SAMPLE_TIME_OPTION_CNT; smprindex++)
    {
      adc_sampletime_cfg(priv, smprindex, priv->sample_time[smprindex] - 2U);
    }

  /* pretrigger and trigger no need to set in continuous mode */
  if (priv->hasdma)
    {
      adc_cfg1_regval[instancenum] = (ADC_CFG1_OVRMOD(ADC_OVERRUN_MODE_OVERWRITE) |
                                      ADC_CFG1_SEQ_LEN(priv->nchannels - 1) |
                                      ADC_CFG1_TRIGSRC(ADC_TRIGSRC_TRGSEL) |
                                      ADC_CFG1_TRIGMODE(ADC_TRIGMODE_RISING_EDGE) |
                                      ADC_CFG1_ALIGN(ADC_ALIGN_RIGHT) |
                                      ADC_CFG1_DMAEN(true));
    }
  else
    {
      adc_cfg1_regval[instancenum] = (ADC_CFG1_OVRMOD(ADC_OVERRUN_MODE_OVERWRITE) |
                                      ADC_CFG1_SEQ_LEN(priv->nchannels - 1) |
                                      ADC_CFG1_TRIGSRC(ADC_TRIGSRC_TRGSEL) |
                                      ADC_CFG1_TRIGMODE(ADC_TRIGMODE_SW) |
                                      ADC_CFG1_ALIGN(ADC_ALIGN_RIGHT) |
                                      ADC_CFG1_DMAEN(false));
    }
  adc_cfg2_regval[instancenum] = (ADC_CFG2_FWMARK(priv->nchannels - 1) |
                                  ADC_CFG2_AVG_LEN(ADC_AVERAGE_4) |
                                  ADC_CFG2_REF_EXT(priv->refsel) |
                                  ADC_CFG2_DIV(priv->clockdivider) |
                                  ADC_CFG2_STCNT(priv->startupcnt));
}

#if defined(CONFIG_FC7300_ADC0_IRQ) || defined(CONFIG_FC7300_ADC1_IRQ) || \
    defined(CONFIG_FC7300_ADC2_IRQ) || defined(CONFIG_FC7300_ADC3_IRQ)
/****************************************************************************
 * Name: fc7300_adc_isr_handle
 *
 * Description:
 *   Disable adcn interrupt.
 *
 * Input Parameters:
 *
 * Returned Value: void
 *
 ****************************************************************************/

#ifdef CONFIG_FC7300_ADC_IRQ_ATTACH_THREAD
__ramfunc__ static int fc7300_adc_isr_handle(int irq, void *regs, void *arg)
{
  up_disable_irq(irq);

  return IRQ_WAKE_THREAD;
}
#endif

/****************************************************************************
 * Name: adc_interrupt_cfg
 *
 * Description:
 *   Config adcn interrupt.
 *
 * Input Parameters:
 *
 *   priv  - A reference to the ADC block status
 *   enable - enable or disable adcn interrupt
 *
 * Returned Value: void
 *
 ****************************************************************************/

static int adc_interrupt_cfg(struct adc_dev_s *dev, bool enable)
{
  struct fc7300_dev_s *priv = (struct fc7300_dev_s *)dev->ad_priv;
  uint32_t interruptcfg;
  int ret = OK;

  if ((enable) && (!priv->hasdma))
    {
      /* Attach the ADC interrupt */

#ifdef CONFIG_FC7300_ADC_IRQ_ATTACH_THREAD
      ret = irq_attach_wqueue(priv->irq, fc7300_adc_isr_handle, priv->isr,
                              dev, isr_cfg[ADC_ISR_THREAD]);
#else
      ret = irq_attach(priv->irq, priv->isr, dev);
#endif

      if (ret < 0)
        {
          ainfo("irq_attach failed: %d\n", ret);
          return ret;
        }

      interruptcfg = ADC_INT_ENABLE_FIFO_RDY_IE(false) |
                     ADC_INT_ENABLE_ACMP_IE(priv->adcwdginten) |
                     ADC_INT_ENABLE_OVRIE(priv->overruninten) |
                     ADC_INT_ENABLE_EOSEQIE(priv->convcompleteinten) |
                     ADC_INT_ENABLE_EOCIE(false) |
                     ADC_INT_ENABLE_EOSMPIE(false) |
                     ADC_INT_ENABLE_ADRDYIE(false);
      adc_putreg(priv, FC7300_ADC_INT_ENABLE_OFFSET, interruptcfg);

#ifdef CONFIG_ARCH_IRQPRIO
      switch (priv->base)
        {
#ifdef CONFIG_FC7300_ADC0_IRQ
          case FC7300_ADC0_BASE:
            up_prioritize_irq(priv->irq, NVIC_ADC0_PRIORITY_DEFAULT);
            break;
#endif
#ifdef CONFIG_FC7300_ADC1_IRQ
          case FC7300_ADC1_BASE:
            up_prioritize_irq(priv->irq, NVIC_ADC1_PRIORITY_DEFAULT);
            break;
#endif
#ifdef CONFIG_FC7300_ADC2_IRQ
          case FC7300_ADC2_BASE:
            up_prioritize_irq(priv->irq, NVIC_ADC2_PRIORITY_DEFAULT);
            break;
#endif
#ifdef CONFIG_FC7300_ADC3_IRQ
          case FC7300_ADC3_BASE:
            up_prioritize_irq(priv->irq, NVIC_ADC3_PRIORITY_DEFAULT);
            break;
#endif
          default:
            break;
        }
#endif

      /* Enable the ADC interrupt */

      ainfo("Enable the ADC interrupt: irq=%d\n", priv->irq);
      up_enable_irq(priv->irq);
    }
  else
    {
      interruptcfg = 0;
      adc_putreg(priv, FC7300_ADC_INT_ENABLE_OFFSET, interruptcfg);
#ifdef CONFIG_FC7300_ADC_IRQ_ATTACH_THREAD
      if (!priv->hasdma)
      {
        /* Detach the ADC interrupt */

        ret = irq_detach_wqueue(priv->irq);
      }
#endif
    }

  return ret;
}
#endif /* (CONFIG_FC7300_ADCx_IRQ) */

#ifdef ADC_HAVE_DMA

/****************************************************************************
 * Name: adc_dmaconvcallback
 *
 * Description:
 *   This function checks the current DMA state and calls the generic
 *   serial stack when bytes appear to be available.
 *
 ****************************************************************************/

__ramfunc__ static void adc_dmaconvcallback(struct dma_chan_s *chan, void *arg,
                                  ssize_t len)
{
  struct adc_dev_s *dev = (struct adc_dev_s *)arg;
  struct fc7300_dev_s *priv = (struct fc7300_dev_s *)dev->ad_priv;

  if ((CHECK_INT_REG(priv, ADC_INT_STATUS_EOSEQ) == true))
    {
      /* EOC: End of conversion */

      adc_clearendofseq(priv);
      DEBUGASSERT(priv->cb->au_receive_batch != NULL);
#if defined(CONFIG_FC7300_TMU)
      if (priv->base == FC7300_ADC0_BASE)
      {
          priv->cb->au_receive_batch(dev, priv->pchlistsbuffer,
                                     (void *)priv->pdmabuffer,
                                     priv->nchannels * priv->thresh);
          priv->tmucnt = 1;
          adc_stop(priv);
          adc_switchtmu(dev);

          /* Re-enable hw trig */

          adc_reg_startconv(priv);
      }
      else
#endif
      {
          priv->cb->au_receive_batch(dev, priv->pchlistsbuffer,
                                     (void *)priv->pdmabuffer,
                                     priv->nchannels * priv->thresh);
      }

      /* Restart the conversion sequence from the beginning */

      priv->current = 0;
      adc_clearendofconv(priv);
    }

  /* Identifies the interruption AWD, OVR or EOC */

  if (CHECK_INT_REG(priv, ADC_INT_STATUS_ACMP) == true)
    {
      adc_clearcompareflg(priv);
      awarn("WARNING: Analog Watchdog, Value converted out of range!\n");
    }

  if (CHECK_INT_REG(priv, ADC_INT_STATUS_OVR) == true)
    {
      adc_clearoverrun(priv);
      awarn("WARNING: Overrun has occurred!\n");
    }
}

/****************************************************************************
 * Name: adc_dma_cfg
 ****************************************************************************/

static void adc_dma_cfg(struct fc7300_dev_s *priv)
{
  uint32_t regtemp;

  regtemp = adc_getreg(priv, FC7300_ADC_CFG1_OFFSET);
  regtemp |= ADC_CFG1_DMAEN_MASK;

  /* Enable DMA */

  adc_putreg(priv, FC7300_ADC_CFG1_OFFSET, regtemp);
}

/****************************************************************************
 * Name: adc_dma_start
 ****************************************************************************/

static void adc_dma_start(struct adc_dev_s *dev)
{
  struct fc7300_dev_s *priv = (struct fc7300_dev_s *)dev->ad_priv;
  struct dma_dev_s *dma_dev  = fc7300_dma_getdev(DMA_INSTANCE_0);
  struct dma_config_s config;
  uint16_t thresh = priv->thresh;

  /* Stop and free DMA if it was started before */

  priv->dmastream = DMA_GET_CHAN(dma_dev, priv->dmachan);
  if (priv->dmastream == NULL)
    {
      return;
    }

  /* Configure for circular DMA reception into the RX FIFO */

  config.src_width = 4;                      /* 4Bytes */
  config.dst_width = 4;                      /* 4Bytes */
  config.src_drq   = priv->dma_reqsrc;
  config.src_step  = 0;
  config.dst_step  = 4;
  config.option    = DMA_CONFIG_LOOPDEST |
                     DMA_CONFIG_INTOUTER;

  DMA_CONFIG(priv->dmastream , &config);

  DMA_START_CYCLIC(priv->dmastream, adc_dmaconvcallback, (void *)dev,
                    (uintptr_t)priv->pdmabuffer,
                    (uintptr_t)(priv->base + FC7300_ADC_FIFO_DATA_OFFSET),
                    ((priv->nchannels * 4) * thresh), (priv->nchannels * 4));
}
#endif /* ADC_HAVE_DMA */

/****************************************************************************
 * Name: adc_reg_check
 *
 * Description:
 *   Check adc register config.
 *
 * Input Parameters:
 *
 *   priv  - A reference to the ADC block status
 *
 * Returned Value: void
 *
 ****************************************************************************/

static enum adc_checkstatustype_e adc_reg_check(struct fc7300_dev_s *priv)
{
  uint32_t tmpval;
  uint8_t hwnum = 0;

  switch (priv->base)
  {
    case FC7300_ADC0_BASE:
    {
        hwnum = 0;
    }
    break;

    case FC7300_ADC1_BASE:
    {
        hwnum = 1;
    }
    break;

    case FC7300_ADC2_BASE:
    {
        hwnum = 2;
    }
    break;

    case FC7300_ADC3_BASE:
    {
        hwnum = 3;
    }
    break;

    default:
        break;
  }

  switch (adc_checkstatus[hwnum])
    {
      case CHECK_CFG1:
        {
          tmpval = adc_getreg(priv, FC7300_ADC_CFG1_OFFSET);
          if (adc_cfg1_regval[hwnum] == tmpval)
            {
              adc_checkstatus[hwnum] = CHECK_CFG2;
              adc_cfg1_retry_times[hwnum] = priv->adcchk_errcnt;
              priv->adcchk_errcnt = 0;
            }
          else
            {
              priv->adcchk_errcnt ++;
              if (priv->adcchk_errcnt > 3)
                {
                  adc_cfg1_retry_times[hwnum] = priv->adcchk_errcnt;
                  adc_checkstatus[hwnum] = CHECK_FINISH;
                  break;
                }
              adc_checkstatus[hwnum] = RECONFIG_CFG1;
            }
        }
      break;

      case RECONFIG_CFG1:
        {
          adc_putreg(priv, FC7300_ADC_CFG1_OFFSET, adc_cfg1_regval[hwnum]);
          adc_checkstatus[hwnum] = CHECK_CFG1;
        }
      break;

      case CHECK_CFG2:
        {
          tmpval = adc_getreg(priv, FC7300_ADC_CFG2_OFFSET);
          if (adc_cfg2_regval[hwnum] == tmpval)
            {
              adc_checkstatus[hwnum] = CHECK_SMPR;
              adc_cfg2_retry_times[hwnum] = priv->adcchk_errcnt;
              priv->adcchk_errcnt = 0;
            }
          else
            {
              priv->adcchk_errcnt ++;
              if (priv->adcchk_errcnt > 3)
                {
                  adc_cfg2_retry_times[hwnum] = priv->adcchk_errcnt;
                  adc_checkstatus[hwnum] = CHECK_FINISH;
                  break;
                }
              adc_checkstatus[hwnum] = RECONFIG_CFG2;
            }
        }
      break;

      case RECONFIG_CFG2:
        {
          adc_putreg(priv, FC7300_ADC_CFG2_OFFSET, adc_cfg2_regval[hwnum]);
          adc_checkstatus[hwnum] = CHECK_CFG2;
        }
      break;

      case CHECK_SMPR:
        {
          tmpval = adc_getreg(priv, FC7300_ADC_SMPR_OFFSET);
          if (adc_smpr_regval[hwnum] == tmpval)
            {
              adc_smpr_retry_times[hwnum] = priv->adcchk_errcnt;
              adc_checkstatus[hwnum] = CHECK_FINISH;
              priv->adcchk_errcnt = 0;
            }
          else
            {
              priv->adcchk_errcnt ++;
              if (priv->adcchk_errcnt > 3)
                {
                  adc_smpr_retry_times[hwnum] = priv->adcchk_errcnt;
                  adc_checkstatus[hwnum] = CHECK_FINISH;
                  break;
                }
              adc_checkstatus[hwnum] = RECONFIG_SMPR;
            }
        }
      break;

      case RECONFIG_SMPR:
        {
          adc_putreg(priv, FC7300_ADC_SMPR_OFFSET, adc_smpr_regval[hwnum]);
          adc_checkstatus[hwnum] = CHECK_SMPR;
        }
      break;

      default:
          break;
    }

  return adc_checkstatus[hwnum];
}

/****************************************************************************
 * Name: adc_configure
 *
 * Description:
 *   Config all functions of ADCn.
 *
 * Input Parameters:
 *
 *   dev  - the selected device
 *
 * Returned Value: void
 *
 ****************************************************************************/

static void adc_configure(struct adc_dev_s *dev)
{
  struct fc7300_dev_s *priv = (struct fc7300_dev_s *)dev->ad_priv;

#if defined(CONFIG_FC7300_ADC_WDG)
  /* Initialize the ADC watchdog */

  adc_watchdog_cfg(priv);
#endif

  /* Initialize the ADC instance */

  adc_module_cfg(priv);

  /* Configuration of the channel conversions */

  if (priv->nchannels > 0)
    {
      adc_set_ch(dev);
    }

#ifdef ADC_HAVE_DMA
  /* Configure ADC DMA if enabled */

  if (priv->hasdma)
    {
      /* Configure ADC DMA */

      adc_dma_cfg(priv);

      /* Start ADC DMA */

      adc_dma_start(dev);
    }
#endif

  /* Check Adc reg configure value */

  while (adc_reg_check(priv) != CHECK_FINISH);

  /* Enable ADC */

  (void)adc_enable(priv, true);
}

/****************************************************************************
 * Name: adc_reset
 *
 * Description:
 *   Reset the ADC device.  Called early to initialize the hardware.
 *   This is called, before adc_setup() and on error conditions.
 *
 * Input Parameters:
 *
 * Returned Value: void
 *
 ****************************************************************************/

static void adc_reset(struct adc_dev_s *dev)
{
  struct fc7300_dev_s *priv = (struct fc7300_dev_s *)dev->ad_priv;
  irqstate_t flags;

  ainfo("intf: %d\n", priv->intf);
  flags = enter_critical_section();

  /* Do nothing if ADC instance is currently in use */

  if (priv->cmn->refcount > 0)
    {
      goto out;
    }

  /* Only if this is the first initialzied ADC instance in the ADC block */

  if (nxmutex_lock(&priv->cmn->lock) < 0)
    {
      goto out;
    }

  if (priv->cmn->refcount == 0)
    {
      /* Enable ADC reset state */

        adc_hwreset(priv);
    }

  nxmutex_unlock(&priv->cmn->lock);

out:
  leave_critical_section(flags);
}

static void adc_timerinit(struct fc7300_dev_s *priv)
{
  uint32_t timeouttick;
  uint32_t timeout = priv->timeout;
  uint16_t val;
  uint32_t timerclk;

  val = getreg32(FC7300_FCPIT0_MCR);
  if((val & FCPIT_MCR_M_CEN_MASK) != 0x1u)
    {
      val |= FCPIT_MCR_M_CEN_MASK;
      putreg32(val, FC7300_FCPIT0_MCR);
    }
  timerclk = fc7300_adc_timer_pckfreq();
  timeouttick = timeout * (timerclk / USEC_PER_SEC);
  timer_putreg(priv, FC7300_FCPIT_TVAL_OFFSET ,timeouttick);

  val = timer_getreg(priv, FC7300_FCPIT_TCTRL_OFFSET);
  val = ((val & ~(uint32_t)FCPIT_TCTRL_MODE_MASK) | FCPIT_TCTRL_T_EN_MASK| \
         FCPIT_TCTRL_MODE(FC7300_FCPIT_32PERIODIC_COUNTER));
  timer_putreg(priv, FC7300_FCPIT_TCTRL_OFFSET, val);
}

static void adc_timerdeinit(struct fc7300_dev_s *priv)
{
  uint32_t val;

  val = timer_getreg(priv, FC7300_FCPIT_TCTRL_OFFSET);
  val = (val & ~(uint32_t)FCPIT_TCTRL_T_EN_MASK);
  timer_putreg(priv, FC7300_FCPIT_TCTRL_OFFSET, val);
}

/****************************************************************************
 * Name: adc_setup
 *
 * Description:
 *   Configure the ADC. This method is called the first time that the ADC
 *   device is opened.  This will occur when the port is first opened.
 *   This setup includes configuring and attaching ADC interrupts.
 *   Interrupts are all disabled upon return.
 *
 * Input Parameters:
 *
 * Returned Value: setup status.
 *
 ****************************************************************************/

static int adc_setup(struct adc_dev_s *dev)
{
  struct fc7300_dev_s *priv = (struct fc7300_dev_s *)dev->ad_priv;
  int ret = OK;

  /* Do nothing when the ADC device is already set up */

  if (priv->cmn->refcount > 0)
    {
      return OK;
    }

  /* Make sure that the ADC device is in the powered up, reset state */

  adc_reset(dev);

  /* Configure ADC device */

  adc_configure(dev);

  /* As default conversion is started here */

  if (priv->sequencemode)
    {
      fc7300_trgsel0_settriggersource(priv->channellists[0].trgseldes,
                                      priv->channellists[0].trgselsrc);
      adc_timerinit(priv);
      adc_reg_startconv(priv);
    }

  /* Increase instances counter */

  ret = nxmutex_lock(&priv->cmn->lock);
  if (ret < 0)
    {
      return ret;
    }

  /* The ADC device is ready */

  priv->cmn->refcount += 1;
  nxmutex_unlock(&priv->cmn->lock);

  return ret;
}

/****************************************************************************
 * Name: adc_shutdown
 *
 * Description:
 *   Disable the ADC.  This method is called when the ADC device is closed.
 *   This method reverses the operation the setup method.
 *
 * Input Parameters:
 *
 * Returned Value: void
 *
 ****************************************************************************/

static void adc_shutdown(struct adc_dev_s *dev)
{
  struct fc7300_dev_s *priv = (struct fc7300_dev_s *)dev->ad_priv;
#ifdef ADC_HAVE_DMA
  struct dma_dev_s *dma_dev  = fc7300_dma_getdev(DMA_INSTANCE_0);
#endif

  if (nxmutex_lock(&priv->cmn->lock) < 0)
    {
      return;
    }

  /* Decrement count only when ADC device is in use */

  if (priv->cmn->refcount > 0)
    {
      priv->cmn->refcount -= 1;
    }

  /* Shutdown the ADC device only when not in use */

  if (priv->cmn->refcount > 0)
    {
      return;
    }

#if defined(CONFIG_FC7300_ADC0_IRQ) || defined(CONFIG_FC7300_ADC1_IRQ) || \
    defined(CONFIG_FC7300_ADC2_IRQ) || defined(CONFIG_FC7300_ADC3_IRQ)
  /* Disable ADC interrupts and detach the ADC interrupt handler */

  (void)adc_interrupt_cfg(dev, false);
#endif

  if (priv->base != 0)
    {
      /* Disable ADC */

      if (priv->sequencemode)
        {
          adc_timerdeinit(priv);
        }

      (void)adc_enable(priv, false);
    }

  if (priv->hasdma)
    {
#ifdef ADC_HAVE_DMA
      DMA_STOP(priv->dmastream);

      /* Release the RX DMA channel */

      DMA_PUT_CHAN(dma_dev, priv->dmastream);
#endif
    }
  else
    {
      up_disable_irq(priv->irq);
      irq_detach(priv->irq);
    }
  adc_checkstatus[priv->intf] = CHECK_CFG1;
  nxmutex_unlock(&priv->cmn->lock);
}

/****************************************************************************
 * Name: adc_rxint
 *
 * Description:
 *   Call to enable or disable RX interrupts.
 *
 * Input Parameters:
 *
 * Returned Value: void
 *
 ****************************************************************************/

static void adc_rxint(struct adc_dev_s *dev, bool enable)
{
#if defined(CONFIG_FC7300_ADC0_IRQ) || defined(CONFIG_FC7300_ADC1_IRQ) || \
    defined(CONFIG_FC7300_ADC2_IRQ) || defined(CONFIG_FC7300_ADC3_IRQ)

  struct fc7300_dev_s *priv = (struct fc7300_dev_s *)dev->ad_priv;

  ainfo("intf: %d enable: %d\n", priv->intf, enable ? 1 : 0);

  /* Enable the analog watchdog / overrun interrupts, and if no DMA,
   * end-of-conversion ADC.
   */

  (void)adc_interrupt_cfg(dev, enable);

#endif
}

/****************************************************************************
 * Name: adc_set_ch
 *
 * Description:
 *   Sets the ADC channel.
 *
 * Input Parameters:
 *   dev - pointer to device structure used by the driver
 *   ch  - ADC channel number + 1. 0 reserved for all configured channels
 *
 * Returned Value:
 *   int - errno
 *
 ****************************************************************************/

static int adc_set_ch(struct adc_dev_s *dev)
{
  struct fc7300_dev_s *priv = (struct fc7300_dev_s *)dev->ad_priv;
  uint32_t regtemp;
  uint8_t i;
  uint8_t ch;

  for (i = 0U; i < priv->nchannels; i++)
    {
      ch = priv->channellists[i].channel;

      /* Configure the ADC input pin */

      if (ch < 32U)
        {
          /* No configuration pins required for internal adc channels */

          fc7300_pinconfig(priv->channellists[i].pincfg);
        }

      /* Set channel sample time index */

      regtemp = adc_getreg(priv, FC7300_ADC_SC_OFFSET(i));
      regtemp = (regtemp & ~ADC_SC_SMPSEL_MASK) |
                ADC_SC_SMPSEL(priv->channellists[i].sampletime);
      adc_putreg(priv, FC7300_ADC_SC_OFFSET(i), regtemp);

      /* set channel interrupt disable */

      regtemp = adc_getreg(priv, FC7300_ADC_SC_OFFSET(i));
      regtemp = (regtemp & ~ADC_SC_AIEN_MASK) |
                ADC_SC_AIEN(false);
      adc_putreg(priv, FC7300_ADC_SC_OFFSET(i), regtemp);

      /* set channel input */

      regtemp = adc_getreg(priv, FC7300_ADC_SC_OFFSET(i));
      regtemp = (regtemp & ~ADC_SC_CHS_MASK) |
                ADC_SC_CHS(ch);
      adc_putreg(priv, FC7300_ADC_SC_OFFSET(i), regtemp);
    }

  /* Set sequence length */

  regtemp = adc_getreg(priv, FC7300_ADC_CFG1_OFFSET);
  regtemp = (regtemp & ~ADC_CFG1_SEQ_LEN_MASK) |
            ADC_CFG1_SEQ_LEN(priv->nchannels - 1);
  adc_putreg(priv, FC7300_ADC_CFG1_OFFSET, regtemp);

  /* Set FIFO water mark */

  regtemp = adc_getreg(priv, FC7300_ADC_CFG2_OFFSET);
  regtemp = (regtemp & ~ADC_CFG2_FWMARK_MASK) |
            ADC_CFG2_FWMARK(priv->nchannels - 1);
  adc_putreg(priv, FC7300_ADC_CFG2_OFFSET, regtemp);

  return OK;
}

#if defined(CONFIG_FC7300_TMU)
static void adc_switchtmu(struct adc_dev_s *dev)
{
  struct fc7300_dev_s *priv = (struct fc7300_dev_s *)dev->ad_priv;
  volatile uint32_t cfgreg;
  volatile int32_t  data;
  volatile uint32_t timeout = ADC_WAITTMUCONVERT_TIMEOUT;

  /* Start conversion if channels are configured */

  if (priv->nchannels > 0)
    {
      if (priv->base == FC7300_ADC0_BASE)
        {
          irqstate_t flags = enter_critical_section();
          if (priv->tmucnt == 1)
            {
              if (priv->hasdma)
                {
                  cfgreg = ADC_CFG1_OVRMOD(priv->overrunmode) |
                            ADC_CFG1_SEQ_LEN(0) |
                            ADC_CFG1_SEQ_MOD(0) |
                            ADC_CFG1_WAIT(priv->waitenable) |
                            ADC_CFG1_DIFF(true) |
                            ADC_CFG1_TRIGSRC(ADC_TRIGSRC_TRGSEL) |
                            ADC_CFG1_TRIGMODE(ADC_TRIGMODE_SW) |
                            ADC_CFG1_ALIGN(ADC_ALIGN_RIGHT) |
                            ADC_CFG1_RES(priv->resolution) |
                            ADC_CFG1_DMAEN(false);
                  adc_putreg(priv, FC7300_ADC_CFG1_OFFSET, cfgreg);

                  /* Start convert */

                  adc_reg_startconv(priv);
                  while ((CHECK_INT_REG(priv, ADC_INT_STATUS_EOSEQ) != true)
                        && (timeout != 0))
                    {
                      timeout--;
                    }

                  /* EOC: End of conversion */

                  adc_clearendofseq(priv);
                  data = adc_getfifodata(priv);
                  adc_tmuvalue = (uint16_t)data;
                  priv->tmucnt = 0;
                  adc_stop(priv);
                  cfgreg = adc_getreg(priv, FC7300_ADC_CFG1_OFFSET);
                  cfgreg = (cfgreg & ~ADC_CFG1_DIFF_MASK) | ADC_CFG1_DIFF(false);
                  cfgreg = (cfgreg & ~ADC_CFG1_SEQ_LEN_MASK) | ADC_CFG1_SEQ_LEN(priv->nchannels - 1);
                  cfgreg |= (ADC_CFG1_DMAEN_MASK | ADC_CFG1_TRIGMODE(ADC_TRIGMODE_RISING_EDGE));
                  adc_putreg(priv, FC7300_ADC_CFG1_OFFSET, cfgreg);
                  leave_critical_section(flags);
                  return;
                }
              else
                {
                  adc_stop(priv);
                  cfgreg = ADC_CFG1_OVRMOD(priv->overrunmode) |
                            ADC_CFG1_SEQ_LEN(0) |
                            ADC_CFG1_SEQ_MOD(0) |
                            ADC_CFG1_WAIT(priv->waitenable) |
                            ADC_CFG1_DIFF(true) |
                            ADC_CFG1_TRIGSRC(ADC_TRIGSRC_TRGSEL) |
                            ADC_CFG1_TRIGMODE(ADC_TRIGMODE_SW) |
                            ADC_CFG1_ALIGN(ADC_ALIGN_RIGHT) |
                            ADC_CFG1_RES(priv->resolution) |
                            ADC_CFG1_DMAEN(false);
                  adc_putreg(priv, FC7300_ADC_CFG1_OFFSET, cfgreg);
                }
            }
          else if (priv->tmucnt == 0)
            {
              adc_stop(priv);
              cfgreg = ADC_CFG1_OVRMOD(priv->overrunmode) |
                        ADC_CFG1_SEQ_LEN(priv->nchannels - 1) |
                        ADC_CFG1_SEQ_MOD(0) |
                        ADC_CFG1_WAIT(priv->waitenable) |
                        ADC_CFG1_DIFF(false) |
                        ADC_CFG1_TRIGSRC(ADC_TRIGSRC_TRGSEL) |
                        ADC_CFG1_TRIGMODE(ADC_TRIGMODE_SW) |
                        ADC_CFG1_ALIGN(ADC_ALIGN_RIGHT) |
                        ADC_CFG1_RES(priv->resolution) |
                        ADC_CFG1_DMAEN(false);
              adc_putreg(priv, FC7300_ADC_CFG1_OFFSET, cfgreg);
            }
          else
            {
              /* nothing todo */
            }
          leave_critical_section(flags);
        }
      adc_reg_startconv(priv);
    }
}
#endif

/****************************************************************************
 * Name: adc_ioctl
 *
 * Description:
 *   All ioctl calls will be routed through this method.
 *
 * Input Parameters:
 *   dev - pointer to device structure used by the driver
 *   cmd - command
 *   arg - arguments passed with command
 *
 * Returned Value:
 *
 ****************************************************************************/

static int adc_ioctl(struct adc_dev_s *dev, int cmd, unsigned long arg)
{
  struct fc7300_dev_s *priv = (struct fc7300_dev_s *)dev->ad_priv;
  int ret = OK;
#if defined(CONFIG_FC7300_ADC_WDG)
  uint32_t regtemp;
  uint32_t tmp;
#endif
  switch (cmd)
    {
      case ANIOC_TRIGGER:
        {
          /* Start conversion if channels are configured */

          if (priv->nchannels > 0)
            {
              adc_reg_startconv(priv);
            }

          break;
        }

      case ANIOC_GET_NCHANNELS:
        {
          /* Return the number of configured channels */

          ret = priv->nchannels;
        }
        break;

      case ANIOC_WDOG_UPPER: /* Set watchdog upper threshold */
        {
#if defined(CONFIG_FC7300_ADC_WDG)
          adc_wdog_enable(priv, false);
          regtemp = adc_getreg(priv, FC7300_ADC_CMP_TR_OFFSET);

          /* Verify new upper threshold greater than lower threshold */

          tmp = (regtemp & ADC_CMP_TR_LT_MASK) >> ADC_CMP_TR_LT_SHIFT;
          if ((arg < tmp) || (arg > 4095))
            {
              ret = -EINVAL;
              break;
            }

          /* Set the watchdog threshold register */

          regtemp =  (regtemp & ~ADC_CMP_TR_HT_MASK) | ADC_CMP_TR_HT(arg);
          adc_putreg(priv, FC7300_ADC_CMP_TR_OFFSET, regtemp);

          /* Ensure analog watchdog is enabled */

          adc_wdog_enable(priv, true);
#endif
        }
        break;

      case ANIOC_WDOG_LOWER: /* Set watchdog lower threshold */
        {
#if defined(CONFIG_FC7300_ADC_WDG)
          adc_wdog_enable(priv, false);
          regtemp = adc_getreg(priv, FC7300_ADC_CMP_TR_OFFSET);

          /* Verify new lower threshold less than upper threshold */

          tmp = (regtemp & ADC_CMP_TR_HT_MASK) >> ADC_CMP_TR_HT_SHIFT;
          if (arg > tmp)
            {
              ret = -EINVAL;
              break;
            }

          /* Set the watchdog threshold register */

          regtemp =  (regtemp & ~ADC_CMP_TR_LT_MASK) | ADC_CMP_TR_LT(arg);
          adc_putreg(priv, FC7300_ADC_CMP_TR_OFFSET, regtemp);

          /* Ensure analog watchdog is enabled */

          adc_wdog_enable(priv, true);
#endif
        }
        break;

      default:
        {
          aerr("ERROR: Unknown cmd: %d\n", cmd);
          ret = -ENOTTY;
          break;
        }
    }

  return ret;
}

#if defined(CONFIG_FC7300_ADC0_IRQ) || defined(CONFIG_FC7300_ADC1_IRQ) || \
    defined(CONFIG_FC7300_ADC2_IRQ) || defined(CONFIG_FC7300_ADC3_IRQ)

/****************************************************************************
 * Name: adc_getdata
 *
 * Description:
 *   Get adc data and channel without fifo.
 *
 * Input Parameters:
 *   priv - device structure
 *   data - adc data
 *
 * Returned Value: void.
 *
 ****************************************************************************/
#if defined(CONFIG_FC7300_ADC_USE_NO_FIFO)
static void adc_getdata(struct fc7300_dev_s *priv, uint32_t data)
{
  if (!(priv->fifo.fifochbuffer) || !(priv->fifo.fifodatabuffer))
    {
      return;
    }

    priv->fifo.fifodatabuffer[priv->current] = data;
    priv->fifo.fifochbuffer[priv->current]   = priv->channellists[priv->current].channel;

  if (priv->current == priv->nchannels -1)
    {
      priv->adc_convert_flag = true;
    }

  return;
}
#endif
/****************************************************************************
 * Name: adc_interrupt
 *
 * Description:
 *   Common ADC interrupt handler.
 *
 * Input Parameters:
 *
 * Returned Value: always return OK.
 *
 ****************************************************************************/

__ramfunc__ static int adc_interrupt(struct adc_dev_s *dev)
{
  struct fc7300_dev_s *priv = (struct fc7300_dev_s *)dev->ad_priv;
  volatile int32_t  data;

  if ((CHECK_INT_REG(priv, ADC_INT_STATUS_EOSEQ) == true))
    {
      /* EOC: End of conversion */

      adc_clearendofseq(priv);

      priv->adc_convert_flag = false;

#if defined(CONFIG_FC7300_TMU)
      if (priv->base == FC7300_ADC0_BASE)
        {
          if (priv->tmucnt == 0)
            {

              for (priv->current = 0; priv->current < priv->nchannels; priv->current++)
                {
                  /* Read the converted value and clear EOC bit
                  * (It is cleared by reading the ADC_DR)
                  */

                  data = adc_getfifodata(priv);

        #if defined(CONFIG_FC7300_ADC_USE_NO_FIFO)
                      adc_getdata(priv, data);
        #else
                  /* Verify that the upper-half driver has bound its callback */

                  if (priv->cb != NULL)
                    {
                      /* Give the ADC data to the ADC driver.  The ADC receive()
                      * accepts 3 parameters:
                      *
                      * 1) The first is the ADC device instance for this ADC block.
                      * 2) The second is the channel number for the data, and
                      * 3) The third is the converted data for the channel.
                      */

                      DEBUGASSERT(priv->cb->au_receive != NULL);
                      priv->cb->au_receive(dev,
                                    priv->channellists[priv->current].channel, data);
                    }
        #endif
                }

                priv->tmucnt = 1;
                adc_switchtmu(dev);
            }
          else
            {
              data = adc_getfifodata(priv);
              adc_tmuvalue = (uint16_t)data;
              priv->tmucnt = 0;
            }
        }
      else
#endif /* defined(CONFIG_FC7300_TMU) */
        {
          for (priv->current = 0; priv->current < priv->nchannels; priv->current++)
            {
              /* Read the converted value and clear EOC bit
              * (It is cleared by reading the ADC_DR)
              */

              data = adc_getfifodata(priv);

#if defined(CONFIG_FC7300_ADC_USE_NO_FIFO)
                  adc_getdata(priv, data);
#else
              /* Verify that the upper-half driver has bound its callback */

              if (priv->cb != NULL)
                {
                  /* Give the ADC data to the ADC driver.  The ADC receive()
                  * accepts 3 parameters:
                  *
                  * 1) The first is the ADC device instance for this ADC block.
                  * 2) The second is the channel number for the data, and
                  * 3) The third is the converted data for the channel.
                  */

                  DEBUGASSERT(priv->cb->au_receive != NULL);
                  priv->cb->au_receive(dev,
                                priv->channellists[priv->current].channel, data);
                }
#endif
            }
        }

      /* Restart the conversion sequence from the beginning */

      priv->current = 0;
      adc_clearendofconv(priv);
    }

  /* Identifies the interruption AWD, OVR or EOC */

  if (CHECK_INT_REG(priv, ADC_INT_STATUS_ACMP) == true)
    {
      adc_clearcompareflg(priv);
      awarn("WARNING: Analog Watchdog, Value converted out of range!\n");
    }

  if (CHECK_INT_REG(priv, ADC_INT_STATUS_OVR) == true)
    {
      adc_clearoverrun(priv);
      awarn("WARNING: Overrun has occurred!\n");
    }

  return OK;
}
#endif /* CONFIG_FC7300_ADCx_IRQ */

#if defined(CONFIG_FC7300_ADC0_IRQ) || defined(CONFIG_FC7300_ADC1_IRQ) || \
    defined(CONFIG_FC7300_ADC2_IRQ) || defined(CONFIG_FC7300_ADC3_IRQ)
/****************************************************************************
 * Name: fc7300_adc_interrupt_handle
 *
 * Description:
 *   ADCx interrupt handler
 *
 * Input Parameters:
 *
 * Returned Value:
 *
 ****************************************************************************/

static int fc7300_adc_interrupt_handle(int irq, void *context, void *arg)
{
  UNUSED(irq);
  UNUSED(context);

  adc_interrupt(arg);
#ifdef CONFIG_FC7300_ADC_IRQ_ATTACH_THREAD
  fc7300_clearpendingisr(irq);
  up_enable_irq(irq);
#endif
  return OK;
}
#endif /* CONFIG_FC7300_ADCx_IRQ */

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: fc7300_adc_initialize
 *
 * Description:
 *   Initialize the ADC.
 *
 *   The logic allow initialize ADC channels.
 *
 *   The number of channels for given ADC will be calculated automatically,
 *   and instance attributes config are from Kconfig, channel attributes are
 *   from boards.
 *
 * Input Parameters:
 *   intf      - Could be {0,1,2,3} for ADC0, ADC1, ADC2, ADC3
 *   chanlist  - The list of channels
 *   nchannels  - Number of channels
 *
 * Returned Value:
 *   Valid ADC device structure reference on success; a NULL on failure
 *
 ****************************************************************************/

struct adc_dev_s *fc7300_adc_initialize(enum adc_instancetype_e instance,
                                        const struct adcchan_s *chanlist,
                                        int nchannels)
{
  struct adc_dev_s   *dev;
  struct fc7300_dev_s *priv;
  uint8_t i;
#ifdef ADC_HAVE_DMA
  uint8_t j;
#endif

  DEBUGASSERT(nchannels > 0);

  switch (instance)
    {
#ifdef CONFIG_FC7300_ADC0
      case ADC_INSTANCE_0:
        {
          ainfo("ADC0 selected\n");
          dev = &g_adcdev0;
          break;
        }
#endif /* CONFIG_FC7300_ADC0 */

#ifdef CONFIG_FC7300_ADC1
      case ADC_INSTANCE_1:
        {
          ainfo("ADC1 selected\n");
          dev = &g_adcdev1;
          break;
        }
#endif /* CONFIG_FC7300_ADC1 */

#ifdef CONFIG_FC7300_ADC2
      case ADC_INSTANCE_2:
        {
          ainfo("ADC2 selected\n");
          dev = &g_adcdev2;
          break;
        }
#endif /* CONFIG_FC7300_ADC2 */

#ifdef CONFIG_FC7300_ADC3
      case ADC_INSTANCE_3:
        {
          ainfo("ADC3 selected\n");
          dev = &g_adcdev3;
          break;
        }
#endif /* CONFIG_FC7300_ADC3 */

      default:
        {
          aerr("ERROR: No ADC interface defined\n");
          return NULL;
        }
    }

  priv = (struct fc7300_dev_s *)dev->ad_priv;
  priv->nchannels = nchannels;
  dev->ad_recv.af_data = priv->fifo.fifodatabuffer;
  dev->ad_recv.af_channel = priv->fifo.fifochbuffer;
  dev->ad_recv.af_fifosize = priv->fifo.fifosize;

  for (i = 0; i < nchannels; i++)
    {
      priv->channellists[i].channel = chanlist[i].channel;
      priv->channellists[i].sampletime = chanlist[i].sampletime;
      priv->channellists[i].pincfg = chanlist[i].pincfg;
      priv->channellists[i].trgselsrc = chanlist[i].trgselsrc;
      priv->channellists[i].trgseldes = chanlist[i].trgseldes;
#ifdef ADC_HAVE_DMA
      if (priv->hasdma)
        {
          for (j = 0; j < priv->thresh; j++)
            {
              priv->pchlistsbuffer[i + (j * nchannels)] = (uint8_t)chanlist[i].channel;
            }
        }
#endif
    }

#if defined(CONFIG_FC7300_TMU)
  /* Get temperature fix parameters */

  adc_toffset = ((ADC_TOFFSET >> 16) & 0x3F);
  adc_tslope = ((ADC_TSLOPE >> 16) & 0x3F);
#endif

  ainfo("intf: %d num_channels: %d\n", instance, priv->nchannels);

  return dev;
}

#endif /* CONFIG_FC7300_ADC */
