/*
 * Copyright (c) @CompanyNameMagicTag 2018-2020. All rights reserved.
 * Description:  common logging producer interface - need to change name of log.h in all the protocol core files
 * Author: @CompanyNameTag
 * Create:
 */

#ifndef LOG_DEF_H
#define LOG_DEF_H

#ifdef BTC_SYS_PART
#include "log_def_btc.h"
#endif
#ifdef BTH_SYS_PART
#include "log_def_bth.h"
#else
#include "log_def_bt.h"
#endif
#include "log_def_bth.h"
#include "log_def_gpu.h"
#include "log_def_gui.h"
#include "log_def_pf.h"
#include "log_def_dsp.h"
#include "log_def_app.h"
#include "log_def_media.h"

#endif
