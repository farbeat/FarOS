#!/usr/bin/env python3
# encoding=utf-8
# ============================================================================
# @brief    Build Update Pkg File
# Copyright CompanyNameMagicTag 2023-2023. All rights reserved.
# ============================================================================

import os
import sys

file_dir = os.path.dirname(os.path.realpath(__file__))
g_root = os.path.realpath(os.path.join(file_dir, "..", "..", "..", ".."))
sys.path.append(os.path.join(g_root, 'build', 'script'))
from build_upg_pkg import get_parameters, begin
from make_upg_res_pack import res_pack

# 使用此脚本生成升级包，请注意修改以下几点：
#   1. class upg_base_info 中的产品路径、产品镜像签名路径等等仅为举例，需要根据实际情况进行修改
#       尤其bt、dsp_main、dsp_overlay的相关路径，实际并不存在，请根据实际情况进行修改
#   2. 在 get_new_image() 中对想要升级的镜像进行增减，仅打开想要升级的镜像即可
#   3. 通过在fota.cfg中各镜像配置项中的 DecompressFlag，修改为想要的升级方式，若修改为差分升级的方式，
#       则需要在 get_old_image() 中进行增减，仅保留升级方式为差分的镜像即可
#   4. 升级的校验方式分为安全校验和非安全校验，详细修改方式请详见开发文档
#   5. 修改完成后，运行此脚本即可在默认路径(output/brandy/upgrade)下生成升级包update.fwpkg

class upg_base_info:
    def __init__(self):
        self.root_path = g_root
        # 升级包结构配置文件
        self.fota_format_path = os.path.join(self.root_path, "build", "config", "target_config", "brandy", "fota")
        # 产品升级配置文件
        self.fota_cfg = os.path.join(self.root_path, "build", "config", "target_config", "brandy", "fota", "fota.cfg")
        # 产品镜像输出路径
        self.output = os.path.join(self.root_path, "output", "brandy")
        # 产品升级镜像包输出路径
        self.upg_output = os.path.join(self.output, "upgrade")
        # 产品升级制作临时文件输出路径
        self.temp_dir = os.path.join(self.upg_output, "temp_dir")

        # 产品镜像路径 (此处路径仅为参考，需要根据实际使用的target修改)
        # 以下bt、dsp_main、dsp_overlay的路经并不存在，需要根据实际修改
        self.ssb_bin         = os.path.join(self.output, "acore", "brandy-ssb", "ssb.bin")
        self.recovery_bin    = os.path.join(self.output, "acore", "brandy-target1-r", "recovery.bin")
        self.app_bin         = os.path.join(self.output, "acore", "brandy-target1", "application.bin")
        self.bt_bin          = os.path.join(self.output, "bt_core", "bt.bin")
        self.dsp_main_bin    = os.path.join(self.output, "dsp", "dsp_main.bin")
        self.dsp_overlay_bin = os.path.join(self.output, "dsp", "dsp_overlay.bin")
        self.nv_bin          = os.path.join(self.output, "nv_bin", "brandy_all_nv.bin")
        # 产品镜像签名路径 (此处路径仅为参考，需要根据实际使用的target修改)
        self.ssb_sign         = os.path.join(self.output, "acore", "brandy-ssb", "ssb_sign.bin")
        self.recovery_sign    = os.path.join(self.output, "acore", "brandy-target1-r", "recovery_sign.bin")
        self.app_sign         = os.path.join(self.output, "acore", "brandy-target1", "application_sign.bin")
        self.bt_sign          = os.path.join(self.output, "bt_core", "bt_sign.bin")
        self.dsp_main_sign    = os.path.join(self.output, "dsp", "dsp_main_sign.bin")
        self.dsp_overlay_sign = os.path.join(self.output, "dsp", "dsp_overlay_sign.bin")

        # 资源文件路径，根据需要自行修改添加
        self.res_dir          = os.path.join(self.output, "res_file")
        self.res_index        = os.path.join(self.output, "res_index.bin")
        self.res_data         = os.path.join(self.output, "res_data.bin")

        # 差分升级时，以下路径配置为上一个版本的镜像路径 (此处路径仅为参考，需要根据实际使用的target修改)
        self.app_old_bin         = os.path.join(self.output, "old_version", "application.bin")
        self.bt_old_bin          = os.path.join(self.output, "old_version", "bt.bin")
        self.dsp_main_old_bin    = os.path.join(self.output, "old_version", "dsp_main.bin")
        self.dsp_overlay_old_bin = os.path.join(self.output, "old_version", "dsp_overlay.bin")
        # 旧产品镜像签名路径 (此处路径仅为参考，需要根据实际使用的target修改)
        self.app_old_sign         = os.path.join(self.output, "old_version", "application_sign.bin")
        self.bt_old_sign          = os.path.join(self.output, "old_version", "bt_sign.bin")
        self.dsp_main_old_sign    = os.path.join(self.output, "old_version", "dsp_main_sign.bin")
        self.dsp_overlay_old_sign = os.path.join(self.output, "old_version", "dsp_overlay_sign.bin")

