/**
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved. \n
 *
 * Description: Provides hash driver \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-08-09, Create file. \n
 */
#ifdef __FREERTOS__
#include "xlt_memory.h"
#endif
#include "soc_osal.h"
#include "irmalloc.h"
#include "securec.h"
#include "common_def.h"
#include "errcode.h"
#include "hal_sha.h"
#include "systick.h"
#include "sha_porting.h"
#include "drv_hash.h"

#define HASH_MAX_BUFFER_WORDS_LEN   16
#define HASH_MAX_BUFFER_BYTES_LEN   128
#define HASH_MAX_WORDS_LEN          16

#define SHA256_BUFFER_WORDS_LEN     16
#define SHA512_BUFFER_WORDS_LEN     16
#define SHA256_BUFFER_BYTES_LEN     64
#define SHA512_BUFFER_BYTES_LEN     128
#define SHA256_HASH_WORDS_LEN       8
#define SHA512_HASH_WORDS_LEN       16

#define SHA_4BYTE                   4U
#define SHA_SHIFT_2                 2
#define SHA_SHIFT_3                 3U
#define SHA_SHIFT_6                 6
#define SHA_SHIFT_7                 7
#define SHA_SHIFT_8                 8

#define HASH_TIMEOUT_MS             1000UL

#define MAX_HASH_HANDLE_NUM         1
#define INVALID_HANDLE_INDEX        (-1)

#define SHA256_RESULT_LENGTH        32
#define SHA256_BLOCK_SIZE           64
#define SHA256_WORDS_LEN            8

#define SHA512_RESULT_LENGTH        64
#define SHA512_BLOCK_SIZE           128
#define SHA512_WORDS_LEN            16

#define HASH_ALIGN_8                8
#define INVALID_HANDLE              0xFFFFFFFF

#define padding_m(x, size) (((x) + (size) - 1) & (~((size) - 1)))

typedef struct {
#if SHA512_SUPPORT == YES
    union {
        uint64_t words[SHA512_BUFFER_WORDS_LEN];
        uint8_t bytes[SHA512_BUFFER_BYTES_LEN];
    } buffer;
    uint32_t hash[SHA512_HASH_WORDS_LEN];
#else
    union {
        uint32_t words[SHA256_BUFFER_WORDS_LEN];
        uint8_t bytes[SHA256_BUFFER_BYTES_LEN];
    } buffer;
    uint32_t hash[SHA256_HASH_WORDS_LEN];
#endif
    uint32_t total_len;
    uint32_t buffer_len;
    uint32_t block_len;
    uint32_t words_len;
    uint32_t result_len;
    uint32_t timeout_ms;
    drv_cipher_hash_type_t hash_type;
} hash_context_t __attribute__((aligned(8)));

typedef struct {
    hash_context_t ctx;
    drv_cipher_buffer_secure_t buf_sec;
} hash_handle_t;

static hash_handle_t *g_handles[MAX_HASH_HANDLE_NUM] = {NULL};
static uint8_t *g_align_handle = NULL;

