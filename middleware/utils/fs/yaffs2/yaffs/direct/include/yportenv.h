/*
 * YAFFS: Yet another Flash File System . A NAND-flash specific file system.
 *
 * Copyright (C) 2002-2018 Aleph One Ltd.
 *
 * Created by Charles Manning <charles@aleph1.co.uk>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License version 2.1 as
 * published by the Free Software Foundation.
 *
 * Note: Only YAFFS headers are LGPL, YAFFS C code is covered by GPL.
 */


#ifndef __YPORTENV_H__
#define __YPORTENV_H__


#ifdef __rtems__

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>

#define CONFIG_YAFFS_DIRECT 1
#define CONFIG_YAFFS_SHORT_NAMES_IN_RAM 1
#define CONFIG_YAFFS_YAFFS2 1
#define CONFIG_YAFFS_PROVIDE_DEFS 1
#define CONFIG_YAFFSFS_PROVIDE_VALUES 1
#define CONFIG_YAFFS_DEFINES_TYPES 1
#define NO_Y_INLINE 1
#define loff_t off_t

#endif /* __rtems__ */

#include "errno.h"
#include "fcntl.h"
#include "unistd.h"
#include "sys/stat.h"
#include "dirent.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define CONFIG_YAFFS_DIRECT
#define CONFIG_YAFFS_PROVIDE_DEFS
#define __CC_ARM

/* Definition of types */
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;
typedef signed int s32;


#ifdef CONFIG_YAFFS_PROVIDE_DEFS
/*
 * Attribute flags.
 * These are or-ed together to select what has been changed.
 */
#define ATTR_MODE	1
#define ATTR_UID	2
#define ATTR_GID	4
#define ATTR_SIZE	8
#define ATTR_ATIME	16
#define ATTR_MTIME	32
#define ATTR_CTIME	64

struct iattr {
	unsigned int ia_valid;
	unsigned ia_mode;
	unsigned ia_uid;
	unsigned ia_gid;
	unsigned ia_size;
	unsigned ia_atime;
	unsigned ia_mtime;
	unsigned ia_ctime;
	unsigned int ia_attr_flags;
};

#endif



#if defined CONFIG_YAFFS_WINCE

#include "ywinceenv.h"


#elif defined CONFIG_YAFFS_DIRECT

/* Direct interface */
#include "ydirectenv.h"

#elif defined CONFIG_YAFFS_UTIL

#include "yutilsenv.h"

#else
/* Should have specified a configuration type */
#error Unknown configuration

#endif

#ifndef XATTR_CREATE
#define XATTR_CREATE 1
#endif

#ifndef XATTR_REPLACE
#define XATTR_REPLACE 2
#endif

#ifndef S_ISSOCK
#define S_ISSOCK(m)	(((m) & S_IFMT) == S_IFSOCK)
#endif

#ifndef Y_DUMP_STACK
#define Y_DUMP_STACK() do { } while (0)
#endif

#ifndef YAFFS_BUG
#define YAFFS_BUG()
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* __YPORTENV_H__ */
