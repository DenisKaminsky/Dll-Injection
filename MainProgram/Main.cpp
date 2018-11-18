#include <Windows.h>
#include <stdio.h>
#include <iostream>
#include <string>

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

	DeleteObjects();
}
