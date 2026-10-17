/*
	- текст длиной n символов
	- ключ длиной k символов
	- разделить текст на блоки длиной k символов
		- каждый блок это [ixk : (i + 1)xk -1]
	- перестановка символов в каждом блоке (отразить)
		- for (int j = 0; i < len/2; ++i) { swap([i], [-i]); } 
	- применить ключ к каждому блоку: part[i] = postswap[i] xor key[i]
*/
#include <Windows.h>
#include <iostream>
#include <tchar.h>
#include <stdlib.h>
#include <format>


const int n = 13;
const char* source = "Hello, World!";

const int k = 8;
const char* key = "password";

char buffer[n + 1];

const int parts = (n + k - 1) / k;

int parts_numbers[parts];


DWORD WINAPI encode_part(LPVOID param) {
	// Defining block boundaries
	int start = *(int*)param * k;
	int end = start + k - 1;
	if (n <= end) {
		end = n - 1;
	}

	int i;
	int len = end - start;
	// XOR'ing the symbols and then swapping
	for (i = 0; start < end; ++i) {
		buffer[start] ^= key[i];
		buffer[end] ^= key[len - i];
		std::swap(buffer[start++], buffer[end--]);
	}

	if (start == end) {
		buffer[start] ^= key[i];
	}

	return 0;
}


int _tmain(int argc, _TCHAR* argv[]) {
	memcpy(buffer, source, 14);
	std::cout << "Source:\n" << buffer << '\n';
	std::cout << "Key: " << key << '\n' << '\n';
	
	HANDLE hThreadEncode[parts];
	DWORD dwThreadEncode[parts];

	for (int i = 0; i < parts; ++i) {
		parts_numbers[i] = i;
	}

	for (int i = 0; i < parts; ++i) {
		hThreadEncode[i] = CreateThread(NULL, 0, encode_part, &(parts_numbers[i]), 0, &(dwThreadEncode[i]));
		if (hThreadEncode[i] == NULL) {
			std::cout << "Thread № " << i << " was not created\n" << "Error: " << GetLastError();
			return 1; // cant work without mtx so return 1 if error occured
		}
	}
	if (WaitForMultipleObjects(parts, hThreadEncode, true, INFINITE) == WAIT_FAILED) {
		std::cout << "Wait failed.\n" << "Error: " << GetLastError() << std::endl;
		return 1; // same
	}
	for (int i = 0; i < parts; ++i) {
		CloseHandle(hThreadEncode[i]);
	}

	std::cout << "Result:\n";
	for (int i = 0; i < n; ++i) {
		std::cout << std::format("{:02x} ", (unsigned char)buffer[i]);
	}
	std::cout << std::endl;

	return 0;
}
