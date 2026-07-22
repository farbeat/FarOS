/*
 * Copyright (C) @CompanyNameMagicTag 2021-2021. All rights reserved.
 * Description: Flash FMC MTD List header file.
 * Author: @CompanyNameMagicTag
 * Create: 2021-02-01
 */

#ifndef __DPAL_MTD_LIST_H__
#define __DPAL_MTD_LIST_H__

#include "dpal_mtd.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

typedef struct Node {
    char *type;           /* flash type */
    int status;           /* flash status */
    struct mtd_info *mtd; /* mtd_info struct */
    struct Node *next;    /* next mtd_info struct */
} Lnode, *linklist;

extern linklist head;
struct mtd_info* get_mtd(char *type);
int get_mtd_info(char *type);
int free_mtd(struct mtd_info *mtd);
int del_mtd_list(struct mtd_info *mtd);
void add_mtd_list(char *type, struct mtd_info *mtd);
int mtd_init_list(void);

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* __DPAL_MTD_LIST_H__ */
