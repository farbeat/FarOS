/*
 * Copyright (c) @CompanyNameMagicTag 2022. All rights reserved.
 * Description: This module include test functions of PBAP service.
 */
#ifndef SAMPLE_PBAP_SERVICE_H
#define SAMPLE_PBAP_SERVICE_H
#include <td_type.h>
#include <stdint.h>

void bt_pbap_service_sample_cmd(uint32_t cmd_id, const char **params, int32_t params_cnt);
void wstp_pbap_pce_reg_callback(void);
void wstp_pbap_cmd_pull_phonebook(
    uint8_t folder, uint8_t vcf, uint16_t max_list_count, uint16_t list_offset, uint8_t format);
void wstp_pbap_cmd_pull_vcard_listing(const char **params, int32_t params_cnt);
void wstp_pbap_cmd_pull_vcard_entry(uint8_t folder, uint8_t vcf, td_char *vcf_name, uint8_t format);

#endif /* SAMPLE_PBAP_SERVICE_H */
