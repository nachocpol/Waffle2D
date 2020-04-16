#pragma once

namespace Waffle
{
	struct InputState
	{
		struct Key
		{
			enum T
			{
				Up,
				Down,
				Left,
				Right,
				Space,
				ESC,
				Enter,
				COUNT
			};
		};
		bool KeyState[Key::COUNT];
		bool MouseButtons[3]; // Left, middle, right
		int MouseX;
		int MouseY;
	};

	class Input
	{
	private:
		Input();
		Input(const Input& other) {};
		~Input() {};
	public:
		static Input& Get();
		void SetState(InputState state);
		bool GetKeyPressed(InputState::Key::T key);
		bool GetMouseButton(int idx)const;
		int GetMouseX()const;
		int GetMouseY()const;

	private:
		InputState m_state;
	};
}