#include "Noise.h"
#include "Graphics/MathUtils.h"

#include <cstdlib>
#include <cmath>

using namespace Waffle;

void Noise::SetSeed(unsigned int seed)
{
	srand(seed);
}

float Noise::Random()
{
	return (float)rand() / (float)RAND_MAX;
}

float Noise::Random1D(float p)
{
	float val = sin(p) * 43758.5453321f;
	return val - floor(val);
}

float Waffle::Noise::Random2D(float px, float py)
{
	float val = sin(Dot(Vec2(px, py), Vec2(12.8989f, 4.1414f))) * 43758.5453321f;
	return val - floor(val);
}

float Noise::ValueNoise2D(float px, float py)
{
	float maxX = ceil(px);
	float maxY = ceil(py);

	float minX = floor(px);
	float minY = floor(py);

	float dx = px - minX;
	float dy = py - minY;

	float tl = Random2D(minX, maxY);
	float tr = Random2D(maxX, maxY);

	float bl = Random2D(minX, minY);
	float br = Random2D(maxX, minY);

	float lv = quintic(bl, tl, dy);
	float rb = quintic(br, tr, dy);

	return quintic(lv, rb, dx);
}
