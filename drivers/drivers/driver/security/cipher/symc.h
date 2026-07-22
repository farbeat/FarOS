/**
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved. \n
 *
 * Description: Provides symc crypto driver api\n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-08-09, Create file. \n
 */
#ifndef SYMC_H
#define SYMC_H

#include <stdint.h>
#include "errcode.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define AES_IV_SIZE                     16
#define DRV_WORD_WIDTH                  4
#define DRV_CIPHER_AES_IV_LEN_IN_BYTES  16
#define DRV_CIPHER_AES_BLOCK_SIZE       16

/**
 * @defgroup drivers_driver_cipher Cipher
 * @ingroup  drivers_driver
 * @{
 */

/**
 * @if Eng
 * @brief Encryption/Decryption type selecting
 * @else
 * @brief Cipher加解密类型选择
 * @endif
 */
typedef enum {
    DRV_CIPHER_TYPE_NORMAL = 0x0,
    DRV_CIPHER_TYPE_REG,
    DRV_CIPHER_TYPE_MAX,
    DRV_CIPHER_TYPE_INVALID = 0xffffffff,
} drv_cipher_type_t;

/**
 * @if Eng
 * @brief Key parity, valid when key length is not more than 128bit
 * @else
 * @brief 密钥奇偶属性,密钥长度不超过128bit时有效
 * @endif
 */
typedef enum {
    DRV_CIPHER_KEY_EVEN =  0x0,
    DRV_CIPHER_KEY_ODD  =  0x1,
    DRV_CIPHER_KEY_PARITY_MAX,
    DRV_CIPHER_KEY_PARITY_INVALID = 0xffffffff,
} drv_cipher_key_parity_t;

/**
 * @if Eng
 * @brief IV Change Type
 * @else
 * @brief IV变化类型
 * @endif
 */
typedef enum {
    DRV_CIPHER_IV_DO_NOT_CHANGE = 0,   /*!< @if Eng IV donot change, cipher only set IV at the first time
                                            @else   iv不改变，只在第一次配置IV @endif */
    DRV_CIPHER_IV_CHANGE_ONE_PKG = 1,  /*!< @if Eng Cipher set IV for the first package
                                            @else   第一个包配置IV @endif */
    DRV_CIPHER_IV_CHANGE_ALL_PKG = 2,  /*!< @if Eng Cipher set IV for each package
                                            @else   每一个包都配置IV @endif */
    DRV_CIPHER_IV_CHANGE_MAX,
    DRV_CIPHER_IV_CHANGE_INVALID = 0xffffffff,
} drv_cipher_iv_change_type_t;

/**
 * @if Eng
 * @brief GCM iv Change Type
 * @else
 * @brief GCM模式iv变化类型
 * @endif
 */
typedef enum {
    DRV_CIPHER_GCM_IV_DO_NOT_CHANGE = 0,
    DRV_CIPHER_GCM_IV_CHANGE_START = 1,
    DRV_CIPHER_GCM_IV_CHANGE_UPDATE = 2,
    DRV_CIPHER_GCM_IV_CHANGE_FINISH = 3,
    DRV_CIPHER_GCM_IV_CHANGE_MAX,
    DRV_CIPHER_GCM_IV_CHANGE_INVALID = 0xffffffff,
} drv_cipher_gcm_iv_change_type_t;

/**
 * @if Eng
 * @brief CCM iv Change Type
 * @else
 * @brief CCM模式iv变化类型
 * @endif
 */
typedef enum {
    DRV_CIPHER_CCM_IV_DO_NOT_CHANGE = 0,
    DRV_CIPHER_CCM_IV_CHANGE_START = 1,
    DRV_CIPHER_CCM_IV_CHANGE_UPDATE = 2,
    DRV_CIPHER_CCM_IV_CHANGE_FINISH = 3,
    DRV_CIPHER_CCM_IV_CHANGE_MAX,
    DRV_CIPHER_CCM_IV_CHANGE_INVALID = 0xffffffff,
} drv_cipher_ccm_iv_change_type_t;

