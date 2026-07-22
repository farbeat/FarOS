/*
 * Copyright (c) @CompanyNameMagicTag 2015-2022. All rights reserved.
 */

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
Module Name:
    btuser.h
Abstract:
    This file provides definitions for user interface.
Author:
    Gang He
Revision History:2000.2
--------------------------------------------------------------------------- */

#ifndef BTUSER_H
#define BTUSER_H

#ifndef CTL_CODE
#define ctl_code(DeviceType, Function, Method, Access) (                   \
    ((DeviceType) << 16) | ((Access) << 14) | ((Function) << 2) | (Method) \
)
#define METHOD_NEITHER 3
#define FILE_ANY_ACCESS 0
#endif

#define BTDEV                    1000
#define BTDEV_PASS_EVENT        ctl_code(BTDEV, 1, METHOD_NEITHER, FILE_ANY_ACCESS)
#define BTDEV_RECV_DATA            ctl_code(BTDEV, 2, METHOD_NEITHER, FILE_ANY_ACCESS)
#define BTDEV_SEND_DATA            ctl_code(BTDEV, 3, METHOD_NEITHER, FILE_ANY_ACCESS)
#define BTDEV_L2CA_CONN_REQ        ctl_code(BTDEV, 4, METHOD_NEITHER, FILE_ANY_ACCESS)
#define BTDEV_L2CA_CONN_RSP        ctl_code(BTDEV, 5, METHOD_NEITHER, FILE_ANY_ACCESS)

#endif
