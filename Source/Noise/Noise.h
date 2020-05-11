#pragma once

namespace Waffle
{
	class Noise
	{
	public:
		static void SetSeed(unsigned int seed);
		static float Random();
		
		static float Random1D(float p);
		static float Random2D(float px, float py);

		static float ValueNoise2D(float px, float py);
	};
}