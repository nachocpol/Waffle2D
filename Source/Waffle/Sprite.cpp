#include "Sprite.h"

using namespace Waffle;

Sprite::Sprite()
	: m_size(0, 0)
	, m_image(nullptr)
{
}

Sprite::Sprite(float sx, float sy)
	: m_size(sx,sy)
	, m_image(nullptr)
{
}

Sprite::Sprite(float sx, float sy, Image* image)
	: m_size(sx, sy)
	, m_image(image)
{
}

void Sprite::SetPosition(float x, float y)
{
	m_transform.Position = Vec2(x, y);
}

void Sprite::SetScale(float x, float y)
{
	m_transform.Scale = Vec2(x, y);
}

void Sprite::SetRotation(float r)
{
	m_transform.Rotation = r;
}

Vec2 Sprite::GetSize() const
{
	return m_size;
}

void Sprite::SetSize(float sx, float sy)
{
	m_size = Vec2(sx, sy);
}

void Sprite::SetTint(float r, float g, float b, float a)
{
	m_tint = Color(r, g, b, a);
}

Color Sprite::GetTint() const
{
	return m_tint;
}

Transform Sprite::GetTransform() const
{
	return m_transform;
}

const Image* Sprite::GetImage()const
{
	return m_image;
}

void Sprite::SetImage(Image* image)
{
	m_image = image;
}
