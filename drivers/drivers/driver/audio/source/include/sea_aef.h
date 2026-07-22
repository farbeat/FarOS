/*
 * Copyright (c) CompanyNameMagicTag 2021-2022. All rights reserved.
 * Description: header of sws aef
 * Author: audio
 * Create: 2021-12-30
 */

#ifndef __SEA_AEF_H__
#define __SEA_AEF_H__

#include "td_type.h"

#ifdef __cplusplus
extern "C" {
#endif

/* 调度返回码和宏定义 */
#define AUDIO_STFT_LEN     640  // stft length, or window length
#define AUDIO_FRAME_LEN    320  // 1 frame length
#define AUDIO_FRAME_LEN_MS 20   // 1 frame time length (ms)
#define AUDIO_FFT_LEN      640  // fft length
#define AUDIO_BIN_LEN      320  // half of fft length
#define AUDIO_INPUT_FS    16000  // input samplerate
#define AUDIO_PROCESS_FS  16000  // process samplerate

#define AUDIO_VQE_VERSION_LEN 64         // 版本号字符串长度
#define AUDIO_VQE_48K_SAMPLERATE 48000   // 48000采样率定义
#define AUDIO_VQE_16K_SAMPLERATE 16000   // 16000采样率定义
#define AUDIO_VQE_MAX_MIC_NUMBER 1       // 最大麦克风输入个数
#define AUDIO_VQE_MIN_MIC_NUMBER 1       // 最小麦克风输入个数
#define AUDIO_VQE_MAX_REF_NUMBER 1       // 最大参考信号输入个数
#define AUDIO_VQE_MIN_REF_NUMBER 0       // 最小参考信号输入个数
#define AUDIO_VQE_MAX_OUT_NUMBER 1       // 最大输出个数
#define AUDIO_VQE_MIN_OUT_NUMBER 1       // 最小输出个数
#define AUDIO_VQE_SCRATCHBUFF_SIZE 36000 // scratchbuffer大小

#define AUDIO_VQE_MODE_RECORD 1 // 录音模式标志定义
#define AUDIO_VQE_MODE_VOIP 2   // 通话模式标志定义

#define AUDIO_VQE_EQBAND_NUM 50          // EQ频段数
#define AUDIO_VQE_DRC_SEGNUM 6           // DRC段数
#define AUDIO_VQE_NVITEM_INBYTE_CALL 512 // 算法参数大小定义

#define AUDIO_AMPSPEC_Q 7   /* Q of spectrum after fft */
#define AUDIO_POWER_Q (-10) /* Q of power in freq domain */

// 产品ID在算法参数中的偏移量
#define AUDIO_VQE_PRODUCTID_OFFSET (AUDIO_VQE_NVITEM_INBYTE_CALL - 4)
#define AUDIO_VQE_BEAM_NUM 3                // BF最大输出个数
#define AUDIO_VQE_NVITEM_INBYTE_BF 73728    // BF parameter byte number
#define AUDIO_VQE_NVITEM_INBYTE_MD 673412   // MD parameter byte number
#define AUDIO_VQE_NVITEM_INBYTE_AINR 625224 // AINR parameter byte number
#define AUDIO_VQE_COM_PAD_2 2               // padding array size
#define AUDIO_VQE_COM_PAD_3 3               // padding array size
#define AUDIO_VQE_COM_PAD_4 4               // padding array size
#define AUDIO_VQE_COM_PAD_5 5               // padding array size
#define AUDIO_VQE_COM_PAD_6 6               // padding array size
#define AUDIO_VQE_COM_PAD_7 7               // padding array size
#define AUDIO_VQE_COM_PAD_8 8               // padding array size
#define AUDIO_VQE_COM_PAD_10 10             // padding array size
#define AUDIO_VQE_COM_PAD_11 11             // padding array size
/* 错误返回码定义 */
#define AUDIO_VQE_EOK 0 // 函数运行正常
/* AudioVqeVoiceGetSize 返回码 */
#define AUDIO_VQE_GETSIZE_INV_MEMSIZE (-1)               // 通道大小指针为空
#define AUDIO_VQE_GETSIZE_4_BYTES_NOT_ALIGN_MEMSIZE (-2) // 通道大小指针未4字节对齐
#define AUDIO_VQE_GETSIZE_INV_PARAMS (-3)                // 参数指针为空
/* AudioVqeVoiceInit 返回码 */
#define AUDIO_VQE_INIT_INV_HANDLE (-4)               // 句柄为空
#define AUDIO_VQE_INIT_INV_PARAMS (-5)               // 配置参数结构体指针为空
#define AUDIO_VQE_INIT_8_BYTES_NOT_ALIGN_PARAMS (-6) // 配置参数结构体未8字节对齐
#define AUDIO_VQE_INIT_INV_SCRATCHBUF (-7)           // scratch指针为空
#define AUDIO_VQE_INIT_ERR_SCRATCHSIZE (-8)          // scratchSize大小不够
/* AudioVqeVoiceSetParam 返回码 */
#define AUDIO_VQE_SETPARAM_INV_HANDLE (-9)                // 句柄为空
#define AUDIO_VQE_SETPARAM_INV_PARAMS (-10)               // 参数内存指针为空
#define AUDIO_VQE_SETPARAM_8_BYTES_NOT_ALIGN_PARAMS (-11) // 参数内存指针未8字节对齐
#define AUDIO_VQE_SETPARAM_UNINITIED (-12)                // 调用次序错误, 未初始化
#define AUDIO_VQE_SETPARAM_ERR_PROTECTHEAD (-13)          // 通道保护变量错误
/* AudioVqeVoiceGetParam 返回码 */
#define AUDIO_VQE_GETPARAM_INV_HANDLE (-14)               // 句柄为空
#define AUDIO_VQE_GETPARAM_INV_PARAMS (-15)               // 参数内存指针为空
#define AUDIO_VQE_GETPARAM_8_BYTES_NOT_ALIGN_PARAMS (-16) // 参数内存指针未8字节对齐
#define AUDIO_VQE_GETPARAM_UNINITIED (-17)                // 调用次序错误, 未初始化
#define AUDIO_VQE_GETPARAM_ERR_PROTECTHEAD (-18)          // 通道保护变量错误
/* AudioVqeVoiceCheckParam 返回码 */
#define AUDIO_VQE_CHECKPARAM_ERR_MICNUM (-22)            // 输入个数错误
#define AUDIO_VQE_CHECKPARAM_ERR_MICPROCNUM (-23)        // 处理的输入个数错误
#define AUDIO_VQE_CHECKPARAM_ERR_REFNUM (-24)            // 参考个数错误
#define AUDIO_VQE_CHECKPARAM_ERR_REFPROCNUM (-25)        // 处理的参考个数错误
#define AUDIO_VQE_CHECKPARAM_ERR_LINOUT (-26)            // AEC线性滤波个数错误
#define AUDIO_VQE_CHECKPARAM_ERR_OUTNUM (-27)            // 输出个数错误
#define AUDIO_VQE_CHECKPARAM_ERR_MODE (-28)              // 模式错误
#define AUDIO_VQE_CHECKPARAM_ERR_SAMPLERATE (-29)        // 采样率错误
#define AUDIO_VQE_CHECKPARAM_ERR_MICSELECT (-30)         // mic选择使能开关错误
#define AUDIO_VQE_CHECKPARAM_ERR_REFSELECT (-31)         // ref选择使能开关错误
#define AUDIO_VQE_CHECKPARAM_ERR_MICIDX (-32)            // mic选择序号错误
#define AUDIO_VQE_CHECKPARAM_ERR_REFIDX (-33)            // ref选择序号错误
#define AUDIO_VQE_CHECKPARAM_ERR_SWITCH_AEC (-34)        // AEC使能开关错误
#define AUDIO_VQE_CHECKPARAM_ERR_SWITCH_DMNR (-35)       // NR使能开关错误
#define AUDIO_VQE_CHECKPARAM_ERR_SWITCH_FMP (-36)        // FMP使能开关错误
#define AUDIO_VQE_CHECKPARAM_ERR_SWITCH_DEREVERB (-37)   // Dereverb使能开关错误
#define AUDIO_VQE_CHECKPARAM_ERR_SWITCH_AGC (-38)        // AGC使能开关错误
#define AUDIO_VQE_CHECKPARAM_ERR_SWITCH_DRC (-39)        // DRC使能开关错误
#define AUDIO_VQE_CHECKPARAM_ERR_SWITCH_EQ (-40)         // EQ使能开关错误
#define AUDIO_VQE_CHECKPARAM_ERR_SWITCH_LIMITER (-41)    // Limiter使能开关错误
#define AUDIO_VQE_CHECKPARAM_ERR_SWITCH_BF (-42)         // BF使能开关错误
#define AUDIO_VQE_CHECKPARAM_ERR_SWITCH_FMP_AEC_NR (-43) // FMP开关与AEC,NR不匹配
#define AUDIO_VQE_CHECKPARAM_ERR_SWITCH_AINR (-58)       // AINR使能开关错误
#define AUDIO_VQE_CHECKPARAM_ERR_ENABLEREF_AEC (-44)     // AEC开关与参考数不匹配
#define AUDIO_VQE_CHECKPARAM_ERR_ENABLELINEAR_AEC (-45)  // AEC开关与线性滤波个数不匹配
#define AUDIO_VQE_CHECKPARAM_ERR_DMNR_MICNUM (-46)       // DMNRmic数与mic处理数不匹配
#define AUDIO_VQE_CHECKPARAM_ERR_SWITCH_MVDR (-19)       // mvdr使能开关错误
/* AudioVqeVoiceApply 返回码 */
#define AUDIO_VQE_APPLY_INV_HANDLE (-47)                     // 句柄为空
#define AUDIO_VQE_APPLY_UNINITIED (-48)                      // 句柄未初始化
#define AUDIO_VQE_APPLY_ERR_PROTECTHEAD (-49)                // 通道保护位被改写
#define AUDIO_VQE_APPLY_INV_DATA (-50)                       // 数据结构体指针为空
#define AUDIO_VQE_APPLY_4_BYTES_NOT_ALIGN_DATA (-51)         // 数据结构体指针未4字节对齐
#define AUDIO_VQE_APPLY_INV_DATA_DATAIN (-52)                // 数据结构体输入指针为空
#define AUDIO_VQE_APPLY_8_BYTES_NOT_ALIGN_DATA_DATAIN (-53)  // 数据结构体输入指针未8字节对齐
#define AUDIO_VQE_APPLY_INV_DATA_DATAOUT (-54)               // 数据结构体输出指针为空
#define AUDIO_VQE_APPLY_8_BYTES_NOT_ALIGN_DATA_DATAOUT (-55) // 数据结构体输出指针未8字节对齐
#define AUDIO_VQE_APPLY_ERR_SCRATCHSIZE (-56)                // scratchbuffer长度不够
#define AUDIO_VQE_APPLY_INV_SCRATCHBUF (-57)                 // scratchbuffer指针为空
/* AudioVqeVoiceDownGetSize 返回码 */
#define AUDIO_VQE_DOWNGETSIZE_INV_MEMSIZE (-63)               // 下行通道大小指针为空
#define AUDIO_VQE_DOWNGETSIZE_4_BYTES_NOT_ALIGN_MEMSIZE (-64) // 下行通道大小指针未4字节对齐
/* AudioVqeVoiceDownInit 返回码 */
#define AUDIO_VQE_DOWNINIT_INV_HANDLE (-65)               // 下行句柄为空
#define AUDIO_VQE_DOWNINIT_INV_PARAMS (-69)               // 下行配置参数结构体为空
#define AUDIO_VQE_DOWNINIT_8_BYTES_NOT_ALIGN_PARAMS (-70) // 下行配置参数结构体未8字节对齐
#define AUDIO_VQE_DOWNINIT_INV_SCRATCHBUF (-71)           // scratchBuf指针为空
#define AUDIO_VQE_DOWNINIT_ERR_SCRATCHSIZE (-72)          // scratchSize大小不够
/* AudioVqeVoiceDownSetParam 返回码 */
#define AUDIO_VQE_DOWNSETPARAM_INV_HANDLE (-73)               // 句柄为空
#define AUDIO_VQE_DOWNSETPARAM_INV_PARAMS (-74)               // 配置参数指针为空
#define AUDIO_VQE_DOWNSETPARAM_8_BYTES_NOT_ALIGN_PARAMS (-75) // 配置参数指针未8字节对齐
#define AUDIO_VQE_DOWNSETPARAM_UNINITIED (-76)                // 调用次序错误, 未初始化
#define AUDIO_VQE_DOWNSETPARAM_ERR_PROTECTHEAD (-77)          // 通道保护变量错误
/* AudioVqeVoiceDownGetParam 返回码 */
#define AUDIO_VQE_DOWNGETPARAM_INV_HANDLE (-78)               // 句柄为空
#define AUDIO_VQE_DOWNGETPARAM_INV_PARAMS (-79)               // 配置参数指针为空
#define AUDIO_VQE_DOWNGETPARAM_8_BYTES_NOT_ALIGN_PARAMS (-80) // 配置参数指针未8字节对齐
#define AUDIO_VQE_DOWNGETPARAM_UNINITIED (-81)                // 调用次序错误, 未初始化
#define AUDIO_VQE_DOWNGETPARAM_ERR_PROTECTHEAD (-82)          // 通道保护变量错误
/* AudioVqeVoiceDownCheckParam 返回码 */
#define AUDIO_VQE_DOWNCHECKPARAM_ERR_SAMPLERATE (-83)     // 下行采样率配置错误
#define AUDIO_VQE_DOWNCHECKPARAM_ERR_FFT (-84)            // 下行fft配置错误
#define AUDIO_VQE_DOWNCHECKPARAM_ERR_SWITCH_DMNR (-85)    // 下行NR使能开关错误
#define AUDIO_VQE_DOWNCHECKPARAM_ERR_SWITCH_AGC (-86)     // 下行AGC使能开关错误
#define AUDIO_VQE_DOWNCHECKPARAM_ERR_SWITCH_DRC (-87)     // 下行DRC使能开关错误
#define AUDIO_VQE_DOWNCHECKPARAM_ERR_SWITCH_EQ (-88)      // 下行EQ使能开关错误
#define AUDIO_VQE_DOWNCHECKPARAM_ERR_SWITCH_LIMITER (-89) // 下行Limiter使能开关错误
/* AudioVqeVoiceDownApply 返回码 */
#define AUDIO_VQE_DOWNAPPLY_INV_HANDLE (-90)                    // 句柄为空
#define AUDIO_VQE_DOWNAPPLY_UNINITIED (-92)                     // 句柄未初始化
#define AUDIO_VQE_DOWNAPPLY_ERR_PROTECTHEAD (-93)               // 通道保护位被改写
#define AUDIO_VQE_DOWNAPPLY_INV_DATA (-94)                      // 数据输入结构体指针为空
#define AUDIO_VQE_DOWNAPPLY_4_BYTES_NOT_ALIGN_DATA (-95)        // 数据输入结构体未4字节对齐
#define AUDIO_VQE_DOWNAPPLY_INV_DATA_DATAIN (-98)               // 数据输入结构体输入数据指针为空
#define AUDIO_VQE_DOWNAPPLY_8_BYTES_NOT_ALIGN_DATA_DATAIN (-99) // 数据输入结构体输入数据指针未8字节对齐
#define AUDIO_VQE_DOWNAPPLY_INV_DATA_DATAOUT (-100)             // 数据输出结构体输出数据指针为空
#define AUDIO_VQE_DOWNAPPLY_8_BYTES_NOT_ALIGN_DATA_DATAOUT (-101) // 数据输出结构体输出数据指针未8字节对齐
#define AUDIO_VQE_DOWNAPPLY_ERR_SCRATCHSIZE (-102)                // scratchbuffer大小不够
#define AUDIO_VQE_DOWNAPPLY_INV_SCRATCHBUF (-103)                 // scratchbuffer指针为空
/* AudioVqeVoiceGetVersion返回码 */
#define AUDIO_VQE_GETVERSION_INV_VERSION (-105) // 版本指针为空
// AEC
#define AUDIO_VQE_CHECKPARAM_ERR_AECSWITCHNLP (-231)            // ucSwitchNLP配置错误
#define AUDIO_VQE_CHECKPARAM_ERR_AECFILTERNUM (-232)            // ucFilterNum配置错误
#define AUDIO_VQE_CHECKPARAM_ERR_AECALLPARALLEL (-233)          // ucAllParallel配置错误
#define AUDIO_VQE_CHECKPARAM_ERR_AECXCHANGE (-234)              // ucXChange配置错误
#define AUDIO_VQE_CHECKPARAM_ERR_AECXCHANGEGAIN (-235)          // usXChangeGain配置错误
#define AUDIO_VQE_CHECKPARAM_ERR_AECPURE_DELAY (-236)           // usPureDelay配置错误
#define AUDIO_VQE_CHECKPARAM_ERR_AECABF_PFMAX1 (-237)           // usABF_pfMax1配置错误
#define AUDIO_VQE_CHECKPARAM_ERR_AECABF_PFMAX2 (-238)           // usABF_pfMax2配置错误
#define AUDIO_VQE_CHECKPARAM_ERR_AECABF_PFMAX1H (-239)          // usABF_pfMax1H配置错误
#define AUDIO_VQE_CHECKPARAM_ERR_AECABF_PFMAX2H (-240)          // usABF_pfMax2H配置错误
#define AUDIO_VQE_CHECKPARAM_ERR_AECUSPMIN1 (-241)              // usPmin1配置错误
#define AUDIO_VQE_CHECKPARAM_ERR_AECUSPMIN2 (-242)              // usPmin2配置错误
#define AUDIO_VQE_CHECKPARAM_ERR_AECUSDTPMIN1 (-243)            // usDTPmin1配置错误
#define AUDIO_VQE_CHECKPARAM_ERR_AECUSDTPMIN2 (-244)            // usDTPmin2配置错误
#define AUDIO_VQE_CHECKPARAM_ERR_AECMAXHIDX (-245)              // usMaxHidx配置错误
#define AUDIO_VQE_CHECKPARAM_ERR_AECPOSTFILTER (-246)           // ucPostFilter配置错误
#define AUDIO_VQE_CHECKPARAM_ERR_AECPOSTFILTER2 (-247)          // ucPostFilter2配置错误
#define AUDIO_VQE_CHECKPARAM_ERR_AECZOOMFACTOR1 (-248)          // usZoomFactor1配置错误
#define AUDIO_VQE_CHECKPARAM_ERR_AECZOOMFACTOR2 (-249)          // usZoomFactor2配置错误
#define AUDIO_VQE_CHECKPARAM_ERR_AECNLMS_BLOCKNUM1 (-250)       // ucNlmsBlockNum1配置错误
#define AUDIO_VQE_CHECKPARAM_ERR_AECNLMS_BLOCKNUM2 (-251)       // ucNlmsBlockNum2配置错误
#define AUDIO_VQE_CHECKPARAM_ERR_AECKALMAN_BLOCKNUM1 (-252)     // ucKalmanBlockNum1配置错误
#define AUDIO_VQE_CHECKPARAM_ERR_AECKALMAN_BLOCKNUM2 (-253)     // ucKalmanBlockNum2配置错误
#define AUDIO_VQE_CHECKPARAM_ERR_AECNLMS_REF1 (-254)            // ucNlmsRef1配置错误
#define AUDIO_VQE_CHECKPARAM_ERR_AECNLMS_REF2 (-255)            // ucNlmsRef2配置错误
#define AUDIO_VQE_CHECKPARAM_ERR_AECKALMAN_REF1 (-256)          // ucKalmanRef1配置错误
#define AUDIO_VQE_CHECKPARAM_ERR_AECKALMAN_REF2 (-257)          // ucKalmanRef2配置错误
#define AUDIO_VQE_CHECKPARAM_ERR_AECF0FRE1 (-258)               // usF0Fre1配置错误
#define AUDIO_VQE_CHECKPARAM_ERR_AECF0FRE2 (-259)               // usF0Fre2配置错误
#define AUDIO_VQE_CHECKPARAM_ERR_AECDDFLAPARAMG (-260)          // usDDDFlag配置错误
#define AUDIO_VQE_CHECKPARAM_ERR_AECDTZOOM_FACTORL (-261)       // usDTZoomFactorL配置错误
#define AUDIO_VQE_CHECKPARAM_ERR_AECDTZOOM_FACTORH (-262)       // usDTZoomFactorH配置错误
#define AUDIO_VQE_CHECKPARAM_ERR_AECSDZOOM_FACTOR (-263)        // usSDZoomFactor配置错误
#define AUDIO_VQE_CHECKPARAM_ERR_AECALLPARALLEL_BLOCKNUM (-264) // ucAllParallelBlockNum配置错误
#define AUDIO_VQE_CHECKPARAM_ERR_AECFCEPAMP (-265)              // ucCEPAMP配置错误
#define AUDIO_VQE_CHECKPARAM_ERR_AECRESZOOM_FACTORL (-266)      // usResZoomFactor配置错误
#define AUDIO_VQE_CHECKPARAM_ERR_HIGHPROC (-267)                // 超大回声场景门限配置错误
#define AUDIO_VQE_CHECKPARAM_ERR_FASTFILTERSWITCH (-268)        // fast filter开关配置错误

// FMP
#define AUDIO_VQE_CHECKPARAM_ERR_FMPCONFORTSWITCH (-314)    // FMP舒适噪声补偿开关错误
#define AUDIO_VQE_CHECKPARAM_ERR_FMPCONFORTINTENSITY (-315) // FMP噪声补偿因子错误
#define AUDIO_VQE_CHECKPARAM_ERR_FMPPROCESSMODE (-316)      // FMP运行模式错误
// AGC
#define AUDIO_VQE_CHECKPARAM_ERR_AGCTARGETLEVEL (-383)      // AGC目标电平错误
#define AUDIO_VQE_CHECKPARAM_ERR_AGCVOICEFLOOR (-384)       // AGC语音下限错误
#define AUDIO_VQE_CHECKPARAM_ERR_AGCMAXGAIN (-385)          // AGC最大增益错误
#define AUDIO_VQE_CHECKPARAM_ERR_AGCMINGAIN (-386)          // AGC最小增益错误
#define AUDIO_VQE_CHECKPARAM_ERR_AGCADJUSTSPEED (-387)      // AGC调整速度错误
#define AUDIO_VQE_CHECKPARAM_ERR_AGCSPDIFF (-388)           // AGC声压差阈值错误
#define AUDIO_VQE_CHECKPARAM_ERR_AGCSMALLSIGNAL (-389)      // AGC小信号门限错误
#define AUDIO_VQE_CHECKPARAM_ERR_AGCADDGAIN (-390)          // AGC小信号额外增益错误
#define AUDIO_VQE_CHECKPARAM_ERR_AGCDISTSWITCH (-391)       // AGC失真单频音处理开关
#define AUDIO_VQE_CHECKPARAM_ERR_AGCDISTALPHA (-392)        // AGC失真单频音处理系数
#define AUDIO_VQE_CHECKPARAM_ERR_AGCSPECIALDISTLEVEL (-393) // AGC特殊失真单频音电平错误
#define AUDIO_VQE_CHECKPARAM_ERR_AGCNORMALDISTLEVEL (-394)  // AGC一般失真单频音电平错误
#define AUDIO_VQE_CHECKPARAM_ERR_AGCNOISETHRESHOLD (-395)   // AGC噪声阈值错误
#define AUDIO_VQE_CHECKPARAM_ERR_AGCNOISETHRESHOLD2 (-396)  // AGC噪声阈值错误2
#define AUDIO_VQE_CHECKPARAM_ERR_AGCSNRSWITCH (-397)        // AGC提高信噪比开关错误
#define AUDIO_VQE_CHECKPARAM_ERR_AGCFREQGAINUPSTEP (-398)   // AGC频域增益上升步长错误
#define AUDIO_VQE_CHEKCPARAM_ERR_AGCFREQGAINDOWNSTEP (-399) // AGC频域增益下降步长错误
#define AUDIO_VQE_CHECKPARAM_ERR_AGCNOISSPFACTOR (-400)     // AGC噪声抑制系数错误
#define AUDIO_VQE_CHECKPARAM_ERR_AGCBURRTHRESHOLD (-401)    // AGC安静门限值错误
#define AUDIO_VQE_CHECKPARAM_ERR_AGCBURRPERIOD (-402)       // AGC毛刺持续时间错误
#define AUDIO_VQE_CHECKPARAM_ERR_AGCLEVELALPHA (-403)       // AGC小信号电平平滑系数错误
#define AUDIO_VQE_CHEKCPARAM_ERR_AGCGAINALPHA (-404)        // AGC小信号增益系数错误
#define AUDIO_VQE_CHECKPARAM_ERR_AGCSWITCH (-405)           // AGC增益控制开关错误
// EQ
#define AUDIO_VQE_CHECKPARAM_ERR_EQBANDGAIN (-621) // EQ频段增益错误
// DMNR
#define AUDIO_VQE_CHECKPARAM_ERR_DMNRMINGAINLIMIT (-491)              // DMNR最大降噪力度配置错误
#define AUDIO_VQE_CHECKPARAM_ERR_DMNRSNRPRIORLIMIT (-492)             // DMNR最大先验噪声抑制力度错误
#define AUDIO_VQE_CHECKPARAM_ERR_DMNRVADTHRESHOLD (-493)              // DMNRVAD判定阈值错误
#define AUDIO_VQE_CHECKPARAM_ERR_DMNRHARMONICSEARCH (-494)            // DMNR谐波搜索阈值错误
#define AUDIO_VQE_CHECKPARAM_ERR_DMNRNOISEESTMODE (-495)              // DMNR预置单麦算法选择错误
#define AUDIO_VQE_CHECKPARAM_ERR_DMNRPRIORSNRTHRESHOLD (-496)         // DMNR瞬时信噪比阈值错误
#define AUDIO_VQE_CHECKPARAM_ERR_DMNRMULTIMICPRIORSNRTHRESHOLD (-497) // DMNR多麦平滑信噪比阈值错误
#define AUDIO_VQE_CHECKPARAM_ERR_DMNRPRIOSNR (-498)                   // 维纳滤波SmPrioSNRThr配置错误
#define AUDIO_VQE_CHECKPARAM_ERR_DMNRSNRSMFACTOR (-499)               // 维纳滤波SmPrioSNRThr配置错误
#define AUDIO_VQE_CHECKPARAM_ERR_D_MNR_SPPSMFACTOR (-500)             // 维纳滤波SmPrioSNRThr配置错误
#define AUDIO_VQE_CHECKPARAM_ERR_DMNRNOISEPWRSMFACTOR (-501)          // 维纳滤波SmPrioSNRThr配置错误
#define AUDIO_VQE_CHECKPARAM_ERR_DMNRREFNOISEPWRSMFACTOR (-502)       // 维纳滤波SmPrioSNRThr配置错误
#define AUDIO_VQE_CHECKPARAM_ERR_DMNRMICPROCNUM (-503)                // mic处理个数配置错误
// SMNR
#define AUDIO_VQE_CHECKPARAM_ERR_SMNRMINGAINLIMIT (-591)              // DMNR最大降噪力度配置错误
#define AUDIO_VQE_CHECKPARAM_ERR_SMNRSNRPRIORLIMIT (-592)             // DMNR最大先验噪声抑制力度错误
#define AUDIO_VQE_CHECKPARAM_ERR_SMNRVADTHRESHOLD (-593)              // DMNRVAD判定阈值错误
#define AUDIO_VQE_CHECKPARAM_ERR_SMNRHARMONICSEARCH (-594)            // DMNR谐波搜索阈值错误
#define AUDIO_VQE_CHECKPARAM_ERR_SMNRNOISEESTMODE (-595)              // DMNR预置单麦算法选择错误
#define AUDIO_VQE_CHECKPARAM_ERR_SMNRPRIORSNRTHRESHOLD (-596)         // DMNR瞬时信噪比阈值错误
#define AUDIO_VQE_CHECKPARAM_ERR_SMNRMULTIMICPRIORSNRTHRESHOLD (-597) // DMNR多麦平滑信噪比阈值错误
#define AUDIO_VQE_CHECKPARAM_ERR_SMNRPRIOSNR (-598)                   // 维纳滤波SmPrioSNRThr配置错误
#define AUDIO_VQE_CHECKPARAM_ERR_SMNRSNRSMFACTOR (-599)               // 维纳滤波SmPrioSNRThr配置错误
#define AUDIO_VQE_CHECKPARAM_ERR_S_MNR_SPPSMFACTOR (-600)             // 维纳滤波SmPrioSNRThr配置错误
#define AUDIO_VQE_CHECKPARAM_ERR_SMNRNOISEPWRSMFACTOR (-601)          // 维纳滤波SmPrioSNRThr配置错误
#define AUDIO_VQE_CHECKPARAM_ERR_SMNRREFNOISEPWRSMFACTOR (-602)       // 维纳滤波SmPrioSNRThr配置错误
#define AUDIO_VQE_CHECKPARAM_ERR_SMNRMICPROCNUM (-603)                // mic处理个数配置错误
// DRC
#define AUDIO_VQE_CHECKPARAM_ERR_DRCFIXGAIN (-666)          // DRC固定增益配置错误
#define AUDIO_VQE_CHECKPARAM_ERR_DRCLEVELATTACKTIME (-667)  // DRC包络下降时间配置错误
#define AUDIO_VQE_CHECKPARAM_ERR_DRCLEVELRELEASETIME (-668) // DRC包络上升时间配置错误
#define AUDIO_VQE_CHECKPARAM_ERR_DRCGAINATTACKTIME (-669)   // DRC增益下降时间配置错误
#define AUDIO_VQE_CHECKPARAM_ERR_DRCGAINRELEASETIME (-670)  // DRC增益上升时间配置错误
#define AUDIO_VQE_CHECKPARAM_ERR_DRCOLDLEVEL (-672)         // DRC调整前的拐点电平配置错误
#define AUDIO_VQE_CHECKPARAM_ERR_DRCNEWLEVEL (-671)         // DRC调整后的拐点电平配置错误
#define AUDIO_VQE_CHECKPARAM_ERR_DRCCORNERLEVEL (-673)      // DRC预留前两位电平配置错误
// Dereverb
#define AUDIO_VQE_CHECKPARAM_ERR_DEREVERBDELAY (-747) // Dereverb延迟配置错误
#define AUDIO_VQE_CHECKPARAM_ERR_DEREVERBBLOCK (-748) // Dereverb块数配置错误
// LIMITER
#define AUDIO_VQE_CHECKPARAM_ERR_LIMITERMAXLEVEL (-816)      // Limiter最大电平配置错误
#define AUDIO_VQE_CHECKPARAM_ERR_LIMITERMUTETHRESHOLD (-817) // Limiter小信号电平阈值配置错误
#define AUDIO_VQE_CHECKPARAM_ERR_LIMITERSMALLSWITCH (-818)   // Limiter小信号开关配置错误
// BF
#define AUDIO_VQE_CHECKPARAM_ERR_BFMICNUM (-921)      // BF麦克风个数配置错误
#define AUDIO_VQE_CHECKPARAM_ERR_BFMICDISTANCE (-922) // BF麦克风间距配置错误
#define AUDIO_VQE_CHECKPARAM_ERR_BFANGLELEFT (-923)   // BF麦克风左边界角度配置错误
#define AUDIO_VQE_CHECKPARAM_ERR_BFANGLERIGHT (-924)  // BF麦克风右边界角度配置错误
#define AUDIO_VQE_CHECKPARAM_ERR_BFGSCENABLE (-925)   // BFgsc滤波开关配置错误
// Music Detection
#define AUDIO_VQE_CHECKPARAM_ERR_MDFEATUREFRAME (-881) // MDFeatureFrame配置错误
/* 算法模块返回码偏移量 */
#define AUDIO_VQE_AEC_BIAS (-200)         // aec return value bias -200 ~ -300
#define AUDIO_VQE_FMP_BIAS (-300)         // fmp return value bias -300 ~ -350
#define AUDIO_VQE_AGC_BIAS (-350)         // agc return value bias -350 ~ -400
#define AUDIO_VQE_DMRNR_BIAS (-450)       // dmnr return value bias -400 ~ -450
#define AUDIO_VQE_SMNR_BIAS (-550)        // nr return value bias -550 ~ -620
#define AUDIO_VQE_EQ_BIAS (-620)          // eq return value bias -620 ~ -650
#define AUDIO_VQE_DRC_BIAS (-650)         // drc return value bias -650 ~ -700
#define AUDIO_VQE_DEREVERB_BIAS (-700)    // dereverb return value bias -700 ~ -800
#define AUDIO_VQE_LIMITER_BIAS (-800)     // limiter return value bias -800 ~ -850
#define AUDIO_VQE_MUSICDETECT_BIAS (-850) // music detection return value bias -850 ~ -900
#define AUDIO_VQE_BF_BIAS (-900)          // bf return value bias -900 ~
#define AUDIO_VQE_AINR_BIAS (-1000)       // AINR bias
#define AUDIO_VQE_MVDR_BIAS (-750)        // MVDR bias
/* 产品ID */
typedef enum audio_vqe_voice_pro_ver {
    AUDIO_VQE_VER_OSCA = 0,
    AUDIO_VQE_VER_HEGE_560_570,
    AUDIO_VQE_VER_HEGE_550,
    AUDIO_VQE_VER_LOK,
    AUDIO_VQE_VER_LOKXY,
    AUDIO_VQE_VER_PLATO,
    AUDIO_VQE_VER_MAX,
} audio_vqe_voice_pro_ver;

typedef struct tag_param_struct {
    // 保留位
    td_s16 reserve0[AUDIO_VQE_COM_PAD_8];
    // 输入声道数; 范围:[1,14]; 默认值:2; 错误码:-22;
    td_u8 mic_num;
    // 算法处理输入个数; 范围:[1,8]; 默认值:2; 错误码:-23;
    // 依赖关系: micProcNum <= micNum;
    td_u8 mic_proc_num;
    // 参考个数; 范围:[0,12]; 默认值:2; 错误码:-24;
    td_u8 ref_num;
    // 算法处理参考个数; 范围:[0,12]; 默认值:2; 错误码:-25;
    // 依赖关系: refProcNum <= refNum;
    td_u8 ref_proc_num;
    // AEC线性滤波个数; 范围:[1,8]; 默认值:2; 错误码:-26;
    // 依赖关系: linOutNum <= micProcNum;
    td_u8 lin_out_num;
    // 输出声道个数; 范围:[1,8]; 默认值:1; 错误码:-27;
    // 依赖关系: outNum <= micProcNum;
    td_u8 out_num;
    // 保留位
    td_s8 init_mic_gain;
    // 模式, 1-录音模式, 2-通话模式; 范围:{1,2}; 默认值:2; 错误码:-28;
    td_u8 mode;

    // 输入信号采样率; 范围:{16000,48000}; 默认值:16000; 错误码:-29;
    td_u32 mic_in_sample_rate;
    // 采样率; 范围:{16000,48000}; 默认值:16000; 错误码:-29;
    td_u32 sample_rate;

    // 保留位
    td_u16 up_fft_size;
    // mic选择使能开关; 范围:{0,1}; 默认值:0; 错误码:-30;
    td_u8 mic_select;
    // ref选择使能开关; 范围:{0,1}; 默认值:0; 错误码:-31;
    td_u8 ref_select;
    // 音乐检测使能开关; 范围:{0,1}; 默认值:0; 错误码:-43;
    // 保留位
    td_u8 post_gain;
    // 保留位
    td_u8 reserve1[AUDIO_VQE_COM_PAD_3];
    // mic选择序号1; 范围:[0,7]; 默认值:0; 错误码:-32; BitLength:4; 符号类型: unsigned;
    // mic选择序号2; 范围:[0,7]; 默认值:0; 错误码:-32; BitLength:4; 符号类型: unsigned;
    // mic选择序号3; 范围:[0,7]; 默认值:0; 错误码:-32; BitLength:4; 符号类型: unsigned;
    // mic选择序号4; 范围:[0,7]; 默认值:0; 错误码:-32; BitLength:4; 符号类型: unsigned;
    // mic选择序号5; 范围:[0,7]; 默认值:0; 错误码:-32; BitLength:4; 符号类型: unsigned;
    // mic选择序号6; 范围:[0,7]; 默认值:0; 错误码:-32; BitLength:4; 符号类型: unsigned;
    // mic选择序号7; 范围:[0,7]; 默认值:0; 错误码:-32; BitLength:4; 符号类型: unsigned;
    // mic选择序号8; 范围:[0,7]; 默认值:0; 错误码:-32; BitLength:4; 符号类型: unsigned;
    // 依赖关系: micSelect == 1;
    td_u32 mic_idx;
    // ref选择序号1; 范围:[0,7]; 默认值:0; 错误码:-33; BitLength:4; 符号类型: unsigned;
    // ref选择序号2; 范围:[0,7]; 默认值:0; 错误码:-33; BitLength:4; 符号类型: unsigned;
    // ref选择序号3; 范围:[0,7]; 默认值:0; 错误码:-33; BitLength:4; 符号类型: unsigned;
    // ref选择序号4; 范围:[0,7]; 默认值:0; 错误码:-33; BitLength:4; 符号类型: unsigned;
    // ref选择序号5; 范围:[0,7]; 默认值:0; 错误码:-33; BitLength:4; 符号类型: unsigned;
    // ref选择序号6; 范围:[0,7]; 默认值:0; 错误码:-33; BitLength:4; 符号类型: unsigned;
    // ref选择序号7; 范围:[0,7]; 默认值:0; 错误码:-33; BitLength:4; 符号类型: unsigned;
    // ref选择序号8; 范围:[0,7]; 默认值:0; 错误码:-33; BitLength:4; 符号类型: unsigned;
    // 依赖关系: refSelect == 1;
    td_u32 ref_idx;

    // AIWNR使能开关; 范围:{0,1}; 默认值:1; 错误码5;
    td_u8 aiwnr_enable;
	// NNSPK使能开关
    td_u8 nnspk_enable;
    // AEC使能开关; 范围:{0,1}; 默认值:1; 错误码:-34;
    td_u8 aec_enable;
    // NR使能开关; 范围:{0,1}; 默认值:1; 错误码:-35;
    td_u8 dmnr_enable;
	// AGC使能开关; 范围:{0,1}; 默认值:1; 错误码:-58;
    td_u8 agc_enable;
    td_u8 fmp_enable;
    // 保留位
    td_u8 reserve4[AUDIO_VQE_COM_PAD_2];

	// ####### AEC参数
    // 非线性开关; 范围:{0,1}; 默认值:1; 错误码:-231;
    td_u8 switch_nlp;
    // 滤波器个数; 范围:{1,2}; 默认值:2; 错误码:-232;
    td_u8 filter_num;
    // 并行滤波开关; 范围:{0,1}; 默认值:0; 错误码:-233;
    td_u8 all_parallel;
    // 参考改变开关; 范围:{0,1,2}; 默认值:0; 错误码:-234;
    td_u8 x_change;
    // 参考增益值[2, 25600]Q8，实际范围[0.01,100] 默认值:256; 错误码:-235;
    td_s16 x_change_gain;
    // 纯时延; 范围:[0,300]; 默认值:0; 错误码:-236;
    td_u16 us_pure_delay;
    // 高频保护分界点, 单位:Hz; 范围:[0,8000]; 默认值:0; 错误码:-245;
    td_u16 us_max_hidx;
    // 近端增益值1, 单位:0.01; 范围:[1,10000]; 默认值:100; 错误码:-248;
    td_u16 us_zoom_factor1;
    // 近端增益值2, 单位:0.01; 范围:[1,10000]; 默认值:100; 错误码:-249;
    td_u16 us_zoom_factor2;
    // 二次滤波开关1; 范围:{0,1}; 默认值:1; 错误码:-246;
    td_u8 post_filter;
    // 二次滤波开关2; 范围:{0,1}; 默认值:1; 错误码:-247;
    td_u8 post_filter2;
    // nlms1 block个数; 范围:[0,30]; 默认值:8; 错误码:-250;
    td_u8 nlms_block_num1;
    // nlms2 block个数; 范围:[0,30]; 默认值:8; 错误码:-251;
    td_u8 nlms_block_num2;
    // nlms1参考序列号; 范围:[0,12]; 默认值:0; 错误码:-254;
    // 依赖关系: ucNlmsRef1 < refProcNum;
    td_u8 nlms_ref1;
    // nlms2参考序列号; 范围:[0,12]; 默认值:0; 错误码:-255;
    // 依赖关系: ucNlmsRef1 < refProcNum;
    td_u8 nlms_ref2;
	// 保留位
    td_s32 res;
    // 超大回声场景门限
    td_u16 xd_b_th_hh;
    // 混响时间帧数
    td_u16 frme_cnt_reverb_th;
    // F0计算频率下限; 范围:[0,8000]; 默认值:300; 错误码:-258;
    // 依赖关系:usF0Fre1 < usF0Fre2;
    td_u16 us_f0_fre1;
    // F0计算频率上限; 范围:[0,8000]; 默认值:1200; 错误码:-259;
    td_u16 us_f0_fre2;
    // 保留位
    td_u8 e_db_f0_th;
    // 高频强抑制模式; 范围:{0,1}; 默认值:1; 错误码:-260;
    td_u8 high_proc_flag;
    // 双讲低频估计残留回声系数, [1,10240]->Q10, real range: [0.001,10]; default:1024; error code:-261;
    td_s16 dt_zoom_factor_l;
    // 双讲高频估计残留回声系数, [1,10240]->Q10, real range: [0.001,10]; default:1024; error code:-262;
    td_s16 dt_zoom_factor_h;
    // 单讲估计残留回声系数, [2,25600]->Q8, real range: [0.01,100]; default:1280; error code:-263;
    td_s16 sd_zoom_factor;
    // 并行滤波block数; 范围:[0,60]; 默认值:30; 错误码:-264;
    // 依赖关系:ucAllParallelBlockNum * refProcNum < 60;
    td_u8 all_parallel_block_num;
    // 谐波增强幅度, 单位:0.1; 范围:[10,50]; 默认值:50; 错误码:-265;
    td_u8 cepamp;
    // 低频残留回声过估校正系数, [1,10240]->Q10, real range: [0.001,10]; default:1024; error code:-266;
    td_s16 res_zoom_factor_l;
    // 高频残留回声过估校正系数, [1,30720]->Q10, real range: [0.001,30]; default:1024; error code:-267;
    td_s16 res_zoom_factor_h;
    // 快速滤波器开关; 范围:{0,1}; 默认值:1; 错误码:-268;
    td_u16 us_fast_filter_switch;

    // ####### PMWF
    // 最大允许的噪声抑制力度; 范围:[1,32767]; 默认值:3276; 错误码:-491;
    td_s16 min_gain_limit;
    // 最大先验噪声抑制力度; 范围:[1,32767]; 默认值:1080; 错误码:-492;
    td_s16 snr_prior_limit;
    // VAD判定阈值; 范围:[0,80]; 默认值:10;错误码:-493;
    td_s16 ht_thrd;
    // 谐波搜索阈值; 范围:[100,1100]; 默认值:120; 错误码:-494;
    td_s16 hs_thrd;
    // 预置单麦算法选择, 0:DSA, 1:MMHH, 2:MMNR; 范围:{0,1,2}; 默认值:0; 错误码:-495;
    td_s16 pri_noise_esti_mode;
    // 瞬时信噪比dB; 范围:[1,60]; 默认值:5; 错误码:-496;
    td_s16 prio_snr_thr;
    // 多麦平滑信噪比dB; 范围:[2,100]; 默认值:10; 错误码:-497;
    td_s16 sm_prio_snr_thr;
    // 固定先验信噪比; 范围:[0,16];  默认值:5;  错误码:-498;
    td_s16 fixed_prior_snr;
    // 后验信噪比平滑系数;  Q15:[3277, 32767], real range:[0.1, 1]  默认值:21843;  错误码:-499;
    td_u16 post_snr_smooth_factor;
    // 语音概率平滑系数; Q15:[3277, 32767], real range:[0.1, 1]  默认值:32440;  错误码:-500;
    td_u16 spp_smooth_factor;
    // 噪声能量平滑系数;  Q15:[3277, 32767], real range:[0.1, 1]  默认值:29799; 错误码:-501;
    td_u16 noise_pwr_smooth_factor;
    // 大回声场合 噪声能量平滑系数;  Q15:[3277, 32767], real range:[0.1, 1]  默认值:9990; 错误码:-502;
    td_s16 refnoise_pwr_sm_factor;
    // NR mic处理个数; 范围:{1,2}; 默认值:2; 错误码:-503;
    td_s16 nr_mic_proc_num;
    // 保留位
    td_s16 reserve11[AUDIO_VQE_COM_PAD_11];

    // ####### AGC
    // target_level 目标电平: 范围[-30 ~ 30]d_b; 错误码: -383;
    td_s16 target_level;
    // max_gain     最大增益: 范围[0 ~ 60]d_b; 错误码: -384;
    td_s16 max_gain;
    // min_gain     最小增益: 范围[-30 ~ 0 ]d_b; 错误码: -385;
    td_s16 min_gain;
    // up_gradient_ratio 抬升速度: [1 ~ 30]; 错误码: -386;
    td_s16 up_gradient_ratio;
    // down_gradient_ratio 压制速度: [1 ~ 30]; 错误码: -387;
    td_s16 down_gradient_ratio;
    // decay       噪声压制力度: -65; 错误码: -388;
    td_s16 decay;
    // fast_envelop_param  快包络控制参数: 范围[0~65536.0]; 错误码: -389;
    td_s32 fast_envelop_param;
    // slow_envelop_param  慢包络控制参数: 范围[0~65536.0]; 错误码: -390;
    td_s32 slow_envelop_param;
    // vad_threshold_prob     vad语音概率控制参数: 范围[0~1]; 错误码: -391;
    td_s32 vad_threshold_prob;
    // noise_threshold_prob   噪声概率控制参数: 范围[0        ~1]; 错误码: -392;
    td_s32 noise_threshold_prob;
    // 保留区,保留3字节
    td_u16 reserve9[2];

    /* fmp */
    td_s8 comfort_flag; /* flag of noise compensation 1->open, 0->close */
    td_s8 comfort_intensity;       /* noise compensation level, 1-10, default:3 */
    td_s8 mode2;                   /* no-echo_area, 0->headset, 1->handsfree, 2->headphone */
    td_s8 fmp_reserved[5];         /* reserved */
} audio_vqe_param_struct;


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __SEA_AEF_H__ */
