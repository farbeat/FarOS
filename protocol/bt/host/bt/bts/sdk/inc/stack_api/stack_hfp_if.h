/*
 * Copyright (c) @CompanyNameMagicTag 2021-2022. All rights reserved.
 * Description: test at cmd proc
 * Author: @CompanyNameTag
 * Create: 2021-09-01
 */
#ifndef STATCK_HFP_IF_H
#define STATCK_HFP_IF_H

#include "ipc.h"
#include "stack_def.h"
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif
#define BTSDK_HFP_PHONENUM_LEN 32

#define CONFIG_BTSDK_HFPHF_SERVER_CHANNEL 7
#define CONFIG_BTSDK_HFPAG_SERVER_CHANNEL 8
#define CONFIG_BTSDK_HS_SERVER_CHANNEL    9
#define CONFIG_BTSDK_VG_SERVER_CHANNEL    10

/* sync handle */
#define BTSDK_HFP_SVC_HANDLE_CLASS_ID               0x8000
#define BTSDK_HFP_CONN_HANDLE_CLASS_ID              0x8001


/* AG Action Reason */
#define BTSDK_HFP_CANCELED_ALLCALL                  0x01    /* AG released all calls or GSM Service Unavailable */
#define BTSDK_HFP_CANCELED_CALLSETUP                0x02    /* AG or GSM Release the Incoming Call or Outgoing Call */
#define BTSDK_HFP_CANCELED_LASTCALL                 0x03    /* AG or GSM Release Last Call in Call=1 */
#define BTSDK_HFP_CANCELED_CALLHELD                 0x04    /* GSM Release only held calls */
#define BTSDK_HFP_AG_PRIVATE_MODE                   0x05    /* Answer the Outgoing/Incoming Call on AG */
#define BTSDK_HFP_AG_HANDSFREE_MODE                 0x06    /* Answer the Outgoing/Incoming Call on HF */

/* AG type of the call, possible values of HFP_EV_ANSWER_CALL_REQ and HFP_EV_CANCEL_CALL_REQ event parameter */
#define BTSDK_HFP_TYPE_ALL_CALLS                    0x01    /* (Release) all the existing calls */
#define BTSDK_HFP_TYPE_INCOMING_CALL                0x02    /* (Reject or accept) the incoming call */
#define BTSDK_HFP_TYPE_HELDINCOMING_CALL            0x03    /* (Reject or accept) the Held incoming call */
#define BTSDK_HFP_TYPE_OUTGOING_CALL                0x04    /* (Release) the outgoing call */
#define BTSDK_HFP_TYPE_ONGOING_CALL                 0x05    /* (Release) the ongoing call */


/* Possible received events from GSM/CDMA cellular network */
#define BTSDK_AGAP_NETWORK_RMT_IS_BUSY              0x01
#define BTSDK_AGAP_NETWORK_ALERTING_RMT             0x02
#define BTSDK_AGAP_NETWORK_INCOMING_CALL            0x03
#define BTSDK_AGAP_NETWORK_RMT_ANSWER_CALL          0x04
#define BTSDK_AGAP_NETWORK_SVC_UNAVAILABLE          0x05
#define BTSDK_AGAP_NETWORK_SVC_AVAILABLE            0x06
#define BTSDK_AGAP_NETWORK_SIGNAL_STRENGTH          0x07
#define BTSDK_AGAP_NETWORK_ROAMING_RESET            0x08
#define BTSDK_AGAP_NETWORK_ROAMING_ACTIVE           0x09

/*---------------------------------------------------------------------------*/
/*                                      HF / AG BRSF Features                        */
/*---------------------------------------------------------------------------*/
/* BRSF feature mask ID for HF */
#define BTSDK_HF_BRSF_NREC                          0x00000001 /* EC and/or NR function */
#define BTSDK_HF_BRSF_3WAYCALL                      0x00000002 /* Call waiting and 3-way calling */
#define BTSDK_HF_BRSF_CLIP                          0x00000004 /* CLI presentation capability */
#define BTSDK_HF_BRSF_BVRA                          0x00000008 /* Voice recognition activation */
#define BTSDK_HF_BRSF_RMTVOLCTRL                    0x00000010 /* Remote volume control */
#define BTSDK_HF_BRSF_ENHANCED_CALLSTATUS           0x00000020 /* Enhanced call status */
#define BTSDK_HF_BRSF_ENHANCED_CALLCONTROL          0x00000040 /* Enhanced call control */
#define BTSDK_HF_BRSF_CODEC_NEGOTIATION             0x00000080 /* Codec negotiation, HFP1.6 defined */
#define BTSDK_HF_BRSF_ALL                           0x000000FF /* 0~7 Bits */
#define BTSDK_HF_BRSF_DEFAULT                       0x00000000 /* HF Server Default BRSF Features, P79 */

