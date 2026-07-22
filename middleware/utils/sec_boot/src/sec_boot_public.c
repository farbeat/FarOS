/*
 * Copyright (c) CompanyNameMagicTag 2020-2022. All rights reserved.
 * Description:  ROM SEC BOOT MODULE
 * Author:
 * Create: 2020-11-26
 */
#include "sec_boot_public.h"
#include "errcode.h"
#include "sha256.h"
#if defined(SUPPORT_SEC_TRNG) && SUPPORT_SEC_TRNG == YES
#include "trng.h"
#endif
#ifdef EFUSE_INTERFACE_REPLACE
#include "efuse.h"
#else
#include "otp.h"
#endif
#include "debug_print.h"
#include "securec.h"
#if SEC_BOOT_SIGN_CHECK_EN == YES
#include "flash.h"
#include "non_os.h"
#include "pke.h"
#ifdef EFUSE_INTERFACE_REPLACE
#include "efuse.h"
#else
#include "otp.h"
#endif
#endif
#include "chip_io.h"

#define SEC_BOOT_SHIFT_24      24
#define SEC_BOOT_SHIFT_16      16
#define SEC_BOOT_SHIFT_8       8
#define WORD_SIZE              4
#define RSA_PSS_MAGIC_FLAG     0xBC
#define RSA_PSS_PADDING_LEN    8
#define CRC_RIGHT_SHIFT_BIT    24
#define CRC_MASK               0xff000000
#ifdef EFUSE_INTERFACE_REPLACE
#define PAL_OTP_SEC_BOOT_DEBUG_FLAG_LEN 1
#define PAL_OTP_BOOTLOADER_BYTE_LEN     1
#define PAL_OTP_MCU_ROOT_PUBKEY_SHA256_CRC_LEN 1
#endif

#if SEC_BOOT_SIGN_CHECK_EN == YES
static uint8_t g_rsa_pubkey[RSA4096_PUBKEY_LENGTH] __attribute__((section(".rsa_public_key_mem")));
static uint8_t g_rsa_sign[RSA4096_SIGN_LENGTH] __attribute__((section(".rsa_signature_mem")));
static uint8_t g_rsa_result[RSA4096_SIGN_LENGTH] __attribute__((section(".rsa_result_mem")));

uint32_t sec_boot_global_rsa_pubkey_addr_get(void)
{
    return (uint32_t)g_rsa_pubkey;
}

uint32_t sec_boot_global_rsa_sign_addr_get(void)
{
    return (uint32_t)g_rsa_sign;
}

uint32_t sec_boot_global_rsa_result_addr_get(void)
{
    return (uint32_t)g_rsa_result;
}

static void i2osp(uint32_t data, uint8_t* byte, uint8_t len)
{
    uint8_t i = 0;
    if (len == sizeof(uint32_t)) {
        byte[i++] = (uint8_t)(data >> SEC_BOOT_SHIFT_24);
        byte[i++] = (uint8_t)(data >> SEC_BOOT_SHIFT_16);
        byte[i++] = (uint8_t)(data >> SEC_BOOT_SHIFT_8);
        byte[i++] = (uint8_t)(data >> 0);
    }
}

static void sec_boot_rsa_pss_mgf1(uint8_t *dst, uint32_t dst_len, uint8_t *src, uint32_t src_len)
{
    errcode_t ret;
    uint32_t out_len = SHA256_HASH_SIZE;
    uintptr_t handle;
    uint32_t counter = 0;
    uint8_t byte[WORD_SIZE] = { 0 };
    uint8_t sha_result[SHA256_HASH_SIZE] = {0};

    while ((counter * SHA256_HASH_SIZE) < dst_len) {
        i2osp(counter, byte, sizeof(byte));

        ret = uapi_drv_cipher_sha256_start(&handle);
        if (ret != ERRCODE_SUCC) {
            return;
        }

        ret = uapi_drv_cipher_sha256_update(handle, src, src_len);
        if (ret != ERRCODE_SUCC) {
            return;
        }

        ret = uapi_drv_cipher_sha256_update(handle, byte, sizeof(byte));
        if (ret != ERRCODE_SUCC) {
            return;
        }

        ret = uapi_drv_cipher_sha256_finish(handle, sha_result, &out_len);
        if (ret != ERRCODE_SUCC) {
            return;
        }

        uint32_t dst_index = counter * SHA256_HASH_SIZE;
        for (uint32_t i = 0; (i < SHA256_HASH_SIZE) && (dst_index < dst_len); i++) {
            dst[dst_index] ^= sha_result[i];
            dst_index++;
        }
        counter++;
    }
}