static void sha_pre_process(void);
static void sha_post_process(void);
static errcode_t sha_init(void);
static void sha_deinit(void);
static bool sha_get_result(void);
static bool sha_cal_with_timeout(hash_context_t *sha_context);
static void sha_cal_start(hash_context_t *sha_context, uint32_t data_in_addr, uint32_t data_len, bool is_last_package);
static errcode_t sha_data_align_and_cal(hash_context_t *sha_context, uint32_t data_addr, uint32_t len);
static uint32_t sha_get_block_size_fix(uint32_t len, drv_cipher_hash_type_t sha_type);
static uint32_t sha_get_block_size_mod(uint32_t len, drv_cipher_hash_type_t sha_type);
static uint32_t sha_get_no_aligned_cal_times(uint32_t len, drv_cipher_hash_type_t sha_type);
static errcode_t sha_update(hash_handle_t *handle, const uint8_t *src_data, const uint32_t len);
static errcode_t sha_final(hash_handle_t *handle, uint8_t *result, uint8_t result_len);
static int32_t get_handle_index(const hash_handle_t *handle);
static errcode_t get_hash_handle(hash_handle_t **handle);
static errcode_t check_hash_type(drv_cipher_hash_type_t hash_type);
static errcode_t set_hash_type(hash_handle_t *handle, const drv_cipher_hash_attr_t *hash_attr);
static void set_hash_attrs(hash_handle_t *handle);
static errcode_t hash_config_handle(hash_handle_t *handle, const drv_cipher_hash_attr_t *hash_attr);
static errcode_t release_hash_handle(hash_handle_t *handle);
static void hash_destruct(hash_handle_t *handle);
static errcode_t check_handle_validity(const hash_handle_t *handle);
static errcode_t hash_update(hash_handle_t *handle, const uint8_t *src_buf, uint32_t len, const uint32_t timeout_ms);
static errcode_t hash_get_result(hash_handle_t *handle, uint8_t *out, uint32_t *out_len);
static errcode_t hash_init(hash_handle_t *handle);
static void hash_deinit(const hash_handle_t *handle);

typedef errcode_t (*hash_init_func_t)(void);
typedef void (*hash_deinit_func_t)(void);
typedef errcode_t (*hash_update_func_t)(hash_handle_t *handle, const uint8_t *src_data, uint32_t len);
typedef errcode_t (*hash_final_func_t)(hash_handle_t *handle, uint8_t *result, uint8_t result_len);

typedef struct {
    hash_init_func_t init;
    hash_deinit_func_t deinit;
    hash_update_func_t update;
    hash_final_func_t final;
} hash_func_t;

typedef struct {
    drv_cipher_hash_type_t hash_type;
    uint32_t block_len;
    uint32_t result_len;
    uint32_t words_len;
    hash_func_t hash_func;
} hash_properties_t;

static hash_properties_t g_hash_properties[] = {
    {DRV_CIPHER_HASH_TYPE_SHA256, SHA256_BLOCK_SIZE, SHA256_RESULT_LENGTH, SHA256_WORDS_LEN,
     {sha_init, sha_deinit, sha_update, sha_final}},
#if SHA512_SUPPORT == YES
    {DRV_CIPHER_HASH_TYPE_SHA512, SHA512_BLOCK_SIZE, SHA512_RESULT_LENGTH, SHA512_WORDS_LEN,
     {sha_init, sha_deinit, sha_update, sha_final}}
#endif
};

static const uint8_t g_sha256_def_iv_data[] = {
    0xE3, 0xB0, 0xC4, 0x42, 0x98, 0xFC, 0x1C, 0x14, \
    0x9A, 0xFB, 0xF4, 0xC8, 0x99, 0x6F, 0xB9, 0x24, \
    0x27, 0xAE, 0x41, 0xE4, 0x64, 0x9B, 0x93, 0x4C, \
    0xA4, 0x95, 0x99, 0x1B, 0x78, 0x52, 0xB8, 0x55  \
};

#if SHA512_SUPPORT == YES
static const uint8_t g_sha512_def_iv_data[] = {
    0xCF, 0x83, 0xE1, 0x35, 0x7E, 0xEF, 0xB8, 0xBD, \
    0xF1, 0x54, 0x28, 0x50, 0xD6, 0x6D, 0x80, 0x07, \
    0xD6, 0x20, 0xE4, 0x05, 0x0B, 0x57, 0x15, 0xDC, \
    0x83, 0xF4, 0xA9, 0x21, 0xD3, 0x6C, 0xE9, 0xCE, \
    0x47, 0xD0, 0xD1, 0x3C, 0x5D, 0x85, 0xF2, 0xB0, \
    0xFF, 0x83, 0x18, 0xD2, 0x87, 0x7E, 0xEC, 0x2F, \
    0x63, 0xB9, 0x31, 0xBD, 0x47, 0x41, 0x7A, 0x81, \
    0xA5, 0x38, 0x32, 0x7A, 0xF9, 0x27, 0xDA, 0x3E  \
};
#endif

