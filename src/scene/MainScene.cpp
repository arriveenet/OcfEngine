#include "MainScene.h"
#include "Game.h"
#include "renderer/Image.h"
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>


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

	return true;
}

void MainScene::update()
{

}

void MainScene::draw()
{
}
