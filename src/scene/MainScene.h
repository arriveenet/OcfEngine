#include "Scene.h"
#include "2d/Sprite.h"

class MainScene : public Scene {
public:
	MainScene();
	virtual ~MainScene();

	bool init() override;

	void update() override;
	void draw() override;
};