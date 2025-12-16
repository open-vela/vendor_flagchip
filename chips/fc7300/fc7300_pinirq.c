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

#include <arch/board/board.h>
#include <nuttx/config.h>

#include <assert.h>
#include <errno.h>
#include <debug.h>

#include <nuttx/irq.h>
#include <nuttx/arch.h>
#include <nuttx/spinlock.h>

#include "arm_internal.h"
#include "hardware/fc7300_pin.h"
#include "hardware/fc7300_port.h"
#include "chip.h"
#include "fc7300_irq.h"
#include "fc7300_multicore.h"

#ifdef CONFIG_FC7300_GPIOIRQ

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Configuration ************************************************************/

/* The fc7300 port interrupt logic is very flexible and will program
 * interrupts on most all pin events.  In order to keep the memory usage to
 * a minimum, the NuttX port supports enabling interrupts on a per-port
 * basis.
 */

#if defined (CONFIG_FC7300_PORTAINTS) || defined (CONFIG_FC7300_PORTBINTS) || \
    defined (CONFIG_FC7300_PORTCINTS) || defined (CONFIG_FC7300_PORTDINTS) || \
    defined (CONFIG_FC7300_PORTEINTS) || defined (CONFIG_FC7300_PORTFINTS) || \
    defined (CONFIG_FC7300_PORTGINTS) || defined (CONFIG_FC7300_PORTHINTS) || \
    defined (CONFIG_FC7300_PORTIINTS)
#  define HAVE_PORTINTS 1
#endif

/****************************************************************************
 * Private Types
 ****************************************************************************/

struct fc7300_pinirq_s
{
  xcpt_t handler;
  void *arg;
};

/****************************************************************************
 * Private Data
 ****************************************************************************/

/* Per pin port interrupt vectors.  NOTE:  Not all pins in each port
 * correspond to externally available GPIOs.  However, I believe that the
 * Kinesis will support interrupts even if the pin is not available as
 * a GPIO. Hence, we need to support all 32 pins for each port.  To keep the
 * memory usage at a minimum, the logic may be configure per port.
 */

#ifdef CONFIG_FC7300_PORTAINTS
static struct fc7300_pinirq_s g_portaisrs[32];
#endif
#ifdef CONFIG_FC7300_PORTBINTS
static struct fc7300_pinirq_s g_portbisrs[32];
#endif
#ifdef CONFIG_FC7300_PORTCINTS
static struct fc7300_pinirq_s g_portcisrs[32];
#endif
#ifdef CONFIG_FC7300_PORTDINTS
static struct fc7300_pinirq_s g_portdisrs[32];
#endif
#ifdef CONFIG_FC7300_PORTEINTS
static struct fc7300_pinirq_s g_porteisrs[32];
#endif
#ifdef CONFIG_FC7300_PORTFINTS
static struct fc7300_pinirq_s g_portfisrs[32];
#endif
#ifdef CONFIG_FC7300_PORTGINTS
static struct fc7300_pinirq_s g_portgisrs[32];
#endif
#ifdef CONFIG_FC7300_PORTHINTS
static struct fc7300_pinirq_s g_porthisrs[32];
#endif
#ifdef CONFIG_FC7300_PORTIINTS
static struct fc7300_pinirq_s g_portiisrs[32];
#endif

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: fc7300_portinterrupt
 *
 * Description:
 *   Common port interrupt handling.
 *
 ****************************************************************************/

#ifdef HAVE_PORTINTS
__ramfunc__ static int fc7300_portinterrupt(int irq, void *context,
                                 uintptr_t addr,
                                 struct fc7300_pinirq_s *isrtab)
{
  uint32_t isfr = getreg32(addr + FC7300_PORT_ISFR_OFFSET);
  uint8_t coreid = fc7300_cpu_index();
  int i;

  /* Examine each pin in the port */

