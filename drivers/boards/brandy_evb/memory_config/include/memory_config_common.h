/*
 * Copyright (c) @CompanyNameMagicTag 2021-2022. All rights reserved.
 * Description:  Default memory configurations
 * Author: @CompanyNameTag
 * Create:  2022-03-09
 */

#ifndef MEMORY_CONFIG_COMMON_H
#define MEMORY_CONFIG_COMMON_H

#include "product.h"
#include "chip_definitions.h"
#include "chip_core_definition.h"
#include "psram.h"

/**
 * @defgroup connectivity_config_memory MEMORY
 * @ingroup  connectivity_config
 * @{
 */
/* Standard lengths */
#define BT_VECTORS_LENGTH     0x800
/* Copy the startup of the app to the start address of the itcm in the ssb.
 * Do not overwrite the ssb code segment.
 * app startup = ssb startup + ssb version */
#define APP_VECTORS_LENGTH    436
#define VERSION_LENGTH        88
#define CHANGE_ID_LENGTH      16

#ifdef MASTER_LOAD_SLAVE
#define BUILD_INFO_LENGRH    552
#else
#define BUILD_INFO_LENGRH    0
#endif

#define FILL_4_BYTE           4

#define BCPU_RAM_START        0x0

/* rom ram text */
#define BCPU_STARTUP_LENGTH       0x800
#define BCPU_PATCH_TBL_LENGTH     0x800
#define BCPU_ROM_RAM_START   (BCPU_RAM_START + BCPU_STARTUP_LENGTH + BCPU_PATCH_TBL_LENGTH)
/* ----------------------------------------------------------------------------------------------------------------- */
/* Share MEM defines, 64K in FPGA, 160K in ASIC
 * SHARE MEM                IPC Mailbox
 *                          SYSTEM CONFIG
 *                          SYSTEM STATUS
 *                          BT LOG
 *                          APP LOG
 *                          DSP LOG
 */
#define SHARED_MEM_START 0x87000000
#if CHIP_ASIC
#define SHARED_MEM_LENGTH 0x18000
#else
#define SHARED_MEM_LENGTH 0x10000
#endif
#define SHARED_MEM_END    (SHARED_MEM_START + SHARED_MEM_LENGTH)

/*
 * ********************* ROM ALLOCATION ***********************
 *
 * Used solely by the MCU Core ROM image built into the chip.
 */
/* 48K ROM */
#define ROM_START  0x00000000
#define ROM_LENGTH 0xC000

/* APP ITCM config */
#define APP_ITCM_ORIGIN 0x10000
#define APP_ITCM_LENGTH 0x30000

/* APP DTCM config */
#define APP_DTCM_ORIGIN 0x20000000
#define APP_DTCM_LENGTH 0x70000

#ifdef SW_RTT_DEBUG
#define MCU_RTT_LEN     0x500
#endif
/*
 * ********************* L2M ALLOCATION ***********************
 * Description                  addr:mcpu/others         len:FPGA/ASIC
 * L2M_DSP_RAM_DSP_SHARE        0x60100000 / 0x80100000  0k/1024k        // can used as DSP RAM
 * L2M_DSP_RAM_MCU_SHARE        0x60200000 / 0x80200000  0k/256k         // can used for mcu
 * L2M_GPU_RAM_MCU_SHARE        0x60240000 / 0x00240000  0k/128k         // can used for mcu
 * L2M_GPU_RAM_GPU_SHARE        0x60260000 / 0x00260000  0k/1280k        // can used as GPU RAM
 */
/* L2M_DSP_RAM */
#ifndef CONFIG_PSRAM_SUPPORT  // 无PSRAM场景
#define L2M_DSP_RAM_RESERVED_ORIGIN  0x60100000
#define L2M_DSP_RAM_MCU_SHARE_ORIGIN 0x60136000
#define L2M_DSP_RAM_DSP_SHARE_ORIGIN 0x60180000

#define L2M_DSP_RAM_RESERVED_LENGTH  0x36000   // ASIC version 216K for mcu
#define L2M_DSP_RAM_MCU_SHARE_LENGTH 0x4A000   // ASIC version 296K for mcu
#define L2M_DSP_RAM_DSP_SHARE_LENGTH 0xC0000   // ASIC version 768K for dsp
#else
#ifdef CONFIG_SEA_PHS_SUPPORT  // 有PSRAM场景，DSP带语音唤醒特性
#define L2M_DSP_RAM_MCU_SHARE_ORIGIN 0x60100000
#define L2M_DSP_RAM_DSP_SHARE_ORIGIN 0x60100000

