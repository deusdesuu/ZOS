/*
	хеш таблица из х указателей на списки целых чисел
	n потоков генерируют случайные числа
*/
#include <Windows.h>
#include <iostream>
#include <tchar.h>
#include <locale>
#include <fstream>
#include "CMonitor.hpp"

struct Node {
	int value;
	Node* next = nullptr;
	Node(int v) { value = v; }
	~Node() { next = nullptr; }
};
void Add(Node*& root, int value) {
	if (root == nullptr) {
		root = new Node(value);
		return;
	}
	Node* new_node = new Node(value);
	(*new_node).next = root;
	root = new_node;
	return;
}
void Clear(Node*& node) {
	if (node == nullptr) {
		return;
	}
	Node* next = (*node).next;
	while (next != nullptr) {
		delete node;
		node = next;
		next = (*node).next;
	}
	node = nullptr;
	delete node;
}
void Print(Node* node) {
	while (node != nullptr) {
		std::cout << (*node).value << ' ';
		node = (*node).next;
	}
	std::cout << std::endl;
	return;
}

const int THREAD_COUNT = 10;
int TABLE_SIZE = 10;
int ELEM_COUNT = 10;

HANDLE* MUTEX_TABLE;
int* THREAD_ARR;

int hash(int value) { 
	int res = value % TABLE_SIZE;
	if (res >= 0) { return res; }
	return res * (-1);
}

Node** TABLE;
void print_table() {
	for (int i = 0; i < TABLE_SIZE; ++i) {
		Print(TABLE[i]);
	}
}

DWORD WINAPI table_add(LPVOID param) {
	std::srand((unsigned int)(std::time(NULL) + *(int*)param));
	int value, index;
	for (int i = 0; i < ELEM_COUNT; ++i) {
		value = (rand() % 1000) - 500;
		index = hash(value);

		WaitForSingleObject(MUTEX_TABLE[index], INFINITE);
		Add(TABLE[index], value);
		ReleaseMutex(MUTEX_TABLE[index]);
	}
	return 0;
}

int _tmain(int argc, _TCHAR* argv[]) {
	//std::setlocale(LC_ALL, "rus");
	// hash table init
	TABLE = new Node*[TABLE_SIZE];
	// mutex array init
	MUTEX_TABLE = new HANDLE[TABLE_SIZE];
	// init
	for (int i = 0; i < TABLE_SIZE; ++i) {
		TABLE[i] = nullptr;
		MUTEX_TABLE[i] = CreateMutex(NULL, false, NULL);
	}
	// thread array init (only for srand)
	THREAD_ARR = new int[THREAD_COUNT];
	for (int i = 0; i < THREAD_COUNT; ++i) {
		THREAD_ARR[i] = i;
	}
	HANDLE hThread[THREAD_COUNT];
	DWORD dwThread[THREAD_COUNT];
	
	for (int i = 0; i < THREAD_COUNT; ++i) {
		hThread[i] = CreateThread(NULL, 0, table_add, &(THREAD_ARR[i]), 0, &(dwThread[i]));
		if (hThread[i] == NULL) {
			std::cout << "Thread № " << i << " was not created\n" << "Error: " << GetLastError();
			return 1; // cant work without mtx so return 1 if error occured
		}
	}
	if (WaitForMultipleObjects(THREAD_COUNT, hThread, true, INFINITE) == WAIT_FAILED) {
		std::cout << "Wait failed.\n" << "Error: " << GetLastError() << std::endl;
		return 1; // same
	}
	for (int i = 0; i < THREAD_COUNT; ++i) {
		CloseHandle(hThread[i]);
	}
	print_table();


	// clearing memory
	for (int i = 0; i < TABLE_SIZE; ++i) {
		CloseHandle(MUTEX_TABLE[i]);
		Clear(TABLE[i]);
	}
	delete[] TABLE;
	delete[] MUTEX_TABLE;
	delete[] THREAD_ARR;
	return 0;
}
