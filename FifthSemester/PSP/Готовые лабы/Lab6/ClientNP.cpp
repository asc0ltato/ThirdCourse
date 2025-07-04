#include <iostream>
#include <Windows.h>
using namespace std;
#pragma warning(disable:4996)

string GetPipeErrorText(int code) {
    string msgText;
    switch (code) {
    case WSAEINTR: msgText = "WSAEINTR: Работа функции прервана "; break;
    case WSAEACCES: msgText = "WSAEACCES: Разрешение отвергнуто"; break;
    case WSAEFAULT: msgText = "WSAEFAULT: Ошибочный адрес"; break;
    case WSAEINVAL: msgText = "WSAEINVAL: Ошибка в аргументе"; break;
    case WSAEMFILE: msgText = "WSAEMFILE: Слишком много файлов открыто"; break;
    case WSAEWOULDBLOCK:msgText = "WSAEWOULDBLOCK: Ресурс временно недоступен"; break;
    case WSAEINPROGRESS:msgText = "WSAEINPROGRESS: Операция в процессе развития"; break;
    case WSAEALREADY:msgText = "WSAEALREADY: Операция уже выполняется "; break;
    case WSAENOTSOCK:msgText = "WSAENOCTSOCK: Сокет задан неправильно   "; break;
    case WSAEDESTADDRREQ:msgText = "WSAEDESTADDRREQ: Требуется адрес расположения "; break;
    case WSAEMSGSIZE:msgText = "WSAEMSGSIZE: Сообщение слишком длинное "; break;
    case WSAEPROTOTYPE:msgText = "WSAEPROTOTYPE: Неправильный тип протокола для сокета "; break;
    case WSAENOPROTOOPT:msgText = "WSAENOPROTOOPT: Ошибка в опции протокола"; break;
    case WSAEPROTONOSUPPORT:msgText = "WSAEPROTONOSUPPORT: Протокол не поддерживается "; break;
    case WSAESOCKTNOSUPPORT:msgText = "WSAESOCKTNOSUPPORT: Тип сокета не поддерживается "; break;
    case WSAEOPNOTSUPP:msgText = "WSAEOPNOTSUPP: Операция не поддерживается "; break;
    case WSAEPFNOSUPPORT:msgText = "WSAEPFNOSUPPORT: Тип протоколов не поддерживается "; break;
    case WSAEAFNOSUPPORT:msgText = "WSAEAFNOSUPPORT: Тип адресов не поддерживается протоколом"; break;
    case WSAEADDRINUSE:msgText = "WSAEADDRINUSE: Адрес уже используется "; break;
    case WSAEADDRNOTAVAIL:msgText = "WSAEADDRNOTAVAIL: Запрошенный адрес не может быть использован"; break;
    case WSAENETDOWN:msgText = "WSAENETDOWN: Сеть отключена "; break;
    case WSAENETUNREACH:msgText = "WSAENETUNREACH: Сеть не достижима"; break;
    case WSAENETRESET:msgText = "WSAENETRESET: Сеть разорвала соединение"; break;
    case WSAECONNABORTED:msgText = "WSAECONNABORTED: Программный отказ связи "; break;
    case WSAECONNRESET:msgText = "WSAECONNRESET: Связь восстановлена "; break;
    case WSAENOBUFS:msgText = "WSAENOBUFS: Не хватает памяти для буферов"; break;
    case WSAEISCONN:msgText = "WSAEISCONN: Сокет уже подключен"; break;
    case WSAENOTCONN:msgText = "WSAENOTCONN: Сокет не подключен"; break;
    case WSAESHUTDOWN:msgText = "WSAESHUTDOWN: Нельзя выполнить send: сокет завершил работу"; break;
    case WSAETIMEDOUT:msgText = "WSAETIMEDOUT: Закончился отведенный интервал  времени"; break;
    case WSAECONNREFUSED:msgText = "WSAECONNREFUSED: Соединение отклонено"; break;
    case WSAEHOSTDOWN:msgText = "WSAEHOSTDOWN: Хост в неработоспособном состоянии"; break;
    case WSAEHOSTUNREACH:msgText = "WSAEHOSTUNREACH: Нет маршрута для хоста"; break;
    case WSAEPROCLIM:msgText = "WSAEPROCLIM: Слишком много процессов "; break;
    case WSASYSNOTREADY:msgText = "WSASYSNOTREADY: Сеть не доступна "; break;
    case WSAVERNOTSUPPORTED:msgText = "WSAVERNOTSUPPORTED: Данная версия недоступна"; break;
    case WSANOTINITIALISED:msgText = "WSANOTINITIALISED: Не выполнена инициализация WS2_32.DLL"; break;
    case WSAEDISCON:msgText = "WSAEDISCON: Выполняется отключение"; break;
    case WSATYPE_NOT_FOUND:msgText = "WSATYPE_NOT_FOUND: Класс не найден"; break;
    case WSAHOST_NOT_FOUND:msgText = "WSAHOST_NOT_FOUND: Хост не найден"; break;
    case WSATRY_AGAIN:msgText = "WSATRY_AGAIN: Неавторизированный хост не найден"; break;
    case WSANO_RECOVERY:msgText = "WSANO_RECOVERY: Неопределенная  ошибка"; break;
    case WSANO_DATA:msgText = "WSANO_DATA: Нет записи запрошенного типа "; break;
    case WSAEINVALIDPROCTABLE:msgText = "WSAEINVALIDPROCTABLE: Ошибочный сервис"; break;
    case WSAEINVALIDPROVIDER:msgText = "WSAEINVALIDPROVIDER: Ошибка в версии сервиса"; break;
    case WSAEPROVIDERFAILEDINIT:msgText = "WSAEPROVIDERFAILEDINIT: Невозможно инициализировать сервис"; break;
    case WSASYSCALLFAILURE: msgText = "WSASYSCALLFAILURE: Аварийное завершение системного вызова"; break;
    case 2: msgText = "Ошибка"; break;
    case ERROR_INVALID_PARAMETER: msgText = "ERROR_INVALID_PARAMETER: Недопустимый параметр"; break;
    case ERROR_NO_DATA: msgText = "ERROR_NO_DATA: Канал закрывается"; break;
    case ERROR_PIPE_CONNECTED: msgText = "ERROR_PIPE_CONNECTED: Есть процесс на другом конце канала"; break;
    case ERROR_PIPE_LISTENING: msgText = "ERROR_PIPE_LISTENING: Ожидание открытия канала другим процессом"; break;
    case ERROR_CALL_NOT_IMPLEMENTED: msgText = "ERROR_CALL_NOT_IMPLEMENTED: Функция не поддерживается на данной системе"; break;
    case ERROR_BROKEN_PIPE: msgText = "ERROR_BROKEN_PIPE: Другой конец канала закрыт"; break;
    case ERROR_PIPE_BUSY: msgText = "ERROR_PIPE_BUSY: Канал занят"; break;
    case ERROR_PIPE_LOCAL: msgText = "ERROR_PIPE_LOCAL: Операция для локального канала"; break;
    case ERROR_PIPE_NOT_CONNECTED: msgText = "ERROR_PIPE_NOT_CONNECTED: Канал не подключен"; break;
    case ERROR_BAD_PIPE: msgText = "ERROR_BAD_PIPE: Неправильное состояние канала"; break;
    case ERROR_SEM_TIMEOUT: msgText = "ERROR_SEM_TIMEOUT: Время ожидания операции истекло"; break;
    case ERROR_IO_PENDING: msgText = "ERROR_IO_PENDING: Операция ввода/вывода в процессе выполнения"; break;
    default: msgText = "**ERROR**: Неизвестная ошибка"; break;
    };
    return msgText;
}

