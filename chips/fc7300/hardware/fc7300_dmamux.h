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

#ifndef __VENDOR_FLAGCHIP_CHIPS_FC7300_HARDWARE_FC7300_DMAMUX_H
#define __VENDOR_FLAGCHIP_CHIPS_FC7300_HARDWARE_FC7300_DMAMUX_H

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* DMAMUX Register Offsets **************************************************/

#define FC7300_DMAMUX_CHCFG_OFFSET(n)             (n)  /* Channel Configuration register, n=1..31 */

#define FC7300_DMAMUX_CHTRG_OFFSET                0x20 /* Channel Triger register */

/* DMAMUX Register Bitfield Definitions *************************************/

/* Channel Configuration register, n=1..31 */

#define DMAMUX_CHCFG_SOURCE_SHIFT                 (0)       /* Bits 0-5:  DMA Channel Source (Slot) */
#define DMAMUX_CHCFG_SOURCE_MASK                  (0x3f << DMAMUX_CHCFG_SOURCE_SHIFT)
#  define DMAMUX_CHCFG_SOURCE(s)                  ((uint32_t)(s) << DMAMUX_CHCFG_SOURCE_SHIFT) /* chip-specific */

#define DMAMUX_CHCFG_TRIG                         (1 << 6)  /* Bit 6:  DMA Channel Trigger Enable */
#define DMAMUX_CHCFG_ENBL                         (1 << 7)  /* Bit 7:  DMA Channel Enable */

/* Peripheral DMA request channels */

