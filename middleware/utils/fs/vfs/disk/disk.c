/* ----------------------------------------------------------------------------
 * Copyright (c) @CompanyNameMagicTag 2013-2019. All rights reserved.
 * Description: LiteOS Disk Module Implementation
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

#include "disk_pri.h"
#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "sys/mount.h"
#include "inode/inode.h"
#ifdef CONFIG_DRIVERS_MMC
#include "block.h"
#endif

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

los_disk g_sysDisk[SYS_MAX_DISK];
los_part g_sysPart[SYS_MAX_PART];

#define CONFIG_FS_FAT_MAX_SECTOR_PER_BLOCK 2048
#if CONFIG_FS_FAT_SECTOR_PER_BLOCK > CONFIG_FS_FAT_MAX_SECTOR_PER_BLOCK
#error CONFIG_FS_FAT_SECTOR_PER_BLOCK should less than 2048!
#endif
uint32_t g_uwFatSectorsPerBlock = CONFIG_FS_FAT_SECTOR_PER_BLOCK;
uint32_t g_uwFatBlockNums = CONFIG_FS_FAT_BLOCK_NUMS;

dpal_spinlock g_diskSpinlock;
dpal_spinlock g_diskFatBlockSpinlock;

uint32_t g_usbMode = 0;

#define MEM_ADDR_ALIGN_BYTE  64
#define RWE_RW_RW            0755

typedef void *(*StorageHookFunction)(void *);

static uint32_t OsReHookFuncAddDiskRef(StorageHookFunction handler,
    void *param) __attribute__((weakref("osReHookFuncAdd")));
static uint32_t OsReHookFuncDelDiskRef(StorageHookFunction handler) __attribute__((weakref("osReHookFuncDel")));

#ifdef CONFIG_FS_FAT_CACHE
uint32_t GetFatBlockNums(void)
{
    return g_uwFatBlockNums;
}

void SetFatBlockNums(uint32_t blockNums)
{
    g_uwFatBlockNums = blockNums;
}

uint32_t GetFatSectorsPerBlock(void)
{
    return g_uwFatSectorsPerBlock;
}

void SetFatSectorsPerBlock(uint32_t sectorsPerBlock)
{
    if (((sectorsPerBlock % UNSIGNED_INTEGER_BITS) == 0) &&
        (sectorsPerBlock <= CONFIG_FS_FAT_MAX_SECTOR_PER_BLOCK)) {
        g_uwFatSectorsPerBlock = sectorsPerBlock;
    }
}
#endif

int32_t los_alloc_diskid_byname(const char *diskName)
{
    int32_t diskId;
    los_disk *disk = NULL;
    unsigned long intSave;

    if (diskName == NULL) {
        disk_log_err("The paramter disk_name is NULL");
        return VFS_ERROR;
    }

    if (strnlen(diskName, DISK_NAME + 1) > DISK_NAME) {
        disk_log_err("diskName is too long!\n");
        return VFS_ERROR;
    }
    dpal_spin_lock_irqsave(&g_diskSpinlock, &intSave);

    for (diskId = 0; diskId < SYS_MAX_DISK; diskId++) {
        disk = get_disk(diskId);
        if ((disk != NULL) && (disk->disk_status == STAT_UNUSED)) {
            disk->disk_status = STAT_UNREADY;
            break;
        }
    }


    dpal_spin_unlock_irqrestore(&g_diskSpinlock, &intSave);

    if ((disk == NULL) || (diskId == SYS_MAX_DISK)) {
        disk_log_err("los_alloc_diskid_byname failed %d!\n", diskId);
        return VFS_ERROR;
    }

    if (strncpy_s(disk->disk_name, DISK_NAME + 1, diskName, strlen(diskName)) != EOK) {
        disk_log_err("The strncpy_s failed.\n");
        return VFS_ERROR;
    }

    return diskId;
}

int32_t los_get_diskid_byname(const char *diskName)
{
    int32_t diskId;
    los_disk *disk = NULL;
    size_t diskNameLen;

    if (diskName == NULL) {
        disk_log_err("The paramter diskName is NULL");
        return VFS_ERROR;
    }

    diskNameLen = strnlen(diskName, DISK_NAME + 1);
    if (diskNameLen > DISK_NAME) {
        disk_log_err("diskName is too long!\n");
        return VFS_ERROR;
    }

    for (diskId = 0; diskId < SYS_MAX_DISK; diskId++) {
        disk = get_disk(diskId);
        if ((disk != NULL) && (disk->disk_status != STAT_INUSED)) {
            continue;
        }
        if ((disk != NULL) && (strcmp(diskName, disk->disk_name) == 0)) {
            break;
        }
    }
    if ((disk == NULL) || (diskId == SYS_MAX_DISK)) {
        disk_log_err("los_get_diskid_byname failed!\n");
        return VFS_ERROR;
    }
    return diskId;
}

void OsSetUsbStatus(uint32_t diskId)
{
    if (diskId < SYS_MAX_DISK) {
        g_usbMode |= (1u << diskId) & UINT_MAX;
    }
}

void OsClearUsbStatus(uint32_t diskId)
{
    if (diskId < SYS_MAX_DISK) {
        g_usbMode &= ~((1u << diskId) & UINT_MAX);
    }
}

#ifdef CONFIG_FS_FAT_CACHE
static uint32_t GetDiskUsbStatus(uint32_t diskId)
{
    return (g_usbMode & (1u << diskId)) ? TRUE : FALSE;
}
#endif

los_disk *get_disk(int32_t id)
{
    if ((id >= 0) && (id < SYS_MAX_DISK)) {
        return &g_sysDisk[id];
    }

    return NULL;
}

los_part *get_part(int32_t id)
{
    if ((id >= 0) && (id < SYS_MAX_PART)) {
        return &g_sysPart[id];
    }

    return NULL;
}

static uint64_t GetFirstPartStart(const los_part *part)
{
    los_part *firstPart = NULL;
    los_disk *disk = get_disk((int32_t)part->disk_id);
    firstPart = (disk == NULL) ? NULL : DPAL_DL_LIST_ENTRY(disk->head.next, los_part, list);
    return (firstPart == NULL) ? 0 : firstPart->sector_start;
}

static void DiskPartAddToDisk(los_disk *disk, los_part *part)
{
    DISK_LOCK(disk);
    part->disk_id = disk->disk_id;
    part->part_no_disk = disk->part_count;
    dpal_list_tail_insert(&disk->head, &part->list);
    disk->part_count++;
    DISK_UNLOCK(disk);
}

static void DiskPartDelFromDisk(los_disk *disk, los_part *part)
{
    dpal_list_delete(&part->list);
    disk->part_count--;
}

static los_part *DiskPartAllocate(struct inode *dev, uint64_t start, uint64_t count)
{
    uint32_t i;
    los_part *part = get_part(0); /* traversing from the beginning of the array */

    for (i = 0; i < SYS_MAX_PART; i++) {
        if (part->dev == NULL) {
            part->part_id = i;
            part->part_no_mbr = 0;
            part->dev = dev;
            part->sector_start = start;
            part->sector_count = count;
            part->part_name = NULL;
            dpal_list_init(&part->list);

            return part;
        }
        part++;
    }

    return NULL;
}

