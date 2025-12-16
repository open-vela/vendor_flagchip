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

#include <debug.h>
#include <fcntl.h>
#include <nuttx/nuttx.h>
#include <nuttx/rptun/rptun.h>
#include <nuttx/signal.h>
#include <nuttx/streams.h>
#include "arm_internal.h"

#include "fc7300_mb.h"
#include "fc7300_multicore.h"
#include "fc7300_rptun.h"
#include "fc7300_norflash_mtd.h"
#include "fc7300_partition.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#if defined(CONFIG_ARMV7M_DCACHE) && !defined(CONFIG_ARM_MPU)
#  error CONFIG_ARM_MPU must be enabled
#endif

/* Vring configuration parameters */

#define VRINGS                   (2)           /* Number of vrings */
#define VRING_ALIGN              (8)           /* Vring alignment */
#define VRING_NR                 (8)           /* Number of descriptors */
#define VRING_SIZE               (128)         /* Size of one descriptor */
#define VRING_RSVD               (512)
#define VRING0_NOTIFYID          (RSC_NOTIFY_ID_ANY)  /* Vring0 id */
#define VRING1_NOTIFYID          (RSC_NOTIFY_ID_ANY)  /* Vring1 id */

/* MB configuration */

#define RPTUN_MB_CHAN_RX_MASTER       (0)        /* Notify master */
#define RPTUN_MB_CHAN_RX_SLAVE        (1)        /* Notify slave */
#define RPTUN_MB_CHAN_RESET           (2)
#define RPTUN_MB_CHAN_PANIC           (3)

/****************************************************************************
 * Private Types
 ****************************************************************************/

/* FC7300 rptun sharred memory */

struct fc7300_rptun_shmem_s
{
  volatile uintptr_t         base;
  struct rptun_rsc_s         rsc;
  char                       buf[VRINGS * VRING_NR * VRING_SIZE + VRING_RSVD];
};

/* FC7300 rptun device */

struct fc7300_rptun_dev_s
{
  struct rptun_dev_s           rptun;
  struct fc7300_mb_dev_s      *mailbox;     /* fc7300 mailbox driver */
  rptun_callback_t             callback;
  void                        *arg;
  bool                         master;
  struct fc7300_rptun_shmem_s *shmem;
  struct inode                *inode;
  struct geometry              geo;
  char                         cpuname[RPMSG_NAME_SIZE + 1];
};

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

static const char *fc7300_rptun_get_cpuname(struct rptun_dev_s *dev);
static struct rptun_rsc_s *
fc7300_rptun_get_resource(struct rptun_dev_s *dev);
static bool fc7300_rptun_is_autostart(struct rptun_dev_s *dev);
static bool fc7300_rptun_is_master(struct rptun_dev_s *dev);
static int fc7300_rptun_start(struct rptun_dev_s *dev);
static int fc7300_rptun_stop(struct rptun_dev_s *dev);
static int fc7300_rptun_notify(struct rptun_dev_s *dev, uint32_t vqid);
static int fc7300_rptun_register_callback(struct rptun_dev_s *dev,
                                         rptun_callback_t callback,
                                         void *arg);
static void fc7300_rptun_callback(uint8_t id, void *arg);
static void fc7300_rptun_reset(struct rptun_dev_s *dev, int value);
static void fc7300_rptun_panic(struct rptun_dev_s *dev);

/****************************************************************************
 * Private Data
 ****************************************************************************/

static const struct rptun_ops_s g_fc7300_rptun_ops =
{
  .get_cpuname       = fc7300_rptun_get_cpuname,
  .get_resource      = fc7300_rptun_get_resource,
  .is_autostart      = fc7300_rptun_is_autostart,
  .is_master         = fc7300_rptun_is_master,
  .start             = fc7300_rptun_start,
  .stop              = fc7300_rptun_stop,
  .notify            = fc7300_rptun_notify,
  .register_callback = fc7300_rptun_register_callback,
  .reset             = fc7300_rptun_reset,
  .panic             = fc7300_rptun_panic
};

