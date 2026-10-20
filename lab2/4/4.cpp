/*
	заполнить дек из n элементов
	файл из 10000 целочисленных элементов
*/

#include <Windows.h>
#include <iostream>
#include <tchar.h>
#include <locale>
#include <deque>
#include <fstream>


const int ELEM_COUNT = 10000;
const int THREAD_COUNT = 10; //n
std::deque<int> dq_front;
std::deque<int> dq_back;
int buf[ELEM_COUNT];

CRITICAL_SECTION front;
CRITICAL_SECTION back;

DWORD WINAPI deque_func(LPVOID param) {
	int index = *(int*)param;
	std::srand((unsigned)(std::time(NULL) + index * 1000));
	int start = index * (ELEM_COUNT / THREAD_COUNT);
	int end = (index + 1) * (ELEM_COUNT / THREAD_COUNT);
	bool f;
	if (index == THREAD_COUNT - 1) {
		end = ELEM_COUNT;
	}
	for (int i = start; i < end; ++i) {
		f = rand() % 2;
		if (f) {
			EnterCriticalSection(&front);
			dq_front.push_front(buf[i]);
			LeaveCriticalSection(&front);
		}
		else {
			EnterCriticalSection(&back);
			dq_back.push_back(buf[i]);
			LeaveCriticalSection(&back);
		}
	}
	
	return 0;
}

void print_dq() {
	while(!dq_front.empty()) {
		std::cout << dq_front.front() << '\t';
		dq_front.pop_front();
	}
}

int _tmain(int argc, _TCHAR* argv[]) {
	//std::setlocale(LC_ALL, "rus");
	
	HANDLE hThreadDeque[THREAD_COUNT];
	DWORD dwThreadDeque[THREAD_COUNT];

	InitializeCriticalSection(&front);
	InitializeCriticalSection(&back);

	std::ifstream stream("data.txt");
	for (int i = 0; i < ELEM_COUNT; ++i) {
		stream >> buf[i];
	}
	stream.close();

	int* thread_indexes = new int[THREAD_COUNT];
	for (int i = 0; i < THREAD_COUNT; ++i) {
		thread_indexes[i] = i;
	}

	for (int i = 0; i < THREAD_COUNT; ++i) {
		hThreadDeque[i] = CreateThread(NULL, 0, deque_func, &(thread_indexes[i]), 0, &(dwThreadDeque[i]));
		if (hThreadDeque[i] == NULL) {
			std::cout << "Thread № " << i << " was not created\n" << "Error: " << GetLastError();
			return 1;
		}
	}
	if (WaitForMultipleObjects(THREAD_COUNT, hThreadDeque, true, INFINITE) == WAIT_FAILED) {
		std::cout << "Wait failed.\n" << "Error: " << GetLastError() << std::endl;
		return 1;
	}
	for (int i = 0; i < THREAD_COUNT; ++i) {
		CloseHandle(hThreadDeque[i]);
	}
	dq_front.insert(dq_front.end(), dq_back.begin(), dq_back.end());
	print_dq();
	dq_front.clear();
	dq_back.clear();
	delete[] thread_indexes;
	DeleteCriticalSection(&front);
	DeleteCriticalSection(&back);
	return 0;
}
