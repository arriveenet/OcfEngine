#include "AudioDecoderOgg.h"

NS_OCF_BEGIN

bool AudioDecoderOgg::open(std::string_view filename)
{
    if (ov_fopen(filename.data(), &m_vorbisFile) == 0) {
        vorbis_info* info = ov_info(&m_vorbisFile, -1);
        if (info) {
            m_sampleRate = info->rate;
            m_channelCount = info->channels;
            m_bytesPerBlock = info->channels * sizeof(short);
            m_totalFrames = static_cast<uint32_t>(ov_pcm_total(&m_vorbisFile, -1));
            m_isOpened = true;
            return true;
        } else {
            OCFLOG("Failed to get Ogg Vorbis info for file: %s", filename.data());
        }
    } else {
        OCFLOG("Failed to open Ogg Vorbis file: %s", filename.data());
    }

    return false;
}

void AudioDecoderOgg::close()
{
    if (isOpened()) {
        ov_clear(&m_vorbisFile);
        m_isOpened = false;
    }
}

uint32_t AudioDecoderOgg::read(uint32_t framesToRead, char* pcmBuffer)
{
    int currentSection = 0;
    int bytesToread = framesToBytes(framesToRead);
    int32_t bytesRead = ov_read(&m_vorbisFile, pcmBuffer, bytesToread, 0, 2, 1, &currentSection);
    return bytesToFrames(bytesRead);
}

bool AudioDecoderOgg::seek(uint32_t frameOffset)
{
    return (ov_raw_seek(&m_vorbisFile, frameOffset) == 0);
}

AudioDecoderOgg::AudioDecoderOgg()
    : m_vorbisFile()
{
}

AudioDecoderOgg::~AudioDecoderOgg()
{
    close();
}

NS_OCF_END
