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
#include <errno.h>

#include "fc7300_progmem.h"
#include "arm_internal.h"
#include "hardware/fc7300_fmc.h"

#include <arch/board/board.h> /* Include last:  has dependencies */

/****************************************************************************
 * LOCAL MACROS
 ****************************************************************************/
#define FLASHDRV_ADDR                   0x04810200U

#define STATUS_SUCCESS                  0x001UL
#define STATUS_HVOP                     0x8001UL

#define FC7300_ERASE(x, y, z)           (s_flash_driver_func_header->erase_sector(x, y, z))
#define FC7300_PEGM(x, y, z)            (s_flash_driver_func_header->program(x, y, z))
#define FC7300_HV_CHECK()               (s_flash_driver_func_header->hv_status_check())
#define FC7300_ERASE_CLEAR()            (s_flash_driver_func_header->erase_sector_clear())
#define FC7300_PEGM_CLEAR()             (s_flash_driver_func_header->program_clear())
/****************************************************************************
 * LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
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

typedef struct
{
  uint32_t RESERVED1[3U];
  uint32_t (*erase_sector)
           (flash_drv_erase_sector_cfg_t * flash_api_cfg,
            uint32_t int_en, uint32_t type);
  uint32_t (*erase_sector_clear)(void);
  uint32_t RESERVED2[2U];
  uint32_t (*program)(flash_drv_prgm_cfg_t * flash_api_cfg,
                                uint32_t int_en, uint32_t type);
  uint32_t (*program_clear)(void);
  uint32_t RESERVED3[6U];
  uint32_t (*hv_status_check)(void);
}flash_rom_api_entry_t;

/****************************************************************************
 * Private Data
 ****************************************************************************/

static const flash_rom_api_entry_t  *s_flash_driver_func_header;
/****************************************************************************
 * Private Functions
 ****************************************************************************/

/**
 * \brief PFlash Driver Function for unlock sector
 *
 * \param u32Address sector address
 */

static void fc7300_flash_unlock(void)
{
  /* 1 bank contains only 256KB, only used FB_FPELCK */

  putreg32(0, FC7300_FB_FPELCK4);
}

static int fc7300_flash_erase(uint32_t block)
{
  flash_drv_erase_sector_cfg_t flash_api_cfg;
  uint32_t temp;
  uint32_t try_count;
  flash_api_cfg.blk_sel = 4UL;
  flash_api_cfg.dest = block * FC7300_PROGMEM_BLOCK_SECTOR_SIZE
                       + FC7300_PROGMEM_START_ADDR;

  fc7300_flash_unlock();
  temp = FC7300_ERASE(&flash_api_cfg, 0, 0);

  if (temp != STATUS_SUCCESS)
    {
      return -1;
    }

  temp = FC7300_HV_CHECK();
  try_count = 0;
  while ((STATUS_HVOP == temp) && (try_count++ < 1000))
    {
      temp = FC7300_HV_CHECK();
      usleep(100);
    }

  if (temp == STATUS_HVOP)
    {
      return -1;
    }
  else if (temp == STATUS_SUCCESS)
    {
      temp = FC7300_ERASE_CLEAR();

      if (temp != STATUS_SUCCESS)
        {
          return -1;
        }
      else
        {
          return 0;
        }
    }
  else
    {
      FC7300_ERASE_CLEAR();
      return -1;
    }
}

