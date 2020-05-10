#pragma once

#include <vector>

namespace Waffle
{
	struct CharacterData
	{
		float X0;
		float Y0;
		float X1;
		float Y1;
		float XOffset;
		float YOffset;
		float XAdvance;
	};

	class Image;
	class Font
	{
	public:
		Font();
		~Font();
		const CharacterData& GetCharacterData(char c);
		static Font* CreateFromFile(const char* path);
		Image* GetFontImage()const;

	private:
		Image* m_FontImage;
		std::vector<CharacterData> m_Characters;
	};
}