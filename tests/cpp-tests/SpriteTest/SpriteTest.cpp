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
    Viewport* root = getRoot();
    const auto size = m_pGame->getVisibleSize();

    auto sprite1 = Sprite::create("textures/25_Crono.bmp");
    sprite1->setPosition(size / 2.0f);
    sprite1->setGlobalZOrder(1.0f);
    root->addChild(sprite1);

    auto sprite2 = Sprite::create("textures/28_Frog.png");
    sprite2->setPosition((size / 2.0f) * 1.5f);
    root->addChild(sprite2);

    SpriteFrameManager::getInstance()->addSpriteFramesWithFile("texture.xml");
    auto sprite3 = Sprite::createWithSpriteFrameName("Kirby_1.png");
    sprite3->setPosition(glm::vec2(100, 300));
    sprite3->setScale(glm::vec2(10, 10));
    root->addChild(sprite3);

    auto sprite4 = Sprite::create();
    sprite4->setPosition(glm::vec2(100, 500));
    sprite4->setSize(glm::vec2(50, 50));
    root->addChild(sprite4);

    auto sprite5 = Sprite::create("textures/fruit_suika_red.jpg");
    sprite5->setPosition(glm::vec2(100, 600));
    sprite5->setSize(glm::vec2(100, 100));
    root->addChild(sprite5);


    return TestCase::init();
}
