/* ----------------------------------------------------------------------------
 * Copyright (c) @CompanyNameMagicTag 2013-2019. All rights reserved.
 * Description: LiteOS Disk Module Headfile
 * Create: 2013-01-01
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright notice, this list of
 * conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list
 * of conditions and the following disclaimer in the documentation and/or other materials
 * provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used
 * to endorse or promote products derived from this software without specific prior written
 * permission.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * --------------------------------------------------------------------------- */

/**
 * @defgroup disk Disk
 * @ingroup filesystem
 */

#ifndef _DISK_H
#define _DISK_H

#ifdef CONFIG_FS_FAT_CACHE
#include "bcache.h"
#else
#include "inode/inode.h"
#endif
#include "dpal_typedef.h"
#include "dpal_list.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

#define SYS_MAX_DISK                5
#define MAX_DIVIDE_PART_PER_DISK    16
#define MAX_PRIMARY_PART_PER_DISK   4
#define SYS_MAX_PART                (SYS_MAX_DISK * MAX_DIVIDE_PART_PER_DISK)
#define DISK_NAME                   255
#define DISK_MAX_SECTOR_SIZE        512

#define PAR_OFFSET           446     /* MBR: Partition table offset (2) */
#define BS_SIG55AA           510     /* Signature word (2) */
#define BS_FILSYSTEMTYPE32   82      /* File system type (1) */
#define BS_JMPBOOT           0       /* x86 jump instruction (3-byte) */
#define BS_FILSYSTYPE        0x36    /* File system type (2) */
#define BS_SIG55AA_VALUE     0xAA55

#define PAR_TYPE_OFFSET      4
#define PAR_START_OFFSET     8
#define PAR_COUNT_OFFSET     12
#define PAR_TABLE_SIZE       16
#define EXTENDED_PAR         0x0F
#define EXTENDED_8G          0x05
#define EMMC                 0xEC

#define BS_FS_TYPE_MASK      0xFFFFFF
#define BS_FS_TYPE_VALUE     0x544146
#define BS_FS_TYPE_FAT       0x0B
#define BS_FS_TYPE_NTFS      0x07

#define FIRST_BYTE       1
#define SECOND_BYTE      2
#define THIRD_BYTE       3
#define FOURTH_BYTE      4

#define BIT_FOR_BYTE     8

#define LD_WORD_DISK(ptr)    (uint16_t)(((uint16_t)*((uint8_t *)(ptr) + FIRST_BYTE) << (BIT_FOR_BYTE * FIRST_BYTE)) | \
                                      (uint16_t)*(uint8_t *)(ptr))
#define LD_DWORD_DISK(ptr)   (uint32_t)(((uint32_t)*((uint8_t *)(ptr) + THIRD_BYTE) << (BIT_FOR_BYTE * THIRD_BYTE)) |   \
                                      ((uint32_t)*((uint8_t *)(ptr) + SECOND_BYTE) << (BIT_FOR_BYTE * SECOND_BYTE)) | \
                                      ((uint16_t)*((uint8_t *)(ptr) + FIRST_BYTE) << (BIT_FOR_BYTE * FIRST_BYTE)) |   \
                                      (*(uint8_t *)(ptr)))

#define LD_QWORD_DISK(ptr)   ((uint64_t)(((uint64_t)LD_DWORD_DISK(&(ptr)[FOURTH_BYTE]) << (BIT_FOR_BYTE * FOURTH_BYTE)) | \
                              LD_DWORD_DISK(ptr)))

/* Check VBR string, including FAT, NTFS */
#define VERIFY_FS(ptr)       (((LD_DWORD_DISK(&(ptr)[BS_FILSYSTEMTYPE32]) & BS_FS_TYPE_MASK) == BS_FS_TYPE_VALUE) || \
                              !strncmp(&(ptr)[BS_FILSYSTYPE], "FAT", strlen("FAT")) || \
                              !strncmp(&(ptr)[BS_JMPBOOT], "\xEB\x52\x90" "NTFS    ",  \
                                       strlen("\xEB\x52\x90" "NTFS    ")))

