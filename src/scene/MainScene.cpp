#include "MainScene.h"
#include <glm/gtc/type_ptr.hpp>
#include "base/Game.h"
#include "renderer/Image.h"
#include "2d/Label.h"

USING_OCF

MainScene::MainScene()
{
}

MainScene::~MainScene()
{
}

bool MainScene::init()
{
	Scene::init();

	Sprite* sprite1 = Sprite::create("..\\assets\\textures\\25_Crono.png", 120);
	Sprite* sprite2 = Sprite::create("..\\assets\\textures\\28_Frog.png");

	this->addChild(sprite1);
	this->addChild(sprite2);

	glm::vec2 size = Game::getInstance()->getVisibleSize();
	size /= 2.0f;

	sprite1->setPosition(size.x, size.y);
	sprite2->setPosition(240, 240);

	Label* label = Label::create("Hello World!");
	this->addChild(label);

	label->setPosition(700.0f, 200.0f);
	label->setRotation(30.0f);

	return true;
}

void MainScene::update(float deltaTime)
{
	Entity::update(deltaTime);
}
