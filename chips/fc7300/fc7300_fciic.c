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

#include <arch/board/board.h>
#include <assert.h>
#include <debug.h>
#include <errno.h>
#include <nuttx/arch.h>
#include <nuttx/i2c/i2c_master.h>
#include <nuttx/irq.h>
#include <nuttx/mutex.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <sys/types.h>

#include "arm_internal.h"
#include "chip.h"
#include "fc7300_clockconfig.h"
#include "fc7300_fciic.h"
#include "fc7300_irq.h"
#include "fc7300_periphclocks.h"
#include "hardware/fc7300_fciic.h"
#include "hardware/fc7300_pin.h"
#include "hardware/fc7300_pinmux.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Configuration ************************************************************/

/* CONFIG_I2C_POLLED may be set so that I2C interrupts will not be used.
 * Instead, CPU-intensive polling will be used.
 */

/* Interrupt/Polled wait time timeout in system timer ticks */

#define FCIIC_TIMEOTICKS                    \
  (SEC2TICK(CONFIG_FC7300_FCIIC_TIMEOSEC) + \
   MSEC2TICK(CONFIG_FC7300_FCIIC_TIMEOMS))

/****************************************************************************
 * Private Types
 ****************************************************************************/

/* Polled state */

enum fc7300_pollstate_e
{
  POLLSTATE_IDLE = 0, /* No I2C activity */
  POLLSTATE_WAITING,  /* Waiting for completion of data activity */
  POLLSTATE_DONE,     /* Data activity complete */
};

/* I2C Device hardware configuration */

struct fc7300_fciic_config_s
{
  uint32_t reg_base; /* I2C register base address */
  uint32_t scl_pin;  /* GPIO configuration for SCL */
  uint32_t sda_pin;  /* GPIO configuration for SDA */
  uint32_t filt_scl; /* Glitch filter SCL */
  uint32_t filt_sda; /* Glitch filter SDA */
  uint32_t bus_idle; /* Bus idle timeout */
  uint32_t pin_low;  /* Pin low timeout */

#ifndef CONFIG_I2C_POLLED
  uint32_t irq;          /* Interrupt ID */
  uint32_t irq_priority; /* Interrupt priority */
#endif

  enum pcc_clksrctype_e clk_name; /* Periphclock name */
};

/* I2C Device Private Data */

struct fc7300_fciic_priv_s
{
  const struct i2c_ops_s *ops; /* Standard I2C operations */

  /* Port configuration */

  const struct fc7300_fciic_config_s *config;
  mutex_t                             lock; /* Mutual exclusion mutex */
#ifdef CONFIG_I2C_POLLED
  volatile uint8_t pollstate; /* Polled handshake (see enum fc7300_pollstate_e)
                               */
#else
  sem_t sem_isr; /* Interrupt wait semaphore */
#endif

  /* I2C work state */

  uint16_t          status;
  struct i2c_msg_s *msgv;      /* Message list */
  uint8_t           msgc;      /* Message count */
  uint8_t          *ptr;       /* Current message buffer */
  uint32_t          frequency; /* Current I2C frequency */
  int               dcnt;      /* Current message length */
  uint16_t          flags;     /* Current message flags */
  int               refs;      /* Reference count */
};

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

/* Register functions */

static inline uint32_t fc7300_fciic_getreg(struct fc7300_fciic_priv_s *priv,
                                           uint16_t                    offset);
static inline void     fc7300_fciic_putreg(struct fc7300_fciic_priv_s *priv,
                                           uint16_t offset, uint32_t value);
static inline void     fc7300_fciic_modifyreg(struct fc7300_fciic_priv_s *priv,
                                              uint16_t offset, uint32_t clearbits,
                                              uint32_t setbits);

static inline int fc7300_fciic_sem_waitdone(struct fc7300_fciic_priv_s *priv);

static void            fc7300_fciic_setclock(struct fc7300_fciic_priv_s *priv,
                                             uint32_t                    frequency);
static inline void     fc7300_fciic_sendstart(struct fc7300_fciic_priv_s *priv,
                                              uint8_t                     address);
static inline void     fc7300_fciic_sendstop(struct fc7300_fciic_priv_s *priv);
static inline uint32_t fc7300_fciic_getstatus(struct fc7300_fciic_priv_s *priv);

static int fc7300_fciic_isr_process(struct fc7300_fciic_priv_s *priv);

#ifndef CONFIG_I2C_POLLED
static int fc7300_fciic_isr(int irq, void *context, void *arg);
#endif

static int fc7300_fciic_init(struct fc7300_fciic_priv_s *priv);
static int fc7300_fciic_deinit(struct fc7300_fciic_priv_s *priv);

static int fc7300_fciic_transfer(struct i2c_master_s *dev,
                                 struct i2c_msg_s *msgs, int count);
#ifdef CONFIG_I2C_RESET
static int fc7300_fciic_reset(struct i2c_master_s *dev);
#endif

