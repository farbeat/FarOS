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

#ifndef DPAL_TASK_H
#define DPAL_TASK_H

#include "dpal.h"
#include "dpal_typedef.h"
#include "dpal_list.h"

typedef struct {
    dpal_dl_list_t sort_link_node;
    uint32_t idx_roll_num;
} dpal_sort_link_list;

typedef void *(*tsk_entry_func)(void *param);

typedef struct {
    void              *stack_pointer;      /* Task stack pointer */
    uint16_t          task_status;         /* Task status */
    uint16_t          priority;            /* Task priority */
    uint32_t          task_flags : 31;     /* Task extend flags: taskFlags uses 8 bits now. 23 bits left */
    uint32_t          usr_stack : 1;       /* Usr Stack uses the last bit */
    uint32_t          stack_size;          /* Task stack size */
    uintptr_t         top_of_stack;        /* Task stack top */
    uint32_t          task_id;             /* Task ID */
    tsk_entry_func    task_entry;          /* Task entrance function */
    void              *task_sem;           /* Task-held semaphore */
#ifdef DPALCFG_COMPAT_POSIX
    void              *thread_join;        /* pthread adaption */
    void              *thread_join_retval; /* pthread adaption */
#endif
    void              *task_mux;           /* Task-held mutex */
    void              *args;               /* Parameter, of which the type is void * */
    char              *task_name;          /* Task name */
    dpal_dl_list_t  pend_list;             /* Task pend node */
    dpal_sort_link_list    sort_list;      /* Task sortlink node */
#ifdef DPALCFG_BASE_IPC_EVENT
    dpal_event_t      event;
    uint32_t          event_mask;          /* Event mask */
    uint32_t          event_mode;          /* Event mode */
#endif
    void              *msg;                /* Memory allocated to queues */
    uint32_t          pri_bit_map;         /* BitMap for recording the change of task priority,
                                             the priority can not be greater than 31 */
    uint32_t          signal;              /* Task signal */
#ifdef DPALCFG_BASE_CORE_TIMESLICE
    uint16_t          time_slice;          /* Remaining time slice */
#endif
} dpal_task_cb;

#endif
