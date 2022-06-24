#include <windows.h>
#include <iostream>

using std::cin;
using std::cout;
using std::endl;
using std::string;

static int const radix = 10;
static int const maxStrLength = 255;
static int const bufferHeight = 50;

char* input() {
	string a = "Child.exe ";
	string b;

	int n;
	cout << "Input the size of your mass." << endl;
	cin >> n;

	char* buff = new char[maxStrLength];
	_itoa_s(n, buff, maxStrLength, radix);
	a += buff;
	delete[] buff;

	cout << "Input the elements of your mass." << endl;
	for (int i = 0; i < n; i++)
	{
		cin >> b;
		a += " ";
		a += b;
	}

	char* lpszCommandLine = new char[a.length() + 1];
	strcpy_s(lpszCommandLine, a.length() + 1, a.c_str());
	return lpszCommandLine;
}

int main()
{
	STARTUPINFO si;
	PROCESS_INFORMATION piApp;

	ZeroMemory(&si, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);
	si.dwYCountChars = bufferHeight;
	si.dwFlags = STARTF_USECOUNTCHARS;

	char* lpszCommandLine = input();

	if (NULL == CreateProcess(NULL, lpszCommandLine, NULL, NULL, FALSE,
		CREATE_NEW_CONSOLE, NULL, NULL, &si, &piApp))
	{
		delete[] lpszCommandLine;

		cout << "The new process is not created." << endl;
		system("pause");
		return 0;
	}
	cout << "The new process is created." << endl;

	WaitForSingleObject(piApp.hProcess, INFINITE);

	CloseHandle(piApp.hThread);
	CloseHandle(piApp.hProcess);
	delete[] lpszCommandLine;

	return 0;

}