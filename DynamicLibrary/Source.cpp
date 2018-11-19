#include "Header.h"
#include <Windows.h>
#include <math.h>
#include <iostream>

using namespace std;

double GetSqrt(double number)
{
	return sqrt(number);
}

double GetExp(double number)
{
	return exp(number);
}

bool APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		cout << "Sqrt(10): " << GetSqrt(10) << endl;
		cout << "Exp(5): " << GetExp(5) << endl;
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}