/****************************************************************************
 * Private Data
 ****************************************************************************/

/* I2C interface */

static const struct i2c_ops_s g_fciic_ops =
{
  .transfer = fc7300_fciic_transfer,
#ifdef CONFIG_I2C_RESET
  .reset = fc7300_fciic_reset,
#endif
};

/* I2C device structures */

#ifdef CONFIG_FC7300_FCIIC0
static const struct fc7300_fciic_config_s g_fciic0_config = {
  .reg_base = FC7300_FCIIC0_BASE,
  .scl_pin  = PIN_FCIIC0_SCL,
  .sda_pin  = PIN_FCIIC0_SDA,
  .filt_scl = CONFIG_FC7300_FCIIC0_FILTSCL,
  .filt_sda = CONFIG_FC7300_FCIIC0_FILTSDA,
  .bus_idle = CONFIG_FC7300_FCIIC0_BUSIDLE,
  .pin_low  = CONFIG_FC7300_FCIIC0_PINLOW,

#  ifndef CONFIG_I2C_POLLED
  .irq          = FC7300_IRQ_FCIIC0,
  .irq_priority = NVIC_IIC0_PRIORITY_DEFAULT,
#  endif

  .clk_name = PCC_CLK_FCIIC0,
};

static struct fc7300_fciic_priv_s g_fciic0_priv =
{
  .ops    = &g_fciic_ops,
  .config = &g_fciic0_config,
  .frequency = 100000,
  .refs      = 0,
  .lock = NXMUTEX_INITIALIZER,
#  ifdef CONFIG_I2C_POLLED
  .pollstate = POLLSTATE_IDLE,
#  else
  .sem_isr = SEM_INITIALIZER(0),
#  endif
  .msgc   = 0,
  .msgv   = NULL,
  .ptr    = NULL,
  .dcnt   = 0,
  .flags  = 0,
  .status = 0
};
#endif /* CONFIG_FC7300_FCIIC0 */

#ifdef CONFIG_FC7300_FCIIC1
static const struct fc7300_fciic_config_s g_fciic1_config =
{
  .reg_base = FC7300_FCIIC1_BASE,
  .scl_pin  = PIN_FCIIC1_SCL,
  .sda_pin  = PIN_FCIIC1_SDA,
  .filt_scl = CONFIG_FC7300_FCIIC1_FILTSCL,
  .filt_sda = CONFIG_FC7300_FCIIC1_FILTSDA,
  .bus_idle = CONFIG_FC7300_FCIIC1_BUSIDLE,
  .pin_low  = CONFIG_FC7300_FCIIC1_PINLOW,

#  ifndef CONFIG_I2C_POLLED
  .irq          = FC7300_IRQ_FCIIC1,
  .irq_priority = NVIC_IIC1_PRIORITY_DEFAULT,
#  endif
  .clk_name = PCC_CLK_FCIIC1,
};

static struct fc7300_fciic_priv_s g_fciic1_priv =
{
  .ops    = &g_fciic_ops,
  .config = &g_fciic1_config,
  .frequency = 100000,
  .refs      = 0,
  .lock = NXMUTEX_INITIALIZER,
#  ifdef CONFIG_I2C_POLLED
  .pollstate = POLLSTATE_IDLE,
#  else
  .sem_isr = SEM_INITIALIZER(0),
#  endif
  .msgc   = 0,
  .msgv   = NULL,
  .ptr    = NULL,
  .dcnt   = 0,
  .flags  = 0,
  .status = 0
};
#endif /* CONFIG_FC7300_FCIIC1 */

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: fc7300_fciic_getreg
 *
 * Description:
 *   Get a 32-bit register value by offset
 *
 * Input Parameters:
 *   priv   - I2C slave device private data
 *   offset - Register offset with respect to the base address of the I2C
 *            peripheral
 *
 * Returned Value:
 *   The 32-bit value retrieved from the register
 *
 ****************************************************************************/

static inline uint32_t fc7300_fciic_getreg(struct fc7300_fciic_priv_s *priv,
                                           uint16_t                    offset)
{
  return getreg32(priv->config->reg_base + offset);
}

/****************************************************************************
 * Name: fc7300_fciic_putreg
 *
 * Description:
 *  Put a 32-bit register value by offset
 *
 * Input Parameters:
 *   priv   - I2C slave device private data
 *   offset - Register offset with respect to the base address of the I2C
 *            peripheral
 *   value  - The 32-bit value that should be put into the register
 *
 ****************************************************************************/

static inline void fc7300_fciic_putreg(struct fc7300_fciic_priv_s *priv,
                                       uint16_t offset, uint32_t value)
{
  putreg32(value, priv->config->reg_base + offset);
}

