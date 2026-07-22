/*
 * Copyright (c) @CompanyNameMagicTag 2021-2022. All rights reserved.
 * Description:  psram memory configurations
 * Author: @CompanyNameTag
 * Create:  2022-03-09
 */

#ifndef PSRAM_32MB_H
#define PSRAM_32MB_H

#ifndef POOL_ADDR_ALIGNSIZE
#define POOL_ADDR_ALIGNSIZE 64
#endif

#define psram_align_size(x, y) (((x) + (y) - 1) & (~((y) - 1)))

#define PSRAM_BASE_ADDR                     0x6C000000
#define PSRAM_LENTH                         0x2000000

#define PSRAM_APP_SYN_LOAD_ADDR             PSRAM_BASE_ADDR
#define PSRAM_APP_SYN_LOAD_LENGTH           psram_align_size(0x10000, POOL_ADDR_ALIGNSIZE)

#define PSRAM_APP_CODE_ADDR                 (PSRAM_APP_SYN_LOAD_ADDR + PSRAM_APP_SYN_LOAD_LENGTH)
#define PSRAM_APP_CODE_LENGTH               psram_align_size(0xF0000, POOL_ADDR_ALIGNSIZE)

#define PSRAM_APP_DATA_ADDR                 (PSRAM_APP_CODE_ADDR + PSRAM_APP_CODE_LENGTH)
#define PSRAM_APP_DATA_LENGTH               psram_align_size(0x100000, POOL_ADDR_ALIGNSIZE)

/* Address used by the ACE of the application subsystem */
#define PSRAM_HM_APP_SYS_ADDR               (PSRAM_APP_DATA_ADDR + PSRAM_APP_DATA_LENGTH)
#define PSRAM_HM_APP_SYS_LENGTH             psram_align_size(0x2A800, POOL_ADDR_ALIGNSIZE)

#define PSRAM_USER_APP_RESERVE_ADDR         (PSRAM_HM_APP_SYS_ADDR + PSRAM_HM_APP_SYS_LENGTH)
#define PSRAM_USER_APP_RESERVE_LENGTH       psram_align_size(0x69E380, POOL_ADDR_ALIGNSIZE)

/* Heap start address */
#define PSRAM_HEAP_ADDR                     (PSRAM_USER_APP_RESERVE_ADDR + PSRAM_USER_APP_RESERVE_LENGTH)
#define PSRAM_HEAP_LENGTH                   psram_align_size(0x700000, POOL_ADDR_ALIGNSIZE)

/* plt reserve address */
#define PSRAM_PLT_RESERVE_ADDR              (PSRAM_HEAP_ADDR + PSRAM_HEAP_LENGTH)
#define PSRAM_PLT_RESERVE_LENGTH            psram_align_size(0x37480, POOL_ADDR_ALIGNSIZE)

/* GUI memory pool address NON Cache */
#define PSRAM_GUI_MEM_POOL_ADDR             (PSRAM_PLT_RESERVE_ADDR + PSRAM_PLT_RESERVE_LENGTH)
#define PSRAM_GUI_MEM_POOL_SIZE             psram_align_size(0xC00000, POOL_ADDR_ALIGNSIZE)

/* VAU reserve Address NON Cache */
#define PSRAM_DISP_VAU_RESERVE_ADDR         (PSRAM_GUI_MEM_POOL_ADDR + PSRAM_GUI_MEM_POOL_SIZE)
#define PSRAM_DISP_VAU_RESERVE_LENGTH       psram_align_size(0x40, POOL_ADDR_ALIGNSIZE)

/* VAU Linked List Address NON Cache */
#define PSRAM_DISP_VAU_NODE_ADDR            (PSRAM_DISP_VAU_RESERVE_ADDR + PSRAM_DISP_VAU_RESERVE_LENGTH)
#define PSRAM_DISP_VAU_NODE_LENGTH          psram_align_size(0x7FFC0, POOL_ADDR_ALIGNSIZE)

/* reserve */
#define PSRAM_RESERVE_ADDR                  (PSRAM_DISP_VAU_NODE_ADDR + PSRAM_DISP_VAU_NODE_LENGTH)
#define PSRAM_RESERVE_ADDR_LENGTH           psram_align_size(0x380000, POOL_ADDR_ALIGNSIZE)

#endif // PSRAM_H