bool sec_boot_rsa_exp_mod(uint32_t key_addr, uint8_t *rsa_input, uint8_t *rsa_result)
{
    errcode_t ret;

    uapi_drv_cipher_pke_data_t n = {
        .data = (uint8_t*)(uintptr_t)key_addr,
        .length = UAPI_DRV_CIPHER_PKE_LEN_4096
    };

    uapi_drv_cipher_pke_data_t k = {
        .data = ((uint8_t*)(uintptr_t)key_addr + UAPI_DRV_CIPHER_PKE_LEN_4096),
        .length = UAPI_DRV_CIPHER_PKE_LEN_4096
    };

    uapi_drv_cipher_pke_data_t in = {
        .data = rsa_input,
        .length = UAPI_DRV_CIPHER_PKE_LEN_4096
    };

    uapi_drv_cipher_pke_data_t out = {
        .data = rsa_result,
        .length = UAPI_DRV_CIPHER_PKE_LEN_4096
    };

    ret = uapi_drv_cipher_pke_exp_mod(&n, &k, &in, &out);
    if (ret != ERRCODE_SUCC) {
        return false;
    }

    return true;
}


/* ******************** RSA_PSS VERTIFY ************************************** RSA_PSS SIGN ******************
 *                           +-----------+            *                           +-----------+              *
 *                           |     M     |            *                           |     M     |              *
 *                           +-----------+            *                           +-----------+              *
 *                                 |                  *                                 |                    *
 *                                 V                  *                                 V                    *
 *                               Hash                 *                               Hash                   *
 *                                 |                  *                                 |                    *
 *                                 V                  *                                 V                    *
 *                   +--------+----------+----------+ *                   +--------+----------+----------+   *
 *              M' = |Padding1|  mHash   |   salt   | *              M' = |Padding1|  mHash   |   salt   |   *
 *                   +--------+----------+----------+ *                   +--------+----------+----------+   *
 *                                 |                  *                                  |                   *
 *        +--------+----------+    V                  *        +--------+----------+     V                   *
 *  DB =  |Padding2|   salt   |  Hash                 *  DB =  |Padding2|   salt   |   Hash                  *
 *        +--------+----------+    ^                  *        +--------+----------+     |                   *
 *                  ^              |                  *                  |               |                   *
 *                  |              |                  *                  V               |                   *
 *                 xor <--- MGF <--|                  *                 xor <--- MGF <---|                   *
 *                  ^              |                  *                  |               |                   *
 *                  |              |                  *                  |               |                   *
 *                  |              |                  *                  V               V                   *
 *        +-------------------+----------+--+         *        +-------------------+----------+--+           *
 *  EM =  |    maskedDB       |     H    |bc|         *  EM =  |    maskedDB       |     H    |bc|           *
 *        +-------------------+----------+--+         *        +-------------------+----------+--+           *
 ************************************************************************************************************/
