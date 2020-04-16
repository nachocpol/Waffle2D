#include "Graphics.h"
#include "Sprite.h"
#include "Input.h"
#include "Image.h"

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

	Image* img = Image::CreateFromFile("../../Data/Waffle/Img/DefaultTest.png");
	Sprite spriteImg(128, 128, img);

	while (true)
	{
		graphics.ClearScreen(0.5f, 0.5f, 0.5f, 1.0f);
		graphics.DrawSprite(&spriteImg);
		if(Input::Get().GetKeyPressed(InputState::Key::Space))
		{
 			rot += 10.0;
			sprite.SetRotation(rot);
		}
		graphics.Flush();
	}
	return 0;
}