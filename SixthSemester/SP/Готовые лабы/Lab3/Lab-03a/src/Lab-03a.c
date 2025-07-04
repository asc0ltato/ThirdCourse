#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ZSSLib.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("The function being called isn't specified!\n");
        return 1;
    }

    const char* func_name = argv[1];
    int number;

    if (argc < 3) {
        printf("Enter a number to search for: ");
        scanf("%d", &number);
    }
    else {
        number = atoi(argv[2]);
    }

    int index = -1;

    if (strcmp(func_name, "bsearch_i") == 0) {
        index = bsearch_i(barray, ARRAY_SIZE, number);
    }
    else if (strcmp(func_name, "bsearch_r") == 0) {
        index = bsearch_r(barray, number, 0, ARRAY_SIZE - 1);
    }
    else {
        printf("Unknown function: %s\n", func_name);
        return 1;
    }

    if (index == -1) {
        printf("%s: The specified number wasn't found!\n", func_name);
    }
    else {
        printf("%s: The number %d found in the position %d!\n", func_name, number, index);
    }

    return 0;
}