bool sec_boot_rsa_pss_verify(uint32_t salt_length, uint8_t *text, uint32_t text_len)
{
    // pay attention: the caller guarantees that the pubkey and sign text have been copied to global rsa addr
    // 1. based on signature, get EM
    uint32_t sign_addr = sec_boot_global_rsa_sign_addr_get();
    uint32_t pubkey_addr = sec_boot_global_rsa_pubkey_addr_get();
    uint8_t* rsa_result = (uint8_t *)sec_boot_global_rsa_result_addr_get();

    if (!sec_boot_rsa_exp_mod(pubkey_addr, (uint8_t*)(uintptr_t)sign_addr, rsa_result)) {
        PRINT("rsa exp mod fail!"NEWLINE);
        return false;
    }

    // 2. EM = maskDB || M' || 0xBC
    if (rsa_result[RSA4096_SIGN_LENGTH - 1] != RSA_PSS_MAGIC_FLAG) {
        PRINT("0xBC check error!!!"NEWLINE);
        return false;
    }
    uint8_t* maskdb = rsa_result;
    uint32_t maskdb_len = (RSA4096_SIGN_LENGTH - 1 - SHA256_HASH_SIZE);
    uint8_t* map = (uint8_t *)&rsa_result[maskdb_len];
    uint32_t map_len = SHA256_HASH_SIZE;

    // 3. get dbMask by mgf(M’)， and do xor with maskdb
    sec_boot_rsa_pss_mgf1(maskdb, maskdb_len, map, map_len);

    // 4. recovery salt in encoded buffer
    uint8_t* salt = maskdb + maskdb_len - salt_length;

    // 5. caculate mhash based on m, Padding1 length is 8
    errcode_t ret;
    uint8_t m_hash[(SHA256_HASH_SIZE << 1) + RSA_PSS_PADDING_LEN] = {0};

    ret = uapi_drv_cipher_sha256((uint8_t*)text, text_len, (uint8_t*)(&m_hash[RSA_PSS_PADDING_LEN]), SHA256_HASH_SIZE);
    if (ret != ERRCODE_SUCC) {
        return false;
    }

    errno_t sec_ret = memcpy_s((uint8_t *)(&m_hash[RSA_PSS_PADDING_LEN + SHA256_HASH_SIZE]),
                               SHA256_HASH_SIZE, salt, salt_length);
    if (sec_ret != EOK) {
        PRINT("memcpy error!!!"NEWLINE);
        return false;
    }

    uint8_t selfmap[SHA256_HASH_SIZE] = {0};
    ret = uapi_drv_cipher_sha256(m_hash, sizeof(m_hash), selfmap, sizeof(selfmap));
    if (ret != ERRCODE_SUCC) {
        return false;
    }

    // 6. compare selfmap and map
    if (memcmp(selfmap, map, SHA256_HASH_SIZE) != 0) {
        PRINT("rsa pss verify fail!"NEWLINE);
        return false;
    }
    PRINT("rsa pss verify success!"NEWLINE);
    return true;
}

#if (defined SUPPORT_SEC_TRNG) && (defined SEC_TRNG_ENABLE) && (SEC_TRNG_ENABLE == YES)
uint8_t *sec_boot_rsa_pss_sign(uint8_t *text, uint32_t text_len)
{
    if (text == NULL) {
        PRINT("ptr is NULL!"NEWLINE);
        return NULL;
    }

    uint8_t *em = (uint8_t *)sec_boot_global_rsa_sign_addr_get();
    memset_s(em, RSA4096_SIGN_LENGTH, 0, RSA4096_SIGN_LENGTH);

    // 1. caculate m'(m_hash) based on m, Padding1 length is 8, salt generator by trng, salt length is 32.
    uint8_t m_hash[(SHA256_HASH_SIZE << 1) + RSA_PSS_PADDING_LEN] = {0};

    errcode_t ret = uapi_drv_cipher_sha256(text, text_len, (uint8_t*)(&m_hash[RSA_PSS_PADDING_LEN]), SHA256_HASH_SIZE);
    if (ret != ERRCODE_SUCC) {
        return false;
    }

    ret = uapi_drv_cipher_trng_get_random_bytes(m_hash[RSA_PSS_PADDING_LEN + SHA256_HASH_SIZE], SHA256_HASH_SIZE);
    if (ret != ERRCODE_SUCC) {
        PRINT("get trng random fail!"NEWLINE);
        return false;
    }

    // 2. caculate H.
    uint8_t h[SHA256_HASH_SIZE] = {0};
    ret = uapi_drv_cipher_sha256(text, text_len, &m_hash[RSA_PSS_PADDING_LEN], SHA256_HASH_SIZE);
    if (ret != ERRCODE_SUCC) {
        return false;
    }

    errno_t sec_ret = memcpy_s(&em[RSA4096_SIGN_LENGTH - 1 - SHA256_HASH_SIZE], SHA256_HASH_SIZE, h, SHA256_HASH_SIZE);
    if (sec_ret != EOK) {
        PRINT("memcpy error!!!"NEWLINE);
        return NULL;
    }

    // 3. EM = maskDB || M' || 0xBC
    em[RSA4096_SIGN_LENGTH - 1] = RSA_PSS_MAGIC_FLAG;

    // 4. get DB = Padding2 | salt
    uint8_t* db = em;
    uint32_t db_len = (RSA4096_SIGN_LENGTH - 1 - SHA256_HASH_SIZE);
    sec_ret = memcpy_s((uint8_t *)(&em[db_len - SHA256_HASH_SIZE]), SHA256_HASH_SIZE,
                       (uint8_t *)(&m_hash[RSA_PSS_PADDING_LEN + SHA256_HASH_SIZE]), SHA256_HASH_SIZE);
    if (sec_ret != EOK) {
        PRINT("memcpy error!!!"NEWLINE);
        return NULL;
    }

    // 5. get dbMask by mgf(H)， and do xor with db
    uint8_t* map = (uint8_t *)&em[db_len];
    uint32_t map_len = SHA256_HASH_SIZE;
    sec_boot_rsa_pss_mgf1(db, db_len, map, map_len);

    // 6. rsa4096 caculate
    uint32_t private_addr = sec_boot_global_rsa_pubkey_addr_get();
    uint32_t rsa_result = sec_boot_global_rsa_result_addr_get();
    if (!sec_boot_rsa_exp_mod(private_addr, em, (uint8_t*)(uintpr_t)rsa_result)) {
        PRINT("rsa exp mod fail!"NEWLINE);
        return NULL;
    }

    return (uint8_t *)(uintptr_t)rsa_result;
}
#endif

