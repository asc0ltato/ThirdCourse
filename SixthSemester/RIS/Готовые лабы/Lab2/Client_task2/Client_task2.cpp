#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <string>
#include <fstream>
#include <ctime>
#include "Winsock2.h"
#include <thread>
#include <vector>
#pragma comment(lib, "WS2_32.lib")

using namespace std;

typedef void* HDFS;

// --------------------- Работа с критическими секциями через сервер ---------------------
struct CA {
    char ipaddr[15];
    char resurce[20];
    int clientnumber;
    enum Status { NOINIT, INIT, ENTER, LEAVE, WAIT } status;
    SOCKET socket;
    SOCKADDR_IN serverAddr;
};

string GetStatusText(CA::Status status) {
    switch (status) {
    case CA::NOINIT: return "NOINIT";
    case CA::INIT:   return "INIT";
    case CA::ENTER:  return "ENTER";
    case CA::LEAVE:  return "LEAVE";
    case CA::WAIT:   return "WAIT";
    default:         return "UNKNOWN";
    }
}


// --------------------- Инициализация критической секции с подключением к серверу ---------------------
CA InitCA(char ipaddr[15], char resurce[20], int number) {
    CA result;
    strcpy(result.ipaddr, ipaddr);
    strcpy(result.resurce, resurce);
    result.clientnumber = number;
    result.status = CA::NOINIT;

    // Инициализация сокетов
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0) {
        throw runtime_error("WSAStartup failed");
    }

    result.socket = socket(AF_INET, SOCK_DGRAM, NULL);
    if (result.socket == INVALID_SOCKET) {
        throw runtime_error("Socket creation failed");
    }

    result.serverAddr.sin_family = AF_INET;
    result.serverAddr.sin_port = htons(2000);
    result.serverAddr.sin_addr.s_addr = inet_addr(result.ipaddr);

    // Отправляем команду на сервер об инициализации
    string message = "i " + string(result.resurce);
    int sentBytes = sendto(result.socket, message.c_str(), message.size(), 0,
        (sockaddr*)&result.serverAddr, sizeof(result.serverAddr));
    if (sentBytes == SOCKET_ERROR) {
        throw runtime_error("Failed to send init message");
    }

    result.status = CA::INIT;
    cout << "Section initialized: " << result.resurce << endl;

    return result;
}


// --------------------- Вход в критическую секцию через сервер ---------------------
bool EnterCA(CA& ca) {
    string message = "e " + string(ca.resurce);
    sendto(ca.socket, message.c_str(), message.size(), 0,
        (sockaddr*)&ca.serverAddr, sizeof(ca.serverAddr));

    char buffer[10];
    int serverAddrSize = sizeof(ca.serverAddr);
    int bytesReceived = recvfrom(ca.socket, buffer, sizeof(buffer), 0,
        (sockaddr*)&ca.serverAddr, &serverAddrSize);

    if (bytesReceived == SOCKET_ERROR) {
        cout << "Ошибка при получении ответа сервера" << endl;
        return false;
    }

    ca.status = CA::ENTER;
    cout << "Вход в секцию: " << ca.resurce << endl;
    return true;
}

// --------------------- Выход из критической секции через сервер ---------------------
bool LeaveCA(CA& ca) {
    string message = "l " + string(ca.resurce);
    sendto(ca.socket, message.c_str(), message.size(), 0,
        (sockaddr*)&ca.serverAddr, sizeof(ca.serverAddr));

    ca.status = CA::LEAVE;
    cout << "Выход из секции: " << ca.resurce << endl;
    return true;
}

// --------------------- Закрытие критической секции ---------------------
bool CloseCA(CA& ca) {
    string message = "c " + string(ca.resurce);
    sendto(ca.socket, message.c_str(), message.size(), 0,
        (sockaddr*)&ca.serverAddr, sizeof(ca.serverAddr));

    closesocket(ca.socket);
    WSACleanup();

    ca.status = CA::NOINIT;
    cout << "Закрытие секции: " << ca.resurce << endl;
    return true;
}

