/*
 * Copyright (c) CompanyNameMagicTag 2021-2021. All rights reserved.
 * Description: header file for profile module
 * Author: audio
 */

#ifndef TIME_CONSUME_H
#define TIME_CONSUME_H

#ifdef SAP_TIME_CONSUME
#include "td_type.h"

td_void time_consume_start(td_void);
td_void time_consume_stop(td_char *type, td_s32 module_idx, td_u32 ioctl_cmd);
#else
#define time_consume_start()
#define time_consume_stop(type, module_idx, ioctl_cmd)
#endif

#endif
