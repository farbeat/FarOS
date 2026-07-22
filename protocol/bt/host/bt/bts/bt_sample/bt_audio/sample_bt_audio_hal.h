/*
 * Copyright (c) @CompanyNameMagicTag 2021. All rights reserved.
 * Description: This module include test function of BT AUDIO HAL module.
 */
#ifndef SAMPLE_BT_AUDIO_HAL_H
#define SAMPLE_BT_AUDIO_HAL_H
#include <stdint.h>

#define MAX_SEND_TIME_CNT (100) /* 100： 维测记录最大个数 */
enum {
    A2DP_PLAY_START,
    A2DP_PLAY_END
};

void bt_audio_hal_sample_cmd(uint32_t cmd_id, const char **params, int32_t params_cnt);

#endif /* SAMPLE_BT_AUDIO_HAL_H */
