//Кендысь Алексей, 2 курс, 9 группа. Лабораторная №4

#include <windows.h>
#include <iostream>

using namespace std;


HANDLE EventA, EventB, EventEndChild, EventEndParent;

int main()
{
	EventA = CreateEvent(NULL, FALSE, FALSE, NULL);
	if (EventA == NULL)
		return GetLastError();
	EventB = CreateEvent(NULL, FALSE, FALSE, NULL);
	if (EventB == NULL)
		return GetLastError();
	EventEndChild = CreateEvent(NULL, FALSE, FALSE, NULL);
	if (EventEndChild == NULL)
		return GetLastError();
	EventEndParent = CreateEvent(NULL, FALSE, FALSE, NULL);
	if (EventEndParent == NULL)
		return GetLastError();

	HANDLE hSemaphore;
	hSemaphore = CreateSemaphore(NULL, 0, 10, "ParentSemaphore");
	if (hSemaphore == NULL)
		return GetLastError();

	HANDLE	hMutex;
	hMutex = CreateMutex(NULL, FALSE, "ChildMutex");
	if (hMutex == NULL)
	{
		cout << "Create mutex failed." << endl;
		cout << "Press any key to exit." << endl;
		cin.get();

		return GetLastError();
	}

	int nPr1, nPr2;
	cout << "Input the amount of Parent processes." << endl;
	cin >> nPr1;
	cout << "Input the amount of Child processes." << endl;
	cin >> nPr2;

	int nMes1, nMes2;
	cout << "Input the amount of Parent messages." << endl;
	cin >> nMes1;
	cout << "Input the amount of Child messages." << endl;
	cin >> nMes2;

	char	lpszAppNameParent[] = "Parent.exe";
	char	lpszAppNameChild[] = "Child.exe";
	STARTUPINFO	siChild;
	STARTUPINFO	siParent;
	PROCESS_INFORMATION	piChild;
	PROCESS_INFORMATION	piParent;

	ZeroMemory(&siChild, sizeof(STARTUPINFO));
	siChild.cb = sizeof(STARTUPINFO);

	ZeroMemory(&siParent, sizeof(STARTUPINFO));
	siParent.cb = sizeof(STARTUPINFO);

	if (!CreateProcess(lpszAppNameChild, NULL, NULL, NULL, FALSE,
		NULL, NULL, NULL, &siChild, &piChild))
	{
		cout << "The new process Child is not created." << endl;
		cout << "Press any key to exit." << endl;
		cin.get();

		return GetLastError();
	}

	if (!CreateProcess(lpszAppNameParent, NULL, NULL, NULL, FALSE,
		NULL, NULL, NULL, &siParent, &piParent))
	{
		cout << "The new process Parent is not created." << endl;
		cout << "Press any key to exit." << endl;
		cin.get();

		return GetLastError();
	}

	// выводим на экран строки
	for (int j = 0; j < 10; j++)
	{
		// захватываем мьютекс
		WaitForSingleObject(hMutex, INFINITE);
		for (int i = 0; i < 10; i++)
		{
			cout << j << ' ';
			cout.flush();
			Sleep(10);
		}
		cout << endl;
		// освобождаем мьютекс
		ReleaseMutex(hMutex);
	}

	CloseHandle(hSemaphore);
	CloseHandle(hMutex);

	WaitForSingleObject(piChild.hProcess, INFINITE);
	WaitForSingleObject(piParent.hProcess, INFINITE);

	CloseHandle(piChild.hThread);
	CloseHandle(piChild.hProcess);

	CloseHandle(piParent.hThread);
	CloseHandle(piParent.hProcess);

	CloseHandle(EventA);
	CloseHandle(EventB);
	CloseHandle(EventEndChild);
	CloseHandle(EventEndParent);

	return 0;
}