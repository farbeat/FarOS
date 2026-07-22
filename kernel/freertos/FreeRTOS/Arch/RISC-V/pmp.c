/*
 * Copyright (c) CompanyNameMagicTag 2021-2022. All rights reserved.
 */

#include "FreeRTOS.h"
#include "print.h"
#include "hwi.h"
#include "encoding.h"
#include "print.h"
#include "pmp.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

static inline uint32_t Ctz32(uint32_t val)
{
    if (val == 0)
    {
        return 32; /* The number of preamble 0 of zero is 32. */
    }

    return __builtin_ctz(val);
}

/**
 * Related instructions of registers are specified in register_config.h and they are not explained here.
 */
#define PMP_ADDR_ALIGN_SIZE  4
#define PMP_TOR_MIN_SIZE     4
#define PMP_NAPOT_MIN_SIZE   8

#define GET_PMPREG_NUM(pmpIdx) ((uint8_t)((uint8_t)(pmpIdx) & 0x3) << 3)
#define CLEAR_PMPCFG_BIT(pmpIdx, cfgValue) ((uint32_t)(cfgValue) & \
    ~((0xff << GET_PMPREG_NUM(pmpIdx)) & UINT32_CUT_MASK))
#define GET_PMPCFG_VALID_VALUE(pmpIdx, cfgOldValue, cfgValue) (((uint32_t)(cfgValue) << GET_PMPREG_NUM(pmpIdx)) | \
    CLEAR_PMPCFG_BIT(pmpIdx, cfgOldValue))
#define GET_PMPCFG_NUM(pmpIdx) ((uint8_t)(pmpIdx) >> 2)

#define GET_SECTL_PMP_NUM(secIndx) ((uint8_t)(secIndx) << 1)
#define CLEAR_SECTL_BIT(secIdx) (~((0x3U << GET_SECTL_PMP_NUM(secIdx)) & UINT32_CUT_MASK))

#define GET_MEMATTRREG_NUM(memAttrIndx) ((uint8_t)(memAttrIndx) >> 3)
#define GET_MEMATTR_PMP_NUM(memAttrIndx) (((uint8_t)(memAttrIndx) & 0x7) << 2)
#define CLEAR_MEMATTR_BIT(memAttrIndx) (~((0xfU << GET_MEMATTR_PMP_NUM(memAttrIndx)) & UINT32_CUT_MASK))

#define GET_PMP_NAPOT_SIZE(value) (((uint32_t)(value) >> 1) - 1)
#define SET_PMP_PROT_ADDR(value) ((uint32_t)(value) >> 2)
#define GET_PMP_PROT_ADDR(value) (((uint32_t)(value) << 2) & UINT32_CUT_MASK)

#define GET_NAPOT_PMP_REG_PROT_SIZE(value) (1U << (Ctz32((value) + 1) + 3))
#define IS_ALIGN(addr, boundary) ((uint32_t)(addr) & ((uint32_t)(boundary) - 1))

uint64_t g_pmpMaxAddr = PMP_MAX_ADDRESS;
uint64_t g_pmpMinAddr = PMP_MIN_ADDRESS;
static BaseType_t g_pmpTopFlag = pdFALSE;

/*****************************************************************************
 Function   : PmpCfgRead
 Description: Read pmp config register.
 Input      : number -- pmp config serial number.
 Return     : Pmp config register value.
 *****************************************************************************/
static uint8_t PmpCfgRead(uint8_t number)
{
    uint32_t pmpCfgRegVal = 0;

    switch (GET_PMPCFG_NUM(number))
    {
        case 0: /* PMP memory R/W/X property sets register number register 0 */
            pmpCfgRegVal = read_csr(pmpcfg0);
            break;
        case 1: /* PMP memory R/W/X property sets register number register 1 */
            pmpCfgRegVal = read_csr(pmpcfg1);
            break;
        case 2: /* PMP memory R/W/X property sets register number register 2 */
            pmpCfgRegVal = read_csr(pmpcfg2);
            break;
        case 3: /* PMP memory R/W/X property sets register number register 3 */
            pmpCfgRegVal = read_csr(pmpcfg3);
            break;
        default:
            break;
    }
    return (uint8_t)(pmpCfgRegVal >> GET_PMPREG_NUM(number)) & UINT8_CUT_MASK;
}

