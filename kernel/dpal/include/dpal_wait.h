/*----------------------------------------------------------------------------
 * Copyright (c) CompanyNameMagicTag 2021. All rights reserved.
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright notice, this list of
 * conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list
 * of conditions and the following disclaimer in the documentation and/or other materials
 * provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used
 * to endorse or promote products derived from this software without specific prior written
 * permission.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *---------------------------------------------------------------------------*/

#ifndef DPAL_WAIT_H
#define DPAL_WAIT_H

#include "dpal_list.h"

typedef struct {
    uint32_t event_id;          /* Event mask in the event control block,
                                  indicating the event that has been logically processed. */
    dpal_dl_list_t event_list; /* Event control block linked list */
} even_cb;

typedef struct {
    size_t      rawLock;            /**< raw spinlock */
#ifdef LOSCFG_KERNEL_SMP_LOCKDEP
    uint32_t    cpuid;              /**< the cpu id when the lock is obtained. It is defined
                                         only when LOSCFG_KERNEL_SMP_LOCKDEP is defined. */
    VOID        *owner;             /**< the pointer to the lock owner's task control block.
                                         It is defined only when LOSCFG_KERNEL_SMP_LOCKDEP is
                                         defined. */
    const char  *name;              /**< the lock owner's task name. It is defined only when
                                         LOSCFG_KERNEL_SMP_LOCKDEP is defined. */
#endif
} spinlock;


/*
 * Notice about wait_queue_head_t:
 * 1.The stEvent is used for task-synchronization and has the same function as wait_event_head in Linux.
 *   In LiteOS, when wait_event is called, if the condition is not true, the task will be blocked and
 *   mounted on stEvent.stEventList. In Linux, the blocked task will be mounted on wait_queue_head.task_list.
 * 2.The lock and poll_queue are only used for poll operation: poll_queue is used to link poll_wait_node,
 *   and lock is used to protect this poll_queue.
 */
typedef struct dpal_wait {
    even_cb event;
    spinlock lock;
    dpal_dl_list_t poll_queue;
} dpal_wait_queue_head;

#endif

