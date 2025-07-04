#define _CRT_SECURE_NO_WARNINGS
#include <locale.h>
#include <wchar.h>
#include <Windows.h>
#include <iostream>
#include <fileapi.h>
#include <vector>
#include <string>
using namespace std;

#define FILE_PATH L"C:/Users/super/Desktop/Lab7/OS09_01.txt"
#define READ_BYTES 1000

BOOL printFileText(LPWSTR fileName) {
    try {
        cout << "\n-------------RESULT-------------\n";
        HANDLE file = CreateFile(fileName, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
        if (file == INVALID_HANDLE_VALUE) throw "ERROR: Create or open file failed";
        DWORD n = 0;
        char buf[1024];
        ZeroMemory(buf, sizeof(buf));
        BOOL b = ReadFile(file, buf, READ_BYTES, &n, NULL);
        if (!b) throw "ERROR: Read file failed";
        cout << buf << '\n';
        CloseHandle(file);
        return true;
    }
    catch (const char* ex) {
        cout << ex << " \n";
        return false;
    }
}

BOOL insRowFileTxt(LPWSTR fileName, LPWSTR str, DWORD row) {
    try {
        HANDLE file = CreateFile(fileName, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        if (file == INVALID_HANDLE_VALUE) throw "ERROR: Failed to open file for reading";

        DWORD bytesRead = NULL;
        char buf[1024];
        ZeroMemory(buf, sizeof(buf));
        BOOL b = ReadFile(file, buf, sizeof(buf), &bytesRead, NULL);
        CloseHandle(file);
        if (!b) throw "ERROR: Read file failed";

        string content(buf, bytesRead);
        vector<string> lines;
        size_t pos = 0;
        size_t start = 0;
        while ((pos = content.find("\r\n", start)) != string::npos) {
            lines.push_back(content.substr(start, pos - start));
            start = pos + 2;
        }
        if (start < content.length()) {
            lines.push_back(content.substr(start));
        }

        char tempBuffer[50];
        if (wcslen(str) >= sizeof(tempBuffer)) {
            throw "ERROR: String too large for buffer";
        }
        wcstombs(tempBuffer, str, sizeof(tempBuffer) - 1);
        tempBuffer[sizeof(tempBuffer) - 1] = '\0';
        string newLine = tempBuffer;

        if (row == 0) {
            lines.insert(lines.begin(), newLine);
        }
        else if (row == -1 || row > lines.size()) {
            lines.push_back(newLine);
        }
        else {
            lines.insert(lines.begin() + row - 1, newLine);
        }

        string editedString;
        for (const auto& line : lines) {
            editedString += line + "\r\n";
        }

        HANDLE hAppend = CreateFile(fileName, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
        if (hAppend == INVALID_HANDLE_VALUE) throw "ERROR: Failed to open file for writing";

        DWORD n = 0;
        b = WriteFile(hAppend, editedString.c_str(), editedString.size(), &n, NULL);
        CloseHandle(hAppend);
        if (!b) throw "ERROR: Write file failed";
        return true;
    }
    catch (const char* ex) {
        cout << ex << " \n";
        return false;
    }
}

int main() {
    LPWSTR file = (LPWSTR)FILE_PATH;
    char str[] = "Lab9";
    wchar_t wStr[50];
    mbstowcs(wStr, str, strlen(str) + 1);
    LPWSTR strToIns = wStr;

    insRowFileTxt(file, strToIns, 0);
    insRowFileTxt(file, strToIns, -1);
    insRowFileTxt(file, strToIns, 5);
    insRowFileTxt(file, strToIns, 7);

    printFileText(file);
}
