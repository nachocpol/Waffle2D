#include "Sprite.h"

using namespace Waffle;

Sprite::Sprite(float sx, float sy)
	:m_size(sx,sy)
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

void Sprite::SetTint(float r, float g, float b, float a)
{
	m_tint = Color(r, g, b, a);
}

Vec2 Sprite::GetSize() const
{
	return m_size;
}

Transform Sprite::GetTransform() const
{
	return m_transform;
}