  for (i = 0; i < 32 && isfr != 0; i++)
    {
      uint32_t bit = (1 << i);
      /* A bit set in the ISR means that an interrupt is pending for this
       * pin.  If the pin is programmed for level sensitive inputs, then
       * the interrupt handling logic MUST disable the interrupt (or cause
       * the level to change) to prevent infinite interrupts.
       */
      volatile uint32_t pcr = getreg32(addr + FC7300_PORT_PCR_OFFSET(i));

      if ((pcr & ((1 << coreid) << 25)) == 0)
      {
        if ((isfr & bit) != 0)
        {
          /* That bits may be set in the ISFR for DMA activities
           * well.  So, no error is declared if there is no registered
           * interrupt handler for the pin.
           */

          if (isrtab[i].handler != NULL)
            {
              xcpt_t handler = isrtab[i].handler;
              void  *arg     = isrtab[i].arg;

              /* There is a registered interrupt handler... invoke it */

              handler(irq, context, arg);
            }

          /* Writing a one to the ISFR register will clear the pending
           * interrupt.  If pin is configured to generate a DMA request
           * then the ISFR bit will be cleared automatically at the
           * completion of the requested DMA transfer. If configured for
           * a level sensitive interrupt and the pin remains asserted and
           * the bit will set again immediately after it is cleared.
           */

          isfr &= ~bit;
          putreg32(bit, addr + FC7300_PORT_ISFR_OFFSET);
        }
      }
    }
#ifdef CONFIG_FC7300_GPIO_IRQ_ATTACH_THREAD
  fc7300_clearpendingisr(irq);
  up_enable_irq(irq);
#endif
  return OK;
}
#endif

/****************************************************************************
 * Name: fc7300_portXinterrupt
 *
 * Description:
 *   Handle interrupts arriving on individual ports
 *
 ****************************************************************************/

#ifdef CONFIG_FC7300_PORTAINTS
__ramfunc__ static int fc7300_portainterrupt(int irq, void *context, void *arg)
{
  return fc7300_portinterrupt(irq, context, FC7300_PORTA_BASE,
                               g_portaisrs);
}
#endif

#ifdef CONFIG_FC7300_PORTBINTS
static int fc7300_portbinterrupt(int irq, void *context, void *arg)
{
  return fc7300_portinterrupt(irq, context, FC7300_PORTB_BASE,
                               g_portbisrs);
}
#endif

#ifdef CONFIG_FC7300_PORTCINTS
static int fc7300_portcinterrupt(int irq, void *context, void *arg)
{
  return fc7300_portinterrupt(irq, context, FC7300_PORTC_BASE,
                               g_portcisrs);
}
#endif

#ifdef CONFIG_FC7300_PORTDINTS
static int fc7300_portdinterrupt(int irq, void *context, void *arg)
{
  return fc7300_portinterrupt(irq, context, FC7300_PORTD_BASE,
                               g_portdisrs);
}
#endif

#ifdef CONFIG_FC7300_PORTEINTS
static int fc7300_porteinterrupt(int irq, void *context, void *arg)
{
  return fc7300_portinterrupt(irq, context, FC7300_PORTE_BASE,
                               g_porteisrs);
}
#endif

#ifdef CONFIG_FC7300_PORTFINTS
static int fc7300_portfinterrupt(int irq, void *context, void *arg)
{
  return fc7300_portinterrupt(irq, context, FC7300_PORTF_BASE,
                               g_portfisrs);
}
#endif

#ifdef CONFIG_FC7300_PORTGINTS
static int fc7300_portginterrupt(int irq, void *context, void *arg)
{
  return fc7300_portinterrupt(irq, context, FC7300_PORTG_BASE,
                               g_portgisrs);
}
#endif

#ifdef CONFIG_FC7300_PORTHINTS
static int fc7300_porthinterrupt(int irq, void *context, void *arg)
{
  return fc7300_portinterrupt(irq, context, FC7300_PORTH_BASE,
                               g_porthisrs);
}
#endif

#ifdef CONFIG_FC7300_PORTIINTS
static int fc7300_portiinterrupt(int irq, void *context, void *arg)
{
  return fc7300_portinterrupt(irq, context, FC7300_PORTI_BASE,
                               g_portiisrs);
}
#endif

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: fc7300_gpio_isr_handle
 *
 * Description:
 *   Disable gpion interrupt.
 *
 * Input Parameters:
 *
 * Returned Value: void
 *
 ****************************************************************************/

