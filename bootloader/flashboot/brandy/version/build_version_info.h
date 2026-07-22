/*
 * Copyright (c) @CompanyNameMagicTag 2021-2021. All rights reserved.
 * Description:   SSB APPLICATION BUILD VERSION INFORMATION
 * Author: @CompanyNameTag
 * Date:
 */

#ifndef SRC_APPLICATION_BT_BUILD_VERSION_H
#define SRC_APPLICATION_BT_BUILD_VERSION_H

#include "application_version.h"
#include "codeloader_version.h"

#define BUILD_VERSION_SECTION ".rom_version"

#define APPLICATION_VERSION_NUMBER (CODELOADER_VERSION & APPLICATION_CORE_VERSION_MASK)
#define CORE_VERSION               (APPLICATION_CORE_ROM_BUILD | APPLICATION_VERSION_NUMBER)

#endif