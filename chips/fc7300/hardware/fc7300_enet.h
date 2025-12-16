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

#ifndef __VENDOR_FLAGCHIP_CHIPS_FC7300_HARDWARE_FC7300_ENET_H
#define __VENDOR_FLAGCHIP_CHIPS_FC7300_HARDWARE_FC7300_ENET_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>
#include "hardware/fc7300_memorymap.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Register offsets *********************************************************/

#define FC7300_ENET_MAC_CONFIGURATION_OFFSET                0x0000                /* MAC Configuration Register */
#define FC7300_ENET_MAC_EXT_CONFIGURATION_OFFSET            0x0004                /* MAC Extended Configuration Register */
#define FC7300_ENET_MAC_PACKET_FILTER_OFFSET                0x0008                /* MAC Packet Filter Register */
#define FC7300_ENET_MAC_WATCHDOG_TIMEOUT_OFFSET             0x000c                /* MAC Watchdog Timeout Register */
#define FC7300_ENET_MAC_HASH_TABLE_REG0_OFFSET              0x0010                /* MAC Hash Table First 32 Bits Register */
#define FC7300_ENET_MAC_HASH_TABLE_REG1_OFFSET              0x0014                /* MAC Hash Table Second 32 Bits Register */
#define FC7300_ENET_MAC_VLAN_TAG_CTRL_OFFSET                0x0050                /* MAC VLAN Tag Control Register */
#define FC7300_ENET_MAC_VLAN_TAG_DATA_OFFSET                0x0054                /* MAC VLAN Tag Data Register */
#define FC7300_ENET_MAC_VLAN_HASH_TABLE_OFFSET              0x0058                /* MAC VLAN Hash Table Register */
#define FC7300_ENET_MAC_VLAN_INCL_OFFSET                    0x0060                /* MAC VLAN Inclusion Or Replacement Register */
#define FC7300_ENET_MAC_INNER_VLAN_INCL_OFFSET              0x0064                /* Inner VLAN Tag Inclusion Or Replacement Register */
#define FC7300_ENET_MAC_Q0_TX_FLOW_CTRL_OFFSET              0x0070                /* MAC Q0 Tx Flow Control Register */
#define FC7300_ENET_MAC_RX_FLOW_CTRL_OFFSET                 0x0090                /* MAC Receive Flow Control Register */
#define FC7300_ENET_MAC_RXQ_CTRL4_OFFSET                    0x0094                /* MAC RxQ Control 4 Register */
#define FC7300_ENET_MAC_RXQ_CTRL0_OFFSET                    0x00a0                /* MAC RxQ Control 0 Register */
#define FC7300_ENET_MAC_RXQ_CTRL1_OFFSET                    0x00a4                /* MAC RxQ Control 1 Register */
#define FC7300_ENET_MAC_RXQ_CTRL2_OFFSET                    0x00a8                /* MAC RxQ Control 2 Register */
#define FC7300_ENET_MAC_INTERRUPT_STATUS_OFFSET             0x00b0                /* MAC Interrupt Status Register */
#define FC7300_ENET_MAC_INTERRUPT_ENABLE_OFFSET             0x00b4                /* MAC Interrupt Enable Register */
#define FC7300_ENET_MAC_DPP_FSM_INT_STATUS_OFFSET           0x0140                /* MAC DPP FSM Interrupt Status Register */
#define FC7300_ENET_MAC_FSM_CONTROL_OFFSET                  0x0148                /* MAC FSM Control Register */
#define FC7300_ENET_MAC_FSM_ACT_TIMER_OFFSET                0x014c                /* MAC FSM ACT Timer Register */
#define FC7300_ENET_MAC_MDIO_ADDRESS_OFFSET                 0x0200                /* MAC MDIO Address Register */
#define FC7300_ENET_MAC_MDIO_DATA_OFFSET                    0x0204                /* MAC MDIO Data Register */
#define FC7300_ENET_MAC_FPE_CTRL_STS_OFFSET                 0x0234                /* MAC FPE Control STS Register */
#define FC7300_ENET_MAC_ADDRESS0_HIGH_OFFSET                0x0300                /* MAC Address 0 High Register */
#define FC7300_ENET_MAC_ADDRESS0_LOW_OFFSET                 0x0304                /* MAC Address 0 Low Register */
#define FC7300_ENET_MMC_CONTROL_OFFSET                      0x0700                /* MMC Control Register */
#define FC7300_ENET_MMC_RX_INTERRUPT_OFFSET                 0x0704                /* MMC Receive Interrupt Register */
#define FC7300_ENET_MMC_TX_INTERRUPT_OFFSET                 0x0708                /* MMC Transmit Interrupt Register */
#define FC7300_ENET_MMC_RX_INTERRUPT_MASK_OFFSET            0x070c                /* MMC Receive Interrupt Mask Register */
#define FC7300_ENET_MMC_TX_INTERRUPT_MASK_OFFSET            0x0710                /* MMC Transmit Interrupt Mask Register */
#define FC7300_ENET_TX_OCTET_COUNT_GOOD_BAD_OFFSET          0x0714                /* Transmit Octet Count Good Bad Register */
#define FC7300_ENET_TX_PACKET_COUNT_GOOD_BAD_OFFSET         0x0718                /* Transmit Packet Count Good Bad Register */
#define FC7300_ENET_TX_BROADCAST_PKTS_GOOD_OFFSET           0x071c                /* Transmit Broadcast Packets Good Register */
#define FC7300_ENET_TX_MULTICAST_PKTS_GOOD_OFFSET           0x0720                /* Transmit Multicast Packets Good Register */
#define FC7300_ENET_TX_64_PKTS_GOOD_BAD_OFFSET              0x0724                /* Transmit 64-Octet Packets Good Bad Register */
#define FC7300_ENET_TX_65TO127_PKTS_GOOD_BAD_OFFSET         0x0728                /* Transmit 65 To 127 Octet Packets Good Bad Register */
#define FC7300_ENET_TX_128TO255_PKTS_GOOD_BAD_OFFSET        0x072c                /* Transmit 128 To 255 Octet Packets Good Bad Register */
#define FC7300_ENET_TX_256TO511_PKTS_GOOD_BAD_OFFSET        0x0730                /* Transmit 256 To 511 Octet Packets Good Bad Register */
#define FC7300_ENET_TX_512TO1023_PKTS_GOOD_BAD_OFFSET       0x0734                /* Transmit 512 To 1023 Octet Packets Good Bad Register */
#define FC7300_ENET_TX_1024TOMAX_PKTS_GOOD_BAD_OFFSET       0x0738                /* Transmit 1024 To Max Octet Packets Good Bad Register */
#define FC7300_ENET_TX_UNICAST_PKTS_GOOD_BAD_OFFSET         0x073c                /* Transmit Unicast Packets Good Bad Register */
#define FC7300_EENET_TX_MULTICAST_PKTS_GOOD_BAD_OFFSET      0x0740                /* Transmit Multicast Packets Good Bad Register */
#define FC7300_ENET_TX_BROADCAST_PKTS_GOOD_BAD_OFFSET       0x0744                /* Transmit Broadcast Packets Good Bad Register */
#define FC7300_ENET_TX_UNDERFLOW_ERROR_PKTS_OFFSET          0x0748                /* Transmit Underflow Error Packets Register */
#define FC7300_ENET_TX_SINGLE_COLLISION_GOOD_PKTS_OFFSET    0x074c                /* Transmit Single Collision Good Packets Register */
#define FC7300_ENET_TX_MULTI_COLLISION_GOOD_PKTS_OFFSET     0x0750                /* Transmit Multiple Collision Good Packets Register */
#define FC7300_ENET_TX_DEFERRED_PKTS_OFFSET                 0x0754                /* Transmit Deferred Packets Register */
#define FC7300_ENET_TX_LATE_COLLISION_PKTS_OFFSET           0x0758                /* Transmit Late Collision Packets Register */
#define FC7300_ENET_TX_EXCESSIVE_COLLISION_PKTS_OFFSET      0x075c                /* Transmit Excessive Collision Packets Register */
#define FC7300_ENET_TX_CARRIER_ERROR_PKTS_OFFSET            0x0760                /* Transmit Carrier Error Packets Register */
#define FC7300_ENET_TX_OCTET_COUNT_GOOD_OFFSET              0x0764                /* Transmit Octet Count Good Register */
#define FC7300_ENET_TX_PACKET_COUNT_GOOD_OFFSET             0x0768                /* Transmit Packet Count Good Register */
#define FC7300_ENET_TX_EXCESSIVE_DEFERRAL_ERROR_OFFSET      0x076c                /* Transmit Excessive Deferral Error Register */
#define FC7300_ENET_TX_PAUSE_PACKETS_OFFSET                 0x0770                /* Transmit Pause Packets Register */
#define FC7300_ENET_TX_VLAN_PACKETS_GOOD_OFFSET             0x0774                /* Transmit VLAN Packets Good Register */
#define FC7300_ENET_TX_OSIZE_PACKETS_GOOD_OFFSET            0x0778                /* Transmit O Size Packets Good Register */
#define FC7300_ENET_RX_PACKETS_COUNT_GOOD_BAD_OFFSET        0x0780                /* Receive Packets Count Good Bad Register */
#define FC7300_ENET_RX_OCTET_COUNT_GOOD_BAD_OFFSET          0x0784                /* Receive Octet Count Good Bad Register */
#define FC7300_ENET_RX_OCTET_COUNT_GOOD_OFFSET              0x0788                /* Receive Octet Count Good Register */
#define FC7300_ENET_RX_BROADCAST_PACKETS_GOOD_OFFSET        0x078c                /* Receive Broadcast Packets Good Register */
#define FC7300_ENET_RX_MULTICAST_PACKETS_GOOD_OFFSET        0x0790                /* Receive Multicast Packets Good Register */
#define FC7300_ENET_RX_CRC_ERROR_PACKETS_OFFSET             0x0794                /* Receive CRC Error Packets Register */
#define FC7300_ENET_RX_ALIGNMENT_ERROR_PACKETS_OFFSET       0x0798                /* Receive Alignment Error Packets Register */
#define FC7300_ENET_RX_RUNT_ERROR_PACKETS_OFFSET            0x079c                /* Receive Runt Error Packets Register */
#define FC7300_ENET_RX_JABBER_ERROR_PACKETS_OFFSET          0x07a0                /* Receive Jabber Error Packets Register */
#define FC7300_ENET_RX_UNDERSIZE_PACKETS_GOOD_OFFSET        0x07a4                /* Receive Undersize Packets Good Register */
#define FC7300_ENET_RX_OVERSIZE_PACKETS_GOOD_OFFSET         0x07a8                /* Receive Oversize Packets Good Register */
#define FC7300_ENET_RX_64_PKTS_GOOD_BAD_OFFSET              0x07ac                /* Receive 64 Octets Packets Good Bad Register */
#define FC7300_ENET_RX_65TO127_PKTS_GOOD_BAD_OFFSET         0x07b0                /* Receive 65-127 Octets Packets Good Bad Register */
#define FC7300_ENET_RX_128TO255_PKTS_GOOD_BAD_OFFSET        0x07b4                /* Receive 128-255 Octets Packets Good Bad Register */
#define FC7300_ENET_RX_256TO511_PKTS_GOOD_BAD_OFFSET        0x07b8                /* Receive 256-511 Octets Packets Good Bad Register */
#define FC7300_ENET_RX_512TO1023_PKTS_GOOD_BAD_OFFSET       0x07bc                /* Receive 512-1023 Octets Packets Good Bad Register */
#define FC7300_ENET_RX_1024TOMAX_PKTS_GOOD_BAD_OFFSET       0x07c0                /* Receive 1024 To Max Octets Good Bad Register */
#define FC7300_ENET_RX_UNICAST_PACKETS_GOOD_OFFSET          0x07c4                /* Receive Unicast Packets Good Register */
#define FC7300_ENET_RX_LENGTH_ERROR_PACKETS_OFFSET          0x07c8                /* Receive Length Error Packets Register */
#define FC7300_ENET_RX_OUT_OF_RANGE_TYPE_PACKETS_OFFSET     0x07cc                /* Receive Out of Range Type Packet Register */
#define FC7300_ENET_RX_PAUSE_PACKETS_OFFSET                 0x07d0                /* Receive Pause Packets Register */
#define FC7300_ENET_RX_FIFO_OVERFLOW_PACKETS_OFFSET         0x07d4                /* Receive FIFO Overflow Packets Register */
#define FC7300_ENET_RX_VLAN_PACKETS_GOOD_BAD_OFFSET         0x07d8                /* Receive VLAN Packets Good Bad Register */
#define FC7300_ENET_RX_WATCHDOG_ERROR_PACKETS_OFFSET        0x07dc                /* Receive Watchdog Error Packets Register */
#define FC7300_ENET_RX_RECEIVE_ERROR_PACKETS_OFFSET         0x07e0                /* Receive Receive Error Packets Register */
#define FC7300_ENET_RX_CONTROL_PACKETS_GOOD_OFFSET          0x07e4                /* Receive Control Packets Good Register */
#define FC7300_ENET_MAC_L3_L4_CONTROL_OFFSET(n)             (0x0900 + (n * 0x30)) /* MAC Layer 3 Layer 4 Control Register */
#define FC7300_ENET_MAC_LAYER4_ADDRESS_OFFSET(n)            (0x0904 + (n * 0x30)) /* MAC Layer 4 Address Register */
#define FC7300_ENET_MAC_LAYER3_ADDR0_REG_OFFSET(n)          (0x0910 + (n * 0x30)) /* MAC Layer 3 Address 0 Reg Register */
#define FC7300_ENET_MAC_LAYER3_ADDR1_REG_OFFSET(n)          (0x0914 + (n * 0x30)) /* MAC Layer 3 Address 1 Reg Register */
#define FC7300_ENET_MAC_LAYER3_ADDR2_REG_OFFSET(n)          (0x0918 + (n * 0x30)) /* MAC Layer 3 Address 2 Reg Register */
#define FC7300_ENET_MAC_LAYER3_ADDR3_REG_OFFSET(n)          (0x091c + (n * 0x30)) /* MAC Layer 3 Address 3 Reg Register */
#define FC7300_ENET_MAC_TIMESTAMP_CONTROL_OFFSET            0x0b00                /* MAC Timestamp Control Register */
#define FC7300_ENET_MAC_SUB_SECOND_INCREMENT_OFFSET         0x0b04                /* MAC Sub Second Increment Register */
#define FC7300_ENET_MAC_SYSTEM_TIME_SECONDS_OFFSET          0x0b08                /* MAC System Time In Seconds Register */
#define FC7300_ENET_MAC_SYSTEM_TIME_NANOSECONDS_OFFSET      0x0b0c                /* MAC System Time In Nanoseconds Register */
#define FC7300_ENET_MAC_SYSTEM_TIME_SEC_UPDATE_OFFSET       0x0b10                /* MAC System Time Seconds Update Register */
#define FC7300_ENET_MAC_SYSTEM_TIME_NSEC_UPDATE_OFFSET      0x0b14                /* MAC System Time Nanoseconds Update Register */
#define FC7300_ENET_MAC_TIMESTAMP_ADDEND_OFFSET             0x0b18                /* MAC Timestamp Addend Register */
#define FC7300_ENET_MAC_SYSTIME_HIGHER_WORD_SEC_OFFSET      0x0b1c                /* MAC System Time Higher Word In Seconds Register */
#define FC7300_ENET_MAC_TIMESTAMP_STATUS_OFFSET             0x0b20                /* MAC Timestamp Status Register */
#define FC7300_ENET_MAC_TX_TIMESTAMP_STATUS_NSEC_OFFSET     0x0b30                /* MAC Transmit Timestamp Status In Nanoseconds Register */
#define FC7300_ENET_MAC_TX_TIMESTAMP_STATUS_SEC_OFFSET      0x0b34                /* MAC Transmit Timestamp Status In Seconds Register */
#define FC7300_ENET_MAC_TSTAMP_INGRESS_ASYM_CORR_OFFSET     0x0b50                /* MAC Timestamp Ingress Asymmetry Correction Register */
#define FC7300_ENET_MAC_TSTAMP_EGRESS_ASYM_CORR_OFFSET      0x0b54                /* MAC Timestamp Egress Asymmetry Correction Register */
#define FC7300_ENET_MAC_TSTAMP_INGRESS_CORR_NSEC_OFFSET     0x0b58                /* MAC Timestamp Ingress Correction In Nanoseconds Register */
#define FC7300_ENET_MAC_TSTAMP_EGRESS_CORR_NSEC_OFFSET      0x0b5c                /* MAC Timestamp Egress Correction In Nanoseconds Register */
#define FC7300_ENET_MAC_TSTAMP_INGRESS_CORR_SUBNS_OFFSET    0x0b60                /* MAC Timestamp Ingress Correction In Subnanoseconds Register */
#define FC7300_ENET_MAC_TSTAMP_EGRESS_CORR_SUBNS_OFFSET     0x0b64                /* MAC Timestamp Engress Correction In Subnanoseconds Register */
#define FC7300_ENET_MAC_TIMESTAMP_INGRESS_LATENCY_OFFSET    0x0b68                /* MAC Timestamp Ingress Latency Register */
#define FC7300_ENET_MAC_TIMESTAMP_EGRESS_LATENCY_OFFSET     0x0b6c                /* MAC Timestamp Egress Latecy Register */
#define FC7300_ENET_MAC_PPS_CONTROL_OFFSET                  0x0b70                /* MAC PPS Control Register */
#define FC7300_ENET_MAC_PPS_TARGET_TIME_SEC_OFFSET(n)       (0x0b80 + (n * 0x10)) /* MAC PPS Target Time In Seconds Register */
#define FC7300_ENET_MAC_PPS_TARGET_TIME_NSEC_OFFSET(n)      (0x0b84 + (n * 0x10)) /* MAC PPS Target Time In Nanoseconds Register */
#define FC7300_ENET_MAC_PPS_INTERVAL_OFFSET(n)              (0x0b88 + (n * 0x10)) /* MAC PPS Interval Register */
#define FC7300_ENET_MAC_PPS_WIDTH_OFFSET(n)                 (0x0b8c + (n * 0x10)) /* MAC PPS Width Register */
#define FC7300_ENET_MTL_OPERATION_MODE_OFFSET               0x0c00                /* MTL Operation Mode Register */
#define FC7300_ENET_MTL_DBG_CTL_OFFSET                      0x0c08                /* MTL Debug Control Register */
#define FC7300_ENET_MTL_DBG_STS_OFFSET                      0x0c0c                /* MTL Debug Status Register */
#define FC7300_ENET_MTL_FIFO_DEBUG_DATA_OFFSET              0x0c10                /* MTL FIFO Debug Data Register */
#define FC7300_ENET_MTL_INTERRUPT_STATUS_OFFSET             0x0c20                /* MTL Interrupt Status Register */
#define FC7300_ENET_MTL_RXQ_DMA_MAP0_OFFSET                 0x0c30                /* MTL Receive Queue DMA Map 0 Register */
#define FC7300_ENET_MTL_TBS_CTRL_OFFSET                     0x0c40                /* MTL TBS Control Register */
#define FC7300_ENET_MTL_EST_CONTROL_OFFSET                  0x0c50                /* MTL EST Control Register */
#define FC7300_ENET_MTL_EST_EXT_CONTROL_OFFSET              0x0c54                /* MTL EST extended Control Register */
#define FC7300_ENET_MTL_EST_STATUS_OFFSET                   0x0c58                /* MTL EST Status Register */
#define FC7300_ENET_MTL_EST_SCH_ERROR_OFFSET                0x0c60                /* MTL EST Scheduling Error Register */
#define FC7300_ENET_MTL_EST_FRM_SIZE_ERROR_OFFSET           0x0c64                /* MTL EST Frame Size Error Register */
#define FC7300_ENET_MTL_EST_FRM_SIZE_CAPTURE_OFFSET         0x0c68                /* MTL EST Frame Size Capture Register */
#define FC7300_ENET_MTL_EST_INTR_ENABLE_OFFSET              0x0c70                /* MTL EST Interrupt Enable Register */
#define FC7300_ENET_MTL_EST_GCL_CONTROL_OFFSET              0x0c80                /* MTL EST GCL Control Register */
#define FC7300_ENET_MTL_EST_GCL_DATA_OFFSET                 0x0c84                /* MTL EST GCL Data Register */
#define FC7300_ENET_MTL_FPE_CTRL_STS_OFFSET                 0x0c90                /* MTL FPE Control Status Register */
#define FC7300_ENET_MTL_FPE_ADVANCE_OFFSET                  0x0c94                /* MTL FPE Advance Register */
#define FC7300_ENET_MTL_ECC_CONTROL_OFFSET                  0x0cc0                /* MTL ECC Control Register */
#define FC7300_ENET_MTL_SAFETY_INT_STATUS_OFFSET            0x0cc4                /* MTL Safety Interript Status Register */
#define FC7300_ENET_MTL_ECC_INT_ENABLE_OFFSET               0x0cc8                /* MTL ECC Interrupt Enable Register */
#define FC7300_ENET_MTL_ECC_INT_STATUS_OFFSET               0x0ccc                /* MTL ECC Interrupt Status Register */
#define FC7300_ENET_MTL_ECC_ERR_STS_RCTL_OFFSET             0x0cd0                /* MTL ECC Error Status Register */
#define FC7300_ENET_MTL_ECC_ERR_ADDR_STATUS_OFFSET          0x0cd4                /* MTL ECC Error Adress Status Register */
#define FC7300_ENET_MTL_ECC_ERR_CNTR_STATUS_OFFSET          0x0cd8                /* MTL ECC Error Control Status Register */
#define FC7300_ENET_MTL_DPP_CONTROL_OFFSET                  0x0ce0                /* MTL DPP Control Register */
#define FC7300_ENET_MTL_TXQ_OPERATION_MODE_OFFSET(q)        (0x0d00 + (q * 0x40)) /* MTL Tx Queue Operation Mode Register */
#define FC7300_ENET_MTL_TXQ_UNDERFLOW_OFFSET(q)             (0x0d04 + (q * 0x40)) /* MTL Tx Queue Underflow Register */
#define FC7300_ENET_MTL_TXQ_DEBUG_OFFSET(q)                 (0x0d08 + (q * 0x40)) /* MTL Tx Queue Debug Register */
#define FC7300_ENET_MTL_TXQ_ETS_CONTROL_OFFSET(q)           (0x0d10 + (q * 0x40)) /* MTL Tx Queue ETS Control Register */
#define FC7300_ENET_MTL_TXQ_ETS_STATUS_OFFSET(q)            (0x0d14 + (q * 0x40)) /* MTL Tx Queue ETS Status Register */
#define FC7300_ENET_MTL_TXQ_QUANTUM_WEIGHT_OFFSET(q)        (0x0d18 + (q * 0x40)) /* MTL Tx Queue Quantum Weight Register */
#define FC7300_ENET_MTL_TXQ_SENDSLOPE_OFFSET(q)             (0x0d1c + (q * 0x40)) /* MTL Tx Queue Sendslope Credit Register */
#define FC7300_ENET_MTL_TXQ_HICREDIT_OFFSET(q)              (0x0d20 + (q * 0x40)) /* MTL Tx Queue HiCredit Register */
#define FC7300_ENET_MTL_TXQ_LOCREDIT_OFFSET(q)              (0x0d24 + (q * 0x40)) /* MTL Tx Queue LoCredit Register */
#define FC7300_ENET_MTLQ_INT_CONTROL_STATUS_OFFSET(q)       (0x0d2c + (q * 0x40)) /* MTL Queue Interrupt Control Status Register */
#define FC7300_ENET_MTL_RXQ_OPERATION_MODE_OFFSET(q)        (0x0d30 + (q * 0x40)) /* MTL Rx Queue Operation Mode Register */
#define FC7300_ENET_MTL_RXQ_MISSED_PKT_OVFL_CNT_OFFSET(q)   (0x0d34 + (q * 0x40)) /* MTL Rx Queue Missed Packet Overflow Count Register */
#define FC7300_ENET_MTL_RXQ_DEBUG_OFFSET(q)                 (0x0d38 + (q * 0x40)) /* MTL Rx Queue Debug Register */
#define FC7300_ENET_MTL_RXQ_CONTROL_OFFSET(q)               (0x0d3c + (q * 0x40)) /* MTL Rx Queue Control Register */
#define FC7300_ENET_DMA_MODE_OFFSET                         0x1000                /* DMA Mode Register */
#define FC7300_ENET_DMA_SYSBUS_MODE_OFFSET                  0x1004                /* DMA System Bus Mode Register */
#define FC7300_ENET_DMA_INTERRUPT_STATUS_OFFSET             0x1008                /* DMA Interrupt Status Register */
#define FC7300_ENET_DMA_DEBUG_STATUS0_OFFSET                0x100c                /* DMA Debug Status 0 Register */
#define FC7300_ENET_DMA_TBS_CTRL_OFFSET                     0x1050                /* DMA TBS Control Register */
#define FC7300_ENET_DMA_SAFETY_INT_STATUS_OFFSET            0x1080                /* DMA Safety Interrupt Status Register */
#define FC7300_ENET_DMA_CH_CONTROL_OFFSET(c)                (0x1100 + (c * 0x80)) /* DMA Channel Control Register */
#define FC7300_ENET_DMA_CH_TX_CONTROL_OFFSET(c)             (0x1104 + (c * 0x80)) /* DMA Channel Tx Control Register */
#define FC7300_ENET_DMA_CH_RX_CONTROL_OFFSET(c)             (0x1108 + (c * 0x80)) /* DMA Channel Rx Control Register */
#define FC7300_ENET_DMA_CH_TXDESC_LIST_ADDR_OFFSET(c)       (0x1114 + (c * 0x80)) /* DMA Channel Tx Descriptor List Address Register */
#define FC7300_ENET_DMA_CH_RXDESC_LIST_ADDR_OFFSET(c)       (0x111c + (c * 0x80)) /* DMA Channel Rx Descriptor List Address Register */
#define FC7300_ENET_DMA_CH_TXDESC_TAIL_POINTER_OFFSET(c)    (0x1120 + (c * 0x80)) /* DMA Channel Tx Descriptor Tail Pointer Register */
#define FC7300_ENET_DMA_CH_RXDESC_TAIL_POINTER_OFFSET(c)    (0x1128 + (c * 0x80)) /* DMA Channel Rx Descriptor Tail Pointer Register */
#define FC7300_ENET_DMA_CH_TXDESC_RING_LENGTH_OFFSET(c)     (0x112c + (c * 0x80)) /* DMA Channel Tx Descriptor Ring Length Register */
#define FC7300_ENET_DMA_CH_RXDESC_RING_LENGTH_OFFSET(c)     (0x1130 + (c * 0x80)) /* DMA Channel Rx Descriptor Ring Length Register */
#define FC7300_ENET_DMA_CH_INTERRUPT_ENABLE_OFFSET(c)       (0x1134 + (c * 0x80)) /* DMA Channel Interrupt Enable Register */
#define FC7300_ENET_DMA_CH_RX_INT_WDOG_TIMER_OFFSET(c)      (0x1138 + (c * 0x80)) /* DMA Channel Rx Interrupt Watchdog Timer Register */
#define FC7300_ENET_DMA_CH_SLOT_FUNC_CTRL_STATUS_OFFSET(c)  (0x113c + (c * 0x80)) /* DMA Channel Slot Function Control Status Register */
#define FC7300_ENET_DMA_CH_CURRENT_APP_TXDESC_OFFSET(c)     (0x1144 + (c * 0x80)) /* DMA Channel Current Application Transmit Descriptor Register */
#define FC7300_ENET_DMA_CH_CURRENT_APP_RXDESC_OFFSET(c)     (0x114c + (c * 0x80)) /* DMA Channel Current Application Receive Descriptor Register */
#define FC7300_ENET_DMA_CH_CURRENT_APP_TXBUF_OFFSET(c)      (0x1154 + (c * 0x80)) /* DMA Channel Current Application Transmit Descriptor Register */
#define FC7300_ENET_DMA_CH_CURRENT_APP_RXBUF_OFFSET(c)      (0x115c + (c * 0x80)) /* DMA Channel Current Application Receive Buffer Register */
#define FC7300_ENET_DMA_CH_STATUS_OFFSET(c)                 (0x1160 + (c * 0x80)) /* DMA Channel Status Register */
#define FC7300_ENET_DMA_CH_MISS_FRAME_CNT_OFFSET(c)         (0x1164 + (c * 0x80)) /* DMA Channel Miss Frame Counter Register */

/* Register bit definitions *************************************************/

/* MAC Configuration Register */

