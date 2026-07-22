/*
 * Copyright (c) CompanyNameMagicTag 2021-2021. All rights reserved.
 * Description: afe_reg.h for DallasV100
 * Author: audio
 */

#ifndef __REG_AFE_H__
#define __REG_AFE_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define AFE_BIT_SET 0x1
#define AFE_BIT_RESET 0x0
#define AFE_DELAY_10US 10
#define AFE_DELAY_20US 20
#define AFE_DELAY_50US 50
#define AFE_DELAY_100US 100
#define AFE_DELAY_120US 120
#define AFE_DELAY_500US 500
#define AFE_DELAY_1MS 1000
#define AFE_DELAY_5MS 5000
#define AFE_DELAY_6MS 6000
#define AFE_DELAY_10MS 10000

#define AFE_PGA_GAIN_MAX 60
#define AFE_PGA_GAIN_MIN (-120)

#define AFE_BOOST_ENABLE 0x1
#define AFE_BOOST_DISABLE 0x0
#define AFE_ADC_ANA_BOOST_LEVEL 14
#define AFE_LPADC_ANA_BOOST_LEVEL 26

#define AFE_ADC_ANA_PGA_GAIN_MAX 36
#define AFE_ADC_ANA_PGA_GAIN_MIN 0
#define AFE_ADC_ANA_PGA_GAIN_STEP 2

#define AFE_LPADC_ANA_PGA_GAIN_MAX 24
#define AFE_LPADC_ANA_PGA_GAIN_MIN 0
#define AFE_LPADC_ANA_PGA_GAIN_STEP 3

#define AFE_ADC_ANA_GAIN_MAX (AFE_ADC_ANA_BOOST_LEVEL + AFE_ADC_ANA_PGA_GAIN_MAX)
#define AFE_ADC_ANA_GAIN_MIN (AFE_ADC_ANA_PGA_GAIN_MIN)

#define AFE_LPADC_ANA_GAIN_MAX (AFE_LPADC_ANA_BOOST_LEVEL + AFE_LPADC_ANA_PGA_GAIN_MAX)
#define AFE_LPADC_ANA_GAIN_MIN (AFE_LPADC_ANA_PGA_GAIN_MIN)

#define AFE_FIFO_AFULL_TH_DEFAULT 0x7
#define AFE_FIFO_AEMPTY_TH_DEFAULT 0x1

#define AFE_PGA_ZERO_NUM_DEFAULT 20

#define AFE_ADC_CIC_GAIN_DEFAULT 0x2D
#define AFE_ADC_HPF_DC_OFFSET_DEFAULT 0x000000

#define calc_reg_num(a, b)  (((b - a) >> 2) - 1)