/*
 * 1. Check that the rootpubkey sha256 CRC matches the rootpubkey sha256. Not set will always fail.
 * 2. Check that the rootpubkey sha256 matches the rootpubkey.
 */
bool sec_boot_root_pubkey_sha256_validate(uint32_t root_pubkey_addr)
{
    // check rootpubkey sha256 in efuse is validate or not
    uint8_t root_pubkey_sha256[PAL_OTP_MCU_ROOT_PUBKEY_SHA256_LENGTH] = { 0 };
    if (!sec_boot_rootkey_sha256_get(root_pubkey_sha256, PAL_OTP_MCU_ROOT_PUBKEY_SHA256_LENGTH)) {
        PRINT("public key otp crc check failed!!!"NEWLINE);
        return false;
    }

    if (!non_os_is_aon_flag_initialised(AON_XIP) && is_xip_address(root_pubkey_addr)) {
        uapi_flash_read_data(CHIP_FLASH_ID, (root_pubkey_addr - FLASH_START),
                             (uint8_t *)(&g_rsa_pubkey), RSA4096_PUBKEY_LENGTH);
    } else {
        errno_t sec_ret = memcpy_s(g_rsa_pubkey, RSA4096_PUBKEY_LENGTH,
                                   (uint8_t *)(uintptr_t)root_pubkey_addr, RSA4096_PUBKEY_LENGTH);
        if (sec_ret != EOK) { return false; }
    }

    // calculate public key sha256 of fota package and compare
    uint8_t sha_result[SHA256_HASH_SIZE] = { 0 };

    errcode_t ret = uapi_drv_cipher_sha256(g_rsa_pubkey, RSA4096_PUBKEY_LENGTH, sha_result, sizeof(sha_result));
    if (ret != ERRCODE_SUCC) {
        return false;
    }

    // memcmp return 0 if str1 equals to str2
    if (memcmp(root_pubkey_sha256, sha_result, SHA256_HASH_SIZE) != 0) {
        PRINT("public key sha256 check fail!"NEWLINE);
        return false;
    }

    PRINT("root public key sha256 check success!"NEWLINE);
    return true;
}
#endif

