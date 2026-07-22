/*
 * Copyright (c) CompanyNameMagicTag 2020-2020. All rights reserved.
 * Description: source file for sea msg
 * Author: audio
 * Create: 2020-05-07
 * Notes: NA
 * History: 2020-05-07 1st version
 */

#include "securec.h"
#include "audio_struct.h"
#include "audio_osal.h"
#include "audio_debug.h"
#include "core_msg_struct.h"
#include "core_msg.h"
#include "drv_msg_sea.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define drv_sea_send_msg(msg)  core_msg_send(AUDIO_CORE_DSP0, MSG_CORE_SEA_MODULE, msg, MSG_SYNC_WAIT_TIME_MS)

td_s32 drv_msg_sea_init(td_void)
{
    core_msg_data msg_data;

    msg_data.cmd = MSG_CORE_SEA_INIT;
    msg_data.input_data_size = 0;
    msg_data.output_data_size = 0;

    return drv_sea_send_msg(&msg_data);
}

td_s32 drv_msg_sea_deinit(td_void)
{
    core_msg_data msg_data;

    msg_data.cmd = MSG_CORE_SEA_DEINIT;
    msg_data.input_data_size = 0;
    msg_data.output_data_size = 0;

    return drv_sea_send_msg(&msg_data);
}

td_s32 drv_msg_sea_get_eng_caps(sea_lib_id lib_id, sea_eng_caps *eng_caps)
{
    core_msg_data msg_data;

    msg_data.cmd = MSG_CORE_SEA_GET_ENG_CAPS;
    msg_data.handle = (td_u32)lib_id;
    msg_data.input_data_size = 0;
    msg_data.output_data = eng_caps;
    msg_data.output_data_size = (td_u32)sizeof(*eng_caps);

    return drv_sea_send_msg(&msg_data);
}

td_s32 drv_msg_sea_get_def_pcm_attr(sea_eng_sel *eng_sel, sea_pcm_attr *attr)
{
    core_msg_data msg_data;

    msg_data.cmd = MSG_CORE_SEA_GET_DEF_ATTR;
    msg_data.handle = 0;
    msg_data.input_data = eng_sel;
    msg_data.input_data_size = (td_u32)sizeof(*eng_sel);
    msg_data.output_data = attr;
    msg_data.output_data_size = (td_u32)sizeof(*attr);

    return drv_sea_send_msg(&msg_data);
}

td_s32 drv_msg_sea_create(td_handle *sea, sea_eng_sel *eng_sel, const sea_pcm_attr *attr)
{
    core_msg_data msg_data;
    msg_data_sea_attr sea_attr;

    (td_void)memcpy_s(&sea_attr.eng_sel, sizeof(sea_attr.eng_sel), eng_sel, sizeof(*eng_sel));
    (td_void)memcpy_s(&sea_attr.attr, sizeof(sea_attr.attr), attr, sizeof(*attr));
    msg_data.cmd = MSG_CORE_SEA_CREATE;
    msg_data.handle = 0;
    msg_data.input_data = (td_void *)&sea_attr;
    msg_data.input_data_size = (td_u32)sizeof(sea_attr);
    msg_data.output_data = sea;
    msg_data.output_data_size = (td_u32)sizeof(*sea);

    return drv_sea_send_msg(&msg_data);
}

td_s32 drv_msg_sea_destroy(td_handle sea)
{
    core_msg_data msg_data;

    msg_data.cmd = MSG_CORE_SEA_DESTROY;
    msg_data.handle = sea;
    msg_data.input_data_size = 0;
    msg_data.output_data_size = 0;

    return drv_sea_send_msg(&msg_data);
}

td_s32 drv_msg_sea_start(td_handle sea)
{
    core_msg_data msg_data;

    msg_data.cmd = MSG_CORE_SEA_START;
    msg_data.handle = sea;
    msg_data.input_data_size = 0;
    msg_data.output_data_size = 0;

    return drv_sea_send_msg(&msg_data);
}

td_s32 drv_msg_sea_stop(td_handle sea)
{
    core_msg_data msg_data;

    msg_data.cmd = MSG_CORE_SEA_STOP;
    msg_data.handle = sea;
    msg_data.input_data_size = 0;
    msg_data.output_data_size = 0;

    return drv_sea_send_msg(&msg_data);
}

td_s32 drv_msg_sea_get_eng_attr(td_handle sea, sea_eng_attr *attr)
{
    core_msg_data msg_data;

    msg_data.cmd = MSG_CORE_SEA_GET_ENG_ATTR;
    msg_data.handle = sea;
    msg_data.input_data = &attr->type;
    msg_data.input_data_size = (td_u32)sizeof(attr->type);
    msg_data.output_data = attr;
    msg_data.output_data_size = (td_u32)sizeof(*attr);

    return drv_sea_send_msg(&msg_data);
}