/**
 * @if Eng
 * @brief Buffer Security Properties
 * @else
 * @brief 缓冲区安全属性
 * @endif
 */
typedef enum {
    DRV_CIPHER_BUFFER_NONSECURE = 0x00,
    DRV_CIPHER_BUFFER_SECURE,
    DRV_CIPHER_BUFFER_INVALID = 0xffffffff,
} drv_cipher_buffer_sec_t;

/**
 * @if Eng
 * @brief cipher lock mode
 * @note Search for idle cipher channels and lock for CMAC. It is recommended that other algorithms be locked or
 *       unlocked only during encryption and decryption.
 * @else
 * @brief 加密通道占用类型
 * @note 查找空闲的cipher通道并lock for CMAC,其它算法最好只在加解密操作的时候lock/unlock
 * @endif
 */
typedef enum {
    DRV_CIPHER_LONG_TERM_LOCK = 0x0,
    DRV_CIPHER_SHORT_TERM_LOCK,
    DRV_CIPHER_LOCK_MODE_MAX,
    DRV_CIPHER_LOCK_MODE_INVALID = 0xffffffff,
} drv_lock_mode_t;

/**
 * @if Eng
 * @brief  Cipher algorithm
 * @else
 * @brief Cipher 加密算法
 * @endif
 */
typedef enum {
    DRV_CIPHER_ALG_TDES = 0x0,
    DRV_CIPHER_ALG_AES = 0x1,
    DRV_CIPHER_ALG_SM4 = 0x2,
    DRV_CIPHER_ALG_LEA = 0x3,
    DRV_CIPHER_ALG_MAX,
    DRV_CIPHER_ALG_INVALID = 0xffffffff,
} drv_cipher_alg_t;

/**
 * @if Eng
 * @brief  Cipher work mode
 * @else
 * @brief Cipher工作模式
 * @endif
 */
typedef enum {
    DRV_CIPHER_WORK_MODE_ECB,
    DRV_CIPHER_WORK_MODE_CBC,
    DRV_CIPHER_WORK_MODE_CFB,
    DRV_CIPHER_WORK_MODE_OFB,
    DRV_CIPHER_WORK_MODE_CTR,
    DRV_CIPHER_WORK_MODE_CCM,
    DRV_CIPHER_WORK_MODE_GCM,
    DRV_CIPHER_WORK_MODE_CBC_MAC,
    DRV_CIPHER_WORK_MODE_CMAC,
    DRV_CIPHER_WORK_MODE_MAX,
    DRV_CIPHER_WORK_MODE_INVALID = 0xffffffff,
} drv_cipher_work_mode_t;

/**
 * @if Eng
 * @brief  Cipher bit width
 * @else
 * @brief 加密位宽
 * @endif
 */
typedef enum {
    DRV_CIPHER_BIT_WIDTH_1BIT = 0x0,
    DRV_CIPHER_BIT_WIDTH_8BIT = 0x1,
    DRV_CIPHER_BIT_WIDTH_64BIT = 0x2,
    DRV_CIPHER_BIT_WIDTH_128BIT = 0x3,
    DRV_CIPHER_BIT_WIDTH_MAX,
    DRV_CIPHER_BIT_WIDTH_INVALID = 0xffffffff,
} drv_cipher_bit_width_t;

/**
 * @if Eng
 * @brief  Key length
 * @else
 * @brief 密钥长度
 * @endif
 */
typedef enum {
    DRV_CIPHER_KEY_64BIT =  0x0,
    DRV_CIPHER_KEY_128BIT = 0x1,
    DRV_CIPHER_KEY_192BIT = 0x2,
    DRV_CIPHER_KEY_256BIT = 0x3,
    DRV_CIPHER_KEY_LENGTH_MAX,
    DRV_CIPHER_KEY_LENGTH_INVALID = 0xffffffff,
} drv_cipher_key_length_t;

/**
 * @if Eng
 * @brief  Indicates the DFA type
 * @else
 * @brief DFA类型
 * @endif
 */
