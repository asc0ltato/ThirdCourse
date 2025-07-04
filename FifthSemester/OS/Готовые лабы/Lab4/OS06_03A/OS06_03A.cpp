#include <iostream>
#include <Windows.h>
using namespace std;

int main()
{
	HANDLE mutex = OpenMutex(SYNCHRONIZE, FALSE, L"OS06_03");
	if (mutex == NULL) {
		cout << "OS06_03: Open Error Mutex \n";
	}
	else {
		cout << "OS06_03: Open Mutex \n";
	}

	DWORD pid = GetCurrentProcessId();

	for (int i = 1; i <= 90; ++i)
	{
		if (i == 30) {
			WaitForSingleObject(mutex, INFINITE);
		}
		else if (i == 60) {
			ReleaseMutex(mutex);
		}

		printf("[OS06_03A]\t%d.PID = %d\n", i, pid);
		Sleep(100);
	}

	CloseHandle(mutex);
}