#define PARTION_MODE_BTYE    (PAR_OFFSET + PAR_TYPE_OFFSET) /* 0xEE: GPT(GUID), else: MBR */
#define PARTION_MODE_GPT     0xEE /* 0xEE: GPT(GUID), else: MBR */
#define SIGNATURE_OFFSET     0    /* The offset of GPT partition header signature */
#define SIGNATURE_LEN        8    /* The length of GPT signature */
#define HEADER_SIZE_OFFSET   12   /* The offset of GPT header size */
#define TABLE_SIZE_OFFSET    84   /* The offset of GPT table size */
#define TABLE_NUM_OFFSET     80   /* The number of GPT table */
#define TABLE_START_SECTOR   2
#define TABLE_MAX_NUM        128
#define TABLE_SIZE           128
#define GPT_PAR_START_OFFSET      32
#define GPT_PAR_END_OFFSET        40
#define PAR_ENTRY_NUM_PER_SECTOR  4
#define HEADER_SIZE_MASK          0xFFFFFFFF
#define HEADER_SIZE               0x5C
#define HARD_DISK_GUID_OFFSET     56
#define HARD_DISK_GUID_FOR_ESP    0x0020004900460045
#define HARD_DISK_GUID_FOR_MSP    0x007200630069004D
#define PAR_VALID_OFFSET0         0
#define PAR_VALID_OFFSET1         4
#define PAR_VALID_OFFSET2         8
#define PAR_VALID_OFFSET3         12

#define VERIFY_GPT(ptr) ((!strncmp(&(ptr)[SIGNATURE_OFFSET], "EFI PART", SIGNATURE_LEN)) && \
                         ((LD_DWORD_DISK(&(ptr)[HEADER_SIZE_OFFSET]) & HEADER_SIZE_MASK) == HEADER_SIZE))

#define VERITY_PAR_VALID(ptr) ((LD_DWORD_DISK(&(ptr)[PAR_VALID_OFFSET0]) + \
                                LD_DWORD_DISK(&(ptr)[PAR_VALID_OFFSET1]) + \
                                LD_DWORD_DISK(&(ptr)[PAR_VALID_OFFSET2]) + \
                                LD_DWORD_DISK(&(ptr)[PAR_VALID_OFFSET3])) != 0)

/* ESP MSP */
#define VERITY_AVAILABLE_PAR(ptr) ((LD_QWORD_DISK(&(ptr)[HARD_DISK_GUID_OFFSET]) != HARD_DISK_GUID_FOR_ESP) && \
                                   (LD_QWORD_DISK(&(ptr)[HARD_DISK_GUID_OFFSET]) != HARD_DISK_GUID_FOR_MSP))

/* Command code for disk_ioctrl function */
/* Generic command (Used by FatFs) */
#define DISK_CTRL_SYNC          0   /* Complete pending write process */
#define DISK_GET_SECTOR_COUNT   1   /* Get media size */
#define DISK_GET_SECTOR_SIZE    2   /* Get sector size */
#define DISK_GET_BLOCK_SIZE     3   /* Get erase block size */
#define DISK_CTRL_TRIM          4   /* Inform device that the data on the block of sectors is no longer used */

/* Generic command (Not used by FatFs) */
#define DISK_CTRL_POWER         5   /* Get/Set power status */
#define DISK_CTRL_LOCK          6   /* Lock/Unlock media removal */
#define DISK_CTRL_EJECT         7   /* Eject media */
#define DISK_CTRL_FORMAT        8   /* Create physical format on the media */

/* MMC/SDC specific ioctl command */
#define DISK_MMC_GET_TYPE       10  /* Get card type */
#define DISK_MMC_GET_CSD        11  /* Get CSD */
#define DISK_MMC_GET_CID        12  /* Get CID */
#define DISK_MMC_GET_OCR        13  /* Get OCR */
#define DISK_MMC_GET_SDSTAT     14  /* Get SD status */

/* ATA/CF specific ioctl command */
#define DISK_ATA_GET_REV        20  /* Get F/W revision */
#define DISK_ATA_GET_MODEL      21  /* Get model name */
#define DISK_ATA_GET_SN         22  /* Get serial number */

typedef enum _disk_status_ {
    STAT_UNUSED,
    STAT_INUSED,
    STAT_UNREADY
} disk_status_e;

