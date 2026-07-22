/**
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved. \n
 *
 * Description: Provides pke driver header file\n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-08-09, Create file. \n
 */
#ifndef DRV_PKE_H
#define DRV_PKE_H

#include <stdint.h>
#include "errcode.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/**
 * @if Eng
 * @brief Hash types supported by the PKE module
 * @note  SHA1 is not secure, and we advise not to use it.
 * @else
 * @brief pke 模块支持的 hash 类型
 * @note  SHA1不安全，不建议使用
 * @endif
 */
typedef enum drv_pke_hash_type {
    DRV_PKE_HASH_TYPE_SHA1 = 0x00,
    DRV_PKE_HASH_TYPE_SHA224,
    DRV_PKE_HASH_TYPE_SHA256,
    DRV_PKE_HASH_TYPE_SHA384,
    DRV_PKE_HASH_TYPE_SHA512,
    DRV_PKE_HASH_TYPE_SM3 = 0x10,
    DRV_PKE_HASH_TYPE_MAX,
    DRV_PKE_HASH_TYPE_INVALID = 0xffffffff,
} drv_pke_hash_type_t;

/**
 * @if Eng
 * @brief  Length of the key used by the PKE module.
 * @else
 * @brief  pke 模块使用的密钥长度。
 * @endif
 */
typedef enum drv_pke_len {
    DRV_PKE_LEN_2048 = 256,
    DRV_PKE_LEN_4096 = 512,
    DRV_PKE_LEN_MAX,
    DRV_PKE_LEN_INVALID = 0xffffffff,
} drv_pke_len_t;

/**
 * @if Eng
 * @brief  Modes supported by the RSA algorithm of the PKE module
 * @else
 * @brief  pke模块rsa算法支持的模式。
 * @endif
 */
typedef enum drv_pke_rsa_scheme {
    DRV_PKE_RSA_SCHEME_PKCS1_V15 = 0x00,
    DRV_PKE_RSA_SCHEME_PKCS1_V21,
    DRV_PKE_RSA_SCHEME_MAX,
    DRV_PKE_RSA_SCHEME_INVALID = 0xffffffff,
} drv_pke_rsa_scheme_t;

/**
 * @if Eng
 * @brief  RSA private key structure
 * @else
 * @brief  RSA 算法私钥结构体
 * @endif
 */
typedef struct drv_pke_rsa_priv_key {
    uint8_t *n;                             /*!< @if Eng common modulus.
                                                 @else   公共模数 @endif */
    uint8_t *e;                             /*!< @if Eng public index.
                                                 @else   公共指数 @endif */
    uint8_t *d;                             /*!< @if Eng private index.
                                                 @else   私有指数 @endif */
    uint8_t *p;                             /*!< @if Eng First prime factor.
                                                 @else   第一素因数 @endif */
    uint8_t *q;                             /*!< @if Eng Second prime factor.
                                                 @else   第二素因数 @endif */
    uint8_t *dp;                            /*!< @if Eng D % (P - 1).
                                                 @else   D % (P - 1) @endif */
    uint8_t *dq;                            /*!< @if Eng D % (Q - 1).
                                                 @else   D % (Q - 1) @endif */
    uint8_t *qp;                            /*!< @if Eng 1 / (Q % P).
                                                 @else   1 / (Q % P) @endif */
    uint16_t n_len;                         /*!< @if Eng Size of the buffer of the public modulus n. Only 2048, 3072,
                                                         and 4096 are supported.
                                                 @else   公共模数 n 缓冲区大小，仅支持 2048，3072 和 4096 @endif */
    uint16_t e_len;                         /*!< @if Eng Common exponent e buffer size.
                                                 @else   公共指数 e 缓冲区大小 @endif */
    uint16_t d_len;                         /*!< @if Eng Private index d buffer size.
                                                 @else   私有指数 d 缓冲区大小 @endif */
    uint16_t p_len;                         /*!< @if Eng Length of the first prime factor p, which should be half
                                                         of n_len.
                                                 @else   第一个素因数 p 的长度，应为 n_len 的一半 @endif */
    uint16_t q_len;                         /*!< @if Eng Length of the second prime factor q, which should be half
                                                         of n_len.
                                                 @else   第二个素因数 q 的长度，应为 n_len 的一半 @endif */
    uint16_t dp_len;                        /*!< @if Eng Length of D% (P - 1), should be half of n_len.
                                                 @else   D % (P - 1)的长度，应为 n_len 的一半 @endif */
    uint16_t dq_len;                        /*!< @if Eng Length of D% (Q - 1), should be half of n_len.
                                                 @else   D % (Q - 1)的长度，应为 n_len 的一半 @endif */
    uint16_t qp_len;                        /*!< @if Eng Length of 1 / (Q% P) should be half of n_len.
                                                 @else   1 / (Q % P)的长度，应为 n_len 的一半 @endif */
} drv_pke_rsa_priv_key_t;

