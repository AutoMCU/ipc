/****************************************************************************
 * @file    ipc.c
 * @brief   IPC driver source file.
 * @version 0.1.0
 ****************************************************************************/
/*
 * Copyright (c) 2026 AutoMCU
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifdef __cplusplus
extern "C" {
#endif

#include "../include/ipc.h"

#include <string.h>

#include "../include/ipc_port.h"

/*===========================================================================*/
/* Local functions.                                                          */
/*===========================================================================*/

static inline bool ipc_queue_is_full(ipc_queue_t *queue)
{
    return ((queue->write_index - queue->read_index) >= IPC_SLOT_COUNT);
}

static inline bool ipc_queue_is_empty(ipc_queue_t *queue)
{
    return (queue->write_index == queue->read_index);
}

static inline uint32_t ipc_get_slot_index(uint32_t idx)
{
    return idx & (IPC_SLOT_COUNT - 1);
}

/*===========================================================================*/
/* Global functions.                                                         */
/*===========================================================================*/

bool ipc_is_ready(ipc_driver_t *ipcd)
{
    bool ready = false;

    if ((ipcd != NULL) && (ipcd->config != NULL) && (ipcd->config->shm != NULL)) {
        if ((ipcd->config->shm->ready[ipcd->config->endpoint_id] == IPC_DRIVER_STATE_READY) &&
            (ipcd->config->shm->ready[1U - ipcd->config->endpoint_id] == IPC_DRIVER_STATE_READY)) {
            ready = true;
        }
    }

    return ready;
}

bool ipc_init(ipc_driver_t *ipcd, const ipc_config_t *config)
{
    bool ret = false;

    if ((ipcd != NULL) && (config != NULL) && (config->shm != NULL)) {
        ipcd->config = config;
        if (ipcd->config->endpoint_id == IPC_ENDPOINT_ID_0) {
            /* Initialize the shared memory region. */
            memset(ipcd->config->shm, 0U, sizeof(ipc_shm_t));
        }
        /* Initialize the rx interrupt configuration */
        if (ipcd->config->irq != NULL) {
            ipc_port_set_irq_priority(ipcd->config->irq->id, ipcd->config->irq->prio);
            ipc_port_enable_irq(ipcd->config->irq->id);
        }
        /* Initialize the ready state for the local core */
        ipcd->config->shm->ready[ipcd->config->endpoint_id] = IPC_DRIVER_STATE_READY;
        ret                                                 = true;
    }

    return ret;
}

bool ipc_deinit(ipc_driver_t *ipcd)
{
    bool ret = false;

    if (ipcd != NULL && ipcd->config != NULL && ipcd->config->shm != NULL) {
        /* Deinitialize the ready state for the local core */
        ipcd->config->shm->ready[ipcd->config->endpoint_id] = IPC_DRIVER_STATE_UNINIT;
        if (ipcd->config->irq != NULL) {
            ipc_port_disable_irq(ipcd->config->irq->id);
        }
        ipcd->config = NULL;
        ret          = true;
    }

    return ret;
}

bool ipc_send(ipc_driver_t *ipcd, const uint8_t *data, uint16_t len)
{
    bool        ret  = false;
    ipc_slot_t *slot = NULL;

    if ((ipc_is_ready(ipcd) == true) && (data != NULL) && (len <= IPC_SLOT_SIZE)) {
        ret = ipc_slot_allocate(ipcd, &slot);
        if (ret) {
            memcpy(slot->data, data, len);
            slot->len = len;
            ret       = ipc_slot_send(ipcd, slot);
        }
    }

    return ret;
}

bool ipc_receive(ipc_driver_t *ipcd, uint8_t *data, uint16_t *len)
{
    bool        ret  = false;
    ipc_slot_t *slot = NULL;

    if ((ipc_is_ready(ipcd) == true) && (data != NULL) && (len != NULL)) {
        ret = ipc_slot_receive(ipcd, &slot);
        if (ret) {
            memcpy(data, slot->data, slot->len);
            *len = slot->len;
            ret  = ipc_slot_release(ipcd, slot);
        }
    }

    return ret;
}

bool ipc_slot_allocate(ipc_driver_t *ipcd, ipc_slot_t **slot)
{
    bool         ret   = false;
    ipc_queue_t *queue = NULL;

    if ((ipc_is_ready(ipcd) == true) && (slot != NULL)) {
        queue = &ipcd->config->shm->queue[ipcd->config->endpoint_id];
        if (ipc_queue_is_full(queue) == false) {
            *slot = &queue->slot[ipc_get_slot_index(queue->write_index)];
            ret   = true;
        }
    }

    return ret;
}

bool ipc_slot_send(ipc_driver_t *ipcd, ipc_slot_t *slot)
{
    bool         ret   = false;
    ipc_queue_t *queue = NULL;

    if ((ipc_is_ready(ipcd) == true) && (slot != NULL)) {
        queue = &ipcd->config->shm->queue[ipcd->config->endpoint_id];
        /* Ensure that the data is written to memory before updating the write index. */
        ipc_port_dmb();
        ++queue->write_index;
        ret = true;
    }

    return ret;
}

bool ipc_slot_receive(ipc_driver_t *ipcd, ipc_slot_t **slot)
{
    bool         ret   = false;
    ipc_queue_t *queue = NULL;

    if ((ipc_is_ready(ipcd) == true) && (slot != NULL)) {
        queue = &ipcd->config->shm->queue[1U - ipcd->config->endpoint_id];
        if (ipc_queue_is_empty(queue) == false) {
            *slot = &queue->slot[ipc_get_slot_index(queue->read_index)];
            /* Load the producer's write index before accessing the slot */
            ipc_port_dmb();
            ret = true;
        }
    }

    return ret;
}

bool ipc_slot_release(ipc_driver_t *ipcd, ipc_slot_t *slot)
{
    bool         ret   = false;
    ipc_queue_t *queue = NULL;

    if ((ipc_is_ready(ipcd) == true) && (slot != NULL)) {
        queue     = &ipcd->config->shm->queue[1U - ipcd->config->endpoint_id];
        slot->len = 0U;
        /* Ensure that the data is read from memory before updating the read index. */
        ipc_port_dmb();
        ++queue->read_index;
        ret = true;
    }

    return ret;
}

bool ipc_notify(ipc_driver_t *ipcd)
{
    bool ret = false;

    if (ipc_is_ready(ipcd) == true) {
        /* Notify the remote core. */
        ipc_port_notify();
        ret = true;
    }

    return ret;
}

#ifdef __cplusplus
}
#endif
