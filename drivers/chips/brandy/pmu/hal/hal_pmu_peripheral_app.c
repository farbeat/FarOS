/*
 * Copyright (c) @CompanyNameMagicTag 2020-2022. All rights reserved.
 * Description:  HAL APP PMU DRIVER.
 */

#include "hal_pmu_peripheral_app.h"
#include "chip_definitions.h"
#include "non_os.h"
#include "chip_io.h"
#include "hal_pmu_peripheral.h"
#include "hal_clocks.h"
#include "hal_clocks_sdio.h"
#include "hal_clocks_app.h"

/* MDMA, SMDMA and MTIMER peripheral clock gating. */
#define PMU_TOP_PERIPHERALS_MTOP0_L_REG (M_CTL_RB_BASE + 0x150)
#define PMU_MTOP0_MDMA_RST              2
#define PMU_MTOP0_SMDMA_RST             1
#define PMU_MTOP0_TIMER_RST             0
#define PMU_TOP_PERIPHERALS_MTOP0_H_REG (M_CTL_RB_BASE + 0x154)
#define PMU_MTOP0_MDMA_STS_OFFSET       2
#define PMU_MTOP0_SMDMA_STS_OFFSET      1
#define PMU_MTOP0_MTIMER_STS_OFFSET     0

/* I2C0/1/2/3 peripheral clock gating. */
#define PMU_TOP_PERIPHERALS_MTOP1_L_REG (M_CTL_RB_BASE + 0x158)
#define PMU_MTOP1_I2C3_RST              3
#define PMU_MTOP1_I2C2_RST              2
#define PMU_MTOP1_I2C1_RST              1
#define PMU_MTOP1_I2C0_RST              0
#define PMU_TOP_PERIPHERALS_MTOP1_H_REG (M_CTL_RB_BASE + 0x15C)
#define PMU_MTOP1_I2C3_STS_OFFSET       3
#define PMU_MTOP1_I2C2_STS_OFFSET       2
#define PMU_MTOP1_I2C1_STS_OFFSET       1
#define PMU_MTOP1_I2C0_STS_OFFSET       0

/* SPI0_M, SPI1_MS, SPI2_MS and QSPI2_1CS peripheral clock gating. */
#define PMU_TOP_PERIPHERALS_MTOP2_L_REG (M_CTL_RB_BASE + 0x160)
#define PMU_MTOP2_QSPI2_1CS_RST         3
#define PMU_MTOP2_SPI2_RST              2
#define PMU_MTOP2_SPI1_RST              1
#define PMU_MTOP2_SPI0_RST              0
#define PMU_TOP_PERIPHERALS_MTOP2_H_REG (M_CTL_RB_BASE + 0x164)
#define PMU_MTOP2_QSPI2_1CS_STS_OFFSET  3
#define PMU_MTOP2_SPI2_STS_OFFSET       2
#define PMU_MTOP2_SPI1_STS_OFFSET       1
#define PMU_MTOP2_SPI0_STS_OFFSET       0

/* UART_H0, UART_H1 and UART_L0 peripheral clock gating. */
#define PMU_TOP_PERIPHERALS_MTOP3_L_REG (M_CTL_RB_BASE + 0x168)
#define PMU_MTOP3_UART_L0_RST           2
#define PMU_MTOP3_UART_H1_RST           1
#define PMU_MTOP3_UART_H0_RST           0
#define PMU_TOP_PERIPHERALS_MTOP3_H_REG (M_CTL_RB_BASE + 0x16C)
#define PMU_MTOP3_UART_H1_STS_OFFSET    1
#define PMU_MTOP3_UART_H0_STS_OFFSET    0

/* EMMC and SDIO_HOST peripheral clock gating. */
#define PMU_TOP_PERIPHERALS_MTOP4_L_REG (M_CTL_RB_BASE + 0x170)
#define PMU_MTOP4_SDIO_HOST_RST         1
#define PMU_MTOP4_EMMC_RST              0
#define PMU_TOP_PERIPHERALS_MTOP4_H_REG (M_CTL_RB_BASE + 0x174)
#define PMU_MTOP4_SDIO_HOST_STS_OFFSET  1
#define PMU_MTOP4_EMMC_STS_OFFSET       0

/* I2C4 5 6 and spi3 peripheral clock gating. */
#define PMU_TOP_PERIPHERALS_MTOP5_L_REG (M_CTL_RB_BASE + 0x178)
#define PMU_MTOP4_I2C_BUS_4_RST         0
#define PMU_MTOP4_I2C_BUS_5_RST         1
#define PMU_MTOP4_I2C_BUS_6_RST         2
#define PMU_MTOP4_SPI_M3_RST            3
#define PMU_TOP_PERIPHERALS_MTOP5_H_REG (M_CTL_RB_BASE + 0x17c)
#define PMU_MTOP4_I2C_BUS_4_STS_OFFSET  0
#define PMU_MTOP4_I2C_BUS_5_STS_OFFSET  1
#define PMU_MTOP4_I2C_BUS_6_STS_OFFSET  2
#define PMU_MTOP4_SPI_M3_STS_OFFSET     3

/* QSPI0_2CS, QSPI1_2CS and OPI peripheral clock gating. */
#define PMU_MEM_SUB_PERIPHERALS_CONFIG_REG (XIP_CACHE_CTL + 0x700)
#define PMU_MEM_QSPI0_2CS_RST           0
#define PMU_MEM_QSPI1_2CS_RST           1
#define PMU_MEM_OPI_RST                 2
#define PMU_MEM_QSPI3_2CS_RST           3

