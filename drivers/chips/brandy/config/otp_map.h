/*
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved.
 * Description:  OTP map
 */
#ifndef OTP_MAP_H
#define OTP_MAP_H

#include "chip_definitions.h"
#include "chip_io.h"

/**
 * @addtogroup connectivity_config_otp OTP
 * @{
 */
/*
 * OTP address map provides start addresses and lengths
 */
// EFUSE0
// ATE  0~17
#define OTP_ATE_START                  0
#define OTP_ATE_DIEID_START            0
#define OTP_CHIP_ID_START              19
#define OTP_ATE_DIEID_LENGTH           20

// PMU_TRIM  20~28
#define OTP_PMU_TRIM_START 20
// 4-0 -> simo1_vo1_trim[4:0]
// 5-7 -> simo1_vo2_trim[2:0]
#define OTP_PMU_TRIM_SIMO1 20

// 1-0 -> simo1_vo2_trim[4:3]
// 6-2 -> simo2_vo1_trim[4:0]
// 7   -> simo2_vo2_trim[0]
#define OTP_PMU_TRIM_SIMO2 21

// 3-0 -> simo2_vo2_trim[4:1]
// 6-4 -> vset_rfldo1[2:0]
// 7   -> vset_rfldo2[0]
#define OTP_PMU_TRIM_VSET_RFLDO1 22

// 1-0 -> vset_rfldo2[2:1]
// 4-2 -> vset_rfldo3[2:0]
// 7-5 -> vset_rfldo4[2:0]
#define OTP_PMU_TRIM_VSET_RFLDO2 23

// 2-0 -> vset_rfldo5[2:0]
// 6-3 -> vset_paldo[3:0]
// 7   -> vset_bbldo[0]
#define OTP_PMU_TRIM_VSET_RFLDO3 24

// 2-0 -> vset_bbldo[3:1]
// 6-3 -> vset_clkldo[3:0]
#define OTP_PMU_TRIM_VSET 25

// 7-0 -> ref_trim1[7:0]
#define OTP_PMU_TRIM_REF1 26

// 3-0 -> ref_trim1[11:8]
// 7-4 -> ref_trim2[3:0]
#define OTP_PMU_TRIM_REF2 27

// 6-0 -> ref_trim2[10:4]
//  7  -> efuse_en
#define OTP_PMU_TRIM_REF3   28
#define OTP_PMU_TRIM_LENGTH 9

// PMU ULP trim  29
#define OTP_PMU_ULP_TRIM_START  29
#define OTP_PMU_ULP_LENGTH 1

// 1-0 -> pmu_ulp_trim[9:8]
// 7-2 -> pmu_rc_32k_trim[5:0]
#define OTP_PMU_CMU_TRIM_START  30
#define OTP_PMU_CMU_LENGTH 1
// PMU_TRIM_2  34
#define OTP_CODEC_D2A_START 34
#define OTP_CODEC_D2A_LENGTH  2
#define OTP_CODEC_LDO_TRIM 36

// DEBUG FUSED  37
#define OTP_DEBUG_FUSED     37
#define OTP_DEBUG_FUSED_SWD_VERIFY_KEY_BIT  0
#define OTP_DEBUG_FUSED_SWD_EN_BIT      1
#define OTP_DEBUG_FUSED_DSP_JTAG_EN_BIT 2
#define OTP_DEBUG_FUSED_SSI_EN_BIT      3
#define OTP_DEBUG_FUSED_SECURIY_KEY_EN_ACCESS_BIT 4

// RF TRIM 42-49
#define OTP_RF_TRIM_START 42
// 7-0 -> cfg_rx_iq_comp_p_val[7:0]: rx iq compensation
#define OTP_BT_RX_IQ_COMP_P_VAL1 42

// 7-1 -> Reserved
// 0   -> cfg_rx_iq_comp_p_val[8]: rx iq compensation
#define OTP_BT_RX_IQ_COMP_P_VAL2 43