typedef enum {
    DRV_CIPHER_DFA_ENABLE = 0x05,
    DRV_CIPHER_DFA_DISABLE = 0x0a,
    DRV_CIPHER_DFA_INVALID = 0xffffffff,
} drv_cipher_dfa_t;

/**
 * @if Eng
 * @brief  indicates key select, soft key or hardware key
 * @else
 * @brief key的类型
 * @endif
 */
typedef enum {
    DRV_DIPHER_KEY_HARDWARE = 0x00,
    DRV_DIPHER_KEY_SOFT = 0x01,
    DRV_CIPHER_KEY_SEL_INVALID = 0xffffffff,
} drv_cipher_key_sel_t;

/**
 * @if Eng
 * @brief Expand Structure of the cipher control information
 * @else
 * @brief 加密控制信息扩展结构
 * @endif
 */
typedef struct {
    drv_cipher_type_t cipher_type;        /*!< @if Eng cipher type normal or reg
                                               @else 加解密类型普通模式或者寄存器模式 @endif */
    drv_lock_mode_t lock_mode;            /*!< @if Eng long term or short term lock
                                               @else 长期或短期占用通道 @endif */
} drv_cipher_attr_t;

/**
 * @if Eng
 * @brief Structure of the cipher AES ECB/CBC/CTR/OFB/CFB/CBC-MAC/CMAC control information
 * @else
 * @brief AES加密控制信息结构
 * @endif
 */
typedef struct {
    drv_cipher_key_length_t key_len;             /*!< @if Eng Key length
                                                      @else 密钥长度 @endif */
    drv_cipher_key_parity_t key_parity;          /*!< @if Eng Key parity
                                                      @else 密钥奇偶性 @endif */
    drv_cipher_bit_width_t bit_width;            /*!< @if Eng Bit width for encryption or decryption for CFB/OFB mode
                                                      @else CFB/OFB模式加解密的比特位宽 @endif */
    drv_cipher_iv_change_type_t iv_change_flag;  /*!< @if Eng iv state change flag
                                                      @else iv状态配置类型 @endif */
    uint8_t iv[DRV_CIPHER_AES_IV_LEN_IN_BYTES];  /*!< @if Eng Initialization vector (IV) in/out for CBC/CTR
                                                      @else CBC/CTR模式的初始化向量 @endif */
} drv_cipher_config_aes_t;

/**
 * @if Eng
 * @brief Structure of the cipher AES GCM control information
 * @else
 * @brief AES GCM 加密控制信息结构
 * @endif
 */
typedef struct {
    drv_cipher_key_length_t key_len;                   /*!< @if Eng Key length
                                                            @else 密钥长度 @endif */
    drv_cipher_key_parity_t key_parity;                /*!< @if Eng Key parity
                                                            @else 密钥奇偶性 @endif */
    drv_cipher_gcm_iv_change_type_t iv_change_flag;    /*!< @if Eng iv state change flag
                                                            @else iv状态配置类型 @endif */
    uint8_t *iv;                                       /*!< @if Eng Initialization vector (IV)
                                                            @else GCM初始化向量 @endif */
    uint32_t iv_len;                                   /*!< @if Eng Initialization vector (IV)
                                                            @else GCM初始化向量长度 @endif */
    uint8_t *aad;                                      /*!< @if Eng Associated data for GCM
                                                            @else GCM的关联数据 @endif */
    uint32_t aad_len;                                  /*!< @if Eng Associated data for GCM
                                                            @else GCM的关联数据长度 @endif */
    uint32_t data_len;                                 /*!< @if Eng data for GCM
                                                            @else GCM的数据长度 @endif */
    uint8_t j0[DRV_CIPHER_AES_IV_LEN_IN_BYTES];        /*!< @if Eng Internally calculated value j0.
                                                            @else gcm内部计算需要的j0 @endif */
    uint8_t iv_ctr[DRV_CIPHER_AES_IV_LEN_IN_BYTES];    /*!< @if Eng IV Values Used for Internal CTR Calculations
                                                            @else CTR计算的iv @endif */
    uint8_t iv_ghash[DRV_CIPHER_AES_IV_LEN_IN_BYTES];  /*!< @if Eng IV value used for internal GHASH calculation
                                                            @else GHASH计算的iv @endif */
} drv_cipher_config_aes_gcm_t;

