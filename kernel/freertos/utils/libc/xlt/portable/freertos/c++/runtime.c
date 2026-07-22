void *__dso_handle = 0;

int __cxa_atexit ( void (*f)(void *), void *p, void *d )
{
    (void)f;
    (void)p;
    (void)d;
    return -1;
}
