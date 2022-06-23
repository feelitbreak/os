#include <windows.h>
#include <iostream>
#include <ctime>

using std::cin;
using std::cout;
using std::endl;
using std::swap;

extern CRITICAL_SECTION cs;
extern HANDLE Event1, Event2;

struct Mass {
	int n;
	char* A;
	int k;
	double avrg;

	Mass(int n, char* A, int k) {
		this->n = n;
		this->A = new char[n];
		for (int i = 0; i < n; i++) {
			this->A[i] = A[i];
		}
		this->k = k;
		this->avrg = 0;
	}

	~Mass() {
		delete[] A;
	}
};

Mass* input();
double countAvrg(int k, char* A);
void outMass(int start, int n, char* A);
void outRes(int avrg, int k, int n, char* A);

void sort(int n, char* A, long sleep);

DWORD WINAPI work(LPVOID par);
DWORD WINAPI SumElement(LPVOID par);