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

#include <debug.h>
#include <string.h>

#include <nuttx/drivers/addrenv.h>
#include <nuttx/nuttx.h>
#include <nuttx/rpmsg/rpmsg_virtio.h>

#include "fc7300_mb.h"
#include "fc7300_multicore.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#define RPMSG_VIRTIO_MB_CHAN_RX_MASTER  (0)             /* Notify master */
#define RPMSG_VIRTIO_MB_CHAN_RX_SLAVE   (1)             /* Notify slave */

#define VRINGS                   (2)             /* Number of vrings */
#define VRING_ALIGN              (8)             /* Vring alignment */
#define VRING_NR                 (8)             /* Number of descriptors */
#define VRING_SIZE               (256)           /* Size of one descriptor */
#define VRING_RSVD               (1024)
#define VRING0_NOTIFYID          (RSC_NOTIFY_ID_ANY)  /* Vring0 id */
#define VRING1_NOTIFYID          (RSC_NOTIFY_ID_ANY)  /* Vring1 id */

/****************************************************************************
 * Private Types
 ****************************************************************************/

struct fc7300_rpmsg_virtio_shmem_s
{
  volatile uintptr_t        base;
  struct rpmsg_virtio_rsc_s rsc;
  char                      buf[VRINGS * VRING_NR * VRING_SIZE + VRING_RSVD];
};

struct fc7300_rpmsg_virtio_dev_s
{
  struct rpmsg_virtio_s              dev;
  struct fc7300_mb_dev_s             *mailbox;     /* fc7300 mailbox driver */
  rpmsg_virtio_callback_t            callback;
  void                               *arg;
  int                                master;
  struct fc7300_rpmsg_virtio_shmem_s *shmem;
  struct simple_addrenv_s            addrenv[2];
  char                               cpuname[RPMSG_NAME_SIZE + 1];
};

static const char *fc7300_rpmsg_virtio_get_cpuname(struct rpmsg_virtio_s *dev);
static struct rpmsg_virtio_rsc_s *
fc7300_rpmsg_virtio_get_resource(struct rpmsg_virtio_s *dev);
static int fc7300_rpmsg_virtio_is_master(struct rpmsg_virtio_s *dev);
static int fc7300_rpmsg_virtio_notify(struct rpmsg_virtio_s *dev,
                                      uint32_t vqid);
static int
fc7300_rpmsg_virtio_register_callback(struct rpmsg_virtio_s *dev,
                                      rpmsg_virtio_callback_t callback,
                                      void *arg);

/****************************************************************************
 * Private Data
 ****************************************************************************/

static struct fc7300_rpmsg_virtio_shmem_s
g_fc7300_rpmsg_shmem locate_data(".rptun_shmem");
static struct fc7300_rpmsg_virtio_dev_s g_fc7300_rpmsg_dev;
static const struct rpmsg_virtio_ops_s g_fc7300_rpmsg_virtio_ops =
{
  .get_cpuname       = fc7300_rpmsg_virtio_get_cpuname,
  .get_resource      = fc7300_rpmsg_virtio_get_resource,
  .is_master         = fc7300_rpmsg_virtio_is_master,
  .notify            = fc7300_rpmsg_virtio_notify,
  .register_callback = fc7300_rpmsg_virtio_register_callback,
};

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: fc7300_rpmsg_virtio_get_cpuname
 ****************************************************************************/

static const char *fc7300_rpmsg_virtio_get_cpuname(struct rpmsg_virtio_s *dev)
{
  struct fc7300_rpmsg_virtio_dev_s *priv =
    container_of(dev, struct fc7300_rpmsg_virtio_dev_s, dev);

  return priv->cpuname;
}

/****************************************************************************
 * Name: fc7300_rpmsg_virtio_get_resource
 ****************************************************************************/

static struct rpmsg_virtio_rsc_s *
fc7300_rpmsg_virtio_get_resource(struct rpmsg_virtio_s *dev)
{
  struct fc7300_rpmsg_virtio_dev_s *priv =
    container_of(dev, struct fc7300_rpmsg_virtio_dev_s, dev);
  struct rpmsg_virtio_rsc_s *rsc;

  priv->shmem = &g_fc7300_rpmsg_shmem;
  if (!priv->shmem)
    {
      return NULL;
    }

  rsc = &priv->shmem->rsc;

  if (priv->master)
    {
      memset(priv->shmem, 0, sizeof(*priv->shmem));
      rsc->rpmsg_vdev.id            = VIRTIO_ID_RPMSG;
      rsc->rpmsg_vdev.dfeatures     = 1 << VIRTIO_RPMSG_F_NS |
                                      1 << VIRTIO_RPMSG_F_ACK;
      rsc->rpmsg_vdev.config_len    = sizeof(struct fw_rsc_config);
      rsc->rpmsg_vdev.num_of_vrings = VRINGS;
      rsc->rpmsg_vring0.da          = 0;
      rsc->rpmsg_vring0.align       = VRING_ALIGN;
      rsc->rpmsg_vring0.num         = VRING_NR;
      rsc->rpmsg_vring1.da          = 0;
      rsc->rpmsg_vring1.align       = VRING_ALIGN;
      rsc->rpmsg_vring1.num         = VRING_NR;
      rsc->config.r2h_buf_size      = VRING_SIZE;
      rsc->config.h2r_buf_size      = VRING_SIZE;

      priv->shmem->base = (uintptr_t)priv->shmem;
    }
  else
    {
      /* Wait untils master is ready */

      while (priv->shmem->base == 0)
        {
          usleep(100);
        }

      priv->addrenv[0].va   = (uintptr_t)priv->shmem;
      priv->addrenv[0].pa   = priv->shmem->base;
      priv->addrenv[0].size = sizeof(*priv->shmem);

      simple_addrenv_initialize(&priv->addrenv[0]);
    }

  return rsc;
}

