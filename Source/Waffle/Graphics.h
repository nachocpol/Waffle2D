#pragma once

namespace Waffle
{
	class Window;
	class Sprite;
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
		void Flush();
		void ClearScreen(float r, float g, float b, float a);
		void DrawSprite(Sprite* sprite);

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
		void* m_renderContext;

		bool m_viewportDirty;
		int m_width;
		int m_height;
	};
}