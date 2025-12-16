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
#include <stdbool.h>
#include <stdint.h>
#include <assert.h>
#include <debug.h>
#include <errno.h>

#include <nuttx/ioexpander/gpio.h>

#include "fc7300f8m-evb.h"

#if defined(CONFIG_DEV_GPIO) && !defined(CONFIG_GPIO_LOWER_HALF)

/****************************************************************************
 * Private Types
 ****************************************************************************/

struct fc7300_gpio_dev_s
{
  struct gpio_dev_s gpio;
  uint8_t id;
  pin_interrupt_t callback;
};

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

static int gpio_read(struct gpio_dev_s *dev, bool *value);
static int gpio_write(struct gpio_dev_s *dev, bool value);
#ifdef CONFIG_FC7300_GPIOIRQ
static int fc7300_gpio_interrupt(int irq, void *context, void *arg);
static int gpio_irqattach(struct gpio_dev_s *dev, pin_interrupt_t callback);
static int gpio_irqenable(struct gpio_dev_s *dev, bool enable);
#endif /* CONFIG_FC7300_GPIOIRQ */
static int gpio_setpintype(struct gpio_dev_s *dev,
                           enum gpio_pintype_e pintype);

/****************************************************************************
 * Private Data
 ****************************************************************************/

/* Set of GPIO pins */

static uint32_t g_gpiopins[NUM_OF_GPIO] =
{
  GPIO0,
  GPIO1,
};

#if NUM_OF_GPIO > 0
static struct fc7300_gpio_dev_s g_gpio[NUM_OF_GPIO];
#endif

/* Different pin types support different operations */

static const struct gpio_operations_s gpin_ops =
{
  .go_read       = gpio_read,
  .go_write      = NULL,
  .go_attach     = NULL,
  .go_enable     = NULL,
  .go_setpintype = gpio_setpintype,
};

static const struct gpio_operations_s gpout_ops =
{
  .go_read       = gpio_read,
  .go_write      = gpio_write,
  .go_attach     = NULL,
  .go_enable     = NULL,
  .go_setpintype = gpio_setpintype,
};

#ifdef CONFIG_FC7300_GPIOIRQ
static const struct gpio_operations_s gpint_ops =
{
  .go_read       = gpio_read,
  .go_write      = NULL,
  .go_attach     = gpio_irqattach,
  .go_enable     = gpio_irqenable,
  .go_setpintype = gpio_setpintype,
};
#endif /* CONFIG_FC7300_GPIOIRQ */

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: gpio_read
 *
 * Description:
 *   Read one or zero from the selected GPIO pin.
 *
 ****************************************************************************/

static int gpio_read(struct gpio_dev_s *dev, bool *value)
{
  struct fc7300_gpio_dev_s *fc7300_gpio = (struct fc7300_gpio_dev_s *)dev;

  DEBUGASSERT(fc7300_gpio != NULL && value != NULL);
  DEBUGASSERT(fc7300_gpio->id < NUM_OF_GPIO);
  gpioinfo("Reading...\n");

  *value = fc7300_gpioread(g_gpiopins[fc7300_gpio->id]);
  return OK;
}

/****************************************************************************
 * Name: gpio_write
 *
 * Description:
 *   Write one or zero to the selected GPIO pin.
 *
 ****************************************************************************/

static int gpio_write(struct gpio_dev_s *dev, bool value)
{
  struct fc7300_gpio_dev_s *fc7300_gpio = (struct fc7300_gpio_dev_s *)dev;

  DEBUGASSERT(fc7300_gpio != NULL);
  DEBUGASSERT(fc7300_gpio->id < NUM_OF_GPIO);
  gpioinfo("Writing %d\n", (int) value);

  fc7300_gpiowrite(g_gpiopins[fc7300_gpio->id], value);
  return OK;
}

#ifdef CONFIG_FC7300_GPIOIRQ

/****************************************************************************
 * Name: fc7300_gpio_interrupt
 *
 * Description:
 *   This function will be attached as irq function,
 *   and used to call the callback function.
 *
 ****************************************************************************/

static int fc7300_gpio_interrupt(int irq, void *context, void *arg)
{
  struct fc7300_gpio_dev_s *fc7300_gpio = (struct fc7300_gpio_dev_s *)arg;

  DEBUGASSERT(fc7300_gpio != NULL && fc7300_gpio->callback != NULL);
  gpioinfo("Interrupt! callback=%p\n", fc7300_gpio->callback);

  fc7300_gpio->callback(&fc7300_gpio->gpio, fc7300_gpio->id);
  return OK;
}

/****************************************************************************
 * Name: gpio_irqattach
 *
 * Description:
 *   Attach the specific callback function.
 *
 ****************************************************************************/

