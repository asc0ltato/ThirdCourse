#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>
#include <vector>
#include <thread>
#include <chrono>
#pragma comment(lib, "ws2_32.lib")
#pragma warning(disable : 4996)

using namespace std;

#define SERVER_PORT 2000
#define BUFFER_SIZE 50
#define BROADCAST_IP "255.255.255.255"

class ClientAPI {
public:
    ClientAPI() : clientSocket(INVALID_SOCKET) {
        WSADATA wsaData;
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
            throw runtime_error("WSAStartup failed");
        }
    }

    ~ClientAPI() {
        CloseConnection();
        WSACleanup();
    }

    void ConnectToServerWithNik(const char* msg) {
        clientSocket = socket(AF_INET, SOCK_DGRAM, 0);

        sockaddr_in broadcastAddr;
        broadcastAddr.sin_family = AF_INET;
        broadcastAddr.sin_port = htons(SERVER_PORT);
        broadcastAddr.sin_addr.s_addr = inet_addr(BROADCAST_IP);

        const char broadcastPermission = 1;
        setsockopt(clientSocket, SOL_SOCKET, SO_BROADCAST, &broadcastPermission, sizeof(broadcastPermission));

        sendto(clientSocket, msg, strlen(msg), 0, (sockaddr*)&broadcastAddr, sizeof(broadcastAddr));

        char buffer[BUFFER_SIZE];
        sockaddr_in serverAddr;
        int addrLen = sizeof(serverAddr);
        int result = recvfrom(clientSocket, buffer, BUFFER_SIZE, 0, (sockaddr*)&serverAddr, &addrLen);
        if (result > 0) {
            serverIP = inet_ntoa(serverAddr.sin_addr);
            serverPort = ntohs(serverAddr.sin_port);
            cout << "Server found at " << serverIP << ":" << serverPort << endl;
        }
        else {
            throw runtime_error("No server response");
        }

        clientSocket = socket(AF_INET, SOCK_STREAM, 0);
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(serverPort);
        inet_pton(AF_INET, serverIP.c_str(), &serverAddr.sin_addr);

        if (connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
            throw runtime_error("Connection to server failed");
        }
    }

    void ConnectToServerByName(const string& serverName, unsigned short port) {
        struct hostent* s = gethostbyname(serverName.c_str());
        if (s == NULL) {
            throw runtime_error("Сервер не найден");
        }

        sockaddr_in serverAddr;
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(port);
        serverAddr.sin_addr = *(struct in_addr*)s->h_addr_list[0];

        clientSocket = socket(AF_INET, SOCK_STREAM, 0);
        if (clientSocket == INVALID_SOCKET) {
            throw runtime_error("Socket creation failed");
        }

        if (connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
            throw runtime_error("Connection to server failed");
        }

        cout << "Подключено к серверу: " << serverName << " на порту " << port << endl;
    }

    void SendRequest(const string& request) {
        send(clientSocket, request.c_str(), request.length() + 1, 0);
    }

    string ReceiveResponse() {
        char buffer[BUFFER_SIZE];
        int result = recv(clientSocket, buffer, BUFFER_SIZE, 0);
        if (result > 0) {
            return string(buffer);
        }
        else {
            return "";
        }
    }

    void CloseConnection() {
        if (clientSocket != INVALID_SOCKET) {
            closesocket(clientSocket);
            clientSocket = INVALID_SOCKET;
        }
    }

private:
    SOCKET clientSocket;
    string serverIP;
    unsigned short serverPort;
};



int main() {
    setlocale(0, "ru");

    try {
        int connectionType, port;
        ClientAPI client;
        char name[50];

        cout << "1 - Подключение по имени сервера" << endl << "2 - Подключение по позывному" << endl << "3 - Default\n";
        cin >> connectionType;

        if (connectionType == 1) {
            cout << "Ведите имя сервера\n";
            cin >> name;
            cout << "Ведите порт\n";
            cin >> port;
            client.ConnectToServerByName(name, port);
        }
        else if (connectionType == 2) {
            cout << "Ведите позывной сервера\n";
            cin >> name;
            client.ConnectToServerWithNik(name);
        }
        else if (connectionType == 3) {
            client.ConnectToServerByName("DESKTOP-2SJTH0H", 2000);

        }
        else {
            cout << "Введен неправильный код\n";
            return 0;
        }

        cout << "Выберите сервис\n 1 - Echo\n 2 - Time\n 3 - Random\n";
        string service;
        int serviceNum;

            cin >> serviceNum;
            if (serviceNum == 4) {
                cout << "Выход из программмы";
                return 0;
            }

            switch (serviceNum) {
                case 1: service = "Echo"; break;
                case 2: service = "Time"; break;
                case 3: service = "Random"; break;
                default: cout << "Invalid service requestes\n";
            }

            client.SendRequest(service);
            string response = client.ReceiveResponse();
            cout << "Ответ сервера: " << response << endl;
            cout << "---------- ---------- ----------\n";
        }
    catch (const exception& e) {
        cerr << "Error: " << e.what() << endl; 
    }

    return 0;
}
