/* ----------------------------------------------------------------------------
 * Copyright (c) @CompanyNameMagicTag 2020-2021. All rights reserved.
 * Description: Defines common data types.
 * Author: @CompanyNameTag
 * Create: 2020-12-29
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright notice, this list of
 * conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list
 * of conditions and the following disclaimer in the documentation and/or other materials
 * provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used
 * to endorse or promote products derived from this software without specific prior written
 * permission.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * --------------------------------------------------------------------------- */
#ifndef _STACK_DEF_H
#define _STACK_DEF_H
#include "stdint.h"

#define BTSDK_TRUE         1
#define BTSDK_FALSE        0

/* Max size value used in service attribute structures */
#define BTSDK_SERVICENAME_MAXLENGTH              80
#define BTSDK_MAX_SUPPORT_FORMAT                 6        /* OPP format number */
#define BTSDK_PATH_MAXLENGTH                     256      /* Shall not be larger than FTP_MAX_PATH and OPP_MAX_PATH */
#define BTSDK_CARDNAME_MAXLENGTH                 256      /* Shall not be larger than OPP_MAX_NAME */
#define BTSDK_PACKETTYPE_MAXNUM                  10       /* PAN supported network packet type */

/* Max size value used in device attribute structures */
#define BTSDK_DEVNAME_LEN                        64       /* Shall not be larger than MAX_NAME_LEN */
#define BTSDK_SHORTCUT_NAME_LEN                  100
#define BTSDK_BDADDR_LEN                         6
#define BTSDK_LINKKEY_LEN                        16
#define BTSDK_PINCODE_LEN                        16
/* For compatibility */
#define BTSDK_LINK_KEY_LENGTH                    16
#define BTSDK_PIN_CODE_LEN                       16

/* if: BT2.1 Supported */
#define BTSDK_MAX_EIR_LEN                        240
#define BT_SHIFT_BITS_8                          8
/* value range for ownAddrType/peerAddrType */
typedef enum {
    BLE_PUBLIC_DEVICE_ADDRESS =                             0x00,
    BLE_RANDOM_DEVICE_ADDRESS =                             0x01,
    BLE_PUBLIC_IDENTITY_ADDRESS =                           0x02,
    BLE_RANDOM_STATIC_IDENTITY_ADDRESS =                    0x03
} ble_address_type;

typedef enum {
    BTSDK_EIR_FLAGS = 1,
    BTSDK_EIR_UUID16_MORE,
    BTSDK_EIR_UUID16_COMPLETE,
    BTSDK_EIR_UUID32_MORE,
    BTSDK_EIR_UUID32_COMPLETE,
    BTSDK_EIR_UUID128_MORE,
    BTSDK_EIR_UUID128_COMPLETE,
    BTSDK_EIR_NAME_SHORT,
    BTSDK_EIR_NAME_COMPLETE,
    BTSDK_EIR_TX_POWER_LEVEL,
    BTSDK_EIR_VENDOR_SPECIFIC = 0xFF,
} btsdk_eir_data_type;
/* endif: BT2.1 Supported */


/* Invalid handle value for all handle type */
#define BTSDK_INVALID_HANDLE                    0x00000000

/* Invalid ACL handle value */
#define BTSDK_INVALID_ACL_HANDLE                0xFFFF

/* Error Code List */
#define BTSDK_OK                                0x0000

/* SDP error */
#define BTSDK_ER_SDP_INDEX                      0x00C0
#define BTSDK_ER_SERVER_IS_ACTIVE               (BTSDK_ER_SDP_INDEX + 0x0000)
#define BTSDK_ER_NO_SERVICE                     (BTSDK_ER_SDP_INDEX + 0x0001)
#define BTSDK_ER_SERVICE_RECORD_NOT_EXIST       (BTSDK_ER_SDP_INDEX + 0x0002)

/* General Error */
#define BTSDK_ER_GENERAL_INDEX                  0x0300
#define BTSDK_ER_HANDLE_NOT_EXIST               (BTSDK_ER_GENERAL_INDEX + 0x0001)
#define BTSDK_ER_OPERATION_FAILURE              (BTSDK_ER_GENERAL_INDEX + 0x0002)
#define BTSDK_ER_SDK_UNINIT                     (BTSDK_ER_GENERAL_INDEX + 0x0003)
#define BTSDK_ER_INVALID_PARAMETER              (BTSDK_ER_GENERAL_INDEX + 0x0004)
#define BTSDK_ER_NULL_POINTER                   (BTSDK_ER_GENERAL_INDEX + 0x0005)
#define BTSDK_ER_NO_MEMORY                      (BTSDK_ER_GENERAL_INDEX + 0x0006)
#define BTSDK_ER_BUFFER_NOT_ENOUGH              (BTSDK_ER_GENERAL_INDEX + 0x0007)
#define BTSDK_ER_FUNCTION_NOTSUPPORT            (BTSDK_ER_GENERAL_INDEX + 0x0008)
#define BTSDK_ER_NO_FIXED_PIN_CODE              (BTSDK_ER_GENERAL_INDEX + 0x0009)
#define BTSDK_ER_CONNECTION_EXIST               (BTSDK_ER_GENERAL_INDEX + 0x000A)
#define BTSDK_ER_OPERATION_CONFLICT             (BTSDK_ER_GENERAL_INDEX + 0x000B)
#define BTSDK_ER_NO_MORE_CONNECTION_ALLOWED     (BTSDK_ER_GENERAL_INDEX + 0x000C)
#define BTSDK_ER_ITEM_EXIST                     (BTSDK_ER_GENERAL_INDEX + 0x000D)
#define BTSDK_ER_ITEM_INUSE                     (BTSDK_ER_GENERAL_INDEX + 0x000E)
#define BTSDK_ER_DEVICE_UNPAIRED                (BTSDK_ER_GENERAL_INDEX + 0x000F)
#define BTSDK_ER_BLUETOOTH_NOTREADY             (BTSDK_ER_GENERAL_INDEX + 0x0010)
#define BTSDK_ER_CHANGE_NOTHING                 (BTSDK_ER_GENERAL_INDEX + 0x0011)
#define BTSDK_ER_FLOW_CONTROL_OFF               (BTSDK_ER_GENERAL_INDEX + 0x0012)
#define BTSDK_ER_NO_FREE_ATT_HANDLE             (BTSDK_ER_GENERAL_INDEX + 0x0013)
#define BTSDK_ER_INC_SVC_CIRCULAR_DEPENDENCY    (BTSDK_ER_GENERAL_INDEX + 0x0014)

