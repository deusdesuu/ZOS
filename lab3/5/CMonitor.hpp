#pragma once
#include <Windows.h>


class CMonitor {
private:
	HANDLE* row_mutex;
	/*
		пригодится наверное для вывода состояния мьютексов
		на данный момент никак не используется
		но в задании было
	*/
	int* occupated;
	int rows;

	CMonitor(const CMonitor&);
	CMonitor& operator=(const CMonitor&);

public:
	CMonitor(int row_count);
	~CMonitor();

	void OcupateRow(int row);
	void FreeRow(int row);
};

CMonitor::CMonitor(int row_count) {
	rows = row_count;
	row_mutex = new HANDLE[rows];
	occupated = new int[rows];
	for (int i = 0; i < rows; ++i) {
		row_mutex[i] = CreateMutex(NULL, false, NULL);
		occupated[i] = 0;
	}
}

CMonitor::~CMonitor() {
	for (int i = 0; i < rows; ++i)
		CloseHandle(row_mutex[i]);
	delete[] row_mutex;
	delete[] occupated;
}

void CMonitor::OcupateRow(int row) {
	WaitForSingleObject(row_mutex[row], INFINITE);
	occupated[row] = 1;
}

void CMonitor::FreeRow(int row) {
	occupated[row] = 0;
	ReleaseMutex(row_mutex[row]);
}