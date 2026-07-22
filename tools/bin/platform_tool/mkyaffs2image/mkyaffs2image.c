/*
 * YAFFS: Yet Another Flash File System. A NAND-flash specific file system.
 *
 * Copyright (C) 2002-2011 Aleph One Ltd.
 *   for Toby Churchill Ltd and Brightstar Engineering
 *
 * Created by Charles Manning <charles@aleph1.co.uk>
 * Nick Bane modifications flagged NCB
 * Endian handling patches by James Ng.
 * mkyaffs2image hacks by NCB
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

/*
 * makeyaffs2image.c 
 *
 * Makes a YAFFS2 file system image that can be used to load up a file system.
 * Uses default Linux MTD layout - search for "NAND LAYOUT" to change.
 */
 
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <assert.h>
#include "yaffs_ecc.h"
#include "yaffs_guts.h"
#include "yaffs_endian.h"

#include "yaffs_packedtags2.h"
#include <time.h>

unsigned yaffs_trace_mask=0;

#define YAFFS2UTILS_VERSION    "1.0.0"

#define MAX_BUFFER_LEN  8192
#define MAX_OBJECTS     20000

#define MKYAFFS2_PRINT(s, args...)      \
        do {                            \
            fprintf(stdout, s, ##args); \
            fflush(stdout);             \
        } while (0)

#define MKYAFFS2_HELP(s, args...)    MKYAFFS2_PRINT(s, ##args)
#define _S(_s)   (char *)(_s)

unsigned int chunkSize, spareSize, pagesPerBlock;


#ifndef OOB_CONFIGH
#define OOB_CONFIGH

#define NANDIP_V100    0x100


/* this follow must be consistent with fastboot !!! */
enum ecc_type
{
    et_ecc_none    = 0x00,
    et_ecc_1bit    = 0x01,
    et_ecc_4bytes  = 0x02,
    et_ecc_4bit    = 0x02,
    et_ecc_8bytes  = 0x03,
    et_ecc_8bit    = 0x03,
    et_ecc_24bit1k = 0x04,
    et_ecc_40bit1k = 0x05,
    et_ecc_64bit1k = 0x06,
    et_ecc_last    = 0x07,
};


enum page_type
{
    pt_pagesize_512   = 0x00,
    pt_pagesize_2K    = 0x01,
    pt_pagesize_4K    = 0x02,
    pt_pagesize_8K    = 0x03,
    pt_pagesize_16K   = 0x04,
    pt_pagesize_last  = 0x05,
};


enum ecc_type g_ecctype = 0;
enum page_type g_pagetype = 0;


struct nand_oob_free
{
    unsigned long offset;
    unsigned long length;
};


struct oob_info
{
    unsigned int nandip;
    enum page_type pagetype;
    enum ecc_type ecctype;
    unsigned int oobsize;
    struct nand_oob_free *freeoob;
};


const char *nand_controller_version = "Flash Memory Controller V100";
static int align_parttitionksize= 0;


struct nand_oob_free oobfree_def[] =
{
    {2, 30}, {0, 0},
};


static struct oob_info hifmc100_oob_info[] =
{
    {NANDIP_V100, pt_pagesize_2K, et_ecc_4bit,    64,  oobfree_def},
    {NANDIP_V100, pt_pagesize_2K, et_ecc_8bit,    88,  oobfree_def},
    {NANDIP_V100, pt_pagesize_2K, et_ecc_24bit1k, 128, oobfree_def},

    {NANDIP_V100, pt_pagesize_4K, et_ecc_4bit,    88,  oobfree_def},
    {NANDIP_V100, pt_pagesize_4K, et_ecc_8bit,    144, oobfree_def},
    {NANDIP_V100, pt_pagesize_4K, et_ecc_24bit1k, 200, oobfree_def},

    {0},
};


static struct oob_info *get_oob_info(enum page_type pagetype,
    enum ecc_type ecctype)
{
    struct oob_info *info = hifmc100_oob_info;

    for (; info->freeoob != NULL; info++)
    {
        if (info->ecctype == ecctype && info->pagetype == pagetype)
        {
            return info;
        }
    }

    return NULL;
}


static const char *get_ecctype_str(enum ecc_type ecctype)
{
    const char *ecctype_str[et_ecc_last + 1] = {"None", "1bit",
        "4bit", "8Bytes", "24bits/1K", "40bits/1K", "64bits/1K"};
    if (ecctype < et_ecc_none || ecctype > et_ecc_last)
    {
        ecctype = et_ecc_last;
    }
    return ecctype_str[ecctype];
}


static const char *get_pagesize_str(enum page_type pagetype)
{
    const char *pagesize_str[pt_pagesize_last + 1] = {"512", "2K",
        "4K", "8K", "16K", "unknown"};
    if (pagetype < pt_pagesize_512 || pagetype > pt_pagesize_last)
    {
        pagetype = pt_pagesize_last;
    }
    return pagesize_str[pagetype];
}


static unsigned int get_pagesize(enum page_type pagetype)
{
    unsigned int pagesize[] = {512, 2048, 4096, 8192, 16384, 0};
    return pagesize[pagetype];
}

#endif /* OOB_CONFIGH */


static struct oob_info *sel_oob_info = NULL;


static int mkyaffs2_helper (void)
{
    MKYAFFS2_HELP("mkyaffs2image %s - A utility to make the yaffs2 image\n\n", YAFFS2UTILS_VERSION);
    MKYAFFS2_HELP("Usage: mkyaffs2image {dir} {image} {pagesize} {ecctype}\n\n");
    MKYAFFS2_HELP("Options:\n");
    MKYAFFS2_HELP("dir       the directory tree to be converted\n");
    MKYAFFS2_HELP("image     the output file to hold the image\n");
    MKYAFFS2_HELP("pagesize  the page size to be converted\n");

    // Currently, only [1, 4] [2, 4] [1, 2] config are supported
    // MKYAFFS2_HELP("        0 | 512     512 Bytes pagesize\n");
    MKYAFFS2_HELP("        1 | 2k      2K Bytes pagesize\n");
    MKYAFFS2_HELP("        2 | 4K      4K Bytes pagesize\n");
    // MKYAFFS2_HELP("        3 | 8K      8K Bytes pagesize\n");
    // MKYAFFS2_HELP("        4 | 16K     16K Bytes pagesize\n");
    MKYAFFS2_HELP("ecctype   the ECC type to be converted\n");
    // MKYAFFS2_HELP("        0 | none    no ecc\n");
    // MKYAFFS2_HELP("        1 | 1bit    used 1bit/512 ecc\n");
    MKYAFFS2_HELP("        2 | 4bit    used 4bit/512 ecc\n");
    // MKYAFFS2_HELP("        3 | 8bit    used 8bit/512 ecc\n");
    MKYAFFS2_HELP("        4 | 24bit   used 24bit/1k ecc\n");
    // MKYAFFS2_HELP("        5 | 40bit   used 40bit/1k ecc\n");
    // MKYAFFS2_HELP("        6 | 64bit   used 64bit/1k ecc\n");
    MKYAFFS2_HELP("Example:\n");
    MKYAFFS2_HELP(" mkyaffs2image100 ./rootfs rootfs_4k24bit.yaffs 2 4\n");
    MKYAFFS2_HELP(" mkyaffs2image610 ./rootfs rootfs_4k24bit.yaffs 2 4\n");
    MKYAFFS2_HELP("\n");

    return -1;
}


struct param_table_t
{
    char *str;
    int   type;
};


struct param_table_t page_type_param[] =
{
    {_S("0"),    pt_pagesize_512},
    {_S("512"),  pt_pagesize_512},

    {_S("1"),    pt_pagesize_2K},
    {_S("2048"), pt_pagesize_2K},
    {_S("2K"),   pt_pagesize_2K},

    {_S("2"),    pt_pagesize_4K},
    {_S("4096"), pt_pagesize_4K},
    {_S("4K"),   pt_pagesize_4K},

    {_S("3"),    pt_pagesize_8K},
    {_S("8192"), pt_pagesize_8K},
    {_S("8K"),   pt_pagesize_8K},

    {_S("4"),    pt_pagesize_16K},
    {_S("16384"), pt_pagesize_16K},
    {_S("16K"),   pt_pagesize_16K},

    {NULL, pt_pagesize_last},
};


struct param_table_t ecc_type_param[] =
{
    {_S("0"),        et_ecc_none},
    {_S("none"),     et_ecc_none},

    {_S("1"),        et_ecc_1bit},
    {_S("1bit"),     et_ecc_1bit},
    {_S("1bits"),    et_ecc_1bit},

    {_S("2"),        et_ecc_4bytes},
    {_S("4bit"),     et_ecc_4bit},
    {_S("4bits"),    et_ecc_4bit},
    {_S("4byte"),    et_ecc_4bytes},
    {_S("4bytes"),   et_ecc_4bytes},

    {_S("3"),        et_ecc_8bytes},
    {_S("8bit"),     et_ecc_8bytes},
    {_S("8bits"),    et_ecc_8bytes},
    {_S("8byte"),    et_ecc_8bytes},
    {_S("8bytes"),   et_ecc_8bytes},

    {_S("4"),        et_ecc_24bit1k},
    {_S("24bit"),    et_ecc_24bit1k},
    {_S("24bits"),   et_ecc_24bit1k},
    {_S("24bits1k"), et_ecc_24bit1k},
    {_S("24bit1k"),  et_ecc_24bit1k},
    {_S("24bit1k"),  et_ecc_24bit1k},

    {_S("5"),        et_ecc_40bit1k},
    {_S("40bit"),    et_ecc_40bit1k},
    {_S("40bits"),   et_ecc_40bit1k},
    {_S("40bits1k"), et_ecc_40bit1k},
    {_S("40bit1k"),  et_ecc_40bit1k},

    {_S("6"),        et_ecc_64bit1k},
    {_S("64bit"),    et_ecc_64bit1k},
    {_S("64bits"),   et_ecc_64bit1k},
    {_S("64bits1k"), et_ecc_64bit1k},
    {_S("64bit1k"),  et_ecc_64bit1k},

    {NULL, et_ecc_last},
};
#undef _S


typedef struct
{
    dev_t dev;
    ino_t ino;
    int   obj;
} objItem;

struct yaffs_dev dummy_dev = {.swap_endian = 0};


static objItem obj_list[MAX_OBJECTS];
static int obj_id = YAFFS_NOBJECT_BUCKETS + 1;

static int n_obj, nDirectories, nPages;

static int outFile;

static int error;
static int savedErrno;

static int convert_endian = 0;

/* Provide our own endian swap that does nothing. */
void yaffs_do_endian_packed_tags2(struct yaffs_dev *dev,
                struct yaffs_packed_tags2_tags_only *ptt)
{
#if 0
    if (!dev->swap_endian)
        return;
    ptt->seq_number = swap_u32(ptt->seq_number);
    ptt->obj_id = swap_u32(ptt->obj_id);
    ptt->chunk_id = swap_u32(ptt->chunk_id);
    ptt->n_bytes = swap_u32(ptt->n_bytes);
#endif
}

static void fatal(const char *fn)
{
    perror(fn);
    error |= 1;
    exit(error);
}

static int warn(const char *fn)
{
    savedErrno = errno;
    perror(fn);
    error |= 2;
    return error;
}

static int obj_compare(const void *a, const void * b)
{
  objItem *oa, *ob;
  
  oa = (objItem *)a;
  ob = (objItem *)b;
  
  if(oa->dev < ob->dev) return -1;
  if(oa->dev > ob->dev) return 1;
  if(oa->ino < ob->ino) return -1;
  if(oa->ino > ob->ino) return 1;
  
  return 0;
}


static void add_obj_to_list(dev_t dev, ino_t ino, int obj)
{
    if(n_obj < MAX_OBJECTS)
    {
        obj_list[n_obj].dev = dev;
        obj_list[n_obj].ino = ino;
        obj_list[n_obj].obj = obj;
        n_obj++;
        qsort(obj_list,n_obj,sizeof(objItem),obj_compare);
        
    }
    else
    {
        // oops! not enough space in the object array
        fprintf(stderr,"Not enough space in object array\n");
        exit(1);
    }
}


static int find_obj_in_list(dev_t dev, ino_t ino)
{
    objItem *i = NULL;
    objItem test;

    test.dev = dev;
    test.ino = ino;
    
    if(n_obj > 0)
    {
        i = bsearch(&test,obj_list,n_obj,sizeof(objItem),obj_compare);
    }

    if(i)
    {
        return i->obj;
    }
    return -1;
}

/* This little function converts a little endian tag to a big endian tag.
 * NOTE: The tag is not usable after this other than calculating the CRC
 * with.
 */
static void little_to_big_endian(struct yaffs_ext_tags *tagsPtr)
{
#if 0 // FIXME NCB
    union yaffs_tags_union * tags = (union yaffs_tags_union* )tagsPtr; // Work in bytes.
    union yaffs_tags_union   temp;

    memset(&temp, 0, sizeof(temp));
    // Ick, I hate magic numbers.
    temp.as_bytes[0] = ((tags->as_bytes[2] & 0x0F) << 4) | ((tags->as_bytes[1] & 0xF0) >> 4);
    temp.as_bytes[1] = ((tags->as_bytes[1] & 0x0F) << 4) | ((tags->as_bytes[0] & 0xF0) >> 4);
    temp.as_bytes[2] = ((tags->as_bytes[0] & 0x0F) << 4) | ((tags->as_bytes[2] & 0x30) >> 2) | ((tags->as_bytes[3] & 0xC0) >> 6);
    temp.as_bytes[3] = ((tags->as_bytes[3] & 0x3F) << 2) | ((tags->as_bytes[2] & 0xC0) >> 6);
    temp.as_bytes[4] = ((tags->as_bytes[6] & 0x03) << 6) | ((tags->as_bytes[5] & 0xFC) >> 2);
    temp.as_bytes[5] = ((tags->as_bytes[5] & 0x03) << 6) | ((tags->as_bytes[4] & 0xFC) >> 2);
    temp.as_bytes[6] = ((tags->as_bytes[4] & 0x03) << 6) | (tags->as_bytes[7] & 0x3F);
    temp.as_bytes[7] = (tags->as_bytes[6] & 0xFC) | ((tags->as_bytes[7] & 0xC0) >> 6);

    // Now copy it back.
    tags->as_bytes[0] = temp.as_bytes[0];
    tags->as_bytes[1] = temp.as_bytes[1];
    tags->as_bytes[2] = temp.as_bytes[2];
    tags->as_bytes[3] = temp.as_bytes[3];
    tags->as_bytes[4] = temp.as_bytes[4];
    tags->as_bytes[5] = temp.as_bytes[5];
    tags->as_bytes[6] = temp.as_bytes[6];
    tags->as_bytes[7] = temp.as_bytes[7];
#endif
}

static void shuffle_oob(char *spareData, struct yaffs_packed_tags2 *pt)
{
    assert(sizeof(*pt) <= spareSize);
    // NAND LAYOUT: For non-trivial OOB orderings, here would be a good place to shuffle.
    memcpy(spareData, pt, sizeof(*pt));
}

static int write_chunk(u8 *data, u32 id, u32 chunk_id, u32 n_bytes)
{
    struct yaffs_ext_tags t;
    struct yaffs_packed_tags2 pt;
    char spareData[spareSize];
    struct nand_oob_free *oobfree = sel_oob_info->freeoob;

    if (write(outFile,data,chunkSize) != chunkSize)
        fatal("write");

    memset(&t, 0, sizeof(t));
    
    t.chunk_id = chunk_id;
//    t.serial_number = 0;
    t.serial_number = 1;    // **CHECK**
    t.n_bytes = n_bytes;
    t.obj_id = id;
    
    t.seq_number = YAFFS_LOWEST_SEQUENCE_NUMBER;

// added NCB **CHECK**
    t.chunk_used = 1;

    if (convert_endian)
    {
            little_to_big_endian(&t);
    }

    nPages++;

    memset(&pt, 0, sizeof(pt));
    yaffs_pack_tags2(&dummy_dev, &pt,&t,1);

    memset(spareData, 0xff, sizeof(spareData));
    spareData[oobfree->length] = 0;
    spareData[oobfree->length + 1] = 0;

    shuffle_oob(&spareData[2], &pt); // use 2bit to mark bad block

    if (write(outFile,spareData,sizeof(spareData)) != sizeof(spareData))
        fatal("write");
    return 0;
}

#define SWAP32(x)   ((((x) & 0x000000FF) << 24) | \
                     (((x) & 0x0000FF00) << 8 ) | \
                     (((x) & 0x00FF0000) >> 8 ) | \
                     (((x) & 0xFF000000) >> 24))

