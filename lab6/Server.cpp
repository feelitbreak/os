//Кендысь Алексей, 2 курс, 9 группа. Лабораторная №5
#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <conio.h>
#include <iostream>
static const int MAX_STR_LENGTH = 80;

int main()
{
	char lpszComLine[MAX_STR_LENGTH];
	char pipeName[MAX_STR_LENGTH];
	HANDLE hNamedPipe;
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	int n;
	__int8* mass;
	_cputs("Input the size of your mass.\n");
	_cscanf("%d", &n);
	mass = new __int8[n];
	srand((__int8)time(NULL));
	for (int i = 0; i < n; i++) {
		mass[i] = (__int8)rand() % ('z' - '0') + '0';
	}

	strcpy(pipeName, "\\\\.\\pipe\\demo_pipe");

	hNamedPipe = CreateNamedPipe(
		pipeName,
		PIPE_ACCESS_DUPLEX,
		PIPE_TYPE_MESSAGE | PIPE_WAIT,
		1,
		0,
		0,
		INFINITE,
		(LPSECURITY_ATTRIBUTES)NULL
	);
	if (hNamedPipe == INVALID_HANDLE_VALUE)
	{
		_cputs("Create pipe failed.\n");
		_cputs("Press any key to finish.\n");
		_getch();

		return GetLastError();
	}

	ZeroMemory(&si, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);

	strcpy(lpszComLine, "Sort.exe ");
	strcat(lpszComLine, pipeName);

	if (!CreateProcess(NULL, lpszComLine, NULL, NULL, TRUE,
		CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi))
	{
		CloseHandle(hNamedPipe);
		_cputs("The new process is not created.\n");
		_cputs("Check the name of the process.\n");
		_cputs("Press any key to finish.\n");
		_getch();
		return 0;
	}
	_cputs("The new process is created.\n");


	DWORD dwBytesWritten;
	if (!WriteFile(hNamedPipe, &n, sizeof(int), &dwBytesWritten, NULL))
	{
		CloseHandle(pi.hThread);
		CloseHandle(pi.hProcess);
		CloseHandle(hNamedPipe);
		_cputs("Write to file failed.\n");
		_cputs("Press any key to finish.\n");
		_getch();
		return GetLastError();
	}	
	_cprintf("The size of the array %d has been written to the pipe.\n", n);
	for (int i = 0; i < n; i++)
	{
		if (!WriteFile(hNamedPipe, &mass[i], sizeof(__int8), &dwBytesWritten, NULL))
		{
			CloseHandle(pi.hThread);
			CloseHandle(pi.hProcess);
			CloseHandle(hNamedPipe);
			_cputs("Write to file failed.\n");
			_cputs("Press any key to finish.\n");
			_getch();
			return GetLastError();
		}
		_cprintf("The element %c has been written to the pipe.\n", mass[i]);
	}
	_cputs("The process finished writing to the pipe.\n");

	_cputs("The server is waiting for connection with the client.\n");
	if (!ConnectNamedPipe(hNamedPipe, (LPOVERLAPPED)NULL))
	{
		CloseHandle(pi.hThread);
		CloseHandle(pi.hProcess);
		CloseHandle(hNamedPipe);
		_cputs("The connection failed.\n");
		_cputs("Press any key to finish.\n");
		_getch();
		return GetLastError();
	}

	DWORD dwBytesRead;
	for (int i = 0; i < n; i++)
	{
		if (!ReadFile(hNamedPipe, &mass[i], sizeof(__int8), &dwBytesRead, NULL))
		{
			CloseHandle(pi.hThread);
			CloseHandle(pi.hProcess);
			CloseHandle(hNamedPipe);
			_cputs("Read from the pipe failed.\n");
			_cputs("Press any key to finish.\n");
			_getch();
			return GetLastError();
		}
		_cprintf("The element %c has been read from the pipe.\n", mass[i]);
	}
	_cputs("The process finished reading from the pipe.\n");

	CloseHandle(hNamedPipe);
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);

	_cputs("Press any key to exit.\n");
	_getch();

	return 0;
}