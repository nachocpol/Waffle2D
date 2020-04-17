#include "Window.h"
#include "Graphics.h"
#include "Input.h"

#include <string>

#include <windows.h>
#include <Windowsx.h>

using namespace Waffle;

InputState g_inputState;

static void KeyUpdate(WPARAM key, bool pressed)
{
	switch (key)
	{
		case VK_ESCAPE:
		{
			g_inputState.KeyState[Key::ESC] = pressed;		break;
		}
		case VK_LEFT:
		{
			g_inputState.KeyState[Key::Left] = pressed;		break;
		}
		case VK_RIGHT:
		{
			g_inputState.KeyState[Key::Right] = pressed;	break;
		}
		case VK_UP:
		{
			g_inputState.KeyState[Key::Up] = pressed;		break;
		}
		case VK_DOWN:
		{
			g_inputState.KeyState[Key::Down] = pressed;		break;
		}
		case VK_RETURN:
		{
			g_inputState.KeyState[Key::Enter] = pressed;	break;
		}
		case VK_SPACE:
		{
			g_inputState.KeyState[Key::Space] = pressed;	break;
		}
	}
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	Window* owner = (Window*)GetWindowLongPtrA(hWnd, -21);

	switch (message)
	{
		case WM_DESTROY:
		{
			break;
		}
		case WM_SIZE:
		{
			int w = LOWORD(lParam);  
			int h = HIWORD(lParam); 
			if (owner) // On creation this is not set.
			{
				// TODO: minimize sends 0,0
				owner->Resized(w, h);
			}
			break;
		}
		case WM_MOUSEWHEEL:
		{
			int delta = GET_WHEEL_DELTA_WPARAM(wParam);
			g_inputState.MouseWheelDelta = delta < 0 ? -1 : 1;
			break;
		}
		case WM_LBUTTONDOWN:
		{
			g_inputState.MouseButtons[0] = true;	break;
		}
		case WM_LBUTTONUP:
		{
			g_inputState.MouseButtons[0] = false;	break;
		}
		case WM_MBUTTONDOWN:
		{
			g_inputState.MouseButtons[1] = true;	break;
		}
		case WM_MBUTTONUP:
		{
			g_inputState.MouseButtons[1] = false;	break;
		}
		case WM_RBUTTONDOWN:
		{
			g_inputState.MouseButtons[2] = true;	break;
		}
		case WM_RBUTTONUP:
		{
			g_inputState.MouseButtons[2] = false;	break;
		}
		case WM_KEYDOWN:
		{
			KeyUpdate(wParam, true);				break;
		}
		case WM_KEYUP:
		{
			KeyUpdate(wParam, false);				break;
		}
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}

Window::Window()
	: m_handle(nullptr)
	, m_width(1280)
	, m_height(720)

{
}

Window::~Window()
{
}

bool Window::Init(const char* displayName)
{
	if (m_handle)
	{
		return true;
	}

	std::string strName(displayName);
	std::wstring wstrName(strName.begin(), strName.end());

	WNDCLASS wc = {};
	wc.lpfnWndProc = WndProc;
	wc.hInstance = GetModuleHandle(NULL);
	wc.hbrBackground = (HBRUSH)(COLOR_BACKGROUND);
	wc.lpszClassName = wstrName.c_str();
	wc.style = CS_OWNDC;
	if (!RegisterClass(&wc))
	{
		return false;
	}

	ULONG  style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_VISIBLE;
	style = WS_OVERLAPPEDWINDOW | WS_MAXIMIZE;
	m_handle = CreateWindowW(wc.lpszClassName, wstrName.c_str(), style, 0, 0, m_width, m_height, 0, 0, wc.hInstance, 0);
	SetWindowLongPtr((HWND)m_handle, -21, (LONG_PTR)this); // GWL_USERDATA
	if (!m_handle)
	{
		return false;
	}

	ShowWindow((HWND)m_handle, SW_SHOWMAXIMIZED);
	UpdateWindow((HWND)m_handle);

	return true;
}

void* Window::GetHandle() const
{
	return m_handle;
}

void Window::Resized(int w, int h)
{
	m_width = w;
	m_height = h;
	Graphics::Get().OnResize(w, h);
}

int Window::GetWidth() const
{
	return m_width;
}

int Window::GetHeight() const
{
	return m_height;
}

bool Window::Update()
{
	// Reset some state:
	g_inputState.MouseWheelDelta = 0;

	MSG msg = {};
	while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		if (msg.message == WM_QUIT)
		{
			break;
		}
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	// Query mouse state:
	POINT cursor;
	if (GetCursorPos(&cursor))
	{
		g_inputState.MouseX = cursor.x;
		g_inputState.MouseY = cursor.y;
	}

	Input::Get().SetState(g_inputState);
	return true;
}