/* HCI Error */
#define BTSDK_ER_HCI_INDEX                                        0x0400
#define BTSDK_ER_UNKNOWN_HCI_COMMAND                              (BTSDK_ER_HCI_INDEX + 0x0001)
#define BTSDK_ER_NO_CONNECTION                                    (BTSDK_ER_HCI_INDEX + 0x0002)
#define BTSDK_ER_HARDWARE_FAILURE                                 (BTSDK_ER_HCI_INDEX + 0x0003)
#define BTSDK_ER_PAGE_TIMEOUT                                     (BTSDK_ER_HCI_INDEX + 0x0004)
#define BTSDK_ER_AUTHENTICATION_FAILURE                           (BTSDK_ER_HCI_INDEX + 0x0005)
#define BTSDK_ER_KEY_MISSING                                      (BTSDK_ER_HCI_INDEX + 0x0006)
#define BTSDK_ER_MEMORY_FULL                                      (BTSDK_ER_HCI_INDEX + 0x0007)
#define BTSDK_ER_CONNECTION_TIMEOUT                               (BTSDK_ER_HCI_INDEX + 0x0008)
#define BTSDK_ER_MAX_NUMBER_OF_CONNECTIONS                        (BTSDK_ER_HCI_INDEX + 0x0009)
#define BTSDK_ER_MAX_NUMBER_OF_SCO_CONNECTIONS                    (BTSDK_ER_HCI_INDEX + 0x000a)
#define BTSDK_ER_ACL_CONNECTION_ALREADY_EXISTS                    (BTSDK_ER_HCI_INDEX + 0x000b)
#define BTSDK_ER_COMMAND_DISALLOWED                               (BTSDK_ER_HCI_INDEX + 0x000c)
#define BTSDK_ER_HOST_REJECTED_LIMITED_RESOURCES                  (BTSDK_ER_HCI_INDEX + 0x000d)
#define BTSDK_ER_HOST_REJECTED_SECURITY_REASONS                   (BTSDK_ER_HCI_INDEX + 0x000e)
#define BTSDK_ER_HOST_REJECTED_PERSONAL_DEVICE                    (BTSDK_ER_HCI_INDEX + 0x000f)
#define BTSDK_ER_HOST_TIMEOUT                                     (BTSDK_ER_HCI_INDEX + 0x0010)
#define BTSDK_ER_UNSUPPORTED_FEATURE                              (BTSDK_ER_HCI_INDEX + 0x0011)
#define BTSDK_ER_INVALID_HCI_COMMAND_PARAMETERS                   (BTSDK_ER_HCI_INDEX + 0x0012)
#define BTSDK_ER_PEER_DISCONNECTION_USER_END                      (BTSDK_ER_HCI_INDEX + 0x0013)
#define BTSDK_ER_PEER_DISCONNECTION_LOW_RESOURCES                 (BTSDK_ER_HCI_INDEX + 0x0014)
#define BTSDK_ER_PEER_DISCONNECTION_TO_POWER_OFF                  (BTSDK_ER_HCI_INDEX + 0x0015)
#define BTSDK_ER_LOCAL_DISCONNECTION                              (BTSDK_ER_HCI_INDEX + 0x0016)
#define BTSDK_ER_REPEATED_ATTEMPTS                                (BTSDK_ER_HCI_INDEX + 0x0017)
#define BTSDK_ER_PAIRING_NOT_ALLOWED                              (BTSDK_ER_HCI_INDEX + 0x0018)
#define BTSDK_ER_UNKNOWN_LMP_PDU                                  (BTSDK_ER_HCI_INDEX + 0x0019)
#define BTSDK_ER_UNSUPPORTED_REMOTE_FEATURE                       (BTSDK_ER_HCI_INDEX + 0x001a)
#define BTSDK_ER_SCO_OFFSET_REJECTED                              (BTSDK_ER_HCI_INDEX + 0x001b)
#define BTSDK_ER_SCO_INTERVAL_REJECTED                            (BTSDK_ER_HCI_INDEX + 0x001c)
#define BTSDK_ER_SCO_AIR_MODE_REJECTED                            (BTSDK_ER_HCI_INDEX + 0x001d)
#define BTSDK_ER_INVALID_LMP_PARAMETERS                           (BTSDK_ER_HCI_INDEX + 0x001e)
#define BTSDK_ER_UNSPECIFIED_ERROR                                (BTSDK_ER_HCI_INDEX + 0x001f)
#define BTSDK_ER_UNSUPPORTED_LMP_PARAMETER_VALUE                  (BTSDK_ER_HCI_INDEX + 0x0020)
#define BTSDK_ER_ROLE_CHANGE_NOT_ALLOWED                          (BTSDK_ER_HCI_INDEX + 0x0021)
#define BTSDK_ER_LMP_RESPONSE_TIMEOUT                             (BTSDK_ER_HCI_INDEX + 0x0022)
#define BTSDK_ER_LMP_ERROR_TRANSACTION_COLLISION                  (BTSDK_ER_HCI_INDEX + 0x0023)
#define BTSDK_ER_LMP_PDU_NOT_ALLOWED                              (BTSDK_ER_HCI_INDEX + 0x0024)
#define BTSDK_ER_ENCRYPTION_MODE_NOT_ACCEPTABLE                   (BTSDK_ER_HCI_INDEX + 0x0025)
#define BTSDK_ER_UNIT_KEY_USED                                    (BTSDK_ER_HCI_INDEX + 0x0026)
#define BTSDK_ER_QOS_IS_NOT_SUPPORTED                             (BTSDK_ER_HCI_INDEX + 0x0027)
#define BTSDK_ER_INSTANT_PASSED                                   (BTSDK_ER_HCI_INDEX + 0x0028)
#define BTSDK_ER_PAIRING_WITH_UNIT_KEY_NOT_SUPPORTED              (BTSDK_ER_HCI_INDEX + 0x0029)
#define BTSDK_ER_DIFFERENT_TRANSACTION_COLLISION                  (BTSDK_ER_HCI_INDEX + 0x002A)
#define BTSDK_ER_QOS_UNACCEPTABLE_PARAMETER                       (BTSDK_ER_HCI_INDEX + 0x002C)
#define BTSDK_ER_QOS_REJECTED                                     (BTSDK_ER_HCI_INDEX + 0x002D)
#define BTSDK_ER_CHANNEL_CLASS_NOT_SUPPORTED                      (BTSDK_ER_HCI_INDEX + 0x002E)
#define BTSDK_ER_INSUFFICIENT_SECURITY                            (BTSDK_ER_HCI_INDEX + 0x002F)
#define BTSDK_ER_PARAMETER_OUT_OF_RANGE                           (BTSDK_ER_HCI_INDEX + 0x0030)
#define BTSDK_ER_ROLE_SWITCH_PENDING                              (BTSDK_ER_HCI_INDEX + 0x0032)
#define BTSDK_ER_RESERVED_SLOT_VIOLATION                          (BTSDK_ER_HCI_INDEX + 0x0034)
#define BTSDK_ER_ROLE_SWITCH_FAILED                               (BTSDK_ER_HCI_INDEX + 0x0035)
#define BTSDK_ER_CONN_CMD_NOT_MATCH_ADDR_TYPE                     (BTSDK_ER_HCI_INDEX + 0x0036)

/* OBEX error */
#define BTSDK_ER_OBEX_INDEX                                       0x0600
#define BTSDK_ER_AUTH_FAILED                                      (BTSDK_ER_OBEX_INDEX + 0x0007)
#define BTSDK_ER_SESSION_CREATE_FAILED                            (BTSDK_ER_OBEX_INDEX + 0x0008)
#define BTSDK_ER_SESSION_CLOSE_CFM                                (BTSDK_ER_OBEX_INDEX + 0x0009)
#define BTSDK_ER_SESSION_RESUME_CFMERR                            (BTSDK_ER_OBEX_INDEX + 0x000B)
#define BTSDK_ER_SESSION_SUSPEND_CFMERR                           (BTSDK_ER_OBEX_INDEX + 0x000C)
#define BTSDK_ER_SESSION_SUSPEND_INDERR                           (BTSDK_ER_OBEX_INDEX + 0x000D)
#define BTSDK_ER_SESSION_SSN_INVAILD                              (BTSDK_ER_OBEX_INDEX + 0x000E)
#define BTSDK_ER_RTXTIMEOUT                                       (BTSDK_ER_OBEX_INDEX + 0x000F)
#define BTSDK_ER_ERROR_PACKET_RECEIVED                            (BTSDK_ER_OBEX_INDEX + 0x0010)
#define BTSDK_ER_CONTINUE                                         (BTSDK_ER_OBEX_INDEX + 0x0090)
#define BTSDK_ER_SUCCESS                                          (BTSDK_ER_OBEX_INDEX + 0x00A0)
#define BTSDK_ER_CREATED                                          (BTSDK_ER_OBEX_INDEX + 0x00A1)
#define BTSDK_ER_ACCEPTED                                         (BTSDK_ER_OBEX_INDEX + 0x00A2)
#define BTSDK_ER_NON_AUTH_INFO                                    (BTSDK_ER_OBEX_INDEX + 0x00A3)
#define BTSDK_ER_NO_CONTENT                                       (BTSDK_ER_OBEX_INDEX + 0x00A4)
#define BTSDK_ER_RESET_CONTENT                                    (BTSDK_ER_OBEX_INDEX + 0x00A5)
#define BTSDK_ER_PARTIAL_CONTENT                                  (BTSDK_ER_OBEX_INDEX + 0x00A6)
#define BTSDK_ER_MULT_CHOICES                                     (BTSDK_ER_OBEX_INDEX + 0x00B0)
#define BTSDK_ER_MOVE_PERM                                        (BTSDK_ER_OBEX_INDEX + 0x00B1)
#define BTSDK_ER_MOVE_TEMP                                        (BTSDK_ER_OBEX_INDEX + 0x00B2)
#define BTSDK_ER_SEE_OTHER                                        (BTSDK_ER_OBEX_INDEX + 0x00B3)
#define BTSDK_ER_NOT_MODIFIED                                     (BTSDK_ER_OBEX_INDEX + 0x00B4)
#define BTSDK_ER_USE_PROXY                                        (BTSDK_ER_OBEX_INDEX + 0x00B5)
#define BTSDK_ER_BAD_REQUEST                                      (BTSDK_ER_OBEX_INDEX + 0x00C0)
#define BTSDK_ER_UNAUTHORIZED                                     (BTSDK_ER_OBEX_INDEX + 0x00C1)
#define BTSDK_ER_PAY_REQ                                          (BTSDK_ER_OBEX_INDEX + 0x00C2)
#define BTSDK_ER_FORBIDDEN                                        (BTSDK_ER_OBEX_INDEX + 0x00C3)
#define BTSDK_ER_NOTFOUND                                         (BTSDK_ER_OBEX_INDEX + 0x00C4)
#define BTSDK_ER_METHOD_NOT_ALLOWED                               (BTSDK_ER_OBEX_INDEX + 0x00C5)
#define BTSDK_ER_NOT_ACCEPTABLE                                   (BTSDK_ER_OBEX_INDEX + 0x00C6)
#define BTSDK_ER_PROXY_AUTH_REQ                                   (BTSDK_ER_OBEX_INDEX + 0x00C7)
#define BTSDK_ER_REQUEST_TIMEOUT                                  (BTSDK_ER_OBEX_INDEX + 0x00C8)
#define BTSDK_ER_CONFLICT                                         (BTSDK_ER_OBEX_INDEX + 0x00C9)
#define BTSDK_ER_GONE                                             (BTSDK_ER_OBEX_INDEX + 0x00CA)
#define BTSDK_ER_LEN_REQ                                          (BTSDK_ER_OBEX_INDEX + 0x00CB)
#define BTSDK_ER_PREC_FAIL                                        (BTSDK_ER_OBEX_INDEX + 0x00CC)
#define BTSDK_ER_REQ_ENTITY_TOO_LARGE                             (BTSDK_ER_OBEX_INDEX + 0x00CD)
#define BTSDK_ER_URL_TOO_LARGE                                    (BTSDK_ER_OBEX_INDEX + 0x00CE)
#define BTSDK_ER_UNSUPPORTED_MEDIA_TYPE                           (BTSDK_ER_OBEX_INDEX + 0x00CF)
#define BTSDK_ER_SVR_ERR                                          (BTSDK_ER_OBEX_INDEX + 0x00D0)
#define BTSDK_ER_NOTIMPLEMENTED                                   (BTSDK_ER_OBEX_INDEX + 0x00D1)
#define BTSDK_ER_BAD_GATEWAY                                      (BTSDK_ER_OBEX_INDEX + 0x00D2)
#define BTSDK_ER_SERVICE_UNAVAILABLE                              (BTSDK_ER_OBEX_INDEX + 0x00D3)
#define BTSDK_ER_GATEWAY_TIMEOUT                                  (BTSDK_ER_OBEX_INDEX + 0x00D4)
#define BTSDK_ER_HTTP_NOTSUPPORT                                  (BTSDK_ER_OBEX_INDEX + 0x00D5)
#define BTSDK_ER_DATABASE_FULL                                    (BTSDK_ER_OBEX_INDEX + 0x00E0)
#define BTSDK_ER_DATABASE_LOCK                                    (BTSDK_ER_OBEX_INDEX + 0x00E1)

