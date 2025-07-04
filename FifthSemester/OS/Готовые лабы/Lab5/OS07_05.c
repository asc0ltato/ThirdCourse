#include <stdio.h>
#include <time.h>

int main() {
    time_t t1 = 0;
    t1 = time(&t1);
    struct tm* tmptr;
    tmptr = localtime(&t1);

    printf("%02d.%02d.%04d %02d:%02d:%02d\n", 
           tmptr->tm_mday, tmptr->tm_mon + 1, tmptr->tm_year + 1900,
           tmptr->tm_hour, tmptr->tm_min, tmptr->tm_sec);

    return 0;
}
