#pragma once

#include "Graphics/MathUtils.h"

#include <vector>

namespace Waffle
{
	class Window;
	class Sprite;
	class Image;
	class Font;
	struct BatchManager;
	class Graphics
	{
		friend BatchManager;
	private:
		Graphics();
		Graphics(const Graphics& other);
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
		void DrawTextString(const char* text, Font* font, Vec2 position, Color color = Color(1.0f, 1.0f, 1.0f, 1.0f));
		void SetView(Vec2 view);
		Vec2 GetView()const;
		Vec2 GetCurViewport();
		void SetRenderScale(float scale);
		float GetRenderScale()const;

	private:
		bool InitResources();
		struct DrawCallInfo
		{
			Mat3 WorldTransform;
			Vec2 Projection;
			unsigned int ImageID;
			float ImageScaleBias[4];
			Color Tint;
			bool IsFont;
		};
		void SubmitDrawCall(const DrawCallInfo& drawCall);
		unsigned int CreatePipeline(const char* vs, const char* ps);

		unsigned int m_SpritePipeline;
		unsigned int m_SpriteInstancedPipeline;
		struct Mesh
		{
			unsigned int ID;
			unsigned int VertexBufferID;
		}m_SpriteMesh;

		struct BatchManager
		{
			BatchManager(Graphics* graphics);
			void Push(const DrawCallInfo& dc);
			void Flush();
			void SwapBuffer();
			bool Enabled = true;
			int DrawsPerBatch = 4096;
			int MaxBatches = 128;
		private:
			struct PerBatchData
			{
				float Transform[4][4]; // Make this a mat4 to avoid vec3 weirdness
				float ScaleBias[4];
				float Tint[4];
			};
			int m_ItemCount;	// Item count inside each batch
			int m_BatchIndex;	// Current batch index
			int m_BufferIndex;
			unsigned int* m_BatchBuffers[2];
			Graphics* m_Graphics;
			std::vector<PerBatchData> m_Batch;
			DrawCallInfo m_CurItem;
		}m_BatchManager;

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