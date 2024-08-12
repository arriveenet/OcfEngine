#pragma once
#include "platform/PlatformMacros.h"
#include <string>
#include <AL/al.h>

NS_OCF_BEGIN

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

    State m_state;
    ALuint m_alBufferId;
    std::string m_fileFullPath;

    friend class AudioEngineImpl;
};

NS_OCF_END