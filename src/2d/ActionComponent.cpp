#include "ActionComponent.h"
#include <algorithm>
#include "2d/Node.h"
#include "2d/Animation.h"
#include "2d/SpriteFrame.h"
#include "2d/Sprite.h"

NS_OCF_BEGIN

ActionComponent::ActionComponent(Node* pNode)
    : Component(pNode)
    , m_duration(0.0f)
    , m_elapsed(0.0f)
    , m_firstTick(true)
    , m_done(false)
{
}

ActionComponent::~ActionComponent()
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

    const float updateDelta = (std::max)(0.0f, (std::min)(1.0f, m_elapsed / m_duration));

    this->step(updateDelta);

    m_done = m_elapsed >= m_duration;
}

void ActionComponent::step(float /* time */)
{
}

void ActionComponent::start()
{
    m_elapsed = 0.0f;
    m_firstTick = true;
    m_done = false;
}

void ActionComponent::stop()
{
}

bool ActionComponent::isDone() const
{
    return m_done;
}

AnimateComponent::AnimateComponent(Node* pNode)
    : ActionComponent(pNode)
    , m_pAnimation(nullptr)
    , m_pOriginFrame(nullptr)
    , m_nextFrame(0)
    , m_currentFrameIndex(0)
    , m_excutedLoops(0)
{
}

AnimateComponent::~AnimateComponent()
{
    OCF_SAFE_RELEASE(m_pAnimation);
    OCF_SAFE_RELEASE(m_pOriginFrame);
}

bool AnimateComponent::initWithAnimation(Animation* pAnimation)
{
    float singleDuration = pAnimation->getDuration();

    if (ActionComponent::initWithDuration(singleDuration * pAnimation->getLoops())) {
        m_pAnimation = pAnimation;

        float accumUnitsOftime = 0.0f;
        const float newUnitOfTimeValue = singleDuration / pAnimation->getTotalDelayUnits();

        auto& frames = pAnimation->getFrames();
        m_splitTimes.reserve(frames.size());
        
        for (auto&& frame : frames) {
            float value = (accumUnitsOftime * newUnitOfTimeValue) / singleDuration;
            accumUnitsOftime += frame.delayUnits;
            m_splitTimes.emplace_back(value);
        }

        start();
    }

    return false;
}

void AnimateComponent::start()
{
    ActionComponent::start();

    Sprite* sprite = static_cast<Sprite*>(m_pOwner);
    if (m_pAnimation->getResetOriginalFrame()) {
        m_pOriginFrame = sprite->getSpriteFrame();
        m_pOriginFrame->retain();
    }
    m_nextFrame = 0;
    m_excutedLoops = 0;
}

void AnimateComponent::stop()
{
}

void AnimateComponent::step(float time)
{
    if (time < 1.0f) {
        time *= m_pAnimation->getLoops();

        const unsigned int loopNumber = static_cast<unsigned int>(time);
        if (loopNumber > m_excutedLoops) {
            m_nextFrame = 0;
            m_excutedLoops++;
        }

        time = fmodf(time, 1.0f);
    }

    auto& frames = m_pAnimation->getFrames();
    size_t numberOfFrames = frames.size();
    SpriteFrame* frameToDisplay = nullptr;

    for (int i = m_nextFrame; i < numberOfFrames; i++) {
        const float splitTime = m_splitTimes.at(i);

        if (splitTime <= time) {
            m_currentFrameIndex = i;
            AnimationFrame frame = frames.at(m_currentFrameIndex);
            frameToDisplay = frame.spriteFrame;
            static_cast<Sprite*>(m_pOwner)->setSpriteFrame(frameToDisplay);

            m_nextFrame = i + 1;
        }
        else {
            break;
        }
    }
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

RepeatForever::RepeatForever(Node* pNode)
    : ActionComponent(pNode)
    , m_pInnerlAction(nullptr)
{
}

RepeatForever::~RepeatForever()
{
    OCF_SAFE_DELETE(m_pInnerlAction);
}

bool RepeatForever::initWithAction(ActionComponent* pAction)
{
    m_pInnerlAction = pAction;
    return true;
}

void RepeatForever::update(float deltaTime)
{
    m_pInnerlAction->update(deltaTime);

    if (m_pInnerlAction->isDone() && m_pInnerlAction->getDuration() > 0) {
        float diff = m_pInnerlAction->getElapsed() - m_pInnerlAction->getDuration();
        if (diff > m_pInnerlAction->getDuration()) {
            diff = fmodf(diff, m_pInnerlAction->getDuration());
        }
        m_pInnerlAction->start();
        m_pInnerlAction->update(0.0f);
        m_pInnerlAction->update(diff);
    }
}

bool RepeatForever::isDone() const
{
    return false;
}

NS_OCF_END
