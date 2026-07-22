/*-----------------------------------------------------------------------*/
/* Low level disk I/O module SKELETON for FatFs     (C)ChaN, 2019        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "diskio.h"		/* Declarations of disk functions */
#include "fs/fs.h"
#include "string.h"
#include "disk.h"

/* Definitions of physical drive number for each drive */
#define DEV_RAM		0	/* Example: Map Ramdisk to physical drive 0 */
#define DEV_MMC		1	/* Example: Map MMC/SD card to physical drive 1 */
#define DEV_USB		2	/* Example: Map USB MSD to physical drive 2 */


/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	BYTE pdrv		/* Physical drive nmuber to identify the drive */
)
{
	return 0;
}



/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber to identify the drive */
)
{
	return 0;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
	BYTE pdrv,		/* Physical drive nmuber to identify the drive */
	BYTE *buff,		/* Data buffer to store read data */
	LBA_t sector,	/* Start sector in LBA */
	UINT count		/* Number of sectors to read */
)
{
	int result;

	result = los_part_read((int)pdrv, (void*)buff, sector, (UINT32)count);

	if (result == 0)
		return RES_OK;
	else
		return RES_ERROR;
}

DRESULT disk_raw_read (int id, void *buff, LBA_t sector, UINT32 count)
{
	int result;

	result = los_disk_read(id, buff, sector, count);

	if (result == 0)
		return RES_OK;
	else
		return RES_ERROR;
}


/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

DRESULT disk_write (
	BYTE pdrv,			/* Physical drive nmuber to identify the drive */
	const BYTE *buff,	/* Data to be written */
	LBA_t sector,		/* Start sector in LBA */
	UINT count			/* Number of sectors to write */
)
{
	int result;

	result = los_part_write((int)pdrv, (void*)buff, sector, (UINT32)count);

	if (result == 0)
		return RES_OK;
	else
		return RES_ERROR;
}

DRESULT disk_raw_write(int id, const void *buff, LBA_t sector, UINT32 count){
	int result;
	const void *uwBuff = buff;

	result = los_disk_write(id, (const void*)uwBuff, sector, count);

	if (result == 0)
		return RES_OK;
	else
		return RES_ERROR;
}

/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

DRESULT disk_ioctl (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
	int result;

	result = los_part_ioctl((int)pdrv, (int)cmd, buff);

	if (result == 0)
		return RES_OK;
	else
		return RES_ERROR;
}

DWORD fattime(time_t sec)
{
	time_t seconds = sec;
	struct tm local_time = {0};

	if (localtime_r(&seconds, &local_time) == NULL)
		return 0;
	if ((local_time.tm_year + 1900) < 1980) {	/* year must start at 1980 */
		return 0;
	}

	/* get system time */
	return  ((DWORD)(local_time.tm_year - 80) << 25) |
			((DWORD)(local_time.tm_mon + 1) << 21) |
			((DWORD)local_time.tm_mday << 16) |
			((DWORD)local_time.tm_hour << 11) |
			((DWORD)local_time.tm_min << 5) |
			((DWORD)local_time.tm_sec >> 1);
}

DWORD get_fattime (void)
{
	time_t seconds = time(NULL);
	return fattime(seconds);
}

DWORD time2fat(const struct timespec *ts)
{
	return fattime(ts->tv_sec);
}

time_t fat2time(DWORD tm)
{
	struct tm ftm;
	INT year, mon, day, hour, min, sec;
	WORD mtime;

	mtime = tm >> 16;
	day = mtime & 0x1F;		/* bit[4:0] Day(1..31) */
	mtime >>= 5;
	mon = mtime & 0x0F;		/* bit[8:5] Month(1..12) */
	mtime >>= 4;
	year = (mtime & 0x7F) + 1980;	/* bit[15:9] Year since 1980(0..127) */

	mtime = tm;
	sec = (mtime & 0x1F) * 2;	/* bit[4:0] Second/2(0..29) */
	mtime >>= 5;
	min = mtime & 0x3F;		/* bit[10:5] Minute(0..59) */
	mtime >>= 6;
	hour = mtime & 0x1F;		/* bit[15:11] Hour(0..23) */

	(void)memset_s(&ftm, sizeof(ftm), 0, sizeof(ftm));
	ftm.tm_year = year - 1900;	/* Years since 1900 */
	ftm.tm_mon  = mon - 1;		/* Months since January: 0-11 */
	ftm.tm_mday = day;		/* Day: 1-31 */
	ftm.tm_hour = hour;		/* Hours: 0-23 */
	ftm.tm_min  = min;		/* Minutes: 0-59 */
	ftm.tm_sec  = sec;		/* Seconds: 0-59 */

	return mktime(&ftm);
}

