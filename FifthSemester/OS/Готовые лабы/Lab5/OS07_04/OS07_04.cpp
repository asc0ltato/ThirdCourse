#include <iostream>
#include <Windows.h>
#include <ctime>
using namespace std;
#define SECOND 10000000

int main () {
    clock_t start = clock();
    setlocale(LC_ALL, "Russian");
    DWORD pid = GetCurrentProcessId();
    cout << "PID: " << pid << endl;

    HANDLE htimer1 = CreateWaitableTimer(NULL, TRUE, L"timer1");
    HANDLE htimer2 = CreateWaitableTimer(NULL, TRUE, L"timer2");

    long long it1 = -60 * SECOND;
    long long it2 = -120 * SECOND;

    if (!SetWaitableTimer(htimer1, (LARGE_INTEGER*)&it1, 0, NULL, NULL, FALSE)) {
        throw "Error SetWaitableTimer htimer1";
    }

    if (!SetWaitableTimer(htimer2, (LARGE_INTEGER*)&it2, 0, NULL, NULL, FALSE)) {
        throw "Error SetWaitableTimer htimer2";
    }

    LPCWSTR an = L"C:\\Users\\super\\Desktop\\ОС\\Готовые лабы\\Lab5\\Debug\\OS07_04x.exe";
    STARTUPINFO si1;
    STARTUPINFO si2;
    PROCESS_INFORMATION pi1;
    PROCESS_INFORMATION pi2;
    ZeroMemory(&si1, sizeof(STARTUPINFO));
    ZeroMemory(&si2, sizeof(STARTUPINFO));
    si1.cb = sizeof(STARTUPINFO);
    si2.cb = sizeof(STARTUPINFO);

    if (CreateProcess(an, (LPWSTR)L" 1", NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si1, &pi1)) {
        cout << "Process OS07_04x #1 was created\n";
    }
    else {
        cout << "Process OS07_04x #1 not created\n";
    }
        
    if (CreateProcess(an, (LPWSTR)L" 2", NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si2, &pi2)) {
        cout << "Process OS07_04x #2 was created\n";
    }
    else {
        cout << "Process OS07_04x #2 not created\n";
    }

    WaitForSingleObject(pi1.hProcess, INFINITE);
    WaitForSingleObject(pi2.hProcess, INFINITE);

    double second = double(clock() - start) / CLOCKS_PER_SEC;
    cout << "Прошло секунд: " << second << endl;

    CloseHandle(pi1.hProcess);
    CloseHandle(pi2.hProcess);

    CancelWaitableTimer(htimer1);
    CancelWaitableTimer(htimer2);

    system("pause");
    return 0;
}