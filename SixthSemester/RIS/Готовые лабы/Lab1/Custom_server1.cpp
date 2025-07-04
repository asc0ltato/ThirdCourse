#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "iostream"
#include "Winsock2.h"
#include "time.h"
#include <chrono>
#include <tchar.h>
#pragma comment(lib, "WS2_32.lib")
#define SIP "172.20.10."
//#define SIP "172.20.10.5"
#define PORT 5000
using namespace std;
using namespace chrono;

struct GETSINCHRO {
	char cmd[7];
	long long curvalue;
};

struct SETSINCRO		///ответ С на синхр счетчика времени
{
	char cmd[7];			///всегда значение SINCRO
	long long correction;		///знач, кот надо прибавить к знач счетчика t
	long long rtt;
};

string GetErrorMsgText(int code)
{
	string msgText;

	switch (code)
	{
	case WSAEINTR:				 msgText = "Работа функции прервана\n";						  break;
	case WSAEACCES:				 msgText = "Разрешение отвергнуто\n";						  break;
	case WSAEFAULT:				 msgText = "Ошибочный адрес\n";								  break;
	case WSAEINVAL:				 msgText = "Ошибка в аргументе\n";							  break;
	case WSAEMFILE:				 msgText = "Слишком много файлов открыто\n";				  break;
	case WSAEWOULDBLOCK:		 msgText = "Ресурс временно недоступен\n";					  break;
	case WSAEINPROGRESS:		 msgText = "Операция в процессе развития\n";				  break;
	case WSAEALREADY: 			 msgText = "Операция уже выполняется\n";					  break;
	case WSAENOTSOCK:   		 msgText = "Сокет задан неправильно\n";						  break;
	case WSAEDESTADDRREQ:		 msgText = "Требуется адрес расположения\n";				  break;
	case WSAEMSGSIZE:  			 msgText = "Сообщение слишком длинное\n";				      break;
	case WSAEPROTOTYPE:			 msgText = "Неправильный тип протокола для сокета\n";		  break;
	case WSAENOPROTOOPT:		 msgText = "Ошибка в опции протокола\n";					  break;
	case WSAEPROTONOSUPPORT:	 msgText = "Протокол не поддерживается\n";					  break;
	case WSAESOCKTNOSUPPORT:	 msgText = "Тип сокета не поддерживается\n";				  break;
	case WSAEOPNOTSUPP:			 msgText = "Операция не поддерживается\n";					  break;
	case WSAEPFNOSUPPORT:		 msgText = "Тип протоколов не поддерживается\n";			  break;
	case WSAEAFNOSUPPORT:		 msgText = "Тип адресов не поддерживается протоколом\n";	  break;
	case WSAEADDRINUSE:			 msgText = "Адрес уже используется\n";						  break;
	case WSAEADDRNOTAVAIL:		 msgText = "Запрошенный адрес не может быть использован\n";	  break;
	case WSAENETDOWN:			 msgText = "Сеть отключена\n";								  break;
	case WSAENETUNREACH:		 msgText = "Сеть не достижима\n";							  break;
	case WSAENETRESET:			 msgText = "Сеть разорвала соединение\n";					  break;
	case WSAECONNABORTED:		 msgText = "Программный отказ связи\n";						  break;
	case WSAECONNRESET:			 msgText = "Связь восстановлена\n";							  break;
	case WSAENOBUFS:			 msgText = "Не хватает памяти для буферов\n";				  break;
	case WSAEISCONN:			 msgText = "Сокет уже подключен\n";							  break;
	case WSAENOTCONN:			 msgText = "Сокет не подключен\n";							  break;
	case WSAESHUTDOWN:			 msgText = "Нельзя выполнить send: сокет завершил работу\n";  break;
	case WSAETIMEDOUT:			 msgText = "Закончился отведенный интервал  времени\n";		  break;
	case WSAECONNREFUSED:		 msgText = "Соединение отклонено\n";						  break;
	case WSAEHOSTDOWN:			 msgText = "Хост в неработоспособном состоянии\n";			  break;
	case WSAEHOSTUNREACH:		 msgText = "Нет маршрута для хоста\n";						  break;
	case WSAEPROCLIM:			 msgText = "Слишком много процессов\n";						  break;
	case WSASYSNOTREADY:		 msgText = "Сеть не доступна\n";							  break;
	case WSAVERNOTSUPPORTED:	 msgText = "Данная версия недоступна\n";					  break;
	case WSANOTINITIALISED:		 msgText = "Не выполнена инициализация WS2_32.DLL\n";		  break;
	case WSAEDISCON:			 msgText = "Выполняется отключение\n";						  break;
	case WSATYPE_NOT_FOUND:		 msgText = "Класс не найден\n";								  break;
	case WSAHOST_NOT_FOUND:		 msgText = "Хост не найден\n";								  break;
	case WSATRY_AGAIN:			 msgText = "Неавторизированный хост не найден\n";			  break;
	case WSANO_RECOVERY:		 msgText = "Неопределенная ошибка\n";						  break;
	case WSANO_DATA:			 msgText = "Нет записи запрошенного типа\n";				  break;
	case WSA_INVALID_HANDLE:	 msgText = "Указанный дескриптор события  с ошибкой\n";		  break;
	case WSA_INVALID_PARAMETER:	 msgText = "Один или более параметров с ошибкой\n";			  break;
	case WSA_IO_INCOMPLETE:		 msgText = "Объект ввода-вывода не в сигнальном состоянии\n"; break;
	case WSA_IO_PENDING:		 msgText = "Операция завершится позже\n";					  break;
	case WSA_NOT_ENOUGH_MEMORY:	 msgText = "Не достаточно памяти\n";						  break;
	case WSA_OPERATION_ABORTED:	 msgText = "Операция отвергнута\n";							  break;
	case WSAEINVALIDPROCTABLE:	 msgText = "Ошибочный сервис\n";							  break;
	case WSAEINVALIDPROVIDER:	 msgText = "Ошибка в версии сервиса\n";						  break;
	case WSAEPROVIDERFAILEDINIT: msgText = "Невозможно инициализировать сервис\n";			  break;
	case WSASYSCALLFAILURE:		 msgText = "Аварийное завершение системного вызова\n";		  break;
	default:					 msgText = "Error\n";										  break;
	};

	return msgText;
}

