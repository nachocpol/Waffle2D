#pragma once

namespace Waffle
{
	class Timer
	{
	public:
		Timer();
		~Timer();
		void Start();
		// Returns elapsed time from the start point. Time in ms
		float Stop();
	private:
		bool mStarted;
		long long mStart;
	};
}