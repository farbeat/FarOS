/*
 * Copyright (c) CompanyNameMagicTag 2020-2020. All rights reserved.
 * Description: source file for mpi sea
 * Author: audio
 */

#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>

#include "securec.h"
#include "audio_type.h"
#include "drv_sap_ioctl.h"

#include "mpi_sea.h"

#ifdef SAP_SEA_DUMP_SUPPORT
#include "mpi_dump.h"
#endif

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define SEA_DUMP_FILE_SIZE (1024 * 1024)
#define SEA_DUMP_FILE_PATH "/user/stream/sea_dump.pcm"

#define SAP_SEA_DEV_NAME "/dev/" UMAP_DEVNAME_SEA

#define mpi_sea_check_inited()         \
    do {                               \
        if (g_mpi_sea_init_cnt == 0) { \
            return ERR_SEA_NOT_INIT;   \
        }                              \
    } while (0)

static td_s32 g_mpi_sea_fd = -1;
static td_u32 g_mpi_sea_init_cnt = 0;
static pthread_mutex_t g_mpi_sea_mutex = PTHREAD_MUTEX_INITIALIZER;

static td_void mpi_sea_lock(td_void)
{
    (td_void)pthread_mutex_lock(&g_mpi_sea_mutex);
}

static td_void mpi_sea_unlock(td_void)
{
    (td_void)pthread_mutex_unlock(&g_mpi_sea_mutex);
}

td_s32 mpi_sea_init(td_void)
{
    mpi_sea_lock();

    if (g_mpi_sea_init_cnt > 0) {
        g_mpi_sea_init_cnt++;
        mpi_sea_unlock();
        return AUDIO_SUCCESS;
    }

    if (g_mpi_sea_fd < 0) {
        g_mpi_sea_fd = audio_open(SAP_SEA_DEV_NAME, O_RDWR, 0);
        if (g_mpi_sea_fd < 0) {
            g_mpi_sea_fd = -1;
            mpi_sea_unlock();
            return ERR_SEA_INIT_FAILED;
        }

        audio_api_time_consum_stat();

        audio_ioctl(g_mpi_sea_fd, CMD_IOCTL_SEA_INIT, TD_NULL);
    }

    g_mpi_sea_init_cnt++;
    mpi_sea_unlock();
    return AUDIO_SUCCESS;
}

td_s32 mpi_sea_deinit(td_void)
{
    mpi_sea_lock();

    if (g_mpi_sea_init_cnt == 0) {
        mpi_sea_unlock();
        return AUDIO_SUCCESS;
    }
    if (g_mpi_sea_init_cnt > 1) {
        g_mpi_sea_init_cnt--;
        mpi_sea_unlock();
        return AUDIO_SUCCESS;
    }

    if (g_mpi_sea_fd > 0) {
        audio_api_time_consum_stat();

        audio_ioctl(g_mpi_sea_fd, CMD_IOCTL_SEA_DEINIT, TD_NULL);
        audio_close(g_mpi_sea_fd);
        g_mpi_sea_fd = -1;
    }

    g_mpi_sea_init_cnt--;
    mpi_sea_unlock();
    return AUDIO_SUCCESS;
}

td_s32 mpi_sea_load_engine(sea_lib_id lib_id, const td_char *lib_name)
{
    td_s32 ret;
    sap_drv_load drv_load;

    mpi_sea_check_inited();
    mpi_sea_lock();

    (td_void)memset_s(&drv_load, sizeof(drv_load), 0, sizeof(drv_load));
    if (lib_name != TD_NULL) {
        ret = strcpy_s(drv_load.name, sizeof(drv_load.name), lib_name);
        if (ret != EOK) {
            mpi_sea_unlock();
            return ret;
        }
    }

    drv_load.id = lib_id;

    audio_api_time_consum_stat();

    ret = audio_ioctl(g_mpi_sea_fd, CMD_IOCTL_SEA_LOAD_ENG, &drv_load);
    if (ret != AUDIO_SUCCESS) {
        mpi_sea_unlock();
        return ret;
    }

    mpi_sea_unlock();
    return AUDIO_SUCCESS;
}

td_s32 mpi_sea_unload_engine(sea_lib_id lib_id, const td_char *lib_name)
{
    td_s32 ret;
    sap_drv_load drv_load;

    mpi_sea_check_inited();
    mpi_sea_lock();

    (td_void)memset_s(&drv_load, sizeof(drv_load), 0, sizeof(drv_load));
    if (lib_name != TD_NULL) {
        ret = strcpy_s(drv_load.name, sizeof(drv_load.name), lib_name);
        if (ret != EOK) {
            mpi_sea_unlock();
            return ret;
        }
    }
    drv_load.id = lib_id;

    audio_api_time_consum_stat();

    ret = audio_ioctl(g_mpi_sea_fd, CMD_IOCTL_SEA_UNLOAD_ENG, &drv_load);
    if (ret != AUDIO_SUCCESS) {
        mpi_sea_unlock();
        return ret;
    }

    mpi_sea_unlock();
    return AUDIO_SUCCESS;
}

td_s32 mpi_sea_get_engine_caps(sea_lib_id lib_id, sea_eng_caps *eng_caps)
{
    td_s32 ret;
    sap_sea_eng_caps sea_caps;

    sea_check_null_ptr_return(eng_caps);

    mpi_sea_check_inited();
    mpi_sea_lock();

    sea_caps.lib_id = lib_id;

    audio_api_time_consum_stat();

    ret = audio_ioctl(g_mpi_sea_fd, CMD_IOCTL_SEA_GET_ENG_CAPS, &sea_caps);
    if (ret != AUDIO_SUCCESS) {
        mpi_sea_unlock();
        return ret;
    }

    (td_void)memcpy_s(eng_caps, sizeof(*eng_caps), &sea_caps.eng_caps, sizeof(sea_caps.eng_caps));
    mpi_sea_unlock();
    return AUDIO_SUCCESS;
}

td_s32 mpi_sea_get_def_attr(const sea_eng_sel *eng_sel, sea_pcm_attr *attr)
{
    td_s32 ret;
    sap_sea_attr sea_attr;

    sea_check_null_ptr_return(eng_sel);
    sea_check_null_ptr_return(attr);

    mpi_sea_check_inited();
    mpi_sea_lock();

    (td_void)memcpy_s(&sea_attr.eng_sel, sizeof(sea_attr.eng_sel), eng_sel, sizeof(*eng_sel));

    audio_api_time_consum_stat();

    ret = audio_ioctl(g_mpi_sea_fd, CMD_IOCTL_SEA_GET_DEF_ATTR, &sea_attr);
    if (ret != AUDIO_SUCCESS) {
        mpi_sea_unlock();
        return ret;
    }
    (td_void)memcpy_s(attr, sizeof(*attr), &sea_attr.attr, sizeof(sea_attr.attr));

    mpi_sea_unlock();
    return AUDIO_SUCCESS;
}

td_s32 mpi_sea_create(td_handle *sea, const sea_eng_sel *eng_sel, const sea_pcm_attr *attr)
{
    td_s32 ret;
    sap_sea_attr sea_attr;

    sea_check_null_ptr_return(sea);
    sea_check_null_ptr_return(eng_sel);
    sea_check_null_ptr_return(attr);

    mpi_sea_check_inited();
    mpi_sea_lock();

    (td_void)memcpy_s(&sea_attr.eng_sel, sizeof(sea_attr.eng_sel), eng_sel, sizeof(*eng_sel));
    (td_void)memcpy_s(&sea_attr.attr, sizeof(sea_attr.attr), attr, sizeof(*attr));

    audio_api_time_consum_stat();

    ret = audio_ioctl(g_mpi_sea_fd, CMD_IOCTL_SEA_CREATE, &sea_attr);
    if (ret != AUDIO_SUCCESS) {
        mpi_sea_unlock();
        return ret;
    }

    *sea = sea_attr.h_mod;
    mpi_sea_unlock();

#ifdef SAP_SEA_DUMP_SUPPORT
    if (mpi_dump_get_mod_flag(*sea) == TD_TRUE) {
        /* modify output type if want to dump other. */
        (td_void)mpi_dump_open(*sea, SEA_OUTPUT_REC_SRC, SEA_DUMP_FILE_PATH, SEA_DUMP_FILE_SIZE);
    }
#endif

    return AUDIO_SUCCESS;
}

td_s32 mpi_sea_destroy(td_handle sea)
{
    td_s32 ret;
    sap_drv_handle sea_handle;

    mpi_sea_check_inited();

#ifdef SAP_SEA_DUMP_SUPPORT
    if (mpi_dump_get_mod_flag(sea) == TD_TRUE) {
        (td_void)mpi_dump_close(sea);
    }
#endif

    mpi_sea_lock();

    sea_handle.h_mod = sea;

    audio_api_time_consum_stat();

    ret = audio_ioctl(g_mpi_sea_fd, CMD_IOCTL_SEA_DESTROY, &sea_handle);
    if (ret != AUDIO_SUCCESS) {
        mpi_sea_unlock();
        return ret;
    }

    mpi_sea_unlock();
    return AUDIO_SUCCESS;
}

td_s32 mpi_sea_register_event_proc(td_handle sea, audio_event_proc proc, td_void *context)
{
    td_s32 ret;
    sap_drv_event sea_event;

    sea_check_null_ptr_return(proc);

    mpi_sea_check_inited();
    mpi_sea_lock();

    sea_event.h_mod = sea;
    sea_event.context = (td_u64)(td_uintptr_t)context;
    sea_event.event_proc = (td_u64)(td_uintptr_t)proc;

    audio_api_time_consum_stat();

    ret = audio_ioctl(g_mpi_sea_fd, CMD_IOCTL_SEA_REG_EVENT_PROC, &sea_event);
    if (ret != AUDIO_SUCCESS) {
        mpi_sea_unlock();
        return ret;
    }

    mpi_sea_unlock();
    return AUDIO_SUCCESS;
}

td_s32 mpi_sea_start(td_handle sea)
{
    td_s32 ret;
    sap_drv_handle sea_handle;

    mpi_sea_check_inited();
    mpi_sea_lock();

    sea_handle.h_mod = sea;

    audio_api_time_consum_stat();

    ret = audio_ioctl(g_mpi_sea_fd, CMD_IOCTL_SEA_START, &sea_handle);
    if (ret != AUDIO_SUCCESS) {
        mpi_sea_unlock();
        return ret;
    }

    mpi_sea_unlock();
    return AUDIO_SUCCESS;
}

td_s32 mpi_sea_stop(td_handle sea)
{
    td_s32 ret;
    sap_drv_handle sea_handle;

    mpi_sea_check_inited();
    mpi_sea_lock();

    sea_handle.h_mod = sea;

    audio_api_time_consum_stat();

    ret = audio_ioctl(g_mpi_sea_fd, CMD_IOCTL_SEA_STOP, &sea_handle);
    if (ret != AUDIO_SUCCESS) {
        mpi_sea_unlock();
        return ret;
    }

    mpi_sea_unlock();
    return AUDIO_SUCCESS;
}

td_s32 mpi_sea_get_eng_attr(td_handle sea, td_u8 type, sea_eng_attr *attr)
{
    td_s32 ret;
    sap_sea_eng_attr eng_attr;

    sea_check_null_ptr_return(attr);

    mpi_sea_check_inited();
    mpi_sea_lock();

    eng_attr.h_mod = sea;
    eng_attr.attr.type = type;

    audio_api_time_consum_stat();

    ret = audio_ioctl(g_mpi_sea_fd, CMD_IOCTL_SEA_GET_ENG_ATTR, &eng_attr);
    if (ret != AUDIO_SUCCESS) {
        mpi_sea_unlock();
        return ret;
    }
    (td_void)memcpy_s(attr, sizeof(*attr), &eng_attr.attr, sizeof(eng_attr.attr));

    mpi_sea_unlock();
    return AUDIO_SUCCESS;
}

td_s32 mpi_sea_set_eng_attr(td_handle sea, td_u8 type, const sea_eng_attr *attr)
{
    td_s32 ret;
    sap_sea_eng_attr eng_attr;

    sea_check_null_ptr_return(attr);

    mpi_sea_check_inited();
    mpi_sea_lock();

    eng_attr.h_mod = sea;
    eng_attr.attr.type = type;
    (td_void)memcpy_s(&eng_attr.attr, sizeof(eng_attr.attr), attr, sizeof(*attr));

    audio_api_time_consum_stat();

    ret = audio_ioctl(g_mpi_sea_fd, CMD_IOCTL_SEA_SET_ENG_ATTR, &eng_attr);
    if (ret != AUDIO_SUCCESS) {
        mpi_sea_unlock();
        return ret;
    }

    mpi_sea_unlock();
    return AUDIO_SUCCESS;
}

td_s32 mpi_sea_get_pcm_attr(td_handle sea, sea_pcm_attr *attr)
{
    td_s32 ret;
    sap_sea_attr sea_attr;

    sea_check_null_ptr_return(attr);

    mpi_sea_check_inited();
    mpi_sea_lock();

    sea_attr.h_mod = sea;

    audio_api_time_consum_stat();

    ret = audio_ioctl(g_mpi_sea_fd, CMD_IOCTL_SEA_GET_PCM_ATTR, &sea_attr);
    if (ret != AUDIO_SUCCESS) {
        mpi_sea_unlock();
        return ret;
    }
    (td_void)memcpy_s(attr, sizeof(*attr), &sea_attr.attr, sizeof(sea_attr.attr));

    mpi_sea_unlock();
    return AUDIO_SUCCESS;
}

td_s32 mpi_sea_set_pcm_attr(td_handle sea, const sea_pcm_attr *attr)
{
    td_s32 ret;
    sap_sea_attr sea_attr;

    sea_check_null_ptr_return(attr);

    mpi_sea_check_inited();
    mpi_sea_lock();

    sea_attr.h_mod = sea;
    (td_void)memcpy_s(&sea_attr.attr, sizeof(sea_attr.attr), attr, sizeof(*attr));

    audio_api_time_consum_stat();

    ret = audio_ioctl(g_mpi_sea_fd, CMD_IOCTL_SEA_SET_PCM_ATTR, &sea_attr);
    if (ret != AUDIO_SUCCESS) {
        mpi_sea_unlock();
        return ret;
    }

    mpi_sea_unlock();
    return AUDIO_SUCCESS;
}

td_s32 mpi_sea_get_param(td_handle sea, sea_afe_type type, td_void *param, td_u32 param_size)
{
    td_s32 ret;
    sap_sea_param ioctl_attr;

    sea_check_null_ptr_return(param);

    if (param_size > SEA_PARAM_SIZE_MAX) {
        return ERR_SEA_INVALID_PARAM;
    }

    mpi_sea_check_inited();
    mpi_sea_lock();

    ioctl_attr.h_mod = sea;
    ioctl_attr.type = type;
    ioctl_attr.param_size = param_size;

    ret = audio_ioctl(g_mpi_sea_fd, CMD_IOCTL_SEA_GET_PARAM, &ioctl_attr);
    if (ret != AUDIO_SUCCESS) {
        mpi_sea_unlock();
        return ret;
    }

    ret = memcpy_s(param, param_size, ioctl_attr.param, ioctl_attr.param_size);
    if (ret != EOK) {
        mpi_sea_unlock();
        return ret;
    }

    mpi_sea_unlock();

    return AUDIO_SUCCESS;
}

td_s32 mpi_sea_set_param(td_handle sea, sea_afe_type type, const td_void *param, td_u32 param_size)
{
    td_s32 ret;
    sap_sea_param ioctl_attr;

    sea_check_null_ptr_return(param);

    if (param_size > SEA_PARAM_SIZE_MAX) {
        return ERR_SEA_INVALID_PARAM;
    }

    mpi_sea_check_inited();
    mpi_sea_lock();

    ioctl_attr.type = type;
    ioctl_attr.h_mod = sea;

    ret = memcpy_s(ioctl_attr.param, SEA_PARAM_SIZE_MAX, param, param_size);
    if (ret != EOK) {
        mpi_sea_unlock();
        return ret;
    }

    ioctl_attr.param_size = param_size;

    ret = audio_ioctl(g_mpi_sea_fd, CMD_IOCTL_SEA_SET_PARAM, &ioctl_attr);
    if (ret != AUDIO_SUCCESS) {
        mpi_sea_unlock();
        return ret;
    }

    mpi_sea_unlock();

    return AUDIO_SUCCESS;
}

td_s32 mpi_sea_set_bypass(td_handle sea, td_bool enable)
{
    td_s32 ret;
    sap_drv_enable sea_enable;

    mpi_sea_check_inited();
    mpi_sea_lock();

    sea_enable.h_mod = sea;
    sea_enable.enable = enable;

    audio_api_time_consum_stat();

    ret = audio_ioctl(g_mpi_sea_fd, CMD_IOCTL_SEA_SET_BYPASS, &sea_enable);
    if (ret != AUDIO_SUCCESS) {
        mpi_sea_unlock();
        return ret;
    }

    mpi_sea_unlock();
    return AUDIO_SUCCESS;
}

td_s32 mpi_sea_get_item_count(td_handle sea, sea_item_type type, td_u32 *count)
{
    td_s32 ret;
    sap_sea_item_count sea_item;

    sea_check_null_ptr_return(count);

    mpi_sea_check_inited();
    mpi_sea_lock();

    sea_item.h_mod = sea;
    sea_item.type = type;

    audio_api_time_consum_stat();

    ret = audio_ioctl(g_mpi_sea_fd, CMD_IOCTL_SEA_GET_ITEM_COUNT, &sea_item);
    if (ret != AUDIO_SUCCESS) {
        mpi_sea_unlock();
        return ret;
    }
    *count = sea_item.count;

    mpi_sea_unlock();
    return AUDIO_SUCCESS;
}

