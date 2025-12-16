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

#include <stdint.h>
#include <assert.h>
#include <debug.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <inttypes.h>
#include <errno.h>

#include <nuttx/irq.h>
#include <nuttx/arch.h>
#include <nuttx/init.h>
#include <nuttx/rwsem.h>
#include <nuttx/mtd/mtd.h>
#include <nuttx/spinlock.h>

#include "nvic.h"
#include "arm_internal.h"

#include "hardware/fc7300_fmc.h"
#include "fc7300_norflash_mtd.h"
#include "fc7300_partition.h"

#if defined(CONFIG_FC7300_FCSMU_CHANNEL_FLASH_ECC)
#include "hardware/fc7300_fcsmu.h"
#include "fc7300_irq.h"
#endif

#if defined(CONFIG_FC7300_ERM_DFLASH_ECC_MODE_M) || defined(CONFIG_FC7300_ERM_DFLASH_ECC_MODE_A)
#include "hardware/fc7300_erm.h"
#include "fc7300_irq.h"
#endif

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#if defined(CONFIG_FC7300_FCSMU_CHANNEL_FLASH_ECC)
#define FCSMU_OP_STATE_SUCCESSFUL                        0X3U
#define FCSMU_FST_UNLOCK_KEY                             0x951413CFUL
#define FCSMU_FAULT_CHANNEL_FLASH_ECC_ERROR              0X200000U
#endif

/****************************************************************************
 * Private Types
 ****************************************************************************/

typedef struct
{
  uint32_t blk_sel;
  uint32_t dest;
} flash_drv_erase_sector_cfg_t;

typedef struct
{
  uint32_t dest;
  uint32_t size;
  uint32_t *data;
  uint32_t wdg_tune;
  uint32_t pgff;
} flash_drv_prgm_cfg_t;

#ifdef CONFIG_FC7300_DFLASH_INJECT_ECC
typedef struct
{
  uint32_t data1;
  uint32_t data2;
  uint32_t data3;
  uint32_t data4;
  uint32_t data5;
  uint32_t size;
  uint32_t dest;
} flash_drv_ecc_cfg_t;
#endif

typedef struct
{
  uint32_t RESERVED1[3U];
  uint32_t (*erase_sector)
           (flash_drv_erase_sector_cfg_t * flash_api_cfg,
            uint32_t int_en, uint32_t type);
  uint32_t (*erase_sector_clear)(void);
  uint32_t (*erase_nvr)(int32_t int_en);
  uint32_t (*erase_nvr_clear)(void);
  uint32_t (*program)(flash_drv_prgm_cfg_t * flash_api_cfg,
                                uint32_t int_en, uint32_t type);
  uint32_t (*program_clear)(void);
  uint32_t (*program_nvr)(flash_drv_prgm_cfg_t * flash_api_cfg,
                                uint32_t int_en);
  uint32_t (*program_nvr_clear)(void);
  uint32_t RESERVED3[4U];
  uint32_t (*hv_status_check)(void);
  uint32_t RESERVED4[5U];
#ifdef CONFIG_FC7300_DFLASH_INJECT_ECC
  uint32_t (*ecc_check_type)(flash_drv_ecc_cfg_t * flash_api_cfg,
                             uint32_t size);
  uint32_t (*test_mode_close_type)(void);
#else
  uint32_t RESERVED5[2U];
#endif
  uint32_t RESERVED6[1U];
  uint32_t (*enable_hold)(uint32_t  flash_api_cfg);
  uint32_t RESERVED7[3U];
#ifdef CONFIG_FC7300_DFLASH_INJECT_ECC
  uint32_t (*test_mode_open_type)(void);
#else
  uint32_t RESERVED8[1U];
#endif
  uint32_t RESERVED9[2U];
  uint32_t (*read_dflash_status)(uint32_t flash_api_cfg);
  uint32_t RESERVED10[1U];
  uint32_t (*clear_dflash_status)(uint32_t flash_api_cfg);
}flash_rom_api_entry_t;

struct flash_inf_s
{
  uint32_t start_address;
  uint32_t sector_count;
};

enum flash_type_e
{
  FC7300_FLASH_DFLASH  = 0,
  FC7300_FLASH_PFLASH  = 1,
  FC7300_FLASH_NVR     = 2,
};

enum fc7300_flashstate_e
{
  FC7300_FLASH_IDLE  = 0,    /* DMA is allocated, but not yet configured */
  FC7300_FLASH_ERASE = 1,    /* DMA configured, but not yet started or has stopped */
  FC7300_FLASH_WRITE = 2,    /* DMA has been started and is in progress */
};

/* FC7300 Nor Flash device private data  */

struct fc7300_mtd_dev_s
{
  struct mtd_dev_s mtd;

  /* NOR Flash start address */

  enum flash_type_e flash_type;
};

/****************************************************************************
 * Private Data
 ****************************************************************************/

static const flash_rom_api_entry_t  *const
  s_flash_driver_func_header = (flash_rom_api_entry_t  *)FLASHDRV_ADDR;
/****************************************************************************
 * Private Functions Prototypes
 ****************************************************************************/

/* MTD driver methods */

