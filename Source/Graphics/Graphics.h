#pragma once

#include "Graphics/MathUtils.h"

namespace Waffle
{
	class Window;
	class Sprite;
	class Image;
	class Graphics
	{
	private:
		Graphics();
		Graphics(const Graphics& other) {};
		~Graphics();

	public:
		static Graphics& Get();
		bool Init();
		void OnResize(int w, int h);
		bool Closed()const;
		void BeginFrame();
		void Flush();
		void ClearScreen(float r, float g, float b, float a);
		void DrawSprite(Sprite* sprite);
		void SetView(Vec2 view);
		Vec2 GetView()const;
		Vec2 GetCurViewport();
		void SetRenderScale(float scale);

	private:
		bool InitResources();

		unsigned int m_SpritePipeline;
		struct Mesh
		{
			unsigned int ID;
			unsigned int VertexBufferID;
		}m_SpriteMesh;

		bool m_Initialized;
		Window* m_Window;
		bool m_WindowClosed;
		void* m_RenderContext;

		bool m_ViewportDirty;
		int m_Width;
		int m_Height;

		float m_RenderScale;
		Vec2 m_View;

		Image* m_WhiteImage;
	};
}