#include "Graphics.h"
#include "Sprite.h"
#include "Input.h"
#include "Image.h"

using namespace Waffle;

int main()
{
	Graphics& graphics = Graphics::Get();
	graphics.Init();

	Image* img = Image::CreateFromFile("../../Data/Waffle/Img/ColorChecker.png");
	Sprite spriteImg(256, 128, img);

	while (!graphics.Closed())
	{
		graphics.ClearScreen(0.5f, 0.5f, 0.5f, 1.0f);
		graphics.DrawSprite(&spriteImg);
		graphics.Flush();
	}
	return 0;
}