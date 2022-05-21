//Кендысь Алексей, 2 курс, 9 группа. Лабораторная №7

#include "Header.h"

int main()
{
	DWORD IDProducer, IDConsumer;
	HANDLE hSemaphoreProd, hSemaphoreCons;
	int size;
	int nProd, nCons;
	HANDLE* handles;

	cout << "Input stack size." << endl;
	cin >> size;
	MonitorStack stack(size);

	cout << "Input the amount of producer threads." << endl;
	cin >> nProd;
	cout << "Input the amount of consumer threads." << endl;
	cin >> nCons;

	handles = new HANDLE[nProd + nCons];

	hSemaphoreProd = CreateSemaphore(NULL, 0, size, "SemaphoreProd");
	if (hSemaphoreProd == NULL)
	{
		cout << "Create semaphore failed. Press any key to exit." << endl;
		cin.get();
		return GetLastError();
	}

	hSemaphoreCons = CreateSemaphore(NULL, 0, size, "SemaphoreCons");
	if (hSemaphoreCons == NULL)
	{
		cout << "Create semaphore failed. Press any key to exit." << endl;
		cin.get();
		CloseHandle(hSemaphoreProd);
		return GetLastError();
	}

	int sumProd = 0;
	int sumCons = 0;

	int nElem;
	short* massElem;
	for (int i = 0; i < nProd; i++) 
	{
		cout << "Input the amount of elements for producer " << i + 1 << "." << endl;
		cin >> nElem;
		sumProd += nElem;
		massElem = new short[nElem];
		cout << "Input the elements for producer " << i + 1 << "." << endl;
		for (int j = 0; j < nElem; j++) {
			cin >> massElem[j];
		}

		Info* prodInput = new Info(nElem, massElem, stack);
		handles[i] = CreateThread(NULL, 0, producer, (void*)prodInput, 0, &IDProducer);
		if (handles[i] == NULL)
			return GetLastError();
	}

	for (int i = 0; i < nCons; i++) {
		cout << "Input the amount of elements for consumer " << i + 1 << "." << endl;
		cin >> nElem;
		sumCons += nElem;

		Info* consInput = new Info(nElem, stack);
		handles[i + nProd] = CreateThread(NULL, 0, consumer, (void*)consInput, 0, &IDConsumer);
		if (handles[i + nProd] == NULL)
			return GetLastError();
	}

	if (sumProd < sumCons) 
	{
		cout << "Error. Too many products to consume." << endl;
		CloseHandle(hSemaphoreCons);
		CloseHandle(hSemaphoreProd);
		for (int i = nProd + nCons - 1; i >= 0; i--)
		{
			CloseHandle(handles[i]);
		}

		return 0;
	} else if(sumProd > sumCons + size) {
		cout << "Error. Not enough space in the stack." << endl;
		CloseHandle(hSemaphoreCons);
		CloseHandle(hSemaphoreProd);
		for (int i = nProd + nCons - 1; i >= 0; i--)
		{
			CloseHandle(handles[i]);
		}

		return 0;
	}
	ReleaseSemaphore(hSemaphoreProd, size, NULL);

	WaitForMultipleObjects(nProd + nCons, handles, TRUE, INFINITE);

	CloseHandle(hSemaphoreCons);
	CloseHandle(hSemaphoreProd);
	for(int i = nProd + nCons - 1; i >= 0; i--)
	{
		CloseHandle(handles[i]);
	}

	return 0;
}