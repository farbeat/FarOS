/*
 * Copyright (c) @CompanyNameMagicTag 1999-2015. All rights reserved.
 */

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
Module Name:
    sdap_uistru.h
Abstract:
    Export UI data structures.
------------------------------------------------------------------------------------------------- */
#ifndef BT_SDAP_UISTRU_H
#define BT_SDAP_UISTRU_H

#include "sysdep.h"

/* ++++++++++++++++++++ SDAP UI Structure Definition +++++++++++++++++++ */
#define SDAP_UUID_DATE4_MAXLEN  8
struct sdap_uuid_stru {
    DWORD data1;
    uint16_t data2;
    uint16_t data3;
    uint8_t data4[SDAP_UUID_DATE4_MAXLEN];
};

struct sdap_u_int64stru {
    DWORD    higher_4bytes;
    DWORD    lower_4bytes;
};

struct sdap_data_ele_stru {
    DWORD    size;
    uint8_t    descriptor;
    uint8_t    data[1];
};

struct sdap_svc_attr_stru {
    uint16_t    attr_id;
    uint16_t    align_byte;
    struct sdap_data_ele_stru    attr_val;
};

struct sdap_browse_info_stru {
    uint16_t                trans_hdl;                /* transaction handle representing the browsing transaction */
    uint16_t                mask;                    /* flag used to identify the type of group_id, */
    struct uuid_stru    group_id;            /* any UUID value used to identify the browse group
                                               that is, the size of the UUID value */
    uint16_t                duration;                /* duration of the ServiceSearch transaction */
};

struct sdap_svc_attr_id_stru {
    uint8_t                mask;
    uint8_t                reserved;
    uint16_t                id;
    uint16_t                end_id;
};

struct sdap_search_info_stru {
    uint16_t                trans_hdl;
    uint16_t                mask;
    struct uuid_stru    pattern;
    uint16_t                duration;
    uint16_t                id_count;
    struct sdap_svc_attr_id_stru    attr_id_list[1];
};

/* For compatibility */
struct sdp_uuid {
    uint8_t    var[16]; /* 16bit & 32bit UUIDs stores in the first four bytes. All in system Endian. */
    uint8_t    mask;    /* SD_UUID_16, SD_UUID_32, SD_UUID_128 */
};

struct sdap_service_attribute_id {
    uint8_t    mask;        /* SD_ATTRID_RANGE, SD_ATTRID_SINGLE, SD_ATTRID_TWO */
    uint8_t    reserved;
    uint16_t    id;
    uint16_t    end_id;
};
#define SERVICE_ATTRIBUTE_ID    sdap_service_attribute_id    /* For compatibility */

struct sdap_search_req_stru {
    uint16_t trans_handle;
    uint16_t mask;
    /* Stop Rule */
    uint16_t    max_count;
    uint16_t    duration;
    /* Input Parameters */
    DWORD    handle;
    uint16_t    pattern_count;
    uint16_t    id_count;
    struct sdp_uuid    *service_search_pattern;
    struct sdap_service_attribute_id    *attribute_id_list;
};
#define SDP_SEARCH_REQ_STRU    sdap_search_req_stru    /* For compatibility */

struct sdap_search_cfm_stru {
    DWORD count;
    uint8_t *var;
};
#define SDP_SEARCH_CFM_STRU    sdap_search_cfm_stru    /* For compatibility */

/* ++++++++++++++ Service Information Structure +++++++++++++++ */
struct sdap_general_info_stru {
    /* sizeof sdap_general_info_stru, include additional bytes allocated dynamically */
    uint16_t           size;
    uint16_t            reserved1;            /* reserved for alignment */
    DWORD            svc_hdl;            /* service handle */
    uint8_t            reserved2[1];        /* reserved for alignment */
};

struct sdap_ctp_info_stru {
    uint16_t             size;                /* sizeof sdap_ctp_info_stru */
    uint16_t             mask;                /* optional attribute mask */
    DWORD             svc_hdl;            /* service handle */
    uint8_t            ext_network;        /* extern network */
};