/* Define the union u_codec_clk_en0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    reserved_0            : 1   ; /* [0] */
        unsigned int    reserved_1            : 1   ; /* [1] */
        unsigned int    reserved_2            : 1   ; /* [2] */
        unsigned int    reserved_3            : 1   ; /* [3] */
        unsigned int    reserved_4            : 1   ; /* [4] */
        unsigned int    reserved_5            : 1   ; /* [5] */
        unsigned int    reserved_6            : 1   ; /* [6] */
        unsigned int    reserved_7            : 1   ; /* [7] */
        unsigned int    reserved_8            : 1   ; /* [8] */
        unsigned int    reserved_9            : 1   ; /* [9] */
        /* mic2上行FIFO时钟控制。       0：关闭时钟；      1：打开时钟。      */
        unsigned int    mic2_up_afifo_clken   : 1   ; /* [10] */
        /* mic3上行FIFO时钟控制。       0：关闭时钟；      1：打开时钟。      */
        unsigned int    mic3_up_afifo_clken   : 1   ; /* [11] */
        unsigned int    reserved_10           : 1   ; /* [12] */
        unsigned int    reserved_11           : 1   ; /* [13] */
        unsigned int    reserved_12           : 1   ; /* [14] */
        unsigned int    reserved_13           : 1   ; /* [15] */
        /* mad上行FIFO的时钟控制        0：关闭时钟；      1：打开时钟。      */
        unsigned int    mad_up_afifo_clken    : 1   ; /* [16] */
        unsigned int    reserved_14           : 1   ; /* [17] */
        unsigned int    reserved_15           : 1   ; /* [18] */
        unsigned int    reserved_16           : 1   ; /* [19] */
        unsigned int    reserved_17           : 1   ; /* [20] */
        unsigned int    reserved_18           : 1   ; /* [21] */
        unsigned int    reserved_19           : 1   ; /* [22] */
        /* mic2 PGA的时钟控制。      0：关闭时钟；      1：打开时钟。      */
        unsigned int    mic2_pga_clken        : 1   ; /* [23] */
        /* mic3 PGA的时钟控制。      0：关闭时钟；      1：打开时钟。      */
        unsigned int    mic3_pga_clken        : 1   ; /* [24] */
        unsigned int    reserved_20           : 1   ; /* [25] */
        unsigned int    reserved_21           : 1   ; /* [26] */
        unsigned int    reserved_22           : 1   ; /* [27] */
        unsigned int    reserved_23           : 1   ; /* [28] */
        unsigned int    reserved_24           : 1   ; /* [29] */
        /* mad_adc_r  PGA的时钟控制。      0：关闭时钟；      1：打开时钟。      */
        unsigned int    mad_adc_r_pga_clken   : 1   ; /* [30] */
        unsigned int    reserved_25           : 1   ; /* [31] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_codec_clk_en0;

/* Define the union u_codec_clk_en1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    reserved_0            : 1   ; /* [0] */
        unsigned int    reserved_1            : 1   ; /* [1] */
        unsigned int    reserved_2            : 1   ; /* [2] */
        unsigned int    reserved_3            : 1   ; /* [3] */
        unsigned int    reserved_4            : 1   ; /* [4] */
        unsigned int    reserved_5            : 1   ; /* [5] */
        /* mic2_srcdn时钟控制。     0：关闭时钟；      1：打开时钟。      */
        unsigned int    mic2_srcdn_clken      : 1   ; /* [6] */
        /* mic3_srcdn时钟控制。     0：关闭时钟；      1：打开时钟。      */
        unsigned int    mic3_srcdn_clken      : 1   ; /* [7] */
        unsigned int    reserved_6            : 1   ; /* [8] */
        unsigned int    reserved_7            : 1   ; /* [9] */
        unsigned int    reserved_8            : 1   ; /* [10] */
        unsigned int    reserved_9            : 1   ; /* [11] */
        unsigned int    reserved_10           : 1   ; /* [12] */
        /* mad_adc_r_srcdn时钟控制。     0：关闭时钟；      1：打开时钟。      */
        unsigned int    mad_adc_r_srcdn_clken : 1   ; /* [13] */
        unsigned int    reserved_11           : 1   ; /* [14] */
        unsigned int    reserved_12           : 1   ; /* [15] */
        /* mad_src_r时钟控制。     0：关闭时钟；      1：打开时钟。      */
        unsigned int    mad_src_r_clken       : 1   ; /* [16] */
        unsigned int    reserved_13           : 1   ; /* [17] */
        unsigned int    reserved_14           : 1   ; /* [18] */
        unsigned int    reserved_15           : 1   ; /* [19] */
        unsigned int    reserved_16           : 1   ; /* [20] */
        unsigned int    reserved_17           : 1   ; /* [21] */
        unsigned int    reserved_18           : 1   ; /* [22] */
        unsigned int    reserved_19           : 1   ; /* [23] */
        unsigned int    reserved_20           : 1   ; /* [24] */
        unsigned int    reserved_21           : 1   ; /* [25] */
        /* mic2_adc时钟控制。     0：关闭时钟；      1：打开时钟。      */
        unsigned int    mic2_adc_clken        : 1   ; /* [26] */
        /* mic3_adc时钟控制。     0：关闭时钟；      1：打开时钟。      */
        unsigned int    mic3_adc_clken        : 1   ; /* [27] */
        unsigned int    reserved_22           : 1   ; /* [28] */
        unsigned int    reserved_23           : 1   ; /* [29] */
        unsigned int    reserved_24           : 1   ; /* [30] */
        unsigned int    reserved_25           : 1   ; /* [31] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_codec_clk_en1;

/* Define the union u_codec_clk_en2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    reserved_0            : 1   ; /* [0] */
        unsigned int    reserved_1            : 1   ; /* [1] */
        /* mad_adc_r_cic时钟控制。     0：关闭时钟；      1：打开时钟。      */
        unsigned int    mad_adc_r_cic_clken   : 1   ; /* [2] */
        unsigned int    reserved_2            : 1   ; /* [3] */
        /* mad_adc_r_hpf时钟控制。     0：关闭时钟；      1：打开时钟。      */
        unsigned int    mad_adc_r_hpf_clken   : 1   ; /* [4] */
        /* mad时钟控制。     0：关闭时钟；      1：打开时钟。      */
        unsigned int    mad_clken             : 1   ; /* [5] */
        /* mad_buf时钟控制。     0：关闭时钟；      1：打开时钟。      */
        unsigned int    mad_buf_clken         : 1   ; /* [6] */
        unsigned int    reserved_3            : 1   ; /* [7] */
        unsigned int    reserved_4            : 1   ; /* [8] */
        unsigned int    reserved_5            : 1   ; /* [9] */
        unsigned int    reserved_6            : 1   ; /* [10] */
        unsigned int    reserved_7            : 1   ; /* [11] */
        unsigned int    reserved_8            : 1   ; /* [12] */
        unsigned int    reserved_9            : 1   ; /* [13] */
        unsigned int    reserved_10           : 1   ; /* [14] */
        /* dmic0时钟控制。     0：关闭时钟；      1：打开时钟。      */
        unsigned int    dmic0_clken           : 1   ; /* [15] */
        unsigned int    reserved_11           : 1   ; /* [16] */
        unsigned int    reserved_12           : 1   ; /* [17] */
        /* codec_ana时钟控制。     0：关闭时钟；      1：打开时钟。      */
        unsigned int    codec_ana_clken       : 1   ; /* [18] */
        /* 双耳同步时钟控制。       0：关闭时钟；      1：打开时钟。      */
        unsigned int    double_ear_clken      : 1   ; /* [19] */
        unsigned int    calib_eq_clken        : 1   ; /* [20] */
        unsigned int    reserved_14           : 1   ; /* [21] */
        unsigned int    reserved_15           : 1   ; /* [22] */
        /* lp_cicdn_adc2时钟控制。     0：关闭时钟；      1：打开时钟。      */
        unsigned int    lp_cicdn_adc2_clken   : 1   ; /* [23] */
        /* lp_cicdn_adc3时钟控制。     0：关闭时钟；      1：打开时钟。      */
        unsigned int    lp_cicdn_adc3_clken   : 1   ; /* [24] */
        unsigned int    reserved_16           : 1   ; /* [25] */
        unsigned int    reserved_17           : 1   ; /* [26] */
        unsigned int    reserved_18           : 1   ; /* [27] */
        unsigned int    reserved_19           : 1   ; /* [28] */
        unsigned int    reserved_20           : 1   ; /* [29] */
        unsigned int    reserved_21           : 1   ; /* [30] */
        unsigned int    reserved_22           : 1   ; /* [31] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_codec_clk_en2;

/* Define the union u_codec_sw_rst_n */
typedef union {
    /* Define the struct bits */
    struct {
        /* codec的软复位，低有效；       */
        unsigned int    codec_sw_rst_n        : 1   ; /* [0] */
        unsigned int    reserved_0            : 1   ; /* [1] */
        /* 上行4amic语音通路一键复位请求，高有效；        */
        unsigned int    rst_4amic_access_irq  : 1   ; /* [2] */
        /* 上行4dmic录音通路一键复位请求，高有效；        */
        unsigned int    rst_4dmic_access_irq  : 1   ; /* [3] */
        /* 上行8mic录音通路一键复位请求，高有效；       */
        unsigned int    rst_8mic_access_irq   : 1   ; /* [4] */
        unsigned int    reserved_1            : 1   ; /* [5] */
        unsigned int    reserved_2            : 1   ; /* [6] */
        unsigned int    reserved_3            : 1   ; /* [7] */
        unsigned int    reserved_4            : 1   ; /* [8] */
        unsigned int    postproc_clken        : 1   ; /* [9] */
        unsigned int    reserved_6            : 22  ; /* [31..10] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_codec_sw_rst_n;

/* Define the union u_i2s_inner_ctrl */
typedef union {
    /* Define the struct bits */
    struct {
        /* inner I2S采样率：2'b00: 16KHz，2'b01: 32KHz，  2'b10: 48KHz，2'b11: 96KHz */
        unsigned int    fs_i2s_inner          : 2   ; /* [1..0] */
        /* I2S_INNER接口RX方向时钟选择:
         * 1'b0:  BCLK上升沿接收数据，        1'b1:  BCLK下降沿接收数据
         */
        unsigned int    reserved_0            : 30  ; /* [31..2] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_i2s_inner_ctrl;

/* Define the union u_pga_gainoffset_ctrl1 */
typedef union {
    /* Define the struct bits */
    struct {
        /* 增益偏置，gainoffset有效配置范围是0~255
         * gain_offset的调整粒度与fadeInTime相关，映射关系为粒度= max(2^(fadeInTime -12),1)，
         * 以fadeInTime=16为例，gainOffset可取0,16,32……;
         */
        unsigned int    reserved_0            : 8   ; /* [7..0] */
        /* 增益偏置，gainoffset有效配置范围是0~255
         * gain_offset的调整粒度与fadeInTime相关，映射关系为粒度= max(2^(fadeInTime -12),1)，
         * 以fadeInTime=16为例，gainOffset可取0,16,32……;
         */
        unsigned int    reserved_1            : 8   ; /* [15..8] */
        /* 增益偏置，gainoffset有效配置范围是0~255
         * gain_offset的调整粒度与fadeInTime相关，映射关系为粒度= max(2^(fadeInTime -12),1)，
         * 以fadeInTime=16为例，gainOffset可取0,16,32……;
         */
        unsigned int    mic2_adc_pga_gainoffset : 8   ; /* [23..16] */
        /* 增益偏置，gainoffset有效配置范围是0~255
         * gain_offset的调整粒度与fadeInTime相关，映射关系为粒度= max(2^(fadeInTime -12),1)，
         * 以fadeInTime=16为例，gainOffset可取0,16,32……;
         */
        unsigned int    mic3_adc_pga_gainoffset : 8   ; /* [31..24] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_pga_gainoffset_ctrl1;

/* Define the union u_pga_gainoffset_ctrl3 */
typedef union {
    /* Define the struct bits */
    struct {
        /* 增益偏置，gainoffset有效配置范围是0~255
         * gain_offset的调整粒度与fadeInTime相关，映射关系为粒度= max(2^(fadeInTime -12),1)，
         * 以fadeInTime=16为例，gainOffset可取0,16,32……;
         */
        unsigned int    reserved_0            : 8   ; /* [7..0] */
        /* 增益偏置，gainoffset有效配置范围是0~255
         * gain_offset的调整粒度与fadeInTime相关，映射关系为粒度= max(2^(fadeInTime -12),1)，
         * 以fadeInTime=16为例，gainOffset可取0,16,32……;
         */
        unsigned int    mad_adc_r_pga_gainoffset : 8   ; /* [15..8] */
        /* 增益偏置，gainoffset有效配置范围是0~255
         * gain_offset的调整粒度与fadeInTime相关，映射关系为粒度= max(2^(fadeInTime -12),1)，
         * 以fadeInTime=16为例，gainOffset可取0,16,32……;
         */
        unsigned int    reserved_1            : 8   ; /* [23..16] */
        unsigned int    reserved_2            : 8   ; /* [31..24] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_pga_gainoffset_ctrl3;

/* Define the union u_mic2_pga_ctrl */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    reserved_0            : 1   ; /* [0] */
        /* PGA线性选择控制
         * 1'b0: 选择对数淡出淡出，        1'b1: 选择线性淡入淡出
         */
        unsigned int    mic2_pga_linear_sel   : 1   ; /* [1] */
        /* PGA过零检测数目，默认值为5'd20            */
        unsigned int    mic2_pga_zero_num     : 5   ; /* [6..2] */
        /* PGA阈值选择信号，用于过零检测。其中每个pga通过使用其中2bit的id选择四种场景的阈值
         * 0：选择pga_thre_ctrl[23:0]
         * 1：选择pga_thre_ctrl[47:24]
         * 2：选择pga_thre_ctrl[71:48]
         * 3：选择pga_thre_ctrl[95:72]
         */
        unsigned int    mic2_pga_thre_id      : 2   ; /* [8..7] */
        /* PGA噪音使能，高电平代表将噪音使能，即将抵于阈值的小信号放过去，低表示低于阈值的信号钳制到0
         * 1：不对噪音信号做任何处理，0：将低于阈值的噪音信号过滤成0
         */
        unsigned int    mic2_pga_noise_en     : 1   ; /* [9] */
        /* PGA bypass使能开关
         * 1'b0: 不bypass，  1'b1: bypass
         */
        unsigned int    mic2_pga_bypass       : 1   ; /* [10] */
        /* PGA淡出时间等级设置，实际淡入时间与等级设置、采样率相关。        */
        unsigned int    mic2_pga_fade_out     : 5   ; /* [15..11] */
        /* PGA淡入时间等级设置，实际淡入时间与等级设置、采样率相关。       */
        unsigned int    mic2_pga_fade_in      : 5   ; /* [20..16] */
        /* PGA三种功能使能开关，淡入淡出，抗削波，小信号
         * [7]：淡入淡出功能使能，        1：淡入淡出功能使能，        0：淡入淡出功能不使能
         * [6]: 抗削波功能使能，        1：抗削波功能使能，        0：抗削波功能不使能
         * [5]: 小信号功能使能，        1：小信号功能使能，        0：小信号功能不使能
         */
        unsigned int    mic2_pga_cfg          : 3   ; /* [23..21] */
        /* PGA增益配置，配置范围: -120~60dB（bit7为符号位）
         * 例如：60dB配置成0x3C；       0dB配置为0x00；    -120dB配置为0x88；
         */
        unsigned int    mic2_pga_gain         : 8   ; /* [31..24] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_mic2_pga_ctrl;

/* Define the union u_mic3_pga_ctrl */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    reserved_0            : 1   ; /* [0] */
        /* PGA线性选择控制：       1'b0: 选择对数淡出淡出，        1'b1: 选择线性淡入淡出          */
        unsigned int    mic3_pga_linear_sel   : 1   ; /* [1] */
        /* PGA过零检测数目，默认值为5'd20            */
        unsigned int    mic3_pga_zero_num     : 5   ; /* [6..2] */
        /* PGA阈值选择信号，用于过零检测。其中每个pga通过使用其中2bit的id选择四种场景的阈值
         * 0：选择pga_thre_ctrl[23:0]
         * 1：选择pga_thre_ctrl[47:24]
         * 2：选择pga_thre_ctrl[71:48]
         * 3：选择pga_thre_ctrl[95:72]
         */
        unsigned int    mic3_pga_thre_id      : 2   ; /* [8..7] */
        /* PGA噪音使能，高电平代表将噪音使能，即将抵于阈值的小信号放过去，低表示低于阈值的信号钳制到0
         * 1：不对噪音信号做任何处理，0：将低于阈值的噪音信号过滤成0
         */
        unsigned int    mic3_pga_noise_en     : 1   ; /* [9] */
        /* PGA bypass使能开关：     1'b0: 不bypass，1'b1: bypass   */
        unsigned int    mic3_pga_bypass       : 1   ; /* [10] */
        /* PGA淡出时间等级设置，实际淡入时间与等级设置、采样率相关。        */
        unsigned int    mic3_pga_fade_out     : 5   ; /* [15..11] */
        /* PGA淡入时间等级设置，实际淡入时间与等级设置、采样率相关。       */
        unsigned int    mic3_pga_fade_in      : 5   ; /* [20..16] */
        /* PGA三种功能使能开关，淡入淡出，抗削波，小信号
         * [7]：淡入淡出功能使能，        1：淡入淡出功能使能，        0：淡入淡出功能不使能
         * [6]:  抗削波功能使能，        1：抗削波功能使能，        0：抗削波功能不使能
         * [5]: 小信号功能使能，        1：小信号功能使能，        0：小信号功能不使能
         */
        unsigned int    mic3_pga_cfg          : 3   ; /* [23..21] */
        /* PGA增益配置，配置范围: -120~60dB（bit7为符号位）
         * 例如：60dB配置成0x3C；       0dB配置为0x00；    -120dB配置为0x88；
         */
        unsigned int    mic3_pga_gain         : 8   ; /* [31..24] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_mic3_pga_ctrl;

/* Define the union u_mad_adc_r_pga_ctrl */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    reserved_0            : 1   ; /* [0] */
        /* PGA线性选择控制：       1'b0: 选择对数淡出淡出，        1'b1: 选择线性淡入淡出          */
        unsigned int    mad_adc_r_pga_linear_sel : 1   ; /* [1] */
        /* PGA过零检测数目，默认值为5'd20            */
        unsigned int    mad_adc_r_pga_zero_num : 5   ; /* [6..2] */
        /* PGA阈值选择信号，用于过零检测。其中每个pga通过使用其中2bit的id选择四种场景的阈值
         * 0：选择pga_thre_ctrl[23:0]
         * 1：选择pga_thre_ctrl[47:24]
         * 2：选择pga_thre_ctrl[71:48]
         * 3：选择pga_thre_ctrl[95:72]
         */
        unsigned int    mad_adc_r_pga_thre_id : 2   ; /* [8..7] */
        /*
        PGA噪音使能，高电平代表将噪音使能，即将抵于阈值的小信号放过去，低表示低于阈值的信号钳制到0
        1：不对噪音信号做任何处理
        0：将低于阈值的噪音信号过滤成0
        */
        unsigned int    mad_adc_r_pga_noise_en : 1   ; /* [9] */
        /* PGA bypass使能开关：     1'b0: 不bypass，  1'b1: bypass */
        unsigned int    mad_adc_r_pga_bypass  : 1   ; /* [10] */
        /* PGA淡出时间等级设置，实际淡入时间与等级设置、采样率相关。       */
        unsigned int    mad_adc_r_pga_fade_out : 5   ; /* [15..11] */
        /* PGA淡入时间等级设置，实际淡入时间与等级设置、采样率相关。        */
        unsigned int    mad_adc_r_pga_fade_in : 5   ; /* [20..16] */
        /* PGA三种功能使能开关，淡入淡出，抗削波，小信号
         * [7]：淡入淡出功能使能，        1：淡入淡出功能使能，        0：淡入淡出功能不使能
         * [6]: 抗削波功能使能，        1：抗削波功能使能，        0：抗削波功能不使能
         * [5]: 小信号功能使能，        1：小信号功能使能，        0：小信号功能不使能
         */
        unsigned int    mad_adc_r_pga_cfg     : 3   ; /* [23..21] */
        /* PGA增益配置，配置范围: -120~60dB（bit7为符号位）
         * 例如：60dB配置成0x3C；       0dB配置为0x00；    -120dB配置为0x88；
         */
        unsigned int    mad_adc_r_pga_gain    : 8   ; /* [31..24] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_mad_adc_r_pga_ctrl;

/* Define the union u_srcdn_ctrl0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    reserved_0            : 3   ; /* [2..0] */
        unsigned int    reserved_1            : 1   ; /* [3] */
        unsigned int    reserved_2            : 3   ; /* [6..4] */
        unsigned int    reserved_3            : 1   ; /* [7] */
        /* SRC降采样抽取倍数配置
         * 3'b000:  3倍抽取；
         * 3'b001:  保留；
         * 3'b010:  6倍抽取；
         * 3'b011:  2倍抽取;
         * 3'b100:  1.5倍抽取;
         * others:  保留.
         */
        unsigned int    mic2_srcdn_src_mode   : 3   ; /* [10..8] */
        /* FIFO Clear Signal, high active.
         * 0：不清空FIFO数据；1：清空FIFO数据
         */
        unsigned int    mic2_srcdn_fifo_clr   : 1   ; /* [11] */
        /* SRC降采样抽取倍数配置
         * 3'b000:  3倍抽取；
         * 3'b001:  保留；
         * 3'b010:  6倍抽取；
         * 3'b011:  2倍抽取;
         * 3'b100:  1.5倍抽取;
         * others:  保留.
         */
        unsigned int    mic3_srcdn_src_mode   : 3   ; /* [14..12] */
        /* FIFO Clear Signal, high active. 0：不清空FIFO数据；1：清空FIFO数据              */
        unsigned int    mic3_srcdn_fifo_clr   : 1   ; /* [15] */
        unsigned int    reserved_4            : 3   ; /* [18..16] */
        unsigned int    reserved_5            : 1   ; /* [19] */
        unsigned int    reserved_6            : 3   ; /* [22..20] */
        unsigned int    reserved_7            : 1   ; /* [23] */
        unsigned int    reserved_8            : 3   ; /* [26..24] */
        unsigned int    reserved_9            : 1   ; /* [27] */
        unsigned int    reserved_10           : 3   ; /* [30..28] */
        unsigned int    reserved_11           : 1   ; /* [31] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_srcdn_ctrl0;

/* Define the union u_srcdn_ctrl1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    reserved_0            : 3   ; /* [2..0] */
        unsigned int    reserved_1            : 1   ; /* [3] */
        /* SRC降采样抽取倍数配置
         * 3'b000:  3倍抽取；
         * 3'b001:  保留；
         * 3'b010:  6倍抽取；
         * 3'b011:  2倍抽取;
         * 3'b100:  1.5倍抽取;
         * others:  保留.
         */
        unsigned int    mad_adc_r_srcdn_src_mode : 3   ; /* [6..4] */
        /* FIFO Clear Signal, high active. 0：不清空FIFO数据；1：清空FIFO数据             */
        unsigned int    mad_adc_r_srcdn_fifo_clr : 1   ; /* [7] */
        unsigned int    reserved_2            : 3   ; /* [10..8] */
        unsigned int    reserved_3            : 1   ; /* [11] */
        unsigned int    reserved_4            : 20  ; /* [31..12] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_srcdn_ctrl1;

/* Define the union u_dmic_ctrl */
typedef union {
    /* Define the struct bits */
    struct {
        /* DMIC声道配置。     0: 不交换左右声道；        1: 交换左右声道       */
        unsigned int    dmic0_sw_dmic_mode    : 1   ; /* [0] */
        /* DMIC输入编码配置。       0: 0 -> +4,  1 -> -4；1: 0 -> -4, 1 -> +4 */
        unsigned int    dmic0_reverse         : 1   ; /* [1] */
        unsigned int    reserved_0            : 1   ; /* [2] */
        unsigned int    reserved_1            : 1   ; /* [3] */
        unsigned int    reserved_2            : 28  ; /* [31..4] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_dmic_ctrl;

/* Define the union u_dmic_div */
typedef union {
    /* Define the struct bits */
    struct {
        /* DMIC时钟配置：
         * 0000：关闭时钟；
         * 0001：2分频；3.072MHz
         * 0010：3分频；2.048MHz
         * 0011：4分频；1.536MHz
         * 0111：8分频；768KHz
         * 1011：12分频；512KHz
         * Other:关闭时钟。
         */
        unsigned int    fs_dmic0              : 4   ; /* [3..0] */
        unsigned int    reserved_0            : 4   ; /* [7..4] */
        unsigned int    reserved_1            : 24  ; /* [31..8] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_dmic_div;

/* Define the union u_mic23_up_afifo_ctrl */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    reserved_0            : 5   ; /* [4..0] */
        /* MIC上行通路通路三声道AFIFO将空阈值。        */
        unsigned int    mic2_up_fifo_aempty_th : 5   ; /* [9..5] */
        /* MIC上行通路通路三声道AFIFO将满阈值。       */
        unsigned int    mic2_up_fifo_afull_th : 5   ; /* [14..10] */
        /* MIC上行通路通路三声道AFIFO Clear Signal, high active.
         * 0：不清空FIFO数据；      1：清空FIFO数据
         */
        unsigned int    mic2_up_fifo_clr      : 1   ; /* [15] */
        unsigned int    reserved_1            : 5   ; /* [20..16] */
        /* MIC上行通路通路四声道AFIFO将空阈值。        */
        unsigned int    mic3_up_fifo_aempty_th : 5   ; /* [25..21] */
        /* MIC上行通路通路四声道AFIFO将满阈值。        */
        unsigned int    mic3_up_fifo_afull_th : 5   ; /* [30..26] */
        /* MIC上行通路通路四声道AFIFO Clear Signal, high active.
         * 0：不清空FIFO数据；       1：清空FIFO数据
         */
        unsigned int    mic3_up_fifo_clr      : 1   ; /* [31] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_mic23_up_afifo_ctrl;

/* Define the union u_mad_vad_up_afifo_ctrl */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    reserved_0            : 5   ; /* [4..0] */
        /* MAD AFIFO将空阈值。     */
        unsigned int    mad_up_fifo_aempty_th : 5   ; /* [9..5] */
        /* MAD AFIFO将满阈值。     */
        unsigned int    mad_up_fifo_afull_th  : 5   ; /* [14..10] */
        /* MAD AFIFO Clear Signal, high active. 0：不清空FIFO数据；       1：清空FIFO数据      */
        unsigned int    mad_up_fifo_clr       : 1   ; /* [15] */
        unsigned int    reserved_1            : 5   ; /* [20..16] */
        unsigned int    reserved_2            : 5   ; /* [25..21] */
        unsigned int    reserved_3            : 5   ; /* [30..26] */
        unsigned int    reserved_4            : 1   ; /* [31] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_mad_vad_up_afifo_ctrl;

/* Define the union u_fs_ctrl0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    reserved_0            : 3   ; /* [2..0] */
        unsigned int    reserved_1            : 2   ; /* [4..3] */
        unsigned int    reserved_2            : 2   ; /* [6..5] */
        unsigned int    reserved_3            : 3   ; /* [9..7] */
        unsigned int    reserved_4            : 2   ; /* [11..10] */
        unsigned int    reserved_5            : 3   ; /* [14..12] */
        unsigned int    reserved_6            : 3   ; /* [17..15] */
        /* mic2输入端采样率控制：
         * 3'b000：16k；3'b001：32k；3'b010：48k；3'b011：96k；其它   ： 保留 ；
         */
        unsigned int    fs_mic2_afifo         : 3   ; /* [20..18] */
        /* mic3输入端采样率控制：
         * 3'b000：16k；3'b001：32k；3'b010：48k；3'b011：96k；其它   ： 保留 ；
         */
        unsigned int    fs_mic3_afifo         : 3   ; /* [23..21] */
        unsigned int    reserved_7            : 3   ; /* [26..24] */
        unsigned int    reserved_8            : 3   ; /* [29..27] */
        unsigned int    reserved_9            : 2   ; /* [31..30] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_fs_ctrl0;

/* Define the union u_fs_ctrl1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    reserved_0            : 3   ; /* [2..0] */
        unsigned int    reserved_1            : 3   ; /* [5..3] */
        unsigned int    reserved_2            : 1   ; /* [6] */
        unsigned int    reserved_3            : 3   ; /* [9..7] */
        unsigned int    reserved_4            : 1   ; /* [10] */
        unsigned int    reserved_5            : 3   ; /* [13..11] */
        unsigned int    reserved_6            : 3   ; /* [16..14] */
        unsigned int    reserved_7            : 1   ; /* [17] */
        unsigned int    reserved_8            : 2   ; /* [19..18] */
        unsigned int    reserved_9            : 2   ; /* [21..20] */
        /* mic2_adc上行srcdn输出端采样率控制：
         * 2'b00：16k；  2'b01：32k；  2'b10：48k；  2'b11 : 96k ;
         */
        unsigned int    mic2_up_srcdn_dout    : 2   ; /* [23..22] */
        /* mic3_adc上行srcdn输出端采样率控制：
         * 2'b00：16k；  2'b01：32k；  2'b10：48k；  2'b11 : 96k;
         */
        unsigned int    mic3_up_srcdn_dout    : 2   ; /* [25..24] */
        unsigned int    reserved_10           : 2   ; /* [27..26] */
        unsigned int    reserved_11           : 2   ; /* [29..28] */
        unsigned int    reserved_12           : 2   ; /* [31..30] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_fs_ctrl1;

/* Define the union u_fs_ctrl2 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    reserved_0            : 2   ; /* [1..0] */
        unsigned int    reserved_1            : 1   ; /* [2] */
        /* mad cic的时钟选择，      1'b0:768k；1'b1:512k；  */
        unsigned int    mad_cic_sel           : 1   ; /* [3] */
        unsigned int    reserved_2            : 3   ; /* [6..4] */
        unsigned int    reserved_3            : 4   ; /* [10..7] */
        unsigned int    reserved_4            : 21  ; /* [31..11] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_fs_ctrl2;

/* Define the union u_codec_din_mux0 */
typedef union {
    /* Define the struct bits */
    struct {
        /* I2S_INNER的tx左声道的数据源：
         * 2'b00：mic2_adc_filter_dout；
         * 2'b01：mic3_adc_filter_dout：
         * 2'b10：mad_4_srcdn_dout；
         * 其它：保留；
         */
        unsigned int    i2s_inner_tx_l_dout_sel : 2   ; /* [1..0] */
        unsigned int    reserved_0            : 5   ; /* [6..2] */
        unsigned int    reserved_1            : 1   ; /* [7] */
        unsigned int    reserved_2            : 3   ; /* [10..8] */
        unsigned int    reserved_3            : 1   ; /* [11] */
        unsigned int    reserved_4            : 1   ; /* [12] */
        unsigned int    reserved_5            : 2   ; /* [14..13] */
        unsigned int    reserved_6            : 1   ; /* [15] */
        unsigned int    reserved_7            : 4   ; /* [19..16] */
        unsigned int    reserved_8            : 1   ; /* [20] */
        unsigned int    reserved_9            : 1   ; /* [21] */
        unsigned int    reserved_10           : 2   ; /* [23..22] */
        /* mad_r_cic_din的数据源选择：
         * 2'b00：从dmic0_l_dout；
         * 2'b01：从dmic0_r_dout；
         * 2'b10：从dmic1_l_dout；
         * 2'b11：从dmic1_r_dout；
         */
        unsigned int    mad_r_cic_din_sel     : 2   ; /* [25..24] */
        unsigned int    reserved_11           : 4   ; /* [29..26] */
        unsigned int    reserved_12           : 2   ; /* [31..30] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_codec_din_mux0;

/* Define the union u_codec_din_mux1 */
typedef union {
    /* Define the struct bits */
    struct {
        /* bm_mad_src_r_din的数据源选择：
         * 4'b0000 : mad_r_adc_srcdn_dout;
         * 4'b0011 : mic2_adc_pga_dout;
         * 4'b0100 : mic3_adc_pga_dout;
         * 其它   ： 保留；
         */
        unsigned int    bm_mad_src_r_din_sel  : 4   ; /* [3..0] */
        unsigned int    reserved_0            : 3   ; /* [6..4] */
        unsigned int    reserved_1            : 1   ; /* [7] */
        unsigned int    reserved_2            : 1   ; /* [8] */
        unsigned int    reserved_3            : 23  ; /* [31..9] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_codec_din_mux1;

/* Define the union u_codec_adc2_dc_offset */
typedef union {
    /* Define the struct bits */
    struct {
        /* adc2_hpf dc_offset配置   */
        unsigned int    adc2_dc_offset        : 24  ; /* [23..0] */
        unsigned int    reserved_0            : 8   ; /* [31..24] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_codec_adc2_dc_offset;

/* Define the union u_codec_adc3_dc_offset */
typedef union {
    /* Define the struct bits */
    struct {
        /* adc3_hpf dc_offset配置   */
        unsigned int    adc3_dc_offset        : 24  ; /* [23..0] */
        unsigned int    reserved_0            : 8   ; /* [31..24] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_codec_adc3_dc_offset;

/* Define the union u_codec_adc9_dc_offset */
typedef union {
    /* Define the struct bits */
    struct {
        /* mad_r_adc_dc_offset配置   */
        unsigned int    mad_r_adc_dc_offset   : 24  ; /* [23..0] */
        unsigned int    reserved_0            : 8   ; /* [31..24] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_codec_adc9_dc_offset;

/* Define the union u_codec_adc0_cic_gain */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    reserved_0            : 6   ; /* [5..0] */
        unsigned int    reserved_1            : 2   ; /* [7..6] */
        unsigned int    reserved_2            : 6   ; /* [13..8] */
        unsigned int    reserved_3            : 2   ; /* [15..14] */
        unsigned int    adc2_cic_gain         : 6   ; /* [21..16] */
        unsigned int    reserved_4            : 2   ; /* [23..22] */
        unsigned int    adc3_cic_gain         : 6   ; /* [29..24] */
        unsigned int    reserved_5            : 2   ; /* [31..30] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_codec_adc0_cic_gain;

/* Define the union u_codec_adc2_cic_gain */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    reserved_0            : 6   ; /* [5..0] */
        unsigned int    reserved_1            : 2   ; /* [7..6] */
        unsigned int    mad_r_adc_cic_gain    : 6   ; /* [13..8] */
        unsigned int    reserved_2            : 2   ; /* [15..14] */
        unsigned int    reserved_3            : 6   ; /* [21..16] */
        unsigned int    reserved_4            : 10  ; /* [31..22] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_codec_adc2_cic_gain;

/* Define the union u_mem_ctrl_1w2r */
typedef union {
    /* Define the struct bits */
    struct {
        /* 双口memory的控制信号        */
        unsigned int    mem_ctrl_1w2r         : 16  ; /* [15..0] */
        unsigned int    reserved_0            : 16  ; /* [31..16] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_mem_ctrl_1w2r;

/* define the union u_ana_reg_00 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int rx0_pd_08 : 1;          /* [0]  */
        unsigned int rx0_pd_sdm_08 : 1;      /* [1]  */
        unsigned int rx0_pd_pga_08 : 1;      /* [2]  */
        unsigned int rx0_pd_chp_pga_08 : 1;  /* [3]  */
        unsigned int rx0_pd_dout_08 : 1;     /* [4]  */
        unsigned int pd_mclk_rxadc_div2 : 1; /* [5]  */
        unsigned int pd_mclk_rxadc_chp : 1;  /* [6]  */
        unsigned int pd_mclk_mad_div2 : 1;   /* [7]  */
        unsigned int pd_clk_gen_l : 1;       /* [8]  */
        unsigned int pd_pga_l : 1;           /* [9]  */
        unsigned int pd_sdm_l : 1;           /* [10]  */
        unsigned int pd_dout_l : 1;          /* [11]  */
        unsigned int pd_ibias : 1;           /* [12]  */
        unsigned int pd_ib08_rctune : 1;     /* [13]  */
        unsigned int pd_rctune08 : 1;        /* [14]  */
        unsigned int pd_vcm0p45 : 1;         /* [15]  */
        unsigned int pd_vcm0p9 : 1;          /* [16]  */
        unsigned int pd_vbulk : 1;           /* [17]  */
        unsigned int reserved : 14;          /* [31..18]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} u_ana_reg_00;

/* define the union u_ana_reg_01 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int rx0_mute_pga : 1;        /* [0]  */
        unsigned int rx0_mute_fls : 1;        /* [1]  */
        unsigned int rx0_zc_en : 1;           /* [2]  */
        unsigned int rx0_zc_rstb : 1;         /* [3]  */
        unsigned int rx0_dwa_rst : 1;         /* [4]  */
        unsigned int rx0_byp_chp_int_s1 : 1;  /* [5]  */
        unsigned int rx0_rctune08_reg : 5;    /* [10..6]  */
        unsigned int rx0_rctune_code_sel : 1; /* [11]  */
        unsigned int rx0_dwa_byp : 1;         /* [12]  */
        unsigned int rx0_zc_mute_byp : 1;     /* [13]  */
        unsigned int rx0_line_sel : 1;        /* [14]  */
        unsigned int rx0_pga_gain : 5;        /* [19..15]  */
        unsigned int rx0_boost_ctrl : 1;      /* [20]  */
        unsigned int rx0_swp : 4;             /* [24..21]  */
        unsigned int rx0_swn : 4;             /* [28..25]  */
        unsigned int rx0_adatn : 2;           /* [30..29]  */
        unsigned int reserved : 1;            /* [31]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} u_ana_reg_01;

/* define the union u_ana_reg_02 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int rx0_flstn : 2;           /* [1..0]  */
        unsigned int rx0_ictln : 2;           /* [3..2]  */
        unsigned int rx0_ictlp : 3;           /* [6..4]  */
        unsigned int rx0_zcto : 2;            /* [8..7]  */
        unsigned int rx0_byp_pga_unlock : 1;  /* [9]  */
        unsigned int rx0_sel_clk : 1;         /* [10]  */
        unsigned int rx0_sel_clk_dly : 1;     /* [11]  */
        unsigned int rx0_sel_clk_timing : 1;  /* [12]  */
        unsigned int rx0_sel_dout_timing : 1; /* [13]  */
        unsigned int rx0_op2_cc_ctrl : 2;     /* [15..14]  */
        unsigned int reserved : 16;           /* [31..16]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} u_ana_reg_02;

/* define the union u_ana_reg_03 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int en_mclk_rxadc : 1;           /* [0]  */
        unsigned int en_clk_rxadc_6p144m : 1;     /* [1]  */
        unsigned int en_rxadc_chp_pga : 1;        /* [2]  */
        unsigned int en_rxadc_chp_s1 : 1;         /* [3]  */
        unsigned int en_rxadc_zc : 1;             /* [4]  */
        unsigned int en_mclk_mad : 1;             /* [5]  */
        unsigned int en_clk_mad_ch : 1;           /* [6]  */
        unsigned int en_mad_chp_pga : 1;          /* [7]  */
        unsigned int en_mad_chp_sdm : 1;          /* [8]  */
        unsigned int en_clk_rctune_6p144m : 1;    /* [9]  */
        unsigned int mclk_rctune_dege_sel : 1;    /* [10]  */
        unsigned int rxadc_chp_clk_pga_sel : 2;   /* [12..11]  */
        unsigned int rxadc_chp_clk_s1_sel : 2;    /* [14..13]  */
        unsigned int rxadc_clk_zc_sel : 1;        /* [15]  */
        unsigned int mclk_rxadc_dege_sel : 1;     /* [16]  */
        unsigned int mclk_rxadc_dege_sel_pre : 1; /* [17]  */
        unsigned int mad_chp_clk_pga_sel : 2;     /* [19..18]  */
        unsigned int mad_chp_clk_sdm_sel : 2;     /* [21..20]  */
        unsigned int mclk_mad_dege_sel_pre : 1;   /* [22]  */
        unsigned int mclk_mad_dege_sel_post : 1;  /* [23]  */
        unsigned int reserved : 8;                /* [31..24]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} u_ana_reg_03;

/* define the union u_ana_reg_04 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int mic0_sel_en : 1;       /* [0]  */
        unsigned int pga_gain : 4;          /* [4..1]  */
        unsigned int pga_mute : 2;          /* [6..5]  */
        unsigned int byp_chop_pga : 1;      /* [7]  */
        unsigned int boost_en : 1;          /* [8]  */
        unsigned int byp_chop_sdm : 1;      /* [9]  */
        unsigned int int_tune_reg : 5;      /* [14..10]  */
        unsigned int int_tune_sel : 1;      /* [15]  */
        unsigned int resdown_sel : 4;       /* [19..16]  */
        unsigned int sdm_adatn : 2;         /* [21..20]  */
        unsigned int sdm_dwa_byps : 1;      /* [22]  */
        unsigned int sdm_dwa_rst : 1;       /* [23]  */
        unsigned int sdm_flstn : 2;         /* [25..24]  */
        unsigned int sel_clk_dig : 1;       /* [26]  */
        unsigned int mute_fls : 1;          /* [27]  */
        unsigned int vref0p45_sel : 1;      /* [28]  */
        unsigned int mclk_edge_sel : 1;     /* [29]  */
        unsigned int mclk_fls_dege_sel : 1; /* [30]  */
        unsigned int mclk_dac_dege_sel : 1; /* [31]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} u_ana_reg_04;

/* define the union u_ana_reg_05 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int mclk_dig_dege_sel : 1;  /* [0]  */
        unsigned int mclk_dwa_delay_sel : 1; /* [1]  */
        unsigned int mclk_dwa_dege_sel : 1;  /* [2]  */
        unsigned int mclk_rstn : 1;          /* [3]  */
        unsigned int reserved : 28;          /* [31..4]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} u_ana_reg_05;

/* define the union u_ana_reg_07 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int en_ibiasop_hp : 1;     /* [0]  */
        unsigned int ctib_ctune : 2;        /* [2..1]  */
        unsigned int ctib_mad0_1u : 6;      /* [8..3]  */
        unsigned int ctib_rx : 14;          /* [22..9]  */
        unsigned int ctib_micbias_0p1u : 2; /* [24..23]  */
        unsigned int ctib_vcmbuf_1p25u : 6; /* [30..25]  */
        unsigned int reserved : 1;          /* [31]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} u_ana_reg_07;

/* define the union u_ana_reg_08 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int ctib_micbias_2u : 2; /* [1..0]  */
        unsigned int ctrl_vbulk : 2;      /* [3..2]  */
        unsigned int reserved_0 : 12;     /* [15..4]  */
        unsigned int rctune_st08 : 1;     /* [16]  */
        unsigned int rsetn : 1;           /* [17]  */
        unsigned int vcm_ib_sel : 4;      /* [21..18]  */
        unsigned int reserved : 10;       /* [31..22]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} u_ana_reg_08;

/* define the union u_ana_reg_09 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int en_vreg : 1;        /* [0]  */
        unsigned int en_bypass : 1;      /* [1]  */
        unsigned int enb_soft_start : 1; /* [2]  */
        unsigned int en_mux : 2;         /* [4..3]  */
        unsigned int en_lp : 1;          /* [5]  */
        unsigned int enb_ilimit : 1;     /* [6]  */
        unsigned int enb_dischg : 1;     /* [7]  */
        unsigned int sel_vout : 3;       /* [10..8]  */
        unsigned int reserved : 21;      /* [31..11]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} u_ana_reg_09;

/* define the union u_ana_reg_10 */
typedef union {
    /* define the struct bits */
    struct {
        unsigned int en_ibg3 : 1;       /* [0]  */
        unsigned int en_ref_bg3 : 1;    /* [1]  */
        unsigned int en_refbuffer3 : 1; /* [2]  */
        unsigned int pmu_reserv3 : 16;  /* [18..3]  */
        unsigned int ref_trim3 : 11;    /* [29..19]  */
        unsigned int sel : 1;           /* [30]  */
        unsigned int pmu3_iso_n : 1;    /* [31]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} u_ana_reg_10;

/* define the union u_ana_reg_11 */
typedef union {
	/* define the struct bits */
    struct {
        unsigned int vset_vref1p0_code : 3;    /* [2..0]  */
        unsigned int codecldo0p9_en : 1;       /* [3]  */
        unsigned int codecldo0p9_en_delay : 1; /* [4]  */
        unsigned int codecldo0p9_en_hiz : 1;   /* [5]  */
        unsigned int codecldo0p9_en_sw : 1;    /* [6]  */
        unsigned int coedecldo0p9_ictr : 3;    /* [9..7]  */
        unsigned int coedecldo0p9_reserv : 2;  /* [11..10]  */
        unsigned int coedecldo0p9_vset : 4;    /* [15..12]  */
        unsigned int codecldo_en : 1;          /* [16]  */
        unsigned int codecldo_en_delay : 1;    /* [17]  */
        unsigned int codecldo_en_hiz : 1;      /* [18]  */
        unsigned int codecldo_en_sw : 1;       /* [19]  */
        unsigned int coedecldo_ictr : 3;       /* [22..20]  */
        unsigned int coedecldo_reserv : 2;     /* [24..23]  */
        unsigned int coedecldo_vset : 4;       /* [28..25]  */
        unsigned int test_sel : 3;             /* [31..29]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} u_ana_reg_11;

/* define the union u_ana_reg_ro */
typedef union {
	/* define the struct bits */
    struct {
        unsigned int reserved0 : 2;            /* [1..0]  */
        unsigned int rctune_code : 5;          /* [6..2]  */
        unsigned int reserved1 : 25;           /* [31..7]  */
    } bits;

    /* define an unsigned member */
    unsigned int u32;
} u_ana_reg_ro;

typedef union {
    struct {
        unsigned int eq_byp_en : 1;     /* [0] */
        unsigned int eq_fs : 1;         /* [1] */
        unsigned int reserved : 30;     /* [31..2] */
    } bits;
    unsigned int u32;
} u_eq_ctrl;

typedef struct {
    unsigned int a1;
    unsigned int a2;
    unsigned int b0;
    unsigned int b1;
    unsigned int b2;
} s_biquad_band_coefs;

/* Define the union u_lp_cic_gain0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    reserved_0            : 8   ; /* [7..0] */
        unsigned int    reserved_1            : 8   ; /* [15..8] */
        unsigned int    lp_cicdn_adc2_gain    : 8   ; /* [23..16] */
        unsigned int    lp_cicdn_adc3_gain    : 8   ; /* [31..24] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_lp_cic_gain0;

/* Define the union u_data_mux */
typedef union {
    /* Define the struct bits */
    struct {
        /* mic3的adc输入：
         * 2'b00：输入dmic_r_dout;
         * 2'b01：输入adc0_ana_din；
         * 2'b10: 输入mad_adc0_a2d;
         */
        unsigned int    mic3_adc_din_sel      : 2   ; /* [1..0] */
        unsigned int    reserved_0            : 2   ; /* [3..2] */
        unsigned int    reserved_1            : 3   ; /* [6..4] */
        unsigned int    reserved_2            : 2   ; /* [8..7] */
        /* mic2的adc输入：
         * 2'b00：输入dmic_l_dout;
         * 2'b01：输入adc0_ana_din；
         * 2'b10: 输入mad_adc0_a2d;
         */
        unsigned int    mic2_adc_din_sel      : 2   ; /* [10..9] */
        unsigned int    reserved_3            : 21  ; /* [31..11] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_data_mux;

typedef union {
    struct {
        unsigned int    postproc_en         : 1 ;   /* [0] */
        unsigned int    postproc_eq_byp_en  : 1 ;   /* [1] */
        unsigned int    postproc_sft        : 5 ;   /* [6..2] */
        unsigned int    reserved            : 9 ;   /* [15..7] */
        unsigned int    postproc_gain       : 16;   /* [31..16 */
    } bits;
    unsigned int u32;
} u_postproc_ctrl;

typedef union {
    struct {
        unsigned int    postproc_limiter_en     : 1 ;   /* [0] */
        unsigned int    postproc_limiter_alpha  : 9 ;   /* [9..1] */
        unsigned int    reserved                : 22;   /* [31..10] */
    } bits;
    unsigned int u32;
} u_postproc_limiter_ctrl;

/* Define the union u_adc_filter */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    reserved_0            : 1   ; /* [0] */
        unsigned int    reserved_1            : 1   ; /* [1] */
        unsigned int    reserved_2            : 1   ; /* [2] */
        unsigned int    reserved_3            : 1   ; /* [3] */
        unsigned int    reserved_4            : 1   ; /* [4] */
        unsigned int    reserved_5            : 1   ; /* [5] */
        unsigned int    reserved_6            : 1   ; /* [6] */
        unsigned int    reserved_7            : 1   ; /* [7] */
        unsigned int    adc2_compd_bypass_en  : 1   ; /* [8] */
        unsigned int    adc2_fir2d_bypass_en  : 1   ; /* [9] */
        unsigned int    adc2_com2d_bypass_en  : 1   ; /* [10] */
        unsigned int    adc2_hpf_bypass_en    : 1   ; /* [11] */
        unsigned int    adc3_compd_bypass_en  : 1   ; /* [12] */
        unsigned int    adc3_fir2d_bypass_en  : 1   ; /* [13] */
        unsigned int    adc3_com2d_bypass_en  : 1   ; /* [14] */
        unsigned int    adc3_hpf_bypass_en    : 1   ; /* [15] */
        unsigned int    reserved_8            : 1   ; /* [16] */
        unsigned int    reserved_9            : 1   ; /* [17] */
        unsigned int    reserved_10           : 1   ; /* [18] */
        unsigned int    reserved_11           : 1   ; /* [19] */
        unsigned int    reserved_12           : 1   ; /* [20] */
        unsigned int    reserved_13           : 1   ; /* [21] */
        unsigned int    reserved_14           : 1   ; /* [22] */
        unsigned int    reserved_15           : 1   ; /* [23] */
        unsigned int    reserved_16           : 1   ; /* [24] */
        unsigned int    reserved_17           : 1   ; /* [25] */
        unsigned int    reserved_18           : 1   ; /* [26] */
        unsigned int    reserved_19           : 1   ; /* [27] */
        unsigned int    reserved_20           : 1   ; /* [28] */
        unsigned int    reserved_21           : 1   ; /* [29] */
        unsigned int    reserved_22           : 1   ; /* [30] */
        unsigned int    reserved_23           : 1   ; /* [31] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_adc_filter;

/* Define the union u_codec_clk_div0 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    clk_12288k_div_ratio  : 7   ; /* [6..0] */
        unsigned int    reserved_0            : 1   ; /* [7] */
        unsigned int    clk_6144k_div_ratio   : 7   ; /* [14..8] */
        unsigned int    reserved_1            : 1   ; /* [15] */
        unsigned int    clk_3072k_div_ratio   : 7   ; /* [22..16] */
        unsigned int    reserved_2            : 1   ; /* [23] */
        unsigned int    clk_768k_div_ratio    : 7   ; /* [30..24] */
        unsigned int    reserved_3            : 1   ; /* [31] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_codec_clk_div0;

/* Define the union u_codec_clk_div1 */
typedef union {
    /* Define the struct bits */
    struct {
        unsigned int    clk_512k_div_ratio    : 7   ; /* [6..0] */
        unsigned int    reserved_0            : 25  ; /* [31..7] */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;
} u_codec_clk_div1;

/* Define the global struct */
typedef struct {
    volatile unsigned int           version;        /* 0x0000 */
    volatile u_codec_clk_en0        codec_clk_en0;  /* 0x0004 */
    volatile u_codec_clk_en1        codec_clk_en1;  /* 0x0008 */
    volatile u_codec_clk_en2        codec_clk_en2;  /* 0x000C */
    volatile u_codec_sw_rst_n       codec_sw_rst_n; /* 0x0010 */
    volatile u_i2s_inner_ctrl       i2s_inner_ctrl; /* 0x0014 */
    volatile unsigned int           reserved_0[calc_reg_num(0x0014, 0x0020)];
    volatile unsigned int           pga_thre_ctrl0; /* 0x0020 */
    volatile unsigned int           pga_thre_ctrl1; /* 0x0024 */
    volatile unsigned int           pga_thre_ctrl2; /* 0x0028 */
    volatile unsigned int           reserved_1;
    volatile u_pga_gainoffset_ctrl1 pga_gainoffset_ctrl1; /* 0x0030 */
    volatile unsigned int           reserved_2;
    volatile u_pga_gainoffset_ctrl3 pga_gainoffset_ctrl3; /* 0x0038 */
    volatile unsigned int           reserved_3[calc_reg_num(0x0038, 0x0050)];
    volatile u_mic2_pga_ctrl        mic2_pga_ctrl;  /* 0x0050 */
    volatile u_mic3_pga_ctrl        mic3_pga_ctrl;  /* 0x0054 */
    volatile unsigned int           reserved_4[calc_reg_num(0x0054, 0x006C)];
    volatile u_mad_adc_r_pga_ctrl   mad_adc_r_pga_ctrl; /* 0x006C */
    volatile unsigned int           reserved_5[calc_reg_num(0x006C, 0x0084)];
    volatile u_srcdn_ctrl0          srcdn_ctrl0;    /* 0x0084 */
    volatile u_srcdn_ctrl1          srcdn_ctrl1;    /* 0x0088 */
    volatile unsigned int           reserved_6[calc_reg_num(0x0088, 0x0094)];
    volatile u_dmic_ctrl            dmic_ctrl;      /* 0x0094 */
    volatile u_dmic_div             dmic_div;       /* 0x0098 */
    volatile unsigned int           reserved_7[calc_reg_num(0x0098, 0x00B4)];
    volatile u_mic23_up_afifo_ctrl  mic23_up_afifo_ctrl; /* 0x00B4 */
    volatile unsigned int           reserved_8[calc_reg_num(0x00B4, 0x00C0)];
    volatile u_mad_vad_up_afifo_ctrl mad_vad_up_afifo_ctrl; /* 0x00C0 */
    volatile unsigned int           reserved_9;
    volatile u_fs_ctrl0             fs_ctrl0;       /* 0x00C8 */
    volatile u_fs_ctrl1             fs_ctrl1;       /* 0x00CC */
    volatile u_fs_ctrl2             fs_ctrl2;       /* 0x00D0 */
    volatile u_codec_din_mux0       codec_din_mux0; /* 0x00D4 */
    volatile u_codec_din_mux1       codec_din_mux1; /* 0x00D8 */
    volatile unsigned int           reserved_10[calc_reg_num(0x00D8, 0x00E4)];
    volatile u_codec_adc2_dc_offset codec_adc2_dc_offset; /* 0x00E4 */
    volatile u_codec_adc3_dc_offset codec_adc3_dc_offset; /* 0x00E8 */
    volatile unsigned int           reserved_11[calc_reg_num(0x00E8, 0x0100)];
    volatile u_codec_adc9_dc_offset codec_adc9_dc_offset; /* 0x0100 */
    volatile unsigned int           reserved_12;
    volatile u_codec_adc0_cic_gain  codec_adc0_cic_gain;  /* 0x0108 */
    volatile unsigned int           reserved_13;
    volatile u_codec_adc2_cic_gain  codec_adc2_cic_gain;  /* 0x0110 */
    volatile unsigned int           reserved_14;
    volatile u_mem_ctrl_1w2r        mem_ctrl_1w2r;  /* 0x0118 */
} s_codec_afe_regs;

typedef struct {
    volatile u_ana_reg_00           ana_reg_00;   /* 0x01F0 */
    volatile u_ana_reg_01           ana_reg_01;   /* 0x01F4 */
    volatile u_ana_reg_02           ana_reg_02;   /* 0x01F8 */
    volatile u_ana_reg_03           ana_reg_03;   /* 0x01FC */
    volatile u_ana_reg_04           ana_reg_04;   /* 0x0200 */
    volatile u_ana_reg_05           ana_reg_05;   /* 0x0204 */
    volatile unsigned int           ana_reg_06;   /* 0x0208 */
    volatile u_ana_reg_07           ana_reg_07;   /* 0x020C */
    volatile u_ana_reg_08           ana_reg_08;   /* 0x0210 */
    volatile u_ana_reg_09           ana_reg_09;   /* 0x0214 */
    volatile u_ana_reg_10           ana_reg_10;   /* 0x0218 */
    volatile u_ana_reg_11           ana_reg_11;   /* 0x021C */
    volatile unsigned int           reserved[calc_reg_num(0x021C, 0x0244)];
    volatile u_ana_reg_ro           ana_reg_ro;   /* 0x0244 */
} s_codec_ana_regs;

typedef struct {
    volatile u_eq_ctrl              eq_ctrl;            /* 0x025c */
    volatile s_biquad_band_coefs    eq_calib_bands[8];  /* 0x0260 ~ 02FC */
} s_calib_eq_regs;

typedef struct {
    volatile u_lp_cic_gain0         lp_cic_gain0;   /* 0x0300 */
    volatile unsigned int           reserved_20[calc_reg_num(0x0300, 0x030C)];
    volatile u_data_mux             data_mux;       /* 0x030C */
    volatile unsigned int           reserved_21[calc_reg_num(0x030C, 0x03E0)];
    volatile unsigned int           codec_reserve;  /* 0x03E0 */
    volatile unsigned int           adc_ec_reserve; /* 0x03E4 */
    volatile u_postproc_ctrl        postproc_ctrl;  /* 0x03E8 */
    volatile s_biquad_band_coefs    eq_postproc_bands[8];   /* 0x03EC ~ 0488 */
    volatile u_postproc_limiter_ctrl postproc_limiter_ctrl; /* 0x048C */
    volatile unsigned int           reserved_22[calc_reg_num(0x048C, 0x04F4)];
    volatile u_adc_filter           adc_filter;     /* 0x04F4 */
    volatile unsigned int           reserved_23;
    volatile u_codec_clk_div0       codec_clk_div0; /* 0x04FC */
    volatile u_codec_clk_div1       codec_clk_div1; /* 0x0500 */
} s_codec_common_regs;

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* __REG_AFE_H__ */
