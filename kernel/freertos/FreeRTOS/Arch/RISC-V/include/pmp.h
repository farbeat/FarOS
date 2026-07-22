/*
 * Copyright (c) CompanyNameMagicTag 2021-2022. All rights reserved.
 */

#ifndef _ARCH_PMP_H
#define _ARCH_PMP_H

#include "stdint.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/**
 * @ingroup pmp
 * Locking PMP entry.
 */
#define PMP_RGN_LOCK                0x80

#define PMP_MAX_SUPPORT      16
#define PMP_MIN_ADDRESS      0x00000000UL
#define PMP_MAX_ADDRESS      0xFFFFFFFFUL

/**
 * @ingroup pmp
 * Address Matching.
 */
#define PMP_RGN_ADDR_MATCH_OFF      0    /* Null region (disabled) */
#define PMP_RGN_ADDR_MATCH_TOR      0x8  /* Top of range */
#define PMP_RGN_ADDR_MATCH_NA4      0x10 /* Naturally aligned four-byte region */
#define PMP_RGN_ADDR_MATCH_NAPOT    0x18 /* Naturally aligned power-of-two region, >= 8 bytes */
#define PMP_RGN_ADDR_MATCH_MASK     0x18

/**
 * @ingroup pmp
 * Access permission.
 */
#define PMPCFG_R_BIT    0
#define PMPCFG_W_BIT    1
#define PMPCFG_X_BIT    2

/* risc-v pmp register config */
#define MEMATTRL                    0x7D8
#define MEMATTRH                    0x7D9
#define SECTL                       0x7DA

#ifndef __ASSEMBLER__
enum MemReadAcc {
    E_MEM_RD_ACC_NON_RD = 0,
    E_MEM_RD_ACC_RD = 1,
};

enum MemWriteReadAcc {
    E_MEM_WR_ACC_NON_WR = 0,
    E_MEM_WR_ACC_WR = 1,
};

enum MemExecuteAcc {
    E_MEM_EX_ACC_NON_EX = 0,
    E_MEM_EX_ACC_EX = 1,
};

typedef struct {
    enum MemReadAcc       readAcc;
    enum MemWriteReadAcc  writeAcc;
    enum MemExecuteAcc    excuteAcc;
} PmpAccInfo;

/**
 * @ingroup pmp
 * memory attribute.
 */
typedef enum {
    MEM_ATTR_DEV_NON_BUF = 0,         /* 4b0000, device non-bufferable */
    MEM_ATTR_NORM_NON_CA_NON_BUF = 2, /* 4b0010, normal non-cacheable non-bufferable */
    MEM_ATTR_NORM_NON_CA_BUF = 3,     /* 4b0011, normal non-cacheable bufferable */
    MEM_ATTR_WB_RD_ALLOC = 7,         /* 4b0111, write-back read-only allocate */
    MEM_ATTR_WB_WR_ALLOC = 11,        /* 4b1011, write-back write-only allocate */
    MEM_ATTR_WB_RW_ALLOC = 15,        /* 4b1111, write-back write and read allocate */
} PmpMemAttrInfo;

typedef enum {
    SEC_CONTRLO_SECURE_NOMMU = 0,   /* secure and disable SoC IOMMU */
    SEC_CONTRLO_NOSECURE_NOMMU = 1, /* no-secure and disable SoC IOMMU */
    SEC_CONTRLO_SECURE_MMU = 2,     /* secure and enable SoC IOMMU */
    SEC_CONTRLO_NOSECURE_MMU = 3,   /* no-secure and enable SoC IOMMU */
} PmpSeControlInfo;

/**
 * @ingroup pmp
 * PMP entry info
 * uwBaseAddress must be in the range of RAM, and
 * uwRegionSize granularity must be the multiple of cache line size
 * if addrress matching mode  equals to NPAOT, the base address and size should match the type and size
 * uwAccessPermission is not arbitrary input
 */
