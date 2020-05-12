#include "Input.h"

#include <string.h>

using namespace Waffle;

Input::Input()
{
	memset(&m_State, 0, sizeof(m_State));
}

Input& Input::Get()
{
	static Input* kInstance = nullptr;
	if (!kInstance)
	{
		kInstance = new Input;
	}
	return *kInstance;
}

void Input::SetState(InputState state)
{
	m_State = state;
}

bool Input::GetKeyPressed(Key::T key)
{
	return Get().m_State.KeyState[key];
}

bool Input::GetMouseButton(int idx)
{
	return Get().m_State.MouseButtons[idx];
}

int Input::GetMouseX()
{
	return Get().m_State.MouseX;
}

int Input::GetMouseY()
{
	return Get().m_State.MouseY;
}

int Input::GetMouseWheelDelta()
{
	return Get().m_State.MouseWheelDelta;
}
