#include "AudioBuffer.h"
#include "FileUtils.h"
#include "Logging.h"

#include "stb_vorbis.c"

#include "al.h"

using namespace Waffle;

AudioBuffer::AudioBuffer()
{
}

AudioBuffer::~AudioBuffer()
{
}

AudioBuffer* AudioBuffer::CreateFromOGG(const char* path)
{
	std::string fixedPath(path);
	if (!FileSystem::Get().FixupPath(fixedPath))
	{
		ERR("Failed to fixup the path: %s", path);
		return nullptr;
	}

	AudioBuffer* audioBuffer = new AudioBuffer;

	int err = 0;
	stb_vorbis* vorbis = stb_vorbis_open_file(fopen(fixedPath.c_str(), "rb"), 1, &err, nullptr);
	alGenBuffers(1, &audioBuffer->m_BufferID);
	// Finish this.
	//alBufferData(audioBuffer->m_BufferID, AL_FORMAT_MONO8, vorbis->samples_output, vorbis->stream_len, vorbis->sample_rate);

	return audioBuffer;
}
