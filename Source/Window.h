#pragma once

static void* g_hinstance = 0;
static int g_cmdShow = 0;

namespace Waffle
{
	class Window
	{
	public:
		Window();
		~Window();
		bool Init(const char* displayName);
		void* GetHandle()const;
		void Resized(int w, int h);
		int GetWidth()const;
		int GetHeight()const;
		bool Update();

	private:
		void* m_handle;
		int m_width;
		int m_height;
	};
}