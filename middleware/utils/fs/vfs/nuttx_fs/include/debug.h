/****************************************************************************
 * include/debug.h
 *
 *   Copyright (C) 2007-2011, 2014, 2016 Gregory Nutt. All rights reserved.
 *   Copyright (c) Huawei Technologies Co., Ltd. 2014-2019. All rights reserved.
 *
 *   Author: Gregory Nutt <gnutt@nuttx.org>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name NuttX nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

#ifndef __INCLUDE_DEBUG_H
#define __INCLUDE_DEBUG_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include "vfs_config.h"
#include "compiler.h"
#include "syslog.h"


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Debug macros to runtime filter the debug messages sent to the console.  In
 * general, there are four forms of the debug macros:
 *
 * [a-z]info() -- Outputs messages to the console similar to printf() except
 *    that the output is not buffered.  Output is only generated if
 *    CONFIG_DEBUG_INFO is defined.  The info macros are intended for
 *    verbose "informational" debug output.  If you enable CONFIG_DEBUG_INFO,
 *    then very chatty (and often annoying) output will be generated.
 *
 *    The first character of the macro name indicates the system system
 *    (e.g., n=network, f=filesystm, etc.).  If the first character is
 *    missing (i.e., _info()), then it is common.  The common _info() macro
 *    is enabled simply with CONFIG_DEBUG_INFO.  Subsystem debug requires an
 *    additional configuration setting to enable it (e.g., CONFIG_DEBUG_NET
 *    for the network, CONFIG_DEBUG_FS for the file system, etc).
 *
 *    In general, error messages and output of importance use [a-z]err().
 *    [a-z]err() is implementation dependent but usually uses file descriptors.
 *    (that is a problem only because the interrupt task may have re-
 *    directed stdout).  Therefore [a-z]err() should not be used in interrupt
 *    handlers.
 *
 * [a-z]warn() -- Identical to [a-z]info() except that it also requires that
 *    CONFIG_DEBUG_WARN be defined.  This is intended for important exception
 *    conditions that are potential errors (or perhaps real errors with non-
 *    fatal consequences).
 *
 * [a-z]err() -- Identical to [a-z]info() except that it also requires that
 *    CONFIG_DEBUG_ERROR be defined.  This is intended for important error-related
 *    information that you probably not want to suppress during normal debug
 *    general debugging.
 *
 * _alert() - is a special, high-priority, unconditional version that is really
 *    intended only for crash error reporting.
 */

#ifdef CONFIG_HAVE_FUNCTIONNAME
#  define EXTRA_FMT "%s: "
#  define EXTRA_ARG ,__FUNCTION__
#else
#  define EXTRA_FMT
#  define EXTRA_ARG
#endif

/* Debug macros will differ depending upon if the toolchain supports
 * macros with a variable number of arguments or not.
 */

#ifdef CONFIG_CPP_HAVE_VARARGS

/* C-99 style variadic macros are supported */

#ifdef CONFIG_DEBUG
# define dbg(format, ...) \
  syslog(LOG_ERR, EXTRA_FMT format EXTRA_ARG, ##__VA_ARGS__)

# ifdef CONFIG_ARCH_LOWPUTC
#  define lldbg(format, ...) \
   lowsyslog(LOG_ERR, EXTRA_FMT format EXTRA_ARG, ##__VA_ARGS__)
# else
#  define lldbg(x...)
# endif

# ifdef CONFIG_DEBUG_VERBOSE
#  define vdbg(format, ...) \
   syslog(LOG_DEBUG, EXTRA_FMT format EXTRA_ARG, ##__VA_ARGS__)

#  ifdef CONFIG_ARCH_LOWPUTC
#    define llvdbg(format, ...) \
     lowsyslog(LOG_DEBUG, EXTRA_FMT format EXTRA_ARG, ##__VA_ARGS__)
#  else
#    define llvdbg(x...)
#  endif

# else
#  define vdbg(x...)
#  define llvdbg(x...)
# endif

#else /* CONFIG_DEBUG */

# define dbg(x...)
# define lldbg(x...)
# define vdbg(x...)
# define llvdbg(x...)

#endif /* CONFIG_DEBUG */

/* Subsystem specific debug */

#ifdef CONFIG_DEBUG_FS
# define fdbg(format, ...)    dbg(format, ##__VA_ARGS__)
# define flldbg(format, ...)  lldbg(format, ##__VA_ARGS__)
# define fvdbg(format, ...)   vdbg(format, ##__VA_ARGS__)
# define fllvdbg(format, ...) llvdbg(format, ##__VA_ARGS__)
#else
# define fdbg(x...)
# define flldbg(x...)
# define fvdbg(x...)
# define fllvdbg(x...)
#endif

#endif /* CONFIG_CPP_HAVE_VARARGS */


#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
#endif /* __INCLUDE_DEBUG_H */
