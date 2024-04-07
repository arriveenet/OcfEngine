#include "MainScene.h"
#include <stdlib.h>
#include <glm/gtc/type_ptr.hpp>
#include "base/Game.h"
#include "renderer/Image.h"
#include "2d/Label.h"
#include "2d/MoveComponent.h"

OCF_BEGIN

Asteroid::Asteroid()
{
	initWithFile("..\\assets\\textures\\Asteroid.png");
	m_pMoveComponent = new MoveComponent(this);
	addComponent(m_pMoveComponent);

	m_pMoveComponent->setForwardSpeed(150.0f);

	glm::vec2 visibleSize = Game::getInstance()->getVisibleSize();
	float rot = static_cast<float>(rand() % 360);
	float x = static_cast<float>(rand() % (int)visibleSize.x);
	float y = static_cast<float>(rand() % (int)visibleSize.y);

	setRotation(rot);
	setPosition(x, y);
}

void Asteroid::updateEntity(float deltaTime)
{
	glm::vec2 visibleSize = Game::getInstance()->getVisibleSize();

	if (m_position.x < 0.0f) {
		m_position.x = visibleSize.x;
	}
	if (m_position.x > visibleSize.x) {
		m_position.x = 0.0f;
	}
	if (m_position.y < 0.0f) {
		m_position.y = visibleSize.y;
	}
	if (m_position.y > visibleSize.y) {
		m_position.y = 0.0f;
	}

	Sprite::updateEntity(deltaTime);
}

Ship::Ship()
{
	initWithFile("..\\assets\\textures\\Ship.png");

	m_pMoveComponent = new MoveComponent(this);
	addComponent(m_pMoveComponent);

	glm::vec2 visibleSize = Game::getInstance()->getVisibleSize();
	setPosition(visibleSize.x / 2, visibleSize.y / 2);
}

void Ship::processInput(const InputState& inputState)
{

	float fowardSpeed = 0.0f;
	if (inputState.keyboard.getKeyState(GLFW_KEY_W) == ButtonState::Hold) {
		fowardSpeed = 300.0f;
	}
	if (inputState.keyboard.getKeyState(GLFW_KEY_S) == ButtonState::Hold) {
		fowardSpeed = -300.0f;
	}

	float rotation = getRotation();
	if (inputState.keyboard.getKeyState(GLFW_KEY_Q) == ButtonState::Hold) {
		rotation += 3.0f;
	}
	if (inputState.keyboard.getKeyState(GLFW_KEY_E) == ButtonState::Hold) {
		rotation -= 3.0f;
	}
	setRotation(rotation);

	m_pMoveComponent->setForwardSpeed(fowardSpeed);

}



MainScene::MainScene()
{
}

MainScene::~MainScene()
{
}

bool MainScene::init()
{
	Scene::init();

	for (int i = 0; i < 20; i++) {
		Asteroid* astroid = new Asteroid();
		addChild(astroid);
	}

	Ship* ship = new Ship();
	addChild(ship);

	return true;
}

void MainScene::update(float deltaTime)
{
	Entity::update(deltaTime);
}

OCF_END
