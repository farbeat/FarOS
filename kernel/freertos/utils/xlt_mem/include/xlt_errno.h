/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2012-2021. All rights reserved.
 * Description:  errno
 */

#ifndef _XLT_ERRNO_H
#define _XLT_ERRNO_H

#include "xlt_typedef.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/**
 * @ingroup xlt_errno
 * OS error code flag. It is a 24-bit unsigned integer. Its value is 0x000000U.
 */
#define XLT_ERRNO_OS_ID (0x00U << 16)

/**
 * @ingroup xlt_errno
 * Define the error level as informative. It is a 32-bit unsigned integer. Its value is 0x00000000U.
 */
#define XLT_ERRTYPE_NORMAL (0x00U << 24)

/**
 * @ingroup xlt_errno
 * Define the error level as warning. It is a 32-bit unsigned integer. Its value is 0x01000000U.
 */
#define XLT_ERRTYPE_WARN (0x01U << 24)

/**
 * @ingroup xlt_errno
 * Define the error level as critical. It is a 32-bit unsigned integer. Its value is 0x02000000U.
 */
#define XLT_ERRTYPE_ERROR (0x02U << 24)

/**
 * @ingroup xlt_errno
 * Define the error level as fatal. It is a 32-bit unsigned integer. Its value is 0x03000000U.
 */
#define XLT_ERRTYPE_FATAL (0x03U << 24)

/**
 * @ingroup xlt_errno
 * Define fatal OS errors. It is a 32-bit unsigned integer error code.
 * <ul>
 * <li>24-31 bits indicate the error level, here is #XLT_ERRTYPE_FATAL.</li>
 * <li>16-23 bits indicate the os error code flag, here is #XLT_ERRNO_OS_ID.</li>
 * <li>8-15 bits indicate the module which the error code belongs to. It is specified by MID.</li>
 * <li>0-7 bits indicate the error code number. It is specified by ERRNO.</li>
 * </ul>
 */
#define xlt_errno_os_fatal(mid, error) \
    (XLT_ERRTYPE_FATAL | XLT_ERRNO_OS_ID | ((unsigned int)(mid) << 8) | ((unsigned int)(error)))

/**
 * @ingroup xlt_errno
 * Define critical OS errors. It is a 32-bit unsigned integer error code.
 * <ul>
 * <li>24-31 bits indicate the error level, here is #XLT_ERRTYPE_ERROR.</li>
 * <li>16-23 bits indicate the os error code flag, here is #XLT_ERRNO_OS_ID.</li>
 * <li>8-15 bits indicate the module which the error code belongs to. It is specified by MID.</li>
 * <li>0-7 bits indicate the error code number. It is specified by ERRNO.</li>
 * </ul>
 */
#define xlt_errno_os_error(mid, error) \
    (XLT_ERRTYPE_ERROR | XLT_ERRNO_OS_ID | ((unsigned int)(mid) << 8) | ((unsigned int)(error)))

/**
 * @ingroup xlt_errno
 * Define warning OS errors. It is a 32-bit unsigned integer error code.
 * <ul>
 * <li>24-31 bits indicate the error level, here is #XLT_ERRTYPE_WARN.</li>
 * <li>16-23 bits indicate the os error code flag, here is #XLT_ERRNO_OS_ID.</li>
 * <li>8-15 bits indicate the module which the error code belongs to. It is specified by MID.</li>
 * <li>0-7 bits indicate the error code number. It is specified by ERRNO.</li>
 * </ul>
 */
#define xlt_errno_os_warn(mid, error) \
    (XLT_ERRTYPE_WARN | XLT_ERRNO_OS_ID | ((unsigned int)(mid) << 8) | ((unsigned int)(error)))

/**
 * @ingroup xlt_errno
 * Define informative OS errors. It is a 32-bit unsigned integer error code.
 * <ul>
 * <li>24-31 bits indicate the error level, here is #XLT_ERRTYPE_NORMAL.</li>
 * <li>16-23 bits indicate the os error code flag, here is #XLT_ERRNO_OS_ID.</li>
 * <li>8-15 bits indicate the module which the error code belongs to. It is specified by MID.</li>
 * <li>0-7 bits indicate the error code number. It is specified by ERRNO.</li>
 * </ul>
 */
