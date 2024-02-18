#pragma once
#include <Windows.h>
#include <GLFW/glfw3.h>

class Window {
public:
	Window();
	virtual ~Window();

	bool create(int width, int height, const char* title);
	void destroy();
	void close();

	void setWindowPos(int posX, int posY);
	void setCallback();
	bool windowShouldClose();
	void pollEvents();
	void swapBuffers();

private:
	GLFWwindow* m_pWindow;
};