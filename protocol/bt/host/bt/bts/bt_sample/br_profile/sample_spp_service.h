/*
 * Copyright (c) @CompanyNameMagicTag 2022. All rights reserved.
 * Description: This module include test functions of BT SPP Service module.
 */
#ifndef SAMPLE_BT_SPP_SERVICE_H
#define SAMPLE_BT_SPP_SERVICE_H

#include <stdint.h>

int sample_spp_register_callbacks(void);
void bt_spp_service_sample_cmd(uint32_t cmd_id, const char **params, int32_t params_cnt);
void wstp_spp_cmd_server_create(const char *spp_uuid, const char *name, uint8_t name_len);
void wstp_spp_cmd_server_accept(uint32_t server_id);
void wstp_spp_cmd_server_close(int32_t server_id);
void wstp_spp_cmd_connect(const char *spp_uuid, const char *addr);
void wstp_spp_cmd_disconnect(uint32_t client_id);
void wstp_spp_cmd_is_spp_connect(int32_t client_id);
void wstp_spp_cmd_get_remote_addr(int32_t client_id);
void wstp_spp_cmd_read(int32_t client_id, int32_t buf_len, int32_t duration);
void wstp_spp_cmd_write(int32_t client_id, int32_t repeat_times, int32_t cycle_num, int32_t delay_time);

#endif /* SAMPLE_BT_SPP_SERVICE_H */
