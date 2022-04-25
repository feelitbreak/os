//Кендысь Алексей, 2 курс, 9 группа. Лабораторная №5
#include <windows.h>
#include <conio.h>
#include <iostream>
using namespace std;

int main()
{
	int n;
	cout << "Input the size of your mass." << endl;
	cin >> n;
	__int8* mass = new __int8[n];
	srand((__int8)time(NULL));
	for (int i = 0; i < n; i++) {
		mass[i] = (__int8)rand() % 20 - 10;
	}
	char lpszComLine[80];

	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	HANDLE hWritePipe, hReadPipe, ReadyToRead1, ReadyToRead2;
	SECURITY_ATTRIBUTES sa;

	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor = NULL;	
	sa.bInheritHandle = TRUE;

	if (!CreatePipe(&hReadPipe,	&hWritePipe, &sa, 0))
	{
		_cputs("Create pipe failed.\n");
		_cputs("Press any key to finish.\n");
		_getch();

		return GetLastError();
	}
	
	ReadyToRead1 = CreateEvent(NULL, FALSE, FALSE, "Ready1");
	if (ReadyToRead1 == NULL)
		return GetLastError();
	ReadyToRead2 = CreateEvent(NULL, FALSE, FALSE, "Ready2");
	if (ReadyToRead2 == NULL)
		return GetLastError();

	ZeroMemory(&si, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);

	wsprintf(lpszComLine, "Sort.exe %d %d", (int)hWritePipe, (int)hReadPipe);

	if (!CreateProcess(NULL, lpszComLine, NULL, NULL, FALSE,
		CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi))
	{
		_cputs("The new process is not created.\n");
		_cputs("Check the name of the process.\n");
		_cputs("Press any key to finish.\n");
		_getch();
		return 0;
	}
	_cputs("The new process is created.\n");

	// закрываем дескрипторы нового процесса
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);

	DWORD dwBytesWritten;
	if (!WriteFile(hWritePipe, &n, sizeof(int), &dwBytesWritten, NULL))
	{
		_cputs("Write to file failed.\n");
		_cputs("Press any key to finish.\n");
		_getch();
		return GetLastError();
	}
	_cprintf("The size of the array %d has been written to the pipe.\n", n);
	for (int i = 0; i < n; i++)
	{
		if (!WriteFile(hWritePipe, &mass[i], sizeof(__int8), &dwBytesWritten, NULL))
		{
			_cputs("Write to file failed.\n");
			_cputs("Press any key to finish.\n");
			_getch();
			return GetLastError();
		}
		_cprintf("The element %c has been written to the pipe.\n", mass[i]);
	}
	_cputs("The process finished writing to the pipe.\n");

	SetEvent(ReadyToRead1);


	WaitForSingleObject(ReadyToRead2, INFINITE);
	DWORD dwBytesRead;
	for (int i = 0; i < n; i++)
	{
		if (!ReadFile(
			hReadPipe,
			&mass[i],
			sizeof(__int8),
			&dwBytesRead,
			NULL))
		{
			_cputs("Read from the pipe failed.\n");
			_cputs("Press any key to finish.\n");
			_getch();
			return GetLastError();
		}
		_cprintf("The element %c has been read from the pipe.\n", mass[i]);
	}
	_cputs("The process finished reading from the pipe.\n");

	
	CloseHandle(hReadPipe);
	CloseHandle(hWritePipe);
	CloseHandle(ReadyToRead1);
	CloseHandle(ReadyToRead2);

	_cputs("Press any key to exit.\n");
	_getch();

	return 0;
}