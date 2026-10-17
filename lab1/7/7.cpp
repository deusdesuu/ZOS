/*
	- разрезать по глубине
		- все, что до определенной глубины посчитать как обычно
		- все, что после - раздать потокам
	- раздать части по потокам
	- свернуть результаты в один
*/

#include <Windows.h>
#include <iostream>
#include <tchar.h>
#include <stdlib.h>
#include <queue>
#include <random>
#define ll long long


// Bin tree structure (with repeats)
struct Node {
	int value;
	Node* left;
	Node* right;

	Node(int v) {
		value = v;
		left = nullptr;
		right = nullptr;
	}
	~Node() {
		delete left;
		delete right;
	}
};

void Add(Node*& node, int value) {
	if (node == nullptr) {
		node = new Node(value);
	}
	else if (value < (*node).value) {
		Add((*node).left, value);
	}
	else {
		Add((*node).right, value);
	}
}


const int n = 10000; // elem_count

const int THREAD_COUNT = 10;
const int MAX_TASKS = THREAD_COUNT * 4;
int TASK_COUNT = 0;
ll SUM = 0;
ll SUM_TO_GET = 0;

Node** partition = new Node*[MAX_TASKS];
ll* part_sums = new ll[MAX_TASKS];
int* indexes = new int[MAX_TASKS];


// заполняем дерево и сравнительный массив значениями
void fill_tree(Node*& root) {
	int value;
	for (int i = 0; i < n; ++i) {
		value = rand();
		Add(root, value);
		SUM_TO_GET += value;
	}
}
/*
	- разбиваем дерево на поддеревья (нарезка в глубину)
	- проходим дерево через BST, сразу считаем текущую вершину в сумму
		заполняем очередь до тех пор, пока q.size() < MAX_TASKS
	- в конце переносим указатели из очереди в массив, с которым будем работать через потоки
	- если дерево вырожденное (1 ветвь), то все расчеты пройдут тут, (вроде) ничего с этим не сделать
*/
void break_tree(Node* root) {
	if (root == nullptr) { return; }
	std::queue<Node*> q;
	q.push(root);

	Node* buf = nullptr;
	int counter = 0;

	while (!q.empty() && q.size() < MAX_TASKS) {
		buf = q.front();
		q.pop();

		SUM += (*buf).value;
		++counter;
		if ((*buf).left != nullptr) {
			q.push((*buf).left);
		}
		if ((*buf).right != nullptr) {
			q.push((*buf).right);
		}
	}

	for (; !q.empty(); ++TASK_COUNT) {
		partition[TASK_COUNT] = q.front();
		q.pop();
	}
	std::cout << "Во время нарезки дерева было посчитано " << counter << " / " << n << " вершин.\n";
}

/*
	- получаем индекс поддерева из нарезки, которое будем считать
	- проходимся через BST по поддереву, считаем сумму
	- записываем результат в свою ячейку
	- каждый поток просчитывает по 4 поддерева, во избежание простоев
*/
DWORD WINAPI sum_tree(LPVOID param) {
	int index = *(int*)param;
	ll local_sum;
	std::queue<Node*> q;

	for (int i = index; i < TASK_COUNT; i += THREAD_COUNT) {
		q.push(partition[i]);
		local_sum = 0;
		Node* buf;

		while (!q.empty()) {
			buf = q.front();
			local_sum += (*buf).value;
			if ((*buf).left != nullptr) {
				q.push((*buf).left);
			}
			if ((*buf).right != nullptr) {
				q.push((*buf).right);
			}
			q.pop();
		}
		part_sums[index] += local_sum;
	}

	return 0;
}

int _tmain(int argc, _TCHAR* argv[]) {
	std::srand((unsigned int)std::time(NULL));
	HANDLE hThreadSum[THREAD_COUNT];
	DWORD dwThreadSum[THREAD_COUNT];
	
	Node* root = nullptr;
	
	fill_tree(root);
	break_tree(root);

	if (TASK_COUNT == 0) {
		std::cout << "Сумма, которую должны были получить: " << SUM_TO_GET << '\n';
		std::cout << "Сумма, которую получили: " << SUM << '\n';
		std::cout << "Все расчеты были проведены в момент нарезки, потоки не потребовались." << std::endl;
		delete root;
		delete[] partition;
		delete[] part_sums;
		delete[] indexes;
		return 0;
	}

	for (int i = 0; i < THREAD_COUNT; ++i) {
		indexes[i] = i;
		part_sums[i] = 0;
	}

	for (int i = 0; i < THREAD_COUNT; ++i) {
		hThreadSum[i] = CreateThread(NULL, 0, sum_tree, &(indexes[i]), 0, &(dwThreadSum[i]));
		if (hThreadSum[i] == NULL) {
			std::cout << "Thread № " << i << " was not created\n" << "Error: " << GetLastError();
			return 1; // cant work without mtx so return 1 if error occured
		}
	}
	if (WaitForMultipleObjects(THREAD_COUNT, hThreadSum, true, INFINITE) == WAIT_FAILED) {
		std::cout << "Wait failed.\n" << "Error: " << GetLastError() << std::endl;
		return 1; // same
	}
	for (int i = 0; i < THREAD_COUNT; ++i) {
		CloseHandle(hThreadSum[i]);
	}

	std::cout << "Сумма, которую должны были получить: " << SUM_TO_GET << '\n';
	std::cout << "Сумма, которую получили: ";
	for (int i = 0; i < THREAD_COUNT; ++i) {
		SUM += part_sums[i];
	}
	std::cout << SUM << std::endl;

	delete root;
	delete[] partition;
	delete[] part_sums;
	delete[] indexes;

	return 0;
}
