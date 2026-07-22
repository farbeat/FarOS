/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2022-2022. All rights reserved.
 * Description: XLT memory Module Implementation
 * --------------------------------------------------------------------------- */

#ifdef XLTCFG_MEM_SLAB_EXTENTION

#include "xlt_slab_pri.h"
#include "xlt_memory_pri.h"

#include "securec.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

#define NUM_BITS_IN_ONE_WORD              32
#define LOW_BITS_MASK                     31U
#define NUMBITS_TO_NUMBYTES(num_bits)      (((num_bits) + LOW_BITS_MASK) / 8)

static void os_atomic_bitset_init(struct atomic_bitset *set, UINT32 num_bits)
{
    /* mark all high bits so that os_atomic_bitset_find_clear_and_set() is simpler */
    if (num_bits & LOW_BITS_MASK) {
        set->words[num_bits / NUM_BITS_IN_ONE_WORD] =
            ((UINT32)((INT32) - 1LL)) << (num_bits & LOW_BITS_MASK);
    }
}

UINT32 os_atomic_bitset_get_num_bits(const struct atomic_bitset *set)
{
    return set->num_bits;
}

bool os_atomic_bitset_get_bit(const struct atomic_bitset *set, UINT32 num)
{
    /* any value is as good as the next */
    if (num >= set->num_bits) {
        return FALSE;
    }
    return !!((set->words[num / NUM_BITS_IN_ONE_WORD]) & (1U << (num & LOW_BITS_MASK)));
}

static void os_atomic_bitset_clear_bit(struct atomic_bitset *set, UINT32 num)
{
    UINT32 *word_ptr = set->words + (num / NUM_BITS_IN_ONE_WORD);

    if (num >= set->num_bits) {
        return;
    }
    (*word_ptr) &= ~(1U << (num & LOW_BITS_MASK));
}

/* find from the high bit to high bit return the address of the first available bit */
static INT32 os_atomic_bitset_find_clear_and_set(struct atomic_bitset *set)
{
    UINT32 idx;
    UINT32 num_words = (set->num_bits + LOW_BITS_MASK) / NUM_BITS_IN_ONE_WORD;
    UINT32 *word_ptr = set->words;
    UINT32 tmp_word;
    UINT32 count = 0;

    for (idx = 0; idx < num_words; idx++, word_ptr++) {
        if (*word_ptr == 0xFFFFFFFF) {
            continue;
        }

        tmp_word = ~(*word_ptr);

        while (tmp_word) {
            tmp_word = tmp_word >> 1U;
            count++;
        }

        *word_ptr |= (1U << (count - 1));

        return (INT32)(idx * NUM_BITS_IN_ONE_WORD + count - 1);
    }

    return -1;
}

#ifdef LOSCFG_KERNEL_MEM_SLAB_AUTO_EXPANSION_MODE
static bool os_atomic_bitset_empty(const struct atomic_bitset *bitset)
{
    UINT32 idx;
    for (idx = 0; idx < (bitset->num_bits / NUM_BITS_IN_ONE_WORD); idx++) {
        if (bitset->words[idx] != 0) {
            return FALSE;
        }
    }
    if (bitset->num_bits & LOW_BITS_MASK) {
        if (bitset->words[idx] & ~((UINT32)0xFFFFFFFF << (bitset->num_bits & LOW_BITS_MASK))) {
            return FALSE;
        }
    }
    return TRUE;
}
#endif

os_slab_allocator *os_slab_allocator_new(void *pool, UINT32 item_sz, UINT32 item_align, UINT32 num_items)
{
    os_slab_allocator *allocator = NULL;
    UINT32 atomic_bit_set_sz;
    UINT32 bit_set_sz;
    UINT32 data_sz;
    UINT32 item_size;

    /* calculate size */
    bit_set_sz = NUMBITS_TO_NUMBYTES(num_items);
    atomic_bit_set_sz = sizeof(struct atomic_bitset) + bit_set_sz;

    atomic_bit_set_sz = (atomic_bit_set_sz + item_align - 1) & (~(item_align - 1));
    item_size = (item_sz + item_align - 1) & (~(item_align - 1));
    data_sz = item_size * num_items;

    allocator = (os_slab_allocator*)os_mem_alloc(pool, sizeof(os_slab_allocator) + atomic_bit_set_sz + data_sz);
    if (allocator != NULL) {
        allocator->item_sz = item_size;

        allocator->bitset = (struct atomic_bitset *)(void *)((UINT8*)allocator + sizeof(os_slab_allocator));
        allocator->data_chunks = ((UINT8*)allocator->bitset) + atomic_bit_set_sz;

        struct atomic_bitset *set = allocator->bitset;
        set->num_bits = num_items;
        (void)memset_s(set->words, bit_set_sz, 0, bit_set_sz);
        os_atomic_bitset_init(set, num_items);
    }

    return allocator;
}

void os_slab_allocator_destroy(void *pool, os_slab_allocator *allocator)
{
    (void)os_mem_free(pool, allocator);
}

void *os_slab_allocator_alloc(os_slab_allocator *allocator)
{
    INT32 item_idx = os_atomic_bitset_find_clear_and_set(allocator->bitset);
    if (item_idx < 0) {
        return NULL;
    }

    return allocator->data_chunks + allocator->item_sz * (UINT32)item_idx;
}

bool os_slab_allocator_free(os_slab_allocator *allocator, void* ptr)
{
    UINT8 *ptr_tmp = (UINT8*)ptr;
    UINT32 item_offset = (UINT32)(ptr_tmp - allocator->data_chunks);
    UINT32 item_idx = item_offset / allocator->item_sz;

    /* check for invalid inputs */
    if ((item_offset % allocator->item_sz) || (item_idx >= os_atomic_bitset_get_num_bits(allocator->bitset)) ||
         !(os_atomic_bitset_get_bit(allocator->bitset, item_idx))) {
        return FALSE;
    }

    os_atomic_bitset_clear_bit(allocator->bitset, item_idx);
    return TRUE;
}

#ifdef LOSCFG_KERNEL_MEM_SLAB_AUTO_EXPANSION_MODE
bool os_slab_allocator_empty(const os_slab_allocator *allocator)
{
    return os_atomic_bitset_empty(allocator->bitset);
}
#endif

bool os_slab_allocator_check(const os_slab_allocator *allocator, const void* ptr)
{
    UINT8 *ptr_tmp = (UINT8*)ptr;
    UINT32 item_offset = (UINT32)(ptr_tmp - allocator->data_chunks);
    UINT32 item_idx = item_offset / allocator->item_sz;

    /* check for invalid inputs */
    if ((item_offset % allocator->item_sz) || (item_idx >= os_atomic_bitset_get_num_bits(allocator->bitset)) ||
        !(os_atomic_bitset_get_bit(allocator->bitset, item_idx))) {
        return FALSE;
    }

    return TRUE;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
#endif