struct sdap_icp_info_stru {
    uint16_t             size;                /* sizeof sdap_icp_info_stru */
    uint16_t             reserved;            /* reserved for alignment */
    DWORD             svc_hdl;            /* service handle */
};

struct sdap_spp_info_stru {
    uint16_t             size;                /* sizeof sdap_spp_info_stru */
    uint16_t            reserved;            /* reserved for alignment */
    DWORD            svc_hdl;            /* service handle */
    uint8_t            svr_chnl;            /* service channel */
};

struct sdap_hephs_info_stru {
    uint16_t             size;                /* sizeof sdap_hephs_info_stru */
    uint16_t            mask;                /* optional attribute mask */
    DWORD            svc_hdl;            /* service handle */
    uint8_t            svr_chnl;            /* service channel */
};

struct sdap_hepag_info_stru {
    uint16_t             size;                /* sizeof sdap_hepag_info_stru */
    uint16_t            reserved;            /* reserved for alignment */
    DWORD            svc_hdl;            /* service handle */
    uint8_t            svr_chnl;            /* service channel */
};

struct sdap_hfphf_info_stru {
    uint16_t            size;                /* sizeof sdap_hfphf_info_stru */
    uint16_t            mask;                /* optional attribute mask */
    DWORD            svc_hdl;            /* service handle */
    uint8_t            svr_chnl;            /* service channel */
    uint8_t            reserved;            /* reserved for alignment */
    uint16_t            features;            /* supported features */
    uint16_t            profile_ver;        /* HFP HF Profile version */
};

struct sdap_hfpag_info_stru {
    uint16_t            size;                /* sizeof sdap_hfpag_info_stru */
    uint16_t            mask;                /* optional attribute mask */
    DWORD            svc_hdl;            /* service handle */
    uint8_t            svr_chnl;            /* service channel */
    uint8_t            ag_network;            /* network attribute */
    uint16_t            features;            /* supported features */
    uint16_t            profile_ver;        /* HFP AG Profile version */
};

struct sdap_dun_info_stru {
    uint16_t            size;                /* sizeof sdap_dun_info_stru */
    uint16_t            mask;                /* optional attribute mask */
    DWORD            svc_hdl;            /* service handle */
    uint8_t            svr_chnl;            /* service channel */
};

struct sdap_fax_info_stru {
    uint16_t             size;                /* sizeof sdap_fax_info_stru */
    uint16_t            mask;                /* optional attribute mask */
    DWORD            svc_hdl;            /* service handle */
    uint8_t             svr_chnl;            /* service channel */
};

struct sdap_lap_info_stru {
    uint16_t            size;                /* sizeof sdap_lap_info_stru */
    uint16_t            reserved;            /* reserved for alignment */
    DWORD            svc_hdl;            /* service handle */
    uint8_t            svr_chnl;            /* service channel */
};

struct sdap_opp_info_stru {
    uint16_t             size;                /* sizeof sdap_opp_info_stru, include additional bytes for fmt_list */
    uint16_t            reserved;            /* reserved for alignment */
    DWORD            svc_hdl;            /* service handle */
    uint16_t            psm;                /* PSM of OBEX over L2CAP */
    uint8_t            svr_chnl;            /* service channel */
    uint8_t            fmt_num;            /* number of supported formats */
    uint8_t            fmt_list[1];        /* list of supported formats */
};

struct sdap_ftp_info_stru {
    uint16_t            size;                /* sizeof sdap_ftp_info_stru */
    uint16_t            reserved;            /* reserved for alignment */
    DWORD            svc_hdl;            /* service handle */
    uint16_t            psm;                /* PSM of OBEX over L2CAP */
    uint8_t            svr_chnl;            /* server channel */
};

struct sdap_sync_info_stru {
    uint16_t            size;                /* sizeof sdap_sync_info_stru, include additional bytes for stores_list */
    uint16_t             reserved;            /* reserved for alignment */
    DWORD            svc_hdl;            /* service handle */
    uint8_t            svr_chnl;            /* server channel */
    uint8_t            stores_num;            /* number of supported data stores */
    uint8_t             stores_list[1];        /* list of supported data stores */
};