/**
 * @if Eng
 * @brief Structure of the cipher AES CCM control information
 * @else
 * @brief AES CCM 加密控制信息结构
 * @endif
 */
typedef struct {
    drv_cipher_key_length_t key_len;                 /*!< @if Eng Key length
                                                          @else 密钥长度 @endif */
    drv_cipher_key_parity_t key_parity;              /*!< @if Eng Key parity
                                                          @else 密钥奇偶性 @endif */
    drv_cipher_ccm_iv_change_type_t iv_change_flag;  /*!< @if Eng iv state change flag
                                                          @else iv状态配置类型 @endif */
    uint32_t iv_len;                                 /*!< @if Eng IV lenght for CCM, which should be 7~13
                                                          @else CCM IV长度,范围7~13 @endif */
    uint8_t *aad;                                    /*!< @if Eng Associated data for CCM
                                                          @else CCM的关联数据 @endif */
    uint32_t aad_len;                                /*!< @if Eng length of Eng Associated data for CCM
                                                          @else CCM的关联数据长度 @endif */
    uint8_t  tag_len;                                /*!< @if Eng should be 4,6,8,10,12,14,16
                                                          @else 长度需要是4,6,8,10,12,14,16 @endif */
    uint32_t total_len;                              /*!< @if Eng Total length of encrypted data
                                                          @else 加密数据总长度 @endif */
    uint8_t s0[DRV_CIPHER_AES_IV_LEN_IN_BYTES];      /*!< @if Eng Internally calculated value s0.
                                                          @else ccm内部计算需要的s0 @endif */
    uint8_t iv_ctr[DRV_CIPHER_AES_IV_LEN_IN_BYTES];  /*!< @if Eng IV Values Used for Internal CTR Calculations.
                                                          @else CTR内部计算的iv @endif */
    uint8_t iv_mac[DRV_CIPHER_AES_IV_LEN_IN_BYTES];  /*!< @if Eng IV value used for internal CBC-MAC calculation.
                                                          @else CBC-MAC计算的iv @endif */
} drv_cipher_config_aes_ccm_t;

/**
 * @if Eng
 * @brief cipher buffer attributes
 * @else
 * @brief 加密buffer属性
 * @endif
 */
typedef struct {
    uint8_t *address;                      /*!< @if Eng physical address, should align to 4 bytes
                                                @else 物理地址,需要4字节对齐 @endif */
    drv_cipher_buffer_sec_t buf_sec;       /*!< @if Eng Buffer security attributes
                                                @else buffer安全属性 @endif */
} drv_cipher_buff_attr_t;

/**
 * @if Eng
 * @brief Expand Structure of the cipher control information
 * @else
 * @brief 加密控制信息扩展结构
 * @endif
 */
typedef struct {
    drv_cipher_alg_t alg;             /*!< @if Eng Cipher algorithm
                                           @else   Cipher 算法类型 @endif */
    drv_cipher_work_mode_t work_mode; /*!< @if Eng Cipher work mode
                                           @else   Cipher 工作模式 @endif */
    drv_cipher_dfa_t dfa;             /*!< @if Eng Cipher dfa type
                                           @else   Cipher dfa类型 @endif */
    drv_cipher_key_sel_t key_sel;     /*!< @if Eng Cipher key sel type
                                           @else   Cipher key sel @endif */
    /** @if Eng  Parameter for special algorithm. \n
     *           for AES ECB/CBC/CTR/OFB/CFB/CBC-MAC, the pointer should point to drv_cipher_config_aes_t;
     *           for AES_CMAC, the pointer should point to drv_cipher_config_aes_cmac_t;
     *           for AES_GCM, the pointer should point to drv_cipher_config_aes_gcm_t;
     *           for AES_CCM, the pointer should point to drv_cipher_config_aes_ccm_t;
     *           for TDES, the pointer should point to drv_cipher_config_tdes_t;
     *           for SM4, the pointer should point to drv_cipher_config_sm4_t;
     *  @else    算法的专用参数，\n
     *           对于 AES ECB/CBC/CTR/OFB/CFB/CBC-MAC, 指针应指向 drv_cipher_config_aes_t;
     *           对于 AES_CMAC, 指针应指向 drv_cipher_config_aes_cmac_t;
     *           对于 AES_GCM, 指针应指向 drv_cipher_config_aes_gcm_t;
     *           对于 AES_CCM, 指针应指向 drv_cipher_config_aes_ccm_t;
     *           对于 TDES, 指针应指向 drv_cipher_config_tdes_t;
     *           对于 SM4, 指针应指向 drv_cipher_config_sm4_t;
     *           对于 LEA, 指针应指向 drv_cipher_config_lea_t;
     *  @endif */
    void *param;
} drv_cipher_config_t;

