#include "Viewport.h"

NS_OCF_BEGIN

Viewport::Viewport()
    : m_size(512, 512)
{
    setName("Viewport");
}

Viewport::~Viewport()
{
}

glm::ivec2 Viewport::getSize() const
{
    return m_size;
}

void Viewport::setSize(const glm::ivec2& size)
{
    m_size = size;
}

Camera2D* Viewport::getCamera2D() const
{
    return m_pCamera2D;
}

Camera3D* Viewport::getCamera3D() const
{
    return m_pCamera3D;
}



NS_OCF_END