static int fc7300_flash_erase(struct mtd_dev_s *dev, off_t startblock,
                         size_t nblocks);
static ssize_t fc7300_flash_read(struct mtd_dev_s *dev, off_t offset,
                            size_t nbytes, uint8_t *buffer);
static ssize_t fc7300_flash_bread(struct mtd_dev_s *dev, off_t startblock,
                             size_t nblocks, uint8_t *buffer);
static ssize_t fc7300_flash_write(struct mtd_dev_s *dev, off_t offset,
                             size_t nbytes, const uint8_t *buffer);
static ssize_t fc7300_flash_bwrite(struct mtd_dev_s *dev, off_t startblock,
                              size_t nblocks, const uint8_t *buffer);
static int fc7300_flash_ioctl(struct mtd_dev_s *dev, int cmd,
                         unsigned long arg);

/****************************************************************************
 * Private Data
 ****************************************************************************/

static enum fc7300_flashstate_e g_flash_opt_state = FC7300_FLASH_IDLE;
static bool g_fc7300_eccfault; /* ECC hard fault flag */
static rw_semaphore_t g_fc7300_flash_rwlock;
static uint32_t g_fc7300_flash_opt_addr;
static uint32_t g_fc7300_flash_rwlock_initialized = 0;
static const struct fc7300_mtd_dev_s g_fc7300_nordflash =
{
  .mtd =
          {
            .erase  = fc7300_flash_erase,
            .bread  = fc7300_flash_bread,
            .bwrite = fc7300_flash_bwrite,
            .read   = fc7300_flash_read,
            .ioctl  = fc7300_flash_ioctl,
#ifdef CONFIG_MTD_BYTE_WRITE
            .write  = fc7300_flash_write,
#endif
            .name   = "fc7300_nordflash"
          },
  .flash_type = FC7300_FLASH_DFLASH,
};

static const struct fc7300_mtd_dev_s g_fc7300_norpflash =
{
  .mtd =
          {
            .erase  = fc7300_flash_erase,
            .bread  = fc7300_flash_bread,
            .bwrite = fc7300_flash_bwrite,
            .read   = fc7300_flash_read,
            .ioctl  = fc7300_flash_ioctl,
#ifdef CONFIG_MTD_BYTE_WRITE
            .write  = fc7300_flash_write,
#endif
            .name   = "fc7300_norpflash"
          },
  .flash_type = FC7300_FLASH_PFLASH,
};

static const struct fc7300_mtd_dev_s g_fc7300_nornvr =
{
  .mtd =
          {
            .erase  = fc7300_flash_erase,
            .bread  = fc7300_flash_bread,
            .bwrite = fc7300_flash_bwrite,
            .read   = fc7300_flash_read,
            .ioctl  = fc7300_flash_ioctl,
#ifdef CONFIG_MTD_BYTE_WRITE
            .write  = fc7300_flash_write,
#endif
            .name   = "fc7300_nornvr"
          },
  .flash_type = FC7300_FLASH_NVR,
};

/****************************************************************************
 * Private Functions
 ****************************************************************************/

static void fc7300_dflash_unlock(void)
{
  /* 1 bank contains only 256KB, only used FB_FPELCK */

  putreg32(0, FC7300_FB_FPELCK4);
}

static void fc7300_dflash_lock(void)
{
  /* 1 bank contains only 256KB, only used FB_FPELCK */

  putreg32(0xFFFFFFFF, FC7300_FB_FPELCK4);
}

static void fc7300_nvr_unlock(void)
{
  /* 1 bank contains only 8KB, only used FN_FPELCK */

  putreg32(0, FC7300_FN_FPELCK);
}

static void fc7300_nvr_lock(void)
{
  /* 1 bank contains only 8KB, only used FN_FPELCK */

  putreg32(0xFFFFFFFF, FC7300_FN_FPELCK);
}

static void fc7300_pflash_unlock(uint32_t addr)
{
  uint32_t index;
  uint32_t length;
  uint32_t temp;

  /* 1 bank contains only 256KB, only used FB_FPELCK */

  index = (addr - FC7300_PFLASH_START_ADDR) / FC7300_PFLASH_BANK_SIZE;
  length = ((addr - FC7300_PFLASH_START_ADDR) % FC7300_PFLASH_BANK_SIZE);

  if (length < (FC7300_PFLASH_BANK_SIZE - FC7300_PFLASH_256KB_SIZE))
    {
      temp = ((addr - FC7300_PFLASH_START_ADDR -
               FC7300_PFLASH_BANK_SIZE * index) >> 16);
      temp = ((uint32_t)1UL << temp);
      temp = 0xffffffff ^ temp;
      putreg32(temp, FC7300_FB_CPELCK0 + 4*index);
    }
  else
    {
      temp = ((addr - FC7300_PFLASH_START_ADDR -
               FC7300_PFLASH_BANK_SIZE * index -
               FC7300_PFLASH_LAST_256KB_START) >> 13);
      temp = ((uint32_t)1UL << temp);
      temp = 0xffffffff ^ temp;
      putreg32(temp, FC7300_FB_FPELCK0 + 4*index);
    }
}

