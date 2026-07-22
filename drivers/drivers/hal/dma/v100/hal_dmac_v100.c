/**
 * Copyright (c) @CompanyNameMagicTag 2022-2022. All rights reserved. \n
 *
 * Description: Provides V100 HAL dma \n
 * Author: @CompanyNameTag \n
 * History: \n
 * 2022-10-16， Create file. \n
 */
#include "memory/osal_addr.h"
#include "securec.h"
#include "hal_dmac_v100_regs_op.h"
#include "hal_dmac_v100.h"
#ifdef CONFIG_SUPPORT_DATA_CACHE
#include "osal_adapt.h"
#endif

#define HAL_DMA_CH_MAX_TRANSFER_NUM 4096
#define HAL_DMA_CH_EN_WE_OFFSET 8

static void hal_dmac_v100_close(void);
static bool hal_dmac_v100_ch_is_enabled(dma_channel_t ch);

/* DMA ch informations */
typedef struct hal_dma_ch_info {
    hal_dma_ch_state_t state;                    /* ch transferring state */
    hal_dma_transfer_cb_t isr_callback;          /* ch ISR callback functions */
    uintptr_t arg;                               /* ch user arg */
} hal_dma_ch_info_t;

/* DMA transfer link list buffer. */
typedef struct hal_dma_lli {
    uint32_t src_addr;
    uint32_t dst_addr;
    struct hal_dma_lli *next;
    dma_ctrl_l_data_t ctl_l;
    dma_ctrl_h_data_t ctl_h;
    uint32_t src_stat;
    uint32_t dst_stat;
} hal_dma_lli_t;

static hal_dma_ch_info_t g_hal_dma_channel[DMA_CHANNEL_MAX_NUM];

static hal_dma_lli_t *g_dma_node_cfg[DMA_CHANNEL_MAX_NUM] = { NULL };
static hal_dma_lli_t *g_dma_lli_add[DMA_CHANNEL_MAX_NUM] = { NULL };

#if defined(CONFIG_DMA_SUPPORT_SMDMA)
/* pay attention please, the S_DMA_CHANNEL_MAX_NUM shouble keep same with g_sdma_burst_len_limit_ch size. */
static uint8_t g_sdma_burst_len_limit_ch[] = { HAL_DMA_BURST_TRANSACTION_LENGTH_4,
                                               HAL_DMA_BURST_TRANSACTION_LENGTH_4,
                                               HAL_DMA_BURST_TRANSACTION_LENGTH_8,
                                               HAL_DMA_BURST_TRANSACTION_LENGTH_16 };
#endif /* CONFIG_DMA_SUPPORT_SMDMA */

static dma_regs_t *hal_dma_type_get(dma_channel_t ch)
{
    if (ch >= B_DMA_CHANNEL_MAX_NUM) {
        return (dma_regs_t *)g_sdma_regs;
    } else {
        return (dma_regs_t *)g_dma_regs;
    }
}

static dma_channel_t hal_dma_ch_get(dma_channel_t ch)
{
    return (dma_channel_t)((uint32_t)ch % B_DMA_CHANNEL_MAX_NUM);
}

/* Get dma real channel num. */
static dma_channel_t hal_dma_get_real_channel_num(hal_dma_type_t dma_type, dma_channel_t ch)
{
    return (dma_channel_t)((dma_type == HAL_DMA_TYPE_SDMA) ? ((uint8_t)ch + B_DMA_CHANNEL_MAX_NUM) : ((uint8_t)ch));
}

/* Specific configuration to enable the DMA controller. */
static void hal_dma_enable_controller(void)
{
    hal_dma_cfg_reg_set_en((uint32_t)1, (dma_regs_t *)g_dma_regs);
#if defined(CONFIG_DMA_SUPPORT_SMDMA)
    hal_dma_cfg_reg_set_en((uint32_t)1, (dma_regs_t *)g_sdma_regs);
#endif /* CONFIG_DMA_SUPPORT_SMDMA */
}

/* Specific configuration to disable the DMA controller. */
static void hal_dma_disable_controller(void)
{
    hal_dma_cfg_reg_set_en((uint32_t)0, (dma_regs_t *)g_dma_regs);
#if defined(CONFIG_DMA_SUPPORT_SMDMA)
    hal_dma_cfg_reg_set_en((uint32_t)0, (dma_regs_t *)g_sdma_regs);
#endif /* CONFIG_DMA_SUPPORT_SMDMA */
}

