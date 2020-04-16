#include "Graphics.h"
#include "Sprite.h"
#include "Input.h"

using namespace Waffle;

int main()
{
	Graphics& graphics = Graphics::Get();
	graphics.Init();

	float rot = -45.0f;
	Sprite sprite(128, 1057);
	sprite.SetRotation(-45.0f);
	sprite.SetScale(0.5f, 0.5f);
	sprite.SetPosition(256, 0);

	while (true)
	{
		graphics.ClearScreen(0.5f, 0.5f, 0.5f, 1.0f);
		graphics.DrawSprite(&sprite);
		if(Input::Get().GetKeyPressed(InputState::Key::Space))
		{
 			rot += 10.0;
			sprite.SetRotation(rot);
		}
		graphics.Flush();
	}
	return 0;
}