/*
 * Copyright (c) @CompanyNameMagicTag 2021. All rights reserved.
 * Description: This module include test functions of HFP AG service.
 */
#ifndef SAMPLE_HFP_AG_SERVICE_H
#define SAMPLE_HFP_AG_SERVICE_H

#include <stdint.h>

#define SAMPLE_HFP_PROF_VOL_LEVEL 15
#define SAMPLE_HFP_MEDIA_VOL_LEVEL 100

void bt_hfp_ag_service_sample_cmd(uint32_t cmd_id, const char **params, int32_t params_cnt);

#endif /* SAMPLE_HFP_AG_SERVICE_H */
