/*
 * Copyright (c) @CompanyNameMagicTag 1999-2015. All rights reserved.
 */

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
Module Name:
    buff.h
Abstract:
    This file includes the definition of buffer management functions.
--------------------------------------------------------------------------- */
#ifndef BUFF_H
#define BUFF_H

#include "sysdep.h"

struct buff_stru {
    uint16_t len; /* length of whole buffer, including data before off */
    uint16_t off;
    uint8_t data[1];
};

#ifdef CONFIG_HW_MEMORY_DEBUG
struct buff_stru *_buff_new_ext(uint16_t size, uint16_t off, uint32_t pos);
struct buff_stru *_buff_new_ext_s(uint16_t size, uint16_t off, uint32_t pos);
#else
struct buff_stru *_buff_new(uint16_t size, uint16_t off);
struct buff_stru *_buff_new_s(uint16_t size, uint16_t off);
#endif

#ifdef CONFIG_MEMORY_LEAK_DETECT
#define bth_buff_new(size_, off_)    dbg_memory_leak_new(_buff_new((size_), (off_)), 0, \
                                    (size_) + (off_) + sizeof(struct buff_stru) + sizeof(uint32_t), MEMORY_LEAK_POSINFO)
#define bth_buff_new_s(size_, off_)    _buff_new_s((size_), (off_))
#elif defined(CONFIG_HW_MEMORY_DEBUG) /* CONFIG_MEMORY_LEAK_DETECT */
#define bth_buff_new(size_, off_)    _buff_new_ext((size_), (off_), MEMORY_LEAK_POSINFO)
#define bth_buff_new_s(size_, off_)    _buff_new_ext_s((size_), (off_), MEMORY_LEAK_POSINFO)
#else
#define bth_buff_new(size_, off_)    _buff_new((size_), (off_))
#define bth_buff_new_s(size_, off_)    _buff_new_s((size_), (off_))
#endif

#define pri_buf_new(size_, off_, buf_, pri_) buf_ = bth_buff_new((size_), (off_)); hci_bufpri(buf_) = (pri_)
#define pri_buf_new_s(size_, off_, buf_, pri_) do { \
    (buf_) = bth_buff_new_s((size_), (off_)); \
    if ((buf_) != NULL) { \
        hci_bufpri(buf_) = (pri_); \
    } \
} while (0)

struct buff_stru *buff_copy(struct buff_stru *in);
uint8_t buff_res(struct buff_stru *buf, int16_t delta);
#define BUFF_FREE MEM_FREE

enum { /* HCI_BUFPRI */
    PRI_RELIABLE =     1, /* Lowest priority, but reliable, donot flush, etc, FTP */
    PRI_REALTIME =     2, /* etc, A2DP, could be flush */
    PRI_SIGNAL =     3, /* Signal, harry to send out */
    PRI_PARTOUT =     0xFF /* Partial send out, do not interlace */
};

/* a pointer, shall be used as *hdl_ptr() */
#define hdl_ptr(buf)                ((uint32_t *)(&(((struct buff_stru *)(buf))->data[0])))
/* for HCI_RESERVE, Bufdata Priority(1), HCI AMPID(1), L2CAP Payload or S_Frame Flag(1) */
#define hcictrl(buf)            (&(((struct buff_stru *)(buf))->data[4]))
/* Bufdata Priority, 1 octet, etc, PRI_RELIABLE */
#define hci_bufpri(buf)            (*hcictrl(buf))
/* HCI AMPID, 1 octet */
#define hci_bufampid(buf)        (*(hcictrl(buf) + 1))
/* L2CAP Payload or S_Frame Flag, 1 octet */
#define hci_bufl2payload(buf)    (*(hcictrl(buf) + 2))
#define HCI_HEADER                5 /* data type (1), Handle (2), HCI Payload len (2) */
/* 3 for HCICTRL as above 5 is for hci_acl_data_send2tl, HCI headers */
#define HCI_RESERVE                (3 + HCI_HEADER)
#define L2CAP_RESERVE            (4 + HCI_RESERVE) /* L2CAP CID(2), Length(2), as Basic Header */

#define buflen(buf)              (((struct buff_stru *)(buf))->len)
#define buf_data(buf)            ((uint8_t *)(((struct buff_stru *)(buf))->data + ((struct buff_stru *)(buf))->off))
/* BUFDATA + DATASIZE, 1st invalid octet */
#define buf_end(buf)                ((uint8_t *)(((struct buff_stru *)(buf))->data + ((struct buff_stru *)(buf))->len))
#define datasize(buf)             (uint16_t)(buflen((struct buff_stru *)(buf)) - ((struct buff_stru *)(buf))->off)

#define GLE_HCI_HEADER             4 /* Handle(12 bits) PB_Flag(2 bits) Ts(1 bit) Prio(1 bit) Len (16 bits) */

#endif

