/*
 * Copyright (c) @CompanyNameMagicTag 1999-2015. All rights reserved.
 */

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
Module Name:
    buff.h
Abstract:
    This file includes the definition of handle management functions.
--------------------------------------------------------------------------- */
#ifndef HDLMGNT_H
#define HDLMGNT_H

#include "hdlmgnt.h"

struct bth_handle_struct {
    HANDLE handle_id; /* Handle ID of the handle */
    uint8_t *field_id; /* field attached to the handle */
    uint8_t *param; /* Parameter attached to the handle */
    uint16_t class_id; /* Class of the handle */
    uint8_t field_len; /* Length of field_id */
    signed char active; /* status of the handle */
};

#define HDL_TIMEOUT 2

#ifdef CONFIG_SYNC_API_SUPPORTED
void init_handle(void);
void done_handle(void);
void cleanup_handle(void);

struct bth_handle_struct *create_handle(uint16_t class_id, uint8_t field_len, uint8_t* field_id, uint8_t* param);
struct bth_handle_struct *find_handle(uint16_t class_id, uint8_t field_len, unsigned char *field_id);
uint8_t find_hdl(const struct bth_handle_struct *hdl);
char activate_handle(uint16_t class_id, uint8_t field_len, uint8_t *field_id, uint8_t par_len, uint8_t *param);
char fast_act_handle(struct bth_handle_struct *hdl, uint8_t f, uint8_t par_len, uint8_t *param);
char act_handle3(struct bth_handle_struct *hdl, uint8_t f);
uint8_t bth_wait_handle(struct bth_handle_struct *hdl);
uint8_t  delete_handle(struct bth_handle_struct *hdl);
uint8_t wait_handle2(struct bth_handle_struct *hdl, DWORD msecond);
uint8_t wait_handle_ex(struct bth_handle_struct *hdl);
void to_expire(DWORD arg);

#else
#define init_handle()
#define done_handle()
#define cleanup_handle()
#define create_handle(_clsid, _flen, _fid, _par) NULL
#define find_handle(_clsid, _flen, _fid, _par) NULL
#define find_hdl(_hdl) BT_FAIL
#define activate_handle(_clsid, _flen, _fid, _plen, _par) (-1)
#define bth_wait_handle(_hdl) BT_FAIL
#define fast_act_handle(_hdl, _f, _plen, _par) (-1)
#define act_handle3(_hdl) (-1)
#define wait_handle2(_hdl, _ms) BT_FAIL
#define wait_handle_ex(_hdl) BT_FAIL
#endif

#define act_handle2(h)  act_handle3((h), 0)

#endif
