/*
 * Copyright (c) @CompanyNameMagicTag 2015-2022. All rights reserved.
 */

#ifndef BTTASK_H
#define BTTASK_H

typedef void (func_ind_msg)(uint8_t msg_id, uint8_t* param);

#define RX_RESERVE_DONE        0
#define RX_RESERVE_INIT        1
#define RX_USER_MSG            10

struct ind_msg_stru {
    uint8_t* param;
    func_ind_msg* func;
    uint8_t msg_id;
};

void init_rx_loop(void);
void send_ind_msg(uint8_t* param, func_ind_msg* func, uint8_t msg_id);
#define done_rx_loop()    send_ind_msg(NULL, NULL, RX_RESERVE_DONE)

void rx_loop(void);
uint8_t process_msg(void);
#endif
