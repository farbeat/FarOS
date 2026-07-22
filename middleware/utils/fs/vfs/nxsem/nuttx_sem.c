/* ---------------------------------------------------------------------------
 * Copyright (c) @CompanyNameMagicTag 2021-2021. All rights reserved.
 * Description: Adapt the NuttX semaphore interface
 * Create: 2021-01-25
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
 * --------------------------------------------------------------------------- */

#include "dpal_typedef.h"
#include "dpal_errno.h"
#include "dpal_sam.h"
#include "nuttx.h"

int nxsem_init(nu_sem *sem, int pshared, unsigned int value)
{
    (void)pshared;
    if (sem == NULL || value > SEM_COUNT_MAX) {
        return -EINVAL;
    }
    return dpal_sem_init(sem, value);
}

int nxsem_post(nu_sem *sem)
{
    if (sem == NULL) {
        return -EINVAL;
    }
    dpal_sem_up(sem);
    return 0;
}

int nxsem_wait(nu_sem *sem)
{
    if (sem == NULL) {
        return -EINVAL;
    }
    return dpal_sem_down_interruptible(sem);
}

int nxsem_destroy(nu_sem *sem)
{
    if (sem == NULL) {
        return -EINVAL;
    }
    dpal_sem_destroy(sem);
    return 0;
}

