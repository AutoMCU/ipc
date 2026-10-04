/****************************************************************************
 * @file    ipc_cfg.c
 * @brief   IPC configuration source file.
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

#include "../include/ipc_cfg.h"
#include "../include/ipc.h"

/*===========================================================================*/
/* Module local variables.                                                   */
/*===========================================================================*/

/* Each ipc shared memory region is used for bidirectional communication. */
IPC_SHM_VAR static ipc_shm_t ipc_shm1;

/*===========================================================================*/
/* Module local constants.                                                   */
/*===========================================================================*/

/* Core 1 rx interrupt triggered by core 2. */
static const ipc_irq_t ipc_irq_core1 = {
    .id   = IPC_IRQ_ID_CORE1,
    .prio = IPC_IRQ_PRIO_CORE1,
};

/* Core 2 rx interrupt triggered by core 1. */
static const ipc_irq_t ipc_irq_core2 = {
    .id   = IPC_IRQ_ID_CORE2,
    .prio = IPC_IRQ_PRIO_CORE2,
};

/*===========================================================================*/
/* Module exported constants.                                                */
/*===========================================================================*/

const ipc_config_t ipc_config_core1 = {
    .endpoint_id = IPC_ENDPOINT_CORE1,
    .shm         = &ipc_shm1,
    .irq         = &ipc_irq_core1,
};

const ipc_config_t ipc_config_core2 = {
    .endpoint_id = IPC_ENDPOINT_CORE2,
    .shm         = &ipc_shm1,
    .irq         = &ipc_irq_core2,
};

#ifdef __cplusplus
}
#endif
