#pragma once
#include <vector>
#include "2d/Node2D.h"
#include "base/Viewport.h"

NS_OCF_BEGIN

class Camera2D;
class Game;
class Viewport;

class Scene {
public:
    Scene();
    virtual ~Scene();

    virtual bool init();

    void update(float deltaTime);

    virtual void updateScene(float deltaTime);

    void render(Renderer* renderer, const glm::mat4& transform);

    virtual void onEnter();

    virtual void onExit();

    virtual Camera2D* getDefaultCamera() { return m_pDefaultCamera; }

    const std::vector<Camera2D*>& getCameras();

    void addNode(Node* node);
    void removeNode(Node* node);

    inline Viewport* getRoot() const
    {
        return m_root;
    }

protected:
    friend Camera2D;

    Camera2D* m_pDefaultCamera;
    std::vector<Camera2D*> m_cameras;

    Viewport* m_root = nullptr;
    Game* m_pGame = nullptr;
};

NS_OCF_END
