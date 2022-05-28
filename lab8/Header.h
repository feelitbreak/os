#include <windows.h>
#include <iostream>
#include <process.h>

using std::cout;
using std::cin;
using std::endl;

static int const N = 5;
static int const ThinkingTime = 300;
static int const EatingTime = 250;

extern HANDLE forks[N];
extern HANDLE hMutex;

struct Singleton {
	int i;

	Singleton(int i) 
	{
		this->i = i;
	}
};

DWORD WINAPI philosopher(LPVOID par);

void think(int i);
void eat(int i);
void takeForks(int i);
void putForks(int i);