/****************************************************************************
 * Name: fc7300_fciic_modifyreg
 *
 * Description:
 *   Modify a 32-bit register value by offset
 *
 * Input Parameters:
 *   priv      - I2C slave device private data
 *   offset    - Register offset with respect to the base address of the I2C
 *               peripheral
 *   clearbits - Bitmask with the bits that should be cleared (put to 0)
 *   setbits   - Bitmask with the bits that should be set (put to 1)
 *
 ****************************************************************************/

static inline void fc7300_fciic_modifyreg(struct fc7300_fciic_priv_s *priv,
                                          uint16_t offset, uint32_t clearbits,
                                          uint32_t setbits)
{
  modifyreg32(priv->config->reg_base + offset, clearbits, setbits);
}

/****************************************************************************
 * Name: fc7300_fciic_sem_waitdone
 *
 * Description:
 *   Wait for a transfer to complete
 *
 ****************************************************************************/

#ifndef CONFIG_I2C_POLLED
static inline int fc7300_fciic_sem_waitdone(struct fc7300_fciic_priv_s *priv)
{
  irqstate_t flags;
  uint32_t   regval;
  int        ret;

  flags = enter_critical_section();

  /* Clear the TX and RX FIFOs */

  fc7300_fciic_modifyreg(priv, FC7300_FCIIC_MCR_OFFSET, 0,
                         FCIIC_MCR_RTF | FCIIC_MCR_RRF);

  /* Enable Interrupts for master mode */

  if ((priv->msgv->flags & I2C_M_READ) != 0)
    {
      regval = FCIIC_MIER_TDIE | FCIIC_MIER_RDIE | FCIIC_MIER_NDIE |
               FCIIC_MIER_ALIE | FCIIC_MIER_SDIE;
    }
  else
    {
      regval = FCIIC_MIER_TDIE | FCIIC_MIER_NDIE | FCIIC_MIER_ALIE |
               FCIIC_MIER_SDIE;
    }

  fc7300_fciic_putreg(priv, FC7300_FCIIC_MIER_OFFSET, regval);

  /* Wait until either the transfer is complete or the timeout expires */

  ret = nxsem_tickwait_uninterruptible(&priv->sem_isr, FCIIC_TIMEOTICKS);

  /* Disable I2C interrupts */

  fc7300_fciic_putreg(priv, FC7300_FCIIC_MIER_OFFSET, 0);

  leave_critical_section(flags);

  return ret;
}
#else
static inline int fc7300_fciic_sem_waitdone(struct fc7300_fciic_priv_s *priv)
{
  clock_t timeout;
  clock_t start;
  clock_t elapsed;
  int     ret;

  /* Get the timeout value */

  timeout         = FCIIC_TIMEOTICKS;

  priv->pollstate = POLLSTATE_WAITING;
  start           = clock_systime_ticks();

  do
    {
      /* Calculate the elapsed time */

      elapsed = clock_systime_ticks() - start;

      /* Poll by simply calling the timer interrupt handler until it
       * reports that it is done.
       */

      fc7300_fciic_isr_process(priv);
    }

  /* Loop until the transfer is complete. */

  while (priv->pollstate != POLLSTATE_DONE && elapsed < timeout);

  i2cinfo("pollstate: %d elapsed: %ld threshold: %ld status: %08x\n",
          priv->pollstate, (long)elapsed, (long)timeout, priv->status);

  /* Set the interrupt state back to IDLE */

  ret             = (priv->pollstate == POLLSTATE_DONE) ? OK : -ETIMEDOUT;
  priv->pollstate = POLLSTATE_IDLE;
  return ret;
}
#endif

/****************************************************************************
 * Name: fc7300_fciic_setclock
 *
 * Description:
 *   Set the I2C clock
 *
 ****************************************************************************/

