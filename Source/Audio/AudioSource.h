#pragma once

namespace Waffle
{
	class Audio;
	class AudioBuffer;
	class AudioSource
	{
		friend Audio;
	private:
		AudioSource();
		AudioSource(const AudioSource& other);
		~AudioSource();

	public:
		void AttachBuffer(AudioBuffer* buffer);
		void Play();

	private:
		AudioBuffer* m_Buffer;
		unsigned int m_SourceID;
	};
}