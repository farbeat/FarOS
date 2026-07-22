#include <stdint.h>
#include <stdio.h>

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

uintptr_t __stack_chk_guard = 0x000a0dff;

void __stack_chk_fail(void)
{
    printf("%s %d\n", __FUNCTION__, __LINE__);
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