errcode_t drv_hash_start(uintptr_t *hhash, const drv_cipher_hash_attr_t *hash_attr)
{
    if (hhash == NULL || hash_attr == NULL) {
        return ERRCODE_HASH_INVALID_PARAMETER;
    }

    errcode_t ret;
    hash_handle_t *handle = NULL;
    ret = get_hash_handle(&handle);
    if (ret != ERRCODE_SUCC) {
        return ret;
    }

    ret = hash_config_handle(handle, hash_attr);
    if (ret != ERRCODE_SUCC) {
        goto exit;
    }

    ret = hash_init(handle);
    if (ret != ERRCODE_SUCC) {
        goto exit;
    }

    *hhash = (uintptr_t)handle;

    return ERRCODE_SUCC;
exit:
    hash_destruct(handle);
    return ret;
}

errcode_t drv_hash_update(uintptr_t hhash, const drv_cipher_buf_attr_t *src_buf, const uint32_t len,
    const uint32_t timeout_ms)
{
    errcode_t ret;
    hash_handle_t *handle = (hash_handle_t*)hhash;

    ret = check_handle_validity(handle);
    if (ret != ERRCODE_SUCC) {
        goto exit;
    }

    if (src_buf == NULL || src_buf->address == NULL) {
        ret = ERRCODE_HASH_INVALID_PARAMETER;
        goto exit;
    }

    if (len == 0) {
        return ERRCODE_SUCC;
    }

    ret = hash_update(handle, src_buf->address, len, timeout_ms);
    if (ret != ERRCODE_SUCC) {
        goto exit;
    }

    return ERRCODE_SUCC;
exit:
    hash_destruct(handle);
    return ret;
}

errcode_t drv_hash_final(uintptr_t hhash, uint8_t *out, uint32_t *out_len)
{
    errcode_t ret;
    hash_handle_t *handle = (hash_handle_t*)(uintptr_t)hhash;

    ret = check_handle_validity(handle);
    if (ret != ERRCODE_SUCC) {
        goto exit;
    }

    if (out == NULL || out_len == NULL) {
        ret = ERRCODE_HASH_INVALID_PARAMETER;
        goto exit;
    }

    ret = hash_get_result(handle, out, out_len);
    if (ret != ERRCODE_SUCC) {
        goto exit;
    }

    hash_deinit(handle);

    (void)release_hash_handle(handle);

    return ERRCODE_SUCC;

exit:
    hash_destruct(handle);
    return ret;
}

static void sha_pre_process(void)
{
    sha_port_set_soft_reset(0x1);
    sha_port_set_clk_en(0x1);
    sha_set_endian(BIG_ENDIAN_32BIT);
}

static void sha_post_process(void)
{
    sha_port_set_soft_reset(0x0);
    sha_port_set_clk_en(0x0);
}

static errcode_t sha_init(void)
{
    errcode_t ret;
    sha_pre_process();
    sha_port_register_hal_funcs();

    hal_sha_funcs_t *hal_funcs = sha_port_get_funcs();
    if (hal_funcs == NULL) {
        return ERRCODE_HASH_INVALID_PARAMETER;
    }

    ret = hal_funcs->init(NULL);
    if (ret != ERRCODE_SUCC) {
        return ret;
    }

    return ERRCODE_SUCC;
}

static void sha_deinit(void)
{
    hal_sha_funcs_t *hal_funcs = sha_port_get_funcs();
    hal_funcs->deinit();
    sha_port_unregister_hal_funcs();
    sha_post_process();
}

static bool sha_get_result(void)
{
    hal_sha_funcs_t *hal_funcs = sha_port_get_funcs();
    return hal_funcs->finish();
}

