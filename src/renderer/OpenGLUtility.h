#pragma once
#include "OpenGLInclude.h"
#include "Renderer/backend/DriverEnums.h"
#include "base/Macros.h"
#include "base/Types.h"

namespace ocf {

struct OpenGLUtility {
    static GLenum toGLPrimitive(backend::PrimitiveType primitiveType);
    static GLenum toGLFormat(PixelFormat format);
    static GLsizei getGLDataTypeSize(GLenum type);
};

} // namespace ocf
