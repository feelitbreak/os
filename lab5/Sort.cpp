#include <windows.h>
#include <algorithm>
#include <iostream>

using std::sort;
using std::cin;
using std::cout;
using std::endl;

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

int main(int argc, char* argv[])
{
	HANDLE hWritePipe, hReadPipe;
	HANDLE ReadyToRead1, ReadyToRead2;

	ReadyToRead1 = OpenEvent(EVENT_ALL_ACCESS, FALSE, "Ready1");
	if (NULL == ReadyToRead1)
	{
		cout << "Failed to open event." << endl;
		system("pause");
		return GetLastError();
	}
	ReadyToRead2 = OpenEvent(EVENT_ALL_ACCESS, FALSE, "Ready2");
	if (NULL == ReadyToRead2)
	{
		CloseHandle(ReadyToRead1);

		cout << "Failed to open event." << endl;
		system("pause");
		return GetLastError();
	}

	hWritePipe = reinterpret_cast<HANDLE>(atoll(argv[1]));
	hReadPipe = reinterpret_cast<HANDLE>(atoll(argv[2]));

	WaitForSingleObject(ReadyToRead1, INFINITE);

	int n;
	__int8 elem;
	DWORD dwBytesRead;
	if (NULL == ReadFile(hReadPipe, &n, sizeof(n), &dwBytesRead, NULL))
	{
		CloseHandle(hReadPipe);
		CloseHandle(hWritePipe);
		CloseHandle(ReadyToRead2);
		CloseHandle(ReadyToRead1);

		cout << "Failed to read from the pipe." << endl;
		system("pause");
		return GetLastError();
	}
	cout << "The size of the array " << n << " has been read from the pipe." << endl;

	__int8* mass = new __int8[n];
	for (int i = 0; i < n; i++)
	{
		if (NULL == ReadFile(hReadPipe, &elem, sizeof(__int8), &dwBytesRead, NULL))
		{
			delete[] mass;
			CloseHandle(hReadPipe);
			CloseHandle(hWritePipe);
			CloseHandle(ReadyToRead2);
			CloseHandle(ReadyToRead1);

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
		if (NULL == WriteFile(hWritePipe, &mass[i], sizeof(__int8), &dwBytesWritten, NULL))
		{
			delete[] mass;
			CloseHandle(hReadPipe);
			CloseHandle(hWritePipe);
			CloseHandle(ReadyToRead2);
			CloseHandle(ReadyToRead1);

			cout << "Failed to write to file." << endl;
			system("pause");
			return GetLastError();
		}
		cout << "The element " << mass[i] << " has been written to the pipe." << endl;
	}
	cout << "The process finished writing to the pipe." << endl;

	SetEvent(ReadyToRead2);

	system("pause");

	delete[] mass;
	CloseHandle(hReadPipe);
	CloseHandle(hWritePipe);
	CloseHandle(ReadyToRead2);
	CloseHandle(ReadyToRead1);

	return 0;
}