/* SDP */
#define BTSDK_HF_SDP_NREC                           0x0001 /* 0, EC and/or NR function */
#define BTSDK_HF_SDP_3WAYCALL                       0x0002 /* 1, Call waiting and 3-way calling */
#define BTSDK_HF_SDP_CLIP                           0x0004 /* 2, CLI presentation capability */
#define BTSDK_HF_SDP_BVRA                           0x0008 /* 3, Voice recognition activation */
#define BTSDK_HF_SDP_RMTVOLCTRL                     0x0010 /* 4, Remote volume control */
#define BTSDK_HF_SDP_WIDE_BAND_SPEECH               0x0020 /* 5, Wide band speech */

/* BRSF feature mask ID for AG, defined in spec */
#define BTSDK_AG_BRSF_3WAYCALL                      0x00000001 /* Three-way calling */
#define BTSDK_AG_BRSF_NREC                          0x00000002 /* EC and/or NR function */
#define BTSDK_AG_BRSF_BVRA                          0x00000004 /* Voice recognition function */
#define BTSDK_AG_BRSF_INBANDRING                    0x00000008 /* In-band ring tone capability */
#define BTSDK_AG_BRSF_BINP                          0x00000010 /* Attach a number to a voice tag */
#define BTSDK_AG_BRSF_REJECT_CALL                   0x00000020 /* Ability to reject a call */
#define BTSDK_AG_BRSF_ENHANCED_CALLSTATUS           0x00000040 /* Enhanced call status */
#define BTSDK_AG_BRSF_ENHANCED_CALLCONTROL          0x00000080 /* Enhanced call control */
#define BTSDK_AG_BRSF_EXTENDED_ERRORRESULT          0x00000100 /* Extended Error Result Codes */
#define BTSDK_AG_BRSF_CODEC_NEGOTIATION             0x00000200 /* Codec negotiation, HFP1.6 defined */
#define BTSDK_AG_BRSF_ALL                           0x000003FF /* 0~9 Bits */
#define BTSDK_AG_BRSF_DEFAULT                       0x00000009 /* AG Server Default BRSF Features, P80 */

/* SDP */
#define BTSDK_AG_SDP_3WAYCALL                       0x00000001 /* 0, Three-way calling */
#define BTSDK_AG_SDP_NREC                           0x00000002 /* 1, EC and/or NR function */
#define BTSDK_AG_SDP_BVRA                           0x00000004 /* 2, Voice recognition function */
#define BTSDK_AG_SDP_INBANDRING                     0x00000008 /* 3, In-band ring tone capability */
#define BTSDK_AG_SDP_BINP                           0x00000010 /* 4, Attach a number to a voice tag */
#define BTSDK_AG_SDP_WIDE_BAND_SPEECH               0x00000020 /* 5, Wide band speech */

/* Additional feature masks specify the AG supported indicators. Refer to HFP_FEA_AG_XXX macros. */
#define BTSDK_AG_IND_SERVICE        0x00010000   /* "service" indicator supported. */
#define BTSDK_AG_IND_CALL           0x00020000   /* "call" indicator supported. */
#define BTSDK_AG_IND_CALLSETUP      0x00040000   /* "callsetup" indicator supported. */
#define BTSDK_AG_IND_CALLHELD       0x00080000   /* "callheld" indicator supported. */
#define BTSDK_AG_IND_SIGNAL         0x00100000   /* "signal" indicator supported. */
#define BTSDK_AG_IND_ROAM           0x00200000   /* "roam" indicator supported. */
#define BTSDK_AG_IND_BATTCHG        0x00400000   /* "battchg" indicator supported. */

/* Mapping of Codec Types to Codec ID's */
#define BTSDK_HFP_CODECID_CVSD        0x01
#define BTSDK_HFP_CODECID_MSBC        0x02