typedef struct {
    /* Index of PMP entry */
    uint8_t  ucNumber;

    /* whether lock, Works in any supported mode */
    BaseType_t  bLocked;

    /*
     * Address matching mode of PMP configuration,
     * Currently supported modes: PMP_RGN_ADDR_MATCH_OFF,
     * PMP_RGN_ADDR_MATCH_TOR and PMP_RGN_ADDR_MATCH_NAPOT.
     */
    uint8_t ucAddressMatch;

    /*
     * access permission of read, write and instruction execution.
     * The changed member is not valid in PMP_RGN_ADDR_MATCH_OFF mode.
     */
    PmpAccInfo accPermission;

    /*
     * set the base address of the protected region,
     * the base address must be in the range of RAM.
     * It should be cache line aligned under the cache.
     */
    uint32_t uwBaseAddress;

    /*
     * PMP entry region size: It is valid only in PMP_RGN_ADDR_MATCH_NAPOT mode,
     * requiring >= 8byte and aligned power-of-2 byte.
     * It should be cache line aligned under the cache.
     */
    uint32_t uwRegionSize;

    /*
     * PMP memory attribute register information:
     * The changed member is not valid in PMP_RGN_ADDR_MATCH_OFF mode.
     */
    PmpMemAttrInfo memAttr;

    /*
     * Security control register information:
     * The changed member is not valid in PMP_RGN_ADDR_MATCH_OFF mode.
     */
    PmpSeControlInfo  seCtl;
} PMP_ENTRY_INFO;

/**
 * @ingroup pmp
 * PMP error code: The pointer to an input parameter is NULL.
 *
 * Value: 0x00
 *
 * Solution: Check whether the pointer to the input parameter is usable.
 */
#define errPMP_PTR_NULL                    0x00

/**
 * @ingroup pmp
 * PMP error code: The base address is not aligned to the boundary of the region capacity.
 *
 * Value: 0x01
 *
 * Solution: Check base address.
 */
#define errPMP_INVALID_BASE_ADDRESS        0x01

/**
 * @ingroup pmp
 * PMP error code: Capacity less than cacheline bytes.
 * The PMP entry size granularity must be the multiple of cache line size.
 *
 * Value: 0x02
 *
 * Solution: Guaranteed that the capacity is greater than or equal to cacheline, supporting cacheline granularity.
 */
#define errPMP_INVALID_CAPACITY            0x02

/**
 * @ingroup pmp
 * PMP error code: Type extension that given is invalid.
 *
 * Value: 0x03
 *
 * Solution: memory attribute configuration only support 0000,0010,0011,0111,,1011,1111
 */
#define errPMP_ATTR_INVALID                0x03

/**
 * @ingroup pmp
 * PMP error code: Invalid number.
 *
 * Value: 0x04
 *
 * Solution: Enter a valid number.
 */
#define errPMP_INVALID_NUMBER              0x04

/**
 * @ingroup pmp
 * PMP error code: The next PMP is TOR mode, current pmpcfg match is error.
 *
 * Value: 0x05
 *
 * Solution: The next PMP is TOR mode, and when modifying the current pmpcfg,
 * When modifying the current config, if the current pmpcfg original match is
 * PMP_RGN_ADDR_MATCH_TOR or PMP_RGN_ADDR_MATCH_OFF, the modified match must
 * be PMP_RGN_ADDR_MATCH_TOR or PMP_RGN_ADDR_MATCH_OFF.
 */
#define errPMP_TOR_INVALID_MATCH_MODE     0x05

/**
 * @ingroup pmp
 * PMP error code: The next PMP is TOR mode, current pmpcfg match is error.
 *
 * Value: 0x06
 *
 * Solution: The next PMP is TOR mode, and when modifying the current pmpcfg,
 * When modifying the current config, if the current pmpcfg original match is
 * PMP_RGN_ADDR_MATCH_NAPOT, the modified match must be PMP_RGN_ADDR_MATCH_NAPOT.
 */
#define errPMP_NAPOT_INVALID_MATCH_MODE   0x06

/**
 * @ingroup pmp
 * PMP error code: Invalid access.
 *
 * Value: 0x07
 *
 * Solution: Checking whether the access is correct.
 */
#define errPMP_REGION_INVALID_ACCESS      0x07

/**
 * @ingroup pmp
 * PMP error code: Base address is not in range.
 *
 * Value: 0x08
 *
 * Solution: Checking base address
 */
#define errPMP_BASE_ADDRESS_NOT_IN_RANGE  0x08

/**
 * @ingroup pmp
 * PMP error code: According to the current base address, the size of the application is too big.
 *
 * Value: 0x09
 *
 * Solution: baseAddress + regionSize must not exceed RAM Max address
 */
#define errPMP_REGION_SIZE_IS_TOO_BIG     0x09

/**
 * @ingroup pmp
 * PMP error code: According to the current configuration, the PMP entry is locked.
 *
 * Value: 0x0a
 *
 * Solution: if PMP entry is locked, then write configuration and address register will be ignored,
 * you should reset the system.
 */
#define errPMP_ENTRY_IS_LOCKED            0x0a

