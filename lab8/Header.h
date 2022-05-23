#include <windows.h>
#include <iostream>
#include <process.h>

using std::cout;
using std::cin;
using std::endl;

static int const N = 5;

HANDLE forks[N];

DWORD WINAPI philosopher(LPVOID par);