/* HF Send AT Commands to AG, AG Receive Commands, Command Code Group Mask */
#define BTSDK_HFP_CMD_GROUP1           0x8000         /* AT Command will response directly by OK */
#define BTSDK_HFP_CMD_GROUP2           0x4000         /* AT Command will response result 1st, then OK */
#define BTSDK_HFP_CMD_PARAM            0x1000         /* AT Command param exist or not bit-mask */
#define BTSDK_HFP_CMD_GROUPMASK        (BTSDK_HFP_CMD_GROUP1 | BTSDK_HFP_CMD_GROUP2)
/* HF AT Commands Group for SLC Establish Procedure */
#define BTSDK_HFP_CMD_BRSF             (BTSDK_HFP_CMD_GROUP2 | BTSDK_HFP_CMD_PARAM | 0x10)    /* AT+BRSF=<value> */
#define BTSDK_HFP_CMD_CIND_T           (BTSDK_HFP_CMD_GROUP2 | 0x11)                           /* AT+CIND=? */
#define BTSDK_HFP_CMD_CIND_R           (BTSDK_HFP_CMD_GROUP2 | 0x12)                           /* AT+CIND? */
#define BTSDK_HFP_CMD_CMER             (BTSDK_HFP_CMD_GROUP1 | BTSDK_HFP_CMD_PARAM | 0x20)    /* AT+CMER=3,0,0,1 */
#define BTSDK_HFP_CMD_CHLD_T           (BTSDK_HFP_CMD_GROUP2 | 0x13)                        /* AT+CHLD=? */
/* HF AT Commands Group1 */
/* AT+COPS=3,0 Set name format to long alphanumeric */
#define BTSDK_HFP_CMD_COPS_SET_FORMAT  (BTSDK_HFP_CMD_GROUP1 | 0x01)
/* AT+CCWA=1 Call waiting notification Activation */
#define BTSDK_HFP_CMD_CCWA_ACTIVATE    (BTSDK_HFP_CMD_GROUP1 | 0x02)
/* AT+CMEE= Enable the use of result code +CME ERROR: <err> */
#define BTSDK_HFP_CMD_CMEE             (BTSDK_HFP_CMD_GROUP1 | BTSDK_HFP_CMD_PARAM | 0x03)
/* AT+CLIP= Calling Line Identification Presentation */
#define BTSDK_HFP_CMD_CLIP             (BTSDK_HFP_CMD_GROUP1 | BTSDK_HFP_CMD_PARAM | 0x04)
/* AT+BVRA=1 Voice Recognition Activation */
#define BTSDK_HFP_CMD_BVRA_ENABLE      (BTSDK_HFP_CMD_GROUP1 | 0x05)
/* AT+BVRA=0 Voice Recognition Deactivation */
#define BTSDK_HFP_CMD_BVRA_DISABLE     (BTSDK_HFP_CMD_GROUP1 | 0x06)
/* AT+VTS= DTMF code */
#define BTSDK_HFP_CMD_VTS              (BTSDK_HFP_CMD_GROUP1 | BTSDK_HFP_CMD_PARAM | 0x07)
/* AT+VGS= Set speaker volume */
#define BTSDK_HFP_CMD_VGS              (BTSDK_HFP_CMD_GROUP1 | BTSDK_HFP_CMD_PARAM | 0x08)
/* AT+VGM= Set microphone volume */
#define BTSDK_HFP_CMD_VGM              (BTSDK_HFP_CMD_GROUP1 | BTSDK_HFP_CMD_PARAM | 0x09)
/* AT+NREC= Noise Reduction & Echo Canceling */
#define BTSDK_HFP_CMD_NREC             (BTSDK_HFP_CMD_GROUP1 | BTSDK_HFP_CMD_PARAM | 0x0a)
/* AT+CHLD=0 Held Call Release */
#define BTSDK_HFP_CMD_CHLD_0           (BTSDK_HFP_CMD_GROUP1 | 0x0b)
/* AT+CHLD=1 Release Specified Active Call */
#define BTSDK_HFP_CMD_CHLD_1           (BTSDK_HFP_CMD_GROUP1 | 0x0c)
/* AT+CHLD=2 Call Held or Active/Held Position Swap */
#define BTSDK_HFP_CMD_CHLD_2           (BTSDK_HFP_CMD_GROUP1 | 0x0d)
/* AT+CHLD=3 Adds a held call to the conversation */
#define BTSDK_HFP_CMD_CHLD_3           (BTSDK_HFP_CMD_GROUP1 | 0x0e)
/* AT+CHLD=4 Connects the two calls and disconnects the subscriber from both calls */
#define BTSDK_HFP_CMD_CHLD_4           (BTSDK_HFP_CMD_GROUP1 | 0x0f)
/* AT+CHUP Reject the Incoming Call or Terminate the Outgoing Call or Release the Ongoing Call */
#define BTSDK_HFP_CMD_CANCELCALL       (BTSDK_HFP_CMD_GROUP1 | 0x10)
/* ATA, Answer the Incoming Call */
#define BTSDK_HFP_CMD_ANSWERCALL       (BTSDK_HFP_CMD_GROUP1 | 0x11)
/* ATD, Dial the specific phone number */
#define BTSDK_HFP_CMD_DIAL             (BTSDK_HFP_CMD_GROUP1 | BTSDK_HFP_CMD_PARAM | 0x12)
/* ATD>, Dial the phone number indexed by the specific memory location */
#define BTSDK_HFP_CMD_MEMDIAL          (BTSDK_HFP_CMD_GROUP1 | BTSDK_HFP_CMD_PARAM | 0x13)
/* AT+BLDN, Redial the Last Dialed Phone Number */
#define BTSDK_HFP_CMD_BLDN             (BTSDK_HFP_CMD_GROUP1 | 0x14)
/* AT+CKPD=200, It is used for Headset  */
#define BTSDK_HFP_CMD_CKPD             (BTSDK_HFP_CMD_GROUP1 | 0x15)
/* AT+CKPD="e", It is used for Headset, used for reject call, it is not defined by HSP */
#define BTSDK_HFP_CMD_CKPD_REJ_CALL    (BTSDK_HFP_CMD_GROUP1 | 0x16)
/* AT+BIA=x,x,x,x,x Indicators Activation and Deactivation */
#define BTSDK_HFP_CMD_BIA              (BTSDK_HFP_CMD_GROUP1 | 0x17)
/* AT+BCC, Bluetooth Codec Connection */
#define BTSDK_HFP_CMD_BCC              (BTSDK_HFP_CMD_GROUP1 | 0x21)
/* AT+BCS=, Bluetooth Codec Selection */
#define BTSDK_HFP_CMD_BCS              (BTSDK_HFP_CMD_GROUP1 | BTSDK_HFP_CMD_PARAM | 0x22)
/* AT+BAC=, Bluetooth Available Codecs */
#define BTSDK_HFP_CMD_BAC              (BTSDK_HFP_CMD_GROUP1 | BTSDK_HFP_CMD_PARAM | 0x23)
/* HF AT Commands Group2 */
/* AT+COPS? Find the currently selected operator */
#define BTSDK_HFP_CMD_COPS_READ        (BTSDK_HFP_CMD_GROUP2 | 0x01)
/* AT+BINP=1 Bluetooth Input (attach a phone number to a voice tag) */
#define BTSDK_HFP_CMD_BINP             (BTSDK_HFP_CMD_GROUP2 | 0x02)
/* AT+BTRH? Query the status of the Rsponse and Hold state of the AG. */
#define BTSDK_HFP_CMD_BTRH_QUERY       (BTSDK_HFP_CMD_GROUP2 | 0x03)
/* AT+BTRH=0 Put the incoming call on Hold. 1 Accept, 2 Reject */
#define BTSDK_HFP_CMD_BTRH             (BTSDK_HFP_CMD_GROUP2 | BTSDK_HFP_CMD_PARAM | 0x04)
/* AT+CNUM query the AG subscriber number information. */
#define BTSDK_HFP_CMD_CNUM             (BTSDK_HFP_CMD_GROUP2 | 0x07)
/* AT+CLCC query the list of current calls in AG. */
#define BTSDK_HFP_CMD_CLCC             (BTSDK_HFP_CMD_GROUP2 | 0x08)
/* AT+CGMI Get the AG Manufacturer ID */
#define BTSDK_HFP_CMD_CGMI             (BTSDK_HFP_CMD_GROUP2 | 0x09)
/* AT+CGMM Get the AG Model ID */
#define BTSDK_HFP_CMD_CGMM             (BTSDK_HFP_CMD_GROUP2 | 0x0a)

#define BTSDK_HFP_EXTEND_CMD           0x88    /* Extended at-command, Both AG / HF use this */

/*-----------------------------------------------------------------------------*/
/*                      CME Error Code and Standard Error Code for APP         */
/*-----------------------------------------------------------------------------*/
/* This CME ERROR Code is only for APP Reference. More Code reference to GSM Spec. */
#define BTSDK_HFP_CMEERR_AGFAILURE                  0  /* +CME ERROR:0 - AG failure */
#define BTSDK_HFP_CMEERR_NOCONN2PHONE               1  /* +CME ERROR:1 - no connection to phone */
#define BTSDK_HFP_CMEERR_OPERATION_NOTALLOWED       3  /* +CME ERROR:3 - operation not allowed */
#define BTSDK_HFP_CMEERR_OPERATION_NOTSUPPORTED     4  /* +CME ERROR:4 - operation not supported */
#define BTSDK_HFP_CMEERR_PHSIMPIN_REQUIRED          5  /* +CME ERROR:5 - PH-SIM PIN required */
#define BTSDK_HFP_CMEERR_SIMNOT_INSERTED            10 /* +CME ERROR:10 - SIM not inserted */
#define BTSDK_HFP_CMEERR_SIMPIN_REQUIRED            11 /* +CME ERROR:11 - SIM PIN required */
#define BTSDK_HFP_CMEERR_SIMPUK_REQUIRED            12 /* +CME ERROR:12 - SIM PUK required */
#define BTSDK_HFP_CMEERR_SIM_FAILURE                13 /* +CME ERROR:13 - SIM failure */
#define BTSDK_HFP_CMEERR_SIM_BUSY                   14 /* +CME ERROR:14 - SIM busy */
#define BTSDK_HFP_CMEERR_INCORRECT_PASSWORD         16 /* +CME ERROR:16 - incorrect password */
#define BTSDK_HFP_CMEERR_SIMPIN2_REQUIRED           17 /* +CME ERROR:17 - SIM PIN2 required */
#define BTSDK_HFP_CMEERR_SIMPUK2_REQUIRED           18 /* +CME ERROR:18 - SIM PUK2 required */
#define BTSDK_HFP_CMEERR_MEMORY_FULL                20 /* +CME ERROR:20 - memory full */
#define BTSDK_HFP_CMEERR_INVALID_INDEX              21 /* +CME ERROR:21 - invalid index */
#define BTSDK_HFP_CMEERR_MEMORY_FAILURE             23 /* +CME ERROR:23 - memory failure */
#define BTSDK_HFP_CMEERR_TEXTSTRING_TOOLONG         24 /* +CME ERROR:24 - text string too long */
#define BTSDK_HFP_CMEERR_INVALID_CHAR_INTEXTSTRING  25 /* +CME ERROR:25 - invalid characters in text string */
#define BTSDK_HFP_CMEERR_DIAL_STRING_TOOLONG        26 /* +CME ERROR:26 - dial string too long */
#define BTSDK_HFP_CMEERR_INVALID_CHAR_INDIALSTRING  27 /* +CME ERROR:27 - invalid characters in dial string */
#define BTSDK_HFP_CMEERR_NETWORK_NOSERVICE          30 /* +CME ERROR:30 - no network service */
#define BTSDK_HFP_CMEERR_NETWORK_TIMEOUT            31 /* +CME ERROR:31 - network timeout */
#define BTSDK_HFP_CMEERR_EMERGENCYCALL_ONLY         32 /* +CME ERROR:32 - Network not allowed, emergency calls only */

