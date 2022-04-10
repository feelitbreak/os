#include <windows.h>
#include <iostream>
#include <string>

using namespace std;

int main()
{
	HANDLE hSemaphore;
	hSemaphore = OpenSemaphore(SYNCHRONIZE, FALSE, "ParentSemaphore");
	if (hSemaphore == NULL)
	{
		cout << "Open semaphore failed." << endl;
		cout << "Press any key to exit." << endl;
		cin.get();

		return GetLastError();
	}

	string mess;
	cout << "Input A message." << endl;
	cin >> mess;

	HANDLE EventA = OpenEvent(SYNCHRONIZE, FALSE, "A");
	if (EventA == NULL)
		return GetLastError();


	SetEvent(EventA);
	ReleaseSemaphore(hSemaphore, 1, NULL);

	CloseHandle(hSemaphore);

	return 0;
}
