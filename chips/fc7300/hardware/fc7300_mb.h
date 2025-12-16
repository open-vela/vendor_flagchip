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

#ifndef __VENDOR_FLAGCHIP_CHIPS_FC7300_HARDWARE_FC7300_MB_H
#define __VENDOR_FLAGCHIP_CHIPS_FC7300_HARDWARE_FC7300_MB_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>
#include <hardware/fc7300_memorymap.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Mailbox Communication Channels Count *************************************/

#define FC7300_MB_COM_CHN_CNT                              (16u)

/* Mailbox Interrupt Channels Count *****************************************/

#define FC7300_MB_INT_CHN_CNT                              (4u)

/* Register Offsets *********************************************************/

#define FC7300_MB_CC_SEMA_OFFSET(N)                        (0x0000 + 0x30 * (N))  /* Communication Channel Smeaphore Register */
#define FC7300_MB_CC_SEMA_UNLK_OFFSET(N)                   (0x0004 + 0x30 * (N))  /* Communication Channel Smeaphore Unlock Register */
#define FC7300_MB_CC_REQUEST_OFFSET(N)                     (0x0008 + 0x30 * (N))  /* Communication Channel Request Register */
#define FC7300_MB_CC_DONE_OFFSET(N)                        (0x000C + 0x30 * (N))  /* Communication Channel Done Register */
#define FC7300_MB_CC_DONE_MASK_OFFSET(N)                   (0x0010 + 0x30 * (N))  /* Communication Channel Done Mask Register */
#define FC7300_MB_CC_DATA0_OFFSET(N)                       (0x0014 + 0x30 * (N))  /* Communication Channel Data Register 0 */
#define FC7300_MB_CC_DATA1_OFFSET(N)                       (0x0018 + 0x30 * (N))  /* Communication Channel Data Register 1 */
#define FC7300_MB_CC_STAT_OFFSET(N)                        (0x001C + 0x30 * (N))  /* Communication Channel Status Register */
#define FC7300_MB_CC_CLR_OFFSET(N)                         (0x0020 + 0x30 * (N))  /* Communication Channel Clear Register */
#define FC7300_MB_INT_FLG_OFFSET(N)                        (0x0800 + 0x20 * (N))  /* Interrupt Channel Flag Register */
#define FC7300_MB_INT_FLG_MASK_OFFSET(N)                   (0x0804 + 0x20 * (N))  /* Interrupt Channel Flag Mask Register */
#define FC7300_MB_INT_INTEN_OFFSET(N)                      (0x0808 + 0x20 * (N))  /* Interrupt Channel Interrupt Enable Register */
#define FC7300_MB_INT_FLG_STAT_OFFSET(N)                   (0x080C + 0x20 * (N))  /* Interrupt Channel Flag Status Register */
#define FC7300_MB_INT_CTRL_OFFSET(N)                       (0x0810 + 0x20 * (N))  /* Interrupt Channel Control Register */

/* Register Addresses *******************************************************/

#define FC7300_MB_CC_SEMA(N)                               (FC7300_MB_BASE + FC7300_MB_CC_SEMA_OFFSET(N))
#define FC7300_MB_CC_SEMA_UNLK(N)                          (FC7300_MB_BASE + FC7300_MB_CC_SEMA_UNLK_OFFSET(N))
#define FC7300_MB_CC_REQUEST(N)                            (FC7300_MB_BASE + FC7300_MB_CC_REQUEST_OFFSET(N))
#define FC7300_MB_CC_DONE(N)                               (FC7300_MB_BASE + FC7300_MB_CC_DONE_OFFSET(N))
#define FC7300_MB_CC_DONE_MASK(N)                          (FC7300_MB_BASE + FC7300_MB_CC_DONE_MASK_OFFSET(N))
#define FC7300_MB_CC_DATA0(N)                              (FC7300_MB_BASE + FC7300_MB_CC_DATA0_OFFSET(N))
#define FC7300_MB_CC_DATA1(N)                              (FC7300_MB_BASE + FC7300_MB_CC_DATA1_OFFSET(N))
#define FC7300_MB_CC_STAT(N)                               (FC7300_MB_BASE + FC7300_MB_CC_STAT_OFFSET(N))
#define FC7300_MB_CC_CLR(N)                                (FC7300_MB_BASE + FC7300_MB_CC_CLR_OFFSET(N))
#define FC7300_MB_INT_FLG(N)                               (FC7300_MB_BASE + FC7300_MB_INT_FLG_OFFSET(N))
#define FC7300_MB_INT_FLG_MASK(N)                          (FC7300_MB_BASE + FC7300_MB_INT_FLG_MASK_OFFSET(N))
#define FC7300_MB_INT_INTEN(N)                             (FC7300_MB_BASE + FC7300_MB_INT_INTEN_OFFSET(N))
#define FC7300_MB_INT_FLG_STAT(N)                          (FC7300_MB_BASE + FC7300_MB_INT_FLG_STAT_OFFSET(N))
#define FC7300_MB_INT_CTRL(N)                              (FC7300_MB_BASE + FC7300_MB_INT_CTRL_OFFSET(N))

