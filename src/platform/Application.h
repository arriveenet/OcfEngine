#pragma once
#include <glm/glm.hpp>
#include "ApplicationBase.h"
#include "2d/Scene.h"

NS_OCF_BEGIN

class Applicaiton : public ApplicationBase {
public:
	static Applicaiton* getInstance();

	Applicaiton();
	~Applicaiton();

	bool init();

	int run();

private:
	int m_windowWidth;
	int m_windowHeight;
};

NS_OCF_END
