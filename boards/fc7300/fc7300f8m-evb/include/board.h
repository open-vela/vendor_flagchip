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

#ifndef __VENDOR_FLAGCHIP_BOARDS_FC7300_FC7300F8M_EVB_INCLUDE_BOARD_H
#define __VENDOR_FLAGCHIP_BOARDS_FC7300_FC7300F8M_EVB_INCLUDE_BOARD_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>
#include "fc7300_trgsel.h"
#include "fc7300_periphclocks.h"
/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Clocking *****************************************************************/

/* The FC7300F8M-EVAL is fitted with a 24 MHz crystal */

#define BOARD_XTAL_FREQUENCY     24000000U

/* The FC7300 will run at 300 MHz in RUN mode */

#define FC7300_SYSCLK_FREQUENCY  300000000UL

/* LED definitions **********************************************************/

/* The FC7300F8M-EVAL has three LEDs:
 *
 *   LED0    PTC30
 *   LED1    PTD4
 *   LED2    PTB28
 *
 * An output of '1' illuminates the LED.
 *
 * If CONFIG_ARCH_LEDS is not defined, then the user can control the LEDs in
 * any way.  The following definitions are used to access individual RGB
 * components (see fc7300f8m_eval.h):
 */

/* LED index values for use with board_userled() */

#define BOARD_LED_0       0
#define BOARD_LED_1       1
#define BOARD_LED_2       2
#define BOARD_NLEDS       3

/* LED bits for use with board_userled_all() */

#define BOARD_LED_0_BIT   (1 << BOARD_LED_0)
#define BOARD_LED_1_BIT   (1 << BOARD_LED_1)
#define BOARD_LED_2_BIT   (1 << BOARD_LED_2)

/* If CONFIG_ARCH_LEDs is defined, then NuttX will control the LEDs on
 * board * the FC7300F8M-EVAL. The following definitions describe how
 * NuttX controls
 * the LEDs:
 *
 *      SYMBOL            Meaning                        LED state
 *                                                       LED0   LED1   LED2
 *      ----------------  -----------------------------  -------------------
 */

#define LED_STARTED       4 /* NuttX has been started                       */
#define LED_HEAPALLOCATE  5 /* Heap has been allocated                      */
#define LED_IRQSENABLED   6 /* Interrupts enabled                           */
#define LED_STACKCREATED  7 /* Idle stack created                           */
#define LED_INIRQ         8 /* In an interrupt                              */
#define LED_SIGNAL        9 /* In a signal handler                          */
#define LED_ASSERTION    10 /* An assertion failed                          */
#define LED_PANIC        11 /* The system has crashed                       */
#define LED_NORMAL       12 /* FC7300 is in pm normal mode                  */
#define LED_IDLE         13 /* FC7300 is in pm idle mode                    */
#define LED_STANDBY      14 /* FC7300 is in pm standby mode                 */
#define LED_SLEEP        15 /* FC7300 is in pm sleep mode                   */

/* Button definitions *******************************************************/

/* The FC7300F8M-EVAL supports three buttons:
 *
 *   KEY1  PTC28
 *   KEY2  PTC27
 *   KEY3  PTD27
 */

#define BUTTON_NMI         0
#define BUTTON_KEY1        1
#define BUTTON_KEY2        2
#define BUTTON_KEY3        3
#define NUM_BUTTONS        1

#define MIN_IRQBUTTON      0
#define MAX_IRQBUTTON      6

#define BUTTON_KEY1_BIT    (1 << BUTTON_KEY1)
#define BUTTON_KEY2_BIT    (1 << BUTTON_KEY2)
#define BUTTON_KEY3_BIT    (1 << BUTTON_KEY3)

/* UART selections **********************************************************/

/* By default, the serial console will be provided on the CH340C port:
 *
 *   UART RX  PTA19  (FCUART1_RX)
 *   UART TX  PTA18  (FCUART1_TX)
 */

#define PIN_FCUART1_RX    PIN_FCUART1_RX_1   /* PTA19 */
#define PIN_FCUART1_TX    PIN_FCUART1_TX_1   /* PTA18 */

/* LIN selections ***********************************************************/

/* LIN2 (FCUART2) */

#define PIN_LIN2_RX       PIN_FCUART2_RX_2   /* PTA8 */
#define PIN_LIN2_TX       PIN_FCUART2_TX_1   /* PTA9 */
#define PIN_LIN2_ENABLE   (PIN_PTA20 | GPIO_OUTPUT)
#define LIN2_ENABLE_OUT   1

