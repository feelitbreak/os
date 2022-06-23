#include <windows.h>
#include <iostream>

using std::cin;
using std::cout;
using std::endl;

int main(int argc, char* argv[])
{
	int n = atoi(argv[1]);

	HANDLE hMutex;

	hMutex = OpenMutex(SYNCHRONIZE, FALSE, "ChildMutex");
	if (NULL == hMutex)
	{
		cout << "Error. Failed to open mutex." << endl;
		system("pause");
		return GetLastError();
	}

	HANDLE EventB = OpenEvent(EVENT_MODIFY_STATE, FALSE, "B");
	if (NULL == EventB)
	{
		CloseHandle(hMutex);

		cout << "Error. Failed to open event." << endl;
		system("pause");
		return GetLastError();
	}
	HANDLE EventEndChild = OpenEvent(SYNCHRONIZE, FALSE, "EndChild");
	if (NULL == EventEndChild)
	{
		CloseHandle(EventB);
		CloseHandle(hMutex);

		cout << "Error. Failed to open event." << endl;
		system("pause");
		return GetLastError();
	}

	WaitForSingleObject(hMutex, INFINITE);

	cout << "I am active." << endl;

	char mes;
	for (int i = 0; i < n; i++) {
		cout << "Input message." << endl;
		cin >> mes;
		if (mes == 'B')
		{
			SetEvent(EventB);
		} 
		else 
		{
			i--;
		}
	}

	ReleaseMutex(hMutex);
	cout << "Waiting for other processes.\n";
	
	WaitForSingleObject(EventEndChild, INFINITE);

	CloseHandle(EventEndChild);
	CloseHandle(EventB);
	CloseHandle(hMutex);

	return 0;
}