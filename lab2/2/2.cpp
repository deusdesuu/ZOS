/*
	- матрица nxm вещественных чисел
	- заполнение n параллельными потоками
	- сумма тоже
	- сумма по строке только после формирования
	- потоки подсчета сумм в приостановленном состоянии
*/

#include <Windows.h>
#include <iostream>
#include <tchar.h>
#include <locale>


const int n = 10;
const int m = 10;
float** mtx = new float* [n];

const int THREAD_COUNT = n;
CRITICAL_SECTION cs;
float SUM = 0;


void print_mtx() {
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < m; ++j) {
			std::cout << mtx[i][j] << '\t';
		}
		std::cout << '\n';
	}
	std::cout << std::endl;
}

DWORD WINAPI fill_mtx(LPVOID param) {
	int i = *(int*)param;
	std::srand((unsigned)(std::time(NULL) + 1000 * i));
	for (int j = 0; j < m; ++j) {
		mtx[i][j] = (rand() - 16384) * .1;
	}
	return 0;
}

DWORD WINAPI sum_mtx(LPVOID param) {
	int i = *(int*)param;
	float local_sum = 0;
	for (int j = 0; j < m; ++j) {
		local_sum += mtx[i][j];
	}
	EnterCriticalSection(&cs);
	SUM += local_sum;
	LeaveCriticalSection(&cs);

	return 0;
}

int _tmain(int argc, _TCHAR* argv[]) {
	//std::setlocale(LC_ALL, "rus");

	HANDLE hThreadMtx[THREAD_COUNT], hThreadSum[THREAD_COUNT];
	DWORD dwThreadMtx[THREAD_COUNT], dwThreadSum[THREAD_COUNT];

	int mtx_indexes[THREAD_COUNT];
	for (int i = 0; i < THREAD_COUNT; ++i) {
		mtx_indexes[i] = i;
		mtx[i] = new float[m];
	}

	InitializeCriticalSection(&cs);
	SUM = 0;


	for (int i = 0; i < THREAD_COUNT; ++i) {
		hThreadSum[i] = CreateThread(NULL, 0, sum_mtx, &(mtx_indexes[i]), CREATE_SUSPENDED, &(dwThreadSum[i]));
		if (hThreadSum[i] == NULL) {
			std::cout << "Thread № " << i << " was not created\n" << "Error: " << GetLastError();
			return 1;
		}
		hThreadMtx[i] = CreateThread(NULL, 0, fill_mtx, &(mtx_indexes[i]), 0, &(dwThreadMtx[i]));
		if (hThreadMtx[i] == NULL) {
			std::cout << "Thread № " << i << " was not created\n" << "Error: " << GetLastError();
			return 1;
		}
	}
	for (int i = 0; i < THREAD_COUNT; ++i) {
		ResumeThread(hThreadSum[i]);
	}
	if (WaitForMultipleObjects(THREAD_COUNT, hThreadMtx, true, INFINITE) == WAIT_FAILED) {
		std::cout << "Wait failed.\n" << "Error: " << GetLastError() << std::endl;
		return 1;
	}
	if (WaitForMultipleObjects(THREAD_COUNT, hThreadSum, true, INFINITE) == WAIT_FAILED) {
		std::cout << "Wait failed.\n" << "Error: " << GetLastError() << std::endl;
		return 1;
	}
	for (int i = 0; i < THREAD_COUNT; ++i) {
		CloseHandle(hThreadMtx[i]);
		CloseHandle(hThreadSum[i]);
	}

	print_mtx();

	std::cout << "Сумма элементов матрицы: " << SUM << std::endl;

	for (int i = 0; i < THREAD_COUNT; ++i) {
		delete mtx[i];
	}
	delete mtx;

	return 0;
}
