/*
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved.
 * Description: sdk api gatt
 * Author: @CompanyNameTag
 * Create: 2022-03-15
 */

#ifndef AT_COMMAND_GATT_H
#define AT_COMMAND_GATT_H

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/*
 * Input: 0 params
 */
int bts_at_cmd_gatt_create_server(int argc, char *argv[]);

/*
 * Input: 3 params
 * < server id > < service uuid > < service type >
 * service type: BTH_GATT_PRIMARY_SERVICE, BTH_GATT_SECOND_SERVICE
 */
int bts_at_cmd_gatt_create_service(int argc, char *argv[]);

/*
 * Input: 3 params
 * < service_handle > < service_uuid > < service_handle >
 * server id: return by gatt_reg_service
 * service_uuid: set to the UUID of the included service
 * service_handle: set to 0; else, it shall be set to the attribute handle of its service declaraction
 */
int bts_at_cmd_gatt_add_include_service(int argc, char *argv[]);

/*
 * Input: 11 or 13 params(13 params include characteristic value)
 * < service_handle > < broadcastable_flag > < readable_flag > < writable_flag >
 * < writable_without_response_flag >
 * < signed_writable_flag > < notifiable_flag > < indicatable_flag > < uuid > < authentication_flag > < mitm_flag >
 * [ data_len < character value > ]
 */
int bts_at_cmd_gatt_add_characteristic(int argc, char *argv[]);

/*
 * Input: 5 or 7 params
 * < characteristic_handle > < descriptor_type > < descriptor_uuid > <read_permission> <write_permission>
 * characteristic_handle:temporary handle used to identify this characteristic instance,
                         which is returned by bts_at_cmd_gatt_add_characteristic
 * descriptor_type: decides the format of the descriptor value
 * descriptor_uuid:the UUID of this descriptor
 * read_permission: read permissions for this descriptor
 * write_permission:write permissions for this descriptor
 */
int bts_at_cmd_gatt_add_descriptor(int argc, char *argv[]);

/*
 * Input: 1 params
 * < server id >
 */
int bts_at_cmd_gatt_start_service(int argc, char *argv[]);

/*
 * Input: 1 params
 * < server uuid >
 */
int bts_at_cmd_gatt_stop_service(int argc, char *argv[]);
/*
 * Input: 6 params
 * < addr > < request_id > < status > <offset> <data_size> <data>
 * request_id: id of the request that was received with the callback
 * status: the status of the request to be sent to the remote device
 * offset: value offset for partial read/write operation. It shall be
 *         set to the same offset value requested by the read or write event callback.
 * data_size: size of the value data that was read/written.
 * data:value data that was read. the application can provide values different with that stored
 *            in the host stack. the stored value is sent if data is NULL.
 *            It is ignored in the response to write request.
 */
int bts_at_cmd_gatt_send_response(int argc, char *argv[]);

/*
 * Input: 3 params
 * < characteristic_handle > < data_len > < character value >
 * characteristic_handle: attribute handle of the characteristic declaraction
 * data: Value data
 */
int bts_at_cmd_gatt_notify_characteristic_changed(int argc, char *argv[]);

/*
 * Input: 3 params
 * < uuid > < data_len > < character value >
 */
int bts_at_cmd_gatt_notify_characteristic_changed_by_uuid(int argc, char *argv[]);

/*
 * Input: 3 params
 * < descriptor_handle > < data len > < data >
 * characteristic_handle: attribute handle of the descriptor declaraction
 * data: Value data
 */
int bts_at_cmd_gatt_update_descriptor_value_changed(int argc, char *argv[]);
/*
 * Input: 1 or 2 params
 * < addr > [ uuid ]
 * addr: remote device addr which need to discovery service
 * uuid: option item. discovery service by uuid if with input uuid or discovery all service
 */

int bts_at_cmd_gatt_discovery_service(int argc, char *argv[]);
/*
 * Input: 2 params
 * < addr > < service_handle >
 * addr: remote device addr which need to discovery service
 * service_handle: which service that need to discovery
 */

int bts_at_cmd_gatt_find_include_service(int argc, char *argv[]);
/*
 * Input: 2 or 3 params
 * < addr > < service_handle > [ uuid ]
 * addr: remote device addr which need to discovery service
 * service_handle: which characteristics that need to discovery
 * uuid: option item. discovery characteristics by uuid if with input uuid
 */

