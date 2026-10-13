#include <Windows.h>
#include <iostream>
#include <tchar.h>
#include <stdlib.h>


const int m = 10, n = 20;
float mtx[m][n]; // matrix

int row_numbers[m]; // row numbers
int seeds[m]; // seeds for mtx fill
float row_sums[m]; // row sums for averages


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

// row sums function
DWORD WINAPI row_sum(LPVOID param) {
	int row = *(int*)param;
	float sum = 0;
	for (int i = 0; i < n; ++i) {
		sum += mtx[row][i];
	}
	row_sums[row] = sum;
	return 0;
}

int _tmain(int argc, _TCHAR* argv[]) {
	// defining variable to work with threads
	// array of n thread pointers
	HANDLE hThreadFill[m];
	// array of n thread identifiers
	DWORD dwThreadFill[m];

	std::srand((unsigned)time(NULL));

	for (int i = 0; i < m; ++i) {
		row_numbers[i] = i;
		seeds[i] = rand();
		row_sums[i] = 0;
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
	// ------------------------------------ New Code -------------------------------------------------
	for (int i = 0; i < m; ++i) {
		hThreadFill[i] = CreateThread(NULL, 0, row_sum, &(row_numbers[i]), 0, &(dwThreadFill[i]));
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

	float max = row_sums[0];
	int max_i = 0;
	for (int i = 1; i < m; ++i) {
		if (max < row_sums[i]) {
			max = row_sums[i];
			max_i = i;
		}
	}
	std::cout << "The target row № " << max_i + 1 << std::endl;
	// -----------------------------------------------------------------------------------------------

	return 0;
}