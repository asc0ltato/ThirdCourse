#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <string.h>

#define ARRAY_SIZE 1024

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("The library being called isn't specified!\n");
        return 1;
    }

    if (argc < 3) {
        printf("The function being called isn't specified!\n");
        return 1;
    }

    const char* libName = argv[1];
    const char* funcName = argv[2];

    void* handle = dlopen(libName, RTLD_LAZY);
    if (!handle) {
        printf("The downloaded library wasn't found!\n");
        return 1;
    }

    void* rawFunc = dlsym(handle, funcName);
    if (!rawFunc) {
        printf("The %s function wasn't found in %s library: %s\n", funcName, libName, dlerror());
        dlclose(handle);
        return 1;
    }

    int (*searchFunc)(const int *, int, int) = (int (*)(const int *, int, int)) rawFunc;

    int number = 0;
    if (argc < 4) {
        printf("Enter a number to search for: ");
        scanf("%d", &number);
    }
    else {
        number = atoi(argv[3]);
    }

    int arr[ARRAY_SIZE];
    for (int i = 0; i < ARRAY_SIZE; i++) arr[i] = i;

    int index = searchFunc(arr, ARRAY_SIZE, number);

    if (index == -1) {
        printf("%s: The specified number wasn't found!\n", funcName);
    }
    else {
        printf("%s: The number %d found in the position %d!\n", funcName, number, index);
    }

    dlclose(handle);
    return 0;
}