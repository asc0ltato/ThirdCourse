#ifndef ZSS_D_LIB_PP_H
#define ZSS_D_LIB_PP_H

#define ARRAY_SIZE 1024

#ifdef _WIN32
#define EXPORT __declspec(dllexport)
#else
#define EXPORT __attribute__((visibility("default")))
#endif

extern "C" EXPORT const int barray[ARRAY_SIZE];
EXPORT int bsearch_i(const int* a, int n, int x);
EXPORT int bsearch_r(const int* a, int x, int i, int j);

#endif 