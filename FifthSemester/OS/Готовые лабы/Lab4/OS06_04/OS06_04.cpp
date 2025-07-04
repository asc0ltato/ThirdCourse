#include <iostream>
#include <Windows.h>
using namespace std;

PROCESS_INFORMATION createProcess(LPCWSTR path)
{
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&si, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);

	if (CreateProcess(path, NULL, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi)) {
		cout << "Process created\n";
	}
	else {
		cout << "Process not created\n";
	}

	return pi;
}

int main()
{
	DWORD pid = GetCurrentProcessId();
	HANDLE processes[2];

	processes[0] = createProcess(L"C:\\Users\\super\\Desktop\\ОС\\Готовые лабы\\Lab4\\Debug\\OS06_04A.exe").hProcess;
	processes[1] = createProcess(L"C:\\Users\\super\\Desktop\\ОС\\Готовые лабы\\Lab4\\Debug\\OS06_04B.exe").hProcess;

	HANDLE semaphore = CreateSemaphore(NULL, 2, 2, L"OS06_04");

	for (int i = 1; i <= 90; ++i)
	{
		if (i == 30) {
			WaitForSingleObject(semaphore, INFINITE);
		}
		else if (i == 60) {
			ReleaseSemaphore(semaphore, 1, NULL);
		}

		printf("[OS06_04]\t %d.PID = %d\n", i, pid);
		Sleep(100);
	}

	WaitForMultipleObjects(2, processes, TRUE, INFINITE);
	CloseHandle(processes[0]);
	CloseHandle(processes[1]);
	CloseHandle(semaphore);

	return 0;
}