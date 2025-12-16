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

#ifndef __ARCH_ARM_SRC_FC7300_FC7300_ADC_H
#define __ARCH_ARM_SRC_FC7300_FC7300_ADC_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>
#include <nuttx/analog/adc.h>
#include "chip.h"
#include "hardware/fc7300_adc.h"
#include "hardware/fc7300_trgsel.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Generalized definitions for ADC  *****************************************/

/* One ADC instance has 4 sample time options */

#define ADC_SAMPLE_TIME_OPTION_CNT 4U

/* Max sequence length */

#define ADC_MAX_SAMPLES            32U

/* ADC default configuration parameters */

#define ADC_DEFAULT_SAMPLE_TIME_OPTION_0                  (0x04U)
#define ADC_DEFAULT_SAMPLE_TIME_OPTION_1                  (0x0AU)
#define ADC_DEFAULT_SAMPLE_TIME_OPTION_2                  (0x22U)
#define ADC_DEFAULT_SAMPLE_TIME_OPTION_3                  (0x50U)
#define ADC_DEFAULT_STARTUP_COUNTER                       (0xC0U)
#define ADC_DEFAULT_WATER_MARK                            (0x10U)
#define ADC_DEFAULT_COMPARE_HIGH_THRESHOLD                (0xFFFU)
#define ADC_DEFAULT_COMPARE_LOW_THRESHOLD                 (0x000U)
#define ADC_DEFAULT_SC_CHANNEL                            (0x3FU)

/* ADC temperature registers */

#define ADC_TOFFSET                          *(uint32_t *)(0x400235FCU)
#define ADC_TSLOPE                           *(uint32_t *)(0x400236FCU)

#define ADC_WAITTMUCONVERT_TIMEOUT                         100000

/* Configuration ************************************************************/

/* Up to 2 ADC interfaces are supported */

#if defined(CONFIG_FC7300_ADC0) || defined(CONFIG_FC7300_ADC1) || \
    defined(CONFIG_FC7300_ADC2) || defined(CONFIG_FC7300_ADC3)

/* DMA support */

#undef ADC_HAVE_DMA
#if defined(CONFIG_FC7300_ADC0_DMA) || defined(CONFIG_FC7300_ADC1_DMA) || \
    defined(CONFIG_FC7300_ADC2_DMA) || defined(CONFIG_FC7300_ADC3_DMA)
#  define ADC_HAVE_DMA  1
#endif

#ifdef CONFIG_FC7300_ADC0_DMA
#  define ADC0_HAVE_DMA 1
#else
#  undef  ADC0_HAVE_DMA
#endif

#ifdef CONFIG_FC7300_ADC1_DMA
#  define ADC1_HAVE_DMA 1
#else
#  undef  ADC1_HAVE_DMA
#endif

#ifdef CONFIG_FC7300_ADC2_DMA
#  define ADC2_HAVE_DMA 1
#else
#  undef  ADC2_HAVE_DMA
#endif

#ifdef CONFIG_FC7300_ADC3_DMA
#  define ADC3_HAVE_DMA 1
#else
#  undef  ADC3_HAVE_DMA
#endif

/* EXTSEL configuration *****************************************************/

/* NOTE: Continuous mode not supported */

#endif /* CONFIG_FC7300_ADCx */

/****************************************************************************
 * Public Types
 ****************************************************************************/

/* The instance index of the ADC peripheral */

enum adc_instancetype_e
{
  ADC_INSTANCE_0 = 0U,  /* ADC instance 0 is selected */
  ADC_INSTANCE_1 = 1U,  /* ADC instance 1 is selected */
  ADC_INSTANCE_2 = 2U,  /* ADC instance 2 is selected */
  ADC_INSTANCE_3 = 3U   /* ADC instance 3 is selected */
};

/* The channel selected for ADC conversion */

