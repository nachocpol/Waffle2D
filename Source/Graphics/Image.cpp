#include "Graphics/Image.h"
#include "Graphics/MathUtils.h"
#include "FileUtils.h"
#include "Logging.h"

#include "glad/glad.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <string>

using namespace Waffle;

Image::Image()
	: m_ImageID(0)
	, m_Width(0)
	, m_Height(0)
{
}

Image::~Image()
{
}

static bool CreateTextureInternal(unsigned int textureId, unsigned char* data, int w, int h, PixelFormat::T pixelFormat)
{
	GLint internalFmt = 0;
	GLenum format = 0;
	int numComponents = 0;

	switch (pixelFormat)
	{
	case PixelFormat::SRGB:
		internalFmt = GL_SRGB;
		format = GL_RGB;
		numComponents = 3;
		break;
	case PixelFormat::SRGBA:
		internalFmt = GL_SRGB8_ALPHA8;
		format = GL_RGBA;
		numComponents = 4;
		break;
	case PixelFormat::R:
		internalFmt = GL_R8;
		format = GL_RED;
		numComponents = 1;
		break;
	}

	// https://www.khronos.org/opengl/wiki/Common_Mistakes#Texture_upload_and_pixel_reads
	// This could be fixed using glPixelStorei(GL_UNPACK_ALIGNMENT, #). I need to investigate it.
	bool changedPack = false;
	if (((w*numComponents) % 4) != 0)
	{
		changedPack = true;
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		//ERR("Texture width invalid: %i * %i modulo 4 must be 0 \n", w, numComponents);
		//return false;
	}

	// Create the GL texture:
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, internalFmt, w, h, 0, format, GL_UNSIGNED_BYTE, data);
	glBindTexture(GL_TEXTURE_2D, GL_NONE);

	if (changedPack)
	{
		glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	}

	return true;
}

Image* Waffle::Image::CreateFromFile(const char* path)
{
	std::string fixedPath(path);
	if (!FileSystem::Get().FixupPath(fixedPath))
	{
		return nullptr;
	}

	int w, h, n;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* pData = stbi_load(fixedPath.c_str(), &w, &h, &n, 0);
	if (!pData)
	{
		ERR("Failed to create the image (%s) \n INFO:%s\n", path, stbi_failure_reason());
		return nullptr;
	}

	if (n < 3)
	{
		stbi_image_free(pData);
		ERR("Can't load texture with %i channels! \n", n);
		return nullptr;
	}
	
	Image* image = new Image;
	image->m_Width = w;
	image->m_Height = h;

	glGenTextures(1, &image->m_ImageID);

	if (!CreateTextureInternal(image->m_ImageID, pData, w, h, n == 3 ? PixelFormat::SRGB : PixelFormat::SRGBA))
	{
		glDeleteTextures(1, &image->m_ImageID);
		stbi_image_free(pData);
		delete image;
		return nullptr;
	}

	return image;
}

Image* Waffle::Image::CreateFromMemory(unsigned char* data, int width, int height, PixelFormat::T format)
{
	if ((width * height) <= 0)
	{
		return nullptr;
	}

	Image* image = new Image;
	image->m_Width = width;
	image->m_Height = height;

	glGenTextures(1, &image->m_ImageID);

	if (!CreateTextureInternal(image->m_ImageID, data, width, height, format))
	{
		glDeleteTextures(1, &image->m_ImageID);
		delete image;
		return nullptr;
	}

	return image;
}

int Image::GetWidth() const
{
	return m_Width;
}

int Image::GetHeight() const
{
	return m_Height;
}
