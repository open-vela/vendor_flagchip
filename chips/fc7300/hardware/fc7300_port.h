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

#ifndef __VENDOR_FLAGCHIP_CHIPS_FC7300_HARDWARE_FC7300_PORT_H
#define __VENDOR_FLAGCHIP_CHIPS_FC7300_HARDWARE_FC7300_PORT_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>
#include <hardware/fc7300_memorymap.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#define FC7300_PORTA               0
#define FC7300_PORTB               1
#define FC7300_PORTC               2
#define FC7300_PORTD               3
#define FC7300_PORTE               4
#define FC7300_PORTF               5
#define FC7300_PORTG               6
#define FC7300_PORTH               7
#define FC7300_PORTI               8

/* PORT Register Offsets ****************************************************/

#define FC7300_PORT_PCR_OFFSET(n)   (0 + ((n) << 2)) /* Pin Control Register n=0..31 */

#define FC7300_PORT_GPCLR_OFFSET   0x0080  /* Global Pin Control Low Register */
#define FC7300_PORT_GPCHR_OFFSET   0x0084  /* Global Pin Control High Register */
#define FC7300_PORT_GICLR_OFFSET   0x0088  /* Global Interrupt Control Low Register */
#define FC7300_PORT_GICHR_OFFSET   0x008C  /* Global Interrupt Control High Register */
#define FC7300_PORT_ISFR_OFFSET    0x00A0  /* Interrupt Status Flag Register */
#define FC7300_PORT_DFER_OFFSET    0x00C0  /* Digital Filter Enable Register */
#define FC7300_PORT_DFCR_OFFSET    0x00C4  /* Digital Filter Clock Register */
#define FC7300_PORT_DFWR_OFFSET    0x00C8  /* Digital Filter Width Register */
#define FC7300_PORT_GLDWP_OFFSET   0x00CC  /* Global Domain Write Protection Register */

/* PORT Register Addresses **************************************************/

#define FC7300_PORT_PCR_BASE(p,n)  (FC7300_PORT_BASE(p) + FC7300_PORT_PCR_OFFSET(n))
#define FC7300_PORT_GPCLR(p)       (FC7300_PORT_BASE(p) + FC7300_PORT_GPCLR_OFFSET)
#define FC7300_PORT_GPCHR(p)       (FC7300_PORT_BASE(p) + FC7300_PORT_GPCHR_OFFSET)
#define FC7300_PORT_GICLR(p)       (FC7300_PORT_BASE(p) + FC7300_PORT_GICLR_OFFSET)
#define FC7300_PORT_GICHR(p)       (FC7300_PORT_BASE(p) + FC7300_PORT_GICHR_OFFSET)
#define FC7300_PORT_ISFR(p)        (FC7300_PORT_BASE(p) + FC7300_PORT_ISFR_OFFSET)
#define FC7300_PORT_DFER(p)        (FC7300_PORT_BASE(p) + FC7300_PORT_DFER_OFFSET)
#define FC7300_PORT_DFCR(p)        (FC7300_PORT_BASE(p) + FC7300_PORT_DFCR_OFFSET)
#define FC7300_PORT_DFWR(p)        (FC7300_PORT_BASE(p) + FC7300_PORT_DFWR_OFFSET)
#define FC7300_PORT_GLDWP(p)       (FC7300_PORT_BASE(p) + FC7300_PORT_GLDWP_OFFSET)

#define FC7300_PORTA_PCR_BASE(n)   (FC7300_PORTA_BASE + FC7300_PORT_PCR_OFFSET(n))
#define FC7300_PORTA_GPCLR         (FC7300_PORTA_BASE + FC7300_PORT_GPCLR_OFFSET)
#define FC7300_PORTA_GPCHR         (FC7300_PORTA_BASE + FC7300_PORT_GPCHR_OFFSET)
#define FC7300_PORTA_GICLR         (FC7300_PORTA_BASE + FC7300_PORT_GICLR_OFFSET)
#define FC7300_PORTA_GICHR         (FC7300_PORTA_BASE + FC7300_PORT_GICHR_OFFSET)
#define FC7300_PORTA_ISFR          (FC7300_PORTA_BASE + FC7300_PORT_ISFR_OFFSET)
#define FC7300_PORTA_DFER          (FC7300_PORTA_BASE + FC7300_PORT_DFER_OFFSET)
#define FC7300_PORTA_DFCR          (FC7300_PORTA_BASE + FC7300_PORT_DFCR_OFFSET)
#define FC7300_PORTA_DFWR          (FC7300_PORTA_BASE + FC7300_PORT_DFWR_OFFSET)
#define FC7300_PORTA_GLDWP         (FC7300_PORTA_BASE + FC7300_PORT_GLDWP_OFFSET)

