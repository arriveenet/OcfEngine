#pragma once
#include <glm/glm.hpp>
#include "2d/Component.h"

NS_OCF_BEGIN

class ActionComponent : public Component {
public:
    ActionComponent(Node* pNode);

    bool initWithDuration(float duration);

    void update(float deltaTime) override;

    virtual void step(float time);

    virtual bool isDone() const;

protected:
    float m_duration;
    float m_elapsed;
    bool m_firstTick;
    bool m_done;
};

class BlinkComponent : public ActionComponent {
public:
    BlinkComponent(Node* pNode);

    bool initWithDuration(float duration, int blinks);

    void step(float time) override;

protected:
    int m_times;
};

class RotateByComponent : public ActionComponent {
public:
    RotateByComponent(Node* pNode);

    bool initWithDuration(float duration, float deltaAngle);

    void step(float time);

protected:
    glm::vec3 m_deltaAngle;
    glm::vec3 m_startAngle;
};

class MoveByComponent : public ActionComponent {
public:
    MoveByComponent(Node* pNode);

    bool initWithDuration(float duration, const glm::vec2& deltaPosition);

    void step(float time) override;

protected:
    glm::vec3 m_positionDelta;
    glm::vec3 m_startPosition;
    glm::vec3 m_previousPosition;
};

class ScaleToComponent : public ActionComponent {
public:
    ScaleToComponent(Node* pNode);

    bool initWithDuration(float duration, float s);

    void step(float time) override;

protected:
    glm::vec3 m_startScale;
    glm::vec3 m_endScale;
    glm::vec3 m_deltaScale;
};

NS_OCF_END