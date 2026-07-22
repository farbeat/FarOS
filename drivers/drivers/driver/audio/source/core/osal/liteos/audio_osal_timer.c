/*
 * Copyright (c) CompanyNameMagicTag 2021-2021. All rights reserved.
 * Description: API of audio osal timer
 * Author: chenchao
 */
#include <sys/time.h>

#include "td_type.h"
#include "audio_osal.h"

#define CONVERT_LEVEL 1000000

td_u32 audio_osal_gettime_us(td_void)
{
    struct timeval times;
    td_u32 time_us;

    gettimeofday(&times, NULL);

    time_us = (td_u32)((td_u64)times.tv_sec * CONVERT_LEVEL + (td_u64)times.tv_usec);

    return time_us;
}

td_u32 audio_osal_cali_time_diff_us(td_u32 begin_us, td_u32 end_us)
{
    /* begin_us和end_us不能跨越1个回绕周期（4295秒），否则计算不准确，仅适合短时间耗时的计算 */
    if (end_us < begin_us) {
        /* 时间戳回绕处理，单位为us */
        return (0xFFFFFFFF - (begin_us - end_us));
    } else {
        return (end_us - begin_us);
    }
}
