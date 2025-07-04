#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

int main() {
    clock_t start_cpu_time = clock();
    time_t start_real_time = time(NULL);
    clock_t end_cpu_time;
    time_t  end_real_time;
    int counter = 0;

    while (1) {
        counter++;
        end_cpu_time = clock();

        if ((end_cpu_time - start_cpu_time) / CLOCKS_PER_SEC >= 2) {
            printf("Iteration count: %d\n", counter);
            start_cpu_time = end_cpu_time;
        }

	end_real_time = time(NULL);

	if (end_real_time - start_real_time >= 7) {
	     break;
        }
    }

    printf("Real time elapsed: %ld seconds\n", end_real_time - start_real_time);
    printf("Final iterations: %d\n", counter);
    exit(0);
}