static void hal_dmac_v100_reset(void)
{
    for (uint8_t ch = 0; ch < B_DMA_CHANNEL_MAX_NUM; ch++) {
        uapi_reg_setbit(&((dma_regs_t *)g_dma_regs)->dma_ch_en, ch + HAL_DMA_CH_EN_WE_OFFSET);
        g_hal_dma_channel[ch].state = HAL_DMA_CH_STATE_CLOSED;
    }
#if defined(CONFIG_DMA_SUPPORT_SMDMA)
    for (uint8_t ch = 0; ch < S_DMA_CHANNEL_MAX_NUM; ch++) {
        uapi_reg_setbit(&((dma_regs_t *)g_sdma_regs)->dma_ch_en, ch + HAL_DMA_CH_EN_WE_OFFSET);
        g_hal_dma_channel[ch + B_DMA_CHANNEL_MAX_NUM].state = HAL_DMA_CH_STATE_CLOSED;
    }
#endif /* CONFIG_DMA_SUPPORT_SMDMA */
    hal_dma_disable_controller();
    hal_dma_enable_controller();
}

static errcode_t hal_dmac_v100_init(void)
{
    if (hal_dma_regs_init() != ERRCODE_SUCC) {
        return ERRCODE_DMA_REG_ADDR_INVALID;
    }

    hal_dmac_v100_reset();

    return ERRCODE_SUCC;
}

static void hal_dmac_v100_deinit(void)
{
    hal_dmac_v100_close();
    hal_dma_regs_deinit();
}

static void hal_dmac_v100_open(void)
{
    hal_dma_enable_controller();

    for (uint8_t ch = 0; ch < DMA_CHANNEL_MAX_NUM; ch++) {
        g_hal_dma_channel[ch].state = HAL_DMA_CH_STATE_CLOSED;
    }
}

static void hal_dmac_v100_close(void)
{
    hal_dma_disable_controller();
}

static void hal_dma_lli_free(dma_channel_t ch)
{
    hal_dma_lli_t *dma_p = g_dma_node_cfg[ch];
    hal_dma_lli_t *dma_q;
    while (dma_p != NULL) {
        dma_q = dma_p->next;
        osal_kfree(dma_p);
        dma_p = dma_q;
    }
    g_dma_node_cfg[ch] = NULL;
}

static void hal_dmac_v100_ch_enable(dma_channel_t ch, bool en)
{
    dma_regs_t *tmp_dma_regs = hal_dma_type_get(ch);
    dma_channel_t tmp_dma_ch = hal_dma_ch_get(ch);

    if (en) {
#ifdef CONFIG_SUPPORT_DATA_CACHE
        osal_dcache_flush_all();
#endif
        dma_port_add_sleep_veto();
        uint32_t ch_data = bit((uint32_t)tmp_dma_ch) | bit((uint32_t)tmp_dma_ch + HAL_DMA_CH_EN_WE_OFFSET);
        uapi_reg_write32(&tmp_dma_regs->dma_ch_en, ch_data);
    } else {
        hal_dma_lli_free(ch);
        uapi_reg_setbit(&tmp_dma_regs->dma_ch_en, (uint32_t)tmp_dma_ch + HAL_DMA_CH_EN_WE_OFFSET);
        g_hal_dma_channel[ch].state = HAL_DMA_CH_STATE_CLOSED;
    }
}

static uint32_t hal_dmac_v100_get_block_ts(dma_channel_t ch)
{
    return hal_dma_ctrl_h_get_block_ts(hal_dma_ch_get(ch), hal_dma_type_get(ch));
}

static void hal_dmac_v100_interrupt_clear(dma_channel_t ch, hal_dma_interrupt_t int_type)
{
    dma_regs_t *tmp_dma_regs = hal_dma_type_get(ch);
    dma_channel_t tmp_dma_ch = hal_dma_ch_get(ch);
    switch (int_type) {
        case HAL_DMA_INTERRUPT_TFR:
            uapi_reg_write32(&tmp_dma_regs->dma_interrupt_type[DMA_INT_REG_CLEAR].tfr, bit(tmp_dma_ch));
            break;
        case HAL_DMA_INTERRUPT_BLOCK:
            uapi_reg_write32(&tmp_dma_regs->dma_interrupt_type[DMA_INT_REG_CLEAR].block, bit(tmp_dma_ch));
            break;
        case HAL_DMA_INTERRUPT_SRCTRAN:
            uapi_reg_write32(&tmp_dma_regs->dma_interrupt_type[DMA_INT_REG_CLEAR].src_tran, bit(tmp_dma_ch));
            break;
        case HAL_DMA_INTERRUPT_DSTTRAN:
            uapi_reg_write32(&tmp_dma_regs->dma_interrupt_type[DMA_INT_REG_CLEAR].dst_tran, bit(tmp_dma_ch));
            break;
        case HAL_DMA_INTERRUPT_ERR:
            uapi_reg_write32(&tmp_dma_regs->dma_interrupt_type[DMA_INT_REG_CLEAR].err, bit(tmp_dma_ch));
            break;
    }
}