// 7-0 -> cfg_rx_iq_comp_e_val[7:0]: rx iq compensation
#define OTP_BT_RX_IQ_COMP_E_VAL 44

// 7-0 -> buffer_gain_i[7:0]:I channel PDET gain
#define OTP_BT_BUFFER_GAIN_I 45

// 7-0 -> buffer_gain_q[7:0]:Q channel PDET gain
#define OTP_BT_BUFFER_GAIN_Q 46

// 57, 58 Reserved
// 7-1 -> Reserved
// 0 -> LNA G2_FLAG
#define OTP_BT_BUFFER_LNA_TRIM 49

// 60, 61 Reserved
// 7-1 -> Reserved
// 0 -> chip is trimmed
#define OTP_TRIM_FLAG    50
#define OTP_IS_CHIP_TRIMED 0
// 7-0 -> Version of the trim
#define OTP_TRIM_VERSION 51

// LOW BAND 2ND HARMONICS  52~55
#define OTP_LOW_BAND 52
#define OTP_LOW_BAND_LENGTH 4

// MID BAND 2ND HARMONICS  56~59
#define OTP_MID_BAND 56
#define OTP_MID_BAND_LENGTH 4

#define OTP_ENCRYPTED_DATA 56
#define OTP_ENCRYPTED_DATA_LENGTH 4

// High BAND 2ND HARMONICS  60~63
#define OTP_HIGH_BAND 60
#define OTP_HIGH_BAND_LENGTH 4

#define OTP_SECURITY_KEY_GID 96
#define OTP_SECURITY_KEY_GID_LEN 32

#define OTP_CODELOADER_FLAGS_BYTE                           128
#define OTP_CODELOADER_FLAGS_LOCKED_CORES_BT_CORE_BIT       0
#define OTP_CODELOADER_FLAGS_LOCKED_CORES_PROTOCOL_CORE_BIT 1
#define OTP_CODELOADER_FLAGS_LOCKED_CORES_APPS_CORE_BIT     2
#define OTP_QSPI_BAUD_CLK_DIV_2_EN_BIT                      3

#define OTP_BOOTLOADER_BYTE                       129
#define OTP_BOOTLOADER_FSB_MINIMAL_ROM_BIT        0
#define OTP_BOOTLOADER_FSB_EN_SSB_HASH_BIT        1
#define OTP_BOOTLOADER_FSB_DIS_ATE_MODE_BIT       2
#define OTP_BOOTLOADER_FSB_DIS_PKCS1_ALG_BIT      3
#define OTP_BOOTLOADER_FSB_DIS_CODELOADER_BIT     4
#define OTP_BOOTLOADER_FSB_NON_EFLASH_BIT         5
#define OTP_BOOTLOADER_FSB_EN_UART_RX_BIT         6   // enable UARTH0 single RX
#define OTP_BOOTLOADER_FSB_DIS_UART_RX_BIT        7   // disable UARTH0 single RX

#define OTP_BOOTLOADER_BYTE_1                     130
#define OTP_BOOTLOADER_FSB_EN_WAIT_BIT            0
#define OTP_BOOTLOADER_FSB_DIS_WAIT_BIT           1
#define OTP_PIO_DRIVE_QSPI_DRIVE_EN_BIT           2
#define OTP_PIO_DRIVE_UART_DRIVE_EN_BIT           3
#define OTP_PIO_DRIVE_UART_DATA_OFFSET            4U
#define OTP_PIO_DRIVE_UART_DATA_0_BIT             4
#define OTP_PIO_DRIVE_UART_DATA_1_BIT             5
#define OTP_PIO_DRIVE_UART_DATA_2_BIT             6
#define OTP_PIO_DRIVE_UART_DATA_3_BIT             7

#define OTP_SSB_CODE_SIZE_START           131
#define OTP_SSB_CODE_SIZE_LENGTH          3
#define OTP_SSB_CODE_CRC                  134

#define OTP_MCU_ROOT_PUBKEY_SHA256_START  135
#define OTP_MCU_ROOT_PUBKEY_SHA256_LENGTH 32
#define OTP_MCU_ROOT_PUBKEY_SHA256_CRC    167

