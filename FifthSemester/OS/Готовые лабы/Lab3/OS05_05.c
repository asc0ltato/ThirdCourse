#include <stdio.h>
#include <unistd.h>
#include <sys/syscall.h>

int main()
{
    pid_t pid = getpid();
    pid_t tid = syscall(SYS_gettid);

    for (int i = 1; i < 100000; ++i)
    {
        printf("%d. PID:  %d, TID:  %d\n", i, pid, tid);
        sleep(1);
    }
}