#define ENET_MAC_CONFIGURATION_RE                           (1 << 0)    /* Bit 1: Receiver Enable */
#define ENET_MAC_CONFIGURATION_TE                           (1 << 1)    /* Bit 1: Transmitter Enable */
#define ENET_MAC_CONFIGURATION_PRELEN_SHIFT                 (2)         /* Bits 2-3: Preamble Length for Transmit packets */
#define ENET_MAC_CONFIGURATION_PRELEN_MASK                  (3 << ENET_MAC_CONFIGURATION_PRELEN_SHIFT)
#define ENET_MAC_CONFIGURATION_PRELEN_7BYTES                (0 << ENET_MAC_CONFIGURATION_PRELEN_SHIFT) /* 7 bytes of preamble */
#define ENET_MAC_CONFIGURATION_PRELEN_5BYTES                (1 << ENET_MAC_CONFIGURATION_PRELEN_SHIFT) /* 5 bytes of preamble */
#define ENET_MAC_CONFIGURATION_PRELEN_4BYTES                (2 << ENET_MAC_CONFIGURATION_PRELEN_SHIFT) /* 3 bytes of preamble */
#define ENET_MAC_CONFIGURATION_PRELEN_RESERVED              (3 << ENET_MAC_CONFIGURATION_PRELEN_SHIFT) /* Reserved */
#define ENET_MAC_CONFIGURATION_DC                           (1 << 4)                                   /* Bit 4: Deferral Check */
#define ENET_MAC_CONFIGURATION_BL_SHIFT                     (5)                                        /* Bits 5-6: Back-Off Limit */
#define ENET_MAC_CONFIGURATION_BL_MASK                      (3 << ENET_MAC_CONFIGURATION_BL_SHIFT)
#define ENET_MAC_CONFIGURATION_BL_0                         (0 << ENET_MAC_CONFIGURATION_BL_SHIFT) /* k = min(n,10) */
#define ENET_MAC_CONFIGURATION_BL_1                         (1 << ENET_MAC_CONFIGURATION_BL_SHIFT) /* k = min(n,8) */
#define ENET_MAC_CONFIGURATION_BL_2                         (2 << ENET_MAC_CONFIGURATION_BL_SHIFT) /* k = min(n,4) */
#define ENET_MAC_CONFIGURATION_BL_3                         (3 << ENET_MAC_CONFIGURATION_BL_SHIFT) /* k = min(n,1) */
#define ENET_MAC_CONFIGURATION_DR                           (1 << 8)                               /* Bit 8: Disable Retry */
#define ENET_MAC_CONFIGURATION_DCRS                         (1 << 9)                               /* Bit 9: Disable Carrier Sense During Transmission */
#define ENET_MAC_CONFIGURATION_DO                           (1 << 10)                              /* Bit 10: Disable Receive Own */
#define ENET_MAC_CONFIGURATION_ECRSFD                       (1 << 11)                              /* Bit 11: Enable Carrier Sense In Full-Duplex Mode */
#define ENET_MAC_CONFIGURATION_LM                           (1 << 12)                              /* Bit 12: Loopback Mode */
#define ENET_MAC_CONFIGURATION_DM                           (1 << 13)                              /* Bit 13: Duplex Mode */
#define ENET_MAC_CONFIGURATION_FES                          (1 << 14)                              /* Bit 14: Speed */
#define ENET_MAC_CONFIGURATION_PS                           (1 << 15)                              /* Bit 15: Port Select */
#define ENET_MAC_CONFIGURATION_JE                           (1 << 16)                              /* Bit 16: Jumbo Packet Enable */
#define ENET_MAC_CONFIGURATION_JD                           (1 << 17)                              /* Bit 17: Jabber Disable */
#define ENET_MAC_CONFIGURATION_BE                           (1 << 18)                              /* Bit 18: Packet Burst Enable */
#define ENET_MAC_CONFIGURATION_WD                           (1 << 19)                              /* Bit 19: Watchdog Disable */
#define ENET_MAC_CONFIGURATION_ACS                          (1 << 20)                              /* Bit 20: Automatic Pad Or CRC Stripping */
#define ENET_MAC_CONFIGURATION_CST                          (1 << 21)                              /* Bit 21: CRC Stripping For Type Packets */
#define ENET_MAC_CONFIGURATION_S2KP                         (1 << 22)                              /* Bit 22: IEEE 802.3 Support For 2K Packets */
#define ENET_MAC_CONFIGURATION_GPSLCE                       (1 << 23)                              /* Bit 23: Giant Packet Size Limit Control Enable */
#define ENET_MAC_CONFIGURATION_IPG_SHIFT                    (24)                                   /* Bits 24-26: Inter-Packet Gap */
#define ENET_MAC_CONFIGURATION_IPG_MASK                     (7 << ENET_MAC_CONFIGURATION_IPG_SHIFT)
#define ENET_MAC_CONFIGURATION_IPG_96BIT                    (0 << ENET_MAC_CONFIGURATION_IPG_SHIFT) /* 96 bit times IPG */
#define ENET_MAC_CONFIGURATION_IPG_88BIT                    (1 << ENET_MAC_CONFIGURATION_IPG_SHIFT) /* 88 bit times IPG */
#define ENET_MAC_CONFIGURATION_IPG_80BIT                    (2 << ENET_MAC_CONFIGURATION_IPG_SHIFT) /* 80 bit times IPG */
#define ENET_MAC_CONFIGURATION_IPG_72BIT                    (3 << ENET_MAC_CONFIGURATION_IPG_SHIFT) /* 72 bit times IPG */
#define ENET_MAC_CONFIGURATION_IPG_64BIT                    (4 << ENET_MAC_CONFIGURATION_IPG_SHIFT) /* 64 bit times IPG */
#define ENET_MAC_CONFIGURATION_IPG_56BIT                    (5 << ENET_MAC_CONFIGURATION_IPG_SHIFT) /* 56 bit times IPG */
#define ENET_MAC_CONFIGURATION_IPG_48BIT                    (6 << ENET_MAC_CONFIGURATION_IPG_SHIFT) /* 48 bit times IPG */
#define ENET_MAC_CONFIGURATION_IPG_40BIT                    (7 << ENET_MAC_CONFIGURATION_IPG_SHIFT) /* 40 bit times IPG */
#define ENET_MAC_CONFIGURATION_IPC                          (1 << 27)                               /* Bit 27: Checksum Offload */
#define ENET_MAC_CONFIGURATION_SARC_SHIFT                   (28)                                    /* Bits 28-30: Source Address Insertion Or Replacement Control */
#define ENET_MAC_CONFIGURATION_SARC_MASK                    (7 << ENET_MAC_CONFIGURATION_SARC_SHIFT)
#define ENET_MAC_CONFIGURATION_SARC_INPUT                   (0 << ENET_MAC_CONFIGURATION_SARC_SHIFT) /* Input signals control the SA field generation */
#define ENET_MAC_CONFIGURATION_SARC_ADDR0_INSERT            (2 << ENET_MAC_CONFIGURATION_SARC_SHIFT) /* Contents of MAC Addr-0 inserted in the SA field */
#define ENET_MAC_CONFIGURATION_SARC_ADDR0_REPLACE           (3 << ENET_MAC_CONFIGURATION_SARC_SHIFT) /* Contents of MAC Addr-0 replaces the SA field */
#define ENET_MAC_CONFIGURATION_SARC_ADDR1_INSERT            (6 << ENET_MAC_CONFIGURATION_SARC_SHIFT) /* Contents of MAC Addr-1 inserted in the SA field */
#define ENET_MAC_CONFIGURATION_SARC_ADDR1_REPLACE           (7 << ENET_MAC_CONFIGURATION_SARC_SHIFT) /* Contents of MAC Addr-1 replaces the SA field */
                                                                                                     /* Bit 31: Reserved */

/* MAC Extended Configuration Register */

#define ENET_MAC_EXT_CONFIGURATION_GPSL_SHIFT               (0)         /* Bits 0-13: Giant Packet Size Limit */
#define ENET_MAC_EXT_CONFIGURATION_GPSL_MASK                (0x1fff << ENET_MAC_EXT_CONFIGURATION_GPSL_SHIFT)
#define ENET_MAC_EXT_CONFIGURATION_GPSL(x)                  (((uint32_t)(x) << ENET_MAC_EXT_CONFIGURATION_GPSL_SHIFT) & ENET_MAC_EXT_CONFIGURATION_GPSL_MASK)
                                                                        /* Bits 14-15: Reserved */
#define ENET_MAC_EXT_CONFIGURATION_DCRCC                    (1 << 16)   /* Bit 16: Disable CRC Checking For Received Packets */
#define ENET_MAC_EXT_CONFIGURATION_SPEN                     (1 << 17)   /* Bit 17: Slow Protocol Detection Enable */
#define ENET_MAC_EXT_CONFIGURATION_USP                      (1 << 18)   /* Bit 18: Unicast Slow Protocol Packet Detect */
#define NET_MAC_EXT_CONFIGURATION_PDC                       (1 << 19)   /* Bit 19: Packet Duplication Control */
                                                                        /* Bits 20-23: Reserved */
#define ENET_MAC_EXT_CONFIGURATION_EIPGEN                   (1 << 24)   /* Bit 24: Extended Inter-Packet Gap Enable */
#define ENET_MAC_EXT_CONFIGURATION_EIPG_SHIFT               (25)        /* Bits 25-29: Extended Inter-Packet Gap */
#define ENET_MAC_EXT_CONFIGURATION_EIPG_MASK                (0x1f << ENET_MAC_EXT_CONFIGURATION_EIPG_SHIFT)
#define ENET_MAC_EXT_CONFIGURATION_EIPG(x)                  (((uint32_t)(x) << ENET_MAC_EXT_CONFIGURATION_EIPG_SHIFT) & ENET_MAC_EXT_CONFIGURATION_EIPG_MASK)
                                                                        /* Bits 30-31: Reserved */

/* MAC Packet Filter Register */

#define ENET_MAC_PACKET_FILTER_PR                           (1 << 0)    /* Bit 0: Promiscuous Mode */
#define ENET_MAC_PACKET_FILTER_HUC                          (1 << 1)    /* Bit 1: Hash Unicast */
#define ENET_MAC_PACKET_FILTER_HMC                          (1 << 2)    /* Bit 2: Hash Multicast */
#define ENET_MAC_PACKET_FILTER_DAIF                         (1 << 3)    /* Bit 3: DA Inverse Filtering */
#define ENET_MAC_PACKET_FILTER_PM                           (1 << 4)    /* Bit 4: Pass All Multicast */
#define ENET_MAC_PACKET_FILTER_DBF                          (1 << 5)    /* Bit 5: Disable Broadcast Packets */
#define ENET_MAC_PACKET_FILTER_PCF_SHIFT                    (6)         /* Bits 6-7: Pass Control Packets */
#define ENET_MAC_PACKET_FILTER_PCF_MASK                     (3 << ENET_MAC_PACKET_FILTER_PCF_SHIFT)
#define ENET_MAC_PACKET_FILTER_PCF_FLTR_ALL                 (0 << ENET_MAC_PACKET_FILTER_PCF_SHIFT) /* MAC filters all control packets from reaching the application */
#define ENET_MAC_PACKET_FILTER_PCF_FW_X_PAU                 (1 << ENET_MAC_PACKET_FILTER_PCF_SHIFT) /* MAC forwards all control packets except Pause packets to the application even if they fail the Address filter */
#define ENET_MAC_PACKET_FILTER_PCF_FW_ALL                   (2 << ENET_MAC_PACKET_FILTER_PCF_SHIFT) /* MAC forwards all control packets to the application even if they fail the Address filter */
#define ENET_MAC_PACKET_FILTER_PCF_FW_PASS                  (3 << ENET_MAC_PACKET_FILTER_PCF_SHIFT) /* MAC forwards the control packets that pass the Address filter */
#define ENET_MAC_PACKET_FILTER_SAIF                         (1 << 8)                                /* Bit 8: SA Inverse Filtering */
#define ENET_MAC_PACKET_FILTER_SAF                          (1 << 9)                                /* Bit 9: Source Address Filter Enable */
#define ENET_MAC_PACKET_FILTER_HPF                          (1 << 10)                               /* Bit 10: Hash or Perfect Filter */
                                                                                                    /* Bits 11-15: Reserved */
#define ENET_MAC_PACKET_FILTER_VTFE                         (1 << 16)                               /* Bit 16: VLAN Tag Filter Enable */
                                                                                                    /* Bits 17-19: Reserved */
#define ENET_MAC_PACKET_FILTER_IPFE                         (1 << 20)                               /* Bit 20: Layer 3 and Layer 4 Filter Enable */
#define ENET_MAC_PACKET_FILTER_DNTU                         (1 << 21)                               /* Bit 21: Drop Non-TCP/UDP over IP Packets */
                                                                                                    /* Bits 22-30: Reserved */
#define ENET_MAC_PACKET_FILTER_RA                           (1 << 31)                               /* Bit 31: Receive All */

/* MAC Watchdog Timeout Register */

#define ENET_MAC_WATCHDOG_TIMEOUT_WTO_SHIFT                 (0)         /* Bits 0-3: Watchdog Timeout */
#define ENET_MAC_WATCHDOG_TIMEOUT_WTO_MASK                  (0xf << ENET_MAC_WATCHDOG_TIMEOUT_WTO_SHIFT)
#define ENET_MAC_WATCHDOG_TIMEOUT_WTO(x)                    (((uint32_t)(x) << ENET_MAC_WATCHDOG_TIMEOUT_WTO_SHIFT) & ENET_MAC_WATCHDOG_TIMEOUT_WTO_MASK)
                                                                        /* Bits 4-7: Reserved */
#define ENET_MAC_WATCHDOG_TIMEOUT_PWE                       (1 << 8)    /* Bit 8: Programmable Watchdog Enable */
                                                                        /* Bits 9-31: Reserved */

/* MAC VLAN Tag Control Register */

#define ENET_MAC_VLAN_TAG_CTRL_OB                           (1 << 0)    /* Bit 0: Operation Busy */
#define ENET_MAC_VLAN_TAG_CTRL_CT                           (1 << 1)    /* Bit 1: Command Type, 1：read operation, 0: write operation */
#define ENET_MAC_VLAN_TAG_CTRL_OFS_SHIFT                    (2)         /* Bits 2-3: The address offset of the MAC VLAN Tag Filter register */
#define ENET_MAC_VLAN_TAG_CTRL_OFS_MASK                     (3 << ENET_MAC_VLAN_TAG_CTRL_OFS_SHIFT)
#define ENET_MAC_VLAN_TAG_CTRL_OFS(x)                       (((uint32_t)(x) << ENET_MAC_VLAN_TAG_CTRL_OFS_SHIFT) & ENET_MAC_VLAN_TAG_CTRL_OFS_MASK)
                                                                        /* Bits 4-15: Reserved */
#define ENET_MAC_VLAN_TAG_CTRL_ETV                          (1 << 16)   /* Bit 16: Enable Tag For VLAN */
#define ENET_MAC_VLAN_TAG_CTRL_VTIM                         (1 << 17)   /* Bit 17: VLAN Tag Inverse Match Enable */
#define ENET_MAC_VLAN_TAG_CTRL_ESVL                         (1 << 18)   /* Bit 18: Enable S-VLAN */
#define ENET_MAC_VLAN_TAG_CTRL_ERSVLM                       (1 << 19)   /* Bit 19: Enable Receive S-VLAN Match */
#define ENET_MAC_VLAN_TAG_CTRL_DOVLTC                       (1 << 20)   /* Bit 20: Disable VLAN Type Check */
#define ENET_MAC_VLAN_TAG_CTRL_EVLS_SHIFT                   (21)        /* Bits 21-22: Enable VLAN Tag Stripping  */
#define ENET_MAC_VLAN_TAG_CTRL_EVLS_MASK                    (3 << ENET_MAC_VLAN_TAG_CTRL_EVLS_SHIFT)
#define ENET_MAC_VLAN_TAG_CTRL_EVLS_STRIP_NONE              (0 << ENET_MAC_VLAN_TAG_CTRL_EVLS_SHIFT) /* Do not strip */
#define ENET_MAC_VLAN_TAG_CTRL_EVLS_STRIP_FLTR_PASS         (1 << ENET_MAC_VLAN_TAG_CTRL_EVLS_SHIFT) /* Strip if VLAN filter passes */
#define ENET_MAC_VLAN_TAG_CTRL_EVLS_STRIP_FLTR_FAIL         (2 << ENET_MAC_VLAN_TAG_CTRL_EVLS_SHIFT) /* Strip if VLAN filter fails */
#define ENET_MAC_VLAN_TAG_CTRL_EVLS_STRIP_ALWAYS            (3 << ENET_MAC_VLAN_TAG_CTRL_EVLS_SHIFT) /* Always strip */
                                                                                                     /* Bit 23: Reserved */
#define ENET_MAC_VLAN_TAG_CTRL_EVLRXS                       (1 << 24)                                /* Bit 24: Enable VLAN Tag In Receive Status */
#define ENET_MAC_VLAN_TAG_CTRL_VTHM                         (1 << 25)                                /* Bit 25: VLAN Tag Hash Table Match */
#define ENET_MAC_VLAN_TAG_CTRL_EDVLP                        (1 << 26)                                /* Bit 26: Enable Double VLAN Processing */
#define ENET_MAC_VLAN_TAG_CTRL_ERIVLT                       (1 << 27)                                /* Bit 27: Enable Inner VLAN Tag Comparison Enables or disables the inner VLAN tag */
#define ENET_MAC_VLAN_TAG_CTRL_EIVLS_SHIFT                  (28)                                     /* Bits 28-29: Enable VLAN Tag Stripping  */
#define ENET_MAC_VLAN_TAG_CTRL_EIVLS_MASK                   (3 << ENET_MAC_VLAN_TAG_CTRL_EIVLS_SHIFT)
#define ENET_MAC_VLAN_TAG_CTRL_EIVLS_STRIP_NONE             (0 << ENET_MAC_VLAN_TAG_CTRL_EIVLS_SHIFT) /* Do not strip */
#define ENET_MAC_VLAN_TAG_CTRL_EIVLS_STRIP_FLTR_PASS        (1 << ENET_MAC_VLAN_TAG_CTRL_EIVLS_SHIFT) /* Strip if VLAN filter passes */
#define ENET_MAC_VLAN_TAG_CTRL_EIVLS_STRIP_FLTR_FAIL        (2 << ENET_MAC_VLAN_TAG_CTRL_EIVLS_SHIFT) /* Strip if VLAN filter fails */
#define ENET_MAC_VLAN_TAG_CTRL_EIVLS_STRIP_ALWAYS           (3 << ENET_MAC_VLAN_TAG_CTRL_EIVLS_SHIFT) /* Always strip */
                                                                                                      /* Bit 30: Reserved */
#define ENET_MAC_VLAN_TAG_CTRL_EIVLRXS                      (1 << 31)                                 /* Bit 31: Enable Inner VLAN Tag In Receive Status */

/* MAC VLAN Tag Data Register */

#define ENET_MAC_VLAN_TAG_DATA_VID_SHIFT                    (0)         /* Bits 0-15: VLAN Tag ID */
#define ENET_MAC_VLAN_TAG_DATA_VID_MASK                     (0xffff << ENET_MAC_VLAN_TAG_DATA_VID_SHIFT)
#define ENET_MAC_VLAN_TAG_DATA_VID(x)                       (((uint32_t)(x) << ENET_MAC_VLAN_TAG_DATA_VID_SHIFT) & ENET_MAC_VLAN_TAG_DATA_VID_MASK)
#define ENET_MAC_VLAN_TAG_DATA_VEN                          (1 << 16)   /* Bit 16: VLAN Tag Enable */
#define ENET_MAC_VLAN_TAG_DATA_ETV                          (1 << 17)   /* Bit 17: 12-bit or 16-bit VLAN comparison */
#define ENET_MAC_VLAN_TAG_DATA_DOVLTC                       (1 << 18)   /* Bit 18: Disable VLAN Type Comparison */
#define ENET_MAC_VLAN_TAG_DATA_ERSVLM                       (1 << 19)   /* Bit 19: Enable S-VLAN Match */
#define ENET_MAC_VLAN_TAG_DATA_ERIVLT                       (1 << 20)   /* Bit 20: Enable Inner VLAN Tag */
                                                                        /* Bits 21-23: Reserved */
#define ENET_MAC_VLAN_TAG_DATA_DMACHEN                      (1 << 24)   /* Bit 24: DMA Channel Number Enable */
#define ENET_MAC_VLAN_TAG_DATA_DMACHN                       (1 << 25)   /* Bit 25: DMA Channel Number */
                                                                        /* Bits 26-31: Reserved */

/* MAC VLAN Hash Table Register */

#define ENET_MAC_VLAN_HASH_TABLE_VLHT_SHIFT                 (0)         /* Bits 0-15: VLAN Hash Table */
#define ENET_MAC_VLAN_HASH_TABLE_VLHT_MASK                  (0xffff << ENET_MAC_VLAN_HASH_TABLE_VLHT_SHIFT)
#define ENET_MAC_VLAN_HASH_TABLE_VLHT(x)                    (((uint32_t)(x) << ENET_MAC_VLAN_HASH_TABLE_VLHT_SHIFT) & ENET_MAC_VLAN_HASH_TABLE_VLHT_MASK)
                                                                        /* Bits 16-31: Reserved */

/* MAC VLAN Inclusion Or Replacement Register */

#define ENET_MAC_VLAN_INCL_VLT_SHIFT                        (0)         /* Bits 0-15: VLAN Tag For Transmit Packets */
#define ENET_MAC_VLAN_INCL_VLT_MASK                         (0xffff << ENET_MAC_VLAN_INCL_VLT_SHIFT)
#define ENET_MAC_VLAN_INCL_VLT(x)                           (((uint32_t)(x) << ENET_MAC_VLAN_INCL_VLT_SHIFT) & ENET_MAC_VLAN_INCL_VLT_MASK)
#define ENET_MAC_VLAN_INCL_VLC_SHIFT                        (16)        /* Bits 16-17: VLAN Tag Control */
#define ENET_MAC_VLAN_INCL_VLC_MASK                         (3 << ENET_MAC_VLAN_INCL_VLC_SHIFT)
#define ENET_MAC_VLAN_INCL_VLC_NO_OPERATION                 (0 << ENET_MAC_VLAN_INCL_VLC_SHIFT) /* No VLAN tag deletion, insertion, or replacement */
#define ENET_MAC_VLAN_INCL_VLC_DELETION                     (1 << ENET_MAC_VLAN_INCL_VLC_SHIFT) /* VLAN tag deletion */
#define ENET_MAC_VLAN_INCL_VLC_INSERTION                    (2 << ENET_MAC_VLAN_INCL_VLC_SHIFT) /* VLAN tag insertion */
#define ENET_MAC_VLAN_INCL_VLC_REPLACEMENT                  (3 << ENET_MAC_VLAN_INCL_VLC_SHIFT) /* VLAN tag replacement */
#define ENET_MAC_VLAN_INCL_VLP                              (1 << 18)                           /* Bit 18: VLAN Priority Control */
#define ENET_MAC_VLAN_INCL_CSVL                             (1 << 19)                           /* Bit 19: C-VLAN Or S-VLAN */
#define ENET_MAC_VLAN_INCL_VLTI                             (1 << 20)                           /* Bit 20: VLAN Tag Input */
#define ENET_MAC_VLAN_INCL_CBTI                             (1 << 21)                           /* Bit 21: Channel-Based Tag Insertion */
                                                                                                /* Bits 22-23: Reserved */
#define ENET_MAC_VLAN_INCL_ADDR                             (1 << 24)                           /* Bit 24: The queue- or channel-specific VLAN inclusion registers for read or write access */
                                                                                                /* Bits 25-29: Reserved */
#define ENET_MAC_VLAN_INCL_RDWR                             (1 << 30)                           /* Bit 30: Read Write Control, 0：read operation, 1: write operation */
#define ENET_MAC_VLAN_INCL_BUSY                             (1 << 31)                           /* Bit 31: Busy */

/* Inner VLAN Tag Inclusion Or Replacement Register */

#define ENET_MAC_INNER_VLAN_INCL_VLT_SHIFT                  (0)         /* Bits 0-15: VLAN Tag For Transmit Packets */
#define ENET_MAC_INNER_VLAN_INCL_VLT_MASK                   (0xffff << ENET_MAC_INNER_VLAN_INCL_VLT_SHIFT)
#define ENET_MAC_INNER_VLAN_INCL_VLT(x)                     (((uint32_t)(x) << ENET_MAC_INNER_VLAN_INCL_VLT_SHIFT) & ENET_MAC_INNER_VLAN_INCL_VLT_MASK)
#define ENET_MAC_INNER_VLAN_INCL_VLC_SHIFT                  (16)        /* Bits 16-17: VLAN Tag Control */
#define ENET_MAC_INNER_VLAN_INCL_VLC_MASK                   (3 << ENET_MAC_INNER_VLAN_INCL_VLC_SHIFT)
#define ENET_MAC_INNER_VLAN_INCL_VLC_NO_OPERATION           (0 << ENET_MAC_INNER_VLAN_INCL_VLC_SHIFT) /* No VLAN tag deletion, insertion, or replacement */
#define ENET_MAC_INNER_VLAN_INCL_VLC_DELETION               (1 << ENET_MAC_INNER_VLAN_INCL_VLC_SHIFT) /* VLAN tag deletion */
#define ENET_MAC_INNER_VLAN_INCL_VLC_INSERTION              (2 << ENET_MAC_INNER_VLAN_INCL_VLC_SHIFT) /* VLAN tag insertion */
#define ENET_MAC_INNER_VLAN_INCL_VLC_REPLACEMENT            (3 << ENET_MAC_INNER_VLAN_INCL_VLC_SHIFT) /* VLAN tag replacement */
#define ENET_MAC_INNER_VLAN_INCL_VLP                        (1 << 18)                                 /* Bit 18: VLAN Priority Control */
#define ENET_MAC_INNER_VLAN_INCL_CSVL                       (1 << 19)                                 /* Bit 19: C-VLAN Or S-VLAN */
#define ENET_MAC_INNER_VLAN_INCL_VLTI                       (1 << 20)                                 /* Bit 20: VLAN Tag Input */
                                                                                                      /* Bits 21-31: Reserved */

/* MAC Q0 Tx Flow Control Register */

#define ENET_MAC_Q0_TX_FLOW_CTRL_FCB_BPA                    (1 << 0)    /* Bit 0: Flow Control Busy or Backpressure Activate */
#define ENET_MAC_Q0_TX_FLOW_CTRL_TFE                        (1 << 1)    /* Bit 1: Transmit Flow Control Enable */
                                                                        /* Bits 2-3: Reserved */
#define ENET_MAC_Q0_TX_FLOW_CTRL_PLT_SHIFT                  (4)         /* Bits 4-6: Pause Low Threshold */
#define ENET_MAC_Q0_TX_FLOW_CTRL_PLT_MASK                   (7 << ENET_MAC_Q0_TX_FLOW_CTRL_PLT_SHIFT)
#define ENET_MAC_Q0_TX_FLOW_CTRL_PLT_PT4                    (0 << ENET_MAC_Q0_TX_FLOW_CTRL_PLT_SHIFT) /* Pause Time minus 4 Slot Times */
#define ENET_MAC_Q0_TX_FLOW_CTRL_PLT_PT28                   (1 << ENET_MAC_Q0_TX_FLOW_CTRL_PLT_SHIFT) /* Pause Time minus 28 Slot Times */
#define ENET_MAC_Q0_TX_FLOW_CTRL_PLT_PT36                   (2 << ENET_MAC_Q0_TX_FLOW_CTRL_PLT_SHIFT) /* Pause Time minus 36 Slot Times */
#define ENET_MAC_Q0_TX_FLOW_CTRL_PLT_PT144                  (3 << ENET_MAC_Q0_TX_FLOW_CTRL_PLT_SHIFT) /* Pause Time minus 144 Slot Times */
#define ENET_MAC_Q0_TX_FLOW_CTRL_PLT_PT256                  (4 << ENET_MAC_Q0_TX_FLOW_CTRL_PLT_SHIFT) /* Pause Time minus 256 Slot Times */
#define ENET_MAC_Q0_TX_FLOW_CTRL_PLT_PT512                  (5 << ENET_MAC_Q0_TX_FLOW_CTRL_PLT_SHIFT) /* Pause Time minus 512 Slot Times */
#define ENET_MAC_Q0_TX_FLOW_CTRL_PLT_RESERVED               (6 << ENET_MAC_Q0_TX_FLOW_CTRL_PLT_SHIFT) /* Reserved */
#define ENET_MAC_Q0_TX_FLOW_CTRL_DZPQ                       (1 << 7)                                  /* Bit 7: Disable Zero-Quanta Pause */
                                                                                                      /* Bits 8-15: Reserved */
