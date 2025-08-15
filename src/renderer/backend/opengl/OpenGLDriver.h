#pragma once
#include "renderer/backend/DriverBase.h"
#include "GLTexture.h"
#include "OpenGLContext.h"
#include <string>

namespace ocf::backend {

class OpenGLDriver : public DriverBase {
    OpenGLDriver();
    ~OpenGLDriver() override;

public:
    static OpenGLDriver *create();

    std::string getVenderString() const;
    std::string getRendererString() const;

    TextureHandle createTexture(SamplerType target, uint8_t levels, TextureFormat format,
                                uint32_t width, uint32_t height, uint32_t depth) override;

private:
    OpenGLContext m_context;
};

}; // namespace ocf::backend