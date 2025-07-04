#include <windows.h>
#include <iostream>
using namespace std;

DWORD WINAPI OS04_02_T1(LPVOID lpParam) {
    for (int i = 0; i < 50; ++i) {
        cout << i << ". PID = " << GetCurrentProcessId() << " TID-1 = " << GetCurrentThreadId() << "\n";
        Sleep(1000);
    }

    return 0;
}

DWORD WINAPI OS04_02_T2(LPVOID lpParam) {
    for (int i = 0; i < 125; ++i) {
        cout << i << ". PID = " << GetCurrentProcessId() << " TID-2 = " << GetCurrentThreadId() << "\n";
        Sleep(1000);
    }

    return 0;
}

int main() {
    HANDLE hThread1 = CreateThread(
        NULL,        //Атрибуты безопасности по умолчанию
        0,           //Размер стека по умолчанию
        OS04_02_T1,  
        NULL,        //Параметры для функции потока
        0,           //Запуск потока немедленно
        NULL         //Возвращаемый идентификатор потока
    );

    HANDLE hThread2 = CreateThread(NULL, 0, OS04_02_T2, NULL, 0, NULL);

    for (int i = 0; i < 100; ++i) {
        cout << i << ". PID = " << GetCurrentProcessId() << " TID-MAIN = " << GetCurrentThreadId() << "\n";
        Sleep(1000);
    }

    WaitForSingleObject(hThread1, INFINITE);
    WaitForSingleObject(hThread2, INFINITE);
    CloseHandle(hThread1);
    CloseHandle(hThread2);

    system("pause");
    return 0;
}