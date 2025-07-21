#pragma once
#include <vector>
#include "2d/Node2D.h"

NS_OCF_BEGIN

class Camera2D;

class Scene : public Node2D {
public:
    Scene();
    virtual ~Scene();

    virtual bool init();

    void render(Renderer* renderer, const glm::mat4& transform);

    virtual Camera2D* getDefaultCamera() { return m_pDefaultCamera; }

    const std::vector<Camera2D*>& getCameras();

protected:
    friend Camera2D;

    Camera2D* m_pDefaultCamera;
    std::vector<Camera2D*> m_cameras;
};

NS_OCF_END
