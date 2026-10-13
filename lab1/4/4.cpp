#include <Windows.h>
#include <iostream>
#include <tchar.h>
#include <stdlib.h>


const int m = 4, n = 4;
float mtx[m][n]; // matrix

int row_numbers[m]; // row numbers
int seeds[m]; // seeds for mtx fill
float det4_mas[4];
float det3_mas[3];

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
		mtx[row][j] = (float)(rand() % 10);
	}

	return 0;
}

DWORD WINAPI det4(LPVOID param) {
	int column = *(int*)param;
	int res = 0;
	int a, b;
	int det3_mas_i = 0;

	for (int i = 0; i < 4; ++i) {
		if (i != column) {
			a = -1; b = -1;
			for (int j = 0; j < 4; ++j) {
				if (j == i || j == column) { continue; }
				if (a < 0) {
					a = j;
				}
				else {
					b = j;
				}
			}
			det3_mas[det3_mas_i++] = mtx[1][i] * (mtx[2][a] * mtx[3][b] - mtx[2][b] * mtx[3][a]);
		}
	}
	det4_mas[column] = mtx[0][column] * (det3_mas[0] - det3_mas[1] + det3_mas[2]);
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
	}
	for (int i = 0; i < n; ++i) {
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

	for (int i = 0; i < 4; ++i) {
		hThreadFill[i] = CreateThread(NULL, 0, det4, &(row_numbers[i]), 0, &(dwThreadFill[i]));
		if (hThreadFill[i] == NULL) {
			std::cout << "Thread № " << i << " was not created\n" << "Error: " << GetLastError();
			return 1; // cant work without mtx so return 1 if error occured
		}
	}
	if (WaitForMultipleObjects(4, hThreadFill, true, INFINITE) == WAIT_FAILED) {
		std::cout << "Wait failed.\n" << "Error: " << GetLastError() << std::endl;
		return 1; // same
	}
	for (int i = 0; i < 4; ++i) {
		CloseHandle(hThreadFill[i]);
	}

	std::cout << "Matrix determinant = " << det4_mas[0] - det4_mas[1] + det4_mas[2] - det4_mas[3] << std::endl;

	return 0;
}