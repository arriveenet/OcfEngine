#pragma once
#include "renderer/backend/opengl/OpenGLInclude.h"
#include "ocf/renderer/backend/DriverEnums.h"
#include "ocf/core/Macros.h"
#include "ocf/core/Types.h"

namespace ocf {

struct OpenGLUtility {
    static GLenum toGLPrimitive(backend::PrimitiveType primitiveType);
    static GLenum toGLFormat(PixelFormat format);
    static GLsizei getGLDataTypeSize(GLenum type);
};

} // namespace ocf
