#include <conio.h>

int main(int argc, char* argv[])
{
	int i;
	int n = (int)argv[1];
	char* A = new char[n];
	for (int i = 2; i < n + 2; i++) {
		A[i] = (char)argv[i];
	}
	_cputs("I am created.");

	_cputs("\nMy name is: ");
	_cputs(argv[0]);


	delete[] A;

	_cputs("\nPress any key to finish.\n");
	_getch();

	return 0;
}