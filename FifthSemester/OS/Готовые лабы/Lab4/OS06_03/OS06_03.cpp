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

	processes[0] = createProcess(L"C:\\Users\\super\\Desktop\\ОС\\Готовые лабы\\Lab4\\Debug\\OS06_03A.exe").hProcess;
	processes[1] = createProcess(L"C:\\Users\\super\\Desktop\\ОС\\Готовые лабы\\Lab4\\Debug\\OS06_03B.exe").hProcess;

	HANDLE mutex = CreateMutex(NULL, FALSE, L"OS06_03");

	for (int i = 1; i <= 90; ++i)
	{
		if (i == 30) {
			WaitForSingleObject(mutex, INFINITE);
		}
		else if (i == 60) {
			ReleaseMutex(mutex);
		}

		printf("[OS06_03]\t %d.PID = %d\n", i, pid);
		Sleep(100);
	}

	WaitForMultipleObjects(2, processes, TRUE, INFINITE);
	CloseHandle(processes[0]);
	CloseHandle(processes[1]);
	CloseHandle(mutex);

	return 0;
}