static void fc7300_pflash_lock(uint32_t addr)
{
  uint32_t index;
  uint32_t length;
  uint32_t temp;

  /* 1 bank contains only 256KB, only used FB_FPELCK */

  index = (addr - FC7300_PFLASH_START_ADDR) / FC7300_PFLASH_BANK_SIZE;
  length = ((addr - FC7300_PFLASH_START_ADDR) % FC7300_PFLASH_BANK_SIZE);

  if (length < (FC7300_PFLASH_BANK_SIZE - FC7300_PFLASH_256KB_SIZE))
    {
      temp = ((addr - FC7300_PFLASH_START_ADDR -
               FC7300_PFLASH_BANK_SIZE * index) >> 16);
      temp = ((uint32_t)1UL << temp);
      temp = getreg32(FC7300_FB_CPELCK0 + 4*index) | temp;
      putreg32(temp, FC7300_FB_CPELCK0 + 4*index);
    }
  else
    {
      temp = ((addr - FC7300_PFLASH_START_ADDR -
               FC7300_PFLASH_BANK_SIZE * index -
               FC7300_PFLASH_LAST_256KB_START) >> 13);
      temp = ((uint32_t)1UL << temp);
      temp = getreg32(FC7300_FB_CPELCK0 + 4*index) | temp;
      putreg32(temp, FC7300_FB_FPELCK0 + 4*index);
    }
}

static uint32_t pflash_getblkn(uint32_t addr)
{
  uint32_t nb;

  if (addr < FC7300_PFLASH_START_ADDR + FC7300_PFLASH_BANK_SIZE)
    {
      nb = 0;
    }
  else if (addr < FC7300_PFLASH_START_ADDR + FC7300_PFLASH_BANK_SIZE * 2)
    {
      nb = 1;
    }
  else if (addr < FC7300_PFLASH_START_ADDR + FC7300_PFLASH_BANK_SIZE * 3)
    {
      nb = 2;
    }
  else
    {
      nb = 3;
    }

  return nb;
}

static int flash_erase(off_t startblock, size_t nblocks,
                        enum flash_type_e type)
{
  flash_drv_erase_sector_cfg_t flash_api_cfg;
  irqstate_t irqflags;
  uint32_t temp;
  uint32_t index;
  uint32_t try_count;
  uint32_t addr;

  if (type == FC7300_FLASH_DFLASH)
    {
      addr = startblock * FC7300_FLASH_SECTOR_SIZE +
            FC7300_DFLASH_START_ADDR;
    }
  else if (type == FC7300_FLASH_PFLASH)
    {
      addr = startblock * FC7300_FLASH_SECTOR_SIZE +
            FC7300_PFLASH_START_ADDR;
    }
  else if (type == FC7300_FLASH_NVR)
    {
      addr = startblock * FC7300_FLASH_SECTOR_SIZE +
            FC7300_NVR_START_ADDR;
    }
  else
    {
      return -1;
    }

  for (index = 0U; index < nblocks; index++)
    {
      flash_api_cfg.dest = addr;

      if (type == FC7300_FLASH_DFLASH)
        {
          flash_api_cfg.blk_sel = 4UL;
          fc7300_dflash_unlock();
        }
      else if (type == FC7300_FLASH_PFLASH)
        {
          flash_api_cfg.blk_sel = pflash_getblkn(addr);
          fc7300_pflash_unlock(addr);
        }
      else
        {
          fc7300_nvr_unlock();
        }

      irqflags = enter_critical_section();
      if (g_flash_opt_state == FC7300_FLASH_IDLE)
        {
          if (type == FC7300_FLASH_NVR)
            {
              FC7300_ENABLE_HOLD(1);
              temp = FC7300_ERASE_NVR(0);
              FC7300_ENABLE_HOLD(0);
            }
          else
            {
              temp = FC7300_ERASE(&flash_api_cfg, 0, 0);
            }

          g_flash_opt_state = FC7300_FLASH_ERASE;
          g_fc7300_flash_opt_addr = addr;
          leave_critical_section(irqflags);
        }
      else
        {
          if (type == FC7300_FLASH_DFLASH)
            {
              fc7300_dflash_lock();
            }
          else if (type == FC7300_FLASH_PFLASH)
            {
              fc7300_pflash_lock(addr);
            }
          else
            {
              fc7300_nvr_lock();
            }

          leave_critical_section(irqflags);
          return -1;
        }

      if (temp != STATUS_SUCCESS)
        {
          return -1;
        }

      temp = FC7300_HV_CHECK();
      try_count = 0;
      while ((STATUS_HVOP == temp) && (try_count++ < 1000))
        {
          temp = FC7300_HV_CHECK();

          if (up_interrupt_context())
            {
              up_udelay(100);
            }
          else
            {
              usleep(100);
            }
        }

      if (temp == STATUS_HVOP)
        {
          return -1;
        }
      else if (temp == STATUS_SUCCESS)
        {
          irqflags = enter_critical_section();
          if (type == FC7300_FLASH_NVR)
            {
              temp = FC7300_ERASE_NVR_CLEAR();
            }
          else
            {
              temp = FC7300_ERASE_CLEAR();
            }

          g_flash_opt_state = FC7300_FLASH_IDLE;
          leave_critical_section(irqflags);

          if (temp != STATUS_SUCCESS)
            {
              return -1;
            }
          else
            {
              addr = addr + FC7300_FLASH_SECTOR_SIZE;
            }
        }
      else
        {
          irqflags = enter_critical_section();
          if (type == FC7300_FLASH_NVR)
            {
              FC7300_ERASE_NVR_CLEAR();
            }
          else
            {
              FC7300_ERASE_CLEAR();
            }

          g_flash_opt_state = FC7300_FLASH_IDLE;
          leave_critical_section(irqflags);
          return -1;
        }
    }

  return 0;
}

static int flash_write(off_t offset, size_t nbytes, const uint8_t *buffer,
                       enum flash_type_e type)
{
  flash_drv_prgm_cfg_t flash_api_cfg;
  irqstate_t irqflags;
  uint32_t address;
  uint32_t length;
  uint32_t data_addr;
  uint32_t align_len;
  uint32_t temp_len;
  uint32_t align_offset;
  uint32_t index;
  uint32_t count;
  uint32_t try_count;
  uint32_t temp;

  if (type == FC7300_FLASH_DFLASH)
    {
      address = offset + FC7300_DFLASH_START_ADDR;
    }
  else if (type == FC7300_FLASH_PFLASH)
    {
      address = offset + FC7300_PFLASH_START_ADDR;
    }
  else if (type == FC7300_FLASH_NVR)
    {
      address = offset + FC7300_NVR_START_ADDR;
    }
  else
    {
      return -1;
    }

  length = nbytes;

  /* align address, write must align to FLASH_PROGRAM_PAGE_MAX_SIZE */

  align_offset = address & (FC7300_FLASH_PAGE_MAX_SIZE - 1U);
  align_len = length + align_offset;

  count = align_len & (FC7300_FLASH_PAGE_MAX_SIZE - 1U);
  count = count > 0U ? 1U : 0U;

  count += align_len / FC7300_FLASH_PAGE_MAX_SIZE;

  data_addr = (uint32_t)buffer;

  for (index = 0U; index < count; index++)
    {
      /* real write length in this cycle */

      temp_len = length + align_offset;
      temp_len = temp_len >= FC7300_FLASH_PAGE_MAX_SIZE ?
                             FC7300_FLASH_PAGE_MAX_SIZE : temp_len;
      temp_len -= align_offset;

      flash_api_cfg.pgff = 0;
      flash_api_cfg.dest = address;
      flash_api_cfg.size = temp_len / 4;
      flash_api_cfg.data = (uint32_t *)data_addr;
      flash_api_cfg.wdg_tune = 1;

      /* next address and length */

      address += temp_len;
      data_addr += temp_len;
      length -= temp_len;

      align_offset = 0U;

      if (type == FC7300_FLASH_DFLASH)
        {
          fc7300_dflash_unlock();
        }
      else if (type == FC7300_FLASH_PFLASH)
        {
          fc7300_pflash_unlock(flash_api_cfg.dest);
        }
      else
        {
          fc7300_nvr_unlock();
        }

      irqflags = enter_critical_section();

      if (g_flash_opt_state == FC7300_FLASH_IDLE)
        {
          if (type == FC7300_FLASH_NVR)
            {
              FC7300_ENABLE_HOLD(1);
              temp = FC7300_PEGM_NVR(&flash_api_cfg, 0);
              FC7300_ENABLE_HOLD(0);
            }
          else
            {
              temp = FC7300_PEGM(&flash_api_cfg, 0, 0);
            }

          g_flash_opt_state = FC7300_FLASH_WRITE;
          g_fc7300_flash_opt_addr = flash_api_cfg.dest;
          leave_critical_section(irqflags);
        }
      else
        {
          if (type == FC7300_FLASH_DFLASH)
            {
              fc7300_dflash_lock();
            }
          else if (type == FC7300_FLASH_PFLASH)
            {
              fc7300_pflash_lock(flash_api_cfg.dest);
            }
          else
            {
              fc7300_nvr_lock();
            }

          leave_critical_section(irqflags);
          return -1;
        }

      if (temp != STATUS_SUCCESS)
        {
          return -1;
        }

      try_count = 0;
      temp = FC7300_HV_CHECK();

      while ((temp == STATUS_HVOP) && (try_count++ < 100))
        {
          temp = FC7300_HV_CHECK();

          if (up_interrupt_context())
            {
              up_udelay(100);
            }
          else
            {
              usleep(100);
            }
        }

      if (temp == STATUS_HVOP)
        {
          return -1;
        }
      else if (temp == STATUS_SUCCESS)
        {
          irqflags = enter_critical_section();
          if (type == FC7300_FLASH_NVR)
            {
              temp = FC7300_PEGM_NVR_CLEAR();
            }
          else
            {
              temp = FC7300_PEGM_CLEAR();
            }

          g_flash_opt_state = FC7300_FLASH_IDLE;
          leave_critical_section(irqflags);

          if (temp != STATUS_SUCCESS)
            {
              return -1;
            }
        }
      else
        {
          irqflags = enter_critical_section();
          if (type == FC7300_FLASH_NVR)
            {
              FC7300_PEGM_NVR_CLEAR();
            }
          else
            {
              FC7300_PEGM_CLEAR();
            }

          g_flash_opt_state = FC7300_FLASH_IDLE;
          leave_critical_section(irqflags);
          return -1;
        }
    }

