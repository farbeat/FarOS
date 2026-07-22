/*
 * Copyright (c) CompanyNameMagicTag 2018-2020. All rights reserved.
 * Description:  CODELOADER CONFIGURATION MODULE HEADER
 * Author:
 * Create:
 */

#ifndef SRC_APPLICATION_CODELOADER_CONFIGURATION_H
#define SRC_APPLICATION_CODELOADER_CONFIGURATION_H

#include <stdbool.h>
#include "platform_types.h"

/**
 * @brief  Check the codeloader permissions for security protocol and apps. If either the permanent
 * or temporal lock are set the return will be false, true will be returned otherwise.
 * @param  sec_allowed  (returned value) codeloader permissions allowed for CORES_BT_CORE
 * @param  prot_allowed (returned value) codeloader permissions allowed for CORES_PROTOCOL_CORE
 * @param  apps_allowed (returned value) codeloader permissions allowed for CORES_APPS_CORE
 */
void codeloader_configuration_get_permissions(bool *sec_allowed, bool *prot_allowed, bool *apps_allowed);

/**
 * @brief  Lock permanently the permissions for a given core.
 * @param  core Core to lock the codeloader for
 * @note   it uses the OTP to lock it. Once locked the codeloader cannot run again with permissions for that core.
 */
void codeloader_configuration_permanent_lock_permissions(cores_t core);

/**
 * @brief  Lock temporarily the permissions for a given core.
 * @param  core Core to lock the codeloader for
 */
void codeloader_configuration_temporary_lock_permissions(cores_t core);

/**
 * @brief  Unlock the temporal lock for a given core
 * @param  core Core to release the temporal lock for
 */
void codeloader_configuration_temporary_unlock_permissions(cores_t core);

#endif
