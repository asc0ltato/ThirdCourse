#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <chrono>
#include <thread>
#include <atomic>
#include <vector>
#include <algorithm>

#pragma comment(lib, "ws2_32.lib")

#define SERVER_PORT 12345
#define NTP_SERVER "150.254.190.51"  // Замените на ваш NTP сервер
#define NTP_PORT 123
#define NTP_TIMESTAMP_DELTA 2208988800ull
#define UPDATE_INTERVAL 10 // Интервал обновления (10 секунд)
#define CORRECTION_HISTORY_SIZE 10

std::atomic<uint64_t> Cs(0);
std::vector<int> correctionHistory(CORRECTION_HISTORY_SIZE, 0);
int correctionHistoryIndex = 0;

struct NTPPacket {
    uint8_t li_vn_mode;
    uint8_t stratum;
    uint8_t poll;
    uint8_t precision;
    uint32_t root_delay;
    uint32_t root_dispersion;
    uint32_t reference_id;
    uint32_t reference_timestamp[2];
    uint32_t originate_timestamp[2];
    uint32_t receive_timestamp[2];
    uint32_t transmit_timestamp[2];
};
struct SETSINCRO		///ответ С на синхр счетчика времени
{
    std::string cmd;			///всегда значение SINCRO
    int correction;		///знач, кот надо прибавить к знач счетчика t
};

// Функция для получения времени с NTP-сервера
uint64_t getNTPTime() {
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    SOCKET sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock == INVALID_SOCKET) {
        std::cerr << "[ERROR] Socket creation failed: " << WSAGetLastError() << std::endl;
        return 0;
    }

    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(NTP_PORT);

    // Используем inet_pton для конвертации IP-адреса
    if (inet_pton(AF_INET, NTP_SERVER, &serverAddr.sin_addr) <= 0) {
        std::cerr << "[ERROR] Invalid address/Address not supported." << std::endl;
        closesocket(sock);
        WSACleanup();
        return 0;
    }

    NTPPacket packet = {};
    packet.li_vn_mode = (0 << 6) | (4 << 3) | 3; // Версия 4, режим клиента

    // Отправляем запрос
    int sent = sendto(sock, (char*)&packet, sizeof(packet), 0, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    if (sent == SOCKET_ERROR) {
        std::cerr << "[ERROR] Failed to send data: " << WSAGetLastError() << std::endl;
        closesocket(sock);
        WSACleanup();
        return 0;
    }

    socklen_t addrLen = sizeof(serverAddr);
    int recvLen = recvfrom(sock, (char*)&packet, sizeof(packet), 0, (struct sockaddr*)&serverAddr, &addrLen);
    if (recvLen == SOCKET_ERROR) {
        std::cerr << "[ERROR] Failed to receive data: " << WSAGetLastError() << std::endl;
        closesocket(sock);
        WSACleanup();
        return 0;
    }

    closesocket(sock);
    WSACleanup();

    // Преобразуем время (получаем временную метку в секундах и наносекундах)
    uint64_t ntp_time = ntohl(packet.transmit_timestamp[0]) - NTP_TIMESTAMP_DELTA;
    if (ntp_time == 0) {
        std::cerr << "[ERROR] NTP time is 0! Something went wrong with the conversion." << std::endl;
        return 0;
    }

    // Получаем миллисекунды
    uint64_t milliseconds = ntp_time * 1000 + (ntohl(packet.transmit_timestamp[1]) * 1000 / 0xFFFFFFFF);

    std::cout << "[DEBUG] NTP Time in ms: " << milliseconds << " ms" << std::endl;

    return milliseconds;
}

// Функция для обновления времени
void updateTime() {
    while (true) {
        uint64_t new_time = getNTPTime();
        if (new_time > 0) {
            Cs.store(new_time);
            std::cout << "[SYNC] Updated Cs: " << Cs.load() << " ms\n";
        }
        else {
            std::cout << "[ERROR] Failed to sync time with NTP server." << std::endl;
        }
        std::this_thread::sleep_for(std::chrono::seconds(UPDATE_INTERVAL));
    }
}

