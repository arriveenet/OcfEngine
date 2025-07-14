#pragma once
#include "2d/Node2D.h"

NS_OCF_BEGIN

class Scene;

class Camera2D : public Node2D {
    friend class Scene;

public:
    static Camera2D* createPerspective(float fovy, float aspect, float zNear, float zFar);
    static Camera2D* createOrthographic(float width, float height, float zNear = -1.0f, float zFar = 1.0f);
    static Camera2D* getDefaultCamera();

    static Camera2D* getVisitingCamera();

    static const glm::vec4& getDefaultViewpot();
    static void setDefaultViewport(const glm::vec4& viewport);

    Camera2D();
    ~Camera2D();

    bool init() override;
    virtual bool initOrthographic(float left, float right, float bottom, float top,
                                  float zNear = -1.0f, float zFar = 1.0f);

    virtual void lookAt(const glm::vec3& center, const glm::vec3& up = glm::vec3(0, 1, 0));

    virtual const glm::mat4 getProjectionMatrix() const;
    virtual const glm::mat4 getViewMatrix() const;
    virtual const glm::mat4 getViewProjectionMatrix() const;

    CameraFlag getCameraFlag() const { return m_cameraFlag; }
    void setCameraFlag(CameraFlag flag) { m_cameraFlag = flag; }

    void onEnter() override;
    void onExit() override;

    void apply();

    void setScene(Scene* scene);

    glm::vec3 unProjectGL(const glm::vec3& src) const;

protected:
    static Camera2D* s_pVisitingCamera;
    static glm::vec4 s_defaultViewport;
    CameraFlag m_cameraFlag;
    glm::mat4 m_projection;
    glm::vec3 m_center;
    mutable glm::mat4 m_view;
    mutable glm::mat4 m_viewInverse;
    mutable glm::mat4 m_viewProjection;
    float m_zNear;
    float m_zFar;
    Scene* m_scene;
    mutable bool m_viewProjectionDirty;
};

NS_OCF_END