  return 0;
}

static uint32_t flash_wait_for_operation_finish(void)
{
  uint32_t timer = FLASH_ABT_TIMEOUT_VALUE;
  uint32_t ret = false;

  if (g_flash_opt_state == FC7300_FLASH_IDLE)
    {
      ret = true;
    }
  else
    {
      while ((FC7300_HV_CHECK() == STATUS_HVOP) && (0U < timer))
        {
          --timer;
        }

      if (FC7300_HV_CHECK() != STATUS_HVOP)
        {
          if (g_flash_opt_state == FC7300_FLASH_ERASE)
            {
              if (FC7300_ERASE_CLEAR() == STATUS_SUCCESS)
                {
                  ret = true;
                }
              else
                {
                  ret = false;
                }
            }
            else
            {
              if (FC7300_PEGM_CLEAR() == STATUS_SUCCESS)
                {
                  ret = true;
                }
              else
                {
                  ret = false;
                }
            }

            g_flash_opt_state = FC7300_FLASH_IDLE;
        }
      else
        {
          ret = false;
        }
    }

  return ret;
}

/****************************************************************************
 * Name: flash driver
 ****************************************************************************/

static int fc7300_flash_erase(struct mtd_dev_s *dev, off_t startblock,
                         size_t nblocks)
{
  ssize_t ret;
  struct fc7300_mtd_dev_s *priv = (struct fc7300_mtd_dev_s *)dev;

  if (priv->flash_type == FC7300_FLASH_DFLASH)
    {
      if ((startblock > FC7300_DFLASH_SECTOR_COUNT) ||
          ((startblock + nblocks) > FC7300_DFLASH_SECTOR_COUNT))
        {
          return -EINVAL;
        }
    }
  else if (priv->flash_type == FC7300_FLASH_PFLASH)
    {
      if ((startblock > FC7300_PFLASH_SECTOR_COUNT) ||
          ((startblock + nblocks) > FC7300_PFLASH_SECTOR_COUNT))
        {
          return -EINVAL;
        }
    }
  else if (priv->flash_type == FC7300_FLASH_NVR)
    {
      if ((startblock > FC7300_NVR_SECTOR_COUNT) ||
          ((startblock + nblocks) > FC7300_NVR_SECTOR_COUNT))
        {
          return -EINVAL;
        }
    }
  else
    {
      return -EINVAL;
    }

  if (up_interrupt_context())
    {
      flash_wait_for_operation_finish();
    }
  else
    {
      down_write(&g_fc7300_flash_rwlock);
    }

  ret = flash_erase(startblock, nblocks, priv->flash_type);

  if (ret == 0)
    {
      ret = nblocks;
    }
  else
    {
      ret = -1;
    }

  if (up_interrupt_context() == 0)
    {
      up_write(&g_fc7300_flash_rwlock);
    }

  return ret;
}

static ssize_t fc7300_flash_read(struct mtd_dev_s *dev, off_t offset,
                          size_t nbytes, uint8_t *buffer)
{
  uint32_t addr;
  uint32_t index;
  uint32_t temp;
  irqstate_t irqflags;
  struct fc7300_mtd_dev_s *priv = (struct fc7300_mtd_dev_s *)dev;

  if (priv->flash_type == FC7300_FLASH_DFLASH)
    {
      if ((offset > FC7300_DFLASH_BLOCK_SIZE) ||
          ((offset + nbytes) > FC7300_DFLASH_BLOCK_SIZE))
        {
          return -EINVAL;
        }

      addr = offset + FC7300_DFLASH_START_ADDR;
    }
  else if (priv->flash_type == FC7300_FLASH_PFLASH)
    {
      if ((offset > FC7300_PFLASH_BLOCK_SIZE) ||
          ((offset + nbytes) > FC7300_PFLASH_BLOCK_SIZE))
        {
          return -EINVAL;
        }

      addr = offset + FC7300_PFLASH_START_ADDR;
    }
  else if (priv->flash_type == FC7300_FLASH_NVR)
    {
      if ((offset > FC7300_NVR_BLOCK_SIZE) ||
          ((offset + nbytes) > FC7300_NVR_BLOCK_SIZE))
        {
          return -EINVAL;
        }

      addr = offset + FC7300_NVR_START_ADDR;
    }
  else
    {
      return -EINVAL;
    }

  irqflags = enter_critical_section();
  temp = ((g_fc7300_flash_opt_addr >> 21) == (addr >> 21)) ? true : false;
  if ((g_flash_opt_state != FC7300_FLASH_IDLE) && (temp == true))
    {
      leave_critical_section(irqflags);
      down_read(&g_fc7300_flash_rwlock);
      irqflags = enter_critical_section();
      for (index = 0U; index < nbytes; index++)
        {
          buffer[index] = *(uint8_t *)(addr + index);
        }

      leave_critical_section(irqflags);
      up_read(&g_fc7300_flash_rwlock);
    }
  else
    {
      for (index = 0U; index < nbytes; index++)
        {
          buffer[index] = *(uint8_t *)(addr + index);
        }

      leave_critical_section(irqflags);
    }

  if (g_fc7300_eccfault)
    {
      g_fc7300_eccfault = false;
      return -EBADMSG;
    }

  return nbytes;
}

