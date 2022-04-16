#include <windows.h>
#include <iostream>
#include <string>

using namespace std;

int main(int argc, char* argv[])
{
	int n = atoi(argv[1]);

	HANDLE hSemaphore;
	hSemaphore = OpenSemaphore(SYNCHRONIZE, FALSE, "ParentSemaphore");
	if (hSemaphore == NULL)
	{
		cout << "Open semaphore failed." << endl;
		cout << "Press any key to exit." << endl;
		cin.get();

		return GetLastError();
	}

	HANDLE EventA = OpenEvent(SYNCHRONIZE, FALSE, "A");
	if (EventA == NULL)
		return GetLastError();
	HANDLE EventEndParent = OpenEvent(SYNCHRONIZE, FALSE, "EndParent");
	if (EventEndParent == NULL)
		return GetLastError();

	WaitForSingleObject(hSemaphore, INFINITE);
	char mes;
	int i = 0;
	while(i < n)
	{
		cout << "Input message." << endl;
		cin >> mes;
		if (mes == 'A')
		{
			i++;
			SetEvent(EventA);
		}
	}
	ReleaseSemaphore(hSemaphore, 1, NULL);

	WaitForSingleObject(EventEndParent, INFINITE);

	CloseHandle(hSemaphore);

	return 0;
}
