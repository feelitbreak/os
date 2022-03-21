//Кендысь Алексей, 2 курс, 9 группа. Лабораторная №1

#include <windows.h>
#include <iostream>
#include <ctime>
#include <process.h>
using namespace std;

CRITICAL_SECTION cs;
HANDLE Event1, Event2;

struct Mass {
	int n;
	char* A;
	int k;
	Mass(int n, char* A, int k) {
		this->n = n;
		this->A = new char[n];
		for (int i = 0; i < n; i++) {
			this->A[i] = A[i];
		}
		this->k = k;
	}
};

DWORD WINAPI work(LPVOID par)
{
	cout << "Input sleep time for each processed element." << endl;
	int sleep;
	cin >> sleep;
	Mass* m = (Mass*)par;

	for (int i = 1; i < m->k; i++) {
		for (int j = i; j > 0 && m->A[j - 1] > m->A[j]; j--)
			swap(m->A[j - 1], m->A[j]);
		Sleep(sleep);
	}

	cout << "Sorted mass (from work): ";
	for (int i = 0; i < m->n; i++)
	{
		cout << m->A[i] << " ";
	}
	cout << endl;

	SetEvent(Event1);
	return 0;
}

DWORD WINAPI SumElement(LPVOID par)
{
	/*Mass* m = (Mass*)par;
	int n = m->n;
	char* A = m->A;
	int k = m->k;
	int count = 0;
	cout << "Массив:" << endl;
	for (int i = 0; i < n; i++)
	{
		cout << A[i] << endl;
		if (A[i]) {
			count++;
		}

		Sleep(12);
	}
	cout << "Количество ненулевых элементов = " << count << endl; */
	return 0;
}

int main()
{
	setlocale(LC_ALL, "rus");

	HANDLE	hThreadWork;
	DWORD IDThreadWork;
	HANDLE	hThreadSum;
	DWORD IDThreadSum;
	int n;
	char* A;
	int k;

	InitializeCriticalSection(&cs);
	Event1 = CreateEvent(NULL, FALSE, FALSE, NULL);
	if (Event1 == NULL)
		return GetLastError();
	Event2 = CreateEvent(NULL, FALSE, FALSE, NULL);
	if (Event2 == NULL)
		return GetLastError();

	cout << "Input the size of your mass." << endl;
	cin >> n;
	A = new char[n];
	/*srand((int)time(NULL));
	for (int i = 0; i < n; i++) {
		A[i] = (int)rand() % 20 - 10;
	}*/
	cout << "Input the elements of your mass." << endl;
	for (int i = 0; i < n; i++)
	cin >> A[i];

	cout << "Size = " << n << endl;
	cout << "Mass elements: ";
	for (int i = 0; i < n; i++)
	{
		cout << A[i] << " ";
	}

	cout << endl;

	cout << "Input k." << endl;
	cin >> k;

	Mass* mass = new Mass(n, A, k);
	hThreadWork = CreateThread(NULL, 0, work, (void*)mass, 0, &IDThreadWork);
	if (hThreadWork == NULL)
		return GetLastError();

	hThreadSum = CreateThread(NULL, 0, SumElement, (void*)mass, 0, &IDThreadSum);
	if (hThreadSum == NULL)
		return GetLastError();

	WaitForSingleObject(Event1, INFINITE);
	cout << "First k sorted elements (from main): ";
	for (int i = 0; i < mass->k; i++)
	{
		cout << mass->A[i] << " ";
	}
	cout << endl;

	delete[] A;

	WaitForSingleObject(hThreadWork, INFINITE);
	WaitForSingleObject(hThreadSum, INFINITE);

	DeleteCriticalSection(&cs);
	CloseHandle(Event1);
	CloseHandle(Event2);
	CloseHandle(hThreadWork);
	CloseHandle(hThreadSum);

	return 0;
}