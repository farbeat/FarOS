/* ----------------------------------------------------------------------------
 * Copyright (c) CompanyNameMagicTag 2021-2021. All rights reserved.
 * Description: Errno
 * Create: 2021-06-08
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

/**
 * @defgroup dpal_errno Error code
 * @ingroup kernel
 */

#ifndef DPAL_ERRNO_H
#define DPAL_ERRNO_H

#include "dpal_typedef.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/**
 * @ingroup los_errno
 * OS error code flag. It is a 24-bit unsigned integer. Its value is 0x000000U.
 */
#define DPAL_ERRNO_OS_ID (0x00U << 16)

/**
 * @ingroup los_errno
 * Define the error level as informative. It is a 32-bit unsigned integer. Its value is 0x00000000U.
 */
#define DPAL_ERRTYPE_NORMAL (0x00U << 24)

/**
 * @ingroup los_errno
 * Define the error level as warning. It is a 32-bit unsigned integer. Its value is 0x01000000U.
 */
#define DPAL_ERRTYPE_WARN (0x01U << 24)

/**
 * @ingroup los_errno
 * Define the error level as critical. It is a 32-bit unsigned integer. Its value is 0x02000000U.
 */
#define DPAL_ERRTYPE_ERROR (0x02U << 24)

/**
 * @ingroup los_errno
 * Define the error level as fatal. It is a 32-bit unsigned integer. Its value is 0x03000000U.
 */
#define DPAL_ERRTYPE_FATAL (0x03U << 24)

/**
 * @ingroup los_errno
 * Define fatal OS errors. It is a 32-bit unsigned integer error code.
 * <ul>
 * <li>24-31 bits indicate the error level, here is #DPAL_ERRTYPE_FATAL.</li>
 * <li>16-23 bits indicate the os error code flag, here is #DPAL_ERRNO_OS_ID.</li>
 * <li>8-15 bits indicate the module which the error code belongs to. It is specified by MID.</li>
 * <li>0-7 bits indicate the error code number. It is specified by ERRNO.</li>
 * </ul>
 */
#define DPAL_ERRNO_OS_FATAL(MID, ERRNO) \
    (DPAL_ERRTYPE_FATAL | DPAL_ERRNO_OS_ID | ((uint32_t)(MID) << 8) | ((uint32_t)(ERRNO)))

/**
 * @ingroup los_errno
 * Define critical OS errors. It is a 32-bit unsigned integer error code.
 * <ul>
 * <li>24-31 bits indicate the error level, here is #DPAL_ERRTYPE_ERROR.</li>
 * <li>16-23 bits indicate the os error code flag, here is #DPAL_ERRNO_OS_ID.</li>
 * <li>8-15 bits indicate the module which the error code belongs to. It is specified by MID.</li>
 * <li>0-7 bits indicate the error code number. It is specified by ERRNO.</li>
 * </ul>
 */
#define DPAL_ERRNO_OS_ERROR(MID, ERRNO) \
    (DPAL_ERRTYPE_ERROR | DPAL_ERRNO_OS_ID | ((uint32_t)(MID) << 8) | ((uint32_t)(ERRNO)))

/**
 * @ingroup los_errno
 * Define warning OS errors. It is a 32-bit unsigned integer error code.
 * <ul>
 * <li>24-31 bits indicate the error level, here is #DPAL_ERRTYPE_WARN.</li>
 * <li>16-23 bits indicate the os error code flag, here is #DPAL_ERRNO_OS_ID.</li>
 * <li>8-15 bits indicate the module which the error code belongs to. It is specified by MID.</li>
 * <li>0-7 bits indicate the error code number. It is specified by ERRNO.</li>
 * </ul>
 */
#define DPAL_ERRNO_OS_WARN(MID, ERRNO) \
    (DPAL_ERRTYPE_WARN | DPAL_ERRNO_OS_ID | ((uint32_t)(MID) << 8) | ((uint32_t)(ERRNO)))

/**
 * @ingroup los_errno
 * Define informative OS errors. It is a 32-bit unsigned integer error code.
 * <ul>
 * <li>24-31 bits indicate the error level, here is #DPAL_ERRTYPE_NORMAL.</li>
 * <li>16-23 bits indicate the os error code flag, here is #DPAL_ERRNO_OS_ID.</li>
 * <li>8-15 bits indicate the module which the error code belongs to. It is specified by MID.</li>
 * <li>0-7 bits indicate the error code number. It is specified by ERRNO.</li>
 * </ul>
 */
