#include "Animation.h"
#include "2d/SpriteFrame.h"

NS_OCF_BEGIN

Animation::Animation()
    : m_totalDelayUnits(0.0f)
    , m_delayPerUnit(0.0f)
    , m_duration(0.0f)
    , m_loops(0)
    , m_resetOriginalFrame(false)
{
}

Animation* Animation::createWithSpriteFrames(const std::vector<SpriteFrame*>& spriteFrames,
                                             float delay /* = 0.0f*/, unsigned int loops /* = 1*/)
{
    Animation* animation = new Animation();
    animation->initWithSpriteFrames(spriteFrames, delay, loops);
    animation->autorelease();

    return animation;
}

Animation::~Animation()
{
}

bool Animation::initWithSpriteFrames(const std::vector<SpriteFrame*>& frames, float delay, unsigned int loops)
{
    m_delayPerUnit = delay;
    m_loops = loops;

    for (auto&& spriteFrame : frames) {
        m_frames.emplace_back(AnimationFrame(spriteFrame, 1.0f));
        m_totalDelayUnits++;
    }

    return true;
}

float Animation::getDuration() const
{
    return m_totalDelayUnits * m_delayPerUnit;
}

NS_OCF_END
