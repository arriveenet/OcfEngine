#pragma once

#include "AudioDecoder.h"
#include <vorbis/vorbisfile.h>

NS_OCF_BEGIN

class AudioDecoderOgg : public AudioDecoder {
public:
    bool open(std::string_view filename) override;

    void close() override;

    uint32_t read(uint32_t framesToRead, char* pcmBuffer) override;

    bool seek(uint32_t frameOffset) override;

protected:
    AudioDecoderOgg();
    ~AudioDecoderOgg();

    OggVorbis_File m_vorbisFile;

    friend class AudioDecoderManager;
};

NS_OCF_END