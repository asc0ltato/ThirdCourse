#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

__declspec(dllimport) int bsearch_i(const int*, int, int);
__declspec(dllimport) int recursive_search(const int*, int, int, int);
__declspec(dllimport) const int barray[1024]; 

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
        index = bsearch_i(barray, 1024, number);
    }
    else if ((strcmp(func_name, "recursive_search") == 0) || (strcmp(func_name, "?bsearch_r@@YAHPBHHHH@Z") == 0)) {
        index = recursive_search(barray, number, 0, 1023);
    }

    if (index == -1) {
        printf("%s: The specified number wasn't found!\n", func_name);
    }
    else {
        printf("%s: The number %d found in the position %d!\n", func_name, number, index);
    }

    return 0;
}