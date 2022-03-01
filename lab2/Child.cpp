#include <conio.h>
#include <iostream>
using namespace std;
int main(int argc, char* argv[])
{
	int n = atoi(argv[1]);
	char* A = new char[n];
	for (int i = 2; i < n + 2; i++) {
		A[i - 2] = argv[i][0];
	}
	_cputs("I am created.");

	_cputs("\nMy name is: ");
	_cputs(argv[0]);

	_cputs("\nYour mass input:\n");

	for (int i = 0; i < n; i++) {
		_cprintf("%c ", A[i]);
	}

	for (int i = 1; i < n; i++)
		for (int j = i; j > 0 && A[j - 1] > A[j]; j--)
			swap(A[j - 1], A[j]);

	_cputs("\nResult mass:\n");
	for (int i = 0; i < n; i++) {
		_cprintf("%c ", A[i]);
	}

	delete[] A;

	_cputs("\nPress any key to finish.\n");
	_getch();

	return 0;
}