#define SWAP16(x)   ((((x) & 0x00FF) << 8) | \
                     (((x) & 0xFF00) >> 8))
        
// This one is easier, since the types are more standard. No funky shifts here.
static void object_header_little_to_big_endian(struct yaffs_obj_hdr* oh)
{
    oh->type = SWAP32(oh->type); // GCC makes enums 32 bits.
    oh->parent_obj_id = SWAP32(oh->parent_obj_id); // int
    oh->sum_no_longer_used = SWAP16(oh->sum_no_longer_used); // u16 - Not used, but done for completeness.
    // name = skip. Char array. Not swapped.
    oh->yst_mode = SWAP32(oh->yst_mode);

    // Regular POSIX.
    oh->yst_uid = SWAP32(oh->yst_uid);
    oh->yst_gid = SWAP32(oh->yst_gid);
    oh->yst_atime = SWAP32(oh->yst_atime);
    oh->yst_mtime = SWAP32(oh->yst_mtime);
    oh->yst_ctime = SWAP32(oh->yst_ctime);

    oh->file_size_low = SWAP32(oh->file_size_low); // Aiee. An int... signed, at that!
    oh->file_size_high = SWAP32(oh->file_size_high); // Aiee. An int... signed, at that!
    oh->equiv_id = SWAP32(oh->equiv_id);
    // alias  - char array.
    oh->yst_rdev = SWAP32(oh->yst_rdev);

    oh->win_ctime[0] = SWAP32(oh->win_ctime[0]);
    oh->win_ctime[1] = SWAP32(oh->win_ctime[1]);
    oh->win_atime[0] = SWAP32(oh->win_atime[0]);
    oh->win_atime[1] = SWAP32(oh->win_atime[1]);
    oh->win_mtime[0] = SWAP32(oh->win_mtime[0]);
    oh->win_mtime[1] = SWAP32(oh->win_mtime[1]);

    oh->reserved[0] = SWAP32(oh->reserved[0]);

    oh->inband_shadowed_obj_id = SWAP32(oh->inband_shadowed_obj_id);
    oh->inband_is_shrink = SWAP32(oh->inband_is_shrink);
    oh->shadows_obj = SWAP32(oh->shadows_obj);
    oh->is_shrink = SWAP32(oh->is_shrink);
}


