#pragma once
#include <vector>
#include "base/Node.h"

NS_OCF_BEGIN

class Camera;

class Scene : public Node {
public:
    Scene();
    virtual ~Scene();

    virtual bool init();

    void render(Renderer* renderer, const glm::mat4& transform);

    virtual Camera* getDefaultCamera() { return m_pDefaultCamera; }

    const std::vector<Camera*>& getCameras();

protected:
    friend Camera;

    Camera* m_pDefaultCamera;
    std::vector<Camera*> m_cameras;
};

NS_OCF_END
