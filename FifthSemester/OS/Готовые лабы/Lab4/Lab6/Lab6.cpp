#include <iostream>
#include <windows.h>
#include <atomic>
using namespace std;

atomic<uint8_t> lock_flag(0);  

void lock() {
    while (true) {
        uint8_t result;
        __asm {
            lock bts byte ptr[lock_flag], 0
            setc result                  
        }
        if (result == 0) {          
            break;
        }
    }
}
    
void unlock() {
    __asm {
        lock btr byte ptr[lock_flag], 0
    }
}

DWORD WINAPI thread_func(LPVOID param) {
    const char* thread_name = static_cast<const char*>(param);
    DWORD pid = GetCurrentProcessId();
    DWORD tid = GetCurrentThreadId();

    for (int i = 1; i <= 60; ++i) {
        if (i == 20) {
            cout << thread_name << " пытается войти в критическую секцию (но у него не получается :O).\n";
            lock();  
            cout << thread_name << " вошел в критическую секцию.\n";
        }

        printf("%d.\tPID = %d\tTID = %u\tlock_flag: %d\tthread: %s\n", i, pid, tid, lock_flag.load(), thread_name);

        if (i == 40) {
            unlock();  
            cout << thread_name << " вышел из критической секции.\n";
        }

        Sleep(100);
    }
    cout << thread_name << " закончил свою работу.\n";
    return 0;
}

int main() {
    setlocale(LC_ALL, "Russian");

    HANDLE threads[2];
    threads[0] = CreateThread(NULL, 0, thread_func, (LPVOID)"Поток 1", 0, NULL);
    threads[1] = CreateThread(NULL, 0, thread_func, (LPVOID)"Поток 2", 0, NULL);

    WaitForMultipleObjects(2, threads, TRUE, INFINITE);

    CloseHandle(threads[0]);
    CloseHandle(threads[1]);

    return 0;
}