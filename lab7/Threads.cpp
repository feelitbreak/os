#include "Header.h"

DWORD WINAPI producer(LPVOID par) {
	Info* elems = (Info*) par;

	int n = elems->nElem;
	short* mass = elems->massElem;
	MonitorStack &stack = elems->stack;

	HANDLE hSemaphoreProd, hSemaphoreCons, hMutex;

	hSemaphoreProd = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, "SemaphoreProd");
	if (hSemaphoreProd == NULL)
	{
		cout << "Open semaphore failed. Press any key to exit." << endl;
		cin.get();
		return GetLastError();
	}

	hSemaphoreCons = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, "SemaphoreCons");
	if (hSemaphoreCons == NULL)
	{
		cout << "Open semaphore failed. Press any key to exit." << endl;
		cin.get();
		CloseHandle(hSemaphoreProd);
		return GetLastError();
	}

	hMutex = OpenMutex(SYNCHRONIZE, FALSE, "Mutex");
	if (hMutex == NULL)
	{
		cout << "Open mutex failed. Press any key to exit." << endl;
		cin.get();
		CloseHandle(hSemaphoreCons);
		CloseHandle(hSemaphoreProd);
		return GetLastError();
	}

	for (int i = 0; i < n; i++) {
		WaitForSingleObject(hSemaphoreProd, INFINITE);
		WaitForSingleObject(hMutex, INFINITE);
		stack.Push(mass[i]);
		cout << "Element " << mass[i] << " has been produced." << endl;
		ReleaseMutex(hMutex);
		ReleaseSemaphore(hSemaphoreCons, 1, NULL);
	}

	CloseHandle(hMutex);
	CloseHandle(hSemaphoreCons);
	CloseHandle(hSemaphoreProd);
	return 0;
}

DWORD WINAPI consumer(LPVOID par) {
	Info* elems = (Info*)par;

	int n = elems->nElem;
	MonitorStack& stack = elems->stack;

	HANDLE hSemaphoreProd, hSemaphoreCons, hMutex;

	hSemaphoreProd = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, "SemaphoreProd");
	if (hSemaphoreProd == NULL)
	{
		cout << "Create semaphore failed. Press any key to exit." << endl;
		cin.get();
		return GetLastError();
	}

	hSemaphoreCons = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, "SemaphoreCons");
	if (hSemaphoreCons == NULL)
	{
		cout << "Create semaphore failed. Press any key to exit." << endl;
		cin.get();
		CloseHandle(hSemaphoreProd);
		return GetLastError();
	}

	hMutex = OpenMutex(SYNCHRONIZE, FALSE, "Mutex");
	if (hMutex == NULL)
	{
		cout << "Open mutex failed. Press any key to exit." << endl;
		cin.get();
		CloseHandle(hSemaphoreCons);
		CloseHandle(hSemaphoreProd);
		return GetLastError();
	}

	short elem;
	for (int i = 0; i < n; i++) {
		WaitForSingleObject(hSemaphoreCons, INFINITE);
		WaitForSingleObject(hMutex, INFINITE);
		elem = stack.Pop();
		cout << "Element " << elem << " has been consumed." << endl;
		ReleaseMutex(hMutex);
		ReleaseSemaphore(hSemaphoreProd, 1, NULL);
	}

	CloseHandle(hMutex);
	CloseHandle(hSemaphoreCons);
	CloseHandle(hSemaphoreProd);
	return 0;
}