/**
 * @if Eng
 * @brief  Public key structure of the RSA algorithm
 * @else
 * @brief  RSA 算法公钥结构体
 * @endif
 */
typedef struct drv_pke_rsa_pub_key {
    uint8_t  *n;                             /*!< @if Eng common modulus.
                                                  @else   公共模数 @endif */
    uint8_t  *e;                             /*!< @if Eng public index.
                                                  @else   公共指数 @endif */
    uint16_t len;                            /*!< @if Eng Common modulus size. The maximum value is 512 bytes..
                                                  @else   公共模数大小,最大为512字节 @endif */
} drv_pke_rsa_pub_key_t;

/**
 * @if Eng
 * @brief  Data structure of the PKE module
 * @else
 * @brief  PKE模块数据结构体
 * @endif
 */
typedef struct drv_pke_data {
    uint32_t length;                         /*!< @if Eng Data Byte Length.
                                                  @else   数据字节长度 @endif */
    uint8_t *data;                           /*!< @if Eng Data Address.
                                                  @else   数据地址 @endif */
} drv_pke_data_t;

/**
 * @if Eng
 * @brief  Use the RSA public key to encrypt a plaintext segment.。
 * @param  [in] scheme RSA padding mode, for example, PKCS1_V21/PKCS1_V15.
 * @param  [in] hash_type Hash type, which is checked in PSS-OAEP mode.
 * @param  [in] pub_key RSA public key.
 * @param  [in] input Data to be encrypted.
 * @param  [in] label User label data structure, which is optional and used only in OAEP mode.
 * @param  [out] output Encrypted result data. The buffer size cannot be smaller than the bit width of RSA key N.
 * @retval ERRCODE_SUCC Success.
 * @retval Other        Failure. For details, see @ref errcode_t
 * @else
 * @brief  使用RSA公钥加密一段明文
 * @param  [in] scheme RSA填充模式，例如：PKCS1_V21/PKCS1_V15。
 * @param  [in] hash_type Hash类型，PSS-OAEP 模式会对其进行检查。
 * @param  [in] pub_key RSA公钥
 * @param  [in] input 待加密数据。
 * @param  [in] label 用户标签数据结构体，可选并且只在OAEP模式中使用。
 * @param  [out] output 加密结果数据，它的缓冲区大小不能小于RSA密钥N的位宽。
 * @retval ERRCODE_SUCC 成功
 * @retval Other        失败，参考 @ref errcode_t
 * @endif
 */
errcode_t uapi_drv_pke_rsa_public_encrypt(const drv_pke_rsa_scheme_t scheme, const drv_pke_hash_type_t hash_type,
    const drv_pke_rsa_pub_key_t *pub_key, const drv_pke_data_t *input, const drv_pke_data_t *label,
    drv_pke_data_t *output);

/**
 * @if Eng
 * @brief  Use the RSA private key to decrypt a segment of ciphertext.
 * @param  [in] scheme RSA padding mode, for example, PKCS1_V21/PKCS1_V15.
 * @param  [in] hash_type Hash type, which is checked in PSS-OAEP mode.
 * @param  [in] priv_key RSA private key.
 * @param  [in] input Data to be decrypted.
 * @param  [in] label User label data structure, which is optional and used only in OAEP mode.
 * @param  [out] output Decryption result data.
 * @retval ERRCODE_SUCC Success.
 * @retval Other        Failure. For details, see @ref errcode_t
 * @else
 * @brief  使用RSA私钥解密一段密文
 * @param  [in] scheme RSA填充模式，例如：PKCS1_V21/PKCS1_V15。
 * @param  [in] hash_type Hash类型，PSS-OAEP 模式会对其进行检查。
 * @param  [in] priv_key RSA private key
 * @param  [in] input 待解密数据。
 * @param  [in] label 用户标签数据结构体，可选并且只在OAEP模式中使用。
 * @param  [out] output 解密结果数据。
 * @retval ERRCODE_SUCC 成功
 * @retval Other        失败，参考 @ref errcode_t
 * @endif
 */