#define FC7300_PORTB_PCR_BASE(n)   (FC7300_PORTB_BASE + FC7300_PORT_PCR_OFFSET(n))
#define FC7300_PORTB_GPCLR         (FC7300_PORTB_BASE + FC7300_PORT_GPCLR_OFFSET)
#define FC7300_PORTB_GPCHR         (FC7300_PORTB_BASE + FC7300_PORT_GPCHR_OFFSET)
#define FC7300_PORTB_GICLR         (FC7300_PORTB_BASE + FC7300_PORT_GICLR_OFFSET)
#define FC7300_PORTB_GICHR         (FC7300_PORTB_BASE + FC7300_PORT_GICHR_OFFSET)
#define FC7300_PORTB_ISFR          (FC7300_PORTB_BASE + FC7300_PORT_ISFR_OFFSET)
#define FC7300_PORTB_DFER          (FC7300_PORTB_BASE + FC7300_PORT_DFER_OFFSET)
#define FC7300_PORTB_DFCR          (FC7300_PORTB_BASE + FC7300_PORT_DFCR_OFFSET)
#define FC7300_PORTB_DFWR          (FC7300_PORTB_BASE + FC7300_PORT_DFWR_OFFSET)
#define FC7300_PORTB_GLDWP         (FC7300_PORTB_BASE + FC7300_PORT_GLDWP_OFFSET)

#define FC7300_PORTC_PCR_BASE(n)   (FC7300_PORTC_BASE + FC7300_PORT_PCR_OFFSET(n))
#define FC7300_PORTC_GPCLR         (FC7300_PORTC_BASE + FC7300_PORT_GPCLR_OFFSET)
#define FC7300_PORTC_GPCHR         (FC7300_PORTC_BASE + FC7300_PORT_GPCHR_OFFSET)
#define FC7300_PORTC_GICLR         (FC7300_PORTC_BASE + FC7300_PORT_GICLR_OFFSET)
#define FC7300_PORTC_GICHR         (FC7300_PORTC_BASE + FC7300_PORT_GICHR_OFFSET)
#define FC7300_PORTC_ISFR          (FC7300_PORTC_BASE + FC7300_PORT_ISFR_OFFSET)
#define FC7300_PORTC_DFER          (FC7300_PORTC_BASE + FC7300_PORT_DFER_OFFSET)
#define FC7300_PORTC_DFCR          (FC7300_PORTC_BASE + FC7300_PORT_DFCR_OFFSET)
#define FC7300_PORTC_DFWR          (FC7300_PORTC_BASE + FC7300_PORT_DFWR_OFFSET)
#define FC7300_PORTC_GLDWP         (FC7300_PORTC_BASE + FC7300_PORT_GLDWP_OFFSET)

#define FC7300_PORTD_PCR_BASE(n)   (FC7300_PORTD_BASE + FC7300_PORT_PCR_OFFSET(n))
#define FC7300_PORTD_GPCLR         (FC7300_PORTD_BASE + FC7300_PORT_GPCLR_OFFSET)
#define FC7300_PORTD_GPCHR         (FC7300_PORTD_BASE + FC7300_PORT_GPCHR_OFFSET)
#define FC7300_PORTD_GICLR         (FC7300_PORTD_BASE + FC7300_PORT_GICLR_OFFSET)
#define FC7300_PORTD_GICHR         (FC7300_PORTD_BASE + FC7300_PORT_GICHR_OFFSET)
#define FC7300_PORTD_ISFR          (FC7300_PORTD_BASE + FC7300_PORT_ISFR_OFFSET)
#define FC7300_PORTD_DFER          (FC7300_PORTD_BASE + FC7300_PORT_DFER_OFFSET)
#define FC7300_PORTD_DFCR          (FC7300_PORTD_BASE + FC7300_PORT_DFCR_OFFSET)
#define FC7300_PORTD_DFWR          (FC7300_PORTD_BASE + FC7300_PORT_DFWR_OFFSET)
#define FC7300_PORTD_GLDWP         (FC7300_PORTD_BASE + FC7300_PORT_GLDWP_OFFSET)