#define PMU_MEM_SUB_PERIPHERALS_STATUS_REG (XIP_CACHE_CTL + 0x700)
#define PMU_MEM_QSPI0_2CS_STS_OFFSET    8
#define PMU_MEM_QSPI1_2CS_STS_OFFSET    9
#define PMU_MEM_OPI_STS_OFFSET          10
#define PMU_MEM_QSPI3_2CS_STS_OFFSET    11
#define CMU_CLOCKS_MCU_PERP_LS_CR_REG      (M_CTL_RB_BASE + 0x554)

//lint -esym(551, g_clocks_state_in_abnormal) Symbol not accessed
static volatile hal_pmu_m_clocks_state_t g_clocks_state_in_abnormal = { 0 };

static hal_pmu_perip_status_type_t hal_pmu_mcpum_top0_perip_get_status(hal_pmu_mcpu_perips_type_t perip)
{
    uint16_t offset;

    switch (perip) {
        case HAL_PMU_MCPU_PERIP_TIMER:
            offset = PMU_MTOP0_MTIMER_STS_OFFSET;
            break;
        case HAL_PMU_MCPU_PERIP_SMDMA:
            offset = PMU_MTOP0_SMDMA_STS_OFFSET;
            break;
        case HAL_PMU_MCPU_PERIP_MDMA:
            offset = PMU_MTOP0_MDMA_STS_OFFSET;
            break;
        default:
            return HAL_PMU_PERIP_STATUS_CLOCK_OFF;
    }

    return (hal_pmu_perip_status_type_t)reg16_getbits(PMU_TOP_PERIPHERALS_MTOP0_H_REG, offset, PMU_PERIPHERAL_STS_BITS);
}

static hal_pmu_perip_status_type_t hal_pmu_mcpum_top1_perip_get_status(hal_pmu_mcpu_perips_type_t perip)
{
    uint16_t offset;

    switch (perip) {
        case HAL_PMU_MCPU_PERIP_I2C_BUS_0:
            offset = PMU_MTOP1_I2C0_STS_OFFSET;
            break;
        case HAL_PMU_MCPU_PERIP_I2C_BUS_1:
            offset = PMU_MTOP1_I2C1_STS_OFFSET;
            break;
        case HAL_PMU_MCPU_PERIP_I2C_BUS_2:
            offset = PMU_MTOP1_I2C2_STS_OFFSET;
            break;
        case HAL_PMU_MCPU_PERIP_I2C_BUS_3:
            offset = PMU_MTOP1_I2C3_STS_OFFSET;
            break;
        default:
            return HAL_PMU_PERIP_STATUS_CLOCK_OFF;
    }

    return (hal_pmu_perip_status_type_t)reg16_getbits(PMU_TOP_PERIPHERALS_MTOP1_H_REG, offset, PMU_PERIPHERAL_STS_BITS);
}

static hal_pmu_perip_status_type_t hal_pmu_mcpum_top2_perip_get_status(hal_pmu_mcpu_perips_type_t perip)
{
    uint16_t offset;

    switch (perip) {
        case HAL_PMU_MCPU_PERIP_SPI0_M:
            offset = PMU_MTOP2_SPI0_STS_OFFSET;
            break;
        case HAL_PMU_MCPU_PERIP_SPI1_MS:
            offset = PMU_MTOP2_SPI1_STS_OFFSET;
            break;
        case HAL_PMU_MCPU_PERIP_SPI2_MS:
            offset = PMU_MTOP2_SPI2_STS_OFFSET;
            break;
        case HAL_PMU_MCPU_PERIP_QSPI2_1CS:
            offset = PMU_MTOP2_QSPI2_1CS_STS_OFFSET;
            break;
        default:
            return HAL_PMU_PERIP_STATUS_CLOCK_OFF;
    }

    return (hal_pmu_perip_status_type_t)reg16_getbits(PMU_TOP_PERIPHERALS_MTOP2_H_REG, offset, PMU_PERIPHERAL_STS_BITS);
}

static hal_pmu_perip_status_type_t hal_pmu_mcpum_top3_perip_get_status(hal_pmu_mcpu_perips_type_t perip)
{
    uint16_t offset;

    switch (perip) {
        case HAL_PMU_MCPU_PERIP_UART_H0:
            offset = PMU_MTOP3_UART_H0_STS_OFFSET;
            break;
        case HAL_PMU_MCPU_PERIP_UART_H1:
            offset = PMU_MTOP3_UART_H1_STS_OFFSET;
            break;
        default:
            return HAL_PMU_PERIP_STATUS_CLOCK_OFF;
    }

    return (hal_pmu_perip_status_type_t)reg16_getbits(PMU_TOP_PERIPHERALS_MTOP3_H_REG, offset, PMU_PERIPHERAL_STS_BITS);
}

static hal_pmu_perip_status_type_t hal_pmu_mcpum_top4_perip_get_status(hal_pmu_mcpu_perips_type_t perip)
{
    uint16_t offset;

    switch (perip) {
        case HAL_PMU_MCPU_PERIP_EMMC:
            offset = PMU_MTOP4_EMMC_STS_OFFSET;
            break;
        case HAL_PMU_MCPU_PERIP_SDIO_HOST:
            offset = PMU_MTOP4_SDIO_HOST_STS_OFFSET;
            break;
        default:
            return HAL_PMU_PERIP_STATUS_CLOCK_OFF;
    }

    return (hal_pmu_perip_status_type_t)reg16_getbits(PMU_TOP_PERIPHERALS_MTOP4_H_REG, offset, PMU_PERIPHERAL_STS_BITS);
}

