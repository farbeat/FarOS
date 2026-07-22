/*
 * Copyright (c) @CompanyNameMagicTag 2022-2023. All rights reserved.
 * Description: bth mpc stack struct for gatt
 * Author: @CompanyNameTag
 * Create: 2022-03-01
 */
#ifndef _BTH_STACK_GATT_H_
#define _BTH_STACK_GATT_H_

#include "bth_stack.h"

#define BD_ADDR_SIZE 6
#define GATT_UUID_LEN           16
#define GATT_UUID_LEN_2         2
#define GATT_UUID_LEN_16        16
#define GATT_GAP_MAX_NAME_LEN   248
#define GATT_READ_MULTIPLE_HANDLE_NUM    5
#define GATT_MULTIPLE_VALUE_HANDLE_NUM   5
#define GATT_MULTIPLE_VALUE_DATA_LEN     10
#define GATT_MIN_ATT_MTU        23
#define GATT_MAX_ATT_MTU        517
#define GATT_MAX_ATTR_LEN       512
#define GATT_MAX_CONTROLLER_MTU    236
#define GATT_MAX_DYNAMIC_CID_NUM    0x40  /* dynamic cid range: 0x40 ~ 0x7f */

typedef enum gatt_secure_conn_type {
    NONE_SECURE = 0,
    NEED_ENCRYPTION,
    NEED_AUTHENTICATE,
} gatt_secure_conn_type_t;

typedef struct {
    uint8_t uuid_len;    /* GATT_UUID_LEN2 or GATT_UUID_LEN_16 */
    uint8_t *uuid_val;
} ble_gatt_uuid_stru;

typedef struct {
    uint16_t len;
    uint8_t *val;
} ble_gatt_attr_val_stru;

typedef struct {
    uint16_t desc_hdl;             /* [Out], descriptor handle */
    uint16_t permission;           /* [In], permission for descriptor val access: BLE_WRITE */
    ble_gatt_uuid_stru uuid;       /* [In], uuid for descriptor */
    ble_gatt_attr_val_stru val;    /* [In], value of attribute */
} ble_gatt_chara_desc_info_stru;

typedef enum {
    BLE_READ_EVENT = 1,
    BLE_WRITE_EVENT,
    BLE_EXCUTE_WRITE_EVENT,
} ble_gatt_req_event;

typedef struct {
    bth_address addr; /* Remote Addr */
    uint8_t    handle_count;
    uint16_t   handle_set[GATT_MULTIPLE_VALUE_HANDLE_NUM];
    uint8_t    value_count;
    uint8_t    value[GATT_MULTIPLE_VALUE_HANDLE_NUM][GATT_MULTIPLE_VALUE_DATA_LEN];
    uint8_t   flags;
} bth_gatt_multiple_value_nfy_request;

typedef struct {
    uint8_t u[GATT_UUID_LEN]; /* Bluetooth Base UUID + 16bits UUID, or 128bit UUID */
} bth_att_uuid;

typedef struct {
    uint32_t      is_short_uuid;
    uint16_t      short_uuid;
    uint16_t      reserve;
    bth_att_uuid  long_uuid;
} bth_gatt_uuid;

typedef enum {
    BTH_GATT_PRIMARY_SERVICE,
    BTH_GATT_SECOND_SERVICE
} bth_gatt_service_type;
typedef struct {
    uint32_t server_handle;
} bth_gatt_add_server_result;

typedef struct {
    uint32_t              server_handle;
    bth_gatt_uuid         service_uuid;
    bth_gatt_service_type service_type;
} bth_gatt_add_server;

typedef struct {
    bth_gatt_uuid service_uuid;
    uint32_t      service_handle;
} bth_gatt_start_service;

typedef struct {
    bth_gatt_uuid service_uuid;
    uint16_t      service_handle;
} bth_gatt_service;

typedef struct {
    uint32_t service_handle;
} bth_gatt_create_service_result;

