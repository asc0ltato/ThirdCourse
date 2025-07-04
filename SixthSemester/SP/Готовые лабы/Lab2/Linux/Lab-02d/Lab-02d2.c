#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>

#define FILE_PATH "Lab-02d2.txt"
#define OFFSET 0
#define FILE_SIZE (64 * 1024)
#define NEW_DATA "Zhuk Svetlana"

void error_exit(const char* message) {
    perror(message);
    exit(EXIT_FAILURE);
}

void show_proc() {
    system("cat /proc/self/maps");
}

int main(int argc, char* argv[]) {
    struct stat sb;

    int fd = open(FILE_PATH, O_RDWR | O_CREAT, 0666);
    if (fd == -1) {
        error_exit("Error: Open file filed");
    }

    if (ftruncate(fd, FILE_SIZE) == -1)
        error_exit("Error: Fruncate failed");

    if (fstat(fd, &sb) == -1)
        error_exit("Error: Stat failed");

    char *mapped = mmap(NULL, sb.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, OFFSET);
    if (mapped == MAP_FAILED) {
        error_exit("Error: Mmap failed");
    }

    printf("File mapped into memory /proc/self/maps\n");
    show_proc();
    getchar();

    printf("File contents before changes:\n%s\n", mapped);
    getchar();

    memcpy(mapped, NEW_DATA, strlen(NEW_DATA));
    printf("New data:%s\n", NEW_DATA);
    getchar();

    if (msync(mapped, FILE_SIZE, MS_SYNC) == -1) {
        error_exit("Error: Msync error");
    }
    printf("Msync successfully.\n");
    getchar();

    printf("File mapped into memory /proc/self/maps\n");
    show_proc();
    getchar();

    printf("Current file contents:\n%s\n", mapped);
    getchar();

    if (munmap(mapped, FILE_SIZE) == -1) {
        error_exit("Error: Munmap failed");
    }
    close(fd);

    printf("All resources released. Exiting program.\n");
    getchar();

    return 0;
}