#define OTP_MCU_SUBKEY_ID_MASK_START      168
#define OTP_MCU_SUBKEY_ID_MASK_LENGTH     4
#define OTP_MCU_SUBKEY_ID_MASK_CRC        172

#define OTP_MCU_SUBKEY_CATEGORY_START     173
#define OTP_MCU_SUBKEY_CATEGORY_LENGTH    4
#define OTP_MCU_SUBKEY_CATEGORY_CRC       177

#define OTP_SEC_BOOT_DEBUG_FLAG           178
#define OTP_SEC_BOOT_DEBUG_FLAG_LENGTH    1
#define OTP_SEC_BOOT_EN_BIT               0
#define OTP_SEC_BOOT_DEBUG_BIT            1
#define OTP_SEC_BOOT_KEY_CHECK_EN_BIT     2
#define OTP_SEC_BOOT_AES_EN_BIT           3

#define OTP_BOOTLOADER_DOUBLE_BYTE        179

#define OTP_SEC_BOOT_DEBUG_DOUBLE_FLAG    180
#define OTP_SSB_VERSION_COUNTER_START     181
#define OTP_SSB_VERSION_LENGTH            4
#define OTP_SSB_VERSION_LENGTH_BIT        32

#define OTP_PIO_DRIVE_BYTE                182
#define OTP_PIO_DRIVE_QSPI_DATA_0_BIT     0
#define OTP_PIO_DRIVE_QSPI_DATA_1_BIT     1
#define OTP_PIO_DRIVE_QSPI_DATA_2_BIT     2
#define OTP_PIO_DRIVE_QSPI_DATA_3_BIT     3
#define OTP_PIO_DRIVE_QSPI_CLK_OFFSET     4U
#define OTP_PIO_DRIVE_QSPI_CLK_0_BIT      4
#define OTP_PIO_DRIVE_QSPI_CLK_1_BIT      5
#define OTP_PIO_DRIVE_QSPI_CLK_2_BIT      6
#define OTP_PIO_DRIVE_QSPI_CLK_3_BIT      7

#define OTP_BOOTLOADER_CFCT_BYTE          183
#define OTP_BOOTLOADER_CFCT_EN_BIT        0

#define OTP_SSB_SHA256_START              184
#define OTP_SSB_SHA256_LENGTH             32
#define OTP_SSB_SHA256_CRC                216

// reserved 217-224(7bytes)
#define XO_CORE_CTRIM  225
#define OTP_TRIM_AUXLDO_RFPA              226
#define OTP_TRIM_AUXLDO_BIT               4

// 227-247(20bytes):roolback version
#define OTP_RECOVERY_ROLLBACK_VERSION     227
#define OTP_RECOVERY_ROLLBACK_VERSION_LEN 4
#define OTP_RECOVERY_ROLLBACK_VERSION_BIT 32
#define OTP_APP_ROLLBACK_VERSION          231
#define OTP_APP_ROLLBACK_VERSION_LEN      4
#define OTP_APP_ROLLBACK_VERSION_BIT      32
#define OTP_BT_ROLLBACK_VERSION           235
#define OTP_BT_ROLLBACK_VERSION_LEN       4
#define OTP_BT_ROLLBACK_VERSION_BIT       32
#define OTP_DSP_MAIN_ROLLBACK_VERSION     239
#define OTP_DSP_MAIN_ROLLBACK_VERSION_LEN 4
#define OTP_DSP_MAIN_ROLLBACK_VERSION_BIT 32
#define OTP_DSP_OVERLAY_ROLLBACK_VERSION     244
#define OTP_DSP_OVERLAY_ROLLBACK_VERSION_LEN 4
#define OTP_DSP_OVERLAY_ROLLBACK_VERSION_BIT 32

#define OTP_ATE_CRC                       255

// reserved 256-383(128 bytes) for user
#define OTP_APP_USER_START              256
#define OTP_APP_USER_END                383


/**
 * @}
 */
#endif