/* APP specific error code. */
#define BTSDK_HFP_APPERR_TIMEOUT                    200 /* Wait for response timeout */

/* Standard error result code. */
#define BTSDK_HFP_STDERR_ERROR                      201 /* result code: ERROR */
#define BTSDK_HFP_STDRR_NOCARRIER                   202 /* result code: NO CARRIER */
#define BTSDK_HFP_STDERR_BUSY                       203 /* result code: BUSY */
#define BTSDK_HFP_STDERR_NOANSWER                   204 /* result code: NO ANSWER */
#define BTSDK_HFP_STDERR_DELAYED                    205 /* result code: DELAYED */
#define BTSDK_HFP_STDERR_BLACKLISTED                206 /* result code: BLACKLISTED */
#define BTSDK_HFP_OK                                255 /* result code: OK */

/* Possible SCO Codec */
#define BTSDK_SCO_CODEC_CVSD    1       /* CVSD Coding */
#define BTSDK_SCO_CODEC_MSBC    2       /* MSBC Coding */

/* SCO Codec Packet type */
#define BTSDK_SCO_PACKET_TYPE_HV1                 0x0020
#define BTSDK_SCO_PACKET_TYPE_HV2                 0x0040
#define BTSDK_SCO_PACKET_TYPE_HV3                 0x0080
#define BTSDK_SCO_PACKET_TYPE_DEFAULT_SCO         0x00E0
#define BTSDK_SCO_PACKET_TYPE_HV1_MAY             0x0001
#define BTSDK_SCO_PACKET_TYPE_HV2_MAY             0x0002
#define BTSDK_SCO_PACKET_TYPE_HV3_MAY             0x0004
#define BTSDK_SCO_PACKET_TYPE_EV3_MAY             0x0008
#define BTSDK_SCO_PACKET_TYPE_EV4_MAY             0x0010
#define BTSDK_SCO_PACKET_TYPE_EV5_MAY             0x0020
#define BTSDK_SCO_PACKET_TYPE_2EV3_MAY_NOT        0x0040
#define BTSDK_SCO_PACKET_TYPE_3EV3_MAY_NOT        0x0080
#define BTSDK_SCO_PACKET_TYPE_2EV5_MAY_NOT        0x0100
#define BTSDK_SCO_PACKET_TYPE_3EV5_MAY_NOT        0x0200
#define BTSDK_SCO_PACKET_TYPE_DEFAULT_SYNCHRONOUS 0x003F