static bool sha_cal_with_timeout(hash_context_t *sha_context)
{
    uint64_t start_time = uapi_systick_get_ms();
    while (sha_get_result() == 0) {
        if ((uapi_systick_get_ms() - start_time) >= sha_context->timeout_ms) {
            sha_context->buffer_len = 0;
            return false;
        }
    }
    return true;
}

static uint32_t sha_flash_addr_remap(uint32_t addr)
{
#if SHA_FLASH_XIP_ADDR_REMAP_SUPPORT == NO
    return addr;
#else
    return addr >= SHA_FLASH_XIP_ADDR_OFFSET ? (addr - SHA_FLASH_XIP_ADDR_OFFSET) : addr;
#endif
}

static void sha_cal_start(hash_context_t *sha_context, uint32_t data_in_addr, uint32_t data_len, bool is_last_package)
{
    sha_attr_t sha_attr;
    sha_attr.input_addr = data_in_addr;
    sha_attr.output_addr = (uint32_t)(uintptr_t)sha_context->hash;
    sha_attr.data_length = data_len;
    sha_attr.total_length = sha_context->total_len;
    sha_attr.sha_mode = (sha_mode_t)sha_context->hash_type;
    sha_attr.is_last_package = is_last_package;
    sha_attr.is_iv_exist = (sha_context->hash[0] != 0) ? true : false;

    hal_sha_funcs_t *hal_funcs = sha_port_get_funcs();
    hal_funcs->start(sha_attr);
}

static uint32_t sha_get_block_size_fix(uint32_t len, drv_cipher_hash_type_t sha_type)
{
    if (sha_type == DRV_CIPHER_HASH_TYPE_SHA512) {
        return (len >> SHA_SHIFT_7) << SHA_SHIFT_7;
    }

    return (len >> SHA_SHIFT_6) << SHA_SHIFT_6;
}

static uint32_t sha_get_block_size_mod(uint32_t len, drv_cipher_hash_type_t sha_type)
{
    if (sha_type == DRV_CIPHER_HASH_TYPE_SHA512) {
        return (len - ((len >> SHA_SHIFT_7) << SHA_SHIFT_7));
    }

    return (len - ((len >> SHA_SHIFT_6) << SHA_SHIFT_6));
}

static uint32_t sha_get_no_aligned_cal_times(uint32_t len, drv_cipher_hash_type_t sha_type)
{
    uint32_t cal_times = (len >> SHA_SHIFT_6);

    if (sha_type == DRV_CIPHER_HASH_TYPE_SHA512) {
        cal_times = (len >> SHA_SHIFT_7);
    }

    return cal_times;
}

static errcode_t sha_data_align_and_cal(hash_context_t *sha_context, uint32_t data_addr, uint32_t len)
{
#ifndef HASH_MEM_COPY
    if ((data_addr & 0x7) == 0) {
        sha_context->total_len += len;
        sha_port_clean_cache((uint32_t*)(uintptr_t)data_addr, len);
        sha_cal_start(sha_context, data_addr, len, false);
        if (!sha_cal_with_timeout(sha_context)) {
            return ERRCODE_HASH_TIMEOUT;
        }
        return ERRCODE_SUCC;
    }
#endif

    // data_addr is not 8bit aligned, we need copy to sha_context.buffer, then calculate
    errno_t ret;
    uint32_t cal_times = sha_get_no_aligned_cal_times(len, sha_context->hash_type);
    for (uint8_t i = 0; i < cal_times; i++) {
        ret = memcpy_s(&sha_context->buffer.bytes[0], sha_context->block_len,
                       (uint8_t*)(uintptr_t)(data_addr + (i * sha_context->block_len)),
                       sha_context->block_len);
        if (ret != EOK) {
            return ERRCODE_MEMCPY;
        }

        sha_context->total_len += sha_context->block_len;
        sha_cal_start(sha_context, (uintptr_t)sha_context->buffer.words, sha_context->block_len, false);
        if (!sha_cal_with_timeout(sha_context)) {
            return ERRCODE_HASH_TIMEOUT;
        }
    }
    return ERRCODE_SUCC;
}