#define ENET_MAC_Q0_TX_FLOW_CTRL_PT_SHIFT                   (16)                                      /* Bits 16-31: VLAN Tag For Transmit Packets */
#define ENET_MAC_Q0_TX_FLOW_CTRL_PT_MASK                    (0xffff << ENET_MAC_Q0_TX_FLOW_CTRL_PT_SHIFT)
#define ENET_MAC_Q0_TX_FLOW_CTRL_PT(x)                      (((uint32_t)(x) << ENET_MAC_Q0_TX_FLOW_CTRL_PT_SHIFT) & ENET_MAC_Q0_TX_FLOW_CTRL_PT_MASK)

/* MAC Receive Flow Control Register */

#define ENET_MAC_RX_FLOW_CTRL_RFE                           (1 << 0)    /* Bit 0: Receive Flow Control Enable */
#define ENET_MAC_RX_FLOW_CTRL_UP                            (1 << 1)    /* Bit 1: Unicast Pause Packet Detect */
                                                                        /* Bits 2-31: Reserved */

/* MAC RxQ Control 4 Register */

#define ENET_MAC_RXQ_CTRL4_UFFQE                            (1 << 0)    /* Bit 0: Unicast Address Filter Fail Packets Queuing Enable */
#define ENET_MAC_RXQ_CTRL4_UFFQ                             (1 << 1)    /* Bit 1: Unicast Address Filter Fail Packets Queue */
                                                                        /* Bits 2-7: Reserved */
#define ENET_MAC_RXQ_CTRL4_MFFQE                            (1 << 8)    /* Bit 8: Multicast Address Filter Fail Packets Queuing Enable */
#define ENET_MAC_RXQ_CTRL4_MFFQ                             (1 << 9)    /* Bit 9: Multicast Address Filter Fail Packets Queue */
                                                                        /* Bits 10-15: Reserved */
#define ENET_MAC_RXQ_CTRL4_VFFQE                            (1 << 16)   /* Bit 16: VLAN Tag Filter Fail Packets Queuing Enable */
#define ENET_MAC_RXQ_CTRL4_VFFQ                             (1 << 17)   /* Bit 17: VLAN Tag Filter Fail Packets Queue */
                                                                        /* Bits 18-31: Reserved */

/* MAC RxQ Control 0 Register */

#define ENET_MAC_RXQ_CTRL0_RXQEN_SHIFT(q)                   (q * 2)     /* Bits 0+2*q-1+2*q: Receive Queue q Enable */
#define ENET_MAC_RXQ_CTRL0_RXQEN_MASK(q)                    (3 << ENET_MAC_RXQ_CTRL0_RXQEN_SHIFT(q))
#define ENET_MAC_RXQ_CTRL0_RXQEN_DISABLE(q)                 (0 << ENET_MAC_RXQ_CTRL0_RXQEN_SHIFT(q)) /* Queue not enabled */
#define ENET_MAC_RXQ_CTRL0_RXQEN_EN_AV(q)                   (1 << ENET_MAC_RXQ_CTRL0_RXQEN_SHIFT(q)) /* Queue enabled for AV */
#define ENET_MAC_RXQ_CTRL0_RXQEN_EN_GENERIC(q)              (2 << ENET_MAC_RXQ_CTRL0_RXQEN_SHIFT(q)) /* Queue enabled for DCB/Generic */
#define ENET_MAC_RXQ_CTRL0_RXQEN_RESERVED(q)                (3 << ENET_MAC_RXQ_CTRL0_RXQEN_SHIFT(q)) /* Reserved */
                                                                                                     /* Bits 4-31: Reserved */

/* MAC RxQ Control 1 Register */

#define ENET_MAC_RXQ_CTRL1_AVCPQ_SHIFT                      (0)         /* Bits 0-2: AV Untagged Control Packets Queue */
#define ENET_MAC_RXQ_CTRL1_AVCPQ_MASK                       (7 << ENET_MAC_RXQ_CTRL1_AVCPQ_SHIFT)
#define ENET_MAC_RXQ_CTRL1_AVCPQ_QUEUE0                     (0 << ENET_MAC_RXQ_CTRL1_AVCPQ_SHIFT) /* Receive Queue 0 */
#define ENET_MAC_RXQ_CTRL1_AVCPQ_QUEUE1                     (1 << ENET_MAC_RXQ_CTRL1_AVCPQ_SHIFT) /* Receive Queue 1 */
                                                                                                  /* Bit 3: Reserved */
#define ENET_MAC_RXQ_CTRL1_PTPQ_SHIFT                       (4)                                   /* Bits 4-6: PTP Packets Queue */
#define ENET_MAC_RXQ_CTRL1_PTPQ_MASK                        (7 << ENET_MAC_RXQ_CTRL1_PTPQ_SHIFT)
#define ENET_MAC_RXQ_CTRL1_PTPQ_QUEUE0                      (0 << ENET_MAC_RXQ_CTRL1_PTPQ_SHIFT) /* Receive Queue 0 */
#define ENET_MAC_RXQ_CTRL1_PTPQ_QUEUE1                      (1 << ENET_MAC_RXQ_CTRL1_PTPQ_SHIFT) /* Receive Queue 1 */
                                                                                                 /* Bits 7-11: Reserved */
#define ENET_MAC_RXQ_CTRL1_UPQ_SHIFT                        (12)                                 /* Bits 12-14: Untagged Packet Queue */
#define ENET_MAC_RXQ_CTRL1_UPQ_MASK                         (7 << ENET_MAC_RXQ_CTRL1_UPQ_SHIFT)
#define ENET_MAC_RXQ_CTRL1_UPQ_QUEUE0                       (0 << ENET_MAC_RXQ_CTRL1_UPQ_SHIFT) /* Receive Queue 0 */
#define ENET_MAC_RXQ_CTRL1_UPQ_QUEUE1                       (1 << ENET_MAC_RXQ_CTRL1_UPQ_SHIFT) /* Receive Queue 1 */
                                                                                                /* Bit 15: Reserved */
#define ENET_MAC_RXQ_CTRL1_MCBCQ_SHIFT                      (16)                                /* Bits 16-18: Multicast and Broadcast Queue */
#define ENET_MAC_RXQ_CTRL1_MCBCQ_MASK                       (7 << ENET_MAC_RXQ_CTRL1_MCBCQ_SHIFT)
#define ENET_MAC_RXQ_CTRL1_MCBCQ_QUEUE0                     (0 << ENET_MAC_RXQ_CTRL1_MCBCQ_SHIFT) /* Receive Queue 0 */
#define ENET_MAC_RXQ_CTRL1_MCBCQ_QUEUE1                     (1 << ENET_MAC_RXQ_CTRL1_MCBCQ_SHIFT) /* Receive Queue 1 */
                                                                                                  /* Bit 19: Reserved */
#define ENET_MAC_RXQ_CTRL1_MCBCQEN                          (1 << 20)                             /* Bit 20: Multicast and Broadcast Queue Enable */
#define ENET_MAC_RXQ_CTRL1_TACPQE                           (1 << 21)                             /* Bit 21: Tagged AV Control Packets Queuing Enable */
#define ENET_MAC_RXQ_CTRL1_TPQC_SHIFT                       (22)                                  /* Bits 22-23: Pause Low Threshold */
#define ENET_MAC_RXQ_CTRL1_TPQC_MASK                        (3 << ENET_MAC_RXQ_CTRL1_TPQC_SHIFT)
#define ENET_MAC_RXQ_CTRL1_TPQC_PSRQ_NONAV                  (0 << ENET_MAC_RXQ_CTRL1_TPQC_SHIFT) /* VLAN Tagged PTPoE packets are routed based on PSRQ for only non-AV enabled Rx Queues */
#define ENET_MAC_RXQ_CTRL1_TPQC_PTPQ                        (1 << ENET_MAC_RXQ_CTRL1_TPQC_SHIFT) /* VLAN Tagged PTPoE packets are routed to Rx Queue specified by PTPQ field */
#define ENET_MAC_RXQ_CTRL1_TPQC_PSRQ_AV                     (2 << ENET_MAC_RXQ_CTRL1_TPQC_SHIFT) /* VLAN Tagged PTPoE packets are routed to only AV enabled Rx Queues based on PSRQ */
#define ENET_MAC_RXQ_CTRL1_TPQC_RESERVED                    (3 << ENET_MAC_RXQ_CTRL1_TPQC_SHIFT) /* Reserved */
#define ENET_MAC_RXQ_CTRL1_FPRQ_SHIFT                       (24)                                 /* Bits 24-26: Frame Preemption Residue Queue */
#define ENET_MAC_RXQ_CTRL1_FPRQ_MASK                        (7 << ENET_MAC_RXQ_CTRL1_FPRQ_SHIFT)
#define ENET_MAC_RXQ_CTRL1_FPRQ_QUEUE0                      (0 << ENET_MAC_RXQ_CTRL1_FPRQ_SHIFT) /* Receive Queue 0 */
#define ENET_MAC_RXQ_CTRL1_FPRQ_QUEUE1                      (1 << ENET_MAC_RXQ_CTRL1_FPRQ_SHIFT) /* Receive Queue 1 */
                                                                                                 /* Bit 27: Reserved */
#define ENET_MAC_RXQ_CTRL1_OMCBCQ                           (1 << 28)                            /* Bit 28: Over-riding MC-BC queue priority select */
#define ENET_MAC_RXQ_CTRL1_TBRQE                            (1 << 29)                            /* Bit 29: Type Field Based Rx Queuing Enable */
                                                                                                 /* Bits 30-31: Reserved */

/* MAC RxQ Control 2 Register */

#define ENET_MAC_RXQ_CTRL2_PSRQ_SHIFT(q)                    (q * 8)     /* Bits 0+8*q-7+8*q: Priorities Selected in the Receive Queue */
#define ENET_MAC_RXQ_CTRL2_PSRQ_MASK(q)                     (0xff << ENET_MAC_RXQ_CTRL2_PSRQ_SHIFT(q))
#define ENET_MAC_RXQ_CTRL2_PSRQ(q,x)                        (((uint32_t)(x) << ENET_MAC_RXQ_CTRL2_PSRQ_SHIFT(q)) & ENET_MAC_RXQ_CTRL2_PSRQ_MASK(q))
                                                                        /* Bits 16-31: Reserved */

/* MAC Interrupt Status Register */

#define ENET_MAC_INTERRUPT_STATUS_PHYIS                     (1 << 3)    /* Bit 3: PHY Interrupt Status */
                                                                        /* Bits 4-7: Reserved */
#define ENET_MAC_INTERRUPT_STATUS_MMCIS                     (1 << 8)    /* Bit 8: MMC Interrupt Status */
#define ENET_MAC_INTERRUPT_STATUS_MMCRXIS                   (1 << 9)    /* Bit 9: MMC Receive Interrupt Status */
#define ENET_MAC_INTERRUPT_STATUS_MMCTXIS                   (1 << 10)   /* Bit 10: MMC Transmit Interrupt Status */
                                                                        /* Bit 11: Reserved */
#define ENET_MAC_INTERRUPT_STATUS_TSIS                      (1 << 12)   /* Bit 12: Timestamp Interrupt Status */
#define ENET_MAC_INTERRUPT_STATUS_TXSTSIS                   (1 << 13)   /* Bit 13: Transmit Interrupt Status */
#define ENET_MAC_INTERRUPT_STATUS_RXSTSIS                   (1 << 14)   /* Bit 14: Receive Interrupt Status */
                                                                        /* Bits 15-16: Reserved */
#define ENET_MAC_INTERRUPT_STATUS_FPEIS                     (1 << 17)   /* Bit 17: Frame Preemption Interrupt Status */
#define ENET_MAC_INTERRUPT_STATUS_MDIOIS                    (1 << 18)   /* Bit 18: MDIO Interrupt Status */
#define ENET_MAC_INTERRUPT_STATUS_MFTIS                     (1 << 19)   /* Bit 19: MMC FPE Transmit Interrupt Status */
#define ENET_MAC_INTERRUPT_STATUS_MFRIS                     (1 << 20)   /* Bit 20: MMC FPE Receive Interrupt Status */
                                                                        /* Bits 21-31: Reserved */

/* MAC Interrupt Enable Register */

#define ENET_MAC_INTERRUPT_ENABLE_PHYIE                     (1 << 3)    /* Bit 3: PHY Interrupt Enable */
                                                                        /* Bits 4-11: Reserved */
#define ENET_MAC_INTERRUPT_ENABLE_TSIE                      (1 << 12)   /* Bit 12: Timestamp Interrupt Enable */
#define ENET_MAC_INTERRUPT_ENABLE_TXSTSIE                   (1 << 13)   /* Bit 13: Transmit Interrupt Enable */
#define ENET_MAC_INTERRUPT_ENABLE_RXSTSIE                   (1 << 14)   /* Bit 14: Receive Interrupt Enable */
                                                                        /* Bits 15-16: Reserved */
#define ENET_MAC_INTERRUPT_ENABLE_FPEIE                     (1 << 17)   /* Bit 17: Frame Preemption Interrupt Enable */
#define ENET_MAC_INTERRUPT_ENABLE_MDIOIE                    (1 << 18)   /* Bit 18: MDIO Interrupt Enable */
                                                                        /* Bits 19-31: Reserved */

/* MAC DPP FSM Interrupt Status Register */

#define ENET_MAC_DPP_FSM_INT_STATUS_RDPES                   (1 << 2)    /* Bit 2: Read Descriptor Parity checker Error Status */
#define ENET_MAC_DPP_FSM_INT_STATUS_MPES                    (1 << 3)    /* Bit 3: MTL data path Parity checker Error Status */
#define ENET_MAC_DPP_FSM_INT_STATUS_MTSPES                  (1 << 4)    /* Bit 4: MTL TX Status data path Parity checker Error Status */
#define ENET_MAC_DPP_FSM_INT_STATUS_ARPES                   (1 << 5)    /* Bit 5: Application Receive interface data path Parity Error Status */
                                                                        /* Bits 6-7: Reserved */
#define ENET_MAC_DPP_FSM_INT_STATUS_TTES                    (1 << 8)    /* Bit 8: Tx FSM Timeout Error Status */
#define ENET_MAC_DPP_FSM_INT_STATUS_RTES                    (1 << 9)    /* Bit 9: Rx FSM Timeout Error Status */
                                                                        /* Bit 10: Reserved */
#define ENET_MAC_DPP_FSM_INT_STATUS_ATES                    (1 << 11)   /* Bit 11: APP FSM Timeout Error Status */
#define ENET_MAC_DPP_FSM_INT_STATUS_PTES                    (1 << 12)   /* Bit 12: PTP FSM Timeout Error Status */
                                                                        /* Bits 13-15: Reserved */
#define ENET_MAC_DPP_FSM_INT_STATUS_MSTTES                  (1 << 16)   /* Bit 16: Master Read/Write Timeout Error Status */
                                                                        /* Bits 17-23: Reserved */
#define ENET_MAC_DPP_FSM_INT_STATUS_FSMPES                  (1 << 24)   /* Bit 24: FSM State Parity Error Status */
#define ENET_MAC_DPP_FSM_INT_STATUS_MTBUPES                 (1 << 25)   /* Bit 25: MAC TBU data path Parity checker Error Status */
#define ENET_MAC_DPP_FSM_INT_STATUS_MTFCPES                 (1 << 26)   /* Bit 26: MAC TFC data path Parity checker Error Status */
#define ENET_MAC_DPP_FSM_INT_STATUS_MRWCPES                 (1 << 27)   /* Bit 27: MTL RWC data path Parity checker Error Status */
                                                                        /* Bits 28-31: Reserved */

/* MAC FSM Control Register */

#define ENET_MAC_FSM_CONTROL_TMOUTEN                        (1 << 0)    /* Bit 0: FSM Timeout Feature Enable */
#define ENET_MAC_FSM_CONTROL_PRTYEN                         (1 << 1)    /* Bit 1: FSM Parity Feature Enable */
                                                                        /* Bits 2-7: Reserved */
#define ENET_MAC_FSM_CONTROL_TTEIN                          (1 << 8)    /* Bit 8: Tx FSM Timeout Error Injection Enable */
#define ENET_MAC_FSM_CONTROL_RTEIN                          (1 << 9)    /* Bit 9: Rx FSM Timeout Error Injection Enable */
                                                                        /* Bit 10: Reserved */
#define ENET_MAC_FSM_CONTROL_ATEIN                          (1 << 11)   /* Bit 11: APP FSM Timeout Error Injection Enable */
#define ENET_MAC_FSM_CONTROL_PTEIN                          (1 << 12)   /* Bit 12: PTP FSM Timeout Error Injection Enable */
                                                                        /* Bits 13-15: Reserved */
#define ENET_MAC_FSM_CONTROL_TPEIN                          (1 << 16)   /* Bit 16: Tx FSM Parity Error Injection Enable */
#define ENET_MAC_FSM_CONTROL_RPEIN                          (1 << 17)   /* Bit 17: Rx FSM Parity Error Injection Enable */
                                                                        /* Bit 18: Reserved */
#define ENET_MAC_FSM_CONTROL_APEIN                          (1 << 19)   /* Bit 19: APP FSM Parity Error Injection Enable */
#define ENET_MAC_FSM_CONTROL_PPEIN                          (1 << 20)   /* Bit 20: PTP FSM Parity Error Injection Enable */
                                                                        /* Bits 21-23: Reserved */
#define ENET_MAC_FSM_CONTROL_TLGRNML                        (1 << 24)   /* Bit 24: Tx Large/Normal Mode Select */
#define ENET_MAC_FSM_CONTROL_RLGRNML                        (1 << 25)   /* Bit 25: Rx Large/Normal Mode Select */
                                                                        /* Bit 26: Reserved */
#define ENET_MAC_FSM_CONTROL_ALGRNML                        (1 << 27)   /* Bit 27: APP Large/Normal Mode Select */
#define ENET_MAC_FSM_CONTROL_PLGRNML                        (1 << 28)   /* Bit 28: PTP Large/Normal Mode Select */
                                                                        /* Bits 29-31: Reserved */

/* MAC FSM ACT Timer Register */

#define ENET_MAC_FSM_ACT_TIMER_TMR_SHIFT                    (0)         /* Bits 0-9: CSR Clocks for 1us Tick */
#define ENET_MAC_FSM_ACT_TIMER_TMR_MASK                     (0x3ff << ENET_MAC_FSM_ACT_TIMER_TMR_SHIFT)
#define ENET_MAC_FSM_ACT_TIMER_TMR(x)                       (((uint32_t)(x) << ENET_MAC_FSM_ACT_TIMER_TMR_SHIFT) & ENET_MAC_FSM_ACT_TIMER_TMR_MASK)
                                                                        /* Bits 10-15: Reserved */
#define ENET_MAC_FSM_ACT_TIMER_NTMRMD_SHIFT                 (16)        /* Bits 16-19: Normal Mode Timeout Value */
#define ENET_MAC_FSM_ACT_TIMER_NTMRMD_MASK                  (0x3ff << ENET_MAC_FSM_ACT_TIMER_NTMRMD_SHIFT)
#define ENET_MAC_FSM_ACT_TIMER_NTMRMD_DISABLE               (0x0 << ENET_MAC_FSM_ACT_TIMER_NTMRMD_SHIFT) /* Timer disabled */
#define ENET_MAC_FSM_ACT_TIMER_NTMRMD_M_1MICRO_SEC          (0x1 << ENET_MAC_FSM_ACT_TIMER_NTMRMD_SHIFT) /* 1us */
#define ENET_MAC_FSM_ACT_TIMER_NTMRMD_M_4MILLI_SEC          (0x2 << ENET_MAC_FSM_ACT_TIMER_NTMRMD_SHIFT) /* 4ms */
#define ENET_MAC_FSM_ACT_TIMER_NTMRMD_M_16MILLI_SEC         (0x3 << ENET_MAC_FSM_ACT_TIMER_NTMRMD_SHIFT) /* 16ms */
#define ENET_MAC_FSM_ACT_TIMER_NTMRMD_M_64MILLI_SEC         (0x4 << ENET_MAC_FSM_ACT_TIMER_NTMRMD_SHIFT) /* 64ms */
#define ENET_MAC_FSM_ACT_TIMER_NTMRMD_M_256MILLI_SEC        (0x5 << ENET_MAC_FSM_ACT_TIMER_NTMRMD_SHIFT) /* 256ms */
#define ENET_MAC_FSM_ACT_TIMER_NTMRMD_M_1SEC                (0x6 << ENET_MAC_FSM_ACT_TIMER_NTMRMD_SHIFT) /* 1sec */
#define ENET_MAC_FSM_ACT_TIMER_NTMRMD_M_4SEC                (0x7 << ENET_MAC_FSM_ACT_TIMER_NTMRMD_SHIFT) /* 4sec */
#define ENET_MAC_FSM_ACT_TIMER_NTMRMD_M_16SEC               (0x8 << ENET_MAC_FSM_ACT_TIMER_NTMRMD_SHIFT) /* 16sec */
#define ENET_MAC_FSM_ACT_TIMER_NTMRMD_M_32SEC               (0x9 << ENET_MAC_FSM_ACT_TIMER_NTMRMD_SHIFT) /* 32sec */
#define ENET_MAC_FSM_ACT_TIMER_NTMRMD_M_64SEC               (0xa << ENET_MAC_FSM_ACT_TIMER_NTMRMD_SHIFT) /* 64sec */
#define ENET_MAC_FSM_ACT_TIMER_NTMRMD_RSVD                  (0xb << ENET_MAC_FSM_ACT_TIMER_NTMRMD_SHIFT) /* Reserved */
                                                                                                         /* Bits 20-31: Reserved */

/* MAC MDIO Address Register */

#define ENET_MAC_MDIO_ADDRESS_GB                            (1 << 0)    /* Bit 0: GMII Busy */
#define ENET_MAC_MDIO_ADDRESS_C45E                          (1 << 1)    /* Bit 1: Clause 45 PHY Enable */
#define ENET_MAC_MDIO_ADDRESS_OP_SHIFT                      (2)         /* Bits 2-3: GMII Operation Command */
#define ENET_MAC_MDIO_ADDRESS_OP_MASK                       (0x3 << ENET_MAC_MDIO_ADDRESS_OP_SHIFT)
#define ENET_MAC_MDIO_ADDRESS_OP_RESERVED                   (0x0 << ENET_MAC_MDIO_ADDRESS_OP_SHIFT) /* Reserved */
#define ENET_MAC_MDIO_ADDRESS_OP_WRITE                      (0x1 << ENET_MAC_MDIO_ADDRESS_OP_SHIFT) /* Write */
#define ENET_MAC_MDIO_ADDRESS_OP_POST_READ                  (0x2 << ENET_MAC_MDIO_ADDRESS_OP_SHIFT) /* Post Read Increment Address for Clause 45 PHY */
#define ENET_MAC_MDIO_ADDRESS_OP_READ                       (0x3 << ENET_MAC_MDIO_ADDRESS_OP_SHIFT) /* Read */
#define ENET_MAC_MDIO_ADDRESS_SKAP                          (1 << 4)                                /* Bit 4: Skip Address Packet */
                                                                                                    /* Bits 5-7: Reserved */
#define ENET_MAC_MDIO_ADDRESS_CR_SHIFT                      (8)                                     /* Bits 8-11: CSR Clock Range */
#define ENET_MAC_MDIO_ADDRESS_CR_MASK                       (0xf << ENET_MAC_MDIO_ADDRESS_CR_SHIFT)
#define ENET_MAC_MDIO_ADDRESS_CR(x)                        (((uint32_t)(x) << ENET_MAC_MDIO_ADDRESS_CR_SHIFT) & ENET_MAC_MDIO_ADDRESS_CR_MASK)
#define ENET_MAC_MDIO_ADDRESS_CR_60M_100M                   (0x0 << ENET_MAC_MDIO_ADDRESS_CR_SHIFT) /* CSR clock = 60-100 MHz */
#define ENET_MAC_MDIO_ADDRESS_CR_100M_150M                  (0x1 << ENET_MAC_MDIO_ADDRESS_CR_SHIFT) /* CSR clock = 100-150 MHz */
#define ENET_MAC_MDIO_ADDRESS_CR_20M_35M                    (0x2 << ENET_MAC_MDIO_ADDRESS_CR_SHIFT) /* CSR clock = 20-35 MHz */
#define ENET_MAC_MDIO_ADDRESS_CR_35M_60M                    (0x3 << ENET_MAC_MDIO_ADDRESS_CR_SHIFT) /* CSR clock = 35-60 MHz */
#define ENET_MAC_MDIO_ADDRESS_CR_150M_250M                  (0x4 << ENET_MAC_MDIO_ADDRESS_CR_SHIFT) /* CSR clock = 150-250 MHz */
#define ENET_MAC_MDIO_ADDRESS_CR_250M_300M                  (0x5 << ENET_MAC_MDIO_ADDRESS_CR_SHIFT) /* CSR clock = 250-300 MHz */
#define ENET_MAC_MDIO_ADDRESS_CR_300M_500M                  (0x6 << ENET_MAC_MDIO_ADDRESS_CR_SHIFT) /* CSR clock = 300-500 MHz */
#define ENET_MAC_MDIO_ADDRESS_CR_500M_800M                  (0x7 << ENET_MAC_MDIO_ADDRESS_CR_SHIFT) /* CSR clock = 500-800 MHz */
#define ENET_MAC_MDIO_ADDRESS_CR_CSR_DIV4                   (0x8 << ENET_MAC_MDIO_ADDRESS_CR_SHIFT) /* CSR clock/4 */
#define ENET_MAC_MDIO_ADDRESS_CR_CSR_DIV6                   (0x9 << ENET_MAC_MDIO_ADDRESS_CR_SHIFT) /* CSR clock/6 */
#define ENET_MAC_MDIO_ADDRESS_CR_CSR_DIV8                   (0xa << ENET_MAC_MDIO_ADDRESS_CR_SHIFT) /* CSR clock/8 */
#define ENET_MAC_MDIO_ADDRESS_CR_CSR_DIV10                  (0xb << ENET_MAC_MDIO_ADDRESS_CR_SHIFT) /* CSR clock/10 */
#define ENET_MAC_MDIO_ADDRESS_CR_CSR_DIV12                  (0xc << ENET_MAC_MDIO_ADDRESS_CR_SHIFT) /* CSR clock/12 */
#define ENET_MAC_MDIO_ADDRESS_CR_CSR_DIV14                  (0xd << ENET_MAC_MDIO_ADDRESS_CR_SHIFT) /* CSR clock/14 */
#define ENET_MAC_MDIO_ADDRESS_CR_CSR_DIV16                  (0xe << ENET_MAC_MDIO_ADDRESS_CR_SHIFT) /* CSR clock/16 */
#define ENET_MAC_MDIO_ADDRESS_CR_CSR_DIV18                  (0xf << ENET_MAC_MDIO_ADDRESS_CR_SHIFT) /* CSR clock/18 */
#define ENET_MAC_MDIO_ADDRESS_NTC_SHIFT                     (12)                                    /* Bits 12-14: Number of Trailing Clocks */
#define ENET_MAC_MDIO_ADDRESS_NTC_MASK                      (7 << ENET_MAC_MDIO_ADDRESS_NTC_SHIFT)
#define ENET_MAC_MDIO_ADDRESS_NTC(x)                        (((uint32_t)(x) << ENET_MAC_MDIO_ADDRESS_NTC_SHIFT) & ENET_MAC_MDIO_ADDRESS_NTC_MASK)
                                                                        /* Bit 15: Reserved */
#define ENET_MAC_MDIO_ADDRESS_RDA_SHIFT                     (16)        /* Bits 16-20: Register/Device Address */
#define ENET_MAC_MDIO_ADDRESS_RDA_MASK                      (0x1f << ENET_MAC_MDIO_ADDRESS_RDA_SHIFT)
#define ENET_MAC_MDIO_ADDRESS_RDA(x)                        (((uint32_t)(x) << ENET_MAC_MDIO_ADDRESS_RDA_SHIFT) & ENET_MAC_MDIO_ADDRESS_RDA_MASK)
#define ENET_MAC_MDIO_ADDRESS_PA_SHIFT                      (21)        /* Bits 21-25: Physical Layer Address */
#define ENET_MAC_MDIO_ADDRESS_PA_MASK                       (0x1f << ENET_MAC_MDIO_ADDRESS_PA_SHIFT)
#define ENET_MAC_MDIO_ADDRESS_PA(x)                         (((uint32_t)(x) << ENET_MAC_MDIO_ADDRESS_PA_SHIFT) & ENET_MAC_MDIO_ADDRESS_PA_MASK)
#define ENET_MAC_MDIO_ADDRESS_BTB                           (1 << 26)   /* Bit 26: Back to Back transactions */
#define ENET_MAC_MDIO_ADDRESS_PSE                           (1 << 27)   /* Bit 27: Preamble Suppression Enable */
                                                                        /* Bits 28-31: Reserved */

/* MAC MDIO Data Register */

