#include <iostream>
#include <windows.h>
#include <atomic>
#include <thread>
using namespace std;

int main() {
    setlocale(LC_ALL, "Russian");
    atomic<int> counter(0);
    bool running = true;
    HANDLE hTimer = CreateWaitableTimer(NULL, TRUE, NULL);

    if (!hTimer) {
        cout << "Error CreateWaitableTimer." << endl;
        return 1;
    }

    LARGE_INTEGER li;
    li.QuadPart = -30000000LL;
    DWORD startTime = GetTickCount();

    thread counterThread([&counter, &running]() {
        while (running) {
            counter++;
        }
    });

    while (true) {
        if (!SetWaitableTimer(hTimer, &li, 0, NULL, NULL, FALSE)) {
            cout << "Error SetWaitableTimer." << endl;
            CloseHandle(hTimer);
            running = false;
            counterThread.join();
            return 1;
        }

        WaitForSingleObject(hTimer, INFINITE);

        cout << "Значение счётчика каждые 3 секунды: " << counter.load() << endl;

        DWORD elapsedTime = (GetTickCount() - startTime) / 1000;
        if (elapsedTime == 15) {
            cout << "Итоговое значение счётчика после 15 секунд: " << counter.load() << endl;
            break;
        }
    }

    CloseHandle(hTimer);
    running = false;
    counterThread.join();
    return 0;
}