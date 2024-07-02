#include "MainScene.h"

#include "SpriteTest/SpriteTest.h"
#include "ComponentTest/ComponentTest.h"

USING_NS_OCF;

using namespace ocf::ui;

MainScene::MainScene()
{
}

MainScene::~MainScene()
{
}

bool MainScene::init()
{
	if (Scene::init()) {
		auto visibleSize = m_pGame->getVisibleSize();
		auto button1 = Button::create();
		button1->setText("SpriteTest");
		button1->setPosition(100, visibleSize.y -130.0f);
		button1->setOnClickCallback([=]() {
			auto scene = new SpriteTestDemo();
			scene->init();
			m_pGame->replaceScene(scene);
			});
		addChild(button1);

		auto button2 = Button::create();
		button2->setText("CompoTest");
		button2->setPosition(100, button1->getPosition().y - 60.0f);
		button2->setOnClickCallback([=]() {
			auto scene = new ComponentTest();
			scene->init();
			m_pGame->replaceScene(scene);
			});
		addChild(button2);

		auto button = Button::create();
		button->setText("Exit");
		button->setPosition(100, button2->getPosition().y -60.0f);
		button->setOnClickCallback([=]() {
			m_pGame->exit();
			});
		addChild(button);

		return true;
	}

	return false;
}

