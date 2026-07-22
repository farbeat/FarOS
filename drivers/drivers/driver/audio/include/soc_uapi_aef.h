/*
 * Copyright (c) @CompanyNameMagicTag 2023-2023. All rights reserved.
 * Description: user api for audio effect
 * Author: @CompanyNameTag
 */

#ifndef __SOC_UAPI_AEF_H__
#define __SOC_UAPI_AEF_H__

#include "td_type.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

/**
 * @defgroup soc_uapi_aef soc_uapi_aef
 * @ingroup  audio
 * @{
 */

/**
 * @if Eng
 * @brief  Audio effect type.
 * @else
 * @brief  音效类型
 * @endif
 */
typedef enum {
    UAPI_AEF_TYPE_NULL, /*!< @if Eng NULL audio effect.
                             @else   无效音效 @endif */

    UAPI_AEF_TYPE_DOLBY = 0x001, /*!< @if Eng Dolby audio effect
                                      @else   Dolby音效 @endif */

    UAPI_AEF_TYPE_SRS3D = 0x010, /*!< @if Eng SRS audio effect
                                      @else   SRS音效 @endif */

    UAPI_AEF_TYPE_CUSTOMER = 0x020, /*!< @if Eng Customer audio effect
                                      @else   客户自研音效 @endif */

    UAPI_AEF_TYPE_CUSTOMER_A = 0x021, /*!< @if Eng Customer A audio effect
                                           @else   客户A自研音效 @endif */

    UAPI_AEF_TYPE_LP = 0x100, /*!< @if Eng LP audio effect
                                   @else   LP音效 @endif */

    UAPI_AEF_TYPE_DBX = 0x110, /*!< @if Eng DBX audio effect
                                   @else   DBX音效 @endif */

    UAPI_AEF_TYPE_DBX2 = 0x111, /*!< @if Eng DBX2 audio effect
                                     @else   DBX2音效 @endif */

    UAPI_AEF_TYPE_SPECTRUM = 0x120, /*!< @if Eng SPECTRUM
                                     @else   SPECTRUM组件 @endif */

    UAPI_AEF_TYPE_SWS1 = 0x130, /*!< @if Eng SWS3D V1.0 audio effect
                                     @else   SWS3D V1.0音效 @endif */

    UAPI_AEF_TYPE_SWS2 = 0x131, /*!< @if Eng SWS3D V2.0 audio effect
                                     @else   SWS3D V2.0音效 @endif */

    UAPI_AEF_TYPE_SWS3 = 0x132, /*!< @if Eng SWS3D V3.0 audio effect
                                     @else   SWS3D V3.0音效 @endif */

    UAPI_AEF_TYPE_KDE = 0x150, /*!< @if Eng KDE audio effect
                                    @else   卡拉OK音效 @endif */

    UAPI_AEF_TYPE_SOOVEN = 0x160, /*!< @if Eng SOOVEN audio effect
                                       @else   SOOVEN音效 @endif */

    UAPI_AEF_TYPE_SOOVEN2 = 0x161, /*!< @if Eng SOOVEN2 audio effect
                                        @else   SOOVEN2音效 @endif */

    UAPI_AEF_TYPE_SOOVEN3 = 0x162, /*!< @if Eng SOOVEN3 audio effect
                                        @else   SOOVEN3音效 @endif */

    UAPI_SND_AEF_TYPE_GLOBAL = 0x170, /*!< @if Eng global audio effect
                                        @else   全局音效 @endif */

    UAPI_SND_AEF_TYPE_SUPPLEMENTAL1 = 0x171, /*!< @if Eng supplemental1 audio effect
                                                  @else   补充1音效，补充全局音效的单一特性 @endif */

    UAPI_SND_AEF_TYPE_SUPPLEMENTAL2 = 0x172, /*!< @if Eng supplemental2 audio effect
                                                  @else   补充2音效，补充全局音效的单一特性 @endif */

    UAPI_AEF_TYPE_SMARTPA = 0x180, /*!< @if Eng SMARTPA audio effect
                                        @else   SMARTPA音效 @endif */

    UAPI_AEF_TYPE_SAID = 0x190, /*!< @if Eng Sleep aid audio effect.
                                     @else   助眠音效 @endif */

    UAPI_AEF_TYPE_MAX
} uapi_aef_type;

/**
 * @}
 */

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __SOC_UAPI_AEF_H__ */
