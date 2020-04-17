#include "Input.h"

#include <string.h>

using namespace Waffle;

Input::Input()
{
	memset(&m_state, 0, sizeof(m_state));
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
	m_state = state;
}

bool Input::GetKeyPressed(Key::T key)
{
	return Get().m_state.KeyState[key];
}

bool Input::GetMouseButton(int idx)
{
	return Get().m_state.MouseButtons[idx];
}

int Input::GetMouseX()
{
	return Get().m_state.MouseX;
}

int Input::GetMouseY()
{
	return Get().m_state.MouseY;
}