__attribute__((section(".rptun_shmem")))
static struct fc7300_rptun_shmem_s g_fc7300_rptun_shmem = {0, };
struct fc7300_rptun_dev_s          g_fc7300_rptun_dev;

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: fc7300_rptun_get_cpuname
 ****************************************************************************/

static const char *fc7300_rptun_get_cpuname(struct rptun_dev_s *dev)
{
  struct fc7300_rptun_dev_s *priv =
    container_of(dev, struct fc7300_rptun_dev_s, rptun);

  return priv->cpuname;
}

/****************************************************************************
 * Name: fc7300_rptun_get_resource
 ****************************************************************************/

static struct rptun_rsc_s *
fc7300_rptun_get_resource(struct rptun_dev_s *dev)
{
  struct fc7300_rptun_dev_s *priv =
    container_of(dev,  struct fc7300_rptun_dev_s, rptun);
  struct rptun_rsc_s *rsc;

  if (priv->shmem != NULL)
    {
      return &priv->shmem->rsc;
    }

  priv->shmem = &g_fc7300_rptun_shmem;

  if (priv->master)
    {
      /* Perform initial setup */

      rsc = &priv->shmem->rsc;
      rsc->rsc_tbl_hdr.ver          = 1;
      rsc->rsc_tbl_hdr.num          = 1;
      rsc->rsc_tbl_hdr.reserved[0]  = 0;
      rsc->rsc_tbl_hdr.reserved[1]  = 0;
      rsc->offset[0]                = offsetof(struct rptun_rsc_s,
                                               rpmsg_vdev);

      rsc->rpmsg_vdev.type          = RSC_VDEV;
      rsc->rpmsg_vdev.id            = VIRTIO_ID_RPMSG;
      rsc->rpmsg_vdev.dfeatures     = 1 << VIRTIO_RPMSG_F_NS
                                    | 1 << VIRTIO_RPMSG_F_ACK
                                    | 1 << VIRTIO_RPMSG_F_BUFSZ;
      rsc->rpmsg_vdev.config_len    = sizeof(struct fw_rsc_config);
      rsc->rpmsg_vdev.num_of_vrings = VRINGS;

      rsc->rpmsg_vring0.align       = VRING_ALIGN;
      rsc->rpmsg_vring0.num         = VRING_NR;
      rsc->rpmsg_vring0.notifyid    = VRING0_NOTIFYID;
      rsc->rpmsg_vring1.align       = VRING_ALIGN;
      rsc->rpmsg_vring1.num         = VRING_NR;
      rsc->rpmsg_vring1.notifyid    = VRING1_NOTIFYID;
      rsc->config.r2h_buf_size      = VRING_SIZE;
      rsc->config.h2r_buf_size      = VRING_SIZE;

      priv->shmem->base             = (uintptr_t)priv->shmem;
    }
  else
    {
      /* TODO: use MB */

      while (priv->shmem->base == 0)
        {
          nxsig_usleep(100);
        }
    }

  return &priv->shmem->rsc;
}

/****************************************************************************
 * Name: fc7300_rptun_is_autostart
 ****************************************************************************/

static bool fc7300_rptun_is_autostart(struct rptun_dev_s *dev)
{
  return true;
}

/****************************************************************************
 * Name: fc7300_rptun_is_master
 ****************************************************************************/

static bool fc7300_rptun_is_master(struct rptun_dev_s *dev)
{
  struct fc7300_rptun_dev_s *priv =
    container_of(dev, struct fc7300_rptun_dev_s, rptun);
  return priv->master;
}

/****************************************************************************
 * Name: fc7300_rptun_start
 ****************************************************************************/

static int fc7300_rptun_start(struct rptun_dev_s *dev)
{
  return 0;
}

/****************************************************************************
 * Name: fc7300_rptun_stop
 ****************************************************************************/

static int fc7300_rptun_stop(struct rptun_dev_s *dev)
{
  return 0;
}

/****************************************************************************
 * Name: fc7300_rptun_notify
 ****************************************************************************/