/* this follow must be consistent with fastboot !!! */
struct yaffs2_tags {
#define YAFFS2_SIGN_MAGIC       "YFSS!V10"
#define YAFFS2_SIGN_MAGIC_SIZE  8
    unsigned char magic[YAFFS2_SIGN_MAGIC_SIZE];
    unsigned int nandip;
    unsigned char yaffs2ver[4];
    unsigned int pagesize;
    enum ecc_type ecctype;
};


typedef struct mkyaffs2_obj {
    dev_t dev;
    ino_t ino;
    unsigned obj_id;
    struct mkyaffs2_obj *parent_obj;
    unsigned type;
    char name[256 + 1];
    struct list_head children;    /* for a directory */
    struct list_head siblings;    /* neighbors in the same directory */
    struct list_head hashlist;    /* hash table */
} mkyaffs2_obj_t;


static int write_tag_file(unsigned int nandip, enum page_type page_type, enum ecc_type ecc_type)
{
    char buffer[MAX_BUFFER_LEN] = {0};
    unsigned short *len;
    char *ptr;

    struct yaffs2_tags *yaffs2_tags = (struct yaffs2_tags *)buffer;
    struct stat status =
    {
        .st_mode = 0x81FF,
    };

    memcpy(yaffs2_tags->magic, YAFFS2_SIGN_MAGIC, YAFFS2_SIGN_MAGIC_SIZE);
    yaffs2_tags->nandip = nandip;
    /* yaffs2 version number */
    yaffs2_tags->yaffs2ver[0] = 2;
    yaffs2_tags->yaffs2ver[1] = 6;
    yaffs2_tags->yaffs2ver[2] = 35;
    yaffs2_tags->yaffs2ver[3] = 7;
    yaffs2_tags->pagesize = get_pagesize(page_type);
    yaffs2_tags->ecctype  = ecc_type;

    ptr = buffer + sizeof(struct yaffs2_tags);
    /* tag string length. */
    len = (unsigned short *)ptr;
    ptr += sizeof(unsigned short);

    time_t now;
    ctime(&now);

    /* tag string comment */
    *len = sprintf(ptr, "\nyaffs2 file system: %s %s \n"
            "mkyaffs2image version: %s %s, build time: %s.",
            get_pagesize_str(g_pagetype),
            get_ecctype_str(g_ecctype),
            YAFFS2UTILS_VERSION, nand_controller_version, ctime(&now));

    ptr += (*len + 1);

    status.st_size = (int)(ptr - buffer);

    write_chunk((u8 *)buffer, 0, 1024, status.st_size);
    return 0;
}


