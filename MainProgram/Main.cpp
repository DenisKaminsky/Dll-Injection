#include <Windows.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <tlhelp32.h>

#include "..\\StringReplace\\Header.h"
#pragma comment(lib,"..\\Debug\\StringReplace.dll")

#include "..\\SomeLibrary\\Header.h"
#pragma comment(lib,"..\\Debug\\SomeLibrary.lib")

using namespace std;

typedef double Func(double);

const int n = 5; //strings count
string* stringArray;

void InitializeArray()
{
	int a = 5;
	string s = "";

	stringArray = new string[n];
	for (int i = 0; i < n; i++)
	{
		s = "sometext" + to_string(i + 1);
		stringArray[i] = s;
	}
}

void DeleteObjects()
{
	delete[] stringArray;
}

DWORD getPID(string processName)
{
	DWORD PID = 0;
	wstring wName(processName.begin(), processName.end());
	HANDLE pHandle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);//создает снэпшот запущенных процессов
	PROCESSENTRY32 processEntry;	
	ZeroMemory(&processEntry, sizeof(processEntry));
	processEntry.dwSize = sizeof(processEntry);
	bool Loop = Process32First(pHandle, &processEntry);//первый процесс в снэпшоте

	while (Loop)
	{
		if (wName.compare(processEntry.szExeFile) == 0)//имя исполняемого файла процесса
		{
			PID = processEntry.th32ProcessID;
			CloseHandle(pHandle);
			break;
		}
		Loop = Process32Next(pHandle, &processEntry);
	}

	return PID;
}

void injectDll(string processName)
{
	string dllName = "DynamicLibrary.dll";
	LPVOID lpStartAddress, dllNameAddress;
	HANDLE hRemoteProcess = OpenProcess(PROCESS_ALL_ACCESS | PROCESS_VM_WRITE, true , getPID(processName));//дескриптор демо процесса

	if (hRemoteProcess != NULL)
	{
		lpStartAddress = (LPVOID)GetProcAddress(GetModuleHandle(L"kernel32.dll"), "LoadLibraryA");//адрес LoadLibraryA в демо процессе
		dllNameAddress = VirtualAllocEx(hRemoteProcess, NULL, dllName.length() + 1, MEM_COMMIT, PAGE_READWRITE);//выделяем память в демо процессе для имени dll
		WriteProcessMemory(hRemoteProcess, dllNameAddress, dllName.c_str(), dllName.length() + 1, NULL);//записываем строку в память демо процесса

		if (CreateRemoteThread(hRemoteProcess, NULL, 0, (LPTHREAD_START_ROUTINE)lpStartAddress, (LPVOID)dllNameAddress, 0, NULL))
		{
			Sleep(1000);
			cout << "Dll was injected" << endl;
			CloseHandle(hRemoteProcess);
		}
		else
			cout << "Cant create remote thread" << endl;
	}
	else {
		cout << "Cannot find process " << processName << endl;
	}
}

void loadLibrary()
{
	HMODULE dll = 0;
	if ((dll = LoadLibrary(L"DynamicLibrary")))//загрузка библиотеки в память процесса
	{
		Func *Sqrt, *Exp;
		Sqrt = (Func*)GetProcAddress(dll,MAKEINTRESOURCEA(1));
		Exp = (Func*)GetProcAddress(dll,MAKEINTRESOURCEA(2));
		cout << "Sqrt(3): " << Sqrt(3) << endl;
		cout << "Exp(4): " << Exp(4) << endl;
		FreeLibrary(dll);
	}
	else
		cout << "Can't load this dll" << endl;
}

void replaceString()
{
	cout << "Initial array: \n" << endl;
	for (int i = 0; i < n; i++)
		cout << stringArray[i] << endl;
	cout << "" << endl;

	Replace(stringArray[0], "DenisKaminsky");

	cout << "Final array: \n" << endl;
	for (int i = 0; i < n; i++)
		cout << stringArray[i] << endl;
	cout << "" << endl;
}

void main()
{
	InitializeArray();
	//string search
	cout << "<<Task 1 - String replace>>:" << endl;
	replaceString();

	//static lib link
	cout << "<<Task 2 - Static lib load>>:" << endl;
	cout << "Sin(40): " << GetSin(40) << endl;
	cout << "Cos(40): " << GetCos(40) << "\n" << endl;
	
	//dynamic lib link
	cout << "<<Task3 - Dll load>>:" << endl;
	loadLibrary();

	cout << "" << endl << "<<Task4 - Dll injection>>:" << endl;
	cout << "Press Enter to start:" << endl;
	getchar();
	injectDll("DemoProcess.exe");

	DeleteObjects();
	cout << "Press Enter to Exit: " << endl;
	getchar();
}