#define DPAL_ERRNO_OS_NORMAL(MID, ERRNO) \
    (DPAL_ERRTYPE_NORMAL | DPAL_ERRNO_OS_ID | ((uint32_t)(MID) << 8) | ((uint32_t)(ERRNO)))

/**
 * @ingroup los_errno
 * Define the ID of each module in kernel. The ID is used in error code.
 */
enum DPAL_MOUDLE_ID {
    DPAL_MOD_SYS = 0x0,          /**< System ID. Its value is 0x0. */
    DPAL_MOD_MEM = 0x1,          /**< Dynamic memory module ID. Its value is 0x1. */
    DPAL_MOD_TSK = 0x2,          /**< Task module ID. Its value is 0x2. */
    DPAL_MOD_SWTMR = 0x3,        /**< Software timer module ID. Its value is 0x3. */
    DPAL_MOD_TICK = 0x4,         /**< Tick module ID. Its value is 0x4. */
    DPAL_MOD_MSG = 0x5,          /**< Message module ID. Its value is 0x5. */
    DPAL_MOD_QUE = 0x6,          /**< Queue module ID. Its value is 0x6. */
    DPAL_MOD_SEM = 0x7,          /**< Semaphore module ID. Its value is 0x7. */
    DPAL_MOD_MBOX = 0x8,         /**< Static memory module ID. Its value is 0x8. */
    DPAL_MOD_HWI = 0x9,          /**< Hardware interrupt module ID. Its value is 0x9. */
    DPAL_MOD_HWWDG = 0xa,        /**< Hardware watchdog module ID. Its value is 0xa. */
    DPAL_MOD_CACHE = 0xb,        /**< Cache module ID. Its value is 0xb. */
    DPAL_MOD_HWTMR = 0xc,        /**< Hardware timer module ID. Its value is 0xc. */
    DPAL_MOD_MMU = 0xd,          /**< MMU module ID. Its value is 0xd. */
    DPAL_MOD_LOG = 0xe,          /**< Log module ID. Its value is 0xe. */
    DPAL_MOD_ERR = 0xf,          /**< Error handling module ID. Its value is 0xf. */
    DPAL_MOD_EXC = 0x10,         /**< Exception handling module ID. Its value is 0x10. */
    DPAL_MOD_CSTK = 0x11,        /**< This module ID is reserved. Its value is 0x11. */
    DPAL_MOD_MPU = 0x12,         /**< MPU module ID. Its value is 0x12. */
    DPAL_MOD_NMHWI = 0x13,       /**< NMI module ID. It is reserved. Its value is 0x13. */
    DPAL_MOD_TRACE = 0x14,       /**< Trace module ID. Its value is 0x14. */
    DPAL_MOD_KNLSTAT = 0x15,     /**< This module ID is reserved. Its value is 0x15. */
    DPAL_MOD_EVTTIME = 0x16,     /**< This module ID is reserved. Its value is 0x16. */
    DPAL_MOD_THRDCPUP = 0x17,    /**< This module ID is reserved. Its value is 0x17. */
    DPAL_MOD_IPC = 0x18,         /**< This module ID is reserved. Its value is 0x18. */
    DPAL_MOD_STKMON = 0x19,      /**< This module ID is reserved. Its value is 0x19. */
    DPAL_MOD_TIMER = 0x1a,       /**< This module ID is reserved. Its value is 0x1a. */
    DPAL_MOD_RESLEAKMON = 0x1b,  /**< This module ID is reserved. Its value is 0x1b. */
    DPAL_MOD_EVENT = 0x1c,       /**< event module ID. Its value is 0x1c. */
    DPAL_MOD_MUX = 0x1d,         /**< mutex module ID. Its value is 0x1d. */
    DPAL_MOD_CPUP = 0x1e,        /**< CPU usage module ID. Its value is 0x1e. */
    DPAL_MOD_FPB = 0x1f,         /**< FPB module ID. Its value is 0x1f. */
    DPAL_MOD_PERF = 0x20,        /**< Perf module ID. Its value is 0x20. */
    DPAL_MOD_SHELL = 0x31,       /**< shell module ID. Its value is 0x31. */
    DPAL_MOD_DRIVER = 0x41,      /**< driver module ID. Its value is 0x41. */
    DPAL_MOD_BUTT                /**< It is end flag of this enumeration. */
};