#define ENET_MAC_MDIO_DATA_GD_SHIFT                         (0)         /* Bits 0-15: GMII Data */
#define ENET_MAC_MDIO_DATA_GD_MASK                          (0xffff << ENET_MAC_MDIO_DATA_GD_SHIFT)
#define ENET_MAC_MDIO_DATA_GD(x)                            (((uint32_t)(x) << ENET_MAC_MDIO_DATA_GD_SHIFT) & ENET_MAC_MDIO_DATA_GD_MASK)
#define ENET_MAC_MDIO_DATA_RA_SHIFT                         (16)        /* Bits 16-31: Register Address */
#define ENET_MAC_MDIO_DATA_RA_MASK                          (0xffff << ENET_MAC_MDIO_DATA_RA_SHIFT)
#define ENET_MAC_MDIO_DATA_RA(x)                            (((uint32_t)(x) << ENET_MAC_MDIO_DATA_RA_SHIFT) & ENET_MAC_MDIO_DATA_RA_MASK)

/* MAC FPE Control STS Register */

#define ENET_MAC_FPE_CTRL_STS_EFPE                          (1 << 0)    /* Bit 0: Enable Tx Frame Preemption */
#define ENET_MAC_FPE_CTRL_STS_SVER                          (1 << 1)    /* Bit 1: Send Verify mPacket */
#define ENET_MAC_FPE_CTRL_STS_SRSP                          (1 << 2)    /* Bit 2: Send Respond mPacket */
                                                                        /* Bits 3-15: Reserved */
#define ENET_MAC_FPE_CTRL_STS_RVER                          (1 << 16)   /* Bit 16: Received Verify Frame */
#define ENET_MAC_FPE_CTRL_STS_RRSP                          (1 << 17)   /* Bit 17: Received Respond Frame */
#define ENET_MAC_FPE_CTRL_STS_TVER                          (1 << 18)   /* Bit 18: Transmitted Verify Frame */
#define ENET_MAC_FPE_CTRL_STS_TRSP                          (1 << 19)   /* Bit 19: Transmitted Respond Frame */
                                                                        /* Bits 20-31: Reserved */

/* MAC Address 0 High Register */

#define ENET_MAC_ADDRESS0_HIGH_ADDRHI_SHIFT                 (0)         /* Bits 0-15: MAC Address0[47:32] */
#define ENET_MAC_ADDRESS0_HIGH_ADDRHI_MASK                  (0xffff << ENET_MAC_ADDRESS0_HIGH_ADDRHI_SHIFT)
#define ENET_MAC_ADDRESS0_HIGH_ADDRHI(x)                    (((uint32_t)(x) << ENET_MAC_ADDRESS0_HIGH_ADDRHI_SHIFT) & ENET_MAC_ADDRESS0_HIGH_ADDRHI_MASK)
#define ENET_MAC_ADDRESS0_HIGH_DCS_SHIFT                    (16)        /* Bits 16-17: DMA Channel Select */
#define ENET_MAC_ADDRESS0_HIGH_DCS_MASK                     (3 << ENET_MAC_ADDRESS0_HIGH_DCS_SHIFT)
#define ENET_MAC_ADDRESS0_HIGH_DCS(x)                       (((uint32_t)(x) << ENET_MAC_ADDRESS0_HIGH_DCS_SHIFT) & ENET_MAC_ADDRESS0_HIGH_DCS_MASK)
                                                                        /* Bits 18-30: Reserved */
#define ENET_MAC_ADDRESS0_HIGH_AE                           (1 << 31)   /* Bit 31: Address Enable */

/* MMC Control Register */

#define ENET_MMC_CONTROL_CNTRST                             (1 << 0)    /* Bit 0: Counters Reset */
#define ENET_MMC_CONTROL_CNTSTOPRO                          (1 << 1)    /* Bit 1: Counter Stop Rollover */
#define ENET_MMC_CONTROL_RSTONRD                            (1 << 2)    /* Bit 2: Reset on Read */
#define ENET_MMC_CONTROL_CNTFREEZ                           (1 << 3)    /* Bit 3: MMC Counter Freeze */
#define ENET_MMC_CONTROL_CNTPRST                            (1 << 4)    /* Bit 4: Counters Preset */
#define ENET_MMC_CONTROL_CNTPRSTLVL                         (1 << 5)    /* Bit 5: Full-Half Preset */
                                                                        /* Bits 6-7: Reserved */
#define ENET_MMC_CONTROL_UCDBC                              (1 << 8)    /* Bit 8: Update MMC Counters for Dropped Broadcast Packets */
                                                                        /* Bits 9-31: Reserved */

/* MMC Receive Interrupt Register */

#define ENET_MMC_RX_INTERRUPT_RXGBPKTIS                     (1 << 0)    /* Bit 0: MMC Receive Good Bad Packet Counter Interrupt Status */
#define ENET_MMC_RX_INTERRUPT_RXGBOCTIS                     (1 << 1)    /* Bit 1: MMC Receive Good Bad Octet Counter Interrupt Status */
#define ENET_MMC_RX_INTERRUPT_RXGOCTIS                      (1 << 2)    /* Bit 2: MMC Receive Good Octet Counter Interrupt Status */
#define ENET_MMC_RX_INTERRUPT_RXBCGPIS                      (1 << 3)    /* Bit 3: MMC Receive Broadcast Good Packet Counter Interrupt Status */
#define ENET_MMC_RX_INTERRUPT_RXMCGPIS                      (1 << 4)    /* Bit 4: MMC Receive Multicast Good Packet Counter Interrupt Status */
#define ENET_MMC_RX_INTERRUPT_RXCRCERPIS                    (1 << 5)    /* Bit 5: MMC Receive CRC Error Packet Counter Interrupt Status */
#define ENET_MMC_RX_INTERRUPT_RXALGNERPIS                   (1 << 6)    /* Bit 6: MMC Receive Alignment Error Packet Counter Interrupt Status */
#define ENET_MMC_RX_INTERRUPT_RXRUNTPIS                     (1 << 7)    /* Bit 7: MMC Receive Runt Packet Counter Interrupt Status */
#define ENET_MMC_RX_INTERRUPT_RXJABERPIS                    (1 << 8)    /* Bit 8: MMC Receive Jabber Error Packet Counter Interrupt Status */
#define ENET_MMC_RX_INTERRUPT_RXUSIZEGPIS                   (1 << 9)    /* Bit 9: MMC Receive Undersize Good Packet Counter Interrupt Status */
#define ENET_MMC_RX_INTERRUPT_RXOSIZEGPIS                   (1 << 10)   /* Bit 10: MMC Receive Oversize Good Packet Counter Interrupt Status */
#define ENET_MMC_RX_INTERRUPT_RX64OCTGBPIS                  (1 << 11)   /* Bit 11: MMC Receive 64 Octet Good Bad Packet Counter Interrupt Status */
#define ENET_MMC_RX_INTERRUPT_RX65T127OCTGBPIS              (1 << 12)   /* Bit 12: MMC Receive 65 to 127 Octet Good Bad Packet Counter Interrupt Status */
#define ENET_MMC_RX_INTERRUPT_RX128T255OCTGBPIS             (1 << 13)   /* Bit 13: MMC Receive 128 to 255 Octet Good Bad Packet Counter Interrupt Status */
#define ENET_MMC_RX_INTERRUPT_RX256T511OCTGBPIS             (1 << 14)   /* Bit 14: MMC Receive 256 to 511 Octet Good Bad Packet Counter Interrupt Status */
#define ENET_MMC_RX_INTERRUPT_RX512T1023OCTGBPIS            (1 << 15)   /* Bit 15: MMC Receive 512 to 1023 Octet Good Bad Packet Counter Interrupt Status */
#define ENET_MMC_RX_INTERRUPT_RX1024TMAXOCTGBPIS            (1 << 16)   /* Bit 16: MMC Receive 1024 to Maximum Octet Good Bad Packet Counter Interrupt Status */
#define ENET_MMC_RX_INTERRUPT_RXUCGPIS                      (1 << 17)   /* Bit 17: MMC Receive Unicast Good Packet Counter Interrupt Status */
#define ENET_MMC_RX_INTERRUPT_RXLENERPIS                    (1 << 18)   /* Bit 18: MMC Receive Length Error Packet Counter Interrupt Status */
#define ENET_MMC_RX_INTERRUPT_RXORANGEPIS                   (1 << 19)   /* Bit 19: MMC Receive Out Of Range Error Packet Counter Interrupt Status */
#define ENET_MMC_RX_INTERRUPT_RXPAUSPIS                     (1 << 20)   /* Bit 20: MMC Receive Pause Packet Counter Interrupt Status */
#define ENET_MMC_RX_INTERRUPT_RXFOVPIS                      (1 << 21)   /* Bit 21: MMC Receive FIFO Overflow Packet Counter Interrupt Status */
#define ENET_MMC_RX_INTERRUPT_RXVLANGBPIS                   (1 << 22)   /* Bit 22: MMC Receive VLAN Good Bad Packet Counter Interrupt Status */
#define ENET_MMC_RX_INTERRUPT_RXWDOGPIS                     (1 << 23)   /* Bit 23: MMC Receive Watchdog Error Packet Counter Interrupt Status */
#define ENET_MMC_RX_INTERRUPT_RXRCVERRPIS                   (1 << 24)   /* Bit 24: MMC Receive Error Packet Counter Interrupt Status */
#define ENET_MMC_RX_INTERRUPT_RXCTRLPIS                     (1 << 25)   /* Bit 25: MMC Receive Control Packet Counter Interrupt Status */
                                                                        /* Bits 26-31: Reserved */

/* MMC Transmit Interrupt Register */

#define ENET_MMC_TX_INTERRUPT_TXGBOCTIS                     (1 << 0)    /* Bit 0: MMC Transmit Good Bad Octet Counter Interrupt Status */
#define ENET_MMC_TX_INTERRUPT_TXGBPKTIS                     (1 << 1)    /* Bit 1: MMC Transmit Good Bad Packet Counter Interrupt Status */
#define ENET_MMC_TX_INTERRUPT_TXBCGPIS                      (1 << 2)    /* Bit 2: MMC Transmit Broadcast Good Packet Counter Interrupt Status */
#define ENET_MMC_TX_INTERRUPT_TXMCGPIS                      (1 << 3)    /* Bit 3: MMC Transmit Multicast Good Packet Counter Interrupt Status */
#define ENET_MMC_TX_INTERRUPT_TX64OCTGBPIS                  (1 << 4)    /* Bit 4: MMC Transmit 64 Octet Good Bad Packet Counter Interrupt Status */
#define ENET_MMC_TX_INTERRUPT_TX65T127OCTGBPIS              (1 << 5)    /* Bit 5: MMC Transmit 65 to 127 Octet Good Bad Packet Counter Interrupt Status */
#define ENET_MMC_TX_INTERRUPT_TX128T255OCTGBPIS             (1 << 6)    /* Bit 6: MMC Transmit 128 to 255 Octet Good Bad Packet Counter Interrupt Status */
#define ENET_MMC_TX_INTERRUPT_TX256T511OCTGBPIS             (1 << 7)    /* Bit 7: MMC Transmit 256 to 511 Octet Good Bad Packet Counter Interrupt Status */
#define ENET_MMC_TX_INTERRUPT_TX512T1023OCTGBPIS            (1 << 8)    /* Bit 8: MMC Transmit 512 to 1023 Octet Good Bad Packet Counter Interrupt Status */
#define ENET_MMC_TX_INTERRUPT_TX1024TMAXOCTGBPIS            (1 << 9)    /* Bit 9: MMC Transmit 1024 to Maximum Octet Good Bad Packet Counter Interrupt Status */
#define ENET_MMC_TX_INTERRUPT_TXUCGBPIS                     (1 << 10)   /* Bit 10: MMC Transmit Unicast Good Bad Packet Counter Interrupt Status */
#define ENET_MMC_TX_INTERRUPT_TXMCGBPIS                     (1 << 11)   /* Bit 11: MMC Transmit Multicast Good Bad Packet Counter Interrupt Status */
#define ENET_MMC_TX_INTERRUPT_TXBCGBPIS                     (1 << 12)   /* Bit 12: MMC Transmit Broadcast Good Bad Packet Counter Interrupt Status */
#define ENET_MMC_TX_INTERRUPT_TXUFLOWERPIS                  (1 << 13)   /* Bit 13: MMC Transmit Underflow Error Packet Counter Interrupt Status */
#define ENET_MMC_TX_INTERRUPT_TXSCOLGPIS                    (1 << 14)   /* Bit 14: MMC Transmit Single Collision Good Packet Counter Interrupt Status */
#define ENET_MMC_TX_INTERRUPT_TXMCOLGPIS                    (1 << 15)   /* Bit 15: MMC Transmit Multiple Collision Good Packet Counter Interrupt Status */
#define ENET_MMC_TX_INTERRUPT_TXDEFPIS                      (1 << 16)   /* Bit 16: MMC Transmit Deferred Packet Counter Interrupt Status */
#define ENET_MMC_TX_INTERRUPT_TXLATCOLPIS                   (1 << 17)   /* Bit 17: MMC Transmit Late Collision Packet Counter Interrupt Status */
#define ENET_MMC_TX_INTERRUPT_TXEXCOLPIS                    (1 << 18)   /* Bit 18: MMC Transmit Excessive Collision Packet Counter Interrupt Status */
#define ENET_MMC_TX_INTERRUPT_TXCARERPIS                    (1 << 19)   /* Bit 19: MMC Transmit Carrier Error Packet Counter Interrupt Status */
#define ENET_MMC_TX_INTERRUPT_TXGOCTIS                      (1 << 20)   /* Bit 20: MMC Transmit Good Octet Counter Interrupt Status */
#define ENET_MMC_TX_INTERRUPT_TXGPKTIS                      (1 << 21)   /* Bit 21: MMC Transmit Good Packet Counter Interrupt Status */
#define ENET_MMC_TX_INTERRUPT_TXEXDEFPIS                    (1 << 22)   /* Bit 22: MMC Transmit Excessive Deferral Packet Counter Interrupt Status */
#define ENET_MMC_TX_INTERRUPT_TXPAUSPIS                     (1 << 23)   /* Bit 23: MMC Transmit Pause Packet Counter Interrupt Status */
#define ENET_MMC_TX_INTERRUPT_TXVLANGPIS                    (1 << 24)   /* Bit 24: MMC Transmit VLAN Good Packet Counter Interrupt Status */
#define ENET_MMC_TX_INTERRUPT_TXOSIZEGPIS                   (1 << 25)   /* Bit 25: MMC Transmit Oversize Good Packet Counter Interrupt Status */
                                                                        /* Bits 26-31: Reserved */

/* MMC Receive Interrupt Mask Register */

#define ENET_MMC_RX_INTERRUPT_MASK                          0x3ffffff
#define ENET_MMC_RX_INTERRUPT_MASK_RXGBPKTIM                (1 << 0)    /* Bit 0: MMC Receive Good Bad Packet Counter Interrupt Mask */
#define ENET_MMC_RX_INTERRUPT_MASK_RXGBOCTIM                (1 << 1)    /* Bit 1: MMC Receive Good Bad Octet Counter Interrupt Mask */
#define ENET_MMC_RX_INTERRUPT_MASK_RXGOCTIM                 (1 << 2)    /* Bit 2: MMC Receive Good Octet Counter Interrupt Mask */
#define ENET_MMC_RX_INTERRUPT_MASK_RXBCGPIM                 (1 << 3)    /* Bit 3: MMC Receive Broadcast Good Packet Counter Interrupt Mask */
#define ENET_MMC_RX_INTERRUPT_MASK_RXMCGPIM                 (1 << 4)    /* Bit 4: MMC Receive Multicast Good Packet Counter Interrupt Mask */
#define ENET_MMC_RX_INTERRUPT_MASK_RXCRCERPIM               (1 << 5)    /* Bit 5: MMC Receive CRC Error Packet Counter Interrupt Mask */
#define ENET_MMC_RX_INTERRUPT_MASK_RXALGNERPIM              (1 << 6)    /* Bit 6: MMC Receive Alignment Error Packet Counter Interrupt Mask */
#define ENET_MMC_RX_INTERRUPT_MASK_RXRUNTPIM                (1 << 7)    /* Bit 7: MMC Receive Runt Packet Counter Interrupt Mask */
#define ENET_MMC_RX_INTERRUPT_MASK_RXJABERPIM               (1 << 8)    /* Bit 8: MMC Receive Jabber Error Packet Counter Interrupt Mask */
#define ENET_MMC_RX_INTERRUPT_MASK_RXUSIZEGPIM              (1 << 9)    /* Bit 9: MMC Receive Undersize Good Packet Counter Interrupt Mask */
#define ENET_MMC_RX_INTERRUPT_MASK_RXOSIZEGPIM              (1 << 10)   /* Bit 10: MMC Receive Oversize Good Packet Counter Interrupt Mask */
#define ENET_MMC_RX_INTERRUPT_MASK_RX64OCTGBPIM             (1 << 11)   /* Bit 11: MMC Receive 64 Octet Good Bad Packet Counter Interrupt Mask */
#define ENET_MMC_RX_INTERRUPT_MASK_RX65T127OCTGBPIM         (1 << 12)   /* Bit 12: MMC Receive 65 to 127 Octet Good Bad Packet Counter Interrupt Mask */
#define ENET_MMC_RX_INTERRUPT_MASK_RX128T255OCTGBPIM        (1 << 13)   /* Bit 13: MMC Receive 128 to 255 Octet Good Bad Packet Counter Interrupt Mask */
#define ENET_MMC_RX_INTERRUPT_MASK_RX256T511OCTGBPIM        (1 << 14)   /* Bit 14: MMC Receive 256 to 511 Octet Good Bad Packet Counter Interrupt Mask */
#define ENET_MMC_RX_INTERRUPT_MASK_RX512T1023OCTGBPIM       (1 << 15)   /* Bit 15: MMC Receive 512 to 1023 Octet Good Bad Packet Counter Interrupt Mask */
#define ENET_MMC_RX_INTERRUPT_MASK_RX1024TMAXOCTGBPIM       (1 << 16)   /* Bit 16: MMC Receive 1024 to Maximum Octet Good Bad Packet Counter Interrupt Mask */
#define ENET_MMC_RX_INTERRUPT_MASK_RXUCGPIM                 (1 << 17)   /* Bit 17: MMC Receive Unicast Good Packet Counter Interrupt Mask */
#define ENET_MMC_RX_INTERRUPT_MASK_RXLENERPIM               (1 << 18)   /* Bit 18: MMC Receive Length Error Packet Counter Interrupt Mask */
#define ENET_MMC_RX_INTERRUPT_MASK_RXORANGEPIM              (1 << 19)   /* Bit 19: MMC Receive Out Of Range Error Packet Counter Interrupt Mask */
#define ENET_MMC_RX_INTERRUPT_MASK_RXPAUSPIM                (1 << 20)   /* Bit 20: MMC Receive Pause Packet Counter Interrupt Mask */
#define ENET_MMC_RX_INTERRUPT_MASK_RXFOVPIM                 (1 << 21)   /* Bit 21: MMC Receive FIFO Overflow Packet Counter Interrupt Mask */
#define ENET_MMC_RX_INTERRUPT_MASK_RXVLANGBPIM              (1 << 22)   /* Bit 22: MMC Receive VLAN Good Bad Packet Counter Interrupt Mask */
#define ENET_MMC_RX_INTERRUPT_MASK_RXWDOGPIM                (1 << 23)   /* Bit 23: MMC Receive Watchdog Error Packet Counter Interrupt Mask */
#define ENET_MMC_RX_INTERRUPT_MASK_RXRCVERRPIM              (1 << 24)   /* Bit 24: MMC Receive Error Packet Counter Interrupt Mask */
#define ENET_MMC_RX_INTERRUPT_MASK_RXCTRLPIM                (1 << 25)   /* Bit 25: MMC Receive Control Packet Counter Interrupt Mask */
                                                                        /* Bits 26-31: Reserved */

/* MMC Transmit Interrupt Mask Register */

#define ENET_MMC_TX_INTERRUPT_MASK                          0x3ffffff
#define ENET_MMC_TX_INTERRUPT_MASK_TXGBOCTIM                (1 << 0)    /* Bit 0: MMC Transmit Good Bad Octet Counter Interrupt Mask */
#define ENET_MMC_TX_INTERRUPT_MASK_TXGBPKTIM                (1 << 1)    /* Bit 1: MMC Transmit Good Bad Packet Counter Interrupt Mask */
#define ENET_MMC_TX_INTERRUPT_MASK_TXBCGPIM                 (1 << 2)    /* Bit 2: MMC Transmit Broadcast Good Packet Counter Interrupt Mask */
#define ENET_MMC_TX_INTERRUPT_MASK_TXMCGPIM                 (1 << 3)    /* Bit 3: MMC Transmit Multicast Good Packet Counter Interrupt Mask */
#define ENET_MMC_TX_INTERRUPT_MASK_TX64OCTGBPIM             (1 << 4)    /* Bit 4: MMC Transmit 64 Octet Good Bad Packet Counter Interrupt Mask */
#define ENET_MMC_TX_INTERRUPT_MASK_TX65T127OCTGBPIM         (1 << 5)    /* Bit 5: MMC Transmit 65 to 127 Octet Good Bad Packet Counter Interrupt Mask */
#define ENET_MMC_TX_INTERRUPT_MASK_TX128T255OCTGBPIM        (1 << 6)    /* Bit 6: MMC Transmit 128 to 255 Octet Good Bad Packet Counter Interrupt Mask */
#define ENET_MMC_TX_INTERRUPT_MASK_TX256T511OCTGBPIM        (1 << 7)    /* Bit 7: MMC Transmit 256 to 511 Octet Good Bad Packet Counter Interrupt Mask */
#define ENET_MMC_TX_INTERRUPT_MASK_TX512T1023OCTGBPIM       (1 << 8)    /* Bit 8: MMC Transmit 512 to 1023 Octet Good Bad Packet Counter Interrupt Mask */
#define ENET_MMC_TX_INTERRUPT_MASK_TX1024TMAXOCTGBPIM       (1 << 9)    /* Bit 9: MMC Transmit 1024 to Maximum Octet Good Bad Packet Counter Interrupt Mask */
#define ENET_MMC_TX_INTERRUPT_MASK_TXUCGBPIM                (1 << 10)   /* Bit 10: MMC Transmit Unicast Good Bad Packet Counter Interrupt Mask */
#define ENET_MMC_TX_INTERRUPT_MASK_TXMCGBPIM                (1 << 11)   /* Bit 11: MMC Transmit Multicast Good Bad Packet Counter Interrupt Mask */
#define ENET_MMC_TX_INTERRUPT_MASK_TXBCGBPIM                (1 << 12)   /* Bit 12: MMC Transmit Broadcast Good Bad Packet Counter Interrupt Mask */
#define ENET_MMC_TX_INTERRUPT_MASK_TXUFLOWERPIM             (1 << 13)   /* Bit 13: MMC Transmit Underflow Error Packet Counter Interrupt Mask */
#define ENET_MMC_TX_INTERRUPT_MASK_TXSCOLGPIM               (1 << 14)   /* Bit 14: MMC Transmit Single Collision Good Packet Counter Interrupt Mask */
#define ENET_MMC_TX_INTERRUPT_MASK_TXMCOLGPIM               (1 << 15)   /* Bit 15: MMC Transmit Multiple Collision Good Packet Counter Interrupt Mask */
#define ENET_MMC_TX_INTERRUPT_MASK_TXDEFPIM                 (1 << 16)   /* Bit 16: MMC Transmit Deferred Packet Counter Interrupt Mask */
#define ENET_MMC_TX_INTERRUPT_MASK_TXLATCOLPIM              (1 << 17)   /* Bit 17: MMC Transmit Late Collision Packet Counter Interrupt Mask */
#define ENET_MMC_TX_INTERRUPT_MASK_TXEXCOLPIM               (1 << 18)   /* Bit 18: MMC Transmit Excessive Collision Packet Counter Interrupt Mask */
#define ENET_MMC_TX_INTERRUPT_MASK_TXCARERPIM               (1 << 19)   /* Bit 19: MMC Transmit Carrier Error Packet Counter Interrupt Mask */
#define ENET_MMC_TX_INTERRUPT_MASK_TXGOCTIM                 (1 << 20)   /* Bit 20: MMC Transmit Good Octet Counter Interrupt Mask */
#define ENET_MMC_TX_INTERRUPT_MASK_TXGPKTIM                 (1 << 21)   /* Bit 21: MMC Transmit Good Packet Counter Interrupt Mask */
#define ENET_MMC_TX_INTERRUPT_MASK_TXEXDEFPIM               (1 << 22)   /* Bit 22: MMC Transmit Excessive Deferral Packet Counter Interrupt Mask */
#define ENET_MMC_TX_INTERRUPT_MASK_TXPAUSPIM                (1 << 23)   /* Bit 23: MMC Transmit Pause Packet Counter Interrupt Mask */
#define ENET_MMC_TX_INTERRUPT_MASK_TXVLANGPIM               (1 << 24)   /* Bit 24: MMC Transmit VLAN Good Packet Counter Interrupt Mask */
#define ENET_MMC_TX_INTERRUPT_MASK_TXOSIZEGPIM              (1 << 25)   /* Bit 25: MMC Transmit Oversize Good Packet Counter Interrupt Mask */
                                                                        /* Bits 26-31: Reserved */

/* MAC Layer 3 Layer 4 Control Register */

#define ENET_MAC_L3_L4_CONTROL_L3PEN                        (1 << 0)    /* Bit 0: Layer 3 Protocol Enable */
                                                                        /* Bit 1: Reserved */
#define ENET_MAC_L3_L4_CONTROL_L3SAM                        (1 << 2)    /* Bit 2: Layer 3 IP SA Match Enable */
#define ENET_MAC_L3_L4_CONTROL_L3SAIM                       (1 << 3)    /* Bit 3: Layer 3 IP SA Inverse Match Enable */
#define ENET_MAC_L3_L4_CONTROL_L3DAM                        (1 << 4)    /* Bit 4: Layer 3 IP DA Match Enable */
#define ENET_MAC_L3_L4_CONTROL_L3DAIM                       (1 << 5)    /* Bit 5: Layer 3 IP DA Inverse Match Enable */
#define ENET_MAC_L3_L4_CONTROL_L3HSBM_SHIFT                 (6)         /* Bits 6-10: Layer 3 IP SA Higher Bits Match */
#define ENET_MAC_L3_L4_CONTROL_L3HSBM_MASK                  (0x1f << ENET_MAC_L3_L4_CONTROL_L3HSBM_SHIFT)
#define ENET_MAC_L3_L4_CONTROL_L3HSBM(x)                    (((uint32_t)(x) << ENET_MAC_L3_L4_CONTROL_L3HSBM_SHIFT) & ENET_MAC_L3_L4_CONTROL_L3HSBM_MASK)
#define ENET_MAC_L3_L4_CONTROL_L3HDBM_SHIFT                 (11)        /* Bits 11-15: Layer 3 IP DA Higher Bits Match */
#define ENET_MAC_L3_L4_CONTROL_L3HDBM_MASK                  (0x1f << ENET_MAC_L3_L4_CONTROL_L3HDBM_SHIFT)
#define ENET_MAC_L3_L4_CONTROL_L3HDBM(x)                    (((uint32_t)(x) << ENET_MAC_L3_L4_CONTROL_L3HDBM_SHIFT) & ENET_MAC_L3_L4_CONTROL_L3HDBM_MASK)
#define ENET_MAC_L3_L4_CONTROL_L4PEN                        (1 << 16)   /* Bit 16: Layer 4 Protocol Enable */
                                                                        /* Bit 17: Reserved */
#define ENET_MAC_L3_L4_CONTROL_L4SPM                        (1 << 18)   /* Bit 18: Layer 4 Source Port Match Enable */
#define ENET_MAC_L3_L4_CONTROL_L4SPIM                       (1 << 19)   /* Bit 19: Layer 4 Source Port Inverse Match Enable */
#define ENET_MAC_L3_L4_CONTROL_L4DPM                        (1 << 20)   /* Bit 20: Layer 4 Destination Port Match Enable */
#define ENET_MAC_L3_L4_CONTROL_L4DPIM                       (1 << 21)   /* Bit 21: Layer 4 Destination Port Inverse Match Enable */
                                                                        /* Bits 22-23: Reserved */
#define ENET_MAC_L3_L4_CONTROL_DMCHN_SHIFT                  (24)        /* Bit 24: DMA Channel Number */
#define ENET_MAC_L3_L4_CONTROL_DMCHN_MASK                   (0x1 << ENET_MAC_L3_L4_CONTROL_DMCHN_SHIFT)
#define ENET_MAC_L3_L4_CONTROL_DMCHN(x)                     (((uint32_t)(x) << ENET_MAC_L3_L4_CONTROL_DMCHN_SHIFT) & ENET_MAC_L3_L4_CONTROL_DMCHN_MASK)
                                                                        /* Bits 25-27: Reserved */
