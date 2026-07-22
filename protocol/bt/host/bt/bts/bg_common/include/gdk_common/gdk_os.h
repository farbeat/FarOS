/*
 * Copyright (c) @CompanyNameMagicTag 2021-2022. All rights reserved.
 */

#ifndef GDK_OS_H
#define GDK_OS_H
#pragma once

#include "custdefs.h"
#include "stdint.h"

#define GDK_WAIT_FOREVER 0xFFFFFFFF

typedef struct {
    void* p_platform_mutex;
} gdk_mutex_t;

typedef struct {
    void* p_platform_timer;
} gdk_timer_t;

typedef void(*GDK_TIMER_CBACK_FN)(const void* p_args);
#endif
