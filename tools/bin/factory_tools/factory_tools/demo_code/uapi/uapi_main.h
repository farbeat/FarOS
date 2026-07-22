#ifndef UAPI_MAIN_H
#define UAPI_MAIN_H

typedef enum {
    FAIL = 0x0,
    SUCC = 0x1,
} uapi_errcode;

typedef struct {
    uint32_t total_size; // 当前传输文件总大小
    uint32_t transmited_size; // 当前文件已传输大小
} uapi_transmit_progress;

typedef struct {
    uint16_t sn; // 序号
    uint8_t name_len; // 文件（夹）名长度
    uint8_t is_dir; // 是否是文件夹
    uint32_t file_size; // 文件大小
    uint8_t name[0xFF]; // 文件（夹）名
} uapi_ls_info_t;

typedef uint32_t (*uart_rx_function)(void *buf, uint32_t len); //  uart RX 回调函数
typedef uint32_t (*at_result_callback)(void *buf, uint32_t len); // AT 命令返回值回调函数，入参buf包括 mould_id + cmd_id + payload
typedef uint32_t (*transmit_ls_result)(uint32_t size, uapi_ls_info_t *buff); // transmit get list回调函数，入参 size 为文件（夹）的数量，buff 为文件（夹）的信息
typedef uint32_t (*transmit_result_hook)(bool is_finished, uintptr_t usr_data); // 传输结果回调函数，is_finished 为传输结束标志，usr_data 预留接口，目前为NULL

uapi_errcode uapi_uart_open(const char *com,  uint32_t baud); // 打开uart口， com 格式为 "\\\\.\\COM47"， baud 为波特率
uapi_errcode uapi_uart_tx_data(void *buf, uint32_t buf_len); // uart TX 发送数据buf
uapi_errcode uapi_uart_set_rts_dtr(int rts_flag, int dtr_flag); // uart 设置rts和dtr开关
uapi_errcode uapi_uart_callback_register(uart_rx_function callback); // uart 注册回调函数
uapi_errcode uapi_uart_close(void); // 关闭uart口

uapi_errcode uapi_at_send(uint8_t module_id, uint8_t cmd_id, uint8_t *pkt, uint32_t pkt_size, at_result_callback callback); // at命令发送接口，module_id及cmd_id请参考《Sparta 产线 测试指南》10.2章
uapi_errcode uapi_file_download(char* board_file_path, char* local_file_path, transmit_result_hook handler); // 文件下载接口，参数分别为板端文件地址（详细到文件后缀），pc端存储路径（详细到文件夹），传输结果回调函数
uapi_errcode uapi_file_upload(char* board_file_path,char* local_file_path, transmit_result_hook handler); // 文件上传接口，参数分别为板端文件地址（详细到文件后缀），pc端存储路径（详细到文件后缀），传输结果回调函数
uapi_errcode uapi_ls_send(char *path, uint32_t path_len, transmit_ls_result callback); // 查询文件列表接口，参数为板端文件路径（详细到文件夹），路径长度，get list回调函数
uapi_errcode uapi_file_delete(char *board_file_path, uint32_t board_file_path_len); // 删除指定文件接口，参数为板端文件地址（详细到文件后缀），文件路径长度
uapi_errcode uapi_file_transmit_stop(void); // 停止文件下载接口
uapi_errcode uapi_transmit_process(uapi_transmit_progress *data); // 获取文件传输进度，入参 data 更新为正在传输的文件的总大小和已传输大小
#endif
