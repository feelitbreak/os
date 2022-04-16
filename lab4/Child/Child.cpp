#include <windows.h>
#include <iostream>

using namespace std;

int main(int argc, char* argv[])
{
	int n = atoi(argv[1]);

	HANDLE	hMutex;

	// открываем мьютекс
	hMutex = OpenMutex(SYNCHRONIZE, FALSE, "ChildMutex");
	if (hMutex == NULL)
	{
		cout << "Open mutex failed." << endl;
		cout << "Press any key to exit." << endl;
		cin.get();

		return GetLastError();
	}

	HANDLE EventB = OpenEvent(SYNCHRONIZE, FALSE, "B");
	if (EventB == NULL)
		return GetLastError();
	HANDLE EventEndChild = OpenEvent(SYNCHRONIZE, FALSE, "EndChild");
	if (EventEndChild == NULL)
		return GetLastError();
	WaitForSingleObject(hMutex, INFINITE);
	char mes;
	int i = 0;
	while (i < n)
	{
		cout << "Input message." << endl;
		cin >> mes;
		if (mes == 'B')
		{
			i++;
			SetEvent(EventB);
		}

		Sleep(500);
	}
	ReleaseMutex(hMutex);
	
	WaitForSingleObject(EventEndChild, INFINITE);

	CloseHandle(hMutex);

	return 0;
}