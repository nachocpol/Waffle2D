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

bool Waffle::Input::GetKeyPressed(InputState::Key::T key)
{
	return m_state.KeyState[key];
}

bool Waffle::Input::GetMouseButton(int idx) const
{
	return m_state.MouseButtons[idx];
}

int Waffle::Input::GetMouseX() const
{
	return m_state.MouseX;
}

int Waffle::Input::GetMouseY() const
{
	return m_state.MouseY;
}
