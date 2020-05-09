#include "Graphics/Graphics.h"
#include "Graphics/Sprite.h"
#include "Graphics/Image.h"
#include "Input.h"
#include "Time.h"

using namespace Waffle;

int main()
{
	float totalTime = 0.0f;
	float deltaTime = 0.0f;

	Timer timer;
	timer.Start();

	Graphics& graphics = Graphics::Get();
	graphics.Init();

	Image* img = Image::CreateFromFile("data:Img/ColorChecker.png");
	
	// Create one sprite and assign an image:
	Sprite spriteImg(512, 256, img);
	spriteImg.SetPosition(200.0f, 50.0f);
	float rot = 0.0f;
	
	// Simple sprite (just tint)
	Sprite spriteImg2(512, 256);
	spriteImg2.SetTint(0.8f, 0.25f, 0.0f);
	spriteImg2.SetPosition(-200.0f, -50.0f);

	while (!graphics.Closed())
	{
		graphics.BeginFrame();

		timer.Start();

		rot += 25.0f * deltaTime;
		spriteImg.SetRotation(rot);
		spriteImg2.SetRotation(-rot);

		if (Input::GetKeyPressed(Key::Space))
		{
			rot = 0.0f;
		}

		graphics.ClearScreen(0.5f, 0.5f, 0.5f, 1.0f);
		graphics.DrawSprite(&spriteImg2);
		graphics.DrawSprite(&spriteImg);
		graphics.Flush();

		// Update frame time:
		deltaTime = timer.Stop() / 1000.0f;
		totalTime += deltaTime;
	}
	return 0;
}