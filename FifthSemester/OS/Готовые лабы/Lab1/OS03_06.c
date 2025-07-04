#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    system("./OS03_05_01");
    for (int i = 1; i <= 100; i++) {
        printf("%d. PID = %d OS03_06\n", i, getpid());
        sleep(1);
    }
    exit(0);
}
