#pragma once
#include <string>
#include "platform/PlatformMacros.h"

NS_OCF_BEGIN

class AudioDecoder;

class AudioDecoderManager {
public:
    static bool init();
    static void destroy();
    static AudioDecoder* createDecoder(std::string_view filePath);
    static void destoroyDecoder(AudioDecoder* decoder);
};

NS_OCF_END