errcode_t uapi_drv_pke_rsa_private_decrypt(const drv_pke_rsa_scheme_t scheme, const drv_pke_hash_type_t hash_type,
    const drv_pke_rsa_priv_key_t *priv_key, const drv_pke_data_t *input, const drv_pke_data_t *label,
    drv_pke_data_t *output);

/**
 * @if Eng
 * @brief  Sign a piece of text using the RSA private key.
 * @param  [in] priv_key RSA private key.
 * @param  [in] scheme RSA padding mode, for example, PKCS1_V21/PKCS1_V15.
 * @param  [in] hash_type Hash type.
 * @param  [in] input_hash Abstracts to be signed.
 * @param  [out] sign Data structure of the signature information.
 * @retval ERRCODE_SUCC Success.
 * @retval Other        Failure. For details, see @ref errcode_t
 * @else
 * @brief  使用RSA私钥签名一段文本
 * @param  [in] priv_key RSA private key.
 * @param  [in] scheme RSA填充模式，例如：PKCS1_V21/PKCS1_V15。
 * @param  [in] hash_type Hash类型
 * @param  [in] input_hash 待签名的摘要。
 * @param  [out] sign 签名信息的数据结构体。
 * @retval ERRCODE_SUCC 成功
 * @retval Other        失败，参考 @ref errcode_t
 * @endif
 */
errcode_t uapi_drv_pke_rsa_sign(const drv_pke_rsa_priv_key_t *priv_key, const drv_pke_rsa_scheme_t scheme,
    const drv_pke_hash_type_t hash_type, const drv_pke_data_t *input_hash, drv_pke_data_t *sign);

/**
 * @if Eng
 * @brief  Sign a piece of text using the RSA private key.
 * @param  [in] pub_key RSA public key.
 * @param  [in] scheme RSA padding mode, for example, PKCS1_V21/PKCS1_V15.
 * @param  [in] hash_type Hash type.
 * @param  [in] input_hash Abstracts to be signed.
 * @param  [out] sign Data structure of the signature information.
 * @retval ERRCODE_SUCC Success.
 * @retval Other        Failure. For details, see @ref errcode_t
 * @else
 * @brief  使用RSA公钥验签签名文件
 * @param  [in] pub_key RSA public key.
 * @param  [in] scheme RSA填充模式，例如：PKCS1_V21/PKCS1_V15。
 * @param  [in] hash_type Hash类型
 * @param  [in] input_hash 待签名的摘要。
 * @param  [out] sign 签名信息的数据结构体。
 * @retval ERRCODE_SUCC 成功
 * @retval Other        失败，参考 @ref errcode_t
 * @endif
 */
errcode_t uapi_drv_pke_rsa_verify(const drv_pke_rsa_pub_key_t *pub_key, drv_pke_rsa_scheme_t scheme,
    const drv_pke_hash_type_t hash_type, const drv_pke_data_t *input_hash, const drv_pke_data_t *sign);

/**
 * @if Eng
 * @brief  out = in ^ k mod n,exponentiation of modules
 * @param  [in] n, public modulus.
 * @param  [in] k, common exponent
 * @param  [in] in, input to modular operation.
 * @param  [out] out, oputput of modular operation.
 * @retval ERRCODE_SUCC Success.
 * @retval Other        Failure. For details, see @ref errcode_t
 * @else
 * @brief  out = in ^ k mod n，模的幂运算。
 * @param  [in] n, 公共模数
 * @param  [in] k, 公共指数
 * @param  [in] in, 模运算的输入
 * @param  [out] out, 模运算的输出
 * @retval ERRCODE_SUCC 成功
 * @retval Other        失败，参考 @ref errcode_t
 * @endif
 */
errcode_t uapi_drv_pke_exp_mod(const drv_pke_data_t *n, const drv_pke_data_t *k, const drv_pke_data_t *in,
    drv_pke_data_t *out);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif