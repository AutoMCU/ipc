/****************************************************************************
 * @file    ipc_cfg.h
 * @brief   IPC configuration header file.
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

#ifndef IPC_CFG_H
#define IPC_CFG_H

#ifdef __cplusplus
extern "C" {
#endif

/* IPC Configuration */
#define IPC_SHM_VAR    __attribute__((section(".ipc_shm"), aligned(32)))
#define IPC_SLOT_COUNT 8U  /* Must be a power of 2 */
#define IPC_SLOT_SIZE  32U /* Max payload size in bytes */

/* Hardware Resources (IRQ) */
#define IPC_IRQ_ID_CORE1   150U
#define IPC_IRQ_PRIO_CORE1 10U
#define IPC_IRQ_ID_CORE2   151U
#define IPC_IRQ_PRIO_CORE2 10U

/* Application Endpoints */
#define IPC_ENDPOINT_CORE1 IPC_ENDPOINT_ID_0
#define IPC_ENDPOINT_CORE2 IPC_ENDPOINT_ID_1

/* Compile-time Checks */
#if ((IPC_SLOT_COUNT & (IPC_SLOT_COUNT - 1)) != 0)
#error "IPC_SLOT_COUNT must be a power of 2!"
#endif

#ifdef __cplusplus
}
#endif

#endif /* IPC_CFG_H */