#define ENET_MAC_L3_L4_CONTROL_DMCHEN                       (1 << 28)   /* Bit 28: DMA Channel Select Enable */
                                                                        /* Bits 29-31: Reserved */

/* MAC Layer 4 Address Register */

#define ENET_MAC_LAYER4_ADDRESS_L4SP_SHIFT                  (0)         /* Bits 0-15: Layer 4 Source Port Number Field */
#define ENET_MAC_LAYER4_ADDRESS_L4SP_MASK                   (0xffff << ENET_MAC_LAYER4_ADDRESS_L4SP_SHIFT)
#define ENET_MAC_LAYER4_ADDRESS_L4SP(x)                     (((uint32_t)(x) << ENET_MAC_LAYER4_ADDRESS_L4SP_SHIFT) & ENET_MAC_LAYER4_ADDRESS_L4SP_MASK)
#define ENET_MAC_LAYER4_ADDRESS_L4DP_SHIFT                  (16)        /* Bits 16-31: Layer 4 Destination Port Number Field */
#define ENET_MAC_LAYER4_ADDRESS_L4DP_MASK                   (0xffff << ENET_MAC_LAYER4_ADDRESS_L4DP_SHIFT)
#define ENET_MAC_LAYER4_ADDRESS_L4DP(x)                     (((uint32_t)(x) << ENET_MAC_LAYER4_ADDRESS_L4DP_SHIFT) & ENET_MAC_LAYER4_ADDRESS_L4DP_MASK)

/* MAC Timestamp Control Register */

#define ENET_MAC_TIMESTAMP_CONTROL_TSENA                    (1 << 0)    /* Bit 0: Enable Timestamp */
#define ENET_MAC_TIMESTAMP_CONTROL_TSCFUPDT                 (1 << 1)    /* Bit 1: Fine or Coarse Timestamp Update */
#define ENET_MAC_TIMESTAMP_CONTROL_TSINIT                   (1 << 2)    /* Bit 2: Initialize Timestamp */
#define ENET_MAC_TIMESTAMP_CONTROL_TSUPDT                   (1 << 3)    /* Bit 3: Update Timestamp */
                                                                        /* Bit 4: Reserved */
#define ENET_MAC_TIMESTAMP_CONTROL_TSADDREG                 (1 << 5)    /* Bit 5: Update Addend Register */
#define ENET_MAC_TIMESTAMP_CONTROL_PTGE                     (1 << 6)    /* Bit 6: Presentation Time Generation Enable */
                                                                        /* Bit 7: Reserved */
#define ENET_MAC_TIMESTAMP_CONTROL_TSENALL                  (1 << 8)    /* Bit 8: Enable Timestamp for All Packets */
#define ENET_MAC_TIMESTAMP_CONTROL_TSCTRLSSR                (1 << 9)    /* Bit 9: Timestamp Digital or Binary Rollover Control */
#define ENET_MAC_TIMESTAMP_CONTROL_TSVER2ENA                (1 << 10)   /* Bit 10: Enable PTP Packet Processing for Version 2 Format */
#define ENET_MAC_TIMESTAMP_CONTROL_TSIPENA                  (1 << 11)   /* Bit 11: Enable Processing of PTP over Ethernet Packets */
#define ENET_MAC_TIMESTAMP_CONTROL_TSIPV6ENA                (1 << 12)   /* Bit 12: Enable Processing of PTP Packets Sent over IPv6-UDP */
#define ENET_MAC_TIMESTAMP_CONTROL_TSIPV4ENA                (1 << 13)   /* Bit 13: Enable Processing of PTP Packets Sent over IPv4-UDP */
#define ENET_MAC_TIMESTAMP_CONTROL_TSEVNTENA                (1 << 14)   /* Bit 14: Enable Timestamp Snapshot for Event Messages */
#define ENET_MAC_TIMESTAMP_CONTROL_TSMSTRENA                (1 << 15)   /* Bit 15: Enable Snapshot for Messages Relevant to Master */
#define ENET_MAC_TIMESTAMP_CONTROL_SNAPTYPSEL_SHIFT         (16)        /* Bits 16-17: Select PTP packets for Taking Snapshots */
#define ENET_MAC_TIMESTAMP_CONTROL_SNAPTYPSEL_MASK          (3 << ENET_MAC_TIMESTAMP_CONTROL_SNAPTYPSEL_SHIFT)
#define ENET_MAC_TIMESTAMP_CONTROL_SNAPTYPSEL(x)            (((uint32_t)(x) << ENET_MAC_TIMESTAMP_CONTROL_SNAPTYPSEL_SHIFT) & ENET_MAC_TIMESTAMP_CONTROL_SNAPTYPSEL_MASK)
#define ENET_MAC_TIMESTAMP_CONTROL_TSENMACADDR              (1 << 18)   /* Bit 18: Enable MAC Address for PTP Packet Filtering */
                                                                        /* Bits 19-23: Reserved */
#define ENET_MAC_TIMESTAMP_CONTROL_TXTSSTSM                 (1 << 24)   /* Bit 24: Transmit Timestamp Status Mode */
                                                                        /* Bits 25-27: Reserved */
#define ENET_MAC_TIMESTAMP_CONTROL_AV8021ASMEN              (1 << 28)   /* Bit 28: AV 802.1AS Mode Enable */
                                                                        /* Bits 29-31: Reserved */

/* MAC Sub Second Increment Register */

#define ENET_MAC_SUB_SECOND_INCREMENT_SNSINC_SHIFT          (8)         /* Bits 8-15: Sub-nanosecond Increment Value */
#define ENET_MAC_SUB_SECOND_INCREMENT_SNSINC_MASK           (0xff << ENET_MAC_SUB_SECOND_INCREMENT_SNSINC_SHIFT)
#define ENET_MAC_SUB_SECOND_INCREMENT_SNSINC(x)             (((uint32_t)(x) << ENET_MAC_SUB_SECOND_INCREMENT_SNSINC_SHIFT) & ENET_MAC_SUB_SECOND_INCREMENT_SNSINC_MASK)
#define ENET_MAC_SUB_SECOND_INCREMENT_SSINC_SHIFT           (16)        /* Bits 16-23: Sub-second Increment Value */
#define ENET_MAC_SUB_SECOND_INCREMENT_SSINC_MASK            (0xff << ENET_MAC_SUB_SECOND_INCREMENT_SSINC_SHIFT)
#define ENET_MAC_SUB_SECOND_INCREMENT_SSINC(x)              (((uint32_t)(x) << ENET_MAC_SUB_SECOND_INCREMENT_SSINC_SHIFT) & ENET_MAC_SUB_SECOND_INCREMENT_SSINC_MASK)
                                                                        /* Bits 24-31: Reserved */

/* MAC System Time In Seconds Register (32-bit address) */

/* MAC System Time In Nanoseconds Register */

#define ENET_MAC_SYSTEM_TIME_NANOSECONDS_TSSS_SHIFT         (0)         /* Bits 0-30: Timestamp Sub Seconds */
#define ENET_MAC_SYSTEM_TIME_NANOSECONDS_TSSS_MASK          (0x7fffffff << ENET_MAC_SYSTEM_TIME_NANOSECONDS_TSSS_SHIFT)
#define ENET_MAC_SYSTEM_TIME_NANOSECONDS_TSSS(x)            (((uint32_t)(x) << ENET_MAC_SYSTEM_TIME_NANOSECONDS_TSSS_SHIFT) & ENET_MAC_SYSTEM_TIME_NANOSECONDS_TSSS_MASK)
                                                                        /* Bit 31: Reserved */

/* MAC System Time Seconds Update Register (32-bit address) */

/* MAC System Time Nanoseconds Update Register */

#define ENET_MAC_SYSTEM_TIME_NSEC_UPDATE_TSSS_SHIFT         (0)         /* Bits 0-30: Timestamp Sub Seconds */
#define ENET_MAC_SYSTEM_TIME_NSEC_UPDATE_TSSS_MASK          (0x7fffffff << ENET_MAC_SYSTEM_TIME_NSEC_UPDATE_TSSS_SHIFT)
#define ENET_MAC_SYSTEM_TIME_NSEC_UPDATE_TSSS(x)            (((uint32_t)(x) << ENET_MAC_SYSTEM_TIME_NSEC_UPDATE_TSSS_SHIFT) & ENET_MAC_SYSTEM_TIME_NSEC_UPDATE_TSSS_MASK)
#define ENET_MAC_SYSTEM_TIME_NSEC_UPDATE_ADDSUB             (1 << 31)   /* Bit 31: Add or Subtract Time */

/* MAC Timestamp Addend Register (32-bit address) */

/* MAC System Time Higher Word In Seconds Register */

#define ENET_MAC_SYSTIME_HIGHER_WORD_SEC_TSHWR_SHIFT         (0)        /* Bits 0-15: Timestamp Sub Seconds */
#define ENET_MAC_SYSTIME_HIGHER_WORD_SEC_TSHWR_MASK          (0xffff << ENET_MAC_SYSTIME_HIGHER_WORD_SEC_TSHWR_SHIFT)
#define ENET_MAC_SYSTIME_HIGHER_WORD_SEC_TSHWR(x)            (((uint32_t)(x) << ENET_MAC_SYSTIME_HIGHER_WORD_SEC_TSHWR_SHIFT) & ENET_MAC_SYSTIME_HIGHER_WORD_SEC_TSHWR_MASK)
                                                                        /* Bits 16-31: Reserved */

/* MAC Timestamp Status Register */

#define ENET_MAC_TIMESTAMP_STATUS_TSSOVF                    (1 << 0)    /* Bit 0: Timestamp Seconds Overflow */
#define ENET_MAC_TIMESTAMP_STATUS_TSTARGT0                  (1 << 1)    /* Bit 1: Timestamp Target Time 0 Reached */
                                                                        /* Bit 2: Reserved */
#define ENET_MAC_TIMESTAMP_STATUS_TSTRGTERR0                (1 << 3)    /* Bit 3: Timestamp Target Time 1 Error */
#define ENET_MAC_TIMESTAMP_STATUS_TSTARGT1                  (1 << 4)    /* Bit 4: Timestamp Target Time 1 Reached */
#define ENET_MAC_TIMESTAMP_STATUS_TSTRGTERR1                (1 << 5)    /* Bit 5: Timestamp Target Time 1 Error */
#define ENET_MAC_TIMESTAMP_STATUS_TSTARGT2                  (1 << 6)    /* Bit 6: Timestamp Target Time 2 Reached */
#define ENET_MAC_TIMESTAMP_STATUS_TSTRGTERR2                (1 << 7)    /* Bit 7: Timestamp Target Time 2 Error */
#define ENET_MAC_TIMESTAMP_STATUS_TSTARGT3                  (1 << 8)    /* Bit 8: Timestamp Target Time 3 Reached */
#define ENET_MAC_TIMESTAMP_STATUS_TSTRGTERR3                (1 << 9)    /* Bit 9: Timestamp Target Time 3 Error */
                                                                        /* Bits 10-14: Reserved */
#define ENET_MAC_TIMESTAMP_STATUS_TXTSSIS                   (1 << 15)   /* Bit 15: Tx Timestamp Status Interrupt Status */
                                                                        /* Bits 16-31: Reserved */

/* MAC Transmit Timestamp Status In Nanoseconds Register */

#define ENET_MAC_TX_TIMESTAMP_STATUS_NSEC_TXTSSLO_SHIFT     (0)         /* Bits 0-30: Transmit Timestamp Status Low */
#define ENET_MAC_TX_TIMESTAMP_STATUS_NSEC_TXTSSLO_MASK      (0x7fffffff << ENET_MAC_TX_TIMESTAMP_STATUS_NSEC_TXTSSLO_SHIFT)
#define ENET_MAC_TX_TIMESTAMP_STATUS_NSEC_TXTSSLO(x)        (((uint32_t)(x) << ENET_MAC_TX_TIMESTAMP_STATUS_NSEC_TXTSSLO_SHIFT) & ENET_MAC_TX_TIMESTAMP_STATUS_NSEC_TXTSSLO_MASK)
#define ENET_MAC_TX_TIMESTAMP_STATUS_NSEC_TXTSSMIS          (1 << 31)   /* Bit 31: Transmit Timestamp Status Missed */

/* MAC Timestamp Ingress Correction In Subnanoseconds Register */

#define ENET_MAC_TSTAMP_INGRESS_CORR_SUBNSEC_TSICSNS_SHIFT  (8)         /* Bits 8-15: Timestamp Ingress Correction, sub-nanoseconds */
#define ENET_MAC_TSTAMP_INGRESS_CORR_SUBNSEC_TSICSNS_MASK   (0xff << ENET_MAC_TSTAMP_INGRESS_CORR_SUBNSEC_TSICSNS_SHIFT)
#define ENET_MAC_TSTAMP_INGRESS_CORR_SUBNSEC_TSICSNS(x)     (((uint32_t)(x) << ENET_MAC_TSTAMP_INGRESS_CORR_SUBNSEC_TSICSNS_SHIFT) & ENET_MAC_TSTAMP_INGRESS_CORR_SUBNSEC_TSICSNS_MASK)
                                                                        /* Bits 16-31: Reserved */

/* MAC Timestamp Engress Correction In Subnanoseconds Register */

#define ENET_MAC_TSTAMP_EGRESS_CORR_SUBNSEC_TSECSNS_SHIFT   (8)         /* Bits 8-15: Timestamp Egress Correction, sub-nanoseconds */
#define ENET_MAC_TSTAMP_EGRESS_CORR_SUBNSEC_TSECSNS_MASK    (0xff << ENET_MAC_TSTAMP_EGRESS_CORR_SUBNSEC_TSECSNS_SHIFT)
#define ENET_MAC_TSTAMP_EGRESS_CORR_SUBNSEC_TSECSNS(x)      (((uint32_t)(x) << ENET_MAC_TSTAMP_EGRESS_CORR_SUBNSEC_TSECSNS_SHIFT) & ENET_MAC_TSTAMP_EGRESS_CORR_SUBNSEC_TSECSNS_MASK)
                                                                        /* Bits 16-31: Reserved */

/* MAC Timestamp Ingress Latency Register */

#define ENET_MAC_TIMESTAMP_INGRESS_LATENCY_ITLSNS_SHIFT     (8)         /* Bits 8-15: Ingress Timestamp Latency, in sub-nanoseconds */
#define ENET_MAC_TIMESTAMP_INGRESS_LATENCY_ITLSNS_MASK      (0xff << ENET_MAC_TIMESTAMP_INGRESS_LATENCY_ITLSNS_SHIFT)
#define ENET_MAC_TIMESTAMP_INGRESS_LATENCY_ITLSNS(x)        (((uint32_t)(x) << ENET_MAC_TIMESTAMP_INGRESS_LATENCY_ITLSNS_SHIFT) & ENET_MAC_TIMESTAMP_INGRESS_LATENCY_ITLSNS_MASK)
#define ENET_MAC_TIMESTAMP_INGRESS_LATENCY_ITLNS_SHIFT      (16)         /* Bits 16-27: Ingress Timestamp Latency, in nanoseconds */
#define ENET_MAC_TIMESTAMP_INGRESS_LATENCY_ITLNS_MASK       (0xff << ENET_MAC_TIMESTAMP_INGRESS_LATENCY_ITLSNS_SHIFT)
#define ENET_MAC_TIMESTAMP_INGRESS_LATENCY_ITLNS(x)         (((uint32_t)(x) << ENET_MAC_TIMESTAMP_INGRESS_LATENCY_ITLSNS_SHIFT) & ENET_MAC_TIMESTAMP_INGRESS_LATENCY_ITLSNS_MASK)
                                                                        /* Bits 28-31: Reserved */

/* MAC Timestamp Egress Latecy Register */

#define ENET_MAC_TIMESTAMP_EGRESS_LATENCY_ETLSNS_SHIFT      (8)         /* Bits 8-15: Egress Timestamp Latency, in sub-nanoseconds */
#define ENET_MAC_TIMESTAMP_EGRESS_LATENCY_ETLSNS_MASK       (0xff << ENET_MAC_TIMESTAMP_EGRESS_LATENCY_ETLSNS_SHIFT)
#define ENET_MAC_TIMESTAMP_EGRESS_LATENCY_ETLSNS(x)         (((uint32_t)(x) << ENET_MAC_TIMESTAMP_EGRESS_LATENCY_ETLSNS_SHIFT) & ENET_MAC_TIMESTAMP_EGRESS_LATENCY_ETLSNS_MASK)
#define ENET_MAC_TIMESTAMP_EGRESS_LATENCY_ETLNS_SHIFT       (16)         /* Bits 16-27: Egress Timestamp Latency, in nanoseconds */
#define ENET_MAC_TIMESTAMP_EGRESS_LATENCY_ETLNS_MASK        (0xff << ENET_MAC_TIMESTAMP_EGRESS_LATENCY_ETLNS_SHIFT)
#define ENET_MAC_TIMESTAMP_EGRESS_LATENCY_ETLNS(x)          (((uint32_t)(x) << ENET_MAC_TIMESTAMP_EGRESS_LATENCY_ETLNS_SHIFT) & ENET_MAC_TIMESTAMP_EGRESS_LATENCY_ETLNS_MASK)
                                                                        /* Bits 28-31: Reserved */

/* MAC PPS Control Register */

#define ENET_MAC_PPS_CONTROL_PPSCTRL_PPSCMD0_SHIFT          (0)         /* Bits 0-3: PPS Output Frequency Control and Command */
#define ENET_MAC_PPS_CONTROL_PPSCTRL_PPSCMD0_MASK           (0xf << ENET_MAC_PPS_CONTROL_PPSCTRL_PPSCMD0_SHIFT)
#define ENET_MAC_PPS_CONTROL_PPSCTRL_PPSCMD0(x)             (((uint32_t)(x) << ENET_MAC_PPS_CONTROL_PPSCTRL_PPSCMD0SHIFT) & ENET_MAC_PPS_CONTROL_PPSCTRL_PPSCMD0_MASK)
#define ENET_MAC_PPS_CONTROL_PPSEN0                         (1 << 4)    /* Bit 4: Flexible PPS Output Mode Enable */
#define ENET_MAC_PPS_CONTROL_TRGTMODSEL0_SHIFT              (5)         /* Bits 5-6: Target Time Register Mode for PPS0 Output */
#define ENET_MAC_PPS_CONTROL_TRGTMODSEL0_MASK               (3 << ENET_MAC_PPS_CONTROL_TRGTMODSEL0_SHIFT)
#define ENET_MAC_PPS_CONTROL_TRGTMODSEL0(x)                 (((uint32_t)(x) << ENET_MAC_PPS_CONTROL_TRGTMODSEL0_SHIFT) & ENET_MAC_PPS_CONTROL_TRGTMODSEL0_MASK)
#define ENET_MAC_PPS_CONTROL_MCGREN0                        (1 << 7)    /* Bit 7: MCGR Mode Enable for PPS0 Output */
#define ENET_MAC_PPS_CONTROL_PPSCMD1_SHIFT                  (8)         /* Bits 8-11: Flexible PPS1 Output Control */
#define ENET_MAC_PPS_CONTROL_PPSCMD1_MASK                   (0xf << ENET_MAC_PPS_CONTROL_PPSCMD1_SHIFT)
#define ENET_MAC_PPS_CONTROL_PPSCMD1(x)                     (((uint32_t)(x) << ENET_MAC_PPS_CONTROL_PPSCMD1_SHIFT) & ENET_MAC_PPS_CONTROL_PPSCMD1_MASK)
                                                                        /* Bit 12: Reserved */
#define ENET_MAC_PPS_CONTROL_TRGTMODSEL1_SHIFT              (13)        /* Bits 13-14: Target Time Register Mode for PPS1 Output */
#define ENET_MAC_PPS_CONTROL_TRGTMODSEL1_MASK               (3 << ENET_MAC_PPS_CONTROL_TRGTMODSEL1_SHIFT)
#define ENET_MAC_PPS_CONTROL_TRGTMODSEL1(x)                 (((uint32_t)(x) << ENET_MAC_PPS_CONTROL_TRGTMODSEL1_SHIFT) & ENET_MAC_PPS_CONTROL_TRGTMODSEL1_MASK)
#define ENET_MAC_PPS_CONTROL_MCGREN1                        (1 << 15)   /* Bit 15: MCGR Mode Enable for PPS1 Output */
#define ENET_MAC_PPS_CONTROL_PPSCMD2_SHIFT                  (16)        /* Bits 16-19: Flexible PPS2 Output Control */
#define ENET_MAC_PPS_CONTROL_PPSCMD2_MASK                   (0xf << ENET_MAC_PPS_CONTROL_PPSCMD2_SHIFT)
#define ENET_MAC_PPS_CONTROL_PPSCMD2(x)                     (((uint32_t)(x) << ENET_MAC_PPS_CONTROL_PPSCMD2_SHIFT) & ENET_MAC_PPS_CONTROL_PPSCMD2_MASK)
                                                                        /* Bit 20: Reserved */
#define ENET_MAC_PPS_CONTROL_TRGTMODSEL2_SHIFT              (21)        /* Bits 21-22: Target Time Register Mode for PPS2 Output */
#define ENET_MAC_PPS_CONTROL_TRGTMODSEL2_MASK               (3 << ENET_MAC_PPS_CONTROL_TRGTMODSEL2_SHIFT)
#define ENET_MAC_PPS_CONTROL_TRGTMODSEL2(x)                 (((uint32_t)(x) << ENET_MAC_PPS_CONTROL_TRGTMODSEL2_SHIFT) & ENET_MAC_PPS_CONTROL_TRGTMODSEL2_MASK)
#define ENET_MAC_PPS_CONTROL_MCGREN2                        (1 << 23)   /* Bit 23: MCGR Mode Enable for PPS2 Output */
#define ENET_MAC_PPS_CONTROL_PPSCMD3_SHIFT                  (24)        /* Bits 24-27: Flexible PPS3 Output Control */
#define ENET_MAC_PPS_CONTROL_PPSCMD3_MASK                   (0xf << ENET_MAC_PPS_CONTROL_PPSCMD3_SHIFT)
#define ENET_MAC_PPS_CONTROL_PPSCMD3(x)                     (((uint32_t)(x) << ENET_MAC_PPS_CONTROL_PPSCMD3_SHIFT) & ENET_MAC_PPS_CONTROL_PPSCMD3_MASK)
#define ENET_MAC_PPS_CONTROL_TIMESEL                        (1 << 28)   /* Bit 28: Time Select */
#define ENET_MAC_PPS_CONTROL_TRGTMODSEL3_SHIFT              (29)        /* Bits 29-30: Target Time Register Mode for PPS3 Output */
#define ENET_MAC_PPS_CONTROL_TRGTMODSEL3_MASK               (3 << ENET_MAC_PPS_CONTROL_TRGTMODSEL3_SHIFT)
#define ENET_MAC_PPS_CONTROL_TRGTMODSEL3(x)                 (((uint32_t)(x) << ENET_MAC_PPS_CONTROL_TRGTMODSEL3_SHIFT) & ENET_MAC_PPS_CONTROL_TRGTMODSEL3_MASK)
#define ENET_MAC_PPS_CONTROL_MCGREN3                        (1 << 31)   /* Bit 31: MCGR Mode Enable for PPS3 Output */

/* MAC PPS Target Time In Seconds Register (32-bit address) */

/* MAC PPS Target Time In Nanoseconds Register */

#define ENET_MAC_PPS_TARGET_TIME_NSEC_TTSL_SHIFT            (0)         /* Bits 0-30: Target Time Low for PPS Register */
#define ENET_MAC_PPS_TARGET_TIME_NSEC_TTSL_MASK             (0x7fffffff << ENET_MAC_PPS_TARGET_TIME_NSEC_TTSL_SHIFT)
#define ENET_MAC_PPS_TARGET_TIME_NSEC_TTSL(x)               (((uint32_t)(x) << ENET_MAC_PPS_TARGET_TIME_NSEC_TTSL_SHIFT) & ENET_MAC_PPS_TARGET_TIME_NSEC_TTSL_MASK)
#define ENET_MAC_PPS_TARGET_TIME_NSEC_TRGTBUSY              (1 << 31)   /* Bit 31: PPS Target Time Register Busy */

/* MTL Operation Mode Register */

#define ENET_MTL_OPERATION_MODE_DTXSTS                      (1 << 1)    /* Bit 1: Drop Transmit Status */
#define ENET_MTL_OPERATION_MODE_RAA                         (1 << 2)    /* Bit 2: Receive Arbitration Algorithm */
                                                                        /* Bits 3-4: Reserved */
#define ENET_MTL_OPERATION_MODE_SCHALG_SHIFT                (5)         /* Bits 5-6: CSR Clock Range */
#define ENET_MTL_OPERATION_MODE_SCHALG_MASK                 (3 << ENET_MTL_OPERATION_MODE_SCHALG_SHIFT)
#define ENET_MTL_OPERATION_MODE_SCHALG_WRR                  (0x0 << ENET_MTL_OPERATION_MODE_SCHALG_SHIFT) /* WRR algorithm */
#define ENET_MTL_OPERATION_MODE_SCHALG_WFQ                  (0x1 << ENET_MTL_OPERATION_MODE_SCHALG_SHIFT) /* WFQ algorithm when DCB feature is selected.Otherwise, Reserved */
#define ENET_MTL_OPERATION_MODE_SCHALG_DWRR                 (0x2 << ENET_MTL_OPERATION_MODE_SCHALG_SHIFT) /* DWRR algorithm when DCB feature is selected.Otherwise, Reserved */
#define ENET_MTL_OPERATION_MODE_SCHALG_SP                   (0x3 << ENET_MTL_OPERATION_MODE_SCHALG_SHIFT) /* Strict priority algorithm */
                                                                                                          /* Bit 7: Reserved */
#define ENET_MTL_OPERATION_MODE_CNTPRST                     (1 << 8)                                      /* Bit 8: Counters Preset */
#define ENET_MTL_OPERATION_MODE_CNTCLR                      (1 << 9)                                      /* Bit 9: Counters Reset */
                                                                                                          /* Bits 10-14: Reserved */
#define ENET_MTL_OPERATION_MODE_FRPE                        (1 << 15)                                     /* Bit 15: Flexible Rx parser Enable */
                                                                                                          /* Bits 16-31: Reserved */

/* MTL Debug Control Register */

#define ENET_MTL_DBG_CTL_FDBGEN                             (1 << 0)    /* Bit 0: FIFO Debug Access Enable */
#define ENET_MTL_DBG_CTL_DBGMOD                             (1 << 1)    /* Bit 1: Debug Mode Access to FIFO */
#define ENET_MTL_DBG_CTL_BYTEEN_SHIFT                       (2)         /* Bits 2-3: Byte Enables */
#define ENET_MTL_DBG_CTL_BYTEEN_MASK                        (3 << ENET_MTL_DBG_CTL_BYTEEN_SHIFT)
#define ENET_MTL_DBG_CTL_BYTEEN_B0_VAL                      (0x0 << ENET_MTL_DBG_CTL_BYTEEN_SHIFT) /* Byte 0 valid */
#define ENET_MTL_DBG_CTL_BYTEEN_B01_VAL                     (0x1 << ENET_MTL_DBG_CTL_BYTEEN_SHIFT) /* Byte 0 and Byte 1 are valid */
#define ENET_MTL_DBG_CTL_BYTEEN_B012_VAL                    (0x2 << ENET_MTL_DBG_CTL_BYTEEN_SHIFT) /* Byte 0, Byte 1, and Byte 2 are valid */
#define ENET_MTL_DBG_CTL_BYTEEN_B0123_VAL                   (0x3 << ENET_MTL_DBG_CTL_BYTEEN_SHIFT) /* All four bytes are valid */
                                                                                                   /* Bit 4: Reserved */
#define ENET_MTL_DBG_CTL_PKTSTATE_SHIFT                     (5)                                    /* Bits 5-6: Encoded Packet State */
#define ENET_MTL_DBG_CTL_PKTSTATE_MASK                      (3 << ENET_MTL_DBG_CTL_PKTSTATE_SHIFT)
#define ENET_MTL_DBG_CTL_PKTSTATE_PKT_DATA                  (0x0 << ENET_MTL_DBG_CTL_PKTSTATE_SHIFT) /* Packet Data */
#define ENET_MTL_DBG_CTL_PKTSTATE_CW_NS                     (0x1 << ENET_MTL_DBG_CTL_PKTSTATE_SHIFT) /* Control Word/Normal Status */
#define ENET_MTL_DBG_CTL_PKTSTATE_SOP_LS                    (0x2 << ENET_MTL_DBG_CTL_PKTSTATE_SHIFT) /* SOP Data/Last Status */
#define ENET_MTL_DBG_CTL_PKTSTATE_EOP                       (0x3 << ENET_MTL_DBG_CTL_PKTSTATE_SHIFT) /* EOP Data/EOP */
                                                                                                     /* Bit 7: Reserved */