/*-----------------------------------------------------------------------------*/
/*                     Extened Error Code for APP                              */
/*-----------------------------------------------------------------------------*/
/* 3WayCalling Error Code, for BTSDK_HFP_EV_THREEWAY_ERROR_IND event use. */
#define BTSDK_HFP_3WAY_ERR_BAD_STATUS        0x01
#define BTSDK_HFP_3WAY_ERR_IDX_INVALID       0x02
#define BTSDK_HFP_3WAY_ERR_CHLD_NOTSUPPORT   0x03
#define BTSDK_HFP_3WAY_ERR_BAD_CMD           0x04

/*---------------------------------------------------------------------------*/
/*                                 HFP APP Interface Structures                 */
/*---------------------------------------------------------------------------*/
/* Used by SDK internal message */
typedef struct {
    uint16_t role;      /* 16bit UUID specifies the local role of the connection:
                          BTSDK_CLS_HANDSFREE - Local device acts as a HF.
                          BTSDK_CLS_HANDSFREE_AG - Local device acts as a Hands-free AG.
                          BTSDK_CLS_HEADSET - Local device acts as a HS.
                          BTSDK_CLS_HEADSET_AG - Local device acts as a Headset AG. */
    uint16_t param_len;  /* Length of the param */
    uint8_t  *param;    /* Additional event parameter */
} bt_hfp_msg_param_stru, *pbt_hfp_msg_param_stru;

/* Parameter of the BTSDK_HFP_EV_SLC_ESTABLISHED_IND and BTSDK_HFP_EV_SLC_RELEASED_IND events */
typedef struct {
    uint16_t role;     /* 16bit UUID specifies the local role of the connection:
                          BTSDK_CLS_HANDSFREE - Local device acts as a HF.
                          BTSDK_CLS_HANDSFREE_AG - Local device acts as a Hands-free AG.
                          BTSDK_CLS_HEADSET - Local device acts as a HS.
                          BTSDK_CLS_HEADSET_AG - Local device acts as a Headset AG. */
    uint32_t dev_hdl;   /* Handle to the remote device. */
} btsdk_hfp_conn_info_stru, *pbtsdk_hfp_conn_info_stru;

/* Used By +COPS */
typedef struct {
    uint8_t mode;            /* current mode and provides no information with regard to the name of the operator */
    uint8_t format;          /* the format of the operator parameter string */
    uint8_t operator_len;
    int8_t  operator_name[1]; /* the string in alphanumeric format representing the name of the network operator */
} btsdk_hfp_cops_info_stru, *pbtsdk_hfp_cops_info_stru;

/* Used By +BINP, +CNUM, +CLIP, +CCWA */
typedef struct {
    uint8_t type;      /* the format of the phone number provided */
    /* Indicates which service this phone number relates to. Shall be either 4 (voice) or 5 (fax). */
    uint8_t service;
    uint8_t num_len;    /* the length of the phone number provided */
    int8_t  number[32];  /* subscriber number, the length shall be PHONENUM_MAX_DIGITS */
    uint8_t name_len;     /* length of subaddr */
    int8_t  alpha_str[1]; /* string type subaddress of format specified by <cli_validity> */
} btsdk_hfp_phone_info_stru, *pbtsdk_hfp_phone_info_stru;

/* Used By +CLCC */
typedef struct {
    uint8_t idx;       /* The numbering (start with 1) of the call given by the sequence of setting up or
                          receiving the calls */
    uint8_t dir;       /* Direction, 0=outgoing, 1=incoming */
    uint8_t status;    /* 0=active, 1=held, 2=dialling(outgoing), 3=alerting(outgoing), 4=incoming(incoming),
                          5=waiting(incoming) */
    uint8_t mode;      /* 0=voice, 1=data, 2=fax */
    uint8_t mpty;      /* 0=not multiparty, 1=multiparty */
    uint8_t type;      /* the format of the phone number provided */
    uint8_t num_len;    /* the length of the phone number provided */
    int8_t  number[1]; /* phone number */
} btsdk_hfp_clcc_info_stru, *pbtsdk_hfp_clcc_info_stru;