#define FC7300_PORTE_PCR_BASE(n)   (FC7300_PORTE_BASE + FC7300_PORT_PCR_OFFSET(n))
#define FC7300_PORTE_GPCLR         (FC7300_PORTE_BASE + FC7300_PORT_GPCLR_OFFSET)
#define FC7300_PORTE_GPCHR         (FC7300_PORTE_BASE + FC7300_PORT_GPCHR_OFFSET)
#define FC7300_PORTE_GICLR         (FC7300_PORTE_BASE + FC7300_PORT_GICLR_OFFSET)
#define FC7300_PORTE_GICHR         (FC7300_PORTE_BASE + FC7300_PORT_GICHR_OFFSET)
#define FC7300_PORTE_ISFR          (FC7300_PORTE_BASE + FC7300_PORT_ISFR_OFFSET)
#define FC7300_PORTE_DFER          (FC7300_PORTE_BASE + FC7300_PORT_DFER_OFFSET)
#define FC7300_PORTE_DFCR          (FC7300_PORTE_BASE + FC7300_PORT_DFCR_OFFSET)
#define FC7300_PORTE_DFWR          (FC7300_PORTE_BASE + FC7300_PORT_DFWR_OFFSET)
#define FC7300_PORTE_GLDWP         (FC7300_PORTE_BASE + FC7300_PORT_GLDWP_OFFSET)

#define FC7300_PORTF_PCR_BASE(n)   (FC7300_PORTF_BASE + FC7300_PORT_PCR_OFFSET(n))
#define FC7300_PORTF_GPCLR         (FC7300_PORTF_BASE + FC7300_PORT_GPCLR_OFFSET)
#define FC7300_PORTF_GPCHR         (FC7300_PORTF_BASE + FC7300_PORT_GPCHR_OFFSET)
#define FC7300_PORTF_GICLR         (FC7300_PORTF_BASE + FC7300_PORT_GICLR_OFFSET)
#define FC7300_PORTF_GICHR         (FC7300_PORTF_BASE + FC7300_PORT_GICHR_OFFSET)
#define FC7300_PORTF_ISFR          (FC7300_PORTF_BASE + FC7300_PORT_ISFR_OFFSET)
#define FC7300_PORTF_DFER          (FC7300_PORTF_BASE + FC7300_PORT_DFER_OFFSET)
#define FC7300_PORTF_DFCR          (FC7300_PORTF_BASE + FC7300_PORT_DFCR_OFFSET)
#define FC7300_PORTF_DFWR          (FC7300_PORTF_BASE + FC7300_PORT_DFWR_OFFSET)
#define FC7300_PORTF_GLDWP         (FC7300_PORTF_BASE + FC7300_PORT_GLDWP_OFFSET)

#define FC7300_PORTG_PCR_BASE(n)   (FC7300_PORTG_BASE + FC7300_PORT_PCR_OFFSET(n))
#define FC7300_PORTG_GPCLR         (FC7300_PORTG_BASE + FC7300_PORT_GPCLR_OFFSET)
#define FC7300_PORTG_GPCHR         (FC7300_PORTG_BASE + FC7300_PORT_GPCHR_OFFSET)
#define FC7300_PORTG_GICLR         (FC7300_PORTG_BASE + FC7300_PORT_GICLR_OFFSET)
#define FC7300_PORTG_GICHR         (FC7300_PORTG_BASE + FC7300_PORT_GICHR_OFFSET)
#define FC7300_PORTG_ISFR          (FC7300_PORTG_BASE + FC7300_PORT_ISFR_OFFSET)
#define FC7300_PORTG_DFER          (FC7300_PORTG_BASE + FC7300_PORT_DFER_OFFSET)
#define FC7300_PORTG_DFCR          (FC7300_PORTG_BASE + FC7300_PORT_DFCR_OFFSET)
#define FC7300_PORTG_DFWR          (FC7300_PORTG_BASE + FC7300_PORT_DFWR_OFFSET)
#define FC7300_PORTG_GLDWP         (FC7300_PORTG_BASE + FC7300_PORT_GLDWP_OFFSET)

