#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <conio.h>
#include <algorithm>
#include <iostream>
static const int MAX_STR_LENGTH = 80;

using std::sort;

int main(int argc, char* argv[])
{
	HANDLE hNamedPipe;
	char machineName[MAX_STR_LENGTH];
	char pipeName[MAX_STR_LENGTH];

	_cputs("Input the name of the server machine.\n");
	_cscanf("%s", machineName);
	strcpy(pipeName, "\\\\");
	strcat(pipeName, machineName);
	strcat(pipeName, "\\pipe\\demo_pipe");

	hNamedPipe = CreateFile(
		pipeName,
		GENERIC_WRITE | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		(LPSECURITY_ATTRIBUTES)NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		(HANDLE)NULL
	);
	if (hNamedPipe == INVALID_HANDLE_VALUE)
	{
		_cputs("Create pipe failed.\n");
		_cputs("Press any key to finish.\n");
		_getch();

		return GetLastError();
	}

	int n = 0;
	__int8 elem;
	__int8* mass;
	DWORD dwBytesRead;

	if (!ReadFile(hNamedPipe, &n, sizeof(n), &dwBytesRead, NULL))
	{
		CloseHandle(hNamedPipe);
		_cputs("Read from the pipe failed.\n");
		_cputs("Press any key to finish.\n");
		_getch();
		return GetLastError();
	}
	_cprintf("The size of the array %d has been read from the pipe.\n", n);

	mass = new __int8[n];
	for (int i = 0; i < n; i++)
	{
		if (!ReadFile(hNamedPipe, &elem, sizeof(__int8), &dwBytesRead, NULL))
		{
			CloseHandle(hNamedPipe);
			_cputs("Read from the pipe failed.\n");
			_cputs("Press any key to finish.\n");
			_getch();
			return GetLastError();
		}
		_cprintf("The element %c has been read from the pipe.\n", elem);
		mass[i] = elem;
	}
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
		if (!WriteFile(hNamedPipe, &mass[i], sizeof(__int8), &dwBytesWritten, NULL))
		{
			CloseHandle(hNamedPipe);
			_cputs("Write to file failed.\n");
			_cputs("Press any key to finish.\n");
			_getch();
			return GetLastError();
		}
		_cprintf("The element %c has been written to the pipe.\n", mass[i]);
	}
	_cputs("The process finished writing to the pipe.\n");

	CloseHandle(hNamedPipe);
	
	_cputs("Press any key to exit.\n");
	_getch();

	return 0;
}
