#include <iostream>
#include <Windows.h>
using namespace std;

int main()
{
	HANDLE event = OpenEvent(EVENT_ALL_ACCESS, FALSE, L"OS06_05");
	if (event == NULL) {
		cout << "OS06_04: Open Error Event \n";
	}
	else {
		cout << "OS06_04: Open Event \n";
	}

	DWORD pid = GetCurrentProcessId();

	WaitForSingleObject(event, INFINITE);
	SetEvent(event);

	for (int i = 1; i <= 90; i++)
	{
		printf("[OS06_05B]\t %d.  PID = %d\n", i, pid);
		Sleep(100);
	}

	CloseHandle(event);
}