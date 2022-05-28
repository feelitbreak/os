
#include "Header.h"

DWORD WINAPI philosopher(LPVOID par) {
	int i = ((Singleton*) par)->i;

	while (1) {
		think(i);
		takeForks(i);
		eat(i);
		putForks(i);
	}
}

void think(int i) {
	cout << "Philosopher" << i + 1 << " is thinking." << endl;
	Sleep(ThinkingTime);
}

void eat(int i) {
	cout << "Philosopher" << i + 1 << " is eating." << endl;
	Sleep(EatingTime);
}

void takeForks(int i) {
	int leftFork = i;
	int rightFork = (i + 1) % N;
	HANDLE iForks[2] = { forks[leftFork], forks[rightFork] };
	WaitForMultipleObjects(2, iForks, TRUE, INFINITE);
}

void putForks(int i) {
	int leftFork = i;
	int rightFork = (i + 1) % N;
	ReleaseSemaphore(forks[leftFork], 1, NULL);
	ReleaseSemaphore(forks[rightFork], 1, NULL);
}