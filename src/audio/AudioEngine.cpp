#include "AudioEngine.h"
#include "audio/AudioEngineImpl.h"

NS_OCF_BEGIN

const int AudioEngine::AUDIO_ID_INVALID = -1;

AudioEngineImpl* AudioEngine::m_pAudioEngineImpl = nullptr;

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

AUDIO_ID AudioEngine::play(std::string_view filename)
{
    if (!lazyInit()) {
        return 0;
    }

    m_pAudioEngineImpl->play(filename);

    return 0;
}

NS_OCF_END