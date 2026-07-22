/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2019. All rights reserved.
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
 * ---------------------------------------------------------------------------- */

#include "sys/types.h"
#include "sys/uio.h"
#include "unistd.h"
#include "string.h"
#include "stdlib.h"
#include "limits.h"
#include "fs/fs.h"
#include "dpal.h"

ssize_t writev(int fd, const struct iovec *iov, int iovcnt)
{
  int     i;
  char    *buf      = NULL;
  char    *curbuf   = NULL;
  char    *writebuf = NULL;
  size_t  buflen    = 0;
  size_t  bytestowrite;
  ssize_t totalbyteswritten;
  size_t  totallen;

  if (iov == NULL)
    {
      return VFS_ERROR;
    }

  for (i = 0; i < iovcnt; ++i)
    {
      if (SSIZE_MAX - buflen < iov[i].iov_len)
        {
          set_errno(EINVAL);
          return VFS_ERROR;
        }
      buflen += iov[i].iov_len;
    }

  totallen = buflen * sizeof(char);
  buf = (char *)dpal_malloc(totallen);
  if (buf == NULL)
    {
      return VFS_ERROR;
    }
  curbuf = buf;
  for (i = 0; i < iovcnt; ++i)
    {
      writebuf = (char *)iov[i].iov_base;
      bytestowrite = iov[i].iov_len;
      (void)memcpy_s(curbuf, totallen, writebuf, bytestowrite);
      curbuf   += bytestowrite;
      totallen -= bytestowrite;
    }

  totalbyteswritten = write(fd, buf, buflen);
  dpal_free(buf);

  return totalbyteswritten;
}
