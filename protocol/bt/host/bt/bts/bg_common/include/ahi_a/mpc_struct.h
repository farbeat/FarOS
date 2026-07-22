/*
 * Copyright (c) @CompanyNameMagicTag 2021-2022. All rights reserved.
 * Description: mpc command param struct
 * Author: @CompanyNameTag
 * Create: 2021-09-01
 */
#ifndef __MPC_STRUCT_H_
#define __MPC_STRUCT_H_

#include "custdefs.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define BD_NAME_MAX_LEN       32
#define BD_ADDR_LEN           6

#pragma pack (1)
typedef struct {
    uint8_t bd_addr[BD_ADDR_LEN];
    uint8_t phone_bd_addr[BD_ADDR_LEN];
    uint8_t bd_name[BD_NAME_MAX_LEN];
} mc_dev_info_param;

#pragma pack ()

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */
#endif /* __MPC_TRANS_H_ */
