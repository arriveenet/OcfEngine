#pragma once
#include <map>
#include <unordered_map>
#include <memory>
#include <queue>
#include "audio/AudioEngine.h"
#include "audio/AudioPlayer.h"
#include "audio/AudioCache.h"

#define AUDIO_SOURCE_MAX	32

NS_OCF_BEGIN

class AudioEngineImpl {
public:
    AudioEngineImpl();
    ~AudioEngineImpl();

    bool init();
    AUDIO_ID play(std::string_view fileFullPath);
    void stop(AUDIO_ID audioID);

    AudioCache* preload(std::string_view filePath);

private:
    ALuint findSource();

    ALuint m_alSources[AUDIO_SOURCE_MAX];
    std::queue<ALuint> m_unusedSourcesPool;
    std::map<std::string, std::unique_ptr<AudioCache>> m_audioCaches;
    std::unordered_map<AUDIO_ID, AudioPlayer*> m_audioPlayers;

    AUDIO_ID m_currentAudioID;
};

NS_OCF_END