static void DiskPartRelease(los_part *part)
{
    part->dev = NULL;
    part->part_no_disk = 0;
    part->part_no_mbr = 0;
    if (part->part_name != NULL) {
        dpal_free(part->part_name);
        part->part_name = NULL;
    }
}

/*
 * name is a combination of disk_name, 'p' and part_count, such as "/dev/mmcblk0p0"
 * disk_name : DISK_NAME + 1
 * 'p' : 1
 * part_count: 1
 */
#define DEV_NAME_BUFF_SIZE  (DISK_NAME + 3)
static int32_t DiskAddPart(los_disk *disk, uint64_t sectorStart, uint64_t sectorCount, BOOL IsValidPart)
{
    struct inode_search_s desc;
    char devName[DEV_NAME_BUFF_SIZE];
    struct inode *diskDev = NULL;
    los_part *part = NULL;

    if ((disk == NULL) || (disk->disk_status == STAT_UNUSED) || (disk->dev == NULL)) {
        return VFS_ERROR;
    }

    if ((sectorCount > disk->sector_count) || ((disk->sector_count - sectorCount) < sectorStart)) {
        disk_log_err("DiskAddPart failed: sector start is %llu, sector count is %llu\n", sectorStart, sectorCount);
        return VFS_ERROR;
    }

    diskDev = disk->dev;
    if (IsValidPart == TRUE) {
        if (snprintf_s(devName, sizeof(devName), sizeof(devName) - 1, "%s%c%u",
                       disk->disk_name, 'p', disk->part_count) < 0) {
            return VFS_ERROR;
        }

        if (register_blockdriver(devName, diskDev->u.i_bops, RWE_RW_RW, diskDev->i_private)) {
            disk_log_err("DiskAddPart : register %s fail!\n", devName);
            return VFS_ERROR;
        }

        SETUP_SEARCH(&desc, devName, false);
        if (inode_find(&desc) < 0) {
            disk_log_err("DiskAddPart : find %s fail!\n", devName);
            RELEASE_SEARCH(&desc);
            return VFS_ERROR;
        }
        printf("DiskAddPart : register %s ok!\n", devName);

        part = DiskPartAllocate(desc.node, sectorStart, sectorCount);
        inode_release(desc.node);
        RELEASE_SEARCH(&desc);
        if (part == NULL) {
            (void)unregister_blockdriver(devName);
            return VFS_ERROR;
        }
    } else {
        part = DiskPartAllocate(diskDev, sectorStart, sectorCount);
        if (part == NULL) {
            return VFS_ERROR;
        }
    }

    DiskPartAddToDisk(disk, part);
    if (disk->type == EMMC) {
        part->type = EMMC;
    }

    return (int32_t)part->part_id;
}

static int32_t DiskDivide(los_disk *disk, struct disk_divide_info *info)
{
    uint32_t i;
    int32_t ret;

    disk->type = info->part[0].type;
    for (i = 0; i < info->part_count; i++) {
        if (info->sector_count < info->part[i].sector_start) {
            return VFS_ERROR;
        }
        if (info->part[i].sector_count > (info->sector_count - info->part[i].sector_start)) {
            disk_log_err("Part[%u] sector_start:%llu, sector_count:%llu, exceed emmc sector_count:%llu.\n", i,
                      info->part[i].sector_start, info->part[i].sector_count,
                      (info->sector_count - info->part[i].sector_start));
            info->part[i].sector_count = info->sector_count - info->part[i].sector_start;
            disk_log_err("Part[%u] sector_count change to %llu.\n", i, info->part[i].sector_count);

            ret = DiskAddPart(disk, info->part[i].sector_start, info->part[i].sector_count, TRUE);
            if (ret == VFS_ERROR) {
                return VFS_ERROR;
            }
            break;
        }
        ret = DiskAddPart(disk, info->part[i].sector_start, info->part[i].sector_count, TRUE);
        if (ret == VFS_ERROR) {
            return VFS_ERROR;
        }
    }

    return ENOERR;
}

static int32_t DiskPartitionMemZalloc(size_t boundary, size_t size, char **gptBuf, char **partitionBuf)
{
    char *buffer1 = NULL;
    char *buffer2 = NULL;

    buffer1 = (char *)dpal_memalign(boundary, size);
    if (buffer1 == NULL) {
        disk_log_err("%s buffer1 malloc %lu failed! %d\n", __FUNCTION__, size, __LINE__);
        return -ENOMEM;
    }
    buffer2 = (char *)dpal_memalign(boundary, size);
    if (buffer2 == NULL) {
        disk_log_err("%s buffer2 malloc %lu failed! %d\n", __FUNCTION__, size, __LINE__);
        dpal_free(buffer1);
        return -ENOMEM;
    }
    (void)memset_s(buffer1, size, 0, size);
    (void)memset_s(buffer2, size, 0, size);

    *gptBuf = buffer1;
    *partitionBuf = buffer2;

    return ENOERR;
}


#ifdef CONFIG_FS_FAT_GPT
static char GPTPartitionTypeRecognition(const char *parBuf)
{
    const char *buf = parBuf;
    const char *fsType = "FAT";
    const char *str = "\xEB\x52\x90" "NTFS    "; /* NTFS Boot entry point */

    if (((LD_DWORD_DISK(&buf[BS_FILSYSTEMTYPE32]) & BS_FS_TYPE_MASK) == BS_FS_TYPE_VALUE) ||
        (strncmp(&buf[BS_FILSYSTYPE], fsType, strlen(fsType)) == 0)) {
        return BS_FS_TYPE_FAT;
    } else if (strncmp(&buf[BS_JMPBOOT], str, strlen(str)) == 0) {
        return BS_FS_TYPE_NTFS;
    }

    return ENOERR;
}

static int32_t GPTInfoGet(struct inode *blkDrv, char *gptBuf)
{
    int32_t ret;

    ret = blkDrv->u.i_bops->read(blkDrv, (unsigned char *)gptBuf, 1, 1); /* Read the device first sector */
    if (ret != 1) { /* Read failed */
        disk_log_err("%s %d\n", __FUNCTION__, __LINE__);
        return -EIO;
    }

    if (!VERIFY_GPT(gptBuf)) {
        disk_log_err("%s %d\n", __FUNCTION__, __LINE__);
        return VFS_ERROR;
    }

    return ENOERR;
}

static int32_t OsGPTPartitionRecognitionSub(struct disk_divide_info *info, const char *partitionBuf,
                                          uint32_t *partitionCount, uint64_t partitionStart, uint64_t partitionEnd)
{
    char partitionType;

    if (VERIFY_FS(partitionBuf)) {
        partitionType = GPTPartitionTypeRecognition(partitionBuf);
        if (partitionType) {
            if (*partitionCount >= MAX_DIVIDE_PART_PER_DISK) {
                return VFS_ERROR;
            }
            info->part[*partitionCount].type = partitionType;
            info->part[*partitionCount].sector_start = partitionStart;
            info->part[*partitionCount].sector_count = (partitionEnd - partitionStart) + 1;
            (*partitionCount)++;
        } else {
            disk_log_err("The partition type is not allowed to use!\n");
        }
    } else {
        disk_log_err("Do not support the partition type!\n");
    }
    return ENOERR;
}