static int fc7300_flash_write(size_t addr, const void *buf, size_t len)
{
  flash_drv_prgm_cfg_t flash_api_cfg;
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

  address = addr;
  length = len;

  /* align address, write must align to FLASH_PROGRAM_PAGE_MAX_SIZE */

  align_offset = address & (FC7300_PROGRAM_PAGE_MAX_SIZE - 1U);
  align_len = length + align_offset;

  count = align_len & (FC7300_PROGRAM_PAGE_MAX_SIZE - 1U);
  count = count > 0U ? 1U : 0U;

  count += align_len / FC7300_PROGRAM_PAGE_MAX_SIZE;

  data_addr = (uint32_t)buf;

  for (index = 0U; index < count; index++)
    {
      /* real write length in this cycle */

      temp_len = length + align_offset;
      temp_len = temp_len >= FC7300_PROGRAM_PAGE_MAX_SIZE ?
                             FC7300_PROGRAM_PAGE_MAX_SIZE : temp_len;
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
      fc7300_flash_unlock();
      temp = FC7300_PEGM(&flash_api_cfg, 0, 0);

      if (temp != STATUS_SUCCESS)
        {
          return -1;
        }

      try_count = 0;
      temp = FC7300_HV_CHECK();

      while ((temp == STATUS_HVOP) && (try_count++ < 100))
        {
          temp = FC7300_HV_CHECK();
          usleep(100);
        }

      if (temp == STATUS_HVOP)
        {
          return -1;
        }
      else if (temp == STATUS_SUCCESS)
        {
          temp = FC7300_PEGM_CLEAR();

          if (temp != STATUS_SUCCESS)
            {
              return -1;
            }
        }
      else
        {
          FC7300_PEGM_CLEAR();
          return -1;
        }
    }

  return 0;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: up_progmem_neraseblocks
 *
 * Description:
 *   Return number of erase blocks
 *
 ****************************************************************************/

size_t up_progmem_neraseblocks(void)
{
  return FC7300_PROGMEM_SECTOR_COUNT;
}

/****************************************************************************
 * Name: up_progmem_isuniform
 *
 * Description:
 *   Is program memory uniform or page size differs?
 *
 ****************************************************************************/

bool up_progmem_isuniform(void)
{
  return true;
}

/****************************************************************************
 * Name: up_progmem_pagesize
 *
 * Description:
 *   Return read/write page size
 *
 ****************************************************************************/

size_t up_progmem_pagesize(size_t page)
{
  return (size_t)FC7300_PROGMEM_PAGE_SIZE;
}

/****************************************************************************
 * Name: up_progmem_erasesize
 *
 * Description:
 *   Return erase block size
 *
 ****************************************************************************/

size_t up_progmem_erasesize(size_t block)
{
  return (size_t)FC7300_PROGMEM_BLOCK_SECTOR_SIZE;
}

/****************************************************************************
 * Name: up_progmem_getpage
 *
 * Description:
 *   Address to read/write page conversion
 *
 * Input Parameters:
 *   addr - Address with or without flash offset (absolute or aligned to
 *          page0)
 *
 * Returned Value:
 *   Page or negative value on error.  The following errors are reported
 *   (errno is not set!):
 *
 *     -EFAULT: On invalid address
 *
 ****************************************************************************/

ssize_t up_progmem_getpage(size_t addr)
{
  if (addr >= FC7300_PROGMEM_START_ADDR)
    {
      addr -= FC7300_PROGMEM_START_ADDR;
    }

  return (size_t)(addr / FC7300_PROGMEM_PAGE_SIZE);
}

/****************************************************************************
 * Name: up_progmem_getaddress
 *
 * Description:
 *   Read/write page to address conversion
 *
 * Input Parameters:
 *   page - page index
 *
 * Returned Value:
 *   Base address of given page, SIZE_MAX if page index is not valid.
 *
 ****************************************************************************/

size_t up_progmem_getaddress(size_t page)
{
  return (size_t)(FC7300_PROGMEM_START_ADDR
           + (page * FC7300_PROGMEM_PAGE_SIZE));
}

/****************************************************************************
 * Name: up_progmem_eraseblock
 *
 * Description:
 *   Erase selected block.
 *
 * Input Parameters:
 *   block - The erase block index to be erased.
 *
 * Returned Value:
 *   block size or negative value on error.  The following errors are
 *   reported (errno is not set!):
 *
 *     -EFAULT: On invalid page
 *     -EIO:    On unsuccessful erase
 *     -EROFS:  On access to write protected area
 *     -EACCES: Insufficient permissions (read/write protected)
 *     -EPERM:  If operation is not permitted due to some other constraints
 *              (i.e. some internal block is not running etc.)
 *
 ****************************************************************************/

ssize_t up_progmem_eraseblock(size_t block)
{
  int ret;
  if (block >= FC7300_PROGMEM_SECTOR_COUNT)
    {
      return -EFAULT;
    }

  ret = fc7300_flash_erase(block);

  if (ret < 0)
    {
      return -EIO;
    }

  return (ssize_t)FC7300_PROGMEM_BLOCK_SECTOR_SIZE;
}

/****************************************************************************
 * Name: up_progmem_ispageerased
 *
 * Description:
 *   Checks whether page is erased
 *
 * Input Parameters:
 *   page - The erase page index to be checked.
 *
 * Returned Value:
 *   Returns number of bytes NOT erased or negative value on error. If it
 *   returns zero then complete page is erased.
 *
 *   The following errors are reported:
 *     -EFAULT: On invalid page
 *
 ****************************************************************************/

ssize_t up_progmem_ispageerased(size_t page)
{
  const uint8_t *p;
  int i;

  if (page >= FC7300_PROGMEM_PAGE_COUNT)
    {
      return -EFAULT;
    }

  p = (const uint8_t *)up_progmem_getaddress(page);

  for (i = 0; i < FC7300_PROGMEM_PAGE_SIZE; i++)
    {
      if (p[i] != FC7300_PROGMEM_ERASEDVAL)
        {
          break;
        }
    }

  return (ssize_t)(FC7300_PROGMEM_PAGE_SIZE - i);
}

/****************************************************************************
 * Name: up_progmem_write
 *
 * Description:
 *   Program data at given address
 *
 *   Note: this function is not limited to single page and nor it requires
 *   the address be aligned inside the page boundaries.
 *
 * Input Parameters:
 *   addr  - Address with or without flash offset
 *   buf   - Pointer to buffer
 *   count - Number of bytes to write
 *
 * Returned Value:
 *   Bytes written or negative value on error.  The following errors are
 *   reported (errno is not set!)
 *
 *     EINVAL: If count is not aligned with the flash boundaries (i.e.
 *             some MCU's require per half-word or even word access)
 *     EFAULT: On invalid address
 *     EIO:    On unsuccessful write, do note when this occurs the complete
 *             flash sector is deemed to be unreadable and a read will most
 *             likely result in a hard fault.
 *     EROFS:  On access to write protected area
 *     EACCES: Insufficient permissions (read/write protected)
 *     EPERM:  If operation is not permitted due to some other constraints
 *             (i.e. some internal block is not running etc.)
 *
 ****************************************************************************/

ssize_t up_progmem_write(size_t addr, const void *buf, size_t count)
{
  size_t offset;
  int ret;

  /* Check for valid address range. */

  if (count % FC7300_PROGMEM_PAGE_SIZE != 0)
    {
      return -EINVAL;
    }

  offset = addr;

  if (addr >= FC7300_PROGMEM_START_ADDR)
    {
      offset -= FC7300_PROGMEM_START_ADDR;
    }
  else
    {
      return -EFAULT;
    }

  if (offset + count > FC7300_PROGMEM_BLOCK_SIZE)
    {
      return -EFAULT;
    }

  ret = fc7300_flash_write(addr, buf, count);

  if (ret < 0)
    {
      return -EIO;
    }

  return (ssize_t)count;
}

/****************************************************************************
 * Name: up_progmem_erasestate
 *
 * Description:
 *   Return value of erase state.
 *
 ****************************************************************************/

uint8_t up_progmem_erasestate(void)
{
  return FC7300_PROGMEM_ERASEDVAL;
}

void fc7300_progmem_init(void)
{
  s_flash_driver_func_header = (flash_rom_api_entry_t *)FLASHDRV_ADDR;
}
