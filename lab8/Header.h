#include <windows.h>
#include <iostream>
#include <process.h>

using std::cout;
using std::cin;
using std::endl;

static int const N = 5;
static int const ThinkingTime = 300;
static int const EatingTime = 250;

HANDLE forks[N];

DWORD WINAPI philosopher(LPVOID par);

void think(int i);

void eat(int i);