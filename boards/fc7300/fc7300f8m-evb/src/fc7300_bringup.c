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
#include <stdint.h>
#include <syslog.h>

#ifdef CONFIG_INPUT_BUTTONS
#  include <nuttx/input/buttons.h>
#endif

#ifdef CONFIG_USERLED
#  include <nuttx/leds/userled.h>
#endif

#ifdef CONFIG_FC7300_PWM
#  include "fc7300_pwm.h"
extern int fc7300_pwm_setup(void);
#endif

#ifdef CONFIG_CAPTURE
#  include "fc7300_capture.h"
#endif

#ifdef CONFIG_FS_PROCFS
#  include <nuttx/fs/fs.h>
#endif

#ifdef CONFIG_FC7300_PROGMEM
#  include <nuttx/mtd/mtd.h>
# include <nuttx/mtd/configdata.h>
#endif

#ifdef CONFIG_ADC
#  include "fc7300_adc.h"
#endif

#ifdef CONFIG_WATCHDOG
#   include "fc7300_wdog.h"
extern int fc7300_wdog_setup(void);
#endif

#if defined(CONFIG_NET_CAN) && defined(CONFIG_NETDEV_LATEINIT)
extern int fc7300_flexcan_socket_setup(void);
#endif

#ifdef CONFIG_CAN
extern int fc7300_flexcan_chardriver_setup(void);
#endif

#ifdef CONFIG_SPI_DRIVER
#  include "fc7300_fcspi.h"
#endif

#ifdef CONFIG_FC7300_FCIIC
#  include "fc7300_fciic.h"
#endif

#ifdef CONFIG_FC7300_ONESHOT
#include <nuttx/timers/oneshot.h>
#endif

#ifdef CONFIG_FC7300_LIN
#  include "fc7300_lin.h"
#endif

#ifdef CONFIG_FC7300_ENET
#include "fc7300_enet.h"
#endif

#ifdef CONFIG_FC7300_NORFLASH_MTD
#  include "fc7300_partition.h"
#endif

#ifdef CONFIG_TIMER
#  include "fc7300_fcpit.h"
#endif

#ifdef CONFIG_FC7300_HWSPINLOCK
#  include "fc7300_hwspinlock.h"
#endif

#include "fc7300f8m-evb.h"

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: fc7300_bringup
 *
 * Description:
 *   Perform architecture-specific initialization
 *
 *   CONFIG_BOARD_LATE_INITIALIZE=y :
 *     Called from board_late_initialize().
 *
 *   CONFIG_BOARD_LATE_INITIALIZE=n && CONFIG_BOARDCTL=y :
 *     Called from the NSH library
 *
 ****************************************************************************/

