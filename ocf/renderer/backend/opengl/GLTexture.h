#pragma once

#include "renderer/backend/DriverBase.h"

#include "OpenGLInclude.h"

namespace ocf::backend {

struct GLTexture : public HwTexture {
    struct GL {
        GLuint id = 0;
        GLenum target = 0;
    }gl;
};

}; // namespace ocf::backend
