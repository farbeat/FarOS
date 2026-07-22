#!/usr/local/bin/python3.7
# -*- coding: utf-8 -*-

import os
import sys
import time
import struct
import shutil

"""
介绍：
    本脚本主要实现调用mkyaffs2image工具生成文件镜像
    请注意使用本脚本时支持的python版本，本脚本只支持python3版本。
    请根据实际情况配置对应信息，主要是nandflash的信息，以及分区信息

"""

cur_dir = os.getcwd()
image_tool_path = os.getcwd()

# 配置生成文件镜像工具路径
mkfatimgtool = os.path.join(image_tool_path, 'mkfatimg.exe')
image_name = "file"

# 配置emmc信息
cfg_sector_size = 512

# 配置分区信息，请根据实际情况进行配置，添加格式为[分区路径， 分区大小]
# 分区大小需要根据按照百分系统分配好后的sector数进行计算，大小=sector(对应区域扇区数) * cfg_sector_size(单扇区字节数) / 1024(KB单位换算)
# 分区大小必须指定，按照当前系统默认emmc总容量4GB, 第一分区system大小为11%，换算分区大小为461260
partition_list = [
    [os.path.join(cur_dir, 'fs'), "461260"],
]

if __name__ == "__main__":
    image_file = os.path.join(cur_dir, "%s.bin" %image_name)
    tmp_file = os.path.join(cur_dir, "%s_tmp.bin" %image_name)
    sector_size = " 4096"
    ret = 0

    if sys.platform == 'win32':
        if os.path.exists(image_file):
            os.remove(image_file)

        for partition in partition_list:
            command = mkfatimgtool + " " + partition[0] + " " + image_file + " " + partition[1] + sector_size
            print(command)
            ret = os.system(command)
    else:
        print("sys platform err!not windows system!\n")

    if ret == 0:
        print("make fat image file success!")
    else:
        print("make fat image file fail!")
