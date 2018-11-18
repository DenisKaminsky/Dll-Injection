#include <Windows.h>
#include <stdio.h>
#include <iostream>
#include <string>

using namespace std;

const int n = 5; //strings count
string* stringArray;

void InitializeArray()
{
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

	//Replace(stringArray[3].c_str(), "DenisKaminsky");
	cout << "Final array: \n" << endl;
	for (int i = 0; i < n; i++)
		cout << stringArray[i] << endl;
	cout << "" << endl;
}

void main()
{
	InitializeArray();
	//string replace
	replaceString();

	DeleteObjects();
}
