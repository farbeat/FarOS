/*
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved.
 * Description: PM FSM header
 */

#ifndef SRC_LIB_PM_FSM_H
#define SRC_LIB_PM_FSM_H

#include "errcode.h"
#include "pm.h"

/**
 * @brief  The callback handler that on the id and state.
 * @param  id The ID register handler.
 * @param  state The state about chip work.
 * @return errcode_t ERRER number. For details, see errcode_t.
 */
errcode_t pm_process_fsm_handler(pm_id_t id, pm_states_t state);

#endif