td_s32 drv_msg_sea_set_eng_attr(td_handle sea, sea_eng_attr *attr)
{
    core_msg_data msg_data;

    msg_data.cmd = MSG_CORE_SEA_SET_ENG_ATTR;
    msg_data.handle = sea;
    msg_data.input_data = attr;
    msg_data.input_data_size = (td_u32)sizeof(*attr);
    msg_data.output_data_size = 0;

    return drv_sea_send_msg(&msg_data);
}

td_s32 drv_msg_sea_get_pcm_attr(td_handle sea, sea_pcm_attr *attr)
{
    core_msg_data msg_data;

    msg_data.cmd = MSG_CORE_SEA_GET_PCM_ATTR;
    msg_data.handle = sea;
    msg_data.input_data_size = 0;
    msg_data.output_data = attr;
    msg_data.output_data_size = (td_u32)sizeof(*attr);

    return drv_sea_send_msg(&msg_data);
}

td_s32 drv_msg_sea_set_pcm_attr(td_handle sea, sea_pcm_attr *attr)
{
    core_msg_data msg_data;

    msg_data.cmd = MSG_CORE_SEA_SET_PCM_ATTR;
    msg_data.handle = sea;
    msg_data.input_data = attr;
    msg_data.input_data_size = (td_u32)sizeof(*attr);
    msg_data.output_data_size = 0;

    return drv_sea_send_msg(&msg_data);
}

td_s32 drv_msg_sea_get_param(td_handle sea, td_u32 type, td_void *param, td_u32 size)
{
    core_msg_data msg_data;
    msg_data_sea_param_get sea_param;

    sea_param.type = type;
    sea_param.param_size = size;

    msg_data.cmd = MSG_CORE_SEA_GET_PARAM;
    msg_data.handle = sea;
    msg_data.input_data = &sea_param;
    msg_data.input_data_size = (td_u32)sizeof(sea_param);
    msg_data.output_data = param;
    msg_data.output_data_size = size;

    return drv_sea_send_msg(&msg_data);
}

td_s32 drv_msg_sea_set_param(td_handle sea, td_u32 type, const td_void *param, td_u32 size)
{
    td_s32 ret;
    core_msg_data msg_data;
    msg_data_sea_param_set *sea_param = TD_NULL;

    sea_param = (msg_data_sea_param_set *)audio_osal_malloc(sizeof(msg_data_sea_param_set));
    if (sea_param == TD_NULL) {
        return ERR_SYS_MEM_ALLOC;
    }

    sea_param->type = type;
    sea_param->param_size = size;
    ret = memcpy_s(&sea_param->param[0], sizeof(sea_param->param), param, size);
    if (ret != EOK) {
        free(sea_param);
        return ret;
    }

    msg_data.cmd = MSG_CORE_SEA_SET_PARAM;
    msg_data.handle = sea;
    msg_data.input_data = sea_param;
    msg_data.input_data_size = (td_u32)sizeof(msg_data_sea_param_set);
    msg_data.output_data_size = 0;

    ret = drv_sea_send_msg(&msg_data);
    audio_osal_free(sea_param);
    return ret;
}

td_s32 drv_msg_sea_set_bypass(td_handle sea, td_bool enable)
{
    core_msg_data msg_data;

    msg_data.cmd = MSG_CORE_SEA_SET_BYPASS;
    msg_data.handle = sea;
    msg_data.input_data = &enable;
    msg_data.input_data_size = (td_u32)sizeof(enable);
    msg_data.output_data_size = 0;

    return drv_sea_send_msg(&msg_data);
}

td_s32 drv_msg_sea_get_item_count(td_handle sea, sea_item_type type, td_u32 *count)
{
    core_msg_data msg_data;

    msg_data.cmd = MSG_CORE_SEA_GET_ITEM_COUNT;
    msg_data.handle = sea;
    msg_data.input_data = &type;
    msg_data.input_data_size = (td_u32)sizeof(type);
    msg_data.output_data = count;
    msg_data.output_data_size = (td_u32)sizeof(*count);

    return drv_sea_send_msg(&msg_data);
}

td_s32 drv_msg_sea_get_item_sets(td_handle sea, sea_item_type type, sea_item_sets *sets)
{
    core_msg_data msg_data;
    msg_data_sea_item_index sea_item;

    sea_item.type = type;
    sea_item.index = sets->index;

    msg_data.cmd = MSG_CORE_SEA_GET_ITEM_SETS;
    msg_data.handle = sea;
    msg_data.input_data = &sea_item;
    msg_data.input_data_size = (td_u32)sizeof(sea_item);
    msg_data.output_data = sets;
    msg_data.output_data_size = (td_u32)sizeof(*sets);

    return drv_sea_send_msg(&msg_data);
}

