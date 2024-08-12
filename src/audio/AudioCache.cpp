#include "AudioCache.h"
#include "audio/AudioDecoderManager.h"

NS_OCF_BEGIN

AudioCache::AudioCache()
    : m_state(State::Inital)
    , m_alBufferId(AL_INVALID)
{
}

AudioCache::~AudioCache()
{
}

void AudioCache::readDate()
{
    AudioDecoder* pAudioDecoder = AudioDecoderManager::createDecoder(m_fileFullPath);
    AudioDecoderManager::destoroyDecoder(pAudioDecoder);
}

NS_OCF_END
