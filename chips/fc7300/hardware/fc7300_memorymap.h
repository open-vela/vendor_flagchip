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

#ifndef __VENDOR_FLAGCHIP_CHIPS_FC7300_HARDWARE_FC7300_MEMORYMAP_H
#define __VENDOR_FLAGCHIP_CHIPS_FC7300_HARDWARE_FC7300_MEMORYMAP_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#define FC7300_AFCB0_BASE       0x40000000                 /* Peripheral bridge0 (AFCB0) */

#define FC7300_DMA0_BASE        0x40008000                 /* DMA controller 0 */
#define FC7300_DMACFG0_BASE     0x40009000                 /* DMA configuration 0 */
#define FC7300_DMAMUX0_BASE     0x4000A000                 /* DMA mux 0 */
#define FC7300_FCSMU_BASE       0x4000F000                 /* Fault Control and Safety Management Unit */
#define FC7300_SEC_BASE         0x40014000                 /* Security controller */
#define FC7300_ERM_BASE         0x40018000                 /* Error Reporting Module */
#define FC7300_EIM_BASE         0x40019000                 /* Error Injection Module */
#define FC7300_INTM0_BASE       0x4001A000                 /* Interrupt Monitor 0 */
#define FC7300_ISM_BASE         0x4001B000                 /* Interface Safety Monitor  */
#define FC7300_FMC_BASE         0x4001E000                 /* Flash memory controller */
#define FC7300_FC_BASE          0x40020000                 /* Flash controller */
#define FC7300_WDOG0_BASE       0x40022000                 /* Software watchdog 0 */
#define FC7300_SCG_BASE         0x40023000                 /* System Clock Generator */
#define FC7300_PCC_BASE         0x40024000                 /* Peripheral Clock Control */
#define FC7300_TRGSEL0_BASE     0x40026000                 /* Trigger Multiplexing Selection 0 */
#define FC7300_TRGSEL1_BASE     0x40027000                 /* Trigger Multiplexing Selection 1 */
#define FC7300_TRGSEL2_BASE     0x40028000                 /* Trigger Multiplexing Selection 2 */
#define FC7300_TRGSEL3_BASE     0x40029000                 /* Trigger Multiplexing Selection 3 */
#define FC7300_CRC0_BASE        0x4002A000                 /* Cyclic Redundancy Check 0 */
#define FC7300_CORDIC_BASE      0x4002B000                 /* CORDIC Accelerator */
#define FC7300_TSTMP0_BASE      0x4002C000                 /* Timer Stamp 0 */
#define FC7300_TSTMP1_BASE      0x4002D000                 /* Timer Stamp 1 */
#define FC7300_FCPIT0_BASE      0x4002E000                 /* FC Programmable Interrupt Timer 0 */
#define FC7300_AONTIMER_BASE    0x4002F000                 /* Always-on Timer */
#define FC7300_RTC_BASE         0x40030000                 /* Real-Time Clock */
#define FC7300_CMU0_BASE        0x40031000                 /* Clock Monitor Unit 0 */
#define FC7300_CMU1_BASE        0x40032000                 /* Clock Monitor Unit 1 */
#define FC7300_CMU2_BASE        0x40033000                 /* Clock Monitor Unit 2 */
#define FC7300_CMU3_BASE        0x40034000                 /* Clock Monitor Unit 3 */
#define FC7300_CMU4_BASE        0x40035000                 /* Clock Monitor Unit 4 */
#define FC7300_PTIMER0_BASE     0x40037000                 /* Programmable Timer 0 */
#define FC7300_PTIMER1_BASE     0x40038000                 /* Programmable Timer 1 */
#define FC7300_ADC0_BASE        0x4003B000                 /* Analog-to-Digital Converter 0 */
#define FC7300_ADC1_BASE        0x4003C000                 /* Analog-to-Digital Converter 1 */
#define FC7300_WKU_BASE         0x4003F000                 /* Wake-up Unit */
#define FC7300_CMP0_BASE        0x40040000                 /* Analog Comparator 0 */
#define FC7300_CMP1_BASE        0x40041000                 /* Analog Comparator 1 */
#define FC7300_CMP2_BASE        0x40042000                 /* Analog Comparator 2 */
#define FC7300_TMU_BASE         0x40043000                 /* Temperature Monitor Unit */
#define FC7300_PMC_BASE         0x40044000                 /* Power management controller */
#define FC7300_SMC_BASE         0x40045000                 /* System Mode controller */
#define FC7300_RGM_BASE         0x40046000                 /* Reset Generation Module */
#define FC7300_PORT_BASE(n)     (0x40047000 + ((n) << 12)) /* Port n multiplexing control */
#define FC7300_PORTA_BASE       0x40047000                 /* Port A multiplexing control */
#define FC7300_PORTB_BASE       0x40048000                 /* Port B multiplexing control */
#define FC7300_PORTC_BASE       0x40049000                 /* Port C multiplexing control */
#define FC7300_PORTD_BASE       0x4004A000                 /* Port D multiplexing control */
#define FC7300_PORTE_BASE       0x4004B000                 /* Port E multiplexing control */
#define FC7300_PORTF_BASE       0x4004C000                 /* Port F multiplexing control */
#define FC7300_PORTG_BASE       0x4004D000                 /* Port G multiplexing control */
#define FC7300_PORTH_BASE       0x4004E000                 /* Port H multiplexing control */
#define FC7300_PORTI_BASE       0x4004F000                 /* Port I multiplexing control */
#define FC7300_SENT0_BASE       0x40054000                 /* Single Edge Nibble Transmission 0 */
#define FC7300_MB_BASE          0x40058000                 /* Mailbox */
#define FC7300_FTU0_BASE        0x4005C000                 /* Flexible Timer Unit 0 */
#define FC7300_FTU1_BASE        0x4005D000                 /* Flexible Timer Unit 1 */
#define FC7300_FTU2_BASE        0x4005E000                 /* Flexible Timer Unit 2 */
#define FC7300_FTU3_BASE        0x4005F000                 /* Flexible Timer Unit 3 */
#define FC7300_FTU4_BASE        0x40060000                 /* Flexible Timer Unit 4 */
#define FC7300_FTU5_BASE        0x40061000                 /* Flexible Timer Unit 5 */
#define FC7300_FCSPI0_BASE      0x40062000                 /* FC Serial Peripheral Interface 0 */
#define FC7300_FCSPI1_BASE      0x40063000                 /* FC Serial Peripheral Interface 1 */
#define FC7300_FCSPI2_BASE      0x40064000                 /* FC Serial Peripheral Interface 2 */
#define FC7300_FCSPI3_BASE      0x40065000                 /* FC Serial Peripheral Interface 3 */
#define FC7300_FCIIC0_BASE      0x40066000                 /* FC Inter-Integrated Circuit 0 */
#define FC7300_FCUART0_BASE     0x40068000                 /* FCUART 0 */
#define FC7300_FCUART1_BASE     0x40069000                 /* FCUART 1 */
#define FC7300_FCUART2_BASE     0x4006A000                 /* FCUART 2 */
#define FC7300_FCUART3_BASE     0x4006B000                 /* FCUART 3 */
#define FC7300_FCUART4_BASE     0x4006C000                 /* FCUART 4 */
#define FC7300_FCUART5_BASE     0x4006D000                 /* FCUART 5 */
#define FC7300_FCUART6_BASE     0x4006E000                 /* FCUART 6 */
#define FC7300_FCUART7_BASE     0x4006F000                 /* FCUART 7 */
#define FC7300_LU0_BASE         0x40070000                 /* Logic Unit 0 */
#define FC7300_LU1_BASE         0x40071000                 /* Logic Unit 1 */
#define FC7300_SCM_BASE         0x40072000                 /* System Controller Module */
#define FC7300_ENET_BASE        0x40074000                 /* Ethernet */
#define FC7300_FREQM_BASE       0x40078000                 /* Frequency Measurement */
#define FC7300_CSC0_BASE        0x40079000                 /* CPU System Control 0 */
#define FC7300_CSC1_BASE        0x4007A000                 /* CPU System Control 1 */
#define FC7300_CSC2_BASE        0x4007B000                 /* CPU System Control 2 */
#define FC7300_STCU_BASE        0x4007F000                 /* Safety Test and Control Unit */
#define FC7300_FLEXCAN0_BASE    0x40080000                 /* FLEX Controller Area Network 0 */
#define FC7300_FLEXCAN1_BASE    0x40084000                 /* FLEX Controller Area Network 1 */
#define FC7300_FLEXCAN2_BASE    0x40088000                 /* FLEX Controller Area Network 2 */
#define FC7300_FLEXCAN3_BASE    0x4008C000                 /* FLEX Controller Area Network 3 */
#define FC7300_FLEXCAN4_BASE    0x40090000                 /* FLEX Controller Area Network 4 */

