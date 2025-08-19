#pragma once
#include "OpenGLInclude.h"
#include "Types.h"
#include "base/Macros.h"

namespace ocf {

struct OpenGLUtility {
    static GLenum toGLPrimitive(PrimitiveType primitiveType);
    static GLenum toGLFormat(PixelFormat format);
    static GLsizei getGLDataTypeSize(GLenum type);
};

} // namespace ocf
