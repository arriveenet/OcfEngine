#include "MainScene.h"
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>
#include "Game.h"
#include "renderer/Image.h"
#include "2d/Label.h"


MainScene::MainScene()
{
}

MainScene::~MainScene()
{
}

bool MainScene::init()
{
	Scene::init();

	Sprite* sprite1 = new Sprite(120);
	Sprite* sprite2 = new Sprite();

	sprite1->initWithFile(".\\resource\\25_Crono.png");
	sprite2->initWithFile(".\\resource\\25_Crono.png");
	addChild(sprite1);
	addChild(sprite2);

	glm::vec2 size = Game::getInstance()->getVisibleSize();
	size /= 2.0f;

	sprite1->setPosition(size.x, size.y);
	sprite2->setPosition(200, 200);

	Label* label = Label::create("Hello World!");
	addChild(label);

	label->setPosition(700.0f, 0.0f);

	return true;
}

void MainScene::update(float deltaTime)
{
	Entity::update(deltaTime);
}

void MainScene::draw()
{
}
