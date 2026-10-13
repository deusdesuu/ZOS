#include <Windows.h>
#include <iostream>
#include <tchar.h>


const int m = 10, n = 20;
float mtx[m][n]; // matrix
float col_sums[n]; // sums array
int col_numbers[n]; // column numbers


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


int _tmain(int argc, _TCHAR* argv[]) {
	// defining variable to work with threads
	// array of n thread pointers
	HANDLE hThread[n];
	// array of n thread identifiers
	DWORD dwThreadID[n];

	// creating matrix
	for (int i = 0; i < m; ++i) {
		for (int j = 0; j < n; ++j) {
			mtx[i][j] = (float)(rand());
		}
	}

	// filling the matrix with starting values
	for (int i = 0; i < n; ++i) {
		col_sums[i] = 0;
		col_numbers[i] = i;
	}

	// starting threads
	for (int i = 0; i < n; ++i) {
		hThread[i] = CreateThread(
			NULL,				// safety attributes by default
			0,					// stack size by default
			col_sum,			// function name
			&(col_numbers[i]),	// parameteres pointer
			0,					// creation flag
			&(dwThreadID[i]));	// variable address for identificator
		// check if the thread was created
		if (hThread[i] == NULL) {
			std::cout << "Thread № " << i << " was not created\n" << "Error: " << GetLastError();
		}
	}

	// waiting for threads to complete
	WaitForMultipleObjects(n, hThread, true, INFINITE);

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
		CloseHandle(hThread[i]);
	}

	return 0;
}