static void fc7300_fciic_setclock(struct fc7300_fciic_priv_s *priv,
                                  uint32_t                    frequency)
{
  uint32_t src_freq = 0;
  uint32_t regval;
  uint32_t men;
  uint32_t prescale      = 0;
  uint32_t best_prescale = 0;
  uint32_t best_clk_hi   = 0;
  uint32_t abs_error     = 0;
  uint32_t best_error    = 0xffffffff;
  uint32_t clk_hi_cycle;
  uint32_t computed_rate;
  uint32_t count;
  uint32_t retval;

  /* Has the FCIIC bus frequency changed */

  if (frequency != priv->frequency)
    {
      /* Disable the selected FCIIC peripheral to configure the new
       * clock if it is enabled.
       */

      men = fc7300_fciic_getreg(priv, FC7300_FCIIC_MCR_OFFSET) & FCIIC_MCR_MEN;
      if (men)
        {
          fc7300_fciic_modifyreg(priv, FC7300_FCIIC_MCR_OFFSET, FCIIC_MCR_MEN,
                                 0);
        }

      /* Get the FCIIC clock source frequency */

      retval = fc7300_get_pclkfreq(priv->config->clk_name, &src_freq);
      if (OK == retval)
        {
          /* FCIIC output frequency = (Source Clock (Hz)/ 2^prescale) /
           *   (CLKLO + 1 + CLKHI + 1 + ROUNDDOWN((2 + FILTSCL) / 2^prescale)
           *
           * Assume  CLKLO = 2 * CLKHI, SETHOLD = CLKHI, DATAVD = CLKHI / 2
           */

          for (prescale = 1; (prescale <= 128) && (best_error != 0);
               prescale *= 2)
            {
              for (clk_hi_cycle = 1; clk_hi_cycle < 32; clk_hi_cycle++)
                {
                  if (clk_hi_cycle == 1)
                    {
                      computed_rate = (src_freq / prescale) /
                                      (6 + (2 / prescale));
                    }
                  else
                    {
                      computed_rate = (src_freq / prescale) /
                                      ((3 * clk_hi_cycle + 2) + (2 / prescale));
                    }

                  if (frequency > computed_rate)
                    {
                      abs_error = frequency - computed_rate;
                    }
                  else
                    {
                      abs_error = computed_rate - frequency;
                    }

                  if (abs_error < best_error)
                    {
                      best_prescale = prescale;
                      best_clk_hi   = clk_hi_cycle;
                      best_error    = abs_error;

                      if (abs_error == 0)
                        {
                          break;
                        }
                    }
                }
            }

          regval = FCIIC_MCCR_CLKHI(best_clk_hi);

          if (best_clk_hi < 2)
            {
              regval |= FCIIC_MCCR_CLKLO(3) | FCIIC_MCCR_SETHOLD(2) |
                        FCIIC_MCCR_DATAVD(1);
            }
          else
            {
              regval |= FCIIC_MCCR_CLKLO(2 * best_clk_hi) |
                        FCIIC_MCCR_SETHOLD(best_clk_hi) |
                        FCIIC_MCCR_DATAVD(best_clk_hi / 2);
            }

          fc7300_fciic_putreg(priv, FC7300_FCIIC_MCCR_OFFSET, regval);

          for (count = 0; count < 8; count++)
            {
              if (best_prescale == (1 << count))
                {
                  best_prescale = count;
                  break;
                }
            }

          fc7300_fciic_modifyreg(priv, FC7300_FCIIC_MCFGR1_OFFSET,
                                 FCIIC_MCFGR1_PRESCALE_MASK,
                                 FCIIC_MCFGR1_PRESCALE(best_prescale));

          /* Re-enable FCIIC if it was enabled previously */

          if (men)
            {
              fc7300_fciic_modifyreg(priv, FC7300_FCIIC_MCR_OFFSET, 0,
                                     FCIIC_MCR_MEN);
            }

          /* Save the new FCIIC frequency */

          priv->frequency = frequency;
        }
    }
}

/****************************************************************************
 * Name: fc7300_fciic_sendstart
 *
 * Description:
 *   Send the START conditions/force Master mode
 *
 ****************************************************************************/

static inline void fc7300_fciic_sendstart(struct fc7300_fciic_priv_s *priv,
                                          uint8_t                     address)
{
  uint8_t addr;

  /* Disable AUTOSTOP and turn NAK Ignore off */

  fc7300_fciic_modifyreg(priv, FC7300_FCIIC_MCFGR1_OFFSET,
                         FCIIC_MCFGR1_IGNACK | FCIIC_MCFGR1_AUTOSTOP, 0);

  if ((priv->flags & I2C_M_READ) != 0)
    {
      addr = I2C_READADDR8(address);
    }
  else
    {
      addr = I2C_WRITEADDR8(address);
    }

  /* Generate START condition and send the address */

  fc7300_fciic_putreg(priv, FC7300_FCIIC_MTDR_OFFSET,
                      (FCIIC_MTDR_CMD_STARTANDTRANSMIT | FCIIC_MTDR_DATA(addr)));
}

/****************************************************************************
 * Name: fc7300_fciic_sendstop
 *
 * Description:
 *   Send the STOP conditions
 *
 ****************************************************************************/

static inline void fc7300_fciic_sendstop(struct fc7300_fciic_priv_s *priv)
{
  fc7300_fciic_putreg(priv, FC7300_FCIIC_MTDR_OFFSET, FCIIC_MTDR_CMD_STOP);
}

/****************************************************************************
 * Name: fc7300_fciic_getstatus
 *
 * Description:
 *   Get 32-bit status
 *
 ****************************************************************************/

static inline uint32_t fc7300_fciic_getstatus(struct fc7300_fciic_priv_s *priv)
{
  return fc7300_fciic_getreg(priv, FC7300_FCIIC_MSR_OFFSET);
}

/****************************************************************************
 * Name: fc7300_fciic_isr_process
 *
 * Description:
 *  Common Interrupt Service Routine
 *
 ****************************************************************************/