typedef struct _los_disk_ {
    uint32_t disk_id : 8;     /* physics disk number */
    uint32_t disk_status : 2; /* status of disk */
    uint32_t part_count : 8;  /* current partition count */
    uint32_t reserved : 14;
    struct inode *dev;      /* device */
#ifdef CONFIG_FS_FAT_CACHE
    OsBcache *bcache;       /* cache of the disk, shared in all partitions */
#endif
    uint32_t sector_size;     /* disk sector size */
    uint64_t sector_start;    /* disk start sector */
    uint64_t sector_count;    /* disk sector number */
    uint8_t  type;
    char disk_name[DISK_NAME + 1];
    dpal_dl_list_t head;       /* link head of all the partitions */
    uint32_t disk_mutex;
} los_disk;

typedef struct _los_part_ {
    uint32_t disk_id : 8;      /* physics disk number */
    uint32_t part_id : 8;      /* partition number in the system */
    uint32_t part_no_disk : 8; /* partition number in the disk */
    uint32_t part_no_mbr : 5;  /* partition number in the mbr */
    uint32_t reserved : 3;
    uint8_t filesystem_type;   /* filesystem used in the partition */
    struct inode  *dev;      /* dev devices used in the partition */
    uint64_t sector_start;     /*
                              * offset of a partition to the primary devices
                              * (multi-mbr partitions are seen as same parition)
                              */
    uint64_t sector_count;     /*
                              * sector numbers of a partition. If there is no addpartition operation,
                              * then all the mbr devices equal to the primary device count.
                              */
    uint8_t type;
    char *part_name;
    dpal_dl_list_t list;        /* linklist of partition */
} los_part;

struct partition_info {
    uint8_t  type;
    uint64_t sector_start;
    uint64_t sector_count;
};

struct disk_divide_info {
    uint64_t sector_count;
    uint32_t sector_size;
    uint32_t part_count;
    /*
     * The primary partition place should be reversed and set to 0 in case all the partitions are
     * logical partition (maximum 16 currently). So the maximum part number should be 4 + 16.
     */
    struct partition_info part[MAX_DIVIDE_PART_PER_DISK + MAX_PRIMARY_PART_PER_DISK];
};

/**
 * @ingroup  disk
 * @brief Disk driver initialization.
 *
 * @par Description:
 * Initializate a disk dirver, and set the block cache.
 *
 * @attention
 * <ul>
 * <li>The parameter diskName must point a valid string, which end with the terminating null byte.</li>
 * <li>The total length of parameter diskName must be less than the value defined by PATH_MAX.</li>
 * <li>The parameter bops must pointed the right functions, otherwise the system
 * will crash when the disk is being operated.</li>
 * <li>The parameter info can be null or point to struct disk_divide_info. when info is null,
 * the disk will be divided base the information of MBR, otherwise,
 * the disk will be divided base the information of parameter info.</li>
 * </ul>
 *
 * @param  diskName  [IN] Type #const char *                      disk driver name.
 * @param  bops      [IN] Type #const struct block_operations *   block driver control sturcture.
 * @param  priv      [IN] Type #void *                            private data of inode.
 * @param  diskId    [IN] Type #int32_t                             disk id number, less than SYS_MAX_DISK.
 * @param  info      [IN] Type #void *                            disk driver partition information.
 *
 * @retval #0      Initialization success.
 * @retval #-1     Initialization failed.
 *
 * @par Dependency:
 * <ul><li>disk.h</li></ul>
 * @see los_disk_deinit
 *
 */
int32_t los_disk_init(const char *diskName, const struct block_operations *bops,
                    void *priv, int32_t diskId, void *info);

/**
 * @ingroup  disk
 * @brief Destroy a disk driver.
 *
 * @par Description:
 * Destroy a disk driver, free the dependent resource.
 *
 * @attention
 * <ul>
 * None
 * </ul>
 *
 * @param  diskId [IN] Type #int32_t  disk driver id number, less than the value defined by SYS_MAX_DISK.
 *
 * @retval #0      Destroy success.
 * @retval #-1     Destroy failed.
 *
 * @par Dependency:
 * <ul><li>disk.h</li></ul>
 * @see los_disk_init
 *
 */
int32_t los_disk_deinit(int32_t diskId);

