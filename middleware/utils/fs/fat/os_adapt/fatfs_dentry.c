/*----------------------------------------------------------------------------
 * Copyright (c) @CompanyNameMagicTag 2021. All rights reserved.
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

#include "inode/inode.h"
#include "ff.h"
#include "fatfs_dentry.h"

#ifdef LOSCFG_FS_FAT_DCACHE
STATIC int32_t get_filename(const WCHAR *lfnbuf, CHAR *name, int32_t nameLen)
{
    int i;

    for (i = 0; i < nameLen; i++) {
        name[i] = (CHAR)lfnbuf[i];
        if (name[i] == 0) {
            return ENOERR;
        }
    }

    return -EPERM;
}

LosDentry* f_dentry_new(LosDentry *parent, const WCHAR* lfnbuf)
{
    LosDentry *dentry;
    char name[FF_MAX_LFN + 1];

    if (parent == NULL) {
        return NULL;
    }

    /* lfnbuf type is unsigned short, can't use strcpy */
    if (get_filename(lfnbuf, name, sizeof(name)) != ENOERR) {
        return NULL;
    }
    dentry = DentryLookup(parent, name);
    if (dentry != NULL) {
        return dentry;
    }
    return DentryCreate(parent, name);
}

VOID f_dentry_destroy(LosDentry *dentry)
{
    DentryDestroy(dentry);
}

VOID f_dentry_mode_update(LosDentry *dentry, BYTE fattrib)
{
    if (dentry == NULL) {
        return;
    }
    dentry->mode = S_IFREG | S_IRUSR | S_IRGRP | S_IROTH |
                   S_IWUSR | S_IWGRP | S_IWOTH;
    if (fattrib & AM_RDO)
        dentry->mode &= ~(S_IWUSR | S_IWGRP | S_IWOTH);

    if (fattrib & AM_DIR) {
        dentry->mode &= ~S_IFREG;
        dentry->mode |= S_IFDIR | S_IXUSR | S_IXGRP | S_IXOTH;
    }
}

VOID f_dentry_mtime_update(LosDentry *dentry, const struct timespec *ts)
{
    if (dentry != NULL) {
        DentryTimeUpdate(dentry, ts, MTIME);
    }
}

VOID f_dentry_ctime_update(LosDentry *dentry, const struct timespec *ts)
{
    if (dentry != NULL) {
        DentryTimeUpdate(dentry, ts, CTIME);
    }
}

VOID f_dentry_size_update(LosDentry *dentry, FSIZE_t size)
{
    if (dentry != NULL) {
        DentrySizeUpdate(dentry, size);
    }
}

VOID f_dentry_blocksize_update(LosDentry *dentry, FSIZE_t blksize)
{
    if (dentry != NULL) {
        DentryBlockSizeUpdate(dentry, blksize);
    }
}

BOOL f_dentry_support(VOID)
{
    return TRUE;
}

#else /* !LOSCFG_FS_FAT_DCACHE */

LosDentry* f_dentry_new(LosDentry *parent, const WCHAR* lfnbuf)
{
    return NULL;
}

VOID f_dentry_destroy(LosDentry *dentry) { }
VOID f_dentry_mode_update(LosDentry *dentry, BYTE fattrib) { }
VOID f_dentry_mtime_update(LosDentry *dentry, const struct timespec *ts) { }
VOID f_dentry_ctime_update(LosDentry *dentry, const struct timespec *ts) { }
VOID f_dentry_size_update(LosDentry *dentry, FSIZE_t size) { }
VOID f_dentry_blocksize_update(LosDentry *dentry, FSIZE_t blksize) { }

BOOL f_dentry_support(VOID)
{
    return FALSE;
}

#endif /* LOSCFG_FS_FAT_DCACHE */

