#include <iostream>
#include <Windows.h>
using namespace std;

int main()
{
	HANDLE semaphore = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, L"OS06_04");
	if (semaphore == NULL) {
		cout << "OS06_04: Open Error Semaphore \n";
	}
	else {
		cout << "OS06_04: Open Semaphore \n";
	}

	DWORD pid = GetCurrentProcessId();

	for (int i = 1; i <= 90; ++i)
	{
		if (i == 30) {
			WaitForSingleObject(semaphore, INFINITE);
		}
		else if (i == 60) {
			ReleaseSemaphore(semaphore, 1, NULL);
		}

		printf("[OS06_04A]\t%d.PID = %d\n", i, pid);
		Sleep(100);
	}

	CloseHandle(semaphore);
}