#define L2M_DSP_RAM_MCU_SHARE_LENGTH 0x0       // ASIC version 0K for mcu
#define L2M_DSP_RAM_DSP_SHARE_LENGTH 0x140000  // ASIC version 1280K for dsp
#else                          // 有PSRAM场景，DSP不带语音唤醒特性
#define L2M_DSP_RAM_MCU_SHARE_ORIGIN 0x60100000
#define L2M_DSP_RAM_DSP_SHARE_ORIGIN 0x60140000

#define L2M_DSP_RAM_MCU_SHARE_LENGTH 0x40000   // ASIC version 256K for mcu
#define L2M_DSP_RAM_DSP_SHARE_LENGTH 0x100000  // ASIC version 1024K for dsp
#endif
#endif

/* L2M_GPU_RAM */
#ifndef CONFIG_PSRAM_SUPPORT  // 无PSRAM场景
#define L2M_GPU_RAM_SHARE_MCU_ORIGIN 0x60240000
#define L2M_GPU_RAM_GPU_SHARE_ORIGIN 0x60240000

#define L2M_GPU_RAM_MCU_SHARE_LENGTH 0x0      // ASIC version 0K for mcu
#define L2M_GPU_RAM_GPU_SHARE_LENGTH 0x160000 // ASIC version 1408K for gpu
#else                         // 有PSRAM场景
#define L2M_GPU_RAM_SHARE_MCU_ORIGIN 0x60260000
#define L2M_GPU_RAM_GPU_SHARE_ORIGIN 0x60280000

#define L2M_GPU_RAM_MCU_SHARE_LENGTH 0x20000  // ASIC version 128K for mcu
#define L2M_GPU_RAM_GPU_SHARE_LENGTH 0x120000 // ASIC version 1152K for gpu
#endif

/*
 * norflash 分区修改会导致已商用版本OTA失败，此处修改需谨慎
 * norflash修改请同时修改以下文件：
 *  分区表配置：
 *   16M:build\config\target_config\brandy\param_sector\param_sector_16M.json
 *   8M:build\config\target_config\brandy\param_sector\param_sector.json
 *  打包脚本：
 *  tools\pkg\chip_packet\brandy\packet.py
*/
#if defined(CFG_FLASH_SIZE) && (CFG_FLASH_SIZE == 16)
#include "flash_16M.h"
#else
#include "flash_8M.h"
#endif

/*
 * ********************* RAM ALLOCATION *********************
 *
 * Three main RAM areas, the 'BCPU' RAM, the 'APP' RAM, and 'share' RAM.
 *      'APP' RAM include 'ITCM' for code and 'DTCM' for DATA.
 * As the name implies, every core can access it's RAM and they all allowed
 * to access 'share' RAM
 *
 * BT core can access all RAM include APP RAM and DSP RAM.
 *
 * The 'shared' RAM is used by all cores, this area visible to them all
 * to exchange larger amounts of data.
 *
 * SHARED RAM               IPC mail box
 *                          LOG Area
 */
/* ----------------------------------------------------------------------------------------------------------------- */
/* BCPU RAM defines
 * BCPU has base 640K RAM and can set other 96K at the share RAM
 * as it's private, total 736K.
 * BCPU RAM                 VECTORS TABLE
 *                          STACK
 *                          RAM TEXT
 *                          RAM
 *                          PRESERVE
 *                          *VIRTUAL_OTP
 */
#define BCPU_ROM_START    0xA0000
#define BCPU_ROM_LENGTH   0xE0000

#define BCPU_DIAG_START   0x80000
#define BCPU_DIAG_LENGTH  0x10000

#define BCPU_PATCH_START  BCPU_DIAG_START
#define BCPU_PATCH_LENGTH 0x4000

#define BCPU_RAM_LENGTH   0x90000

/* APP Core can also access the BT core RAM, need mapping out. */
#define BT_RAM_ORIGIN_APP_MAPPING 0xA6000000
#define BT_RAM_ORIGIN_APP_MAPPING_LENGTH BCPU_RAM_LENGTH

/* 48 byte for BCPU system clocks status */
#define BCPU_SYSTEM_CLOCKS_LENGTH 0x30

/* 4096 bits for virtual otp */
#define OTP_SIZE_IN_BITS  4096 /* Size of OTP memory in bits */
#define OTP_SIZE_IN_BYTES ((OTP_SIZE_IN_BITS) / 8)

#define BCPU_SYSTEM_CLOCKS_ORIGIN (BT_USE_RAM_ORIGIN + BT_USE_RAM_LENGTH - BCPU_SYSTEM_CLOCKS_LENGTH)

/* the minimum BT heap size (50kB) */
#define BT_HEAP_MINIMUM_SIZE 0xC800

/* the minimum APP heap size */
#define APP_HEAP_MINIMUM_SIZE (APP_HEAP_SIZE)

/* for update the updater */
#define UPDATER_PRESERVED_LENGTH 0x10000
#define UPDATER_PRESERVED_ORIGIN (BCPU_SYSTEM_CLOCKS_ORIGIN - UPDATER_PRESERVED_LENGTH)

