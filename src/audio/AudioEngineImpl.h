#pragma once
#include <map>
#include <memory>
#include <queue>
#include "audio/AudioEngine.h"
#include "audio/AudioCache.h"

#define AUDIO_SOURCE_MAX	32

NS_OCF_BEGIN

class AudioEngineImpl {
public:
    AudioEngineImpl();
    ~AudioEngineImpl();

    bool init();
    AUDIO_ID play(std::string_view fileFullPath);

    AudioCache* preload(std::string_view filePath);

private:
    ALuint findSource();

    ALuint m_alSources[AUDIO_SOURCE_MAX];
    std::queue<ALuint> m_unusedSourcesPool;
    std::map<std::string, std::unique_ptr<AudioCache>> m_audioCaches;
};

NS_OCF_END