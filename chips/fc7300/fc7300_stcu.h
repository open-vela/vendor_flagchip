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

#ifndef __VENDOR_FLAGCHIP_CHIPS_FC7300_FC7300_STCU_H
#define __VENDOR_FLAGCHIP_CHIPS_FC7300_FC7300_STCU_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>
#include <stdint.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#define LBIST_RESULT_MASK  (0x3u << 0u)
#define MBIST_RESULT_MASK  (0x3u << 2u)
#define STCU_LBIST_FAIL_FLAG     0x02u
#define STCU_LBIST_SUCCESS_FLAG  0x01u
#define STCU_LBIST_DISABLE_FLAG  0x00u
#define STCU_MBIST_SUCCESS_FLAG  0x04u
#define STCU_MBIST_FAIL_FLAG     0x08u
#define STCU_MBIST_DISABLE_FLAG  0x00u

/* STCU self test status flag */

typedef enum
{
  STCU_SELFTEST_NONE            = 0x00u,   /* No any status flag */
  STCU_SELFTEST_DONE            = 0x01u,   /* Self-test Done Flag */
  STCU_SELFTEST_ABORT           = 0x02u,   /* Self-test Abort Flag */
  STCU_SELFTEST_LBIST_ERROR     = 0x10u,   /* LBIST Error Flag */
  STCU_SELFTEST_MBIST_ERROR     = 0x20u,   /* MBIST Error Flag */
  STCU_SELFTEST_TIMEOUT_ERROR   = 0x40u,   /* Self-test Timeout Error Flag */
  STCU_SELFTEST_NVRLOAD_ERROR   = 0x100u,  /* NVR Load Error Flag */
  STCU_SELFTEST_SELFCHECK_ERROR = 0x200u,  /* STCU Self Check Error Flag */
}stcu_selftest_status;

typedef enum
{
  STCU_MBIST_FAIL_NONE        = 0x00u,        /* No any MBIST fail flag. */
  STCU_MBIST_FAIL_SRAM0       = 0x01u,        /* SRAM0 SRAM MBIST Fail Flag. */
  STCU_MBIST_FAIL_SRAM1       = 0x02u,        /* SRAM1 SRAM MBIST Fail Flag. */
  STCU_MBIST_FAIL_SRAM2_1     = 0x04u,        /* SRAM2_1 SRAM MBIST Fail Flag. */
  STCU_MBIST_FAIL_SRAM2_2     = 0x08u,        /* SRAM2_2 SRAM MBIST Fail Flag. */
  STCU_MBIST_FAIL_ITCM_CPU0   = 0x10u,        /* ITCM_CPU0 SRAM MBIST Fail Flag. */
  STCU_MBIST_FAIL_DTCM0_CPU0  = 0x20u,        /* DTCM0_CPU0 SRAM MBIST Fail Flag. */
  STCU_MBIST_FAIL_DTCM1_CPU0  = 0x40u,        /* DTCM1_CPU0 SRAM MBIST Fail Flag. */
  STCU_MBIST_FAIL_ITCM_CPU1   = 0x80u,        /* ITCM_CPU1 SRAM MBIST Fail Flag. */
  STCU_MBIST_FAIL_DTCM0_CPU1  = 0x100u,       /* DTCM0_CPU1 SRAM MBIST Fail Flag. */
  STCU_MBIST_FAIL_DTCM1_CPU1  = 0x200u,       /* DTCM1_CPU1 SRAM MBIST Fail Flag. */
  STCU_MBIST_FAIL_ITCM_CPU2   = 0x400u,       /* ITCM_CPU2 SRAM MBIST Fail Flag. */
  STCU_MBIST_FAIL_DTCM0_CPU2  = 0x800u,       /* DTCM0_CPU2 SRAM MBIST Fail Flag. */
  STCU_MBIST_FAIL_DTCM1_CPU2  = 0x1000u,      /* DTCM1_CPU2 SRAM MBIST Fail Flag. */
  STCU_MBIST_FAIL_CACHE_CPU0  = 0x2000u,      /* CACHE_CPU0 SRAM MBIST Fail Flag. */
  STCU_MBIST_FAIL_CACHE_CPU1  = 0x4000u,      /* CACHE_CPU1 SRAM MBIST Fail Flag. */
  STCU_MBIST_FAIL_CACHE_CPU2  = 0x8000u,      /* CACHE_CPU2 SRAM MBIST fail flag. */
  STCU_MBIST_FAIL_SRAM_HSM    = 0x10000u,     /* HSM SRAM MBIST fail flag. */
  STCU_MBIST_FAIL_SRAM_DMACAN = 0x20000u,     /* DMA_FLEXCAN SRAM MBIST fail flag. */
  STCU_MBIST_FAIL_SRAM_ENET   = 0x40000u,     /* ENET SRAM MBIST fail flag. */
  STCU_MBIST_FAIL_SRAM_ETB    = 0x80000u,     /* ETB SRAM MBIST Fail Flag. */
  STCU_MBIST_FAIL_ROM_HOST    = 0x100000u,    /* System ROM MBIST Fail Flag. */
  STCU_MBIST_FAIL_ROM_HSM     = 0x200000u     /* HSM ROM MBIST Fail Flag. */
}stcu_mbistfailed_type;

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

/****************************************************************************
 * Name: fc7300_get_stcu_selftest_result
 *
 * Description:
 *   Get the stcu selftest result
 *
 * Input Parameters:
 *   None
 *
 * Returned Value:
 *   The self-test result
 *
 ****************************************************************************/

uint32_t fc7300_get_stcu_selftest_result(void);

#ifndef __ASSEMBLY__

#undef EXTERN
#if defined(__cplusplus)
#define EXTERN extern "C"
extern "C"
{
#else
#define EXTERN extern
#endif

#undef EXTERN
#if defined(__cplusplus)
}
#endif

#endif /* __ASSEMBLY__ */
#endif /* __VENDOR_FLAGCHIP_CHIPS_FC7300_FC7300_STCU_H */