enum adc_channeltype_e
{
  ADC_CHANNEL_0          = 0U,
  ADC_CHANNEL_1          = 1U,
  ADC_CHANNEL_2          = 2U,
  ADC_CHANNEL_3          = 3U,
  ADC_CHANNEL_4          = 4U,
  ADC_CHANNEL_5          = 5U,
  ADC_CHANNEL_6          = 6U,
  ADC_CHANNEL_7          = 7U,
  ADC_CHANNEL_8          = 8U,
  ADC_CHANNEL_9          = 9U,
  ADC_CHANNEL_10         = 10U,
  ADC_CHANNEL_11         = 11U,
  ADC_CHANNEL_12         = 12U,
  ADC_CHANNEL_13         = 13U,
  ADC_CHANNEL_14         = 14U,
  ADC_CHANNEL_15         = 15U,
  ADC_CHANNEL_16         = 16U,
  ADC_CHANNEL_17         = 17U,
  ADC_CHANNEL_18         = 18U,
  ADC_CHANNEL_19         = 19U,
  ADC_CHANNEL_20         = 20U,
  ADC_CHANNEL_21         = 21U,
  ADC_CHANNEL_22         = 22U,
  ADC_CHANNEL_23         = 23U,
  ADC_CHANNEL_24         = 24U,
  ADC_CHANNEL_25         = 25U,
  ADC_CHANNEL_26         = 26U,
  ADC_CHANNEL_27         = 27U,
  ADC_CHANNEL_28         = 28U,
  ADC_CHANNEL_29         = 29U,
  ADC_CHANNEL_30         = 30U,
  ADC_CHANNEL_31         = 31U,
  ADC_CHANNEL_INTERNAL_0 = 32U,
  ADC_CHANNEL_INTERNAL_1 = 33U,
  ADC_CHANNEL_INTERNAL_2 = 34U,
  ADC_CHANNEL_INTERNAL_4 = 36U
};

/* The channel selected for ADC conversion in differential mode */

enum adc_differentialchanneltype_e
{
  ADC_CHANNEL_0_4        = 0U,
  ADC_CHANNEL_1_5        = 1U,
  ADC_CHANNEL_2_6        = 2U,
  ADC_CHANNEL_3_7        = 3U,
  ADC_CHANNEL_INTERNAL_5 = 37U
};

/* The ADC sample time option for selection */

enum adc_sampletimeoptiontype_e
{
  ADC_SAMPLE_TIME_OPTION_0 = 0U,
  ADC_SAMPLE_TIME_OPTION_1 = 1U,
  ADC_SAMPLE_TIME_OPTION_2 = 2U,
  ADC_SAMPLE_TIME_OPTION_3 = 3U
};

/* adc operation return values */

enum adc_statustype_e
{
  ADC_STATUS_SUCCESS = 0U,  /* The ADC operation is succeed */
  ADC_STATUS_ERROR   = 1U,  /* The ADC operation is failed */
  ADC_STATUS_TIMEOUT = 2U   /* The ADC operation is failed because of time out */
};

/* Set the ADC clock divider,
 * ADC clock divider is not available in FC7300F512K
 */

enum adc_clockdivide_e
{
  ADC_CLOCK_DIV_1 = 0U,
  ADC_CLOCK_DIV_2 = 1U,
  ADC_CLOCK_DIV_4 = 2U,
  ADC_CLOCK_DIV_8 = 3U
};

/* Select the ADC Resolution */

enum adc_resolutiontype_e
{
  ADC_RESOLUTION_12_BIT = 0U,  /* 12 bit resolution */
  ADC_RESOLUTION_10_BIT = 1U,  /* 10 bit resolution */
  ADC_RESOLUTION_8_BIT  = 2U   /* 8 bit resolution */
};

/* Select the ADC result alignment */

enum adc_aligntype_e
{
  ADC_ALIGN_RIGHT = 0U,   /* ADC result is aligned in right */
  ADC_ALIGN_LEFT  = 1U    /* ADC result is aligned in left */
};

/* Select the ADC trigger mode
 * This option is only valid in single sequence mode and continuous mode.
 */

