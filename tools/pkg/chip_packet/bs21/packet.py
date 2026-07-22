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

# bs21
def make_all_in_one_packet(pack_style_str):
    # make all in one packet
    bin_dir = os.path.join(SDK_DIR, "interim_binary", "bs21", "bin", "boot_bin")

    application_dir = os.path.join(SDK_DIR, "output", "bs21", "acore", pack_style_str)

    fpga_fwpkg = os.path.join(SDK_DIR, "output", "bs21", "fwpkg", pack_style_str, "fpga_all_in_one.fwpkg")
    fpga_loadapp_only_fwpkg = os.path.join(SDK_DIR, "output", "bs21", "fwpkg", pack_style_str, "fpga_loadapp_only_fwpkg.fwpkg")

    partition = os.path.join(bin_dir, "partition.bin")
    loadboot = os.path.join(bin_dir, "loaderboot_sign.bin")
    flashboot = os.path.join(bin_dir, "flashboot_sign_a.bin")
    flashboot_bak = os.path.join(bin_dir, "flashboot_sign_b.bin")

    loadboot_bx = loadboot + "|0x0|0x0|0"
    partition_bx = partition + "|0x90100000|0x1000|1"
    flashboot_bx = flashboot + "|0x90101000|0x8000|1"
    flashboot_bak_bx = flashboot_bak + "|0x90109000|0x8000|1"

    app = os.path.join(application_dir, "application_sign.bin")
    app_bx = app + "|0x90111000|0xee000|1"

    try:
        packet_post_agvs = list()
        packet_post_agvs.append(loadboot_bx)
        packet_post_agvs.append(partition_bx)
        packet_post_agvs.append(flashboot_bx)
        packet_post_agvs.append(flashboot_bak_bx)
        packet_post_agvs.append(app_bx)

        packet_bin(fpga_fwpkg, packet_post_agvs)

        packet_post_agvs = list()
        packet_post_agvs.append(loadboot_bx)
        packet_post_agvs.append(app_bx)
        packet_bin(fpga_loadapp_only_fwpkg, packet_post_agvs)

    except Exception as e:
        print(e)
        exit(-1)


def is_packing_files_exist(soc, pack_style_str):
    """
    判断打包文件是否存在
    :return:
    """
    packing_files = get_packing_files(soc, pack_style_str)
    lost_files = list()
    for f_path in packing_files:
        if not os.path.isfile(f_path):
            lost_files.append(f_path)
    return lost_files

def get_packing_files(soc, pack_style_str):
    """
    直接添加需要打包的文件路径
    :return:
    """

    packing_files = list()
    application_path = os.path.join(SDK_DIR,
                                    "output",
                                    "bs21",
                                    "acore",
                                    pack_style_str,
                                    "application.bin")
    packing_files.append(application_path)
    return packing_files