static int fc7300_rptun_notify(struct rptun_dev_s *dev, uint32_t vqid)
{
  struct fc7300_rptun_dev_s *priv =
    container_of(dev, struct fc7300_rptun_dev_s, rptun);
  uint8_t core_id = fc7300_cpu_index();

  if (core_id == 0)
    {
      /* Notify slave that RX is ready */

      FC7300_MB_RELEASE_SEMA(priv->mailbox, RPTUN_MB_CHAN_RX_SLAVE);
      FC7300_MB_SEND_DATA(priv->mailbox, RPTUN_MB_CHAN_RX_SLAVE,
                          fc7300_mb_core1, 0, 0);
    }
  else if (core_id == 1)
    {
      /* Notify master that RX is ready */

      FC7300_MB_RELEASE_SEMA(priv->mailbox, RPTUN_MB_CHAN_RX_MASTER);
      FC7300_MB_SEND_DATA(priv->mailbox, RPTUN_MB_CHAN_RX_MASTER,
                          fc7300_mb_core0, 0, 0);
    }
  else if (core_id == 2)
    {
      /* Not supported for core2 */
    }
  else
    {
      DEBUGASSERT(0);
    }

  return 0;
}

/****************************************************************************
 * Name: fc7300_rptun_register_callback
 ****************************************************************************/

static int fc7300_rptun_register_callback(struct rptun_dev_s *dev,
                                       rptun_callback_t callback,
                                       void *arg)
{
  struct fc7300_rptun_dev_s *priv =
    container_of(dev, struct fc7300_rptun_dev_s, rptun);

  priv->callback = callback;
  priv->arg      = arg;

  return 0;
}

/****************************************************************************
 * Name: fc7300_rptun_reset
 ****************************************************************************/

static void fc7300_rptun_reset(struct rptun_dev_s *dev, int value)
{
  struct fc7300_rptun_dev_s *priv =
    container_of(dev, struct fc7300_rptun_dev_s, rptun);
  uint8_t core_id = fc7300_cpu_index();

  if ((value == 0) && (core_id == 0))
    {
      /* Soft reset */

      FC7300_MB_RELEASE_SEMA(priv->mailbox, RPTUN_MB_CHAN_RESET);
      FC7300_MB_SEND_DATA(priv->mailbox, RPTUN_MB_CHAN_RESET, fc7300_mb_core1,
                          0, 0);
    }
}

/****************************************************************************
 * Name: fc7300_rptun_panic
 ****************************************************************************/

static void fc7300_rptun_panic(struct rptun_dev_s *dev)
{
  struct fc7300_rptun_dev_s *priv =
    container_of(dev, struct fc7300_rptun_dev_s, rptun);
  uint8_t core_id = fc7300_cpu_index();

  if (core_id == 0)
    {
      FC7300_MB_RELEASE_SEMA(priv->mailbox, RPTUN_MB_CHAN_PANIC);
      FC7300_MB_SEND_DATA(priv->mailbox, RPTUN_MB_CHAN_PANIC, fc7300_mb_core1,
                          0, 0);
    }
  else if (core_id == 1)
    {
      FC7300_MB_RELEASE_SEMA(priv->mailbox, RPTUN_MB_CHAN_PANIC);
      FC7300_MB_SEND_DATA(priv->mailbox, RPTUN_MB_CHAN_PANIC, fc7300_mb_core0,
                          0, 0);
    }
  else if (core_id == 2)
    {
      /* Not supported for core2 */
    }
  else
    {
      DEBUGASSERT(0);
    }
}

/****************************************************************************
 * Name: fc7300_rptun_mailbox_regcallback
 ****************************************************************************/

