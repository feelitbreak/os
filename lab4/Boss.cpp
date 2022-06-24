//Кендысь Алексей, 2 курс, 9 группа. Лабораторная №4

#include <windows.h>
#include <iostream>

using std::cin;
using std::cout;
using std::endl;

static const int maxLength = 255;
static const int radix = 10;
static const int initCount = 1;
static const int maxCount = 1;

HANDLE EventA, EventB, EventEndChild, EventEndParent;

char* formCommandLine(char* name, int nMes, char* buff) 
{
	char* commandLine = new char[maxLength];
	strcpy_s(commandLine, maxLength, name);
	_itoa_s(nMes, buff, maxLength, radix);
	buff[maxLength - 1] = '\0';
	strcat_s(commandLine, maxLength, buff);
	return commandLine;
}

int main()
{
	EventA = CreateEvent(NULL, FALSE, FALSE, "A");
	if (NULL == EventA)
	{
		cout << "Error. Couldn't create event." << endl;
		system("pause");
		return GetLastError();
	}
	EventB = CreateEvent(NULL, FALSE, FALSE, "B");
	if (NULL == EventB)
	{
		CloseHandle(EventA);

		cout << "Error. Couldn't create event." << endl;
		system("pause");
		return GetLastError();
	}
	EventEndChild = CreateEvent(NULL, TRUE, FALSE, "EndChild");
	if (NULL == EventEndChild)
	{
		CloseHandle(EventB);
		CloseHandle(EventA);

		cout << "Error. Couldn't create event." << endl;
		system("pause");
		return GetLastError();
	}
	EventEndParent = CreateEvent(NULL, TRUE, FALSE, "EndParent");
	if (NULL == EventEndParent)
	{
		CloseHandle(EventEndChild);
		CloseHandle(EventB);
		CloseHandle(EventA);

		cout << "Error. Couldn't create event." << endl;
		system("pause");
		return GetLastError();
	}

	HANDLE hSemaphore;
	hSemaphore = CreateSemaphore(NULL, initCount, maxCount, "ParentSemaphore");
	if (NULL == hSemaphore)
	{
		CloseHandle(EventEndParent);
		CloseHandle(EventEndChild);
		CloseHandle(EventB);
		CloseHandle(EventA);

		cout << "Error. Couldn't create semaphore." << endl;
		system("pause");
		return GetLastError();
	}

	HANDLE	hMutex;
	hMutex = CreateMutex(NULL, FALSE, "ChildMutex");
	if (NULL == hMutex)
	{
		CloseHandle(hSemaphore);
		CloseHandle(EventEndParent);
		CloseHandle(EventEndChild);
		CloseHandle(EventB);
		CloseHandle(EventA);

		cout << "Error. Couldn't create mutex." << endl;
		system("pause");
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

	char buff[maxLength];
	char* ParentLine = formCommandLine(const_cast <char*>("Parent.exe "), nMes1, buff);
	char* ChildLine = formCommandLine(const_cast <char*>("Child.exe "), nMes2, buff);

	int n = nPr1 + nPr2;
	HANDLE* hMassProcess = new HANDLE[n];
	HANDLE* hMassThread = new HANDLE[n];

	for (int i = 0; i < nPr1 && i < n; i++) 
	{
		STARTUPINFO	siParent;
		PROCESS_INFORMATION	piParent;

		ZeroMemory(&siParent, sizeof(STARTUPINFO));
		siParent.cb = sizeof(STARTUPINFO);

		if (NULL == CreateProcess(NULL, ParentLine, NULL, NULL, FALSE,
			CREATE_NEW_CONSOLE, NULL, NULL, &siParent, &piParent))
		{
			for (int j = i - 1; j >= 0; j--)
			{
				CloseHandle(hMassThread[j]);
				CloseHandle(hMassProcess[j]);
			}

			delete[] hMassThread;
			delete[] hMassProcess;
			delete[] ChildLine;
			delete[] ParentLine;
			CloseHandle(hMutex);
			CloseHandle(hSemaphore);
			CloseHandle(EventEndParent);
			CloseHandle(EventEndChild);
			CloseHandle(EventB);
			CloseHandle(EventA);

			cout << "Error. Failed to create Parent process." << endl;
			system("pause");
			return GetLastError();
		}
		hMassProcess[i] = piParent.hProcess;
		hMassThread[i] = piParent.hThread;
	}

	for (int i = nPr1; i < n; i++)
	{
		STARTUPINFO	siChild;
		PROCESS_INFORMATION	piChild;
		ZeroMemory(&siChild, sizeof(STARTUPINFO));
		siChild.cb = sizeof(STARTUPINFO);
		if (NULL == CreateProcess(NULL, ChildLine, NULL, NULL, FALSE,
			CREATE_NEW_CONSOLE, NULL, NULL, &siChild, &piChild))
		{
			for (int j = i - 1; j >= 0; j--)
			{
				CloseHandle(hMassThread[j]);
				CloseHandle(hMassProcess[j]);
			}

			delete[] hMassThread;
			delete[] hMassProcess;
			delete[] ChildLine;
			delete[] ParentLine;
			CloseHandle(hMutex);
			CloseHandle(hSemaphore);
			CloseHandle(EventEndParent);
			CloseHandle(EventEndChild);
			CloseHandle(EventB);
			CloseHandle(EventA);

			cout << "Error. Failed to create Child process." << endl;
			system("pause");
			return GetLastError();
		}

		hMassProcess[i] = piChild.hProcess;
		hMassThread[i] = piChild.hThread;
	}

	HANDLE mass[] = {EventA, EventB};
	for(int i = 0; i < nMes1 * nPr1 + nMes2 * nPr2; i++)
	{
		int ind = WaitForMultipleObjects(2, mass, FALSE, INFINITE) - WAIT_OBJECT_0;
		if(ind == 0) 
		{
			cout << "Message A by Parent" << endl;
		}
		if(ind == 1)
		{
			cout << "Message B by Child" << endl;
		}
	}

	cout << "All messages have been sent. Closing the app." << endl;

	SetEvent(EventEndParent);
	SetEvent(EventEndChild);

	for (int i = n - 1; i >= 0; i--) 
	{
		CloseHandle(hMassThread[i]);
		CloseHandle(hMassProcess[i]);
	}
	
	delete[] hMassThread;
	delete[] hMassProcess;
	delete[] ChildLine;
	delete[] ParentLine;
	CloseHandle(hMutex);
	CloseHandle(hSemaphore);
	CloseHandle(EventEndParent);
	CloseHandle(EventEndChild);
	CloseHandle(EventB);
	CloseHandle(EventA);

	return 0;
}