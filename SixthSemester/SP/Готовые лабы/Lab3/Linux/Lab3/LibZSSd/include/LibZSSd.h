#ifndef ZSS_LIB_H
#define ZSS_LIB_H

#define ARRAY_SIZE 1024

extern const int barray[ARRAY_SIZE];

int bsearch_i(const int* a, int n, int x);
int bsearch_r(const int* a, int x, int i, int j);

void constructor(const char* log_file);
void destructor();

#endif