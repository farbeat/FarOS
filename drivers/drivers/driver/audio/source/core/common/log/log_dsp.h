/*
 * Copyright (c) CompanyNameMagicTag 2022-2023. All rights reserved.
 * Description: header of log DSP
 * Author: audio
 * Create: 2022-10-15
 */

#ifndef LOG_DSP_H
#define LOG_DSP_H

#ifdef SAP_IPC_LOG_SUPPORT

#include "td_type.h"
#include "log_def_dsp.h"

#define OML_LOG_HEADER_ARRAY_LENTH        3
#define OML_LOG_ZERO_ARG_SEND             0
#define OML_LOG_ONE_ARG_SEND              1
#define OML_LOG_TWO_ARG_SEND              2
#define OML_LOG_THREE_ARG_SEND            3
#define OML_LOG_FOUR_ARG_SEND             4
#define OML_LOG_TAIL_LENTH                1
#define OM_FRAME_DELIMITER                0x7E

#define OML_LOG_LEVEL_OFFSET              6
#define OML_LOG_LEVEL_MASK                0x3
#define OML_LOG_MODULE_MASK               0x3F
#define OM_LOG_HEAD_PRESS_MASK            0xFF
#define OM_MSG_TYPE_LOG                   1
#define OM_MSG_TYPE_LOG_OFFSET            8
#define OM_LOG_RPT_IND                    3
#define OM_LOG_RPT_IND_MODULE_ID_OFFSET   16
#define OM_LOG_SN_OFFSET                  16

enum oml_log_mod_id {
    LOG_MOD_WIFI    = 0,
    LOG_MOD_BT      = 1,
    LOG_MOD_GNSS    = 2,
    LOG_MOD_DSP     = 3,
    LOG_MOD_PF      = 4,
    LOG_MOD_RESERVE = 5,
    LOG_MOD_NFC     = 6,
    LOG_MOD_APP     = 7,
    LOG_MOD_MAX,
};

enum om_msg_mod_id {
    OM_WIFI_HOST = 0x00,
    OM_BT        = 0x10,
    OM_GNSS      = 0x20,
    OM_DSP       = 0x30,
    OM_PF        = 0x40,
    OM_WIFI      = 0x50,
    OM_NFC       = 0x60,
    OM_IR        = 0x70,
    OM_MAX,
};

enum log_oam_entry_index {
    LOG_OAM_INDEX_0,
    LOG_OAM_INDEX_1,
    LOG_OAM_INDEX_2,
    LOG_OAM_INDEX_3,
    LOG_OAM_INDEX_4,
    LOG_OAM_INDEX_5,
    LOG_OAM_INDEX_6,
    LOG_OAM_INDEX_7,
};

typedef struct {
    td_u8 frame_start;
    td_u8 func_type;
    td_u8 prime_id;
    td_u8 arr_reserver[1];
    td_u16 frame_len;
    td_u16 sn;
} __attribute__((packed)) om_msg_header;

typedef enum {
    LOG_LEVEL_NONE,
    LOG_LEVEL_ERROR,
    LOG_LEVEL_WARNING,
    LOG_LEVEL_INFO,
    LOG_LEVEL_DBG,
    LOG_LEVEL_MAX,
} log_level_e;

/* Log message struct */
typedef struct {
    om_msg_header header;
    log_level_e level;
    td_u8 file_id_high;
    td_u8 file_id_low : 2;
    td_u8 line_num_high : 6;
    td_u8 line_num_low;
} om_log_header_t;

#define get_log_level(press_para) (((press_para) >> OML_LOG_LEVEL_OFFSET) & OML_LOG_LEVEL_MASK)

#define log_head_press(msg_mod_id) ((OM_FRAME_DELIMITER & OM_LOG_HEAD_PRESS_MASK) | \
                                   ((OM_MSG_TYPE_LOG & OM_LOG_HEAD_PRESS_MASK) << OM_MSG_TYPE_LOG_OFFSET) |  \
                                   ((OM_LOG_RPT_IND & OM_LOG_HEAD_PRESS_MASK) << OM_LOG_RPT_IND_MODULE_ID_OFFSET) |  \
                                   (((msg_mod_id) & OM_LOG_HEAD_PRESS_MASK) << OM_LOG_RPT_IND_MODULE_ID_OFFSET))