/**
 * @ingroup  disk
 * @brief Read data from disk driver.
 *
 * @par Description:
 * Read data from disk driver.
 *
 * @attention
 * <ul>
 * <li>The sector size of the disk to be read should be acquired by los_part_ioctl before calling this function.</li>
 * <li>The parameter buf must point to a valid memory and the buf size is count * sector_size.</li>
 * </ul>
 *
 * @param  drvId   [IN]  Type #int32_t         disk driver id number, less than the value defined by SYS_MAX_DISK.
 * @param  buf     [OUT] Type #void *        memory which used to store read data.
 * @param  sector  [IN]  Type #uint64_t        expected start sector number to read.
 * @param  count   [IN]  Type #uint32_t        expected sector count to read.
 *
 * @retval #0      Read success.
 * @retval #-1     Read failed.
 *
 * @par Dependency:
 * <ul><li>disk.h</li></ul>
 * @see los_disk_write
 *
 */
int32_t los_disk_read(int32_t drvId, void *buf, uint64_t sector, uint32_t count);

/**
 * @ingroup  disk
 * @brief Write data to a disk driver.
 *
 * @par Description:
 * Write data to a disk driver.
 *
 * @attention
 * <ul>
 * <li>The sector size of the disk to be read should be acquired by los_part_ioctl before calling this function.</li>
 * <li>The parameter buf must point to a valid memory and the buf size is count * sector_size.</li>
 * </ul>
 *
 * @param  drvId   [IN]  Type #int32_t           disk driver id number, less than the value defined by SYS_MAX_DISK.
 * @param  buf     [IN]  Type #const void *    memory which used to storage write data.
 * @param  sector  [IN]  Type #uint64_t          expected start sector number to read.
 * @param  count   [IN]  Type #uint32_t          experted sector count of write.
 *
 * @retval #0      Write success.
 * @retval #-1     Write failed.
 *
 * @par Dependency:
 * <ul><li>disk.h</li></ul>
 * @see los_disk_read
 *
 */
int32_t los_disk_write(int32_t drvId, const void *buf, uint64_t sector, uint32_t count);

/**
 * @ingroup  disk
 * @brief Get information of disk driver.
 *
 * @par Description:
 * Get information of disk driver.
 *
 * @attention
 * <ul>
 * None
 * </ul>
 *
 * @param  drvId [IN]  Type #int32_t     disk driver id number, less than the value defined by SYS_MAX_DISK.
 * @param  cmd   [IN]  Type #int32_t     command to issu, currently support GET_SECTOR_COUNT, GET_SECTOR_SIZE,
 *                                     GET_BLOCK_SIZE, CTRL_SYNC.
 * @param  buf   [OUT] Type #void *    memory to storage the information, the size must enough for data type(uint64_t)
 *                                     when cmd type is DISK_GET_SECTOR_COUNT, others is size_t.
 *
 * @retval #0      Get information success.
 * @retval #-1     Get information failed.
 *
 * @par Dependency:
 * <ul><li>disk.h</li></ul>
 * @see None
 */
int32_t los_disk_ioctl(int32_t drvId, int32_t cmd, void *buf);

/**
 * @ingroup  disk
 * @brief Sync blib cache.
 *
 * @par Description:
 * Sync blib cache, write the valid data to disk driver.
 *
 * @attention
 * <ul>
 * None
 * </ul>
 *
 * @param  drvId [IN] Type #int32_t  disk driver id number, less than the value defined by SYS_MAX_DISK.
 *
 * @retval #0         Sync success.
 * @retval #int32_t     Sync failed.
 *
 * @par Dependency:
 * <ul><li>disk.h</li></ul>
 * @see None
 */
int32_t los_disk_sync(int32_t drvId);

/**
 * @ingroup  disk
 * @brief Set blib cache for the disk driver.
 *
 * @par Description:
 * Set blib cache for the disk driver, users can set the number of sectors of per block,
 * and the number of blocks.
 *
 * @attention
 * <ul>
 * None
 * </ul>
 *
 * @param  drvId             [IN] Type #int32_t     disk driver id number, less than the value defined by SYS_MAX_DISK.
 * @param  sectorPerBlock    [IN] Type #uint32_t    sector number of per block, only can be 32 * (1, 2, ..., 8).
 * @param  blockNum          [IN] Type #uint32_t    block number of cache.
 *
 * @retval #0         Set success.
 * @retval #int32_t     Set failed.
 *
 * @par Dependency:
 * <ul><li>disk.h</li></ul>
 * @see None
 */