hal_pmu_perip_status_type_t hal_pmu_mcpu_perip_get_status(hal_pmu_mcpu_perips_type_t perip)
{
    if (perip <= HAL_PMU_MCPU_PERIP_TIMER) {
        return hal_pmu_mcpum_top0_perip_get_status(perip);
    } else if (perip >= HAL_PMU_MCPU_PERIP_I2C_BUS_0 && perip <= HAL_PMU_MCPU_PERIP_I2C_BUS_3) {
        return hal_pmu_mcpum_top1_perip_get_status(perip);
    } else if (perip <= HAL_PMU_MCPU_PERIP_SPI2_MS) {
        return hal_pmu_mcpum_top2_perip_get_status(perip);
    } else if (perip <= HAL_PMU_MCPU_PERIP_UART_H1) {
        return hal_pmu_mcpum_top3_perip_get_status(perip);
    } else if (perip <= HAL_PMU_MCPU_PERIP_SDIO_HOST) {
        return hal_pmu_mcpum_top4_perip_get_status(perip);
    } else {
        return HAL_PMU_PERIP_STATUS_CLOCK_OFF;
    }
}

static void hal_pmu_mcpu_mtop0_perip_reset(hal_pmu_mcpu_perips_type_t perip,
                                           hal_pmu_perip_request_type_t request)
{
    uint16_t bit;

    switch (perip) {
        case HAL_PMU_MCPU_PERIP_TIMER:
            bit = PMU_MTOP0_TIMER_RST;
            break;
        case HAL_PMU_MCPU_PERIP_SMDMA:
            bit = PMU_MTOP0_SMDMA_RST;
            break;
        case HAL_PMU_MCPU_PERIP_MDMA:
            bit = PMU_MTOP0_MDMA_RST;
            break;
        default:
            return;
    }

    if (request == HAL_PMU_PERIP_REQUEST_RESET) {
        reg16_clrbit(PMU_TOP_PERIPHERALS_MTOP0_L_REG, bit);
    } else if (request == HAL_PMU_PERIP_REQUEST_DERESET) {
        reg16_setbit(PMU_TOP_PERIPHERALS_MTOP0_L_REG, bit);
    }
}

static void hal_pmu_mcpu_mtop1_perip_reset(hal_pmu_mcpu_perips_type_t perip,
                                           hal_pmu_perip_request_type_t request)
{
    uint16_t bit;

    switch (perip) {
        case HAL_PMU_MCPU_PERIP_I2C_BUS_0:
            bit = PMU_MTOP1_I2C0_RST;
            break;
        case HAL_PMU_MCPU_PERIP_I2C_BUS_1:
            bit = PMU_MTOP1_I2C1_RST;
            break;
        case HAL_PMU_MCPU_PERIP_I2C_BUS_2:
            bit = PMU_MTOP1_I2C2_RST;
            break;
        case HAL_PMU_MCPU_PERIP_I2C_BUS_3:
            bit = PMU_MTOP1_I2C3_RST;
            break;
        default:
            return;
    }

    if (request == HAL_PMU_PERIP_REQUEST_RESET) {
        reg16_clrbit(PMU_TOP_PERIPHERALS_MTOP1_L_REG, bit);
    } else if (request == HAL_PMU_PERIP_REQUEST_DERESET) {
        reg16_setbit(PMU_TOP_PERIPHERALS_MTOP1_L_REG, bit);
    }
}

static void hal_pmu_mcpu_mtop2_perip_reset(hal_pmu_mcpu_perips_type_t perip,
                                           hal_pmu_perip_request_type_t request)
{
    uint16_t bit;

    switch (perip) {
        case HAL_PMU_MCPU_PERIP_SPI0_M:
            bit = PMU_MTOP2_SPI0_RST;
            break;
        case HAL_PMU_MCPU_PERIP_SPI1_MS:
            bit = PMU_MTOP2_SPI1_RST;
            break;
        case HAL_PMU_MCPU_PERIP_SPI2_MS:
            bit = PMU_MTOP2_SPI2_RST;
            break;
        case HAL_PMU_MCPU_PERIP_QSPI2_1CS:
            bit = PMU_MTOP2_QSPI2_1CS_RST;
            break;
        default:
            return;
    }

    if (request == HAL_PMU_PERIP_REQUEST_RESET) {
        reg16_clrbit(PMU_TOP_PERIPHERALS_MTOP2_L_REG, bit);
    } else if (request == HAL_PMU_PERIP_REQUEST_DERESET) {
        reg16_setbit(PMU_TOP_PERIPHERALS_MTOP2_L_REG, bit);
    }
}

static void hal_pmu_mcpu_mtop3_perip_reset(hal_pmu_mcpu_perips_type_t perip,
                                           hal_pmu_perip_request_type_t request)
{
    uint16_t bit;

    switch (perip) {
        case HAL_PMU_MCPU_PERIP_UART_H0:
            bit = PMU_MTOP3_UART_H0_RST;
            break;
        case HAL_PMU_MCPU_PERIP_UART_H1:
            bit = PMU_MTOP3_UART_H1_RST;
            break;
        default:
            return;
    }

    if (request == HAL_PMU_PERIP_REQUEST_RESET) {
        reg16_clrbit(PMU_TOP_PERIPHERALS_MTOP3_L_REG, bit);
    } else if (request == HAL_PMU_PERIP_REQUEST_DERESET) {
        reg16_setbit(PMU_TOP_PERIPHERALS_MTOP3_L_REG, bit);
    }
}

