#include "Scene.h"
#include "2d/Sprite.h"
#include "2d/Label.h"

class MainScene : public Scene {
public:
	MainScene();
	virtual ~MainScene();

	bool init() override;

	void update(float deltaTime) override;
	void draw() override;
};