/**
 * @if Eng
 * @brief  Creates a cipher virtual channel handle.
 * @param  [out] hcipher cipher handle
 * @param  [in]  cipher_attr Pointer to the cipher handle property
 * @retval ERRCODE_SUCC Success.
 * @retval Other        Failure. For details, see @ref errcode_t
 * @else
 * @brief  创建cipher虚拟通道句柄
 * @param  [out] hcipher cipher句柄
 * @param  [in]  cipher_attr cipher 句柄属性指针
 * @retval ERRCODE_SUCC 成功
 * @retval Other        失败，参考 @ref errcode_t
 * @endif
 */
errcode_t uapi_drv_cipher_create(uintptr_t *hcipher, const drv_cipher_attr_t *cipher_attr);

/**
 * @if Eng
 * @brief  Creates a cipher virtual channel handle.
 * @param  [in] hcipher cipher handle
 * @note   The cipher handle must have been created.The detach operation has been performed to unbind the cipher
           handle from the keyslot channel.
 * @retval ERRCODE_SUCC Success.
 * @retval Other        Failure. For details, see @ref errcode_t
 * @else
 * @brief  销毁cipher虚拟通道句柄
 * @param  [in] hcipher cipher句柄
 * @note   cipher句柄必须已创建。执行销毁操作前必须已经执行了detach操作解绑cipher句柄与keyslot通道
 * @retval ERRCODE_SUCC 成功
 * @retval Other        失败，参考 @ref errcode_t
 * @endif
 */
errcode_t uapi_drv_cipher_destroy(const uintptr_t hcipher);

/**
 * @if Eng
 * @brief Configuring the cipher handle attribute.
 * @note The cipher handle must have been created.
 * @param [in] hcipher : cipher handle.
 * @param [in] ctrl : Pointer to the cipher cryptographic algorithm attribute, such as the algorithm, working mode,
               IV, key length, and key parity.
 * @retval ERRCODE_SUCC Success.
 * @retval Other        Failure. For details, see @ref errcode_t
 * @else
 * @brief  配置cipher句柄属性
 * @note  cipher句柄必须已创建
 * @param [in] hcipher : cipher句柄。
 * @param [in] ctrl : cipher 密码算法属性配置指针，例如算法、工作模式、iv、密钥长度、密钥奇偶属性等。
 * @retval ERRCODE_SUCC 成功
 * @retval Other        失败，参考 @ref errcode_t
 * @endif
 */
errcode_t uapi_drv_cipher_set_config(const uintptr_t hcipher, const drv_cipher_config_t *ctrl);

/**
 * @if Eng
 * @brief Obtain the cipher handle attribute.
 * @note The cipher handle must have been created.
 * @param [in] hcipher : cipher handle.
 * @param [out] ctrl : Pointer to the cipher cryptographic algorithm attribute
 * @retval ERRCODE_SUCC Success.
 * @retval Other        Failure. For details, see @ref errcode_t
 * @else
 * @brief 获取cipher句柄属性
 * @note  cipher句柄必须已创建
 * @param [in] hcipher : cipher句柄。
 * @param [out] ctrl : cipher 存储密码算法属性配置指针。
 * @retval ERRCODE_SUCC 成功
 * @retval Other        失败，参考 @ref errcode_t
 * @endif
 */