static int gpio_irqattach(struct gpio_dev_s *dev, pin_interrupt_t callback)
{
  struct fc7300_gpio_dev_s *fc7300_gpio = (struct fc7300_gpio_dev_s *)dev;

  gpioinfo("Attaching the callback\n");
  fc7300_pinirqattach(g_gpiopins[fc7300_gpio->id], fc7300_gpio_interrupt,
                       &g_gpio[fc7300_gpio->id]);

  gpioinfo("Attach %p\n", callback);
  fc7300_gpio->callback = callback;
  return OK;
}

/****************************************************************************
 * Name: gpio_irqenable
 *
 * Description:
 *   Enable the interrupt for specified pin IRQ.
 *
 ****************************************************************************/

static int gpio_irqenable(struct gpio_dev_s *dev, bool enable)
{
  struct fc7300_gpio_dev_s *fc7300_gpio = (struct fc7300_gpio_dev_s *)dev;

  DEBUGASSERT(fc7300_gpio != NULL);

  if (enable)
    {
      if (fc7300_gpio->callback != NULL)
        {
          gpioinfo("Enabling the interrupt\n");
          fc7300_pinirqenable(g_gpiopins[fc7300_gpio->id]);
        }
    }
  else
    {
      gpioinfo("Disable the interrupt\n");
      fc7300_pinirqdisable(g_gpiopins[fc7300_gpio->id]);
    }

  return OK;
}
#endif /* CONFIG_FC7300_GPIOIRQ */

/****************************************************************************
 * Name: gpio_setpintype
 *
 * Description:
 *   Select the pin type.
 *
 ****************************************************************************/

static int gpio_setpintype(struct gpio_dev_s *dev,
                           enum gpio_pintype_e pintype)
{
  int ret = OK;
  uint32_t pinconfig;
  const struct gpio_operations_s *gpio_ops;
  struct fc7300_gpio_dev_s *fc7300_gpio = (struct fc7300_gpio_dev_s *)dev;

  DEBUGASSERT(fc7300_gpio != NULL);
  DEBUGASSERT(fc7300_gpio->id < NUM_OF_GPIO);
  gpioinfo("Setpintype...\n");

  /* Check if the new pintype is actually different from the old pintype */

  if (fc7300_gpio->gpio.gp_pintype == pintype)
    {
      /* Pintype has not changed. We're done already. */

      return ret;
    }

  /* Clear the pin mode, pin options and interrupt options */

  pinconfig = (g_gpiopins[fc7300_gpio->id] &
              ~(_PIN_MODE_MASK | _PIN_OPTIONS_MASK | _PIN_INT_MASK));

  /* Set the pinconfig and device operations according to the new pintype */

  switch (pintype)
    {
      case GPIO_INPUT_PIN:
        {
          pinconfig |= GPIO_INPUT;
          gpio_ops = &gpin_ops;
        }
        break;

      case GPIO_INPUT_PIN_PULLUP:
        {
          pinconfig |= GPIO_PULLUP;
          gpio_ops = &gpin_ops;
        }
        break;

      case GPIO_INPUT_PIN_PULLDOWN:
        {
          pinconfig |= GPIO_PULLDOWN;
          gpio_ops = &gpin_ops;
        }
        break;

      case GPIO_OUTPUT_PIN:
        {
          pinconfig |= GPIO_OUTPUT;
          gpio_ops = &gpout_ops;
        }
        break;

#ifdef CONFIG_FC7300_GPIOIRQ
      case GPIO_INTERRUPT_HIGH_PIN:
        {
          pinconfig |= (GPIO_INPUT | PIN_INT_ONE);
          gpio_ops = &gpint_ops;
        }
        break;

      case GPIO_INTERRUPT_LOW_PIN:
        {
          pinconfig |= (GPIO_INPUT | PIN_INT_ZERO);
          gpio_ops = &gpint_ops;
        }
        break;

      case GPIO_INTERRUPT_RISING_PIN:
        {
          pinconfig |= (GPIO_INPUT | PIN_INT_RISING);
          gpio_ops = &gpint_ops;
        }
        break;

      case GPIO_INTERRUPT_FALLING_PIN:
        {
          pinconfig |= (GPIO_INPUT | PIN_INT_FALLING);
          gpio_ops = &gpint_ops;
        }
        break;

      case GPIO_INTERRUPT_BOTH_PIN:
      case GPIO_INTERRUPT_PIN:
        {
          pinconfig |= (GPIO_INPUT | PIN_INT_BOTH);
          gpio_ops = &gpint_ops;
        }
        break;
#endif /* CONFIG_FC7300_GPIOIRQ */

      default:
        {
          /* Not implemented yet! */

          return -EINVAL; /* Return without changing the pin settings */
        }
        break;
    }

#ifdef CONFIG_FC7300_GPIOIRQ

