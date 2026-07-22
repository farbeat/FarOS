/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: posix stub function
 * Author: Huawei LiteOS Team
 * Create: 2021-08-25
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

#include "unistd.h"
#include "errno.h"
#include "los_typedef.h"
#include "los_printf.h"
#include "termios.h"

#define FUNC_STUB() PRINT_ERR("%s is not supported\n", __FUNCTION__)
#define SET_ERRNO() (errno = ENOSYS)

/* process */
int fork(void)
{
    FUNC_STUB();
    SET_ERRNO();
    return -1;
}

int waitpid(int pid, int *statLoc, int options)
{
    (VOID)pid;
    (VOID)statLoc;
    (VOID)options;
    FUNC_STUB();
    SET_ERRNO();
    return -1;
}

int execve(const char *file, char * const *argv, char * const *envp)
{
    (VOID)file;
    (VOID)argv;
    (VOID)envp;
    FUNC_STUB();
    SET_ERRNO();
    return -1;
}

/* unistd */
unsigned int alarm(unsigned int seconds)
{
    (VOID)seconds;
    FUNC_STUB();
    SET_ERRNO();
    return 0;
}

int setuid(uid_t id)
{
    (VOID)id;
    FUNC_STUB();
    SET_ERRNO();
    return -1;
}

uid_t getuid(void)
{
    FUNC_STUB();
    SET_ERRNO();
    return 0;
}

uid_t geteuid(void)
{
    FUNC_STUB();
    SET_ERRNO();
    return 0;
}

pid_t setsid(void)
{
    FUNC_STUB();
    SET_ERRNO();
    return -1;
}

int setgid(gid_t id)
{
    (VOID)id;
    FUNC_STUB();
    SET_ERRNO();
    return -1;
}

int pipe(int pipedes[])
{
    (VOID)pipedes;
    FUNC_STUB();
    SET_ERRNO();
    return -1;
}

int nice(int inc)
{
    (VOID)inc;
    FUNC_STUB();
    SET_ERRNO();
    return -1;
}

int fchown(int fd, uid_t owner, gid_t group)
{
    (VOID)fd;
    (VOID)owner;
    (VOID)group;
    FUNC_STUB();
    SET_ERRNO();
    return -1;
}

ssize_t readlink(const char *path, char *buf, size_t len)
{
    (VOID)path;
    (VOID)buf;
    (VOID)len;
    FUNC_STUB();
    SET_ERRNO();
    return -1;
}

int gethostname(char *name, size_t len)
{
    (VOID)name;
    (VOID)len;
    FUNC_STUB();
    SET_ERRNO();
    return -1;
}

/*  crypt */
char *crypt(const char *key, const char *salt)
{
    (VOID)key;
    (VOID)salt;
    FUNC_STUB();
    SET_ERRNO();
    return NULL;
}

/* mman */
void *mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset)
{
    (VOID)addr;
    (VOID)length;
    (VOID)prot;
    (VOID)flags;
    (VOID)fd;
    return (void *)(UINTPTR)offset;
}

int munmap(void *addr, size_t length)
{
    (VOID)addr;
    (VOID)length;
    return 0;
}

/* passwd */
void *getpwnam(const char *name)
{
    (VOID)name;
    FUNC_STUB();
    SET_ERRNO();
    return NULL;
}

/* misc */
int getrlimit(int resource, void *rlimits)
{
    (VOID)resource;
    (VOID)rlimits;
    FUNC_STUB();
    SET_ERRNO();
    return -1;
}

int setrlimit(int resource, void *rlimits)
{
    (VOID)resource;
    (VOID)rlimits;
    FUNC_STUB();
    SET_ERRNO();
    return -1;
}

void openlog(const char *ident, int option, int facility)
{
    (VOID)ident;
    (VOID)option;
    (VOID)facility;
    FUNC_STUB();
    SET_ERRNO();
}

void closelog(void)
{
    FUNC_STUB();
    SET_ERRNO();
}

/* legacy */
int daemon(int noChdir, int noClose)
{
    (VOID)noChdir;
    (VOID)noClose;
    FUNC_STUB();
    SET_ERRNO();
    return -1;
}

int getdtablesize(void)
{
    FUNC_STUB();
    SET_ERRNO();
    return -1;
}

/* __dso_handle is used during execution of global destructors via _fini array,
 * or a system crash occurs when using c++.
 */
#ifndef LOSCFG_MAKE_COMPILER_LIBS
void *__dso_handle = NULL;
#endif

/* libstdc++.a init_array segment uses it */
void __cxa_atexit(void (*func)(void *), void *arg1, void *arg2)
{
    (VOID)func;
    (VOID)arg1;
    (VOID)arg2;
}

int __tls_get_addr(void *th, int mapAddress, size_t offset, void *address)
{
    (VOID)th;
    (VOID)mapAddress;
    (VOID)offset;
    (VOID)address;
    FUNC_STUB();
    SET_ERRNO();
    return -1;
}

int atexit(void (*func)(void))
{
    (VOID)func;
    FUNC_STUB();
    SET_ERRNO();
    return -1;
}

_Noreturn void exit(int code)
{
    (VOID)code;
    FUNC_STUB();
    SET_ERRNO();
    for (;;) {}
}

_Noreturn void _Exit(int ec)
{
    (VOID)ec;
    FUNC_STUB();
    SET_ERRNO();
    for (;;) {}
}

int system(const char *cmd)
{
    (VOID)cmd;
    FUNC_STUB();
    SET_ERRNO();
    return -1;
}
 
int tcgetattr(int fd, struct termios *tio)
{
    (VOID)fd;
    (VOID)tio;
    FUNC_STUB();
    SET_ERRNO();
    return -1;
}
 
int tcsetattr(int fd, int act, const struct termios *tio)
{
    (VOID)fd;
    (VOID)act;
    (VOID)tio;
    FUNC_STUB();
    SET_ERRNO();
    return -1;
}

void tzset(void)
{
    FUNC_STUB();
}
