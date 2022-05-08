//Кендысь Алексей, 2 курс, 9 группа. Лабораторная №5
#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <conio.h>
#include <iostream>
static const int MAX_STR_LENGTH = 80;

int main()
{
	char pipeName[MAX_STR_LENGTH];
	HANDLE hNamedPipe;
	SECURITY_ATTRIBUTES sa;
	SECURITY_DESCRIPTOR sd;

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

	sa.nLength = sizeof(sa);
	sa.bInheritHandle = FALSE;
	InitializeSecurityDescriptor(&sd, SECURITY_DESCRIPTOR_REVISION);
	SetSecurityDescriptorDacl(&sd, TRUE, NULL, FALSE);
	sa.lpSecurityDescriptor = &sd;

	hNamedPipe = CreateNamedPipe(pipeName, PIPE_ACCESS_DUPLEX, PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT, 1, 0, 0, INFINITE, &sa);
	if (hNamedPipe == INVALID_HANDLE_VALUE)
	{
		_cputs("Create pipe failed.\n");
		_cputs("Press any key to finish.\n");
		_getch();

		return GetLastError();
	}

	_cputs("The server is waiting for connection with the client.\n");
	if (!ConnectNamedPipe(hNamedPipe, NULL))
	{
		CloseHandle(hNamedPipe);
		_cputs("The connection failed.\n");
		_cputs("Press any key to finish.\n");
		_getch();
		return GetLastError();
	}

	DWORD dwBytesWritten;
	if (!WriteFile(hNamedPipe, &n, sizeof(int), &dwBytesWritten, NULL))
	{
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
			CloseHandle(hNamedPipe);
			_cputs("Write to file failed.\n");
			_cputs("Press any key to finish.\n");
			_getch();
			return GetLastError();
		}
		_cprintf("The element %c has been written to the pipe.\n", mass[i]);
	}
	_cputs("The process finished writing to the pipe.\n");

	DWORD dwBytesRead;
	for (int i = 0; i < n; i++)
	{
		if (!ReadFile(hNamedPipe, &mass[i], sizeof(__int8), &dwBytesRead, NULL))
		{
			CloseHandle(hNamedPipe);
			_cputs("Read from the pipe failed.\n");
			_cputs("Press any key to finish.\n");
			_getch();
			return GetLastError();
		}
		_cprintf("The element %c has been read from the pipe.\n", mass[i]);
	}
	_cputs("The process finished reading from the pipe.\n");

	DisconnectNamedPipe(hNamedPipe);

	_cputs("Press any key to exit.\n");
	_getch();

	CloseHandle(hNamedPipe);

	return 0;
}