int32_t los_disk_set_bcache(int32_t drvId, uint32_t sectorPerBlock, uint32_t blockNum);

/**
 * @ingroup  disk
 * @brief Read data from chosen partition.
 *
 * @par Description:
 * Read data from chosen partition.
 *
 * @attention
 * <ul>
 * <li>The sector size of the disk to be read should be acquired by los_part_ioctl before calling this function.</li>
 * <li>The parameter buf must point to valid memory and the buf size is count * sector_size.</li>
 * </ul>
 *
 * @param  pt      [IN]  Type #int32_t        partition number, less than the value defined by SYS_MAX_PART.
 * @param  buf     [OUT] Type #void *       memory which used to store the data to be read.
 * @param  sector  [IN]  Type #uint64_t       start sector number of chosen partition.
 * @param  count   [IN]  Type #uint32_t       the expected sector count for reading.
 *
 * @retval #0      Read success.
 * @retval #-1     Read failed.
 *
 * @par Dependency:
 * <ul><li>disk.h</li></ul>
 * @see los_part_read
 */
int32_t los_part_read(int32_t pt, void *buf, uint64_t sector, uint32_t count);

/**
 * @ingroup  disk
 * @brief Write data to chosen partition.
 *
 * @par Description:
 * Write data to chosen partition.
 *
 * @attention
 * <ul>
 * <li>The sector size of the disk to be write should be acquired by los_part_ioctl before calling this function.</li>
 * <li>The parameter buf must point to valid memory and the buf size is count * sector_size.</li>
 * </ul>
 *
 * @param  pt      [IN] Type #int32_t        partition number,less than the value defined by SYS_MAX_PART.
 * @param  buf     [IN] Type #void *       memory which used to storage the written data.
 * @param  sector  [IN] Type #uint64_t       start sector number of chosen partition.
 * @param  count   [IN] Type #uint32_t       the expected sector count for write.
 *
 * @retval #0      Write success.
 * @retval #-1     Write failed.
 *
 * @par Dependency:
 * <ul><li>disk.h</li></ul>
 * @see los_part_read
 */
int32_t los_part_write(int32_t pt, const void *buf, uint64_t sector, uint32_t count);

/**
 * @ingroup  disk
 * @brief Get information of chosen partition.
 *
 * @par Description:
 * By passed command to get information of chosen partition.
 *
 * @attention
 * <ul>
 * None
 * </ul>
 *
 * @param  pt   [IN]  Type #int32_t      partition number,less than the value defined by SYS_MAX_PART.
 * @param  cmd  [IN]  Type #int32_t      command to issu, currently support GET_SECTOR_COUNT, GET_SECTOR_SIZE,
 *                                     GET_BLOCK_SIZE, CTRL_SYNC.
 * @param  buf  [OUT] Type #void *     memory to store the information, the size must enough for data type (uint64_t)
 *                                     when cmd type is DISK_GET_SECTOR_COUNT, others is size_t.
 *
 * @retval #0      Get information success.
 * @retval #-1     Get information failed.
 *
 * @par Dependency:
 * <ul><li>disk.h</li></ul>
 * @see None
 */
int32_t los_part_ioctl(int32_t pt, int32_t cmd, void *buf);

/**
 * @ingroup  disk
 * @brief Decide the chosen partition is exist or not.
 *
 * @par Description:
 * Decide the chosen partition is exist or not.
 *
 * @attention
 * <ul>
 * <li>The parameter dev is a full path, which begin with '/' and end with '/0'.</li>
 * </ul>
 *
 * @param  dev  [IN]  Type #const char *    partition driver name.
 * @param  mode [IN]  Type #mode_t          access modd.
 *
 * @retval #0      The chosen partition is exist.
 * @retval #-1     The chosen partition is not exist.
 *
 * @par Dependency:
 * <ul><li>disk.h</li></ul>
 * @see None
 */
int32_t los_part_access(const char *dev, mode_t mode);

/**
 * @ingroup  disk
 * @brief Find disk partition.
 *
 * @par Description:
 * By driver partition inode to find disk partition.
 *
 * @attention
 * <ul>
 * None
 * </ul>
 *
 * @param  blkDriver  [IN]  Type #struct inode *    partition driver inode.
 *
 * @retval #NULL           Can't find chosen disk partition.
 * @retval #los_part *     This is partition structure pointer of chosen disk partition.
 *
 * @par Dependency:
 * <ul><li>disk.h</li></ul>
 * @see None
 */
