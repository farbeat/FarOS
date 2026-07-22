#ifndef _BOARD_H_
#define _BOARD_H_

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define STACK_SIZE              0x800
#define FLASH_BASE_ADDR         0x00300000
#define IMAGE_HEADER_SIZE       0x300
#define FLASH_KERENL_ADDRESS    (FLASH_BASE_ADDR + 0x1D5000)
#define FLASH_KERENL_LENGTH     0x4F9000
#define SRAM_DATA_ADDRESS       0x10013800
#define SRAM_DATA_LENGTH        0x77000      /* 476K */
#define SRAM_IPC_ADDRESS        0x10091000
#define SRAM_IPC_LENGTH         0xb000
#define SRAM_GCC_TEXT_VMA       0x10000000
#define SRAM_GCC_TEXT_SIZE      0xc00

/* PSRAM */
#define PSRAM_DATA_VMA      0x208c8b80
#define PSRAM_DATA_SIZE     0xA76200

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif
