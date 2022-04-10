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

	string mess;
	for (int i = 0; i < n; i++)
	{
		cout << "Input A message." << endl;
		cin >> mess;
		SetEvent(EventA);
		ReleaseSemaphore(hSemaphore, 1, NULL);
		Sleep(500);
	}

	CloseHandle(hSemaphore);

	return 0;
}