static uint32_t PmpIsLocked(uint8_t number)
{
    uint8_t pmpCfg;

    pmpCfg = PmpCfgRead(number);
    if (pmpCfg & PMP_RGN_LOCK)
    {
        return pdTRUE;
    }
    return pdFALSE;
}

static uint32_t PmpAddrRead(uint8_t pmpAddrIdx)
{
    switch (pmpAddrIdx)
    {
        case 0: /* Pmpaddr registers numbered 0 */
            return read_csr(pmpaddr0);
        case 1: /* Pmpaddr registers numbered 1 */
            return read_csr(pmpaddr1);
        case 2: /* Pmpaddr registers numbered 2 */
            return read_csr(pmpaddr2);
        case 3: /* Pmpaddr registers numbered 3 */
            return read_csr(pmpaddr3);
        case 4: /* Pmpaddr registers numbered 4 */
            return read_csr(pmpaddr4);
        case 5: /* Pmpaddr registers numbered 5 */
            return read_csr(pmpaddr5);
        case 6: /* Pmpaddr registers numbered 6 */
            return read_csr(pmpaddr6);
        case 7: /* Pmpaddr registers numbered 7 */
            return read_csr(pmpaddr7);
        case 8: /* Pmpaddr registers numbered 8 */
            return read_csr(pmpaddr8);
        case 9: /* Pmpaddr registers numbered 9 */
            return read_csr(pmpaddr9);
        case 10: /* Pmpaddr registers numbered 10 */
            return read_csr(pmpaddr10);
        case 11: /* Pmpaddr registers numbered 11 */
            return read_csr(pmpaddr11);
        case 12: /* Pmpaddr registers numbered 12 */
            return read_csr(pmpaddr12);
        case 13: /* Pmpaddr registers numbered 13 */
            return read_csr(pmpaddr13);
        case 14: /* Pmpaddr registers numbered 14 */
            return read_csr(pmpaddr14);
        case 15: /* Pmpaddr registers numbered 15 */
            return read_csr(pmpaddr15);
        default:
            return 0;
    }
}

static void PmpAddrWrite(uint8_t pmpAddrIdx, uint32_t pmpAddrVal)
{
    switch (pmpAddrIdx)
    {
        case 0: /* Pmpaddr registers numbered 0 */
            write_csr(pmpaddr0, pmpAddrVal);
            break;
        case 1: /* Pmpaddr registers numbered 1 */
            write_csr(pmpaddr1, pmpAddrVal);
            break;
        case 2: /* Pmpaddr registers numbered 2 */
            write_csr(pmpaddr2, pmpAddrVal);
            break;
        case 3: /* Pmpaddr registers numbered 3 */
            write_csr(pmpaddr3, pmpAddrVal);
            break;
        case 4: /* Pmpaddr registers numbered 4 */
            write_csr(pmpaddr4, pmpAddrVal);
            break;
        case 5: /* Pmpaddr registers numbered 5 */
            write_csr(pmpaddr5, pmpAddrVal);
            break;
        case 6: /* Pmpaddr registers numbered 6 */
            write_csr(pmpaddr6, pmpAddrVal);
            break;
        case 7: /* Pmpaddr registers numbered 7 */
            write_csr(pmpaddr7, pmpAddrVal);
            break;
        case 8: /* Pmpaddr registers numbered 8 */
            write_csr(pmpaddr8, pmpAddrVal);
            break;
        case 9: /* Pmpaddr registers numbered 9 */
            write_csr(pmpaddr9, pmpAddrVal);
            break;
        case 10: /* Pmpaddr registers numbered 10 */
            write_csr(pmpaddr10, pmpAddrVal);
            break;
        case 11: /* Pmpaddr registers numbered 11 */
            write_csr(pmpaddr11, pmpAddrVal);
            break;
        case 12: /* Pmpaddr registers numbered 12 */
            write_csr(pmpaddr12, pmpAddrVal);
            break;
        case 13: /* Pmpaddr registers numbered 13 */
            write_csr(pmpaddr13, pmpAddrVal);
            break;
        case 14: /* Pmpaddr registers numbered 14 */
            write_csr(pmpaddr14, pmpAddrVal);
            break;
        case 15: /* Pmpaddr registers numbered 15 */
            write_csr(pmpaddr15, pmpAddrVal);
            break;
        default:
            break;
    }
}

