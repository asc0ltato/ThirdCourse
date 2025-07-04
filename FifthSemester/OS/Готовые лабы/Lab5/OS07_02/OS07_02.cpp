#include <iostream>
#include <ctime>
#include <windows.h>
using namespace std;

int main() {
	clock_t start = clock();
	int counter = 0;
	setlocale(LC_ALL, "Russian");

	while (true) {
		counter++;
		double second = double(clock() - start) / CLOCKS_PER_SEC;

		if (second == 5) {
			cout << "Значение счетчика через 5 секунд: " << counter << endl;
		}
		else if (second == 10) {
			cout << "Значение счетчика через 10 секунд: " << counter << endl;
		}
		else if (second == 15) {
			cout << "Итоговое значение счётчика после 15 секунд: " << counter << endl;
			break;  
		}
	}

	return 0;
}