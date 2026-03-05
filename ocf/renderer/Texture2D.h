#pragma once
#include "base/GameObject.h"
#include "base/Types.h"
#include "renderer/backend/DriverBase.h"
#include "renderer/backend/Handle.h"
#include <glm/glm.hpp>
#include <string>

namespace ocf {

class Image;

class Texture2D : public GameObject {
public:
    Texture2D();
    virtual ~Texture2D();

    bool initWithFile(const std::string& filename);
    bool initWithData(uint8_t* data, size_t dataSize,
                      int width, int height, PixelFormat format);
    bool initWithImage(Image* image, PixelFormat format);

    void setActive() const;

    void updateSubData(uint8_t* data, int xoffset, int yoffset, int width, int height) const;

    int getWidth() const;
    int getHeight() const;
    glm::vec2 getSize() const;

private:
    unsigned int m_textureId;
    int m_width;
    int m_height;
    PixelFormat m_pixelFormat;
    glm::vec2 m_size;
    backend::Handle<backend::HwTexture> m_handle;

};

} // namespace ocf
