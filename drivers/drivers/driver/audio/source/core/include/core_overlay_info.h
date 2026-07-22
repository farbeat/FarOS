/*
 * Copyright (c) CompanyNameMagicTag 2012-2021. All rights reserved.
 * Description: overlay information
 */

#ifndef __CORE_OVERLAY_INFO_H__
#define __CORE_OVERLAY_INFO_H__

#include "audio_type.h"
#include "audio_struct.h"
#include "mem_addr.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

#if defined(BOARD_EVB4MINI) || defined(MEMORY_MINI)
/* Mini version:: DSP:BOARD_EVB4MINI ACORE:MEMORY_MINI */
#define MAIN_SRAM_SIZE 0x0007F000 /* 508KB */
#define DFX_SHM_SIZE 0x00010000 /* 64KB */
#define AEF_BIN_SIZE 0x00018000 /* 96KB */
#define CODEC_BIN_SIZE 0x00000000 /* 0KB */
#define SEA_BIN_SIZE 0x00019000 /* 100KB */
#define AAI_BIN_SIZE 0x00000000 /* 0KB */
#elif defined(BOARD_EVB4MAX) || defined(CONFIG_SEA_PHS_SUPPORT)
/* Max version:: DSP:BOARD_EVB4MAX ACORE:CONFIG_SEA_PHS_SUPPORT */
#define MAIN_SRAM_SIZE 0x00095000 /* 596KB */
#define DFX_SHM_SIZE 0x00003000 /* 12KB */
#define AEF_BIN_SIZE 0x0018000 /* 96KB */
#define CODEC_BIN_SIZE 0x00026000 /* 152KB */
#define SEA_BIN_SIZE 0x0002a000 /* 168KB */ /* SEE */
#define AAI_BIN_SIZE 0x00040000 /* 256KB */ /* PHS */
#else /* Middle */
#if (SAP_CHIP_TYPE == socmn1)
#define MAIN_SRAM_SIZE 0x0007c000 /* 496KB */
#define DFX_SHM_SIZE 0x00004000 /* 16KB */
#define AEF_BIN_SIZE 0x00000000 /* 0KB */
#define CODEC_BIN_SIZE 0x00026000 /* 152KB */
#define SEA_BIN_SIZE 0x0002a000 /* 168KB */
#define AAI_BIN_SIZE 0x00000000 /* 0KB */
#else
/* Middle version:: */
#define MAIN_SRAM_SIZE 0x00095000 /* 596KB */
#define DFX_SHM_SIZE 0x00003000 /* 12KB */
#define AEF_BIN_SIZE 0x0018000 /* 96KB */
#define CODEC_BIN_SIZE 0x00026000 /* 152KB */
#define SEA_BIN_SIZE 0x0002a000 /* 168KB */ /* SEE */
#define AAI_BIN_SIZE 0x00040000 /* 256KB */ /* PHS */
#endif /* SAP_CHIP_TYPE */
#endif

#define MAIN_SRAM_ADDR SRAM_MAP_ADDR
#define DFX_SHM_ADDR (MAIN_SRAM_ADDR + MAIN_SRAM_SIZE)
#define AEF_ENTRY_ADDR (DFX_SHM_ADDR + DFX_SHM_SIZE)
#define CODEC_ENTRY_ADDR (AEF_ENTRY_ADDR + AEF_BIN_SIZE)
#define SEA_ENTRY_ADDR (CODEC_ENTRY_ADDR + CODEC_BIN_SIZE)
#define AAI_ENTRY_ADDR (SEA_ENTRY_ADDR + SEA_BIN_SIZE)

#define HAID_BIN_SIZE 0x000F0000 /* 960KB */
#define HAID_ENTRY_ADDR 0xE818C000

#define EXTCODEC_BIN_SIZE SOC_L2RAM_SIZE
#define EXTCODEC_ENTRY_ADDR SOC_L2RAM_AUDIO_VIRT_ADDR

/* overlay module id */
typedef enum {
    OVERLAY_CODEC = 0,
    OVERLAY_EXTCODEC,
    OVERLAY_SEA,
    OVERLAY_AAI,
    OVERLAY_HAID,
    OVERLAY_AEF,

    OVERLAY_MAX,
} core_overlay_id;

td_s32 overlay_get_section_attr(audio_lib_id type, td_u32 *addr, td_u32 *size);

td_void *overlay_get_entry(core_overlay_id overlay_id);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __CORE_OVERLAY_INFO_H__ */