/* LIN6 (FCUART6) */

#define PIN_LIN6_RX       PIN_FCUART6_RX_1   /* PTA21 */
#define PIN_LIN6_TX       PIN_FCUART6_TX_1   /* PTD1 */
#define PIN_LIN6_ENABLE   (PIN_PTE4 | GPIO_OUTPUT)
#define LIN6_ENABLE_OUT   1

/* LIN7 (FCUART7) */

#define PIN_LIN7_RX       PIN_FCUART7_RX_2   /* PTH2 */
#define PIN_LIN7_TX       PIN_FCUART7_TX_2   /* PTI0 */

/* LIN17 (FCUART17) */

#define PIN_LIN17_RX      PIN_FCUART17_RX_1  /* PTE5 */
#define PIN_LIN17_TX      PIN_FCUART17_TX_1  /* PTD0 */

/* IIC selections ***********************************************************/

/* EEPROM  (FCIIC0) */

#define PIN_FCIIC0_SCL    PIN_FCIIC0_SCL_1    /* PTA3 */
#define PIN_FCIIC0_SDA    PIN_FCIIC0_SDA_3    /* PTG16 */

/* EEPROM  (FCIIC1) */

#define PIN_FCIIC1_SCL    PIN_FCIIC1_SCL_3    /* PTE1 */
#define PIN_FCIIC1_SDA    PIN_FCIIC1_SDA_4    /* PTE0 */

/* SPI selections ***********************************************************/

/* Flash SPI  (FCSPI0) */

#define PIN_FCSPI0_SCK    PIN_FCSPI0_SCK_1    /* PTE0 */
#define PIN_FCSPI0_MOSI   PIN_FCSPI0_SOUT_1   /* PTA12 */
#define PIN_FCSPI0_MISO   PIN_FCSPI0_SIN_1    /* PTE1 */

/* Flash SPI  (FCSPI1) */

#define PIN_FCSPI1_SCK    PIN_FCSPI1_SCK_2    /* PTD2 */
#define PIN_FCSPI1_MOSI   PIN_FCSPI1_SOUT_1   /* PTD4 */
#define PIN_FCSPI1_MISO   PIN_FCSPI1_SIN_2    /* PTD3 */

/* Flash SPI  (FCSPI2) */

#define PIN_FCSPI2_SCK    PIN_FCSPI2_SCK_2    /* PTB31 */
#define PIN_FCSPI2_MOSI   PIN_FCSPI2_SOUT_3   /* PTC16 */
#define PIN_FCSPI2_MISO   PIN_FCSPI2_SIN_2    /* PTC18 */

/* Flash SPI  (FCSPI3) */

#define PIN_FCSPI3_SCK    PIN_FCSPI3_SCK_1    /* PTH21 */
#define PIN_FCSPI3_MOSI   PIN_FCSPI3_SOUT     /* PTG6 */
#define PIN_FCSPI3_MISO   PIN_FCSPI3_SIN      /* PTF8 */

/* Flash SPI  (FCSPI4) */

#define PIN_FCSPI4_SCK    PIN_FCSPI4_SCK_2    /* PTG19 */
#define PIN_FCSPI4_MOSI   PIN_FCSPI4_SOUT_2   /* PTH18 */
#define PIN_FCSPI4_MISO   PIN_FCSPI4_SIN_2    /* PTG20 */

#define GPIO_SPI0_CS0     PIN_FCSPI0_PCS0_1   /* PTE25 */

#define GPIO_SPI1_CS0     PIN_FCSPI1_PCS0_2   /* PTD23 */
#define GPIO_SPI1_CS1     PIN_FCSPI1_PCS1_1   /* PTD17 */
#define GPIO_SPI1_CS2     PIN_FCSPI1_PCS2_2   /* PTA30 */
#define GPIO_SPI1_CS3     PIN_FCSPI1_PCS3_2   /* PTF18 */

#define GPIO_SPI2_CS0     PIN_FCSPI2_PCS0_1   /* PTB23 */
#define GPIO_SPI2_CS1     PIN_FCSPI2_PCS1_2   /* PTD9 */
#define GPIO_SPI2_CS2     PIN_FCSPI2_PCS2_1   /* PTD8 */
#define GPIO_SPI2_CS3     PIN_FCSPI2_PCS3_2   /* PTD8 */

#define GPIO_SPI3_CS0     PIN_FCSPI3_PCS0_1   /* PTG7 */
#define GPIO_SPI3_CS1     PIN_FCSPI3_PCS1     /* PTC0 */
#define GPIO_SPI3_CS2     PIN_FCSPI3_PCS2     /* PTB22 */
#define GPIO_SPI3_CS3     PIN_FCSPI3_PCS3     /* PTC15 */

