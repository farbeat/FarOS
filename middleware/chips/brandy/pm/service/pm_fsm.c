/*
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved.
 * Description: power manage common code
 */

#include "pm_fsm.h"
#include "errcode.h"
#include "pm_sleep.h"
#include "pm_core.h"
#include "pm_dev.h"
#include "pm_os.h"

typedef struct pm_fsm_list pm_fsm_t;
struct pm_fsm_list {
    pm_fsm_t *pre;
    pm_fsm_t *next;
    pm_id_t fsm_id;
    pm_states_t state;
    pm_fsm_content_t content;
};

pm_fsm_t *g_pm_fsm_list = NULL;

/*
 * Add a callback to the handler array.
 */
errcode_t uapi_pm_register_fsm_handler(pm_id_t id, pm_states_t state, pm_fsm_content_t *content)
{
    if (id >= PM_ID_MAX || state >= PM_STATE_MAX || content == NULL) {
        return ERRCODE_PM_INVALID_PARAMETER;
    }

    if (content->handler == NULL) {
        return ERRCODE_PM_INVALID_PARAMETER;
    }

    bool existed = false;
    pm_fsm_t *fsm_list = g_pm_fsm_list;
    while (fsm_list != NULL) {
        if (fsm_list->fsm_id == id && fsm_list->state == state) {
            existed = true;
            break;
        }

        if (fsm_list->next == NULL) {
            break;
        }
        fsm_list = fsm_list->next;
    }

    if (existed) {
        fsm_list->content.len = content->len;
        fsm_list->content.handler = content->handler;
        (void)memcpy_s(fsm_list->content.data, content->len, content->data, content->len);

        return ERRCODE_SUCC;
    }

    pm_fsm_t *fsm_node = (pm_fsm_t *)pm_os_malloc(sizeof(pm_fsm_t));
    if (fsm_node == NULL) {
        return ERRCODE_PM_MEM_ALLOC_ERROR;
    }

    fsm_node->fsm_id = id;
    fsm_node->state = state;
    fsm_node->pre = NULL;
    fsm_node->next = NULL;
    (void)memcpy_s(&fsm_node->content, sizeof(pm_fsm_content_t), content, sizeof(pm_fsm_content_t));
    if (fsm_node->content.len != 0) {
        fsm_node->content.data = pm_os_malloc(content->len);
        if (fsm_node->content.data == NULL) {
            pm_os_free(fsm_node);
            return ERRCODE_PM_MEM_ALLOC_ERROR;
        }

        (void)memcpy_s(fsm_node->content.data, content->len, content->data, content->len);
    }

    if (fsm_list == NULL) {
        g_pm_fsm_list = fsm_node;
    } else {
        fsm_node->pre = fsm_list;
        fsm_list->next = fsm_node;
    }
    return ERRCODE_SUCC;
}

errcode_t pm_process_fsm_handler(pm_id_t id, pm_states_t state)
{
    if (id >= PM_ID_MAX || state >= PM_STATE_MAX) {
        return ERRCODE_PM_INVALID_PARAMETER;
    }
    pm_fsm_content_t *content = NULL;
    pm_fsm_t *fsm_list = g_pm_fsm_list;
    while (fsm_list != NULL) {
        if (fsm_list->fsm_id == id && fsm_list->state == state) {
            content = &(fsm_list->content);
            break;
        }
        fsm_list = fsm_list->next;
    }

    if (content != NULL && content->handler != NULL) {
        content->handler(content->data, content->len);
        return ERRCODE_SUCC;
    } else {
        return ERRCODE_PM_ERROR;
    }
}