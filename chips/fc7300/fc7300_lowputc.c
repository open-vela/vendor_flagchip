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
#include <fixedmath.h>
#include <assert.h>
#include <errno.h>

#include "hardware/fc7300_pinmux.h"
#include "hardware/fc7300_fcuart.h"
#include "hardware/fc7300_pin.h"

#include "fc7300_config.h"
#include "fc7300_lowputc.h"
#include "fc7300_periphclocks.h"
#include "arm_internal.h"

#include <arch/board/board.h> /* Include last:  has dependencies */

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Private Data
 ****************************************************************************/

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: fc7300_lowsetup
 *
 * Description:
 *   Called at the very beginning of _start.  Performs low level
 *   initialization including setup of the console UART.  This UART done
 *   early so that the serial console is available for debugging very early
 *   in the boot sequence.
 *
 ****************************************************************************/

void fc7300_lowsetup(void)
{
#ifndef CONFIG_NO_FCUART_SERIAL_CONSOLE
  uint32_t osr;
  uint16_t sbr;
  uint32_t regval;
#endif /* CONFIG_NO_FCUART_SERIAL_CONSOLE */

#ifdef CONFIG_FC7300_FCUART0
  /* Configure FCUART0 pins: RXD and TXD.
   * Also configure RTS and CTS if flow control is enabled.
   */

  fc7300_pinconfig(PIN_FCUART0_RX);
  fc7300_pinconfig(PIN_FCUART0_TX);
#ifdef CONFIG_FCUART0_OFLOWCONTROL
  fc7300_pinconfig(PIN_FCUART0_CTS);
#endif
#ifdef CONFIG_FCUART0_IFLOWCONTROL
  fc7300_pinconfig(PIN_FCUART0_RTS);
#endif
#endif

#ifdef CONFIG_FC7300_FCUART1
  /* Configure FCUART1 pins: RXD and TXD.
   * Also configure RTS and CTS if flow control is enabled.
   */

  fc7300_pinconfig(PIN_FCUART1_RX);
  fc7300_pinconfig(PIN_FCUART1_TX);
#ifdef CONFIG_FCUART1_OFLOWCONTROL
  fc7300_pinconfig(PIN_FCUART1_CTS);
#endif
#ifdef CONFIG_FCUART1_IFLOWCONTROL
  fc7300_pinconfig(PIN_FCUART1_RTS);
#endif
#endif

#ifdef CONFIG_FC7300_FCUART2
  /* Configure FCUART2 pins: RXD and TXD.
   * Also configure RTS and CTS if flow control is enabled.
   */

  fc7300_pinconfig(PIN_FCUART2_RX);
  fc7300_pinconfig(PIN_FCUART2_TX);
#ifdef CONFIG_FCUART2_OFLOWCONTROL
  fc7300_pinconfig(PIN_FCUART2_CTS);
#endif
#ifdef CONFIG_FCUART2_IFLOWCONTROL
  fc7300_pinconfig(PIN_FCUART2_RTS);
#endif
#endif

#ifdef CONFIG_FC7300_FCUART3
  /* Configure FCUART3 pins: RXD and TXD.
   * Also configure RTS and CTS if flow control is enabled.
   */

  fc7300_pinconfig(PIN_FCUART3_RX);
  fc7300_pinconfig(PIN_FCUART3_TX);
#ifdef CONFIG_FCUART3_OFLOWCONTROL
  fc7300_pinconfig(PIN_FCUART3_CTS);
#endif
#ifdef CONFIG_FCUART3_IFLOWCONTROL
  fc7300_pinconfig(PIN_FCUART3_RTS);
#endif
#endif

#ifdef CONFIG_FC7300_FCUART4
  /* Configure FCUART4 pins: RXD and TXD.
   * Also configure RTS and CTS if flow control is enabled.
   */

  fc7300_pinconfig(PIN_FCUART4_RX);
  fc7300_pinconfig(PIN_FCUART4_TX);
#ifdef CONFIG_FCUART4_OFLOWCONTROL
  fc7300_pinconfig(PIN_FCUART4_CTS);
#endif
#ifdef CONFIG_FCUART4_IFLOWCONTROL
  fc7300_pinconfig(PIN_FCUART4_RTS);
#endif
#endif

#ifdef CONFIG_FC7300_FCUART5
  /* Configure FCUART5 pins: RXD and TXD.
   * Also configure RTS and CTS if flow control is enabled.
   */

  fc7300_pinconfig(PIN_FCUART5_RX);
  fc7300_pinconfig(PIN_FCUART5_TX);
#ifdef CONFIG_FCUART5_OFLOWCONTROL
  fc7300_pinconfig(PIN_FCUART5_CTS);
#endif
#ifdef CONFIG_FCUART5_IFLOWCONTROL
  fc7300_pinconfig(PIN_FCUART5_RTS);
#endif
#endif

#ifdef CONFIG_FC7300_FCUART6
  /* Configure FCUART6 pins: RXD and TXD.
   * Also configure RTS and CTS if flow control is enabled.
   */

  fc7300_pinconfig(PIN_FCUART6_RX);
  fc7300_pinconfig(PIN_FCUART6_TX);
#ifdef CONFIG_FCUART6_OFLOWCONTROL
  fc7300_pinconfig(PIN_FCUART6_CTS);
#endif
#ifdef CONFIG_FCUART6_IFLOWCONTROL
  fc7300_pinconfig(PIN_FCUART6_RTS);
#endif
#endif

#ifdef CONFIG_FC7300_FCUART7
  /* Configure FCUART7 pins: RXD and TXD.
   * Also configure RTS and CTS if flow control is enabled.
   */

  fc7300_pinconfig(PIN_FCUART7_RX);
  fc7300_pinconfig(PIN_FCUART7_TX);
#ifdef CONFIG_FCUART7_OFLOWCONTROL
  fc7300_pinconfig(PIN_FCUART7_CTS);
#endif
#ifdef CONFIG_FCUART7_IFLOWCONTROL
  fc7300_pinconfig(PIN_FCUART7_RTS);
#endif
#endif

#ifdef CONFIG_FC7300_FCUART8
  /* Configure FCUART8 pins: RXD and TXD.
   * Also configure RTS and CTS if flow control is enabled.
   */

  fc7300_pinconfig(PIN_FCUART8_RX);
  fc7300_pinconfig(PIN_FCUART8_TX);
#ifdef CONFIG_FCUART8_OFLOWCONTROL
  fc7300_pinconfig(PIN_FCUART8_CTS);
#endif
#ifdef CONFIG_FCUART8_IFLOWCONTROL
  fc7300_pinconfig(PIN_FCUART8_RTS);
#endif
#endif

#ifdef CONFIG_FC7300_FCUART9
  /* Configure FCUART9 pins: RXD and TXD.
   * Also configure RTS and CTS if flow control is enabled.
   */

  fc7300_pinconfig(PIN_FCUART9_RX);
  fc7300_pinconfig(PIN_FCUART9_TX);
#ifdef CONFIG_FCUART9_OFLOWCONTROL
  fc7300_pinconfig(PIN_FCUART9_CTS);
#endif
#ifdef CONFIG_FCUART9_IFLOWCONTROL
  fc7300_pinconfig(PIN_FCUART9_RTS);
#endif
#endif

#ifdef CONFIG_FC7300_FCUART10
  /* Configure FCUART10 pins: RXD and TXD.
   * Also configure RTS and CTS if flow control is enabled.
   */

  fc7300_pinconfig(PIN_FCUART10_RX);
  fc7300_pinconfig(PIN_FCUART10_TX);
#ifdef CONFIG_FCUART10_OFLOWCONTROL
  fc7300_pinconfig(PIN_FCUART10_CTS);
#endif
#ifdef CONFIG_FCUART10_IFLOWCONTROL
  fc7300_pinconfig(PIN_FCUART10_RTS);
#endif
#endif

#ifdef CONFIG_FC7300_FCUART11
  /* Configure FCUART11 pins: RXD and TXD.
   * Also configure RTS and CTS if flow control is enabled.
   */

  fc7300_pinconfig(PIN_FCUART11_RX);
  fc7300_pinconfig(PIN_FCUART11_TX);
#ifdef CONFIG_FCUART11_OFLOWCONTROL
  fc7300_pinconfig(PIN_FCUART11_CTS);
#endif
#ifdef CONFIG_FCUART11_IFLOWCONTROL
  fc7300_pinconfig(PIN_FCUART11_RTS);
#endif
#endif

#ifdef CONFIG_FC7300_FCUART12
  /* Configure FCUART12 pins: RXD and TXD.
   * Also configure RTS and CTS if flow control is enabled.
   */

  fc7300_pinconfig(PIN_FCUART12_RX);
  fc7300_pinconfig(PIN_FCUART12_TX);
#ifdef CONFIG_FCUART12_OFLOWCONTROL
  fc7300_pinconfig(PIN_FCUART12_CTS);
#endif
#ifdef CONFIG_FCUART12_IFLOWCONTROL
  fc7300_pinconfig(PIN_FCUART12_RTS);
#endif
#endif

#ifdef CONFIG_FC7300_FCUART13
  /* Configure FCUART13 pins: RXD and TXD.
   * Also configure RTS and CTS if flow control is enabled.
   */

  fc7300_pinconfig(PIN_FCUART13_RX);
  fc7300_pinconfig(PIN_FCUART13_TX);
#ifdef CONFIG_FCUART13_OFLOWCONTROL
  fc7300_pinconfig(PIN_FCUART13_CTS);
#endif
#ifdef CONFIG_FCUART13_IFLOWCONTROL
  fc7300_pinconfig(PIN_FCUART13_RTS);
#endif
#endif

#ifdef CONFIG_FC7300_FCUART14
  /* Configure FCUART14 pins: RXD and TXD.
   * Also configure RTS and CTS if flow control is enabled.
   */

  fc7300_pinconfig(PIN_FCUART14_RX);
  fc7300_pinconfig(PIN_FCUART14_TX);
#ifdef CONFIG_FCUART14_OFLOWCONTROL
  fc7300_pinconfig(PIN_FCUART14_CTS);
#endif
#ifdef CONFIG_FCUART14_IFLOWCONTROL
  fc7300_pinconfig(PIN_FCUART14_RTS);
#endif
#endif

#ifdef CONFIG_FC7300_FCUART15
  /* Configure FCUART15 pins: RXD and TXD.
   * Also configure RTS and CTS if flow control is enabled.
   */

  fc7300_pinconfig(PIN_FCUART15_RX);
  fc7300_pinconfig(PIN_FCUART15_TX);
#ifdef CONFIG_FCUART15_OFLOWCONTROL
  fc7300_pinconfig(PIN_FCUART15_CTS);
#endif
#ifdef CONFIG_FCUART15_IFLOWCONTROL
  fc7300_pinconfig(PIN_FCUART15_RTS);
#endif
#endif

#ifdef CONFIG_FC7300_FCUART16
  /* Configure FCUART16 pins: RXD and TXD.
   * Also configure RTS and CTS if flow control is enabled.
   */

  fc7300_pinconfig(PIN_FCUART16_RX);
  fc7300_pinconfig(PIN_FCUART16_TX);
#ifdef CONFIG_FCUART16_OFLOWCONTROL
  fc7300_pinconfig(PIN_FCUART16_CTS);
#endif
#ifdef CONFIG_FCUART16_IFLOWCONTROL
  fc7300_pinconfig(PIN_FCUART16_RTS);
#endif
#endif

#ifdef CONFIG_FC7300_FCUART17
  /* Configure FCUART17 pins: RXD and TXD.
   * Also configure RTS and CTS if flow control is enabled.
   */

  fc7300_pinconfig(PIN_FCUART17_RX);
  fc7300_pinconfig(PIN_FCUART17_TX);
#ifdef CONFIG_FCUART17_OFLOWCONTROL
  fc7300_pinconfig(PIN_FCUART17_CTS);
#endif
#ifdef CONFIG_FCUART17_IFLOWCONTROL
  fc7300_pinconfig(PIN_FCUART17_RTS);
#endif
#endif

#ifndef CONFIG_NO_FCUART_SERIAL_CONSOLE

  /* Configure the serial console for initial, non-interrupt driver mode */

  if (fc7300_fcuart_processbaud(FC7300_CONSOLE_BASE, FC7300_CONSOLE_BAUD, \
                                &osr, &sbr) != OK)
    {
      return;
    }

  /* Reset all internal logic and registers */

  regval  = getreg32(FC7300_CONSOLE_BASE + FC7300_FCUART_RST_OFFSET);
  regval |= FCUART_RST_RST;
  putreg32(regval, FC7300_CONSOLE_BASE + FC7300_FCUART_RST_OFFSET);

  regval &= ~FCUART_RST_RST;
  putreg32(regval, FC7300_CONSOLE_BASE + FC7300_FCUART_RST_OFFSET);

  /* Configure BAUD register */

  regval = 0;

  if ((osr > 3) && (osr < 8))
    {
      regval |= FCUART_BAUD_BEDGE_SAMP;
    }

  if (FC7300_CONSOLE_2STOP)
    {
      regval |= FCUART_BAUD_SBNS;
    }

  regval |= FCUART_BAUD_OVR_SAMP(osr - 1) | FCUART_BAUD_SBR(sbr);
  putreg32(regval, FC7300_CONSOLE_BASE + FC7300_FCUART_BAUD_OFFSET);

  /* Configure CTRL register */

  regval = 0;
  if (FC7300_CONSOLE_PARITY == 1)
    {
      regval |= FCUART_CTRL_PE | FCUART_CTRL_PT_ODD;
    }
  else if (FC7300_CONSOLE_PARITY == 2)
    {
      regval |= FCUART_CTRL_PE | FCUART_CTRL_PT_EVEN;
    }

  if (FC7300_CONSOLE_BITS == 7)
    {
      regval |= FCUART_CTRL_7BMS;
    }
  else if (FC7300_CONSOLE_BITS == 8)
    {
      regval |= FCUART_CTRL_BMSEL_8BIT;
    }
  else if (FC7300_CONSOLE_BITS == 9)
    {
      regval |= FCUART_CTRL_BMSEL_9BIT;
    }
  else
    {
      return;
    }

  regval |= FCUART_CTRL_RE | FCUART_CTRL_TE;
  putreg32(regval, FC7300_CONSOLE_BASE + FC7300_FCUART_CTRL_OFFSET);

#endif /* CONFIG_NO_FCUART_SERIAL_CONSOLE */
}

