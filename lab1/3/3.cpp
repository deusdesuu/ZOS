#include <Windows.h>
#include <iostream>
#include <tchar.h>
#include <stdlib.h>


const int m = 10, n = 20;
float mtx[m][n]; // matrix

int row_numbers[m]; // row numbers
int seeds[m]; // seeds for mtx fill

int column_numbers[n];


void printmtx() {
	for (int i = 0; i < m; ++i) {
		for (int j = 0; j < n; ++j) {
			std::cout << mtx[i][j] << '\t';
		}
		std::cout << '\n';
	}
	std::cout << std::endl;
}

// mtx fill function 
DWORD WINAPI fill_row(LPVOID param) {
	int row = *(int*)param;
	std::srand(seeds[row]);
	for (int j = 0; j < n; ++j) {
		mtx[row][j] = (float)(rand());
	}

	return 0;
}

// column sort function
DWORD WINAPI col_sort(LPVOID param) {
	int column = *(int*)param;
	for (int i = 0; i < m - 1; ++i) {
		for (int j = 1; j < m - i; ++j) {
			if (mtx[j][column] < mtx[j - 1][column]) {
				std::swap(mtx[j][column], mtx[j - 1][column]);
			}
		}
	}

	return 0;
}

int _tmain(int argc, _TCHAR* argv[]) {
	// defining variable to work with threads
	// array of n thread pointers
	HANDLE hThreadFill[m], hThreadSort[n];
	// array of n thread identifiers
	DWORD dwThreadFill[m], dwThreadSort[n];

	std::srand((unsigned)time(NULL));

	for (int i = 0; i < m; ++i) {
		row_numbers[i] = i;
		seeds[i] = rand();
	}
	for (int i = 0; i < n; ++i) {
		column_numbers[i] = i;
	}

	for (int i = 0; i < m; ++i) {
		hThreadFill[i] = CreateThread(NULL, 0, fill_row, &(row_numbers[i]), 0, &(dwThreadFill[i]));
		if (hThreadFill[i] == NULL) {
			std::cout << "Thread № " << i << " was not created\n" << "Error: " << GetLastError();
			return 1; // cant work without mtx so return 1 if error occured
		}
	}
	if (WaitForMultipleObjects(m, hThreadFill, true, INFINITE) == WAIT_FAILED) {
		std::cout << "Wait failed.\n" << "Error: " << GetLastError() << std::endl;
		return 1; // same
	}
	for (int i = 0; i < m; ++i) {
		CloseHandle(hThreadFill[i]);
	}

	printmtx();

	for (int i = 0; i < n; ++i) {
		hThreadSort[i] = CreateThread(NULL, 0, col_sort, &(column_numbers[i]), 0, &(dwThreadSort[i]));
		if (hThreadSort[i] == NULL) {
			std::cout << "Thread № " << i << " was not created\n" << "Error: " << GetLastError();
			return 1; // cant work without mtx so return 1 if error occured
		}
	}
	if (WaitForMultipleObjects(n, hThreadSort, true, INFINITE) == WAIT_FAILED) {
		std::cout << "Wait failed.\n" << "Error: " << GetLastError() << std::endl;
		return 1; // same
	}
	for (int i = 0; i < n; ++i) {
		CloseHandle(hThreadSort[i]);
	}

	std::cout << '\n';
	printmtx();

	return 0;
}