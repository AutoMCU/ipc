/****************************************************************************
 * @file    ipc.h
 * @brief   IPC driver header file.
 * @version 0.0.1
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

#ifndef IPC_H
#define IPC_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ipc_cfg.h"

#include <stdint.h>
#include <stdbool.h>

/*===========================================================================*/
/* Data structures and types.                                                */
/*===========================================================================*/

/**
 * @brief   IPC driver ready enumeration.
 * @details Represents the current ready of the IPC driver instance.
 */
typedef enum {
    IPC_DRIVER_STATE_UNINIT = 0U, /**< Driver is uninitialized */
    IPC_DRIVER_STATE_READY  = 1U  /**< Driver is initialized and ready for use */
} ipc_driver_state_t;

/**
 * @brief   IPC endpoint identifier enumeration.
 * @details Represents the unique identifier for each IPC endpoint.
 */
typedef enum {
    IPC_ENDPOINT_ID_0   = 0U, /**< End-point id #0, core A -> core B  */
    IPC_ENDPOINT_ID_1   = 1U, /**< End-point id #1, core B -> core A  */
    IPC_ENDPOINT_ID_NUM = 2U  /**< Number of end-points */
} ipc_endpoint_id_t;

/**
 * @brief   Memory slot structure.
 * @details Represents a physical storage block in the shared memory ring slot.
 */
typedef struct {
    uint32_t len;                 /**< Actual valid data length */
    uint8_t  data[IPC_SLOT_SIZE]; /**< Raw payload slot */
} ipc_slot_t;

/**
 * @brief   IPC queue control structure.
 * @details Manages the shared memory queue for IPC communication.
 * @note    The queue must be allocated in a shared memory region accessible by both cores.
 */
typedef struct {
    volatile uint32_t write_index;          /**< Written by producer, read by consumer */
    volatile uint32_t read_index;           /**< Written by consumer, read by producer */
    ipc_slot_t        slot[IPC_SLOT_COUNT]; /**< Slot data area */
} ipc_queue_t;

/**
 * @brief   Shared memory structure.
 * @details Represents the shared memory region used for IPC communication, containing the ready and queue for each
 * endpoint.
 * @note   The shared memory region must be allocated in a memory area that is accessible by both cores and should have
 * CACHE disabled.
 */
typedef struct {
    volatile uint32_t ready[IPC_ENDPOINT_ID_NUM]; /**< State of each IPC endpoint */
    ipc_queue_t       queue[IPC_ENDPOINT_ID_NUM]; /**< Queue for each IPC endpoint */
} ipc_shm_t;

/**
 * @brief   IPC interrupt structure.
 * @details Represents the interrupt configuration for IPC events.
 */
typedef struct {
    uint32_t id;   /**< IRQ ID of the current core */
    uint32_t prio; /**< IRQ priority of the current core */
} ipc_irq_t;

/**
 * @brief   IPC configuration structure.
 * @details Represents the configuration for an IPC driver instance, including the endpoint ID, shared memory region,
 * and interrupt configuration.
 */
typedef struct {
    ipc_endpoint_id_t endpoint_id; /**< Unique identifier for the IPC endpoint */
    ipc_shm_t        *shm;         /**< Pointer to the shared memory region */
    const ipc_irq_t  *irq;         /**< Pointer to the interrupt configuration */
} ipc_config_t;

/**
 * @brief   IPC driver instance.
 * @details Encapsulates the ready and configuration of the IPC driver.
 */
typedef struct {
    const ipc_config_t *config; /**< Pointer to the IPC configuration */
} ipc_driver_t;

/*===========================================================================*/
/* Global functions.                                                         */
/*===========================================================================*/

/**
 * @brief Check the ready state of both the local and remote cores.
 * @details This function checks the ready state of the both cores by accessing the shared memory region.
 *
 * @param[in] ipcd Pointer to the IPC driver instance
 *
 * @return bool
 * @retval true if the both cores are ready
 * @retval false if the both cores are uninitialized or any input pointer is NULL
 */
bool ipc_is_ready(ipc_driver_t *ipcd);

/**
 * @brief Initialize the IPC driver instance
 * @details This function initializes the IPC driver instance with the provided configuration, setting up the shared
 * memory queue and interrupt handling.
 *
 * @param[in] ipcd Pointer to the IPC driver instance
 * @param[in] config Pointer to the IPC configuration structure
 *
 * @return bool
 * @retval true if initialization is successful
 * @retval false if any of the input pointers are NULL
 */
