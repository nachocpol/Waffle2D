#include "AudioSource.h"
#include "AudioBuffer.h"

#include "al.h"

using namespace Waffle;

AudioSource::AudioSource()
	:m_Buffer()
	, m_SourceID(0)
{
}

AudioSource::~AudioSource()
{
}

void AudioSource::AttachBuffer(AudioBuffer* buffer)
{
}

void AudioSource::Play()
{
	if (m_Buffer)
	{
		// TO-DO: finish this
		//alSourcePlay(m_SourceID);
	}
}