static int32_t OsGPTPartitionRecognition(struct inode *blkDrv, struct disk_divide_info *info,
                                       const char *gptBuf, char *partitionBuf, uint32_t *partitionCount)
{
    uint32_t j;
    int32_t ret;
    uint64_t partitionStart, partitionEnd;

    for (j = 0; j < PAR_ENTRY_NUM_PER_SECTOR; j++) {
        if (!VERITY_AVAILABLE_PAR(&gptBuf[j * TABLE_SIZE])) {
            printf("The partition type is ESP or MSR!\n");
            continue;
        }

        if (!VERITY_PAR_VALID(&gptBuf[j * TABLE_SIZE])) {
            return VFS_ERROR;
        }

        partitionStart = LD_QWORD_DISK(&gptBuf[(j * TABLE_SIZE) + GPT_PAR_START_OFFSET]);
        partitionEnd = LD_QWORD_DISK(&gptBuf[(j * TABLE_SIZE) + GPT_PAR_END_OFFSET]);
        if ((partitionStart >= partitionEnd) || (partitionEnd > info->sector_count)) {
            disk_log_err("GPT partition %u recognition failed : partitionStart = %llu, partitionEnd = %llu\n",
                      j, partitionStart, partitionEnd);
            return VFS_ERROR;
        }

        (void)memset_s(partitionBuf, info->sector_size, 0, info->sector_size);
        ret = blkDrv->u.i_bops->read(blkDrv, (unsigned char *)partitionBuf, partitionStart, 1);
        if (ret != 1) { /* read failed */
            disk_log_err("%s %d\n", __FUNCTION__, __LINE__);
            return -EIO;
        }

        ret = OsGPTPartitionRecognitionSub(info, partitionBuf, partitionCount, partitionStart, partitionEnd);
        if (ret != ENOERR) {
            return VFS_ERROR;
        }
    }

    return ret;
}

static int32_t DiskGPTPartitionRecognition(struct inode *blkDrv, struct disk_divide_info *info)
{
    char *gptBuf = NULL;
    char *partitionBuf = NULL;
    uint32_t tableNum, i, index;
    uint32_t partitionCount = 0;
    int32_t ret;
    ret = DiskPartitionMemZalloc(MEM_ADDR_ALIGN_BYTE, info->sector_size, &gptBuf, &partitionBuf);
    if (ret != ENOERR) {
        return ret;
    }

    ret = GPTInfoGet(blkDrv, gptBuf);
    if (ret < 0) {
        goto OUT_WITH_MEM;
    }

    tableNum = LD_DWORD_DISK(&gptBuf[TABLE_NUM_OFFSET]);
    if (tableNum > TABLE_MAX_NUM) {
        tableNum = TABLE_MAX_NUM;
    }

    index = (tableNum % PAR_ENTRY_NUM_PER_SECTOR) ? ((tableNum / PAR_ENTRY_NUM_PER_SECTOR) + 1) :
                                                    (tableNum / PAR_ENTRY_NUM_PER_SECTOR);

    for (i = 0; i < index; i++) {
        (void)memset_s(gptBuf, info->sector_size, 0, info->sector_size);
        ret = blkDrv->u.i_bops->read(blkDrv, (unsigned char *)gptBuf, TABLE_START_SECTOR + i, 1);
        if (ret != 1) { /* read failed */
            disk_log_err("%s %d\n", __FUNCTION__, __LINE__);
            ret = -EIO;
            goto OUT_WITH_MEM;
        }

        ret = OsGPTPartitionRecognition(blkDrv, info, gptBuf, partitionBuf, &partitionCount);
        if (ret < 0) {
            if (ret == VFS_ERROR) {
                ret = (int32_t)partitionCount;
            }
            goto OUT_WITH_MEM;
        }
    }
    ret = (int32_t)partitionCount;

OUT_WITH_MEM:
    dpal_free(gptBuf);
    dpal_free(partitionBuf);
    return ret;
}
#endif

static int32_t OsMBRInfoGet(struct inode *blkDrv, char *mbrBuf)
{
    int32_t ret;

    /* read MBR, start from sector 0, length is 1 sector */
    ret = blkDrv->u.i_bops->read(blkDrv, (unsigned char *)mbrBuf, 0, 1);
    if (ret != 1) { /* read failed */
        disk_log_err("driver read return error: %d\n", ret);
        return -EIO;
    }

    /* Check boot record signature. */
    if (LD_WORD_DISK(&mbrBuf[BS_SIG55AA]) != BS_SIG55AA_VALUE) {
        return VFS_ERROR;
    }

    return ENOERR;
}

static int32_t OsEBRInfoGet(struct inode *blkDrv, const struct disk_divide_info *info,
                          char *ebrBuf, const char *mbrBuf)
{
    int32_t ret;

    if (VERIFY_FS(mbrBuf)) {
        if (info->sector_count <= LD_DWORD_DISK(&mbrBuf[PAR_OFFSET + PAR_START_OFFSET])) {
            return VFS_ERROR;
        }

        ret = blkDrv->u.i_bops->read(blkDrv, (unsigned char *)ebrBuf,
                                     LD_DWORD_DISK(&mbrBuf[PAR_OFFSET + PAR_START_OFFSET]), 1);
        if ((ret != 1) || (!VERIFY_FS(ebrBuf))) { /* read failed */
            disk_log_err("OsEBRInfoGet, verify_fs error, ret = %d\n", ret);
            return -EIO;
        }
    }

    return ENOERR;
}

static int32_t OsPrimaryPartitionRecognition(const char *mbrBuf, struct disk_divide_info *info,
                                           int32_t *extendedPos, int32_t *mbrCount)
{
    int32_t i;
    char mbrPartitionType;
    int32_t extendedFlag = 0;
    int32_t count = 0;

    for (i = 0; i < MAX_PRIMARY_PART_PER_DISK; i++) {
        mbrPartitionType = mbrBuf[PAR_OFFSET + PAR_TYPE_OFFSET + (i * PAR_TABLE_SIZE)];
        if (mbrPartitionType) {
            info->part[i].type = mbrPartitionType;
            info->part[i].sector_start = LD_DWORD_DISK(&mbrBuf[PAR_OFFSET + PAR_START_OFFSET + (i * PAR_TABLE_SIZE)]);
            info->part[i].sector_count = LD_DWORD_DISK(&mbrBuf[PAR_OFFSET + PAR_COUNT_OFFSET + (i * PAR_TABLE_SIZE)]);
            if ((mbrPartitionType == EXTENDED_PAR) || (mbrPartitionType == EXTENDED_8G)) {
                extendedFlag = 1;
                *extendedPos = i;
                continue;
            }
            count++;
        }
    }
    *mbrCount = count;

    return extendedFlag;
}

