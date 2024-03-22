#pragma once
#include "OpenGLInclude.h"
#include "Types.h"
#include "base/Macros.h"

OCF_BEGIN

struct OpenGLUtility {
	static GLenum toGLPrimitive(PrimitiveType primitiveType);
	static GLenum toGLFormat(PixelFormat format);
};

OCF_END