/**
 * @ingroup pmp
 * PMP error code: PMP_RGN_ADDR_MATCH_NA4 is Unsupported patterns.
 *
 * Value: 0x0b
 *
 * Solution: PMP_RGN_ADDR_MATCH_NA4 is not support now.
 */
#define errPMP_INVALID_MATCH              0x0b

/**
 * @ingroup pmp
 * PMP error code: According to the current configuration,
 * the NAPOT type and size is not matched in NAPOT range encoding.
 *
 * Value: 0x0c
 *
 * Solution: baseAddress + regionSize must obey the NAPOT range encoding rules.
 */
#define errPMP_NAPOT_ENCODING_NON_MATCHED 0x0c

/**
 * @ingroup pmp
 * PMP error code: According to the current configuration, if pmp(i + 1)cfg.A is set to TOR,
 * writes to pmpaddr(i) are ignored.
 *
 * Value: 0x0d
 *
 * Solution: check if the lower bound of region is locked of this region when setting address matching mode to TOR.
 */
#define errPMP_TOR_LOWER_BOUND_LOCKED     0x0d

/**
 * @ingroup pmp
 * PMP error code: According to the current configuration, check whether the lower bound of TOR region equals to
 * the pre-region end address
 *
 * Value: 0x0e
 *
 * Solution: set the lower bound of TOR i-th region to the pre-region, i.e. (i-1)'th region end address
 */
#define errPMP_TOR_LOWER_BOUND_NONMATCHED 0x0e

/**
 * @ingroup pmp
 * PMP error code: Type extension that given is invalid.
 *
 * Value: 0xf
 *
 * Solution: memory sectl configuration only support 00,01,10,11
 */
#define errPMP_SECTL_INVALID              0xf

/**
 * pmp.h
 * <pre>
 * uint32_t ulArchProtectionUnitInit(uint64_t minAddr, uint64_t maxAddr);
 * </pre>
 *
 * pmp region init
 *
 * @param minAddr Minimum protected address.
 *
 * @param maxAddr Maximum protected address
 *
 * @return pdPASS.
 *
 * \defgroup ulArchProtectionUnitInit ulArchProtectionUnitInit
 * \ingroup pmp
 */
uint32_t ulArchProtectionUnitInit(uint64_t minAddr, uint64_t maxAddr);

/**
 * pmp.h
 * <pre>
 * uint32_t ulArchGetSectl( void );
 * </pre>
 *
 * pmp Sectl region get.
 *
 * @param none.
 *
 * @return Sectl register value is returned.
 *
 * \defgroup ulArchGetSectl ulArchGetSectl
 * \ingroup pmp
 */
uint32_t ulArchGetSectl(void);

/**
 * pmp.h
 * <pre>
 * uint32_t ulArchRegionDisable(uint8_t number);
 * </pre>
 *
 * pmp region disable.
 *
 * @param number The pmp entry number.
 *
 * @return pdPASS, error information.
 *
 * \defgroup ulArchRegionDisable ulArchRegionDisable
 * \ingroup pmp
 */
uint32_t ulArchRegionDisable(uint8_t number);

/**
 * pmp.h
 * <pre>
 * uint32_t ulArchProtectionRegionSet(PMP_ENTRY_INFO *pmpInfo);
 * </pre>
 *
 * Set protection memory region.
 *
 * @param pmpInfo Set the related configuration information for the protected area,
 * @param pmpInfo->bLocked Lock region in machine mode. if 1, lock region, validating access permission in
 * machine mode, otherwise, disable it.
 *
 * @return pdPASS or the index of pmpInfo which set up failed.
 *
 * \defgroup ulArchProtectionRegionSet ulArchProtectionRegionSet
 * \ingroup pmp
 */
uint32_t ulArchProtectionRegionSet(PMP_ENTRY_INFO *pmpInfo);

/**
 * pmp.h
 * <pre>
 * void vArchPmpAddrWrite(uint8_t pmpAddrIdx, uint32_t pmpAddrVal);
 * </pre>
 *
 * Set pmp addr.
 *
 * @param pmpAddrIdx pmp addr index.
 * @param pmpAddrVal pmp addr value.
 *
 * @return none.
 *
 * \defgroup ulArchProtectionRegionSet ulArchProtectionRegionSet
 * \ingroup pmp
 */
void vArchPmpAddrWrite(uint8_t pmpAddrIdx, uint32_t pmpAddrVal);
#endif  /* __ASSEMBLER__ */

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* _ARCH_PMP_H */
