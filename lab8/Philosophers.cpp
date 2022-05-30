#include "Header.h"

DWORD WINAPI philosopher(LPVOID par) {
	int i = ((Singleton*) par)->i;

	while (1) {
		if (i != N - 1)
		{
			takeForks(i);
		}
		else
		{
			takeForksReverse(i);
		}
		eat(i);
		putForks(i);
		think(i);
	}
}

void think(int i) {
	EnterCriticalSection(&cs);
	cout << "Philosopher" << i + 1 << " is thinking." << endl;
	LeaveCriticalSection(&cs);
	Sleep(ThinkingTime);
}

void eat(int i) {
	EnterCriticalSection(&cs);
	cout << "Philosopher" << i + 1 << " is eating." << endl;
	LeaveCriticalSection(&cs);
	Sleep(EatingTime);
}

void takeForks(int i) {
	int leftFork = i;
	int rightFork = (i + 1) % N;

	WaitForSingleObject(forks[leftFork], INFINITE);

	Sleep(TimeToSeparate);

	WaitForSingleObject(forks[rightFork], INFINITE);
}

void takeForksReverse(int i) {
	int leftFork = i;
	int rightFork = (i + 1) % N;

	WaitForSingleObject(forks[rightFork], INFINITE);

	Sleep(TimeToSeparate);

	WaitForSingleObject(forks[leftFork], INFINITE);
}

void putForks(int i) {
	int leftFork = i;
	int rightFork = (i + 1) % N;

	ReleaseSemaphore(forks[leftFork], 1, NULL);
	ReleaseSemaphore(forks[rightFork], 1, NULL);
}