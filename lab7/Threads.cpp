#include "Header.h"
DWORD WINAPI producer(LPVOID par) {
	ElementsToProduce* elems = (ElementsToProduce*) par;
	int n = elems->nElem;
	short* mass = elems->massElem;
	MonitorStack &stack = elems->stack;

	for (int i = 0; i < n; i++) {
		stack.Push(mass[i]);
		cout << "Element " << mass[i] << " has been produced." << endl;
	}
	return 0;
}