/*
 * Copyright (c) CompanyNameMagicTag 2022-2023. All rights reserved.
 * Description: header of log memory section
 * Author: audio
 * Create: 2022-10-15
 */

#ifndef LOG_MEMORY_SECTION_H
#define LOG_MEMORY_SECTION_H

#include "td_type.h"
#include "chip_type.h"

typedef enum {
    LOG_MEMORY_REGION_SECTION_0 = 0, /* LOG_MEMORY_REGION_SECTION_0 for bt core */
    LOG_MEMORY_REGION_SECTION_1,     /* LOG_MEMORY_REGION_SECTION_1 for application core */
    LOG_MEMORY_REGION_SECTION_2,     /* LOG_MEMORY_REGION_SECTION_2 for hifi core when dsp exists. */
    LOG_MEMORY_REGION_MAX_NUMBER,    /* LOG_MEMORY_REGION_MAX_NUMBER */
} log_memory_region_section;

typedef struct {
    volatile td_u32 read;
    volatile td_u32 write;
} log_memory_section_control;

typedef struct {
    td_u32 offset[LOG_MEMORY_REGION_MAX_NUMBER];
    td_u32 length;
    log_memory_section_control section_control[LOG_MEMORY_REGION_MAX_NUMBER];
} log_memory_region_control;

typedef struct {
    td_u8 *start;
    td_u32 length;
} log_memory_section_params;

#define SHARED_MEM_START              0x87000000

/*
 * IPC Mail box region, every core need have a send mail box to other cores
 * so there is 6 mailbox totally, use (3KB+16B) * 3 share mem.
 */
#define IPC_MAILBOX_REGION_START      (SHARED_MEM_START)
#define IPC_MAILBOX_REGION_LENGTH     0x4860

/* BT core preserve region */
#define PRESERVED_REGION_ORIGIN       (IPC_MAILBOX_REGION_START + IPC_MAILBOX_REGION_LENGTH)
#define PRESERVED_REGION_LENGTH       0xfc

/* App core preserve region */
#define APP_PRESERVED_REGION_ORIGIN   (PRESERVED_REGION_ORIGIN + PRESERVED_REGION_LENGTH)
#define APP_PRESERVED_REGION_LENGTH   0x0

/* System config region. */
#define SYSTEM_CFG_REGION_START       (APP_PRESERVED_REGION_ORIGIN + APP_PRESERVED_REGION_LENGTH)
#define SYSTEM_CFG_REGION_LENGTH      0x40

/* Reboot magic region. */
#define REBOOT_MAGIC_START            (SYSTEM_CFG_REGION_START + SYSTEM_CFG_REGION_LENGTH)
#define REBOOT_MAGIC_LENGTH           0x80

/* System clock region. */
#define SYSTEM_CLK_REGION_START       (REBOOT_MAGIC_START + REBOOT_MAGIC_LENGTH)
#define SYSTEM_CLK_REGION_LENGTH      0xC0

/* System status region. */
#define SYSTEM_STATUS_ORIGIN          (SYSTEM_CLK_REGION_START + SYSTEM_CLK_REGION_LENGTH)
#define SYSTEM_STATUS_LENGTH          0xC0

/* Mass data region */
/* BT - 1K, APP - 2K */
#define BT_MASSDATA_LENGTH            0x400
#define APP_MASSDATA_LENGTH           0x800

#define MASSDATA_REGION_START         (SYSTEM_STATUS_ORIGIN + SYSTEM_STATUS_LENGTH)
#define MASSDATA_REGION_LENGTH        (BT_MASSDATA_LENGTH + APP_MASSDATA_LENGTH)

/* LOG Region */
/* BT - 7K, APP - 7K, DSP - 8K */
#define BT_LOGGING_LENGTH             0x1C00
#define APP_LOGGING_LENGTH            0x1C00
#define DSP_LOGGING_LENGTH            0x2000

#if (SAP_CHIP_TYPE == socmn1)
#define LOGGING_REGION_START 0x87008840
#else
#define LOGGING_REGION_START          (MASSDATA_REGION_START + MASSDATA_REGION_LENGTH)                /* 0x8700579C */
#endif
#define LOGGING_REGION_LENGTH         (BT_LOGGING_LENGTH + APP_LOGGING_LENGTH + DSP_LOGGING_LENGTH)   /* 5800 */

void log_memory_section_get(log_memory_region_section section, log_memory_section_params *section_data);

log_memory_section_control *log_memory_section_get_control(log_memory_region_section section);

#define pointer_in_log_memory_region(x) (((td_u32)(x) >= (LOGGING_REGION_START)) && \
                                         ((td_u32)(x) <= ((LOGGING_REGION_START) + (LOGGING_REGION_LENGTH))))

#endif
