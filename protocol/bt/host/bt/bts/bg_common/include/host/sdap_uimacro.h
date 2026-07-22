/*
 * Copyright (c) @CompanyNameMagicTag 1999-2015. All rights reserved.
 */

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
Module Name:
    sdap_uimacro.h
Abstract:
    Export UI constants.
------------------------------------------------------------------------------------------------- */
#ifndef BT_SDAP_UIMACRO_H
#define BT_SDAP_UIMACRO_H

/* +++++++++++++++++++++ SDAP MACRO Definition +++++++++++++++++++++ */
#if (defined(CONFIG_PHONE_TL) || defined(CONFIG_PHONE_GW_TL))
#define CONFIG_CTP_CLIENT
#endif

#if (defined(CONFIG_PHONE_GW) || defined(CONFIG_PHONE_GW_TL))
#define CONFIG_TCS_GW
#endif

#ifndef CONFIG_SDAP_MAX_BYTES_COUNT
#define CONFIG_SDAP_MAX_BYTES_COUNT        300
#endif
#define MAX_SVC_INFO_STRU_LEN        100

/* SDP Transaction type - For compatibility */
#define TRANS_SS                    0x00000001
#define TRANS_SA                    0x00000002
#define TRANS_SSA                    0x00000003

/* Mask for stop rule - For compatibility */
#define STOP_MAXCOUNT                0x00000010
#define STOP_DURATION                0x00000020


/* Data Element */
#define DEMODE_NULL            0
#define DEMODE_UINT            1
#define DEMODE_INT            2
#define DEMODE_UUID            3
#define DEMODE_STRING        4
#define DEMODE_BOOL            5
#define DEMODE_DESEQ        6
#define DEMODE_DEALT        7
#define DEMODE_URL            8

#define DEMODE_1_BYTE        0
#define DEMODE_2_BYTES        1
#define DEMODE_4_BYTES        2
#define DEMODE_8_BYTES        3
#define DEMODE_16_BYTES        4
#define DEMODE_ADD_8BITS    5
#define DEMODE_ADD_16BITS    6
#define DEMODE_ADD_32BITS    7

#define demode_desc(type_, size_)        ((uint8_t)(((type_) << 3) | (size_)))

/* Service attribute ID */
#define SRV_RECORD_HANDLE            0x0000
#define SRV_CLSID_LIST                0x0001
#define SRV_RECORD_STATE            0x0002
#define SRV_ID                        0x0003
#define PROT_DESCRIPTION_LIST        0x0004
#define BROWSE_GROUP_LIST            0x0005
#define LANG_BASE_ATTR_ID_LIST        0x0006
#define SRV_INFO_TIME_TO_LIVE        0x0007
#define SRV_AVAILABILITY            0x0008
#define PROF_DESCRIPTION_LIST        0x0009
#define DOC_URL                        0x000A
#define CLIENT_EXEC_URL                0x000B
#define ICON_URL                    0x000C
#define ADDITIONAL_PROT_DESC_LIST    0x000D
#define OFFSET_SRV_NAME                0x0000
#define OFFSET_SRV_DESCRIPTION        0x0001
#define OFFSET_PROVIDER_NAME        0x0002
#define VER_NUM_LIST                0x0200
#define SRV_DB_STATE                0x0201
#define GROUP_ID                    0x0200
#define REM_AUDIO_VOL_CTRL            0x0302
#define EXT_NETWORK                    0x0301
#define SRV_VERSION                    0x0300
#define SUP_DATA_STORE_LST            0x0301
#define FAX_CLS_1                    0x0302
#define FAX_CLS_2_0                    0x0303
#define FAX_CLS_2                    0x0304
#define AUDIO_FEEDBACK                0x0305
#define SUP_FMT_LST                    0x0303
#define SECURITY_DESCRIPTION        0x030A
#define NET_ACCESS_TYPE                0x030B
#define MAX_NET_ACCESS_RATE            0x030C
#define IP_SUBNET                    0x0200
#define HCRP_1284ID                    0x0300
#define HCRP_DEVICE_NAME            0x0302
#define HCRP_FRIENDLY_NAME            0x0304
#define HF_AG_NETWORK                0x0301
#define HF_SUP_FEATURES                0x0311
#define BIP_SUP_CAPABILITIES        0x0310
#define BIP_SUP_FEATURES            0x0311
#define BIP_SUP_FUNCTIONS            0x0312
#define BIP_TOTAL_DATA_CAPACITY        0x0313
#define PBAP_SUP_REPOSITORIES        0x0314
#define HID_DEV_RELEASE_NUM            0x0200
#define HID_PARSER_VER                0x0201
#define HID_DEV_SUBCLS                0x0202
#define HID_COUNTRY_CODE            0x0203
#define HID_VIRTUAL_CABLE            0x0204
#define HID_RECONNECT_INIT            0x0205
#define HID_DESCRIPTOR_LIST            0x0206
#define HID_LANGID_BASE_LIST        0x0207
#define HID_SDP_DISABLE                0x0208
#define HID_BATTERY_POWER            0x0209
#define HID_REMOTE_WAKE                0x020A
#define HID_PROFILE_VERSION            0x020B
#define HID_SUPERVISION_TO            0x020C
#define HID_NORM_CONNECTABLE        0x020D
#define HID_BOOT_DEVICE                0x020E
#define DI_SPECIFICATION_ID            0x0200
#define DI_VENDOR_ID                0x0201
#define DI_PRODUCT_ID                0x0202
#define DI_VERSION                    0x0203
#define DI_PRIMARY_RECORD            0x0204
#define DI_VENDORID_SOURCE            0x0205
#define A2DP_SUP_FEATURES            0x311
#define AVRCP_SUP_FEATURES            0x311
#define BPP_SUP_DOC_FMTS            0x350
#define BPP_SUP_CHAR_REPERTOIRES    0x352
#define BPP_SUP_XHTML_IMG_FMTS        0x354
#define BPP_COLOR_SUP                0x356
#define BPP_1284ID                    0x358
#define BPP_PRINTER_NAME            0x35A
#define BPP_PRINTER_LOCATION        0x35C
#define BPP_DUPLEX_SUP                0x35E
#define BPP_SUP_MEDIA_TYPES            0x360
#define BPP_MAX_MEDIA_WIDTH            0x362
#define BPP_MAX_MEDIA_LENGTH        0x364
#define BPP_ENHANCED_LAYOUT_SUP        0x366
#define BPP_SUP_RUI_FMTS            0x368
#define BPP_PBR_RUI_SUP                0x370
#define BPP_DPS_RUI_SUP                0x372
#define BPP_PBR_TOP_URL                0x374
#define BPP_DPS_TOP_URL                0x376
#define BPP_ADMIN_RUI_TOP_URL        0x378
#define BPP_DEVICE_NAME                0x37A
#define HDP_SUP_FEATURES_LIST       0x200
#define HDP_DATA_EXCHG_SPEC         0x301
#define HDP_MCAP_SUP_PROCEDURES     0x302
/* Temp test for GOEP2.0 PSM attribute, it shall be modified later when the final value is defined by the SIG. */
#define GOEP_PSM                    0x200
#define MAP_MAS_INSTANCE_ID         0x315
#define MAP_SUP_MSG_TYPES            0x316
#define PBAP_SUP_FEATURES            0x317
#define MAP_SUP_FEATURES            0x317

/* Service class UUID */
#define CLS_SERVER_SERVICE            0x1000
#define CLS_BROWSE_GROUP            0x1001
#define CLS_PUBLIC_BROWSE_GROUP        0x1002
#define CLS_SERIAL_PORT                0x1101
#define CLS_LAN_ACCESS                0x1102
#define CLS_DIALUP_NET                0x1103
#define CLS_IRMC_SYNC                0x1104
#define CLS_OBEX_OBJ_PUSH            0x1105
#define CLS_OBEX_FILE_TRANS            0x1106
#define CLS_IRMC_SYNC_CMD            0x1107
#define CLS_HEADSET                    0x1108
#define CLS_CORDLESS_TELE            0x1109
#define CLS_AUDIO_SOURCE            0x110A
#define CLS_AUDIO_SINK                0x110B
#define CLS_AVRCP_TG                0x110C
#define CLS_ADV_AUDIO_DISTRIB        0x110D
#define CLS_AVRCP_CT                0x110E
#define CLS_AVRCP_CT_CT                0x110F
#define CLS_INTERCOM                0x1110
#define CLS_FAX                        0x1111
#define CLS_HEADSET_AG                0x1112
#define CLS_WAP                        0x1113
#define CLS_WAP_CLIENT                0x1114
#define CLS_PAN_PANU                0x1115
#define CLS_PAN_NAP                    0x1116
#define CLS_PAN_GN                    0x1117
#define CLS_DIRECT_PRINT            0x1118
#define CLS_REF_PRINT                0x1119
#define CLS_IMAGING                    0x111A
#define CLS_IMAG_RESPONDER            0x111B
#define CLS_IMAG_AUTO_ARCH            0x111C
#define CLS_IMAG_REF_OBJ            0x111D
#define CLS_HANDSFREE                0x111E
#define CLS_HANDSFREE_AG            0x111F
#define CLS_DPS_REF_OBJ                0x1120
#define CLS_REFLECTED_UI            0x1121
#define CLS_BASIC_PRINT                0x1122
#define CLS_PRINT_STATUS            0x1123
#define CLS_HID                        0x1124
#define CLS_HCRP                    0x1125
#define CLS_HCR_PRINT                0x1126
#define CLS_HCR_SCAN                0x1127
#define CLS_SIM_ACCESS                0x112D
#define CLS_PBAP_PCE                0x112E
#define CLS_PBAP_PSE                0x112F
#define CLS_PBAP                    0x1130
#define CLS_MAP_MAS                    0x1132
#define CLS_MAP_MNS                    0x1133
#define CLS_MAP                        0x1134
#define CLS_PNP_INFO                0x1200
#define CLS_GENERIC_NET                0x1201
#define CLS_GENERIC_FILE_TRANS        0x1202
#define CLS_GENERIC_AUDIO            0x1203
#define CLS_GENERIC_TELE            0x1204
#define CLS_VIDEO_SOURCE            0x1303
#define CLS_VIDEO_SINK                0x1304
#define CLS_VIDEO_DISTRIBUTION        0x1305
#define CLS_HDP                     0x1400
#define CLS_HDP_SOURCE              0x1401
#define CLS_HDP_SINK                0x1402
#define CLS_GENERIC_ATT             0x1801
#define CLS_MAX_GATT_SERVICE_TYPE   0x18FF

/* Protocol UUID */
#define PROTOCOL_SDP                0x0001
#define PROTOCOL_UDP                0x0002
#define PROTOCOL_RFCOMM                0x0003
#define PROTOCOL_TCP                0x0004
#define PROTOCOL_TCSBIN                0x0005
#define PROTOCOL_TCS_AT                0x0006
#define PROTOCOL_ATT                0x0007
#define PROTOCOL_OBEX                0x0008
#define PROTOCOL_IP                    0x0009
#define PROTOCOL_FTP                0x000A
#define PROTOCOL_HTTP                0x000C
#define PROTOCOL_WSP                0x000E
#define PROTOCOL_BNEP                0x000F
#define PROTOCOL_HIDP                0x0011
#define PROTOCOL_HCR_CONTROL        0x0012
#define PROTOCOL_HIDP_INTERRUPT        0x0013
#define PROTOCOL_HCR_DATA            0x0014
#define PROTOCOL_HCR_NOTIFY            0x0016
#define PROTOCOL_AVCTP                0x0017
#define PROTOCOL_AVDTP                0x0019
#define PROTOCOL_AVCTP_BROWSING     0x001B
#define PROTOCOL_MCAP_CTRL          0x001E
#define PROTOCOL_MCAP_DATA          0x001F
#define PROTOCOL_L2CAP                0x0100

/* Protocol PSM */
#define SDP_PSM_TCSBIN_LESS            0x0007
#define SDP_PSM_ATT                    0x001F
#define SDP_PSM_3DSP                   0x0021


/* default values */
#define GRP_ROOT_ID                    0x1002

#define LANGUAGE_ENGLISH            0x656E
#define ENCODING_UFT8                0x006A
#define LANG_BASE_PRI                0x0100

#define PROFILE_VERSION                0x0100

/* Hands-Free AG network attribute values */
#define HF_GSM_LIKE_NET                0x01
#define HF_OTHER_NET                0x02

/* PAN Supported Network Packet Type */
#define PAN_IP_V4                    0x0800
#define PAN_IPV4                    0x0800
#define PAN_ARP                        0x0806

