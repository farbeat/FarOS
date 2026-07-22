/**
 * Copyright (c) @CompanyNameMagicTag 2022-2023. All rights reserved.
 *
 * Description: Provides mipi driver api
 * Author: @CompanyNameTag
 * History:
 * Create: 2022-04-14

 */


#ifndef __SOC_MIPI_TX_H__
#define __SOC_MIPI_TX_H__
#include <stdint.h>
#define LANE_MAX_NUM   2
#define MIPI_TX_DISABLE_LANE_ID (-1)

#define ext_fence()                  __asm__ __volatile__("fence":::"memory")

typedef enum {
    OUT_MODE_DSI_VIDEO      = 0x1,          /**< dsi video mode */
                                            /**< CNcomment:dsi视频模式。CNend */
    OUT_MODE_DSI_CMD        = 0x2,          /**< dsi command mode. */
                                            /**< CNcomment:dsi命令模式。CNend */
    OUT_MODE_BUTT
} out_mode_t;

typedef enum {
    MIPI_TX_WORK_MODE_HS     = 0x0,         /**< HS(High Speed) work mode */
                                            /**< CNcomment:HS（高速）工作模式。CNend */
    MIPI_TX_WORK_MODE_LP     = 0x1,         /**< LP(Low Power) work mode */
                                            /**< CNcomment:LP（低功耗）工作模式。CNend */
    MIPI_TX_WORK_MODE_BUTT
} mipi_tx_work_mode_t;

typedef enum {
    BURST_MODE                      = 0x0,
    NON_BURST_MODE_SYNC_PULSES      = 0x1,
    NON_BURST_MODE_SYNC_EVENTS      = 0x2,

    VIDEO_DATA_MODE_BUTT
} video_mode_t;

typedef enum {
    OUT_FORMAT_RGB_16BIT          = 0x0, /**< DSI */
    OUT_FORMAT_RGB_18BIT          = 0x1,
    OUT_FORMAT_RGB_18BIT_LOOSELY  = 0x2,
    OUT_FORMAT_RGB_24BIT          = 0x3,
    OUT_FORMAT_YUV420_12BIT       = 0x4,
    OUT_FORMAT_YUV422_16BIT       = 0x5,

    OUT_FORMAT_BUTT
} out_format_t;

typedef struct {
    uint16_t  hsa_pixels;
    uint16_t  hbp_pixels;
    uint16_t  hact_pixels;
    uint16_t  hfp_pixels;

    uint16_t  vsa_lines;
    uint16_t  vbp_lines;
    uint16_t  vact_lines;
    uint16_t  vfp_lines;
} sync_info_t;

typedef struct {
    uint8_t   eotp_tx_en;
    uint8_t   eotp_rx_en;
} pck_en_t;

typedef struct {
    uint8_t         devno;                   /**< device number */
                                                 /**< CNcomment:设备编号 CNend */
    short           lane_id[LANE_MAX_NUM];   /**< lane_id: -1 - disable */
                                                 /**< CNcomment:车道ID:-1-禁用 CNend */
    out_mode_t      out_mode;                /**< output mode: DSI_VIDEO/DSI_CMD */
                                                 /**< CNcomment:输出模式：DSI_VIDEO/DSI_CMD CNend */
    video_mode_t    video_mode;
    out_format_t    out_format;
    sync_info_t     sync_info;
    pck_en_t        pck_en;
    uint32_t        phy_data_rate;           /**< mbps */
    uint32_t        pixel_clk;               /**< KHz */
} combo_dev_cfg_t;

typedef struct {
    mipi_tx_work_mode_t work_mode;  /**< work mode: low power mode, high speed mode. */
                                    /**< CNcomment:工作模式：低功耗模式、高速模式。 CNend */
    uint8_t             devno;      /**< device number */
                                    /**< CNcomment:设备编号 CNend */
    uint16_t            data_type;  /**< data type */
                                    /**< CNcomment:数据类型 CNend */
    uint16_t            cmd_size;
    uint8_t             *cmd;
} cmd_info_t;

typedef struct {
    mipi_tx_work_mode_t work_mode;      /**< work mode: low power mode, high speed mode. */
                                        /**< CNcomment:工作模式：低功耗模式、高速模式。 CNend */
    uint8_t             devno;          /**< device number */
                                        /**< CNcomment:设备编号 CNend */
    uint16_t            data_type;      /**< DSI data type */
                                        /**< CNcomment:DSI数据类型 CNend */
    uint16_t            data_param;     /**< data param,low 8 bit:first param.
                                             high 8 bit:second param, set 0 if not use */
                                        /**< CNcomment:数据参数，低8位：第一个参数。
                                             高8位：第二个参数，如果不使用，则设置0 CNend */
    uint16_t            get_data_size;  /**< read data size */
                                        /**< CNcomment:读取数据大小 CNend */
    uint8_t             *get_data;      /**< read data memery address, should  malloc by user */
                                        /**< CNcomment:读取数据内存地址，应按用户malloc CNend */
} get_cmd_info_t;