static void hal_pmu_mcpu_mtop4_perip_reset(hal_pmu_mcpu_perips_type_t perip,
                                           hal_pmu_perip_request_type_t request)
{
    uint16_t bit;

    switch (perip) {
        case HAL_PMU_MCPU_PERIP_EMMC:
            bit = PMU_MTOP4_EMMC_RST;
            break;
        case HAL_PMU_MCPU_PERIP_SDIO_HOST:
            bit = PMU_MTOP4_SDIO_HOST_RST;
            break;
        default:
            return;
    }

    if (request == HAL_PMU_PERIP_REQUEST_RESET) {
        reg16_clrbit(PMU_TOP_PERIPHERALS_MTOP4_L_REG, bit);
    } else if (request == HAL_PMU_PERIP_REQUEST_DERESET) {
        reg16_setbit(PMU_TOP_PERIPHERALS_MTOP4_L_REG, bit);
    }
}

void hal_pmu_mcpu_perip_reset(hal_pmu_mcpu_perips_type_t perip, hal_pmu_perip_request_type_t request)
{
    if (perip <= HAL_PMU_MCPU_PERIP_MDMA) {
        hal_pmu_mcpu_mtop0_perip_reset(perip, request);
    } else if (perip <= HAL_PMU_MCPU_PERIP_I2C_BUS_3) {
        hal_pmu_mcpu_mtop1_perip_reset(perip, request);
    } else if (perip <= HAL_PMU_MCPU_PERIP_QSPI2_1CS) {
        hal_pmu_mcpu_mtop2_perip_reset(perip, request);
    } else if (perip <= HAL_PMU_MCPU_PERIP_UART_H1) {
        hal_pmu_mcpu_mtop3_perip_reset(perip, request);
    } else if (perip <= HAL_PMU_MCPU_PERIP_SDIO_HOST) {
        hal_pmu_mcpu_mtop4_perip_reset(perip, request);
    } else {
        return;
    }
}

static hal_clocks_app_perips_clk_type_t hal_pmu_cmu_get_perips_clk_type(hal_pmu_mcpu_perips_type_t app_clk)
{
    if (app_clk >= HAL_PMU_MCPU_PERIP_MAX) {
        return HAL_CLOCKS_APP_BUTT;
    }

    hal_clocks_app_perips_clk_type_t clk_type[HAL_PMU_MCPU_PERIP_MAX] = {
        HAL_CLOCKS_APP_TIMER_CLK,       // !< HAL_PMU_MCPU_PERIP_TIMER
        HAL_CLOCKS_APP_BUTT,            // !< HAL_PMU_MCPU_PERIP_SMDMA
        HAL_CLOCKS_APP_BUTT,            // !< HAL_PMU_MCPU_PERIP_MDMA
        HAL_CLOCKS_APP_I2C0_CLK,        // !< HAL_PMU_MCPU_PERIP_I2C_BUS_0
        HAL_CLOCKS_APP_I2C1_CLK,        // !< HAL_PMU_MCPU_PERIP_I2C_BUS_1
        HAL_CLOCKS_APP_I2C2_CLK,        // !< HAL_PMU_MCPU_PERIP_I2C_BUS_2
        HAL_CLOCKS_APP_I2C3_CLK,        // !< HAL_PMU_MCPU_PERIP_I2C_BUS_3
        HAL_CLOCKS_APP_SPI0_M_CLK,      // !< HAL_PMU_MCPU_PERIP_SPI_MST
        HAL_CLOCKS_APP_SPI1_MS_CLK,     // !< HAL_PMU_MCPU_PERIP_SPI_MS_0
        HAL_CLOCKS_APP_SPI2_MS_CLK,     // !< HAL_PMU_MCPU_PERIP_SPI_MS_1
        HAL_CLOCKS_APP_QSPI_1CS_CLK,    // !< HAL_PMU_MCPU_PERIP_QSPI_BUS_1
        HAL_CLOCKS_APP_UART_H0_CLK,     // !< HAL_PMU_MCPU_PERIP_UART_H0
        HAL_CLOCKS_APP_UART_H1_CLK,     // !< HAL_PMU_MCPU_PERIP_UART_H1
        HAL_CLOCKS_APP_BUTT,            // !< HAL_PMU_MCPU_PERIP_EMMC
        HAL_CLOCKS_APP_BUTT,            // !< HAL_PMU_MCPU_PERIP_SDIO_HOST
        HAL_CLOCKS_APP_PWM_CLK,         // !< HAL_PMU_MCPU_PERIP_PWM
    };
    return clk_type[app_clk];
}

static void hal_pmu_mcu_common_perips_sleep(hal_pmu_mcpu_perips_type_t app_clk, switch_type_t clk_en)
{
    hal_clocks_app_perips_clk_type_t clk_type = hal_pmu_cmu_get_perips_clk_type(app_clk);
    if (clk_type >= HAL_CLOCKS_APP_BUTT) { return; }
    hal_clocks_app_perips_config(clk_type, clk_en);
}

