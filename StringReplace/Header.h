#pragma once
#include <stdio.h>

__declspec(dllexport) std::string GetName();
__declspec(dllexport) std::string GetSurname();
__declspec(dllexport) void Replace(std::string searchString, std::string replaceString);