static ssize_t fc7300_flash_bread(struct mtd_dev_s *dev, off_t startblock,
                             size_t nblocks, uint8_t *buffer)
{
  uint32_t offset;
  uint32_t nbytes;
  ssize_t result;

  offset = startblock * FC7300_FLASH_PAGE_SIZE;
  nbytes = nblocks * FC7300_FLASH_PAGE_SIZE;
  result = fc7300_flash_read(dev, offset, nbytes, buffer);
  return result < 0 ? result : nblocks;
}

static ssize_t fc7300_flash_write(struct mtd_dev_s *dev, off_t offset,
                           size_t nbytes, const uint8_t *buffer)
{
  ssize_t ret;
  struct fc7300_mtd_dev_s *priv = (struct fc7300_mtd_dev_s *)dev;

  ASSERT(buffer);

  if ((nbytes == 0) || (offset % FC7300_FLASH_PAGE_SIZE)
       || (nbytes % FC7300_FLASH_PAGE_SIZE))
    {
      return -EINVAL;
    }

  if (priv->flash_type == FC7300_FLASH_DFLASH)
    {
      if ((offset > FC7300_DFLASH_BLOCK_SIZE) ||
          ((offset + nbytes) > FC7300_DFLASH_BLOCK_SIZE))
        {
          return -EINVAL;
        }
    }
  else if (priv->flash_type == FC7300_FLASH_PFLASH)
    {
      if ((offset > FC7300_PFLASH_BLOCK_SIZE) ||
          ((offset + nbytes) > FC7300_PFLASH_BLOCK_SIZE))
        {
          return -EINVAL;
        }
    }
  else if (priv->flash_type == FC7300_FLASH_NVR)
    {
      if ((offset > FC7300_NVR_BLOCK_SIZE) ||
          ((offset + nbytes) > FC7300_NVR_BLOCK_SIZE))
        {
          return -EINVAL;
        }
    }
  else
    {
      return -EINVAL;
    }

  if (up_interrupt_context())
    {
      flash_wait_for_operation_finish();
    }
  else
    {
      down_write(&g_fc7300_flash_rwlock);
    }

  ret = flash_write(offset, nbytes, buffer, priv->flash_type);

  if (ret == 0)
    {
      ret = nbytes;
    }

  if (up_interrupt_context() == 0)
    {
      up_write(&g_fc7300_flash_rwlock);
    }

  return ret;
}

static ssize_t fc7300_flash_bwrite(struct mtd_dev_s *dev, off_t startblock,
                              size_t nblocks, const uint8_t *buffer)
{
  uint32_t offset;
  uint32_t nbytes;
  ssize_t result;

  offset = startblock * FC7300_FLASH_PAGE_SIZE;
  nbytes = nblocks * FC7300_FLASH_PAGE_SIZE;
  result = fc7300_flash_write(dev, offset, nbytes, buffer);
  return result < 0 ? result : nblocks;
}

static int fc7300_flash_ioctl(struct mtd_dev_s *dev, int cmd,
                         unsigned long arg)
{
  int ret = -EINVAL;
  struct fc7300_mtd_dev_s *priv = (struct fc7300_mtd_dev_s *)dev;
  finfo("cmd: %d\n", cmd);

  switch (cmd)
    {
      case MTDIOC_GEOMETRY:
        {
          struct mtd_geometry_s *geo = (struct mtd_geometry_s *)arg;
          if (geo)
            {
              memset(geo, 0, sizeof(*geo));

              geo->blocksize    = FC7300_FLASH_PAGE_SIZE;
              geo->erasesize    = FC7300_FLASH_SECTOR_SIZE;
              if (priv->flash_type == FC7300_FLASH_DFLASH)
                {
                  geo->neraseblocks = FC7300_DFLASH_SECTOR_COUNT;
                }
              else if (priv->flash_type == FC7300_FLASH_PFLASH)
                {
                  geo->neraseblocks = FC7300_PFLASH_SECTOR_COUNT;
                }
              else
                {
                  geo->neraseblocks = FC7300_NVR_SECTOR_COUNT;
                }

              ret  = OK;

              finfo("blocksize: %" PRId32 " erasesize: %" PRId32 \
                    " neraseblocks: %" PRId32 "\n",
                    geo->blocksize, geo->erasesize, geo->neraseblocks);
            }
        }
        break;

      case BIOC_PARTINFO:
        {
          struct partition_info_s *info = (struct partition_info_s *)arg;
          if (info != NULL)
            {
              if (priv->flash_type == FC7300_FLASH_DFLASH)
                {
                  info->numsectors  = FC7300_DFLASH_BLOCK_SIZE /
                                       FC7300_FLASH_PAGE_SIZE;
                }
              else if (priv->flash_type == FC7300_FLASH_PFLASH)
                {
                  info->numsectors  = FC7300_PFLASH_BLOCK_SIZE /
                                       FC7300_FLASH_PAGE_SIZE;
                }
              else
                {
                  info->numsectors  = FC7300_NVR_BLOCK_SIZE /
                                       FC7300_FLASH_PAGE_SIZE;
                }

              info->sectorsize  = FC7300_FLASH_PAGE_SIZE;
              info->startsector = 0;
              info->parent[0]   = '\0';
              ret  = OK;
            }
        }
        break;

      case BIOC_XIPBASE:
        {
          FAR void **ppv = (FAR void**)arg;

          if (ppv)
            {
              /* Return (void*) base address of FLASH memory. */

              ret  = OK;
              if (priv->flash_type == FC7300_FLASH_DFLASH)
                {
                  *ppv = (FAR void *)FC7300_DFLASH_START_ADDR;
                }
              else if (priv->flash_type == FC7300_FLASH_PFLASH)
                {
                  *ppv = (FAR void *)FC7300_PFLASH_START_ADDR;
                }
              else
                {
                  *ppv = (FAR void *)FC7300_NVR_START_ADDR;
                }
            }
        }
        break;

      case MTDIOC_ERASESTATE:
        {
          uint8_t *result = (uint8_t *)arg;
          *result = MTD_ERASED_STATE;

          ret = OK;
        }
        break;

      default:
        ret = -ENOTTY;
        break;
    }

  finfo("return %d\n", ret);
  return ret;
}