static void hal_pmu_mcu_perips_clk_en(hal_pmu_mcpu_perips_type_t app_clk, hal_pmu_perip_request_type_t request)
{
    switch_type_t clk_en = TURN_OFF;
    if (request == HAL_PMU_PERIP_REQUEST_EXIT_LOW_POWER) {
        clk_en = TURN_ON;
    }

    switch (app_clk) {
        case HAL_PMU_MCPU_PERIP_SMDMA:
            hal_clocks_sub_normal_cfg(HAL_CLOCKS_SDMA_CLKEN_NOR, clk_en);
            return;
        case HAL_PMU_MCPU_PERIP_MDMA:
            hal_clocks_sub_normal_cfg(HAL_CLOCKS_MDMA_CLKEN_NOR, clk_en);
            return;
        case HAL_PMU_MCPU_PERIP_EMMC:
            hal_clocks_emmc_host_clken(clk_en);
            return;
        case HAL_PMU_MCPU_PERIP_SDIO_HOST:
            hal_clocks_sdio_host_clken(clk_en);
            return;
        default:
            break;
    }

    hal_pmu_mcu_common_perips_sleep(app_clk, clk_en);
}

void hal_pmu_mcpu_perip_config(hal_pmu_mcpu_perips_type_t perip, hal_pmu_perip_request_type_t request)
{
    // Multiple peripheral may call the funciton, need protect the register.
    uint32_t irq_sts = osal_irq_lock();
    switch (request) {
        case HAL_PMU_PERIP_REQUEST_RESET:
        // Peripheral reset/dereset request unified processing.
        case HAL_PMU_PERIP_REQUEST_DERESET:
            hal_pmu_mcpu_perip_reset(perip, request);
            break;
        case HAL_PMU_PERIP_REQUEST_ENTER_LOW_POWER:
            hal_pmu_mcu_perips_clk_en(perip, request);
            break;
        case HAL_PMU_PERIP_REQUEST_EXIT_LOW_POWER:
            // Make sure the peripheral not been reset.
            hal_pmu_mcpu_perip_reset(perip, HAL_PMU_PERIP_REQUEST_DERESET);
            hal_pmu_mcu_perips_clk_en(perip, request);
            while (hal_pmu_mcpu_perip_get_status(perip) != HAL_PMU_PERIP_STATUS_TURNING_ON) { }
            break;
        default:
            break;
    }
    osal_irq_restore(irq_sts);
}

hal_pmu_perip_status_type_t hal_pmu_maon_perip_get_status(hal_pmu_maon_perips_type_t perip)
{
    uint32_t addr = 0;
    uint16_t offset = 0;

    switch (perip) {
        case HAL_PMU_MAON_PERIP_GPIO0:
            addr = PMU_TOP_PERIPHERALS_ATOP0_H_REG;
            offset = PMU_ATOP0_MGPIO0_STS_OFFSET;
            break;
        case HAL_PMU_MAON_PERIP_GPIO1:
            addr = PMU_TOP_PERIPHERALS_ATOP0_H_REG;
            offset = PMU_ATOP0_MGPIO1_STS_OFFSET;
            break;
        case HAL_PMU_MAON_PERIP_GPIO2:
            addr = PMU_TOP_PERIPHERALS_ATOP0_H_REG;
            offset = PMU_ATOP0_MGPIO2_STS_OFFSET;
            break;
        case HAL_PMU_MAON_PERIP_GPIO3:
            addr = PMU_TOP_PERIPHERALS_ATOP0_H_REG;
            offset = PMU_ATOP0_MGPIO3_STS_OFFSET;
            break;
        case HAL_PMU_MAON_PERIP_GPIO4:
            addr = PMU_TOP_PERIPHERALS_ATOP1_H_REG;
            offset = PMU_ATOP1_MGPIO4_STS_OFFSET;
            break;
        case HAL_PMU_MAON_PERIP_RTC:
            addr = PMU_TOP_PERIPHERALS_ATOP1_H_REG;
            offset = PMU_ATOP1_MRTC_STS_OFFSET;
            break;
        case HAL_PMU_MAON_PERIP_CWDT:
            addr = PMU_TOP_PERIPHERALS_ATOP1_H_REG;
            offset = PMU_ATOP1_CWDT_STS_OFFSET;
            break;
        default:
            break;
    }

    if (addr != 0) {
        return (hal_pmu_perip_status_type_t)reg16_getbits((void *)((uintptr_t)addr), offset, PMU_PERIPHERAL_STS_BITS);
    } else {
        return HAL_PMU_PERIP_STATUS_CLOCK_OFF;
    }
}

