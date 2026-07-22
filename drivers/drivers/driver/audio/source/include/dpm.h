/*
 * Copyright (c) CompanyNameMagicTag 2020-2021. All rights reserved.
 * Description: header file for audio dpms and dpms
 * Author: audio
 */

#ifndef __DPM_H__
#define __DPM_H__

#include "audio_struct.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/* Define audio DPM common command ID */
/* CNcomment:定义音频 DPM 公共CMD ID */
#define DPM_COMMON_CMD_ID 0xeeee

/* Define  DPM get DPM output */
/* CNcomment:定义获取编DPM参数 */
#define DPM_PRIVATE_PARAM_GET_CMD ((((td_u32)DPM_COMMON_CMD_ID) << 16) | 0x1000)

#define DPM_IN_FRAME_SIZE_GET_CMD ((((td_u32)DPM_COMMON_CMD_ID) << 16) | 0x1001)
#define DPM_OUT_FRAME_SIZE_GET_CMD ((((td_u32)DPM_COMMON_CMD_ID) << 16) | 0x1002)
#define DPM_SCRATCH_BUF_SET_CMD ((((td_u32)DPM_COMMON_CMD_ID) << 16) | 0x1003)

/* audio format definition */
/* CNcomment:audio format 定义 */
typedef enum {
    DPM_FORMAT_FIR = 0x000,
    DPM_FORMAT_MAX = 0x1ff,
} dpm_format;

/* audio DPM ID definition */
/* CNcomment:音频编DPMID定义 */
typedef enum {
    DPM_ID_FIR,

    DPM_ID_MAX,
} dpm_id;

/* Define hardware dpm set parameter of out buffer address. */
/* CNcomment:定义硬解输出缓存地址设置参数 */
typedef struct {
    /* set outbuf addr command. */
    /* CNcomment: 设置输出Buffer地址命令 */
    td_u32 cmd;

    /* outbuf addr. */
    /* CNcomment:输出Buffer地址 */
    td_u32 out_buf_addr;
} dpm_out_buf_addr_set_param;

/* parameter to set DPM scratch buffer */
/* CNcomment:设置scratch buf */
typedef struct {
    td_void *scratch_buf;
    td_u32 scratch_buf_size;
} dpm_param_set_scratch_buf;

/* Parameters for creating an dpm. */
/* CNcomment:DPM创建配置参数 */
typedef struct {
    /* Pointer of dpm private open parameters
     * note: set NULL if there is no private parameters
     */
    /* CNcomment: DPM私有参数 */
    td_void *private_data;

    /* Size of dpm private open parameters
     * note: set to 0 if there is no private parameters
     */
    /* CNcomment: DPM私有参数大小 */
    td_u32 private_data_size;
} dpm_param;

/* input pcm packet struct */
/* CNcomment: DPM输入pcm数据结构体 */
typedef struct {
    /* IN PTS (unit:us) Note: dpm should discard this information. */
    /* CNcomment:PTS，单位:微秒 */
    td_s64 pts;

    /* IN/OUT pointer to input auduo pcm data note: dpm would update data after dpm. */
    /* CNcomment:输入数据指针 */
    td_u8 *data;

    /* IN/OUT size of the input auduo data. unit: Byte. */
    /* CNcomment:输入数据大小 */
    td_u32 size;
} dpm_in_packet;

/* dpm output struct. */
/* CNcomment:DPM输出结构体 */
typedef struct {
    /* the pointer to dpm bitstream output buffer note: ps32BitsOutBuf must  be word32-aligned. */
    /* CNcomment:编码数据输出buffer */
    td_s32 *bits_out_buf;

    /* the buffer size of bitstream output buffer. unit:Byte. */
    /* CNcomment:DPM输出buffer大小 */
    td_u32 bits_out_buf_size;

    /* size of the dpm audio data frame, unit: Byte. */
    /* CNcomment:编码数据帧大小，单位:Byte */
    td_u32 bits_out_bytes;

    /* compress bit rate of the audio stream. */
    /* CNcomment:比特率 */
    td_u32 bit_rate;
} dpm_out_packet;

