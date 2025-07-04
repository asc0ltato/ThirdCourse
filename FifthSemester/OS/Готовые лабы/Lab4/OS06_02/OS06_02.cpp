#include <iostream>
#include <windows.h>
using namespace std;
CRITICAL_SECTION cs;

DWORD WINAPI thread_func(LPVOID param)
{
	const char* thread_name = static_cast<const char*>(param);
	DWORD pid = GetCurrentProcessId();
	DWORD tid = GetCurrentThreadId();

	for (int i = 1; i <= 90; ++i)
	{
		if (i == 30) {
			cout << thread_name << " пытается войти в критическую секцию (но у него не получается :O).\n";
			EnterCriticalSection(&cs);
			cout << thread_name << " вошел в критическую секцию.\n";
		}

		printf("[%s]\t%d.PID = %d\tTID = %u\n", thread_name, i, pid, tid);

		if (i == 60) {
			LeaveCriticalSection(&cs);
			cout << thread_name << " вышел из критической секции.\n";
		}

		Sleep(100);
	}

	cout << "\n---------------  " << thread_name << " завершился" << "  ---------------\n\n";
	return 0;
}

int main()
{
	setlocale(LC_ALL, "Russian");

	DWORD pid = GetCurrentProcessId();

	HANDLE threads[2];
	threads[0] = CreateThread(NULL, 0, thread_func, (LPVOID)"A", 0, NULL);
	threads[1] = CreateThread(NULL, 0, thread_func, (LPVOID)"B", 0, NULL);

	InitializeCriticalSection(&cs);

	for (int i = 1; i <=90; ++i)
	{
		if (i == 30) {
			EnterCriticalSection(&cs);
		}

		printf("[MAIN]\t%d.PID = %d\n", i, pid);

		if (i == 60) {
			LeaveCriticalSection(&cs);
		}

		Sleep(100);
	}

	cout << "\n---------------  MAIN завершился  --------------- \n\n";

	WaitForMultipleObjects(2, threads, TRUE, INFINITE);

	CloseHandle(threads[0]);
	CloseHandle(threads[1]);

	DeleteCriticalSection(&cs);
	return 0;
}