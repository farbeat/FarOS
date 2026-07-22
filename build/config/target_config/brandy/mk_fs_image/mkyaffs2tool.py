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

root_path = os.path.join(os.path.split(os.path.realpath(__file__))[0], '..', '..', '..', '..', '..')
cur_dir = os.path.dirname(os.path.abspath(__file__))
image_tool_path = os.path.realpath(os.path.join(root_path, 'tools', 'bin', 'platform_tool', 'mkyaffs2image'))

# 配置生成文件镜像工具路径
mkyaffstool = os.path.join(image_tool_path, 'mkyaffs2image')
image_name = "file"

# 配置nandflash信息
cfg_page_size = 2048
cfg_obb_size = 64

# 配置分区信息，请根据实际情况进行配置，添加格式为[分区路径， 分区大小]
# 分区大小需要根据按照百分系统分配好后的blocks数进行计算，大小=blocks（文件块个数）*pages_per_block（每个文件块包含的页数）*bytes_per_page（每页包含字节数）
# 前面按照比例分好分区，最后分区置成null，即代表剩下所有blocks
partition_list = [
    [os.path.join(cur_dir, 'fs', 'system'), "0xcc0000"],
    [os.path.join(cur_dir, 'fs', 'update'), "0xcc0000"],
    [os.path.join(cur_dir, 'fs', 'music'), "0xcc0000"],
    [os.path.join(cur_dir, 'fs', 'user'), "null"]
]

# 将生成image文件转换大小端，保存成另外文件
def convert(src_file, dest_file):
    fp_src = open(src_file, 'rb')
    fp_dest = open(dest_file, 'wb')
    src_size = os.path.getsize(src_file)
    for i in range(int(src_size / 4)):
        data = fp_src.read(4)
        rw = struct.pack('BBBB', data[3], data[2], data[1], data[0])
        fp_dest.write(rw)
    fp_src.close()
    fp_dest.close()

def get_page_type(page_size):
    if page_size == 2048:
        return " 1"
    elif page_size == 4096:
        return " 2"
    else:
        return " 0"

def get_obb_type(obb_size):
    if obb_size == 64:
        return " 2"
    elif page_size == 128:
        return " 4"
    else:
        return " 0"

if __name__ == "__main__":
    image_file = os.path.join(cur_dir, "%s.bin" %image_name)
    tmp_file = os.path.join(cur_dir, "%s_tmp.bin" %image_name)

    if sys.platform == 'linux2' or sys.platform == 'linux':
        partition_total = 0
        page_type =  get_page_type(cfg_page_size)
        obb_type = get_obb_type(cfg_obb_size)

        if os.path.exists(tmp_file):
            os.remove(tmp_file)
        if os.path.exists(image_file):
            os.remove(image_file)

        for partition in partition_list:
            if (partition[1] != "null"):
                partition_total = partition_total + int(partition[1][2:], 16)
                command = mkyaffstool + " " + partition[0] + " " + tmp_file + page_type + obb_type + (" 0x%x" %partition_total)
            else:
                command = mkyaffstool + " " + partition[0] + " " + tmp_file + page_type + obb_type
            print(command)
            os.system(command)
        convert(tmp_file, image_file)

    if (len(sys.argv) == 2 and os.path.exists(image_file)):
        out_path = os.path.join(sys.argv[1], "%s.bin" %image_name)
        shutil.copyfile(image_file, out_path)

    print("make yaffs image file over!")