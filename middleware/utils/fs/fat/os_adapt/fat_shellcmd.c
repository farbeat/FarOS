/* ----------------------------------------------------------------------------
 * Copyright (c) @CompanyNameMagicTag 2013-2019. All rights reserved.
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

#if defined(CONFIG_SHELL) && defined(CONFIG_FS_FAT)
#include "stdlib.h"
#include "stdio.h"
#include "errno.h"
#include "fs/fs.h"
#include "fatfs.h"

int osShellCmdFormat(int argc, const char **argv)
{
	if (argc < 3) {
		printf("format error");
		printf("Usage  :\n");
		printf("        format <dev_inodename> <sectors> <option> <label>\n");
		printf("        dev_inodename : the name of dev\n");
		printf("        sectors       : Size of allocation unit in unit of byte or sector, ");
		printf("0 instead of default size\n");
		printf("        options       : Index of filesystem. 1 for FAT filesystem, ");
		printf("2 for FAT32 filesystem, 7 for any, 8 for erase\n");
		printf("        label         : The volume of device. It will be emptyed when this parameter is null\n");
		printf("Example:\n");
		printf("        format /dev/mmcblk0 128 2\n");

		set_errno(EINVAL);
		return 0;
	}

	if (argc == 4) {
		if (strncmp(argv[3], "NULL", strlen(argv[3])) == 0 || strncmp(argv[3], "null", strlen(argv[3])) == 0) {
		    set_label(NULL);
		} else {
		    set_label(argv[3]);
		}
	}

	if (format(argv[0], atoi(argv[1]), atoi(argv[2])) == 0) {
		printf("format %s Success \n", argv[0]);
	} else {
		printf("format failed errno:%d \n", errno);
	}

	return 0;
}

#ifdef CONFIG_FS_FAT_VIRTUAL_PARTITION
int osShellCmdVirstatfs(int argc, const char **argv)
{
	struct statfs sfs;
	unsigned long long toatl_size, free_size;
	int result;

	if (argc != 1) {
		printf("virstatfs error\n");
		printf("Usage :\n");
		printf("        virstatfs <virtual_entry>\n");
		printf("        virtual_entry     :   the path of the virtual partition entry\n");
		printf("Example:\n");
		printf("        virstatfs /mnt/sd/virpart0\n");
		return FAT_ERROR;
	}

	(void)memset_s(&sfs, sizeof(sfs), 0, sizeof(sfs));
	result = virstatfs(argv[0], &sfs);
	if (result < 0) {
		set_errno(-result);
		printf("virstatfs failed errno: %d \n", errno);
		return FAT_ERROR;
	}
	toatl_size  = (unsigned long long)sfs.f_bsize * sfs.f_blocks;
	free_size   = (unsigned long long)sfs.f_bsize * sfs.f_bfree;

	printf("Virtual partition \"%s\" Info:\n", argv[0]);
	printf(" f_type         = %u\n cluster_size   = %u\n total_clusters = 0x%llx\n ",
	    sfs.f_type, sfs.f_bsize, sfs.f_blocks);
	printf("free_clusters  = 0x%llx\n avail_clusters = 0x%llx\n f_namelen      = %d\n",
	    sfs.f_bfree, sfs.f_bavail, sfs.f_namelen);
	printf("\n%s\n total size: %4llu Bytes\n free size: %4llu Bytes\n", argv[0], toatl_size, free_size);

	return 0;
}
#endif
#endif
