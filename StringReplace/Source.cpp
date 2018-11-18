#include "Header.h"
#include <Windows.h>
#include <iostream>

using namespace std;

string GetName()
{
	return "Denis";
}

string GetSurname()
{
	return "Kaminsky";
}

void SayHello()
{
	cout << "\nHELLO FROM INJECTION DLL, MF!\n" << endl;
}

void Replace(string searchString, string replaceString)
{
	MEMORY_BASIC_INFORMATION mbi;//��������� � ��������� � ����������� ������ ��������
	BOOL isMatch,isChanged;
	CHAR page[4096];
	size_t searchLength = searchString.length();
	size_t replaceLength = replaceString.length();
	SYSTEM_INFO sysInfo;
	GetSystemInfo(&sysInfo);//���������� � ������� �������
	DWORD_PTR memStart = (DWORD_PTR)sysInfo.lpMinimumApplicationAddress;
	DWORD_PTR memEnd = (DWORD_PTR)sysInfo.lpMaximumApplicationAddress;
	DWORD_PTR pageSize = sysInfo.dwPageSize;
	HANDLE hProcess = GetCurrentProcess();

	for (DWORD_PTR currAdress = memStart; currAdress < memEnd; currAdress += pageSize)
	{
		VirtualQueryEx(hProcess, (LPCVOID)(currAdress), &mbi, sizeof(mbi));//������ ���� � ���������. currAdr - ����� ������� �������
		if (mbi.State == MEM_COMMIT && mbi.Protect != PAGE_NOACCESS)//Mem commit - �������� ��� ������� �������� ����� � ��� ������ 																							
		{			
			isChanged = false;
			ZeroMemory(page, sizeof(page));
			ReadProcessMemory(hProcess, (LPCVOID)currAdress, page, sizeof(page), NULL);			

			for (size_t pagePos = 0; pagePos < pageSize - searchLength; pagePos++)
			{
				isMatch = false;
				for (int strPos = 0; (strPos < searchLength) && isMatch; strPos++)
				{
					isMatch = (page[pagePos + strPos] == searchString[strPos]);
				}

				if (isMatch)
				{
					for (int strPos = 0; strPos < searchLength; strPos++)
					{
						page[pagePos + strPos] = replaceString[strPos % replaceLength];
					}
					isChanged = true;
				}
			}
			
			if (isChanged)
			{
				WriteProcessMemory(hProcess, (LPVOID)currAdress, (LPCVOID)page, sizeof(page), NULL);
			}
		}
	}

}