#define GPIO_SPI4_CS0     PIN_FCSPI4_PCS0_2   /* PTG18 */
#define GPIO_SPI4_CS1     PIN_FCSPI4_PCS1     /* PTF20 */
#define GPIO_SPI4_CS2     PIN_FCSPI4_PCS2     /* PTF19 */
#define GPIO_SPI4_CS3     PIN_FCSPI4_PCS3     /* PTF18 */

/* PWM Configuration ********************************************************/

/* FTU0 Channels */

#define GPIO_FTU0_CH3OUT PIN_FTU0_CH3_1 /* PTA13 */

/* FTU1 Channels */

#define GPIO_FTU1_CH0OUT PIN_FTU1_CH0_1  /* PTB20 */
#define GPIO_FTU1_CH1OUT PIN_FTU1_CH1_1  /* PTB21 */
#define GPIO_FTU1_CH4OUT PIN_FTU1_CH4_1  /* PTC13 */
#define GPIO_FTU1_CH6OUT PIN_FTU1_CH6_1  /* PTC19 */

/* FTU6 Channels */

#define GPIO_FTU6_CH6OUT PIN_FTU6_CH6_1  /* PTC23 */

/* CAN selections ***********************************************************/

/* CAN1 ==> AZKN9125P ==> FLEXCAN1 */
#define PIN_CAN1_RX       PIN_FLEXCAN1_RX_3  /* PTE23 */
#define PIN_CAN1_TX       PIN_FLEXCAN1_TX_2  /* PTE24 */
#define PIN_CAN1_STB      (PIN_PTC7 | GPIO_OUTPUT)
#define CAN1_STB_OUT      0

/* CAN2 ==> TJA1043 ==> FLEXCAN2 */

#define PIN_CAN2_RX       PIN_FLEXCAN2_RX_2  /* PTD15 */
#define PIN_CAN2_TX       PIN_FLEXCAN2_TX_3  /* PTD16 */
#define PIN_CAN2_STB      (PIN_PTB18 | GPIO_OUTPUT)
#define CAN2_STB_OUT      1
#define PIN_CAN2_ENABLE   (PIN_PTA31 | GPIO_OUTPUT)
#define CAN2_ENABLE_OUT   1
#define PIN_CAN2_ERRN     (PIN_PTE9  | GPIO_INPUT)

/* CAN3 ==> TJA1051 ==> FLEXCAN5 */
#define PIN_CAN5_RX       PIN_FLEXCAN5_RX_3  /* PTE15 */
#define PIN_CAN5_TX       PIN_FLEXCAN5_TX_3  /* PTE16 */
#define PIN_CAN5_ENABLE   (PIN_PTA22 | GPIO_OUTPUT)
#define CAN5_ENABLE_OUT   0

/* CAN4 ==> AZKN9125P ==> FLEXCAN6 */
#define PIN_CAN6_RX       PIN_FLEXCAN6_RX_2  /* PTG9 */
#define PIN_CAN6_TX       PIN_FLEXCAN6_TX_2  /* PTF11 */

/* CAN5 ==> AZKN9145P ==> FLEXCAN7 */
#define PIN_CAN7_RX       PIN_FLEXCAN7_RX_2  /* PTH15 */
#define PIN_CAN7_TX       PIN_FLEXCAN7_TX_2  /* PTG17 */

/* CAN6 ==> AZKN9125P ==> FLEXCAN8 */
#define PIN_CAN8_RX       PIN_FLEXCAN8_RX_2  /* PTI4 */
#define PIN_CAN8_TX       PIN_FLEXCAN8_TX_2  /* PTI3 */

/* CAP Configuration ********************************************************/

/* ISM0 Channels */

#define GPIO_ISM_CH0IN    (PIN_PTE18 | GPIO_INPUT)   /* PTE18 */
#define TRGSEL3_SRC_CH0IN  TRGSEL3_SRC_PTE18_IN

#define GPIO_ISM_CH1IN    (PIN_PTE22 | GPIO_INPUT)   /* PTE22 */
#define TRGSEL3_SRC_CH1IN  TRGSEL3_SRC_PTE22_IN

#define FC7300_CAP_ISM_CH0_FREQ  30000               /* 5kHz */
#define FC7300_CAP_ISM_CH1_FREQ  30000               /* 5kHz */

/* FTU0 Channels */

