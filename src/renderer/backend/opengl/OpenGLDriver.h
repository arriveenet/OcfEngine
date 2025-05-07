#pragma once
#include "renderer/backend/DriverBase.h"
#include "renderer/backend/opengl/OpenGLContext.h"
#include <string>

namespace ocf::backend {

class OpenGLDriver : public DriverBase {
    OpenGLDriver();
    ~OpenGLDriver() override;

public:
    static OpenGLDriver *create();

    std::string getVenderString() const;
    std::string getRendererString() const;

private:
    OpenGLContext m_context;
};

}; // namespace ocf::backend