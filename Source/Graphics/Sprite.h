#pragma once

#include "Graphics/MathUtils.h"

namespace Waffle
{
	class Image;
	class Sprite
	{
	public:
		Sprite();
		Sprite(float sx, float sy);
		Sprite(float sx, float sy, Image* image);
		Sprite(const Sprite& other);
		~Sprite() {};

		void SetPosition(float x, float y);
		void SetScale(float x, float y);
		void SetRotation(float r); // Rotation in degrees
		void SetSize(float sx, float sy);
		Vec2 GetSize()const;
		Transform GetTransform()const;
		void Move(float dx, float dy);
		void Rotate(float dr);

		void SetTint(float r, float g, float b, float a = 1.0f);
		Color GetTint()const;

		const Image* GetImage()const;
		void SetImage(Image* image);

		void SetImageScaleBias(float su, float sv, float bu, float bv);
		Vec2 GetImageScale()const;
		Vec2 GetImageBias()const;

		void SetIsUI(bool ui);
		bool GetIsUI()const;

	private:
		Vec2 m_size;
		Transform m_transform;
		Color m_tint;
		Image* m_image;
		
		Vec2 m_imageScale;
		Vec2 m_imageBias;
		bool m_isUI;
	};
}