#if defined(CONFIG_FC7300_FCSMU_CHANNEL_FLASH_ECC)
static void clear_fcsmu_int(void)
{
  uint32_t rst = 0;
  uint32_t timeout = 65535U;
  putreg32(FCSMU_FST_UNLOCK_KEY,
            FC7300_FCSMU_BASE + FC7300_FCSMU_FST_UNLK_OFFSET);
  putreg32(FCSMU_FAULT_CHANNEL_FLASH_ECC_ERROR,
            FC7300_FCSMU_BASE + FC7300_FCSMU_FST0_OFFSET);
  rst = getreg32(FC7300_FCSMU_BASE + FC7300_FCSMU_CTRL_OFFSET);

  while (((rst & FCSMU_CTRL_OPS_MASK) >> FCSMU_CTRL_OPS_SHIFT) !=
        FCSMU_OP_STATE_SUCCESSFUL)
    {
      timeout--;
      if (timeout == 0U)
        {
          break;
        }
    }

    fc7300_clearpendingisr(FC7300_IRQ_FCSMU);
}
#endif

#if defined(CONFIG_FC7300_ERM_DFLASH_ECC_MODE_M) || defined(CONFIG_FC7300_ERM_DFLASH_ECC_MODE_A)
static void clear_erm_int(void)
{
  putreg32(ERM_SR0_NCE3, FC7300_ERM_BASE + FC7300_ERM_SR0_OFFSET);
  fc7300_clearpendingisr(FC7300_IRQ_ERM);
}
#endif

static int fc7300_hardfault_isr(int irq, void *context, void *arg)
{
  if (STATUS_ECC_ERROR == FC7300_DFLASH_STATUS_READ(FLASH_ECC_CHECK_MASK))
    {
      uint32_t ecc_addr = 0;
      uint32_t cfcr = 0;
      uint16_t ins_type;
      uint32_t *regs = context;
      uint32_t nvs_start_addr = FC7300_DFLASH_START_ADDR
                                  + FC7300_DFLASH_NVS_OFFSET;
      uint32_t nvs_end_addr = FC7300_DFLASH_START_ADDR
                                  + FC7300_DFLASH_NVS_OFFSET
                                  + FC7300_DFLASH_NVS_SIZE;
      ecc_addr = getreg32(NVIC_BFAULT_ADDR);

      if ((nvs_start_addr <= ecc_addr) && (ecc_addr < nvs_end_addr))
        {
          FC7300_DFLASH_STATUS_CLEAR(FLASH_ECC_CHECK_MASK);
          g_fc7300_eccfault = true;
          ins_type = (*(volatile uint16_t *)(regs[REG_PC]) >> 11) & 0x1F;

          if ((ins_type == 0x1D) || (ins_type == 0x1E) || (ins_type == 0x1F))
            {
              regs[REG_PC] += 4;
            }
          else
            {
              regs[REG_PC] += 2;
            }

          cfcr = getreg32(NVIC_CFAULTS);
          cfcr |= NVIC_CFAULTS_PRECISERR;
          putreg32(cfcr, NVIC_CFAULTS);
#if defined(CONFIG_FC7300_FCSMU_CHANNEL_FLASH_ECC)
          clear_fcsmu_int();
#endif
#if defined(CONFIG_FC7300_ERM_DFLASH_ECC_MODE_M) || defined(CONFIG_FC7300_ERM_DFLASH_ECC_MODE_A)
          clear_erm_int();
#endif
          return OK;
        }
    }

  return arm_hardfault(irq, context, arg);
}

