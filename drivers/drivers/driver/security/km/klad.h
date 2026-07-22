/**drv_klad_attr_t
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved. \n
 *
 * Description: Provides klad driver api\n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-08-09, Create file. \n
 */
#ifndef KLAD_H
#define KLAD_H

#include <stdint.h>
#include <stdbool.h>
#include "errcode.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define HKL_KEY_LEN    16

/**
 * @defgroup drivers_driver_km Klad
 * @ingroup  drivers_driver
 * @{
 */

/**
 * @if Eng
 * @brief Define the algorithm of crypto engine.
 * @else
 * @brief 定义加密引擎的算法
 * @endif
 */
typedef enum {
    DRV_CRYPTO_ALG_AES,
    DRV_CRYPTO_ALG_LEA,
    DRV_CRYPTO_ALG_SM4,
    DRV_CRYPTO_ALG_TDES,
    DRV_CRYPTO_ALG_HMAC_SHA1,
    DRV_CRYPTO_ALG_HMAC_SHA2,
    DRV_CRYPTO_ALG_HMAC_SM3,
    DRV_CRYPTO_ALG_MAX,
    DRV_CRYPTO_ALG_INVALID = 0xffffffff,
} drv_crypto_alg_t;

/**
 * @if Eng
 * @brief Define the destination type of keyladder.
 * @else
 * @brief 定义层级密钥的目标类型
 * @endif
 */
typedef enum {
    DRV_KLAD_DEST_TYPE_MCIPHER = 0x00,
    DRV_KLAD_DEST_TYPE_HMAC,
    DRV_KLAD_DEST_TYPE_FLASH,
    DRV_KLAD_DEST_TYPE_MAX,
    DRV_KLAD_DEST_TYPE_INVALID = 0xffffffff,
} drv_klad_dest_type_t;

/**
 * @if Eng
 * @brief  Key parity, valid when key length is not more than 128bit
 * @else
 * @brief  密钥奇偶属性
 * @endif
 */
typedef enum {
    DRV_KLAD_KEY_EVEN =  0x0,
    DRV_KLAD_KEY_ODD  =  0x1,
    DRV_KLAD_KEY_PARITY_MAX,
    DRV_KLAD_KEY_PARITY_INVALID = 0xffffffff,
} drv_klad_key_parity_t;

/**
 * @if Eng
 * @brief  Define the key security attribute.
 * @else
 * @brief  密钥安全属性
 * @endif
 */
typedef enum {
    DRV_KLAD_SEC_DISABLE = 0,
    DRV_KLAD_SEC_ENABLE,
    DRV_KLAD_SEC_MAX,
    DRV_KLAD_SEC_INVALID = 0xffffffff,
} drv_klad_sec_t;

/**
 * @if Eng
 * @brief Define the keyladder algorithm.
 * @else
 * @brief 定义层级可用于的算法
 * @endif
 */
typedef enum {
    DRV_KLAD_ALG_TYPE_TDES = 0,
    DRV_KLAD_ALG_TYPE_AES,
    DRV_KLAD_ALG_TYPE_SM4,
    DRV_KLAD_ALG_TYPE_MAX,
    DRV_KLAD_ALG_TYPE_INVALID = 0xffffffff,
} drv_klad_alg_type_t;

/**
 * @if Eng
 * @brief Define the klad level.
 * @else
 * @brief 定义密钥的级别
 * @endif
 */
typedef enum {
    DRV_KLAD_LEVEL1 = 0,
    DRV_KLAD_LEVEL_MAX,
    DRV_KLAD_LEVEL_INVALID = 0xffffffff,
} drv_klad_level_t;

/**
 * @if Eng
 * @brief Define the keyladder key size type.
 * @else
 * @brief 定义层级密钥算法密钥长度
 * @endif
 */
typedef struct {
    uint32_t key_size;
    uint32_t key_size_reg_val;
} drv_klad_key_size_t;

/**
 * @if Eng
 * @brief Define the keyladder .
 * @else
 * @brief 定义层级密钥配置
 * @endif
 */
typedef struct {
    uint32_t rootkey_type;
} drv_klad_config_t;

/**
 * @if Eng
 * @brief Define the structure of content key security configurations.
 * @note  when cipher work mode is CBC_MAC, dest_buf_sec_support and dest_buf_non_sec_support cannot be both false.
 * @else
 * @note 当密码工作模式为CBC_MAC时，dest_buf_sec_support和dest_buf_non_sec_support不能同时为false。
 * @brief 定义层级密钥配置
 * @endif
 */
