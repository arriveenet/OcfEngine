#include "Texture2D.h"
#include "Image.h"
#include "OpenGLInclude.h"
#include "OpenGLUtility.h"

NS_OCF_BEGIN

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
    glDeleteTextures(1, &m_textureId);
}

bool Texture2D::initWithFile(const std::string& filename)
{
    Image img;
    if (!img.loadImageFile(filename))
        return false;

    return initWithImage(&img, img.getPixelFormat());
}

bool Texture2D::initWithData(uint8_t* data, size_t dataSize,
    int width, int height, PixelFormat format)
{
    if (data == nullptr || width <= 0 || height <= 0) {
        return false;
    }

    Image img;
    img.initWithRawData(data, dataSize, width, height, format);

    return initWithImage(&img, format);
}

bool Texture2D::initWithImage(Image* image, PixelFormat format)
{
    if (image == nullptr || image->getData() == nullptr
        || image->getWidth() <= 0 || image->getHeight() <= 0) {
        return false;
    }

    m_width = image->getWidth();
    m_height = image->getHeight();
    m_pixelFormat = (format == PixelFormat::NONE) ? image->getPixelFormat() : format;

    const GLenum renderFormat = OpenGLUtility::toGLFormat(m_pixelFormat);
    const int alignment = getPixelAlignment(m_pixelFormat);

    glGenTextures(1, &m_textureId);

    glBindTexture(GL_TEXTURE_2D, m_textureId);

    glPixelStorei(GL_UNPACK_ALIGNMENT, alignment);

    glTexImage2D(GL_TEXTURE_2D, 0, renderFormat, m_width, m_height,
                 0, renderFormat, GL_UNSIGNED_BYTE, image->getData());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glBindTexture(GL_TEXTURE_2D, 0);

    return true;
}

void Texture2D::setActive() const
{
    glBindTexture(GL_TEXTURE_2D, m_textureId);
}

void Texture2D::updateSubData(uint8_t* data, int xoffset, int yoffset, int width, int height) const
{
    glBindTexture(GL_TEXTURE_2D, m_textureId);

    const GLenum glFormat = OpenGLUtility::toGLFormat(m_pixelFormat);
    glTexSubImage2D(GL_TEXTURE_2D, 0, xoffset, yoffset,
                    width, height, glFormat, GL_UNSIGNED_BYTE, data);

    glBindTexture(GL_TEXTURE_2D, 0);
}

int Texture2D::getWidth() const
{
    return m_width;
}

int Texture2D::getHeight() const
{
    return m_height;
}

glm::vec2 Texture2D::getSize() const
{
    glm::vec2 result(0.0f, 0.0f);
    result.x = static_cast<float>(m_width);
    result.y = static_cast<float>(m_height);

    return result;
}

NS_OCF_END