enum adc_trigmodetype_e
{
  ADC_TRIGMODE_SW           = 0U,  /* ADC is triggered by software */
  ADC_TRIGMODE_RISING_EDGE  = 1U,  /* ADC is triggered by hardware trigger on rising edge */
  ADC_TRIGMODE_FALLING_EDGE = 2U,  /* ADC is triggered by hardware trigger on falling edge */
  ADC_TRIGMODE_BOTH_EDGE    = 3U,  /* ADC is triggered by hardware trigger on both edges */
  ADC_TRIGMODE_HIGH_VOLTAGE = 4U,  /* ADC is triggered when hardware trigger is high voltage */
  ADC_TRIGMODE_LOW_VOLTAGE  = 5U   /* ADC is triggered when hardware trigger is low voltage */
};

/* Select the ADC sequence mode */

enum adc_seqmodetype_e
{
  ADC_SEQMODE_SINGLE          = 0U,   /* ADC single sequence mode */
  ADC_SEQMODE_CONTINUOUS      = 1U,   /* ADC continuous mode */
  ADC_SEQMODE_DISCONTINUOUS_0 = 2U,   /* ADC discontinuous 0 mode */
  ADC_SEQMODE_DISCONTINUOUS_1 = 3U    /* ADC discontinuous 1 mode */
};

/* Select the ADC overrun management mode
 * To select whether the old data are preserved or overwritten by the new
 * data when ADC is overrun (FIFO is full when new convertion result comes)
 */

enum adc_ovrmodetype_e
{
  ADC_OVERRUN_MODE_PRESERVE  = 0U, /* old data are preserved when ADC is overrun */
  ADC_OVERRUN_MODE_OVERWRITE = 1U  /* old data are overwritten when ADC is overrun */
};

/* Select the ADC voltage reference source */

enum adc_reftype_e
{
  ADC_REF_INTERNAL = 0U,   /* select the internal voltage reference */
  ADC_REF_EXTERNAL = 1U    /* select the external voltage reference */
};

/* Select the ADC hardware average samples */

enum adc_averagetype_e
{
  ADC_AVERAGE_4  = 0U,  /* result average by 4 samples */
  ADC_AVERAGE_8  = 1U,  /* result average by 8 samples */
  ADC_AVERAGE_16 = 2U,  /* result average by 16 samples */
  ADC_AVERAGE_32 = 3U   /* result average by 32 samples */
};

/* The trigger source of the ADC instance
 * In ADC discontinuous 1 mode, the trigger source is from SMISC
 * In ADC single and continuous mode, if hardware trigger is enabled,
 * the trigger source is from TRGSEL
 */

enum adc_trigsrctype_e
{
  ADC_TRIGSRC_SMISC  = 0U,       /* Trigger source from PTIMER/TRGSEL with SMISC */
  ADC_TRIGSRC_TRGSEL = 2U        /* Trigger source from TRGSEL */
};

/* Select the ADC pretrigger source for discontinuous mode
 * This option is only valid when ADC sequence mode is discontinuous.
 * In ADC single and continuous modes, the pretrigger is not needed
 */

enum adc_pretriggerseltype_e
{
  ADC_PRETRIGGER_SEL_PTIMER     = 0U,   /* PTIMER pretrigger selected. */
  ADC_PRETRIGGER_SEL_TRGSEL     = 1U,   /* TRGSEL pretrigger selected. */
  ADC_PRETRIGGER_SEL_SW         = 2U    /* Software pretrigger selected. */
};

/* Select the ADC trigger source for discontinuous mode
 * This option is only valid when ADC sequence mode is discontinuous.
 * In ADC single and continuous modes, if the trigger mode is not software,
 * then the hardware trigger source is always from TRGSEL
 */

enum adc_triggerseltype_e
{
  ADC_TRIGGER_SEL_PTIMER        = 0U,   /* PTIMER trigger selected. */
  ADC_TRIGGER_SEL_TRGSEL        = 1U    /* TRGSEL trigger selected. */
};

