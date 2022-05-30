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
		if (hSemaphore == NULL)
		{
			cout << "Create semaphore failed. Press any key to exit." << endl;
			cin.get();
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
			cout << "Create thread failed. Press any key to exit." << endl;
			cin.get();

			DeleteCriticalSection(&cs);
			for (int k = N - 1; k >= 0; k--) {
				CloseHandle(forks[k]);
			}
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
}
