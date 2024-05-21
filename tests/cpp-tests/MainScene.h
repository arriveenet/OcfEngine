#include "2d/Scene.h"
#include "2d/Sprite.h"
#include "2d/Label.h"

OCF_BEGIN

class MoveComponent;
class CircleComponent;

class Laser : public Sprite {
public:
	Laser();
	virtual ~Laser();

	void updateNode(float deltaTime) override;

	MoveComponent* m_pMoveComponent;
	CircleComponent* m_pCircleComponent;
	float m_deathTimer;
};

class Asteroid : public Sprite {
public:
	Asteroid();

	void updateNode(float deltaTime) override;

	MoveComponent* m_pMoveComponent;
	CircleComponent* m_pCircleComponent;
};

class Ship : public Sprite {
public:
	Ship();

	void processInput(const InputState& inputState) override;

	MoveComponent* m_pMoveComponent;
};

class MainScene : public Scene {
public:
	MainScene();
	virtual ~MainScene();

	bool init() override;

	Asteroid* m_astroid;
};

OCF_END
