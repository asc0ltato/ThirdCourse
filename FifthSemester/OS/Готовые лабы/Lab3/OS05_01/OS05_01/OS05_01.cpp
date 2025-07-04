#include <iostream>
#include <Windows.h>
#include <bitset>
using namespace std;

int main() {
    setlocale(LC_ALL, "RU");
    DWORD processId = GetCurrentProcessId();
    DWORD threadId = GetCurrentThreadId();

    DWORD processPriorityClass = GetPriorityClass(GetCurrentProcess());
    string processPriorityClassName;
    switch (processPriorityClass) {
    case REALTIME_PRIORITY_CLASS:
        processPriorityClassName = "Realtime";
        break;
    case HIGH_PRIORITY_CLASS:
        processPriorityClassName = "High";
        break;
    case ABOVE_NORMAL_PRIORITY_CLASS:
        processPriorityClassName = "Above Normal";
        break;
    case NORMAL_PRIORITY_CLASS:
        processPriorityClassName = "Normal";
        break;
    case BELOW_NORMAL_PRIORITY_CLASS:
        processPriorityClassName = "Below Normal";
        break;
    case IDLE_PRIORITY_CLASS:
        processPriorityClassName = "Idle";
        break;
    default:
        processPriorityClassName = "Unknown";
    }

    int threadPriority = GetThreadPriority(GetCurrentThread());

    DWORD_PTR processAffinityMask, systemAffinityMask;
    GetProcessAffinityMask(GetCurrentProcess(), &processAffinityMask, &systemAffinityMask);

    int numProcessors = 0;
    for (int i = 0; i < sizeof(DWORD_PTR) * 8; i++) {
        if ((processAffinityMask >> i) & 1) {
            numProcessors++;
        }
    }

    DWORD currentProcessor = GetCurrentProcessorNumber();

    cout << "Идентификатор текущего процесса: " << processId << endl;
    cout << "Идентификатор текущего (main) потока: " << threadId << endl;
    cout << "Приоритет (приоритетный класс) текущего процесса: " << processPriorityClassName << endl;
    cout << "Приоритет текущего потока: " << threadPriority << endl;
    cout << "Маска (affinity mask) доступных процессу процессоров в двоичном виде: " << bitset<sizeof(DWORD_PTR) * 8>(processAffinityMask) << endl;
    cout << "Количество процессоров, доступных процессу: " << numProcessors << endl;
    cout << "Процессор, назначенный текущему потоку: " << currentProcessor << endl;

    return 0;
}