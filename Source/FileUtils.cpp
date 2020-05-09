#include "FileUtils.h"

#include <stdio.h>

#include <Windows.h>

using namespace Waffle;

bool File::ReadFile(const char* file, const char* data, long& size)
{
	std::string fixedPath(file);
	if (!FileSystem::Get().FixupPath(fixedPath))
	{
		return false;
	}

	FILE* pFile = nullptr;
	fopen_s(&pFile, fixedPath.c_str(), "r");
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

FileSystem::FileSystem()
{
}

FileSystem::FileSystem(const FileSystem& other)
{
}

FileSystem::~FileSystem()
{
}

FileSystem& FileSystem::Get()
{
	static FileSystem* kInstance = nullptr;
	if (!kInstance)
	{
		kInstance = new	FileSystem;
		kInstance->AddFileDevice({ "Data/", FileDevice::Type::Assets, "data" });
		kInstance->AddFileDevice({ "Source/Shaders/", FileDevice::Type::ShaderSource, "shaders" });
	}
	return *kInstance;
}

void FileSystem::AddFileDevice(FileDevice device)
{
	m_Devices.push_back(device);
}

bool FileSystem::FixupPath(std::string& path)
{
	// Get path identifier:
	size_t charPos = path.find(':');
	if (charPos == std::string::npos)
	{
		return false;
	}
	std::string pathIdentifier;
	pathIdentifier = std::string(path.c_str(), charPos);

	// Try to match with a file device:
	for (const FileDevice& device : m_Devices)
	{
		if (device.Identifier == pathIdentifier)
		{
			std::string tempPath = path;
			tempPath = tempPath.erase(0, charPos + 1);
			tempPath.insert(0, device.RootPath);
			if (FileExists(tempPath))
			{
				path = tempPath;
				return true;
			}
			// Don't break, we can have more devices for this file type.
		}
	}

	return false;
}

bool FileSystem::FileExists(const std::string& fixeupFile)
{
	DWORD res;
	res = GetFileAttributesA(fixeupFile.c_str());
	if (res == INVALID_FILE_ATTRIBUTES)
	{
		return false;
	}
	return true;
}
