#include "Font.h"
#include "FileUtils.h"

#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"

#include <string>

using namespace Waffle;

Font::Font()
{

}

Font::~Font()
{
}

Font* Font::CreateFromFile(const char* path)
{
	std::string fixedPath(path);
	if (!FileSystem::Get().FixupPath(fixedPath))
	{
		return nullptr;
	}


	static unsigned char kTTFData[1 << 25]; // 33 MBs?
	memset(kTTFData, 0, sizeof(kTTFData));
	fread(kTTFData, 1, 1 << 25, fopen(fixedPath.c_str(), "rb"));

	stbtt_fontinfo fontInfo = {};
	stbtt_InitFont(&fontInfo, kTTFData, stbtt_GetFontOffsetForIndex(kTTFData, 0));

	unsigned  char* fontData = nullptr;
	int width, height;
	fontData = stbtt_GetCodepointBitmap(&fontInfo, 1.0f, 1.0f, 'a', &width, &height, 0, 0);

	return nullptr;
}