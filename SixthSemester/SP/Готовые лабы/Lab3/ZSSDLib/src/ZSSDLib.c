#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <stdio.h>
#define LOG_FILE "C:\\Users\\ascoltat0\\Desktop\\Lab3\\ZSSDLib_log.txt"

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason, LPVOID lpReserved) {
    FILE* logFile = fopen(LOG_FILE, "a");

    if (logFile) {
        switch (ul_reason) {
        case DLL_PROCESS_ATTACH:
            fprintf(logFile, "DLL_PROCESS_ATTACH\n");
            break;
        case DLL_THREAD_ATTACH:
            fprintf(logFile, "DLL_THREAD_ATTACH\n");
            break;
        case DLL_THREAD_DETACH:
            fprintf(logFile, "DLL_THREAD_DETACH\n");
            break;
        case DLL_PROCESS_DETACH:
            fprintf(logFile, "DLL_PROCESS_DETACH\n");
            break;
        }
        fclose(logFile);
    }
    return TRUE;
}