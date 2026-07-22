/*
 * Copyright (c) @CompanyNameMagicTag 2021. All rights reserved.
 * Description: This module include test functions of HFP HF service.
 */
#ifndef SAMPLE_HFP_HF_SERVICE_H
#define SAMPLE_HFP_HF_SERVICE_H

#include <stdint.h>

#define SAMPLE_HFP_PROF_VOL_LEVEL 15
#define SAMPLE_HFP_MEDIA_VOL_LEVEL 100

void bt_hfp_hf_service_sample_cmd(uint32_t cmd_id, const char **params, int32_t params_cnt);
void wstp_hfp_hf_reg_callback(void);
void sample_hfp_hf_set_spk_volume(uint8_t volume);

#endif /* SAMPLE_HFP_HF_SERVICE_H */