#define BCPU_NEW_STACKS_LENGTH 0x10000
#define BCPU_NEW_STACKS_ORIGIN (UPDATER_PRESERVED_ORIGIN - BCPU_NEW_STACKS_LENGTH)

/* Define the Vectors table location in ram, used by link script. */
/* BCPU (SSB, RECOVERY, BT) VECTORS region */
#define BCPU_VECTORS_ORIGIN (BCPU_RAM_START)
#define BCPU_RAM_END        (BCPU_RAM_START + BCPU_RAM_LENGTH)
/* BT core actually used ram, whole bt ram exclude VECTORS TABLE */
#define BT_USE_RAM_LENGTH (BCPU_RAM_LENGTH - BT_VECTORS_LENGTH)
#define BT_USE_RAM_ORIGIN (BCPU_RAM_END - BT_USE_RAM_LENGTH)

#define BCPU_USER_STACK_LEN      0x800
#define BCPU_IRQ_STACK_LEN       0x1000
#define BCPU_EXCP_STACK_LEN      0x800
#define BCPU_NMI_STACK_LEN       0X800
#define BCPU_STACK_LEN           (BCPU_USER_STACK_LEN + BCPU_IRQ_STACK_LEN + BCPU_EXCP_STACK_LEN + BCPU_NMI_STACK_LEN)

/* ----------------------------------------------------------------------------------------------------------------- */
/* APP RAM defines
 * APP has base 192K ITCM (Instruction TCM) for code
 *              448K DTCM (Data TCM) for DATA.
 * APP ITCM                 VECTORS TABLE
 *                          RAM TEXT
 *
 * APP DTCM                 STACK
 *                          RAM
 */
/* 192K ITCM for APP core code, start at end of vectors table */
#define APP_RAMTEXT_ORIGIN (APP_ITCM_ORIGIN + APP_VECTORS_LENGTH)
#define APP_RAMTEXT_END    (APP_ITCM_ORIGIN + APP_ITCM_LENGTH)
#define APP_RAMTEXT_LENGTH (APP_RAMTEXT_END - APP_RAMTEXT_ORIGIN)

/* 448K DTCM for APP core data */
/* stack for normal 10k */
#define APP_USER_STACK_BASEADDR APP_DTCM_ORIGIN
#define APP_USER_STACK_LEN      0xC00
#define APP_USER_STACK_LIMIT    (APP_USER_STACK_BASEADDR + APP_USER_STACK_LEN)

/* stack for irq 4k */
#define APP_IRQ_STACK_BASEADDR APP_USER_STACK_LIMIT
#define APP_IRQ_STACK_LEN      0xC00
#define APP_IRQ_STACK_LIMIT    (APP_IRQ_STACK_BASEADDR + APP_IRQ_STACK_LEN)

/* stack for exception 4k */
#define APP_EXCP_STACK_BASEADDR APP_IRQ_STACK_LIMIT
#define APP_EXCP_STACK_LEN      0xC00
#define APP_EXCP_STACK_LIMIT    (APP_EXCP_STACK_BASEADDR + APP_EXCP_STACK_LEN)

/* stack for nmi 4k */
#define APP_NMI_STACK_BASEADDR APP_EXCP_STACK_LIMIT
#define APP_NMI_STACK_LEN      0xC00
#define APP_NMI_STACK_LIMIT    (APP_NMI_STACK_BASEADDR + APP_NMI_STACK_LEN)

/* 448K DTCM for APP core data, start at end of vectors table */
#define APP_RAM_ORIGIN (APP_NMI_STACK_LIMIT)
#define APP_RAM_END    (APP_DTCM_ORIGIN + APP_DTCM_LENGTH)
#define APP_RAM_LENGTH (APP_RAM_END - APP_RAM_ORIGIN)

/* BT Core can also access the APP core RAM, need mapping out. */
#define APP_ITCM_ORIGIN_BT_MAPPING 0x4C000000
#define APP_DTCM_ORIGIN_BT_MAPPING 0x4E000000

/* Define the Vectors table location in ram, used by link script. */
/* APP CPU VECTORS region */
#define APP_VECTORS_ORIGIN (APP_ITCM_ORIGIN)

/* ----------------------------------------------------------------------------------------------------------------- */
/* DSP RAM use and manage by DSP team, M7 just need to know the access mapping. */
#define DSP_CODE_ADDR_BT_MAPPING 0x58000000

/* 12*N bytes for cpu trace, trace line is 12 bytes, 8184 bytes */
#define MCPU_TRACE_MEM_REGION_START 0x52006000
#define BCPU_TRACE_MEM_REGION_START 0x5900E000
#define CPU_TRACE_MEM_REGION_LENGTH 0x1FF8

