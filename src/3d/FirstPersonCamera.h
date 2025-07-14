#pragma once

#include "3d/Camera3D.h"

NS_OCF_BEGIN

class Event;

class FirstPersonCamera : public Camera3D {
public:
    enum class Movement {
        Forward,
        Backward,
        Left,
        Right,
        Up,
        Down
    };

    static FirstPersonCamera* create(float fovy, float aspect, float zNear, float zFar);

    FirstPersonCamera();
    virtual ~FirstPersonCamera();

    bool initPerspective(float fovy, float aspect, float zNear, float zFar) override;

    void updateNode(float deltaTime) override;

    void onMouseMove(Event* pEvent);
    void onKeyPressed(Keyboard::KeyCode key, Event* pEvent);
    void onKeyReleased(Keyboard::KeyCode key, Event* pEvent);

    const glm::mat4 getViewMatrix() const override;

    void setMoveSpeed(float moveSpeed) { m_moveSpeed = moveSpeed; }
    float getMoveSpeed() const { return m_moveSpeed; }

    void setSensitivity(float sensitivity) { m_sensitivity = sensitivity; }
    float getSensitivity() const { return m_sensitivity; }

    void setCameraControl(bool centerCursor);
    bool getCameraControl() const { return m_isCameraControl; }

    float getYaw() const { return m_yaw; }
    float getPitch() const { return m_pitch; }

protected:
    void updateCameraVectors();

protected:
    float m_moveSpeed;
    float m_sensitivity;
    float m_pitch;
    float m_yaw;
    glm::vec3 m_front;
    glm::vec3 m_up;
    glm::vec3 m_worldUp;
    glm::vec3 m_right;
    bool m_keyStates[6];
    bool m_isCameraControl;

};

NS_OCF_END