#define FC7300_PORTH_PCR_BASE(n)   (FC7300_PORTH_BASE + FC7300_PORT_PCR_OFFSET(n))
#define FC7300_PORTH_GPCLR         (FC7300_PORTH_BASE + FC7300_PORT_GPCLR_OFFSET)
#define FC7300_PORTH_GPCHR         (FC7300_PORTH_BASE + FC7300_PORT_GPCHR_OFFSET)
#define FC7300_PORTH_GICLR         (FC7300_PORTH_BASE + FC7300_PORT_GICLR_OFFSET)
#define FC7300_PORTH_GICHR         (FC7300_PORTH_BASE + FC7300_PORT_GICHR_OFFSET)
#define FC7300_PORTH_ISFR          (FC7300_PORTH_BASE + FC7300_PORT_ISFR_OFFSET)
#define FC7300_PORTH_DFER          (FC7300_PORTH_BASE + FC7300_PORT_DFER_OFFSET)
#define FC7300_PORTH_DFCR          (FC7300_PORTH_BASE + FC7300_PORT_DFCR_OFFSET)
#define FC7300_PORTH_DFWR          (FC7300_PORTH_BASE + FC7300_PORT_DFWR_OFFSET)
#define FC7300_PORTH_GLDWP         (FC7300_PORTH_BASE + FC7300_PORT_GLDWP_OFFSET)

#define FC7300_PORTI_PCR_BASE(n)   (FC7300_PORTI_BASE + FC7300_PORT_PCR_OFFSET(n))
#define FC7300_PORTI_GPCLR         (FC7300_PORTI_BASE + FC7300_PORT_GPCLR_OFFSET)
#define FC7300_PORTI_GPCHR         (FC7300_PORTI_BASE + FC7300_PORT_GPCHR_OFFSET)
#define FC7300_PORTI_GICLR         (FC7300_PORTI_BASE + FC7300_PORT_GICLR_OFFSET)
#define FC7300_PORTI_GICHR         (FC7300_PORTI_BASE + FC7300_PORT_GICHR_OFFSET)
#define FC7300_PORTI_ISFR          (FC7300_PORTI_BASE + FC7300_PORT_ISFR_OFFSET)
#define FC7300_PORTI_DFER          (FC7300_PORTI_BASE + FC7300_PORT_DFER_OFFSET)
#define FC7300_PORTI_DFCR          (FC7300_PORTI_BASE + FC7300_PORT_DFCR_OFFSET)
#define FC7300_PORTI_DFWR          (FC7300_PORTI_BASE + FC7300_PORT_DFWR_OFFSET)
#define FC7300_PORTI_GLDWP         (FC7300_PORTI_BASE + FC7300_PORT_GLDWP_OFFSET)

/* PORT Register Bitfield Definitions ***************************************/

/* Pin Control Register n=0..31 */

