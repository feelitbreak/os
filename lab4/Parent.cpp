#include <windows.h>
#include <iostream>

using std::cin;
using std::cout;
using std::endl;

static const int releaseNum = 1;

int main(int argc, char* argv[])
{
	int n = atoi(argv[1]);

	HANDLE hSemaphore;
	hSemaphore = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, "ParentSemaphore");
	if (NULL == hSemaphore)
	{
		cout << "Error. Failed to open semaphore." << endl;
		system("pause");
		return GetLastError();
	}

	HANDLE EventA = OpenEvent(EVENT_MODIFY_STATE, FALSE, "A");
	if (NULL == EventA)
	{
		CloseHandle(hSemaphore);

		cout << "Error. Failed to open event." << endl;
		system("pause");
		return GetLastError();
	}
	HANDLE EventEndParent = OpenEvent(SYNCHRONIZE, FALSE, "EndParent");
	if (NULL == EventEndParent)
	{
		CloseHandle(EventA);
		CloseHandle(hSemaphore);

		cout << "Error. Failed to open event." << endl;
		system("pause");
		return GetLastError();
	}

	WaitForSingleObject(hSemaphore, INFINITE);
	cout << "I am active." << endl;

	char mes;
	for (int i = 0; i < n; i++) {
		cout << "Input message." << endl;
		cin >> mes;
		if (mes == 'A')
		{
			SetEvent(EventA);
		} 
		else 
		{
			i--;
		}
	}

	ReleaseSemaphore(hSemaphore, releaseNum, NULL);

	cout << "Waiting for other processes." << endl;

	WaitForSingleObject(EventEndParent, INFINITE);

	CloseHandle(EventEndParent);
	CloseHandle(EventA);
	CloseHandle(hSemaphore);

	return 0;
}