#define ENET_MTL_DBG_CTL_RSTALL                             (1 << 8)                                 /* Bit 8: Reset All Pointers */
#define ENET_MTL_DBG_CTL_RSTSEL                             (1 << 9)                                 /* Bit 9: Reset Pointers of Selected FIFO */
#define ENET_MTL_DBG_CTL_FIFORDEN                           (1 << 10)                                /* Bit 10: FIFO Read Enable */
#define ENET_MTL_DBG_CTL_FIFOWREN                           (1 << 11)                                /* Bit 11: FIFO Write Enable */
#define ENET_MTL_DBG_CTL_FIFOSEL_SHIFT                      (12)                                     /* Bits 12-13: FIFO Selected for Access */
#define ENET_MTL_DBG_CTL_FIFOSEL_MASK                       (3 << ENET_MTL_DBG_CTL_FIFOSEL_SHIFT)
#define ENET_MTL_DBG_CTL_FIFOSEL_TXFIFO                     (0x0 << ENET_MTL_DBG_CTL_FIFOSEL_SHIFT) /* Tx FIFO */
#define ENET_MTL_DBG_CTL_FIFOSEL_TXSTSFIFO                  (0x1 << ENET_MTL_DBG_CTL_FIFOSEL_SHIFT) /* Tx Status FIFO */
#define ENET_MTL_DBG_CTL_FIFOSEL_TSOFIFO                    (0x2 << ENET_MTL_DBG_CTL_FIFOSEL_SHIFT) /* TSO FIFO */
#define ENET_MTL_DBG_CTL_FIFOSEL_RXFIFO                     (0x3 << ENET_MTL_DBG_CTL_FIFOSEL_SHIFT) /* Rx FIFO */
#define ENET_MTL_DBG_CTL_PKTIE                              (1 << 14)                               /* Bit 14: Receive Packet Available Interrupt Status Enable */
#define ENET_MTL_DBG_CTL_STSIE                              (1 << 15)                               /* Bit 15: Transmit Status Available Interrupt Status Enable */
#define ENET_MTL_DBG_CTL_EIEE                               (1 << 16)                               /* Bit 16: ECC Inject Error Enable for Tx, Rx, TSO and DCACHE memories */
#define ENET_MTL_DBG_CTL_EIAEE                              (1 << 17)                               /* Bit 17: ECC Inject Address Error for Tx, Rx, TSO and DCACHE memories */
#define ENET_MTL_DBG_CTL_EIEC                               (1 << 18)                               /* Bit 18: ECC Inject Error Control for Tx, Rx, TSO and DCACHE memories */
                                                                                                    /* Bits 19-31: Reserved */

/* MTL Debug Status Register */

#define ENET_MTL_DBG_STS_FIFOBUSY                           (1 << 0)    /* Bit 0: FIFO Busy */
#define ENET_MTL_DBG_STS_PKTSTATE_SHIFT                     (1)         /* Bits 1-2: Encoded Packet State */
#define ENET_MTL_DBG_STS_PKTSTATE_MASK                      (3 << ENET_MTL_DBG_STS_PKTSTATE_SHIFT)
#define ENET_MTL_DBG_STS_PKTSTATE_PKT_DATA                  (0x0 << ENET_MTL_DBG_STS_PKTSTATE_SHIFT) /* Packet Data */
#define ENET_MTL_DBG_STS_PKTSTATE_CW_NS                     (0x1 << ENET_MTL_DBG_STS_PKTSTATE_SHIFT) /* Control Word/Normal Status */
#define ENET_MTL_DBG_STS_PKTSTATE_SOP_LS                    (0x2 << ENET_MTL_DBG_STS_PKTSTATE_SHIFT) /* SOP Data/Last Status */
#define ENET_MTL_DBG_STS_PKTSTATE_EOP                       (0x3 << ENET_MTL_DBG_STS_PKTSTATE_SHIFT) /* EOP Data/EOP */
#define ENET_MTL_DBG_STS_BYTEEN_SHIFT                       (3)                                      /* Bits 3-4: Byte Enables */
#define ENET_MTL_DBG_STS_BYTEEN_MASK                        (3 << ENET_MTL_DBG_STS_BYTEEN_SHIFT)
#define ENET_MTL_DBG_STS_BYTEEN_B0_VAL                      (0x0 << ENET_MTL_DBG_STS_BYTEEN_SHIFT) /* Byte 0 valid */
#define ENET_MTL_DBG_STS_BYTEEN_B01_VAL                     (0x1 << ENET_MTL_DBG_STS_BYTEEN_SHIFT) /* Byte 0 and Byte 1 are valid */
#define ENET_MTL_DBG_STS_BYTEEN_B012_VAL                    (0x2 << ENET_MTL_DBG_STS_BYTEEN_SHIFT) /* Byte 0, Byte 1, and Byte 2 are valid */
#define ENET_MTL_DBG_STS_BYTEEN_B0123_VAL                   (0x3 << ENET_MTL_DBG_STS_BYTEEN_SHIFT) /* All four bytes are valid */
                                                                                                   /* Bits 6-7: Reserved */
#define ENET_MTL_DBG_STS_PKTI                               (1 << 8)                               /* Bit 8: Receive Packet Available Interrupt Status */
#define ENET_MTL_DBG_STS_STSI                               (1 << 9)                               /* Bit 9: Transmit Status Available Interrupt Status */
                                                                                                   /* Bits 10-14: Reserved */
#define ENET_MTL_DBG_STS_LOCR_SHIFT                         (15)                                   /* Bits 15-31: Remaining Locations in the FIFO */
#define ENET_MTL_DBG_STS_LOCR_MASK                          (0xffff << ENET_MTL_DBG_STS_LOCR_SHIFT)
#define ENET_MTL_DBG_STS_LOCR(x)                            (((uint32_t)(x) << ENET_MTL_DBG_STS_LOCR_SHIFT) & ENET_MTL_DBG_STS_LOCR_MASK)

/* MTL FIFO Debug Data Register (32-bit address) */

/* MTL Interrupt Status Register */

#define ENET_MTL_INTERRUPT_STATUS_QIS(q)                    (1 << (q))  /* Bit 0-1: Queue Interrupt status */
                                                                        /* Bits 2-15: Reserved */
#define ENET_MTL_INTERRUPT_STATUS_MACIS                     (1 << 16)   /* Bit 16: MAC Interrupt Status */
#define ENET_MTL_INTERRUPT_STATUS_DBGIS                     (1 << 17)   /* Bit 17: Debug Interrupt status */
#define ENET_MTL_INTERRUPT_STATUS_ESTIS                     (1 << 18)   /* Bit 18: EST (TAS- 802.1Qbv) Interrupt Status */
                                                                        /* Bits 19-22: Reserved */
#define ENET_MTL_INTERRUPT_STATUS_MTLPIS                    (1 << 23)   /* Bit 23: MTL Rx Parser Interrupt Status */
                                                                        /* Bits 24-31: Reserved */

/* MTL Receive Queue DMA Map 0 Register */

#define ENET_MTL_RXQ_DMA_MAP0_QMDMACH_SHIFT(q)              ((q) * 8)       /* Bit 0+q*8: Queue Mapped to DMA Channel */
#define ENET_MTL_RXQ_DMA_MAP0_QMDMACH_MASK(q)               (0x1 << ENET_MTL_RXQ_DMA_MAP0_QMDMACH_SHIFT(q))
#define ENET_MTL_RXQ_DMA_MAP0_QMDMACH(q,x)                  (((uint32_t)(x) << ENET_MTL_RXQ_DMA_MAP0_QMDMACH_SHIFT(q)) & ENET_MTL_RXQ_DMA_MAP0_QMDMACH_MASK(q))
                                                                            /* Bits 1-3: Reserved */
#define ENET_MTL_RXQ_DMA_MAP0_QDDMACH_SHIFT(q)              (4 + (q) * 8)   /* Bit 4+q*8: Queue Enabled for DA-based DMA Channel Selection */
#define ENET_MTL_RXQ_DMA_MAP0_QDDMACH_MASK(q)               (0x1 << ENET_MTL_RXQ_DMA_MAP0_QDDMACH_SHIFT(q))
#define ENET_MTL_RXQ_DMA_MAP0_QDDMACH(q,x)                  (((uint32_t)(x) << ENET_MTL_RXQ_DMA_MAP0_QDDMACH_SHIFT(q)) & ENET_MTL_RXQ_DMA_MAP0_QDDMACH_MASK(q))
                                                                            /* Bits 5-7: Reserved */
                                                                            /* Bits 9-11: Reserved */
                                                                            /* Bits 13-31: Reserved */

/* MTL TBS Control Register */

#define ENET_MTL_TBS_CTRL_ESTM                              (1 << 0)    /* Bit 0: EST offset Mode */
#define ENET_MTL_TBS_CTRL_LEOV                              (1 << 1)    /* Bit 1: Launch Expiry Offset Valid */
                                                                        /* Bits 2-3: Reserved */
#define ENET_MTL_TBS_CTRL_LEGOS_SHIFT                       (4)         /* Bits 4-6: Launch Expiry GSN Offset */
#define ENET_MTL_TBS_CTRL_LEGOS_MASK                        (7 << ENET_MTL_TBS_CTRL_LEGOS_SHIFT)
#define ENET_MTL_TBS_CTRL_LEGOS(x)                          (((uint32_t)(x) << ENET_MTL_TBS_CTRL_LEGOS_SHIFT) & ENET_MTL_TBS_CTRL_LEGOS_MASK)
                                                                        /* Bit 7: Reserved */
#define ENET_MTL_TBS_CTRL_LEOS_SHIFT                        (8)         /* Bits 8-31: Launch Expiry GSN Offset */
#define ENET_MTL_TBS_CTRL_LEOS_MASK                         (0xffffff << ENET_MTL_TBS_CTRL_LEOS_SHIFT)
#define ENET_MTL_TBS_CTRL_LEOS(x)                           (((uint32_t)(x) << ENET_MTL_TBS_CTRL_LEOS_SHIFT) & ENET_MTL_TBS_CTRL_LEOS_MASK)

/* MTL EST Control Register */

#define ENET_MTL_EST_CONTROL_EEST                           (1 << 0)    /* Bit 0: Enable EST */
#define ENET_MTL_EST_CONTROL_SSWL                           (1 << 1)    /* Bit 1: Switch to S/W owned list */
                                                                        /* Bits 2-3: Reserved */
#define ENET_MTL_EST_CONTROL_DDBF                           (1 << 4)    /* Bit 4: Do not Drop frames during Frame Size Error */
#define ENET_MTL_EST_CONTROL_DFBS                           (1 << 5)    /* Bit 5: Drop Frames causing Scheduling Error */
#define ENET_MTL_EST_CONTROL_LCSE_SHIFT                     (6)         /* Bits 6-7: Loop Count to report Scheduling Error */
#define ENET_MTL_EST_CONTROL_LCSE_MASK                      (3 << ENET_MTL_EST_CONTROL_LCSE_SHIFT)
#define ENET_MTL_EST_CONTROL_LCSE_M_4_ITERNS                (0x0 << ENET_MTL_EST_CONTROL_LCSE_SHIFT) /* 4 iterations */
#define ENET_MTL_EST_CONTROL_LCSE_M_8_ITERNS                (0x1 << ENET_MTL_EST_CONTROL_LCSE_SHIFT) /* 8 iterations */
#define ENET_MTL_EST_CONTROL_LCSE_M_16_ITERNS               (0x2 << ENET_MTL_EST_CONTROL_LCSE_SHIFT) /* 16 iterations */
#define ENET_MTL_EST_CONTROL_LCSE_M_32_ITERNS               (0x3 << ENET_MTL_EST_CONTROL_LCSE_SHIFT) /* 32 iterations */
#define ENET_MTL_EST_CONTROL_TILS_SHIFT                     (8)                                      /* Bits 8-10: Time Interval Left Shift Amount */
#define ENET_MTL_EST_CONTROL_TILS_MASK                      (7 << ENET_MTL_EST_CONTROL_TILS_SHIFT)
#define ENET_MTL_EST_CONTROL_TILS(x)                        (((uint32_t)(x) << ENET_MTL_EST_CONTROL_TILS_SHIFT) & ENET_MTL_EST_CONTROL_TILS_MASK)
                                                                        /* Bit 11: Reserved */
#define ENET_MTL_EST_CONTROL_CTOV_SHIFT                     (12)        /* Bits 12-23: Time Interval Left Shift Amount */
#define ENET_MTL_EST_CONTROL_CTOV_MASK                      (0xfff << ENET_MTL_EST_CONTROL_CTOV_SHIFT)
#define ENET_MTL_EST_CONTROL_CTOV(x)                        (((uint32_t)(x) << ENET_MTL_EST_CONTROL_CTOV_SHIFT) & ENET_MTL_EST_CONTROL_CTOV_MASK)
#define ENET_MTL_EST_CONTROL_PTOV_SHIFT                     (24)        /* Bits 24-31: Time Interval Left Shift Amount */
#define ENET_MTL_EST_CONTROL_PTOV_MASK                      (0xff << ENET_MTL_EST_CONTROL_PTOV_SHIFT)
#define ENET_MTL_EST_CONTROL_PTOV(x)                        (((uint32_t)(x) << ENET_MTL_EST_CONTROL_PTOV_SHIFT) & ENET_MTL_EST_CONTROL_PTOV_MASK)

/* MTL EST extended Control Register */

#define ENET_MTL_EST_EXT_CONTROL_OVHD_SHIFT                 (0)        /* Bits 0-5: Overhead Bytes Value */
#define ENET_MTL_EST_EXT_CONTROL_OVHD_MASK                  (0x3f << ENET_MTL_EST_EXT_CONTROL_OVHD_SHIFT)
#define ENET_MTL_EST_EXT_CONTROL_OVHD(x)                    (((uint32_t)(x) << ENET_MTL_EST_EXT_CONTROL_OVHD_SHIFT) & ENET_MTL_EST_EXT_CONTROL_OVHD_MASK)
                                                                        /* Bits 6-31: Reserved */

/* MTL EST Status Register */

#define ENET_MTL_EST_STATUS_SWLC                            (1 << 0)    /* Bit 0: Switch to S/W owned list Complete */
#define ENET_MTL_EST_STATUS_BTRE                            (1 << 1)    /* Bit 1: BTR Error */
#define ENET_MTL_EST_STATUS_HLBF                            (1 << 2)    /* Bit 2: Head-Of-Line Blocking due to Frame Size */
#define ENET_MTL_EST_STATUS_HLBS                            (1 << 3)    /* Bit 3: Head-Of-Line Blocking due to Scheduling */
#define ENET_MTL_EST_STATUS_CGCE                            (1 << 4)    /* Bit 4: Constant Gate Control Error */
                                                                        /* Bits 5-6: Reserved */
#define ENET_MTL_EST_STATUS_SWOL                            (1 << 7)    /* Bit 7: S/W owned list */
#define ENET_MTL_EST_STATUS_BTRL_SHIFT                      (8)         /* Bits 8-15: BTR Error Loop Count */
#define ENET_MTL_EST_STATUS_BTRL_MASK                       (0xff << ENET_MTL_EST_STATUS_BTRL_SHIFT)
#define ENET_MTL_EST_STATUS_CGSN_SHIFT                      (16)        /* Bits 16-19: Current GCL Slot Number */
#define ENET_MTL_EST_STATUS_CGSN_MASK                       (0xf << ENET_MTL_EST_STATUS_CGSN_SHIFT)
                                                                        /* Bits 20-31: Reserved */

/* MTL EST Scheduling Error Register */

#define ENET_MTL_EST_SCH_ERROR_SEQN_SHIFT                   (0)         /* Bits 0-1: Schedule Error Queue Number */
#define ENET_MTL_EST_SCH_ERROR_SEQN_MASK                    (0x3 << ENET_MTL_EST_SCH_ERROR_SEQN_SHIFT)
#define ENET_MTL_EST_SCH_ERROR_SEQN(x)                      (((uint32_t)(x) << ENET_MTL_EST_SCH_ERROR_SEQN_SHIFT) & ENET_MTL_EST_SCH_ERROR_SEQN_MASK)
                                                                        /* Bits 2-31: Reserved */

/* MTL EST Frame Size Error Register */

#define ENET_MTL_EST_FRM_SIZE_ERROR_FEQN_SHIFT              (0)         /* Bits 0-1: Frame Size Error Queue Number */
#define ENET_MTL_EST_FRM_SIZE_ERROR_FEQN_MASK               (0x3 << ENET_MTL_EST_FRM_SIZE_ERROR_FEQN_SHIFT)
#define ENET_MTL_EST_FRM_SIZE_ERROR_FEQN(x)                 (((uint32_t)(x) << ENET_MTL_EST_FRM_SIZE_ERROR_FEQN_SHIFT) & ENET_MTL_EST_FRM_SIZE_ERROR_FEQN_MASK)
                                                                        /* Bits 2-31: Reserved */

/* MTL EST Frame Size Capture Register */

#define ENET_MTL_EST_FRM_SIZE_CAPTURE_HBFS_SHIFT            (0)         /* Bits 0-14: Frame Size of HLBF */
#define ENET_MTL_EST_FRM_SIZE_CAPTURE_HBFS_MASK             (0x7fff << ENET_MTL_EST_FRM_SIZE_CAPTURE_HBFS_SHIFT)
                                                                        /* Bit 15: Reserved */
#define ENET_MTL_EST_FRM_SIZE_CAPTURE_HBFQ_SHIFT            (16)        /* Bits 16-16: Queue Number of HLBF */
#define ENET_MTL_EST_FRM_SIZE_CAPTURE_HBFQ_MASK             (1 << ENET_MTL_EST_FRM_SIZE_CAPTURE_HBFQ_SHIFT)
                                                                        /* Bits 17-31: Reserved */

/* MTL EST Interrupt Enable Register */

#define ENET_MTL_EST_INTR_ENABLE_IECC                       (1 << 0)    /* Bit 0: Interrupt Enable for Switch List */
#define ENET_MTL_EST_INTR_ENABLE_IEBE                       (1 << 1)    /* Bit 1: Interrupt Enable for BTR Error */
#define ENET_MTL_EST_INTR_ENABLE_IEHF                       (1 << 2)    /* Bit 2: Interrupt Enable for HLBF */
#define ENET_MTL_EST_INTR_ENABLE_IEHS                       (1 << 3)    /* Bit 3: Interrupt Enable for HLBS */
#define ENET_MTL_EST_INTR_ENABLE_CGCE                       (1 << 4)    /* Bit 4: Interrupt Enable for CGCE */
                                                                        /* Bits 5-31: Reserved */

/* MTL EST GCL Control Register */

#define ENET_MTL_EST_GCL_CONTROL_SRWO                       (1 << 0)    /* Bit 0: Start Read/Write Op */
#define ENET_MTL_EST_GCL_CONTROL_R1W0                       (1 << 1)    /* Bit 1: Read '1', Write '0' */
#define ENET_MTL_EST_GCL_CONTROL_GCRR_SHIFT                 (2)         /* Bit 2: Gate Control Related Registers */
#define ENET_MTL_EST_GCL_CONTROL_GCRR_MASK                  (1 << ENET_MTL_EST_GCL_CONTROL_GCRR_SHIFT)
#define ENET_MTL_EST_GCL_CONTROL_GCRR(x)                    (((uint32_t)(x) << ENET_MTL_EST_GCL_CONTROL_GCRR_SHIFT) & ENET_MTL_EST_GCL_CONTROL_GCRR_MASK)
                                                                        /* Bit 3: Reserved */
#define ENET_MTL_EST_GCL_CONTROL_DBGM                       (1 << 4)    /* Bit 4: Debug Mode */
#define ENET_MTL_EST_GCL_CONTROL_DBGB                       (1 << 5)    /* Bit 5: Debug Mode Bank Select */
                                                                        /* Bits 6-7: Reserved */
#define ENET_MTL_EST_GCL_CONTROL_ADDR_SHIFT                 (8)         /* Bits 8-15: Gate Control List Address or Related Registers Address */
#define ENET_MTL_EST_GCL_CONTROL_ADDR_MASK                  (0xff << ENET_MTL_EST_GCL_CONTROL_ADDR_SHIFT)
#define ENET_MTL_EST_GCL_CONTROL_ADDR(x)                    (((uint32_t)(x) << ENET_MTL_EST_GCL_CONTROL_ADDR_SHIFT) & ENET_MTL_EST_GCL_CONTROL_ADDR_MASK)
                                                                        /* Bits 16-20: Reserved */
#define ENET_MTL_EST_GCL_CONTROL_ESTEIEE                    (1 << 21)   /* Bit 21: EST ECC Inject Error Enable */
#define ENET_MTL_EST_GCL_CONTROL_ESTEIAEE                   (1 << 22)   /* Bit 22: EST ECC Inject Address Error Enable */
#define ENET_MTL_EST_GCL_CONTROL_ESTEIEC                    (1 << 23)   /* Bit 23: ECC Inject Error Control for EST Memory */
                                                                        /* Bits 24-31: Reserved */
#define ENET_MTL_EST_GCL_CONTROL_R1W0_SHIFT                 (1)
#define ENET_MTL_EST_GCL_CONTROL_R1W0_MASK                  (1 << ENET_MTL_EST_GCL_CONTROL_R1W0_SHIFT)
#define ENET_MTL_EST_GCL_CONTROL_SRWO_SHIFT                 (0)
#define ENET_MTL_EST_GCL_CONTROL_SRWO_MASK                  (1 << ENET_MTL_EST_GCL_CONTROL_SRWO_SHIFT)

/* MTL EST GCL Data Register (32-bit address) */

#define ENET_EST_INTERVAL_WIDTH                              (24U)         /* ENET GCL interval time width */
#define ENET_EST_INTERVAL_MASK                               (0xFFFFFFU)   /* ENET GCL interval time mask */
#define ENET_EST_GATE_MASK                                   (0x3U)        /* ENET GCL gate mask */

/* MTL FPE Control Status Register */

#define ENET_MTL_FPE_CTRL_STS_AFSZ_SHIFT                    (0)         /* Bits 0-1: Additional Fragment Size */
#define ENET_MTL_FPE_CTRL_STS_AFSZ_MASK                     (3 << ENET_MTL_FPE_CTRL_STS_AFSZ_SHIFT)
#define ENET_MTL_FPE_CTRL_STS_AFSZ_64BYTES                  (0x0 << ENET_MTL_FPE_CTRL_STS_AFSZ_SHIFT) /* 64 bytes */
#define ENET_MTL_FPE_CTRL_STS_AFSZ_128BYTES                 (0x1 << ENET_MTL_FPE_CTRL_STS_AFSZ_SHIFT) /* 128 bytes */
#define ENET_MTL_FPE_CTRL_STS_AFSZ_192BYTES                 (0x2 << ENET_MTL_FPE_CTRL_STS_AFSZ_SHIFT) /* 192 bytes */
#define ENET_MTL_FPE_CTRL_STS_AFSZ_256BYTES                 (0x3 << ENET_MTL_FPE_CTRL_STS_AFSZ_SHIFT) /* 256 bytes */
                                                                                                      /* Bits 2-7: Reserved */
#define ENET_MTL_FPE_CTRL_STS_PEC_SHIFT                     (8)                                       /* Bits 8-9: Preemption Classification */
#define ENET_MTL_FPE_CTRL_STS_PEC_MASK                      (3 << ENET_MTL_FPE_CTRL_STS_PEC_SHIFT)
#define ENET_MTL_FPE_CTRL_STS_PEC(x)                        (((uint32_t)(x) << ENET_MTL_FPE_CTRL_STS_PEC_SHIFT) & ENET_MTL_FPE_CTRL_STS_PEC_MASK)
                                                                        /* Bits 10-27: Reserved */
#define ENET_MTL_FPE_CTRL_STS_HRS                           (1 << 28)   /* Bit 28: Hold/Release Status */
                                                                        /* Bits 29-31: Reserved */

/* MTL FPE Advance Register */

#define ENET_MTL_FPE_ADVANCE_HADV_SHIFT                     (0)         /* Bits 0-15: Hold Advance */
#define ENET_MTL_FPE_ADVANCE_HADV_MASK                      (0xffff << ENET_MTL_FPE_ADVANCE_HADV_SHIFT)
#define ENET_MTL_FPE_ADVANCE_HADV(x)                        (((uint32_t)(x) << ENET_MTL_FPE_ADVANCE_HADV_SHIFT) & ENET_MTL_FPE_ADVANCE_HADV_MASK)
#define ENET_MTL_FPE_ADVANCE_RADV_SHIFT                     (16)         /* Bits 16-31: Release Advance */
#define ENET_MTL_FPE_ADVANCE_RADV_MASK                      (0xffff << ENET_MTL_FPE_ADVANCE_RADV_SHIFT)
#define ENET_MTL_FPE_ADVANCE_RADV(x)                        (((uint32_t)(x) << ENET_MTL_FPE_ADVANCE_RADV_SHIFT) & ENET_MTL_FPE_ADVANCE_RADV_MASK)

/* MTL ECC Control Register */

#define ENET_MTL_ECC_CONTROL_MTXEE                          (1 << 0)    /* Bit 0: MTL Tx FIFO ECC Enable */
#define ENET_MTL_ECC_CONTROL_MRXEE                          (1 << 1)    /* Bit 1: MTL Rx FIFO ECC Enable */
#define ENET_MTL_ECC_CONTROL_MESTEE                         (1 << 2)    /* Bit 2: MTL EST ECC Enable */
#define ENET_MTL_ECC_CONTROL_MRXPEE                         (1 << 3)    /* Bit 3: MTL Rx Parser ECC Enable */
                                                                        /* Bits 4-7: Reserved */
#define ENET_MTL_ECC_CONTROL_MEEAO                          (1 << 8)    /* Bit 8: MTL ECC Error Address Status Over-ride */
                                                                        /* Bits 9-31: Reserved */

/* MTL Safety Interript Status Register */

#define ENET_MTL_SAFETY_INT_STATUS_MECIS                    (1 << 0)    /* Bit 0: MTL ECC Correctable error Interrupt Status */
#define ENET_MTL_SAFETY_INT_STATUS_MEUIS                    (1 << 1)    /* Bit 1: MTL ECC Uncorrectable error Interrupt Status */
                                                                        /* Bits 2-31: Reserved */

/* MTL ECC Interrupt Enable Register */

#define ENET_MTL_ECC_INT_ENABLE_TXCEIE                      (1 << 0)    /* Bit 0: Tx memory Correctable Error Interrupt Enable */
                                                                        /* Bits 1-3: Reserved */
#define ENET_MTL_ECC_INT_ENABLE_RXCEIE                      (1 << 4)    /* Bit 4: Rx memory Correctable Error Interrupt Enable */
                                                                        /* Bits 5-7: Reserved */
#define ENET_MTL_ECC_INT_ENABLE_ECEIE                       (1 << 8)    /* Bit 8: EST memory Correctable Error Interrupt Enable */
                                                                        /* Bits 9-11: Reserved */
#define ENET_MTL_ECC_INT_ENABLE_RPCEIE                      (1 << 12)   /* Bit 12: Rx Parser memory Correctable Error Interrupt Enable */
                                                                        /* Bits 13-31: Reserved */

/* MTL ECC Interrupt Status Register */

#define ENET_MTL_ECC_INT_STATUS_TXCES                       (1 << 0)    /* Bit 0: MTL Tx memory Correctable Error Status */
#define ENET_MTL_ECC_INT_STATUS_TXAMS                       (1 << 1)    /* Bit 1: MTL Tx memory Address Mismatch Status */
#define ENET_MTL_ECC_INT_STATUS_TXUES                       (1 << 2)    /* Bit 2: MTL Tx memory Uncorrectable Error Status */
                                                                        /* Bit 3: Reserved */
#define ENET_MTL_ECC_INT_STATUS_RXCES                       (1 << 4)    /* Bit 4: MTL Rx memory Correctable Error Status */
#define ENET_MTL_ECC_INT_STATUS_RXAMS                       (1 << 5)    /* Bit 5: MTL Rx memory Address Mismatch Status */
#define ENET_MTL_ECC_INT_STATUS_RXUES                       (1 << 6)    /* Bit 6: MTL Rx memory Uncorrectable Error Status */
                                                                        /* Bit 7: Reserved */
#define ENET_MTL_ECC_INT_STATUS_ECES                        (1 << 8)    /* Bit 8: MTL EST memory Correctable Error Status */
#define ENET_MTL_ECC_INT_STATUS_EAMS                        (1 << 9)    /* Bit 9: MTL EST memory Address Mismatch Status */
#define ENET_MTL_ECC_INT_STATUS_EUES                        (1 << 10)   /* Bit 10: MTL EST memory Uncorrectable Error Status */
                                                                        /* Bit 11: Reserved */