#define PORT_PCR_PS                 (1 << 0)  /* Bit 0:  Pull Select */
#define PORT_PCR_PULLDOWN           (0)       /*         Enable internal pulldown */
#define PORT_PCR_PULLUP             (1 << 0)  /*         Enable internal pullup */
#define PORT_PCR_PE                 (1 << 1)  /* Bit 1:  Pull Enable */
#define PORT_PCR_SRE                (1 << 2)  /* Bit 2:  Slew rate Enable */
#define PORT_PCR_PFE                (1 << 4)  /* Bit 4:  Passive Filter Enable */
#define PORT_PCR_ODE                (1 << 5)  /* Bit 5:  Open drain is enable */
#define PORT_PCR_DSE0               (1 << 6)  /* Bit 6:  Drive Strength Enable0 */
#define PORT_PCR_DSE1               (1 << 7)  /* Bit 6:  Drive Strength Enable0 */
#define PORT_PCR_MUX_SHIFT          (8)       /* Bits 8-10:  Pin Mux Control */
#define PORT_PCR_MUX_MASK           (7 << PORT_PCR_MUX_SHIFT)
#define PORT_PCR_MUX_ANALOG         (0 << PORT_PCR_MUX_SHIFT) /* Alternative 0: Pin disable/analog */
#define PORT_PCR_MUX_GPIO           (1 << PORT_PCR_MUX_SHIFT) /* Alternative 1 (GPIO) */
#define PORT_PCR_MUX_ALT2           (2 << PORT_PCR_MUX_SHIFT) /* Alternative 2 (chip-specific) */
#define PORT_PCR_MUX_ALT3           (3 << PORT_PCR_MUX_SHIFT) /* Alternative 3 (chip-specific) */
#define PORT_PCR_MUX_ALT4           (4 << PORT_PCR_MUX_SHIFT) /* Alternative 4 (chip-specific) */
#define PORT_PCR_MUX_ALT5           (5 << PORT_PCR_MUX_SHIFT) /* Alternative 5 (chip-specific) */
#define PORT_PCR_MUX_ALT6           (6 << PORT_PCR_MUX_SHIFT) /* Alternative 6 (chip-specific) */
#define PORT_PCR_MUX_ALT7           (7 << PORT_PCR_MUX_SHIFT) /* Alternative 7 (chip-specific) */

#define PORT_PCR_LK                 (1 << 15) /* Bit 15: Lock Register */
#define PORT_PCR_IRQC_SHIFT         (16)      /* Bits 16-19:  Interrupt Configuration */
#define PORT_PCR_IRQC_MASK          (15 << PORT_PCR_IRQC_SHIFT)
#define PORT_PCR_IRQC_DISABLED      (0 << PORT_PCR_IRQC_SHIFT)  /* Interrupt Status Flag (ISF) is disabled */
#define PORT_PCR_IRQC_DMARISING     (1 << PORT_PCR_IRQC_SHIFT)  /* ISF flag and DMA request on rising edge */
#define PORT_PCR_IRQC_DMAFALLING    (2 << PORT_PCR_IRQC_SHIFT)  /* ISF flag and DMA request on falling edge */
#define PORT_PCR_IRQC_DMABOTH       (3 << PORT_PCR_IRQC_SHIFT)  /* ISF flag and DMA request on either edge */
#define PORT_PCR_IRQC_ZERO          (8 << PORT_PCR_IRQC_SHIFT)  /* ISF flag and Interrupt when logic 0 */
#define PORT_PCR_IRQC_RISING        (9 << PORT_PCR_IRQC_SHIFT)  /* ISF flag and Interrupt on rising-edge */
#define PORT_PCR_IRQC_FALLING       (10 << PORT_PCR_IRQC_SHIFT) /* ISF flag and Interrupt on falling-edge */
#define PORT_PCR_IRQC_BOTH          (11 << PORT_PCR_IRQC_SHIFT) /* ISF flag and Interrupt on either edge */
#define PORT_PCR_IRQC_ONE           (12 << PORT_PCR_IRQC_SHIFT) /* ISF flag and Interrupt when logic 1 */

#define PORT_PCR_ISF                (1 << 24)  /* Bit 24: Interrupt Status Flag */
#define PORT_PCR_DWP_MASK           (31 << 25) /* Bit 25:29: Domain Write Protection */

/* Global Pin Control Low Register */

#define PORT_GPCLR_GPWD_SHIFT       (0)       /* Bits 0-15: Global Pin Write Data */
#define PORT_GPCLR_GPWD_MASK        (0xffff << PORT_GPCLR_GPWD_SHIFT)
#define PORT_GPCLR_GPWD_PIN(n)      ((uint32_t)(n) << PORT_GPCLR_GPWD_SHIFT) /* Pin n=0..15 */

