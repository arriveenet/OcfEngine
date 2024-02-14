#pragma once
#include <GLFW/glfw3.h>

class Window {
public:
	Window();
	virtual ~Window();

	bool create(int width, int height, const char* title);
	void destroy();

	void setCallback();
	bool shouldClose();
	void pollEvents();
	void swapBuffers();

private:
	static void onKeyEvent(GLFWwindow* window, int key, int scancode, int action, int mods);

private:
	GLFWwindow* m_pWindow;
};