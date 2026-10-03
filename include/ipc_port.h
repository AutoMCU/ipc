/****************************************************************************
 * @file    ipc_port.h
 * @brief   IPC port header file.
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

#ifndef IPC_PORT_H
#define IPC_PORT_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/*===========================================================================*/
/* Global functions.                                                         */
/*===========================================================================*/

/**
 * @brief   Perform a memory barrier operation.
 * @details This function performs a memory barrier operation to ensure proper synchronization between the local and
 *          remote cores.
 *
 * @return  void
 */
void ipc_port_dmb(void);

/**
 * @brief   Notify the remote core that a message has been received.
 * @details This function sends a notification to the remote core that a message has been received, allowing it to
 * proceed with processing the message.
 *
 * @return  void
 */
void ipc_port_notify(void);

/**
 * @brief   Enable the interrupt for the specified port.
 * @details This function enables the interrupt for the specified port, allowing the remote core to receive messages.
 *
 * @param[in] id The ID of the port to enable.
 *
 * @return  void
 */
void ipc_port_enable_irq(uint32_t id);

/**
 * @brief   Disable the interrupt for the specified port.
 * @details This function disables the interrupt for the specified port, preventing the remote core from receiving
 * messages.
 *
 * @param[in] id The ID of the port to disable.
 *
 * @return  void
 */
void ipc_port_disable_irq(uint32_t id);

/**
 * @brief   Set the priority of the interrupt for the specified port.
 * @details This function sets the priority of the interrupt for the specified port, allowing the remote core to
 * prioritize message processing.
 *
 * @param[in] id The ID of the port to set the priority for.
 * @param[in] priority The priority to set for the interrupt.
 *
 * @return  void
 */
void ipc_port_set_irq_priority(uint32_t id, uint32_t priority);

#ifdef __cplusplus
}
#endif

#endif /* IPC_PORT_H */