static int32_t OsLogicalPartitionRecognition(struct inode *blkDrv, struct disk_divide_info *info,
                                           uint32_t extendedAddress, char *ebrBuf, int32_t mbrCount)
{
    int32_t ret;
    uint32_t extendedOffset = 0;
    char ebrPartitionType;
    int32_t ebrCount = 0;

    do {
        (void)memset_s(ebrBuf, info->sector_size, 0, info->sector_size);
        if (((uint64_t)(extendedAddress) + extendedOffset) >= info->sector_count) {
            disk_log_err("extended partition is out of disk range: extendedAddress = %u, extendedOffset = %u\n",
                      extendedAddress, extendedOffset);
            break;
        }
        ret = blkDrv->u.i_bops->read(blkDrv, (unsigned char *)ebrBuf,
                                     extendedAddress + extendedOffset, 1);
        if (ret != 1) { /* read failed */
            disk_log_err("driver read return error: %d, extendedAddress = %u, extendedOffset = %u\n", ret,
                      extendedAddress, extendedOffset);
            return -EIO;
        }
        ebrPartitionType = ebrBuf[PAR_OFFSET + PAR_TYPE_OFFSET];
        if (ebrPartitionType && ((mbrCount + ebrCount) < MAX_DIVIDE_PART_PER_DISK)) {
            info->part[MAX_PRIMARY_PART_PER_DISK + ebrCount].type = ebrPartitionType;
            info->part[MAX_PRIMARY_PART_PER_DISK + ebrCount].sector_start = extendedAddress + extendedOffset +
                LD_DWORD_DISK(&ebrBuf[PAR_OFFSET + PAR_START_OFFSET]);
            info->part[MAX_PRIMARY_PART_PER_DISK + ebrCount].sector_count = LD_DWORD_DISK(&ebrBuf[PAR_OFFSET +
                                                                                          PAR_COUNT_OFFSET]);
            ebrCount++;
        }
        extendedOffset = LD_DWORD_DISK(&ebrBuf[PAR_OFFSET + PAR_START_OFFSET + PAR_TABLE_SIZE]);
    } while ((ebrBuf[PAR_OFFSET + PAR_TYPE_OFFSET + PAR_TABLE_SIZE] != 0) &&
             ((mbrCount + ebrCount) < MAX_DIVIDE_PART_PER_DISK));

    return ebrCount;
}

static int32_t DiskPartitionRecognition(struct inode *blkDrv, struct disk_divide_info *info)
{
    int32_t ret;
    int32_t extendedFlag;
    int32_t extendedPos = 0;
    int32_t mbrCount = 0;
    uint32_t extendedAddress;
    char *mbrBuf = NULL;
    char *ebrBuf = NULL;

    if ((blkDrv == NULL) || (blkDrv->u.i_bops == NULL) || (blkDrv->u.i_bops->read == NULL)) {
        return -EINVAL;
    }

    ret = DiskPartitionMemZalloc(MEM_ADDR_ALIGN_BYTE, info->sector_size, &mbrBuf, &ebrBuf);
    if (ret != ENOERR) {
        return ret;
    }

    ret = OsMBRInfoGet(blkDrv, mbrBuf);
    if (ret < 0) {
        goto OUT_WITH_MEM;
    }

#ifdef CONFIG_FS_FAT_GPT
    /* The partition type is GPT */
    if (mbrBuf[PARTION_MODE_BTYE] == (char)PARTION_MODE_GPT) {
        ret = DiskGPTPartitionRecognition(blkDrv, info);
        goto OUT_WITH_MEM;
    }
#endif

    ret = OsEBRInfoGet(blkDrv, info, ebrBuf, mbrBuf);
    if (ret < 0) {
        ret = 0; /* no mbr */
        goto OUT_WITH_MEM;
    }

    extendedFlag = OsPrimaryPartitionRecognition(mbrBuf, info, &extendedPos, &mbrCount);
    if (extendedFlag) {
        extendedAddress = LD_DWORD_DISK(&mbrBuf[PAR_OFFSET + PAR_START_OFFSET + (extendedPos * PAR_TABLE_SIZE)]);
        ret = OsLogicalPartitionRecognition(blkDrv, info, extendedAddress, ebrBuf, mbrCount);
        if (ret <= 0) {
            goto OUT_WITH_MEM;
        }
    }
    ret += mbrCount;

OUT_WITH_MEM:
    dpal_free(ebrBuf);
    dpal_free(mbrBuf);
    return ret;
}

int32_t DiskPartitionRegister(los_disk *disk)
{
    int32_t count;
    uint32_t i, partSize;
    los_part *part = NULL;
    struct disk_divide_info parInfo;

    /* Fill disk_divide_info structure to set partition's infomation. */
    (void)memset_s(parInfo.part, sizeof(parInfo.part), 0, sizeof(parInfo.part));
    partSize = sizeof(parInfo.part) / sizeof(parInfo.part[0]);

    parInfo.sector_size = disk->sector_size;
    parInfo.sector_count = disk->sector_count;
    count = DiskPartitionRecognition(disk->dev, &parInfo);
    if (count == VFS_ERROR) {
        part = get_part(DiskAddPart(disk, 0, disk->sector_count, FALSE));
        if (part == NULL) {
            return VFS_ERROR;
        }
        part->part_no_mbr = 0;
        printf("Disk %s doesn't contain a valid partition table.\n", disk->disk_name);
        return ENOERR;
    } else if (count < 0) {
        return VFS_ERROR;
    }

    parInfo.part_count = count;
    if (count == 0) {
        part = get_part(DiskAddPart(disk, 0, disk->sector_count, TRUE));
        if (part == NULL) {
            return VFS_ERROR;
        }
        part->part_no_mbr = 0;

        printf("No MBR detected.\n");
        return ENOERR;
    }

    for (i = 0; i < partSize; i++) {
        /* Read the disk_divide_info structure to get partition's infomation. */
        if ((parInfo.part[i].type != 0) && (parInfo.part[i].type != EXTENDED_PAR) &&
            (parInfo.part[i].type != EXTENDED_8G)) {
            part = get_part(DiskAddPart(disk, parInfo.part[i].sector_start, parInfo.part[i].sector_count, TRUE));
            if (part == NULL) {
                return VFS_ERROR;
            }
            part->part_no_mbr = i + 1;
            part->filesystem_type = parInfo.part[i].type;
        }
    }

    return ENOERR;
}

