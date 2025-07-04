#include <iostream>
#include <Windows.h>
#include "TlHelp32.h"
#include <iomanip>
using namespace std;

int main() {
	DWORD pid = GetCurrentProcessId();
	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPALL, 0);	// делает снапшот текущих процессов системы
	// 1: включает в снапшот все процессы и потоки системы, модули и стеки
	// 2: 0 - вкл все процессы в системе, а не конкретный
	PROCESSENTRY32 peProcessEntry;						
	peProcessEntry.dwSize = sizeof(PROCESSENTRY32);		
	wcout << L"Current PID: " << pid << endl << L"------------------------------" << endl;

	try
	{
		// извлекает инфу о первом процессе снапшота
		if (!Process32First(snapshot, &peProcessEntry)) throw L"Process32First";
		do
		{
			wcout << L"Name = " << peProcessEntry.szExeFile << endl
				  << L"PID = " << peProcessEntry.th32ProcessID
			      << L", Parent PID = " << peProcessEntry.th32ParentProcessID;
			if (peProcessEntry.th32ProcessID == pid)  wcout << "--> current process";
				wcout << endl << L"--------------------------------------" << endl;
		} while (Process32Next(snapshot, &peProcessEntry));
	}
	catch (char* errMessage)
	{
		wcout << L"ERROR: " << errMessage << endl;
	}

	system("pause");
	return 0;
}