/* current state mask code for function hfp_ag_set_cur_indicator_val */
typedef struct {
    uint8_t     service;            /* 0=unavailable, 1=available */
    uint8_t     call;               /* 0=no active call, 1=have active call */
    uint8_t     callsetup;          /* 0=no callsetup, 1=incoming, 2=outgoing, 3=outalert */
    uint8_t     callheld;           /* 0=no callheld, 1=active-hold, 2=onhold */
    uint8_t     signal;             /* 0~5 */
    uint8_t     roam;               /* 0=no roam, 1= roam */
    uint8_t     battchg;            /* 0~5 */
} btsdk_hfp_cind_info_stru, *pbtsdk_hfp_cind_info_stru;

/* Used by hfp ipc message b to a */
typedef struct {
    unsigned long hfp_hdl;   /* Handle to the remote device. */
    unsigned short event;
    unsigned short param_len;
    unsigned char param[0];    /* Ipc event parameter */
} bt_hfp_ipc_msg_param_stru, *pbt_hfp_ipc_msg_param_stru;

/* Used by BTSDK_HFP_EV_ATCMD_RESULT */
typedef struct {
    uint16_t cmd_code;   /* Which AT command code got an error */
    /* What result occurs, BTSDK_HFP_APPERR_TIMEOUT, CME Error Code or standard error result code */
    uint8_t result_code;
} btsdk_hfp_at_cmd_result_stru, *pbtsdk_hfp_at_cmd_result_stru;

/* Used by BTSDK_HFP_EV_ATCMD_RESULT */
typedef struct {
    uint8_t bd[BTSDK_BDADDR_LEN]; /* [in] Address of the peer device */
    uint8_t initiator;            /* [in] BTSDK_HFP_CONN_SCO_PEER_INITIATOR or BTSDK_HFP_CONN_LOCAL_INITIATOR */
    uint8_t link_type;             /* [in] BTSDK_SCO_LINK or BTSDK_ESCO_LINK. */
    uint8_t codec;                /* [in] BTSDK_SCO_CODEC_CVSD or BTSDK_SCO_CODEC_MSBC.
                                     Shall be ignored when linkType is BTSDK_SCO_LINK. */
    uint8_t retransmission_effort; /* [in/out] Retransmission effort of the new SCO connection. Shall be ignored when
                                     linkType is BTSDK_SCO_LINK. */
    uint16_t max_latency;          /* [in/out] Max latency of the new SCO connection.
                                     Shall be ignored when linkType is BTSDK_SCO_LINK. */
    uint16_t packet_type;          /* [in/out] Packet type of the new SCO connection. */
    uint8_t  result;              /* [out] SCO created by the application or not.
                                     Possible values: BTSDK_AGAP_CONN_SCO_DEFAULT, BTSDK_AGAP_CONN_SCO_PENDING. */
} btsdk_hfp_pre_sco_conn_ind_stru, *pbtsdk_hfp_pre_sco_conn_ind_stru;

/* Possible values of the result member of BTSDK_AGAP_PreSCOConnIndStru */
#define BTSDK_HFP_CONN_SCO_DEFAULT 0x00 /* Application won't create the SCO connection */
#define BTSDK_HFP_CONN_SCO_PENDING 0x01 // Application will create the SCO connection later, usually in anther thread
#define BTSDK_HFP_CONN_SCO_CANCEL  0x02 /* Application cancels the SCO connection. */

#define BTSDK_HFP_CONN_SCO_PEER_INITIATOR    0x00 /* The peer device initiates the SCO/eSCO connection. */
#define BTSDK_HFP_CONN_LOCAL_INITIATOR       0x01 /* The local device initiates the SCO/eSCO connection. */
/* lParam of BTSDK_StartClient, BTSDK_StartClientEx and BTSDK_ConnectShortCutEx; and,
   ext_attributes of bt_sdk_local_server_attr_stru. */