#if defined(CONFIG_DMA_SUPPORT_SMDMA)
static dma_channel_t hal_dma_get_sdma_start_ch(hal_dma_burst_transaction_length_t burst_length)
{
    for (uint32_t i = 0; i < sizeof(g_sdma_burst_len_limit_ch); i++) {
        if (burst_length <= g_sdma_burst_len_limit_ch[i]) {
            return (dma_channel_t)(DMA_CHANNEL_8 + i);
        }
    }
    return (dma_channel_t)DMA_CHANNEL_MAX_NUM;
}
#endif /* CONFIG_DMA_SUPPORT_SMDMA */

static dma_channel_t hal_dmac_v100_ch_get_idle(hal_dma_handshaking_source_t source,
                                               hal_dma_burst_transaction_length_t burst_length)
{
    uint8_t ch;
    uint8_t max_ch_num = DMA_CHANNEL_MAX_NUM;
    dma_channel_t start_ch_num = DMA_CHANNEL_0;

#if defined(CONFIG_DMA_SUPPORT_SMDMA)
    if (source < HAL_DMA_HANDSHAKING_MAX_NUM) {
        if (source >= HAL_DMA_HANDSHAKING_SDMA) {
            max_ch_num = DMA_CHANNEL_MAX_NUM;
            start_ch_num = hal_dma_get_sdma_start_ch(burst_length);
        } else {
            max_ch_num = B_DMA_CHANNEL_MAX_NUM;
            start_ch_num = DMA_CHANNEL_0;
        }
    }
#else
    unused(burst_length);
    unused(source);
#endif /* CONFIG_DMA_SUPPORT_SMDMA */
    for (ch = start_ch_num; ch < max_ch_num; ch++) {
        if (!hal_dmac_v100_ch_is_enabled(ch) && (g_hal_dma_channel[ch].state == HAL_DMA_CH_STATE_CLOSED)) {
            g_hal_dma_channel[ch].state = HAL_DMA_CH_STATE_ACTIVE;
            return ch;
        }
    }
    return DMA_CHANNEL_NONE;
}

static errcode_t hal_dma_config_periph(dma_channel_t ch, hal_dma_transfer_peri_config_t *periph_cfg)
{
    if (dma_port_set_mux_channel(ch, periph_cfg) != ERRCODE_SUCC) {
        return ERRCODE_DMA_RET_HANDSHAKING_USING;
    }
    dma_regs_t *tmp_dma_regs = hal_dma_type_get(ch);
    dma_channel_t tmp_dma_ch = hal_dma_ch_get(ch);
    if (periph_cfg->trans_dir == HAL_DMA_TRANSFER_DIR_MEM_TO_PERIPHERAL) {
        hal_dma_cfg_l_set_hs_sel_dst(tmp_dma_ch, (uint32_t)HAL_DMA_HARDWARE_HANDSHAKING, tmp_dma_regs);
        hal_dma_cfg_l_set_hs_sel_src(tmp_dma_ch, (uint32_t)HAL_DMA_SOFTWARE_HANDSHAKING, tmp_dma_regs);
        dma_port_set_channel_handshaking_source((hal_dma_hardshaking_channel_t)tmp_dma_ch, periph_cfg->hs_dest);
    } else if (periph_cfg->trans_dir == HAL_DMA_TRANSFER_DIR_PERIPHERAL_TO_MEM) {
        hal_dma_cfg_l_set_hs_sel_src(tmp_dma_ch, (uint32_t)HAL_DMA_HARDWARE_HANDSHAKING, tmp_dma_regs);
        hal_dma_cfg_l_set_hs_sel_dst(tmp_dma_ch, (uint32_t)HAL_DMA_SOFTWARE_HANDSHAKING, tmp_dma_regs);
        dma_port_set_channel_handshaking_source((hal_dma_hardshaking_channel_t)tmp_dma_ch, periph_cfg->hs_source);
    } else {
        hal_dma_cfg_l_set_hs_sel_dst(tmp_dma_ch, (uint32_t)HAL_DMA_HARDWARE_HANDSHAKING, tmp_dma_regs);
        hal_dma_cfg_l_set_hs_sel_src(tmp_dma_ch, (uint32_t)HAL_DMA_HARDWARE_HANDSHAKING, tmp_dma_regs);
        dma_port_set_channel_handshaking_source((hal_dma_hardshaking_channel_t)tmp_dma_ch, periph_cfg->hs_dest);
    }
    hal_dma_cfg_h_set_src_per(tmp_dma_ch, (uint32_t)periph_cfg->hs_source, tmp_dma_regs);
    hal_dma_cfg_h_set_dest_per(tmp_dma_ch, (uint32_t)periph_cfg->hs_dest, tmp_dma_regs);
    hal_dma_cfg_h_set_protctl(tmp_dma_ch, (uint32_t)periph_cfg->protection, tmp_dma_regs);

    return ERRCODE_SUCC;
}

static void hal_dma_set_base_cfg_single_block(dma_channel_t ch, dma_regs_t *dma_regs,
                                              const hal_dma_transfer_base_config_t *base_cfg)
{
    /* Transfer src addr and dest addr */
    dma_regs->dma_ch_config[ch].src = base_cfg->src;
    dma_regs->dma_ch_config[ch].dst = base_cfg->dest;

    /* Transfer width */
    hal_dma_ctrl_l_set_src_tr_width(ch, (uint32_t)base_cfg->src_width, dma_regs);
    hal_dma_ctrl_l_set_dst_tr_width(ch, (uint32_t)base_cfg->dest_width, dma_regs);

    /* Address increment mode */
    hal_dma_ctrl_l_set_dinc(ch, (uint32_t)base_cfg->dest_inc, dma_regs);
    hal_dma_ctrl_l_set_sinc(ch, (uint32_t)base_cfg->src_inc, dma_regs);

    /* Burst Transaction Length */
    hal_dma_ctrl_l_set_dest_msize(ch, (uint32_t)base_cfg->dest_burst_trans_length, dma_regs);
    hal_dma_ctrl_l_set_src_msize(ch, (uint32_t)base_cfg->src_burst_trans_length, dma_regs);

    /* Transfer length */
    hal_dma_ctrl_h_set_block_ts(ch, (uint32_t)base_cfg->transfer_num, dma_regs);

    /* priority */
    hal_dma_cfg_l_set_ch_prior(ch, (uint32_t)base_cfg->priority, dma_regs);
}

static errcode_t hal_dmac_v100_config_single_block(dma_channel_t ch,
                                                   const hal_dma_transfer_base_config_t *base_cfg,
                                                   hal_dma_transfer_peri_config_t *periph_cfg)
{
    dma_regs_t *tmp_dma_regs = hal_dma_type_get(ch);
    dma_channel_t tmp_dma_ch = hal_dma_ch_get(ch);
    if (base_cfg->transfer_num >= HAL_DMA_CH_MAX_TRANSFER_NUM) {
        return ERRCODE_DMA_RET_TOO_MANY_DATA_TO_TRANSFER;
    }

    if (hal_dmac_v100_ch_is_enabled(ch)) {
        return ERRCODE_DMA_CH_BUSY;
    }

    hal_dma_set_base_cfg_single_block(tmp_dma_ch, tmp_dma_regs, base_cfg);

    /* master: only used in mdma/smdma */
    if ((dma_port_get_master_select(ch, base_cfg->dest) != HAL_DMA_MASTER_SELECT_ERROR) &&
        (dma_port_get_master_select(ch, base_cfg->src) != HAL_DMA_MASTER_SELECT_ERROR)) {
        hal_dma_ctrl_l_set_dms(tmp_dma_ch, (uint32_t)dma_port_get_master_select(ch, base_cfg->dest), tmp_dma_regs);
        hal_dma_ctrl_l_set_sms(tmp_dma_ch, (uint32_t)dma_port_get_master_select(ch, base_cfg->src), tmp_dma_regs);
    } else {
        return ERRCODE_DMA_RET_ADDRESS_ACCESS_ERROR;
    }

    if (base_cfg->callback != NULL) {
        hal_dma_ctrl_l_set_int_en(tmp_dma_ch, (uint32_t)1, tmp_dma_regs);
    }

    if (periph_cfg != NULL) {
        hal_dma_ctrl_l_set_tt_fc(tmp_dma_ch, (uint32_t)periph_cfg->trans_type, tmp_dma_regs);
        if (hal_dma_config_periph(ch, periph_cfg) != ERRCODE_SUCC) {
            return ERRCODE_DMA_RET_HANDSHAKING_USING;
        }
    } else {
        hal_dma_ctrl_l_set_tt_fc(tmp_dma_ch, (uint32_t)HAL_DMA_TRANS_MEMORY_TO_MEMORY_DMA, tmp_dma_regs);
    }

    /* Register callback or clear it */
    g_hal_dma_channel[ch].isr_callback = base_cfg->callback;
    g_hal_dma_channel[ch].arg = base_cfg->priv_arg;

    uint32_t ch_data = bit((uint32_t)tmp_dma_ch) | bit((uint32_t)tmp_dma_ch + HAL_DMA_CH_EN_WE_OFFSET);
    uapi_reg_write32(&(tmp_dma_regs->dma_interrupt_type[DMA_INT_REG_MASK].tfr), ch_data);
    uapi_reg_write32(&(tmp_dma_regs->dma_interrupt_type[DMA_INT_REG_MASK].err), ch_data);

    return ERRCODE_SUCC;
}

static void hal_dma_set_base_cfg_lli(dma_channel_t ch, const hal_dma_transfer_base_config_t *base_cfg)
{
    g_dma_lli_add[ch]->src_addr = base_cfg->src;
    g_dma_lli_add[ch]->dst_addr = base_cfg->dest;

    g_dma_lli_add[ch]->ctl_l.b.dst_tr_width = base_cfg->dest_width;
    g_dma_lli_add[ch]->ctl_l.b.src_tr_width = base_cfg->src_width;

    /* Address increment mode. */
    g_dma_lli_add[ch]->ctl_l.b.dinc = base_cfg->dest_inc;
    g_dma_lli_add[ch]->ctl_l.b.sinc = base_cfg->src_inc;

    /* Burst Transaction Length. */
    g_dma_lli_add[ch]->ctl_l.b.dest_msize = base_cfg->dest_burst_trans_length;
    g_dma_lli_add[ch]->ctl_l.b.src_msize = base_cfg->src_burst_trans_length;

    /* Transfer length. */
    g_dma_lli_add[ch]->ctl_h.b.block_ts = base_cfg->transfer_num;

    g_dma_lli_add[ch]->ctl_h.b.done = 0;
}

static errcode_t hal_dmac_lli_get(dma_channel_t ch)
{
    if (g_dma_node_cfg[ch] == NULL) {
        g_dma_node_cfg[ch] = (hal_dma_lli_t *)osal_kmalloc(sizeof(hal_dma_lli_t), 0);
        if (g_dma_node_cfg[ch] == NULL) {
            return ERRCODE_MALLOC;
        }
        g_dma_lli_add[ch] = g_dma_node_cfg[ch];
        g_dma_lli_add[ch]->next = NULL;
    } else {
        /* llp enable */
        g_dma_lli_add[ch]->ctl_l.b.llp_dst_en = 1;
        g_dma_lli_add[ch]->ctl_l.b.llp_src_en = 1;
        g_dma_lli_add[ch]->next = (hal_dma_lli_t *)osal_kmalloc(sizeof(hal_dma_lli_t), 0);
        if (g_dma_lli_add[ch]->next == NULL) {
            return ERRCODE_MALLOC;
        }
        g_dma_lli_add[ch] = g_dma_lli_add[ch]->next;
        g_dma_lli_add[ch]->next = NULL;
    }
    return ERRCODE_SUCC;
}

static errcode_t hal_dmac_v100_add_lli_transfer(dma_channel_t ch,
                                                const hal_dma_transfer_base_config_t *base_cfg,
                                                hal_dma_transfer_peri_config_t *periph_cfg)
{
    bool dma_flag = false;
    if (ch >= DMA_CHANNEL_MAX_NUM) { return ERRCODE_DMA_INVALID_PARAMETER; }

    if (g_dma_node_cfg[ch] == NULL) {
        dma_flag = true;
    }

    if (hal_dmac_lli_get(ch) != ERRCODE_SUCC) {
        return ERRCODE_MALLOC;
    }

    dma_regs_t *tmp_dma_regs = hal_dma_type_get(ch);
    dma_channel_t tmp_dma_ch = hal_dma_ch_get(ch);

    (void)memset_s((void *)g_dma_lli_add[ch], sizeof(hal_dma_lli_t), 0, sizeof(hal_dma_lli_t));

    hal_dma_set_base_cfg_lli(ch, base_cfg);

    if ((dma_port_get_master_select(ch, base_cfg->dest) != HAL_DMA_MASTER_SELECT_ERROR) &&
        (dma_port_get_master_select(ch, base_cfg->src) != HAL_DMA_MASTER_SELECT_ERROR)) {
        g_dma_lli_add[ch]->ctl_l.b.dms = (uint32_t)dma_port_get_master_select(ch, base_cfg->dest);
        g_dma_lli_add[ch]->ctl_l.b.sms = (uint32_t)dma_port_get_master_select(ch, base_cfg->src);
    } else {
        return ERRCODE_DMA_RET_ADDRESS_ACCESS_ERROR;
    }

    if (periph_cfg != NULL) {
        g_dma_lli_add[ch]->ctl_l.b.tt_fc = periph_cfg->trans_type;
        if (dma_flag) {
            if (hal_dma_config_periph(ch, periph_cfg) != ERRCODE_SUCC) {
                return ERRCODE_DMA_RET_HANDSHAKING_USING;
            }
        }
    } else {
        g_dma_lli_add[ch]->ctl_l.b.tt_fc = HAL_DMA_TRANS_MEMORY_TO_MEMORY_DMA;
    }

    g_dma_lli_add[ch]->ctl_l.b.int_en = 1;

    /* lli node address updata. */
    if (dma_flag) {
        hal_dma_cfg_l_set_ch_prior(tmp_dma_ch, (uint32_t)base_cfg->priority, tmp_dma_regs);
        uint32_t ch_data = bit((uint32_t)tmp_dma_ch) | bit((uint32_t)tmp_dma_ch + HAL_DMA_CH_EN_WE_OFFSET);
        uapi_reg_write32(&(tmp_dma_regs->dma_interrupt_type[DMA_INT_REG_MASK].tfr), ch_data);
        uapi_reg_write32(&(tmp_dma_regs->dma_interrupt_type[DMA_INT_REG_MASK].err), ch_data);
    }
    return ERRCODE_SUCC;
}

static bool hal_dmac_v100_ch_is_enabled(dma_channel_t ch)
{
    return (bool)uapi_reg_getbits(&(hal_dma_type_get(ch))->dma_ch_en, hal_dma_ch_get(ch), 1);
}

static void hal_dmac_v100_enable_lli(dma_channel_t ch, hal_dma_transfer_cb_t callback, uintptr_t arg)
{
    if (g_dma_node_cfg[ch] == NULL) { return; }
    dma_regs_t *tmp_dma_regs = hal_dma_type_get(ch);
    dma_channel_t tmp_dma_ch = hal_dma_ch_get(ch);
    tmp_dma_regs->dma_ch_config[tmp_dma_ch].src = g_dma_node_cfg[ch]->src_addr;
    tmp_dma_regs->dma_ch_config[tmp_dma_ch].dst = g_dma_node_cfg[ch]->dst_addr;

    tmp_dma_regs->dma_ch_config[tmp_dma_ch].ctl_l = g_dma_node_cfg[ch]->ctl_l.d32;
    tmp_dma_regs->dma_ch_config[tmp_dma_ch].ctl_h = g_dma_node_cfg[ch]->ctl_h.d32;

    tmp_dma_regs->dma_ch_config[tmp_dma_ch].llp = (uint32_t)(uintptr_t)g_dma_node_cfg[ch];

    g_hal_dma_channel[ch].isr_callback = callback;
    g_hal_dma_channel[ch].arg = arg;

    hal_dmac_v100_ch_enable(ch, true);
}

/* Get mdma/smdma channel max num. */
static uint8_t hal_dma_get_channel_max_num(hal_dma_type_t dma_type)
{
    return ((dma_type == HAL_DMA_TYPE_SDMA) ? (S_DMA_CHANNEL_MAX_NUM) : (B_DMA_CHANNEL_MAX_NUM));
}

