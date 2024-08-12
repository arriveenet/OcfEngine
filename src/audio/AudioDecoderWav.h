#pragma once
#include "audio/AudioDecoder.h"

NS_OCF_BEGIN

class AudioDecoderWav : public AudioDecoder {
public:
    bool open(std::string_view filename) override;

    void close() override;

protected:
    AudioDecoderWav();
    ~AudioDecoderWav();

    friend class AudioDecoderManager;
};

NS_OCF_END