static int write_object_header(int id, enum yaffs_obj_type t, struct stat *s, int parent, const char *name, int equivalentObj, const char * alias)
{
    u8 bytes[chunkSize];
    
    
    struct yaffs_obj_hdr *oh = (struct yaffs_obj_hdr *)bytes;
    
    memset(bytes,0xff,sizeof(bytes));
    
    oh->type = t;

    oh->parent_obj_id = parent;
    
    if (strlen(name)+1 > sizeof(oh->name))
    {
        errno = ENAMETOOLONG;
        return warn("object name");
    }
    memset(oh->name,0,sizeof(oh->name));
    strcpy(oh->name,name);
    
    
    if(t != YAFFS_OBJECT_TYPE_HARDLINK)
    {
        oh->yst_mode = s->st_mode;
        oh->yst_uid = s->st_uid;
// NCB 12/9/02        oh->yst_gid = s->yst_uid;
        oh->yst_gid = s->st_gid;
        oh->yst_atime = s->st_atime;
        oh->yst_mtime = s->st_mtime;
        oh->yst_ctime = s->st_ctime;
        oh->yst_rdev  = s->st_rdev;
    }
    
    if(t == YAFFS_OBJECT_TYPE_FILE)
    {
        oh->file_size_low = s->st_size;
        oh->file_size_high = (s->st_size >> 32);
    }
    
    if(t == YAFFS_OBJECT_TYPE_HARDLINK)
    {
        oh->equiv_id = equivalentObj;
    }
    
    if(t == YAFFS_OBJECT_TYPE_SYMLINK)
    {
        if (strlen(alias)+1 > sizeof(oh->alias))
        {
            errno = ENAMETOOLONG;
            return warn("object alias");
        }
        memset(oh->alias,0,sizeof(oh->alias));
        strcpy(oh->alias,alias);
    }

    if (convert_endian)
    {
            object_header_little_to_big_endian(oh);
    }
    
    return write_chunk(bytes,id,0,0xffff);
    
}

