//������� �������, 2 ����, 9 ������. ������������ �1

#include <windows.h>
#include <iostream>
#include <ctime>
#include <process.h>
using namespace std;

volatile int n;

struct Mass {
	int n;
	int* A;
	int end;
	Mass(int n, int* A, int end) {
		this->n = n;
		this->A = new int[n];
		for (int i = 0; i < n; i++) {
			this->A[i] = A[i];
		}
		this->end = end;
	}
};

UINT WINAPI worker(LPVOID par)
{
	Mass* m = (Mass*)par;
	int n = m->n;
	int* A = m->A;
	long end = m->end;
	int count = 0;
	cout << "������:" << endl;
	for (int i = 0; i < n; i++)
	{
		cout << A[i] << endl;
		if (A[i]) {
			count++;
		}

		Sleep(12);
	}
	Sleep(end);
	cout << "���������� ��������� ��������� = " << count << endl;
	return 0;
}

int main()
{
	int	inc = 10;
	HANDLE	hThread;
	UINT IDThread;
	int n;
	int* A;


	setlocale(LC_ALL, "rus");

	cout << "������� ������ �������." << endl;
	cin >> n;
	A = new int[n];
	srand((int)time(NULL));
	for (int i = 0; i < n; i++) {
		A[i] = (int)rand() % 20 - 10;
	}
	/*cout << "������� �������� �������." << endl;
	for (int i = 0; i < n; i++)
	cin >> A[i];*/

	int start, end;
	cout << "������� ����� ��� ��������� ������." << endl;
	cin >> end;
	cout << "������� ����� ��� ������� ������." << endl;
	cin >> start;

	Mass* mass = new Mass(n, A, end);
	/*hThread = CreateThread(NULL, 0, worker, (void*)mass, 0, &IDThread);
	if (hThread == NULL)
		return GetLastError();*/
	hThread = (HANDLE)
		_beginthreadex(NULL, 0, worker, (void*)mass, 0, &IDThread);
	if (hThread == NULL)
		return GetLastError();

	SuspendThread(hThread);
	Sleep(start);
	ResumeThread(hThread);


	delete[] A;

	WaitForSingleObject(hThread, INFINITE);

	CloseHandle(hThread);

	return 0;
}