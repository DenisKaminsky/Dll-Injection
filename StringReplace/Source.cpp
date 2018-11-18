#include "Header.h"
#include <Windows.h>

using namespace std;

string GetName()
{
	return "Denis";
}

string GetSurname()
{
	return "Kaminsky";
}

void Replace(string &searchString, string replaceString)
{
	cout << "REPLACING...\n" << endl;
	MEMORY_BASIC_INFORMATION mbi;//иформация о страницах в виртуальной памяти процесса
	BOOL isMatch,isChanged;
	CHAR page[4096];
	size_t searchLength = searchString.length();
	size_t replaceLength = replaceString.length();
	SYSTEM_INFO sysInfo;
	GetSystemInfo(&sysInfo);//информация о текущей системе
	DWORD_PTR memStart = (DWORD_PTR)sysInfo.lpMinimumApplicationAddress;
	DWORD_PTR memEnd = (DWORD_PTR)sysInfo.lpMaximumApplicationAddress;
	DWORD_PTR pageSize = sysInfo.dwPageSize;
	HANDLE hProcess = GetCurrentProcess();

	for (DWORD_PTR currAdress = memStart; currAdress < memEnd; currAdress += pageSize)
	{
		VirtualQueryEx(hProcess, (LPCVOID)(currAdress), &mbi, sizeof(mbi));//запрос инфы о страницах. currAdr - адрес региона страниц
		if (mbi.State == MEM_COMMIT && mbi.Protect != PAGE_NOACCESS)//Mem commit - страница для которых выделено место в физ памяти 																							
		{			
			isChanged = false;
			ZeroMemory(page, sizeof(page));
			ReadProcessMemory(hProcess, (LPCVOID)currAdress, page, sizeof(page), NULL);			

			for (size_t pagePos = 0; pagePos < pageSize - searchLength; pagePos++)
			{
				isMatch = true;
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