/* PAN Security Description */
#define PAN_SECU_NONE                0x0000
#define PAN_SECU_SERVICE            0x0001
#define PAN_SECU_8021X                0x0002

/* PAN NetAccess Type */
#define PAN_NET_PSTN                0x0000
#define PAN_NET_ISDN                0x0001
#define PAN_NET_DSL                    0x0002
#define PAN_NET_CABLEM                0x0003
#define PAN_NET_10ETH                0x0004
#define PAN_NET_100ETH                0x0005
#define PAN_NET_4TOKENR                0x0006
#define PAN_NET_16TOKENR            0x0007
#define PAN_NET_100TOKENR            0x0008
#define PAN_NET_FDDI                0x0009
#define PAN_NET_GMS                    0x000A
#define PAN_NET_CDMA                0x000B
#define PAN_NET_GRPS                0x000C
#define PAN_NET_3GCELL                0x000D
#define PAN_NET_OTHER                0xFFFE

/* OPP Supported Formats */
#define OPP_FMT_VCARD21            0x01
#define OPP_FMT_VCARD30            0x02
#define OPP_FMT_VCAL10            0x03
#define OPP_FMT_ICAL20            0x04
#define OPP_FMT_VNOTE            0x05
#define OPP_FMT_VMESSAGE        0x06
#define OPP_FMT_ANYTYPE            0xFF

/* SYNC Supported Data Stores */
#define SYNC_STORE_PHONEBOOK    0x01
#define SYNC_STORE_CALENDAR        0x03
#define SYNC_STORE_NOTES        0x05
#define SYNC_STORE_MESSAGES        0x06

/* AVRCP Supported Categories/Features */
#define AVCT_CATEGORY_1            0x0001
#define AVCT_CATEGORY_2            0x0002
#define AVCT_CATEGORY_3            0x0004
#define AVCT_CATEGORY_4            0x0008
#define AVCT_FEATURE_BROWSING   0x0040
#define AVCT_FEATURE_CA_GET_IMAGE_PROPERTIES  0x0080
#define AVCT_FEATURE_CA_GET_IMAGE  0x0100
#define AVCT_FEATURE_CA_GET_THUMBNAIL  0x0200

#define AVTG_CATEGORY_1            0x0001
#define AVTG_CATEGORY_2            0x0002
#define AVTG_CATEGORY_3            0x0004
#define AVTG_CATEGORY_4            0x0008
#define AVTG_FEATURE_PLAYER_SET 0x0010
#define AVTG_FEATURE_GROUP_NAVI 0x0020
#define AVTG_FEATURE_BROWSING   0x0040
#define AVTG_FEATURE_MULTI_MEDIA 0x0080
#define AVTG_FEATURE_COVER_ART    0x0100

/* "SupportedFeatures" attribute bit mapping for the HF */
#define HF_ECNRFUNC_SUP                    0x0001
#define HF_CALLWAIT_3WAYCALL_SUP        0x0002
#define HF_CLIPRESENTCAP_SUP            0x0004
#define HF_VOICERECOG_SUP                0x0008
#define HF_RMVOLCTRL_SUP                0x0010

/* "SupportedFeatures" attribute bit mapping for the HF AG */
#define HFAG_3WAYCALL_SUP                0x0001
#define HFAG_ECNRFUNC_SUP                0x0002
#define HFAG_VOICERECOG_SUP                0x0004
#define HFAG_INBANDRIGNTONE_SUP            0x0008
#define HFAG_ATTACHNUMTOTAG_SUP            0x0010

/* "SupportedFeatures" attribute bit mapping for the A2DP SRC */
#define A2DPSRC_PLAYER                0x0001
#define A2DPSRC_MICROPHONE            0x0002
#define A2DPSRC_TUNER                0x0004
#define A2DPSRC_MIXER                0x0008

/* "SupportedFeatures" attribute bit mapping for the A2DP SNK */
#define A2DPSNK_HEADPHONE            0x0001
#define A2DPSNK_SPEAKER                0x0002
#define A2DPSNK_RECORDER            0x0004
#define A2DPSNK_AMPLIFIER            0x0008

