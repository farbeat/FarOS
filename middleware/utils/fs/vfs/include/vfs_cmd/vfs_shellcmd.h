/* ----------------------------------------------------------------------------
 * Copyright (c) @CompanyNameMagicTag 2021-2021. All rights reserved.
 * Description: LiteOS VFS shell cmd
 * Create: 2021-04-12
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

#ifndef _VFS_SHELLCMD_H
#define _VFS_SHELLCMD_H

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

extern int OsShellCmdLsfd(int argc, const char **argv);
extern int OsShellCmdLs(int argc, const char **argv);
extern int OsShellCmdPwd(int argc, const char **argv);
extern int OsShellCmdCd(int argc, const char **argv);
extern int OsShellCmdCat(int argc, const char **argv);
extern int OsShellCmdRm(int argc, const char **argv);
extern int OsShellCmdRmdir(int argc, const char **argv);
extern int OsShellCmdMkdir(int argc, const char **argv);
extern int OsShellCmdSync(int argc, const char **argv);
extern int OsShellCmdStatfs(int argc, const char **argv);
extern int OsShellCmdMount(int argc, const char **argv);
extern int OsShellCmdUmount(int argc, const char **argv);
extern int OsShellCmdTouch(int argc, const char **argv);
extern int OsShellCmdCp(int argc, const char **argv);
extern int OsShellCmdDd(int argc, const char **argv);
extern int OsShellCmdPartInfo(int argc, const char **argv);
extern int osShellCmdVirstatfs(int argc, const char **argv);
extern int osShellCmdFormat(int argc, const char **argv);
extern int OsShellCmdPartitionShow(int argc, const char **argv);


#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* _VFS_SHELLCMD_H */
