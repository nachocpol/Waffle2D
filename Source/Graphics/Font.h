#pragma once

namespace Waffle
{
	class Image;
	class Font
	{
	public:
		Font();
		~Font();

		static Font* CreateFromFile(const char* path);

	private:
		Image* m_FontImage;
	}
};