#include <pthread.h>

int pthread_key_create(pthread_key_t *key, void (*destructor)(void*))
{
    (void)key;
    (void)destructor;
    return -1;
}

void *pthread_getspecific(pthread_key_t key)
{
    (void)key;
    return NULL;
}

int pthread_setspecific(pthread_key_t key, const void *value)
{
    (void)key;
    (void)value;
    return -1;
}

int pthread_once(pthread_once_t *onceControl, void (*initRoutine)(void))
{
    (void)onceControl;
    (void)initRoutine;
    return -1;
}