int bts_at_cmd_gatt_discovery_characteristics(int argc, char *argv[]);
/*
 * Input: 3 params
 * < addr > < service_handle > < declare_handle >
 * addr: remote device addr which need to discovery service
 * service_handle: which service that need to discovery
 * declare_handle: which characteristic that need to discovery
 */

int bts_at_cmd_gatt_discovery_characteristics_by_uuid(int argc, char *argv[]);

/*
 * Input: 4 params
 * < addr > < start_handle > < end_handle > < uuid >
 * addr: remote device addr which need to discovery service
 * start_handle: which handle that need start to discovery
*  end_handle: which handle that need end to discovery
 * uuid: discovery characteristics by uuid
 */
int bts_at_cmd_gatt_read_characteristics_by_uuid(int argc, char *argv[]);

/*
 * Input: 3 params
 * < addr > < service_handle > < declare_handle >
 * addr: remote device addr which need to discovery service
 * service_handle: which service that need to discovery
 * declare_handle: which characteristic that need to discovery
 */
int bts_at_cmd_gatt_discovery_descriptor(int argc, char *argv[]);

/*
 * Input: 1 params
 * < addr >
 * addr: remote device addr which need to discovery service
 */
int bts_at_cmd_gatt_start_discovery_all_services(int argc, char *argv[]);
/*
 * Input: 5 params
 * < addr > < value_handle > < writable_without_response_flag > < len > < data >
 * addr: remote device addr which need to discovery service
 * value_handle: which characteristic that need to write
 */
int bts_at_cmd_gatt_discovery_descriptor_by_handle(int argc, char *argv[]);

/*
 * Input: 6 params
 * < addr > < value_handle > < writable_without_response_flag > < offset > < len > [ data ]
 * addr: remote device addr which need to discovery service
 * value_handle: which characteristic that need to write
 */
int bts_at_cmd_gatt_write_characteristic_value(int argc, char *argv[]);

/*
 * Input: 5 params
 * < addr > < value_handle > < writable_without_response_flag > < len > [ data ]
 * addr: remote device addr which need to discovery service
 * value_handle: which characteristic that need to write
 */
int bts_at_cmd_gatt_write_characteristic_by_handle(int argc, char *argv[]);

/*
 * Input: 6 params
 * < addr > < value_handle > < writable_without_response_flag > < offset > < len > [ data ]
 * addr: remote device addr which need to discovery service
 * value_handle: which characteristic that need to write
 */
int bts_at_cmd_gatt_reliable_write_characteristic_value(int argc, char *argv[]);

/*
 * Input: 5 params
 * < addr > < attribute_handle > < descriptor_type > < len > < data >
 * addr: remote device addr which need to discovery service
 * attribute_handle: which descriptor that need to write
 */
int bts_at_cmd_gatt_write_descriptor_value(int argc, char *argv[]);

/*
 * Input: 2 params
 * < addr > < mtu >
 * addr: remote device addr which need to discovery service
 * mtu: which mtu that need to exchange
 */

int bts_at_cmd_gatt_mtu_config(int argc, char *argv[]);

/*
 * Input: 0 params
 */
int bts_at_cmd_gatt_get_gap_ui_attributes(int argc, char *argv[]);

/*
 * Input: 3 params
 * < name_len > < name > < appearance >
 */
int bts_at_cmd_gatt_set_gap_ui_attributes(int argc, char *argv[]);

/*
 * Input: 5 - 6 params
 * < addr > <addr_type> < att handle > < offset > < security mode > [ timeout ]
 */
int bts_at_cmd_gatt_get_report_map(int argc, char *argv[]);
int bts_at_cmd_gatt_hid_device_ccc_enable(int argc, char *argv[]);
int bts_at_cmd_gatt_discovery_character(int argc, char *argv[]);
int bts_at_cmd_gatt_discovery_descriptors(int argc, char *argv[]);
int bts_at_cmd_hid_device_report_id_init(int argc, char *argv[]);
int bt_at_cmd_multiple_value_nfy(int argc, char *argv[]);
int bt_at_cmd_multiple_value_read_character(int argc, char *argv[]);
#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */

#endif /* AT_COMMAND_GATT_H */
