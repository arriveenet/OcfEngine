#include "SpriteTest.h"
#include "2d/SpriteFrameManager.h"
#include "2d/ActionComponent.h"
#include "audio/AudioEngine.h"

USING_NS_OCF;

SpriteTestDemo::SpriteTestDemo()
{
    setTestCaseName("SpriteTest");
}

SpriteTestDemo::~SpriteTestDemo()
{
}

bool SpriteTestDemo::init()
{
    if (TestCase::init()) {
        const auto size = m_pGame->getVisibleSize();

        auto sprite1 = Sprite::create("textures/Mario.png");
        sprite1->setPosition(size / 2.0f);
        sprite1->setScale(10.0f);
        sprite1->setGlobalZOrder(1.0f);
        this->addChild(sprite1);

        auto sprite2 = Sprite::create("textures/28_Frog.png");
        sprite2->setPosition((size / 2.0f) * 0.5f);
        this->addChild(sprite2);

        SpriteFrameManager::getInstance()->addSpriteFramesWithFile("texture.xml");
        auto sprite3 = Sprite::createWithSpriteFrameName("Kirby_1.png");
        sprite3->setPosition(100, 100);
        sprite3->setScale(10.0f);
        this->addChild(sprite3);

        auto sprite4 = Sprite::create();
        sprite4->setSize(100, 100);
        this->addChild(sprite4);

        return true;
    }

    return false;
}
