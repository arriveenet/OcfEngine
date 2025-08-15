#pragma once
#include "OpenGLInclude.h"
#include "Renderer/backend/DriverEnums.h"
#include "base/Macros.h"
#include "base/Types.h"

NS_OCF_BEGIN

struct OpenGLUtility {
    static GLenum toGLPrimitive(backend::PrimitiveType primitiveType);
    static GLenum toGLFormat(PixelFormat format);
    static GLsizei getGLDataTypeSize(GLenum type);
};

NS_OCF_END
