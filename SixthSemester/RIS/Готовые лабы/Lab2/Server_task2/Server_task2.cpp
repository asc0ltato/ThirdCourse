#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include "string.h"
#include <locale>
#include "time.h"
#include "Winsock2.h"
#include <queue>
#include <tchar.h>
#pragma comment(lib, "WS2_32.lib")
using namespace std;

class Link
{
public:
	char* FileName; // Имя файла, с которым связан поток
	char command; // Команда от клиента (например, "e" или "l")
	bool UsingFile; // Флаг, указывающий используется ли файл
	SOCKADDR_IN Adr; // Адрес клиента (IP и порт)
	HANDLE WaitEvent; // Событие для синхронизации
	HANDLE Thread; // Поток, связанный с этим клиентом
};

#define MAX_FILE 10 // Максимальное количество потоков/файлов
Link* masthr[MAX_FILE]; // Массив ссылок на объекты Link
SOCKET sS;
int lc;

string GetErrorMsgText(int code) 
{
	string msgText;
	switch (code) 
	{
	case WSAEINTR: msgText = "WSAEINTR"; break;
	case WSAEACCES: msgText = "WSAEACCES"; break;
	case WSAEFAULT: msgText = "WSAEFAULT"; break;
	case WSAEINVAL: msgText = "WSAEINVAL"; break;
	case WSAEMFILE: msgText = "WSAEMFILE"; break;
	case WSAEWOULDBLOCK: msgText = "WSAEWOULDBLOCK"; break;
	case WSAEINPROGRESS: msgText = "WSAEINPROGRESS"; break;
	case WSAEALREADY: msgText = "WSAEALREADY"; break;
	case WSAENOTSOCK: msgText = "WSAENOTSOCK"; break;
	case WSAEDESTADDRREQ: msgText = "WSAEDESTADDRREQ"; break;
	case WSAEMSGSIZE: msgText = "WSAEMSGSIZE"; break;
	case WSAEPROTOTYPE: msgText = "WSAEPROTOTYPE"; break;
	case WSAENOPROTOOPT: msgText = "WSAENOPROTOOPT"; break;
	case WSAEPROTONOSUPPORT: msgText = "WSAEPROTONOSUPPORT"; break;
	case WSAESOCKTNOSUPPORT: msgText = "WSAESOCKTNOSUPPORT"; break;
	case WSAEOPNOTSUPP: msgText = "WSAEOPNOTSUPP"; break;
	case WSAEPFNOSUPPORT: msgText = "WSAEPFNOSUPPORT"; break;
	case WSAEAFNOSUPPORT: msgText = "WSAEAFNOSUPPORT"; break;
	case WSAEADDRINUSE: msgText = "WSAEADDRINUSE"; break;
	case WSAEADDRNOTAVAIL: msgText = "WSAEADDRNOTAVAIL"; break;
	case WSAENETDOWN: msgText = "WSAENETDOWN"; break;
	case WSAENETUNREACH: msgText = "WSAENETUNREACH"; break;
	case WSAENETRESET: msgText = "WSAENETRESET"; break;
	case WSAECONNABORTED: msgText = "WSAECONNABORTED"; break;
	case WSAECONNRESET: msgText = "WSAECONNRESET"; break;
	case WSAENOBUFS: msgText = "WSAENOBUFS"; break;
	case WSAEISCONN: msgText = "WSAEISCONN"; break;
	case WSAENOTCONN: msgText = "WSAENOTCONN"; break;
	case WSAESHUTDOWN: msgText = "WSAESHUTDOWN"; break;
	case WSAETIMEDOUT: msgText = "WSAETIMEDOUT"; break;
	case WSAECONNREFUSED: msgText = "WSAECONNREFUSED"; break;
	case WSAEHOSTDOWN: msgText = "WSAEHOSTDOWN"; break;
	case WSAEHOSTUNREACH: msgText = "WSAEHOSTUNREACH"; break;
	case WSAEPROCLIM: msgText = "WSAEPROCLIM"; break;
	case WSASYSNOTREADY: msgText = "WSASYSNOTREADY"; break;
	case WSAVERNOTSUPPORTED: msgText = "WSAVERNOTSUPPORTED"; break;
	case WSANOTINITIALISED: msgText = "WSANOTINITIALISED"; break;
	case WSAEDISCON: msgText = "WSAEDISCON"; break;
	case WSATYPE_NOT_FOUND: msgText = "WSATYPE_NOT_FOUND"; break;
	case WSAHOST_NOT_FOUND: msgText = "WSAHOST_NOT_FOUND"; break;
	case WSATRY_AGAIN: msgText = "WSATRY_AGAIN"; break;
	case WSANO_RECOVERY: msgText = "WSANO_RECOVERY"; break;
	case WSANO_DATA: msgText = "WSANO_DATA"; break;
	case WSA_INVALID_HANDLE: msgText = "WSA_INVALID_HANDLE"; break;
	case WSA_INVALID_PARAMETER: msgText = "WSA_INVALID_PARAMETER"; break;
	case WSA_IO_INCOMPLETE: msgText = "WSA_IO_INCOMPLETE"; break;
	case WSA_IO_PENDING: msgText = "WSA_IO_PENDING"; break;
	case WSA_NOT_ENOUGH_MEMORY: msgText = "WSA_NOT_ENOUGH_MEMORY"; break;
	case WSA_OPERATION_ABORTED: msgText = "WSA_OPERATION_ABORTED"; break;
	case WSASYSCALLFAILURE: msgText = "WSASYSCALLFAILURE"; break;
	default: msgText = "***ERROR***"; break;
	};
	return msgText;
};

