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

	Sprite* sprite = new Sprite();
	sprite->initWithFile(".\\resource\\25_Crono.png");
	addChild(sprite);

	glm::vec2 size = Game::getInstance()->getVisibleSize();
	size /= 2.0f;
	sprite->setPosition(size.x, size.y);

	return true;
}

void MainScene::update()
{

}

void MainScene::draw()
{
}

void MainScene::onKeyEnvet(int key, int scancode, int action, int mods)
{

}
