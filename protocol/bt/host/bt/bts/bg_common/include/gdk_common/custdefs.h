/*
 * Copyright (c) @CompanyNameMagicTag 2017. All rights reserved.
 * Description: customer type define
 */
#ifndef BT_CUSTDEFS_H
#define BT_CUSTDEFS_H

#include "stdint.h"
#ifndef BOOLEAN
typedef unsigned char boolean;
#endif

#ifndef NULL
#define NULL 0
#endif

#ifndef TRUE
#define TRUE 1
#endif /* TRUE */

#ifndef FALSE
#define FALSE 0
#endif /* FALSE */

#ifndef NULL
#define NULL 0
#endif /* NULL */

#ifndef NONE
#define NONE
#endif /* NONE */

#ifndef BD_ADDR_LEN
#define BD_ADDR_LEN 6
#endif

#ifndef BD_LINKKEY_LEN
#define BD_LINKKEY_LEN 16
#endif

#define param_null_ret(param, ret) \
    do {if (NULL == (param)) return ret;} while (0)

typedef enum {
    BT_RESULT_PARAM_ERR          =  -2,      /**< configure fail */
    BT_RESULT_FAILURE            =  -1,      /**< parameter error */
    BT_RESULT_SUCCESS            =   0,      /**< success */
    BT_RESULT_UNKNOWN,                       /**< no attation */
} bt_return_em;

typedef uint8_t raw_device_addr[6];
typedef raw_device_addr bdaddr_t;

#endif /* BT_CUSTDEFS_H */

