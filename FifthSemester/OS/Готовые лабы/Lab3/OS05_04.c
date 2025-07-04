#define _GNU_SOURCE
#include <stdio.h>
#include <sys/resource.h>
#include <unistd.h>
#include <sched.h>
#include <sys/syscall.h>

int main() {
    pid_t pid = getpid();
    pid_t tid = syscall(SYS_gettid);
    int niceValue = getpriority(PRIO_PROCESS, 0);
    cpu_set_t cpuSet;
    CPU_ZERO(&cpuSet);

    printf("PID: %d\nTID: %d\nNice: %d\nCPUs: ", pid, tid, niceValue);

    if (sched_getaffinity(0, sizeof(cpu_set_t), &cpuSet) == 0)
    {
        for (int i = 0; i < CPU_SETSIZE; i++)
        {
            if (CPU_ISSET(i, &cpuSet))
                printf("%d ", i);
        }
    }
    else {
        printf("Error: shed_getaffinity returned -1");
    }
    printf("\n");
}