#ifdef FC7300_GPIO_IRQ_ATTACH_THREAD
__ramfunc__ static int fc7300_gpio_isr_handle(int irq, void *regs, void *arg)
{
  up_disable_irq(irq);
  return IRQ_WAKE_THREAD;
}
#endif

/****************************************************************************
 * Name: fc7300_pinirq_initialize
 *
 * Description:
 *   Initialize logic to support a second level of interrupt decoding for
 *   GPIO pins.
 *
 ****************************************************************************/

void fc7300_pinirq_initialize(void)
{
#ifdef FC7300_GPIO_IRQ_ATTACH_THREAD
#  ifdef CONFIG_FC7300_PORTAINTS
  (void)irq_attach_wqueue(FC7300_IRQ_PORTA, fc7300_gpio_isr_handle,
                          fc7300_portainterrupt, NULL,
                          isr_cfg[GPIO_ISR_THREAD]);
  putreg32(0xffffffff, FC7300_PORTA_ISFR);
  up_enable_irq(FC7300_IRQ_PORTA);
#  endif
#  ifdef CONFIG_FC7300_PORTBINTS
  (void)irq_attach_wqueue(FC7300_IRQ_PORTB, fc7300_gpio_isr_handle,
                          fc7300_portbinterrupt, NULL,
                          isr_cfg[GPIO_ISR_THREAD]);
  putreg32(0xffffffff, FC7300_PORTB_ISFR);
  up_enable_irq(FC7300_IRQ_PORTB);
#  endif
#  ifdef CONFIG_FC7300_PORTCINTS
  (void)irq_attach_wqueue(FC7300_IRQ_PORTC, fc7300_gpio_isr_handle,
                          fc7300_portcinterrupt, NULL,
                          isr_cfg[GPIO_ISR_THREAD]);
  putreg32(0xffffffff, FC7300_PORTC_ISFR);
  up_enable_irq(FC7300_IRQ_PORTC);
#  endif
#  ifdef CONFIG_FC7300_PORTDINTS
  (void)irq_attach_wqueue(FC7300_IRQ_PORTD, fc7300_gpio_isr_handle,
                          fc7300_portdinterrupt, NULL,
                          isr_cfg[GPIO_ISR_THREAD]);
  putreg32(0xffffffff, FC7300_PORTD_ISFR);
  up_enable_irq(FC7300_IRQ_PORTD);
#  endif
#  ifdef CONFIG_FC7300_PORTEINTS
  (void)irq_attach_wqueue(FC7300_IRQ_PORTE, fc7300_gpio_isr_handle,
                          fc7300_porteinterrupt, NULL,
                          isr_cfg[GPIO_ISR_THREAD]);
  putreg32(0xffffffff, FC7300_PORTE_ISFR);
  up_enable_irq(FC7300_IRQ_PORTE);
#  endif
#  ifdef CONFIG_FC7300_PORTFINTS
  (void)irq_attach_wqueue(FC7300_IRQ_PORTF, fc7300_gpio_isr_handle,
                          fc7300_portfinterrupt, NULL,
                          isr_cfg[GPIO_ISR_THREAD]);
  putreg32(0xffffffff, FC7300_PORTF_ISFR);
  up_enable_irq(FC7300_IRQ_PORTF);
#  endif
#  ifdef CONFIG_FC7300_PORTGINTS
  (void)irq_attach_wqueue(FC7300_IRQ_PORTG, fc7300_gpio_isr_handle,
                          fc7300_portginterrupt, NULL,
                          isr_cfg[GPIO_ISR_THREAD]);
  putreg32(0xffffffff, FC7300_PORTG_ISFR);
  up_enable_irq(FC7300_IRQ_PORTG);
#  endif
#  ifdef CONFIG_FC7300_PORTHINTS
  (void)irq_attach_wqueue(FC7300_IRQ_PORTH, fc7300_gpio_isr_handle,
                          fc7300_porthinterrupt, NULL,
                          isr_cfg[GPIO_ISR_THREAD]);
  putreg32(0xffffffff, FC7300_PORTH_ISFR);
  up_enable_irq(FC7300_IRQ_PORTH);
#  endif
#  ifdef CONFIG_FC7300_PORTIINTS
  (void)irq_attach_wqueue(FC7300_IRQ_PORTI, fc7300_gpio_isr_handle,
                          fc7300_portiinterrupt, NULL,
                          isr_cfg[GPIO_ISR_THREAD]);
  putreg32(0xffffffff, FC7300_PORTI_ISFR);
  up_enable_irq(FC7300_IRQ_PORTI);
#  endif
#else
#  ifdef CONFIG_FC7300_PORTAINTS
  irq_attach(FC7300_IRQ_PORTA, fc7300_portainterrupt, NULL);
  putreg32(0xffffffff, FC7300_PORTA_ISFR);
  up_enable_irq(FC7300_IRQ_PORTA);
#  endif
#  ifdef CONFIG_FC7300_PORTBINTS
  irq_attach(FC7300_IRQ_PORTB, fc7300_portbinterrupt, NULL);
  putreg32(0xffffffff, FC7300_PORTB_ISFR);
  up_enable_irq(FC7300_IRQ_PORTB);
#  endif
#  ifdef CONFIG_FC7300_PORTCINTS
  irq_attach(FC7300_IRQ_PORTC, fc7300_portcinterrupt, NULL);
  putreg32(0xffffffff, FC7300_PORTC_ISFR);
  up_enable_irq(FC7300_IRQ_PORTC);
#  endif
#  ifdef CONFIG_FC7300_PORTDINTS
  irq_attach(FC7300_IRQ_PORTD, fc7300_portdinterrupt, NULL);
  putreg32(0xffffffff, FC7300_PORTD_ISFR);
  up_enable_irq(FC7300_IRQ_PORTD);
#  endif
#  ifdef CONFIG_FC7300_PORTEINTS
  irq_attach(FC7300_IRQ_PORTE, fc7300_porteinterrupt, NULL);
  putreg32(0xffffffff, FC7300_PORTE_ISFR);
  up_enable_irq(FC7300_IRQ_PORTE);
#  endif
#  ifdef CONFIG_FC7300_PORTFINTS
  irq_attach(FC7300_IRQ_PORTF, fc7300_portfinterrupt, NULL);
  putreg32(0xffffffff, FC7300_PORTF_ISFR);
  up_enable_irq(FC7300_IRQ_PORTF);
#  endif
#  ifdef CONFIG_FC7300_PORTGINTS
  irq_attach(FC7300_IRQ_PORTG, fc7300_portginterrupt, NULL);
  putreg32(0xffffffff, FC7300_PORTG_ISFR);
  up_enable_irq(FC7300_IRQ_PORTG);
#  endif
#  ifdef CONFIG_FC7300_PORTHINTS
  irq_attach(FC7300_IRQ_PORTH, fc7300_porthinterrupt, NULL);
  putreg32(0xffffffff, FC7300_PORTH_ISFR);
  up_enable_irq(FC7300_IRQ_PORTH);
#  endif
#  ifdef CONFIG_FC7300_PORTIINTS
  irq_attach(FC7300_IRQ_PORTI, fc7300_portiinterrupt, NULL);
  putreg32(0xffffffff, FC7300_PORTI_ISFR);
  up_enable_irq(FC7300_IRQ_PORTI);
#  endif
#endif
}