static void hal_pmu_maon_perip_reset(hal_pmu_maon_perips_type_t perip, hal_pmu_perip_request_type_t request)
{
    uint32_t addr = 0;
    uint16_t bit = 0;

    switch (perip) {
        case HAL_PMU_MAON_PERIP_GPIO0:
            addr = PMU_TOP_PERIPHERALS_ATOP0_L_REG;
            bit = PMU_ATOP0_MGPIO0_RST;
            break;
        case HAL_PMU_MAON_PERIP_GPIO1:
            addr = PMU_TOP_PERIPHERALS_ATOP0_L_REG;
            bit = PMU_ATOP0_MGPIO1_RST;
            break;
        case HAL_PMU_MAON_PERIP_GPIO2:
            addr = PMU_TOP_PERIPHERALS_ATOP0_L_REG;
            bit = PMU_ATOP0_MGPIO2_RST;
            break;
        case HAL_PMU_MAON_PERIP_GPIO3:
            addr = PMU_TOP_PERIPHERALS_ATOP0_L_REG;
            bit = PMU_ATOP0_MGPIO3_RST;
            break;
        case HAL_PMU_MAON_PERIP_GPIO4:
            addr = PMU_TOP_PERIPHERALS_ATOP1_L_REG;
            bit = PMU_ATOP1_MGPIO4_RST;
            break;
        case HAL_PMU_MAON_PERIP_RTC:
            addr = PMU_TOP_PERIPHERALS_ATOP1_L_REG;
            bit = PMU_ATOP1_MRTC_RST;
            break;
        case HAL_PMU_MAON_PERIP_CWDT:
            addr = PMU_TOP_PERIPHERALS_ATOP1_L_REG;
            bit = PMU_ATOP1_CWDT_RST;
            break;
        default:
            break;
    }

    if (addr != 0) {
        if (request == HAL_PMU_PERIP_REQUEST_RESET) {
            reg16_clrbit((void *)((uintptr_t)addr), bit);
        } else if (request == HAL_PMU_PERIP_REQUEST_DERESET) {
            reg16_setbit((void *)((uintptr_t)addr), bit);
        }
    }
}

static void hal_pmu_maon_perip_sleep(hal_pmu_maon_perips_type_t perip, hal_pmu_perip_request_type_t request)
{
    uint32_t addr = 0;
    uint16_t bit = 0;

    switch (perip) {
        case HAL_PMU_MAON_PERIP_GPIO0:
            addr = PMU_TOP_PERIPHERALS_ATOP0_L_REG;
            bit = PMU_ATOP0_MGPIO0_REQUEST_LP;
            break;
        case HAL_PMU_MAON_PERIP_GPIO1:
            addr = PMU_TOP_PERIPHERALS_ATOP0_L_REG;
            bit = PMU_ATOP0_MGPIO1_REQUEST_LP;
            break;
        case HAL_PMU_MAON_PERIP_GPIO2:
            addr = PMU_TOP_PERIPHERALS_ATOP0_L_REG;
            bit = PMU_ATOP0_MGPIO2_REQUEST_LP;
            break;
        case HAL_PMU_MAON_PERIP_GPIO3:
            addr = PMU_TOP_PERIPHERALS_ATOP0_L_REG;
            bit = PMU_ATOP0_MGPIO3_REQUEST_LP;
            break;
        case HAL_PMU_MAON_PERIP_GPIO4:
            addr = PMU_TOP_PERIPHERALS_ATOP1_L_REG;
            bit = PMU_ATOP1_MGPIO4_REQUEST_LP;
            break;
        case HAL_PMU_MAON_PERIP_RTC:
            addr = PMU_TOP_PERIPHERALS_ATOP1_L_REG;
            bit = PMU_ATOP1_MRTC_REQUEST_LP;
            break;
        case HAL_PMU_MAON_PERIP_CWDT:
            addr = PMU_TOP_PERIPHERALS_ATOP1_L_REG;
            bit = PMU_ATOP1_CWDT_REQUEST_LP;
            break;
        default:
            return;
    }
    if (request == HAL_PMU_PERIP_REQUEST_EXIT_LOW_POWER) {
        reg16_setbit((void *)((uintptr_t)addr), bit);
    } else if (request == HAL_PMU_PERIP_REQUEST_ENTER_LOW_POWER) {
        reg16_clrbit((void *)((uintptr_t)addr), bit);
    }
}

void hal_pmu_maon_perip_config(hal_pmu_maon_perips_type_t perip, hal_pmu_perip_request_type_t request)
{
    // Multiple peripheral may call the funciton, need protect the register.
    uint32_t irq_sts = osal_irq_lock();
    switch (request) {
        case HAL_PMU_PERIP_REQUEST_RESET:
        // Peripheral reset/dereset request unified processing.
        case HAL_PMU_PERIP_REQUEST_DERESET:
            hal_pmu_maon_perip_reset(perip, request);
            break;
        case HAL_PMU_PERIP_REQUEST_ENTER_LOW_POWER:
            hal_pmu_maon_perip_sleep(perip, request);
            break;
        case HAL_PMU_PERIP_REQUEST_EXIT_LOW_POWER:
            // Make sure the peripheral not been reset.
            hal_pmu_maon_perip_reset(perip, HAL_PMU_PERIP_REQUEST_DERESET);
            hal_pmu_maon_perip_sleep(perip, request);
            while (hal_pmu_maon_perip_get_status(perip) != HAL_PMU_PERIP_STATUS_TURNING_ON) { }
            break;
        default:
            break;
    }
    osal_irq_restore(irq_sts);
}

hal_pmu_perip_status_type_t hal_pmu_mem_perip_get_status(hal_pmu_mem_perips_type_t perip)
{
    uint32_t addr = 0;
    uint16_t offset = 0;

    switch (perip) {
        case HAL_PMU_MEM_PERIP_QSPI0_2CS:
            addr = PMU_MEM_SUB_PERIPHERALS_STATUS_REG;
            offset = PMU_MEM_QSPI0_2CS_STS_OFFSET;
            break;
        case HAL_PMU_MEM_PERIP_QSPI1_2CS:
            addr = PMU_MEM_SUB_PERIPHERALS_STATUS_REG;
            offset = PMU_MEM_QSPI1_2CS_STS_OFFSET;
            break;
        case HAL_PMU_MEM_PERIP_OPI:
            addr = PMU_MEM_SUB_PERIPHERALS_STATUS_REG;
            offset = PMU_MEM_OPI_STS_OFFSET;
            break;
        default:
            break;
    }
    if (addr != 0) {
        return (hal_pmu_perip_status_type_t)reg16_getbits((void *)((uintptr_t)addr), offset, PMU_PERIPHERAL_STS_BITS);
    } else {
        return HAL_PMU_PERIP_STATUS_CLOCK_OFF;
    }
}

