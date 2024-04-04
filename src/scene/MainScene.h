#include "Scene.h"
#include "2d/Sprite.h"
#include "2d/Label.h"

OCF_BEGIN

class MoveComponent;

class Asteroid : public Sprite {
public:
	Asteroid();

	void updateEntity(float deltaTime) override;

	MoveComponent* m_pMoveComponent;
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

	void update(float deltaTime) override;

	Asteroid* m_astroid;
};

OCF_END
