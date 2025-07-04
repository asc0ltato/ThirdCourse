#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cstdlib>
#include "Windows.h"
using namespace std;

#define FILE_PATH L"C:/Users/super/Desktop/Lab7/OS09_01.txt"
#define DIR_PATH L"C:/Users/super/Desktop/Lab7"

BOOL printWatchRowFileTxt(LPWSTR fileName, DWORD mlsec)
{
    try
    {
        HANDLE notif = FindFirstChangeNotification(DIR_PATH, false, FILE_NOTIFY_CHANGE_LAST_WRITE);
        if (notif == INVALID_HANDLE_VALUE) throw "ERROR: Failed to set up notification";
        DWORD rowCount = 0;
        clock_t startTime = clock();
        cout << "\n-----------------------------------------------\n";
        printf("\tStarted filewatch (timestamp %d)", startTime);
        while (true)
        {
            DWORD dwWaitStatus = WaitForSingleObject(notif, mlsec);
            if (dwWaitStatus != WAIT_OBJECT_0) break;
            if (FindNextChangeNotification(notif) == FALSE) break;

            HANDLE file = CreateFile(fileName, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
            if (file == INVALID_HANDLE_VALUE) throw "ERROR: Failed to open file for reading";

            LARGE_INTEGER fileSize;
            if (!GetFileSizeEx(file, &fileSize)) throw "ERROR: Failed to get file size";

            char* buf = new char[fileSize.QuadPart + 1];
            ZeroMemory(buf, fileSize.QuadPart + 1);
            DWORD bytesRead;
            if (ReadFile(file, buf, fileSize.QuadPart, &bytesRead, NULL))
            {
                DWORD position = 0;
                DWORD newRowCount = 0;
                while (buf[position++] != '\0')
                {
                    if (buf[position] == '\n') newRowCount++;
                }

                if (rowCount != newRowCount)
                {
                    printf("\nRows: %d", newRowCount);
                    rowCount = newRowCount;
                }
            }

            CloseHandle(file);
        }
        CloseHandle(notif);
        clock_t endTime = clock();
        printf("\nEnded filewatch (timestamp %d)\n", endTime);
        return true;
    }
    catch (const char* ex) {
        cout << ex << " \n";
        return false;
    }
}

int main()
{
    LPWSTR fileName = (LPWSTR)FILE_PATH;
    printWatchRowFileTxt(fileName, 15000);
}