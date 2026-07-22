/**
* Copyright (c) @CompanyNameMagicTag 2021-2022. All rights reserved. \n
* Description: header file for AT CMD\n
* Author: @CompanyNameTag \n
* History: \n
* 2023-07-31, Create file. \n
*/

#ifndef APP_AT_CMD_H
#define APP_AT_CMD_H

/**
 * @defgroup null AT CMD Description
 * @ingroup
 * @{
 */

/**
 * @if Eng
 * @brief  AT^HEAPINFO
 * @par Description: Querying the Heap Usage of the System
 * @param  [in]  No Param
 * @retval OK~                    Success
 * @retval ERR~                   Fail
 * @else
 * @brief  AT^HEAPINFO
 * @par 说明: 查询系统的堆使用情况
 * @param  [in]  无参数
 * @retval OK~                    成功返回，打印堆使用情况
 * @retval ERR~                   失败返回，无打印
 * @endif
 */
AT^HEAPINFO;

/**
 * @if Eng
 * @brief  AT^STACKINFO
 * @par Description: Querying the stack Usage of the System
 * @param  [in]  No Param
 * @retval OK~                    Success
 * @retval ERR~                   Fail
 * @else
 * @brief  AT^STACKINFO
 * @par 说明: 查询系统的栈使用情况
 * @param  [in]  无参数
 * @retval OK~                    成功返回，打印栈使用情况
 * @retval ERR~                   失败返回，无打印
 * @endif
 */
AT^STACKINFO;

/**
 * @if Eng
 * @brief  AT^RESET
 * @par Description: System reset
 * @param  [in]  No Param
 * @retval OK~                    Success
 * @retval ERR~                   Fail
 * @else
 * @brief  AT^RESET
 * @par 说明: 系统复位
 * @param  [in]  无参数
 * @retval OK~                    成功返回，系统重启
 * @retval ERR~                   失败返回
 * @endif
 */
AT^RESET;

/**
 * @if Eng
 * @brief  AT^FSRWTEST
 * @par Description: FileSystem RW Test, write 1M data to file
 * @par and then compare the read file data with the written data.
 * @param  [in]  No Param
 * @retval OK~                    Success
 * @retval ERR~                   Fail
 * @else
 * @brief  AT^FSRWTEST
 * @par 说明: 文件写读测试，将1M数据写入文件，然后读取全部文件数据与写入的数据进行比较
 * @param  [in]  无参数
 * @retval OK~                    成功返回，读写文件对比一致
 * @retval ERR~                   失败返回，读写文件有异常
 * @endif
 */
AT^FSRWTEST;

/**
 * @if Eng
 * @brief  AT^FSFORMAT
 * @par Description: EMMC/NANDFLASH Format
 * @param  [in]  No Param
 * @retval OK~                    Success
 * @retval ERR~                   Fail
 * @else
 * @brief  AT^FSFORMAT
 * @par 说明: 外设(EMMC/NANDFLASH)格式化命令
 * @param  [in]  无参数
 * @retval OK~                    成功返回，格式化成功
 * @retval ERR~                   失败返回，格式化失败
 * @endif
 */
AT^FSFORMAT;

/**
 * @if Eng
 * @brief  AT^FSSETCLK
 * @par Description: Set EMMC Clock，98M or 32M
 * @param  [in]  0  98M
 * @param  [in]  1  32M
 * @retval OK~                    Success
 * @retval ERR~                   Fail
 * @else
 * @brief  AT^FSSETCLK
 * @par 说明: 配置EMMC时钟频率，有高频98M和低频32M可选
 * @param  [in]  0  高频98M
 * @param  [in]  1  低频32M
 * @retval OK~                    成功返回，配置成功
 * @retval ERR~                   失败返回，配置失败
 * @endif
 */
AT^FSSETCLK;

