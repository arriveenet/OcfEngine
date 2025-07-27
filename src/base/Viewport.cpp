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
    Camera2D* camera2D = nullptr;
    Camera3D* camera3D = nullptr;

    findCamera2DRecursive(this, camera2D);
    findCamera3DRecursive(this, camera3D);

    m_pCamera2D = camera2D;
    m_pCamera3D = camera3D;
}

void Viewport::findCamera2DRecursive(Node* currentNode,  Camera2D* foundCamera)
{
    if (foundCamera) {
        return;
    }
 
    Camera2D* camera = dynamic_cast<Camera2D*>(currentNode);
    if (camera) {
        foundCamera = camera;
        return;
    }

    for (const auto& child : m_children) {
        findCamera2DRecursive(child, foundCamera);
        if (foundCamera) {
            return;
        }
    }
}

void Viewport::findCamera3DRecursive(Node* currentNode, Camera3D* foundCamera)
{
    if (foundCamera) {
        return;
    }

    Camera3D* camera = dynamic_cast<Camera3D*>(currentNode);
    if (camera) {
        foundCamera = camera;
        return;
    }

    for (const auto& child : m_children) {
        findCamera3DRecursive(child, foundCamera);
        if (foundCamera) {
            return;
        }
    }
}

NS_OCF_END
