#!/usr/bin/env python3
# encoding=utf-8
# ============================================================================
# @brief    packet files
# ============================================================================

import os
import sys
import shutil

PY_PATH = os.path.dirname(os.path.realpath(__file__))
sys.path.append(PY_PATH)
PKG_DIR = os.path.dirname(PY_PATH)
PKG_DIR = os.path.dirname(PKG_DIR)

from packet_create import create_sha_file
from packet_create import packet_bin

TOOLS_DIR = os.path.dirname(PKG_DIR)
SDK_DIR = os.path.dirname(TOOLS_DIR)
sys.path.append(os.path.join(SDK_DIR, "build", "script"))

# bs25
def make_all_in_one_packet(pack_style_str):
    # make all in one packet
    if pack_style_str.startswith("pack"):
        bin_dir = os.path.join(SDK_DIR, "output", "package", "bs25", pack_style_str)
        application_path = os.path.join(bin_dir, "application.bin")
    else:
        application_path = os.path.join(SDK_DIR,
                                    "output",
                                    "bs25",
                                    "acore",
                                    pack_style_str,
                                    "application.bin")
        bin_dir = os.path.join(SDK_DIR, "interim_binary", "bs25", "bin")
    evb_all_fwpkg = os.path.join(PKG_DIR, "fwpkg", "bs25", "evb_all.fwpkg")
    evb_app_fwpkg = os.path.join(PKG_DIR, "fwpkg", "bs25", "evb_app.fwpkg")
    evb_nv_fwpkg = os.path.join(PKG_DIR, "fwpkg", "bs25", "evb_nv_bin.fwpkg")
    evb_partition_fwpkg = os.path.join(PKG_DIR, "fwpkg", "bs25", "evb_partition_bin.fwpkg")

    ssb = os.path.join(bin_dir, "ssb.bin")
    app = application_path
    bt = os.path.join(bin_dir, "bt.bin")
    nv = os.path.join(SDK_DIR, "interim_binary", "bs25", "bin", "bs25_all_nv.bin")
    partition = os.path.join(SDK_DIR, "tools", "pkg", "bin", "bs25", "partition.bin")
    if pack_style_str.endswith("flash"):
        ssb_bx = ssb + "|0x98000000|0x18000|0"
        app_bx = app + "|0x9807c000|0x84000|18"
        bt_bx = bt + "|0x9802c000|0x50000|20"
        nv_bx = nv + "|0x900fc000|0x4000|100"
        partition_bx = partition + "|0x900fb000|0x1000|100"
    else:
        ssb_bx = ssb + "|0x8c400000|0x18000|0"
        app_bx = app + "|0x8c47c000|0x84000|18"
        bt_bx = bt + "|0x8c42c000|0x50000|20"
        nv_bx = nv + "|0x8c5fc000|0x4000|100"
        partition_bx = partition + "|0x8c5fb000|0x1000|100"
    try:
        packet_post_agvs = list()
        packet_post_agvs.append(ssb_bx)
        packet_post_agvs.append(bt_bx)
        packet_post_agvs.append(app_bx)
        packet_post_agvs.append(nv_bx)
        packet_bin(evb_all_fwpkg, packet_post_agvs)

        packet_post_agvs = list()
        packet_post_agvs.append(app_bx)
        packet_bin(evb_app_fwpkg, packet_post_agvs)

        packet_post_agvs = list()
        packet_post_agvs.append(ssb_bx)
        packet_post_agvs.append(nv_bx)
        packet_bin(evb_nv_fwpkg, packet_post_agvs)

        packet_post_agvs = list()
        packet_post_agvs.append(ssb_bx)
        packet_post_agvs.append(partition_bx)
        packet_bin(evb_partition_fwpkg, packet_post_agvs)

        if pack_style_str.startswith("pack"):
            shutil.copy(evb_all_fwpkg, bin_dir)
            shutil.copy(evb_app_fwpkg, bin_dir)
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
    if pack_style_str.startswith("pack"):
        pack_path = os.path.join(SDK_DIR, "output", "package", "bs25", pack_style_str)
        packing_files = [
            os.path.join(pack_path, "ssb.bin"),
            os.path.join(pack_path, "application.bin"),
            os.path.join(pack_path, "bt.bin")
        ]
    else:
        packing_files.append(os.path.join(SDK_DIR, "interim_binary", "bs25", "bin", "ssb.bin"))
        application_path = os.path.join(SDK_DIR,
                                        "output",
                                        "bs25",
                                        "acore",
                                        pack_style_str,
                                        "application.bin")
        packing_files.append(application_path)
        packing_files.append(os.path.join(SDK_DIR, "interim_binary", "bs25", "bin", "bt.bin"))
    return packing_files