#define GPIO_FTU0_CH0IN  PIN_FTU0_CH0_1             /* Pin: PTE20 */
#define GPIO_FTU_CH0IN   GPIO_FTU0_CH0IN

#define GPIO_FTU0_CH1IN  PIN_FTU0_CH1_1             /* Pin: PTE21 */
#define GPIO_FTU_CH1IN   GPIO_FTU0_CH1IN

#define GPIO_FTU0_CH2IN  PIN_FTU0_CH2_2             /* Pin: PTE3 */
#define GPIO_FTU_CH2IN   GPIO_FTU0_CH2IN

#define GPIO_FTU0_CH4IN  PIN_FTU0_CH4_2             /* Pin: PTE23 */
#define GPIO_FTU_CH4IN   GPIO_FTU0_CH4IN

#define GPIO_FTU0_CH3IN  PIN_FTU0_CH3_1             /* Pin: PTA13 */
#define GPIO_FTU_CH3IN   GPIO_FTU0_CH3IN

#define GPIO_FTU0_CH5IN  PIN_FTU0_CH5_2             /* Pin: PTE24 */
#define GPIO_FTU_CH5IN   GPIO_FTU0_CH5IN

#define GPIO_FTU0_CH6IN  PIN_FTU0_CH6_3             /* Pin: PTG9 */
#define GPIO_FTU_CH6IN   GPIO_FTU0_CH6IN

#define GPIO_FTU0_CH7IN  PIN_FTU0_CH7_2             /* Pin: PTE9 */
#define GPIO_FTU_CH7IN   GPIO_FTU0_CH7IN

/* FTU6 Channels */

#define GPIO_FTU6_CH7IN  PIN_FTU6_CH7_2             /* Pin: PTD10 */
#define GPIO_FTU_CH55IN  GPIO_FTU6_CH7IN

#define GPIO_FTU6_CH6IN  PIN_FTU6_CH6_2             /* Pin: PTD11 */
#define GPIO_FTU_CH54IN  GPIO_FTU6_CH6IN

#define GPIO_FTU6_CH5IN  PIN_FTU6_CH5_1             /* Pin: PTD12 */
#define GPIO_FTU_CH53IN  GPIO_FTU6_CH5IN

#define GPIO_FTU6_CH1IN  PIN_FTU6_CH1_3             /* Pin: PTE8 */
#define GPIO_FTU_CH49IN  GPIO_FTU6_CH1IN

#define GPIO_FTU6_CH0IN  PIN_FTU6_CH0_1             /* Pin: PTB26 */
#define GPIO_FTU_CH48IN  GPIO_FTU6_CH0IN

#define GPIO_FTU6_CH4IN  PIN_FTU6_CH4_3             /* Pin: PTF6 */
#define GPIO_FTU_CH52IN  GPIO_FTU6_CH4IN

#define GPIO_FTU6_CH3IN  PIN_FTU6_CH3_3             /* Pin: PTI5 */
#define GPIO_FTU_CH51IN  GPIO_FTU6_CH3IN

/* FTU7 Channels */

#define GPIO_FTU7_CH4IN  PIN_FTU7_CH4_1             /* Pin: PTC14 */
#define GPIO_FTU_CH60IN  GPIO_FTU7_CH4IN

/* FTU8 Channels */

#define GPIO_FTU8_CH2IN  PIN_FTU8_CH2_1             /* Pin: PTH16 */
#define GPIO_FTU_CH66IN  GPIO_FTU8_CH2IN

#define GPIO_FTU8_CH3IN  PIN_FTU8_CH3_2             /* Pin: PTH17 */
#define GPIO_FTU_CH67IN  GPIO_FTU8_CH3IN

#define GPIO_FTU8_CH4IN  PIN_FTU8_CH4_2             /* Pin: PTI15 */
#define GPIO_FTU_CH68IN  GPIO_FTU8_CH4IN

#define GPIO_FTU8_CH5IN  PIN_FTU8_CH5_2             /* Pin: PTI16 */
#define GPIO_FTU_CH69IN  GPIO_FTU8_CH5IN

/* FTU11 Channels */

#define GPIO_FTU11_CH0IN  PIN_FTU11_CH0_2           /* Pin: PTH4 */
#define GPIO_FTU_CH88IN   GPIO_FTU11_CH0IN

#define GPIO_FTU11_CH1IN  PIN_FTU11_CH1_1           /* Pin: PTH5 */
#define GPIO_FTU_CH89IN   GPIO_FTU11_CH1IN

/* ADC Configuration ********************************************************/