struct mtd_dev_s *fc7300_nordflash_mtd(void)
{
  struct mtd_dev_s *mtd = (struct mtd_dev_s *)&g_fc7300_nordflash;
  irq_attach(FC7300_IRQ_HARDFAULT, fc7300_hardfault_isr, NULL);
  if (g_fc7300_flash_rwlock_initialized == 0)
    {
      init_rwsem(&g_fc7300_flash_rwlock);
      g_fc7300_flash_rwlock_initialized = 1;
    }

  return mtd;
}

struct mtd_dev_s *fc7300_norpflash_mtd(void)
{
  struct mtd_dev_s *mtd = (struct mtd_dev_s *)&g_fc7300_norpflash;
  FC7300_ENABLE_HOLD(0);
  if (g_fc7300_flash_rwlock_initialized == 0)
    {
      init_rwsem(&g_fc7300_flash_rwlock);
      g_fc7300_flash_rwlock_initialized = 1;
    }

  return mtd;
}

struct mtd_dev_s *fc7300_nornvr_mtd(void)
{
  struct mtd_dev_s *mtd = (struct mtd_dev_s *)&g_fc7300_nornvr;
  if (g_fc7300_flash_rwlock_initialized == 0)
    {
      init_rwsem(&g_fc7300_flash_rwlock);
      g_fc7300_flash_rwlock_initialized = 1;
    }

  return mtd;
}

void fc7300_pflash_dblecc_rsp_enable(void)
{
  uint32_t temp = 0;
  temp = getreg32(FC7300_FEEC);
  temp &= ~(FMC_FEEC_DDBEE_MASK);
  putreg32(temp, FC7300_FEEC);
}

void fc7300_pflash_dblecc_rsp_disable(void)
{
  uint32_t temp = 0;
  temp = getreg32(FC7300_FEEC);
  temp |= FMC_FEEC_DDBEE_MASK;
  putreg32(temp, FC7300_FEEC);
}

void fc7300_pflash_prefetch_enable(void)
{
  putreg32(FMC_FAPC0_DBPEN_MASK | FMC_FAPC0_CBPEN_MASK |
           FMC_FAPC0_DBBEN_MASK | FMC_FAPC0_CBBEN_MASK,
           FC7300_FAPC0);
  putreg32(FMC_FAPC1_DBPEN_MASK | FMC_FAPC1_CBPEN_MASK |
           FMC_FAPC1_DBBEN_MASK | FMC_FAPC1_CBBEN_MASK,
           FC7300_FAPC1);
}

void fc7300_pflash_prefetch_disable(void)
{
  uint32_t temp = 0;
  temp = getreg32(FC7300_FAPC0);
  temp &= ~(FMC_FAPC0_DBPEN_MASK | FMC_FAPC0_CBPEN_MASK);
  putreg32(temp, FC7300_FAPC0);
  temp = getreg32(FC7300_FAPC1);
  temp &= ~(FMC_FAPC1_DBPEN_MASK | FMC_FAPC1_CBPEN_MASK);
  putreg32(temp, FC7300_FAPC1);
}

#ifdef CONFIG_FC7300_DFLASH_INJECT_ECC
__attribute__((__optimize__("-O0"))) uint32_t fc7300_flash_ecc_inject(
                                               uint32_t addr)
{
  uint32_t temp = 1;
  uint32_t ret = false;
  flash_drv_ecc_cfg_t cfg;
  cfg.data1 = ECC_TEST_INJECT_DATA1,
  cfg.data2 = ECC_TEST_INJECT_DATA2,
  cfg.data3 = ECC_TEST_INJECT_DATA3,
  cfg.data4 = ECC_TEST_INJECT_DATA4,
  cfg.data5 = ECC_TEST_INJECT_DATA5,
  cfg.size = 0,
  cfg.dest = addr + FC7300_DFLASH_START_ADDR;
  temp = getreg32(FC7300_FAPC0);
  temp &= ~(FMC_FAPC0_CBBEN_MASK | FMC_FAPC0_DBBEN_MASK);
  putreg32(temp, FC7300_FAPC0);
  temp = getreg32(FC7300_FAPC1);
  temp &= ~(FMC_FAPC1_CBBEN_MASK | FMC_FAPC1_DBBEN_MASK);
  putreg32(temp, FC7300_FAPC1);
  FC7300_FLASH_TEST_MODE_OPEN();
  temp = FC7300_FLASH_INJECT_ECC(&cfg, 0);

  if (temp == STATUS_SUCCESS)
    {
      temp = *(uint32_t *)(cfg.dest);
      ret = true;
    }
  else
    {
      ret = false;
    }

  FC7300_FLASH_TEST_MODE_CLOSE();
  temp = getreg32(FC7300_FAPC0);
  temp |= (FMC_FAPC0_CBBEN_MASK | FMC_FAPC0_DBBEN_MASK);
  putreg32(temp, FC7300_FAPC0);
  temp = getreg32(FC7300_FAPC1);
  temp |= (FMC_FAPC1_CBBEN_MASK | FMC_FAPC1_DBBEN_MASK);
  putreg32(temp, FC7300_FAPC1);

  return ret;
}
#endif