#ifdef BLKNEEDPADDING
static void pad_image(void)
{
    u8 data[chunkSize + spareSize];
    int padPages = (nPages % pagesPerBlock);

    if (padPages)
    {
        memset(data, 0xff, sizeof(data));
        for (padPages = pagesPerBlock-padPages; padPages; padPages--)
        {
            if (write(outFile, data, sizeof(data)) != sizeof(data))
                fatal("write");
        }
    }
}
#endif

static int process_directory(int parent, const char *path)
{

    DIR *dir;
    struct dirent *entry;

    nDirectories++;
    
    dir = opendir(path);
    if(!dir)
    {
        warn("opendir");
    }
    else
    {
        while((entry = readdir(dir)) != NULL)
        {
        
            /* Ignore . and .. */
            if(strcmp(entry->d_name,".") &&
               strcmp(entry->d_name,".."))
             {
                 char full_name[500];
                struct stat stats;
                int equivalentObj;
                int newObj;
                
                if (snprintf(full_name,sizeof(full_name),"%s/%s",path,entry->d_name) >= (int)sizeof(full_name))
                {
                    error = -1;
                    continue;
                }
                
                if (lstat(full_name,&stats) < 0)
                {
                    warn("lstat");
                    continue;
                }
                
                if(S_ISLNK(stats.st_mode) ||
                    S_ISREG(stats.st_mode) ||
                    S_ISDIR(stats.st_mode) ||
                    S_ISFIFO(stats.st_mode) ||
                    S_ISBLK(stats.st_mode) ||
                    S_ISCHR(stats.st_mode) ||
                    S_ISSOCK(stats.st_mode))
                {
                
                    newObj = obj_id++;
                    n_obj++;
                    
                    printf("Object %d, %s is a ",newObj,full_name);
                    
                    /* We're going to create an object for it */
                    if((equivalentObj = find_obj_in_list(stats.st_dev, stats.st_ino)) > 0)
                    {
                         /* we need to make a hard link */
                         printf("hard link to object %d\n",equivalentObj);
                        write_object_header(newObj, YAFFS_OBJECT_TYPE_HARDLINK, &stats, parent, entry->d_name, equivalentObj, NULL);
                    }
                    else 
                    {
                        
                        add_obj_to_list(stats.st_dev,stats.st_ino,newObj);
                        
                        if(S_ISLNK(stats.st_mode))
                        {
                    
                            char symname[500];
                        
                            memset(symname,0, sizeof(symname));
                    
                            if (readlink(full_name,symname,sizeof(symname) -1) < 0)
                            {
                                warn("readlink");
                            }
                            else
                            {
                                printf("symlink to \"%s\"\n",symname);
                                write_object_header(newObj, YAFFS_OBJECT_TYPE_SYMLINK, &stats, parent, entry->d_name, -1, symname);
                            }
                        }
                        else if(S_ISREG(stats.st_mode))
                        {
                            printf("file, ");
                            if(write_object_header(newObj, YAFFS_OBJECT_TYPE_FILE, &stats, parent, entry->d_name, -1, NULL) == 0)
                            {
                                int h;
                                u8 bytes[chunkSize];
                                int n_bytes;
                                int chunk = 0;
                                
                                h = open(full_name,O_RDONLY);
                                if(h >= 0)
                                {
                                    memset(bytes,0xff,sizeof(bytes));
                                    while((n_bytes = read(h,bytes,sizeof(bytes))) > 0)
                                    {
                                        chunk++;
                                        write_chunk(bytes,newObj,chunk,n_bytes);
                                        memset(bytes,0xff,sizeof(bytes));
                                    }
                                    if(n_bytes < 0) 
                                       warn("read");
                                       
                                    printf("%d data chunks written\n",chunk);
                                    close(h);
                                }
                                else
                                {
                                    warn("open");
                                }
                                
                            }                            
                                                        
                        }
                        else if(S_ISSOCK(stats.st_mode))
                        {
                            printf("socket\n");
                            write_object_header(newObj, YAFFS_OBJECT_TYPE_SPECIAL, &stats, parent, entry->d_name, -1, NULL);
                        }
                        else if(S_ISFIFO(stats.st_mode))
                        {
                            printf("fifo\n");
                            write_object_header(newObj, YAFFS_OBJECT_TYPE_SPECIAL, &stats, parent, entry->d_name, -1, NULL);
                        }
                        else if(S_ISCHR(stats.st_mode))
                        {
                            printf("character device\n");
                            write_object_header(newObj, YAFFS_OBJECT_TYPE_SPECIAL, &stats, parent, entry->d_name, -1, NULL);
                        }
                        else if(S_ISBLK(stats.st_mode))
                        {
                            printf("block device\n");
                            write_object_header(newObj, YAFFS_OBJECT_TYPE_SPECIAL, &stats, parent, entry->d_name, -1, NULL);
                        }
                        else if(S_ISDIR(stats.st_mode))
                        {
                            printf("directory\n");
                            if (write_object_header(newObj, YAFFS_OBJECT_TYPE_DIRECTORY, &stats, parent, entry->d_name, -1, NULL) == 0)
                                process_directory(newObj,full_name);
                        }
                    }
                }
                else
                {
                    fprintf(stderr, "%s: unhandled type\n", full_name);
                    error |= 2;
                    savedErrno = EINVAL;
                }
            }
        }
        closedir(dir);
    }
    
    return 0;

}