#define PORT_GPCLR_GPWE_SHIFT       (16)      /* Bits 16-31: Global Pin Write Enable */
#define PORT_GPCLR_GPWE_MASK        (0xffff << PORT_GPCLR_GPWE_SHIFT)
#define PORT_GPCLR_GPWE_PIN(n)      ((uint32_t)(n) << PORT_GPCLR_GPWE_SHIFT) /* Pin n=0..15 */

/* Global Pin Control High Register */

#define PORT_GPCHR_GPWD_SHIFT       (0)       /* Bits 0-15: Global Pin Write Data */
#define PORT_GPCHR_GPWD_MASK        (0xffff << PORT_GPCHR_GPWD_SHIFT)
#define PORT_GPCHR_GPWD_PIN(n)      ((uint32_t)((n) - 16) << PORT_GPCHR_GPWD_SHIFT) /* Pin n=16..31 */

#define PORT_GPCHR_GPWE_SHIFT       (16)      /* Bits 16-31: Global Pin Write Enable */
#define PORT_GPCHR_GPWE_MASK        (0xffff << PORT_GPCHR_GPWE_SHIFT)
#define PORT_GPCHR_GPWE_PIN(n)      ((uint32_t)((n) - 16) << PORT_GPCHR_GPWE_SHIFT) /* Pin n=16..31 */

/* Global Interrupt Control Low Register */

#define PORT_GICLR_GIWD_SHIFT       (0)       /* Bits 0-15: Global Interrupt Write Data */
#define PORT_GICLR_GIWD_MASK        (0xffff << PORT_GICLR_GIWD_SHIFT)
#define PORT_GICLR_GIWD_PIN(n)      ((uint32_t)(n) << PORT_GICLR_GIWD_SHIFT) /* Pin n=0..15 */

#define PORT_GICLR_GIWE_SHIFT       (16)      /* Bits 16-31: Global Interrupt Write Enable */
#define PORT_GICLR_GIWE_MASK        (0xffff << PORT_GICLR_GIWE_SHIFT)
#define PORT_GICLR_GIWE_PIN(n)      ((uint32_t)(n) << PORT_GICLR_GIWE_SHIFT) /* Pin n=0..15 */

/* Global Interrupt Control High Register */

#define PORT_GICHR_GIWD_SHIFT       (0)       /* Bits 0-15: Global Interrupt Write Data */
#define PORT_GICHR_GIWD_MASK        (0xffff << PORT_GICHR_GIWD_SHIFT)
#define PORT_GICHR_GIWD_PIN(n)      ((uint32_t)((n) - 16) << PORT_GICHR_GIWD_SHIFT) /* Pin n=16..31 */

#define PORT_GICHR_GIWE_SHIFT       (16)      /* Bits 16-31: Global Interrupt Write Enable */
#define PORT_GICHR_GIWE_MASK        (0xffff << PORT_GICHR_GIWE_SHIFT)
#define PORT_GICHR_GIWE_PIN(n)      ((uint32_t)((n) - 16) << PORT_GICHR_GIWE_SHIFT) /* Pin n=16..31 */

/* Interrupt Status Flag Register */

#define PORT_ISFR(n)                (1 << (n))  /* Interrupt Status Flag, n=0-31 */

/* Digital Filter Enable Register */

#define PORT_DFER(n)                (1 << (n))  /* Digital Filter Enable, n=0-31 */

/* Digital Filter Clock Register */

#define PORT_DFCR_CS                (1 << 0)  /* Bit 0:  Clock Source */
#define PORT_DFCR_BUSCLK            (0 << 0)  /*         Digital filters clocked by bus clock */
#define PORT_DFCR_LPOPCLK           (1 << 0)  /*         Digital filters clocked by AON32K clock */

/* Digital Filter Width Register */

#define PORT_DFWR_FILT_SHIFT        (0)       /* Bits 0-4: Filter Length */
#define PORT_DFWR_FILT_MASK         (31 << PORT_DFWR_FILT_SHIFT)
#define PORT_DFWR_FILT(n)           ((uint32_t)(n) << PORT_DFWR_FILT_SHIFT)

#endif /* __VENDOR_FLAGCHIP_CHIPS_FC7300_HARDWARE_FC7300_PORT_H */
