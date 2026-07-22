/*
 * Copyright (c) @CompanyNameMagicTag. 2022-2022. All rights reserved.
 * Description: UPG product definitions header file
 */

#ifndef UPG_DEFINITIONS_PORTING_H
#define UPG_DEFINITIONS_PORTING_H

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/**
 * @defgroup middleware_chips_brandy_update_api update
 * @ingroup  middleware_chips
 * @{
 */

/**
 * @if Eng
 * @brief  SSB signature image ids.
 * @else
 * @brief  SSB签名镜像ID
 * @endif
 */
#define SSB_SIGNATURE_IMAGE_ID                 0x4B1E3C1E

/**
 * @if Eng
 * @brief  Image ids.
 * @else
 * @brief  SSB镜像ID
 * @endif
 */
#define SSB_IMAGE_ID                           0x4B1E3C2D

/**
 * @if Eng
 * @brief  Root Public Key ids.
 * @else
 * @brief  根公钥ID
 * @endif
 */
#define ROOT_PUBLIC_KEY_ID                     0x4B96B41E

/**
 * @if Eng
 * @brief  customer's Root Public Key ids.
 * @else
 * @brief  客户根公钥ID
 * @endif
 */
#define OEM_ROOT_PUBLIC_KEY_ID                 0x4BA5C31E

/**
 * @if Eng
 * @brief  Recovery signature image ids.
 * @else
 * @brief  Recovery签名镜像ID
 * @endif
 */
#define RECOVERY_SIGNATURE_IMAGE_ID            0x4B69871E

/**
 * @if Eng
 * @brief  Recovery image ids.
 * @else
 * @brief  Recovery镜像ID
 * @endif
 */
#define RECOVERY_IMAGE_ID                      0x4B69872D

/**
 * @if Eng
 * @brief  Application signature image ids.
 * @else
 * @brief  Application签名镜像ID
 * @endif
 */
#define APPLICATION_SIGNATURE_IMAGE_ID         0x4B0F2D1E

/**
 * @if Eng
 * @brief  Application image ids.
 * @else
 * @brief  Application镜像ID
 * @endif
 */
#define APPLICATION_IMAGE_ID                   0x4B0F2D2D

/**
 * @if Eng
 * @brief  Bt signature image ids.
 * @else
 * @brief  Bt签名镜像ID
 * @endif
 */
#define BT_SIGNATURE_IMAGE_ID                  0x4BF01E1E

/**
 * @if Eng
 * @brief  Bt image ids.
 * @else
 * @brief  Bt镜像ID
 * @endif
 */
#define BT_IMAGE_ID                            0x4BF01E2D

/**
 * @if Eng
 * @brief  Dsp_main signature image ids.
 * @else
 * @brief  Dsp_main签名镜像ID
 * @endif
 */
#define DSP_MAIN_SIGNATURE_IMAGE_ID            0x5A87A51E

/**
 * @if Eng
 * @brief  Dsp_main image ids.
 * @else
 * @brief  Dsp_main镜像ID
 * @endif
 */
#define DSP_MAIN_IMAGE_ID                      0x5A87A52D

/**
 * @if Eng
 * @brief  Dsp_overlay signature image ids.
 * @else
 * @brief  Dsp_overlay签名镜像ID
 * @endif
 */
#define DSP_OVERLAY_SIGNATURE_IMAGE_ID         0x5A87A53A

/**
 * @if Eng
 * @brief  Dsp_overlay image ids.
 * @else
 * @brief  Dsp_overlay镜像ID
 * @endif
 */
#define DSP_OVERLAY_IMAGE_ID                   0x5A87A54B

/**
 * @if Eng
 * @brief  key area reserved length.
 * @else
 * @brief  key区保留字段长度
 * @endif
 */
#define KEY_AREA_RESERVED_LEN 176

/**
 * @if Eng
 * @brief  info area user defined length.
 * @else
 * @brief  info区用户字段长度
 * @endif
 */
#define INFO_AREA_USER_LEN 176

/**
 * @if Eng
 * @brief  signature length.
 * @else
 * @brief  签名字段长度
 * @endif
 */
#define SIG_LEN         512

/**
 * @if Eng
 * @brief  FOTA external public key.
 * @else
 * @brief  FOTA二级公钥长度
 * @endif
 */
#define PUBLIC_KEY_LEN 516

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

/**
 * @}
 */

#endif /* UPG_DEFINITIONS_PORTING_H */