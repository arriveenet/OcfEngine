#include "AudioDecoder.h"

NS_OCF_BEGIN

bool AudioDecoder::isOpened() const
{
    return m_isOpened;
}

AudioDecoder::AudioDecoder()
    : m_isOpened(false)
{
}

AudioDecoder::~AudioDecoder()
{
}

NS_OCF_END
