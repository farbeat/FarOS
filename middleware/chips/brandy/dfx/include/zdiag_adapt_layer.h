/*
 * Copyright (c) @CompanyNameMagicTag 2021-2023. All rights reserved.
 * Description: diag usr adapt
 * This file should be changed only infrequently and with great care.
 */

#ifndef ZDIAG_ADAPT_LAYER_H
#define ZDIAG_ADAPT_LAYER_H

#include <stdbool.h>
#include "sys/time.h"
#include "time.h"
#include "errcode.h"
#include "zdiag_config.h"
#include "diag_common.h"
#include "diag_channel.h"
#include "dfx_adapt_layer.h"
#include "soc_diag_util.h"

#define DIAG_HSO_MAX_MUX_PKT_SIZE 512
#define DIAG_HSO_VER_DEFAULT 0
#define DIAG_HSO_THIS_VER 0x11

#define USER_CMD_LIST_NUM 10 /* Maximum number of non-system command lists */

static inline uint32_t diag_adapt_get_msg_time(void)
{
    struct timeval tv = { 0 };
    if (gettimeofday(&tv, NULL)) {
        return 0;
    }
    return tv.tv_sec;
}

static inline uint64_t diag_adapt_get_msg_time_ms(void)
{
    uint64_t time_ms = 0;
    struct timeval cur_time = { 0 };
    if (gettimeofday(&cur_time, NULL)) {
        return 0;
    }

    time_ms = ((uint64_t)cur_time.tv_sec) * 1000 ;  /* 1 second = 1000 milliseconds */
    time_ms += (uint64_t)(cur_time.tv_usec / 1000); /* 1 milliseconds = 1000 Microseconds */
    return time_ms;
}

static inline uint32_t diag_adapt_get_msg_local_time(void)
{
    struct timeval cur_time = { 0 };
    gettimeofday(&cur_time, NULL);

    struct tm lm = { 0 };
    if (localtime_r(&cur_time.tv_sec, &lm) == NULL) {
        return 0;
    }

    uint32_t local_time = (lm.tm_mon + 1) << 28; /* 28~31 bit: month */
    /* 1 day = 24 hours, 1 hour = 60 minutes, 1 minute = 60 seconds, 1 second = 100 * 10 milliseconds */
    local_time += (((lm.tm_mday * 24 + lm.tm_hour) * 60 + lm.tm_min) * 60 + lm.tm_sec) * 100;
    local_time += cur_time.tv_usec / 10000; /* 10 millisecond = 10000 microseconds */
    return local_time;
}

static inline errcode_t diag_adapt_sync_tx_sem_wait(void)
{
    return ERRCODE_FAIL;
}

static inline void diag_adapt_sync_tx_sem_signal(void)
{
    return;
}

static inline uint32_t diag_adapt_get_default_dst(void)
{
    return DIAG_FRAME_FID_PC;
}

static inline diag_channel_id_t diag_adapt_addr_2_channel_id(diag_frame_fid_t addr)
{
    if (addr == DIAG_FRAME_FID_PC) {
        return DIAG_CHANNEL_ID_0;
    } else if (addr == DIAG_FRAME_FID_PHONE) {
        return DIAG_CHANNEL_ID_1;
    } else if (addr == DIAG_FRAME_FID_BT) {
        return DIAG_CHANNEL_ID_2;
    }

    return DIAG_CHANNEL_ID_INVALID;
}

static inline diag_frame_fid_t diag_adapt_channel_id_2_addr(diag_channel_id_t id)
{
    if (id == DIAG_CHANNEL_ID_0) {
        return DIAG_FRAME_FID_PC;
    } else if (id == DIAG_CHANNEL_ID_1) {
        return DIAG_FRAME_FID_PHONE;
    } else if (id == DIAG_CHANNEL_ID_2) {
        return DIAG_FRAME_FID_BT;
    }
    return DIAG_FRAME_FID_MAX;
}

static inline diag_frame_fid_t diag_adapt_get_local_addr(void)
{
    return DIAG_FRAME_FID_LOCAL;
}

static inline bool diag_adapt_is_in_unblocking_context(void)
{
    return false;
}

#endif /* ZDIAG_ADAPT_LAYER_H */