td_s32 drv_msg_sea_add_item(td_handle sea, sea_item_type type, sea_info_item *item)
{
    core_msg_data msg_data;
    msg_data_sea_item_node sea_item;

    sea_item.type = type;
    (td_void)memcpy_s(&sea_item.node, sizeof(sea_item.node), item, sizeof(*item));

    msg_data.cmd = MSG_CORE_SEA_ADD_ITEM;
    msg_data.handle = sea;
    msg_data.input_data = &sea_item;
    msg_data.input_data_size = (td_u32)sizeof(sea_item);
    msg_data.output_data_size = 0;

    return drv_sea_send_msg(&msg_data);
}

td_s32 drv_msg_sea_remove_item(td_handle sea, sea_item_type type, sea_info_item *item)
{
    core_msg_data msg_data;
    msg_data_sea_item_node sea_item;

    sea_item.type = type;
    (td_void)memcpy_s(&sea_item.node, sizeof(sea_item.node), item, sizeof(*item));

    msg_data.cmd = MSG_CORE_SEA_REMOVE_ITEM;
    msg_data.handle = sea;
    msg_data.input_data = &sea_item;
    msg_data.input_data_size = (td_u32)sizeof(sea_item);
    msg_data.output_data_size = 0;

    return drv_sea_send_msg(&msg_data);
}

td_s32 drv_msg_sea_enable_vid_enroll(td_handle sea, td_bool enable)
{
    core_msg_data msg_data;

    msg_data.cmd = MSG_CORE_SEA_ENABLE_VID_ENROLL;
    msg_data.handle = sea;
    msg_data.input_data = &enable;
    msg_data.input_data_size = (td_u32)sizeof(enable);
    msg_data.output_data_size = 0;

    return drv_sea_send_msg(&msg_data);
}

td_s32 drv_msg_sea_attach_output(td_handle sea, sea_output_type type, td_handle output)
{
    core_msg_data msg_data;
    msg_data_adp_port data_port;

    data_port.data_port_id = (td_u32)type;
    data_port.h_mod = output;

    msg_data.cmd = MSG_CORE_SEA_ATTACH_OUTPUT;
    msg_data.handle = sea;
    msg_data.input_data = &data_port;
    msg_data.input_data_size = (td_u32)sizeof(data_port);
    msg_data.output_data_size = 0;

    return drv_sea_send_msg(&msg_data);
}

td_s32 drv_msg_sea_detach_output(td_handle sea, sea_output_type type, td_handle output)
{
    core_msg_data msg_data;
    msg_data_adp_port data_port;

    data_port.data_port_id = (td_u32)type;
    data_port.h_mod = output;

    msg_data.cmd = MSG_CORE_SEA_DETACH_OUTPUT;
    msg_data.handle = sea;
    msg_data.input_data = &data_port;
    msg_data.input_data_size = (td_u32)sizeof(data_port);
    msg_data.output_data_size = 0;

    return drv_sea_send_msg(&msg_data);
}

td_s32 drv_msg_sea_get_proc_info(td_handle sea, sea_proc_item *proc)
{
    core_msg_data msg_data;

    msg_data.cmd = MSG_CORE_SEA_GET_PROC_INFO;
    msg_data.handle = sea;
    msg_data.input_data_size = 0;
    msg_data.output_data = proc;
    msg_data.output_data_size = (td_u32)sizeof(sea_proc_item);

    return drv_sea_send_msg(&msg_data);
}

td_s32 drv_msg_sea_query_lib_status(sea_lib_id lib_id, audio_lib_load_type load_type, td_bool *need_action)
{
    core_msg_data msg_data;

    msg_data.cmd = MSG_CORE_SEA_QUERY_LIB_STATUS;
    msg_data.handle = lib_id;
    msg_data.input_data = &load_type;
    msg_data.input_data_size = (td_u32)sizeof(audio_lib_load_type);
    msg_data.output_data = need_action;
    msg_data.output_data_size = (td_u32)sizeof(td_bool);

    return drv_sea_send_msg(&msg_data);
}

td_s32 drv_msg_sea_update_lib_status(sea_lib_id lib_id, audio_lib_load_type load_type)
{
    core_msg_data msg_data;

    msg_data.cmd = MSG_CORE_SEA_UPDATE_LIB_STATUS;
    msg_data.handle = lib_id;
    msg_data.input_data = &load_type;
    msg_data.input_data_size = (td_u32)sizeof(audio_lib_load_type);
    msg_data.output_data_size = 0;

    return drv_sea_send_msg(&msg_data);
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
