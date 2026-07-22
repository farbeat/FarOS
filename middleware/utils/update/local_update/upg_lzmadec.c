/*
 * Copyright (c) @CompanyNameMagicTag 2021-2021. All rights reserved.
 * Description: UPG lzma decode functions source file
 */

#include <stddef.h>
#include <stdint.h>
#include "securec.h"
#include "common_def.h"
#include "upg_alloc.h"
#include "upg_porting.h"
#include "upg_common.h"
#include "upg_debug.h"
#if (UPG_CFG_SUPPORT_RESOURCES_FILE == YES)
#include "dfx_file_operation.h"
#endif
#include "upg_lzmadec.h"

STATIC void *upg_lzma_alloc(const ISzAllocPtr unused, size_t size)
{
    unused(unused);
    void *p;
    p = upg_malloc((uint32_t)size);
    if (p == NULL) {
        upg_msg1("upg_lzma_alloc failed!!! size = ", size);
        return NULL;
    }
    return p;
}

STATIC void upg_lzma_free(const ISzAllocPtr unused, void *address)
{
    unused(unused);
    if (address == NULL) {
        return;
    }
    upg_free(address);
}

static inline void lzmadec_init(CLzmaDec *p)
{
    return LzmaDec_Init(p);
}

static inline int32_t lzmadec_decodetodic(CLzmaDec *p, SizeT dic_limit, const Byte *src, SizeT *srclen,
    ELzmaFinishMode finish_mode, ELzmaStatus *status)
{
    return LzmaDec_DecodeToDic(p, dic_limit, src, srclen, finish_mode, status);
}

static inline void lzmadec_freeprobs(CLzmaDec *p, ISzAllocPtr alloc)
{
    return LzmaDec_FreeProbs(p, alloc);
}

static inline int32_t lzmadec_allocateprobs(CLzmaDec *p, const Byte *props, unsigned props_size, ISzAllocPtr alloc)
{
    return LzmaDec_AllocateProbs(p, props, props_size, alloc);
}

STATIC SRes lzmadec_decodetobuf(CLzmaDec *p, Byte *dest, SizeT *dest_len, const Byte *src, SizeT *src_len,
                                ELzmaFinishMode finish_mode, ELzmaStatus *status)
{
    return LzmaDec_DecodeToBuf(p, dest, dest_len, src, src_len, finish_mode, status);
}

uint32_t upg_lzma_init(CLzmaDec *p, upg_lzma_decode2_data_t *val_data, const Byte *props, uint32_t props_len)
{
    LzmaDec_Construct(p);

    ISzAlloc alloc;
    alloc.Alloc = upg_lzma_alloc;
    alloc.Free = upg_lzma_free;

    if (props_len < LZMA_PROPS_SIZE) {
        return ERRCODE_UPG_INVALID_PARAMETER;
    }

    int32_t res = lzmadec_allocateprobs(p, props, LZMA_PROPS_SIZE, &alloc);
    if (res != 0) {
        return (uint32_t)res;
    }

    uint32_t unpack_size = 0;
    for (uint32_t i = 0; i < props_len - LZMA_PROPS_SIZE; i++) {
        unpack_size += (uint32_t)props[LZMA_PROPS_SIZE + i] << (i * 8); /* 8: 用于计算偏移位数 */
    }
    uint32_t dic_size = unpack_size;

    if ((p->prop).dicSize <= unpack_size) {
        dic_size = (p->prop).dicSize;
    }

    if (dic_size == 0) {
        return ERRCODE_UPG_INVALID_PARAMETER;
    }

    p->dic = upg_lzma_alloc(&alloc, dic_size);
    if (p->dic == NULL) {
        return ERRCODE_MALLOC;
    }

    p->dicBufSize = dic_size;

    val_data->inbuf = upg_lzma_alloc(&alloc, IN_BUF_SIZE);
    if (val_data->inbuf == NULL) {
        upg_lzma_free(&alloc, p->dic);
        p->dic = NULL;
        return ERRCODE_MALLOC;
    }

    val_data->outbuf = upg_lzma_alloc(&alloc, OUT_BUF_SIZE);
    if (val_data->outbuf == NULL) {
        upg_lzma_free(&alloc, p->dic);
        p->dic = NULL;
        upg_lzma_free(&alloc, val_data->inbuf);
        val_data->inbuf = NULL;
        return ERRCODE_MALLOC;
    }
    val_data->decompress_len = unpack_size;

    lzmadec_init(p);
    return ERRCODE_SUCC;
}

void upg_lzma_deinit(CLzmaDec *p, upg_lzma_decode2_data_t *val_data)
{
    ISzAlloc alloc;
    alloc.Alloc = upg_lzma_alloc;
    alloc.Free = upg_lzma_free;

    upg_lzma_free(&alloc, p->dic);
    p->dic = NULL;

    lzmadec_freeprobs(p, &alloc);

    upg_lzma_free(&alloc, val_data->inbuf);
    val_data->inbuf = NULL;

    upg_lzma_free(&alloc, val_data->outbuf);
    val_data->outbuf = NULL;
}

STATIC bool upg_lzma_dec_check_ret(SRes res, uint32_t unpack_size, bool in_out_zero, ELzmaStatus status, SRes *ret)
{
    if ((res != SZ_OK) || (unpack_size == 0)) {
        *ret = res;
        return true;
    }

    if (in_out_zero) {
        if (status != LZMA_STATUS_FINISHED_WITH_MARK) {
            *ret = SZ_ERROR_DATA;
            return true;
        }
        *ret = res;
        return true;
    }
    return false;
}

