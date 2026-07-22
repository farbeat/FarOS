#include "stdio_impl.h"
#include "lock.h"
#include "fork_impl.h"

static FILE *ofl_head;
#ifdef __LITEOS__
static pthread_mutex_t ofl_lock = PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP;
#else
static volatile int ofl_lock[1];
volatile int *const __stdio_ofl_lockptr = ofl_lock;
#endif

FILE **__ofl_lock()
{
	LOCK(ofl_lock);
	return &ofl_head;
}

void __ofl_unlock()
{
	UNLOCK(ofl_lock);
}
