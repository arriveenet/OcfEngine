#pragma once
#include <string>
#include <memory>
#include "platform/PlatformMacros.h"

NS_OCF_BEGIN

class AudioEngineImpl;

using AUDIO_ID = int;

class AudioEngine {
public:
	static const int AUDIO_ID_INVALID;

	static bool lazyInit();
	static void end();

	static AUDIO_ID play(std::string_view filename);

private:
	static AudioEngineImpl* m_pAudioEngineImpl;
};

NS_OCF_END