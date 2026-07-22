/*----------------------------------------------------------------------------
 * Copyright (c) @CompanyNameMagicTag 2013-2015. All rights reserved.
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

#include "fs/fs.h"
#include "inode/inode.h"

#include "mtd_common.h"
static int mtdblock_open(FAR struct inode *inode_p)
{
    return 0;
}

static int mtdblock_close(FAR struct inode *inode_p)
{
    return 0;
}

static ssize_t mtdblock_read(FAR struct inode *inode_p, FAR unsigned char *buffer,
                             unsigned long long start_sector, unsigned int nsectors)
{
    return 0;
}

static ssize_t mtdblock_write(FAR struct inode *inode_p, FAR const unsigned char *buffer,
                              unsigned long long start_sector, unsigned int nsectors)
{
    return 0;
}

static int mtdblock_geometry(FAR struct inode *inode_p, FAR struct geometry *geometry_p)
{
    return 0;
}

static int mtdblock_ioctl(FAR struct inode *inode_p, int cmd, unsigned long arg)
{
    return 0;
}

const struct block_operations g_dev_nand_ops = {
    .open       = mtdblock_open,
    .close      = mtdblock_close,
    .read       = mtdblock_read,
    .write      = mtdblock_write,
    .geometry   = mtdblock_geometry,
    .ioctl      = mtdblock_ioctl,
    .unlink     = NULL
};

const struct block_operations g_dev_spinor_ops = {
    .open       = mtdblock_open,
    .close      = mtdblock_close,
    .read       = mtdblock_read,
    .write      = mtdblock_write,
    .geometry   = mtdblock_geometry,
    .ioctl      = mtdblock_ioctl,
    .unlink     = NULL
};