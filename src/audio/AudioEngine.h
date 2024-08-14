#pragma once
#include <string>
#include <memory>
#include <unordered_map>
#include "platform/PlatformMacros.h"

NS_OCF_BEGIN

class AudioEngineImpl;

using AUDIO_ID = int;

class AudioEngine {
public:
	static const int AUDIO_ID_INVALID;
	static const float TIME_UNKNOWN;

	static bool lazyInit();
	static void end();

	static AUDIO_ID play(std::string_view filename, bool loop = false, float volume = 1.0f);

	static void stop(AUDIO_ID audioID);

private:
	struct AudioInfo {
		std::string filePath;

		float volume;
		bool loop;
		float duration;

		AudioInfo();
		~AudioInfo();
	};

	static std::unordered_map<AUDIO_ID, AudioInfo> m_audioIdInfoMap;

	static AudioEngineImpl* m_pAudioEngineImpl;
};

NS_OCF_END