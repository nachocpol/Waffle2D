#pragma once

namespace Waffle
{
	class File
	{
	public:
		// Reads the contents of the file into data, data must be large enough to hold the file.
		//  Passing nullptr as data is valid, the method will return the size of the file.
		static bool ReadFile(const char* file, const char* data, long& size);
	};
}