typedef struct {
    drv_klad_sec_t key_sec;        /*!< @if Eng Key security attribute
                                        @else 密钥安全属性 @endif */
    bool master_only_enable;        /*!< @if Eng master only enable
                                        @else 仅master可用 @endif */
    bool dest_buf_sec_support;     /*!< @if Eng The destination buffer of target engine can be secure.
                                        @else 目标引擎的目标缓冲区可以是安全的 @endif */
    bool dest_buf_non_sec_support; /*!< @if Eng The destination buffer of target engine can be non-secure.
                                        @else 目标引擎的目标缓冲区可以是不安全的 @endif */
    bool src_buf_sec_support;      /*!< @if Eng The source buffer of target engine can be secure.
                                        @else 目标引擎的源缓冲区可以是安全的 @endif */
    bool src_buf_non_sec_support;  /*!< @if Eng The source buffer of target engine can be non-secure.
                                        @else 目标引擎的源缓冲区可以是不安全的 @endif */
} drv_klad_key_secure_config_t;

/* Define the structure of content key configurations. */

typedef struct {
    drv_crypto_alg_t engine; /*!< @if Eng The content key can be used for which algorithm of the crypto engine.
                                  @else 密文密钥可用于加密引擎的哪种算法 @endif */
    bool decrypt_support;    /*!< @if Eng The content key can be used for decrypting.
                                  @else 密文密钥可用于解密 @endif */
    bool encrypt_support;    /*!< @if Eng The content key can be used for encrypting.
                                  @else 密文密钥可用于加密 @endif */
} drv_klad_key_config_t;

/**
 * @if Eng
 * @brief Define the struct of keyladder attributes
 * @else
 * @brief 定义keylader属性的结构体
 * @endif
 */
typedef struct {
    drv_klad_config_t klad_cfg;               /*!< @if Eng The keyladder configuration, valid for harware key.
                                                   @else KeyLader配置，对硬件密钥有效 @endif */
    drv_klad_key_config_t key_cfg;            /*!< @if Eng The content key configuration.
                                                   @else 密文密钥配置 @endif */
    drv_klad_key_secure_config_t key_sec_cfg; /*!< @if Eng The content key security configuration.
                                                   @else 密文密钥安全属性 @endif */
} drv_klad_attr_t;

/**
 * @if Eng
 * @brief Define the struct of clear key
 * @else
 * @brief 定义明文key的属性
 * @endif
 */
typedef struct {
    uint32_t key_size;                      /*!< @if Eng The size of content key, 16 or 32 for cipher
                                                 @else 明文密钥的大小 @endif */
    uint8_t *key;                           /*!< @if Eng The clear key.
                                                 @else 明文key存储指针 @endif */
    drv_klad_key_parity_t key_parity;       /*!< @if Eng Odd or even key flag.
                                                 @else 密钥奇偶属性 @endif */
} drv_klad_clear_key_t;

/**
 * @if Eng
 * @brief Define the struct of effective key
 * @else
 * @brief 定义硬件key的属性
 * @endif
 */
typedef struct drv_klad_effective_key {
    uint8_t *deriv_param;                   /*!< @if Eng  The deriv param.
                                                 @else    密钥派生参数。 @endif */
    uint32_t param_len;                     /*!< @if Eng  The length of deriv param.
                                                 @else    密钥派生参数长度。 @endif */
    uint32_t key_size;                      /*!< @if Eng  The size of effective key, 16 or 32 for cipher
                                                 @else    硬件密钥的大小 @endif */
} drv_klad_effective_key_t;

/**
 * @if Eng
 * @brief Cipher crate klad handle.
 * @note Before calling any other api in klad, you must call this function first.
 * @param [out] hklad : klad handle.
 * @retval ERRCODE_SUCC Success.
 * @retval Other        Failure. For details, see @ref errcode_t
 * @else
 * @brief  创建cipher虚拟通道句柄
 * @note 在调用klad其他接口之前，要求先调用本接口
 * @param [out] hklad : klad handle.
 * @retval ERRCODE_SUCC 成功
 * @retval Other        失败，参考 @ref errcode_t
 * @endif
 */
errcode_t drv_klad_create(uintptr_t *hklad);

/**
 * @if Eng
 * @brief destroy klad handle.
 * @note The klad handle must have been created.
 * @param [in] hklad : klad handle.
 * @retval ERRCODE_SUCC Success.
 * @retval Other        Failure. For details, see @ref errcode_t
 * @else
 * @brief  销毁cipher虚拟通道句柄
 * @note klad句柄必须已创建
 * @param [in] hklad : klad句柄.
 * @retval ERRCODE_SUCC 成功
 * @retval Other        失败，参考 @ref errcode_t
 * @endif
 */
errcode_t drv_klad_destroy(const uintptr_t hklad);

/**
 * @if Eng
 * @brief Bind the klad handle to the keylot channel.
 * @note The klad handle must have been created.
 * @param [in] hklad : klad handle.
 * @param [in] dest : Indicates the keylot type bound to the dest.
 * @param [in] hkeyslot : hkeyslot When dest is set to mcipher or hmac, this parameter indicates the keyslot handle.
 * @retval ERRCODE_SUCC Success.
 * @retval Other        Failure. For details, see @ref errcode_t
 * @else
 * @brief  绑定klad句柄与keyslot通道。
 * @note klad句柄必须已创建。
 * @param [in] hklad : klad句柄.
 * @param [in] dest : 绑定的keyslot类型.
 * @param [in] hkeyslot : 当dest为mcipher或hmac时，指keyslot句柄
 * @retval ERRCODE_SUCC 成功
 * @retval Other        失败，参考 @ref errcode_t
 * @endif
 */