def get_new_image(info):
    # 要升级的image列表，需根据升级的实际情况进行增删
    image_list = []
    image_list.append("=".join([info.ssb_bin,          "ssb"]))
    image_list.append("=".join([info.recovery_bin,     "recovery"]))
    image_list.append("=".join([info.app_bin,          "application"]))
    # image_list.append("=".join([info.bt_bin,           "bt"]))
    # image_list.append("=".join([info.dsp_main_bin,     "dsp_main"]))
    # image_list.append("=".join([info.dsp_overlay_bin,  "dsp_overlay"]))
    # image_list.append("=".join([info.nv_bin,           "nv"]))

    # 资源文件
    # result = res_pack(info.res_dir, info.res_index, info.res_data)
    # if not result:
    #     sys.exit("[ERROR] resource file pack failed!!!")
    # image_list.append("=".join([info.res_index,        "res_index"]))
    # image_list.append("=".join([info.res_data,         "res_data"]))

    # 镜像签名
    # image_list.append("=".join([info.ssb_sign,         "ssb_sign"]))
    # image_list.append("=".join([info.recovery_sign,    "recovery_sign"]))
    # image_list.append("=".join([info.app_sign,         "application_sign"]))
    # image_list.append("=".join([info.bt_sign,          "bt_sign"]))
    # image_list.append("=".join([info.dsp_main_sign,    "dsp_main_sign"]))
    # image_list.append("=".join([info.dsp_overlay_sign, "dsp_overlay_sign"]))

    new_image = "|".join(image_list)
    return new_image


def get_old_image(info):
    # 要进行差分升级的image的旧版本镜像列表
    image_list = []
    # image_list.append("=".join([info.app_old_bin,          "application"]))
    # image_list.append("=".join([info.bt_old_bin,           "bt"]))
    # image_list.append("=".join([info.dsp_main_old_bin,     "dsp_main"]))
    # image_list.append("=".join([info.dsp_overlay_old_bin,  "dsp_overlay"]))

    # image_list.append("=".join([info.app_old_sign,         "application_sign"]))
    # image_list.append("=".join([info.bt_old_sign,          "bt_sign"]))
    # image_list.append("=".join([info.dsp_main_old_sign,    "dsp_main_sign"]))
    # image_list.append("=".join([info.dsp_overlay_old_sign, "dsp_overlay_sign"]))

    old_image = "|".join(image_list)
    return old_image

if __name__ == '__main__':
    info = upg_base_info()

    conf = get_parameters()
    conf.app_name        = "update"
    conf.upg_format_path = info.fota_format_path
    conf.base            = info.fota_cfg
    conf.temp_dir        = info.temp_dir
    conf.new_images      = get_new_image(info)
    conf.old_images      = get_old_image(info)
    conf.output_dir      = info.upg_output
    conf.type            = 0
    begin(conf)
