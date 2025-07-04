#include <iostream>
using namespace std;

int main() {
    time_t t1;
    tm ttm;
    t1 = time(&t1);
    localtime_s(&ttm, &t1);
   
    cout << (ttm.tm_mday < 10 ? "0" : "") << ttm.tm_mday << "."
        << (ttm.tm_mon + 1 < 10 ? "0" : "") << (ttm.tm_mon + 1) << "."
        << (ttm.tm_year + 1900) << " "
        << (ttm.tm_hour < 10 ? "0" : "") << ttm.tm_hour << ":"
        << (ttm.tm_min < 10 ? "0" : "") << ttm.tm_min << ":"
        << (ttm.tm_sec < 10 ? "0" : "") << ttm.tm_sec << "\n";

    return 0;
}