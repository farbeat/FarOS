#ifndef __SIGNAL_ADAPT__
#define __SIGNAL_ADAPT__

#ifdef __LITEOS__

struct sigevent {
    union sigval sigev_value;
    int sigev_signo;
    int sigev_notify;
    void (*sigev_notify_function)(union sigval);
    pthread_attr_t *sigev_notify_attributes;
    char __pad[56-3*sizeof(long)];
};

#elif defined(__FREERTOS__)

struct sigevent
{
    int sigev_notify;
    int sigev_signo;
    union sigval sigev_value;
    void (*sigev_notify_function)(union sigval);
    pthread_attr_t *sigev_notify_attributes;
};

#endif

#endif /* __SIGNAL_ADAPT__ */
