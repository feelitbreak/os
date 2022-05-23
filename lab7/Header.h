#include <windows.h>
#include <iostream>
#include <ctime>
#include <process.h>
using std::cin;
using std::cout;
using std::endl;

class MonitorStack
{
private:
	short* mass;
	int n;
	int size;
public:
	MonitorStack(int nSize);
	~MonitorStack();
	void Push(short& nElement);
	short Pop();
};

class Info
{
public:
	short* massElem;
	int nElem;
	MonitorStack& stack;

	Info(int n, MonitorStack& _stack) : stack(_stack)
	{
		nElem = n;
		massElem = NULL;
	}

	Info(int n, short* mass, MonitorStack& _stack) : stack(_stack)
	{
		nElem = n;
		massElem = mass;
	}
};

DWORD WINAPI producer(LPVOID par);

DWORD WINAPI consumer(LPVOID par);