#pragma once
#include <string>
#include "platform/PlatformMacros.h"

NS_OCF_BEGIN

class AudioDecoder {
public:
    virtual bool open(std::string_view filename) = 0;

    virtual void close() = 0;

    virtual bool isOpened() const;

protected:
    AudioDecoder();
    virtual ~AudioDecoder();

    bool m_isOpened;

    friend class AudioDecoderManager;
};

NS_OCF_END
