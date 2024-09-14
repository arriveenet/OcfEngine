#pragma once
#include <vector>
#include "base/GameObject.h"

NS_OCF_BEGIN

class SpriteFrame;

struct AnimationFrame {
    SpriteFrame* spriteFrame;
    float delayUnits;

    AnimationFrame(SpriteFrame* frame, float delay)
    : spriteFrame(frame)
    , delayUnits(delay)
    {}
};

class Animation : public GameObject {
public:
    static Animation* createWithSpriteFrames(const std::vector<SpriteFrame*>& spriteFrames,
                                             float delay = 0.0f, unsigned int loops = 1);
    Animation();
    virtual ~Animation();

    bool initWithSpriteFrames(const std::vector<SpriteFrame*>& frames, float delay = 0.0f, unsigned int loops = 1);

    float getDuration() const;

    unsigned int getLoops() const { return m_loops; }

    float getTotalDelayUnits() const { return m_totalDelayUnits; }

    const std::vector<AnimationFrame>& getFrames() const { return m_frames; }

    bool getResetOriginalFrame() const { return m_resetOriginalFrame; }

    void setResetOriginalFrame(bool resetOriginalFrame) { m_resetOriginalFrame = resetOriginalFrame; }

protected:
    std::vector<AnimationFrame> m_frames;
    float m_totalDelayUnits;
    float m_delayPerUnit;
    float m_duration;
    unsigned int m_loops;
    bool m_resetOriginalFrame;
};

NS_OCF_END
