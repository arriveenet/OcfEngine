#include "AudioEngine.h"
#include "audio/AudioEngineImpl.h"

NS_OCF_BEGIN

const int AudioEngine::AUDIO_ID_INVALID = -1;
const float AudioEngine::TIME_UNKNOWN = -1.0f;

std::unordered_map<AUDIO_ID, AudioEngine::AudioInfo> AudioEngine::m_audioIdInfoMap;
AudioEngineImpl* AudioEngine::m_pAudioEngineImpl = nullptr;

AudioEngine::AudioInfo::AudioInfo()
    : volume(1.0f), loop(false), duration(TIME_UNKNOWN)
{
}

AudioEngine::AudioInfo::~AudioInfo()
{
}

bool AudioEngine::lazyInit()
{
    if (m_pAudioEngineImpl == nullptr) {
        m_pAudioEngineImpl = new AudioEngineImpl();
        if (!m_pAudioEngineImpl->init()) {
            delete m_pAudioEngineImpl;
            m_pAudioEngineImpl = nullptr;
            return false;
        }
    }
    return true;
}

void AudioEngine::end()
{
    delete m_pAudioEngineImpl;
    m_pAudioEngineImpl = nullptr;
}

AUDIO_ID AudioEngine::play(std::string_view filename, bool loop, float volume)
{
    AUDIO_ID result = AudioEngine::AUDIO_ID_INVALID;

    if (!lazyInit()) {
        return 0;
    }

    volume = std::min(volume, 1.0f);
    volume = std::max(volume, 0.0f);

    result = m_pAudioEngineImpl->play(filename);
    if (result != AUDIO_ID_INVALID) {
        auto& audioInfo = m_audioIdInfoMap[result];
        audioInfo.volume = volume;
        audioInfo.loop = loop;
        audioInfo.filePath = filename;
    }

    return result;
}

void AudioEngine::stop(AUDIO_ID audioID)
{
    auto iter = m_audioIdInfoMap.find(audioID);
    if (iter != m_audioIdInfoMap.end()) {
        m_pAudioEngineImpl->stop(audioID);
    }
}

NS_OCF_END