int fc7300_bringup(void)
{
  int ret = OK;

#if defined(CONFIG_DEV_GPIO) && !defined(CONFIG_GPIO_LOWER_HALF)
  /* Initialize and register the GPIO driver */

  ret = fc7300_gpio_initialize();
  if (ret < 0)
    {
      syslog(LOG_ERR, "ERROR: Failed to initialize GPIO driver: %d\n", ret);
    }
#endif /* CONFIG_DEV_GPIO && !CONFIG_GPIO_LOWER_HALF */

#ifdef CONFIG_INPUT_BUTTONS
  /* Register the BUTTON driver */

  ret = btn_lower_initialize("/dev/buttons");
  if (ret < 0)
    {
      syslog(LOG_ERR, "ERROR: btn_lower_initialize() failed: %d\n", ret);
    }
#endif

#ifdef CONFIG_USERLED
  /* Register the LED driver */

  ret = userled_lower_initialize("/dev/userleds");
  if (ret < 0)
    {
      syslog(LOG_ERR, "ERROR: userled_lower_initialize() failed: %d\n", ret);
    }
#endif

#if defined(CONFIG_FC7300_FCPIT1_CH0)

  ret = fc7300_timer_driver_setup("/dev/timer2", 4);
  if (ret < 0)
    {
      syslog(LOG_ERR, "ERROR: fc7300_timer_driver_setup timer2 failed.\n");
    }
#endif

#if defined(CONFIG_FC7300_FCPIT1_CH1)

  ret = fc7300_timer_driver_setup("/dev/timer1", 5);
  if (ret < 0)
    {
      syslog(LOG_ERR, "ERROR: fc7300_timer_driver_setup timer1 failed.\n");
    }
#endif

#if defined(CONFIG_FC7300_FCPIT1_CH2)

  ret = fc7300_timer_driver_setup("/dev/timer2", 6);
  if (ret < 0)
    {
      syslog(LOG_ERR, "ERROR: fc7300_timer_driver_setup timer2 failed.\n");
    }
#endif

#ifdef CONFIG_FC7300_PWM
  /* Initialize PWM and register the PWM device. */

  ret = fc7300_pwm_initialize();
  if (ret < 0)
    {
      syslog(LOG_ERR, "ERROR: fc7300_pwm_setup() failed: %d\n", ret);
    }
#endif

#ifdef CONFIG_FC7300_NORFLASH_MTD
  fc7300_partition_init();
#endif

#if defined(CONFIG_NET_CAN) && defined(CONFIG_NETDEV_LATEINIT)
  fc7300_flexcan_socket_setup();
#  ifdef CONFIG_FC7300_LIN0
  fc7300_lininitialize(0);
#  endif /* CONFIG_FC7300_LIN0 */
#  ifdef CONFIG_FC7300_LIN1
  fc7300_lininitialize(1);
#  endif /* CONFIG_FC7300_LIN1 */
#  ifdef CONFIG_FC7300_LIN2
  fc7300_lininitialize(2);
#  endif /* CONFIG_FC7300_LIN2 */
#  ifdef CONFIG_FC7300_LIN3
  fc7300_lininitialize(3);
#  endif /* CONFIG_FC7300_LIN3 */
#  ifdef CONFIG_FC7300_LIN4
  fc7300_lininitialize(4);
#  endif /* CONFIG_FC7300_LIN4 */
#  ifdef CONFIG_FC7300_LIN5
  fc7300_lininitialize(5);
#  endif /* CONFIG_FC7300_LIN5 */
#  ifdef CONFIG_FC7300_LIN6
  fc7300_lininitialize(6);
#  endif /* CONFIG_FC7300_LIN6 */
#  ifdef CONFIG_FC7300_LIN7
  fc7300_lininitialize(7);
#  endif /* CONFIG_FC7300_LIN7 */
#  ifdef CONFIG_FC7300_LIN8
  fc7300_lininitialize(8);
#  endif /* CONFIG_FC7300_LIN8 */
#  ifdef CONFIG_FC7300_LIN9
  fc7300_lininitialize(9);
#  endif /* CONFIG_FC7300_LIN9 */
#  ifdef CONFIG_FC7300_LIN10
  fc7300_lininitialize(10);
#  endif /* CONFIG_FC7300_LIN10 */
#  ifdef CONFIG_FC7300_LIN11
  fc7300_lininitialize(11);
#  endif /* CONFIG_FC7300_LIN11 */
#  ifdef CONFIG_FC7300_LIN12
  fc7300_lininitialize(12);
#  endif /* CONFIG_FC7300_LIN12 */
#  ifdef CONFIG_FC7300_LIN13
  fc7300_lininitialize(13);
#  endif /* CONFIG_FC7300_LIN13 */
#  ifdef CONFIG_FC7300_LIN14
  fc7300_lininitialize(14);
#  endif /* CONFIG_FC7300_LIN14 */
#  ifdef CONFIG_FC7300_LIN15
  fc7300_lininitialize(15);
#  endif /* CONFIG_FC7300_LIN15 */
#  ifdef CONFIG_FC7300_LIN16
  fc7300_lininitialize(16);
#  endif /* CONFIG_FC7300_LIN16 */
#  ifdef CONFIG_FC7300_LIN17
  fc7300_lininitialize(17);
#  endif /* CONFIG_FC7300_LIN17 */
#  ifdef CONFIG_FC7300_ENET0
  fc7300_enetinitialize(0);
#  endif
#endif /* CONFIG_NETDEV_LATEINIT */

#ifdef CONFIG_FC7300_HWSPINLOCK
  fc7300_hwspinlock_initialize();
#endif

#ifdef CONFIG_CAN
  fc7300_flexcan_chardriver_setup();
#endif

#ifdef CONFIG_FC7300_FCIIC
  /* EEPROM WP Pin */

  fc7300_pinconfig(PIN_ALT1_OUTPUT | PIN_PORTE | PIN6 | GPIO_OUTPUT_ZERO);

#ifdef CONFIG_FC7300_FCIIC0
  struct i2c_master_s *i2c_bus0;
  i2c_bus0 = fc7300_i2cbus_initialize(0);
  if (i2c_bus0 != NULL)
    {
      ret = i2c_register(i2c_bus0, 0);
      if (ret < 0)
        {
          syslog(LOG_ERR, "ERROR: IIC bus0 register failed\n");
        }
    }
  else
    {
      syslog(LOG_ERR, "ERROR: IIC bus0 initialize failed\n");
    }
#endif /* CONFIG_FC7300_FCIIC0 */

#ifdef CONFIG_FC7300_FCIIC1
  struct i2c_master_s *i2c_bus1;
  i2c_bus1 = fc7300_i2cbus_initialize(1);
  if (i2c_bus1 != NULL)
    {
      ret = i2c_register(i2c_bus1, 1);
      if (ret < 0)
        {
          syslog(LOG_ERR, "ERROR: IIC bus1 register failed\n");
        }
    }
  else
    {
      syslog(LOG_ERR, "ERROR: IIC bus1 initialize failed\n");
    }
#endif /* CONFIG_FC7300_FCIIC1 */

#endif /* CONFIG_FC7300_FCIIC */

#ifdef CONFIG_FC7300_ONESHOT
#if (defined(CONFIG_FC7300_TSTMP1_CH0) && \
     !defined(CONFIG_FC7300_TSTMP1_CH0_USED_FOR_TICK)) || \
    (defined(CONFIG_FC7300_TSTMP2_CH0) && \
     !defined(CONFIG_FC7300_TSTMP2_CH0_USED_FOR_TICK)) || \
    (defined(CONFIG_FC7300_TSTMP3_CH0) && \
     !defined(CONFIG_FC7300_TSTMP3_CH0_USED_FOR_TICK))
  struct oneshot_lowerhalf_s *oneshot_dev0;
  oneshot_dev0 = oneshot_initialize(0, 1);
  if (oneshot_dev0 != NULL)
    {
      ret = oneshot_register("/dev/oneshot0", oneshot_dev0);
      if (ret < 0)
        {
          syslog(LOG_ERR, "ERROR: Failed to register oneshot0\n");
        }
    }
  else
    {
      syslog(LOG_ERR, "ERROR: oneshot0 initialize failed\n");
    }
