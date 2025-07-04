#include <iostream>
#include <Windows.h>
using namespace std;

int main()
{
	for (int i = 1; i <= 10000; ++i)
	{
		cout << i << ". PID = " << GetCurrentProcessId() << " TID = " << GetCurrentThreadId() << "\n";
		Sleep(1000);
	}
}