/* ATT & BLE Profiles error */
/* ATT Common Errors */
#define BTSDK_ER_BLE_INDEX                                        0X0800
#define BTSDK_ER_ATT_INVALID_HANDLE                               (BTSDK_ER_BLE_INDEX + 0x0001)
#define BTSDK_ER_ATT_READ_NOT_PERMITTED                           (BTSDK_ER_BLE_INDEX + 0x0002)
#define BTSDK_ER_ATT_WRITE_NOT_PERMITTED                          (BTSDK_ER_BLE_INDEX + 0x0003)
#define BTSDK_ER_ATT_INVALID_PDU                                  (BTSDK_ER_BLE_INDEX + 0x0004)
#define BTSDK_ER_ATT_INSUFFICIENT_AUTHENTICATION                  (BTSDK_ER_BLE_INDEX + 0x0005)
#define BTSDK_ER_ATT_REQUEST_NOT_SUPPORTED                        (BTSDK_ER_BLE_INDEX + 0x0006)
#define BTSDK_ER_ATT_INVALID_OFFSET                               (BTSDK_ER_BLE_INDEX + 0x0007)
#define BTSDK_ER_ATT_INSUFFICIENT_AUTHORIZATION                   (BTSDK_ER_BLE_INDEX + 0x0008)
#define BTSDK_ER_ATT_PREPARE_QUEUE_FULL                           (BTSDK_ER_BLE_INDEX + 0x0009)
#define BTSDK_ER_ATT_ATTRIBUTE_NOT_FOUND                          (BTSDK_ER_BLE_INDEX + 0x000A)
#define BTSDK_ER_ATT_ATTRIBUTE_NOT_LONG                           (BTSDK_ER_BLE_INDEX + 0x000B)
#define BTSDK_ER_ATT_INSUFFICIENT_ENCRYPTION_KEYSIZE              (BTSDK_ER_BLE_INDEX + 0x000C)
#define BTSDK_ER_ATT_INVALID_ATTRIBUTE_VALUE_LENGTH               (BTSDK_ER_BLE_INDEX + 0x000D)
#define BTSDK_ER_ATT_UNLIKELY_ERROR                               (BTSDK_ER_BLE_INDEX + 0x000E)
#define BTSDK_ER_ATT_INSUFFICIENT_ENCRYPTION                      (BTSDK_ER_BLE_INDEX + 0x000F)
#define BTSDK_ER_ATT_UNSUPPORTED_GROUPTYPE                        (BTSDK_ER_BLE_INDEX + 0x0010)
#define BTSDK_ER_ATT_INSUFFICIENT_RESOURCES                       (BTSDK_ER_BLE_INDEX + 0x0011)
/* CSS V2 */
#define BTSDK_ER_ATT_CCC_IMPROPERLY_CONFIGURED                    (BTSDK_ER_BLE_INDEX + 0x00FD)
#define BTSDK_ER_ATT_PROCEDURE_ALREADY_IN_PROGRESS                (BTSDK_ER_BLE_INDEX + 0x00FE)
#define BTSDK_ER_ATT_OUT_OF_RANGE                                 (BTSDK_ER_BLE_INDEX + 0x00FF)
/* Private Errors - For reference only. The actual meaning may be changed by different server implementations */
#define BTSDK_ER_ATT_SMP_FAILED                                   (BTSDK_ER_BLE_INDEX + 0x00E9)
#define BTSDK_ER_ATT_TASK_CANCEL                                  (BTSDK_ER_BLE_INDEX + 0x00EB)
#define BTSDK_ER_ATT_BTSTACK_RELEASED                             (BTSDK_ER_BLE_INDEX + 0x00EC)
#define BTSDK_ER_ATT_TL_CONNECT_FAILED                            (BTSDK_ER_BLE_INDEX + 0x00ED)
#define BTSDK_ER_ATT_RESPONSE_TIMEOUT                             (BTSDK_ER_BLE_INDEX + 0x00EE)

/* APP extended error base */
#define BTSDK_ER_APPEXTEND_INDEX                                0x4000

/* Class of Device */
/* major service classes */
#define BTSDK_SRVCLS_LDM                        0x002000
#define BTSDK_SRVCLS_POSITION                   0x010000
#define BTSDK_SRVCLS_NETWORK                    0x020000
#define BTSDK_SRVCLS_RENDER                     0x040000
#define BTSDK_SRVCLS_CAPTURE                    0x080000
#define BTSDK_SRVCLS_OBJECT                     0x100000
#define BTSDK_SRVCLS_AUDIO                      0x200000
#define BTSDK_SRVCLS_TELEPHONE                  0x400000
#define BTSDK_SRVCLS_INFOR                      0x800000
#define btsdk_srvcls_mask(a)                    (((uint32_t)(a) >> 13) & 0x7FF)

/* major device classes */
#define BTSDK_DEVCLS_MISC                       0x000000
#define BTSDK_DEVCLS_COMPUTER                   0x000100
#define BTSDK_DEVCLS_PHONE                      0x000200
#define BTSDK_DEVCLS_LAP                        0x000300
#define BTSDK_DEVCLS_AUDIO                      0x000400
#define BTSDK_DEVCLS_PERIPHERAL                 0x000500
#define BTSDK_DEVCLS_IMAGE                      0x000600
#define BTSDK_DEVCLS_WEARABLE                   0x000700
#define BTSDK_DEVCLS_UNCLASSIFIED               0x001F00
#define btsdk_devcls_mask(a)                    (((uint32_t)(a) >> 8) & 0x1F)
#define btsdk_mindevcls_mask(a)                 (((uint32_t)(a) >> 2) & 0x3F)