errcode_t uapi_drv_cipher_get_config(const uintptr_t hcipher, drv_cipher_config_t *ctrl);

/**
 * @if Eng
 * @brief Bind the cipher handle to the keyslot channel.
 * @note  The cipher handle must have been created.The keyslot channel type for attach must be KEYSLOT_TYPE_MCIPHER.
 * @param [in] hcipher : cipher handle.
 * @param [in] hkeyslot : keyslot channel.
 * @retval ERRCODE_SUCC Success.
 * @retval Other        Failure. For details, see @ref errcode_t
 * @else
 * @brief 绑定cipher句柄与keyslot通道
 * @note cipher句柄必须已创建。attach的keyslot通道类型必须为DRV_KEYSLOT_TYPE_MCIPHER。
 * @param [in] hcipher : cipher句柄。
 * @param [in] hkeyslot : keyslot通道。
 * @retval ERRCODE_SUCC 成功
 * @retval Other        失败，参考 @ref errcode_t
 * @endif
 */
errcode_t uapi_drv_cipher_attach(const uintptr_t hcipher, const uintptr_t hkeyslot);

/**
 * @if Eng
 * @brief  Unbind the cipher handle from the keyslot channel.
 * @note  The cipher handle must have been created.The keyslot channel type for attach must be KEYSLOT_TYPE_MCIPHER.
 * @param [in] hcipher : cipher handle.
 * @param [in] hkeyslot : keyslot channel.
 * @retval ERRCODE_SUCC Success.
 * @retval Other        Failure. For details, see @ref errcode_t
 * @else
 * @brief  解绑cipher句柄与keyslot通道
 * @note cipher句柄必须已创建。attach的keyslot通道类型必须为DRV_KEYSLOT_TYPE_MCIPHER。
 * @param [in] hcipher : cipher句柄。
 * @param [in] hkeyslot : keyslot通道。
 * @retval ERRCODE_SUCC 成功
 * @retval Other        失败，参考 @ref errcode_t
 * @endif
 */
errcode_t uapi_drv_cipher_detach(const uintptr_t hcipher, const uintptr_t hkeyslot);

/**
 * @if Eng
 * @brief Symmetric encryption
 * @note  The cipher handle must have been created. The cipher handle attribute must have been configured. The KM
          module must have been invoked to send the key to the keylot channel.
          In ECB/CBC/CTR mode, the length of encrypted data must be block_size aligned and cannot be 0.
          The AES block_size is 16 bytes. In CCM/GCM mode, if the get_tag interface has been invoked to obtain the
          tag value, this interface cannot be invoked.
 * @param [in] hcipher : cipher handle.
 * @param [in] src_buf : Encrypted data source buffer attributes, including the buffer address and buffer security type.
 * @param [in] dest_buf : Attributes of the encryption result buffer, including the buffer address and buffer
               security type.
 * @param [in] length : Length of encrypted data, which must be aligned with block_size..
 * @param [in] timeout_ms : Timeout interval, in milliseconds.
 * @retval ERRCODE_SUCC Success.
 * @retval Other        Failure. For details, see @ref errcode_t
 * @else
 * @brief 对称加密
 * @note cipher句柄必须已创建。必须已经配置了cipher句柄属性。必须已经调用km模块将密钥送到了keyslot通道。
         对于ECB/CBC/CTR模式，加密数据长度必须block_size对齐，且长度不能为0，AES block_size为16字节
         对于CCM/GCM模式，如果已经调用了get_tag接口获取tag值，则不能再调用该接口。
 * @param [in] hcipher : cipher handle。
 * @param [in] src_buf : 加密数据源缓冲区属性，包括缓冲区地址，与缓冲区安全类型。
 * @param [in] dest_buf : 加密结果缓冲区属性，包括缓冲区地址，与缓冲区安全类型。
 * @param [in] length : 加密数据长度，必须block_size对齐。
 * @retval ERRCODE_SUCC 成功
 * @retval Other        失败，参考 @ref errcode_t
 * @endif
 */
