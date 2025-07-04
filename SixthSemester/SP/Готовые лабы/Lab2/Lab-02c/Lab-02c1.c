#include <windows.h>
#include <stdio.h>
#define FILE_MAPPING_NAME "Lab-02"
#define FILE_SIZE (640 * 1024) 
#define VIEW_SIZE (64 * 1024) 
#define N 10

void error_exit(const char* message) {
    fprintf(stderr, "%s. Error code: %lu\n", message, (unsigned long)GetLastError());
    exit(EXIT_FAILURE);
}

int main() {
    printf("Step 1: CreateMutex...\n");
    HANDLE hMutex = CreateMutex(NULL, FALSE, "Lab-02-Mutex");
    if (!hMutex) {
        error_exit("CreateMutex failed");
    }
    printf("CreateMutex successfully.\n");
    getchar();

    printf("Step 2 writer: CreateFileMapping...\n");
    HANDLE hMapping = CreateFileMapping(INVALID_HANDLE_VALUE, NULL,
        PAGE_READWRITE, 0, FILE_SIZE, FILE_MAPPING_NAME);
    if (!hMapping) {
        error_exit("CreateFileMapping failed");
    }
    printf("CreateFileMapping successfully.\n");
    getchar();

    printf("Step 3: MapViewOfFile...\n");
    LPVOID pViewWrite = MapViewOfFile(hMapping, FILE_MAP_WRITE, 0, 0, VIEW_SIZE);
    if (!pViewWrite) {
        error_exit("MapViewOfFile failed");
    }

    int* pData = (int*)pViewWrite;

    printf("Writer: Writing data...\n");
    for (int i = 0; i < N; i++) {
        WaitForSingleObject(hMutex, INFINITE);

        for (int j = 0; j < (VIEW_SIZE / sizeof(int)); j++) {
            pData[j] = i + j;
            printf("%d. pData[j] = %d\n", i, pData[j]);
        }

        printf("tap...\n");
        getchar();

        ReleaseMutex(hMutex);
        Sleep(500); 
    }

    printf("MapViewOfFile successfully.\n");
    getchar();

    UnmapViewOfFile(pViewWrite);
    CloseHandle(hMapping);
    CloseHandle(hMutex);

    printf("Writer: Finished.\n");
    getchar(); 

    return 0;
}