/* ADC Timer support */

#define FC7300_ADC0_TIMER_BASE         FC7300_FCPIT0_CH0_CTRL_BASE
#define FC7300_ADC1_TIMER_BASE         FC7300_FCPIT0_CH0_CTRL_BASE
#define FC7300_ADC2_TIMER_BASE         FC7300_FCPIT0_CH0_CTRL_BASE
#define FC7300_ADC3_TIMER_BASE         FC7300_FCPIT0_CH0_CTRL_BASE

/* ADC0 Channels */

#define GPIO_ADC0_CH2IN  PIN_ADC0_SE2
#define GPIO_ADC0_CH1IN  PIN_ADC0_SE1
#define GPIO_ADC0_CH14IN PIN_ADC0_SE14_CMP0_IN6
#define GPIO_ADC0_CH15IN PIN_ADC0_SE15_CMP0_IN7
#define GPIO_ADC0_CH11IN PIN_ADC0_SE11
#define GPIO_ADC0_CH16IN PIN_ADC0_SE16
#define GPIO_ADC0_CH18IN PIN_ADC0_SE18
#define GPIO_ADC0_CH19IN PIN_ADC0_SE19
#define GPIO_ADC0_CH25IN PIN_ADC0_SE25
#define GPIO_ADC0_CH29IN PIN_ADC0_SE29
#define GPIO_ADC0_CH30IN PIN_ADC0_SE30
#define GPIO_ADC0_CH31IN PIN_ADC0_SE31
#define GPIO_ADC0_CH13IN PIN_ADC0_SE13
#define GPIO_ADC0_CH6IN  PIN_ADC0_SE6
#define GPIO_ADC0_CH27IN PIN_ADC0_SE27
#define GPIO_ADC0_CH26IN PIN_ADC0_SE26
#define GPIO_ADC0_CH24IN PIN_ADC0_SE24
#define GPIO_ADC0_CH12IN PIN_ADC0_SE12
#define GPIO_ADC0_CH28IN PIN_ADC0_SE28
#define GPIO_ADC0_CH9IN  PIN_ADC0_SE9
#define GPIO_ADC0_CH8IN  PIN_ADC0_SE8
#define GPIO_ADC0_CH17IN PIN_ADC0_SE17
#define GPIO_ADC0_CH10IN PIN_ADC0_SE10
#define GPIO_ADC0_CH5IN  PIN_ADC0_SE5
#define GPIO_ADC0_CH7IN  PIN_ADC0_SE7
#define GPIO_ADC0_CH0IN  PIN_ADC0_SE0
#define GPIO_ADC0_CH4IN  PIN_ADC0_SE4
#define GPIO_ADC0_CH20IN PIN_ADC0_SE20
#define GPIO_ADC0_CH21IN PIN_ADC0_SE21
#define GPIO_ADC0_CH22IN PIN_ADC0_SE22
#define GPIO_ADC0_CH23IN PIN_ADC0_SE23
#define GPIO_ADC0_CH3IN  PIN_ADC0_SE3
#define GPIO_ADC0_CH32IN 0                      /* VBG, No Pin needed */
#define GPIO_ADC0_CH33IN 0                      /* V25, No Pin needed */
#define GPIO_ADC0_CH34IN 0                      /* V11, No Pin needed */
#define GPIO_ADC0_CH36IN 0                      /* CMP0_DAC, No Pin needed */
#define GPIO_ADC0_CH37IN 0                      /* TEMPSENSOR, No Pin needed */

/* ADC1 Channels */