/* Register Bit Definitions *************************************************/

/* MB_CCn_SEMA Bit Fields */

#define FC7300_MB_CC_SEMA_LOCK_MASTER_SUPERVISOR           (1 << 0)
#define FC7300_MB_CC_SEMA_LOCK_MASTER_SEC                  (1 << 1)
#define FC7300_MB_CC_SEMA_LOCK_MASTER_ID_MASK              (0xF0u)
#define FC7300_MB_CC_SEMA_LOCK_MASTER_ID_SHIFT             (4u)
#define FC7300_MB_CC_SEMA_LOCK_MASTER_ID_WIDTH             (4u)
#define FC7300_MB_CC_SEMA_LOCK                             (1 << 31)

/* MB_CCn_SEMA_UNLK Bit Fields */

#define FC7300_MB_CC_SEMA_UNLK_AUTO_CLEAR_EN0              (1 << 0)
#define FC7300_MB_CC_SEMA_UNLK_AUTO_CLEAR_EN1              (1 << 1)
#define FC7300_MB_CC_SEMA_UNLK_AUTO_CLEAR_EN2              (1 << 2)
#define FC7300_MB_CC_SEMA_UNLK_AUTO_CLEAR_EN3              (1 << 3)
#  define FC7300_MB_CC_SEMA_UNLK_AUTO_CLEAR_EN(n)          (1 << (uint32_t)(n))

/* MB_CCn_REQUEST Bit Fields */

#define FC7300_MB_CC_REQUEST_REQ0                          (1 << 0)
#define FC7300_MB_CC_REQUEST_REQ1                          (1 << 1)
#define FC7300_MB_CC_REQUEST_REQ2                          (1 << 2)
#define FC7300_MB_CC_REQUEST_REQ3                          (1 << 3)
#  define FC7300_MB_CC_REQUEST_REQ(n)                      (1 << (uint32_t)(n))

/* MB_CCn_DONE Bit Fields */

#define FC7300_MB_CC_DONE_DONE0                            (1 << 0)
#define FC7300_MB_CC_DONE_DONE1                            (1 << 1)
#define FC7300_MB_CC_DONE_DONE2                            (1 << 2)
#define FC7300_MB_CC_DONE_DONE3                            (1 << 3)
#  define FC7300_MB_CC_DONE_DONE(n)                        (1 << (uint32_t)(n))

/* MB_CCn_DONE_MASK Bit Fields */

#define FC7300_MB_CC_DONE_CLEAR_LOCK_KEY                   (0xFC730000)

#define FC7300_MB_CC_DONE_MASK_DONE_MASK0                  (1 << 0)
#define FC7300_MB_CC_DONE_MASK_DONE_MASK1                  (1 << 1)
#define FC7300_MB_CC_DONE_MASK_DONE_MASK2                  (1 << 2)
#define FC7300_MB_CC_DONE_MASK_DONE_MASK3                  (1 << 3)
#  define FC7300_MB_CC_DONE_MASK_DONE_MASK(n)              (1 << (uint32_t)(n))
#define FC7300_MB_CC_DONE_MASK_DONE_MASTER_ID_MASK         (0xF0000u)
#define FC7300_MB_CC_DONE_MASK_DONE_MASTER_ID_SHIFT        (16u)
#define FC7300_MB_CC_DONE_MASK_DONE_MASTER_ID_WIDTH        (4u)
#  define FC7300_MB_CC_DONE_MASK_DONE_MASTER_ID(x)         (((uint32_t)(((uint32_t)(x))<<FC7300_MB_CC_DONE_MASK_DONE_MASTER_ID_SHIFT))&FC7300_MB_CC_DONE_MASK_DONE_MASTER_ID_MASK)

/* MB_CCn_DATA0 Bit Fields */

/* MB_CCn_DATA1 Bit Fields */

/* MB_CCn_STAT Bit Fields */

#define FC7300_MB_CC_STAT_CURRENT_LOCK_MASTER_SUPERVISOR   (1 << 0)
#define FC7300_MB_CC_STAT_CURRENT_LOCK_MASTER_SEC          (1 << 1)
#define FC7300_MB_CC_STAT_CURRENT_LOCK_MASTER_ID_MASK      (0xF0u)
#define FC7300_MB_CC_STAT_CURRENT_LOCK_MASTER_ID_SHIFT     (4u)
#define FC7300_MB_CC_STAT_CURRENT_LOCK_MASTER_ID_WIDTH     (4u)
#define FC7300_MB_CC_STAT_CURRENT_LOCK_STATUS              (1 << 31)

/* MB_CCn_CLR Bit Fields */

#define FC7300_MB_CC_CLR_SOFTWARE_CLEAR_LOCK_KEY           (0xFC200000u)

/* MB_INTn_FLG Bit Fields */

