#include "AudioEngineImpl.h"
#include <AL/al.h>
#include <AL/alc.h>
#include "audio/AudioDecoderManager.h"
#include "base/Console.h"
#include "base/FileUtils.h"

NS_OCF_BEGIN

static ALCdevice* s_pALDevice = nullptr;
static ALCcontext* s_pALContext = nullptr;

AudioEngineImpl::AudioEngineImpl()
    : m_alSources()
    , m_currentAudioID(0)
{
}

AudioEngineImpl::~AudioEngineImpl()
{
    for (auto&& player : m_audioPlayers) {
        player.second->destroy();
    }

    if (s_pALContext != nullptr) {
        alDeleteSources(AUDIO_SOURCE_MAX, m_alSources);

        m_audioCaches.clear();

        alcMakeContextCurrent(nullptr);
        alcDestroyContext(s_pALContext);
        s_pALContext = nullptr;
    }

    if (s_pALDevice != nullptr) {
        alcCloseDevice(s_pALDevice);
        s_pALDevice = nullptr;
    }

    AudioDecoderManager::destroy();
}

bool AudioEngineImpl::init()
{
    bool result = false;
    do {
        s_pALDevice = alcOpenDevice(nullptr);

        if (s_pALDevice != nullptr) {
            s_pALContext = alcCreateContext(s_pALDevice, nullptr);
            alcMakeContextCurrent(s_pALContext);

            alGenSources(AUDIO_SOURCE_MAX, m_alSources);
            ALenum alError = alGetError();
            if (alError != AL_NO_ERROR) {
                break;
            }

            for (int i = 0; i < AUDIO_SOURCE_MAX; i++) {
                m_unusedSourcesPool.push(m_alSources[i]);
            }

            AudioDecoderManager::init();

            const char* vender = alGetString(AL_VENDOR);
            const char* version = alGetString(AL_VERSION);

            OCFLOG("OpenAL initialization successful. vender: %s, version: %s\n", vender, version);

            result = true;
        }
    } while (false);

    return result;
}

AUDIO_ID AudioEngineImpl::play(std::string_view fileFullPath)
{
    if (s_pALDevice == nullptr) {
        return AudioEngine::AUDIO_ID_INVALID;
    }

    ALuint alSource = findSource();
    if (alSource == AL_INVALID) {
        return AudioEngine::AUDIO_ID_INVALID;
    }

    AudioPlayer* pAudioPlayer = new AudioPlayer();

    pAudioPlayer->m_alSource = alSource;

    AudioCache* pAudioCache = preload(fileFullPath);
    if (pAudioCache == nullptr) {
        delete pAudioPlayer;
        return AudioEngine::AUDIO_ID_INVALID;
    }

    pAudioPlayer->setCache(pAudioCache);
    m_audioPlayers.emplace(++m_currentAudioID, pAudioPlayer);

    pAudioPlayer->play();

    return m_currentAudioID;
}

void AudioEngineImpl::stop(AUDIO_ID audioID)
{
    auto iter = m_audioPlayers.find(audioID);
    if (iter != m_audioPlayers.end()) {
        AudioPlayer* player = iter->second;
        player->destroy();
    }
}

AudioCache* AudioEngineImpl::preload(std::string_view filePath)
{
    AudioCache* pAudioCach = nullptr;

    auto iter = m_audioCaches.find(filePath.data());
    if (iter == m_audioCaches.end()) {
        pAudioCach = new AudioCache();
        m_audioCaches.emplace(filePath.data(), std::unique_ptr<AudioCache>(pAudioCach));

        pAudioCach->m_fileFullPath = FileUtils::getInstance()->fullPathForFilename(filePath.data());

        pAudioCach->readDate();
    }
    else {
        pAudioCach = iter->second.get();
    }

    return pAudioCach;
}

ALuint AudioEngineImpl::findSource()
{
    ALuint sourceId = AL_INVALID;
    if (!m_unusedSourcesPool.empty()) {
        sourceId = m_unusedSourcesPool.front();
        m_unusedSourcesPool.pop();
    }

    return sourceId;
}

NS_OCF_END
