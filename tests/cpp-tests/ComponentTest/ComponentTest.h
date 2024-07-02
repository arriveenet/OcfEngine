#pragma once
#include "../BaseTest.h"

class ComponentTest : public TestCase {
public:
	ComponentTest();
	~ComponentTest();

	bool init() override;
};

class Laser : public ocf::Sprite {
public:
	Laser();
	virtual ~Laser();

	void updateNode(float deltaTime) override;

	ocf::MoveComponent* m_pMoveComponent;
	ocf::CircleComponent* m_pCircleComponent;
	float m_deathTimer;
};

class Asteroid : public ocf::Sprite {
public:
	Asteroid();

	void updateNode(float deltaTime) override;

	ocf::MoveComponent* m_pMoveComponent;
	ocf::CircleComponent* m_pCircleComponent;
};

class Ship : public ocf::Sprite {
public:
	Ship();

	void processInput(const ocf::InputState& inputState) override;

	ocf::MoveComponent* m_pMoveComponent;
};