static void hal_pmu_mem_perip_reset(hal_pmu_mem_perips_type_t perip, hal_pmu_perip_request_type_t request)
{
    uint32_t addr = 0;
    uint16_t bit = 0;

    switch (perip) {
        case HAL_PMU_MEM_PERIP_OPI:
            addr = PMU_MEM_SUB_PERIPHERALS_CONFIG_REG;
            bit = PMU_MEM_OPI_RST;
            break;
        case HAL_PMU_MEM_PERIP_QSPI1_2CS:
            addr = PMU_MEM_SUB_PERIPHERALS_CONFIG_REG;
            bit = PMU_MEM_QSPI1_2CS_RST;
            break;
        case HAL_PMU_MEM_PERIP_QSPI0_2CS:
            addr = PMU_MEM_SUB_PERIPHERALS_CONFIG_REG;
            bit = PMU_MEM_QSPI0_2CS_RST;
            break;
        default:
            break;
    }

    if (addr != 0) {
        if (request == HAL_PMU_PERIP_REQUEST_RESET) {
            reg16_clrbit((void *)((uintptr_t)addr), bit);
        } else if (request == HAL_PMU_PERIP_REQUEST_DERESET) {
            reg16_setbit((void *)((uintptr_t)addr), bit);
        }
    }
}

static void hal_pmu_mem_perip_sleep(hal_pmu_mem_perips_type_t perip, hal_pmu_perip_request_type_t request)
{
    switch_type_t clken = TURN_OFF;
    if (request == HAL_PMU_PERIP_REQUEST_EXIT_LOW_POWER) {
        clken = TURN_ON;
    }

    switch (perip) {
        case HAL_PMU_MEM_PERIP_QSPI0_2CS:
            hal_clocks_mem_perips_config(HAL_CLOCKS_MEM_QSPI0_CLKEN, clken);
            break;
        case HAL_PMU_MEM_PERIP_OPI:
            hal_clocks_mem_perips_config(HAL_CLOCKS_MEM_OPI_CLKEN, clken);
            break;
        default:
            break;
    }
}

void hal_pmu_mem_perip_config(hal_pmu_mem_perips_type_t perip, hal_pmu_perip_request_type_t request)
{
    // Multiple peripheral may call the funciton, need protect the register.
    uint32_t irq_sts = osal_irq_lock();
    switch (request) {
        case HAL_PMU_PERIP_REQUEST_RESET:
        // Peripheral reset/dereset request unified processing.
        case HAL_PMU_PERIP_REQUEST_DERESET:
            hal_pmu_mem_perip_reset(perip, request);
            break;
        case HAL_PMU_PERIP_REQUEST_ENTER_LOW_POWER:
            hal_pmu_mem_perip_sleep(perip, request);
            break;
        case HAL_PMU_PERIP_REQUEST_EXIT_LOW_POWER:
            // Make sure the peripheral not been reset.
            hal_pmu_mem_perip_reset(perip, HAL_PMU_PERIP_REQUEST_DERESET);
            hal_pmu_mem_perip_sleep(perip, request);
            while (hal_pmu_mem_perip_get_status(perip) != HAL_PMU_PERIP_STATUS_TURNING_ON) { }
            break;
        default:
            break;
    }
    osal_irq_restore(irq_sts);
}

hal_pmu_perip_status_type_t hal_pmu_daon_perip_get_status(void)
{
    uint32_t addr = PMU_TOP_PERIPHERALS_ATOP3_H_REG;
    uint16_t offset = PMU_ATOP3_DGPIO_STS_OFFSET;

    return (hal_pmu_perip_status_type_t)reg16_getbits((void *)((uintptr_t)addr), offset, PMU_PERIPHERAL_STS_BITS);
}

static void hal_pmu_daon_perip_reset(hal_pmu_perip_request_type_t request)
{
    uint32_t addr = PMU_TOP_PERIPHERALS_ATOP3_L_REG;
    uint16_t bit = PMU_ATOP3_DGPIO_RST;

    if (request == HAL_PMU_PERIP_REQUEST_RESET) {
        reg16_clrbit((void *)((uintptr_t)addr), bit);
    } else if (request == HAL_PMU_PERIP_REQUEST_DERESET) {
        reg16_setbit((void *)((uintptr_t)addr), bit);
    }
}

static void hal_pmu_daon_perip_sleep(hal_pmu_perip_request_type_t request)
{
    uint32_t addr = PMU_TOP_PERIPHERALS_ATOP3_L_REG;
    uint16_t bit = PMU_ATOP3_DGPIO_REQUEST_LP;

    if (request == HAL_PMU_PERIP_REQUEST_EXIT_LOW_POWER) {
        reg16_clrbit((void *)((uintptr_t)addr), bit);
    } else if (request == HAL_PMU_PERIP_REQUEST_ENTER_LOW_POWER) {
        reg16_setbit((void *)((uintptr_t)addr), bit);
    }
}

