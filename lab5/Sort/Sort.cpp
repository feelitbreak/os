// Пример процесса клиента анонимного канала.
// Клиент сначала пишет в анонимный канал, а потом читает из него.
// Дескриптор анонимного канала передается клиенту через командную строку. 

#include <windows.h>
#include <conio.h>
#include <algorithm>
#include <iostream>

using namespace std;

int main(int argc, char* argv[])
{
	HANDLE hWritePipe, hReadPipe;
	HANDLE ReadyToRead1, ReadyToRead2;

	ReadyToRead1 = OpenEvent(EVENT_ALL_ACCESS, FALSE, "Ready1");
	if (ReadyToRead1 == NULL)
		return GetLastError();
	ReadyToRead2 = OpenEvent(EVENT_ALL_ACCESS, FALSE, "Ready2");
	if (ReadyToRead2 == NULL)
		return GetLastError();

	hWritePipe = (HANDLE)atoi(argv[1]);
	hReadPipe = (HANDLE)atoi(argv[2]);
	cout << hReadPipe << endl;
	_cprintf("%s\n", argv[2]);
	_cprintf("%d", (int)hReadPipe);

	WaitForSingleObject(ReadyToRead1, INFINITE);

	int n;
	__int8 elem;
	DWORD dwBytesRead;
	if (!ReadFile(
		hReadPipe,
		&n,
		sizeof(n),
		&dwBytesRead,
		NULL))
	{
		_cputs("Read from the pipe failed.\n");
		_cputs("Press any key to finish.\n");
		_getch();
		return GetLastError();
	}
	_cprintf("The size of the array %d has been read from the pipe.\n", n);
	__int8* mass = new __int8[n];
	/*for (int i = 0; i < n; i++)
	{
		if (!ReadFile(
			hReadPipe,
			&elem,
			sizeof(__int8),
			&dwBytesRead,
			NULL))
		{
			_cputs("Read from the pipe failed.\n");
			_cputs("Press any key to finish.\n");
			_getch();
			return GetLastError();
		}
		_cprintf("The element %c has been read from the pipe.\n", elem);
		mass[i] = elem;
	}*/
	_cputs("The process finished reading from the pipe.\n");

	sort(mass, mass + n, [](__int8 x, __int8 y) { return (x < y); });
	_cputs("Sorted array: ");
	for (int i = 0; i < n; i++) {
		_cprintf("%c ", mass[i]);
	}
	_cputs("\n");


	for (int i = 0; i < n; i++)
	{
		DWORD dwBytesWritten;
		if (!WriteFile(
			hWritePipe,
			&mass[i],
			sizeof(__int8),
			&dwBytesWritten,
			NULL))
		{
			_cputs("Write to file failed.\n");
			_cputs("Press any key to finish.\n");
			_getch();
			return GetLastError();
		}
		_cprintf("The elem %d has been written to the pipe.\n", mass[i]);
	}
	_cputs("The process finished writing to the pipe.\n");
	SetEvent(ReadyToRead2);

	
	_cputs("Press any key to exit.\n");
	_getch();

	// закрываем дескрипторы канала
	CloseHandle(hWritePipe);
	CloseHandle(hReadPipe);
	CloseHandle(ReadyToRead1);
	CloseHandle(ReadyToRead2);

	return 0;
}
