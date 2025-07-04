#include <windows.h>
#include <stdio.h>
#define FILE_MAPPING_NAME "Lab-02"
#define VIEW_SIZE (64 * 1024) 
#define N 10

void error_exit(const char* message) {
    fprintf(stderr, "%s. Error code: %lu\n", message, (unsigned long)GetLastError());
    exit(EXIT_FAILURE);
}

int main() {
    printf("Step 1: OpenMutex...\n");
    HANDLE hMutex = OpenMutex(SYNCHRONIZE, FALSE, "Lab-02-Mutex");
    if (!hMutex) {
        error_exit("OpenMutex failed");
    }
    printf("OpenMutex successfully.\n");
    getchar();

    printf("Step 2 reader: OpenFileMapping...\n");
    HANDLE hMapping = OpenFileMapping(FILE_MAP_READ, FALSE, FILE_MAPPING_NAME);
    if (!hMapping) {
        error_exit("OpenFileMapping failed");
    }
    printf("OpenFileMapping successfully.\n");
    getchar();

    printf("Step 3: MapViewOfFile...\n");
    LPVOID pViewRead = MapViewOfFile(hMapping, FILE_MAP_READ, 0, 0, VIEW_SIZE);
    if (!pViewRead) {
        error_exit("MapViewOfFile failed");
    }

    int* pData = (int*)pViewRead;

    printf("Reader: Reading data...\n");
    for (int i = 0; i < N; i++) {
        WaitForSingleObject(hMutex, INFINITE);

        for (int j = 0; j < (VIEW_SIZE / sizeof(int)); j++) {
            printf("%d. pData[j] = %d\n", i, pData[j]);
        }

        printf("tap...\n");
        getchar();

        ReleaseMutex(hMutex); 
        Sleep(500); 
    }

    printf("MapViewOfFile successfully.\n");
    getchar();

    UnmapViewOfFile(pViewRead);
    CloseHandle(hMapping);
    CloseHandle(hMutex);

    printf("Reader: Finished.\n");
    getchar();

    return 0;
}