/* the minor device class field - computer major class */
#define BTSDK_COMPCLS_UNCLASSIFIED              (BTSDK_DEVCLS_COMPUTER | 0x000000)
#define BTSDK_COMPCLS_DESKTOP                   (BTSDK_DEVCLS_COMPUTER | 0x000004)
#define BTSDK_COMPCLS_SERVER                    (BTSDK_DEVCLS_COMPUTER | 0x000008)
#define BTSDK_COMPCLS_LAPTOP                    (BTSDK_DEVCLS_COMPUTER | 0x00000C)
#define BTSDK_COMPCLS_HANDHELD                  (BTSDK_DEVCLS_COMPUTER | 0x000010)
#define BTSDK_COMPCLS_PALMSIZED                 (BTSDK_DEVCLS_COMPUTER | 0x000014)
#define BTSDK_COMPCLS_WEARABLE                  (BTSDK_DEVCLS_COMPUTER | 0x000018)

/* the minor device class field - phone major class */
#define BTSDK_PHONECLS_UNCLASSIFIED             (BTSDK_DEVCLS_PHONE | 0x000000)
#define BTSDK_PHONECLS_CELLULAR                 (BTSDK_DEVCLS_PHONE | 0x000004)
#define BTSDK_PHONECLS_CORDLESS                 (BTSDK_DEVCLS_PHONE | 0x000008)
#define BTSDK_PHONECLS_SMARTPHONE               (BTSDK_DEVCLS_PHONE | 0x00000C)
#define BTSDK_PHONECLS_WIREDMODEM               (BTSDK_DEVCLS_PHONE | 0x000010)
#define BTSDK_PHONECLS_COMMONISDNACCESS         (BTSDK_DEVCLS_PHONE | 0x000014)
#define BTSDK_PHONECLS_SIMCARDREADER            (BTSDK_DEVCLS_PHONE | 0x000018)

/* the minor device class field - LAN/Network access point major class */
#define BTSDK_LAP_FULLY                         (BTSDK_DEVCLS_LAP | 0x000000)
#define BTSDK_LAP_17                            (BTSDK_DEVCLS_LAP | 0x000020)
#define BTSDK_LAP_33                            (BTSDK_DEVCLS_LAP | 0x000040)
#define BTSDK_LAP_50                            (BTSDK_DEVCLS_LAP | 0x000060)
#define BTSDK_LAP_67                            (BTSDK_DEVCLS_LAP | 0x000080)
#define BTSDK_LAP_83                            (BTSDK_DEVCLS_LAP | 0x0000A0)
#define BTSDK_LAP_99                            (BTSDK_DEVCLS_LAP | 0x0000C0)
#define BTSDK_LAP_NOSRV                         (BTSDK_DEVCLS_LAP | 0x0000E0)

/* the minor device class field - Audio/Video major class */
#define BTSDK_AV_UNCLASSIFIED                   (BTSDK_DEVCLS_AUDIO | 0x000000)
#define BTSDK_AV_HEADSET                        (BTSDK_DEVCLS_AUDIO | 0x000004)
#define BTSDK_AV_HANDSFREE                      (BTSDK_DEVCLS_AUDIO | 0x000008)
#define BTSDK_AV_HEADANDHAND                    (BTSDK_DEVCLS_AUDIO | 0x00000C)
#define BTSDK_AV_MICROPHONE                     (BTSDK_DEVCLS_AUDIO | 0x000010)
#define BTSDK_AV_LOUDSPEAKER                    (BTSDK_DEVCLS_AUDIO | 0x000014)
#define BTSDK_AV_HEADPHONES                     (BTSDK_DEVCLS_AUDIO | 0x000018)
#define BTSDK_AV_PORTABLEAUDIO                  (BTSDK_DEVCLS_AUDIO | 0x00001C)
#define BTSDK_AV_CARAUDIO                       (BTSDK_DEVCLS_AUDIO | 0x000020)
#define BTSDK_AV_SETTOPBOX                      (BTSDK_DEVCLS_AUDIO | 0x000024)
#define BTSDK_AV_HIFIAUDIO                      (BTSDK_DEVCLS_AUDIO | 0x000028)
#define BTSDK_AV_VCR                            (BTSDK_DEVCLS_AUDIO | 0x00002C)
#define BTSDK_AV_VIDEOCAMERA                    (BTSDK_DEVCLS_AUDIO | 0x000030)
#define BTSDK_AV_CAMCORDER                      (BTSDK_DEVCLS_AUDIO | 0x000034)
#define BTSDK_AV_VIDEOMONITOR                   (BTSDK_DEVCLS_AUDIO | 0x000038)
#define BTSDK_AV_VIDEODISPANDLOUDSPK            (BTSDK_DEVCLS_AUDIO | 0x00003C)
#define BTSDK_AV_VIDEOCONFERENCE                (BTSDK_DEVCLS_AUDIO | 0x000040)
#define BTSDK_AV_GAMEORTOY                      (BTSDK_DEVCLS_AUDIO | 0x000048)

/* the minor device class field - peripheral major class */
#define BTSDK_PERIPHERAL_UNCLASSIFIED           (BTSDK_DEVCLS_PERIPHERAL | 0x000000)
#define BTSDK_PERIPHERAL_JOYSTICK               (BTSDK_DEVCLS_PERIPHERAL | 0x000004)
#define BTSDK_PERIPHERAL_GAMEPAD                (BTSDK_DEVCLS_PERIPHERAL | 0x000008)
#define BTSDK_PERIPHERAL_REMCONTROL             (BTSDK_DEVCLS_PERIPHERAL | 0x00000C)
#define BTSDK_PERIPHERAL_SENSE                  (BTSDK_DEVCLS_PERIPHERAL | 0x000010)
#define BTSDK_PERIPHERAL_TABLET                 (BTSDK_DEVCLS_PERIPHERAL | 0x000014)
#define BTSDK_PERIPHERAL_SIMCARDREADER          (BTSDK_DEVCLS_PERIPHERAL | 0x000018)
#define BTSDK_PERIPHERAL_KEYBOARD               (BTSDK_DEVCLS_PERIPHERAL | 0x000040)
#define BTSDK_PERIPHERAL_POINT                  (BTSDK_DEVCLS_PERIPHERAL | 0x000080)
#define BTSDK_PERIPHERAL_KEYORPOINT             (BTSDK_DEVCLS_PERIPHERAL | 0x0000C0)

/* the minor device class field - imaging major class */
#define BTSDK_IMAGE_DISPLAY                     (BTSDK_DEVCLS_IMAGE | 0x000010)
#define BTSDK_IMAGE_CAMERA                      (BTSDK_DEVCLS_IMAGE | 0x000020)
#define BTSDK_IMAGE_SCANNER                     (BTSDK_DEVCLS_IMAGE | 0x000040)
#define BTSDK_IMAGE_PRINTER                     (BTSDK_DEVCLS_IMAGE | 0x000080)

/* the minor device class field - wearable major class */
#define BTSDK_WERABLE_WATCH                     (BTSDK_DEVCLS_WEARABLE | 0x000004)
#define BTSDK_WERABLE_PAGER                     (BTSDK_DEVCLS_WEARABLE | 0x000008)
#define BTSDK_WERABLE_JACKET                    (BTSDK_DEVCLS_WEARABLE | 0x00000C)
#define BTSDK_WERABLE_HELMET                    (BTSDK_DEVCLS_WEARABLE | 0x000010)
#define BTSDK_WERABLE_GLASSES                   (BTSDK_DEVCLS_WEARABLE | 0x000014)

/* Class of Service */
#define BTSDK_CLS_SERIAL_PORT                   0x1101
#define BTSDK_CLS_LAN_ACCESS                    0x1102
#define BTSDK_CLS_DIALUP_NET                    0x1103
#define BTSDK_CLS_IRMC_SYNC                     0x1104
#define BTSDK_CLS_OBEX_OBJ_PUSH                 0x1105
#define BTSDK_CLS_OBEX_FILE_TRANS               0x1106
#define BTSDK_CLS_IRMC_SYNC_CMD                 0x1107
#define BTSDK_CLS_HEADSET                       0x1108
#define BTSDK_CLS_CORDLESS_TELE                 0x1109
#define BTSDK_CLS_AUDIO_SOURCE                  0x110A
#define BTSDK_CLS_AUDIO_SINK                    0x110B
#define BTSDK_CLS_AVRCP_TG                      0x110C
#define BTSDK_CLS_ADV_AUDIO_DISTRIB             0x110D
#define BTSDK_CLS_AVRCP_CT                      0x110E
#define BTSDK_CLS_VIDEO_CONFERENCE              0x110F
#define BTSDK_CLS_INTERCOM                      0x1110
#define BTSDK_CLS_FAX                           0x1111
#define BTSDK_CLS_HEADSET_AG                    0x1112
#define BTSDK_CLS_WAP                           0x1113
#define BTSDK_CLS_WAP_CLIENT                    0x1114
#define BTSDK_CLS_PAN_PANU                      0x1115
#define BTSDK_CLS_PAN_NAP                       0x1116
#define BTSDK_CLS_PAN_GN                        0x1117
#define BTSDK_CLS_DIRECT_PRINT                  0x1118
#define BTSDK_CLS_REF_PRINT                     0x1119
#define BTSDK_CLS_IMAGING                       0x111A
#define BTSDK_CLS_IMAG_RESPONDER                0x111B
#define BTSDK_CLS_IMAG_AUTO_ARCH                0x111C
#define BTSDK_CLS_IMAG_REF_OBJ                  0x111D
#define BTSDK_CLS_HANDSFREE                     0x111E
#define BTSDK_CLS_HANDSFREE_AG                  0x111F
#define BTSDK_CLS_DPS_REF_OBJ                   0x1120
#define BTSDK_CLS_REFLECTED_UI                  0x1121
#define BTSDK_CLS_BASIC_PRINT                   0x1122
#define BTSDK_CLS_PRINT_STATUS                  0x1123
#define BTSDK_CLS_HID                           0x1124
#define BTSDK_CLS_HCRP                          0x1125
#define BTSDK_CLS_HCR_PRINT                     0x1126
#define BTSDK_CLS_HCR_SCAN                      0x1127
#define BTSDK_CLS_SIM_ACCESS                    0x112D
#define BTSDK_CLS_PBAP_PCE                      0x112E
#define BTSDK_CLS_PBAP_PSE                      0x112F
#define BTSDK_CLS_PHONEBOOK_ACCESS              0x1130
#define BTSDK_CLS_MAP_MAS                       0x1132
#define BTSDK_CLS_MAP_MNS                       0x1133
#define BTSDK_CLS_MESSAGE_ACCESS                0x1134
#define BTSDK_CLS_PNP_INFO                      0x1200

/* type of Serial Wrap specific event */
#define BTSDK_APP_EV_SWRAP_DRVOPEN              1
#define BTSDK_APP_EV_SWRAP_DRVCLOSE             2

/* type of Connection event */
#define BTSDK_APP_EV_CONN_IND                    0x01
#define BTSDK_APP_EV_DISC_IND                    0x02
#define BTSDK_APP_EV_CONN_CFM                    0x07
#define BTSDK_APP_EV_DISC_CFM                    0x08
/* Definitions for Compatibility */
#define BTSDK_APP_EV_CONN                        0x01
#define BTSDK_APP_EV_DISC                        0x02

/* type of Shortcut event */
#define BTSDK_SHC_EV_CONN                        BTSDK_APP_EV_CONN
#define BTSDK_SHC_EV_DISC                        BTSDK_APP_EV_DISC

/* BPP specific event */
#define BTSDK_APP_EV_BPP_BASE                    0x50

#define BTSDK_APP_EV_BASE                        0x100
/* PAN specific event */
#define BTSDK_APP_EV_PAN_BASE                    0x100

/* OPP specific event */
#define BTSDK_APP_EV_OPP_BASE                    0x200

/* FTP specific event */
#define BTSDK_APP_EV_FTP_BASE                    0x300

/* SYNC specific event */
#define BTSDK_APP_EV_SYNC_BASE                   0x400

/* HCRP specific event */
#define BTSDK_APP_EV_HCRP_BASE                   0x500

/* HID specific event */
#define BTSDK_APP_EV_HID_BASE                    0x600

/* HID Server specific event */
#define BTSDK_APP_EV_HID_SVR_BASE                (BTSDK_APP_EV_HID_BASE + 0x10)

/* BIP specific event */
#define BTSDK_APP_EV_BIP_BASE                    0x700

/* HSP/HFP AG specific event. */
#define BTSDK_APP_EV_AGAP_BASE                   0x3200
enum btsdk_hfp_app_event_code_enum {
    /* hfp_set_state Callback to Application event Code */
    /* SLC - Both AG and HF */
    /* HFP Service Level connection established. Parameter: btsdk_hfp_conn_info_stru */
    BTSDK_HFP_EV_SLC_ESTABLISHED_IND = 0x01,
    BTSDK_HFP_EV_SLC_RELEASED_IND,     /* SPP connection released. Parameter: btsdk_hfp_conn_info_stru */

    /* SCO - Both AG and HF  */
    BTSDK_HFP_EV_AUDIO_CONN_ESTABLISHED_IND, /* SCO audio connection established */
    BTSDK_HFP_EV_AUDIO_CONN_RELEASED_IND,    /* SCO audio connection released */

    /* Status Changed Indication */
    BTSDK_HFP_EV_STANDBY_IND,     /* STANDBY Menu, the incoming call or outgoing call or ongoing call is canceled  */
    /* ONGOING-CALL Menu, a call (incoming call or outgoing call) is established (ongoing)
    */
    BTSDK_HFP_EV_ONGOINGCALL_IND,
    /* RINGING Menu, a call is incoming. Parameter: uint8_t - in-band ring tone or not. */
    BTSDK_HFP_EV_RINGING_IND,
    BTSDK_HFP_EV_OUTGOINGCALL_IND, /* OUTGOING-CALL Menu, an outgoing call is being established, 3Way in Guideline P91
                                    */
    BTSDK_HFP_EV_CALLHELD_IND,     /* BTRH-HOLD Menu, +BTRH:0, AT+BTRH=0, incoming call is put on hold */
    BTSDK_HFP_EV_CALL_WAITING_IND, /* 10, Call Waiting Menu, +CCWA, When Call=Active, call waiting notification.
                                      Parameter: btsdk_hfp_phone_info_stru */
    BTSDK_HFP_EV_TBUSY_IND,        /* GSM Network Remote Busy, TBusy Timer Activated */

    /* AG & HF APP General event Indication */
    BTSDK_HFP_EV_GENERATE_INBAND_RINGTONE_IND, /* AG Only, Generate the in-band ring tone */
    /* 0x90d, Terminate local generated ring tone or the in-band ring tone */
    BTSDK_HFP_EV_TERMINATE_LOCAL_RINGTONE_IND,
    /* +BVRA:1, voice recognition activated indication or HF request to start
       voice recognition procedure */
    BTSDK_HFP_EV_VOICE_RECOGN_ACTIVATED_IND,
    BTSDK_HFP_EV_VOICE_RECOGN_DEACTIVATED_IND, /* +BVRA:0, voice recognition deactivated indication or requests AG to
                                                  deactivate the voice recognition procedure */
    BTSDK_HFP_EV_NETWORK_AVAILABLE_IND,        /* +CIEV:<service><value>, cellular network is available */
    BTSDK_HFP_EV_NETWORK_UNAVAILABLE_IND,      /* +CIEV:<service><value>, cellular network is unavailable */
    BTSDK_HFP_EV_ROAMING_RESET_IND,            /* +CIEV:<roam><value>, roaming is not active */
    BTSDK_HFP_EV_ROAMING_ACTIVE_IND,           /* +CIEV:<roam><value>, a roaming is active */
    /* 20, +CIEV:<signal><value>, signal strength indication. Parameter: uint8_t -
       indicator value */
    BTSDK_HFP_EV_SIGNAL_STRENGTH_IND,
    BTSDK_HFP_EV_BATTERY_CHARGE_IND,     /* +CIEV:<battchg><value>, battery charge indication. Parameter: uint8_t -
                                            indicator value  */
    BTSDK_HFP_EV_CHLDHELD_ACTIVATED_IND, /* +CIEV:<callheld><1>, Call on CHLD Held to be or has been actived. */
    BTSDK_HFP_EV_CHLDHELD_RELEASED_IND,  /* +CIEV:<callheld><0>, Call on CHLD Held to be or has been released. */
    BTSDK_HFP_EV_MICVOL_CHANGED_IND,     /* +VGM, AT+VGM, microphone volume changed indication */
    BTSDK_HFP_EV_SPKVOL_CHANGED_IND,     /* +VGS, AT+VGS, speaker volume changed indication */

    /* OK and Error Code - HF only */
    BTSDK_HFP_EV_ATCMD_RESULT, /* 0x91a,  HF Received OK, Error/+CME ERROR from AG or Wait for AG Response Timeout.
                                  Parameter: btsdk_hfp_at_cmd_result_stru */

