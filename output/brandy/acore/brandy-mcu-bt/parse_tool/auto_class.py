from ctypes import *
td_bool=c_bool
td_char=c_char
td_uchar=c_ubyte
td_s16=c_short
td_u16=c_ushort
td_s32=c_int
td_u32=c_uint
td_s64=c_longlong
td_u64=c_ulonglong
td_float=c_float
td_double=c_double
uint8_t=td_uchar
uint32_t=td_u32
hal_uart_init_t=td_u32
hal_uart_deinit_t=td_u32
hal_uart_ctrl_t=td_u32
hal_uart_write_t=td_u32
hal_uart_read_t=td_u32
hal_uart_dma_cfg_t=td_u32
class random_type_1(Structure):
    _fields_ = [
        ('init', hal_uart_init_t),
        ('deinit', hal_uart_deinit_t),
        ('ctrl', hal_uart_ctrl_t),
        ('write', hal_uart_write_t),
        ('read', hal_uart_read_t),
        ('dma_cfg', hal_uart_dma_cfg_t),
    ]
errcode_t=td_u32
hal_uart_callback_t=td_u32
uintptr_t=td_u32
class uart_attr(Structure):
    _fields_ = [
        ('baud_rate', uint32_t),
        ('data_bits', uint8_t),
        ('stop_bits', uint8_t),
        ('parity', uint8_t),
        ('pad', uint8_t),
    ]
hal_uart_attr_t=uart_attr
uint16_t=td_u16
int32_t=td_s32
class uart_extra_attr(Structure):
    _fields_ = [
        ('tx_dma_enable', td_bool),
        ('tx_int_threshold', uint8_t),
        ('rx_dma_enable', td_bool),
        ('rx_int_threshold', uint8_t),
    ]
hal_uart_extra_attr_t=uart_extra_attr
at_cmd_handle=td_u32
class random_type_3(Structure):
    _fields_ = [
        ('str_cmd', td_u32),
        ('cmd_handle', at_cmd_handle),
        ('str_common', td_u32),
    ]
at_cmd_table_t=random_type_3
va_list=td_u32
__builtin_va_list=td_u32
class random_type_5(Structure):
    _fields_ = [
        ('ser_id', uint8_t),
        ('cmd_id', uint8_t),
        ('src', uint8_t),
        ('dst', uint8_t),
        ('crc_en', td_bool),
        ('ack_en', td_bool),
        ('length', uint16_t),
    ]
diag_ser_header_t=random_type_5
class random_type_4(Structure):
    _fields_ = [
        ('header', diag_ser_header_t),
        ('payload', (uint8_t*0)),
    ]
diag_ser_data_t=random_type_4
class random_type_6(Structure):
    _fields_ = [
        ('str_cmd', td_u32),
        ('cmd_handle', at_cmd_handle),
        ('str_common', td_u32),
    ]
class random_type_8(Structure):
    _fields_ = [
        ('devType', uint16_t),
        ('tranID', uint8_t),
        ('control', uint8_t),
        ('serviceID', uint8_t),
        ('commandID', uint8_t),
        ('appID', (uint8_t*4)),
    ]
commu_inter_header_t=random_type_8
class random_type_7(Structure):
    _fields_ = [
        ('interHeader', commu_inter_header_t),
        ('payloadLength', uint16_t),
        ('tranIDSrc', uint8_t),
        ('reserved', uint8_t),
        ('payload', (uint8_t*0)),
    ]
commu_inter_info_t=random_type_7
class random_type_10(Structure):
    _fields_ = [
        ('name', td_u32),
        ('attr_bits', uint32_t),
        ('cb_mem', td_u32),
        ('cb_size', uint32_t),
    ]
osTimerAttr_t=random_type_10
osTimerFunc_t=td_u32
class random_type_11(Structure):
    _fields_ = [
        ('ser_id', uint8_t),
        ('cmd_id', uint8_t),
        ('src', uint8_t),
        ('dst', uint8_t),
        ('crc_en', td_bool),
        ('ack_en', td_bool),
        ('length', uint16_t),
    ]
osTimerId_t=td_u32
dft_func=td_u32
class random_type_12(Structure):
    _fields_ = [
        ('at_id', uint32_t),
        ('func', dft_func),
    ]
btsrv_at_cmd_2_dft_t=random_type_12
btsrv_at_cmd_handle_func=td_u32
class random_type_13(Structure):
    _fields_ = [
        ('id', uint32_t),
        ('func', btsrv_at_cmd_handle_func),
    ]
btsrv_at_cmd_hdl_info=random_type_13
class random_type_15(Structure):
    _fields_ = [
        ('devType', uint16_t),
        ('tranID', uint8_t),
        ('control', uint8_t),
        ('serviceID', uint8_t),
        ('commandID', uint8_t),
        ('appID', (uint8_t*4)),
    ]
bt_manager_commu_header_t=random_type_15
class random_type_14(Structure):
    _fields_ = [
        ('interHeader', bt_manager_commu_header_t),
        ('payloadLength', uint16_t),
        ('tranIDSrc', uint8_t),
        ('reserved', uint8_t),
        ('payload', (uint8_t*0)),
    ]
bt_manager_commu_inter_info_t=random_type_14
class random_type_16(Structure):
    _fields_ = [
        ('module_id', uint8_t),
        ('cmd_id', uint8_t),
        ('tlv', (uint8_t*0)),
    ]
diag_ser_frame_t=random_type_16
class random_type_17(Structure):
    _fields_ = [
        ('interHeader', commu_inter_header_t),
        ('payloadLength', uint16_t),
        ('tranIDSrc', uint8_t),
        ('reserved', uint8_t),
        ('payload', (uint8_t*0)),
    ]
class random_type_18(Structure):
    _fields_ = [
        ('devType', uint16_t),
        ('tranID', uint8_t),
        ('control', uint8_t),
        ('serviceID', uint8_t),
        ('commandID', uint8_t),
        ('appID', (uint8_t*4)),
    ]
class random_type_19(Structure):
    _fields_ = [
        ('addr', (td_uchar*6)),
        ('type', uint8_t),
    ]
bth_bd_addr=random_type_19
class random_type_20(Structure):
    _fields_ = [
        ('addr', (uint8_t*6)),
        ('type', uint8_t),
    ]
bd_addr_t=random_type_20
class random_type_21(Structure):
    _fields_ = [
        ('header', diag_ser_header_t),
        ('payload', (uint8_t*0)),
    ]
errno_t=td_s32
class random_type_22(Structure):
    _fields_ = [
        ('payload_len', uint32_t),
        ('payload', td_u32),
    ]
at_cmd_msg_info_t=random_type_22
class random_type_23(Structure):
    _fields_ = [
        ('payload_len', uint32_t),
        ('payload', td_u32),
    ]
class random_type_24(Structure):
    _fields_ = [
        ('ssb_to_application_length', uint32_t),
        ('ssb_to_application_ssb_version', uint32_t),
        ('ssb_to_application_checksum', uint32_t),
        ('ssb_to_application_codeloader_version', uint32_t),
        ('ssb_to_application_reboot_cause', uint32_t),
        ('ssb_to_application_security_valid', uint32_t),
        ('ssb_to_application_protocol_valid', uint32_t),
        ('ssb_to_application_mcu_freq', uint32_t),
        ('ssb_to_application_flash_id', uint32_t),
        ('ssb_to_application_flash_unique_id', uint32_t),
    ]
startup_table=random_type_24
EXC_PROC_FUNC=td_u32
UINT32=td_u32
FLOATREG_TYPE=td_u32
class random_type_26(Structure):
    _fields_ = [
        ('mstatus', UINT32),
        ('mepc', UINT32),
        ('tp', UINT32),
        ('sp', UINT32),
        ('s11', UINT32),
        ('s10', UINT32),
        ('s9', UINT32),
        ('s8', UINT32),
        ('s7', UINT32),
        ('s6', UINT32),
        ('s5', UINT32),
        ('s4', UINT32),
        ('s3', UINT32),
        ('s2', UINT32),
        ('s1', UINT32),
        ('s0', UINT32),
        ('t6', UINT32),
        ('t5', UINT32),
        ('t4', UINT32),
        ('t3', UINT32),
        ('a7', UINT32),
        ('a6', UINT32),
        ('a5', UINT32),
        ('a4', UINT32),
        ('a3', UINT32),
        ('a2', UINT32),
        ('a1', UINT32),
        ('a0', UINT32),
        ('t2', UINT32),
        ('t1', UINT32),
        ('t0', UINT32),
        ('ra', UINT32),
        ('fs11', FLOATREG_TYPE),
        ('fs10', FLOATREG_TYPE),
        ('fs9', FLOATREG_TYPE),
        ('fs8', FLOATREG_TYPE),
        ('fs7', FLOATREG_TYPE),
        ('fs6', FLOATREG_TYPE),
        ('fs5', FLOATREG_TYPE),
        ('fs4', FLOATREG_TYPE),
        ('fs3', FLOATREG_TYPE),
        ('fs2', FLOATREG_TYPE),
        ('fs1', FLOATREG_TYPE),
        ('fs0', FLOATREG_TYPE),
        ('ft11', FLOATREG_TYPE),
        ('ft10', FLOATREG_TYPE),
        ('ft9', FLOATREG_TYPE),
        ('ft8', FLOATREG_TYPE),
        ('fa7', FLOATREG_TYPE),
        ('fa6', FLOATREG_TYPE),
        ('fa5', FLOATREG_TYPE),
        ('fa4', FLOATREG_TYPE),
        ('fa3', FLOATREG_TYPE),
        ('fa2', FLOATREG_TYPE),
        ('fa1', FLOATREG_TYPE),
        ('fa0', FLOATREG_TYPE),
        ('ft7', FLOATREG_TYPE),
        ('ft6', FLOATREG_TYPE),
        ('ft5', FLOATREG_TYPE),
        ('ft4', FLOATREG_TYPE),
        ('ft3', FLOATREG_TYPE),
        ('ft2', FLOATREG_TYPE),
        ('ft1', FLOATREG_TYPE),
        ('ft0', FLOATREG_TYPE),
        ('fcsr', UINT32),
        ('reserved', (UINT32*3)),
    ]
TaskContext=random_type_26
class random_type_25(Structure):
    _fields_ = [
        ('ccause', UINT32),
        ('mcause', UINT32),
        ('mtval', UINT32),
        ('gp', UINT32),
        ('taskContext', TaskContext),
    ]
ExcContext=random_type_25
NMI_PROC_FUNC=td_u32
UINT16=td_u16
UINTPTR=td_u32
TSK_ENTRY_FUNC=td_u32
class LOS_DL_LIST(Structure):
    _fields_ = [
        ('pstPrev', td_u32),
        ('pstNext', td_u32),
    ]
LOS_DL_LIST=LOS_DL_LIST
class random_type_27(Structure):
    _fields_ = [
        ('sortLinkNode', LOS_DL_LIST),
        ('idxRollNum', UINT32),
    ]
SortLinkList=random_type_27
class tagEvent(Structure):
    _fields_ = [
        ('uwEventID', UINT32),
        ('stEventList', LOS_DL_LIST),
    ]
EVENT_CB_S=tagEvent
class tagTaskCB(Structure):
    _fields_ = [
        ('stackPointer', td_u32),
        ('taskStatus', UINT16),
        ('priority', UINT16),
        ('taskFlags', UINT32,31),
        ('usrStack', UINT32,1),
        ('stackSize', UINT32),
        ('topOfStack', UINTPTR),
        ('taskId', UINT32),
        ('taskEntry', TSK_ENTRY_FUNC),
        ('taskSem', td_u32),
        ('joinner', td_u32),
        ('joined', td_u32),
        ('threadJoinRetval', td_u32),
        ('taskMux', td_u32),
        ('args', td_u32),
        ('taskName', td_u32),
        ('pendList', LOS_DL_LIST),
        ('sortList', SortLinkList),
        ('event', EVENT_CB_S),
        ('eventMask', UINT32),
        ('eventMode', UINT32),
        ('msg', td_u32),
        ('priBitMap', UINT32),
    ]
LosTaskCB=tagTaskCB
CHAR=td_char
SystemInitFunc=td_u32
size_t=td_u32
hal_gpio_callback_t=td_u32
class random_type_30(Structure):
    _fields_ = [
        ('str_cmd', td_u32),
        ('cmd_handle', at_cmd_handle),
        ('str_common', td_u32),
    ]
class random_type_32(Structure):
    _fields_ = [
        ('mstatus', uint32_t),
        ('mepc', uint32_t),
        ('tp', uint32_t),
        ('sp', uint32_t),
        ('s11', uint32_t),
        ('s10', uint32_t),
        ('s9', uint32_t),
        ('s8', uint32_t),
        ('s7', uint32_t),
        ('s6', uint32_t),
        ('s5', uint32_t),
        ('s4', uint32_t),
        ('s3', uint32_t),
        ('s2', uint32_t),
        ('s1', uint32_t),
        ('s0', uint32_t),
        ('t6', uint32_t),
        ('t5', uint32_t),
        ('t4', uint32_t),
        ('t3', uint32_t),
        ('a7', uint32_t),
        ('a6', uint32_t),
        ('a5', uint32_t),
        ('a4', uint32_t),
        ('a3', uint32_t),
        ('a2', uint32_t),
        ('a1', uint32_t),
        ('a0', uint32_t),
        ('t2', uint32_t),
        ('t1', uint32_t),
        ('t0', uint32_t),
        ('ra', uint32_t),
    ]
task_context_t=random_type_32
class random_type_31(Structure):
    _fields_ = [
        ('ccause', uint32_t),
        ('mcause', uint32_t),
        ('mtval', uint32_t),
        ('gp', uint32_t),
        ('task_context', task_context_t),
    ]
upg_func_malloc=td_u32
upg_func_free=td_u32
upg_func_serial_putc=td_u32
class upg_func(Structure):
    _fields_ = [
        ('malloc', upg_func_malloc),
        ('free', upg_func_free),
        ('serial_putc', upg_func_serial_putc),
    ]
upg_func_t=upg_func
osSemaphoreId_t=td_u32
osPriority_t=td_u32
TZ_ModuleId_t=td_u32
class random_type_34(Structure):
    _fields_ = [
        ('name', td_u32),
        ('attr_bits', uint32_t),
        ('cb_mem', td_u32),
        ('cb_size', uint32_t),
        ('stack_mem', td_u32),
        ('stack_size', uint32_t),
        ('priority', osPriority_t),
        ('tz_module', TZ_ModuleId_t),
        ('reserved', uint32_t),
    ]
osThreadAttr_t=random_type_34
osThreadFunc_t=td_u32
CreateQueueFunction_t=td_u32
class random_type_33(Structure):
    _fields_ = [
        ('attr', osThreadAttr_t),
        ('func', osThreadFunc_t),
        ('task_handle', td_u32),
        ('create_queue_handle', CreateQueueFunction_t),
        ('use_mem', uint32_t),
    ]
app_task_definition_t=random_type_33
class random_type_35(Structure):
    _fields_ = [
        ('name', td_u32),
        ('attr_bits', uint32_t),
        ('cb_mem', td_u32),
        ('cb_size', uint32_t),
    ]
class random_type_36(Structure):
    _fields_ = [
        ('attr', osThreadAttr_t),
        ('func', osThreadFunc_t),
        ('task_handle', td_u32),
        ('create_queue_handle', CreateQueueFunction_t),
        ('use_mem', uint32_t),
    ]
class random_type_37(Structure):
    _fields_ = [
        ('name', td_u32),
        ('attr_bits', uint32_t),
        ('cb_mem', td_u32),
        ('cb_size', uint32_t),
        ('stack_mem', td_u32),
        ('stack_size', uint32_t),
        ('priority', osPriority_t),
        ('tz_module', TZ_ModuleId_t),
        ('reserved', uint32_t),
    ]
class fota_upgrade_flag_area(Structure):
    _fields_ = [
        ('head_magic', uint32_t),
        ('head_before_offset', uint32_t),
        ('package_length', uint32_t),
        ('firmware_num', uint32_t),
        ('firmware_flag', ((uint8_t*3)*20)),
        ('nv_flag', (uint8_t*3)),
        ('ver_change_flag', uint8_t),
        ('update_result', uint32_t),
        ('nv_data_offset', uint32_t),
        ('nv_data_len', uint32_t),
        ('nv_hash_offset', uint32_t),
        ('nv_hash_len', uint32_t),
        ('complete_flag', uint32_t),
        ('head_end_magic', uint32_t),
    ]
fota_upgrade_flag_area_t=fota_upgrade_flag_area
reboot_cause_t=td_u16
critical_statistic_handler=td_u32
critical_record_handler=td_u32
SysInitcallFunc=td_u32
class random_type_38(Structure):
    _fields_ = [
        ('ccause', UINT32),
        ('mcause', UINT32),
        ('mtval', UINT32),
        ('gp', UINT32),
        ('taskContext', TaskContext),
    ]
class random_type_39(Structure):
    _fields_ = [
        ('mstatus', UINT32),
        ('mepc', UINT32),
        ('tp', UINT32),
        ('sp', UINT32),
        ('s11', UINT32),
        ('s10', UINT32),
        ('s9', UINT32),
        ('s8', UINT32),
        ('s7', UINT32),
        ('s6', UINT32),
        ('s5', UINT32),
        ('s4', UINT32),
        ('s3', UINT32),
        ('s2', UINT32),
        ('s1', UINT32),
        ('s0', UINT32),
        ('t6', UINT32),
        ('t5', UINT32),
        ('t4', UINT32),
        ('t3', UINT32),
        ('a7', UINT32),
        ('a6', UINT32),
        ('a5', UINT32),
        ('a4', UINT32),
        ('a3', UINT32),
        ('a2', UINT32),
        ('a1', UINT32),
        ('a0', UINT32),
        ('t2', UINT32),
        ('t1', UINT32),
        ('t0', UINT32),
        ('ra', UINT32),
        ('fs11', FLOATREG_TYPE),
        ('fs10', FLOATREG_TYPE),
        ('fs9', FLOATREG_TYPE),
        ('fs8', FLOATREG_TYPE),
        ('fs7', FLOATREG_TYPE),
        ('fs6', FLOATREG_TYPE),
        ('fs5', FLOATREG_TYPE),
        ('fs4', FLOATREG_TYPE),
        ('fs3', FLOATREG_TYPE),
        ('fs2', FLOATREG_TYPE),
        ('fs1', FLOATREG_TYPE),
        ('fs0', FLOATREG_TYPE),
        ('ft11', FLOATREG_TYPE),
        ('ft10', FLOATREG_TYPE),
        ('ft9', FLOATREG_TYPE),
        ('ft8', FLOATREG_TYPE),
        ('fa7', FLOATREG_TYPE),
        ('fa6', FLOATREG_TYPE),
        ('fa5', FLOATREG_TYPE),
        ('fa4', FLOATREG_TYPE),
        ('fa3', FLOATREG_TYPE),
        ('fa2', FLOATREG_TYPE),
        ('fa1', FLOATREG_TYPE),
        ('fa0', FLOATREG_TYPE),
        ('ft7', FLOATREG_TYPE),
        ('ft6', FLOATREG_TYPE),
        ('ft5', FLOATREG_TYPE),
        ('ft4', FLOATREG_TYPE),
        ('ft3', FLOATREG_TYPE),
        ('ft2', FLOATREG_TYPE),
        ('ft1', FLOATREG_TYPE),
        ('ft0', FLOATREG_TYPE),
        ('fcsr', UINT32),
        ('reserved', (UINT32*3)),
    ]
class random_type_40(Structure):
    _fields_ = [
        ('phase', UINT16),
        ('type', UINT16),
        ('faultAddr', UINT32),
        ('thrdPid', UINT32),
        ('nestCnt', UINT16),
        ('reserved', UINT16),
        ('context', td_u32),
    ]
ExcInfo=random_type_40
UINT8=td_uchar
class random_type_41(Structure):
    _fields_ = [
        ('mstatus', UINT32),
        ('mepc', UINT32),
        ('tp', UINT32),
        ('sp', UINT32),
        ('s11', UINT32),
        ('s10', UINT32),
        ('s9', UINT32),
        ('s8', UINT32),
        ('s7', UINT32),
        ('s6', UINT32),
        ('s5', UINT32),
        ('s4', UINT32),
        ('s3', UINT32),
        ('s2', UINT32),
        ('s1', UINT32),
        ('s0', UINT32),
        ('t6', UINT32),
        ('t5', UINT32),
        ('t4', UINT32),
        ('t3', UINT32),
        ('a7', UINT32),
        ('a6', UINT32),
        ('a5', UINT32),
        ('a4', UINT32),
        ('a3', UINT32),
        ('a2', UINT32),
        ('a1', UINT32),
        ('a0', UINT32),
        ('t2', UINT32),
        ('t1', UINT32),
        ('t0', UINT32),
        ('ra', UINT32),
        ('fs11', FLOATREG_TYPE),
        ('fs10', FLOATREG_TYPE),
        ('fs9', FLOATREG_TYPE),
        ('fs8', FLOATREG_TYPE),
        ('fs7', FLOATREG_TYPE),
        ('fs6', FLOATREG_TYPE),
        ('fs5', FLOATREG_TYPE),
        ('fs4', FLOATREG_TYPE),
        ('fs3', FLOATREG_TYPE),
        ('fs2', FLOATREG_TYPE),
        ('fs1', FLOATREG_TYPE),
        ('fs0', FLOATREG_TYPE),
        ('ft11', FLOATREG_TYPE),
        ('ft10', FLOATREG_TYPE),
        ('ft9', FLOATREG_TYPE),
        ('ft8', FLOATREG_TYPE),
        ('fa7', FLOATREG_TYPE),
        ('fa6', FLOATREG_TYPE),
        ('fa5', FLOATREG_TYPE),
        ('fa4', FLOATREG_TYPE),
        ('fa3', FLOATREG_TYPE),
        ('fa2', FLOATREG_TYPE),
        ('fa1', FLOATREG_TYPE),
        ('fa0', FLOATREG_TYPE),
        ('ft7', FLOATREG_TYPE),
        ('ft6', FLOATREG_TYPE),
        ('ft5', FLOATREG_TYPE),
        ('ft4', FLOATREG_TYPE),
        ('ft3', FLOATREG_TYPE),
        ('ft2', FLOATREG_TYPE),
        ('ft1', FLOATREG_TYPE),
        ('ft0', FLOATREG_TYPE),
        ('fcsr', UINT32),
        ('reserved', (UINT32*3)),
    ]
INT32=td_s32
class random_type_42(Structure):
    _fields_ = [
        ('sortLink', td_u32),
        ('cursor', UINT16),
        ('reserved', UINT16),
    ]
LOWPOWERIDLEHOOK=td_u32
BOOL=td_u32
class random_type_43(Structure):
    _fields_ = [
        ('name', td_u32),
        ('attr_bits', uint32_t),
        ('cb_mem', td_u32),
        ('cb_size', uint32_t),
    ]
class random_type_44(Structure):
    _fields_ = [
        ('sortLinkNode', LOS_DL_LIST),
        ('idxRollNum', UINT32),
    ]
class random_type_45(Structure):
    _fields_ = [
        ('queueHandle', td_u32),
        ('queueState', UINT8),
        ('queueMemType', UINT8),
        ('queueLen', UINT16),
        ('queueSize', UINT16),
        ('queueId', UINT32),
        ('queueHead', UINT16),
        ('queueTail', UINT16),
        ('readWriteableCnt', (UINT16*2)),
        ('readWriteList', (LOS_DL_LIST*2)),
        ('memList', LOS_DL_LIST),
    ]
LosQueueCB=random_type_45
class random_type_46(Structure):
    _fields_ = [
        ('muxList', LOS_DL_LIST),
        ('owner', td_u32),
        ('muxCount', UINT16),
        ('muxStat', UINT8),
        ('muxId', UINT32),
    ]
LosMuxCB=random_type_46
class random_type_47(Structure):
    _fields_ = [
        ('semStat', UINT8),
        ('semType', UINT8),
        ('semCount', UINT16),
        ('semId', UINT32),
        ('semList', LOS_DL_LIST),
    ]
LosSemCB=random_type_47
SWTMR_PROC_FUNC=td_u32
class random_type_48(Structure):
    _fields_ = [
        ('sortList', SortLinkList),
        ('state', UINT8),
        ('mode', UINT8),
        ('overrun', UINT8),
        ('inProcess', UINT8),
        ('timerId', UINT16),
        ('interval', UINT32),
        ('expiry', UINT32),
        ('arg', UINTPTR),
        ('handler', SWTMR_PROC_FUNC),
    ]
LosSwtmrCB=random_type_48
class tagTskInitParam(Structure):
    _fields_ = [
        ('pfnTaskEntry', TSK_ENTRY_FUNC),
        ('usTaskPrio', UINT16),
        ('pArgs', td_u32),
        ('uwStackSize', UINT32),
        ('pcName', td_u32),
        ('uwResved', UINT32),
    ]
TSK_INIT_PARAM_S=tagTskInitParam
class random_type_49(Structure):
    _fields_ = [
        ('sortLinkNode', LOS_DL_LIST),
        ('idxRollNum', UINT32),
    ]
class Spinlock(Structure):
    _fields_ = [
        ('rawLock', size_t),
    ]
SPIN_LOCK_S=Spinlock
PEVENT_CB_S=td_u32
class random_type_50(Structure):
    _fields_ = [
        ('realValue', td_u32),
        ('value', INT32),
        ('clearEvent', UINT32),
    ]
EventCond=random_type_50
class random_type_51(Structure):
    _fields_ = [
        ('triggerIrq', td_u32),
        ('clearIrq', td_u32),
        ('enableIrq', td_u32),
        ('disableIrq', td_u32),
        ('setIrqPriority', td_u32),
        ('getCurIrqNum', td_u32),
        ('getIrqVersion', td_u32),
        ('getHandleForm', td_u32),
        ('handleIrq', td_u32),
    ]
HwiControllerOps=random_type_51
HWI_HANDLE_T=td_u32
HWI_PRIOR_T=td_u16
HWI_PROC_FUNC=td_u32
HWI_ARG_T=td_u32
class random_type_53(Union):
    _fields_ = [
        ('shareMode', HWI_ARG_T),
        ('registerInfo', HWI_ARG_T),
    ]
class tagHwiHandleForm(Structure):
    _fields_ = [
        ('hook', HWI_PROC_FUNC),
        ('random_var_52', random_type_53),
        ('respCount', UINT32),
    ]
HwiHandleInfo=tagHwiHandleForm
HWI_PROC_FUNC2=td_u32
HWI_PROC_FUNC0=td_u32
class tagIrqParam(Structure):
    _fields_ = [
        ('swIrq', td_s32),
        ('pDevId', td_u32),
        ('pName', td_u32),
    ]
HWI_IRQ_PARAM_S=tagIrqParam
HWI_MODE_T=td_u16
class random_type_54(Structure):
    _fields_ = [
        ('muxList', LOS_DL_LIST),
        ('owner', td_u32),
        ('muxCount', UINT16),
        ('muxStat', UINT8),
        ('muxId', UINT32),
    ]
class random_type_55(Structure):
    _fields_ = [
        ('sortLinkNode', LOS_DL_LIST),
        ('idxRollNum', UINT32),
    ]
class random_type_56(Structure):
    _fields_ = [
        ('muxList', LOS_DL_LIST),
        ('owner', td_u32),
        ('muxCount', UINT16),
    ]
MuxBaseCB=random_type_56
class random_type_58(Structure):
    _fields_ = [
        ('sortLink', td_u32),
        ('cursor', UINT16),
        ('reserved', UINT16),
    ]
SortLinkAttribute=random_type_58
class random_type_59(Structure):
    _fields_ = [
        ('head', UINT16),
        ('tail', UINT16),
        ('free', UINT32),
        ('buf', td_u32),
    ]
ExpiryList=random_type_59
class random_type_57(Structure):
    _fields_ = [
        ('taskSortLink', SortLinkAttribute),
        ('swtmrSortLink', SortLinkAttribute),
        ('expiryList', ExpiryList),
        ('idleTaskId', UINT32),
        ('taskLockCnt', UINT32),
        ('swtmrHandlerQueue', UINT32),
        ('swtmrTaskId', UINT32),
        ('schedFlag', UINT32),
    ]
Percpu=random_type_57
class random_type_60(Structure):
    _fields_ = [
        ('queueHandle', td_u32),
        ('queueState', UINT8),
        ('queueMemType', UINT8),
        ('queueLen', UINT16),
        ('queueSize', UINT16),
        ('queueId', UINT32),
        ('queueHead', UINT16),
        ('queueTail', UINT16),
        ('readWriteableCnt', (UINT16*2)),
        ('readWriteList', (LOS_DL_LIST*2)),
        ('memList', LOS_DL_LIST),
    ]
class random_type_61(Structure):
    _fields_ = [
        ('sortLinkNode', LOS_DL_LIST),
        ('idxRollNum', UINT32),
    ]
QueueMsgHead=td_u32
class random_type_62(Structure):
    _fields_ = [
        ('semStat', UINT8),
        ('semType', UINT8),
        ('semCount', UINT16),
        ('semId', UINT32),
        ('semList', LOS_DL_LIST),
    ]
class random_type_63(Structure):
    _fields_ = [
        ('sortLinkNode', LOS_DL_LIST),
        ('idxRollNum', UINT32),
    ]
class random_type_64(Structure):
    _fields_ = [
        ('sortLinkNode', LOS_DL_LIST),
        ('idxRollNum', UINT32),
    ]
class random_type_65(Structure):
    _fields_ = [
        ('sortLink', td_u32),
        ('cursor', UINT16),
        ('reserved', UINT16),
    ]
class random_type_66(Structure):
    _fields_ = [
        ('stackTop', td_u32),
        ('stackSize', UINT32),
        ('stackName', td_u32),
    ]
StackInfo=random_type_66
class random_type_67(Structure):
    _fields_ = [
        ('sortList', SortLinkList),
        ('state', UINT8),
        ('mode', UINT8),
        ('overrun', UINT8),
        ('inProcess', UINT8),
        ('timerId', UINT16),
        ('interval', UINT32),
        ('expiry', UINT32),
        ('arg', UINTPTR),
        ('handler', SWTMR_PROC_FUNC),
    ]
class random_type_68(Structure):
    _fields_ = [
        ('sortLinkNode', LOS_DL_LIST),
        ('idxRollNum', UINT32),
    ]
class random_type_69(Structure):
    _fields_ = [
        ('sortLink', td_u32),
        ('cursor', UINT16),
        ('reserved', UINT16),
    ]
TASK_RECYCLE_HOOK=td_u32
TASK_DELETE_HOOK=td_u32
TSKSWITCHHOOK=td_u32
IDLEHANDLERHOOK=td_u32
class random_type_70(Structure):
    _fields_ = [
        ('sortLinkNode', LOS_DL_LIST),
        ('idxRollNum', UINT32),
    ]
class random_type_71(Structure):
    _fields_ = [
        ('sortLink', td_u32),
        ('cursor', UINT16),
        ('reserved', UINT16),
    ]
class random_type_72(Structure):
    _fields_ = [
        ('taskSortLink', SortLinkAttribute),
        ('swtmrSortLink', SortLinkAttribute),
        ('expiryList', ExpiryList),
        ('idleTaskId', UINT32),
        ('taskLockCnt', UINT32),
        ('swtmrHandlerQueue', UINT32),
        ('swtmrTaskId', UINT32),
        ('schedFlag', UINT32),
    ]
class random_type_73(Structure):
    _fields_ = [
        ('head', UINT16),
        ('tail', UINT16),
        ('free', UINT32),
        ('buf', td_u32),
    ]
class tagTskInfo(Structure):
    _fields_ = [
        ('acName', (CHAR*32)),
        ('uwTaskID', UINT32),
        ('usTaskStatus', UINT16),
        ('usTaskPrio', UINT16),
        ('pTaskSem', td_u32),
        ('pTaskMux', td_u32),
        ('uwEvent', EVENT_CB_S),
        ('uwEventMask', UINT32),
        ('uwStackSize', UINT32),
        ('uwTopOfStack', UINTPTR),
        ('uwBottomOfStack', UINTPTR),
        ('uwSP', UINTPTR),
        ('uwCurrUsed', UINT32),
        ('uwPeakUsed', UINT32),
        ('bOvf', BOOL),
    ]
TSK_INFO_S=tagTskInfo
DOUBLE=td_double
MALLOC_HOOK=td_u32
class random_type_76(Structure):
    _fields_ = [
        ('memUsed', UINT32),
        ('memPeak', UINT32),
    ]
TaskMemUsedInfo=random_type_76
class random_type_75(Structure):
    _fields_ = [
        ('memTotalUsed', UINT32),
        ('memTotalPeak', UINT32),
        ('taskMemstats', (TaskMemUsedInfo*41)),
    ]
Memstat=random_type_75
class tagOsSlabMem(Structure):
    _fields_ = [
        ('blkSz', UINT32),
        ('blkCnt', UINT32),
        ('blkUsedCnt', UINT32),
        ('alloc', td_u32),
    ]
OsSlabMem=tagOsSlabMem
class LosSlabControlHeader(Structure):
    _fields_ = [
        ('enabled', BOOL),
        ('slabClass', (OsSlabMem*4)),
    ]
class random_type_74(Structure):
    _fields_ = [
        ('pool', td_u32),
        ('poolSize', UINT32),
        ('stat', Memstat),
        ('slabCtrlHdr', LosSlabControlHeader),
    ]
LosMemPoolInfo=random_type_74
class tagOsSlabAllocator(Structure):
    _fields_ = [
        ('itemSz', UINT32),
        ('dataChunks', td_u32),
        ('bitset', td_u32),
    ]
OsSlabAllocator=tagOsSlabAllocator
class AtomicBitset(Structure):
    _fields_ = [
        ('numBits', UINT32),
        ('words', (UINT32*0)),
    ]
class random_type_79(Structure):
    _fields_ = [
        ('magic', UINTPTR),
        ('taskId', UINTPTR,16),
    ]
class random_type_80(Union):
    _fields_ = [
        ('freeNodeInfo', LOS_DL_LIST),
        ('random_var_78', random_type_79),
    ]
class random_type_81(Structure):
    _fields_ = [
        ('random_var_77', random_type_80),
        ('preNode', td_u32),
        ('sizeAndFlag', UINT32),
    ]
LosMemCtlNode=random_type_81
class tagLosMemDynNode(Structure):
    _fields_ = [
        ('selfNode', LosMemCtlNode),
    ]
LosMemDynNode=tagLosMemDynNode
class random_type_82(Structure):
    _fields_ = [
        ('listHead', (LOS_DL_LIST*26)),
    ]
LosMultipleDlinkHead=random_type_82
class random_type_83(Structure):
    _fields_ = [
        ('sortLinkNode', LOS_DL_LIST),
        ('idxRollNum', UINT32),
    ]
class random_type_84(Structure):
    _fields_ = [
        ('uwTotalUsedSize', UINT32),
        ('uwTotalFreeSize', UINT32),
        ('uwMaxFreeNodeSize', UINT32),
        ('uwUsedNodeNum', UINT32),
        ('uwFreeNodeNum', UINT32),
        ('uwUsageWaterLine', UINT32),
    ]
LOS_MEM_POOL_STATUS=random_type_84
class random_type_85(Structure):
    _fields_ = [
        ('listHead', (LOS_DL_LIST*26)),
    ]
class random_type_86(Structure):
    _fields_ = [
        ('pool', td_u32),
        ('poolSize', UINT32),
        ('stat', Memstat),
        ('slabCtrlHdr', LosSlabControlHeader),
    ]
class random_type_87(Structure):
    _fields_ = [
        ('memTotalUsed', UINT32),
        ('memTotalPeak', UINT32),
        ('taskMemstats', (TaskMemUsedInfo*41)),
    ]
class random_type_88(Structure):
    _fields_ = [
        ('memUsed', UINT32),
        ('memPeak', UINT32),
    ]
class tagOsSlabBlockNode(Structure):
    _fields_ = [
        ('magic', UINT16),
        ('blkSz', UINT8),
        ('recordId', UINT8),
    ]
OsSlabBlockNode=tagOsSlabBlockNode
class random_type_89(Structure):
    _fields_ = [
        ('pool', td_u32),
        ('poolSize', UINT32),
        ('stat', Memstat),
        ('slabCtrlHdr', LosSlabControlHeader),
    ]
class random_type_90(Structure):
    _fields_ = [
        ('memTotalUsed', UINT32),
        ('memTotalPeak', UINT32),
        ('taskMemstats', (TaskMemUsedInfo*41)),
    ]
class random_type_91(Structure):
    _fields_ = [
        ('memUsed', UINT32),
        ('memPeak', UINT32),
    ]
class tagLosSlabStatus(Structure):
    _fields_ = [
        ('totalSize', UINT32),
        ('usedSize', UINT32),
        ('freeSize', UINT32),
        ('allocCount', UINT32),
        ('freeCount', UINT32),
    ]
LosSlabStatus=tagLosSlabStatus
class random_type_92(Structure):
    _fields_ = [
        ('sortLinkNode', LOS_DL_LIST),
        ('idxRollNum', UINT32),
    ]
class random_type_93(Structure):
    _fields_ = [
        ('sortLinkNode', LOS_DL_LIST),
        ('idxRollNum', UINT32),
    ]
class random_type_94(Structure):
    _fields_ = [
        ('taskSortLink', SortLinkAttribute),
        ('swtmrSortLink', SortLinkAttribute),
        ('expiryList', ExpiryList),
        ('idleTaskId', UINT32),
        ('taskLockCnt', UINT32),
        ('swtmrHandlerQueue', UINT32),
        ('swtmrTaskId', UINT32),
        ('schedFlag', UINT32),
    ]
class random_type_95(Structure):
    _fields_ = [
        ('sortLink', td_u32),
        ('cursor', UINT16),
        ('reserved', UINT16),
    ]
class random_type_96(Structure):
    _fields_ = [
        ('head', UINT16),
        ('tail', UINT16),
        ('free', UINT32),
        ('buf', td_u32),
    ]
class random_type_97(Structure):
    _fields_ = [
        ('strLen', UINT32),
        ('symNum', UINT32),
        ('strTotalLen', UINT32),
        ('symTotalNum', UINT32),
        ('symTbl', td_u32),
        ('strTbl', td_u32),
    ]
SymbolManage=random_type_97
class random_type_98(Structure):
    _fields_ = [
        ('name', td_u32),
        ('addr', td_u32),
    ]
LDSymbol=random_type_98
INT8=td_char
class random_type_99(Structure):
    _fields_ = [
        ('bucket', td_u32),
        ('bucketSize', UINT32),
        ('chain', td_u32),
        ('chainSize', UINT32),
    ]
LDHashInfo=random_type_99
class random_type_100(Structure):
    _fields_ = [
        ('stName', UINT32),
        ('stValue', UINT32),
        ('stSize', UINT32),
        ('stInfo', UINT8),
        ('stOther', UINT8),
        ('stShndx', UINT16),
    ]
LDElf32Sym=random_type_100
class random_type_101(Structure):
    _fields_ = [
        ('buf', (td_uchar*2)),
    ]
SecStrBuf2=random_type_101
class random_type_102(Structure):
    _fields_ = [
        ('buf', (td_uchar*3)),
    ]
SecStrBuf3=random_type_102
class random_type_103(Structure):
    _fields_ = [
        ('buf', (td_uchar*4)),
    ]
SecStrBuf4=random_type_103
class random_type_104(Structure):
    _fields_ = [
        ('buf', (td_uchar*5)),
    ]
SecStrBuf5=random_type_104
class random_type_105(Structure):
    _fields_ = [
        ('buf', (td_uchar*6)),
    ]
SecStrBuf6=random_type_105
class random_type_106(Structure):
    _fields_ = [
        ('buf', (td_uchar*7)),
    ]
SecStrBuf7=random_type_106
class random_type_107(Structure):
    _fields_ = [
        ('buf', (td_uchar*8)),
    ]
SecStrBuf8=random_type_107
class random_type_108(Structure):
    _fields_ = [
        ('buf', (td_uchar*9)),
    ]
SecStrBuf9=random_type_108
class random_type_109(Structure):
    _fields_ = [
        ('buf', (td_uchar*10)),
    ]
SecStrBuf10=random_type_109
class random_type_110(Structure):
    _fields_ = [
        ('buf', (td_uchar*11)),
    ]
SecStrBuf11=random_type_110
class random_type_111(Structure):
    _fields_ = [
        ('buf', (td_uchar*12)),
    ]
SecStrBuf12=random_type_111
class random_type_112(Structure):
    _fields_ = [
        ('buf', (td_uchar*13)),
    ]
SecStrBuf13=random_type_112
class random_type_113(Structure):
    _fields_ = [
        ('buf', (td_uchar*14)),
    ]
SecStrBuf14=random_type_113
class random_type_114(Structure):
    _fields_ = [
        ('buf', (td_uchar*15)),
    ]
SecStrBuf15=random_type_114
class random_type_115(Structure):
    _fields_ = [
        ('buf', (td_uchar*16)),
    ]
SecStrBuf16=random_type_115
class random_type_116(Structure):
    _fields_ = [
        ('buf', (td_uchar*17)),
    ]
SecStrBuf17=random_type_116
class random_type_117(Structure):
    _fields_ = [
        ('buf', (td_uchar*18)),
    ]
SecStrBuf18=random_type_117
class random_type_118(Structure):
    _fields_ = [
        ('buf', (td_uchar*19)),
    ]
SecStrBuf19=random_type_118
class random_type_119(Structure):
    _fields_ = [
        ('buf', (td_uchar*20)),
    ]
SecStrBuf20=random_type_119
class random_type_120(Structure):
    _fields_ = [
        ('buf', (td_uchar*21)),
    ]
SecStrBuf21=random_type_120
class random_type_121(Structure):
    _fields_ = [
        ('buf', (td_uchar*22)),
    ]
SecStrBuf22=random_type_121
class random_type_122(Structure):
    _fields_ = [
        ('buf', (td_uchar*23)),
    ]
SecStrBuf23=random_type_122
class random_type_123(Structure):
    _fields_ = [
        ('buf', (td_uchar*24)),
    ]
SecStrBuf24=random_type_123
class random_type_124(Structure):
    _fields_ = [
        ('buf', (td_uchar*25)),
    ]
SecStrBuf25=random_type_124
class random_type_125(Structure):
    _fields_ = [
        ('buf', (td_uchar*26)),
    ]
SecStrBuf26=random_type_125
class random_type_126(Structure):
    _fields_ = [
        ('buf', (td_uchar*27)),
    ]
SecStrBuf27=random_type_126
class random_type_127(Structure):
    _fields_ = [
        ('buf', (td_uchar*28)),
    ]
SecStrBuf28=random_type_127
class random_type_128(Structure):
    _fields_ = [
        ('buf', (td_uchar*29)),
    ]
SecStrBuf29=random_type_128
class random_type_129(Structure):
    _fields_ = [
        ('buf', (td_uchar*30)),
    ]
SecStrBuf30=random_type_129
class random_type_130(Structure):
    _fields_ = [
        ('buf', (td_uchar*31)),
    ]
SecStrBuf31=random_type_130
class random_type_131(Structure):
    _fields_ = [
        ('buf', (td_uchar*32)),
    ]
SecStrBuf32=random_type_131
class random_type_132(Structure):
    _fields_ = [
        ('buf', (td_uchar*33)),
    ]
SecStrBuf33=random_type_132
class random_type_133(Structure):
    _fields_ = [
        ('buf', (td_uchar*34)),
    ]
SecStrBuf34=random_type_133
class random_type_134(Structure):
    _fields_ = [
        ('buf', (td_uchar*35)),
    ]
SecStrBuf35=random_type_134
class random_type_135(Structure):
    _fields_ = [
        ('buf', (td_uchar*36)),
    ]
SecStrBuf36=random_type_135
class random_type_136(Structure):
    _fields_ = [
        ('buf', (td_uchar*37)),
    ]
SecStrBuf37=random_type_136
class random_type_137(Structure):
    _fields_ = [
        ('buf', (td_uchar*38)),
    ]
SecStrBuf38=random_type_137
class random_type_138(Structure):
    _fields_ = [
        ('buf', (td_uchar*39)),
    ]
SecStrBuf39=random_type_138
class random_type_139(Structure):
    _fields_ = [
        ('buf', (td_uchar*40)),
    ]
SecStrBuf40=random_type_139
class random_type_140(Structure):
    _fields_ = [
        ('buf', (td_uchar*41)),
    ]
SecStrBuf41=random_type_140
class random_type_141(Structure):
    _fields_ = [
        ('buf', (td_uchar*42)),
    ]
SecStrBuf42=random_type_141
class random_type_142(Structure):
    _fields_ = [
        ('buf', (td_uchar*43)),
    ]
SecStrBuf43=random_type_142
class random_type_143(Structure):
    _fields_ = [
        ('buf', (td_uchar*44)),
    ]
SecStrBuf44=random_type_143
class random_type_144(Structure):
    _fields_ = [
        ('buf', (td_uchar*45)),
    ]
SecStrBuf45=random_type_144
class random_type_145(Structure):
    _fields_ = [
        ('buf', (td_uchar*46)),
    ]
SecStrBuf46=random_type_145
class random_type_146(Structure):
    _fields_ = [
        ('buf', (td_uchar*47)),
    ]
SecStrBuf47=random_type_146
class random_type_147(Structure):
    _fields_ = [
        ('buf', (td_uchar*48)),
    ]
SecStrBuf48=random_type_147
class random_type_148(Structure):
    _fields_ = [
        ('buf', (td_uchar*49)),
    ]
SecStrBuf49=random_type_148
class random_type_149(Structure):
    _fields_ = [
        ('buf', (td_uchar*50)),
    ]
SecStrBuf50=random_type_149
class random_type_150(Structure):
    _fields_ = [
        ('buf', (td_uchar*51)),
    ]
SecStrBuf51=random_type_150
class random_type_151(Structure):
    _fields_ = [
        ('buf', (td_uchar*52)),
    ]
SecStrBuf52=random_type_151
class random_type_152(Structure):
    _fields_ = [
        ('buf', (td_uchar*53)),
    ]
SecStrBuf53=random_type_152
class random_type_153(Structure):
    _fields_ = [
        ('buf', (td_uchar*54)),
    ]
SecStrBuf54=random_type_153
class random_type_154(Structure):
    _fields_ = [
        ('buf', (td_uchar*55)),
    ]
SecStrBuf55=random_type_154
class random_type_155(Structure):
    _fields_ = [
        ('buf', (td_uchar*56)),
    ]
SecStrBuf56=random_type_155
class random_type_156(Structure):
    _fields_ = [
        ('buf', (td_uchar*57)),
    ]
SecStrBuf57=random_type_156
class random_type_157(Structure):
    _fields_ = [
        ('buf', (td_uchar*58)),
    ]
SecStrBuf58=random_type_157
class random_type_158(Structure):
    _fields_ = [
        ('buf', (td_uchar*59)),
    ]
SecStrBuf59=random_type_158
class random_type_159(Structure):
    _fields_ = [
        ('buf', (td_uchar*60)),
    ]
SecStrBuf60=random_type_159
class random_type_160(Structure):
    _fields_ = [
        ('buf', (td_uchar*61)),
    ]
SecStrBuf61=random_type_160
class random_type_161(Structure):
    _fields_ = [
        ('buf', (td_uchar*62)),
    ]
SecStrBuf62=random_type_161
class random_type_162(Structure):
    _fields_ = [
        ('buf', (td_uchar*63)),
    ]
SecStrBuf63=random_type_162
class random_type_163(Structure):
    _fields_ = [
        ('buf', (td_uchar*64)),
    ]
SecStrBuf64=random_type_163
class random_type_164(Structure):
    _fields_ = [
        ('buf', (td_uchar*32)),
    ]
class random_type_165(Structure):
    _fields_ = [
        ('buf', (td_uchar*2)),
    ]
class random_type_166(Structure):
    _fields_ = [
        ('buf', (td_uchar*3)),
    ]
class random_type_167(Structure):
    _fields_ = [
        ('buf', (td_uchar*4)),
    ]
class random_type_168(Structure):
    _fields_ = [
        ('buf', (td_uchar*5)),
    ]
class random_type_169(Structure):
    _fields_ = [
        ('buf', (td_uchar*6)),
    ]
class random_type_170(Structure):
    _fields_ = [
        ('buf', (td_uchar*7)),
    ]
class random_type_171(Structure):
    _fields_ = [
        ('buf', (td_uchar*8)),
    ]
class random_type_172(Structure):
    _fields_ = [
        ('buf', (td_uchar*9)),
    ]
class random_type_173(Structure):
    _fields_ = [
        ('buf', (td_uchar*10)),
    ]
class random_type_174(Structure):
    _fields_ = [
        ('buf', (td_uchar*11)),
    ]
class random_type_175(Structure):
    _fields_ = [
        ('buf', (td_uchar*12)),
    ]
class random_type_176(Structure):
    _fields_ = [
        ('buf', (td_uchar*13)),
    ]
class random_type_177(Structure):
    _fields_ = [
        ('buf', (td_uchar*14)),
    ]
class random_type_178(Structure):
    _fields_ = [
        ('buf', (td_uchar*15)),
    ]
class random_type_179(Structure):
    _fields_ = [
        ('buf', (td_uchar*16)),
    ]
class random_type_180(Structure):
    _fields_ = [
        ('buf', (td_uchar*17)),
    ]
class random_type_181(Structure):
    _fields_ = [
        ('buf', (td_uchar*18)),
    ]
class random_type_182(Structure):
    _fields_ = [
        ('buf', (td_uchar*19)),
    ]
class random_type_183(Structure):
    _fields_ = [
        ('buf', (td_uchar*20)),
    ]
class random_type_184(Structure):
    _fields_ = [
        ('buf', (td_uchar*21)),
    ]
class random_type_185(Structure):
    _fields_ = [
        ('buf', (td_uchar*22)),
    ]
class random_type_186(Structure):
    _fields_ = [
        ('buf', (td_uchar*23)),
    ]
class random_type_187(Structure):
    _fields_ = [
        ('buf', (td_uchar*24)),
    ]
class random_type_188(Structure):
    _fields_ = [
        ('buf', (td_uchar*25)),
    ]
class random_type_189(Structure):
    _fields_ = [
        ('buf', (td_uchar*26)),
    ]
class random_type_190(Structure):
    _fields_ = [
        ('buf', (td_uchar*27)),
    ]
class random_type_191(Structure):
    _fields_ = [
        ('buf', (td_uchar*28)),
    ]
class random_type_192(Structure):
    _fields_ = [
        ('buf', (td_uchar*29)),
    ]
class random_type_193(Structure):
    _fields_ = [
        ('buf', (td_uchar*30)),
    ]
class random_type_194(Structure):
    _fields_ = [
        ('buf', (td_uchar*31)),
    ]
class random_type_195(Union):
    _fields_ = [
        ('buf32', SecStrBuf32),
        ('buf31', SecStrBuf31),
        ('buf30', SecStrBuf30),
        ('buf29', SecStrBuf29),
        ('buf28', SecStrBuf28),
        ('buf27', SecStrBuf27),
        ('buf26', SecStrBuf26),
        ('buf25', SecStrBuf25),
        ('buf24', SecStrBuf24),
        ('buf23', SecStrBuf23),
        ('buf22', SecStrBuf22),
        ('buf21', SecStrBuf21),
        ('buf20', SecStrBuf20),
        ('buf19', SecStrBuf19),
        ('buf18', SecStrBuf18),
        ('buf17', SecStrBuf17),
        ('buf16', SecStrBuf16),
        ('buf15', SecStrBuf15),
        ('buf14', SecStrBuf14),
        ('buf13', SecStrBuf13),
        ('buf12', SecStrBuf12),
        ('buf11', SecStrBuf11),
        ('buf10', SecStrBuf10),
        ('buf9', SecStrBuf9),
        ('buf8', SecStrBuf8),
        ('buf7', SecStrBuf7),
        ('buf6', SecStrBuf6),
        ('buf5', SecStrBuf5),
        ('buf4', SecStrBuf4),
        ('buf3', SecStrBuf3),
        ('buf2', SecStrBuf2),
    ]
SecStrBuf32Union=random_type_195
SecInt=td_s32
SecUnsignedChar=td_uchar
wchar_t=td_s32
SecChar=td_char
SecUnsignedInt=td_u32
class random_type_197(Structure):
    _fields_ = [
        ('floatStrTotalLen', size_t),
        ('floatStrUsedLen', size_t),
        ('floatStr', td_u32),
        ('allocatedFloatStr', td_u32),
        ('buffer', (SecChar*350)),
    ]
SecFloatSpec=random_type_197
class _IO_FILE(Structure):
    _fields_ = [
    ]
FILE=_IO_FILE
class random_type_199(Structure):
    _fields_ = [
        ('table', (td_uchar*32)),
        ('mask', td_uchar),
    ]
SecBracketTable=random_type_199
ptrdiff_t=td_s32
SecUnsignedInt32=td_u32
class random_type_200(Structure):
    _fields_ = [
        ('count', td_s32),
        ('cur', td_u32),
    ]
SecPrintfStream=random_type_200
class random_type_202(Union):
    _fields_ = [
        ('str', td_u32),
        ('wStr', td_u32),
    ]
SecFormatBuf=random_type_202
class random_type_203(Union):
    _fields_ = [
        ('str', (td_char*513)),
        ('wStr', (wchar_t*4)),
    ]
SecBuffer=random_type_203
class random_type_201(Structure):
    _fields_ = [
        ('digits', td_u32),
        ('text', SecFormatBuf),
        ('textLen', td_s32),
        ('textIsWide', td_s32),
        ('radix', td_u32),
        ('flags', td_u32),
        ('fldWidth', td_s32),
        ('precision', td_s32),
        ('dynWidth', td_s32),
        ('dynPrecision', td_s32),
        ('padding', td_s32),
        ('prefixLen', td_s32),
        ('prefix', (SecChar*2)),
        ('buffer', SecBuffer),
    ]
SecFormatAttr=random_type_201
class random_type_204(Structure):
    _fields_ = [
        ('buffer', (td_char*16)),
        ('fmtStr', td_u32),
        ('allocatedFmtStr', td_u32),
        ('floatBuffer', td_u32),
        ('bufferSize', td_s32),
    ]
SecFloatAdapt=random_type_204
class random_type_205(Structure):
    _fields_ = [
        ('buf', (td_uchar*2)),
    ]
class random_type_206(Structure):
    _fields_ = [
        ('buf', (td_uchar*3)),
    ]
class random_type_207(Structure):
    _fields_ = [
        ('buf', (td_uchar*4)),
    ]
class random_type_208(Structure):
    _fields_ = [
        ('buf', (td_uchar*5)),
    ]
class random_type_209(Structure):
    _fields_ = [
        ('buf', (td_uchar*6)),
    ]
class random_type_210(Structure):
    _fields_ = [
        ('buf', (td_uchar*7)),
    ]
class random_type_211(Structure):
    _fields_ = [
        ('buf', (td_uchar*8)),
    ]
class random_type_212(Structure):
    _fields_ = [
        ('buf', (td_uchar*9)),
    ]
class random_type_213(Structure):
    _fields_ = [
        ('buf', (td_uchar*10)),
    ]
class random_type_214(Structure):
    _fields_ = [
        ('buf', (td_uchar*11)),
    ]
class random_type_215(Structure):
    _fields_ = [
        ('buf', (td_uchar*12)),
    ]
class random_type_216(Structure):
    _fields_ = [
        ('buf', (td_uchar*13)),
    ]
class random_type_217(Structure):
    _fields_ = [
        ('buf', (td_uchar*14)),
    ]
class random_type_218(Structure):
    _fields_ = [
        ('buf', (td_uchar*15)),
    ]
class random_type_219(Structure):
    _fields_ = [
        ('buf', (td_uchar*16)),
    ]
class random_type_220(Structure):
    _fields_ = [
        ('buf', (td_uchar*17)),
    ]
class random_type_221(Structure):
    _fields_ = [
        ('buf', (td_uchar*18)),
    ]
class random_type_222(Structure):
    _fields_ = [
        ('buf', (td_uchar*19)),
    ]
class random_type_223(Structure):
    _fields_ = [
        ('buf', (td_uchar*20)),
    ]
class random_type_224(Structure):
    _fields_ = [
        ('buf', (td_uchar*21)),
    ]
class random_type_225(Structure):
    _fields_ = [
        ('buf', (td_uchar*22)),
    ]
class random_type_226(Structure):
    _fields_ = [
        ('buf', (td_uchar*23)),
    ]
class random_type_227(Structure):
    _fields_ = [
        ('buf', (td_uchar*24)),
    ]
class random_type_228(Structure):
    _fields_ = [
        ('buf', (td_uchar*25)),
    ]
class random_type_229(Structure):
    _fields_ = [
        ('buf', (td_uchar*26)),
    ]
class random_type_230(Structure):
    _fields_ = [
        ('buf', (td_uchar*27)),
    ]
class random_type_231(Structure):
    _fields_ = [
        ('buf', (td_uchar*28)),
    ]
class random_type_232(Structure):
    _fields_ = [
        ('buf', (td_uchar*29)),
    ]
class random_type_233(Structure):
    _fields_ = [
        ('buf', (td_uchar*30)),
    ]
class random_type_234(Structure):
    _fields_ = [
        ('buf', (td_uchar*31)),
    ]
class random_type_235(Structure):
    _fields_ = [
        ('buf', (td_uchar*32)),
    ]
locale_t=td_u32
class __locale_struct(Structure):
    _fields_ = [
    ]
class errmsgstr_t(Structure):
    _fields_ = [
        ('str0', (td_char*21)),
        ('strEILSEQ', (td_char*22)),
        ('strEDOM', (td_char*13)),
        ('strERANGE', (td_char*25)),
        ('strENOTTY', (td_char*10)),
        ('strEACCES', (td_char*18)),
        ('strEPERM', (td_char*24)),
        ('strENOENT', (td_char*26)),
        ('strESRCH', (td_char*16)),
        ('strEEXIST', (td_char*12)),
        ('strEOVERFLOW', (td_char*30)),
        ('strENOSPC', (td_char*24)),
        ('strENOMEM', (td_char*14)),
        ('strEBUSY', (td_char*14)),
        ('strEINTR', (td_char*24)),
        ('strEAGAIN', (td_char*33)),
        ('strESPIPE', (td_char*13)),
        ('strEXDEV', (td_char*18)),
        ('strEROFS', (td_char*22)),
        ('strENOTEMPTY', (td_char*20)),
        ('strECONNRESET', (td_char*25)),
        ('strETIMEDOUT', (td_char*20)),
        ('strECONNREFUSED', (td_char*19)),
        ('strEHOSTDOWN', (td_char*13)),
        ('strEHOSTUNREACH', (td_char*20)),
        ('strEADDRINUSE', (td_char*15)),
        ('strEPIPE', (td_char*12)),
        ('strEIO', (td_char*10)),
        ('strENXIO', (td_char*26)),
        ('strENOTBLK', (td_char*22)),
        ('strENODEV', (td_char*15)),
        ('strENOTDIR', (td_char*16)),
        ('strEISDIR', (td_char*15)),
        ('strETXTBSY', (td_char*15)),
        ('strENOEXEC', (td_char*18)),
        ('strEINVAL', (td_char*17)),
        ('strE2BIG', (td_char*23)),
        ('strELOOP', (td_char*19)),
        ('strENAMETOOLONG', (td_char*18)),
        ('strENFILE', (td_char*30)),
        ('strEMFILE', (td_char*30)),
        ('strEBADF', (td_char*20)),
        ('strECHILD', (td_char*17)),
        ('strEFAULT', (td_char*12)),
        ('strEFBIG', (td_char*15)),
        ('strEMLINK', (td_char*15)),
        ('strENOLCK', (td_char*19)),
        ('strEDEADLK', (td_char*30)),
        ('strENOTRECOVERABLE', (td_char*22)),
        ('strEOWNERDEAD', (td_char*20)),
        ('strECANCELED', (td_char*19)),
        ('strENOSYS', (td_char*25)),
        ('strENOMSG', (td_char*27)),
        ('strEIDRM', (td_char*19)),
        ('strENOSTR', (td_char*20)),
        ('strENODATA', (td_char*18)),
        ('strETIME', (td_char*15)),
        ('strENOSR', (td_char*25)),
        ('strENOLINK', (td_char*22)),
        ('strEPROTO', (td_char*15)),
        ('strEBADMSG', (td_char*12)),
        ('strEBADFD', (td_char*29)),
        ('strENOTSOCK', (td_char*13)),
        ('strEDESTADDRREQ', (td_char*29)),
        ('strEMSGSIZE', (td_char*18)),
        ('strEPROTOTYPE', (td_char*31)),
        ('strENOPROTOOPT', (td_char*23)),
        ('strEPROTONOSUPPORT', (td_char*23)),
        ('strESOCKTNOSUPPORT', (td_char*26)),
        ('strENOTSUP', (td_char*14)),
        ('strEPFNOSUPPORT', (td_char*30)),
        ('strEAFNOSUPPORT', (td_char*41)),
        ('strEADDRNOTAVAIL', (td_char*22)),
        ('strENETDOWN', (td_char*16)),
        ('strENETUNREACH', (td_char*20)),
        ('strENETRESET', (td_char*28)),
        ('strECONNABORTED', (td_char*19)),
        ('strENOBUFS', (td_char*26)),
        ('strEISCONN', (td_char*20)),
        ('strENOTCONN', (td_char*21)),
        ('strESHUTDOWN', (td_char*34)),
        ('strEALREADY', (td_char*30)),
        ('strEINPROGRESS', (td_char*22)),
        ('strESTALE', (td_char*18)),
        ('strEREMOTEIO', (td_char*17)),
        ('strEDQUOT', (td_char*15)),
        ('strENOMEDIUM', (td_char*16)),
        ('strEMEDIUMTYPE', (td_char*18)),
        ('strEMULTIHOP', (td_char*19)),
        ('strENOKEY', (td_char*27)),
        ('strEKEYEXPIRED', (td_char*16)),
        ('strEKEYREVOKED', (td_char*21)),
        ('strEKEYREJECTED', (td_char*28)),
    ]
class __locale_map(Structure):
    _fields_ = [
        ('map', td_u32),
        ('map_size', size_t),
        ('name', (td_char*24)),
        ('next', td_u32),
    ]
ldouble_t=td_double
class pthread_mutexattr(Structure):
    _fields_ = [
        ('protocol', UINT8),
        ('prioceiling', UINT8),
        ('type', UINT8),
        ('reserved', UINT8),
    ]
pthread_mutexattr_t=pthread_mutexattr
class pthread_mutex(Structure):
    _fields_ = [
        ('stAttr', pthread_mutexattr_t),
        ('stLock', MuxBaseCB),
    ]
pthread_mutex_t=pthread_mutex
class random_type_237(Structure):
    _fields_ = [
        ('muxList', LOS_DL_LIST),
        ('owner', td_u32),
        ('muxCount', UINT16),
    ]
class random_type_238(Structure):
    _fields_ = [
        ('muxList', LOS_DL_LIST),
        ('owner', td_u32),
        ('muxCount', UINT16),
    ]
class random_type_239(Structure):
    _fields_ = [
        ('muxList', LOS_DL_LIST),
        ('owner', td_u32),
        ('muxCount', UINT16),
    ]
class __mbstate_t(Structure):
    _fields_ = [
        ('__opaque1', td_u32),
        ('__opaque2', td_u32),
    ]
mbstate_t=__mbstate_t
class glue(Structure):
    _fields_ = [
        ('next', td_u32),
        ('niobs', td_s32),
        ('iobs', td_u32),
    ]
class random_type_240(Structure):
    _fields_ = [
        ('muxList', LOS_DL_LIST),
        ('owner', td_u32),
        ('muxCount', UINT16),
    ]
class random_type_241(Structure):
    _fields_ = [
        ('muxList', LOS_DL_LIST),
        ('owner', td_u32),
        ('muxCount', UINT16),
    ]
class random_type_242(Structure):
    _fields_ = [
        ('muxList', LOS_DL_LIST),
        ('owner', td_u32),
        ('muxCount', UINT16),
    ]
class iovec(Structure):
    _fields_ = [
        ('iov_base', td_u32),
        ('iov_len', size_t),
    ]
class random_type_243(Structure):
    _fields_ = [
        ('muxList', LOS_DL_LIST),
        ('owner', td_u32),
        ('muxCount', UINT16),
    ]
ssize_t=td_s32
class random_type_244(Structure):
    _fields_ = [
        ('muxList', LOS_DL_LIST),
        ('owner', td_u32),
        ('muxCount', UINT16),
    ]
class random_type_245(Structure):
    _fields_ = [
        ('muxList', LOS_DL_LIST),
        ('owner', td_u32),
        ('muxCount', UINT16),
    ]
class random_type_246(Structure):
    _fields_ = [
        ('muxList', LOS_DL_LIST),
        ('owner', td_u32),
        ('muxCount', UINT16),
    ]
class random_type_247(Structure):
    _fields_ = [
        ('muxList', LOS_DL_LIST),
        ('owner', td_u32),
        ('muxCount', UINT16),
    ]
class random_type_248(Structure):
    _fields_ = [
        ('muxList', LOS_DL_LIST),
        ('owner', td_u32),
        ('muxCount', UINT16),
    ]
class random_type_249(Structure):
    _fields_ = [
        ('muxList', LOS_DL_LIST),
        ('owner', td_u32),
        ('muxCount', UINT16),
    ]
class random_type_250(Structure):
    _fields_ = [
        ('muxList', LOS_DL_LIST),
        ('owner', td_u32),
        ('muxCount', UINT16),
    ]
class random_type_251(Structure):
    _fields_ = [
        ('muxList', LOS_DL_LIST),
        ('owner', td_u32),
        ('muxCount', UINT16),
    ]
class random_type_252(Structure):
    _fields_ = [
        ('muxList', LOS_DL_LIST),
        ('owner', td_u32),
        ('muxCount', UINT16),
    ]
class random_type_253(Structure):
    _fields_ = [
        ('muxList', LOS_DL_LIST),
        ('owner', td_u32),
        ('muxCount', UINT16),
    ]
class random_type_254(Structure):
    _fields_ = [
        ('muxList', LOS_DL_LIST),
        ('owner', td_u32),
        ('muxCount', UINT16),
    ]
class random_type_255(Structure):
    _fields_ = [
        ('muxList', LOS_DL_LIST),
        ('owner', td_u32),
        ('muxCount', UINT16),
    ]
class random_type_256(Structure):
    _fields_ = [
        ('muxList', LOS_DL_LIST),
        ('owner', td_u32),
        ('muxCount', UINT16),
    ]
class random_type_257(Structure):
    _fields_ = [
        ('muxList', LOS_DL_LIST),
        ('owner', td_u32),
        ('muxCount', UINT16),
    ]
class random_type_258(Structure):
    _fields_ = [
        ('muxList', LOS_DL_LIST),
        ('owner', td_u32),
        ('muxCount', UINT16),
    ]
class random_type_259(Structure):
    _fields_ = [
        ('muxList', LOS_DL_LIST),
        ('owner', td_u32),
        ('muxCount', UINT16),
    ]
class random_type_260(Structure):
    _fields_ = [
        ('muxList', LOS_DL_LIST),
        ('owner', td_u32),
        ('muxCount', UINT16),
    ]
class random_type_261(Structure):
    _fields_ = [
        ('muxList', LOS_DL_LIST),
        ('owner', td_u32),
        ('muxCount', UINT16),
    ]
mode_t=td_u32
nlink_t=td_u32
uid_t=td_u32
gid_t=td_u32
blksize_t=td_s32
class random_type_262(Structure):
    _fields_ = [
        ('muxList', LOS_DL_LIST),
        ('owner', td_u32),
        ('muxCount', UINT16),
    ]
class random_type_263(Structure):
    _fields_ = [
        ('muxList', LOS_DL_LIST),
        ('owner', td_u32),
        ('muxCount', UINT16),
    ]
class random_type_264(Structure):
    _fields_ = [
        ('muxList', LOS_DL_LIST),
        ('owner', td_u32),
        ('muxCount', UINT16),
    ]
class random_type_265(Structure):
    _fields_ = [
        ('muxList', LOS_DL_LIST),
        ('owner', td_u32),
        ('muxCount', UINT16),
    ]
class lockfile_func(Structure):
    _fields_ = [
        ('lock', td_u32),
        ('unlock', td_u32),
    ]
class random_type_266(Structure):
    _fields_ = [
        ('muxList', LOS_DL_LIST),
        ('owner', td_u32),
        ('muxCount', UINT16),
    ]
class random_type_267(Structure):
    _fields_ = [
        ('muxList', LOS_DL_LIST),
        ('owner', td_u32),
        ('muxCount', UINT16),
    ]
class cookie(Structure):
    _fields_ = [
        ('s', td_u32),
        ('n', size_t),
    ]
class random_type_268(Structure):
    _fields_ = [
        ('muxList', LOS_DL_LIST),
        ('owner', td_u32),
        ('muxCount', UINT16),
    ]
class random_type_269(Structure):
    _fields_ = [
        ('muxList', LOS_DL_LIST),
        ('owner', td_u32),
        ('muxCount', UINT16),
    ]
WT=td_u32
class timezone(Structure):
    _fields_ = [
        ('tz_minuteswest', td_s32),
        ('tz_dsttime', td_s32),
    ]
clockid_t=td_s32
int_fast32_t=td_s32
class random_type_270(Structure):
    _fields_ = [
        ('sortLinkNode', LOS_DL_LIST),
        ('idxRollNum', UINT32),
    ]
class random_type_271(Structure):
    _fields_ = [
        ('muxList', LOS_DL_LIST),
        ('owner', td_u32),
        ('muxCount', UINT16),
    ]
class random_type_274(Structure):
    _fields_ = [
        ('triggerIrq', td_u32),
        ('clearIrq', td_u32),
        ('enableIrq', td_u32),
        ('disableIrq', td_u32),
        ('setIrqPriority', td_u32),
        ('getCurIrqNum', td_u32),
        ('getIrqVersion', td_u32),
        ('getHandleForm', td_u32),
        ('handleIrq', td_u32),
    ]
class LosDeviceConfig(Structure):
    _fields_ = [
        ('irqNum', UINTPTR),
        ('numRegs', UINT32),
        ('regs', td_u32),
    ]
class LosDevice(Structure):
    _fields_ = [
        ('name', td_u32),
        ('id', INT32),
        ('driverNode', LOS_DL_LIST),
        ('deviceItem', LOS_DL_LIST),
        ('pmNode', LOS_DL_LIST),
        ('mutex', UINT32),
        ('isRegistered', BOOL),
        ('driver', td_u32),
        ('cfg', LosDeviceConfig),
        ('data', td_u32),
    ]
class LosDriverOps(Structure):
    _fields_ = [
        ('probe', td_u32),
        ('remove', td_u32),
        ('shutdown', td_u32),
    ]
class LosPowerMgrOps(Structure):
    _fields_ = [
        ('suspend', td_u32),
        ('resume', td_u32),
        ('prepare', td_u32),
        ('complete', td_u32),
    ]
class LosDriver(Structure):
    _fields_ = [
        ('name', td_u32),
        ('deviceList', LOS_DL_LIST),
        ('driverItem', LOS_DL_LIST),
        ('mutex', UINT32),
        ('isRegistered', BOOL),
        ('ops', LosDriverOps),
        ('pmOps', LosPowerMgrOps),
    ]
class LosDeviceRegs(Structure):
    _fields_ = [
        ('base', UINTPTR),
        ('size', UINTPTR),
    ]
osKernelState_t=td_u32
class random_type_275(Structure):
    _fields_ = [
        ('api', uint32_t),
        ('kernel', uint32_t),
    ]
osVersion_t=random_type_275
osThreadId_t=td_u32
class random_type_276(Structure):
    _fields_ = [
        ('sortLinkNode', LOS_DL_LIST),
        ('idxRollNum', UINT32),
    ]
osEventFlagsId_t=td_u32
class random_type_277(Structure):
    _fields_ = [
        ('sortList', SortLinkList),
        ('state', UINT8),
        ('mode', UINT8),
        ('overrun', UINT8),
        ('inProcess', UINT8),
        ('timerId', UINT16),
        ('interval', UINT32),
        ('expiry', UINT32),
        ('arg', UINTPTR),
        ('handler', SWTMR_PROC_FUNC),
    ]
osMutexId_t=td_u32
class random_type_278(Structure):
    _fields_ = [
        ('muxList', LOS_DL_LIST),
        ('owner', td_u32),
        ('muxCount', UINT16),
        ('muxStat', UINT8),
        ('muxId', UINT32),
    ]
class random_type_279(Structure):
    _fields_ = [
        ('semStat', UINT8),
        ('semType', UINT8),
        ('semCount', UINT16),
        ('semId', UINT32),
        ('semList', LOS_DL_LIST),
    ]
osMessageQueueId_t=td_u32
class random_type_280(Structure):
    _fields_ = [
        ('queueHandle', td_u32),
        ('queueState', UINT8),
        ('queueMemType', UINT8),
        ('queueLen', UINT16),
        ('queueSize', UINT16),
        ('queueId', UINT32),
        ('queueHead', UINT16),
        ('queueTail', UINT16),
        ('readWriteableCnt', (UINT16*2)),
        ('readWriteList', (LOS_DL_LIST*2)),
        ('memList', LOS_DL_LIST),
    ]
osStatus_t=td_u32
osThreadState_t=td_u32
class random_type_281(Structure):
    _fields_ = [
        ('name', td_u32),
        ('attr_bits', uint32_t),
        ('cb_mem', td_u32),
        ('cb_size', uint32_t),
        ('stack_mem', td_u32),
        ('stack_size', uint32_t),
        ('priority', osPriority_t),
        ('tz_module', TZ_ModuleId_t),
        ('reserved', uint32_t),
    ]
class random_type_282(Structure):
    _fields_ = [
        ('name', td_u32),
        ('attr_bits', uint32_t),
        ('cb_mem', td_u32),
        ('cb_size', uint32_t),
    ]
osEventFlagsAttr_t=random_type_282
class random_type_283(Structure):
    _fields_ = [
        ('name', td_u32),
        ('attr_bits', uint32_t),
        ('cb_mem', td_u32),
        ('cb_size', uint32_t),
    ]
class random_type_284(Structure):
    _fields_ = [
        ('name', td_u32),
        ('attr_bits', uint32_t),
        ('cb_mem', td_u32),
        ('cb_size', uint32_t),
    ]
osMutexAttr_t=random_type_284
class random_type_285(Structure):
    _fields_ = [
        ('name', td_u32),
        ('attr_bits', uint32_t),
        ('cb_mem', td_u32),
        ('cb_size', uint32_t),
    ]
osSemaphoreAttr_t=random_type_285
class random_type_286(Structure):
    _fields_ = [
        ('name', td_u32),
        ('attr_bits', uint32_t),
        ('cb_mem', td_u32),
        ('cb_size', uint32_t),
        ('mq_mem', td_u32),
        ('mq_size', uint32_t),
    ]
osMessageQueueAttr_t=random_type_286
class random_type_287(Structure):
    _fields_ = [
        ('sortLinkNode', LOS_DL_LIST),
        ('idxRollNum', UINT32),
    ]
InitcallFunc=td_u32
class workqueue_struct(Structure):
    _fields_ = [
        ('cpu_wq', td_u32),
        ('list', LOS_DL_LIST),
        ('pendList', LOS_DL_LIST),
        ('wq_id', td_u32),
        ('delayed_work_count', td_s32),
        ('name', td_u32),
        ('singlethread', td_s32),
        ('wq_status', td_s32),
        ('freezeable', td_s32),
        ('rt', td_s32),
    ]
class tag_cpu_workqueue_struct(Structure):
    _fields_ = [
        ('worklist', LOS_DL_LIST),
        ('current_work', td_u32),
        ('wq', td_u32),
        ('thread', td_u32),
    ]
cpu_workqueue_struct=tag_cpu_workqueue_struct
work_func_t=td_u32
class work_struct(Structure):
    _fields_ = [
        ('entry', LOS_DL_LIST),
        ('func', work_func_t),
        ('work_status', UINT32),
    ]
task_struct=LosTaskCB
class random_type_288(Structure):
    _fields_ = [
        ('sortLinkNode', LOS_DL_LIST),
        ('idxRollNum', UINT32),
    ]
ULONG=td_u32
class timer_list(Structure):
    _fields_ = [
        ('expires', ULONG),
        ('function', td_u32),
        ('timerid', UINT16),
        ('flag', UINT32),
        ('created', BOOL),
        ('lock', SPIN_LOCK_S),
    ]
class delayed_work(Structure):
    _fields_ = [
        ('work', work_struct),
        ('timer', timer_list),
        ('wq', td_u32),
        ('cpu', td_s32),
    ]
class random_type_289(Structure):
    _fields_ = [
        ('sortList', SortLinkList),
        ('state', UINT8),
        ('mode', UINT8),
        ('overrun', UINT8),
        ('inProcess', UINT8),
        ('timerId', UINT16),
        ('interval', UINT32),
        ('expiry', UINT32),
        ('arg', UINTPTR),
        ('handler', SWTMR_PROC_FUNC),
    ]
class random_type_290(Structure):
    _fields_ = [
        ('sortLink', td_u32),
        ('cursor', UINT16),
        ('reserved', UINT16),
    ]
OsSortLinkCond=td_u32
class lock_class_key(Structure):
    _fields_ = [
    ]
class random_type_291(Structure):
    _fields_ = [
        ('event', td_u32),
    ]
osal_event=random_type_291
osal_irq_handler=td_u32
class random_type_292(Structure):
    _fields_ = [
        ('tasklet', td_u32),
        ('handler', td_u32),
        ('data', td_u32),
    ]
osal_tasklet=random_type_292
class random_type_293(Structure):
    _fields_ = [
        ('muxList', LOS_DL_LIST),
        ('owner', td_u32),
        ('muxCount', UINT16),
        ('muxStat', UINT8),
        ('muxId', UINT32),
    ]
class random_type_294(Structure):
    _fields_ = [
        ('sortLinkNode', LOS_DL_LIST),
        ('idxRollNum', UINT32),
    ]
class random_type_295(Structure):
    _fields_ = [
        ('mutex', td_u32),
    ]
osal_mutex=random_type_295
class random_type_296(Structure):
    _fields_ = [
        ('sem', td_u32),
    ]
osal_semaphore=random_type_296
class random_type_297(Structure):
    _fields_ = [
        ('lock', td_u32),
    ]
osal_spinlock=random_type_297
class random_type_298(Structure):
    _fields_ = [
        ('task', td_u32),
    ]
osal_task=random_type_298
osal_kthread_handler=td_u32
class random_type_299(Structure):
    _fields_ = [
        ('handler', osal_kthread_handler),
        ('taskprio', td_u32),
        ('stacksize', td_u32),
        ('taskname', td_u32),
        ('data', td_u32),
    ]
osal_kthread_init=random_type_299
class random_type_300(Structure):
    _fields_ = [
        ('timer', td_u32),
        ('handler', td_u32),
        ('data', td_u32),
        ('interval', td_u32),
    ]
osal_timer=random_type_300
class dpal_dl_list(Structure):
    _fields_ = [
        ('prev', td_u32),
        ('next', td_u32),
    ]
dpal_dl_list_t=dpal_dl_list
class dpal_device_config(Structure):
    _fields_ = [
        ('irq_num', uintptr_t),
        ('num_regs', uint32_t),
        ('regs', td_u32),
    ]
class dpal_device(Structure):
    _fields_ = [
        ('name', td_u32),
        ('id', int32_t),
        ('driver_node', dpal_dl_list_t),
        ('device_item', dpal_dl_list_t),
        ('pm_node', dpal_dl_list_t),
        ('mutex', uint32_t),
        ('is_registered', td_bool),
        ('drv', td_u32),
        ('cfg', dpal_device_config),
        ('data', td_u32),
    ]
class dpal_driver_ops(Structure):
    _fields_ = [
        ('probe', td_u32),
        ('remove', td_u32),
        ('shutdown', td_u32),
    ]
class dpal_power_mgr_ops(Structure):
    _fields_ = [
        ('suspend', td_u32),
        ('resume', td_u32),
        ('prepare', td_u32),
        ('complete', td_u32),
    ]
class dpal_driver(Structure):
    _fields_ = [
        ('name', td_u32),
        ('device_list', dpal_dl_list_t),
        ('driver_item', dpal_dl_list_t),
        ('mutex', uint32_t),
        ('is_registered', td_bool),
        ('ops', dpal_driver_ops),
        ('pm_ops', dpal_power_mgr_ops),
    ]
class dpal_device_regs(Structure):
    _fields_ = [
        ('base', uintptr_t),
        ('size', uintptr_t),
    ]
class random_type_302(Structure):
    _fields_ = [
        ('counter', td_s32),
    ]
osal_atomic=random_type_302
class random_type_303(Structure):
    _fields_ = [
        ('mutex', td_u32),
    ]
class random_type_304(Structure):
    _fields_ = [
        ('event', td_u32),
    ]
class random_type_305(Structure):
    _fields_ = [
        ('task', td_u32),
    ]
class dpal_device_driver(Structure):
    _fields_ = [
        ('dpal_drv', dpal_driver),
        ('name', td_u32),
        ('pm', td_u32),
    ]
class dpal_platform_driver(Structure):
    _fields_ = [
        ('probe', td_u32),
        ('remove', td_u32),
        ('shutdown', td_u32),
        ('suspend', td_u32),
        ('resume', td_u32),
        ('drv', dpal_device_driver),
    ]
class dpal_pm_message(Structure):
    _fields_ = [
        ('event', int32_t),
    ]
dpal_dev_pm_message_t=dpal_pm_message
class dpal_dev_pm_info(Structure):
    _fields_ = [
        ('power_state', dpal_dev_pm_message_t),
        ('entry', dpal_dl_list_t),
    ]
class device_dpal(Structure):
    _fields_ = [
        ('dpal_dev', dpal_device),
        ('name', td_u32),
        ('driver_data', td_u32),
        ('platform_data', td_u32),
        ('power', dpal_dev_pm_info),
    ]
class dpal_platform_device(Structure):
    _fields_ = [
        ('name', td_u32),
        ('dev', device_dpal),
        ('id', int32_t),
        ('num_resources', uint32_t),
        ('resource', td_u32),
    ]
class dpal_resource(Structure):
    _fields_ = [
        ('start', uint32_t),
        ('end', uint32_t),
        ('name', td_u32),
        ('flags', td_u32),
        ('desc', td_u32),
        ('parent', td_u32),
        ('sibling', td_u32),
        ('child', td_u32),
    ]
dpal_resource_t=dpal_resource
class dpal_dev_pm_op(Structure):
    _fields_ = [
        ('suspend', td_u32),
        ('resume', td_u32),
        ('prepare', td_u32),
        ('complete', td_u32),
    ]
class random_type_306(Structure):
    _fields_ = [
        ('lock', td_u32),
    ]
class random_type_307(Structure):
    _fields_ = [
        ('sem', td_u32),
    ]
dpal_hwi_proc_func=td_u32
class random_type_308(Structure):
    _fields_ = [
        ('event_id', uint32_t),
        ('event_max_num', uint32_t),
        ('event_bits', uint32_t),
        ('event_bit_idx', uint32_t),
        ('event', osal_event),
    ]
dpal_event_t=random_type_308
class random_type_309(Structure):
    _fields_ = [
        ('task_prio', uint16_t),
        ('stack_size', uint32_t),
        ('task_policy', uint32_t),
        ('task_nice', uint32_t),
        ('task_cpuid', uint32_t),
        ('task_name', td_u32),
        ('reserved', td_u32),
    ]
dpal_task_attr_t=random_type_309
dpal_tsk_entry_func=td_u32
class random_type_310(Structure):
    _fields_ = [
        ('lock', td_u32),
    ]
dpal_spinlock=random_type_310
class random_type_311(Structure):
    _fields_ = [
        ('sem', td_u32),
    ]
nu_sem=random_type_311
class _cpu_trace_item_(Structure):
    _fields_ = [
        ('time', uint32_t),
        ('lr', uint32_t),
        ('pc', uint32_t),
    ]
cpu_trace_item_t=_cpu_trace_item_
hal_dma_init_t=td_u32
hal_dma_deinit_t=td_u32
hal_dma_open_t=td_u32
hal_dma_close_t=td_u32
hal_dma_ch_enable_t=td_u32
hal_dma_get_block_t=td_u32
hal_dma_clear_t=td_u32
hal_dma_get_idle_t=td_u32
hal_dma_cfg_sgl_t=td_u32
hal_dma_add_lli_t=td_u32
hal_dma_is_enabled_t=td_u32
hal_dma_enable_lli_t=td_u32
class hal_dma_funcs(Structure):
    _fields_ = [
        ('init', hal_dma_init_t),
        ('deinit', hal_dma_deinit_t),
        ('open', hal_dma_open_t),
        ('close', hal_dma_close_t),
        ('ch_enable', hal_dma_ch_enable_t),
        ('get_block', hal_dma_get_block_t),
        ('clear', hal_dma_clear_t),
        ('get_idle', hal_dma_get_idle_t),
        ('cfg_single', hal_dma_cfg_sgl_t),
        ('add_lli', hal_dma_add_lli_t),
        ('is_enabled', hal_dma_is_enabled_t),
        ('enable_lli', hal_dma_enable_lli_t),
    ]
hal_dma_funcs_t=hal_dma_funcs
hal_dma_transfer_cb_t=td_u32
class dma_ch_user_peripheral_config(Structure):
    _fields_ = [
        ('src', uint32_t),
        ('dest', uint32_t),
        ('transfer_num', uint16_t),
        ('src_handshaking', uint16_t),
        ('dest_handshaking', uint16_t),
        ('trans_type', uint8_t),
        ('trans_dir', uint8_t),
        ('priority', uint8_t),
        ('src_width', uint8_t),
        ('dest_width', uint8_t),
        ('burst_length', uint8_t),
        ('src_increment', uint8_t),
        ('dest_increment', uint8_t),
        ('protection', uint8_t),
    ]
dma_ch_user_peripheral_config_t=dma_ch_user_peripheral_config
class dma_ch_user_memory_config(Structure):
    _fields_ = [
        ('src', uint32_t),
        ('dest', uint32_t),
        ('transfer_num', uint16_t),
        ('priority', uint8_t),
        ('width', uint8_t),
    ]
dma_ch_user_memory_config_t=dma_ch_user_memory_config
dma_transfer_cb_t=td_u32
class hal_spi_attr(Structure):
    _fields_ = [
        ('is_slave', td_bool),
        ('slave_num', uint32_t),
        ('bus_clk', uint32_t),
        ('freq_mhz', uint32_t),
        ('clk_polarity', uint32_t),
        ('clk_phase', uint32_t),
        ('frame_format', uint32_t),
        ('spi_frame_format', uint32_t),
        ('frame_size', uint32_t),
        ('tmod', uint32_t),
        ('ndf', uint32_t),
        ('sste', uint32_t),
    ]
spi_attr_t=hal_spi_attr
hal_spi_attr_t=hal_spi_attr
class hal_spi_xfer_sspi_param(Structure):
    _fields_ = [
        ('wait_cycles', uint32_t),
    ]
hal_spi_xfer_sspi_param_t=hal_spi_xfer_sspi_param
class flash_device_parameter(Structure):
    _fields_ = [
        ('manufacturer_id', uint32_t),
        ('flash_size', uint32_t),
        ('enter_xip', uint8_t),
        ('exit_xip', uint8_t),
        ('read_dummy_clk', uint8_t),
        ('security_reg_cmd', uint8_t),
        ('suspend_cmd', uint8_t),
        ('resume_cmd', uint8_t),
        ('enter_qspi_mode_cmd', td_u32),
        ('enter_xip_mode_cmd', td_u32),
        ('enter_xip_mode_config', td_u32),
    ]
flash_device_parameter_t=flash_device_parameter
class flash_qspi_xip_config(Structure):
    _fields_ = [
        ('enter_xip_before_wait_cycles_config', uint32_t),
        ('enter_xip_before_inst_l', uint32_t),
        ('enter_xip_before_addr_l', uint32_t),
        ('enter_xip_before_trans_type', uint32_t),
        ('enter_xip_after_wait_cycles_config', uint32_t),
        ('enter_xip_after_inst_l', uint32_t),
        ('enter_xip_after_addr_l', uint32_t),
        ('enter_xip_after_trans_type', uint32_t),
        ('enter_xip_after_enable_32bit_addr', td_bool),
        ('enter_xip_after_enable_wrap', td_bool),
    ]
flash_qspi_xip_config_t=flash_qspi_xip_config
class ssi_regs(Structure):
    _fields_ = [
        ('ctrlr0', uint32_t),
        ('ctrlr1', uint32_t),
        ('ssienr', uint32_t),
        ('mwcr', uint32_t),
        ('ser', uint32_t),
        ('baudr', uint32_t),
        ('txftlr', uint32_t),
        ('rxftlr', uint32_t),
        ('txflr', uint32_t),
        ('rxflr', uint32_t),
        ('sr', uint32_t),
        ('imr', uint32_t),
        ('isr', uint32_t),
        ('risr', uint32_t),
        ('txoicr', uint32_t),
        ('rxoicr', uint32_t),
        ('rxuicr', uint32_t),
        ('msticr', uint32_t),
        ('icr', uint32_t),
        ('dmacr', uint32_t),
        ('dmatdlr', uint32_t),
        ('dmardlr', uint32_t),
        ('idr', uint32_t),
        ('ssi_version_id', uint32_t),
        ('dr', (uint32_t*36)),
        ('rx_sample_dly', uint32_t),
        ('spi_ctrlr0', uint32_t),
        ('txd_drive_edge', uint32_t),
        ('rsvd', uint32_t),
    ]
ssi_regs_t=ssi_regs
class random_type_312(Structure):
    _fields_ = [
        ('ssi_en', uint32_t,1),
    ]
class ssienr_data(Union):
    _fields_ = [
        ('d32', uint32_t),
        ('b', random_type_312),
    ]
ssienr_data_t=ssienr_data
class random_type_313(Structure):
    _fields_ = [
        ('ser', uint32_t,16),
    ]
class ser_data(Union):
    _fields_ = [
        ('d32', uint32_t),
        ('b', random_type_313),
    ]
ser_data_t=ser_data
class random_type_314(Structure):
    _fields_ = [
        ('tft', uint32_t),
    ]
class txftlr_data(Union):
    _fields_ = [
        ('d32', uint32_t),
        ('b', random_type_314),
    ]
txftlr_data_t=txftlr_data
class random_type_315(Structure):
    _fields_ = [
        ('rft', uint32_t),
    ]
class rxftlr_data(Union):
    _fields_ = [
        ('d32', uint32_t),
        ('b', random_type_315),
    ]
rxftlr_data_t=rxftlr_data
class hal_spi_xfer_data(Structure):
    _fields_ = [
        ('tx_buff', td_u32),
        ('tx_bytes', uint32_t),
        ('rx_buff', td_u32),
        ('rx_bytes', uint32_t),
        ('cmd', uint8_t),
        ('reserved', (uint8_t*3)),
        ('addr', uint32_t),
    ]
class random_type_316(Structure):
    _fields_ = [
        ('txtfl', uint32_t),
    ]
class txflr_data(Union):
    _fields_ = [
        ('d32', uint32_t),
        ('b', random_type_316),
    ]
txflr_data_t=txflr_data
class random_type_317(Structure):
    _fields_ = [
        ('busy', uint32_t,1),
        ('tfnf', uint32_t,1),
        ('tfe', uint32_t,1),
        ('rfne', uint32_t,1),
        ('rff', uint32_t,1),
        ('txe', uint32_t,1),
        ('dcol', uint32_t,1),
        ('reserved7_31', uint32_t),
    ]
class sr_data(Union):
    _fields_ = [
        ('d32', uint32_t),
        ('b', random_type_317),
    ]
sr_data_t=sr_data
class random_type_318(Structure):
    _fields_ = [
        ('rxtfl', uint32_t),
    ]
class rxflr_data(Union):
    _fields_ = [
        ('d32', uint32_t),
        ('b', random_type_318),
    ]
rxflr_data_t=rxflr_data
spi_xfer_data_t=hal_spi_xfer_data
hal_spi_xfer_data_t=hal_spi_xfer_data
class random_type_319(Structure):
    _fields_ = [
        ('rdmae', uint32_t,1),
        ('tdmae', uint32_t,1),
        ('reserved1_31', uint32_t,30),
    ]
class dmacr_data(Union):
    _fields_ = [
        ('d32', uint32_t),
        ('b', random_type_319),
    ]
dmacr_data_t=dmacr_data
class random_type_320(Structure):
    _fields_ = [
        ('dmadl', uint32_t),
    ]
class dmadlr_data(Union):
    _fields_ = [
        ('d32', uint32_t),
        ('b', random_type_320),
    ]
dmardlr_data_t=dmadlr_data
class flash_info(Structure):
    _fields_ = [
        ('flash_id', uint32_t),
        ('flash_size', uint32_t),
        ('flash_unique_id', uint32_t),
    ]
flash_info_t=flash_info
class random_type_321(Structure):
    _fields_ = [
        ('rsd', uint32_t,8),
        ('reserved8_31', uint32_t,24),
    ]
class rx_sample_dly_data(Union):
    _fields_ = [
        ('d32', uint32_t),
        ('b', random_type_321),
    ]
rx_sample_dly_data_t=rx_sample_dly_data
class random_type_322(Structure):
    _fields_ = [
        ('sckdv', uint32_t,16),
    ]
class baudr_data(Union):
    _fields_ = [
        ('d32', uint32_t),
        ('b', random_type_322),
    ]
baudr_data_t=baudr_data
hal_gpio_init_t=td_u32
hal_gpio_deinit_t=td_u32
hal_gpio_setdir_t=td_u32
hal_gpio_getdir_t=td_u32
hal_gpio_ctrl_t=td_u32
hal_gpio_output_t=td_u32
hal_gpio_getoutval_t=td_u32
hal_gpio_input_t=td_u32
hal_gpio_register_t=td_u32
hal_gpio_unregister_t=td_u32
class random_type_323(Structure):
    _fields_ = [
        ('init', hal_gpio_init_t),
        ('deinit', hal_gpio_deinit_t),
        ('setdir', hal_gpio_setdir_t),
        ('getdir', hal_gpio_getdir_t),
        ('ctrl', hal_gpio_ctrl_t),
        ('output', hal_gpio_output_t),
        ('get_output_val', hal_gpio_getoutval_t),
        ('input', hal_gpio_input_t),
        ('registerfunc', hal_gpio_register_t),
        ('unregisterfunc', hal_gpio_unregister_t),
    ]
hal_gpio_funcs_t=random_type_323
gpio_callback_t=td_u32
class _ipc_spin_send_info_(Structure):
    _fields_ = [
        ('err_cnt', uint32_t,31),
        ('pending', uint32_t,1),
        ('pc', uint32_t),
        ('lr', uint32_t),
        ('sp', uint32_t),
        ('called', uint32_t),
        ('loop_cnt', uint32_t),
    ]
ipc_spin_send_info=_ipc_spin_send_info_
class _ipc_send_info_(Structure):
    _fields_ = [
        ('send_cnt', uint32_t),
        ('loop_cnt', uint32_t),
        ('send_data_called', uintptr_t),
        ('spin_send_info', ipc_spin_send_info),
    ]
ipc_send_info=_ipc_send_info_
ipc_action_t=td_u16
class random_type_326(Structure):
    _fields_ = [
        ('source_ptr', uint32_t),
        ('dest_ptr', uint32_t),
        ('length', uint16_t),
    ]
ipc_payload_memory=random_type_326
class random_type_329(Structure):
    _fields_ = [
        ('reboot_cause', uint32_t),
    ]
ipc_payload_core_started=random_type_329
class random_type_330(Structure):
    _fields_ = [
        ('data', (uint8_t*3072)),
    ]
ipc_payload_rpc=random_type_330
class random_type_331(Structure):
    _fields_ = [
        ('requested_reboot_reason', uint16_t),
    ]
ipc_payload_request_reboot=random_type_331
class random_type_332(Structure):
    _fields_ = [
        ('log_level', uint32_t),
    ]
ipc_payload_set_log_level=random_type_332
class random_type_335(Structure):
    _fields_ = [
        ('size', uint32_t),
    ]
ipc_payload_bt_hifi_mem_req=random_type_335
class random_type_336(Structure):
    _fields_ = [
        ('result', uint32_t),
    ]
ipc_payload_hifi_bt_mem_cnf=random_type_336
class random_type_337(Structure):
    _fields_ = [
        ('params', (uint8_t*256)),
        ('length', uint32_t),
    ]
ipc_payload_ap_bt_calibration_param_req=random_type_337
class random_type_339(Structure):
    _fields_ = [
        ('type', uint8_t),
        ('event_id', uint8_t),
        ('info1', uint8_t),
        ('info2', uint8_t),
        ('info3', uint16_t),
    ]
ipc_payload_mass_data_type=random_type_339
class random_type_340(Structure):
    _fields_ = [
        ('mass_data', ipc_payload_mass_data_type),
        ('data_len', uint8_t),
        ('data', (uint8_t*128)),
    ]
ipc_payload_mass_data_extend_type=random_type_340
class random_type_341(Structure):
    _fields_ = [
        ('data', (uint8_t*160)),
        ('length', uint16_t),
    ]
ipc_payload_hci_data_type=random_type_341
class random_type_342(Structure):
    _fields_ = [
        ('clock_type', uint32_t),
    ]
ipc_payload_get_cal_freq_req_t=random_type_342
class random_type_343(Structure):
    _fields_ = [
        ('result', uint32_t),
    ]
ipc_payload_get_cal_freq_cnf_t=random_type_343
class random_type_344(Structure):
    _fields_ = [
        ('status', uint16_t),
        ('length', uint16_t),
        ('msg', (td_char*60)),
    ]
ipc_payload_test_suite_string=random_type_344
class random_type_345(Structure):
    _fields_ = [
        ('flag', td_bool),
    ]
ipc_payload_bt_power_on_t=random_type_345
class random_type_346(Structure):
    _fields_ = [
        ('flag', td_bool),
    ]
ipc_payload_dsp_power_on_t=random_type_346
class random_type_347(Structure):
    _fields_ = [
        ('flag', td_bool),
    ]
ipc_payload_temperature_t=random_type_347
class random_type_348(Structure):
    _fields_ = [
        ('updata', uint32_t),
    ]
ipc_payload_updata_trng_t=random_type_348
class random_type_349(Structure):
    _fields_ = [
        ('slave_freq', uint32_t),
    ]
ipc_payload_adjust_slave_freq_t=random_type_349
class random_type_350(Structure):
    _fields_ = [
        ('freq', uint32_t),
    ]
ipc_payload_dsp_current_freq_t=random_type_350
class random_type_351(Structure):
    _fields_ = [
        ('power_switch', uint32_t),
    ]
ipc_payload_high_power_t=random_type_351
int8_t=td_char
class random_type_352(Structure):
    _fields_ = [
        ('config_nv_1', uint16_t),
        ('config_nv_2', uint16_t),
        ('config_nv_3', uint16_t),
        ('config_nv_4', uint16_t),
        ('config_nv_5', (uint16_t*5)),
        ('config_nv_6', uint16_t),
        ('config_nv_7', uint16_t),
        ('config_nv_8', uint16_t),
        ('config_nv_9', uint16_t),
        ('config_nv_10', uint8_t),
        ('config_nv_11', int8_t),
        ('config_nv_12', int8_t),
        ('flag', uint8_t),
    ]
ipc_payload_config_nv_info_t=random_type_352
class random_type_353(Structure):
    _fields_ = [
        ('nv_customize_config', uint32_t),
        ('nv_customize_config_reserved', uint32_t),
        ('nv_max_ble_tx_power_cfg_val', uint8_t),
        ('nv_max_gle_tx_power_cfg_val', uint8_t),
        ('fcc_flag', uint8_t),
        ('customize_four_k_support', uint8_t),
        ('customize_freq_map', (uint8_t*10)),
        ('reserved', (uint8_t*86)),
    ]
ipc_payload_bt_nv_customize_info_t=random_type_353
ipc_action_handler=td_u32
ipc_idle_callback_handler=td_u32
rtc_handle_t=td_u32
rtc_callback_t=td_u32
lpc_fsm_callback_handler=td_u32
class random_type_354(Structure):
    _fields_ = [
        ('lpc_sleep_vetos', uint16_t),
        ('lr_pointer', uint32_t),
    ]
lpc_veto_type=random_type_354
class random_type_356(Structure):
    _fields_ = [
        ('source_ptr', uint32_t),
        ('dest_ptr', uint32_t),
        ('length', uint16_t),
    ]
class random_type_359(Structure):
    _fields_ = [
        ('reboot_cause', uint32_t),
    ]
class random_type_360(Structure):
    _fields_ = [
        ('data', (uint8_t*3072)),
    ]
class random_type_361(Structure):
    _fields_ = [
        ('requested_reboot_reason', uint16_t),
    ]
class random_type_362(Structure):
    _fields_ = [
        ('log_level', uint32_t),
    ]
class random_type_365(Structure):
    _fields_ = [
        ('size', uint32_t),
    ]
class random_type_366(Structure):
    _fields_ = [
        ('result', uint32_t),
    ]
class random_type_367(Structure):
    _fields_ = [
        ('params', (uint8_t*256)),
        ('length', uint32_t),
    ]
class random_type_369(Structure):
    _fields_ = [
        ('type', uint8_t),
        ('event_id', uint8_t),
        ('info1', uint8_t),
        ('info2', uint8_t),
        ('info3', uint16_t),
    ]
class random_type_370(Structure):
    _fields_ = [
        ('mass_data', ipc_payload_mass_data_type),
        ('data_len', uint8_t),
        ('data', (uint8_t*128)),
    ]
class random_type_371(Structure):
    _fields_ = [
        ('data', (uint8_t*160)),
        ('length', uint16_t),
    ]
class random_type_372(Structure):
    _fields_ = [
        ('clock_type', uint32_t),
    ]
class random_type_373(Structure):
    _fields_ = [
        ('result', uint32_t),
    ]
class random_type_374(Structure):
    _fields_ = [
        ('status', uint16_t),
        ('length', uint16_t),
        ('msg', (td_char*60)),
    ]
class random_type_375(Structure):
    _fields_ = [
        ('flag', td_bool),
    ]
class random_type_376(Structure):
    _fields_ = [
        ('flag', td_bool),
    ]
class random_type_377(Structure):
    _fields_ = [
        ('flag', td_bool),
    ]
class random_type_378(Structure):
    _fields_ = [
        ('updata', uint32_t),
    ]
class random_type_379(Structure):
    _fields_ = [
        ('slave_freq', uint32_t),
    ]
class random_type_380(Structure):
    _fields_ = [
        ('freq', uint32_t),
    ]
class random_type_381(Structure):
    _fields_ = [
        ('power_switch', uint32_t),
    ]
class random_type_382(Structure):
    _fields_ = [
        ('config_nv_1', uint16_t),
        ('config_nv_2', uint16_t),
        ('config_nv_3', uint16_t),
        ('config_nv_4', uint16_t),
        ('config_nv_5', (uint16_t*5)),
        ('config_nv_6', uint16_t),
        ('config_nv_7', uint16_t),
        ('config_nv_8', uint16_t),
        ('config_nv_9', uint16_t),
        ('config_nv_10', uint8_t),
        ('config_nv_11', int8_t),
        ('config_nv_12', int8_t),
        ('flag', uint8_t),
    ]
class random_type_383(Structure):
    _fields_ = [
        ('nv_customize_config', uint32_t),
        ('nv_customize_config_reserved', uint32_t),
        ('nv_max_ble_tx_power_cfg_val', uint8_t),
        ('nv_max_gle_tx_power_cfg_val', uint8_t),
        ('fcc_flag', uint8_t),
        ('customize_four_k_support', uint8_t),
        ('customize_freq_map', (uint8_t*10)),
        ('reserved', (uint8_t*86)),
    ]
class nand_driver_struct(Structure):
    _fields_ = [
        ('nand_flash_init', td_u32),
        ('check_id', td_u32),
        ('read_page', td_u32),
        ('write_page', td_u32),
        ('erase_block', td_u32),
        ('mark_bad_block', td_u32),
        ('check_bad_block', td_u32),
        ('bytes_per_page', uint32_t),
        ('bytes_per_oob', uint32_t),
        ('pages_per_block', uint32_t),
        ('total_block', uint32_t),
        ('chip_id', uint32_t),
        ('oob_total_len', uint16_t),
        ('oob1_per_size', uint16_t),
        ('oob2_per_size', uint16_t),
        ('oob3_per_size', uint16_t),
        ('oob4_per_size', uint16_t),
        ('oob1_offset', uint16_t),
        ('oob2_offset', uint16_t),
        ('oob3_offset', uint16_t),
        ('oob4_offset', uint16_t),
        ('enable_ecc', td_bool),
    ]
u_char=td_uchar
u_int=td_u32
class random_type_385(Structure):
    _fields_ = [
        ('sem', td_u32),
    ]
class random_type_387(Structure):
    _fields_ = [
        ('dfs', uint32_t,4),
        ('frf', uint32_t,2),
        ('scph', uint32_t,1),
        ('scpol', uint32_t,1),
        ('tmod', uint32_t,2),
        ('slv_oe', uint32_t,1),
        ('srl', uint32_t,1),
        ('cfs', uint32_t,4),
        ('dfs_32', uint32_t,5),
        ('spi_frf', uint32_t,2),
        ('rsvd_clt0_23', uint32_t,1),
        ('sste', uint32_t,1),
        ('rsvd0', uint32_t,7),
    ]
class _hal_spi_interface_ctrlr0_reg(Union):
    _fields_ = [
        ('d', random_type_387),
        ('d32', uint32_t),
    ]
hal_spi_interface_ctrlr0_reg=_hal_spi_interface_ctrlr0_reg
class random_type_388(Structure):
    _fields_ = [
        ('trans_type', uint32_t,2),
        ('addr_l', uint32_t,4),
        ('rsvd_spi_ctrlr0_6_7', uint32_t,2),
        ('inst_l', uint32_t,2),
        ('rsvd_spi_ctrlr0_10', uint32_t,1),
        ('wait_cycles', uint32_t,5),
        ('spi_ddr_en', uint32_t,1),
        ('inst_ddr_en', uint32_t,1),
        ('spi_rxds_en', uint32_t,1),
        ('rsvd_spi_ctrlr0', uint32_t,13),
    ]
class _hal_spi_interface_spi_ctrlr0_reg(Union):
    _fields_ = [
        ('d', random_type_388),
        ('d32', uint32_t),
    ]
hal_spi_interface_spi_ctrlr0_reg=_hal_spi_interface_spi_ctrlr0_reg
class random_type_389(Structure):
    _fields_ = [
        ('mutex', td_u32),
    ]
ext_errno=td_u32
hal_pin_set_mode=td_u32
hal_pin_get_mode=td_u32
hal_pin_set_ds=td_u32
hal_pin_get_ds=td_u32
hal_pin_set_pull=td_u32
hal_pin_get_pull=td_u32
hal_pin_set_ie=td_u32
hal_pin_get_ie=td_u32
class random_type_391(Structure):
    _fields_ = [
        ('set_mode', hal_pin_set_mode),
        ('get_mode', hal_pin_get_mode),
        ('set_ds', hal_pin_set_ds),
        ('get_ds', hal_pin_get_ds),
        ('set_pull', hal_pin_set_pull),
        ('get_pull', hal_pin_get_pull),
        ('set_ie', hal_pin_set_ie),
        ('get_ie', hal_pin_get_ie),
    ]
hal_pin_funcs_t=random_type_391
class hal_pmpx_config(Structure):
    _fields_ = [
        ('rwx', uint8_t,3),
        ('a', uint8_t,2),
        ('resv_0', uint8_t,2),
        ('l', uint8_t,1),
    ]
hal_pmpx_config_t=hal_pmpx_config
class hal_pmp_conf(Structure):
    _fields_ = [
        ('idx', uint32_t),
        ('addr', uint32_t),
        ('attr', uint8_t),
        ('cfg', hal_pmpx_config_t),
    ]
hal_pmp_conf_t=hal_pmp_conf
hal_pmp_config=td_u32
class hal_pmp_funcs(Structure):
    _fields_ = [
        ('config', hal_pmp_config),
    ]
hal_pmp_funcs_t=hal_pmp_funcs
class random_type_393(Union):
    _fields_ = [
        ('words', (uint32_t*16)),
        ('bytes', (uint8_t*64)),
    ]
drv_cipher_hash_type_t=td_u32
class random_type_394(Structure):
    _fields_ = [
        ('buffer', random_type_393),
        ('hash', (uint32_t*8)),
        ('total_len', uint32_t),
        ('buffer_len', uint32_t),
        ('block_len', uint32_t),
        ('words_len', uint32_t),
        ('result_len', uint32_t),
        ('timeout_ms', uint32_t),
        ('hash_type', drv_cipher_hash_type_t),
    ]
hash_context_t=random_type_394
drv_cipher_buffer_secure_t=td_u32
class random_type_392(Structure):
    _fields_ = [
        ('ctx', hash_context_t),
        ('buf_sec', drv_cipher_buffer_secure_t),
    ]
hash_handle_t=random_type_392
hash_init_func_t=td_u32
hash_deinit_func_t=td_u32
hash_update_func_t=td_u32
hash_final_func_t=td_u32
class random_type_396(Structure):
    _fields_ = [
        ('init', hash_init_func_t),
        ('deinit', hash_deinit_func_t),
        ('update', hash_update_func_t),
        ('final', hash_final_func_t),
    ]
hash_func_t=random_type_396
class random_type_395(Structure):
    _fields_ = [
        ('hash_type', drv_cipher_hash_type_t),
        ('block_len', uint32_t),
        ('result_len', uint32_t),
        ('words_len', uint32_t),
        ('hash_func', hash_func_t),
    ]
hash_properties_t=random_type_395
sha_mode_t=td_u32
class random_type_397(Structure):
    _fields_ = [
        ('hash_type', drv_cipher_hash_type_t),
        ('keyslot', uint32_t),
    ]
drv_cipher_hash_attr_t=random_type_397
hal_sha_init_t=td_u32
hal_sha_deinit_t=td_u32
hal_sha_start_t=td_u32
hal_sha_finish_t=td_u32
class random_type_398(Structure):
    _fields_ = [
        ('init', hal_sha_init_t),
        ('deinit', hal_sha_deinit_t),
        ('start', hal_sha_start_t),
        ('finish', hal_sha_finish_t),
    ]
hal_sha_funcs_t=random_type_398
hal_sha_callback_t=td_u32
class sha_attr(Structure):
    _fields_ = [
        ('input_addr', uint32_t),
        ('output_addr', uint32_t),
        ('data_length', uint32_t),
        ('total_length', uint32_t),
        ('sha_mode', sha_mode_t),
        ('is_iv_exist', td_bool),
        ('is_last_package', td_bool),
    ]
sha_attr_t=sha_attr
class random_type_399(Structure):
    _fields_ = [
        ('address', td_u32),
        ('buf_sec', drv_cipher_buffer_secure_t),
    ]
drv_cipher_buf_attr_t=random_type_399
class random_type_400(Structure):
    _fields_ = [
        ('hash_type', drv_cipher_hash_type_t),
        ('keyslot', uint32_t),
    ]
class random_type_401(Structure):
    _fields_ = [
        ('address', td_u32),
        ('buf_sec', drv_cipher_buffer_secure_t),
    ]
uapi_drv_cipher_hash_type_t=td_u32
class uapi_drv_hash_attr(Structure):
    _fields_ = [
        ('key', td_u32),
        ('key_len', uint32_t),
        ('keyslot_handle', uint32_t),
        ('hash_type', uapi_drv_cipher_hash_type_t),
        ('is_keyslot', td_bool),
        ('is_long_term', td_bool),
    ]
uapi_drv_cipher_hash_attr_t=uapi_drv_hash_attr
drv_pke_hash_type_t=td_u32
class random_type_402(Structure):
    _fields_ = [
        ('hash_type', drv_pke_hash_type_t),
        ('block_len', uint32_t),
        ('result_len', uint32_t),
    ]
pke_hash_properties_type_t=random_type_402
drv_pke_rsa_scheme_t=td_u32
class drv_pke_data(Structure):
    _fields_ = [
        ('length', uint32_t),
        ('data', td_u32),
    ]
drv_pke_data_t=drv_pke_data
class random_type_403(Structure):
    _fields_ = [
        ('klen', uint32_t),
        ('em_bit', uint32_t),
        ('em', td_u32),
        ('em_len', uint32_t),
        ('hash', td_u32),
        ('hash_len', uint32_t),
        ('data', td_u32),
        ('data_len', uint32_t),
    ]
rsa_pkcs1_pack_t=random_type_403
class random_type_404(Structure):
    _fields_ = [
        ('hash_type', uapi_drv_cipher_hash_type_t),
        ('hash_len', uint32_t),
        ('lhash_data', td_u32),
        ('asn1_data', td_u32),
        ('asn1_len', uint32_t),
    ]
rsa_pkcs1_hash_info_t=random_type_404
hal_rsa_init_t=td_u32
hal_rsa_deinit_t=td_u32
hal_rsa_start_t=td_u32
hal_rsa_finish_t=td_u32
class hal_rsa_funcs(Structure):
    _fields_ = [
        ('init', hal_rsa_init_t),
        ('deinit', hal_rsa_deinit_t),
        ('start', hal_rsa_start_t),
        ('finish', hal_rsa_finish_t),
    ]
hal_rsa_funcs_t=hal_rsa_funcs
hal_rsa_callback_t=td_u32
class rsa_attr(Structure):
    _fields_ = [
        ('ptext_addr', uint32_t),
        ('key_addr', uint32_t),
        ('output_addr', uint32_t),
        ('key_len', uint32_t),
    ]
rsa_attr_t=rsa_attr
class drv_pke_rsa_pub_key(Structure):
    _fields_ = [
        ('n', td_u32),
        ('e', td_u32),
        ('len', uint16_t),
    ]
drv_pke_rsa_pub_key_t=drv_pke_rsa_pub_key
class drv_pke_rsa_priv_key(Structure):
    _fields_ = [
        ('n', td_u32),
        ('e', td_u32),
        ('d', td_u32),
        ('p', td_u32),
        ('q', td_u32),
        ('dp', td_u32),
        ('dq', td_u32),
        ('qp', td_u32),
        ('n_len', uint16_t),
        ('e_len', uint16_t),
        ('d_len', uint16_t),
        ('p_len', uint16_t),
        ('q_len', uint16_t),
        ('dp_len', uint16_t),
        ('dq_len', uint16_t),
        ('qp_len', uint16_t),
    ]
drv_pke_rsa_priv_key_t=drv_pke_rsa_priv_key
class random_type_405(Structure):
    _fields_ = [
        ('n', td_u32),
        ('e', td_u32),
        ('d', td_u32),
        ('p', td_u32),
        ('q', td_u32),
        ('dp', td_u32),
        ('dq', td_u32),
        ('qp', td_u32),
        ('n_len', uint16_t),
        ('e_len', uint16_t),
        ('d_len', uint16_t),
        ('p_len', uint16_t),
        ('q_len', uint16_t),
        ('dp_len', uint16_t),
        ('dq_len', uint16_t),
        ('qp_len', uint16_t),
    ]
uapi_drv_cipher_pke_rsa_priv_key_t=random_type_405
uapi_drv_cipher_pke_rsa_scheme_t=td_u32
uapi_drv_cipher_pke_hash_type_t=td_u32
class random_type_406(Structure):
    _fields_ = [
        ('length', uint32_t),
        ('data', td_u32),
    ]
uapi_drv_cipher_pke_data_t=random_type_406
class random_type_407(Structure):
    _fields_ = [
        ('n', td_u32),
        ('e', td_u32),
        ('len', uint16_t),
    ]
uapi_drv_cipher_pke_rsa_pub_key_t=random_type_407
hal_spi_init_t=td_u32
hal_spi_deinit_t=td_u32
hal_spi_write_t=td_u32
hal_spi_read_t=td_u32
hal_spi_writeread_t=td_u32
hal_spi_ctrl_t=td_u32
class random_type_408(Structure):
    _fields_ = [
        ('init', hal_spi_init_t),
        ('deinit', hal_spi_deinit_t),
        ('write', hal_spi_write_t),
        ('read', hal_spi_read_t),
        ('writeread', hal_spi_writeread_t),
        ('ctrl', hal_spi_ctrl_t),
    ]
hal_spi_funcs_t=random_type_408
hal_spi_callback_t=td_u32
class spi_dma_trans_inf(Structure):
    _fields_ = [
        ('trans_succ', td_bool),
        ('is_enable', td_bool),
        ('channel', uint8_t),
        ('reserved', uint8_t),
        ('dma_sem', osal_semaphore),
    ]
spi_dma_trans_inf_t=spi_dma_trans_inf
class random_type_409(Structure):
    _fields_ = [
        ('sem', td_u32),
    ]
class spi_dma_config(Structure):
    _fields_ = [
        ('src_width', uint8_t),
        ('dest_width', uint8_t),
        ('burst_length', uint8_t),
        ('priority', uint8_t),
    ]
spi_dma_config_t=spi_dma_config
class hal_spi_dma_cfg_param(Structure):
    _fields_ = [
        ('is_enable', td_bool),
        ('dma_rx_level', uint8_t),
        ('dma_tx_level', uint8_t),
    ]
hal_spi_dma_cfg_param_t=hal_spi_dma_cfg_param
class spi_crc_config(Structure):
    _fields_ = [
        ('tx_crc_len', uint32_t),
        ('rx_crc_len', uint32_t),
        ('tx_crc_ini', uint32_t),
        ('rx_crc_ini', uint32_t),
        ('tx_crc_poly', uint32_t),
        ('rx_crc_poly', uint32_t),
        ('tx_crc_xor_out', uint32_t),
        ('rx_crc_xor_out', uint32_t),
        ('tx_crc_refin', td_bool),
        ('tx_crc_refout', td_bool),
        ('rx_crc_refin', td_bool),
        ('rx_crc_refout', td_bool),
    ]
spi_crc_config_t=spi_crc_config
spi_crc_err_callback_t=td_u32
hal_systick_init_t=td_u32
hal_systick_deinit_t=td_u32
hal_systick_ctrl_t=td_u32
class random_type_410(Structure):
    _fields_ = [
        ('init', hal_systick_init_t),
        ('deinit', hal_systick_deinit_t),
        ('ctrl', hal_systick_ctrl_t),
    ]
hal_systick_funcs_t=random_type_410
hal_tcxo_init_t=td_u32
hal_tcxo_deinit_t=td_u32
hal_tcxo_get_t=td_u32
class random_type_411(Structure):
    _fields_ = [
        ('init', hal_tcxo_init_t),
        ('deinit', hal_tcxo_deinit_t),
        ('get', hal_tcxo_get_t),
    ]
hal_tcxo_funcs_t=random_type_411
timer_handle_t=td_u32
timer_callback_t=td_u32
class soft_timer_cfg(Structure):
    _fields_ = [
        ('timer_addr', uintptr_t),
        ('timer_num', uint8_t),
    ]
soft_timer_cfg_t=soft_timer_cfg
class timer_irq_info(Structure):
    _fields_ = [
        ('irq', uint32_t),
        ('priority', uint16_t),
    ]
timer_irq_info_t=timer_irq_info
high_precision_timer_callback_t=td_u32
uart_tx_callback_t=td_u32
class random_type_413(Structure):
    _fields_ = [
        ('data', td_u32),
        ('params', td_u32),
        ('release_func', uart_tx_callback_t),
        ('data_length', uint32_t),
    ]
uart_tx_fragment_t=random_type_413
class random_type_412(Structure):
    _fields_ = [
        ('current_tx_fragment', td_u32),
        ('free_tx_fragment', td_u32),
        ('fragments_to_process', uint16_t),
        ('current_tx_fragment_pos', uint16_t),
        ('fragment_buffer', (uart_tx_fragment_t*4)),
    ]
uart_tx_state_t=random_type_412
uart_rx_callback_t=td_u32
uart_error_callback_t=td_u32
uart_attr_t=uart_attr
class uart_buffer_config(Structure):
    _fields_ = [
        ('rx_buffer', td_u32),
        ('rx_buffer_size', size_t),
    ]
uart_buffer_config_t=uart_buffer_config
class random_type_416(Structure):
    _fields_ = [
        ('init', hal_uart_init_t),
        ('deinit', hal_uart_deinit_t),
        ('ctrl', hal_uart_ctrl_t),
        ('write', hal_uart_write_t),
        ('read', hal_uart_read_t),
        ('dma_cfg', hal_uart_dma_cfg_t),
    ]
hal_uart_funcs_t=random_type_416
uart_extra_attr_t=uart_extra_attr
hal_watchdog_init_t=td_u32
hal_watchdog_deinit_t=td_u32
hal_watchdog_set_attr_t=td_u32
hal_watchdog_get_attr_t=td_u32
hal_watchdog_enable_t=td_u32
hal_watchdog_disable_t=td_u32
hal_watchdog_kick_t=td_u32
hal_watchdog_get_time_t=td_u32
hal_register_watchdog_callback_t=td_u32
class random_type_417(Structure):
    _fields_ = [
        ('init', hal_watchdog_init_t),
        ('deinit', hal_watchdog_deinit_t),
        ('set_attr', hal_watchdog_set_attr_t),
        ('get_attr', hal_watchdog_get_attr_t),
        ('enable', hal_watchdog_enable_t),
        ('disable', hal_watchdog_disable_t),
        ('kick', hal_watchdog_kick_t),
        ('get_left_time', hal_watchdog_get_time_t),
        ('register_callback', hal_register_watchdog_callback_t),
    ]
hal_watchdog_funcs_t=random_type_417
hal_watchdog_callback_t=td_u32
watchdog_callback_t=td_u32
linklist=td_u32
class Node(Structure):
    _fields_ = [
        ('type', td_u32),
        ('status', td_s32),
        ('mtd', td_u32),
        ('next', td_u32),
    ]
class block_operations(Structure):
    _fields_ = [
        ('open', td_u32),
        ('close', td_u32),
        ('read', td_u32),
        ('write', td_u32),
        ('writev', td_u32),
        ('geometry', td_u32),
        ('ioctl', td_u32),
        ('unlink', td_u32),
    ]
int16_t=td_s16
class inode_ops_u(Union):
    _fields_ = [
        ('i_ops', td_u32),
        ('i_bops', td_u32),
        ('i_mops', td_u32),
    ]
class file_operations_vfs(Structure):
    _fields_ = [
        ('open', td_u32),
        ('close', td_u32),
        ('read', td_u32),
        ('write', td_u32),
        ('seek', td_u32),
        ('ioctl', td_u32),
        ('unlink', td_u32),
    ]
class mountpt_operations(Structure):
    _fields_ = [
        ('open', td_u32),
        ('close', td_u32),
        ('read', td_u32),
        ('write', td_u32),
        ('seek', td_u32),
        ('ioctl', td_u32),
        ('sync', td_u32),
        ('dup', td_u32),
        ('fstat', td_u32),
        ('truncate', td_u32),
        ('opendir', td_u32),
        ('closedir', td_u32),
        ('readdir', td_u32),
        ('rewinddir', td_u32),
        ('bind', td_u32),
        ('unbind', td_u32),
        ('statfs', td_u32),
        ('virstatfs', td_u32),
        ('unlink', td_u32),
        ('mkdir', td_u32),
        ('rmdir', td_u32),
        ('rename', td_u32),
        ('stat', td_u32),
        ('utime', td_u32),
        ('chattr', td_u32),
        ('seek64', td_u32),
        ('getlabel', td_u32),
        ('fallocate', td_u32),
        ('fallocate64', td_u32),
        ('truncate64', td_u32),
        ('fscheck', td_u32),
    ]
class fs_dirent_s(Structure):
    _fields_ = [
    ]
class __fsid_t(Structure):
    _fields_ = [
        ('__val', (td_s32*2)),
    ]
fsid_t=__fsid_t
class soft_rtc_cfg(Structure):
    _fields_ = [
        ('rtc_addr', uintptr_t),
        ('rtc_num', uint8_t),
    ]
soft_rtc_cfg_t=soft_rtc_cfg
hal_efuse_init_t=td_u32
hal_efuse_deinit_t=td_u32
hal_efuse_flush_write_t=td_u32
hal_efuse_refresh_read_t=td_u32
hal_efuse_read_byte_t=td_u32
hal_efuse_write_byte_t=td_u32
hal_efuse_clear_all_write_regs_t=td_u32
hal_efuse_write_operation_t=td_u32
hal_efuse_write_buffer_operation_t=td_u32
hal_efuse_get_die_id_t=td_u32
hal_efuse_get_chip_id_t=td_u32
class random_type_418(Structure):
    _fields_ = [
        ('init', hal_efuse_init_t),
        ('deinit', hal_efuse_deinit_t),
        ('flush_write', hal_efuse_flush_write_t),
        ('refresh_read', hal_efuse_refresh_read_t),
        ('read_byte', hal_efuse_read_byte_t),
        ('write_byte', hal_efuse_write_byte_t),
        ('clear', hal_efuse_clear_all_write_regs_t),
        ('write_op', hal_efuse_write_operation_t),
        ('write_buffer_op', hal_efuse_write_buffer_operation_t),
        ('get_die_id', hal_efuse_get_die_id_t),
        ('get_chip_id', hal_efuse_get_chip_id_t),
    ]
hal_efuse_funcs_t=random_type_418
class random_type_419(Structure):
    _fields_ = [
        ('cpu_info_base_addr', uint32_t),
        ('pc_low_offset', uint32_t),
        ('pc_high_offset', uint32_t),
        ('lr_low_offset', uint32_t),
        ('lr_high_offset', uint32_t),
        ('cpu_diag_offset', uint32_t),
        ('cpu_lock_enable', uint32_t),
    ]
hal_cpu_info_t=random_type_419
class random_type_420(Structure):
    _fields_ = [
        ('int_en', uint32_t,1),
        ('dst_tr_width', uint32_t,3),
        ('src_tr_width', uint32_t,3),
        ('dinc', uint32_t,2),
        ('sinc', uint32_t,2),
        ('dest_msize', uint32_t,3),
        ('src_msize', uint32_t,3),
        ('src_gather_en', uint32_t,1),
        ('dst_scatter_en', uint32_t,1),
        ('reserved0', uint32_t,1),
        ('tt_fc', uint32_t,3),
        ('dms', uint32_t,2),
        ('sms', uint32_t,2),
        ('llp_dst_en', uint32_t,1),
        ('llp_src_en', uint32_t,1),
    ]
class dma_ctrl_l_data(Union):
    _fields_ = [
        ('d32', uint32_t),
        ('b', random_type_420),
    ]
dma_ctrl_l_data_t=dma_ctrl_l_data
class random_type_421(Structure):
    _fields_ = [
        ('block_ts', uint32_t,12),
        ('done', uint32_t,1),
    ]
class dma_ctrl_h_data(Union):
    _fields_ = [
        ('d32', uint32_t),
        ('b', random_type_421),
    ]
dma_ctrl_h_data_t=dma_ctrl_h_data
class hal_dma_lli(Structure):
    _fields_ = [
        ('src_addr', uint32_t),
        ('dst_addr', uint32_t),
        ('next', td_u32),
        ('ctl_l', dma_ctrl_l_data_t),
        ('ctl_h', dma_ctrl_h_data_t),
        ('src_stat', uint32_t),
        ('dst_stat', uint32_t),
    ]
hal_dma_lli_t=hal_dma_lli
class dma_ch_cfg(Structure):
    _fields_ = [
        ('src', uint32_t),
        ('src_reserved', uint32_t),
        ('dst', uint32_t),
        ('dst_reserved', uint32_t),
        ('llp', uint32_t),
        ('llp_reserved', uint32_t),
        ('ctl_l', uint32_t),
        ('ctl_h', uint32_t),
        ('sstat', uint32_t),
        ('sstat_reserved', uint32_t),
        ('dstat', uint32_t),
        ('dstat_reserved', uint32_t),
        ('sstatar', uint32_t),
        ('sstatar_reserved', uint32_t),
        ('dstatar', uint32_t),
        ('dstatar_reserved', uint32_t),
        ('cfg_l', uint32_t),
        ('cfg_h', uint32_t),
        ('sgr', uint32_t),
        ('sgr_reserved', uint32_t),
        ('dsr', uint32_t),
        ('dsr_reserved', uint32_t),
    ]
dma_ch_cfg_t=dma_ch_cfg
class dma_int_type(Structure):
    _fields_ = [
        ('tfr', uint32_t),
        ('tfr_reserved', uint32_t),
        ('block', uint32_t),
        ('block_reserved', uint32_t),
        ('src_tran', uint32_t),
        ('src_tran_reserved', uint32_t),
        ('dst_tran', uint32_t),
        ('dst_tran_reserved', uint32_t),
        ('err', uint32_t),
        ('err_reserved', uint32_t),
    ]
dma_int_type_t=dma_int_type
class dma_regs(Structure):
    _fields_ = [
        ('dma_ch_config', (dma_ch_cfg_t*8)),
        ('dma_interrupt_type', (dma_int_type_t*4)),
        ('dma_sts_int', uint32_t),
        ('dma_sts_int_reserved', uint32_t),
        ('dma_hs_src', uint32_t),
        ('dma_hs_src_reserved', uint32_t),
        ('dma_hs_dst', uint32_t),
        ('dma_hs_dst_reserved', uint32_t),
        ('dma_hs_sgl_src', uint32_t),
        ('dma_hs_sgl_src_reserved', uint32_t),
        ('dma_hs_sgl_dst', uint32_t),
        ('dma_hs_sgl_dst_reserved', uint32_t),
        ('dma_hs_lst_src', uint32_t),
        ('dma_hs_lst_src_reserved', uint32_t),
        ('dma_hs_lst_dst', uint32_t),
        ('dma_hs_lst_dst_reserved', uint32_t),
        ('dma_cfg_reg', uint32_t),
        ('reserved', uint32_t),
        ('dma_ch_en', uint32_t),
    ]
dma_regs_t=dma_regs
class random_type_422(Structure):
    _fields_ = [
        ('tfr', uint32_t,1),
        ('block', uint32_t,1),
        ('srct', uint32_t,1),
        ('dstt', uint32_t,1),
        ('err', uint32_t,1),
    ]
class dma_int_status_data(Union):
    _fields_ = [
        ('d32', uint32_t),
        ('b', random_type_422),
    ]
dma_int_status_data_t=dma_int_status_data
class random_type_423(Structure):
    _fields_ = [
        ('fcmode', uint32_t,1),
        ('fifo_mode', uint32_t,1),
        ('protctl', uint32_t,3),
        ('ds_upd_en', uint32_t,1),
        ('ss_upd_en', uint32_t,1),
        ('src_per', uint32_t,4),
        ('dest_per', uint32_t,4),
        ('reserved', uint32_t,17),
    ]
class dma_cfg_h_data(Union):
    _fields_ = [
        ('d32', uint32_t),
        ('b', random_type_423),
    ]
dma_cfg_h_data_t=dma_cfg_h_data
class random_type_424(Structure):
    _fields_ = [
        ('dma_enable', uint32_t,1),
    ]
class dma_cfg_reg_data(Union):
    _fields_ = [
        ('d32', uint32_t),
        ('b', random_type_424),
    ]
dma_cfg_reg_data_t=dma_cfg_reg_data
class random_type_425(Structure):
    _fields_ = [
        ('reserved0', uint32_t,5),
        ('ch_prior', uint32_t,3),
        ('ch_susp', uint32_t,1),
        ('fifo_empty', uint32_t,1),
        ('hs_sel_dst', uint32_t,1),
        ('hs_sel_src', uint32_t,1),
        ('lock_ch_l', uint32_t,2),
        ('lock_b_l', uint32_t,2),
        ('lock_ch', uint32_t,1),
        ('lock_b', uint32_t,1),
        ('dst_hs_pol', uint32_t,1),
        ('src_hs_pol', uint32_t,1),
        ('max_abrst', uint32_t,10),
        ('reload_src', uint32_t,1),
        ('reload_dst', uint32_t,1),
    ]
class dma_cfg_l_data(Union):
    _fields_ = [
        ('d32', uint32_t),
        ('b', random_type_425),
    ]
dma_cfg_l_data_t=dma_cfg_l_data
class random_type_426(Structure):
    _fields_ = [
        ('init', hal_gpio_init_t),
        ('deinit', hal_gpio_deinit_t),
        ('setdir', hal_gpio_setdir_t),
        ('getdir', hal_gpio_getdir_t),
        ('ctrl', hal_gpio_ctrl_t),
        ('output', hal_gpio_output_t),
        ('get_output_val', hal_gpio_getoutval_t),
        ('input', hal_gpio_input_t),
        ('registerfunc', hal_gpio_register_t),
        ('unregisterfunc', hal_gpio_unregister_t),
    ]
class random_type_428(Structure):
    _fields_ = [
        ('flag', td_bool),
        ('callback', gpio_callback_t),
    ]
hal_gpio_interrupt_flag_t=random_type_428
class random_type_430(Structure):
    _fields_ = [
        ('init', hal_gpio_init_t),
        ('deinit', hal_gpio_deinit_t),
        ('setdir', hal_gpio_setdir_t),
        ('getdir', hal_gpio_getdir_t),
        ('ctrl', hal_gpio_ctrl_t),
        ('output', hal_gpio_output_t),
        ('get_output_val', hal_gpio_getoutval_t),
        ('input', hal_gpio_input_t),
        ('registerfunc', hal_gpio_register_t),
        ('unregisterfunc', hal_gpio_unregister_t),
    ]
class gpio_info_stru(Structure):
    _fields_ = [
        ('gpio_swporta_dr', uint32_t),
        ('gpio_swporta_ddr', uint32_t),
        ('gpio_swporta_ctl', uint32_t),
        ('gpio_swportb_dr', uint32_t),
        ('gpio_swportb_ddr', uint32_t),
        ('gpio_swportb_ctl', uint32_t),
        ('gpio_swportc_dr', uint32_t),
        ('gpio_swportc_ddr', uint32_t),
        ('gpio_swportc_ctl', uint32_t),
        ('gpio_swportd_dr', uint32_t),
        ('gpio_swportd_ddr', uint32_t),
        ('gpio_swportd_ctl', uint32_t),
        ('gpio_inten', uint32_t),
        ('gpio_intmask', uint32_t),
        ('gpio_inttype_level', uint32_t),
        ('gpio_int_polarity', uint32_t),
        ('gpio_intstatus', uint32_t),
        ('gpio_raw_intstatus', uint32_t),
        ('gpio_debounce', uint32_t),
        ('gpio_porta_eoi', uint32_t),
        ('gpio_ext_porta', uint32_t),
        ('gpio_ext_portb', uint32_t),
        ('gpio_ext_portc', uint32_t),
        ('gpio_ext_portd', uint32_t),
        ('gpio_ls_sync', uint32_t),
        ('gpio_id_code', uint32_t),
        ('gpio_int_bothedge', uint32_t),
        ('gpio_ver_id_code', uint32_t),
        ('gpio_config_reg2', uint32_t),
        ('gpio_config_reg1', uint32_t),
    ]
gpio_info_stru_t=gpio_info_stru
class random_type_431(Structure):
    _fields_ = [
        ('interrupt_status', uint32_t),
    ]
class gpio_intstatus_data(Union):
    _fields_ = [
        ('d32', uint32_t),
        ('b', random_type_431),
    ]
gpio_intstatus_data_t=gpio_intstatus_data
class random_type_432(Structure):
    _fields_ = [
        ('interrupt_level', uint32_t),
    ]
class gpio_inttype_level_data(Union):
    _fields_ = [
        ('d32', uint32_t),
        ('b', random_type_432),
    ]
gpio_inttype_level_data_t=gpio_inttype_level_data
class random_type_433(Structure):
    _fields_ = [
        ('clear_interrupt_w', uint32_t),
    ]
class gpio_porta_eoi_data(Union):
    _fields_ = [
        ('d32', uint32_t),
        ('b', random_type_433),
    ]
gpio_porta_eoi_data_t=gpio_porta_eoi_data
class random_type_434(Structure):
    _fields_ = [
        ('port_a_data_direction', uint32_t),
    ]
class gpio_swporta_ddr_data(Union):
    _fields_ = [
        ('d32', uint32_t),
        ('b', random_type_434),
    ]
gpio_swporta_ddr_data_t=gpio_swporta_ddr_data
class random_type_435(Structure):
    _fields_ = [
        ('debounce_enable', uint32_t),
    ]
class gpio_debounce_data(Union):
    _fields_ = [
        ('d32', uint32_t),
        ('b', random_type_435),
    ]
gpio_debounce_data_t=gpio_debounce_data
class random_type_436(Structure):
    _fields_ = [
        ('port_a_data', uint32_t),
    ]
class gpio_swporta_dr_data(Union):
    _fields_ = [
        ('d32', uint32_t),
        ('b', random_type_436),
    ]
gpio_swporta_dr_data_t=gpio_swporta_dr_data
class random_type_437(Structure):
    _fields_ = [
        ('external_port_a', uint32_t),
    ]
class gpio_ext_porta_data(Union):
    _fields_ = [
        ('d32', uint32_t),
        ('b', random_type_437),
    ]
gpio_ext_porta_data_t=gpio_ext_porta_data
class random_type_438(Structure):
    _fields_ = [
        ('interrupt_polarity', uint32_t),
    ]
class gpio_int_polarity_data(Union):
    _fields_ = [
        ('d32', uint32_t),
        ('b', random_type_438),
    ]
gpio_int_polarity_data_t=gpio_int_polarity_data
class random_type_439(Structure):
    _fields_ = [
        ('gpio_int_bothedge', uint32_t),
    ]
class gpio_int_bothedge_data(Union):
    _fields_ = [
        ('d32', uint32_t),
        ('b', random_type_439),
    ]
gpio_int_bothedge_data_t=gpio_int_bothedge_data
class random_type_440(Structure):
    _fields_ = [
        ('interrupt_enable', uint32_t),
    ]
class gpio_inten_data(Union):
    _fields_ = [
        ('d32', uint32_t),
        ('b', random_type_440),
    ]
gpio_inten_data_t=gpio_inten_data
class random_type_441(Structure):
    _fields_ = [
        ('set_mode', hal_pin_set_mode),
        ('get_mode', hal_pin_get_mode),
        ('set_ds', hal_pin_set_ds),
        ('get_ds', hal_pin_get_ds),
        ('set_pull', hal_pin_set_pull),
        ('get_pull', hal_pin_get_pull),
        ('set_ie', hal_pin_set_ie),
        ('get_ie', hal_pin_get_ie),
    ]
class random_type_442(Structure):
    _fields_ = [
        ('set_mode', hal_pin_set_mode),
        ('get_mode', hal_pin_get_mode),
        ('set_ds', hal_pin_set_ds),
        ('get_ds', hal_pin_get_ds),
        ('set_pull', hal_pin_set_pull),
        ('get_pull', hal_pin_get_pull),
        ('set_ie', hal_pin_set_ie),
        ('get_ie', hal_pin_get_ie),
    ]
class hal_pmpcfg(Union):
    _fields_ = [
        ('d32', uint32_t),
        ('b', (hal_pmpx_config_t*4)),
    ]
hal_pmp_riscv70_cfg_t=hal_pmpcfg
class random_type_446(Structure):
    _fields_ = [
        ('init', hal_sha_init_t),
        ('deinit', hal_sha_deinit_t),
        ('start', hal_sha_start_t),
        ('finish', hal_sha_finish_t),
    ]
class random_type_447(Structure):
    _fields_ = [
        ('init', hal_sha_init_t),
        ('deinit', hal_sha_deinit_t),
        ('start', hal_sha_start_t),
        ('finish', hal_sha_finish_t),
    ]
class random_type_448(Structure):
    _fields_ = [
        ('reserved_0', uint32_t,2),
        ('sha_int_clr', uint32_t,1),
        ('reserved_1', uint32_t,29),
    ]
class sha_int_clr(Union):
    _fields_ = [
        ('b', random_type_448),
        ('d32', uint32_t),
    ]
sha_int_clr_t=sha_int_clr
class random_type_449(Structure):
    _fields_ = [
        ('reserved_0', uint32_t,2),
        ('sha_int_en', uint32_t,1),
        ('reserved_1', uint32_t,29),
    ]
class sha_int_en(Union):
    _fields_ = [
        ('b', random_type_449),
        ('d32', uint32_t),
    ]
sha_int_en_t=sha_int_en
class random_type_450(Structure):
    _fields_ = [
        ('sha_en', uint32_t,1),
        ('sha_start', uint32_t,1),
        ('sha_end', uint32_t,1),
        ('reserved_0', uint32_t,29),
    ]
class sha_ctrl(Union):
    _fields_ = [
        ('b', random_type_450),
        ('d32', uint32_t),
    ]
sha_ctrl_t=sha_ctrl
class random_type_451(Structure):
    _fields_ = [
        ('sha_ivin_sel', uint32_t,1),
        ('sha_padding_dis', uint32_t,1),
        ('reserved_0', uint32_t,2),
        ('sha_compute_mode', uint32_t,1),
        ('reserved_1', uint32_t,27),
    ]
class sha_cfg(Union):
    _fields_ = [
        ('b', random_type_451),
        ('d32', uint32_t),
    ]
sha_cfg_t=sha_cfg
class random_type_454(Structure):
    _fields_ = [
        ('reserved_0', uint32_t,2),
        ('sha_int_sts', uint32_t,1),
        ('reserved_1', uint32_t,29),
    ]
class sha_int_sts(Union):
    _fields_ = [
        ('b', random_type_454),
        ('d32', uint32_t),
    ]
sha_int_sts_t=sha_int_sts
class random_type_457(Structure):
    _fields_ = [
        ('sha_fsm_st', uint32_t,4),
        ('reserved_0', uint32_t,28),
    ]
class sha_fsm_st(Union):
    _fields_ = [
        ('b', random_type_457),
        ('d32', uint32_t),
    ]
sha_fsm_st_t=sha_fsm_st
class sha_regs(Structure):
    _fields_ = [
        ('sha_int_en', sha_int_en_t),
        ('sha_int_clr', sha_int_clr_t),
        ('sha_int_sts', sha_int_sts_t),
        ('reserved', (uint32_t*17)),
        ('sha_ctrl', sha_ctrl_t),
        ('sha_cfg', sha_cfg_t),
        ('sha_data_len', uint32_t),
        ('sha_ivin_start_addr', uint32_t),
        ('sha_data_in_start_addr', uint32_t),
        ('sha_data_out_start_addr', uint32_t),
        ('sha_data_len_all', uint32_t),
        ('sha_fsm_st', sha_fsm_st_t),
    ]
sha_regs_t=sha_regs
class random_type_458(Structure):
    _fields_ = [
        ('reserved_0', uint32_t,4),
        ('nc_cipher_done_rint', uint32_t,1),
        ('reserved_1', uint32_t,3),
        ('nc_fail_rint', uint32_t,1),
        ('reserved_2', uint32_t,5),
        ('ahbm_timeout_rint', uint32_t,1),
        ('reserved_3', uint32_t,17),
    ]
class rsav2_rint(Union):
    _fields_ = [
        ('b', random_type_458),
        ('d32', uint32_t),
    ]
rsav2_rint_t=rsav2_rint
class random_type_459(Structure):
    _fields_ = [
        ('reserved_0', uint32_t,4),
        ('nc_cipher_done_msk', uint32_t,1),
        ('reserved_1', uint32_t,3),
        ('nc_fail_msk', uint32_t,1),
        ('reserved_2', uint32_t,5),
        ('ahbm_timeout_msk', uint32_t,1),
        ('reserved_3', uint32_t,17),
    ]
class rsav2_intmsk(Union):
    _fields_ = [
        ('b', random_type_459),
        ('d32', uint32_t),
    ]
rsav2_intmsk_t=rsav2_intmsk
class random_type_460(Structure):
    _fields_ = [
        ('ahbm_timeout_waterline', uint32_t,16),
        ('reserved_0', uint32_t,16),
    ]
class rsav2_cfg_ahbm_timeout_waterline(Union):
    _fields_ = [
        ('b', random_type_460),
        ('d32', uint32_t),
    ]
rsav2_cfg_ahbm_timeout_waterline_t=rsav2_cfg_ahbm_timeout_waterline
class random_type_461(Structure):
    _fields_ = [
        ('otask_len', uint32_t,8),
        ('plaintext_len_sel', uint32_t,1),
        ('reserved_0', uint32_t,23),
    ]
class rsav2_otskcfg_attribute(Union):
    _fields_ = [
        ('b', random_type_461),
        ('d32', uint32_t),
    ]
rsav2_otskcfg_attribute_t=rsav2_otskcfg_attribute
class random_type_462(Structure):
    _fields_ = [
        ('otask_start', uint32_t,1),
        ('reserved_0', uint32_t,31),
    ]
class rsav2_otskcfg_start(Union):
    _fields_ = [
        ('b', random_type_462),
        ('d32', uint32_t),
    ]
rsav2_otskcfg_start_t=rsav2_otskcfg_start
class random_type_465(Structure):
    _fields_ = [
        ('otask_force_end', uint32_t,1),
        ('reserved_0', uint32_t,31),
    ]
class rsav2_otskcfg_stop(Union):
    _fields_ = [
        ('b', random_type_465),
        ('d32', uint32_t),
    ]
rsav2_otskcfg_stop_t=rsav2_otskcfg_stop
class random_type_469(Structure):
    _fields_ = [
        ('reserved_0', uint32_t,4),
        ('nc_cipher_done_intsts', uint32_t,1),
        ('reserved_1', uint32_t,3),
        ('nc_fail_intsts', uint32_t,1),
        ('reserved_2', uint32_t,5),
        ('ahbm_timeout_intsts', uint32_t,1),
        ('reserved_3', uint32_t,17),
    ]
class rsav2_intsts(Union):
    _fields_ = [
        ('b', random_type_469),
        ('d32', uint32_t),
    ]
rsav2_intsts_t=rsav2_intsts
class random_type_470(Structure):
    _fields_ = [
        ('reserved_0', uint32_t,4),
        ('nc_cipher_done_intset', uint32_t,1),
        ('reserved_1', uint32_t,3),
        ('nc_fail_intset', uint32_t,1),
        ('reserved_2', uint32_t,5),
        ('ahbm_timeout_intset', uint32_t,1),
        ('reserved_3', uint32_t,17),
    ]
class rsav2_intset(Union):
    _fields_ = [
        ('b', random_type_470),
        ('d32', uint32_t),
    ]
rsav2_intset_t=rsav2_intset
class random_type_471(Structure):
    _fields_ = [
        ('dfxcnt_err_otask_attribute', uint32_t,10),
        ('reserved_0', uint32_t,22),
    ]
class rsav2_dfxcnt_err_otask_attribute(Union):
    _fields_ = [
        ('b', random_type_471),
        ('d32', uint32_t),
    ]
rsav2_dfxcnt_err_otask_attribute_t=rsav2_dfxcnt_err_otask_attribute
class random_type_472(Structure):
    _fields_ = [
        ('dfxcnt_nc_get', uint32_t,20),
        ('reserved_0', uint32_t,12),
    ]
class rsav2_dfxcnt_nc_get(Union):
    _fields_ = [
        ('b', random_type_472),
        ('d32', uint32_t),
    ]
rsav2_dfxcnt_nc_get_t=rsav2_dfxcnt_nc_get
class random_type_473(Structure):
    _fields_ = [
        ('dfxcnt_nc_sucess', uint32_t,20),
        ('reserved_0', uint32_t,12),
    ]
class rsav2_dfxcnt_nc_success(Union):
    _fields_ = [
        ('b', random_type_473),
        ('d32', uint32_t),
    ]
rsav2_dfxcnt_nc_success_t=rsav2_dfxcnt_nc_success
class random_type_474(Structure):
    _fields_ = [
        ('dfxcnt_nc_fail', uint32_t,10),
        ('reserved_0', uint32_t,22),
    ]
class rsav2_dfxcnt_nc_fail(Union):
    _fields_ = [
        ('b', random_type_474),
        ('d32', uint32_t),
    ]
rsav2_dfxcnt_nc_fail_t=rsav2_dfxcnt_nc_fail
class random_type_475(Structure):
    _fields_ = [
        ('dfx_nc_fail_reason', uint32_t,8),
        ('reserved_0', uint32_t,24),
    ]
class rsav2_dfx_nc_fail_reason(Union):
    _fields_ = [
        ('b', random_type_475),
        ('d32', uint32_t),
    ]
rsav2_dfx_nc_fail_reason_t=rsav2_dfx_nc_fail_reason
class random_type_476(Structure):
    _fields_ = [
        ('rsa_otask_busy', uint32_t,1),
        ('rsa_itask_busy', uint32_t,1),
        ('rsa_busy', uint32_t,1),
        ('rsa_wr_busy', uint32_t,1),
        ('reserved_0', uint32_t,28),
    ]
class rsav2_curr_busy(Union):
    _fields_ = [
        ('b', random_type_476),
        ('d32', uint32_t),
    ]
rsav2_curr_busy_t=rsav2_curr_busy
class random_type_477(Structure):
    _fields_ = [
        ('normal_codec_main_fsm_st', uint32_t,6),
        ('reserved_0', uint32_t,26),
    ]
class rsav2_normal_codec_fsm_st(Union):
    _fields_ = [
        ('b', random_type_477),
        ('d32', uint32_t),
    ]
rsav2_normal_codec_fsm_st_t=rsav2_normal_codec_fsm_st
class random_type_478(Structure):
    _fields_ = [
        ('mod_fsm_st', uint32_t,7),
        ('reserved_0', uint32_t,25),
    ]
class rsav2_mod_fsm_st(Union):
    _fields_ = [
        ('b', random_type_478),
        ('d32', uint32_t),
    ]
rsav2_mod_fsm_st_t=rsav2_mod_fsm_st
class random_type_479(Structure):
    _fields_ = [
        ('left_modex_ctrl_fsm_st', uint32_t,8),
        ('left_modex_ctrl_exp_cnt_st', uint32_t,15),
        ('reserved_0', uint32_t,9),
    ]
class rsav2_modex_ctrl_st_left(Union):
    _fields_ = [
        ('b', random_type_479),
        ('d32', uint32_t),
    ]
rsav2_modex_ctrl_st_left_t=rsav2_modex_ctrl_st_left
class random_type_480(Structure):
    _fields_ = [
        ('right_modex_ctrl_fsm_st', uint32_t,8),
        ('right_modex_ctrl_exp_cnt_st', uint32_t,15),
        ('reserved_0', uint32_t,9),
    ]
class rsav2_modex_ctrl_st_right(Union):
    _fields_ = [
        ('b', random_type_480),
        ('d32', uint32_t),
    ]
rsav2_modex_ctrl_st_right_t=rsav2_modex_ctrl_st_right
class random_type_481(Structure):
    _fields_ = [
        ('dat_rd_st', uint32_t,7),
        ('reserved_0', uint32_t,25),
    ]
class rsav2_dat_rd_st(Union):
    _fields_ = [
        ('b', random_type_481),
        ('d32', uint32_t),
    ]
rsav2_dat_rd_st_t=rsav2_dat_rd_st
class random_type_482(Structure):
    _fields_ = [
        ('dat_wr_st', uint32_t,10),
        ('reserved_0', uint32_t,22),
    ]
class rsav2_dat_wr_st(Union):
    _fields_ = [
        ('b', random_type_482),
        ('d32', uint32_t),
    ]
rsav2_dat_wr_st_t=rsav2_dat_wr_st
class random_type_483(Structure):
    _fields_ = [
        ('ram_ini_st', uint32_t,1),
        ('reserved_0', uint32_t,31),
    ]
class rsav2_ram_ini_st(Union):
    _fields_ = [
        ('b', random_type_483),
        ('d32', uint32_t),
    ]
rsav2_ram_ini_st_t=rsav2_ram_ini_st
class random_type_484(Structure):
    _fields_ = [
        ('ahbm_hready', uint32_t,1),
        ('ahbm_wr_incr_256_cnt', uint32_t,1),
        ('ahbm_wr_incr_512_cnt', uint32_t,5),
        ('ahbm_wr_ack_cnt', uint32_t,4),
        ('ahbm_rd_incr_256_cnt', uint32_t,1),
        ('ahbm_rd_incr_512_cnt', uint32_t,5),
        ('ahbm_rd_ack_cnt', uint32_t,4),
        ('reserved_0', uint32_t,11),
    ]
class rsav2_ahbm_rw_cmd_view(Union):
    _fields_ = [
        ('b', random_type_484),
        ('d32', uint32_t),
    ]
rsav2_ahbm_rw_cmd_view_t=rsav2_ahbm_rw_cmd_view
class rsa_regs(Structure):
    _fields_ = [
        ('reserved_0', (uint32_t*79)),
        ('rsav2_cfg_ahbm_timeout_waterline', rsav2_cfg_ahbm_timeout_waterline_t),
        ('reserved_1', (uint32_t*1968)),
        ('rsav2_otskcfg_start', rsav2_otskcfg_start_t),
        ('rsav2_otskcfg_stop', rsav2_otskcfg_stop_t),
        ('rsav2_otskcfg_attribute', rsav2_otskcfg_attribute_t),
        ('rsav2_otskcfg_plaintext_addr', uint32_t),
        ('reserved_2', uint32_t),
        ('rsav2_otskcfg_key_addr', uint32_t),
        ('reserved_3', (uint32_t*5)),
        ('rsav2_otskcfg_result_addr', uint32_t),
        ('reserved_4', (uint32_t*500)),
        ('rsav2_intmsk', rsav2_intmsk_t),
        ('rsav2_rint', rsav2_rint_t),
        ('rsav2_intsts', rsav2_intsts_t),
        ('rsav2_intset', rsav2_intset_t),
        ('reserved_5', (uint32_t*510)),
        ('rsav2_dfxcnt_err_otask_attribute', rsav2_dfxcnt_err_otask_attribute_t),
        ('reserved_6', (uint32_t*9)),
        ('rsav2_dfxcnt_nc_get', rsav2_dfxcnt_nc_get_t),
        ('rsav2_dfxcnt_nc_success', rsav2_dfxcnt_nc_success_t),
        ('rsav2_dfxcnt_nc_fail', rsav2_dfxcnt_nc_fail_t),
        ('rsav2_dfx_nc_fail_reason', rsav2_dfx_nc_fail_reason_t),
        ('reserved_7', (uint32_t*497)),
        ('rsav2_curr_busy', rsav2_curr_busy_t),
        ('reserved_8', (uint32_t*32)),
        ('rsav2_normal_codec_fsm_st', rsav2_normal_codec_fsm_st_t),
        ('rsav2_mod_fsm_st', rsav2_mod_fsm_st_t),
        ('reserved_9', uint32_t),
        ('rsav2_modex_ctrl_st_left', rsav2_modex_ctrl_st_left_t),
        ('rsav2_modex_ctrl_st_right', rsav2_modex_ctrl_st_right_t),
        ('reserved_10', (uint32_t*6)),
        ('rsav2_dat_rd_st', rsav2_dat_rd_st_t),
        ('rsav2_dat_wr_st', rsav2_dat_wr_st_t),
        ('reserved_11', (uint32_t*19)),
        ('rsav2_ram_ini_st', rsav2_ram_ini_st_t),
        ('reserved_12', (uint32_t*192)),
        ('rsav2_ahbm_rw_cmd_view', rsav2_ahbm_rw_cmd_view_t),
    ]
rsa_regs_t=rsa_regs
class random_type_485(Structure):
    _fields_ = [
        ('init', hal_efuse_init_t),
        ('deinit', hal_efuse_deinit_t),
        ('flush_write', hal_efuse_flush_write_t),
        ('refresh_read', hal_efuse_refresh_read_t),
        ('read_byte', hal_efuse_read_byte_t),
        ('write_byte', hal_efuse_write_byte_t),
        ('clear', hal_efuse_clear_all_write_regs_t),
        ('write_op', hal_efuse_write_operation_t),
        ('write_buffer_op', hal_efuse_write_buffer_operation_t),
        ('get_die_id', hal_efuse_get_die_id_t),
        ('get_chip_id', hal_efuse_get_chip_id_t),
    ]
class random_type_486(Structure):
    _fields_ = [
        ('init', hal_efuse_init_t),
        ('deinit', hal_efuse_deinit_t),
        ('flush_write', hal_efuse_flush_write_t),
        ('refresh_read', hal_efuse_refresh_read_t),
        ('read_byte', hal_efuse_read_byte_t),
        ('write_byte', hal_efuse_write_byte_t),
        ('clear', hal_efuse_clear_all_write_regs_t),
        ('write_op', hal_efuse_write_operation_t),
        ('write_buffer_op', hal_efuse_write_buffer_operation_t),
        ('get_die_id', hal_efuse_get_die_id_t),
        ('get_chip_id', hal_efuse_get_chip_id_t),
    ]
class random_type_487(Structure):
    _fields_ = [
        ('efuse_wr_rd', uint32_t,16),
    ]
class efuse_ctl_data(Union):
    _fields_ = [
        ('d32', uint32_t),
        ('b', random_type_487),
    ]
efuse_ctl_data_t=efuse_ctl_data
class random_type_489(Structure):
    _fields_ = [
        ('clk', uint32_t,8),
        ('reserved8_15', uint32_t,8),
    ]
class efuse_clk_period_data(Union):
    _fields_ = [
        ('d32', uint32_t),
        ('b', random_type_489),
    ]
efuse_period_data_t=efuse_clk_period_data
class random_type_490(Structure):
    _fields_ = [
        ('mode', uint32_t,1),
        ('reserved2_15', uint32_t,15),
    ]
class efuse_mr_data(Union):
    _fields_ = [
        ('d32', uint32_t),
        ('b', random_type_490),
    ]
efuse_mr_data_t=efuse_mr_data
class efuse_regs(Structure):
    _fields_ = [
        ('efuse_ctl_data', efuse_ctl_data_t),
        ('efuse_clk_period_data', efuse_period_data_t),
        ('efuse_mr_data', efuse_mr_data_t),
    ]
efuse_regs_t=efuse_regs
class random_type_491(Structure):
    _fields_ = [
        ('init', hal_spi_init_t),
        ('deinit', hal_spi_deinit_t),
        ('write', hal_spi_write_t),
        ('read', hal_spi_read_t),
        ('writeread', hal_spi_writeread_t),
        ('ctrl', hal_spi_ctrl_t),
    ]
class random_type_492(Structure):
    _fields_ = [
        ('init', hal_spi_init_t),
        ('deinit', hal_spi_deinit_t),
        ('write', hal_spi_write_t),
        ('read', hal_spi_read_t),
        ('writeread', hal_spi_writeread_t),
        ('ctrl', hal_spi_ctrl_t),
    ]
class random_type_494(Structure):
    _fields_ = [
        ('dfs', uint32_t,4),
        ('frf', uint32_t,2),
        ('scph', uint32_t,1),
        ('scpol', uint32_t,1),
        ('tmod', uint32_t,2),
        ('slv_oe', uint32_t,1),
        ('srl', uint32_t,1),
        ('cfs', uint32_t,4),
        ('dfs_32', uint32_t,5),
        ('spi_frf', uint32_t,2),
        ('reserved_23', uint32_t,1),
        ('sste', uint32_t,1),
        ('reserved25_31', uint32_t,7),
    ]
class ctrlr0_data(Union):
    _fields_ = [
        ('d32', uint32_t),
        ('b', random_type_494),
    ]
ctrlr0_data_t=ctrlr0_data
class random_type_495(Structure):
    _fields_ = [
        ('trans_type', uint32_t,2),
        ('addr_l', uint32_t,4),
        ('reserved6_7', uint32_t,2),
        ('inst_l', uint32_t,2),
        ('reserved8', uint32_t,1),
        ('wait_cycles', uint32_t,5),
        ('spi_ddr_en', uint32_t,1),
        ('inst_ddr_en', uint32_t,1),
        ('spi_rxds_en', uint32_t,1),
        ('reserved19_31', uint32_t,13),
    ]
class spi_ctrlr0_data(Union):
    _fields_ = [
        ('d32', uint32_t),
        ('b', random_type_495),
    ]
spi_ctrlr0_data_t=spi_ctrlr0_data
class random_type_496(Structure):
    _fields_ = [
        ('mwmod', uint32_t,1),
        ('mdd', uint32_t,1),
        ('mhs', uint32_t,1),
    ]
class mwcr_data(Union):
    _fields_ = [
        ('d32', uint32_t),
        ('b', random_type_496),
    ]
mwcr_data_t=mwcr_data
class random_type_501(Structure):
    _fields_ = [
        ('txei', uint32_t,1),
        ('txoi', uint32_t,1),
        ('rxui', uint32_t,1),
        ('rxoi', uint32_t,1),
        ('rxfi', uint32_t,1),
        ('msti', uint32_t,1),
        ('reserved6_31', uint32_t,26),
    ]
class interrupt_data(Union):
    _fields_ = [
        ('d32', uint32_t),
        ('b', random_type_501),
    ]
class random_type_504(Structure):
    _fields_ = [
        ('ndf', uint32_t,16),
    ]
class ctrlr1_data(Union):
    _fields_ = [
        ('d32', uint32_t),
        ('b', random_type_504),
    ]
ctrlr1_data_t=ctrlr1_data
dmatdlr_data_t=dmadlr_data
class random_type_507(Structure):
    _fields_ = [
        ('init', hal_systick_init_t),
        ('deinit', hal_systick_deinit_t),
        ('ctrl', hal_systick_ctrl_t),
    ]
class random_type_508(Structure):
    _fields_ = [
        ('init', hal_systick_init_t),
        ('deinit', hal_systick_deinit_t),
        ('ctrl', hal_systick_ctrl_t),
    ]
class random_type_509(Structure):
    _fields_ = [
        ('frz', uint32_t,1),
        ('stz', uint32_t,1),
        ('reserved2_3', uint32_t,2),
        ('clr', uint32_t,1),
        ('reserved5_15', uint32_t,11),
    ]
class cfg_data(Union):
    _fields_ = [
        ('d32', uint32_t),
        ('b', random_type_509),
    ]
cfg_data_t=cfg_data
class systick_regs(Structure):
    _fields_ = [
        ('cfg', uint32_t),
        ('value0', uint32_t),
        ('value1', uint32_t),
        ('value2', uint32_t),
        ('value3', uint32_t),
    ]
systick_regs_t=systick_regs
class random_type_511(Structure):
    _fields_ = [
        ('data', uint32_t,16),
    ]
class value_data(Union):
    _fields_ = [
        ('d32', uint32_t),
        ('b', random_type_511),
    ]
value_data_t=value_data
class random_type_512(Structure):
    _fields_ = [
        ('init', hal_tcxo_init_t),
        ('deinit', hal_tcxo_deinit_t),
        ('get', hal_tcxo_get_t),
    ]
class random_type_513(Structure):
    _fields_ = [
        ('init', hal_tcxo_init_t),
        ('deinit', hal_tcxo_deinit_t),
        ('get', hal_tcxo_get_t),
    ]
class random_type_514(Structure):
    _fields_ = [
        ('refresh', uint32_t,1),
        ('clear', uint32_t,1),
        ('enable', uint32_t,1),
        ('reserved', uint32_t,1),
        ('valid', uint32_t,1),
    ]
class tcxo_status_data(Union):
    _fields_ = [
        ('d32', uint32_t),
        ('b', random_type_514),
    ]
tcxo_status_data_t=tcxo_status_data
class tcxo_regs(Structure):
    _fields_ = [
        ('status', uint32_t),
        ('count0', uint32_t),
        ('count1', uint32_t),
        ('count2', uint32_t),
        ('count3', uint32_t),
    ]
tcxo_regs_t=tcxo_regs
class random_type_515(Structure):
    _fields_ = [
        ('data', uint32_t,16),
    ]
class tcxo_count_data(Union):
    _fields_ = [
        ('d32', uint32_t),
        ('b', random_type_515),
    ]
tcxo_count_data_t=tcxo_count_data
hal_timer_callback_t=td_u32
class timer_regs_info(Structure):
    _fields_ = [
        ('load_count', uint32_t),
        ('current_value', uint32_t),
        ('control_reg', uint32_t),
        ('eoi', uint32_t),
        ('int_status', uint32_t),
    ]
timer_regs_info_t=timer_regs_info
class random_type_516(Structure):
    _fields_ = [
        ('enable', uint32_t,1),
        ('mode', uint32_t,1),
        ('int_mask', uint32_t,1),
        ('reserved3_31', uint32_t,29),
    ]
class timer_control_reg_data(Union):
    _fields_ = [
        ('d32', uint32_t),
        ('b', random_type_516),
    ]
timer_control_reg_data_t=timer_control_reg_data
class random_type_517(Structure):
    _fields_ = [
        ('init', hal_uart_init_t),
        ('deinit', hal_uart_deinit_t),
        ('ctrl', hal_uart_ctrl_t),
        ('write', hal_uart_write_t),
        ('read', hal_uart_read_t),
        ('dma_cfg', hal_uart_dma_cfg_t),
    ]
class random_type_519(Structure):
    _fields_ = [
        ('init', hal_uart_init_t),
        ('deinit', hal_uart_deinit_t),
        ('ctrl', hal_uart_ctrl_t),
        ('write', hal_uart_write_t),
        ('read', hal_uart_read_t),
        ('dma_cfg', hal_uart_dma_cfg_t),
    ]
class uart_regs(Structure):
    _fields_ = [
        ('uartdr', uint32_t),
        ('uartrsr', uint32_t),
        ('res0', uint32_t),
        ('res1', uint32_t),
        ('res2', uint32_t),
        ('res3', uint32_t),
        ('uartfr', uint32_t),
        ('res4', uint32_t),
        ('uartilpr', uint32_t),
        ('uartibrd', uint32_t),
        ('uartfbrd', uint32_t),
        ('uartlcr_h', uint32_t),
        ('uartcr', uint32_t),
        ('uartifls', uint32_t),
        ('uartimsc', uint32_t),
        ('uartris', uint32_t),
        ('uartmis', uint32_t),
        ('uarticr', uint32_t),
        ('uartdmacr', uint32_t),
    ]
uart_regs_t=uart_regs
class random_type_521(Structure):
    _fields_ = [
        ('frame_error', uint32_t,1),
        ('parity_error', uint32_t,1),
        ('break_error', uint32_t,1),
        ('overrun_error', uint32_t,1),
    ]
class random_type_522(Structure):
    _fields_ = [
        ('wdata', uint32_t,8),
    ]
class uartrsr_data(Union):
    _fields_ = [
        ('d32', uint32_t),
        ('rb', random_type_521),
        ('wb', random_type_522),
    ]
uartrsr_data_t=uartrsr_data
class random_type_523(Structure):
    _fields_ = [
        ('uarten', uint32_t,1),
        ('siren', uint32_t,1),
        ('sirlp', uint32_t,1),
        ('reserved3_6', uint32_t,4),
        ('lbe', uint32_t,1),
        ('txe', uint32_t,1),
        ('rxe', uint32_t,1),
        ('dtr', uint32_t,1),
        ('rst', uint32_t,1),
        ('out1', uint32_t,1),
        ('out2', uint32_t,1),
        ('rts_en', uint32_t,1),
        ('cts_en', uint32_t,1),
    ]
class uartcr_data(Union):
    _fields_ = [
        ('d32', uint32_t),
        ('b', random_type_523),
    ]
uartcr_data_t=uartcr_data
class random_type_524(Structure):
    _fields_ = [
        ('brk', uint32_t,1),
        ('parity_en', uint32_t,1),
        ('eps', uint32_t,1),
        ('stp2', uint32_t,1),
        ('en_fifos', uint32_t,1),
        ('wlen', uint32_t,2),
        ('sps', uint32_t,1),
        ('reserved8_15', uint32_t,8),
    ]
class uartlcr_h_data(Union):
    _fields_ = [
        ('d32', uint32_t),
        ('b', random_type_524),
    ]
uartlcr_h_data_t=uartlcr_h_data
class random_type_525(Structure):
    _fields_ = [
        ('txiflsel', uint32_t,3),
        ('rxiflsel', uint32_t,3),
        ('reserved6_15', uint32_t,10),
    ]
class uartifls_data(Union):
    _fields_ = [
        ('d32', uint32_t),
        ('b', random_type_525),
    ]
uartifls_data_t=uartifls_data
class random_type_526(Structure):
    _fields_ = [
        ('cts', uint32_t,1),
        ('dsr', uint32_t,1),
        ('dcd', uint32_t,1),
        ('busy', uint32_t,1),
        ('rxfe', uint32_t,1),
        ('txff', uint32_t,1),
        ('rxff', uint32_t,1),
        ('txfe', uint32_t,1),
        ('ri', uint32_t,1),
        ('reserved9_15', uint32_t,7),
    ]
class uartfr_data(Union):
    _fields_ = [
        ('d32', uint32_t),
        ('b', random_type_526),
    ]
uartfr_data_t=uartfr_data
class random_type_527(Structure):
    _fields_ = [
        ('data', uint32_t,8),
        ('frame_error', uint32_t,1),
        ('parity_error', uint32_t,1),
        ('break_error', uint32_t,1),
        ('overrun_error', uint32_t,1),
        ('reserved12_15', uint32_t,4),
    ]
class uartdr_data(Union):
    _fields_ = [
        ('d32', uint32_t),
        ('b', random_type_527),
    ]
uartdr_data_t=uartdr_data
class random_type_528(Structure):
    _fields_ = [
        ('rxdmaen', uint32_t,1),
        ('txdmaen', uint32_t,1),
        ('dmaonerr', uint32_t,1),
        ('reserved3_15', uint32_t,13),
    ]
class uartdmacr_data(Union):
    _fields_ = [
        ('d32', uint32_t),
        ('b', random_type_528),
    ]
uartdmacr_data_t=uartdmacr_data
class random_type_529(Structure):
    _fields_ = [
        ('baud_divint', uint32_t,16),
    ]
class uartibrd_data(Union):
    _fields_ = [
        ('d32', uint32_t),
        ('b', random_type_529),
    ]
uartibrd_data_t=uartibrd_data
class random_type_530(Structure):
    _fields_ = [
        ('baud_divfrac', uint32_t,6),
    ]
class uartfbrd_data(Union):
    _fields_ = [
        ('d32', uint32_t),
        ('b', random_type_530),
    ]
uartfbrd_data_t=uartfbrd_data
class random_type_531(Structure):
    _fields_ = [
        ('init', hal_watchdog_init_t),
        ('deinit', hal_watchdog_deinit_t),
        ('set_attr', hal_watchdog_set_attr_t),
        ('get_attr', hal_watchdog_get_attr_t),
        ('enable', hal_watchdog_enable_t),
        ('disable', hal_watchdog_disable_t),
        ('kick', hal_watchdog_kick_t),
        ('get_left_time', hal_watchdog_get_time_t),
        ('register_callback', hal_register_watchdog_callback_t),
    ]
class random_type_532(Structure):
    _fields_ = [
        ('init', hal_watchdog_init_t),
        ('deinit', hal_watchdog_deinit_t),
        ('set_attr', hal_watchdog_set_attr_t),
        ('get_attr', hal_watchdog_get_attr_t),
        ('enable', hal_watchdog_enable_t),
        ('disable', hal_watchdog_disable_t),
        ('kick', hal_watchdog_kick_t),
        ('get_left_time', hal_watchdog_get_time_t),
        ('register_callback', hal_register_watchdog_callback_t),
    ]
class watchdog_regs(Structure):
    _fields_ = [
        ('wdt_cr', uint32_t),
        ('wdt_torr', uint32_t),
        ('wdt_ccvr', uint32_t),
        ('wdt_crr', uint32_t),
        ('wdt_stat', uint32_t),
        ('wdt_eoi', uint32_t),
    ]
watchdog_regs_t=watchdog_regs
class random_type_533(Structure):
    _fields_ = [
        ('wdt_eoi', uint32_t,1),
        ('rsvd_wdt_eoi1_31', uint32_t,31),
    ]
class wdt_eoi_data(Union):
    _fields_ = [
        ('d32', uint32_t),
        ('b', random_type_533),
    ]
wdt_eoi_data_t=wdt_eoi_data
class random_type_534(Structure):
    _fields_ = [
        ('wdt_en', uint32_t,1),
        ('rmod', uint32_t,1),
        ('rpl', uint32_t,3),
        ('no_name', uint32_t,1),
        ('rsvd_wdt_cr6_31', uint32_t,26),
    ]
class wdt_cr_data(Union):
    _fields_ = [
        ('d32', uint32_t),
        ('b', random_type_534),
    ]
wdt_cr_data_t=wdt_cr_data
class random_type_535(Structure):
    _fields_ = [
        ('top', uint32_t,4),
        ('top_init', uint32_t,4),
        ('reserved8_31', uint32_t,24),
    ]
class wdt_torr_data(Union):
    _fields_ = [
        ('d32', uint32_t),
        ('b', random_type_535),
    ]
wdt_torr_data_t=wdt_torr_data
class random_type_536(Structure):
    _fields_ = [
        ('wdt_crr', uint32_t,8),
        ('rsvd_wdt_crr8_31', uint32_t,24),
    ]
class wdt_crr_data(Union):
    _fields_ = [
        ('d32', uint32_t),
        ('b', random_type_536),
    ]
wdt_crr_data_t=wdt_crr_data
class random_type_537(Structure):
    _fields_ = [
        ('wdt_ccvr', uint32_t,31),
        ('rsvd_gpio_wdt_ccvry_31', uint32_t,1),
    ]
class wdt_ccvr_data(Union):
    _fields_ = [
        ('d32', uint32_t),
        ('b', random_type_537),
    ]
wdt_ccvr_data_t=wdt_ccvr_data
class random_type_539(Structure):
    _fields_ = [
        ('man_all_req', uint32_t,1),
        ('man_all_done', uint32_t,1),
        ('reserved2_15', uint32_t,14),
    ]
class man_all_data(Union):
    _fields_ = [
        ('d32', uint32_t),
        ('b', random_type_539),
    ]
man_all_data_t=man_all_data
class random_type_540(Structure):
    _fields_ = [
        ('icu_en', uint32_t,1),
        ('reserved1_15', uint32_t,15),
    ]
class xip_icu_en_data(Union):
    _fields_ = [
        ('d32', uint32_t),
        ('b', random_type_540),
    ]
xip_icu_en_data_t=xip_icu_en_data
class random_type_541(Structure):
    _fields_ = [
        ('monitor_sel', uint32_t,1),
        ('reserved1_15', uint32_t,15),
    ]
class xip_monitor_sel_data(Union):
    _fields_ = [
        ('d32', uint32_t),
        ('b', random_type_541),
    ]
xip_monitor_sel_data_t=xip_monitor_sel_data
class random_type_542(Structure):
    _fields_ = [
        ('cache_en', uint32_t,1),
        ('reserved1_15', uint32_t,15),
    ]
class xip_cache_en_data(Union):
    _fields_ = [
        ('d32', uint32_t),
        ('b', random_type_542),
    ]
xip_cache_en_data_t=xip_cache_en_data
class random_type_543(Structure):
    _fields_ = [
        ('read_qspi_enable', uint32_t,1),
        ('reserved1_15', uint32_t,15),
    ]
class xip_read_qspi_enable_data(Union):
    _fields_ = [
        ('d32', uint32_t),
        ('b', random_type_543),
    ]
xip_read_qspi_enable_data_t=xip_read_qspi_enable_data
class random_type_544(Structure):
    _fields_ = [
        ('wrap_operation', uint32_t,1),
        ('reserved1_15', uint32_t,15),
    ]
class cfg_wrap_operation_data(Union):
    _fields_ = [
        ('d32', uint32_t),
        ('b', random_type_544),
    ]
cfg_wrap_operation_data_t=cfg_wrap_operation_data
class random_type_545(Structure):
    _fields_ = [
        ('addr_24_32', uint32_t,1),
        ('reserved1_15', uint32_t,15),
    ]
class cfg_addr_24_32_data(Union):
    _fields_ = [
        ('d32', uint32_t),
        ('b', random_type_545),
    ]
cfg_addr_24_32_data_t=cfg_addr_24_32_data
class random_type_546(Structure):
    _fields_ = [
        ('read_mode_code', uint32_t,8),
        ('reserved8_15', uint32_t,8),
    ]
class xip_mode_code_data(Union):
    _fields_ = [
        ('d32', uint32_t),
        ('b', random_type_546),
    ]
xip_mode_code_data_t=xip_mode_code_data
class random_type_547(Structure):
    _fields_ = [
        ('flash_sel', uint32_t,1),
        ('reserved1_15', uint32_t,15),
    ]
class cfg_flash_sel_data(Union):
    _fields_ = [
        ('d32', uint32_t),
        ('b', random_type_547),
    ]
cfg_flash_sel_data_t=cfg_flash_sel_data
class random_type_548(Structure):
    _fields_ = [
        ('flash_read_cmd', uint32_t,8),
        ('reserved8_15', uint32_t,8),
    ]
class flash_read_cmd_data(Union):
    _fields_ = [
        ('d32', uint32_t),
        ('b', random_type_548),
    ]
flash_read_cmd_data_t=flash_read_cmd_data
class random_type_549(Structure):
    _fields_ = [
        ('disable_ahb_resp_mask', uint32_t,1),
        ('write_wrap_resp_mask', uint32_t,1),
        ('wrong_burst_size_resp_mask', uint32_t,1),
        ('overtime_resp_mask', uint32_t,1),
        ('reserved4_15', uint32_t,12),
    ]
class xip_read_error_resp_mask_data(Union):
    _fields_ = [
        ('d32', uint32_t),
        ('b', random_type_549),
    ]
xip_read_error_resp_mask_data_t=xip_read_error_resp_mask_data
class random_type_550(Structure):
    _fields_ = [
        ('data', uint32_t,16),
    ]
class cfg_xip_read_over_time_data(Union):
    _fields_ = [
        ('d32', uint32_t),
        ('b', random_type_550),
    ]
cfg_xip_read_over_time_data_t=cfg_xip_read_over_time_data
class random_type_551(Structure):
    _fields_ = [
        ('wait_cnt', uint32_t,8),
        ('reserved8_15', uint32_t,8),
    ]
class xip_cfg_wait_cnt0_data(Union):
    _fields_ = [
        ('d32', uint32_t),
        ('b', random_type_551),
    ]
xip_cfg_wait_cnt0_data_t=xip_cfg_wait_cnt0_data
class random_type_552(Structure):
    _fields_ = [
        ('read_qspi_sync', uint32_t,1),
        ('reserved1_15', uint32_t,15),
    ]
class xip_read_qspi_sync_data(Union):
    _fields_ = [
        ('d32', uint32_t),
        ('b', random_type_552),
    ]
xip_read_qspi_sync_data_t=xip_read_qspi_sync_data
class random_type_553(Structure):
    _fields_ = [
        ('cache_diag_en', uint32_t,1),
        ('opi_diag_en', uint32_t,1),
        ('qspi_diag_en', uint32_t,1),
        ('cache2ahbm_diag_en', uint32_t,1),
        ('first_32k_data_en', uint32_t,1),
        ('write_read_en', uint32_t,1),
        ('reserved4_15', uint32_t,12),
    ]
class xip_sub_diag_en_data(Union):
    _fields_ = [
        ('d32', uint32_t),
        ('b', random_type_553),
    ]
xip_sub_diag_en_data_t=xip_sub_diag_en_data
class random_type_554(Structure):
    _fields_ = [
        ('data', uint32_t,16),
    ]
class cfg_cache2habm_over_time_data(Union):
    _fields_ = [
        ('d32', uint32_t),
        ('b', random_type_554),
    ]
cfg_cache2habm_over_time_data_t=cfg_cache2habm_over_time_data
class random_type_555(Structure):
    _fields_ = [
        ('soft_rst_qspi0_xip_n', uint32_t,1),
        ('soft_rst_qspi1_xip_n', uint32_t,1),
        ('soft_rst_qspi3_xip_n', uint32_t,1),
        ('soft_rst_opi_xip_n', uint32_t,1),
        ('soft_rst_xip_cache_n', uint32_t,1),
        ('soft_rst_xip_sub_diag_n', uint32_t,1),
        ('soft_rst_pinmux_ctl_n', uint32_t,1),
        ('reserved7_15', uint32_t,9),
    ]
class mem_soft_rst_n_data(Union):
    _fields_ = [
        ('d32', uint32_t),
        ('b', random_type_555),
    ]
mem_soft_rst_n_data_t=mem_soft_rst_n_data
class random_type_556(Structure):
    _fields_ = [
        ('clr', uint32_t,1),
        ('en', uint32_t,1),
        ('reserved2_15', uint32_t,14),
    ]
class cfg_calculate_en_data(Union):
    _fields_ = [
        ('d32', uint32_t),
        ('b', random_type_556),
    ]
cfg_calculate_en_data_t=cfg_calculate_en_data
class random_type_557(Structure):
    _fields_ = [
        ('miss_load', uint32_t,1),
        ('reserved1_15', uint32_t,15),
    ]
class cache_miss_load_data(Union):
    _fields_ = [
        ('d32', uint32_t),
        ('b', random_type_557),
    ]
cache_miss_load_data_t=cache_miss_load_data
class random_type_558(Structure):
    _fields_ = [
        ('data', uint32_t,16),
    ]
class cache_data(Union):
    _fields_ = [
        ('d32', uint32_t),
        ('b', random_type_558),
    ]
cache_data_t=cache_data
class random_type_559(Structure):
    _fields_ = [
        ('write_read_opi_en', uint32_t,1),
        ('reserved1_15', uint32_t,15),
    ]
class xip_write_read_enable_data(Union):
    _fields_ = [
        ('d32', uint32_t),
        ('b', random_type_559),
    ]
xip_write_read_enable_data_t=xip_write_read_enable_data
class random_type_560(Structure):
    _fields_ = [
        ('redundant_cnt', uint32_t,7),
        ('psram_type', uint32_t,1),
        ('reserved9_15', uint32_t,8),
    ]
class write_redundant_cnt_data(Union):
    _fields_ = [
        ('d32', uint32_t),
        ('b', random_type_560),
    ]
write_redundant_cnt_data_t=write_redundant_cnt_data
class random_type_561(Structure):
    _fields_ = [
        ('write_read_opi_sync', uint32_t,1),
        ('reserved1_15', uint32_t,15),
    ]
class xip_write_read_sync_data(Union):
    _fields_ = [
        ('d32', uint32_t),
        ('b', random_type_561),
    ]
xip_write_read_sync_data_t=xip_write_read_sync_data
class xip_qspi(Structure):
    _fields_ = [
        ('xip_read_qspi_enable', uint32_t),
        ('xip_read_qspi_sync', uint32_t),
        ('cfg_wrap_operation', uint32_t),
        ('cfg_addr_24_32', uint32_t),
        ('cfg_flash_sel', uint32_t),
        ('xip_mode_code', uint32_t),
        ('flash_read_cmd', uint32_t),
        ('cfg_xip_read_over_time_l', uint32_t),
        ('cfg_xip_read_over_time_h', uint32_t),
        ('xip_read_error_resp_mask', uint32_t),
        ('res0', (uint32_t*6)),
        ('xip_read_disable_ahb_addr_l', uint32_t),
        ('xip_read_disable_ahb_addr_h', uint32_t),
        ('xip_read_write_addr_l', uint32_t),
        ('xip_read_write_addr_h', uint32_t),
        ('xip_read_write_data_l', uint32_t),
        ('xip_read_write_data_h', uint32_t),
        ('xip_read_wrong_burst_size_addr_l', uint32_t),
        ('xip_read_wrong_burst_size_addr_h', uint32_t),
        ('xip_read_qspi_apb_addr_l', uint32_t),
        ('xip_read_qspi_apb_addr_h', uint32_t),
        ('xip_read_cur_sts', uint32_t),
        ('res1', (uint32_t*37)),
    ]
xip_qspi_t=xip_qspi
class xip_regs(Structure):
    _fields_ = [
        ('cache_ctl_id', uint32_t),
        ('res0', (uint32_t*3)),
        ('gp_reg0', uint32_t),
        ('gp_reg1', uint32_t),
        ('gp_reg2', uint32_t),
        ('gp_reg3', uint32_t),
        ('mem_clken0', uint32_t),
        ('mem_clken1', uint32_t),
        ('res1', (uint32_t*2)),
        ('mem_soft_rst_n', uint32_t),
        ('res2', (uint32_t*7)),
        ('mem_div4', uint32_t),
        ('res3', (uint32_t*43)),
        ('xip_cache_en', uint32_t),
        ('xip_monitor_sel', uint32_t),
        ('xip_icu_en', uint32_t),
        ('cfg_cache2habm_over_time_l', uint32_t),
        ('cfg_cache2habm_over_time_h', uint32_t),
        ('xip_cache_error_resp_mask', uint32_t),
        ('res4', (uint32_t*2)),
        ('xip_cache_intr_sts', uint32_t),
        ('xip_cache_intr_mask_sts', uint32_t),
        ('xip_cache_intr_mask', uint32_t),
        ('xip_cache_intr_clr', uint32_t),
        ('res5', (uint32_t*4)),
        ('cfg_calculate_en', uint32_t),
        ('cache_miss_load', uint32_t),
        ('cache_total_h', uint32_t),
        ('cache_total_m', uint32_t),
        ('cache_total_l', uint32_t),
        ('cache_hit_h', uint32_t),
        ('cache_hit_m', uint32_t),
        ('cache_hit_l', uint32_t),
        ('cache_miss_h', uint32_t),
        ('cache_miss_m', uint32_t),
        ('cache_miss_l', uint32_t),
        ('res6', uint32_t),
        ('man_single', uint32_t),
        ('man_single_addr_h', uint32_t),
        ('man_single_addr_l', uint32_t),
        ('man_all', uint32_t),
        ('res7', (uint32_t*4)),
        ('diag_read_addr_l', uint32_t),
        ('diag_read_addr_h', uint32_t),
        ('diag_write_addr_l', uint32_t),
        ('diag_write_addr_h', uint32_t),
        ('diag_write_data_l', uint32_t),
        ('diag_write_data_h', uint32_t),
        ('cache2ahbm_cur_sts', uint32_t),
        ('res8', (uint32_t*21)),
        ('xip_write_read_enable', uint32_t),
        ('xip_write_read_sync', uint32_t),
        ('write_tcph_period', uint32_t),
        ('write_redundant_cnt', uint32_t),
        ('write_fifo_threshold', uint32_t),
        ('write_fifo_soft_reset', uint32_t),
        ('write_fifo_sts_clr', uint32_t),
        ('cfg_xip_opi_read_over_time_l', uint32_t),
        ('cfg_xip_opi_read_over_time_h', uint32_t),
        ('cfg_xip_write_over_time_l', uint32_t),
        ('cfg_xip_write_over_time_h', uint32_t),
        ('xip_write_read_error_resp_mask', uint32_t),
        ('cfg_xip_write_psram_cmd_l', uint32_t),
        ('cfg_xip_write_psram_cmd_h', uint32_t),
        ('cfg_xip_read_psram_cmd_l', uint32_t),
        ('cfg_xip_read_psram_cmd_h', uint32_t),
        ('cfg_clk_bus_low_freq', uint32_t),
        ('res9', (uint32_t*15)),
        ('write_fifo_sts', uint32_t),
        ('res10', (uint32_t*3)),
        ('xip_write_read_disable_ahb_add_l', uint32_t),
        ('xip_write_read_disable_ahb_add_h', uint32_t),
        ('xip_write_read_wrong_burst_size_addr_l', uint32_t),
        ('xip_write_read_wrong_burst_size_addr_h', uint32_t),
        ('xip_write_read_write_wrap_addr_l', uint32_t),
        ('xip_write_read_write_wrap_addr_h', uint32_t),
        ('xip_write_read_opi_apb_addr_l', uint32_t),
        ('xip_write_read_opi_apb_addr_h', uint32_t),
        ('xip_write_read_cur_sts_write', uint32_t),
        ('xip_write_read_cur_sts_read', uint32_t),
        ('res11', (uint32_t*18)),
        ('xip_qspi', (xip_qspi_t*2)),
        ('xip_ctl_intr_sts', uint32_t),
        ('xip_ctl_intr_mask_sts', uint32_t),
        ('xip_ctl_intr_mask', uint32_t),
        ('xip_ctl_intr_clr', uint32_t),
        ('res12', (uint32_t*60)),
        ('cache_ahb_s1_icm_priority', uint32_t),
        ('cache_ahb_s2_icm_priority', uint32_t),
        ('cache_ahb_s3_icm_priority', uint32_t),
        ('cache_ahb_s4_icm_priority', uint32_t),
        ('res13', (uint32_t*60)),
        ('mem_sub_gating_configure', uint32_t),
        ('mem_sub_gating_sts', uint32_t),
        ('res14', (uint32_t*2)),
        ('xip_sub_diag_en', uint32_t),
        ('xip_sub_diag_info', uint32_t),
        ('xip_sub_diag_clken', uint32_t),
        ('res15', uint32_t),
        ('rxds_sel', uint32_t),
        ('rxds_high_sel', uint32_t),
        ('res16', (uint32_t*250)),
        ('xip_cfg_wait_cnt0', uint32_t),
    ]
xip_regs_t=xip_regs
class random_type_562(Structure):
    _fields_ = [
        ('qspi0_div_num', uint32_t,3),
        ('reserved3', uint32_t,1),
        ('qspi1_div_num', uint32_t,3),
        ('reserved7', uint32_t,1),
        ('qspi3_div_num', uint32_t,3),
        ('reserved11_15', uint32_t,5),
    ]
class mem_div4_data(Union):
    _fields_ = [
        ('d32', uint32_t),
        ('b', random_type_562),
    ]
mem_div4_data_t=mem_div4_data
class random_type_563(Structure):
    _fields_ = [
        ('qspi0_div_en', uint32_t,1),
        ('qspi1_div_en', uint32_t,1),
        ('qspi3_div_en', uint32_t,1),
        ('reserved3', uint32_t,1),
        ('qspi0_div_clken', uint32_t,1),
        ('qspi1_div_clken', uint32_t,1),
        ('qspi3_div_clken', uint32_t,1),
        ('reserved7', uint32_t,1),
        ('qspi0_clken', uint32_t,1),
        ('qspi1_clken', uint32_t,1),
        ('qspi3_clken', uint32_t,1),
        ('opi_clken', uint32_t,1),
        ('qspi0_xip_clken', uint32_t,1),
        ('qspi1_xip_clken', uint32_t,1),
        ('qspi3_xip_clken', uint32_t,1),
        ('opi_xip_clken', uint32_t,1),
    ]
class mem_clken_data(Union):
    _fields_ = [
        ('d32', uint32_t),
        ('b', random_type_563),
    ]
mem_clken_data_t=mem_clken_data
hal_rtc_callback_t=td_u32
class rtc_regs_info(Structure):
    _fields_ = [
        ('load_count', uint32_t),
        ('current_value', uint32_t),
        ('control_reg', uint32_t),
        ('eoi', uint32_t),
        ('int_status', uint32_t),
    ]
rtc_regs_info_t=rtc_regs_info
class random_type_564(Structure):
    _fields_ = [
        ('enable', uint32_t,1),
        ('mode', uint32_t,1),
        ('int_mask', uint32_t,1),
        ('reserved3_31', uint32_t,29),
    ]
class rtc_control_reg_data(Union):
    _fields_ = [
        ('d32', uint32_t),
        ('b', random_type_564),
    ]
rtc_control_reg_data_t=rtc_control_reg_data
class random_type_565(Structure):
    _fields_ = [
        ('init', hal_gpio_init_t),
        ('deinit', hal_gpio_deinit_t),
        ('setdir', hal_gpio_setdir_t),
        ('getdir', hal_gpio_getdir_t),
        ('ctrl', hal_gpio_ctrl_t),
        ('output', hal_gpio_output_t),
        ('get_output_val', hal_gpio_getoutval_t),
        ('input', hal_gpio_input_t),
        ('registerfunc', hal_gpio_register_t),
        ('unregisterfunc', hal_gpio_unregister_t),
    ]
class random_type_568(Structure):
    _fields_ = [
        ('flag', td_bool),
        ('callback', gpio_callback_t),
    ]
cb_uart_is_need_awake_fn=td_u32
class random_type_569(Structure):
    _fields_ = [
        ('reserved_0', uint32_t,2),
        ('soft_rst_sha_n', uint32_t,1),
        ('reserved_1', uint32_t,29),
    ]
class rst_soft_n(Union):
    _fields_ = [
        ('b', random_type_569),
        ('b32', uint32_t),
    ]
rst_soft_n_t=rst_soft_n
class random_type_570(Structure):
    _fields_ = [
        ('reserved_0', uint32_t,2),
        ('sha_clken', uint32_t,1),
        ('reserved_1', uint32_t,29),
    ]
class soft_clken(Union):
    _fields_ = [
        ('b', random_type_570),
        ('b32', uint32_t),
    ]
soft_clken_t=soft_clken
class random_type_571(Structure):
    _fields_ = [
        ('cfg_endian', uint32_t,2),
        ('reserved_0', uint32_t,30),
    ]
class cfg_endian(Union):
    _fields_ = [
        ('b', random_type_571),
        ('b32', uint32_t),
    ]
cfg_endian_t=cfg_endian
class hash_sha_soft_ctrl_regs(Structure):
    _fields_ = [
        ('rst_soft_n', rst_soft_n_t),
        ('soft_clken', soft_clken_t),
        ('cfg_endian', cfg_endian_t),
    ]
sha_soft_ctrl_regs_t=hash_sha_soft_ctrl_regs
class random_type_572(Structure):
    _fields_ = [
        ('init', hal_sha_init_t),
        ('deinit', hal_sha_deinit_t),
        ('start', hal_sha_start_t),
        ('finish', hal_sha_finish_t),
    ]
class rsa_soft_ctrl_regs(Structure):
    _fields_ = [
        ('rst_soft_n', rst_soft_n_t),
        ('soft_clken', soft_clken_t),
        ('cfg_endian', cfg_endian_t),
    ]
rsa_soft_ctrl_regs_t=rsa_soft_ctrl_regs
class random_type_576(Structure):
    _fields_ = [
        ('en_efuse_switch_val', uint32_t,16),
    ]
class efuse_en_switch_data(Union):
    _fields_ = [
        ('d32', uint32_t),
        ('b', random_type_576),
    ]
en_switch_data_t=efuse_en_switch_data
class efuse_en_switch_reg(Structure):
    _fields_ = [
        ('en_switch_data', en_switch_data_t),
    ]
efuse_en_switch_reg_t=efuse_en_switch_reg
class random_type_577(Structure):
    _fields_ = [
        ('init', hal_efuse_init_t),
        ('deinit', hal_efuse_deinit_t),
        ('flush_write', hal_efuse_flush_write_t),
        ('refresh_read', hal_efuse_refresh_read_t),
        ('read_byte', hal_efuse_read_byte_t),
        ('write_byte', hal_efuse_write_byte_t),
        ('clear', hal_efuse_clear_all_write_regs_t),
        ('write_op', hal_efuse_write_operation_t),
        ('write_buffer_op', hal_efuse_write_buffer_operation_t),
        ('get_die_id', hal_efuse_get_die_id_t),
        ('get_chip_id', hal_efuse_get_chip_id_t),
    ]
class random_type_580(Structure):
    _fields_ = [
        ('mutex', td_u32),
    ]
class random_type_581(Structure):
    _fields_ = [
        ('uartilpr', uint32_t),
        ('uartibrd', uint32_t),
        ('uartfbrd', uint32_t),
        ('uartlcr_h', uint32_t),
        ('uartcr', uint32_t),
        ('uartifls', uint32_t),
        ('uartimsc', uint32_t),
        ('uartdmacr', uint32_t),
    ]
uart_recovery_regs_t=random_type_581
class random_type_582(Structure):
    _fields_ = [
        ('init', hal_uart_init_t),
        ('deinit', hal_uart_deinit_t),
        ('ctrl', hal_uart_ctrl_t),
        ('write', hal_uart_write_t),
        ('read', hal_uart_read_t),
        ('dma_cfg', hal_uart_dma_cfg_t),
    ]
class random_type_583(Structure):
    _fields_ = [
        ('init', hal_systick_init_t),
        ('deinit', hal_systick_deinit_t),
        ('ctrl', hal_systick_ctrl_t),
    ]
class random_type_584(Structure):
    _fields_ = [
        ('init', hal_tcxo_init_t),
        ('deinit', hal_tcxo_deinit_t),
        ('get', hal_tcxo_get_t),
    ]
hal_chip_watchdog_dump_callback_t=td_u32
class random_type_585(Structure):
    _fields_ = [
        ('init', hal_watchdog_init_t),
        ('deinit', hal_watchdog_deinit_t),
        ('set_attr', hal_watchdog_set_attr_t),
        ('get_attr', hal_watchdog_get_attr_t),
        ('enable', hal_watchdog_enable_t),
        ('disable', hal_watchdog_disable_t),
        ('kick', hal_watchdog_kick_t),
        ('get_left_time', hal_watchdog_get_time_t),
        ('register_callback', hal_register_watchdog_callback_t),
    ]
class random_type_588(Structure):
    _fields_ = [
        ('set_mode', hal_pin_set_mode),
        ('get_mode', hal_pin_get_mode),
        ('set_ds', hal_pin_set_ds),
        ('get_ds', hal_pin_get_ds),
        ('set_pull', hal_pin_set_pull),
        ('get_pull', hal_pin_get_pull),
        ('set_ie', hal_pin_set_ie),
        ('get_ie', hal_pin_get_ie),
    ]
class dma_hs_regs(Structure):
    _fields_ = [
        ('dma_hs_ch_config', (uint32_t*4)),
    ]
dma_hs_regs_t=dma_hs_regs
class spi_recovery_reg_s(Structure):
    _fields_ = [
        ('ctrlr0', uint32_t),
        ('ctrlr1', uint32_t),
        ('ser', uint32_t),
        ('baudr', uint32_t),
        ('txftlr', uint32_t),
        ('rxftlr', uint32_t),
        ('txflr', uint32_t),
        ('rxflr', uint32_t),
        ('imr', uint32_t),
        ('dmacr', uint32_t),
        ('dmatdlr', uint32_t),
        ('dmardlr', uint32_t),
        ('rx_sample_dly', uint32_t),
        ('spi_ctrlr0', uint32_t),
    ]
spi_recovery_cfg_t=spi_recovery_reg_s
class random_type_591(Structure):
    _fields_ = [
        ('init', hal_spi_init_t),
        ('deinit', hal_spi_deinit_t),
        ('write', hal_spi_write_t),
        ('read', hal_spi_read_t),
        ('writeread', hal_spi_writeread_t),
        ('ctrl', hal_spi_ctrl_t),
    ]
set_clk_div_t=td_u32
power_on_t=td_u32
class random_type_594(Structure):
    _fields_ = [
        ('button_pmu_pwr_cfg', td_u32),
        ('button_check_interrupt', td_u32),
        ('button_check_vbus_interrupt', td_u32),
        ('button_deal_vbus_event', td_u32),
        ('button_clear_interrupt', td_u32),
        ('button_get_state', td_u32),
        ('button_register_callback', td_u32),
        ('button_unregister_callback', td_u32),
        ('button_add_veto', td_u32),
        ('button_remove_veto', td_u32),
    ]
button_peripheral_api=random_type_594
isr_function=td_u32
class random_type_595(Structure):
    _fields_ = [
        ('pll_fbdiv', uint16_t),
        ('pll_frac_h', uint16_t),
        ('pll_frac_l', uint16_t),
        ('pll_postdiv1', uint8_t),
        ('pll_postdiv2', uint8_t),
    ]
cmu_pll_config_t=random_type_595
sdiom_clk_handler_func=td_u32
class random_type_608(Structure):
    _fields_ = [
        ('source_ptr', uint32_t),
        ('dest_ptr', uint32_t),
        ('length', uint16_t),
    ]
class random_type_611(Structure):
    _fields_ = [
        ('reboot_cause', uint32_t),
    ]
class random_type_612(Structure):
    _fields_ = [
        ('data', (uint8_t*3072)),
    ]
class random_type_613(Structure):
    _fields_ = [
        ('requested_reboot_reason', uint16_t),
    ]
class random_type_614(Structure):
    _fields_ = [
        ('log_level', uint32_t),
    ]
class random_type_617(Structure):
    _fields_ = [
        ('size', uint32_t),
    ]
class random_type_618(Structure):
    _fields_ = [
        ('result', uint32_t),
    ]
class random_type_619(Structure):
    _fields_ = [
        ('params', (uint8_t*256)),
        ('length', uint32_t),
    ]
class random_type_621(Structure):
    _fields_ = [
        ('type', uint8_t),
        ('event_id', uint8_t),
        ('info1', uint8_t),
        ('info2', uint8_t),
        ('info3', uint16_t),
    ]
class random_type_622(Structure):
    _fields_ = [
        ('mass_data', ipc_payload_mass_data_type),
        ('data_len', uint8_t),
        ('data', (uint8_t*128)),
    ]
class random_type_623(Structure):
    _fields_ = [
        ('data', (uint8_t*160)),
        ('length', uint16_t),
    ]
class random_type_624(Structure):
    _fields_ = [
        ('clock_type', uint32_t),
    ]
class random_type_625(Structure):
    _fields_ = [
        ('result', uint32_t),
    ]
class random_type_626(Structure):
    _fields_ = [
        ('status', uint16_t),
        ('length', uint16_t),
        ('msg', (td_char*60)),
    ]
class random_type_627(Structure):
    _fields_ = [
        ('flag', td_bool),
    ]
class random_type_628(Structure):
    _fields_ = [
        ('flag', td_bool),
    ]
class random_type_629(Structure):
    _fields_ = [
        ('flag', td_bool),
    ]
class random_type_630(Structure):
    _fields_ = [
        ('updata', uint32_t),
    ]
class random_type_631(Structure):
    _fields_ = [
        ('slave_freq', uint32_t),
    ]
class random_type_632(Structure):
    _fields_ = [
        ('freq', uint32_t),
    ]
class random_type_633(Structure):
    _fields_ = [
        ('power_switch', uint32_t),
    ]
class random_type_634(Structure):
    _fields_ = [
        ('config_nv_1', uint16_t),
        ('config_nv_2', uint16_t),
        ('config_nv_3', uint16_t),
        ('config_nv_4', uint16_t),
        ('config_nv_5', (uint16_t*5)),
        ('config_nv_6', uint16_t),
        ('config_nv_7', uint16_t),
        ('config_nv_8', uint16_t),
        ('config_nv_9', uint16_t),
        ('config_nv_10', uint8_t),
        ('config_nv_11', int8_t),
        ('config_nv_12', int8_t),
        ('flag', uint8_t),
    ]
class random_type_635(Structure):
    _fields_ = [
        ('nv_customize_config', uint32_t),
        ('nv_customize_config_reserved', uint32_t),
        ('nv_max_ble_tx_power_cfg_val', uint8_t),
        ('nv_max_gle_tx_power_cfg_val', uint8_t),
        ('fcc_flag', uint8_t),
        ('customize_four_k_support', uint8_t),
        ('customize_freq_map', (uint8_t*10)),
        ('reserved', (uint8_t*86)),
    ]
class random_type_637(Structure):
    _fields_ = [
        ('dsp_clk_level', uint16_t),
        ('dsp0_div', uint8_t),
        ('dsp1_div', uint8_t),
    ]
class random_type_638(Union):
    _fields_ = [
        ('freq', random_type_637),
        ('data', uint32_t),
    ]
dsp_freq_level_t=random_type_638
class random_type_640(Structure):
    _fields_ = [
        ('pll_fbdiv', uint16_t),
        ('pll_frac_h', uint16_t),
        ('pll_frac_l', uint16_t),
        ('pll_postdiv1', uint8_t),
        ('pll_postdiv2', uint8_t),
    ]
clock_switch_proc_func=td_u32
class random_type_643(Structure):
    _fields_ = [
        ('source_ptr', uint32_t),
        ('dest_ptr', uint32_t),
        ('length', uint16_t),
    ]
class random_type_646(Structure):
    _fields_ = [
        ('reboot_cause', uint32_t),
    ]
class random_type_647(Structure):
    _fields_ = [
        ('data', (uint8_t*3072)),
    ]
class random_type_648(Structure):
    _fields_ = [
        ('requested_reboot_reason', uint16_t),
    ]
class random_type_649(Structure):
    _fields_ = [
        ('log_level', uint32_t),
    ]
class random_type_652(Structure):
    _fields_ = [
        ('size', uint32_t),
    ]
class random_type_653(Structure):
    _fields_ = [
        ('result', uint32_t),
    ]
class random_type_654(Structure):
    _fields_ = [
        ('params', (uint8_t*256)),
        ('length', uint32_t),
    ]
class random_type_656(Structure):
    _fields_ = [
        ('type', uint8_t),
        ('event_id', uint8_t),
        ('info1', uint8_t),
        ('info2', uint8_t),
        ('info3', uint16_t),
    ]
class random_type_657(Structure):
    _fields_ = [
        ('mass_data', ipc_payload_mass_data_type),
        ('data_len', uint8_t),
        ('data', (uint8_t*128)),
    ]
class random_type_658(Structure):
    _fields_ = [
        ('data', (uint8_t*160)),
        ('length', uint16_t),
    ]
class random_type_659(Structure):
    _fields_ = [
        ('clock_type', uint32_t),
    ]
class random_type_660(Structure):
    _fields_ = [
        ('result', uint32_t),
    ]
class random_type_661(Structure):
    _fields_ = [
        ('status', uint16_t),
        ('length', uint16_t),
        ('msg', (td_char*60)),
    ]
class random_type_662(Structure):
    _fields_ = [
        ('flag', td_bool),
    ]
class random_type_663(Structure):
    _fields_ = [
        ('flag', td_bool),
    ]
class random_type_664(Structure):
    _fields_ = [
        ('flag', td_bool),
    ]
class random_type_665(Structure):
    _fields_ = [
        ('updata', uint32_t),
    ]
class random_type_666(Structure):
    _fields_ = [
        ('slave_freq', uint32_t),
    ]
class random_type_667(Structure):
    _fields_ = [
        ('freq', uint32_t),
    ]
class random_type_668(Structure):
    _fields_ = [
        ('power_switch', uint32_t),
    ]
class random_type_669(Structure):
    _fields_ = [
        ('config_nv_1', uint16_t),
        ('config_nv_2', uint16_t),
        ('config_nv_3', uint16_t),
        ('config_nv_4', uint16_t),
        ('config_nv_5', (uint16_t*5)),
        ('config_nv_6', uint16_t),
        ('config_nv_7', uint16_t),
        ('config_nv_8', uint16_t),
        ('config_nv_9', uint16_t),
        ('config_nv_10', uint8_t),
        ('config_nv_11', int8_t),
        ('config_nv_12', int8_t),
        ('flag', uint8_t),
    ]
class random_type_670(Structure):
    _fields_ = [
        ('nv_customize_config', uint32_t),
        ('nv_customize_config_reserved', uint32_t),
        ('nv_max_ble_tx_power_cfg_val', uint8_t),
        ('nv_max_gle_tx_power_cfg_val', uint8_t),
        ('fcc_flag', uint8_t),
        ('customize_four_k_support', uint8_t),
        ('customize_freq_map', (uint8_t*10)),
        ('reserved', (uint8_t*86)),
    ]
class random_type_671(Structure):
    _fields_ = [
        ('pll_fbdiv', uint16_t),
        ('pll_frac_h', uint16_t),
        ('pll_frac_l', uint16_t),
        ('pll_postdiv1', uint8_t),
        ('pll_postdiv2', uint8_t),
    ]
class random_type_673(Structure):
    _fields_ = [
        ('lock', uint32_t),
        ('req_closed', uint32_t),
    ]
pmu_vdd1p0_lock_core_t=random_type_673
class random_type_672(Structure):
    _fields_ = [
        ('system_lock_turn', uint32_t),
        ('m_req', pmu_vdd1p0_lock_core_t),
        ('s_req', pmu_vdd1p0_lock_core_t),
    ]
pmu_vdd1p0_lock_t=random_type_672
class random_type_674(Structure):
    _fields_ = [
        ('m_top0_l', uint16_t),
        ('m_top0_h', uint16_t),
        ('m_top1_l', uint16_t),
        ('m_top1_h', uint16_t),
        ('m_top2_l', uint16_t),
        ('m_top2_h', uint16_t),
        ('m_top3_l', uint16_t),
        ('m_top3_h', uint16_t),
        ('m_top4_l', uint16_t),
        ('m_top4_h', uint16_t),
        ('m_top5_l', uint16_t),
        ('m_top5_h', uint16_t),
        ('m_clocken0', uint16_t),
        ('m_clocken1', uint16_t),
        ('m_clocken2', uint16_t),
        ('m_glb_clocken', uint16_t),
        ('m_perp_ls_cr', uint16_t),
    ]
hal_pmu_m_clocks_state_t=random_type_674
nmi_proc_func=td_u32
class random_type_675(Structure):
    _fields_ = [
        ('ccause', uint32_t),
        ('mcause', uint32_t),
        ('mtval', uint32_t),
        ('gp', uint32_t),
        ('task_context', task_context_t),
    ]
exc_context_t=random_type_675
class random_type_676(Structure):
    _fields_ = [
        ('mstatus', uint32_t),
        ('mepc', uint32_t),
        ('tp', uint32_t),
        ('sp', uint32_t),
        ('s11', uint32_t),
        ('s10', uint32_t),
        ('s9', uint32_t),
        ('s8', uint32_t),
        ('s7', uint32_t),
        ('s6', uint32_t),
        ('s5', uint32_t),
        ('s4', uint32_t),
        ('s3', uint32_t),
        ('s2', uint32_t),
        ('s1', uint32_t),
        ('s0', uint32_t),
        ('t6', uint32_t),
        ('t5', uint32_t),
        ('t4', uint32_t),
        ('t3', uint32_t),
        ('a7', uint32_t),
        ('a6', uint32_t),
        ('a5', uint32_t),
        ('a4', uint32_t),
        ('a3', uint32_t),
        ('a2', uint32_t),
        ('a1', uint32_t),
        ('a0', uint32_t),
        ('t2', uint32_t),
        ('t1', uint32_t),
        ('t0', uint32_t),
        ('ra', uint32_t),
    ]
class exc_info(Structure):
    _fields_ = [
        ('phase', uint16_t),
        ('type', uint16_t),
        ('fault_addr', uint32_t),
        ('thrd_pid', uint32_t),
        ('nest_cnt', uint16_t),
        ('reserved', uint16_t),
        ('context', td_u32),
    ]
exc_info_t=exc_info
class random_type_677(Structure):
    _fields_ = [
        ('nmi_int_raw_sts', uint16_t),
        ('xip_cache_int_sts', uint16_t),
        ('xip_int_sts', uint16_t),
        ('reserved', uint16_t),
    ]
exc_nmi_stat_t=random_type_677
hal_exception_dump_callback=td_u32
class random_type_678(Structure):
    _fields_ = [
        ('ccause', UINT32),
        ('mcause', UINT32),
        ('mtval', UINT32),
        ('gp', UINT32),
        ('taskContext', TaskContext),
    ]
class random_type_679(Structure):
    _fields_ = [
        ('mstatus', UINT32),
        ('mepc', UINT32),
        ('tp', UINT32),
        ('sp', UINT32),
        ('s11', UINT32),
        ('s10', UINT32),
        ('s9', UINT32),
        ('s8', UINT32),
        ('s7', UINT32),
        ('s6', UINT32),
        ('s5', UINT32),
        ('s4', UINT32),
        ('s3', UINT32),
        ('s2', UINT32),
        ('s1', UINT32),
        ('s0', UINT32),
        ('t6', UINT32),
        ('t5', UINT32),
        ('t4', UINT32),
        ('t3', UINT32),
        ('a7', UINT32),
        ('a6', UINT32),
        ('a5', UINT32),
        ('a4', UINT32),
        ('a3', UINT32),
        ('a2', UINT32),
        ('a1', UINT32),
        ('a0', UINT32),
        ('t2', UINT32),
        ('t1', UINT32),
        ('t0', UINT32),
        ('ra', UINT32),
        ('fs11', FLOATREG_TYPE),
        ('fs10', FLOATREG_TYPE),
        ('fs9', FLOATREG_TYPE),
        ('fs8', FLOATREG_TYPE),
        ('fs7', FLOATREG_TYPE),
        ('fs6', FLOATREG_TYPE),
        ('fs5', FLOATREG_TYPE),
        ('fs4', FLOATREG_TYPE),
        ('fs3', FLOATREG_TYPE),
        ('fs2', FLOATREG_TYPE),
        ('fs1', FLOATREG_TYPE),
        ('fs0', FLOATREG_TYPE),
        ('ft11', FLOATREG_TYPE),
        ('ft10', FLOATREG_TYPE),
        ('ft9', FLOATREG_TYPE),
        ('ft8', FLOATREG_TYPE),
        ('fa7', FLOATREG_TYPE),
        ('fa6', FLOATREG_TYPE),
        ('fa5', FLOATREG_TYPE),
        ('fa4', FLOATREG_TYPE),
        ('fa3', FLOATREG_TYPE),
        ('fa2', FLOATREG_TYPE),
        ('fa1', FLOATREG_TYPE),
        ('fa0', FLOATREG_TYPE),
        ('ft7', FLOATREG_TYPE),
        ('ft6', FLOATREG_TYPE),
        ('ft5', FLOATREG_TYPE),
        ('ft4', FLOATREG_TYPE),
        ('ft3', FLOATREG_TYPE),
        ('ft2', FLOATREG_TYPE),
        ('ft1', FLOATREG_TYPE),
        ('ft0', FLOATREG_TYPE),
        ('fcsr', UINT32),
        ('reserved', (UINT32*3)),
    ]
class random_type_681(Structure):
    _fields_ = [
        ('ra', uint32_t),
        ('fp', uint32_t),
    ]
exc_stack_info_t=random_type_681
class random_type_680(Structure):
    _fields_ = [
        ('task_name', (td_char*16)),
        ('thrd_pid', uint32_t),
        ('type', uint16_t),
        ('nest_cnt', uint16_t),
        ('phase', uint16_t),
        ('ret', uint16_t),
        ('ccause', uint32_t),
        ('mcause', uint32_t),
        ('mtval', uint32_t),
        ('gp', uint32_t),
        ('mstatus', uint32_t),
        ('mepc', uint32_t),
        ('ra', uint32_t),
        ('sp', uint32_t),
        ('x4', uint32_t),
        ('x5', uint32_t),
        ('x6', uint32_t),
        ('x7', uint32_t),
        ('x8', uint32_t),
        ('x9', uint32_t),
        ('x10', uint32_t),
        ('x11', uint32_t),
        ('x12', uint32_t),
        ('x13', uint32_t),
        ('x14', uint32_t),
        ('x15', uint32_t),
        ('x16', uint32_t),
        ('x17', uint32_t),
        ('x18', uint32_t),
        ('x19', uint32_t),
        ('x20', uint32_t),
        ('x21', uint32_t),
        ('x22', uint32_t),
        ('x23', uint32_t),
        ('x24', uint32_t),
        ('x25', uint32_t),
        ('x26', uint32_t),
        ('x27', uint32_t),
        ('x28', uint32_t),
        ('x29', uint32_t),
        ('x30', uint32_t),
        ('x31', uint32_t),
        ('reg0', uint32_t),
        ('reg1', uint32_t),
        ('reg2', uint32_t),
        ('reg3', uint32_t),
        ('stack_size', uint32_t),
        ('stack', (exc_stack_info_t*0)),
    ]
exc_info_save_t=random_type_680
class random_type_682(Structure):
    _fields_ = [
        ('put_msg_2_cache_fail_times', uint32_t),
        ('dfx_msg_q_num', uint16_t),
        ('dfx_msg_q_peak_num', uint16_t),
        ('transmit_msg_q_num', uint16_t),
        ('transmit_msg_q_peak_num', uint16_t),
        ('dfx_msg_process_max_time', uint32_t),
        ('transmit_msg_process_max_time', uint32_t),
        ('send_local_q_fail', uint32_t),
        ('send_local_q_success', uint32_t),
        ('transmit_msg_rev_cnt', uint32_t),
        ('msg_rev_cnt', uint32_t),
        ('diag_pkt_msg_rev_cnt', uint32_t),
        ('beat_heart_msg_rev_cnt', uint32_t),
        ('channel_receive_data_cnt', (uint32_t*4)),
        ('channel_receive_frame_cnt', (uint32_t*4)),
        ('mem_pkt_alloc_size', (uint32_t*2)),
        ('mem_pkt_free_size', (uint32_t*2)),
        ('alloc_mem_size', uint32_t),
        ('alloc_mem_peak_size', uint32_t),
        ('conn_excep_cnt', uint32_t),
        ('conn_bu_cnt', uint32_t),
    ]
zdiag_dfx_stat_t=random_type_682
class calendar(Structure):
    _fields_ = [
        ('sec', uint32_t),
        ('min', uint32_t),
        ('hour', uint32_t),
        ('day', uint32_t),
        ('mon', uint32_t),
        ('year', uint32_t),
    ]
calendar_t=calendar
class random_type_683(Structure):
    _fields_ = [
        ('name', td_u32),
        ('start_addr', uintptr_t),
        ('len', uint32_t),
    ]
mem_dump_to_file_t=random_type_683
class random_type_684(Structure):
    _fields_ = [
        ('ccause', uint32_t),
        ('mcause', uint32_t),
        ('mtval', uint32_t),
        ('gp', uint32_t),
        ('task_context', task_context_t),
    ]
class random_type_685(Structure):
    _fields_ = [
        ('mstatus', uint32_t),
        ('mepc', uint32_t),
        ('tp', uint32_t),
        ('sp', uint32_t),
        ('s11', uint32_t),
        ('s10', uint32_t),
        ('s9', uint32_t),
        ('s8', uint32_t),
        ('s7', uint32_t),
        ('s6', uint32_t),
        ('s5', uint32_t),
        ('s4', uint32_t),
        ('s3', uint32_t),
        ('s2', uint32_t),
        ('s1', uint32_t),
        ('s0', uint32_t),
        ('t6', uint32_t),
        ('t5', uint32_t),
        ('t4', uint32_t),
        ('t3', uint32_t),
        ('a7', uint32_t),
        ('a6', uint32_t),
        ('a5', uint32_t),
        ('a4', uint32_t),
        ('a3', uint32_t),
        ('a2', uint32_t),
        ('a1', uint32_t),
        ('a0', uint32_t),
        ('t2', uint32_t),
        ('t1', uint32_t),
        ('t0', uint32_t),
        ('ra', uint32_t),
    ]
class random_type_686(Structure):
    _fields_ = [
        ('mutex', td_u32),
    ]
diag_addr=td_uchar
class random_type_687(Structure):
    _fields_ = [
        ('peer_addr', diag_addr),
        ('pad', (uint8_t*3)),
    ]
spp_receive_data_callback=td_u32
spp_conn_state_changed_callback=td_u32
class random_type_688(Structure):
    _fields_ = [
        ('receive_data_cb', spp_receive_data_callback),
        ('conn_state_changed_cb', spp_conn_state_changed_callback),
    ]
spp_callbacks_t=random_type_688
gatts_add_service_callback=td_u32
gatts_add_characteristic_callback=td_u32
gatts_add_descriptor_callback=td_u32
gatts_start_service_callback=td_u32
gatts_stop_service_callback=td_u32
gatts_delete_all_service_callback=td_u32
gatts_read_request_callback=td_u32
gatts_write_request_callback=td_u32
gatts_mtu_changed_callback=td_u32
class random_type_689(Structure):
    _fields_ = [
        ('add_service_cb', gatts_add_service_callback),
        ('add_characteristic_cb', gatts_add_characteristic_callback),
        ('add_descriptor_cb', gatts_add_descriptor_callback),
        ('start_service_cb', gatts_start_service_callback),
        ('stop_service_cb', gatts_stop_service_callback),
        ('delete_service_cb', gatts_delete_all_service_callback),
        ('read_request_cb', gatts_read_request_callback),
        ('write_request_cb', gatts_write_request_callback),
        ('mtu_changed_cb', gatts_mtu_changed_callback),
    ]
gatts_callbacks_t=random_type_689
class random_type_690(Structure):
    _fields_ = [
        ('uuid_len', uint8_t),
        ('uuid', (uint8_t*16)),
    ]
bt_uuid_t=random_type_690
class random_type_691(Structure):
    _fields_ = [
        ('handle', uint16_t),
        ('value_handle', uint16_t),
    ]
gatts_add_character_result_t=random_type_691
class random_type_692(Structure):
    _fields_ = [
        ('request_id', uint16_t),
        ('handle', uint16_t),
        ('offset', uint16_t),
        ('need_rsp', td_bool),
        ('need_authorize', td_bool),
        ('is_long', td_bool),
    ]
gatts_req_read_cb_t=random_type_692
class random_type_693(Structure):
    _fields_ = [
        ('request_id', uint16_t),
        ('handle', uint16_t),
        ('offset', uint16_t),
        ('need_rsp', td_bool),
        ('need_authorize', td_bool),
        ('is_prep', td_bool),
        ('length', uint16_t),
        ('value', td_u32),
    ]
gatts_req_write_cb_t=random_type_693
class random_type_694(Structure):
    _fields_ = [
        ('attr_handle', uint16_t),
        ('value_len', uint16_t),
        ('value', td_u32),
    ]
diag_option_t=random_type_687
gatts_ntf_ind_t=random_type_694
diag_cmd_f=td_u32
class random_type_695(Structure):
    _fields_ = [
        ('min_id', uint16_t),
        ('max_id', uint16_t),
        ('fn_input_cmd', diag_cmd_f),
    ]
diag_cmd_reg_obj_t=random_type_695
class random_type_696(Structure):
    _fields_ = [
        ('peer_addr', diag_addr),
        ('pad', (uint8_t*3)),
    ]
class random_type_697(Structure):
    _fields_ = [
        ('put_msg_2_cache_fail_times', uint32_t),
        ('dfx_msg_q_num', uint16_t),
        ('dfx_msg_q_peak_num', uint16_t),
        ('transmit_msg_q_num', uint16_t),
        ('transmit_msg_q_peak_num', uint16_t),
        ('dfx_msg_process_max_time', uint32_t),
        ('transmit_msg_process_max_time', uint32_t),
        ('send_local_q_fail', uint32_t),
        ('send_local_q_success', uint32_t),
        ('transmit_msg_rev_cnt', uint32_t),
        ('msg_rev_cnt', uint32_t),
        ('diag_pkt_msg_rev_cnt', uint32_t),
        ('beat_heart_msg_rev_cnt', uint32_t),
        ('channel_receive_data_cnt', (uint32_t*4)),
        ('channel_receive_frame_cnt', (uint32_t*4)),
        ('mem_pkt_alloc_size', (uint32_t*2)),
        ('mem_pkt_free_size', (uint32_t*2)),
        ('alloc_mem_size', uint32_t),
        ('alloc_mem_peak_size', uint32_t),
        ('conn_excep_cnt', uint32_t),
        ('conn_bu_cnt', uint32_t),
    ]
diag_report_sys_msg_handler=td_u32
class random_type_698(Structure):
    _fields_ = [
        ('report_sys_msg', diag_report_sys_msg_handler),
    ]
class dfx_write_data_interface(Structure):
    _fields_ = [
        ('write', td_u32),
        ('fd', td_u32),
    ]
diag_rom_api_t=random_type_698
dfx_write_data_interface_t=dfx_write_data_interface
class diag_mem_config(Structure):
    _fields_ = [
        ('start_addr', uintptr_t),
        ('end_addr', uintptr_t),
    ]
diag_mem_config_t=diag_mem_config
class random_type_699(Structure):
    _fields_ = [
        ('name', td_u32),
        ('start_addr', uintptr_t),
        ('len', uint32_t),
    ]
diag_mem_dump_t=random_type_699
class random_type_700(Structure):
    _fields_ = [
        ('name', td_u32),
        ('start_addr', uintptr_t),
        ('end_addr', uintptr_t),
    ]
diag_reg_dump_t=random_type_700
class random_type_702(Structure):
    _fields_ = [
        ('frame_start', uint8_t),
        ('func_type', uint8_t),
        ('prime_id', uint8_t),
        ('arr_reserver', (uint8_t*1)),
        ('frame_len', uint16_t),
        ('sn', uint16_t),
    ]
om_msg_header_stru_t=random_type_702
class random_type_701(Structure):
    _fields_ = [
        ('header', om_msg_header_stru_t),
        ('msg_id', uint16_t),
        ('data_len', uint16_t),
    ]
om_pcm_header_t=random_type_701
class random_type_703(Structure):
    _fields_ = [
        ('peer_addr', diag_addr),
        ('pad', (uint8_t*3)),
    ]
class random_type_704(Structure):
    _fields_ = [
        ('ret', uint32_t),
        ('flag', uint32_t),
        ('transmit_id', uint32_t),
    ]
diag_sample_data_ind_t=random_type_704
class random_type_705(Structure):
    _fields_ = [
        ('flag', uint32_t),
        ('transmit_id', uint32_t),
    ]
diag_sample_data_cmd_t=random_type_705
oam_cmd_handle_callback=td_u32
class random_type_706(Structure):
    _fields_ = [
        ('msg', td_u32),
        ('len', uint32_t),
    ]
zdiag_adapt_sdt_msg_t=random_type_706
class random_type_707(Structure):
    _fields_ = [
        ('frame_start', uint8_t),
        ('func_type', uint8_t),
        ('prime_id', uint8_t),
        ('arr_reserver', (uint8_t*1)),
        ('frame_len', uint16_t),
        ('sn', uint16_t),
    ]
class random_type_708(Structure):
    _fields_ = [
        ('header', om_msg_header_stru_t),
        ('msg_id', uint16_t),
        ('data_len', uint16_t),
    ]
om_ota_header_t=random_type_708
class _om_status_data_stru_t(Structure):
    _fields_ = [
        ('header', om_msg_header_stru_t),
        ('msg_id', uint16_t),
        ('data_len', uint16_t),
        ('data', (uint8_t*60)),
        ('frame_tail', uint8_t),
    ]
om_status_data_stru_t=_om_status_data_stru_t
class random_type_710(Structure):
    _fields_ = [
        ('msg_header', om_msg_header_stru_t),
        ('stack_limit', uint32_t),
        ('fault_type', uint32_t),
        ('fault_reason', uint32_t),
        ('address', uint32_t),
        ('reg_value', (uint32_t*32)),
        ('psp_value', uint32_t),
        ('lr_value', uint32_t),
        ('pc_value', uint32_t),
        ('psps_value', uint32_t),
        ('primask_value', uint32_t),
        ('fault_mask_value', uint32_t),
        ('bserpri_value', uint32_t),
        ('control_value', uint32_t),
        ('msg_tail', uint8_t),
    ]
om_exception_info_stru_t=random_type_710
class diag_msg_para(Structure):
    _fields_ = [
        ('module_id', uint32_t),
        ('msg_id', uint32_t),
        ('no', uint32_t),
        ('buf', td_u32),
        ('buf_size', uint16_t),
        ('level', uint8_t),
    ]
diag_msg_para_t=diag_msg_para
class random_type_711(Structure):
    _fields_ = [
        ('put_msg_2_cache_fail_times', uint32_t),
        ('dfx_msg_q_num', uint16_t),
        ('dfx_msg_q_peak_num', uint16_t),
        ('transmit_msg_q_num', uint16_t),
        ('transmit_msg_q_peak_num', uint16_t),
        ('dfx_msg_process_max_time', uint32_t),
        ('transmit_msg_process_max_time', uint32_t),
        ('send_local_q_fail', uint32_t),
        ('send_local_q_success', uint32_t),
        ('transmit_msg_rev_cnt', uint32_t),
        ('msg_rev_cnt', uint32_t),
        ('diag_pkt_msg_rev_cnt', uint32_t),
        ('beat_heart_msg_rev_cnt', uint32_t),
        ('channel_receive_data_cnt', (uint32_t*4)),
        ('channel_receive_frame_cnt', (uint32_t*4)),
        ('mem_pkt_alloc_size', (uint32_t*2)),
        ('mem_pkt_free_size', (uint32_t*2)),
        ('alloc_mem_size', uint32_t),
        ('alloc_mem_peak_size', uint32_t),
        ('conn_excep_cnt', uint32_t),
        ('conn_bu_cnt', uint32_t),
    ]
class random_type_712(Structure):
    _fields_ = [
        ('suspend', td_u32),
        ('resume', td_u32),
        ('ioctl', td_u32),
    ]
pm_dev_ops_t=random_type_712
class random_type_713(Structure):
    _fields_ = [
        ('data', td_u32),
        ('len', uint32_t),
        ('handler', td_u32),
    ]
pm_fsm_content_t=random_type_713
class random_type_715(Structure):
    _fields_ = [
        ('total_counts', uint8_t),
        ('sub_counts', (uint8_t*22)),
    ]
pm_veto_counts_t=random_type_715
hifi_inform_handler=td_u32
class random_type_719(Structure):
    _fields_ = [
        ('section_type', uint32_t),
        ('section_run_begin', td_u32),
        ('section_run_end', td_u32),
        ('section_load_addr', td_u32),
        ('section_reserved_value', uint32_t),
    ]
section_info_t=random_type_719
class random_type_718(Structure):
    _fields_ = [
        ('section_num', uint32_t),
        ('reserved', uint32_t),
        ('section_info', (section_info_t*20)),
    ]
class random_type_720(Structure):
    _fields_ = [
        ('reg_cfg', uint32_t),
        ('reg_int_ie', (uint8_t*96)),
        ('reg_int_ip', (uint8_t*96)),
        ('reg_int_attr', (uint8_t*96)),
        ('reg_int_ctl', (uint8_t*96)),
    ]
clic_regs_group=random_type_720
class random_type_721(Structure):
    _fields_ = [
        ('pwr_en', uint8_t,1),
        ('pwr_sw_en', uint8_t,1),
        ('pwr_vset', uint8_t,6),
    ]
pm_pwr_cfg_t=random_type_721
class random_type_722(Structure):
    _fields_ = [
        ('slp_ms', uint32_t),
        ('handle', uint32_t),
        ('cb', rtc_callback_t),
    ]
pm_rtc_timer_t=random_type_722
class random_type_724(Structure):
    _fields_ = [
        ('suspend', td_u32),
        ('resume', td_u32),
        ('ioctl', td_u32),
    ]
class random_type_725(Structure):
    _fields_ = [
        ('data', td_u32),
        ('len', uint32_t),
        ('handler', td_u32),
    ]
class random_type_726(Structure):
    _fields_ = [
        ('item_id', uint32_t),
        ('file_path', td_u32),
    ]
partition_path_map_t=random_type_726
class upg_image_collections(Structure):
    _fields_ = [
        ('img_ids', td_u32),
        ('img_ids_cnt', uint32_t),
    ]
upg_image_collections_t=upg_image_collections
class upg_key_area_data(Structure):
    _fields_ = [
        ('image_id', uint32_t),
        ('struct_version', uint32_t),
        ('struct_length', uint32_t),
        ('signature_length', uint32_t),
        ('key_owner_id', uint32_t),
        ('key_id', uint32_t),
        ('key_alg', uint32_t),
        ('ecc_curve_type', uint32_t),
        ('key_length', uint32_t),
        ('fota_key_version_ext', uint32_t),
        ('mask_fota_key_version_ext', uint32_t),
        ('msid_ext', uint32_t),
        ('mask_msid_ext', uint32_t),
        ('maintenance_mode', uint32_t),
        ('die_id', (uint8_t*16)),
        ('fota_info_addr', uint32_t),
        ('reserved', (uint8_t*176)),
        ('fota_external_public_key', (uint8_t*516)),
        ('sig_fota_key_area', (uint8_t*512)),
    ]
upg_key_area_data_t=upg_key_area_data
class upg_fota_info_data(Structure):
    _fields_ = [
        ('image_id', uint32_t),
        ('struct_version', uint32_t),
        ('struct_length', uint32_t),
        ('signature_length', uint32_t),
        ('fota_version_ext', uint32_t),
        ('mask_fota_version_ext', uint32_t),
        ('msid_ext', uint32_t),
        ('mask_msid_ext', uint32_t),
        ('image_hash_table_addr', uint32_t),
        ('image_hash_table_length', uint32_t),
        ('image_hash_table_hash', (uint8_t*32)),
        ('image_num', uint32_t),
        ('hardware_id', uint32_t),
        ('user_defined', (uint8_t*176)),
        ('sign_fota_info', (uint8_t*512)),
    ]
upg_fota_info_data_t=upg_fota_info_data
class upg_image_hash_node(Structure):
    _fields_ = [
        ('image_id', uint32_t),
        ('image_addr', uint32_t),
        ('image_length', uint32_t),
        ('image_hash', (uint8_t*32)),
    ]
upg_image_hash_node_t=upg_image_hash_node
class upg_package_header(Structure):
    _fields_ = [
        ('key_area', upg_key_area_data_t),
        ('info_area', upg_fota_info_data_t),
    ]
class upg_image_header(Structure):
    _fields_ = [
        ('header_magic', uint32_t),
        ('image_id', uint32_t),
        ('image_offset', uint32_t),
        ('image_len', uint32_t),
        ('image_hash', (uint8_t*32)),
        ('old_image_len', uint32_t),
        ('old_image_hash', (uint8_t*32)),
        ('new_image_len', uint32_t),
        ('version_ext', uint32_t),
        ('version_mask', uint32_t),
        ('decompress_flag', uint32_t),
        ('re_enc_flag', uint32_t),
        ('root_key_type', uint32_t),
        ('enc_pk_l1', (uint8_t*16)),
        ('enc_pk_l2', (uint8_t*16)),
        ('iv', (uint8_t*16)),
        ('padding', (uint8_t*4)),
    ]
upg_package_header_t=upg_package_header
upg_image_header_t=upg_image_header
class upg_partition_id_ref(Structure):
    _fields_ = [
        ('primary_id', uint32_t),
        ('backup_id', uint32_t),
    ]
upg_partition_id_ref_t=upg_partition_id_ref
class TagCstlRawListNode(Structure):
    _fields_ = [
        ('next', td_u32),
        ('prev', td_u32),
    ]
CstlRawListNode=TagCstlRawListNode
class TagHashNode(Structure):
    _fields_ = [
        ('node', CstlRawListNode),
        ('key', uintptr_t),
        ('value', uintptr_t),
    ]
CstlHashNode=TagHashNode
CstlDupFunc=td_u32
CstlFreeFunc=td_u32
class random_type_731(Structure):
    _fields_ = [
        ('dupFunc', CstlDupFunc),
        ('freeFunc', CstlFreeFunc),
    ]
CstlDupFreeFuncPair=random_type_731
CstlHashMatchFunc=td_u32
CstlHashCodeCalcFunc=td_u32
class random_type_732(Structure):
    _fields_ = [
        ('head', CstlRawListNode),
        ('freeFunc', CstlFreeFunc),
    ]
CstlRawList=random_type_732
class CstlHashInfo(Structure):
    _fields_ = [
        ('keyFunc', CstlDupFreeFuncPair),
        ('valueFunc', CstlDupFreeFuncPair),
        ('matchFunc', CstlHashMatchFunc),
        ('hashFunc', CstlHashCodeCalcFunc),
        ('bucketSize', size_t),
        ('hashCount', size_t),
        ('listArray', (CstlRawList*0)),
    ]
CstlHash=CstlHashInfo
CstlHashIterator=td_u32
class random_type_733(Structure):
    _fields_ = [
        ('inputData', uintptr_t),
        ('dataSize', size_t),
    ]
CstlUserData=random_type_733
class random_type_734(Structure):
    _fields_ = [
        ('head', CstlRawListNode),
        ('freeFunc', CstlFreeFunc),
    ]
CstlDataCmpFunc=td_u32
CstlMatchFunc=td_u32
intptr_t=td_s32
class at_cmd_set_t(Structure):
    _fields_ = [
        ('cmd_set', td_u32),
        ('count', uint32_t),
    ]
at_cmd_set_t=at_cmd_set_t
class random_type_735(Structure):
    _fields_ = [
        ('str_cmd', td_u32),
        ('cmd_handle', at_cmd_handle),
        ('str_common', td_u32),
    ]
class random_type_736(Structure):
    _fields_ = [
        ('nv_tx_power_cali_num', uint8_t),
        ('nv_tx_power_cali_pref_offset', (int16_t*8)),
        ('nv_tx_power_cali_freq', (uint8_t*8)),
        ('nv_tx_power_coff', (int16_t*3)),
        ('nv_cali_switch_all', uint32_t),
        ('nv_ble_1m_max_power_val', uint16_t),
        ('nv_ble_2m_max_power_val', uint16_t),
        ('nv_br_max_power_val', uint16_t),
        ('nv_edr_gfsk_max_power_val', uint16_t),
        ('nv_customized_feature', uint8_t),
        ('nv_fcc_ind', uint8_t),
        ('nv_tws_monitor_switch', uint8_t),
        ('nv_tws_dual_switch', uint8_t),
    ]
btsrv_calibration_config_t=random_type_736
class random_type_737(Structure):
    _fields_ = [
        ('length', uint32_t),
        ('params', (uint8_t*256)),
    ]
ipc_calibration_config_t=random_type_737
class random_type_739(Structure):
    _fields_ = [
        ('source_ptr', uint32_t),
        ('dest_ptr', uint32_t),
        ('length', uint16_t),
    ]
class random_type_742(Structure):
    _fields_ = [
        ('reboot_cause', uint32_t),
    ]
class random_type_743(Structure):
    _fields_ = [
        ('data', (uint8_t*3072)),
    ]
class random_type_744(Structure):
    _fields_ = [
        ('requested_reboot_reason', uint16_t),
    ]
class random_type_745(Structure):
    _fields_ = [
        ('log_level', uint32_t),
    ]
class random_type_748(Structure):
    _fields_ = [
        ('size', uint32_t),
    ]
class random_type_749(Structure):
    _fields_ = [
        ('result', uint32_t),
    ]
class random_type_750(Structure):
    _fields_ = [
        ('params', (uint8_t*256)),
        ('length', uint32_t),
    ]
class random_type_752(Structure):
    _fields_ = [
        ('type', uint8_t),
        ('event_id', uint8_t),
        ('info1', uint8_t),
        ('info2', uint8_t),
        ('info3', uint16_t),
    ]
class random_type_753(Structure):
    _fields_ = [
        ('mass_data', ipc_payload_mass_data_type),
        ('data_len', uint8_t),
        ('data', (uint8_t*128)),
    ]
class random_type_754(Structure):
    _fields_ = [
        ('data', (uint8_t*160)),
        ('length', uint16_t),
    ]
class random_type_755(Structure):
    _fields_ = [
        ('clock_type', uint32_t),
    ]
class random_type_756(Structure):
    _fields_ = [
        ('result', uint32_t),
    ]
class random_type_757(Structure):
    _fields_ = [
        ('status', uint16_t),
        ('length', uint16_t),
        ('msg', (td_char*60)),
    ]
class random_type_758(Structure):
    _fields_ = [
        ('flag', td_bool),
    ]
class random_type_759(Structure):
    _fields_ = [
        ('flag', td_bool),
    ]
class random_type_760(Structure):
    _fields_ = [
        ('flag', td_bool),
    ]
class random_type_761(Structure):
    _fields_ = [
        ('updata', uint32_t),
    ]
class random_type_762(Structure):
    _fields_ = [
        ('slave_freq', uint32_t),
    ]
class random_type_763(Structure):
    _fields_ = [
        ('freq', uint32_t),
    ]
class random_type_764(Structure):
    _fields_ = [
        ('power_switch', uint32_t),
    ]
class random_type_765(Structure):
    _fields_ = [
        ('config_nv_1', uint16_t),
        ('config_nv_2', uint16_t),
        ('config_nv_3', uint16_t),
        ('config_nv_4', uint16_t),
        ('config_nv_5', (uint16_t*5)),
        ('config_nv_6', uint16_t),
        ('config_nv_7', uint16_t),
        ('config_nv_8', uint16_t),
        ('config_nv_9', uint16_t),
        ('config_nv_10', uint8_t),
        ('config_nv_11', int8_t),
        ('config_nv_12', int8_t),
        ('flag', uint8_t),
    ]
class random_type_766(Structure):
    _fields_ = [
        ('nv_customize_config', uint32_t),
        ('nv_customize_config_reserved', uint32_t),
        ('nv_max_ble_tx_power_cfg_val', uint8_t),
        ('nv_max_gle_tx_power_cfg_val', uint8_t),
        ('fcc_flag', uint8_t),
        ('customize_four_k_support', uint8_t),
        ('customize_freq_map', (uint8_t*10)),
        ('reserved', (uint8_t*86)),
    ]
class random_type_767(Structure):
    _fields_ = [
        ('payload_len', uint32_t),
        ('payload', td_u32),
    ]
class random_type_768(Structure):
    _fields_ = [
        ('version', uint32_t),
        ('build_version_info_magic', uint32_t),
        ('build_version_info_version', uint16_t),
        ('pad', uint16_t),
        ('epoch', uint32_t),
        ('string', (td_char*40)),
        ('partition_info', (uint16_t*4)),
        ('hash', (td_char*20)),
    ]
build_version_info=random_type_768
class random_type_770(Structure):
    _fields_ = [
        ('app_core_level', uint32_t),
        ('bt_core_level', uint32_t),
        ('dsp_core_level', uint32_t),
    ]
log_level_config_t=random_type_770
class random_type_771(Structure):
    _fields_ = [
        ('app_core_mode', uint32_t),
        ('bt_core_mode', uint32_t),
        ('dsp_core_mode', uint32_t),
    ]
log_mode_config_t=random_type_771
class random_type_774(Structure):
    _fields_ = [
        ('source_ptr', uint32_t),
        ('dest_ptr', uint32_t),
        ('length', uint16_t),
    ]
class random_type_777(Structure):
    _fields_ = [
        ('reboot_cause', uint32_t),
    ]
class random_type_778(Structure):
    _fields_ = [
        ('data', (uint8_t*3072)),
    ]
class random_type_779(Structure):
    _fields_ = [
        ('requested_reboot_reason', uint16_t),
    ]
class random_type_780(Structure):
    _fields_ = [
        ('log_level', uint32_t),
    ]
class random_type_782(Structure):
    _fields_ = [
        ('size', uint32_t),
    ]
class random_type_783(Structure):
    _fields_ = [
        ('result', uint32_t),
    ]
class random_type_784(Structure):
    _fields_ = [
        ('params', (uint8_t*256)),
        ('length', uint32_t),
    ]
class random_type_786(Structure):
    _fields_ = [
        ('type', uint8_t),
        ('event_id', uint8_t),
        ('info1', uint8_t),
        ('info2', uint8_t),
        ('info3', uint16_t),
    ]
class random_type_787(Structure):
    _fields_ = [
        ('mass_data', ipc_payload_mass_data_type),
        ('data_len', uint8_t),
        ('data', (uint8_t*128)),
    ]
class random_type_788(Structure):
    _fields_ = [
        ('data', (uint8_t*160)),
        ('length', uint16_t),
    ]
class random_type_789(Structure):
    _fields_ = [
        ('clock_type', uint32_t),
    ]
class random_type_790(Structure):
    _fields_ = [
        ('result', uint32_t),
    ]
class random_type_791(Structure):
    _fields_ = [
        ('status', uint16_t),
        ('length', uint16_t),
        ('msg', (td_char*60)),
    ]
class random_type_792(Structure):
    _fields_ = [
        ('flag', td_bool),
    ]
class random_type_793(Structure):
    _fields_ = [
        ('flag', td_bool),
    ]
class random_type_794(Structure):
    _fields_ = [
        ('flag', td_bool),
    ]
class random_type_795(Structure):
    _fields_ = [
        ('updata', uint32_t),
    ]
class random_type_796(Structure):
    _fields_ = [
        ('slave_freq', uint32_t),
    ]
class random_type_797(Structure):
    _fields_ = [
        ('freq', uint32_t),
    ]
class random_type_798(Structure):
    _fields_ = [
        ('power_switch', uint32_t),
    ]
class random_type_799(Structure):
    _fields_ = [
        ('config_nv_1', uint16_t),
        ('config_nv_2', uint16_t),
        ('config_nv_3', uint16_t),
        ('config_nv_4', uint16_t),
        ('config_nv_5', (uint16_t*5)),
        ('config_nv_6', uint16_t),
        ('config_nv_7', uint16_t),
        ('config_nv_8', uint16_t),
        ('config_nv_9', uint16_t),
        ('config_nv_10', uint8_t),
        ('config_nv_11', int8_t),
        ('config_nv_12', int8_t),
        ('flag', uint8_t),
    ]
class random_type_800(Structure):
    _fields_ = [
        ('nv_customize_config', uint32_t),
        ('nv_customize_config_reserved', uint32_t),
        ('nv_max_ble_tx_power_cfg_val', uint8_t),
        ('nv_max_gle_tx_power_cfg_val', uint8_t),
        ('fcc_flag', uint8_t),
        ('customize_four_k_support', uint8_t),
        ('customize_freq_map', (uint8_t*10)),
        ('reserved', (uint8_t*86)),
    ]
class random_type_801(Structure):
    _fields_ = [
        ('section_num', uint32_t),
        ('reserved', uint32_t),
        ('section_info', (section_info_t*20)),
    ]
section_head_t=random_type_801
class random_type_802(Structure):
    _fields_ = [
        ('section_type', uint32_t),
        ('section_run_begin', td_u32),
        ('section_run_end', td_u32),
        ('section_load_addr', td_u32),
        ('section_reserved_value', uint32_t),
    ]
cpu_utils_reboot_cb=td_u32
class random_type_804(Structure):
    _fields_ = [
        ('source_ptr', uint32_t),
        ('dest_ptr', uint32_t),
        ('length', uint16_t),
    ]
class random_type_807(Structure):
    _fields_ = [
        ('reboot_cause', uint32_t),
    ]
class random_type_808(Structure):
    _fields_ = [
        ('data', (uint8_t*3072)),
    ]
class random_type_809(Structure):
    _fields_ = [
        ('requested_reboot_reason', uint16_t),
    ]
class random_type_810(Structure):
    _fields_ = [
        ('log_level', uint32_t),
    ]
class random_type_813(Structure):
    _fields_ = [
        ('size', uint32_t),
    ]
class random_type_814(Structure):
    _fields_ = [
        ('result', uint32_t),
    ]
class random_type_815(Structure):
    _fields_ = [
        ('params', (uint8_t*256)),
        ('length', uint32_t),
    ]
class random_type_817(Structure):
    _fields_ = [
        ('type', uint8_t),
        ('event_id', uint8_t),
        ('info1', uint8_t),
        ('info2', uint8_t),
        ('info3', uint16_t),
    ]
class random_type_818(Structure):
    _fields_ = [
        ('mass_data', ipc_payload_mass_data_type),
        ('data_len', uint8_t),
        ('data', (uint8_t*128)),
    ]
class random_type_819(Structure):
    _fields_ = [
        ('data', (uint8_t*160)),
        ('length', uint16_t),
    ]
class random_type_820(Structure):
    _fields_ = [
        ('clock_type', uint32_t),
    ]
class random_type_821(Structure):
    _fields_ = [
        ('result', uint32_t),
    ]
class random_type_822(Structure):
    _fields_ = [
        ('status', uint16_t),
        ('length', uint16_t),
        ('msg', (td_char*60)),
    ]
class random_type_823(Structure):
    _fields_ = [
        ('flag', td_bool),
    ]
class random_type_824(Structure):
    _fields_ = [
        ('flag', td_bool),
    ]
class random_type_825(Structure):
    _fields_ = [
        ('flag', td_bool),
    ]
class random_type_826(Structure):
    _fields_ = [
        ('updata', uint32_t),
    ]
class random_type_827(Structure):
    _fields_ = [
        ('slave_freq', uint32_t),
    ]
class random_type_828(Structure):
    _fields_ = [
        ('freq', uint32_t),
    ]
class random_type_829(Structure):
    _fields_ = [
        ('power_switch', uint32_t),
    ]
class random_type_830(Structure):
    _fields_ = [
        ('config_nv_1', uint16_t),
        ('config_nv_2', uint16_t),
        ('config_nv_3', uint16_t),
        ('config_nv_4', uint16_t),
        ('config_nv_5', (uint16_t*5)),
        ('config_nv_6', uint16_t),
        ('config_nv_7', uint16_t),
        ('config_nv_8', uint16_t),
        ('config_nv_9', uint16_t),
        ('config_nv_10', uint8_t),
        ('config_nv_11', int8_t),
        ('config_nv_12', int8_t),
        ('flag', uint8_t),
    ]
class random_type_831(Structure):
    _fields_ = [
        ('nv_customize_config', uint32_t),
        ('nv_customize_config_reserved', uint32_t),
        ('nv_max_ble_tx_power_cfg_val', uint8_t),
        ('nv_max_gle_tx_power_cfg_val', uint8_t),
        ('fcc_flag', uint8_t),
        ('customize_four_k_support', uint8_t),
        ('customize_freq_map', (uint8_t*10)),
        ('reserved', (uint8_t*86)),
    ]
class random_type_832(Structure):
    _fields_ = [
        ('requested_reboot_reason', uint16_t),
    ]
class random_type_833(Structure):
    _fields_ = [
        ('min_id', uint16_t),
        ('max_id', uint16_t),
        ('fn_input_cmd', diag_cmd_f),
    ]
class random_type_834(Structure):
    _fields_ = [
        ('peer_addr', diag_addr),
        ('pad', (uint8_t*3)),
    ]
class random_type_835(Structure):
    _fields_ = [
        ('key', uint16_t),
        ('crc', uint16_t),
    ]
nv_read_input_t=random_type_835
class random_type_836(Structure):
    _fields_ = [
        ('ret', uint16_t),
        ('key', uint16_t),
        ('length', uint32_t),
        ('data', (uint8_t*0)),
    ]
nv_read_output_t=random_type_836
class random_type_838(Structure):
    _fields_ = [
        ('permanent', td_bool),
        ('encrypted', td_bool),
        ('non_upgrade', td_bool),
        ('reserve', uint8_t),
    ]
nv_attr=random_type_838
class random_type_837(Structure):
    _fields_ = [
        ('ret', uint32_t),
        ('key', uint16_t),
        ('length', uint16_t),
        ('attr', nv_attr),
        ('value', (uint8_t*0)),
    ]
nv_read_with_attr_output_t=random_type_837
class random_type_839(Structure):
    _fields_ = [
        ('key', uint16_t),
        ('crc', uint16_t),
        ('length', uint32_t),
        ('value', (uint8_t*0)),
    ]
nv_write_input_t=random_type_839
class random_type_840(Structure):
    _fields_ = [
        ('key', uint16_t),
        ('length', uint16_t),
        ('permanent', td_bool),
        ('encrypted', td_bool),
        ('non_upgrade', td_bool),
        ('value', (uint8_t*0)),
    ]
nv_write_with_attr_input_t=random_type_840
class random_type_841(Structure):
    _fields_ = [
        ('region_mode', (td_bool*16)),
    ]
nv_backup_mode_t=random_type_841
class random_type_842(Structure):
    _fields_ = [
        ('mode', uint8_t),
        ('region_flag', (td_bool*16)),
    ]
nv_reset_mode_t=random_type_842
class random_type_843(Structure):
    _fields_ = [
        ('region_mode', (td_bool*16)),
    ]
nv_restore_mode_t=random_type_843
class random_type_844(Structure):
    _fields_ = [
        ('put_msg_2_cache_fail_times', uint32_t),
        ('dfx_msg_q_num', uint16_t),
        ('dfx_msg_q_peak_num', uint16_t),
        ('transmit_msg_q_num', uint16_t),
        ('transmit_msg_q_peak_num', uint16_t),
        ('dfx_msg_process_max_time', uint32_t),
        ('transmit_msg_process_max_time', uint32_t),
        ('send_local_q_fail', uint32_t),
        ('send_local_q_success', uint32_t),
        ('transmit_msg_rev_cnt', uint32_t),
        ('msg_rev_cnt', uint32_t),
        ('diag_pkt_msg_rev_cnt', uint32_t),
        ('beat_heart_msg_rev_cnt', uint32_t),
        ('channel_receive_data_cnt', (uint32_t*4)),
        ('channel_receive_frame_cnt', (uint32_t*4)),
        ('mem_pkt_alloc_size', (uint32_t*2)),
        ('mem_pkt_free_size', (uint32_t*2)),
        ('alloc_mem_size', uint32_t),
        ('alloc_mem_peak_size', uint32_t),
        ('conn_excep_cnt', uint32_t),
        ('conn_bu_cnt', uint32_t),
    ]
class random_type_845(Structure):
    _fields_ = [
        ('permanent', td_bool),
        ('encrypted', td_bool),
        ('non_upgrade', td_bool),
        ('reserve', uint8_t),
    ]
class random_type_848(Structure):
    _fields_ = [
        ('store_id', uint16_t),
        ('ver', uint8_t),
        ('page_index', uint8_t),
    ]
kv_page_details_t=random_type_848
class random_type_847(Structure):
    _fields_ = [
        ('details', kv_page_details_t),
        ('inverted_details_word', uint32_t),
        ('sequence_number', uint32_t),
        ('inverted_sequence_number', uint32_t),
    ]
kv_page_header_t=random_type_847
kv_page_location=td_u32
class random_type_846(Structure):
    _fields_ = [
        ('page_header', kv_page_header_t),
        ('page_location', kv_page_location),
    ]
class random_type_849(Structure):
    _fields_ = [
        ('total_space', uint32_t),
        ('used_space', uint32_t),
        ('reclaimable_space', uint32_t),
        ('corrupted_space', uint32_t),
        ('max_key_space', uint32_t),
        ('first_writable_location', uint32_t),
    ]
class random_type_850(Structure):
    _fields_ = [
        ('total_space', uint32_t),
        ('used_space', uint32_t),
        ('reclaimable_space', uint32_t),
        ('corrupted_space', uint32_t),
        ('max_key_space', uint32_t),
    ]
class random_type_851(Structure):
    _fields_ = [
        ('entries', td_u32),
        ('page_status_map', td_u32),
        ('num_entries', uint32_t),
    ]
class random_type_852(Structure):
    _fields_ = [
        ('page_location', uint32_t),
        ('used_space', uint16_t),
        ('reclaimable_space', uint16_t),
        ('corrupted_space', uint16_t),
        ('first_writable_offset', uint16_t),
    ]
nv_page_status_map_t=random_type_852
class random_type_853(Structure):
    _fields_ = [
        ('ret', uint32_t),
        ('key', uint32_t),
    ]
nv_write_output_t=random_type_853
class random_type_854(Structure):
    _fields_ = [
        ('ret', uint32_t),
        ('key', uint32_t),
    ]
nv_write_with_attr_output_t=random_type_854
nv_key_attr_t=random_type_845
nv_store_status_t=random_type_850
kv_page_handle_t=random_type_846
kv_page_status_t=random_type_849
class random_type_855(Structure):
    _fields_ = [
        ('store_id', uint16_t),
        ('ver', uint8_t),
        ('page_index', uint8_t),
        ('inverted_details_word', uint32_t),
        ('sequence_number', uint32_t),
        ('inverted_sequence_number', uint32_t),
        ('total_space', uint32_t),
        ('used_space', uint32_t),
        ('reclaimable_space', uint32_t),
        ('corrupted_space', uint32_t),
        ('max_key_space', uint32_t),
        ('first_writable_location', uint32_t),
    ]
nv_page_usage_out=random_type_855
kv_nvregion_map_t=random_type_851
class random_type_856(Structure):
    _fields_ = [
        ('mode', uint8_t),
        ('region_flag', (uint8_t*16)),
        ('resverd', uint8_t),
        ('ret', uint32_t),
    ]
nv_reset_mode_out_t=random_type_856
class random_type_857(Structure):
    _fields_ = [
        ('min_id', uint16_t),
        ('max_id', uint16_t),
        ('fn_input_cmd', diag_cmd_f),
    ]
class random_type_858(Structure):
    _fields_ = [
        ('peer_addr', diag_addr),
        ('pad', (uint8_t*3)),
    ]
class upg_prepare_info(Structure):
    _fields_ = [
        ('package_len', uint32_t),
    ]
upg_prepare_info_t=upg_prepare_info
class random_type_859(Structure):
    _fields_ = [
        ('timer', td_u32),
        ('handler', td_u32),
        ('data', td_u32),
        ('interval', td_u32),
    ]
dfx_timer=random_type_859
temp_osal_handler=td_u32
class random_type_861(Structure):
    _fields_ = [
        ('peer_addr', diag_addr),
        ('pad', (uint8_t*3)),
    ]
dfx_timer_handler=td_u32
class random_type_862(Structure):
    _fields_ = [
        ('dir', uint32_t),
        ('random_data', uint32_t),
    ]
diag_beat_heart_cmd_ind_t=random_type_862
class random_type_863(Structure):
    _fields_ = [
        ('peer_addr', diag_addr),
        ('pad', (uint8_t*3)),
    ]
class random_type_865(Structure):
    _fields_ = [
        ('major_minor_version', uint32_t),
        ('revision_version', uint32_t),
        ('build_version', uint32_t),
    ]
ue_soft_version_t=random_type_865
class random_type_866(Structure):
    _fields_ = [
        ('version_v', uint16_t),
        ('version_r', uint16_t),
        ('version_c', uint16_t),
        ('version_b', uint16_t),
        ('version_spc', uint16_t),
        ('reserved', (uint16_t*3)),
    ]
ue_product_ver_t=random_type_866
class random_type_864(Structure):
    _fields_ = [
        ('rc', uint32_t),
        ('imei', (td_char*16)),
        ('ue_soft_version', ue_soft_version_t),
        ('build_version', ue_product_ver_t),
    ]
diag_cmd_host_connect_cnf_stru_t=random_type_864
class random_type_867(Structure):
    _fields_ = [
        ('peer_addr', diag_addr),
        ('pad', (uint8_t*3)),
    ]
class random_type_868(Structure):
    _fields_ = [
        ('module_id', uint16_t),
        ('is_dest_module', uint8_t),
        ('switch_code', uint8_t),
    ]
diag_cmd_msg_cfg_req_stru_t=random_type_868
class random_type_869(Structure):
    _fields_ = [
        ('total', uint32_t),
        ('used', uint32_t),
        ('free', uint32_t),
        ('free_node_num', uint32_t),
        ('used_node_num', uint32_t),
        ('max_free_node_size', uint32_t),
        ('peek_size', uint32_t),
    ]
class random_type_870(Structure):
    _fields_ = [
        ('peer_addr', diag_addr),
        ('pad', (uint8_t*3)),
    ]
mdm_mem_info_t=random_type_869
class random_type_871(Structure):
    _fields_ = [
        ('put_msg_2_cache_fail_times', uint32_t),
        ('dfx_msg_q_num', uint16_t),
        ('dfx_msg_q_peak_num', uint16_t),
        ('transmit_msg_q_num', uint16_t),
        ('transmit_msg_q_peak_num', uint16_t),
        ('dfx_msg_process_max_time', uint32_t),
        ('transmit_msg_process_max_time', uint32_t),
        ('send_local_q_fail', uint32_t),
        ('send_local_q_success', uint32_t),
        ('transmit_msg_rev_cnt', uint32_t),
        ('msg_rev_cnt', uint32_t),
        ('diag_pkt_msg_rev_cnt', uint32_t),
        ('beat_heart_msg_rev_cnt', uint32_t),
        ('channel_receive_data_cnt', (uint32_t*4)),
        ('channel_receive_frame_cnt', (uint32_t*4)),
        ('mem_pkt_alloc_size', (uint32_t*2)),
        ('mem_pkt_free_size', (uint32_t*2)),
        ('alloc_mem_size', uint32_t),
        ('alloc_mem_peak_size', uint32_t),
        ('conn_excep_cnt', uint32_t),
        ('conn_bu_cnt', uint32_t),
    ]
class random_type_872(Structure):
    _fields_ = [
        ('name', (td_char*32)),
        ('valid', td_bool),
        ('id', uint32_t),
        ('status', uint16_t),
        ('priority', uint16_t),
        ('task_sem', td_u32),
        ('task_mutex', td_u32),
        ('event_stru', (uint32_t*3)),
        ('event_mask', uint32_t),
        ('stack_size', uint32_t),
        ('top_of_stack', uint32_t),
        ('bottom_of_stack', uint32_t),
        ('sp', uint32_t),
        ('curr_used', uint32_t),
        ('peak_used', uint32_t),
        ('overflow_flag', uint32_t),
    ]
class random_type_873(Structure):
    _fields_ = [
        ('peer_addr', diag_addr),
        ('pad', (uint8_t*3)),
    ]
task_info_t=random_type_872
class random_type_874(Structure):
    _fields_ = [
        ('peer_addr', diag_addr),
        ('pad', (uint8_t*3)),
    ]
class random_type_875(Structure):
    _fields_ = [
        ('start_addr', uintptr_t),
        ('cnt', uint32_t),
    ]
mem_read_cmd_t=random_type_875
class random_type_877(Structure):
    _fields_ = [
        ('start_addr', uintptr_t),
        ('size', uint32_t),
    ]
mem_read_ind_head_t=random_type_877
class random_type_876(Structure):
    _fields_ = [
        ('head', mem_read_ind_head_t),
        ('data', (uint32_t*16)),
    ]
mem_read32_ind_t=random_type_876
class random_type_878(Structure):
    _fields_ = [
        ('head', mem_read_ind_head_t),
        ('data', (uint16_t*32)),
    ]
mem_read16_ind_t=random_type_878
class random_type_879(Structure):
    _fields_ = [
        ('head', mem_read_ind_head_t),
        ('data', (uint8_t*64)),
    ]
mem_read8_ind_t=random_type_879
class random_type_880(Structure):
    _fields_ = [
        ('start_addr', uintptr_t),
        ('val', uint32_t),
    ]
mem_write_cmd_t=random_type_880
class random_type_881(Structure):
    _fields_ = [
        ('ret', uint32_t),
    ]
mem_write_ind_t=random_type_881
class random_type_882(Structure):
    _fields_ = [
        ('peer_addr', diag_addr),
        ('pad', (uint8_t*3)),
    ]
class random_type_884(Structure):
    _fields_ = [
        ('err_no', uint16_t),
        ('res_try_cnt', uint16_t),
        ('res_lock_time', uint16_t),
        ('pad', uint16_t),
    ]
diag_cmd_conn_auth_ind_stru_t=random_type_884
class random_type_883(Structure):
    _fields_ = [
        ('pub_b', (uint8_t*384)),
        ('salt', (uint8_t*20)),
        ('salt_len', uint32_t),
        ('ret_ind', diag_cmd_conn_auth_ind_stru_t),
    ]
diag_cmd_conn_rsp_pub_b_stru_t=random_type_883
class random_type_885(Structure):
    _fields_ = [
        ('ret', uint32_t),
        ('rsp_m2', (uint8_t*32)),
        ('ret_ind', diag_cmd_conn_auth_ind_stru_t),
    ]
diag_cmd_conn_rsp_m_stru_t=random_type_885
class random_type_886(Structure):
    _fields_ = [
        ('id', uint32_t),
    ]
diag_dbg_stat_query_t=random_type_886
class random_type_887(Structure):
    _fields_ = [
        ('peer_addr', diag_addr),
        ('pad', (uint8_t*3)),
    ]
class random_type_888(Structure):
    _fields_ = [
        ('obj_id', uint16_t),
        ('object', td_u32),
        ('obj_size', uint16_t),
        ('obj_cnt', uint16_t),
        ('option', td_u32),
        ('sync', td_bool),
        ('pad', (uint8_t*2)),
    ]
zdiag_report_stat_obj_stru_t=random_type_888
diag_cmd_shell_data_proc=td_u32
zdiag_data_put_uart=td_u32
class random_type_889(Structure):
    _fields_ = [
        ('min_id', uint16_t),
        ('max_id', uint16_t),
        ('fn_input_cmd', diag_cmd_f),
    ]
class random_type_890(Structure):
    _fields_ = [
        ('peer_addr', diag_addr),
        ('pad', (uint8_t*3)),
    ]
class random_type_891(Structure):
    _fields_ = [
        ('put_msg_2_cache_fail_times', uint32_t),
        ('dfx_msg_q_num', uint16_t),
        ('dfx_msg_q_peak_num', uint16_t),
        ('transmit_msg_q_num', uint16_t),
        ('transmit_msg_q_peak_num', uint16_t),
        ('dfx_msg_process_max_time', uint32_t),
        ('transmit_msg_process_max_time', uint32_t),
        ('send_local_q_fail', uint32_t),
        ('send_local_q_success', uint32_t),
        ('transmit_msg_rev_cnt', uint32_t),
        ('msg_rev_cnt', uint32_t),
        ('diag_pkt_msg_rev_cnt', uint32_t),
        ('beat_heart_msg_rev_cnt', uint32_t),
        ('channel_receive_data_cnt', (uint32_t*4)),
        ('channel_receive_frame_cnt', (uint32_t*4)),
        ('mem_pkt_alloc_size', (uint32_t*2)),
        ('mem_pkt_free_size', (uint32_t*2)),
        ('alloc_mem_size', uint32_t),
        ('alloc_mem_peak_size', uint32_t),
        ('conn_excep_cnt', uint32_t),
        ('conn_bu_cnt', uint32_t),
    ]
class random_type_892(Structure):
    _fields_ = [
        ('name', (td_char*64)),
        ('total_size', uint32_t),
        ('start_address', uint32_t),
    ]
last_dump_data_ind_finish_t=random_type_892
class random_type_893(Structure):
    _fields_ = [
        ('peer_addr', diag_addr),
        ('pad', (uint8_t*3)),
    ]
class random_type_894(Structure):
    _fields_ = [
        ('file_num', uint32_t),
    ]
last_dump_start_ind_t=random_type_894
class random_type_895(Structure):
    _fields_ = [
        ('name', (td_char*64)),
        ('total_size', uint32_t),
        ('offset', uint32_t),
        ('size', uint32_t),
        ('crc', uint32_t),
        ('data', (uint8_t*0)),
    ]
last_dump_data_ind_t=random_type_895
class random_type_896(Structure):
    _fields_ = [
        ('transmit_id', uint32_t),
        ('offset', uint16_t),
    ]
diag_sample_data_transmit_info_t=random_type_896
class random_type_897(Structure):
    _fields_ = [
        ('peer_addr', diag_addr),
        ('pad', (uint8_t*3)),
    ]
class random_type_898(Structure):
    _fields_ = [
        ('transmit_id', uint32_t),
        ('ret', uint32_t),
        ('offset', uint32_t),
        ('size', uint32_t),
        ('crc', uint32_t),
        ('data', (uint8_t*0)),
    ]
diag_sample_data_reply_pkt_t=random_type_898
dfx_file_report_node_hook=td_u32
class random_type_899(Structure):
    _fields_ = [
        ('is_dir', td_bool),
        ('name', td_u32),
        ('file_size', uint32_t),
    ]
dfx_file_ls_node_info_t=random_type_899
class random_type_900(Structure):
    _fields_ = [
        ('put_msg_2_cache_fail_times', uint32_t),
        ('dfx_msg_q_num', uint16_t),
        ('dfx_msg_q_peak_num', uint16_t),
        ('transmit_msg_q_num', uint16_t),
        ('transmit_msg_q_peak_num', uint16_t),
        ('dfx_msg_process_max_time', uint32_t),
        ('transmit_msg_process_max_time', uint32_t),
        ('send_local_q_fail', uint32_t),
        ('send_local_q_success', uint32_t),
        ('transmit_msg_rev_cnt', uint32_t),
        ('msg_rev_cnt', uint32_t),
        ('diag_pkt_msg_rev_cnt', uint32_t),
        ('beat_heart_msg_rev_cnt', uint32_t),
        ('channel_receive_data_cnt', (uint32_t*4)),
        ('channel_receive_frame_cnt', (uint32_t*4)),
        ('mem_pkt_alloc_size', (uint32_t*2)),
        ('mem_pkt_free_size', (uint32_t*2)),
        ('alloc_mem_size', uint32_t),
        ('alloc_mem_peak_size', uint32_t),
        ('conn_excep_cnt', uint32_t),
        ('conn_bu_cnt', uint32_t),
    ]
class DIR(Structure):
    _fields_ = [
    ]
class random_type_901(Structure):
    _fields_ = [
        ('data0', uint32_t),
    ]
zdiag_log_msg1_t=random_type_901
class random_type_902(Structure):
    _fields_ = [
        ('data0', uint32_t),
        ('data1', uint32_t),
    ]
zdiag_log_msg2_t=random_type_902
class random_type_903(Structure):
    _fields_ = [
        ('data0', uint32_t),
        ('data1', uint32_t),
        ('data2', uint32_t),
    ]
zdiag_log_msg3_t=random_type_903
class random_type_904(Structure):
    _fields_ = [
        ('data0', uint32_t),
        ('data1', uint32_t),
        ('data2', uint32_t),
        ('data3', uint32_t),
    ]
zdiag_log_msg4_t=random_type_904
class random_type_905(Structure):
    _fields_ = [
        ('report_sys_msg', diag_report_sys_msg_handler),
    ]
class random_type_906(Structure):
    _fields_ = [
        ('sof', uint8_t),
        ('ctrl', uint8_t),
        ('inner', (uint8_t*0)),
    ]
diag_router_frame_t=random_type_906
class random_type_907(Structure):
    _fields_ = [
        ('len_lsb', uint8_t),
        ('len_msb', uint8_t),
        ('payload', (uint8_t*0)),
    ]
diag_router_inner_frame_t=random_type_907
class random_type_908(Structure):
    _fields_ = [
        ('data', (td_u32*3)),
        ('data_len', (uint16_t*3)),
        ('data_cnt', uint8_t,2),
        ('need_free', uint8_t,1),
        ('single_task', uint8_t,1),
        ('critical', uint8_t,1),
        ('output_type', uint8_t,2),
        ('resvered', uint8_t,1),
    ]
diag_pkt_handle_t=random_type_908
class random_type_909(Structure):
    _fields_ = [
        ('len_lsb', uint8_t),
        ('len_msb', uint8_t),
        ('payload', (uint8_t*0)),
    ]
class random_type_910(Structure):
    _fields_ = [
        ('sof', uint8_t),
        ('ctrl', uint8_t),
        ('inner', (uint8_t*0)),
    ]
class random_type_911(Structure):
    _fields_ = [
        ('data', (td_u32*3)),
        ('data_len', (uint16_t*3)),
        ('data_cnt', uint8_t,2),
        ('need_free', uint8_t,1),
        ('single_task', uint8_t,1),
        ('critical', uint8_t,1),
        ('output_type', uint8_t,2),
        ('resvered', uint8_t,1),
    ]
class random_type_912(Structure):
    _fields_ = [
        ('module', uint32_t),
        ('dest_mod', uint32_t),
        ('no', uint32_t),
        ('id', uint32_t),
        ('time', uint32_t),
        ('data', (int8_t*0)),
    ]
diag_cmd_log_layer_stru_t=random_type_912
class random_type_913(Structure):
    _fields_ = [
        ('module', uint8_t),
        ('no_lo', uint8_t),
        ('no_hi', uint16_t,6),
        ('time_ms', uint16_t,10),
        ('id', uint32_t),
        ('time', uint32_t),
        ('data', (int8_t*0)),
    ]
diag_cmd_normal_log_layer_t=random_type_913
class random_type_914(Structure):
    _fields_ = [
        ('sub_mod', uint8_t),
        ('no', uint8_t),
        ('id', uint32_t),
        ('time', uint32_t),
        ('data', (int8_t*0)),
    ]
diag_cmd_extend_log_layer_t=random_type_914
class random_type_915(Structure):
    _fields_ = [
        ('no', uint16_t),
        ('id', uint32_t),
        ('data', (int8_t*0)),
    ]
diag_cmd_minimal_log_layer_t=random_type_915
class random_type_916(Structure):
    _fields_ = [
        ('mod_id', uint8_t),
        ('cmd_id', uint8_t),
        ('param', (uint8_t*0)),
    ]
diag_head_req_stru_t=random_type_916
class random_type_917(Structure):
    _fields_ = [
        ('mod_id', uint8_t),
        ('cmd_id', uint8_t),
        ('param', (uint8_t*0)),
    ]
diag_head_ind_stru_t=random_type_917
class random_type_918(Structure):
    _fields_ = [
        ('data', (td_u32*3)),
        ('data_len', (uint16_t*3)),
        ('data_cnt', uint8_t,2),
        ('need_free', uint8_t,1),
        ('single_task', uint8_t,1),
        ('critical', uint8_t,1),
        ('output_type', uint8_t,2),
        ('resvered', uint8_t,1),
    ]
class random_type_919(Structure):
    _fields_ = [
        ('put_msg_2_cache_fail_times', uint32_t),
        ('dfx_msg_q_num', uint16_t),
        ('dfx_msg_q_peak_num', uint16_t),
        ('transmit_msg_q_num', uint16_t),
        ('transmit_msg_q_peak_num', uint16_t),
        ('dfx_msg_process_max_time', uint32_t),
        ('transmit_msg_process_max_time', uint32_t),
        ('send_local_q_fail', uint32_t),
        ('send_local_q_success', uint32_t),
        ('transmit_msg_rev_cnt', uint32_t),
        ('msg_rev_cnt', uint32_t),
        ('diag_pkt_msg_rev_cnt', uint32_t),
        ('beat_heart_msg_rev_cnt', uint32_t),
        ('channel_receive_data_cnt', (uint32_t*4)),
        ('channel_receive_frame_cnt', (uint32_t*4)),
        ('mem_pkt_alloc_size', (uint32_t*2)),
        ('mem_pkt_free_size', (uint32_t*2)),
        ('alloc_mem_size', uint32_t),
        ('alloc_mem_peak_size', uint32_t),
        ('conn_excep_cnt', uint32_t),
        ('conn_bu_cnt', uint32_t),
    ]
class random_type_920(Structure):
    _fields_ = [
        ('name', td_u32),
        ('path', td_u32),
        ('file_size', uint32_t),
        ('cache_size', uint16_t),
        ('enable_cache', uint8_t),
        ('mult_files', uint8_t),
    ]
store_file_cfg_t=random_type_920
diag_channel_tx_hook=td_u32
diag_channel_notify_hook=td_u32
class random_type_921(Structure):
    _fields_ = [
        ('init', uint32_t,1),
        ('pad', uint32_t,31),
        ('name_flag', uint32_t),
        ('rx_buf', td_u32),
        ('au_id', uint32_t),
        ('rx_buf_len', uint16_t),
        ('rx_buf_pos', uint16_t),
        ('using_size', uint16_t),
        ('min_pkt_size', uint16_t),
        ('rx_buf_is_using', td_bool),
        ('hso_addr', uint8_t),
        ('mfs', uint16_t),
        ('max_serv_pkg_len', uint32_t),
        ('tx_hook', diag_channel_tx_hook),
        ('notify_hook', diag_channel_notify_hook),
    ]
diag_channel_item_t=random_type_921
class random_type_922(Structure):
    _fields_ = [
        ('data', (td_u32*3)),
        ('data_len', (uint16_t*3)),
        ('data_cnt', uint8_t,2),
        ('need_free', uint8_t,1),
        ('single_task', uint8_t,1),
        ('critical', uint8_t,1),
        ('output_type', uint8_t,2),
        ('resvered', uint8_t,1),
    ]
class random_type_923(Structure):
    _fields_ = [
        ('put_msg_2_cache_fail_times', uint32_t),
        ('dfx_msg_q_num', uint16_t),
        ('dfx_msg_q_peak_num', uint16_t),
        ('transmit_msg_q_num', uint16_t),
        ('transmit_msg_q_peak_num', uint16_t),
        ('dfx_msg_process_max_time', uint32_t),
        ('transmit_msg_process_max_time', uint32_t),
        ('send_local_q_fail', uint32_t),
        ('send_local_q_success', uint32_t),
        ('transmit_msg_rev_cnt', uint32_t),
        ('msg_rev_cnt', uint32_t),
        ('diag_pkt_msg_rev_cnt', uint32_t),
        ('beat_heart_msg_rev_cnt', uint32_t),
        ('channel_receive_data_cnt', (uint32_t*4)),
        ('channel_receive_frame_cnt', (uint32_t*4)),
        ('mem_pkt_alloc_size', (uint32_t*2)),
        ('mem_pkt_free_size', (uint32_t*2)),
        ('alloc_mem_size', uint32_t),
        ('alloc_mem_peak_size', uint32_t),
        ('conn_excep_cnt', uint32_t),
        ('conn_bu_cnt', uint32_t),
    ]
class random_type_924(Structure):
    _fields_ = [
        ('cur_proc', uint8_t),
        ('dst_addr', uint8_t),
        ('src_addr', uint8_t),
    ]
diag_pkt_process_param_t=random_type_924
class random_type_925(Structure):
    _fields_ = [
        ('sof', uint8_t),
        ('ctrl', uint8_t),
        ('inner', (uint8_t*0)),
    ]
class random_type_926(Structure):
    _fields_ = [
        ('en_crc', uint8_t,1),
        ('en_fid', uint8_t,1),
        ('en_sn', uint8_t,2),
        ('en_eof', uint8_t,1),
        ('en_full_crc', uint8_t,1),
        ('en_crypto', uint8_t,1),
        ('en_extend', uint8_t,1),
    ]
diag_router_ctrl_t=random_type_926
class random_type_927(Structure):
    _fields_ = [
        ('len_lsb', uint8_t),
        ('len_msb', uint8_t),
        ('payload', (uint8_t*0)),
    ]
class random_type_928(Structure):
    _fields_ = [
        ('put_msg_2_cache_fail_times', uint32_t),
        ('dfx_msg_q_num', uint16_t),
        ('dfx_msg_q_peak_num', uint16_t),
        ('transmit_msg_q_num', uint16_t),
        ('transmit_msg_q_peak_num', uint16_t),
        ('dfx_msg_process_max_time', uint32_t),
        ('transmit_msg_process_max_time', uint32_t),
        ('send_local_q_fail', uint32_t),
        ('send_local_q_success', uint32_t),
        ('transmit_msg_rev_cnt', uint32_t),
        ('msg_rev_cnt', uint32_t),
        ('diag_pkt_msg_rev_cnt', uint32_t),
        ('beat_heart_msg_rev_cnt', uint32_t),
        ('channel_receive_data_cnt', (uint32_t*4)),
        ('channel_receive_frame_cnt', (uint32_t*4)),
        ('mem_pkt_alloc_size', (uint32_t*2)),
        ('mem_pkt_free_size', (uint32_t*2)),
        ('alloc_mem_size', uint32_t),
        ('alloc_mem_peak_size', uint32_t),
        ('conn_excep_cnt', uint32_t),
        ('conn_bu_cnt', uint32_t),
    ]
class random_type_929(Structure):
    _fields_ = [
        ('init', uint32_t,1),
        ('pad', uint32_t,31),
        ('name_flag', uint32_t),
        ('rx_buf', td_u32),
        ('au_id', uint32_t),
        ('rx_buf_len', uint16_t),
        ('rx_buf_pos', uint16_t),
        ('using_size', uint16_t),
        ('min_pkt_size', uint16_t),
        ('rx_buf_is_using', td_bool),
        ('hso_addr', uint8_t),
        ('mfs', uint16_t),
        ('max_serv_pkg_len', uint32_t),
        ('tx_hook', diag_channel_tx_hook),
        ('notify_hook', diag_channel_notify_hook),
    ]
class random_type_930(Structure):
    _fields_ = [
        ('data', (td_u32*3)),
        ('data_len', (uint16_t*3)),
        ('data_cnt', uint8_t,2),
        ('need_free', uint8_t,1),
        ('single_task', uint8_t,1),
        ('critical', uint8_t,1),
        ('output_type', uint8_t,2),
        ('resvered', uint8_t,1),
    ]
class random_type_931(Structure):
    _fields_ = [
        ('cur_proc', uint8_t),
        ('dst_addr', uint8_t),
        ('src_addr', uint8_t),
    ]
class random_type_932(Structure):
    _fields_ = [
        ('item', (diag_channel_item_t*3)),
    ]
diag_channel_ctrl_t=random_type_932
class random_type_933(Structure):
    _fields_ = [
        ('init', uint32_t,1),
        ('pad', uint32_t,31),
        ('name_flag', uint32_t),
        ('rx_buf', td_u32),
        ('au_id', uint32_t),
        ('rx_buf_len', uint16_t),
        ('rx_buf_pos', uint16_t),
        ('using_size', uint16_t),
        ('min_pkt_size', uint16_t),
        ('rx_buf_is_using', td_bool),
        ('hso_addr', uint8_t),
        ('mfs', uint16_t),
        ('max_serv_pkg_len', uint32_t),
        ('tx_hook', diag_channel_tx_hook),
        ('notify_hook', diag_channel_notify_hook),
    ]
class random_type_934(Structure):
    _fields_ = [
        ('put_msg_2_cache_fail_times', uint32_t),
        ('dfx_msg_q_num', uint16_t),
        ('dfx_msg_q_peak_num', uint16_t),
        ('transmit_msg_q_num', uint16_t),
        ('transmit_msg_q_peak_num', uint16_t),
        ('dfx_msg_process_max_time', uint32_t),
        ('transmit_msg_process_max_time', uint32_t),
        ('send_local_q_fail', uint32_t),
        ('send_local_q_success', uint32_t),
        ('transmit_msg_rev_cnt', uint32_t),
        ('msg_rev_cnt', uint32_t),
        ('diag_pkt_msg_rev_cnt', uint32_t),
        ('beat_heart_msg_rev_cnt', uint32_t),
        ('channel_receive_data_cnt', (uint32_t*4)),
        ('channel_receive_frame_cnt', (uint32_t*4)),
        ('mem_pkt_alloc_size', (uint32_t*2)),
        ('mem_pkt_free_size', (uint32_t*2)),
        ('alloc_mem_size', uint32_t),
        ('alloc_mem_peak_size', uint32_t),
        ('conn_excep_cnt', uint32_t),
        ('conn_bu_cnt', uint32_t),
    ]
class random_type_936(Structure):
    _fields_ = [
        ('param', diag_pkt_process_param_t),
        ('pkt', diag_pkt_handle_t),
    ]
diag_pkt_msg_t=random_type_936
class random_type_937(Structure):
    _fields_ = [
        ('cur_proc', uint8_t),
        ('dst_addr', uint8_t),
        ('src_addr', uint8_t),
    ]
class random_type_938(Structure):
    _fields_ = [
        ('data', (td_u32*3)),
        ('data_len', (uint16_t*3)),
        ('data_cnt', uint8_t,2),
        ('need_free', uint8_t,1),
        ('single_task', uint8_t,1),
        ('critical', uint8_t,1),
        ('output_type', uint8_t,2),
        ('resvered', uint8_t,1),
    ]
class random_type_939(Structure):
    _fields_ = [
        ('sof', uint8_t),
        ('ctrl', uint8_t),
        ('inner', (uint8_t*0)),
    ]
class random_type_940(Structure):
    _fields_ = [
        ('en_crc', uint8_t,1),
        ('en_fid', uint8_t,1),
        ('en_sn', uint8_t,2),
        ('en_eof', uint8_t,1),
        ('en_full_crc', uint8_t,1),
        ('en_crypto', uint8_t,1),
        ('en_extend', uint8_t,1),
    ]
class random_type_941(Structure):
    _fields_ = [
        ('ack_type', uint8_t,2),
        ('reserved', uint8_t,6),
    ]
diag_router_ctrl2_t=random_type_941
class random_type_942(Structure):
    _fields_ = [
        ('module_id', uint8_t),
        ('cmd_id', uint8_t),
        ('tlv', (uint8_t*0)),
    ]
class random_type_943(Structure):
    _fields_ = [
        ('len_lsb', uint8_t),
        ('len_msb', uint8_t),
        ('payload', (uint8_t*0)),
    ]
class random_type_944(Structure):
    _fields_ = [
        ('put_msg_2_cache_fail_times', uint32_t),
        ('dfx_msg_q_num', uint16_t),
        ('dfx_msg_q_peak_num', uint16_t),
        ('transmit_msg_q_num', uint16_t),
        ('transmit_msg_q_peak_num', uint16_t),
        ('dfx_msg_process_max_time', uint32_t),
        ('transmit_msg_process_max_time', uint32_t),
        ('send_local_q_fail', uint32_t),
        ('send_local_q_success', uint32_t),
        ('transmit_msg_rev_cnt', uint32_t),
        ('msg_rev_cnt', uint32_t),
        ('diag_pkt_msg_rev_cnt', uint32_t),
        ('beat_heart_msg_rev_cnt', uint32_t),
        ('channel_receive_data_cnt', (uint32_t*4)),
        ('channel_receive_frame_cnt', (uint32_t*4)),
        ('mem_pkt_alloc_size', (uint32_t*2)),
        ('mem_pkt_free_size', (uint32_t*2)),
        ('alloc_mem_size', uint32_t),
        ('alloc_mem_peak_size', uint32_t),
        ('conn_excep_cnt', uint32_t),
        ('conn_bu_cnt', uint32_t),
    ]
class random_type_945(Structure):
    _fields_ = [
        ('ctrl', diag_router_ctrl_t),
        ('ctrl2', diag_router_ctrl2_t),
        ('fid', uint8_t),
        ('sn_count', uint8_t),
        ('echo', uint8_t),
        ('mfs', uint16_t),
        ('data_len', uint16_t),
        ('data', td_u32),
    ]
diag_router_data_t=random_type_945
class random_type_946(Structure):
    _fields_ = [
        ('put_msg_2_cache_fail_times', uint32_t),
        ('dfx_msg_q_num', uint16_t),
        ('dfx_msg_q_peak_num', uint16_t),
        ('transmit_msg_q_num', uint16_t),
        ('transmit_msg_q_peak_num', uint16_t),
        ('dfx_msg_process_max_time', uint32_t),
        ('transmit_msg_process_max_time', uint32_t),
        ('send_local_q_fail', uint32_t),
        ('send_local_q_success', uint32_t),
        ('transmit_msg_rev_cnt', uint32_t),
        ('msg_rev_cnt', uint32_t),
        ('diag_pkt_msg_rev_cnt', uint32_t),
        ('beat_heart_msg_rev_cnt', uint32_t),
        ('channel_receive_data_cnt', (uint32_t*4)),
        ('channel_receive_frame_cnt', (uint32_t*4)),
        ('mem_pkt_alloc_size', (uint32_t*2)),
        ('mem_pkt_free_size', (uint32_t*2)),
        ('alloc_mem_size', uint32_t),
        ('alloc_mem_peak_size', uint32_t),
        ('conn_excep_cnt', uint32_t),
        ('conn_bu_cnt', uint32_t),
    ]
class random_type_947(Structure):
    _fields_ = [
        ('peer_addr', diag_addr),
        ('pad', (uint8_t*3)),
    ]
class random_type_948(Structure):
    _fields_ = [
        ('case_id', uint32_t),
        ('data', (uint32_t*3)),
    ]
diag_dfx_cmd_ind_st_t=random_type_948
class random_type_949(Structure):
    _fields_ = [
        ('case_id', uint32_t),
        ('data', (uint32_t*3)),
    ]
diag_dfx_cmd_req_st_t=random_type_949
class random_type_950(Structure):
    _fields_ = [
        ('id', uint16_t),
        ('array_cnt', uint16_t),
        ('stat_packet_size', uint32_t),
        ('stat_packet', td_u32),
    ]
diag_sys_stat_obj_t=random_type_950
class random_type_951(Structure):
    _fields_ = [
        ('stat_cmd_list', (td_u32*10)),
        ('aus_stat_cmd_num', (uint16_t*10)),
    ]
zdiag_stat_ctrl_t=random_type_951
class random_type_952(Structure):
    _fields_ = [
        ('id', uint16_t),
        ('array_cnt', uint16_t),
        ('stat_packet_size', uint32_t),
        ('stat_packet', td_u32),
    ]
class random_type_953(Structure):
    _fields_ = [
        ('peer_addr', diag_addr),
        ('pad', (uint8_t*3)),
    ]
class random_type_954(Structure):
    _fields_ = [
        ('obj_id', uint16_t),
        ('object', td_u32),
        ('obj_size', uint16_t),
        ('obj_cnt', uint16_t),
        ('option', td_u32),
        ('sync', td_bool),
        ('pad', (uint8_t*2)),
    ]
class random_type_955(Structure):
    _fields_ = [
        ('enable', td_bool),
        ('level', uint8_t),
        ('tool_addr', diag_addr),
        ('last_rev_time', uint32_t),
        ('enable_id', (uint16_t*10)),
    ]
zdiag_filter_ctrl_t=random_type_955
zdiag_filter_notify_hook=td_u32
diag_notify_f=td_u32
class random_type_958(Structure):
    _fields_ = [
        ('header', diag_ser_header_t),
        ('payload', (uint8_t*0)),
    ]
class random_type_959(Structure):
    _fields_ = [
        ('ser_id', uint8_t),
        ('cmd_id', uint8_t),
        ('src', uint8_t),
        ('dst', uint8_t),
        ('crc_en', td_bool),
        ('ack_en', td_bool),
        ('length', uint16_t),
    ]
class random_type_960(Structure):
    _fields_ = [
        ('module_id', uint8_t),
        ('cmd_id', uint8_t),
        ('tlv', (uint8_t*0)),
    ]
class random_type_961(Structure):
    _fields_ = [
        ('len_lsb', uint8_t),
        ('len_msb', uint8_t),
        ('payload', (uint8_t*0)),
    ]
class random_type_962(Structure):
    _fields_ = [
        ('sof', uint8_t),
        ('ctrl', uint8_t),
        ('inner', (uint8_t*0)),
    ]
class random_type_963(Structure):
    _fields_ = [
        ('init', uint32_t,1),
        ('pad', uint32_t,31),
        ('name_flag', uint32_t),
        ('rx_buf', td_u32),
        ('au_id', uint32_t),
        ('rx_buf_len', uint16_t),
        ('rx_buf_pos', uint16_t),
        ('using_size', uint16_t),
        ('min_pkt_size', uint16_t),
        ('rx_buf_is_using', td_bool),
        ('hso_addr', uint8_t),
        ('mfs', uint16_t),
        ('max_serv_pkg_len', uint32_t),
        ('tx_hook', diag_channel_tx_hook),
        ('notify_hook', diag_channel_notify_hook),
    ]
class random_type_964(Structure):
    _fields_ = [
        ('put_msg_2_cache_fail_times', uint32_t),
        ('dfx_msg_q_num', uint16_t),
        ('dfx_msg_q_peak_num', uint16_t),
        ('transmit_msg_q_num', uint16_t),
        ('transmit_msg_q_peak_num', uint16_t),
        ('dfx_msg_process_max_time', uint32_t),
        ('transmit_msg_process_max_time', uint32_t),
        ('send_local_q_fail', uint32_t),
        ('send_local_q_success', uint32_t),
        ('transmit_msg_rev_cnt', uint32_t),
        ('msg_rev_cnt', uint32_t),
        ('diag_pkt_msg_rev_cnt', uint32_t),
        ('beat_heart_msg_rev_cnt', uint32_t),
        ('channel_receive_data_cnt', (uint32_t*4)),
        ('channel_receive_frame_cnt', (uint32_t*4)),
        ('mem_pkt_alloc_size', (uint32_t*2)),
        ('mem_pkt_free_size', (uint32_t*2)),
        ('alloc_mem_size', uint32_t),
        ('alloc_mem_peak_size', uint32_t),
        ('conn_excep_cnt', uint32_t),
        ('conn_bu_cnt', uint32_t),
    ]
class random_type_965(Structure):
    _fields_ = [
        ('en_crc', uint8_t,1),
        ('en_fid', uint8_t,1),
        ('en_sn', uint8_t,2),
        ('en_eof', uint8_t,1),
        ('en_full_crc', uint8_t,1),
        ('en_crypto', uint8_t,1),
        ('en_extend', uint8_t,1),
    ]
class random_type_966(Structure):
    _fields_ = [
        ('ack_type', uint8_t,2),
        ('reserved', uint8_t,6),
    ]
class random_type_967(Structure):
    _fields_ = [
        ('ctrl', diag_router_ctrl_t),
        ('ctrl2', diag_router_ctrl2_t),
        ('fid', uint8_t),
        ('sn_count', uint8_t),
        ('echo', uint8_t),
        ('mfs', uint16_t),
        ('data_len', uint16_t),
        ('data', td_u32),
    ]
class random_type_968(Structure):
    _fields_ = [
        ('header', diag_ser_header_t),
        ('payload', (uint8_t*0)),
    ]
class random_type_969(Structure):
    _fields_ = [
        ('ser_id', uint8_t),
        ('cmd_id', uint8_t),
        ('src', uint8_t),
        ('dst', uint8_t),
        ('crc_en', td_bool),
        ('ack_en', td_bool),
        ('length', uint16_t),
    ]
class random_type_970(Structure):
    _fields_ = [
        ('peer_addr', diag_addr),
        ('pad', (uint8_t*3)),
    ]
class random_type_971(Structure):
    _fields_ = [
        ('user_cmd_list', (td_u32*10)),
        ('aus_usert_cmd_num', (uint16_t*10)),
    ]
diag_cmd_ctrl_t=random_type_971
class random_type_972(Structure):
    _fields_ = [
        ('min_id', uint16_t),
        ('max_id', uint16_t),
        ('fn_input_cmd', diag_cmd_f),
    ]
class random_type_973(Structure):
    _fields_ = [
        ('peer_addr', diag_addr),
        ('pad', (uint8_t*3)),
    ]
class random_type_974(Structure):
    _fields_ = [
        ('mod_id', uint8_t),
        ('cmd_id', uint8_t),
        ('param', (uint8_t*0)),
    ]
class random_type_975(Structure):
    _fields_ = [
        ('header', diag_ser_header_t),
        ('payload', (uint8_t*0)),
    ]
class random_type_976(Structure):
    _fields_ = [
        ('ser_id', uint8_t),
        ('cmd_id', uint8_t),
        ('src', uint8_t),
        ('dst', uint8_t),
        ('crc_en', td_bool),
        ('ack_en', td_bool),
        ('length', uint16_t),
    ]
class random_type_977(Structure):
    _fields_ = [
        ('user_cmd_list', (td_u32*3)),
        ('aus_usert_cmd_num', (uint16_t*3)),
    ]
diag_ind_ctrl_t=random_type_977
class random_type_978(Structure):
    _fields_ = [
        ('min_id', uint16_t),
        ('max_id', uint16_t),
        ('fn_input_cmd', diag_cmd_f),
    ]
class random_type_979(Structure):
    _fields_ = [
        ('peer_addr', diag_addr),
        ('pad', (uint8_t*3)),
    ]
class random_type_980(Structure):
    _fields_ = [
        ('mod_id', uint8_t),
        ('cmd_id', uint8_t),
        ('param', (uint8_t*0)),
    ]
class random_type_981(Structure):
    _fields_ = [
        ('header', diag_ser_header_t),
        ('payload', (uint8_t*0)),
    ]
class random_type_982(Structure):
    _fields_ = [
        ('ser_id', uint8_t),
        ('cmd_id', uint8_t),
        ('src', uint8_t),
        ('dst', uint8_t),
        ('crc_en', td_bool),
        ('ack_en', td_bool),
        ('length', uint16_t),
    ]
diag_msg_flow_control_hook=td_u32
class random_type_983(Structure):
    _fields_ = [
        ('sof', uint8_t),
        ('ctrl', uint8_t),
        ('inner', (uint8_t*0)),
    ]
class random_type_984(Structure):
    _fields_ = [
        ('mod_id', uint8_t),
        ('cmd_id', uint8_t),
        ('param', (uint8_t*0)),
    ]
class random_type_985(Structure):
    _fields_ = [
        ('module', uint32_t),
        ('dest_mod', uint32_t),
        ('no', uint32_t),
        ('id', uint32_t),
        ('time', uint32_t),
        ('data', (int8_t*0)),
    ]
class random_type_986(Structure):
    _fields_ = [
        ('data', (td_u32*3)),
        ('data_len', (uint16_t*3)),
        ('data_cnt', uint8_t,2),
        ('need_free', uint8_t,1),
        ('single_task', uint8_t,1),
        ('critical', uint8_t,1),
        ('output_type', uint8_t,2),
        ('resvered', uint8_t,1),
    ]
class random_type_987(Structure):
    _fields_ = [
        ('cur_proc', uint8_t),
        ('dst_addr', uint8_t),
        ('src_addr', uint8_t),
    ]
class random_type_988(Structure):
    _fields_ = [
        ('peer_addr', diag_addr),
        ('pad', (uint8_t*3)),
    ]
class random_type_989(Structure):
    _fields_ = [
        ('packet', td_u32),
        ('packet_size', td_u32),
        ('pkt_cnt', uint8_t),
    ]
diag_report_packet=random_type_989
class random_type_990(Structure):
    _fields_ = [
        ('packet', td_u32),
        ('packet_size', uint16_t),
    ]
diag_report_sys_msg_packet=random_type_990
class random_type_991(Structure):
    _fields_ = [
        ('dir_len', uint32_t),
        ('path', (uint8_t*0)),
    ]
diag_ls_cmd_t=random_type_991
class random_type_992(Structure):
    _fields_ = [
        ('type', uint8_t),
        ('len', uint8_t),
        ('data', (uint8_t*0)),
    ]
transmit_pkt_tlv_t=random_type_992
class random_type_993(Structure):
    _fields_ = [
        ('cmd_id', uint16_t),
        ('option', diag_option_t),
        ('idx', uint32_t),
    ]
usr_data_param_t=random_type_993
class random_type_994(Structure):
    _fields_ = [
        ('peer_addr', diag_addr),
        ('pad', (uint8_t*3)),
    ]
class random_type_995(Structure):
    _fields_ = [
        ('idx', uint16_t),
        ('path_len', uint8_t),
        ('file_type', uint8_t),
        ('file_size', uint32_t),
        ('file_name', (uint8_t*0)),
    ]
diag_ls_ind_t=random_type_995
class random_type_996(Structure):
    _fields_ = [
        ('is_dir', td_bool),
        ('name', td_u32),
        ('file_size', uint32_t),
    ]
class transmit_item_t(Structure):
    _fields_ = [
        ('transmit_id', uint32_t),
        ('used', uint32_t,1),
        ('init_fail', uint32_t,1),
        ('permanent', uint32_t,1),
        ('local_start', uint32_t,1),
        ('local_src', uint32_t,1),
        ('remote_type', uint32_t,4),
        ('local_type', uint32_t,4),
        ('down_machine', uint32_t,1),
        ('enable', uint32_t,1),
        ('step', uint32_t,2),
        ('re_trans', uint32_t,1),
        ('pm_veto', uint32_t,1),
        ('resvered', uint32_t,13),
        ('data_block_number', uint16_t),
        ('data_block_size', uint16_t),
        ('received_size', uint32_t),
        ('request_size', uint32_t),
        ('total_size', uint32_t),
        ('write_read', uintptr_t),
        ('usr_wr_data', uintptr_t),
        ('bus_addr', uint32_t),
        ('file_name', td_u32),
        ('file_fd', int32_t),
        ('expiration', uint32_t),
        ('last_rcv_pkt_time', uint32_t),
        ('last_send_pkt_time', uint32_t),
        ('option', diag_option_t),
        ('result_hook', uintptr_t),
    ]
class random_type_997(Structure):
    _fields_ = [
        ('dir_len', uint16_t),
        ('rev', uint8_t),
        ('file_type', uint8_t),
        ('name', (uint8_t*0)),
    ]
diag_del_cmd_t=random_type_997
class random_type_998(Structure):
    _fields_ = [
        ('ret_value', uint32_t),
    ]
diag_del_ind_t=random_type_998
class random_type_999(Structure):
    _fields_ = [
        ('type', uint8_t),
        ('len', uint8_t),
        ('data', (uint8_t*0)),
    ]
class random_type_1000(Structure):
    _fields_ = [
        ('peer_addr', diag_addr),
        ('pad', (uint8_t*3)),
    ]
transmit_pkt_recv_hook=td_u32
class random_type_1001(Structure):
    _fields_ = [
        ('cmd_id', uint8_t),
        ('handler', transmit_pkt_recv_hook),
    ]
transmit_cmd_ind_item_t=random_type_1001
class random_type_1002(Structure):
    _fields_ = [
        ('peer_addr', diag_addr),
        ('pad', (uint8_t*3)),
    ]
class random_type_1003(Structure):
    _fields_ = [
        ('module_id', uint8_t),
        ('cmd_id', uint8_t),
        ('tlv', (uint8_t*0)),
    ]
class random_type_1004(Structure):
    _fields_ = [
        ('header', diag_ser_header_t),
        ('payload', (uint8_t*0)),
    ]
class random_type_1005(Structure):
    _fields_ = [
        ('ser_id', uint8_t),
        ('cmd_id', uint8_t),
        ('src', uint8_t),
        ('dst', uint8_t),
        ('crc_en', td_bool),
        ('ack_en', td_bool),
        ('length', uint16_t),
    ]
class random_type_1006(Structure):
    _fields_ = [
        ('transmit_id', uint32_t),
        ('src_send', uint16_t,1),
        ('struct_ver', uint16_t,2),
        ('re_trans', uint16_t,1),
        ('bitmap_enable', uint16_t,1),
        ('pad', uint16_t,11),
        ('transmit_type', uint16_t),
        ('total_size', uint32_t),
        ('info_size', uint32_t),
        ('info', (uint8_t*0)),
    ]
transmit_start_pkt_t=random_type_1006
class random_type_1007(Structure):
    _fields_ = [
        ('transmit_id', uint32_t),
        ('src_send', uint16_t,1),
        ('struct_ver', uint16_t,2),
        ('re_trans', uint16_t,1),
        ('bitmap_enable', uint16_t,1),
        ('pad', uint16_t,11),
        ('transmit_type', uint16_t),
        ('total_size', uint32_t),
        ('data_block_number', uint16_t),
        ('data_block_size', uint16_t),
        ('info_size', uint32_t),
        ('info', (uint8_t*0)),
    ]
transmit_negotiate_pkt_t=random_type_1007
class random_type_1009(Structure):
    _fields_ = [
        ('offset', uint32_t),
        ('size', uint32_t),
    ]
transmit_data_request_item_t=random_type_1009
class random_type_1008(Structure):
    _fields_ = [
        ('transmit_id', uint32_t),
        ('cnt', uint32_t),
        ('item', (transmit_data_request_item_t*0)),
    ]
transmit_data_request_pkt_t=random_type_1008
class random_type_1010(Structure):
    _fields_ = [
        ('transmit_id', uint32_t),
        ('offset', uint32_t),
        ('size', uint32_t),
        ('data', (uint8_t*0)),
    ]
transmit_data_reply_pkt_t=random_type_1010
class random_type_1011(Structure):
    _fields_ = [
        ('transmit_id', uint32_t),
        ('state_code', uint32_t),
        ('len', uint32_t),
        ('data', (uint8_t*0)),
    ]
transmit_state_notify_pkt_t=random_type_1011
class random_type_1012(Structure):
    _fields_ = [
        ('transmit_id', uint32_t),
        ('reason', uint32_t),
    ]
transmit_stop_pkt_t=random_type_1012
class random_type_1013(Structure):
    _fields_ = [
        ('ser_id', uint8_t),
        ('cmd_id', uint8_t),
        ('src', uint8_t),
        ('dst', uint8_t),
        ('crc_en', td_bool),
        ('ack_en', td_bool),
        ('length', uint16_t),
    ]
class random_type_1014(Structure):
    _fields_ = [
        ('module_id', uint8_t),
        ('cmd_id', uint8_t),
        ('tlv', (uint8_t*0)),
    ]
class random_type_1015(Structure):
    _fields_ = [
        ('header', diag_ser_header_t),
        ('payload', (uint8_t*0)),
    ]
class random_type_1016(Structure):
    _fields_ = [
        ('type', uint8_t),
        ('len', uint8_t),
        ('data', (uint8_t*0)),
    ]
class random_type_1017(Structure):
    _fields_ = [
        ('peer_addr', diag_addr),
        ('pad', (uint8_t*3)),
    ]
class random_type_1018(Structure):
    _fields_ = [
        ('put_msg_2_cache_fail_times', uint32_t),
        ('dfx_msg_q_num', uint16_t),
        ('dfx_msg_q_peak_num', uint16_t),
        ('transmit_msg_q_num', uint16_t),
        ('transmit_msg_q_peak_num', uint16_t),
        ('dfx_msg_process_max_time', uint32_t),
        ('transmit_msg_process_max_time', uint32_t),
        ('send_local_q_fail', uint32_t),
        ('send_local_q_success', uint32_t),
        ('transmit_msg_rev_cnt', uint32_t),
        ('msg_rev_cnt', uint32_t),
        ('diag_pkt_msg_rev_cnt', uint32_t),
        ('beat_heart_msg_rev_cnt', uint32_t),
        ('channel_receive_data_cnt', (uint32_t*4)),
        ('channel_receive_frame_cnt', (uint32_t*4)),
        ('mem_pkt_alloc_size', (uint32_t*2)),
        ('mem_pkt_free_size', (uint32_t*2)),
        ('alloc_mem_size', uint32_t),
        ('alloc_mem_peak_size', uint32_t),
        ('conn_excep_cnt', uint32_t),
        ('conn_bu_cnt', uint32_t),
    ]
class random_type_1019(Structure):
    _fields_ = [
        ('is_busy', td_u32),
        ('transmit_id', uint32_t),
        ('used_cnt', uint8_t),
        ('period_cnt', uint8_t),
        ('buf_used', uint32_t,1),
        ('pkt_size', uint32_t),
        ('pkt_buf', td_u32),
        ('timer_cnt', uint8_t),
        ('timer', osal_timer),
        ('item', (transmit_item_t*3)),
    ]
transmit_ctrl_t=random_type_1019
class random_type_1020(Structure):
    _fields_ = [
        ('timer', td_u32),
        ('handler', td_u32),
        ('data', td_u32),
        ('interval', td_u32),
    ]
class random_type_1021(Structure):
    _fields_ = [
        ('peer_addr', diag_addr),
        ('pad', (uint8_t*3)),
    ]
class random_type_1022(Structure):
    _fields_ = [
        ('g_transmit_timer', osal_timer),
        ('start', td_bool),
    ]
transmit_timer_t=random_type_1022
class random_type_1023(Structure):
    _fields_ = [
        ('id_start', uint32_t),
        ('id_end', uint32_t),
        ('used', uint32_t),
        ('hook', uintptr_t),
    ]
transmit_msg_proc_t=random_type_1023
class random_type_1024(Structure):
    _fields_ = [
        ('transmit_id', uint32_t),
        ('state_code', uint32_t),
        ('len', uint32_t),
        ('data', (uint8_t*0)),
    ]
transmit_result_hook=td_u32
transmit_register_hook=td_u32
class random_type_1025(Structure):
    _fields_ = [
        ('put_msg_2_cache_fail_times', uint32_t),
        ('dfx_msg_q_num', uint16_t),
        ('dfx_msg_q_peak_num', uint16_t),
        ('transmit_msg_q_num', uint16_t),
        ('transmit_msg_q_peak_num', uint16_t),
        ('dfx_msg_process_max_time', uint32_t),
        ('transmit_msg_process_max_time', uint32_t),
        ('send_local_q_fail', uint32_t),
        ('send_local_q_success', uint32_t),
        ('transmit_msg_rev_cnt', uint32_t),
        ('msg_rev_cnt', uint32_t),
        ('diag_pkt_msg_rev_cnt', uint32_t),
        ('beat_heart_msg_rev_cnt', uint32_t),
        ('channel_receive_data_cnt', (uint32_t*4)),
        ('channel_receive_frame_cnt', (uint32_t*4)),
        ('mem_pkt_alloc_size', (uint32_t*2)),
        ('mem_pkt_free_size', (uint32_t*2)),
        ('alloc_mem_size', uint32_t),
        ('alloc_mem_peak_size', uint32_t),
        ('conn_excep_cnt', uint32_t),
        ('conn_bu_cnt', uint32_t),
    ]
class random_type_1027(Structure):
    _fields_ = [
        ('type', uint8_t),
        ('len', uint8_t),
        ('data', (uint8_t*0)),
    ]
transmit_write_hook=td_u32
transmit_read_hook=td_u32
class random_type_1028(Structure):
    _fields_ = [
        ('name_size', uint16_t),
        ('pad', uint16_t),
        ('file_name', (td_char*0)),
    ]
transmit_save_info_t=random_type_1028
class random_type_1029(Structure):
    _fields_ = [
        ('transmit_id', uint32_t),
        ('reason', uint32_t),
    ]
class random_type_1030(Structure):
    _fields_ = [
        ('transmit_id', uint32_t),
        ('cnt', uint32_t),
        ('item', (transmit_data_request_item_t*0)),
    ]
class random_type_1031(Structure):
    _fields_ = [
        ('offset', uint32_t),
        ('size', uint32_t),
    ]
class random_type_1033(Structure):
    _fields_ = [
        ('peer_addr', diag_addr),
        ('pad', (uint8_t*3)),
    ]
class random_type_1034(Structure):
    _fields_ = [
        ('transmit_id', uint32_t),
        ('offset', uint32_t),
        ('size', uint32_t),
        ('data', (uint8_t*0)),
    ]
class random_type_1035(Structure):
    _fields_ = [
        ('transmit_id', uint32_t),
        ('src_send', uint16_t,1),
        ('struct_ver', uint16_t,2),
        ('re_trans', uint16_t,1),
        ('bitmap_enable', uint16_t,1),
        ('pad', uint16_t,11),
        ('transmit_type', uint16_t),
        ('total_size', uint32_t),
        ('info_size', uint32_t),
        ('info', (uint8_t*0)),
    ]
class random_type_1036(Structure):
    _fields_ = [
        ('transmit_id', uint32_t),
        ('src_send', uint16_t,1),
        ('struct_ver', uint16_t,2),
        ('re_trans', uint16_t,1),
        ('bitmap_enable', uint16_t,1),
        ('pad', uint16_t,11),
        ('transmit_type', uint16_t),
        ('total_size', uint32_t),
        ('data_block_number', uint16_t),
        ('data_block_size', uint16_t),
        ('info_size', uint32_t),
        ('info', (uint8_t*0)),
    ]
class random_type_1037(Structure):
    _fields_ = [
        ('type', uint8_t),
        ('len', uint8_t),
        ('data', (uint8_t*0)),
    ]
class random_type_1038(Structure):
    _fields_ = [
        ('name_size', uint16_t),
        ('pad', uint16_t),
        ('file_name', (td_char*0)),
    ]
class random_type_1039(Structure):
    _fields_ = [
        ('start_addr', uint32_t),
        ('size', uint32_t),
    ]
transmit_save_data_info_t=random_type_1039
class random_type_1040(Structure):
    _fields_ = [
        ('transmit_id', uint32_t),
        ('reason', uint32_t),
    ]
class random_type_1041(Structure):
    _fields_ = [
        ('transmit_id', uint32_t),
        ('offset', uint32_t),
        ('size', uint32_t),
        ('data', (uint8_t*0)),
    ]
class random_type_1042(Structure):
    _fields_ = [
        ('transmit_id', uint32_t),
        ('src_send', uint16_t,1),
        ('struct_ver', uint16_t,2),
        ('re_trans', uint16_t,1),
        ('bitmap_enable', uint16_t,1),
        ('pad', uint16_t,11),
        ('transmit_type', uint16_t),
        ('total_size', uint32_t),
        ('data_block_number', uint16_t),
        ('data_block_size', uint16_t),
        ('info_size', uint32_t),
        ('info', (uint8_t*0)),
    ]
class random_type_1043(Structure):
    _fields_ = [
        ('transmit_id', uint32_t),
        ('data_block_number', uint16_t),
        ('data_block_size', uint16_t),
        ('info_size', uint32_t),
        ('info', (uint8_t*0)),
    ]
transmit_negotiate_ack_pkt_t=random_type_1043
class random_type_1044(Structure):
    _fields_ = [
        ('transmit_id', uint32_t),
        ('src_send', uint16_t,1),
        ('struct_ver', uint16_t,2),
        ('re_trans', uint16_t,1),
        ('bitmap_enable', uint16_t,1),
        ('pad', uint16_t,11),
        ('transmit_type', uint16_t),
        ('total_size', uint32_t),
        ('info_size', uint32_t),
        ('info', (uint8_t*0)),
    ]
class random_type_1046(Structure):
    _fields_ = [
        ('peer_addr', diag_addr),
        ('pad', (uint8_t*3)),
    ]
class random_type_1047(Structure):
    _fields_ = [
        ('type', uint8_t),
        ('len', uint8_t),
        ('data', (uint8_t*0)),
    ]
class random_type_1048(Structure):
    _fields_ = [
        ('transmit_id', uint32_t),
        ('cnt', uint32_t),
        ('item', (transmit_data_request_item_t*0)),
    ]
class random_type_1049(Structure):
    _fields_ = [
        ('offset', uint32_t),
        ('size', uint32_t),
    ]
class random_type_1050(Structure):
    _fields_ = [
        ('peer_addr', diag_addr),
        ('pad', (uint8_t*3)),
    ]
class random_type_1051(Structure):
    _fields_ = [
        ('uwTotalUsedSize', UINT32),
        ('uwTotalFreeSize', UINT32),
        ('uwMaxFreeNodeSize', UINT32),
        ('uwUsedNodeNum', UINT32),
        ('uwFreeNodeNum', UINT32),
        ('uwUsageWaterLine', UINT32),
    ]
class random_type_1052(Structure):
    _fields_ = [
        ('total', uint32_t),
        ('used', uint32_t),
        ('free', uint32_t),
        ('free_node_num', uint32_t),
        ('used_node_num', uint32_t),
        ('max_free_node_size', uint32_t),
        ('peek_size', uint32_t),
    ]
class random_type_1053(Structure):
    _fields_ = [
        ('name', (td_char*32)),
        ('valid', td_bool),
        ('id', uint32_t),
        ('status', uint16_t),
        ('priority', uint16_t),
        ('task_sem', td_u32),
        ('task_mutex', td_u32),
        ('event_stru', (uint32_t*3)),
        ('event_mask', uint32_t),
        ('stack_size', uint32_t),
        ('top_of_stack', uint32_t),
        ('bottom_of_stack', uint32_t),
        ('sp', uint32_t),
        ('curr_used', uint32_t),
        ('peak_used', uint32_t),
        ('overflow_flag', uint32_t),
    ]
class random_type_1054(Structure):
    _fields_ = [
        ('put_msg_2_cache_fail_times', uint32_t),
        ('dfx_msg_q_num', uint16_t),
        ('dfx_msg_q_peak_num', uint16_t),
        ('transmit_msg_q_num', uint16_t),
        ('transmit_msg_q_peak_num', uint16_t),
        ('dfx_msg_process_max_time', uint32_t),
        ('transmit_msg_process_max_time', uint32_t),
        ('send_local_q_fail', uint32_t),
        ('send_local_q_success', uint32_t),
        ('transmit_msg_rev_cnt', uint32_t),
        ('msg_rev_cnt', uint32_t),
        ('diag_pkt_msg_rev_cnt', uint32_t),
        ('beat_heart_msg_rev_cnt', uint32_t),
        ('channel_receive_data_cnt', (uint32_t*4)),
        ('channel_receive_frame_cnt', (uint32_t*4)),
        ('mem_pkt_alloc_size', (uint32_t*2)),
        ('mem_pkt_free_size', (uint32_t*2)),
        ('alloc_mem_size', uint32_t),
        ('alloc_mem_peak_size', uint32_t),
        ('conn_excep_cnt', uint32_t),
        ('conn_bu_cnt', uint32_t),
    ]
class random_type_1055(Structure):
    _fields_ = [
        ('type', uint8_t),
        ('level', uint8_t),
        ('module_id', uint16_t),
        ('fn_name', td_u32),
        ('line_num', uint32_t),
    ]
soc_log_param_t=random_type_1055
dfx_soc_log_output_handler=td_u32
class random_type_1056(Structure):
    _fields_ = [
        ('type', uint8_t),
        ('level', uint8_t),
        ('module_id', uint16_t),
        ('fn_name', td_u32),
        ('line_num', uint32_t),
    ]
class random_type_1057(Structure):
    _fields_ = [
        ('put_msg_2_cache_fail_times', uint32_t),
        ('dfx_msg_q_num', uint16_t),
        ('dfx_msg_q_peak_num', uint16_t),
        ('transmit_msg_q_num', uint16_t),
        ('transmit_msg_q_peak_num', uint16_t),
        ('dfx_msg_process_max_time', uint32_t),
        ('transmit_msg_process_max_time', uint32_t),
        ('send_local_q_fail', uint32_t),
        ('send_local_q_success', uint32_t),
        ('transmit_msg_rev_cnt', uint32_t),
        ('msg_rev_cnt', uint32_t),
        ('diag_pkt_msg_rev_cnt', uint32_t),
        ('beat_heart_msg_rev_cnt', uint32_t),
        ('channel_receive_data_cnt', (uint32_t*4)),
        ('channel_receive_frame_cnt', (uint32_t*4)),
        ('mem_pkt_alloc_size', (uint32_t*2)),
        ('mem_pkt_free_size', (uint32_t*2)),
        ('alloc_mem_size', uint32_t),
        ('alloc_mem_peak_size', uint32_t),
        ('conn_excep_cnt', uint32_t),
        ('conn_bu_cnt', uint32_t),
    ]
class random_type_1064(Structure):
    _fields_ = [
        ('start_flag', uint32_t),
        ('version', uint32_t),
        ('update_time', uint32_t),
        ('cur_pos', uint32_t),
        ('file_size', uint32_t),
        ('first_record_pos', uint32_t),
        ('records', uint32_t),
        ('service_type', uint8_t),
        ('offset', uint8_t),
        ('crc', uint16_t),
    ]
store_file_head_t=random_type_1064
class random_type_1059(Structure):
    _fields_ = [
        ('timer', td_u32),
        ('handler', td_u32),
        ('data', td_u32),
        ('interval', td_u32),
    ]
class random_type_1060(Structure):
    _fields_ = [
        ('task', td_u32),
    ]
class random_type_1061(Structure):
    _fields_ = [
        ('event', td_u32),
    ]
class random_type_1062(Structure):
    _fields_ = [
        ('mutex', td_u32),
    ]
class random_type_1065(Structure):
    _fields_ = [
        ('name', td_u32),
        ('path', td_u32),
        ('file_size', uint32_t),
        ('cache_size', uint16_t),
        ('enable_cache', uint8_t),
        ('mult_files', uint8_t),
    ]
class random_type_1066(Structure):
    _fields_ = [
        ('oldest_file_idx', uint32_t),
        ('cur_file_idx', uint32_t),
        ('file_count', uint32_t),
    ]
store_muti_file_idx_t=random_type_1066
class random_type_1067(Structure):
    _fields_ = [
        ('cache_write_pos', uint32_t),
        ('cache_read_pos', uint32_t),
        ('cache_size', uint32_t),
        ('threshold_size', uint32_t),
        ('data', (td_char*0)),
    ]
store_cache_t=random_type_1067
class random_type_1068(Structure):
    _fields_ = [
        ('magic', uint16_t),
        ('len', uint16_t),
        ('type', uint8_t),
        ('rev', uint8_t),
        ('crc', uint16_t),
        ('data', (td_char*0)),
    ]
store_record_info_t=random_type_1068
class random_type_1070(Structure):
    _fields_ = [
        ('start_flag', uint32_t),
        ('version', uint32_t),
        ('update_time', uint32_t),
        ('cur_pos', uint32_t),
        ('file_size', uint32_t),
        ('first_record_pos', uint32_t),
        ('records', uint32_t),
        ('service_type', uint8_t),
        ('offset', uint8_t),
        ('crc', uint16_t),
    ]
class random_type_1071(Structure):
    _fields_ = [
        ('name', td_u32),
        ('path', td_u32),
        ('file_size', uint32_t),
        ('cache_size', uint16_t),
        ('enable_cache', uint8_t),
        ('mult_files', uint8_t),
    ]
class random_type_1072(Structure):
    _fields_ = [
        ('oldest_file_idx', uint32_t),
        ('cur_file_idx', uint32_t),
        ('file_count', uint32_t),
    ]
class random_type_1073(Structure):
    _fields_ = [
        ('cache_write_pos', uint32_t),
        ('cache_read_pos', uint32_t),
        ('cache_size', uint32_t),
        ('threshold_size', uint32_t),
        ('data', (td_char*0)),
    ]
class random_type_1074(Structure):
    _fields_ = [
        ('magic', uint16_t),
        ('len', uint16_t),
        ('type', uint8_t),
        ('rev', uint8_t),
        ('crc', uint16_t),
        ('data', (td_char*0)),
    ]
class random_type_1076(Structure):
    _fields_ = [
        ('timer', td_u32),
        ('handler', td_u32),
        ('data', td_u32),
        ('interval', td_u32),
    ]
class random_type_1077(Structure):
    _fields_ = [
        ('task', td_u32),
    ]
class random_type_1078(Structure):
    _fields_ = [
        ('event', td_u32),
    ]
class random_type_1079(Structure):
    _fields_ = [
        ('mutex', td_u32),
    ]
lib_panic_dump_callback=td_u32
class random_type_1080(Structure):
    _fields_ = [
        ('sortLinkNode', LOS_DL_LIST),
        ('idxRollNum', UINT32),
    ]
class random_type_1081(Structure):
    _fields_ = [
        ('data0', uint32_t),
        ('data1', uint32_t),
        ('data2', uint32_t),
    ]
class excep_info_t(Structure):
    _fields_ = [
        ('sp', td_u32),
        ('sp_bottom', td_u32),
        ('exp_task_id', td_u32),
        ('task_array', td_u32),
        ('task_max_num', td_u32),
    ]
exception_info_t=excep_info_t
class stack_frame_t(Structure):
    _fields_ = [
        ('stacked_r0', td_u32),
        ('stacked_r1', td_u32),
        ('stacked_r2', td_u32),
        ('stacked_r3', td_u32),
        ('stacked_r12', td_u32),
        ('stacked_lr', td_u32),
        ('stacked_pc', td_u32),
        ('stacked_psr', td_u32),
    ]
exception_stack_frame_t=stack_frame_t
panic_deal_handler=td_u32
class random_type_1084(Structure):
    _fields_ = [
        ('ccause', uint32_t),
        ('mcause', uint32_t),
        ('mtval', uint32_t),
        ('gp', uint32_t),
        ('task_context', task_context_t),
    ]
class random_type_1085(Structure):
    _fields_ = [
        ('mstatus', uint32_t),
        ('mepc', uint32_t),
        ('tp', uint32_t),
        ('sp', uint32_t),
        ('s11', uint32_t),
        ('s10', uint32_t),
        ('s9', uint32_t),
        ('s8', uint32_t),
        ('s7', uint32_t),
        ('s6', uint32_t),
        ('s5', uint32_t),
        ('s4', uint32_t),
        ('s3', uint32_t),
        ('s2', uint32_t),
        ('s1', uint32_t),
        ('s0', uint32_t),
        ('t6', uint32_t),
        ('t5', uint32_t),
        ('t4', uint32_t),
        ('t3', uint32_t),
        ('a7', uint32_t),
        ('a6', uint32_t),
        ('a5', uint32_t),
        ('a4', uint32_t),
        ('a3', uint32_t),
        ('a2', uint32_t),
        ('a1', uint32_t),
        ('a0', uint32_t),
        ('t2', uint32_t),
        ('t1', uint32_t),
        ('t0', uint32_t),
        ('ra', uint32_t),
    ]
class random_type_1087(Structure):
    _fields_ = [
        ('pc', uint32_t),
        ('lr', uint32_t),
        ('sp', uint32_t),
    ]
preserve_cpu_info_t=random_type_1087
class random_type_1088(Structure):
    _fields_ = [
        ('time_us', uint32_t),
        ('length', uint16_t),
    ]
log_buffer_header_t=random_type_1088
class random_type_1089(Structure):
    _fields_ = [
        ('module_id', uint8_t),
        ('print_level', uint8_t),
    ]
om_log_module_lev_t=random_type_1089
class random_type_1090(Structure):
    _fields_ = [
        ('header', om_msg_header_stru_t),
        ('mem_start_addr', uint32_t),
        ('data_len', uint16_t),
        ('mode', uint8_t),
        ('reserve', uint8_t),
    ]
om_mem_req_header_t=random_type_1090
class random_type_1091(Structure):
    _fields_ = [
        ('frame_start', uint8_t),
        ('func_type', uint8_t),
        ('prime_id', uint8_t),
        ('arr_reserver', (uint8_t*1)),
        ('frame_len', uint16_t),
        ('sn', uint16_t),
    ]
class random_type_1093(Structure):
    _fields_ = [
        ('source_ptr', uint32_t),
        ('dest_ptr', uint32_t),
        ('length', uint16_t),
    ]
class random_type_1096(Structure):
    _fields_ = [
        ('reboot_cause', uint32_t),
    ]
class random_type_1097(Structure):
    _fields_ = [
        ('data', (uint8_t*3072)),
    ]
class random_type_1098(Structure):
    _fields_ = [
        ('requested_reboot_reason', uint16_t),
    ]
class random_type_1099(Structure):
    _fields_ = [
        ('log_level', uint32_t),
    ]
class random_type_1102(Structure):
    _fields_ = [
        ('size', uint32_t),
    ]
class random_type_1103(Structure):
    _fields_ = [
        ('result', uint32_t),
    ]
class random_type_1104(Structure):
    _fields_ = [
        ('params', (uint8_t*256)),
        ('length', uint32_t),
    ]
class random_type_1106(Structure):
    _fields_ = [
        ('type', uint8_t),
        ('event_id', uint8_t),
        ('info1', uint8_t),
        ('info2', uint8_t),
        ('info3', uint16_t),
    ]
class random_type_1107(Structure):
    _fields_ = [
        ('mass_data', ipc_payload_mass_data_type),
        ('data_len', uint8_t),
        ('data', (uint8_t*128)),
    ]
class random_type_1108(Structure):
    _fields_ = [
        ('data', (uint8_t*160)),
        ('length', uint16_t),
    ]
class random_type_1109(Structure):
    _fields_ = [
        ('clock_type', uint32_t),
    ]
class random_type_1110(Structure):
    _fields_ = [
        ('result', uint32_t),
    ]
class random_type_1111(Structure):
    _fields_ = [
        ('status', uint16_t),
        ('length', uint16_t),
        ('msg', (td_char*60)),
    ]
class random_type_1112(Structure):
    _fields_ = [
        ('flag', td_bool),
    ]
class random_type_1113(Structure):
    _fields_ = [
        ('flag', td_bool),
    ]
class random_type_1114(Structure):
    _fields_ = [
        ('flag', td_bool),
    ]
class random_type_1115(Structure):
    _fields_ = [
        ('updata', uint32_t),
    ]
class random_type_1116(Structure):
    _fields_ = [
        ('slave_freq', uint32_t),
    ]
class random_type_1117(Structure):
    _fields_ = [
        ('freq', uint32_t),
    ]
class random_type_1118(Structure):
    _fields_ = [
        ('power_switch', uint32_t),
    ]
class random_type_1119(Structure):
    _fields_ = [
        ('config_nv_1', uint16_t),
        ('config_nv_2', uint16_t),
        ('config_nv_3', uint16_t),
        ('config_nv_4', uint16_t),
        ('config_nv_5', (uint16_t*5)),
        ('config_nv_6', uint16_t),
        ('config_nv_7', uint16_t),
        ('config_nv_8', uint16_t),
        ('config_nv_9', uint16_t),
        ('config_nv_10', uint8_t),
        ('config_nv_11', int8_t),
        ('config_nv_12', int8_t),
        ('flag', uint8_t),
    ]
class random_type_1120(Structure):
    _fields_ = [
        ('nv_customize_config', uint32_t),
        ('nv_customize_config_reserved', uint32_t),
        ('nv_max_ble_tx_power_cfg_val', uint8_t),
        ('nv_max_gle_tx_power_cfg_val', uint8_t),
        ('fcc_flag', uint8_t),
        ('customize_four_k_support', uint8_t),
        ('customize_freq_map', (uint8_t*10)),
        ('reserved', (uint8_t*86)),
    ]
class random_type_1121(Structure):
    _fields_ = [
        ('header', om_msg_header_stru_t),
        ('reg_addr', uint32_t),
        ('reg_len', uint16_t),
        ('mode', uint8_t),
        ('reserver', uint8_t),
        ('reg_value', uint32_t),
        ('msg_tailer', uint8_t),
    ]
om_reg_data_stru_t=random_type_1121
class random_type_1122(Structure):
    _fields_ = [
        ('frame_start', uint8_t),
        ('func_type', uint8_t),
        ('prime_id', uint8_t),
        ('arr_reserver', (uint8_t*1)),
        ('frame_len', uint16_t),
        ('sn', uint16_t),
    ]
class om_wrssi_reg_data_stru_t(Structure):
    _fields_ = [
        ('header', om_msg_header_stru_t),
        ('unused_bit8', uint32_t),
        ('reg_addr', uint32_t),
        ('reg_value', uint32_t),
        ('msg_tailer', uint8_t),
    ]
class om_rdssi_reg_data_stru_t(Structure):
    _fields_ = [
        ('header', om_msg_header_stru_t),
        ('unused_bit8', uint32_t),
        ('reg_addr', uint32_t),
        ('msg_tailer', uint8_t),
    ]
class random_type_1123(Structure):
    _fields_ = [
        ('header', om_msg_header_stru_t),
        ('reg_addr', uint32_t),
        ('output_value', uint16_t),
        ('mode', uint8_t),
        ('reserver', uint8_t),
        ('reg_value', uint32_t),
        ('msg_tailer', uint8_t),
    ]
om_ssi_reg_output_stru_t=random_type_1123
class random_type_1124(Structure):
    _fields_ = [
        ('msg_header', om_msg_header_stru_t),
        ('stack_limit', uint32_t),
        ('fault_type', uint32_t),
        ('fault_reason', uint32_t),
        ('address', uint32_t),
        ('reg_value', (uint32_t*32)),
        ('psp_value', uint32_t),
        ('lr_value', uint32_t),
        ('pc_value', uint32_t),
        ('psps_value', uint32_t),
        ('primask_value', uint32_t),
        ('fault_mask_value', uint32_t),
        ('bserpri_value', uint32_t),
        ('control_value', uint32_t),
        ('msg_tail', uint8_t),
    ]
class random_type_1125(Structure):
    _fields_ = [
        ('frame_start', uint8_t),
        ('func_type', uint8_t),
        ('prime_id', uint8_t),
        ('arr_reserver', (uint8_t*1)),
        ('frame_len', uint16_t),
        ('sn', uint16_t),
    ]
class random_type_1126(Structure):
    _fields_ = [
        ('ccause', uint32_t),
        ('mcause', uint32_t),
        ('mtval', uint32_t),
        ('gp', uint32_t),
        ('task_context', task_context_t),
    ]
class random_type_1127(Structure):
    _fields_ = [
        ('mstatus', uint32_t),
        ('mepc', uint32_t),
        ('tp', uint32_t),
        ('sp', uint32_t),
        ('s11', uint32_t),
        ('s10', uint32_t),
        ('s9', uint32_t),
        ('s8', uint32_t),
        ('s7', uint32_t),
        ('s6', uint32_t),
        ('s5', uint32_t),
        ('s4', uint32_t),
        ('s3', uint32_t),
        ('s2', uint32_t),
        ('s1', uint32_t),
        ('s0', uint32_t),
        ('t6', uint32_t),
        ('t5', uint32_t),
        ('t4', uint32_t),
        ('t3', uint32_t),
        ('a7', uint32_t),
        ('a6', uint32_t),
        ('a5', uint32_t),
        ('a4', uint32_t),
        ('a3', uint32_t),
        ('a2', uint32_t),
        ('a1', uint32_t),
        ('a0', uint32_t),
        ('t2', uint32_t),
        ('t1', uint32_t),
        ('t0', uint32_t),
        ('ra', uint32_t),
    ]
class random_type_1129(Structure):
    _fields_ = [
        ('source_ptr', uint32_t),
        ('dest_ptr', uint32_t),
        ('length', uint16_t),
    ]
class random_type_1132(Structure):
    _fields_ = [
        ('reboot_cause', uint32_t),
    ]
class random_type_1133(Structure):
    _fields_ = [
        ('data', (uint8_t*3072)),
    ]
class random_type_1134(Structure):
    _fields_ = [
        ('requested_reboot_reason', uint16_t),
    ]
class random_type_1135(Structure):
    _fields_ = [
        ('log_level', uint32_t),
    ]
class random_type_1138(Structure):
    _fields_ = [
        ('size', uint32_t),
    ]
class random_type_1139(Structure):
    _fields_ = [
        ('result', uint32_t),
    ]
class random_type_1140(Structure):
    _fields_ = [
        ('params', (uint8_t*256)),
        ('length', uint32_t),
    ]
class random_type_1142(Structure):
    _fields_ = [
        ('type', uint8_t),
        ('event_id', uint8_t),
        ('info1', uint8_t),
        ('info2', uint8_t),
        ('info3', uint16_t),
    ]
class random_type_1143(Structure):
    _fields_ = [
        ('mass_data', ipc_payload_mass_data_type),
        ('data_len', uint8_t),
        ('data', (uint8_t*128)),
    ]
class random_type_1144(Structure):
    _fields_ = [
        ('data', (uint8_t*160)),
        ('length', uint16_t),
    ]
class random_type_1145(Structure):
    _fields_ = [
        ('clock_type', uint32_t),
    ]
class random_type_1146(Structure):
    _fields_ = [
        ('result', uint32_t),
    ]
class random_type_1147(Structure):
    _fields_ = [
        ('status', uint16_t),
        ('length', uint16_t),
        ('msg', (td_char*60)),
    ]
class random_type_1148(Structure):
    _fields_ = [
        ('flag', td_bool),
    ]
class random_type_1149(Structure):
    _fields_ = [
        ('flag', td_bool),
    ]
class random_type_1150(Structure):
    _fields_ = [
        ('flag', td_bool),
    ]
class random_type_1151(Structure):
    _fields_ = [
        ('updata', uint32_t),
    ]
class random_type_1152(Structure):
    _fields_ = [
        ('slave_freq', uint32_t),
    ]
class random_type_1153(Structure):
    _fields_ = [
        ('freq', uint32_t),
    ]
class random_type_1154(Structure):
    _fields_ = [
        ('power_switch', uint32_t),
    ]
class random_type_1155(Structure):
    _fields_ = [
        ('config_nv_1', uint16_t),
        ('config_nv_2', uint16_t),
        ('config_nv_3', uint16_t),
        ('config_nv_4', uint16_t),
        ('config_nv_5', (uint16_t*5)),
        ('config_nv_6', uint16_t),
        ('config_nv_7', uint16_t),
        ('config_nv_8', uint16_t),
        ('config_nv_9', uint16_t),
        ('config_nv_10', uint8_t),
        ('config_nv_11', int8_t),
        ('config_nv_12', int8_t),
        ('flag', uint8_t),
    ]
class random_type_1156(Structure):
    _fields_ = [
        ('nv_customize_config', uint32_t),
        ('nv_customize_config_reserved', uint32_t),
        ('nv_max_ble_tx_power_cfg_val', uint8_t),
        ('nv_max_gle_tx_power_cfg_val', uint8_t),
        ('fcc_flag', uint8_t),
        ('customize_four_k_support', uint8_t),
        ('customize_freq_map', (uint8_t*10)),
        ('reserved', (uint8_t*86)),
    ]
class random_type_1157(Structure):
    _fields_ = [
        ('start_addr', uint32_t),
        ('end_addr', uint32_t),
    ]
log_reg_dump_t=random_type_1157
class random_type_1158(Structure):
    _fields_ = [
        ('frame_start', uint8_t),
        ('func_type', uint8_t),
        ('prime_id', uint8_t),
        ('arr_reserver', (uint8_t*1)),
        ('frame_len', uint16_t),
        ('sn', uint16_t),
    ]
log_trigger_callback_t=td_u32
class random_type_1160(Structure):
    _fields_ = [
        ('source_ptr', uint32_t),
        ('dest_ptr', uint32_t),
        ('length', uint16_t),
    ]
class random_type_1163(Structure):
    _fields_ = [
        ('reboot_cause', uint32_t),
    ]
class random_type_1164(Structure):
    _fields_ = [
        ('data', (uint8_t*3072)),
    ]
class random_type_1165(Structure):
    _fields_ = [
        ('requested_reboot_reason', uint16_t),
    ]
class random_type_1166(Structure):
    _fields_ = [
        ('log_level', uint32_t),
    ]
class random_type_1169(Structure):
    _fields_ = [
        ('size', uint32_t),
    ]
class random_type_1170(Structure):
    _fields_ = [
        ('result', uint32_t),
    ]
class random_type_1171(Structure):
    _fields_ = [
        ('params', (uint8_t*256)),
        ('length', uint32_t),
    ]
class random_type_1173(Structure):
    _fields_ = [
        ('type', uint8_t),
        ('event_id', uint8_t),
        ('info1', uint8_t),
        ('info2', uint8_t),
        ('info3', uint16_t),
    ]
class random_type_1174(Structure):
    _fields_ = [
        ('mass_data', ipc_payload_mass_data_type),
        ('data_len', uint8_t),
        ('data', (uint8_t*128)),
    ]
class random_type_1175(Structure):
    _fields_ = [
        ('data', (uint8_t*160)),
        ('length', uint16_t),
    ]
class random_type_1176(Structure):
    _fields_ = [
        ('clock_type', uint32_t),
    ]
class random_type_1177(Structure):
    _fields_ = [
        ('result', uint32_t),
    ]
class random_type_1178(Structure):
    _fields_ = [
        ('status', uint16_t),
        ('length', uint16_t),
        ('msg', (td_char*60)),
    ]
class random_type_1179(Structure):
    _fields_ = [
        ('flag', td_bool),
    ]
class random_type_1180(Structure):
    _fields_ = [
        ('flag', td_bool),
    ]
class random_type_1181(Structure):
    _fields_ = [
        ('flag', td_bool),
    ]
class random_type_1182(Structure):
    _fields_ = [
        ('updata', uint32_t),
    ]
class random_type_1183(Structure):
    _fields_ = [
        ('slave_freq', uint32_t),
    ]
class random_type_1184(Structure):
    _fields_ = [
        ('freq', uint32_t),
    ]
class random_type_1185(Structure):
    _fields_ = [
        ('power_switch', uint32_t),
    ]
class random_type_1186(Structure):
    _fields_ = [
        ('config_nv_1', uint16_t),
        ('config_nv_2', uint16_t),
        ('config_nv_3', uint16_t),
        ('config_nv_4', uint16_t),
        ('config_nv_5', (uint16_t*5)),
        ('config_nv_6', uint16_t),
        ('config_nv_7', uint16_t),
        ('config_nv_8', uint16_t),
        ('config_nv_9', uint16_t),
        ('config_nv_10', uint8_t),
        ('config_nv_11', int8_t),
        ('config_nv_12', int8_t),
        ('flag', uint8_t),
    ]
class random_type_1187(Structure):
    _fields_ = [
        ('nv_customize_config', uint32_t),
        ('nv_customize_config_reserved', uint32_t),
        ('nv_max_ble_tx_power_cfg_val', uint8_t),
        ('nv_max_gle_tx_power_cfg_val', uint8_t),
        ('fcc_flag', uint8_t),
        ('customize_four_k_support', uint8_t),
        ('customize_freq_map', (uint8_t*10)),
        ('reserved', (uint8_t*86)),
    ]
class random_type_1188(Structure):
    _fields_ = [
        ('frame_start', uint8_t),
        ('func_type', uint8_t),
        ('prime_id', uint8_t),
        ('arr_reserver', (uint8_t*1)),
        ('frame_len', uint16_t),
        ('sn', uint16_t),
    ]
class random_type_1189(Structure):
    _fields_ = [
        ('mode', uint8_t),
        ('reg_mem_size', uint8_t),
        ('reg_mem_addr_value', (uint32_t*2)),
    ]
rw_reg_mem_cmd_t=random_type_1189
class random_type_1190(Structure):
    _fields_ = [
        ('read', uint32_t),
        ('write', uint32_t),
        ('flow_flag', uint32_t),
        ('region_len', uint32_t),
        ('region_start', uint32_t),
        ('water_line', uint32_t),
    ]
massdata_memory_section_control_t=random_type_1190
class random_type_1191(Structure):
    _fields_ = [
        ('read', uint32_t),
        ('write', uint32_t),
    ]
log_memory_section_control_t=random_type_1191
class random_type_1193(Structure):
    _fields_ = [
        ('source_ptr', uint32_t),
        ('dest_ptr', uint32_t),
        ('length', uint16_t),
    ]
class random_type_1196(Structure):
    _fields_ = [
        ('reboot_cause', uint32_t),
    ]
class random_type_1197(Structure):
    _fields_ = [
        ('data', (uint8_t*3072)),
    ]
class random_type_1198(Structure):
    _fields_ = [
        ('requested_reboot_reason', uint16_t),
    ]
class random_type_1199(Structure):
    _fields_ = [
        ('log_level', uint32_t),
    ]
class random_type_1202(Structure):
    _fields_ = [
        ('size', uint32_t),
    ]
class random_type_1203(Structure):
    _fields_ = [
        ('result', uint32_t),
    ]
class random_type_1204(Structure):
    _fields_ = [
        ('params', (uint8_t*256)),
        ('length', uint32_t),
    ]
class random_type_1206(Structure):
    _fields_ = [
        ('type', uint8_t),
        ('event_id', uint8_t),
        ('info1', uint8_t),
        ('info2', uint8_t),
        ('info3', uint16_t),
    ]
class random_type_1207(Structure):
    _fields_ = [
        ('mass_data', ipc_payload_mass_data_type),
        ('data_len', uint8_t),
        ('data', (uint8_t*128)),
    ]
class random_type_1208(Structure):
    _fields_ = [
        ('data', (uint8_t*160)),
        ('length', uint16_t),
    ]
class random_type_1209(Structure):
    _fields_ = [
        ('clock_type', uint32_t),
    ]
class random_type_1210(Structure):
    _fields_ = [
        ('result', uint32_t),
    ]
class random_type_1211(Structure):
    _fields_ = [
        ('status', uint16_t),
        ('length', uint16_t),
        ('msg', (td_char*60)),
    ]
class random_type_1212(Structure):
    _fields_ = [
        ('flag', td_bool),
    ]
class random_type_1213(Structure):
    _fields_ = [
        ('flag', td_bool),
    ]
class random_type_1214(Structure):
    _fields_ = [
        ('flag', td_bool),
    ]
class random_type_1215(Structure):
    _fields_ = [
        ('updata', uint32_t),
    ]
class random_type_1216(Structure):
    _fields_ = [
        ('slave_freq', uint32_t),
    ]
class random_type_1217(Structure):
    _fields_ = [
        ('freq', uint32_t),
    ]
class random_type_1218(Structure):
    _fields_ = [
        ('power_switch', uint32_t),
    ]
class random_type_1219(Structure):
    _fields_ = [
        ('config_nv_1', uint16_t),
        ('config_nv_2', uint16_t),
        ('config_nv_3', uint16_t),
        ('config_nv_4', uint16_t),
        ('config_nv_5', (uint16_t*5)),
        ('config_nv_6', uint16_t),
        ('config_nv_7', uint16_t),
        ('config_nv_8', uint16_t),
        ('config_nv_9', uint16_t),
        ('config_nv_10', uint8_t),
        ('config_nv_11', int8_t),
        ('config_nv_12', int8_t),
        ('flag', uint8_t),
    ]
class random_type_1220(Structure):
    _fields_ = [
        ('nv_customize_config', uint32_t),
        ('nv_customize_config_reserved', uint32_t),
        ('nv_max_ble_tx_power_cfg_val', uint8_t),
        ('nv_max_gle_tx_power_cfg_val', uint8_t),
        ('fcc_flag', uint8_t),
        ('customize_four_k_support', uint8_t),
        ('customize_freq_map', (uint8_t*10)),
        ('reserved', (uint8_t*86)),
    ]
class random_type_1221(Structure):
    _fields_ = [
        ('start', td_u32),
        ('length', uint32_t),
    ]
class random_type_1222(Structure):
    _fields_ = [
        ('time_us', uint32_t),
        ('length', uint16_t),
    ]
log_memory_section_params_t=random_type_1221
class random_type_1223(Structure):
    _fields_ = [
        ('type', uint8_t),
        ('event_id', uint8_t),
        ('info1', uint8_t),
        ('info2', uint8_t),
        ('info3', uint16_t),
        ('data_len', uint8_t),
        ('data', (uint8_t*128)),
    ]
chr_extend_data_t=random_type_1223
class random_type_1224(Structure):
    _fields_ = [
        ('read', uint32_t),
        ('write', uint32_t),
    ]
class random_type_1225(Structure):
    _fields_ = [
        ('log_buffer_start', td_u32),
        ('log_buffer_size', uint32_t),
        ('lb_next_to_consume', uint32_t),
    ]
log_buffer_reader_control_t=random_type_1225
class random_type_1226(Structure):
    _fields_ = [
        ('start', td_u32),
        ('length', uint32_t),
    ]
class random_type_1227(Structure):
    _fields_ = [
        ('time_us', uint32_t),
        ('length', uint16_t),
    ]
class random_type_1228(Structure):
    _fields_ = [
        ('offset', (uint32_t*3)),
        ('length', uint32_t),
        ('section_control', (log_memory_section_control_t*3)),
    ]
log_memory_region_control_t=random_type_1228
class random_type_1229(Structure):
    _fields_ = [
        ('read', uint32_t),
        ('write', uint32_t),
    ]
class random_type_1230(Structure):
    _fields_ = [
        ('offset', (uint32_t*3)),
        ('length', uint32_t),
        ('section_control', (log_memory_section_control_t*3)),
    ]
class random_type_1231(Structure):
    _fields_ = [
        ('read', uint32_t),
        ('write', uint32_t),
    ]
class random_type_1232(Structure):
    _fields_ = [
        ('start', td_u32),
        ('length', uint32_t),
    ]
class random_type_1234(Structure):
    _fields_ = [
        ('name', td_u32),
        ('attr_bits', uint32_t),
        ('cb_mem', td_u32),
        ('cb_size', uint32_t),
    ]
class random_type_1235(Structure):
    _fields_ = [
        ('name', td_u32),
        ('attr_bits', uint32_t),
        ('cb_mem', td_u32),
        ('cb_size', uint32_t),
    ]
class random_type_1237(Structure):
    _fields_ = [
        ('source_ptr', uint32_t),
        ('dest_ptr', uint32_t),
        ('length', uint16_t),
    ]
class random_type_1240(Structure):
    _fields_ = [
        ('reboot_cause', uint32_t),
    ]
class random_type_1241(Structure):
    _fields_ = [
        ('data', (uint8_t*3072)),
    ]
class random_type_1242(Structure):
    _fields_ = [
        ('requested_reboot_reason', uint16_t),
    ]
class random_type_1243(Structure):
    _fields_ = [
        ('log_level', uint32_t),
    ]
class random_type_1246(Structure):
    _fields_ = [
        ('size', uint32_t),
    ]
class random_type_1247(Structure):
    _fields_ = [
        ('result', uint32_t),
    ]
class random_type_1248(Structure):
    _fields_ = [
        ('params', (uint8_t*256)),
        ('length', uint32_t),
    ]
class random_type_1250(Structure):
    _fields_ = [
        ('type', uint8_t),
        ('event_id', uint8_t),
        ('info1', uint8_t),
        ('info2', uint8_t),
        ('info3', uint16_t),
    ]
class random_type_1251(Structure):
    _fields_ = [
        ('mass_data', ipc_payload_mass_data_type),
        ('data_len', uint8_t),
        ('data', (uint8_t*128)),
    ]
class random_type_1252(Structure):
    _fields_ = [
        ('data', (uint8_t*160)),
        ('length', uint16_t),
    ]
class random_type_1253(Structure):
    _fields_ = [
        ('clock_type', uint32_t),
    ]
class random_type_1254(Structure):
    _fields_ = [
        ('result', uint32_t),
    ]
class random_type_1255(Structure):
    _fields_ = [
        ('status', uint16_t),
        ('length', uint16_t),
        ('msg', (td_char*60)),
    ]
class random_type_1256(Structure):
    _fields_ = [
        ('flag', td_bool),
    ]
class random_type_1257(Structure):
    _fields_ = [
        ('flag', td_bool),
    ]
class random_type_1258(Structure):
    _fields_ = [
        ('flag', td_bool),
    ]
class random_type_1259(Structure):
    _fields_ = [
        ('updata', uint32_t),
    ]
class random_type_1260(Structure):
    _fields_ = [
        ('slave_freq', uint32_t),
    ]
class random_type_1261(Structure):
    _fields_ = [
        ('freq', uint32_t),
    ]
class random_type_1262(Structure):
    _fields_ = [
        ('power_switch', uint32_t),
    ]
class random_type_1263(Structure):
    _fields_ = [
        ('config_nv_1', uint16_t),
        ('config_nv_2', uint16_t),
        ('config_nv_3', uint16_t),
        ('config_nv_4', uint16_t),
        ('config_nv_5', (uint16_t*5)),
        ('config_nv_6', uint16_t),
        ('config_nv_7', uint16_t),
        ('config_nv_8', uint16_t),
        ('config_nv_9', uint16_t),
        ('config_nv_10', uint8_t),
        ('config_nv_11', int8_t),
        ('config_nv_12', int8_t),
        ('flag', uint8_t),
    ]
class random_type_1264(Structure):
    _fields_ = [
        ('nv_customize_config', uint32_t),
        ('nv_customize_config_reserved', uint32_t),
        ('nv_max_ble_tx_power_cfg_val', uint8_t),
        ('nv_max_gle_tx_power_cfg_val', uint8_t),
        ('fcc_flag', uint8_t),
        ('customize_four_k_support', uint8_t),
        ('customize_freq_map', (uint8_t*10)),
        ('reserved', (uint8_t*86)),
    ]
class random_type_1265(Structure):
    _fields_ = [
        ('time_us', uint32_t),
        ('length', uint16_t),
    ]
class inode_search_s(Structure):
    _fields_ = [
        ('path', td_u32),
        ('node', td_u32),
        ('peer', td_u32),
        ('parent', td_u32),
        ('relpath', td_u32),
    ]
class random_type_1266(Structure):
    _fields_ = [
        ('lock', td_u32),
    ]
class random_type_1267(Structure):
    _fields_ = [
        ('sem', td_u32),
    ]
class random_type_1268(Structure):
    _fields_ = [
        ('lock', td_u32),
    ]
class random_type_1269(Structure):
    _fields_ = [
        ('sem', td_u32),
    ]
class random_type_1270(Structure):
    _fields_ = [
        ('lock', td_u32),
    ]
class random_type_1271(Structure):
    _fields_ = [
        ('sem', td_u32),
    ]
class inode_sem_s(Structure):
    _fields_ = [
        ('sem', nu_sem),
        ('holder', uint32_t),
        ('count', int16_t),
    ]
class random_type_1272(Structure):
    _fields_ = [
        ('sem', td_u32),
    ]
class random_type_1273(Structure):
    _fields_ = [
        ('sem', td_u32),
    ]
pid_t=td_s32
class file_struct(Structure):
    _fields_ = [
        ('fs_fd', td_s32),
        ('fs_sem', nu_sem),
        ('fs_holder', pid_t),
        ('fs_counts', td_s32),
        ('fs_bufstart', td_u32),
        ('fs_bufend', td_u32),
        ('fs_bufpos', td_u32),
        ('fs_bufread', td_u32),
        ('fs_oflags', uint16_t),
        ('fs_flags', uint8_t),
    ]
class streamlist(Structure):
    _fields_ = [
        ('sl_sem', nu_sem),
        ('sl_streams', (file_struct*1)),
    ]
class fs_pseudodir_s(Structure):
    _fields_ = [
        ('fd_next', td_u32),
    ]
fs_dir_s=td_u32
class mtd_node(Structure):
    _fields_ = [
        ('start_block', u_int),
        ('end_block', u_int),
        ('patitionnum', u_int),
        ('blockdriver_name', td_u32),
        ('chardriver_name', td_u32),
        ('mountpoint_name', td_u32),
        ('mtd_info', td_u32),
        ('node_info', dpal_dl_list_t),
        ('lock', u_int),
        ('user_num', u_int),
    ]
mtd_partition=mtd_node
class fsmap_t(Structure):
    _fields_ = [
        ('fs_filesystemtype', td_u32),
        ('fs_mops', td_u32),
        ('is_mtd_support', BOOL),
        ('is_bdfs', BOOL),
    ]
class random_type_1278(Structure):
    _fields_ = [
        ('sem', td_u32),
    ]
class rb_root(Structure):
    _fields_ = [
        ('rb_node', td_u32),
    ]
class random_type_1280(Structure):
    _fields_ = [
        ('rbe_left', td_u32),
        ('rbe_right', td_u32),
        ('rbe_parent', td_u32),
        ('rbe_color', td_s32),
    ]
class rb_node(Structure):
    _fields_ = [
        ('__entry', random_type_1280),
    ]
BcacheReadFun=td_u32
BcacheWriteFun=td_u32
BcachePrereadFun=td_u32
class random_type_1282(Structure):
    _fields_ = [
        ('event_id', uint32_t),
        ('event_max_num', uint32_t),
        ('event_bits', uint32_t),
        ('event_bit_idx', uint32_t),
        ('event', osal_event),
    ]
class random_type_1283(Structure):
    _fields_ = [
        ('event', td_u32),
    ]
class random_type_1284(Structure):
    _fields_ = [
        ('lock', td_u32),
    ]
StorageHookFunction=td_u32
class random_type_1285(Structure):
    _fields_ = [
        ('sem', td_u32),
    ]
class random_type_1286(Structure):
    _fields_ = [
        ('nv_sem_handle', uint32_t),
        ('nv_change_notify_list', td_u32),
        ('notify_regitser_max_nums', uint8_t),
        ('notify_registered_nums', uint8_t),
        ('rsv2', uint16_t),
    ]
nv_direct_ctrl_t=random_type_1286
nv_changed_notify_func=td_u32
class random_type_1287(Structure):
    _fields_ = [
        ('func', nv_changed_notify_func),
        ('min_key', uint16_t),
        ('max_key', uint16_t),
    ]
nv_changed_proc_t=random_type_1287
class random_type_1288(Structure):
    _fields_ = [
        ('permanent', td_bool),
        ('encrypted', td_bool),
        ('non_upgrade', td_bool),
        ('reserve', uint8_t),
    ]
flash_task_completed_callback=td_u32
class owner_callback_data_t(Structure):
    _fields_ = [
        ('func', flash_task_completed_callback),
    ]
owner_callback=owner_callback_data_t
class random_type_1290(Structure):
    _fields_ = [
        ('kv_addr', uint32_t),
        ('kv_size', uint32_t),
    ]
flash_task_kv_init_t=random_type_1290
kv_storage_key=td_u16
class random_type_1291(Structure):
    _fields_ = [
        ('kvalue', td_u32),
        ('key', kv_storage_key),
        ('kvalue_length', uint16_t),
        ('attribute', uint8_t),
        ('force_write', td_bool),
    ]
flash_task_kv_data_t=random_type_1291
class random_type_1292(Structure):
    _fields_ = [
        ('key', kv_storage_key),
        ('attribute', uint8_t),
    ]
flash_task_kv_attribute_t=random_type_1292
class random_type_1293(Structure):
    _fields_ = [
        ('key', kv_storage_key),
    ]
flash_task_kv_erase_t=random_type_1293
class random_type_1294(Structure):
    _fields_ = [
        ('src', td_u32),
        ('dst', td_u32),
        ('length', uint16_t),
    ]
flash_task_flash_data_t=random_type_1294
class random_type_1295(Structure):
    _fields_ = [
        ('start_address', uint32_t),
        ('length', uint32_t),
    ]
flash_task_flash_erase_t=random_type_1295
class random_type_1289(Union):
    _fields_ = [
        ('kv_region', flash_task_kv_init_t),
        ('kv', flash_task_kv_data_t),
        ('kv_attribute', flash_task_kv_attribute_t),
        ('kv_erase', flash_task_kv_erase_t),
        ('flash', flash_task_flash_data_t),
        ('flash_erase', flash_task_flash_erase_t),
    ]
task_node_data_t=random_type_1289
class random_type_1296(Structure):
    _fields_ = [
        ('kvalue_max_length', uint16_t),
        ('kvalue_actual_length', uint16_t),
        ('kvalue', td_u32),
    ]
class random_type_1297(Structure):
    _fields_ = [
        ('total_space', uint32_t),
        ('used_space', uint32_t),
        ('reclaimable_space', uint32_t),
        ('corrupted_space', uint32_t),
        ('max_key_space', uint32_t),
    ]
nv_storage_completed_callback=td_u32
kv_store_key_data_t=random_type_1296
class random_type_1298(Structure):
    _fields_ = [
        ('region_mode', (td_bool*16)),
    ]
class random_type_1299(Structure):
    _fields_ = [
        ('region_mode', (td_bool*16)),
    ]
class random_type_1300(Structure):
    _fields_ = [
        ('permanent', td_bool),
        ('encrypted', td_bool),
        ('non_upgrade', td_bool),
        ('reserve', uint8_t),
    ]
class random_type_1301(Structure):
    _fields_ = [
        ('total_space', uint32_t),
        ('used_space', uint32_t),
        ('reclaimable_space', uint32_t),
        ('corrupted_space', uint32_t),
        ('max_key_space', uint32_t),
    ]
class random_type_1302(Structure):
    _fields_ = [
        ('nv_sem_handle', uint32_t),
        ('nv_change_notify_list', td_u32),
        ('notify_regitser_max_nums', uint8_t),
        ('notify_registered_nums', uint8_t),
        ('rsv2', uint16_t),
    ]
class random_type_1303(Structure):
    _fields_ = [
        ('func', nv_changed_notify_func),
        ('min_key', uint16_t),
        ('max_key', uint16_t),
    ]
class random_type_1304(Structure):
    _fields_ = [
        ('region_mode', (td_bool*16)),
    ]
class random_type_1305(Structure):
    _fields_ = [
        ('region_mode', (td_bool*16)),
    ]
kv_key_location=td_u32
class random_type_1306(Structure):
    _fields_ = [
        ('magic', uint8_t),
        ('valid', uint8_t),
        ('length', uint16_t),
        ('type', uint8_t),
        ('upgrade', uint8_t),
        ('key_id', uint16_t),
        ('enc_key', uint16_t),
        ('version', uint16_t),
        ('rnd', uint32_t),
    ]
kv_key_header_t=random_type_1306
class random_type_1307(Structure):
    _fields_ = [
        ('header', kv_key_header_t),
        ('key_location', kv_key_location),
    ]
kv_key_handle_t=random_type_1307
class random_type_1308(Structure):
    _fields_ = [
        ('crypto_handle', uint32_t),
        ('key_data_chunk', td_u32),
        ('compare_data_chunk', td_u32),
    ]
kv_helper_compare_key_data_info_t=random_type_1308
kv_key_id=td_u16
class random_type_1309(Structure):
    _fields_ = [
        ('nv_data_addr', uint32_t),
        ('nv_data_size', uint32_t),
        ('nv_backup_addr', uint32_t),
        ('nv_backup_size', uint32_t),
    ]
kv_nvregion_area_t=random_type_1309
class random_type_1312(Structure):
    _fields_ = [
        ('entries', td_u32),
        ('page_status_map', td_u32),
        ('num_entries', uint32_t),
    ]
class random_type_1313(Structure):
    _fields_ = [
        ('details', kv_page_details_t),
        ('inverted_details_word', uint32_t),
        ('sequence_number', uint32_t),
        ('inverted_sequence_number', uint32_t),
    ]
class random_type_1314(Structure):
    _fields_ = [
        ('store_id', uint16_t),
        ('ver', uint8_t),
        ('page_index', uint8_t),
    ]
class random_type_1315(Structure):
    _fields_ = [
        ('page_location', uint32_t),
        ('used_space', uint16_t),
        ('reclaimable_space', uint16_t),
        ('corrupted_space', uint16_t),
        ('first_writable_offset', uint16_t),
    ]
class random_type_1316(Structure):
    _fields_ = [
        ('nv_data_addr', uint32_t),
        ('nv_data_size', uint32_t),
        ('nv_backup_addr', uint32_t),
        ('nv_backup_size', uint32_t),
    ]
class random_type_1317(Structure):
    _fields_ = [
        ('page_header', kv_page_header_t),
        ('page_location', kv_page_location),
    ]
class random_type_1318(Structure):
    _fields_ = [
        ('header', kv_key_header_t),
        ('key_location', kv_key_location),
    ]
class random_type_1319(Structure):
    _fields_ = [
        ('magic', uint8_t),
        ('valid', uint8_t),
        ('length', uint16_t),
        ('type', uint8_t),
        ('upgrade', uint8_t),
        ('key_id', uint16_t),
        ('enc_key', uint16_t),
        ('version', uint16_t),
        ('rnd', uint32_t),
    ]
class random_type_1321(Structure):
    _fields_ = [
        ('total_space', uint32_t),
        ('used_space', uint32_t),
        ('reclaimable_space', uint32_t),
        ('corrupted_space', uint32_t),
        ('max_key_space', uint32_t),
        ('first_writable_location', uint32_t),
    ]
class random_type_1322(Structure):
    _fields_ = [
        ('page_location', uint32_t),
        ('used_space', uint16_t),
        ('reclaimable_space', uint16_t),
        ('corrupted_space', uint16_t),
        ('first_writable_offset', uint16_t),
    ]
class random_type_1323(Structure):
    _fields_ = [
        ('entries', td_u32),
        ('page_status_map', td_u32),
        ('num_entries', uint32_t),
    ]
class random_type_1324(Structure):
    _fields_ = [
        ('details', kv_page_details_t),
        ('inverted_details_word', uint32_t),
        ('sequence_number', uint32_t),
        ('inverted_sequence_number', uint32_t),
    ]
class random_type_1325(Structure):
    _fields_ = [
        ('store_id', uint16_t),
        ('ver', uint8_t),
        ('page_index', uint8_t),
    ]
class random_type_1326(Structure):
    _fields_ = [
        ('page_header', kv_page_header_t),
        ('page_location', kv_page_location),
    ]
class random_type_1327(Structure):
    _fields_ = [
        ('details', kv_page_details_t),
        ('inverted_details_word', uint32_t),
        ('sequence_number', uint32_t),
        ('inverted_sequence_number', uint32_t),
    ]
class random_type_1328(Structure):
    _fields_ = [
        ('store_id', uint16_t),
        ('ver', uint8_t),
        ('page_index', uint8_t),
    ]
class random_type_1329(Structure):
    _fields_ = [
        ('page_header', kv_page_header_t),
        ('page_location', kv_page_location),
    ]
class random_type_1331(Structure):
    _fields_ = [
        ('header', kv_key_header_t),
        ('key_location', kv_key_location),
    ]
class random_type_1332(Structure):
    _fields_ = [
        ('magic', uint8_t),
        ('valid', uint8_t),
        ('length', uint16_t),
        ('type', uint8_t),
        ('upgrade', uint8_t),
        ('key_id', uint16_t),
        ('enc_key', uint16_t),
        ('version', uint16_t),
        ('rnd', uint32_t),
    ]
class random_type_1333(Structure):
    _fields_ = [
        ('total_space', uint32_t),
        ('used_space', uint32_t),
        ('reclaimable_space', uint32_t),
        ('corrupted_space', uint32_t),
        ('max_key_space', uint32_t),
        ('first_writable_location', uint32_t),
    ]
class random_type_1334(Structure):
    _fields_ = [
        ('kvalue_max_length', uint16_t),
        ('kvalue_actual_length', uint16_t),
        ('kvalue', td_u32),
    ]
class random_type_1335(Structure):
    _fields_ = [
        ('total_space', uint32_t),
        ('used_space', uint32_t),
        ('reclaimable_space', uint32_t),
        ('corrupted_space', uint32_t),
        ('max_key_space', uint32_t),
    ]
kv_state_action=td_u32
class random_type_1341(Structure):
    _fields_ = [
        ('data', td_u32),
        ('write_location', uint32_t),
        ('resume_location', uint32_t),
        ('crypto_handle', uint32_t),
        ('size', uint16_t),
        ('data_length', uint16_t),
        ('crc_ret', uint32_t),
        ('data_consumed', uint16_t),
        ('hash_claimed', td_bool,1),
        ('encrypt_claimed', td_bool,1),
        ('crc_claimed', td_bool,1),
        ('gcm_tag_claimed', td_bool,1),
    ]
kv_managed_write_buffer_t=random_type_1341
class managed_source_buffer(Structure):
    _fields_ = [
        ('next', td_u32),
        ('data', td_u32),
        ('data_length', uint16_t),
        ('data_consumed', uint16_t),
        ('release_data', td_bool,1),
        ('receive_hash', td_bool,1),
        ('hash_data', td_bool,1),
        ('crc_data', td_bool,1),
        ('encrypt_data', td_bool,1),
        ('gcm_tag_data', td_bool,1),
    ]
kv_managed_source_buffer_t=managed_source_buffer
class random_type_1342(Structure):
    _fields_ = [
        ('page_header', kv_page_header_t),
        ('page_location', kv_page_location),
    ]
class random_type_1343(Structure):
    _fields_ = [
        ('details', kv_page_details_t),
        ('inverted_details_word', uint32_t),
        ('sequence_number', uint32_t),
        ('inverted_sequence_number', uint32_t),
    ]
class random_type_1344(Structure):
    _fields_ = [
        ('store_id', uint16_t),
        ('ver', uint8_t),
        ('page_index', uint8_t),
    ]
class random_type_1345(Structure):
    _fields_ = [
        ('header', kv_key_header_t),
        ('key_location', kv_key_location),
    ]
class random_type_1346(Structure):
    _fields_ = [
        ('magic', uint8_t),
        ('valid', uint8_t),
        ('length', uint16_t),
        ('type', uint8_t),
        ('upgrade', uint8_t),
        ('key_id', uint16_t),
        ('enc_key', uint16_t),
        ('version', uint16_t),
        ('rnd', uint32_t),
    ]
class random_type_1347(Structure):
    _fields_ = [
        ('entries', td_u32),
        ('page_status_map', td_u32),
        ('num_entries', uint32_t),
    ]
class random_type_1348(Structure):
    _fields_ = [
        ('page_location', uint32_t),
        ('used_space', uint16_t),
        ('reclaimable_space', uint16_t),
        ('corrupted_space', uint16_t),
        ('first_writable_offset', uint16_t),
    ]
class random_type_1349(Structure):
    _fields_ = [
        ('total_space', uint32_t),
        ('used_space', uint32_t),
        ('reclaimable_space', uint32_t),
        ('corrupted_space', uint32_t),
        ('max_key_space', uint32_t),
        ('first_writable_location', uint32_t),
    ]
class random_type_1350(Union):
    _fields_ = [
        ('kv_region', flash_task_kv_init_t),
        ('kv', flash_task_kv_data_t),
        ('kv_attribute', flash_task_kv_attribute_t),
        ('kv_erase', flash_task_kv_erase_t),
        ('flash', flash_task_flash_data_t),
        ('flash_erase', flash_task_flash_erase_t),
    ]
class random_type_1351(Structure):
    _fields_ = [
        ('kv_addr', uint32_t),
        ('kv_size', uint32_t),
    ]
class random_type_1352(Structure):
    _fields_ = [
        ('kvalue', td_u32),
        ('key', kv_storage_key),
        ('kvalue_length', uint16_t),
        ('attribute', uint8_t),
        ('force_write', td_bool),
    ]
class random_type_1353(Structure):
    _fields_ = [
        ('key', kv_storage_key),
        ('attribute', uint8_t),
    ]
class random_type_1354(Structure):
    _fields_ = [
        ('key', kv_storage_key),
    ]
class random_type_1355(Structure):
    _fields_ = [
        ('src', td_u32),
        ('dst', td_u32),
        ('length', uint16_t),
    ]
class random_type_1356(Structure):
    _fields_ = [
        ('start_address', uint32_t),
        ('length', uint32_t),
    ]
class random_type_1357(Structure):
    _fields_ = [
        ('details', kv_page_details_t),
        ('inverted_details_word', uint32_t),
        ('sequence_number', uint32_t),
        ('inverted_sequence_number', uint32_t),
    ]
class random_type_1358(Structure):
    _fields_ = [
        ('store_id', uint16_t),
        ('ver', uint8_t),
        ('page_index', uint8_t),
    ]
class random_type_1359(Structure):
    _fields_ = [
        ('magic', uint8_t),
        ('valid', uint8_t),
        ('length', uint16_t),
        ('type', uint8_t),
        ('upgrade', uint8_t),
        ('key_id', uint16_t),
        ('enc_key', uint16_t),
        ('version', uint16_t),
        ('rnd', uint32_t),
    ]
class random_type_1360(Union):
    _fields_ = [
        ('kv_region', flash_task_kv_init_t),
        ('kv', flash_task_kv_data_t),
        ('kv_attribute', flash_task_kv_attribute_t),
        ('kv_erase', flash_task_kv_erase_t),
        ('flash', flash_task_flash_data_t),
        ('flash_erase', flash_task_flash_erase_t),
    ]
class random_type_1361(Structure):
    _fields_ = [
        ('kv_addr', uint32_t),
        ('kv_size', uint32_t),
    ]
class random_type_1362(Structure):
    _fields_ = [
        ('kvalue', td_u32),
        ('key', kv_storage_key),
        ('kvalue_length', uint16_t),
        ('attribute', uint8_t),
        ('force_write', td_bool),
    ]
class random_type_1363(Structure):
    _fields_ = [
        ('key', kv_storage_key),
        ('attribute', uint8_t),
    ]
class random_type_1364(Structure):
    _fields_ = [
        ('key', kv_storage_key),
    ]
class random_type_1365(Structure):
    _fields_ = [
        ('src', td_u32),
        ('dst', td_u32),
        ('length', uint16_t),
    ]
class random_type_1366(Structure):
    _fields_ = [
        ('start_address', uint32_t),
        ('length', uint32_t),
    ]
class random_type_1367(Structure):
    _fields_ = [
        ('sem', td_u32),
    ]
class nv_key_map(Structure):
    _fields_ = [
        ('key_id', uint16_t),
        ('key_len', uint16_t,12),
        ('permanent', uint16_t,1),
        ('encrypted', uint16_t,1),
        ('non_upgrade', uint16_t,1),
        ('flush_flag', uint16_t,1),
        ('kvalue', (uint8_t*0)),
    ]
nv_key_map_t=nv_key_map
class random_type_1368(Structure):
    _fields_ = [
        ('permanent', td_bool),
        ('encrypted', td_bool),
        ('non_upgrade', td_bool),
        ('reserve', uint8_t),
    ]
class random_type_1369(Structure):
    _fields_ = [
        ('key_id', uint16_t),
        ('key_ram_location', uintptr_t),
    ]
nv_queue_msg_handle_t=random_type_1369
class random_type_1370(Structure):
    _fields_ = [
        ('total_space', uint32_t),
        ('used_space', uint32_t),
        ('reclaimable_space', uint32_t),
        ('corrupted_space', uint32_t),
        ('max_key_space', uint32_t),
    ]
class random_type_1371(Structure):
    _fields_ = [
        ('page_header', kv_page_header_t),
        ('page_location', kv_page_location),
    ]
class random_type_1372(Structure):
    _fields_ = [
        ('details', kv_page_details_t),
        ('inverted_details_word', uint32_t),
        ('sequence_number', uint32_t),
        ('inverted_sequence_number', uint32_t),
    ]
class random_type_1373(Structure):
    _fields_ = [
        ('store_id', uint16_t),
        ('ver', uint8_t),
        ('page_index', uint8_t),
    ]
class random_type_1374(Structure):
    _fields_ = [
        ('header', kv_key_header_t),
        ('key_location', kv_key_location),
    ]
class random_type_1375(Structure):
    _fields_ = [
        ('magic', uint8_t),
        ('valid', uint8_t),
        ('length', uint16_t),
        ('type', uint8_t),
        ('upgrade', uint8_t),
        ('key_id', uint16_t),
        ('enc_key', uint16_t),
        ('version', uint16_t),
        ('rnd', uint32_t),
    ]
class random_type_1377(Structure):
    _fields_ = [
        ('dupFunc', CstlDupFunc),
        ('freeFunc', CstlFreeFunc),
    ]
class random_type_1378(Structure):
    _fields_ = [
        ('handler', osal_kthread_handler),
        ('taskprio', td_u32),
        ('stacksize', td_u32),
        ('taskname', td_u32),
        ('data', td_u32),
    ]
class random_type_1379(Structure):
    _fields_ = [
        ('task', td_u32),
    ]
class random_type_1381(Structure):
    _fields_ = [
        ('addr', uint32_t),
        ('dir', uint32_t,1),
        ('rsv', uint32_t,31),
        ('id', uint32_t,8),
        ('size', uint32_t,24),
        ('extra', uint32_t),
    ]
partition_info_t=random_type_1381
class random_type_1380(Structure):
    _fields_ = [
        ('image_id', uint32_t),
        ('struct_ver', uint32_t),
        ('params_area_ver', uint32_t),
        ('partition_cnt', uint32_t),
        ('partition_tbl', (partition_info_t*20)),
    ]
partition_t=random_type_1380
class random_type_1382(Structure):
    _fields_ = [
        ('image_id', uint32_t),
        ('struct_ver', uint32_t),
        ('params_area_ver', uint32_t),
        ('param_item_offset', uint16_t),
        ('param_item_count', uint8_t),
    ]
params_area_t=random_type_1382
class random_type_1383(Structure):
    _fields_ = [
        ('addr', uint32_t),
        ('size', uint32_t),
    ]
param_area_data_t=random_type_1383
class random_type_1384(Structure):
    _fields_ = [
        ('item_id', uint32_t),
        ('file_path', td_u32),
    ]
uapi_upg_progress_cb=td_u32
class upg_package_info(Structure):
    _fields_ = [
        ('total_new_fw_size', uint32_t),
        ('finished_fw_size', uint32_t),
    ]
upg_package_info_t=upg_package_info
uapi_upg_user_defined_check=td_u32
class random_type_1389(Structure):
    _fields_ = [
        ('length', uint32_t),
        ('data', td_u32),
    ]
class upg_auth_data(Structure):
    _fields_ = [
        ('length', uint32_t),
        ('data', td_u32),
    ]
upg_auth_data_t=upg_auth_data
class random_type_1390(Structure):
    _fields_ = [
        ('n', td_u32),
        ('e', td_u32),
        ('len', uint16_t),
    ]
uapi_upg_write_done_cb=td_u32
SizeT=td_u32
Byte=td_uchar
UInt32=td_u32
class _CLzmaProps(Structure):
    _fields_ = [
        ('lc', Byte),
        ('lp', Byte),
        ('pb', Byte),
        ('_pad_', Byte),
        ('dicSize', UInt32),
    ]
CLzmaProps=_CLzmaProps
class random_type_1391(Structure):
    _fields_ = [
        ('prop', CLzmaProps),
        ('probs', td_u32),
        ('probs_1664', td_u32),
        ('dic', td_u32),
        ('dicBufSize', SizeT),
        ('dicPos', SizeT),
        ('buf', td_u32),
        ('range', UInt32),
        ('code', UInt32),
        ('processedPos', UInt32),
        ('checkDicSize', UInt32),
        ('reps', (UInt32*4)),
        ('state', UInt32),
        ('remainLen', UInt32),
        ('numProbs', UInt32),
        ('tempBufSize', td_u32),
        ('tempBuf', (Byte*20)),
    ]
CLzmaDec=random_type_1391
CLzmaProb=td_u16
UInt16=td_u16
ISzAllocPtr=td_u32
class ISzAlloc(Structure):
    _fields_ = [
        ('Alloc', td_u32),
        ('Free', td_u32),
    ]
ISzAlloc=ISzAlloc
SRes=td_s32
class upg_lzma_decode2_data(Structure):
    _fields_ = [
        ('inbuf', td_u32),
        ('outbuf', td_u32),
        ('image_id', uint32_t),
        ('in_offset', uint32_t),
        ('out_offset', uint32_t),
        ('compress_len', uint32_t),
        ('decompress_len', uint32_t),
    ]
upg_lzma_decode2_data_t=upg_lzma_decode2_data
class upg_resource_index(Structure):
    _fields_ = [
        ('file_path', (td_char*128)),
        ('offset', uint32_t),
        ('file_len', uint32_t),
        ('operation_type', uint32_t),
    ]
upg_resource_node_t=upg_resource_index
class patch_state(Structure):
    _fields_ = [
        ('desc', td_u32),
        ('err_code', errcode_t),
        ('lzma_alloc', ISzAlloc),
        ('local_buffer', td_u32),
        ('local_buffer_page', int32_t),
        ('page_first_written', int32_t),
        ('page_last_written', int32_t),
        ('done_skipping', td_bool),
    ]
patch_state_t=patch_state
failfunc=td_u32
fetch_image_contents=td_u32
prep_image_contents_for_write=td_u32
copy_recovered_buffer_to_flash_cache=td_u32
plaintext_flash_cache_init=td_u32
encrypt_one_flash_page=td_u32
class patch_description(Structure):
    _fields_ = [
        ('image_id', uint32_t),
        ('maxsize', int32_t),
        ('newsize', int32_t),
        ('oldsize', int32_t),
        ('num_new_pages', uint32_t),
        ('num_old_pages', uint32_t),
        ('num_maxsize_pages', uint32_t),
        ('new_sig_page', uint32_t),
        ('old_sig_page', uint32_t),
        ('failpoint', int32_t),
        ('failfn', failfunc),
        ('patch_contents_ram_copy', uint32_t),
        ('patch_contents_flash_offset', uint32_t),
        ('patch_contents_len', uint32_t),
        ('bottom_up', td_bool),
        ('image_flash_offset', uint32_t),
        ('image_flash_length', uint32_t),
        ('buffers_flash_offset', uint32_t),
        ('buffers_length', uint32_t),
        ('fetch_image_contents_fn', fetch_image_contents),
        ('prep_image_contents_for_write_fn', prep_image_contents_for_write),
        ('copy_recovered_buffer_to_flash_cache_fn', copy_recovered_buffer_to_flash_cache),
        ('plaintext_flash_cache_init_fn', plaintext_flash_cache_init),
        ('encrypt_flash_page_fn', encrypt_one_flash_page),
        ('use_plain_text_cache', td_bool),
        ('image_cache', td_u32),
        ('image_encrypted', td_bool),
        ('image_hdrs_len', uint16_t),
        ('image_signature_size', uint16_t),
        ('key_area_aes_params', upg_key_area_data_t),
    ]
patch=patch_description
class process_patch_state(Structure):
    _fields_ = [
        ('recovery_buffer', td_u32),
        ('recovery_found', td_bool),
    ]
process_patch_state_t=process_patch_state
class fota_buffers(Structure):
    _fields_ = [
        ('page_buffer', (uint8_t*4096)),
        ('page_status', (uint8_t*2048)),
    ]
fota_buffers_t=fota_buffers
class zip_context(Structure):
    _fields_ = [
        ('dec', td_u32),
        ('offset', int32_t),
        ('cdata', td_u32),
        ('unpacked_len', size_t),
        ('unpacked_so_far', size_t),
        ('cdata_len', size_t),
    ]
zip_context_t=zip_context
class random_type_1392(Structure):
    _fields_ = [
        ('prop', CLzmaProps),
        ('probs', td_u32),
        ('probs_1664', td_u32),
        ('dic', td_u32),
        ('dicBufSize', SizeT),
        ('dicPos', SizeT),
        ('buf', td_u32),
        ('range', UInt32),
        ('code', UInt32),
        ('processedPos', UInt32),
        ('checkDicSize', UInt32),
        ('reps', (UInt32*4)),
        ('state', UInt32),
        ('remainLen', UInt32),
        ('numProbs', UInt32),
        ('tempBufSize', td_u32),
        ('tempBuf', (Byte*20)),
    ]
class control_block(Structure):
    _fields_ = [
        ('magic', uint32_t),
        ('copy', uint32_t),
        ('extra', uint32_t),
        ('seek', int32_t),
    ]
control_block_t=control_block
class apply_patch_state(Structure):
    _fields_ = [
        ('zcontext', td_u32),
        ('unpackedbuf', td_u32),
        ('cb', control_block_t),
        ('cerror', int32_t),
        ('lenread', uint32_t),
        ('oldpos', int32_t),
        ('newpos', int32_t),
        ('blockcount', int32_t),
        ('chunk_start', int32_t),
        ('chunk_size', int32_t),
    ]
apply_patch_state_t=apply_patch_state
class random_type_1395(Structure):
    _fields_ = [
        ('prop', CLzmaProps),
        ('probs', td_u32),
        ('probs_1664', td_u32),
        ('dic', td_u32),
        ('dicBufSize', SizeT),
        ('dicPos', SizeT),
        ('buf', td_u32),
        ('range', UInt32),
        ('code', UInt32),
        ('processedPos', UInt32),
        ('checkDicSize', UInt32),
        ('reps', (UInt32*4)),
        ('state', UInt32),
        ('remainLen', UInt32),
        ('numProbs', UInt32),
        ('tempBufSize', td_u32),
        ('tempBuf', (Byte*20)),
    ]
class upg_resource_head(Structure):
    _fields_ = [
        ('file_num', uint32_t),
        ('file_node', (upg_resource_node_t*0)),
    ]
upg_resource_index_t=upg_resource_head
class random_type_1396(Structure):
    _fields_ = [
        ('prop', CLzmaProps),
        ('probs', td_u32),
        ('probs_1664', td_u32),
        ('dic', td_u32),
        ('dicBufSize', SizeT),
        ('dicPos', SizeT),
        ('buf', td_u32),
        ('range', UInt32),
        ('code', UInt32),
        ('processedPos', UInt32),
        ('checkDicSize', UInt32),
        ('reps', (UInt32*4)),
        ('state', UInt32),
        ('remainLen', UInt32),
        ('numProbs', UInt32),
        ('tempBufSize', td_u32),
        ('tempBuf', (Byte*20)),
    ]
class random_type_1397(Structure):
    _fields_ = [
        ('prop', CLzmaProps),
        ('probs', td_u32),
        ('probs_1664', td_u32),
        ('dic', td_u32),
        ('dicBufSize', SizeT),
        ('dicPos', SizeT),
        ('buf', td_u32),
        ('range', UInt32),
        ('code', UInt32),
        ('processedPos', UInt32),
        ('checkDicSize', UInt32),
        ('reps', (UInt32*4)),
        ('state', UInt32),
        ('remainLen', UInt32),
        ('numProbs', UInt32),
        ('tempBufSize', td_u32),
        ('tempBuf', (Byte*20)),
    ]
BoolInt=td_s32
ble_at_dis_cmd_handle_func=td_u32
class random_type_1398(Structure):
    _fields_ = [
        ('id', td_s32),
        ('func', ble_at_dis_cmd_handle_func),
    ]
ble_at_dis_srv_cmd_hdl_info=random_type_1398
class random_type_1399(Structure):
    _fields_ = [
        ('id', td_s32),
        ('func', ble_at_dis_cmd_handle_func),
    ]
ble_at_dis_cln_cmd_hdl_info=random_type_1399
class random_type_1400(Structure):
    _fields_ = [
        ('vendor_id_source', uint8_t),
        ('vendor_id', uint16_t),
        ('product_id', uint16_t),
        ('product_revision', uint16_t),
    ]
ble_pro_dis_pnp_id=random_type_1400
ble_at_bas_cmd_handle_func_t=td_u32
class random_type_1401(Structure):
    _fields_ = [
        ('id', uint8_t),
        ('func', ble_at_bas_cmd_handle_func_t),
    ]
ble_at_bas_srv_cmd_hdl_info_t=random_type_1401
class random_type_1402(Structure):
    _fields_ = [
        ('id', uint8_t),
        ('func', ble_at_bas_cmd_handle_func_t),
    ]
ble_at_bas_cln_cmd_hdl_info_t=random_type_1402
class random_type_1403(Structure):
    _fields_ = [
        ('format', uint8_t),
        ('exponent', uint8_t),
        ('unit', bt_uuid_t),
        ('name_space', uint8_t),
        ('description', bt_uuid_t),
    ]
class random_type_1404(Structure):
    _fields_ = [
        ('uuid_len', uint8_t),
        ('uuid', (uint8_t*16)),
    ]
bas_client_cfd_t=random_type_1403
ble_at_hid_cmd_handle_func=td_u32
class random_type_1405(Structure):
    _fields_ = [
        ('id', uint8_t),
        ('func', ble_at_hid_cmd_handle_func),
    ]
ble_hid_at_cmd_hdl_info_t=random_type_1405
ble_at_fmp_cmd_handle_func=td_u32
class random_type_1406(Structure):
    _fields_ = [
        ('id', td_s32),
        ('func', ble_at_fmp_cmd_handle_func),
    ]
ble_at_fmp_cmd_info_t=random_type_1406
fmp_target_evt_handler_t=td_u32
ble_ancs_client_at_func_t=td_u32
class random_type_1408(Structure):
    _fields_ = [
        ('att_id', uint8_t),
        ('att_length', uint16_t),
        ('att', td_u32),
    ]
ble_ancs_attr_rsp_t=random_type_1408
class random_type_1409(Structure):
    _fields_ = [
        ('att_id', uint8_t),
        ('att_max_length', uint16_t),
    ]
ble_ancs_attr_req_t=random_type_1409
ancs_client_setup_result_cbk=td_u32
ancs_client_subscribe_result_cbk=td_u32
ancs_client_notify_source_cbk=td_u32
ancs_client_notification_attr_rsp_cbk=td_u32
ancs_client_app_attr_rsp_cbk=td_u32
ancs_client_write_result_cbk=td_u32
class random_type_1410(Structure):
    _fields_ = [
        ('setup_cb', ancs_client_setup_result_cbk),
        ('subscribe_cb', ancs_client_subscribe_result_cbk),
        ('notify_source_cb', ancs_client_notify_source_cbk),
        ('notify_attr_cb', ancs_client_notification_attr_rsp_cbk),
        ('app_attr_cb', ancs_client_app_attr_rsp_cbk),
        ('write_result_cb', ancs_client_write_result_cbk),
    ]
class random_type_1411(Structure):
    _fields_ = [
        ('event_id', uint8_t),
        ('event_flags', uint8_t),
        ('category_id', uint8_t),
        ('category_count', uint8_t),
        ('notification_uid', uint32_t),
    ]
ble_ancs_notification_src_t=random_type_1411
class osal_list_head(Structure):
    _fields_ = [
        ('next', td_u32),
        ('prev', td_u32),
    ]
class random_type_1412(Structure):
    _fields_ = [
        ('notification_uid', uint32_t),
        ('att_num', uint16_t),
        ('attr_rsp_lst', osal_list_head),
    ]
ble_ancs_notification_attr_rsp_t=random_type_1412
class random_type_1414(Structure):
    _fields_ = [
        ('len', uint16_t),
        ('id', td_u32),
    ]
ble_ancs_app_id_t=random_type_1414
class random_type_1413(Structure):
    _fields_ = [
        ('app_id', ble_ancs_app_id_t),
        ('att_num', uint16_t),
        ('attr_rsp_lst', osal_list_head),
    ]
ble_ancs_app_attr_rsp_t=random_type_1413
class random_type_1415(Structure):
    _fields_ = [
        ('notification_uid', uint32_t),
        ('att_num', uint16_t),
        ('attr', (ble_ancs_attr_req_t*0)),
    ]
class random_type_1416(Structure):
    _fields_ = [
        ('app_id', ble_ancs_app_id_t),
        ('att_num', uint16_t),
        ('attr_id', (uint8_t*0)),
    ]
ble_ancs_app_attr_req_t=random_type_1416
class random_type_1417(Structure):
    _fields_ = [
        ('notification_uid', uint32_t),
        ('action_id', uint8_t),
    ]
ble_ancs_client_callbacks_t=random_type_1410
ble_ancs_notification_attr_req_t=random_type_1415
ble_ancs_notification_action_req_t=random_type_1417
class random_type_1418(Structure):
    _fields_ = [
        ('cmd_num', uint16_t),
        ('remote_cmd', (uint8_t*0)),
    ]
ble_ams_remote_cmd_rpt_t=random_type_1418
ble_ams_client_at_func_t=td_u32
ams_client_setup_result_cbk=td_u32
ams_client_subscribe_result_cbk=td_u32
ams_client_remote_command_rpt_cbk=td_u32
ams_client_entity_update_rpt_cbk=td_u32
ams_client_write_character_rsp_cbk=td_u32
ams_client_read_full_attr_result_cbk=td_u32
class random_type_1420(Structure):
    _fields_ = [
        ('setup_cb', ams_client_setup_result_cbk),
        ('subscribe_cb', ams_client_subscribe_result_cbk),
        ('remote_cmd_cb', ams_client_remote_command_rpt_cbk),
        ('entity_update_cb', ams_client_entity_update_rpt_cbk),
        ('write_rsp_cb', ams_client_write_character_rsp_cbk),
        ('read_full_attr_result_cb', ams_client_read_full_attr_result_cbk),
    ]
class random_type_1421(Structure):
    _fields_ = [
        ('entity_id', uint8_t),
        ('att_id', uint8_t),
        ('entity_update_flags', uint8_t),
        ('value_length', uint16_t),
        ('value', (uint8_t*0)),
    ]
ble_ams_entity_update_rpt_t=random_type_1421
class random_type_1422(Structure):
    _fields_ = [
        ('entity_id', uint8_t),
        ('att_id', uint8_t),
        ('value_len', uint16_t),
        ('value', (uint8_t*0)),
    ]
ble_ams_full_attr_value_t=random_type_1422
class random_type_1423(Structure):
    _fields_ = [
        ('entity_id', uint8_t),
        ('att_num', uint16_t),
        ('att_id', (uint8_t*0)),
    ]
class random_type_1424(Structure):
    _fields_ = [
        ('entity_id', uint8_t),
        ('att_id', uint8_t),
    ]
ble_ams_client_callbacks_t=random_type_1420
ble_ams_interested_entity_attr_req_t=random_type_1423
ble_ams_full_entity_attr_cmd_t=random_type_1424
class random_type_1425(Structure):
    _fields_ = [
        ('system_id', td_u32),
        ('model_number', td_u32),
        ('serial_number', td_u32),
        ('firmware_revision', td_u32),
        ('hardware_revision', td_u32),
        ('software_revision', td_u32),
        ('manufacturer_name', td_u32),
        ('ieee_regulatory_certification_data_list', td_u32),
        ('pnp_id', ble_pro_dis_pnp_id),
    ]
ble_pro_dis_data=random_type_1425
class random_type_1426(Structure):
    _fields_ = [
        ('vendor_id_source', uint8_t),
        ('vendor_id', uint16_t),
        ('product_id', uint16_t),
        ('product_revision', uint16_t),
    ]
class random_type_1427(Structure):
    _fields_ = [
        ('chara_uuid', bt_uuid_t),
        ('permissions', uint8_t),
        ('properties', uint8_t),
        ('value_len', uint16_t),
        ('value', td_u32),
    ]
gatts_add_chara_info_t=random_type_1427
class random_type_1428(Structure):
    _fields_ = [
        ('uuid_len', uint8_t),
        ('uuid', (uint8_t*16)),
    ]
dis_client_read_character_cbk=td_u32
class dis_client_character_info(Structure):
    _fields_ = [
        ('status', uint8_t),
        ('handle', uint16_t),
        ('cbk', dis_client_read_character_cbk),
    ]
dis_client_character_info_t=dis_client_character_info
class ble_dis_client_remote_dev(Structure):
    _fields_ = [
        ('node', osal_list_head),
        ('client_id', uint8_t),
        ('conn_id', uint16_t),
        ('att_mtu', uint16_t),
        ('start_hdl', uint16_t),
        ('end_hdl', uint16_t),
        ('character', (dis_client_character_info_t*9)),
    ]
ble_dis_client_remote_dev_t=ble_dis_client_remote_dev
class random_type_1429(Structure):
    _fields_ = [
        ('uuid_len', uint8_t),
        ('uuid', (uint8_t*16)),
    ]
class random_type_1430(Structure):
    _fields_ = [
        ('service_handle', uint16_t),
        ('uuid', bt_uuid_t),
    ]
gattc_discovery_character_param_t=random_type_1430
class random_type_1431(Structure):
    _fields_ = [
        ('start_hdl', uint16_t),
        ('end_hdl', uint16_t),
        ('uuid', bt_uuid_t),
    ]
gattc_discovery_service_result_t=random_type_1431
class random_type_1432(Structure):
    _fields_ = [
        ('uuid', bt_uuid_t),
        ('declare_handle', uint16_t),
        ('value_handle', uint16_t),
        ('properties', uint8_t),
    ]
gattc_discovery_character_result_t=random_type_1432
class random_type_1433(Structure):
    _fields_ = [
        ('handle', uint16_t),
        ('data_len', uint16_t),
        ('data', td_u32),
    ]
gattc_handle_value_t=random_type_1433
ble_bas_func_result_cbk_t=td_u32
ble_bas_notify_cbk_t=td_u32
ble_bas_character_read_cbk_t=td_u32
ble_bas_cfd_read_cbk_t=td_u32
class random_type_1436(Structure):
    _fields_ = [
        ('descriptor_handle', uint16_t),
        ('cfd', bas_client_cfd_t),
        ('cbk', ble_bas_cfd_read_cbk_t),
    ]
bas_client_cfd_info_t=random_type_1436
ble_bas_cccd_read_cbk_t=td_u32
class random_type_1439(Structure):
    _fields_ = [
        ('descriptor_handle', uint16_t),
        ('value', uint16_t),
        ('cbk', ble_bas_cccd_read_cbk_t),
    ]
bas_client_cccd_info_t=random_type_1439
class random_type_1435(Structure):
    _fields_ = [
        ('declare_handle', uint16_t),
        ('value_handle', uint16_t),
        ('value', uint8_t),
        ('notify_cbk', ble_bas_notify_cbk_t),
        ('read_cbk', ble_bas_character_read_cbk_t),
        ('cfd_info', bas_client_cfd_info_t),
        ('cccd_info', bas_client_cccd_info_t),
        ('properties', uint8_t),
    ]
bas_client_character_info_t=random_type_1435
class random_type_1434(Structure):
    _fields_ = [
        ('node', osal_list_head),
        ('client_id', uint8_t),
        ('conn_id', uint16_t),
        ('start_hdl', uint16_t),
        ('end_hdl', uint16_t),
        ('character', bas_client_character_info_t),
        ('cbk', ble_bas_func_result_cbk_t),
    ]
ble_bas_client_t=random_type_1434
class random_type_1437(Structure):
    _fields_ = [
        ('format', uint8_t),
        ('exponent', uint8_t),
        ('unit', bt_uuid_t),
        ('name_space', uint8_t),
        ('description', bt_uuid_t),
    ]
class random_type_1438(Structure):
    _fields_ = [
        ('uuid_len', uint8_t),
        ('uuid', (uint8_t*16)),
    ]
class random_type_1440(Structure):
    _fields_ = [
        ('handle', uint16_t),
        ('data_len', uint16_t),
        ('data', td_u32),
    ]
class random_type_1441(Structure):
    _fields_ = [
        ('service_handle', uint16_t),
        ('uuid', bt_uuid_t),
    ]
class random_type_1442(Structure):
    _fields_ = [
        ('start_hdl', uint16_t),
        ('end_hdl', uint16_t),
        ('uuid', bt_uuid_t),
    ]
class random_type_1443(Structure):
    _fields_ = [
        ('uuid', bt_uuid_t),
        ('declare_handle', uint16_t),
        ('value_handle', uint16_t),
        ('properties', uint8_t),
    ]
class random_type_1444(Structure):
    _fields_ = [
        ('descriptor_hdl', uint16_t),
        ('uuid', bt_uuid_t),
    ]
gattc_discovery_descriptor_result_t=random_type_1444
class random_type_1445(Structure):
    _fields_ = [
        ('battery_level', uint8_t),
        ('start_hdl', uint16_t),
        ('cbk', ble_bas_func_result_cbk_t),
    ]
ble_bas_data_t=random_type_1445
class random_type_1446(Structure):
    _fields_ = [
        ('uuid_len', uint8_t),
        ('uuid', (uint8_t*16)),
    ]
class random_type_1447(Structure):
    _fields_ = [
        ('chara_uuid', bt_uuid_t),
        ('permissions', uint8_t),
        ('properties', uint8_t),
        ('value_len', uint16_t),
        ('value', td_u32),
    ]
class random_type_1448(Structure):
    _fields_ = [
        ('handle', uint16_t),
        ('value_handle', uint16_t),
    ]
class random_type_1449(Structure):
    _fields_ = [
        ('desc_uuid', bt_uuid_t),
        ('permissions', uint8_t),
        ('value_len', uint16_t),
        ('value', td_u32),
    ]
gatts_add_desc_info_t=random_type_1449
class random_type_1450(Structure):
    _fields_ = [
        ('attr_handle', uint16_t),
        ('value_len', uint16_t),
        ('value', td_u32),
    ]
class random_type_1451(Structure):
    _fields_ = [
        ('chara_uuid', bt_uuid_t),
        ('permissions', uint8_t),
        ('properties', uint8_t),
        ('value_len', uint16_t),
        ('value', td_u32),
    ]
class random_type_1452(Structure):
    _fields_ = [
        ('uuid_len', uint8_t),
        ('uuid', (uint8_t*16)),
    ]
class random_type_1453(Structure):
    _fields_ = [
        ('desc_uuid', bt_uuid_t),
        ('permissions', uint8_t),
        ('value_len', uint16_t),
        ('value', td_u32),
    ]
class random_type_1454(Structure):
    _fields_ = [
        ('attr_handle', uint16_t),
        ('value_len', uint16_t),
        ('value', td_u32),
    ]
class fmp_target_ias(Structure):
    _fields_ = [
        ('evt_handler', fmp_target_evt_handler_t),
        ('service_handle', uint32_t),
        ('srvc_att_hdl', uint16_t),
        ('chara_hdl', uint16_t),
        ('chara_value_hdl', uint16_t),
        ('is_srvc_started', td_bool),
    ]
fmp_target_ias_t=fmp_target_ias
class random_type_1455(Structure):
    _fields_ = [
        ('uuid_len', uint8_t),
        ('uuid', (uint8_t*16)),
    ]
class random_type_1456(Structure):
    _fields_ = [
        ('chara_uuid', bt_uuid_t),
        ('permissions', uint8_t),
        ('properties', uint8_t),
        ('value_len', uint16_t),
        ('value', td_u32),
    ]
class random_type_1457(Structure):
    _fields_ = [
        ('node', osal_list_head),
        ('client_id', uint8_t),
        ('conn_id', uint16_t),
        ('srvc_hdl', uint16_t),
        ('alc_hdl', uint16_t),
        ('alc_value_hdl', uint16_t),
    ]
fmp_locator_remote_dev_t=random_type_1457
class random_type_1458(Structure):
    _fields_ = [
        ('uuid_len', uint8_t),
        ('uuid', (uint8_t*16)),
    ]
class random_type_1459(Structure):
    _fields_ = [
        ('handle', uint16_t),
        ('data_len', uint16_t),
        ('data', td_u32),
    ]
class random_type_1460(Structure):
    _fields_ = [
        ('service_handle', uint16_t),
        ('uuid', bt_uuid_t),
    ]
class random_type_1461(Structure):
    _fields_ = [
        ('start_hdl', uint16_t),
        ('end_hdl', uint16_t),
        ('uuid', bt_uuid_t),
    ]
class random_type_1462(Structure):
    _fields_ = [
        ('uuid', bt_uuid_t),
        ('declare_handle', uint16_t),
        ('value_handle', uint16_t),
        ('properties', uint8_t),
    ]
class random_type_1463(Structure):
    _fields_ = [
        ('add_service_cb', gatts_add_service_callback),
        ('add_characteristic_cb', gatts_add_characteristic_callback),
        ('add_descriptor_cb', gatts_add_descriptor_callback),
        ('start_service_cb', gatts_start_service_callback),
        ('stop_service_cb', gatts_stop_service_callback),
        ('delete_service_cb', gatts_delete_all_service_callback),
        ('read_request_cb', gatts_read_request_callback),
        ('write_request_cb', gatts_write_request_callback),
        ('mtu_changed_cb', gatts_mtu_changed_callback),
    ]
class random_type_1464(Structure):
    _fields_ = [
        ('uuid_len', uint8_t),
        ('uuid', (uint8_t*16)),
    ]
class random_type_1465(Structure):
    _fields_ = [
        ('handle', uint16_t),
        ('value_handle', uint16_t),
    ]
class random_type_1466(Structure):
    _fields_ = [
        ('request_id', uint16_t),
        ('handle', uint16_t),
        ('offset', uint16_t),
        ('need_rsp', td_bool),
        ('need_authorize', td_bool),
        ('is_long', td_bool),
    ]
class random_type_1467(Structure):
    _fields_ = [
        ('request_id', uint16_t),
        ('handle', uint16_t),
        ('offset', uint16_t),
        ('need_rsp', td_bool),
        ('need_authorize', td_bool),
        ('is_prep', td_bool),
        ('length', uint16_t),
        ('value', td_u32),
    ]
gattc_discovery_service_callback=td_u32
gattc_discovery_service_complete_callback=td_u32
gattc_discovery_character_callback=td_u32
gattc_discovery_character_complete_callback=td_u32
gattc_discovery_descriptor_callback=td_u32
gattc_discovery_descriptor_complete_callback=td_u32
gattc_read_cfm_callback=td_u32
gattc_read_by_uuid_complete_callback=td_u32
gattc_write_cfm_callback=td_u32
gattc_mtu_changed_callback=td_u32
gattc_notification_callback=td_u32
gattc_indication_callback=td_u32
class ble_gattc_callbacks(Structure):
    _fields_ = [
        ('discovery_svc_cb', gattc_discovery_service_callback),
        ('discovery_svc_cmp_cb', gattc_discovery_service_complete_callback),
        ('discovery_chara_cb', gattc_discovery_character_callback),
        ('discovery_chara_cmp_cb', gattc_discovery_character_complete_callback),
        ('discovery_desc_cb', gattc_discovery_descriptor_callback),
        ('discovery_desc_cmp_cb', gattc_discovery_descriptor_complete_callback),
        ('read_cb', gattc_read_cfm_callback),
        ('read_cmp_cb', gattc_read_by_uuid_complete_callback),
        ('write_cb', gattc_write_cfm_callback),
        ('mtu_changed_cb', gattc_mtu_changed_callback),
        ('notification_cb', gattc_notification_callback),
        ('indication_cb', gattc_indication_callback),
    ]
class random_type_1468(Structure):
    _fields_ = [
        ('start_hdl', uint16_t),
        ('end_hdl', uint16_t),
        ('uuid', bt_uuid_t),
    ]
class random_type_1469(Structure):
    _fields_ = [
        ('uuid', bt_uuid_t),
        ('declare_handle', uint16_t),
        ('value_handle', uint16_t),
        ('properties', uint8_t),
    ]
class random_type_1470(Structure):
    _fields_ = [
        ('service_handle', uint16_t),
        ('uuid', bt_uuid_t),
    ]
class random_type_1471(Structure):
    _fields_ = [
        ('descriptor_hdl', uint16_t),
        ('uuid', bt_uuid_t),
    ]
class random_type_1472(Structure):
    _fields_ = [
        ('handle', uint16_t),
        ('data_len', uint16_t),
        ('data', td_u32),
    ]
class random_type_1473(Structure):
    _fields_ = [
        ('start_hdl', uint16_t),
        ('end_hdl', uint16_t),
        ('uuid', bt_uuid_t),
    ]
gattc_read_req_by_uuid_param_t=random_type_1473
gattc_callbacks_t=ble_gattc_callbacks
class random_type_1474(Structure):
    _fields_ = [
        ('setup_cb', ancs_client_setup_result_cbk),
        ('subscribe_cb', ancs_client_subscribe_result_cbk),
        ('notify_source_cb', ancs_client_notify_source_cbk),
        ('notify_attr_cb', ancs_client_notification_attr_rsp_cbk),
        ('app_attr_cb', ancs_client_app_attr_rsp_cbk),
        ('write_result_cb', ancs_client_write_result_cbk),
    ]
class random_type_1475(Structure):
    _fields_ = [
        ('event_id', uint8_t),
        ('event_flags', uint8_t),
        ('category_id', uint8_t),
        ('category_count', uint8_t),
        ('notification_uid', uint32_t),
    ]
class random_type_1476(Structure):
    _fields_ = [
        ('notification_uid', uint32_t),
        ('att_num', uint16_t),
        ('attr_rsp_lst', osal_list_head),
    ]
class random_type_1477(Structure):
    _fields_ = [
        ('app_id', ble_ancs_app_id_t),
        ('att_num', uint16_t),
        ('attr_rsp_lst', osal_list_head),
    ]
class random_type_1478(Structure):
    _fields_ = [
        ('len', uint16_t),
        ('id', td_u32),
    ]
class random_type_1479(Structure):
    _fields_ = [
        ('node', osal_list_head),
        ('client_id', uint8_t),
        ('conn_id', uint16_t),
        ('start_handle', uint16_t),
        ('end_handle', uint16_t),
        ('notify_src', gattc_discovery_character_result_t),
        ('ctl_point', gattc_discovery_character_result_t),
        ('data_src', gattc_discovery_character_result_t),
        ('notify_src_ccc_handle', uint16_t),
        ('data_src_ccc_handle', uint16_t),
    ]
ble_ancs_service_t=random_type_1479
class random_type_1480(Structure):
    _fields_ = [
        ('uuid', bt_uuid_t),
        ('declare_handle', uint16_t),
        ('value_handle', uint16_t),
        ('properties', uint8_t),
    ]
class random_type_1481(Structure):
    _fields_ = [
        ('uuid_len', uint8_t),
        ('uuid', (uint8_t*16)),
    ]
class random_type_1482(Structure):
    _fields_ = [
        ('att_id', uint8_t),
        ('att_length', uint16_t),
        ('att', td_u32),
    ]
class random_type_1483(Structure):
    _fields_ = [
        ('service_handle', uint16_t),
        ('uuid', bt_uuid_t),
    ]
class random_type_1484(Structure):
    _fields_ = [
        ('handle', uint16_t),
        ('data_len', uint16_t),
        ('data', td_u32),
    ]
class random_type_1485(Structure):
    _fields_ = [
        ('notification_uid', uint32_t),
        ('att_num', uint16_t),
        ('attr', (ble_ancs_attr_req_t*0)),
    ]
class random_type_1486(Structure):
    _fields_ = [
        ('att_id', uint8_t),
        ('att_max_length', uint16_t),
    ]
class random_type_1487(Structure):
    _fields_ = [
        ('app_id', ble_ancs_app_id_t),
        ('att_num', uint16_t),
        ('attr_id', (uint8_t*0)),
    ]
class random_type_1488(Structure):
    _fields_ = [
        ('start_hdl', uint16_t),
        ('end_hdl', uint16_t),
        ('uuid', bt_uuid_t),
    ]
class random_type_1489(Structure):
    _fields_ = [
        ('descriptor_hdl', uint16_t),
        ('uuid', bt_uuid_t),
    ]
class random_type_1490(Structure):
    _fields_ = [
        ('notification_uid', uint32_t),
        ('action_id', uint8_t),
    ]
class random_type_1491(Structure):
    _fields_ = [
        ('setup_cb', ams_client_setup_result_cbk),
        ('subscribe_cb', ams_client_subscribe_result_cbk),
        ('remote_cmd_cb', ams_client_remote_command_rpt_cbk),
        ('entity_update_cb', ams_client_entity_update_rpt_cbk),
        ('write_rsp_cb', ams_client_write_character_rsp_cbk),
        ('read_full_attr_result_cb', ams_client_read_full_attr_result_cbk),
    ]
class random_type_1492(Structure):
    _fields_ = [
        ('cmd_num', uint16_t),
        ('remote_cmd', (uint8_t*0)),
    ]
class random_type_1493(Structure):
    _fields_ = [
        ('entity_id', uint8_t),
        ('att_id', uint8_t),
        ('entity_update_flags', uint8_t),
        ('value_length', uint16_t),
        ('value', (uint8_t*0)),
    ]
class random_type_1494(Structure):
    _fields_ = [
        ('entity_id', uint8_t),
        ('att_id', uint8_t),
        ('value_len', uint16_t),
        ('value', (uint8_t*0)),
    ]
class random_type_1495(Structure):
    _fields_ = [
        ('node', osal_list_head),
        ('client_id', uint8_t),
        ('conn_id', uint16_t),
        ('start_handle', uint16_t),
        ('end_handle', uint16_t),
        ('remote_cmd', gattc_discovery_character_result_t),
        ('entity_update', gattc_discovery_character_result_t),
        ('entity_attr', gattc_discovery_character_result_t),
        ('remote_cmd_ccc_handle', uint16_t),
        ('entity_update_ccc_handle', uint16_t),
    ]
ble_ams_service_t=random_type_1495
class random_type_1496(Structure):
    _fields_ = [
        ('uuid', bt_uuid_t),
        ('declare_handle', uint16_t),
        ('value_handle', uint16_t),
        ('properties', uint8_t),
    ]
class random_type_1497(Structure):
    _fields_ = [
        ('uuid_len', uint8_t),
        ('uuid', (uint8_t*16)),
    ]
class random_type_1498(Structure):
    _fields_ = [
        ('service_handle', uint16_t),
        ('uuid', bt_uuid_t),
    ]
class random_type_1499(Structure):
    _fields_ = [
        ('handle', uint16_t),
        ('data_len', uint16_t),
        ('data', td_u32),
    ]
class random_type_1500(Structure):
    _fields_ = [
        ('start_hdl', uint16_t),
        ('end_hdl', uint16_t),
        ('uuid', bt_uuid_t),
    ]
class random_type_1501(Structure):
    _fields_ = [
        ('descriptor_hdl', uint16_t),
        ('uuid', bt_uuid_t),
    ]
class random_type_1502(Structure):
    _fields_ = [
        ('entity_id', uint8_t),
        ('att_num', uint16_t),
        ('att_id', (uint8_t*0)),
    ]
class random_type_1503(Structure):
    _fields_ = [
        ('entity_id', uint8_t),
        ('att_id', uint8_t),
    ]
class random_type_1504(Structure):
    _fields_ = [
        ('task', td_u32),
    ]
btsrv_sample_cmd_handle_func=td_u32
class random_type_1505(Structure):
    _fields_ = [
        ('id', uint32_t),
        ('func', btsrv_sample_cmd_handle_func),
    ]
btsrv_sample_cmd_hdl_info=random_type_1505
class random_type_1508(Structure):
    _fields_ = [
        ('uuid_len', uint8_t),
        ('uuid', (uint8_t*16)),
    ]
class random_type_1509(Structure):
    _fields_ = [
        ('addr', (uint8_t*6)),
        ('type', uint8_t),
    ]
class random_type_1510(Structure):
    _fields_ = [
        ('sample_mode', uint32_t),
        ('client_id', td_s32),
        ('data_len', td_s32),
        ('cycle_num', td_s32),
        ('delay_time', td_s32),
        ('is_round_print', td_s32),
        ('data', (uint8_t*0)),
    ]
sample_spp_write_task_params=random_type_1510
class random_type_1511(Structure):
    _fields_ = [
        ('receive_data_cb', spp_receive_data_callback),
        ('conn_state_changed_cb', spp_conn_state_changed_callback),
    ]
class random_type_1512(Structure):
    _fields_ = [
        ('id', uint32_t),
        ('func', btsrv_sample_cmd_handle_func),
    ]
pan_net_state_callback=td_u32
pan_rpt_data_callback=td_u32
class random_type_1513(Structure):
    _fields_ = [
        ('net_state_cb', pan_net_state_callback),
        ('rpt_data_cb', pan_rpt_data_callback),
    ]
pan_callbacks_t=random_type_1513
class random_type_1514(Structure):
    _fields_ = [
        ('addr', (uint8_t*6)),
        ('type', uint8_t),
    ]
pbap_sample_cmd_handle_func=td_u32
class random_type_1516(Structure):
    _fields_ = [
        ('bts_code', uint32_t),
        ('print_message', (td_char*20)),
    ]
sample_pbap_code_print=random_type_1516
class random_type_1517(Structure):
    _fields_ = [
        ('addr', (uint8_t*6)),
        ('type', uint8_t),
    ]
class random_type_1518(Structure):
    _fields_ = [
        ('property_selector', (td_uchar*8)),
        ('vcard_selector', (td_uchar*8)),
        ('max_list_count', td_u16),
        ('list_offset', td_u16),
        ('name', td_u32),
        ('format', td_uchar),
        ('reset_new_missed_calls', td_uchar),
        ('vcard_selector_operator', td_uchar),
    ]
pbap_pce_pull_phone_book_param_t=random_type_1518
class random_type_1519(Structure):
    _fields_ = [
        ('vcard_selector', (td_uchar*8)),
        ('max_list_count', td_u16),
        ('list_offset', td_u16),
        ('name', td_u32),
        ('search_value', td_u32),
        ('search_property', td_uchar),
        ('order', td_uchar),
        ('reset_new_missed_calls', td_uchar),
        ('vcard_selector_operator', td_uchar),
    ]
pbap_pce_pull_vcard_listing_param_t=random_type_1519
class random_type_1520(Structure):
    _fields_ = [
        ('property_selector', (td_uchar*8)),
        ('name', td_u32),
        ('format', td_uchar),
    ]
pbap_pce_pull_vcard_entry_param_t=random_type_1520
pbap_pce_conn_state_changed_callback=td_u32
pbap_pce_phone_book_size_callback=td_u32
pbap_pce_trans_complete_callback=td_u32
pbap_pce_receive_data_callback=td_u32
pbap_pce_error_code_callback=td_u32
class random_type_1521(Structure):
    _fields_ = [
        ('state_changed_cb', pbap_pce_conn_state_changed_callback),
        ('phone_book_sized_cb', pbap_pce_phone_book_size_callback),
        ('trans_complete_cb', pbap_pce_trans_complete_callback),
        ('receive_data_cb', pbap_pce_receive_data_callback),
        ('error_code_cb', pbap_pce_error_code_callback),
    ]
class random_type_1522(Structure):
    _fields_ = [
        ('task', td_u32),
    ]
pbap_pce_callbacks_t=random_type_1521
class random_type_1523(Structure):
    _fields_ = [
        ('addr', (uint8_t*6)),
        ('type', uint8_t),
    ]
class random_type_1524(Structure):
    _fields_ = [
        ('addr', (uint8_t*6)),
        ('type', uint8_t),
    ]
class random_type_1525(Structure):
    _fields_ = [
        ('id', uint32_t),
        ('func', btsrv_sample_cmd_handle_func),
    ]
class random_type_1526(Structure):
    _fields_ = [
        ('node', osal_list_head),
        ('bd_addr', bd_addr_t),
        ('name', (uint8_t*32)),
        ('cod', int32_t),
        ('rssi', int32_t),
    ]
sample_found_dev_list_node=random_type_1526
class random_type_1527(Structure):
    _fields_ = [
        ('addr', bd_addr_t),
        ('device_name', (td_uchar*32)),
    ]
gap_paired_device_info_t=random_type_1527
gap_state_changed_callback=td_u32
gap_acl_state_changed_callback=td_u32
gap_scan_mode_changed_callback=td_u32
gap_pair_status_changed_callback=td_u32
gap_local_device_name_changed_callback=td_u32
gap_local_device_addr_changed_callback=td_u32
gap_pair_requested_callback=td_u32
gap_pair_confirmed_callback=td_u32
gap_discovery_state_changed_callback=td_u32
gap_discovery_result_callback=td_u32
gap_remote_uuid_changed_callback=td_u32
gap_remote_name_changed_callback=td_u32
gap_remote_alias_changed_callback=td_u32
gap_remote_cod_changed_callback=td_u32
gap_remote_battery_level_changed_callback=td_u32
gap_read_remote_rssi_event_callback=td_u32
gap_is_accept_conn_on_safe_mode_callback=td_u32
class random_type_1528(Structure):
    _fields_ = [
        ('state_change_callback', gap_state_changed_callback),
        ('acl_state_changed_callbak', gap_acl_state_changed_callback),
        ('scan_mode_changed_callback', gap_scan_mode_changed_callback),
        ('pair_status_changed_callback', gap_pair_status_changed_callback),
        ('device_name_changed_callback', gap_local_device_name_changed_callback),
        ('device_addr_changed_callback', gap_local_device_addr_changed_callback),
        ('pair_requested_callback', gap_pair_requested_callback),
        ('pair_confiremed_callback', gap_pair_confirmed_callback),
        ('discovery_state_changed_callback', gap_discovery_state_changed_callback),
        ('discovery_result_callback', gap_discovery_result_callback),
        ('remote_uuid_changed_callback', gap_remote_uuid_changed_callback),
        ('remote_name_changed_callback', gap_remote_name_changed_callback),
        ('remote_alias_changed_callback', gap_remote_alias_changed_callback),
        ('remote_cod_changed_callback', gap_remote_cod_changed_callback),
        ('remote_battery_level_changed_callback', gap_remote_battery_level_changed_callback),
        ('read_remote_rssi_event_callback', gap_read_remote_rssi_event_callback),
        ('is_accept_conn_on_safe_mode_callback', gap_is_accept_conn_on_safe_mode_callback),
    ]
class random_type_1529(Structure):
    _fields_ = [
        ('uuid_len', uint8_t),
        ('uuid', (uint8_t*16)),
    ]
class random_type_1530(Structure):
    _fields_ = [
        ('conn_handle', uint16_t),
        ('interval_min', uint16_t),
        ('interval_max', uint16_t),
        ('slave_latency', uint16_t),
        ('timeout_multiplier', uint16_t),
    ]
class random_type_1531(Structure):
    _fields_ = [
        ('bt_auto_reconnect_closed', uint32_t,1),
        ('bt_a2dp_snk_closed', uint32_t,1),
        ('reserved', uint32_t,30),
    ]
gap_call_backs_t=random_type_1528
gap_conn_param_update_t=random_type_1530
bts_feature_t=random_type_1531
ble_at_gatt_cmd_handle_func=td_u32
class random_type_1532(Structure):
    _fields_ = [
        ('id', uint32_t),
        ('func', ble_at_gatt_cmd_handle_func),
    ]
ble_at_gatt_cmd_hdl_info=random_type_1532
class random_type_1533(Structure):
    _fields_ = [
        ('addr', (uint8_t*6)),
        ('type', uint8_t),
    ]
class random_type_1534(Structure):
    _fields_ = [
        ('uuid_len', uint8_t),
        ('uuid', (uint8_t*16)),
    ]
class random_type_1535(Structure):
    _fields_ = [
        ('chara_uuid', bt_uuid_t),
        ('permissions', uint8_t),
        ('properties', uint8_t),
        ('value_len', uint16_t),
        ('value', td_u32),
    ]
class random_type_1536(Structure):
    _fields_ = [
        ('handle', uint16_t),
        ('value_handle', uint16_t),
    ]
class random_type_1537(Structure):
    _fields_ = [
        ('desc_uuid', bt_uuid_t),
        ('permissions', uint8_t),
        ('value_len', uint16_t),
        ('value', td_u32),
    ]
class random_type_1538(Structure):
    _fields_ = [
        ('attr_handle', uint16_t),
        ('value_len', uint16_t),
        ('value', td_u32),
    ]
class random_type_1539(Structure):
    _fields_ = [
        ('add_service_cb', gatts_add_service_callback),
        ('add_characteristic_cb', gatts_add_characteristic_callback),
        ('add_descriptor_cb', gatts_add_descriptor_callback),
        ('start_service_cb', gatts_start_service_callback),
        ('stop_service_cb', gatts_stop_service_callback),
        ('delete_service_cb', gatts_delete_all_service_callback),
        ('read_request_cb', gatts_read_request_callback),
        ('write_request_cb', gatts_write_request_callback),
        ('mtu_changed_cb', gatts_mtu_changed_callback),
    ]
class random_type_1540(Structure):
    _fields_ = [
        ('request_id', uint16_t),
        ('handle', uint16_t),
        ('offset', uint16_t),
        ('need_rsp', td_bool),
        ('need_authorize', td_bool),
        ('is_long', td_bool),
    ]
class random_type_1541(Structure):
    _fields_ = [
        ('request_id', uint16_t),
        ('handle', uint16_t),
        ('offset', uint16_t),
        ('need_rsp', td_bool),
        ('need_authorize', td_bool),
        ('is_prep', td_bool),
        ('length', uint16_t),
        ('value', td_u32),
    ]
class random_type_1542(Structure):
    _fields_ = [
        ('chara_uuid', bt_uuid_t),
        ('start_handle', uint16_t),
        ('end_handle', uint16_t),
        ('value_len', uint16_t),
        ('value', td_u32),
    ]
class random_type_1543(Structure):
    _fields_ = [
        ('service_handle', uint16_t),
        ('uuid', bt_uuid_t),
    ]
class random_type_1544(Structure):
    _fields_ = [
        ('start_hdl', uint16_t),
        ('end_hdl', uint16_t),
        ('uuid', bt_uuid_t),
    ]
class random_type_1545(Structure):
    _fields_ = [
        ('handle', uint16_t),
        ('data_len', uint16_t),
        ('data', td_u32),
    ]
class random_type_1546(Structure):
    _fields_ = [
        ('start_hdl', uint16_t),
        ('end_hdl', uint16_t),
        ('uuid', bt_uuid_t),
    ]
class random_type_1547(Structure):
    _fields_ = [
        ('uuid', bt_uuid_t),
        ('declare_handle', uint16_t),
        ('value_handle', uint16_t),
        ('properties', uint8_t),
    ]
class random_type_1548(Structure):
    _fields_ = [
        ('descriptor_hdl', uint16_t),
        ('uuid', bt_uuid_t),
    ]
class random_type_1549(Structure):
    _fields_ = [
        ('adv_length', uint16_t),
        ('adv_data', td_u32),
        ('scan_rsp_length', uint16_t),
        ('scan_rsp_data', td_u32),
    ]
class random_type_1550(Structure):
    _fields_ = [
        ('min_interval', uint32_t),
        ('max_interval', uint32_t),
        ('adv_type', uint8_t),
        ('own_addr', bd_addr_t),
        ('peer_addr', bd_addr_t),
        ('channel_map', uint8_t),
        ('adv_filter_policy', uint8_t),
        ('tx_power', uint32_t),
        ('duration', uint32_t),
    ]
class random_type_1551(Structure):
    _fields_ = [
        ('scan_interval', uint16_t),
        ('scan_window', uint16_t),
        ('scan_type', uint8_t),
        ('scan_phy', uint8_t),
        ('scan_filter_policy', uint8_t),
    ]
gap_ble_enable_callback=td_u32
gap_ble_disable_callback=td_u32
gap_ble_set_adv_data_callback=td_u32
gap_ble_set_adv_param_callback=td_u32
gap_ble_set_scan_param_callback=td_u32
gap_ble_start_adv_callback=td_u32
gap_ble_stop_adv_callback=td_u32
gap_ble_scan_result_callback=td_u32
gap_ble_connect_state_changed_callback=td_u32
gap_ble_paired_complete_callback=td_u32
gap_ble_read_rssi_callback=td_u32
gap_ble_terminate_adv_callback=td_u32
gap_ble_auth_complete_callback=td_u32
gap_ble_connect_param_update_callback=td_u32
class random_type_1552(Structure):
    _fields_ = [
        ('ble_enable_cb', gap_ble_enable_callback),
        ('ble_disable_cb', gap_ble_disable_callback),
        ('set_adv_data_cb', gap_ble_set_adv_data_callback),
        ('set_adv_param_cb', gap_ble_set_adv_param_callback),
        ('set_scan_param_cb', gap_ble_set_scan_param_callback),
        ('start_adv_cb', gap_ble_start_adv_callback),
        ('stop_adv_cb', gap_ble_stop_adv_callback),
        ('scan_result_cb', gap_ble_scan_result_callback),
        ('conn_state_change_cb', gap_ble_connect_state_changed_callback),
        ('pair_result_cb', gap_ble_paired_complete_callback),
        ('read_rssi_cb', gap_ble_read_rssi_callback),
        ('terminate_adv_cb', gap_ble_terminate_adv_callback),
        ('auth_complete_cb', gap_ble_auth_complete_callback),
        ('conn_param_update_cb', gap_ble_connect_param_update_callback),
    ]
class random_type_1553(Structure):
    _fields_ = [
        ('event_type', uint8_t),
        ('data_status', uint8_t),
        ('addr', bd_addr_t),
        ('primary_phy', uint8_t),
        ('secondary_phy', uint8_t),
        ('adv_sid', uint8_t),
        ('tx_power', int8_t),
        ('rssi', int8_t),
        ('periodic_adv_interval', uint16_t),
        ('direct_addr', bd_addr_t),
        ('adv_len', uint8_t),
        ('adv_data', td_u32),
    ]
gap_scan_result_data_t=random_type_1553
class random_type_1554(Structure):
    _fields_ = [
        ('ltk_len', uint8_t),
        ('ltk', (uint8_t*16)),
    ]
ble_auth_info_evt_t=random_type_1554
class random_type_1555(Structure):
    _fields_ = [
        ('interval', uint16_t),
        ('latency', uint16_t),
        ('timeout', uint16_t),
    ]
gap_ble_conn_param_update_t=random_type_1555
class random_type_1556(Structure):
    _fields_ = [
        ('conn_handle', uint16_t),
        ('all_phys', uint8_t),
        ('tx_phys', uint8_t),
        ('rx_phys', uint8_t),
        ('phy_options', uint16_t),
    ]
class random_type_1557(Structure):
    _fields_ = [
        ('conn_handle', uint16_t),
        ('maxtxoctets', uint16_t),
        ('maxtxtime', uint16_t),
    ]
class random_type_1558(Structure):
    _fields_ = [
        ('bondable', uint8_t),
        ('io_capability', uint8_t),
        ('sc_enable', uint8_t),
        ('sc_mode', uint8_t),
    ]
gatts_ntf_ind_by_uuid_t=random_type_1542
gap_ble_config_adv_data_t=random_type_1549
gap_ble_adv_params_t=random_type_1550
gap_ble_scan_params_t=random_type_1551
gap_ble_callbacks_t=random_type_1552
gap_le_set_phy_t=random_type_1556
gap_le_set_data_length_t=random_type_1557
gap_ble_sec_params_t=random_type_1558
class random_type_1560(Structure):
    _fields_ = [
        ('uuid_len', uint8_t),
        ('uuid', (uint8_t*16)),
    ]
btsrv_task_msg_handle_func_type=td_u32
class random_type_1561(Structure):
    _fields_ = [
        ('msg_id', uint32_t),
        ('param', (uintptr_t*4)),
    ]
ext_sys_queue_msg=random_type_1561
class random_type_1562(Structure):
    _fields_ = [
        ('task', td_u32),
    ]
errcode_bt_t=td_u32
class random_type_1563(Structure):
    _fields_ = [
        ('task_prio', uint16_t),
        ('stack_size', uint32_t),
        ('task_policy', uint32_t),
        ('task_nice', uint32_t),
        ('task_cpuid', uint32_t),
        ('task_name', td_u32),
        ('resved', td_u32),
    ]
btsrv_task_attr=random_type_1563
class list_stru(Structure):
    _fields_ = [
        ('head', td_u32),
        ('tail', td_u32),
    ]
class random_type_1565(Structure):
    _fields_ = [
        ('state_change_callback', gap_state_changed_callback),
        ('acl_state_changed_callbak', gap_acl_state_changed_callback),
        ('scan_mode_changed_callback', gap_scan_mode_changed_callback),
        ('pair_status_changed_callback', gap_pair_status_changed_callback),
        ('device_name_changed_callback', gap_local_device_name_changed_callback),
        ('device_addr_changed_callback', gap_local_device_addr_changed_callback),
        ('pair_requested_callback', gap_pair_requested_callback),
        ('pair_confiremed_callback', gap_pair_confirmed_callback),
        ('discovery_state_changed_callback', gap_discovery_state_changed_callback),
        ('discovery_result_callback', gap_discovery_result_callback),
        ('remote_uuid_changed_callback', gap_remote_uuid_changed_callback),
        ('remote_name_changed_callback', gap_remote_name_changed_callback),
        ('remote_alias_changed_callback', gap_remote_alias_changed_callback),
        ('remote_cod_changed_callback', gap_remote_cod_changed_callback),
        ('remote_battery_level_changed_callback', gap_remote_battery_level_changed_callback),
        ('read_remote_rssi_event_callback', gap_read_remote_rssi_event_callback),
        ('is_accept_conn_on_safe_mode_callback', gap_is_accept_conn_on_safe_mode_callback),
    ]
class random_type_1566(Structure):
    _fields_ = [
        ('addr', (uint8_t*6)),
        ('type', uint8_t),
    ]
class random_type_1567(Structure):
    _fields_ = [
        ('uuid_len', uint8_t),
        ('uuid', (uint8_t*16)),
    ]
class random_type_1568(Structure):
    _fields_ = [
        ('bt_discovery_status', uint8_t),
        ('deiniting', td_bool),
        ('exist_phone_connect', td_bool),
        ('exist_audio_dev_connect', td_bool),
        ('scan_mode', uint16_t),
        ('bt_broadcast_timer', osal_timer),
        ('phone_reconnect_timer', osal_timer),
        ('phone_reconnect_count', uint32_t),
        ('phone_reconnect_addr', bd_addr_t),
        ('conn_list_head', osal_list_head),
    ]
btsrv_connection_manager=random_type_1568
class random_type_1569(Structure):
    _fields_ = [
        ('timer', td_u32),
        ('handler', td_u32),
        ('data', td_u32),
        ('interval', td_u32),
    ]
class random_type_1570(Structure):
    _fields_ = [
        ('mutex', td_u32),
    ]
class random_type_1571(Structure):
    _fields_ = [
        ('node', osal_list_head),
        ('dev_hdl', uint32_t),
        ('conn_status', uint8_t),
        ('link_type', uint8_t),
        ('master', td_bool),
        ('phone', td_bool),
    ]
device_connection_list_node=random_type_1571
class random_type_1572(Structure):
    _fields_ = [
        ('conn_id', uint16_t),
        ('addr', bd_addr_t),
        ('pair_status', uint8_t),
        ('interval', uint16_t),
        ('latency', uint16_t),
        ('timeout', uint16_t),
        ('ltk_len', uint8_t),
        ('ltk', (uint8_t*16)),
    ]
gap_ble_conn_list_node=random_type_1572
class random_type_1574(Structure):
    _fields_ = [
        ('bd', (uint8_t*6)),
        ('atype', uint8_t),
        ('cap', uint8_t),
    ]
bth_address=random_type_1574
class random_type_1573(Structure):
    _fields_ = [
        ('addr', bth_address),
        ('ltk_len', uint8_t),
        ('ltk', (uint8_t*16)),
    ]
bth_smp_keys_info=random_type_1573
class random_type_1575(Structure):
    _fields_ = [
        ('ltk_len', uint8_t),
        ('ltk', (uint8_t*16)),
    ]
class random_type_1576(Structure):
    _fields_ = [
        ('rmt_dev_hdl', uint32_t),
        ('result', uint8_t),
    ]
bth_smp_pair_result=random_type_1576
class random_type_1578(Structure):
    _fields_ = [
        ('addr', (td_uchar*6)),
    ]
bth_addr_t=random_type_1578
class random_type_1577(Structure):
    _fields_ = [
        ('dev_hdl', uint32_t),
        ('dev_class', uint32_t),
        ('link_type', uint8_t),
        ('bd_addr', bth_addr_t),
    ]
connect_req_ind_struct=random_type_1577
class random_type_1579(Structure):
    _fields_ = [
        ('msg_id', uint32_t),
        ('param', (uintptr_t*4)),
    ]
class random_type_1580(Structure):
    _fields_ = [
        ('bd_addr', (uint8_t*6)),
        ('have_key', uint8_t),
        ('key_type', uint8_t),
        ('name', (uint8_t*32)),
        ('class_of_device', int32_t),
        ('link_key', (uint8_t*16)),
    ]
remote_device_info=random_type_1580
class remote_device_list_node(Structure):
    _fields_ = [
        ('node', osal_list_head),
        ('info', remote_device_info),
        ('dev_hdl', uint32_t),
        ('status', uint8_t),
        ('rssi', int8_t),
        ('pad', uint16_t),
    ]
remote_device_list_node=remote_device_list_node
class random_type_1581(Structure):
    _fields_ = [
        ('rmt_addr', bth_address),
        ('conn_handle', uint16_t),
        ('mtu_size', uint16_t),
        ('conn_state', uint8_t),
        ('os_type', uint8_t),
        ('pair_status', uint8_t),
        ('disc_reason', uint8_t),
    ]
bth_gap_conn_status_node=random_type_1581
class random_type_1582(Structure):
    _fields_ = [
        ('addr', bth_address),
        ('interval_min', uint16_t),
        ('interval_max', uint16_t),
        ('slave_latency', uint16_t),
        ('timeout_multiplier', uint16_t),
    ]
class random_type_1583(Structure):
    _fields_ = [
        ('conn_handle', uint16_t),
        ('interval_min', uint16_t),
        ('interval_max', uint16_t),
        ('slave_latency', uint16_t),
        ('timeout_multiplier', uint16_t),
    ]
bth_gap_connection_param=random_type_1582
class random_type_1584(Structure):
    _fields_ = [
        ('interval', uint16_t),
        ('latency', uint16_t),
        ('timeout', uint16_t),
    ]
class random_type_1585(Structure):
    _fields_ = [
        ('bt_auto_reconnect_closed', uint32_t,1),
        ('bt_a2dp_snk_closed', uint32_t,1),
        ('reserved', uint32_t,30),
    ]
class random_type_1587(Structure):
    _fields_ = [
        ('scan_param', gap_ble_scan_params_t),
        ('is_scanning', td_bool),
    ]
btsrv_ble_scan_info=random_type_1587
class random_type_1586(Structure):
    _fields_ = [
        ('adv_count', uint8_t),
        ('adv_info', list_stru),
        ('scan_info', btsrv_ble_scan_info),
    ]
btsrv_ble_adv_scan_manager=random_type_1586
class random_type_1588(Structure):
    _fields_ = [
        ('scan_interval', uint16_t),
        ('scan_window', uint16_t),
        ('scan_type', uint8_t),
        ('scan_phy', uint8_t),
        ('scan_filter_policy', uint8_t),
    ]
class random_type_1589(Structure):
    _fields_ = [
        ('bd_addr', (uint8_t*6)),
        ('factory_reset', td_bool),
        ('device_mode', uint8_t),
        ('name', (uint8_t*64)),
    ]
btsrv_local_device_info=random_type_1589
class random_type_1590(Structure):
    _fields_ = [
        ('ble_enable_cb', gap_ble_enable_callback),
        ('ble_disable_cb', gap_ble_disable_callback),
        ('set_adv_data_cb', gap_ble_set_adv_data_callback),
        ('set_adv_param_cb', gap_ble_set_adv_param_callback),
        ('set_scan_param_cb', gap_ble_set_scan_param_callback),
        ('start_adv_cb', gap_ble_start_adv_callback),
        ('stop_adv_cb', gap_ble_stop_adv_callback),
        ('scan_result_cb', gap_ble_scan_result_callback),
        ('conn_state_change_cb', gap_ble_connect_state_changed_callback),
        ('pair_result_cb', gap_ble_paired_complete_callback),
        ('read_rssi_cb', gap_ble_read_rssi_callback),
        ('terminate_adv_cb', gap_ble_terminate_adv_callback),
        ('auth_complete_cb', gap_ble_auth_complete_callback),
        ('conn_param_update_cb', gap_ble_connect_param_update_callback),
    ]
class random_type_1591(Structure):
    _fields_ = [
        ('event_type', uint8_t),
        ('data_status', uint8_t),
        ('addr', bd_addr_t),
        ('primary_phy', uint8_t),
        ('secondary_phy', uint8_t),
        ('adv_sid', uint8_t),
        ('tx_power', int8_t),
        ('rssi', int8_t),
        ('periodic_adv_interval', uint16_t),
        ('direct_addr', bd_addr_t),
        ('adv_len', uint8_t),
        ('adv_data', td_u32),
    ]
class random_type_1592(Structure):
    _fields_ = [
        ('addr', (uint8_t*6)),
        ('type', uint8_t),
    ]
class random_type_1593(Structure):
    _fields_ = [
        ('ltk_len', uint8_t),
        ('ltk', (uint8_t*16)),
    ]
class random_type_1594(Structure):
    _fields_ = [
        ('interval', uint16_t),
        ('latency', uint16_t),
        ('timeout', uint16_t),
    ]
class random_type_1595(Structure):
    _fields_ = [
        ('rmt_addr', bth_address),
        ('conn_handle', uint16_t),
        ('mtu_size', uint16_t),
        ('conn_state', uint8_t),
        ('os_type', uint8_t),
        ('pair_status', uint8_t),
        ('disc_reason', uint8_t),
    ]
class random_type_1596(Structure):
    _fields_ = [
        ('bd', (uint8_t*6)),
        ('atype', uint8_t),
        ('cap', uint8_t),
    ]
class random_type_1597(Structure):
    _fields_ = [
        ('dev_hdl', uint32_t),
        ('dev_class', uint32_t),
        ('link_type', uint8_t),
        ('bd_addr', bth_addr_t),
    ]
class random_type_1598(Structure):
    _fields_ = [
        ('addr', (td_uchar*6)),
    ]
class _bt_sdk_connection_property_stru(Structure):
    _fields_ = [
        ('role', uint32_t,2),
        ('result', uint32_t,30),
        ('device_handle', uint32_t),
        ('service_handle', uint32_t),
        ('service_class', uint16_t),
        ('duration', uint32_t),
        ('bd_addr', (uint8_t*6)),
    ]
bt_sdk_connection_property_stru=_bt_sdk_connection_property_stru
class random_type_1599(Structure):
    _fields_ = [
        ('dev_hdl', uintptr_t),
        ('addr', bd_addr_t),
    ]
pan_remote_device_info=random_type_1599
class random_type_1600(Structure):
    _fields_ = [
        ('event_type', uint8_t),
        ('data_status', uint8_t),
        ('addr_type', uint8_t),
        ('addr', (uint8_t*6)),
        ('primary_phy', uint8_t),
        ('secondary_phy', uint8_t),
        ('adv_sid', uint8_t),
        ('tx_power', uint8_t),
        ('rssi', uint8_t),
        ('periodic_adv_interval', uint16_t),
        ('direct_addr_type', uint8_t),
        ('direct_addr', (uint8_t*6)),
        ('data_length', uint8_t),
        ('data', (uint8_t*1)),
    ]
bth_gap_ext_adv_report_node=random_type_1600
class random_type_1601(Structure):
    _fields_ = [
        ('status', uint8_t),
        ('adv_handle', uint8_t),
        ('co_handle', uint16_t),
        ('num_cmp_ext_adv_events', uint8_t),
    ]
bth_gap_adv_set_terminated=random_type_1601
class random_type_1602(Structure):
    _fields_ = [
        ('handle', uint32_t),
        ('rssi', int8_t),
        ('status', uint8_t),
        ('reserved', uint16_t),
    ]
bth_gap_read_remote_rssi=random_type_1602
class random_type_1603(Structure):
    _fields_ = [
        ('status', uint8_t),
        ('connection_handle', uint16_t),
        ('conn_interval', uint16_t),
        ('conn_latency', uint16_t),
        ('supervision_timeout', uint16_t),
    ]
sdk_ble_connection_param_t=random_type_1603
class random_type_1605(Structure):
    _fields_ = [
        ('adv_handle', uint8_t),
        ('adv_type', uint8_t),
        ('adv_event_flag', uint8_t),
        ('primary_adv_channel_map', uint8_t),
        ('primary_adv_interval_min', uint32_t),
        ('primary_adv_interval_max', uint32_t),
        ('own_addr', (uint8_t*6)),
        ('peer_addr_type', uint8_t),
        ('peer_addr', (uint8_t*6)),
        ('adv_filter_policy', uint8_t),
        ('adv_tx_power', uint8_t),
        ('primary_adv_phy', uint8_t),
        ('secondary_adv_max_skip', uint8_t),
        ('secondary_adv_phy', uint8_t),
        ('advertising_sid', uint8_t),
        ('scan_request_notification_enable', uint8_t),
    ]
bth_gap_ext_adv_param=random_type_1605
class random_type_1606(Structure):
    _fields_ = [
        ('fragment_preference', uint8_t),
        ('eir_flag', uint8_t),
        ('adv_data_length', uint16_t),
        ('scan_rsp_data_length', uint16_t),
        ('adv_data', td_u32),
        ('scan_rsp_data', td_u32),
    ]
bth_gap_ext_adv_data=random_type_1606
class random_type_1607(Structure):
    _fields_ = [
        ('duration', uint16_t),
        ('max_extended_adv_events', uint8_t),
        ('priority', uint8_t),
    ]
bth_gap_ext_adv_en=random_type_1607
class random_type_1604(Structure):
    _fields_ = [
        ('adv_id', uint8_t),
        ('adv_param', bth_gap_ext_adv_param),
        ('adv_data', bth_gap_ext_adv_data),
        ('adv_en', bth_gap_ext_adv_en),
        ('is_advertising', td_bool),
        ('ble_adv_timer', osal_timer),
    ]
btsrv_ble_discovery_info=random_type_1604
class random_type_1608(Structure):
    _fields_ = [
        ('timer', td_u32),
        ('handler', td_u32),
        ('data', td_u32),
        ('interval', td_u32),
    ]
class random_type_1609(Structure):
    _fields_ = [
        ('sem', td_u32),
    ]
class random_type_1610(Structure):
    _fields_ = [
        ('bondable', uint8_t),
        ('io_capability', uint8_t),
        ('sc_enable', uint8_t),
        ('sc_mode', uint8_t),
    ]
class random_type_1611(Structure):
    _fields_ = [
        ('msg_id', uint32_t),
        ('param', (uintptr_t*4)),
    ]
class random_type_1612(Structure):
    _fields_ = [
        ('op', uint8_t),
        ('func', td_u32),
    ]
bt_sdk_callback_stru=random_type_1612
class random_type_1613(Structure):
    _fields_ = [
        ('bd_addr', (uint8_t*6)),
        ('have_key', uint8_t),
        ('key_type', uint8_t),
        ('name', (uint8_t*32)),
        ('class_of_device', int32_t),
        ('link_key', (uint8_t*16)),
    ]
class random_type_1614(Structure):
    _fields_ = [
        ('conn_id', uint16_t),
        ('addr', bd_addr_t),
        ('pair_status', uint8_t),
        ('interval', uint16_t),
        ('latency', uint16_t),
        ('timeout', uint16_t),
        ('ltk_len', uint8_t),
        ('ltk', (uint8_t*16)),
    ]
class random_type_1615(Structure):
    _fields_ = [
        ('ext_adv_param', bth_gap_ext_adv_param),
        ('ext_adv_data', bth_gap_ext_adv_data),
        ('ext_adv_en', bth_gap_ext_adv_en),
    ]
bth_gap_set_extern_adv=random_type_1615
class random_type_1617(Structure):
    _fields_ = [
        ('scan_filter_policy', uint8_t),
        ('scan_phys', uint8_t),
        ('scan_type_1m', uint8_t),
        ('scan_type_coded', uint8_t),
        ('scan_interval_1m', uint16_t),
        ('scan_window_1m', uint16_t),
        ('scan_interval_coded', uint16_t),
        ('scan_window_coded', uint16_t),
    ]
bth_gap_ext_scan_param=random_type_1617
class random_type_1618(Structure):
    _fields_ = [
        ('filter_duplicate', uint8_t),
        ('limited', uint8_t),
        ('duration', uint16_t),
        ('period', uint16_t),
    ]
bth_gap_ext_scan_en=random_type_1618
class random_type_1616(Structure):
    _fields_ = [
        ('ext_scan_param', bth_gap_ext_scan_param),
        ('ext_scan_en', bth_gap_ext_scan_en),
    ]
class random_type_1620(Structure):
    _fields_ = [
        ('conn_handle', uint16_t),
        ('all_phys', uint8_t),
        ('tx_phys', uint8_t),
        ('rx_phys', uint8_t),
        ('phy_options', uint16_t),
    ]
bth_gap_le_set_phy_t=random_type_1620
class random_type_1619(Structure):
    _fields_ = [
        ('addr', bth_address),
        ('phys', bth_gap_le_set_phy_t),
    ]
class random_type_1622(Structure):
    _fields_ = [
        ('conn_handle', uint16_t),
        ('maxtxoctets', uint16_t),
        ('maxtxtime', uint16_t),
    ]
bth_gap_le_set_data_length=random_type_1622
class random_type_1621(Structure):
    _fields_ = [
        ('addr', bth_address),
        ('len', bth_gap_le_set_data_length),
    ]
bth_smp_sec_param=random_type_1610
class random_type_1623(Structure):
    _fields_ = [
        ('adv_length', uint16_t),
        ('adv_data', td_u32),
        ('scan_rsp_length', uint16_t),
        ('scan_rsp_data', td_u32),
    ]
class random_type_1624(Structure):
    _fields_ = [
        ('min_interval', uint32_t),
        ('max_interval', uint32_t),
        ('adv_type', uint8_t),
        ('own_addr', bd_addr_t),
        ('peer_addr', bd_addr_t),
        ('channel_map', uint8_t),
        ('adv_filter_policy', uint8_t),
        ('tx_power', uint32_t),
        ('duration', uint32_t),
    ]
class random_type_1625(Structure):
    _fields_ = [
        ('bondable', uint8_t),
        ('io_capability', uint8_t),
        ('sc_enable', uint8_t),
        ('sc_mode', uint8_t),
    ]
bth_gap_set_extern_scan=random_type_1616
bth_gap_le_phys_ampid_addr=random_type_1619
class random_type_1626(Structure):
    _fields_ = [
        ('conn_handle', uint16_t),
        ('all_phys', uint8_t),
        ('tx_phys', uint8_t),
        ('rx_phys', uint8_t),
        ('phy_options', uint16_t),
    ]
bth_gap_le_data_length_ampid_addr=random_type_1621
class random_type_1627(Structure):
    _fields_ = [
        ('conn_handle', uint16_t),
        ('maxtxoctets', uint16_t),
        ('maxtxtime', uint16_t),
    ]
class bt_sdk_conn_dev_info(Structure):
    _fields_ = [
        ('link_type', uint8_t),
        ('bd', (uint8_t*6)),
        ('atype', uint8_t),
        ('cap', uint8_t),
    ]
bt_sdk_conn_dev_info_t=bt_sdk_conn_dev_info
p_bt_sdk_obex_auth_info_stru=td_u32
class _bt_sdk_obex_auth_info_stru(Structure):
    _fields_ = [
        ('dev_hdl', uint32_t),
        ('svc_type', uint16_t),
        ('pwd_only', uint8_t),
        ('rsp_func', td_u32),
        ('user_id', (uint8_t*21)),
        ('pwd', (uint8_t*65)),
    ]
class random_type_1628(Structure):
    _fields_ = [
        ('mutex', td_u32),
    ]
class random_type_1629(Structure):
    _fields_ = [
        ('addr', (uint8_t*6)),
        ('type', uint8_t),
    ]
class random_type_1630(Structure):
    _fields_ = [
        ('bd_addr', (uint8_t*6)),
        ('have_key', uint8_t),
        ('key_type', uint8_t),
        ('name', (uint8_t*32)),
        ('class_of_device', int32_t),
        ('link_key', (uint8_t*16)),
    ]
class random_type_1631(Structure):
    _fields_ = [
        ('device_num', uint8_t),
        ('reserve', (uint8_t*3)),
        ('info', (remote_device_info*16)),
    ]
btsrv_paired_device_info_t=random_type_1631
class hci_remote_name_request_complete_ev_stru(Structure):
    _fields_ = [
        ('status', uint8_t),
        ('bd', (uint8_t*6)),
        ('remote_name', (uint8_t*248)),
    ]
class random_type_1632(Structure):
    _fields_ = [
        ('op', uint8_t),
        ('func', td_u32),
    ]
class gap_execute_command_cfm_stru(Structure):
    _fields_ = [
        ('opindex', uint32_t),
        ('size', uint32_t),
        ('event_parameter', td_u32),
    ]
class random_type_1633(Structure):
    _fields_ = [
        ('bd', (uint8_t*6)),
        ('atype', uint8_t),
        ('cap', uint8_t),
    ]
class random_type_1634(Structure):
    _fields_ = [
        ('link_key', (uint8_t*16)),
        ('key_type', uint8_t),
        ('accept', uint8_t),
    ]
br_security_link_key_stru=random_type_1634
class random_type_1636(Structure):
    _fields_ = [
        ('addr', bd_addr_t),
        ('device_name', (td_uchar*32)),
    ]
class random_type_1637(Structure):
    _fields_ = [
        ('uuid_len', uint8_t),
        ('uuid', (uint8_t*16)),
    ]
dft_function=td_u32
class random_type_1638(Structure):
    _fields_ = [
        ('op', uint8_t),
        ('func', dft_function),
    ]
dft_table=random_type_1638
class random_type_1639(Structure):
    _fields_ = [
        ('min_id', uint16_t),
        ('max_id', uint16_t),
        ('fn_input_cmd', diag_cmd_f),
    ]
class random_type_1640(Structure):
    _fields_ = [
        ('peer_addr', diag_addr),
        ('pad', (uint8_t*3)),
    ]
class random_type_1641(Structure):
    _fields_ = [
        ('ocf', uint16_t),
        ('param_len', uint8_t),
        ('param', (uint8_t*0)),
    ]
bt_dft_set_vendor_params=random_type_1641
class random_type_1642(Structure):
    _fields_ = [
        ('on_off', uint8_t),
    ]
class random_type_1643(Structure):
    _fields_ = [
        ('length', uint8_t),
        ('subcode', uint8_t),
        ('freq', uint8_t),
        ('mode', uint8_t),
        ('switch_trx', uint8_t),
    ]
class random_type_1644(Structure):
    _fields_ = [
        ('on_off', uint8_t),
        ('tx_power', uint8_t),
        ('tx_channel', uint8_t),
        ('pkt_type', uint8_t),
        ('pkt_len', (uint8_t*2)),
        ('br_edr', uint8_t),
        ('payload', uint8_t),
    ]
class random_type_1645(Structure):
    _fields_ = [
        ('on_off', uint8_t),
        ('rx_channel', uint8_t),
        ('pkt_type', uint8_t),
        ('pkt_len', (uint8_t*2)),
        ('br_edr', uint8_t),
        ('payload', uint8_t),
    ]
class random_type_1646(Structure):
    _fields_ = [
        ('on_off', uint8_t),
        ('tx_channel', uint8_t),
        ('pkt_type', uint8_t),
        ('pkt_len', uint8_t),
        ('tx_phy', uint8_t),
    ]
class random_type_1647(Structure):
    _fields_ = [
        ('on_off', uint8_t),
        ('rx_channel', uint8_t),
        ('rx_phy', uint8_t),
    ]
bt_dft_dut_params=random_type_1642
bt_dft_sig_params=random_type_1643
bt_dft_bt_tx_params=random_type_1644
bt_dft_bt_rx_params=random_type_1645
bt_dft_ble_tx_params=random_type_1646
bt_dft_ble_rx_params=random_type_1647
class random_type_1648(Structure):
    _fields_ = [
        ('start_hdl', uint16_t),
        ('end_hdl', uint16_t),
        ('uuid', bt_uuid_t),
    ]
class random_type_1649(Structure):
    _fields_ = [
        ('uuid_len', uint8_t),
        ('uuid', (uint8_t*16)),
    ]
class random_type_1650(Structure):
    _fields_ = [
        ('uuid', bt_uuid_t),
        ('declare_handle', uint16_t),
        ('value_handle', uint16_t),
        ('properties', uint8_t),
    ]
class random_type_1651(Structure):
    _fields_ = [
        ('service_handle', uint16_t),
        ('uuid', bt_uuid_t),
    ]
class random_type_1652(Structure):
    _fields_ = [
        ('descriptor_hdl', uint16_t),
        ('uuid', bt_uuid_t),
    ]
class random_type_1653(Structure):
    _fields_ = [
        ('handle', uint16_t),
        ('data_len', uint16_t),
        ('data', td_u32),
    ]
class random_type_1654(Structure):
    _fields_ = [
        ('start_hdl', uint16_t),
        ('end_hdl', uint16_t),
        ('uuid', bt_uuid_t),
    ]
class random_type_1655(Structure):
    _fields_ = [
        ('client_id', uint8_t),
        ('conn_id', uint16_t),
        ('service', gattc_discovery_service_result_t),
        ('status', errcode_t),
    ]
gattc_disc_svc_cb_param=random_type_1655
class random_type_1656(Structure):
    _fields_ = [
        ('client_id', uint8_t),
        ('conn_id', uint16_t),
        ('character', gattc_discovery_character_result_t),
        ('status', errcode_t),
    ]
gattc_disc_chara_cb_param=random_type_1656
class random_type_1657(Structure):
    _fields_ = [
        ('client_id', uint8_t),
        ('conn_id', uint16_t),
        ('descriptor', gattc_discovery_descriptor_result_t),
        ('status', errcode_t),
    ]
gattc_disc_desc_cb_param=random_type_1657
class random_type_1658(Structure):
    _fields_ = [
        ('client_id', uint8_t),
        ('conn_id', uint16_t),
        ('uuid', bt_uuid_t),
        ('status', errcode_t),
    ]
gattc_disc_svc_compl_cb_param=random_type_1658
class random_type_1659(Structure):
    _fields_ = [
        ('client_id', uint8_t),
        ('conn_id', uint16_t),
        ('param', gattc_discovery_character_param_t),
        ('status', errcode_t),
    ]
gattc_disc_chara_compl_cb_param=random_type_1659
class random_type_1660(Structure):
    _fields_ = [
        ('client_id', uint8_t),
        ('conn_id', uint16_t),
        ('charatcer_handle', uint16_t),
        ('status', errcode_t),
    ]
gattc_disc_desc_compl_cb_param=random_type_1660
class random_type_1661(Structure):
    _fields_ = [
        ('client_id', uint8_t),
        ('conn_id', uint16_t),
        ('mtu_size', uint32_t),
        ('status', errcode_t),
    ]
gattc_mtu_changed_cb_param=random_type_1661
class random_type_1662(Structure):
    _fields_ = [
        ('client_id', uint8_t),
        ('conn_id', uint16_t),
        ('data', gattc_handle_value_t),
        ('status', errcode_t),
    ]
gattc_notify_indicate_cb_param=random_type_1662
class random_type_1664(Structure):
    _fields_ = [
        ('client_id', uint8_t),
        ('conn_id', uint16_t),
        ('param', gattc_read_req_by_uuid_param_t),
        ('status', errcode_t),
    ]
gattc_read_by_uuid_compl_cb_param=random_type_1664
class random_type_1669(Structure):
    _fields_ = [
        ('u', (uint8_t*16)),
    ]
bth_att_uuid=random_type_1669
class random_type_1668(Structure):
    _fields_ = [
        ('is_short_uuid', uint32_t),
        ('short_uuid', uint16_t),
        ('reserve', uint16_t),
        ('long_uuid', bth_att_uuid),
    ]
bth_gatt_uuid=random_type_1668
class random_type_1666(Structure):
    _fields_ = [
        ('addr', bth_address),
        ('start_hdl', uint16_t),
        ('end_hdl', uint16_t),
        ('uuid', bth_gatt_uuid),
    ]
bth_gatt_discovery_service_response=random_type_1666
class random_type_1667(Structure):
    _fields_ = [
        ('bd', (uint8_t*6)),
        ('atype', uint8_t),
        ('cap', uint8_t),
    ]
class random_type_1670(Structure):
    _fields_ = [
        ('client_id', uint8_t),
        ('conn_id', uint16_t),
        ('uuid', bth_gatt_uuid),
        ('status', uint16_t),
    ]
bth_gatt_discovery_service_complete=random_type_1670
class random_type_1672(Structure):
    _fields_ = [
        ('service_handle', uint16_t),
        ('characteristic_uuid', bth_gatt_uuid),
        ('declare_handle', uint16_t),
        ('value_handle', uint16_t),
        ('broadcastable_flag', uint32_t),
        ('readable_flag', uint32_t),
        ('writable_flag', uint32_t),
        ('writable_without_response_flag', uint32_t),
        ('signed_writable_flag', uint32_t),
        ('notifiable_flag', uint32_t),
        ('indicatable_flag', uint32_t),
        ('authentication_flag', uint32_t),
        ('authorization_flag', uint32_t),
        ('mitm_flag', uint32_t),
        ('has_extended_properties', uint32_t),
    ]
bth_gatt_characteristic=random_type_1672
class random_type_1671(Structure):
    _fields_ = [
        ('addr', bth_address),
        ('characteristic', bth_gatt_characteristic),
    ]
bth_gatt_discovery_character_response=random_type_1671
class random_type_1673(Structure):
    _fields_ = [
        ('client_id', uint8_t),
        ('conn_id', uint16_t),
        ('service_handle', uint16_t),
        ('uuid', bth_gatt_uuid),
        ('status', uint16_t),
    ]
bth_gatt_discovery_character_complete=random_type_1673
class random_type_1675(Structure):
    _fields_ = [
        ('client_id', uint8_t),
        ('conn_id', uint16_t),
        ('character_handle', uint16_t),
        ('status', uint16_t),
    ]
bth_gatt_discovery_descriptor_complete=random_type_1675
class random_type_1676(Structure):
    _fields_ = [
        ('addr', bth_address),
        ('error_code', uint8_t),
        ('handle', uint16_t),
        ('offset', uint16_t),
        ('val_len', uint16_t),
        ('val', (uint8_t*0)),
    ]
bth_gatt_read_character_response=random_type_1676
class random_type_1677(Structure):
    _fields_ = [
        ('client_id', uint8_t),
        ('conn_id', uint16_t),
        ('start_hdl', uint16_t),
        ('end_hdl', uint16_t),
        ('uuid', bth_gatt_uuid),
        ('status', uint16_t),
    ]
bth_gatt_read_characteristic_complete=random_type_1677
class random_type_1678(Structure):
    _fields_ = [
        ('addr', bth_address),
        ('error_code', uint32_t),
        ('handle', uint16_t),
        ('offset', uint16_t),
        ('val_len', uint16_t),
        ('val', (uint8_t*0)),
    ]
bth_gatt_write_attribute_response=random_type_1678
class random_type_1680(Structure):
    _fields_ = [
        ('len', uint32_t),
        ('data', (uint8_t*0)),
    ]
bth_gatt_characteristic_value=random_type_1680
class random_type_1679(Structure):
    _fields_ = [
        ('addr', bth_address),
        ('characteristic_handle', uint16_t),
        ('value', bth_gatt_characteristic_value),
    ]
bth_gatt_client_notification=random_type_1679
class random_type_1681(Structure):
    _fields_ = [
        ('msg_id', uint32_t),
        ('param', (uintptr_t*4)),
    ]
class random_type_1682(Structure):
    _fields_ = [
        ('conn_id', uint16_t),
        ('addr', bd_addr_t),
        ('pair_status', uint8_t),
        ('interval', uint16_t),
        ('latency', uint16_t),
        ('timeout', uint16_t),
        ('ltk_len', uint8_t),
        ('ltk', (uint8_t*16)),
    ]
class random_type_1683(Structure):
    _fields_ = [
        ('addr', (uint8_t*6)),
        ('type', uint8_t),
    ]
class random_type_1684(Structure):
    _fields_ = [
        ('client_id', uint8_t),
        ('app_uuid', bt_uuid_t),
    ]
gatt_client_list_node=random_type_1684
class random_type_1685(Structure):
    _fields_ = [
        ('addr', bth_address),
        ('by_uuid_flag', uint8_t),
        ('uuid', bth_gatt_uuid),
        ('flag', uint16_t),
    ]
class random_type_1686(Structure):
    _fields_ = [
        ('addr', bth_address),
        ('service_handle', uint16_t),
        ('by_uuid_flag', uint8_t),
        ('character_uuid', bth_gatt_uuid),
        ('flag', uint16_t),
    ]
class random_type_1687(Structure):
    _fields_ = [
        ('addr', bth_address),
        ('flag', uint16_t),
        ('characteristic_info', bth_gatt_characteristic),
    ]
class random_type_1688(Structure):
    _fields_ = [
        ('addr', bth_address),
        ('att_handle', uint16_t),
        ('offset', uint16_t),
        ('security_mode', uint8_t),
        ('timeout', uint32_t),
    ]
class random_type_1689(Structure):
    _fields_ = [
        ('addr', bth_address),
        ('start_handle', uint16_t),
        ('end_handle', uint16_t),
        ('uuid', bth_gatt_uuid),
        ('flags', uint16_t),
    ]
class random_type_1690(Structure):
    _fields_ = [
        ('addr', bth_address),
        ('characteristic', bth_gatt_characteristic),
        ('flag', uint16_t),
        ('offset', uint16_t),
        ('len', uint16_t),
        ('data', (uint8_t*0)),
    ]
class random_type_1691(Structure):
    _fields_ = [
        ('addr', bth_address),
        ('mtu', uint16_t),
        ('flag', uint32_t),
    ]
class random_type_1692(Structure):
    _fields_ = [
        ('addr', bth_address),
        ('mtu', uint16_t),
    ]
bth_gatt_mtu=random_type_1692
bth_gatt_discovery_service=random_type_1685
bth_gatt_discovery_character=random_type_1686
bth_gatt_discovery_descriptor=random_type_1687
bth_gatt_read_character_request=random_type_1688
bth_gatt_discovery_character_by_uuid=random_type_1689
bth_gatt_write_characteristic=random_type_1690
bth_gatt_mtu_config=random_type_1691
class random_type_1693(Structure):
    _fields_ = [
        ('add_service_cb', gatts_add_service_callback),
        ('add_characteristic_cb', gatts_add_characteristic_callback),
        ('add_descriptor_cb', gatts_add_descriptor_callback),
        ('start_service_cb', gatts_start_service_callback),
        ('stop_service_cb', gatts_stop_service_callback),
        ('delete_service_cb', gatts_delete_all_service_callback),
        ('read_request_cb', gatts_read_request_callback),
        ('write_request_cb', gatts_write_request_callback),
        ('mtu_changed_cb', gatts_mtu_changed_callback),
    ]
class random_type_1694(Structure):
    _fields_ = [
        ('uuid_len', uint8_t),
        ('uuid', (uint8_t*16)),
    ]
class random_type_1695(Structure):
    _fields_ = [
        ('handle', uint16_t),
        ('value_handle', uint16_t),
    ]
class random_type_1696(Structure):
    _fields_ = [
        ('request_id', uint16_t),
        ('handle', uint16_t),
        ('offset', uint16_t),
        ('need_rsp', td_bool),
        ('need_authorize', td_bool),
        ('is_long', td_bool),
    ]
class random_type_1697(Structure):
    _fields_ = [
        ('request_id', uint16_t),
        ('handle', uint16_t),
        ('offset', uint16_t),
        ('need_rsp', td_bool),
        ('need_authorize', td_bool),
        ('is_prep', td_bool),
        ('length', uint16_t),
        ('value', td_u32),
    ]
gatts_service_read=td_u32
gatts_service_write=td_u32
gatts_service_indicate=td_u32
class random_type_1698(Structure):
    _fields_ = [
        ('read', gatts_service_read),
        ('write', gatts_service_write),
        ('indicate', gatts_service_indicate),
    ]
gatts_operate_func=random_type_1698
class serv_node(Structure):
    _fields_ = [
        ('handle', uint16_t),
        ('uuid', bt_uuid_t),
        ('is_primary', td_bool),
        ('is_active', td_bool),
        ('func', gatts_operate_func),
        ('character', list_stru),
        ('include', list_stru),
    ]
gatts_svc_list_node=serv_node
class random_type_1699(Structure):
    _fields_ = [
        ('handle', uint16_t),
        ('value_handle', uint16_t),
        ('uuid', bt_uuid_t),
        ('property', uint8_t),
        ('permission', uint8_t),
        ('descriptor', list_stru),
        ('len', uint16_t),
        ('val', td_u32),
    ]
gatts_char_info=random_type_1699
class random_type_1700(Structure):
    _fields_ = [
        ('characteristic_handle', uint16_t),
        ('data_size', uint16_t),
        ('data', (uint8_t*0)),
    ]
bth_gatt_notify_characteristic=random_type_1700
class random_type_1701(Structure):
    _fields_ = [
        ('server_id', uint8_t),
        ('conn_id', uint16_t),
        ('mtu_size', uint32_t),
        ('status', errcode_t),
    ]
gatts_mtu_changed_cb_param=random_type_1701
class random_type_1702(Structure):
    _fields_ = [
        ('server_id', uint8_t),
        ('conn_id', uint16_t),
        ('read_cb_para', gatts_req_read_cb_t),
        ('status', errcode_t),
    ]
gatts_read_req_cb_param=random_type_1702
class random_type_1703(Structure):
    _fields_ = [
        ('server_id', uint8_t),
        ('conn_id', uint16_t),
        ('write_cb_para', gatts_req_write_cb_t),
        ('status', errcode_t),
    ]
gatts_write_req_cb_param=random_type_1703
class random_type_1704(Structure):
    _fields_ = [
        ('server_id', uint8_t),
        ('uuid', bt_uuid_t),
        ('handle', uint16_t),
        ('status', errcode_t),
    ]
gatts_svc_add_cb_param=random_type_1704
class random_type_1705(Structure):
    _fields_ = [
        ('server_id', uint8_t),
        ('uuid', bt_uuid_t),
        ('service_handle', uint16_t),
        ('result', gatts_add_character_result_t),
        ('status', errcode_t),
    ]
gatts_char_add_cb_param=random_type_1705
class random_type_1706(Structure):
    _fields_ = [
        ('server_id', uint8_t),
        ('uuid', bt_uuid_t),
        ('service_handle', uint16_t),
        ('handle', uint16_t),
        ('status', errcode_t),
    ]
gatts_desc_add_cb_param=random_type_1706
class random_type_1707(Structure):
    _fields_ = [
        ('service_handle', uint32_t),
    ]
bth_gatt_create_service_result=random_type_1707
class random_type_1708(Structure):
    _fields_ = [
        ('service_handle', uint32_t),
        ('characteristic', bth_gatt_characteristic),
        ('data_size', uint32_t),
        ('data', (uint8_t*0)),
    ]
bth_gatt_add_characteristic=random_type_1708
class random_type_1709(Structure):
    _fields_ = [
        ('service_handle', uint16_t),
        ('characteristic_uuid', bth_gatt_uuid),
        ('declare_handle', uint16_t),
        ('value_handle', uint16_t),
        ('broadcastable_flag', uint32_t),
        ('readable_flag', uint32_t),
        ('writable_flag', uint32_t),
        ('writable_without_response_flag', uint32_t),
        ('signed_writable_flag', uint32_t),
        ('notifiable_flag', uint32_t),
        ('indicatable_flag', uint32_t),
        ('authentication_flag', uint32_t),
        ('authorization_flag', uint32_t),
        ('mitm_flag', uint32_t),
        ('has_extended_properties', uint32_t),
    ]
class random_type_1710(Structure):
    _fields_ = [
        ('is_short_uuid', uint32_t),
        ('short_uuid', uint16_t),
        ('reserve', uint16_t),
        ('long_uuid', bth_att_uuid),
    ]
class random_type_1711(Structure):
    _fields_ = [
        ('u', (uint8_t*16)),
    ]
class random_type_1712(Structure):
    _fields_ = [
        ('service_uuid', bth_gatt_uuid),
        ('service_handle', uint32_t),
    ]
bth_gatt_start_service=random_type_1712
class random_type_1721(Structure):
    _fields_ = [
        ('service_uuid', bth_gatt_uuid),
        ('service_handle', uint16_t),
    ]
bth_gatt_service=random_type_1721
class random_type_1722(Structure):
    _fields_ = [
        ('addr', bth_address),
        ('mtu', uint16_t),
    ]
class random_type_1723(Structure):
    _fields_ = [
        ('bd', (uint8_t*6)),
        ('atype', uint8_t),
        ('cap', uint8_t),
    ]
class random_type_1724(Structure):
    _fields_ = [
        ('opcode', uint16_t),
        ('addr', (uint8_t*6)),
        ('addr_type', uint8_t),
        ('request_id', uint16_t),
        ('att_hdl', uint16_t),
        ('svc_uuid', (uint8_t*16)),
        ('uuid', (uint8_t*16)),
        ('offset', uint16_t),
        ('len', uint16_t),
        ('need_authorize', uint8_t),
        ('need_rsp', uint8_t),
        ('data', (uint8_t*0)),
    ]
bth_gatt_upper_data_t=random_type_1724
class random_type_1725(Structure):
    _fields_ = [
        ('server_id', uint8_t),
        ('app_uuid', bt_uuid_t),
        ('svc_list', list_stru),
    ]
gatt_server_list_node=random_type_1725
class hdl_node(Structure):
    _fields_ = [
        ('hdl', uint16_t),
    ]
gatts_hdl_list_node=hdl_node
class random_type_1727(Structure):
    _fields_ = [
        ('handle', uint16_t),
        ('uuid', bt_uuid_t),
        ('permission', uint8_t),
        ('len', uint16_t),
        ('val', td_u32),
    ]
gatts_desc_info=random_type_1727
class random_type_1728(Structure):
    _fields_ = [
        ('msg_id', uint32_t),
        ('param', (uintptr_t*4)),
    ]
class random_type_1729(Structure):
    _fields_ = [
        ('conn_id', uint16_t),
        ('addr', bd_addr_t),
        ('pair_status', uint8_t),
        ('interval', uint16_t),
        ('latency', uint16_t),
        ('timeout', uint16_t),
        ('ltk_len', uint8_t),
        ('ltk', (uint8_t*16)),
    ]
class random_type_1730(Structure):
    _fields_ = [
        ('addr', (uint8_t*6)),
        ('type', uint8_t),
    ]
class random_type_1732(Structure):
    _fields_ = [
        ('start_hdl', uint16_t),
        ('end_hdl', uint16_t),
        ('uuid', bth_gatt_uuid),
        ('data_size', uint16_t),
        ('data', (uint8_t*0)),
    ]
class random_type_1733(Structure):
    _fields_ = [
        ('le_att_mtu_default', uint16_t),
        ('le_att_mtu_le_max', uint16_t),
    ]
class random_type_1734(Structure):
    _fields_ = [
        ('chara_uuid', bt_uuid_t),
        ('permissions', uint8_t),
        ('properties', uint8_t),
        ('value_len', uint16_t),
        ('value', td_u32),
    ]
class random_type_1735(Structure):
    _fields_ = [
        ('desc_uuid', bt_uuid_t),
        ('permissions', uint8_t),
        ('value_len', uint16_t),
        ('value', td_u32),
    ]
class random_type_1736(Structure):
    _fields_ = [
        ('request_id', uint16_t),
        ('status', uint8_t),
        ('offset', uint16_t),
        ('value_len', uint16_t),
        ('value', td_u32),
    ]
gatts_send_rsp_t=random_type_1736
class random_type_1737(Structure):
    _fields_ = [
        ('attr_handle', uint16_t),
        ('value_len', uint16_t),
        ('value', td_u32),
    ]
class random_type_1738(Structure):
    _fields_ = [
        ('chara_uuid', bt_uuid_t),
        ('start_handle', uint16_t),
        ('end_handle', uint16_t),
        ('value_len', uint16_t),
        ('value', td_u32),
    ]
bth_gatt_notify_characteristic_by_uuid=random_type_1732
bth_le_att_mtu_param=random_type_1733
class random_type_1739(Structure):
    _fields_ = [
        ('chara_uuid', bt_uuid_t),
        ('start_handle', uint16_t),
        ('end_handle', uint16_t),
        ('value_len', uint16_t),
        ('value', td_u32),
    ]
class random_type_1740(Structure):
    _fields_ = [
        ('uuid_len', uint8_t),
        ('uuid', (uint8_t*16)),
    ]
class random_type_1741(Structure):
    _fields_ = [
        ('chara_uuid', bt_uuid_t),
        ('permissions', uint8_t),
        ('properties', uint8_t),
        ('value_len', uint16_t),
        ('value', td_u32),
    ]
class random_type_1742(Structure):
    _fields_ = [
        ('handle', uint16_t),
        ('value_handle', uint16_t),
    ]
class random_type_1743(Structure):
    _fields_ = [
        ('desc_uuid', bt_uuid_t),
        ('permissions', uint8_t),
        ('value_len', uint16_t),
        ('value', td_u32),
    ]
td_u8=td_uchar
class random_type_1746(Structure):
    _fields_ = [
        ('codec_type', td_u8),
        ('cap_len', td_u8),
        ('payload', td_u16),
        ('codec_caps', (td_u8*16)),
    ]
bt_a2dp_codec_param=random_type_1746
class random_type_1744(Structure):
    _fields_ = [
        ('active_dev_addr', bd_addr_t),
        ('dev_hdl', uint32_t),
        ('conn_hdl', uint32_t),
        ('stream_hdl', uint32_t),
        ('conn_state', uint8_t),
        ('conn_state_reported', uint8_t),
        ('stream_state', uint8_t),
        ('playing_state', uint8_t),
        ('codec', bt_a2dp_codec_param),
    ]
a2dp_src_service_ctx=random_type_1744
class random_type_1745(Structure):
    _fields_ = [
        ('addr', (uint8_t*6)),
        ('type', uint8_t),
    ]
a2dp_src_connect_state_changed_callback=td_u32
a2dp_src_playing_state_changed_callback=td_u32
a2dp_src_configuration_changed_callback=td_u32
class random_type_1747(Structure):
    _fields_ = [
        ('connectstate_changed_cb', a2dp_src_connect_state_changed_callback),
        ('playing_state_changed_cb', a2dp_src_playing_state_changed_callback),
        ('configuration_changed_cb', a2dp_src_configuration_changed_callback),
    ]
a2dp_src_callbacks_t=random_type_1747
class random_type_1748(Structure):
    _fields_ = [
        ('codec_priority', td_s32),
        ('codec_type', td_u32),
        ('sample_rate', td_uchar),
        ('bits_per_sample', td_uchar),
        ('channel_mode', td_uchar),
        ('codec_specific1', td_u32),
        ('codec_specific2', td_u32),
        ('codec_specific3', td_u32),
        ('codec_specific4', td_u32),
    ]
a2dp_codec_info_t=random_type_1748
class random_type_1751(Structure):
    _fields_ = [
        ('sample_f', uint32_t),
        ('chan_mode', uint8_t),
        ('block_len', uint8_t),
        ('sub_band', uint8_t),
        ('alloc_method', uint8_t),
        ('min_bit_pool', uint8_t),
        ('max_bit_pool', uint8_t),
    ]
sdk_sbc_cfg_stru=random_type_1751
class random_type_1752(Structure):
    _fields_ = [
        ('sample_rate', uint32_t),
        ('bit_rate', uint32_t),
        ('channel', uint8_t),
        ('object_type', uint8_t),
        ('rfa', uint8_t),
        ('vbr', uint8_t),
    ]
sdk_aac_cfg_stru=random_type_1752
class random_type_1749(Union):
    _fields_ = [
        ('sbc', sdk_sbc_cfg_stru),
        ('aac', sdk_aac_cfg_stru),
    ]
class random_type_1750(Structure):
    _fields_ = [
        ('payload', uint16_t),
        ('code_c', uint8_t),
        ('info', random_type_1749),
    ]
sdka2dp_stream_cfg_stru=random_type_1750
class btsdk_a2dp_service_cap_stru(Structure):
    _fields_ = [
        ('len', uint8_t),
        ('service_capability', (uint8_t*1)),
    ]
class random_type_1753(Structure):
    _fields_ = [
        ('rmt_hdl', td_u32),
        ('seid', uint8_t),
        ('sc', btsdk_a2dp_service_cap_stru),
    ]
sdka2dp_rmt_sep_cap_stru=random_type_1753
class random_type_1754(Structure):
    _fields_ = [
        ('stream_hdl', uint32_t),
        ('state', uint8_t),
        ('src', uint8_t),
        ('len', uint16_t),
        ('param', (uint8_t*0)),
    ]
a2dp_src_stream_chg_params=random_type_1754
class random_type_1755(Structure):
    _fields_ = [
        ('stream_handle', td_u32),
        ('role', uint8_t),
        ('event', uint8_t),
        ('err', uint8_t),
    ]
sdka2dp_stream_err_stru=random_type_1755
class random_type_1756(Structure):
    _fields_ = [
        ('dev_hdl', uint32_t),
        ('addr', (uint8_t*6)),
    ]
remote_device_s=random_type_1756
class random_type_1757(Structure):
    _fields_ = [
        ('stream_mtu', uint16_t),
        ('frame_size', uint16_t),
        ('num_frame', uint8_t),
    ]
sdka2dp_stream_mtu_stru=random_type_1757
class random_type_1758(Structure):
    _fields_ = [
        ('sample_frequency', td_u32),
        ('chnl_mode', td_u8),
        ('block_length', td_u8),
        ('subband', td_u8),
        ('alloc_method', td_u8),
        ('min_bitpool', td_u8),
        ('max_bitpool', td_u8),
    ]
bt_a2dp_sbc_codec_caps=random_type_1758
class random_type_1759(Structure):
    _fields_ = [
        ('sample_frequency', td_u32),
        ('bit_rate', td_u32),
        ('channels', td_u8),
        ('object_type', td_u8),
        ('rfa', td_u8),
        ('vbr', td_u8),
    ]
bt_a2dp_mpeg24_codec_caps=random_type_1759
btsdk_a2dp_rmt_sep_cap_cbk=td_u32
btsdk_a2dp_stream_state_chg_cbk=td_u32
btsdk_a2dp_stream_cfg_chg_cbk=td_u32
btsdk_a2dp_stream_reject_cbk=td_u32
class random_type_1760(Structure):
    _fields_ = [
        ('rmt_sep_cap_cbk', btsdk_a2dp_rmt_sep_cap_cbk),
        ('stream_state_chg_cbk', btsdk_a2dp_stream_state_chg_cbk),
        ('stream_cfg_chg_cbk', btsdk_a2dp_stream_cfg_chg_cbk),
        ('stream_reject_cbk', btsdk_a2dp_stream_reject_cbk),
    ]
btsdka2dp_cbk=random_type_1760
class random_type_1761(Structure):
    _fields_ = [
        ('msg_id', uint32_t),
        ('param', (uintptr_t*4)),
    ]
td_pvoid=td_u32
class random_type_1762(Structure):
    _fields_ = [
        ('stream_hdl', td_pvoid),
        ('codec', bt_a2dp_codec_param),
    ]
bt_audio_a2dp_config_chg_data=random_type_1762
class random_type_1763(Structure):
    _fields_ = [
        ('stream_hdl', td_pvoid),
        ('stream_mtu', td_u16),
        ('frame_size', td_u16),
        ('num_frame', td_u8),
        ('pad', td_u8),
        ('pad1', td_u16),
    ]
bt_audio_a2dp_stream_open_data=random_type_1763
profile_connect_state_t=td_u8
a2dp_playing_state_t=td_u8
class random_type_1764(Structure):
    _fields_ = [
        ('active_dev_addr', bd_addr_t),
        ('dev_hdl', uint32_t),
        ('conn_hdl', uint32_t),
        ('stream_hdl', uint32_t),
        ('conn_state', uint8_t),
        ('conn_state_reported', uint8_t),
        ('stream_state', uint8_t),
        ('playing_state', uint8_t),
        ('codec', bt_a2dp_codec_param),
    ]
a2dp_snk_service_ctx=random_type_1764
class random_type_1765(Structure):
    _fields_ = [
        ('addr', (uint8_t*6)),
        ('type', uint8_t),
    ]
class random_type_1766(Structure):
    _fields_ = [
        ('codec_type', td_u8),
        ('cap_len', td_u8),
        ('payload', td_u16),
        ('codec_caps', (td_u8*16)),
    ]
a2dp_snk_connect_state_changed_callback=td_u32
a2dp_snk_start_stream_request_callback=td_u32
a2dp_snk_playing_state_changed_callback=td_u32
a2dp_snk_configuration_changed_callback=td_u32
class random_type_1767(Structure):
    _fields_ = [
        ('connectstate_changed_cb', a2dp_snk_connect_state_changed_callback),
        ('start_stream_request_cb', a2dp_snk_start_stream_request_callback),
        ('playing_state_changed_cb', a2dp_snk_playing_state_changed_callback),
        ('configuration_changed_cb', a2dp_snk_configuration_changed_callback),
    ]
a2dp_snk_callbacks_t=random_type_1767
class random_type_1768(Structure):
    _fields_ = [
        ('codec_priority', td_s32),
        ('codec_type', td_u32),
        ('sample_rate', td_uchar),
        ('bits_per_sample', td_uchar),
        ('channel_mode', td_uchar),
        ('codec_specific1', td_u32),
        ('codec_specific2', td_u32),
        ('codec_specific3', td_u32),
        ('codec_specific4', td_u32),
    ]
a2dp_snk_codec_info_t=random_type_1768
class random_type_1769(Union):
    _fields_ = [
        ('sbc', sdk_sbc_cfg_stru),
        ('aac', sdk_aac_cfg_stru),
    ]
class random_type_1770(Structure):
    _fields_ = [
        ('payload', uint16_t),
        ('code_c', uint8_t),
        ('info', random_type_1769),
    ]
class random_type_1771(Structure):
    _fields_ = [
        ('sample_f', uint32_t),
        ('chan_mode', uint8_t),
        ('block_len', uint8_t),
        ('sub_band', uint8_t),
        ('alloc_method', uint8_t),
        ('min_bit_pool', uint8_t),
        ('max_bit_pool', uint8_t),
    ]
class random_type_1772(Structure):
    _fields_ = [
        ('sample_rate', uint32_t),
        ('bit_rate', uint32_t),
        ('channel', uint8_t),
        ('object_type', uint8_t),
        ('rfa', uint8_t),
        ('vbr', uint8_t),
    ]
class random_type_1773(Structure):
    _fields_ = [
        ('rmt_hdl', td_u32),
        ('seid', uint8_t),
        ('sc', btsdk_a2dp_service_cap_stru),
    ]
class random_type_1774(Structure):
    _fields_ = [
        ('stream_hdl', uint32_t),
        ('state', uint8_t),
        ('src', uint8_t),
        ('len', uint16_t),
        ('param', (uint8_t*0)),
    ]
class random_type_1775(Structure):
    _fields_ = [
        ('stream_handle', td_u32),
        ('role', uint8_t),
        ('event', uint8_t),
        ('err', uint8_t),
    ]
class random_type_1776(Structure):
    _fields_ = [
        ('dev_hdl', uint32_t),
        ('addr', (uint8_t*6)),
    ]
class random_type_1777(Structure):
    _fields_ = [
        ('stream_mtu', uint16_t),
        ('frame_size', uint16_t),
        ('num_frame', uint8_t),
    ]
class random_type_1778(Structure):
    _fields_ = [
        ('sample_frequency', td_u32),
        ('chnl_mode', td_u8),
        ('block_length', td_u8),
        ('subband', td_u8),
        ('alloc_method', td_u8),
        ('min_bitpool', td_u8),
        ('max_bitpool', td_u8),
    ]
class random_type_1779(Structure):
    _fields_ = [
        ('sample_frequency', td_u32),
        ('bit_rate', td_u32),
        ('channels', td_u8),
        ('object_type', td_u8),
        ('rfa', td_u8),
        ('vbr', td_u8),
    ]
class random_type_1780(Structure):
    _fields_ = [
        ('rmt_sep_cap_cbk', btsdk_a2dp_rmt_sep_cap_cbk),
        ('stream_state_chg_cbk', btsdk_a2dp_stream_state_chg_cbk),
        ('stream_cfg_chg_cbk', btsdk_a2dp_stream_cfg_chg_cbk),
        ('stream_reject_cbk', btsdk_a2dp_stream_reject_cbk),
    ]
class random_type_1781(Structure):
    _fields_ = [
        ('msg_id', uint32_t),
        ('param', (uintptr_t*4)),
    ]
class random_type_1782(Structure):
    _fields_ = [
        ('stream_hdl', td_pvoid),
        ('codec', bt_a2dp_codec_param),
    ]
class random_type_1783(Structure):
    _fields_ = [
        ('stream_hdl', td_pvoid),
        ('stream_mtu', td_u16),
        ('frame_size', td_u16),
        ('num_frame', td_u8),
        ('pad', td_u8),
        ('pad1', td_u16),
    ]
avrcp_ct_conn_state_changed_callback=td_u32
avrcp_ct_press_button_response_callback=td_u32
avrcp_ct_release_button_response_callback=td_u32
avrcp_ct_supported_companies_callback=td_u32
avrcp_ct_supported_event_callback=td_u32
avrcp_ct_get_element_attribute_callback=td_u32
avrcp_ct_get_play_status_callback=td_u32
avrcp_ct_notification_callback=td_u32
class random_type_1784(Structure):
    _fields_ = [
        ('conn_state_changed_cb', avrcp_ct_conn_state_changed_callback),
        ('press_button_cb', avrcp_ct_press_button_response_callback),
        ('release_button_cb', avrcp_ct_release_button_response_callback),
        ('supported_companies_cb', avrcp_ct_supported_companies_callback),
        ('supported_event_cb', avrcp_ct_supported_event_callback),
        ('element_attr_cb', avrcp_ct_get_element_attribute_callback),
        ('play_status_cb', avrcp_ct_get_play_status_callback),
        ('notification_cb', avrcp_ct_notification_callback),
    ]
avrcp_ct_callbacks_t=random_type_1784
class random_type_1785(Structure):
    _fields_ = [
        ('addr', (uint8_t*6)),
        ('type', uint8_t),
    ]
class random_type_1786(Structure):
    _fields_ = [
        ('company_num', td_uchar),
        ('company_id', td_u32),
    ]
avrcp_ct_support_company_para_t=random_type_1786
class random_type_1787(Structure):
    _fields_ = [
        ('event_num', td_uchar),
        ('event_id', td_u32),
    ]
avrcp_ct_support_event_para_t=random_type_1787
class random_type_1788(Structure):
    _fields_ = [
        ('attr_num', td_uchar),
        ('attr_value', td_u32),
    ]
avrcp_ct_element_attr_para_cb_t=random_type_1788
class random_type_1789(Structure):
    _fields_ = [
        ('attr_id', td_u32),
        ('character_set_id', td_u16),
        ('value_len', td_u16),
        ('value', td_u32),
    ]
avrcp_ct_element_attr_value_cb_t=random_type_1789
avrcp_play_status_t=td_u8
class random_type_1790(Structure):
    _fields_ = [
        ('song_length', td_u32),
        ('song_position', td_u32),
        ('play_status', avrcp_play_status_t),
    ]
avrcp_ct_play_status_cb_t=random_type_1790
avrcp_notify_event_t=td_u8
avrcp_battery_status_t=td_u8
avrcp_ct_system_status_t=td_u8
class random_type_1792(Structure):
    _fields_ = [
        ('player_id', td_u16),
        ('uid_counter', td_u16),
    ]
avrcp_ct_addressed_player_changed_cb_t=random_type_1792
class random_type_1793(Structure):
    _fields_ = [
        ('size', uint32_t),
        ('song_length', uint32_t),
        ('song_position', uint32_t),
        ('play_status', uint8_t),
    ]
bt_sdk_play_status_rsp_stru=random_type_1793
class random_type_1794(Structure):
    _fields_ = [
        ('size', uint32_t),
        ('id', uint8_t),
    ]
bt_sdk_set_absolute_vol_req_stru=random_type_1794
class random_type_1795(Structure):
    _fields_ = [
        ('size', uint32_t),
        ('cmd_type', uint16_t),
        ('error_code', uint8_t),
    ]
bt_sdk_general_reject_rsp_stru=random_type_1795
class random_type_1796(Structure):
    _fields_ = [
        ('size', uint32_t),
        ('state_flag', uint8_t),
        ('op_id', uint8_t),
        ('vendor_unique_id', uint16_t),
    ]
bt_sdk_pass_through_stru=random_type_1796
class random_type_1797(Structure):
    _fields_ = [
        ('size', uint32_t),
        ('capability_id', uint8_t),
        ('count', uint8_t),
        ('capability', (uint8_t*1)),
    ]
bt_sdk_get_capabilities_rsp_stru=random_type_1797
class random_type_1801(Structure):
    _fields_ = [
        ('attr_id', uint32_t),
        ('characterset_id', uint16_t),
        ('len', uint16_t),
        ('value', (uint8_t*0)),
    ]
bt_sdk4id_string_stru=random_type_1801
class random_type_1799(Union):
    _fields_ = [
        ('id_num', uint8_t),
        ('id_value', bt_sdk4id_string_stru),
    ]
class random_type_1800(Structure):
    _fields_ = [
        ('size', uint32_t),
        ('subpacket_type', uint32_t),
        ('random_var_1798', random_type_1799),
    ]
bt_sdk_get_element_attr_rsp_stru=random_type_1800
class random_type_1802(Structure):
    _fields_ = [
        ('msg_id', uint32_t),
        ('param', (uintptr_t*4)),
    ]
avrcp_srv_msg_handle_func=td_u32
class random_type_1803(Structure):
    _fields_ = [
        ('id', uint32_t),
        ('func', avrcp_srv_msg_handle_func),
    ]
avrcp_serv_hdl_func_type=random_type_1803
class random_type_1804(Structure):
    _fields_ = [
        ('attr_num', td_uchar),
        ('attr_id_list', td_u32),
    ]
avrcp_ct_get_element_attr_para_t=random_type_1804
class random_type_1805(Structure):
    _fields_ = [
        ('mutex', td_u32),
    ]
class random_type_1806(Structure):
    _fields_ = [
        ('addr', (uint8_t*6)),
        ('type', uint8_t),
    ]
class random_type_1807(Structure):
    _fields_ = [
        ('node', osal_list_head),
        ('dev_hdl', uint32_t),
        ('tg_conn_hdl', uint32_t),
        ('conn_hdl', uint32_t),
        ('active_dev_addr', bd_addr_t),
        ('tg_conn_state', uint8_t),
        ('conn_state', uint8_t),
        ('conn_state_reported', uint8_t),
        ('peer_conn_type', (uint16_t*2)),
    ]
avrcp_service_ctx=random_type_1807
class random_type_1808(Structure):
    _fields_ = [
        ('msg_id', uint32_t),
        ('param', (uintptr_t*4)),
    ]
class random_type_1809(Structure):
    _fields_ = [
        ('bd_addr', (uint8_t*6)),
        ('have_key', uint8_t),
        ('key_type', uint8_t),
        ('name', (uint8_t*32)),
        ('class_of_device', int32_t),
        ('link_key', (uint8_t*16)),
    ]
avrcp_tg_conn_state_changed_callback=td_u32
class random_type_1810(Structure):
    _fields_ = [
        ('conn_state_changed_cb', avrcp_tg_conn_state_changed_callback),
    ]
avrcp_tg_callbacks_t=random_type_1810
class random_type_1811(Structure):
    _fields_ = [
        ('addr', (uint8_t*6)),
        ('type', uint8_t),
    ]
class random_type_1812(Structure):
    _fields_ = [
        ('size', uint32_t),
        ('capability_id', uint8_t),
        ('count', uint8_t),
        ('capability', (uint8_t*1)),
    ]
class random_type_1813(Structure):
    _fields_ = [
        ('size', uint32_t),
        ('data', (uint8_t*1)),
    ]
avrcp_evt_param_inf=random_type_1813
class random_type_1814(Structure):
    _fields_ = [
        ('size', uint32_t),
        ('id', uint8_t),
    ]
bt_sdk_get_capabilities_req_stru=random_type_1814
bt_sdk_set_absolute_vol_rsp_stru=random_type_1814
class random_type_1815(Structure):
    _fields_ = [
        ('size', uint32_t),
        ('state_flag', uint8_t),
        ('op_id', uint8_t),
        ('vendor_unique_id', uint16_t),
    ]
td_ulong=td_u32
class random_type_1816(Structure):
    _fields_ = [
        ('event_id', td_u8),
        ('pad', (td_u8*3)),
        ('event_status', (td_u8*8)),
    ]
bt_avrcp_tg_evt_status_param=random_type_1816
class random_type_1817(Structure):
    _fields_ = [
        ('msg_id', uint32_t),
        ('param', (uintptr_t*4)),
    ]
class random_type_1818(Structure):
    _fields_ = [
        ('id', uint32_t),
        ('func', avrcp_srv_msg_handle_func),
    ]
hfp_hf_event_handle_func=td_u32
class random_type_1819(Structure):
    _fields_ = [
        ('event', uint16_t),
        ('func', hfp_hf_event_handle_func),
    ]
hfp_hf_event_struct=random_type_1819
class random_type_1820(Structure):
    _fields_ = [
        ('active_dev_addr', bd_addr_t),
        ('dev_hdl', uintptr_t),
        ('conn_hdl', uintptr_t),
        ('sco_hdl', uintptr_t),
    ]
hfp_hf_ctx_t=random_type_1820
class random_type_1821(Structure):
    _fields_ = [
        ('addr', (uint8_t*6)),
        ('type', uint8_t),
    ]
hfp_hf_conn_state_changed_callback=td_u32
hfp_hf_sco_conn_state_changed_callback=td_u32
hfp_hf_battery_level_changed_callback=td_u32
hfp_hf_signal_strength_changed_callback=td_u32
hfp_hf_registration_status_changed_callback=td_u32
hfp_hf_roaming_status_changed_callback=td_u32
hfp_hf_operator_selection_callback=td_u32
hfp_hf_subscriber_number_callback=td_u32
hfp_hf_voice_recognition_status_changed_callback=td_u32
hfp_hf_in_band_ring_tone_changed_callback=td_u32
hfp_hf_volume_changed_callback=td_u32
hfp_hf_call_changed_callback=td_u32
hfp_hf_dial_error_code_callback=td_u32
hfp_hf_codec_changed_callback=td_u32
class random_type_1822(Structure):
    _fields_ = [
        ('conn_state_changed_cb', hfp_hf_conn_state_changed_callback),
        ('sco_conn_state_changed_cb', hfp_hf_sco_conn_state_changed_callback),
        ('battery_level_changed_cb', hfp_hf_battery_level_changed_callback),
        ('signal_strength_changed_cb', hfp_hf_signal_strength_changed_callback),
        ('registration_status_changed_cb', hfp_hf_registration_status_changed_callback),
        ('roaming_status_changed_cb', hfp_hf_roaming_status_changed_callback),
        ('operator_selection_cb', hfp_hf_operator_selection_callback),
        ('subscriber_number_cb', hfp_hf_subscriber_number_callback),
        ('voice_recognition_status_changed_cb', hfp_hf_voice_recognition_status_changed_callback),
        ('in_band_ring_tone_changed_cb', hfp_hf_in_band_ring_tone_changed_callback),
        ('volume_changed_cb', hfp_hf_volume_changed_callback),
        ('call_changed_cb', hfp_hf_call_changed_callback),
        ('dial_error_code_cb', hfp_hf_dial_error_code_callback),
        ('codec_changed_cb', hfp_hf_codec_changed_callback),
    ]
hfp_hf_callbacks_t=random_type_1822
hfp_sco_connect_state_t=td_u8
hfp_volume_type_t=td_u8
class random_type_1824(Structure):
    _fields_ = [
        ('uuid_len', uint8_t),
        ('uuid', (uint8_t*16)),
    ]
class random_type_1825(Structure):
    _fields_ = [
        ('sdk_code', uint32_t),
        ('bts_code', errcode_t),
    ]
hfp_hf_code_pair=random_type_1825
class random_type_1826(Structure):
    _fields_ = [
        ('dev_hdl', uintptr_t),
        ('addr', bd_addr_t),
    ]
hfp_hf_remote_device_info=random_type_1826
class random_type_1827(Structure):
    _fields_ = [
        ('mode', uint8_t),
        ('format', uint8_t),
        ('operator_len', uint8_t),
        ('operator_name', (int8_t*1)),
    ]
btsdk_hfp_cops_info_stru=random_type_1827
class random_type_1828(Structure):
    _fields_ = [
        ('type', uint8_t),
        ('service', uint8_t),
        ('num_len', uint8_t),
        ('number', (int8_t*32)),
        ('name_len', uint8_t),
        ('alpha_str', (int8_t*1)),
    ]
btsdk_hfp_phone_info_stru=random_type_1828
class random_type_1829(Structure):
    _fields_ = [
        ('idx', uint8_t),
        ('dir', uint8_t),
        ('status', uint8_t),
        ('mode', uint8_t),
        ('mpty', uint8_t),
        ('type', uint8_t),
        ('num_len', uint8_t),
        ('number', (int8_t*1)),
    ]
btsdk_hfp_clcc_info_stru=random_type_1829
class random_type_1830(Structure):
    _fields_ = [
        ('cmd_code', uint16_t),
        ('result_code', uint8_t),
    ]
btsdk_hfp_at_cmd_result_stru=random_type_1830
hfp_hf_codec_id_t=td_u8
class random_type_1831(Structure):
    _fields_ = [
        ('msg_id', uint32_t),
        ('param', (uintptr_t*4)),
    ]
hfp_hf_accept_call_action_type_t=td_u8
class random_type_1832(Structure):
    _fields_ = [
        ('service', uint8_t),
        ('call', uint8_t),
        ('callsetup', uint8_t),
        ('callheld', uint8_t),
        ('signal', uint8_t),
        ('roam', uint8_t),
        ('battchg', uint8_t),
    ]
hfp_cind_info_t=random_type_1832
hfp_ag_event_handle_func=td_u32
class random_type_1833(Structure):
    _fields_ = [
        ('event', uint16_t),
        ('func', hfp_ag_event_handle_func),
    ]
hfp_ag_event_struct=random_type_1833
class random_type_1834(Structure):
    _fields_ = [
        ('active_dev_addr', bd_addr_t),
        ('dev_hdl', uintptr_t),
        ('conn_hdl', uintptr_t),
        ('sco_hdl', uintptr_t),
    ]
hfp_ag_ctx_t=random_type_1834
class random_type_1835(Structure):
    _fields_ = [
        ('addr', (uint8_t*6)),
        ('type', uint8_t),
    ]
hfp_ag_conn_state_changed_callback=td_u32
hfp_ag_sco_conn_state_changed_callback=td_u32
hfp_ag_codec_changed_callback=td_u32
hfp_ag_volume_changed_callback=td_u32
hfp_ag_dial_req_callback=td_u32
hfp_ag_mem_dial_req_callback=td_u32
hfp_ag_lastnum_dial_req_callback=td_u32
hfp_ag_answer_call_req_callback=td_u32
hfp_ag_cancel_call_req_callback=td_u32
hfp_ag_dtmf_req_callback=td_u32
hfp_ag_subscriber_number_req_callback=td_u32
hfp_ag_current_calls_req_callback=td_u32
hfp_ag_current_indicator_req_callback=td_u32
hfp_ag_network_operator_format_req_callback=td_u32
hfp_ag_network_operator_req_callback=td_u32
class random_type_1836(Structure):
    _fields_ = [
        ('conn_state_changed_cb', hfp_ag_conn_state_changed_callback),
        ('sco_conn_state_changed_cb', hfp_ag_sco_conn_state_changed_callback),
        ('codec_changed_cb', hfp_ag_codec_changed_callback),
        ('volume_changed_cb', hfp_ag_volume_changed_callback),
        ('dial_req_cb', hfp_ag_dial_req_callback),
        ('mem_dial_req_cb', hfp_ag_mem_dial_req_callback),
        ('lastnum_dial_req_cb', hfp_ag_lastnum_dial_req_callback),
        ('answer_call_req_cb', hfp_ag_answer_call_req_callback),
        ('cancel_call_req_cb', hfp_ag_cancel_call_req_callback),
        ('dtmf_req_cb', hfp_ag_dtmf_req_callback),
        ('subscriber_number_req_callback', hfp_ag_subscriber_number_req_callback),
        ('current_calls_req_callback', hfp_ag_current_calls_req_callback),
        ('current_indicator_req_callback', hfp_ag_current_indicator_req_callback),
        ('network_operator_format_req_callback', hfp_ag_network_operator_format_req_callback),
        ('network_operator_req_callback', hfp_ag_network_operator_req_callback),
    ]
hfp_ag_callbacks_t=random_type_1836
hfp_ag_codec_id_t=td_u8
hfp_ag_type_t=td_u8
class random_type_1837(Structure):
    _fields_ = [
        ('sdk_code', uint32_t),
        ('bts_code', errcode_t),
    ]
hfp_ag_code_pair=random_type_1837
class random_type_1838(Structure):
    _fields_ = [
        ('dev_hdl', uintptr_t),
        ('addr', bd_addr_t),
    ]
hfp_ag_remote_device_info=random_type_1838
pbtsdk_hfp_clcc_info_stru=td_u32
class random_type_1839(Structure):
    _fields_ = [
        ('idx', uint8_t),
        ('dir', uint8_t),
        ('status', uint8_t),
        ('mode', uint8_t),
        ('mpty', uint8_t),
        ('type', uint8_t),
        ('num_len', uint8_t),
        ('number', (int8_t*1)),
    ]
pbtsdk_hfp_phone_info_stru=td_u32
class random_type_1840(Structure):
    _fields_ = [
        ('type', uint8_t),
        ('service', uint8_t),
        ('num_len', uint8_t),
        ('number', (int8_t*32)),
        ('name_len', uint8_t),
        ('alpha_str', (int8_t*1)),
    ]
pbtsdk_hfp_cops_info_stru=td_u32
class random_type_1841(Structure):
    _fields_ = [
        ('mode', uint8_t),
        ('format', uint8_t),
        ('operator_len', uint8_t),
        ('operator_name', (int8_t*1)),
    ]
pbtsdk_hfp_cind_info_stru=td_u32
class random_type_1842(Structure):
    _fields_ = [
        ('service', uint8_t),
        ('call', uint8_t),
        ('callsetup', uint8_t),
        ('callheld', uint8_t),
        ('signal', uint8_t),
        ('roam', uint8_t),
        ('battchg', uint8_t),
    ]
class random_type_1843(Structure):
    _fields_ = [
        ('msg_id', uint32_t),
        ('param', (uintptr_t*4)),
    ]
hfp_ag_action_t=td_u8
class random_type_1844(Structure):
    _fields_ = [
        ('type', uint8_t),
        ('service', uint8_t),
        ('num_len', uint8_t),
        ('number', (int8_t*32)),
        ('name_len', uint8_t),
        ('alpha_str', (int8_t*1)),
    ]
hfp_phone_info_t=random_type_1844
hfp_ag_network_event_t=td_u8
class random_type_1845(Structure):
    _fields_ = [
        ('idx', uint8_t),
        ('dir', uint8_t),
        ('status', uint8_t),
        ('mode', uint8_t),
        ('mpty', uint8_t),
        ('type', uint8_t),
        ('num_len', uint8_t),
        ('number', (int8_t*1)),
    ]
hfp_clcc_info_t=random_type_1845
hfp_ag_last_t=td_u8
hfp_ag_cmee_error_t=td_u8
class random_type_1846(Structure):
    _fields_ = [
        ('mode', uint8_t),
        ('format', uint8_t),
        ('operator_len', uint8_t),
        ('operator_name', (int8_t*1)),
    ]
hfp_cops_info_t=random_type_1846
class random_type_1847(Structure):
    _fields_ = [
        ('state_changed_cb', pbap_pce_conn_state_changed_callback),
        ('phone_book_sized_cb', pbap_pce_phone_book_size_callback),
        ('trans_complete_cb', pbap_pce_trans_complete_callback),
        ('receive_data_cb', pbap_pce_receive_data_callback),
        ('error_code_cb', pbap_pce_error_code_callback),
    ]
class random_type_1848(Structure):
    _fields_ = [
        ('addr', (uint8_t*6)),
        ('type', uint8_t),
    ]
class random_type_1849(Structure):
    _fields_ = [
        ('sdk_code', uint32_t),
        ('bts_code', uint32_t),
    ]
pbap_code_pair=random_type_1849
class random_type_1850(Structure):
    _fields_ = [
        ('mask', uint16_t),
        ('filter', (uint8_t*8)),
        ('max_count', uint16_t),
        ('list_offset', uint16_t),
        ('order', uint8_t),
        ('format', uint8_t),
        ('search_val', td_u32),
        ('search_attrib', uint8_t),
        ('missed_calls', uint8_t),
        ('pb_size', uint16_t),
    ]
bt_sdk_pbap_param_stru=random_type_1850
class random_type_1851(Structure):
    _fields_ = [
        ('first', uint8_t),
        ('last', uint8_t),
        ('filesize', uint32_t),
        ('cursize', uint32_t),
        ('filename', (uint8_t*1)),
    ]
bt_ipc_pbap_status_ev_arg=random_type_1851
p_bt_sdk_pbap_param_stru=td_u32
class random_type_1852(Structure):
    _fields_ = [
        ('event_cbk', td_u32),
        ('write_func', td_u32),
    ]
sdk_pce_inst_stru=random_type_1852
bt_sdk_pbap_event_type=td_u8
class random_type_1853(Structure):
    _fields_ = [
        ('property_selector', (td_uchar*8)),
        ('vcard_selector', (td_uchar*8)),
        ('max_list_count', td_u16),
        ('list_offset', td_u16),
        ('name', td_u32),
        ('format', td_uchar),
        ('reset_new_missed_calls', td_uchar),
        ('vcard_selector_operator', td_uchar),
    ]
class random_type_1854(Structure):
    _fields_ = [
        ('vcard_selector', (td_uchar*8)),
        ('max_list_count', td_u16),
        ('list_offset', td_u16),
        ('name', td_u32),
        ('search_value', td_u32),
        ('search_property', td_uchar),
        ('order', td_uchar),
        ('reset_new_missed_calls', td_uchar),
        ('vcard_selector_operator', td_uchar),
    ]
class random_type_1855(Structure):
    _fields_ = [
        ('property_selector', (td_uchar*8)),
        ('name', td_u32),
        ('format', td_uchar),
    ]
class random_type_1856(Structure):
    _fields_ = [
        ('receive_data_cb', spp_receive_data_callback),
        ('conn_state_changed_cb', spp_conn_state_changed_callback),
    ]
class random_type_1857(Structure):
    _fields_ = [
        ('service_hdl', uint32_t),
        ('server_id', int8_t),
        ('client_id', int8_t),
        ('access_cnt', int8_t),
        ('deleted', td_bool),
        ('wait_accept', td_bool),
        ('accept_sem', osal_semaphore),
    ]
spp_server=random_type_1857
class random_type_1858(Structure):
    _fields_ = [
        ('sem', td_u32),
    ]
class random_type_1859(Structure):
    _fields_ = [
        ('dev_hdl', uint32_t),
        ('uuid', bt_uuid_t),
        ('conn_hdl', uint32_t),
        ('client_id', int8_t),
        ('connected', td_bool),
        ('access_cnt', int8_t),
        ('deleted', td_bool),
    ]
spp_client=random_type_1859
class random_type_1860(Structure):
    _fields_ = [
        ('uuid_len', uint8_t),
        ('uuid', (uint8_t*16)),
    ]
bt_sdk_spp_event_type=td_u8
class random_type_1861(Structure):
    _fields_ = [
        ('msg_id', uint32_t),
        ('param', (uintptr_t*4)),
    ]
class random_type_1862(Structure):
    _fields_ = [
        ('addr', (uint8_t*6)),
        ('type', uint8_t),
    ]
spp_socket_type_t=td_u8
class random_type_1863(Structure):
    _fields_ = [
        ('uuid', bt_uuid_t),
        ('socket_type', spp_socket_type_t),
        ('is_encrypt', td_bool),
    ]
spp_create_socket_para_t=random_type_1863
class random_type_1864(Structure):
    _fields_ = [
        ('active_dev_addr', bd_addr_t),
        ('dev_hdl', uintptr_t),
        ('conn_hdl', uintptr_t),
        ('acl_state', uint8_t),
        ('net_state', uint8_t),
        ('ipv4_only', uint8_t),
    ]
pan_service_ctx_t=random_type_1864
class random_type_1865(Structure):
    _fields_ = [
        ('addr', (uint8_t*6)),
        ('type', uint8_t),
    ]
class random_type_1866(Structure):
    _fields_ = [
        ('net_state_cb', pan_net_state_callback),
        ('rpt_data_cb', pan_rpt_data_callback),
    ]
class random_type_1867(Structure):
    _fields_ = [
        ('dev_hdl', uintptr_t),
        ('addr', bd_addr_t),
    ]
btsdk_pan_rpt_net_state=td_u32
btsdk_pan_rpt_filter_result=td_u32
btsdk_pan_rpt_ethernet_packet=td_u32
class random_type_1868(Structure):
    _fields_ = [
        ('net_state_cbk', btsdk_pan_rpt_net_state),
        ('filter_result_cbk', btsdk_pan_rpt_filter_result),
        ('ethernet_cbk', btsdk_pan_rpt_ethernet_packet),
    ]
btsdkpan_cbk=random_type_1868
class random_type_1869(Structure):
    _fields_ = [
        ('msg_id', uint32_t),
        ('param', (uintptr_t*4)),
    ]
bt_audio_listener_cb=td_u32
bt_audio_event_type=td_u8
bt_avrcp_tg_pass_through_response_cbks=td_u32
bt_avrcp_tg_get_media_status_cbks=td_u32
class random_type_1870(Structure):
    _fields_ = [
        ('notify_pass_through_status_cbk', bt_avrcp_tg_pass_through_response_cbks),
        ('get_media_status_cbk', bt_avrcp_tg_get_media_status_cbks),
    ]
bt_avrcp_tg_bts_cbk=random_type_1870
class random_type_1871(Structure):
    _fields_ = [
        ('event_id', td_u8),
        ('pad', (td_u8*3)),
        ('event_status', (td_u8*8)),
    ]
class random_type_1872(Structure):
    _fields_ = [
        ('timer', td_u32),
        ('handler', td_u32),
        ('data', td_u32),
        ('interval', td_u32),
    ]
class random_type_1873(Structure):
    _fields_ = [
        ('codec_type', td_u8),
        ('cap_len', td_u8),
        ('payload', td_u16),
        ('codec_caps', (td_u8*16)),
    ]
class random_type_1874(Structure):
    _fields_ = [
        ('addr', (uint8_t*6)),
        ('type', uint8_t),
    ]
bt_audio_port_type=td_u8
class random_type_1876(Union):
    _fields_ = [
        ('share_mem_id', td_u32),
    ]
class random_type_1877(Structure):
    _fields_ = [
        ('port_type', bt_audio_port_type),
        ('random_var_1875', random_type_1876),
    ]
bt_audio_port_params=random_type_1877
bt_audio_param_type=td_u8
mpc_function=td_u32
class random_type_1878(Structure):
    _fields_ = [
        ('op', uint8_t),
        ('func', mpc_function),
    ]
table_function=random_type_1878
class random_type_1880(Structure):
    _fields_ = [
        ('trans_id', uint8_t),
        ('sof', uint8_t),
        ('service_id', uint8_t),
        ('command_id', uint8_t),
    ]
mpc_header=random_type_1880
class random_type_1879(Structure):
    _fields_ = [
        ('header', mpc_header),
        ('payload_length', uint16_t),
        ('src_transid', uint8_t),
        ('resv', uint8_t),
        ('payload', (uint8_t*0)),
    ]
mpc_message=random_type_1879
class random_type_1882(Structure):
    _fields_ = [
        ('src_trans_id', uint8_t,4),
        ('dst_trans_id', uint8_t,4),
        ('cmd', uint8_t),
        ('length', uint16_t),
    ]
ipc_msg_header_t=random_type_1882
class random_type_1881(Structure):
    _fields_ = [
        ('hdr', ipc_msg_header_t),
        ('data', (uint8_t*0)),
    ]
ipc_msg_t=random_type_1881
ipc_flash_command=td_u8
class random_type_1885(Structure):
    _fields_ = [
        ('command', ipc_flash_command),
        ('type', uint8_t),
        ('id', uint8_t),
        ('length', uint32_t),
        ('address', uint32_t),
    ]
ipc_payload_flash_req=random_type_1885
class random_type_1886(Structure):
    _fields_ = [
        ('command', ipc_flash_command),
        ('type', uint8_t),
        ('id', uint8_t),
    ]
ipc_payload_flash_cnf=random_type_1886
ipc_payload_flash_ind=random_type_1886
ipc_bt_ap_hook_type_e=td_u8
class random_type_1891(Structure):
    _fields_ = [
        ('hook_type', ipc_bt_ap_hook_type_e),
        ('command', (uint8_t*256)),
        ('command_len', uint32_t),
    ]
ipc_payload_ap_bt_hook_data=random_type_1891
class random_type_1892(Structure):
    _fields_ = [
        ('hook_type', ipc_bt_ap_hook_type_e),
        ('addr', uint32_t),
        ('len', uint32_t),
        ('value', uint32_t),
    ]
ipc_payload_bt_ap_hook_data=random_type_1892
ipc_exception_test_command_e=td_u8
class random_type_1896(Structure):
    _fields_ = [
        ('command', ipc_exception_test_command_e),
    ]
ipc_payload_exception_test=random_type_1896
class random_type_1883(Union):
    _fields_ = [
        ('memory', ipc_payload_memory),
        ('flash_req', ipc_payload_flash_req),
        ('flash_cnf', ipc_payload_flash_cnf),
        ('flash_ind', ipc_payload_flash_ind),
        ('core_started', ipc_payload_core_started),
        ('rpc_payload', ipc_payload_rpc),
        ('request_reboot', ipc_payload_request_reboot),
        ('set_log_level', ipc_payload_set_log_level),
        ('ap_bt_hook_data', ipc_payload_ap_bt_hook_data),
        ('bt_ap_hook_data', ipc_payload_bt_ap_hook_data),
        ('bt_hifi_mem_req', ipc_payload_bt_hifi_mem_req),
        ('hifi_bt_mem_cnf', ipc_payload_hifi_bt_mem_cnf),
        ('ap_bt_calibration_param_req', ipc_payload_ap_bt_calibration_param_req),
        ('exception_test_cmd', ipc_payload_exception_test),
        ('mass_data_type', ipc_payload_mass_data_type),
        ('mass_data_extend_type', ipc_payload_mass_data_extend_type),
        ('hci_data_type', ipc_payload_hci_data_type),
        ('get_cal_freq_req', ipc_payload_get_cal_freq_req_t),
        ('get_cal_freq_cnf', ipc_payload_get_cal_freq_cnf_t),
        ('testsuite', ipc_payload_test_suite_string),
        ('bt_power_on_flag', ipc_payload_bt_power_on_t),
        ('dsp_power_on_flag', ipc_payload_dsp_power_on_t),
        ('temperature_change_flag', ipc_payload_temperature_t),
        ('updata_trng_signal', ipc_payload_updata_trng_t),
        ('adjust_slave_freq', ipc_payload_adjust_slave_freq_t),
        ('dsp_freq', ipc_payload_dsp_current_freq_t),
        ('bt_high_power', ipc_payload_high_power_t),
        ('bt_nv_info', ipc_payload_config_nv_info_t),
        ('bt_nv_customize_info', ipc_payload_bt_nv_customize_info_t),
    ]
ipc_payload=random_type_1883
class random_type_1884(Structure):
    _fields_ = [
        ('source_ptr', uint32_t),
        ('dest_ptr', uint32_t),
        ('length', uint16_t),
    ]
class random_type_1887(Structure):
    _fields_ = [
        ('reboot_cause', uint32_t),
    ]
class random_type_1888(Structure):
    _fields_ = [
        ('data', (uint8_t*3072)),
    ]
class random_type_1889(Structure):
    _fields_ = [
        ('requested_reboot_reason', uint16_t),
    ]
class random_type_1890(Structure):
    _fields_ = [
        ('log_level', uint32_t),
    ]
class random_type_1893(Structure):
    _fields_ = [
        ('size', uint32_t),
    ]
class random_type_1894(Structure):
    _fields_ = [
        ('result', uint32_t),
    ]
class random_type_1895(Structure):
    _fields_ = [
        ('params', (uint8_t*256)),
        ('length', uint32_t),
    ]
class random_type_1897(Structure):
    _fields_ = [
        ('type', uint8_t),
        ('event_id', uint8_t),
        ('info1', uint8_t),
        ('info2', uint8_t),
        ('info3', uint16_t),
    ]
class random_type_1898(Structure):
    _fields_ = [
        ('mass_data', ipc_payload_mass_data_type),
        ('data_len', uint8_t),
        ('data', (uint8_t*128)),
    ]
class random_type_1899(Structure):
    _fields_ = [
        ('data', (uint8_t*160)),
        ('length', uint16_t),
    ]
class random_type_1900(Structure):
    _fields_ = [
        ('clock_type', uint32_t),
    ]
class random_type_1901(Structure):
    _fields_ = [
        ('result', uint32_t),
    ]
class random_type_1902(Structure):
    _fields_ = [
        ('status', uint16_t),
        ('length', uint16_t),
        ('msg', (td_char*60)),
    ]
class random_type_1903(Structure):
    _fields_ = [
        ('flag', td_bool),
    ]
class random_type_1904(Structure):
    _fields_ = [
        ('flag', td_bool),
    ]
class random_type_1905(Structure):
    _fields_ = [
        ('flag', td_bool),
    ]
class random_type_1906(Structure):
    _fields_ = [
        ('updata', uint32_t),
    ]
class random_type_1907(Structure):
    _fields_ = [
        ('slave_freq', uint32_t),
    ]
class random_type_1908(Structure):
    _fields_ = [
        ('freq', uint32_t),
    ]
class random_type_1909(Structure):
    _fields_ = [
        ('power_switch', uint32_t),
    ]
class random_type_1910(Structure):
    _fields_ = [
        ('config_nv_1', uint16_t),
        ('config_nv_2', uint16_t),
        ('config_nv_3', uint16_t),
        ('config_nv_4', uint16_t),
        ('config_nv_5', (uint16_t*5)),
        ('config_nv_6', uint16_t),
        ('config_nv_7', uint16_t),
        ('config_nv_8', uint16_t),
        ('config_nv_9', uint16_t),
        ('config_nv_10', uint8_t),
        ('config_nv_11', int8_t),
        ('config_nv_12', int8_t),
        ('flag', uint8_t),
    ]
class random_type_1911(Structure):
    _fields_ = [
        ('nv_customize_config', uint32_t),
        ('nv_customize_config_reserved', uint32_t),
        ('nv_max_ble_tx_power_cfg_val', uint8_t),
        ('nv_max_gle_tx_power_cfg_val', uint8_t),
        ('fcc_flag', uint8_t),
        ('customize_four_k_support', uint8_t),
        ('customize_freq_map', (uint8_t*10)),
        ('reserved', (uint8_t*86)),
    ]
class random_type_1912(Structure):
    _fields_ = [
        ('hdr', ipc_msg_header_t),
        ('data', (uint8_t*0)),
    ]
class random_type_1913(Structure):
    _fields_ = [
        ('src_trans_id', uint8_t,4),
        ('dst_trans_id', uint8_t,4),
        ('cmd', uint8_t),
        ('length', uint16_t),
    ]
class random_type_1914(Structure):
    _fields_ = [
        ('data_len', uint16_t),
        ('data', (uint8_t*0)),
    ]
bth_data_t=random_type_1914
class random_type_1915(Union):
    _fields_ = [
        ('value', uint32_t),
        ('data', td_u32),
    ]
ipc_status_t=td_u8
bt_property_value=random_type_1915
bt_property_id=td_u16
bt_propertycallback=td_u32
class random_type_1916(Structure):
    _fields_ = [
        ('id', bt_property_id),
        ('data', bt_property_value),
        ('func', bt_propertycallback),
    ]
bt_property=random_type_1916
class random_type_1917(Union):
    _fields_ = [
        ('value', uint32_t),
        ('data', td_u32),
    ]
class random_type_1918(Structure):
    _fields_ = [
        ('sem', td_u32),
    ]
class random_type_1919(Union):
    _fields_ = [
        ('memory', ipc_payload_memory),
        ('flash_req', ipc_payload_flash_req),
        ('flash_cnf', ipc_payload_flash_cnf),
        ('flash_ind', ipc_payload_flash_ind),
        ('core_started', ipc_payload_core_started),
        ('rpc_payload', ipc_payload_rpc),
        ('request_reboot', ipc_payload_request_reboot),
        ('set_log_level', ipc_payload_set_log_level),
        ('ap_bt_hook_data', ipc_payload_ap_bt_hook_data),
        ('bt_ap_hook_data', ipc_payload_bt_ap_hook_data),
        ('bt_hifi_mem_req', ipc_payload_bt_hifi_mem_req),
        ('hifi_bt_mem_cnf', ipc_payload_hifi_bt_mem_cnf),
        ('ap_bt_calibration_param_req', ipc_payload_ap_bt_calibration_param_req),
        ('exception_test_cmd', ipc_payload_exception_test),
        ('mass_data_type', ipc_payload_mass_data_type),
        ('mass_data_extend_type', ipc_payload_mass_data_extend_type),
        ('hci_data_type', ipc_payload_hci_data_type),
        ('get_cal_freq_req', ipc_payload_get_cal_freq_req_t),
        ('get_cal_freq_cnf', ipc_payload_get_cal_freq_cnf_t),
        ('testsuite', ipc_payload_test_suite_string),
        ('bt_power_on_flag', ipc_payload_bt_power_on_t),
        ('dsp_power_on_flag', ipc_payload_dsp_power_on_t),
        ('temperature_change_flag', ipc_payload_temperature_t),
        ('updata_trng_signal', ipc_payload_updata_trng_t),
        ('adjust_slave_freq', ipc_payload_adjust_slave_freq_t),
        ('dsp_freq', ipc_payload_dsp_current_freq_t),
        ('bt_high_power', ipc_payload_high_power_t),
        ('bt_nv_info', ipc_payload_config_nv_info_t),
        ('bt_nv_customize_info', ipc_payload_bt_nv_customize_info_t),
    ]
class random_type_1920(Structure):
    _fields_ = [
        ('source_ptr', uint32_t),
        ('dest_ptr', uint32_t),
        ('length', uint16_t),
    ]
class random_type_1921(Structure):
    _fields_ = [
        ('command', ipc_flash_command),
        ('type', uint8_t),
        ('id', uint8_t),
        ('length', uint32_t),
        ('address', uint32_t),
    ]
class random_type_1922(Structure):
    _fields_ = [
        ('command', ipc_flash_command),
        ('type', uint8_t),
        ('id', uint8_t),
    ]
class random_type_1923(Structure):
    _fields_ = [
        ('reboot_cause', uint32_t),
    ]
class random_type_1924(Structure):
    _fields_ = [
        ('data', (uint8_t*3072)),
    ]
class random_type_1925(Structure):
    _fields_ = [
        ('requested_reboot_reason', uint16_t),
    ]
class random_type_1926(Structure):
    _fields_ = [
        ('log_level', uint32_t),
    ]
class random_type_1927(Structure):
    _fields_ = [
        ('hook_type', ipc_bt_ap_hook_type_e),
        ('command', (uint8_t*256)),
        ('command_len', uint32_t),
    ]
class random_type_1928(Structure):
    _fields_ = [
        ('hook_type', ipc_bt_ap_hook_type_e),
        ('addr', uint32_t),
        ('len', uint32_t),
        ('value', uint32_t),
    ]
class random_type_1929(Structure):
    _fields_ = [
        ('size', uint32_t),
    ]
class random_type_1930(Structure):
    _fields_ = [
        ('result', uint32_t),
    ]
class random_type_1931(Structure):
    _fields_ = [
        ('params', (uint8_t*256)),
        ('length', uint32_t),
    ]
class random_type_1932(Structure):
    _fields_ = [
        ('command', ipc_exception_test_command_e),
    ]
class random_type_1933(Structure):
    _fields_ = [
        ('type', uint8_t),
        ('event_id', uint8_t),
        ('info1', uint8_t),
        ('info2', uint8_t),
        ('info3', uint16_t),
    ]
class random_type_1934(Structure):
    _fields_ = [
        ('mass_data', ipc_payload_mass_data_type),
        ('data_len', uint8_t),
        ('data', (uint8_t*128)),
    ]
class random_type_1935(Structure):
    _fields_ = [
        ('data', (uint8_t*160)),
        ('length', uint16_t),
    ]
class random_type_1936(Structure):
    _fields_ = [
        ('clock_type', uint32_t),
    ]
class random_type_1937(Structure):
    _fields_ = [
        ('result', uint32_t),
    ]
class random_type_1938(Structure):
    _fields_ = [
        ('status', uint16_t),
        ('length', uint16_t),
        ('msg', (td_char*60)),
    ]
class random_type_1939(Structure):
    _fields_ = [
        ('flag', td_bool),
    ]
class random_type_1940(Structure):
    _fields_ = [
        ('flag', td_bool),
    ]
class random_type_1941(Structure):
    _fields_ = [
        ('flag', td_bool),
    ]
class random_type_1942(Structure):
    _fields_ = [
        ('updata', uint32_t),
    ]
class random_type_1943(Structure):
    _fields_ = [
        ('slave_freq', uint32_t),
    ]
class random_type_1944(Structure):
    _fields_ = [
        ('freq', uint32_t),
    ]
class random_type_1945(Structure):
    _fields_ = [
        ('power_switch', uint32_t),
    ]
class random_type_1946(Structure):
    _fields_ = [
        ('config_nv_1', uint16_t),
        ('config_nv_2', uint16_t),
        ('config_nv_3', uint16_t),
        ('config_nv_4', uint16_t),
        ('config_nv_5', (uint16_t*5)),
        ('config_nv_6', uint16_t),
        ('config_nv_7', uint16_t),
        ('config_nv_8', uint16_t),
        ('config_nv_9', uint16_t),
        ('config_nv_10', uint8_t),
        ('config_nv_11', int8_t),
        ('config_nv_12', int8_t),
        ('flag', uint8_t),
    ]
class random_type_1947(Structure):
    _fields_ = [
        ('nv_customize_config', uint32_t),
        ('nv_customize_config_reserved', uint32_t),
        ('nv_max_ble_tx_power_cfg_val', uint8_t),
        ('nv_max_gle_tx_power_cfg_val', uint8_t),
        ('fcc_flag', uint8_t),
        ('customize_four_k_support', uint8_t),
        ('customize_freq_map', (uint8_t*10)),
        ('reserved', (uint8_t*86)),
    ]
cores_t=td_u8
class random_type_1948(Structure):
    _fields_ = [
        ('name', td_u32),
        ('attr_bits', uint32_t),
        ('cb_mem', td_u32),
        ('cb_size', uint32_t),
        ('mq_mem', td_u32),
        ('mq_size', uint32_t),
    ]
class random_type_1949(Structure):
    _fields_ = [
        ('op', uint8_t),
        ('func', mpc_function),
    ]
class random_type_1950(Structure):
    _fields_ = [
        ('header', mpc_header),
        ('payload_length', uint16_t),
        ('src_transid', uint8_t),
        ('resv', uint8_t),
        ('payload', (uint8_t*0)),
    ]
class random_type_1951(Structure):
    _fields_ = [
        ('trans_id', uint8_t),
        ('sof', uint8_t),
        ('service_id', uint8_t),
        ('command_id', uint8_t),
    ]
sdk_ble_cbk=td_u32
class random_type_1952(Structure):
    _fields_ = [
        ('op', uint8_t),
        ('func', mpc_function),
    ]
class random_type_1953(Structure):
    _fields_ = [
        ('header', mpc_header),
        ('payload_length', uint16_t),
        ('src_transid', uint8_t),
        ('resv', uint8_t),
        ('payload', (uint8_t*0)),
    ]
class random_type_1954(Structure):
    _fields_ = [
        ('trans_id', uint8_t),
        ('sof', uint8_t),
        ('service_id', uint8_t),
        ('command_id', uint8_t),
    ]
class random_type_1955(Structure):
    _fields_ = [
        ('data_len', uint16_t),
        ('data', td_u32),
    ]
bt_at_data_t=random_type_1955
class random_type_1956(Structure):
    _fields_ = [
        ('op', uint8_t),
        ('func', mpc_function),
    ]
class random_type_1957(Structure):
    _fields_ = [
        ('header', mpc_header),
        ('payload_length', uint16_t),
        ('src_transid', uint8_t),
        ('resv', uint8_t),
        ('payload', (uint8_t*0)),
    ]
class random_type_1958(Structure):
    _fields_ = [
        ('trans_id', uint8_t),
        ('sof', uint8_t),
        ('service_id', uint8_t),
        ('command_id', uint8_t),
    ]
mc_start_up_status=td_u8
class hci_address_stru(Structure):
    _fields_ = [
        ('bd', (uint8_t*6)),
        ('atype', uint8_t),
        ('cap', uint8_t),
    ]
class smp_key(Structure):
    _fields_ = [
        ('ltk_len', uint8_t),
        ('fea', uint8_t),
        ('div', (uint8_t*2)),
        ('ltk', (uint8_t*16)),
        ('ediv', (uint8_t*2)),
        ('rand', (uint8_t*8)),
        ('irk', (uint8_t*16)),
        ('csrk', (uint8_t*16)),
    ]
smp_key_stru_t=smp_key
class random_type_1959(Structure):
    _fields_ = [
        ('addr', hci_address_stru),
        ('key', smp_key_stru_t),
    ]
smp_key_store_stru_t=random_type_1959
class random_type_1960(Structure):
    _fields_ = [
        ('bd_addr', (uint8_t*6)),
        ('phone_bd_addr', (uint8_t*6)),
        ('bd_name', (uint8_t*32)),
    ]
mc_dev_info_param=random_type_1960
class random_type_1961(Structure):
    _fields_ = [
        ('header', mpc_header),
        ('payload_length', uint16_t),
        ('src_transid', uint8_t),
        ('resv', uint8_t),
        ('payload', (uint8_t*0)),
    ]
class random_type_1962(Structure):
    _fields_ = [
        ('trans_id', uint8_t),
        ('sof', uint8_t),
        ('service_id', uint8_t),
        ('command_id', uint8_t),
    ]
bt_data_config_id=td_u8
class random_type_1963(Structure):
    _fields_ = [
        ('config_id', bt_data_config_id),
        ('config_offset', uint16_t),
        ('config_length', uint16_t),
    ]
bt_config_data_t=random_type_1963
class random_type_1964(Structure):
    _fields_ = [
        ('bd_addr', (uint8_t*6)),
        ('phone_bd_addr', (uint8_t*6)),
        ('bd_name', (uint8_t*32)),
        ('config_data', (uint8_t*541)),
    ]
product_data_config=random_type_1964
class random_type_1965(Structure):
    _fields_ = [
        ('permanent', td_bool),
        ('encrypted', td_bool),
        ('non_upgrade', td_bool),
        ('reserve', uint8_t),
    ]
class random_type_1966(Structure):
    _fields_ = [
        ('bd_addr', (uint8_t*6)),
        ('phone_bd_addr', (uint8_t*6)),
        ('bd_name', (uint8_t*32)),
        ('config_data', (uint8_t*541)),
    ]
class random_type_1967(Structure):
    _fields_ = [
        ('addr', hci_address_stru),
        ('key', smp_key_stru_t),
    ]
class random_type_1968(Structure):
    _fields_ = [
        ('addr', (uint8_t*6)),
        ('type', uint8_t),
    ]
class random_type_1969(Structure):
    _fields_ = [
        ('data_len', uint16_t),
        ('data', td_u32),
    ]
class random_type_1970(Structure):
    _fields_ = [
        ('bondable', uint8_t),
        ('io_capability', uint8_t),
        ('sc_enable', uint8_t),
        ('sc_mode', uint8_t),
    ]
class random_type_1971(Structure):
    _fields_ = [
        ('bd', (uint8_t*6)),
        ('atype', uint8_t),
        ('cap', uint8_t),
    ]
class random_type_1972(Structure):
    _fields_ = [
        ('addr_type', uint8_t),
        ('use_private_addr', uint8_t),
    ]
bth_gap_addr_info=random_type_1972
class random_type_1973(Structure):
    _fields_ = [
        ('op', uint8_t),
        ('func', mpc_function),
    ]
class random_type_1974(Structure):
    _fields_ = [
        ('header', mpc_header),
        ('payload_length', uint16_t),
        ('src_transid', uint8_t),
        ('resv', uint8_t),
        ('payload', (uint8_t*0)),
    ]
class random_type_1975(Structure):
    _fields_ = [
        ('trans_id', uint8_t),
        ('sof', uint8_t),
        ('service_id', uint8_t),
        ('command_id', uint8_t),
    ]
class random_type_1977(Structure):
    _fields_ = [
        ('add_tx_power', uint8_t),
        ('periodic_adv_interval_min', uint16_t),
        ('periodic_adv_interval_max', uint16_t),
    ]
bth_periodic_adv_params=random_type_1977
class random_type_1978(Structure):
    _fields_ = [
        ('data_length', uint16_t),
        ('prd_adv_data', (uint8_t*0)),
    ]
bth_periodic_adv_data=random_type_1978
class random_type_1976(Structure):
    _fields_ = [
        ('adv_handle', uint8_t),
        ('adv_param', bth_periodic_adv_params),
        ('adv_data', bth_periodic_adv_data),
    ]
bth_gap_set_periodic_adv_param=random_type_1976
class random_type_1980(Structure):
    _fields_ = [
        ('addr_number', uint8_t),
        ('adv_sid', uint8_t),
        ('addrs', (bth_address*0)),
    ]
bth_periodic_adv_list=random_type_1980
class random_type_1979(Structure):
    _fields_ = [
        ('add_dev', uint8_t),
        ('dev_list', bth_periodic_adv_list),
    ]
bth_gap_set_adv_list=random_type_1979
class random_type_1981(Structure):
    _fields_ = [
        ('bd', (uint8_t*6)),
        ('atype', uint8_t),
        ('cap', uint8_t),
    ]
class random_type_1982(Structure):
    _fields_ = [
        ('ext_adv_param', bth_gap_ext_adv_param),
        ('ext_adv_data', bth_gap_ext_adv_data),
        ('ext_adv_en', bth_gap_ext_adv_en),
    ]
class random_type_1983(Structure):
    _fields_ = [
        ('adv_handle', uint8_t),
        ('adv_type', uint8_t),
        ('adv_event_flag', uint8_t),
        ('primary_adv_channel_map', uint8_t),
        ('primary_adv_interval_min', uint32_t),
        ('primary_adv_interval_max', uint32_t),
        ('own_addr', (uint8_t*6)),
        ('peer_addr_type', uint8_t),
        ('peer_addr', (uint8_t*6)),
        ('adv_filter_policy', uint8_t),
        ('adv_tx_power', uint8_t),
        ('primary_adv_phy', uint8_t),
        ('secondary_adv_max_skip', uint8_t),
        ('secondary_adv_phy', uint8_t),
        ('advertising_sid', uint8_t),
        ('scan_request_notification_enable', uint8_t),
    ]
class random_type_1984(Structure):
    _fields_ = [
        ('fragment_preference', uint8_t),
        ('eir_flag', uint8_t),
        ('adv_data_length', uint16_t),
        ('scan_rsp_data_length', uint16_t),
        ('adv_data', td_u32),
        ('scan_rsp_data', td_u32),
    ]
class random_type_1985(Structure):
    _fields_ = [
        ('duration', uint16_t),
        ('max_extended_adv_events', uint8_t),
        ('priority', uint8_t),
    ]
class random_type_1986(Structure):
    _fields_ = [
        ('scan_filter_policy', uint8_t),
        ('scan_phys', uint8_t),
        ('scan_type_1m', uint8_t),
        ('scan_type_coded', uint8_t),
        ('scan_interval_1m', uint16_t),
        ('scan_window_1m', uint16_t),
        ('scan_interval_coded', uint16_t),
        ('scan_window_coded', uint16_t),
    ]
class random_type_1987(Structure):
    _fields_ = [
        ('addr', bth_address),
        ('interval_min', uint16_t),
        ('interval_max', uint16_t),
        ('slave_latency', uint16_t),
        ('timeout_multiplier', uint16_t),
    ]
class random_type_1989(Structure):
    _fields_ = [
        ('filter_policy', uint8_t),
        ('adv_sid', uint8_t),
        ('adv_addr_type', uint8_t),
        ('peer_addr', (uint8_t*6)),
        ('skip', uint16_t),
        ('sync_timeout', uint16_t),
    ]
bth_periodic_adv_sync_parm=random_type_1989
class random_type_1988(Structure):
    _fields_ = [
        ('sync_id', uint8_t),
        ('adv_sync_param', bth_periodic_adv_sync_parm),
    ]
bth_gap_create_adv_syn_param=random_type_1988
class random_type_1990(Structure):
    _fields_ = [
        ('ext_scan_param', bth_gap_ext_scan_param),
        ('ext_scan_en', bth_gap_ext_scan_en),
    ]
class random_type_1991(Structure):
    _fields_ = [
        ('filter_duplicate', uint8_t),
        ('limited', uint8_t),
        ('duration', uint16_t),
        ('period', uint16_t),
    ]
class random_type_1992(Structure):
    _fields_ = [
        ('global_value_flag', uint32_t),
        ('global_value', uint32_t),
    ]
bth_set_global_value=random_type_1992
class random_type_1993(Structure):
    _fields_ = [
        ('conn_interval_min', uint16_t),
        ('conn_interval_max', uint16_t),
        ('conn_interval_coded_min', uint16_t),
        ('conn_interval_coded_max', uint16_t),
    ]
bth_hci_tgap_le_conn_param=random_type_1993
class random_type_1994(Structure):
    _fields_ = [
        ('isset', uint8_t),
        ('mask', (uint8_t*8)),
    ]
bth_hci_tgap_le_set_event_mask=random_type_1994
class random_type_1995(Structure):
    _fields_ = [
        ('le_att_mtu_default', uint16_t),
        ('le_att_mtu_le_max', uint16_t),
    ]
bth_gatt_service_type=td_u8
class random_type_1996(Structure):
    _fields_ = [
        ('server_handle', uint32_t),
        ('service_uuid', bth_gatt_uuid),
        ('service_type', bth_gatt_service_type),
    ]
class random_type_1997(Structure):
    _fields_ = [
        ('is_short_uuid', uint32_t),
        ('short_uuid', uint16_t),
        ('reserve', uint16_t),
        ('long_uuid', bth_att_uuid),
    ]
class random_type_1998(Structure):
    _fields_ = [
        ('u', (uint8_t*16)),
    ]
class random_type_1999(Structure):
    _fields_ = [
        ('service_handle', uint32_t),
        ('service', bth_gatt_start_service),
    ]
class random_type_2000(Structure):
    _fields_ = [
        ('service_uuid', bth_gatt_uuid),
        ('service_handle', uint32_t),
    ]
class random_type_2001(Structure):
    _fields_ = [
        ('service_handle', uint32_t),
        ('characteristic', bth_gatt_characteristic),
        ('data_size', uint32_t),
        ('data', (uint8_t*0)),
    ]
class random_type_2002(Structure):
    _fields_ = [
        ('service_handle', uint16_t),
        ('characteristic_uuid', bth_gatt_uuid),
        ('declare_handle', uint16_t),
        ('value_handle', uint16_t),
        ('broadcastable_flag', uint32_t),
        ('readable_flag', uint32_t),
        ('writable_flag', uint32_t),
        ('writable_without_response_flag', uint32_t),
        ('signed_writable_flag', uint32_t),
        ('notifiable_flag', uint32_t),
        ('indicatable_flag', uint32_t),
        ('authentication_flag', uint32_t),
        ('authorization_flag', uint32_t),
        ('mitm_flag', uint32_t),
        ('has_extended_properties', uint32_t),
    ]
bth_gatt_descriptor_type=td_u8
class random_type_2005(Structure):
    _fields_ = [
        ('is_reliable_write', uint32_t),
        ('is_auxiliaries_writable', uint32_t),
    ]
class random_type_2006(Structure):
    _fields_ = [
        ('is_subscribeto_notification', uint32_t),
        ('is_subscribeto_indication', uint32_t),
    ]
class random_type_2007(Structure):
    _fields_ = [
        ('is_broadcast', uint32_t),
    ]
class random_type_2008(Structure):
    _fields_ = [
        ('format', uint8_t),
        ('exponent', uint8_t),
        ('unit', bth_gatt_uuid),
        ('name_space', uint8_t),
        ('description', bth_gatt_uuid),
    ]
class random_type_2009(Union):
    _fields_ = [
        ('bth_characteristic_extended_properties', random_type_2005),
        ('client_characteristic_configuration', random_type_2006),
        ('server_characteristic_configuration', random_type_2007),
        ('characteristic_format', random_type_2008),
    ]
class random_type_2010(Structure):
    _fields_ = [
        ('descriptor_type', bth_gatt_descriptor_type),
        ('descriptor_uuid', bth_gatt_uuid),
        ('random_var_2004', random_type_2009),
        ('data_size', uint32_t),
        ('data', (uint8_t*0)),
    ]
bth_gatt_descriptor_value=random_type_2010
class random_type_2003(Structure):
    _fields_ = [
        ('characteristic_handle', uint32_t),
        ('permission', uint8_t),
        ('descriptor_value', bth_gatt_descriptor_value),
    ]
class random_type_2011(Structure):
    _fields_ = [
        ('service_uuid', bth_gatt_uuid),
        ('service_handle', uint16_t),
    ]
class random_type_2013(Structure):
    _fields_ = [
        ('bd', (uint8_t*6)),
        ('atype', uint8_t),
        ('cap', uint8_t),
    ]
class random_type_2014(Structure):
    _fields_ = [
        ('characteristic_handle', uint16_t),
        ('data_size', uint16_t),
        ('data', (uint8_t*0)),
    ]
class random_type_2015(Structure):
    _fields_ = [
        ('start_hdl', uint16_t),
        ('end_hdl', uint16_t),
        ('uuid', bth_gatt_uuid),
        ('data_size', uint16_t),
        ('data', (uint8_t*0)),
    ]
class random_type_2016(Structure):
    _fields_ = [
        ('descriptor_handle', uint16_t),
        ('descriptor_value', bth_gatt_descriptor_value),
    ]
class random_type_2017(Structure):
    _fields_ = [
        ('addr', bth_address),
        ('att_handle', uint16_t),
        ('offset', uint16_t),
        ('security_mode', uint8_t),
        ('timeout', uint32_t),
    ]
class random_type_2018(Structure):
    _fields_ = [
        ('addr', bth_address),
        ('handle_count', uint16_t),
        ('handle_set', (uint16_t*5)),
        ('offset', uint16_t),
        ('flags', uint16_t),
    ]
class random_type_2019(Structure):
    _fields_ = [
        ('addr', bth_address),
        ('handle_count', uint8_t),
        ('handle_set', (uint16_t*5)),
        ('value_count', uint8_t),
        ('value', ((uint8_t*10)*5)),
        ('flags', uint8_t),
    ]
class random_type_2020(Structure):
    _fields_ = [
        ('addr', bth_address),
        ('by_uuid_flag', uint8_t),
        ('uuid', bth_gatt_uuid),
        ('flag', uint16_t),
    ]
class random_type_2021(Structure):
    _fields_ = [
        ('addr', bth_address),
        ('flag', uint16_t),
        ('service_info', bth_gatt_service),
    ]
class random_type_2022(Structure):
    _fields_ = [
        ('addr', bth_address),
        ('service_handle', uint16_t),
        ('by_uuid_flag', uint8_t),
        ('character_uuid', bth_gatt_uuid),
        ('flag', uint16_t),
    ]
class random_type_2023(Structure):
    _fields_ = [
        ('addr', bth_address),
        ('start_handle', uint16_t),
        ('end_handle', uint16_t),
        ('uuid', bth_gatt_uuid),
        ('flags', uint16_t),
    ]
class random_type_2024(Structure):
    _fields_ = [
        ('addr', bth_address),
        ('flag', uint16_t),
        ('characteristic_info', bth_gatt_characteristic),
    ]
class random_type_2025(Structure):
    _fields_ = [
        ('addr', bth_address),
        ('start_handle', uint16_t),
        ('end_handle', uint16_t),
        ('flags', uint16_t),
    ]
class random_type_2026(Structure):
    _fields_ = [
        ('addr', bth_address),
        ('characteristic', bth_gatt_characteristic),
        ('flag', uint16_t),
        ('offset', uint16_t),
        ('len', uint16_t),
        ('data', (uint8_t*0)),
    ]
class random_type_2027(Structure):
    _fields_ = [
        ('addr', bth_address),
        ('mtu', uint16_t),
        ('flag', uint32_t),
    ]
class random_type_2029(Structure):
    _fields_ = [
        ('service_handle', uint16_t),
        ('characteristic_handle', uint16_t),
        ('descriptor_type', bth_gatt_descriptor_type),
        ('descriptor_uuid', bth_gatt_uuid),
        ('attribute_handle', uint16_t),
    ]
bth_gatt_descriptor=random_type_2029
class random_type_2028(Structure):
    _fields_ = [
        ('addr', bth_address),
        ('flag', uint16_t),
        ('descriptor', bth_gatt_descriptor),
        ('descriptor_value', bth_gatt_descriptor_value),
    ]
class random_type_2030(Structure):
    _fields_ = [
        ('name_len', uint16_t),
        ('name', (uint8_t*248)),
        ('appearance', uint16_t),
    ]
bth_gatt_add_server=random_type_1996
bth_gatt_add_include=random_type_1999
bth_gatt_add_descriptor=random_type_2003
bth_gatt_update_descriptor=random_type_2016
bth_gatt_read_multiple_character_request=random_type_2018
bth_gatt_multiple_value_nfy_request=random_type_2019
bth_gatt_find_include_service=random_type_2021
bth_gatt_discovery_descriptor_by_handle=random_type_2025
bth_gatt_write_descriptor=random_type_2028
bth_gatt_set_gap_ui=random_type_2030
class random_type_2031(Structure):
    _fields_ = [
        ('le_att_item_auth_check', uint8_t),
        ('le_att_item_pend_flag', uint8_t),
    ]
bth_le_att_config_param=random_type_2031
class sdk_gatt_discovery(Structure):
    _fields_ = [
        ('addr', bth_address),
        ('service', list_stru),
    ]
class random_type_2032(Structure):
    _fields_ = [
        ('bd', (uint8_t*6)),
        ('atype', uint8_t),
        ('cap', uint8_t),
    ]
class sdk_gatt_service(Structure):
    _fields_ = [
        ('start_hdl', uint16_t),
        ('end_hdl', uint16_t),
        ('uuid', bth_gatt_uuid),
        ('include', list_stru),
        ('character', list_stru),
        ('secondary', uint8_t),
    ]
class random_type_2033(Structure):
    _fields_ = [
        ('is_short_uuid', uint32_t),
        ('short_uuid', uint16_t),
        ('reserve', uint16_t),
        ('long_uuid', bth_att_uuid),
    ]
class random_type_2034(Structure):
    _fields_ = [
        ('u', (uint8_t*16)),
    ]
class sdk_gatt_characteristic(Structure):
    _fields_ = [
        ('hdl', uint16_t),
        ('value_hdl', uint16_t),
        ('val', bth_gatt_characteristic_value),
        ('descriptor', list_stru),
        ('uuid', bth_gatt_uuid),
        ('properties', uint16_t),
    ]
class random_type_2035(Structure):
    _fields_ = [
        ('len', uint32_t),
        ('data', (uint8_t*0)),
    ]
class sdk_hid_value_report_map(Structure):
    _fields_ = [
        ('handle', uint16_t),
        ('report_id', uint8_t),
    ]
class sdk_hid_device_report_info(Structure):
    _fields_ = [
        ('addr', bth_address),
        ('report_count', uint8_t),
        ('report_map', (sdk_hid_value_report_map*5)),
    ]
class random_type_2036(Structure):
    _fields_ = [
        ('addr', bth_address),
        ('by_uuid_flag', uint8_t),
        ('uuid', bth_gatt_uuid),
        ('flag', uint16_t),
    ]
class random_type_2037(Structure):
    _fields_ = [
        ('addr', bth_address),
        ('att_handle', uint16_t),
        ('offset', uint16_t),
        ('security_mode', uint8_t),
        ('timeout', uint32_t),
    ]
class random_type_2038(Structure):
    _fields_ = [
        ('addr', bth_address),
        ('characteristic', bth_gatt_characteristic),
        ('flag', uint16_t),
        ('offset', uint16_t),
        ('len', uint16_t),
        ('data', (uint8_t*0)),
    ]
class random_type_2039(Structure):
    _fields_ = [
        ('service_handle', uint16_t),
        ('characteristic_uuid', bth_gatt_uuid),
        ('declare_handle', uint16_t),
        ('value_handle', uint16_t),
        ('broadcastable_flag', uint32_t),
        ('readable_flag', uint32_t),
        ('writable_flag', uint32_t),
        ('writable_without_response_flag', uint32_t),
        ('signed_writable_flag', uint32_t),
        ('notifiable_flag', uint32_t),
        ('indicatable_flag', uint32_t),
        ('authentication_flag', uint32_t),
        ('authorization_flag', uint32_t),
        ('mitm_flag', uint32_t),
        ('has_extended_properties', uint32_t),
    ]
class random_type_2040(Structure):
    _fields_ = [
        ('addr', bth_address),
        ('service_handle', uint16_t),
        ('by_uuid_flag', uint8_t),
        ('character_uuid', bth_gatt_uuid),
        ('flag', uint16_t),
    ]
class random_type_2041(Structure):
    _fields_ = [
        ('addr', bth_address),
        ('start_handle', uint16_t),
        ('end_handle', uint16_t),
        ('flags', uint16_t),
    ]
class random_type_2042(Structure):
    _fields_ = [
        ('addr', bth_address),
        ('flag', uint16_t),
        ('characteristic_info', bth_gatt_characteristic),
    ]
class random_type_2043(Structure):
    _fields_ = [
        ('addr', bth_address),
        ('start_hdl', uint16_t),
        ('end_hdl', uint16_t),
        ('uuid', bth_gatt_uuid),
    ]
class random_type_2044(Structure):
    _fields_ = [
        ('addr', bth_address),
        ('start_hdl', uint16_t),
        ('end_hdl', uint16_t),
        ('include_hdl', uint16_t),
        ('uuid', bth_att_uuid),
    ]
bth_gatt_find_include_service_response=random_type_2044
class sdk_gatt_include(Structure):
    _fields_ = [
        ('hdl', uint16_t),
        ('include_hdl', uint16_t),
        ('endhdl', uint16_t),
        ('uuid', bth_gatt_uuid),
    ]
class random_type_2045(Structure):
    _fields_ = [
        ('addr', bth_address),
        ('characteristic', bth_gatt_characteristic),
    ]
class random_type_2046(Structure):
    _fields_ = [
        ('addr', bth_address),
        ('character_hdl', uint16_t),
        ('descriptor_hdl', uint16_t),
        ('properties', uint16_t),
        ('descriptor_type', bth_gatt_descriptor_type),
        ('uuid', bth_gatt_uuid),
    ]
bth_gatt_discovery_descriptor_response=random_type_2046
class sdk_gatt_descriptor(Structure):
    _fields_ = [
        ('hdl', uint16_t),
        ('properties', uint16_t),
        ('uuid', bth_gatt_uuid),
        ('val', td_u32),
    ]
class random_type_2048(Structure):
    _fields_ = [
        ('is_reliable_write', uint32_t),
        ('is_auxiliaries_writable', uint32_t),
    ]
class random_type_2049(Structure):
    _fields_ = [
        ('is_subscribeto_notification', uint32_t),
        ('is_subscribeto_indication', uint32_t),
    ]
class random_type_2050(Structure):
    _fields_ = [
        ('is_broadcast', uint32_t),
    ]
class random_type_2051(Structure):
    _fields_ = [
        ('format', uint8_t),
        ('exponent', uint8_t),
        ('unit', bth_gatt_uuid),
        ('name_space', uint8_t),
        ('description', bth_gatt_uuid),
    ]
class random_type_2052(Union):
    _fields_ = [
        ('bth_characteristic_extended_properties', random_type_2048),
        ('client_characteristic_configuration', random_type_2049),
        ('server_characteristic_configuration', random_type_2050),
        ('characteristic_format', random_type_2051),
    ]
class random_type_2053(Structure):
    _fields_ = [
        ('descriptor_type', bth_gatt_descriptor_type),
        ('descriptor_uuid', bth_gatt_uuid),
        ('random_var_2047', random_type_2052),
        ('data_size', uint32_t),
        ('data', (uint8_t*0)),
    ]
class random_type_2054(Structure):
    _fields_ = [
        ('addr', bth_address),
        ('error_code', uint8_t),
        ('handle', uint16_t),
        ('offset', uint16_t),
        ('val_len', uint16_t),
        ('val', (uint8_t*0)),
    ]
class random_type_2055(Structure):
    _fields_ = [
        ('mutex', td_u32),
    ]
class handle_struct(Structure):
    _fields_ = [
        ('sync_flag', uint8_t),
        ('opcode', uint16_t),
        ('field_len', uint8_t),
        ('field_id', td_u32),
        ('sem', osal_semaphore),
        ('out_len', uint16_t),
        ('out_data', td_u32),
        ('cfm_cbk', td_u32),
    ]
class random_type_2056(Structure):
    _fields_ = [
        ('sem', td_u32),
    ]
class handle_param(Structure):
    _fields_ = [
        ('opcode', uint16_t),
        ('field_len', uint8_t),
        ('field_id', (uint8_t*0)),
    ]
class random_type_2057(Structure):
    _fields_ = [
        ('op', uint8_t),
        ('func', td_u32),
    ]
class random_type_2058(Structure):
    _fields_ = [
        ('op', uint8_t),
        ('func', mpc_function),
    ]
class random_type_2059(Structure):
    _fields_ = [
        ('header', mpc_header),
        ('payload_length', uint16_t),
        ('src_transid', uint8_t),
        ('resv', uint8_t),
        ('payload', (uint8_t*0)),
    ]
class random_type_2060(Structure):
    _fields_ = [
        ('trans_id', uint8_t),
        ('sof', uint8_t),
        ('service_id', uint8_t),
        ('command_id', uint8_t),
    ]
class gap_create_connection_cfm_stru(Structure):
    _fields_ = [
        ('addr', hci_address_stru),
        ('acl_hdl', uint16_t),
        ('link_type', uint8_t),
    ]
class authorization_req_ind_struct(Structure):
    _fields_ = [
        ('svc_hdl', uint32_t),
        ('dev_hdl', uint32_t),
    ]
class random_type_2061(Structure):
    _fields_ = [
        ('dev_hdl', uint32_t),
        ('dev_class', uint32_t),
        ('link_type', uint8_t),
        ('bd_addr', bth_addr_t),
    ]
class random_type_2062(Structure):
    _fields_ = [
        ('addr', (td_uchar*6)),
    ]
class connection_complete_ind_struct(Structure):
    _fields_ = [
        ('dev_hdl', uint32_t),
        ('conn_dev_info', bt_sdk_conn_dev_info_t),
    ]
class connect_event_ind_str(Structure):
    _fields_ = [
        ('conn_hdl', uint32_t),
        ('event', uint16_t),
        ('conn', bt_sdk_connection_property_stru),
    ]
class usr_cfm_req_ind_struct(Structure):
    _fields_ = [
        ('dev_hdl', uint32_t),
        ('num_value', uint32_t),
    ]
class passkey_notif_ind_struct(Structure):
    _fields_ = [
        ('dev_hdl', uint32_t),
        ('num_value', uint32_t),
    ]
class disconnection_complete_ind_struct(Structure):
    _fields_ = [
        ('dev_hdl', uint32_t),
        ('reason', uint32_t),
    ]
bt_sdk_dev_found_data_type=td_u8
class random_type_2063(Structure):
    _fields_ = [
        ('s_dev_name', (td_char*128)),
        ('class_of_device', uint32_t),
        ('appearance', uint16_t),
        ('bd_addr', (uint8_t*6)),
        ('a_type', uint8_t),
        ('u_cap', uint8_t),
        ('rssi', int8_t),
        ('data_type', bt_sdk_dev_found_data_type),
        ('eir_size', uint16_t),
        ('eir_data', td_u32),
    ]
bt_sdk_dev_found_info=random_type_2063
class remot_device_found_struct(Structure):
    _fields_ = [
        ('dev_hdl', uint32_t),
        ('info', bt_sdk_dev_found_info),
    ]
class link_key_req_struct(Structure):
    _fields_ = [
        ('addr', (uint8_t*6)),
        ('link_key', br_security_link_key_stru),
    ]
class random_type_2064(Structure):
    _fields_ = [
        ('link_key', (uint8_t*16)),
        ('key_type', uint8_t),
        ('accept', uint8_t),
    ]
class simple_pair_complete_struct(Structure):
    _fields_ = [
        ('dev_hdl', uint32_t),
        ('result', uint8_t),
    ]
class random_type_2065(Structure):
    _fields_ = [
        ('dev_class', uint32_t),
        ('addr', (uint8_t*6)),
        ('link_key', (uint8_t*16)),
    ]
remote_device_t=random_type_2065
class random_type_2066(Structure):
    _fields_ = [
        ('dev_class', uint32_t),
        ('max_num', uint8_t),
        ('max_seconds', uint8_t),
    ]
device_discovery_para_t=random_type_2066
class create_acl_para_str(Structure):
    _fields_ = [
        ('dev_hdl', uintptr_t),
        ('link_type', uint32_t),
    ]
class random_type_2067(Structure):
    _fields_ = [
        ('dev_hdl', uint32_t),
        ('pwr_level', uint8_t),
    ]
bt_set_tx_pwr_params_t=random_type_2067
class sdk_connect_ex_para(Structure):
    _fields_ = [
        ('dev_hdl', uintptr_t),
        ('service_class', uint32_t),
    ]
class set_voice_shmid_str(Structure):
    _fields_ = [
        ('sync_hdl', uint16_t),
        ('stream_in_mem', uint32_t),
        ('stream_out_mem', uint32_t),
        ('enable', uint16_t),
    ]
class random_type_2068(Structure):
    _fields_ = [
        ('scene', uint8_t),
        ('bit_rate', uint8_t),
        ('is_low_delay', uint8_t),
        ('addr', (uint8_t*6)),
    ]
btsdk_gap_active_scene_t=random_type_2068
class random_type_2069(Structure):
    _fields_ = [
        ('interval', uint16_t),
        ('window', uint16_t),
    ]
btsdk_gap_inquiry_scan_param_t=random_type_2069
class random_type_2070(Structure):
    _fields_ = [
        ('rmt_sep_cap_cbk', btsdk_a2dp_rmt_sep_cap_cbk),
        ('stream_state_chg_cbk', btsdk_a2dp_stream_state_chg_cbk),
        ('stream_cfg_chg_cbk', btsdk_a2dp_stream_cfg_chg_cbk),
        ('stream_reject_cbk', btsdk_a2dp_stream_reject_cbk),
    ]
class random_type_2071(Structure):
    _fields_ = [
        ('rmt_hdl', td_u32),
        ('seid', uint8_t),
        ('sc', btsdk_a2dp_service_cap_stru),
    ]
class random_type_2072(Union):
    _fields_ = [
        ('sbc', sdk_sbc_cfg_stru),
        ('aac', sdk_aac_cfg_stru),
    ]
class random_type_2073(Structure):
    _fields_ = [
        ('payload', uint16_t),
        ('code_c', uint8_t),
        ('info', random_type_2072),
    ]
class random_type_2074(Structure):
    _fields_ = [
        ('sample_f', uint32_t),
        ('chan_mode', uint8_t),
        ('block_len', uint8_t),
        ('sub_band', uint8_t),
        ('alloc_method', uint8_t),
        ('min_bit_pool', uint8_t),
        ('max_bit_pool', uint8_t),
    ]
class random_type_2075(Structure):
    _fields_ = [
        ('sample_rate', uint32_t),
        ('bit_rate', uint32_t),
        ('channel', uint8_t),
        ('object_type', uint8_t),
        ('rfa', uint8_t),
        ('vbr', uint8_t),
    ]
class random_type_2076(Structure):
    _fields_ = [
        ('stream_handle', td_u32),
        ('role', uint8_t),
        ('event', uint8_t),
        ('err', uint8_t),
    ]
class random_type_2077(Structure):
    _fields_ = [
        ('op', uint8_t),
        ('func', mpc_function),
    ]
class random_type_2078(Structure):
    _fields_ = [
        ('header', mpc_header),
        ('payload_length', uint16_t),
        ('src_transid', uint8_t),
        ('resv', uint8_t),
        ('payload', (uint8_t*0)),
    ]
class random_type_2079(Structure):
    _fields_ = [
        ('trans_id', uint8_t),
        ('sof', uint8_t),
        ('service_id', uint8_t),
        ('command_id', uint8_t),
    ]
class random_type_2080(Structure):
    _fields_ = [
        ('reconfig', td_bool),
        ('stream_hdl', uint32_t),
        ('cfg', (uint8_t*1)),
    ]
a2dp_src_stream_codec_params=random_type_2080
class random_type_2081(Structure):
    _fields_ = [
        ('audio_card', (uint8_t*128)),
        ('len', uint16_t),
    ]
a2dp_snk_audio_card_params=random_type_2081
class random_type_2082(Structure):
    _fields_ = [
        ('stream_hdl', uint32_t),
        ('state', uint8_t),
        ('src', uint8_t),
        ('len', uint16_t),
        ('param', (uint8_t*0)),
    ]
class random_type_2083(Structure):
    _fields_ = [
        ('stream_handle', td_u32),
        ('src', uint8_t),
        ('res', (uint8_t*3)),
        ('stream_cfg', sdka2dp_stream_cfg_stru),
    ]
a2dp_stream_info_s=random_type_2083
class random_type_2084(Structure):
    _fields_ = [
        ('stream_hdl', uint32_t),
        ('enable', td_bool),
        ('share_mem_id', uint32_t),
    ]
bt_audio_shmid_params=random_type_2084
class random_type_2085(Structure):
    _fields_ = [
        ('op', uint8_t),
        ('func', mpc_function),
    ]
class random_type_2086(Structure):
    _fields_ = [
        ('header', mpc_header),
        ('payload_length', uint16_t),
        ('src_transid', uint8_t),
        ('resv', uint8_t),
        ('payload', (uint8_t*0)),
    ]
class random_type_2087(Structure):
    _fields_ = [
        ('trans_id', uint8_t),
        ('sof', uint8_t),
        ('service_id', uint8_t),
        ('command_id', uint8_t),
    ]
class random_type_2088(Structure):
    _fields_ = [
        ('ct_rsp_cbk', td_u32),
        ('tg_cmd_cbk', td_u32),
    ]
sdk_avrcp_cbk_stru=random_type_2088
class random_type_2089(Structure):
    _fields_ = [
        ('dev_hdl', uint32_t),
        ('size', uint32_t),
        ('identifier', (uint8_t*8)),
        ('num', uint8_t),
        ('reserved', (uint8_t*3)),
        ('attr_id', (uint32_t*1)),
    ]
avrcp_ct_get_element_attr_params=random_type_2089
class random_type_2090(Structure):
    _fields_ = [
        ('dev_hdl', uint32_t),
        ('tl', uint8_t),
        ('reserved', (uint8_t*3)),
        ('size', uint32_t),
        ('capability_id', uint8_t),
        ('count', uint8_t),
        ('capability', (uint8_t*1)),
    ]
avrcp_ct_get_capabilities_rsp_param=random_type_2090
class random_type_2091(Structure):
    _fields_ = [
        ('dev_hdl', uint32_t),
        ('rsp_type', uint16_t),
        ('len', uint16_t),
        ('param', (uint8_t*1)),
    ]
avrcp_ct_event_rsp_param=random_type_2091
class random_type_2092(Structure):
    _fields_ = [
        ('dev_hdl', uint32_t),
        ('tl', uint8_t),
        ('reserved', uint8_t),
        ('cmd_type', uint16_t),
        ('param', (uint8_t*1)),
    ]
avrcp_tg_cmd_param=random_type_2092
class random_type_2093(Structure):
    _fields_ = [
        ('dev_hdl', uint32_t),
        ('event_id', uint8_t),
        ('reserved', (uint8_t*3)),
        ('playback_interval', uint32_t),
    ]
avrcp_ct_reg_notify_params=random_type_2093
class random_type_2094(Structure):
    _fields_ = [
        ('dev_hdl', uint32_t),
        ('id', uint8_t),
        ('val', uint8_t),
    ]
avrcp_ct_bt_sdk_id_params=random_type_2094
class random_type_2095(Structure):
    _fields_ = [
        ('dev_hdl', uint32_t),
        ('state_flag', uint8_t),
        ('op_id', uint8_t),
        ('vendor_unique_id', uint8_t),
    ]
avrcp_ct_pass_through_params=random_type_2095
class random_type_2096(Structure):
    _fields_ = [
        ('size', uint32_t),
        ('rsp_code', uint8_t),
        ('id', uint8_t),
    ]
avrcp_play_status_changed_stru=random_type_2096
class random_type_2097(Structure):
    _fields_ = [
        ('size', uint32_t),
        ('rsp_code', uint8_t),
        ('identifier', (uint32_t*8)),
    ]
avrcp_ct_bt_sdk_identifier_params=random_type_2097
class random_type_2098(Structure):
    _fields_ = [
        ('size', uint32_t),
        ('rsp_code', uint8_t),
        ('pos', uint32_t),
    ]
avrcp_ct_bt_sdk_pos_params=random_type_2098
class random_type_2099(Structure):
    _fields_ = [
        ('size', uint32_t),
        ('rsp_code', uint8_t),
        ('player_id', uint16_t),
        ('uid_counter', uint16_t),
    ]
avrcp_ct_addr_player_params=random_type_2099
class random_type_2100(Structure):
    _fields_ = [
        ('size', uint32_t),
        ('rsp_code', uint8_t),
        ('uid_counter', uint16_t),
    ]
avrcp_ct_uids_params=random_type_2100
avrcp_vol_changed_stru=random_type_2096
class random_type_2101(Structure):
    _fields_ = [
        ('on_off', uint8_t),
    ]
class random_type_2102(Structure):
    _fields_ = [
        ('length', uint8_t),
        ('subcode', uint8_t),
        ('freq', uint8_t),
        ('mode', uint8_t),
        ('switch_trx', uint8_t),
    ]
class random_type_2103(Structure):
    _fields_ = [
        ('ocf', uint16_t),
        ('param_len', uint8_t),
        ('param', (uint8_t*0)),
    ]
class random_type_2104(Structure):
    _fields_ = [
        ('on_off', uint8_t),
        ('tx_power', uint8_t),
        ('tx_channel', uint8_t),
        ('pkt_type', uint8_t),
        ('pkt_len', (uint8_t*2)),
        ('br_edr', uint8_t),
        ('payload', uint8_t),
    ]
class random_type_2105(Structure):
    _fields_ = [
        ('on_off', uint8_t),
        ('rx_channel', uint8_t),
        ('pkt_type', uint8_t),
        ('pkt_len', (uint8_t*2)),
        ('br_edr', uint8_t),
        ('payload', uint8_t),
    ]
class random_type_2106(Structure):
    _fields_ = [
        ('on_off', uint8_t),
        ('tx_channel', uint8_t),
        ('pkt_type', uint8_t),
        ('pkt_len', uint8_t),
        ('tx_phy', uint8_t),
    ]
class random_type_2107(Structure):
    _fields_ = [
        ('on_off', uint8_t),
        ('rx_channel', uint8_t),
        ('rx_phy', uint8_t),
    ]
class random_type_2108(Structure):
    _fields_ = [
        ('op', uint8_t),
        ('func', mpc_function),
    ]
class random_type_2109(Structure):
    _fields_ = [
        ('header', mpc_header),
        ('payload_length', uint16_t),
        ('src_transid', uint8_t),
        ('resv', uint8_t),
        ('payload', (uint8_t*0)),
    ]
class random_type_2110(Structure):
    _fields_ = [
        ('trans_id', uint8_t),
        ('sof', uint8_t),
        ('service_id', uint8_t),
        ('command_id', uint8_t),
    ]
class random_type_2111(Structure):
    _fields_ = [
        ('type', uint8_t),
        ('service', uint8_t),
        ('num_len', uint8_t),
        ('number', (int8_t*32)),
        ('name_len', uint8_t),
        ('alpha_str', (int8_t*1)),
    ]
class random_type_2112(Structure):
    _fields_ = [
        ('hfp_hdl', td_u32),
        ('event', td_u16),
        ('param_len', td_u16),
        ('param', (td_uchar*0)),
    ]
bt_hfp_ipc_msg_param_stru=random_type_2112
class random_type_2113(Structure):
    _fields_ = [
        ('stream_hdl', uint32_t),
        ('len', uint8_t),
        ('number', (uint8_t*1)),
    ]
bt_hfp_dail_params=random_type_2113
class random_type_2114(Structure):
    _fields_ = [
        ('hdl', uint32_t),
    ]
bt_hfp_general_params=random_type_2114
class random_type_2115(Structure):
    _fields_ = [
        ('hdl', uint32_t),
        ('op_code', uint16_t),
        ('idx', uint8_t),
    ]
bt_hfp_3way_params=random_type_2115
class random_type_2116(Structure):
    _fields_ = [
        ('hdl', uint32_t),
        ('volume', uint8_t),
    ]
bt_hfp_vol_params=random_type_2116
class random_type_2117(Structure):
    _fields_ = [
        ('hdl', uint32_t),
        ('code', uint8_t),
    ]
bt_hfp_dtmf_params=random_type_2117
class random_type_2118(Structure):
    _fields_ = [
        ('hdl', uint32_t),
        ('is_accept', uint8_t),
    ]
bt_hfp_sco_params=random_type_2118
class random_type_2119(Structure):
    _fields_ = [
        ('hdl', uint32_t),
        ('mode', uint8_t),
        ('type', uint8_t),
    ]
bt_hfp_ag_params=random_type_2119
class random_type_2120(Structure):
    _fields_ = [
        ('hdl', uint32_t),
        ('event', uint8_t),
        ('param', uint8_t),
    ]
bt_hfp_ag_network_event=random_type_2120
class random_type_2121(Structure):
    _fields_ = [
        ('hdl', uint32_t),
        ('type', uint8_t),
        ('service', uint8_t),
        ('num_len', uint8_t),
        ('number', (int8_t*32)),
        ('name_len', uint8_t),
        ('alpha_str', (int8_t*1)),
    ]
bt_hfp_ag_incoming_event=random_type_2121
class random_type_2122(Structure):
    _fields_ = [
        ('idx', uint8_t),
        ('dir', uint8_t),
        ('status', uint8_t),
        ('mode', uint8_t),
        ('mpty', uint8_t),
        ('type', uint8_t),
        ('num_len', uint8_t),
        ('number', (int8_t*1)),
    ]
class random_type_2123(Structure):
    _fields_ = [
        ('hdl', uint32_t),
        ('complete', uint8_t),
        ('idx', uint8_t),
        ('dir', uint8_t),
        ('status', uint8_t),
        ('mode', uint8_t),
        ('mpty', uint8_t),
        ('type', uint8_t),
        ('num_len', uint8_t),
        ('number', (int8_t*1)),
    ]
bt_hfp_ag_current_call_t=random_type_2123
class random_type_2124(Structure):
    _fields_ = [
        ('hdl', uint32_t),
        ('complete', uint8_t),
        ('type', uint8_t),
        ('service', uint8_t),
        ('num_len', uint8_t),
        ('number', (int8_t*32)),
        ('name_len', uint8_t),
        ('alpha_str', (int8_t*1)),
    ]
bt_hfp_ag_phone_info_t=random_type_2124
class random_type_2125(Structure):
    _fields_ = [
        ('hdl', uint32_t),
        ('value', uint8_t),
    ]
bt_hfp_ag_battery_charge_t=random_type_2125
class random_type_2126(Structure):
    _fields_ = [
        ('hdl', uint32_t),
        ('error_code', uint8_t),
    ]
bt_hfp_ag_cmee_error_t=random_type_2126
class random_type_2127(Structure):
    _fields_ = [
        ('mode', uint8_t),
        ('format', uint8_t),
        ('operator_len', uint8_t),
        ('operator_name', (int8_t*1)),
    ]
class random_type_2128(Structure):
    _fields_ = [
        ('hdl', uint32_t),
        ('mode', uint8_t),
        ('format', uint8_t),
        ('operator_len', uint8_t),
        ('operator_name', (int8_t*1)),
    ]
bt_hfp_ag_cops_info_t=random_type_2128
class random_type_2129(Structure):
    _fields_ = [
        ('hdl', uint32_t),
        ('service', uint8_t),
        ('call', uint8_t),
        ('callsetup', uint8_t),
        ('callheld', uint8_t),
        ('signal', uint8_t),
        ('roam', uint8_t),
        ('battchg', uint8_t),
    ]
bt_hfp_ag_cind_info_t=random_type_2129
class random_type_2130(Structure):
    _fields_ = [
        ('service', uint8_t),
        ('call', uint8_t),
        ('callsetup', uint8_t),
        ('callheld', uint8_t),
        ('signal', uint8_t),
        ('roam', uint8_t),
        ('battchg', uint8_t),
    ]
class random_type_2131(Structure):
    _fields_ = [
        ('event_cbk', td_u32),
        ('write_func', td_u32),
    ]
class random_type_2132(Structure):
    _fields_ = [
        ('op', uint8_t),
        ('func', mpc_function),
    ]
class random_type_2133(Structure):
    _fields_ = [
        ('header', mpc_header),
        ('payload_length', uint16_t),
        ('src_transid', uint8_t),
        ('resv', uint8_t),
        ('payload', (uint8_t*0)),
    ]
class random_type_2134(Structure):
    _fields_ = [
        ('trans_id', uint8_t),
        ('sof', uint8_t),
        ('service_id', uint8_t),
        ('command_id', uint8_t),
    ]
class random_type_2135(Structure):
    _fields_ = [
        ('event', uint32_t),
        ('result', uint32_t),
        ('arg', (uint8_t*0)),
    ]
bt_pbap_ipc_app_param_stru=random_type_2135
class random_type_2136(Structure):
    _fields_ = [
        ('hdl', uintptr_t),
        ('len', uint32_t),
        ('buffer', (uint8_t*0)),
    ]
bt_pbap_ipc_msg_param_stru=random_type_2136
class random_type_2137(Structure):
    _fields_ = [
        ('mask', uint16_t),
        ('filter', (uint8_t*8)),
        ('max_count', uint16_t),
        ('list_offset', uint16_t),
        ('order', uint8_t),
        ('format', uint8_t),
        ('search_val', td_u32),
        ('search_attrib', uint8_t),
        ('missed_calls', uint8_t),
        ('pb_size', uint16_t),
    ]
class random_type_2138(Structure):
    _fields_ = [
        ('conn_hdl', uint32_t),
        ('path_len', uint32_t),
        ('path', (td_char*20)),
        ('file_hdl', uintptr_t),
        ('pbap_param', bt_sdk_pbap_param_stru),
        ('search_val', (uint8_t*0)),
    ]
bt_pbap_phonebook_params=random_type_2138
class random_type_2139(Structure):
    _fields_ = [
        ('conn_hdl', uint32_t),
        ('path_len', uint32_t),
        ('path', (td_char*0)),
    ]
bt_pbap_phonebook_path_params=random_type_2139
class random_type_2140(Structure):
    _fields_ = [
        ('conn_hdl', uint32_t),
    ]
bt_pbap_transfer_params=random_type_2140
class random_type_2141(Structure):
    _fields_ = [
        ('op', uint8_t),
        ('func', mpc_function),
    ]
class random_type_2142(Structure):
    _fields_ = [
        ('header', mpc_header),
        ('payload_length', uint16_t),
        ('src_transid', uint8_t),
        ('resv', uint8_t),
        ('payload', (uint8_t*0)),
    ]
class random_type_2143(Structure):
    _fields_ = [
        ('trans_id', uint8_t),
        ('sof', uint8_t),
        ('service_id', uint8_t),
        ('command_id', uint8_t),
    ]
class random_type_2144(Structure):
    _fields_ = [
        ('real_len', uint32_t),
        ('buf', (uint8_t*0)),
    ]
bt_spp_read_ret_params=random_type_2144
class random_type_2145(Structure):
    _fields_ = [
        ('conn_hdl', uint32_t),
        ('event_type', uint32_t),
        ('data_len', uint16_t),
        ('data', (uint8_t*0)),
    ]
bt_spp_ipc_app_param_stru=random_type_2145
class random_type_2146(Structure):
    _fields_ = [
        ('conn_hdl', uint32_t),
        ('data_size', uint32_t),
        ('field_key', uint32_t),
    ]
bt_spp_read_params=random_type_2146
class random_type_2147(Structure):
    _fields_ = [
        ('conn_hdl', uint32_t),
        ('data_size', uint32_t),
        ('data', (uint8_t*0)),
    ]
bt_spp_write_params=random_type_2147
class random_type_2148(Structure):
    _fields_ = [
        ('svr_chnl', uint16_t),
        ('field_len', uint16_t),
        ('svc_name', (td_char*0)),
    ]
bt_spp_reg_params=random_type_2148
class random_type_2149(Structure):
    _fields_ = [
        ('net_state_cbk', btsdk_pan_rpt_net_state),
        ('filter_result_cbk', btsdk_pan_rpt_filter_result),
        ('ethernet_cbk', btsdk_pan_rpt_ethernet_packet),
    ]
class random_type_2150(Structure):
    _fields_ = [
        ('op', uint8_t),
        ('func', mpc_function),
    ]
class random_type_2151(Structure):
    _fields_ = [
        ('header', mpc_header),
        ('payload_length', uint16_t),
        ('src_transid', uint8_t),
        ('resv', uint8_t),
        ('payload', (uint8_t*0)),
    ]
class random_type_2152(Structure):
    _fields_ = [
        ('trans_id', uint8_t),
        ('sof', uint8_t),
        ('service_id', uint8_t),
        ('command_id', uint8_t),
    ]
class random_type_2153(Structure):
    _fields_ = [
        ('addr', (uint8_t*6)),
        ('ip_packet', (uint8_t*0)),
    ]
pan_service_ethernet_req_t=random_type_2153
class random_type_2154(Structure):
    _fields_ = [
        ('op', uint8_t),
        ('func', mpc_function),
    ]
class random_type_2155(Structure):
    _fields_ = [
        ('header', mpc_header),
        ('payload_length', uint16_t),
        ('src_transid', uint8_t),
        ('resv', uint8_t),
        ('payload', (uint8_t*0)),
    ]
class random_type_2156(Structure):
    _fields_ = [
        ('trans_id', uint8_t),
        ('sof', uint8_t),
        ('service_id', uint8_t),
        ('command_id', uint8_t),
    ]
l2cap_le_cbk=td_u32
class random_type_2157(Structure):
    _fields_ = [
        ('addr', bth_address),
        ('cid', uint16_t),
        ('conn_handle', uint16_t),
        ('data', bth_data_t),
    ]
bth_l2cap_credit_conn_rx=random_type_2157
class random_type_2158(Structure):
    _fields_ = [
        ('bd', (uint8_t*6)),
        ('atype', uint8_t),
        ('cap', uint8_t),
    ]
class random_type_2159(Structure):
    _fields_ = [
        ('data_len', uint16_t),
        ('data', (uint8_t*0)),
    ]
class random_type_2160(Structure):
    _fields_ = [
        ('addr', bth_address),
        ('psm', uint16_t),
        ('cid', (uint16_t*2)),
        ('mps', (uint16_t*2)),
        ('mtu', (uint16_t*2)),
        ('init_credits', uint16_t),
        ('conn_hdl', uint16_t),
        ('channel_type', uint8_t),
        ('role', uint8_t),
        ('status', uint8_t),
    ]
class random_type_2161(Structure):
    _fields_ = [
        ('psm', uint16_t),
        ('cid', uint16_t),
        ('channel_type', uint8_t),
        ('mps', uint16_t),
        ('mtu', uint16_t),
        ('init_credits', uint16_t),
        ('cbk', td_u32),
    ]
bth_l2cap_psm_node=random_type_2161
bth_l2cap_credit_conn=random_type_2160
class random_type_2162(Structure):
    _fields_ = [
        ('conn', bth_l2cap_credit_conn),
        ('sub_ev', uint16_t),
        ('result', uint16_t),
    ]
bth_l2cap_credit_conn_ev=random_type_2162
class random_type_2163(Structure):
    _fields_ = [
        ('addr', bth_address),
        ('psm', uint16_t),
        ('cid', uint16_t),
        ('result', uint16_t),
    ]
bth_l2cap_disconnect=random_type_2163
class random_type_2164(Structure):
    _fields_ = [
        ('server_event_cbk_list', list_stru),
        ('client_event_cbk_list', list_stru),
        ('tl_list', list_stru),
    ]
ble_gatt_local_data=random_type_2164
class random_type_2165(Structure):
    _fields_ = [
        ('op', uint8_t),
        ('func', mpc_function),
    ]
class random_type_2166(Structure):
    _fields_ = [
        ('header', mpc_header),
        ('payload_length', uint16_t),
        ('src_transid', uint8_t),
        ('resv', uint8_t),
        ('payload', (uint8_t*0)),
    ]
class random_type_2167(Structure):
    _fields_ = [
        ('trans_id', uint8_t),
        ('sof', uint8_t),
        ('service_id', uint8_t),
        ('command_id', uint8_t),
    ]
class random_type_2168(Structure):
    _fields_ = [
        ('table_id', uint16_t),
        ('type', uint16_t),
        ('node_list', list_stru),
    ]
ble_general_cbk_table=random_type_2168
class random_type_2169(Structure):
    _fields_ = [
        ('event', uint16_t),
        ('cbk', sdk_ble_cbk),
    ]
ble_general_cbk_node=random_type_2169
class random_type_2170(Structure):
    _fields_ = [
        ('addr', bth_address),
        ('error_code', uint8_t),
        ('handle', uint16_t),
        ('offset', uint16_t),
        ('val_len', uint16_t),
        ('val', (uint8_t*0)),
    ]
class random_type_2171(Structure):
    _fields_ = [
        ('bd', (uint8_t*6)),
        ('atype', uint8_t),
        ('cap', uint8_t),
    ]
class random_type_2172(Structure):
    _fields_ = [
        ('addr', bth_address),
        ('error_code', uint32_t),
        ('handle', uint16_t),
        ('offset', uint16_t),
        ('val_len', uint16_t),
        ('val', (uint8_t*0)),
    ]
class random_type_2173(Structure):
    _fields_ = [
        ('addr', bth_address),
        ('characteristic_handle', uint16_t),
        ('value', bth_gatt_characteristic_value),
    ]
class random_type_2174(Structure):
    _fields_ = [
        ('len', uint32_t),
        ('data', (uint8_t*0)),
    ]
class random_type_2175(Structure):
    _fields_ = [
        ('opcode', uint16_t),
        ('addr', (uint8_t*6)),
        ('addr_type', uint8_t),
        ('request_id', uint16_t),
        ('att_hdl', uint16_t),
        ('svc_uuid', (uint8_t*16)),
        ('uuid', (uint8_t*16)),
        ('offset', uint16_t),
        ('len', uint16_t),
        ('need_authorize', uint8_t),
        ('need_rsp', uint8_t),
        ('data', (uint8_t*0)),
    ]
sdk_ble_gatt_role=td_u8
class random_type_2176(Structure):
    _fields_ = [
        ('bd_addr', bth_address),
        ('role', sdk_ble_gatt_role),
        ('tl_task', list_stru),
    ]
ble_gatt_tl_data=random_type_2176
sdk_ble_gatt_cbk_event=td_u8
class random_type_2177(Structure):
    _fields_ = [
        ('addr', bth_address),
        ('by_uuid_flag', uint8_t),
        ('uuid', bth_gatt_uuid),
        ('flag', uint16_t),
    ]
class random_type_2178(Structure):
    _fields_ = [
        ('is_short_uuid', uint32_t),
        ('short_uuid', uint16_t),
        ('reserve', uint16_t),
        ('long_uuid', bth_att_uuid),
    ]
class random_type_2179(Structure):
    _fields_ = [
        ('u', (uint8_t*16)),
    ]
class random_type_2180(Structure):
    _fields_ = [
        ('addr', bth_address),
        ('att_handle', uint16_t),
        ('offset', uint16_t),
        ('security_mode', uint8_t),
        ('timeout', uint32_t),
    ]
class random_type_2181(Structure):
    _fields_ = [
        ('addr', bth_address),
        ('flag', uint16_t),
        ('service_info', bth_gatt_service),
    ]
class random_type_2182(Structure):
    _fields_ = [
        ('service_uuid', bth_gatt_uuid),
        ('service_handle', uint16_t),
    ]
class random_type_2183(Structure):
    _fields_ = [
        ('addr', bth_address),
        ('service_handle', uint16_t),
        ('by_uuid_flag', uint8_t),
        ('character_uuid', bth_gatt_uuid),
        ('flag', uint16_t),
    ]
class random_type_2184(Structure):
    _fields_ = [
        ('addr', bth_address),
        ('start_handle', uint16_t),
        ('end_handle', uint16_t),
        ('uuid', bth_gatt_uuid),
        ('flags', uint16_t),
    ]
class random_type_2185(Structure):
    _fields_ = [
        ('addr', bth_address),
        ('start_handle', uint16_t),
        ('end_handle', uint16_t),
        ('flags', uint16_t),
    ]
class random_type_2186(Structure):
    _fields_ = [
        ('addr', bth_address),
        ('flag', uint16_t),
        ('characteristic_info', bth_gatt_characteristic),
    ]
class random_type_2187(Structure):
    _fields_ = [
        ('service_handle', uint16_t),
        ('characteristic_uuid', bth_gatt_uuid),
        ('declare_handle', uint16_t),
        ('value_handle', uint16_t),
        ('broadcastable_flag', uint32_t),
        ('readable_flag', uint32_t),
        ('writable_flag', uint32_t),
        ('writable_without_response_flag', uint32_t),
        ('signed_writable_flag', uint32_t),
        ('notifiable_flag', uint32_t),
        ('indicatable_flag', uint32_t),
        ('authentication_flag', uint32_t),
        ('authorization_flag', uint32_t),
        ('mitm_flag', uint32_t),
        ('has_extended_properties', uint32_t),
    ]
class random_type_2188(Structure):
    _fields_ = [
        ('addr', bth_address),
        ('characteristic', bth_gatt_characteristic),
        ('flag', uint16_t),
        ('offset', uint16_t),
        ('len', uint16_t),
        ('data', (uint8_t*0)),
    ]
class random_type_2189(Structure):
    _fields_ = [
        ('addr', bth_address),
        ('flag', uint16_t),
        ('descriptor', bth_gatt_descriptor),
        ('descriptor_value', bth_gatt_descriptor_value),
    ]
class random_type_2190(Structure):
    _fields_ = [
        ('service_handle', uint16_t),
        ('characteristic_handle', uint16_t),
        ('descriptor_type', bth_gatt_descriptor_type),
        ('descriptor_uuid', bth_gatt_uuid),
        ('attribute_handle', uint16_t),
    ]
class random_type_2192(Structure):
    _fields_ = [
        ('is_reliable_write', uint32_t),
        ('is_auxiliaries_writable', uint32_t),
    ]
class random_type_2193(Structure):
    _fields_ = [
        ('is_subscribeto_notification', uint32_t),
        ('is_subscribeto_indication', uint32_t),
    ]
class random_type_2194(Structure):
    _fields_ = [
        ('is_broadcast', uint32_t),
    ]
class random_type_2195(Structure):
    _fields_ = [
        ('format', uint8_t),
        ('exponent', uint8_t),
        ('unit', bth_gatt_uuid),
        ('name_space', uint8_t),
        ('description', bth_gatt_uuid),
    ]
class random_type_2196(Union):
    _fields_ = [
        ('bth_characteristic_extended_properties', random_type_2192),
        ('client_characteristic_configuration', random_type_2193),
        ('server_characteristic_configuration', random_type_2194),
        ('characteristic_format', random_type_2195),
    ]
class random_type_2197(Structure):
    _fields_ = [
        ('descriptor_type', bth_gatt_descriptor_type),
        ('descriptor_uuid', bth_gatt_uuid),
        ('random_var_2191', random_type_2196),
        ('data_size', uint32_t),
        ('data', (uint8_t*0)),
    ]
class random_type_2198(Structure):
    _fields_ = [
        ('name_len', uint16_t),
        ('name', (uint8_t*248)),
        ('appearance', uint16_t),
    ]
class random_type_2199(Structure):
    _fields_ = [
        ('addr', bth_address),
        ('mtu', uint16_t),
        ('flag', uint32_t),
    ]
class random_type_2200(Structure):
    _fields_ = [
        ('server_handle', uint32_t),
        ('service_uuid', bth_gatt_uuid),
        ('service_type', bth_gatt_service_type),
    ]
class random_type_2201(Structure):
    _fields_ = [
        ('service_handle', uint32_t),
        ('service', bth_gatt_start_service),
    ]
class random_type_2202(Structure):
    _fields_ = [
        ('service_uuid', bth_gatt_uuid),
        ('service_handle', uint32_t),
    ]
class random_type_2203(Structure):
    _fields_ = [
        ('le_att_mtu_default', uint16_t),
        ('le_att_mtu_le_max', uint16_t),
    ]
class random_type_2204(Structure):
    _fields_ = [
        ('service_handle', uint32_t),
        ('characteristic', bth_gatt_characteristic),
        ('data_size', uint32_t),
        ('data', (uint8_t*0)),
    ]
class random_type_2205(Structure):
    _fields_ = [
        ('characteristic_handle', uint32_t),
        ('permission', uint8_t),
        ('descriptor_value', bth_gatt_descriptor_value),
    ]
class random_type_2207(Structure):
    _fields_ = [
        ('characteristic_handle', uint16_t),
        ('data_size', uint16_t),
        ('data', (uint8_t*0)),
    ]
class random_type_2208(Structure):
    _fields_ = [
        ('start_hdl', uint16_t),
        ('end_hdl', uint16_t),
        ('uuid', bth_gatt_uuid),
        ('data_size', uint16_t),
        ('data', (uint8_t*0)),
    ]
class random_type_2209(Structure):
    _fields_ = [
        ('descriptor_handle', uint16_t),
        ('descriptor_value', bth_gatt_descriptor_value),
    ]
class random_type_2210(Structure):
    _fields_ = [
        ('addr', bth_address),
        ('flag', uint16_t),
        ('descriptor', bth_gatt_descriptor),
    ]
bth_gatt_read_descriptor=random_type_2210
class random_type_2211(Structure):
    _fields_ = [
        ('addr', bth_address),
        ('handle_count', uint16_t),
        ('handle_set', (uint16_t*5)),
        ('offset', uint16_t),
        ('flags', uint16_t),
    ]
class random_type_2212(Structure):
    _fields_ = [
        ('addr', bth_address),
        ('handle_count', uint8_t),
        ('handle_set', (uint16_t*5)),
        ('value_count', uint8_t),
        ('value', ((uint8_t*10)*5)),
        ('flags', uint8_t),
    ]
class random_type_2213(Structure):
    _fields_ = [
        ('rmt_addr', bth_address),
        ('conn_handle', uint16_t),
        ('mtu_size', uint16_t),
        ('conn_state', uint8_t),
        ('os_type', uint8_t),
        ('pair_status', uint8_t),
        ('disc_reason', uint8_t),
    ]
class random_type_2214(Structure):
    _fields_ = [
        ('server_handle', uint32_t),
    ]
bth_gatt_add_server_result=random_type_2214
class random_type_2215(Structure):
    _fields_ = [
        ('service_handle', uint32_t),
    ]
class random_type_2216(Structure):
    _fields_ = [
        ('characteristic_handle', uint32_t),
    ]
bth_gatt_add_characteristic_result=random_type_2216
bth_gatt_get_gap_ui_result=random_type_2198
class random_type_2217(Structure):
    _fields_ = [
        ('addr', bth_address),
        ('start_hdl', uint16_t),
        ('end_hdl', uint16_t),
        ('uuid', bth_gatt_uuid),
    ]
class random_type_2218(Structure):
    _fields_ = [
        ('client_id', uint8_t),
        ('conn_id', uint16_t),
        ('uuid', bth_gatt_uuid),
        ('status', uint16_t),
    ]
class random_type_2219(Structure):
    _fields_ = [
        ('addr', bth_address),
        ('start_hdl', uint16_t),
        ('end_hdl', uint16_t),
        ('include_hdl', uint16_t),
        ('uuid', bth_att_uuid),
    ]
class random_type_2220(Structure):
    _fields_ = [
        ('addr', bth_address),
        ('characteristic', bth_gatt_characteristic),
    ]
class random_type_2221(Structure):
    _fields_ = [
        ('client_id', uint8_t),
        ('conn_id', uint16_t),
        ('service_handle', uint16_t),
        ('uuid', bth_gatt_uuid),
        ('status', uint16_t),
    ]
class random_type_2222(Structure):
    _fields_ = [
        ('addr', bth_address),
        ('character_hdl', uint16_t),
        ('descriptor_hdl', uint16_t),
        ('properties', uint16_t),
        ('descriptor_type', bth_gatt_descriptor_type),
        ('uuid', bth_gatt_uuid),
    ]
class random_type_2223(Structure):
    _fields_ = [
        ('client_id', uint8_t),
        ('conn_id', uint16_t),
        ('character_handle', uint16_t),
        ('status', uint16_t),
    ]
class random_type_2224(Structure):
    _fields_ = [
        ('client_id', uint8_t),
        ('conn_id', uint16_t),
        ('start_hdl', uint16_t),
        ('end_hdl', uint16_t),
        ('uuid', bth_gatt_uuid),
        ('status', uint16_t),
    ]
class random_type_2225(Structure):
    _fields_ = [
        ('addr', bth_address),
        ('descriptor_handle', uint16_t),
        ('value', bth_gatt_descriptor_value),
    ]
bth_gatt_read_descriptor_response=random_type_2225
class random_type_2226(Structure):
    _fields_ = [
        ('addr', bth_address),
        ('mtu', uint16_t),
    ]
class random_type_2227(Structure):
    _fields_ = [
        ('hook_list', list_stru),
        ('conn_list', list_stru),
        ('resv', td_u32),
    ]
ble_gap_local_data=random_type_2227
class random_type_2228(Structure):
    _fields_ = [
        ('rmt_addr', bth_address),
        ('conn_handle', uint16_t),
        ('mtu_size', uint16_t),
        ('conn_state', uint8_t),
        ('os_type', uint8_t),
        ('pair_status', uint8_t),
        ('disc_reason', uint8_t),
    ]
class random_type_2229(Structure):
    _fields_ = [
        ('bd', (uint8_t*6)),
        ('atype', uint8_t),
        ('cap', uint8_t),
    ]
class random_type_2230(Structure):
    _fields_ = [
        ('op', uint8_t),
        ('func', mpc_function),
    ]
class random_type_2231(Structure):
    _fields_ = [
        ('header', mpc_header),
        ('payload_length', uint16_t),
        ('src_transid', uint8_t),
        ('resv', uint8_t),
        ('payload', (uint8_t*0)),
    ]
class random_type_2232(Structure):
    _fields_ = [
        ('trans_id', uint8_t),
        ('sof', uint8_t),
        ('service_id', uint8_t),
        ('command_id', uint8_t),
    ]
class random_type_2233(Structure):
    _fields_ = [
        ('table_id', uint16_t),
        ('type', uint16_t),
        ('node_list', list_stru),
    ]
class random_type_2234(Structure):
    _fields_ = [
        ('event', uint16_t),
        ('cbk', sdk_ble_cbk),
    ]
class random_type_2235(Structure):
    _fields_ = [
        ('event_type', uint8_t),
        ('data_status', uint8_t),
        ('addr_type', uint8_t),
        ('addr', (uint8_t*6)),
        ('primary_phy', uint8_t),
        ('secondary_phy', uint8_t),
        ('adv_sid', uint8_t),
        ('tx_power', uint8_t),
        ('rssi', uint8_t),
        ('periodic_adv_interval', uint16_t),
        ('direct_addr_type', uint8_t),
        ('direct_addr', (uint8_t*6)),
        ('data_length', uint8_t),
        ('data', (uint8_t*1)),
    ]
class random_type_2237(Structure):
    _fields_ = [
        ('addr', bth_address),
        ('mask', uint8_t),
    ]
bth_gap_white_addr_node=random_type_2237
class random_type_2236(Structure):
    _fields_ = [
        ('count', uint32_t),
        ('addr_list', (bth_gap_white_addr_node*16)),
    ]
bth_gap_white_addr_list=random_type_2236
class random_type_2238(Structure):
    _fields_ = [
        ('rmt_dev_hdl', uint32_t),
        ('passkey_number', uint32_t),
    ]
bth_smp_passkey_rsp=random_type_2238
class random_type_2239(Structure):
    _fields_ = [
        ('rmt_dev_hdl', uint32_t),
        ('result', uint8_t),
    ]
class random_type_2240(Structure):
    _fields_ = [
        ('status', uint8_t),
        ('adv_handle', uint8_t),
        ('co_handle', uint16_t),
        ('num_cmp_ext_adv_events', uint8_t),
    ]
class random_type_2241(Structure):
    _fields_ = [
        ('addr', hci_address_stru),
        ('key', smp_key_stru_t),
    ]
class random_type_2242(Structure):
    _fields_ = [
        ('handle', uint32_t),
        ('rssi', int8_t),
        ('status', uint8_t),
        ('reserved', uint16_t),
    ]
class random_type_2243(Structure):
    _fields_ = [
        ('status', uint8_t),
        ('connection_handle', uint16_t),
        ('conn_interval', uint16_t),
        ('conn_latency', uint16_t),
        ('supervision_timeout', uint16_t),
    ]
class random_type_2244(Structure):
    _fields_ = [
        ('ext_adv_param', bth_gap_ext_adv_param),
        ('ext_adv_data', bth_gap_ext_adv_data),
        ('ext_adv_en', bth_gap_ext_adv_en),
    ]
class random_type_2245(Structure):
    _fields_ = [
        ('adv_handle', uint8_t),
        ('adv_type', uint8_t),
        ('adv_event_flag', uint8_t),
        ('primary_adv_channel_map', uint8_t),
        ('primary_adv_interval_min', uint32_t),
        ('primary_adv_interval_max', uint32_t),
        ('own_addr', (uint8_t*6)),
        ('peer_addr_type', uint8_t),
        ('peer_addr', (uint8_t*6)),
        ('adv_filter_policy', uint8_t),
        ('adv_tx_power', uint8_t),
        ('primary_adv_phy', uint8_t),
        ('secondary_adv_max_skip', uint8_t),
        ('secondary_adv_phy', uint8_t),
        ('advertising_sid', uint8_t),
        ('scan_request_notification_enable', uint8_t),
    ]
class random_type_2246(Structure):
    _fields_ = [
        ('fragment_preference', uint8_t),
        ('eir_flag', uint8_t),
        ('adv_data_length', uint16_t),
        ('scan_rsp_data_length', uint16_t),
        ('adv_data', td_u32),
        ('scan_rsp_data', td_u32),
    ]
class random_type_2247(Structure):
    _fields_ = [
        ('duration', uint16_t),
        ('max_extended_adv_events', uint8_t),
        ('priority', uint8_t),
    ]
sdk_ble_gap_cbk_event=td_u16
class random_type_2248(Structure):
    _fields_ = [
        ('ext_scan_param', bth_gap_ext_scan_param),
        ('ext_scan_en', bth_gap_ext_scan_en),
    ]
class random_type_2249(Structure):
    _fields_ = [
        ('scan_filter_policy', uint8_t),
        ('scan_phys', uint8_t),
        ('scan_type_1m', uint8_t),
        ('scan_type_coded', uint8_t),
        ('scan_interval_1m', uint16_t),
        ('scan_window_1m', uint16_t),
        ('scan_interval_coded', uint16_t),
        ('scan_window_coded', uint16_t),
    ]
class random_type_2250(Structure):
    _fields_ = [
        ('filter_duplicate', uint8_t),
        ('limited', uint8_t),
        ('duration', uint16_t),
        ('period', uint16_t),
    ]
class random_type_2251(Structure):
    _fields_ = [
        ('addr', bth_address),
        ('phys', bth_gap_le_set_phy_t),
    ]
class random_type_2252(Structure):
    _fields_ = [
        ('conn_handle', uint16_t),
        ('all_phys', uint8_t),
        ('tx_phys', uint8_t),
        ('rx_phys', uint8_t),
        ('phy_options', uint16_t),
    ]
class random_type_2253(Structure):
    _fields_ = [
        ('addr', bth_address),
        ('len', bth_gap_le_set_data_length),
    ]
class random_type_2254(Structure):
    _fields_ = [
        ('conn_handle', uint16_t),
        ('maxtxoctets', uint16_t),
        ('maxtxtime', uint16_t),
    ]
class random_type_2255(Structure):
    _fields_ = [
        ('addr', bth_address),
        ('interval_min', uint16_t),
        ('interval_max', uint16_t),
        ('slave_latency', uint16_t),
        ('timeout_multiplier', uint16_t),
    ]
class random_type_2256(Structure):
    _fields_ = [
        ('bondable', uint8_t),
        ('io_capability', uint8_t),
        ('sc_enable', uint8_t),
        ('sc_mode', uint8_t),
    ]
class random_type_2257(Structure):
    _fields_ = [
        ('addr', bth_address),
        ('ltk_len', uint8_t),
        ('ltk', (uint8_t*16)),
    ]
class random_type_2258(Structure):
    _fields_ = [
        ('hook_list', list_stru),
        ('psm_list', list_stru),
        ('conn_list', list_stru),
        ('resv', td_u32),
    ]
ble_l2cap_local_data=random_type_2258
class random_type_2259(Structure):
    _fields_ = [
        ('addr', bth_address),
        ('psm', uint16_t),
        ('cid', (uint16_t*2)),
        ('mps', (uint16_t*2)),
        ('mtu', (uint16_t*2)),
        ('init_credits', uint16_t),
        ('conn_hdl', uint16_t),
        ('channel_type', uint8_t),
        ('role', uint8_t),
        ('status', uint8_t),
    ]
class random_type_2260(Structure):
    _fields_ = [
        ('bd', (uint8_t*6)),
        ('atype', uint8_t),
        ('cap', uint8_t),
    ]
class random_type_2261(Structure):
    _fields_ = [
        ('psm', uint16_t),
        ('cid', uint16_t),
        ('channel_type', uint8_t),
        ('mps', uint16_t),
        ('mtu', uint16_t),
        ('init_credits', uint16_t),
        ('cbk', td_u32),
    ]
class random_type_2262(Structure):
    _fields_ = [
        ('table_id', uint16_t),
        ('type', uint16_t),
        ('node_list', list_stru),
    ]
class random_type_2263(Structure):
    _fields_ = [
        ('event', uint16_t),
        ('cbk', sdk_ble_cbk),
    ]
class random_type_2264(Structure):
    _fields_ = [
        ('conn', bth_l2cap_credit_conn),
        ('sub_ev', uint16_t),
        ('result', uint16_t),
    ]
class random_type_2265(Structure):
    _fields_ = [
        ('addr', bth_address),
        ('psm', uint16_t),
        ('cid', uint16_t),
        ('result', uint16_t),
    ]
class random_type_2266(Structure):
    _fields_ = [
        ('psm', uint16_t),
        ('cid', uint16_t),
        ('channel_type', uint8_t),
        ('mps', uint16_t),
        ('mtu', uint16_t),
        ('init_credits', uint16_t),
        ('cbk', td_u32),
    ]
class random_type_2267(Structure):
    _fields_ = [
        ('conn', bth_l2cap_credit_conn),
        ('sub_ev', uint16_t),
        ('result', uint16_t),
    ]
class random_type_2268(Structure):
    _fields_ = [
        ('addr', bth_address),
        ('psm', uint16_t),
        ('cid', (uint16_t*2)),
        ('mps', (uint16_t*2)),
        ('mtu', (uint16_t*2)),
        ('init_credits', uint16_t),
        ('conn_hdl', uint16_t),
        ('channel_type', uint8_t),
        ('role', uint8_t),
        ('status', uint8_t),
    ]
class random_type_2269(Structure):
    _fields_ = [
        ('bd', (uint8_t*6)),
        ('atype', uint8_t),
        ('cap', uint8_t),
    ]
class random_type_2270(Structure):
    _fields_ = [
        ('addr', bth_address),
        ('psm', uint16_t),
        ('cid', uint16_t),
        ('data', bth_data_t),
    ]
bth_l2cap_ch_tx=random_type_2270
class random_type_2271(Structure):
    _fields_ = [
        ('data_len', uint16_t),
        ('data', (uint8_t*0)),
    ]
class random_type_2272(Structure):
    _fields_ = [
        ('addr', bth_address),
        ('psm', uint16_t),
        ('cid', uint16_t),
        ('credits', uint16_t),
        ('r_credits', uint16_t),
    ]
bth_l2cap_credit_flow_cntl=random_type_2272
class random_type_2273(Structure):
    _fields_ = [
        ('addr', bth_address),
        ('psm', uint16_t),
        ('cid', uint16_t),
        ('result', uint16_t),
    ]
class random_type_2274(Structure):
    _fields_ = [
        ('l2cap_key_size', uint8_t),
        ('l2cap_item_need_auth_check', uint8_t),
    ]
bth_l2cap_config_param=random_type_2274
class random_type_2275(Structure):
    _fields_ = [
        ('ext_adv_param', bth_gap_ext_adv_param),
        ('ext_adv_data', bth_gap_ext_adv_data),
        ('ext_adv_en', bth_gap_ext_adv_en),
    ]
class random_type_2276(Structure):
    _fields_ = [
        ('adv_handle', uint8_t),
        ('adv_type', uint8_t),
        ('adv_event_flag', uint8_t),
        ('primary_adv_channel_map', uint8_t),
        ('primary_adv_interval_min', uint32_t),
        ('primary_adv_interval_max', uint32_t),
        ('own_addr', (uint8_t*6)),
        ('peer_addr_type', uint8_t),
        ('peer_addr', (uint8_t*6)),
        ('adv_filter_policy', uint8_t),
        ('adv_tx_power', uint8_t),
        ('primary_adv_phy', uint8_t),
        ('secondary_adv_max_skip', uint8_t),
        ('secondary_adv_phy', uint8_t),
        ('advertising_sid', uint8_t),
        ('scan_request_notification_enable', uint8_t),
    ]
class random_type_2277(Structure):
    _fields_ = [
        ('fragment_preference', uint8_t),
        ('eir_flag', uint8_t),
        ('adv_data_length', uint16_t),
        ('scan_rsp_data_length', uint16_t),
        ('adv_data', td_u32),
        ('scan_rsp_data', td_u32),
    ]
class random_type_2278(Structure):
    _fields_ = [
        ('duration', uint16_t),
        ('max_extended_adv_events', uint8_t),
        ('priority', uint8_t),
    ]
class random_type_2279(Structure):
    _fields_ = [
        ('rmt_addr', bth_address),
        ('conn_handle', uint16_t),
        ('mtu_size', uint16_t),
        ('conn_state', uint8_t),
        ('os_type', uint8_t),
        ('pair_status', uint8_t),
        ('disc_reason', uint8_t),
    ]
class random_type_2280(Structure):
    _fields_ = [
        ('bd', (uint8_t*6)),
        ('atype', uint8_t),
        ('cap', uint8_t),
    ]
class random_type_2281(Structure):
    _fields_ = [
        ('addr', bth_address),
        ('interval_min', uint16_t),
        ('interval_max', uint16_t),
        ('slave_latency', uint16_t),
        ('timeout_multiplier', uint16_t),
    ]
class random_type_2282(Structure):
    _fields_ = [
        ('bondable', uint8_t),
        ('io_capability', uint8_t),
        ('sc_enable', uint8_t),
        ('sc_mode', uint8_t),
    ]
class random_type_2283(Structure):
    _fields_ = [
        ('conn_handle', uint16_t),
        ('interval_min', uint16_t),
        ('interval_max', uint16_t),
        ('slave_latency', uint16_t),
        ('timeout_multiplier', uint16_t),
    ]
bth_gap_connection_param_app=random_type_2283
