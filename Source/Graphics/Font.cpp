#include "Font.h"
#include "FileUtils.h"
#include "Image.h"

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

const CharacterData& Font::GetCharacterData(char c)
{
	// TO-DO: I mean.. cmon.
	return m_Characters[c - 32];
}

Font* Font::CreateFromFile(const char* path)
{
	// TO-DO: this methods is giving me aids. Clean this shiet up. All on the stack???
	std::string fixedPath(path);
	if (!FileSystem::Get().FixupPath(fixedPath))
	{
		return nullptr;
	}

	static unsigned char ttfBuffer[1 << 20];
	fread(ttfBuffer, 1, 1 << 20, fopen(fixedPath.c_str(), "rb"));
	static unsigned char tempBitmap[1024* 1024];
	stbtt_bakedchar cdata[96];
	stbtt_BakeFontBitmap(ttfBuffer, 0, 32.0f, tempBitmap, 1024, 1024, 32, 96, cdata);

	Font* font = new Font;
	font->m_FontImage = Image::CreateFromMemory(tempBitmap, 1024, 1024, PixelFormat::R);

	font->m_Characters.resize(96);
	for (int cidx = 0; cidx < 96; ++cidx)
	{
		// stbtt_GetBakedQuad?
		CharacterData& dat = font->m_Characters[cidx];
		dat.X0 = cdata[cidx].x0;
		dat.Y0 = cdata[cidx].y0;
		dat.X1 = cdata[cidx].x1;
		dat.Y1 = cdata[cidx].y1;
		dat.XOffset = cdata[cidx].xoff;
		dat.YOffset = cdata[cidx].yoff;
		dat.XAdvance = cdata[cidx].xadvance;
	}

	return font;
}

Image* Font::GetFontImage() const
{
	return m_FontImage;
}
