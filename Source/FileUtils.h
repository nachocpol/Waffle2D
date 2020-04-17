#pragma once

#include <string>
#include <vector>

namespace Waffle
{
	class File
	{
	public:
		// Reads the contents of the file into data, data must be large enough to hold the file.
		//  Passing nullptr as data is valid, the method will return the size of the file.
		static bool ReadFile(const char* file, const char* data, long& size);
	};

	struct FileDevice
	{
		struct Type
		{
			enum T
			{
				Assets,
				ShaderSource,
				COUNT
			};
		};
		std::string RootPath;
		Type::T DeviceType;
		std::string Identifier; // assets
	};

	class FileSystem
	{
	private:
		FileSystem();
		FileSystem(const FileSystem& other);
		~FileSystem();

	public:
		static FileSystem& Get();
		void AddFileDevice(FileDevice device);
		bool FixupPath(std::string& path);
		bool FileExists(const std::string& fixeupFile);

	private:
		std::vector<FileDevice> mDevices;
	};
}