int32_t los_disk_read(int32_t drvId, void *buf, uint64_t sector, uint32_t count)
{
#ifdef CONFIG_FS_FAT_CACHE
    uint32_t len;
#endif
    int32_t result = VFS_ERROR;
    los_disk *disk = get_disk(drvId);

    if ((buf == NULL) || (count == 0)) { /* buff equal to NULL or count equal to 0 */
        return result;
    }

    if (disk == NULL) {
        return result;
    }

    DISK_LOCK(disk);

    if (disk->disk_status != STAT_INUSED) {
        goto ERROR_HANDLE;
    }

    if ((count > disk->sector_count) || ((disk->sector_count - count) < sector)) {
        goto ERROR_HANDLE;
    }

#ifdef CONFIG_FS_FAT_CACHE
    if (disk->bcache != NULL) {
        if (((uint64_t)(disk->bcache->sectorSize) * count) > UINT_MAX) {
            goto ERROR_HANDLE;
        }
        len = disk->bcache->sectorSize * count;
        result = BlockCacheRead(disk->bcache, (unsigned char *)buf, &len, 0, sector);
        if (result != ENOERR) {
            disk_log_err("los_disk_read read err = %d, sector = %llu, len = %u\n", result, sector, len);
        }
    } else {
#endif
        if ((disk->dev != NULL) && (disk->dev->u.i_bops != NULL) && (disk->dev->u.i_bops->read != NULL)) {
            result = disk->dev->u.i_bops->read(disk->dev, (unsigned char *)buf, sector, count);
            if (result == (int32_t)count) {
                result = ENOERR;
            }
        }
#ifdef CONFIG_FS_FAT_CACHE
    }
#endif

    if (result != ENOERR) {
        goto ERROR_HANDLE;
    }

    DISK_UNLOCK(disk);
    return ENOERR;

ERROR_HANDLE:
    DISK_UNLOCK(disk);
    return VFS_ERROR;
}

int32_t los_disk_write(int32_t drvId, const void *buf, uint64_t sector, uint32_t count)
{
#ifdef CONFIG_FS_FAT_CACHE
    uint32_t len;
#endif
    int32_t result = VFS_ERROR;
    los_disk *disk = get_disk(drvId);
    if (disk == NULL) {
        return result;
    }

    if ((buf == NULL) || (count == 0)) { /* buff equal to NULL or count equal to 0 */
        return result;
    }

    DISK_LOCK(disk);

    if (disk->disk_status != STAT_INUSED) {
        goto ERROR_HANDLE;
    }

    if ((count > disk->sector_count) || ((disk->sector_count - count) < sector)) {
        goto ERROR_HANDLE;
    }

#ifdef CONFIG_FS_FAT_CACHE
    if (disk->bcache != NULL) {
        if (((uint64_t)(disk->bcache->sectorSize) * count) > UINT_MAX) {
            goto ERROR_HANDLE;
        }
        len = disk->bcache->sectorSize * count;
        result = BlockCacheWrite(disk->bcache, (const unsigned char *)buf, &len, 0, sector);
        if (result != ENOERR) {
            disk_log_err("los_disk_write write err = %d, sector = %llu, len = %u\n", result, sector, len);
        }
    } else {
#endif
        if ((disk->dev != NULL) && (disk->dev->u.i_bops != NULL) && (disk->dev->u.i_bops->write != NULL)) {
            result = disk->dev->u.i_bops->write(disk->dev, (unsigned char *)buf, sector, count);
            if (result == (int32_t)count) {
                result = ENOERR;
            }
        }
#ifdef CONFIG_FS_FAT_CACHE
    }
#endif

    if (result != ENOERR) {
        goto ERROR_HANDLE;
    }

    DISK_UNLOCK(disk);
    return ENOERR;

ERROR_HANDLE:
    DISK_UNLOCK(disk);
    return VFS_ERROR;
}

int32_t los_disk_ioctl(int32_t drvId, int32_t cmd, void *buf)
{
    struct geometry info;
    los_disk *disk = get_disk(drvId);
    if (disk == NULL) {
        return VFS_ERROR;
    }

    DISK_LOCK(disk);

    if ((disk->dev == NULL) || (disk->disk_status != STAT_INUSED)) {
        goto ERROR_HANDLE;
    }

    if (cmd == DISK_CTRL_SYNC) {
        DISK_UNLOCK(disk);
        return ENOERR;
    }

    if (buf == NULL) {
        goto ERROR_HANDLE;
    }

    (void)memset_s(&info, sizeof(info), 0, sizeof(info));
    if ((disk->dev->u.i_bops == NULL) || (disk->dev->u.i_bops->geometry == NULL) ||
        (disk->dev->u.i_bops->geometry(disk->dev, &info) != 0)) {
        goto ERROR_HANDLE;
    }

    if (cmd == DISK_GET_SECTOR_COUNT) {
        *(uint64_t *)buf = info.geo_nsectors;
        if (info.geo_nsectors == 0) {
            goto ERROR_HANDLE;
        }
    } else if (cmd == DISK_GET_SECTOR_SIZE) {
        *(size_t *)buf = info.geo_sectorsize;
    } else if (cmd == DISK_GET_BLOCK_SIZE) { /* Get erase block size in unit of sectors (uint32_t) */
        /* Block Num SDHC == 512, SD can be set to 512 or other */
        if (info.geo_nsectors == 0) {
            goto ERROR_HANDLE;
        }
        *(size_t *)buf = DISK_MAX_SECTOR_SIZE / info.geo_sectorsize;
    } else {
        goto ERROR_HANDLE;
    }

    DISK_UNLOCK(disk);
    return ENOERR;

ERROR_HANDLE:
    DISK_UNLOCK(disk);
    return VFS_ERROR;
}

int32_t los_part_read(int32_t pt, void *buf, uint64_t sector, uint32_t count)
{
    const los_part *part = get_part(pt);
    los_disk *disk = NULL;
    int32_t ret;

    if (part == NULL) {
        return VFS_ERROR;
    }

    disk = get_disk((int32_t)part->disk_id);
    if (disk == NULL) {
        return VFS_ERROR;
    }

    DISK_LOCK(disk);
    if ((part->dev == NULL) || (disk->disk_status != STAT_INUSED)) {
        goto ERROR_HANDLE;
    }

    if (count > part->sector_count) {
        disk_log_err("los_part_read failed, invaild count, count = %u\n", count);
        goto ERROR_HANDLE;
    }

    /* Read from absolute sector. */
    if (part->type == EMMC) {
        if ((disk->sector_count - part->sector_start) > sector) {
            sector += part->sector_start;
        } else {
            disk_log_err("los_part_read failed, invaild sector, sector = %llu\n", sector);
            goto ERROR_HANDLE;
        }
    }

    if ((sector >= GetFirstPartStart(part)) &&
        (((sector + count) > (part->sector_start + part->sector_count)) || (sector < part->sector_start))) {
        disk_log_err("los_part_read error, sector = %llu, count = %u, part->sector_start = %llu, "
                  "part->sector_count = %llu\n", sector, count, part->sector_start, part->sector_count);
        goto ERROR_HANDLE;
    }

    DISK_UNLOCK(disk);

    ret = los_disk_read((int32_t)part->disk_id, buf, sector, count);
    if (ret < 0) {
        return VFS_ERROR;
    }

    return ENOERR;

ERROR_HANDLE:
    DISK_UNLOCK(disk);
    return VFS_ERROR;
}