typedef struct {
    uint32_t               service_handle;
    bth_gatt_start_service service;
} bth_gatt_add_include;

typedef struct {
    uint32_t len;
    uint8_t  data[0];
} bth_gatt_characteristic_value;

typedef struct {
    uint16_t      service_handle;
    bth_gatt_uuid characteristic_uuid;
    uint16_t      declare_handle;
    uint16_t      value_handle;
    uint32_t      broadcastable_flag;
    uint32_t      readable_flag;
    uint32_t      writable_flag;
    uint32_t      writable_without_response_flag;
    uint32_t      signed_writable_flag;
    uint32_t      notifiable_flag;
    uint32_t      indicatable_flag;
    uint32_t      authentication_flag;
    uint32_t      authorization_flag;
    uint32_t      mitm_flag;
    uint32_t      has_extended_properties;
} bth_gatt_characteristic;

typedef enum {
    BTH_ADD_SERVICE_COMPLETE_EVENT,
    BTH_READ_CHARACTERISTIC_REQUEST_EVENT,
    BTH_WRITE_CHARACTERISTIC_REQUEST_EVENT,
} bth_gatt_event_type;

typedef struct {
    bth_address addr; /* Remote Addr */
    uint8_t     data_size;
    uint8_t     data[0];
} bth_gatt_request_result;

typedef struct {
    uint32_t                service_handle;
    bth_gatt_characteristic characteristic;
    uint32_t                data_size;
    uint8_t                 data[0];
} bth_gatt_add_characteristic;

typedef struct {
    uint32_t characteristic_handle;
} bth_gatt_add_characteristic_result;

typedef enum {
    BTH_CHARACTERISTIC_EXTENDED_PROPERTIES,
    BTH_CHARACTERISTIC_USER_DESCRIPTION,
    BTH_CLIENT_CHARACTERISTIC_CONFIGURATION,
    BTH_SERVER_CHARACTERISTIC_CONFIGURATION,
    BTH_CHARACTERISTIC_FORMAT,
    BTH_CHARACTERISTIC_AGGREGATE_FORMAT,
    BTH_CUSTOM_DESCRIPTOR
} bth_gatt_descriptor_type;

typedef struct {
    bth_gatt_descriptor_type descriptor_type;
    bth_gatt_uuid            descriptor_uuid;
    union {
        struct {
            uint32_t is_reliable_write;
            uint32_t is_auxiliaries_writable;
        } bth_characteristic_extended_properties;
        struct {
            uint32_t is_subscribeto_notification;;
            uint32_t is_subscribeto_indication;;
        } client_characteristic_configuration;
        struct {
            uint32_t is_broadcast;
        } server_characteristic_configuration;
        struct {
            uint8_t       format;
            uint8_t       exponent;
            bth_gatt_uuid unit;
            uint8_t       name_space;
            bth_gatt_uuid description;
        } characteristic_format;
    };
    uint32_t data_size;
    uint8_t  data[0];
} bth_gatt_descriptor_value;

typedef enum {
    BTH_GATT_NOT_READABLE,
    BTH_GATT_READABLE_WITHOUT_SECURITY,
    BTH_GATT_READABLE_WITH_AUTHENTICATION,
    BTH_GATT_READABLE_WITH_ENCRYPTION
} bth_gatt_read_permission;

typedef enum {
    BTH_GATT_NOT_WRITABLE,
    BTH_GATT_WRITABLE_WITHOUT_SECURITY,
    BTH_GATT_WRITABLE_WITH_AUTHENTICATION,
    BTH_GATT_WRITABLE_WITH_ENCRYPTION
} bth_gatt_write_permission;

typedef enum {
    BTH_GATT_WRITE_WITH_RESPONSE,         /* Write Request */
    BTH_GATT_WRITE_WITHOUT_RESPONSE,      /* Write Command */
    BTH_GATT_WRITE_PREPARE,               /* Prepare Write */
} bth_gatt_write_type;