__ramfunc__ static int fc7300_fciic_isr_process(struct fc7300_fciic_priv_s *priv)
{
  uint32_t status = fc7300_fciic_getstatus(priv);

  if ((status & (FCIIC_MSR_NDF | FCIIC_MSR_ALF)) == 0)
    {
      /* Check if there is more bytes to send */

      if (((priv->flags & I2C_M_READ) == 0) && ((status & FCIIC_MSR_TDF) != 0))
        {
          if (priv->dcnt > 0)
            {
              fc7300_fciic_putreg(priv, FC7300_FCIIC_MTDR_OFFSET,
                                  FCIIC_MTDR_CMD_TRANSMIT |
                                    FCIIC_MTDR_DATA(*priv->ptr++));
              priv->dcnt--;

              /* Last byte of last message? */

              if ((priv->msgc <= 0) && (priv->dcnt == 0))
                {
                  if ((priv->flags & I2C_M_NOSTOP) == 0)
                    {
                      /* Do this once */

                      priv->flags |= I2C_M_NOSTOP;
                      fc7300_fciic_sendstop(priv);
                    }
                }
            }
        }

      /* Check if there is more bytes to read */

      else if (((priv->flags & I2C_M_READ) != 0) &&
               ((status & FCIIC_MSR_RDF) != 0))
        {
          /* Read a byte, if dcnt goes < 0, read dummy bytes to ack ISRs */

          if (priv->dcnt > 0)
            {
              /* No interrupts or context switches should occur in the
               * following sequence. Otherwise, additional bytes may be
               * sent by the device.
               */

#ifdef CONFIG_I2C_POLLED
              irqstate_t flags = enter_critical_section();
#endif

              /* Receive a byte */

              *priv->ptr++ = fc7300_fciic_getreg(priv,
                                                 FC7300_FCIIC_MRDR_OFFSET) &
                             FCIIC_MRDR_DATA_MASK;
              priv->dcnt--;

#ifdef CONFIG_I2C_POLLED
              leave_critical_section(flags);
#endif
              /* Last byte of last message? */

              if ((priv->msgc <= 0) && (priv->dcnt == 0))
                {
                  if ((priv->flags & I2C_M_NOSTOP) == 0)
                    {
                      /* Do this once */

                      priv->flags |= I2C_M_NOSTOP;
                      fc7300_fciic_sendstop(priv);
                    }
                }
            }
          else
            {
              /* Read and discard data */

              fc7300_fciic_getreg(priv, FC7300_FCIIC_MRDR_OFFSET);
            }
        }

      /* Start the first or next message */

      if ((priv->dcnt <= 0) && ((status & FCIIC_MSR_SDF) == 0))
        {
          if (priv->msgc > 0)
            {
              priv->ptr   = priv->msgv->buffer;
              priv->dcnt  = priv->msgv->length;
              priv->flags = priv->msgv->flags;

              if ((priv->flags & I2C_M_NOSTART) == 0)
                {
                  /* Do this once */

                  priv->flags |= I2C_M_NOSTART;

                  fc7300_fciic_sendstart(priv, priv->msgv->addr);
                }

              priv->msgv++;
              priv->msgc--;

              if ((priv->flags & I2C_M_READ) != 0)
                {
#ifndef CONFIG_I2C_POLLED
                  /* Stop TX interrupt */

                  fc7300_fciic_modifyreg(priv, FC7300_FCIIC_MIER_OFFSET,
                                         FCIIC_MIER_TDIE, FCIIC_MIER_RDIE);
#endif
                  /* Set FCIIC in read mode */

                  fc7300_fciic_putreg(priv, FC7300_FCIIC_MTDR_OFFSET,
                                      FCIIC_MTDR_CMD_RECEIVE |
                                        FCIIC_MTDR_DATA((priv->dcnt - 1)));
                }
              else
                {
                  /* Send the first byte from tx buffer */

                  fc7300_fciic_putreg(priv, FC7300_FCIIC_MTDR_OFFSET,
                                      FCIIC_MTDR_CMD_TRANSMIT |
                                        FCIIC_MTDR_DATA(*priv->ptr++));
                  priv->dcnt--;

                  /* Last byte of last message? */

                  if ((priv->msgc <= 0) && (priv->dcnt == 0))
                    {
                      if ((priv->flags & I2C_M_NOSTOP) == 0)
                        {
                          /* Do this once */

                          priv->flags |= I2C_M_NOSTOP;
                          fc7300_fciic_sendstop(priv);
                        }
                    }
                }
            }
        }
    }
  else
    {
      priv->status = status;

      if ((priv->flags & I2C_M_NOSTOP) == 0)
        {
          /* Do this once */

          priv->flags |= I2C_M_NOSTOP;
          fc7300_fciic_sendstop(priv);
        }

      /* Clear the error */

      fc7300_fciic_putreg(priv, FC7300_FCIIC_MSR_OFFSET,
                          (status & (FCIIC_MSR_NDF | FCIIC_MSR_ALF |
                                     FCIIC_MSR_FEF | FCIIC_MSR_EPF)));
    }

  /* Check for endof packet */

  if ((status & FCIIC_MSR_SDF) != 0)
    {
      fc7300_fciic_putreg(priv, FC7300_FCIIC_MSR_OFFSET,
                          status & (FCIIC_MSR_EPF | FCIIC_MSR_SDF));

#ifndef CONFIG_I2C_POLLED
      /* inform the thread that transfer is complete
       * and wake it up
       */

      fc7300_fciic_modifyreg(priv, FC7300_FCIIC_MIER_OFFSET,
                             FCIIC_MIER_TDIE | FCIIC_MIER_RDIE |
                               FCIIC_MIER_NDIE | FCIIC_MIER_ALIE |
                               FCIIC_MIER_SDIE | FCIIC_MIER_EPIE,
                             0);
      nxsem_post(&priv->sem_isr);
#else
      priv->pollstate = POLLSTATE_DONE;
#endif
    }

  return OK;
}

