#include <iostream>
#include <ctime>
#include <Windows.h>
using namespace std;
bool active = true;

DWORD WINAPI stop(HANDLE htimer) {
    WaitForSingleObject(htimer, INFINITE);
    active = false;
    return 0;
}

bool isSimple(int n) {
    if (n < 2) return false;
    for (int i = 2; i * i <= n; i++) {
        if (n % i == 0) return false;
    }
    return true;
}

int main(int argc, char* argv[])
{
    clock_t start = clock();
    setlocale(LC_ALL, "Russian");
    int val = 1;
    int count = 1;
    int parm = (argc > 1) ? atoi(argv[1]) : 1;

    HANDLE htimer = OpenWaitableTimer(TIMER_ALL_ACCESS, FALSE, parm == 1 ? L"timer1" : L"timer2");

    if (htimer == NULL) {
        cout << "Error htimer.\n";
        return 1;
    }

    cout << "Child Process " << parm << " start" << endl;
    HANDLE hChild = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)stop, htimer, 0, NULL);

    while (active) {
        if (isSimple(val)) {
            cout << count << ". " << val << endl;
            count++;
        }
        val++;
    }

    double second = double(clock() - start) / CLOCKS_PER_SEC;
    cout << "Процесс " << parm << ". Прошло секунд: " << second << endl;

    WaitForSingleObject(hChild, INFINITE);
    CloseHandle(htimer);

    return 0;
}