int32_t los_part_write(int32_t pt, const void *buf, uint64_t sector, uint32_t count)
{
    const los_part *part = get_part(pt);
    los_disk *disk = NULL;
    int32_t ret;

    if (part == NULL) {
        return VFS_ERROR;
    }

    disk = get_disk((int32_t)part->disk_id);
    if (disk == NULL) {
        return VFS_ERROR;
    }

    DISK_LOCK(disk);
    if ((part->dev == NULL) || (disk->disk_status != STAT_INUSED)) {
        goto ERROR_HANDLE;
    }

    if (count > part->sector_count) {
        disk_log_err("los_part_write failed, invaild count, count = %u\n", count);
        goto ERROR_HANDLE;
    }

    /* Write to absolute sector. */
    if (part->type == EMMC) {
        if ((disk->sector_count - part->sector_start) > sector) {
            sector += part->sector_start;
        } else {
            disk_log_err("los_part_write failed, invaild sector, sector = %llu\n", sector);
            goto ERROR_HANDLE;
        }
    }

    if ((sector >= GetFirstPartStart(part)) &&
        (((sector + count) > (part->sector_start + part->sector_count)) || (sector < part->sector_start))) {
        disk_log_err("los_part_write, sector = %llu, count = %u, part->sector_start = %llu, "
                  "part->sector_count = %llu\n", sector, count, part->sector_start, part->sector_count);
        goto ERROR_HANDLE;
    }

    DISK_UNLOCK(disk);

    ret = los_disk_write((int32_t)part->disk_id, (const void *)buf, sector, count);
    if (ret < 0) {
        goto ERROR_HANDLE;
    }

    return ENOERR;

ERROR_HANDLE:
    DISK_UNLOCK(disk);
    return VFS_ERROR;
}

#define GET_ERASE_BLOCK_SIZE 0x2

int32_t los_part_ioctl(int32_t pt, int32_t cmd, void *buf)
{
    struct geometry info;
    los_part *part = get_part(pt);
    los_disk *disk = NULL;

    if (part == NULL) {
        return VFS_ERROR;
    }

    disk = get_disk((int32_t)part->disk_id);
    if (disk == NULL) {
        return VFS_ERROR;
    }

    DISK_LOCK(disk);
    if ((part->dev == NULL) || (disk->disk_status != STAT_INUSED)) {
        goto ERROR_HANDLE;
    }

    if (cmd == DISK_CTRL_SYNC) {
        DISK_UNLOCK(disk);
        return ENOERR;
    }

    if (buf == NULL) {
        goto ERROR_HANDLE;
    }

    (void)memset_s(&info, sizeof(info), 0, sizeof(info));
    if ((part->dev->u.i_bops == NULL) || (part->dev->u.i_bops->geometry == NULL) ||
        (part->dev->u.i_bops->geometry(part->dev, &info) != 0)) {
        goto ERROR_HANDLE;
    }

    if (cmd == DISK_GET_SECTOR_COUNT) {
        *(uint64_t *)buf = part->sector_count;
        if (*(uint64_t *)buf == 0) {
            goto ERROR_HANDLE;
        }
    } else if (cmd == DISK_GET_SECTOR_SIZE) {
        *(size_t *)buf = info.geo_sectorsize;
    } else if (cmd == DISK_GET_BLOCK_SIZE) { /* Get erase block size in unit of sectors (uint32_t) */
        if ((part->dev->u.i_bops->ioctl == NULL) ||
            (part->dev->u.i_bops->ioctl(part->dev, GET_ERASE_BLOCK_SIZE, (uintptr_t)buf) != 0)) {
            goto ERROR_HANDLE;
        }
    } else {
        goto ERROR_HANDLE;
    }

    DISK_UNLOCK(disk);
    return ENOERR;

ERROR_HANDLE:
    DISK_UNLOCK(disk);
    return VFS_ERROR;
}

#ifdef CONFIG_FS_FAT_CACHE
static void DiskCacheThreadInit(uint32_t diskId, OsBcache *bc)
{
    bc->prereadFun = NULL;

    if (GetDiskUsbStatus(diskId) == FALSE) {
#ifdef CONFIG_FS_FAT_CACHE_SYNC_THREAD
        BcacheSyncThreadInit(bc, diskId);
#endif
    }

    if (OsReHookFuncAddDiskRef != NULL) {
        (void)OsReHookFuncAddDiskRef((StorageHookFunction)OsSdSync, (void *)0);
        (void)OsReHookFuncAddDiskRef((StorageHookFunction)OsSdSync, (void *)1);
    }
}

static OsBcache *DiskCacheInit(uint32_t sectorSize, uint64_t nsectors, struct inode *blkDriver)
{
#define SECTOR_SIZE 512

    OsBcache *bc = NULL;
    uint32_t sectorPerBlock;
    uint32_t div = sectorSize / SECTOR_SIZE;
    if (div != 0) {
        sectorPerBlock = g_uwFatSectorsPerBlock / div;
        if (sectorPerBlock != 0) {
            bc = BlockCacheInit(blkDriver, sectorSize, sectorPerBlock,
                                g_uwFatBlockNums, nsectors / sectorPerBlock);
        }
    }

    if (bc == NULL) {
        disk_log_err("disk_init : disk have not init bcache cache!\n");
        return NULL;
    }
    return bc;
}

static void DiskCacheDeinit(los_disk *disk)
{
    uint32_t diskId = disk->disk_id;
    if (GetDiskUsbStatus(diskId) == FALSE) {
#ifdef CONFIG_FS_FAT_CACHE_SYNC_THREAD
        BcacheSyncThreadDeinit(disk->bcache);
#endif
    }

    BlockCacheDeinit(disk->bcache);
    disk->bcache = NULL;

    if (OsReHookFuncDelDiskRef != NULL) {
        (void)OsReHookFuncDelDiskRef((StorageHookFunction)OsSdSync);
    }
}
#endif

static void DiskStructInit(const char *diskName, int32_t diskId, const struct geometry *diskInfo,
                           struct inode *blkDriver, los_disk *disk)
{
    disk->disk_id = diskId;
    disk->dev = blkDriver;
    disk->sector_start = 0;
    disk->sector_size = diskInfo->geo_sectorsize;
    disk->sector_count = diskInfo->geo_nsectors;
    if (strncpy_s(disk->disk_name, DISK_NAME + 1, diskName, strlen(diskName)) != EOK) {
        disk_log_err("DiskStructInit strncpy_s failed.\n");
        return;
    }
    dpal_list_init(&disk->head);
}

static int32_t DiskDivideAndPartitionRegister(struct disk_divide_info *info, los_disk *disk)
{
    int32_t ret;

    if (info != NULL) {
        ret = DiskDivide(disk, info);
        if (ret != ENOERR) {
            disk_log_err("DiskDivide failed, ret = %d\n", ret);
            return ret;
        }
    } else {
        ret = DiskPartitionRegister(disk);
        if (ret != ENOERR) {
            disk_log_err("DiskPartitionRegister failed, ret = %d\n", ret);
            return ret;
        }
    }
    return ENOERR;
}