void hal_pmu_daon_perip_config(hal_pmu_perip_request_type_t request)
{
    // Multiple peripheral may call the funciton, need protect the register.
    uint32_t irq_sts = osal_irq_lock();
    switch (request) {
        case HAL_PMU_PERIP_REQUEST_RESET:
        // Peripheral reset/dereset request unified processing.
        case HAL_PMU_PERIP_REQUEST_DERESET:
            hal_pmu_daon_perip_reset(request);
            break;
        case HAL_PMU_PERIP_REQUEST_ENTER_LOW_POWER:
            hal_pmu_daon_perip_sleep(request);
            break;
        case HAL_PMU_PERIP_REQUEST_EXIT_LOW_POWER:
            // Make sure the peripheral not been reset.
            hal_pmu_daon_perip_reset(HAL_PMU_PERIP_REQUEST_DERESET);
            hal_pmu_daon_perip_sleep(request);
            while (hal_pmu_daon_perip_get_status() != HAL_PMU_PERIP_STATUS_TURNING_ON) { }
            break;
        default:
            break;
    }
    osal_irq_restore(irq_sts);
}

void hal_pmu_mgpio_low_power_config(switch_type_t on)
{
    if (on == TURN_ON) {
        hal_pmu_maon_perip_sleep(HAL_PMU_MAON_PERIP_GPIO0, HAL_PMU_PERIP_REQUEST_EXIT_LOW_POWER);
        hal_pmu_maon_perip_sleep(HAL_PMU_MAON_PERIP_GPIO1, HAL_PMU_PERIP_REQUEST_EXIT_LOW_POWER);
        hal_pmu_maon_perip_sleep(HAL_PMU_MAON_PERIP_GPIO2, HAL_PMU_PERIP_REQUEST_EXIT_LOW_POWER);
        hal_pmu_maon_perip_sleep(HAL_PMU_MAON_PERIP_GPIO3, HAL_PMU_PERIP_REQUEST_EXIT_LOW_POWER);
        hal_pmu_maon_perip_sleep(HAL_PMU_MAON_PERIP_GPIO4, HAL_PMU_PERIP_REQUEST_EXIT_LOW_POWER);
    } else {
        hal_pmu_maon_perip_sleep(HAL_PMU_MAON_PERIP_GPIO0, HAL_PMU_PERIP_REQUEST_ENTER_LOW_POWER);
        hal_pmu_maon_perip_sleep(HAL_PMU_MAON_PERIP_GPIO1, HAL_PMU_PERIP_REQUEST_ENTER_LOW_POWER);
        hal_pmu_maon_perip_sleep(HAL_PMU_MAON_PERIP_GPIO2, HAL_PMU_PERIP_REQUEST_ENTER_LOW_POWER);
        hal_pmu_maon_perip_sleep(HAL_PMU_MAON_PERIP_GPIO3, HAL_PMU_PERIP_REQUEST_ENTER_LOW_POWER);
        hal_pmu_maon_perip_sleep(HAL_PMU_MAON_PERIP_GPIO4, HAL_PMU_PERIP_REQUEST_ENTER_LOW_POWER);
    }
}

void hal_pmu_get_perip_state(void)
{
    g_clocks_state_in_abnormal.m_top0_l = readw(PMU_TOP_PERIPHERALS_MTOP0_L_REG);
    g_clocks_state_in_abnormal.m_top0_h = readw(PMU_TOP_PERIPHERALS_MTOP0_H_REG);
    g_clocks_state_in_abnormal.m_top1_l = readw(PMU_TOP_PERIPHERALS_MTOP1_L_REG);
    g_clocks_state_in_abnormal.m_top1_h = readw(PMU_TOP_PERIPHERALS_MTOP1_H_REG);
    g_clocks_state_in_abnormal.m_top2_l = readw(PMU_TOP_PERIPHERALS_MTOP2_L_REG);
    g_clocks_state_in_abnormal.m_top2_h = readw(PMU_TOP_PERIPHERALS_MTOP2_H_REG);
    g_clocks_state_in_abnormal.m_top3_l = readw(PMU_TOP_PERIPHERALS_MTOP3_L_REG);
    g_clocks_state_in_abnormal.m_top3_h = readw(PMU_TOP_PERIPHERALS_MTOP3_H_REG);
    g_clocks_state_in_abnormal.m_top4_l = readw(PMU_TOP_PERIPHERALS_MTOP4_L_REG);
    g_clocks_state_in_abnormal.m_top4_h = readw(PMU_TOP_PERIPHERALS_MTOP4_H_REG);
    g_clocks_state_in_abnormal.m_top5_l = readw(PMU_TOP_PERIPHERALS_MTOP5_L_REG);
    g_clocks_state_in_abnormal.m_top5_h = readw(PMU_TOP_PERIPHERALS_MTOP5_H_REG);
    g_clocks_state_in_abnormal.m_clocken0 = readw(CMU_CLOCKS_M_CLKEN0_REG);
    g_clocks_state_in_abnormal.m_clocken1 = readw(CMU_CLOCKS_M_CLKEN1_REG);
    g_clocks_state_in_abnormal.m_clocken2 = readw(CMU_CLOCKS_M_CLKEN2_REG);
    g_clocks_state_in_abnormal.m_glb_clocken = readw(CMU_CLOCKS_GLB_CLKEN_REG);
    g_clocks_state_in_abnormal.m_perp_ls_cr = readw(CMU_CLOCKS_MCU_PERP_LS_CR_REG);
}