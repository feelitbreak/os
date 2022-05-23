//Кендысь Алексей, 2 курс, 9 группа. Лабораторная №8. Задача "Обедающие философы".

#include "Header.h"

int main()
{
	DWORD IDPhilosopher;
	HANDLE hSemaphore;
	HANDLE philosophers[N];

	for (int i = 0; i < N; i++) {
		hSemaphore = CreateSemaphore(NULL, 0, 1, NULL);
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
		philosophers[i] = CreateThread(NULL, 0, philosopher, NULL, 0, &IDPhilosopher);
		if (philosophers[i] == NULL)
		{
			cout << "Create thread failed. Press any key to exit." << endl;
			cin.get();
			return GetLastError();
		}
	}
}