string SetPipeError(string msgText, int code) {
    return msgText + GetPipeErrorText(code);
}

int main()
{
    setlocale(LC_ALL, "RU");
    HANDLE cH;
    DWORD dwWrite; 
    char wbuf[50], rbuf[50];

    try {
        // \\\\ASCOLTAT0\\pipe\\Tube
        if (!WaitNamedPipe(TEXT("\\\\ASCOLTAT0\\pipe\\Tube"), 60000)) {
            throw SetPipeError("waitNamedPipe: ", GetLastError());
        }

        if ((cH = CreateFile(TEXT("\\\\ASCOLTAT0\\pipe\\Tube"), GENERIC_READ | GENERIC_WRITE,
            FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, NULL, NULL)) == INVALID_HANDLE_VALUE) 
        {
            throw  SetPipeError("CreateFile: ", GetLastError());
        }

        int messageCount;
        cout << "Введите количество передаваемых сообщений: ";
        cin >> messageCount;

        clock_t start = clock();

        for (int i = 1; i <= messageCount; ++i) {
            sprintf(wbuf, "Hello from Client %d", i);

            if (!WriteFile(cH, wbuf, sizeof(wbuf), &dwWrite, NULL)) {
                throw SetPipeError("WriteFile: ", GetLastError());
            }
            if (!ReadFile(cH, rbuf, sizeof(rbuf), &dwWrite, NULL)) {
                throw SetPipeError("ReadFile: ", GetLastError());
            }
            else {
                cout << "Ответ от сервера: \0" << rbuf << endl;
            }
        }

        strcpy(wbuf, "");

        if (!WriteFile(cH, wbuf, sizeof(wbuf), &dwWrite, NULL)) {
            throw SetPipeError("WriteFile: ", GetLastError());
        }

        clock_t end = clock();
        double elapsed_time = double(end - start) / CLOCKS_PER_SEC;
        cout << "Время обмена: " << elapsed_time << " секунд" << endl;

        if (!CloseHandle(cH)) {
            throw SetPipeError("CloseHandle: ", GetLastError());
        }

    }
    catch (string ErrorPipeText) {
        cout << endl << "SetPipeError: " << ErrorPipeText;
    }

    return 0;
}