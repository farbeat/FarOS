/*
 * Copyright (c) @CompanyNameMagicTag 2021-2022. All rights reserved.
 */

#ifndef GDK_INT_H
#define GDK_INT_H
#pragma once

#include "gdk_os.h"
#include "gdk_list.h"

typedef struct {
    uint32_t event_sigs;
    gdk_mutex_t  event_sigs_mutex;
    gdk_mutex_t  event_mutex;
    gdk_timer_t  event_timer;
    gdk_list_t   event_list;                    /* event list to handle */
} gdk_os_t;

typedef struct {
    gdk_os_t os;
} gdk_cb_t;

extern gdk_cb_t gdk_cb;
#endif

