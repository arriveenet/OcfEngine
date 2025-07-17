#pragma once
#include "base/Node.h"

NS_OCF_BEGIN

class Camera2D;
class Camera3D;

class Viewport : public Node {
public:
    Viewport();
    ~Viewport();

    glm::ivec2 getSize() const;
    void setSize(const glm::ivec2& size);

    Camera2D* getCamera2D() const;
    Camera3D* getCamera3D() const;

private:
    glm::ivec2 m_size;
    Camera2D* m_pCamera2D = nullptr;
    Camera3D* m_pCamera3D = nullptr;
};

NS_OCF_END