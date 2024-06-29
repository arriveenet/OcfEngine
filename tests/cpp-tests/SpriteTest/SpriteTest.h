#pragma once
#include "../BaseTest.h"

//class Laser : public ocf::Sprite {
//public:
//	Laser();
//	virtual ~Laser();
//
//	void updateNode(float deltaTime) override;
//
//	MoveComponent* m_pMoveComponent;
//	CircleComponent* m_pCircleComponent;
//	float m_deathTimer;
//};
//
//class Asteroid : public ocf::Sprite {
//public:
//	Asteroid();
//
//	void updateNode(float deltaTime) override;
//
//	MoveComponent* m_pMoveComponent;
//	CircleComponent* m_pCircleComponent;
//};
//
//class Ship : public ocf::Sprite {
//public:
//	Ship();
//
//	void processInput(const ocf::InputState& inputState) override;
//
//	MoveComponent* m_pMoveComponent;
//};

class SpriteTestDemo : public TestCase {
public:
	SpriteTestDemo();
	virtual ~SpriteTestDemo();
};
