#include "Graphics/Sprite.h"

using namespace Waffle;

Sprite::Sprite()
	: m_Size(0, 0)
	, m_Image(nullptr)
	, m_ImageScale(1.0f, 1.0f)
	, m_ImageBias(0.0f, 0.0f)
	, m_IsUI(false)
{
}

Sprite::Sprite(float sx, float sy)
	: m_Size(sx,sy)
	, m_Image(nullptr)
	, m_ImageScale(1.0f, 1.0f)
	, m_ImageBias(0.0f, 0.0f)
	, m_IsUI(false)
{
}

Sprite::Sprite(float sx, float sy, Image* image)
	: m_Size(sx, sy)
	, m_Image(image)
	, m_ImageScale(1.0f, 1.0f)
	, m_ImageBias(0.0f, 0.0f)
	, m_IsUI(false)
{
}

Sprite::Sprite(const Sprite& other)
	: m_Size(other.m_Size)
	, m_Image(other.m_Image)
	, m_Tint(other.m_Tint)
	, m_ImageScale(other.m_ImageScale)
	, m_ImageBias(other.m_ImageBias)
	, m_IsUI(other.m_IsUI)
{
}

void Sprite::SetPosition(float x, float y)
{
	m_Transform.Position = Vec2(x, y);
}

void Sprite::SetScale(float x, float y)
{
	m_Transform.Scale = Vec2(x, y);
}

void Sprite::SetRotation(float r)
{
	m_Transform.Rotation = r;
}

Vec2 Sprite::GetSize() const
{
	return m_Size;
}

void Sprite::SetSize(float sx, float sy)
{
	m_Size = Vec2(sx, sy);
}

void Sprite::SetTint(float r, float g, float b, float a)
{
	m_Tint = Color(r, g, b, a);
}

Color Sprite::GetTint() const
{
	return m_Tint;
}

Transform Sprite::GetTransform() const
{
	return m_Transform;
}

void Sprite::Move(float dx, float dy)
{
	m_Transform.Position.X += dx;
	m_Transform.Position.Y += dy;
}

void Sprite::Rotate(float dr)
{
	m_Transform.Rotation += dr;
}

const Image* Sprite::GetImage()const
{
	return m_Image;
}

void Sprite::SetImage(Image* image)
{
	m_Image = image;
}

void Sprite::SetImageScaleBias(float su, float sv, float bu, float bv)
{
	m_ImageScale = Vec2(su, sv);
	m_ImageBias = Vec2(bu, bv);
}

void Sprite::SetImageScaleBias(Vec2 scale, Vec2 bias)
{
	m_ImageScale = scale;
	m_ImageBias = bias;
}

Vec2 Sprite::GetImageScale() const
{
	return m_ImageScale;
}

Vec2 Sprite::GetImageBias() const
{
	return m_ImageBias;
}

void Sprite::SetIsUI(bool ui)
{
	m_IsUI = ui;
}

bool Sprite::GetIsUI()const
{
	return m_IsUI;
}
