#pragma once
#include "base/GameObject.h"
#include "base/types.h"
#include <glm/glm.hpp>
#include <string>

NS_OCF_BEGIN

class Image;

class Texture2D : public GameObject {
public:
    Texture2D();
    virtual ~Texture2D();

    bool initWithFile(const std::string& filename);
    bool initWithImage(Image* image, PixelFormat format);
    void setActive() const;

    int getWidth() const;
    int getHeight() const;
    glm::vec2 getSize() const;

private:
    unsigned int m_textureId;
    int m_width;
    int m_height;
    PixelFormat m_pixelFormat;
    glm::vec2 m_size;
};

NS_OCF_END
