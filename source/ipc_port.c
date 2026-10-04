/****************************************************************************
 * @file    ipc_port.c
 * @brief   IPC port source file.
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

#include "../include/ipc_port.h"

/*===========================================================================*/
/* Global functions.                                                         */
/*===========================================================================*/

void ipc_port_dmb(void)
{
    // __DMB();
}

void ipc_port_notify(void)
{
    // __SEV();
}

void ipc_port_enable_irq(uint32_t id)
{
    // __NVIC_EnableIRQ(id);
}

void ipc_port_disable_irq(uint32_t id)
{
    // __NVIC_DisableIRQ(id);
}

void ipc_port_set_irq_priority(uint32_t id, uint32_t priority)
{
    // __NVIC_SetPriority(id, priority);
}

#ifdef __cplusplus
}
#endif