static int fc7300_rpmsg_virtio_is_master(struct rpmsg_virtio_s *dev)
{
  struct fc7300_rpmsg_virtio_dev_s *priv =
    container_of(dev, struct fc7300_rpmsg_virtio_dev_s, dev);

  return priv->master;
}

/****************************************************************************
 * Name: fc7300_rpmsg_callback
 ****************************************************************************/

static void fc7300_rpmsg_callback(uint8_t id, void *arg)
{
  struct fc7300_rpmsg_virtio_dev_s *priv = &g_fc7300_rpmsg_dev;
  uint8_t core_id = fc7300_cpu_index();
  rpmsginfo("Rpmsg MB %u\n", id);

  if ((core_id != 0) && (core_id != 1))
    {
      rpmsgerr("Error core_id=%u\n", core_id);
      return;
    }

  switch (id)
    {
      case RPMSG_VIRTIO_MB_CHAN_RX_MASTER:
      case RPMSG_VIRTIO_MB_CHAN_RX_SLAVE:
        {
          if (priv->callback != NULL)
            {
              priv->callback(priv->arg, RPMSG_VIRTIO_NOTIFY_ALL);
            }
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
 * Name: fc7300_rpmsg_virtio_register_callback
 ****************************************************************************/

static int fc7300_rpmsg_virtio_register_callback(struct rpmsg_virtio_s *dev,
                                           rpmsg_virtio_callback_t callback,
                                           void *arg)
{
  struct fc7300_rpmsg_virtio_dev_s *priv =
    container_of(dev, struct fc7300_rpmsg_virtio_dev_s, dev);
  uint8_t core_id = fc7300_cpu_index();
  DEBUGASSERT(dev);

  priv->callback = callback;
  priv->arg      = arg;

  if (core_id == 0)
    {
      FC7300_MB_REG_REQ_CBK(priv->mailbox, RPMSG_VIRTIO_MB_CHAN_RX_MASTER,
                            fc7300_rpmsg_callback, dev);
    }
  else if (core_id == 1)
    {
      FC7300_MB_REG_REQ_CBK(priv->mailbox, RPMSG_VIRTIO_MB_CHAN_RX_SLAVE,
                            fc7300_rpmsg_callback, dev);
    }
  else if (core_id == 2)
    {
      /* Not supported for core2 */

      rpmsgerr("Not supported core_id=%u", core_id);
    }
  else
    {
      DEBUGASSERT(0);
    }

  return 0;
}

/****************************************************************************
 * Name: fc7300_rpmsg_virtio_notify
 ****************************************************************************/

static int fc7300_rpmsg_virtio_notify(struct rpmsg_virtio_s *dev, uint32_t vqid)
{
  struct fc7300_rpmsg_virtio_dev_s *priv =
    container_of(dev, struct fc7300_rpmsg_virtio_dev_s, dev);
  uint8_t core_id = fc7300_cpu_index();

  if (core_id == 0)
    {
      /* Notify slave that RX is ready */

      FC7300_MB_RELEASE_SEMA(priv->mailbox, RPMSG_VIRTIO_MB_CHAN_RX_SLAVE);
      FC7300_MB_SEND_DATA(priv->mailbox, RPMSG_VIRTIO_MB_CHAN_RX_SLAVE, fc7300_mb_core1,
                          0, 0);
    }
  else if (core_id == 1)
    {
      /* Notify master that RX is ready */

      FC7300_MB_RELEASE_SEMA(priv->mailbox, RPMSG_VIRTIO_MB_CHAN_RX_MASTER);
      FC7300_MB_SEND_DATA(priv->mailbox, RPMSG_VIRTIO_MB_CHAN_RX_MASTER, fc7300_mb_core0,
                          0, 0);
    }
  else if (core_id == 2)
    {
      /* Not supported for core2 */

      rpmsgerr("Not supported core_id=%u", core_id);
    }
  else
    {
      DEBUGASSERT(0);
    }

  return 0;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

int fc7300_rpmsg_virtio_init(const char *cpuname, bool master)
{
  struct fc7300_rpmsg_virtio_dev_s *priv = &g_fc7300_rpmsg_dev;
  int ret;

  priv->mailbox = fc7300_mb_getdev();

  priv->master = master;
  priv->dev.ops = &g_fc7300_rpmsg_virtio_ops;
  strlcpy(priv->cpuname, cpuname, RPMSG_NAME_SIZE);

  ret = rpmsg_virtio_initialize(&priv->dev);
  if (ret < 0)
    {
      rpmsgerr("rpmsg_virtio_initialize failed: %d\n", ret);
      return ret;
    }

  return ret;
}
