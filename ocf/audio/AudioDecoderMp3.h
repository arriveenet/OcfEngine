#pragma once

#include "AudioDecoder.h"
#include <fstream>
#include <memory>

typedef struct Mp3DecImpl* Mp3DecHandle;

namespace ocf {

class AudioDecoderMp3 :public AudioDecoder {
public:
    bool open(std::string_view filename) override;

    void close() override;

    uint32_t read(uint32_t framesToRead, char* pcmBuffer) override;

    bool seek(uint32_t frameOffset) override;

protected:
    AudioDecoderMp3();
    ~AudioDecoderMp3();

private:
    Mp3DecHandle m_handle;
    std::unique_ptr<std::ifstream> m_fileStream;

    friend class AudioDecoderManager;
};

} // namespace ocf