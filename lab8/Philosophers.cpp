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
	WaitForSingleObject(hMutex, INFINITE);
	cout << "Philosopher" << i + 1 << " is thinking." << endl;
	ReleaseMutex(hMutex);
	Sleep(ThinkingTime);
}

void eat(int i) {
	WaitForSingleObject(hMutex, INFINITE);
	cout << "Philosopher" << i + 1 << " is eating." << endl;
	ReleaseMutex(hMutex);
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