/****************************************************************************
 * Name: fc7300_pinirq_deinitialize
 *
 * Description:
 *   Initialize logic to support a second level of interrupt decoding for
 *   GPIO pins.
 *
 ****************************************************************************/

void fc7300_pinirq_deinitialize(void)
{
#ifdef CONFIG_FC7300_GPIO_IRQ_ATTACH_THREAD
#  if defined CONFIG_FC7300_PORTAINTS
  up_disable_irq(FC7300_IRQ_PORTA);
  putreg32(0xffffffff, FC7300_PORTA_ISFR);
  (void)irq_detach_wqueue(FC7300_IRQ_PORTA);
#  endif
#  if defined CONFIG_FC7300_PORTBINTS
  up_disable_irq(FC7300_IRQ_PORTB);
  putreg32(0xffffffff, FC7300_PORTB_ISFR);
  (void)irq_detach_wqueue(FC7300_IRQ_PORTB);
#  endif
#  if defined CONFIG_FC7300_PORTCINTS
  up_disable_irq(FC7300_IRQ_PORTC);
  putreg32(0xffffffff, FC7300_PORTC_ISFR);
  (void)irq_detach_wqueue(FC7300_IRQ_PORTC);
#  endif
#  if defined CONFIG_FC7300_PORTDINTS
  up_disable_irq(FC7300_IRQ_PORTD);
  putreg32(0xffffffff, FC7300_PORTD_ISFR);
  (void)irq_detach_wqueue(FC7300_IRQ_PORTD);
#  endif
#  if defined CONFIG_FC7300_PORTEINTS
  up_disable_irq(FC7300_IRQ_PORTE);
  putreg32(0xffffffff, FC7300_PORTE_ISFR);
  (void)irq_detach_wqueue(FC7300_IRQ_PORTE);
#  endif
#  if defined CONFIG_FC7300_PORTFINTS
  up_disable_irq(FC7300_IRQ_PORTF);
  putreg32(0xffffffff, FC7300_PORTF_ISFR);
  (void)irq_detach_wqueue(FC7300_IRQ_PORTF);
#  endif
#  if defined CONFIG_FC7300_PORTGINTS
  up_disable_irq(FC7300_IRQ_PORTG);
  putreg32(0xffffffff, FC7300_PORTG_ISFR);
  (void)irq_detach_wqueue(FC7300_IRQ_PORTG);
#  endif
#  if defined CONFIG_FC7300_PORTHINTS
  up_disable_irq(FC7300_IRQ_PORTH);
  putreg32(0xffffffff, FC7300_PORTH_ISFR);
  (void)irq_detach_wqueue(FC7300_IRQ_PORTH);
#  endif
#  if defined CONFIG_FC7300_PORTIINTS
  up_disable_irq(FC7300_IRQ_PORTI);
  putreg32(0xffffffff, FC7300_PORTI_ISFR);
  (void)irq_detach_wqueue(FC7300_IRQ_PORTI);
#  endif
#else
#  ifdef CONFIG_FC7300_PORTAINTS
  up_disable_irq(FC7300_IRQ_PORTA);
  putreg32(0xffffffff, FC7300_PORTA_ISFR);
  irq_attach(FC7300_IRQ_PORTA, fc7300_portainterrupt, NULL);
#  endif
#  ifdef CONFIG_FC7300_PORTBINTS
  up_disable_irq(FC7300_IRQ_PORTB);
  putreg32(0xffffffff, FC7300_PORTB_ISFR);
  irq_attach(FC7300_IRQ_PORTB, fc7300_portbinterrupt, NULL);
#  endif
#  ifdef CONFIG_FC7300_PORTCINTS
  up_disable_irq(FC7300_IRQ_PORTC);
  putreg32(0xffffffff, FC7300_PORTC_ISFR);
  irq_attach(FC7300_IRQ_PORTC, fc7300_portcinterrupt, NULL);
#  endif
#  ifdef CONFIG_FC7300_PORTDINTS
  up_disable_irq(FC7300_IRQ_PORTD);
  putreg32(0xffffffff, FC7300_PORTD_ISFR);
  irq_attach(FC7300_IRQ_PORTD, fc7300_portdinterrupt, NULL);
#  endif
#  ifdef CONFIG_FC7300_PORTEINTS
  up_disable_irq(FC7300_IRQ_PORTE);
  putreg32(0xffffffff, FC7300_PORTE_ISFR);
  irq_attach(FC7300_IRQ_PORTE, fc7300_porteinterrupt, NULL);
#  endif
#  ifdef CONFIG_FC7300_PORTFINTS
  up_disable_irq(FC7300_IRQ_PORTF);
  putreg32(0xffffffff, FC7300_PORTF_ISFR);
  irq_attach(FC7300_IRQ_PORTF, fc7300_portfinterrupt, NULL);
#  endif
#  ifdef CONFIG_FC7300_PORTGINTS
  up_disable_irq(FC7300_IRQ_PORTG);
  putreg32(0xffffffff, FC7300_PORTG_ISFR);
  irq_attach(FC7300_IRQ_PORTG, fc7300_portginterrupt, NULL);
#  endif
#  ifdef CONFIG_FC7300_PORTHINTS
  up_disable_irq(FC7300_IRQ_PORTH);
  putreg32(0xffffffff, FC7300_PORTH_ISFR);
  irq_attach(FC7300_IRQ_PORTH, fc7300_porthinterrupt, NULL);
#  endif
#  ifdef CONFIG_FC7300_PORTIINTS
  up_disable_irq(FC7300_IRQ_PORTI);
  putreg32(0xffffffff, FC7300_PORTI_ISFR);
  irq_attach(FC7300_IRQ_PORTI, fc7300_portiinterrupt, NULL);
#  endif
#endif /* CONFIG_FC7300_GPIO_IRQ_ATTACH_THREAD */
}

