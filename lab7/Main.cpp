//Кендысь Алексей, 2 курс, 9 группа. Лабораторная №7

#include "Header.h"

int main()
{
	DWORD IDProducer, IDConsumer;
	int size;
	int nProd, nCons;
	HANDLE* handles;

	setlocale(LC_ALL, "rus");

	cout << "Input stack size." << endl;
	cin >> size;
	MonitorStack stack(size);

	cout << "Input the amount of producer threads." << endl;
	cin >> nProd;
	cout << "Input the amount of consumer threads." << endl;
	cin >> nCons;

	handles = new HANDLE[nProd + nCons];

	int nElem;
	short* massElem;
	for (int i = 0; i < nProd; i++) 
	{
		cout << "Input the amount of elements for producer " << i + 1 << "." << endl;
		cin >> nElem;
		massElem = new short[nElem];
		cout << "Input the elements for producer " << i + 1 << "." << endl;
		for (int j = 0; j < nElem; j++) {
			cin >> massElem[j];
		}

		ElementsToProduce* prodInput = new ElementsToProduce(nElem, massElem);
		handles[i] = CreateThread(NULL, 0, producer, (void*)prodInput, 0, &IDProducer);
		if (handles[i] == NULL)
			return GetLastError();
	}

	WaitForMultipleObjects(nProd + nCons, handles, TRUE, INFINITE);
	for(int i = 0; i < nProd + nCons; i++)
	{
		CloseHandle(handles[i]);
	}

	return 0;
}