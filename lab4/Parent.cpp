#include <windows.h>
#include <iostream>
#include <string>

using namespace std;

int main(int argc, char* argv[])
{
	int n = atoi(argv[1]);

	HANDLE hSemaphore;
	hSemaphore = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, "ParentSemaphore");
	if (hSemaphore == NULL)
	{
		cout << "Open semaphore failed." << endl;
		cout << "Press any key to exit." << endl;
		cin.get();

		return GetLastError();
	}

	HANDLE EventA = OpenEvent(EVENT_MODIFY_STATE, FALSE, "A");
	if (EventA == NULL)
		return GetLastError();
	HANDLE EventEndParent = OpenEvent(SYNCHRONIZE, FALSE, "EndParent");
	if (EventEndParent == NULL)
		return GetLastError();

	WaitForSingleObject(hSemaphore, INFINITE);
	cout << "I am active.\n";
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

	ReleaseSemaphore(hSemaphore, 1, NULL);
	cout << "Waiting for other processes.\n";

	WaitForSingleObject(EventEndParent, INFINITE);

	CloseHandle(hSemaphore);
	CloseHandle(EventEndParent);

	return 0;
}
