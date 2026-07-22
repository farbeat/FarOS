/*
 * Copyright (c) @CompanyNameMagicTag 2021. All rights reserved.
 * Description: This module include test function of GAP module.
 */
#ifndef SAMPLE_BTS_GAP_H
#define SAMPLE_BTS_GAP_H

#include <stdint.h>
#include <stdbool.h>
#include "bts_def.h"

void btsrv_gap_sample_cmd(uint32_t cmd_id, const char **params, int32_t params_cnt);
void btsrv_at_cmd_gap_register_callbacks(void);
void reset_found_dev_list(void);
void update_found_dev_list(void);
void show_found_dev_list(void);
void wstp_get_paired_devices_num(void);
void wstp_get_paired_devices_list(void);
void wstp_get_pair_state(const bd_addr_t *bd_addr);
bool wstp_connect_remote_device(const bd_addr_t *bd_addr);

#endif /* SAMPLE_BTS_GAP_H */