errcode_t drv_klad_attach(const uintptr_t hklad, const drv_klad_dest_type_t dest, const uintptr_t hkeyslot);

/**
 * @if Eng
 * @brief Unbind the Klad Handle from the Keyslot Channel.
 * @note The klad handle must have been created.
 * @param [in] hklad : klad handle.
 * @param [in] dest : Indicates the keylot type bound to the dest.
 * @param [in] hkeyslot : hkeyslot When dest is set to mcipher or hmac, this parameter indicates the keyslot handle.
 * @retval ERRCODE_SUCC Success.
 * @retval Other        Failure. For details, see @ref errcode_t
 * @else
 * @brief 解绑klad句柄与keyslot通道。
 * @note klad句柄必须已创建。
 * @param [in] hklad : klad句柄.
 * @param [in] dest : 绑定的keyslot类型.
 * @param [in] hkeyslot : 当dest为mcipher或hmac时，指keyslot句柄
 * @retval ERRCODE_SUCC 成功
 * @retval Other        失败，参考 @ref errcode_t
 * @endif
 */
errcode_t drv_klad_detach(const uintptr_t hklad, const drv_klad_dest_type_t dest, const uintptr_t hkeyslot);

/**
 * @if Eng
 * @brief Sets the klad handle attribute.
 * @note  The klad handle must have been created.The klad handle attribute pointer cannot be null.
 * @param [in] hklad : klad handle.
 * @param [in] attr : Pointer to the klad handle attribute.
 * @retval ERRCODE_SUCC Success.
 * @retval Other        Failure. For details, see @ref errcode_t
 * @else
 * @brief 设置klad句柄属性
 * @note  klad句柄必须已创建。klad句柄属性指针不能为空。
 * @param [in] hklad : klad句柄。
 * @param [in] attr : klad句柄属性指针.
 * @retval ERRCODE_SUCC 成功
 * @retval Other        失败，参考 @ref errcode_t
 * @endif
 */
errcode_t drv_klad_set_attr(const uintptr_t hklad, const drv_klad_attr_t *attr);

/**
 * @if Eng
 * @brief Obtains the klad handle attribute.
 * @note  The klad handle must have been created.The klad handle attribute pointer cannot be null.
 * @param [in] hklad : klad handle.
 * @param [out] attr : Pointer to the klad handle attribute.
 * @retval ERRCODE_SUCC Success.
 * @retval Other        Failure. For details, see @ref errcode_t
 * @else
 * @brief 获取klad句柄属性
 * @note  klad句柄必须已创建。klad句柄属性指针不能为空。
 * @param [in] hklad : klad句柄。
 * @param [out] attr : klad句柄属性指针.
 * @retval ERRCODE_SUCC 成功
 * @retval Other        失败，参考 @ref errcode_t
 * @endif
 */
errcode_t drv_klad_get_attr(const uintptr_t hklad, drv_klad_attr_t *attr);

/**
 * @if Eng
 * @brief Set the plaintext key.
 * @note The klad handle must have been created.The pointer for storing the plaintext key attribute cannot be empty.
 * @param [in] hklad : klad handle.
 * @param [in] key : Pointer to the plaintext key attribute.
 * @retval ERRCODE_SUCC Success.
 * @retval Other        Failure. For details, see @ref errcode_t
 * @else
 * @brief 设置明文密钥
 * @note klad句柄必须已创建。存储key属性指针不能为空。
 * @param [in] hklad : klad句柄.
 * @param [in] key : 存储明文密钥属性的指针.
 * @retval ERRCODE_SUCC 成功
 * @retval Other        失败，参考 @ref errcode_t
 * @endif
 */
errcode_t drv_klad_set_clear_key(const uintptr_t hklad, const drv_klad_clear_key_t *key);

/**
 * @if Eng
 * @brief Set the effective key.
 * @note The klad handle must have been created.The pointer for storing the effective key attribute cannot be empty.
 * @param [in] hklad : klad handle.
 * @param [in] key : Pointer to the plaintext key attribute.
 * @retval ERRCODE_SUCC Success.
 * @retval Other        Failure. For details, see @ref errcode_t
 * @else
 * @brief 设置硬件密钥
 * @note klad句柄必须已创建。存储key属性指针不能为空。
 * @param [in] hklad : klad句柄.
 * @param [in] key : 存储硬件密钥属性的指针.
 * @retval ERRCODE_SUCC 成功
 * @retval Other        失败，参考 @ref errcode_t
 * @endif
 */
errcode_t drv_klad_set_effective_key(const uintptr_t hklad, const drv_klad_effective_key_t *key);

/**
 * @}
 */

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
