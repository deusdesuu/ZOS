/*
	- текст из n символов
	- найти сумму кодов символов
	- k потоков, k < n, каждый k-й обрабатываей i + kxn i........i........i........i
*/

#include <Windows.h>
#include <iostream>
#include <tchar.h>


CRITICAL_SECTION cs;
int SUM = 0;

const int n = 44;
const char text[] = "asdasdasdasdasdasdasdasdadadadadad!!!!!!!!!!";
const int THREAD_COUNT = n / 2; //k


DWORD WINAPI sum_text(LPVOID param) {
	int to_sum;
	for (int i = *(int*)param; i < n; i += THREAD_COUNT) {
		to_sum = text[i];
		EnterCriticalSection(&cs);
		SUM += to_sum;
		LeaveCriticalSection(&cs);
	}
	return 0;
}

int _tmain(int argc, _TCHAR* argv[]) {
	
	HANDLE hThreadSum[THREAD_COUNT];
	DWORD dwThreadSum[THREAD_COUNT];

	int thread_indexes[THREAD_COUNT];
	for (int i = 0; i < THREAD_COUNT; ++i) {
		thread_indexes[i] = i;
	}

	InitializeCriticalSection(&cs);
	SUM = 0;


	for (int i = 0; i < THREAD_COUNT; ++i) {
		hThreadSum[i] = CreateThread(NULL, 0, sum_text, &(thread_indexes[i]), 0, &(dwThreadSum[i]));
		if (hThreadSum[i] == NULL) {
			std::cout << "Thread № " << i << " was not created\n" << "Error: " << GetLastError();
			return 1;
		}
	}
	if (WaitForMultipleObjects(THREAD_COUNT, hThreadSum, true, INFINITE) == WAIT_FAILED) {
		std::cout << "Wait failed.\n" << "Error: " << GetLastError() << std::endl;
		return 1;
	}
	for (int i = 0; i < THREAD_COUNT; ++i) {
		CloseHandle(hThreadSum[i]);
	}

	std::cout << "Сумма символов по их кодам по модулю 256: " << SUM % 256 << std::endl;

	return 0;
}