/* Transfer Complete Interrupt. */
static dma_channel_t hal_dma_tfr_isr(hal_dma_type_t dma_type, const dma_regs_t *dma_regs)
{
    dma_channel_t real_ch = DMA_CHANNEL_NONE;
    uint8_t ch_max = hal_dma_get_channel_max_num(dma_type);
    for (uint8_t ch = 0; ch < ch_max; ch++) {
        if (uapi_reg_getbits(&dma_regs->dma_interrupt_type[DMA_INT_REG_RAW].tfr, ch, 1) != 0) {
            real_ch = hal_dma_get_real_channel_num(dma_type, ch);
            hal_dmac_v100_interrupt_clear(real_ch, HAL_DMA_INTERRUPT_TFR);
            g_hal_dma_channel[real_ch].state = HAL_DMA_CH_STATE_CLOSED;
            if (g_hal_dma_channel[real_ch].isr_callback != NULL) {
                g_hal_dma_channel[real_ch].isr_callback(HAL_DMA_INTERRUPT_TFR, real_ch, g_hal_dma_channel[real_ch].arg);
            }
        }
    }
    return real_ch;
}

/* Block Transfer Complete. */
static dma_channel_t hal_dma_block_isr(hal_dma_type_t dma_type, const dma_regs_t *dma_regs)
{
    dma_channel_t real_ch = DMA_CHANNEL_NONE;
    uint8_t ch_max = hal_dma_get_channel_max_num(dma_type);
    for (uint8_t ch = 0; ch < ch_max; ch++) {
        if (uapi_reg_getbits(&dma_regs->dma_interrupt_type[DMA_INT_REG_RAW].block, ch, 1) != 0) {
            real_ch = hal_dma_get_real_channel_num(dma_type, ch);
            hal_dmac_v100_interrupt_clear(real_ch, HAL_DMA_INTERRUPT_BLOCK);
            g_hal_dma_channel[real_ch].state = HAL_DMA_CH_STATE_CLOSED;
        }
    }
    return real_ch;
}

/* DMA src tran Interrupt. */
static dma_channel_t hal_dma_src_tran_isr(hal_dma_type_t dma_type, const dma_regs_t *dma_regs)
{
    dma_channel_t real_ch = DMA_CHANNEL_NONE;
    uint8_t ch_max = hal_dma_get_channel_max_num(dma_type);
    for (uint8_t ch = 0; ch < ch_max; ch++) {
        if (uapi_reg_getbits(&dma_regs->dma_interrupt_type[DMA_INT_REG_RAW].src_tran, ch, 1) != 0) {
            real_ch = hal_dma_get_real_channel_num(dma_type, ch);
            hal_dmac_v100_interrupt_clear(real_ch, HAL_DMA_INTERRUPT_SRCTRAN);
            g_hal_dma_channel[real_ch].state = HAL_DMA_CH_STATE_CLOSED;
        }
    }
    return real_ch;
}

/* DMA dst tran Interrupt. */
static dma_channel_t hal_dma_dst_tran_isr(hal_dma_type_t dma_type, const dma_regs_t *dma_regs)
{
    dma_channel_t real_ch = DMA_CHANNEL_NONE;
    uint8_t ch_max = hal_dma_get_channel_max_num(dma_type);
    for (uint8_t ch = 0; ch < ch_max; ch++) {
        if (uapi_reg_getbits(&dma_regs->dma_interrupt_type[DMA_INT_REG_RAW].dst_tran, ch, 1) != 0) {
            real_ch = hal_dma_get_real_channel_num(dma_type, ch);
            hal_dmac_v100_interrupt_clear(real_ch, HAL_DMA_INTERRUPT_DSTTRAN);
            g_hal_dma_channel[real_ch].state = HAL_DMA_CH_STATE_CLOSED;
        }
    }
    return real_ch;
}

/* DMA Error Interrupt. */
static dma_channel_t hal_dma_err_isr(hal_dma_type_t dma_type, const dma_regs_t *dma_regs)
{
    dma_channel_t real_ch = DMA_CHANNEL_NONE;
    uint8_t ch_max = hal_dma_get_channel_max_num(dma_type);
    for (uint8_t ch = 0; ch < ch_max; ch++) {
        if (uapi_reg_getbits(&dma_regs->dma_interrupt_type[DMA_INT_REG_RAW].err, ch, 1) != 0) {
            real_ch = hal_dma_get_real_channel_num(dma_type, ch);
            hal_dmac_v100_interrupt_clear(real_ch, HAL_DMA_INTERRUPT_ERR);
            g_hal_dma_channel[real_ch].state = HAL_DMA_CH_STATE_CLOSED;
            if (g_hal_dma_channel[real_ch].isr_callback != NULL) {
                g_hal_dma_channel[real_ch].isr_callback(HAL_DMA_INTERRUPT_ERR, real_ch, g_hal_dma_channel[real_ch].arg);
            }
        }
    }
    return real_ch;
}

