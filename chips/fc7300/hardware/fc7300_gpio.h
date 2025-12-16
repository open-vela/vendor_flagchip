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

#ifndef __VENDOR_FLAGCHIP_CHIPS_FC7300_HARDWARE_FC7300_GPIO_H
#define __VENDOR_FLAGCHIP_CHIPS_FC7300_HARDWARE_FC7300_GPIO_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>
#include <hardware/fc7300_memorymap.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* GPIO Register Offsets ****************************************************/

#define FC7300_GPIO_PDOR_OFFSET     0x0000  /* Port Data Output Register */
#define FC7300_GPIO_PSOR_OFFSET     0x0004  /* Port Set Output Register */
#define FC7300_GPIO_PCOR_OFFSET     0x0008  /* Port Clear Output Register */
#define FC7300_GPIO_PTOR_OFFSET     0x000c  /* Port Toggle Output Register */
#define FC7300_GPIO_PDIR_OFFSET     0x0010  /* Port Data Input Register */
#define FC7300_GPIO_PDDR_OFFSET     0x0014  /* Port Data Direction Register */
#define FC7300_GPIO_PIDR_OFFSET     0x0018  /* Port Input Disable Register */

/* GPIO Register Addresses **************************************************/

#define FC7300_GPIO_PDOR(g)        (FC7300_GPIO_BASE(g) + FC7300_GPIO_PDOR_OFFSET)
#define FC7300_GPIO_PSOR(g)        (FC7300_GPIO_BASE(g) + FC7300_GPIO_PSOR_OFFSET)
#define FC7300_GPIO_PCOR(g)        (FC7300_GPIO_BASE(g) + FC7300_GPIO_PCOR_OFFSET)
#define FC7300_GPIO_PTOR(g)        (FC7300_GPIO_BASE(g) + FC7300_GPIO_PTOR_OFFSET)
#define FC7300_GPIO_PDIR(g)        (FC7300_GPIO_BASE(g) + FC7300_GPIO_PDIR_OFFSET)
#define FC7300_GPIO_PDDR(g)        (FC7300_GPIO_BASE(g) + FC7300_GPIO_PDDR_OFFSET)
#define FC7300_GPIO_PIDR(g)        (FC7300_GPIO_BASE(g) + FC7300_GPIO_PIDR_OFFSET)

#define FC7300_GPIOA_PDOR          (FC7300_GPIOA_BASE + FC7300_GPIO_PDOR_OFFSET)
#define FC7300_GPIOA_PSOR          (FC7300_GPIOA_BASE + FC7300_GPIO_PSOR_OFFSET)
#define FC7300_GPIOA_PCOR          (FC7300_GPIOA_BASE + FC7300_GPIO_PCOR_OFFSET)
#define FC7300_GPIOA_PTOR          (FC7300_GPIOA_BASE + FC7300_GPIO_PTOR_OFFSET)
#define FC7300_GPIOA_PDIR          (FC7300_GPIOA_BASE + FC7300_GPIO_PDIR_OFFSET)
#define FC7300_GPIOA_PDDR          (FC7300_GPIOA_BASE + FC7300_GPIO_PDDR_OFFSET)
#define FC7300_GPIOA_PIDR          (FC7300_GPIOA_BASE + FC7300_GPIO_PIDR_OFFSET)

#define FC7300_GPIOB_PDOR          (FC7300_GPIOB_BASE + FC7300_GPIO_PDOR_OFFSET)
#define FC7300_GPIOB_PSOR          (FC7300_GPIOB_BASE + FC7300_GPIO_PSOR_OFFSET)
#define FC7300_GPIOB_PCOR          (FC7300_GPIOB_BASE + FC7300_GPIO_PCOR_OFFSET)
#define FC7300_GPIOB_PTOR          (FC7300_GPIOB_BASE + FC7300_GPIO_PTOR_OFFSET)
#define FC7300_GPIOB_PDIR          (FC7300_GPIOB_BASE + FC7300_GPIO_PDIR_OFFSET)
#define FC7300_GPIOB_PDDR          (FC7300_GPIOB_BASE + FC7300_GPIO_PDDR_OFFSET)
#define FC7300_GPIOB_PIDR          (FC7300_GPIOB_BASE + FC7300_GPIO_PIDR_OFFSET)