/* HID mandatory Bool type attribute bit mask */
#define HID_MASK_VCABLE_TRUE        0x0001
#define HID_MASK_RECONNINIT_TRUE    0x0002
#define HID_MASK_BOOTDEVICE_TRUE    0x0004

/* DI mandatory Bool type attribute bit mask */
#define DI_MASK_PRIMARY_TRUE        0x0004

/* "Supported capabilities" attribute bit mapping for the Imaging service record */
#define IMG_CAP_GENERIC                0x01
#define IMG_CAP_CAPTURING            0x02
#define IMG_CAP_PRINTING            0x04
#define IMG_CAP_DISPLAYING            0x08

/* "Supported features" attribute bit mapping for the Imaging service record */
#define IMG_IMAGEPUSH                0x0001
#define IMG_IMAGEPUSH_STORE            0x0002
#define IMG_IMAGEPUSH_PRINT            0x0004
#define IMG_IMAGEPUSH_DISPLAY        0x0008
#define IMG_IMAGEPULL                0x0010
#define IMG_ADVIMG_PRINTING            0x0020
#define IMG_AUTO_ARCHIVE            0x0040
#define IMG_REMOTE_CAMERA            0x0080
#define IMG_REMOTE_DISPLAY            0x0100

/* "Supported functions" attribute bit mapping for the Imaging service record */
#define IMG_FUNC_GETCAPABILITIES    0x00000001
#define IMG_FUNC_PUTIMAGE            0x00000002
#define IMG_FUNC_PUTLINKATTACH        0x00000004
#define IMG_FUNC_PUTLINKTHUMB        0x00000008
#define IMG_FUNC_REMOTEDISPLAY        0x00000010
#define IMG_FUNC_GETIMAGESLIST        0x00000020
#define IMG_FUNC_GETIMAGEPROP        0x00000040
#define IMG_FUNC_GETIMAGE            0x00000080
#define IMG_FUNC_GETLINKTHUMB        0x00000100
#define IMG_FUNC_GETLINKATTACH        0x00000200
#define IMG_FUNC_DELETEIMAGE        0x00000400
#define IMG_FUNC_STARTPRINT            0x00000800
#define IMG_FUNC_GETPARTIALIMG        0x00001000
#define IMG_FUNC_STARTARCHIVE        0x00002000
#define IMG_FUNC_GETMONITORIMG        0x00004000
#define IMG_FUNC_GETSTATUS            0x00010000

/* "External Network" attribute bit mapping for the CTP GW service record */
#define CTP_EXTNW_PSTN                0x01
#define CTP_EXTNW_IDSN                0x02
#define CTP_EXTNW_GSM                0x03
#define CTP_EXTNW_CDMA                0x04
#define CTP_EXTNW_ANALCELL            0x05
#define CTP_EXTNW_PACKSW            0x06
#define CTP_EXTNW_OTHER                0x07

/* HID Language ID value */
#define LAGNID_USA_ENG                0x0309

/* PBAP SupportedRepositories attribute bit mask */
#define PBAP_SR_PHONEBOOK           0x01
#define PBAP_SR_SIMCARD             0x02
#define PBAP_SR_SPEEDDIAL           0x04
#define PBAP_SR_FAVORITES           0x08

/* HDP "MCAPSupportedProcedures" attribute bit mask */
#define HDP_MCAP_SUP_RECONN_INIT    0x02
#define HDP_MCAP_SUP_RECONN_ACPT    0x04
#define HDP_MCAP_SUP_CLOCK_SYNC     0x08
#define HDP_MCAP_SYNC_MASTER        0x10

/* HDP "MDEP Data Type" value */
#define HDP_MDEP_DATATYPE_PULS_OXIM     4100
#define HDP_MDEP_DATATYPE_BP            4103
#define HDP_MDEP_DATATYPE_TEMP          4104
#define HDP_MDEP_DATATYPE_SCALE         4111
#define HDP_MDEP_DATATYPE_GLUCOSE       4113
#define HDP_MDEP_DATATYPE_HFCARDIO      4137
#define HDP_MDEP_DATATYPE_HFSTRENGTH    4138
#define HDP_MDEP_DATATYPE_AIACTHUB      4167
#define HDP_MDEP_DATATYPE_AIMEDMINDER   4168

