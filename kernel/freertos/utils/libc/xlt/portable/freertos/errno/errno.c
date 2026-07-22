int *__errno_location(void)
{
    extern int FreeRTOS_errno;
    return &FreeRTOS_errno;
}
