#include <windows.h>
#include <iostream>

using namespace std;

int main(int argc, char* argv[])
{
	int n = atoi(argv[1]);

	HANDLE	hMutex;

	// ��������� �������
	hMutex = OpenMutex(SYNCHRONIZE, FALSE, "ChildMutex");
	if (hMutex == NULL)
	{
		cout << "Open mutex failed." << endl;
		cout << "Press any key to exit." << endl;
		cin.get();

		return GetLastError();
	}

	HANDLE EventB = OpenEvent(SYNCHRONIZE, FALSE, "B");
	if (EventB == NULL)
		return GetLastError();

	string mess;
	for (int i = 0; i < n; i++)
	{
		// ����������� �������
		WaitForSingleObject(hMutex, INFINITE);
		cout << "Input B message." << endl;
		cin >> mess;
		SetEvent(EventB);
		ReleaseMutex(hMutex);
		Sleep(500);
	}
	// ��������� ���������� �������
	CloseHandle(hMutex);

	return 0;
}