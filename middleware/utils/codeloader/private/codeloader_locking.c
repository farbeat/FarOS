/*
 * Copyright (c) CompanyNameMagicTag 2018-2020. All rights reserved.
 * Description: BT Codeloader Module
 * Author:
 * Create: 2018-10-15
 */

#include "codeloader.h"
#include "codeloader_locking.h"

static uint32_t g_codeloader_lock;

/**
 * Codeloader internal state vector type
 */
typedef struct {
    bool security_core_allowed : 1;
    bool protocol_core_allowed : 1;
    bool apps_core_allowed : 1;
} codeloader_permissions_vec_t;  //lint !e959

static volatile codeloader_permissions_vec_t g_codeloader_perm_state;

/*
 * read codeloade lock value from security KV store
 * if no ky is present, nothing is locked
 */
void codeloader_lock_init(const codeloader_options_t *options)
{
    g_codeloader_lock = CODELOADER_LOCK_NO_LOCKS;
    g_codeloader_perm_state.security_core_allowed = options->security_core_allowed;
    g_codeloader_perm_state.protocol_core_allowed = options->protocol_core_allowed;
    g_codeloader_perm_state.apps_core_allowed = options->apps_core_allowed;
}

/*
 * check if requested lock is set
 */
bool codeloader_unlocked_check(uint32_t mask)
{
    if (g_codeloader_lock & mask) {
        return false;  // if bit is set, lock is set
    } else {
        return true;  // if bit is clear, no lock is set
    }
}

/*
 * return codeloader production lock value
 */
uint32_t codeloader_lock_read(void)
{
    return (g_codeloader_lock);
}

bool codeloader_check_permissions(cores_t core)
{
    bool allowed = false;
    switch (core) {
        case CORES_BT_CORE:
            allowed = g_codeloader_perm_state.security_core_allowed;
            break;
        case CORES_EXTERN0_CORE:
        case CORES_EXTERN1_CORE:
        case CORES_PROTOCOL_CORE:
            allowed = g_codeloader_perm_state.protocol_core_allowed;
            break;

        case CORES_APPS_CORE:
            allowed = g_codeloader_perm_state.apps_core_allowed;
            break;

        default:
            break;
    }

    return allowed;
}

void codeloader_update_permissions(cores_t core, bool allowed)
{
    switch (core) {
        case CORES_BT_CORE:
            g_codeloader_perm_state.security_core_allowed = allowed;
            break;
        case CORES_EXTERN0_CORE:
        case CORES_EXTERN1_CORE:
        case CORES_PROTOCOL_CORE:
            g_codeloader_perm_state.protocol_core_allowed = allowed;
            break;

        case CORES_APPS_CORE:
            g_codeloader_perm_state.apps_core_allowed = allowed;
            break;

        default:
            break;
    }
}