  /* If the pin previously had an interrupt pintype... */

  if ((fc7300_gpio->gpio.gp_pintype >= GPIO_INTERRUPT_PIN) &&
      (fc7300_gpio->gpio.gp_pintype < GPIO_NPINTYPES))
    {
      /* ...disable the interrupt... */

      ret = gpio_irqenable(dev, false);
      if (ret < 0)
        {
          return ret;
        }

      /* ...and detach the old callback. */

      ret = gpio_irqattach(dev, NULL);
      if (ret < 0)
        {
          return ret;
        }
    }
#endif /* CONFIG_FC7300_GPIOIRQ */

  /* Change the pintype and set of operations */

  fc7300_gpio->gpio.gp_pintype = pintype;
  fc7300_gpio->gpio.gp_ops = gpio_ops;

  /* Reconfigure the actual pin */

  g_gpiopins[fc7300_gpio->id] = pinconfig;
  ret = fc7300_pinconfig(pinconfig);

  return ret;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: fc7300_gpio_initialize
 *
 * Description:
 *   Initialize GPIO drivers for use with /apps/examples/gpio
 *
 ****************************************************************************/

int fc7300_gpio_initialize(void)
{
  int ret = OK;

#if NUM_OF_GPIO > 0
  int i;
  uint32_t pinset;

  for (i = 0; i < NUM_OF_GPIO; i++)
    {
      DEBUGASSERT((g_gpiopins[i] & (_PIN_MODE_MASK)) == _PIN_MODE_GPIO);

      g_gpio[i].id = i;

      /* Find which pin type we are dealing with */

      pinset = g_gpiopins[i];

      if ((pinset & (_PIN_IO_MASK)) == _PIN_INPUT)
        {
#  ifdef CONFIG_FC7300_GPIOIRQ
          /* Input pin (with or without interrupt) */

          if (pinset & (_PIN_INTERRUPT))
            {
              /* Interrupt pin */

              g_gpio[i].gpio.gp_ops = &gpint_ops;

              /* Determine specific interrupt pin type */

              switch (pinset & (_PIN_INT_MASK))
                {
                  case PIN_INT_ONE:
                    {
                      g_gpio[i].gpio.gp_pintype = GPIO_INTERRUPT_HIGH_PIN;
                    }
                    break;

                  case PIN_INT_ZERO:
                    {
                      g_gpio[i].gpio.gp_pintype = GPIO_INTERRUPT_LOW_PIN;
                    }
                    break;

                  case PIN_INT_RISING:
                    {
                      g_gpio[i].gpio.gp_pintype = GPIO_INTERRUPT_RISING_PIN;
                    }
                    break;

                  case PIN_INT_FALLING:
                    {
                      g_gpio[i].gpio.gp_pintype = GPIO_INTERRUPT_FALLING_PIN;
                    }
                    break;

                  case PIN_INT_BOTH:
                    {
                      g_gpio[i].gpio.gp_pintype = GPIO_INTERRUPT_BOTH_PIN;
                    }
                    break;

                  default:
                    {
                      g_gpio[i].gpio.gp_pintype = GPIO_INTERRUPT_PIN;
                    }
                    break;
                }
            }
          else
#  endif /* CONFIG_FC7300_GPIOIRQ */
            {
              /* Input pin without interrupt */

              g_gpio[i].gpio.gp_ops = &gpin_ops;

              /* Determine specific input pin type */

              switch (pinset & (_PIN_INPUT_PULLMASK))
                {
                  case _PIN_INPUT_PULLUP:
                    {
                      g_gpio[i].gpio.gp_pintype = GPIO_INPUT_PIN_PULLUP;
                    }
                    break;

                  case _PIN_INPUT_PULLDOWN:
                    {
                      g_gpio[i].gpio.gp_pintype = GPIO_INPUT_PIN_PULLDOWN;
                    }
                    break;

                  default:
                    {
                      g_gpio[i].gpio.gp_pintype = GPIO_INPUT_PIN;
                    }
                    break;
                }
            }
        }
      else
        {
          /* Output pin */

          g_gpio[i].gpio.gp_ops = &gpout_ops;
          g_gpio[i].gpio.gp_pintype = GPIO_OUTPUT_PIN;
        }

      /* Configure and register the GPIO pin */

      ret = fc7300_pinconfig(g_gpiopins[i]);
      if (ret < 0)
        {
          return ret;
        }

      ret = gpio_pin_register(&g_gpio[i].gpio, i);
      if (ret < 0)
        {
          return ret;
        }
    }
#endif

  return ret;
}
#endif /* CONFIG_DEV_GPIO && !CONFIG_GPIO_LOWER_HALF */
