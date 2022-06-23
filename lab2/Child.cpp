#include <iostream>

using std::cin;
using std::cout;
using std::endl;
using std::swap;
using std::string;

void outInput(char* A, int n)
{
	cout << "Your mass input:" << endl;
	for (int i = 0; i < n; i++) {
		cout << A[i] << " ";
	}
	cout << endl;
}

void sort(char* A, int n) 
{
	for (int i = 1; i < n; i++)
		for (int j = i; j > 0 && A[j - 1] > A[j]; j--)
			swap(A[j - 1], A[j]);
}

void outRes(char* A, int n)
{
	cout << "Sorted mass:" << endl;
	for (int i = 0; i < n; i++) {
		cout << A[i] << " ";
	}
	cout << endl;
}

int main(int argc, char* argv[])
{
	int n = atoi(argv[1]);

	char* A = new char[n];
	for (int i = 2; i < n + 2; i++) {
		A[i - 2] = argv[i][0];
	}
	
	outInput(A, n);
	sort(A, n);
	outRes(A, n);

	delete[] A;

	system("pause");
	return 0;
}