/* dpm struct define. */
/* CNcomment:定义DPM结构体 */
typedef struct dpm_entry {
    /* Input, description information about audio dpm. */
    /* CNcomment:IN DPM描述名字 */
    const td_char *name;

    /* dpm ID. Note: This ID is the identifier of a dpm, and must be unique. */
    /* CNcomment:IN DPM标识，注意: 此标识作为DPM唯一身份标识，必须唯一而不能有重复 */
    const td_u32 dpm_id;

    /* Input, dpm version. */
    /* CNcomment:IN DPM版本描述 */
    const audio_lib_version version;

    /* Input, Description information of the audio dpm. */
    /* CNcomment:IN DPM详细描述信息 */
    const td_char *description;

    /* OUT pointer to next dpm.manager by client.
    client:The layer of software that invokes the methods of the dpm. */
    /* CNcomment:OUT 下个DPM结构体指针，由客户管理 */
    struct dpm *next;

    /* Output, pointer to the dll symbol of an dpm.
    This member variable is maintained by the client that calls the  DPM Therefore,
    the developers of the dpm can ignore this member variable. */
    /* CNcomment:OUT 该成员变量由调用DPM 的客户端维护, DPM开发者可以忽略该成员 */
    td_void *module;

    /*
    \brief Initializes a dpm. CNcomment:初始化DPM CNend
    \attention
    \param[in] open_param pointer of the open params CNcomment:open参数结构体指针 CNend
    \param[out] ph_dpm   pointer of the dpm handle CNcomment:DPM句柄指针 CNend
    \retval ::0  SUCCESS CNcomment:成功 CNend
    \retval ::s32 FAILURE CNcomment:失败 CNend
    \see
    N/A
    */
    td_s32 (*init)(td_void **dpm, const dpm_param *open_param);

    /*
    \brief DeInitializes a dpm. CNcomment:去初始化DPM CNend
    \attention
    \param[in] h_dpm   pointer of the dpm handle CNcomment:DPM句柄 CNend
    \retval ::0  SUCCESS CNcomment:成功 CNend
    \retval ::s32 FAILURE CNcomment:失败 CNend
    \see
    N/A
    */
    td_s32 (*deinit)(td_void *dpm);

    /*
    \brief Configure a dpm dynamically. You can call this API before a dpm works.
    CNcomment:DPM动态配置方法，用户可以在DPM运行前调用该接口 CNend
    \attention
    \param[in] dpm   the dpm handle CNcomment:DPM句柄 CNend
    \param[in] cmd   the cmd code CNcomment:命令字 CNend
    \param[in] config pointer to application allocated structure to be used for initialization by the dpm
    CNcomment:DPM初始化结构体 CNend
    \retval ::0  SUCCESS CNcomment:成功 CNend
    \retval ::s32 FAILURE CNcomment:失败 CNend
    \see
    N/A
    */
    td_s32 (*send_cmd)(td_void *dpm, td_u32 cmd, td_void *arg);

    /*
    \brief Maximum output size of an dpm bitstream CNcomment:DPM输出所需最大缓冲区size CNend
    \attention
    \param[in] dpm   the dpm handle CNcomment:DPM句柄 CNend
    \param[out] out_size pointer to the max size of the dpm audio frame. unit:Byte
    CNcomment:最大编码帧输出缓冲区大小 CNend \retval ::0  SUCCESS CNcomment:成功 CNend \retval
    ::s32 FAILURE CNcomment:失败 CNend \see N/A
    */
    td_s32 (*get_max_bits_out_size)(td_void *dpm, td_u32 *out_size);

    /*
    \brief This method is used to dpm a frame. CNcomment:DPM编码一帧方法 CNend
    \attention
    \param[in] dpm   the dpm handle CNcomment:DPM句柄 CNend
    \param[in] in_pkt pointer to audio stream packet CNcomment:音频输入流指针 CNend
    \param[out] out_pkt pointer to audio output CNcomment:音频输出流指针 CNend
    \retval ::0  SUCCESS CNcomment:成功 CNend
    \retval ::s32 FAILURE CNcomment:失败 CNend
    \see
    N/A
    */
    td_s32 (*proc_data)(td_void *dpm, dpm_in_packet *in_pkt, dpm_out_packet *out_pkt);
} dpm_entry;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif /* __DPM_H__ */
