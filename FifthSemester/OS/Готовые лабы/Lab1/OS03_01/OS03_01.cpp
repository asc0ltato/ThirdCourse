#include <iostream>
#include <Windows.h>
using namespace std;

int main() {
	for (int i = 1; i <= 5000; ++i) {
		cout << i << ". PID = " << GetCurrentProcessId() << "\n";
		Sleep(1000);
	}
	return 0;
}