static int32_t DiskDeinit(los_disk *disk)
{
    los_part *part = NULL;
    char devName[DEV_NAME_BUFF_SIZE];
    int32_t ret;

    if (dpal_list_empty(&disk->head) == FALSE) {
        part = DPAL_DL_LIST_ENTRY(disk->head.next, los_part, list);
        while (&part->list != &disk->head) {
            ret = snprintf_s(devName, sizeof(devName), sizeof(devName) - 1, "%s%c%d",
                             disk->disk_name, 'p', disk->part_count - 1);
            if (ret < 0) {
                return -ENAMETOOLONG;
            }
            DiskPartDelFromDisk(disk, part);
            (void)unregister_blockdriver(devName);
            DiskPartRelease(part);

            part = DPAL_DL_LIST_ENTRY(disk->head.next, los_part, list);
        }
    }

    DISK_LOCK(disk);

#ifdef CONFIG_FS_FAT_CACHE
    DiskCacheDeinit(disk);
#endif

    disk->dev = NULL;
    DISK_UNLOCK(disk);

    (void)unregister_blockdriver(disk->disk_name);
    disk->disk_status = STAT_UNUSED;
    disk->disk_name[0] = '\0';

    return ENOERR;
}

void OsDiskInit(void)
{
    int32_t diskId;
    uint32_t ret;
    los_disk *disk = NULL;
    for (diskId = 0; diskId < SYS_MAX_DISK; diskId++) {
        disk = get_disk(diskId);
        disk->disk_status = STAT_UNUSED;
        ret = dpal_mux_create(&disk->disk_mutex);
        if (ret != DPAL_OK) {
            disk_log_err("%s %d, mutex created failed, error:%u\n", __FUNCTION__, __LINE__, ret);
        }
    }
}

static void OsDiskInitSub(const char *diskName, int32_t diskId, los_disk *disk,
                          const struct geometry *diskInfo, struct inode *blkDriver)
{
#ifdef CONFIG_FS_FAT_CACHE
    OsBcache *bc = DiskCacheInit(diskInfo->geo_sectorsize, diskInfo->geo_nsectors, blkDriver);
    if (bc != NULL) {
        DiskCacheThreadInit(diskId, bc);
    }
    disk->bcache = bc;
#endif

    DiskStructInit(diskName, diskId, diskInfo, blkDriver, disk);
}

int32_t los_disk_init(const char *diskName, const struct block_operations *bops,
                    void *priv, int32_t diskId, void *info)
{
    struct inode_search_s desc;
    struct geometry diskInfo;
    struct inode *blkDriver = NULL;
    los_disk *disk = get_disk(diskId);
    int ret;

    if ((diskName == NULL) || (disk == NULL) ||
        (disk->disk_status != STAT_UNREADY) || (strnlen(diskName, DISK_NAME + 1) > DISK_NAME)) {
        return VFS_ERROR;
    }

    if (register_blockdriver(diskName, bops, RWE_RW_RW, priv) != 0) {
        disk_log_err("disk_init : register %s fail!\n", diskName);
        return VFS_ERROR;
    }

    SETUP_SEARCH(&desc, diskName, false);
    ret = inode_find(&desc);
    if (ret < 0) {
        disk_log_err("disk_init : find %s fail!\n", diskName);
        goto DISK_FIND_ERROR;
    }
    blkDriver = desc.node;

    if ((blkDriver->u.i_bops == NULL) || (blkDriver->u.i_bops->geometry == NULL) ||
        (blkDriver->u.i_bops->geometry(blkDriver, &diskInfo) != 0)) {
        goto DISK_BLKDRIVER_ERROR;
    }
    if (diskInfo.geo_sectorsize < DISK_MAX_SECTOR_SIZE) {
        goto DISK_BLKDRIVER_ERROR;
    }

    printf("disk_init : register %s ok!\n", diskName);

    OsDiskInitSub(diskName, diskId, disk, &diskInfo, blkDriver);
    inode_release(blkDriver);

    if (DiskDivideAndPartitionRegister(info, disk) != ENOERR) {
        (void)DiskDeinit(disk);
        return VFS_ERROR;
    }

    disk->disk_status = STAT_INUSED;
    RELEASE_SEARCH(&desc);

    return ENOERR;

DISK_BLKDRIVER_ERROR:
    disk_log_err("disk_init : register %s ok but get disk info fail!\n", diskName);
    inode_release(blkDriver);
DISK_FIND_ERROR:
    RELEASE_SEARCH(&desc);
    (void)unregister_blockdriver(diskName);
    return VFS_ERROR;
}

int32_t los_disk_deinit(int32_t diskId)
{
    los_disk *disk = get_disk(diskId);
    if (disk == NULL) {
        return -EINVAL;
    }

    DISK_LOCK(disk);

    if (disk->disk_status != STAT_INUSED) {
        DISK_UNLOCK(disk);
        return -EINVAL;
    }

    disk->disk_status = STAT_UNREADY;
    DISK_UNLOCK(disk);

    return DiskDeinit(disk);
}

int32_t los_disk_sync(int32_t drvId)
{
    int32_t ret = ENOERR;
    los_disk *disk = get_disk(drvId);
    if (disk == NULL) {
        return EINVAL;
    }

    DISK_LOCK(disk);
    if (disk->disk_status != STAT_INUSED) {
        DISK_UNLOCK(disk);
        return EINVAL;
    }

#ifdef CONFIG_FS_FAT_CACHE
    if (disk->bcache != NULL) {
        ret = BlockCacheSync(disk->bcache);
    }
#endif

    DISK_UNLOCK(disk);
    return ret;
}

int32_t los_disk_set_bcache(int32_t drvId, uint32_t sectorPerBlock, uint32_t blockNum)
{
#ifdef CONFIG_FS_FAT_CACHE

    int32_t ret;
    uint32_t intSave;
    OsBcache *bc = NULL;
    los_disk *disk = get_disk(drvId);
    if ((disk == NULL) || (sectorPerBlock == 0)) {
        return EINVAL;
    }

    if (((sectorPerBlock % UNSIGNED_INTEGER_BITS) != 0) ||
        (sectorPerBlock > CONFIG_FS_FAT_MAX_SECTOR_PER_BLOCK)) {
            return EINVAL;
        }

    DISK_LOCK(disk);

    if (disk->disk_status != STAT_INUSED) {
        goto ERROR_HANDLE;
    }

    if (disk->bcache != NULL) {
        ret = BlockCacheSync(disk->bcache);
        if (ret != ENOERR) {
            DISK_UNLOCK(disk);
            return ret;
        }
    }

    dpal_spin_lock_irqsave(&g_diskFatBlockSpinlock, &intSave);
    DiskCacheDeinit(disk);

    g_uwFatBlockNums = blockNum;
    g_uwFatSectorsPerBlock = sectorPerBlock;

    bc = DiskCacheInit(disk->sector_size, disk->sector_count, disk->dev);
    if ((bc == NULL) && (blockNum != 0)) {
        dpal_spin_unlock_irqrestore(&g_diskFatBlockSpinlock, &intSave);
        DISK_UNLOCK(disk);
        return ENOMEM;
    }

    if (bc != NULL) {
        DiskCacheThreadInit((uint32_t)drvId, bc);
    }

    disk->bcache = bc;
    dpal_spin_unlock_irqrestore(&g_diskFatBlockSpinlock, &intSave);
    DISK_UNLOCK(disk);
    return ENOERR;

ERROR_HANDLE:
    DISK_UNLOCK(disk);
    return EINVAL;
#else
    return VFS_ERROR;
#endif
}