static void hal_dma_clear_mux_channel_stat(dma_channel_t ch)
{
    dma_regs_t *tmp_dma_regs = hal_dma_type_get(ch);
    dma_channel_t tmp_dma_ch = hal_dma_ch_get(ch);
    if (ch != DMA_CHANNEL_NONE) {
        uint8_t src_per = (uint8_t)hal_dma_cfg_h_get_src_per(tmp_dma_ch, tmp_dma_regs);
        uint8_t dest_per = (uint8_t)hal_dma_cfg_h_get_dest_per(tmp_dma_ch, tmp_dma_regs);
        if (dest_per == HAL_DMA_HANDSHAKING_I2C2_TX || dest_per == HAL_DMA_HANDSHAKING_UART_L_TX ||
            dest_per == HAL_DMA_HANDSHAKING_IR_TX || dest_per == HAL_DMA_HANDSHAKING_QSPI0_2CS_TX) {
            dma_port_set_handshaking_channel_status(dest_per, false);
        }
        if (src_per == HAL_DMA_HANDSHAKING_I2C2_RX || src_per == HAL_DMA_HANDSHAKING_UART_L_RX ||
            src_per == HAL_DMA_HANDSHAKING_IR_RX || src_per == HAL_DMA_HANDSHAKING_QSPI0_2CS_RX) {
            dma_port_set_handshaking_channel_status(src_per, false);
        }
    }
}

static void hal_dma_isr(hal_dma_type_t dma_type)
{
    dma_channel_t int_channel = DMA_CHANNEL_NONE;
    dma_regs_t *tmp_dma_regs;
    if (dma_type == HAL_DMA_TYPE_BDMA) {
        tmp_dma_regs = (dma_regs_t *)g_dma_regs;
    } else {
        tmp_dma_regs = (dma_regs_t *)g_sdma_regs;
    }
    /* Transfer Complete. */
    if (hal_dma_status_data_get_tfr(tmp_dma_regs)) {
        int_channel = hal_dma_tfr_isr(dma_type, tmp_dma_regs);
    }

    /* Block Transfer Complete. */
    if (hal_dma_status_data_get_block(tmp_dma_regs)) {
        int_channel = hal_dma_block_isr(dma_type, tmp_dma_regs);
    }

    /* Source Transaction Complete. */
    if (hal_dma_status_data_get_srct(tmp_dma_regs)) {
        int_channel = hal_dma_src_tran_isr(dma_type, tmp_dma_regs);
    }

    /* Destination Transaction Complete. */
    if (hal_dma_status_data_get_dstt(tmp_dma_regs)) {
        int_channel = hal_dma_dst_tran_isr(dma_type, tmp_dma_regs);
    }

    /* Error. */
    if (hal_dma_status_data_get_err(tmp_dma_regs)) {
        int_channel = hal_dma_err_isr(dma_type, tmp_dma_regs);
    }
    hal_dma_clear_mux_channel_stat(int_channel);
}

void hal_dma_irq_handler(void)
{
    hal_dma_isr(HAL_DMA_TYPE_BDMA);

    /* remove pm veto */
    dma_port_remove_sleep_veto();
}

#if defined(CONFIG_DMA_SUPPORT_SMDMA)
void hal_sdma_irq_handler(void)
{
    hal_dma_isr(HAL_DMA_TYPE_SDMA);

    /* remove pm veto */
    dma_port_remove_sleep_veto();
}
#endif /* CONFIG_DMA_SUPPORT_SMDMA */

static hal_dma_funcs_t g_hal_dmac_v100_funcs = {
    .init = hal_dmac_v100_init,
    .deinit = hal_dmac_v100_deinit,
    .open = hal_dmac_v100_open,
    .close = hal_dmac_v100_close,
    .ch_enable = hal_dmac_v100_ch_enable,
    .get_block = hal_dmac_v100_get_block_ts,
    .clear = hal_dmac_v100_interrupt_clear,
    .get_idle = hal_dmac_v100_ch_get_idle,
    .cfg_single = hal_dmac_v100_config_single_block,
    .add_lli = hal_dmac_v100_add_lli_transfer,
    .is_enabled = hal_dmac_v100_ch_is_enabled,
    .enable_lli = hal_dmac_v100_enable_lli
};

hal_dma_funcs_t *hal_dmac_v100_funcs_get(void)
{
    return &g_hal_dmac_v100_funcs;
}
