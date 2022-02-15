// ������ �������� ������ �������� CreateThread

#include <windows.h>
#include <iostream>
#include <ctime>
using namespace std;

volatile int n;

struct Mass {
	int n;
	int *A;
	Mass(int n, int* A) {
		this->n = n;
		this->A = A;
	}
};

DWORD WINAPI worker(LPVOID iNum)
{
	Mass *m = (Mass*) iNum;
	int n = m->n;
	int* A = m->A;
	for (int i = 0; i < n; i++)
	{

		cout << A[i] << endl;
		// ���������������� ����� �� 12 �����������
		Sleep(12);
	}

	return 0;
}

int main()
{
	int	inc = 10;
	HANDLE	hThread;
	DWORD	IDThread;
	int n;
	int *A;


	setlocale(LC_ALL, "rus");

	cout << "������� ������ �������." << endl;
	cin >> n;
	A = new int[n];
	srand((int)time(NULL));
	for (int i = 0; i < n; i++) {
		A[i] = (int)rand();
	}
	Mass* mass = new Mass(n, A);

	hThread = CreateThread(NULL, 0, worker, (void*)mass, 0, &IDThread);
	if (hThread == NULL)
		return GetLastError();

	

	delete[] A;
	// ���� ���� ����� Add �������� ������
	WaitForSingleObject(hThread, INFINITE);
	// ��������� ���������� ������
	CloseHandle(hThread);

	return 0;
}