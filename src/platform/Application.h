#pragma once
#include <glm/glm.hpp>
#include "Window.h"
#include "Scene/Scene.h"

OCF_BEGIN

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
	glm::vec2 getCursorPosition() const;

private:
	friend Window;
	static void onWindowSize(GLFWwindow* window, int width, int height);

private:
	Window m_window;
	int m_windowWidth;
	int m_windowHeight;
};

OCF_END
