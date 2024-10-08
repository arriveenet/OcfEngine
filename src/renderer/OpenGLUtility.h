#pragma once
#include "OpenGLInclude.h"
#include "Types.h"
#include "base/Macros.h"

NS_OCF_BEGIN

struct OpenGLUtility {
    static GLenum toGLPrimitive(PrimitiveType primitiveType);
    static GLenum toGLFormat(PixelFormat format);
    static GLsizei getGLDataTypeSize(GLenum type);
};

NS_OCF_END
