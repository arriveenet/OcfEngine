#include "SpriteTest.h"

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


		return true;
	}

	return false;
}
