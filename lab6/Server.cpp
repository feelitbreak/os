//Кендысь Алексей, 2 курс, 9 группа. Лабораторная №6

#include <windows.h>
#include <iostream>

using std::cin;
using std::cout;
using std::endl;

static const int MAX_STR_LENGTH = 80;
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
	HANDLE hNamedPipe;
	char pipeName[MAX_STR_LENGTH];
	SECURITY_ATTRIBUTES sa;
	SECURITY_DESCRIPTOR sd;

	int n;
	cout << "Input the size of your mass." << endl;
	cin >> n;

	__int8* mass;
	mass = new __int8[n];

	generateMass(n, mass);

	strcpy_s(pipeName, MAX_STR_LENGTH, "\\\\.\\pipe\\demo_pipe");

	sa.nLength = sizeof(sa);
	sa.bInheritHandle = FALSE;
	InitializeSecurityDescriptor(&sd, SECURITY_DESCRIPTOR_REVISION);
	PACL pAcl = NULL;
	SetSecurityDescriptorDacl(&sd, TRUE, pAcl, FALSE);
	sa.lpSecurityDescriptor = &sd;

	hNamedPipe = CreateNamedPipe(pipeName, PIPE_ACCESS_DUPLEX, PIPE_READMODE_MESSAGE | PIPE_TYPE_MESSAGE | PIPE_WAIT, 
		1, 0, 0, INFINITE, &sa);
	if (INVALID_HANDLE_VALUE == hNamedPipe)
	{
		delete[] mass;

		cout << "Failed to create pipe." << endl;
		system("pause");
		return GetLastError();
	}

	cout << "The server is waiting for connection with the client." << endl;
	if (NULL == ConnectNamedPipe(hNamedPipe, (LPOVERLAPPED)NULL))
	{
		CloseHandle(hNamedPipe);
		delete[] mass;

		cout << "The connection failed." << endl;
		system("pause");
		return GetLastError();
	}


	DWORD dwBytesWritten;
	if (NULL == WriteFile(hNamedPipe, &n, sizeof(int), &dwBytesWritten, (LPOVERLAPPED)NULL))
	{
		CloseHandle(hNamedPipe);
		delete[] mass;

		cout << "Failed to write to file." << endl;
		system("pause");
		return GetLastError();
	}	
	cout << "The size of the array " << n << " has been written to the pipe." << endl;

	for (int i = 0; i < n; i++)
	{
		if (NULL == WriteFile(hNamedPipe, &mass[i], sizeof(__int8), &dwBytesWritten, (LPOVERLAPPED)NULL))
		{
			CloseHandle(hNamedPipe);
			delete[] mass;

			cout << "Failed to write to file." << endl;
			system("pause");
			return GetLastError();
		}
		cout << "The element " << mass[i] << " has been written to the pipe." << endl;
	}
	cout << "The process finished writing to the pipe." << endl;

	DWORD dwBytesRead;
	for (int i = 0; i < n; i++)
	{
		if (NULL == ReadFile(hNamedPipe, &mass[i], sizeof(__int8), &dwBytesRead, (LPOVERLAPPED)NULL))
		{
			CloseHandle(hNamedPipe);
			delete[] mass;

			cout << "Failed to read from the pipe." << endl;
			system("pause");
			return GetLastError();
		}
		cout << "The element " << mass[i] << " has been read from the pipe." << endl;
	}
	cout << "The process finished reading from the pipe." << endl;

	system("pause");

	CloseHandle(hNamedPipe);
	delete[] mass;

	return 0;
}