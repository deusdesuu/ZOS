/*
	заполнить дек из n элементов
	файл из 10000 целочисленных элементов
*/

#include <Windows.h>
#include <iostream>
#include <tchar.h>
#include <locale>
#include <fstream>


const int m = 16; // buffer size

HANDLE hMutex;

char buf[m]; // buffer
int buf_len = 0; // buffer elem count
bool buffer_free = true;
bool data_ready = false;
bool finished = false;
int SUM = 0; // res


DWORD WINAPI read(LPVOID param) {
	std::ifstream file("data.txt", std::ios::binary);
	while (true) {
		// waiting until buffer released
		while (true) {
			WaitForSingleObject(hMutex, INFINITE);
			bool can_read = buffer_free;
			ReleaseMutex(hMutex);
			if (can_read) {
				break;
			}
		}
		// taking mutex
		WaitForSingleObject(hMutex, INFINITE);
		file.read(reinterpret_cast<char*>(buf), m);
		buf_len = (int)file.gcount();
		buffer_free = false;
		if (buf_len > 0) {
			data_ready = true;
		}
		if (buf_len == 0) {
			finished = true;
		}
		// releasing mutex
		ReleaseMutex(hMutex);
		if (buf_len == 0) {
			break;
		}
	}
	file.close();
	return 0;
}

DWORD WINAPI calc(LPVOID param) {
	bool has_data;
	while (true) {
		while (true) {
			WaitForSingleObject(hMutex, INFINITE);
			has_data = data_ready;
			bool done = finished;
			ReleaseMutex(hMutex);
			if (has_data || done) {
				break;
			}
		}
		if (!has_data) {
			break;
		}
		// taking mutex
		WaitForSingleObject(hMutex, INFINITE);
		// calculating data
		for (int i = 0; i < buf_len; ++i) {
			SUM = (SUM + buf[i]) % 256;
		}
		data_ready = false;
		buffer_free = true;
		ReleaseMutex(hMutex);
	}
	return 0;
}

int _tmain(int argc, _TCHAR* argv[]) {
	//std::setlocale(LC_ALL, "rus");

	hMutex = CreateMutex(NULL, false, NULL);

	HANDLE hReader = CreateThread(NULL, 0, read, NULL, 0, NULL);
	HANDLE hCalc = CreateThread(NULL, 0, calc, NULL, 0, NULL);
	
	HANDLE hThread[2];
	hThread[0] = hReader;
	hThread[1] = hCalc;

	WaitForMultipleObjects(2, hThread, true, INFINITE);

	CloseHandle(hReader);
	CloseHandle(hCalc);
	CloseHandle(hMutex);

	std::cout << SUM << std::endl;

	return 0;
}
