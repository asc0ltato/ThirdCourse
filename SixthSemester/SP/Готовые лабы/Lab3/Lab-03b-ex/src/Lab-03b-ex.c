#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef int (*SearchFuncIterative)(const int*, int, int);
typedef int (*SearchFuncRecursive)(const int*, int, int, int);
typedef const int* barray;

FARPROC GetFunctionByIndex(HMODULE hLib, int funcIndex) {
    if (funcIndex == 1) {
        return GetProcAddress(hLib, (LPCSTR)(uintptr_t)1);
    }
    else if (funcIndex == 2) {
        return GetProcAddress(hLib, (LPCSTR)(uintptr_t)2);
    }
    return NULL;
}

barray LoadArray(HMODULE hLib) {
    barray arr = (barray)GetProcAddress(hLib, "barray");
    if (!arr) {
        arr = (barray)GetProcAddress(hLib, (LPCSTR)(uintptr_t)3);
    }
    return arr;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("The function being called isn't specified!\n");
        return 1;
    }

    if (argc < 3) {
        printf("The function being called isn't specified!\n");
        return 1;
    }

    const char* libName = argv[1];
    const char* funcName = argv[2];

    HMODULE hLib = LoadLibraryA(libName);
    if (!hLib) {
        printf("The downloaded library wasn't found!\n");
        return 1;
    }

    FARPROC rawFunc = NULL;

    if (atoi(funcName) != 0) {
        int funcIndex = atoi(funcName);
        rawFunc = GetFunctionByIndex(hLib, funcIndex);
        if (!rawFunc) {
            printf("Function number isn't specified: %d\n", funcIndex);
            FreeLibrary(hLib);
            return 1;
        }
    }
    else {
        rawFunc = GetProcAddress(hLib, funcName);
    }

    if (!rawFunc) {
        printf("The desired function %s wasn't found in the library(GetProc) %s!\n", funcName, libName);
        FreeLibrary(hLib);
        return 1;
    }

    barray arr = LoadArray(hLib);
    if (!arr) {
        printf("Array error!\n");
        FreeLibrary(hLib);
        return 1;
    }

    int number;
    if (argc < 4) {
        printf("Enter a number to search for: ");
        scanf("%d", &number);
    }
    else {
        number = atoi(argv[3]);
    }

    int index = -1;

    if ((strcmp(funcName, "bsearch_i") == 0) || (strcmp(funcName, "?bsearch_i@@YAHPBHHH@Z") == 0) || (strcmp(funcName, "1") == 0)) {
        SearchFuncIterative searchFunc = (SearchFuncIterative)rawFunc;
        index = searchFunc(arr, 1024, number);
    }
    else if ((strcmp(funcName, "recursive_search") == 0) || (strcmp(funcName, "?bsearch_r@@YAHPBHHHH@Z") == 0) || (strcmp(funcName, "2") == 0)) {
        SearchFuncRecursive searchFunc = (SearchFuncRecursive)rawFunc;
        index = searchFunc(arr, number, 0, 1023);
    }
    else {
        printf("Unknown function: %s\n", funcName);
        return 1;
    }

    if (index == -1) {
        printf("%s: The specified number wasn't found!\n", funcName);
    }
    else {
        printf("%s: The number %d found in the position %d!\n", funcName, number, index);
    }

    FreeLibrary(hLib);
    return 0;
}