#define GPIO_ADC1_CH14IN PIN_ADC1_SE14
#define GPIO_ADC1_CH15IN PIN_ADC1_SE15
#define GPIO_ADC1_CH18IN PIN_ADC1_SE18
#define GPIO_ADC1_CH28IN PIN_ADC1_SE28
#define GPIO_ADC1_CH7IN  PIN_ADC1_SE7
#define GPIO_ADC1_CH3IN  PIN_ADC1_SE3
#define GPIO_ADC1_CH6IN  PIN_ADC1_SE6
#define GPIO_ADC1_CH30IN PIN_ADC1_SE30
#define GPIO_ADC1_CH31IN PIN_ADC1_SE31
#define GPIO_ADC1_CH24IN PIN_ADC1_SE24
#define GPIO_ADC1_CH25IN PIN_ADC1_SE25
#define GPIO_ADC1_CH2IN  PIN_ADC1_SE2
#define GPIO_ADC1_CH29IN PIN_ADC1_SE29
#define GPIO_ADC1_CH23IN PIN_ADC1_SE23
#define GPIO_ADC1_CH22IN PIN_ADC1_SE22
#define GPIO_ADC1_CH21IN PIN_ADC1_SE21
#define GPIO_ADC1_CH20IN PIN_ADC1_SE20
#define GPIO_ADC1_CH19IN PIN_ADC1_SE19
#define GPIO_ADC1_CH17IN PIN_ADC1_SE17
#define GPIO_ADC1_CH16IN PIN_ADC1_SE16
#define GPIO_ADC1_CH13IN PIN_ADC1_SE13
#define GPIO_ADC1_CH12IN PIN_ADC1_SE12
#define GPIO_ADC1_CH11IN PIN_ADC1_SE11_CMP1_IN3
#define GPIO_ADC1_CH10IN PIN_ADC1_SE10_CMP1_IN2
#define GPIO_ADC1_CH9IN  PIN_ADC1_SE9_CMP1_IN1
#define GPIO_ADC1_CH8IN  PIN_ADC1_SE8_CMP1_IN0
#define GPIO_ADC1_CH5IN  PIN_ADC1_SE5
#define GPIO_ADC1_CH1IN  PIN_ADC1_SE1
#define GPIO_ADC1_CH4IN  PIN_ADC1_SE4
#define GPIO_ADC1_CH27IN PIN_ADC1_SE27
#define GPIO_ADC1_CH26IN PIN_ADC1_SE26
#define GPIO_ADC1_CH0IN  PIN_ADC1_SE0
#define GPIO_ADC1_CH32IN 0                      /* V11, No Pin needed */
#define GPIO_ADC1_CH33IN 0                      /* VREFH0, No Pin needed */
#define GPIO_ADC1_CH34IN 0                      /* VDDA0, No Pin needed */
#define GPIO_ADC1_CH35IN 0                      /* V15, No Pin needed */
#define GPIO_ADC1_CH36IN 0                      /* CMP1_DAC, No Pin needed */

/* ADC2 Channels */

#define GPIO_ADC2_CH13IN PIN_ADC2_SE13
#define GPIO_ADC2_CH11IN PIN_ADC2_SE11
#define GPIO_ADC2_CH19IN PIN_ADC2_SE19
#define GPIO_ADC2_CH29IN PIN_ADC2_SE29
#define GPIO_ADC2_CH28IN PIN_ADC2_SE28
#define GPIO_ADC2_CH27IN PIN_ADC2_SE27
#define GPIO_ADC2_CH26IN PIN_ADC2_SE26
#define GPIO_ADC2_CH25IN PIN_ADC2_SE25
#define GPIO_ADC2_CH20IN PIN_ADC2_SE20
#define GPIO_ADC2_CH9IN PIN_ADC2_SE9
#define GPIO_ADC2_CH8IN PIN_ADC2_SE8
#define GPIO_ADC2_CH3IN PIN_ADC2_SE3
#define GPIO_ADC2_CH6IN PIN_ADC2_SE6
#define GPIO_ADC2_CH0IN PIN_ADC2_SE0
#define GPIO_ADC2_CH2IN PIN_ADC2_SE2
#define GPIO_ADC2_CH7IN PIN_ADC2_SE7
#define GPIO_ADC2_CH10IN PIN_ADC2_SE10
#define GPIO_ADC2_CH12IN PIN_ADC2_SE12
#define GPIO_ADC2_CH14IN PIN_ADC2_SE14
#define GPIO_ADC2_CH16IN PIN_ADC2_SE16
#define GPIO_ADC2_CH17IN PIN_ADC2_SE17
#define GPIO_ADC2_CH30IN PIN_ADC2_SE30
#define GPIO_ADC2_CH31IN PIN_ADC2_SE31
#define GPIO_ADC2_CH15IN PIN_ADC2_SE15
#define GPIO_ADC2_CH4IN PIN_ADC2_SE4
#define GPIO_ADC2_CH18IN PIN_ADC2_SE18
#define GPIO_ADC2_CH1IN PIN_ADC2_SE1
#define GPIO_ADC2_CH5IN PIN_ADC2_SE5
#define GPIO_ADC2_CH21IN PIN_ADC2_SE21
#define GPIO_ADC2_CH22IN PIN_ADC2_SE22
#define GPIO_ADC2_CH23IN PIN_ADC2_SE23
#define GPIO_ADC2_CH24IN PIN_ADC2_SE24
#define GPIO_ADC2_CH32IN 0                      /* V11, No Pin needed */
#define GPIO_ADC2_CH33IN 0                      /* VREFH1, No Pin needed */
#define GPIO_ADC2_CH34IN 0                      /* VDDA1, No Pin needed */
#define GPIO_ADC2_CH36IN 0                      /* CMP2_DAC, No Pin needed */

