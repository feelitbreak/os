#include <windows.h>
#include <iostream>
#include <ctime>
using namespace std;

volatile int n;

struct Mass {
	int n;
	int* A;
	long end;
	Mass(int n, int* A, long end) {
		this->n = n;
		this->A = new int[n];
		for (int i = 0; i < n; i++) {
			this->A[i] = A[i];
		}
		this->end = end;
	}
};

DWORD WINAPI worker(LPVOID par)
{
	Mass* m = (Mass*)par;
	int n = m->n;
	int* A = m->A;
	long end = m->end;
	int count = 0;
	cout << "Массив:" << endl;
	for (int i = 0; i < n; i++)
	{
		cout << A[i] << endl;
		if (A[i]) {
			count++;
		}
		// приостанавливаем поток на 12 миллисекунд
		Sleep(12);
	}
	Sleep(end);
	cout << "Количество ненулевых элементов = " << count << endl;
	return 0;
}

int main()
{
	int	inc = 10;
	HANDLE	hThread;
	DWORD	IDThread;
	int n;
	int* A;


	setlocale(LC_ALL, "rus");

	cout << "Введите размер массива." << endl;
	cin >> n;
	A = new int[n];
	srand((int)time(NULL));
	for (int i = 0; i < n; i++) {
		A[i] = (int)rand() % 20 - 10;
	}
	/*cout << "Введите элементы массива." << endl;
	for (int i = 0; i < n; i++)
	cin >> A[i];*/

	long start, end;
	cout << "Введите время для остановки потока." << endl;
	cin >> end;
	cout << "Введите время для запуска потока." << endl;
	cin >> start;

	Mass* mass = new Mass(n, A, end);
	hThread = CreateThread(NULL, 0, worker, (void*)mass, 0, &IDThread);
	if (hThread == NULL)
		return GetLastError();

	SuspendThread(hThread);
	Sleep(start);
	ResumeThread(hThread);


	delete[] A;
	// ждем пока поток Add закончит работу
	WaitForSingleObject(hThread, INFINITE);
	// закрываем дескриптор потока
	CloseHandle(hThread);

	return 0;
}