/****************************************************************************
 * Name: fc7300_fcuart_processbaud
 *
 * Description:
 *   Process get OverSamp and SBR
 *
 ****************************************************************************/

#if defined(USE_SERIALDRIVER) || defined(CONFIG_FC7300_FCUART)
int fc7300_fcuart_processbaud(uint32_t base, uint32_t baud,
                              uint32_t *osr, uint16_t *sbr)
{
  enum pcc_clksrctype_e clkname;
  uint32_t fcuart_freq = 0;
  uint16_t temp_sbr;
  uint32_t temp_osr;
  uint32_t temp_diff;
  uint32_t calculated_baud;
  uint32_t baud_diff;
  int ret;

  *osr       = 16;
  *sbr       = 4;

  /* Functional clocking is provided via the  PCC.  The PCC clocking must
   * be configured by board-specific logic prior to using the FCUART.
   */

  /* Get the PCC source clock */

  switch (base)
    {
      case FC7300_FCUART0_BASE:
        clkname = PCC_CLK_FCUART0;
        break;
      case FC7300_FCUART1_BASE:
        clkname = PCC_CLK_FCUART1;
        break;
      case FC7300_FCUART2_BASE:
        clkname = PCC_CLK_FCUART2;
        break;
      case FC7300_FCUART3_BASE:
        clkname = PCC_CLK_FCUART3;
        break;
      case FC7300_FCUART4_BASE:
        clkname = PCC_CLK_FCUART4;
        break;
      case FC7300_FCUART5_BASE:
        clkname = PCC_CLK_FCUART5;
        break;
      case FC7300_FCUART6_BASE:
        clkname = PCC_CLK_FCUART6;
        break;
      case FC7300_FCUART7_BASE:
        clkname = PCC_CLK_FCUART7;
        break;
      case FC7300_FCUART8_BASE:
        clkname = PCC_CLK_FCUART8;
        break;
      case FC7300_FCUART9_BASE:
        clkname = PCC_CLK_FCUART9;
        break;
      case FC7300_FCUART10_BASE:
        clkname = PCC_CLK_FCUART10;
        break;
      case FC7300_FCUART11_BASE:
        clkname = PCC_CLK_FCUART11;
        break;
      case FC7300_FCUART12_BASE:
        clkname = PCC_CLK_FCUART12;
        break;
      case FC7300_FCUART13_BASE:
        clkname = PCC_CLK_FCUART13;
        break;
      case FC7300_FCUART14_BASE:
        clkname = PCC_CLK_FCUART14;
        break;
      case FC7300_FCUART15_BASE:
        clkname = PCC_CLK_FCUART15;
        break;
      case FC7300_FCUART16_BASE:
        clkname = PCC_CLK_FCUART16;
        break;
      case FC7300_FCUART17_BASE:
        clkname = PCC_CLK_FCUART17;
        break;
      default:
        DEBUGPANIC();
        return -EINVAL;
    }

  ret = fc7300_get_pclkfreq(clkname, &fcuart_freq);
  DEBUGASSERT(ret >= 0);
  if (ret < 0)
    {
      return ret;
    }

  /* This FCUART instantiation uses a slightly different baud rate
   * calculation.  The idea is to use the best OSR (over-sampling rate)
   * possible.
   *
   * NOTE: OSR is typically hard-set to 16 in other FCUART instantiations
   * loop to find the best OSR value possible, one that generates minimum
   * baud_diff iterate through the rest of the supported values of OSR
   */

  baud_diff = baud;

  for (temp_osr = 32; temp_osr > 3; temp_osr--)
    {
      /* Calculate the temporary sbr value   */

      temp_sbr = (fcuart_freq / (baud * temp_osr));

      /* Set temp_sbr to 1 if the sourceClockInHz can not satisfy the
       * desired baud rate.
       */

      if (temp_sbr == 0)
        {
          temp_sbr = 1;
        }

      /* Calculate the baud rate based on the temporary OSR and SBR values */

      calculated_baud = (fcuart_freq / (temp_osr * temp_sbr));
      temp_diff       = calculated_baud - baud;

      /* Select the better value between srb and (sbr + 1) */

      if (temp_diff >
          (baud - (fcuart_freq / (temp_osr * (temp_sbr + 1)))))
        {
          temp_diff = baud -
                      (fcuart_freq / (temp_osr * (temp_sbr + 1)));
          temp_sbr += 1;
        }

      if (temp_diff < baud_diff)
        {
          baud_diff = temp_diff;
          *osr       = temp_osr;
          *sbr       = temp_sbr;
        }

      if (baud_diff == 0)
        {
          break;
        }
    }

  if (baud_diff > ((baud / 100) * 3))
    {
      /* Unacceptable baud rate difference of more than 3% */

      return ERROR;
    }

  return OK;
}
#endif /* USE_SERIALDRIVER || CONFIG_FC7300_FCUART */