#define FC7300_GPIOC_PDOR          (FC7300_GPIOC_BASE + FC7300_GPIO_PDOR_OFFSET)
#define FC7300_GPIOC_PSOR          (FC7300_GPIOC_BASE + FC7300_GPIO_PSOR_OFFSET)
#define FC7300_GPIOC_PCOR          (FC7300_GPIOC_BASE + FC7300_GPIO_PCOR_OFFSET)
#define FC7300_GPIOC_PTOR          (FC7300_GPIOC_BASE + FC7300_GPIO_PTOR_OFFSET)
#define FC7300_GPIOC_PDIR          (FC7300_GPIOC_BASE + FC7300_GPIO_PDIR_OFFSET)
#define FC7300_GPIOC_PDDR          (FC7300_GPIOC_BASE + FC7300_GPIO_PDDR_OFFSET)
#define FC7300_GPIOC_PIDR          (FC7300_GPIOC_BASE + FC7300_GPIO_PIDR_OFFSET)

#define FC7300_GPIOD_PDOR          (FC7300_GPIOD_BASE + FC7300_GPIO_PDOR_OFFSET)
#define FC7300_GPIOD_PSOR          (FC7300_GPIOD_BASE + FC7300_GPIO_PSOR_OFFSET)
#define FC7300_GPIOD_PCOR          (FC7300_GPIOD_BASE + FC7300_GPIO_PCOR_OFFSET)
#define FC7300_GPIOD_PTOR          (FC7300_GPIOD_BASE + FC7300_GPIO_PTOR_OFFSET)
#define FC7300_GPIOD_PDIR          (FC7300_GPIOD_BASE + FC7300_GPIO_PDIR_OFFSET)
#define FC7300_GPIOD_PDDR          (FC7300_GPIOD_BASE + FC7300_GPIO_PDDR_OFFSET)
#define FC7300_GPIOD_PIDR          (FC7300_GPIOD_BASE + FC7300_GPIO_PIDR_OFFSET)

#define FC7300_GPIOE_PDOR          (FC7300_GPIOE_BASE + FC7300_GPIO_PDOR_OFFSET)
#define FC7300_GPIOE_PSOR          (FC7300_GPIOE_BASE + FC7300_GPIO_PSOR_OFFSET)
#define FC7300_GPIOE_PCOR          (FC7300_GPIOE_BASE + FC7300_GPIO_PCOR_OFFSET)
#define FC7300_GPIOE_PTOR          (FC7300_GPIOE_BASE + FC7300_GPIO_PTOR_OFFSET)
#define FC7300_GPIOE_PDIR          (FC7300_GPIOE_BASE + FC7300_GPIO_PDIR_OFFSET)
#define FC7300_GPIOE_PDDR          (FC7300_GPIOE_BASE + FC7300_GPIO_PDDR_OFFSET)
#define FC7300_GPIOE_PIDR          (FC7300_GPIOE_BASE + FC7300_GPIO_PIDR_OFFSET)

#define FC7300_GPIOF_PDOR          (FC7300_GPIOF_BASE + FC7300_GPIO_PDOR_OFFSET)
#define FC7300_GPIOF_PSOR          (FC7300_GPIOF_BASE + FC7300_GPIO_PSOR_OFFSET)
#define FC7300_GPIOF_PCOR          (FC7300_GPIOF_BASE + FC7300_GPIO_PCOR_OFFSET)
#define FC7300_GPIOF_PTOR          (FC7300_GPIOF_BASE + FC7300_GPIO_PTOR_OFFSET)
#define FC7300_GPIOF_PDIR          (FC7300_GPIOF_BASE + FC7300_GPIO_PDIR_OFFSET)
#define FC7300_GPIOF_PDDR          (FC7300_GPIOF_BASE + FC7300_GPIO_PDDR_OFFSET)
#define FC7300_GPIOF_PIDR          (FC7300_GPIOF_BASE + FC7300_GPIO_PIDR_OFFSET)

