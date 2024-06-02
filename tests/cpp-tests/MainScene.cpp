#include "MainScene.h"
#include <stdlib.h>
#include <iostream>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include "base/Game.h"
#include "renderer/Image.h"
#include "2d/Label.h"
#include "2d/MoveComponent.h"
#include "2d/CircleComponent.h"

OCF_BEGIN

std::vector<Asteroid*> g_pAsteroid;

Laser::Laser()
	: m_deathTimer(1.0f)
{
	initWithFile("..\\assets\\textures\\Laser.png");
	m_pMoveComponent = new MoveComponent(this);
	m_pMoveComponent->setForwardSpeed(800.0f);
	addComponent(m_pMoveComponent);
	
	m_pCircleComponent = new CircleComponent(this);
	m_pCircleComponent->setRadius(11.0f);
	addComponent(m_pCircleComponent);
}

Laser::~Laser()
{
}

void Laser::updateNode(float deltaTime)
{
	m_deathTimer -= deltaTime;
	if (m_deathTimer <= 0.0f) {
		setState(Dead);
	}
	else {
		for (auto astroid : g_pAsteroid) {
			if (intersectCircle(*m_pCircleComponent, *astroid->m_pCircleComponent)) {
				setState(Dead);
				astroid->setState(Dead);

				auto iter = std::find(g_pAsteroid.begin(), g_pAsteroid.end(), astroid);
				if (iter != g_pAsteroid.end()) {
					g_pAsteroid.erase(iter);
				}
				break;
			}
		}
	}
}

Asteroid::Asteroid()
{
	initWithFile("..\\assets\\textures\\Asteroid.png");
	m_pMoveComponent = new MoveComponent(this);
	addComponent(m_pMoveComponent);
	m_pCircleComponent = new CircleComponent(this);
	addComponent(m_pCircleComponent);

	m_pCircleComponent->setRadius(40.0f);
	m_pMoveComponent->setForwardSpeed(150.0f);

	glm::vec2 visibleSize = Game::getInstance()->getVisibleSize();
	float rot = static_cast<float>(rand() % 360);
	float x = static_cast<float>(rand() % (int)visibleSize.x);
	float y = static_cast<float>(rand() % (int)visibleSize.y);

	setRotation(rot);
	setPosition(x, y);
}

void Asteroid::updateNode(float deltaTime)
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


	if (inputState.keyboard.getKeyState(GLFW_KEY_SPACE) == ButtonState::Pressed) {
		Laser* pLaser = new Laser();
		pLaser->setPosition(getPosition());
		pLaser->setRotation(getRotation());
		this->addChild(pLaser);
	}
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

	auto sprite1 = Sprite::create("..\\assets\\textures\\25_Crono.png");
	addChild(sprite1);
	//auto sprite2 = Sprite::create("..\\assets\\textures\\28_Frog.png");
	//sprite1->addChild(sprite2);
	//sprite2->setPosition(100, 100);

	return true;
}

OCF_END
