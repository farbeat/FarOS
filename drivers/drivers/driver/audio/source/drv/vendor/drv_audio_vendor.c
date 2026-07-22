/*
 * Copyright (c) CompanyNameMagicTag 2020-2021. All rights reserved.
 * Description: source file for audio vendor
 */

#include "drv_sap_ioctl.h"
#include "audio_osal.h"
#include "drv_audio_vendor.h"

static OSAL_LIST_HEAD(g_drv_vendor_list);

td_s32 drv_audio_vendor_register(drv_vendor_driver *driver)
{
    vendor_check_null_ptr_return(driver->init);

    if (driver->out_port >= AO_PORT_MAX) {
        return ERR_VENDOR_INVALID_PARAM;
    }

    osal_list_add_tail(&driver->node, &g_drv_vendor_list);

    return AUDIO_SUCCESS;
}

td_s32 drv_audio_vendor_init(ao_port_id out_port, audio_sample_rate sample_rate, audio_bit_depth bit_depth,
    audio_channel channel)
{
    drv_vendor_driver *driver = TD_NULL;
    vendor_codec_attr codec_attr;
    audio_unused(channel);

    osal_list_for_each_entry(driver, &g_drv_vendor_list, node) {
        if (out_port == driver->out_port && driver->init != TD_NULL) {
            codec_attr.sample_rate = sample_rate;
            codec_attr.bit_depth = bit_depth;
            codec_attr.master_mode = TD_TRUE;

            return driver->init(&codec_attr);
        }
    }

    return AUDIO_SUCCESS;
}

td_s32 drv_audio_vendor_deinit(ao_port_id out_port)
{
    drv_vendor_driver *driver = TD_NULL;

    osal_list_for_each_entry(driver, &g_drv_vendor_list, node) {
        if (out_port == driver->out_port && driver->deinit != TD_NULL) {
            return driver->deinit();
        }
    }

    return AUDIO_SUCCESS;
}

td_s32 drv_audio_vendor_set_aef_profile(ao_port_id out_port, ao_aef_profile aef_profile)
{
    drv_vendor_driver *driver = TD_NULL;

    osal_list_for_each_entry(driver, &g_drv_vendor_list, node) {
        if (out_port == driver->out_port && driver->set_aef_profile != TD_NULL) {
            return driver->set_aef_profile(aef_profile);
        }
    }

    return AUDIO_SUCCESS;
}