#define xlt_errno_os_normal(mid, error) \
    (XLT_ERRTYPE_NORMAL | XLT_ERRNO_OS_ID | ((unsigned int)(mid) << 8) | ((unsigned int)(error)))

/**
 * @ingroup xlt_errno
 * Define the ID of each module in kernel. The ID is used in error code.
 */
enum XLT_MOUDLE_ID {
    XLT_MOD_SYS = 0x0,         /* *< System ID. Its value is 0x0. */
    XLT_MOD_MEM = 0x1,         /* *< Dynamic memory module ID. Its value is 0x1. */
    XLT_MOD_TSK = 0x2,         /* *< Task module ID. Its value is 0x2. */
    XLT_MOD_SWTMR = 0x3,       /* *< Software timer module ID. Its value is 0x3. */
    XLT_MOD_TICK = 0x4,        /* *< Tick module ID. Its value is 0x4. */
    XLT_MOD_MSG = 0x5,         /* *< Message module ID. Its value is 0x5. */
    XLT_MOD_QUE = 0x6,         /* *< Queue module ID. Its value is 0x6. */
    XLT_MOD_SEM = 0x7,         /* *< Semaphore module ID. Its value is 0x7. */
    XLT_MOD_MBOX = 0x8,        /* *< Static memory module ID. Its value is 0x8. */
    XLT_MOD_HWI = 0x9,         /* *< Hardware interrupt module ID. Its value is 0x9. */
    XLT_MOD_HWWDG = 0xa,       /* *< Hardware watchdog module ID. Its value is 0xa. */
    XLT_MOD_CACHE = 0xb,       /* *< Cache module ID. Its value is 0xb. */
    XLT_MOD_HWTMR = 0xc,       /* *< Hardware timer module ID. Its value is 0xc. */
    XLT_MOD_MMU = 0xd,         /* *< MMU module ID. Its value is 0xd. */
    XLT_MOD_LOG = 0xe,         /* *< Log module ID. Its value is 0xe. */
    XLT_MOD_ERR = 0xf,         /* *< Error handling module ID. Its value is 0xf. */
    XLT_MOD_EXC = 0x10,        /* *< Exception handling module ID. Its value is 0x10. */
    XLT_MOD_CSTK = 0x11,       /* *< This module ID is reserved. Its value is 0x11. */
    XLT_MOD_MPU = 0x12,        /* *< MPU module ID. Its value is 0x12. */
    XLT_MOD_NMHWI = 0x13,      /* *< NMI module ID. It is reserved. Its value is 0x13. */
    XLT_MOD_TRACE = 0x14,      /* *< Trace module ID. Its value is 0x14. */
    XLT_MOD_KNLSTAT = 0x15,    /* *< This module ID is reserved. Its value is 0x15. */
    XLT_MOD_EVTTIME = 0x16,    /* *< This module ID is reserved. Its value is 0x16. */
    XLT_MOD_THRDCPUP = 0x17,   /* *< This module ID is reserved. Its value is 0x17. */
    XLT_MOD_IPC = 0x18,        /* *< This module ID is reserved. Its value is 0x18. */
    XLT_MOD_STKMON = 0x19,     /* *< This module ID is reserved. Its value is 0x19. */
    XLT_MOD_TIMER = 0x1a,      /* *< This module ID is reserved. Its value is 0x1a. */
    XLT_MOD_RESLEAKMON = 0x1b, /* *< This module ID is reserved. Its value is 0x1b. */
    XLT_MOD_EVENT = 0x1c,      /* *< event module ID. Its value is 0x1c. */
    XLT_MOD_MUX = 0x1d,        /* *< mutex module ID. Its value is 0x1d. */
    XLT_MOD_CPUP = 0x1e,       /* *< CPU usage module ID. Its value is 0x1e. */
    XLT_MOD_FPB = 0x1f,        /* *< FPB module ID. Its value is 0x1f. */
    XLT_MOD_PERF = 0x20,       /* *< Perf module ID. Its value is 0x20. */
    XLT_MOD_SHELL = 0x31,      /* *< shell module ID. Its value is 0x31. */
    XLT_MOD_DRIVER = 0x41,     /* *< driver module ID. Its value is 0x41. */
    XLT_MOD_BUTT               /* *< It is end flag of this enumeration. */
};

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* _XLT_ERRNO_H */
