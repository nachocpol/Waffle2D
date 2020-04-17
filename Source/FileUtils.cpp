#include "FileUtils.h"

#include <stdio.h>
#include <string.h>

using namespace Waffle;

bool File::ReadFile(const char* file, const char* data, long& size)
{
	FILE* pFile = nullptr;
	fopen_s(&pFile, file, "r");
	if (!pFile)
	{
		return false;
	}
	fseek(pFile, 0, SEEK_END);
	size = ftell(pFile);
	if (data)
	{
		memset((void*)data, 0, size);
		fseek(pFile, 0, SEEK_SET);
		fread_s((void*)data, size, sizeof(char), size / sizeof(char), pFile);
	}
	fclose(pFile);
	return true;
}