static uint32_t PmpTorLowerBoundRead(uint8_t pmpAddrIdx)
{
    uint8_t  pmpAddrMatch;
    uint32_t pmpPreAddrVal;
    uint32_t pmpPreSize;
    uint32_t pmpPreBaseAddr;
    uint32_t napotSize;

    if (pmpAddrIdx == 0)
    {
        return 0;
    }

    pmpAddrMatch = PmpCfgRead(pmpAddrIdx - 1) & PMP_RGN_ADDR_MATCH_MASK;
    switch (pmpAddrMatch)
    {
        case PMP_RGN_ADDR_MATCH_OFF:
            /* FALLTHROUGH */
        case PMP_RGN_ADDR_MATCH_TOR:
            pmpPreAddrVal = PmpAddrRead(pmpAddrIdx - 1);
            return GET_PMP_PROT_ADDR(pmpPreAddrVal);
        case PMP_RGN_ADDR_MATCH_NAPOT:
        {
            pmpPreAddrVal = PmpAddrRead(pmpAddrIdx - 1);
            PRINTK(ePrintkErr, "pre region addr value = 0x%x\n", pmpPreAddrVal);
            pmpPreSize = GET_NAPOT_PMP_REG_PROT_SIZE(pmpPreAddrVal);
            PRINTK(ePrintkErr, "pre region size = 0x%x\n", pmpPreSize);
            napotSize = GET_PMP_NAPOT_SIZE(pmpPreSize);    /* get the pre-region size */
            pmpPreBaseAddr = GET_PMP_PROT_ADDR(pmpPreAddrVal) & (~napotSize);    /* get the pre-region base address */
            PRINTK(ePrintkErr, "pre region base address = 0x%x\n", pmpPreBaseAddr);
            /* get the pre-region end address, i.e. the lower bound of this TOR region */
            return (pmpPreBaseAddr + pmpPreSize);
        }
        default:
            return 0;
    }
}

static uint32_t PmpCheckNextPmpEntry(const PMP_ENTRY_INFO *pmpInfo)
{
    uint8_t pmpcfgMode = PmpCfgRead(pmpInfo->ucNumber) & PMP_RGN_ADDR_MATCH_MASK;
    if ((pmpcfgMode == PMP_RGN_ADDR_MATCH_OFF) || (pmpcfgMode == PMP_RGN_ADDR_MATCH_TOR))
    {
        if (pmpInfo->ucAddressMatch == PMP_RGN_ADDR_MATCH_NAPOT)
        {
            return errPMP_TOR_INVALID_MATCH_MODE;
        }
    }
    else if (pmpcfgMode == PMP_RGN_ADDR_MATCH_NAPOT)
    {
        if (pmpInfo->ucAddressMatch != PMP_RGN_ADDR_MATCH_NAPOT)
        {
            return errPMP_NAPOT_INVALID_MATCH_MODE;
        }
    }

    return pdPASS;
}

static uint32_t PmpBaseAddrAndRegionSizeCheck(uint8_t number,
                                              uint8_t addrMatch,
                                              const char baseAddr,
                                              uint32_t regionSize )
{
    uint32_t prevAddr;
    /* Base address is not in the range of [PMP_MIN_ADDRESS, PMP_MAX_ADDRESS] */
    if ((baseAddr < g_pmpMinAddr) || (baseAddr > g_pmpMaxAddr))
    {
        return errPMP_BASE_ADDRESS_NOT_IN_RANGE;
    }

    /* the PMP region start address and end address must be 4 aligned. */
    if (IS_ALIGN(baseAddr, PMP_ADDR_ALIGN_SIZE) != 0)
    {
        return errPMP_INVALID_BASE_ADDRESS;
    }

    if (addrMatch == PMP_RGN_ADDR_MATCH_NAPOT)
    {
        if ((regionSize < PMP_NAPOT_MIN_SIZE) && ((baseAddr + regionSize) > g_pmpMaxAddr))
        {
            return errPMP_NAPOT_ENCODING_NON_MATCHED;
        }

        /* check the naturally aligned power-of-2 regions (NAPOT) size */
        if (((regionSize - 1) & regionSize) != 0)
        {
            return errPMP_INVALID_CAPACITY;
        }

        /* the NAPOT type and size should be matched in NAPOT range encoding. */
        if (((regionSize - 1) & baseAddr) != 0)
        {
            return errPMP_NAPOT_ENCODING_NON_MATCHED;
        }
    }

    /* check TOR base address equals to the pre-region end address */
    if (addrMatch == PMP_RGN_ADDR_MATCH_TOR)
    {
        prevAddr = PmpTorLowerBoundRead(number);
        if (baseAddr < (prevAddr + PMP_TOR_MIN_SIZE))
        {
            return errPMP_TOR_LOWER_BOUND_NONMATCHED;
        }
    }

    return pdPASS;
}