#define FC7300_MB_INT_FLG_REQ_FLAG_MASK                    (0xFFFFu)
#define FC7300_MB_INT_FLG_REQ_FLAG_SHIFT                   (0u)
#define FC7300_MB_INT_FLG_REQ_FLAG_WIDTH                   (16u)
#define FC7300_MB_INT_FLG_REQ_FLAG_CH(n)                   (1 << (FC7300_MB_INT_FLG_REQ_FLAG_SHIFT + (uint32_t)(n)))
#define FC7300_MB_INT_FLG_DONE_FLAG_MASK                   (0xFFFF0000u)
#define FC7300_MB_INT_FLG_DONE_FLAG_SHIFT                  (16u)
#define FC7300_MB_INT_FLG_DONE_FLAG_WIDTH                  (16u)
#define FC7300_MB_INT_FLG_DONE_FLAG_CH(n)                  (1 << (FC7300_MB_INT_FLG_DONE_FLAG_SHIFT + (uint32_t)(n)))

/* MB_INTn_FLG_MASK Bit Fields */

#define FC7300_MB_INT_FLG_MASK_REQ_FLAG_MASK_MASK          (0xFFFFu)
#define FC7300_MB_INT_FLG_MASK_REQ_FLAG_MASK_SHIFT         (0u)
#define FC7300_MB_INT_FLG_MASK_REQ_FLAG_MASK_WIDTH         (16u)
#define FC7300_MB_INT_FLG_MASK_REQ_FLAG_MASK_CH(n)         (1 << (FC7300_MB_INT_FLG_MASK_REQ_FLAG_MASK_SHIFT + (uint32_t)(n)))
#define FC7300_MB_INT_FLG_MASK_DONE_FLAG_MASK_MASK         (0xFFFF0000u)
#define FC7300_MB_INT_FLG_MASK_DONE_FLAG_MASK_SHIFT        (16u)
#define FC7300_MB_INT_FLG_MASK_DONE_FLAG_MASK_WIDTH        (16u)
#define FC7300_MB_INT_FLG_MASK_DONE_FLAG_MASK_CH(n)        (1 << (FC7300_MB_INT_FLG_MASK_DONE_FLAG_MASK_SHIFT + (uint32_t)(n)))

/* MB_INTn_INTEN Bit Fields */

#define FC7300_MB_INT_INTEN_REQ_INT_EN_MASK                (0xFFFFu)
#define FC7300_MB_INT_INTEN_REQ_INT_EN_SHIFT               (0u)
#define FC7300_MB_INT_INTEN_REQ_INT_EN_WIDTH               (16u)
#define FC7300_MB_INT_INTEN_REQ_INT_EN_CH(n)               (1 << (FC7300_MB_INT_INTEN_REQ_INT_EN_SHIFT + (uint32_t)(n)))
#define FC7300_MB_INT_INTEN_DONE_INT_EN_MASK               (0xFFFF0000u)
#define FC7300_MB_INT_INTEN_DONE_INT_EN_SHIFT              (16u)
#define FC7300_MB_INT_INTEN_DONE_INT_EN_WIDTH              (16u)
#define FC7300_MB_INT_INTEN_DONE_INT_EN_CH(n)              (1 << (FC7300_MB_INT_INTEN_DONE_INT_EN_SHIFT + (uint32_t)(n)))

/* MB_INTn_FLG_STAT Bit Fields */

#define FC7300_MB_INT_FLG_STAT_REQ_FLAG_STAT_MASK          (0xFFFFu)
#define FC7300_MB_INT_FLG_STAT_REQ_FLAG_STAT_SHIFT         (0u)
#define FC7300_MB_INT_FLG_STAT_REQ_FLAG_STAT_WIDTH         (16u)
#define FC7300_MB_INT_FLG_STAT_REQ_FLAG_STAT_CH(n)         (1 << (FC7300_MB_INT_FLG_STAT_REQ_FLAG_STAT_SHIFT + (uint32_t)(n)))
#define FC7300_MB_INT_FLG_STAT_DONE_FLAG_STAT_MASK         (0xFFFF0000u)
#define FC7300_MB_INT_FLG_STAT_DONE_FLAG_STAT_SHIFT        (16u)
#define FC7300_MB_INT_FLG_STAT_DONE_FLAG_STAT_WIDTH        (16u)
#define FC7300_MB_INT_FLG_STAT_DONE_FLAG_STAT_CH(n)        (1 << (FC7300_MB_INT_FLG_STAT_DONE_FLAG_STAT_SHIFT + (uint32_t)(n)))

/* MB_INTn_CTRL */

#define FC7300_MB_INT_CTRL_FLG_LOCK                        (1 << 0)
#define FC7300_MB_INT_CTRL_FLG_MASK_LOCK                   (1 << 1)
#define FC7300_MB_INT_CTRL_INTEN_LOCK                      (1 << 2)

#endif /* __VENDOR_FLAGCHIP_CHIPS_FC7300_HARDWARE_FC7300_MB_H */
