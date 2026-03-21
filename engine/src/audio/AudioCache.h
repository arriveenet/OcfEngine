#pragma once
#include "audio/AudioMacros.h"
#include <AL/al.h>
#include <cstdint>
#include <string>

namespace ocf {

class AudioCache {
public:
    enum class State {
        Inital,
        Loading,
        Ready,
        Filed
    };

    AudioCache();
    ~AudioCache();

protected:
    void readDate();

    ALenum m_format;
    ALsizei m_sampleRate;
    float m_duration;
    uint32_t m_totalFrames;
    uint32_t m_framesRead;

    State m_state;
    ALuint m_alBufferId;
    std::string m_fileFullPath;

    char* m_queBuffers[QUEUEBUFFER_NUM];
    ALsizei m_queBufferSize[QUEUEBUFFER_NUM];
    uint32_t m_queBufferFrames;

    friend class AudioEngineImpl;
    friend class AudioPlayer;
};

} // namespace ocf
