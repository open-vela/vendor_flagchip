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
#include <nuttx/mtd/mtd.h>
#include <nuttx/mtd/configdata.h>
#include <nuttx/fs/nxffs.h>
#include <nuttx/fs/smart.h>
#include <nuttx/fs/fs.h>

#include <debug.h>
#include <stdio.h>

#include "fc7300_partition.h"
#include "fc7300_norflash_mtd.h"

/****************************************************************************
 * Pre-Processor Definitions
 ****************************************************************************/

/* Define default values to silent compiler warning about undefined macro */

/****************************************************************************
 * Private Definitions
 ****************************************************************************/

struct part_table
{
    int         offset; /* partition offset */
    int         size;   /* partition size in byte */
    const char *name;   /* name of the partition */
};

/* partition table, first entry *must always* be program flash memory */

#ifdef CONFIG_FC7300_NOR_PFLASH
static const struct part_table pflash_part_table[] =
{
  {
    .offset   = FC7300_PFLASH_A_AP0_OFFSET,
    .size     = FC7300_PFLASH_A_BANK0_SIZE,
    .name     = "/dev/ap0",
  },

  {
    .offset   = FC7300_PFLASH_A_OTAVER1_OFFSET,
    .size     = FC7300_PFLASH_OTAVER_SIZE,
    .name     = "/dev/ota_b0",
  },

  {
    .offset   = FC7300_PFLASH_B_OTAVER1_OFFSET,
    .size     = FC7300_PFLASH_OTAVER_SIZE,
    .name     = "/dev/ota_b1",
  },

  {
    .offset   = FC7300_PFLASH_B_OTAVER2_OFFSET,
    .size     = FC7300_PFLASH_OTAVER_SIZE,
    .name     = "/dev/ota_b2",
  },

  {
    .offset   = FC7300_PFLASH_A_TSW_OFFSET,
    .size     = FC7300_PFLASH_TSW_SIZE,
    .name     = "/dev/tsw_b0",
  },

  {
    .offset   = FC7300_PFLASH_B_TSW_OFFSET,
    .size     = FC7300_PFLASH_TSW_SIZE,
    .name     = "/dev/tsw_b1",
  }
};
#endif

#ifdef CONFIG_FC7300_NOR_DFLASH
static const struct part_table dflash_part_table[] =
{
  {
    .offset   = FC7300_DFLASH_NVS_OFFSET,
    .size     = FC7300_DFLASH_NVS_SIZE,
#ifdef CONFIG_FC7300_MTD_CFG_FOR_NVM
    .name     = CONFIG_FC7300_MTD_CFG_PATH_FOR_NVM,
#else
    .name     = "/dev/config",
#endif
  },
  {
    .offset   = FC7300_DFLASH_MANUFACTURY_DATA_OFFSET,
    .size     = FC7300_DFLASH_MANUFACTURY_DATA_SIZE,
    .name     = "/dev/manufactury"
  },
  {
    .offset   = FC7300_DFLASH_TRAPINFO_OFFSET,
    .size     = FC7300_DFLASH_TRAPINFO_SIZE,
    .name     = "/dev/trapinfo",
  },
  {
    .offset   = FC7300_DFLASH_BSWLOG_OFFSET,
    .size     = FC7300_DFLASH_BSWLOG_SIZE,
    .name     = "/dev/bswlog",
  },
};
#endif

#ifdef CONFIG_FC7300_NOR_NVR
static const struct part_table nvr_part_table[] =
{
  {
    .offset   = FC7300_NVR_TEST_OFFSET,
    .size     = FC7300_NVR_TEST_SIZE,
    .name     = "/dev/nvr_test"
  }
};
#endif

/****************************************************************************
 * Private Functions
 ****************************************************************************/

#ifdef CONFIG_AUTOCORE_FEE

static void det_report(int error_id)
{
}
#endif

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: fc7300_partition_init
 *
 *   Initialize fc7300 partition. Read partition information, and use
 *   these data for creating MTD.
 *
 * Input Parameters:
 *   None
 *
 * Returned Value:
 *   0 if success or a negative value if fail.
 *
 ****************************************************************************/

