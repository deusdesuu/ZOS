#include <Windows.h>
#include <iostream>
#include <tchar.h>
#include <stdlib.h>


const int m = 10, n = 20;
float mtx[m][n]; // matrix
float col_sums[n]; // sums array
int col_numbers[n]; // column numbers
int row_numbers[m]; // row numbers
int seeds[m]; // seeds for mtx fill


void printmtx() {
	for (int i = 0; i < m; ++i) {
		for (int j = 0; j < n; ++j) {
			std::cout << mtx[i][j] << '\t';
		}
		std::cout << '\n';
	}
	std::cout << std::endl;
}

// thread function
DWORD WINAPI col_sum(LPVOID param) {
	// getting param value
	// column number
	int* pcol_num = (int*)param;
	int col_num = *pcol_num;

	// finding the sum
	col_sums[col_num] = 0;
	for (int i = 0; i < m; ++i) {
		col_sums[col_num] += (mtx[i][col_num]);
	}

	return 0;
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

int _tmain(int argc, _TCHAR* argv[]) {
	// defining variable to work with threads
	// array of n thread pointers
	HANDLE hThreadFill[m], hThreadSum[n];
	// array of n thread identifiers
	DWORD dwThreadID[n], dwThreadFill[m];

	/*
		- initializing row_numbers and seeds
		- filling the matrix similalry to like we read it
		- opening threads
		- filling the mtx
		- waiting for them to sync
		- closing threads
	*/

	// --------------------------------------- New Code -------------------------------------------
	for (int i = 0; i < m; ++i) {
		row_numbers[i] = i;
		seeds[i] = rand();
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

	// ---------------------------------------------------------------------------------------------
	// filling arrays with starting values
	for (int i = 0; i < n; ++i) {
		col_sums[i] = 0;
		col_numbers[i] = i;
	}
	// starting threads
	for (int i = 0; i < n; ++i) {
		hThreadSum[i] = CreateThread(
			NULL,				// safety attributes by default
			0,					// stack size by default
			col_sum,			// function name
			&(col_numbers[i]),	// parameteres pointer
			0,					// creation flag
			&(dwThreadID[i]));	// variable address for identificator
		// check if the thread was created
		if (hThreadSum[i] == NULL) {
			std::cout << "Thread № " << i << " was not created\n" << "Error: " << GetLastError();
		}
	}

	// waiting for threads to complete
	WaitForMultipleObjects(n, hThreadSum, true, INFINITE);

	// searching for the column with the minimal sum
	int num_min = 0;
	float min = col_sums[0];

	for (int i = 1; i < n; ++i) {
		if (min > col_sums[i]) {
			min = col_sums[i];
			num_min = i;
		}
	}

	// result output
	std::cout << "The target column № " << num_min << std::endl;

	// closing threads
	for (int i = 0; i < n; ++i) {
		CloseHandle(hThreadSum[i]);
	}

	return 0;
}