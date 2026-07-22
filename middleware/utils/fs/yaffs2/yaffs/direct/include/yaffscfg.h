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

/*
 * Header file for using yaffs in an application via
 * a direct interface.
 */


#ifndef __YAFFSCFG_H__
#define __YAFFSCFG_H__


#include "yportenv.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#ifndef MEMORY_MINI
#define YAFFSFS_N_HANDLES    100
#define YAFFSFS_N_DSC        20
#else
#define YAFFSFS_N_HANDLES    10
#define YAFFSFS_N_DSC        2
#endif

struct yaffsfs_DeviceConfiguration {
	const YCHAR *prefix;
	struct yaffs_dev *dev;
};


#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* __YAFFSCFG_H__ */