static errcode_t sha_update(hash_handle_t *handle, const uint8_t *src_data, const uint32_t len)
{
    errcode_t ret;
    uint32_t update_len = len;
    const uint8_t *data = src_data;
    hash_context_t *sha_context = &handle->ctx;

    if (sha_context->buffer_len != 0) {
        uint32_t diff_len = sha_context->block_len - sha_context->buffer_len;
        uint32_t padding_len = (diff_len < update_len) ? diff_len : update_len;
        if (padding_len > 0) {
            if (memcpy_s(&sha_context->buffer.bytes[sha_context->buffer_len],
                         (sha_context->block_len - sha_context->buffer_len), data, padding_len) != EOK) {
                return ERRCODE_MEMCPY;
            }

            sha_context->total_len += padding_len;
            sha_context->buffer_len += padding_len;
            data += padding_len;
            update_len -= padding_len;
        }

        if ((sha_context->buffer_len == sha_context->block_len) && (update_len != 0)) {
            sha_cal_start(sha_context, (uintptr_t)sha_context->buffer.words, sha_context->block_len, false);
            if (!sha_cal_with_timeout(sha_context)) {
                return ERRCODE_HASH_TIMEOUT;
            }
            sha_context->buffer_len = 0;
        }
    }
    if (update_len > sha_context->block_len) {
        uint32_t cal_len = (sha_get_block_size_mod(update_len, sha_context->hash_type) == 0) ?
                           (len - sha_context->block_len) : sha_get_block_size_fix(update_len, sha_context->hash_type);
        ret = sha_data_align_and_cal(sha_context, sha_flash_addr_remap((uint32_t)(uintptr_t)data), cal_len);
        if (ret != ERRCODE_SUCC) {
            return ret;
        }
        data += cal_len;
        update_len -= cal_len;
    }

    if (update_len != 0) {
        if (memcpy_s(&sha_context->buffer.bytes[sha_context->buffer_len],
                     (sha_context->block_len - sha_context->buffer_len), data, update_len) != EOK) {
            return ERRCODE_MEMCPY;
        }
        sha_context->total_len += update_len;
        sha_context->buffer_len += update_len;
    }

    return ERRCODE_SUCC;
}

#if SHA_LAST_PACKAGE_IS_BIG_ENDIAN == NO
static uint32_t little2big(uint32_t number, uint8_t remaind)
{
    return number << ((uint8_t)(SHA_4BYTE - remaind) << SHA_SHIFT_3);
}
#endif

static void sha_4byte_align(hash_context_t *sha_context)
{
#if SHA_LAST_PACKAGE_IS_BIG_ENDIAN == NO
    if (sha_get_endian() == BIG_ENDIAN_32BIT) {
        uint8_t remaind = sha_context->buffer_len % SHA_4BYTE;
        if (remaind != 0) {
            uint32_t temp = sha_context->buffer.words[sha_context->buffer_len >> SHA_SHIFT_2];
            sha_context->buffer.words[sha_context->buffer_len >> SHA_SHIFT_2] = little2big(temp, remaind);
        }
    }
#else
    unused(sha_context);
#endif
}

static void sha_get_def_iv_data(hash_context_t *sha_context)
{
    if (sha_context->hash_type == DRV_CIPHER_HASH_TYPE_SHA256) {
        (void)memcpy_s((void *)(uintptr_t)sha_context->hash, sha_context->words_len * sizeof(uint32_t),
                       g_sha256_def_iv_data, sizeof(g_sha256_def_iv_data));
    }

#if SHA512_SUPPORT == YES
    if (sha_context->hash_type == DRV_CIPHER_HASH_TYPE_SHA512) {
        (void)memcpy_s((void *)(uintptr_t)sha_context->hash, sha_context->words_len * sizeof(uint32_t),
                       g_sha512_def_iv_data, sizeof(g_sha512_def_iv_data));
    }
#endif
}