int main(int argc, char *argv[])
{
    struct stat stats;
    char* imagefile;
    
    printf("mkyaffs2image: image building tool for YAFFS2 built "__DATE__"\n");
    
    if(argc < 4)
    {
        mkyaffs2_helper();
        exit(1);
    }

    if (argc == 6) {
        if (!strncmp(argv[5], "convert", strlen("convert")))
        {
            convert_endian = 1;
        } else {
            align_parttitionksize = strtol(argv[5], NULL, 0);
        }
    }

    if(stat(argv[1],&stats) < 0)
    {
        printf("Could not stat %s\n",argv[1]);
        exit(1);
    }
    
    if(!S_ISDIR(stats.st_mode))
    {
        printf(" %s is not a directory\n",argv[1]);
        exit(1);
    }
    
    // outFile = open(argv[2],O_CREAT | O_TRUNC | O_WRONLY, S_IREAD | S_IWRITE);
    imagefile = argv[2];
    if ( 0 == access(imagefile,0))
    {
        int file_size = 0;
        outFile = open(imagefile, O_WRONLY, S_IREAD | S_IWRITE);
        file_size = lseek(outFile, 0, SEEK_END);
        printf("File [%s] is exisits,file_size = %d\n",imagefile, file_size);
    }
    else
    {
       outFile = open(imagefile, O_CREAT | O_TRUNC | O_WRONLY, S_IREAD | S_IWRITE);
       printf("File [%s] is cread\n",imagefile);
    }
    
    
    if(outFile < 0)
    {
        printf("Could not open output file %s\n",argv[2]);
        exit(1);
    }

    struct param_table_t *ptp = page_type_param;
    struct param_table_t *etp = ecc_type_param;
    g_pagetype = pt_pagesize_last;
    for (; ptp->str; ptp++)
    {
        if (!strcasecmp(ptp->str, argv[3]))
        {
            g_pagetype = ptp->type;
            break;
        }
    }
    g_ecctype = et_ecc_last;
    if (argc == 4)
    {
        printf("No ECC type input, using the default value 4bit/512\n");
        g_ecctype = et_ecc_4bit;
    }
    else
    {
        for (; etp->str; etp++)
        {
            if (!strcasecmp(etp->str, argv[4]))
            {
                g_ecctype = etp->type;
                break;
            }
        }
    }
    sel_oob_info = get_oob_info(g_pagetype, g_ecctype);
    if (sel_oob_info == NULL) {
        fprintf(stderr, "not support this parameter.\n");
        printf("Input parameter: pagetype: %d(%s), ecctype: %d(%s)\n",
                g_pagetype, get_pagesize_str(g_pagetype),
                g_ecctype, get_ecctype_str(g_ecctype));
        return -1;
    }

    chunkSize = get_pagesize(g_pagetype);
    spareSize = sel_oob_info->oobsize;

    printf("Processing directory %s into image file %s\n",argv[1],argv[2]);

    write_tag_file(sel_oob_info->nandip, g_pagetype, g_ecctype);

    /* Write the head of root directory */
    error =  write_object_header(1, YAFFS_OBJECT_TYPE_DIRECTORY, &stats, 1, "", -1, NULL);
    if (!error)
    {
        error = process_directory(YAFFS_OBJECTID_ROOT, argv[1]);
    }

    if(align_parttitionksize)
    {
        int total_length = 0;
        int total_page = 0;
        int pad_page = 0;
        char buffer[MAX_BUFFER_LEN];
        memset(buffer, 0xFF, sizeof(buffer));
        total_length = lseek(outFile, 0, SEEK_END);
        total_page = align_parttitionksize/chunkSize;
        pad_page = total_page - (total_length / (chunkSize + spareSize));
        printf("tlen=%d tp=%d csz=%d pp=%d \r\n", total_length, total_page, chunkSize, pad_page);
        if(pad_page < 0)
        {
            fprintf(stderr, "Bad align_parttitionksize\n");
            exit(error);
        }
        while(pad_page--)
        {
            if (write(outFile,buffer,chunkSize + spareSize) != chunkSize + spareSize) {
                printf("write data fail\r\n");
            }
        }
    }

    close(outFile);
    
    if(error)
    {
        errno = savedErrno;
        perror("operation incomplete");
    }
    else
    {
        printf("Operation complete.\n"
               "%d objects in %d directories\n"
               "%d NAND pages\n",n_obj, nDirectories, nPages);
    }
    
    exit(error);
}    