    /* to HF APP, Call Related, AG Send information to HF */
    BTSDK_HFP_EV_CLIP_IND,              /* +CLIP, Phone Number Indication. Parameter: btsdk_hfp_phone_info_stru */
    /* 0x91c, +CLCC, the current calls of AG. Parameter: btsdk_hfp_clcc_info_stru */
    BTSDK_HFP_EV_CURRENT_CALLS_IND,
    BTSDK_HFP_EV_NETWORK_OPERATOR_IND,  /* +COPS, the current network operator name of AG. Parameter:
                                           btsdk_hfp_cops_info_stru */
    /* 30, +CNUM, the subscriber number of AG. Parameter: btsdk_hfp_phone_info_stru */
    BTSDK_HFP_EV_SUBSCRIBER_NUMBER_IND,
    /* +BINP, AG inputted phone number for voice-tag; requests AG to input a phone
       number for the voice-tag at the HF side. Parameter: btsdk_hfp_phone_info_stru
    */
    BTSDK_HFP_EV_VOICETAG_PHONE_NUM_IND,

    /* AG APP, HF Request or Indicate AG */
    BTSDK_HFP_EV_CURRENT_CALLS_REQ,           /* AT+CLCC, query the list of current calls in AG. */
    BTSDK_HFP_EV_NETWORK_OPERATOR_FORMAT_REQ, /* AT+COPS=3, 0, indicate app the network operator name should be set to
                                                 long alphanumeric */
    BTSDK_HFP_EV_NETWORK_OPERATOR_REQ,   /* AT+COPS?, requests AG to respond with +COPS response
                                            indicating the currently selected operator */
    BTSDK_HFP_EV_SUBSCRIBER_NUMBER_REQ,  /* AT+CNUM, query the AG subscriber number information. */
    /* AT+BINP, requests AG to input a phone number for the voice-tag at the HF */
    BTSDK_HFP_EV_VOICETAG_PHONE_NUM_REQ,
    BTSDK_HFP_EV_CUR_INDICATOR_VAL_REQ,  /* AT+CIND?, get the current indicator during the service level connection
                                            initialization procedure */
    BTSDK_HFP_EV_HF_DIAL_REQ,            /* ATD, instructs AG to dial the specific phone number.
                                            Parameter: (HFP only) uint8_t* - phone number */
    BTSDK_HFP_EV_HF_MEM_DIAL_REQ,        /* ATD>, instructs AG to dial the phone number indexed by
                                            the specific memory location of SIM card.
                                            Parameter: uint8_t* - memory location */
    BTSDK_HFP_EV_HF_LASTNUM_REDIAL_REQ,  /* 40, AT+BLDN, instructs AG to redial the last dialed phone number */
    BTSDK_HFP_EV_MANUFACTURER_REQ,       /* AT+CGMI, requests AG to respond with the Manufacturer ID */
    BTSDK_HFP_EV_MODEL_REQ,              /* AT+CGMM, requests AG to respond with the Model ID */
    BTSDK_HFP_EV_NREC_DISABLE_REQ,       /* AT+NREC=0, requests AG to disable NREC function */
    BTSDK_HFP_EV_DTMF_REQ,               /* AT+VTS, instructs AG to transmit the specific DTMF code.
                                            Parameter: uint8_t - DTMF code */
    BTSDK_HFP_EV_ANSWER_CALL_REQ,        /* inform AG app to answer the call. Parameter: uint8_t - One of
                                            BTSDK_HFP_TYPE_INCOMING_CALL, BTSDK_HFP_TYPE_HELDINCOMING_CALL. */
    BTSDK_HFP_EV_CANCEL_CALL_REQ,        /* inform AG app to cancel the call. Parameter: uint8_t - One of
                                            BTSDK_HFP_TYPE_ALL_CALLS, BTSDK_HFP_TYPE_INCOMING_CALL,
                                            BTSDK_HFP_TYPE_HELDINCOMING_CALL, BTSDK_HFP_TYPE_OUTGOING_CALL,
                                            BTSDK_HFP_TYPE_ONGOING_CALL. */
    BTSDK_HFP_EV_HOLD_CALL_REQ,          /* inform AG app to hold the incoming call (AT+BTRH=0) */

    /* AG APP, 3-Way Calling */
    BTSDK_HFP_EV_REJECTWAITINGCALL_REQ,      /* AT+CHLD=0, Release all held calls or reject waiting call. */
    /* AT+CHLD=1, Accept the held or waiting call and release all avtive calls.
       Parameter: uint8_t - value of <idx> */
    BTSDK_HFP_EV_ACPTWAIT_RELEASEACTIVE_REQ,
    BTSDK_HFP_EV_HOLDACTIVECALL_REQ,         /* 50, AT+CHLD=2, Held Specified Active Call.
                                                Parameter: uint8_t - value of <idx> */
    BTSDK_HFP_EV_ADD_ONEHELDCALL_2ACTIVE_REQ, /* AT+CHLD=3, Add One CHLD Held Call to Active Call. */
    BTSDK_HFP_EV_LEAVE3WAYCALLING_REQ,        /* AT+CHLD=4, Leave The 3-Way Calling. */

    /* Extended */
    BTSDK_HFP_EV_EXTEND_CMD_IND,         /* indicate app extend command received.
                                            Parameter: uint8_t* - Full extended AT command or result code. */
    BTSDK_HFP_EV_PRE_SCO_CONNECTION_IND, /* indicate app to create SCO connection. Parameter:
                                            btsdk_hfp_pre_sco_conn_ind_stru. */
    BTSDK_HFP_EV_SPP_ESTABLISHED_IND,    /* 0x937, SPP connection created. Parameter: btsdk_hfp_conn_info_stru. added
                                            2008-7-3 */
    BTSDK_HFP_EV_HF_MANUFACTURERID_IND,  /* ManufacturerID indication. Parameter: uint8_t* - Manufacturer ID of the AG
                                            device, a null-terminated ASCII string. */
    BTSDK_HFP_EV_HF_MODELID_IND,         /* ModelID indication.  Parameter: uint8_t* - Model ID of the AG device, a
                                            null-terminated ASCII string. */

    BTSDK_HFP_EV_HF_RECV_RING_IND,       /* +RING received, play ring tone if possible, otherwise ignored.
                                            Parameter: uint8_t - 1 if AG supports in-band ring tone; 0 otherwise. */
    BTSDK_HFP_EV_HF_RECV_CIEV_CALL_IND, /* 59, +CIEV (call, callsetup, or callheld indicator) received. Parameter:
                                           uint8_t* - indicator value(call - 10 + (0, 1); callsetup - 20 + (0, 1, 2,
                                               3);
                                           callheld - 30 + (0, 1, 2)). */
    BTSDK_HFP_EV_SERVER_START_CFM,               /* 60 Server start confirmation with Server Channel applied */
    BTSDK_HFP_EV_GET_AGFEATURES_COMPLETE,        /* 61 Get AG features complete */
    BTSDK_HFP_EV_CHECKSCO_COMPLETE,              /* 62 Check SCO complete */
    BTSDK_HFP_EV_CONNECT_CFM,                    /* 63 RFCOMM connect confirmation with instance handle,
                                                struct hfp_connect_ev_stru */
    BTSDK_HFP_EV_CONNECT_IND,                    /* 64 RFCOMM Connection indication */
    BTSDK_HFP_EV_DISCONNECT_COMPLETE,            /* 65 HFP connection released indication with reason,
                                                used to replace HFP_EV_SLC_RELEASED_IND */
    BTSDK_HFP_EV_THREEWAY_ERROR_IND, /* 66 CHLD does not succeed. Parameter: uint8_t* - 3WayCalling Error Code */
    BTSDK_HFP_EV_BTRH_QUERY_REQ = 67, /* AT+BTRH? received, HF query the status of the "Response and Hold" state of the
                                      AG. Parameter: uint8_t* to return the current state: 0 -
                                          No incoming call on hold;
                                      1 - The incoming call is on hold;
                                      Other values to respond later with btsdk_agap_set_cur_indicator_val */
    BTSDK_HFP_EV_CODEC_CHANGED_IND,   /* Codec changed by the latest Codec Negotiation procedure. Parameter: uint8_t -
                                        Codec ID, e.g. BTSDK_SCO_CODEC_CVSD */
    BTSDK_HFP_EV_AUDIO_CONNECT_FAILURE_IND, /* 69 Fails to create the audio connection. UINT8* to return the reason */
    BTSDK_HFP_EV_INBANDRING_IND = 74,   /* 74 +BSIR=<supported>, +BRSF(inband ring feature)
                                                                    AG notify the HF inband ring supported or not,
                                                                    uint8_t * - inbandring supported (0,1) */
    BTSDK_HFP_EV_SCO_DISCONNECT_COMPLETE = 80,
};