// --------------------- API DFS ---------------------
// Открытие файла
HDFS OpenDFSFile(const char* FileName) {
    fstream* file = new fstream();
    file->open(FileName, ios::in | ios::out | ios::app); // Открываем файл в режиме чтения и записи
    if (!file->is_open()) {
        file->open(FileName, ios::out); // Если файл не открыт — создаём его
        file->close();
        file->open(FileName, ios::in | ios::out | ios::app);
    }

    if (file->is_open()) {
        cout << "Файл открыт: " << FileName << endl;
        return (HDFS)file;
    }
    cout << "Ошибка открытия файла: " << FileName << endl;
    delete file;
    return nullptr;
}

// Чтение из файла
int ReadDFSFile(HDFS hdfs, void* buf, int bufsize) {
    fstream* file = (fstream*)hdfs;
    file->read((char*)buf, bufsize);
    return file->gcount();
}

// Запись в файл
int WriteDFSFile(HDFS hdfs, void* buf, int bufsize) {
    fstream* file = (fstream*)hdfs;
    file->write((char*)buf, bufsize);
    return bufsize;
}

// Закрытие файла
void CloseDFSFile(HDFS hdfs) {
    fstream* file = (fstream*)hdfs;
    file->close();
    delete file;
}


// --------------------- Основная функция ---------------------
int main() {
    setlocale(LC_ALL, "rus");

    CA section = InitCA((char*)"172.20.10.2", (char*)"Z:\\test.txt", 1);

    try {
        while (true) {
            cout << "\n1. Write to file\n2. Read from file\n3. Exit\n";
            int choice;
            cin >> choice;
            if (choice == 1) {
                if (EnterCA(section)) {
                    HDFS hdfs = OpenDFSFile("Z:\\test.txt");
                    for (int i = 0; i < 10; ++i) {
                        time_t rawtime;
                        struct tm* timeinfo;
                        time(&rawtime);
                        timeinfo = localtime(&rawtime);
                        string data = "Client " + to_string(section.clientnumber) +
                            " Line " + to_string(i + 1) +
                            " Time :" + asctime(timeinfo);
                        data.pop_back(); 
                        data += "\n";
                        WriteDFSFile(hdfs, (void*)data.c_str(), data.size());
                    }
                    CloseDFSFile(hdfs);
                    LeaveCA(section);
                    cout << "\n10 строк успешно записано в файл." << endl;
                }
            }

            if (choice == 2) {
                cout << "\nВведите с какой строки читать:\n";
                int startLine;
                cin >> startLine;

                if (EnterCA(section)) {
                    HDFS hdfs = OpenDFSFile("Z:\\test.txt");
                    vector<string> lines;
                    string line;
                    int lineCount = 0;

                    char buffer[128] = { 0 };
                    int bytesRead = 0;
                    string leftover = "";  

                    while ((bytesRead = ReadDFSFile(hdfs, buffer, sizeof(buffer))) > 0) {
                        leftover += string(buffer, bytesRead);  

                        size_t pos;
                        while ((pos = leftover.find('\n')) != string::npos) {
                            line = leftover.substr(0, pos); 
                            leftover = leftover.substr(pos + 1);  
                            lines.push_back(line);  
                            lineCount++;
                        }
                    }

  
                    if (startLine < 1 || startLine > lineCount) {
                        cout << "Ошибка: введен неверный номер строки для начала чтения.\n";
                        CloseDFSFile(hdfs);
                        LeaveCA(section);
                        return 1;
                    }

                    cout << "\nЧтение из файла, начиная с " << startLine << " строки:\n";
                    int linesRead = 0;

                    for (int i = startLine - 1; i < lineCount && linesRead < 10; i++) {
                        cout << lines[i] << endl;
                        linesRead++;
                    }

                    cout << linesRead << " строк успешно считано из файла." << endl;
                    CloseDFSFile(hdfs);
                    LeaveCA(section);
                }
            }

            if (choice == 3) {
                CloseCA(section);
                break;
            }
        }
    }
    catch (const exception& e) {
        cout << "Ошибка: " << e.what() << endl;
        CloseCA(section);
    }

    return 0;
}