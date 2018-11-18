#include <Windows.h>
#include <stdio.h>
#include <iostream>
#include <string>

#include "..\\StringReplace\\Header.h"
#pragma comment(lib,"..\\Debug\\StringReplace.dll")

using namespace std;

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
	cout << "<<Task 1>>:" << endl;
	replaceString();

	//static lib
	cout << "<<Task 2>>:" << endl;
	cout << "Name: " << GetName() << endl;
	cout << "Surname: " << GetSurname() << "\n" << endl;
	DeleteObjects();
}