uint32_t upg_lzma_decode(CLzmaDec *p, upg_lzma_decode2_data_t *data)
{
    size_t in_pos = 0;
    size_t in_size = 0;
    uint32_t compress_len = data->compress_len; /* 压缩包的总长度，即需要解压缩的总长度 */
    uint32_t unpack_size = data->decompress_len;
    uint32_t in_offset = data->in_offset; /* 已解压数据的压缩包的偏移位置 */
    uint32_t out_offset = data->out_offset; /* 已解压数据解压后的总长度, 用作写入的偏移位置 */
    ELzmaStatus status  = LZMA_STATUS_NOT_SPECIFIED;

    for (;;) {
        if (in_pos == in_size) {
            in_size = (compress_len > IN_BUF_SIZE) ? IN_BUF_SIZE : compress_len;
            if (upg_read_fota_pkg_data(in_offset, data->inbuf, (uint32_t *)(uintptr_t)&in_size) != ERRCODE_SUCC) {
                return SZ_ERROR_DATA;
            }
            compress_len -= in_size;
            in_pos = 0;
        }

        SizeT in_processed = in_size - in_pos;
        SizeT out_processed = OUT_BUF_SIZE;

        ELzmaFinishMode finish_mode = LZMA_FINISH_ANY;
        if (out_processed > unpack_size) {
            out_processed = (SizeT)unpack_size;
            finish_mode = LZMA_FINISH_END;
        }

        SRes res = lzmadec_decodetobuf(p, data->outbuf, &out_processed, data->inbuf + in_pos,
            &in_processed, finish_mode, &status);

        in_pos += in_processed;
        unpack_size -= out_processed;

        errcode_t ret = upg_write_new_image_data(out_offset, data->outbuf, (uint32_t *)(uintptr_t)&out_processed,
            data->image_id);
        if (ret != ERRCODE_SUCC) {
            return SZ_ERROR_DATA;
        }

        upg_calculate_and_notify_process((uint32_t)out_processed);

        in_offset += in_processed;
        out_offset += out_processed;

        // 踢狗
        upg_watchdog_kick();

        SRes res_ret;
        if (upg_lzma_dec_check_ret(res, unpack_size, (in_processed == 0 && out_processed == 0), status, &res_ret)) {
            return (uint32_t)res_ret;
        }
    }
}

#if (UPG_CFG_SUPPORT_RESOURCES_FILE == YES)
errcode_t upg_resource_file_decode(CLzmaDec *p, upg_lzma_decode2_data_t *data, upg_resource_node_t *file_info)
{
    errcode_t ret = ERRCODE_SUCC;
    size_t in_size = 0; /* 待读出压缩数据的大小 */
    size_t out_len = 0; /* 解压缩后数据的大小 */
    uint32_t file_len = file_info->file_len; /* 文件的大小，即本次需要解压缩出来的原始数据长度 */
    uint32_t in_offset = data->in_offset; /* 已解压数据的压缩包的偏移位置 */
    uint32_t file_offset = 0; /* 本次解压出的数据长度, 用作写入的偏移位置 */
    ELzmaStatus status  = LZMA_STATUS_NOT_SPECIFIED;

    if (file_info->offset != data->out_offset) {
        return ERRCODE_UPG_INVALID_OFFSET;
    }

    int32_t write_fd = dfx_file_open_for_write(file_info->file_path);
    if (write_fd < 0) {
        return ERRCODE_UPG_FILE_OPEN_FAIL;
    }

    while (file_len > 0) {
        in_size = (data->compress_len > IN_BUF_SIZE) ? IN_BUF_SIZE : data->compress_len;
        ret = upg_read_fota_pkg_data(in_offset, data->inbuf, (uint32_t *)(uintptr_t)&in_size);
        if (ret != ERRCODE_SUCC) {
            break;
        }

        out_len = (file_len > OUT_BUF_SIZE) ? OUT_BUF_SIZE : file_len;
        if (lzmadec_decodetobuf(p, data->outbuf, &out_len, data->inbuf, &in_size, LZMA_FINISH_ANY, &status) != SZ_OK) {
            ret = ERRCODE_UPG_DECOMPRESS_FAIL;
            break;
        }

        if (dfx_file_write_fd(write_fd, file_offset, data->outbuf, out_len) != (int32_t)out_len) {
            ret = ERRCODE_UPG_FILE_WRITE_FAIL;
            break;
        }

        in_offset += in_size;
        data->compress_len -= in_size;
        file_offset += out_len;
        file_len -= out_len;

        upg_calculate_and_notify_process((uint32_t)out_len);

        if (data->compress_len == 0 || (in_size == 0 && out_len == 0)) {
            break;
        }
    }

    data->in_offset = in_offset;
    data->out_offset += file_offset;
    if (in_size == 0 && out_len == 0 && status != LZMA_STATUS_FINISHED_WITH_MARK && file_info->file_len != 0) {
        ret = ERRCODE_UPG_DECOMPRESS_FAIL;
    }

    dfx_file_fsync(write_fd);
    dfx_file_close(write_fd);
    return ret;
}
#else
errcode_t upg_resource_file_decode(CLzmaDec *p, upg_lzma_decode2_data_t *data, upg_resource_node_t *file_info)
{
    unused(p);
    unused(data);
    unused(file_info);
    return ERRCODE_SUCC;
}
#endif