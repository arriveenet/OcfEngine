#include "Scene.h"
#include "../Sprite.h"

class MainScene : public Scene {
public:
	MainScene();
	virtual ~MainScene();

	bool init() override;

	void update() override;
	void draw() override;

	void onKeyEnvet(int key, int scancode, int action, int mods) override;

private:
};