static errcode_t sha_final(hash_handle_t *handle, uint8_t *result, uint8_t result_len)
{
    hash_context_t *sha_context = &handle->ctx;
    sha_4byte_align(sha_context);
    if (sha_context->buffer_len != 0) {
        sha_cal_start(sha_context, (uintptr_t)sha_context->buffer.words, sha_context->buffer_len, true);
        if (!sha_cal_with_timeout(sha_context)) {
            return ERRCODE_HASH_TIMEOUT;
        }
    }

    if (sha_context->total_len == 0) {
        sha_get_def_iv_data(sha_context);
    }

    if (memcpy_s(result, result_len, sha_context->hash, sha_context->words_len * sizeof(uint32_t)) != EOK) {
        return ERRCODE_MEMCPY;
    }

    return ERRCODE_SUCC;
}

static int32_t get_handle_index(const hash_handle_t *handle)
{
    int32_t i;
    for (i = 0; i < MAX_HASH_HANDLE_NUM; ++i) {
        if (handle == g_handles[i]) {
            return i;
        }
    }

    return INVALID_HANDLE_INDEX;
}

static errcode_t get_hash_handle(hash_handle_t **handle)
{
    int32_t index = 0;

    index = get_handle_index(NULL);
    if (index == INVALID_HANDLE_INDEX) {
        return ERRCODE_HASH_BUSY;
    }

#ifdef __FREERTOS__
    g_align_handle = (uint8_t*)osal_pool_mem_alloc(OS_SYS_MEM_POOL, sizeof(hash_handle_t) + HASH_ALIGN_8);
#else
    g_align_handle = (uint8_t*)irmalloc(sizeof(hash_handle_t) + HASH_ALIGN_8);
#endif
    if (g_align_handle == NULL) {
        return ERRCODE_HASH_FAILED_MEM;
    }

    if (memset_s(g_align_handle, sizeof(hash_handle_t) + HASH_ALIGN_8, 0,
        sizeof(hash_handle_t) + HASH_ALIGN_8) != EOK) {
#ifdef __FREERTOS__
        osal_pool_mem_free(OS_SYS_MEM_POOL, (void*)(g_align_handle));
#else
        irfree((void*)(g_align_handle));
#endif
        g_align_handle = NULL;
        return ERRCODE_MEMSET;
    }

    *handle = (hash_handle_t*)padding_m((uintptr_t)g_align_handle, HASH_ALIGN_8); // 8字节对齐
    g_handles[index] = *handle;
    return ERRCODE_SUCC;
}

static errcode_t check_hash_type(drv_cipher_hash_type_t hash_type)
{
    uint32_t i;
    for (i = 0; i < sizeof(g_hash_properties) / sizeof(hash_properties_t); ++i) {
        if (g_hash_properties[i].hash_type == hash_type) {
            return ERRCODE_SUCC;
        }
    }
    return ERRCODE_HASH_UNSUPPORTED;
}

static errcode_t set_hash_type(hash_handle_t *handle, const drv_cipher_hash_attr_t *hash_attr)
{
    errcode_t ret = check_hash_type(hash_attr->hash_type);
    if (ret != ERRCODE_SUCC) {
        return ret;
    }

    handle->ctx.hash_type = hash_attr->hash_type;
    return ERRCODE_SUCC;
}

static void set_hash_attrs(hash_handle_t *handle)
{
    uint32_t i;
    for (i = 0; i < sizeof(g_hash_properties) / sizeof(hash_properties_t); ++i) {
        if (handle->ctx.hash_type == g_hash_properties[i].hash_type) {
            handle->ctx.block_len = g_hash_properties[i].block_len;
            handle->ctx.words_len = g_hash_properties[i].words_len;
            handle->ctx.result_len = g_hash_properties[i].result_len;
            break;
        }
    }
}

