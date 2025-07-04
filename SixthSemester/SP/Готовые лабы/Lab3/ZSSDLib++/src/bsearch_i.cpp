#include "ZSSLib.hpp"

int bsearch_i(const int* a, int n, int x) {
    int i = 0, j = n - 1;
    while (i <= j) {
        int k = i + ((j - i) / 2);
        if (a[k] == x) {
            return k;
        }
        else if (a[k] < x) {
            i = k + 1;
        }
        else {
            j = k - 1;
        }
    }
    return -1;
}