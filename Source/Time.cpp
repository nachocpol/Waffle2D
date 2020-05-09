#include "Time.h"

#include <Windows.h>

using namespace Waffle;

static long long gSysFrequency = -1;

Timer::Timer() :
	m_Started(false)
	, m_Start(0)
{
	if (gSysFrequency == -1)
	{
		LARGE_INTEGER freq;
		QueryPerformanceFrequency(&freq);
		gSysFrequency = freq.QuadPart;
	}
}

Timer::~Timer()
{
}

void Timer::Start()
{
	// According with mcdoc, from WinXP this should never return false/fail
	LARGE_INTEGER startStamp;
	QueryPerformanceCounter(&startStamp);
	m_Start = startStamp.QuadPart;
}

float Timer::Stop()
{
	LARGE_INTEGER endStamp;
	LARGE_INTEGER elapsed;
	QueryPerformanceCounter(&endStamp);

	elapsed.QuadPart = endStamp.QuadPart - m_Start;
	elapsed.QuadPart *= 1000000;
	elapsed.QuadPart /= gSysFrequency;

	// from microseconds to ms
	return (float)elapsed.QuadPart / 1000.0f;
}