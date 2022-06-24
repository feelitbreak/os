//Кендысь Алексей, 2 курс, 9 группа. Лабораторная №5
#include <windows.h>
#include <iostream>

using std::cin;
using std::cout;
using std::endl;

static int const maxLength = 80;
static char const lastElem = 'Z';
static char const firstElem = 'A';

void generateMass(int n, __int8* mass)
{
	srand((__int8)time(NULL));
	for (int i = 0; i < n; i++) {
		mass[i] = (__int8)rand() % (lastElem - firstElem) + firstElem;
	}
}

int main()
{
	int n;
	cout << "Input the size of your mass." << endl;
	cin >> n;

	__int8* mass;
	mass = new __int8[n];

	generateMass(n, mass);

	char lpszComLine[maxLength];
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	HANDLE hWritePipe, hReadPipe, ReadyToRead1, ReadyToRead2;
	SECURITY_ATTRIBUTES sa;

	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor = NULL;	
	sa.bInheritHandle = TRUE;

	if (NULL == CreatePipe(&hReadPipe,	&hWritePipe, &sa, 0))
	{
		delete[] mass;

		cout << "Failed to create pipe." << endl;
		system("pause");
		return GetLastError();
	}
	
	ReadyToRead1 = CreateEvent(NULL, FALSE, FALSE, "Ready1");
	if (NULL == ReadyToRead1)
	{
		CloseHandle(hReadPipe);
		CloseHandle(hWritePipe);
		delete[] mass;

		cout << "Failed to create event." << endl;
		system("pause");
		return GetLastError();
	}
	ReadyToRead2 = CreateEvent(NULL, FALSE, FALSE, "Ready2");
	if (NULL == ReadyToRead2)
	{
		CloseHandle(ReadyToRead1);
		CloseHandle(hReadPipe);
		CloseHandle(hWritePipe);
		delete[] mass;

		cout << "Failed to create event." << endl;
		system("pause");
		return GetLastError();
	}

	ZeroMemory(&si, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);

	sprintf_s(lpszComLine, maxLength, "Sort.exe %lld %lld", reinterpret_cast<intptr_t>(hWritePipe), reinterpret_cast<intptr_t>(hReadPipe));

	if (NULL == CreateProcess(NULL, lpszComLine, NULL, NULL, TRUE,
		CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi))
	{
		CloseHandle(ReadyToRead2);
		CloseHandle(ReadyToRead1);
		CloseHandle(hReadPipe);
		CloseHandle(hWritePipe);
		delete[] mass;

		cout << "Failed to create process." << endl;
		system("pause");
		return 0;
	}
	cout << "The new process has been created." << endl;


	DWORD dwBytesWritten;
	if (NULL == WriteFile(hWritePipe, &n, sizeof(int), &dwBytesWritten, NULL))
	{
		CloseHandle(pi.hThread);
		CloseHandle(pi.hProcess);
		CloseHandle(ReadyToRead2);
		CloseHandle(ReadyToRead1);
		CloseHandle(hReadPipe);
		CloseHandle(hWritePipe);
		delete[] mass;

		cout << "Failed to write to file." << endl;
		system("pause");
		return GetLastError();
	}	
	cout << "The size of the array " << n << " has been written to the pipe." << endl;

	for (int i = 0; i < n; i++)
	{
		if (NULL == WriteFile(hWritePipe, &mass[i], sizeof(__int8), &dwBytesWritten, NULL))
		{
			CloseHandle(pi.hThread);
			CloseHandle(pi.hProcess);
			CloseHandle(ReadyToRead2);
			CloseHandle(ReadyToRead1);
			CloseHandle(hReadPipe);
			CloseHandle(hWritePipe);
			delete[] mass;

			cout << "Failed to write to file." << endl;
			system("pause");
			return GetLastError();
		}
		cout << "The element " << mass[i] << " has been written to the pipe." << endl;
	}
	cout << "The process finished writing to the pipe." << endl;

	SetEvent(ReadyToRead1);

	WaitForSingleObject(ReadyToRead2, INFINITE);
	DWORD dwBytesRead;
	for (int i = 0; i < n; i++)
	{
		if (NULL == ReadFile(hReadPipe, &mass[i], sizeof(__int8), &dwBytesRead, NULL))
		{
			CloseHandle(pi.hThread);
			CloseHandle(pi.hProcess);
			CloseHandle(ReadyToRead2);
			CloseHandle(ReadyToRead1);
			CloseHandle(hReadPipe);
			CloseHandle(hWritePipe);
			delete[] mass;

			cout << "Failed to read from the pipe." << endl;
			system("pause");
			return GetLastError();
		}
		cout << "The element " << mass[i] << " has been read from the pipe." << endl;
	}
	cout << "The process finished reading from the pipe." << endl;

	system("pause");
	
	CloseHandle(pi.hThread);
	CloseHandle(pi.hProcess);
	CloseHandle(ReadyToRead2);
	CloseHandle(ReadyToRead1);
	CloseHandle(hReadPipe);
	CloseHandle(hWritePipe);
	delete[] mass;

	return 0;
}