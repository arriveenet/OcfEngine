#include "Texture2D.h"
#include "Image.h"
#include <GLFW/glfw3.h>

OCF_BEGIN

static GLenum convertPixelFormatToOpneGLFormat(PixelFormat format)
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

static int getPixelAlignment(PixelFormat format)
{
	int alignment = 1;
	switch (format) {
	case PixelFormat::GRAY:
	case PixelFormat::RGB:
		alignment = 1;
		break;
	case PixelFormat::RGBA:
		alignment = 4;
		break;
	default:
		break;
	}

	return alignment;
}

Texture2D::Texture2D()
	: m_textureId(0)
	, m_width(0)
	, m_height(0)
	, m_pixelFormat(PixelFormat::NONE)
	, m_size()
{
}

Texture2D::~Texture2D()
{
}

bool Texture2D::initWithFile(const std::string& filename)
{
	Image img;
	if (!img.loadImageFile(filename))
		return false;

	m_width = img.getWidth();
	m_height = img.getHeight();
	m_pixelFormat = img.getPixelFormat();

	const GLenum format = convertPixelFormatToOpneGLFormat(m_pixelFormat);
	const int alignment = getPixelAlignment(m_pixelFormat);
	
	glGenTextures(1, &m_textureId);

	glBindTexture(GL_TEXTURE_2D, m_textureId);

	glPixelStorei(GL_UNPACK_ALIGNMENT, alignment);

	glTexImage2D(GL_TEXTURE_2D, 0, format, m_width, m_height, 0, format, GL_UNSIGNED_BYTE, img.getData());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glBindTexture(GL_TEXTURE_2D, 0);

	return true;
}

void Texture2D::setActive() const
{
	glBindTexture(GL_TEXTURE_2D, m_textureId);
}

int Texture2D::getWidth() const
{
	return m_width;
}

int Texture2D::getHeight() const
{
	return m_height;
}

OCF_END