/**
 * @ingroup  iot_mipi_tx
 * @brief Initialize mipi tx. CNcomment:mipi tx初始化。CNend
 *
 * @par Description:
 *         Initialize mipi tx. CNcomment:mipi tx初始化。CNend
 * @attention None
 *
 * @param  None
 *
 * @retval #EXT_ERR_SUCCESS  Success.
 * @retval #EXT_ERR_FAILURE  Failure.
 * @par Dependency:
 *            @li soc_mipi_tx.h：describes the mipi tx APIs.CNcomment:文件用于描述mipi tx相关接口。CNend
 * @see None
 */
uint32_t uapi_mipi_tx_init(void);

/**
 * @ingroup  iot_mipi_tx
 * @brief Deinitializes mipi tx. CNcomment:mipi tx去初始化。CNend
 *
 * @par Description:
 *         Deinitializes mipi tx. CNcomment:mipi tx去初始化。CNend
 * @attention None
 *
 * @param  None
 * @retval #EXT_ERR_SUCCESS  Success.
 * @retval #EXT_ERR_FAILURE  Failure.
 * @par Dependency:
 *            @li soc_mipi_tx.h：describes the mipi tx APIs.CNcomment:文件用于描述mipi tx相关接口。CNend
 * @see None
 */
uint32_t uapi_mipi_tx_deinit(void);

/**
 * @ingroup  iot_mipi_tx
 * @brief Configure mipi tx controller and phy. CNcomment:配置mipi tx的控制器和phy。CNend
 *
 * @par Description:
 *         Configure mipi tx controller and phy. CNcomment:配置mipi tx的控制器和phy。CNend
 * @attention None
 *
 * @param  dev_cfg        [IN] type #combo_dev_cfg_t*，mipi tx params. CNcomment:mipi tx配置参数。CNend
 * @retval #EXT_ERR_SUCCESS  Success.
 * @retval #EXT_ERR_FAILURE  Failure.
 * @par Dependency:
 *            @li soc_mipi_tx.h：describes the mipi tx APIs.CNcomment:文件用于描述mipi tx相关接口。CNend
 * @see None
 */
uint32_t uapi_mipi_tx_set_dev_cfg(combo_dev_cfg_t *dev_cfg);

/**
 * @ingroup  iot_mipi_tx
 * @brief Configure mipi tx peripheral via DSI write command. CNcomment:通过DSI写命令配置mipi tx从设备。CNend
 *
 * @par Description:
 *         Configure mipi tx peripheral via DSI write command. CNcomment:通过DSI写命令配置mipi tx从设备。CNend
 * @attention CMD with data type 0x37 can only be send in MIPI_TX_WORK_MODE_HS mode during frame data transmission
 *
 * @param  cmd_info        [IN] type #cmd_info_t*，DSI write command params. CNcomment:mipi DSI写命令参数。CNend
 * @retval #EXT_ERR_SUCCESS  Success.
 * @retval #EXT_ERR_FAILURE  Failure.
 * @par Dependency:
 *            @li soc_mipi_tx.h：describes the mipi tx APIs.CNcomment:文件用于描述mipi tx相关接口。CNend
 * @see None
 */
uint32_t uapi_mipi_tx_set_cmd(cmd_info_t *cmd_info);

/**
 * @ingroup  iot_mipi_tx
 * @brief Read mipi tx peripheral imformation via DSI read command. CNcomment:通过DSI读命令获取mipi tx从设备此信息。CNend
 *
 * @par Description:
 *         Read mipi tx peripheral imformation via DSI read command. CNcomment:通过DSI读命令获取mipi tx从设备此信息。CNend
 * @attention None
 *
 * @param  cmd_info        [OUT] type #get_cmd_info_t*，DSI read command params. CNcomment:mipi DSI读命令参数。CNend
 * @retval #EXT_ERR_SUCCESS  Success.
 * @retval #EXT_ERR_FAILURE  Failure.
 * @par Dependency:
 *            @li soc_mipi_tx.h：describes the mipi tx APIs.CNcomment:文件用于描述mipi tx相关接口。CNend
 * @see None
 */
uint32_t uapi_mipi_tx_get_cmd(get_cmd_info_t *cmd_info);