#define ENOERR           0
#define EPERM            1
#define ENOENT           2
#define ESRCH            3
#define EINTR            4
#define EIO              5
#define ENXIO            6
#define E2BIG            7
#define ENOEXEC          8
#define EBADF            9
#define ECHILD          10
#define EAGAIN          11
#define ENOMEM          12
#define EACCES          13
#define EFAULT          14
#define ENOTBLK         15
#define EBUSY           16
#define EEXIST          17
#define EXDEV           18
#define ENODEV          19
#define ENOTDIR         20
#define EISDIR          21
#define EINVAL          22
#define ENFILE          23
#define EMFILE          24
#define ENOTTY          25
#define ETXTBSY         26
#define EFBIG           27
#define ENOSPC          28
#define ESPIPE          29
#define EROFS           30
#define EMLINK          31
#define EPIPE           32
#define EDOM            33
#define ERANGE          34
#define EDEADLK         35
#define ENAMETOOLONG    36
#define ENOLCK          37
#define ENOSYS          38
#define ENOTEMPTY       39
#define ELOOP           40
#define EWOULDBLOCK     EAGAIN
#define ENOMSG          42
#define EIDRM           43
#define ECHRNG          44
#define EL2NSYNC        45
#define EL3HLT          46
#define EL3RST          47
#define ELNRNG          48
#define EUNATCH         49
#define ENOCSI          50
#define EL2HLT          51
#define EBADE           52
#define EBADR           53
#define EXFULL          54
#define ENOANO          55
#define EBADRQC         56
#define EBADSLT         57
#define EDEADLOCK       EDEADLK
#define EBFONT          59
#define ENOSTR          60
#define ENODATA         61
#define ETIME           62
#define ENOSR           63
#define ENONET          64
#define ENOPKG          65
#define EREMOTE         66
#define ENOLINK         67
#define EADV            68
#define ESRMNT          69
#define ECOMM           70
#define EPROTO          71
#define EMULTIHOP       72
#define EDOTDOT         73
#define EBADMSG         74
#define EOVERFLOW       75
#define ENOTUNIQ        76
#define EBADFD          77
#define EREMCHG         78
#define ELIBACC         79
#define ELIBBAD         80
#define ELIBSCN         81
#define ELIBMAX         82
#define ELIBEXEC        83
#define EILSEQ          84
#define ERESTART        85
#define ESTRPIPE        86
#define EUSERS          87
#define ENOTSOCK        88
#define EDESTADDRREQ    89
#define EMSGSIZE        90
#define EPROTOTYPE      91
#define ENOPROTOOPT     92
#define EPROTONOSUPPORT 93
#define ESOCKTNOSUPPORT 94
#define EOPNOTSUPP      95
#define ENOTSUP         EOPNOTSUPP
#define EPFNOSUPPORT    96
#define EAFNOSUPPORT    97
#define EADDRINUSE      98
#define EADDRNOTAVAIL   99
#define ENETDOWN        100
#define ENETUNREACH     101
#define ENETRESET       102
#define ECONNABORTED    103
#define ECONNRESET      104
#define ENOBUFS         105
#define EISCONN         106
#define ENOTCONN        107
#define ESHUTDOWN       108
#define ETOOMANYREFS    109
#define ETIMEDOUT       110
#define ECONNREFUSED    111
#define EHOSTDOWN       112
#define EHOSTUNREACH    113
#define EALREADY        114
#define EINPROGRESS     115
#define ESTALE          116
#define EUCLEAN         117
#define ENOTNAM         118
#define ENAVAIL         119
#define EISNAM          120
#define EREMOTEIO       121
#define EDQUOT          122
#define ENOMEDIUM       123
#define EMEDIUMTYPE     124
#define ECANCELED       125
#define ENOKEY          126
#define EKEYEXPIRED     127
#define EKEYREVOKED     128
#define EKEYREJECTED    129
#define EOWNERDEAD      130
#define ENOTRECOVERABLE 131
#define ERFKILL         132
#define EHWPOISON       133

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* _DPAL_ERRNO_H */
