/*
 * Copyright (c) CompanyNameMagicTag 2020-2021. All rights reserved.
 * Description: audio work buffer
 * Author: audio
 */

#ifndef __WK_BUF_H__
#define __WK_BUF_H__

#include "audio_type.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define WORK_BUF_NUM  2
#define WORK_BUF_SIZE 0x2000 /* 8k work buffer */

#define SCRATCH_BUF_NUM  1
#define SCRATCH_BUF_SIZE 0x9000  /* 36k scratch buffer */

td_void wk_buf_flush(td_void);

td_void *wk_buf_request(td_void);

td_u32 wk_buf_get_size(td_void);

td_void *wk_buf_request_all(td_void);

td_u32 wk_buf_get_all_size(td_void);

td_void *scratch_buf_request(td_void);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* __WK_BUF_H__ */
