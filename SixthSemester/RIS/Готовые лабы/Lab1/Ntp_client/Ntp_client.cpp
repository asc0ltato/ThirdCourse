#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <chrono>
#include <thread>
#include <climits>
#include <iomanip>
#include <string>
#include <cmath>

#pragma comment(lib, "ws2_32.lib")

//#define SERVER_IP "172.20.10.3"
#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 12345

using namespace std;
using namespace chrono;

struct GETSINCHRO	///запрос К на синхронизацию счетчика времени
{
    string cmd;		///всегда значение SINC
    int curvalue;	///тек. значение счетчика времени
};

uint64_t getOSTime() 
{
    auto now = system_clock::now();
    auto duration = duration_cast<milliseconds>(now.time_since_epoch());
    return duration.count();
}

void runClient() 
{
    SYSTEMTIME tm;
    GETSINCHRO getsincro, setsincro;
    ZeroMemory(&setsincro, sizeof(setsincro));
    ZeroMemory(&getsincro, sizeof(getsincro));
    getsincro.cmd = "SINC";
    getsincro.curvalue = 0;
    int Tc = 8000;

    cout << "Client run..." << endl;

    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    SOCKET cS = socket(AF_INET, SOCK_DGRAM, 0);
    if (cS == INVALID_SOCKET) {
        cerr << "[ERROR] Socket creation failed: " << WSAGetLastError() << endl;
        return;
    }

    SOCKADDR_IN serv;
    serv.sin_family = AF_INET;
    serv.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &serv.sin_addr);

    int maxcor = 0;
    int mincor = INT_MAX;
    int avgcorr = 0;
    int lensockaddr = sizeof(serv);
    uint64_t totalDiff = 0;

    SOCKET ccS = socket(AF_INET, SOCK_DGRAM, 0);;

    for (int i = 0; i < 10; ++i) {
        GetSystemTime(&tm);
        // Отправляем запрос с текущим временем клиента
        sendto(ccS, (char*)&getsincro, sizeof(getsincro), 0, (sockaddr*)&serv, sizeof(serv));
        // Получаем корректировку
        recvfrom(ccS, (char*)&setsincro, sizeof(setsincro), 0, (sockaddr*)&serv, &lensockaddr);

        int correction = setsincro.curvalue;

        if (i > 0) {
            maxcor = max(maxcor, correction);
            mincor = min(mincor, correction);
        }
        else if (i == 0) {
            maxcor = 0;
        }

        cout << tm.wMonth << "." << tm.wDay << "." << tm.wYear << " - " << (tm.wHour + 3) % 24 << ":" << tm.wMinute << ":" << tm.wSecond << "." << tm.wMilliseconds << "\n";
        cout << i + 1 << ") curvalue = " << getsincro.curvalue << " correction = " << correction << " max corr/min corr: " << maxcor << "/" << mincor << "\n";

        avgcorr += correction;

        char request[] = "sync";
        sendto(cS, request, sizeof(request), 0, (struct sockaddr*)&serv, sizeof(serv));

        uint64_t Cs;
        socklen_t ls = sizeof(serv);
        // Получаем текущее значение времени с сервера
        recvfrom(cS, (char*)&Cs, sizeof(Cs), 0, (struct sockaddr*)&serv, &ls);

        uint64_t ostime = getOSTime();
        int64_t diff = abs((int64_t)(Cs - ostime));
        if (i > 0) {
            totalDiff += diff;
        }

        cout << "[CLIENT] Cs (время сервера): " << Cs << " ms\n";
        cout << "[CLIENT] OSTime (время клиента): " << ostime << " ms\n";
        cout << "[CLIENT] Cs - OSTime: " << abs((int64_t)(Cs - ostime)) << " ms\n";
        cout << "------------------------------------------\n\n";

        getsincro.curvalue += correction + Tc;

        Sleep(Tc);
    }

    cout << "Average Cs - OStime: " << totalDiff / 9 << " ms\n";

    closesocket(cS);
    WSACleanup();
}

int main() 
{
    setlocale(LC_ALL, "Russian");
    runClient();
    getchar();
    return 0;
}