#define ENET_MTL_ECC_INT_STATUS_RPCES                       (1 << 12)   /* Bit 12: MTL Rx Parser memory Correctable Error Status */
#define ENET_MTL_ECC_INT_STATUS_RPAMS                       (1 << 13)   /* Bit 13: MTL Rx Parser memory Address Mismatch Status */
#define ENET_MTL_ECC_INT_STATUS_RPUES                       (1 << 14)   /* Bit 14: MTL Rx Parser memory Uncorrectable Error Status */
                                                                        /* Bits 15-31: Reserved */

/* MTL ECC Error Status Register */

#define ENET_MTL_ECC_ERR_STS_RCTL_EESRE                     (1 << 0)    /* Bit 0: MTL ECC Error Status Read Enable */
#define ENET_MTL_ECC_ERR_STS_RCTL_EMS_SHIFT                 (1)         /* Bits 1-3: MTL ECC Memory Selection */
#define ENET_MTL_ECC_ERR_STS_RCTL_EMS_MASK                  (7 << ENET_MTL_ECC_ERR_STS_RCTL_EMS_SHIFT)
#define ENET_MTL_ECC_ERR_STS_RCTL_EMS_TX_MEM                (0x0 << ENET_MTL_ECC_ERR_STS_RCTL_EMS_SHIFT) /* MTL Tx memory */
#define ENET_MTL_ECC_ERR_STS_RCTL_EMS_RX_MEM                (0x1 << ENET_MTL_ECC_ERR_STS_RCTL_EMS_SHIFT) /* MTL Rx memory */
#define ENET_MTL_ECC_ERR_STS_RCTL_EMS_EST_MEM               (0x2 << ENET_MTL_ECC_ERR_STS_RCTL_EMS_SHIFT) /* MTL EST memory */
#define ENET_MTL_ECC_ERR_STS_RCTL_EMS_RXP_MEM               (0x3 << ENET_MTL_ECC_ERR_STS_RCTL_EMS_SHIFT) /* MTL Rx Parser memory */
#define ENET_MTL_ECC_ERR_STS_RCTL_CCES                      (1 << 4)                                     /* Bit 4: Clear Correctable Error Status */
#define ENET_MTL_ECC_ERR_STS_RCTL_CUES                      (1 << 5)                                     /* Bit 5: Clear Uncorrectable Error Status */
                                                                                                         /* Bits 6-31: Reserved */

/* MTL ECC Error Adress Status Register */

#define ENET_MTL_ECC_ERR_ADDR_STATUS_ECEAS_SHIFT            (0)         /* Bits 0-15: MTL ECC Correctable Error Address Status */
#define ENET_MTL_ECC_ERR_ADDR_STATUS_ECEAS_MASK             (0xffff << ENET_MTL_ECC_ERR_ADDR_STATUS_ECEAS_SHIFT)
#define ENET_MTL_ECC_ERR_ADDR_STATUS_ECEAS(x)               (((uint32_t)(x) << ENET_MTL_ECC_ERR_ADDR_STATUS_ECEAS_SHIFT) & ENET_MTL_ECC_ERR_ADDR_STATUS_ECEAS_MASK)
#define ENET_MTL_ECC_ERR_ADDR_STATUS_EUEAS_SHIFT            (16)        /* Bits 16-31: MTL ECC Uncorrectable Error Address Status */
#define ENET_MTL_ECC_ERR_ADDR_STATUS_EUEAS_MASK             (0xffff << ENET_MTL_ECC_ERR_ADDR_STATUS_EUEAS_SHIFT)
#define ENET_MTL_ECC_ERR_ADDR_STATUS_EUEAS(x)               (((uint32_t)(x) << ENET_MTL_ECC_ERR_ADDR_STATUS_EUEAS_SHIFT) & ENET_MTL_ECC_ERR_ADDR_STATUS_EUEAS_MASK)

/* MTL ECC Error Control Status Register */

#define ENET_MTL_ECC_ERR_CNTR_STATUS_ECECS_SHIFT            (0)         /* Bits 0-7: MTL ECC Correctable Error Counter Status */
#define ENET_MTL_ECC_ERR_CNTR_STATUS_ECECS_MASK             (0xff << ENET_MTL_ECC_ERR_CNTR_STATUS_ECECS_SHIFT)
                                                                        /* Bits 8-15: Reserved */
#define ENET_MTL_ECC_ERR_CNTR_STATUS_EUECS_SHIFT            (0)         /* Bits 16-19: MTL ECC Uncorrectable Error Counter Status */
#define ENET_MTL_ECC_ERR_CNTR_STATUS_EUECS_MASK             (0xff << ENET_MTL_ECC_ERR_CNTR_STATUS_EUECS_SHIFT)
                                                                        /* Bits 20-31: Reserved */

/* MTL DPP Control Register */

#define ENET_MTL_DPP_CONTROL_EDPP                           (1 << 0)    /* Bit 0: Enable Data path Parity Protection */
#define ENET_MTL_DPP_CONTROL_OPE                            (1 << 1)    /* Bit 1: Odd Parity Enable */
                                                                        /* Bits 2-5: Reserved */
#define ENET_MTL_DPP_CONTROL_IPETRD                         (1 << 6)    /* Bit 6: Insert Parity Error in DMA Tx/Rx Descriptor parity checker */
#define ENET_MTL_DPP_CONTROL_IPEMTF                         (1 << 7)    /* Bit 7: Insert Parity Error in MTL Tx FIFO write data parity checker */
#define ENET_MTL_DPP_CONTROL_IPEMTS                         (1 << 8)    /* Bit 8: Insert Parity Error in MTL Tx Status FIFO parity checker */
#define ENET_MTL_DPP_CONTROL_IPERID                         (1 << 9)    /* Bit 9: Insert Parity Error in RX Interface Data parity checker */
                                                                        /* Bits 10-12: Reserved */
#define ENET_MTL_DPP_CONTROL_IPEMTBU                        (1 << 13)   /* Bit 13: Insert Parity error in MTL RWC data parity checker */
#define ENET_MTL_DPP_CONTROL_IPEMTFC                        (1 << 14)   /* Bit 14: Insert Parity error in MAC TFC data parity checker */
#define ENET_MTL_DPP_CONTROL_IPEMRWC                        (1 << 15)   /* Bit 15: Insert Parity error in MTL RWC data parity checker */
                                                                        /* Bits 16-31: Reserved */

/* MTL Tx Queue Operation Mode Register */

#define ENET_MTL_TXQ_OPERATION_MODE_FTQ                     (1 << 0)    /* Bit 0: Flush Transmit Queue */
#define ENET_MTL_TXQ_OPERATION_MODE_TSF                     (1 << 1)    /* Bit 1: Transmit Store and Forward */
#define ENET_MTL_TXQ_OPERATION_MODE_TXQEN_SHIFT             (2)         /* Bits 2-3: MTL ECC Memory Selection */
#define ENET_MTL_TXQ_OPERATION_MODE_TXQEN_MASK              (3 << ENET_MTL_TXQ_OPERATION_MODE_TXQEN_SHIFT)
#define ENET_MTL_TXQ_OPERATION_MODE_TXQEN_DISABLE           (0x0 << ENET_MTL_TXQ_OPERATION_MODE_TXQEN_SHIFT) /* Not enabled */
#define ENET_MTL_TXQ_OPERATION_MODE_TXQEN_ENAV              (0x1 << ENET_MTL_TXQ_OPERATION_MODE_TXQEN_SHIFT) /* Enable in AV mode */
#define ENET_MTL_TXQ_OPERATION_MODE_TXQEN_ENABLE            (0x2 << ENET_MTL_TXQ_OPERATION_MODE_TXQEN_SHIFT) /* Enabled */
#define ENET_MTL_TXQ_OPERATION_MODE_TTC_SHIFT               (4)                                              /* Bits 4-6: MTL ECC Memory Selection */
#define ENET_MTL_TXQ_OPERATION_MODE_TTC_MASK                (7 << ENET_MTL_TXQ_OPERATION_MODE_TTC_SHIFT)
#define ENET_MTL_TXQ_OPERATION_MODE_TTC_32BYTES             (0x0 << ENET_MTL_TXQ_OPERATION_MODE_TTC_SHIFT)   /* 32 Bytes */
#define ENET_MTL_TXQ_OPERATION_MODE_TTC_64BYTES             (0x1 << ENET_MTL_TXQ_OPERATION_MODE_TTC_SHIFT)   /* 64 Bytes */
#define ENET_MTL_TXQ_OPERATION_MODE_TTC_96BYTES             (0x2 << ENET_MTL_TXQ_OPERATION_MODE_TTC_SHIFT)   /* 96 Bytes */
#define ENET_MTL_TXQ_OPERATION_MODE_TTC_128BYTES            (0x3 << ENET_MTL_TXQ_OPERATION_MODE_TTC_SHIFT)   /* 128 Bytes */
#define ENET_MTL_TXQ_OPERATION_MODE_TTC_192BYTES            (0x4 << ENET_MTL_TXQ_OPERATION_MODE_TTC_SHIFT)   /* 192 Bytes */
#define ENET_MTL_TXQ_OPERATION_MODE_TTC_256BYTES            (0x5 << ENET_MTL_TXQ_OPERATION_MODE_TTC_SHIFT)   /* 256 Bytes */
#define ENET_MTL_TXQ_OPERATION_MODE_TTC_384BYTES            (0x6 << ENET_MTL_TXQ_OPERATION_MODE_TTC_SHIFT)   /* 384 Bytes */
#define ENET_MTL_TXQ_OPERATION_MODE_TTC_512BYTES            (0x7 << ENET_MTL_TXQ_OPERATION_MODE_TTC_SHIFT)   /* 512 Bytes */
                                                                                                             /* Bits 7-15: Reserved */
#define ENET_MTL_TXQ_OPERATION_MODE_TQS_SHIFT               (16)                                             /* Bits 16-20: MTL ECC Uncorrectable Error Address Status */
#define ENET_MTL_TXQ_OPERATION_MODE_TQS_MASK                (0x1f << ENET_MTL_TXQ_OPERATION_MODE_TQS_SHIFT)
#define ENET_MTL_TXQ_OPERATION_MODE_TQS(x)                  (((uint32_t)(x) << ENET_MTL_TXQ_OPERATION_MODE_TQS_SHIFT) & ENET_MTL_TXQ_OPERATION_MODE_TQS_MASK)
                                                                        /* Bits 21-31: Reserved */

/* MTL Tx Queue Underflow Register */

#define ENET_MTL_TXQ_UNDERFLOW_UFFRMCNT_SHIFT               (0)         /* Bits 0-10: Underflow Packet Counter */
#define ENET_MTL_TXQ_UNDERFLOW_UFFRMCNT_MASK                (0x1f << ENET_MTL_TXQ_UNDERFLOW_UFFRMCNT_SHIFT)
#define ENET_MTL_TXQ_UNDERFLOW_UFCNTOVF                     (1 << 11)   /* Bit 11: Overflow Bit for Underflow Packet Counter */
                                                                        /* Bits 12-31: Reserved */

/* MTL Tx Queue Debug Register */

#define ENET_MTL_TXQ_DEBUG_TXQPAUSED                        (1 << 0)                                 /* Bit 0: Flush Transmit Queue */
#define ENET_MTL_TXQ_DEBUG_TRCSTS_SHIFT                     (1)                                      /* Bits 1-2: MTL ECC Memory Selection */
#define ENET_MTL_TXQ_DEBUG_TRCSTS_MASK                      (3 << ENET_MTL_TXQ_DEBUG_TRCSTS_SHIFT)
#define ENET_MTL_TXQ_DEBUG_TRCSTS_IDLE                      (0x0 << ENET_MTL_TXQ_DEBUG_TRCSTS_SHIFT) /* Idle state */
#define ENET_MTL_TXQ_DEBUG_TRCSTS_READ                      (0x1 << ENET_MTL_TXQ_DEBUG_TRCSTS_SHIFT) /* Read state (transferring data to the MAC transmitter) */
#define ENET_MTL_TXQ_DEBUG_TRCSTS_WAIT                      (0x2 << ENET_MTL_TXQ_DEBUG_TRCSTS_SHIFT) /* Waiting for pending Tx Status from the MAC transmitter */
#define ENET_MTL_TXQ_DEBUG_TRCSTS_FLUSH                     (0x3 << ENET_MTL_TXQ_DEBUG_TRCSTS_SHIFT) /* Flushing the Tx queue because of the Packet Abort request from the MAC */
#define ENET_MTL_TXQ_DEBUG_TWCSTS                           (1 << 3)                                 /* Bit 3: MTL Tx Queue Write Controller Status */
#define ENET_MTL_TXQ_DEBUG_TXQSTS                           (1 << 4)                                 /* Bit 4: MTL Tx Queue Not Empty Status */
#define ENET_MTL_TXQ_DEBUG_TXSTSFSTS                        (1 << 5)                                 /* Bit 5: MTL Tx Status FIFO Full Status */
                                                                                                     /* Bits 6-15: Reserved */
#define ENET_MTL_TXQ_DEBUG_PTXQ_SHIFT                       (16)                                     /* Bits 16-18: Number of Packets in the Transmit Queue */
#define ENET_MTL_TXQ_DEBUG_PTXQ_MASK                        (7 << ENET_MTL_TXQ_DEBUG_PTXQ_SHIFT)
                                                                                                     /* Bit 19: Reserved */
#define ENET_MTL_TXQ_DEBUG_STXSTSF_SHIFT                    (20)                                     /* Bits 20-22: Number of Status Words in Tx Status FIFO of Queue */
#define ENET_MTL_TXQ_DEBUG_STXSTSF_MASK                     (7 << ENET_MTL_TXQ_DEBUG_STXSTSF_SHIFT)
                                                                                                     /* Bits 23-31: Reserved */

/* MTL Tx Queue ETS Control Register */

#define ENET_MTL_TXQ_ETS_CONTROL_AVALG                      (1 << 2)    /* Bit 2: AV Algorithm */
#define ENET_MTL_TXQ_ETS_CONTROL_CC                         (1 << 3)    /* Bit 3: Credit Control */
#define ENET_MTL_TXQ_ETS_CONTROL_SLC_SHIFT                  (4)         /* Bits 4-6: MTL ECC Memory Selection */
#define ENET_MTL_TXQ_ETS_CONTROL_SLC_MASK                   (7 << ENET_MTL_TXQ_ETS_CONTROL_SLC_SHIFT)
#define ENET_MTL_TXQ_ETS_CONTROL_SLC_M_1_SLOT               (0x0 << ENET_MTL_TXQ_ETS_CONTROL_SLC_SHIFT) /* 1 slot */
#define ENET_MTL_TXQ_ETS_CONTROL_SLC_M_2_SLOT               (0x1 << ENET_MTL_TXQ_ETS_CONTROL_SLC_SHIFT) /* 2 slots */
#define ENET_MTL_TXQ_ETS_CONTROL_SLC_M_4_SLOT               (0x2 << ENET_MTL_TXQ_ETS_CONTROL_SLC_SHIFT) /* 4 slots */
#define ENET_MTL_TXQ_ETS_CONTROL_SLC_M_8_SLOT               (0x3 << ENET_MTL_TXQ_ETS_CONTROL_SLC_SHIFT) /* 8 slots */
#define ENET_MTL_TXQ_ETS_CONTROL_SLC_M_16_SLOT              (0x4 << ENET_MTL_TXQ_ETS_CONTROL_SLC_SHIFT) /* 16 slots */
                                                                                                        /* Bits 7-31: Reserved */

/* MTL Tx Queue ETS Status Register */

#define ENET_MTL_TXQ_ETS_STATUS_ABS_SHIFT                   (0)         /* Bits 0-23: Average Bits per Slot */
#define ENET_MTL_TXQ_ETS_STATUS_ABS_MASK                    (0xffffff << ENET_MTL_TXQ_ETS_STATUS_ABS_SHIFT)
                                                                        /* Bits 24-31: Reserved */

/* MTL Tx Queue Quantum Weight Register */

#define ENET_MTL_TXQ_QUANTUM_WEIGHT_ISCQW_SHIFT             (0)         /* Bits 0-20: idleSlopeCredit, Quantum or Weights */
#define ENET_MTL_TXQ_QUANTUM_WEIGHT_TCW_MASK                (0xff << ENET_MTL_TXQ_QUANTUM_WEIGHT_ISCQW_SHIFT)
#define ENET_MTL_TXQ_QUANTUM_WEIGHT_TCW(x)                  (((uint32_t)(x) << ENET_MTL_TXQ_QUANTUM_WEIGHT_ISCQW_SHIFT) & ENET_MTL_TXQ_QUANTUM_WEIGHT_TCW_MASK)
#define ENET_MTL_TXQ_QUANTUM_WEIGHT_ISC_MASK                (0x3fff << ENET_MTL_TXQ_QUANTUM_WEIGHT_ISCQW_SHIFT)
#define ENET_MTL_TXQ_QUANTUM_WEIGHT_ISC(x)                  (((uint32_t)(x) << ENET_MTL_TXQ_QUANTUM_WEIGHT_ISCQW_SHIFT) & ENET_MTL_TXQ_QUANTUM_WEIGHT_ISC_MASK)
                                                                        /* Bits 21-31: Reserved */

/* MTL Tx Queue Sendslope Credit Register */

#define ENET_MTL_TXQ_SENDSLOPE_SSC_SHIFT                    (0)         /* Bits 0-13: sendSlopeCredit Value */
#define ENET_MTL_TXQ_SENDSLOPE_SSC_MASK                     (0x3fff << ENET_MTL_TXQ_SENDSLOPE_SSC_SHIFT)
#define ENET_MTL_TXQ_SENDSLOPE_SSC(x)                       (((uint32_t)(x) << ENET_MTL_TXQ_SENDSLOPE_SSC_SHIFT) & ENET_MTL_TXQ_SENDSLOPE_SSC_MASK)
                                                                        /* Bits 14-31: Reserved */

/* MTL Tx Queue HiCredit Register */

#define ENET_MTL_TXQ_HICREDIT_HC_SHIFT                      (0)         /* Bits 0-28: hiCredit Value */
#define ENET_MTL_TXQ_HICREDIT_HC_MASK                       (0x1fffffff << ENET_MTL_TXQ_HICREDIT_HC_SHIFT)
#define ENET_MTL_TXQ_HICREDIT_HC(x)                         (((uint32_t)(x) << ENET_MTL_TXQ_HICREDIT_HC_SHIFT) & ENET_MTL_TXQ_HICREDIT_HC_MASK)
                                                                        /* Bits 29-31: Reserved */

/* MTL Tx Queue LoCredit Register */

#define ENET_MTL_TXQ_LOCREDIT_LC_SHIFT                      (0)         /* Bits 0-28: loCredit Value */
#define ENET_MTL_TXQ_LOCREDIT_LC_MASK                       (0x1fffffff << ENET_MTL_TXQ_LOCREDIT_LC_SHIFT)
#define ENET_MTL_TXQ_LOCREDIT_LC(x)                         (((uint32_t)(x) << ENET_MTL_TXQ_LOCREDIT_LC_SHIFT) & ENET_MTL_TXQ_LOCREDIT_LC_MASK)
                                                                        /* Bits 29-31: Reserved */

/* MTL Queue Interrupt Control Status Register */

#define ENET_MTLQ_INT_CONTROL_STATUS_TXUNFIS                (1 << 0)    /* Bit 0: Transmit Queue Underflow Interrupt Status */
#define ENET_MTLQ_INT_CONTROL_STATUS_ABPSIS                 (1 << 1)    /* Bit 1: Average Bits Per Slot Interrupt Status */
                                                                        /* Bits 2-7: Reserved */
#define ENET_MTLQ_INT_CONTROL_STATUS_TXUIE                  (1 << 8)    /* Bit 8: Transmit Queue Underflow Interrupt Enable */
#define ENET_MTLQ_INT_CONTROL_STATUS_ABPSIE                 (1 << 9)    /* Bit 9: Average Bits Per Slot Interrupt Enable */
                                                                        /* Bits 10-15: Reserved */
#define ENET_MTLQ_INT_CONTROL_STATUS_RXOVFIS                (1 << 16)   /* Bit 16: Receive Queue Overflow Interrupt Status */
                                                                        /* Bits 17-23: Reserved */
#define ENET_MTLQ_INT_CONTROL_STATUS_RXOIE                  (1 << 24)   /* Bit 24: Receive Queue Overflow Interrupt Enable */
                                                                        /* Bits 25-31: Reserved */

/* MTL Rx Queue Operation Mode Register */

#define ENET_MTL_RXQ_OPERATION_MODE_RTC_SHIFT               (0)         /* Bits 0-1: Receive Queue Threshold Control */
#define ENET_MTL_RXQ_OPERATION_MODE_RTC_MASK                (3 << ENET_MTL_RXQ_OPERATION_MODE_RTC_SHIFT)
#define ENET_MTL_RXQ_OPERATION_MODE_RTC(x)                  (((uint32_t)(x) << ENET_MTL_RXQ_OPERATION_MODE_RTC_SHIFT) & ENET_MTL_RXQ_OPERATION_MODE_RTC_MASK)
                                                                        /* Bit 2: Reserved */
#define ENET_MTL_RXQ_OPERATION_MODE_FUP                     (1 << 3)    /* Bit 3: Forward Undersized Good Packets */
#define ENET_MTL_RXQ_OPERATION_MODE_FEP                     (1 << 4)    /* Bit 4: Forward Error Packets */
#define ENET_MTL_RXQ_OPERATION_MODE_RSF                     (1 << 5)    /* Bit 5: Receive Queue Store and Forward */
#define ENET_MTL_RXQ_OPERATION_MODE_DIS_TCP_EF              (1 << 6)    /* Bit 6: Disable Dropping of TCP/IP Checksum Error Packets */
#define ENET_MTL_RXQ_OPERATION_MODE_EHFC                    (1 << 7)    /* Bit 7: Enable Hardware Flow Control */
#define ENET_MTL_RXQ_OPERATION_MODE_RFA_SHIFT               (8)         /* Bits 8-11: Threshold for Activating Flow Control  */
#define ENET_MTL_RXQ_OPERATION_MODE_RFA_MASK                (0xf << ENET_MTL_RXQ_OPERATION_MODE_RFA_SHIFT)
#define ENET_MTL_RXQ_OPERATION_MODE_RFA(x)                  (((uint32_t)(x) << ENET_MTL_RXQ_OPERATION_MODE_RFA_SHIFT) & ENET_MTL_RXQ_OPERATION_MODE_RFA_MASK)
                                                                        /* Bits 12-13: Reserved */
#define ENET_MTL_RXQ_OPERATION_MODE_RFD_SHIFT               (14)        /* Bits 14-17: Threshold for Deactivating Flow Control  */
#define ENET_MTL_RXQ_OPERATION_MODE_RFD_MASK                (0xf << ENET_MTL_RXQ_OPERATION_MODE_RFD_SHIFT)
#define ENET_MTL_RXQ_OPERATION_MODE_RFD(x)                  (((uint32_t)(x) << ENET_MTL_RXQ_OPERATION_MODE_RFD_SHIFT) & ENET_MTL_RXQ_OPERATION_MODE_RFD_MASK)
                                                                        /* Bits 18-19: Reserved */
#define ENET_MTL_RXQ_OPERATION_MODE_RQS_SHIFT               (20)        /* Bits 20-24: Receive Queue Size  */
#define ENET_MTL_RXQ_OPERATION_MODE_RQS_MASK                (0x1f << ENET_MTL_RXQ_OPERATION_MODE_RQS_SHIFT)
#define ENET_MTL_RXQ_OPERATION_MODE_RQS(x)                  (((uint32_t)(x) << ENET_MTL_RXQ_OPERATION_MODE_RQS_SHIFT) & ENET_MTL_RXQ_OPERATION_MODE_RQS_MASK)
                                                                        /* Bits 25-31: Reserved */

/* MTL Rx Queue Missed Packet Overflow Count Register */

#define ENET_MTL_RXQ_MISSED_PKT_OVFLOW_CNT_OVFPKTCNT_SHIFT  (0)         /* Bits 0-10: Overflow Packet Counter */
#define ENET_MTL_RXQ_MISSED_PKT_OVFLOW_CNT_OVFPKTCNT_MASK   (0x7ff << ENET_MTL_RXQ_MISSED_PKT_OVFLOW_CNT_OVFPKTCNT_SHIFT)
#define ENET_MTL_RXQ_MISSED_PKT_OVFLOW_CNT_OVFPKTCNT(x)     (((uint32_t)(x) << ENET_MTL_RXQ_MISSED_PKT_OVFLOW_CNT_OVFPKTCNT_SHIFT) & ENET_MTL_RXQ_MISSED_PKT_OVFLOW_CNT_OVFPKTCNT_MASK)
#define ENET_MTL_RXQ_MISSED_PKT_OVFLOW_CNT_OVFCNTOVF        (1 << 11)   /* Bit 11: Overflow Counter Overflow Bit */
                                                                        /* Bits 12-15: Reserved */
#define ENET_MTL_RXQ_MISSED_PKT_OVFLOW_CNT_MISPKTCNT_SHIFT  (16)        /* Bits 16-26: Missed Packet Counter */
#define ENET_MTL_RXQ_MISSED_PKT_OVFLOW_CNT_MISPKTCNT_MASK   (0x7ff << ENET_MTL_RXQ_MISSED_PKT_OVFLOW_CNT_MISPKTCNT_SHIFT)
#define ENET_MTL_RXQ_MISSED_PKT_OVFLOW_CNT_MISPKTCNT(x)     (((uint32_t)(x) << ENET_MTL_RXQ_MISSED_PKT_OVFLOW_CNT_MISPKTCNT_SHIFT) & ENET_MTL_RXQ_MISSED_PKT_OVFLOW_CNT_MISPKTCNT_MASK)
#define ENET_MTL_RXQ_MISSED_PKT_OVFLOW_CNT_MISCNTOVF        (1 << 27)   /* Bit 27: Missed Packet Counter Overflow Bit */
                                                                        /* Bits 28-31: Reserved */

/* MTL Rx Queue Debug Register */

#define ENET_MTL_RXQ_DEBUG_RWCSTS                           (1 << 0)                                 /* Bit 0: MTL Rx Queue Write Controller Active Status */
#define ENET_MTL_RXQ_DEBUG_RRCSTS_SHIFT                     (1)                                      /* Bits 1-2: MTL Rx Queue Read Controller State */
#define ENET_MTL_RXQ_DEBUG_RRCSTS_MASK                      (3 << ENET_MTL_RXQ_DEBUG_RRCSTS_SHIFT)
#define ENET_MTL_RXQ_DEBUG_RRCSTS_IDLE                      (0x0 << ENET_MTL_RXQ_DEBUG_RRCSTS_SHIFT) /* Idle state */
#define ENET_MTL_RXQ_DEBUG_RRCSTS_READ_DATA                 (0x1 << ENET_MTL_RXQ_DEBUG_RRCSTS_SHIFT) /* Reading packet data */
#define ENET_MTL_RXQ_DEBUG_RRCSTS_READ_STS                  (0x2 << ENET_MTL_RXQ_DEBUG_RRCSTS_SHIFT) /* Reading packet status  */
#define ENET_MTL_RXQ_DEBUG_RRCSTS_FLUSH                     (0x3 << ENET_MTL_RXQ_DEBUG_RRCSTS_SHIFT) /* Flushing the packet data and status */
                                                                                                     /* Bit 3: Reserved */
#define ENET_MTL_RXQ_DEBUG_RXQSTS_SHIFT                     (4)                                      /* Bits 4-5: MTL Rx Queue Fill-Level Status */
#define ENET_MTL_RXQ_DEBUG_RXQSTS_MASK                      (3 << ENET_MTL_RXQ_DEBUG_RXQSTS_SHIFT)
#define ENET_MTL_RXQ_DEBUG_RXQSTS_EMPTY                     (0x0 << ENET_MTL_RXQ_DEBUG_RXQSTS_SHIFT) /* Rx Queue empty */
#define ENET_MTL_RXQ_DEBUG_RXQSTS_BLW_THR                   (0x1 << ENET_MTL_RXQ_DEBUG_RXQSTS_SHIFT) /* Rx Queue fill-level below flow-control deactivate threshold */
#define ENET_MTL_RXQ_DEBUG_RXQSTS_ABV_THR                   (0x2 << ENET_MTL_RXQ_DEBUG_RXQSTS_SHIFT) /* Rx Queue fill-level above flow-control activate threshold  */
#define ENET_MTL_RXQ_DEBUG_RXQSTS_FULL                      (0x3 << ENET_MTL_RXQ_DEBUG_RXQSTS_SHIFT) /* Rx Queue full */
                                                                                                     /* Bits 6-15: Reserved */
