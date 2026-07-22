/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2018-2020. All rights reserved.
 * Description:   DRV MMC DRIVER HEADER FILE
 * Author:
 * Create:  2020-04
 */
#ifndef DRV_MMC_ADAPTER_H
#define DRV_MMC_ADAPTER_H

#include "mmcsd_host.h"
#include "mmcsd_card.h"
#include "hal_mmc_common.h"
/*
 * Card Command Classes (CCC)
 */
#define CCC_BASIC           (1 << 0)
#define CCC_BLOCK_READ      (1 << 2)
#define CCC_BLOCK_WRITE     (1 << 4)
#define CCC_ERASE           (1 << 5)
#define CCC_WRITE_PROT      (1 << 6)
#define CCC_LOCK_CARD       (1 << 7)
#define CCC_APP_SPEC        (1 << 8)
#define CCC_IO_MODE         (1 << 9)
#define CCC_SWITCH          (1 << 10)
#define MMC_CMD_DATA_REQ_TIMEOUT  (1000 * 10) // 30s
#define MMC_CMD_REQ_TIMEOUT  (1000 * 3)       // 3s
#define MMC_TUNINT_REQ_TIMEOUT (1000 / 5)     // 200ms

#define is_sd_cmd23_support(card)  ((card)->scr.cmds & SD_SCR_CMD23_SUPPORT)
#define is_mmc_cmd23_support(card)  \
    (((card)->csd.card_cmd_class & CCC_BLOCK_READ) || ((card)->csd.card_cmd_class & CCC_BLOCK_WRITE))
#define CART_PLUG_STATE         (MSHC_INT_CARD_INSERT | MSHC_INT_CARD_REMOVE)
#define CONFIG_ERASE_MAX_SECTOR  0x100000 // 512M
#define ADMA2_TRANS_END         0x2
#define NON_OS_WAIT_INT_PER_MSEC    1
#define WAIT_RSP_PER_USEC           1
#define NON_OS_WAIT_SUM_TIMES       1000000
#define MMC_ERASE_CMD_RETRIES_TIMES 5
#define MMC_ERASE_MAX_TIME_MS       6000
#define ADMA_EVERY_ITEM_LEN         4
#define MMC_HOST_MAX_BLKS           65535
#define MMC_RCA_OFFSET_VAL          16
#define EMMC_DMA_ALIGN              4

#ifndef LOSCFG_LIB_LIBC
#define ENODMAALIGN         1
#define ENOCACHEALIGN       2
#define EPARAMERR           3
#define EIO                 5
#define ENOMEM              12
#define EACCES              13
#define EBUSY               16
#define ETIME               62
#define EOPNOTSUPP          95 /* Operation not supported on socket */

#endif

#define DATA_CMD_AUTO_STOP (0x1 << 6) // for struct rt_mmcsd_data.flags

typedef enum {
    MMC_ERASE_TYPE_ERASE   = 0x0,
    MMC_ERASE_TYPE_TRIM    = 0x1,
    MMC_ERASE_TYPE_DISCARD = 0x3,
    MMC_ERASE_TYPE_MAX
} mmc_erase_type_t;

typedef struct {
    uint8_t magic;
    uint8_t mmc_cmd_code;
    uint16_t mmc_err_int_stat;
    uint32_t mmc_present_stat;
    int32_t mmc_cmd_err;
    int32_t mmc_cmd_data_err;
    uint32_t mmc_cmd_resp[4];
} mmc_err_info_type_t;

typedef enum {
    EMMC_NORMAL_MAGIC_VAL = 0xAC,
    EMMC_ERROR_MAGIC_VAL  = 0xDE,
} mmc_err_flag_t;

int test_emmc_init(void);
struct rt_mmcsd_host* get_ssb_emmc(void);
int32_t ssb_emmc_read(const char *buffer, uint32_t start_sector, uint32_t nsectors);
int32_t ssb_emmc_write(const char *buffer, uint32_t start_sector, uint32_t nsectors);
int32_t ssb_emmc_erase(uint32_t start_sector, uint32_t nsectors);

struct rt_mmcsd_host *hi_mmc_get_host(uint32_t id);

int hi_mmc_exec_tuning(struct rt_mmcsd_host *mmc, uint32_t opcode);

void hi_mmc_set_timing(struct rt_mmcsd_host *mmc, int timing);

uint32_t hi_mmc_write(const void *buffer, uint32_t pos, uint32_t size);

uint32_t hi_mmc_read(void *buffer, uint32_t pos, uint32_t size);

uint32_t hi_mmc_pio_write(const void *buffer, uint32_t pos, uint32_t size);

uint32_t hi_mmc_pio_read(void *buffer, uint32_t pos, uint32_t size);

int hi_mmc_card_busy(struct rt_mmcsd_host *mmc);

void hi_mmc_handle_non_os_xfer(struct rt_mmcsd_host *mmc, struct rt_mmcsd_req *req);

void hi_mmc_set_clock(struct rt_mmcsd_host *mmc, unsigned int clocknum);

void hi_mmc_set_os_interrupt(struct rt_mmcsd_host *mmc, bool en);

void hi_mmc_set_interrup_status(bool en);

void mmc_set_bus_width(void);

void hi_mmc_set_bus_width(struct rt_mmcsd_host *mmc, int width);

int mmc_switch(struct rt_mmcsd_card *card, rt_uint8_t set, rt_uint8_t index, rt_uint8_t value);

void mmc_set_sdiom_clk_mode(int clk_type);

void hi_mmc_set_drv_features(struct hi_mmc_drv *mmc_drv, uint8_t id);

int hi_mmc_set_int_info(struct rt_mmcsd_host *mmc, struct hi_mmc_drv *mmc_drv);

void hi_mmc_set_host_features(struct rt_mmcsd_host *mmc);

void hi_mmc_set_drv_bus_width_features(struct hi_mmc_drv *mmc_drv);

void mmc_set_driver_capability(struct rt_mmcsd_host* mmc, uint32_t cap);

bool mmc_detect_bus_error(struct rt_mmcsd_host* host);

void hi_mmc_set_err_to_aon_area(void);

void hi_mmc_reset_cmd_data(struct rt_mmcsd_host* host);

uint32_t hi_mmc_get_present_state(struct rt_mmcsd_host *host);

int hi_mmc_send_stop_cmd(struct rt_mmcsd_host *mmc, uint32_t *status);

mmc_err_info_type_t *hi_mmc_get_err_from_aon_area(void);

void hi_mmc_set_magic_to_aon_area(void);
#endif
