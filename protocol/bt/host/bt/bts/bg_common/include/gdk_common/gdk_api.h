/*
 * Copyright (c) @CompanyNameMagicTag 2021-2022. All rights reserved.
 */

#ifndef GDK_API_H
#define GDK_API_H
#pragma once
#include "gdk_os.h"

boolean gdk_timer_create(gdk_timer_t *p_timer, GDK_TIMER_CBACK_FN pfn_cback);
boolean gdk_timer_start(gdk_timer_t *p_timer, uint64_t millisec);
boolean gdk_timer_stop(gdk_timer_t *p_timer);
uint64_t gdk_get_time_ms(void);
/**
 * @brief frees memory
 * @param size The size of the memory that has to be
 *                  allocated
 * @return the address of the memory allocated, or NULL if failed
 */
void* gdk_os_malloc(uint32_t size);
/**
 * @brief frees memory
 * @param size The address of the memory that needs to be freed
 * @return void
 */
void gdk_os_free(void *p_mem);
/**
 * @brief create mutex.
 * @param the point of mutex allocated.
 */
boolean gdk_mutex_create(gdk_mutex_t *p_mutex);

/**
 * @brief lock mutex.
 * @param the point of mutex allocated.
 */
boolean gdk_mutex_lock(gdk_mutex_t *p_mutex);
/**
 * @brief unlock mutex.
 * @param the point of mutex allocated.
 */
boolean gdk_mutex_unlock(gdk_mutex_t *p_mutex);

/**
 * @brief release mutex.
 * @param the point of mutex allocated.
 */
void gdk_mutex_delete(gdk_mutex_t *p_mutex);
void gdk_msg_handler(void const * pv_params);
#endif
