#include "AudioDecoderManager.h"
#include <filesystem>
#include "audio/AudioDecoderWav.h"

NS_OCF_BEGIN

bool AudioDecoderManager::init()
{
    return true;
}

void AudioDecoderManager::destroy()
{
}

AudioDecoder* AudioDecoderManager::createDecoder(std::string_view filePath)
{
    std::filesystem::path p(filePath);
    const auto extension = p.extension();
    if (extension == ".wav") {
        return new AudioDecoderWav();
    }
    else {

    }

    return nullptr;
}

void AudioDecoderManager::destoroyDecoder(AudioDecoder* decoder)
{
    if (decoder != nullptr) {
        decoder->close();
    }
    delete decoder;
}

NS_OCF_END