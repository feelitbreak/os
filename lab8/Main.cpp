//Кендысь Алексей, 2 курс, 9 группа. Лабораторная №8. Задача "Обедающие философы".

#include "Header.h"

HANDLE forks[N];
HANDLE hMutex;

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

	hMutex = CreateMutex(NULL, FALSE, NULL);
	if (hMutex == NULL)
	{
		cout << "Create mutex failed. Press any key to exit." << endl;
		cin.get();

		for (int k = N - 1; k >= 0; k--) {
			CloseHandle(forks[k]);
		}
		return GetLastError();
	}

	for (int i = 0; i < N; i++)
	{
		Singleton* par = new Singleton(i);
		philosophers[i] = CreateThread(NULL, 0, philosopher, (void*) par, 0, &IDPhilosopher);
		if (philosophers[i] == NULL)
		{
			cout << "Create thread failed. Press any key to exit." << endl;
			cin.get();

			CloseHandle(hMutex);
			for (int k = N - 1; k >= 0; k--) {
				CloseHandle(forks[k]);
			}
			return GetLastError();
		}
	}

	Sleep(INFINITE);
}
