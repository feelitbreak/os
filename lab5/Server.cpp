//Кендысь Алексей, 2 курс, 9 группа. Лабораторная №5
#include <windows.h>
#include <conio.h>
#include <iostream>
using namespace std;

int main()
{
	cout << "Input the size of your mass." << endl;
	int n;
	cin >> n;
	__int8* mass = new __int8[n];
	srand((__int8)time(NULL));
	for (int i = 0; i < n; i++) {
		mass[i] = (__int8)rand() % 20 - 10;
	}
	char lpszComLine[80];

	HANDLE hEnableRead;
	char lpszEnableRead[] = "EnableRead";

	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	HANDLE hWritePipe, hReadPipe;
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
	
	HANDLE ReadyToRead = CreateEvent(NULL, FALSE, FALSE, "Ready");
	if (ReadyToRead == NULL)
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

	for (int i = 0; i < n; i++)
	{
		DWORD dwBytesWritten;
		if (!WriteFile(hWritePipe, &mass[i], sizeof(__int8), &dwBytesWritten, NULL))
		{
			_cputs("Write to file failed.\n");
			_cputs("Press any key to finish.\n");
			_getch();
			return GetLastError();
		}
		_cprintf("The number %d is written to the pipe.\n", mass[i]);
	}

	// закрываем дескрипторы нового процесса
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);


	// читаем из анонимного канала
	for (int i = 0; i < 10; i++)
	{
		int nData;
		DWORD dwBytesRead;
		if (!ReadFile(
			hReadPipe,
			&nData,
			sizeof(nData),
			&dwBytesRead,
			NULL))
		{
			_cputs("Read from the pipe failed.\n");
			_cputs("Press any key to finish.\n");
			_getch();
			return GetLastError();
		}
		_cprintf("The number %d is read from the pipe.\n", nData);
	}
	_cputs("The process finished reading from the pipe.\n");

	
	// закрываем дескрипторы канала
	CloseHandle(hReadPipe);
	CloseHandle(hWritePipe);
	CloseHandle(hEnableRead);

	_cputs("The process finished writing to the pipe.\n");
	_cputs("Press any key to exit.\n");
	_getch();

	return 0;
}