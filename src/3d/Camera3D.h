#pragma once
#include "3d/Node3D.h"

NS_OCF_BEGIN

class Camera3D : public Node3D {
    friend class Scene;

public:
    enum class Type {
        Perspective, //!< 透視投影
        Orthographic //!< 平行投影
    };

    static Camera3D* createPerspective(float fovy, float aspect, float zNear, float zFar);
    static Camera3D* createOrthographic(float width, float height, float zNear = -1.0f,
                                      float zFar = 1.0f);

    static Camera3D* getVisitingCamera();

    static const glm::vec4& getDefaultViewpot();
    static void setDefaultViewport(const glm::vec4& viewport);

    Camera3D();
    ~Camera3D();

    bool init() override;
    virtual bool initPerspective(float fovy, float aspect, float zNear, float zFar);
    virtual bool initOrthographic(float left, float right, float bottom, float top,
                                  float zNear = -1.0f, float zFar = 1.0f);

    virtual void lookAt(const glm::vec3& center, const glm::vec3& up = glm::vec3(0, 1, 0));

    virtual const glm::mat4 getProjectionMatrix() const;
    virtual const glm::mat4 getViewMatrix() const;
    virtual const glm::mat4 getViewProjectionMatrix() const;

    Camera3D::Type getType() const
    {
        return m_type;
    }

    CameraFlag getCameraFlag() const
    {
        return m_cameraFlag;
    }
    void setCameraFlag(CameraFlag flag)
    {
        m_cameraFlag = flag;
    }

    glm::vec3 unProjectGL(const glm::vec3& src) const;

protected:
    static Camera3D* s_pVisitingCamera;
    static glm::vec4 s_defaultViewport;
    CameraFlag m_cameraFlag;
    glm::mat4 m_projection;
    glm::vec3 m_center;
    mutable glm::mat4 m_view;
    mutable glm::mat4 m_viewInverse;
    mutable glm::mat4 m_viewProjection;
    float m_zNear;
    float m_zFar;
    Type m_type;
    Scene* m_scene;
    mutable bool m_viewProjectionDirty;
};

NS_OCF_END