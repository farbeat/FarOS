#!/usr/bin/env python3
# encoding=utf-8
# ============================================================================
# @brief    packet files
# ============================================================================

import os
import sys

PY_PATH = os.path.dirname(os.path.realpath(__file__))
sys.path.append(PY_PATH)
PKG_DIR = os.path.dirname(PY_PATH)
PKG_DIR = os.path.dirname(PKG_DIR)

from packet_create import create_sha_file
from packet_create import packet_bin

TOOLS_DIR = os.path.dirname(PKG_DIR)
SDK_DIR = os.path.dirname(TOOLS_DIR)
sys.path.append(os.path.join(SDK_DIR, "build", "script"))

# ws53
def make_all_in_one_packet(pack_style_str):
    # make all in one packet
    bin_dir = os.path.join(SDK_DIR, "interim_binary", "ws53", "bin", "boot_bin")
    loadboot = os.path.join(bin_dir, "loaderboot_sign.bin")
    loadboot_bx = loadboot + "|0x0|0x0|0"
    partition = os.path.join(bin_dir, "partition.bin")
    partition_bx = partition + "|0x400000|0x1000|1"
    flashboot = os.path.join(bin_dir, "flashboot_sign_a.bin")
    flashboot_bx = flashboot + "|0x401000|0x8000|1"
    flashboot_bak = os.path.join(bin_dir, "flashboot_sign_b.bin")
    flashboot_bak_bx = flashboot_bak + "|0x409000|0x8000|1"
    print(pack_style_str)
    if "btc" in pack_style_str:
        sub_dir = "btc_only"
        acore_rom = os.path.join(SDK_DIR, "output", "ws53", "acore", "liteos_ws53_btc_only", "application_rom.bin")
        acore_rom_bx = acore_rom + "|0x1A800|0x0|1"
        acore_app = os.path.join(SDK_DIR, "output", "ws53", "acore", "liteos_ws53_btc_only", "application_sign.bin")
        acore_app_bx = acore_app + "|0x411000|0x200000|1"
        ccore_rom = os.path.join(SDK_DIR, "output", "ws53", "control_core", "control_ws53_btc_only", "control_ws53_btc_only_rom.bin")
        ccore_rom_bx = ccore_rom + "|0x611000|0x48000|1"
        ccore_app = os.path.join(SDK_DIR, "output", "ws53", "control_core", "control_ws53_btc_only", "control_ws53_btc_only_sign.bin")
        ccore_app_bx = ccore_app + "|0x659000|0xAF000|1"
    elif "bt" in pack_style_str or "bgle" in pack_style_str:
        sub_dir = "bt_only"
        acore_rom = os.path.join(SDK_DIR, "output", "ws53", "acore", "liteos_ws53_bt_only", "application_rom.bin")
        acore_rom_bx = acore_rom + "|0x1A800|0x0|1"
        acore_app = os.path.join(SDK_DIR, "output", "ws53", "acore", "liteos_ws53_bt_only", "application_sign.bin")
        acore_app_bx = acore_app + "|0x411000|0x200000|1"
        ccore_rom = os.path.join(SDK_DIR, "output", "ws53", "control_core", "control_ws53_bgle_all", "control_ws53_bgle_all_rom.bin")
        ccore_rom_bx = ccore_rom + "|0x611000|0x48000|1"
        ccore_app = os.path.join(SDK_DIR, "output", "ws53", "control_core", "control_ws53_bgle_all", "control_ws53_bgle_all_sign.bin")
        ccore_app_bx = ccore_app + "|0x659000|0xAF000|1"
    elif "wifi" in pack_style_str:
        sub_dir = "wifi_only"
        acore_rom = os.path.join(SDK_DIR, "output", "ws53", "acore", "liteos_ws53_wifi_only", "application_rom.bin")
        acore_rom_bx = acore_rom + "|0x1A800|0x0|1"
        acore_app = os.path.join(SDK_DIR, "output", "ws53", "acore", "liteos_ws53_wifi_only", "application_sign.bin")
        acore_app_bx = acore_app + "|0x411000|0x200000|1"
        ccore_rom = os.path.join(SDK_DIR, "output", "ws53", "control_core", "control_ws53_wifi_only", "control_ws53_rom.bin")
        ccore_rom_bx = ccore_rom + "|0x611000|0x48000|1"
        ccore_app = os.path.join(SDK_DIR, "output", "ws53", "control_core", "control_ws53_wifi_only", "control_ws53_sign.bin")
        ccore_app_bx = ccore_app + "|0x659000|0xAF000|1"
    elif "plt" in pack_style_str:
        sub_dir = "plt_only"
        acore_rom = os.path.join(SDK_DIR, "output", "ws53", "acore", "liteos_ws53_plt_only", "application_rom.bin")
        acore_rom_bx = acore_rom + "|0x1A800|0x0|1"
        acore_app = os.path.join(SDK_DIR, "output", "ws53", "acore", "liteos_ws53_plt_only", "application_sign.bin")
        acore_app_bx = acore_app + "|0x411000|0x200000|1"
        ccore_rom = os.path.join(SDK_DIR, "output", "ws53", "control_core", "control_ws53_plt_only", "control_ws53_rom.bin")
        ccore_rom_bx = ccore_rom + "|0x611000|0x48000|1"
        ccore_app = os.path.join(SDK_DIR, "output", "ws53", "control_core", "control_ws53_plt_only", "control_ws53_sign.bin")
        ccore_app_bx = ccore_app + "|0x659000|0xAF000|1"
    elif "master" in pack_style_str:
        sub_dir = "plt_wifi_bt_master"
        acore_rom = os.path.join(SDK_DIR, "output", "ws53", "acore", "liteos_ws53_master", "application_rom.bin")
        acore_rom_bx = acore_rom + "|0x1A800|0x0|1"
        acore_app = os.path.join(SDK_DIR, "output", "ws53", "acore", "liteos_ws53_master", "application_sign.bin")
        acore_app_bx = acore_app + "|0x411000|0x200000|1"
        ccore_rom = os.path.join(SDK_DIR, "output", "ws53", "control_core", "control_ws53_master", "control_ws53_rom.bin")
        ccore_rom_bx = ccore_rom + "|0x611000|0x48000|1"
        ccore_app = os.path.join(SDK_DIR, "output", "ws53", "control_core", "control_ws53_master", "control_ws53_sign.bin")
        ccore_app_bx = ccore_app + "|0x659000|0xAF000|1"
    else:
        sub_dir = "plt_wifi_bt"
        acore_rom = os.path.join(SDK_DIR, "output", "ws53", "acore", "liteos_ws53", "application_rom.bin")
        acore_rom_bx = acore_rom + "|0x1A800|0x0|1"
        acore_app = os.path.join(SDK_DIR, "output", "ws53", "acore", "liteos_ws53", "application_sign.bin")
        acore_app_bx = acore_app + "|0x411000|0x200000|1"
        ccore_rom = os.path.join(SDK_DIR, "output", "ws53", "control_core", "control_ws53", "control_ws53_rom.bin")
        ccore_rom_bx = ccore_rom + "|0x611000|0x48000|1"
        ccore_app = os.path.join(SDK_DIR, "output", "ws53", "control_core", "control_ws53", "control_ws53_sign.bin")
        ccore_app_bx = ccore_app + "|0x659000|0xAF000|1"

    try:
        if os.path.exists(acore_app):
            packet_post_agvs = list()
            packet_post_agvs.append(loadboot_bx)
            packet_post_agvs.append(partition_bx)
            packet_post_agvs.append(flashboot_bx)
            packet_post_agvs.append(flashboot_bak_bx)
            packet_post_agvs.append(acore_rom_bx)
            packet_post_agvs.append(acore_app_bx)
            fpga_fwpkg = os.path.join(SDK_DIR, "output", "ws53", "fwpkg", sub_dir, "fpga_acore_all.fwpkg")
            packet_bin(fpga_fwpkg, packet_post_agvs)

            packet_post_agvs = list()
            packet_post_agvs.append(loadboot_bx)
            packet_post_agvs.append(acore_rom_bx)
            packet_post_agvs.append(acore_app_bx)
            fpga_loadapp_only_fwpkg = os.path.join(SDK_DIR, "output", "ws53", "fwpkg", sub_dir, "fpga_acore_load_only.fwpkg")
            packet_bin(fpga_loadapp_only_fwpkg, packet_post_agvs)

        if os.path.exists(ccore_app):
            packet_post_agvs = list()
            packet_post_agvs.append(loadboot_bx)
            packet_post_agvs.append(partition_bx)
            packet_post_agvs.append(flashboot_bx)
            packet_post_agvs.append(flashboot_bak_bx)
            packet_post_agvs.append(ccore_rom_bx)
            packet_post_agvs.append(ccore_app_bx)
            fpga_fwpkg = os.path.join(SDK_DIR, "output", "ws53", "fwpkg", sub_dir, "fpga_ccore_all.fwpkg")
            packet_bin(fpga_fwpkg, packet_post_agvs)

            packet_post_agvs = list()
            packet_post_agvs.append(loadboot_bx)
            packet_post_agvs.append(ccore_rom_bx)
            packet_post_agvs.append(ccore_app_bx)
            fpga_loadapp_only_fwpkg = os.path.join(SDK_DIR, "output", "ws53", "fwpkg", sub_dir, "fpga_ccore_load_only.fwpkg")
            packet_bin(fpga_loadapp_only_fwpkg, packet_post_agvs)

        if os.path.exists(acore_app) and os.path.exists(ccore_app):
            packet_post_agvs = list()
            packet_post_agvs.append(loadboot_bx)
            packet_post_agvs.append(partition_bx)
            packet_post_agvs.append(flashboot_bx)
            packet_post_agvs.append(flashboot_bak_bx)
            packet_post_agvs.append(acore_rom_bx)
            packet_post_agvs.append(acore_app_bx)
            packet_post_agvs.append(ccore_rom_bx)
            packet_post_agvs.append(ccore_app_bx)
            fpga_fwpkg_all = os.path.join(SDK_DIR, "output", "ws53", "fwpkg", "pack_all_core", sub_dir, "fpga_all_in_one.fwpkg")
            packet_bin(fpga_fwpkg_all, packet_post_agvs)

            packet_post_agvs = list()
            packet_post_agvs.append(loadboot_bx)
            packet_post_agvs.append(acore_rom_bx)
            packet_post_agvs.append(acore_app_bx)
            packet_post_agvs.append(ccore_rom_bx)
            packet_post_agvs.append(ccore_app_bx)
            fpga_loadapp_only_fwpkg = os.path.join(SDK_DIR, "output", "ws53", "fwpkg", "pack_all_core", sub_dir, "fpga_load_only.fwpkg")
            packet_bin(fpga_loadapp_only_fwpkg, packet_post_agvs)

    except Exception as e:
        print(e)
        exit(-1)
    sub_dir = "plt_wifi_bt_base"
    acore_rom = os.path.join(SDK_DIR, "output", "ws53", "acore", "liteos_ws53_base", "application_rom.bin")
    acore_rom_bx = acore_rom + "|0x1A800|0x0|1"
    acore_app = os.path.join(SDK_DIR, "output", "ws53", "acore", "liteos_ws53_base", "application_sign.bin")
    acore_app_bx = acore_app + "|0x411000|0x200000|1"
    ccore_rom = os.path.join(SDK_DIR, "output", "ws53", "control_core", "control_ws53", "control_ws53_rom.bin")
    ccore_rom_bx = ccore_rom + "|0x611000|0x48000|1"
    ccore_app = os.path.join(SDK_DIR, "output", "ws53", "control_core", "control_ws53", "control_ws53_sign.bin")
    ccore_app_bx = ccore_app + "|0x659000|0xAF000|1"
    try:
        if os.path.exists(acore_app):
            packet_post_agvs = list()
            packet_post_agvs.append(loadboot_bx)
            packet_post_agvs.append(partition_bx)
            packet_post_agvs.append(flashboot_bx)
            packet_post_agvs.append(flashboot_bak_bx)
            packet_post_agvs.append(acore_rom_bx)
            packet_post_agvs.append(acore_app_bx)
            fpga_fwpkg = os.path.join(SDK_DIR, "output", "ws53", "fwpkg", sub_dir, "fpga_acore_all.fwpkg")
            packet_bin(fpga_fwpkg, packet_post_agvs)

            packet_post_agvs = list()
            packet_post_agvs.append(loadboot_bx)
            packet_post_agvs.append(acore_rom_bx)
            packet_post_agvs.append(acore_app_bx)
            fpga_loadapp_only_fwpkg = os.path.join(SDK_DIR, "output", "ws53", "fwpkg", sub_dir, "fpga_acore_load_only.fwpkg")
            packet_bin(fpga_loadapp_only_fwpkg, packet_post_agvs)

        if os.path.exists(ccore_app):
            packet_post_agvs = list()
            packet_post_agvs.append(loadboot_bx)
            packet_post_agvs.append(partition_bx)
            packet_post_agvs.append(flashboot_bx)
            packet_post_agvs.append(flashboot_bak_bx)
            packet_post_agvs.append(ccore_rom_bx)
            packet_post_agvs.append(ccore_app_bx)
            fpga_fwpkg = os.path.join(SDK_DIR, "output", "ws53", "fwpkg", sub_dir, "fpga_ccore_all.fwpkg")
            packet_bin(fpga_fwpkg, packet_post_agvs)

            packet_post_agvs = list()
            packet_post_agvs.append(loadboot_bx)
            packet_post_agvs.append(ccore_rom_bx)
            packet_post_agvs.append(ccore_app_bx)
            fpga_loadapp_only_fwpkg = os.path.join(SDK_DIR, "output", "ws53", "fwpkg", sub_dir, "fpga_ccore_load_only.fwpkg")
            packet_bin(fpga_loadapp_only_fwpkg, packet_post_agvs)

        if os.path.exists(acore_app) and os.path.exists(ccore_app):
            packet_post_agvs = list()
            packet_post_agvs.append(loadboot_bx)
            packet_post_agvs.append(partition_bx)
            packet_post_agvs.append(flashboot_bx)
            packet_post_agvs.append(flashboot_bak_bx)
            packet_post_agvs.append(acore_rom_bx)
            packet_post_agvs.append(acore_app_bx)
            packet_post_agvs.append(ccore_rom_bx)
            packet_post_agvs.append(ccore_app_bx)
            fpga_fwpkg_all = os.path.join(SDK_DIR, "output", "ws53", "fwpkg", "pack_all_core", sub_dir, "fpga_all_in_one.fwpkg")
            packet_bin(fpga_fwpkg_all, packet_post_agvs)

            packet_post_agvs = list()
            packet_post_agvs.append(loadboot_bx)
            packet_post_agvs.append(acore_rom_bx)
            packet_post_agvs.append(acore_app_bx)
            packet_post_agvs.append(ccore_rom_bx)
            packet_post_agvs.append(ccore_app_bx)
            fpga_loadapp_only_fwpkg = os.path.join(SDK_DIR, "output", "ws53", "fwpkg", "pack_all_core", sub_dir, "fpga_load_only.fwpkg")
            packet_bin(fpga_loadapp_only_fwpkg, packet_post_agvs)
    except Exception as e:
        print(e)
        exit(-1)





def is_packing_files_exist(soc, pack_style_str):
    return
