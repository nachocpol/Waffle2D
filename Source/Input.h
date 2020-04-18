#pragma once

namespace Waffle
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
			W,
			A,
			S,
			D,
			COUNT
		};
	};

	struct InputState
	{
		bool KeyState[Key::COUNT];
		bool MouseButtons[3]; // Left, middle, right
		int MouseX;
		int MouseY;
		int MouseWheelDelta;// +1 away from user. -1 towards
	};

	class Input
	{
	private:
		Input();
		Input(const Input& other) {};
		~Input() {};
	public:
		static Input& Get();
		
		static bool GetKeyPressed(Key::T key);
		static bool GetMouseButton(int idx);
		static int GetMouseX();
		static int GetMouseY();

		void SetState(InputState state);

	private:
		InputState m_state;
	};
}