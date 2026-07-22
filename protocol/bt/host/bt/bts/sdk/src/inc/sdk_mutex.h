/*
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved.
 * Description: sdk list header
 * Author: @CompanyNameTag
 * Create: 2022-03-03
 */

#ifndef SDK_MUTEX_H
#define SDK_MUTEX_H

#include "sdk_private.h"
#ifdef __LITEOS__
#include "los_mux.h"
#endif

#ifndef HANDLE
#define HANDLE void *
#endif

#ifndef SYSTEM_LOCK
#define SYSTEM_LOCK HANDLE
#endif

SYSTEM_LOCK bt_sdk_create_critical(const char *name);
void bt_sdk_enter_critical(SYSTEM_LOCK mylock);
void bt_sdk_leave_critical(SYSTEM_LOCK mylock);
#endif

