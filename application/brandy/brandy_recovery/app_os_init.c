/*
 * Copyright (c) CompanyNameMagicTag 2021-2021. All rights reserved.
 * Description:  Application Core OS Initialize Interface for Standard.
 * Author:
 * Create: 2021-03-09
 */

#include "cmsis_os2.h"
#include "los_status.h"
#include "pmu_interrupt.h"
#include "log_reg_dump.h"
#include "watchdog.h"
#include "preserve.h"
#include "hal_reboot.h"
#include "pinctrl_porting.h"
#include "partition.h"
#include "partition_resource_id.h"
#include "errno.h"
#include "upg.h"
#include "irmalloc.h"
#include "upg_definitions.h"
#include "upg_porting.h"
#include "non_os_reboot.h"
#include "debug_print.h"
#include "tcxo.h"
#include "cpu_utils.h"
#include "app_os_init.h"
#ifdef CFG_DRIVERS_NANDFLASH
#include "nandflash_config.h"
#include "yaffs_nand_drv.h"
#endif
#ifdef SUPPORT_RECOVERY_DISPLAY
#include "recovery_display.h"
#endif

#define TCXO_REBOOT_DELAY             500ULL
#define TASK_COMMON_APP_DELAY_MS 1000
#define OSDELAY  1000
#define UPG_FLAG_OFFSET_OF_FOTA_DATA (0x1000 * 2)

typedef void (*CreateQueueFunction_t)(void);

#define TASK_PRIORITY_APP (osPriority_t)(17)
#define TASK_PRIORITY_RPC (osPriority_t)(15)
#define TASK_PRIORITY_LOG (osPriority_t)(13)
#define TASK_PRIORITY_CMD (osPriority_t)(17)
#define TASK_PRIORITY_SDK ((osPriority_t)15)

#define BOOT_CTRL_MAGIC_FLAG        0x5A
#define FPGA_BT_BOOTUP_CTRL_REG     0x52000010
#define FPGA_DSP_BOOTUP_CTRL_REG    0x52000014
#define FPGA_BT_WAIT_CRTL_REG       0x570007b8
#define EMMC_BASE_ADDR              0x52062000

#define CMD_STACK_SIZE       0x1000
#define STACK_SIZE_BASELINE 0x200
#define FS_EXTEND_STACK (STACK_SIZE_BASELINE * 3)
#define BTH_SDK_STACK_SIZE (STACK_SIZE_BASELINE * 2 + FS_EXTEND_STACK)

typedef struct {
    osThreadAttr_t attr;
    osThreadFunc_t func;
    uint32_t *task_handle;
    CreateQueueFunction_t create_queue_fn_p;
} app_task_definition_t;

#define APP_MAIN_QUEUE_LEN 2

#define APP_MAIN_QUEUE_ITEM_SIZE (sizeof(uint32_t))

static void app_main(const void *unused);
void init_emmc_pinmux(void);
int emmc_drv_context_init(void);
void los_vfs_init(void);
uint32_t dpal_driver_base_init(void);

static app_task_definition_t g_app_tasks[] = {
    { { "app", 0, NULL, 0, NULL, (0x2000), TASK_PRIORITY_APP, 0, 0 },
      (osThreadFunc_t)app_main, NULL, NULL },
};

#define M_NUM_TASKS (sizeof(g_app_tasks) / sizeof(app_task_definition_t))

void app_os_init(const void *unused1, unsigned int unused2)
{
    UNUSED(unused1);
    UNUSED(unused2);

    // Create queues before tasks in case they are used during initialisation.
    for (uint8_t i = 0; i < M_NUM_TASKS; i++) {
        if (g_app_tasks[i].create_queue_fn_p) {
            g_app_tasks[i].create_queue_fn_p();
        }
    }
    // Now create tasks
    for (uint8_t i = 0; i < M_NUM_TASKS; i++) {
        // You can't configure a task with a higher priority than the kernel
        // has been compiled to support. RTOS will clip the priority level,
        // which is probably not what you wanted.
        //
        // Similarly, if the watchdog kick is in the idle task and it's
        // intended to detect any other task being stuck, you don't want any
        // other task to have a priority <= the idle task.
        g_app_tasks[i].task_handle = osThreadNew(g_app_tasks[i].func, NULL, &(g_app_tasks[i].attr));
        if (g_app_tasks[i].task_handle == NULL) {
            panic(PANIC_TASK_CREATE_FAILED, i);
        }
    }
}

static void emmc_init(void)
{
    /**
     * FPGA解复位, 设置CMD_CONFLICT_CHECK
     * 之后才能设置管脚
     */
    writew(EMMC_BASE_ADDR + 0x508, 0);

    init_emmc_pinmux();
    int ret = emmc_drv_context_init();
    PRINT("emmc init %s"NEWLINE, (ret == ENOERR) ? "success" : "error");
}

static void nandflash_init(void)
{
    nandflash_filesystem_init();
}

static bool upg_need_upgrade(void)
{
    uint32_t fota_address = 0;
    partition_information_t info;
    errcode_t ret_val = uapi_partition_get_info(PARTITION_FOTA_DATA, &info);
    if (ret_val != ERRCODE_SUCC || info.type != PARTITION_BY_ADDRESS) {
        return false;
    }
    fota_address = info.part_info.addr_info.addr;
    fota_upgrade_flag_area_t upg_flag_info;
    fota_address += UPG_FLAG_OFFSET_OF_FOTA_DATA;
    ret_val = upg_flash_read(fota_address, sizeof(fota_upgrade_flag_area_t), (uint8_t *)(&upg_flag_info));
    if (ret_val != ERRCODE_SUCC) {
        return false;
    }

    if (!(upg_flag_info.head_magic == UPG_HEAD_MAGIC &&
          upg_flag_info.head_end_magic == UPG_END_MAGIC &&
          upg_flag_info.complete_flag != 0)) {
        /* 不需要升级直接返回 */
        printf("No need to upgrade...\r\n");
        return false;
    }

    return true;
}

static void upg_putc(const char c)
{
    printf("%c", c);
}

static void app_main(const void *unused)
{
    UNUSED(unused);
    los_vfs_init();
    uint32_t ret = (uint32_t)dpal_driver_base_init();
    if (ret != 0) {
        panic(PANIC_ASSERT, __LINE__);
    }
#ifdef CFG_DRIVERS_MMC
    emmc_init();
#endif
#ifdef CFG_DRIVERS_NANDFLASH
    yaffs_nand_drv_checkptset(YAFFS_CHECKPT_SKIPED);
    nandflash_init();
#endif
#ifdef SUPPORT_RECOVERY_DISPLAY
    recovery_display_init();
    recovery_display_sample();
#endif
    printf("fs init success.\r\n");
    if (upg_need_upgrade()) {
        printf("need upgrade\r\n");
        upg_func_t upg_func = {0};
        upg_func.malloc = malloc;
        upg_func.free = free;
        upg_func.serial_putc = upg_putc;
        (void)uapi_upg_init(&upg_func);

        errcode_t err = uapi_upg_start();
        if (err != ERRCODE_SUCC) {
            printf("--------------------------\r\n");
            printf("upgrade failed, ret = 0x%x\r\n", ret);
        } else {
            // 升级后重启
            printf("--------------------------\r\n");
            printf("upgrade success, reset now\r\n");
        }
    } else {
        // 不应该进入该分支
        printf("--------------------------\r\n");
        printf("no need upgrade, this branch will never enter\r\n");
    }
    upg_reboot();
}
