//Кендысь Алексей, 2 курс, 9 группа. Лабораторная №1

#include <windows.h>
#include <iostream>
#include <ctime>
#include <process.h>

using std::cin;
using std::cout;
using std::endl;

static int const rangeOfArray = 20;
static int const numOfNegativeElem = 10;

struct Mass 
{
	int n;
	int* A;

	Mass(int n, int* A) 
	{
		this->n = n;
		this->A = new int[n];
		for (int i = 0; i < n; i++)
		{
			this->A[i] = A[i];
		}
	}

	~Mass()
	{
		delete[] A;
	}
};

DWORD WINAPI worker(LPVOID par)
{
	Mass* m = (Mass*)par;

	int count = 0;

	cout << "Array:" << endl;
	for (int i = 0; i < m->n; i++)
	{
		cout << m->A[i] << endl;
		if (m->A[i]) {
			count++;
		}
	}

	cout << "The number of non-zero elements = " << count << endl;
	return 0;
}

int main()
{
	HANDLE	hThread;
	DWORD IDThread;
	int n;
	int* A;


	cout << "Input the size of the array." << endl;
	cin >> n;

	A = new int[n];
	srand((int)time(NULL));
	for (int i = 0; i < n; i++) {
		A[i] = (int)rand() % rangeOfArray - numOfNegativeElem;
	}

	Mass* mass = new Mass(n, A);

	int start, end;
	cout << "Input the time for starting the thread." << endl;
	cin >> start;
	cout << "Input the time for suspending the thread." << endl;
	cin >> end;

	Sleep(start);
	hThread = CreateThread(NULL, 0, worker, (void*)mass, 0, &IDThread);
	if (NULL == hThread)
	{
		delete[] A;
		return GetLastError();
	}
	/*hThread = (HANDLE)_beginthreadex(NULL, 0, worker, (void*)mass, 0, &IDThread);
	if (NULL == hThread)
	{
		delete[] A;
		return GetLastError();
	}*/

	SuspendThread(hThread);
	Sleep(end);
	ResumeThread(hThread);

	WaitForSingleObject(hThread, INFINITE);

	CloseHandle(hThread);
	delete[] A;

	return 0;
}