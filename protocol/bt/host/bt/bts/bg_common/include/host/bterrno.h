/*
 * Copyright (c) @CompanyNameMagicTag 1999-2015. All rights reserved.
 */

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
Module Name:
    bterrno.h
Abstract:
    This file includes the definition of global error numbers.
--------------------------------------------------------------------------- */
#ifndef BTERRNO_H
#define BTERRNO_H

enum {
    /* 0x00 is reserved */
    /* 0x01 is used by HCI internal error, e.g. HCI_STATUS_NO_FLOWCONTROL = 0x0100 */
    ER_TYPE_HCI = 0x02, /* standard HCI error */
    ER_TYPE_HCI_PRV = 0x03, /* ((ER_TYPE_HCI << 8) + HCI internal error) >> 8 */
    ER_TYPE_L2CAP = 0x04,
    ER_TYPE_RFCOMM = 0x05,
    ER_TYPE_OBEX =         0x06,
    ER_TYPE_SMP =         0x07
};

/* only when value != 0, then set type */
#define er_set_c(t, v)                            ((uint16_t)((t) << 8) | (v)) // to deal with pclint 506
#define er_set(t, v)                            ((uint16_t)(((v) != 0) ? ((uint16_t)((t) << 8) | (uint16_t)(v)) : 0))
#define er_get_type(s)                            ((uint8_t)((s) >> 8))
#define er_get_value(s)                            ((uint8_t)((s) & 0xFF))
#define er_setcbk(v, t)                            (((v) & 0xFFFFFF00) ? (((v) & 0xFFFFFF00) >> 8) : er_set((t), (v)))
#define er_l2cap_setcbk(v)                        (((v) & 0xFFFFFF00) ? er_set((ER_TYPE_HCI), \
    (((v) & 0xFFFFFF00) >> 8)) : er_set((ER_TYPE_L2CAP), (v)))

#endif
