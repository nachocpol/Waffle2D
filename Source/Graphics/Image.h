#pragma once

namespace Waffle
{
	struct PixelFormat
	{
		enum T
		{
			SRGB,
			SRGBA,
			R,
			COUNT
		};
	};
	class Graphics;
	class Image
	{
		friend Graphics;
	public:
		Image();
		~Image();
		static Image* CreateFromFile(const char* path);
		static Image* CreateFromMemory(unsigned char* data, int width, int height,PixelFormat::T format);
		int GetWidth()const;
		int GetHeight()const;

	private:
		unsigned int m_ImageID;
		int m_Width;
		int m_Height;
	};
}