los_part *los_part_find(const struct inode *blkDriver);

/**
 * @ingroup  disk
 * @brief Find disk driver.
 *
 * @par Description:
 * By disk driver id number to find disk dirver.
 *
 * @attention
 * <ul>
 * None
 * </ul>
 *
 * @param  id  [IN]  Type #int32_t  disk id number,less than the value defined by SYS_MAX_DISK.
 *
 * @retval #NULL           Can't find chosen disk driver.
 * @retval #los_disk *     This is disk structure pointer of chosen disk driver.
 *
 * @par Dependency:
 * <ul><li>disk.h</li></ul>
 * @see None
 */
los_disk *get_disk(int32_t id);

/**
 * @ingroup  disk
 * @brief Find disk partition.
 *
 * @par Description:
 * By driver partition id number to find disk partition.
 *
 * @attention
 * <ul>
 * None
 * </ul>
 *
 * @param  id  [IN]  Type #int32_t partition id number,less than the value defined by SYS_MAX_PART.
 *
 * @retval #NULL           Can't find chosen disk partition.
 * @retval #los_part *     This is partition structure pointer of chosen disk partition.
 *
 * @par Dependency:
 * <ul><li>disk.h</li></ul>
 * @see None
 */
los_part *get_part(int32_t id);

/**
 * @ingroup  disk
 * @brief Print partition information.
 *
 * @par Description:
 * Print partition information.
 *
 * @attention
 * <ul>
 * None
 * </ul>
 *
 * @param  part  [IN]  Type #los_part *  partition control structure pointer
 *
 * @par Dependency:
 * <ul><li>disk.h</li></ul>
 * @see None
 */
void show_part(const los_part *part);

/**
 * @ingroup  disk
 * @brief Add a new mmc partition.
 *
 * @par Description:
 * Add a new mmc partition, users can set the start sector and size of the new partition.
 *
 * @attention
 * <ul>
 * None
 * </ul>
 *
 * @param  info          [IN]  Type #struct disk_divide_info *  Disk driver information structure pointer.
 * @param  sectorStart   [IN]  Type #size_t                     Start sector number of the new partition.
 * @param  sectorCount   [IN]  Type #size_t                     Sector count of the new partition.
 *
 * @retval #0      Add partition success.
 * @retval #-1     Add partition failed.
 *
 * @par Dependency:
 * <ul><li>disk.h</li></ul>
 * @see None
 */
int32_t add_mmc_partition(struct disk_divide_info *info, size_t sectorStart, size_t sectorCount);
int32_t del_mmc_partition(struct disk_divide_info *info, size_t sectorStart, size_t sectorCount);

/**
 * @ingroup  disk
 * @brief alloc a new UNUSED disk id.
 *
 * @par Description:
 * Get a free disk id for new device.
 *
 * @attention
 * <ul>
 * <li>The parameter diskName must point a valid string, which end with the null byte ('\0') </li>
 * <li>The total length of parameter diskName must be less than the value defined by DISK_NAME </li>
 * </ul>
 *
 * @param  diskName      [IN]  Type #const char *   device name.
 *
 * @retval #int32_t        available disk id
 * @retval #-1           alloc disk id failed

 * @par Dependency:
 * <ul><li>disk.h</li></ul>
 * @see los_get_diskid_byname
 */
int32_t los_alloc_diskid_byname(const char *diskName);

/**
 * @ingroup  disk
 * @brief get the INUSED disk id.
 *
 * @par Description:
 * Get the correponding INUSED disk id by diskName.
 *
 * @attention
 * <ul>
 * <li>The parameter diskName must point a valid string, which end with the null byte ('\0') </li>
 * <li>The total length of parameter diskName must be less than the value defined by DISK_NAME </li>
 * </ul>
 *
 * @param  diskName      [IN]  Type #const char *  device name.
 *
 * @retval #int32_t       available disk id
 * @retval #-1          get disk id failed

 * @par Dependency:
 * <ul><li>disk.h</li></ul>
 * @see los_alloc_diskid_byname
 */
int32_t los_get_diskid_byname(const char *diskName);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif
