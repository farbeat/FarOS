/**
 * \file config-ont.h
 *
 * \brief 华为家庭网络产品，mbedtls库的配置文件，基于3.1.0版本的默认配置文件。
 *
 *  产品在使用时，在编译前，将此文件强制覆盖默认的配置文件即可，使用方式如下：
 *  cp -f ./configs/huawei/config-ont.h ./include/mbedtls/mbedtls_config.h
 *  
 */
/***********************************家庭网络产品定义的宏************************************/

/**
 * \file mbedtls_config.h
 *
 * \brief Configuration options (set of defines)
 *
 *  This set of compile-time options may be used to enable
 *  or disable features selectively, and reduce the global
 *  memory footprint.
 */
/*
 *  Copyright The Mbed TLS Contributors
 *  SPDX-License-Identifier: Apache-2.0
 *
 *  Licensed under the Apache License, Version 2.0 (the "License"); you may
 *  not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 *  WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

/**
 * This is an optional version symbol that enables comatibility handling of
 * config files.
 *
 * It is equal to the #MBEDTLS_VERSION_NUMBER of the Mbed TLS version that
 * introduced the config format we want to be compatible with.
 */
//#define MBEDTLS_CONFIG_VERSION 0x03000000

#define MBEDTLS_HAVE_ASM

#define MBEDTLS_HAVE_TIME

#define MBEDTLS_HAVE_TIME_DATE

#define MBEDTLS_CIPHER_MODE_CBC

#define MBEDTLS_CIPHER_MODE_CFB

#define MBEDTLS_CIPHER_MODE_CTR

#define MBEDTLS_CIPHER_PADDING_PKCS7
#define MBEDTLS_CIPHER_PADDING_ONE_AND_ZEROS
#define MBEDTLS_CIPHER_PADDING_ZEROS_AND_LEN
#define MBEDTLS_CIPHER_PADDING_ZEROS

#define MBEDTLS_ECP_DP_SECP256R1_ENABLED
#define MBEDTLS_ECP_DP_SECP384R1_ENABLED

#define MBEDTLS_ECP_NIST_OPTIM

#define MBEDTLS_ECDSA_DETERMINISTIC

#define MBEDTLS_KEY_EXCHANGE_PSK_ENABLED
#define MBEDTLS_KEY_EXCHANGE_DHE_PSK_ENABLED
#define MBEDTLS_KEY_EXCHANGE_RSA_PSK_ENABLED
#define MBEDTLS_KEY_EXCHANGE_RSA_ENABLED
#define MBEDTLS_KEY_EXCHANGE_DHE_RSA_ENABLED
#define MBEDTLS_KEY_EXCHANGE_ECDHE_RSA_ENABLED
#define MBEDTLS_KEY_EXCHANGE_ECDHE_ECDSA_ENABLED
#define MBEDTLS_KEY_EXCHANGE_ECDH_RSA_ENABLED
#define MBEDTLS_KEY_EXCHANGE_ECDH_ECDSA_ENABLED

#define MBEDTLS_SSL_KEEP_PEER_CERTIFICATE

#define MBEDTLS_PK_PARSE_EC_EXTENDED

#define MBEDTLS_ERROR_STRERROR_DUMMY

#define MBEDTLS_GENPRIME

#define MBEDTLS_FS_IO

#define MBEDTLS_PKCS1_V15

#define MBEDTLS_PKCS1_V21

#define MBEDTLS_SSL_EXTENDED_MASTER_SECRET

#define MBEDTLS_SSL_RENEGOTIATION

#define MBEDTLS_SSL_MAX_FRAGMENT_LENGTH

#define MBEDTLS_SSL_PROTO_TLS1_2

#define MBEDTLS_SSL_PROTO_TLS1_3

#define MBEDTLS_SSL_TLS1_3_COMPATIBILITY_MODE

#define MBEDTLS_PSA_CRYPTO_C

#define MBEDTLS_SSL_SESSION_TICKETS

#define MBEDTLS_SSL_SERVER_NAME_INDICATION

#define MBEDTLS_VERSION_FEATURES

#define MBEDTLS_X509_RSASSA_PSS_SUPPORT

#define MBEDTLS_AESNI_C

#define MBEDTLS_AES_C

#define MBEDTLS_ASN1_PARSE_C

#define MBEDTLS_ASN1_WRITE_C

#define MBEDTLS_BASE64_C

#define MBEDTLS_BIGNUM_C

#define MBEDTLS_BLOWFISH_C

#define MBEDTLS_CAMELLIA_C

#define MBEDTLS_CCM_C

#define MBEDTLS_CIPHER_C

#define MBEDTLS_CMAC_C

#define MBEDTLS_CTR_DRBG_C

#define MBEDTLS_DEBUG_C

#define MBEDTLS_DES_C

#define MBEDTLS_DHM_C

#define MBEDTLS_ECDH_C

#define MBEDTLS_ECDSA_C

#define MBEDTLS_ECP_C

#define MBEDTLS_ENTROPY_C

#define MBEDTLS_ERROR_C

#define MBEDTLS_GCM_C

#define MBEDTLS_HMAC_DRBG_C

#define MBEDTLS_HKDF_C

#define MBEDTLS_MD_C

#define MBEDTLS_MD2_C

#define MBEDTLS_MD4_C

#define MBEDTLS_MD5_C

#define MBEDTLS_NET_C

#define MBEDTLS_OID_C

#define MBEDTLS_PADLOCK_C

#define MBEDTLS_PEM_PARSE_C

#define MBEDTLS_PEM_WRITE_C

#define MBEDTLS_PK_C

#define MBEDTLS_PK_PARSE_C

#define MBEDTLS_PK_WRITE_C

#define MBEDTLS_PKCS5_C

#define MBEDTLS_PKCS12_C

#define MBEDTLS_PLATFORM_C

#define MBEDTLS_RIPEMD160_C

#define MBEDTLS_RSA_C

#define MBEDTLS_SHA1_C
#define MBEDTLS_SHA224_C
#define MBEDTLS_SHA256_C
#define MBEDTLS_SHA384_C
#define MBEDTLS_SHA512_C

#define MBEDTLS_SSL_CACHE_C

#define MBEDTLS_SSL_COOKIE_C

#define MBEDTLS_SSL_TICKET_C

#define MBEDTLS_SSL_CLI_C

#define MBEDTLS_SSL_SRV_C

#define MBEDTLS_SSL_TLS_C

#define MBEDTLS_TIMING_C

#define MBEDTLS_VERSION_C

#define MBEDTLS_X509_USE_C

#define MBEDTLS_X509_CRT_PARSE_C

#define MBEDTLS_X509_CRL_PARSE_C

#define MBEDTLS_X509_CSR_PARSE_C

#define MBEDTLS_X509_CREATE_C

#define MBEDTLS_X509_CRT_WRITE_C

#define MBEDTLS_X509_CSR_WRITE_C

/**
 * 华为家庭网络产品，对POLARSSL API的适配，所有的代码放在hw_polarssl_api.c文件中，
 * 对应的为polarssl_api.h文件中。 使用此编译宏进行隔离。
 * 其它产品不需要使用此API的，不打开此宏即可。
 */
#define HUAWEI_POLARSSL_API_C

#define HUAWEI_POLARSSL_API_DEBUG_C

#define HUAWEI_POLARSSL_API_FT_C

#define HW_MBEDTLS_CONFIG_C

#define MBEDTLS_SSL_PROTO_DTLS

#define MBEDTLS_SSL_DTLS_HELLO_VERIFY

#define MBEDTLS_PK_RSA_ALT_SUPPORT

/* 自测试宏，开发时打开验证 */
//#define MBEDTLS_SELF_TEST

/*****************************************************************************
 * \brief 华为家庭网络产品，对AESCRYPT2 API的适配，所有的代码放在hw_aescrypt2.c文件中，
 * 对应的为hw_aescrypt2.c文件中。 使用此编译宏进行隔离。
 * 其它产品不需要使用此API的，不打开此宏即可。
******************************************************************************/
#define HUAWEI_ONT_AESCRYPT2_API_C

/*****************************************************************************
 * \brief 华为定制特性，是否开启可以动态配置特性的能力。
 * 如果打开此特性，则支持一些特性的动态配置，如MD2、MD4算法，在编译到库里以后，还可以通过API来限制是否使能。
 * 
******************************************************************************/
#define HUAWEI_CUSTOMIZE_CONFIGS_C

/*****************************************************************************
 * \brief 华为定制特性，是否校验证书有效期。
 * 如果打开此特性，则会在TLS协议栈中回调证书有效性的校验函数。
 * 如果产品没有挂回调校验函数，则按默认规则处理。
 * 如果不开此宏，则回调机制也不生效。
 * 
******************************************************************************/
#define HUAWEI_CUSTOMISE_CHECK_CERT_DATE_C

/*****************************************************************************
 * \brief 华为家庭网络产品定制特性，重载x509_crt_verify_name函数，用于校验设备证书名。 
 * 
******************************************************************************/
#define HUAWEI_ONT_CHECK_CERT_CN_NAME_C

/*****************************************************************************
 * \brief 华为家庭网络产品定制特性，重载mbedtls_ssl_write_hostname_ext函数，用于SSL协议栈中返回主机名。
 * 
******************************************************************************/
#define HUAWEI_ONT_SSL_WRITE_HOSTNAME_C

/*****************************************************************************
 * \brief 华为家庭网络产品定制特性，重载ssl_write_client_hello函数，用于SSL协议栈中返回主机名。
 * 
******************************************************************************/
#define HUAWEI_ONT_CUSTOMIZE_SSL_CLIENT_HELLO_C

/*****************************************************************************
 * \brief 华为家庭网络产品，修改ssl_ciphersuite_match函数逻辑，在算法不匹配时继续处理
 * 
******************************************************************************/
#define HUAWEI_ONT_CUSTOMIZE_SSL_CIPHERSUIE_MATCH_C

/*****************************************************************************
 * \brief 华为家庭网络产品，修改ssl_parse_server_dh_params函数逻辑，协商ECC算法参数定制处理
 * 
******************************************************************************/
#define HUAWEI_ONT_CUSTOMIZE_ECP_SIZE_C

/*****************************************************************************
 * \brief 华为家庭网络产品，修改ssl_write_renegotiation_ext函数逻辑，重新协商
 * 
******************************************************************************/
#define HUAWEI_ONT_CUSTOMIZE_SSL_RENEGOTIATION_C

/*****************************************************************************
 * \brief 华为家庭网络产品，修改SSL协议栈，证书失败时通过文件方式与业务通信。
 * 
******************************************************************************/
#define HUAWEI_ONT_TLS_CERT_FAILED_NOTICE_C
