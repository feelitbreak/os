#include "Header.h"

DWORD WINAPI philosopher(LPVOID par) {
	int i = *(int*)par;
	int right = (i + 1) % N;
	int left = (i + N - 1) % N;

	while (1) {
		think(i);

		eat(i);
	}
}

void think(int i) {
	cout << "Philosopher" << i + 1 << "is thinking." << endl;
	Sleep(ThinkingTime);
}

void eat(int i) {
	cout << "Philosopher" << i + 1 << "is eating." << endl;
	Sleep(EatingTime);
}