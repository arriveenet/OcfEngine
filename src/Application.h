#pragma once
#include "Window.h"
#include "Scene/Scene.h"
#include <glm/glm.hpp>

class Applicaiton {
public:
	static Applicaiton* getInstance();

	Applicaiton();
	~Applicaiton();

	bool init();
	void destroy();

	int run();
	void exit();
	void swapBuffers();
	void pollEvents();
	bool windowShouldClose();
	double getTime();

	glm::ivec2 getWindowSize() const;

	void onKeyEnvet(int key, int scancode, int action, int mods);

private:
	Window m_window;
	int m_windowWidth;
	int m_windowHeight;
};