/**
 * @if Eng
 * @brief  AT^FSREADCLK
 * @par Description: read EMMC clock
 * @param  [in]  No Param
 * @retval OK~                    Success
 * @retval ERR~                   Fail
 * @else
 * @brief  AT^FSREADCLK
 * @par 说明: 读取EMMC时钟频率
 * @param  [in]  无参数
 * @retval OK~                    成功返回，读取成功
 * @retval ERR~                   失败返回，读取失败
 * @endif
 */
AT^FSREADCLK;

/**
 * @if Eng
 * @brief  AT^FSWRITE
 * @par Description: Create a file in /user/ and write data of a specified size
 * @param  [in]  ["file name", data size]
 * @retval OK~                    Success
 * @retval ERR~                   Fail
 * @else
 * @brief  AT^FSWRITE
 * @par 说明: 写文件命令，在/user/创建指定新文件，然后写入指定大小固定数据
 * @param  [in]  ["写文件名", 写数据大小] （文件名为字符串，长度小于20字节，写数据大小为大于0小于1M，默认转4K整数倍）
 * @retval OK~                    成功返回，写入成功
 * @retval ERR~                   失败返回，写入失败
 * @endif
 */
AT^FSWRITE;

/**
 * @if Eng
 * @brief  AT^FSCHECK
 * @par Description: Read a specified file in /user/, read a specified size
 * @par and compare the read data with the written fixed data.
 * @param  [in]  ["file name", data size]
 * @retval OK~                    Success
 * @retval ERR~                   Fail
 * @else
 * @brief  AT^FSCHECK
 * @par 说明: 读文件命令，读取/user/下指定文件，读取指定大小，并将读取数据与写入固定数据做对比
 * @param  [in]  ["读文件名", 读数据大小] （文件名为字符串，长度小于20字节，读数据大小为大于0小于1M，默认转4K整数倍）
 * @retval OK~                    成功返回，读取成功
 * @retval ERR~                   失败返回，读取失败
 * @endif
 */
AT^FSCHECK;

/**
 * @if Eng
 * @brief  AT^FSRM
 * @par Description: Delete all files in the target path and its subdirectories
 * @param  [in]  No Param
 * @retval OK~                    Success
 * @retval ERR~                   Fail
 * @else
 * @brief  AT^FSRM
 * @par 说明: 指定目标路径，删除目标路径及其子目录中所有文件
 * @param  [in]  ["目标路径"] 如是文件路径，则删除对应文件。如是目录路径，则删除整个目录
 * @retval OK~                    成功返回，删除成功
 * @retval ERR~                   失败返回，删除失败
 * @endif
 */
AT^FSRM;

/**
 * @if Eng
 * @brief  AT^GETWSTPVER
 * @par Description: Get application version
 * @param  [in]  No Param
 * @retval OK~                    Success
 * @retval ERR~                   Fail
 * @else
 * @brief  AT^GETWSTPVER
 * @par 说明: 获取当前A核版本版本号
 * @param  [in]  无参数
 * @retval OK~                    成功返回，获取版本号
 * @retval ERR~                   失败返回，获取失败
 * @endif
 */
AT^GETWSTPVER;

/**
 * @if Eng
 * @brief  AT^DTCMBW
 * @par Description: DTCM Bandwidth Test
 * @param  [in]  No Param
 * @retval OK~                    Success
 * @retval ERR~                   Fail
 * @else
 * @brief  AT^DTCMBW
 * @par 说明: DTCM带宽测试
 * @param  [in]  无参数
 * @retval OK~                    成功返回，显示带宽速率
 * @retval ERR~                   失败返回
 * @endif
 */
AT^DTCMBW;

/**
 * @if Eng
 * @brief  AT^L2MBW
 * @par Description: L2ram Bandwidth Test
 * @param  [in]  No Param
 * @retval OK~                    Success
 * @retval ERR~                   Fail
 * @else
 * @brief  AT^L2MBW
 * @par 说明: L2ram带宽测试
 * @param  [in]  无参数
 * @retval OK~                    成功返回，显示带宽速率
 * @retval ERR~                   失败返回
 * @endif
 */
