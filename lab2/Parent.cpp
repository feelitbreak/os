#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <iostream>
#include <string>
#include <ctime>
#include <conio.h>
using namespace std;

int main()
{
	setlocale(LC_ALL, "rus");
	char* buff = new char[256];
	string a = "Child.exe";
	string b;
	cout << "Input the size of your mass." << endl;
	int n;
	cin >> n;
	a += " ";
	a += _itoa(n, buff, 10);
	cout << "Input the elements of your mass." << endl;
	for (int i = 0; i < n; i++)
	{
		cin >> b;
		a += " ";
		a += b;
	}
	n = a.length();
	char* lpszCommandLine = new char[n + 1];
	strcpy(lpszCommandLine, a.c_str());
	STARTUPINFO si;
	PROCESS_INFORMATION piApp;
	ZeroMemory(&si, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);
	si.dwYCountChars = 50;
	si.dwFlags = STARTF_USECOUNTCHARS;

	if (!CreateProcess(NULL, lpszCommandLine, NULL, NULL, FALSE,
		CREATE_NEW_CONSOLE, NULL, NULL, &si, &piApp))
	{
		_cputs("The new process is not created.\n");
		_cputs("Check the name of the process.\n");
		_cputs("Press any key to finish.\n");
		_getch();
		return 0;
	}
	_cputs("The new process is created.\n");

	WaitForSingleObject(piApp.hProcess, INFINITE);
	CloseHandle(piApp.hThread);
	CloseHandle(piApp.hProcess);
	return 0;

}