typedef struct {
    bth_gatt_read_permission  read_permission;  /* Read Permission */
    bth_gatt_write_permission write_permission; /* Write Permission */
} bth_gatt_descriptor_permission;

typedef struct {
    uint32_t                       characteristic_handle;
    uint8_t                        permission;
    bth_gatt_descriptor_value      descriptor_value;
} bth_gatt_add_descriptor;

typedef struct {
    bth_address addr; /* Remote Addr */
    uint64_t    status;
    uint32_t    request_id;
    uint32_t    offset;
    uint32_t    data_size;
    uint8_t     data[0];
} bth_gatt_send_response;

typedef struct {
    uint16_t                  descriptor_handle;
    bth_gatt_descriptor_value descriptor_value;
} bth_gatt_update_descriptor;

typedef struct {
    uint16_t characteristic_handle;
    uint16_t data_size;
    uint8_t  data[0];
} bth_gatt_notify_characteristic;

typedef struct {
    uint16_t start_hdl;
    uint16_t end_hdl;
    bth_gatt_uuid uuid;
    uint16_t data_size;
    uint8_t  data[0];
} bth_gatt_notify_characteristic_by_uuid;

typedef struct {
    bth_address addr; /* Remote Addr */
    uint8_t  error_code;
    uint16_t handle;
    uint16_t offset;
    uint16_t val_len;
    uint8_t  val[0];
} bth_gatt_read_character_response;

typedef struct {
    bth_address addr; /* Remote Addr */
    uint32_t  error_code;
    uint16_t handle;
    uint16_t offset;
    uint16_t val_len;
    uint8_t  val[0];
} bth_gatt_write_attribute_response;

typedef struct {
    bth_address addr; /* Remote Addr */
    uint16_t    att_handle;
    uint16_t    offset;
    uint8_t     security_mode;
    uint32_t    timeout;
} bth_gatt_read_character_request;

typedef struct {
    bth_address addr; /* Remote Addr */
    uint16_t    handle_count;
    uint16_t    handle_set[GATT_READ_MULTIPLE_HANDLE_NUM];
    uint16_t    offset;
    uint16_t    flags;
} bth_gatt_read_multiple_character_request;

typedef struct {
    bth_address   addr;
    uint8_t       by_uuid_flag;
    bth_gatt_uuid uuid;
    uint16_t      flag;
} bth_gatt_discovery_service;

typedef struct {
    bth_address   addr;
    uint16_t      start_hdl;
    uint16_t      end_hdl;
    bth_gatt_uuid uuid;
} bth_gatt_discovery_service_response;

typedef struct {
    uint8_t client_id;
    uint16_t conn_id;
    bth_gatt_uuid uuid;
    uint16_t status;
} bth_gatt_discovery_service_complete;

typedef struct {
    bth_address addr;
    uint16_t    flag;
    bth_gatt_service service_info;
} bth_gatt_find_include_service;

typedef struct {
    bth_address  addr;
    uint16_t     start_hdl;
    uint16_t     end_hdl;
    uint16_t     include_hdl;
    bth_att_uuid uuid;
} bth_gatt_find_include_service_response;

typedef struct {
    bth_address   addr;
    uint16_t      service_handle;
    uint8_t       by_uuid_flag;
    bth_gatt_uuid character_uuid;
    uint16_t      flag;
} bth_gatt_discovery_character;

typedef struct {
    bth_address             addr;
    bth_gatt_characteristic characteristic;
} bth_gatt_discovery_character_response;

typedef struct {
    uint8_t client_id;
    uint16_t conn_id;
    uint16_t service_handle;
    bth_gatt_uuid uuid;
    uint16_t status;
} bth_gatt_discovery_character_complete;

typedef struct {
    bth_address   addr;
    uint16_t      start_handle;
    uint16_t      end_handle;
    bth_gatt_uuid uuid;
    uint16_t      flags;
} bth_gatt_discovery_character_by_uuid;

typedef struct {
    bth_address   addr;
    uint16_t      start_handle;
    uint16_t      end_handle;
    uint16_t      flags;
} bth_gatt_discovery_descriptor_by_handle;

typedef struct {
    uint16_t                 service_handle;
    uint16_t                 characteristic_handle;
    bth_gatt_descriptor_type descriptor_type;
    bth_gatt_uuid            descriptor_uuid;
    uint16_t                 attribute_handle;
} bth_gatt_descriptor;

typedef struct {
    bth_address             addr;
    uint16_t                flag;
    bth_gatt_characteristic characteristic_info;
} bth_gatt_discovery_descriptor;

typedef struct {
    bth_address              addr;
    uint16_t                 character_hdl;
    uint16_t                 descriptor_hdl;
    uint16_t                 properties;
    bth_gatt_descriptor_type descriptor_type;
    bth_gatt_uuid uuid;
} bth_gatt_discovery_descriptor_response;

typedef struct {
    uint8_t client_id;
    uint16_t conn_id;
    uint16_t character_handle;
    uint16_t status;
} bth_gatt_discovery_descriptor_complete;

typedef struct {
    bth_address             addr;
    bth_gatt_characteristic characteristic;
    uint16_t                flag;
} bth_gatt_read_characteristic;

typedef struct {
    bth_address                   addr;
    uint16_t                      characteristic_handle;
    bth_gatt_characteristic_value value;
} bth_gatt_read_characteristic_response, bth_gatt_client_notification;

typedef struct {
    uint8_t client_id;
    uint16_t conn_id;
    uint16_t start_hdl;
    uint16_t end_hdl;
    bth_gatt_uuid uuid;
    uint16_t status;
} bth_gatt_read_characteristic_complete;

typedef struct {
    bth_address             addr;
    bth_gatt_characteristic characteristic;
    uint16_t                flag;
    uint16_t                offset;
    uint16_t                len;
    uint8_t                 data[0];
} bth_gatt_write_characteristic;

typedef struct {
    bth_address               addr;
    uint16_t                  flag;
    bth_gatt_descriptor       descriptor;
    bth_gatt_descriptor_value descriptor_value;
} bth_gatt_write_descriptor;

typedef struct {
    bth_address         addr;
    uint16_t            flag;
    bth_gatt_descriptor descriptor;
} bth_gatt_read_descriptor;

typedef struct {
    bth_address               addr;
    uint16_t                  descriptor_handle;
    bth_gatt_descriptor_value value;
} bth_gatt_read_descriptor_response;

typedef struct {
    bth_address addr;
    uint16_t    mtu;
    uint32_t    flag;
} bth_gatt_mtu_config;

typedef struct {
    uint16_t name_len;
    uint8_t  name[GATT_GAP_MAX_NAME_LEN];
    uint16_t appearance;
} bth_gatt_set_gap_ui, bth_gatt_get_gap_ui_result;

typedef struct {
    uint16_t   le_att_mtu_default;
    uint16_t   le_att_mtu_le_max;
} bth_le_att_mtu_param;

typedef struct {
    uint8_t le_att_item_auth_check;
    uint8_t le_att_item_pend_flag;
} bth_le_att_config_param;

typedef struct {
    uint16_t opcode;
    uint8_t addr[BD_ADDR_SIZE];
    uint8_t addr_type;
    uint16_t request_id;
    uint16_t att_hdl;
    uint8_t svc_uuid[GATT_UUID_LEN];
    uint8_t uuid[GATT_UUID_LEN];
    uint16_t offset;
    uint16_t len;
    uint8_t need_authorize;
    uint8_t need_rsp;
    uint8_t data[0];
} bth_gatt_upper_data_t;

typedef struct {
    bth_address addr;
    uint16_t    mtu;
} bth_gatt_mtu;

#endif /* _BTH_STACK_GATT_H_ */
