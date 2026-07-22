/*
 * Copyright (c) CompanyNameMagicTag 2020-2020. All rights reserved.
 * Description: header file for audio type define
 * Author: audio
 */

#ifndef __AUDIO_TYPE_H__
#define __AUDIO_TYPE_H__

#include "td_type.h"
#include "audio_errno.h"

#ifdef __cplusplus
extern "C" {
#endif

#define AUDIO_U32_MAX 0xffffffff
#define AUDIO_INVALID_ID 0xffffffff
#define AUDIO_INVALID_HANDLE 0xffffffff
#define AUDIO_INVALID_TS 0

#define AUDIO_INVALID_PTS_US (-1LL)
#define AUDIO_INVALID_PTS_MS 0xffffffff
#define AUDIO_MS_TO_US_RATE  1000

#define STACK_MARKER 0x5A5A5A5A

#define pts_us_to_ms(pts)  (td_u32)((pts) / AUDIO_MS_TO_US_RATE)
#define pts_ms_to_us(pts)  ((td_s64)(pts) * AUDIO_MS_TO_US_RATE)

static inline td_u32 audio_pts_us_to_ms(td_s64 pts)
{
    return ((pts < 0) || (pts == AUDIO_INVALID_PTS_US)) ? AUDIO_INVALID_PTS_MS : pts_us_to_ms(pts);
}

static inline td_s64 audio_pts_ms_to_us(td_u32 pts)
{
    return (pts == AUDIO_INVALID_PTS_MS) ? AUDIO_INVALID_PTS_US : pts_ms_to_us(pts);
}

#ifdef CONFIG_SAP_RUN_UT
/* special adaptation for UT */
#define AUDIO_STATIC
#else
/* for normal SDK */
#define AUDIO_STATIC static
#endif

#define audio_unused(var) (td_void)(var)

#define is_4byte_aligned(var) (0 == ((var) & 3))

#define audio_align_up(size, align) (((size) + ((align) - 1)) & ~((align) - 1))
#define audio_align_up8(size) audio_align_up((size), 8)

#ifdef CONFIG_SAP_RUN_UT
#define audio_reg_write(addr, val)
#define audio_reg_read(addr, val)
#else
#define audio_reg_write(addr, val)              (*(volatile td_u32 *)(td_uintptr_t)(addr) = (val))
#define audio_reg_read(addr, val)               ((val) = *(volatile td_u32 *)(td_uintptr_t)(addr))
#endif

typedef enum {
    AUDIO_BIT0 = 0,
    AUDIO_BIT1,
    AUDIO_BIT2,
    AUDIO_BIT3,
    AUDIO_BIT4,
    AUDIO_BIT5,
    AUDIO_BIT6,
    AUDIO_BIT7,
    AUDIO_BIT8,
    AUDIO_BIT9,
    AUDIO_BIT10,
    AUDIO_BIT11,
    AUDIO_BIT12,
    AUDIO_BIT13,
    AUDIO_BIT14,
    AUDIO_BIT15,
    AUDIO_BIT16,
    AUDIO_BIT17,
    AUDIO_BIT18,
    AUDIO_BIT19,
    AUDIO_BIT20,
    AUDIO_BIT21,
    AUDIO_BIT22,
    AUDIO_BIT23,
    AUDIO_BIT24,
    AUDIO_BIT25,
    AUDIO_BIT26,
    AUDIO_BIT27,
    AUDIO_BIT28,
    AUDIO_BIT29,
    AUDIO_BIT30,
    AUDIO_BIT31,
    AUDIO_BIT_MAX
} audio_bit;

static inline td_u32 audio_reg_bit_rd(td_u32 addr, audio_bit start, audio_bit end)
{
    td_u32 val;
    td_u32 tmp_msk;
    td_u32 tmp_bit;
    td_u32 start_bit = (td_u32)start;
    td_u32 end_bit = (td_u32)end;

    /* 参数异常时则读取所有32bit内容 */
    if ((end_bit < start) || (start > AUDIO_BIT31) || (end > AUDIO_BIT31)) {
        start_bit = (td_u32)AUDIO_BIT0;
        end_bit   = (td_u32)AUDIO_BIT31;
    }

    /* 根据起始和结束比特生成掩码 */
    tmp_bit = (td_u32)AUDIO_BIT31 - end_bit;
    tmp_msk = AUDIO_U32_MAX << tmp_bit;
    tmp_msk = tmp_msk >> (start_bit + tmp_bit);
    tmp_msk = tmp_msk << start_bit;

    audio_reg_read(addr, val);
    val &= tmp_msk;
    val = val >> start_bit;

    return val;
}

static inline td_void audio_reg_bit_wr(td_u32 addr, audio_bit start_bit, audio_bit end_bit, td_u32 val)
{
    td_u32 set_val;
    td_u32 reg_val;
    td_u32 tmp_msk;
    td_u32 tmp_bit;

    /* 其它参数异常时进行异常处理 */
    if ((end_bit < start_bit) || (start_bit > AUDIO_BIT31) || (end_bit > AUDIO_BIT31)) {
        return;
    }

    /* 根据待写入内容和起始比特信息，生成写寄存器的数据 */
    set_val = val;
    set_val = set_val << (td_u32)start_bit;

    /* 根据起始和结束比特生成掩码 */
    tmp_bit = (td_u32)AUDIO_BIT31 - (td_u32)end_bit;
    tmp_msk = AUDIO_U32_MAX << tmp_bit;
    tmp_msk = tmp_msk >> ((td_u32)start_bit + tmp_bit);
    tmp_msk = tmp_msk << (td_u32)start_bit;

    /* 先读取wbbp寄存器地址中的内容，再写入指定bit数据 */
    audio_reg_read(addr, reg_val);
    reg_val &= (~tmp_msk);
    set_val &= tmp_msk;
    audio_reg_write(addr, (reg_val | set_val));

    return;
}

#define sys_check_null_ptr_return(ptr)       \
    do {                                     \
        if ((td_void *)(ptr) == TD_NULL) {   \
            return ERR_SYS_NULL_PTR;         \
        }                                    \
    } while (0)

#define adp_check_null_ptr_return(ptr)       \
    do {                                     \
        if ((td_void *)(ptr) == TD_NULL) {   \
            return ERR_ADP_NULL_PTR;         \
        }                                    \
    } while (0)

#define ai_check_null_ptr_return(ptr)        \
    do {                                     \
        if ((td_void *)(ptr) == TD_NULL) {   \
            return ERR_AI_NULL_PTR;          \
        }                                    \
    } while (0)

#define sea_check_null_ptr_return(ptr)       \
    do {                                     \
        if ((td_void *)(ptr) == TD_NULL) {   \
            return ERR_SEA_NULL_PTR;         \
        }                                    \
    } while (0)

#define ao_check_null_ptr_return(ptr)        \
    do {                                     \
        if ((td_void *)(ptr) == TD_NULL) {   \
            return ERR_AO_NULL_PTR;          \
        }                                    \
    } while (0)

#define track_check_null_ptr_return(ptr)     \
    do {                                     \
        if ((td_void *)(ptr) == TD_NULL) {   \
            return ERR_TRACK_NULL_PTR;       \
        }                                    \
    } while (0)

#define adec_check_null_ptr_return(ptr)      \
    do {                                     \
        if ((td_void *)(ptr) == TD_NULL) {   \
            return ERR_ADEC_NULL_PTR;        \
        }                                    \
    } while (0)

#define aenc_check_null_ptr_return(ptr)      \
    do {                                     \
        if ((td_void *)(ptr) == TD_NULL) {   \
            return ERR_AENC_NULL_PTR;        \
        }                                    \
    } while (0)

#define anc_check_null_ptr_return(ptr)       \
    do {                                     \
        if ((td_void *)(ptr) == TD_NULL) {   \
            return ERR_ANC_NULL_PTR;         \
        }                                    \
    } while (0)

#define haid_check_null_ptr_return(ptr)      \
    do {                                     \
        if ((td_void *)(ptr) == TD_NULL) {   \
            return ERR_HAID_NULL_PTR;        \
        }                                    \
    } while (0)

#define aef_check_null_ptr_return(ptr)       \
    do {                                     \
        if ((td_void *)(ptr) == TD_NULL) {   \
            return ERR_AEF_NULL_PTR;         \
        }                                    \
    } while (0)

#define haid_check_null_ptr_return(ptr)      \
    do {                                     \
        if ((td_void *)(ptr) == TD_NULL) {   \
            return ERR_HAID_NULL_PTR;        \
        }                                    \
    } while (0)

#define vendor_check_null_ptr_return(ptr)    \
    do {                                     \
        if ((td_void *)(ptr) == TD_NULL) {   \
            return ERR_VENDOR_NULL_PTR;      \
        }                                    \
    }while (0)

#define dpm_check_null_ptr_return(ptr)       \
    do {                                     \
        if ((td_void *)(ptr) == TD_NULL) {   \
            return ERR_DPM_NULL_PTR;         \
        }                                    \
    }while (0)

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __AUDIO_TYPE_H__ */
