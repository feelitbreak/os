//Кендысь Алексей, 2 курс, 9 группа. Лабораторная №3

#include "Header.h"

CRITICAL_SECTION cs;
HANDLE Event1, Event2;

DWORD WINAPI work(LPVOID par)
{
	long sleep;

	cout << "Input sleep time for each processed element." << endl;
	cin >> sleep;

	Mass* m = (Mass*)par;

	sort(m->n, m->A, sleep);

	cout << "Sorted mass (from work): ";
	outMass(0, m->n, m->A);

	SetEvent(Event1);

	return 0;
}

DWORD WINAPI SumElement(LPVOID par)
{
	EnterCriticalSection(&cs);
	WaitForSingleObject(Event2, INFINITE);

	Mass* m = (Mass*)par;
	m->avrg = countAvrg(m->k, m->A);

	LeaveCriticalSection(&cs);

	return 0;
}

int main()
{
	HANDLE	hThreadWork;
	HANDLE hThreadSum;
	DWORD IDThreadWork;
	DWORD IDThreadSum;

	InitializeCriticalSection(&cs);
	Event1 = CreateEvent(NULL, FALSE, FALSE, NULL);
	if (NULL == Event1)
	{
		DeleteCriticalSection(&cs);

		cout << "Error. Couldn't create event." << endl;
		system("pause");
		return GetLastError();
	}
	Event2 = CreateEvent(NULL, FALSE, FALSE, NULL);
	if (NULL == Event2)
	{
		CloseHandle(Event1);
		DeleteCriticalSection(&cs);

		cout << "Error. Couldn't create event." << endl;
		system("pause");
		return GetLastError();
	}

	Mass* mass = input();

	hThreadWork = CreateThread(NULL, 0, work, (void*)mass, 0, &IDThreadWork);
	if (NULL == hThreadWork)
	{
		CloseHandle(Event2);
		CloseHandle(Event1);
		DeleteCriticalSection(&cs);

		cout << "Error. Couldn't create thread." << endl;
		system("pause");
		return GetLastError();
	}

	hThreadSum = CreateThread(NULL, 0, SumElement, (void*)mass, 0, &IDThreadSum);
	if (NULL == hThreadSum) 
	{
		CloseHandle(hThreadWork);
		CloseHandle(Event2);
		CloseHandle(Event1);
		DeleteCriticalSection(&cs);

		cout << "Error. Couldn't create thread." << endl;
		system("pause");
		return GetLastError();
	}

	WaitForSingleObject(Event1, INFINITE);
	cout << "Sorted mass up until k (from main): ";
	outMass(0, mass->k, mass->A);
	SetEvent(Event2);

	EnterCriticalSection(&cs);
	outRes(mass->avrg, mass->k, mass->n, mass->A);
	LeaveCriticalSection(&cs);

	HANDLE handles[2] = { hThreadWork, hThreadSum };
	WaitForMultipleObjects(2, handles, TRUE, INFINITE);

	CloseHandle(hThreadSum);
	CloseHandle(hThreadWork);
	CloseHandle(Event2);
	CloseHandle(Event1);
	DeleteCriticalSection(&cs);

	return 0;
}