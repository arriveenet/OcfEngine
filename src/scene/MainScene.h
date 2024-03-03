#include "Scene.h"
#include "2d/Sprite.h"
#include "2d/Label.h"

OCF_BEGIN

class MainScene : public Scene {
public:
	MainScene();
	virtual ~MainScene();

	bool init() override;

	void update(float deltaTime) override;
};

OCF_END
