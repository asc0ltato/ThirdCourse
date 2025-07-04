#define UNICODE
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define FILE_NAME L"2.txt"
#define FILE_MAPPING_NAME L"FileMapping"
#define FILE_SIZE (512 * 1024 * 1024) 
#define VIEW_OFFSET 65536
#define VIEW_SIZE (128 * 1024 * 1024)

void error_exit(const char* message) {
    fprintf(stderr, "%s. Error code: %lu\n", message, (unsigned long)GetLastError());
    exit(EXIT_FAILURE);
}

int main() {
    DWORD fileAttr = GetFileAttributes(FILE_NAME);
    if (fileAttr == INVALID_FILE_ATTRIBUTES) {
        printf("Creating and writing data...\n");

        FILE* fp;
        if (_wfopen_s(&fp, FILE_NAME, L"w") != 0) {
            error_exit("Failed to create file");
        }
        for (int i = 0; i < FILE_SIZE; i++) {
            fputc('A' + (i % 26), fp); 
        }
        fclose(fp);
    }
    else {
        printf("Skipping file creation.\n");
    }

    printf("Step 1: CreateFile...\n");
    HANDLE hFile = CreateFile(FILE_NAME, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE,
        NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE) {
        error_exit("Failed to create or open file");
    }
    printf("File opened successfully.\n");
    getchar();

    printf("Step 2: CreateFileMapping...\n");
    HANDLE hMapping = CreateFileMapping(hFile, NULL, PAGE_READWRITE, 0, FILE_SIZE, FILE_MAPPING_NAME);
    if (hMapping == NULL) {
        error_exit("CreateFileMapping failed.");
    }
    printf("File mapping created successfully.\n");
    getchar();

    printf("Step 3: MapViewOfFile...\n");
    LPVOID pViewRead = MapViewOfFile(hMapping, FILE_MAP_READ, 0, VIEW_OFFSET, VIEW_SIZE);
    if (!pViewRead) {
        error_exit("Failed to map file view for reading");
    }
    printf("MapViewOfFile successfully.\n");

    printf("\nStep 4: Printf...\n");
    getchar();

    printf("File content at offset %d:\n%.*s\n", VIEW_OFFSET, VIEW_SIZE, (char*)pViewRead);
    getchar();

    UnmapViewOfFile(pViewRead);

    printf("Memory-mapped file operations completed successfully.\n");
    getchar();
;
    CloseHandle(hMapping);
    CloseHandle(hFile);

    printf("All resources released. Exiting program.\n");
    getchar();

    return 0;
}