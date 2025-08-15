#include "TextureCube.h"
#include "base/FileUtils.h"
#include "renderer/Image.h"
#include "backend/opengl/OpenGLInclude.h"
#include "backend/opengl/OpenGLUtility.h"

NS_OCF_BEGIN

using namespace backend;

TextureCube* TextureCube::create(std::string_view positive_x,
                                 std::string_view negative_x,
                                 std::string_view positive_y,
                                 std::string_view negative_y,
                                 std::string_view positive_z,
                                 std::string_view negative_z)
{
    TextureCube* textureCube = new TextureCube();
    if (textureCube->init(positive_x, negative_x, positive_y, negative_y, positive_z, negative_z))
    {
        textureCube->autorelease();
        return textureCube;
    }
    OCF_SAFE_DELETE(textureCube);
    return nullptr;
}

TextureCube::TextureCube()
    : m_imageFiles(6)
    , m_textureId(0)
{
}


TextureCube::~TextureCube()
{
    glDeleteTextures(1, &m_textureId);
}

bool TextureCube::init(std::string_view positive_x,
                       std::string_view negative_x,
                       std::string_view positive_y,
                       std::string_view negative_y,
                       std::string_view positive_z,
                       std::string_view negative_z)
{
    m_imageFiles[0] = positive_x;
    m_imageFiles[1] = negative_x;
    m_imageFiles[2] = positive_y;
    m_imageFiles[3] = negative_y;
    m_imageFiles[4] = positive_z;
    m_imageFiles[5] = negative_z;

    std::vector<Image*> images(6);
    images[0] = createImage(positive_x);
    images[1] = createImage(negative_x);
    images[2] = createImage(positive_y);
    images[3] = createImage(negative_y);
    images[4] = createImage(positive_z);
    images[5] = createImage(negative_z);

    const int imageWidth = images[0]->getWidth();
    const int imageHeight = images[0]->getHeight();
    for (int i = 0; i < 6; i++) {
        Image* image = images[i];
        if (image->getWidth() != image->getHeight()) {
            OCF_ASSERT("TextureCube: image size is not square");
            return false;
        }
        if (image->getHeight() != imageHeight) {
            OCF_ASSERT("TextureCube: image size is not same");
            return false;
        }
    }

    glGenTextures(1, &m_textureId);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_textureId);

    for (int i = 0; i < 6; i++) {
        Image* image = images[i];

        const GLenum format = OpenGLUtility::toGLFormat(image->getPixelFormat());

        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                     0,
                     format, imageWidth, imageHeight,
                     0,
                     format, GL_UNSIGNED_BYTE, image->getData());
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

    for (auto&& image : images)
    {
        delete image;
    }

    return true;
}

void TextureCube::setActive() const
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_textureId);
}

Image* TextureCube::createImage(std::string_view filename)
{
    std::string fullPath = FileUtils::getInstance()->fullPathForFilename(filename.data());
    if (fullPath.empty())
    {
        return nullptr;
    }

    Image* image = new Image();
    if (image->loadImageFile(fullPath))
    {
        return image;
    }

    delete image;
    return nullptr;
}

NS_OCF_END
