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
	char Pop();
};

class ElementsToProduce
{
public:
	short* massElem;
	int nElem;
	MonitorStack& stack;

	ElementsToProduce(int n, short* mass, MonitorStack& _stack) : stack(_stack)
	{
		nElem = n;
		massElem = mass;
	}

};

DWORD WINAPI producer(LPVOID par);