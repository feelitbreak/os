//Кендысь Алексей, 2 курс, 9 группа. Лабораторная №8. Задача "Обедающие философы".

#include "Header.h"

HANDLE forks[N];
CRITICAL_SECTION cs;

int main()
{
	DWORD IDPhilosopher;
	HANDLE hSemaphore;
	HANDLE philosophers[N];

	for (int i = 0; i < N; i++) {
		hSemaphore = CreateSemaphore(NULL, 1, 1, NULL);
		if (NULL == hSemaphore == NULL)
		{
			for (int j = i - 1; j >= 0; j--) {
				CloseHandle(forks[j]);
			}

			cout << "Create semaphore failed." << endl;
			system("pause");
			return GetLastError();
		}
		forks[i] = hSemaphore;
	}

	InitializeCriticalSection(&cs);

	for (int i = 0; i < N; i++)
	{
		Singleton* par = new Singleton(i);
		philosophers[i] = CreateThread(NULL, 0, philosopher, (void*) par, 0, &IDPhilosopher);
		if (philosophers[i] == NULL)
		{
			for (int j = i - 1; j >= 0; j--) {
				CloseHandle(philosophers[j]);
			}
			DeleteCriticalSection(&cs);
			for (int j = N - 1; j >= 0; j--) {
				CloseHandle(forks[j]);
			}

			cout << "Create thread failed." << endl;
			system("pause");
			return GetLastError();
		}
	}

	Sleep(OverallTime);

	for (int i = N - 1; i >= 0; i--) {
		CloseHandle(philosophers[i]);
	}
	DeleteCriticalSection(&cs);
	for (int i = N - 1; i >= 0; i--) {
		CloseHandle(forks[i]);
	}

	system("pause");
	return 0;
}