// Функция для отображения текущего времени Cs каждые 10 секунд
void displayCsPeriodically() {
    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(UPDATE_INTERVAL));
        uint64_t current_Cs = Cs.load();
        std::cout << "[INFO] Current Cs: " << current_Cs << " ms\n";
    }
}

int calculateAverageCorrection() {
    int sum = 0;
    int count = min(CORRECTION_HISTORY_SIZE, (int)correctionHistory.size());
    for (int i = 0; i < count; ++i) {
        sum += correctionHistory[i];
    }
    return sum / count;
}
int setAverageCorrection(int averageCorrection[], int length)
{
    int value = 0;
    for (int i = 0; i < length; i++)
        value += averageCorrection[i];
    return value / length;
}
// Функция для работы сервера
void runServer() {

    SETSINCRO setsincro, getsincro;
    ZeroMemory(&setsincro, sizeof(setsincro));
    ZeroMemory(&getsincro, sizeof(getsincro));

    setsincro.cmd = "SINCRO";
    setsincro.correction = 0;
    SYSTEMTIME tm;

    clock_t cS;
    int averageCorrection[10];

    std::cout << "Server Run" << std::endl;

    WSADATA wsaData;

    if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0) {
        std::cerr << "[ERROR] Socket creation failed: " << WSAGetLastError() << std::endl;
        return;
    }

    SOCKET serverSock;
    SOCKET ssS;

    if ((serverSock = socket(AF_INET, SOCK_DGRAM, NULL)) == INVALID_SOCKET) {
        std::cerr << "[ERROR] Socket creation failed: " << WSAGetLastError() << std::endl;
        return;
    }

    SOCKADDR_IN serverAddr, clientAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY; // Используем INADDR_ANY для прослушивания на всех интерфейсах
    serverAddr.sin_port = htons(SERVER_PORT);

    if (bind(serverSock, (LPSOCKADDR)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "[ERROR] Bind failed: " << WSAGetLastError() << std::endl;
        closesocket(serverSock);
        WSACleanup();
        return;
    }

    std::cout << "[SERVER] Listening for client requests...\n";

    int count = 2;

    while (true) {
        SOCKADDR_IN client;
        int lc = sizeof(client);
        int average = 0;
        GetSystemTime(&tm);

        recvfrom(serverSock, (char*)&getsincro, sizeof(getsincro), NULL, (sockaddr*)&client, &lc);
        cS = clock();
        std::cout << cS << "\n";
        setsincro.correction = cS - getsincro.correction;
        averageCorrection[(count - 2) % 10] = setsincro.correction;	// заносим в массив
        average = setAverageCorrection(averageCorrection, min(count - 1, 10));
        std::cout << tm.wMonth << "." << tm.wDay << "." << tm.wYear << " - " << tm.wHour + 3 << ":" << tm.wMinute << ":" << tm.wSecond << "." << tm.wMilliseconds << "\n";
        std::cout << inet_ntoa(client.sin_addr) << " Correction = " << setsincro.correction << ", Average Correction = " << average << "\n\n\n";
        sendto(serverSock, (char*)&setsincro, sizeof(setsincro), 0, (sockaddr*)&client, sizeof(client));

        socklen_t clientLen = sizeof(clientAddr);
        char buffer[10];
        recvfrom(serverSock, buffer, sizeof(buffer), 0, (struct sockaddr*)&clientAddr, &clientLen);

        uint64_t current_Cs = Cs.load();
        sendto(serverSock, (char*)&current_Cs, sizeof(current_Cs), 0, (struct sockaddr*)&clientAddr, clientLen);
        std::cout << "[REQUEST] Sent Cs: " << current_Cs << " ms\n";

        count++;
    }

    closesocket(serverSock);
    WSACleanup();
}

int main() {
    // Запускаем поток для обновления времени (синхронизация с NTP сервером)
    std::thread timeThread(updateTime);

    // Запускаем поток для отображения значения Cs каждую секунду
    std::thread displayThread(displayCsPeriodically);

    // Запускаем сервер
    runServer();

    // Ожидаем завершения потоков
    timeThread.join();
    displayThread.join();

    return 0;
}
