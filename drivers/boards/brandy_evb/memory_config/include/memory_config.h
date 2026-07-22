/*
 * Copyright (c) @CompanyNameMagicTag 2018-2020. All rights reserved.
 * Description:  Default memory configurations
 * Author: @CompanyNameTag
 * Create:  2020-01
 */
#ifndef MEMORY_CONFIG_H
#define MEMORY_CONFIG_H

#include "memory_config_common.h"

#ifdef BUILD_APPLICATION_SSB
#include "ssb_config_common.h"
#else
/* APP RAM defines
 * APP has base 512K ITCM (Instruction TCM) for code
 *              256K DTCM (Data TCM) for DATA.
 * APP ITCM                 VECTORS TABLE
 *                          RAM TEXT
 *
 * APP DTCM                 STACK
 *                          RAM
 */
/* L2RAM 640K for APP core */
#define L2RAM_OFFSET         0x60240000
#define L2RAM_FOR_APP_START  L2RAM_OFFSET
#define L2RAM_FOR_APP_LENGTH 0x160000    /* 640K */
#endif

/*
ULP_RAM地址规划
ULP_RAM有8K大小，范围[0x57038000, 0x57039FFF]
*/
/* 不下电日历占用ULP_RAM 24字节，8字节存储时间8字节存储counter8字节存储alarm */
#define ULP_RAM_OFFSET          0x57038000
#define ULP_RAM_CALENDAR        ULP_RAM_OFFSET
#define ULP_RAM_CALENDAR_LENGTH 0x18
#endif
