#include "Image.h"
#include "MathUtils.h"

#include "glad/glad.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

using namespace Waffle;

Image::Image()
	: m_imageID(0)
	, m_width(0)
	, m_height(0)
{
}

Image::~Image()
{
}

Image* Waffle::Image::CreateFromFile(const char* path)
{
	int w, h, n;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* pData = stbi_load(path, &w, &h, &n, 3);
	if (!pData)
	{
		printf("Failed to create the image (%s) \n INFO:%s\n", path, stbi_failure_reason());
		return nullptr;
	}

	bool pw2 = IS_POWER_OF_2(w);
	bool pw23 = IS_POWER_OF_2(h);
	if(!IS_POWER_OF_2(w) || !IS_POWER_OF_2(h))
	{
		stbi_image_free(pData);
		printf("Texture dimensions must be power of two: W:%i H:%i \n", w, h);
		return nullptr;
	}
	if (n < 3)
	{
		stbi_image_free(pData);
		printf("Can't load texture with %i channels! \n", n);
		return nullptr;
	}
	
	Image* image = new Image;
	image->m_width = w;
	image->m_height = h;

	// Create the GL texture:
	glGenTextures(1, &image->m_imageID);
	glBindTexture(GL_TEXTURE_2D, image->m_imageID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	GLint internalFmt = n == 3 ? GL_SRGB : GL_SRGB8_ALPHA8;
	GLenum format = n == 3 ? GL_RGB : GL_RGBA;
	glTexImage2D(GL_TEXTURE_2D, 0, internalFmt, w, h, 0, format, GL_UNSIGNED_BYTE, pData);
	glBindTexture(GL_TEXTURE_2D, GL_NONE);

	stbi_image_free(pData);

	return image;
}

int Image::GetWidth() const
{
	return m_width;
}

int Image::GetHeight() const
{
	return m_height;
}
