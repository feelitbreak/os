//Кендысь Алексей, 2 курс, 9 группа. Лабораторная №4
#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <iostream>

using namespace std;


HANDLE EventA, EventB, EventEndChild, EventEndParent;

int main()
{
	EventA = CreateEvent(NULL, FALSE, FALSE, "A");
	if (EventA == NULL)
		return GetLastError();
	EventB = CreateEvent(NULL, FALSE, FALSE, "B");
	if (EventB == NULL)
		return GetLastError();
	EventEndChild = CreateEvent(NULL, FALSE, FALSE, "EndChild");
	if (EventEndChild == NULL)
		return GetLastError();
	EventEndParent = CreateEvent(NULL, FALSE, FALSE, "EndParent");
	if (EventEndParent == NULL)
		return GetLastError();

	HANDLE hSemaphore;
	hSemaphore = CreateSemaphore(NULL, 1, 1, "ParentSemaphore");
	if (hSemaphore == NULL)
	{
		cout << "Create semaphore failed." << endl;
		cout << "Press any key to exit." << endl;
		cin.get();

		return GetLastError();
	}

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

	char	lpszAppNameParent[] = "Parent.exe ";
	char	lpszAppNameChild[] = "Child.exe ";
	char buff[256];
	HANDLE* hMass = new HANDLE[nPr1 + nPr2];

	for (int i = 0; i < nPr1; i++) 
	{
		STARTUPINFO	siParent;
		PROCESS_INFORMATION	piParent;
		ZeroMemory(&siParent, sizeof(STARTUPINFO));
		siParent.cb = sizeof(STARTUPINFO);
		if (!CreateProcess(NULL, strcat(lpszAppNameParent, _itoa(nMes1, buff, 10)), NULL, NULL, FALSE,
			CREATE_NEW_CONSOLE, NULL, NULL, &siParent, &piParent))
		{
			cout << "The new process Parent is not created." << endl;
			cout << "Press any key to exit." << endl;
			cin.get();

			return GetLastError();
		}
		
	}

	for (int i = 0; i < nPr2; i++)
	{
		STARTUPINFO	siChild;
		PROCESS_INFORMATION	piChild;
		ZeroMemory(&siChild, sizeof(STARTUPINFO));
		siChild.cb = sizeof(STARTUPINFO);
		if (!CreateProcess(NULL, strcat(lpszAppNameChild, _itoa(nMes2, buff, 10)), NULL, NULL, FALSE,
			NULL, NULL, NULL, &siChild, &piChild))
		{
			cout << "The new process Child is not created." << endl;
			cout << "Press any key to exit." << endl;
			cin.get();

			return GetLastError();
		}
	}

	HANDLE mass[] = { EventA, EventB};

	for(int i = 0; i < nMes1 + nMes2; i++)
	{
		int ind = WaitForMultipleObjects(2, mass, FALSE, INFINITE) - WAIT_OBJECT_0;
		if(ind == 0) 
		{
			cout << "Message A by Parent\n";
			ResetEvent(EventA);
		}
		if(ind == 1)
		{
			cout << "Message B by Child\n";
			ResetEvent(EventB);
		}
	}

	SetEvent(EventEndParent);
	SetEvent(EventEndChild);
	
	CloseHandle(hSemaphore);
	CloseHandle(hMutex);


	

	CloseHandle(EventA);
	CloseHandle(EventB);
	CloseHandle(EventEndChild);
	CloseHandle(EventEndParent);

	return 0;
}