errcode_t uapi_drv_cipher_encrypt(const uintptr_t hcipher, const drv_cipher_buff_attr_t *src_buf,
    const drv_cipher_buff_attr_t *dest_buf, const uint32_t length, const uint32_t timeout_ms);

/**
 * @if Eng
 * @brief Symmetric decryption
 * @note  The cipher handle must have been created. The cipher handle attribute must have been configured. The KM
          module must have been invoked to send the key to the keylot channel.
          In ECB/CBC/CTR mode, the length of encrypted data must be block_size aligned and cannot be 0.
          The AES block_size is 16 bytes. In CCM/GCM mode, if the get_tag interface has been invoked to obtain the
          tag value, this interface cannot be invoked.
 * @param [in] hcipher : cipher handle.
 * @param [in] src_buf : Encrypted data source buffer attributes, including the buffer address and buffer security type.
 * @param [in] dest_buf : Attributes of the encryption result buffer, including the buffer address and buffer
               security type.
 * @param [in] length : Length of encrypted data, which must be aligned with block_size..
 * @param [in] timeout_ms : Timeout interval, in milliseconds.
 * @retval ERRCODE_SUCC Success.
 * @retval Other        Failure. For details, see @ref errcode_t
 * @else
 * @brief 对称解密
 * @note  cipher句柄必须已创建。必须已经配置了cipher句柄属性。必须已经调用km模块将密钥送到了keyslot通道。
          对于ECB/CBC/CTR模式，加密数据长度必须block_size对齐，且长度不能为0，AES block_size为16字节
          对于CCM/GCM模式，如果已经调用了get_tag接口获取tag值，则不能再调用该接口。
 * @param [in] hcipher : cipher handle。
 * @param [in] src_buf : 加密数据源缓冲区属性，包括缓冲区地址，与缓冲区安全类型。
 * @param [in] dest_buf : 加密结果缓冲区属性，包括缓冲区地址，与缓冲区安全类型。
 * @param [in] length : 加密数据长度，必须block_size对齐。
 * @param [in] timeout_ms : 超时时间,以毫秒为单位。
 * @retval ERRCODE_SUCC 成功
 * @retval Other        失败，参考 @ref errcode_t
 * @endif
 */
errcode_t uapi_drv_cipher_decrypt(const uintptr_t hcipher, const drv_cipher_buff_attr_t *src_buf,
    const drv_cipher_buff_attr_t *dest_buf, const uint32_t length, const uint32_t timeout_ms);

/**
 * @if Eng
 * @brief The CCM/GCM obtains the tag value.
 * @note  The cipher handle must have been created.The symmetric encryption and decryption interface has been invoked
          to encrypt and decrypt data.This interface can be invoked only in CCM/GCM mode.
 * @param [in]  hcipher : cipher handle.
 * @param [out] tag: Address of the buffer for storing tag values.
 * @param [out] tag_len: Pointer to the size of the buffer for storing tag values.
 * @retval ERRCODE_SUCC Success.
 * @retval Other        Failure. For details, see @ref errcode_t
 * @else
 * @brief  CCM/GCM获取tag值
 * @note  cipher句柄必须已创建。必须已经调用了对称加解密接口完成数据的加解密操作。只有CCM/GCM模式可以调用该接口
 * @param [in]  hcipher : cipher句柄。
 * @param [out] tag: 存储tag值的缓冲区地址。
 * @param [out] tag_len: 存储tag值的缓冲区大小指针。
 * @retval ERRCODE_SUCC 成功
 * @retval Other        失败，参考 @ref errcode_t
 * @endif
 */
errcode_t uapi_drv_cipher_get_tag(uintptr_t hcipher, uint8_t *tag, uint32_t *tag_len);

void drv_cipher_set_key_sel(drv_cipher_key_sel_t key_sel);

/**
 * @}
 */

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif
