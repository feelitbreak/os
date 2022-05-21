#include "Header.h"

DWORD WINAPI producer(LPVOID par) {
	ElementsToProduce* elems = (ElementsToProduce*) par;

	int n = elems->nElem;
	short* mass = elems->massElem;
	MonitorStack &stack = elems->stack;

	HANDLE hSemaphoreProd, hSemaphoreCons;

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

	for (int i = 0; i < n; i++) {
		WaitForSingleObject(hSemaphoreProd, INFINITE);
		stack.Push(mass[i]);
		cout << "Element " << mass[i] << " has been produced." << endl;
		ReleaseSemaphore(hSemaphoreCons, 1, NULL);
	}

	CloseHandle(hSemaphoreCons);
	CloseHandle(hSemaphoreProd);
	return 0;
}