static void fc7300_rptun_mailbox_regcallback(struct fc7300_rptun_dev_s *dev)
{
  uint8_t core_id = fc7300_cpu_index();
  DEBUGASSERT(dev);

  if (core_id == 0)
    {
      FC7300_MB_REG_REQ_CBK(dev->mailbox, RPTUN_MB_CHAN_RX_MASTER,
                            fc7300_rptun_callback, dev);
      FC7300_MB_REG_REQ_CBK(dev->mailbox, RPTUN_MB_CHAN_PANIC,
                            fc7300_rptun_callback, dev);
    }
  else if (core_id == 1)
    {
      FC7300_MB_REG_REQ_CBK(dev->mailbox, RPTUN_MB_CHAN_RX_SLAVE,
                            fc7300_rptun_callback, dev);
      FC7300_MB_REG_REQ_CBK(dev->mailbox, RPTUN_MB_CHAN_RESET,
                            fc7300_rptun_callback, dev);
      FC7300_MB_REG_REQ_CBK(dev->mailbox, RPTUN_MB_CHAN_PANIC,
                            fc7300_rptun_callback, dev);
    }
  else if (core_id == 2)
    {
      /* Not supported for core2 */
    }
  else
    {
      DEBUGASSERT(0);
    }
}

#if CONFIG_CPU_COREID == 0

static void fc7300_write_dump(struct fc7300_rptun_dev_s *dev)
{
  int ret;

  struct mtd_dev_s *mtd_flash = fc7300_nordflash_mtd();
  uint32_t block_idx =  FC7300_DFLASH_TRAPINFO_OFFSET / FC7300_FLASH_SECTOR_SIZE;
  ret = MTD_ERASE(mtd_flash, block_idx, 1);
  if (ret < 0)
    {
      rpmsgerr("error: erase coredump partition failed, ret:%d\n", ret);
      return;
    }

  extern uint8_t _sdumpshare[];
  ret = MTD_BWRITE(mtd_flash, FC7300_DFLASH_TRAPINFO_OFFSET / FC7300_FLASH_PAGE_SIZE,
                   FC7300_DFLASH_TRAPINFO_SIZE / FC7300_FLASH_PAGE_SIZE, (const uint8_t*)&_sdumpshare);
  if (ret < 0)
    {
      rpmsgerr("error: write coredump partition failed, ret:%d\n", ret);
      return;
    }
}

#endif

/****************************************************************************
 * Name: fc7300_rptun_callback
 ****************************************************************************/

static void fc7300_rptun_callback(uint8_t id, void *arg)
{
  struct fc7300_rptun_dev_s *dev = &g_fc7300_rptun_dev;
  uint8_t core_id = fc7300_cpu_index();
  rpmsginfo("Rptun MB %u\n", id);

  if ((core_id != 0) && (core_id != 1))
    {
      rpmsgerr("Error core_id=%u\n", core_id);
      return;
    }

  switch (id)
    {
      case RPTUN_MB_CHAN_RX_MASTER:
      case RPTUN_MB_CHAN_RX_SLAVE:
        {
          if (dev->callback != NULL)
            {
              dev->callback(dev->arg, RPTUN_NOTIFY_ALL);
            }
          break;
        }

      case RPTUN_MB_CHAN_PANIC:
        {
#if CONFIG_CPU_COREID == 0
          fc7300_write_dump(dev);
          up_systemreset();
#endif
          break;
        }

      default:
        {
          rpmsgerr("Not supported core_id=%u id=%u\n", core_id, id);
          break;
        }
    }
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

int fc7300_rptun_init(const char *cpuname, bool master)
{
  struct fc7300_rptun_dev_s *dev = &g_fc7300_rptun_dev;
  int                       ret = OK;

  dev->mailbox = fc7300_mb_getdev();
  dev->master = master;

  if (master)
    {
      /* The CM7 core0 always master */

      memset(&g_fc7300_rptun_shmem, 0, sizeof(struct fc7300_rptun_shmem_s));
    }
  else
    {
      /* do nothing */
    }

  /* Configure MB */

  fc7300_rptun_mailbox_regcallback(dev);

  /* Configure device */

  dev->rptun.ops = &g_fc7300_rptun_ops;
  strncpy(dev->cpuname, cpuname, RPMSG_NAME_SIZE);

  ret = rptun_initialize(&dev->rptun);
  if (ret < 0)
    {
      rpmsgerr("ERROR: rptun_initialize failed %d!\n", ret);
      return ret;
    }

  return ret;
}
