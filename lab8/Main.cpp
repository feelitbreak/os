//Кендысь Алексей, 2 курс, 9 группа. Лабораторная №8. Задача "Обедающие философы".

#include "Header.h"

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

	for (int i = 0; i < N; i++)
	{
		philosophers[i] = CreateThread(NULL, 0, philosopher, (void*) i, 0, &IDPhilosopher);
		if (philosophers[i] == NULL)
		{
			cout << "Create thread failed. Press any key to exit." << endl;
			cin.get();

			for (int j = N - 1; j >= 0; j--) {
				CloseHandle(forks[j]);
			}
			return GetLastError();
		}
	}

	
	for (int i = N - 1; i >= 0; i--) {
		CloseHandle(philosophers[i]);
	}
	for (int i = N - 1; i >= 0; i--) {
		CloseHandle(forks[i]);
	}
}
