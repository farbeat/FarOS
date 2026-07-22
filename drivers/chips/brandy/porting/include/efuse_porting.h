/**
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved. \n
 *
 * Description: Provides efuse port template \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-07-25， Create file. \n
 */

#ifndef EFUSE_PORTING_H
#define EFUSE_PORTING_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/**
 * @defgroup drivers_port_efuse Efuse
 * @ingroup  drivers_port
 * @{
 */

#define EFUSE_REGION_NUM               3
#define EFUSE_REGION_MAX_BITS          1024
#define EFUSE_REGION_MAX_BYTES         (EFUSE_REGION_MAX_BITS >> 3)  // MAX_BIT / 8
#define EFUSE_MAX_BITS                 (EFUSE_REGION_MAX_BITS * EFUSE_REGION_NUM)
#define EFUSE_MAX_BYTES                (EFUSE_MAX_BITS >> 3)  // MAX_BIT / 8
#define EFUSE_MAX_BIT_POS              8U
#define bit(x) (1UL << (uint32_t)(x))
#define EFUSE_PMU_CLKLDO_VSET_MASK       0x78
#define EFUSE_PMU_CLKLDO_OFFSET          3
#define EFUSE_TRIM_VERSION_DEFAULT_VALUE 1
#define EFUSE_CHAR_BIT_WIDE              8
#define EFUSE_CALC_CRC_MAX_LEN          32
#define HAL_EFUSE0_REGION_SIZE EFUSE_REGION_MAX_BYTES
#define HAL_EFUSE1_REGION_SIZE EFUSE_REGION_MAX_BYTES
#define HAL_EFUSE2_REGION_SIZE EFUSE_REGION_MAX_BYTES
#define EFUSE_DIE_ID_BASE_BYTE_ADDR     0
#define EFUSE_BIT_OPERATION             YES

#define EFUSE_TRIM_FLAG    50
#define EFUSE_IS_CHIP_TRIMED 0
// 7-0 -> Version of the trim
#define EFUSE_TRIM_VERSION 51
// 2-0 -> vset_bbldo[3:1]
// 6-3 -> vset_clkldo[3:0]
#define EFUSE_PMU_TRIM_VSET 25

typedef enum {
    HAL_EFUSE_REGION_0,
    HAL_EFUSE_REGION_1,
    HAL_EFUSE_REGION_2,
    HAL_EFUSE_REGION_MAX,
} hal_efuse_region_t;

/**
 * @if Eng
 * @brief  Base address for the IP.
 * @else
 * @brief  IP的基地址
 * @endif
 */
extern const uintptr_t g_efuse_base_addr[EFUSE_REGION_NUM];

/**
 * @if Eng
 * @brief  Base read address for the IP.
 * @else
 * @brief  IP的读基地址
 * @endif
 */
extern const uintptr_t g_efuse_region_read_address[EFUSE_REGION_NUM];

/**
 * @if Eng
 * @brief  Base write address for the IP.
 * @else
 * @brief  IP的写基地址
 * @endif
 */
extern const uintptr_t g_efuse_region_write_address[EFUSE_REGION_NUM];

/**
 * @if Eng
 * @brief  EFUSE region size
 * @else
 * @brief  EFUSE各区域大小
 * @endif
 */
extern const uintptr_t g_efuse_region_size[EFUSE_REGION_NUM];

/**
 * @if Eng
 * @brief  Register hal funcs objects into hal_efuse module.
 * @else
 * @brief  将hal funcs对象注册到hal_efuse模块中
 * @endif
 */
void efuse_port_register_hal_funcs(void);

/**
 * @if Eng
 * @brief  Unregister hal funcs objects from hal_efuse module.
 * @else
 * @brief  从hal_efuse模块注销hal funcs对象
 * @endif
 */
void efuse_port_unregister_hal_funcs(void);

/**
 * @brief  Get the region of a otp byte address
 * @param  byte_addr the addr of the byte to get register
 * @retval region The region of otp
 * @else
 * @brief  获取otp字节地址的区域
 * @param  byte_addr 要获取寄存器的字节的地址
 * @retval 区域OTP的区域
 */
hal_efuse_region_t hal_efuse_get_region(uint32_t byte_addr);

/**
 * @brief  Get the offset addr of a otp byte address
 * @param  byte_addr the addr of the byte to get register
 * @retval address
 * @else
 * @brief  获取otp字节地址的偏移地址
 * @param  byte_addr 要获取寄存器的字节的地址
 * @retval 偏移地址
 */
uint16_t hal_efuse_get_byte_offset(uint32_t byte_addr);


/**
 * @brief  load switch4 enbale signal.
 * @param  val Value to be written to the register
 * @retval address
 * @else
 * @brief  负载开关4使能
 * @param  val 要写入寄存器的值
 * @retval 偏移地址
 */
void efuse_port_set_en_switch(uint32_t val);

/**
 * @if Eng
 * @brief  Obtains the enable signal of load switch 4.
 * @else
 * @brief  获取负载开关4使能信号。
 * @endif
 */
uint8_t efuse_port_get_en_switch(void);

/**
 * @if Eng
 * @brief  Reload the efuse data before reads multiple bytes from the eFuse into the provided buffer.
 * @param  [in] buffer The value of the bit read.
 * @param  [in] byte_number The source eFuse bit byte_number of the bit to be read.
 * @param  [in] length The length of the data, in bytes.
 * @retval ERRCODE_SUCC Success.
 * @retval Other        Failure. For details, see @ref errcode_t.
 * @else
 * @brief  重新从efuse源头加载数据后，再从eFuse中读取多个字节，进入提供的缓冲区。
 * @param  [in] buffer 保存读取数据的缓冲区。
 * @param  [in] byte_number 要读取的数据的初始源eFuse字节地址。
 * @param  [in] length 数据的长度，以字节为单位。
 * @retval ERRCODE_SUCC 成功。
 * @retval Other        失败，参考 @ref errcode_t 。
 * @endif
 */
errcode_t efuse_port_read_buffer_by_reload(uint8_t *buffer, uint32_t byte_number, uint16_t length);

/**
 * @if Eng
 * @brief  Open or close efuse region clock gate.
 * @param  [in] efuse_region the efuse region index.
 * @param  [in] en open or close.
 * @else
 * @brief  打开efuse的时钟门控。
 * @param  [in] efuse_region efuse 区域。
 * @param  [in] en 打开或关闭时钟。
 * @endif
 */
void efuse_port_set_clk_en(hal_efuse_region_t efuse_region, bool en);

/**
 * @}
 */

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif
