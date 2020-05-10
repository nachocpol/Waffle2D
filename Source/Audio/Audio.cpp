#include "Audio.h"
#include "AudioSource.h"
#include "Logging.h"

#include "al.h"
#include "alc.h"

using namespace Waffle;

Audio& Audio::Get()
{
	static Audio* k_Instance = nullptr;
	if (!k_Instance)
	{
		k_Instance = new Audio;
	}
	return *k_Instance;
}

bool Audio::Init()
{
	m_AudioDevice = alcOpenDevice(NULL);
	if (alGetError() != AL_NO_ERROR)
	{
		ERR("Failed to create OpenAL device.");
		return false;
	}
	m_AudioContext = alcCreateContext((ALCdevice*)m_AudioDevice, NULL);
	if (alGetError() != AL_NO_ERROR)
	{
		ERR("Failed to create OpenAL context.");
		return false;
	}
	alcMakeContextCurrent((ALCcontext*)m_AudioContext);
	if (alGetError() != AL_NO_ERROR)
	{
		ERR("Failed to set current context");
		return false;
	}

	INFO("Initialized Audio engine");

	return true;
}

AudioSource* Audio::CreateSource()
{
	AudioSource* src = new AudioSource;
	alGenSources(1, &src->m_SourceID);
	return src;
}
