/*
 * Copyright (c) 1997 Berkeley Software Design, Inc. All rights reserved.
 * Copyright (c) @CompanyNameMagicTag 2019-2020. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Berkeley Software Design Inc's name may not be used to endorse or
 *    promote products derived from this software without specific prior
 *    written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY BERKELEY SOFTWARE DESIGN INC ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL BERKELEY SOFTWARE DESIGN INC BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 */
#ifndef DEV_MMC_MUTEX_H
#define DEV_MMC_MUTEX_H

#define	mtx_init(m, n, t, o)	dpal_mux_create(&((m).mux_id))
#define	mtx_destroy(m)		    dpal_mux_delete((m).mux_id)
#define	mtx_lock(m)		        dpal_mux_pend((m).mux_id, DPAL_WAIT_FOREVER)
#define	mtx_unlock(m)		    dpal_mux_post((m).mux_id)
#define	mtx_owned(m)		    (((void *)(m).owner) == (void *)dpal_get_curr_task())

#define	MA_OWNED	1
#define	MA_NOTOWNED	2
#define	mtx_assert(m, what)	do {	\
    switch (what) {	\
    case MA_OWNED:	\
        break;	\
    case MA_NOTOWNED:	\
        if (mtx_owned(m)) {dprintf("mtx assert at %s:%d\n", __FUNCTION__, __LINE__);}	\
        break;	\
    default:	\
        {dprintf("mtx assert at %s:%d\n", __FUNCTION__, __LINE__);}	\
    }	\
} while (0)

#endif
