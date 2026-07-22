/*
 * Copyright (c) CompanyNameMagicTag 2020-2020. All rights reserved.
 * Description: header file for chip type definition
 * Author: audio
 */

#ifndef __CHIP_TYPE_H__
#define __CHIP_TYPE_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/* Definitions for chipset type */
#define boya        0x5a5a0005
#define brandy      0x5a5a0006
#define socmn1      0x5a5a0007

/* Definitions for platform type */
#define plf_type_prot 0x00000222 /* protocol */
#define plf_type_apps 0x00001a22 /* application */
#define plf_type_adsp 0x00002d22 /* dsp */

/* Definitions for system type */
#define os_type_liteos 0x00001005
#define os_type_linux 0x00002005
#define os_type_win 0x00003005
#define os_type_xtensa 0x00004005
#define os_type_rtos 0x00005005

/* Definitions for system arch */
#define core_arch_mips 0x00001020
#define core_arch_arm 0x00002020
#define core_arch_arm64 0x00002040
#define core_arch_x86 0x00003020
#define core_arch_x64 0x00003040
#define core_arch_hifi3 0x00004320
#define core_arch_hifi3z 0x00004321
#define core_arch_hifi4 0x00004420
#define core_arch_hifi5 0x00004520

enum {
    DSP_CORE0 = 0,
    DSP_CORE1,

    DSP_CORE_NUM,
};

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* __CHIP_TYPE_H__ */
