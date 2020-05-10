#pragma once

namespace Waffle
{
	class AudioSource;
	class Audio
	{
	private:
		Audio() {}
		Audio(const Audio& other) {}
		~Audio() {}

	public:
		static Audio& Get();
		bool Init();
		static AudioSource* CreateSource();

	private:
		void* m_AudioDevice;
		void* m_AudioContext;
	};
}