/* ADC3 Channels */

#define GPIO_ADC3_CH18IN PIN_ADC3_SE18
#define GPIO_ADC3_CH17IN PIN_ADC3_SE17
#define GPIO_ADC3_CH20IN PIN_ADC3_SE20
#define GPIO_ADC3_CH31IN PIN_ADC3_SE31_CMP2_IN0
#define GPIO_ADC3_CH30IN PIN_ADC3_SE30_CMP2_IN1
#define GPIO_ADC3_CH29IN PIN_ADC3_SE29_CMP2_IN2
#define GPIO_ADC3_CH19IN PIN_ADC3_SE19
#define GPIO_ADC3_CH21IN PIN_ADC3_SE21
#define GPIO_ADC3_CH22IN PIN_ADC3_SE22
#define GPIO_ADC3_CH0IN  PIN_ADC3_SE0
#define GPIO_ADC3_CH1IN  PIN_ADC3_SE1
#define GPIO_ADC3_CH4IN  PIN_ADC3_SE4
#define GPIO_ADC3_CH5IN  PIN_ADC3_SE5
#define GPIO_ADC3_CH2IN  PIN_ADC3_SE2
#define GPIO_ADC3_CH6IN  PIN_ADC3_SE6
#define GPIO_ADC3_CH10IN PIN_ADC3_SE10
#define GPIO_ADC3_CH23IN PIN_ADC3_SE23
#define GPIO_ADC3_CH25IN PIN_ADC3_SE25
#define GPIO_ADC3_CH24IN PIN_ADC3_SE24
#define GPIO_ADC3_CH3IN  PIN_ADC3_SE3
#define GPIO_ADC3_CH14IN PIN_ADC3_SE14_CMP2_IN5
#define GPIO_ADC3_CH13IN PIN_ADC3_SE13_CMP2_IN4
#define GPIO_ADC3_CH15IN PIN_ADC3_SE15_CMP2_IN6
#define GPIO_ADC3_CH7IN  PIN_ADC3_SE7
#define GPIO_ADC3_CH8IN  PIN_ADC3_SE8
#define GPIO_ADC3_CH9IN  PIN_ADC3_SE9
#define GPIO_ADC3_CH16IN PIN_ADC3_SE16_CMP2_IN7
#define GPIO_ADC3_CH28IN PIN_ADC3_SE28_CMP2_IN3
#define GPIO_ADC3_CH27IN PIN_ADC3_SE27
#define GPIO_ADC3_CH26IN PIN_ADC3_SE26
#define GPIO_ADC3_CH11IN PIN_ADC3_SE11
#define GPIO_ADC3_CH12IN PIN_ADC3_SE12
#define GPIO_ADC3_CH32IN 0                      /* V11, No Pin needed */
#define GPIO_ADC3_CH33IN 0                      /* VREFH1, No Pin needed */
#define GPIO_ADC3_CH34IN 0                      /* VDDA1, No Pin needed */
#define GPIO_ADC3_CH36IN 0                      /* CMP2_DAC, No Pin needed */

/* User peripheral configuration structure 0 */

extern const struct peripheral_clock_config_s g_peripheral_clockconfig0[];

/* PWM instance definations */
#ifdef CONFIG_FC7300_PWM
#  define FC7300F8M_ESMALL_PWM0 0
#  define FC7300F8M_ESMALL_PWM1 1
#  define FC7300F8M_ESMALL_PWM2 2
#  define FC7300F8M_ESMALL_PWM3 3
#  define FC7300F8M_ESMALL_PWM4 4
#  define FC7300F8M_ESMALL_PWM5 5
#  define FC7300F8M_ESMALL_PWM6 6
#  define FC7300F8M_ESMALL_PWM7 7
#  define FC7300F8M_ESMALL_PWM8 8
#  define FC7300F8M_ESMALL_PWM9 9
#  define FC7300F8M_ESMALL_PWM10 10
#  define FC7300F8M_ESMALL_PWM11 11
#endif