/* Select the software pretrigger source
 * This option is only valid when software pretrigger is selected in ADC
 * discontinuous mode.
 */

enum adc_swpretriggertype_e
{
  ADC_SW_PRETRIGGER_DISABLED = 0U,   /* SW pretrigger disabled. */
  ADC_SW_PRETRIGGER_0        = 4U,   /* SW pretrigger 0. */
  ADC_SW_PRETRIGGER_1        = 5U,   /* SW pretrigger 1. */
  ADC_SW_PRETRIGGER_2        = 6U,   /* SW pretrigger 2. */
  ADC_SW_PRETRIGGER_3        = 7U    /* SW pretrigger 3. */
};

/* Select the channel compare mode
 * Select whether the channel compare is enabled on all channels or on the
 * single selected channel
 */

enum adc_cmpchanneltype_e
{
  ADC_CMP_CHANNEL_ALL    = 0U,  /* Compare enabled in all channels */
  ADC_CMP_CHANNEL_SINGLE = 1U   /* Compare enabled in the specified channel */
};

enum adc_checkstatustype_e
{
  CHECK_CFG1    = 0U,
  CHECK_CFG2    = 1U,
  CHECK_SMPR    = 2U,
  RECONFIG_CFG1 = 3U,
  RECONFIG_CFG2 = 4U,
  RECONFIG_SMPR = 5U,
  CHECK_FINISH  = 6U
};

#ifdef ADC_HAVE_DMA

/* Defines the DMA configuretion
 * This structure is used to configure the DMA transfer feature for ADC
 */

struct adc_dmatype_s
{
  bool dmaenable;             /* Enable DMA for the ADC */
  uint8_t dmachannel;         /* DMA channel used to transfer the ADC conversion results */
  uint8_t channelpriority;    /* DMA channel priority, higher value means higher priority. */
                              /* The priority for different channels must be unique */
                              /* Default priority value is same the channel number */
  uint32_t *resultbuffer;     /* Buffer to store the ADC conversion results */
  bool dmaintenable;          /* DMA interrupt enable */
  void* pConvCompleteNotify;  /* DMA transfer complete callback */
};
#endif

/* Adc channel configuration */

struct adcchan_s
{
  enum adc_channeltype_e           channel;     /* Adc channel  */
  enum adc_sampletimeoptiontype_e  sampletime;  /* Sample time selection */
  uint32_t                         pincfg;      /* Input pin configuration */
  uint8_t                          trgselsrc;
  uint8_t                          trgseldes;
};

/****************************************************************************
 * Private Types
 ****************************************************************************/

/* Data common to all ADC instances */

struct adccmn_data_s
{
  uint8_t refcount; /* Representing the usage status of ADC instances */
  mutex_t lock;     /* Exclusive access to common ADC data */
};

/* Adc fifo_Data for data、channel and size */

struct adc_fifo_data
{
  uint32_t *fifodatabuffer;                       /* Fifo data transfer buffer */
  uint8_t  *fifochbuffer;                         /* Fifo channel transfer buffer */
  uint16_t  fifosize;                             /* Fifo size */
};

/* This structure describes the state of one ADC block */