/* A2DP specific event. */
#define BTSDK_APP_EV_A2DP_BASE                            0xA00
/* A2DP SNK specific event. */
#define    BTSDK_APP_EV_A2DP_REGSEP                       (BTSDK_APP_EV_A2DP_BASE + 0x01)
#define    BTSDK_APP_EV_A2DP_SEPDISCOVERY                 (BTSDK_APP_EV_A2DP_BASE + 0x02)
#define    BTSDK_APP_EV_A2DP_GETCAPABILITIES              (BTSDK_APP_EV_A2DP_BASE + 0x03)
#define    BTSDK_APP_EV_A2DP_SETCONFIGURATION             (BTSDK_APP_EV_A2DP_BASE + 0x04)
#define    BTSDK_APP_EV_A2DP_GETCONFIGURATION             (BTSDK_APP_EV_A2DP_BASE + 0x05)
#define    BTSDK_APP_EV_A2DP_STREAMOPEN                   (BTSDK_APP_EV_A2DP_BASE + 0x06)
#define    BTSDK_APP_EV_A2DP_STREAMSTART                  (BTSDK_APP_EV_A2DP_BASE + 0x07)
#define    BTSDK_APP_EV_A2DP_STREAMSUSPEND                (BTSDK_APP_EV_A2DP_BASE + 0x08)
#define    BTSDK_APP_EV_A2DP_STREAMCLOSE                  (BTSDK_APP_EV_A2DP_BASE + 0x09)
#define    BTSDK_APP_EV_A2DP_STREAMABORT                  (BTSDK_APP_EV_A2DP_BASE + 0x0a)
/* this event cannot be used, use BTSDK_APP_EV_A2DP_SETCONFIGURATION */
#define    BTSDK_APP_EV_A2DP_RECONFIGURE                  (BTSDK_APP_EV_A2DP_BASE + 0x0b)
#define    BTSDK_APP_EV_A2DP_DATAIND                      (BTSDK_APP_EV_A2DP_BASE + 0x0c)
#define    BTSDK_APP_EV_A2DP_STREAMDONE                   (BTSDK_APP_EV_A2DP_BASE + 0x0d)

/* AVRCP specific event. */
#define BTSDK_APP_EV_AVRCP_BASE                           0xB00
/* AVRCP TG specific event. */
#define BTSDK_APP_EV_AVTG_BASE                            BTSDK_APP_EV_AVRCP_BASE
#define BTSDK_APP_EV_AVTG_ATTACHPLAYER_IND                (BTSDK_APP_EV_AVTG_BASE + 0x01)
#define BTSDK_APP_EV_AVRCP_IND_CONN_CFM                   (BTSDK_APP_EV_AVTG_BASE + 0x02)
#define BTSDK_APP_EV_AVRCP_DETACHPLAYER_IND               (BTSDK_APP_EV_AVTG_BASE + 0x03)
#define BTSDK_APP_EV_AVRCP_PASSTHROUGH_IND                (BTSDK_APP_EV_AVTG_BASE + 0x06)
#define BTSDK_APP_EV_AVRCP_VENDORDEP_IND                  (BTSDK_APP_EV_AVTG_BASE + 0x07)
#define BTSDK_APP_EV_AVRCP_IND_CONN                       BTSDK_APP_EV_AVTG_ATTACHPLAYER_IND
#define BTSDK_APP_EV_AVRCP_IND_DISCONN                    BTSDK_APP_EV_AVRCP_DETACHPLAYER_IND
#define BTSDK_APP_EV_AVRCP_METADATA_IND                   (BTSDK_APP_EV_AVTG_BASE + 0x0d)
#define BTSDK_APP_EV_AVRCP_GROUPNAV_IND                   (BTSDK_APP_EV_AVTG_BASE + 0x0f)

/* AVRCP CT specific event. */
#define BTSDK_APP_EV_AVCT_BASE                            BTSDK_APP_EV_AVRCP_BASE
#define BTSDK_APP_EV_AVRCP_UNITINFO_RSP                   (BTSDK_APP_EV_AVCT_BASE + 0x08)
#define BTSDK_APP_EV_AVRCP_SUBUNITINFO_RSP                (BTSDK_APP_EV_AVCT_BASE + 0x09)
#define BTSDK_APP_EV_AVRCP_PASSTHROUGH_RSP                (BTSDK_APP_EV_AVCT_BASE + 0x0a)
#define BTSDK_APP_EV_AVRCP_VENDORDEP_RSP                  (BTSDK_APP_EV_AVCT_BASE + 0x0b)
#define BTSDK_APP_EV_AVRCP_METADATA_RSP                   (BTSDK_APP_EV_AVCT_BASE + 0x0c)
#define BTSDK_APP_EV_AVRCP_GROUPNAV_RSP                   (BTSDK_APP_EV_AVCT_BASE + 0x0e)

/* AVRCP CT change notification events */
#define BTSDK_APP_EV_AVRCP_CT_NOTIF_BASE                            (BTSDK_APP_EV_AVRCP_BASE + 0x80)
#define BTSDK_APP_EV_AVRCP_PLAYBACK_STATUS_CHANGED_NOTIF            (BTSDK_APP_EV_AVRCP_CT_NOTIF_BASE + 0x01)
#define BTSDK_APP_EV_AVRCP_TRACK_CHANGED_NOTIF                      (BTSDK_APP_EV_AVRCP_CT_NOTIF_BASE + 0x02)
#define BTSDK_APP_EV_AVRCP_TRACK_REACHED_END_NOTIF                  (BTSDK_APP_EV_AVRCP_CT_NOTIF_BASE + 0x03)
#define BTSDK_APP_EV_AVRCP_TRACK_REACHED_START_NOTIF                (BTSDK_APP_EV_AVRCP_CT_NOTIF_BASE + 0x04)
#define BTSDK_APP_EV_AVRCP_PLAYBACK_POS_CHANGED_NOTIF               (BTSDK_APP_EV_AVRCP_CT_NOTIF_BASE + 0x05)
#define BTSDK_APP_EV_AVRCP_BATT_STATUS_CHANGED_NOTIF                (BTSDK_APP_EV_AVRCP_CT_NOTIF_BASE + 0x06)
#define BTSDK_APP_EV_AVRCP_SYSTEM_STATUS_CHANGED_NOTIF              (BTSDK_APP_EV_AVRCP_CT_NOTIF_BASE + 0x07)
#define BTSDK_APP_EV_AVRCP_PLAYER_APPLICATION_SETTING_CHANGED_NOTIF (BTSDK_APP_EV_AVRCP_CT_NOTIF_BASE + 0x08)
#define BTSDK_APP_EV_AVRCP_NOW_PLAYING_CONTENT_CHANGED_NOTIF        (BTSDK_APP_EV_AVRCP_CT_NOTIF_BASE + 0x09)
#define BTSDK_APP_EV_AVRCP_AVAILABLE_PLAYERS_CHANGED_NOTIF          (BTSDK_APP_EV_AVRCP_CT_NOTIF_BASE + 0x0A)
#define BTSDK_APP_EV_AVRCP_ADDRESSED_PLAYER_CHANGED_NOTIF           (BTSDK_APP_EV_AVRCP_CT_NOTIF_BASE + 0x0B)
#define BTSDK_APP_EV_AVRCP_UIDS_CHANGED_NOTIF                       (BTSDK_APP_EV_AVRCP_CT_NOTIF_BASE + 0x0C)
#define BTSDK_APP_EV_AVRCP_VOLUME_CHANGED_NOTIF                     (BTSDK_APP_EV_AVRCP_CT_NOTIF_BASE + 0x0D)

/* AVRCP TG AV/C & Browsing specific event */
#define BTSDK_APP_EV_AVRCP_TG_METAIND_BASE                          0xC00
#define BTSDK_APP_EV_AVRCP_GET_CAPABILITIES_IND                     (BTSDK_APP_EV_AVRCP_TG_METAIND_BASE + 0x10)
#define BTSDK_APP_EV_AVRCP_LIST_PLAYER_SETTING_ATTR_IND             (BTSDK_APP_EV_AVRCP_TG_METAIND_BASE + 0x11)
#define BTSDK_APP_EV_AVRCP_LIST_PLAYER_SETTING_VALUES_IND           (BTSDK_APP_EV_AVRCP_TG_METAIND_BASE + 0x12)
#define BTSDK_APP_EV_AVRCP_GET_CURRENTPLAYER_SETTING_VALUE_IND      (BTSDK_APP_EV_AVRCP_TG_METAIND_BASE + 0x13)
#define BTSDK_APP_EV_AVRCP_SET_CURRENTPLAYER_SETTING_VALUE_IND      (BTSDK_APP_EV_AVRCP_TG_METAIND_BASE + 0x14)
#define BTSDK_APP_EV_AVRCP_GET_PLAYER_SETTING_ATTR_TEXT_IND         (BTSDK_APP_EV_AVRCP_TG_METAIND_BASE + 0x15)
#define BTSDK_APP_EV_AVRCP_GET_PLAYER_SETTING_VALUE_TEXT_IND        (BTSDK_APP_EV_AVRCP_TG_METAIND_BASE + 0x16)
#define BTSDK_APP_EV_AVRCP_INFORM_CHARACTERSET_IND                  (BTSDK_APP_EV_AVRCP_TG_METAIND_BASE + 0x17)
#define BTSDK_APP_EV_AVRCP_INFORM_BATTERYSTATUS_OF_CT_IND           (BTSDK_APP_EV_AVRCP_TG_METAIND_BASE + 0x18)
#define BTSDK_APP_EV_AVRCP_GET_ELEMENT_ATTR_IND                     (BTSDK_APP_EV_AVRCP_TG_METAIND_BASE + 0x20)
#define BTSDK_APP_EV_AVRCP_GET_PLAY_STATUS_IND                      (BTSDK_APP_EV_AVRCP_TG_METAIND_BASE + 0x30)
#define BTSDK_APP_EV_AVRCP_REGISTER_NOTIFICATION_IND                (BTSDK_APP_EV_AVRCP_TG_METAIND_BASE + 0x31)
#define BTSDK_APP_EV_AVRCP_SET_ABSOLUTE_VOLUME_IND                  (BTSDK_APP_EV_AVRCP_TG_METAIND_BASE + 0x50)
#define BTSDK_APP_EV_AVRCP_SET_ADDRESSED_PLAYER_IND                 (BTSDK_APP_EV_AVRCP_TG_METAIND_BASE + 0x60)
#define BTSDK_APP_EV_AVRCP_SET_BROWSED_PLAYER_IND                   (BTSDK_APP_EV_AVRCP_TG_METAIND_BASE + 0x70)
#define BTSDK_APP_EV_AVRCP_GET_FOLDER_ITEMS_IND                     (BTSDK_APP_EV_AVRCP_TG_METAIND_BASE + 0x71)
#define BTSDK_APP_EV_AVRCP_CHANGE_PATH_IND                          (BTSDK_APP_EV_AVRCP_TG_METAIND_BASE + 0x72)
#define BTSDK_APP_EV_AVRCP_GET_ITEM_ATTRIBUTES_IND                  (BTSDK_APP_EV_AVRCP_TG_METAIND_BASE + 0x73)
#define BTSDK_APP_EV_AVRCP_PLAY_ITEM_IND                            (BTSDK_APP_EV_AVRCP_TG_METAIND_BASE + 0x74)
#define BTSDK_APP_EV_AVRCP_GET_TOTAL_NUMBER_OF_ITMES_IND            (BTSDK_APP_EV_AVRCP_TG_METAIND_BASE + 0x75)
#define BTSDK_APP_EV_AVRCP_SEARCH_IND                               (BTSDK_APP_EV_AVRCP_TG_METAIND_BASE + 0x80)
#define BTSDK_APP_EV_AVRCP_ADDTO_NOWPLAYING_IND                     (BTSDK_APP_EV_AVRCP_TG_METAIND_BASE + 0x90)
#define BTSDK_APP_EV_AVRCP_GENERAL_REJECT_IND                       (BTSDK_APP_EV_AVRCP_TG_METAIND_BASE + 0xA0)

/* AVRCP CT AV/C & Browsing specific event */
#define BTSDK_APP_EV_AVRCP_CT_METARSP_BASE                          0xD00
#define BTSDK_APP_EV_AVRCP_GET_CAPABILITIES_RSP                     (BTSDK_APP_EV_AVRCP_CT_METARSP_BASE + 0x10)
#define BTSDK_APP_EV_AVRCP_LIST_PLAYER_SETTING_ATTR_RSP             (BTSDK_APP_EV_AVRCP_CT_METARSP_BASE + 0x11)
#define BTSDK_APP_EV_AVRCP_LIST_PLAYER_SETTING_VALUES_RSP           (BTSDK_APP_EV_AVRCP_CT_METARSP_BASE + 0x12)
#define BTSDK_APP_EV_AVRCP_GET_CURRENTPLAYER_SETTING_VALUE_RSP      (BTSDK_APP_EV_AVRCP_CT_METARSP_BASE + 0x13)
#define BTSDK_APP_EV_AVRCP_SET_CURRENTPLAYER_SETTING_VALUE_RSP      (BTSDK_APP_EV_AVRCP_CT_METARSP_BASE + 0x14)
#define BTSDK_APP_EV_AVRCP_GET_PLAYER_SETTING_ATTR_TEXT_RSP         (BTSDK_APP_EV_AVRCP_CT_METARSP_BASE + 0x15)
#define BTSDK_APP_EV_AVRCP_GET_PLAYER_SETTING_VALUE_TEXT_RSP        (BTSDK_APP_EV_AVRCP_CT_METARSP_BASE + 0x16)
#define BTSDK_APP_EV_AVRCP_INFORM_CHARACTERSET_RSP                  (BTSDK_APP_EV_AVRCP_CT_METARSP_BASE + 0x17)
#define BTSDK_APP_EV_AVRCP_INFORM_BATTERYSTATUS_OF_CT_RSP           (BTSDK_APP_EV_AVRCP_CT_METARSP_BASE + 0x18)
#define BTSDK_APP_EV_AVRCP_GET_ELEMENT_ATTR_RSP                     (BTSDK_APP_EV_AVRCP_CT_METARSP_BASE + 0x20)
#define BTSDK_APP_EV_AVRCP_GET_PLAY_STATUS_RSP                      (BTSDK_APP_EV_AVRCP_CT_METARSP_BASE + 0x30)
#define BTSDK_APP_EV_AVRCP_SET_ABSOLUTE_VOLUME_RSP                  (BTSDK_APP_EV_AVRCP_CT_METARSP_BASE + 0x50)
#define BTSDK_APP_EV_AVRCP_SET_ADDRESSED_PLAYER_RSP                 (BTSDK_APP_EV_AVRCP_CT_METARSP_BASE + 0x60)
#define BTSDK_APP_EV_AVRCP_SET_BROWSED_PLAYER_RSP                   (BTSDK_APP_EV_AVRCP_CT_METARSP_BASE + 0x70)
#define BTSDK_APP_EV_AVRCP_GET_FOLDER_ITEMS_RSP                     (BTSDK_APP_EV_AVRCP_CT_METARSP_BASE + 0x71)
#define BTSDK_APP_EV_AVRCP_CHANGE_PATH_RSP                          (BTSDK_APP_EV_AVRCP_CT_METARSP_BASE + 0x72)
#define BTSDK_APP_EV_AVRCP_GET_ITEM_ATTRIBUTES_RSP                  (BTSDK_APP_EV_AVRCP_CT_METARSP_BASE + 0x73)
#define BTSDK_APP_EV_AVRCP_PLAY_ITEM_RSP                            (BTSDK_APP_EV_AVRCP_CT_METARSP_BASE + 0x74)
#define BTSDK_APP_EV_AVRCP_GET_TOTAL_NUMBER_OF_ITMES_RSP            (BTSDK_APP_EV_AVRCP_CT_METARSP_BASE + 0x75)
#define BTSDK_APP_EV_AVRCP_SEARCH_RSP                               (BTSDK_APP_EV_AVRCP_CT_METARSP_BASE + 0x80)
#define BTSDK_APP_EV_AVRCP_ADDTO_NOWPLAYING_RSP                     (BTSDK_APP_EV_AVRCP_CT_METARSP_BASE + 0x90)
#define BTSDK_APP_EV_AVRCP_GENERAL_REJECT_RSP                       (BTSDK_APP_EV_AVRCP_CT_METARSP_BASE + 0xA0)

/* MAP specific event */
#define BTSDK_APP_EV_MAP_BASE                                       0xE00

#endif