sec_boot_mode_t sec_boot_mode_get(void)
{
    static uint8_t cf;
#ifdef EFUSE_INTERFACE_REPLACE
    errcode_t otp_ret = uapi_efuse_read_buffer(&cf, PAL_OTP_SEC_BOOT_DEBUG_FLAG, PAL_OTP_SEC_BOOT_DEBUG_FLAG_LEN);
    if (otp_ret != ERRCODE_SUCC) {
        return SEC_BOOT_SSB_SIGN_CHECK;
    }

    static uint8_t double_cf;
    otp_ret = uapi_efuse_read_buffer(&double_cf, PAL_OTP_SEC_BOOT_DEBUG_DOUBLE_FLAG, PAL_OTP_SEC_BOOT_DEBUG_FLAG_LEN);
    if (otp_ret != ERRCODE_SUCC) {
        return SEC_BOOT_SSB_SIGN_CHECK;
    }
#else
    otp_ret_t otp_ret = otp_read_byte(&cf, PAL_OTP_SEC_BOOT_DEBUG_FLAG);
    if (otp_ret != OTP_RET_SUCC) {
        return SEC_BOOT_SSB_SIGN_CHECK;
    }

    static uint8_t double_cf;
    otp_ret = otp_read_byte(&double_cf, PAL_OTP_SEC_BOOT_DEBUG_DOUBLE_FLAG);
    if (otp_ret != OTP_RET_SUCC) {
        return SEC_BOOT_SSB_SIGN_CHECK;
    }
#endif
    cf &= double_cf;
    // only OTP_SEC_BOOT_DEBUG_FLAG equal to 0 that is disable sec boot
    if (cf == 0) {
        return SEC_BOOT_DISABLE;
    }

    // cf bit0 is 0 and bit1 is 1, that means sec boot debug function
    if (((cf & BIT(PAL_OTP_SEC_BOOT_EN_BIT)) == 0) &&
        ((cf & BIT(PAL_OTP_SEC_BOOT_DEBUG_BIT)) != 0)) {
        return SEC_BOOT_SSB_SIGN_DEBUG;
    }

    // cf bit0 is 1 and bit2 is 0, that means sec boot ssb sign check function
    if (((cf & BIT(PAL_OTP_SEC_BOOT_EN_BIT)) != 0) &&
        ((cf & BIT(PAL_OTP_SEC_BOOT_KEY_CHECK_EN_BIT)) == 0)) {
        return SEC_BOOT_SSB_SHA256_CHECK;
    }

    // other value default to secure operation
    return SEC_BOOT_SSB_SIGN_CHECK;
}

bool sec_boot_disable_pkcs1_alg_get(void)
{
    static uint8_t cf;
    static uint8_t double_cf;
#ifdef EFUSE_INTERFACE_REPLACE
    errcode_t otp_ret = uapi_efuse_read_buffer(&cf, PAL_OTP_BOOTLOADER_BYTE, PAL_OTP_BOOTLOADER_BYTE_LEN);
    otp_ret |= uapi_efuse_read_buffer(&double_cf, PAL_OTP_BOOTLOADER_DOUBLE_BYTE, PAL_OTP_BOOTLOADER_BYTE_LEN);
#else
    otp_ret_t otp_ret = otp_read_byte(&cf, PAL_OTP_BOOTLOADER_BYTE);
    otp_ret |= otp_read_byte(&double_cf, PAL_OTP_BOOTLOADER_DOUBLE_BYTE);
#endif
    if (otp_ret != ERRCODE_SUCC) {
        return false;
    }

    cf &= double_cf;
    return ((cf & BIT(OTP_BOOTLOADER_FSB_DIS_PKCS1_ALG_BIT)) != 0);
}

bool sec_boot_ssb_sha256_get(void)
{
    uint32_t crc = 0;
    bool ssb_sha256_set = true;

    errcode_t otp_ret;
#ifdef EFUSE_INTERFACE_REPLACE
    otp_ret = uapi_efuse_read_buffer((uint8_t *)&crc, PAL_OTP_SSB_CODE_SIZE_START, sizeof(uint32_t));
#else
    otp_ret = otp_read_buffer((uint8_t *)&crc, PAL_OTP_SSB_CODE_SIZE_START, sizeof(uint32_t));
#endif
    crc = (crc & CRC_MASK) >> CRC_RIGHT_SHIFT_BIT;

    // if there is an error, default to set so we default to secure operation
    if (otp_ret == ERRCODE_SUCC && crc == 0) {
        ssb_sha256_set = false;
    }
    return ssb_sha256_set;
}

