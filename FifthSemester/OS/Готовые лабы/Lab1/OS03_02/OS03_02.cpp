#include <iostream>
#include <Windows.h>
using namespace std;

int main() {
	cout << "OS03_02\n\n";

	LPCWSTR OS03_02_1 = L"C:\\Users\\super\\Desktop\\Lab1\\Debug\\OS03_02_01.exe";
	LPCWSTR OS03_02_2 = L"C:\\Users\\super\\Desktop\\Lab1\\Debug\\OS03_02_02.exe";
	STARTUPINFO si1;								
	STARTUPINFO si2;
	PROCESS_INFORMATION pi1;						
	PROCESS_INFORMATION pi2;

	ZeroMemory(&si1, sizeof(si1)); 	 
	si1.cb = sizeof(si1);  

	if (CreateProcess( OS03_02_1, NULL, NULL, NULL,				
		FALSE,				// не наследуем дескрипторы
		CREATE_NEW_CONSOLE, // флаги создания процесса, этот создает новый инстанс консоли
		NULL, NULL,				
		&si1,				// внешний вид окна
		&pi1))				// дескрипторы процесса и первичного потока
		cout << "--Process OS03_02_1 created\n";
	else cout << "--Process OS03_02_2 not created\n\n";

	ZeroMemory(&si2, sizeof(si2));
	si2.cb = sizeof(si2);

	if (CreateProcess(OS03_02_2, NULL, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si2, &pi2))
		cout << "--Process OS03_02_2 created\n";
	else cout << "--Process OS03_02_2 not created\n\n";

	for (int i = 1; i <= 100; ++i) {
		cout << i << ". PID = " << GetCurrentProcessId() << "\n";
		Sleep(1000);
	}

	WaitForSingleObject(pi1.hProcess, INFINITE);	
	WaitForSingleObject(pi2.hProcess, INFINITE);	
	CloseHandle(pi1.hProcess);	
	CloseHandle(pi2.hProcess);
	return 0;
}