string SetErrorMsgText(string msgText, int code)
{
	return  msgText + GetErrorMsgText(code);
};

// Выполняется для каждого клиента
DWORD WINAPI Dispath(LPVOID lp)
{
	string str = "e";
	string str2 = "l";

	queue<SOCKADDR_IN>* q = new queue<SOCKADDR_IN>(); // Хранение клиентов
	int index = (int)lp; // Индекс потока

	while (true)
	{
		// Ожидаем сигнал на выполнение потока
		WaitForSingleObject(masthr[index]->WaitEvent, INFINITE);

		// Если команда e, добавляем клиента в очередь
		if (string(1, masthr[index]->command) == str) {
			if (q->empty())
			{
				char buf[5];
				_itoa((masthr[index]->Adr).sin_port, buf, 10); // Преобразуем порт клиента в строку

				sendto(sS, buf, sizeof(buf), NULL, (sockaddr*)&masthr[index]->Adr, lc); // Отправляем порт обратно клиенту
			}
			q->push(masthr[index]->Adr); // Добавляем клиента в очередь
		}
		// Если команда l, отправляем информацию следующему клиенту в очереди
		else if (string(1, masthr[index]->command) == str2) {
			q->pop(); // Убираем клиента из очереди
			if (!q->empty()) { // Если в очереди есть еще клиенты
				cout << "lea" << endl;

				SOCKADDR_IN sc = q->front(); // Берем адрес клиента из очереди
				sendto(sS, (char*)(masthr[index]->Adr).sin_port, sizeof((char*)(masthr[index]->Adr).sin_port), NULL, (sockaddr*)&sc, lc); // Отправляем адрес следующему клиенту
			}
		}
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	setlocale(LC_ALL, "Russian");

	cout << "ServerU" << endl;
	try
	{
		for (int i = 0; i < MAX_FILE; i++)
		{
			masthr[i] = NULL; // Инициализируем массив ссылок на потоки
		}
		WSADATA wsaData;

		if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0)
			throw SetErrorMsgText("Startup: ", WSAGetLastError());

		if ((sS = socket(AF_INET, SOCK_DGRAM, NULL)) == INVALID_SOCKET)
			throw SetErrorMsgText("Socket: ", WSAGetLastError());

		SOCKADDR_IN serv;
		serv.sin_family = AF_INET;
		serv.sin_port = htons(2000);
		serv.sin_addr.s_addr = INADDR_ANY;

		if (bind(sS, (LPSOCKADDR)&serv, sizeof(serv)) == SOCKET_ERROR)
			throw SetErrorMsgText("Bind_Server: ", WSAGetLastError());

		while (true)
		{
			SOCKADDR_IN client;
			lc = sizeof(SOCKADDR_IN);
			char ibuf[50];
			ZeroMemory(ibuf, 50);

			// Получение запроса от клиента
			if ((recvfrom(sS, ibuf, sizeof(ibuf), NULL, (sockaddr*)&client, &lc)) == SOCKET_ERROR)
				throw SetErrorMsgText("RecvFrom: ", WSAGetLastError());

			int port = client.sin_port;
			cout << "Client (" << inet_ntoa(client.sin_addr) << ":" << port << "): " << ibuf << endl;

			bool create = true;
			for (int i = 0; i < MAX_FILE; i++) // Проверяем, существует ли уже поток для этого файла
			{
				if (masthr[i] != NULL)
				{
					if (strcmp(ibuf + 5, masthr[i]->FileName) == 0) // Если файл уже существует
					{
						create = false;
						masthr[i]->command = ibuf[0]; // Обновляем команду
						masthr[i]->Adr = client; // Обновляем адрес клиента
						SetEvent(masthr[i]->WaitEvent); // Устанавливаем событие для выполнения потока
						break;
					}
				}
			}
			if (create) // Если новый файл
			{
				Link* a = new Link();
				int pos = -1;
				bool findplace = false;

				for (int i = 0; i < MAX_FILE; i++) // Ищем свободное место для нового потока
				{
					if (masthr[i] == 0)
					{
						findplace = true;
						pos = i;
						masthr[i] = a; // Назначаем объект в массив
						break;
					}
				}

				if (!findplace)
				{
					cout << "not create new thread" << endl;
					break;
				}
				char* filename = new char[strlen(ibuf) - 5]; // Извлекаем имя файла
				strcpy(filename, ibuf + 5);

				a->FileName = filename;
				a->command = ibuf[0];
				a->Adr = client;
				a->UsingFile = false;
				a->WaitEvent = CreateEvent(NULL, false, true, NULL);
				a->Thread = CreateThread(NULL, NULL, &Dispath, (void*)pos, NULL, NULL);
				SetEvent(a->WaitEvent); // Установка события для синхронизации потока
			}
		}

		if (closesocket(sS) == SOCKET_ERROR)
			throw SetErrorMsgText("close socket: ", WSAGetLastError());

		if (WSACleanup() == SOCKET_ERROR)
			throw SetErrorMsgText("Cleanup: ", WSAGetLastError());
	}
	catch (string errorMsgText)
	{
		cout << endl << errorMsgText;
	}

	return 0;
}