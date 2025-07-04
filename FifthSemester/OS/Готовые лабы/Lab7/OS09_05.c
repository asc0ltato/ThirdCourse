#include <stdio.h>
#include <stdlib.h>
#include <locale.h>

int main()
{
    FILE *file = fopen("./OS09_05.txt", "r");

    int rowNumber = 0;
    char line[1024];

    while (fgets(line, sizeof(line), file)) {
        rowNumber++;
    }

    fclose(file);
    printf("Number rows: %d\n", rowNumber);
    return 0;
}