/*****************************************************************************
 Function   : PmpCheckParame
 Description: check parame
 Input      : pmpInfo   -- PMP info
 Return     : Error Information or pdPASS
 *****************************************************************************/
static uint32_t PmpAttrAndSectlCheck(PMP_ENTRY_INFO *pmpInfo)
{
    if ((pmpInfo->memAttr != MEM_ATTR_DEV_NON_BUF) && (pmpInfo->memAttr != MEM_ATTR_NORM_NON_CA_NON_BUF) &&
        (pmpInfo->memAttr != MEM_ATTR_NORM_NON_CA_BUF) && (pmpInfo->memAttr != MEM_ATTR_WB_RD_ALLOC) &&
        (pmpInfo->memAttr != MEM_ATTR_WB_WR_ALLOC) && (pmpInfo->memAttr != MEM_ATTR_WB_RW_ALLOC))
    {
        return errPMP_ATTR_INVALID;
    }

    if ((pmpInfo->seCtl != SEC_CONTRLO_SECURE_NOMMU) && (pmpInfo->seCtl != SEC_CONTRLO_NOSECURE_NOMMU) &&
        (pmpInfo->seCtl != SEC_CONTRLO_SECURE_MMU) && (pmpInfo->seCtl != SEC_CONTRLO_NOSECURE_MMU))
    {
        return errPMP_SECTL_INVALID;
    }
    return pdPASS;
}

static uint32_t PmpCheckParame(PMP_ENTRY_INFO *pmpInfo)
{
    uint32_t ret;
    uint8_t  pmpcfgNext;

    if (pmpInfo == NULL)
    {
        return errPMP_PTR_NULL;
    }

    if (pmpInfo->ucAddressMatch == PMP_RGN_ADDR_MATCH_NA4)
    {
        return errPMP_INVALID_MATCH;
    }

    /* Number exceeds the maximum value supported by PMO */
    if (pmpInfo->ucNumber >= PMP_MAX_SUPPORT)
    {
        return errPMP_INVALID_NUMBER;
    }

    /* Check the entry whether locked or not */
    if (PmpIsLocked(pmpInfo->ucNumber) == pdTRUE)
    {
        return errPMP_ENTRY_IS_LOCKED;
    }

    g_pmpTopFlag = pdFALSE;
    /* If address matching mode equals to TOR, check the next PMP entry */
    if ((pmpInfo->ucNumber + 1) < PMP_MAX_SUPPORT)
    {
        pmpcfgNext = PmpCfgRead(pmpInfo->ucNumber + 1);
        if ((pmpcfgNext & PMP_RGN_LOCK) && ((pmpcfgNext & PMP_RGN_ADDR_MATCH_MASK) == PMP_RGN_ADDR_MATCH_TOR))
        {
            ret = PmpCheckNextPmpEntry(pmpInfo);
            if (ret != pdPASS)
            {
                return ret;
            }
            g_pmpTopFlag = pdTRUE;
        }
    }

    if (g_pmpTopFlag != pdTRUE)
    {
        ret = PmpBaseAddrAndRegionSizeCheck(pmpInfo->ucNumber, pmpInfo->ucAddressMatch,
                                            pmpInfo->uwBaseAddress, pmpInfo->uwRegionSize);
        if (ret != pdPASS)
        {
            return ret;
        }
    }

    if (pmpInfo->ucAddressMatch != PMP_RGN_ADDR_MATCH_OFF)
    {
        return PmpAttrAndSectlCheck(pmpInfo);
    }
    else
    {
        pmpInfo->memAttr = MEM_ATTR_DEV_NON_BUF;
        pmpInfo->seCtl = SEC_CONTRLO_SECURE_NOMMU;
    }
    return pdPASS;
}