bool sec_boot_rootkey_sha256_get(uint8_t *key_sha256, uint16_t length)
{
    uint8_t sha_crc = 0;
    uint8_t calc_crc = 0;

    if (length < PAL_OTP_MCU_ROOT_PUBKEY_SHA256_LENGTH) {
        PRINT("length is error!!!"NEWLINE);
        return false;
    }
#ifdef EFUSE_INTERFACE_REPLACE
    if (uapi_efuse_read_buffer(key_sha256, PAL_OTP_MCU_ROOT_PUBKEY_SHA256_START,
        PAL_OTP_MCU_ROOT_PUBKEY_SHA256_LENGTH) != ERRCODE_SUCC) {
        PRINT("otp read buffer fail!!!"NEWLINE);
        return false;
    }

    if (uapi_efuse_read_buffer(&sha_crc, PAL_OTP_MCU_ROOT_PUBKEY_SHA256_CRC,
        PAL_OTP_MCU_ROOT_PUBKEY_SHA256_CRC_LEN) != ERRCODE_SUCC) {
        PRINT("otp read buffer fail!!!"NEWLINE);
        return false;
    }

    if (uapi_efuse_calc_crc(key_sha256, PAL_OTP_MCU_ROOT_PUBKEY_SHA256_LENGTH, &calc_crc) != ERRCODE_SUCC) {
        PRINT("otp calc crc fail!!!"NEWLINE);
        return false;
    }
#else
    if (otp_read_buffer(key_sha256, PAL_OTP_MCU_ROOT_PUBKEY_SHA256_START,
        PAL_OTP_MCU_ROOT_PUBKEY_SHA256_LENGTH) != OTP_RET_SUCC) {
        PRINT("otp read buffer fail!!!"NEWLINE);
        return false;
    }

    if (otp_read_byte(&sha_crc, PAL_OTP_MCU_ROOT_PUBKEY_SHA256_CRC) != OTP_RET_SUCC) {
        PRINT("otp read buffer fail!!!"NEWLINE);
        return false;
    }

    if (otp_calc_crc(key_sha256, PAL_OTP_MCU_ROOT_PUBKEY_SHA256_LENGTH, &calc_crc) != OTP_RET_SUCC) {
        PRINT("otp calc crc fail!!!"NEWLINE);
        return false;
    }
#endif
    if (calc_crc != sha_crc) {
        PRINT("crc not equal!!! calc_crc = 0x%x, sha_crc = 0x%x"NEWLINE, calc_crc, sha_crc);
        return false;
    }
    return true;
}

bool sec_boot_aes_decrypt_get(void)
{
#if SEC_IAMGE_AES_DECRYPT_EN == YES
    static uint8_t cf;
#ifdef EFUSE_INTERFACE_REPLACE
    errcode_t otp_ret = uapi_efuse_read_buffer(&cf, PAL_OTP_SEC_BOOT_DEBUG_FLAG, PAL_OTP_SEC_BOOT_DEBUG_FLAG_LEN);
#else
    otp_ret_t otp_ret = otp_read_byte(&cf, PAL_OTP_SEC_BOOT_DEBUG_FLAG);
#endif
    if (otp_ret != ERRCODE_SUCC) {
        return true;
    }

    static uint8_t double_cf;
#ifdef EFUSE_INTERFACE_REPLACE
    otp_ret = uapi_efuse_read_buffer(&double_cf, PAL_OTP_SEC_BOOT_DEBUG_DOUBLE_FLAG, PAL_OTP_SEC_BOOT_DEBUG_FLAG_LEN);
#else
    otp_ret = otp_read_byte(&double_cf, PAL_OTP_SEC_BOOT_DEBUG_DOUBLE_FLAG);
#endif
    if (otp_ret != ERRCODE_SUCC) {
        return true;
    }

    cf &= double_cf;
    if ((cf & BIT(PAL_OTP_SEC_BOOT_AES_EN_BIT)) == 0) {
        return false;
    }
    return true;
#else
    return false;
#endif
}

void sec_boot_clear_rsa_memory(void)
{
#if SEC_BOOT_SIGN_CHECK_EN == YES
    memset_s(g_rsa_pubkey, sizeof(g_rsa_pubkey), 0, sizeof(g_rsa_pubkey));
    memset_s(g_rsa_sign, sizeof(g_rsa_sign), 0, sizeof(g_rsa_sign));
    memset_s(g_rsa_result, sizeof(g_rsa_result), 0, sizeof(g_rsa_result));
#endif
}