#define FC7300_AFCB1_BASE       0x40400000                 /* Peripheral bridge1 (AFCB1) */

#define FC7300_DMA1_BASE        0x40408000                 /* DMA controller 1 */
#define FC7300_DMACFG1_BASE     0x40409000                 /* DMA configuration 1 */
#define FC7300_DMAMUX1_BASE     0x4040A000                 /* DMA mux 1 */
#define FC7300_MAM0_BASE        0x4040D000                 /* Matrix Access Monitor 0 */
#define FC7300_MAM1_BASE        0x4040E000                 /* Matrix Access Monitor 1 */
#define FC7300_MAM2_BASE        0x4040F000                 /* Matrix Access Monitor 2 */
#define FC7300_INTM1_BASE       0x40415000                 /* Interrupt Monitor 1 */
#define FC7300_INTM2_BASE       0x40416000                 /* Interrupt Monitor 2 */
#define FC7300_PTIMER2_BASE     0x40422000                 /* Programmable Timer 2 */
#define FC7300_PTIMER3_BASE     0x40423000                 /* Programmable Timer 3 */
#define FC7300_ADC2_BASE        0x40426000                 /* Analog-to-Digital Converter 2 */
#define FC7300_ADC3_BASE        0x40427000                 /* Analog-to-Digital Converter 3 */
#define FC7300_SDDF_BASE        0x4042B000                 /* Sigma Delta Digital Filter */
#define FC7300_CRC1_BASE        0x40432000                 /* Cyclic Redundancy Check 1 */
#define FC7300_WDOG1_BASE       0x40433000                 /* Software Watchdog 1 */
#define FC7300_WDOG2_BASE       0x40434000                 /* Software Watchdog 2 */
#define FC7300_FCPIT1_BASE      0x40437000                 /* FC Progammable Interrupt Timer 1 */
#define FC7300_TSTMP2_BASE      0x40438000                 /* Timer Stamp 2 */
#define FC7300_TSTMP3_BASE      0x40439000                 /* Timer Stamp 3 */
#define FC7300_OSPI_BASE        0x4043D000                 /* Octal Serial Peripheral Interface */
#define FC7300_FCUART16_BASE    0x40446000                 /* FCUART 16 */
#define FC7300_FCUART17_BASE    0x40447000                 /* FCUART 17 */
#define FC7300_FCSPI6_BASE      0x40448000                 /* FC Serial Peripheral Interface 6 */
#define FC7300_FCSPI7_BASE      0x40449000                 /* FC Serial Peripheral Interface 7 */
#define FC7300_SENT1_BASE       0x40454000                 /* Single Edge Nibble Transmission 1 */
#define FC7300_FTU6_BASE        0x4045F000                 /* Flexible Timer Unit 6 */
#define FC7300_FTU7_BASE        0x40460000                 /* Flexible Timer Unit 7 */
#define FC7300_FTU8_BASE        0x40461000                 /* Flexible Timer Unit 8 */
#define FC7300_FTU9_BASE        0x40462000                 /* Flexible Timer Unit 9 */
#define FC7300_FTU10_BASE       0x40463000                 /* Flexible Timer Unit 10 */
#define FC7300_FTU11_BASE       0x40464000                 /* Flexible Timer Unit 11 */
#define FC7300_FCIIC1_BASE      0x40467000                 /* FC Inter-Integrated Circuit 1 */
#define FC7300_FCUART8_BASE     0x40468000                 /* FCUART 8 */
#define FC7300_FCUART9_BASE     0x40469000                 /* FCUART 9 */
#define FC7300_FCUART10_BASE    0x4046A000                 /* FCUART 10 */
#define FC7300_FCUART11_BASE    0x4046B000                 /* FCUART 11 */
#define FC7300_FCUART12_BASE    0x4046C000                 /* FCUART 12 */
#define FC7300_FCUART13_BASE    0x4046D000                 /* FCUART 13 */
#define FC7300_FCUART14_BASE    0x4046E000                 /* FCUART 14 */
#define FC7300_FCUART15_BASE    0x4046F000                 /* FCUART 15 */
#define FC7300_FCSPI4_BASE      0x40470000                 /* FC Serial Peripheral Interface 4 */
#define FC7300_FCSPI5_BASE      0x40471000                 /* FC Serial Peripheral Interface 5 */
#define FC7300_MSC0_BASE        0x40474000                 /* Micro Second Channel 0 */
#define FC7300_MSC1_BASE        0x40475000                 /* Micro Second Channel 1 */
#define FC7300_FLEXCAN5_BASE    0x40480000                 /* FLEX Controller Area Network 5 */
#define FC7300_FLEXCAN6_BASE    0x40484000                 /* FLEX Controller Area Network 6 */
#define FC7300_FLEXCAN7_BASE    0x40488000                 /* FLEX Controller Area Network 7 */
#define FC7300_FLEXCAN8_BASE    0x4048C000                 /* FLEX Controller Area Network 8 */
#define FC7300_FLEXCAN9_BASE    0x40490000                 /* FLEX Controller Area Network 9 */

