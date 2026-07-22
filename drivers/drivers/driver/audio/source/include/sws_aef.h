/*
 * Copyright (c) CompanyNameMagicTag 2021-2022. All rights reserved.
 * Description: header of sws aef
 * Author: audio
 * Create: 2021-12-30
 */

#ifndef __SWS_AEF_H__
#define __SWS_AEF_H__

#ifdef __cplusplus
extern "C" {
#endif

#define AUDIO_HISTEN_MOBILE_EQ_MAXBANDS 12
#define AUDIO_HISTEN_MOBILE_CH_MAX 2

/* 子模块开关 */
/* Start */
#define AUDIO_HISTEN_MOBILE_MODULE_START 0
/* Sound Stage Width 声场扩展 */
#define AUDIO_HISTEN_MOBILE_SSW (1)
/* Graphic Equalizer-均衡 */
#define AUDIO_HISTEN_MOBILE_GEQ (1 << 1)
/* Dynamic Range Compressor-动态范围控制 */
#define AUDIO_HISTEN_MOBILE_DRC (1 << 2)
/* Limiter-限幅器 */
#define AUDIO_HISTEN_MOBILE_LMT (1 << 3)
/* End */
#define AUDIO_HISTEN_MOBILE_MODULE_END (1 << 4)

typedef struct {
    /* 组件选择; 范围:[0,32767];默认值:0;错误码:-85 */
    short module;
    /* Surround类型,高8位, 0宽广,1前置,2关闭,3宏大; 范围:[0,3];默认值:0;错误码:-101; BitLenth:8; 符号类型:signed; */
    /* 反射强度; 范围:[0,100];默认值:85;错误码:-102; BitLenth:8; 符号类型:signed; */
    short x2b_surround;
    /* 声场增度比例  ; 范围:[0,100];默认值:3;错误码:-100 */
    short x2b_sound_width;

    /* EQ预增益,偏移100, 单位0.1dB; */
    /* EQ预增益1;范围:[0,200];默认值:100;错误码:-141; BitLenth:8; 符号类型:unsigned; */
    /* EQ预增益2;范围:[0,200];默认值:100;错误码:-141; BitLenth:8; 符号类型:unsigned; */
    short eq_pre_gain_lr;
    /* EQ段数L; 范围:[0,12];默认值:3;错误码:-142; BitLenth:8; 符号类型:signed; */
    /* EQ段数R; 范围:[0,12];默认值:3;错误码:-142; BitLenth:8; 符号类型:signed; */
    short eq_bands_lr;
    /* EQ类型1; 范围:[0,6];默认值:0;错误码:-143; BitLenth:8; 符号类型:signed; */
    /* EQ类型2; 范围:[0,6];默认值:0;错误码:-143; BitLenth:8; 符号类型:signed; */
    short eq_type_lr[AUDIO_HISTEN_MOBILE_EQ_MAXBANDS];
    /* EQ增益1,偏移100, 单位0.1dB; 范围:[0,200];默认值:100;错误码:-144; BitLenth:8; 符号类型:unsigned; */
    /* EQ增益2,偏移100, 单位0.1dB; 范围:[0,200];默认值:100;错误码:-144; BitLenth:8; 符号类型:unsigned; */
    short eq_gain_lr[AUDIO_HISTEN_MOBILE_EQ_MAXBANDS];
    /* EQ Q值,Type为1/2时,此值失效; 范围:[100,10000];默认值:1414;错误码:-145 */
    short eq_q[AUDIO_HISTEN_MOBILE_CH_MAX][AUDIO_HISTEN_MOBILE_EQ_MAXBANDS];
    /* EQ Fc,Type为1时，范围[20, 1100]; 范围:[20,20000];默认值:1000;错误码:-146 */
    short eq_fc[AUDIO_HISTEN_MOBILE_CH_MAX][AUDIO_HISTEN_MOBILE_EQ_MAXBANDS];

    /* DRC增益; 范围:[0,18];默认值:3;错误码:-180; */
    short drc_gain;
    /* DRC并行增益,高低8位,单位0.01 */
    /* DRC并行增益1; 范围:[0,250];默认值:100;错误码:-181; BitLenth:8; 符号类型:unsigned; */
    /* DRC并行增益2; 范围:[0,250];默认值:100;错误码:-181; BitLenth:8; 符号类型:unsigned; */
    short drc_paral_glr;
    /* DRC Lookahead长度,单位0.05ms; */
    /* DRC Lookahead长度; 范围:[0,200];默认值:0;错误码:-182; */
    short drc_looka_lr;

    /* Limiter threshold, 偏移200, 单位0.1dB 范围:[0,200];默认值:190;错误码:-210 */
    short lmt_th;
} audio_sws_mobile_para;

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __SWS_AEF_H__ */