struct fc7300_dev_s
{
  const struct adc_callback_s *cb;                /* callbacks to notify the upper driver */
  uint8_t irq;                                    /* Interrupt generated by this ADC block */
  struct adccmn_data_s *cmn;                      /* Common ADC data */
  enum adc_clockdivide_e clockdivider;            /* Adc clock divider */
  enum adc_ovrmodetype_e overrunmode;             /* Whether to preserve data when ADC overruns */
  enum adc_seqmodetype_e sequencemode;
  bool waitenable;                                /* Whether to enable ADC wait conversion mode */
  uint8_t nchannels;                              /* Number of used channels */
  struct adcchan_s channellists[ADC_MAX_SAMPLES]; /* ADC Module channel: 0,1,2,3 */
  uint8_t intf;                                   /* ADC interface number */
  uint8_t current;                                /* Current ADC channel being converted */
  enum adc_resolutiontype_e resolution;           /* ADC resolution (8bit-10bit-12bit) */
#ifdef ADC_HAVE_DMA
  uint8_t dmachan;                                /* DMA channel needed by this ADC */
  uint8_t dma_reqsrc;                             /* DMAMUX source of RX DMA request */
  struct dma_chan_s *dmastream;                   /* currently-open trasnmit DMA stream */
  volatile uint32_t *pdmabuffer;                  /* DMA transfer buffer */
  uint8_t *pchlistsbuffer;                        /* DMA channelslist transfer buffer */
  uint16_t thresh;
#endif
  bool hasdma;
#ifdef CONFIG_FC7300_ADC_WDG
  uint8_t wdgchnsel;                              /* Compare channel selection */
  uint16_t highthres;                             /* Compare high threshold */
  uint16_t lowthres;                              /* Compare low threshold */
#endif
  uint8_t sample_time[ADC_SAMPLE_TIME_OPTION_CNT];
  xcpt_t isr;                                     /* Interrupt handler for this ADC block */
  bool adcwdginten;                               /* Enable interrupt of adc watchdog */
  bool overruninten;                              /* Enable interrupt when overrun occured */
  bool convcompleteinten;                         /* Enable interrupt when conversion completed */
  uint32_t base;                                  /* Base address of registers unique to this ADC block */
  uint32_t tbase;                                 /* Timer register base addr */
  uint32_t timeout;                               /* Timer trigger frequency */
  uint8_t refsel;                                 /* Reference voltage select */
  struct adc_fifo_data fifo;                      /* Adc fifo */
  bool adc_convert_flag;                          /* Flag for adc trans complete */
  uint32_t startupcnt;
  uint8_t tmucnt;
  uint8_t adcchk_errcnt;
};

extern uint32_t adc_cfg1_regval[4u];
extern uint32_t adc_cfg2_regval[4u];
extern uint32_t adc_smpr_regval[4u];
extern uint8_t  adc_cfg1_retry_times[4u];
extern uint8_t  adc_cfg2_retry_times[4u];
extern uint8_t  adc_smpr_retry_times[4u];
#if defined(CONFIG_FC7300_TMU)
extern uint8_t  adc_toffset;
extern uint8_t  adc_tslope;
extern volatile uint16_t adc_tmuvalue;
#endif

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

#ifndef __ASSEMBLY__
#ifdef __cplusplus
#define EXTERN extern "C"
extern "C"
{
#else
#define EXTERN extern
#endif

/****************************************************************************
 * Name: fc7300_adc_initialize
 *
 * Description:
 *   Initialize the ADC. See fc7300_adc.c for more details.
 *
 * Input Parameters:
 *   instance  - Could be {0,1,2,3} for ADC0, ADC1, ADC2, ADC3
 *   chanlist  - The list of channels (regular + injected)
 *   nchannels - Number of channels (regular + injected)
 *
 * Returned Value:
 *   Valid ADC device structure reference on success; a NULL on failure
 *
 ****************************************************************************/

struct adc_dev_s;
struct adc_dev_s *fc7300_adc_initialize(enum adc_instancetype_e instance,
                                        const struct adcchan_s *chanlist,
                                        int nchannels);

/****************************************************************************
 * Name: fc7300_adc_setup
 *
 * Description:
 *   Initialize the ADC. See fc7300_adc.c for more details.
 *
 * Input Parameters:
 *   void
 *
 * Returned Value:
 *   OK on setup success.
 *
 ****************************************************************************/

int fc7300_adc_setup(void);

#undef EXTERN
#ifdef __cplusplus
}
#endif
#endif /* __ASSEMBLY__ */

#endif /* __ARCH_ARM_SRC_FC7300_FC7300_ADC_H */
