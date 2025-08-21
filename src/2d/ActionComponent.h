#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "2d/Component.h"

namespace ocf {

class Animation;
class SpriteFrame;

class ActionComponent : public Component {
public:
    ActionComponent(Node2D* pNode);
    virtual ~ActionComponent();

    bool initWithDuration(float duration);

    void update(float deltaTime) override;

    virtual void step(float time);

    virtual void start();

    virtual void stop();

    virtual bool isDone() const;

    virtual float getDuration() const { return m_duration; }

    virtual float getElapsed() const { return m_elapsed; }

protected:
    float m_duration;
    float m_elapsed;
    bool m_firstTick;
    bool m_done;
};

class AnimateComponent : public ActionComponent {
public:
    AnimateComponent(Node2D* pNode);
    virtual ~AnimateComponent();

    bool initWithAnimation(Animation* pAnimation);

    void start() override;

    void stop() override;

    void step(float time) override;

protected:
    Animation* m_pAnimation;
    SpriteFrame* m_pOriginFrame;
    std::vector<float> m_splitTimes;
    int m_nextFrame;
    int m_currentFrameIndex;
    unsigned int m_excutedLoops;
};

class BlinkComponent : public ActionComponent {
public:
    BlinkComponent(Node2D* pNode);

    bool initWithDuration(float duration, int blinks);

    void step(float time) override;

protected:
    int m_times;
};

class RotateByComponent : public ActionComponent {
public:
    RotateByComponent(Node2D* pNode);

    bool initWithDuration(float duration, float deltaAngle);

    void step(float time);

protected:
    glm::vec3 m_deltaAngle;
    glm::vec3 m_startAngle;
};

class MoveByComponent : public ActionComponent {
public:
    MoveByComponent(Node2D* pNode);

    bool initWithDuration(float duration, const glm::vec2& deltaPosition);

    void step(float time) override;

protected:
    glm::vec2 m_positionDelta;
    glm::vec2 m_startPosition;
    glm::vec2 m_previousPosition;
};

class ScaleToComponent : public ActionComponent {
public:
    ScaleToComponent(Node2D* pNode);

    bool initWithDuration(float duration, float s);

    void step(float time) override;

protected:
    glm::vec2 m_startScale;
    glm::vec2 m_endScale;
    glm::vec2 m_deltaScale;
};

class RepeatForever : public ActionComponent {
public:
    RepeatForever(Node2D* pNode);
    virtual ~RepeatForever();

    bool initWithAction(ActionComponent* pAction);

    void update(float deltaTime) override;

    bool isDone() const override;

protected:
    ActionComponent* m_pInnerlAction;
};

} // namespace ocf