#define ENET_MTL_RXQ_DEBUG_PRXQ_SHIFT                       (16)                                     /* Bits 16-29: Number of Packets in Receive Queue */
#define ENET_MTL_RXQ_DEBUG_PRXQ_MASK                        (0x1fff << ENET_MTL_RXQ_DEBUG_PRXQ_SHIFT)
#define ENET_MTL_RXQ_DEBUG_PRXQ(x)                          (((uint32_t)(x) << ENET_MTL_RXQ_DEBUG_PRXQ_SHIFT) & ENET_MTL_RXQ_DEBUG_PRXQ_MASK)
                                                                                                     /* Bits 30-31: Reserved */

/* MTL Rx Queue Control Register */

#define ENET_MTL_RXQ_CONTROL_RXQ_WEGT_SHIFT                 (0)         /* Bits 0-2: Receive Queue Weight */
#define ENET_MTL_RXQ_CONTROL_RXQ_WEGT_MASK                  (7 << ENET_MTL_RXQ_CONTROL_RXQ_WEGT_SHIFT)
#define ENET_MTL_RXQ_CONTROL_RXQ_WEGT(x)                    (((uint32_t)(x) << ENET_MTL_RXQ_CONTROL_RXQ_WEGT_SHIFT) & ENET_MTL_RXQ_CONTROL_RXQ_WEGT_MASK)
#define ENET_MTL_RXQ_CONTROL_RXQ_FRM_ARBIT                  (1 << 3)    /* Bit 3: Receive Queue Packet Arbitration */
                                                                        /* Bits 4-31: Reserved */

/* DMA Mode Register */

#define ENET_DMA_MODE_SWR                                   (1 << 0)                          /* Bit 0: Software Reset */
#define ENET_DMA_MODE_DA                                    (1 << 1)                          /* Bit 1: DMA Tx or Rx Arbitration Scheme */
#define ENET_DMA_MODE_TAA_SHIFT                             (2)                               /* Bits 2-4: MTL Rx Queue Fill-Level Status */
#define ENET_DMA_MODE_TAA_MASK                              (7 << ENET_DMA_MODE_TAA_SHIFT)
#define ENET_DMA_MODE_TAA_FP                                (0x0 << ENET_DMA_MODE_TAA_SHIFT)  /* Fixed priority */
#define ENET_DMA_MODE_TAA_WSP                               (0x1 << ENET_DMA_MODE_TAA_SHIFT)  /* Weighted Strict Priority */
#define ENET_DMA_MODE_TAA_WRR                               (0x2 << ENET_DMA_MODE_TAA_SHIFT)  /* Weighted Round-Robin  */
#define ENET_DMA_MODE_TAA_RSVD                              (0x3 << ENET_DMA_MODE_TAA_SHIFT)  /* Reserved */
                                                                                              /* Bits 5-10: Reserved */
#define ENET_DMA_MODE_TXPR                                  (1 << 11)                         /* Bit 11: Transmit Priority */
#define ENET_DMA_MODE_PR_SHIFT                              (12)                              /* Bits 12-14: Priority Ratio */
#define ENET_DMA_MODE_PR_MASK                               (7 << ENET_DMA_MODE_PR_SHIFT)
#define ENET_DMA_MODE_PR_R_1_1                              (0x0 << ENET_DMA_MODE_PR_SHIFT)   /* The priority ratio is 1:1*/
#define ENET_DMA_MODE_PR_R_2_1                              (0x1 << ENET_DMA_MODE_PR_SHIFT)   /* The priority ratio is 2:1 */
#define ENET_DMA_MODE_PR_R_3_1                              (0x2 << ENET_DMA_MODE_PR_SHIFT)   /* The priority ratio is 3:1  */
#define ENET_DMA_MODE_PR_R_4_1                              (0x3 << ENET_DMA_MODE_PR_SHIFT)   /* The priority ratio is 4:1 */
#define ENET_DMA_MODE_PR_R_5_1                              (0x4 << ENET_DMA_MODE_PR_SHIFT)   /* The priority ratio is 5:1 */
#define ENET_DMA_MODE_PR_R_6_1                              (0x5 << ENET_DMA_MODE_PR_SHIFT)   /* The priority ratio is 6:1 */
#define ENET_DMA_MODE_PR_R_7_1                              (0x6 << ENET_DMA_MODE_PR_SHIFT)   /* The priority ratio is 7:1 */
#define ENET_DMA_MODE_PR_R_8_1                              (0x7 << ENET_DMA_MODE_PR_SHIFT)   /* The priority ratio is 8:1 */
                                                                                              /* Bit 15: Reserved */
#define ENET_DMA_MODE_INTM_SHIFT                            (16)                              /* Bits 16-17: Interrupt Mode */
#define ENET_DMA_MODE_INTM_MASK                             (3 << ENET_DMA_MODE_INTM_SHIFT)
#define ENET_DMA_MODE_INTM_PULSE                            (0x0 << ENET_DMA_MODE_INTM_SHIFT) /* Interrupt Mode 0 */
#define ENET_DMA_MODE_INTM_LEVEL                            (0x1 << ENET_DMA_MODE_INTM_SHIFT) /* Interrupt Mode 1 */
#define ENET_DMA_MODE_INTM_LEVEL_REENT                      (0x2 << ENET_DMA_MODE_INTM_SHIFT) /* Interrupt Mode 2 */
                                                                                              /* Bits 18-31: Reserved */

/* DMA System Bus Mode Register */

#define ENET_DMA_SYSBUS_MODE_FB                             (1 << 0)    /* Bit 0: Fixed Burst Length */
                                                                        /* Bits 1-11: Reserved */
#define ENET_DMA_SYSBUS_MODE_AAL                            (1 << 12)   /* Bit 12: Address-Aligned Beats */
                                                                        /* Bit 13: Reserved */
#define ENET_DMA_SYSBUS_MODE_MB                             (1 << 14)   /* Bit 14: Mixed Burst */
#define ENET_DMA_SYSBUS_MODE_RB                             (1 << 15)   /* Bit 15: Rebuild INCRx Burst */
                                                                        /* Bits 16-31: Reserved */

/* DMA Interrupt Status Register */

#define ENET_DMA_INTERRUPT_STATUS_DCIS(c)                   (1 << (c))  /* Bits 0-1: DMA Channel c Interrupt Status */
                                                                        /* Bits 2-15: Reserved */
#define ENET_DMA_INTERRUPT_STATUS_MTLIS                     (1 << 16)   /* Bit 16: MTL Interrupt Status */
#define ENET_DMA_INTERRUPT_STATUS_MACIS                     (1 << 17)   /* Bit 17: MAC Interrupt Status */
                                                                        /* Bits 16-31: Reserved */

/* DMA TBS Control Register */

#define ENET_DMA_TBS_CTRL_FTOV                              (1 << 0)    /* Bit 0: Fetch Time Offset Valid */
                                                                        /* Bits 1-3: Reserved */
#define ENET_DMA_TBS_CTRL_FGOS_SHIFT                        (4)         /* Bits 4-6: Fetch GSN Offset */
#define ENET_DMA_TBS_CTRL_FGOS_MASK                         (7 << ENET_DMA_TBS_CTRL_FGOS_SHIFT)
#define ENET_DMA_TBS_CTRL_FGOS(x)                           (((uint32_t)(x) << ENET_DMA_TBS_CTRL_FGOS_SHIFT) & ENET_DMA_TBS_CTRL_FGOS_MASK)
                                                                        /* Bit 7: Reserved */
#define ENET_DMA_TBS_CTRL_FTOS_SHIFT                        (8)         /* Bits 8-31: Fetch Time Offset */
#define ENET_DMA_TBS_CTRL_FTOS_MASK                         (0xffffff << ENET_DMA_TBS_CTRL_FTOS_SHIFT)
#define ENET_DMA_TBS_CTRL_FTOS(x)                           (((uint32_t)(x) << ENET_DMA_TBS_CTRL_FTOS_SHIFT) & ENET_DMA_TBS_CTRL_FTOS_MASK)

/* DMA Safety Interrupt Status Register */

#define ENET_DMA_SAFETY_INT_STATUS_DECIS                    (1 << 0)    /* Bit 0: DMA ECC Correctable error Interrupt Status */
#define ENET_DMA_SAFETY_INT_STATUS_DEUIS                    (1 << 1)    /* Bit 1: DMA ECC Uncorrectable error Interrupt Status */
                                                                        /* Bits 2-27: Reserved */
#define ENET_DMA_SAFETY_INT_STATUS_MSCIS                    (1 << 28)   /* Bit 28: MTL Safety Correctable error Interrupt Status */
#define ENET_DMA_SAFETY_INT_STATUS_MSUIS                    (1 << 29)   /* Bit 29: MTL Safety Uncorrectable error Interrupt Status */
                                                                        /* Bit 30: Reserved */
#define ENET_DMA_SAFETY_INT_STATUS_MCSIS                    (1 << 31)   /* Bit 31: MAC Safety Uncorrectable Interrupt Status */

/* DMA Channel Control Register */

#define ENET_DMA_CH_CONTROL_PBLX8                           (1 << 16)   /* Bit 16: DMA burst length x8 mode */
                                                                        /* Bit 17: Reserved */
#define ENET_DMA_CH_CONTROL_DSL_SHIFT                       (18)        /* Bits 18-20: Descriptor Skip Length */
#define ENET_DMA_CH_CONTROL_DSL_MASK                        (7 << ENET_DMA_CH_CONTROL_DSL_SHIFT)
#define ENET_DMA_CH_CONTROL_DSL(x)                          (((uint32_t)(x) << ENET_DMA_CH_CONTROL_DSL_SHIFT) & ENET_DMA_CH_CONTROL_DSL_MASK)
                                                                        /* Bits 21-31: Reserved */

/* DMA Channel Tx Control Register */

#define ENET_DMA_CH_TX_CONTROL_ST                           (1 << 0)    /* Bit 0: Start or Stop Transmission Command */
#define ENET_DMA_CH_TX_CONTROL_TCW_SHIFT                    (1)         /* Bits 1-3: Transmit Channel Weight */
#define ENET_DMA_CH_TX_CONTROL_TCW_MASK                     (7 << ENET_DMA_CH_TX_CONTROL_TCW_SHIFT)
#define ENET_DMA_CH_TX_CONTROL_TCW(x)                       (((uint32_t)(x) << ENET_DMA_CH_TX_CONTROL_TCW_SHIFT) & ENET_DMA_CH_TX_CONTROL_TCW_MASK)
#define ENET_DMA_CH_TX_CONTROL_OSF                          (1 << 4)    /* Bit 4: Operate on Second Packet */
                                                                        /* Bits 5-15: Reserved */
#define ENET_DMA_CH_TX_CONTROL_TXPBL_SHIFT                  (16)        /* Bits 16-21: Transmit Programmable Burst Length */
#define ENET_DMA_CH_TX_CONTROL_TXPBL_MASK                   (0x3f << ENET_DMA_CH_TX_CONTROL_TXPBL_SHIFT)
#define ENET_DMA_CH_TX_CONTROL_TXPBL(x)                     (((uint32_t)(x) << ENET_DMA_CH_TX_CONTROL_TXPBL_SHIFT) & ENET_DMA_CH_TX_CONTROL_TXPBL_MASK)
#define ENET_DMA_CH_TX_CONTROL_ETIC                         (1 << 22)   /* Bit 22: Early Transmit Interrupt Control */
                                                                        /* Bits 23-27: Reserved */
#define ENET_DMA_CH_TX_CONTROL_EDSE                         (1 << 28)   /* Bit 28: Operate on Second Packet */
                                                                        /* Bits 29-31: Reserved */

/* DMA Channel Rx Control Register */

#define ENET_DMA_CH_RX_CONTROL_SR                           (1 << 0)    /* Bit 0: Start or Stop Receive */
#define ENET_DMA_CH_RX_CONTROL_RBSZ_LOW_SHIFT               (1)         /* Bits 1-2: Receive Buffer size Low (read only) */
#define ENET_DMA_CH_RX_CONTROL_RBSZ_LOW_MASK                (3 << ENET_DMA_CH_RX_CONTROL_RBSZ_LOW_SHIFT)
#define ENET_DMA_CH_RX_CONTROL_RBSZ_HIGH_SHIFT              (3)         /* Bits 3-14: Receive Buffer size High */
#define ENET_DMA_CH_RX_CONTROL_RBSZ_HIGH_MASK               (0xfff << ENET_DMA_CH_RX_CONTROL_RBSZ_HIGH_SHIFT)
#define ENET_DMA_CH_RX_CONTROL_RBSZ_HIGH(x)                 (((uint32_t)(x) << ENET_DMA_CH_RX_CONTROL_RBSZ_HIGH_SHIFT) & ENET_DMA_CH_RX_CONTROL_RBSZ_HIGH_MASK)
                                                                        /* Bit 15: Reserved */
#define ENET_DMA_CH_RX_CONTROL_RXPBL_SHIFT                  (16)        /* Bits 16-21: Receive Buffer size High */
#define ENET_DMA_CH_RX_CONTROL_RXPBL_MASK                   (0x3f << ENET_DMA_CH_RX_CONTROL_RXPBL_SHIFT)
#define ENET_DMA_CH_RX_CONTROL_RXPBL(x)                     (((uint32_t)(x) << ENET_DMA_CH_RX_CONTROL_RXPBL_SHIFT) & ENET_DMA_CH_RX_CONTROL_RXPBL_MASK)
#define ENET_DMA_CH_RX_CONTROL_ERIC                         (1 << 22)   /* Bit 22: Early Receive Interrupt Control */
                                                                        /* Bits 23-31: Reserved */

/* DMA Channel Tx Descriptor List Address Register (32-bit address) */

/* DMA Channel Rx Descriptor List Address Register (32-bit address) */

/* DMA Channel Tx Descriptor Tail Pointer Register (32-bit address) */

/* DMA Channel Rx Descriptor Tail Pointer Register (32-bit address) */

/* DMA Channel Tx Descriptor Ring Length Register */

#define ENET_DMA_CH_TXDESC_RING_LENGTH_TDRL_SHIFT           (0)         /* Bits 0-9: Transmit Descriptor Ring Length */
#define ENET_DMA_CH_TXDESC_RING_LENGTH_TDRL_MASK            (0x3ff << ENET_DMA_CH_TXDESC_RING_LENGTH_TDRL_SHIFT)
#define ENET_DMA_CH_TXDESC_RING_LENGTH_TDRL(x)              (((uint32_t)(x) << ENET_DMA_CH_TXDESC_RING_LENGTH_TDRL_SHIFT) & ENET_DMA_CH_TXDESC_RING_LENGTH_TDRL_MASK)
                                                                        /* Bits 10-31: Reserved */

/* DMA Channel Rx Descriptor Ring Length Register */

#define ENET_DMA_CH_RXDESC_RING_LENGTH_RDRL_SHIFT           (0)         /* Bits 0-9: Receive Descriptor Ring Length */
#define ENET_DMA_CH_RXDESC_RING_LENGTH_RDRL_MASK            (0x3ff << ENET_DMA_CH_RXDESC_RING_LENGTH_RDRL_SHIFT)
#define ENET_DMA_CH_RXDESC_RING_LENGTH_RDRL(x)              (((uint32_t)(x) << ENET_DMA_CH_RXDESC_RING_LENGTH_RDRL_SHIFT) & ENET_DMA_CH_RXDESC_RING_LENGTH_RDRL_MASK)
                                                                        /* Bits 10-31: Reserved */

/* DMA Channel Interrupt Enable Register */

#define ENET_DMA_CH_INTERRUPT_ENABLE_TIE                    (1 << 0)    /* Bit 0: Transmit Interrupt Enable */
#define ENET_DMA_CH_INTERRUPT_ENABLE_TXSE                   (1 << 1)    /* Bit 1: Transmit Stopped Interrupt Enable */
#define ENET_DMA_CH_INTERRUPT_ENABLE_TBUE                   (1 << 2)    /* Bit 2: Transmit Buffer Unavailable Interrupt Enable */
                                                                        /* Bits 3-5: Reserved */
#define ENET_DMA_CH_INTERRUPT_ENABLE_RIE                    (1 << 6)    /* Bit 6: Receive Interrupt Enable */
#define ENET_DMA_CH_INTERRUPT_ENABLE_RBUE                   (1 << 7)    /* Bit 7: Receive Buffer Unavailable Interrupt Enable */
#define ENET_DMA_CH_INTERRUPT_ENABLE_RSE                    (1 << 8)    /* Bit 8: Receive Stopped Interrupt Enable */
#define ENET_DMA_CH_INTERRUPT_ENABLE_RWTE                   (1 << 9)    /* Bit 9: Receive Watchdog Timeout Enable */
#define ENET_DMA_CH_INTERRUPT_ENABLE_ETIE                   (1 << 10)   /* Bit 10: Early Transmit Interrupt Enable */
#define ENET_DMA_CH_INTERRUPT_ENABLE_ERIE                   (1 << 11)   /* Bit 11: Early Receive Interrupt Enable */
#define ENET_DMA_CH_INTERRUPT_ENABLE_FBEE                   (1 << 12)   /* Bit 12: Fatal Bus Error Interrupt Enable */
#define ENET_DMA_CH_INTERRUPT_ENABLE_CDEE                   (1 << 13)   /* Bit 13: Context Descriptor Error Interrupt Enable */
#define ENET_DMA_CH_INTERRUPT_ENABLE_AIE                    (1 << 14)   /* Bit 14: Abnormal Interrupt Summary Enable */
#define ENET_DMA_CH_INTERRUPT_ENABLE_NIE                    (1 << 15)   /* Bit 15: Normal Interrupt Summary Enable */
                                                                        /* Bits 16-31: Reserved */

/* DMA Channel Rx Interrupt Watchdog Timer Register */

#define ENET_DMA_CH_RX_INT_WATCHDOG_TIMER_RWT_SHIFT         (0)         /* Bits 0-7: Receive Interrupt Watchdog Timer Count */
#define ENET_DMA_CH_RX_INT_WATCHDOG_TIMER_RWT_MASK          (0xff << ENET_DMA_CH_RX_INT_WATCHDOG_TIMER_RWT_SHIFT)
#define ENET_DMA_CH_RX_INT_WATCHDOG_TIMER_RWT(x)            (((uint32_t)(x) << ENET_DMA_CH_RX_INT_WATCHDOG_TIMER_RWT_SHIFT) & ENET_DMA_CH_RX_INT_WATCHDOG_TIMER_RWT_MASK)
                                                                        /* Bits 8-15: Reserved */
#define ENET_DMA_CH_RX_INT_WATCHDOG_TIMER_RWTU_SHIFT        (16)        /* Bits 16-17: Receive Interrupt Watchdog Timer Count Units */
#define ENET_DMA_CH_RX_INT_WATCHDOG_TIMER_RWTU_MASK         (3<< ENET_DMA_CH_RX_INT_WATCHDOG_TIMER_RWTU_SHIFT)
#define ENET_DMA_CH_RX_INT_WATCHDOG_TIMER_RWTU(x)           (((uint32_t)(x) << ENET_DMA_CH_RX_INT_WATCHDOG_TIMER_RWTU_SHIFT) & ENET_DMA_CH_RX_INT_WATCHDOG_TIMER_RWTU_MASK)
                                                                        /* Bits 18-31: Reserved */

/* DMA Channel Slot Function Control Status Register */

#define ENET_DMA_CH_SLOT_FUNC_CTRL_STATUS_ESC               (1 << 0)    /* Bit 0: Enable Slot Comparison */
#define ENET_DMA_CH_SLOT_FUNC_CTRL_STATUS_ASC               (1 << 1)    /* Bit 1: Advance Slot Check */
                                                                        /* Bits 2-3: Reserved */
#define ENET_DMA_CH_SLOT_FUNC_CTRL_STATUS_SIV_SHIFT         (4)         /* Bits 4-15: Slot Interval Value */
#define ENET_DMA_CH_SLOT_FUNC_CTRL_STATUS_SIV_MASK          (0xfff << ENET_DMA_CH_SLOT_FUNC_CTRL_STATUS_SIV_SHIFT)
#define ENET_DMA_CH_SLOT_FUNC_CTRL_STATUS_SIV(x)            (((uint32_t)(x) << ENET_DMA_CH_SLOT_FUNC_CTRL_STATUS_SIV_SHIFT) & ENET_DMA_CH_SLOT_FUNC_CTRL_STATUS_SIV_MASK)
#define ENET_DMA_CH_SLOT_FUNC_CTRL_STATUS_RSN_SHIFT         (16)        /* Bits 16-19: Slot Interval Value */
#define ENET_DMA_CH_SLOT_FUNC_CTRL_STATUS_RSN_MASK          (0xfff << ENET_DMA_CH_SLOT_FUNC_CTRL_STATUS_RSN_SHIFT)
#define ENET_DMA_CH_SLOT_FUNC_CTRL_STATUS_RSN(x)            (((uint32_t)(x) << ENET_DMA_CH_SLOT_FUNC_CTRL_STATUS_RSN_SHIFT) & ENET_DMA_CH_SLOT_FUNC_CTRL_STATUS_RSN_MASK)
                                                                        /* Bits 20-31: Reserved */

/* DMA Channel Status Register */

#define ENET_DMA_CH_STATUS_TI                               (1 << 0)    /* Bit 0: Transmit Interrupt */
#define ENET_DMA_CH_STATUS_TPS                              (1 << 1)    /* Bit 1: Transmit Process Stopped */
#define ENET_DMA_CH_STATUS_TBU                              (1 << 2)    /* Bit 2: Transmit Buffer Unavailable */
                                                                        /* Bits 3-5: Reserved */
#define ENET_DMA_CH_STATUS_RI                               (1 << 6)    /* Bit 6: Receive Interrupt */
#define ENET_DMA_CH_STATUS_RBU                              (1 << 7)    /* Bit 7: Receive Buffer Unavailable */
#define ENET_DMA_CH_STATUS_RPS                              (1 << 8)    /* Bit 8: Receive Stopped */
#define ENET_DMA_CH_STATUS_RWT                              (1 << 9)    /* Bit 9: Receive Watchdog Timeout */
#define ENET_DMA_CH_STATUS_ETI                              (1 << 10)   /* Bit 10: Early Transmit Interrupt */
#define ENET_DMA_CH_STATUS_ERI                              (1 << 11)   /* Bit 11: Early Receive Interrupt */
#define ENET_DMA_CH_STATUS_FBE                              (1 << 12)   /* Bit 12: Fatal Bus Error Interrupt */
#define ENET_DMA_CH_STATUS_CDE                              (1 << 13)   /* Bit 13: Context Descriptor Error Interrupt */
#define ENET_DMA_CH_STATUS_AIS                              (1 << 14)   /* Bit 14: Abnormal Interrupt Summary */
#define ENET_DMA_CH_STATUS_NIS                              (1 << 15)   /* Bit 15: Normal Interrupt Summary */
                                                                        /* Bits 16-31: Reserved */

/* DMA Channel Miss Frame Counter Register */

#define ENET_DMA_CH_MISS_FRAME_CNT_MFCSHIFT                 (0)         /* Bits 0-10: Dropped Packet Counters */
#define ENET_DMA_CH_MISS_FRAME_CNT_MFC_MASK                 (0x7ff << ENET_DMA_CH_MISS_FRAME_CNT_MFCSHIFT)
                                                                        /* Bits 11-14: Reserved */
#define ENET_DMA_CH_MISS_FRAME_CNT_MFCO                     (1 << 15)   /* Bit 15: Overflow status of the MFC Counter */
                                                                        /* Bits 16-31: Reserved */

/* Transmit DMA Descriptor 2 bits for Read Format */

#define ENET_DMA_TDES2_RD_IOC                               (1 << 31)                                            /* Interrupt Enabled on Completion */
#define ENET_DMA_TDES2_RD_TTSE                              (1 << 30)                                            /* Transmit Timestamp Enable */
#define ENET_DMA_TDES2_RD_VTIR(x)                           (((uint32_t)(((uint32_t)(x)) << 14u)) & 0x0000c000u) /* VLAN Tag Insertion or Replacement */
#define ENET_DMA_TDES2_RD_B1L(x)                            (((uint32_t)(((uint32_t)(x)) << 0u)) & 0x00003fffu)  /* Buffer 1 Length */
#define ENET_DMA_TDES2_RD_B2L(x)                            (((uint32_t)(((uint32_t)(x)) << 16u)) & 0x3fff0000u) /* Buffer 2 Length */

/* Transmit DMA Descriptor 3 bits for Read Format */

#define ENET_DMA_TDES3_RD_CPC(x)                            (((uint32_t)(((uint32_t)(x)) << 26u)) & 0x0C000000u) /* CRC Pad Control */
#define ENET_DMA_TDES3_RD_CIC(x)                            (((uint32_t)(((uint32_t)(x)) << 16u)) & 0x00030000u) /* Checksum Insertion Control */
#define ENET_DMA_TDES3_RD_FD                                (1 << 29)                                            /* First Descriptor */
#define ENET_DMA_TDES3_RD_LD                                (1 << 28)                                            /* Last Descriptor */
#define ENET_DMA_TDES3_RD_FL(x)                             (((uint32_t)(((uint32_t)(x)) << 0u)) & 0x00007fffu)  /* Frame Length */

/* Transmit DMA Descriptor 3 bits for Write Back Format */

#define ENET_DMA_TDES3_WB_TTSS                              (1 << 17) /* Transmit Timestamp Status */

/* Transmit DMA Descriptor 2 bits for Context Format */

#define ENET_DMA_TDES2_CT_IVT(x)                            (((uint32_t)(((uint32_t)(x)) << 16u)) & 0xffff0000u) /* Inner VLAN Tag */

/* Transmit DMA Descriptor 3 bits for Context Format */

#define ENET_DMA_TDES3_CT_IVTIR(x)                          (((uint32_t)(((uint32_t)(x)) << 18u)) & 0x000C0000u) /* Inner VLAN Tag Insert or Replace */
#define ENET_DMA_TDES3_CT_IVLTV                             (1 << 17)                                            /* Inner VLAN Tag Valid */
#define ENET_DMA_TDES3_CT_VLTV                              (1 << 16)                                            /* VLAN Tag Valid */
#define ENET_DMA_TDES3_CT_VT(x)                             (((uint32_t)(((uint32_t)(x)) << 0u)) & 0x0000ffffu)  /* VLAN Tag */

/* Receive DMA Descriptor 3 bits for Read Format */

#define ENET_DMA_RDES3_RD_IOC                               (1 << 30) /* Interrupt Enabled */
#define ENET_DMA_RDES3_RD_BUF1V                             (1 << 24) /* Buffer 1 Address Valid */
#define ENET_DMA_RDES3_RD_BUF2V                             (1 << 25) /* Buffer 2 Address Valid  */

/* Receive DMA Descriptor 1 bits for Write Back Format */

#define ENET_DMA_RDES1_WB_TSA                               (1 << 14) /* Timestamp Available */
#define ENET_DMA_RDES1_WB_IPCE                              (1 << 7)  /* IP Payload Error */
#define ENET_DMA_RDES1_WB_IPHE                              (1 << 3)  /* IP Header Error */

/* Receive DMA Descriptor 3 bits for Write Back Format */

#define ENET_DMA_RDES3_WB_FD                                (1 << 29) /* First Descriptor */
#define ENET_DMA_RDES3_WB_LD                                (1 << 28) /* Last Descriptor */
#define ENET_DMA_RDES3_WB_RS2V                              (1 << 27) /* Receive Status RDES2 Valid */
#define ENET_DMA_RDES3_WB_RS1V                              (1 << 26) /* Receive Status RDES1 Valid */
#define ENET_DMA_RDES3_WB_RS0V                              (1 << 25) /* Receive Status RDES0 Valid */
#define ENET_DMA_RDES3_WB_CE                                (1 << 24) /* CRC Error */
#define ENET_DMA_RDES3_WB_RWT                               (1 << 22) /* Receive Watchdog Timeout */
#define ENET_DMA_RDES3_WB_OE                                (1 << 21) /* Overflow Error */
#define ENET_DMA_RDES3_WB_RE                                (1 << 20) /* Receive Error */
#define ENET_DMA_RDES3_WB_ES                                (1 << 15) /* Error Summary */
#define ENET_DMA_RDES3_WB_PL                                (0x7fffu) /* Packet Length */

/* DMA Descriptor bits for All Format */

#define ENET_DMA_DESC3_OWN                                  (1 << 31)   /* Descriptor Owned by DMA */
#define ENET_DMA_DESC3_CTXT                                 (1 << 30)   /* Descriptor is Context Format */

#endif /* __VENDOR_FLAGCHIP_CHIPS_FC7300_HARDWARE_FC7300_ENET_H */