string SetErrorMsgText(string msgText, int code)
{
	return  msgText + GetErrorMsgText(code);
};

int setAverageCorrection(int averageCorrection[], int length)
{
	int sum = 0;
	for (int i = 0; i < length; i++)
		sum += averageCorrection[i];
	return sum / length;
}

int _tmain(int argc, _TCHAR* argv[])
{
	setlocale(LC_ALL, "Russian");
	SETSINCRO setsincro;
	GETSINCHRO getsincro;
	ZeroMemory(&setsincro, sizeof(setsincro));
	ZeroMemory(&getsincro, sizeof(getsincro));
	setsincro.correction = 0;
	SYSTEMTIME tm;
	clock_t cS;
	int averageCorrection[10];

	cout << "Server Run" << endl;

	try
	{
		SOCKET sS;
		WSADATA wsaData;

		if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0)
			throw SetErrorMsgText("Startup: ", WSAGetLastError());

		if ((sS = socket(AF_INET, SOCK_DGRAM, NULL)) == INVALID_SOCKET)
			throw SetErrorMsgText("Socket: ", WSAGetLastError());

		SOCKADDR_IN serv;
		serv.sin_family = AF_INET;
		serv.sin_port = htons(PORT);
		serv.sin_addr.s_addr = inet_addr(SIP);

		if (bind(sS, (LPSOCKADDR)&serv, sizeof(serv)) == SOCKET_ERROR)
			throw SetErrorMsgText("Bind_Server: ", WSAGetLastError());

		int count = 2;

		while (count != 22)
		{
			SOCKADDR_IN client;
			int lc = sizeof(client);
			int average = 0;

			// Время получения запроса
			auto recvTime = chrono::high_resolution_clock::now();
			recvfrom(sS, (char*)&getsincro, sizeof(getsincro), NULL, (sockaddr*)&client, &lc);

			// Текущее время на сервере
			auto currentTime = chrono::high_resolution_clock::now();
			long long currentMs = chrono::duration_cast<chrono::milliseconds>(currentTime.time_since_epoch()).count();

			// Коррекция между клиентским и серверным временем
			setsincro.correction = (int)(currentMs - getsincro.curvalue);

			// Время отправки ответа
			auto sendTime = chrono::high_resolution_clock::now();
			setsincro.rtt = (int)duration_cast<milliseconds>(sendTime - recvTime).count();

			// Отправка коррекции клиенту
			strcpy_s(setsincro.cmd, sizeof(setsincro.cmd), "SINC");
			sendto(sS, (char*)&setsincro, sizeof(setsincro), 0, (sockaddr*)&client, sizeof(client));

			averageCorrection[(count - 2) % 10] = setsincro.correction;
			average = setAverageCorrection(averageCorrection, min(count - 1, 10));

			GetSystemTime(&tm);
			cout << tm.wMonth << "." << tm.wDay << "." << tm.wYear << " - " << tm.wHour + 3 << ":" << tm.wMinute << ":" << tm.wSecond << "." << tm.wMilliseconds << "\n";
			cout << "Request #" << count - 1 << " from " << inet_ntoa(client.sin_addr)
				<< " Correction = " << setsincro.correction
				<< ", RTT = " << setsincro.rtt
				<< ", Average Correction = " << average << "\n\n\n";

			count++;
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

	system("pause");
	return 0;
}