#include <windows.h>
#include <iostream>

using namespace std;

int main(int argc, char* argv[])
{
	int n = atoi(argv[1]);

	HANDLE hMutex;

	hMutex = OpenMutex(SYNCHRONIZE, FALSE, "ChildMutex");
	if (hMutex == NULL)
	{
		cout << "Open mutex failed." << endl;
		cout << "Press any key to exit." << endl;
		cin.get();

		return GetLastError();
	}

	HANDLE EventB = OpenEvent(EVENT_MODIFY_STATE, FALSE, "B");
	if (EventB == NULL)
		return GetLastError();
	HANDLE EventEndChild = OpenEvent(SYNCHRONIZE, FALSE, "EndChild");
	if (EventEndChild == NULL)
		return GetLastError();

	WaitForSingleObject(hMutex, INFINITE);
	cout << "I am active.\n";
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

	CloseHandle(hMutex);
	CloseHandle(EventEndChild);

	return 0;
}