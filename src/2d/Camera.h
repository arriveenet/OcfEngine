#pragma once
#include "Node.h"

NS_OCF_BEGIN

class Scene;

enum class CameraFlag : uint16_t {
    Default = 1,
    User1  = 1 << 1,
    User2  = 1 << 2,
    User3  = 1 << 3,
    User4  = 1 << 4,
};

class Camera : public Node {
    friend class Scene;

public:
    enum class Type {
        Perspective,	//!< 透視投影
        Orthographic	//!< 平行投影
    };

    static Camera* createPerspective(float fovy, float aspect, float zNear, float zFar);
    static Camera* createOrthographic(float left, float right, float bottom, float top, float zNear = -1.0f, float zFar = 1.0f);
    static Camera* getDefaultCamera();

    static Camera* getVisitingCamera();

    static const glm::vec4& getDefaultViewpot();
    static void setDefaultViewport(const glm::vec4& viewport);

    Camera();
    ~Camera();

    bool init() override;
    virtual bool initPerspective(float fovy, float aspect, float zNear, float zFar);
    virtual bool initOrthographic(float width, float height, float zNear = -1.0f, float zFar = 1.0f);

    virtual void lookAt(const glm::vec3& center, const glm::vec3& up = glm::vec3(0, 1, 0));

    virtual const glm::mat4 getProjectionMatrix() const;
    virtual const glm::mat4 getViewMatrix() const;
    virtual const glm::mat4 getViewProjectionMatrix() const;

    Camera::Type getType() const { return m_type; }

    CameraFlag getCameraFlag() const { return m_cameraFlag; }
    void setCameraFlag(CameraFlag flag) { m_cameraFlag = flag; }

    void onEnter() override;
    void onExit() override;

    void apply();

    void setScene(Scene* scene);

    glm::vec3 unProjectGL(const glm::vec3& src) const;

protected:
    static Camera* s_pVisitingCamera;
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
