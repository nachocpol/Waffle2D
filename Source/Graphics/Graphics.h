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

		unsigned int m_spritePipeline;
		struct Mesh
		{
			unsigned int ID;
			unsigned int VertexBufferID;
		}m_spriteMesh;

		bool m_initialized;
		Window* m_window;
		bool m_windowClosed;
		void* m_renderContext;

		bool m_viewportDirty;
		int m_width;
		int m_height;

		float m_renderScale;
		Vec2 m_view;

		Image* m_whiteImage;
	};
}