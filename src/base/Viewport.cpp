#include "Viewport.h"
#include "2d/Camera2D.h"
#include "3d/Camera3D.h"

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

void Viewport::determineActiveCamera()
{
    m_pCamera2D = findCamera2DRecursive(this);
    m_pCamera3D = findCamera3DRecursive(this);
}

Camera2D* Viewport::findCamera2DRecursive(Node* currentNode)
{
    if (!currentNode) {
        return nullptr;
    }
 
    Camera2D* camera = dynamic_cast<Camera2D*>(currentNode);
    if (camera) {
        return camera;
    }

    for (const auto& child : currentNode->m_children) {
        camera = findCamera2DRecursive(child);
        if (camera) {
            return camera;
        }
    }

    return nullptr;
}

Camera3D* Viewport::findCamera3DRecursive(Node* currentNode)
{
    if (!currentNode) {
        return nullptr;
    }

    Camera3D* camera = dynamic_cast<Camera3D*>(currentNode);
    if (camera) {
        return camera;
    }

    for (const auto& child : currentNode->m_children) {
        camera = findCamera3DRecursive(child);
        if (camera) {
            return camera;
        }
    }
}

NS_OCF_END
