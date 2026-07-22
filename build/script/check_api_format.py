#!/usr/bin/env python3
# encoding=utf-8
# ============================================================================
# Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved. \n
# Description: API format check \n
# Author: @CompanyNameTag \n
# ============================================================================

import re
import os
import sys
import logging

MODULE_LIST = ["driver", "bts", "wifi", "utils"]

logging.basicConfig(
    filename="check_api_resut.txt",
    level=logging.NOTSET,
    format='[%(asctime)s] [%(levelname)s] - %(message)s')


def file_path_list(folder_path):
    logging.debug("Start to read file name list...")
    file_paths = []  # 存储目录下的所有文件名，含路径
    for root, _, files in os.walk(folder_path):
        for file in files:
            file_paths.append(os.path.join(root, file))
    return file_paths


def api_read(file_path, api_dic):
    if os.path.splitext(file_path)[-1] != ".h":
        return api_dic
    try:
        with open(file_path, 'r', encoding='utf-8') as f:
            txt = f.read()
    except OSError as e:
        logging.error("error file name: %s", file_path)
        raise UnicodeDecodeError('invalid encoding byte because {}.'.format(e))

    res = re.findall(r"/\*\*.+? \* @brief(.*?)\n.*?\*/.*?\n(.*?);", txt, re.S | re.M)

    for i in res:
        i = list(i)
        if "(" in i[1] and "{" not in i[1]:
            api_dic[i[1]] = file_path
    return api_dic


def function_handle(name):
    class_name = name.split(" ", 1)[0]
    function_name = name.split(" ", 1)[1]
    return class_name, function_name


def count_duplicate_module(count_dict, module, sub_module):
    if "total" in count_dict:
        count_dict["total"] += 1
    else:
        count_dict["total"] = 1

    if module in count_dict:
        count_dict[module]["total"] += 1
        if sub_module in count_dict[module]:
            count_dict[module][sub_module] += 1
        else:
            count_dict[module][sub_module] = 1
    else:
        count_dict[module] = {}
        count_dict[module][sub_module] = 1
        count_dict[module]["total"] = 1
    return count_dict

g_tabs_indent_bar = 10 * '-'
g_tabs_indent_space = 10 * ' '
def report_module_api(count_dict, detail):
    print("CFBB total APIs:", str(count_dict["total"]))
    for module in count_dict:
        if module in "total":
            continue
        print(g_tabs_indent_bar, module, " total APIs:", count_dict[module]["total"])
        if not detail:
            continue
        module_sum = 0
        for sub_module in count_dict[module]:
            if sub_module in "total":
                continue
            module_sum += count_dict[module][sub_module]
            print(g_tabs_indent_space, g_tabs_indent_bar, sub_module, " APIs:\t", count_dict[module][sub_module])

        if count_dict[module]["total"] != module_sum:
            raise ImportError("module_sum APIs is not correct")

def save_to_excel(dic, save2file):
    if save2file:
        try:
            import openpyxl
        except ImportError:
            raise ImportError("please install openpyxl")
        wb = openpyxl.Workbook()
        wb.create_sheet("log", 0)
        ws = wb["log"]
        ws.append(["API name", "module name", "file path"])

    count_dict = {}
    for i in dic:
        class_name, func_name = function_handle(i)
        module_name = "unknown"
        for m in MODULE_LIST:
            if m in dic[i]:
                module_name = m
                break
        d = ("%s %s" % (class_name, func_name)), module_name, dic[i]
        sub_module = os.path.basename(dic[i])
        count_dict = count_duplicate_module(count_dict, module_name, sub_module)
        if class_name != "typedef":
            if func_name.startswith("uapi_") is not True and module_name == "driver":
                logging.error("API format is incorrect(don't start with uapi_): %s", func_name)
        if save2file:
            ws.append(d)
    report_module_api(count_dict, True)
    if save2file:
        filename = "api_list.xlsx"
        wb.save(filename)
    logging.info("Saved successfully.")


if __name__ == '__main__':
    curr_path = os.getcwd()
    if os.path.isdir(curr_path):
        path_list = file_path_list(curr_path)
        result = {}
        for k in path_list:
            result = api_read(k, result)
        logging.info("API count: %s", len(result))
        save_to_excel(result, False)
    else:
        logging.error("error path!")
