#pragma once
#include <thread>
#include <condition_variable>
#include <mutex>
#include <AL/al.h>
#include <AL/alc.h>
#include "audio/AudioMacros.h"

NS_OCF_BEGIN

class AudioCache;

class AudioPlayer {
public:
    AudioPlayer();
    ~AudioPlayer();

    void destroy();

    bool setLoop(bool loop);

protected:
    void setCache(AudioCache* cache);
    void rotateBufferThread(int offsetFrame);
    bool play();

    AudioCache* m_pAudioCache;
    ALuint m_alSource;
    ALuint m_bufferIds[QUEUEBUFFER_NUM];

    std::thread* m_rotateBufferThread;
    std::condition_variable m_sleepCondition;
    std::mutex m_sleepMutex;
    bool m_isRotateThreadExited;

    float m_currentTime;
    bool m_loop;
    bool m_isDestory;
    bool m_streamingSource;

    friend class AudioEngineImpl;
};

NS_OCF_END
