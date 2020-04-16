#pragma once

#include "MathUtils.h"

namespace Waffle
{
	class Sprite
	{
	public:
		Sprite() {};
		Sprite(float sx, float sy); // Size in pixels.
		~Sprite() {};
		void SetPosition(float x, float y);
		void SetScale(float x, float y);
		// Rotation in degrees
		void SetRotation(float r);
		void SetTint(float r, float g, float b, float a = 1.0f);
		Vec2 GetSize()const;
		Transform GetTransform()const;

	private:
		Vec2 m_size;
		Transform m_transform;
		Color m_tint;
	};
}