#define FC7300_AHB_BASE         0x41000000                 /* Advanced High-Performance Bus (AHB) */

#define FC7300_GPIO_BASE(n)     (0x41000000 +((n) << 6))   /* GPIO controller */
#define FC7300_GPIOA_BASE       0x41000000                 /* GPIOA controller */
#define FC7300_GPIOB_BASE       0x41000040                 /* GPIOB controller */
#define FC7300_GPIOC_BASE       0x41000080                 /* GPIOC controller */
#define FC7300_GPIOD_BASE       0x410000C0                 /* GPIOD controller */
#define FC7300_GPIOE_BASE       0x41000100                 /* GPIOE controller */
#define FC7300_GPIOF_BASE       0x41000140                 /* GPIOF controller */
#define FC7300_GPIOG_BASE       0x41000180                 /* GPIOG controller */
#define FC7300_GPIOH_BASE       0x410001C0                 /* GPIOH controller */
#define FC7300_GPIOI_BASE       0x41000200                 /* GPIOI controller */

#define FC7300_CPM_BASE         0xE0080000                 /* CPU Private Module */

#define FC7300_CORE0_DTCM_BACKDOOR_OFFSET 0x02000000       /* Core 0 DTCM backdoor address offset */
#define FC7300_CORE0_ITCM_BACKDOOR_OFFSET 0x24000000       /* Core 0 ITCM backdoor address offset */
#define FC7300_CORE1_DTCM_BACKDOOR_OFFSET 0x02100000       /* Core 1 DTCM backdoor address offset */
#define FC7300_CORE1_ITCM_BACKDOOR_OFFSET 0x24100000       /* Core 1 ITCM backdoor address offset */
#define FC7300_CORE2_DTCM_BACKDOOR_OFFSET 0x02200000       /* Core 2 DTCM backdoor address offset */
#define FC7300_CORE2_ITCM_BACKDOOR_OFFSET 0x24200000       /* Core 2 ITCM backdoor address offset */

#endif /* __VENDOR_FLAGCHIP_CHIPS_FC7300_HARDWARE_FC7300_MEMORYMAP_H */
