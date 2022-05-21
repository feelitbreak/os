#include "Header.h"
MonitorStack::MonitorStack(int nSize) {
	size = nSize;
	mass = new short[nSize];
	n = 0;
};

MonitorStack::~MonitorStack() {
	delete[] mass;
};

MonitorStack::MonitorStack(int nSize) {
	size = nSize;
	mass = new short[nSize];
	n = 0;
};

void MonitorStack::Push(short& nElement) {
	if (n < size) {
		mass[n] = nElement;
		n++;
	}
}

char MonitorStack::Pop() {
	if (n > 0) {
		n--;
		return mass[n];
	}
}