static los_part *OsPartFind(los_disk *disk, const struct inode *blkDriver)
{
    los_part *part = NULL;

    DISK_LOCK(disk);
    if ((disk->disk_status != STAT_INUSED) || (dpal_list_empty(&disk->head) == TRUE)) {
        goto EXIT;
    }
    part = DPAL_DL_LIST_ENTRY(disk->head.next, los_part, list);
    if (disk->dev == blkDriver) {
        goto EXIT;
    }

    while (&part->list != &disk->head) {
        if (part->dev == blkDriver) {
            goto EXIT;
        }
        part = DPAL_DL_LIST_ENTRY(part->list.next, los_part, list);
    }
    part = NULL;

EXIT:
    DISK_UNLOCK(disk);
    return part;
}

los_part *los_part_find(const struct inode *blkDriver)
{
    int32_t i;
    los_disk *disk = NULL;
    los_part *part = NULL;

    if (blkDriver == NULL) {
        return NULL;
    }

    for (i = 0; i < SYS_MAX_DISK; i++) {
        disk = get_disk(i);
        if (disk == NULL) {
            continue;
        }
        part = OsPartFind(disk, blkDriver);
        if (part != NULL) {
            return part;
        }
    }

    return NULL;
}

static uint32_t GetDiskNameStart(const char *name)
{
    uint32_t i;
    uint32_t off = 0;
    for (i = 0; i < strlen(name); i++) {
        if (name[i] == '/') {
            off = i + 1;
        }
    }
    return off;
}

BOOL IsBlockStatusReady(const struct inode *blkDriver)
{
    los_disk *disk = NULL;
    int32_t i;
    uint32_t off;

    if (blkDriver == NULL) {
        return FALSE;
    }

    for (i = 0; i < SYS_MAX_DISK; i++) {
        disk = get_disk(i);
        if ((disk == NULL) || (strlen(disk->disk_name) == 0)) {
            continue;
        }
        off = GetDiskNameStart(disk->disk_name); /* get the diskname removed "/dev/" */
        if (strstr(blkDriver->i_name, disk->disk_name + off) == NULL) {
            continue;
        }
        if (OsPartFind(disk, blkDriver) != NULL) {
            return (disk->disk_status == STAT_INUSED) ? TRUE : FALSE;
        } else {
            return FALSE;
        }
    }

    return TRUE;
}

int32_t los_part_access(const char *dev, mode_t mode)
{
    struct inode_search_s desc;
    los_part *part = NULL;
    int ret;

    (void)mode;

    SETUP_SEARCH(&desc, dev, false);
    ret = inode_find(&desc);
    if (ret < 0) {
        RELEASE_SEARCH(&desc);
        return VFS_ERROR;
    }

    part = los_part_find(desc.node);
    inode_release(desc.node);
    if (part == NULL) {
        return VFS_ERROR;
    }

    RELEASE_SEARCH(&desc);
    return ENOERR;
}

int32_t SetDiskPartName(los_part *part, const char *src)
{
    size_t len;
    los_disk *disk = NULL;

    if ((part == NULL) || (src == NULL)) {
        return VFS_ERROR;
    }

    len = strlen(src);
    if ((len == 0) || (len >= DISK_NAME)) {
        return VFS_ERROR;
    }

    disk = get_disk((int32_t)part->disk_id);
    if (disk == NULL) {
        return VFS_ERROR;
    }

    DISK_LOCK(disk);
    if (disk->disk_status != STAT_INUSED) {
        goto ERROR_HANDLE;
    }

    part->part_name = (char *)dpal_zalloc(len + 1);
    if (part->part_name == NULL) {
        disk_log_err("%s[%d] zalloc failure\n", __FUNCTION__, __LINE__);
        goto ERROR_HANDLE;
    }

    (void)strcpy_s(part->part_name, len + 1, src);

    DISK_UNLOCK(disk);
    return ENOERR;

ERROR_HANDLE:
    DISK_UNLOCK(disk);
    return VFS_ERROR;
}

int32_t add_mmc_partition(struct disk_divide_info *info, size_t sectorStart, size_t sectorCount)
{
    uint32_t index, i;

    if (info == NULL) {
        return VFS_ERROR;
    }

    if ((info->part_count >= MAX_DIVIDE_PART_PER_DISK) || (sectorCount == 0)) {
        return VFS_ERROR;
    }

    if ((sectorCount > info->sector_count) || ((info->sector_count - sectorCount) < sectorStart)) {
        return VFS_ERROR;
    }

    index = info->part_count;
    for (i = 0; i < index; i++) {
        if (sectorStart < (info->part[i].sector_start + info->part[i].sector_count)) {
            return VFS_ERROR;
        }
    }

    info->part[index].sector_start = sectorStart;
    info->part[index].sector_count = sectorCount;
    info->part[index].type = EMMC;
    info->part_count++;

    return ENOERR;
}

int32_t del_mmc_partition(struct disk_divide_info *info, size_t sectorStart, size_t sectorCount)
{
    if (info == NULL) {
        return VFS_ERROR;
    }

    if ((info->part_count > MAX_DIVIDE_PART_PER_DISK) || (info->part_count == 0) || (sectorCount == 0)) {
        return VFS_ERROR;
    }

    if ((info->sector_count < sectorCount) || ((info->sector_count - sectorCount) < sectorStart)) {
        return VFS_ERROR;
    }

    uint32_t index = info->part_count - 1;
    if ((info->part[index].sector_start != sectorStart) ||
        (info->part[index].sector_count != sectorCount)) {
        return VFS_ERROR;
    }

    info->part[index].sector_start = 0;
    info->part[index].sector_count = 0;
    info->part[index].type = 0;
    info->part_count--;

    return ENOERR;
}

void show_part(const los_part *part)
{
    if ((part == NULL) || (part->dev == NULL)) {
        disk_log_err("part is NULL\n");
        return;
    }
#if defined(CONFIG_SHELL)
    printf("\npart info :\n");
    printf("disk id          : %u\n", part->disk_id);
    printf("part_id in system: %u\n", part->part_id);
    printf("part no in disk  : %u\n", part->part_no_disk);
    printf("part no in mbr   : %u\n", part->part_no_mbr);
    printf("part filesystem  : %02X\n", part->filesystem_type);
    printf("part dev name    : %s\n", part->dev->i_name);
    printf("part sec start   : %llu\n", part->sector_start);
    printf("part sec count   : %llu\n", part->sector_count);
#endif
}

int32_t EraseDiskByID(uint32_t diskId, size_t startSector, uint32_t sectors)
{
    int32_t ret = OK;
#ifdef CONFIG_DRIVERS_MMC
    los_disk *disk = get_disk((int32_t)diskId);
    if ((disk != NULL) && (disk->dev != NULL)) {
        ret = (int32_t)mmcsd_erase(disk->dev, startSector, sectors);
    }
#endif

    return ret;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
