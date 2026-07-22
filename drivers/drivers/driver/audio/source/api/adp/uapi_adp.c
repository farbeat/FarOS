/*
 * Copyright (c) CompanyNameMagicTag 2020-2021. All rights reserved.
 * Description: adp user api
 * Author: Audio
 */

#include "soc_uapi_adp.h"
#include "mpi_adp.h"
#include "audio_type.h"
#include "adp_struct.h"
#include "core_module.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

td_s32 uapi_adp_init(td_void)
{
    return mpi_adp_init();
}

td_s32 uapi_adp_deinit(td_void)
{
    return mpi_adp_deinit();
}

td_s32 uapi_adp_get_def_attr(uapi_adp_attr *attr)
{
    if (attr == TD_NULL) {
        return ERR_ADP_NULL_PTR;
    }

    attr->buf_size = 0;
    return AUDIO_SUCCESS;
}

td_s32 uapi_adp_create(td_handle *adp, const uapi_adp_attr *attr)
{
    audio_adp_attr adp_attr;

    if (adp == TD_NULL || attr == TD_NULL) {
        return ERR_ADP_NULL_PTR;
    }

    adp_attr.rbuf_size = attr->buf_size;
    adp_attr.tail_size = 0;
    adp_attr.info_buf_size = 0;
    return mpi_adp_open(adp, ADP_TYPE_MAX, &adp_attr);
}

td_s32 uapi_adp_destroy(td_handle adp)
{
    return mpi_adp_close(adp);
}

td_s32 uapi_adp_map(td_handle adp)
{
    return mpi_adp_map(adp);
}

td_s32 uapi_adp_unmap(td_handle adp)
{
    return mpi_adp_unmap(adp);
}

td_s32 uapi_adp_get_attr(td_handle adp, uapi_adp_attr *attr)
{
    td_s32 ret;
    audio_adp_attr adp_attr;

    if (adp == 0) {
        return ERR_ADP_INVALID_HANDLE;
    }

    if (attr == TD_NULL) {
        return ERR_ADP_NULL_PTR;
    }

    ret = mpi_adp_get_attr(adp, &adp_attr);
    if (ret != AUDIO_SUCCESS) {
        return ret;
    }

    attr->buf_size = adp_attr.rbuf_size;
    return AUDIO_SUCCESS;
}

td_s32 uapi_adp_set_attr(td_handle adp, const uapi_adp_attr *attr)
{
    audio_adp_attr adp_attr;

    if (adp == 0) {
        return ERR_ADP_INVALID_HANDLE;
    }

    if (attr == TD_NULL) {
        return ERR_ADP_NULL_PTR;
    }

    adp_attr.rbuf_size = attr->buf_size;
    adp_attr.tail_size = 0;
    adp_attr.info_buf_size = 0;
    return mpi_adp_set_attr(adp, &adp_attr);
}

td_s32 uapi_adp_attach_output(td_handle adp, td_handle output)
{
    td_s32 ret;

    if (adp == 0 || output == 0) {
        return ERR_ADP_INVALID_HANDLE;
    }

    ret = mpi_adp_alloc_input_shrb(adp, output, 0);
    if (ret != AUDIO_SUCCESS) {
        return ret;
    }

    ret = mpi_adp_attach_output(adp, output);
    if (ret != AUDIO_SUCCESS) {
        mpi_adp_free_shrb(adp);
        return ret;
    }

    return AUDIO_SUCCESS;
}

td_s32 uapi_adp_detach_output(td_handle adp, td_handle output)
{
    td_s32 ret;

    if (adp == 0 || output == 0) {
        return ERR_ADP_INVALID_HANDLE;
    }

    if (get_sap_mod_type(adp) != SAP_MOD_ADP) {
        return ERR_ADP_INVALID_HANDLE;
    }

    ret = mpi_adp_detach_output(adp, output);
    if (ret != AUDIO_SUCCESS) {
        return ret;
    }

    ret = mpi_adp_free_shrb(adp);
    if (ret != AUDIO_SUCCESS) {
        return ret;
    }

    return AUDIO_SUCCESS;
}

td_s32 uapi_adp_register_event_proc(td_handle adp, uapi_adp_event_proc proc, td_void *context)
{
    audio_event_proc adp_proc;

    if (adp == 0) {
        return ERR_ADP_INVALID_HANDLE;
    }

    if (proc == TD_NULL) {
        return ERR_ADP_NULL_PTR;
    }

    adp_proc = (audio_event_proc)(td_uintptr_t)proc;
    return mpi_adp_register_event_proc(adp, adp_proc, context);
}

td_s32 uapi_adp_query_free(td_handle adp, td_u32 *free_size)
{
    return mpi_adp_query_free(adp, free_size);
}

td_s32 uapi_adp_query_busy(td_handle adp, td_u32 *busy_size)
{
    return mpi_adp_query_busy(adp, busy_size);
}

td_s32 uapi_adp_get_buffer(td_handle adp, uapi_stream_buf *stream)
{
    return mpi_adp_get_buffer(adp, (audio_stream_buf *)(td_void *)stream);
}

td_s32 uapi_adp_put_buffer(td_handle adp, const uapi_stream_buf *stream)
{
    return mpi_adp_put_buffer(adp, (const audio_stream_buf *)(td_void *)stream);
}

td_s32 uapi_adp_send_stream(td_handle adp, const uapi_stream_buf *stream)
{
    return mpi_adp_send_stream(adp, (const audio_stream_buf *)(td_void *)stream);
}

td_s32 uapi_adp_acquire_stream(td_handle adp, uapi_stream_buf *stream)
{
    return mpi_adp_acquire_stream(adp, (audio_stream_buf *)(td_void *)stream);
}

td_s32 uapi_adp_release_stream(td_handle adp, const uapi_stream_buf *stream)
{
    return mpi_adp_release_stream(adp, (const audio_stream_buf *)(td_void *)stream);
}

td_s32 uapi_adp_send_frame(td_handle adp, const uapi_audio_frame *frame)
{
    return mpi_adp_send_frame(adp, (const audio_frame *)(td_void *)frame);
}

td_s32 uapi_adp_acquire_frame(td_handle adp, uapi_audio_frame *frame)
{
    return mpi_adp_acquire_frame(adp, (audio_frame *)(td_void *)frame);
}

td_s32 uapi_adp_release_frame(td_handle adp, const uapi_audio_frame *frame)
{
    return mpi_adp_release_frame(adp, (const audio_frame *)(td_void *)frame);
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
