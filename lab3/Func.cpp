#include "Header.h"

double countAvrg(int k, char* A)
{
	double avrg = 0;
	for (int i = 0; i < k; i++)
	{
		avrg += A[i];
	}
	avrg /= k;
	return avrg;
}

void outMass(int start, int n, char* A)
{
	for (int i = start; i < n; i++)
	{
		cout << A[i] << " ";
	}
	cout << endl;
}

void outRes(double avrg, int k, int n, char* A) {
	cout << "Result average = " << static_cast<int>(avrg) << endl;
	cout << "Result mass from k position: ";
	outMass(k, n, A);
}

Mass* input() {
	int n;
	char* A;
	int k;

	cout << "Input the size of your mass." << endl;
	cin >> n;

	A = new char[n];

	cout << "Input the elements of your mass." << endl;
	for (int i = 0; i < n; i++)
		cin >> A[i];

	cout << "Your mass:" << endl;
	outMass(0, n, A);

	cout << "Input k." << endl;
	cin >> k;

	Mass* mass = new Mass(n, A, k);

	delete[] A;
	return mass;
}

void sort(int n, char* A, long sleep)
{
	for (int i = 1; i < n; i++) {
		for (int j = i; j > 0 && A[j - 1] > A[j]; j--)
			swap(A[j - 1], A[j]);
		Sleep(sleep);
	}
}