#include "OpenGLUtility.h"

OCF_BEGIN

GLenum OpenGLUtility::toGLPrimitive(PrimitiveType primitiveType)
{
	GLenum result = GL_TRIANGLES;
	switch (primitiveType) {
	case ocf::PrimitiveType::Point:
		result = GL_POINTS;
		break;
	case ocf::PrimitiveType::Line:
		result = GL_LINES;
		break;
	case ocf::PrimitiveType::Triangle:
		result = GL_TRIANGLES;
		break;
	case ocf::PrimitiveType::Max:
		break;
	default:
		break;
	}

	return result;
}

GLenum OpenGLUtility::toGLFormat(PixelFormat format)
{
	GLenum result = 0;

	switch (format) {
	case PixelFormat::GRAY:
		result = GL_LUMINANCE;
		//result = GL_ALPHA;
		break;
	case PixelFormat::RGB:
		result = GL_RGB;
		break;
	case PixelFormat::RGBA:
		result = GL_RGBA;
		break;
	default:
		assert(false);
		break;
	}

	return result;
}

OCF_END