static uint8_t PmpAccpGet(PmpAccInfo accPermission)
{
    uint8_t pmpCfgAcc = 0;

    if (accPermission.readAcc == E_MEM_RD_ACC_RD)
    {
        pmpCfgAcc |= (1 << PMPCFG_R_BIT);
    }

    if (accPermission.writeAcc == E_MEM_WR_ACC_WR)
    {
        pmpCfgAcc |= (1 << PMPCFG_W_BIT);
    }

    if (accPermission.excuteAcc == E_MEM_EX_ACC_EX)
    {
        pmpCfgAcc |= (1 << PMPCFG_X_BIT);
    }
    return pmpCfgAcc;
}

static void PmpCfgWrite(uint8_t pmpCfgIdx, uint8_t pmpCfgVal)
{
    uint32_t cfgVal;

    switch (GET_PMPCFG_NUM(pmpCfgIdx))
    {
        case 0: /* PMP memory R/W/X property sets register number register 0 */
            cfgVal = read_csr(pmpcfg0);
            cfgVal = GET_PMPCFG_VALID_VALUE(pmpCfgIdx, cfgVal, pmpCfgVal);
            write_csr(pmpcfg0, cfgVal);
            break;
        case 1: /* PMP memory R/W/X property sets register number register 1 */
            cfgVal = read_csr(pmpcfg1);
            cfgVal = GET_PMPCFG_VALID_VALUE(pmpCfgIdx, cfgVal, pmpCfgVal);
            write_csr(pmpcfg1, cfgVal);
            break;
        case 2: /* PMP memory R/W/X property sets register number register 2 */
            cfgVal = read_csr(pmpcfg2);
            cfgVal = GET_PMPCFG_VALID_VALUE(pmpCfgIdx, cfgVal, pmpCfgVal);
            write_csr(pmpcfg2, cfgVal);
            break;
        case 3: /* PMP memory R/W/X property sets register number register 3 */
            cfgVal = read_csr(pmpcfg3);
            cfgVal = GET_PMPCFG_VALID_VALUE(pmpCfgIdx, cfgVal, pmpCfgVal);
            write_csr(pmpcfg3, cfgVal);
            break;
        default:
            break;
    }
}

/*****************************************************************************
 Function    : PmpNapotAddrGet
 Description : get the pmp address (NAPOT) of the physical memory protection entry
 Input       : base -- base addr
 Input       : size -- protection size
 Output      : pmp address (NAPOT)
 Return      : OS_SUCCESS
 *****************************************************************************/
static uint32_t PmpNapotAddrGet(uint32_t base,  uint32_t size)
{
    return (uint32_t)SET_PMP_PROT_ADDR(base + GET_PMP_NAPOT_SIZE(size));
}

static void PmpMemattrWrite(uint8_t pmpMemAttrIdx, PmpMemAttrInfo pmpMemAttrVal)
{
    uint32_t memAttrVal;

    if (GET_MEMATTRREG_NUM(pmpMemAttrIdx) == 0)
    {
        memAttrVal  = read_custom_csr(MEMATTRL);
        memAttrVal &= CLEAR_MEMATTR_BIT(pmpMemAttrIdx);
        memAttrVal |= ((uint32_t)pmpMemAttrVal << GET_MEMATTR_PMP_NUM(pmpMemAttrIdx));
        write_custom_csr(MEMATTRL, memAttrVal);
    }
    else
    {
        memAttrVal  = read_custom_csr(MEMATTRH);
        memAttrVal &= CLEAR_MEMATTR_BIT(pmpMemAttrIdx);
        memAttrVal |= ((uint32_t)pmpMemAttrVal << GET_MEMATTR_PMP_NUM(pmpMemAttrIdx));
        write_custom_csr(MEMATTRH, memAttrVal);
    }
}

static void PmpSectlWrite(uint8_t pmpSecIdx, uint8_t pmpCfgVal)
{
    uint32_t sectl;

    sectl  = read_custom_csr(SECTL);
    sectl &= CLEAR_SECTL_BIT(pmpSecIdx);
    sectl |= (pmpCfgVal << GET_SECTL_PMP_NUM(pmpSecIdx));
    write_custom_csr(SECTL, sectl);
}

/*****************************************************************************
 Function   : ulArchRegionDisable
 Description: disable region by number
 Input      : number -- mpu region number want to disable
 Return     : pdPASS or Error Information
 *****************************************************************************/
