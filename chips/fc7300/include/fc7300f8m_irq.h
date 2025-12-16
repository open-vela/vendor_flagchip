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

/* This file should never be included directly but, rather,
 * only indirectly through nuttx/irq.h
 */

#ifndef __VENDOR_FLAGCHIP_CHIPS_FC7300_INCLUDE_FC7300F8M_IRQ_H
#define __VENDOR_FLAGCHIP_CHIPS_FC7300_INCLUDE_FC7300F8M_IRQ_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>

/****************************************************************************
 * Pre-processor Prototypes
 ****************************************************************************/

/* IRQ numbers.
 * The IRQ number corresponds vector number and hence map directly to
 * bits in the NVIC.  This does, however, waste several words of memory in
 * the IRQ to handle mapping tables.
 */

/* Processor Exceptions (vectors 0-15) */

#define FC7300_IRQ_RESERVED       (0)  /* Reserved vector (only used with CONFIG_DEBUG_FEATURES) */
                                       /* Vector  0: Reset stack pointer value */
                                       /* Vector  1: Reset (not handler as an IRQ) */
#define FC7300_IRQ_NMI            (2)  /* Vector  2: Non-Maskable Interrupt (NMI) */
#define FC7300_IRQ_HARDFAULT      (3)  /* Vector  3: Hard fault */
#define FC7300_IRQ_MEMFAULT       (4)  /* Vector  4: Memory management (MPU) */
#define FC7300_IRQ_BUSFAULT       (5)  /* Vector  5: Bus fault */
#define FC7300_IRQ_USAGEFAULT     (6)  /* Vector  6: Usage fault */
                                       /* Vector  7 - 10: reserved */
#define FC7300_IRQ_SVCALL         (11) /* Vector 11: SVC call */
#define FC7300_IRQ_DBGMONITOR     (12) /* Vector 12: Debug Monitor */
                                       /* Vector 13: Reserved */
#define FC7300_IRQ_PENDSV         (14) /* Vector 14: Pendable system service request */
#define FC7300_IRQ_SYSTICK        (15) /* Vector 15: System tick */

/* External interrupts (vectors >= 16).
 * These definitions are chip-specific
 */

#define FC7300_IRQ_EXINT          (16) /* Vector number of the first external interrupt */

