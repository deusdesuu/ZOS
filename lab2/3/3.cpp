/*
	- посчитать площадь криволинейной трапеции через n интегральных сумм
	- делим криволинейную трапецию на n частей (n потоков)
	- каждую часть делим на m частей
	- каждую миничасть считаем через сумму прямоугольников
*/

#include <Windows.h>
#include <iostream>
#include <tchar.h>
#include <locale>
#include <math.h>


const int n = 50;
const int m = 1000;
const int THREAD_COUNT = n;

CRITICAL_SECTION cs;
double SUM = 0;

double y(double x) { return sqrt(x); }
const double a = 5;
const double b = 10;

DWORD WINAPI part_int_sum(LPVOID param) {
	int i = *(int*)param;
	double delta = (b - a) / n;
	double x = a + delta * i;
	delta /= m;
	double local_sum = 0;
	// посчитаем по левому разбиению
	for (int i = 0; i < m; ++i, x += delta) {
		local_sum += y(x);
	}
	local_sum *= delta;
	EnterCriticalSection(&cs);
	SUM += local_sum;
	LeaveCriticalSection(&cs);
	return 0;
}

int _tmain(int argc, _TCHAR* argv[]) {
	//std::setlocale(LC_ALL, "rus");

	HANDLE hThreadInt[THREAD_COUNT];
	DWORD dwThreadInt[THREAD_COUNT];

	InitializeCriticalSection(&cs);
	SUM = 0;

	int* int_indexes = new int[n];
	for (int i = 0; i < n; ++i) {
		int_indexes[i] = i;
	}

	for (int i = 0; i < THREAD_COUNT; ++i) {
		hThreadInt[i] = CreateThread(NULL, 0, part_int_sum, &(int_indexes[i]), 0, &(dwThreadInt[i]));
		if (hThreadInt[i] == NULL) {
			std::cout << "Thread № " << i << " was not created\n" << "Error: " << GetLastError();
			return 1;
		}
	}
	if (WaitForMultipleObjects(THREAD_COUNT, hThreadInt, true, INFINITE) == WAIT_FAILED) {
		std::cout << "Wait failed.\n" << "Error: " << GetLastError() << std::endl;
		return 1;
	}
	for (int i = 0; i < THREAD_COUNT; ++i) {
		CloseHandle(hThreadInt[i]);
	}

	std::cout << "Площадь криволинейной трапеции: " << SUM << std::endl;

	delete int_indexes;
	return 0;
}