#endif

#if defined(CONFIG_FC7300_TSTMP1_CH1) || \
    defined(CONFIG_FC7300_TSTMP2_CH1) || \
    defined(CONFIG_FC7300_TSTMP3_CH1)
  struct oneshot_lowerhalf_s *oneshot_dev1;
  oneshot_dev1 = oneshot_initialize(1, 1);
  if (oneshot_dev1 != NULL)
    {
      ret = oneshot_register("/dev/oneshot1", oneshot_dev1);
      if (ret < 0)
        {
          syslog(LOG_ERR, "ERROR: Failed to register oneshot1\n");
        }
    }
  else
    {
      syslog(LOG_ERR, "ERROR: oneshot1 initialize failed\n");
    }
#endif

#if defined(CONFIG_FC7300_TSTMP1_CH2) || \
    defined(CONFIG_FC7300_TSTMP2_CH2) || \
    defined(CONFIG_FC7300_TSTMP3_CH2)
  struct oneshot_lowerhalf_s *oneshot_dev2;
  oneshot_dev2 = oneshot_initialize(2, 1);
  if (oneshot_dev2 != NULL)
    {
      ret = oneshot_register("/dev/oneshot2", oneshot_dev2);
      if (ret < 0)
        {
          syslog(LOG_ERR, "ERROR: Failed to register oneshot2\n");
        }
    }
  else
    {
      syslog(LOG_ERR, "ERROR: oneshot2 initialize failed\n");
    }
#endif

#if defined(CONFIG_FC7300_TSTMP1_CH3) || \
    defined(CONFIG_FC7300_TSTMP2_CH3) || \
    defined(CONFIG_FC7300_TSTMP3_CH3)
  struct oneshot_lowerhalf_s *oneshot_dev3;
  oneshot_dev3 = oneshot_initialize(3, 1);
  if (oneshot_dev3 != NULL)
    {
      ret = oneshot_register("/dev/oneshot3", oneshot_dev3);
      if (ret < 0)
        {
          syslog(LOG_ERR, "ERROR: Failed to register oneshot3\n");
        }
    }
  else
    {
      syslog(LOG_ERR, "ERROR: oneshot3 initialize failed\n");
    }
#endif
#endif /* CONFIG_FC7300_ONESHOT */

#ifdef CONFIG_CAPTURE
  /* Initialize Capture and register the Capture driver. */

  ret = fc7300_capture_setup();
  if (ret < 0)
    {
      syslog(LOG_ERR, "ERROR: fc7300_capture_setup failed: %d\n", ret);
      return ret;
    }
#endif

#ifdef CONFIG_ADC
  /* Initialize ADC and register the ADC driver. */

  ret = fc7300_adc_setup();
  if (ret < 0)
    {
      syslog(LOG_ERR, "ERROR: fc7300_adc_setup failed: %d\n", ret);
    }
#endif /* CONFIG_ADC */

#ifdef CONFIG_WATCHDOG
  /* Initialize watchdong and register the watchdog driver. */

  ret = fc7300_wdog_setup();
  if (ret < 0)
    {
      syslog(LOG_ERR, "ERROR: fc7300_wdog_setup failed: %d\n", ret);
    }
#endif /* CONFIG_WATCHDOG */

#ifdef CONFIG_SPI_DRIVER
  /* Initialize SPI driver */

  ret = fc7300_spidev_initialize();
  if (ret < 0)
    {
      syslog(LOG_ERR, "ERROR: fc7300_spidev_initialize() failed: %d\n",
             ret);
    }
#endif /* CONFIG_SPI_DRIVER */

  return ret;
}

#ifdef CONFIG_RPMSG_UART
void rpmsg_serialinit(void)
{
  int cpu = fc7300_cpu_index();
  if (cpu == 0)
    {
      uart_rpmsg_init("core1", "core1", 256, false);
    }
  else if (cpu == 1)
    {
      uart_rpmsg_init("core0", "core1", 256, true);
    }
  else
    {
      syslog(LOG_ERR, "ERROR: fc7300_cpu_index() error: cpu = %d\n", cpu);
    }
}
#endif