/**
 * @brief  Fill this 32 bit value as little ending format
 * Last by will be send as the first byte by log system.
 * |----------------------------------------------------------------------------------------------------------------|
 * |-H-------------------------------------------------- 32 bit --------------------------------------------------L-|
 * |        8 bit       |                  8 bit                  |        8 bit       |            8 bit           |
 * | line_num low 8 bit | line_num high 6 bit + file_id low 2 bit | file_id high 8 bit | level 2 bit + mod_id 6 bit |
 * |----------------------------------------------------------------------------------------------------------------|
 */
#define para_press(mod_id, level, file_id, line_num) (((td_u32)(((td_u32)(line_num) & 0xFF) << 0x18)) |  \
                                                     ((td_u32)((((td_u32)(file_id) & 0x3) << 0x10) |   \
                                                     ((td_u32)(line_num) & 0x3F00) << 0xA))  |          \
                                                     ((td_u32)(((td_u32)(file_id) & 0x3FC) << 0x6)) |  \
                                                     ((td_u32)(((mod_id) & 0x3F) | (((level) & 0x3) << 0x6))))
#define oam_log_lenth(arg_num)              (sizeof(om_log_header_t) + sizeof(td_u32) * (arg_num) + sizeof(td_u8))
#define log_lenth_and_sn_press(arg_num, SN) (oam_log_lenth(arg_num) | ((td_u32)(SN) << OM_LOG_SN_OFFSET))

#define oml_dsp_log_print0(level, fmt) \
    log_event_print0(log_head_press(OM_DSP), para_press(LOG_MOD_DSP, level, THIS_FILE_ID, __LINE__))
#define oml_dsp_log_print1(level, fmt, p1) \
    log_event_print1(log_head_press(OM_DSP), para_press(LOG_MOD_DSP, level, THIS_FILE_ID, __LINE__), (td_u32)(p1))
#define oml_dsp_log_print2(level, fmt, p1, p2) \
    log_event_print2(log_head_press(OM_DSP), para_press(LOG_MOD_DSP, level, THIS_FILE_ID, __LINE__), (td_u32)(p1), \
                     (td_u32)(p2))
#define oml_dsp_log_print3(level, fmt, p1, p2, p3) \
    log_event_print3(log_head_press(OM_DSP), para_press(LOG_MOD_DSP, level, THIS_FILE_ID, __LINE__), (td_u32)(p1), \
                     (td_u32)(p2), (td_u32)(p3))
#define oml_dsp_log_print4(level, fmt, p1, p2, p3, p4) \
    log_event_print4(log_head_press(OM_DSP), para_press(LOG_MOD_DSP, level, THIS_FILE_ID, __LINE__), (td_u32)(p1), \
                     (td_u32)(p2), (td_u32)(p3), (td_u32)(p4))

void log_event_print0(td_u32 log_header, td_u32 press_para);
void log_event_print1(td_u32 log_header, td_u32 press_para, td_u32 para1);
void log_event_print2(td_u32 log_header, td_u32 press_para, td_u32 para1, td_u32 para2);
void log_event_print3(td_u32 log_header, td_u32 press_para, td_u32 para1, td_u32 para2, td_u32 para3);
void log_event_print4(td_u32 log_header, td_u32 press_para, td_u32 para1, td_u32 para2, td_u32 para3, td_u32 para4);
td_s32 set_dsp_log_level(td_u32 log_level);

#else

#define oml_dsp_log_print0(level, fmt)
#define oml_dsp_log_print1(level, fmt, p1)
#define oml_dsp_log_print2(level, fmt, p1, p2)
#define oml_dsp_log_print3(level, fmt, p1, p2, p3)
#define oml_dsp_log_print4(level, fmt, p1, p2, p3, p4)

#endif /* SAP_LOG_IPC */
#endif /* LOG_DSP_H */