/**
 * @ingroup  iot_mipi_tx
 * @brief Enable mipi tx pixel data transmission. CNcomment:使能mipi tx像素传输。CNend
 *
 * @par Description:
 *         Enable mipi tx pixel data transmission. CNcomment:使能mipi tx像素传输。CNend
 * @attention None
 *
 * @param  None
 * @retval #EXT_ERR_SUCCESS  Success.
 * @retval #EXT_ERR_FAILURE  Failure.
 * @par Dependency:
 *            @li soc_mipi_tx.h：describes the mipi tx APIs.CNcomment:文件用于描述mipi tx相关接口。CNend
 * @see None
 */
uint32_t uapi_mipi_tx_enable(void);

/**
 * @ingroup  iot_mipi_tx
 * @brief Disable mipi tx pixel data transmission. CNcomment:关闭mipi tx像素传输。CNend
 *
 * @par Description:
 *         Disable mipi tx pixel data transmission. CNcomment:关闭mipi tx像素传输。CNend
 * @attention None
 *
 * @param  None
 * @retval #EXT_ERR_SUCCESS  Success.
 * @retval #EXT_ERR_FAILURE  Failure.
 * @par Dependency:
 *            @li soc_mipi_tx.h：describes the mipi tx APIs.CNcomment:文件用于描述mipi tx相关接口。CNend
 * @see None
 */
uint32_t uapi_mipi_tx_disable(void);

/**
 * @ingroup  iot_mipi_tx
 * @brief Set mipi_tx self define low power mode. CNcomment:设置mipi_tx自定义低功耗模式。CNend
 *
 * @par Description:
 *         Set mipi_tx self define low power mode. CNcomment:设置mipi_tx自定义低功耗模式。CNend
 * @attention In this mode, the DPU controls whether mipi_tx enters low power mode.
 *            CNcomment:该模式下，由DPU来控制mipi_tx是否进入低功耗。CNend
 *
 * @param  None
 * @retval #EXT_ERR_SUCCESS  Success.
 * @retval #EXT_ERR_FAILURE  Failure.
 * @par Dependency:
 *            @li soc_mipi_tx.h：describes the mipi tx APIs.CNcomment:文件用于描述mipi tx相关接口。CNend
 * @see None
 */
uint32_t uapi_mipi_tx_auto_enter_sdlp(void);

/**
 * @ingroup  iot_mipi_tx
 * @brief Software controls mipi_tx to exit self define low power mode. CNcomment:软件控制mipi_tx退出自定义低功耗模式。CNend
 *
 * @par Description:
 *         Software controls mipi_tx to exit self define low power mode. CNcomment:软件控制mipi_tx退出自定义低功耗模式。CNend
 * @attention None
 *
 * @param  None
 * @retval #EXT_ERR_SUCCESS  Success.
 * @retval #EXT_ERR_FAILURE  Failure.
 * @par Dependency:
 *            @li soc_mipi_tx.h：describes the mipi tx APIs.CNcomment:文件用于描述mipi tx相关接口。CNend
 * @see None
 */
uint32_t uapi_mipi_tx_exit_sdlp(void);

/**
 * @ingroup  iot_mipi_tx
 * @brief Software controls mipi_tx to exit ultra low power state. CNcomment:软件控制mipi_tx退出ulps。CNend
 *
 * @par Description:
 *         Software controls mipi_tx to exit ultra low power state. CNcomment:软件控制mipi_tx退出ulps。CNend
 * @attention None
 *
 * @param  None
 * @retval #EXT_ERR_SUCCESS  Success.
 * @retval #EXT_ERR_FAILURE  Failure.
 * @par Dependency:
 *            @li soc_mipi_tx.h：describes the mipi tx APIs.CNcomment:文件用于描述mipi tx相关接口。CNend
 * @see None
 */
uint32_t uapi_mipi_tx_exit_ulps(void);

/**
 * @ingroup  iot_mipi_tx
 * @brief Software controls mipi_tx to enter ultra low power state by clearing exit signal.
          CNcomment:软件通过清除ulps退出信号使mipi_tx重新进入ulps。CNend
 *
 * @par Description:
 *         Software controls mipi_tx to enter ultra low power state by clearing exit signal.
           CNcomment:软件通过清除ulps退出信号使mipi_tx重新进入ulps。CNend
 * @attention None
 *
 * @param  None
 * @retval #EXT_ERR_SUCCESS  Success.
 * @retval #EXT_ERR_FAILURE  Failure.
 * @par Dependency:
 *            @li soc_mipi_tx.h：describes the mipi tx APIs.CNcomment:文件用于描述mipi tx相关接口。CNend
 * @see None
 */
uint32_t uapi_mipi_tx_clear_exit_ulps(void);
#endif /* end of #ifndef __SOC_MIPI_TX_H__ */
