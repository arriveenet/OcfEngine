#include "MainScene.h"

#include "SpriteTest/SpriteTest.h"
#include "ComponentTest/ComponentTest.h"
#include "TiledMapTest/TiledMapTest.h"
#include "AudioEngineTest/AudioEngineTest.h"

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
		auto button1 = Button::create("ButtonNormal.png", "ButtonActive.png");
		button1->setText("SpriteTest");
		button1->setPosition(100, visibleSize.y -130.0f);
		button1->setOnClickCallback([=]() {
			auto scene = new SpriteTestDemo();
			scene->init();
			m_pGame->replaceScene(scene);
			});
		addChild(button1);

		auto button2 = Button::create("ButtonNormal.png", "ButtonActive.png");
		button2->setText("CompoTest");
		button2->setPosition(100, button1->getPosition().y - 60.0f);
		button2->setOnClickCallback([=]() {
			auto scene = new ComponentTest();
			scene->init();
			m_pGame->replaceScene(scene);
			});
		addChild(button2);

		auto button3 = Button::create("ButtonNormal.png", "ButtonActive.png");
		button3->setText("TiledMapTest");
		button3->setPosition(100, button2->getPosition().y - 60.0f);
		button3->setOnClickCallback([=]() {
			auto scene = new TiledMapTest();
			scene->init();
			m_pGame->replaceScene(scene);
			});
		addChild(button3);

		auto button4 = Button::create("ButtonNormal.png", "ButtonActive.png");
		button4->setText("AudioTest");
		button4->setPosition(100, button3->getPosition().y - 60.0f);
		button4->setOnClickCallback([=]() {
			auto scene = new AudioEngineTest();
			scene->init();
			m_pGame->replaceScene(scene);
			});
		addChild(button4);

		auto button = Button::create("ButtonNormal.png", "ButtonActive.png");
		button->setText("Exit");
		button->setPosition(100, button4->getPosition().y -60.0f);
		button->setOnClickCallback([=]() {
			m_pGame->exit();
			});
		addChild(button);

		return true;
	}

	return false;
}

