#define _CRT_SECURE_NO_WARNINGS
#include <locale.h>
#include <wchar.h>
#include <iostream>
#include <Windows.h>
using namespace std;

#define FILE_PATH L"C:/Users/super/Desktop/Lab7/OS09_01.txt"
#define READ_BYTES 500

BOOL printFileText(LPWSTR fileName)
{
	try
	{
		cout << "\n-------------RESULT-------------\n";
		HANDLE file = CreateFile(fileName, GENERIC_READ, NULL, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if (file == INVALID_HANDLE_VALUE) throw "ERROR: Create or open file failed";
		DWORD n = NULL;
		char buf[1024];
		ZeroMemory(buf, sizeof(buf));
		BOOL b = ReadFile(file, &buf, READ_BYTES, &n, NULL);
		if (!b) throw "ERROR: Read file failed";
		cout << buf << '\n';
		CloseHandle(file);
		return true;
	}
	catch (const char* ex)
	{
		cout << ex << " \n";
		return false;
	}
}

BOOL delRowFileTxt(LPWSTR fileName, DWORD row)
{
	char filepath[20];
	filepath[19] = '\0';
	wcstombs(filepath, fileName, 20);
	cout << "\n-------------DELETE ROW: " << row << "-------------\n";

	try
	{
		HANDLE file = CreateFile(fileName, GENERIC_READ, NULL, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if (file == INVALID_HANDLE_VALUE) throw "ERROR: Create or open file failed";
		DWORD n = NULL;
		char buf[1024];
		ZeroMemory(buf, sizeof(buf));
		BOOL b = ReadFile(file, &buf, sizeof(buf), &n, NULL);
		if (!b) throw "ERROR: Read file failed";
		cout << "\nBEFORE:\n";
		cout << buf << '\n';
		CloseHandle(file);

		HANDLE hAppend = CreateFile(fileName, GENERIC_WRITE, NULL, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hAppend == INVALID_HANDLE_VALUE) throw "ERROR: Failed to open file for writing";
		char editedBuf[1024];
		ZeroMemory(editedBuf, sizeof(editedBuf));

		int line = 1;
		int j = 0;
		bool rowFile = false;
		for (int i = 0; i < n; i++)
		{
			if (line == row)
				rowFile = true;
			else
			{
				editedBuf[j] = buf[i];
				j++;
			}

			if (buf[i] == '\n')
				line++;
		}
		if (!rowFile) throw ("ERROR: Can't find this row\n");

		b = WriteFile(hAppend, editedBuf, n, &n, NULL);
		if (!b) throw "ERROR: Read file failed";
		CloseHandle(hAppend);
		return true;
	}
	catch (const char* ex)
	{
		cout << ex << " \n";
		return false;
	}
}

int main()
{
	LPWSTR file = (LPWSTR)FILE_PATH;

	delRowFileTxt(file, 1);
	delRowFileTxt(file, 3);
	delRowFileTxt(file, 8);
	delRowFileTxt(file, 10);

	printFileText(file);
	return 0;
}