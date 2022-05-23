#include "Header.h"

MonitorStack::MonitorStack(int nSize) {
	size = nSize;
	mass = new short[nSize];
	n = 0;
};

MonitorStack::~MonitorStack() {
	delete[] mass;
};

void MonitorStack::Push(short& nElement) {
	if (n < size) {
		mass[n] = nElement;
		n++;
	}
}

short MonitorStack::Pop() {
	if (n > 0) {
		n--;
		return mass[n];
	}
	return 0;
}