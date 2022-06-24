#include "Header.h"

DWORD WINAPI producer(LPVOID par) {
	Info* elems = reinterpret_cast<Info*>(par);

	HANDLE hSemaphoreProd, hSemaphoreCons, hMutex;

	hSemaphoreProd = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, "SemaphoreProd");
	if (NULL == hSemaphoreProd)
	{
		cout << "Failed to open semaphore." << endl;
		system("pause");
		return GetLastError();
	}

	hSemaphoreCons = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, "SemaphoreCons");
	if (NULL == hSemaphoreCons)
	{
		CloseHandle(hSemaphoreProd);

		cout << "Failed to open semaphore." << endl;
		system("pause");
		return GetLastError();
	}

	hMutex = OpenMutex(SYNCHRONIZE, FALSE, "Mutex");
	if (NULL == hMutex)
	{
		CloseHandle(hSemaphoreCons);
		CloseHandle(hSemaphoreProd);

		cout << "Failed to open mutex." << endl;
		system("pause");
		return GetLastError();
	}

	for (int i = 0; i < elems->nElem; i++) {
		WaitForSingleObject(hSemaphoreProd, INFINITE);

		WaitForSingleObject(hMutex, INFINITE);
		elems->stack.Push(elems->massElem[i]);
		cout << "Element " << elems->massElem[i] << " has been produced." << endl;
		ReleaseMutex(hMutex);

		ReleaseSemaphore(hSemaphoreCons, 1, NULL);
	}

	CloseHandle(hMutex);
	CloseHandle(hSemaphoreCons);
	CloseHandle(hSemaphoreProd);
	return 0;
}

DWORD WINAPI consumer(LPVOID par) {
	Info* elems = reinterpret_cast<Info*>(par);

	HANDLE hSemaphoreProd, hSemaphoreCons, hMutex;

	hSemaphoreProd = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, "SemaphoreProd");
	if (NULL == hSemaphoreProd)
	{
		cout << "Failed to open semaphore." << endl;
		system("pause");
		return GetLastError();
	}

	hSemaphoreCons = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, "SemaphoreCons");
	if (NULL == hSemaphoreCons)
	{
		CloseHandle(hSemaphoreProd);

		cout << "Failed to open semaphore." << endl;
		system("pause");
		return GetLastError();
	}

	hMutex = OpenMutex(SYNCHRONIZE, FALSE, "Mutex");
	if (NULL == hMutex)
	{
		CloseHandle(hSemaphoreCons);
		CloseHandle(hSemaphoreProd);

		cout << "Failed to open mutex." << endl;
		system("pause");
		return GetLastError();
	}

	short elem;
	for (int i = 0; i < elems->nElem; i++) {
		WaitForSingleObject(hSemaphoreCons, INFINITE);

		WaitForSingleObject(hMutex, INFINITE);
		elem = elems->stack.Pop();
		cout << "Element " << elem << " has been consumed." << endl;
		ReleaseMutex(hMutex);

		ReleaseSemaphore(hSemaphoreProd, 1, NULL);
	}

	CloseHandle(hMutex);
	CloseHandle(hSemaphoreCons);
	CloseHandle(hSemaphoreProd);
	return 0;
}