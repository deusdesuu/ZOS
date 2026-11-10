/*
	3 потока:
		- открыть файл
		- зашифровать данные
		- записать в файл
	3 потока:
		- открыть файл
		- расшифровать данные
		- записать в файл

	reader -> encoder -> writer
	data -> buffer -> encode -> file
*/
#include <Windows.h>
#include <iostream>
#include <tchar.h>
#include <locale>
#include <fstream>


// key
const char k = 8;
const char* key = "password";

HANDLE hMutex;

char buffer[k];
int buf_len = 0; // buffer elem count
bool buffer_free = true;
bool data_ready = false;
bool data_encoded = false;
bool data_decoded = false;
bool finished = false;

DWORD WINAPI reader1(LPVOID param) {
	std::ifstream stream("data.txt", std::ios::binary);
	bool can_read;
	while (true) {
		while (true) {
			WaitForSingleObject(hMutex, INFINITE);
			can_read = buffer_free;
			ReleaseMutex(hMutex);
			if (can_read) {
				break;
			}
		}

		WaitForSingleObject(hMutex, INFINITE);
		stream.read(reinterpret_cast<char*>(buffer), k);
		buf_len = (int)stream.gcount();
		buffer_free = false;
		if (buf_len > 0) {
			data_ready = true;
		}
		if (buf_len == 0) {
			finished = true;
		}
		ReleaseMutex(hMutex);
		if (buf_len == 0) {
			break;
		}
	}
	stream.close();
	return 0;
}
DWORD WINAPI encoder(LPVOID param) {
	bool can_encode, done;
	int start, end;
	while (true) {
		while (true) {
			WaitForSingleObject(hMutex, INFINITE);
			can_encode = data_ready;
			done = finished;
			ReleaseMutex(hMutex);
			if (can_encode || done) {
				break;
			}
		}
		if (!can_encode) {
			break;
		}
		WaitForSingleObject(hMutex, INFINITE);
		data_ready = false;
		start = 0;
		end = buf_len - 1;
		while(start < end) {
			buffer[start] ^= key[start];
			buffer[end] ^= key[end];
			std::swap(buffer[start++], buffer[end--]);
		}
		if (start == end) {
			buffer[start] ^= key[start];
		}
		data_encoded = true;
		ReleaseMutex(hMutex);
	}
	return 0;
}
DWORD WINAPI writer1(LPVOID param) {
	std::ofstream stream("encrypted.txt", std::ios::binary);
	bool can_write, done;
	while (true) {
		while (true) {
			WaitForSingleObject(hMutex, INFINITE);
			can_write = data_encoded;
			done = finished;
			ReleaseMutex(hMutex);
			if (can_write || done) {
				break;
			}
		}

		if (!can_write) {
			break;
		}
		WaitForSingleObject(hMutex, INFINITE);
		stream.write(buffer, buf_len);
		buffer_free = true;
		data_encoded = false;
		ReleaseMutex(hMutex);
	}
	stream.close();
	return 0;
}
DWORD WINAPI reader2(LPVOID param) {
	std::ifstream stream("encrypted.txt", std::ios::binary);
	bool can_read;
	while (true) {
		while (true) {
			WaitForSingleObject(hMutex, INFINITE);
			can_read = buffer_free;
			ReleaseMutex(hMutex);
			if (can_read) {
				break;
			}
		}

		WaitForSingleObject(hMutex, INFINITE);
		stream.read(reinterpret_cast<char*>(buffer), k);
		buf_len = (int)stream.gcount();
		buffer_free = false;
		if (buf_len > 0) {
			data_ready = true;
		}
		if (buf_len == 0) {
			finished = true;
		}
		ReleaseMutex(hMutex);
		if (buf_len == 0) {
			break;
		}
	}
	stream.close();
	return 0;
}
DWORD WINAPI decoder(LPVOID param) {
	bool can_decode, done;
	int start, end;
	while (true) {
		while (true) {
			WaitForSingleObject(hMutex, INFINITE);
			can_decode = data_ready;
			done = finished;
			ReleaseMutex(hMutex);
			if (can_decode || done) {
				break;
			}
		}
		if (!can_decode) {
			break;
		}
		WaitForSingleObject(hMutex, INFINITE);
		start = 0;
		end = buf_len - 1;
		while (start < end) {
			std::swap(buffer[start], buffer[end]);
			buffer[start] ^= key[start];
			buffer[end] ^= key[end];
			++start; --end;
		}
		if (start == end) {
			buffer[start] ^= key[start];
		}
		data_ready = false;
		data_decoded = true;
		ReleaseMutex(hMutex);
	}
	return 0;
}
DWORD WINAPI writer2(LPVOID param) {
	std::ofstream stream("decrypted.txt", std::ios::binary);
	bool can_write, done;
	while (true) {
		while (true) {
			WaitForSingleObject(hMutex, INFINITE);
			can_write = data_decoded;
			done = finished;
			ReleaseMutex(hMutex);
			if (can_write || done) {
				break;
			}
		}

		if (!can_write) {
			break;
		}
		WaitForSingleObject(hMutex, INFINITE);
		stream.write(buffer, buf_len);
		buffer_free = true;
		data_decoded = false;
		ReleaseMutex(hMutex);
	}
	stream.close();
	return 0;
}

int _tmain(int argc, _TCHAR* argv[]) {
	//std::setlocale(LC_ALL, "rus");
	hMutex = CreateMutex(NULL, false, NULL);

	HANDLE hReader1 = CreateThread(NULL, 0, reader1, NULL, 0, NULL);
	HANDLE hEncoder = CreateThread(NULL, 0, encoder, NULL, 0, NULL);
	HANDLE hWriter1 = CreateThread(NULL, 0, writer1, NULL, 0, NULL);

	HANDLE hThreadEncode[3];
	hThreadEncode[0] = hReader1;
	hThreadEncode[1] = hEncoder;
	hThreadEncode[2] = hWriter1;

	WaitForMultipleObjects(3, hThreadEncode, true, INFINITE);

	CloseHandle(hReader1);
	CloseHandle(hEncoder);
	CloseHandle(hWriter1);
	CloseHandle(hMutex);

	hMutex = CreateMutex(NULL, false, NULL);

	buf_len = 0; // buffer elem count
	buffer_free = true;
	data_ready = false;
	data_decoded = false;
	finished = false;

	HANDLE hReader2 = CreateThread(NULL, 0, reader2, NULL, 0, NULL);
	HANDLE hDecoder = CreateThread(NULL, 0, decoder, NULL, 0, NULL);
	HANDLE hWriter2 = CreateThread(NULL, 0, writer2, NULL, 0, NULL);

	HANDLE hThreadDecode[3];
	hThreadDecode[0] = hReader2;
	hThreadDecode[1] = hDecoder;
	hThreadDecode[2] = hWriter2;

	WaitForMultipleObjects(3, hThreadDecode, true, INFINITE);

	CloseHandle(hReader2);
	CloseHandle(hDecoder);
	CloseHandle(hWriter2);
	CloseHandle(hMutex);

	return 0;
}