/* MAP "SupportedMessageTypes" value */
#define MAP_MSGTYPE_EMAIL               0x01
#define MAP_MSGTYPE_SMS_GSM             0x02
#define MAP_MSGTYPE_SMS_CDMA            0x04
#define MAP_MSGTYPE_MMS                 0x08
#define MAP_MSGTYPE_IM                     0x10

/* PBAP "PbapSupportedFeatures" value */
#define PBAP_FEATURE_DOWNLOAD                       0x00000001
#define PBAP_FEATURE_BROWSING                       0x00000002
#define PBAP_FEATURE_DATABASE_ID                    0x00000004
#define PBAP_FEATURE_FOLDER_VER_COUNTER             0x00000008
#define PBAP_FEATURE_VCARD_SELECTING                0x00000010
#define PBAP_FEATURE_ENHANCED_MISSED_CALLS            0x00000020
#define PBAP_FEATURE_XBTUCI_PROPERTY                0x00000040
#define PBAP_FEATURE_XBTUID_PROPERTY                0x00000080
#define PBAP_FEATURE_CONTACT_REFERENCING            0x00000100
#define PBAP_FEATURE_DEFAULT_CONTACT_IMAGE_FORMAT    0x00000200

/* MAP "MapSupportedFeatures" value */
#define MAP_FEATURE_NOTIFICATION_REGISTRATION        0x00000001
#define MAP_FEATURE_NOTIFICATION                       0x00000002
#define MAP_FEATURE_BROWSING                        0x00000004
#define MAP_FEATURE_UPLOADING                         0x00000008
#define MAP_FEATURE_DELETE                            0x00000010
#define MAP_FEATURE_INSTANCE_INFORMATION            0x00000020
#define MAP_FEATURE_EXTENDED_EVENT_REPORT_V1_1        0x00000040
#define MAP_FEATURE_EVENT_REPORT_V1_2                0x00000080
#define MAP_FEATURE_MESSAGE_FORMAT_V1_1                0x00000100
#define MAP_FEATURE_MESSAGE_LISTING_FORMAT_V1_1        0x00000200
#define MAP_FEATURE_PERSISTENT_MESSAGE_HANDLES        0x00000400
#define MAP_FEATURE_DATABASE_ID                        0x00000800
#define MAP_FEATURE_FOLDER_VERSION_COUNTER            0x00001000
#define MAP_FEATURE_CONVERSATION_VERSION_COUNTERS    0x00002000
#define MAP_FEATURE_PARTICIPANT_PRESENCE_CHANGE        0x00004000
#define MAP_FEATURE_PARTICIPANT_CHAT_STATE_CHANGE    0x00008000
#define MAP_FEATURE_PBAP_CONTACT_CROSS_REFERENCE    0x00010000
#define MAP_FEATURE_NOTIFICATION_FILTERING            0x00020000
#define MAP_FEATURE_UTC_OFFSET_TIMESTAMP_FORMAT        0x00040000
#define MAP_FEATURE_IN_CONNECT_REQUEST                0x00080000    /* MSE only */
#define MAP_FEATURE_CONVERSATION_LISTIN                0x00100000
#define MAP_FEATURE_OWNER_STATUS                    0x00200000
#define MAP_FEATURE_MESSAGE_FORWARDING                0x00400000

/* parameter values */
#define SD_STOP_DURATION            0x1000

#define SD_UUID_16                    0x0001
#define SD_UUID_32                    0x0002
#define SD_UUID_128                    0x0004
#define SD_UUID_LAST                0x000F

#define SD_ATTRID_SINGLE            0x0001
#define SD_ATTRID_RANGE                0x0002
#define SD_ATTRID_TWO                0x0003

/* optional attribute masks */
#define OPTATTR_MASK_EXTNW            0x0001

#define OPTATTR_MASK_RMVOLCTRL        0x0001

#define OPTATTR_MASK_FEATURES        0x0001

#define OPTATTR_MASK_AUFBSUP        0x0001

#define OPTATTR_MASK_SUPFAXCLS1        0x0002
#define OPTATTR_MASK_SUPFAXCLS20    0x0004
#define OPTATTR_MASK_SUPFAXCLS2        0x0008

#define OPTATTR_MASK_SRVID            0x0001
#define OPTATTR_MASK_1284ID            0x0002