AT^L2MBW;

/**
 * @if Eng
 * @brief  AT^MMCBW
 * @par Description: EMMC Bandwidth Test
 * @param  [in]  No Param
 * @retval OK~                    Success
 * @retval ERR~                   Fail
 * @else
 * @brief  AT^MMCBW
 * @par 说明: EMMC文件系统读写带宽测试
 * @param  [in]  无参数
 * @retval OK~                    成功返回，显示带宽速率
 * @retval ERR~                   失败返回
 * @endif
 */
AT^MMCBW;

/**
 * @if Eng
 * @brief  AT^PSRAMBW
 * @par Description: Psram Bandwidth Test
 * @param  [in]  No Param
 * @retval OK~                    Success
 * @retval ERR~                   Fail
 * @else
 * @brief  AT^PSRAMBW
 * @par 说明: Psram读写带宽测试
 * @param  [in]  无参数
 * @retval OK~                    成功返回，显示带宽速率
 * @retval ERR~                   失败返回
 * @endif
 */
AT^PSRAMBW;

/**
 * @if Eng
 * @brief  AT^NORFLASHBW
 * @par Description: NorFlash Bandwidth Test
 * @param  [in]  No Param
 * @retval OK~                    Success
 * @retval ERR~                   Fail
 * @else
 * @brief  AT^NORFLASHBW
 * @par 说明: NorFlash读带宽测试
 * @param  [in]  无参数
 * @retval OK~                    成功返回，显示带宽速率
 * @retval ERR~                   失败返回
 * @endif
 */
AT^NORFLASHBW;

/**
 * @if Eng
 * @brief  AT^NANDFLASHBW
 * @par Description: NandFlash Bandwidth Test
 * @param  [in]  No Param
 * @retval OK~                    Success
 * @retval ERR~                   Fail
 * @else
 * @brief  AT^NANDFLASHBW
 * @par 说明: NandFlash文件系统读写带宽测试
 * @param  [in]  无参数
 * @retval OK~                    成功返回，显示带宽速率
 * @retval ERR~                   失败返回
 * @endif
 */
AT^NANDFLASHBW;

/**
 * @if Eng
 * @brief  AT^FSINFO
 * @par Description: Queries the memory usage of each partition in the file system
 * @param  [in]  No Param
 * @retval OK~                    Success
 * @retval ERR~                   Fail
 * @else
 * @brief  AT^FSINFO
 * @par 说明: 查询文件系统各分区内存使用情况
 * @param  [in]  无参数
 * @retval OK~                    成功返回，显示文件系统使用情况
 * @retval ERR~                   失败返回
 * @endif
 */
AT^FSINFO;

/**
 * @if Eng
 * @brief  AT^PMADDVOTE
 * @par Description: Low-power wakeup command, exiting deep sleep mode, send twice
 * @param  [in]  No Param
 * @retval OK~                    Success
 * @retval ERR~                   Fail
 * @else
 * @brief  AT^PMADDVOTE
 * @par 说明: 低功耗唤醒命令，退出深睡模式，通常需要发送两次
 * @param  [in]  无参数
 * @retval OK~                    成功返回
 * @retval ERR~                   失败返回
 * @endif
 */
AT^PMADDVOTE;

/**
 * @if Eng
 * @brief  AT^PMRMVOTE
 * @par Description: Low-power sleep command, into deep sleep mode
 * @param  [in]  No Param
 * @retval OK~                    Success
 * @retval ERR~                   Fail
 * @else
 * @brief  AT^PMRMVOTE
 * @par 说明: 低功耗睡眠命令，进入深睡模式
 * @param  [in]  无参数
 * @retval OK~                    成功返回
 * @retval ERR~                   失败返回
 * @endif
 */
AT^PMRMVOTE;

