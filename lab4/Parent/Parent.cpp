#include <windows.h>
#include <iostream>

using namespace std;

int main()
{
	HANDLE hSemaphore;
	hSemaphore = OpenSemaphore(SYNCHRONIZE, FALSE, "ParentSemaphore");
	if (hSemaphore == NULL)
	{
		cout << "Open semaphore failed." << endl;
		cout << "Press any key to exit." << endl;
		cin.get();

		return GetLastError();
	}
	int i;

	for (i = 0; i < 10; i++)
	{
		// отмечаем, что один элемент готов
		ReleaseSemaphore(hSemaphore, 1, NULL);
		Sleep(500);
	}

	CloseHandle(hSemaphore);

	return 0;
}
