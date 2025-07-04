#include <iostream>
#include <list>        
#include <stdio.h>
#include <tchar.h>
#include "Winsock2.h"
#include <string>
#include <windows.h>
#pragma comment(lib, "WS2_32.lib")
#pragma warning(disable : 4996)

using namespace std;

class RConsoleAPI {
public:
    RConsoleAPI(const string& pipeName) : namedPipeName(pipeName), hPipe(INVALID_HANDLE_VALUE) {}

    ~RConsoleAPI() {
        disconnect();
    }

    bool connect() {
        hPipe = CreateFileA(
            namedPipeName.c_str(),
            GENERIC_READ | GENERIC_WRITE,
            0,
            NULL,
            OPEN_EXISTING,
            0,
            NULL
        );

        if (hPipe == INVALID_HANDLE_VALUE) {
            cerr << "Не удалось подключиться к каналу: " << GetLastError() << endl;
            return false;
        }
        return true;
    }

    void disconnect() {
        if (hPipe != INVALID_HANDLE_VALUE) {
            CloseHandle(hPipe);
            hPipe = INVALID_HANDLE_VALUE;
        }
    }

    string sendCommand(const string& command) {
        DWORD bytesWritten;
        if (!WriteFile(hPipe, command.c_str(), command.size() + 1, &bytesWritten, NULL)) {
            cerr << "Ошибка при отправке команды: " << GetLastError() << endl;
            return "";
        }

        char buffer[128];
        DWORD bytesRead;
        if (!ReadFile(hPipe, buffer, sizeof(buffer), &bytesRead, NULL)) {
            cerr << "Ошибка при чтении ответа: " << GetLastError() << endl;
            return "";
        }

        return string(buffer, bytesRead);
    }

private:
    string namedPipeName;
    HANDLE hPipe;
};

int main() {
    setlocale(0, "");
    RConsoleAPI api("\\\\.\\pipe\\Tube");

    if (!api.connect()) {
        cerr << "Не удалось подключиться к серверу." << endl;
        return 1;
    }

    int commandNum = 0;
    string command;
    while (true) {
        cout << "Введите команду\n 1 - START\n 2 - STOP\n 3 - WAIT\n 4 - SHUTDOWN\n 5 - STATISTICS\n 6 - EXIT\n ";
        cin >> commandNum;
        cout << "\n---------- ---------- ---------- ----------\n\n";
        switch (commandNum) {
        case 1: command = "START"; break;
        case 2: command = "STOP"; break;
        case 3: command = "WAIT"; break;
        case 4: command = "SHUTDOWN"; break;
        case 5: command = "STATISTICS"; break;
        case 6: command = "EXIT"; break;
        default: cout << "Введена неверная команда"; continue;
        }

        string response = api.sendCommand(command);
        cout << "Ответ сервера: " << response << endl;

        if (command == "EXIT") {
            break;
        }
    }

    api.disconnect();
    return 0;
}