#define FC7300_GPIOG_PDOR          (FC7300_GPIOG_BASE + FC7300_GPIO_PDOR_OFFSET)
#define FC7300_GPIOG_PSOR          (FC7300_GPIOG_BASE + FC7300_GPIO_PSOR_OFFSET)
#define FC7300_GPIOG_PCOR          (FC7300_GPIOG_BASE + FC7300_GPIO_PCOR_OFFSET)
#define FC7300_GPIOG_PTOR          (FC7300_GPIOG_BASE + FC7300_GPIO_PTOR_OFFSET)
#define FC7300_GPIOG_PDIR          (FC7300_GPIOG_BASE + FC7300_GPIO_PDIR_OFFSET)
#define FC7300_GPIOG_PDDR          (FC7300_GPIOG_BASE + FC7300_GPIO_PDDR_OFFSET)
#define FC7300_GPIOG_PIDR          (FC7300_GPIOG_BASE + FC7300_GPIO_PIDR_OFFSET)

#define FC7300_GPIOH_PDOR          (FC7300_GPIOH_BASE + FC7300_GPIO_PDOR_OFFSET)
#define FC7300_GPIOH_PSOR          (FC7300_GPIOH_BASE + FC7300_GPIO_PSOR_OFFSET)
#define FC7300_GPIOH_PCOR          (FC7300_GPIOH_BASE + FC7300_GPIO_PCOR_OFFSET)
#define FC7300_GPIOH_PTOR          (FC7300_GPIOH_BASE + FC7300_GPIO_PTOR_OFFSET)
#define FC7300_GPIOH_PDIR          (FC7300_GPIOH_BASE + FC7300_GPIO_PDIR_OFFSET)
#define FC7300_GPIOH_PDDR          (FC7300_GPIOH_BASE + FC7300_GPIO_PDDR_OFFSET)
#define FC7300_GPIOH_PIDR          (FC7300_GPIOH_BASE + FC7300_GPIO_PIDR_OFFSET)

#define FC7300_GPIOI_PDOR          (FC7300_GPIOI_BASE + FC7300_GPIO_PDOR_OFFSET)
#define FC7300_GPIOI_PSOR          (FC7300_GPIOI_BASE + FC7300_GPIO_PSOR_OFFSET)
#define FC7300_GPIOI_PCOR          (FC7300_GPIOI_BASE + FC7300_GPIO_PCOR_OFFSET)
#define FC7300_GPIOI_PTOR          (FC7300_GPIOI_BASE + FC7300_GPIO_PTOR_OFFSET)
#define FC7300_GPIOI_PDIR          (FC7300_GPIOI_BASE + FC7300_GPIO_PDIR_OFFSET)
#define FC7300_GPIOI_PDDR          (FC7300_GPIOI_BASE + FC7300_GPIO_PDDR_OFFSET)
#define FC7300_GPIOI_PIDR          (FC7300_GPIOI_BASE + FC7300_GPIO_PIDR_OFFSET)

/* GPIO Register Bitfield Definitions ***************************************/

/* Port Data Output Register */

#define GPIO_PDOR(n)                (1 << (n))  /* Pin n data output, n=0..31 */

/* Port Set Output Register */

#define GPIO_PSOR(n)                (1 << (n))  /* Pin n set output, n=0..31 */

/* Port Clear Output Register */

#define GPIO_PCOR(n)                (1 << (n))  /* Pin n clear output, n=0..31 */

/* Port Toggle Output Register */

#define GPIO_PTOR(n)                (1 << (n))  /* Pin n toggle output, n=0..31 */

/* Port Data Input Register */

#define GPIO_PDIR(n)                (1 << (n))  /* Pin n data input, n=0..31 */

/* Port Data Direction Register */

#define GPIO_PDDR(n)                (1 << (n))  /* Pin n data direction, n=0..31 */

/* Port Input Disable Register */

#define GPIO_PIDR(n)                (1 << (n))  /* Pin n input disable, n=0..31 */

#endif /* __VENDOR_FLAGCHIP_CHIPS_FC7300_HARDWARE_FC7300_GPIO_H */