/****************************************************************************
 * Name: fc7300_pinirqattach
 *
 * Description:
 *   Attach a pin interrupt handler.  The normal initialization sequence is:
 *
 *   1. Call fc7300_pinconfig() to configure the interrupting pin (pin
 *      interrupts will be disabled.
 *   2. Call fc7300_pinirqattach() to attach the pin interrupt handling
 *      function.
 *   3. Call fc7300_pinirqenable() to enable interrupts on the pin.
 *
 * Input Parameters:
 *   pinset - Pin configuration
 *   pinisr - Pin interrupt service routine
 *   arg    - An argument that will be provided to the interrupt service
 *            routine.
 *
 * Returned Value:
 *   Zero (OK) is returned on success; a negated errno value is returned on
 *   any failure to indicate the nature of the failure.
 *
 ****************************************************************************/

int fc7300_pinirqattach(uint32_t pinset, xcpt_t pinisr, void *arg)
{
#ifdef HAVE_PORTINTS
  struct fc7300_pinirq_s *isrtab;
  irqstate_t flags;
  unsigned int port;
  unsigned int pin;

  /* It only makes sense to call this function for input pins that are
   * configured as interrupts.
   */

  DEBUGASSERT((pinset & _PIN_INTERRUPT) == _PIN_INTERRUPT);
  DEBUGASSERT((pinset & _PIN_IO_MASK) == _PIN_INPUT);

  /* Get the port number and pin number */

  port = (pinset & _PIN_PORT_MASK) >> _PIN_PORT_SHIFT;
  pin  = (pinset & _PIN_MASK)      >> _PIN_SHIFT;

  /* Get the table associated with this port */

  DEBUGASSERT(port < FC7300_NPORTS);
  flags = enter_critical_section();
  switch (port)
    {
#ifdef CONFIG_FC7300_PORTAINTS
      case FC7300_PORTA :
        isrtab = g_portaisrs;
        break;
#endif
#ifdef CONFIG_FC7300_PORTBINTS
      case FC7300_PORTB :
        isrtab = g_portbisrs;
        break;
#endif
#ifdef CONFIG_FC7300_PORTCINTS
      case FC7300_PORTC :
        isrtab = g_portcisrs;
        break;
#endif
#ifdef CONFIG_FC7300_PORTDINTS
      case FC7300_PORTD :
        isrtab = g_portdisrs;
        break;
#endif
#ifdef CONFIG_FC7300_PORTEINTS
      case FC7300_PORTE :
        isrtab = g_porteisrs;
        break;
#endif
#ifdef CONFIG_FC7300_PORTFINTS
      case FC7300_PORTF :
        isrtab = g_portfisrs;
        break;
#endif
#ifdef CONFIG_FC7300_PORTGINTS
      case FC7300_PORTG :
        isrtab = g_portgisrs;
        break;
#endif
#ifdef CONFIG_FC7300_PORTHINTS
      case FC7300_PORTH :
        isrtab = g_porthisrs;
        break;
#endif
#ifdef CONFIG_FC7300_PORTIINTS
      case FC7300_PORTI :
        isrtab = g_portiisrs;
        break;
#endif
      default:
        leave_critical_section(flags);
        return -EINVAL;
    }

  /* Get the old PIN ISR and set the new PIN ISR */

  isrtab[pin].handler = pinisr;
  isrtab[pin].arg     = arg;

  /* And return the old PIN isr address */

  leave_critical_section(flags);
  return OK;
#else
  return -ENOSYS;
#endif /* HAVE_PORTINTS */
}

