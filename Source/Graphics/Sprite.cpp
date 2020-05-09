#include "Graphics/Sprite.h"

using namespace Waffle;

Sprite::Sprite()
	: m_size(0, 0)
	, m_image(nullptr)
	, m_imageScale(1.0f, 1.0f)
	, m_imageBias(0.0f, 0.0f)
	, m_isUI(false)
{
}

Sprite::Sprite(float sx, float sy)
	: m_size(sx,sy)
	, m_image(nullptr)
	, m_imageScale(1.0f, 1.0f)
	, m_imageBias(0.0f, 0.0f)
	, m_isUI(false)
{
}

Sprite::Sprite(float sx, float sy, Image* image)
	: m_size(sx, sy)
	, m_image(image)
	, m_imageScale(1.0f, 1.0f)
	, m_imageBias(0.0f, 0.0f)
	, m_isUI(false)
{
}

Sprite::Sprite(const Sprite& other)
	: m_size(other.m_size)
	, m_image(other.m_image)
	, m_tint(other.m_tint)
	, m_imageScale(other.m_imageScale)
	, m_imageBias(other.m_imageBias)
	, m_isUI(other.m_isUI)
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

void Sprite::Move(float dx, float dy)
{
	m_transform.Position.X += dx;
	m_transform.Position.Y += dy;
}

void Sprite::Rotate(float dr)
{
	m_transform.Rotation += dr;
}

const Image* Sprite::GetImage()const
{
	return m_image;
}

void Sprite::SetImage(Image* image)
{
	m_image = image;
}

void Sprite::SetImageScaleBias(float su, float sv, float bu, float bv)
{
	m_imageScale = Vec2(su, sv);
	m_imageBias = Vec2(bu, bv);
}

Vec2 Sprite::GetImageScale() const
{
	return m_imageScale;
}

Vec2 Sprite::GetImageBias() const
{
	return m_imageBias;
}

void Sprite::SetIsUI(bool ui)
{
	m_isUI = ui;
}

bool Sprite::GetIsUI()const
{
	return m_isUI;
}
