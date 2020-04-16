#pragma once

#include "MathUtils.h"

namespace Waffle
{
	class Image;
	class Sprite
	{
	public:
		Sprite();
		Sprite(float sx, float sy);
		Sprite(float sx, float sy, Image* image); 
		~Sprite() {};

		void SetPosition(float x, float y);
		void SetScale(float x, float y);
		void SetRotation(float r); // Rotation in degrees
		void SetSize(float sx, float sy);
		Vec2 GetSize()const;
		Transform GetTransform()const;
		
		void SetTint(float r, float g, float b, float a = 1.0f);
		Color GetTint()const;

		const Image* GetImage()const;
		void SetImage(Image* image);

	private:
		Vec2 m_size;
		Transform m_transform;
		Color m_tint;
		Image* m_image;
	};
}