#ifndef CONFIG_I2C_POLLED
#  ifdef CONFIG_FC7300_FCIIC_IRQ_ATTACH_THREAD
/****************************************************************************
 * Name: fc7300_fciic_isr_handle
 *
 * Description:
 *   Disable fciicn interrupt.
 *
 * Input Parameters:
 *
 * Returned Value: void
 *
 ****************************************************************************/

__ramfunc__ static int fc7300_fciic_isr_handle(int irq, void *regs, void *arg)
{
  up_disable_irq(irq);
  return IRQ_WAKE_THREAD;
}
#  endif

/****************************************************************************
 * Name: fc7300_fciic_isr
 *
 * Description:
 *   Common I2C interrupt service routine
 *
 ****************************************************************************/

__ramfunc__ static int fc7300_fciic_isr(int irq, void *context, void *arg)
{
  struct fc7300_fciic_priv_s *priv = (struct fc7300_fciic_priv_s *)arg;

  DEBUGASSERT(priv != NULL);
  fc7300_fciic_isr_process(priv);

#  ifdef CONFIG_FC7300_FCIIC_IRQ_ATTACH_THREAD
  fc7300_clearpendingisr(irq);
  up_enable_irq(irq);
#  endif

  return OK;
}
#endif

/****************************************************************************
 * Name: fc7300_fciic_init
 *
 * Description:
 *   Setup the I2C hardware, ready for operation with defaults
 *
 ****************************************************************************/

static int fc7300_fciic_init(struct fc7300_fciic_priv_s *priv)
{
  /* Configure pins */

  fc7300_pinconfig(priv->config->scl_pin);
  fc7300_pinconfig(priv->config->sda_pin);

  /* Reset FCIIC before configuring it */

  fc7300_fciic_putreg(priv, FC7300_FCIIC_MCR_OFFSET, FCIIC_MCR_RST);
  fc7300_fciic_putreg(priv, FC7300_FCIIC_MCR_OFFSET, 0);

  /* Disable AUTOSTOP and turn NAK Ignore off */

  fc7300_fciic_modifyreg(priv, FC7300_FCIIC_MCFGR1_OFFSET,
                         FCIIC_MCFGR1_AUTOSTOP | FCIIC_MCFGR1_IGNACK, 0);

  priv->frequency = 0;
  fc7300_fciic_setclock(priv, 100000);

  /* Set scl, sda glitch filters and busy idle */

  fc7300_fciic_putreg(priv, FC7300_FCIIC_MCFGR2_OFFSET,
                      FCIIC_MCFGR2_BUSIDLE(priv->config->bus_idle) |
                        FCIIC_MCFGR2_FILTSCL(priv->config->filt_scl) |
                        FCIIC_MCFGR2_FILTSDA(priv->config->filt_sda));

  /* Set pin low cycles to 0 (disable) */

  fc7300_fciic_putreg(priv, FC7300_FCIIC_MCFGR3_OFFSET,
                      FCIIC_MCFGR3_PINLOW(priv->config->pin_low));

  /* Set tx and rx watermarks */

  fc7300_fciic_putreg(priv, FC7300_FCIIC_MFCR_OFFSET,
                      FCIIC_MFCR_TXWATER(0) | FCIIC_MFCR_RXWATER(0));

#ifndef CONFIG_I2C_POLLED
  /* Attach ISRs */

#  ifdef CONFIG_FC7300_FCIIC_IRQ_ATTACH_THREAD
  irq_attach_wqueue(priv->config->irq, fc7300_fciic_isr_handle,
                    fc7300_fciic_isr, priv, isr_cfg[FCIIC_ISR_THREAD]);
#  else
  irq_attach(priv->config->irq, fc7300_fciic_isr, priv);
#  endif
#  ifdef CONFIG_ARCH_IRQPRIO
  up_prioritize_irq(priv->config->irq, priv->config->irq_priority);
#  endif
  up_enable_irq(priv->config->irq);
#endif

  /* Enable FCIIC */

  fc7300_fciic_modifyreg(priv, FC7300_FCIIC_MCR_OFFSET, 0, FCIIC_MCR_MEN);

  return OK;
}

/****************************************************************************
 * Name: fc7300_fciic_deinit
 *
 * Description:
 *   Shutdown the I2C hardware
 *
 ****************************************************************************/

static int fc7300_fciic_deinit(struct fc7300_fciic_priv_s *priv)
{
  /* Disable FCIIC */

  fc7300_fciic_modifyreg(priv, FC7300_FCIIC_MCR_OFFSET, FCIIC_MCR_MEN, 0);

  /* Reset FCIIC */

  fc7300_fciic_putreg(priv, FC7300_FCIIC_MCR_OFFSET, FCIIC_MCR_MEN);
  fc7300_fciic_putreg(priv, FC7300_FCIIC_MCR_OFFSET, 0);

#ifndef CONFIG_I2C_POLLED
  /* Disable and detach interrupts */

  up_disable_irq(priv->config->irq);
  irq_detach(priv->config->irq);
#endif

  return OK;
}

/****************************************************************************
 * Device Driver Operations
 ****************************************************************************/

/****************************************************************************
 * Name: fc7300_fciic_transfer
 *
 * Description:
 *   Generic I2C transfer function
 *
 ****************************************************************************/

static int fc7300_fciic_transfer(struct i2c_master_s *dev,
                                 struct i2c_msg_s *msgs, int count)
{
  struct fc7300_fciic_priv_s *priv = (struct fc7300_fciic_priv_s *)dev;
  int                         ret;

  DEBUGASSERT(count > 0);

  /* Ensure that address or flags don't change meanwhile */

  ret = nxmutex_lock(&priv->lock);
  if (ret < 0)
    {
      return ret;
    }

  /* Clear any pending error interrupts */

  fc7300_fciic_putreg(priv, FC7300_FCIIC_MSR_OFFSET, 0xffffffff);

  /* Reset ptr and bytes to ensure an unexpected data interrupt doesn't
   * overwrite stale data.
   */

  priv->dcnt  = 0;
  priv->ptr   = NULL;

  priv->msgv  = msgs;
  priv->msgc  = count;
  priv->flags = msgs->flags;

  i2cinfo("Flags %x, len %d\n", msgs->flags, msgs->length);

  /* Set I2C clock frequency */

  fc7300_fciic_setclock(priv, msgs->frequency);

  priv->status = 0;

  /* Wait for an ISR, if there was a timeout, fetch latest status to get
   * the BUSY flag.
   */

  if (fc7300_fciic_sem_waitdone(priv) < 0)
    {
      ret = -ETIMEDOUT;
      i2cerr("ERROR: Timed out: MSR: status: 0x%02x\n", priv->status);
    }

  /* Check for error status conditions */
  //coverity[CONSTANT_EXPRESSION_RESULT]
  else if (priv->status & FCIIC_MSR_ALF)
    {
      /* Arbitration Lost (master mode) */

      i2cerr("Arbitration lost\n");
      ret = -EAGAIN;
    }
  else if (priv->status & FCIIC_MSR_NDF)
    {
      /* Acknowledge Failure */

      i2cerr("Ack failure\n");
      ret = -ENXIO;
    }
  else if (priv->status & FCIIC_MIER_ALIE)
    {
      /* FIFO Error */

      i2cerr("Transfer without start condition\n");
      ret = -EINVAL;
    }

  /* Ensure that any ISR happening after we finish can't overwrite any user
   * data.
   */

  priv->dcnt = 0;
  priv->ptr  = NULL;

  nxmutex_unlock(&priv->lock);
  return ret;
}

/****************************************************************************
 * Name: fc7300_fciic_reset
 *
 * Description:
 *   Perform an I2C bus reset in an attempt to break loose stuck I2C devices.
 *
 * Input Parameters:
 *   dev   - Device-specific state data
 *
 * Returned Value:
 *   Zero (OK) on success; a negated errno value on failure.
 *
 ****************************************************************************/