/* IPC Mail box region, every core need have a send mail box to other cores
 * so there is 6 mailbox totally, use (3KB+16B) * 3 share mem.
 */
#define IPC_MAILBOX_REGION_START  (SHARED_MEM_START)
#define IPC_MAILBOX_REGION_LENGTH 0x4860

/* 176 bytes for BT core preserve region */
#define PRESERVED_REGION_ORIGIN (IPC_MAILBOX_REGION_START + IPC_MAILBOX_REGION_LENGTH)
#define PRESERVED_REGION_LENGTH 0xfc

/* 176 bytes for App core preserve region */
#define APP_PRESERVED_REGION_ORIGIN (PRESERVED_REGION_ORIGIN + PRESERVED_REGION_LENGTH)
#define APP_PRESERVED_REGION_LENGTH 0x0

/* fix the memory map layout */
#define IPC_MAILBOX_REGION_FIX_LENGTH (IPC_MAILBOX_REGION_LENGTH + \
                                       PRESERVED_REGION_LENGTH + APP_PRESERVED_REGION_LENGTH) // 0x49C0

/* System config region. */
#define SYSTEM_CFG_REGION_START  (APP_PRESERVED_REGION_ORIGIN + APP_PRESERVED_REGION_LENGTH)
#define SYSTEM_CFG_REGION_LENGTH 0x40

/* Reboot magic region. */
#define REBOOT_MAGIC_START  (SYSTEM_CFG_REGION_START + SYSTEM_CFG_REGION_LENGTH)
#define REBOOT_MAGIC_LENGTH 0x80

#define SYSTEM_PM_REGION_START (REBOOT_MAGIC_START + REBOOT_MAGIC_LENGTH)
#define SYSTEM_PM_REGION_LENGTH 0xC0

/* System status region. */
#define SYSTEM_STATUS_ORIGIN (SYSTEM_PM_REGION_START + SYSTEM_PM_REGION_LENGTH)
#define SYSTEM_STATUS_LENGTH 0xc0

/* Mass data region */
/* wear(BT - 1K, APP - 1K)  tws(BT - 1K, APP - 2K) */
#define MASSDATA_REGION_START   (SYSTEM_STATUS_ORIGIN + SYSTEM_STATUS_LENGTH)
#define MASSDATA_REGION_LENGTH  (BT_MASSDATA_LENGTH + APP_MASSDATA_LENGTH)

/* LOG Region */
/* wear(BT - 7K, APP - 3K, DSP - 7.5K) tws(BT - 7K, APP - 7K, DSP - 8K) */
#define BT_LOGGING_LENGTH  0x1C00

#define LOGGING_REGION_START  (MASSDATA_REGION_START + MASSDATA_REGION_LENGTH)
#define LOGGING_REGION_LENGTH (BT_LOGGING_LENGTH + APP_LOGGING_LENGTH + DSP_LOGGING_LENGTH)

/* Trng data shared memory 512byte */
#define TRNG_DATA_REGION_START  (LOGGING_REGION_START + LOGGING_REGION_LENGTH)
#define TRNG_DATA_REGION_LENGTH 0x200

/* Prompt tone region 10K for tws and 5K for wear */
#define PROMPT_TONE_REGION_START  (TRNG_DATA_REGION_START + TRNG_DATA_REGION_LENGTH)

/* Audio data stream region 8K for tws and 20K for wear */
#define AUDIO_DATA_STREAM_REGION_START  (PROMPT_TONE_REGION_START + PROMPT_TONE_REGION_LENGTH)

/* RESERVED share mem */
#define RESERVED_SHARE_MEM_ORIGIN (AUDIO_DATA_STREAM_REGION_START + AUDIO_DATA_STREAM_REGION_LENGTH)
#define RESERVED_SHARE_MEM_LENGTH (SHARED_MEM_START + SHARED_MEM_LENGTH - RESERVED_SHARE_MEM_ORIGIN)

#if (RESERVED_SHARE_MEM_LENGTH < 0)
#error Share memory overflow
#endif

#ifndef BUILD_APPLICATION_ATE
/* BT DIAG region, Use L2RAM  */
#define BT_DIAG_REGION_MEMORY_START  0x80100000
#define BT_DIAG_REGION_MEMORY_LENGTH 0x20000
#define BT_DIAG_REGION_MEMORY_END    (BT_DIAG_REGION_MEMORY_START + BT_DIAG_REGION_MEMORY_LENGTH)
#endif

/*
 * ********************* ADDITIONAL MEMORY CONFIGURATION DEFINITIONS *********************
 * The offset in the BCPU (either flash or ROM) to where the version information starts.
 */
#define VERSION_INFORMATION_OFFSET (BT_VECTORS_LENGTH)

/**
 * @}
 */
#endif