bool ipc_init(ipc_driver_t *ipcd, const ipc_config_t *config);

/**
 * @brief De-initialize the IPC driver instance
 * @details This function de-initializes the IPC driver instance, releasing any resources and resetting the ready.
 *
 * @param[in] ipcd Pointer to the IPC driver instance
 *
 * @return bool
 * @retval true if de-initialization is successful
 * @retval false if the input pointer is NULL
 */
bool ipc_deinit(ipc_driver_t *ipcd);

/**
 * @brief Send data through IPC with memory copy.
 * @details This function sends data through the IPC mechanism by copying the provided data into an allocated slot in
 * the shared memory queue. It handles the allocation of the slot, copying of data, and committing the slot to the
 * queue.
 *
 * @param[in] ipcd Pointer to the IPC driver instance
 * @param[in] data Pointer to the data slot to be sent
 * @param[in] len Length of the data to be sent
 *
 * @return bool
 * @retval true if the data was successfully sent
 * @retval false if the queue is full or any input is invalid (e.g., NULL pointers or length exceeding slot size)
 */
bool ipc_send(ipc_driver_t *ipcd, const uint8_t *data, uint16_t len);

/**
 * @brief Receive data through IPC with memory copy.
 * @details This function receives data through the IPC mechanism by copying the data from an acquired slot in the
 * shared memory queue into the provided slot. It handles the acquisition of the slot, copying of data, and
 * releasing the slot back to the queue.
 *
 * @param[in] ipcd Pointer to the IPC driver instance
 * @param[out] data Pointer to the slot where received data will be copied
 * @param[out] len Pointer to a variable that specifies the actual number of bytes received
 *
 * @return bool
 * @retval true if data was successfully received
 * @retval false if the queue is empty or any input is invalid (e.g., NULL pointers or length exceeding slot size)
 */
bool ipc_receive(ipc_driver_t *ipcd, uint8_t *data, uint16_t *len);

/**
 * @brief Producer allocates a slot for Zero-Copy sending.
 * @details This function allows the producer to allocate a slot in the shared memory queue for Zero-Copy sending.
 *
 * @param[in] ipcd Pointer to the IPC driver instance
 * @param[out] slot Double pointer to receive the allocated slot address
 *
 * @return bool
 * @retval true if a slot was successfully allocated
 * @retval false if the queue is full or any input pointer is NULL
 */
bool ipc_slot_allocate(ipc_driver_t *ipcd, ipc_slot_t **slot);

/**
 * @brief Producer commits the allocated slot to the queue
 * @details This function allows the producer to commit the allocated slot to the shared memory queue after writing data
 * into it.
 *
 * @param[in] ipcd Pointer to the IPC driver instance.
 * @param[in] slot Pointer to the allocated slot
 *
 * @return bool
 * @retval true if the slot was successfully committed
 * @retval false if any input pointer is NULL
 */
bool ipc_slot_send(ipc_driver_t *ipcd, ipc_slot_t *slot);

/**
 * @brief Consumer acquires a slot for Zero-Copy receiving.
 * @details This function allows the consumer to acquire a slot from the shared memory queue for Zero-Copy receiving.
 *
 * @param[in] ipcd Pointer to the IPC driver instance
 * @param[out] slot Double pointer to receive the acquired slot address
 *
 * @return bool
 * @retval true if a slot was successfully acquired
 * @retval false if the queue is empty or any input pointer is NULL
 */
bool ipc_slot_receive(ipc_driver_t *ipcd, ipc_slot_t **slot);

/**
 * @brief Consumer releases the processed slot back to the queue.
 * @details This function allows the consumer to release a processed slot back to the shared memory queue.
 *
 * @param ipcd Pointer to the IPC driver instance
 * @param slot Pointer to the processed slot
 *
 * @return bool
 * @retval true if the slot was successfully released
 * @retval false if any input pointer is NULL
 */
bool ipc_slot_release(ipc_driver_t *ipcd, ipc_slot_t *slot);

/**
 * @brief Notify the remote core about new data availability.
 * @details This function triggers an interrupt to notify the remote core that new data is available in the shared
 * memory queue.
 *
 * @param[in] ipcd Pointer to the IPC driver instance
 *
 * @return bool
 * @retval true if the notification was successfully sent
 * @retval false if any input pointer is NULL
 */
bool ipc_notify(ipc_driver_t *ipcd);

#ifdef __cplusplus
}
#endif

#endif /* IPC_H */