#define FC7300_IRQ_DMACH0         (16)    /* DMA channel 0 transfer complete */
#define FC7300_IRQ_DMACH1         (17)    /* DMA channel 1 transfer complete */
#define FC7300_IRQ_DMACH2         (18)    /* DMA channel 2 transfer complete */
#define FC7300_IRQ_DMACH3         (19)    /* DMA channel 3 transfer complete */
#define FC7300_IRQ_DMACH4         (20)    /* DMA channel 4 transfer complete */
#define FC7300_IRQ_DMACH5         (21)    /* DMA channel 5 transfer complete */
#define FC7300_IRQ_DMACH6         (22)    /* DMA channel 6 transfer complete */
#define FC7300_IRQ_DMACH7         (23)    /* DMA channel 7 transfer complete */
#define FC7300_IRQ_DMACH8         (24)    /* DMA channel 8 transfer complete */
#define FC7300_IRQ_DMACH9         (25)    /* DMA channel 9 transfer complete */
#define FC7300_IRQ_DMACH10        (26)    /* DMA channel 10 transfer complete */
#define FC7300_IRQ_DMACH11        (27)    /* DMA channel 11 transfer complete */
#define FC7300_IRQ_DMACH12        (28)    /* DMA channel 12 transfer complete */
#define FC7300_IRQ_DMACH13        (29)    /* DMA channel 13 transfer complete */
#define FC7300_IRQ_DMACH14        (30)    /* DMA channel 14 transfer complete */
#define FC7300_IRQ_DMACH15        (31)    /* DMA channel 15 transfer complete */
#define FC7300_IRQ_DMACH16        (32)    /* DMA channel 16 transfer complete */
#define FC7300_IRQ_DMACH17        (33)    /* DMA channel 17 transfer complete */
#define FC7300_IRQ_DMACH18        (34)    /* DMA channel 18 transfer complete */
#define FC7300_IRQ_DMACH19        (35)    /* DMA channel 19 transfer complete */
#define FC7300_IRQ_DMACH20        (36)    /* DMA channel 20 transfer complete */
#define FC7300_IRQ_DMACH21        (37)    /* DMA channel 21 transfer complete */
#define FC7300_IRQ_DMACH22        (38)    /* DMA channel 22 transfer complete */
#define FC7300_IRQ_DMACH23        (39)    /* DMA channel 23 transfer complete */
#define FC7300_IRQ_DMACH24        (40)    /* DMA channel 24 transfer complete */
#define FC7300_IRQ_DMACH25        (41)    /* DMA channel 25 transfer complete */
#define FC7300_IRQ_DMACH26        (42)    /* DMA channel 26 transfer complete */
#define FC7300_IRQ_DMACH27        (43)    /* DMA channel 27 transfer complete */
#define FC7300_IRQ_DMACH28        (44)    /* DMA channel 28 transfer complete */
#define FC7300_IRQ_DMACH29        (45)    /* DMA channel 29 transfer complete */
#define FC7300_IRQ_DMACH30        (46)    /* DMA channel 30 transfer complete */
#define FC7300_IRQ_DMACH31        (47)    /* DMA channel 31 transfer complete */
#define FC7300_IRQ_DMACH_ERR      (48)    /* DMA error interrupt channels 0-31 */
#define FC7300_IRQ_FPU            (49)    /* FPU sources */
#define FC7300_IRQ_FLASH          (50)    /* Flash Controller Command complete etc. interrupt */
#define FC7300_IRQ_PMC            (51)    /* PMC interrupt */
#define FC7300_IRQ_TMU            (52)    /* Temperature Monitor Unit interrupt  */
#define FC7300_IRQ_WDOG0          (53)    /* interrupt request out before wdg0 reset */
#define FC7300_IRQ_WDOG1          (54)    /* interrupt request out before wdg1 reset */
#define FC7300_IRQ_WDOG2          (55)    /* interrupt request out before wdg2 reset */
#define FC7300_IRQ_FCSMU          (56)    /* Fault Control and Safety Manage Unit */
#define FC7300_IRQ_STCU           (57)    /* Safety Control Unit interrupt */
#define FC7300_IRQ_ERM            (58)    /* ERM single/double bit error correction */
#define FC7300_IRQ_MAM0           (59)    /* Matrix Access Monitor interrupt */
#define FC7300_IRQ_MAM1           (60)    /* Matrix Access Monitor interrupt */
#define FC7300_IRQ_MAM2           (61)    /* Matrix Access Monitor interrupt */
#define FC7300_IRQ_RGM_PRE        (63)    /* RGM pre-reset Interrupt */
#define FC7300_IRQ_RGM_OTHER      (64)    /* RGM other CPU exit reset Interrupt */
#define FC7300_IRQ_INTM           (65)    /* INTM alarm interrupt */
#define FC7300_IRQ_ISM            (66)    /* ISM0 interrupt */
#define FC7300_IRQ_MB             (67)    /* Mail Box interrupt */
#define FC7300_IRQ_SCG            (68)    /* SCG bus interrupt request */
#define FC7300_IRQ_CMU0           (69)    /* CMU0 interrupt */
#define FC7300_IRQ_CMU1           (70)    /* CMU1 interrupt */
#define FC7300_IRQ_CMU2           (71)    /* CMU2 interrupt */
#define FC7300_IRQ_CMU3           (72)    /* CMU3 interrupt */
#define FC7300_IRQ_TSTMP0         (73)    /* TimerStamp0 interrupt */
#define FC7300_IRQ_TSTMP1         (74)    /* TimerStamp1 interrupt */
#define FC7300_IRQ_TSTMP2         (75)    /* TimerStamp2 interrupt */
#define FC7300_IRQ_TSTMP3         (76)    /* TimerStamp3 interrupt */
#define FC7300_IRQ_CORDIC         (77)    /* CORDIC Accelerator interrupt */
#define FC7300_IRQ_HSM0           (78)    /* Crypto interrupt */
#define FC7300_IRQ_FCPIT0         (79)    /* FCPIT0 interrupt */
#define FC7300_IRQ_FCPIT1         (80)    /* FCPIT0 interrupt */
#define FC7300_IRQ_RTC            (81)    /* RTC alarm or seconds interrupt  */
#define FC7300_IRQ_ENET_TX0       (82)    /* ENET Data transfer done for Ring/Queue 0 */
#define FC7300_IRQ_ENET_TX1       (83)    /* ENET Data transfer done for Ring/Queue 1 */
#define FC7300_IRQ_ENET_RX0       (84)    /* ENET Receive Buffer Done for Ring/Queue 0 */
#define FC7300_IRQ_ENET_RX1       (85)    /* ENET Receive Buffer Done for Ring/Queue 1 */
#define FC7300_IRQ_ENET_SYSTEM    (86)    /* ENET System interrupt */
#define FC7300_IRQ_AONTIMER       (87)    /* AONTIMER interrupt request */
#define FC7300_IRQ_SWI            (88)    /* Software interrupt */
#define FC7300_IRQ_OSPI           (89)    /* All OSPI interrupts */
#define FC7300_IRQ_FREQM          (90)    /* FREQM interrupt */
#define FC7300_IRQ_PORTA          (91)    /* Port A pin detect interrupt */
#define FC7300_IRQ_PORTB          (92)    /* Port B pin detect interrupt */
#define FC7300_IRQ_PORTC          (93)    /* Port C pin detect interrupt */
#define FC7300_IRQ_PORTD          (94)    /* Port D pin detect interrupt */
#define FC7300_IRQ_PORTE          (95)    /* Port E pin detect interrupt */
#define FC7300_IRQ_PORTF          (96)    /* Port F pin detect interrupt */
#define FC7300_IRQ_PORTG          (97)    /* Port G pin detect interrupt */
#define FC7300_IRQ_PORTH          (98)    /* Port H pin detect interrupt */
#define FC7300_IRQ_PORTI          (99)    /* Port I pin detect interrupt */
#define FC7300_IRQ_FLEXCAN0       (100)   /* CAN0 Interrupt */
#define FC7300_IRQ_FLEXCAN1       (101)   /* CAN1 Interrupt */
#define FC7300_IRQ_FLEXCAN2       (102)   /* CAN2 Interrupt */
#define FC7300_IRQ_FLEXCAN3       (103)   /* CAN3 Interrupt */
#define FC7300_IRQ_FLEXCAN4       (104)   /* CAN4 Interrupt */
#define FC7300_IRQ_FLEXCAN5       (105)   /* CAN5 Interrupt */
#define FC7300_IRQ_FLEXCAN6       (106)   /* CAN6 Interrupt */
#define FC7300_IRQ_FLEXCAN7       (107)   /* CAN7 Interrupt */
#define FC7300_IRQ_FCIIC0         (108)   /* FCIIC0 Interrupt */
#define FC7300_IRQ_FCIIC1         (109)   /* FCIIC1 Interrupt */
#define FC7300_IRQ_FCSPI0         (110)   /* FCSPI0 Interrupt */
#define FC7300_IRQ_FCSPI1         (111)   /* FCSPI1 Interrupt */
#define FC7300_IRQ_FCSPI2         (112)   /* FCSPI2 Interrupt */
#define FC7300_IRQ_FCSPI3         (113)   /* FCSPI0 Interrupt */
#define FC7300_IRQ_FCSPI4         (114)   /* FCSPI1 Interrupt */
#define FC7300_IRQ_FCSPI5         (115)   /* FCSPI2 Interrupt */
#define FC7300_IRQ_FCUART0        (116)   /* FCUART0 Transmit / Receive Interrupt */
#define FC7300_IRQ_FCUART1        (117)   /* FCUART1 Transmit / Receive  Interrupt */
#define FC7300_IRQ_FCUART2        (118)   /* FCUART2 Transmit / Receive  Interrupt */
#define FC7300_IRQ_FCUART3        (119)   /* FCUART3 Transmit / Receive Interrupt */
#define FC7300_IRQ_FCUART4        (120)   /* FCUART4 Transmit / Receive  Interrupt */
#define FC7300_IRQ_FCUART5        (121)   /* FCUART5 Transmit / Receive  Interrupt */
#define FC7300_IRQ_FCUART6        (122)   /* FCUART6 Transmit / Receive Interrupt */
#define FC7300_IRQ_FCUART7        (123)   /* FCUART7 Transmit / Receive  Interrupt */
#define FC7300_IRQ_FCUART8        (124)   /* FCUART8 Transmit / Receive  Interrupt */
#define FC7300_IRQ_FCUART9        (125)   /* FCUART9 Transmit / Receive Interrupt */
#define FC7300_IRQ_FCUART10       (126)   /* FCUART10 Transmit / Receive  Interrupt */
#define FC7300_IRQ_FCUART11       (127)   /* FCUART11 Transmit / Receive  Interrupt */
#define FC7300_IRQ_FCUART12       (128)   /* FCUART12 Transmit / Receive  Interrupt */
#define FC7300_IRQ_FCUART13       (129)   /* FCUART13 Transmit / Receive  Interrupt */
#define FC7300_IRQ_FCUART14       (130)   /* FCUART14 Transmit / Receive Interrupt */
#define FC7300_IRQ_FCUART15       (131)   /* FCUART15 Transmit / Receive  Interrupt */
#define FC7300_IRQ_FTU0           (132)   /* FTU0 all source interrupt */
#define FC7300_IRQ_FTU1           (133)   /* FTU1 all source interrupt */
#define FC7300_IRQ_FTU2           (134)   /* FTU2 all source interrupt */
#define FC7300_IRQ_FTU3           (135)   /* FTU3 all source interrupt */
#define FC7300_IRQ_FTU4           (136)   /* FTU4 all source interrupt */
#define FC7300_IRQ_FTU5           (137)   /* FTU5 all source interrupt */
#define FC7300_IRQ_FTU6           (138)   /* FTU6 all source interrupt */
#define FC7300_IRQ_FTU7           (139)   /* FTU7 all source interrupt */
#define FC7300_IRQ_FTU8           (140)   /* FTU4 all source interrupt */
#define FC7300_IRQ_FTU9           (141)   /* FTU5 all source interrupt */
#define FC7300_IRQ_FTU10          (142)   /* FTU6 all source interrupt */
#define FC7300_IRQ_FTU11          (143)   /* FTU7 all source interrupt */
#define FC7300_IRQ_CMP0           (144)   /* CMP0 interrupt request  */
#define FC7300_IRQ_CMP1           (145)   /* CMP1 interrupt request  */
#define FC7300_IRQ_CMP2           (146)   /* CMP2 interrupt request  */
#define FC7300_IRQ_ADC0           (147)   /* ADC0 interrupt request. */
#define FC7300_IRQ_ADC1           (148)   /* ADC1 interrupt request. */
#define FC7300_IRQ_ADC2           (149)   /* ADC2 interrupt request. */
#define FC7300_IRQ_ADC3           (150)   /* ADC3 interrupt request. */
#define FC7300_IRQ_PTIMER0        (151)   /* PTIMER0 interrupt */
#define FC7300_IRQ_PTIMER1        (152)   /* PTIMER1 interrupt */
#define FC7300_IRQ_PTIMER2        (153)   /* PTIMER2 interrupt */
#define FC7300_IRQ_PTIMER3        (154)   /* PTIMER3 interrupt */
#define FC7300_IRQ_SDDF0          (155)   /* SD Digital Filter interrupt request */
#define FC7300_IRQ_MSC0           (156)   /* MSC interrupt */
#define FC7300_IRQ_MSC1           (157)   /* MSC interrupt */
#define FC7300_IRQ_FLEXCAN8       (158)   /* CAN6 Interrupt */
#define FC7300_IRQ_FLEXCAN9       (159)   /* CAN7 Interrupt */
#define FC7300_IRQ_SENT0          (160)   /* SENT all interrupt (fast or slow) */
#define FC7300_IRQ_SENT1          (161)   /* SENT all interrupt (fast or slow) */
#define FC7300_IRQ_FCSPI6         (162)   /* FCSPI6 Interrupt */
#define FC7300_IRQ_FCSPI7         (163)   /* FCSPI7 Interrupt */
#define FC7300_IRQ_FCUART16       (164)   /* FCUART16 Transmit / Receive Interrupt */
#define FC7300_IRQ_FCUART17       (165)   /* FCUART17 Transmit / Receive  Interrupt */
#define FC7300_IRQ_CTI0           (166)   /* CTI Interrupt */
#define FC7300_IRQ_CTI1           (167)   /* CTI Interrupt */
#define FC7300_IRQ_CTI2           (168)   /* CTI Interrupt */
#define FC7300_IRQ_CMU4           (169)   /* CMU4 Interrupt */

#define FC7300_IRQ_NIRQS          (170)
#define FC7300_IRQ_NEXTINT        (FC7300_IRQ_NIRQS - FC7300_IRQ_EXINT)

/* Total number of IRQ numbers */

#define NR_IRQS                   (FC7300_IRQ_NIRQS)

#endif /* __VENDOR_FLAGCHIP_CHIPS_FC7300_INCLUDE_FC7300F8M_IRQ_H */
