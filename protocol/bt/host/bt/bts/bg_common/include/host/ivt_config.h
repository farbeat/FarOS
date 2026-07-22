/*
 * Copyright (c) @CompanyNameMagicTag 2015-2020. All rights reserved.
 * Description: ivt config
 */

#ifndef _IVT_CONFIG_H_
#define _IVT_CONFIG_H_

#if defined(CONFIG_CSR_8811)
    #define CONFIG_TL_BCSP
#elif defined(CONFIG_BCM_4343S)
    #define CONFIG_TL_H4
#endif

// show midification on vendor's code
#define HUAWEI_ADAPT_VENDOR

#endif /* _IVT_CONFIG_H_ */