#ifdef CONFIG_I2C_RESET
static int fc7300_fciic_reset(struct i2c_master_s *dev)
{
  struct fc7300_fciic_priv_s *priv = (struct fc7300_fciic_priv_s *)dev;
  unsigned int                clock_count;
  uint32_t                    scl_gpio;
  uint32_t                    sda_gpio;
  uint32_t                    frequency;
  int                         ret;

  DEBUGASSERT(dev);

  /* Our caller must own a ref */

  DEBUGASSERT(priv->refs > 0);

  /* Lock out other clients */

  ret = nxmutex_lock(&priv->lock);
  if (ret < 0)
    {
      return ret;
    }

  ret = -EIO;

  /* Save the current frequency */

  frequency = priv->frequency;

  /* De-init the port */

  fc7300_fciic_deinit(priv);

  /* Use GPIO configuration to un-wedge the bus */

  scl_gpio = (priv->config->scl_pin & (~_PIN_MODE_MASK)) | GPIO_OUTPUT;
  sda_gpio = (priv->config->sda_pin & (~_PIN_MODE_MASK)) | GPIO_OUTPUT;

  fc7300_pinconfig(scl_gpio);
  fc7300_pinconfig(sda_gpio);

  /* Let SDA go high */

  fc7300_gpiowrite(sda_gpio, 1);

  scl_gpio = (priv->config->scl_pin & (~_PIN_MODE_MASK)) | GPIO_INPUT;
  sda_gpio = (priv->config->sda_pin & (~_PIN_MODE_MASK)) | GPIO_INPUT;

  fc7300_pinconfig(scl_gpio);
  fc7300_pinconfig(sda_gpio);

  /* Clock the bus until any slaves currently driving it let it go. */

  clock_count = 0;

  while ((!fc7300_gpioread(sda_gpio)) || (!fc7300_gpioread(scl_gpio)))
    {
      /* Give up if we have tried too hard */

      if (clock_count++ > 10)
        {
          goto out;
        }

      scl_gpio = (priv->config->scl_pin & (~_PIN_MODE_MASK)) | GPIO_OUTPUT;
      fc7300_pinconfig(scl_gpio);

      /* Drive SCL low */

      fc7300_gpiowrite(scl_gpio, 0);
      up_udelay(10);

      /* Drive SCL high again */

      fc7300_gpiowrite(scl_gpio, 1);
      up_udelay(10);

      scl_gpio = (priv->config->scl_pin & (~_PIN_MODE_MASK)) | GPIO_INPUT;
      fc7300_pinconfig(scl_gpio);
    }

  scl_gpio = (priv->config->scl_pin & (~_PIN_MODE_MASK)) | GPIO_OUTPUT;
  sda_gpio = (priv->config->sda_pin & (~_PIN_MODE_MASK)) | GPIO_OUTPUT;

  fc7300_pinconfig(scl_gpio);
  fc7300_pinconfig(sda_gpio);

  /* Generate a start followed by a stop to reset slave
   * state machines.
   */

  fc7300_gpiowrite(sda_gpio, 0);
  up_udelay(10);
  fc7300_gpiowrite(scl_gpio, 0);
  up_udelay(10);
  fc7300_gpiowrite(scl_gpio, 1);
  up_udelay(10);
  fc7300_gpiowrite(sda_gpio, 1);
  up_udelay(10);

  /* Re-init the port */

  fc7300_fciic_init(priv);

  /* Restore the frequency */

  fc7300_fciic_setclock(priv, frequency);
  ret = OK;

out:

  /* Release the port for re-use by other clients */

  nxmutex_unlock(&priv->lock);
  return ret;
}
#endif /* CONFIG_I2C_RESET */

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: fc7300_i2cbus_initialize
 *
 * Description:
 *   Initialize one I2C bus
 *
 ****************************************************************************/

struct i2c_master_s *fc7300_i2cbus_initialize(int port)
{
  struct fc7300_fciic_priv_s *priv = NULL;

  /* Get I2C private structure */

  switch (port)
    {
#ifdef CONFIG_FC7300_FCIIC0
      case 0:
        priv = (struct fc7300_fciic_priv_s *)&g_fciic0_priv;
        break;
#endif
#ifdef CONFIG_FC7300_FCIIC1
      case 1:
        priv = (struct fc7300_fciic_priv_s *)&g_fciic1_priv;
        break;
#endif
      default:
        return NULL;
    }

  /* Initialize private data for the first time, increment reference count,
   * power-up hardware and configure GPIOs.
   */

  nxmutex_lock(&priv->lock);

  if (priv->refs == 0)
    {
      fc7300_fciic_init(priv);
    }

  priv->refs++;

  nxmutex_unlock(&priv->lock);

  return (struct i2c_master_s *)priv;
}

/****************************************************************************
 * Name: fc7300_i2cbus_uninitialize
 *
 * Description:
 *   Uninitialize an I2C bus
 *
 ****************************************************************************/

int fc7300_i2cbus_uninitialize(struct i2c_master_s *dev)
{
  struct fc7300_fciic_priv_s *priv = (struct fc7300_fciic_priv_s *)dev;

  DEBUGASSERT(dev);

  /* Decrement reference count and check for underflow */

  nxmutex_lock(&priv->lock);

  if (priv->refs == 0)
    {
      nxmutex_unlock(&priv->lock);
      return ERROR;
    }

  priv->refs--;

  if (priv->refs > 0)
    {
      nxmutex_unlock(&priv->lock);
      return OK;
    }

  /* Disable power and other HW resource (GPIO's) */

  fc7300_fciic_deinit(priv);

  nxmutex_unlock(&priv->lock);

  return OK;
}