/**
 * @if Eng
 * @brief  AT^LCDON
 * @par Description: Turn on the screen, display all the pre-operations, and open the cache
 * @param  [in]  No Param
 * @retval OK~                    Success
 * @retval ERR~                   Fail
 * @else
 * @brief  AT^LCDON
 * @par 说明: 点亮屏幕，所有显示操作的前置操作，并且打开Cache
 * @param  [in]  无参数
 * @retval OK~                    成功返回
 * @retval ERR~                   失败返回
 * @endif
 */
AT^LCDON;

/**
 * @if Eng
 * @brief  AT^LCDOFF
 * @par Description: Turn off the screen
 * @param  [in]  No Param
 * @retval OK~                    Success
 * @retval ERR~                   Fail
 * @else
 * @brief  AT^LCDOFF
 * @par 说明: 关闭屏幕，显示操作将全部失效
 * @param  [in]  无参数
 * @retval OK~                    成功返回
 * @retval ERR~                   失败返回
 * @endif
 */
AT^LCDOFF;

/**
 * @if Eng
 * @brief  AT^NANDAGINGTEST
 * @par Description: nandflash aging test
 * @param  [in] [percent num,times]
 * @retval OK~                    Success
 * @retval ERR~                   Fail
 * @else
 * @brief  AT^NANDAGINGTEST
 * @par 说明: Nandflash器件老化测试，按照指定百分比，从0地址进行指定次数的擦写测试
 * @param  [in]  [写入数据百分比（最大100）, 次数]
 * @retval OK~                    成功返回
 * @retval ERR~                   失败返回
 * @endif
 */
AT^NANDAGINGTEST;

/**
 * @if Eng
 * @brief  AT^FLASHAGINGTEST
 * @par Description: norflash aging test
 * @param  [in]  [times]
 * @retval OK~                    Success
 * @retval ERR~                   Fail
 * @else
 * @brief  AT^FLASHAGINGTEST
 * @par 说明: Norflash器件老化测试，当前只往预留空间写数据，擦写指定次数
 * @param  [in]  [次数]
 * @retval OK~                    成功返回
 * @retval ERR~                   失败返回
 * @endif
 */
AT^FLASHAGINGTEST;

/**
 * @if Eng
 * @brief  AT^ISDONGLECONN
 * @par Description: check dongle is connected
 * @param  [in]  No Param
 * @retval OK~                    Success
 * @retval ERR~                   Fail
 * @else
 * @brief  AT^ISDONGLECONN
 * @par 说明: 测试dongle是否连通，dongle侧的测试命令
 * @param  [in]  无参数
 * @retval OK~                    成功返回
 * @retval ERR~                   失败返回
 * @endif
 */
AT^ISDONGLECONN;

/**
 * @if Eng
 * @brief  AT^TIMESET
 * @par Description: set system time
 * @param  [in]  [input format time string：year-month-day hour:minute:second]
 * @retval OK~                    Success
 * @retval ERR~                   Fail
 * @else
 * @brief  AT^TIMESET
 * @par 说明: 设置系统时间命令
 * @param  [in]  [指定格式的时间输入：年-月-日 时:分:秒 例：2023-10-16 21:11:11]
 * @param  [in]  [输入参数必须严格按照上面输入格式]
 * @retval OK~                    成功返回
 * @retval ERR~                   失败返回
 * @endif
 */
AT^TIMESET;

/**
 * @if Eng
 * @brief  AT^TIMEGET
 * @par Description: get system time
 * @param  [in] No Param
 * @retval format time string,like 2023-10-16 21:11:11:100
 * @retval OK~                    Success
 * @retval ERR~                   Fail
 * @else
 * @brief  AT^TIMEGET
 * @par 说明: 查询系统时间命令
 * @param  [in] 无参数
 * @retval 查询到的时间，格式如：2023-10-16 21:11:11:200
 * @retval OK~                    成功返回
 * @retval ERR~                   失败返回
 * @endif
 */
AT^TIMEGET;

#endif /* DIAG_INTRFACE_H */