uint32_t ulArchRegionDisable(uint8_t number)
{
    uint32_t intSave;
    if (number >= PMP_MAX_SUPPORT)
    {
        return errPMP_INVALID_NUMBER;
    }

    if (PmpIsLocked(number) == pdTRUE)
    {
        return errPMP_ENTRY_IS_LOCKED;
    }

    intSave = uxHwiLock();

    PmpSectlWrite(number, SEC_CONTRLO_SECURE_NOMMU);

    PmpCfgWrite(number, PMP_RGN_ADDR_MATCH_OFF);

    vHwiRestore(intSave);

    return pdPASS;
}

static void PmpRegionConfig(const PMP_ENTRY_INFO *pmpInfo)
{
    uint8_t pmpXCfg = 0;
    uint32_t pmpXAddr = UINT32_CUT_MASK;

    switch ((pmpInfo->ucAddressMatch & PMP_RGN_ADDR_MATCH_MASK))
    {
        case PMP_RGN_ADDR_MATCH_OFF:
            pmpXAddr = SET_PMP_PROT_ADDR(pmpInfo->uwBaseAddress);
            break;
        case PMP_RGN_ADDR_MATCH_NAPOT:
            pmpXAddr = PmpNapotAddrGet(pmpInfo->uwBaseAddress, pmpInfo->uwRegionSize);
            break;
        case PMP_RGN_ADDR_MATCH_TOR:
            pmpXAddr = SET_PMP_PROT_ADDR(pmpInfo->uwBaseAddress);
            break;
        case PMP_RGN_ADDR_MATCH_NA4:
            /* The PMP entry size granularity must be the multiple of cache line size, not support yet! */
            return;
        default:
            break;
    }

    /* write pmp address register */
    if (g_pmpTopFlag != pdTRUE)
    {
        PmpAddrWrite(pmpInfo->ucNumber, pmpXAddr);
    }

    /* write pmp memattr/memattrh register */
    PmpMemattrWrite(pmpInfo->ucNumber, pmpInfo->memAttr);

    /* write pmp sectl register */
    PmpSectlWrite(pmpInfo->ucNumber, pmpInfo->seCtl);

    /* write pmp cfg register */
    if (pmpInfo->ucAddressMatch != PMP_RGN_ADDR_MATCH_OFF)
    {
        pmpXCfg |= (pmpInfo->ucAddressMatch) | (PmpAccpGet(pmpInfo->accPermission));
    }
    if (pmpInfo->bLocked)
    {
        pmpXCfg |= PMP_RGN_LOCK;
    }

    PmpCfgWrite(pmpInfo->ucNumber, pmpXCfg);
}

/*****************************************************************************
 Function   : ulArchGetSectl
 Description: Get Sectl region
 Input      : None
 Return     : Sectl register value
 *****************************************************************************/
uint32_t ulArchGetSectl(void)
{
    return read_custom_csr(SECTL);
}

/*****************************************************************************
 Function   : ulArchProtectionRegionSet
 Description: set protection region
 Input      : pmpInfo --- PMP parameters to be set.
              The base address must be in the range of RAM
 Return     : pdPASS or the index of pmpInfo which set up failed
 *****************************************************************************/
uint32_t ulArchProtectionRegionSet(PMP_ENTRY_INFO *pmpInfo)
{
    uint32_t ret;
    uint32_t intSave;
    PMP_ENTRY_INFO *pmpConfig = NULL;

    intSave = uxHwiLock();

    pmpConfig = pmpInfo;
    ret = PmpCheckParame(pmpConfig);
    if (ret != pdPASS)
    {
        PRINTK(ePrintkErr, "set protection region error, ret: [0x%x]\n", ret);
        vHwiRestore(intSave);
        return ret;
    }

    PmpRegionConfig(pmpConfig);
    vHwiRestore(intSave);

    return pdPASS;
}

uint32_t ulArchProtectionUnitInit(uint64_t minAddr, uint64_t maxAddr)
{
    uint32_t intSave;
    intSave = uxHwiLock();

    g_pmpMinAddr = minAddr;
    g_pmpMaxAddr = maxAddr;

    vHwiRestore(intSave);
    return pdPASS;
}

void vArchPmpAddrWrite(uint8_t pmpAddrIdx, uint32_t pmpAddrVal)
{
    PmpAddrWrite(pmpAddrIdx, SET_PMP_PROT_ADDR(pmpAddrVal));
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

