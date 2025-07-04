#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include <sys/syscall.h>
#define DELAY_MS 100
pthread_mutex_t mutex;

pthread_t create_thread(void* function, void* args)
{
    pthread_t thread;
    pthread_create(&thread, NULL, function, args);
    return thread;
}

void* thread_func(char* name)
{
    pid_t pid = getpid();
    pid_t tid = syscall(SYS_gettid);
    struct timespec timespec;
    timespec.tv_sec = DELAY_MS / 1000;
    timespec.tv_nsec = (DELAY_MS % 1000) * 1000000; 

    for (int i = 1; i <= 90; i++)
    {
        if (i == 30) {
            pthread_mutex_lock(&mutex);
	}
        else if (i == 60) {
            pthread_mutex_unlock(&mutex);
	}

        printf("[%s]\t%d.PID: %d, TID: %d\n", name, i, pid, tid);
	nanosleep(&timespec, NULL);
    }

    printf("\n--------  %s Finish  --------\n\n", name);
}


int main(int argc, char* argv[])
{
    pid_t pid = getpid();
    pid_t tid = syscall(SYS_gettid);
    pthread_mutex_init(&mutex, NULL);
    pthread_t threads[2];

    threads[0] = create_thread(thread_func, (char*)"A");
    threads[1] = create_thread(thread_func, (char*)"B");

    struct timespec timespec;
    timespec.tv_sec = DELAY_MS / 1000;
    timespec.tv_nsec = (DELAY_MS % 1000) * 1000000;

    for (int i = 1; i <= 90; i++)
    {
        if (i == 30) {
            pthread_mutex_lock(&mutex);
	}
        else if (i == 60) {
            pthread_mutex_unlock(&mutex);
	}

        printf("[MAIN]\t%d.PID: %d, TID: %d\n", i, pid, tid);
	nanosleep(&timespec, NULL);
    }

    printf("\n--------  MAIN finish  --------\n\n");

    pthread_join(threads[0], NULL);
    pthread_join(threads[1], NULL);

    pthread_mutex_destroy(&mutex);
    return 0;
}