/* CAP instance definations */
#ifdef CONFIG_CAPTURE
#  define FC7300F8M_EVAL_CAP_ISM(X)        X
#  define FC7300F8M_EVAL_CAP_FTU0(X)      (X + 16)
#  define FC7300F8M_EVAL_CAP_FTU1(X)      (8 + X + 16)
#  define FC7300F8M_EVAL_CAP_FTU2(X)      (16 + X + 16)
#  define FC7300F8M_EVAL_CAP_FTU3(X)      (24 + X + 16)
#  define FC7300F8M_EVAL_CAP_FTU4(X)      (32 + X + 16)
#  define FC7300F8M_EVAL_CAP_FTU5(X)      (40 + X + 16)
#  define FC7300F8M_EVAL_CAP_FTU6(X)      (48 + X + 16)
#  define FC7300F8M_EVAL_CAP_FTU7(X)      (56 + X + 16)
#  define FC7300F8M_EVAL_CAP_FTU8(X)      (64 + X + 16)
#  define FC7300F8M_EVAL_CAP_FTU9(X)      (72 + X + 16)
#  define FC7300F8M_EVAL_CAP_FTU10(X)     (80 + X + 16)
#  define FC7300F8M_EVAL_CAP_FTU11(X)     (88 + X + 16)
#endif

/* WATCHDOG instance definations */
#ifdef CONFIG_WATCHDOG
#  define FC7300F1M_EVAL_WATCHDOG0 0
#  define FC7300F1M_EVAL_WATCHDOG1 1
#endif

/* ENET selections **********************************************************/

/* ENET 1000BaseT1 Mode Select pins */

#define GPIO_ENET_T1_MODE0        (PIN_PTC0 | GPIO_OUTPUT)  /* PTC0 */
#define GPIO_ENET_T1_MODE0_OUT    1
#define GPIO_ENET_T1_MODE1        (PIN_PTD9 | GPIO_OUTPUT)  /* PTD9 */
#define GPIO_ENET_T1_MODE1_OUT    0
#define GPIO_ENET_T1_MODE2        (PIN_PTC15 | GPIO_OUTPUT) /* PTC15 */
#define GPIO_ENET_T1_MODE2_OUT    1
#define GPIO_ENET_T1_MDIO_SEL     (PIN_PTC14 | GPIO_OUTPUT) /* PTC14 */
#define GPIO_ENET_T1_MDIO_SEL_OUT 1
#define GPIO_ENET_T1_ETHEN        (PIN_PTB26 | GPIO_OUTPUT) /* PTB26 */
#define GPIO_ENET_T1_ETHEN_OUT    1
#define GPIO_ENET_T1_WAKE         (PIN_PTB19 | GPIO_OUTPUT) /* PTB19 */
#define GPIO_ENET_T1_WAKE_OUT     1

/* ENET PHY reset pin */

#define GPIO_PHY_RESET            (PIN_PTE8 | GPIO_OUTPUT)  /* PTE8 */
#define GPIO_PHY_RESET_OUT        1

/* ENET MDIO pins */

#define GPIO_ENET_MDC             PIN_ENET_MDC_1            /* PTB5 */
#define GPIO_ENET_MDIO            PIN_ENET_MDIO             /* PTB4 */

/* ENET RGMII pins */

#define GPIO_ENET_TXCLK           PIN_ENET_TX_CLK           /* PTD11 */
#define GPIO_ENET_TXENCTL         PIN_ENET_TX_EN_CTL        /* PTD12 */
#define GPIO_ENET_TXD0            PIN_ENET_TXD0             /* PTC2 */
#define GPIO_ENET_TXD1            PIN_ENET_TXD1             /* PTD7 */
#define GPIO_ENET_TXD2            PIN_ENET_TXD2             /* PTD6 */
#define GPIO_ENET_TXD3            PIN_ENET_TXD3             /* PTD5 */
#define GPIO_ENET_RXCLK           PIN_ENET_RX_CLK           /* PTD10 */
#define GPIO_ENET_RXDVCTL         PIN_ENET_RX_DV_CTL        /* PTD8 */
#define GPIO_ENET_RXD0            PIN_ENET_RXD0_1           /* PTC1 */
#define GPIO_ENET_RXD1            PIN_ENET_RXD1_2           /* PTC0 */
#define GPIO_ENET_RXD2            PIN_ENET_RXD2_2           /* PTD9 */
#define GPIO_ENET_RXD3            PIN_ENET_RXD3_2           /* PTC15 */

/* ENET Reference clock pin */

#define GPIO_ENET_REFCLK          PIN_ENET_REF_CLK          /* PTG8 */

int fc7300_bringup(void);

#endif  /* __VENDOR_FLAGCHIP_BOARDS_FC7300_FC7300F8M_EVB_INCLUDE_BOARD_H */