/****************************************************************************
 * Name: fc7300_pinirqenable
 *
 * Description:
 *   Enable the interrupt for specified pin IRQ
 *
 ****************************************************************************/

void fc7300_pinirqenable(uint32_t pinset)
{
#ifdef HAVE_PORTINTS
  uintptr_t    base;
  uint32_t     regval;
  unsigned int port;
  unsigned int pin;

  /* Get the port number and pin number */

  port = (pinset & _PIN_PORT_MASK) >> _PIN_PORT_SHIFT;
  pin  = (pinset & _PIN_MASK)      >> _PIN_SHIFT;

  DEBUGASSERT(port < FC7300_NPORTS);
  if (port < FC7300_NPORTS)
    {
      /* Get the base address of PORT block for this port */

      base =  FC7300_PORT_BASE(port);

      /* Modify the IRQC field of the port PCR register in order to enable
       * the interrupt.
       */

      regval = getreg32(base + FC7300_PORT_PCR_OFFSET(pin));
      regval &= ~PORT_PCR_IRQC_MASK;

      switch (pinset & _PIN_INT_MASK)
        {
          case PIN_INT_ZERO : /* Interrupt when logic zero */
            regval |= PORT_PCR_IRQC_ZERO;
            break;

          case PIN_INT_RISING : /* Interrupt on rising edge */
            regval |= PORT_PCR_IRQC_RISING;
            break;

          case PIN_INT_FALLING : /* Interrupt on falling edge */
            regval |= PORT_PCR_IRQC_FALLING;
            break;

          case PIN_INT_BOTH : /* Interrupt on either edge */
            regval |= PORT_PCR_IRQC_BOTH;
            break;

          case PIN_INT_ONE : /* Interrupt when logic one */
            regval |= PORT_PCR_IRQC_ONE;
            break;

          case PIN_DMA_RISING : /* DMA on rising edge */
            regval |= PORT_PCR_IRQC_DMARISING;
            break;

          case PIN_DMA_FALLING : /* DMA on falling edge */
            regval |= PORT_PCR_IRQC_DMAFALLING;
            break;

          case PIN_DMA_BOTH : /* DMA on either edge */
            regval |= PORT_PCR_IRQC_DMABOTH;
            break;

          default:
            return;
        }
      /* Config for core ctrl and isr entered.
        * 13 means 25 (reg dwp filed) - 12 (Bit-encoded for gpio) */

      regval |= (pinset & _PIN_DOMAIN_CTRL_MASK) << 13;

      putreg32(regval, base + FC7300_PORT_PCR_OFFSET(pin));

      switch (port)
        {
#if defined CONFIG_FC7300_PORTAINTS
          case FC7300_PORTA :
#ifdef CONFIG_ARCH_IRQPRIO
            up_prioritize_irq(FC7300_IRQ_PORTA, NVIC_PORTA_PRIORITY_DEFAULT);
#endif
            up_enable_irq(FC7300_IRQ_PORTA);
            break;
#endif
#if defined CONFIG_FC7300_PORTBINTS
          case FC7300_PORTB :
#ifdef CONFIG_ARCH_IRQPRIO
            up_prioritize_irq(FC7300_IRQ_PORTB, NVIC_PORTB_PRIORITY_DEFAULT);
#endif
            up_enable_irq(FC7300_IRQ_PORTB);
            break;
#endif
#if defined CONFIG_FC7300_PORTCINTS
          case FC7300_PORTC :
#ifdef CONFIG_ARCH_IRQPRIO
            up_prioritize_irq(FC7300_IRQ_PORTC, NVIC_PORTC_PRIORITY_DEFAULT);
#endif
            up_enable_irq(FC7300_IRQ_PORTC);
            break;
#endif
#if defined CONFIG_FC7300_PORTDINTS
          case FC7300_PORTD :
#ifdef CONFIG_ARCH_IRQPRIO
            up_prioritize_irq(FC7300_IRQ_PORTD, NVIC_PORTD_PRIORITY_DEFAULT);
#endif
            up_enable_irq(FC7300_IRQ_PORTD);
            break;
#endif
#if defined CONFIG_FC7300_PORTEINTS
          case FC7300_PORTE :
#ifdef CONFIG_ARCH_IRQPRIO
            up_prioritize_irq(FC7300_IRQ_PORTE, NVIC_PORTE_PRIORITY_DEFAULT);
#endif
            up_enable_irq(FC7300_IRQ_PORTE);
            break;
#endif
#if defined CONFIG_FC7300_PORTFINTS
          case FC7300_PORTF :
#ifdef CONFIG_ARCH_IRQPRIO
            up_prioritize_irq(FC7300_IRQ_PORTF, NVIC_PORTF_PRIORITY_DEFAULT);
#endif
            up_enable_irq(FC7300_IRQ_PORTF);
            break;
#endif
#if defined CONFIG_FC7300_PORTGINTS
          case FC7300_PORTG :
#ifdef CONFIG_ARCH_IRQPRIO
            up_prioritize_irq(FC7300_IRQ_PORTG, NVIC_PORTG_PRIORITY_DEFAULT);
#endif
            up_enable_irq(FC7300_IRQ_PORTG);
            break;
#endif
#if defined CONFIG_FC7300_PORTHINTS
          case FC7300_PORTH :
#ifdef CONFIG_ARCH_IRQPRIO
            up_prioritize_irq(FC7300_IRQ_PORTH, NVIC_PORTH_PRIORITY_DEFAULT);
#endif
            up_enable_irq(FC7300_IRQ_PORTH);
            break;
#endif
#if defined CONFIG_FC7300_PORTIINTS
          case FC7300_PORTI :
#ifdef CONFIG_ARCH_IRQPRIO
            up_prioritize_irq(FC7300_IRQ_PORTI, NVIC_PORTI_PRIORITY_DEFAULT);
#endif
            up_enable_irq(FC7300_IRQ_PORTI);
            break;
#endif
          default:
            return;
        }
    }
#endif /* HAVE_PORTINTS */
}

/****************************************************************************
 * Name: fc7300_pinirqdisable
 *
 * Description:
 *   Disable the interrupt for specified pin
 *
 ****************************************************************************/

void fc7300_pinirqdisable(uint32_t pinset)
{
#ifdef HAVE_PORTINTS
  uintptr_t    base;
  uint32_t     regval;
  unsigned int port;
  unsigned int pin;

  /* Get the port number and pin number */

  port = (pinset & _PIN_PORT_MASK) >> _PIN_PORT_SHIFT;
  pin  = (pinset & _PIN_MASK)      >> _PIN_SHIFT;

  DEBUGASSERT(port < FC7300_NPORTS);
  if (port < FC7300_NPORTS)
    {
      /* Get the base address of PORT block for this port */

      base =  FC7300_PORT_BASE(port);

      /* Clear the IRQC field of the port PCR register in order to disable
       * the interrupt.
       */

      regval = getreg32(base + FC7300_PORT_PCR_OFFSET(pin));
      regval &= ~PORT_PCR_IRQC_MASK;
      putreg32(regval, base + FC7300_PORT_PCR_OFFSET(pin));
    }
#endif /* HAVE_PORTINTS */
}
#endif /* CONFIG_FC7300_GPIOIRQ */
