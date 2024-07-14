#include "SpriteTest.h"
#include "2d/SpriteFrameManager.h"

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

		auto sprite1 = Sprite::create("textures\\25_Crono.png");
		sprite1->setPosition(size / 2.0f);
		sprite1->setGlobalZOrder(1.0f);
		this->addChild(sprite1);

		auto sprite2 = Sprite::create("textures\\28_Frog.png");
		sprite2->setPosition((size / 2.0f) * 0.5f);
		this->addChild(sprite2);

		SpriteFrameManager::getInstance()->addSpriteFramesWithFile("texture.xml");
		auto sprirte3 = Sprite::createWithSpriteFrameName("Kirby_1.png");
		sprirte3->setPosition(100, 100);
		sprirte3->setScale(10.0f);
		this->addChild(sprirte3);
		return true;
	}

	return false;
}
