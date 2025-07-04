#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>
#include <semaphore.h>
#define FILE_SIZE (640 *1024)
#define VIEW_SIZE (64 * 1024)
#define N 10

void error_exit(const char* message) {
    perror(message);
    exit(EXIT_FAILURE);
}

int main() {
	printf("Step 1: Sem_open...\n");
	sem_t *sem = sem_open("Lab-02-semaphore", 0);
	if(sem == SEM_FAILED) {
		error_exit("Error: Sem_open failed");
	}
	printf("Sem_open successfully.\n");
	getchar();

	printf("Step 2: Create shared memory...\n");
	int fd = shm_open("Lab-02e", O_RDONLY, 0666);
	if(fd == -1) {
		error_exit("Error: Create shared memory failed");
	}

	printf("Open shared memory successfully.\n");
	getchar();

	printf("Step 3: Mmap...\n");
	int* pViewRead = mmap(NULL, VIEW_SIZE, PROT_READ, MAP_SHARED, fd, 0);
	if (pViewRead == MAP_FAILED) {
		error_exit("Error: Mmap failed");
	}

	int* pData = (int*)pViewRead;

	printf("Reader: Reading data...\n");
    for (int i = 0; i < N; i++) {
        sem_wait(sem);

        for (int j = 0; j < (VIEW_SIZE / sizeof(int)); j++) {
            	printf("%d. pData[j] = %d\n", i, pData[j]);
        }
		
 		printf("tap...\n");
 		getchar();

		sem_post(sem); 
		usleep(500000);
    }

    printf("Mmap successfully.\n");
	getchar();

	munmap(pViewRead, VIEW_SIZE);
	close(fd);

    printf("Reader: Finished.\n");
    getchar(); 

	return 0;
}