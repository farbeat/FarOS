#!/usr/bin/env python3
# encoding=utf-8
# ============================================================================
# @brief    Target Definitions File (MCU + Bluetooth slim build)
# Copyright CompanyNameMagicTag 2022-2022. All rights reserved.
# ============================================================================

target = {
    # mcu-bt: LiteOS MCU + 蓝牙协议栈 + AT 命令，无 GUI/多媒体/OTA/运动算法
    'brandy-mcu-bt': {
        'board': 'evb',
        'base_target_name': 'target_standard_brandy_application_template',
        'defines': ['PRE_ASIC', 'BRANDY_PRODUCT_EVB4', 'VERSION_STANDARD', '__LITEOS__',
                    'BRANDY_MCU_BT',
                    'CONFIG_OTA_UPDATE_SUPPORT', '_ALL_SOURCE', 'SUPPORT_CXX',
                    'I2C_SLAVE_REG_ADDR_4BYTE', 'CONFIG_ZDIAG_NV_SUPPORT',
                    "-:TARGET_CHIP_BRANDY=1", "-:BRANDY_CHIP_V100=1", 'CFG_DRIVERS_NANDFLASH',
                    'SUPPORT_BLE', 'SUPPORT_BREDR', 'SW_UART_DEBUG', 'SAVE_EXC_INFO',
                    'ENABLE_ECC', 'HASH_MEM_COPY',
                    'SUPPORT_AT_CMD', 'SUPPORT_AT_BT_MANAGER',
                    'CONFIG_DFX_SUPPORT_DIAG_UP_MACHINE=1', 'SUPPORT_RC_CALIBRATION'],
        'ram_component': ["algorithm", "dfx_port_brandy", "dfx_update", 'dfx_nv', "osal", "arch_port", "-:testsuite",
                          "liteos_port", 'partition', 'partition_brandy', 'dfx_file_operation',
                          'non_os', 'ulp_aon', 'hal_l2ram', 'x_vfs_private',
                          'x_dpal', 'x_vfs', 'x_disk', 'x_fat', 'drv_mmc', 'fs_yaffs2',
                          'pm_service', 'pm_brandy', 'cmn_header', 'at_cmd', 'app_at_service',
                          'bt_manager_at_service', 'bts_header', 'button_port'],
        'ram_component_set': ['bgh', 'update_app', 'dfx_set', 'pinctrl',
                              '-:button', '-:calendar', '-:mem_monitor'],
        'dsp_version': 'mini',
        'packet': True,
        'fs_image': False,
    },
    'brandy-mcu-bt-r': {
        'board': 'evb',
        'base_target_name': 'target_brandy_recovery_template',
        'defines': ['PRE_ASIC', 'BRANDY_PRODUCT_EVB4', 'VERSION_STANDARD', '__LITEOS__',
                    '_ALL_SOURCE', 'SUPPORT_CXX', 'I2C_SLAVE_REG_ADDR_4BYTE', 'CFG_DRIVERS_NANDFLASH',
                    "-:TARGET_CHIP_BRANDY=1", "-:BRANDY_CHIP_V100=1", 'SW_UART_DEBUG', 'HASH_MEM_COPY',
                    'ENABLE_ECC'],
        'ram_component': ["algorithm", "dfx_port_brandy", "dfx_update", "osal", "arch_port", "-:testsuite",
                          "liteos_port", 'dfx_file_operation', 'non_os', 'x_vfs_private',
                          'x_dpal', 'x_vfs', 'x_disk', 'x_fat', 'drv_mmc',
                          'ulp_aon', 'hal_l2ram', 'pm_service', 'pm_brandy', 'cmn_header'],
        'ram_component_set': ['pinctrl', 'dfx_set', '-:button'],
    },
    'brandy-ssb': {
        'base_target_name': 'target_ssb_template_brandy',
        'board': 'evb',
        'defines': ["-:TARGET_CHIP_BRANDY=1", "-:BRANDY_CHIP_V100=1", "PRE_ASIC", "BRANDY_PRODUCT_EVB4",
                    "SSB_USE_PLL", "VERSION_STANDARD", "BUILD_APPLICATION_SSB", "LIBBOOTLOADER", "LIBCODELOADER",
                    "LIBCPU_UTILS", "LIBAPP_VERSION", "LIBPANIC", "LIBCONNECTIVITY", "LIBERROR_CODE", "LIBSEC_RANDOM",
                    "LIBCODELOADER_SSB", "LIBJLINK_LOAD", "LIBLIB_UTILS", "LIBBUILD_VERSION",
                    "NO_TIMEOUT", "CONFIG_UART_BAUD_RATE_DIV_8", 'SW_UART_DEBUG', 'LARGE_BUFFER'],
        'ram_component': ['arch_port', 'hal_l2ram', 'sec_boot', 'libboundscheck',
                          'dfx_exception', 'error_code', 'psram', 'osal', 'cmn_header', 'boot_ulp', 'boot_nandflash', 'boot_emmc'],
        'ram_component_set': ['pinctrl'],
        'target': 'ssb',
    },
    # 兼容旧目标名
    'brandy-mcu-bt-ota': {
        'base_target_name': 'brandy-mcu-bt',
    },
    'brandy-mcu-bt-ota-r': {
        'base_target_name': 'brandy-mcu-bt-r',
    },
}

target_group = {
    'pack_brandy_mcu_bt': ['brandy-ssb', 'brandy-mcu-bt-r', 'brandy-mcu-bt'],
    'pack_brandy_mcu_bt_ota': ['brandy-ssb', 'brandy-mcu-bt-r', 'brandy-mcu-bt'],
}
