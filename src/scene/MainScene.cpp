#include "MainScene.h"
#include "../renderer/Image.h"
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

	Entity* sprite = new Entity();
	sprite->addComponent(new Sprite(sprite));

	addEntity(sprite);

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
