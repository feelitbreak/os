//Кендысь Алексей, 2 курс, 9 группа. Лабораторная №7

#include "Header.h"

Info* createInfoProd(int i, int& sumProd, MonitorStack& stack)
{
	int nElem;
	short* massElem;
	cout << "Input the amount of elements for producer " << i << "." << endl;
	cin >> nElem;
	sumProd += nElem;
	massElem = new short[nElem];
	cout << "Input the elements for producer " << i << "." << endl;
	for (int j = 0; j < nElem; j++) {
		cin >> massElem[j];
	}

	Info* prodInfo = new Info(nElem, massElem, stack);

	delete[] massElem;
	return prodInfo;
}

Info* createInfoCons(int i, int& sumCons, MonitorStack& stack)
{
	int nElem;

	cout << "Input the amount of elements for consumer " << i << "." << endl;
	cin >> nElem;
	sumCons += nElem;

	Info* consInfo = new Info(nElem, stack);

	return consInfo;
}

int main()
{
	DWORD IDProducer, IDConsumer;
	HANDLE hSemaphoreProd, hSemaphoreCons, hMutex;
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

	int n = nProd + nCons;
	handles = new HANDLE[n];

	hSemaphoreProd = CreateSemaphore(NULL, 0, size, "SemaphoreProd");
	if (NULL == hSemaphoreProd)
	{
		delete[] handles;

		cout << "Failed to create semaphore." << endl;
		system("pause");
		return GetLastError();
	}

	hSemaphoreCons = CreateSemaphore(NULL, 0, size, "SemaphoreCons");
	if (NULL == hSemaphoreCons)
	{
		CloseHandle(hSemaphoreProd);
		delete[] handles;

		cout << "Failed to create semaphore." << endl;
		system("pause");
		return GetLastError();
	}

	hMutex = CreateMutex(NULL, FALSE, "Mutex");
	if (NULL == hMutex)
	{
		CloseHandle(hSemaphoreCons);
		CloseHandle(hSemaphoreProd);
		delete[] handles;

		cout << "Failed to create mutex." << endl;
		system("pause");
		return GetLastError();
	}

	int sumProd = 0;
	int sumCons = 0;

	for (int i = 0; i < nProd && i < n; i++) 
	{
		Info* prodInfo = createInfoProd(i + 1, sumProd, stack);

		handles[i] = CreateThread(NULL, 0, producer, reinterpret_cast<void*>(prodInfo), 0, &IDProducer);
		if (NULL == handles[i])
		{
			for (int j = i - 1; j >= 0; j--)
			{
				CloseHandle(handles[j]);
			}
			CloseHandle(hMutex);
			CloseHandle(hSemaphoreCons);
			CloseHandle(hSemaphoreProd);
			delete[] handles;

			cout << "Failed to create thread." << endl;
			system("pause");
			return GetLastError();
		}
	}

	for (int i = nProd; i < n; i++) {
		Info* consInfo = createInfoCons(i + 1 - nProd, sumCons, stack);

		handles[i] = CreateThread(NULL, 0, consumer, reinterpret_cast<void*>(consInfo), 0, &IDConsumer);
		if (NULL == handles[i])
		{
			for (int j = i - 1; j >= 0; j--)
			{
				CloseHandle(handles[j]);
			}
			CloseHandle(hMutex);
			CloseHandle(hSemaphoreCons);
			CloseHandle(hSemaphoreProd);
			delete[] handles;

			cout << "Failed to create thread." << endl;
			system("pause");
			return GetLastError();
		}
	}

	if (sumProd < sumCons) 
	{
		for (int j = n - 1; j >= 0; j--)
		{
			CloseHandle(handles[j]);
		}
		CloseHandle(hMutex);
		CloseHandle(hSemaphoreCons);
		CloseHandle(hSemaphoreProd);
		delete[] handles;

		cout << "Error. Too many products to consume." << endl;
		system("pause");
		return 0;
	} else if(sumProd > sumCons + size) {
		for (int j = n - 1; j >= 0; j--)
		{
			CloseHandle(handles[j]);
		}
		CloseHandle(hMutex);
		CloseHandle(hSemaphoreCons);
		CloseHandle(hSemaphoreProd);
		delete[] handles;

		cout << "Error. Not enough space in the stack." << endl;
		system("pause");
		return 0;
	}
	ReleaseSemaphore(hSemaphoreProd, size, NULL);

	WaitForMultipleObjects(n, handles, TRUE, INFINITE);

	for (int j = n - 1; j >= 0; j--)
	{
		CloseHandle(handles[j]);
	}
	CloseHandle(hMutex);
	CloseHandle(hSemaphoreCons);
	CloseHandle(hSemaphoreProd);
	delete[] handles;

	system("pause");
	return 0;
}