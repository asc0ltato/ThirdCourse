#include <stdio.h>
#include <unistd.h>

int main() {
	pid_t pid = getpid();
	for (int i = 1; i <= 5000; ++i) {
		printf("%d. PID = %d\n", i, pid);
		sleep(1);
	}
	exit(0);
}