static errcode_t hash_config_handle(hash_handle_t *handle, const drv_cipher_hash_attr_t *hash_attr)
{
    errcode_t ret = set_hash_type(handle, hash_attr);
    if (ret != ERRCODE_SUCC) {
        return ret;
    }

    set_hash_attrs(handle);

    return ERRCODE_SUCC;
}

static errcode_t release_hash_handle(hash_handle_t *handle)
{
    int32_t index = INVALID_HANDLE_INDEX;

    index = get_handle_index(handle);
    if (index == INVALID_HANDLE_INDEX) {
        return ERRCODE_HASH_INVALID_PARAMETER;
    }

#ifdef __FREERTOS__
    osal_pool_mem_free(OS_SYS_MEM_POOL, (void*)g_align_handle);
#else
    irfree((void*)g_align_handle);
#endif
    g_handles[index] = NULL;
    return ERRCODE_SUCC;
}

static void hash_destruct(hash_handle_t *handle)
{
    if (handle == NULL) {
        return;
    }

    (void)release_hash_handle(handle);
}

static errcode_t check_handle_validity(const hash_handle_t *handle)
{
    int32_t index = INVALID_HANDLE_INDEX;

    if (handle == NULL) {
        return ERRCODE_HASH_INVALID_PARAMETER;
    }

    index = get_handle_index(handle);
    if (index == INVALID_HANDLE_INDEX) {
        return ERRCODE_HASH_INVALID_PARAMETER;
    }

    return ERRCODE_SUCC;
}

static errcode_t hash_init(hash_handle_t *handle)
{
    errcode_t ret = ERRCODE_HASH_UNSUPPORTED;

    uint32_t i;
    for (i = 0; i < sizeof(g_hash_properties) / sizeof(hash_properties_t); ++i) {
        if (handle->ctx.hash_type == g_hash_properties[i].hash_type) {
            ret = g_hash_properties[i].hash_func.init();
            break;
        }
    }

    return ret;
}

static void hash_deinit(const hash_handle_t *handle)
{
    uint32_t i;
    for (i = 0; i < sizeof(g_hash_properties) / sizeof(hash_properties_t); ++i) {
        if (handle->ctx.hash_type == g_hash_properties[i].hash_type) {
            g_hash_properties[i].hash_func.deinit();
            break;
        }
    }
}

static errcode_t hash_update(hash_handle_t *handle, const uint8_t *src_buf, uint32_t len, const uint32_t timeout_ms)
{
    errcode_t ret = ERRCODE_HASH_UNSUPPORTED;
    handle->ctx.timeout_ms = (timeout_ms == 0) ? HASH_TIMEOUT_MS : timeout_ms;

    uint32_t i;
    for (i = 0; i < sizeof(g_hash_properties) / sizeof(hash_properties_t); ++i) {
        if (handle->ctx.hash_type == g_hash_properties[i].hash_type) {
            ret = g_hash_properties[i].hash_func.update(handle, src_buf, len);
            break;
        }
    }

    return ret;
}

static errcode_t hash_get_result(hash_handle_t *handle, uint8_t *out, uint32_t *out_len)
{
    if (handle->ctx.result_len > *out_len) {
        return ERRCODE_HASH_INVALID_PARAMETER;
    }

    *out_len = handle->ctx.result_len;

    uint32_t i;
    errcode_t ret = ERRCODE_HASH_UNSUPPORTED;
    for (i = 0; i < sizeof(g_hash_properties) / sizeof(hash_properties_t); ++i) {
        if (handle->ctx.hash_type == g_hash_properties[i].hash_type) {
            ret = g_hash_properties[i].hash_func.final(handle, out, (uint8_t)(*out_len));
            break;
        }
    }

    return ret;
}
