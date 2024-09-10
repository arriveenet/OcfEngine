#include "ActionComponent.h"
#include <algorithm>
#include "2d/Node.h"

NS_OCF_BEGIN

ActionComponent::ActionComponent(Node* pNode)
    : Component(pNode)
    , m_duration(0.0f)
    , m_elapsed(0.0f)
    , m_firstTick(true)
    , m_done(false)
{
}

bool ActionComponent::initWithDuration(float duration)
{
    m_duration = duration;
    return true;
}

void ActionComponent::update(float deltaTime)
{
    if (m_firstTick) {
        m_firstTick = false;
        m_elapsed = 0.0f;
    } 
    else {
        m_elapsed += deltaTime;
    }

    const float updateDelta = std::max(0.0f, std::min(1.0f, m_elapsed / m_duration));

    this->step(updateDelta);

    m_done = m_elapsed >= m_duration;
}

void ActionComponent::step(float time)
{
}

bool ActionComponent::isDone() const
{
    return m_done;
}

BlinkComponent::BlinkComponent(Node* pNode)
    : ActionComponent(pNode)
    , m_times(0)
{
}

bool BlinkComponent::initWithDuration(float duration, int blinks)
{
    if (ActionComponent::initWithDuration(duration) && blinks >= 0) {
        m_times = blinks;
        return true;
    }

    return false;
}

void BlinkComponent::step(float time)
{
    if (!isDone()) {
        float slice = 1.0f / m_times;
        float m = fmodf(time, slice);
        m_pOwner->setVisible(m > slice / 2.0f ? true : false);
    }
}

RotateByComponent::RotateByComponent(Node* pNode)
    : ActionComponent(pNode)
    , m_deltaAngle(0.0f)
    , m_startAngle(0.0f)
{
}

bool RotateByComponent::initWithDuration(float duration, float deltaAngle)
{
    if (ActionComponent::initWithDuration(duration)) {
        m_deltaAngle.z = deltaAngle;
        m_startAngle.z = m_pOwner->getRotation();
        return true;
    }
    return false;
}

void RotateByComponent::step(float time)
{
    m_pOwner->setRotation(m_startAngle.z + m_deltaAngle.z * time);
}

MoveByComponent::MoveByComponent(Node* pNode)
    : ActionComponent(pNode)
    , m_positionDelta(0.0f, 0.0f, 0.0f)
    , m_startPosition(0.0f, 0.0f, 0.0f)
    , m_previousPosition(0.0f, 0.0f, 0.0f)
{
}

bool MoveByComponent::initWithDuration(float duration, const glm::vec2& deltaPosition)
{
    if (ActionComponent::initWithDuration(duration)) {
        m_positionDelta = glm::vec3(deltaPosition, 0.0f);
        m_previousPosition = m_startPosition = m_pOwner->getPosition();
    }
    return false;
}


void MoveByComponent::step(float time)
{
    glm::vec3 currentPosition = m_pOwner->getPosition();
    glm::vec3 diff = currentPosition - m_previousPosition;
    m_startPosition = m_startPosition + diff;
    glm::vec3 newPosition = m_startPosition + (m_positionDelta * time);
    m_pOwner->setPosition(newPosition);
    m_previousPosition = newPosition;
}

ScaleToComponent::ScaleToComponent(Node* pNode)
    : ActionComponent(pNode)
    , m_startScale(0.0f)
    , m_endScale(0.0f)
    , m_deltaScale(0.0f)
{
}

bool ScaleToComponent::initWithDuration(float duration, float s)
{
    if (ActionComponent::initWithDuration(duration)) {
        m_startScale = m_pOwner->getScale();
        m_endScale = glm::vec3(s, s, s);
        m_deltaScale = m_endScale - m_startScale;
  
        return true;
    }

    return false;
}

void ScaleToComponent::step(float time)
{
    m_pOwner->setScaleX(m_startScale.x + m_deltaScale.x * time);
    m_pOwner->setScaleY(m_startScale.y + m_deltaScale.y * time);
    m_pOwner->setScaleX(m_startScale.z + m_deltaScale.z * time);
}

NS_OCF_END