void fc7300_partition_init(void)
{
  struct mtd_dev_s *mtd_flash;
  struct mtd_dev_s *mtd_part;
  struct mtd_geometry_s geo;
  int ret = 0;
  int i;

#ifdef CONFIG_FC7300_NOR_PFLASH
  mtd_flash = fc7300_norpflash_mtd();
  if (mtd_flash == NULL)
    {
      ferr("ERROR: Failed to get nor pflash MTD\n");
    }
  else
    {
      ret = MTD_IOCTL(mtd_flash, MTDIOC_GEOMETRY, (unsigned long)&geo);

      if (ret < 0)
        {
          ferr("ERROR: Failed to get info from MTD\n");
        }
      else
        {
          for (i = 0; i != sizeof(pflash_part_table) /
               sizeof(struct part_table); i++)
            {
              mtd_part = mtd_partition(mtd_flash,
                                       pflash_part_table[i].offset /
                                       geo.blocksize,
                                       pflash_part_table[i].size /
                                       geo.blocksize);

              if (mtd_part == NULL)
                {
                  ferr("[%s]ERROR: mtd_partition() failed %d\n",
                       pflash_part_table[i].name, errno);
                  continue;
                }

              ret = register_mtddriver(pflash_part_table[i].name,
                                       mtd_part, 0755, NULL);

              if (ret != 0)
                {
                  _err("register_mtddriver() failed: %d\n", ret);
                }
            }
        }
    }
#endif

#ifdef CONFIG_FC7300_NOR_DFLASH
  mtd_flash = fc7300_nordflash_mtd();
  if (mtd_flash == NULL)
    {
      ferr("ERROR: Failed to get nor dflash MTD\n");
    }
  else
    {
      ret = MTD_IOCTL(mtd_flash, MTDIOC_GEOMETRY, (unsigned long)&geo);

      if (ret < 0)
        {
          ferr("ERROR: Failed to get info from MTD\n");
        }
      else
        {
          for (i = 0; i != sizeof(dflash_part_table) /
               sizeof(struct part_table); i++)
            {
              mtd_part = mtd_partition(mtd_flash,
                                       dflash_part_table[i].offset /
                                       geo.blocksize,
                                       dflash_part_table[i].size /
                                       geo.blocksize);

              if (mtd_part == NULL)
                {
                  ferr("[%s]ERROR: mtd_partition() failed %d\n",
                       dflash_part_table[i].name, errno);
                  continue;
                }

              if (i != 0)
                {
                  ret = register_mtddriver(dflash_part_table[i].name,
                                          mtd_part, 0755, NULL);
                  if (ret < 0)
                    {
                      _err("register_mtddriver() failed: %d\n", ret);
                    }
                }
#ifdef CONFIG_AUTOCORE_FEE
              else
                {
                  ret = mtdconfig_register_by_path(mtd_part,
                                                  dflash_part_table[i].name);
                  if (ret == -EDEADLK)
                    {
                      det_report(ret);
                      mtd_flash->erase(mtd_flash,
                                dflash_part_table[i].offset / geo.erasesize,
                                dflash_part_table[i].size / geo.erasesize);
                      ret = mtdconfig_register_by_path(mtd_part,
                                                  dflash_part_table[i].name);
                    }

                  if (ret < 0)
                    {
                      det_report(ret);
                      _err("mtdconfig_register_by_path() failed: %d\n",
                            ret);
                    }
                }
#endif
            }
        }
    }
#endif

#ifdef CONFIG_FC7300_NOR_NVR
  mtd_flash = fc7300_nornvr_mtd();
  if (mtd_flash == NULL)
    {
      ferr("ERROR: Failed to get nor nvr MTD\n");
    }
  else
    {
      ret = MTD_IOCTL(mtd_flash, MTDIOC_GEOMETRY, (unsigned long)&geo);

      if (ret < 0)
        {
          ferr("ERROR: Failed to get info from MTD\n");
        }
      else
        {
          for (i = 0; i != sizeof(nvr_part_table) /
               sizeof(struct part_table); i++)
            {
              mtd_part = mtd_partition(mtd_flash,
                                       nvr_part_table[i].offset /
                                       geo.blocksize,
                                       nvr_part_table[i].size /
                                       geo.blocksize);

              if (mtd_part == NULL)
                {
                  ferr("[%s]ERROR: mtd_partition() failed %d\n",
                       nvr_part_table[i].name, errno);
                  continue;
                }

              ret = register_mtddriver(nvr_part_table[i].name,
                                       mtd_part, 0755, NULL);

              if (ret != 0)
                {
                  _err("register_mtddriver() failed: %d\n", ret);
                }
            }
        }
    }
#endif
}