struct sdap_sync_cmd_info_stru {
    uint16_t            size;                /* sizeof sdap_sync_cmd_info_stru. */
    uint16_t             reserved;            /* reserved for alignment */
    DWORD            svc_hdl;            /* service handle */
    uint8_t            svr_chnl;            /* server channel */
};

struct sdap_pan_info_stru {
    uint16_t            size;                /* sizeof SDAP_NAP_INFO_STRU, include additional bytes for type_list */
    uint16_t             reserved;            /* reserved for alignment */
    DWORD            svc_hdl;            /* service handle */
    uint16_t            svc_type;            /* type of PAN service in UUID16, it can be GN, NAP or PANU */
    uint16_t            psm;                /* PSM value in ProtocolDescriptorList attribute */
    uint16_t             secu_desc;            /* security description */
    uint16_t             net_access_type;    /* net access type, only valid for NAP service */
    DWORD            max_access_rate;    /* maximum possible network access data rate, only valid for NAP service */
    uint16_t            type_num;            /* number of supported network packet type */
    uint16_t            type_list[1];        /* list of supported network packet type */
};

struct sdap_avrcp_info_stru {
    uint16_t            size;                /* size of SDAP_AVCT_INFO_STRU */
    uint16_t            reserved;            /* reserved for alignment */
    DWORD            svc_hdl;            /* service handle */
    uint16_t            profile_ver;        /* ProfileVersion attribute value */
    uint16_t            svc_type;            /* type of AVRCP service, it can be CT or TG */
    uint16_t            sup_cg;                /* Supported Categories */
    /* The dynamically assigned L2CAP PSM on which Cover Art service is available */
    uint16_t            psm;
};

struct sdap_a2dp_info_stru {
    uint16_t            size;                /* size of sdap_a2dp_info_stru */
    uint16_t            reserved;            /* reserved for alignment */
    DWORD            svc_hdl;            /* service handle */
    uint16_t            profile_ver;        /* ProfileVersion attribute value */
    uint16_t            svc_type;            /* type of A2DP service, it can be SRC or SNK */
    uint16_t            features;            /* supported features */
};

struct sdap_hcrp_info_stru {
    uint16_t            size;                /* sizeof sdap_hcrp_info_stru */
    uint16_t            mask;                /* optional attribute mask */
    DWORD            svc_hdl;            /* service handle */
    /* the UUID16 value of the most specific class in ServiceClassIDList Attribute */
    uint16_t             svc_type;
    uint16_t             conn_type;            /* connection type, it can be Control Channel or Notification Channel. */
    uint16_t            psm_main;            /* psm value for ProtocolDescriptorList attribute */
    uint16_t            psm_data;            /* psm value for AdditionalProtocolDescriptorList attribute, only valid for
                                            when . */
    /* service_id attribute, only valid when conn_type is set to Control Channel. */
    DWORD            svc_id;
    /* size of the 1284id attribute, only valid when conn_type is set to Control Channel. */
    uint16_t             size_of_1284id;
    /* 1284id attribute value, only valid when conn_type is set to Control Channel. */
    uint8_t            str_1284id[1];
};

struct sdap_hid_info_stru {
    uint16_t             size;                /* size of sdap_hid_info_stru, include additional bytes for stores_list */
    uint16_t             mask;                /* optional or mandatory Bool type attribute mask */
    DWORD             svc_hdl;            /* service handle */
    uint16_t            release_num;        /* HID device release number */
    uint16_t            parser_ver;            /* HID parser version */
    uint8_t            sub_cls;            /* HID device subclass */
    uint8_t            country_code;        /* HID country code */
    uint16_t            super_to;            /* HID supervision timeout */
    uint16_t            profile_ver;        /* HID ProfileVersion attribute value */
    uint16_t            desc_list_size;        /* total size of the descriptor list. It also marks the start point
                                           of the report list in the successive memory. */
    uint8_t            list[1];            /* list of HID class descriptor. */
};