#define OPTATTR_MASK_SDPDISABLE        0x0008
#define OPTATTR_MASK_BATTERYPWR     0x0010
#define OPTATTR_MASK_RMWAKE            0x0020
#define OPTATTR_MASK_REPORTLIST        0x0040
#define OPTATTR_MASK_SUPERTO        0x0080
#define OPTATTR_MASK_NORMCONN        0x0100

#define OPTATTR_MASK_DICLIURL        0x0001
#define OPTATTR_MASK_DIDOCURL        0x0002

#define OPTATTR_MASK_BPP_PBRSUP                    0x0100
#define OPTATTR_MASK_BPP_COLOR                    0x0008
#define OPTATTR_MASK_BPP_DUPLEX                       0x0010
#define OPTATTR_MASK_BPP_ENHANCEDLAYOUT            0x0020
#define OPTATTR_MASK_BPP_PBRRUI                    0x0040
#define OPTATTR_MASK_BPP_DPSRUI                    0x0080

/* Error code */
#ifndef PROT_SDP_BASE
#define PROT_SDP                            3
#define MAX_DEFINE_PER_PROTOCOL                100
#define PROT_SDP_BASE                         ((PROT_SDP-1)*MAX_DEFINE_PER_PROTOCOL)
#endif
#define ERROR_SDAP_SUCCESS                  0
#define UNKNOWN_ERROR_SDAP                    (PROT_SDP_BASE + 10)                        /* 210 */
#define ERROR_SDAP_INVALID_VERSION                    (UNKNOWN_ERROR_SDAP + 0x0001)
#define ERROR_SDAP_INVALID_SERVICEHANDLE            (UNKNOWN_ERROR_SDAP + 0x0002)
#define ERROR_SDAP_INVALID_REQUEST_SYNTAX            (UNKNOWN_ERROR_SDAP + 0x0003)
#define    ERROR_SDAP_INVALID_PDUSIZE                    (UNKNOWN_ERROR_SDAP + 0x0004)
#define ERROR_SDAP_INVALID_CONTINUATIONSTATE         (UNKNOWN_ERROR_SDAP + 0x0005)
#define ERROR_SDAP_INSUFFICIENT_RESOURCE            (UNKNOWN_ERROR_SDAP + 0x0006)

#define    ERROR_SDAP_OPEN_PENDING                (UNKNOWN_ERROR_SDAP + 7)
#define ERROR_SDAP_INVALID_TRANSHANDLE        (UNKNOWN_ERROR_SDAP + 8)
#define    ERROR_SDAP_CONFIG_FAILURE            (UNKNOWN_ERROR_SDAP + 9)
#define ERROR_SDAP_TIMER_EXPIRED            (UNKNOWN_ERROR_SDAP + 10)
#define ERROR_SDAP_EXCEEDING_MTU            (UNKNOWN_ERROR_SDAP + 11)
#define ERROR_SDAP_INVALID_UUID                (UNKNOWN_ERROR_SDAP + 12)
#define ERROR_SDAP_NO_MORE_SERVICE            (UNKNOWN_ERROR_SDAP + 13)
#define ERROR_SDAP_INVALID_RESPONSE            (UNKNOWN_ERROR_SDAP + 14)
#define ERROR_SDAP_INVALID_OPERATION        (UNKNOWN_ERROR_SDAP + 15)
#define ERROR_SDAP_FOUND_NO_SERVICE            (UNKNOWN_ERROR_SDAP + 16)
#define ERROR_SDAP_SERVICE_NOTSUPPORT        (UNKNOWN_ERROR_SDAP + 17)
#define ERROR_SDAP_SERVICE_UNAVAILABLE        (UNKNOWN_ERROR_SDAP + 18)
#define ERROR_SDAP_API_UNAVAILABLE            (UNKNOWN_ERROR_SDAP + 19)
#define ERROR_SDAP_BUFFER_TOO_SMALL            (UNKNOWN_ERROR_SDAP + 20)
#define ERROR_SDAP_L2CAP_DISCONNECT            (UNKNOWN_ERROR_SDAP + 21)    /* SDP_ERR_L2CAP_DISCONNECT */
#define ERROR_SDAP_LOCAL_CANCELD            (UNKNOWN_ERROR_SDAP + 22)    /* SDP_ERR_CANCELED */


#endif