/****************************************************************************
 * Name: fc7300_lowputc
 *
 * Description:
 *   Output a byte with as few system dependencies as possible.  This will
 *   even work BEFORE the console is initialized if we are booting from U-
 *   Boot (and the same UART is used for the console, of course.)
 *
 ****************************************************************************/

#if !defined(CONFIG_NO_FCUART_SERIAL_CONSOLE) && defined(CONFIG_DEBUG_FEATURES)
void fc7300_lowputc(int ch)
{
  while ((getreg32(FC7300_CONSOLE_BASE + FC7300_FCUART_STAT_OFFSET) &
         FCUART_STAT_TCF) == 0)
    {
    }

  /* If the character to output is a newline,
   * then pre-pend a carriage return
   */

  if (ch == '\n')
    {
      /* Send the carriage return by writing it into the UART_TXD register. */

      putreg32((uint32_t)'\r',
                FC7300_CONSOLE_BASE + FC7300_FCUART_DATA_OFFSET);

      /* Wait for the transmit register to be emptied. When the TXFE bit is
       * non-zero, the TX Buffer FIFO is empty.
       */

      while ((getreg32(FC7300_CONSOLE_BASE + FC7300_FCUART_STAT_OFFSET) &
             FCUART_STAT_TCF) == 0)
        {
        }
    }

  /* Send the character by writing it into the UART_TXD register. */

  putreg32((uint32_t)ch, FC7300_CONSOLE_BASE + FC7300_FCUART_DATA_OFFSET);

  /* Wait for the transmit register to be emptied. When the TXFE bit is
   * non-zero, the TX Buffer FIFO is empty.
   */

  while ((getreg32(FC7300_CONSOLE_BASE + FC7300_FCUART_STAT_OFFSET) &
         FCUART_STAT_TCF) == 0)
    {
    }
}
#endif
