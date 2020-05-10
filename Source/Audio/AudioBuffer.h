#pragma once

namespace Waffle
{
	class AudioBuffer
	{
	public:
		AudioBuffer();
		~AudioBuffer();
		static AudioBuffer* CreateFromOGG(const char* path);

	private:
		unsigned int m_BufferID;
	};
}