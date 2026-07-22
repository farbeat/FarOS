/*
 * Copyright (c) CompanyNameMagicTag 2021-2021. All rights reserved.
 * Description: osal proc
 * Author: audio
 * Create: 2021-06-18
 */

#include <stdarg.h>
#include "securec.h"
#include "chip_type.h"
#include "audio_type.h"
#include "soc_osal.h"
#include "drv_audio_osal.h"
#include "audio_struct.h"

#ifdef SAP_PROC_OUTPUT_DIAG_SHELL
#include "diag.h"
#include "soc_diag_cmd_id.h"
#endif

td_s32 HA_UsartTransmit(td_u16 bus, const td_void *buf, td_u16 length);

#define DRV_AUDIO_PROC_MAX_FORMAT_LEN 121 /* text(120) + \n(1) */

#ifdef SAP_PROC_OUTPUT_UART
static td_bool g_output_uart_flag = TD_TRUE;
#else
static td_bool g_output_uart_flag = TD_FALSE;
#endif

#ifdef SAP_PROC_OUTPUT_DIAG_SHELL
static td_bool g_output_diag_shell_flag = TD_TRUE;
#else
static td_bool g_output_diag_shell_flag = TD_FALSE;
#endif

#ifdef SAP_PROC_SUPPORT

#ifdef SAP_PROC_TYPE_DUMMY
/* dummy proc instance for all module, do nothing */
static drv_audio_osal_proc_entry g_osal_proc_dummy;
#endif

drv_audio_osal_proc_entry *drv_audio_osal_proc_add(const td_char *name, td_ulong name_size)
{
#ifdef SAP_PROC_TYPE_DUMMY
    audio_unused(name);
    audio_unused(name_size);
    return &g_osal_proc_dummy;
#else
    return (drv_audio_osal_proc_entry *)osal_proc_add(name, name_size);
#endif
}

td_void drv_audio_osal_proc_remove(const td_char *name, td_ulong name_size)
{
#ifdef SAP_PROC_TYPE_DUMMY
    audio_unused(name);
    audio_unused(name_size);
    return;
#else
    return osal_proc_remove(name, name_size);
#endif
}

td_s32 drv_audio_osal_proc_print(td_void *seqfile, const td_char *fmt, ...)
{
    td_s32 len;
    va_list args;
    td_char str[DRV_AUDIO_PROC_MAX_FORMAT_LEN + 1] = {0};

#ifdef SAP_PROC_TYPE_DUMMY
    (td_void)seqfile;
#endif

    va_start(args, fmt);
    len = vsnprintf_s(str, DRV_AUDIO_PROC_MAX_FORMAT_LEN + 1, DRV_AUDIO_PROC_MAX_FORMAT_LEN, fmt, args);
    va_end(args);

    if (len < 0) {
        osal_printk("%s: vsnprintf_s len = %d error.", __FUNCTION__, len);
        return -1;
    }

#ifdef SAP_PROC_TYPE_DUMMY
#ifdef SAP_PROC_OUTPUT_UART
    if (g_output_uart_flag == TD_TRUE) {
#if (SAP_CHIP_TYPE == socmn1)
        HA_UsartTransmit(1, str, len);
#else
        osal_printk("%s", str);
#endif /* SAP_CHIP_TYPE */
    }
#endif /* SAP_PROC_OUTPUT_UART */
#ifdef SAP_PROC_OUTPUT_DIAG_SHELL
    errcode_t ret;
    if (g_output_diag_shell_flag == TD_TRUE) {
        ret = uapi_diag_report_packet(DIAG_CMD_MOCKED_SHELL_IND, TD_NULL, (const uint8_t *)str, (uint16_t)len, TD_TRUE);
        if (ret != ERRCODE_SUCC) {
            osal_printk("%s: uapi_diag_report_packet failed with 0x%x.", __FUNCTION__, ret);
            return ret;
        }
    }
#endif
#else
    osal_proc_print(seqfile, "%s", str);
#endif

    return 0;
}

#endif /* SAP_PROC_SUPPORT */

td_s32 drv_audio_osal_proc_set_mask(td_u8 proc_mask)
{
#ifdef SAP_PROC_OUTPUT_UART
    g_output_uart_flag = ((proc_mask & AUDIO_PROC_MASK_UART) != 0) ? TD_TRUE : TD_FALSE;
#else
    if (proc_mask & AUDIO_PROC_MASK_UART) {
        return -1;
    }
#endif

#ifdef SAP_PROC_OUTPUT_DIAG_SHELL
    g_output_diag_shell_flag = ((proc_mask & AUDIO_PROC_MASK_DIAG_SHELL) != 0) ? TD_TRUE : TD_FALSE;
#else
    if (proc_mask & AUDIO_PROC_MASK_DIAG_SHELL) {
        return -1;
    }
#endif

    return 0;
}

td_void drv_audio_osal_proc_get_mask(td_u8 *proc_mask)
{
    td_u8 temp = 0x0;

    temp |= (g_output_uart_flag == TD_TRUE) ? AUDIO_PROC_MASK_UART : 0x0;
    temp |= (g_output_diag_shell_flag == TD_TRUE) ? AUDIO_PROC_MASK_DIAG_SHELL : 0x0;

    *proc_mask = temp;
}