struct sdap_di_info_stru {
    uint16_t            size;                /* size of sdap_di_info_stru, include additional bytes for str_url_list */
    uint16_t            mask;                /* optional or mandatory Bool type attribute mask */
    DWORD            svc_hdl;            /* service handle */
    uint16_t            spec_id;            /* value of SpecificationID attribute */
    uint16_t            vendor_id;            /* value of VendorID attriubte */
    uint16_t            product_id;            /* value of ProductID attribute */
    uint16_t            version;            /* value of Version attribute */
    uint16_t            vendor_id_src;        /* value of VendorIDSource attribute */
    uint16_t            list_size;            /* size of the text string list */
    uint8_t            str_url_list[1];    /* List of ClientExecutableURL, DocumentationURL and
                                            ServiceDescription attributes. */
};

struct sdap_bip_img_info_stru {
    uint16_t             size;                /* size of sdap_bip_img_info_stru */
    uint16_t            reserved;            /* reserved for alignment */
    DWORD            svc_hdl;            /* service handle */
    uint8_t            svr_chnl;            /* server channel */
    uint8_t            capabilities;        /* supported capabilities */
    uint16_t            psm;                /* PSM of OBEX over L2CAP */
    uint16_t            features;            /* supported features */
    DWORD            functions;            /* supported functions */
    struct sdap_u_int64stru total_data_capacity; /* value of the TotalImagingDataCapacity attribute */
};

struct sdap_bip_obj_info_stru {
    uint16_t            size;                /* size of sdap_bip_obj_info_stru */
    uint16_t            mask;                /* attribute mask. It presents size descriptor of svc_id currently. */
    DWORD            svc_hdl;            /* service handle */
    uint16_t            obj_type;            /* a UUID16 representing the type of the object,
                                            it can be a referenced object
                                            or an automatic archive. */
    uint16_t            psm;                /* PSM of OBEX over L2CAP */
    uint8_t            svr_chnl;            /* server channel */
    struct uuid_stru svc_id;        /* The value of ServiceId attribute */
    DWORD            functions;            /* supported functions */
};

struct sdap_sap_info_stru {
    uint16_t            size;                /* size of sdap_sap_info_stru */
    uint16_t            mask;                /* attribute mask */
    DWORD            svc_hdl;            /* service handle */
    uint8_t            svr_chnl;            /* server channel */
};

struct sdap_vdp_info_stru {
    uint16_t            size;                /* size of sdap_a2dp_info_stru */
    uint16_t            reserved;            /* reserved for alignment */
    DWORD            svc_hdl;            /* service handle */
    uint16_t            svc_type;            /* type of VDP service, it can be SRC or SNK */
};

struct sdap_bpp_printer_info_stru {
    /* size of sdap_bpprui_info_stru, include additional bytes for fmt_url_list */
    uint16_t                   size;
    uint16_t                   mask;                /* attribute mask */
    DWORD                  svc_hdl;            /* service handle */
    struct uuid_stru  svc_id;            /* The value of ServiceId attribute. */
    struct uuid_stru char_repertoires;  /* The value of CharacterRepertoiresSupported attribute */
    uint16_t                  profile_ver;        /* ProfileVersion attribute value */
    uint8_t                  job_chnl;            /* Job channel */
    uint8_t                  status_chnl;        /* status channel */
    uint16_t                  max_media_width;    /* MaxMediaWidth attribute value */
    uint16_t                  max_media_len;    /* MaxMediaLength attribute value */
    uint16_t                  docfmts_size;        /* size of the DocumentFormatsSupported string in str_list */
    uint16_t                  imgfmts_size;        /* size of the XHTML-PrintImageFormatsSupported string in str_list */
    uint16_t                  pt1284id_size;    /* size of the 1284ID string in str_list */
    uint16_t                  ptname_size;        /* size of the PrinterName string in str_list */
    uint16_t                  ptlocation_size;  /* size of the PrinterLocation string in str_list */
    uint16_t                  mediatypes_size;  /* size of the MediaTypesSupported string in str_list */
    uint16_t                  ruifmts_size;        /* size of the RUIFormatsSupported string in str_list */
    uint16_t                  pbr_topurl_size;    /* size of the ReferencePrintingTopURL string in str_list */
    uint16_t                  dps_topurl_size;  /* size of the DirectPrintingTopURL string in str_list */
    uint16_t                  devname_size;        /* size of the DeviceName string in str_list */
    uint8_t                  str_list[1];         /* List of DocumentFormatsSupported, XHTML-PrintImageFormatsSupported,
                                                   1284ID, PrinterName, PrinterLocation, MediaTypesSupported,
                                                   RUIFormatsSupported, ReferencePrintingTopURL, DirectPrintingTopURL
                                                   and DeviceName attributes. */
};

