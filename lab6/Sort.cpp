#include <windows.h>
#include <algorithm>
#include <iostream>

using std::cin;
using std::cout;
using std::endl;
using std::sort;

static const int MAX_STR_LENGTH = 80;

void mySort(int n, __int8* mass)
{
	sort(mass, mass + n, [](__int8 x, __int8 y) { return (x < y); });
}

void outMass(int n, __int8* mass)
{
	for (int i = 0; i < n; i++) {
		cout << mass[i] << " ";
	}
	cout << endl;
}

int main()
{
	char machineName[MAX_STR_LENGTH];
	char pipeName[MAX_STR_LENGTH];
	HANDLE hNamedPipe;

	cout << "Input the name of the server machine." << endl;
	cin >> machineName;

	strcpy_s(pipeName, MAX_STR_LENGTH, "\\\\");
	strcat_s(pipeName, MAX_STR_LENGTH, machineName);
	strcat_s(pipeName, MAX_STR_LENGTH, "\\pipe\\demo_pipe");

	hNamedPipe = CreateFile(pipeName, GENERIC_WRITE | GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE,
		(LPSECURITY_ATTRIBUTES)NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, (HANDLE)NULL);
	if (INVALID_HANDLE_VALUE == hNamedPipe)
	{
		cout << "Failed to create file." << endl;
		system("pause");
		return GetLastError();
	}


	int n = 0;
	__int8 elem;
	__int8* mass;
	DWORD dwBytesRead;


	if (NULL == ReadFile(hNamedPipe, &n, sizeof(n), &dwBytesRead, (LPOVERLAPPED)NULL))
	{
		CloseHandle(hNamedPipe);

		cout << "Failed to read from the pipe." << endl;
		system("pause");
		return GetLastError();
	}
	cout << "The size of the array " << n << " has been read from the pipe." << endl;

	mass = new __int8[n];
	for (int i = 0; i < n; i++)
	{
		if (NULL == ReadFile(hNamedPipe, &elem, sizeof(__int8), &dwBytesRead, (LPOVERLAPPED)NULL))
		{
			delete[] mass;
			CloseHandle(hNamedPipe);

			cout << "Failed to read from the pipe." << endl;
			system("pause");
			return GetLastError();
		}
		cout << "The element " << elem << " has been read from the pipe." << endl;
		mass[i] = elem;
	}
	cout << "The process finished reading from the pipe." << endl;

	mySort(n, mass);
	cout << "Sorted array: ";
	outMass(n, mass);

	for (int i = 0; i < n; i++)
	{
		DWORD dwBytesWritten;
		if (NULL == WriteFile(hNamedPipe, &mass[i], sizeof(__int8), &dwBytesWritten, (LPOVERLAPPED)NULL))
		{
			delete[] mass;
			CloseHandle(hNamedPipe);

			cout << "Failed to write to file." << endl;
			system("pause");
			return GetLastError();
		}
		cout << "The element " << mass[i] << " has been written to the pipe." << endl;
	}
	cout << "The process finished writing to the pipe." << endl;

	system("pause");

	delete[] mass;
	CloseHandle(hNamedPipe);

	return 0;
}
