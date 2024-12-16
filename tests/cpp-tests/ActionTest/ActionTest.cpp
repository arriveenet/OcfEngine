#include "ActionTest.h"
#include "2d/ActionComponent.h"
#include "2d/Animation.h"
#include "2d/Sprite.h"
#include "2d/SpriteFrameManager.h"

USING_NS_OCF;

ActionTest::ActionTest()
{
}

ActionTest::~ActionTest()
{
}

bool ActionTest::init()
{
    if (!TestCase::init()) {
        return false;
    }

    SpriteFrameManager::getInstance()->addSpriteFramesWithFile("UnityChan.xml");
    std::vector<SpriteFrame*> animationFrames;
    char fileName[256] = { 0 };
    for (int i = 1; i < 14; i++) {
        sprintf(fileName, "Unitychan_Walk_%d.png", i);
        auto frame = SpriteFrameManager::getInstance()->getSpriteFrameByName(fileName);
        animationFrames.push_back(frame);
    }

    Sprite* unityChan = Sprite::createWithSpriteFrameName("Unitychan_Walk_1.png");
    unityChan->setPosition(200, 200);
    unityChan->setScale(10);
    Animation* pAnimation = Animation::createWithSpriteFrames(animationFrames, 0.2f);
    AnimateComponent* pAnimate = new AnimateComponent(unityChan);
    pAnimate->initWithAnimation(pAnimation);
    RepeatForever* repeat = new RepeatForever(unityChan);
    repeat->initWithAction(pAnimate);

    unityChan->addComponent(repeat);

    this->addChild(unityChan);


    return true;
}