typedef struct {
    uint32_t size;      /* Must set to sizeof(BtSdkHFPConnParamStru) */
    uint16_t mask;        /* Reserved, set to 0 */
    uint16_t features;  /* Local supported features.
                           1) For HSP, it shall be 0.
                           2) For HFP-HF, it can be the bit OR operation of following values:
                              BTSDK_HF_BRSF_NREC, BTSDK_HF_BRSF_3WAYCALL, BTSDK_HF_BRSF_CLIP,
                              BTSDK_HF_BRSF_BVRA, BTSDK_HF_BRSF_RMTVOLCTRL, BTSDK_HF_BRSF_ENHANCED_CALLSTATUS,
                              BTSDK_HF_BRSF_ENHANCED_CALLCONTROL, BTSDK_HF_BRSF_CODEC_NEGOTIATION.
                           3) For HFP-AG, it can be the bit OR operation of following values:
                              BTSDK_AG_BRSF_3WAYCALL, BTSDK_AG_BRSF_NREC, BTSDK_AG_BRSF_BVRA,
                              BTSDK_AG_BRSF_INBANDRING, BTSDK_AG_BRSF_BINP, BTSDK_AG_BRSF_REJECT_CALL,
                              BTSDK_AG_BRSF_ENHANCED_CALLSTATUS, BTSDK_AG_BRSF_ENHANCED_CALLCONTROL,
                              BTSDK_AG_BRSF_EXTENDED_ERRORRESULT, BTSDK_AG_BRSF_CODEC_NEGOTIATION.
                         */
} bt_sdk_hfpui_param_stru, *p_bt_sdk_hfpui_param_stru,
  bt_sdk_hfp_conn_param_stru, *p_bt_sdk_hfp_conn_param_stru,
  bt_sdk_local_hfp_server_attr_stru, *p_bt_sdk_hfp_local_hfp_server_attr_stru;

typedef struct {
    uint8_t *svc_hdl;
    uint8_t   svr_chnl;
} bt_hfp_reg_svr_cfm_stru;

typedef void (btsdk_hfp_callback)(uint32_t hdl, uint16_t event, uint8_t *param, uint16_t len);

int32_t btsdk_hfap_dial(uint32_t stream_hdl, const uint8_t* number, uint8_t len);
int32_t btsdk_hfap_cancel_call(uint32_t call_hdl);
int32_t btsdk_hfap_3way_calling_handler(uint32_t hdl, uint16_t op_code, uint8_t idx);
int32_t btsdk_hfap_answer_call(uint32_t call_hdl);
int32_t btsdk_hfap_set_spk_vol(uint32_t hdl, uint8_t volume);
int32_t btsdk_hfap_set_mic_vol(uint32_t hdl, uint8_t volume);
int32_t btsdk_hfap_audio_conn_trans(uint32_t hdl);
int32_t btsdk_hfap_tx_dtmf(uint32_t hdl, uint8_t code);
int32_t btsdk_hfap_get_current_calls(uint32_t call_hdl);
int32_t btsdk_hfap_voice_recognition_req(uint32_t hdl, uint8_t param);
int32_t btsdk_hfap_app_reg_cbk(btsdk_hfp_callback *pfunc);

/* HFP-AG */
int32_t btsdk_agap_answer_call(uint32_t hdl, uint8_t mode);
int32_t btsdk_agap_cancel_call(uint32_t hdl, uint8_t type);
int32_t btsdk_agap_audio_conn_trans(uint32_t hdl);
int32_t btsdk_agap_network_event(uint32_t hdl, uint8_t ev, void *param);
int32_t btsdk_agap_current_call_rsp(uint32_t hdl, pbtsdk_hfp_clcc_info_stru call_info, uint8_t complete);
int32_t btsdk_agap_set_spk_vol(uint32_t hdl, uint8_t spk_vol);
int32_t btsdk_agap_set_mic_vol(uint32_t hdl, uint8_t mic_vol);
int32_t btsdk_agap_subscriber_number_rsp(uint32_t hdl, pbtsdk_hfp_phone_info_stru phone_info, uint8_t complete);
int32_t btsdk_agap_send_battery_charge_indicator(uint32_t hdl, uint8_t indicator);
int32_t btsdk_agap_send_error_message(uint32_t hdl, uint8_t err_code);
int32_t btsdk_agap_network_operator_rsp(uint32_t hdl, pbtsdk_hfp_cops_info_stru op_info);
int32_t btsdk_agap_set_cur_indicator_val(uint32_t hdl, pbtsdk_hfp_cind_info_stru indicators);
int32_t btsdk_agap_app_reg_cbk(btsdk_hfp_callback *pfunc);
int32_t btsdk_agap_originate_call(uint32_t hdl, uint8_t mode);
int32_t hfp_hf_accept_or_cancel_sco_req(uint32_t hdl, uint8_t is_accept);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */
#endif /* STATCK_HFP_IF_H */