struct sdap_bpp_obj_info_stru {
    /* size of sdap_bpprui_info_stru, include additional bytes for fmt_url_list */
    uint16_t                   size;
    uint16_t                   mask;                /* attribute mask */
    DWORD                  svc_hdl;            /* service handle */
    struct uuid_stru  svc_id;            /* The value of ServiceId attribute. */
    uint16_t                  profile_ver;        /* ProfileVersion attribute value */
    uint8_t                  obj_chnl;            /* Object server channel */
    uint8_t                  job_chnl;            /* RUI referenced job channel */
};

struct sdap_bpprui_info_stru {
    /* size of sdap_bpprui_info_stru, include additional bytes for fmt_url_list */
    uint16_t                   size;
    uint16_t                   mask;                /* attribute mask */
    DWORD                  svc_hdl;            /* service handle */
    struct uuid_stru  svc_id;            /* The value of ServiceId attribute. */
    uint16_t                  ruifmts_size;        /* size of the RUIFormatsSupported string in str_list */
    uint16_t                  rui_topurl_size;    /* size of the PrinterAdminRUITopURL string in str_list */
    uint8_t                  svr_chnl;            /* RUI server channel */
    /* List of RUIFormatsSupported and PrinterAdminRUITopURL attributes. */
    uint8_t                  str_list[1];
};

struct sdap_pbappce_info_stru {
    uint16_t             size;                /* size of sdap_pbappce_info_stru */
    uint16_t            mask;                /* attribute mask */
    DWORD            svc_hdl;            /* service handle */
    uint16_t            profile_ver;        /* Profile Version value */
};

struct sdap_pbappse_info_stru {
    uint16_t             size;                /* size of sdap_pbappse_info_stru */
    uint16_t            mask;                /* attribute mask */
    DWORD            svc_hdl;            /* service handle */
    uint16_t            profile_ver;        /* Profile Version value */
    uint8_t            svr_chnl;            /* PSE server channel */
    uint8_t            sup_repst;            /* SupportedRepositories attribute value */
    uint16_t            psm;                /* PSM of OBEX over L2CAP */
    DWORD            features;            /* PBAP Supported Features */
};

struct sdap_hdp_info_stru {
    /* size of sdap_hdp_info_stru, include additional bytes for mdep_entry_list */
    uint16_t             size;
    uint16_t            mask;                /* attribute mask */
    DWORD            svc_hdl;            /* service handle */
    uint16_t            protocol_ver;       /* MCAP Protocol Version value */
    uint16_t            profile_ver;        /* HDP Profile Version value */
    uint16_t            fst_svc_type;       /* First UUID in the ServiceClassIDList */
    /* Second UUID in the ServiceClassIDList if presented. Set it to 0 if not required. */
    uint16_t            snd_svc_type;
    uint16_t            psm_ctrl;           /* Control channel PSM */
    uint16_t            psm_data;           /* data channel PSM */
    uint8_t           data_exchg_spec;    /* data Exchange Specification */
    uint8_t           mcap_sup_proc;      /* MCAP Supported Procedure */
    uint16_t            mdep_entry_num;     /* Number of MDEP entries supported */
    uint8_t           mdep_entry_list[1]; /* List of MDEP entries. Each entry shall specifies its MDEP_ID,
                                           MDEP_Data_Type,
                                           uint8_t           mdep_entry_list[1];
                                           MDEP_Role and MDEP_Description values in the following order: ||
                                           -MDEP_ID (1Byte)-||-MDEP_Data_Type (2Bytes,
                                           Little Endian)-||-MDEP_Role (1Byte)-|| ||
                                           -MDEP_Description_Length (2Bytes,
                                            Little Endian)-||-MDEP_Descrition (Variable Length)-||. */
};

struct sdap_hdp_mdep_list_stru {
    uint8_t           id;            /* MDEP ID value */
    uint8_t           role;          /* MDEP Role value */
    uint16_t            data_type;     /* MDEP data type value */
    uint8_t           *description;  /* MDEP Description, null-terminated UTF-8 string */
};

struct sdap_hdp_info_hlp_stru {
    DWORD            svc_hdl;            /* service handle */
    uint16_t            protocol_ver;       /* MCAP Protocol Version value */
    uint16_t            profile_ver;        /* Profile Version value */
    uint16_t            fst_svc_type;       /* First UUID in the ServiceClassIDList */
    /* Second UUID in the ServiceClassIDList if presented. Set it to 0 if not required. */
    uint16_t            snd_svc_type;
    uint16_t            psm_ctrl;           /* Control channel PSM */
    uint16_t            psm_data;           /* data channel PSM */
    uint8_t           data_exchg_spec;    /* data Exchange Specification */
    uint8_t           mcap_sup_proc;      /* MCAP Supported Procedure */
    uint16_t            mdep_entry_num;     /* Number of MDEP entries supported */
    struct sdap_hdp_mdep_list_stru *mdep_entry_list; /* List of MDEP entries */
};

struct sdap_mapmas_info_stru {
    uint16_t            size;               /* size of sdap_mapmas_info_stru */
    uint16_t            mask;               /* attribute mask */
    DWORD            svc_hdl;            /* service handle */
    uint16_t            profile_ver;        /* Profile Version value */
    uint8_t            svr_chnl;            /* MSE server channel */
    uint8_t            mas_inst_id;        /* MAS Instance ID */
    uint8_t            sup_msg_types;      /* Supported Message Types */
    uint16_t            psm;                /* PSM of OBEX over L2CAP */
    DWORD            features;            /* MAP Supported Features */
};

struct sdap_mapmns_info_stru {
    uint16_t            size;               /* size of sdap_mapmas_info_stru */
    uint16_t            mask;               /* attribute mask */
    DWORD            svc_hdl;            /* service handle */
    uint16_t            profile_ver;        /* Profile Version value */
    uint8_t            svr_chnl;            /* MSE server channel */
    uint16_t            psm;                /* PSM of OBEX over L2CAP */
    DWORD            features;            /* MAP Supported Features */
};

struct sdap_gatt_info_stru {
    uint16_t            size;               /* size of sdap_gatt_info_stru */
    uint16_t            mask;               /* attribute mask. For alignment only, set to 0. */
    DWORD           svc_hdl;            /* service handle */
    uint16_t            service_class;        /* GATT Service type */
    uint16_t            start_handle;        /* GATT Start Handle */
    uint16_t            end_handle;            /* GATT End Handle */
};

#define SDAP_HEP_INFO_STRU            sdap_hephs_info_stru
#define SDAP_HFP_INFO_STRU            sdap_hfphf_info_stru

#define SDAP_GN_INFO_STRU                sdap_pan_info_stru
#define SDAP_NAP_INFO_STRU            sdap_pan_info_stru
#define SDAP_PANU_INFO_STRU            sdap_pan_info_stru

#define SDAP_AVCT_INFO_STRU            sdap_avrcp_info_stru
#define SDAP_AVTG_INFO_STRU            sdap_avrcp_info_stru

#define SDAP_A2DP_SRC_INFO_STRU        sdap_a2dp_info_stru
#define SDAP_A2DP_SNK_INFO_STRU        sdap_a2dp_info_stru

#define SDAP_HCRP_CTRL_INFO_STRU        sdap_hcrp_info_stru
#define SDAP_HCRP_NOTIFY_INFO_STRU        sdap_hcrp_info_stru

#define SDAP_BIP_REF_OBJ_INFO_STRU        sdap_bip_obj_info_stru
#define SDAP_BIP_ARCH_INFO_STRU        sdap_bip_obj_info_stru

#define SDAP_HDP_SRC_INFO_STRU         sdap_hdp_info_stru
#define SDAP_HDP_SNK_INFO_STRU         sdap_hdp_info_stru

#endif