#define FC7300_DMA_REQ_DISABLED                   0   /* Disabled DMA Request */
#define FC7300_DMA_REQ_FCIIC0_RX                  1   /* FCIIC0 DMA RX Request */
#define FC7300_DMA_REQ_FCIIC0_TX                  2   /* FCIIC0 DMA TX Request */
#define FC7300_DMA_REQ_FCIIC1_RX                  3   /* FCIIC1 DMA RX Request */
#define FC7300_DMA_REQ_FCIIC1_TX                  4   /* FCIIC1 DMA TX Request */
#define FC7300_DMA_REQ_FCSPI0_RX                  5   /* FCSPI0 DMA RX Request */
#define FC7300_DMA_REQ_FCSPI0_TX                  6   /* FCSPI0 DMA TX Request */
#define FC7300_DMA_REQ_FCSPI1_RX                  7   /* FCSPI1 DMA RX Request */
#define FC7300_DMA_REQ_FCSPI1_TX                  8   /* FCSPI1 DMA TX Request */
#define FC7300_DMA_REQ_FCSPI2_RX                  9   /* FCSPI2 DMA RX Request */
#define FC7300_DMA_REQ_FCSPI2_TX                  10  /* FCSPI2 DMA TX Request */
#define FC7300_DMA_REQ_FCSPI3_RX                  11  /* FCSPI3 DMA RX Request */
#define FC7300_DMA_REQ_FCSPI3_TX                  12  /* FCSPI3 DMA TX Request */
#define FC7300_DMA_REQ_FCSPI4_RX                  13  /* FCSPI4 DMA RX Request */
#define FC7300_DMA_REQ_FCSPI4_TX                  14  /* FCSPI4 DMA TX Request */
#define FC7300_DMA_REQ_FCSPI5_RX                  15  /* FCSPI5 DMA RX Request */
#define FC7300_DMA_REQ_FCSPI5_TX                  16  /* FCSPI5 DMA TX Request */
#define FC7300_DMA_REQ_FCSPI6_RX                  17  /* FCSPI6 DMA RX Request */
#define FC7300_DMA_REQ_FCSPI6_TX                  18  /* FCSPI6 DMA TX Request */
#define FC7300_DMA_REQ_FCSPI7_RX                  19  /* FCSPI7 DMA RX Request */
#define FC7300_DMA_REQ_FCSPI7_TX                  20  /* FCSPI7 DMA TX Request */
#define FC7300_DMA_REQ_OSPI_RX                    21  /* OSPI DMA RX Request */
#define FC7300_DMA_REQ_OSPI_TX                    22  /* OSPI DMA RX Request */
#define FC7300_DMA_REQ_FLEXCAN0                   23  /* FLEXCAN0 DMA request */
#define FC7300_DMA_REQ_FLEXCAN1                   24  /* FLEXCAN1 DMA request */
#define FC7300_DMA_REQ_FLEXCAN2                   25  /* FLEXCAN2 DMA request */
#define FC7300_DMA_REQ_FLEXCAN3                   26  /* FLEXCAN3 DMA request */
#define FC7300_DMA_REQ_FLEXCAN4                   27  /* FLEXCAN4 DMA request */
#define FC7300_DMA_REQ_FLEXCAN5                   28  /* FLEXCAN5 DMA request */
#define FC7300_DMA_REQ_FLEXCAN6                   29  /* FLEXCAN6 DMA request */
#define FC7300_DMA_REQ_FLEXCAN7                   30  /* FLEXCAN7 DMA request */
#define FC7300_DMA_REQ_FLEXCAN8                   31  /* FLEXCAN8 DMA request */
#define FC7300_DMA_REQ_FLEXCAN9                   32  /* FLEXCAN9 DMA request */
#define FC7300_DMA_REQ_ADC0                       33  /* ADC0 DMA Request */
#define FC7300_DMA_REQ_ADC1                       34  /* ADC1 DMA Request */
#define FC7300_DMA_REQ_ADC2                       35  /* ADC2 DMA Request */
#define FC7300_DMA_REQ_ADC3                       36  /* ADC3 DMA Request */
#define FC7300_DMA_REQ_SDDR0                      37  /* SDDF0 DMA Request */
#define FC7300_DMA_REQ_SDDR1                      38  /* SDDF1 DMA Request */
#define FC7300_DMA_REQ_SDDR2                      39  /* SDDF2 DMA Request */
#define FC7300_DMA_REQ_SDDR3                      40  /* SDDF3 DMA Request */
#define FC7300_DMA_REQ_FCUART0_RX                 41  /* FCUART0 RX DMA Request */
#define FC7300_DMA_REQ_FCUART0_TX                 42  /* FCUART0 TX DMA Request */
#define FC7300_DMA_REQ_FCUART1_RX                 43  /* FCUART1 RX DMA Request */
#define FC7300_DMA_REQ_FCUART1_TX                 44  /* FCUART1 TX DMA Request */
#define FC7300_DMA_REQ_FCUART2_RX                 45  /* FCUART2 RX DMA Request */
#define FC7300_DMA_REQ_FCUART2_TX                 46  /* FCUART2 TX DMA Request */
#define FC7300_DMA_REQ_FCUART3_RX                 47  /* FCUART3 RX DMA Request */
#define FC7300_DMA_REQ_FCUART3_TX                 48  /* FCUART3 TX DMA Request */
#define FC7300_DMA_REQ_FCUART4_RX                 49  /* FCUART4 RX DMA Request */
#define FC7300_DMA_REQ_FCUART4_TX                 50  /* FCUART4 TX DMA Request */
#define FC7300_DMA_REQ_FCUART5_RX                 51  /* FCUART5 RX DMA Request */
#define FC7300_DMA_REQ_FCUART5_TX                 52  /* FCUART5 TX DMA Request */
#define FC7300_DMA_REQ_FCUART6_RX                 53  /* FCUART6 RX DMA Request */
#define FC7300_DMA_REQ_FCUART6_TX                 54  /* FCUART6 TX DMA Request */
#define FC7300_DMA_REQ_FCUART7_RX                 55  /* FCUART7 RX DMA Request */
#define FC7300_DMA_REQ_FCUART7_TX                 56  /* FCUART7 TX DMA Request */
#define FC7300_DMA_REQ_FCUART8_RX                 57  /* FCUART8 RX DMA Request */
#define FC7300_DMA_REQ_FCUART8_TX                 58  /* FCUART8 TX DMA Request */
#define FC7300_DMA_REQ_FCUART9_RX                 59  /* FCUART9 RX DMA Request */
#define FC7300_DMA_REQ_FCUART9_TX                 60  /* FCUART9 TX DMA Request */
#define FC7300_DMA_REQ_FCUART10_RX                61  /* FCUART10 RX DMA Request */
#define FC7300_DMA_REQ_FCUART10_TX                62  /* FCUART10 TX DMA Request */
#define FC7300_DMA_REQ_FCUART11_RX                63  /* FCUART11 RX DMA Request */
#define FC7300_DMA_REQ_FCUART11_TX                64  /* FCUART11 TX DMA Request */
#define FC7300_DMA_REQ_FCUART12_RX                65  /* FCUART12 RX DMA Request */
#define FC7300_DMA_REQ_FCUART12_TX                66  /* FCUART12 TX DMA Request */
#define FC7300_DMA_REQ_FCUART13_RX                67  /* FCUART13 RX DMA Request */
#define FC7300_DMA_REQ_FCUART13_TX                68  /* FCUART13 TX DMA Request */
#define FC7300_DMA_REQ_FCUART14_RX                69  /* FCUART14 RX DMA Request */
#define FC7300_DMA_REQ_FCUART14_TX                70  /* FCUART14 TX DMA Request */
#define FC7300_DMA_REQ_FCUART15_RX                71  /* FCUART15 RX DMA Request */
#define FC7300_DMA_REQ_FCUART15_TX                72  /* FCUART15 TX DMA Request */
#define FC7300_DMA_REQ_FCUART16_RX                73  /* FCUART16 RX DMA Request */
#define FC7300_DMA_REQ_FCUART16_TX                74  /* FCUART16 TX DMA Request */
#define FC7300_DMA_REQ_FCUART17_RX                75  /* FCUART17 RX DMA Request */
#define FC7300_DMA_REQ_FCUART17_TX                76  /* FCUART17 TX DMA Request */
#define FC7300_DMA_REQ_PORTA                      77  /* PORTA DMA request */
#define FC7300_DMA_REQ_PORTB                      78  /* PORTB DMA request */
#define FC7300_DMA_REQ_PORTC                      79  /* PORTC DMA request */
#define FC7300_DMA_REQ_PORTD                      80  /* PORTD DMA request */
#define FC7300_DMA_REQ_PORTE                      81  /* PORTE DMA request */
#define FC7300_DMA_REQ_PORTF                      82  /* PORTF DMA request */
#define FC7300_DMA_REQ_PORTG                      83  /* PORTG DMA request */
#define FC7300_DMA_REQ_PORTH                      84  /* PORTH DMA request */
#define FC7300_DMA_REQ_PORTI                      85  /* PORTI DMA request */
#define FC7300_DMA_REQ_AONTIMER0                  86  /* AONTIMER0 DMA request */
#define FC7300_DMA_REQ_CMP0                       87  /* CMP0 DMA request */
#define FC7300_DMA_REQ_CMP1                       88  /* CMP1 DMA request */
#define FC7300_DMA_REQ_CMP2                       89  /* CMP2 DMA request */
#define FC7300_DMA_REQ_PTIMER0                    90  /* Ptimer0 DMA request */
#define FC7300_DMA_REQ_PTIMER1                    91  /* Ptimer1 DMA request */
#define FC7300_DMA_REQ_PTIMER2                    92  /* Ptimer2 DMA request */
#define FC7300_DMA_REQ_PTIMER3                    93  /* Ptimer3 DMA request */
#define FC7300_DMA_REQ_ENET_PPS0                  94  /* Enet PPS Timer Channel0 DMA Request */
#define FC7300_DMA_REQ_ENET_PPS1                  95  /* Enet PPS Timer Channel1 DMA Request */
#define FC7300_DMA_REQ_ENET_PPS2                  96  /* Enet PPS Timer Channel2 DMA Request */
#define FC7300_DMA_REQ_ENET_PPS3                  97  /* Enet PPS Timer Channel3 DMA Request */
#define FC7300_DMA_REQ_FTU0_ALL_CH_OR             98  /* FTU0 All Channels OR DMA Request */
#define FC7300_DMA_REQ_FTU1_ALL_CH_OR             99  /* FTU1 All Channels OR DMA Request */
#define FC7300_DMA_REQ_FTU2_ALL_CH_OR             100 /* FTU2 All Channels OR DMA Request */
#define FC7300_DMA_REQ_FTU3_ALL_CH_OR             101 /* FTU3 All Channels OR DMA Request */
#define FC7300_DMA_REQ_FTU4_ALL_CH_OR             102 /* FTU4 All Channels OR DMA Request */
#define FC7300_DMA_REQ_FTU5_ALL_CH_OR             103 /* FTU5 All Channels OR DMA Request */
#define FC7300_DMA_REQ_FTU6_ALL_CH_OR             104 /* FTU6 All Channels OR DMA Request */
#define FC7300_DMA_REQ_FTU7_ALL_CH_OR             105 /* FTU7 All Channels OR DMA Request */
#define FC7300_DMA_REQ_FTU8_ALL_CH_OR             106 /* FTU8 All Channels OR DMA Request */
#define FC7300_DMA_REQ_FTU9_ALL_CH_OR             107 /* FTU9 All Channels OR DMA Request */
#define FC7300_DMA_REQ_FTU10_ALL_CH_OR            108 /* FTU10 All Channels OR DMA Request */
#define FC7300_DMA_REQ_FTU11_ALL_CH_OR            109 /* FTU11 All Channels OR DMA Request */
#define FC7300_DMA_REQ_SENT0_CH0_FAST             110 /* Sent0 channel 0 Fast DMA Requeset */
#define FC7300_DMA_REQ_SENT0_CH1_FAST             111 /* Sent0 channel 1 Fast DMA Requeset */
#define FC7300_DMA_REQ_SENT0_CH2_FAST             112 /* Sent0 channel 2 Fast DMA Requeset */
#define FC7300_DMA_REQ_SENT0_CH3_FAST             113 /* Sent0 channel 3 Fast DMA Requeset */
#define FC7300_DMA_REQ_SENT1_CH0_FAST             114 /* Sent1 channel 0 Fast DMA Requeset */
#define FC7300_DMA_REQ_SENT1_CH1_FAST             115 /* Sent1 channel 1 Fast DMA Requeset */
#define FC7300_DMA_REQ_SENT1_CH2_FAST             116 /* Sent1 channel 2 Fast DMA Requeset */
#define FC7300_DMA_REQ_SENT1_CH3_FAST             117 /* Sent1 channel 3 Fast DMA Requeset */
#define FC7300_DMA_REQ_SENT0_CH0_SLOW             118 /* Sent0 channel 0 Slow DMA Requeset */
#define FC7300_DMA_REQ_SENT0_CH1_SLOW             119 /* Sent0 channel 1 Slow DMA Requeset */
#define FC7300_DMA_REQ_SENT0_CH2_SLOW             120 /* Sent0 channel 2 Slow DMA Requeset */
#define FC7300_DMA_REQ_SENT0_CH3_SLOW             121 /* Sent0 channel 3 Slow DMA Requeset */
#define FC7300_DMA_REQ_SENT1_CH0_SLOW             122 /* Sent1 channel 0 Slow DMA Requeset */
#define FC7300_DMA_REQ_SENT1_CH1_SLOW             123 /* Sent1 channel 1 Slow DMA Requeset */
#define FC7300_DMA_REQ_SENT1_CH2_SLOW             124 /* Sent1 channel 2 Slow DMA Requeset */
#define FC7300_DMA_REQ_SENT1_CH3_SLOW             125 /* Sent1 channel 3 Slow DMA Requeset */
#define FC7300_DMA_REQ_DMAMUX_ALWAYS_ENABLED0     126 /* Always On DMA request */
#define FC7300_DMA_REQ_DMAMUX_ALWAYS_ENABLED1     127 /* Always On DMA request */

#endif /* __VENDOR_FLAGCHIP_CHIPS_FC7300_HARDWARE_FC7300_DMAMUX_H */
