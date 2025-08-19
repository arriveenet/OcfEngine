#include "ActionTest.h"
#include "2d/ActionComponent.h"
#include "2d/Animation.h"
#include "2d/Sprite.h"
#include "2d/SpriteFrameManager.h"

using namespace ocf;;

ActionTest::ActionTest()
{
}

ActionTest::~ActionTest()
{
}

bool ActionTest::init()
{
    SpriteFrameManager::getInstance()->addSpriteFramesWithFile("UnityChan.xml");
    std::vector<SpriteFrame*> animationFrames;
    char fileName[256] = { 0 };
    for (int i = 1; i < 14; i++) {
        snprintf(fileName, sizeof(fileName), "Unitychan_Walk_%d.png", i);
        auto frame = SpriteFrameManager::getInstance()->getSpriteFrameByName(fileName);
        animationFrames.push_back(frame);
    }

    Sprite* unityChan = Sprite::createWithSpriteFrameName("Unitychan_Walk_1.png");
    unityChan->setPosition(glm::vec2(200, 200));
    unityChan->setScale(glm::vec2(5.0f, 5.0f));
    Animation* pAnimation = Animation::createWithSpriteFrames(animationFrames, 0.2f);
    AnimateComponent* pAnimate = new AnimateComponent(unityChan);
    pAnimate->initWithAnimation(pAnimation);
    RepeatForever* repeat = new RepeatForever(unityChan);
    repeat->initWithAction(pAnimate);

    unityChan->addComponent(repeat);

    m_root->addChild(unityChan);
    
    return TestCase::init();
}
