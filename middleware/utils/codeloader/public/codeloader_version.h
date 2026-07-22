/*
 * Copyright (c) CompanyNameMagicTag 2018-2020. All rights reserved.
 * Description:  Codeloader Module Version Number
 * Author:
 * Date:
 */

#ifndef CODELOADER_VERSION_H
#define CODELOADER_VERSION_H

/**
 * @brief  Codeloader version.
 * Every external change to the codeloader interface requires this number to be incremented
 */
#define CODELOADER_MINOR_VERSION 1
#define CODELOADER_MAJOR_VERSION 0
#define CODELOADER_VERSION       (CODELOADER_MAJOR_VERSION << 16 | CODELOADER_MINOR_VERSION)

#endif