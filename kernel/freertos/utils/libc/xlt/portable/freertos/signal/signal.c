#include "signal.h"
#include "errno.h"
#include "libc_portable.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

void posix_signal_start(void)
{
    PRINT_ERR("%s NOT SUPPORT\n", __FUNCTION__);
    errno = ENOSYS;
}

int kill(pid_t pid, int sig)
{
    (void)pid;
    (void)sig;
    PRINT_ERR("%s NOT SUPPORT\n", __FUNCTION__);
    errno = ENOSYS;
    return -1;
}

sa_sighandler_t signal(int sig, sa_sighandler_t handler)
{
    (void)sig;
    (void)handler;
    PRINT_ERR("%s NOT SUPPORT\n", __FUNCTION__);
    errno = ENOSYS;
    return NULL;
}

sa_sighandler_t sigset(int sig, sa_sighandler_t disp)
{
    (void)sig;
    (void)disp;
    PRINT_ERR("%s NOT SUPPORT\n", __FUNCTION__);
    errno = ENOSYS;
    return NULL;
}

int raise(int sig)
{
    (void)sig;
    PRINT_ERR("%s NOT SUPPORT\n", __FUNCTION__);
    errno = ENOSYS;
    return -1;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
