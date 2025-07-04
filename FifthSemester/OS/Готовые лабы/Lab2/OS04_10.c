#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/syscall.h>

void* OS04_10_T1(void* arg)
{
    for (int i = 1; i <= 75; ++i)
    {
        printf("%d. PID = %d TID = %lu\n", i, getpid(), syscall(SYS_gettid));
        sleep(1);
    }
}

int main()
{
    pthread_t thread;
    pthread_create(&thread, NULL, OS04_10_T1, NULL);

    for (int i = 1; i <= 100; ++i)
    {
        printf("%d. PID-MAIN = %d TID = %lu\n", i, getpid(), syscall(SYS_gettid));
        sleep(1);
    }
}