td_s32 mpi_sea_get_item_sets(td_handle sea, sea_item_type type, sea_item_sets *sets)
{
    td_s32 ret;
    sap_sea_item_sets sea_item;

    sea_check_null_ptr_return(sets);

    mpi_sea_check_inited();
    mpi_sea_lock();

    sea_item.h_mod = sea;
    sea_item.type = type;
    sea_item.sets.index = sets->index;

    audio_api_time_consum_stat();

    ret = audio_ioctl(g_mpi_sea_fd, CMD_IOCTL_SEA_GET_ITEM_SETS, &sea_item);
    if (ret != AUDIO_SUCCESS) {
        mpi_sea_unlock();
        return ret;
    }
    (td_void)memcpy_s(sets, sizeof(*sets), &sea_item.sets, sizeof(sea_item.sets));

    mpi_sea_unlock();
    return AUDIO_SUCCESS;
}

td_s32 mpi_sea_add_item(td_handle sea, sea_item_type type, const sea_info_item *item)
{
    td_s32 ret;
    sap_sea_item_node sea_item;

    sea_check_null_ptr_return(item);

    mpi_sea_check_inited();
    mpi_sea_lock();

    sea_item.h_mod = sea;
    sea_item.type = type;
    (td_void)memcpy_s(&sea_item.item, sizeof(sea_item.item), item, sizeof(*item));

    audio_api_time_consum_stat();

    ret = audio_ioctl(g_mpi_sea_fd, CMD_IOCTL_SEA_ADD_ITEM, &sea_item);
    if (ret != AUDIO_SUCCESS) {
        mpi_sea_unlock();
        return ret;
    }

    mpi_sea_unlock();
    return AUDIO_SUCCESS;
}

td_s32 mpi_sea_remove_item(td_handle sea, sea_item_type type, const sea_info_item *item)
{
    td_s32 ret;
    sap_sea_item_node sea_item;

    sea_check_null_ptr_return(item);

    mpi_sea_check_inited();
    mpi_sea_lock();

    sea_item.h_mod = sea;
    sea_item.type = type;
    (td_void)memcpy_s(&sea_item.item, sizeof(sea_item.item), item, sizeof(*item));

    audio_api_time_consum_stat();

    ret = audio_ioctl(g_mpi_sea_fd, CMD_IOCTL_SEA_REMOVE_ITEM, &sea_item);
    if (ret != AUDIO_SUCCESS) {
        mpi_sea_unlock();
        return ret;
    }

    mpi_sea_unlock();
    return AUDIO_SUCCESS;
}

td_s32 mpi_sea_enable_vid_enroll(td_handle sea, td_bool enable)
{
    td_s32 ret;
    sap_drv_enable sea_enable;

    mpi_sea_check_inited();
    mpi_sea_lock();

    sea_enable.h_mod = sea;
    sea_enable.enable = enable;

    audio_api_time_consum_stat();

    ret = audio_ioctl(g_mpi_sea_fd, CMD_IOCTL_SEA_ENABLE_VID_ENROLL, &sea_enable);
    if (ret != AUDIO_SUCCESS) {
        mpi_sea_unlock();
        return ret;
    }

    mpi_sea_unlock();
    return AUDIO_SUCCESS;
}

td_s32 mpi_sea_attach_output(td_handle sea, sea_output_type type, td_handle output)
{
    td_s32 ret;
    sap_drv_attach_para attach_para;

    mpi_sea_check_inited();
    mpi_sea_lock();

    attach_para.h_src = sea;
    attach_para.h_sink = output;
    attach_para.data_port_id = type;

    audio_api_time_consum_stat();

    ret = audio_ioctl(g_mpi_sea_fd, CMD_IOCTL_SEA_ATTACH_OUTPUT, &attach_para);
    if (ret != AUDIO_SUCCESS) {
        mpi_sea_unlock();
        return ret;
    }

    mpi_sea_unlock();
    return AUDIO_SUCCESS;
}

td_s32 mpi_sea_detach_output(td_handle sea, sea_output_type type, td_handle output)
{
    td_s32 ret;
    sap_drv_attach_para attach_para;

    mpi_sea_check_inited();
    mpi_sea_lock();

    attach_para.h_src = sea;
    attach_para.h_sink = output;
    attach_para.data_port_id = type;

    audio_api_time_consum_stat();

    ret = audio_ioctl(g_mpi_sea_fd, CMD_IOCTL_SEA_DETACH_OUTPUT, &attach_para);
    if (ret != AUDIO_SUCCESS) {
        mpi_sea_unlock();
        return ret;
    }

    mpi_sea_unlock();
    return AUDIO_SUCCESS;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
