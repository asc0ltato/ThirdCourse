#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sched.h>

/*
	sudo cat /proc/pid/maps
	pmap -X pid
	objdump -f ./OS08_02
	size -Ax ./OS08_02
	.text - секция с кодом (+константы)
	.data - инициализированные данные
	.bss  - неинициализированные данные
*/

int main()
{
	pid_t pid = getpid();

	for (int i = 0; i < 10000000; i++)
	{
		printf("[OS08_02] PID = %d\n", pid);
		sleep(1);
	}
	exit(0);
}
