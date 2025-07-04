#include <stdio.h>
#include <stdlib.h>
#include <locale.h>

int main(int argc, char *argv[])
{
    int number = atoi(argv[1]);
    FILE *file = fopen("./OS09_05.txt", "r");
    FILE *outFile = NULL;
    char line[1024];
    int rowNumber = 0;

    if (number % 2 != 0) {
        outFile = fopen("OS09_06_1.txt", "w");
    } else {
        outFile = fopen("OS09_06_2.txt", "w");
    }

    while (fgets(line, sizeof(line), file)) {
        rowNumber++;
        if ((number % 2 != 0 && rowNumber % 2 != 0) || (number % 2 == 0 && rowNumber % 2 == 0)) {
            fputs(line, outFile);
        }
    }

    fclose(file);
    fclose(outFile);

    printf("Rows written to file\n");
    return 0;
}
